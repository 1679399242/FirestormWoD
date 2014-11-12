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

#ifndef TRINITY_DBCSTRUCTURE_H
#define TRINITY_DBCSTRUCTURE_H

#include "Common.h"
#include "DBCEnums.h"
#include "Define.h"
#include "Path.h"
#include "Util.h"

#include <map>
#include <set>
#include <vector>

// Structures using to access raw DBC data and required packing to portability

// GCC have alternative #pragma pack(N) syntax and old gcc version not support pack(push, N), also any gcc version not support it at some platform
#if defined(__GNUC__)
#pragma pack(1)
#else
#pragma pack(push, 1)
#endif

struct AchievementEntry
{
    uint32  ID;                                             // 0
    int32   Faction;                                        // 1 -1=all, 0=horde, 1=alliance
    int32   InstanceId;                                     // 2 -1=none
    uint32  Supercedes;                                     // 3 its Achievement parent (can`t start while parent uncomplete, use its Criteria if don`t have own, use its progress on begin)
    char*   TitleLang;                                      // 4
    char*   DescriptionLang;                                // 5
    uint32  Category;                                       // 6
    uint32  Points;                                         // 7 reward points
    uint32  UiOrder;                                        // 8
    uint32  Flags;                                          // 9
    uint32  IconID;                                         // 10 icon (from SpellIcon.dbc)
    char*   RewardLang;                                     // 11
    uint32  MinimumCriteria;                                // 12 - need this count of completed criterias (own or referenced achievement criterias)
    uint32  SharesCriteria;                                 // 13 - referenced achievement (counting of all completed criterias)
    uint32  CriteriaTree;                                   // 14
};

struct AchievementCategoryEntry
{
    uint32  ID;                                             // 0        m_ID
    int32   parentCategory;                                 // 1        m_parent                -1 for main category
    //char*     name;                                       // 2        m_name_mang
    //uint32    sortOrder;                                  // 3        m_ui_order
};

// DBC removed since MoP 5.0.5 + @issue : #282
struct AchievementCriteriaEntry
{
    uint32  ID;                                             // 0
    uint32  achievement;                                    // 1
    uint32  type;                                           // 2

    union
    {
        // ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE          = 0
        // @TODO: also used for player deaths..
        struct
        {
            uint32  creatureID;                             // 3
            uint32  creatureCount;                          // 4
        } kill_creature;

        // ACHIEVEMENT_CRITERIA_TYPE_WIN_BG                 = 1
        struct
        {
            uint32  bgMapID;                                // 3
            uint32  winCount;                               // 4
        } win_bg;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_ARCHAEOLOGY_PROJECTS = 3
        struct
        {
            uint32 unused;                                 // 3
            uint32 count;                                  // 4
        } archaeology;

        // ACHIEVEMENT_CRITERIA_TYPE_REACH_LEVEL            = 5
        struct
        {
            uint32  unused;                                 // 3
            uint32  level;                                  // 4
        } reach_level;

        // ACHIEVEMENT_CRITERIA_TYPE_REACH_SKILL_LEVEL      = 7
        struct
        {
            uint32  skillID;                                // 3
            uint32  skillLevel;                             // 4
        } reach_skill_level;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_ACHIEVEMENT   = 8
        struct
        {
            uint32  linkedAchievement;                      // 3
        } complete_achievement;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST_COUNT   = 9
        struct
        {
            uint32  unused;                                 // 3
            uint32  totalQuestCount;                        // 4
        } complete_quest_count;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_DAILY_QUEST_DAILY = 10
        struct
        {
            uint32  unused;                                 // 3
            uint32  numberOfDays;                           // 4
        } complete_daily_quest_daily;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUESTS_IN_ZONE = 11
        struct
        {
            uint32  zoneID;                                 // 3
            uint32  questCount;                             // 4
        } complete_quests_in_zone;

        // ACHIEVEMENT_CRITERIA_TYPE_CURRENCY = 12
        struct
        {
            uint32 currency;
            uint32 count;
        } currencyGain;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_DAILY_QUEST   = 14
        struct
        {
            uint32  unused;                                 // 3
            uint32  questCount;                             // 4
        } complete_daily_quest;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_BATTLEGROUND  = 15
        struct
        {
            uint32  mapID;                                  // 3
        } complete_battleground;

        // ACHIEVEMENT_CRITERIA_TYPE_DEATH_AT_MAP           = 16
        struct
        {
            uint32  mapID;                                  // 3
        } death_at_map;

        // ACHIEVEMENT_CRITERIA_TYPE_DEATH_IN_DUNGEON       = 18
        struct
        {
            uint32  manLimit;                               // 3
        } death_in_dungeon;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_RAID          = 19
        struct
        {
            uint32  groupSize;                              // 3 can be 5, 10 or 25
        } complete_raid;

        // ACHIEVEMENT_CRITERIA_TYPE_KILLED_BY_CREATURE     = 20
        struct
        {
            uint32  creatureEntry;                          // 3
        } killed_by_creature;

        // ACHIEVEMENT_CRITERIA_TYPE_FALL_WITHOUT_DYING     = 24
        struct
        {
            uint32  unused;                                 // 3
            uint32  fallHeight;                             // 4
        } fall_without_dying;

        // ACHIEVEMENT_CRITERIA_TYPE_DEATHS_FROM            = 26
        struct
        {
            uint32 type;                                    // 3, see enum EnviromentalDamage
        } death_from;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST         = 27
        struct
        {
            uint32  questID;                                // 3
            uint32  questCount;                             // 4
        } complete_quest;

        // ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET        = 28
        // ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET2       = 69
        struct
        {
            uint32  spellID;                                // 3
            uint32  spellCount;                             // 4
        } be_spell_target;

        // ACHIEVEMENT_CRITERIA_TYPE_CAST_SPELL             = 29
        // ACHIEVEMENT_CRITERIA_TYPE_CAST_SPELL2            = 110
        struct
        {
            uint32  spellID;                                // 3
            uint32  castCount;                              // 4
        } cast_spell;

        // ACHIEVEMENT_CRITERIA_TYPE_BG_OBJECTIVE_CAPTURE
        struct
        {
            uint32 objectiveId;                             // 3
            uint32 completeCount;                           // 4
        } bg_objective;

        // ACHIEVEMENT_CRITERIA_TYPE_HONORABLE_KILL_AT_AREA = 31
        struct
        {
            uint32  areaID;                                 // 3 Reference to AreaTable.dbc
            uint32  killCount;                              // 4
        } honorable_kill_at_area;

        // ACHIEVEMENT_CRITERIA_TYPE_WIN_ARENA              = 32
        struct
        {
            uint32 mapID;                                   // 3 Reference to Map.dbc
            uint32 count;                                   // 4  Number of times that the arena must be won.
        } win_arena;

        // ACHIEVEMENT_CRITERIA_TYPE_PLAY_ARENA             = 33
        struct
        {
            uint32  mapID;                                  // 3 Reference to Map.dbc
        } play_arena;

        // ACHIEVEMENT_CRITERIA_TYPE_LEARN_SPELL            = 34
        struct
        {
            uint32  spellID;                                // 3 Reference to Map.dbc
        } learn_spell;

        // ACHIEVEMENT_CRITERIA_TYPE_OWN_ITEM               = 36
        struct
        {
            uint32  itemID;                                 // 3
            uint32  itemCount;                              // 4
        } own_item;

        // ACHIEVEMENT_CRITERIA_TYPE_WIN_RATED_ARENA        = 37
        struct
        {
            uint32  unused;                                 // 3
            uint32  count;                                  // 4
        } win_rated_arena;

        // ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_TEAM_RATING    = 38
        struct
        {
            uint32  teamtype;                               // 3 {2, 3, 5}
        } highest_team_rating;

        // ACHIEVEMENT_CRITERIA_TYPE_REACH_TEAM_RATING      = 39
        struct
        {
            uint32  teamtype;                               // 3 {2, 3, 5}
            uint32  teamrating;                             // 4
        } reach_team_rating;

        // ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_PERSONAL_RATING = 39
        struct
        {
            uint32 teamtype; // 3 {2, 3, 5}
            uint32 PersonalRating; // 4
        } highest_personal_rating;

        // ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILL_LEVEL      = 40
        struct
        {
            uint32  skillID;                                // 3
            uint32  skillLevel;                             // 4 apprentice=1, journeyman=2, expert=3, artisan=4, master=5, grand master=6
        } learn_skill_level;

        // ACHIEVEMENT_CRITERIA_TYPE_USE_ITEM               = 41
        struct
        {
            uint32  itemID;                                 // 3
            uint32  itemCount;                              // 4
        } use_item;

        // ACHIEVEMENT_CRITERIA_TYPE_LOOT_ITEM              = 42
        struct
        {
            uint32  itemID;                                 // 3
            uint32  itemCount;                              // 4
        } loot_item;

        // ACHIEVEMENT_CRITERIA_TYPE_EXPLORE_AREA           = 43
        struct
        {
            // TODO: This rank is _NOT_ the index from AreaTable.dbc
            uint32  areaReference;                          // 3
        } explore_area;

        // ACHIEVEMENT_CRITERIA_TYPE_OWN_RANK               = 44
        struct
        {
            // TODO: This rank is _NOT_ the index from CharTitles.dbc
            uint32  rank;                                   // 3
        } own_rank;

        // ACHIEVEMENT_CRITERIA_TYPE_BUY_BANK_SLOT          = 45
        struct
        {
            uint32  unused;                                 // 3
            uint32  numberOfSlots;                          // 4
        } buy_bank_slot;

        // ACHIEVEMENT_CRITERIA_TYPE_GAIN_REPUTATION        = 46
        struct
        {
            uint32  factionID;                              // 3
            uint32  reputationAmount;                       // 4 Total reputation amount, so 42000 = exalted
        } gain_reputation;

        // ACHIEVEMENT_CRITERIA_TYPE_GAIN_EXALTED_REPUTATION= 47
        struct
        {
            uint32  unused;                                 // 3
            uint32  numberOfExaltedFactions;                // 4
        } gain_exalted_reputation;

        // ACHIEVEMENT_CRITERIA_TYPE_VISIT_BARBER_SHOP      = 48
        struct
        {
            uint32 unused;                                  // 3
            uint32 numberOfVisits;                          // 4
        } visit_barber;

        // ACHIEVEMENT_CRITERIA_TYPE_EQUIP_EPIC_ITEM        = 49
        // TODO: where is the required itemlevel stored?
        struct
        {
            uint32  itemSlot;                               // 3
            uint32  count;                                  // 4
        } equip_epic_item;

        // ACHIEVEMENT_CRITERIA_TYPE_ROLL_NEED_ON_LOOT      = 50
        struct
        {
            uint32  rollValue;                              // 3
            uint32  count;                                  // 4
        } roll_need_on_loot;
       // ACHIEVEMENT_CRITERIA_TYPE_ROLL_GREED_ON_LOOT      = 51
        struct
        {
            uint32  rollValue;                              // 3
            uint32  count;                                  // 4
        } roll_greed_on_loot;

        // ACHIEVEMENT_CRITERIA_TYPE_HK_CLASS               = 52
        struct
        {
            uint32  classID;                                // 3
            uint32  count;                                  // 4
        } hk_class;

        // ACHIEVEMENT_CRITERIA_TYPE_HK_RACE                = 53
        struct
        {
            uint32  raceID;                                 // 3
            uint32  count;                                  // 4
        } hk_race;

        // ACHIEVEMENT_CRITERIA_TYPE_DO_EMOTE               = 54
        // TODO: where is the information about the target stored?
        struct
        {
            uint32  emoteID;                                // 3 enum TextEmotes
            uint32  count;                                  // 4 count of emotes, always required special target or requirements
        } do_emote;
        // ACHIEVEMENT_CRITERIA_TYPE_DAMAGE_DONE            = 13
        // ACHIEVEMENT_CRITERIA_TYPE_HEALING_DONE           = 55
        // ACHIEVEMENT_CRITERIA_TYPE_GET_KILLING_BLOWS      = 56
        struct
        {
            uint32  unused;                                 // 3
            uint32  count;                                  // 4
        } healing_done;

        // ACHIEVEMENT_CRITERIA_TYPE_GET_KILLING_BLOWS      = 56
        struct
        {
            uint32  unused;
            uint32  killCount;
        } get_killing_blow;

        // ACHIEVEMENT_CRITERIA_TYPE_EQUIP_ITEM             = 57
        struct
        {
            uint32  itemID;                                 // 3
            uint32  count;                                  // 4
        } equip_item;

        // ACHIEVEMENT_CRITERIA_TYPE_MONEY_FROM_QUEST_REWARD= 62
        struct
        {
            uint32  unused;                                 // 3
            uint32  goldInCopper;                           // 4
        } quest_reward_money;

        // ACHIEVEMENT_CRITERIA_TYPE_LOOT_MONEY             = 67
        struct
        {
            uint32  unused;                                 // 3
            uint32  goldInCopper;                           // 4
        } loot_money;

        // ACHIEVEMENT_CRITERIA_TYPE_USE_GAMEOBJECT         = 68
        struct
        {
            uint32  goEntry;                                // 3
            uint32  useCount;                               // 4
        } use_gameobject;

        // ACHIEVEMENT_CRITERIA_TYPE_SPECIAL_PVP_KILL       = 70
        // @TODO: are those special criteria stored in the dbc or do we have to add another sql table?
        struct
        {
            uint32  unused;                                 // 3
            uint32  killCount;                              // 4
        } special_pvp_kill;

        // ACHIEVEMENT_CRITERIA_TYPE_FISH_IN_GAMEOBJECT     = 72
        struct
        {
            uint32  goEntry;                                // 3
            uint32  lootCount;                              // 4
        } fish_in_gameobject;

        // ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILLLINE_SPELLS = 75
        struct
        {
            uint32  skillLine;                              // 3
            uint32  spellCount;                             // 4
        } learn_skillline_spell;

        // ACHIEVEMENT_CRITERIA_TYPE_WIN_DUEL               = 76
        struct
        {
            uint32  unused;                                 // 3
            uint32  duelCount;                              // 4
        } win_duel;

        // ACHIEVEMENT_CRITERIA_TYPE_LOOT_EPIC_ITEM         = 90
        struct
        {
            uint32  unused;                                 //
            uint32  lootCount;                             // 4
        } loot_epic_item;

        // ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_POWER          = 96
        struct
        {
            uint32  powerType;                              // 3 mana=0, 1=rage, 3=energy, 6=runic power
        } highest_power;

        // ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_STAT           = 97
        struct
        {
            uint32  statType;                               // 3 4=spirit, 3=int, 2=stamina, 1=agi, 0=strength
        } highest_stat;

        // ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_SPELLPOWER     = 98
        struct
        {
            uint32  spellSchool;                            // 3
        } highest_spellpower;

        // ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_RATING         = 100
        struct
        {
            uint32  ratingType;                             // 3
        } highest_rating;

        // ACHIEVEMENT_CRITERIA_TYPE_LOOT_TYPE              = 109
        struct
        {
            uint32  lootType;                               // 3 3=fishing, 2=pickpocket, 4=disentchant
            uint32  lootTypeCount;                          // 4
        } loot_type;

        // ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILL_LINE       = 112
        struct
        {
            uint32  skillLine;                              // 3
            uint32  spellCount;                             // 4
        } learn_skill_line;

        // ACHIEVEMENT_CRITERIA_TYPE_EARN_HONORABLE_KILL    = 113
        struct
        {
            uint32  unused;                                 // 3
            uint32  killCount;                              // 4
        } honorable_kill;

        // ACHIEVEMENT_CRITERIA_TYPE_SPENT_GOLD_GUILD_REPAIRS = 124
        struct
        {
            uint32 unused;
            uint32 goldCount;
        } spent_gold_guild_repairs;

        // ACHIEVEMENT_CRITERIA_TYPE_REACH_GUILD_LEVEL    = 125
        struct
        {
            uint32 unused;
            uint32  level;
        } reach_guild_level;

        // ACHIEVEMENT_CRITERIA_TYPE_CRAFT_ITEMS_GUILD = 126
        struct
        {
            uint32 unused;
            uint32 itemsCount;
        } craft_items_guild;

        // ACHIEVEMENT_CRITERIA_TYPE_CATCH_FROM_POOL = 127
        struct
        {
            uint32 unused;
            uint32 catchCount;
        } catch_from_pool;

        // ACHIEVEMENT_CRITERIA_TYPE_BUY_GUILD_BANK_SLOTS = 128
        struct
        {
            uint32 unused;
            uint32 slotsCount;
        } buy_guild_bank_slots;

        // ACHIEVEMENT_CRITERIA_TYPE_EARN_GUILD_ACHIEVEMENT_POINTS = 129
        struct
        {
            uint32 unused;
            uint32  pointsCount;
        } earn_guild_achievement_points;

        // ACHIEVEMENT_CRITERIA_TYPE_WIN_RATED_BATTLEGROUND = 130
        struct
        {
            uint32 unused;
            uint32 winCount;
        } win_rated_battleground;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUESTS_GUILD = 134
        struct
        {
            uint32 unused;
            uint32 questCount;
        } complete_quests_guild;

        // ACHIEVEMENT_CRITERIA_TYPE_HONORABLE_KILLS_GUILD = 135
        struct
        {
            uint32 unused;
            uint32 killCount;
        } honorable_kills_guild;

        // ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE_TYPE_GUILD = 136
        struct
        {
            uint32 unused;
            uint32 count;
        } kill_creature_type_guild;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_GUILD_CHALLENGE_TYPE = 138
        struct
        {
            uint32 challenge_type;
            uint32 count;
        } guild_challenge_complete_type;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_GUILD_CHALLENGE = 139
        struct
        {
            uint32 unused;
            uint32 count;
        } guild_challenge_complete;

        struct
        {
            uint32  unused;
            uint32  dungeonsComplete;
        } use_lfg;

        struct
        {
            uint32  field3;                                 // 3 main requirement
            uint32  count;                                  // 4 main requirement count
        } raw;
    };

    // uint32 unk                                           // 5

    struct
    {
        uint32  additionalRequirement_type;                 // 6-7
        uint32  additionalRequirement_value;                // 8-9
    } additionalRequirements[MAX_CRITERIA_REQUIREMENTS];

    char*  name;                                            // 10        m_description_lang
    uint32  completionFlag;                                 // 11       m_flags
    uint32  timedCriteriaStartType;                         // 12       m_timer_start_event Only appears with timed achievements, seems to be the type of starting a timed Achievement, only type 1 and some of type 6 need manual starting
                                                            //              1: ByEventId(?) (serverside IDs),    2: ByQuestId,   5: ByCastSpellId(?)
                                                            //              6: BySpellIdTarget(some of these are unknown spells, some not, some maybe spells)
                                                            //              7: ByKillNpcId,  9: ByUseItemId
    uint32  timedCriteriaMiscId;                            // 13       m_timer_asset_id Alway appears with timed events, used internally to start the achievement, store
    uint32  timeLimit;                                      // 14       m_timer_time time limit in seconds
    uint32  showOrder;                                      // 15       m_ui_order  also used in achievement shift-links as index in state bitmask
    //uint32 unk1;                                          // 16 only one value, still unknown
    //uint32 unk2;                                          // 17 all zeros
    uint32 additionalConditionType[MAX_ADDITIONAL_CRITERIA_CONDITIONS];     // 18-20
    uint32 additionalConditionValue[MAX_ADDITIONAL_CRITERIA_CONDITIONS-1];    // 21-22
};


struct AreaTableEntry
{
    uint32  ID;                                             // 0        m_ID
    uint32  ContinentID;                                    // 1        m_ContinentID
    uint32  ParentAreaID;                                   // 2        m_ParentAreaID          If 0 then it's zone, else it's zone id of this area
    uint32  AreaBit;                                        // 3        m_AreaBit               Main index
    uint32  Flags;                                          // 4        m_Flags                 -> need uint64  @issue : #283
    //uint32    m_Flags2;                                   // 5        m_Flags2                -> need uint64  @issue : #283
    //uint32    m_SoundProviderPref;                        // 6        m_SoundProviderPref
    //uint32    m_SoundProviderPrefUnderwater;              // 7        m_SoundProviderPrefUnderwater
    //uint32    m_AmbienceID;                               // 8        m_AmbienceID
    //uint32    m_ZoneMusic;                                // 9        m_ZoneMusic
    //char*     m_ZoneName;                                 // 10       m_ZoneName
    //uint32    m_IntroSound;                               // 11       m_IntroSound
    int32   ExplorationLevel;                               // 12       m_ExplorationLevel
    char*   AreaNameLang;                                   // 13       m_AreaNameLang
    uint32  FactionGroupMask;                               // 14       m_FactionGroupMask
    uint32  LiquidTypeID[4];                                // 15-18    m_LiquidTypeID          Liquid override by type
    //float     m_AmbientMultiplier;                        // 19       m_AmbientMultiplier
    //uint32    m_MountFlags;                               // 20       m_MountFlags
    //uint32    m_UwIntroSound;                             // 21       m_UwIntroSound          All zeros
    //uint32    m_UwZoneMusic;                              // 22       m_UwZoneMusic
    //uint32    m_UwAmbience;                               // 23       m_UwAmbience
    //uint32    m_WorldPvpId;                               // 24       m_WorldPvpId
    //int32     m_PvpCombatWorldStateID;                    // 25       m_PvpCombatWorldStateID
    //uint32    m_WildBattlePetLevelMin;                    // 26       m_WildBattlePetLevelMin
    //uint32    m_WildBattlePetLevelMax;                    // 27       m_WildBattlePetLevelMax
    //uint32    m_WindSettingsID;                           // 28       m_WindSettingsID

    // DeathKnightStart map, maybe AreaFlags ?
    bool IsSanctuary() const
    {
        if (ContinentID == 609)
            return true;
        return (Flags & AREA_FLAG_SANCTUARY);
    }
};

#define MAX_GROUP_AREA_IDS 6

struct AreaGroupEntry
{
    uint32  AreaGroupId;                                    // 0        m_ID
    uint32  AreaId[MAX_GROUP_AREA_IDS];                     // 1-6      m_areaID
    uint32  nextGroup;                                      // 7        m_nextAreaID
};

struct AreaTriggerEntry
{
    uint32  ID;                                             // 0        m_ID
    uint32  ContinentID;                                    // 1        m_ContinentID
    float   Pos[3];                                         // 2 - 3 - 4m_x
    //uint32    m_phaseUSeFlags;                            // 5        m_phaseUSeFlags
    //uint32    m_phaseID;                                  // 6        m_phaseID
    //uint32    m_phaseGroupID;                             // 7        m_phaseGroupID
    float   Radius;                                         // 8        m_radius
    float   BoxLength;                                      // 9        m_box_length
    float   BoxWidth;                                       // 10       m_box_width
    float   BoxHeight;                                      // 11       m_box_heigh
    float   BoxYaw;                                         // 12       m_box_yaw
    //uint32    m_shapeType;                                // 13       m_shapeType
    //uint32    m_shapeID;                                  // 14       m_shapeID
    //uint32    m_areaTriggerActionSetID;                   // 15       m_areaTriggerActionSetID
    //uint32    m_flags;                                    // 16       m_flags
};

struct ArmorLocationEntry
{
  uint32    InventoryType;                                  // 0        m_ID
  float     Value[5];                                       // 1-5      m_clothmodifier / m_leathermodifier / m_chainmodifier / m_platemodifier / m_modifier  //multiplier for armor types
};

struct AuctionHouseEntry
{
    uint32  ID;                                             // 0        m_ID
    uint32  FactionID;                                      // 1        m_factionID             m_ID of faction.dbc for player factions associated with city
    uint32  DepositRate;                                    // 2        m_depositRate           1/3 from real
    uint32  ConsignmentRate;                                // 3        m_consignementRate
    //char*     name;                                       // 4        m_name_lang
};

struct BankBagSlotPricesEntry
{
    uint32  ID;                                             // 0        m_ID
    uint32  price;                                          // 1        m_cost
};

struct BarberShopStyleEntry
{
    uint32  ID;                                             // 0        m_ID
    uint32  Type;                                           // 1        m_type                  Value 0 -> hair, value 2 -> facial hair
    //char*     m_DisplayName_lang;                         // 2        m_DisplayName_lang
    //uint32    m_Description_lang;                         // 3        m_Description_lang
    //float     m_CostMultiplier;                           // 4        m_Cost_Modifier
    uint32  Race;                                           // 5        m_race
    uint32  Sex;                                            // 6        m_sex
    uint32  Data;                                           // 7        m_data                  Real ID to hair/facial hair
};

struct BattlemasterListEntry
{
    uint32  ID;                                             // 0        m_ID
    uint32  MapID[16];                                      // 1-16     m_MapID
    uint32  InstanceType;                                   // 17       m_InstanceType          3 - BG, 4 - arena
    //uint32    m_GroupsAllowed;                            // 18       m_GroupsAllowed
    char* NameLang;                                         // 19       m_NameLang
    //uint32    m_MaxGroupSize;                             // 20       m_MaxGroupSize
    uint32 HolidayWorldState;                               // 21       m_HolidayWorldState
    uint32 Minlevel;                                        // 22       m_MinLevel
    uint32 Maxlevel;                                        // 23       m_MaxLevel
    uint32 RatedPlayers;                                    // 24       m_RatedPlayer
    //uint32    m_MinPlayer;                                // 25       m_MinPlayer
    //uint32    maxPlayers;                                 // 26       m_MaxPlayer
    //uint32    isRated;                                    // 27       m_Flags                 2 - Rated / 4 - Unk 6.0.1
    //uint32    m_IconFileDataID;                           // 28       m_IconFileDataID
    //char*     objectiveType;                              // 29       m_GametypeLang
    //uint32    Unk_601_18612;                              // 30       6.0.1 18612             Only Troll Raid 2 have data
};

#define MAX_OUTFIT_ITEMS 24

struct CharStartOutfitEntry
{
    //uint32 m_ID;                                          // 0        m_ID
    uint8  RaceID;                                          // 1
    uint8  ClassID;                                         // 2
    uint8  SexID;                                           // 3
    uint8  OutfitID;                                        // 4
    int32 ItemId[MAX_OUTFIT_ITEMS];                         // 5-28     m_itemID
    //uint32 m_petDisplayID;                                // 29       m_petDisplayID
    //uint32 m_petFamilyId;                                 // 30       m_petFamilyId
};

struct CharTitlesEntry
{
    uint32  ID;                                             // 0        m_ID                    Exemple in Quest::GetCharTitleId()
    //uint32    m_Condition_ID;                             // 1        m_Condition_ID
    char*   NameLang;                                       // 2        m_name_lang
    //char*     m_name1_lang;                               // 3        m_name1_lang
    uint32  MaskID;                                         // 4        m_mask_ID               Used in PLAYER_CHOSEN_TITLE and 1<<index in PLAYER_FIELD_KNOWN_TITLES
    //uint32    m_flags;                                    // 5        m_flags                 4 = temp title / 2 = arena tournament server only
};

struct ChatChannelsEntry
{
    uint32  ID;                                             // 0        m_ID
    uint32  Flags;                                          // 1        m_flags
    //uint32    m_factionGroup;                             // 2        m_factionGroup
    char*   NameLang;                                       // 3        m_name_lang
    //char*     m_shortcut_lang;                            // 4        m_shortcut_lang
};

struct ChrClassesEntry
{
    uint32  ClassID;                                        // 0        m_ID
    uint32  DisplayPower;                                   // 1        m_DisplayPower
    //char*     m_petNameToken;                             // 2        m_petNameToken
    char* NameLang;                                         // 3        m_name_lang
    //char*     m_name_female_lang;                         // 4        m_name_female_lang
    //char*     m_name_male_lang;                           // 5        m_name_male_lang
    //char*     m_filename;                                 // 6        m_filename
    uint32  SpellClassSet;                                  // 7        m_spellClassSet
    //uint32    flags;                                      // 8        m_flags                 (0x08 HasRelicSlot)
    uint32  CinematicSequenceID;                            // 9        m_cinematicSequenceID
    uint32  AttackPowerPerStrength;                         // 10       m_AttackPowerPerStrength        Attack Power bonus per point of strength
    uint32  AttackPowerPerAgility;                          // 11       m_AttackPowerPerAgility         Attack Power bonus per point of agility
    uint32  RangedAttackPowerPerAgility;                    // 12       m_RangedAttackPowerPerAgility   Ranged Attack Power bonus per point of agility
    //uint32    m_DefaultSpec;                              // 13       m_DefaultSpec
    //uint32    m_CreateScreenFileDataID;                   // 14       m_CreateScreenFileDataID
    //uint32    m_SelectScreenFileDataID;                   // 15       m_SelectScreenFileDataID
    //uint32    m_LowResScreenFileDataID;                   // 16       m_LowResScreenFileDataID
    //uint32    m_IconFileDataID;                           // 17       m_IconFileDataID
    uint32   MainStat;                                      // 18       m_MainStat
};

struct ChrRacesEntry
{
    uint32  RaceID;                                         // 0        m_ID
    //uint32    m_Flags;                                    // 1        m_Flags
    uint32  FactionID;                                      // 2        m_FactionID
    //uint32    m_ExplorationSoundID;                       // 3        m_ExplorationSoundID
    uint32  model_m;                                        // 4        m_MaleDisplayId
    uint32  model_f;                                        // 5        m_FemaleDisplayId
    //char*     m_ClientPrefix;                             // 6        m_ClientPrefix
    uint32  TeamID;                                         // 7        m_BaseLanguage          (7 Alliance, 1 Horde, 42 Neutral)
    //uint32    m_CreatureType;                             // 8        m_CreatureType
    //uint32    m_ResSicknessSpellID;                       // 9        m_ResSicknessSpellID
    //uint32    m_SplashSoundID;                            // 10       m_SplashSoundID
    //char*     m_ClientFileString;                         // 11       m_ClientFileString
    uint32  CinematicSequence;                              // 12       m_CinematicSequenceID
    //uint32    m_alliance;                                 // 13       m_alliance              (0 alliance, 1 horde, 2 disabled races)
    char*   name;                                           // 14       m_NameLang used for DBC language detection/selection
    //char*     m_NameFemaleLang;                           // 15       m_NameFemaleLang
    //char*     m_NameMaleLang;                             // 16       m_NameMaleLang
    //char*     m_FacialHairCustomization;                  // 17       m_FacialHairCustomization
    //char*     m_HairCustomization;                        // 18       m_HairCustomization
    //uint32    m_RaceRelated;                              // 19       m_RaceRelated
    //uint32    m_UnalteredVisualRaceID;                    // 20       m_UnalteredVisualRaceID
    //uint32    m_UaMaleCreatureSoundDataID;                // 21       m_UaMaleCreatureSoundDataID
    //uint32    m_UaFemaleCreatureSoundDataID;              // 22       m_UaFemaleCreatureSoundDataID
    //uint32    m_CharComponentTextureLayoutID;             // 23       m_CharComponentTextureLayoutID
    //uint32    m_DefaultClassID;                           // 24       m_DefaultClassID
    //uint32    m_CreateScreenFileDataID;                   // 25       m_CreateScreenFileDataID
    //uint32    m_SelectScreenFileDataID;                   // 26       m_SelectScreenFileDataID
    //float     m_MaleCustomizeOffset;                      // 27       m_MaleCustomizeOffset
    //float     m_MaleCustomizeOffset;                      // 28       m_MaleCustomizeOffset
    //float     m_MaleCustomizeOffset;                      // 29       m_MaleCustomizeOffset
    //float     m_FemaleCustomizeOffset;                    // 30       m_FemaleCustomizeOffset
    //float     m_FemaleCustomizeOffset;                    // 31       m_FemaleCustomizeOffset
    //float     m_FemaleCustomizeOffset;                    // 32       m_FemaleCustomizeOffset
    //uint32    m_NeutralRaceID;                            // 33       m_NeutralRaceID
    //uint32    m_LowResScreenFileDataID;                   // 34       m_LowResScreenFileDataID
    //uint32    m_HighResMaleDisplayId;                     // 35       m_HighResMaleDisplayId
    //uint32    m_HighResFemaleDisplayId;                   // 36       m_HighResFemaleDisplayId
    //uint32    m_CharComponentTexLayoutHiResID;            // 37       m_CharComponentTexLayoutHiResID
    //uint32    Unk_601_18612;                              // 38       6.0.1 18612 unk related to displayID HightRes for Worgen
};

// Removed in 6.0.1 18612
struct ChrPowerTypesEntry
{
   uint32   entry;                                          // 0
   uint32   classId;                                        // 1
   uint32   power;                                          // 2
};

struct ChrSpecializationsEntry
{
    uint32  ID;                                             // 0        m_ID
    char*   BackgroundFile;                                 // 1        m_BackgroundFile
    uint32  ClassID;                                        // 2        m_ClassID
    uint32  MasterySpellID;                                 // 3        m_MasterySpellID
    //uint32    m_MasterySpellID;                           // 4        m_MasterySpellID
    uint32  OrderIndex;                                     // 5        m_OrderIndex
    //uint32    m_PetTalentType;                            // 6        m_PetTalentType
    uint32  Role;                                           // 7        m_Role
    //uint32    m_SpellIconID                               // 8        m_SpellIconID
    //uint32    RaidBuffs;                                  // 9        m_RaidBuffs
    //uint32    Flags;                                      // 10       m_Flags
    char*   specializationName;                             // 11       m_NameLang
    //char*     m_NameLang                                  // 12       m_NameLang1
    char*   description;                                    // 13       m_DescriptionLang
    //uint32    m_MaxBuffs;                                 // 14       m_MaxBuffs
    uint32   MainStat;                                      // 15       m_MainStat
};

struct CinematicCameraEntry
{
    uint32      id;                                         // 0 index
    char*       filename;                                   // 1
    uint32      soundid;                                    // 2 in SoundEntries.dbc or 0
    float       EndPosX;                                    // 3
    float       EndPosY;                                    // 4
    float       EndPosZ;                                    // 5
    float       GlobalRotation;                             // 6
};

struct CinematicSequencesEntry
{
    uint32      Id;                                         // 0 index
    //uint32      unk1;                                     // 1 always 0
    uint32      cinematicCamera;                            // 2 id in CinematicCamera.dbc                                                        // 3-9 always 0
};

struct CreatureDisplayInfoEntry
{
    uint32  Displayid;                                      // 0        m_ID
    uint32  ModelId;                                        // 1        m_ModelID
    //uint32    m_sounID;                                   // 2        m_SoundID
    //uint32    m_extendedDisplayInfoID;                    // 3        m_ExtendedDisplayInfoID
    float   scale;                                          // 4        m_CreatureModelScale
    //uint32    m_creatureModelAlpha;                       // 5        m_CreatureModelAlpha
    //char*     m_textureName;                              // 6-8      m_TextureVariation[3]
    //char*     m_portraitTextureName ;                     // 9        m_PortraitTextureName
    //uint32    m_PortraitCreatureDisplayInfoID             // 10       m_PortraitCreatureDisplayInfoID
    //uint32    m_SizeClass;                                // 11       m_SizeClasss
    //uint32    m_BloodID;                                  // 12       m_BloodID
    //uint32    m_NPCSoundID;                               // 13       m_NPCSoundID
    //uint32    m_ParticleColorID;                          // 14       m_ParticleColorID
    //uint32    m_CreatureGeosetData;                       // 15       m_CreatureGeosetData
    //uint32    m_ObjectEffectPackageID;                    // 16       m_ObjectEffectPackageID
    //uint32    m_AnimReplacementSetID;                     // 17       m_AnimReplacementSetID
    //uint32    m_Flags;                                    // 18       m_Flags
    //int32     m_Gender;                                   // 19       m_Gender
    //uint32    m_StateSpellVisualKitID;                    // 20       m_StateSpellVisualKitID
};

struct CreatureFamilyEntry
{
    uint32  ID;                                             // 0        m_ID
    float   minScale;                                       // 1        m_MinScale
    uint32  minScaleLevel;                                  // 2        m_MinScaleLevel
    float   maxScale;                                       // 3        m_MaxScale
    uint32  maxScaleLevel;                                  // 4        m_MaxScaleLevel
    uint32  skillLine[2];                                   // 5-6      m_SkillLine
    uint32  petFoodMask;                                    // 7        m_PetFoodMask
    int32   petTalentType;                                  // 8        m_PetTalentType
    //uint32    m_categoryEnumID;                           // 9        m_CategoryEnumID
    char*   Name;                                           // 10       m_NameLang
    //char*     m_iconFile;                                 // 11       m_IconFile
};

struct CreatureModelDataEntry
{
    uint32  Id;                                             // 0        m_ID
    //uint32    m_Flags;                                    // 1        m_Flags
    //uint32    m_FileDataID;                               // 2        m_FileDataID
    //uint32    m_SizeClass;                                // 3        m_SizeClass
    //float     m_ModelScale;                               // 4        m_ModelScale
    //int32     m_BloodID;                                  // 5        m_BloodID
    //int32     m_FootprintTextureID;                       // 6        m_FootprintTextureID
    //float     m_FootprintTextureLength;                   // 7        m_FootprintTextureLength
    //float     m_FootprintTextureWidth;                    // 8        m_FootprintTextureWidth
    //float     m_FootprintParticleScale;                   // 9        m_FootprintParticleScale
    //uint32    m_FoleyMaterialID;                          // 10       m_FoleyMaterialID
    //uint32    m_FootstepShakeSize;                        // 11       m_FootstepShakeSize
    //uint32    m_DeathThudShakeSize;                       // 12       m_DeathThudShakeSize
    //uint32    m_SoundID;                                  // 13       m_SoundID
    //float     m_CollisionWidth;                           // 14       m_CollisionWidth
    float   CollisionHeight;                                // 15       m_CollisionHeight
    float   MountHeight;                                    // 16       m_MountHeight
    //float     m_GeoBoxMin[3];                             // 17-19    m_GeoBoxMin[3]
    //float     m_GeoBoxMax[3];                             // 20-22    m_GeoBoxMax[3]
    //float     m_WorldEffectScale;                         // 23       m_WorldEffectScale
    //float     m_AttachedEffectScale;                      // 24       m_AttachedEffectScale
    //float     m_MissileCollisionRadius;                   // 25       m_MissileCollisionRadius
    //float     m_MissileCollisionPush;                     // 26       m_MissileCollisionPush
    //float     m_MissileCollisionRaise;                    // 27       m_MissileCollisionRaise
    //float     m_OverrideLootEffectScale;                  // 28       m_OverrideLootEffectScale
    //float     m_OverrideNameScale;                        // 29       m_OverrideNameScale
    //float     m_OverrideSelectionRadius;                  // 30       m_OverrideSelectionRadius
    //float     m_TamedPetBaseScale;                        // 31       m_TamedPetBaseScale
    //uint32    m_CreatureGeosetDataID;                     // 32       m_CreatureGeosetDataID
    //float     m_HoverHeight;                              // 33       m_HoverHeight
};

#define MAX_CREATURE_SPELL_DATA_SLOT 4

struct CreatureSpellDataEntry
{
    uint32  ID;                                             // 0        m_ID
    uint32  spellId[MAX_CREATURE_SPELL_DATA_SLOT];          // 1-4      m_spells[4]
    //uint32    availability[MAX_CREATURE_SPELL_DATA_SLOT]; // 4-7      m_availability[4]
};

struct CreatureTypeEntry
{
    uint32  ID;                                             // 0        m_ID
    //char*     Name;                                       // 1        m_name_lang
    //uint32    no_experience;                              // 2        m_flags                 1 = no_xp
};

/* Unused see enum CurrencyCategory
struct CurrencyCategoryEntry
{
    uint32      ID;                                         // 0        m_ID
    uint32      m_Flags;                                    // 1        m_Flags
    char*       m_Name_lang;                                // 2        m_Name_lang

};
*/

// @issue : #284
struct CurrencyTypesEntry
{
    uint32  ID;                                             // 0        m_ID
    uint32  Category;                                       // 1        m_CategoryID
    //char*     m_NameLang;                                 // 2        m_NameLang
    //char*     m_InventoryIcon;                            // 3        m_InventoryIcon
    //char*     m_InventoryIcon2;                           // 4        m_InventoryIcon2
    //uint32    m_SpellWeight;                              // 5        m_SpellWeight
    //uint32    m_SpellCategory;                            // 6        m_SpellCategory
    uint32  TotalCap;                                       // 7        m_MaxQty
    uint32  WeekCap;                                        // 8        m_MaxEarnablePerWeek
    uint32  Flags;                                          // 9        m_Flags
    //uint32    m_Quality                                   // 10       m_Quality
    //char*     m_DescriptionLang;                          // 11       m_DescriptionLang

    bool HasPrecision() const   { return Flags & CURRENCY_FLAG_HIGH_PRECISION; }
    bool HasSeasonCount() const { return Flags & CURRENCY_FLAG_HAS_SEASON_COUNT; }
    float GetPrecision() const  { return HasPrecision() ? 100.0f : 1.0f; }
};

struct DestructibleModelDataEntry
{
    uint32  Id;                                             // 0        m_ID
    uint32  DamagedDisplayId;                               // 1        m_State0Wmo
    //uint32    m_State0ImpactEffectDoodadSet;              // 2        m_State0ImpactEffectDoodadSet
    //uint32    m_State0AmbientDoodadSet;                   // 3        m_State0AmbientDoodadSet
    //uint32    m_State0NameSet;                            // 4        m_State0NameSet
    uint32  DestroyedDisplayId;                             // 5        m_State1Wmo
    //uint32    m_State1DestructionDoodadSet;               // 6        m_State1DestructionDoodadSet
    //uint32    m_State1ImpactEffectDoodadSet;              // 7        m_State1ImpactEffectDoodadSet
    //uint32    m_State1AmbientDoodadSet;                   // 8        m_State1AmbientDoodadSet
    //uint32    m_State1NameSet;                            // 9        m_State1NameSet
    uint32  RebuildingDisplayId;                            // 10       m_State2Wmo
    //uint32    m_State2DestructionDoodadSet;               // 11       m_State2DestructionDoodadSet
    //uint32    m_State2ImpactEffectDoodadSet;              // 12       m_State2ImpactEffectDoodadSet
    //uint32    m_State2AmbientDoodadSet;                   // 13       m_State2AmbientDoodadSet
    //uint32    m_State2NameSet;                            // 14       m_State2NameSet
    uint32  SmokeDisplayId;                                 // 15       m_State3Wmo
    //uint32    m_State3InitDoodadSet;                      // 16       m_State3InitDoodadSet
    //uint32    m_State3AmbientDoodadSet;                   // 17       m_State3AmbientDoodadSet
    //uint32    m_State3NameSet;                            // 18       m_State3NameSet
    //uint32    m_EjectDirection;                           // 19       m_EjectDirection
    //uint32    m_RepairGroundFx;                           // 20       m_RepairGroundFx
    //uint32    m_DoNotHighlight;                           // 21       m_DoNotHighlight
    //uint32    m_HealEffect;                               // 22       m_HealEffect
    //uint32    m_HealEffectSpeed;                          // 23       m_HealEffectSpeed;
};

struct DifficultyEntry
{
    uint32 ID;
    uint32 FallbackDifficultyID;
    uint32 InstanceType;
    uint32 MinPlayers;
    uint32 MaxPlayers;
    uint32 OldEnumValue;
    uint32 Flags;
    uint32 ToggleDifficulty;
    uint32 GroupSizeHealthCurveID;
    uint32 GroupSizeDmgCurveID;
    uint32 GroupSizeSpellPointsCurveID;
    char*  NameLang;
    uint32 Unknow1;
};

struct DungeonEncounterEntry
{
    uint32  ID;                                             // 0        m_ID
    uint32  MapID;                                          // 1        m_MapID
    uint32  DifficultyID;                                   // 2        m_DifficultyID
    int32   OrderIndex;                                     // 3        m_orderIndex
    uint32  Bit;                                            // 4        m_Bit
    char*   NameLang;                                       // 5        m_name_lang
    uint32  CreatureDisplayID;                              // 6        m_CreatureDisplayID
    uint32  SpellIconID;                                    // 7        m_spellIconID
    uint32  Flags;                                          // 8        m_flags
};

struct DurabilityCostsEntry
{
    uint32  Itemlvl;                                        // 0        m_ID
    uint32  multiplier[29];                                 // 1-29     m_weaponSubclassCost1-21 and m_armorSubClassCost1-8
};

struct DurabilityQualityEntry
{
    uint32  Id;                                             // 0        m_ID
    float   quality_mod;                                    // 1        m_data
};

struct EmotesEntry
{
    uint32  Id;                                             // 0        m_ID
    //char*     EmoteSlashCommand;                          // 1        EmoteSlashCommand
    //uint32    m_AnimID;                                   // 2        m_AnimID
    uint32  Flags;                                          // 3        m_EmoteFlags
    uint32  EmoteType;                                      // 4        m_EmoteSpecProc
    uint32  UnitStandState;                                 // 5        m_EmoteSpecProcParam
    //uint32    m_EventSoundID;                             // 6        m_EventSoundID
    //uint32    SpellVisualKitID;                           // 7        m_SpellVisualKitID
};

struct EmotesTextEntry
{
    uint32  Id;                                             // 0        m_ID
    //char*     m_Name;                                     // 1        m_Name
    uint32  textid;                                         // 2        m_EmoteID
    //uint32    m_EmoteText[16];                            // 3-18     m_EmoteText
};

struct FactionEntry
{
    uint32  ID;                                             // 0        m_ID
    int32   ReputationIndex;                                // 1        m_ReputationIndex
    uint32  ReputationRaceMask[4];                          // 2-5      m_ReputationRaceMask
    uint32  ReputationClassMask[4];                         // 6-9      m_ReputationClassMask
    int32   ReputationBase[4];                              // 10-13    m_ReputationBase
    uint32  ReputationFlags[4];                             // 14-17    m_ReputationFlags
    uint32  ParentFactionID;                                // 18       m_ParentFactionID
    float   ParentFactionMod[2];                            // 19-20    m_ParentFactionMod      Faction gains incoming rep * spilloverRateIn
    uint32  ParentFactionCap[2];                            // 21-22    m_ParentFactionCap      The highest rank the faction will profit from incoming spillover
    char*   NameLang;                                       // 23       m_name_lang
    char*   DescriptionLang;                                // 24       m_description_lang
    uint32  Expansion;                                      // 25       m_Expansion
    uint32  Flags;                                          // 26       m_Flags
    uint32  FriendshipRepID;                                // 27       m_FriendshipRepID       Friendship system NYI

    // helpers
    bool CanHaveReputation() const
    {
        return ReputationIndex >= 0;
    }
};

#define MAX_FACTION_RELATIONS 4

struct FactionTemplateEntry
{
    uint32  ID;                                             // 0        m_ID
    uint32  Faction;                                        // 1        m_faction
    uint32  Flags;                                          // 2        m_flags
    uint32  FactionGroup;                                   // 3        m_factionGroup
    uint32  FriendGroup;                                    // 4        m_friendGroup
    uint32  EnemyGroup;                                     // 5        m_enemyGroup
    uint32  Enemies[MAX_FACTION_RELATIONS];                 // 6-9      m_enemies[MAX_FACTION_RELATIONS]
    uint32  Friend[MAX_FACTION_RELATIONS];                  // 10-14    m_friend[MAX_FACTION_RELATIONS]

    // helpers
    bool IsFriendlyTo(FactionTemplateEntry const& p_Entry) const
    {
        if (ID == p_Entry.ID)
            return true;

        if (p_Entry.Faction)
        {
            for (uint8 i = 0; i < MAX_FACTION_RELATIONS; ++i)
            {
                if (Enemies[i] == p_Entry.Faction)
                    return false;
            }

            for (uint8 i = 0; i < MAX_FACTION_RELATIONS; ++i)
            {
                if (Friend[i] == p_Entry.Faction)
                    return true;
            }
        }

        return (FriendGroup & p_Entry.FactionGroup) || (FactionGroup & p_Entry.FriendGroup);
    }

    bool IsHostileTo(FactionTemplateEntry const& p_Entry) const
    {
        if (ID == p_Entry.ID)
            return false;

        if (p_Entry.Faction)
        {
            for (uint8 i = 0; i < MAX_FACTION_RELATIONS; ++i)
            {
                if (Enemies[i] == p_Entry.Faction)
                    return true;
            }

            for (uint8 i = 0; i < MAX_FACTION_RELATIONS; ++i)
            {
                if (Friend[i] == p_Entry.Faction)
                    return false;
            }
        }

        return (EnemyGroup & p_Entry.FactionGroup) != 0;
    }

    bool IsHostileToPlayers() const { return (EnemyGroup & FACTION_MASK_PLAYER); }

    bool IsNeutralToAll() const
    {
        for (uint8 i = 0; i < MAX_FACTION_RELATIONS; ++i)
        {
            if (Enemies[i] != 0)
                return false;
        }

        return EnemyGroup == 0 && FriendGroup == 0;
    }

    bool IsContestedGuardFaction() const { return (Flags & FACTION_TEMPLATE_FLAG_CONTESTED_GUARD); }
};

struct GameObjectDisplayInfoEntry
{
    uint32  Displayid;                                      // 0        m_ID
    uint32   m_FileDataID;                                  // 1        m_FileDataID
    //uint32    m_Sound[10];                                // 2-11     m_Sound
    float   minX;                                           // 12       m_GeoBoxMin
    float   minY;                                           // 13       m_GeoBoxMin
    float   minZ;                                           // 14       m_GeoBoxMin
    float   maxX;                                           // 15       m_GeoBoxMax
    float   maxY;                                           // 16       m_GeoBoxMax
    float   maxZ;                                           // 17       m_GeoBoxMax
    //uint32    m_ObjectEffectPackageID;                    // 18       m_ObjectEffectPackageID
    //float     m_OverrideLootEffectScale;                  // 19       m_OverrideLootEffectScale
    //float     m_OverrideNameScale;                        // 20       m_OverrideNameScale
};

struct GemPropertiesEntry
{
    uint32      ID;                                         // 0        m_id
    uint32      spellitemenchantement;                      // 1        m_EnchantId
    //uint32    m_MaxcountInv                               // 2        m_MaxcountInv
    //uint32    m_MaxcountItem                              // 3        m_maxcount_item
    uint32      color;                                      // 4        m_type
    //uint32    m_MinItemLevel                              // 5        m_MinItemLevel
};

struct GlyphPropertiesEntry
{
    uint32  Id;                                             // 0        m_ID
    uint32  SpellId;                                        // 1        m_SpellID
    uint32  TypeFlags;                                      // 2        m_GlyphType
    uint32  m_SpellIconID;                                  // 3        m_SpellIconID
    uint32  GlyphExclusiveCategoryID;                       // 4        m_GlyphExclusiveCategoryID
};

// @author Selenium: 5.4 valid
struct GlyphSlotEntry
{
    uint32  Id;                                             // 0        m_ID
    uint32  TypeFlags;                                      // 1        m_Type
    uint32  Order;                                          // 2        m_Tooltip
};

// All Gt* DBC store data for 123 levels, some by 123 per class/race
#define GT_MAX_LEVEL    123
// gtOCTClassCombatRatingScalar.dbc stores data for 32 ratings, look at MAX_COMBAT_RATING for real used amount
#define GT_MAX_RATING   32

struct gtArmorMitigationByLvlEntry
{
    float    ratio;
};

struct GtBarberShopCostBaseEntry
{
    //uint32    level;
    float   cost;
};

struct GtCombatRatingsEntry
{
    //uint32    level;
    float   ratio;
};

struct GtChanceToMeleeCritBaseEntry
{
    //uint32    level;
    float   base;
};

struct GtChanceToMeleeCritEntry
{
    //uint32    level;
    float   ratio;
};

struct GtChanceToSpellCritBaseEntry
{
    //uint32    level;
    float   base;
};

struct GtChanceToSpellCritEntry
{
    //uint32    level;
    float   ratio;
};

struct GtOCTClassCombatRatingScalarEntry
{
    //uint32    level;
    float   ratio;
};

struct GtOCTLevelExperienceEntry
{
    uint32  Index;
    float   Data;
};

struct GtOCTHpPerStaminaEntry
{
    //uint32    level;
    float   ratio;
};

struct GtRegenHPPerSptEntry
{
    //uint32    level;
    float   ratio;
};

struct GtRegenMPPerSptEntry
{
    //uint32    level;
    float   ratio;
};

struct GtSpellScalingEntry
{
    //uint32    level;
    float   value;
};

struct GtOCTBaseHPByClassEntry
{
    //uint32    level;
    float   ratio;
};

struct GtOCTBaseMPByClassEntry
{
    //uint32    level;
    float   ratio;
};

struct GuildPerkSpellsEntry
{
    //uint32    Id;                                         // 0        m_ID
    uint32  Level;                                          // 1        m_GuildLevel
    uint32  SpellId;                                        // 2        m_SpellID
};

struct ImportPriceArmorEntry
{
    uint32  InventoryType;                                  // 0        m_ID
    float   ClothFactor;                                    // 1        m_ClothModifier
    float   LeatherFactor;                                  // 2        m_LeatherModifier
    float   MailFactor;                                     // 3        m_ChainModifier
    float   PlateFactor;                                    // 4        m_PlateModifier
};

struct ImportPriceQualityEntry
{
    uint32  QualityId;                                      // 0        m_ID
    float   Factor;                                         // 1        m_data
};

struct ImportPriceShieldEntry
{
    uint32  Id;                                             // 0        m_ID
    float   Factor;                                         // 1        m_data
};

struct ImportPriceWeaponEntry
{
    uint32  Id;                                             // 0        m_ID
    float   Factor;                                         // 1        m_data
};

struct ItemSetSpellEntry
{
    uint32 ItemSetID;
    uint32 SpellID;
    uint32 PieceRequirement;
    uint32 SpecializationID;
};

struct ItemPriceBaseEntry
{
    //uint32    ID;                                         // 0        m_ID
    uint32  ItemLevel;                                      // 1        m_ItemLevel
    float   ArmorFactor;                                    // 2        m_Armor
    float   WeaponFactor;                                   // 3        m_Weapon
};


// common struct for:
// ItemDamageAmmo.dbc
// ItemDamageOneHand.dbc
// ItemDamageOneHandCaster.dbc
// ItemDamageRanged.dbc
// ItemDamageThrown.dbc
// ItemDamageTwoHand.dbc
// ItemDamageTwoHandCaster.dbc
// ItemDamageWand.dbc
struct ItemDamageEntry
{
    uint32  Id;                                             // 0        m_ID
    float   DPS[7];                                         // 1-7      m_Qualitymod
    uint32  Id2;                                            // 8        m_ItemLevel
};

struct ItemArmorQualityEntry
{
    uint32  Id;                                             // 0        m_ID
    float   Value[7];                                       // 1-7      m_Qualitymod
    uint32  Id2;                                            // 8        m_ItemLevel
};

struct ItemArmorShieldEntry
{
    uint32  Id;                                             // 0        m_ID
    uint32  Id2;                                            // 1        m_ItemLevel
    float   Value[7];                                       // 2-8      m_Qualitymod
};

struct ItemArmorTotalEntry
{
    uint32  Id;                                             // 0        m_ID
    uint32  Id2;                                            // 1        m_ItemLevel
    float   Value[4];                                       // 2-5      m_Qualitymod
};

struct ItemClassEntry
{
    uint32  Class;                                          // 0        m_ClassID
    //uint32    m_Flags;                                    // 1        m_Flags
    float   PriceFactor;                                    // 2        m_PriceModifier
  //char*       m_ClassNameLang;                            // 3        m_ClassNameLang
};

struct ItemBagFamilyEntry
{
    uint32  ID;                                             // 0        m_ID
    //char*     m_NameLang;                                 // 1        m_NameLang
};

struct gtItemSocketCostPerLevelEntry
{
    float cost;
};

// we have no data in 6.0.1 18612
struct ItemDisplayInfoEntry
{
    uint32  ID;                                             // 0        m_ID
    //char*                                                 // ?
    //uint32                                                // ?
    //uint32                                                // ?
    //uint32                                                // ?
    //uint32                                                // ?
    //uint32                                                // ?
    //uint32                                                // ?
    //uint32                                                // ?
    //uint32                                                // ?
    //uint32                                                // ?
    //uint32                                                // ?
    //uint32                                                // 22
};

struct ItemDisenchantLootEntry
{
    uint32  Id;                                             // 0        m_ID
    uint32  ItemClass;                                      // 1        m_Class
    int32   ItemSubClass;                                   // 2        m_Subclass
    uint32  ItemQuality;                                    // 3        m_Quality
    uint32  MinItemLevel;                                   // 4        m_MinLevel
    uint32  MaxItemLevel;                                   // 5        m_MaxLevel
    uint32  RequiredDisenchantSkill;                        // 6        m_SkillRequired
};

struct ItemLimitCategoryEntry
{
    uint32  ID;                                             // 0 Id
    //char*     name;                                       // 1        m_name_lang
    uint32  maxCount;                                       // 2,       m_quantity max allowed equipped as item or in gem slot
    uint32  mode;                                           // 3,       m_flags 0 = have, 1 = equip (enum ItemLimitCategoryMode)
};

#define MAX_ITEM_ENCHANTMENT_EFFECTS 5

struct ItemRandomPropertiesEntry
{
    uint32  ID;                                             // 0        m_ID
    //char*     internalName                                // 1        m_Name
    uint32  enchant_id[MAX_ITEM_ENCHANTMENT_EFFECTS];       // 2-6      m_Enchantment
    char*   nameSuffix;                                     // 7        m_name_lang
};

struct ItemRandomSuffixEntry
{
    uint32  ID;                                             // 0        m_ID
    char*   nameSuffix;                                     // 1        m_name_lang
    //char*     internalName;                               // 2        m_internalName
    uint32  enchant_id[5];                                  // 3-7      m_enchantment
    uint32  prefix[5];                                      // 8-12     m_allocationPct
};

#define MAX_ITEM_SET_ITEMS 17
#define MAX_ITEM_SET_SPELLS 8

struct ItemSetEntry
{
    //uint32    id                                          // 0        m_ID
    char*   name;                                           // 1        m_name_lang
    uint32  itemId[MAX_ITEM_SET_ITEMS];                     // 2-18     m_itemID
    uint32  required_skill_id;                              // 19       m_requiredSkill
    uint32  required_skill_value;                           // 20       m_requiredSkillRank
};

struct ItemSpecEntry
{
    uint32 ID;
    uint32 MinLevel;
    uint32 MaxLevel;
    uint32 ItemType;
    uint32 PrimaryStat;
    uint32 SecondaryStat;
    uint32 SpecializationID;
};

struct ItemSpecOverrideEntry
{
    uint32  ID;                                             // 1        m_ID
    uint32  itemEntry;                                      // 2        m_IemID
    uint32  specID;                                         // 3        m_SpecID
};

struct LFGDungeonEntry
{
    uint32  ID;                                             // 0        m_ID
    //char*     m_NameLang;                                 // 1        m_NameLang
    uint32  minlevel;                                       // 2        m_MinLevel
    uint32  maxlevel;                                       // 3        m_MaxLevel
    uint32  reclevel;                                       // 4        m_TargetLevel
    uint32  recminlevel;                                    // 5        m_TargetLevelMin
    uint32  recmaxlevel;                                    // 6        m_TargetLevelMax
    int32   map;                                             // 7       m_MapID
    uint32  difficulty;                                     // 8        m_DifficultyID
    uint32  flags;                                          // 9        m_Flags
    uint32  type;                                           // 10       m_TypeID
    //int32     m_Faction;                                  // 11       m_Faction
    //char*     m_TextureFilename;                          // 12       m_TextureFilename
    uint32  expansion;                                      // 13       m_ExpansionLevel
    //uint32    m_OrderIndex;                               // 14       m_OrderIndex
    uint32  grouptype;                                      // 15       m_GroupId
    //char*     m_DescriptionLang;                          // 16       m_DescriptionLang
    uint32  flags2;                                         // 17       m_RandomId
    uint32  tankNeeded;                                     // 18       m_CountTank
    uint32  healerNeeded;                                   // 19       m_CountHealer
    uint32  dpsNeeded;                                      // 20       m_CountDamage
    //uint32    m_MinCountTank;                             // 21       m_MinCountTank
    //uint32    m_MinCountHealer;                           // 22       m_MinCountHealer
    //uint32    m_MinCountDamage;                           // 23       m_MinCountDamage
    //uint32    m_ScenarioID;                               // 24       m_ScenarioID
    uint32  category;                                       // 25       m_Subtype
    //uint32    m_BonusReputationAmount;                    // 26       m_BonusReputationAmount
    //uint32    m_MentorCharLevel;                          // 27       m_MentorCharLevel
    //uint32    m_MentorItemLevel;                          // 28       m_MentorItemLevel

    // Helpers
    uint32 Entry() const { return ID + (type << 24); }
    // 1 = LFG_TYPE_DUNGEON
    bool isScenario() const { return type == 1 && tankNeeded == 0 && healerNeeded == 0 && dpsNeeded == 3; }
};

// delete ? no data for 6.0.1 18612
struct LiquidTypeEntry
{
    uint32  Id;                                             // 1
    //char*     Name;                                       // 2
    //uint32    Flags;                                      // 3
    uint32  Type;                                            // 4
    //uint32    SoundId;                                    // 5
    uint32  SpellId;                                        // 6
    //float     MaxDarkenDepth;                             // 7
    //float     FogDarkenIntensity;                         // 8
    //float     AmbDarkenIntensity;                         // 9
    //float     DirDarkenIntensity;                         // 10
    //uint32    LightID;                                    // 11
    //float     ParticleScale;                              // 12
    //uint32    ParticleMovement;                           // 13
    //uint32    ParticleTexSlots;                           // 14
    //uint32    LiquidMaterialID;                           // 15
    //char*     Texture[6];                                 // 16-22
    //uint32    Color[2];                                   // 23-24
    //float     Unk1[18];                                   // 25-42
    //uint32    Unk2[4];                                    // 43-46
    //uint32
    //uint32
    //uint32
    //uint32
    //uint32
    //uint32                                                // 50
};

#define MAX_LOCK_CASE 8

struct LockEntry
{
    uint32  ID;                                             // 0        m_ID
    uint32  Type[MAX_LOCK_CASE];                            // 1-8      m_Type
    uint32  Index[MAX_LOCK_CASE];                           // 9-16     m_Index
    uint32  Skill[MAX_LOCK_CASE];                           // 17-24    m_Skill
    //uint32    Action[MAX_LOCK_CASE];                      // 25-32    m_Action
};

struct PhaseEntry
{
    uint32  ID;                                             // 0        m_ID
    uint32  flag;                                           // 1        m_Flags
};

struct MailTemplateEntry
{
    uint32  ID;                                             // 0        m_ID
    //char*     subject;                                    // 1        m_subject_lang
    char*   content;                                        // 2        m_body_lang
};

struct MapEntry
{
    uint32  MapID;                                          // 0        m_ID
    //char*     direcotry;                                  // 1        m_Directory
    uint32  instanceType;                                   // 2        m_InstanceType
    //uint32    flags;                                      // 3        m_Flags
    //int     unk_602;                                      // 4
    //uint32    MapType;                                    // 5        m_MapType 4 Garison, 3 Transport 2/1 unk usage
    char*   MapNameLang;                                    // 6        m_MapName_lang
    uint32  AreaTableID;                                    // 7        m_areaTableID
    //char*     MapDescription0Lang;                        // 8        m_MapDescription0_lang
    //char*     MapDescription1Lang;                        // 9        m_MapDescription1_lang
    uint32  LoadingScreenID;                                // 10       m_LoadingScreenID
    //float     MinimapIconScale;                           // 11       m_minimapIconScale
    int32   CorpseMapID;                                    // 12       m_corpseMapID           map_id of entrance map in ghost mode (continent always and in most cases = normal entrance)
    float   CorpseX;                                        // 13       m_corpseX               entrance x coordinate in ghost mode  (in most cases = normal entrance)
    float   CorpseY;                                        // 14       m_corpseY               entrance y coordinate in ghost mode  (in most cases = normal entrance)
    //uint32    TimeOfDayOverride;                          // 15       m_timeOfDayOverride
    uint32  ExpansionID;                                    // 16       m_expansionID
    uint32  RaidOffset;                                     // 17       m_raidOffset
    uint32  MaxPlayers;                                     // 18       m_maxPlayers
    int32   ParentMapID;                                    // 19       m_ParentMapID
    //int32     CosmeticParentMapID;                        // 20       m_CosmeticParentMapID
    //uint32    TimeOffset;                                 // 21       m_TimeOffset

    // Helpers
    uint32 Expansion() const { return ExpansionID; }

    bool IsDungeon()                const { return instanceType == MAP_INSTANCE || instanceType == MAP_RAID; }
    bool IsNonRaidDungeon()         const { return instanceType == MAP_INSTANCE; }
    bool Instanceable()             const { return instanceType == MAP_INSTANCE || instanceType == MAP_RAID || instanceType == MAP_BATTLEGROUND || instanceType == MAP_ARENA || instanceType == MAP_SCENARIO; }
    bool IsRaid()                   const { return instanceType == MAP_RAID; }
    bool IsBattleground()           const { return instanceType == MAP_BATTLEGROUND; }
    bool IsBattleArena()            const { return instanceType == MAP_ARENA; }
    bool IsScenario()               const { return instanceType == MAP_SCENARIO; }
    bool IsBattlegroundOrArena()    const { return instanceType == MAP_BATTLEGROUND || instanceType == MAP_ARENA; }
    bool IsWorldMap()               const { return instanceType == MAP_COMMON; }

    bool GetEntrancePos(int32 &mapid, float &x, float &y) const
    {
        if (CorpseMapID < 0)
            return false;
        mapid = CorpseMapID;
        x = CorpseX;
        y = CorpseY;
        return true;
    }

    // @todo : use m_ContinentID from AreTable.dbc used for SPELL_ATTR4_CAST_ONLY_IN_OUTLAND issue #281
    bool IsContinent() const
    {
        return MapID == 0 || MapID == 1 || MapID == 530 || MapID == 571 || MapID == 860 || MapID == 870 || MapID == 1116;
    }
};

struct MapDifficultyEntry
{
    //uint32    Id;                                         // 0        m_ID
    uint32  MapId;                                          // 1        m_MapID
    uint32  Difficulty;                                     // 2        m_DifficultyID          (for arenas: arena slot)
    char*   areaTriggerText;                                // 3        m_message_lang          (text showed when transfer to map failed)
    uint32  resetTime;                                      // 4        m_raidDuration          in secs, 0 if no fixed reset time
    uint32  maxPlayers;                                     // 5        m_maxPlayers            some heroic versions have 0 when expected same amount as in normal version
    //uint32    LockID;                                     // 6        m_LockID
    //uint32    Unk_601_18612;                              // 7        Unk_601_18612           m_message_lang for WOD
};

struct MinorTalentEntry
{
    // uint32 ID;               // 0
    uint32 specializationID;    // 1
    uint32 spellID;             // 2
    uint32 orderIndex;          // 3
};

struct MountCapabilityEntry
{
    uint32  Id;                                             // 0        m_ID
    uint32  Flags;                                          // 1        m_Flags
    uint32  RequiredRidingSkill;                            // 2        m_ReqRidingSkill
    uint32  RequiredArea;                                   // 3        m_ReqAreaID
    uint32  RequiredAura;                                   // 4        m_ReqSpellAuraID
    uint32  RequiredSpell;                                  // 5        m_ReqSpellKnownID
    uint32  SpeedModSpell;                                  // 6        m_ModSpellAuraID
    int32   RequiredMap;                                    // 7        m_ReqMapID
};

#define MAX_MOUNT_CAPABILITIES 24

struct MountTypeEntry
{
    uint32  Id;                                             // 0        m_ID
    uint32  MountCapability[MAX_MOUNT_CAPABILITIES];        // 1        m_Capability
};

struct MovieEntry
{
    uint32  Id;                                             // 0        m_ID
    //uint32    Volume;                                     // 1        m_Volume
    //uint32    KeyID;                                      // 2        m_KeyID
    //uint32    AudioFileDataID;                            // 3        m_AudioFileDataID
    //uint32    unk3m_SubtitleFileDataID;                   // 4        m_SubtitleFileDataID
};

struct NameGenEntry
{
    //uint32    id;                                         // 0        m_ID
    char*   name;                                           // 1        m_Name
    uint32  race;                                           // 2        m_RaceID
    uint32  gender;                                         // 3        m_Sex
};

struct PvPDifficultyEntry
{
    //uint32    id;                                         // 0        m_ID
    uint32  mapId;                                          // 1        m_MapID
    uint32  bracketId;                                      // 2        m_RangeIndex
    uint32  minLevel;                                       // 3        m_MinLevel
    uint32  maxLevel;                                       // 4        m_MaxLevel

    // helpers
    BattlegroundBracketId GetBracketId() const { return BattlegroundBracketId(bracketId); }
};

struct QuestSortEntry
{
    uint32      id;                                         // 0        m_ID
    //char*       name;                                     // 1        m_SortName_lang
};

struct QuestXPEntry
{
    uint32  id;                                             // 0        m_ID
    uint32  Exp[10];                                        // 1 -9     m_Difficulty
};

struct QuestFactionRewEntry
{
    uint32  id;                                             // 0        m_ID
    int32   QuestRewFactionValue[10];                       // 1 -9     m_Difficulty
};

struct QuestPOIPointEntry
{
    uint32  unk;                                            // 0        m_ID
    int32   x;                                              // 1        m_X
    int32   y;                                              // 2        m_Y
    uint32  ID;                                             // 3        m_QuestPOIBlobID
};

struct RandomPropertiesPointsEntry
{
    uint32  Id;                                             // 0
    uint32  EpicPropertiesPoints[5];                        // 1-5
    uint32  RarePropertiesPoints[5];                        // 6-10
    uint32  UncommonPropertiesPoints[5];                    // 11-15
};

struct ResearchBranchEntry
{
    uint32  ID;                                             // 0        m_ID
    //char*     m_Name;                                     // 1        m_NameLang
    //uint32    m_ResearchFieldID;                          // 2        m_ResearchFieldID
    uint32  CurrencyID;                                     // 3        m_CurrencyID
    //char*     m_Texture;                                  // 4        m_Texture
    uint32  ItemID;                                         // 5        m_ItemID
};

struct ResearchProjectEntry
{
    uint32  ID;                                             // 0        m_ID
    //char*     m_ID;                                       // 1        m_NameLang
    //char*     m_NameLang;                                 // 2        m_DescriptionLang
    uint32  rare;                                           // 3        m_Rarity
    uint32  branchId;                                       // 4        m_ResearchBranchID
    uint32  spellId;                                        // 5        m_SpellID
    //uint32    m_NumSockets;                               // 6        m_NumSockets
    //char*     m_Texture;                                  // 7        m_Texture
    uint32  req_currency;                                   // 8        m_RequiredWeight

};

struct ResearchSiteEntry
{
    uint32  ID;                                             // 0        m_ID
    uint32  mapId;                                          // 1        m_MapID
    uint32  POIid;                                          // 2        m_QuestPOIBlobID
    //char*     m_NameLang;                                 // 3        m_NameLang
    //uint32    AreaPOIIconEnum;                            // 4        m_AreaPOIIconEnum
};

struct ScalingStatDistributionEntry
{
    uint32  Id;                                             // 0        m_ID
    uint32  MinLevel;                                       // 1        m_Minlevel
    uint32  MaxLevel;                                       // 2        m_Maxlevel
    uint32  CurveProperties;                                // 3        m_CurveProperties
};

// 6.0.2 19027 - DBC doesn't exists
struct ScalingStatValuesEntry
{
    uint32 Id;                                              // 0        m_ID
    uint32 Level;                                           // 1        m_Charlevel
    uint32 dpsMod[7];                                       // 2-8      m_EffectiveLevel + m_WeaponDPS1H + m_WeaponDPS2H + m_SpellcasterDPS1H + m_SpellcasterDPS2H + m_RangedDPS + m_WandDPS
    uint32 Spellpower;                                      // 9        m_SpellPower
    uint32 StatMultiplier[5];                               // 10-14    m_BudgetPrimary + m_BudgetSecondary + m_BudgetTertiary + m_BudgetSub + m_BudgetTrivial
    uint32 Armor[8][4];                                     // 15-47    m_ArmorShoulder[4] + m_ArmorChest[4] + m_ArmorHead[4] + m_ArmorLegs[4] + m_ArmorFeet[4] + m_ArmorWaist[4] + m_ArmorHands[4] + m_ArmorWrists[4]
    uint32 CloakArmor;                                      // 48       m_ArmorBack
    //uint32 m_ArmorShield;                                 // 49       m_ArmorShield

    uint32 GetStatMultiplier(uint32 inventoryType) const;
    uint32 GetArmor(uint32 inventoryType, uint32 armorType) const;
    uint32 GetDPSAndDamageMultiplier(uint32 subClass, bool isCasterWeapon, float* damageMultiplier) const;
};

struct ScenarioEntry
{
    uint32 ID;
    char*  NameLang;
    uint32 Flags;
};

struct ScenarioStepEntry
{
    uint32 ID;
    uint32 CriteriaTreeID;
    uint32 ScenarioID;
    uint32 OrderIndex;
    char*  DescriptionLang;
    char*  TitleLang;
    uint32 Flags;
    uint32 RelatedStep;
    uint32 Supersedes;
    uint32 RewardQuestID;
};

// used ?
//struct SkillRaceClassInfoEntry
//{
//    uint32    id;                                         // 0        m_ID
//    uint32    skillId;                                    // 1        m_skillID
//    uint32    raceMask;                                   // 2        m_raceMask
//    uint32    classMask;                                  // 3        m_classMask
//    uint32    flags;                                      // 4        m_flags
//    uint32    reqLevel;                                   // 5        m_minLevel
//    uint32    skillTierId;                                // 6        m_skillTierID
//    uint32    skillCostID;                                // 7        m_skillCostIndex
//};

//struct SkillTiersEntry
//{
//    uint32    id;                                         // 0        m_ID
//    uint32    skillValue[16];                             // 1-17     m_cost
//    uint32    maxSkillValue[16];                          // 18-32    m_valueMax
//};

struct SkillLineEntry
{
    uint32  id;                                             // 0        m_ID
    int32   categoryId;                                     // 1        m_categoryID
    char*   name;                                           // 2        m_displayName_lang
    //char*     description;                                // 3        m_description_lang
    uint32  spellIcon;                                      // 4        m_spellIconID
    //char*     alternateVerb;                              // 5        m_alternateVerb_lang
    uint32  canLink;                                        // 6        m_canLink (prof. with recipes)
    //uint32    m_ParentSkillLineID;                        // 7        m_ParentSkillLineID
    uint32  m_Flags;                                        // 8        m_Flags
};

struct SkillLineAbilityEntry
{
    uint32  id;                                             // 0        m_ID
    uint32  skillId;                                        // 1        m_skillLine
    uint32  spellId;                                        // 2        m_spell
    uint32  racemask;                                       // 3        m_raceMask
    uint32  classmask;                                      // 4        m_classMask
    uint32  req_skill_value;                                // 5        m_minSkillLineRank
    uint32  forward_spellid;                                // 6        m_supercededBySpell
    uint32  learnOnGetSkill;                                // 7        m_acquireMethod
    uint32  max_value;                                      // 8        m_trivialSkillLineRankHigh
    uint32  min_value;                                      // 9        m_trivialSkillLineRankLow
    uint32  skill_gain;                                     // 10       m_NumSkillUps
    //uint32    m_UniqueBit;                                // 11       m_UniqueBit
    //uint32    m_TradeSkillCategoryID;                     // 12       m_TradeSkillCategoryID
};

struct SoundEntriesEntry
{
    uint32  Id;                                             // 0        m_ID
    //uint32    Type;                                       // 1        m_SoundType
    //char*     InternalName;                               // 2        m_Name
    //uint32    m_FileDataID[20];                           // 3-22     m_FileDataID
    //uint32    m_Freq[20];                                 // 23-42    m_Freq
    //float     m_volumeFloat;                              // 43       m_VolumeFloat
    //uint32    m_Flags;                                    // 44       m_Flags
    //float     m_MinDistance;                              // 45       m_MinDistance
    //float     m_DistanceCutoff;                           // 46       m_DistanceCutoff
    //uint32    m_EAXDef;                                   // 47       m_EAXDef
    //uint32    m_SoundEntriesAdvancedID;                   // 48       m_SoundEntriesAdvancedID
    //float     m_Volumevariationplus;                      // 49       m_Volumevariationplus
    //float     m_Volumevariationminus;                     // 50       m_Volumevariationminus
    //float     m_Pitchvariationplus;                       // 51       m_Pitchvariationplus
    //float     m_Pitchvariationminus;                      // 52       m_Pitchvariationminus
    //float     m_PitchAdjust;                              // 53       m_PitchAdjust
    //uint32    m_Dialogtype;                               // 54       m_Dialogtype
    //uint32    m_BusOverwriteID;                           // 55       m_BusOverwriteID
};

struct SpecializationSpellEntry
{
    uint32  Id;                                             // 0        m_ID
    uint32  SpecializationEntry;                            // 1        m_SpecID
    uint32  LearnSpell;                                     // 2        m_SpellID
    uint32  OverrideSpell;                                  // 3        m_OverridesSpellID
    //char*     m_DescriptionLang;                          // 4        m_DescriptionLang       No data
};

struct SpellEffectEntry
{
    uint32  Id;                                             // 0        m_ID
    uint32  EffectDifficulty;                               // 1        m_EffectDifficulty
    uint32  Effect;                                         // 2        m_Effect
    float   EffectValueMultiplier;                          // 3        m_EffectAmplitude
    uint32  EffectApplyAuraName;                            // 4        m_EffectAura
    uint32  EffectAmplitude;                                // 5        m_EffectAuraPeriod
    int32   EffectBasePoints;                               // 6        m_EffectBasePoints     Don't must be used in spell/auras explicitly, must be used cached Spell::m_currentBasePoints
    float   EffectBonusMultiplier;                          // 7        m_EffectBonusCoefficient
    float   EffectDamageMultiplier;                         // 8        m_EffectChainAmplitude
    uint32  EffectChainTarget;                              // 9        m_EffectChainTargets
    int32   EffectDieSides;                                 // 10       m_EffectDieSides
    uint32  EffectItemType;                                 // 11       m_EffectItemType
    uint32  EffectMechanic;                                 // 12       m_EffectMechanic
    int32   EffectMiscValue;                                // 13       m_EffectMiscValue
    int32   EffectMiscValueB;                               // 14       m_EffectMiscValueB
    float   EffectPointsPerComboPoint;                      // 15       m_EffectPointsPerResource
    uint32  EffectRadiusIndex;                              // 16       m_EffectRadiusIndex     Spellradius.dbc
    uint32  EffectRadiusMaxIndex;                           // 17       m_EffectRadiusMaxIndex
    float   EffectRealPointsPerLevel;                       // 18       m_EffectRealPointsPerLevel
    flag128 EffectSpellClassMask;                           // 19-22    m_effectSpellClassMask1 (2/3), effect 0
    uint32  EffectTriggerSpell;                             // 23       m_effectTriggerSpell
    //float     m_EffectPosFacing;                          // 24       m_EffectPosFacing
    uint32  EffectImplicitTargetA;                          // 25       m_implicitTargetA
    uint32  EffectImplicitTargetB;                          // 26       m_implicitTargetB
    uint32  EffectSpellId;                                  // 27       m_SpellID
    uint32  EffectIndex;                                    // 28       m_EffectIndex
    //uint32    m_EffectAttributes;                         // 29       m_EffectAttributes
    float   BonusCoefficientFromAP;                         // 30       m_BonusCoefficientFromAP
};

struct SpellEffectScalingEntry
{
    //uint32    m_ID;                                       // 0        m_ID
    float   Multiplier;                                     // 1        m_Coefficient
    float   RandomMultiplier;                               // 2        m_Variance
    float   OtherMultiplier;                                // 3        m_ResourceCoefficient
    uint32  Id;                                             // 4        m_SpellEffectID
};

#define MAX_SPELL_EFFECTS 32
#define MAX_EFFECT_MASK 4294967295

struct SpellAuraOptionsEntry
{
    uint32  Id;                                             // 0        m_ID
    uint32    m_SpellID;                                    // 1        m_SpellID
    uint32    m_DifficultyID;                               // 2        m_DifficultyID
    uint32    StackAmount;                                  // 1        m_cumulativeAura
    uint32    procChance;                                   // 2        m_procChance
    uint32    procCharges;                                  // 3        m_procCharges
    uint32    procFlags;                                    // 4        m_procTypeMask
    uint32    InternalCooldown;                             // 7       m_internalCooldown
    uint32    ProcsPerMinuteEntry;                          // 8       m_procsPerMinuteEntry
};

struct SpellEntry
{
    uint32  Id;                                             // 0        m_ID
    char*   SpellName;                                      // 1        m_name_lang
    char*   Rank;                                           // 2        m_nameSubtext_lang
    //char*     Description;                                // 3        m_description_lang
    //char*     ToolTip;                                    // 4        m_auraDescription_lang
    uint32  runeCostID;                                     // 5        m_runeCostID
    //uint32    spellMissileID;                             // 6        m_spellMissileID
    //uint32    spellDescriptionVariableID;                 // 7        m_spellDescriptionVariableID
    uint32  SpellScalingId;                                 // 8        m_ScalingID
    uint32  SpellAuraOptionsId;                             // 9        m_AuraOptionsID
    uint32  SpellAuraRestrictionsId;                        // 10       m_AuraRestrictionsID
    uint32  SpellCastingRequirementsId;                     // 11       m_CastingRequirementsID
    uint32  SpellCategoriesId;                              // 12       m_CategoriesID
    uint32  SpellClassOptionsId;                            // 13       m_ClassOptionsID
    uint32  SpellCooldownsId;                               // 14       m_CooldownsID
    uint32  SpellEquippedItemsId;                           // 15       m_EquippedItemsID
    uint32  SpellInterruptsId;                              // 16       m_InterruptsID
    uint32  SpellLevelsId;                                  // 17       m_LevelsID
    uint32  SpellReagentsId;                                // 18       m_ReagentsID
    uint32  SpellShapeshiftId;                              // 19       m_ShapeshiftID
    uint32  SpellTargetRestrictionsId;                      // 20       m_TargetRestrictionsID
    uint32  SpellTotemsId;                                  // 21       m_TotemsID
    uint32  ResearchProject;                                // 22       m_RequiredProjectID
    uint32  SpellMiscId;                                    // 23       m_MiscID

    // struct access functions
    SpellEffectEntry const* GetSpellEffect(uint32 eff, uint32 difficulty) const;
};

struct SpellCategoriesEntry
{
    uint32  Id;                                             // 0        m_ID
    uint32  SpellId;                                        // 1        m_spellId
    uint32  m_DifficultyID;                                 // 2        m_DifficultyID
    uint32  Category;                                       // 3        m_category
    uint32  DmgClass;                                       // 4        m_defenseType
    uint32  Dispel;                                         // 5        m_dispelType
    uint32  Mechanic;                                       // 6        m_mechanic
    uint32  PreventionType;                                 // 7        m_preventionType
    uint32  StartRecoveryCategory;                          // 8        m_startRecoveryCategory
    uint32  ChargesCategory;                                // 9        m_chargesCategory
};

// SpellCategory.dbc
struct SpellCategoryEntry
{
    uint32  Id;                                             // 0        m_ID
    uint32  Flags;                                          // 1        m_Flags
    uint32  m_UsesPerWeek;                                  // 2        m_UsesPerWeek           (byte)
    char*   Description;                                    // 3        m_NameLang
    uint32  MaxCharges;                                     // 4        m_MaxCharges
    uint32  ChargeRegenTime;                                // 5        m_ChargeRecoveryTime
};

typedef std::set<uint32> SpellCategorySet;
typedef std::map<uint32, SpellCategorySet > SpellCategoryStore;
typedef std::list<const SpellEntry*> SpellSkillingList;
typedef std::set<uint32> PetFamilySpellsSet;
typedef std::map<uint32, PetFamilySpellsSet > PetFamilySpellsStore;

struct SpellCastTimesEntry
{
    uint32  ID;                                             // 0        m_ID
    int32   CastTime;                                       // 1        m_Base
    //float     m_PerLevel;                                 // 2        m_PerLevel
    //int32     m_Minimum;                                  // 3        m_Minimum
};

struct SpellFocusObjectEntry
{
    uint32  ID;                                             // 0        m_ID
    //char*     Name;                                       // 1        m_NameLang
};

struct SpellRadiusEntry
{
    uint32  ID;                                             // 0        m_ID
    float   radiusHostile;                                  // 1        m_Radius
    //float     m_RadiusPerLevel;                           // 2        m_RadiusPerLevel
    float   radiusFriend;                                   // 3        m_RadiusMin
    //float     RadiusMaxm_                                 // 4        m_RadiusMax
};

struct SpellRangeEntry
{
    uint32  ID;                                             // 0        m_ID
    float   minRangeHostile;                                // 1        m_RangeMin
    float   minRangeFriend;                                 // 2        m_RangeMin
    float   maxRangeHostile;                                // 3        m_RangeMax
    float   maxRangeFriend;                                 // 4        m_RangeMax
    uint32  type;                                           // 5        m_Flags
    //char*     Name;                                       // 6        m_displayName_lang
    //char*     ShortName;                                  // 7        m_displayNameShort_lang
};

struct SpellEquippedItemsEntry
{
    uint32  Id;                                             // 0        m_ID
    uint32  SpellID;                                        // 1        m_SpellID
    uint32  DifficultyID;                                   // 2        m_DifficultyID
    int32   EquippedItemClass;                              // 3        m_equippedItemClass (value)
    int32   EquippedItemInventoryTypeMask;                  // 4        m_equippedItemInvTypes (mask)
    int32   EquippedItemSubClassMask;                       // 5        m_equippedItemSubclass (mask)
};

struct SpellCooldownsEntry
{
    uint32  Id;                                             // 0        m_ID
    uint32  m_SpellID;                                      // 1        m_SpellID
    uint32  m_DifficultyID;                                 // 2        m_DifficultyID
    uint32  CategoryRecoveryTime;                           // 3        m_categoryRecoveryTime
    uint32  RecoveryTime;                                   // 4        m_recoveryTime
    uint32  StartRecoveryTime;                              // 5        m_startRecoveryTime
};

struct SpellInterruptsEntry
{
    uint32  Id;                                             // 0        m_ID
    uint32  SpellID;                                        // 1        m_SpellID
    uint32  DifficultyID;                                   // 2        m_DifficultyID
    uint64  AuraInterruptFlags;                             // 3        m_auraInterruptFlags
    uint64  ChannelInterruptFlags;                          // 5        m_channelInterruptFlags
    uint32  InterruptFlags;                                 // 7        m_interruptFlags
};

struct SpellLevelsEntry
{
    uint32  Id;                                             // 0        m_ID
    uint32  SpellID;                                        // 1        m_spellId
    uint32  DifficultyID;                                   // 2        m_DifficultyID
    uint32  baseLevel;                                      // 3        m_baseLevel
    uint32  maxLevel;                                       // 4        m_maxLevel
    uint32  spellLevel;                                     // 5        m_spellLevel
};

#define MAX_SHAPESHIFT_SPELLS 8

struct SpellShapeshiftFormEntry
{
    uint32  ID;                                             // 0        m_ID
    //uint32    m_BonusActionBar;                           // 1        m_BonusActionBar
    //char*     m_NameLang;                                 // 2        m_NameLang
    uint32  m_Flags;                                          // 3      m_Flags
    int32   creatureType;                                   // 4        m_CreatureType
    //uint32    m_AttackIconID;                             // 5        m_AttackIconID
    uint32  attackSpeed;                                    // 6        m_CombatRoundTime
    uint32  modelID_A;                                      // 7        m_CreatureDisplayID
    uint32  modelID_H;                                      // 8        m_CreatureDisplayID
    //uint32    m_CreatureDisplayID;                        // 9        m_CreatureDisplayID
    //uint32    m_CreatureDisplayID;                        // 10       m_CreatureDisplayID
    uint32  stanceSpell[MAX_SHAPESHIFT_SPELLS];             // 11-18    m_PresetSpellID
    //uint32    m_MountTypeID;                              // 19       m_MountTypeID
    //uint32    m_ExitSoundEntriesID;                       // 20       m_ExitSoundEntriesID
};

struct SpellShapeshiftEntry
{
    uint32  Id;                                             // 0 -      m_ID
    uint32  StancesNot;                                     // 3 -      m_shapeshiftExclude
    // uint32    m_shapeshiftExclude;                       // 2 -      m_shapeshiftExclude
    uint32  Stances;                                        // 1 -      m_shapeshiftMask
    // uint32   m_ShapeshiftMask;                           // 4 -      m_ShapeshiftMask
    // int32    StanceBarOrder;                             // 5 -      m_stanceBarOrder
};

struct SpellTargetRestrictionsEntry
{
    uint32  Id;                                             // 0        m_ID
    uint32  SpellId;                                        // 1        m_spellId
    uint32  DifficultyID;                                   // 2        m_DifficultyID
    float   MaxTargetRadius;                                // 3        m_ConeAngle
    float   Width;                                          // 4        m_Width
    uint32  MaxAffectedTargets;                             // 5        m_maxTargets
    uint32  MaxTargetLevel;                                 // 6        m_maxTargetLevel
    uint32  TargetCreatureType;                             // 7        m_targetCreatureType
    uint32  Targets;                                        // 8        m_targets
};

struct SpellScalingEntry
{
    //uint32    Id;                                         // 0        m_ID
    int32   CastTimeMin;                                    // 1        m_CastTimeMin
    int32   CastTimeMax;                                    // 2        m_CastTimeMax
    int32   CastTimeMaxLevel;                               // 3        m_CastTimeMaxLevel
    int32   ScalingClass;                                   // 4        m_Class                 index * 100 + charLevel - 1 => gtSpellScaling.dbc
    float   CoefBase;                                       // 5        m_NerfFactor
    uint32  CoefLevelBase;                                  // 6        m_NerfMaxLevel
    uint32  MaxScalingLevel;                                // 7        m_MaxScalingLevel
    uint32  ScalesFromItemLevel;                            // 8        m_ScalesFromItemLevel
};

struct SpellDurationEntry
{
    uint32  ID;                                             // 0        m_ID
    int32   Duration[3];                                    // 1-3      m_Duration + m_DurationPerLevel + m_MaxDuration
};

#define MAX_ENCHANTMENT_SPELLS 3

struct SpellItemEnchantmentEntry
{
    uint32  ID;                                             // 0        m_ID
    //uint32    charges;                                    // 1        m_charges
    uint32  type[MAX_ENCHANTMENT_SPELLS];                   // 2-4      m_effect[3]
    uint32  amount[MAX_ENCHANTMENT_SPELLS];                 // 5-7      m_effectPointsMin[3]
    uint32  spellid[MAX_ENCHANTMENT_SPELLS];                // 8-10     m_effectArg[3]
    char*   description;                                    // 11       m_name_lang
    uint32  aura_id;                                        // 12       m_itemVisual
    uint32  slot;                                           // 13       m_flags
    uint32  GemID;                                          // 14       m_src_itemID
    uint32  EnchantmentCondition;                           // 15       m_condition_id
    uint32  requiredSkill;                                  // 16       m_requiredSkillID
    uint32  requiredSkillValue;                             // 17       m_requiredSkillRank
    uint32  requiredLevel;                                  // 18       m_MinLevel
    //uint32    m_MaxLevel;                                 // 19       m_MaxLevel
    // uint32   m_ItemLevel;                                // 20       m_ItemLevel
    // int32    m_ScalingClass;                             // 21       m_ScalingClass
    // int32    m_ScalingClassRestricted                    // 22       m_ScalingClassRestricted
    // float    m_EffectScalingPoints[3]                    // 23 -25   m_EffectScalingPoints

};

//@todo
struct SpellItemEnchantmentConditionEntry
{
    uint32  ID;                                             // 0        m_ID
    uint8   Color[5];                                       // 1-5      m_lt_operandType[5]
    //uint32  LT_Operand[5];                                // 6-10     m_lt_operand[5]
    uint8   Comparator[5];                                  // 11-15    m_operator[5]
    uint8   CompareColor[5];                                // 15-20    m_rt_operandType[5]
    uint32  Value[5];                                       // 21-25    m_rt_operand[5]
    //uint8   Logic[5]                                      // 25-30    m_logic[5]
};

// SummonProperties.dbc
struct SummonPropertiesEntry
{
    uint32  Id;                                             // 0        m_ID
    uint32  Category;                                       // 1        m_Control
    uint32  Faction;                                        // 2        m_Faction
    uint32  Type;                                           // 3        m_Title
    uint32  Slot;                                           // 4        m_Slot
    uint32  Flags;                                          // 5        m_Flags
};

struct TalentEntry
{
    uint32  Id;                 // 0
    uint32  SpecID;             // 1
    uint32  TierID;             // 2
    uint32  ColumnIndex;        // 3
    uint32  SpellID;            // 4
    uint32  Flags;              // 5
    uint32  CategoryMask[2];    // 6-7
    uint32  ClassID;            // 8
    uint32  OverridesSpellID;   // 9
    char*   DescriptionLang;    // 10
};

struct TaxiNodesEntry
{
    uint32  ID;                                             // 0        m_ID
    uint32  map_id;                                         // 1        m_ContinentID
    float   x;                                              // 2        m_x
    float   y;                                              // 3        m_y
    float   z;                                              // 4        m_z
    char*   name;                                           // 5        m_Name_lang
    uint32  MountCreatureID[2];                             // 6-7      m_MountCreatureID[2]
    //uint32    m_ConditionID                               // 8        m_ConditionID
    //uint32    m_Flags                                     // 9        m_Flags
    //float     m_MapOffset                                 // 10       m_MapOffset
    //float     m_MapOffset                                 // 11       m_MapOffset
};

struct TaxiPathEntry
{
    uint32  ID;                                             // 0        m_ID
    uint32  from;                                           // 1        m_FromTaxiNode
    uint32  to;                                             // 2        m_ToTaxiNode
    uint32  price;                                          // 3        m_Cost
};

struct TotemCategoryEntry
{
    uint32    ID;                                           // 0        m_ID
    //char*   name;                                         // 1        m_name_lang
    uint32    categoryType;                                 // 2        m_totemCategoryType     (one for specialization)
    uint32    categoryMask;                                 // 3        m_totemCategoryMask     (compatibility mask for same type: different for totems, compatible from high to low for rods)
};

struct TransportAnimationEntry
{
    //uint32  Id;
    uint32  TransportEntry;
    uint32  TimeSeg;
    float   X;
    float   Y;
    float   Z;
    //uint32  MovementId;
};

struct TransportRotationEntry
{
    //uint32  Id;
    uint32  TransportEntry;
    uint32  TimeSeg;
    float   X;
    float   Y;
    float   Z;
    float   W;
};

#define MAX_VEHICLE_SEATS 8

struct VehicleEntry
{
    uint32  m_ID;                                           // 0        m_ID
    uint32  m_flags;                                        // 1        m_Flags
    //uint32    m_FlagsB                                    // 2        m_FlagsB
    float   m_turnSpeed;                                    // 3        m_TurnSpeed
    float   m_pitchSpeed;                                   // 4        m_PitchSpeed
    float   m_pitchMin;                                     // 5        m_PitchMin
    float   m_pitchMax;                                     // 6        m_PitchMax
    uint32  m_seatID[MAX_VEHICLE_SEATS];                    // 7-14     m_SeatID
    float   m_mouseLookOffsetPitch;                         // 15       m_MouseLookOffsetPitch
    float   m_cameraFadeDistScalarMin;                      // 16       m_CameraFadeDistScalarMin
    float   m_cameraFadeDistScalarMax;                      // 17       m_CameraFadeDistScalarMax
    float   m_cameraPitchOffset;                            // 18       m_CameraPitchOffset
    float   m_facingLimitRight;                             // 19       m_FacingLimitRight
    float   m_facingLimitLeft;                              // 20       m_FacingLimitLeft
    float   m_msslTrgtTurnLingering;                        // 21       m_MsslTrgtTurnLingering
    float   m_msslTrgtPitchLingering;                       // 22       m_MsslTrgtPitchLingering
    float   m_msslTrgtMouseLingering;                       // 23       m_MsslTrgtMouseLingering
    float   m_msslTrgtEndOpacity;                           // 24       m_MsslTrgtEndOpacity
    float   m_msslTrgtArcSpeed;                             // 25       m_MsslTrgtArcSpeed
    float   m_msslTrgtArcRepeat;                            // 26       m_MsslTrgtArcRepeat
    float   m_msslTrgtArcWidth;                             // 27       m_MsslTrgtArcWidth
    float   m_msslTrgtImpactRadius[2];                      // 28-29    m_MsslTrgtImpactRadius
    char*   m_msslTrgtArcTexture;                           // 30       m_MsslTrgtArcTexture
    char*   m_msslTrgtImpactTexture;                        // 31       m_MsslTrgtImpactTexture
    char*   m_msslTrgtImpactModel[2];                       // 32-33    m_MsslTrgtImpactModel
    float   m_cameraYawOffset;                              // 34       m_CameraYawOffset
    uint32  m_uiLocomotionType;                             // 35       m_UiLocomotionType
    float   m_msslTrgtImpactTexRadius;                      // 36       m_MsslTrgtImpactTexRadius
    uint32  m_uiSeatIndicatorType;                          // 37       m_VehicleUIIndicatorID
    uint32  m_PowerDisplayID;                               // 38       m_PowerDisplayID
    //uint32    m_PowerDisplayID;                           // 39       m_PowerDisplayID
    //uint32    m_PowerDisplayID;                           // 40       m_PowerDisplayID
};

struct VehicleSeatEntry
{
    uint32  m_ID;                                           // 0        m_ID
    uint32  m_flags;                                        // 1        m_Flags
    int32   m_attachmentID;                                 // 2        m_AttachmentID
    float   m_attachmentOffsetX;                            // 3        m_AttachmentOffsetX
    float   m_attachmentOffsetY;                            // 4        m_AttachmentOffsetY
    float   m_attachmentOffsetZ;                            // 5        m_AttachmentOffsetZ
    float   m_enterPreDelay;                                // 6        m_EnterPreDelay
    float   m_enterSpeed;                                   // 7        m_EnterSpeed
    float   m_enterGravity;                                 // 8        m_EnterGravity
    float   m_enterMinDuration;                             // 9        m_EnterMinDuration
    float   m_enterMaxDuration;                             // 10       m_EnterMaxDuration
    float   m_enterMinArcHeight;                            // 11       m_EnterMinArcHeight
    float   m_enterMaxArcHeight;                            // 12       m_EnterMaxArcHeight
    int32   m_enterAnimStart;                               // 13       m_EnterAnimStart
    int32   m_enterAnimLoop;                                // 14       m_EnterAnimLoop
    int32   m_rideAnimStart;                                // 15       m_RideAnimStart
    int32   m_rideAnimLoop;                                 // 16       m_RideAnimLoop
    int32   m_rideUpperAnimStart;                           // 17       m_RideUpperAnimStart
    int32   m_rideUpperAnimLoop;                            // 18       m_RideUpperAnimLoop
    float   m_exitPreDelay;                                 // 19       m_ExitPreDelay
    float   m_exitSpeed;                                    // 20       m_ExitSpeed
    float   m_exitGravity;                                  // 21       m_ExitGravity
    float   m_exitMinDuration;                              // 22       m_ExitMinDuration
    float   m_exitMaxDuration;                              // 23       m_ExitMaxDuration
    float   m_exitMinArcHeight;                             // 24       m_ExitMinArcHeight
    float   m_exitMaxArcHeight;                             // 25       m_ExitMaxArcHeight
    int32   m_exitAnimStart;                                // 26       m_ExitAnimStart
    int32   m_exitAnimLoop;                                 // 27       m_ExitAnimLoop
    int32   m_exitAnimEnd;                                  // 28       m_ExitAnimEnd
    float   m_passengerYaw;                                 // 29       m_PassengerYaw
    float   m_passengerPitch;                               // 30       m_PassengerPitch
    float   m_passengerRoll;                                // 31       m_PassengerRoll
    int32   m_passengerAttachmentID;                        // 32       m_PassengerAttachmentID
    int32   m_vehicleEnterAnim;                             // 33       m_VehicleEnterAnim
    int32   m_vehicleExitAnim;                              // 34       m_VehicleExitAnim
    int32   m_vehicleRideAnimLoop;                          // 35       m_VehicleRideAnimLoop
    int32   m_vehicleEnterAnimBone;                         // 36       m_VehicleEnterAnimBone
    int32   m_vehicleExitAnimBone;                          // 37       m_VehicleExitAnimBone
    int32   m_vehicleRideAnimLoopBone;                      // 38       m_VehicleRideAnimLoopBone
    float   m_vehicleEnterAnimDelay;                        // 39       m_VehicleEnterAnimDelay
    float   m_vehicleExitAnimDelay;                         // 40       m_VehicleExitAnimDelay
    uint32  m_vehicleAbilityDisplay;                        // 41       m_VehicleAbilityDisplay
    uint32  m_enterUISoundID;                               // 42       m_EnterUISoundID
    uint32  m_exitUISoundID;                                // 43       m_ExitUISoundID
    uint32  m_flagsB;                                       // 44       m_FlagsB
    //float     m_CameraEnteringDelay;                      // 45       m_CameraEnteringDelay
    //float     m_CameraEnteringDuration;                   // 46       m_CameraEnteringDuration
    //float     m_CameraExitingDelay;                       // 47       m_CameraExitingDelay
    //float     m_CameraExitingDuration;                    // 48       m_CameraExitingDuration
    //float     m_CameraOffsetX;                            // 48       m_CameraOffsetX
    //float     m_CameraOffsetY;                            // 59       m_CameraOffsetY
    //float     m_CameraOffsetZ;                            // 50       m_CameraOffsetZ
    //float     m_CameraPosChaseRate;                       // 51       m_CameraPosChaseRate
    //float     m_CameraFacingChaseRate;                    // 52       m_CameraFacingChaseRate
    //float     m_CameraEnteringZoom;                       // 53       m_CameraEnteringZoom
    //float     m_CameraSeatZoomMin;                        // 54       m_CameraSeatZoomMin
    //float     m_CameraSeatZoomMax;                        // 55       m_CameraSeatZoomMax
    //int32     m_EnterAnimKitID;                           // 56       m_EnterAnimKitID
    //int32     m_RideAnimKitID;                            // 57       m_RideAnimKitID
    //int32     m_ExitAnimKitID;                            // 58       m_ExitAnimKitID
    //int32     m_VehicleEnterAnimKitID;                    // 59       m_VehicleEnterAnimKitID
    //int32     m_VehicleRideAnimKitID;                     // 60       m_VehicleRideAnimKitID
    //int32     m_VehicleExitAnimKitID;                     // 61       m_VehicleExitAnimKitID
    //int32     m_CameraModeID;                             // 62       m_CameraModeID
    //int32     m_FlagsC;                                   // 63       m_FlagsC
    //int32     m_UiSkinFileDataID;                         // 64       m_UiSkinFileDataID

    bool CanEnterOrExit() const { return m_flags & VEHICLE_SEAT_FLAG_CAN_ENTER_OR_EXIT; }
    bool CanSwitchFromSeat() const { return m_flags & VEHICLE_SEAT_FLAG_CAN_SWITCH; }
    bool IsUsableByOverride() const { return (m_flags & VEHICLE_SEAT_FLAG_UNCONTROLLED)
                                        || (m_flagsB & (VEHICLE_SEAT_FLAG_B_USABLE_FORCED
                                        | VEHICLE_SEAT_FLAG_B_USABLE_FORCED_2
                                        | VEHICLE_SEAT_FLAG_B_USABLE_FORCED_3 | VEHICLE_SEAT_FLAG_B_USABLE_FORCED_4)); }
    bool IsEjectable() const { return m_flagsB & VEHICLE_SEAT_FLAG_B_EJECTABLE; }
};

struct WMOAreaTableEntry
{
    uint32  Id;                                             // 0        m_ID
    int32   rootId;                                         // 1        m_WMOID
    int32   adtId;                                          // 2        m_NameSetID
    int32   groupId;                                        // 3        m_WMOGroupID
    //uint32    m_SoundProviderPref;                        // 4        m_SoundProviderPref
    //uint32    m_SoundProviderPrefUnderwater;              // 5        m_SoundProviderPrefUnderwater
    //uint32    m_AmbienceID;                               // 6        m_AmbienceID
    //uint32    m_ZoneMusic;                                // 7        m_ZoneMusic
    //uint32    m_IntroSound;                               // 8        m_IntroSound
    uint32  Flags;                                          // 9        m_Flags                 Used for indoor/outdoor determination
    uint32  areaId;                                         // 10       m_AreaTableID
    //char*     m_AreaNameLang;                             // 11       m_AreaNameLang
    //uint32    m_UwIntroSound;                             // 12       m_UwIntroSound
    //uint32    m_UwZoneMusic;                              // 13       m_UwZoneMusic
    //uint32    m_UwAmbience;                               // 14       m_UwAmbience
};

struct WorldMapAreaEntry
{
    //uint32    m_ID;                                       // 0        m_ID
    uint32  map_id;                                         // 1        m_MapID
    uint32  area_id;                                        // 2        m_AreaID
    //char*     m_AreaName;                                 // 3        m_AreaName
    float   y1;                                             // 4        m_LocLeft
    float   y2;                                             // 5        m_LocRight
    float   x1;                                             // 6        m_LocTop
    float   x2;                                             // 7        m_LocBottom
    int32   virtual_map_id;                                 // 8        m_DisplayMapID          -1 (map_id have correct map) other: virtual map where zone show (map_id - where zone in fact internally)
    // int32    m_DefaultDungeon;                           // 9        m_DefaultDungeon        Floor    pointer to DungeonMap.dbc (owerride x1, x2, y1, y2 coordinates)
    // uint32   m_ParentWorldMapID;                         // 10       m_ParentWorldMapID
    // uint32   m_Flags;                                    // 11       m_Flags
    uint32  minRecommendedLevel;                            // 12       m_LevelRangeMin
    uint32  maxRecommendedLevel;                            // 13       m_LevelRangeMax
};

#define MAX_WORLD_MAP_OVERLAY_AREA_IDX 4

struct WorldMapOverlayEntry
{
    uint32  ID;                                             // 0        m_ID
    //uint32    worldMapAreaId;                             // 1        m_MapAreaID
    uint32  areatableID[MAX_WORLD_MAP_OVERLAY_AREA_IDX];    // 2-5      m_AreaID
    //char*     mTextureName;                               // 6        m_TextureName
    //uint32    m_TextureWidth;                             // 7        m_TextureWidth
    //uint32    m_TextureHeight;                            // 8        m_TextureHeight
    //uint32    m_OffsetX;                                  // 9        m_OffsetX
    //uint32    m_OffsetY;                                  // 10       m_OffsetY
    //uint32    m_HitRectTop;                               // 11       m_HitRectTop
    //uint32    m_HitRectLeft;                              // 12       m_HitRectLeft
    //uint32    m_HitRectBottom;                            // 13       m_HitRectBottom
    //uint32    m_HitRectRight;                             // 14       m_HitRectRight
    //uint32    m_PlayerConditionID;                        // 15       m_PlayerConditionID
};

struct World_PVP_AreaEntry
{
    uint32 ID;
    uint32 AreaID;
    uint32 NextTimeWorldState;
    uint32 GameTimeWorldState;
    uint32 BattlePopulateTime;
    uint32 MinLevel;
    uint32 MaxLevel;
    int32  MapID;
};

struct WorldSafeLocsEntry
{
    uint32    ID;                                           // 0
    uint32    map_id;                                       // 1
    float     x;                                            // 2
    float     y;                                            // 3
    float     z;                                            // 4
    float     o;                                            // 5
    //char*   name;                                         // 6 m_AreaName_lang
};

//UNUSED ACTUALY
/*
struct WorldStateUIEntry
{
    uint32  m_ID;                                           // 0        m_ID
    uint32  m_MapID;                                        // 1        m_MapID
    uint32  m_AreaID;                                       // 2        m_AreaID
    uint32  m_PhaseUseFlags;                                // 3        m_PhaseUseFlags
    uint32  m_PhaseID;                                      // 4        m_PhaseID
    uint32  m_PhaseGroupID;                                 // 5        m_PhaseGroupID
    char*   m_Icon;                                         // 6        m_Icon
    char*   m_StringLang;                                   // 7        m_StringLang
    char*   m_TooltipLang;                                  // 8        m_TooltipLang
    uint32  m_StateVariable;                                // 9        m_StateVariable
    uint32  m_Type;                                         // 10       m_Type
    char*   m_DynamicIcon;                                  // 11       m_DynamicIcon
    char*   m_DynamicTooltipLang;                           // 12       m_DynamicTooltipLang
    char*   m_ExtendedUI;                                   // 13       m_ExtendedUI
    uint32  m_ExtendedUIStateVariable[3];                   // 14 - 16  m_ExtendedUIStateVariable
};*/


struct CriteriaTreeEntry
{
    uint32 ID;                                              // 0
    uint32 CriteriaID;                                      // 1
    uint32 Amount;                                          // 2
    uint32 OrderIndex;                                      // 3
    uint32 Operator;                                        // 4
    uint32 Parent;                                          // 5
    uint32 Flags;                                           // 6
    char*  DescriptionLang;                                 // 7
};

struct ModifierTreeEntry
{
    uint32 ID;                                              // 0
    uint32 Type;                                            // 1
    uint32 Asset;                                           // 2
    uint32 SecondaryAsset;                                  // 3
    uint32 Operator;                                        // 4
    uint32 Amount;                                          // 5
    uint32 Parent;                                          // 6
};

struct CriteriaEntry
{
    uint32 ID;                                              // 0
    uint32 Type;                                            // 1
    union
    {
        // ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE                  = 0
        struct
        {
            uint32 CreatureID;
        } kill_creature;

        // ACHIEVEMENT_CRITERIA_TYPE_WIN_BG                         = 1
        struct
        {
            uint32 MapID;
        } win_bg;

        // ACHIEVEMENT_CRITERIA_TYPE_REACH_SKILL_LEVEL              = 7
        struct
        {
            uint32  skillID;
        } reach_skill_level;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_ACHIEVEMENT           = 8
        struct
        {
            uint32  linkedAchievement;
        } complete_achievement;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUESTS_IN_ZONE        = 11
        struct
        {
            uint32  zoneID;
        } complete_quests_in_zone;

        // ACHIEVEMENT_CRITERIA_TYPE_CURRENCY                       = 12
        struct
        {
            uint32 currency;
        } currencyGain;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_BATTLEGROUND          = 15
        struct
        {
            uint32  mapID;
        } complete_battleground;

        // ACHIEVEMENT_CRITERIA_TYPE_DEATH_AT_MAP                   = 16
        struct
        {
            uint32  mapID;
        } death_at_map;

        // ACHIEVEMENT_CRITERIA_TYPE_DEATH_IN_DUNGEON               = 18
        struct
        {
            uint32  manLimit;
        } death_in_dungeon;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_RAID                  = 19
        struct
        {
            uint32  groupSize;
        } complete_raid;

        // ACHIEVEMENT_CRITERIA_TYPE_KILLED_BY_CREATURE             = 20
        struct
        {
            uint32  creatureEntry;
        } killed_by_creature;

        // ACHIEVEMENT_CRITERIA_TYPE_DEATHS_FROM                    = 26
        struct
        {
            uint32 type;
        } death_from;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST                 = 27
        struct
        {
            uint32  questID;
        } complete_quest;

        // ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET                = 28
        // ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET2               = 69
        struct
        {
            uint32  spellID;
        } be_spell_target;

        // ACHIEVEMENT_CRITERIA_TYPE_CAST_SPELL                     = 29
        // ACHIEVEMENT_CRITERIA_TYPE_CAST_SPELL2                    = 110
        struct
        {
            uint32  spellID;
        } cast_spell;

        // ACHIEVEMENT_CRITERIA_TYPE_BG_OBJECTIVE_CAPTURE           = 30
        struct
        {
            uint32 objectiveId;
        } bg_objective;

        // ACHIEVEMENT_CRITERIA_TYPE_HONORABLE_KILL_AT_AREA         = 31
        struct
        {
            uint32  areaID;
        } honorable_kill_at_area;

        // ACHIEVEMENT_CRITERIA_TYPE_WIN_ARENA                      = 32
        struct
        {
            uint32 mapID;
        } win_arena;

        // ACHIEVEMENT_CRITERIA_TYPE_PLAY_ARENA                     = 33
        struct
        {
            uint32  mapID;
        } play_arena;

        // ACHIEVEMENT_CRITERIA_TYPE_LEARN_SPELL                    = 34
        struct
        {
            uint32  spellID;
        } learn_spell;

        // ACHIEVEMENT_CRITERIA_TYPE_OWN_ITEM                       = 36
        struct
        {
            uint32  itemID;
        } own_item;

        // ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_TEAM_RATING            = 38
        struct
        {
            uint32  teamtype;
        } highest_team_rating;

        // ACHIEVEMENT_CRITERIA_TYPE_REACH_TEAM_RATING              = 39
        struct
        {
            uint32  teamtype;
        } reach_team_rating;

        // ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_PERSONAL_RATING        = 39
        struct
        {
            uint32 teamtype;
        } highest_personal_rating;

        // ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILL_LEVEL              = 40
        struct
        {
            uint32  skillID;
        } learn_skill_level;

        // ACHIEVEMENT_CRITERIA_TYPE_USE_ITEM                       = 41
        struct
        {
            uint32  itemID;
        } use_item;

        // ACHIEVEMENT_CRITERIA_TYPE_LOOT_ITEM                      = 42
        struct
        {
            uint32  itemID;
        } loot_item;

        // ACHIEVEMENT_CRITERIA_TYPE_EXPLORE_AREA                   = 43
        struct
        {
            uint32  areaReference;
        } explore_area;

        // ACHIEVEMENT_CRITERIA_TYPE_OWN_RANK                       = 44
        struct
        {
            uint32  rank;
        } own_rank;

        // ACHIEVEMENT_CRITERIA_TYPE_GAIN_REPUTATION                = 46
        struct
        {
            uint32  factionID;
        } gain_reputation;

        // ACHIEVEMENT_CRITERIA_TYPE_EQUIP_EPIC_ITEM                = 49
        struct
        {
            uint32  itemSlot;
        } equip_epic_item;

        // ACHIEVEMENT_CRITERIA_TYPE_ROLL_NEED_ON_LOOT              = 50
        struct
        {
            uint32  rollValue;
        } roll_need_on_loot;

        // ACHIEVEMENT_CRITERIA_TYPE_ROLL_GREED_ON_LOOT             = 51
        struct
        {
            uint32  rollValue;
        } roll_greed_on_loot;

        // ACHIEVEMENT_CRITERIA_TYPE_HK_CLASS                       = 52
        struct
        {
            uint32  classID;
        } hk_class;

        // ACHIEVEMENT_CRITERIA_TYPE_HK_RACE                        = 53
        struct
        {
            uint32  raceID;
        } hk_race;

        // ACHIEVEMENT_CRITERIA_TYPE_DO_EMOTE                       = 54
        struct
        {
            uint32  emoteID;
        } do_emote;

        // ACHIEVEMENT_CRITERIA_TYPE_EQUIP_ITEM                     = 57
        struct
        {
            uint32  itemID;
        } equip_item;

        // ACHIEVEMENT_CRITERIA_TYPE_USE_GAMEOBJECT                 = 68
        struct
        {
            uint32  goEntry;
        } use_gameobject;

        // ACHIEVEMENT_CRITERIA_WIN_CHALLENGE_DUNGEON               = 71
        struct
        {
            uint32 MapID;
        } ChallengeDungeon;

        // ACHIEVEMENT_CRITERIA_TYPE_FISH_IN_GAMEOBJECT             = 72
        struct
        {
            uint32  goEntry;
        } fish_in_gameobject;

        // ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILLLINE_SPELLS         = 75
        struct
        {
            uint32  skillLine;
        } learn_skillline_spell;

        // ACHIEVEMENT_CRITERIA_TYPE_CAPTURE_BATTLEPET              = 96
        struct
        {
            uint32 BattlePetEntry;
        } CaptureBattlePet;

        // ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_STAT                   = 97
        struct
        {
            uint32  statType;
        } highest_stat;

        // ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_SPELLPOWER             = 98
        struct
        {
            uint32  spellSchool;
        } highest_spellpower;

        // ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_RATING                 = 100
        struct
        {
            uint32  ratingType;
        } highest_rating;

        // ACHIEVEMENT_CRITERIA_TYPE_LOOT_TYPE                      = 109
        struct
        {
            uint32  lootType;
        } loot_type;

        // ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILL_LINE               = 112
        struct
        {
            uint32  skillLine;
        } learn_skill_line;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_GUILD_CHALLENGE_TYPE  = 138
        struct
        {
            uint32 ChallengeTypeID;
        } GuildChallengeType;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_SCENARIO_ID           = 152
        struct
        {
            uint32 ScenarioID;
        } CompleteScenario;

        // ACHIEVEMENT_CRITERIA_TYPE_LEVELUP_BATTLEPET              = 160
        struct
        {
            uint32 RequiredLevel;
        } BattlePetLevelUp;

        struct
        {
            uint32 criteriaArg1;
        } raw;
    };

    uint32 StartEvent;                                      // 3
    uint32 StartAsset;                                      // 4
    uint32 StartTimer;                                      // 5
    uint32 FailEvent;                                       // 6
    uint32 FailAsset;                                       // 7
    uint32 ModifierTreeId;                                  // 8
    uint32 Flags;                                           // 9
    uint32 EligibilityWorldStateID;                         // 10
    uint32 EligibilityWorldStateValue;                      // 11
};

struct GtBattlePetTypeDamageModEntry
{
    //uint32 [petType][DamageType];
    float mod;
};

struct GtBattlePetXPEntry
{
    //uint32 [petType][DamageType];
    float value;
};

// SpellProcsPerMinute.dbc
struct SpellProcsPerMinuteEntry
{
    uint32 Id;                                              // 0        m_ID
    float  ProcsPerMinute;                                  // 1        m_procsPerMinute
    //bool unk;                                             // 2
};

// GCC have alternative #pragma pack() syntax and old gcc version not support pack(pop), also any gcc version not support it at some platform
#if defined(__GNUC__)
#pragma pack()
#else
#pragma pack(pop)
#endif

struct VectorArray
{
    std::vector<std::string> stringVectorArray[2];
};

typedef std::map<uint32, VectorArray> NameGenVectorArraysMap;

// Structures not used for casting to loaded DBC data and not required then packing
// @author Selenium: 5.4 valid
struct MapDifficulty
{
    MapDifficulty() : resetTime(0), maxPlayers(0), hasErrorMessage(false) {}
    MapDifficulty(uint32 _resetTime, uint32 _maxPlayers, bool _hasErrorMessage) : resetTime(_resetTime), maxPlayers(_maxPlayers), hasErrorMessage(_hasErrorMessage) {}

    uint32 resetTime;
    uint32 maxPlayers;
    bool hasErrorMessage;
};

struct TalentSpellPos
{
    TalentSpellPos() : talent_id(0), rank(0) {}
    TalentSpellPos(uint16 _talent_id, uint8 _rank) : talent_id(_talent_id), rank(_rank) {}

    uint16 talent_id;
    uint8  rank;
};

typedef std::map<uint32, TalentSpellPos> TalentSpellPosMap;

struct SpellEffect
{
    SpellEffect()
    {
        for (int i = 0; i < MAX_DIFFICULTY; i++)
        {
            for (int y = 0; y < MAX_SPELL_EFFECTS; y++)
            {
                effects[i][y] = 0;
            }
        }
    }
    SpellEffectEntry const* effects[MAX_DIFFICULTY][32];
};

typedef std::map<uint32, SpellEffect> SpellEffectMap;

struct TaxiPathBySourceAndDestination
{
    TaxiPathBySourceAndDestination() : ID(0), price(0) {}
    TaxiPathBySourceAndDestination(uint32 _id, uint32 _price) : ID(_id), price(_price) {}

    uint32    ID;
    uint32    price;
};
typedef std::map<uint32, TaxiPathBySourceAndDestination> TaxiPathSetForSource;
typedef std::map<uint32, TaxiPathSetForSource> TaxiPathSetBySource;

#define TaxiMaskSize 162
typedef uint8 TaxiMask[TaxiMaskSize];
#endif
