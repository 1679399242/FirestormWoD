////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#include "FishingHut.hpp"
#include "FishingHut_Level1Data.hpp"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "GameObjectAI.h"
#include "Spell.h"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison 
{
    //////////////////////////////////////////////////////////////////////////
    /// 82436 - Tharis Strongcast                                         ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_TharisStrongcastData
    {
        InitSequenceFunction FnLevel1 = [](GarrisonNPCAI * p_This, Creature * p_Me)
        {
            p_This->SetupActionSequence(TharisStrongcast_Level1::MovePointLoc, TharisStrongcast_Level1::Sequence, sizeof(TharisStrongcast_Level1::Sequence), TharisStrongcast_Level1::MovePointIDs::Home);

            p_This->m_OnPointReached[TharisStrongcast_Level1::MovePointIDs::Home] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(TharisStrongcast_Level1::DestPointDuration::Home, [p_This, p_Me]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push([p_This, p_Me]() -> void
                {
                    p_This->SetFacingBuildingRelative(TharisStrongcast_Level1::MovePointLoc[TharisStrongcast_Level1::MovePointIDs::Home - TharisStrongcast_Level1::MovePointIDs::Home].O);
                });
            };

            p_This->m_OnPointReached[TharisStrongcast_Level1::MovePointIDs::Rack] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(TharisStrongcast_Level1::DestPointDuration::Rack, [p_This, p_Me]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push([p_This, p_Me]() -> void
                {
                    p_This->SetFacingBuildingRelative(TharisStrongcast_Level1::MovePointLoc[TharisStrongcast_Level1::MovePointIDs::Rack - TharisStrongcast_Level1::MovePointIDs::Home].O);
                });
            };

            p_This->m_OnPointReached[TharisStrongcast_Level1::MovePointIDs::Fishing1] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(TharisStrongcast_Level1::DestPointDuration::Fishing1, [p_This, p_Me]() -> void
                {
                    p_Me->RemoveAura(SpellFishingVisual);
                    p_Me->LoadEquipment(1, true);
                    p_This->DoNextSequenceAction();
                });
                p_This->m_DelayedOperations.push([p_This, p_Me]() -> void
                {
                    p_This->SetFacingBuildingRelative(TharisStrongcast_Level1::MovePointLoc[TharisStrongcast_Level1::MovePointIDs::Fishing1 - TharisStrongcast_Level1::MovePointIDs::Home].O);
                    p_Me->LoadEquipment(0, true);
                    p_Me->AddAura(SpellFishingVisual, p_Me);
                });
            };

            p_This->m_OnPointReached[TharisStrongcast_Level1::MovePointIDs::Fishing2] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(TharisStrongcast_Level1::DestPointDuration::Fishing1, [p_This, p_Me]() -> void
                {
                    p_Me->RemoveAura(SpellFishingVisual);
                    p_Me->LoadEquipment(1, true);
                    p_This->DoNextSequenceAction();
                });
                p_This->m_DelayedOperations.push([p_This, p_Me]() -> void
                {
                    p_This->SetFacingBuildingRelative(TharisStrongcast_Level1::MovePointLoc[TharisStrongcast_Level1::MovePointIDs::Fishing2 - TharisStrongcast_Level1::MovePointIDs::Home].O);
                    p_Me->LoadEquipment(0, true);
                    p_Me->AddAura(SpellFishingVisual, p_Me);
                });
            };

            p_This->DoNextSequenceAction();
        };

        InitSequenceFunction FnLevel2 = [](GarrisonNPCAI * p_This, Creature * me)
        {

        };

        InitSequenceFunction FnLevel3 = [](GarrisonNPCAI * p_This, Creature * me)
        {

        };

        char gScriptName[] = "npc_TharisStrongcast_Garr";
    }

    //////////////////////////////////////////////////////////////////////////
    /// 85708 - Segumi                                                    ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_SegumiData
    {
        InitSequenceFunction FnLevel1 = [](GarrisonNPCAI * p_This, Creature * p_Me)
        {
            p_This->SetupActionSequence(Segumi_Level1::MovePointLoc, Segumi_Level1::Sequence, sizeof(Segumi_Level1::Sequence), Segumi_Level1::MovePointIDs::Home);

            p_This->m_OnPointReached[Segumi_Level1::MovePointIDs::Home] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(Segumi_Level1::DestPointDuration::Home, [p_This, p_Me]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push([p_This, p_Me]() -> void { p_This->SetFacingBuildingRelative(Segumi_Level1::MovePointLoc[Segumi_Level1::MovePointIDs::Home - Segumi_Level1::MovePointIDs::Home].O); });
            };

            p_This->m_OnPointReached[Segumi_Level1::MovePointIDs::Logs] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(Segumi_Level1::DestPointDuration::Logs, [p_This, p_Me]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push([p_This, p_Me]() -> void { p_This->SetFacingBuildingRelative(Segumi_Level1::MovePointLoc[Segumi_Level1::MovePointIDs::Logs - Segumi_Level1::MovePointIDs::Home].O); });
            };

            p_This->m_OnPointReached[Segumi_Level1::MovePointIDs::Fire] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(Segumi_Level1::DestPointDuration::Fire, [p_This, p_Me]() -> void{ p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push([p_This, p_Me]() -> void { p_This->SetFacingBuildingRelative(Segumi_Level1::MovePointLoc[Segumi_Level1::MovePointIDs::Fire - Segumi_Level1::MovePointIDs::Home].O); });
            };

            p_This->m_OnPointReached[Segumi_Level1::MovePointIDs::Rack] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(Segumi_Level1::DestPointDuration::Rack, [p_This, p_Me]() -> void
                {
                    p_Me->HandleEmoteCommand(0);
                    p_Me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);

                    p_This->DoNextSequenceAction();
                });
                p_This->m_DelayedOperations.push([p_This, p_Me]() -> void
                {
                    p_This->SetFacingBuildingRelative(Segumi_Level1::MovePointLoc[Segumi_Level1::MovePointIDs::Rack - Segumi_Level1::MovePointIDs::Home].O);
                    p_Me->HandleEmoteCommand(EMOTE_STATE_USE_STANDING_NO_SHEATHE);
                });
            };

            p_This->m_OnPointReached[Segumi_Level1::MovePointIDs::Mesh] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(Segumi_Level1::DestPointDuration::Mesh, [p_This, p_Me]() -> void
                {
                    p_Me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 0);
                    p_Me->HandleEmoteCommand(0);
                    p_Me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);

                    p_This->DoNextSequenceAction();
                });
                p_This->m_DelayedOperations.push([p_This, p_Me]() -> void
                {
                    p_This->SetFacingBuildingRelative(Segumi_Level1::MovePointLoc[Segumi_Level1::MovePointIDs::Mesh - Segumi_Level1::MovePointIDs::Home].O);
                    p_Me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 8);
                    p_Me->HandleEmoteCommand(EMOTE_STATE_USE_STANDING_NO_SHEATHE);
                });
            };

            p_This->m_OnPointReached[Segumi_Level1::MovePointIDs::Chests] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(Segumi_Level1::DestPointDuration::Chests, [p_This, p_Me]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push([p_This, p_Me]() -> void { p_This->SetFacingBuildingRelative(Segumi_Level1::MovePointLoc[Segumi_Level1::MovePointIDs::Chests - Segumi_Level1::MovePointIDs::Home].O); });
            };

            p_This->m_OnPointReached[Segumi_Level1::MovePointIDs::Home2] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(Segumi_Level1::DestPointDuration::Home2, [p_This, p_Me]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push([p_This, p_Me]() -> void { p_This->SetFacingBuildingRelative(Segumi_Level1::MovePointLoc[Segumi_Level1::MovePointIDs::Home2 - Segumi_Level1::MovePointIDs::Home].O); });
            };

            p_This->m_OnPointReached[Segumi_Level1::MovePointIDs::Porch] = [p_This, p_Me]() -> void { p_This->m_DelayedOperations.push([p_This, p_Me]() -> void { p_This->DoNextSequenceAction(); }); };

            p_This->DoNextSequenceAction();
        };

        InitSequenceFunction FnLevel2 = [](GarrisonNPCAI * p_This, Creature * me)
        {

        };

        InitSequenceFunction FnLevel3 = [](GarrisonNPCAI * p_This, Creature * me)
        {

        };

        char gScriptName[] = "npc_Segumi_Garr";
    }

    //////////////////////////////////////////////////////////////////////////
    /// 77733 - Ron Ashton                                                ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_RonAshtonData
    {
        InitSequenceFunction FnLevel1 = [](GarrisonNPCAI * p_This, Creature * p_Me)
        {
            p_This->SetupActionSequence(RonAshton_Level1::MovePointLoc, RonAshton_Level1::Sequence, sizeof(RonAshton_Level1::Sequence), RonAshton_Level1::MovePointIDs::Home);

            p_This->m_OnPointReached[RonAshton_Level1::MovePointIDs::Home] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(RonAshton_Level1::DestPointDuration::Home, [p_This, p_Me]() -> void
                {
                    p_Me->HandleEmoteCommand(0);
                    p_Me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);

                    p_This->DoNextSequenceAction();
                });
                p_This->m_DelayedOperations.push([p_This, p_Me]() -> void
                {
                    p_This->SetFacingBuildingRelative(RonAshton_Level1::MovePointLoc[RonAshton_Level1::MovePointIDs::Home - RonAshton_Level1::MovePointIDs::Home].O);
                    p_Me->HandleEmoteCommand(EMOTE_STATE_USE_STANDING_NO_SHEATHE);
                });
            };

            p_This->m_OnPointReached[RonAshton_Level1::MovePointIDs::Hut1] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(RonAshton_Level1::DestPointDuration::Hut1, [p_This, p_Me]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push([p_This, p_Me]() -> void
                {
                    p_This->SetFacingBuildingRelative(RonAshton_Level1::MovePointLoc[RonAshton_Level1::MovePointIDs::Hut1 - RonAshton_Level1::MovePointIDs::Home].O);
                });
            };

            p_This->m_OnPointReached[RonAshton_Level1::MovePointIDs::Chests] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(RonAshton_Level1::DestPointDuration::Chests, [p_This, p_Me]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push([p_This, p_Me]() -> void
                {
                    p_This->SetFacingBuildingRelative(RonAshton_Level1::MovePointLoc[RonAshton_Level1::MovePointIDs::Chests - RonAshton_Level1::MovePointIDs::Home].O);
                });
            };

            p_This->m_OnPointReached[RonAshton_Level1::MovePointIDs::Fire] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(RonAshton_Level1::DestPointDuration::Fire, [p_This, p_Me]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push([p_This, p_Me]() -> void
                {
                    p_This->SetFacingBuildingRelative(RonAshton_Level1::MovePointLoc[RonAshton_Level1::MovePointIDs::Fire - RonAshton_Level1::MovePointIDs::Home].O);
                });
            };

            p_This->m_OnPointReached[RonAshton_Level1::MovePointIDs::Hut2] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(RonAshton_Level1::DestPointDuration::Hut2, [p_This, p_Me]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push([p_This, p_Me]() -> void
                {
                    p_This->SetFacingBuildingRelative(RonAshton_Level1::MovePointLoc[RonAshton_Level1::MovePointIDs::Hut2 - RonAshton_Level1::MovePointIDs::Home].O);
                });
            };

            p_This->m_OnPointReached[RonAshton_Level1::MovePointIDs::Mesh] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(RonAshton_Level1::DestPointDuration::Mesh, [p_This, p_Me]() -> void
                {
                    p_Me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 0);
                    p_Me->HandleEmoteCommand(0);
                    p_Me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);

                    p_This->DoNextSequenceAction();
                });
                p_This->m_DelayedOperations.push([p_This, p_Me]() -> void
                {
                    p_This->SetFacingBuildingRelative(RonAshton_Level1::MovePointLoc[RonAshton_Level1::MovePointIDs::Mesh - RonAshton_Level1::MovePointIDs::Home].O);
                    p_Me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 8);
                    p_Me->HandleEmoteCommand(EMOTE_STATE_USE_STANDING_NO_SHEATHE);
                });
            };

            p_This->m_OnPointReached[RonAshton_Level1::MovePointIDs::Logs] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(RonAshton_Level1::DestPointDuration::Logs, [p_This, p_Me]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push([p_This, p_Me]() -> void
                {
                    p_This->SetFacingBuildingRelative(RonAshton_Level1::MovePointLoc[RonAshton_Level1::MovePointIDs::Logs - RonAshton_Level1::MovePointIDs::Home].O);
                });
            };

            p_This->m_OnPointReached[RonAshton_Level1::MovePointIDs::Porch] = [p_This, p_Me]() -> void {p_This->DoNextSequenceAction(); };

            p_This->DoNextSequenceAction();
        };

        InitSequenceFunction FnLevel2 = [](GarrisonNPCAI * p_This, Creature * me)
        {

        };

        InitSequenceFunction FnLevel3 = [](GarrisonNPCAI * p_This, Creature * me)
        {

        };

        char gScriptName[] = "npc_RonAshton_Garr";
    }

}   ///< namespace Garrison
}   ///< namespace MS