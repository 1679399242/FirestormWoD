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

#include "Common.h"
#include "DatabaseEnv.h"
#include "Opcodes.h"
#include "Log.h"
#include "Player.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "ObjectAccessor.h"
#include "UpdateMask.h"

void WorldSession::HandleSetSpecialization(WorldPacket& p_Packet)
{
    uint32  l_TabIndex  = p_Packet.read<uint32>();
    uint8   l_ClassID   = m_Player->getClass();

    // Avoid cheat - hack
    if (m_Player->GetSpecializationId(m_Player->GetActiveSpec()))
        return;

    uint32 l_SpecializationID       = 0;
    uint32 l_SpecializationSpell    = 0;

    for (uint32 l_I = 0; l_I < sChrSpecializationsStore.GetNumRows(); l_I++)
    {
        ChrSpecializationsEntry const * l_Specialization = sChrSpecializationsStore.LookupEntry(l_I);

        if (!l_Specialization)
            continue;

        if (l_Specialization->ClassID == l_ClassID && l_Specialization->OrderIndex == l_TabIndex)
        {
            l_SpecializationID      = l_Specialization->ID;
            l_SpecializationSpell   = l_Specialization->MasterySpellID;

            break;
        }
    }

    if (l_SpecializationID)
    {
        m_Player->SetSpecializationId(m_Player->GetActiveSpec(), l_SpecializationID);
        m_Player->SendTalentsInfoData(false);

        if (l_SpecializationSpell)
            m_Player->learnSpell(l_SpecializationSpell, false);

        m_Player->InitSpellForLevel();
        m_Player->UpdateMasteryPercentage();
    }
}

void WorldSession::HandleLearnTalents(WorldPacket& p_RecvPacket)
{
    uint32 l_TalentCount = 0; 
    
    p_RecvPacket >> l_TalentCount;

    if (l_TalentCount > MAX_TALENT_SPELLS)
        return;

    if (l_TalentCount > m_Player->GetFreeTalentPoints())
        return;

    for (uint32 l_I = 0; l_I < l_TalentCount; l_I++)
    {
        uint16 l_TalentID = p_RecvPacket.read<uint16>();
        m_Player->LearnTalent(l_TalentID);
    }

    m_Player->SendTalentsInfoData(false);
}

void WorldSession::HandleTalentWipeConfirmOpcode(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "CMSG_CONFIRM_RESPEC_WIPE");

    uint8 specializationReset = recvData.read<uint8>();

    recvData.rfinish();

    // remove fake death
    if (GetPlayer()->HasUnitState(UNIT_STATE_DIED))
        GetPlayer()->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);

    if (!specializationReset)
    {
        if (!m_Player->ResetTalents())
        {
            WorldPacket data(SMSG_RESPEC_WIPE_CONFIRM, 8+4);    //you have not any talent
            data << uint8(0); // 0 guid bit
            data << uint32(0);
            data << uint8(0);
            SendPacket(&data);
            return;
        }
    }
    else
        m_Player->ResetSpec();

    m_Player->SendTalentsInfoData(false);

    if (Unit* unit = m_Player->GetSelectedUnit())
        unit->CastSpell(m_Player, 14867, true);                  //spell: "Untalent Visual Effect"
}

void WorldSession::HandleUnlearnSkillOpcode(WorldPacket& recvData)
{
    uint32 skillId;
    recvData >> skillId;

    if (!IsProfessionSkill(skillId))
        return;

    GetPlayer()->SetSkill(skillId, 0, 0, 0);
}

void WorldSession::HandleArcheologyRequestHistory(WorldPacket& p_RecvData)
{
    WorldPacket l_Data(SMSG_SETUP_RESEARCH_HISTORY);

    CompletedProjectMap& l_Projects = GetPlayer()->GetArchaeologyMgr().GetCompletedProjects();
    uint32 l_Count = l_Projects.size();

    l_Data << uint32(l_Count);

    if (l_Count > 0)
    {
        for (CompletedProjectMap::iterator l_Iter = l_Projects.begin(); l_Iter != l_Projects.end(); ++l_Iter)
        {
            if (ResearchProjectEntry const* l_Project = sResearchProjectStore.LookupEntry((*l_Iter).first))
            {
                l_Data << uint32((*l_Iter).first);
                l_Data << uint32((*l_Iter).second.count);
                l_Data << uint32((*l_Iter).second.first_date);
            }
            else
                l_Data << uint32(0) << uint32(0) << uint32(0);
        }
    }

    SendPacket(&l_Data);
}
