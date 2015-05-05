////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

#include "highmaul.hpp"

DoorData const g_DoorData[] =
{
    { eHighmaulGameobjects::GateArenaExit,      eHighmaulDatas::BossKargathBladefist,   DoorType::DOOR_TYPE_PASSAGE,    BoundaryType::BOUNDARY_NONE },
    { eHighmaulGameobjects::EarthenPillar,      eHighmaulDatas::BossTheButcher,         DoorType::DOOR_TYPE_ROOM,       BoundaryType::BOUNDARY_NONE },
    { eHighmaulGameobjects::FungalGiantDoor,    eHighmaulDatas::BossTheButcher,         DoorType::DOOR_TYPE_PASSAGE,    BoundaryType::BOUNDARY_NONE },
    { eHighmaulGameobjects::WindDoor,           eHighmaulDatas::BossTheButcher,         DoorType::DOOR_TYPE_PASSAGE,    BoundaryType::BOUNDARY_NONE },
    { eHighmaulGameobjects::WindDoor,           eHighmaulDatas::BossBrackenspore,       DoorType::DOOR_TYPE_ROOM,       BoundaryType::BOUNDARY_NONE },
    { 0,                                        0,                                      DoorType::DOOR_TYPE_ROOM,       BoundaryType::BOUNDARY_NONE } ///< End
};

class instance_highmaul : public InstanceMapScript
{
    public:
        instance_highmaul() : InstanceMapScript("instance_highmaul", 1228) { }

        struct instance_highmaul_InstanceMapScript : public InstanceScript
        {
            instance_highmaul_InstanceMapScript(Map* p_Map) : InstanceScript(p_Map)
            {
                m_ArenaMasterGuid           = 0;

                m_KargathBladefistGuid      = 0;
                m_JhornTheMadGuid           = 0;
                m_ThoktarIronskullGuid      = 0;
                m_ArenaElevatorGuid         = 0;
                m_ArenaElevatorActivated    = false;
                m_CollisionWallGuid         = 0;
                m_VulgorGuid                = 0;
                m_GateArenaInnerGuid        = 0;
                m_CrowdAreatriggerGuid      = 0;
                m_MargokCosmeticGuid        = 0;

                for (uint8 l_I = eHighmaulDatas::RaidGrate001; l_I < eHighmaulDatas::MaxRaidGrates; ++l_I)
                    m_RaidGrateGuids[l_I]   = 0;

                m_IronBombersCount          = 0;
                m_DrunkenBileslingerCount   = 0;

                m_TheButcherGuid            = 0;

                m_BrackensporeGuid          = 0;
            }

            uint64 m_ArenaMasterGuid;

            /// The Coliseum
            uint64 m_KargathBladefistGuid;
            uint64 m_JhornTheMadGuid;
            uint64 m_ThoktarIronskullGuid;
            uint64 m_ArenaElevatorGuid;
            bool   m_ArenaElevatorActivated;
            uint64 m_CollisionWallGuid;
            uint64 m_VulgorGuid;
            uint64 m_GateArenaInnerGuid;
            uint64 m_CrowdAreatriggerGuid;
            uint64 m_MargokCosmeticGuid;
            uint64 m_RaidGrateGuids[4];
            uint8  m_IronBombersCount;
            uint8  m_DrunkenBileslingerCount;

            /// The Underbelly
            uint64 m_TheButcherGuid;

            /// Gorian Strand
            uint64 m_BrackensporeGuid;

            void Initialize() override
            {
                SetBossNumber(eHighmaulDatas::MaxHighmaulBosses);
                LoadDoorData(g_DoorData);

                instance->SetObjectVisibility(500.0f);
            }

            void OnCreatureCreate(Creature* p_Creature) override
            {
                switch (p_Creature->GetEntry())
                {
                    case eHighmaulCreatures::GhargArenaMaster:
                        m_ArenaMasterGuid = p_Creature->GetGUID();
                        break;
                    case eHighmaulCreatures::KargathBladefist:
                        m_KargathBladefistGuid = p_Creature->GetGUID();
                        break;
                    case eHighmaulCreatures::JhornTheMad:
                        m_JhornTheMadGuid = p_Creature->GetGUID();
                        break;
                    case eHighmaulCreatures::ThoktarIronskull:
                        m_ThoktarIronskullGuid = p_Creature->GetGUID();
                        break;
                    case eHighmaulCreatures::Vulgor:
                        m_VulgorGuid = p_Creature->GetGUID();
                        break;
                    case eHighmaulCreatures::CrowdAreatrigger:
                        m_CrowdAreatriggerGuid = p_Creature->GetGUID();
                        break;
                    case eHighmaulCreatures::MargokCosmetic:
                        m_MargokCosmeticGuid = p_Creature->GetGUID();
                        break;
                    case eHighmaulCreatures::IronBomberSpawner:
                        p_Creature->SetReactState(ReactStates::REACT_PASSIVE);
                        p_Creature->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE);
                        break;
                    case eHighmaulCreatures::IronBomber:
                        if (!m_IronBombersCount)
                            SendUpdateWorldState(eHighmaulWorldStates::IronBomberEnable, 1);
                        ++m_IronBombersCount;
                        SendUpdateWorldState(eHighmaulWorldStates::IronBomberRemaining, m_IronBombersCount);
                        break;
                    case eHighmaulCreatures::DrunkenBileslinger:
                        if (!m_DrunkenBileslingerCount)
                            SendUpdateWorldState(eHighmaulWorldStates::DrunkenBileslingerEnable, 1);
                        ++m_DrunkenBileslingerCount;
                        SendUpdateWorldState(eHighmaulWorldStates::DrunkenBileslingerRemaining, m_DrunkenBileslingerCount);
                        break;
                    case eHighmaulCreatures::TheButcher:
                        m_TheButcherGuid = p_Creature->GetGUID();
                        break;
                    case eHighmaulCreatures::Brackenspore:
                        m_BrackensporeGuid = p_Creature->GetGUID();
                        break;
                    default:
                        break;
                }
            }

            void OnCreatureRemove(Creature* p_Creature) override
            {
                switch (p_Creature->GetEntry())
                {
                    case eHighmaulCreatures::IronBomber:
                    {
                        if (!m_IronBombersCount)
                            break;

                        --m_IronBombersCount;
                        SendUpdateWorldState(eHighmaulWorldStates::IronBomberRemaining, m_IronBombersCount);

                        if (!m_IronBombersCount)
                        {
                            SendUpdateWorldState(eHighmaulWorldStates::IronBomberEnable, 0);
                            break;
                        }

                        break;
                    }
                    case eHighmaulCreatures::DrunkenBileslinger:
                    {
                        if (!m_DrunkenBileslingerCount)
                            break;

                        --m_DrunkenBileslingerCount;
                        SendUpdateWorldState(eHighmaulWorldStates::DrunkenBileslingerRemaining, m_DrunkenBileslingerCount);

                        if (!m_DrunkenBileslingerCount)
                        {
                            SendUpdateWorldState(eHighmaulWorldStates::DrunkenBileslingerEnable, 0);
                            break;
                        }

                        break;
                    }
                    default:
                        break;
                }
            }

            void OnGameObjectCreate(GameObject* p_GameObject) override
            {
                switch (p_GameObject->GetEntry())
                {
                    case eHighmaulGameobjects::GateArenaExit:
                    case eHighmaulGameobjects::FungalGiantDoor:
                    case eHighmaulGameobjects::EarthenPillar:
                    case eHighmaulGameobjects::WindDoor:
                        AddDoor(p_GameObject, true);
                        break;
                    case eHighmaulGameobjects::ArenaElevator:
                        m_ArenaElevatorGuid = p_GameObject->GetGUID();
                        p_GameObject->SetTransportState(GOState::GO_STATE_TRANSPORT_ACTIVE);
                        break;
                    case eHighmaulGameobjects::CollisionWall:
                        m_CollisionWallGuid = p_GameObject->GetGUID();
                        p_GameObject->SetGoState(GOState::GO_STATE_ACTIVE);
                        break;
                    case eHighmaulGameobjects::GateArenaInner:
                        m_GateArenaInnerGuid = p_GameObject->GetGUID();
                        break;
                    case eHighmaulGameobjects::RaidGrate1:
                        m_RaidGrateGuids[eHighmaulDatas::RaidGrate001] = p_GameObject->GetGUID();
                        break;
                    case eHighmaulGameobjects::RaidGrate2:
                        m_RaidGrateGuids[eHighmaulDatas::RaidGrate002] = p_GameObject->GetGUID();
                        break;
                    case eHighmaulGameobjects::RaidGrate3:
                        m_RaidGrateGuids[eHighmaulDatas::RaidGrate003] = p_GameObject->GetGUID();
                        break;
                    case eHighmaulGameobjects::RaidGrate4:
                        m_RaidGrateGuids[eHighmaulDatas::RaidGrate004] = p_GameObject->GetGUID();
                        break;
                    default:
                        break;
                }
            }

            bool SetBossState(uint32 p_BossID, EncounterState p_State) override
            {
                if (!InstanceScript::SetBossState(p_BossID, p_State))
                    return false;

                switch (p_BossID)
                {
                    case eHighmaulDatas::BossKargathBladefist:
                    {
                        if (p_State != EncounterState::DONE)
                            break;
                        SendUpdateWorldState(eHighmaulWorldStates::DisableCrowdSound, 1);
                        SendUpdateWorldState(eHighmaulWorldStates::UnknownHighmaulWorldState, 0);
                        SendUpdateWorldState(eHighmaulWorldStates::UnknownHighmaulWorldState2, 0);
                        PlaySceneForPlayers(g_PlayScenePos, 1338);
                        CastSpellToPlayers(instance, nullptr, eHighmaulSpells::ChogallNight, true);
                        break;
                    }
                    default:
                        break;
                }

                return true;
            }

            void SetData(uint32 p_Type, uint32 p_Data) override
            {
                switch (p_Type)
                {
                    case eHighmaulDatas::ElevatorActivated:
                        m_ArenaElevatorActivated = p_Data;
                        break;
                    default:
                        break;
                }
            }

            uint32 GetData(uint32 p_Type) override
            {
                switch (p_Type)
                {
                    case eHighmaulDatas::ElevatorActivated:
                        return m_ArenaElevatorActivated;
                    default:
                        return 0;
                }
            }

            uint64 GetData64(uint32 p_Type) override
            {
                switch (p_Type)
                {
                    case eHighmaulCreatures::GhargArenaMaster:
                        return m_ArenaMasterGuid;
                    case eHighmaulCreatures::KargathBladefist:
                        return m_KargathBladefistGuid;
                    case eHighmaulCreatures::JhornTheMad:
                        return m_JhornTheMadGuid;
                    case eHighmaulCreatures::ThoktarIronskull:
                        return m_ThoktarIronskullGuid;
                    case eHighmaulCreatures::Vulgor:
                        return m_VulgorGuid;
                    case eHighmaulGameobjects::ArenaElevator:
                        return m_ArenaElevatorGuid;
                    case eHighmaulGameobjects::CollisionWall:
                        return m_CollisionWallGuid;
                    case eHighmaulGameobjects::GateArenaInner:
                        return m_GateArenaInnerGuid;
                    case eHighmaulCreatures::CrowdAreatrigger:
                        return m_CrowdAreatriggerGuid;
                    case eHighmaulCreatures::MargokCosmetic:
                        return m_MargokCosmeticGuid;
                    case eHighmaulCreatures::TheButcher:
                        return m_TheButcherGuid;
                    case eHighmaulCreatures::Brackenspore:
                        return m_BrackensporeGuid;
                    case eHighmaulGameobjects::RaidGrate1:
                        return m_RaidGrateGuids[eHighmaulDatas::RaidGrate001];
                    case eHighmaulGameobjects::RaidGrate2:
                        return m_RaidGrateGuids[eHighmaulDatas::RaidGrate002];
                    case eHighmaulGameobjects::RaidGrate3:
                        return m_RaidGrateGuids[eHighmaulDatas::RaidGrate003];
                    case eHighmaulGameobjects::RaidGrate4:
                        return m_RaidGrateGuids[eHighmaulDatas::RaidGrate004];
                    default:
                        break;
                }

                return 0;
            }

            bool CheckRequiredBosses(uint32 p_BossID, Player const* p_Player = nullptr) const override
            {
                if (!InstanceScript::CheckRequiredBosses(p_BossID, p_Player))
                    return false;

                /*switch (p_BossID)
                {
                    default:
                        break;
                }*/

                return true;
            }

            void FillInitialWorldStates(ByteBuffer& p_Buffer) override
            {
                p_Buffer << uint32(eHighmaulWorldStates::IronBomberEnable) << uint32(0);
                p_Buffer << uint32(eHighmaulWorldStates::IronBomberRemaining) << uint32(0);
                p_Buffer << uint32(eHighmaulWorldStates::DrunkenBileslingerEnable) << uint32(0);
                p_Buffer << uint32(eHighmaulWorldStates::DrunkenBileslingerRemaining) << uint32(0);
            }

            void OnPlayerEnter(Player* p_Player) override
            {
                if (GetBossState(eHighmaulDatas::BossKargathBladefist) == EncounterState::DONE)
                {
                    p_Player->SetPhaseMask(eHighmaulDatas::PhaseKargathDefeated, true);
                    p_Player->CastSpell(p_Player, eHighmaulSpells::ChogallNight, true);

                    if (GetBossState(eHighmaulDatas::BossTheButcher) == EncounterState::DONE)
                        p_Player->NearTeleportTo(eHighmaulLocs::BeachEntrance);
                    else
                        p_Player->NearTeleportTo(eHighmaulLocs::KargathDefeated);
                }
                else
                {
                    p_Player->SetPhaseMask(eHighmaulDatas::PhaseNone, true);
                    p_Player->RemoveAura(eHighmaulSpells::PlayChogallScene);
                    p_Player->RemoveAura(eHighmaulSpells::ChogallNight);
                }
            }

            void OnPlayerExit(Player* p_Player) override
            {
                p_Player->RemoveAura(eHighmaulSpells::PlayChogallScene);
                p_Player->RemoveAura(eHighmaulSpells::ChogallNight);
                p_Player->SetPhaseMask(eHighmaulDatas::PhaseNone, true);
            }

            void SendUpdateWorldState(uint32 p_Field, uint32 p_Value)
            {
                Map::PlayerList const& l_Players = instance->GetPlayers();
                for (Map::PlayerList::const_iterator l_Iter = l_Players.begin(); l_Iter != l_Players.end(); ++l_Iter)
                {
                    if (Player* l_Player = l_Iter->getSource())
                        l_Player->SendUpdateWorldState(p_Field, p_Value);
                }
            }

            void PlaySceneForPlayers(Position const p_Pos, uint32 p_ScenePackageID)
            {
                Map::PlayerList const& l_Players = instance->GetPlayers();
                for (Map::PlayerList::const_iterator l_Iter = l_Players.begin(); l_Iter != l_Players.end(); ++l_Iter)
                {
                    if (Player* l_Player = l_Iter->getSource())
                    {
                        l_Player->PlayStandaloneScene(p_ScenePackageID, 16, p_Pos);
                        l_Player->SetPhaseMask(2, true);
                    }
                }
            }

            void CastSpellToPlayers(Map* p_Map, Unit* p_Caster, uint32 p_SpellID, bool p_Triggered)
            {
                if (p_Map == nullptr)
                    return;

                Map::PlayerList const& l_Players = p_Map->GetPlayers();
                for (Map::PlayerList::const_iterator l_Iter = l_Players.begin(); l_Iter != l_Players.end(); ++l_Iter)
                {
                    if (Player* l_Player = l_Iter->getSource())
                    {
                        if (p_Caster != nullptr)
                            p_Caster->CastSpell(l_Player, p_SpellID, p_Triggered);
                        else
                            l_Player->CastSpell(l_Player, p_SpellID, p_Triggered);
                    }
                }
            }
        };

        InstanceScript* GetInstanceScript(InstanceMap* p_Map) const override
        {
            return new instance_highmaul_InstanceMapScript(p_Map);
        }
};

void AddSC_instance_highmaul()
{
    new instance_highmaul();
}