/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
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

#include "InstanceScript.h"
#include "DatabaseEnv.h"
#include "Map.h"
#include "Player.h"
#include "Group.h"
#include "GameObject.h"
#include "Creature.h"
#include "CreatureAI.h"
#include "Log.h"
#include "LFGMgr.h"

InstanceScript::InstanceScript(Map* p_Map)
{
    instance = p_Map;
    completedEncounters = 0;
    m_ChallengeStarted = false;
    m_ConditionCompleted = false;
    m_StartChallengeTime = 0;
    m_ChallengeDoorGuid = 0;
    m_ChallengeTime = 0;
    m_MedalType = eChallengeMedals::MedalTypeNone;

    m_InstanceGuid = MAKE_NEW_GUID(p_Map->GetId(), 0, HIGHGUID_INSTANCE_SAVE);
    m_BeginningTime = 0;
    m_ScenarioID = 0;
    m_ScenarioStep = 0;
}

void InstanceScript::SaveToDB()
{
    std::string data = GetSaveData();
    if (data.empty())
        return;

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_INSTANCE_DATA);
    stmt->setUInt32(0, GetCompletedEncounterMask());
    stmt->setString(1, data);
    stmt->setUInt32(2, instance->GetInstanceId());
    CharacterDatabase.Execute(stmt);
}

void InstanceScript::HandleGameObject(uint64 GUID, bool open, GameObject* go)
{
    if (!go)
        go = instance->GetGameObject(GUID);
    if (go)
        go->SetGoState(open ? GO_STATE_ACTIVE : GO_STATE_READY);
    else
        sLog->outDebug(LOG_FILTER_TSCR, "InstanceScript: HandleGameObject failed");
}

bool InstanceScript::IsEncounterInProgress() const
{
    for (std::vector<BossInfo>::const_iterator itr = bosses.begin(); itr != bosses.end(); ++itr)
        if (itr->state == IN_PROGRESS)
            return true;

    return false;
}

void InstanceScript::OnPlayerEnter(Player* p_Player)
{
    SendScenarioState(ScenarioData(m_ScenarioID, m_ScenarioStep), p_Player);
    UpdateCriteriasAfterLoading();
}

void InstanceScript::LoadMinionData(const MinionData* data)
{
    while (data->entry)
    {
        if (data->bossId < bosses.size())
            minions.insert(std::make_pair(data->entry, MinionInfo(&bosses[data->bossId])));

        ++data;
    }
    sLog->outDebug(LOG_FILTER_TSCR, "InstanceScript::LoadMinionData: " UI64FMTD " minions loaded.", uint64(minions.size()));
}

void InstanceScript::SetBossNumber(uint32 p_Number)
{
    bosses.resize(p_Number);
    m_BossesScenarios.resize(p_Number);
}

void InstanceScript::LoadDoorData(const DoorData* data)
{
    while (data->entry)
    {
        if (data->bossId < bosses.size())
            doors.insert(std::make_pair(data->entry, DoorInfo(&bosses[data->bossId], data->type, BoundaryType(data->boundary))));

        ++data;
    }
    sLog->outDebug(LOG_FILTER_TSCR, "InstanceScript::LoadDoorData: " UI64FMTD " doors loaded.", uint64(doors.size()));
}

void InstanceScript::LoadScenariosInfos(BossScenarios const* p_Scenarios, uint32 p_ScenarioID)
{
    while (p_Scenarios->m_ScenarioID)
    {
        if (p_Scenarios->m_BossID < bosses.size())
            m_BossesScenarios[p_Scenarios->m_BossID] = BossScenarios(p_Scenarios->m_BossID, p_Scenarios->m_ScenarioID);

        ++p_Scenarios;
    }

    m_ScenarioID = p_ScenarioID;
}

void InstanceScript::UpdateMinionState(Creature* minion, EncounterState state)
{
    switch (state)
    {
        case NOT_STARTED:
            if (!minion->isAlive())
                minion->Respawn();
            else if (minion->isInCombat())
                minion->AI()->EnterEvadeMode();
            break;
        case IN_PROGRESS:
            if (!minion->isAlive())
                minion->Respawn();
            else if (!minion->getVictim())
                minion->AI()->DoZoneInCombat();
            break;
        default:
            break;
    }
}

void InstanceScript::UpdateDoorState(GameObject* door)
{
    if (!door)
        return;

    DoorInfoMap::iterator lower = doors.lower_bound(door->GetEntry());
    DoorInfoMap::iterator upper = doors.upper_bound(door->GetEntry());
    if (lower == upper)
        return;

    bool open = true;
    for (DoorInfoMap::iterator itr = lower; itr != upper && open; ++itr)
    {
        switch (itr->second.type)
        {
            case DOOR_TYPE_ROOM:
                open = (itr->second.bossInfo->state != IN_PROGRESS);
                break;
            case DOOR_TYPE_PASSAGE:
                open = (itr->second.bossInfo->state == DONE);
                break;
            case DOOR_TYPE_SPAWN_HOLE:
                open = (itr->second.bossInfo->state == IN_PROGRESS);
                break;
            default:
                break;
        }
    }

    door->SetGoState(open ? GO_STATE_ACTIVE : GO_STATE_READY);
}

void InstanceScript::AddDoor(GameObject* door, bool add)
{
    if (!door)
        return;

    DoorInfoMap::iterator lower = doors.lower_bound(door->GetEntry());
    DoorInfoMap::iterator upper = doors.upper_bound(door->GetEntry());

    if (lower == upper)
        return;

    for (DoorInfoMap::iterator itr = lower; itr != upper; ++itr)
    {
        DoorInfo const& data = itr->second;

        if (add)
        {
            data.bossInfo->door[data.type].insert(door);
            switch (data.boundary)
            {
                default:
                case BOUNDARY_NONE:
                    break;
                case BOUNDARY_N:
                case BOUNDARY_S:
                    data.bossInfo->boundary[data.boundary] = door->GetPositionX();
                    break;
                case BOUNDARY_E:
                case BOUNDARY_W:
                    data.bossInfo->boundary[data.boundary] = door->GetPositionY();
                    break;
                case BOUNDARY_NW:
                case BOUNDARY_SE:
                    data.bossInfo->boundary[data.boundary] = door->GetPositionX() + door->GetPositionY();
                    break;
                case BOUNDARY_NE:
                case BOUNDARY_SW:
                    data.bossInfo->boundary[data.boundary] = door->GetPositionX() - door->GetPositionY();
                    break;
            }
        }
        else
            data.bossInfo->door[data.type].erase(door);
    }

    if (add)
        UpdateDoorState(door);
}

void InstanceScript::AddMinion(Creature* minion, bool add)
{
    MinionInfoMap::iterator itr = minions.find(minion->GetEntry());
    if (itr == minions.end())
        return;

    if (add)
        itr->second.bossInfo->minion.insert(minion);
    else
        itr->second.bossInfo->minion.erase(minion);
}

bool InstanceScript::SetBossState(uint32 id, EncounterState state)
{
    if (id < bosses.size())
    {
        BossInfo* bossInfo = &bosses[id];
        BossScenarios* l_BossScenario = &m_BossesScenarios[id];

        if (bossInfo->state == TO_BE_DECIDED) // loading
        {
            bossInfo->state = state;

            if (state == DONE)
                SendScenarioProgressUpdate(CriteriaProgressData(l_BossScenario->m_ScenarioID, 1, m_InstanceGuid, time(NULL), m_BeginningTime, 0));

            return false;
        }
        else
        {
            if (bossInfo->state == state)
                return false;

            if (state == DONE)
            {
                for (MinionSet::iterator l_Iter = bossInfo->minion.begin(); l_Iter != bossInfo->minion.end(); ++l_Iter)
                {
                    if ((*l_Iter)->isWorldBoss() && (*l_Iter)->isAlive())
                        return false;
                }

                SendScenarioProgressUpdate(CriteriaProgressData(l_BossScenario->m_ScenarioID, 1, m_InstanceGuid, time(NULL), m_BeginningTime, 0));
            }

            bossInfo->state = state;
            SaveToDB();
        }

        for (uint32 type = 0; type < MAX_DOOR_TYPES; ++type)
            for (DoorSet::iterator i = bossInfo->door[type].begin(); i != bossInfo->door[type].end(); ++i)
                UpdateDoorState(*i);

        for (MinionSet::iterator i = bossInfo->minion.begin(); i != bossInfo->minion.end(); ++i)
            UpdateMinionState(*i, state);

        ///< End of challenge
        if (id == (bosses.size() - 1) && state == DONE)
        {
            if (instance->IsChallengeMode() && m_ChallengeStarted && m_ConditionCompleted)
            {
                m_ChallengeStarted = false;

                SendChallengeNewPlayerRecord();
                SendChallengeModeComplete(RewardChallengers());
                SendChallengeStopElapsedTimer(1);

                SaveChallengeDatasIfNeeded();

                DoUpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_WIN_CHALLENGE_DUNGEON, instance->GetId(), m_MedalType);
            }

            SendScenarioState(ScenarioData(m_ScenarioID, ++m_ScenarioStep));
        }

        return true;
    }
    return false;
}

std::string InstanceScript::LoadBossState(const char * data)
{
    if (!data)
        return NULL;
    std::istringstream loadStream(data);
    uint32 buff;
    uint32 bossId = 0;
    for (std::vector<BossInfo>::iterator i = bosses.begin(); i != bosses.end(); ++i, ++bossId)
    {
        loadStream >> buff;
        if (buff < TO_BE_DECIDED)
            SetBossState(bossId, (EncounterState)buff);
    }
    return loadStream.str();
}

std::string InstanceScript::GetBossSaveData()
{
    std::ostringstream saveStream;
    for (std::vector<BossInfo>::iterator i = bosses.begin(); i != bosses.end(); ++i)
        saveStream << (uint32)i->state << ' ';
    return saveStream.str();
}

void InstanceScript::DoUseDoorOrButton(uint64 uiGuid, uint32 uiWithRestoreTime, bool bUseAlternativeState)
{
    if (!uiGuid)
        return;

    GameObject* go = instance->GetGameObject(uiGuid);

    if (go)
    {
        if (go->GetGoType() == GAMEOBJECT_TYPE_DOOR || go->GetGoType() == GAMEOBJECT_TYPE_BUTTON)
        {
            if (go->getLootState() == GO_READY)
                go->UseDoorOrButton(uiWithRestoreTime, bUseAlternativeState);
            else if (go->getLootState() == GO_ACTIVATED)
                go->ResetDoorOrButton();
        }
        else
            sLog->outError(LOG_FILTER_GENERAL, "SD2: Script call DoUseDoorOrButton, but gameobject entry %u is type %u.", go->GetEntry(), go->GetGoType());
    }
}

void InstanceScript::DoRespawnGameObject(uint64 uiGuid, uint32 uiTimeToDespawn)
{
    if (GameObject* go = instance->GetGameObject(uiGuid))
    {
        //not expect any of these should ever be handled
        if (go->GetGoType() == GAMEOBJECT_TYPE_FISHINGNODE || go->GetGoType() == GAMEOBJECT_TYPE_DOOR ||
            go->GetGoType() == GAMEOBJECT_TYPE_BUTTON || go->GetGoType() == GAMEOBJECT_TYPE_TRAP)
            return;

        if (go->isSpawned())
            return;

        go->SetRespawnTime(uiTimeToDespawn);
        go->UpdateObjectVisibility();
    }
}

void InstanceScript::DoUpdateWorldState(uint32 uiStateId, uint32 uiStateData)
{
    Map::PlayerList const& lPlayers = instance->GetPlayers();

    if (!lPlayers.isEmpty())
    {
        for (Map::PlayerList::const_iterator itr = lPlayers.begin(); itr != lPlayers.end(); ++itr)
            if (Player* player = itr->getSource())
                player->SendUpdateWorldState(uiStateId, uiStateData);
    }
    else
        sLog->outDebug(LOG_FILTER_TSCR, "DoUpdateWorldState attempt send data but no players in map.");
}

// Send Notify to all players in instance
void InstanceScript::DoSendNotifyToInstance(char const* format, ...)
{
    InstanceMap::PlayerList const& players = instance->GetPlayers();

    if (!players.isEmpty())
    {
        va_list ap;
        va_start(ap, format);
        char buff[1024];
        vsnprintf(buff, 1024, format, ap);
        va_end(ap);
        for (Map::PlayerList::const_iterator i = players.begin(); i != players.end(); ++i)
            if (Player* player = i->getSource())
                if (WorldSession* session = player->GetSession())
                    session->SendNotification("%s", buff);
    }
}

// Reset Achievement Criteria for all players in instance
void InstanceScript::DoResetAchievementCriteria(AchievementCriteriaTypes type, uint64 miscValue1 /*= 0*/, uint64 miscValue2 /*= 0*/, bool evenIfCriteriaComplete /*= false*/)
{
    Map::PlayerList const &plrList = instance->GetPlayers();

    if (!plrList.isEmpty())
        for (Map::PlayerList::const_iterator i = plrList.begin(); i != plrList.end(); ++i)
            if (Player* pPlayer = i->getSource())
                pPlayer->GetAchievementMgr().ResetAchievementCriteria(type, miscValue1, miscValue2, evenIfCriteriaComplete);
}

// Complete Achievement for all players in instance
void InstanceScript::DoCompleteAchievement(uint32 achievement)
{
  AchievementEntry const* pAE = sAchievementStore.LookupEntry(achievement);
  Map::PlayerList const &plrList = instance->GetPlayers();
  if (!pAE)
      return;

  if (!plrList.isEmpty())
      for (Map::PlayerList::const_iterator i = plrList.begin(); i != plrList.end(); ++i)
          if (Player *pPlayer = i->getSource())
              pPlayer->CompletedAchievement(pAE);
}

// Update Achievement Criteria for all players in instance
void InstanceScript::DoUpdateAchievementCriteria(AchievementCriteriaTypes type, uint32 miscValue1 /*= 0*/, uint32 miscValue2 /*= 0*/, uint32 miscValue3 /*=0*/, Unit* unit /*= NULL*/)
{
    Map::PlayerList const &plrList = instance->GetPlayers();

    if (!plrList.isEmpty())
        for (Map::PlayerList::const_iterator i = plrList.begin(); i != plrList.end(); ++i)
            if (Player* pPlayer = i->getSource())
                pPlayer->UpdateAchievementCriteria(type, miscValue1, miscValue2, miscValue3, unit);
}

// Start timed achievement for all players in instance
void InstanceScript::DoStartTimedAchievement(AchievementCriteriaTimedTypes type, uint32 entry)
{
    Map::PlayerList const &PlayerList = instance->GetPlayers();

    if (!PlayerList.isEmpty())
        for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
            if (Player* player = i->getSource())
                player->GetAchievementMgr().StartTimedAchievement(type, entry);
}

// Stop timed achievement for all players in instance
void InstanceScript::DoStopTimedAchievement(AchievementCriteriaTimedTypes type, uint32 entry)
{
    Map::PlayerList const &PlayerList = instance->GetPlayers();

    if (!PlayerList.isEmpty())
        for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
            if (Player* player = i->getSource())
                player->GetAchievementMgr().RemoveTimedAchievement(type, entry);
}

// Remove Auras due to Spell on all players in instance
void InstanceScript::DoRemoveAurasDueToSpellOnPlayers(uint32 spell)
{
    Map::PlayerList const& PlayerList = instance->GetPlayers();
    if (!PlayerList.isEmpty())
    {
        for (Map::PlayerList::const_iterator itr = PlayerList.begin(); itr != PlayerList.end(); ++itr)
        {
            if (Player* player = itr->getSource())
            {
                player->RemoveAurasDueToSpell(spell);
                if (Pet* pet = player->GetPet())
                    pet->RemoveAurasDueToSpell(spell);
            }
        }
    }
}

// Cast spell on all players in instance
void InstanceScript::DoCastSpellOnPlayers(uint32 spell)
{
    Map::PlayerList const &PlayerList = instance->GetPlayers();

    if (!PlayerList.isEmpty())
        for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
            if (Player* player = i->getSource())
                player->CastSpell(player, spell, true);
}

void InstanceScript::DoSetAlternatePowerOnPlayers(int32 value)
{
    Map::PlayerList const &plrList = instance->GetPlayers();

    if (!plrList.isEmpty())
        for (Map::PlayerList::const_iterator i = plrList.begin(); i != plrList.end(); ++i)
            if (Player* pPlayer = i->getSource())
                pPlayer->SetPower(POWER_ALTERNATE_POWER, value);
}

void InstanceScript::DoModifyPlayerCurrencies(uint32 id, int32 value)
{
    Map::PlayerList const &plrList = instance->GetPlayers();

    if (!plrList.isEmpty())
        for (Map::PlayerList::const_iterator i = plrList.begin(); i != plrList.end(); ++i)
            if (Player* pPlayer = i->getSource())
                pPlayer->ModifyCurrency(id, value);
}

void InstanceScript::DoNearTeleportPlayers(const Position pos, bool casting /*=false*/)
{
    Map::PlayerList const &plrList = instance->GetPlayers();

    if (!plrList.isEmpty())
        for (Map::PlayerList::const_iterator i = plrList.begin(); i != plrList.end(); ++i)
            if (Player* pPlayer = i->getSource())
                pPlayer->NearTeleportTo(pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ(), pos.GetOrientation(), casting);
}

void InstanceScript::DoKilledMonsterKredit(uint32 questId, uint32 entry, uint64 guid/* =0*/)
{
    Map::PlayerList const &plrList = instance->GetPlayers();

    if (!plrList.isEmpty())
        for (Map::PlayerList::const_iterator i = plrList.begin(); i != plrList.end(); ++i)
            if (Player* pPlayer = i->getSource())
                if (pPlayer->GetQuestStatus(questId) == QUEST_STATUS_INCOMPLETE)
                    pPlayer->KilledMonsterCredit(entry, guid);
}

// Add aura on all players in instance
void InstanceScript::DoAddAuraOnPlayers(uint32 spell)
{
    Map::PlayerList const &PlayerList = instance->GetPlayers();

    if (!PlayerList.isEmpty())
        for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
            if (Player* player = i->getSource())
                player->AddAura(spell, player);
}

bool InstanceScript::CheckAchievementCriteriaMeet(uint32 criteria_id, Player const* /*source*/, Unit const* /*target*/ /*= NULL*/, uint32 /*miscvalue1*/ /*= 0*/)
{
    sLog->outError(LOG_FILTER_GENERAL, "Achievement system call InstanceScript::CheckAchievementCriteriaMeet but instance script for map %u not have implementation for achievement criteria %u",
        instance->GetId(), criteria_id);
    return false;
}

bool InstanceScript::CheckRequiredBosses(uint32 /*bossId*/, Player const* player) const
{
    if (player && player->isGameMaster())
        return true;

    if (instance->GetPlayersCountExceptGMs() > instance->ToInstanceMap()->GetMaxPlayers())
        return false;

    return true;
}

void InstanceScript::SendEncounterUnit(uint32 p_Type, Unit* p_Unit /*= NULL*/, uint8 p_Param1 /*= 0*/, uint8 p_Param2 /*= 0*/)
{
    WorldPacket l_Data;

    switch (p_Type)
    {
        case ENCOUNTER_FRAME_ENGAGE:
            if (!p_Unit)
                return;
            p_Unit->BuildEncounterFrameData(&l_Data, true, p_Param1);
            break;
        case ENCOUNTER_FRAME_DISENGAGE:
            if (!p_Unit)
                return;
            p_Unit->BuildEncounterFrameData(&l_Data, false);
            break;
        case ENCOUNTER_FRAME_UPDATE_PRIORITY:
            if (!p_Unit)
                return;
            l_Data.Initialize(SMSG_INSTANCE_ENCOUNTER_CHANGE_PRIORITY, 8 + 1);
            l_Data.append(p_Unit->GetPackGUID());
            l_Data << uint8(p_Param1);  // TargetFramePriority
            break;
        case ENCOUNTER_FRAME_START_TIMER:
            l_Data.Initialize(SMSG_INSTANCE_ENCOUNTER_TIMER_START, 4);
            l_Data << int32(0);         // TimeRemaining
            break;
        case ENCOUNTER_FRAME_START_OBJECTIVE:
            l_Data.Initialize(SMSG_INSTANCE_ENCOUNTER_OBJECTIVE_START, 4);
            l_Data << int32(0);         // ObjectiveID
            break;
        case ENCOUNTER_FRAME_COMPLETE_OBJECTIVE:
            l_Data.Initialize(SMSG_INSTANCE_ENCOUNTER_OBJECTIVE_COMPLETE, 4);
            l_Data << int32(0);         // ObjectiveID
            break;
        case ENCOUNTER_FRAME_START:
            l_Data.Initialize(SMSG_INSTANCE_ENCOUNTER_START, 4 * 4);
            l_Data << uint32(0);        // CombatResChargeRecovery
            l_Data << int32(0);         // MaxInCombatResCount
            l_Data << int32(0);         // InCombatResCount
            l_Data << uint32(0);        // NextCombatResChargeTime
            break;
        case ENCOUNTER_FRAME_UPDATE_OBJECTIVE:
            l_Data.Initialize(SMSG_INSTANCE_ENCOUNTER_OBJECTIVE_UPDATE, 4 * 2);
            l_Data << int32(0);         // ProgressAmount
            l_Data << int32(0);         // ObjectiveID
            break;
        case ENCOUNTER_FRAME_END:
            l_Data.Initialize(SMSG_INSTANCE_ENCOUNTER_END, 0);
            break;
        case ENCOUNTER_FRAME_IN_COMBAT_RESURRECTION:
            l_Data.Initialize(SMSG_INSTANCE_ENCOUNTER_IN_COMBAT_RESURRECTION, 0);
            break;
        case ENCOUNTER_FRAME_PHASE_SHIFT_CHANGED:
            l_Data.Initialize(SMSG_INSTANCE_ENCOUNTER_PHASE_SHIFT_CHANGED, 0);
            break;
        case ENCOUNTER_FRAME_GAIN_COMBAT_RESURRECTION_CHARGE:
            l_Data.Initialize(SMSG_INSTANCE_ENCOUNTER_GAIN_COMBAT_RESURRECTION_CHARGE, 4 * 2);
            l_Data << int32(0);         // InCombatResCount
            l_Data << uint32(0);        // CombatResChargeRecovery
            break;
        default:
            break;
    }

    instance->SendToPlayers(&l_Data);
}

void InstanceScript::SendScenarioState(ScenarioData p_Data, Player* p_Player /*= nullptr*/)
{
    WorldPacket l_Data(SMSG_SCENARIO_STATE);

    l_Data << int32(p_Data.m_ScenarioID);
    l_Data << int32(p_Data.m_StepID);
    l_Data << uint32(instance->GetDifficulty());
    l_Data << uint32(p_Data.m_WaveCurrent);
    l_Data << uint32(p_Data.m_WaveMax);
    l_Data << uint32(p_Data.m_TimerDuration);

    l_Data << uint32(p_Data.m_CriteriaCount);
    l_Data << uint32(p_Data.m_BonusCount);

    for (CriteriaProgressData l_ProgressData : p_Data.m_CriteriaProgress)
        BuildCriteriaProgressPacket(&l_Data, l_ProgressData);

    for (BonusObjectiveData l_BonusObjective : p_Data.m_BonusObjectives)
    {
        l_Data << int32(l_BonusObjective.m_ObjectiveID);
        l_Data.WriteBit(l_BonusObjective.m_ObjectiveComplete);
        l_Data.FlushBits();
    }

    l_Data.WriteBit(p_Data.m_ScenarioComplete);
    l_Data.FlushBits();

    if (p_Player == nullptr)
        instance->SendToPlayers(&l_Data);
    else
        p_Player->SendDirectMessage(&l_Data);
}

void InstanceScript::SendScenarioProgressUpdate(CriteriaProgressData p_Data, Player* p_Player /*= nullptr*/)
{
    WorldPacket l_Data(SMSG_SCENARIO_PROGRESS_UPDATE, 39);
    BuildCriteriaProgressPacket(&l_Data, p_Data);

    if (p_Player == nullptr)
        instance->SendToPlayers(&l_Data);
    else
        p_Player->SendDirectMessage(&l_Data);
}

void InstanceScript::BuildCriteriaProgressPacket(WorldPacket* p_Data, CriteriaProgressData p_CriteriaProgress)
{
    *p_Data << int32(p_CriteriaProgress.m_ID);
    *p_Data << uint64(p_CriteriaProgress.m_Quantity);
    p_Data->appendPackGUID(p_CriteriaProgress.m_Guid);
    *p_Data << uint32(secsToTimeBitFields(p_CriteriaProgress.m_Date));
    *p_Data << int32(p_CriteriaProgress.m_TimeFromStart);
    *p_Data << int32(p_CriteriaProgress.m_TimeFromCreate);

    p_Data->WriteBits(p_CriteriaProgress.m_Flags, 4);
    p_Data->FlushBits();
}

void InstanceScript::UpdateCriteriasAfterLoading()
{
    for (uint8 l_I = 0; l_I < bosses.size(); ++l_I)
    {
        BossInfo* bossInfo = &bosses[l_I];
        BossScenarios* l_BossScenario = &m_BossesScenarios[l_I];

        if (bossInfo->state == DONE)
            SendScenarioProgressUpdate(CriteriaProgressData(l_BossScenario->m_ScenarioID, 1, m_InstanceGuid, time(NULL), m_BeginningTime, 0));
    }
}

//////////////////////////////////////////////////////////////////////////
/// ChallengesMode
void InstanceScript::SendChallengeModeStart()
{
    WorldPacket l_Data(SMSG_CHALLENGE_MODE_START, 4);
    l_Data << uint32(instance->GetId());
    instance->SendToPlayers(&l_Data);
}

void InstanceScript::SendChallengeStartTimer(uint32 p_Time)
{
    Map::PlayerList const& l_PlayerList = instance->GetPlayers();
    for (Map::PlayerList::const_iterator l_Iter = l_PlayerList.begin(); l_Iter != l_PlayerList.end(); ++l_Iter)
    {
        if (Player* l_Player = l_Iter->getSource())
            l_Player->SendStartTimer(p_Time, p_Time, CHALLENGE_TIMER);
    }
}

void InstanceScript::SendChallengeStartElapsedTimer(uint32 p_TimerID, uint32 p_Time)
{
    WorldPacket l_Data(SMSG_START_ELAPSED_TIMER, 8);
    l_Data << uint32(p_TimerID);
    l_Data << uint32(p_Time);
    instance->SendToPlayers(&l_Data);
}

void InstanceScript::SendChallengeStopElapsedTimer(uint32 p_TimerID, bool p_KeepTimer /*= false*/)
{
    WorldPacket l_Data(SMSG_STOP_ELAPSED_TIMER, 5);
    l_Data << uint32(p_TimerID);
    l_Data.WriteBit(p_KeepTimer);
    instance->SendToPlayers(&l_Data);
}

void InstanceScript::SetChallengeModeStarted()
{
    if (m_ChallengeStarted || !instance->IsChallengeMode())
        return;

    m_ChallengeStarted = true;

    ///< Challenge mode start timer is 5 seconds
    SendChallengeStartTimer(5);
    m_StartChallengeTime = 5000;
}

void InstanceScript::ScheduleChallengeStartup(uint32 p_Diff)
{
    if (m_StartChallengeTime)
    {
        if (m_StartChallengeTime <= p_Diff)
        {
            m_StartChallengeTime = 0;

            if (GameObject* l_ChallengeDoor = sObjectAccessor->FindGameObject(m_ChallengeDoorGuid))
                l_ChallengeDoor->SetGoState(GO_STATE_ACTIVE);

            SendChallengeStartElapsedTimer(1, 0);
            SendChallengeModeStart();

            m_MedalType = eChallengeMedals::MedalTypeGold;
        }
        else
            m_StartChallengeTime -= p_Diff;
    }
}

void InstanceScript::ScheduleChallengeTimeUpdate(uint32 p_Diff)
{
    if (!m_ChallengeStarted || m_StartChallengeTime)
        return;

    m_ChallengeTime += p_Diff;

    MapChallengeModeEntry const* l_ChallengeEntry = nullptr;
    for (uint32 l_I = 0; l_I < sMapChallengeModeStore.GetNumRows(); ++l_I)
    {
        if (MapChallengeModeEntry const* l_Challenge = sMapChallengeModeStore.LookupEntry(l_I))
        {
            if (l_Challenge->MapID == instance->GetId())
            {
                l_ChallengeEntry = l_Challenge;
                break;
            }
        }
    }

    ///< Should not happens
    if (l_ChallengeEntry == nullptr)
        return;

    uint32 l_Times[eChallengeMedals::MedalTypeGold];
    MapChallengeModeHotfix* l_ChallengeHotfix = sObjectMgr->GetMapChallengeModeHotfix(l_ChallengeEntry->ID);
    if (l_ChallengeHotfix != nullptr)
    {
        l_Times[eChallengeMedals::MedalTypeBronze - 1] = l_ChallengeHotfix->m_BronzeTime;
        l_Times[eChallengeMedals::MedalTypeSilver - 1] = l_ChallengeHotfix->m_SilverTime;
        l_Times[eChallengeMedals::MedalTypeGold - 1] = l_ChallengeHotfix->m_GoldTime;
    }
    else
    {
        l_Times[eChallengeMedals::MedalTypeBronze - 1] = l_ChallengeEntry->BronzeTime;
        l_Times[eChallengeMedals::MedalTypeSilver - 1] = l_ChallengeEntry->SilverTime;
        l_Times[eChallengeMedals::MedalTypeGold - 1] = l_ChallengeEntry->GoldTime;
    }

    ///< Downgrade Medal if needed
    switch (m_MedalType)
    {
        case eChallengeMedals::MedalTypeGold:
            if (m_ChallengeTime > l_Times[eChallengeMedals::MedalTypeGold - 1])
                --m_MedalType;
            break;
        case eChallengeMedals::MedalTypeSilver:
            if (m_ChallengeTime > l_Times[eChallengeMedals::MedalTypeSilver - 1])
                --m_MedalType;
            break;
        case eChallengeMedals::MedalTypeBronze:
            if (m_ChallengeTime > l_Times[eChallengeMedals::MedalTypeBronze - 1])
                --m_MedalType;
            break;
        default:
            break;
    }
}

void InstanceScript::ScheduleBeginningTimeUpdate(uint32 p_Diff)
{
    m_BeginningTime += p_Diff;
}

void InstanceScript::SendChallengeNewPlayerRecord()
{
    uint32 l_MapID = instance->GetId();

    Map::PlayerList const& l_PlayerList = instance->GetPlayers();
    for (Map::PlayerList::const_iterator l_Iter = l_PlayerList.begin(); l_Iter != l_PlayerList.end(); ++l_Iter)
    {
        if (Player* l_Player = l_Iter->getSource())
        {
            if (l_Player->HasChallengeCompleted(l_MapID))
            {
                CompletedChallenge* l_Challenge = l_Player->GetCompletedChallenge(l_MapID);
                ///< Should not happens
                if (l_Challenge == nullptr)
                    continue;

                bool l_NewBestTime = m_ChallengeTime < l_Challenge->m_BestTime;
                bool l_NewBestMedal = m_MedalType > l_Challenge->m_BestMedal;

                PreparedStatement* l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_UPD_COMPLETED_CHALLENGE);
                l_Statement->setUInt32(0, l_NewBestTime ? m_ChallengeTime : l_Challenge->m_BestTime);
                l_Statement->setUInt32(1, m_ChallengeTime);
                l_Statement->setUInt8(2, l_NewBestMedal ? m_MedalType : l_Challenge->m_BestMedal);
                l_Statement->setUInt32(3, l_NewBestMedal ? time(NULL) : l_Challenge->m_BestMedalDate);
                l_Statement->setUInt32(4, l_Player->GetGUIDLow());
                l_Statement->setUInt32(5, l_MapID);
                CharacterDatabase.Execute(l_Statement);

                if (l_NewBestMedal)
                {
                    l_Challenge->m_BestMedal = m_MedalType;
                    l_Challenge->m_BestMedalDate = time(NULL);
                }

                if (l_NewBestTime)
                    l_Challenge->m_BestTime = m_ChallengeTime;

                l_Challenge->m_LastTime = m_ChallengeTime;

                ///< Send new record only for new best time
                if (!l_NewBestTime)
                    continue;
            }
            else
            {
                PreparedStatement* l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_INS_COMPLETED_CHALLENGE);
                l_Statement->setUInt32(0, l_Player->GetGUIDLow());
                l_Statement->setUInt32(1, l_MapID);
                l_Statement->setUInt32(2, m_ChallengeTime);
                l_Statement->setUInt32(3, m_ChallengeTime);
                l_Statement->setUInt8(4, m_MedalType);
                l_Statement->setUInt32(5, time(NULL));
                CharacterDatabase.Execute(l_Statement);
            }

            WorldPacket l_Data(SMSG_CHALLENGE_MODE_NEW_PLAYER_RECORD, 12);
            l_Data << uint32(l_MapID);
            l_Data << uint32(m_ChallengeTime);
            l_Data << uint32(m_MedalType);
            l_Player->SendDirectMessage(&l_Data);
        }
    }
}

void InstanceScript::SendChallengeModeComplete(uint32 p_Money)
{
    uint32 l_ItemRewards = 0;
    uint32 l_CurrencyRewards = 0;
    WorldPacket l_Data(SMSG_CHALLENGE_MODE_COMPLETE, 4 * 4 + (l_ItemRewards * 22) + (l_CurrencyRewards * 8));

    ///< Reward
    {
        l_Data << uint32(l_ItemRewards);
        l_Data << uint32(l_CurrencyRewards);
        l_Data << uint32(p_Money);

        ///< ItemReward
        for (uint32 l_I = 0; l_I < l_ItemRewards; ++l_I)
        {
            ///< ItemStruct
            {
                l_Data << int32(0);     ///< ItemEntry
                l_Data << int32(0);     ///< RandomPropertiesSeed
                l_Data << int32(0);     ///< RandomPropertiesID

                l_Data.WriteBit(true);  ///< HasBonus
                l_Data.WriteBit(false); ///< HasModifiers

                l_Data << uint8(15);    ///< UnkByte for Bonuses
                l_Data << uint32(0);    ///< BonusCount
            }

            l_Data << uint32(0);    ///< Quantity
        }

        ///< CurrencyReward
        for (uint32 l_I = 0; l_I < l_CurrencyRewards; ++l_I)
        {
            l_Data << uint32(0);    ///< CurrencyID
            l_Data << uint32(0);    ///< Quantity
        }
    }

    l_Data << uint32(m_ChallengeTime);
    l_Data << uint32(instance->GetId());
    l_Data << uint32(m_MedalType);
    instance->SendToPlayers(&l_Data);
}

void InstanceScript::SaveChallengeDatasIfNeeded()
{
    uint32 l_MapID = instance->GetId();
    RealmCompletedChallenge* l_GroupChallenge = sObjectMgr->GetGroupCompletedChallengeForMap(l_MapID);
    RealmCompletedChallenge* l_GuildChallenge = sObjectMgr->GetGuildCompletedChallengeForMap(l_MapID);

    ///< New best record for a classic group
    if (l_GroupChallenge == nullptr)
        SaveNewGroupChallenge();
    ///< Check if update is needed
    else if (l_GroupChallenge->m_CompletionTime > m_ChallengeTime)
    {
        PreparedStatement* l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GROUP_CHALLENGE);

        l_Statement->setUInt32(0, l_MapID);
        CharacterDatabase.Execute(l_Statement);

        SaveNewGroupChallenge();
    }

    bool l_GuildGroup = false;
    uint32 l_GuildID = 0;
    Map::PlayerList const& l_PlayerList = instance->GetPlayers();
    for (Map::PlayerList::const_iterator l_Iter = l_PlayerList.begin(); l_Iter != l_PlayerList.end(); ++l_Iter)
    {
        if (Player* l_Player = l_Iter->getSource())
        {
            if (Group* l_Group = l_Player->GetGroup())
            {
                if (!l_Group->IsGuildGroup(l_Player->GetGuildId(), true, true))
                    continue;

                l_GuildGroup = true;
                l_GuildID = l_Player->GetGuildId();
            }
        }
    }

    ///< New best record for the guild
    if (l_GuildChallenge == nullptr)
    {
        if (l_GuildGroup)
            SaveNewGroupChallenge(l_GuildID);
    }
    ///< Check if update is needed
    else if (l_GuildChallenge->m_CompletionTime > m_ChallengeTime)
    {
        if (l_GuildGroup)
        {
            PreparedStatement* l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GUILD_CHALLENGE);
            l_Statement->setUInt32(0, l_MapID);
            l_Statement->setUInt32(1, l_GuildID);
            CharacterDatabase.Execute(l_Statement);

            SaveNewGroupChallenge(l_GuildID);
        }
    }
}

void InstanceScript::SaveNewGroupChallenge(uint32 p_GuildID /*= 0*/)
{
    uint32 l_Index = 0;
    PreparedStatement* l_Statement = CharacterDatabase.GetPreparedStatement(p_GuildID ? CHAR_INS_GUILD_CHALLENGE : CHAR_INS_GROUP_CHALLENGE);
    l_Statement->setUInt32(l_Index++, instance->GetId());

    if (p_GuildID)
        l_Statement->setUInt32(l_Index++, p_GuildID);

    l_Statement->setUInt32(l_Index++, 0);   ///< AttemptID
    l_Statement->setUInt32(l_Index++, m_ChallengeTime);
    l_Statement->setUInt32(l_Index++, time(NULL));
    l_Statement->setUInt8(l_Index++, m_MedalType);

    Map::PlayerList const& l_PlayerList = instance->GetPlayers();
    uint32 l_Count = 0;
    l_Index = p_GuildID ? 7 : 6;
    for (Map::PlayerList::const_iterator l_Iter = l_PlayerList.begin(); l_Iter != l_PlayerList.end(); ++l_Iter)
    {
        if (Player* l_Player = l_Iter->getSource())
        {
            l_Statement->setUInt32(l_Index++, l_Player->GetGUIDLow());
            l_Statement->setUInt32(l_Index++, l_Player->GetSpecializationId(l_Player->GetActiveSpec()));
            ++l_Count;
        }
    }

    l_Statement->setUInt8(p_GuildID ? 6 : 5, l_Count);

    for (uint8 l_I = 0; l_I < ((p_GuildID ? 6 : 5) - l_Count); ++l_I)
    {
        l_Statement->setUInt32(l_Index++, 0);
        l_Statement->setUInt32(l_Index++, 0);
    }

    CharacterDatabase.Execute(l_Statement);
}

uint32 InstanceScript::RewardChallengers()
{
    ChallengeReward* l_Reward = sObjectMgr->GetChallengeRewardsForMap(instance->GetId());
    if (l_Reward == nullptr)
        return 0;

    if (m_MedalType < eChallengeMedals::MaxMedalType)
    {
        uint32 l_Money = l_Reward->m_MoneyReward[m_MedalType];

        Map::PlayerList const& l_PlayerList = instance->GetPlayers();
        for (Map::PlayerList::const_iterator l_Iter = l_PlayerList.begin(); l_Iter != l_PlayerList.end(); ++l_Iter)
        {
            if (Player* l_Player = l_Iter->getSource())
                l_Player->ModifyMoney(int64(l_Money));
        }

        return l_Money;
    }

    return 0;
}
//////////////////////////////////////////////////////////////////////////

bool InstanceScript::IsWipe()
{
    Map::PlayerList const& PlayerList = instance->GetPlayers();

    if (PlayerList.isEmpty())
        return true;

    for (Map::PlayerList::const_iterator Itr = PlayerList.begin(); Itr != PlayerList.end(); ++Itr)
    {
        Player* player = Itr->getSource();

        if (!player)
            continue;

        if (player->isAlive() && !player->isGameMaster())
            return false;
    }

    return true;
}

void InstanceScript::UpdateEncounterState(EncounterCreditType type, uint32 creditEntry, Unit* source)
{
    DungeonEncounterList const* encounters = sObjectMgr->GetDungeonEncounterList(instance->GetId(), instance->GetDifficulty());
    if (!encounters || encounters->empty())
        return;

    int32 l_MaxIndex = -100000;
    for (DungeonEncounterList::const_iterator itr = encounters->begin(); itr != encounters->end(); ++itr)
    {
        if ((*itr)->dbcEntry->OrderIndex > l_MaxIndex && (*itr)->dbcEntry->DifficultyID == NONE_DIFFICULTY)
            l_MaxIndex = (*itr)->dbcEntry->OrderIndex;
    }

    for (DungeonEncounterList::const_iterator itr = encounters->begin(); itr != encounters->end(); ++itr)
    {
        if ((source && (*itr)->dbcEntry->CreatureDisplayID == source->GetDisplayId()) || ((*itr)->creditType == type && (*itr)->creditEntry == creditEntry))
        {
            completedEncounters |= 1 << (*itr)->dbcEntry->Bit;
            sLog->outDebug(LOG_FILTER_TSCR, "Instance %s (instanceId %u) completed encounter %s", instance->GetMapName(), instance->GetInstanceId(), (*itr)->dbcEntry->NameLang);
            if ((*itr)->dbcEntry->OrderIndex == l_MaxIndex)
            {
                Map::PlayerList const& l_PlayerList = instance->GetPlayers();
                if (l_PlayerList.isEmpty())
                    return;

                for (Map::PlayerList::const_iterator l_Itr = l_PlayerList.begin(); l_Itr != l_PlayerList.end(); ++l_Itr)
                {
                    if (Player* l_Player = l_Itr->getSource())
                    {
                        uint32 l_DungeonID = l_Player->GetGroup() ? sLFGMgr->GetDungeon(l_Player->GetGroup()->GetGUID()) : 0;
                        if (!source || l_Player->IsAtGroupRewardDistance(source))
                            sLFGMgr->RewardDungeonDoneFor(l_DungeonID, l_Player);
                    }
                }
            }
            return;
        }
    }
}

void InstanceScript::UpdatePhasing()
{
    PhaseUpdateData phaseUdateData;
    phaseUdateData.AddConditionType(CONDITION_INSTANCE_DATA);

    Map::PlayerList const& players = instance->GetPlayers();
    for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
        if (Player* player = itr->getSource())
            player->GetPhaseMgr().NotifyConditionChanged(phaseUdateData);
}
