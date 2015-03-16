////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#include "GarrisonHordeLevel1.hpp"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "GameObjectAI.h"
#include "Spell.h"

enum
{
    GARRISON_PHASE_BASE                 = 0x0001,
    GARRISON_PHASE_BUILD_YOUR_BARRACKS  = 0x0002,
};

namespace MS { namespace Garrison { namespace Sites
{
    /// Constructor
    InstanceMapScript_GarrisonHordeLevel1::InstanceMapScript_GarrisonHordeLevel1()
        : InstanceMapScript("instance_Garrison_H1", MapIDs::MAP_GARRISON_HORDE_LEVEL_1)
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Get an instance script instance
    /// @p_Map : Target map
    InstanceScript * InstanceMapScript_GarrisonHordeLevel1::GetInstanceScript(InstanceMap * p_Map) const
    {
        return new InstanceScript_GarrisonHordeLevel1(p_Map);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    /// @p_Map : Target map
    InstanceScript_GarrisonHordeLevel1::InstanceScript_GarrisonHordeLevel1(Map * p_Map)
        : GarrisonSiteBase(p_Map)
    {
        SetData(InstanceDataIDs::GARRISON_INSTANCE_DATA_PEON_ENABLED, 0);
    }
    /// Destructor
    InstanceScript_GarrisonHordeLevel1::~InstanceScript_GarrisonHordeLevel1()
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// When instance script is updated
    /// @p_Diff : Time since last update
    void InstanceScript_GarrisonHordeLevel1::Update(uint32 p_Diff)
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// When the owner enter in the garrison
    /// @p_Owner : Owner instance
    void InstanceScript_GarrisonHordeLevel1::OnOwnerEnter(Player * p_Owner)
    {
        if (p_Owner->GetQuestStatus(Quests::QUEST_WHAT_WE_GOT) == QUEST_STATUS_REWARDED || p_Owner->GetQuestObjectiveCounter(273085) == 1)
        {
            SetData(InstanceDataIDs::GARRISON_INSTANCE_DATA_PEON_ENABLED, 1);

            if (!m_CreaturesPerEntry[NPCs::NPC_FROSTWALL_PEON_DYNAMIC].empty())
            {
                for (uint64 l_DynamicPeon : m_CreaturesPerEntry[NPCs::NPC_FROSTWALL_PEON_DYNAMIC])
                {
                    Creature * l_Creature = HashMapHolder<Creature>::Find(*(m_CreaturesPerEntry[NPCs::NPC_FROSTWALL_PEON_DYNAMIC].begin()));

                    if (l_Creature && l_Creature->AI())
                        l_Creature->AI()->SetData(CreatureAIDataIDs::PeonWorking, HordePeonData::PHASE_WOODCUTTING);
                }
            }
        }

        if (p_Owner->GetQuestObjectiveCounter(275373) == 1 && !m_CreaturesPerEntry[NPCs::NPC_SENIOR_PEON_II].empty())
        {
            Creature * l_Creature = HashMapHolder<Creature>::Find(*(m_CreaturesPerEntry[NPCs::NPC_SENIOR_PEON_II].begin()));

            if (l_Creature && l_Creature->GetDistance2d(5593.f, 4476.f) > 2)
            {
                l_Creature->SetWalk(false);
                l_Creature->LoadPath(Waypoints::MOVE_PATH_SENIOR_PEON_II);
                l_Creature->SetDefaultMovementType(WAYPOINT_MOTION_TYPE);
                l_Creature->GetMotionMaster()->Initialize();
            }
        }
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// When the garrison owner started a quest
    /// @p_Owner : Garrison owner
    /// @p_Quest : Started quest
    void InstanceScript_GarrisonHordeLevel1::OnQuestStarted(Player * p_Owner, const Quest * p_Quest)
    {
        /// Hack fix for storehouse, need more work
        if (p_Owner && p_Quest && p_Quest->GetQuestId() == Quests::Horde_LostInTransition)
            p_Owner->CompleteQuest(Quests::Horde_LostInTransition);
    }
    /// When the garrison owner reward a quest
    /// @p_Owner : Garrison owner
    /// @p_Quest : Rewarded quest
    void InstanceScript_GarrisonHordeLevel1::OnQuestReward(Player * p_Owner, const Quest * p_Quest)
    {

    }
    /// When the garrison owner abandon a quest
    /// @p_Owner : Garrison owner
    /// @p_Quest : Abandoned quest
    void InstanceScript_GarrisonHordeLevel1::OnQuestAbandon(Player * p_Owner, const Quest * p_Quest)
    {
        if (p_Quest && p_Quest->GetQuestId() == Quests::QUEST_WHAT_WE_GOT)
        {
            if (!m_CreaturesPerEntry[NPCs::NPC_SENIOR_PEON_II].empty())
            {
                Creature * l_Creature = HashMapHolder<Creature>::Find(*(m_CreaturesPerEntry[NPCs::NPC_SENIOR_PEON_II].begin()));

                if (l_Creature && l_Creature->GetDistance2d(5593.f, 4476.f) < 2)
                {
                    l_Creature->SetWalk(false);
                    l_Creature->LoadPath(Waypoints::MOVE_PATH_SENIOR_PEON_II_BACK);
                    l_Creature->SetDefaultMovementType(WAYPOINT_MOTION_TYPE);
                    l_Creature->GetMotionMaster()->Initialize();
                }
            }

            SetData(InstanceDataIDs::GARRISON_INSTANCE_DATA_PEON_ENABLED, 0);

            if (!m_CreaturesPerEntry[NPCs::NPC_FROSTWALL_PEON_DYNAMIC].empty())
            {
                for (uint64 l_DynamicPeon : m_CreaturesPerEntry[NPCs::NPC_FROSTWALL_PEON_DYNAMIC])
                {
                    Creature * l_Creature = HashMapHolder<Creature>::Find(*(m_CreaturesPerEntry[NPCs::NPC_FROSTWALL_PEON_DYNAMIC].begin()));

                    if (l_Creature && l_Creature->AI())
                        l_Creature->AI()->SetData(CreatureAIDataIDs::PeonWorking, HordePeonData::PHASE_BACK_TO_HOME);
                }
            }
        }
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Get phase mask
    /// @p_Owner : Garrison owner
    uint32 InstanceScript_GarrisonHordeLevel1::GetPhaseMask(Player * p_Owner)
    {
        uint32 l_PhaseMask = GARRISON_PHASE_BASE;

        if (p_Owner->HasQuest(Quests::QUEST_BUILD_YOUR_BARRACKS))
            l_PhaseMask |= GARRISON_PHASE_BUILD_YOUR_BARRACKS;

        return l_PhaseMask;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Owner can use the garrison cache ?
    /// @p_Owner : Garrison owner
    bool InstanceScript_GarrisonHordeLevel1::CanUseGarrisonCache(Player * p_Owner)
    {
        if (p_Owner->GetQuestStatus(Quests::QUEST_WHAT_WE_GOT) == QUEST_STATUS_REWARDED
            || p_Owner->HasQuest(Quests::QUEST_WHAT_WE_GOT))
            return true;

        return false;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Can upgrade the garrison
    /// @p_Owner                : Garrison owner
    /// @p_CurrentGarrisonLevel : Current garrison level
    bool InstanceScript_GarrisonHordeLevel1::CanUpgrade(Player * p_Owner, uint32 p_CurrentGarrisonLevel)
    {
        if (p_CurrentGarrisonLevel != 1)
            return false;

        if (p_Owner->getLevel() < 93)
            return false;

        if (!p_Owner->HasQuest(Quests::Horde_BiggerIsBetter))
            return false;

        return true;
    }

    /// On upgrade the garrison
    /// @p_Owner : Garrison owner
    void InstanceScript_GarrisonHordeLevel1::OnUpgrade(Player * p_Owner)
    {
        GarrSiteLevelEntry const* l_Entry = p_Owner->GetGarrison()->GetGarrisonSiteLevelEntry();

        if (!l_Entry)
            return;

        p_Owner->AddMovieDelayedTeleport(l_Entry->CreationMovie, l_Entry->MapID, 5754.82f, 4495.425f, 132.50f, 2.90f);
        p_Owner->SendMovieStart(l_Entry->CreationMovie);
        p_Owner->CompleteQuest(Quests::Horde_BiggerIsBetter);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// When a mission start
    /// @p_Owner     : Garrison owner
    /// @p_MissionID : Started mission ID
    /// @p_Followers : Followers on the mission
    void InstanceScript_GarrisonHordeLevel1::OnMissionStart(Player * p_Owner, uint32 p_MissionID, std::vector<uint32> p_Followers)
    {

    }
    /// When a construction start, compute build time
    /// @p_Owner      : Garrison owner
    /// @p_BuildingID : Purchased building ID
    /// @p_BaseTime   : Default build time
    uint32 InstanceScript_GarrisonHordeLevel1::OnPrePurchaseBuilding(Player * p_Owner, uint32 p_BuildingID, uint32 p_BaseTime)
    {
        /// Build your Barracks quest
        if (p_BuildingID == Buildings::Barracks__Barracks_Level1 && p_Owner->HasQuest(Quests::QUEST_BUILD_YOUR_BARRACKS))
            return 2;   ///< 2 second, unk retail value

        return p_BaseTime;
    }
    /// When a construction start
    /// @p_Owner      : Garrison owner
    /// @p_BuildingID : Purchased building ID
    void InstanceScript_GarrisonHordeLevel1::OnPurchaseBuilding(Player * p_Owner, uint32 p_BuildingID)
    {
        /// Build your Barracks quest
        if (p_BuildingID == Buildings::Barracks__Barracks_Level1 && p_Owner->HasQuest(Quests::QUEST_BUILD_YOUR_BARRACKS))
        {
            p_Owner->QuestObjectiveSatisfy(36167, 1, QUEST_OBJECTIVE_TYPE_CRITERIA, p_Owner->GetGUID());

            if (!m_CreaturesPerEntry[NPCs::NPC_GASLOWE].empty())
            {
                Creature * l_Creature = HashMapHolder<Creature>::Find(*(m_CreaturesPerEntry[NPCs::NPC_GASLOWE].begin()));

                if (l_Creature && l_Creature->AI())
                {
                    l_Creature->AI()->Talk(0);

                    l_Creature->SetWalk(false);
                    l_Creature->GetMotionMaster()->MovePoint(MovePointIDs::MOVE_POINT_GAZLOWE_BARRACK_A, 5590.83f, 4481.74f, 129.92f);

                    p_Owner->QuestObjectiveSatisfy(NPCs::NPC_GASLOWE, 1, QUEST_OBJECTIVE_TYPE_NPC, p_Owner->GetGUID());
                }
            }
        }
    }
    /// When a building is activated
    /// @p_Owner      : Garrison owner
    /// @p_BuildingID : Purchased building ID
    void InstanceScript_GarrisonHordeLevel1::OnBuildingActivated(Player * p_Owner, uint32 p_BuildingID)
    {
        /// Build your Barracks quest
        if (p_BuildingID == Buildings::Barracks__Barracks_Level1 && p_Owner->HasQuest(Quests::QUEST_BUILD_YOUR_BARRACKS))
        {
            p_Owner->QuestObjectiveSatisfy(35753, 1, QUEST_OBJECTIVE_TYPE_CRITERIA, p_Owner->GetGUID());
        }
    }

}   ///< namespace Sites
}   ///< namespace Garrison
}   ///< namespace MS

void AddSC_Garrison_H1()
{
    new MS::Garrison::Sites::InstanceMapScript_GarrisonHordeLevel1();
}