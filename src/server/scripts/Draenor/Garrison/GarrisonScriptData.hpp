////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef GARRISON_SCRIPT_DATA_HPP_GARRISON
#define GARRISON_SCRIPT_DATA_HPP_GARRISON

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "GameObjectAI.h"
#include "Spell.h"

namespace MS { namespace Garrison 
{

    /// Garrison map ids
    enum MapIDs
    {
        MapGarrisonHordeLevel1      = 1152,
        MapGarrisonHordeLevel2      = 1330,
        MapGarrisonHordeLevel3      = 1153,
        MapGarrisonAllianceLevel1   = 1158,
        MapGarrisonAllianceLevel2   = 1331,
        MapGarrisonAllianceLevel3   = 1159
    };

    enum GarrisonPhases
    {
        PhaseMagePortalFrostfireRidge = 0x00000008,
        PhaseMagePortalSpiresOfArak   = 0x00000010,
        PhaseMagePortalTalador        = 0x00000020,
        PhaseMagePortalNagrand        = 0x00000040,
        PhaseMagePortalShadowmoon     = 0x00000080,
        PhaseMagePortalGorgrond       = 0x00000100,
    };

    /// Instance data common IDs
    enum InstanceDataIDs
    {
        GARRISON_INSTANCE_DATA_PEON_ENABLED = 99,
        GARRISON_INSTANCE_DATA_THREE_FIRST  = 100,
        GARRISON_INSTANCE_DATA_THREE_END    = 110
    };

    /// Garrison common spells
    enum Spells
    {
        SPELL_GARRISON_HEARTHSTONE             = 171253,
        SPELL_RELIEVED                         = 162410,
        SPELL_LEFT_ROPE                        = 164400,
        SPELL_RIGHT_ROPE                       = 164422,
        SPELL_COMESTIC_SLEEP                   = 162907,
        SPELL_GARRISON_ORC_MALE_CARRYNG_LUMBER = 161329,
        MultiShot                              = 18651,
        Disengage                              = 82707,
        Shoot                                  = 6660,
        SpellArming                            = 167920,
        SpellIronTrap                          = 180609,
        SpellApprenticeLogging                 = 167911,
        SpellSummonStump                       = 170079,
        SpellGarrisonPortal                    = 182464
    };

    enum DisplayIDs
    {
        InvisibleDisplay   = 11686,
        GobIronTrapDisplay = 14212
    };

    /// Garrison Quests
    namespace Quests
    {
        enum
        {
            /// Alliance
            QUEST_ETABLISH_YOUR_GARRISON_A              = 34586,
            QUEST_KEEPING_IT_TOGETHER                   = 35176,
            QUEST_SHIP_SALVAGE                          = 35166,
            QUEST_PALE_MOONLIGHT                        = 35174,
            Alliance_LostInTransition                   = 37087,
            Alliance_BiggerIsBetter                     = 36592,
            /// Small profession building quests
            Alliance_YourFirstBlacksmithingWorkOrder    = 35168,
            Alliance_YourFirstAlchemyWorkOrder          = 36641,
            Alliance_YourFirstLeatherworkingWorkOrder   = 36642,
            Alliance_YourFirstEnchantingWorkOrder       = 36645,
            Alliance_YourFirstJewelcraftingWorkOrder    = 36644,
            Alliance_YourFirstEngineeringWorkOrder      = 36646,
            Alliance_YourFirstInscriptionWorkOrder      = 36647,
            /// Herb garden
            Alliance_ClearingTheGarden                  = 36404,
            /// Barn
            Alliance_BreakingIntoTheTrapGame            = 36271,
            /// Lumber Mill
            Alliance_EasingIntoLumberjacking            = 36189,
            Alliance_TurningTimberIntoProfit            = 36192,
            /// Lunar Fall Excavation
            Alliance_ThingsAreNotGorenOurWay            = 34192,
            /// Mage Tower
            Alliance_PortablePortals                    = 38354,

            /// Horde
            QUEST_ETABLISH_YOUR_GARRISON_H              = 34378,
            QUEST_WHAT_WE_GOT                           = 34824,
            QUEST_BUILD_YOUR_BARRACKS                   = 34461,
            Horde_LostInTransition                      = 37060,
            Horde_BiggerIsBetter                        = 36567,
            /// Small profession building quests
            Horde_YourFirstBlacksmithingWorkOrder       = 37569,
            Horde_YourFirstTailoringWorkOrder           = 37575,
            Horde_YourFirstAlchemyWorkOrder             = 37568,
            Horde_YourFirstLeatherworkingWorkOrder      = 37574,
            Horde_YourFirstEnchantingWorkOrder          = 37570,
            Horde_YourFirstJewelcraftingWorkOrder       = 37573,
            Horde_YourFirstEngineeringWorkOrder         = 37571,
            Horde_YourFirstInscriptionWorkOrder         = 37572,
            /// Herb garden
            Horde_ClearingTheGarden                     = 34193,
            /// Barn
            Horde_BreakingIntoTheTrapGame               = 36345,
            /// Lumber Mill
            Horde_EasingIntoLumberjacking               = 36137,
            Horde_TurningTimberIntoProfit               = 36138,
            /// Frostwall Mines
            Horde_ThingsAreNotGorenOurWay               = 35154,
            Horde_MissionProbable                       = 34775,
            /// Spirit Lodge
            Horde_PortablePortals                       = 38351
        };
    }   ///< namespace Quests

    /// Garrison achievement related
    enum Achievements
    {
        ACHIEVEMENT_STAYING_REGULAR = 8933
    };

    /// Garrison NPCs
    enum NPCs
    {
        NPC_LADY_SENA                                   = 80223,
        NPC_GARRISON_ALLIANCE_CART                      = 81627,
        NPC_GARRISON_ALLIANCE_HORSE                     = 81633,
        NPC_SENIOR_PEON_II                              = 86775,
        NPC_FROSTWALL_PEON_DYNAMIC                      = 79605,
        NPC_FROSTWALL_GRUNT                             = 79781,
        NPC_GASLOWE                                     = 78466,
        NPC_ASSISTANT_BRIGHTSTONE                       = 84455,
        NPC_SKAGGIT                                     = 80225,
        NPC_ARCHER                                      = 87518,
        Marksman                                        = 88391,
        LunarfallRaccoon                                = 85341,
        NpcFrostwallNibbler                             = 81967,
        TrapL1QuestKillCredit                           = 84882,
        NpcFarmerLokLubSummon                           = 85093,
        NpcHomerStonefield                              = 83967,
        NpcTommyJoeStonefield                           = 83968,
        NpcHordePeon                                    = 83985,
        NpcAllianceLumberjack                           = 83950,
        NpcLunarfallGoren                               = 83628,
        NpcLunarfallGorenHatchling                      = 83629,
        NpcFrostwallGoren                               = 81362,
        NpcFrostwallGorenHatchling                      = 81398,
        NpcStonetooth                                   = 81396,
        NpcAllianceMiner                                = 77361,
        NpcHordeMiner                                   = 79837
    };

    namespace StablesData
    {
        namespace Alliance
        {
            namespace FannyQuestGiver
            {
                enum WolfQuests
                {
                    /// Initial Quest
                    QuestWanglingAWolf = 36914,
                    /// Progression Quests
                    WolfTrainingQuest1 = 37022,
                    WolfTrainingQuest2 = 37023,
                    WolfTrainingQuest3 = 37024,
                    WolfTrainingQuest4 = 37025,
                    WolfTrainingQuest5 = 37026,
                    WolfTrainingQuest6 = 37027,
                    WolfTrainingQuest7 = 37028
                };

                static const std::vector<uint32> g_WolfQuests =
                {
                    WolfTrainingQuest1,
                    WolfTrainingQuest2,
                    WolfTrainingQuest3,
                    WolfTrainingQuest4,
                    WolfTrainingQuest5,
                    WolfTrainingQuest6,
                    WolfTrainingQuest7
                };

                enum RiverbeastQuests
                {
                    /// Initial Quest
                    QuestRequisitionARiverbeast = 36918,
                    /// Progression Quests
                    RiverbeastTrainingQuest1    = 37005,
                    RiverbeastTrainingQuest2    = 37006,
                    RiverbeastTrainingQuest3    = 37007,
                    RiverbeastTrainingQuest4    = 37008,
                    RiverbeastTrainingQuest5    = 37009,
                    RiverbeastTrainingQuest6    = 37010,
                    RiverbeastTrainingQuest7    = 37011,
                    RiverbeastTrainingQuest8    = 37012,
                    RiverbeastTrainingQuest9    = 37013
                };

                static const std::vector<uint32> g_RiverbeastQuests =
                {
                    RiverbeastTrainingQuest1,
                    RiverbeastTrainingQuest2,
                    RiverbeastTrainingQuest3,
                    RiverbeastTrainingQuest4,
                    RiverbeastTrainingQuest5,
                    RiverbeastTrainingQuest6,
                    RiverbeastTrainingQuest7,
                    RiverbeastTrainingQuest8,
                    RiverbeastTrainingQuest9
                };
                enum TalbukQuests
                {
                    /// Initial Quest
                    QuestTamingATalbuk    = 36911,
                    /// Progression Quests
                    TalbukTrainingQuest1  = 36971,
                    TalbukTrainingQuest2  = 36972,
                    TalbukTrainingQuest3  = 36973,
                    TalbukTrainingQuest4  = 36974,
                    TalbukTrainingQuest5  = 36975,
                    TalbukTrainingQuest6  = 36976,
                    TalbukTrainingQuest7  = 36977,
                    TalbukTrainingQuest8  = 36978,
                    TalbukTrainingQuest9  = 36979,
                    TalbukTrainingQuest10 = 36980,
                    TalbukTrainingQuest11 = 36981,
                    TalbukTrainingQuest12 = 36982
                };

                static const std::vector<uint32> g_TalbukQuests =
                {
                    TalbukTrainingQuest1,
                    TalbukTrainingQuest2,
                    TalbukTrainingQuest3,
                    TalbukTrainingQuest4,
                    TalbukTrainingQuest5,
                    TalbukTrainingQuest6,
                    TalbukTrainingQuest7,
                    TalbukTrainingQuest8,
                    TalbukTrainingQuest9,
                    TalbukTrainingQuest10,
                    TalbukTrainingQuest11,
                    TalbukTrainingQuest12
                };
            }

            namespace KeeganQuestGiver
            {
                enum ElekkQuests
                {
                    /// Initial Quest
                    QuestEntanglingAnElekk = 36915,
                    /// Progression Quests
                    ElekkTrainingQuest1    = 37015,
                    ElekkTrainingQuest2    = 37016,
                    ElekkTrainingQuest3    = 37017,
                    ElekkTrainingQuest4    = 37018,
                    ElekkTrainingQuest5    = 37019,
                    ElekkTrainingQuest6    = 37020,
                    ElekkTrainingQuest7    = 37021
                };

                static const std::vector<uint32> g_ElekkQuests =
                {
                    ElekkTrainingQuest1,
                    ElekkTrainingQuest2,
                    ElekkTrainingQuest3,
                    ElekkTrainingQuest4,
                    ElekkTrainingQuest5,
                    ElekkTrainingQuest6,
                    ElekkTrainingQuest7
                };

                enum ClefthoofQuests
                {
                    /// Initial Quest
                    QuestCapturingAClefthoof  = 36916,
                    /// Progression Quests
                    ClefthoofTrainingQuest1   = 36983,
                    ClefthoofTrainingQuest2   = 36984,
                    ClefthoofTrainingQuest3   = 36985,
                    ClefthoofTrainingQuest4   = 36986,
                    ClefthoofTrainingQuest5   = 36987,
                    ClefthoofTrainingQuest6   = 36988,
                    ClefthoofTrainingQuest7   = 36989,
                    ClefthoofTrainingQuest8   = 36990,
                    ClefthoofTrainingQuest9   = 36991,
                    ClefthoofTrainingQuest10  = 36992,
                    ClefthoofTrainingQuest11  = 36993,
                    ClefthoofTrainingQuest12  = 36994
                };

                static const std::vector<uint32> g_ClefthoofQuests =
                {
                    ClefthoofTrainingQuest1,
                    ClefthoofTrainingQuest2,
                    ClefthoofTrainingQuest3,
                    ClefthoofTrainingQuest4,
                    ClefthoofTrainingQuest5,
                    ClefthoofTrainingQuest6,
                    ClefthoofTrainingQuest7,
                    ClefthoofTrainingQuest8,
                    ClefthoofTrainingQuest9,
                    ClefthoofTrainingQuest10,
                    ClefthoofTrainingQuest11,
                    ClefthoofTrainingQuest12
                };

                enum BoarQuests
                {
                    /// Initial Quest
                    QuestBestingABoar   = 36913,
                    /// Progression Quests
                    BoarTrainingQuest1  = 36995,
                    BoarTrainingQuest2  = 36996,
                    BoarTrainingQuest3  = 36997,
                    BoarTrainingQuest4  = 36998,
                    BoarTrainingQuest5  = 36999,
                    BoarTrainingQuest6  = 37000,
                    BoarTrainingQuest7  = 37001,
                    BoarTrainingQuest8  = 37002,
                    BoarTrainingQuest9  = 37003,
                    BoarTrainingQuest10 = 37004
                };

                static const std::vector<uint32> g_BoarQuests =
                {
                    BoarTrainingQuest1,
                    BoarTrainingQuest2,
                    BoarTrainingQuest3,
                    BoarTrainingQuest4,
                    BoarTrainingQuest5,
                    BoarTrainingQuest6,
                    BoarTrainingQuest7,
                    BoarTrainingQuest8,
                    BoarTrainingQuest9,
                    BoarTrainingQuest10
                };
            }
        }

        namespace Horde
        {

        }
    }

    namespace GarrisonPortals
    {
        enum DraenorZones : uint16
        {
            /// TODO : Handle Tanaan Jungle in build > 6.2
            ZoneFrostfireRidge    = 6720,
            ZoneSpiresOfArak      = 6722,
            ZoneTalador           = 6662,
            ZoneNagrand           = 6755,
            ZoneShadowmoon        = 6719,
            ZoneGorgrond          = 6721
        };

        /// Those quests are used to handle portals phasing in Garrison and in Draenor
        enum PortalsQuests
        {
            QuestFrostfireRidge = 36904,
            QuestSpiresOfArak   = 36885,
            QuestTalador        = 36905,
            QuestNagrand        = 36906,
            QuestShadowmoon     = 36903,
            QuestGorgrond       = 36886,
            QuestTanaanJungle   = 39497
        };

        /// EffectForcePlayerInteraction, triggers a spell which completes associated quest
        enum ActivationSpells
        {
            FrostfireRidgeActivationSpell = 173817,
            SpiresOfArakActivationSpell   = 173554,
            TaladorActivationSpell        = 173806,
            NagrandActivationSpell        = 173809,
            ShadowmoonActivationSpell     = 173821,
            GorgrondActivationSpell       = 173504
        };

        /// EffectForcePlayerInteraction, triggers a spell which removes the associated quest from questlog,
        /// being incomplete or even rewarded
        enum DeactivationSpells
        {
            FrostfireRidgeDeactivationSpell = 173815,
            SpiresOfArakDeactivationSpell   = 173550,
            TaladorDeactivationSpell        = 173646,
            NagrandDeactivationSpell        = 173811,
            ShadowmoonDeactivationSpell     = 173819,
            GorgrondDeactivationSpell       = 173493
        };

        enum WorldGobs
        {
            FrostfireRidgeWorldGob   = 236911,
            SpiresOfArakWorldGob     = 236774,
            TaladorWorldGob          = 236906,
            NagrandWorldGob          = 236910,
            ShadowmoonValleyWorldGob = 236912,
            GorgrondWorldGob         = 236765
        };

        enum GarrisonGobs
        {
            /// Most of IDs are custom here, have them all would mean sniff them all and loose much time to farm stones on retail...
            FrostfireRidgeBuildingGob  = 236024,
            GorgrondBuildingGob        = 500003,
            NagrandBuildingGob         = 500004,
            ShadowmoonBuildingGob      = 500005,
            SpiresOfArakBuildingGob    = 500006,
            TaladorBuildingGob         = 500007
        };

        enum DraenorPortalPhases
        {
            WorldPhaseDeactivatedPortal = 0x80000000,
            WorldPhaseActivatedPortal   = 0x40000000
        };
    }

    enum GameObjects
    {
        GobStump                = 234568,
        GobBlackrockDeposit     = 232542,
        GobRichBlackrockDeposit = 232543,
        GobTrueIronDeposit      = 232544,
        GobRichTrueIronDeposit  = 232545
    };

    /// NPC texts id
    namespace NPCTexts
    {
        enum
        {
            NPC_TEXT_ASSISTANT_BRIGHTSTONE_TEXT_1        = 85492,
            NPC_TEXT_ASSISTANT_BRIGHTSTONE_TEXT_2        = 85490,
            NPC_TEXT_ASSISTANT_BRIGHTSTONE_TEXT_3        = 85493,
            NPC_TEXT_ASSISTANT_BRIGHTSTONE_TEXT_QUEST    = 83682,
            NPC_TEXT_SHELLY_HAMBY_TEXT_QUEST             = 83505,
            NPC_TEXT_LADY_SENA                           = 82232,
            NPC_TEXT_SERGENT_GRIMJAW                     = 84128,
            NPC_TEXT_SENIOR_PEON_II                      = 88704,
            OllyNimkipDefault                            = 86941
        };
    }   ///< namespace NPCTexts

    /// Creature text index
    enum CreatureTexts
    {
        CREATURE_TEXT_BAROS_ETABLISH_YOUR_GARRISON_CHAT           = 0,
        CREATURE_TEXT_BAROS_KEEPING_IT_TOGETHER_START_CHAT        = 1,
        CREATURE_TEXT_BAROS_KEEPING_IT_TOGETHER_END_CHAT          = 2,
        CREATURE_TEXT_BAROS_SHIP_SALVAGE_START_CHAT               = 3,
        CREATURE_TEXT_BAROS_SHIP_SALVAGE_END_CHAT                 = 4,
        CREATURE_TEXT_VINDICATOR_MARAAD_PALE_MOONLIGHT_START_CHAT = 0,
        CREATURE_TEXT_VINDICATOR_MARAAD_PALE_MOONLIGHT_END_CHAT   = 1,
        CREATURE_TEXT_SENIOR_PEON_II_WHAT_WE_GOT                  = 0
    };

    /// Items
    enum Items
    {
        ITEM_SHELLY_HAMBY_REPORT                = 112730,
        ITEM_GARRISON_BLUEPRINT_BARRACKS_LEVEL1 = 111956,
        ItemFurryCagedBeast                     = 119813,
        ItemLeatheryCagedBeast                  = 119814,
        ItemFireWeed                            = 109125,
        ItemTaladorOrchid                       = 109129,
        ItemNagrandArrowbloom                   = 109128,
        ItemStarflower                          = 109127,
        ItemGorgrondFlytrap                     = 109126,
        ItemFrostWeed                           = 109124,
        ItemBagOfSalvagedGoods                  = 114116,
        ItemCrateOfSalvage                      = 114119,
        ItemBigCrateOfSalvage                   = 114120,
        ItemTimber                              = 114781,
        ItemTimberSample                        = 114827,
        ItemGarrisonResources                   = 114677,
        ItemMine                                = 116055,
        ItemBlackrockOre                        = 109118,
        ItemTrueIronOre                         = 109119
    };

    /// Waypoints
    enum Waypoints
    {
        MOVE_PATH_SENIOR_PEON_II        = 600010,
        MOVE_PATH_SENIOR_PEON_II_BACK   = 600011
    };

    /// Waypoints points
    enum WaypointsPoints
    {
        MOVE_PATH_SENIOR_PEON_II_END = 31
    };

    /// Move point IDs
    enum MovePointIDs
    {
        MOVE_POINT_GAZLOWE_BARRACK_A = 0
    };

    enum ObjectIDs
    {
        TimberWorkOrderAlly  = 37548,
        TimberWorkOrderHorde = 37487,
        FirstMageTowerSpiritLodgeObjectID = 91574,
        MageTowerQuestActivateOgreWaygate = 39133
    };

    /// Garrison creation coords
    static const float gGarrisonCreationCoords[][4] =
    {
        { 1766.761475f,  191.2846830f,  72.115326f, 0.4649370f },   ///< TEAM_ALLIANCE
        { 5698.020020f, 4512.1635574f, 127.401695f, 2.8622720f }    ///< TEAM_HORDE
    };

    /// Garrison hearthstone teleport coords
    static const float gGarrisonHearthStoneTeleportCoord[][4] =
    {
        /// Horde
        { 5561.15f,  4508.785f, 132.70000f, 3.406f   }, ///< Horde Level 1
        { 5565.86f,  4593.500f, 140.89300f, 0.000f   }, ///< Horde Level 2
        { 5563.70f,  4599.920f, 141.71100f, 0.000f   }, ///< Horde Level 3
        /// Alliance
        { 1850.71f,   254.430f,  78.08300f, 1.76f    }, ///< Alliance Level 1
        { 1947.08f,   324.883f,  90.28076f, 0.00f    }, ///< Alliance Level 2
        {    0.f,       0.f,      0.f,      0.00f    }  ///< Alliance Level 3
    };

    /// Creature that follow garrison owner level
    static const uint32 gGarrisonLevelUpdateCreatures[] =
    {
        /// Alliance
        79953,  ///< Lientenant Thorn
        77370,  ///< Lunarfall Footman
        85312,  ///< Lunarfall Rifleman
        88223,  ///< SergeantCrowler
        /// Horde
        79781,  ///< Frostwall grunt
        80299,  ///< Frostwakk Axe thrower
        88228,  ///< Sergent Grinjaw
        80572   ///< Frostwall wolf
    };

    namespace Barn
    {
        /// Creatures that can be tamed with the Iron Trap for the Barn building
        namespace TameableEntries
        {
            /// First building level

            /* Result from :
            SELECT DISTINCT
            ct.entry
            FROM
            creature_template ct
            JOIN creature c
            ON (c.id = ct.`entry`)
            WHERE c.`map` = 1116       ///< Draenor
            AND ct.`family` IN (1, 43) ///< Wolf, Clefthoof
            AND ct.`rank` = 0          ///< Standard
            AND ct.`type` = 1          ///< Beast
            ORDER BY entry ASC ;*/
            static const uint32 gFirstLevel[] =
            {
                72162,
                72991,
                73132,
                73205,
                73234,
                73284,
                73571,
                73619,
                73766,
                74169,
                74600,
                74698,
                74712,
                74748,
                75680,
                76241,
                76337,
                76542,
                76575,
                76576,
                76593,
                76597,
                76660,
                76705,
                76707,
                76710,
                76711,
                76732,
                76822,
                76869,
                76889,
                76895,
                76897,
                76901,
                76902,
                77669,
                77886,
                78196,
                78364,
                78406,
                78570,
                78571,
                78572,
                78574,
                78575,
                78576,
                78798,
                78918,
                78919,
                78920,
                79034,
                79755,
                80261,
                81718,
                81774,
                81898,
                81902,
                82119,
                82205,
                82209,
                82308,
                82535,
                83829,
                84044,
                84045,
                84662,
                84793,
                84798,
                85974,
                86000,
                86414,
                86656,
                86730,
                86839,
                86847,
                86851,
                86931,
                82452
            };
        }

        enum ShipmentIDS
        {
            ShipmentFur     = 82,
            ShipmentLeather = 85
        };
    }

    namespace HordePeonData 
    {
        enum
        {
            MOVE_POINT_ID_HOME_POSITION         = 100,
            MOVE_POINT_ID_THREE_FIRST           = 101,
            MOVE_POINT_ID_THREE_END             = 111,
            MOVE_POINT_ID_WOODCUTTING_DEPOSIT_A = 112,

            MOVE_PATH_FROSTWALL_A                   = 600000,
            MOVE_PATH_FROSTWALL_A_POINT_NEAR_HOME   = 1,
            MOVE_PATH_FROSTWALL_B                   = 600001,
            MOVE_PATH_FROSTWALL_B_POINT_NEAR_HOME   = 1,
            MOVE_PATH_WOOD_CUTTING_A                = 600002,
            MOVE_PATH_WOOD_CUTTING_A_MID            = 10,
            MOVE_PATH_WOOD_CUTTING_A_END            = 20,

            PHASE_BACK_TO_HOME          = 0,
            PHASE_WOODCUTTING           = 1,
            PHASE_WOODCUTTING_DISENGAGE = 2,
            PHASE_WOODCUTTING_REENGAGE  = 3,
            PHASE_WOODCUTTING_DEPOSIT   = 4,

            WOODCUTTING_TIME            = 30 * IN_MILLISECONDS,
            WOODCUTTING_DISENGAGE_TIME  =  2 * IN_MILLISECONDS,

            WOODCUTTING_MAX_CYLE_COUNT  = 1    /// Unk retail value range 1 := 4
        };

        static const float FrostWallTreePos[][4] =
        {
            { 5543.7100f, 4600.020f, 142.000f, 2.8211f },
            { 5558.2724f, 4608.380f, 138.500f, 1.8433f },
            { 5550.3200f, 4594.158f, 136.795f, 3.7440f },
            { 5548.6600f, 4583.240f, 136.640f, 5.4320f },
            { 5544.5209f, 4600.591f, 141.500f, 2.8604f },
            { 5541.5766f, 4584.437f, 137.900f, 1.7134f },
            { 5535.4940f, 4567.104f, 137.480f, 4.0602f },
            { 5561.4848f, 4616.658f, 139.720f, 2.8347f }
        };

        static const float FrostWallWoodCuttingA[3] =
        {
            5572.927f, 4602.003f, 136.58f
        };

    }   ///< namespace HordePeonData

}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_SCRIPT_DATA_HPP_GARRISON