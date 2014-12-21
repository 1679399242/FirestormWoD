/*
* Copyright (C) 2012-2014 JadeCore <http://www.pandashan.com>
* Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
* Copyright (C) 2006-2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
* more details.
*
* You should have received a copy of the GNU General Public License along
* with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "ScriptPCH.h"
#include "upper_blackrock_spire.h"
#include "ObjectAccessor.h"

static const DoorData doordata[] = 
{
    { GOB_OREBENDER_ENTRANCE,   DATA_OREBENDER_GORASHAN,    DOOR_TYPE_ROOM,     BOUNDARY_NONE },
    { GOB_OREBENDER_EXIT,       DATA_OREBENDER_GORASHAN,    DOOR_TYPE_PASSAGE,  BOUNDARY_NONE },
    { GOB_OREBENDER_EXIT,       DATA_KYRAK_THE_CORRUPTOR,   DOOR_TYPE_ROOM,     BOUNDARY_NONE },
    { GOB_KYRAK_EXIT_01,        DATA_KYRAK_THE_CORRUPTOR,   DOOR_TYPE_PASSAGE,  BOUNDARY_NONE },
    { GOB_KYRAK_EXIT_02,        DATA_KYRAK_THE_CORRUPTOR,   DOOR_TYPE_PASSAGE,  BOUNDARY_NONE },
    { GOB_THARBEK_EXIT,         DATA_COMMANDER_THARBEK,     DOOR_TYPE_PASSAGE,  BOUNDARY_NONE },
    { GOB_THARBEK_EXIT_SECOND,  DATA_COMMANDER_THARBEK,     DOOR_TYPE_PASSAGE,  BOUNDARY_NONE },
    { GOB_RAGEWING_ENTRANCE,    DATA_RAGEWING_THE_UNTAMED,  DOOR_TYPE_ROOM,     BOUNDARY_NONE },
    { GOB_RAGEWING_EXIT,        DATA_RAGEWING_THE_UNTAMED,  DOOR_TYPE_PASSAGE,  BOUNDARY_NONE },
    { 0,                        0,                          DOOR_TYPE_ROOM,     0             }  // EOF
};

static const BossScenarios g_BossScenarios[] =
{
    { DATA_OREBENDER_GORASHAN,   SCENARIO_UBRS_GORASHAN },
    { DATA_KYRAK_THE_CORRUPTOR,  SCENARIO_UBRS_KYRAK    },
    { DATA_COMMANDER_THARBEK,    SCENARIO_UBRS_THARBEK  },
    { DATA_RAGEWING_THE_UNTAMED, SCENARIO_UBRS_RAGEWING },
    { DATA_WARLORD_ZAELA,        SCENARIO_UBRS_ZAELA    },
    { 0,                         0                      }
};

class instance_upper_blackrock_spire : public MS::Game::Scripting::Interfaces::InstanceMapScript
{
    public:
        instance_upper_blackrock_spire() : MS::Game::Scripting::Interfaces::InstanceMapScript("instance_upper_blackrock_spire", 1358) { }

        struct instance_upper_blackrock_spireInstanceMapScript : public InstanceScript
        {
            instance_upper_blackrock_spireInstanceMapScript(Map* p_Map) : InstanceScript(p_Map)
            {
                SetBossNumber(DATA_MAX_ENCOUNTERS);
                LoadDoorData(doordata);
                LoadScenariosInfos(g_BossScenarios, p_Map->IsChallengeMode() ? SCENARIO_UBRS_CHALLENGE : SCENARIO_UBRS_ID);

                m_PreOrebenderDoorGuid      = 0;
                m_OrebenderEntranceGuid     = 0;
                m_OrebenderGorashanGuid     = 0;
                m_ThunderingCacophonyCasted = 0;
                m_RunesDisabled             = 0;
                m_CommanderTharbekGuid      = 0;
                m_AwbeeGuid                 = 0;
                m_RagewingGuid              = 0;
                m_LeftTrackerGuid           = 0;
                m_RightTrackerGuid          = 0;
                m_WarlordZaelaGuid          = 0;
                m_LeeroyJenkinsGuid         = 0;
                m_SonOfBeastGuid            = 0;
                m_CreatureKilled            = 0;
                m_EmberscaleKilled          = 0;
                m_RagewingWhelpsKilled      = 0;
                m_RagewingTimeAchiev        = 0;
            }

            uint64 m_PreOrebenderDoorGuid;
            uint64 m_OrebenderEntranceGuid;
            uint64 m_OrebenderGorashanGuid;
            uint8  m_ThunderingCacophonyCasted;
            uint8  m_RunesDisabled;

            uint64 m_CommanderTharbekGuid;
            uint64 m_AwbeeGuid;
            uint64 m_SpawnDoorGuid;

            uint64 m_RagewingGuid;
            uint64 m_LeftTrackerGuid;
            uint64 m_RightTrackerGuid;

            uint64 m_WarlordZaelaGuid;

            uint64 m_LeeroyJenkinsGuid;
            uint64 m_SonOfBeastGuid;

            uint32 m_CreatureKilled;

            uint32 m_EmberscaleKilled;

            uint32 m_RagewingWhelpsKilled;
            uint32 m_RagewingTimeAchiev;

            void OnCreatureCreate(Creature* p_Creature)
            {
                switch (p_Creature->GetEntry())
                {
                    case NPC_OREBENDER_GORASHAN:
                        m_OrebenderGorashanGuid = p_Creature->GetGUID();
                        break;
                    case NPC_COMMANDER_THARBEK:
                        m_CommanderTharbekGuid = p_Creature->GetGUID();
                        break;
                    case NPC_AWBEE:
                        m_AwbeeGuid = p_Creature->GetGUID();
                        break;
                    case NPC_RAGEWING_THE_UNTAMED:
                        m_RagewingGuid = p_Creature->GetGUID();
                        break;
                    case NPC_ENGULFING_FIRE_R_TO_L:
                        m_RightTrackerGuid = p_Creature->GetGUID();
                        break;
                    case NPC_ENGULFING_FIRE_L_TO_R:
                        m_LeftTrackerGuid = p_Creature->GetGUID();
                        break;
                    case NPC_WARLORD_ZAELA:
                        m_WarlordZaelaGuid = p_Creature->GetGUID();
                        break;
                    case NPC_LEEROY_JENKINS:
                        m_LeeroyJenkinsGuid = p_Creature->GetGUID();
                        break;
                    case NPC_SON_OF_THE_BEAST:
                        m_SonOfBeastGuid = p_Creature->GetGUID();
                        break;
                    default:
                        break;
                }
            }

            void OnGameObjectCreate(GameObject* p_Gameobject)
            {
                switch (p_Gameobject->GetEntry())
                {
                    case GOB_EMBERSEER_IN:
                        m_PreOrebenderDoorGuid = p_Gameobject->GetGUID();
                        break;
                    case GOB_OREBENDER_ENTRANCE:
                        m_OrebenderEntranceGuid = p_Gameobject->GetGUID();
                        AddDoor(p_Gameobject, true);
                        break;
                    case GOB_OREBENDER_EXIT:
                    case GOB_KYRAK_EXIT_01:
                    case GOB_KYRAK_EXIT_02:
                    case GOB_THARBEK_EXIT:
                    case GOB_THARBEK_EXIT_SECOND:
                    case GOB_RAGEWING_ENTRANCE:
                    case GOB_RAGEWING_EXIT:
                        AddDoor(p_Gameobject, true);
                        break;
                    case GOB_THARBEK_SPAWN_DOOR:
                        m_SpawnDoorGuid = p_Gameobject->GetGUID();
                        break;
                    case GOB_CHALLENGE_START_DOOR:
                        m_ChallengeDoorGuid = p_Gameobject->GetGUID();
                        break;
                    default:
                        break;
                }
            }
            
            bool SetBossState(uint32 p_ID, EncounterState p_State)
            {
                if (!InstanceScript::SetBossState(p_ID, p_State))
                    return false;

                switch (p_ID)
                {
                    case DATA_OREBENDER_GORASHAN:
                    {
                        if (p_State != DONE)
                            m_ThunderingCacophonyCasted = 0;
                        break;
                    }
                    case DATA_KYRAK_THE_CORRUPTOR:
                    {
                        if (p_State != DONE || !instance->IsHeroic())
                            break;

                        if (Creature* l_Leeroy = sObjectAccessor->FindCreature(m_LeeroyJenkinsGuid))
                            l_Leeroy->AI()->DoAction(0);   ///< ActionActivateLeeroyRes

                        break;
                    }
                    case DATA_RAGEWING_THE_UNTAMED:
                    {
                        if (p_State == NOT_STARTED)
                        {
                            m_RagewingTimeAchiev = 0;
                            m_RagewingWhelpsKilled = 0;
                        }
                        break;
                    }
                    case DATA_WARLORD_ZAELA:
                        if (p_State != DONE)
                            m_EmberscaleKilled = 0;
                        break;
                    case DATA_COMMANDER_THARBEK:
                    default:
                        break;
                }

                return true;
            }

            void SetData(uint32 p_Type, uint32 p_Data)
            {
                switch (p_Type)
                {
                    case DATA_MAGNETS_ACHIEVEMENT:
                        ++m_ThunderingCacophonyCasted;
                        break;
                    case DATA_RUNES_DISABLED:
                    {
                        if (m_RunesDisabled >= 5)
                            break;

                        ++m_RunesDisabled;

                        if (!instance->IsChallengeMode())
                            SendScenarioProgressUpdate(CriteriaProgressData(SCENARIO_UBRS_RUNES, m_RunesDisabled, m_InstanceGuid, time(NULL), m_BeginningTime, 0));

                        Unit* l_Orebender = sObjectAccessor->FindUnit(m_OrebenderGorashanGuid);
                        if (m_RunesDisabled >= 5 && l_Orebender != nullptr)
                        {
                            if (GameObject* l_PreDoor = GameObject::GetGameObject(*l_Orebender, m_PreOrebenderDoorGuid))
                                l_PreDoor->SetGoState(GO_STATE_ACTIVE);
                            if (GameObject* l_Entrance = GameObject::GetGameObject(*l_Orebender, m_OrebenderEntranceGuid))
                                l_Entrance->SetGoState(GO_STATE_ACTIVE);

                            if (!instance->IsChallengeMode())
                                SendScenarioState(ScenarioData(m_ScenarioID, ++m_ScenarioStep));
                            break;
                        }

                        break;
                    }
                    default:
                        break;
                }
            }

            uint32 GetData(uint32 p_Type)
            {
                switch (p_Type)
                {
                    case DATA_MAGNETS_ACHIEVEMENT:
                        return m_ThunderingCacophonyCasted;
                    default:
                        break;
                }

                return 0;
            }

            uint64 GetData64(uint32 p_Type)
            {
                switch (p_Type)
                {
                    case NPC_OREBENDER_GORASHAN:
                        return m_OrebenderGorashanGuid;
                    case NPC_COMMANDER_THARBEK:
                        return m_CommanderTharbekGuid;
                    case NPC_AWBEE:
                        return m_AwbeeGuid;
                    case NPC_RAGEWING_THE_UNTAMED:
                        return m_RagewingGuid;
                    case NPC_ENGULFING_FIRE_R_TO_L:
                        return m_RightTrackerGuid;
                    case NPC_ENGULFING_FIRE_L_TO_R:
                        return m_LeftTrackerGuid;
                    case NPC_WARLORD_ZAELA:
                        return m_WarlordZaelaGuid;
                    case GOB_THARBEK_SPAWN_DOOR:
                        return m_SpawnDoorGuid;
                    case NPC_SON_OF_THE_BEAST:
                        return m_SonOfBeastGuid;
                    default:
                        return 0;
                }
            }

            void OnCreatureKilled(Creature* p_Creature, Player* p_Player)
            {
                if (instance->IsHeroic())
                {
                    ///< Kill 20 Ragewing Whelps in 10 seconds while fighting Ragewing the Untamed in Upper Blackrock Spire on Heroic difficulty.
                    if (p_Creature->GetEntry() == NPC_RAGEWING_WHELP && GetBossState(DATA_RAGEWING_THE_UNTAMED) == IN_PROGRESS)
                    {
                        if (!m_RagewingTimeAchiev)
                        {
                            m_RagewingWhelpsKilled = 0;
                            m_RagewingTimeAchiev = 1;
                        }

                        ++m_RagewingWhelpsKilled;

                        if (m_RagewingWhelpsKilled >= 20 && m_RagewingTimeAchiev > 0 && m_RagewingTimeAchiev <= 10 * IN_MILLISECONDS)
                            DoCompleteAchievement(eAchievements::AchievementBridgeOverFire);

                        return;
                    }
                    ///< Kill 5 Emberscale Ironflight before defeating Warlord Zaela in Upper Blackrock Spire on Heroic difficulty.
                    else if (p_Creature->GetEntry() == NPC_EMBERSCALE_IRONFLIGHT_2 && GetBossState(DATA_WARLORD_ZAELA) == IN_PROGRESS)
                    {
                        ++m_EmberscaleKilled;

                        if (m_EmberscaleKilled >= 5)
                            DoCompleteAchievement(eAchievements::AchievementDragonmawDragonfall);

                        return;
                    }
                }

                if (!instance->IsChallengeMode() || !IsChallengeModeStarted() || m_CreatureKilled >= SCENARIO_UBRS_KILLS)
                    return;

                if (p_Creature == nullptr)
                    return;

                if (!p_Creature->isElite() || p_Creature->IsDungeonBoss())
                    return;

                ++m_CreatureKilled;
                SendScenarioProgressUpdate(CriteriaProgressData(SCENARIO_UBRS_ENNEMIES, m_CreatureKilled, m_InstanceGuid, time(NULL), m_BeginningTime, 0));

                if (m_CreatureKilled >= SCENARIO_UBRS_KILLS)
                    m_ConditionCompleted = true;
            }

            void Update(uint32 p_Diff)
            {
                ScheduleBeginningTimeUpdate(p_Diff);
                ScheduleChallengeStartup(p_Diff);
                ScheduleChallengeTimeUpdate(p_Diff);

                if (m_RagewingTimeAchiev)
                    m_RagewingTimeAchiev += p_Diff;
            }

            void FillInitialWorldStates(ByteBuffer& p_Buffer)
            {
                p_Buffer << uint32(eWorldStates::WorldStateEnableChicken) << uint32(0);
                p_Buffer << uint32(eWorldStates::WorldStateChickenTimer) << uint32(0);
            }

            ///< Must be overrided because of optional (runes) step...
            void OnPlayerEnter(Player* p_Player)
            {
                SendScenarioState(ScenarioData(m_ScenarioID, m_ScenarioStep), p_Player);

                Unit* l_Orebender = sObjectAccessor->FindUnit(m_OrebenderGorashanGuid);
                if (m_ScenarioStep == 0 && m_RunesDisabled >= 5 && l_Orebender != nullptr)
                {
                    if (GameObject* l_PreDoor = GameObject::GetGameObject(*l_Orebender, m_PreOrebenderDoorGuid))
                        l_PreDoor->SetGoState(GO_STATE_ACTIVE);
                    if (GameObject* l_Entrance = GameObject::GetGameObject(*l_Orebender, m_OrebenderEntranceGuid))
                        l_Entrance->SetGoState(GO_STATE_ACTIVE);

                    if (!instance->IsChallengeMode())
                        SendScenarioState(ScenarioData(m_ScenarioID, ++m_ScenarioStep));
                }

                UpdateCriteriasAfterLoading();
            }
        };

        InstanceScript* GetInstanceScript(InstanceMap* p_Map) const
        {
            return new instance_upper_blackrock_spireInstanceMapScript(p_Map);
        }
};

void AddSC_instance_upper_blackrock_spire()
{
    new instance_upper_blackrock_spire();
}

