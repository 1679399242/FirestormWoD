/*
 * Copyright (C) 2008-2010 TrinityCore <http://www.trinitycore.org/>
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
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "WorldPacket.h"
#include "WorldSession.h"

#include "Battlefield.h"
#include "BattlefieldMgr.h"
#include "Opcodes.h"

//This send to player windows for invite player to join the war
//Param1:(m_Guid) the BattleId of Bf
//Param2:(ZoneId) the zone where the battle is (4197 for wg)
//Param3:(time) Time in second that the player have for accept
void WorldSession::SendBfInvitePlayerToWar(uint64 guid, uint32 zoneId, uint32 pTime)
{
    ///< Send packet
    WorldPacket data(SMSG_BATTLEFIELD_MGR_ENTRY_INVITE);
    data << uint64(guid);               ///< QueueID
    data << uint32(zoneId);             ///< Zone Id
    data << uint32(time(NULL) + pTime); ///< Invite lasts until

    ///< Sending the packet to player
    SendPacket(&data);
}

//This send invitation to player to join the queue
//Param1:(guid) the guid of Bf
void WorldSession::SendBfInvitePlayerToQueue(uint64 p_Guid)
{
    WorldPacket data(SMSG_BATTLEFIELD_MGR_QUEUE_INVITE);
    data << uint64(p_Guid);         ///< QueueID
    data << uint8(1);               ///< BattleState
    data << int32(0);
    data << int32(0);
    data << int32(0);
    data << uint32(0);
    data << uint32(0);

    data.WriteBit(0);
    data.FlushBits();
    
    ///< Sending packet to player
    SendPacket(&data);
}

//This send packet for inform player that he join queue
//Param1:(guid) the guid of Bf
//Param2:(ZoneId) the zone where the battle is (4197 for wg)
//Param3:(CanQueue) if able to queue
//Param4:(Full) on log in is full
void WorldSession::SendBfQueueInviteResponse(uint64 p_Guid, uint32 p_ZoneID, bool p_CanQueue, bool p_Full)
{
    WorldPacket l_Data(SMSG_BATTLEFIELD_MGR_QUEUE_REQUEST_RESPONSE);
    l_Data << uint64(p_Guid);               ///< QueueID
    l_Data << uint32(p_ZoneID);             ///< AreaID
    l_Data << uint8((p_CanQueue ? 1 : 0));  ///< Result (0 : you cannot queue wg, 1 : you are queued)
    l_Data << uint8(1);                     ///< Warmup
    l_Data.appendPackGUID(0);               ///< Failed Player GUID

    l_Data.WriteBit((p_Full ? 0 : 1));      ///< Logging In(0 : wg full, 1 : queue for upcoming (we may need to swap it))
    l_Data.FlushBits();

    SendPacket(&l_Data);
}

//This is call when player accept to join war
//Param1:(guid) the guid of Bf
void WorldSession::SendBfEntered(uint64 p_Guid)
{
    WorldPacket data(SMSG_BATTLEFIELD_MGR_ENTERING);
    data.WriteBit(m_Player->isAFK() ? 1 : 0);   ///< Cleared AFK
    data.WriteBit(true);                        ///< On Offense
    data.WriteBit(true);                        ///< Relocated
    data.FlushBits();
    data << uint64(p_Guid);

    SendPacket(&data);
}

void WorldSession::SendBfLeaveMessage(uint64 p_Guid, BFLeaveReason p_Reason)
{
    WorldPacket l_Data(SMSG_BATTLEFIELD_MGR_EJECTED);
    l_Data << uint64(p_Guid);   ///< Queue ID
    l_Data << uint8(p_Reason);  ///< Reason
    l_Data << uint8(2);         ///< BattleStatus

    l_Data.WriteBit(false);     ///< Relocated
    l_Data.FlushBits();

    SendPacket(&l_Data);
}

/// Send by client when he click on accept for queue
void WorldSession::HandleBfQueueInviteResponse(WorldPacket & p_Packet)
{
    uint64 l_QueueID        = 0;
    bool   l_AcceptedInvite = false;

    p_Packet >> l_QueueID;
    l_AcceptedInvite = p_Packet.ReadBit();

    sLog->outTrace(LOG_FILTER_GENERAL, "HandleQueueInviteResponse: GUID:" UI64FMTD " Accepted:%u", (uint64)l_QueueID, l_AcceptedInvite);

    if (!l_AcceptedInvite)
        return;

    Battlefield * l_Battlefield = sBattlefieldMgr->GetBattlefieldByGUID(l_QueueID);

    if (!l_Battlefield)
        return;

    l_Battlefield->PlayerAcceptInviteToQueue(m_Player);
}

/// Send by client on clicking in accept or refuse of invitation windows for join game
void WorldSession::HandleBfEntryInviteResponse(WorldPacket & p_Packet)
{
    uint64 l_QueueID = 0;
    bool l_AcceptedInvite = false;;

    p_Packet >> l_QueueID;
    l_AcceptedInvite = p_Packet.ReadBit();

    sLog->outTrace(LOG_FILTER_GENERAL, "HandleBattlefieldInviteResponse: GUID:" UI64FMTD " Accepted:%u", uint64(l_QueueID), l_AcceptedInvite);

    Battlefield* l_Battlefield = sBattlefieldMgr->GetBattlefieldByGUID(l_QueueID);

    if (!l_Battlefield)
        return;

    if (l_AcceptedInvite)
        l_Battlefield->PlayerAcceptInviteToWar(m_Player);
    else
    {
        if (m_Player->GetZoneId() == l_Battlefield->GetZoneId())
            l_Battlefield->KickPlayerFromBattlefield(m_Player->GetGUID());
    }
}

void WorldSession::HandleBfQueueRequest(WorldPacket& p_Packet)
{
    uint64 l_QueueID = 0;

    p_Packet >> l_QueueID;

    sLog->outError(LOG_FILTER_GENERAL, "HandleBfQueueRequest: GUID:" UI64FMTD " ", (uint64)l_QueueID);

    if (Battlefield * l_Battlefield = sBattlefieldMgr->GetBattlefieldByGUID(l_QueueID))
    {
        if (l_Battlefield->IsWarTime())
            l_Battlefield->InvitePlayerToWar(m_Player);
        else
        {
            uint32 l_Timer = l_Battlefield->GetTimer() / 1000;

            if (l_Timer < 15 * MINUTE)
                l_Battlefield->InvitePlayerToQueue(m_Player);
        }
    }
}

void WorldSession::HandleBfExitQueueRequest(WorldPacket & p_Packet)
{
    uint64 l_QueueID = 0;

    p_Packet >> l_QueueID;

    sLog->outTrace(LOG_FILTER_GENERAL, "HandleBfExitQueueRequest: GUID:" UI64FMTD " ", (uint64)l_QueueID);

    SendBfLeaveMessage(l_QueueID);

    if (Battlefield * l_Battlefield = sBattlefieldMgr->GetBattlefieldByGUID(l_QueueID))
        l_Battlefield->AskToLeaveQueue(m_Player);
}

void WorldSession::HandleBfExitRequest(WorldPacket& recv_data)
{
    sLog->outError(LOG_FILTER_GENERAL, "HandleBfExitRequest");
    Battlefield* bf = sBattlefieldMgr->GetBattlefieldToZoneId(m_Player->GetZoneId());
    if (bf)
         bf->KickPlayerFromBattlefield(m_Player->GetGUID());
}

void WorldSession::HandleReportPvPAFK(WorldPacket& recvData)
{
    ObjectGuid playerGuid;

    uint8 bits[8] = { 7, 6, 3, 0, 4, 5, 1, 2 };
    recvData.ReadBitInOrder(playerGuid, bits);

    recvData.FlushBits();

    uint8 bytes[8] = { 2, 4, 5, 7, 0, 1, 6, 3 };
    recvData.ReadBytesSeq(playerGuid, bytes);

    Player* reportedPlayer = ObjectAccessor::FindPlayer(playerGuid);
    if (!reportedPlayer)
    {
        sLog->outDebug(LOG_FILTER_BATTLEGROUND, "WorldSession::HandleReportPvPAFK: player not found");
        return;
    }

    sLog->outDebug(LOG_FILTER_BATTLEGROUND, "WorldSession::HandleReportPvPAFK: %s reported %s", m_Player->GetName(), reportedPlayer->GetName());

    reportedPlayer->ReportedAfkBy(m_Player);
}

void WorldSession::HandleRequestRatedBgInfo(WorldPacket & recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: CMSG_REQUEST_RATED_BG_INFO");

    uint8 unk;
    recvData >> unk;

    sLog->outDebug(LOG_FILTER_BATTLEGROUND, "WorldSession::HandleRequestRatedBgInfo: get unk = %u", unk);

    /// @Todo: perfome research in this case
    WorldPacket data(SMSG_RATED_BG_STATS, 72);
    for (int32 i = 0; i < 18; ++i)
        data << uint32(0);
    SendPacket(&data);
}

void WorldSession::HandleRequestPvpOptions(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: CMSG_REQUEST_PVP_OPTIONS_ENABLED");

    /// @Todo: perfome research in this case
    WorldPacket data(SMSG_PVP_OPTIONS_ENABLED, 1);
    data.WriteBit(1);
    data.WriteBit(1);
    data.WriteBit(1);
    data.WriteBit(1);
    data.WriteBit(1);
    data.FlushBits();
    SendPacket(&data);
}

void WorldSession::HandleRequestPvpReward(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: CMSG_REQUEST_PVP_REWARDS");

    m_Player->SendPvpRewards();
}

void WorldSession::HandleRequestRatedBgStats(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: CMSG_REQUEST_RATED_BG_STATS");

    WorldPacket data(SMSG_BATTLEFIELD_RATED_INFO, 29);

    for (int i = 0; i < MAX_PVP_SLOT; i++)
    {
        data << uint32(m_Player->GetArenaPersonalRating(i));    ///< current rating
        data << uint32(0);                                      ///< Ranking
        data << uint32(m_Player->GetSeasonGames(i));            ///< games of season
        data << uint32(m_Player->GetSeasonWins(i));             ///< games of season
        data << uint32(m_Player->GetWeekGames(i));              ///< games of week
        data << uint32(m_Player->GetWeekWins(i));               ///< won games of week
        data << uint32(m_Player->GetBestRatingOfWeek(i));       ///< best rating of week
        data << uint32(m_Player->GetBestRatingOfSeason(i));     ///< best rating of season
    }

    SendPacket(&data);
}