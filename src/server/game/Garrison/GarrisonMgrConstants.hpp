////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef GARRISON_CONSTANTS_HPP_GARRISON
#define GARRISON_CONSTANTS_HPP_GARRISON

#include "SharedDefines.h"
#include "GarrisonMgrStructures.hpp"

/// @Hatters gonna hate, float doesn't go into the Globals enum, Change standard ?
#define GARRISON_MISSION_DISTRIB_FOLLOWER_COEFF (1.5f)

namespace MS { namespace Garrison 
{
    namespace Globals
    {
        enum
        {
            MaxLevel                        = 3,
            MaxFollowerPerMission           = 3,
            MaxFollowerLevel                = 100,
            MaxActiveFollower               = 20,
            MaxFollowerItemLevel            = 675,
            FollowerActivationCost          = (250 * GOLD),
            FollowerActivationMaxStack      = 1,
            BaseMap                         = 1116,
            PlotInstanceCount               = 40,
            CurrencyID                      = 824,
            CacheMinToken                   = 5,
            CacheHeftyToken                 = 200,
            CacheMaxToken                   = 500,
            CacheTokenGenerateTime          = (10 * MINUTE),
            MissionDistributionInterval     = (25 * MINUTE),
        };
    }

    namespace FactionIndex
    {
        enum Type : uint8
        {
            Horde       = 0,
            Alliance    = 1,
            Max         = 2
        };
    }

    namespace MissionStates
    {
        enum Type : uint8
        {
            Available             = 0,
            InProgress            = 1,
            CompleteSuccess       = 2,
            CompleteFailed        = 5
        };
    }

    namespace FollowerType
    {
        enum : uint32
        {
            NPC     = 1,
            Ship    = 2
        };
    }

    namespace MissionFlags
    {
        enum : uint8
        {
            Rare        = 0x01,
            Unk2        = 0x02,
            Exhausting  = 0x04
        };
    }

    namespace LearnBluePrintResults
    {
        enum Type : uint8
        {
            Learned             = 0,
            UnableToLearn       = 1,
            Known               = 21
        };
    }

    namespace PlotTypes
    {
        enum
        {
            Small           = 0,
            Medium          = 1,
            Large           = 2,
            Farm            = 3,
            Mine            = 4,
            FishingHut      = 5,
            PetMenagerie    = 6,
            Max             = 7
        };
    }

    namespace PurchaseBuildingResults
    {
        enum Type
        {
            Ok                  = 0,
            BuildingExists      = 24,
            InvalidBuildingID   = 11,
            InvalidPlot         = 10,
            InvalidPlotBuilding = 16,
            RequireBluePrint    = 22,
            NotEnoughCurrency   = 46,
            NotEnoughGold       = 47
        };
    }

    namespace MissionBonusRollResults
    {
        enum Type
        {
            Ok      = 0,
            Error   = 1
        };
    }

    namespace AbilityEffectTypes
    {
        enum Type
        {
            ModUnk0                        = 0,    ///< @TODO
            ModWinRateSolo                 = 1,    ///< Proc if MissionFollowerCount == 1
            ModWinRate                     = 2,    ///< Proc every time
            ModTravelTime                  = 3,    ///< Proc every time
            ModXpGain                      = 4,    ///< Mod the XP earn (self, party)
            ModWinRateClass                = 5,    ///< Proc if Find(MissionFollowers[Class], MiscValueA) != NULL 
            ModWinRateDurationMore         = 6,    ///< Proc if Duration > (3600 * MiscValueB)
            ModWinRateDurationLess         = 7,    ///< Proc if Duration < (3600 * MiscValueB)
            ModGarrCurrencyDrop            = 8,    ///< Implemented.
            ModWinRateTravelDurationMore   = 9,    ///< Proc if TravelDuration > (3600 * MiscValueB)
            ModWinRateTravelDurationLess   = 10,   ///< Proc if TravelDuration < (3600 * MiscValueB)
            ModUnk11                       = 11,   ///< UNUSED
            ModDummyProduction             = 12,   ///< @TODO
            ModBronzeTreasureDrop          = 13,   ///< @TODO
            ModSilverTreasureDrop          = 14,   ///< @TODO
            ModGoldTreasureDrop            = 15,   ///< @TODO
            ModChestDropRate               = 16,   ///< @TODO
            ModMissionDuration             = 17    ///< Proc every time
        };
    }

    namespace AbilityEffectTargetMasks
    {
        enum Type
        {
            Unk     = 0 << 0,
            Self    = 1 << 0,
            Party   = 1 << 1
        };
    }

    namespace MechanicTypes
    {
        enum Type
        {
            Environment = 0,
            Racial      = 1,
            Ability     = 2
        };
    }

    /// WorldState : See WorldState.dbc
    namespace WorldStates
    {
        enum Type
        {
            CacheNumToken = 9573
        };
    }

    namespace BaseQuests
    {
        enum
        {
            FoundedGarrisonH = 36793,
            FoundedGarrisonA = 36797
        };
    }

    enum GarrisonAbilitySpells
    {
        GARRISON_SPELL_GARR_ABILITY                 = 161691,
        GARRISON_SPELL_GARR_ABILITY_HORDE_BASE      = 161693,
        GARRISON_SPELL_GARR_ABILITY_ALLIANCE_BASE   = 161694,
    };

    enum GarrisonAbilities
    {
        AbilityHerbalism = 53
    };

    /// TerrainSwap : See WorldMapArea.dbc
    enum
    {
        TERRAIN_SWAP_GARRISON_FF_HORDE_TIER_1     = 980,
        TERRAIN_SWAP_GARRISON_FF_HORDE_TIER_2     = 990,
        TERRAIN_SWAP_GARRISON_FF_HORDE_TIER_3     = 981,
        TERRAIN_SWAP_GARRISON_SMV_ALLIANCE_TIER_1 = 973,
        TERRAIN_SWAP_GARRISON_SMV_ALLIANCE_TIER_2 = 991,
        TERRAIN_SWAP_GARRISON_SMV_ALLIANCE_TIER_3 = 974
    };

    namespace CreatureAIDataIDs
    {
        enum
        {
            Builder         = 10000,
            PeonWorking     = 10001,
            BuildingID      = 10002,
            PlotInstanceID  = 10003,
            GatheredPos     = 10004,
            HasRecipe       = 0x40000000
        };
    }

    namespace TaxiNodes
    {
        enum
        {
            Alliance = 1476,
            Horde    = 1432
        };
    }

    namespace Buildings
    {
        enum
        {
            DwarvenBunker__WarMill_Level1               = 8,
            DwarvenBunker__WarMill_Level2               = 9,
            DwarvenBunker__WarMill_Level3               = 10,
            Barracks__Barracks_Level1                   = 26,
            Barracks__Barracks_Level2                   = 27,
            Barracks__Barracks_Level3                   = 28,
            Storehouse__Storehouse_Level1               = 51,
            Storehouse__Storehouse_Level2               = 142,
            Storehouse__Storehouse_Level3               = 143,
            TheForge__TheForge_Level1                   = 60,
            TheForge__TheForge_Level2                   = 117,
            TheForge__TheForge_Level3                   = 118,
            TradingPost__TradingPost_Level1             = 111,
            TradingPost__TradingPost_Level2             = 144,
            TradingPost__TradingPost_Level3             = 145,
            TailoringEmporium_TailoringEmporium_Level1  = 94,
            TailoringEmporium__TailoringEmporium_Level2 = 127,
            TailoringEmporium__TailoringEmporium_Level3 = 128,
            AlchemyLab__AlchemyLab_Level1               = 76,
            AlchemyLab__AlchemyLab_Level2               = 119,
            AlchemyLab__AlchemyLab_Level3               = 120,
            TheTannery_TheTannery_Level1                = 90,
            TheTannery__TheTannery_Level2               = 121,
            TheTannery__TheTannery_Level3               = 122,
            EnchanterStudy_EnchanterStudy_Level1        = 93,
            EnchanterStudy_EnchanterStudy_Level2        = 125,
            EnchanterStudy_EnchanterStudy_Level3        = 126,
            GemBoutique_GemBoutique_Level1              = 96,
            GemBoutique__GemBoutique_Level2             = 131,
            GemBoutique__GemBoutique_Level3             = 132,
            EngineeringWorks__EngineeringWorks_Level1   = 91,
            EngineeringWorks__EngineeringWorks_Level2   = 123,
            EngineeringWorks__EngineeringWorks_Level3   = 124,
            ScribesQuarters__ScribesQuarters_Level1     = 95,
            ScribesQuarters__ScribesQuarters_Level2     = 129,
            ScribesQuarters__ScribesQuarters_Level3     = 130,
            HerbGarden__HerbGarden_Level1               = 29,
            HerbGarden__HerbGarden_Level2               = 136,
            HerbGarden__HerbGarden_Level3               = 137
        };
    }

    enum GarrisonFollowerFlags
    {
        GARRISON_FOLLOWER_FLAG_EXHAUSTED    = 0x2,
        GARRISON_FOLLOWER_FLAG_INACTIVE     = 0x4
    };

    /// 6.1.2 19865 - SceneScript.db2 "Garrison - Building Activation Init"
    namespace BuildingType
    {
        enum Type
        {
            Unk                 = 0,    ///< Unused 1
            Mine                = 1,
            Farm                = 2,
            Barn                = 3,
            LumberMill          = 4,
            Inn                 = 5,
            TradingPost         = 6,
            PetMenagerie        = 7,
            Barracks            = 8,
            Orchard             = 9,    ///< Unused 2
            Armory              = 10,
            Stable              = 11,
            Academy             = 12,
            MageTower           = 13,
            SalvageYard         = 14,
            StoreHouse          = 15,
            Alchemy             = 16,
            Blacksmith          = 17,
            Enchanting          = 18,
            Engineering         = 19,
            Inscription         = 20,
            Jewelcrafting       = 21,
            Leatherworking      = 22,
            Tailoring           = 23,
            Fishing             = 24,
            SparringArena       = 25,
            Workshop            = 26
        };
    }

    namespace BuildingCategory
    {
        enum Type
        {
            Prebuilt    = 0,
            UserBuilt   = 1
        };
    }

    namespace WorkOrderGODisplayID
    {
        enum
        {
            BaseA       = 15585,
            BaseH       = 20508,
            AllComplete = 16091,
        };
    }

    static const uint32 gGarrisonBuildingMaxWorkOrderPerBuildingLevel[Globals::MaxLevel] =
    {
        7,
        14,
        21
    };

    static const uint32 gGarrisonInGarrisonAreaID[FactionIndex::Max] =
    {
        7004,   ///< Horde
        7078    ///< Alliance
    };

    static const uint32 gGarrisonEmptyPlotGameObject[PlotTypes::Max * FactionIndex::Max] =
    {
        /// Horde
        233083,     ///< PlotTypes::Small
        232425,     ///< PlotTypes::Medium
        233081,     ///< PlotTypes::Large
        232425,     ///< PlotTypes::Farm
        232425,     ///< PlotTypes::Mine
        233083,     ///< PlotTypes::FishingHut
        233083,     ///< PlotTypes::PetMenagerie
        /// Alliance
        229501,     ///< PlotTypes::Small
        232283,     ///< PlotTypes::Medium
        232143,     ///< PlotTypes::Large
        232283,     ///< PlotTypes::Farm
        232283,     ///< PlotTypes::Mine
        229501,     ///< PlotTypes::FishingHut
        229501      ///< PlotTypes::PetMenagerie
    };

    static const uint32 gGarrisonBuildingPlotGameObject[PlotTypes::Max * FactionIndex::Max] =
    {
        /// Horde
        233958,     ///< PlotTypes::Small
        232373,     ///< PlotTypes::Medium
        232410,     ///< PlotTypes::Large
        232373,     ///< PlotTypes::Farm          same as PlotTypes::Medium
        232373,     ///< PlotTypes::Mine          same as PlotTypes::Medium
        233958,     ///< PlotTypes::FishingHut    same as PlotTypes::Small
        233958,     ///< PlotTypes::PetMenagerie  same as PlotTypes::Small
        /// Alliance
        233957,     ///< PlotTypes::Small
        232409,     ///< PlotTypes::Medium
        232411,     ///< PlotTypes::Large
        232409,     ///< PlotTypes::Farm          same as PlotTypes::Medium
        232409,     ///< PlotTypes::Mine          same as PlotTypes::Medium
        233957,     ///< PlotTypes::FishingHut    same as PlotTypes::Small
        233957      ///< PlotTypes::PetMenagerie  same as PlotTypes::Small
    };

    static const float gGarrisonBuildingPlotAABBDiminishReturnFactor[PlotTypes::Max * FactionIndex::Max] =
    {
        /// Horde
        10,         ///< PlotTypes::Small
        16,         ///< PlotTypes::Medium
        24,         ///< PlotTypes::Large
        0,          ///< PlotTypes::Farm          same as PlotTypes::Medium
        0,          ///< PlotTypes::Mine          same as PlotTypes::Medium
        0,          ///< PlotTypes::FishingHut    same as PlotTypes::Small
        0,          ///< PlotTypes::PetMenagerie  same as PlotTypes::Small
        /// Alliance
        10,         ///< PlotTypes::Small
        16,         ///< PlotTypes::Medium
        24,         ///< PlotTypes::Large
        16,         ///< PlotTypes::Farm          same as PlotTypes::Medium
        16,         ///< PlotTypes::Mine          same as PlotTypes::Medium
        10,         ///< PlotTypes::FishingHut    same as PlotTypes::Small
        10          ///< PlotTypes::PetMenagerie  same as PlotTypes::Small
    };

    static const uint32 gGarrisonBuildingActivationGameObject[FactionIndex::Max] =
    {
        233248,     ///< Horde
        233250      ///< Alliance
    };

    static const GarrisonPlotInstanceInfoLocation gGarrisonPlotInstanceInfoLocation[Globals::PlotInstanceCount] =
    {
        /// SiteLevelID PlotInstanceID      X            Y            Z           O
        /// Alliance Level 1
        {       5,          19,         1829.896f,    197.5504f,    72.00920f,   1.8849560f  },
        {       5,          23,         1911.550f,    232.9792f,    76.65489f,   2.7952700f  },
        /// Alliance Level 2
        {     444,          18,         1819.583f,    231.2813f,    72.17403f,  -1.2915440f  },
        {     444,          19,         1829.896f,    197.5504f,    71.98585f,   1.8849560f  },
        {     444,          22,         1864.955f,    320.2083f,    81.66045f,  -1.4835300f  },
        {     444,          23,         1918.637f,    228.7674f,    76.63956f,   2.7750740f  },
        {     444,          59,         1845.083f,    146.2743f,    53.43811f,   0.3490658f  },
        {     444,          63,         1847.615f,    134.7257f,    78.10705f,   2.7052600f  },
        {     444,          67,         2031.594f,    174.4410f,    84.59409f,   2.8361600f  },
        /// Alliance Level 3
        {       6,          18,         1819.583f,    231.2813f,    72.17403f,  -1.2915440f  },
        {       6,          19,         1830.528f,    196.7465f,    71.98586f,   1.9024090f  },
        {       6,          20,         1804.333f,    189.1458f,    70.07482f,   1.9634950f  },
        {       6,          22,         1864.955f,    320.2083f,    81.66045f,  -1.4835300f  },
        {       6,          23,         1918.637f,    228.7674f,    76.63956f,   2.7750740f  },
        {       6,          24,         1814.323f,    286.3941f,    76.62489f,  -0.6544983f  },
        {       6,          25,         1893.337f,    185.1684f,    78.05621f,   2.5307280f  },
        {       6,          59,         1845.083f,    146.2743f,    53.41686f,   0.3490658f  },
        {       6,          63,         1847.615f,    134.7257f,    78.10705f,   2.7052600f  },
        {       6,          67,         2031.594f,    174.4410f,    84.36597f,   2.8361600f  },
        {       6,          81,         1914.083f,    354.1875f,    88.96585f,  -1.8763060f  },
        /// Horde Level 1
        {     258,          18,         5645.124f,   4508.9600f,   119.27010f,   2.0423500f  },
        {     258,          23,         5575.461f,   4459.3380f,   130.36810f,   0.9599311f  },
        /// Horde level 2
        {     445,          18,         5645.124f,   4508.9600f,   119.27010f,   2.0420350f  },
        {     445,          19,         5665.686f,   4549.2660f,   119.27020f,  -2.7488930f  },
        {     445,          22,         5525.882f,   4523.5690f,   131.71830f,  -0.0436332f  },
        {     445,          23,         5574.562f,   4460.0310f,   130.36800f,   0.9424779f  },
        {     445,          59,         5399.831f,   4465.9760f,   114.46130f,   0.8464847f  },
        {     445,          63,         5415.374f,   4586.4390f,   136.62080f,   2.3561950f  },
        {     445,          67,         5476.589f,   4622.7070f,   134.44980f,  -1.3962630f  },
        /// Horde level 3
        {     259,          18,         5643.449f,   4503.7780f,   119.27010f,   1.6318830f  },
        {     259,          19,         5665.686f,   4549.2660f,   119.26810f,  -2.7488930f  },
        {     259,          20,         5617.798f,   4510.2760f,   119.27020f,   0.8028514f  },
        {     259,          22,         5527.659f,   4523.2130f,   131.72760f,  -0.0436332f  },
        {     259,          23,         5575.183f,   4459.6670f,   130.36800f,   0.9424779f  },
        {     259,          24,         5651.279f,   4441.2680f,   130.52540f,   1.8936820f  },
        {     259,          25,         5693.363f,   4471.8970f,   130.52540f,   2.3300140f  },
        {     259,          59,         5399.831f,   4465.9760f,   114.53970f,   0.8464847f  },
        {     259,          63,         5415.374f,   4586.4390f,   136.58310f,   2.3561950f  },
        {     259,          67,         5476.589f,   4622.7070f,   134.44980f,  -1.3962630f  },
        {     259,          81,         5626.042f,   4660.6960f,   142.45800f,  -1.9024090f  }
    };

    /// Cache game object position for each faction / level
    static const GarrisonCacheInfoLocation gGarrisonCacheInfoLocation[FactionIndex::Max * Globals::MaxLevel] =
    {
        /// SiteLevelID         X           Y          Z         O
        /// Horde Level 1
        {     258,          5565.679f,  4499.0090f, 132.02610f, 0.081281f  },
        /// Horde level 2
        {     445,          5589.409f,  4596.8510f, 136.58750f, 5.953404f  },
        /// Horde level 3
        {     259,          5592.272f,  4589.9390f, 136.66830f, 5.858787f  },
        /// Alliance Level 1
        {       5,          1893.729f,   208.8733f,  77.06371f, 1.685312f  },
        /// Alliance Level 2
        {     444,          1949.946f,   287.2795f,  88.96585f, 3.255662f  },
        /// Alliance Level 3
        {       6,             0.f,        0.f,       0.f,      0.f        }
    };

    /// Cache game object entry for each faction / state
    static const uint32 gGarrisonCacheGameObjectID[FactionIndex::Max * 3] =
    {
        /// Horde
        237191,         ///< Normal
        237720,         ///< Hefty
        237722,         ///< Full
        /// Alliance
        236916,         ///< Normal
        237723,         ///< Hefty
        237724          ///< Full
    };

}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_CONSTANTS_HPP_GARRISON
