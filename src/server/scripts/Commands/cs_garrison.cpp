#include "ScriptMgr.h"
#include "GameEventMgr.h"
#include "Chat.h"
#include "Garrison.h"

/// Garrison commands
class garrison_commandscript : public CommandScript
{
    public:
        /// Constructor
        garrison_commandscript() 
            : CommandScript("garrison_commandscript") 
        { 

        }

        /// Get command table
        ChatCommand * GetCommands() const
        {
            static ChatCommand blueprintCommandTable[] =
            {
                { "learn", SEC_GAMEMASTER, true, &HandleBlueprintLearnCommand, "", NULL },
            };

            static ChatCommand plotCommandTable[] =
            {
                { "info", SEC_GAMEMASTER, true, &HandlePlotInfoCommand, "", NULL },
            };

            static ChatCommand followerCommandTable[] =
            {
                { "add", SEC_GAMEMASTER, true, &HandleFollowerAddCommand, "", NULL },
            };

            static ChatCommand missionCommandTable[] =
            {
                { "add", SEC_GAMEMASTER, true, &HandleMissionAddCommand, "", NULL },
            };

            static ChatCommand garrisonCommandTable[] =
            {
                { "blueprint", SEC_GAMEMASTER,  true,   NULL, "", blueprintCommandTable },
                { "plot",      SEC_GAMEMASTER,  true,   NULL, "", plotCommandTable      },
                { "follower",  SEC_GAMEMASTER,  true,   NULL, "", followerCommandTable  },
                { "mission" ,  SEC_GAMEMASTER,  true,   NULL, "", missionCommandTable   },
                { "info",      SEC_GAMEMASTER,  true,   &HandleGarrisonInfo, "", NULL },
                { NULL,        0,               false,  NULL, "", NULL }
            };
            static ChatCommand commandTable[] =
            {
                { "garrison",   SEC_GAMEMASTER, false, NULL, "", garrisonCommandTable },
                { NULL,         0,              false, NULL, "", NULL }
            };
            return commandTable;
        }

        static bool HandleGarrisonInfo(ChatHandler * p_Handler, char const* p_Args)
        {
            Player * l_TargetPlayer = p_Handler->getSelectedPlayer();

            if (!l_TargetPlayer || !l_TargetPlayer->GetGarrison())
            {
                p_Handler->SendSysMessage(LANG_PLAYER_NOT_FOUND);
                p_Handler->SetSentErrorMessage(true);
                return false;
            }

            const GarrSiteLevelEntry * l_Entry = l_TargetPlayer->GetGarrison()->GetGarrisonSiteLevelEntry();

            p_Handler->PSendSysMessage("Garrison info");
            p_Handler->PSendSysMessage("Level : %u SiteLevelID : %u MapID : %u FactionID : %u", l_Entry->Level, l_Entry->SiteID, l_Entry->MapID, (uint32)l_TargetPlayer->GetGarrison()->GetGarrisonFactionIndex());
            p_Handler->PSendSysMessage("Cache Resource : %u", l_TargetPlayer->GetGarrison()->GetGarrisonCacheTokenCount());

            return false;
        }

        static bool HandleBlueprintLearnCommand(ChatHandler * p_Handler, char const* p_Args)
        {
            Player* l_TargetPlayer = p_Handler->getSelectedPlayer();

            if (!l_TargetPlayer || !l_TargetPlayer->GetGarrison())
            {
                p_Handler->SendSysMessage(LANG_PLAYER_NOT_FOUND);
                p_Handler->SetSentErrorMessage(true);
                return false;
            }

            if (p_Args != 0)
            {
                std::string l_Args = p_Args;

                if (l_Args == "all")
                {
                    for (uint32 l_I = 0; l_I < sGarrBuildingStore.GetNumRows(); ++l_I)
                    {
                        const GarrBuildingEntry * l_Entry = sGarrBuildingStore.LookupEntry(l_I);

                        if (l_Entry)
                            l_TargetPlayer->GetGarrison()->LearnBlueprint(l_Entry->BuildingID);
                    }

                    return true;
                }
                else
                {
                    uint32 l_Blueprint = atoi(p_Args);

                    if (!l_Blueprint)
                        return false;

                    return l_TargetPlayer->GetGarrison()->LearnBlueprint(l_Blueprint);
                }
            }

            return false;
        }

        static bool HandlePlotInfoCommand(ChatHandler * p_Handler, char const* p_Args)
        {
            Player * l_TargetPlayer = p_Handler->GetSession()->GetPlayer();

            if (!l_TargetPlayer || !l_TargetPlayer->GetGarrison())
            {
                p_Handler->SendSysMessage("You don't have a garrison");
                p_Handler->SetSentErrorMessage(true);
                return false;
            }

            GarrisonPlotInstanceInfoLocation l_Info = l_TargetPlayer->GetGarrison()->GetPlot(l_TargetPlayer->GetPositionX(), l_TargetPlayer->GetPositionY(), l_TargetPlayer->GetPositionZ());

            if (!l_Info.PlotInstanceID)
            {
                p_Handler->SendSysMessage("Plot not found");
                p_Handler->SetSentErrorMessage(true);
                return false;
            }

            p_Handler->PSendSysMessage("Plot instance id %u type %u", l_Info.PlotInstanceID, l_TargetPlayer->GetGarrison()->GetPlotType(l_Info.PlotInstanceID));
            p_Handler->PSendSysMessage("Position %f %f %f %f", l_Info.X, l_Info.Y, l_Info.Z, l_Info.O);

            GarrisonBuilding l_Building = l_TargetPlayer->GetGarrison()->GetBuilding(l_Info.PlotInstanceID);

            if (l_Building.BuildingID)
            {
                const GarrBuildingEntry * l_Entry = sGarrBuildingStore.LookupEntry(l_Building.BuildingID);

                p_Handler->PSendSysMessage("Building : %u - %s", l_Entry->BuildingID, l_TargetPlayer->GetGarrison()->GetGarrisonFactionIndex() == GARRISON_FACTION_ALLIANCE ? l_Entry->NameA : l_Entry->NameH);
                p_Handler->PSendSysMessage("Active %u Level %u", l_Building.Active, l_Entry->BuildingLevel);
            }

            return true;
        }

        static bool HandleFollowerAddCommand(ChatHandler * p_Handler, char const* p_Args)
        {
            Player* l_TargetPlayer = p_Handler->getSelectedPlayer();

            if (!l_TargetPlayer || !l_TargetPlayer->GetGarrison())
            {
                p_Handler->SendSysMessage(LANG_PLAYER_NOT_FOUND);
                p_Handler->SetSentErrorMessage(true);
                return false;
            }

            if (p_Args != 0)
            {
                std::string l_Args = p_Args;

                if (l_Args == "all")
                {
                    for (uint32 l_I = 0; l_I < sGarrFollowerStore.GetNumRows(); ++l_I)
                    {
                        const GarrFollowerEntry * l_Entry = sGarrFollowerStore.LookupEntry(l_I);

                        if (l_Entry)
                            l_TargetPlayer->GetGarrison()->AddFollower(l_Entry->ID);
                    }

                    return true;
                }
                else
                {
                    uint32 l_FollowerID = atoi(p_Args);

                    if (!l_FollowerID)
                        return false;

                    return l_TargetPlayer->GetGarrison()->AddFollower(l_FollowerID);
                }
            }

            return false;
        }

        static bool HandleMissionAddCommand(ChatHandler * p_Handler, char const* p_Args)
        {
            Player* l_TargetPlayer = p_Handler->getSelectedPlayer();

            if (!l_TargetPlayer || !l_TargetPlayer->GetGarrison())
            {
                p_Handler->SendSysMessage(LANG_PLAYER_NOT_FOUND);
                p_Handler->SetSentErrorMessage(true);
                return false;
            }

            if (p_Args != 0)
            {
                std::string l_Args = p_Args;

                if (l_Args == "all")
                {
                    for (uint32 l_I = 0; l_I < sGarrMissionStore.GetNumRows(); ++l_I)
                    {
                        const GarrMissionEntry * l_Entry = sGarrMissionStore.LookupEntry(l_I);

                        if (l_Entry)
                            l_TargetPlayer->GetGarrison()->AddMission(l_Entry->MissionRecID);
                    }

                    return true;
                }
                else
                {
                    uint32 l_MissionID = atoi(p_Args);

                    if (!l_MissionID)
                        return false;

                    return l_TargetPlayer->GetGarrison()->AddMission(l_MissionID);
                }
            }

            return false;
        }
        
};

void AddSC_garrison_commandscript()
{
    new garrison_commandscript();
}
