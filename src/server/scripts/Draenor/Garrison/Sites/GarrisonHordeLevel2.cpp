////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#include "GarrisonHordeLevel2.hpp"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "GameObjectAI.h"
#include "Spell.h"

enum
{
    GARRISON_PHASE_BASE = 0x0001,
};

namespace MS { namespace Garrison { namespace Sites
{
    /// Constructor
    InstanceMapScript_GarrisonHordeLevel2::InstanceMapScript_GarrisonHordeLevel2()
        : InstanceMapScript("instance_Garrison_H2", MapIDs::MAP_GARRISON_HORDE_LEVEL_2)
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Get an instance script instance
    /// @p_Map : Target map
    InstanceScript * InstanceMapScript_GarrisonHordeLevel2::GetInstanceScript(InstanceMap * p_Map) const
    {
        return new InstanceScript_GarrisonHordeLevel2(p_Map);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    /// @p_Map : Target map
    InstanceScript_GarrisonHordeLevel2::InstanceScript_GarrisonHordeLevel2(Map* p_Map)
        : GarrisonSiteBase(p_Map)
    {

    }
    /// Destructor
    InstanceScript_GarrisonHordeLevel2::~InstanceScript_GarrisonHordeLevel2()
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// When instance script is updated
    /// @p_Diff : Time since last update
    void InstanceScript_GarrisonHordeLevel2::Update(uint32 p_Diff)
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// When the owner enter in the garrison
    /// @p_Owner : Owner instance
    void InstanceScript_GarrisonHordeLevel2::OnOwnerEnter(Player * p_Owner)
    {
        /// Achievement "More Plots" horde side
        if (p_Owner->GetTeamId() == TEAM_HORDE && !p_Owner->GetAchievementMgr().HasAchieved(9545))
            p_Owner->GetAchievementMgr().CompletedAchievement(sAchievementStore.LookupEntry(9545), nullptr);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// When the garrison owner started a quest
    /// @p_Owner : Garrison owner
    /// @p_Quest : Started quest
    void InstanceScript_GarrisonHordeLevel2::OnQuestStarted(Player * p_Owner, const Quest * p_Quest)
    {

    }
    /// When the garrison owner reward a quest
    /// @p_Owner : Garrison owner
    /// @p_Quest : Rewarded quest
    void InstanceScript_GarrisonHordeLevel2::OnQuestReward(Player * p_Owner, const Quest * p_Quest)
    {

    }
    /// When the garrison owner abandon a quest
    /// @p_Owner : Garrison owner
    /// @p_Quest : Abandoned quest
    void InstanceScript_GarrisonHordeLevel2::OnQuestAbandon(Player * p_Owner, const Quest * p_Quest)
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Get phase mask
    /// @p_Owner : Garrison owner
    uint32 InstanceScript_GarrisonHordeLevel2::GetPhaseMask(Player * p_Owner)
    {
        uint32 l_PhaseMask = GARRISON_PHASE_BASE;

        return l_PhaseMask;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Owner can use the garrison cache ?
    /// @p_Owner : Garrison owner
    bool InstanceScript_GarrisonHordeLevel2::CanUseGarrisonCache(Player * p_Owner)
    {
        return true;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Can upgrade the garrison
    /// @p_Owner                : Garrison owner
    /// @p_CurrentGarrisonLevel : Current garrison level
    bool InstanceScript_GarrisonHordeLevel2::CanUpgrade(Player * p_Owner, uint32 p_CurrentGarrisonLevel)
    {
        if (p_CurrentGarrisonLevel != 2)
            return false;

        return false;
    }

    /// On upgrade the garrison
    /// @p_Owner : Garrison owner
    void InstanceScript_GarrisonHordeLevel2::OnUpgrade(Player * p_Owner)
    {

    }
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// When a mission start
    /// @p_Owner     : Garrison owner
    /// @p_MissionID : Started mission ID
    /// @p_Followers : Followers on the mission
    void InstanceScript_GarrisonHordeLevel2::OnMissionStart(Player * p_Owner, uint32 p_MissionID, std::vector<uint32> p_Followers)
    {

    }
    /// When a construction start, compute build time
    /// @p_Owner      : Garrison owner
    /// @p_BuildingID : Purchased building ID
    /// @p_BaseTime   : Default build time
    uint32 InstanceScript_GarrisonHordeLevel2::OnPrePurchaseBuilding(Player * p_Owner, uint32 p_BuildingID, uint32 p_BaseTime)
    {
        return p_BaseTime;
    }
    /// When a construction start
    /// @p_Owner      : Garrison owner
    /// @p_BuildingID : Purchased building ID
    void InstanceScript_GarrisonHordeLevel2::OnPurchaseBuilding(Player * p_Owner, uint32 p_BuildingID)
    {

    }
    /// When a building is activated
    /// @p_Owner      : Garrison owner
    /// @p_BuildingID : Purchased building ID
    void InstanceScript_GarrisonHordeLevel2::OnBuildingActivated(Player * p_Owner, uint32 p_BuildingID)
    {

    }

}   ///< namespace Sites
}   ///< namespace Garrison
}   ///< namespace MS

void AddSC_Garrison_H2()
{
    new MS::Garrison::Sites::InstanceMapScript_GarrisonHordeLevel2();
}