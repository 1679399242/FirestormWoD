#include "GarrisonMgr.hpp"
#include "../../scripts/Draenor/Garrison/GarrisonScriptData.hpp"
#include "Player.h"
#include "DatabaseEnv.h"
#include "ObjectMgr.h"
#include "ObjectAccessor.h"
#include "CreatureAI.h"

namespace MS { namespace Garrison 
{
    /// Constructor
    Manager::Manager(Player * p_Owner)
        : m_Owner(p_Owner)
    {
        m_GarrisonLevel     = 1;
        m_GarrisonLevelID   = 0;
        m_GarrisonSiteID    = 0;

        m_NumFollowerActivation                = 1;
        m_NumFollowerActivationRegenTimestamp  = time(0);
        m_CacheLastUsage                       = time(0);
        m_MissionDistributionLastUpdate        = time(0);

        m_CacheGameObjectGUID = 0;

        m_CacheLastTokenAmount = 0;

        m_GarrisonScript = nullptr;

        /// Select Garrison site ID
        switch (GetGarrisonFactionIndex())
        {
            case FactionIndex::Alliance:
                m_GarrisonSiteID = 2;
                break;

            case FactionIndex::Horde:
                m_GarrisonSiteID = 71;
                break;

            case FactionIndex::Max:
            default:
                ASSERT(false);
                break;

        }
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Create the garrison
    void Manager::Create()
    {
        /// At creation, the garrison cache contains 50 token
        m_CacheLastUsage = time(0) - (200 * Globals::CacheTokenGenerateTime);

        std::ostringstream l_KnownBluePrintsStr;

        for (uint32 l_I = 0; l_I < m_KnownBlueprints.size(); ++l_I)
            l_KnownBluePrintsStr << m_KnownBlueprints[l_I] << ' ';

        std::ostringstream l_KnownSpecializationsStr;

        for (uint32 l_I = 0; l_I < m_KnownSpecializations.size(); ++l_I)
            l_KnownSpecializationsStr << m_KnownSpecializations[l_I] << ' ';

        PreparedStatement* l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_GARRISON);

        m_ID = sObjectMgr->GetNewGarrisonID();

        uint32 l_Index = 0;
        l_Stmt->setUInt32(l_Index++, m_ID);
        l_Stmt->setUInt32(l_Index++, m_Owner->GetGUIDLow());
        l_Stmt->setUInt32(l_Index++, m_GarrisonLevel);
        l_Stmt->setString(l_Index++, l_KnownBluePrintsStr.str());
        l_Stmt->setString(l_Index++, l_KnownSpecializationsStr.str());
        l_Stmt->setUInt32(l_Index++, m_NumFollowerActivation);
        l_Stmt->setUInt32(l_Index++, m_NumFollowerActivationRegenTimestamp);
        l_Stmt->setUInt32(l_Index++, m_CacheLastUsage);

        CharacterDatabase.AsyncQuery(l_Stmt);

        Init();

        /// Force mission distribution update
        m_MissionDistributionLastUpdate = 0;
    
        std::vector<uint32> l_FollowerQuests = sObjectMgr->FollowerQuests;

        /// Quest non rewarded followers
        for (uint32 l_QuestID : l_FollowerQuests)
        {
            if (m_Owner->GetQuestStatus(l_QuestID) == QUEST_STATUS_REWARDED)
            {
                const Quest         * l_QuestTemplate   = sObjectMgr->GetQuestTemplate(l_QuestID);
                const SpellInfo     * l_SpellInfo       = sSpellMgr->GetSpellInfo(l_QuestTemplate->GetRewSpellCast());

                if (GetFollower(l_SpellInfo->Effects[EFFECT_0].MiscValue) == nullptr)
                    AddFollower(l_SpellInfo->Effects[EFFECT_0].MiscValue);
            }
        }

        /// Garrison ability
        if (!m_Owner->HasSpell(GARRISON_SPELL_GARR_ABILITY))
            m_Owner->learnSpell(GARRISON_SPELL_GARR_ABILITY, false);

        uint32 l_QuestEntry = 0;

        /// Select Garrison site ID
        switch (GetGarrisonFactionIndex())
        {
            case FactionIndex::Alliance:
                l_QuestEntry = BaseQuests::FoundedGarrisonA;
                break;

            case FactionIndex::Horde:
                l_QuestEntry = BaseQuests::FoundedGarrisonH;
                break;

            case FactionIndex::Max:
            default:
                ASSERT(false);
                break;

        }

        /// Minimap icons are completed quest, reason => BLIZZARD !!!!!!!!!!!!!!
        if (m_Owner->GetQuestStatus(l_QuestEntry) != QUEST_STATUS_REWARDED)
        {
            if (const Quest * l_Quest = sObjectMgr->GetQuestTemplate(l_QuestEntry))
            {
                m_Owner->AddQuest(l_Quest, m_Owner);
                m_Owner->CompleteQuest(l_QuestEntry);
            }
        }

        sGarrisonBuildingManager->LearnAllowedBuildings(m_Owner, this);

        /// Learn garrison taxinodes
        if (m_Owner->getFaction() == HORDE)
            m_Owner->m_taxi.SetTaximaskNode(TaxiNodes::Horde);
        else
            m_Owner->m_taxi.SetTaximaskNode(TaxiNodes::Alliance);
    }

    /// Load
    bool Manager::Load(PreparedQueryResult p_GarrisonResult, PreparedQueryResult p_BuildingsResult, PreparedQueryResult p_FollowersResult, PreparedQueryResult p_MissionsResult, PreparedQueryResult p_WorkOrderResult)
    {
        if (p_GarrisonResult)
        {
            Field * l_Fields = p_GarrisonResult->Fetch();

            m_ID            = l_Fields[0].GetUInt32();
            m_GarrisonLevel = l_Fields[1].GetUInt32();

            if (!l_Fields[2].GetString().empty())
            {
                Tokenizer l_BluePrints(l_Fields[2].GetString(), ' ');

                for (Tokenizer::const_iterator l_It = l_BluePrints.begin(); l_It != l_BluePrints.end(); ++l_It)
                    m_KnownBlueprints.push_back(atol(*l_It));
            }

            if (!l_Fields[3].GetString().empty())
            {
                Tokenizer l_Specializations(l_Fields[3].GetString(), ' ');

                for (Tokenizer::const_iterator l_It = l_Specializations.begin(); l_It != l_Specializations.end(); ++l_It)
                    m_KnownSpecializations.push_back(atol(*l_It));
            }

            m_NumFollowerActivation                = l_Fields[4].GetUInt32();
            m_NumFollowerActivationRegenTimestamp  = l_Fields[5].GetUInt32();
            m_CacheLastUsage                       = l_Fields[6].GetUInt32();

            if (p_BuildingsResult)
            {
                do
                {
                    l_Fields = p_BuildingsResult->Fetch();

                    GarrisonBuilding l_Building;
                    l_Building.DatabaseID       = l_Fields[0].GetUInt32();
                    l_Building.PlotInstanceID   = l_Fields[1].GetUInt32();
                    l_Building.BuildingID       = l_Fields[2].GetUInt32();
                    l_Building.SpecID           = l_Fields[3].GetUInt32();
                    l_Building.TimeBuiltStart   = l_Fields[4].GetUInt32();
                    l_Building.TimeBuiltEnd     = l_Fields[5].GetUInt32();
                    l_Building.Active           = l_Fields[6].GetBool();
                    l_Building.GatheringData    = l_Fields[7].GetString();

                    if (!l_Building.Active && time(0) > l_Building.TimeBuiltEnd)
                        l_Building.BuiltNotified = true;    ///< Auto notify by info packet

                    m_Buildings.push_back(l_Building);
                } while (p_BuildingsResult->NextRow());
            }

            if (p_MissionsResult)
            {
                do
                {
                    l_Fields = p_MissionsResult->Fetch();

                    GarrisonMission l_Mission;
                    l_Mission.DatabaseID        = l_Fields[0].GetUInt32();
                    l_Mission.MissionID         = l_Fields[1].GetUInt32();
                    l_Mission.OfferTime         = l_Fields[2].GetUInt32();
                    l_Mission.OfferMaxDuration  = l_Fields[3].GetUInt32();
                    l_Mission.StartTime         = l_Fields[4].GetUInt32();
                    l_Mission.State             = (MissionStates::Type)l_Fields[5].GetUInt32();

                    if ((l_Mission.OfferTime + l_Mission.OfferMaxDuration) > time(0) || l_Mission.State == MissionStates::InProgress)
                        m_Missions.push_back(l_Mission);
                    else
                    {
                        PreparedStatement * l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GARRISON_MISSION);
                        l_Stmt->setUInt32(0, l_Mission.DatabaseID);
                        CharacterDatabase.AsyncQuery(l_Stmt);
                    }

                } while (p_MissionsResult->NextRow());
            }

            if (p_FollowersResult)
            {
                do
                {
                    l_Fields = p_FollowersResult->Fetch();

                    GarrisonFollower l_Follower;
                    l_Follower.DatabaseID        = l_Fields[0].GetUInt32();
                    l_Follower.FollowerID        = l_Fields[1].GetUInt32();
                    l_Follower.Level             = l_Fields[2].GetUInt32();
                    l_Follower.XP                = l_Fields[3].GetUInt32();
                    l_Follower.Quality           = l_Fields[4].GetUInt32();
                    l_Follower.ItemLevelArmor    = l_Fields[5].GetUInt32();
                    l_Follower.ItemLevelWeapon   = l_Fields[6].GetUInt32();
                    l_Follower.CurrentMissionID  = l_Fields[7].GetUInt32();
                    l_Follower.CurrentBuildingID = l_Fields[8].GetUInt32();

                    if (!l_Fields[9].GetString().empty())
                    {
                        Tokenizer l_Abilities(l_Fields[9].GetString(), ' ');

                        for (Tokenizer::const_iterator l_It = l_Abilities.begin(); l_It != l_Abilities.end(); ++l_It)
                            l_Follower.Abilities.push_back(atol(*l_It));
                    }

                    l_Follower.Flags = l_Fields[10].GetUInt32();

                    m_Followers.push_back(l_Follower);

                } while (p_FollowersResult->NextRow());
            }

            if (p_WorkOrderResult)
            {
                do
                {
                    l_Fields = p_WorkOrderResult->Fetch();

                    GarrisonWorkOrder l_Order;
                    l_Order.DatabaseID        = l_Fields[0].GetUInt32();
                    l_Order.PlotInstanceID    = l_Fields[1].GetUInt32();
                    l_Order.ShipmentID        = l_Fields[2].GetUInt32();
                    l_Order.CreationTime      = l_Fields[3].GetUInt32();
                    l_Order.CompleteTime      = l_Fields[4].GetUInt32();

                    m_WorkOrders.push_back(l_Order);

                } while (p_WorkOrderResult->NextRow());
            }

            Init();

            if (!GetGarrisonSiteLevelEntry())
                return false;

            /// Remove doubloon mission
            std::map<uint32, uint32> l_MissionToRemoveCount;
            for (uint32 l_I = 0; l_I < m_Missions.size(); ++l_I)
            {
                GarrisonMission & l_Mission = m_Missions[l_I];

                uint32 l_Count = std::count_if(m_Missions.begin(), m_Missions.end(), [l_Mission](const GarrisonMission & p_Mission)
                {
                    return p_Mission.MissionID == l_Mission.MissionID;
                });

                l_MissionToRemoveCount[l_Mission.MissionID] = l_Count - 1;
            }

            std::vector<uint64> l_MissionToRemove;
            for (uint32 l_I = 0; l_I < m_Missions.size(); ++l_I)
            {
                GarrisonMission & l_Mission = m_Missions[l_I];

                if (l_MissionToRemoveCount[l_Mission.MissionID] > 0)
                {
                    l_MissionToRemove.push_back(l_Mission.DatabaseID);
                    l_MissionToRemoveCount[l_Mission.MissionID]--;
                }
            }

            for (uint64 l_MissionBD_ID : l_MissionToRemove)
            {
                auto l_It = std::find_if(m_Missions.begin(), m_Missions.end(), [l_MissionBD_ID](const GarrisonMission & p_Mission)
                {
                    return p_Mission.DatabaseID == l_MissionBD_ID;
                });

                if (l_It != m_Missions.end())
                {
                    PreparedStatement* l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GARRISON_MISSION);
                    l_Stmt->setUInt32(0, l_MissionBD_ID);

                    CharacterDatabase.AsyncQuery(l_Stmt);

                    m_Missions.erase(l_It);
                }
            }

            l_MissionToRemove.clear();

            for (uint32 l_I = 0; l_I < m_Missions.size(); ++l_I)
            {
                GarrisonMission & l_Mission = m_Missions[l_I];

                if (l_Mission.State != MissionStates::InProgress)
                    continue;

                uint32 l_FollowerCount = std::count_if(m_Followers.begin(), m_Followers.end(), [l_Mission](const GarrisonFollower & p_Follower) -> bool
                {
                    if (p_Follower.CurrentMissionID == l_Mission.MissionID)
                        return true;

                    return false;
                });

                if (l_FollowerCount == 0)
                {
                    l_MissionToRemove.push_back(l_Mission.DatabaseID);
                    continue;
                }

                GarrMissionEntry const* l_MissionTemplate = sGarrMissionStore.LookupEntry(l_Mission.MissionID);

                if (!l_MissionTemplate || l_MissionTemplate->RequiredFollowersCount != l_FollowerCount)
                    l_MissionToRemove.push_back(l_Mission.DatabaseID);
            }

            for (uint64 l_MissionBD_ID : l_MissionToRemove)
            {
                auto l_It = std::find_if(m_Missions.begin(), m_Missions.end(), [l_MissionBD_ID](const GarrisonMission & p_Mission)
                {
                    return p_Mission.DatabaseID == l_MissionBD_ID;
                });

                if (l_It != m_Missions.end())
                {
                    PreparedStatement* l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GARRISON_MISSION);
                    l_Stmt->setUInt32(0, l_MissionBD_ID);

                    CharacterDatabase.AsyncQuery(l_Stmt);

                    m_Missions.erase(l_It);
                }
            }

            std::vector<uint32> l_FollowerQuests = sObjectMgr->FollowerQuests;

            /// Quest non rewarded followers
            for (uint32 l_QuestID : l_FollowerQuests)
            {
                if (m_Owner->GetQuestStatus(l_QuestID) == QUEST_STATUS_REWARDED)
                {
                    const Quest         * l_QuestTemplate = sObjectMgr->GetQuestTemplate(l_QuestID);
                    const SpellInfo     * l_SpellInfo = sSpellMgr->GetSpellInfo(l_QuestTemplate->GetRewSpellCast());

                    if (GetFollower(l_SpellInfo->Effects[EFFECT_0].MiscValue) == nullptr)
                        AddFollower(l_SpellInfo->Effects[EFFECT_0].MiscValue);
                }
            }

            /// Force mission distribution update
            m_MissionDistributionLastUpdate = 0;

            /// Fix bug in mission distribution TEMP CODE
            uint32 l_MaxMissionCount            = ceil(m_Followers.size() * GARRISON_MISSION_DISTRIB_FOLLOWER_COEFF);
            uint32 l_CurrentAvailableMission    = 0;

            std::for_each(m_Missions.begin(), m_Missions.end(), [&l_CurrentAvailableMission](const GarrisonMission & p_Mission) -> void
            {
                if (p_Mission.State == MissionStates::Available && (p_Mission.OfferTime + p_Mission.OfferMaxDuration) > time(0))
                    l_CurrentAvailableMission++;
            });

            if (l_CurrentAvailableMission > l_MaxMissionCount)
            {
                m_Missions.erase(std::remove_if(m_Missions.begin(), m_Missions.end(), [l_CurrentAvailableMission, l_MaxMissionCount](const GarrisonMission & p_Mission) -> bool
                {
                    if (p_Mission.State == MissionStates::Available && (p_Mission.OfferTime + p_Mission.OfferMaxDuration) > time(0) && l_CurrentAvailableMission > l_MaxMissionCount)
                        return true;

                    return false;
                }), m_Missions.end());
            }

            /// Unstuck follower
            std::for_each(m_Followers.begin(), m_Followers.end(), [this](GarrisonFollower & p_Follower)
            {
                if (p_Follower.CurrentMissionID != 0)
                {
                    auto l_It = std::find_if(m_Missions.begin(), m_Missions.end(), [p_Follower](const GarrisonMission & p_Mission) -> bool
                    {
                        if (p_Mission.MissionID == p_Follower.CurrentMissionID)
                            return true;

                        return false;
                    });

                    if (l_It == m_Missions.end() || (*l_It).State == MissionStates::Available)
                        p_Follower.CurrentMissionID = 0;
                }

                /// Corrupted DB
                if (p_Follower.Level == 0)
                {
                    GarrFollowerEntry const* l_Entry = sGarrFollowerStore.LookupEntry(p_Follower.FollowerID);

                    if (l_Entry)
                    {
                        p_Follower.Level            = l_Entry->Level;
                        p_Follower.XP               = 0;
                        p_Follower.Quality          = l_Entry->Quality;
                        p_Follower.ItemLevelArmor   = l_Entry->ItemLevelArmor;
                        p_Follower.ItemLevelWeapon  = l_Entry->ItemLevelWeapon;

                        p_Follower.Abilities.clear();

                        for (uint32 l_I = 0; l_I < sGarrFollowerXAbilityStore.GetNumRows(); ++l_I)
                        {
                            GarrFollowerXAbilityEntry const* l_Entry = sGarrFollowerXAbilityStore.LookupEntry(l_I);

                            if (l_Entry && l_Entry->FollowerID == p_Follower.FollowerID && sGarrAbilityStore.LookupEntry(l_Entry->AbilityID) && l_Entry->FactionIndex == GetGarrisonFactionIndex())
                                p_Follower.Abilities.push_back(l_Entry->AbilityID);
                        }
                    }
                }
            });

            /// Garrison ability
            if (!m_Owner->HasSpell(GARRISON_SPELL_GARR_ABILITY))
                m_Owner->learnSpell(GARRISON_SPELL_GARR_ABILITY, false);

            uint32 l_QuestEntry = 0;
            /// Select Garrison site ID
            switch (GetGarrisonFactionIndex())
            {
                case FactionIndex::Alliance:
                    l_QuestEntry = BaseQuests::FoundedGarrisonA;
                    break;

                case FactionIndex::Horde:
                    l_QuestEntry = BaseQuests::FoundedGarrisonH;
                    break;

                case FactionIndex::Max:
                default:
                    ASSERT(false);
                    break;

            }

            /// Minimap icons are completed quest, reason => BLIZZARD !!!!!!!!!!!!!!
            if (m_Owner->GetQuestStatus(l_QuestEntry) != QUEST_STATUS_REWARDED)
            {
                if (const Quest * l_Quest = sObjectMgr->GetQuestTemplate(l_QuestEntry))
                {
                    m_Owner->AddQuest(l_Quest, m_Owner);
                    m_Owner->CompleteQuest(l_QuestEntry);
                }
            }

            sGarrisonBuildingManager->LearnAllowedBuildings(m_Owner, this);

            return true;
        }

        return false;
    }

    /// Save this garrison to DB
    void Manager::Save()
    {
        SQLTransaction l_GarrisonTransaction = CharacterDatabase.BeginTransaction();

        std::ostringstream l_KnownBluePrintsStr;

        for (uint32 l_I = 0; l_I < m_KnownBlueprints.size(); ++l_I)
            l_KnownBluePrintsStr << m_KnownBlueprints[l_I] << ' ';

        std::ostringstream l_KnownSpecializationsStr;

        for (uint32 l_I = 0; l_I < m_KnownSpecializations.size(); ++l_I)
            l_KnownSpecializationsStr << m_KnownSpecializations[l_I] << ' ';

        PreparedStatement* l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_GARRISON);

        std::string l_KnownBluePrints = l_KnownBluePrintsStr.str();
        if (l_KnownBluePrints.empty())
            l_KnownBluePrints = " ";

        std::string l_KnowSpecializations = l_KnownSpecializationsStr.str();
        if (l_KnowSpecializations.empty())
            l_KnowSpecializations = " ";

        uint32 l_Index = 0;
        l_Stmt->setUInt32(l_Index++, m_GarrisonLevel);
        l_Stmt->setString(l_Index++, l_KnownBluePrints);
        l_Stmt->setString(l_Index++, l_KnowSpecializations);
        l_Stmt->setUInt32(l_Index++, m_NumFollowerActivation);
        l_Stmt->setUInt32(l_Index++, m_NumFollowerActivationRegenTimestamp);
        l_Stmt->setUInt32(l_Index++, m_CacheLastUsage);
        l_Stmt->setUInt32(l_Index++, m_ID);
        
        l_GarrisonTransaction->Append(l_Stmt);

        for (uint32 l_I = 0; l_I < m_Buildings.size(); ++l_I)
        {
            PreparedStatement* l_BuildingStatement = CharacterDatabase.GetPreparedStatement(CHAR_UPD_GARRISON_BUILDING);

            l_Index = 0;
            l_BuildingStatement->setUInt32(l_Index++, m_Buildings[l_I].PlotInstanceID);
            l_BuildingStatement->setUInt32(l_Index++, m_Buildings[l_I].BuildingID);
            l_BuildingStatement->setUInt32(l_Index++, m_Buildings[l_I].SpecID);
            l_BuildingStatement->setUInt32(l_Index++, m_Buildings[l_I].TimeBuiltStart);
            l_BuildingStatement->setUInt32(l_Index++, m_Buildings[l_I].TimeBuiltEnd);
            l_BuildingStatement->setBool  (l_Index++, m_Buildings[l_I].Active);
            l_BuildingStatement->setString(l_Index++, m_Buildings[l_I].GatheringData);
            l_BuildingStatement->setUInt32(l_Index++, m_Buildings[l_I].DatabaseID);
            l_BuildingStatement->setUInt32(l_Index++, m_ID);

            l_GarrisonTransaction->Append(l_BuildingStatement);
        }

        for (uint32 l_I = 0; l_I < m_Missions.size(); ++l_I)
        {
            if ((m_Missions[l_I].OfferTime + m_Missions[l_I].OfferMaxDuration) > time(0) || m_Missions[l_I].State == MissionStates::InProgress || m_Missions[l_I].State == MissionStates::CompleteSuccess)
            {
                PreparedStatement* l_MissionStmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_GARRISON_MISSION);

                l_Index = 0;
                l_MissionStmt->setUInt32(l_Index++, m_Missions[l_I].MissionID);
                l_MissionStmt->setUInt32(l_Index++, m_Missions[l_I].OfferTime);
                l_MissionStmt->setUInt32(l_Index++, m_Missions[l_I].OfferMaxDuration);
                l_MissionStmt->setUInt32(l_Index++, m_Missions[l_I].StartTime);
                l_MissionStmt->setUInt32(l_Index++, m_Missions[l_I].State);

                l_MissionStmt->setUInt32(l_Index++, m_Missions[l_I].DatabaseID);
                l_MissionStmt->setUInt32(l_Index++, m_ID);

                l_GarrisonTransaction->Append(l_MissionStmt);
            }
            else
            {
                PreparedStatement * l_MissionStmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GARRISON_MISSION);
                l_MissionStmt->setUInt32(0, m_Missions[l_I].DatabaseID);
                l_GarrisonTransaction->Append(l_MissionStmt);
            }
        }

        for (uint32 l_I = 0; l_I < m_Followers.size(); ++l_I)
        {
            std::ostringstream l_Abilities;

            for (uint32 l_Y = 0; l_Y < m_Followers[l_I].Abilities.size(); ++l_Y)
                l_Abilities << m_Followers[l_I].Abilities[l_Y] << ' ';

            PreparedStatement* l_FollowerStmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_GARRISON_FOLLOWER);

            l_Index = 0;
            l_FollowerStmt->setUInt32(l_Index++, m_Followers[l_I].FollowerID);
            l_FollowerStmt->setUInt32(l_Index++, m_Followers[l_I].Level);
            l_FollowerStmt->setUInt32(l_Index++, m_Followers[l_I].XP);
            l_FollowerStmt->setUInt32(l_Index++, m_Followers[l_I].Quality);
            l_FollowerStmt->setUInt32(l_Index++, m_Followers[l_I].ItemLevelArmor);
            l_FollowerStmt->setUInt32(l_Index++, m_Followers[l_I].ItemLevelWeapon);
            l_FollowerStmt->setUInt32(l_Index++, m_Followers[l_I].CurrentMissionID);
            l_FollowerStmt->setUInt32(l_Index++, m_Followers[l_I].CurrentBuildingID);
            l_FollowerStmt->setString(l_Index++, l_Abilities.str());
            l_FollowerStmt->setUInt32(l_Index++, m_Followers[l_I].Flags);
            l_FollowerStmt->setUInt32(l_Index++, m_Followers[l_I].DatabaseID);
            l_FollowerStmt->setUInt32(l_Index++, m_ID);

            l_GarrisonTransaction->Append(l_FollowerStmt);
        }

        CharacterDatabase.CommitTransaction(l_GarrisonTransaction);
    }

    /// Update Follower in database
    void Manager::SaveFollowersToDB(uint32 p_Entry /*= 0*/)
    {
        SQLTransaction l_GarrisonTransaction = CharacterDatabase.BeginTransaction();

        for (uint32 l_I = 0; l_I < m_Followers.size(); ++l_I)
        {
            /// Save only one if specified
            if (p_Entry && p_Entry != m_Followers[l_I].FollowerID)
                continue;

            std::ostringstream l_Abilities;

            for (uint32 l_Y = 0; l_Y < m_Followers[l_I].Abilities.size(); ++l_Y)
                l_Abilities << m_Followers[l_I].Abilities[l_Y] << ' ';

            PreparedStatement* l_FollowerStmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_GARRISON_FOLLOWER);

            uint8 l_Index = 0;
            l_FollowerStmt->setUInt32(l_Index++, m_Followers[l_I].FollowerID);
            l_FollowerStmt->setUInt32(l_Index++, m_Followers[l_I].Level);
            l_FollowerStmt->setUInt32(l_Index++, m_Followers[l_I].XP);
            l_FollowerStmt->setUInt32(l_Index++, m_Followers[l_I].Quality);
            l_FollowerStmt->setUInt32(l_Index++, m_Followers[l_I].ItemLevelArmor);
            l_FollowerStmt->setUInt32(l_Index++, m_Followers[l_I].ItemLevelWeapon);
            l_FollowerStmt->setUInt32(l_Index++, m_Followers[l_I].CurrentMissionID);
            l_FollowerStmt->setUInt32(l_Index++, m_Followers[l_I].CurrentBuildingID);
            l_FollowerStmt->setString(l_Index++, l_Abilities.str());
            l_FollowerStmt->setUInt32(l_Index++, m_Followers[l_I].Flags);
            l_FollowerStmt->setUInt32(l_Index++, m_Followers[l_I].DatabaseID);
            l_FollowerStmt->setUInt32(l_Index++, m_ID);

            l_GarrisonTransaction->Append(l_FollowerStmt);
        }

        CharacterDatabase.CommitTransaction(l_GarrisonTransaction);
    }

    /// Delete garrison
    void Manager::DeleteFromDB(uint64 p_PlayerGUID, SQLTransaction p_Transation)
    {
        PreparedStatement* l_Stmt;

        l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GARRISON_BUILDINGS);
        l_Stmt->setUInt32(0, p_PlayerGUID);
        p_Transation->Append(l_Stmt);

        l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GARRISON_MISSIONS);
        l_Stmt->setUInt32(0, p_PlayerGUID);
        p_Transation->Append(l_Stmt);

        l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GARRISON_FOLLOWERS);
        l_Stmt->setUInt32(0, p_PlayerGUID);
        p_Transation->Append(l_Stmt);

        l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GARRISON_WORKORDERS);
        l_Stmt->setUInt32(0, p_PlayerGUID);
        p_Transation->Append(l_Stmt);

        l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GARRISON);
        l_Stmt->setUInt32(0, p_PlayerGUID);
        p_Transation->Append(l_Stmt);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Update the garrison
    void Manager::Update()
    {
        /// Update buildings
        UpdateBuildings();
        /// Update followers
        UpdateFollowers();
        /// Update cache
        UpdateCache();
        /// Update mission distribution
        UpdateMissionDistribution();
        /// Update garrison ability
        UpdateGarrisonAbility();
        /// Update work order
        UpdateWorkOrders();
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Set garrison level
    void Manager::SetLevel(uint32 p_Level)
    {
        m_GarrisonLevel = p_Level;

        UninitPlots();
        Init();
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Get garrison cache token count
    uint32 Manager::GetGarrisonCacheTokenCount() const
    {
        return m_CacheLastTokenAmount;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Get terrain swaps
    void Manager::GetTerrainSwaps(std::set<uint32> & p_TerrainSwaps) const
    {
        if (!GetGarrisonSiteLevelEntry())
            return;

        if (GetGarrisonFactionIndex() == FactionIndex::Horde)
        {
            switch (GetGarrisonSiteLevelEntry()->Level)
            {
                case 1:
                    p_TerrainSwaps.emplace(TERRAIN_SWAP_GARRISON_FF_HORDE_TIER_1);
                    break;

                case 2:
                    p_TerrainSwaps.emplace(TERRAIN_SWAP_GARRISON_FF_HORDE_TIER_2);
                    break;

                case 3:
                    p_TerrainSwaps.emplace(TERRAIN_SWAP_GARRISON_FF_HORDE_TIER_3);
                    break;
            }
        }
        else
        {
            switch (GetGarrisonSiteLevelEntry()->Level)
            {
                case 1:
                    p_TerrainSwaps.emplace(TERRAIN_SWAP_GARRISON_SMV_ALLIANCE_TIER_1);
                    break;

                case 2:
                    p_TerrainSwaps.emplace(TERRAIN_SWAP_GARRISON_SMV_ALLIANCE_TIER_2);
                    break;

                case 3:
                    p_TerrainSwaps.emplace(TERRAIN_SWAP_GARRISON_SMV_ALLIANCE_TIER_3);
                    break;
            }
        }
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Get garrison script
    Interfaces::GarrisonSite * Manager::GetGarrisonScript() const
    {
        return m_GarrisonScript;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Can upgrade the garrison
    bool Manager::CanUpgrade() const
    {
        if (!m_Owner->HasCurrency(Globals::CurrencyID, GetGarrisonSiteLevelEntry()->UpgradeCost))
            return false;

        if (!m_Owner->HasEnoughMoney((int64)GetGarrisonSiteLevelEntry()->UpgradeMoneyCost))
            return false;

        Interfaces::GarrisonSite * l_GarrisonScript = GetGarrisonScript();

        if (l_GarrisonScript)
            return l_GarrisonScript->CanUpgrade(m_Owner, m_GarrisonLevel);

        return false;
    }

    /// Upgrade the garrison
    void Manager::Upgrade()
    {
        m_Owner->ModifyCurrency(Globals::CurrencyID, -((int32)GetGarrisonSiteLevelEntry()->UpgradeCost));
        m_Owner->ModifyMoney(-((int64)GetGarrisonSiteLevelEntry()->UpgradeMoneyCost));

        SetLevel(m_GarrisonLevel + 1);

        Interfaces::GarrisonSite * l_GarrisonScript = GetGarrisonScript();

        if (l_GarrisonScript)
            l_GarrisonScript->OnUpgrade(m_Owner);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Reward garrison cache content
    void Manager::RewardGarrisonCache()
    {
        m_Owner->SendDisplayToast(Globals::CurrencyID, m_CacheLastTokenAmount, DISPLAY_TOAST_METHOD_GARRISON_CACHE, TOAST_TYPE_NEW_CURRENCY, false, false);
        m_Owner->ModifyCurrency(Globals::CurrencyID, m_CacheLastTokenAmount);

        m_CacheLastTokenAmount  = 0;
        m_CacheLastUsage        = time(0);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// When the garrison owner enter in the garrisson (@See Player::UpdateArea)
    void Manager::OnPlayerEnter()
    {
        InitPlots();    ///< AKA update plots

        Interfaces::GarrisonSite * l_GarrisonScript = GetGarrisonScript();

        if (l_GarrisonScript)
            m_Owner->SetPhaseMask(l_GarrisonScript->GetPhaseMask(m_Owner), true);

        /// Enable AI Client collision manager
        m_Owner->SetFlag(UNIT_FIELD_NPC_FLAGS + 1, UNIT_NPC_FLAG2_AI_OBSTACLE);

        for (std::map<uint32, uint64>::iterator l_It = m_PlotsActivateGob.begin(); l_It != m_PlotsActivateGob.end(); ++l_It)
        {
            if (GameObject * l_Gob = HashMapHolder<GameObject>::Find(l_It->second))
                l_Gob->SendGameObjectActivateAnimKit(1696);
        }
    }

    /// When the garrison owner leave the garrisson (@See Player::UpdateArea)
    void Manager::OnPlayerLeave()
    {
        UninitPlots();

        if (m_CacheGameObjectGUID)
        {
            GameObject * l_Cache = HashMapHolder<GameObject>::Find(m_CacheGameObjectGUID);

            if (l_Cache)
            {
                l_Cache->DestroyForNearbyPlayers();
                l_Cache->AddObjectToRemoveList();
            }

            m_CacheGameObjectGUID = 0;
        }

        m_Owner->SetPhaseMask(1, true);

        /// Disable AI Client collision manager
        m_Owner->RemoveFlag(UNIT_FIELD_NPC_FLAGS + 1, UNIT_NPC_FLAG2_AI_OBSTACLE);
    }

    /// When the garrison owner started a quest
    void Manager::OnQuestStarted(const Quest * p_Quest)
    {
        Interfaces::GarrisonSite * l_GarrisonScript = GetGarrisonScript();

        if (l_GarrisonScript)
        {
            /// Broadcast event
            l_GarrisonScript->OnQuestStarted(m_Owner, p_Quest);
            /// Update phasing
            m_Owner->SetPhaseMask(l_GarrisonScript->GetPhaseMask(m_Owner), true);
        }
    }

    /// When the garrison owner reward a quest
    void Manager::OnQuestReward(const Quest * p_Quest)
    {
        Interfaces::GarrisonSite * l_GarrisonScript = GetGarrisonScript();

        if (l_GarrisonScript)
        {
            /// Broadcast event
            l_GarrisonScript->OnQuestReward(m_Owner, p_Quest);
            /// Update phasing
            m_Owner->SetPhaseMask(l_GarrisonScript->GetPhaseMask(m_Owner), true);
        }
    }

    /// When the garrison owner abandon a quest
    void Manager::OnQuestAbandon(const Quest * p_Quest)
    {
        Interfaces::GarrisonSite * l_GarrisonScript = GetGarrisonScript();

        if (l_GarrisonScript)
        {
            /// Broadcast event
            l_GarrisonScript->OnQuestAbandon(m_Owner, p_Quest);
            /// Update phasing
            m_Owner->SetPhaseMask(l_GarrisonScript->GetPhaseMask(m_Owner), true);
        }
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// When the owner player change level
    /// @p_Level : New owner level
    void Manager::OnOwnerLevelChange(uint32 p_Level)
    {
        Interfaces::GarrisonSite * l_GarrisonScript = GetGarrisonScript();

        if (l_GarrisonScript)
        {
            /// Broadcast event
            l_GarrisonScript->OnOwnerLevelChange(p_Level);
        }
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// set last used activation game  object
    void Manager::SetLastUsedActivationGameObject(uint64 p_Guid)
    {
        m_LastUsedActivationGameObject = p_Guid;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Get GarrSiteLevelEntry for current garrison
    const GarrSiteLevelEntry * Manager::GetGarrisonSiteLevelEntry() const
    {
        return sGarrSiteLevelStore.LookupEntry(m_GarrisonLevelID);
    }

    /// Get Garrison Faction Index
    FactionIndex::Type Manager::GetGarrisonFactionIndex() const
    {
        assert(m_Owner);

        switch (m_Owner->GetTeam())
        {
            case ALLIANCE:
                return FactionIndex::Alliance;

            case HORDE:
                return FactionIndex::Horde;
        }

        return FactionIndex::Horde;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Get slots for level
    std::vector<GarrisonPlotInstanceInfoLocation> Manager::GetPlots()
    {
        return m_Plots;
    }

    /// Get plot by position
    GarrisonPlotInstanceInfoLocation Manager::GetPlot(float p_X, float p_Y, float p_Z)
    {
        Position                            l_Position;
        GarrisonPlotInstanceInfoLocation    l_Plot;

        memset(&l_Plot, 0, sizeof(GarrisonPlotInstanceInfoLocation));

        l_Position.m_positionX = p_X;
        l_Position.m_positionY = p_Y;
        l_Position.m_positionZ = p_Z;

        for (uint32 l_I = 0; l_I < m_Plots.size(); ++l_I)
        {
            if (l_Position.GetExactDist2d(m_Plots[l_I].X, m_Plots[l_I].Y) < l_Position.GetExactDist2d(l_Plot.X, l_Plot.Y))
            {
                /// Specific check for mine, plot surface is way bigger than other plots
                if (GetBuilding(m_Plots[l_I].PlotInstanceID).BuildingID == 61 || l_Position.GetExactDist2d(m_Plots[l_I].X, m_Plots[l_I].Y) < 35.0f)
                    l_Plot = m_Plots[l_I];
            }
        }

        return l_Plot;
    }

    /// Get plot instance plot type
    uint32 Manager::GetPlotType(uint32 p_PlotInstanceID) const
    {
        GarrPlotInstanceEntry const* l_PlotInstanceEntry = sGarrPlotInstanceStore.LookupEntry(p_PlotInstanceID);

        if (!l_PlotInstanceEntry)
            return 0;

        GarrPlotEntry const* l_PlotEntry = sGarrPlotStore.LookupEntry(l_PlotInstanceEntry->PlotID);

        if (!l_PlotEntry)
            return 0;

        return l_PlotEntry->PlotType;
    }

    /// Plot is free ?
    bool Manager::PlotIsFree(uint32 p_PlotInstanceID) const
    {
        for (uint32 l_I = 0; l_I < m_Buildings.size(); ++l_I)
            if (m_Buildings[l_I].PlotInstanceID == p_PlotInstanceID)
                return false;

        return true;
    }

    /// Has plot instance
    bool Manager::HasPlotInstance(uint32 p_PlotInstanceID) const
    {
        for (uint32 l_I = 0; l_I < m_Plots.size(); ++l_I)
            if (m_Plots[l_I].PlotInstanceID == p_PlotInstanceID)
                return true;

        return false;
    }

    /// Get plot location
    GarrisonPlotInstanceInfoLocation Manager::GetPlot(uint32 p_PlotInstanceID) const
    {
        for (uint32 l_I = 0; l_I < m_Plots.size(); ++l_I)
        {
            if (m_Plots[l_I].SiteLevelID == m_GarrisonLevelID && m_Plots[l_I].PlotInstanceID == p_PlotInstanceID)
                return m_Plots[l_I];
        }

        return GarrisonPlotInstanceInfoLocation();
    }

    /// Get plot instance ID by activation game object
    uint32 Manager::GetPlotInstanceIDByActivationGameObject(uint64 p_Guid) const
    {
        for (std::map<uint32, uint64>::const_iterator l_It = m_PlotsActivateGob.begin(); l_It != m_PlotsActivateGob.end(); ++l_It)
            if (l_It->second == p_Guid)
                return l_It->first;

        return 0;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Add mission
    bool Manager::AddMission(uint32 p_MissionRecID)
    {
        GarrMissionEntry const* l_MissionEntry = sGarrMissionStore.LookupEntry(p_MissionRecID);

        if (!l_MissionEntry)
            return false;

        uint32 l_Count = std::count_if(m_Missions.begin(), m_Missions.end(), [p_MissionRecID](const GarrisonMission & p_Mission)
        {
            return p_Mission.MissionID == p_MissionRecID;
        });

        if (l_Count)
            return false;

        if (l_MissionEntry->RequiredLevel > (int32)m_Owner->getLevel())
            return false;

        if (l_MissionEntry->RequiredItemLevel > (int32)m_Owner->GetAverageItemLevelEquipped())
            return false;

        GarrisonMission l_Mission;
        l_Mission.DatabaseID        = sObjectMgr->GetNewGarrisonMissionID();
        l_Mission.MissionID         = p_MissionRecID;
        l_Mission.OfferTime         = time(0);
        l_Mission.OfferMaxDuration  = l_MissionEntry->OfferTime;
        l_Mission.State             = MissionStates::Available;
        l_Mission.StartTime         = 0;

        PreparedStatement* l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_GARRISON_MISSION);

        uint32 l_Index = 0;
        l_Stmt->setUInt32(l_Index++, l_Mission.DatabaseID);
        l_Stmt->setUInt32(l_Index++, m_ID);
        l_Stmt->setUInt32(l_Index++, l_Mission.MissionID);
        l_Stmt->setUInt32(l_Index++, l_Mission.OfferTime);
        l_Stmt->setUInt32(l_Index++, l_Mission.OfferMaxDuration);
        l_Stmt->setUInt32(l_Index++, l_Mission.StartTime);
        l_Stmt->setUInt32(l_Index++, l_Mission.State);

        CharacterDatabase.AsyncQuery(l_Stmt);

        m_Missions.push_back(l_Mission);

        WorldPacket l_AddMissionResult(SMSG_GARRISON_ADD_MISSION_RESULT, 40);
        l_AddMissionResult << uint32(PurchaseBuildingResults::Ok);
        l_AddMissionResult << uint64(l_Mission.DatabaseID);
        l_AddMissionResult << uint32(l_Mission.MissionID);
        l_AddMissionResult << uint32(l_Mission.OfferTime);
        l_AddMissionResult << uint32(l_Mission.OfferMaxDuration);
        l_AddMissionResult << uint32(l_Mission.StartTime);
        l_AddMissionResult << uint32(0);   ///< Travel duration
        l_AddMissionResult << uint32(0);   ///< Mission duration
        l_AddMissionResult << uint32(l_Mission.State);

        m_Owner->SendDirectMessage(&l_AddMissionResult);

        return true;
    }

    /// Player have mission
    bool Manager::HaveMission(uint32 p_MissionRecID)  const
    {
        for (uint32 l_I = 0; l_I < m_Missions.size(); ++l_I)
        {
            if (m_Missions[l_I].MissionID == p_MissionRecID)
            {
                if (m_Missions[l_I].State < MissionStates::CompleteSuccess
                    && !((m_Missions[l_I].OfferTime + m_Missions[l_I].OfferMaxDuration) < time(0) && m_Missions[l_I].State == MissionStates::Available))
                    return true;
            }
        }

        return false;
    }

    /// Start mission
    void Manager::StartMission(uint32 p_MissionRecID, std::vector<uint64> p_Followers)
    {
        if (!HaveMission(p_MissionRecID))
        {
            StartMissionFailed(p_MissionRecID, p_Followers);
            return;
        }

        if (GetActivatedFollowerCount() > m_Stat_MaxActiveFollower)
            return;

        GarrMissionEntry const* l_MissionTemplate = sGarrMissionStore.LookupEntry(p_MissionRecID);

        if (!m_Owner->HasCurrency(Globals::CurrencyID, l_MissionTemplate->GarrisonCurrencyStartCost))
        {
            StartMissionFailed(p_MissionRecID, p_Followers);
            return;
        }

        if (p_Followers.size() < l_MissionTemplate->RequiredFollowersCount)
        {
            StartMissionFailed(p_MissionRecID, p_Followers);
            return;
        }

        for (uint32 l_I = 0; l_I < p_Followers.size(); ++l_I)
        {
            std::vector<GarrisonFollower>::iterator l_It = std::find_if(m_Followers.begin(), m_Followers.end(), [this, p_Followers, l_I](const GarrisonFollower p_Follower) -> bool
            {
                if (p_Follower.DatabaseID == p_Followers[l_I])
                    return true;

                return false;
            });

            if (l_It == m_Followers.end())
            {
                StartMissionFailed(p_MissionRecID, p_Followers);
                return;
            }

            if (l_It->CurrentBuildingID != 0 || l_It->CurrentMissionID != 0)
            {
                StartMissionFailed(p_MissionRecID, p_Followers);
                return;
            }

            /// Should not happen
            if (l_It->Flags & GARRISON_FOLLOWER_FLAG_INACTIVE)
                return;

            uint32 l_FollowerItemLevel = (l_It->ItemLevelWeapon + l_It->ItemLevelArmor) / 2;

            if ((int32)l_FollowerItemLevel < l_MissionTemplate->RequiredItemLevel)
            {
                StartMissionFailed(p_MissionRecID, p_Followers);
                return;
            }
        }

        m_Owner->ModifyCurrency(Globals::CurrencyID, -(int32)l_MissionTemplate->GarrisonCurrencyStartCost);

        std::vector<uint32> l_FollowersIDs;
        for (uint32 l_I = 0; l_I < p_Followers.size(); ++l_I)
        {
            std::vector<GarrisonFollower>::iterator l_It = std::find_if(m_Followers.begin(), m_Followers.end(), [this, p_Followers, l_I](const GarrisonFollower p_Follower) -> bool
            {
                if (p_Follower.DatabaseID == p_Followers[l_I])
                    return true;

                return false;
            });

            l_It->CurrentMissionID = p_MissionRecID;
            l_FollowersIDs.push_back(l_It->FollowerID);
        }

        GarrisonMission * l_Mission = nullptr;

        for (uint32 l_I = 0; l_I < m_Missions.size(); ++l_I)
        {
            if (m_Missions[l_I].MissionID == p_MissionRecID)
            {
                l_Mission = &m_Missions[l_I];
                break;
            }
        }

        if (l_Mission)
        {
            l_Mission->State        = MissionStates::InProgress;
            l_Mission->StartTime    = time(0);
            l_Mission->OfferTime    = time(0);

            uint32 l_TravelDuration  = GetMissionTravelDuration(p_MissionRecID);
            uint32 l_MissionDuration = GetMissionDuration(p_MissionRecID);

            WorldPacket l_Result(SMSG_GARRISON_START_MISSION_RESULT, 200);
            l_Result << uint32(0);    ///< Result (0 = OK, 1 = failed)

            l_Result << uint64(l_Mission->DatabaseID);
            l_Result << uint32(l_Mission->MissionID);
            l_Result << uint32(l_Mission->OfferTime);
            l_Result << uint32(l_Mission->OfferMaxDuration);
            l_Result << uint32(l_Mission->StartTime);
            l_Result << uint32(l_TravelDuration);
            l_Result << uint32(l_MissionDuration);
            l_Result << uint32(l_Mission->State);

            l_Result << uint32(p_Followers.size());

            for (uint32 l_I = 0; l_I < p_Followers.size(); ++l_I)
                l_Result << uint64(p_Followers[l_I]);

            m_Owner->SendDirectMessage(&l_Result);

            if (GetGarrisonScript())
                GetGarrisonScript()->OnMissionStart(m_Owner, p_MissionRecID, l_FollowersIDs);
        }
    }

    /// Send mission start failed packet
    void Manager::StartMissionFailed(uint32 p_MissionRecID, std::vector<uint64> p_Followers)
    {
        WorldPacket l_Data(SMSG_GARRISON_START_MISSION_RESULT, 200);
        l_Data << uint32(1);    ///< Result (0 = OK, 1 = failed)

        GarrMissionEntry const* l_MissionTemplate = sGarrMissionStore.LookupEntry(p_MissionRecID);
        GarrisonMission * l_Mission = nullptr;

        for (uint32 l_I = 0; l_I < m_Missions.size(); ++l_I)
        {
            if (m_Missions[l_I].MissionID == p_MissionRecID)
            {
                l_Mission = &m_Missions[l_I];
                break;
            }
        }

        if (l_Mission)
        {
            l_Data << uint64(l_Mission->DatabaseID);
            l_Data << uint32(l_Mission->MissionID);
            l_Data << uint32(l_Mission->OfferTime);
            l_Data << uint32(l_Mission->OfferMaxDuration);
            l_Data << uint32(l_Mission->StartTime);
            l_Data << uint32(0);                            ///< TravelDuration
            l_Data << uint32(l_MissionTemplate->Duration);
            l_Data << uint32(l_Mission->State);

            l_Data << uint32(p_Followers.size());
        }
        else
        {
            /// Mission details
            l_Data << uint64(0);
            l_Data << uint32(0);
            l_Data << uint32(0);
            l_Data << uint32(0);
            l_Data << uint32(0);
            l_Data << uint32(0);
            l_Data << uint32(0);
            l_Data << uint32(0);
        }

        for (uint32 l_I = 0; l_I < p_Followers.size(); ++l_I)
            l_Data << uint64(p_Followers[l_I]);

        m_Owner->SendDirectMessage(&l_Data);
    }

    /// Complete a mission
    void Manager::CompleteMission(uint32 p_MissionRecID)
    {
        if (!HaveMission(p_MissionRecID))
            return;

        GarrMissionEntry const* l_MissionTemplate = sGarrMissionStore.LookupEntry(p_MissionRecID);

        GarrisonMission * l_Mission = nullptr;

        for (uint32 l_I = 0; l_I < m_Missions.size(); ++l_I)
        {
            if (m_Missions[l_I].MissionID == p_MissionRecID)
            {
                l_Mission = &m_Missions[l_I];
                break;
            }
        }

        if (l_Mission == nullptr)
            return;

        uint32 l_TravelDuration  = GetMissionTravelDuration(p_MissionRecID);
        uint32 l_MissionDuration = GetMissionDuration(p_MissionRecID);

        std::vector<GarrisonFollower*> l_MissionFollowers = GetMissionFollowers(p_MissionRecID);

        /// Hack attempt
        if ((l_Mission->StartTime + (l_TravelDuration + l_MissionDuration)) > time(0))
            return;

        uint32 l_ChestChance = GetMissionSuccessChance(p_MissionRecID);

        bool l_Succeeded   = roll_chance_i(l_ChestChance);  ///< Seems to be MissionChance
        bool l_CanComplete = true;

        l_Mission->State = l_Succeeded ? MissionStates::CompleteSuccess : MissionStates::CompleteFailed;

        WorldPacket l_Result(SMSG_GARRISON_COMPLETE_MISSION_RESULT, 100);

        l_Result << uint32(!l_CanComplete);

        l_Result << uint64(l_Mission->DatabaseID);
        l_Result << uint32(l_Mission->MissionID);
        l_Result << uint32(l_Mission->OfferTime);
        l_Result << uint32(l_Mission->OfferMaxDuration);
        l_Result << uint32(l_Mission->StartTime);
        l_Result << uint32(l_TravelDuration);
        l_Result << uint32(l_MissionDuration);
        l_Result << uint32(l_Mission->State);

        l_Result << uint32(l_Mission->MissionID);
        l_Result.WriteBit(l_Succeeded);
        l_Result.FlushBits();

        m_Owner->SendDirectMessage(&l_Result);

        std::vector<uint32> l_PartyXPModifiersEffect = GetMissionFollowersAbilitiesEffects(p_MissionRecID, AbilityEffectTypes::ModXpGain, AbilityEffectTargetMasks::Unk | AbilityEffectTargetMasks::Party);
        std::vector<uint32> l_PassiveEffects         = GetBuildingsPassiveAbilityEffects();

        /// Global XP Bonus modifier
        float l_XPModifier = 1.0f;
        for (uint32 l_I = 0; l_I < l_PartyXPModifiersEffect.size(); ++l_I)
        {
            GarrAbilityEffectEntry const* l_AbilityEffectEntry = sGarrAbilityEffectStore.LookupEntry(l_PartyXPModifiersEffect[l_I]);

            if (!l_AbilityEffectEntry)
                continue;

            l_XPModifier = (l_AbilityEffectEntry->Amount - 1.0) + l_XPModifier;
        }

        for (uint32 l_Y = 0; l_Y < l_PassiveEffects.size(); ++l_Y)
        {
            GarrAbilityEffectEntry const* l_AbilityEffectEntry = sGarrAbilityEffectStore.LookupEntry(l_PassiveEffects[l_Y]);

            if (!l_AbilityEffectEntry)
                continue;

            if (l_AbilityEffectEntry->EffectType == AbilityEffectTypes::ModXpGain && (l_AbilityEffectEntry->TargetMask == AbilityEffectTargetMasks::Party || l_AbilityEffectEntry->TargetMask == AbilityEffectTargetMasks::Unk))
                l_XPModifier = (l_AbilityEffectEntry->Amount - 1.0) + l_XPModifier;
        }
        /// ------------------------------------------

        float l_BonusXP = (l_XPModifier - 1.0f) * l_MissionTemplate->RewardFollowerExperience;

        for (uint32 l_FollowerIt = 0; l_FollowerIt < l_MissionFollowers.size(); ++l_FollowerIt)
        {
            if (!l_MissionFollowers[l_FollowerIt]->CanXP())
                continue;

            uint32 l_FollowerLevel = l_MissionFollowers[l_FollowerIt]->Level;

            WorldPacket l_Update(SMSG_GARRISON_FOLLOWER_CHANGED_XP, 500);
            ByteBuffer l_UpdatePart(150);

            /// Write follower before any modification
            l_MissionFollowers[l_FollowerIt]->Write(l_UpdatePart);

            float l_SecondXPModifier = 1.0f;

            /// Personal XP Bonus
            for (uint32 l_AbilityIt = 0; l_AbilityIt < l_MissionFollowers[l_FollowerIt]->Abilities.size(); l_AbilityIt++)
            {
                uint32 l_CurrentAbilityID = l_MissionFollowers[l_FollowerIt]->Abilities[l_AbilityIt];

                for (uint32 l_EffectIt = 0; l_EffectIt < sGarrAbilityEffectStore.GetNumRows(); l_EffectIt++)
                {
                    GarrAbilityEffectEntry const* l_AbilityEffectEntry = sGarrAbilityEffectStore.LookupEntry(l_EffectIt);

                    if (!l_AbilityEffectEntry || l_AbilityEffectEntry->AbilityID != l_CurrentAbilityID)
                        continue;

                    if (l_AbilityEffectEntry->EffectType == AbilityEffectTypes::ModXpGain && l_AbilityEffectEntry->TargetMask == AbilityEffectTargetMasks::Self)
                        l_SecondXPModifier = (l_AbilityEffectEntry->Amount - 1.0) + l_SecondXPModifier;
                }
            }

            uint32 l_AddedXP = (l_BonusXP + l_MissionTemplate->RewardFollowerExperience) * l_SecondXPModifier;
            l_AddedXP = l_MissionFollowers[l_FollowerIt]->EarnXP(l_AddedXP);

            /// Write follower after modifications
            l_MissionFollowers[l_FollowerIt]->Write(l_UpdatePart);

            l_Update << uint32(l_AddedXP);
            l_Update << uint32(0);              ///< Seems like a reason case
            l_Update.append(l_UpdatePart);

            m_Owner->SendDirectMessage(&l_Update);

            if (l_FollowerLevel != l_MissionFollowers[l_FollowerIt]->Level && l_MissionFollowers[l_FollowerIt]->Level == 100)
                m_Owner->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_LEVELUP_FOLLOWERS);
        }

        m_PendingMissionReward.RewardFollowerXPBonus.clear();
        m_PendingMissionReward.MissionFollowers.clear();
        m_PendingMissionReward.RewardCurrencies.clear();
        m_PendingMissionReward.RewardItems.clear();
        m_PendingMissionReward.RewardGold       = 0;
        m_PendingMissionReward.RewardFollowerXP = 0;
        m_PendingMissionReward.Rewarded         = false;
        m_PendingMissionReward.MissionID        = p_MissionRecID;

        if (l_Succeeded)
        {
            for (uint32 l_I = 0; l_I < l_MissionFollowers.size(); ++l_I)
            {
                m_PendingMissionReward.MissionFollowers.push_back(l_MissionFollowers[l_I]->DatabaseID);
            }

            for (uint32 l_I = 0; l_I < sGarrMissionRewardStore.GetNumRows(); ++l_I)
            {
                GarrMissionRewardEntry const* l_RewardEntry = sGarrMissionRewardStore.LookupEntry(l_I);

                if (!l_RewardEntry || l_RewardEntry->MissionID != p_MissionRecID)
                    continue;

                if (l_RewardEntry->ItemID)
                    m_PendingMissionReward.RewardItems.push_back(std::make_pair(l_RewardEntry->ItemID, l_RewardEntry->ItemQuantity));

                if (l_RewardEntry->RewardCurrencyID == 0)
                    m_PendingMissionReward.RewardGold += l_RewardEntry->RewardCurrencyAmount;

                if (l_RewardEntry->RewardCurrencyID)
                {
                    uint32 l_Amount = l_RewardEntry->RewardCurrencyAmount;

                    if (l_RewardEntry->RewardCurrencyID == Globals::CurrencyID)
                    {
                        std::vector<uint32> l_PartyCurrencyModifiersEffect = GetMissionFollowersAbilitiesEffects(p_MissionRecID, AbilityEffectTypes::ModGarrCurrencyDrop, AbilityEffectTargetMasks::Unk | AbilityEffectTargetMasks::Party);

                        /// Global currency Bonus modifier
                        float l_Modifier = 1.0f;
                        for (uint32 l_I = 0; l_I < l_PartyCurrencyModifiersEffect.size(); ++l_I)
                        {
                            GarrAbilityEffectEntry const* l_AbilityEffectEntry = sGarrAbilityEffectStore.LookupEntry(l_PartyCurrencyModifiersEffect[l_I]);

                            if (!l_AbilityEffectEntry)
                                continue;

                            l_Modifier = (l_AbilityEffectEntry->Amount - 1.0) + l_Modifier;
                        }

                        l_Amount += (l_Modifier - 1.0f) * l_Amount;
                    }

                    m_PendingMissionReward.RewardCurrencies.push_back(std::make_pair(l_RewardEntry->RewardCurrencyID, l_Amount));
                }

                if (l_RewardEntry->BonusRewardXP)
                    m_PendingMissionReward.RewardFollowerXP += l_RewardEntry->BonusRewardXP;
            }

            switch (l_MissionTemplate->RequiredLevel)
            {
                case 90:
                case 91:
                case 92:
                case 93:
                case 94:
                {
                    if (HasActiveBuilding(Buildings::SalvageYard_SalvageYard_Level1))
                    {
                        if (roll_chance_i(30))
                            m_PendingMissionReward.RewardItems.push_back(std::make_pair(Items::ItemBagOfSalvagedGoods, 1));
                    }
                    else if (HasActiveBuilding(Buildings::SalvageYard_SalvageYard_Level2))
                    {
                        if (roll_chance_i(50))
                            m_PendingMissionReward.RewardItems.push_back(std::make_pair(Items::ItemBagOfSalvagedGoods, 1));
                    }
                    else if (HasActiveBuilding(Buildings::SalvageYard_SalvageYard_Level3))
                    {
                        if (roll_chance_i(75))
                            m_PendingMissionReward.RewardItems.push_back(std::make_pair(Items::ItemBagOfSalvagedGoods, 1));
                    }
                    break;
                }
                case 95:
                case 96:
                case 97:
                case 98:
                case 99:
                {
                    if (HasActiveBuilding(Buildings::SalvageYard_SalvageYard_Level1))
                    {
                        if (roll_chance_i(50))
                            m_PendingMissionReward.RewardItems.push_back(std::make_pair(Items::ItemBagOfSalvagedGoods, 1));
                    }
                    else if (HasActiveBuilding(Buildings::SalvageYard_SalvageYard_Level2))
                    {
                        if (roll_chance_i(30))
                            m_PendingMissionReward.RewardItems.push_back(std::make_pair(Items::ItemCrateOfSalvage, 1));
                    }
                    else if (HasActiveBuilding(Buildings::SalvageYard_SalvageYard_Level3))
                    {
                        if (roll_chance_i(50))
                            m_PendingMissionReward.RewardItems.push_back(std::make_pair(Items::ItemCrateOfSalvage, 1));
                    }
                    break;
                }
                case 100:
                {
                    if (HasActiveBuilding(Buildings::SalvageYard_SalvageYard_Level1))
                    {
                        if (roll_chance_i(75))
                            m_PendingMissionReward.RewardItems.push_back(std::make_pair(Items::ItemBagOfSalvagedGoods, 1));
                    }
                    else if (HasActiveBuilding(Buildings::SalvageYard_SalvageYard_Level2))
                    {
                        if (roll_chance_i(50))
                            m_PendingMissionReward.RewardItems.push_back(std::make_pair(Items::ItemCrateOfSalvage, 1));
                    }
                    else if (HasActiveBuilding(Buildings::SalvageYard_SalvageYard_Level3))
                    {
                        if (roll_chance_i(50))
                            m_PendingMissionReward.RewardItems.push_back(std::make_pair(Items::ItemBigCrateOfSalvage, 1));
                    }
                    break;
                }
                default:
                    break;
            }

            /// @TODO fix this
            ///l_BonusXP = (l_XPModifier - 1.0f) * m_PendingMissionReward.RewardFollowerXP;
            ///m_PendingMissionReward.RewardFollowerXP += l_BonusXP;
            ///
            ///for (uint32 l_FollowerIt = 0; l_FollowerIt < l_MissionFollowers.size(); ++l_FollowerIt)
            ///{
            ///    float l_SecondXPModifier = 1.0f;
            ///
            ///    /// Personnal XP Bonus
            ///    for (uint32 l_AbilityIt = 0; l_AbilityIt < l_MissionFollowers[l_FollowerIt]->Abilities.size(); l_AbilityIt++)
            ///    {
            ///        uint32 l_CurrentAbilityID = l_MissionFollowers[l_FollowerIt]->Abilities[l_AbilityIt];
            ///
            ///        for (uint32 l_EffectIt = 0; l_EffectIt < sGarrAbilityEffectStore.GetNumRows(); l_EffectIt++)
            ///        {
            ///            const GarrAbilityEffectEntry * l_AbilityEffectEntry = sGarrAbilityEffectStore.LookupEntry(l_EffectIt);
            ///
            ///            if (!l_AbilityEffectEntry || l_AbilityEffectEntry->AbilityID != l_CurrentAbilityID)
            ///                continue;
            ///
            ///            if (l_AbilityEffectEntry->EffectType == GARRISION_ABILITY_EFFECT_MOD_XP_GAIN && l_AbilityEffectEntry->TargetMask == GARRISON_ABILITY_EFFECT_TARGET_MASK_SELF)
            ///                l_SecondXPModifier = (l_AbilityEffectEntry->Amount - 1.0) + l_SecondXPModifier;
            ///        }
            ///    }
            ///
            ///    m_PendingMissionReward.RewardFollowerXPBonus.push_back(std::make_pair(l_MissionFollowers[l_FollowerIt]->DB_ID, (l_BonusXP + m_PendingMissionReward.RewardFollowerXP) * l_SecondXPModifier));
            ///}
        }

        /// Unasign follower to the mission
        for (uint32 l_I = 0; l_I < m_Followers.size(); ++l_I)
        {
            if (m_Followers[l_I].CurrentMissionID == p_MissionRecID)
            {
                m_Followers[l_I].CurrentMissionID = 0;

                WorldPacket l_Update(SMSG_GARRISON_UPDATE_FOLLOWER, 500);
                m_Followers[l_I].Write(l_Update);

                m_Owner->SendDirectMessage(&l_Update);
            }
        }
    }

    /// Do mission bonus roll
    void Manager::DoMissionBonusRoll(uint64 p_MissionTableGUID, uint32 p_MissionRecID)
    {
        auto l_It = std::find_if(m_Missions.begin(), m_Missions.end(), [p_MissionRecID](const GarrisonMission & p_Mission) -> bool
        {
            if (p_Mission.State == MissionStates::CompleteFailed || p_Mission.State == MissionStates::CompleteSuccess)
            {
                if (p_Mission.MissionID == p_MissionRecID)
                    return true;
            }

            return false;
        });

        GarrisonMission * l_Mission = nullptr;

        if (l_It != m_Missions.end())
            l_Mission = reinterpret_cast<GarrisonMission*>(&(*l_It));

        if (m_PendingMissionReward.MissionID != p_MissionRecID || m_PendingMissionReward.Rewarded == true || !l_Mission || (l_Mission && l_Mission->State == MissionStates::CompleteFailed))
        {
            m_PendingMissionReward.Rewarded = true;

            WorldPacket l_Packet(SMSG_GARRISON_MISSION_BONUS_ROLL_RESULT, 100);

            if (l_Mission)
            {
                l_Packet << uint64(l_Mission->DatabaseID);
                l_Packet << uint32(l_Mission->MissionID);
                l_Packet << uint32(l_Mission->OfferTime);
                l_Packet << uint32(l_Mission->OfferMaxDuration);
                l_Packet << uint32(l_Mission->StartTime);
                l_Packet << uint32(0);
                l_Packet << uint32(0);
                l_Packet << uint32(l_Mission->State);
            }
            else
            {
                l_Packet << uint64(0);
                l_Packet << uint32(0);
                l_Packet << uint32(0);
                l_Packet << uint32(0);
                l_Packet << uint32(0);
                l_Packet << uint32(0);
                l_Packet << uint32(0);
                l_Packet << uint32(0);
            }

            l_Packet << uint32(p_MissionRecID);
            l_Packet << uint32(MissionBonusRollResults::Error);

            m_Owner->SendDirectMessage(&l_Packet);

            return;
        }

        WorldPacket l_Packet(SMSG_GARRISON_MISSION_BONUS_ROLL_RESULT, 100);

        l_Packet << uint64(l_Mission->DatabaseID);
        l_Packet << uint32(l_Mission->MissionID);
        l_Packet << uint32(l_Mission->OfferTime);
        l_Packet << uint32(l_Mission->OfferMaxDuration);
        l_Packet << uint32(l_Mission->StartTime);
        l_Packet << uint32(0);
        l_Packet << uint32(0);
        l_Packet << uint32(l_Mission->State);

        l_Packet << uint32(p_MissionRecID);
        l_Packet << uint32(MissionBonusRollResults::Ok);

        m_Owner->SendDirectMessage(&l_Packet);

        m_PendingMissionReward.Rewarded = true;

        m_Owner->ModifyMoney(m_PendingMissionReward.RewardGold);

        for (auto l_Currency : m_PendingMissionReward.RewardCurrencies)
        {
            CurrencyTypesEntry const* l_CurrencyEntry = sCurrencyTypesStore.LookupEntry(l_Currency.first);

            if (l_CurrencyEntry)
                m_Owner->ModifyCurrency(l_Currency.first, l_Currency.second);
        }

        for (auto l_Item : m_PendingMissionReward.RewardItems)
        {
            const ItemTemplate * l_ItemTemplate = sObjectMgr->GetItemTemplate(l_Item.first);

            if (!l_ItemTemplate)
                continue;

            /// Check if the rewarded item is an follower contract, because make server crash on Player::SendNewItem,
            /// the spell is auto casted by the player, and the spell delete the caster item itself.
            bool l_IsContractItem = false;
            if (l_ItemTemplate->Spells[0].SpellId)
            {
                SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(l_ItemTemplate->Spells[0].SpellId);

                if (l_SpellInfo && l_SpellInfo->Effects[0].Effect == SPELL_EFFECT_ADD_GARRISON_FOLLOWER)
                {
                    l_IsContractItem = true;
                    m_Owner->CastSpell(m_Owner, l_SpellInfo, TRIGGERED_FULL_MASK);
                }
            }

            if (!l_IsContractItem)
            {
                uint32 l_NoSpaceForCount = 0;
                uint32 l_Count = l_Item.second;
                ItemPosCountVec l_Dest;

                if (m_Owner->CanStoreNewItem(NULL_BAG, NULL_SLOT, l_Dest, l_Item.first, l_Count, &l_NoSpaceForCount) != EQUIP_ERR_OK)
                    l_Count -= l_NoSpaceForCount;

                if (l_Count == 0 || l_Dest.empty())
                {
                    Item* l_NewItem = l_Item.first ? Item::CreateItem(l_Item.first, l_Item.second, m_Owner) : nullptr;

                    int l_LocIDX = m_Owner->GetSession()->GetSessionDbLocaleIndex();

                    MailDraft l_Draft("Garrison mission reward", "");

                    SQLTransaction l_Transaction = CharacterDatabase.BeginTransaction();
                    if (l_NewItem)
                    {
                        // /Save new item before send
                        l_NewItem->SaveToDB(l_Transaction);                               /// Save for prevent lost at next mail load, if send fail then item will deleted

                        /// Item
                        l_Draft.AddItem(l_NewItem);
                    }

                    l_Draft.SendMailTo(l_Transaction, m_Owner, MailSender(MAIL_CREATURE, GUID_ENPART(p_MissionTableGUID)));
                    CharacterDatabase.CommitTransaction(l_Transaction);
                }
                else
                    m_Owner->AddItem(l_Item.first, l_Item.second);
            }
        }

        std::vector<GarrisonFollower*> l_MissionFollowers;

        std::for_each(m_Followers.begin(), m_Followers.end(), [this, &l_MissionFollowers](const GarrisonFollower & p_Follower) -> void
        {
            if (std::find(m_PendingMissionReward.MissionFollowers.begin(), m_PendingMissionReward.MissionFollowers.end(), p_Follower.DatabaseID) != m_PendingMissionReward.MissionFollowers.end())
                l_MissionFollowers.push_back(const_cast<GarrisonFollower*>(&p_Follower));
        });

        std::for_each(l_MissionFollowers.begin(), l_MissionFollowers.end(), [this](const GarrisonFollower * p_Follower) -> void
        {
            uint32 l_FollowerLevel = p_Follower->Level;

            WorldPacket l_Update(SMSG_GARRISON_FOLLOWER_CHANGED_XP, 500);
            ByteBuffer l_UpdatePart(150);

            /// Write follower before any modification
            const_cast<GarrisonFollower*>(p_Follower)->Write(l_UpdatePart);

            uint32 l_AddedXP = m_PendingMissionReward.RewardFollowerXP;

            std::for_each(m_PendingMissionReward.RewardFollowerXPBonus.begin(), m_PendingMissionReward.RewardFollowerXPBonus.end(), [p_Follower, &l_AddedXP](const std::pair<uint64, uint32> & p_Values)
            {
                if (p_Values.first == p_Follower->DatabaseID)
                    l_AddedXP += p_Values.second;
            });

            l_AddedXP = const_cast<GarrisonFollower*>(p_Follower)->EarnXP(l_AddedXP);

            /// Write follower after modifications
            const_cast<GarrisonFollower*>(p_Follower)->Write(l_UpdatePart);
            
            l_Update << uint32(l_AddedXP);
            l_Update << uint32(0);              ///< Seems like a reason case
            l_Update.append(l_UpdatePart);

            m_Owner->SendDirectMessage(&l_Update);

            if (l_FollowerLevel != p_Follower->Level && p_Follower->Level == 100)
                m_Owner->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_LEVELUP_FOLLOWERS);
        });
    }

    /// Set mission has complete
    void Manager::SetAllInProgressMissionAsComplete()
    {
        for (uint32 l_I = 0; l_I < m_Missions.size(); ++l_I)
        {
            if (m_Missions[l_I].State == MissionStates::InProgress)
                m_Missions[l_I].StartTime = time(0) - (GetMissionTravelDuration(m_Missions[l_I].MissionID) + GetMissionDuration(m_Missions[l_I].MissionID));
        }

        WorldPacket l_PlaceHolder;
        m_Owner->GetSession()->HandleGetGarrisonInfoOpcode(l_PlaceHolder);
    }

    /// Get followers on a mission
    std::vector<GarrisonFollower*> Manager::GetMissionFollowers(uint32 p_MissionRecID)
    {
        std::vector<GarrisonFollower*> l_MissionFollowers;

        for (uint32 l_I = 0; l_I < m_Followers.size(); ++l_I)
        {
            if (m_Followers[l_I].CurrentMissionID == p_MissionRecID)
                l_MissionFollowers.push_back(&m_Followers[l_I]);
        }

        return l_MissionFollowers;
    }

    /// Get mission followers abilities effect
    std::vector<uint32> Manager::GetMissionFollowersAbilitiesEffects(uint32 p_MissionRecID)
    {
        std::vector<GarrisonFollower*> l_MissionFollowers = GetMissionFollowers(p_MissionRecID);
        std::vector<uint32>            l_AbilitiesEffects;

        for (uint32 l_FollowerIt = 0; l_FollowerIt < l_MissionFollowers.size(); ++l_FollowerIt)
        {
            for (uint32 l_AbilityIt = 0; l_AbilityIt < l_MissionFollowers[l_FollowerIt]->Abilities.size(); l_AbilityIt++)
            {
                uint32 l_CurrentAbilityID = l_MissionFollowers[l_FollowerIt]->Abilities[l_AbilityIt];

                for (uint32 l_EffectIt = 0; l_EffectIt < sGarrAbilityEffectStore.GetNumRows(); l_EffectIt++)
                {
                    GarrAbilityEffectEntry const* l_AbilityEffectEntry = sGarrAbilityEffectStore.LookupEntry(l_EffectIt);

                    if (!l_AbilityEffectEntry || l_AbilityEffectEntry->AbilityID != l_CurrentAbilityID)
                        continue;

                    l_AbilitiesEffects.push_back(l_AbilityEffectEntry->EffectID);
                }
            }
        }

        return l_AbilitiesEffects;
    }

    /// Get mission followers abilities effect
    std::vector<uint32> Manager::GetMissionFollowersAbilitiesEffects(uint32 p_MissionRecID, AbilityEffectTypes::Type p_Type, uint32 p_TargetMask)
    {
        std::vector<GarrisonFollower*> l_MissionFollowers = GetMissionFollowers(p_MissionRecID);
        std::vector<uint32>            l_AbilitiesEffects;

        for (uint32 l_FollowerIt = 0; l_FollowerIt < l_MissionFollowers.size(); ++l_FollowerIt)
        {
            for (uint32 l_AbilityIt = 0; l_AbilityIt < l_MissionFollowers[l_FollowerIt]->Abilities.size(); l_AbilityIt++)
            {
                uint32 l_CurrentAbilityID = l_MissionFollowers[l_FollowerIt]->Abilities[l_AbilityIt];

                for (uint32 l_EffectIt = 0; l_EffectIt < sGarrAbilityEffectStore.GetNumRows(); l_EffectIt++)
                {
                    GarrAbilityEffectEntry const* l_AbilityEffectEntry = sGarrAbilityEffectStore.LookupEntry(l_EffectIt);

                    if (!l_AbilityEffectEntry || l_AbilityEffectEntry->AbilityID != l_CurrentAbilityID)
                        continue;

                    if (l_AbilityEffectEntry->EffectType != p_Type || (l_AbilityEffectEntry->TargetMask != 0 && (l_AbilityEffectEntry->TargetMask & p_TargetMask) == 0))
                        continue;

                    l_AbilitiesEffects.push_back(l_AbilityEffectEntry->EffectID);
                }
            }
        }

        return l_AbilitiesEffects;
    }

    /// Get the mission travel time
    uint32 Manager::GetMissionTravelDuration(uint32 p_MissionRecID)
    {
        GarrMissionEntry const*     l_MissionTemplate = sGarrMissionStore.LookupEntry(p_MissionRecID);
        std::vector<uint32>         l_AbilitiesEffects  = GetMissionFollowersAbilitiesEffects(p_MissionRecID);
        std::vector<uint32>         l_PassiveEffects    = GetBuildingsPassiveAbilityEffects();
        float                       l_MissionTravelTime = l_MissionTemplate->TravelTime;

        for (uint32 l_EffectI = 0; l_EffectI < l_AbilitiesEffects.size(); l_EffectI++)
        {
            GarrAbilityEffectEntry const* l_AbilityEffectEntry = sGarrAbilityEffectStore.LookupEntry(l_AbilitiesEffects[l_EffectI]);

            if (!l_AbilityEffectEntry)
                continue;

            if (l_AbilityEffectEntry->EffectType == AbilityEffectTypes::ModTravelTime)
                l_MissionTravelTime = l_MissionTravelTime * l_AbilityEffectEntry->Amount;
        }

        for (uint32 l_Y = 0; l_Y < l_PassiveEffects.size(); ++l_Y)
        {
            GarrAbilityEffectEntry const* l_AbilityEffectEntry = sGarrAbilityEffectStore.LookupEntry(l_PassiveEffects[l_Y]);

            if (!l_AbilityEffectEntry)
                continue;

            if (l_AbilityEffectEntry->EffectType == AbilityEffectTypes::ModTravelTime)
                l_MissionTravelTime = l_MissionTravelTime * l_AbilityEffectEntry->Amount;
        }

        return floorf(l_MissionTravelTime);
    }

    /// Get the mission duration
    uint32 Manager::GetMissionDuration(uint32 p_MissionRecID)
    {
        GarrMissionEntry const*     l_MissionTemplate   = sGarrMissionStore.LookupEntry(p_MissionRecID);
        std::vector<uint32>         l_AbilitiesEffects  = GetMissionFollowersAbilitiesEffects(p_MissionRecID);
        float                       l_MissionDuration   = l_MissionTemplate->Duration;

        for (uint32 l_EffectI = 0; l_EffectI < l_AbilitiesEffects.size(); l_EffectI++)
        {
            GarrAbilityEffectEntry const* l_AbilityEffectEntry = sGarrAbilityEffectStore.LookupEntry(l_AbilitiesEffects[l_EffectI]);

            if (!l_AbilityEffectEntry)
                continue;

            if (l_AbilityEffectEntry->EffectType == AbilityEffectTypes::ModMissionDuration)
                l_MissionDuration = l_MissionDuration * l_AbilityEffectEntry->Amount;
        }

        return floorf(l_MissionDuration);
    }

    /// Get mission chest chance
    uint32 Manager::GetMissionSuccessChance(uint32 p_MissionRecID)
    {
        GarrMissionEntry const* l_MissionTemplate = sGarrMissionStore.LookupEntry(p_MissionRecID);

        std::vector<uint32>                     l_Encounters;
        std::vector<std::pair<uint32, uint32>>  l_EncoutersMechanics;
        std::vector<GarrisonFollower*>          l_MissionFollowers = GetMissionFollowers(p_MissionRecID);
        std::vector<uint32>                     l_PassiveEffects = GetBuildingsPassiveAbilityEffects();
        std::map<uint64, double>                l_FollowersBiasMap;
        std::vector<uint32>                     l_CounterAbilityUsed; 

        for (uint32 l_I = 0; l_I < sGarrMissionXEncouterStore.GetNumRows(); ++l_I)
        {
            GarrMissionXEncouterEntry const* l_Encounter = sGarrMissionXEncouterStore.LookupEntry(l_I);

            if (l_Encounter && l_Encounter->MissionID == p_MissionRecID)
                l_Encounters.push_back(l_Encounter->EncounterID);
        }

        for (uint32 l_I = 0; l_I < l_Encounters.size(); l_I++)
        {
            for (uint32 l_Y = 0; l_Y < sGarrEncouterXMechanicStore.GetNumRows(); ++l_Y)
            {
                GarrEncouterXMechanicEntry const* l_EncounterMechanic = sGarrEncouterXMechanicStore.LookupEntry(l_Y);

                if (l_EncounterMechanic && l_EncounterMechanic->EncounterID == l_Encounters[l_I])
                    l_EncoutersMechanics.push_back(std::make_pair(l_Encounters[l_I], l_EncounterMechanic->MechanicID));
            }
        }

        for (uint32 l_Y = 0; l_Y < l_MissionFollowers.size(); ++l_Y)
        {
            double l_FollowerBias = (l_MissionFollowers[l_Y]->Level - l_MissionTemplate->RequiredLevel) * 0.33333334;

            if (l_MissionTemplate->RequiredLevel == Globals::MaxFollowerLevel)
            {
                if (l_MissionTemplate->RequiredItemLevel > 0)
                {
                    uint32 l_TotalFollowerItemLevel = l_MissionFollowers[l_Y]->ItemLevelArmor + l_MissionFollowers[l_Y]->ItemLevelWeapon;
                    l_FollowerBias = (((l_TotalFollowerItemLevel >> 1) - l_MissionTemplate->RequiredItemLevel) * 0.06666667) + l_FollowerBias;
                }
            }

            if (l_FollowerBias < -1.0)
                l_FollowerBias = -1.0;
            else if (l_FollowerBias > 1.0)
                l_FollowerBias = 1.0;

            l_FollowersBiasMap[l_MissionFollowers[l_Y]->DatabaseID] = l_FollowerBias;

            #ifdef GARRISON_CHEST_FORMULA_DEBUG
                printf("Follower %u bias %Lf\n", l_MissionFollowers[l_Y]->FollowerID, l_FollowerBias);
            #endif // GARRISON_CHEST_FORMULA_DEBUG
        }

        double l_Float8 = 100.0;
        double l_FloatC = 150.0;

        double l_V8 = l_MissionTemplate->RequiredFollowersCount * l_Float8;
        double l_V60 = l_MissionTemplate->RequiredFollowersCount * l_Float8;

        for (uint32 l_I = 0; l_I < l_EncoutersMechanics.size(); ++l_I)
        {
            GarrMechanicEntry     const* l_MechanicEntry     = sGarrMechanicStore.LookupEntry(l_EncoutersMechanics[l_I].second);
            GarrMechanicTypeEntry const* l_MechanicTypeEntry = sGarrMechanicTypeStore.LookupEntry(l_MechanicEntry->MechanicTypeID);

            if (l_MechanicTypeEntry && l_MechanicTypeEntry->Type != MechanicTypes::Ability)
            {
                l_V8 = l_V60;
            }
            else
            {
                l_V8  = l_MechanicEntry->Unk2 + l_V60;
                l_V60 = l_MechanicEntry->Unk2 + l_V60;
            }
        }

        double l_CurrentAdditionalWinChance = 0;

        double l_V11 = 100.0 / l_V8;
        double l_V62 = 100.0 / l_V8;

        /// OK 100%
        #pragma region Followers Bias
        for (uint32 l_Y = 0; l_Y < l_MissionFollowers.size(); ++l_Y)
        {
            double l_Seil = 0;

            if (l_FollowersBiasMap[l_MissionFollowers[l_Y]->DatabaseID] >= 0.0)
                l_Seil = (l_FloatC - l_Float8) * l_FollowersBiasMap[l_MissionFollowers[l_Y]->DatabaseID] + l_Float8;
            else
                l_Seil = (l_FollowersBiasMap[l_MissionFollowers[l_Y]->DatabaseID] + 1.0) * l_Float8;

            l_V8 = (l_Seil * l_V11) + l_CurrentAdditionalWinChance;
            l_CurrentAdditionalWinChance = (l_Seil * l_V11) + l_CurrentAdditionalWinChance;

            #ifdef GARRISON_CHEST_FORMULA_DEBUG
                printf("Added %.2f to success due to follower %u bias.\n", (l_Seil * l_V11), l_MissionFollowers[l_Y]->FollowerID);
            #endif // GARRISON_CHEST_FORMULA_DEBUG
        }
        #pragma endregion

        //////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////

        /// OK 100%
        #pragma region Counter mechanic
        for (uint32 l_I = 0; l_I < l_EncoutersMechanics.size(); ++l_I)
        {
            GarrMechanicEntry     const* l_MechanicEntry     = sGarrMechanicStore.LookupEntry(l_EncoutersMechanics[l_I].second);
            GarrMechanicTypeEntry const* l_MechanicTypeEntry = sGarrMechanicTypeStore.LookupEntry(l_MechanicEntry->MechanicTypeID);

            if (l_MechanicTypeEntry->Type == MechanicTypes::Ability)
            {
                double l_Unk1 = l_MechanicEntry->Unk2;
                double l_Unk2 = l_MechanicEntry->Unk2;

                if (l_MissionFollowers.size() > 0)
                {
                    for (uint32 l_Y = 0; l_Y < l_MissionFollowers.size(); ++l_Y)
                    {
                        for (uint32 l_Z = 0; l_Z < l_MissionFollowers[l_Y]->Abilities.size(); l_Z++)
                        {
                            uint32 l_CurrentAbilityID = l_MissionFollowers[l_Y]->Abilities[l_Z];

                            for (uint32 l_EffectI = 0; l_EffectI < sGarrAbilityEffectStore.GetNumRows(); l_EffectI++)
                            {
                                GarrAbilityEffectEntry const* l_AbilityEffectEntry = sGarrAbilityEffectStore.LookupEntry(l_EffectI);

                                if (!l_AbilityEffectEntry || l_AbilityEffectEntry->AbilityID != l_CurrentAbilityID)
                                    continue;

                                if (l_AbilityEffectEntry->CounterMechanicTypeID == l_MechanicTypeEntry->ID && !(l_AbilityEffectEntry->Unk3 & 1))
                                {
                                    l_Unk1 = l_Unk2;
                                    if (l_Unk2 != 0.0)
                                    {
                                        float l_Seil = 0;

                                        if (l_FollowersBiasMap[l_MissionFollowers[l_Y]->DatabaseID] >= 0.0)
                                            l_Seil = (l_AbilityEffectEntry->ModMax - l_AbilityEffectEntry->ModMin) * l_FollowersBiasMap[l_MissionFollowers[l_Y]->DatabaseID] + l_AbilityEffectEntry->ModMin;
                                        else
                                            l_Seil = (l_FollowersBiasMap[l_MissionFollowers[l_Y]->DatabaseID] + 1.0) * l_AbilityEffectEntry->ModMin;

                                        l_Unk1 = l_MechanicEntry->Unk2;

                                        if (l_Seil <= l_Unk1)
                                            l_Unk1 = l_Seil;

                                        l_Unk2 = l_Unk2 - l_Unk1;
                                    }
                                }
                            }
                        }
                    }
                }

                if (l_Unk2 < 0.0)
                    l_Unk2 = 0.0;

                l_Unk1 = l_MechanicEntry->Unk2;
                l_Unk1 = (l_Unk1 - l_Unk2) * l_V62;
                l_CurrentAdditionalWinChance = l_Unk1 + l_CurrentAdditionalWinChance;

                #ifdef GARRISON_CHEST_FORMULA_DEBUG
                    printf("Added %.2f to success due to followers countering boss mechanic %u.\n", l_Unk1, l_EncoutersMechanics[l_I].second);
                #endif // GARRISON_CHEST_FORMULA_DEBUG
            }
        }
        #pragma endregion

        //////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////

        /// UNTESTED
        #pragma region Race Ability Counter
        for (uint32 l_I = 0; l_I < l_EncoutersMechanics.size(); ++l_I)
        {
            GarrMechanicEntry     const* l_MechanicEntry     = sGarrMechanicStore.LookupEntry(l_EncoutersMechanics[l_I].second);
            GarrMechanicTypeEntry const* l_MechanicTypeEntry = sGarrMechanicTypeStore.LookupEntry(l_MechanicEntry->MechanicTypeID);

            if (l_MechanicTypeEntry->Type == MechanicTypes::Racial)
            {
                for (uint32 l_Y = 0; l_Y < l_MissionFollowers.size(); ++l_Y)
                {
                    for (uint32 l_Z = 0; l_Z < l_MissionFollowers[l_Y]->Abilities.size(); l_Z++)
                    {
                        uint32 l_CurrentAbilityID = l_MissionFollowers[l_Y]->Abilities[l_Z];

                        for (uint32 l_EffectI = 0; l_EffectI < sGarrAbilityEffectStore.GetNumRows(); l_EffectI++)
                        {
                            GarrAbilityEffectEntry const* l_AbilityEffectEntry = sGarrAbilityEffectStore.LookupEntry(l_EffectI);

                            if (!l_AbilityEffectEntry || l_AbilityEffectEntry->AbilityID != l_CurrentAbilityID)
                                continue;

                            if (l_AbilityEffectEntry->CounterMechanicTypeID == l_MissionTemplate->GarrMechanicTypeRecID)
                            {
                                double l_Seil = 0;

                                if (l_FollowersBiasMap[l_MissionFollowers[l_Y]->DatabaseID] >= 0.0)
                                    l_Seil = (l_AbilityEffectEntry->ModMax - l_AbilityEffectEntry->ModMin) * l_FollowersBiasMap[l_MissionFollowers[l_Y]->DatabaseID] + l_AbilityEffectEntry->ModMin;
                                else
                                    l_Seil = (l_FollowersBiasMap[l_MissionFollowers[l_Y]->DatabaseID] + 1.0) * l_AbilityEffectEntry->ModMin;

                                l_CurrentAdditionalWinChance = (l_Seil * l_V62) + l_CurrentAdditionalWinChance;

                                #ifdef GARRISON_CHEST_FORMULA_DEBUG
                                    printf("Added %.2f to success due to follower %u enemy race ability %d.\n", (l_Seil * l_V62), 0, l_CurrentAbilityID);
                                #endif // GARRISON_CHEST_FORMULA_DEBUG
                            }
                        }
                    }
                }
            }
        }
        #pragma endregion 

        //////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////

        /// OK 100%
        #pragma region Environment Ability
        for (uint32 l_Y = 0; l_Y < l_MissionFollowers.size(); ++l_Y)
        {
            for (uint32 l_Z = 0; l_Z < l_MissionFollowers[l_Y]->Abilities.size(); l_Z++)
            {
                uint32 l_CurrentAbilityID = l_MissionFollowers[l_Y]->Abilities[l_Z];

                for (uint32 l_EffectI = 0; l_EffectI < sGarrAbilityEffectStore.GetNumRows(); l_EffectI++)
                {
                    GarrAbilityEffectEntry const* l_AbilityEffectEntry = sGarrAbilityEffectStore.LookupEntry(l_EffectI);

                    if (!l_AbilityEffectEntry || l_AbilityEffectEntry->AbilityID != l_CurrentAbilityID)
                        continue;

                    if (l_AbilityEffectEntry->CounterMechanicTypeID == l_MissionTemplate->GarrMechanicTypeRecID)
                    {
                        double l_Seil = 0;

                        if (l_FollowersBiasMap[l_MissionFollowers[l_Y]->DatabaseID] >= 0.0)
                            l_Seil = (l_AbilityEffectEntry->ModMax - l_AbilityEffectEntry->ModMin) * l_FollowersBiasMap[l_MissionFollowers[l_Y]->DatabaseID] + l_AbilityEffectEntry->ModMin;
                        else
                            l_Seil = (l_FollowersBiasMap[l_MissionFollowers[l_Y]->DatabaseID] + 1.0) * l_AbilityEffectEntry->ModMin;

                        l_CurrentAdditionalWinChance = (l_Seil * l_V62) + l_CurrentAdditionalWinChance;

                        #ifdef GARRISON_CHEST_FORMULA_DEBUG
                            printf("Added %.2f to success due to follower %u environment ability %u.\n", (l_Seil * l_V62), l_MissionFollowers[l_Y]->FollowerID, l_CurrentAbilityID);
                        #endif // GARRISON_CHEST_FORMULA_DEBUG
                    }
                }
            }
        }
        #pragma endregion 

        //////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////

        /// OK 100%
        #pragma region Follower Trait
        double l_MissionDuration = GetMissionDuration(p_MissionRecID);
        double l_MissionTravelTime = GetMissionTravelDuration(p_MissionRecID);

        for (uint32 l_Y = 0; l_Y < l_MissionFollowers.size(); ++l_Y)
        {
            for (uint32 l_Z = 0; l_Z < l_MissionFollowers[l_Y]->Abilities.size(); l_Z++)
            {
                uint32 l_CurrentAbilityID = l_MissionFollowers[l_Y]->Abilities[l_Z];

                for (uint32 l_EffectI = 0; l_EffectI < sGarrAbilityEffectStore.GetNumRows(); l_EffectI++)
                {
                    GarrAbilityEffectEntry const* l_AbilityEffectEntry = sGarrAbilityEffectStore.LookupEntry(l_EffectI);

                    if (!l_AbilityEffectEntry || l_AbilityEffectEntry->AbilityID != l_CurrentAbilityID)
                        continue;

                    bool l_Proc = false;

                    switch (l_AbilityEffectEntry->EffectType)
                    {
                        /// Proc if MissionFollowerCount == 1
                        case AbilityEffectTypes::ModWinRateSolo:
                            l_Proc = l_MissionFollowers.size() == 1;
                            break;

                        /// Proc every time, no condition
                        case AbilityEffectTypes::ModWinRate:
                            l_Proc = true;
                            break;

                        /// Proc if Find(MissionFollowers[Class], MiscValueA) != NULL
                        case AbilityEffectTypes::ModWinRateClass:
                            for (uint32 l_W = 0; l_W < l_MissionFollowers.size(); ++l_W)
                            {
                                if (l_W != l_Y)
                                {
                                    GarrFollowerEntry const* l_FollowerTemplate = sGarrFollowerStore.LookupEntry(l_MissionFollowers[l_W]->FollowerID);

                                    if (l_FollowerTemplate && l_FollowerTemplate->Class[GetGarrisonFactionIndex()] == l_AbilityEffectEntry->MiscValueA)
                                    {
                                        l_Proc = true;
                                        break;
                                    }
                                }
                            }
                            break;

                        /// Proc if Duration > (3600 * MiscValueB)
                        case AbilityEffectTypes::ModWinRateDurationMore:
                            l_Proc = l_MissionDuration > (3600 * l_AbilityEffectEntry->MiscValueB);
                            break;

                        /// Proc if Duration < (3600 * MiscValueB)
                        case AbilityEffectTypes::ModWinRateDurationLess:
                            l_Proc = l_MissionDuration < (3600 * l_AbilityEffectEntry->MiscValueB);
                            break;

                        /// Proc if TravelDuration > (3600 * MiscValueB)
                        case AbilityEffectTypes::ModWinRateTravelDurationMore:
                            l_Proc = l_MissionTravelTime > (3600 * l_AbilityEffectEntry->MiscValueB);
                            break;

                        /// Proc if TravelDuration < (3600 * MiscValueB)
                        case AbilityEffectTypes::ModWinRateTravelDurationLess:
                            l_Proc = l_MissionTravelTime < (3600 * l_AbilityEffectEntry->MiscValueB);
                            break;

                        case AbilityEffectTypes::ModUnk0:
                        case AbilityEffectTypes::ModTravelTime:
                        case AbilityEffectTypes::ModXpGain:
                        case AbilityEffectTypes::ModGarrCurrencyDrop:
                        case AbilityEffectTypes::ModUnk11:
                        case AbilityEffectTypes::ModDummyProduction:
                        case AbilityEffectTypes::ModBronzeTreasureDrop:
                        case AbilityEffectTypes::ModSilverTreasureDrop:
                        case AbilityEffectTypes::ModGoldTreasureDrop:
                        case AbilityEffectTypes::ModChestDropRate:
                        case AbilityEffectTypes::ModMissionDuration:
                            break;

                        default:
                            break;

                    }

                    if (!l_Proc)
                        break;

                    double l_Seil = 0.0;

                    if (l_FollowersBiasMap[l_MissionFollowers[l_Y]->DatabaseID] >= 0.0)
                        l_Seil = (l_AbilityEffectEntry->ModMax - l_AbilityEffectEntry->ModMin) * l_FollowersBiasMap[l_MissionFollowers[l_Y]->DatabaseID] + l_AbilityEffectEntry->ModMin;
                    else
                        l_Seil = (l_FollowersBiasMap[l_MissionFollowers[l_Y]->DatabaseID] + 1.0) * l_AbilityEffectEntry->ModMin;

                    l_CurrentAdditionalWinChance = (l_Seil * l_V62) + l_CurrentAdditionalWinChance;

                    #ifdef GARRISON_CHEST_FORMULA_DEBUG
                        printf("Added %.2f to success due to follower %u trait %u.\n", (l_Seil * l_V62), l_MissionFollowers[l_Y]->FollowerID, l_AbilityEffectEntry->EffectType);
                    #endif // GARRISON_CHEST_FORMULA_DEBUG
                }
            }
        }
        #pragma endregion 

        //////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////

        /// UNTESTED
        #pragma region Passive Effect
        for (uint32 l_Y = 0; l_Y < l_PassiveEffects.size(); ++l_Y)
        {
            GarrAbilityEffectEntry const* l_AbilityEffectEntry = sGarrAbilityEffectStore.LookupEntry(l_PassiveEffects[l_Y]);

            if (!l_AbilityEffectEntry)
                continue;

            if (l_AbilityEffectEntry->EffectType == AbilityEffectTypes::ModWinRate)
            {
                l_CurrentAdditionalWinChance = (l_AbilityEffectEntry->ModMin * l_V62) + l_CurrentAdditionalWinChance;

                #ifdef GARRISON_CHEST_FORMULA_DEBUG
                    printf("Added %.2f to success due to passive effect %u.\n", l_AbilityEffectEntry->ModMin * l_V62, l_AbilityEffectEntry->AbilityID);
                #endif // GARRISON_CHEST_FORMULA_DEBUG
            }
        }
        #pragma endregion 

        //////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////

        l_CurrentAdditionalWinChance = (((100.0 - l_MissionTemplate->BaseBronzeChestChance) * l_CurrentAdditionalWinChance) * 0.0099999998) + l_MissionTemplate->BaseBronzeChestChance;

        if (l_CurrentAdditionalWinChance > 100.0)
            l_CurrentAdditionalWinChance = 100.0;

        #ifdef GARRISON_CHEST_FORMULA_DEBUG
            printf("Total success chance: %.2f.\n", l_CurrentAdditionalWinChance);
        #endif // GARRISON_CHEST_FORMULA_DEBUG

        return l_CurrentAdditionalWinChance;
    }

    /// Get missions
    std::vector<GarrisonMission> Manager::GetMissions() const
    {
        std::vector<GarrisonMission> l_Result;

        for (uint32 l_I = 0; l_I < m_Missions.size(); ++l_I)
        {
            if (m_Missions[l_I].State < MissionStates::CompleteSuccess
                && !((m_Missions[l_I].OfferTime + m_Missions[l_I].OfferMaxDuration) < time(0) && m_Missions[l_I].State == MissionStates::Available))
                l_Result.push_back(m_Missions[l_I]);
        }

        return l_Result;
    }

    /// Get all completed missions
    std::vector<GarrisonMission> Manager::GetCompletedMissions() const
    {
        std::vector<GarrisonMission> l_Result;

        for (uint32 l_I = 0; l_I < m_Missions.size(); ++l_I)
            if (m_Missions[l_I].State >= MissionStates::CompleteSuccess)
                l_Result.push_back(m_Missions[l_I]);

        return l_Result;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Add follower
    bool Manager::AddFollower(uint32 p_FollowerID)
    {
        if (GetFollower(p_FollowerID) != nullptr)
            return false;

        GarrFollowerEntry const* l_Entry = sGarrFollowerStore.LookupEntry(p_FollowerID);

        if (!l_Entry)
            return false;

        GarrisonFollower l_Follower;
        l_Follower.DatabaseID           = sObjectMgr->GetNewGarrisonFollowerID();
        l_Follower.FollowerID           = p_FollowerID;
        l_Follower.Level                = l_Entry->Level;
        l_Follower.XP                   = 0;
        l_Follower.Quality              = l_Entry->Quality;
        l_Follower.ItemLevelArmor       = l_Entry->ItemLevelArmor;
        l_Follower.ItemLevelWeapon      = l_Entry->ItemLevelWeapon;
        l_Follower.CurrentBuildingID    = 0;
        l_Follower.CurrentMissionID     = 0;
        l_Follower.Flags                = 0;

        for (uint32 l_I = 0; l_I < sGarrFollowerXAbilityStore.GetNumRows(); ++l_I)
        {
            GarrFollowerXAbilityEntry const* l_Entry = sGarrFollowerXAbilityStore.LookupEntry(l_I);

            if (l_Entry && l_Entry->FollowerID == p_FollowerID && sGarrAbilityStore.LookupEntry(l_Entry->AbilityID) && l_Entry->FactionIndex == GetGarrisonFactionIndex())
                l_Follower.Abilities.push_back(l_Entry->AbilityID);
        }

        std::ostringstream l_Abilities;

        for (uint32 l_Y = 0; l_Y < l_Follower.Abilities.size(); ++l_Y)
            l_Abilities << l_Follower.Abilities[l_Y] << ' ';

        PreparedStatement* l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_GARRISON_FOLLOWER);

        uint32 l_Index = 0;
        l_Stmt->setUInt32(l_Index++, l_Follower.DatabaseID);
        l_Stmt->setUInt32(l_Index++, m_ID);
        l_Stmt->setUInt32(l_Index++, l_Follower.FollowerID);
        l_Stmt->setUInt32(l_Index++, l_Follower.Level);
        l_Stmt->setUInt32(l_Index++, l_Follower.XP);
        l_Stmt->setUInt32(l_Index++, l_Follower.Quality);
        l_Stmt->setUInt32(l_Index++, l_Follower.ItemLevelArmor);
        l_Stmt->setUInt32(l_Index++, l_Follower.ItemLevelWeapon);
        l_Stmt->setUInt32(l_Index++, l_Follower.CurrentMissionID);
        l_Stmt->setUInt32(l_Index++, l_Follower.CurrentBuildingID);
        l_Stmt->setString(l_Index++, l_Abilities.str());
        l_Stmt->setUInt32(l_Index++, l_Follower.Flags);

        CharacterDatabase.AsyncQuery(l_Stmt);

        m_Followers.push_back(l_Follower);

        WorldPacket l_AddFollowerResult(SMSG_GARRISON_ADD_FOLLOWER_RESULT, 64);
        l_AddFollowerResult << uint32(PurchaseBuildingResults::Ok);
        l_Follower.Write(l_AddFollowerResult);

        m_Owner->SendDirectMessage(&l_AddFollowerResult);

        uint32 l_AchievementEntry = 0;

        switch (l_Follower.Quality)
        {
            case ItemQualities::ITEM_QUALITY_RARE:
                l_AchievementEntry = 9130;  ///< A Rare Friend
                break;
            case ItemQualities::ITEM_QUALITY_EPIC:
                l_AchievementEntry = 9131;  ///< An Epic Buddy
                break;

            default:
                break;
        }

        if (l_AchievementEntry && !m_Owner->GetAchievementMgr().HasAchieved(l_AchievementEntry))
            m_Owner->GetAchievementMgr().CompletedAchievement(sAchievementStore.LookupEntry(l_AchievementEntry), nullptr);

        m_Owner->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_RECRUIT_FOLLOWER_IN_OWN_GARRISON);

        return true;
    }

    /// Change follower activation state
    void Manager::ChangeFollowerActivationState(uint64 p_FollowerDBID, bool p_Active)
    {
        GarrisonFollower * l_Follower = nullptr;

        if (p_Active)
        {
            if (!m_Owner->HasEnoughMoney((uint64)Globals::FollowerActivationCost))
                return;

            if (GetNumFollowerActivationsRemaining() < 1)
                return;

            auto l_It = std::find_if(m_Followers.begin(), m_Followers.end(), [p_FollowerDBID](const GarrisonFollower & p_Follower) { return p_Follower.DatabaseID == p_FollowerDBID; });

            if (l_It != m_Followers.end())
            {
                m_Owner->ModifyMoney(-Globals::FollowerActivationCost);

                m_NumFollowerActivation--;
                m_NumFollowerActivationRegenTimestamp = time(0);

                l_It->Flags = l_It->Flags & ~GARRISON_FOLLOWER_FLAG_INACTIVE;
                l_Follower = &(*l_It);

                WorldPacket l_Data(SMSG_GARRISON_UPDATE_FOLLOWER_ACTIVATION_COUNT, 4);
                l_Data << uint32(GetNumFollowerActivationsRemaining());

                m_Owner->SendDirectMessage(&l_Data);
            }
        }
        else
        {
            if (!m_Owner->HasEnoughMoney((uint64)Globals::FollowerActivationCost))
                return;

            auto l_It = std::find_if(m_Followers.begin(), m_Followers.end(), [p_FollowerDBID](const GarrisonFollower & p_Follower) { return p_Follower.DatabaseID == p_FollowerDBID; });

            if (l_It != m_Followers.end())
            {
                m_Owner->ModifyMoney(-Globals::FollowerActivationCost);

                l_It->Flags |= GARRISON_FOLLOWER_FLAG_INACTIVE;
                l_Follower = &(*l_It);
            }

            l_Follower = &*l_It;
        }

        if (!l_Follower)
            return;

        WorldPacket l_Update(SMSG_GARRISON_UPDATE_FOLLOWER, 500);
        l_Follower->Write(l_Update);

        m_Owner->SendDirectMessage(&l_Update);
    }

    /// Get followers
    std::vector<GarrisonFollower> Manager::GetFollowers() const
    {
        return m_Followers;
    }

    /// Get follower
    GarrisonFollower* Manager::GetFollower(uint32 p_FollowerID)
    {
        for (uint32 l_I = 0; l_I < m_Followers.size(); l_I++)
        {
            if (m_Followers[l_I].FollowerID == p_FollowerID)
                return &m_Followers[l_I];
        }

        return nullptr;
    }

    /// Get activated followers count
    uint32 Manager::GetActivatedFollowerCount() const
    {
        uint32 l_ActivatedFollowerCount = 0;

        for (uint32 l_I = 0; l_I < m_Followers.size(); l_I++)
        {
            if ((m_Followers[l_I].Flags & GARRISON_FOLLOWER_FLAG_INACTIVE) == 0)
                l_ActivatedFollowerCount++;
        }

        return l_ActivatedFollowerCount;
    }

    /// Get num follower activation remaining
    uint32 Manager::GetNumFollowerActivationsRemaining() const
    {
        return m_NumFollowerActivation;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Can build building X at slot instance Y
    bool Manager::IsBuildingPlotInstanceValid(uint32 p_BuildingRecID, uint32 p_PlotInstanceID) const
    {
        GarrBuildingEntry const* l_BuildingEntry = sGarrBuildingStore.LookupEntry(p_BuildingRecID);

        if (!l_BuildingEntry)
            return false;

        GarrPlotInstanceEntry const* l_PlotInstanceEntry = sGarrPlotInstanceStore.LookupEntry(p_PlotInstanceID);

        if (!l_PlotInstanceEntry)
            return false;

        /// Search building plot ID 
        for (uint32 l_I = 0; l_I < sGarrPlotBuildingStore.GetNumRows(); ++l_I)
        {
            GarrPlotBuildingEntry const* l_PlotBuildingEntry = sGarrPlotBuildingStore.LookupEntry(l_I);

            if (l_PlotBuildingEntry && l_PlotBuildingEntry->PlotId == l_PlotInstanceEntry->PlotID && l_PlotBuildingEntry->BuildingID == p_BuildingRecID)
                return true;
        }

        return false;
    }

    /// Player fill all condition
    PurchaseBuildingResults::Type Manager::CanPurchaseBuilding(uint32 p_BuildingRecID) const
    {
        GarrBuildingEntry const* l_BuildingEntry = sGarrBuildingStore.LookupEntry(p_BuildingRecID);

        if (!l_BuildingEntry)
            return PurchaseBuildingResults::InvalidBuildingID;

        if (l_BuildingEntry->BuildCostCurrencyID != 0)
        {
            if (!m_Owner->HasCurrency(l_BuildingEntry->BuildCostCurrencyID, l_BuildingEntry->BuildCostCurrencyAmount))
                return PurchaseBuildingResults::NotEnoughCurrency;

            if (!m_Owner->HasEnoughMoney((uint64)l_BuildingEntry->MoneyCost * GOLD))
                return PurchaseBuildingResults::NotEnoughGold;
        }

        return PurchaseBuildingResults::Ok;
    }

    /// PurchaseBuilding
    GarrisonBuilding Manager::PurchaseBuilding(uint32 p_BuildingRecID, uint32 p_PlotInstanceID, bool p_Triggered)
    {
        GarrBuildingEntry const* l_BuildingEntry = sGarrBuildingStore.LookupEntry(p_BuildingRecID);

        GarrisonBuilding l_Building;

        if (!l_BuildingEntry)
            return l_Building;

        if (!PlotIsFree(p_PlotInstanceID))
            DeleteBuilding(p_PlotInstanceID);

        if (l_BuildingEntry->BuildCostCurrencyID != 0 && !p_Triggered)
            m_Owner->ModifyCurrency(l_BuildingEntry->BuildCostCurrencyID, -(int32)l_BuildingEntry->BuildCostCurrencyAmount);

        if (l_BuildingEntry->MoneyCost != 0 && !p_Triggered)
            m_Owner->ModifyMoney(-(int64)(l_BuildingEntry->MoneyCost * GOLD));

        if (!p_Triggered)
        {
            WorldPacket l_PlotRemoved(SMSG_GARRISON_PLOT_REMOVED, 4);
            l_PlotRemoved << uint32(p_PlotInstanceID);
            m_Owner->SendDirectMessage(&l_PlotRemoved);
        }

        uint32 l_BuildingTime = l_BuildingEntry->BuildTime;

        if (GetGarrisonScript())
            l_BuildingTime = GetGarrisonScript()->OnPrePurchaseBuilding(m_Owner, p_BuildingRecID, l_BuildingTime);

        l_Building.DatabaseID       = sObjectMgr->GetNewGarrisonBuildingID();
        l_Building.BuildingID       = p_BuildingRecID;
        l_Building.PlotInstanceID   = p_PlotInstanceID;
        l_Building.TimeBuiltStart   = time(nullptr);
        l_Building.TimeBuiltEnd     = time(nullptr) + l_BuildingTime;           ///< 5/5/1905 18:45:05
        l_Building.SpecID           = 0;
        l_Building.Active           = false;
        l_Building.BuiltNotified    = false;

        if (l_BuildingEntry->BuildingCategory == BuildingCategory::Prebuilt)
        {
            l_Building.TimeBuiltStart   = time(nullptr) - l_BuildingTime;
            l_Building.TimeBuiltEnd     = time(nullptr);
            l_Building.Active           = false;
            l_Building.BuiltNotified    = true;
        }
        else if (p_Triggered)
            l_Building.TimeBuiltEnd = l_Building.TimeBuiltStart;

        PreparedStatement * l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_GARRISON_BUILDING);

        uint32 l_Index = 0;
        l_Stmt->setUInt32(l_Index++, l_Building.DatabaseID);
        l_Stmt->setUInt32(l_Index++, m_ID);
        l_Stmt->setUInt32(l_Index++, l_Building.PlotInstanceID);
        l_Stmt->setUInt32(l_Index++, l_Building.BuildingID);
        l_Stmt->setUInt32(l_Index++, l_Building.SpecID);
        l_Stmt->setUInt32(l_Index++, l_Building.TimeBuiltStart);
        l_Stmt->setUInt32(l_Index++, l_Building.TimeBuiltEnd);
        l_Stmt->setBool(l_Index++, l_Building.Active);
        l_Stmt->setString(l_Index++, l_Building.GatheringData);

        CharacterDatabase.AsyncQuery(l_Stmt);

        m_Buildings.push_back(l_Building);

        UpdatePlot(p_PlotInstanceID);

        if (GetGarrisonScript())
            GetGarrisonScript()->OnPurchaseBuilding(m_Owner, p_BuildingRecID);

        return l_Building;
    }

    /// Get building
    GarrisonBuilding Manager::GetBuilding(uint32 p_PlotInstanceID) const
    {
        for (uint32 l_I = 0; l_I < m_Buildings.size(); ++l_I)
            if (m_Buildings[l_I].PlotInstanceID == p_PlotInstanceID)
                return m_Buildings[l_I];

        return GarrisonBuilding();
    }

    /// Get buildings
    std::vector<GarrisonBuilding> Manager::GetBuildings() const
    {
        std::vector<GarrisonBuilding> l_Buildings;

        for (uint32 l_I = 0; l_I < m_Buildings.size(); ++l_I)
        {
            /// Don't include the building if the owner doesn't have access to it
            if (!sGarrisonBuildingManager->MatchsConditionsForBuilding(m_Buildings[l_I].BuildingID, m_Owner))
                continue;

            l_Buildings.push_back(m_Buildings[l_I]);
        }

        return l_Buildings;
    }

    /// Get building passive ability effects
    std::vector<uint32> Manager::GetBuildingsPassiveAbilityEffects() const
    {
        std::vector<uint32> l_PassiveEffects;

        for (uint32 l_I = 0; l_I < m_Buildings.size(); ++l_I)
        {
            /// Don't include prebuilt building passives if the owner doesn't have access to it
            if (!sGarrisonBuildingManager->MatchsConditionsForBuilding(m_Buildings[l_I].BuildingID, m_Owner))
                continue;

            GarrBuildingEntry const* l_BuildingTemplate = sGarrBuildingStore.LookupEntry(m_Buildings[l_I].BuildingID);

            if (l_BuildingTemplate && l_BuildingTemplate->PassiveEffect && sGarrAbilityEffectStore.LookupEntry(l_BuildingTemplate->PassiveEffect) != nullptr)
                l_PassiveEffects.push_back(l_BuildingTemplate->PassiveEffect);
        }

        return l_PassiveEffects;
    }

    /// Activate building
    void Manager::ActivateBuilding(uint32 p_PlotInstanceID)
    {
        GarrisonBuilding * l_Building = nullptr;

        for (uint32 l_I = 0; l_I < m_Buildings.size(); ++l_I)
        {
            if (m_Buildings[l_I].PlotInstanceID == p_PlotInstanceID)
            {
                l_Building = &m_Buildings[l_I];
                break;
            }
        }

        if (!l_Building)
            return;

        GarrBuildingEntry const* l_BuildingEntry = sGarrBuildingStore.LookupEntry(l_Building->BuildingID);

        if (!l_BuildingEntry)
            return;

        l_Building->Active = true;

        UpdatePlot(p_PlotInstanceID);

        WorldPacket l_Packet(SMSG_GARRISON_BUILDING_ACTIVATED, 4);
        l_Packet << uint32(p_PlotInstanceID);
        m_Owner->SendDirectMessage(&l_Packet);

        UpdateStats();

        if (GetGarrisonScript())
            GetGarrisonScript()->OnBuildingActivated(m_Owner, l_Building->BuildingID);
    }

    /// Activate building
    void Manager::ActivateBuilding()
    {
        uint32 l_PlotInstance = GetPlotInstanceIDByActivationGameObject(m_LastUsedActivationGameObject);

        if (l_PlotInstance)
            ActivateBuilding(l_PlotInstance);
    }

    /// Cancel construction
    void Manager::CancelConstruction(uint32 p_PlotInstanceID)
    {
        if (!HasPlotInstance(p_PlotInstanceID))
            return;

        uint32 l_BuildingID = GetBuilding(p_PlotInstanceID).BuildingID;

        if (!l_BuildingID)
            return;

        GarrBuildingEntry const* l_BuildingEntry = sGarrBuildingStore.LookupEntry(l_BuildingID);

        if (!l_BuildingEntry)
            return;

        DeleteBuilding(p_PlotInstanceID);

        if (l_BuildingEntry->BuildCostCurrencyID != 0)
            m_Owner->ModifyCurrency(l_BuildingEntry->BuildCostCurrencyID, (int32)l_BuildingEntry->BuildCostCurrencyAmount);

        if (l_BuildingEntry->MoneyCost != 0)
            m_Owner->ModifyMoney(l_BuildingEntry->MoneyCost);
    }

    /// Delete building
    void Manager::DeleteBuilding(uint32 p_PlotInstanceID)
    {
        if (!HasPlotInstance(p_PlotInstanceID))
            return;

        uint32 l_BuildingID = GetBuilding(p_PlotInstanceID).BuildingID;

        if (!l_BuildingID)
            return;

        GarrBuildingEntry const* l_BuildingEntry = sGarrBuildingStore.LookupEntry(l_BuildingID);

        if (!l_BuildingEntry)
            return;

        PreparedStatement * l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GARRISON_BUILDING);
        l_Stmt->setUInt32(0, GetBuilding(p_PlotInstanceID).DatabaseID);
        CharacterDatabase.AsyncQuery(l_Stmt);

        for (std::vector<GarrisonBuilding>::iterator l_It = m_Buildings.begin(); l_It != m_Buildings.end(); ++l_It)
        {
            if (l_It->BuildingID == l_BuildingID)
            {
                m_Buildings.erase(l_It);
                break;
            }
        }

        UpdatePlot(p_PlotInstanceID);

        GarrisonPlotInstanceInfoLocation l_PlotLocation = GetPlot(p_PlotInstanceID);

        WorldPacket l_PlotPlacedPacket(SMSG_GARRISON_PLOT_PLACED, 24);
        l_PlotPlacedPacket << uint32(p_PlotInstanceID);
        l_PlotPlacedPacket << float(l_PlotLocation.X);
        l_PlotPlacedPacket << float(l_PlotLocation.Y);
        l_PlotPlacedPacket << float(l_PlotLocation.Z);
        l_PlotPlacedPacket << float(l_PlotLocation.O);
        l_PlotPlacedPacket << uint32(GetPlotType(p_PlotInstanceID));

        m_Owner->SendDirectMessage(&l_PlotPlacedPacket);

        WorldPacket l_BuildingRemovedPacket(SMSG_GARRISON_BUILDING_REMOVED, 12);
        l_BuildingRemovedPacket << uint32(PurchaseBuildingResults::Ok);
        l_BuildingRemovedPacket << uint32(p_PlotInstanceID);
        l_BuildingRemovedPacket << uint32(l_BuildingID);

        m_Owner->SendDirectMessage(&l_BuildingRemovedPacket);
    }

    /// Has active building
    bool Manager::HasActiveBuilding(uint32 p_BuildingID) const
    {
        for (std::vector<GarrisonBuilding>::const_iterator l_It = m_Buildings.begin(); l_It != m_Buildings.end(); ++l_It)
        {
            if (l_It->BuildingID == p_BuildingID && l_It->Active)
                return true;
        }

        return false;
    }

    /// Has building type
    bool Manager::HasBuildingType(BuildingType::Type p_BuildingType) const
    {
        for (std::vector<GarrisonBuilding>::const_iterator l_It = m_Buildings.begin(); l_It != m_Buildings.end(); ++l_It)
        {
            GarrBuildingEntry const* l_BuildingEntry = sGarrBuildingStore.LookupEntry(l_It->BuildingID);

            if (!l_BuildingEntry)
                continue;

            if (l_BuildingEntry->BuildingType == p_BuildingType && (*l_It).Active == true)
                return true;
        }

        return false;
    }

    /// Get building max work order
    uint32 Manager::GetBuildingMaxWorkOrder(uint32 p_PlotInstanceID) const
    {
        if (!HasPlotInstance(p_PlotInstanceID))
            return 0;

        uint32 l_BuildingID = GetBuilding(p_PlotInstanceID).BuildingID;

        if (!l_BuildingID)
            return 0;

        GarrBuildingEntry const* l_BuildingEntry = sGarrBuildingStore.LookupEntry(l_BuildingID);

        if (!l_BuildingEntry)
            return 0;

        uint32 l_MaxWorkOrder = 0;

        for (uint32 l_CurrentValue : gGarrisonBuildingMaxWorkOrderPerBuildingLevel)
        {
            if (l_CurrentValue == l_BuildingEntry->BonusAmount)
            {
                l_MaxWorkOrder += l_BuildingEntry->BonusAmount;
                break;
            }
        }

        if (!l_MaxWorkOrder)
            return 0;

        for (uint32 l_I = 0; l_I < m_Buildings.size(); ++l_I)
        {
            GarrBuildingEntry const* l_Building = sGarrBuildingStore.LookupEntry(m_Buildings[l_I].BuildingID);

            if (!l_Building)
                continue;

            if (l_Building->BuildingType != BuildingType::StoreHouse)
                continue;

            l_MaxWorkOrder += l_Building->BonusAmount;
        }

        return l_MaxWorkOrder;
    }

    /// Get in progress work order count
    uint32 Manager::GetWorkOrderCount(uint32 p_PlotInstanceID) const
    {
        return std::count_if(m_WorkOrders.begin(), m_WorkOrders.end(), [p_PlotInstanceID](const GarrisonWorkOrder & p_Order) -> bool
        {
            return p_Order.PlotInstanceID == p_PlotInstanceID;
        });
    }

    /// Start new work order
    uint64 Manager::StartWorkOrder(uint32 p_PlotInstanceID, uint32 p_ShipmentID)
    {
        CharShipmentEntry const* l_ShipmentEntry = sCharShipmentStore.LookupEntry(p_ShipmentID);

        if (!l_ShipmentEntry)
            return 0;

        uint32 l_MaxCompleteTime = time(0);

        for (uint32 l_I = 0; l_I < m_WorkOrders.size(); ++l_I)
        {
            if (m_WorkOrders[l_I].PlotInstanceID == p_PlotInstanceID)
                l_MaxCompleteTime = std::max<uint32>(l_MaxCompleteTime, m_WorkOrders[l_I].CompleteTime);
        }

        GarrisonWorkOrder l_WorkOrder;
        l_WorkOrder.DatabaseID      = sObjectMgr->GetNewGarrisonWorkOrderID();
        l_WorkOrder.PlotInstanceID  = p_PlotInstanceID;
        l_WorkOrder.ShipmentID      = p_ShipmentID;
        l_WorkOrder.CreationTime    = l_MaxCompleteTime;
        l_WorkOrder.CompleteTime    = l_MaxCompleteTime + l_ShipmentEntry->Duration;

        PreparedStatement * l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_GARRISON_WORKORDER);

        uint32 l_Index = 0;
        l_Stmt->setUInt32(l_Index++, l_WorkOrder.DatabaseID);
        l_Stmt->setUInt32(l_Index++, m_ID);
        l_Stmt->setUInt32(l_Index++, l_WorkOrder.PlotInstanceID);
        l_Stmt->setUInt32(l_Index++, l_WorkOrder.ShipmentID);
        l_Stmt->setUInt32(l_Index++, l_WorkOrder.CreationTime);
        l_Stmt->setUInt32(l_Index++, l_WorkOrder.CompleteTime);

        CharacterDatabase.AsyncQuery(l_Stmt);

        m_WorkOrders.push_back(l_WorkOrder);

        return l_WorkOrder.DatabaseID;
    }

    /// Delete work order
    void Manager::DeleteWorkOrder(uint64 p_DBID)
    {
        PreparedStatement * l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GARRISON_WORKORDER);
        l_Stmt->setUInt32(0, p_DBID);
        CharacterDatabase.AsyncQuery(l_Stmt);

        for (std::vector<GarrisonWorkOrder>::iterator l_It = m_WorkOrders.begin(); l_It != m_WorkOrders.end(); ++l_It)
        {
            if (l_It->DatabaseID == p_DBID)
            {
                m_WorkOrders.erase(l_It);
                break;
            }
        }
    }

    /// Get creature plot instance ID
    uint32 Manager::GetCreaturePlotInstanceID(uint64 p_GUID) const
    {
        for (auto & l_Pair : m_PlotsCreatures)
        {
            auto l_It = std::find(l_Pair.second.begin(), l_Pair.second.end(), p_GUID);

            if (l_It != l_Pair.second.end())
                return l_Pair.first;
        }

        return 0;
    }

    /// Get gameobject plot instance ID
    uint32 Manager::GetGameObjectPlotInstanceID(uint64 p_GUID) const
    {
        for (auto & l_Pair : m_PlotsGameObjects)
        {
            auto l_It = std::find(l_Pair.second.begin(), l_Pair.second.end(), p_GUID);

            if (l_It != l_Pair.second.end())
                return l_Pair.first;
        }

        return 0;
    }

    /// Get building gathering data
    /// @p_PlotInstanceID : Plot building location
    std::string Manager::GetBuildingGatheringData(uint32 p_PlotInstanceID)
    {
        for (uint32 l_I = 0; l_I < m_Buildings.size(); ++l_I)
        {
            if (m_Buildings[l_I].PlotInstanceID == p_PlotInstanceID)
                return m_Buildings[l_I].GatheringData;
        }

        return "";
    }

    /// Set building gathering data
    /// @p_PlotInstanceID   : Plot building location
    /// @p_Data             : Gathering data
    void Manager::SetBuildingGatheringData(uint32 p_PlotInstanceID, std::string p_Data)
    {
        for (uint32 l_I = 0; l_I < m_Buildings.size(); ++l_I)
        {
            if (m_Buildings[l_I].PlotInstanceID == p_PlotInstanceID)
            {
                m_Buildings[l_I].GatheringData = p_Data;
                return;
            }
        }
    }

    /// Get list of creature in a specific building type
    /// @p_Type : Building type
    std::vector<uint64> Manager::GetBuildingCreaturesByBuildingType(BuildingType::Type p_Type)
    {
        for (uint32 l_I = 0; l_I < m_Buildings.size(); ++l_I)
        {
            GarrBuildingEntry const* l_BuildingEntry = sGarrBuildingStore.LookupEntry(m_Buildings[l_I].BuildingID);

            if (!l_BuildingEntry)
                continue;

            if (l_BuildingEntry->BuildingType == p_Type && m_Buildings[l_I].Active == true)
                return m_PlotsCreatures[m_Buildings[l_I].PlotInstanceID];
        }

        return std::vector<uint64>();
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Get known blueprints
    std::vector<int32> Manager::GetKnownBlueprints() const
    {
        return m_KnownBlueprints;
    }

    /// Learn blue print
    bool Manager::LearnBlueprint(uint32 p_BuildingRecID)
    {
        LearnBluePrintResults::Type l_ResultCode;

        if (std::find(m_KnownBlueprints.begin(), m_KnownBlueprints.end(), p_BuildingRecID) == m_KnownBlueprints.end())
        {
            GarrBuildingEntry const* l_BuildingEntry = sGarrBuildingStore.LookupEntry(p_BuildingRecID);

            if (l_BuildingEntry)
            {
                m_KnownBlueprints.push_back(p_BuildingRecID);
                l_ResultCode = LearnBluePrintResults::Learned;

                m_Owner->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_LEARN_GARRISON_BLUEPRINTS);
            }
            else
            {
                l_ResultCode = LearnBluePrintResults::UnableToLearn;
            }
        }
        else
        {
            l_ResultCode = LearnBluePrintResults::Known;
        }

        WorldPacket l_Result(SMSG_GARRISON_LEARN_BLUEPRINT_RESULT, 8);
        l_Result << uint32(l_ResultCode);
        l_Result << uint32(p_BuildingRecID);

        m_Owner->SendDirectMessage(&l_Result);

        return true;
    }

    /// Known blue print
    bool Manager::KnownBlueprint(uint32 p_BuildingRecID) const
    {
        return std::find(m_KnownBlueprints.begin(), m_KnownBlueprints.end(), p_BuildingRecID) != m_KnownBlueprints.end();
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Get known specializations
    std::vector<int32> Manager::GetKnownSpecializations() const
    {
        return m_KnownSpecializations;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Get work orders
    std::vector<GarrisonWorkOrder>& Manager::GetWorkOrders()
    {
        return m_WorkOrders;
    }

    /// Check if any followers has ability in parameter
    bool Manager::HasFollowerAbility(uint32 p_AbilityID) const
    {
        std::vector<GarrisonFollower> l_Followers = GetFollowers();

        for (GarrisonFollower l_Follower : l_Followers)
        {
            for (auto l_Ability : l_Follower.Abilities)
            {
                if (l_Ability == p_AbilityID)
                    return true;
            }
        }

        return false;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Init
    void Manager::Init()
    {
        InitDataForLevel();
        InitPlots();
        UpdateStats();
    }

    /// Init data for level
    void Manager::InitDataForLevel()
    {
        GarrSiteLevelEntry const* l_SiteEntry = nullptr;

        /// Search garrison site entry by SideID & Level
        for (uint32 l_I = 0; l_I < sGarrSiteLevelStore.GetNumRows(); ++l_I)
        {
            GarrSiteLevelEntry const* l_CurrentSiteEntry = sGarrSiteLevelStore.LookupEntry(l_I);

            if (l_CurrentSiteEntry && l_CurrentSiteEntry->Level == m_GarrisonLevel && l_CurrentSiteEntry->SiteID == m_GarrisonSiteID)
            {
                l_SiteEntry = l_CurrentSiteEntry;
                break;
            }
        }

        if (l_SiteEntry == nullptr)
        {
            sLog->outAshran("Garrison::InitDataForLevel() not data found");
            assert(false && "Garrison::InitDataForLevel() not data found");
            return;
        }

        m_GarrisonLevelID = l_SiteEntry->SiteLevelID;

        /// Find all plots for garrison level
        m_Plots.clear();

        for (uint32 l_I = 0; l_I < Globals::PlotInstanceCount; ++l_I)
        {
            if (gGarrisonPlotInstanceInfoLocation[l_I].SiteLevelID == m_GarrisonLevelID)
                m_Plots.push_back(gGarrisonPlotInstanceInfoLocation[l_I]);
        }

        /// Add prebuilt buildings
        for (uint32 l_I = 0; l_I < sGarrBuildingStore.GetNumRows(); ++l_I)
        {
            GarrBuildingEntry const* l_BuildingEntry = sGarrBuildingStore.LookupEntry(l_I);

            if (!l_BuildingEntry || l_BuildingEntry->BuildingCategory != BuildingCategory::Prebuilt)
                continue;

            if (HasBuildingType((BuildingType::Type)l_BuildingEntry->BuildingType))
                continue;

            uint32 l_PlotID = 0;

            /// Search building plot ID 
            for (uint32 l_I = 0; l_I < sGarrPlotBuildingStore.GetNumRows(); ++l_I)
            {
                GarrPlotBuildingEntry const* l_PlotBuildingEntry = sGarrPlotBuildingStore.LookupEntry(l_I);

                if (l_PlotBuildingEntry && l_PlotBuildingEntry->BuildingID == l_BuildingEntry->BuildingID)
                {
                    l_PlotID = l_PlotBuildingEntry->PlotId;
                    break;
                }
            }

            if (!l_PlotID)
                continue;

            uint32 l_PlotInstanceID = 0;

            /// Search building plot instance ID
            for (uint32 l_I = 0; l_I < sGarrBuildingStore.GetNumRows(); ++l_I)
            {
                GarrPlotInstanceEntry const* l_PlotInstanceEntry = sGarrPlotInstanceStore.LookupEntry(l_I);

                if (l_PlotInstanceEntry && l_PlotInstanceEntry->PlotID == l_PlotID)
                {
                    l_PlotInstanceID = l_PlotInstanceEntry->InstanceID;
                    break;
                }
            }

            if (!l_PlotInstanceID || !HasPlotInstance(l_PlotInstanceID))
                continue;

            PurchaseBuilding(l_BuildingEntry->BuildingID, l_PlotInstanceID, true);
        }
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Init Game objects
    void Manager::InitPlots()
    {
        if (!m_Owner->IsInGarrison())
            return;

        for (uint32 l_I = 0; l_I < m_Plots.size(); ++l_I)
            UpdatePlot(m_Plots[l_I].PlotInstanceID);
    }

    /// Uninit plots
    void Manager::UninitPlots()
    {
        for (std::map<uint32, uint64>::iterator l_It = m_PlotsActivateGob.begin(); l_It != m_PlotsActivateGob.end(); ++l_It)
        {
            GameObject * l_Gob = HashMapHolder<GameObject>::Find(l_It->second);

            if (l_Gob)
            {
                l_Gob->DestroyForNearbyPlayers();
                l_Gob->AddObjectToRemoveList();
            }
        }

        m_PlotsActivateGob.clear();

        for (std::map<uint32, uint64>::iterator l_It = m_PlotsGob.begin(); l_It != m_PlotsGob.end(); ++l_It)
        {
            GameObject * l_Gob = HashMapHolder<GameObject>::Find(l_It->second);

            if (l_Gob)
            {
                l_Gob->DestroyForNearbyPlayers();
                l_Gob->AddObjectToRemoveList();
            }
        }

        m_PlotsGob.clear();

        for (std::map<uint32, std::vector<uint64>>::iterator l_It = m_PlotsGameObjects.begin(); l_It != m_PlotsGameObjects.end(); ++l_It)
        {
            for (uint32 l_Y = 0; l_Y < l_It->second.size(); ++l_Y)
            {
                GameObject * l_Gob = HashMapHolder<GameObject>::Find(l_It->second[l_Y]);

                if (l_Gob)
                {
                    l_Gob->DestroyForNearbyPlayers();
                    l_Gob->AddObjectToRemoveList();
                }
            }
        }

        m_PlotsGameObjects.clear();

        for (std::map<uint32, std::vector<uint64>>::iterator l_It = m_PlotsCreatures.begin(); l_It != m_PlotsCreatures.end(); ++l_It)
        {
            for (uint32 l_Y = 0; l_Y < l_It->second.size(); ++l_Y)
            {
                Creature * l_Crea = HashMapHolder<Creature>::Find(l_It->second[l_Y]);

                if (l_Crea)
                {
                    l_Crea->DestroyForNearbyPlayers();
                    l_Crea->AddObjectToRemoveList();
                }
            }
        }

        m_PlotsCreatures.clear();
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Update plot game object
    void Manager::UpdatePlot(uint32 p_PlotInstanceID)
    {
        if (!m_Owner->IsInGarrison())
            return;

        GarrisonPlotInstanceInfoLocation    l_PlotInfo = GetPlot(p_PlotInstanceID);
        GarrisonBuilding                    l_Building = GetBuilding(p_PlotInstanceID);
    
        if (m_PlotsGob[p_PlotInstanceID] != 0)
        {
            GameObject * l_Gob = HashMapHolder<GameObject>::Find(m_PlotsGob[p_PlotInstanceID]);

            if (l_Gob)
            {
                m_Owner->SendObjectDeSpawnAnim(m_PlotsGob[p_PlotInstanceID]);
                l_Gob->DestroyForNearbyPlayers();
                l_Gob->AddObjectToRemoveList();
            }

            m_PlotsGob[p_PlotInstanceID] = 0;
        }
    
        m_PlotsWorkOrderGob[p_PlotInstanceID] = 0;

        uint32 l_GobEntry = 0;
        bool l_SpanwActivateGob = false;
        bool l_IsPlotBuilding = false;

        if (PlotIsFree(p_PlotInstanceID))
        {
            l_GobEntry = gGarrisonEmptyPlotGameObject[GetPlotType(p_PlotInstanceID) + (GetGarrisonFactionIndex() * PlotTypes::Max)];
        }
        else
        {
            GarrBuildingEntry const* l_BuildingEntry = sGarrBuildingStore.LookupEntry(l_Building.BuildingID);

            if (!l_BuildingEntry)
                return;

            if (!l_Building.Active && l_BuildingEntry->BuildingCategory != BuildingCategory::Prebuilt)
            {
                l_GobEntry = gGarrisonBuildingPlotGameObject[GetPlotType(p_PlotInstanceID) + (GetGarrisonFactionIndex() * PlotTypes::Max)];

                if (time(0) > l_Building.TimeBuiltEnd)
                    l_SpanwActivateGob  = true;

                l_IsPlotBuilding = true;
            }
            else
            {
                l_GobEntry = l_BuildingEntry->GameObjects[GetGarrisonFactionIndex()];
            }
        }

        if (l_GobEntry != 0)
        {
            GarrBuildingEntry const* l_BuildingEntry = sGarrBuildingStore.LookupEntry(l_Building.BuildingID);
            GameObject * l_Gob = m_Owner->SummonGameObject(l_GobEntry, l_PlotInfo.X, l_PlotInfo.Y, l_PlotInfo.Z, l_PlotInfo.O, 0, 0, 0, 0, 0, 0, 0, 255, 0, true);
        
            if (l_Gob)
            {
                m_PlotsGob[p_PlotInstanceID] = l_Gob->GetGUID();

                if (m_PlotsGameObjects[p_PlotInstanceID].size() != 0 || m_PlotsCreatures[p_PlotInstanceID].size() != 0)
                {
                    for (uint32 l_I = 0; l_I < m_PlotsCreatures[p_PlotInstanceID].size(); ++l_I)
                    {
                        Creature * l_Crea = HashMapHolder<Creature>::Find(m_PlotsCreatures[p_PlotInstanceID][l_I]);

                        if (l_Crea)
                        {
                            l_Crea->SetFlag(OBJECT_FIELD_DYNAMIC_FLAGS, UNIT_DYNFLAG_DISABLE_CLIENT_SIDE);
                            l_Crea->SetFlag(UNIT_FIELD_FLAGS2, UNIT_FLAG2_UNK1);

                            UpdateData l_UpdateData(m_Owner->GetMapId());
                            WorldPacket l_UpdatePacket;

                            l_Crea->BuildValuesUpdateBlockForPlayer(&l_UpdateData, m_Owner);
                                
                            if (l_UpdateData.HasData())
                            {
                                if (l_UpdateData.BuildPacket(&l_UpdatePacket))
                                    m_Owner->SendDirectMessage(&l_UpdatePacket);
                            }

                            l_Crea->DestroyForNearbyPlayers();
                            l_Crea->AddObjectToRemoveList();
                        }
                    }

                    m_PlotsCreatures[p_PlotInstanceID].clear();

                    for (uint32 l_I = 0; l_I < m_PlotsGameObjects[p_PlotInstanceID].size(); ++l_I)
                    {
                        GameObject * l_Gob = HashMapHolder<GameObject>::Find(m_PlotsGameObjects[p_PlotInstanceID][l_I]);

                        if (l_Gob)
                        {
                            m_Owner->SendObjectDeSpawnAnim(m_PlotsGameObjects[p_PlotInstanceID][l_I]);
                            l_Gob->DestroyForNearbyPlayers();
                            l_Gob->AddObjectToRemoveList();
                        }
                    }

                    m_PlotsGameObjects[p_PlotInstanceID].clear();
                }

                G3D::Vector3 l_NonRotatedPosition;
                {
                    G3D::Matrix3 l_Mat = G3D::Matrix3::identity();
                    l_Mat = l_Mat.fromAxisAngle(G3D::Vector3(0, 0, 1), -l_PlotInfo.O);

                    /// transform plot coord
                    l_NonRotatedPosition = l_Mat * G3D::Vector3(l_PlotInfo.X, l_PlotInfo.Y, l_PlotInfo.Z);
                }

                std::vector<GarrisonPlotBuildingContent> l_Contents; 

                if (l_IsPlotBuilding)
                    l_Contents = sObjectMgr->GetGarrisonPlotBuildingContent(GetPlotType(p_PlotInstanceID), GetGarrisonFactionIndex());
                else if ((l_Building.Active || (l_BuildingEntry && l_BuildingEntry->BuildingCategory == BuildingCategory::Prebuilt)) && l_Building.BuildingID)
                    l_Contents = sObjectMgr->GetGarrisonPlotBuildingContent(-(int32)l_Building.BuildingID, GetGarrisonFactionIndex());

                for (uint32 l_I = 0; l_I < l_Contents.size(); ++l_I)
                {
                    if (l_IsPlotBuilding && l_Contents[l_I].PlotTypeOrBuilding < 0)
                        continue;

                    if (!l_IsPlotBuilding)
                    {
                        if (!l_Building.BuildingID)
                            continue;

                        int32 l_NegPlotTypeOrBuilding = -l_Contents[l_I].PlotTypeOrBuilding;

                        if (l_Building.Active && l_NegPlotTypeOrBuilding != l_Building.BuildingID)
                            continue;
                    }

                    G3D::Vector3 l_Position = G3D::Vector3(l_Contents[l_I].X, l_Contents[l_I].Y, 0);

                    G3D::Matrix3 l_Mat = G3D::Matrix3::identity();
                    l_Mat = l_Mat.fromAxisAngle(G3D::Vector3(0, 0, 1), l_PlotInfo.O);

                    l_Position.x += l_NonRotatedPosition.x;
                    l_Position.y += l_NonRotatedPosition.y;

                    l_Position = l_Mat * l_Position;

                    l_Position.z = l_Contents[l_I].Z + l_PlotInfo.Z;

                    if (l_Contents[l_I].CreatureOrGob > 0)
                    {
                        Creature * l_Creature = m_Owner->SummonCreature(l_Contents[l_I].CreatureOrGob, l_Position.x, l_Position.y, l_Position.z, l_Contents[l_I].O + l_PlotInfo.O, TEMPSUMMON_MANUAL_DESPAWN);

                        if (l_Creature)
                        {
                            m_PlotsCreatures[p_PlotInstanceID].push_back(l_Creature->GetGUID());

                            if (l_Creature->AI())
                            {
                                if (l_IsPlotBuilding)
                                    l_Creature->AI()->SetData(CreatureAIDataIDs::Builder, 1);
                                else
                                {
                                    l_Creature->AI()->SetData(CreatureAIDataIDs::BuildingID,    -l_Contents[l_I].PlotTypeOrBuilding);
                                    l_Creature->AI()->SetGUID(m_Owner->GetGUID(), CreatureAIDataIDs::OwnerGuid); ///< Value first, data index next
                                    l_Creature->AI()->SetData(CreatureAIDataIDs::PlotInstanceID, p_PlotInstanceID | (GetGarrisonSiteLevelEntry()->SiteLevelID << 16));
                                }
                            }
                        }
                    }
                    else
                    {
                        GameObject * l_Cosmetic = m_Owner->SummonGameObject(-l_Contents[l_I].CreatureOrGob, l_Position.x, l_Position.y, l_Position.z, l_Contents[l_I].O + l_PlotInfo.O, 0, 0, 0, 0, 0);

                        if (l_Cosmetic)
                        {
                            m_PlotsGameObjects[p_PlotInstanceID].push_back(l_Cosmetic->GetGUID());

                            if (l_Cosmetic->GetGoType() == GAMEOBJECT_TYPE_GARRISON_SHIPMENT)
                                m_PlotsWorkOrderGob[p_PlotInstanceID] = l_Cosmetic->GetGUID();
                        }
                    }
                }

                if (m_PlotsActivateGob[p_PlotInstanceID] != 0)
                {
                    GameObject * l_Gob = HashMapHolder<GameObject>::Find(m_PlotsActivateGob[p_PlotInstanceID]);

                    if (l_Gob)
                    {
                        m_Owner->SendObjectDeSpawnAnim(m_PlotsActivateGob[p_PlotInstanceID]);
                        l_Gob->DestroyForNearbyPlayers();
                        l_Gob->AddObjectToRemoveList();
                    }
                }

                if (l_SpanwActivateGob)
                {
                    /// For this part we use an matrix to transform plot coord in, order to get the position without the rotation
                    /// Once we have the "non rotated" position, we compute activation game object position in a 2 dimensional system
                    /// And after we reapply the rotation on coords to transform and get the correct final position
                    G3D::Matrix3 l_Mat = G3D::Matrix3::identity();
                    l_Mat = l_Mat.fromAxisAngle(G3D::Vector3(0, 0, 1), -l_PlotInfo.O);

                    /// transform plot coord
                    G3D::Vector3 l_NonRotatedPosition = l_Mat * G3D::Vector3(l_PlotInfo.X, l_PlotInfo.Y, l_PlotInfo.Z);

                    GameObjectDisplayInfoEntry const* l_GobDispInfo = sGameObjectDisplayInfoStore.LookupEntry(l_Gob->GetDisplayId());

                    /// Work with plot AABB
                    if (l_GobDispInfo)
                    {
                        /// Get AABB on X axis
                        float l_XAxisSize = fabs(l_GobDispInfo->maxX - l_GobDispInfo->minX) * l_Gob->GetFloatValue(OBJECT_FIELD_SCALE);

                        /// We use a "diminish return" on box size for big plots
                        l_NonRotatedPosition.x += l_XAxisSize * (gGarrisonBuildingPlotAABBDiminishReturnFactor[GetPlotType(p_PlotInstanceID) + (GetGarrisonFactionIndex() * PlotTypes::Max)] / l_XAxisSize);
                    }

                    l_Mat = G3D::Matrix3::identity();
                    l_Mat = l_Mat.fromAxisAngle(G3D::Vector3(0, 0, 1), l_PlotInfo.O);

                    /// Reapply the rotation on coords
                    G3D::Vector3 l_FinalPosition = l_Mat * l_NonRotatedPosition;

                    uint32 l_AnimProgress   = 0;
                    uint32 l_Health         = 255;

                    GameObject * l_ActivationGob = m_Owner->SummonGameObject(gGarrisonBuildingActivationGameObject[GetGarrisonFactionIndex()], l_FinalPosition.x, l_FinalPosition.y, l_FinalPosition.z, l_PlotInfo.O, 0, 0, 0, 0, 0, 0, 0, l_AnimProgress, l_Health);
                
                    if (l_ActivationGob)
                    {
                        m_PlotsActivateGob[p_PlotInstanceID] = l_ActivationGob->GetGUID();

                        l_ActivationGob->SendGameObjectActivateAnimKit(1696);
                    }
                }
            }
        }
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Update garrison stats
    void Manager::UpdateStats()
    {
        uint32 l_BonusMaxActiveFollower = 0;

        /// Some of const values used here are unknown
        /// See GetFollowerSoftCap in client for more details
        for (uint32 l_I = 0; l_I < m_Buildings.size(); ++l_I)
        {
            GarrBuildingEntry const* l_Building = sGarrBuildingStore.LookupEntry(m_Buildings[l_I].BuildingID);

            if (!l_Building)
                continue;

            if (l_Building->BuildingType != BuildingType::Barracks)
                continue;

            l_BonusMaxActiveFollower = l_Building->Unk7;

            for (uint32 l_Y = 0; l_Y < sGarrSpecializationStore.GetNumRows(); ++l_Y)
            {
                GarrSpecializationEntry const* l_Specialization = sGarrSpecializationStore.LookupEntry(l_Y);

                if (!l_Specialization)
                    continue;

                if (   l_Specialization->Unk2 == l_Building->BuildingType
                    && l_Specialization->Unk4 <= l_Building->BuildingLevel
                    && l_Specialization->Unk3 == 10)
                {
                    l_BonusMaxActiveFollower += floor(l_Specialization->BasePoint);
                }
            }
        }

        m_Stat_MaxActiveFollower = l_BonusMaxActiveFollower + Globals::MaxActiveFollower;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Update building
    void Manager::UpdateBuildings()
    {
        /// Update building in construction
        for (uint32 l_I = 0; l_I < m_Buildings.size(); ++l_I)
        {
            GarrisonBuilding * l_Building = &m_Buildings[l_I];

            if (!l_Building->Active && !l_Building->BuiltNotified && time(0) > l_Building->TimeBuiltEnd)
            {
                l_Building->BuiltNotified = true;

                /// Nothing more needed, client auto deduce notification
                UpdatePlot(l_Building->PlotInstanceID);
            }
        }
    }

    /// Update followers
    void Manager::UpdateFollowers()
    {
        /// Update follower activation count
        if (m_NumFollowerActivation < Globals::FollowerActivationMaxStack && (time(0) - m_NumFollowerActivationRegenTimestamp) > DAY)
        {
            m_NumFollowerActivation++;
            m_NumFollowerActivationRegenTimestamp = time(0);

            WorldPacket l_Data(SMSG_GARRISON_UPDATE_FOLLOWER_ACTIVATION_COUNT, 4);
            l_Data << uint32(GetNumFollowerActivationsRemaining());

            m_Owner->SendDirectMessage(&l_Data);
        }
    }

    /// Update cache
    void Manager::UpdateCache()
    {
        if (!m_Owner->IsInGarrison())
            return;

        Interfaces::GarrisonSite * l_GarrisonScript = GetGarrisonScript();

        /// Update garrison cache
        if (m_CacheGameObjectGUID && HashMapHolder<GameObject>::Find(m_CacheGameObjectGUID) == nullptr)
            m_CacheGameObjectGUID = 0;

        if (!l_GarrisonScript || !l_GarrisonScript->CanUseGarrisonCache(m_Owner))
        {
            if (m_CacheGameObjectGUID != 0)
            {
                GameObject * l_Cache = HashMapHolder<GameObject>::Find(m_CacheGameObjectGUID);

                if (l_Cache)
                {
                    l_Cache->DestroyForNearbyPlayers();
                    l_Cache->AddObjectToRemoveList();
                }

                m_CacheGameObjectGUID = 0;
            }

            return;
        }

        uint32 l_NumRessourceGenerated = std::min((uint32)((time(0) - m_CacheLastUsage) / Globals::CacheTokenGenerateTime), (uint32)Globals::CacheMaxToken);

        if (!m_CacheGameObjectGUID)
        {
            m_CacheLastTokenAmount = l_NumRessourceGenerated;
            m_Owner->SendUpdateWorldState(WorldStates::CacheNumToken, l_NumRessourceGenerated);

            if (l_NumRessourceGenerated >= Globals::CacheMinToken)
            {
                /// Get display ID
                uint32 l_DisplayIDOffset    = l_NumRessourceGenerated == Globals::CacheMaxToken ? 2 : ((l_NumRessourceGenerated > Globals::CacheHeftyToken) ? 1 : 0);
                const uint32 & l_DisplayID  = gGarrisonCacheGameObjectID[(GetGarrisonFactionIndex() * 3) + l_DisplayIDOffset];

                /// Destroy old cache if exist
                GameObject * l_Cache = HashMapHolder<GameObject>::Find(m_CacheGameObjectGUID);

                if (l_Cache)
                {
                    l_Cache->DestroyForNearbyPlayers();
                    l_Cache->AddObjectToRemoveList();
                }

                m_CacheGameObjectGUID = 0;

                /// Create new one
                if (m_Owner->IsInGarrison())
                {
                    /// Extract new location
                    const GarrisonCacheInfoLocation & l_Location = gGarrisonCacheInfoLocation[(GetGarrisonFactionIndex() * Globals::MaxLevel) + (m_GarrisonLevel - 1)];
                    l_Cache = m_Owner->SummonGameObject(l_DisplayID, l_Location.X, l_Location.Y, l_Location.Z, l_Location.O, 0, 0, 0, 0, 0);

                    if (l_Cache)
                        m_CacheGameObjectGUID = l_Cache->GetGUID();
                }

            }
        }
    
        if (m_CacheGameObjectGUID && l_NumRessourceGenerated < Globals::CacheMinToken)
        {
            GameObject * l_Cache = HashMapHolder<GameObject>::Find(m_CacheGameObjectGUID);

            if (l_Cache)
            {
                l_Cache->DestroyForNearbyPlayers();
                l_Cache->AddObjectToRemoveList();
            }

            m_CacheGameObjectGUID = 0;
        }
    }

    /// Update mission distribution
    void Manager::UpdateMissionDistribution()
    {
        /// Do ramdom mission distribution
        if ((time(0) - m_MissionDistributionLastUpdate) > Globals::MissionDistributionInterval)
        {
            /// Random, no detail about how blizzard do
            uint32 l_MaxMissionCount         = ceil(m_Followers.size() * GARRISON_MISSION_DISTRIB_FOLLOWER_COEFF);
            uint32 l_CurrentAvailableMission = 0;

            std::for_each(m_Missions.begin(), m_Missions.end(), [&l_CurrentAvailableMission](const GarrisonMission & p_Mission) -> void
            {
                if (p_Mission.State == MissionStates::Available && (p_Mission.OfferTime + p_Mission.OfferMaxDuration) > time(0))
                    l_CurrentAvailableMission++;
            });

            if (l_CurrentAvailableMission < l_MaxMissionCount)
            {
                uint32 l_MaxFollowerLevel = 90;
                uint32 l_MaxFollowerItemLevel = 600;

                std::for_each(m_Followers.begin(), m_Followers.end(), [&l_MaxFollowerLevel, &l_MaxFollowerItemLevel](const GarrisonFollower & p_Follower) -> void
                {
                    l_MaxFollowerLevel      = std::max(l_MaxFollowerLevel, (uint32)p_Follower.Level);
                    l_MaxFollowerItemLevel  = std::max(l_MaxFollowerItemLevel, (uint32)((p_Follower.ItemLevelArmor + p_Follower.ItemLevelWeapon) / 2));
                });

                std::vector<const GarrMissionEntry*> l_Candidates;

                for (uint32 l_I = 0; l_I < sGarrMissionStore.GetNumRows(); ++l_I)
                {
                    GarrMissionEntry const* l_Entry = sGarrMissionStore.LookupEntry(l_I);

                    if (!l_Entry)
                        continue;

                    uint32 l_Count = std::count_if(m_Missions.begin(), m_Missions.end(), [l_Entry](const GarrisonMission & p_Mission)
                    {
                        return p_Mission.MissionID == l_Entry->MissionRecID;
                    });

                    if (l_Count)
                        continue;

                    if (l_Entry->RequiredFollowersCount > m_Followers.size())
                        continue;

                    if (l_Entry->Duration <= 10)
                        continue;

                    if (l_Entry->RequiredFollowersCount > Globals::MaxFollowerPerMission)
                        continue;

                    uint32 l_RewardCount = 0;
                    for (uint32 l_RewardIT = 0; l_RewardIT < sGarrMissionRewardStore.GetNumRows(); ++l_RewardIT)
                    {
                        GarrMissionRewardEntry const* l_RewardEntry = sGarrMissionRewardStore.LookupEntry(l_RewardIT);

                        if (l_RewardEntry && l_RewardEntry->MissionID == l_Entry->MissionRecID)
                            l_RewardCount++;
                    }

                    /// All missions should have a reward
                    if (!l_RewardCount)
                        continue;

                    /// Max Level cap : 2
                    if (l_Entry->RequiredLevel > (int32)(l_MaxFollowerLevel + 2))
                        continue;

                    if (l_Entry->RequiredItemLevel > (int32)l_MaxFollowerItemLevel)
                        continue;

                    l_Candidates.push_back(l_Entry);
                }

                uint32 l_ShuffleCount = std::rand() % 4;

                for (uint32 l_I = 0; l_I < l_ShuffleCount; ++l_I)
                    std::random_shuffle(l_Candidates.begin(), l_Candidates.end());

                int32 l_MissionToAddCount = (int32)l_MaxMissionCount - (int32)l_CurrentAvailableMission;

                if (l_MissionToAddCount > 0)
                {
                    l_MissionToAddCount = std::min(l_MissionToAddCount, (int32)l_Candidates.size());

                    for (int32 l_I = 0; l_I < l_MissionToAddCount; ++l_I)
                        AddMission(l_Candidates[l_I]->MissionRecID);
                }
            }

            m_MissionDistributionLastUpdate = time(0);
        }
    }

    /// Update garrison ability
    void Manager::UpdateGarrisonAbility()
    {
        if (!m_Owner->IsInWorld())
            return;

        uint32 l_AbilityOverrideSpellID = 0;

        switch (GetGarrisonFactionIndex())
        {
            case FactionIndex::Alliance:
                l_AbilityOverrideSpellID = GARRISON_SPELL_GARR_ABILITY_ALLIANCE_BASE;
                break;

            case FactionIndex::Horde:
                l_AbilityOverrideSpellID = GARRISON_SPELL_GARR_ABILITY_HORDE_BASE;
                break;

            case FactionIndex::Max:
            default:
                ASSERT(false);
                break;

        }

        if ((m_Owner->IsInGarrison() || m_Owner->GetMapId() == Globals::BaseMap) && HasBuildingType(BuildingType::Barracks))
        {
            if (!m_Owner->HasAura(l_AbilityOverrideSpellID))
                m_Owner->AddAura(l_AbilityOverrideSpellID, m_Owner);
        }
        else
        {
            if (m_Owner->HasAura(l_AbilityOverrideSpellID))
                m_Owner->RemoveAura(l_AbilityOverrideSpellID);
        }
    }

    /// Update work order
    void Manager::UpdateWorkOrders()
    {
        if (!m_Owner->IsInGarrison())
            return;

        for (uint32 l_PlotI = 0; l_PlotI < m_Plots.size(); ++l_PlotI)
        {
            uint32 l_PlotInstanceID = m_Plots[l_PlotI].PlotInstanceID;

            if (m_PlotsWorkOrderGob[l_PlotInstanceID] == 0)
                continue;

            GameObject * l_WorkOrderGameObject = HashMapHolder<GameObject>::Find(m_PlotsWorkOrderGob[l_PlotInstanceID]);

            if (!l_WorkOrderGameObject)
                continue;

            std::vector<GarrisonWorkOrder*> l_PlotWorkOrder;

            for (uint32 l_OrderI = 0; l_OrderI < m_WorkOrders.size(); ++l_OrderI)
            {
                if (m_WorkOrders[l_OrderI].PlotInstanceID == l_PlotInstanceID)
                    l_PlotWorkOrder.push_back(&m_WorkOrders[l_OrderI]);
            }

            if (l_PlotWorkOrder.size() > 0)
            {
                bool l_AllComplete = l_PlotWorkOrder.size() > 1;

                uint32 l_CurrentTimeStamp = time(0);
                for (uint32 l_OrderI = 0; l_OrderI < l_PlotWorkOrder.size(); ++l_OrderI)
                {
                    if (l_PlotWorkOrder[l_OrderI]->CompleteTime > l_CurrentTimeStamp)
                        l_AllComplete = false;
                    else if (l_PlotWorkOrder.size() == 1)
                        l_AllComplete = true;
                }

                if (l_AllComplete)
                {
                    l_WorkOrderGameObject->SetDisplayId(WorkOrderGODisplayID::AllComplete);
                    l_WorkOrderGameObject->RemoveFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_ACTIVATED);
                }
                else
                {
                    /// Keep original displayID for Barn Work Order
                    if (l_WorkOrderGameObject->GetEntry() != GarrisonBuildingWorkOrderGameObject::GobBarnWOrkOrder)
                        l_WorkOrderGameObject->SetDisplayId(GetGarrisonFactionIndex() == FactionIndex::Alliance ? WorkOrderGODisplayID::BaseA : WorkOrderGODisplayID::BaseH);
                    else
                        l_WorkOrderGameObject->SetDisplayId(WorkOrderGODisplayID::BaseBarn);

                    l_WorkOrderGameObject->SetFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_ACTIVATED);
                }
            }
            else
            {
                /// Keep original displayID for Barn Work Order
                if (l_WorkOrderGameObject->GetEntry() != GarrisonBuildingWorkOrderGameObject::GobBarnWOrkOrder)
                    l_WorkOrderGameObject->SetDisplayId(GetGarrisonFactionIndex() == FactionIndex::Alliance ? WorkOrderGODisplayID::BaseA : WorkOrderGODisplayID::BaseH);
                else
                    l_WorkOrderGameObject->SetDisplayId(WorkOrderGODisplayID::BaseBarn);

                l_WorkOrderGameObject->RemoveFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_ACTIVATED);
            }
        }
    }

}   ///< namespace Garrison
}   ///< namespace MS
