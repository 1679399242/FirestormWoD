#include "Common.h"
#include "Language.h"
#include "DatabaseEnv.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "Opcodes.h"
#include "Log.h"
#include "World.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "UpdateMask.h"
#include "NPCHandler.h"
#include "Pet.h"
#include "MapManager.h"
#include "GarrisonMgr.hpp"
#include "CreatureAI.h"
#include "Chat.h"

void WorldSession::HandleGetGarrisonInfoOpcode(WorldPacket & p_RecvData)
{
    if (!m_Player)
        return;

    MS::Garrison::Manager * l_Garrison = m_Player->GetGarrison();

    if (!l_Garrison || !l_Garrison->GetGarrisonSiteLevelEntry())
        return;
    
    std::vector<MS::Garrison::GarrisonPlotInstanceInfoLocation>   l_Plots             = l_Garrison->GetPlots();
    std::vector<MS::Garrison::GarrisonMission>                    l_CompletedMission  = l_Garrison->GetCompletedMissions();
    std::vector<MS::Garrison::GarrisonMission>                    l_Missions          = l_Garrison->GetMissions();
    std::vector<MS::Garrison::GarrisonBuilding>                   l_Buildings         = l_Garrison->GetBuildings();
    std::vector<MS::Garrison::GarrisonFollower>                   l_Followers         = l_Garrison->GetFollowers();

    if (!m_Player->IsInGarrison())
    {
        WorldPacket l_Data(SMSG_GARRISON_REMOTE_INFO, 200);

        l_Data << uint32(1);                                                        ///< @TODO Site Count

        l_Data << int32(l_Garrison->GetGarrisonSiteLevelEntry()->SiteLevelID);      ///< Site Level ID
        l_Data << uint32(l_Buildings.size());                                       ///< Buildings

        for (uint32 l_I = 0; l_I < l_Buildings.size(); ++l_I)
        {
            l_Data << uint32(l_Buildings[l_I].PlotInstanceID);                      ///< Garr Plot Instance ID
            l_Data << uint32(l_Buildings[l_I].BuildingID);                          ///< Garr Building ID
        }

        SendPacket(&l_Data);
    }

    WorldPacket l_Infos(SMSG_GET_GARRISON_INFO_RESULT, 200);

    l_Infos << int32(l_Garrison->GetGarrisonSiteLevelEntry()->SiteID);          ///< Site ID
    l_Infos << int32(l_Garrison->GetGarrisonSiteLevelEntry()->SiteLevelID);     ///< Site Level ID
    l_Infos << int32(l_Garrison->GetGarrisonFactionIndex());                    ///< Faction Index
    
    l_Infos << uint32(l_Buildings.size());
    l_Infos << uint32(l_Plots.size());
    l_Infos << uint32(l_Followers.size());
    l_Infos << uint32(l_Missions.size());
    l_Infos << uint32(l_CompletedMission.size());

    l_Infos << int32(l_Garrison->GetNumFollowerActivationsRemaining());

    for (uint32 l_I = 0; l_I < l_Buildings.size(); ++l_I)
    {
        l_Infos << uint32(l_Buildings[l_I].PlotInstanceID);
        l_Infos << uint32(l_Buildings[l_I].BuildingID);
        l_Infos << uint32(l_Buildings[l_I].TimeBuiltStart);
        l_Infos << uint32(l_Buildings[l_I].SpecID);
        l_Infos << uint32(l_Buildings[l_I].TimeBuiltEnd);

        l_Infos.WriteBit(l_Buildings[l_I].Active);
        l_Infos.FlushBits();
    }

    for (uint32 l_I = 0; l_I < l_Plots.size(); ++l_I)
    {
        l_Infos << int32(l_Plots[l_I].PlotInstanceID);
        l_Infos << float(l_Plots[l_I].X);
        l_Infos << float(l_Plots[l_I].Y);
        l_Infos << float(l_Plots[l_I].Z);
        l_Infos << float(l_Plots[l_I].O);
        l_Infos << uint32(l_Garrison->GetPlotType(l_Plots[l_I].PlotInstanceID));
    }

    for (uint32 l_I = 0; l_I < l_Followers.size(); ++l_I)
    {
        l_Followers[l_I].Write(l_Infos);
    }

    for (uint32 l_I = 0; l_I < l_Missions.size(); ++l_I)
    {
        uint32 l_TravelDuration     = 0;
        uint32 l_MissionDuration    = 0;

        if (l_Missions[l_I].State == MS::Garrison::MissionStates::InProgress && sGarrMissionStore.LookupEntry(l_Missions[l_I].MissionID))
        {
            l_TravelDuration    = l_Garrison->GetMissionTravelDuration(l_Missions[l_I].MissionID);
            l_MissionDuration   = l_Garrison->GetMissionDuration(l_Missions[l_I].MissionID);
        }

        l_Infos << uint64(l_Missions[l_I].DatabaseID);
        l_Infos << uint32(l_Missions[l_I].MissionID);
        l_Infos << uint32(l_Missions[l_I].OfferTime);
        l_Infos << uint32(l_Missions[l_I].OfferMaxDuration);
        l_Infos << uint32(l_Missions[l_I].StartTime);
        l_Infos << uint32(l_TravelDuration);
        l_Infos << uint32(l_MissionDuration);
        l_Infos << uint32(l_Missions[l_I].State);
    }

    for (uint32 l_I = 0; l_I < l_CompletedMission.size(); ++l_I)
        l_Infos << int32(l_CompletedMission[l_I].MissionID);

    SendPacket(&l_Infos);

    std::vector<int32> l_KnownBlueprints        = l_Garrison->GetKnownBlueprints();
    std::vector<int32> l_KnownSpecializations   = l_Garrison->GetKnownSpecializations();

    WorldPacket l_Data(SMSG_GARRISON_BLUEPRINT_AND_SPECIALIZATION_DATA, 200);

    l_Data << uint32(l_KnownBlueprints.size());
    l_Data << uint32(l_KnownSpecializations.size());

    for (uint32 l_I = 0; l_I < l_KnownBlueprints.size(); ++l_I)
        l_Data << int32(l_KnownBlueprints[l_I]);

    for (uint32 l_I = 0; l_I < l_KnownSpecializations.size(); ++l_I)
        l_Data << int32(l_KnownSpecializations[l_I]);

    SendPacket(&l_Data);
}

void WorldSession::HandleRequestGarrisonUpgradeableOpcode(WorldPacket & p_RecvData)
{
    if (!m_Player)
        return;

    MS::Garrison::Manager * l_Garrison = m_Player->GetGarrison();

    if (!l_Garrison)
        return;

    bool l_CanUpgrade = false;

    WorldPacket l_Data(SMSG_GARRISON_REQUEST_UPGRADEABLE_RESULT, 4);

    l_Data << uint32(!l_CanUpgrade);

    SendPacket(&l_Data);
}

void WorldSession::HandleRequestLandingPageShipmentInfoOpcode(WorldPacket & p_RecvData)
{
    if (!m_Player)
        return;

    MS::Garrison::Manager * l_Garrison = m_Player->GetGarrison();

    if (!l_Garrison)
        return;

}

void WorldSession::HandleGarrisonMissionNPCHelloOpcode(WorldPacket & p_RecvData)
{
    if (!m_Player)
        return;

    MS::Garrison::Manager * l_Garrison = m_Player->GetGarrison();

    if (!l_Garrison)
        return;

    uint64 l_NpcGUID = 0;

    p_RecvData.readPackGUID(l_NpcGUID);

    Creature* l_Unit = GetPlayer()->GetNPCIfCanInteractWithFlag2(l_NpcGUID, UNIT_NPC_FLAG2_GARRISON_MISSION_NPC);

    if (!l_Unit)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleGarrisonMissionNPCHelloOpcode - Unit (GUID: %u) not found or you can not interact with him.", uint32(GUID_LOPART(l_NpcGUID)));
        return;
    }

    SendGarrisonOpenMissionNpc(l_NpcGUID);
}

void WorldSession::HandleGarrisonRequestBuildingsOpcode(WorldPacket & p_RecvData)
{
    if (!m_Player)
        return;

    MS::Garrison::Manager * l_Garrison = m_Player->GetGarrison();

    if (!l_Garrison || !l_Garrison->GetGarrisonSiteLevelEntry())
        return;

    std::vector<MS::Garrison::GarrisonBuilding> l_Buildings = l_Garrison->GetBuildings();

    WorldPacket l_Data(SMSG_GARRISON_GET_BUILDINGS_DATA, 200);
    l_Data << uint32(l_Buildings.size());

    for (uint32 l_I = 0; l_I < l_Buildings.size(); ++l_I)
    {
        MS::Garrison::GarrisonPlotInstanceInfoLocation l_PlotLocation = l_Garrison->GetPlot(l_Buildings[l_I].PlotInstanceID);
        uint32 l_SiteLevelPlotInstanceID = 0;
        uint32 l_BuildingPlotInstanceID  = 0;

        for (uint32 l_Y = 0; l_Y < sGarrSiteLevelPlotInstStore.GetNumRows(); ++l_Y)
        {
            const GarrSiteLevelPlotInstEntry * l_Entry = sGarrSiteLevelPlotInstStore.LookupEntry(l_Y);

            if (l_Entry && l_Entry->PlotInstanceID == l_Buildings[l_I].PlotInstanceID && l_Entry->SiteLevelID == l_Garrison->GetGarrisonSiteLevelEntry()->SiteLevelID)
            {
                l_SiteLevelPlotInstanceID = l_Entry->ID;
                break;
            }
        }

        for (uint32 l_Y = 0; l_Y < sGarrBuildingPlotInstStore.GetNumRows(); ++l_Y)
        {
            const GarrBuildingPlotInstEntry * l_Entry = sGarrBuildingPlotInstStore.LookupEntry(l_Y);

            if (l_Entry && l_Entry->SiteLevelPlotInstID == l_SiteLevelPlotInstanceID && l_Entry->BuildingID == l_Buildings[l_I].BuildingID)
            {
                l_BuildingPlotInstanceID = l_Entry->ID;
                break;
            }
        }

        l_Data << uint32(l_BuildingPlotInstanceID);
        l_Data << float(l_PlotLocation.X);
        l_Data << float(l_PlotLocation.Y);
        l_Data << float(l_PlotLocation.Z);
    }

    SendPacket(&l_Data);
}

void WorldSession::HandleGarrisonPurchaseBuildingOpcode(WorldPacket & p_RecvData)
{
    if (!m_Player)
        return;

    MS::Garrison::Manager * l_Garrison = m_Player->GetGarrison();

    if (!l_Garrison)
        return;

    uint64 l_NpcGUID = 0;

    uint32 l_BuildingID     = 0;
    uint32 l_PlotInstanceID = 0;

    p_RecvData.readPackGUID(l_NpcGUID);
    p_RecvData >> l_PlotInstanceID;
    p_RecvData >> l_BuildingID;

    sGarrisonBuildingManager->LearnAllowedBuildings(m_Player);

    Creature* l_Unit = GetPlayer()->GetNPCIfCanInteractWithFlag2(l_NpcGUID, UNIT_NPC_FLAG2_GARRISON_ARCHITECT);

    if (!l_Unit)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleGarrisonPurchaseBuildingOpcode - Unit (GUID: %u) not found or you can not interact with him.", uint32(GUID_LOPART(l_NpcGUID)));
        return;
    }

    MS::Garrison::PurchaseBuildingResults::Type l_Result = MS::Garrison::PurchaseBuildingResults::Ok;

    if (!sGarrBuildingStore.LookupEntry(l_BuildingID))
        l_Result = MS::Garrison::PurchaseBuildingResults::InvalidBuildingID;

    if (!l_Result && !sGarrPlotInstanceStore.LookupEntry(l_PlotInstanceID))
        l_Result = MS::Garrison::PurchaseBuildingResults::InvalidPlot;

    if (!l_Result && !l_Garrison->KnownBlueprint(l_BuildingID))
        l_Result = MS::Garrison::PurchaseBuildingResults::RequireBluePrint;

    if (!l_Result && l_Garrison->GetBuilding(l_BuildingID).BuildingID != 0)
        l_Result = MS::Garrison::PurchaseBuildingResults::BuildingExists;

    if (!l_Result && !l_Garrison->IsBuildingPlotInstanceValid(l_BuildingID, l_PlotInstanceID))
        l_Result = MS::Garrison::PurchaseBuildingResults::InvalidPlotBuilding;

    if (!l_Result)
        l_Result = l_Garrison->CanPurchaseBuilding(l_BuildingID);

    if (!sGarrisonBuildingManager->IsBluePrintAllowedForPurchasingBuilding(l_BuildingID, m_Player))
        l_Result = MS::Garrison::PurchaseBuildingResults::InvalidBuildingID;

    WorldPacket l_PlaceResult(SMSG_GARRISON_PLACE_BUILDING_RESULT, 26);
    l_PlaceResult << uint32(l_Result);

    if (l_Result == MS::Garrison::PurchaseBuildingResults::Ok)
    {
        MS::Garrison::GarrisonBuilding l_Building = l_Garrison->PurchaseBuilding(l_BuildingID, l_PlotInstanceID);

        l_PlaceResult << uint32(l_PlotInstanceID);
        l_PlaceResult << uint32(l_BuildingID);
        l_PlaceResult << uint32(l_Building.TimeBuiltStart);
        l_PlaceResult << uint32(l_Building.SpecID);
        l_PlaceResult << uint32(l_Building.TimeBuiltEnd);
        l_PlaceResult.WriteBit(l_Building.Active);
        l_PlaceResult.FlushBits();
    }
    else
    {
        l_PlaceResult << uint32(0);
        l_PlaceResult << uint32(0);
        l_PlaceResult << uint32(0);
        l_PlaceResult << uint32(0);
        l_PlaceResult << uint32(0);
        l_PlaceResult.WriteBit(false);
        l_PlaceResult.FlushBits();
    }

    l_PlaceResult.WriteBit(false);                      ///< Unk bit
    l_PlaceResult.FlushBits();

    SendPacket(&l_PlaceResult);
}

void WorldSession::HandleGarrisonCancelConstructionOpcode(WorldPacket & p_RecvData)
{
    if (!m_Player)
        return;

    MS::Garrison::Manager * l_Garrison = m_Player->GetGarrison();

    if (!l_Garrison)
        return;

    uint64 l_NpcGUID = 0;

    uint32 l_PlotInstanceID = 0;

    p_RecvData.readPackGUID(l_NpcGUID);
    p_RecvData >> l_PlotInstanceID;

    Creature * l_Unit = GetPlayer()->GetNPCIfCanInteractWithFlag2(l_NpcGUID, UNIT_NPC_FLAG2_GARRISON_ARCHITECT);

    if (!l_Unit)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleGarrisonCancelConstructionOpcode - Unit (GUID: %u) not found or you can not interact with him.", uint32(GUID_LOPART(l_NpcGUID)));
        return;
    }

    l_Garrison->CancelConstruction(l_PlotInstanceID);
}

void WorldSession::HandleGarrisonStartMissionOpcode(WorldPacket & p_RecvData)
{
    if (!m_Player)
        return;

    MS::Garrison::Manager * l_Garrison = m_Player->GetGarrison();

    if (!l_Garrison)
        return;

    uint64 l_NpcGUID        = 0;
    uint32 l_FollowerCount  = 0;
    uint32 l_MissionID      = 0;

    std::vector<uint64> l_Followers;

    p_RecvData.readPackGUID(l_NpcGUID);
    p_RecvData >> l_FollowerCount;
    p_RecvData >> l_MissionID;

    for (uint32 l_I = 0; l_I < l_FollowerCount; ++l_I)
    {
        uint64 l_FollowerDBID = 0;
        p_RecvData >> l_FollowerDBID;

        l_Followers.push_back(l_FollowerDBID);
    }

    Creature * l_Unit = GetPlayer()->GetNPCIfCanInteractWithFlag2(l_NpcGUID, UNIT_NPC_FLAG2_GARRISON_MISSION_NPC);

    if (!l_Unit)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleGarrisonStartMissionOpcode - Unit (GUID: %u) not found or you can not interact with him.", uint32(GUID_LOPART(l_NpcGUID)));
        return;
    }

    l_Garrison->StartMission(l_MissionID, l_Followers);
}

void WorldSession::HandleGarrisonCompleteMissionOpcode(WorldPacket & p_RecvData)
{
    if (!m_Player)
        return;

    MS::Garrison::Manager * l_Garrison = m_Player->GetGarrison();

    if (!l_Garrison)
        return;

    uint64 l_NpcGUID   = 0;
    uint32 l_MissionID = 0;

    p_RecvData.readPackGUID(l_NpcGUID);
    p_RecvData >> l_MissionID;

    Creature * l_Unit = GetPlayer()->GetNPCIfCanInteractWithFlag2(l_NpcGUID, UNIT_NPC_FLAG2_GARRISON_MISSION_NPC);

    if (!l_Unit)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleGarrisonCompleteMissionOpcode - Unit (GUID: %u) not found or you can not interact with him.", uint32(GUID_LOPART(l_NpcGUID)));
        return;
    }
    
    l_Garrison->CompleteMission(l_MissionID);
}

void WorldSession::HandleGarrisonMissionBonusRollOpcode(WorldPacket & p_RecvData)
{
    if (!m_Player)
        return;

    MS::Garrison::Manager * l_Garrison = m_Player->GetGarrison();

    if (!l_Garrison)
        return;

    uint64 l_NpcGUID    = 0;
    uint32 l_MissionID  = 0;

    p_RecvData.readPackGUID(l_NpcGUID);
    p_RecvData >> l_MissionID;

    Creature * l_Unit = GetPlayer()->GetNPCIfCanInteractWithFlag2(l_NpcGUID, UNIT_NPC_FLAG2_GARRISON_MISSION_NPC);

    if (!l_Unit)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleGarrisonMissionBonusRollOpcode - Unit (GUID: %u) not found or you can not interact with him.", uint32(GUID_LOPART(l_NpcGUID)));
        return;
    }

    l_Garrison->DoMissionBonusRoll(l_MissionID);
}

void WorldSession::HandleGarrisonChangeFollowerActivationStateOpcode(WorldPacket & p_RecvData)
{
    if (!m_Player)
        return;

    MS::Garrison::Manager * l_Garrison = m_Player->GetGarrison();

    if (!l_Garrison)
        return;

    uint64  l_FollowerDBID  = 0;
    bool    l_Desactivate   = false;

    p_RecvData >> l_FollowerDBID;
    l_Desactivate = p_RecvData.ReadBit();

    l_Garrison->ChangeFollowerActivationState(l_FollowerDBID, !l_Desactivate);
}

void WorldSession::HandleGarrisonGetShipmentInfoOpcode(WorldPacket & p_RecvData)
{
    if (!m_Player)
        return;

    MS::Garrison::Manager * l_Garrison = m_Player->GetGarrison();

    if (!l_Garrison || !m_Player->IsInGarrison())
        return;

    uint64 l_NpcGUID = 0;

    p_RecvData.readPackGUID(l_NpcGUID);

    Creature * l_Unit = GetPlayer()->GetNPCIfCanInteractWithFlag2(l_NpcGUID, UNIT_NPC_FLAG2_GARRISON_SHIPMENT_CRAFTER);

    if (!l_Unit)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleGarrisonMissionNPCHelloOpcode - Unit (GUID: %u) not found or you can not interact with him.", uint32(GUID_LOPART(l_NpcGUID)));
        return;
    }

    uint32 l_ShipmentID     = 0;
    uint32 l_OrderAvailable = 0;
    uint32 l_PlotInstanceID = 0;

    l_PlotInstanceID = l_Garrison->GetCreaturePlotInstanceID(l_NpcGUID);

    if (!!l_PlotInstanceID)
    {
        l_OrderAvailable = l_Garrison->GetBuildingMaxWorkOrder(l_PlotInstanceID);

        uint32 l_BuildingID = l_Garrison->GetBuilding(l_PlotInstanceID).BuildingID;

        if (l_BuildingID)
            l_ShipmentID = sGarrisonShipmentManager->GetShipmentIDForBuilding(l_BuildingID, m_Player, false);
    }

    bool l_Success = !!l_ShipmentID && !!l_PlotInstanceID;

    WorldPacket l_Response(SMSG_GET_SHIPMENT_INFO_RESPONSE);
    l_Response.WriteBit(l_Success);
    l_Response.FlushBits();

    if (l_Success)
    {
        std::vector<MS::Garrison::GarrisonWorkOrder> l_WorkOrders = l_Garrison->GetWorkOrders();

        uint32 l_PendingWorkOrderCount = std::count_if(l_WorkOrders.begin(), l_WorkOrders.end(), [l_PlotInstanceID](const MS::Garrison::GarrisonWorkOrder & p_Order) -> bool
        {
            return p_Order.PlotInstanceID == l_PlotInstanceID;
        });

        l_Response << uint32(l_ShipmentID);
        l_Response << uint32(l_OrderAvailable);
        l_Response << uint32(l_PendingWorkOrderCount);
        l_Response << uint32(l_PlotInstanceID);

        for (uint32 l_I = 0; l_I < l_WorkOrders.size(); ++l_I)
        {
            if (l_WorkOrders[l_I].PlotInstanceID != l_PlotInstanceID)
                continue;

            uint32 l_Duration = 0;
        
            const CharShipmentEntry * l_Entry = sCharShipmentStore.LookupEntry(l_WorkOrders[l_I].ShipmentID);
        
            if (l_Entry)
                l_Duration = l_Entry->Duration;

            l_Response << uint32(l_WorkOrders[l_I].ShipmentID);
            l_Response << uint64(l_WorkOrders[l_I].DatabaseID);
            l_Response << uint32(l_WorkOrders[l_I].CreationTime);
            l_Response << uint32(l_Duration);
        }
    }
    else
    {
        l_Response << uint32(0);
        l_Response << uint32(0);
        l_Response << uint32(0);
        l_Response << uint32(0);
    }

    SendPacket(&l_Response);
}

void WorldSession::HandleGarrisonCreateShipmentOpcode(WorldPacket & p_RecvData)
{
    std::function<void(const std::string &)> l_OnError = [this](const std::string & p_Message) -> void
    {
        if (m_Player->GetSession()->GetSecurity() > SEC_PLAYER)
            ChatHandler(m_Player).PSendSysMessage("HandleGarrisonCreateShipmentOpcode => %s", p_Message.c_str());

        WorldPacket l_Ack(SMSG_CREATE_SHIPMENT_RESPONSE, 16);
        l_Ack << uint64(0);
        l_Ack << uint32(0);
        l_Ack << uint32(1); ///< 0 = success & 1 = error

        m_Player->SendDirectMessage(&l_Ack);
    };

    if (!m_Player)
        return;

    MS::Garrison::Manager * l_Garrison = m_Player->GetGarrison();

    if (!l_Garrison || !m_Player->IsInGarrison())
        return;

    uint64 l_NpcGUID = 0;

    p_RecvData.readPackGUID(l_NpcGUID);

    Creature * l_Unit = GetPlayer()->GetNPCIfCanInteractWithFlag2(l_NpcGUID, UNIT_NPC_FLAG2_GARRISON_SHIPMENT_CRAFTER);

    if (!l_Unit)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleGarrisonMissionNPCHelloOpcode - Unit (GUID: %u) not found or you can not interact with him.", uint32(GUID_LOPART(l_NpcGUID)));
        return;
    }

    uint32 l_ShipmentID     = 0;
    uint32 l_OrderMax       = 0;
    uint32 l_PlotInstanceID = 0;

    l_PlotInstanceID = l_Garrison->GetCreaturePlotInstanceID(l_NpcGUID);

    if (!!l_PlotInstanceID)
    {
        l_OrderMax = l_Garrison->GetBuildingMaxWorkOrder(l_PlotInstanceID);

        uint32 l_BuildingID = l_Garrison->GetBuilding(l_PlotInstanceID).BuildingID;

        if (l_BuildingID)
            l_ShipmentID = sGarrisonShipmentManager->GetShipmentIDForBuilding(l_BuildingID, m_Player, true);
    }

    if (!l_ShipmentID || !l_PlotInstanceID)
    {
        l_OnError("Invalid ShipmentID or PlotInstanceID");
        return;
    }

    if (((int32)l_OrderMax - (int32)l_Garrison->GetWorkOrderCount(l_PlotInstanceID)) < 1)
    {
        l_OnError("Max work order for this building reached");
        return;
    }

    const CharShipmentEntry * l_ShipmentEntry = sCharShipmentStore.LookupEntry(l_ShipmentID);

    if (!l_ShipmentEntry)
    {
        l_OnError("Shipment entry not found");
        return;
    }

    const SpellInfo * l_Spell = sSpellMgr->GetSpellInfo(l_ShipmentEntry->SpellID);

    if (!l_Spell)
    {
        l_OnError("Shipment spell not found");
        return;
    }

    bool l_HasReagents = true;
    for (uint32 l_I = 0; l_I < MAX_SPELL_REAGENTS; ++l_I)
    {
        uint32 l_ItemEntry = l_Spell->Reagent[l_I];
        uint32 l_ItemCount = l_Spell->ReagentCount[l_I];

        if (!l_ItemEntry || !l_ItemCount)
            continue;

        if (!m_Player->HasItemCount(l_ItemEntry, l_ItemCount))
            l_HasReagents = false;
    }

    if (!l_HasReagents)
    {
        l_OnError("Doesn't have reagents");
        return;
    }

    for (uint32 l_I = 0; l_I < MAX_SPELL_REAGENTS; ++l_I)
    {
        uint32 l_ItemEntry = l_Spell->Reagent[l_I];
        uint32 l_ItemCount = l_Spell->ReagentCount[l_I];
        
        if (!l_ItemEntry || !l_ItemCount)
            continue;

        m_Player->DestroyItemCount(l_ItemEntry, l_ItemCount, true);
    }

    m_Player->CastSpell(m_Player, l_Spell, TRIGGERED_FULL_MASK);

    uint64 l_DatabaseID = l_Garrison->StartWorkOrder(l_PlotInstanceID, l_ShipmentID);

    WorldPacket l_Ack(SMSG_CREATE_SHIPMENT_RESPONSE, 16);
    l_Ack << uint64(l_DatabaseID);
    l_Ack << uint32(l_ShipmentID);
    l_Ack << uint32(l_DatabaseID == 0);

    m_Player->SendDirectMessage(&l_Ack);
}

void WorldSession::HandleGarrisonGetShipmentsOpcode(WorldPacket & p_RecvData)
{
    if (!m_Player)
        return;

    MS::Garrison::Manager * l_Garrison = m_Player->GetGarrison();

    if (!l_Garrison || !l_Garrison->GetGarrisonSiteLevelEntry())
        return;

    std::vector<MS::Garrison::GarrisonWorkOrder> l_WorkOrders = l_Garrison->GetWorkOrders();

    WorldPacket l_Data(SMSG_GET_SHIPMENTS, 500);
    l_Data << uint32(l_WorkOrders.size());

    for (uint32 l_I = 0; l_I < l_WorkOrders.size(); ++l_I)
    {
        uint32 l_Duration = 0;

        const CharShipmentEntry * l_Entry = sCharShipmentStore.LookupEntry(l_WorkOrders[l_I].ShipmentID);

        if (l_Entry)
            l_Duration = l_Entry->Duration;

        l_Data << uint32(l_WorkOrders[l_I].ShipmentID);
        l_Data << uint64(l_WorkOrders[l_I].DatabaseID);
        l_Data << uint32(l_WorkOrders[l_I].CreationTime);
        l_Data << uint32(l_Duration);
    }

    SendPacket(&l_Data);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void WorldSession::SendGarrisonOpenArchitect(uint64 p_CreatureGUID)
{
    MS::Garrison::Manager * l_Garrison = m_Player->GetGarrison();

    if (!l_Garrison)
        return;

    WorldPacket l_Data(SMSG_GARRISON_OPEN_ARCHITECT, 18);
    l_Data.appendPackGUID(p_CreatureGUID);

    SendPacket(&l_Data);
}
void WorldSession::SendGarrisonOpenMissionNpc(uint64 p_CreatureGUID)
{
    MS::Garrison::Manager * l_Garrison = m_Player->GetGarrison();

    if (!l_Garrison)
        return;

    WorldPacket l_Data(SMSG_GARRISON_OPEN_MISSION_NPC, 18);
    l_Data << uint32(1);
    l_Data << uint32(0);
    l_Data.WriteBit(false);
    l_Data.FlushBits();

    SendPacket(&l_Data);
}