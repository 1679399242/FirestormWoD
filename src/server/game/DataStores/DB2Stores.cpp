/*
 * Copyright (C) 2011 TrintiyCore <http://www.trinitycore.org/>
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

#include "DB2Stores.h"
#include "DBCStores.h"
#include "Log.h"
#include "SharedDefines.h"
#include "SpellMgr.h"
#include "DB2fmt.h"
#include "Item.h"

#include <map>

DB2Storage <PathNodeEntry>                  sPathNodeStore(PathNodeEntryfmt);
DB2Storage <LocationEntry>                  sLocationStore(LocationEntryfmt);

std::map<uint32 /*curveID*/, std::map<uint32/*index*/, CurvePointEntry const*, std::greater<uint32>>> HeirloomCurvePoints;
std::unordered_map<uint32 /*ItemID*/, HeirloomEntry const*> HeirloomEntryByItemID;

DB2Storage <CurrencyTypesEntry>             sCurrencyTypesStore(CurrencyTypesfmt);
DB2Storage <CurvePointEntry>                sCurvePointStore(CurvePointEntryfmt);
DB2Storage <GroupFinderActivityEntry>       sGroupFinderActivityStore(GroupFinderActivityfmt);
DB2Storage <GroupFinderCategoryEntry>       sGroupFinderCategoryStore(GroupFinderCategoryfmt);
DB2Storage <ItemEntry>                      sItemStore(Itemfmt);
DB2Storage <ItemBonusEntry>                 sItemBonusStore(ItemBonusfmt);
DB2Storage <ItemBonusTreeNodeEntry>         sItemBonusTreeNodeStore(ItemBonusTreeNodefmt);
DB2Storage <ItemXBonusTreeEntry>            sItemXBonusTreeStore(ItemXBonusTreefmt);
DB2Storage <ItemCurrencyCostEntry>          sItemCurrencyCostStore(ItemCurrencyCostfmt);
DB2Storage <ItemExtendedCostEntry>          sItemExtendedCostStore(ItemExtendedCostEntryfmt);
DB2Storage <ItemSparseEntry>                sItemSparseStore(ItemSparsefmt);
DB2Storage <ItemEffectEntry>                sItemEffectStore(ItemEffectFmt);
DB2Storage <HeirloomEntry>                  sHeirloomStore(HeirloomFmt);
DB2Storage <PvpItemEntry>                   sPvpItemStore(PvpItemfmt);
DB2Storage <ItemModifiedAppearanceEntry>    sItemModifiedAppearanceStore(ItemModifiedAppearanceFmt);
DB2Storage <ItemAppearanceEntry>            sItemAppearanceStore(ItemAppearanceFmt);
DB2Storage <SpellReagentsEntry>             sSpellReagentsStore(SpellReagentsEntryfmt);
DB2Storage <ItemUpgradeEntry>               sItemUpgradeStore(ItemUpgradeEntryfmt);
DB2Storage <RulesetItemUpgradeEntry>        sRulesetItemUpgradeStore(RulesetItemUpgradeEntryfmt);
DB2Storage <SceneScriptEntry>               sSceneScriptStore(SceneScriptEntryfmt);
DB2Storage <SceneScriptPackageEntry>        sSceneScriptPackageStore(SceneScriptPackageEntryfmt);
DB2Storage <SoundEntriesEntry>              sSoundEntriesStore(SoundEntriesfmt);
DB2Storage <TaxiNodesEntry>                 sTaxiNodesStore(TaxiNodesEntryfmt);
DB2Storage <TaxiPathEntry>                  sTaxiPathStore(TaxiPathEntryfmt);
DB2Storage <TaxiPathNodeEntry>              sTaxiPathNodeStore(TaxiPathNodeEntryfmt);
DB2Storage <SpellRuneCostEntry>             sSpellRuneCostStore(SpellRuneCostfmt);
DB2Storage <SpellCastingRequirementsEntry>  sSpellCastingRequirementsStore(SpellCastingRequirementsEntryfmt);
DB2Storage <SpellAuraRestrictionsEntry>     sSpellAuraRestrictionsStore(SpellAuraRestrictionsEntryfmt);
DB2Storage <AreaPOIEntry>                   sAreaPOIStore(AreaPOIEntryfmt);
DB2Storage <HolidaysEntry>                  sHolidaysStore(Holidaysfmt);
DB2Storage <OverrideSpellDataEntry>         sOverrideSpellDataStore(OverrideSpellDatafmt);
DB2Storage <SpellMiscEntry>                 sSpellMiscStore(SpellMiscEntryfmt);
DB2Storage <SpellPowerEntry>                sSpellPowerStore(SpellPowerEntryfmt);
DB2Storage <SpellTotemsEntry>               sSpellTotemsStore(SpellTotemsEntryfmt);
DB2Storage <SpellClassOptionsEntry>         sSpellClassOptionsStore(SpellClassOptionsEntryfmt);
DB2Storage <MapChallengeModeEntry>          sMapChallengeModeStore(MapChallengeModeEntryfmt);
DB2Storage <QuestPackageItemEntry>          sQuestPackageItemStore(QuestPackageItemEntryfmt);
DB2Storage <MountEntry>                     sMountStore(MountEntryfmt);
DB2Storage <PlayerConditionEntry>           sPlayerConditionStore(PlayerConditionEntryfmt);
DB2Storage <VignetteEntry>                  sVignetteStore(VignetteEntryfmt);
DB2Storage <GlyphRequiredSpecEntry>         sGlyphRequiredSpecStore(GlyphRequiredSpecfmt);

//////////////////////////////////////////////////////////////////////////
/// Garrison DB2
//////////////////////////////////////////////////////////////////////////
DB2Storage <GarrSiteLevelEntry>             sGarrSiteLevelStore(GarrSiteLevelEntryfmt);
DB2Storage <GarrSiteLevelPlotInstEntry>     sGarrSiteLevelPlotInstStore(GarrSiteLevelPlotInstEntryfmt);
DB2Storage <GarrPlotInstanceEntry>          sGarrPlotInstanceStore(GarrPlotInstanceEntryfmt);
DB2Storage <GarrPlotEntry>                  sGarrPlotStore(GarrPlotEntryfmt);
DB2Storage <GarrPlotUICategoryEntry>        sGarrPlotUICategoryStore(GarrPlotUICategoryEntryfmt);
DB2Storage <GarrMissionEntry>               sGarrMissionStore(GarrMissionEntryfmt);
DB2Storage <GarrMissionRewardEntry>         sGarrMissionRewardStore(GarrMissionRewardEntryfmt);
DB2Storage <GarrMissionXEncouterEntry>      sGarrMissionXEncouterStore(GarrMissionXEncouterEntryfmt);
DB2Storage <GarrBuildingEntry>              sGarrBuildingStore(GarrBuildingEntryfmt);
DB2Storage <GarrPlotBuildingEntry>          sGarrPlotBuildingStore(GarrPlotBuildingEntryfmt);
DB2Storage <GarrFollowerEntry>              sGarrFollowerStore(GarrFollowerEntryfmt);
DB2Storage <GarrAbilityEntry>               sGarrAbilityStore(GarrAbilityEntryfmt);
DB2Storage <GarrAbilityEffectEntry>         sGarrAbilityEffectStore(GarrAbilityEffectEntryfmt);
DB2Storage <GarrFollowerXAbilityEntry>      sGarrFollowerXAbilityStore(GarrFollowerXAbilityEntryfmt);
DB2Storage <GarrBuildingPlotInstEntry>      sGarrBuildingPlotInstStore(GarrBuildingPlotInstEntryfmt);
DB2Storage <GarrMechanicTypeEntry>          sGarrMechanicTypeStore(GarrMechanicTypeEntryfmt);
DB2Storage <GarrMechanicEntry>              sGarrMechanicStore(GarrMechanicEntryfmt);
DB2Storage <GarrEncouterXMechanicEntry>     sGarrEncouterXMechanicStore(GarrEncouterXMechanicEntryfmt);
DB2Storage <GarrFollowerLevelXPEntry>       sGarrFollowerLevelXPStore(GarrFollowerLevelXPEntryfmt);
DB2Storage <GarrSpecializationEntry>        sGarrSpecializationStore(GarrSpecializationEntryfmt);
DB2Storage <CharShipmentEntry>              sCharShipmentStore(CharShipmentEntryfmt);
DB2Storage <CharShipmentContainerEntry>     sCharShipmentContainerStore(CharShipmentContainerEntryfmt);

//////////////////////////////////////////////////////////////////////////
/// Battle pet
//////////////////////////////////////////////////////////////////////////
DB2Storage<BattlePetAbilityEntry>           sBattlePetAbilityStore(BattlePetAbilityfmt);
DB2Storage<BattlePetAbilityEffectEntry>     sBattlePetAbilityEffectStore(BattlePetAbilityEffectfmt);
DB2Storage<BattlePetAbilityTurnEntry>       sBattlePetAbilityTurnStore(BattlePetAbilityTurnfmt);
DB2Storage<BattlePetAbilityStateEntry>      sBattlePetAbilityStateStore(BattlePetAbilityStatefmt);
DB2Storage<BattlePetStateEntry>             sBattlePetStateStore(BattlePetStatefmt);
DB2Storage<BattlePetEffectPropertiesEntry>  sBattlePetEffectPropertiesStore(BattlePetEffectPropertiesfmt);
DB2Storage<BattlePetBreedQualityEntry>      sBattlePetBreedQualityStore(BattlePetBreedQualityfmt);
DB2Storage<BattlePetBreedStateEntry>        sBattlePetBreedStateStore(BattlePetBreedStatefmt);
DB2Storage<BattlePetSpeciesEntry>           sBattlePetSpeciesStore(BattlePetSpeciesfmt);
DB2Storage<BattlePetSpeciesStateEntry>      sBattlePetSpeciesStateStore(BattlePetSpeciesStatefmt);
DB2Storage<BattlePetSpeciesXAbilityEntry>   sBattlePetSpeciesXAbilityStore(BattlePetSpeciesXAbilityfmt);

// DBC used only for initialization sTaxiPathNodeStore at startup.
TaxiPathSetBySource sTaxiPathSetBySource;
TaxiPathNodesByPath sTaxiPathNodesByPath;
TaxiNodesByMap      sTaxiNodesByMap;
SpellTotemMap       sSpellTotemMap;
std::map<uint32, std::vector<uint32>> sItemEffectsByItemID;
std::map<uint32, std::vector<ItemBonusEntry const*>> sItemBonusesByID;
std::map<uint32, std::vector<ItemXBonusTreeEntry const*>> sItemBonusTreeByID;
std::map<uint32, std::vector<QuestPackageItemEntry const*>> sQuestPackageItemsByGroup;

typedef std::list<std::string> StoreProblemList1;

uint32 DB2FilesCount = 0;

static bool LoadDB2_assert_print(uint32 fsize,uint32 rsize, const std::string& filename)
{
    sLog->outError(LOG_FILTER_GENERAL, "Size of '%s' setted by format string (%u) not equal size of C++ structure (%u).", filename.c_str(), fsize, rsize);

    // ASSERT must fail after function call
    return false;
}

struct LocalDB2Data
{
    LocalDB2Data(LocaleConstant loc) : defaultLocale(loc), availableDb2Locales(0xFFFFFFFF) {}

    LocaleConstant defaultLocale;

    // bitmasks for index of fullLocaleNameList
    uint32 availableDb2Locales;
};

template<class T>
inline void LoadDB2(StoreProblemList1& errlist, DB2Storage<T>& storage, const std::string& db2_path, const std::string& filename)
{
    // compatibility format and C++ structure sizes
    ASSERT(DB2FileLoader::GetFormatRecordSize(storage.GetFormat()) == sizeof(T) || LoadDB2_assert_print(DB2FileLoader::GetFormatRecordSize(storage.GetFormat()), sizeof(T), filename));

    ++DB2FilesCount;

    std::string db2_filename = db2_path + filename;
    if (!storage.Load(db2_filename.c_str()))
    {
        // sort problematic db2 to (1) non compatible and (2) nonexistent
        if (FILE * f = fopen(db2_filename.c_str(), "rb"))
        {
            char buf[100];
            snprintf(buf, 100,"(exist, but have %d fields instead " SIZEFMTD ") Wrong client version DBC file?", storage.GetFieldCount(), strlen(storage.GetFormat()));
            errlist.push_back(db2_filename + buf);
            fclose(f);
        }
        else
            errlist.push_back(db2_filename);
    }
}

SpellTotemsEntry const* GetSpellTotemEntry(uint32 spellId, uint8 totem)
{
    SpellTotemMap::const_iterator itr = sSpellTotemMap.find(spellId);
    if (itr == sSpellTotemMap.end())
        return NULL;

    return itr->second.totems[totem];
}

void LoadDB2Stores(const std::string& dataPath)
{
    std::string db2Path = dataPath + "dbc/";

    StoreProblemList1 bad_db2_files;

    //////////////////////////////////////////////////////////////////////////
    /// Misc DB2
    //////////////////////////////////////////////////////////////////////////
    LoadDB2(bad_db2_files, sAreaGroupStore,                 db2Path, "AreaGroup.db2");
    LoadDB2(bad_db2_files, sSoundEntriesStore,              db2Path, "SoundEntries.db2");
    LoadDB2(bad_db2_files, sCurrencyTypesStore,             db2Path, "CurrencyTypes.db2");
    LoadDB2(bad_db2_files, sPathNodeStore,                  db2Path, "PathNode.db2");
    LoadDB2(bad_db2_files, sLocationStore,                  db2Path, "Location.db2");
    LoadDB2(bad_db2_files, sAreaPOIStore,                   db2Path, "AreaPOI.db2");
    LoadDB2(bad_db2_files, sCurvePointStore,                db2Path, "CurvePoint.db2");
    LoadDB2(bad_db2_files, sGroupFinderActivityStore,       db2Path, "GroupFinderActivity.db2");
    LoadDB2(bad_db2_files, sGroupFinderCategoryStore,       db2Path, "GroupFinderCategory.db2");
    LoadDB2(bad_db2_files, sHolidaysStore,                  db2Path, "Holidays.db2");
    LoadDB2(bad_db2_files, sMapChallengeModeStore,          db2Path, "MapChallengeMode.db2");
    LoadDB2(bad_db2_files, sMountStore,                     db2Path, "Mount.db2");
    LoadDB2(bad_db2_files, sPlayerConditionStore,           db2Path, "PlayerCondition.db2");
    LoadDB2(bad_db2_files, sVignetteStore,                  db2Path, "Vignette.db2");
    LoadDB2(bad_db2_files, sGlyphRequiredSpecStore,         db2Path, "GlyphRequiredSpec.db2");

    //////////////////////////////////////////////////////////////////////////
    /// Quest DB2
    //////////////////////////////////////////////////////////////////////////
    LoadDB2(bad_db2_files, sQuestPackageItemStore,          db2Path, "QuestPackageItem.db2");

    //////////////////////////////////////////////////////////////////////////
    /// Scene Script DB2
    //////////////////////////////////////////////////////////////////////////
    LoadDB2(bad_db2_files, sSceneScriptStore,               db2Path, "SceneScript.db2");
    LoadDB2(bad_db2_files, sSceneScriptPackageStore,        db2Path, "SceneScriptPackage.db2");

    //////////////////////////////////////////////////////////////////////////
    /// Taxi DB2
    //////////////////////////////////////////////////////////////////////////
    LoadDB2(bad_db2_files, sTaxiNodesStore,                 db2Path, "TaxiNodes.db2");
    LoadDB2(bad_db2_files, sTaxiPathStore,                  db2Path, "TaxiPath.db2");
    LoadDB2(bad_db2_files, sTaxiPathNodeStore,              db2Path, "TaxiPathNode.db2");

    //////////////////////////////////////////////////////////////////////////
    /// Item DB2
    //////////////////////////////////////////////////////////////////////////
    LoadDB2(bad_db2_files, sItemStore,                      db2Path, "Item.db2");
    LoadDB2(bad_db2_files, sItemCurrencyCostStore,          db2Path, "ItemCurrencyCost.db2");
    LoadDB2(bad_db2_files, sItemSparseStore,                db2Path, "Item-sparse.db2");
    LoadDB2(bad_db2_files, sItemEffectStore,                db2Path, "ItemEffect.db2");
    LoadDB2(bad_db2_files, sItemModifiedAppearanceStore,    db2Path, "ItemModifiedAppearance.db2");
    LoadDB2(bad_db2_files, sItemAppearanceStore,            db2Path, "ItemAppearance.db2");
    LoadDB2(bad_db2_files, sItemExtendedCostStore,          db2Path, "ItemExtendedCost.db2");
    LoadDB2(bad_db2_files, sHeirloomStore,                  db2Path, "Heirloom.db2");
    LoadDB2(bad_db2_files, sPvpItemStore,                   db2Path, "PvpItem.db2");
    LoadDB2(bad_db2_files, sItemUpgradeStore,               db2Path, "ItemUpgrade.db2");
    LoadDB2(bad_db2_files, sRulesetItemUpgradeStore,        db2Path, "RulesetItemUpgrade.db2");

    //////////////////////////////////////////////////////////////////////////
    /// Item Bonus DB2
    //////////////////////////////////////////////////////////////////////////
    LoadDB2(bad_db2_files, sItemBonusStore,                 db2Path, "ItemBonus.db2");
    LoadDB2(bad_db2_files, sItemBonusTreeNodeStore,         db2Path, "ItemBonusTreeNode.db2");
    LoadDB2(bad_db2_files, sItemXBonusTreeStore,            db2Path, "ItemXBonusTree.db2");

    //////////////////////////////////////////////////////////////////////////
    /// Spell DB2
    //////////////////////////////////////////////////////////////////////////
    LoadDB2(bad_db2_files, sSpellReagentsStore,             db2Path, "SpellReagents.db2");
    LoadDB2(bad_db2_files, sSpellRuneCostStore,             db2Path, "SpellRuneCost.db2");
    LoadDB2(bad_db2_files, sSpellCastingRequirementsStore,  db2Path, "SpellCastingRequirements.db2");
    LoadDB2(bad_db2_files, sSpellAuraRestrictionsStore,     db2Path, "SpellAuraRestrictions.db2");
    LoadDB2(bad_db2_files, sOverrideSpellDataStore,         db2Path, "OverrideSpellData.db2");
    LoadDB2(bad_db2_files, sSpellMiscStore,                 db2Path, "SpellMisc.db2");
    LoadDB2(bad_db2_files, sSpellPowerStore,                db2Path, "SpellPower.db2");
    LoadDB2(bad_db2_files, sSpellTotemsStore,               db2Path, "SpellTotems.db2");
    LoadDB2(bad_db2_files, sSpellClassOptionsStore,         db2Path, "SpellClassOptions.db2");

    //////////////////////////////////////////////////////////////////////////
    /// Garrison DB2
    //////////////////////////////////////////////////////////////////////////
    LoadDB2(bad_db2_files, sGarrSiteLevelStore,             db2Path, "GarrSiteLevel.db2");
    LoadDB2(bad_db2_files, sGarrSiteLevelPlotInstStore,     db2Path, "GarrSiteLevelPlotInst.db2");
    LoadDB2(bad_db2_files, sGarrPlotInstanceStore,          db2Path, "GarrPlotInstance.db2");
    LoadDB2(bad_db2_files, sGarrPlotStore,                  db2Path, "GarrPlot.db2");
    LoadDB2(bad_db2_files, sGarrPlotUICategoryStore,        db2Path, "GarrPlotUICategory.db2");
    LoadDB2(bad_db2_files, sGarrMissionStore,               db2Path, "GarrMission.db2");
    LoadDB2(bad_db2_files, sGarrMissionRewardStore,         db2Path, "GarrMissionReward.db2");
    LoadDB2(bad_db2_files, sGarrMissionXEncouterStore,      db2Path, "GarrMissionXEncounter.db2");
    LoadDB2(bad_db2_files, sGarrBuildingStore,              db2Path, "GarrBuilding.db2");
    LoadDB2(bad_db2_files, sGarrPlotBuildingStore,          db2Path, "GarrPlotBuilding.db2");
    LoadDB2(bad_db2_files, sGarrFollowerStore,              db2Path, "GarrFollower.db2");
    LoadDB2(bad_db2_files, sGarrAbilityStore,               db2Path, "GarrAbility.db2");
    LoadDB2(bad_db2_files, sGarrAbilityEffectStore,         db2Path, "GarrAbilityEffect.db2");
    LoadDB2(bad_db2_files, sGarrFollowerXAbilityStore,      db2Path, "GarrFollowerXAbility.db2");
    LoadDB2(bad_db2_files, sGarrBuildingPlotInstStore,      db2Path, "GarrBuildingPlotInst.db2");
    LoadDB2(bad_db2_files, sGarrMechanicTypeStore,          db2Path, "GarrMechanicType.db2");
    LoadDB2(bad_db2_files, sGarrMechanicStore,              db2Path, "GarrMechanic.db2");
    LoadDB2(bad_db2_files, sGarrEncouterXMechanicStore,     db2Path, "GarrEncounterXMechanic.db2");
    LoadDB2(bad_db2_files, sGarrFollowerLevelXPStore,       db2Path, "GarrFollowerLevelXP.db2");
    LoadDB2(bad_db2_files, sGarrSpecializationStore,        db2Path, "GarrSpecialization.db2");
    LoadDB2(bad_db2_files, sCharShipmentStore,              db2Path, "CharShipment.db2");
    LoadDB2(bad_db2_files, sCharShipmentContainerStore,     db2Path, "CharShipmentContainer.db2");

    //////////////////////////////////////////////////////////////////////////
    /// Battle pet DB2
    //////////////////////////////////////////////////////////////////////////
    LoadDB2(bad_db2_files, sBattlePetAbilityStore,          db2Path, "BattlePetAbility.db2");
    LoadDB2(bad_db2_files, sBattlePetAbilityEffectStore,    db2Path, "BattlePetAbilityEffect.db2");
    LoadDB2(bad_db2_files, sBattlePetAbilityTurnStore,      db2Path, "BattlePetAbilityTurn.db2");
    LoadDB2(bad_db2_files, sBattlePetAbilityStateStore,     db2Path, "BattlePetAbilityState.db2");
    LoadDB2(bad_db2_files, sBattlePetStateStore,            db2Path, "BattlePetState.db2");
    LoadDB2(bad_db2_files, sBattlePetEffectPropertiesStore, db2Path, "BattlePetEffectProperties.db2");
    LoadDB2(bad_db2_files, sBattlePetBreedQualityStore,     db2Path, "BattlePetBreedQuality.db2");
    LoadDB2(bad_db2_files, sBattlePetBreedStateStore,       db2Path, "BattlePetBreedState.db2");
    LoadDB2(bad_db2_files, sBattlePetSpeciesStore,          db2Path, "BattlePetSpecies.db2");
    LoadDB2(bad_db2_files, sBattlePetSpeciesStateStore,     db2Path, "BattlePetSpeciesState.db2");
    LoadDB2(bad_db2_files, sBattlePetSpeciesXAbilityStore,  db2Path, "BattlePetSpeciesXAbility.db2");

    std::set<uint32> scalingCurves;
    for (uint32 i = 0; i < sScalingStatDistributionStore.GetNumRows(); ++i)
        if (ScalingStatDistributionEntry const* ssd = sScalingStatDistributionStore.LookupEntry(i))
            scalingCurves.insert(ssd->CurveProperties);

    for (uint32 i = 0; i < sCurvePointStore.GetNumRows(); ++i)
        if (CurvePointEntry const* curvePoint = sCurvePointStore.LookupEntry(i))
            if (scalingCurves.count(curvePoint->CurveID))
                HeirloomCurvePoints[curvePoint->CurveID][curvePoint->Index] = curvePoint;

    for (uint32 l_Y = 0; l_Y < sItemModifiedAppearanceStore.GetNumRows(); l_Y++)
    {
        if (const ItemModifiedAppearanceEntry * l_ModifiedAppearanceEntry = sItemModifiedAppearanceStore.LookupEntry(l_Y))
        {
            if (l_ModifiedAppearanceEntry->ItemID != 0 && l_ModifiedAppearanceEntry->Index == 0)
            {
                const ItemAppearanceEntry * l_AppearanceEntry = sItemAppearanceStore.LookupEntry(l_ModifiedAppearanceEntry->AppearanceID);

                uint32 l_DisplayID = 0;

                if (l_AppearanceEntry)
                    l_DisplayID = l_AppearanceEntry->DisplayID;

                ItemEntry * l_Entry = const_cast<ItemEntry*>(sItemStore.LookupEntry(l_ModifiedAppearanceEntry->ItemID));

                if (l_Entry)
                    l_Entry->DisplayId = l_DisplayID;
            }
        }
    }

    for (uint32 l_I = 1; l_I < sItemEffectStore.GetNumRows(); ++l_I)
    {
        if (ItemEffectEntry const* l_Entry = sItemEffectStore.LookupEntry(l_I))
            sItemEffectsByItemID[l_Entry->ItemID].push_back(l_I);
    }

    for (uint32 l_I = 0; l_I < sItemBonusStore.GetNumRows(); l_I++)
    {
        ItemBonusEntry const* l_Entry = sItemBonusStore.LookupEntry(l_I);
        if (!l_Entry)
            continue;

        if (!sItemBonusesByID[l_Entry->Entry].size())
        {
            sItemBonusesByID[l_Entry->Entry].resize(MAX_ITEM_BONUS);
            for (int l_J = 0; l_J < MAX_ITEM_BONUS; l_J++)
                sItemBonusesByID[l_Entry->Entry][l_J] = nullptr;
        }

        sItemBonusesByID[l_Entry->Entry][l_Entry->Index] = l_Entry;
    }

    /// - Load quest package items
    for (uint32 l_I = 0; l_I < sQuestPackageItemStore.GetNumRows(); l_I++)
    {
        QuestPackageItemEntry const* l_QuestPackageItem = sQuestPackageItemStore.LookupEntry(l_I);
        if (l_QuestPackageItem == nullptr)
            continue;

        sQuestPackageItemsByGroup[l_QuestPackageItem->PackageID].push_back(l_QuestPackageItem);
    }

    /// - Load Item Bonus Tree
    for (uint32 l_I = 0; l_I < sItemXBonusTreeStore.GetNumRows(); l_I++)
    {
        auto l_ItemXBonusTree = sItemXBonusTreeStore.LookupEntry(l_I);
        if (l_ItemXBonusTree == nullptr)
            continue;

        sItemBonusTreeByID[l_ItemXBonusTree->ItemId].push_back(l_ItemXBonusTree);
    }

    // Initialize global taxinodes mask
    // Include existed nodes that have at least single not spell base (scripted) path
    {
        std::set<uint32> spellPaths;
        for (uint32 i = 1; i < sSpellEffectStore.GetNumRows(); ++i)
            if (SpellEffectEntry const* sInfo = sSpellEffectStore.LookupEntry (i))
                if (sInfo->Effect == SPELL_EFFECT_SEND_TAXI)
                    spellPaths.insert(sInfo->EffectMiscValue);

        memset(sTaxiNodesMask, 0, sizeof(sTaxiNodesMask));
        memset(sOldContinentsNodesMask, 0, sizeof(sOldContinentsNodesMask));
        memset(sHordeTaxiNodesMask, 0, sizeof(sHordeTaxiNodesMask));
        memset(sAllianceTaxiNodesMask, 0, sizeof(sAllianceTaxiNodesMask));
        memset(sDeathKnightTaxiNodesMask, 0, sizeof(sDeathKnightTaxiNodesMask));
        for (uint32 i = 1; i < sTaxiNodesStore.GetNumRows(); ++i)
        {
            TaxiNodesEntry const* node = sTaxiNodesStore.LookupEntry(i);
            if (!node)
                continue;

            // Needed for getting the current flight path
            sTaxiNodesByMap[node->map_id].push_back(node);

            TaxiPathSetBySource::const_iterator src_i = sTaxiPathSetBySource.find(i);
            if (src_i != sTaxiPathSetBySource.end() && !src_i->second.empty())
            {
                bool ok = false;
                for (TaxiPathSetForSource::const_iterator dest_i = src_i->second.begin(); dest_i != src_i->second.end(); ++dest_i)
                {
                    // Not spell path
                    if (spellPaths.find(dest_i->second.ID) == spellPaths.end())
                    {
                        ok = true;
                        break;
                    }
                }

                if (!ok)
                    continue;
            }

            // Valid taxi network node
            uint8  field   = (uint8)((i - 1) / 8);
            uint32 submask = 1 << ((i-1) % 8);

            sTaxiNodesMask[field] |= submask;
            if (node->MountCreatureID[0] && node->MountCreatureID[0] != 32981)
                sHordeTaxiNodesMask[field] |= submask;
            if (node->MountCreatureID[1] && node->MountCreatureID[1] != 32981)
                sAllianceTaxiNodesMask[field] |= submask;
            if (node->MountCreatureID[0] == 32981 || node->MountCreatureID[1] == 32981)
                sDeathKnightTaxiNodesMask[field] |= submask;

            // Old continent node (+ nodes virtually at old continents, check explicitly to avoid loading map files for zone info)
            if (node->map_id < 2 || i == 82 || i == 83 || i == 93 || i == 94)
                sOldContinentsNodesMask[field] |= submask;

            // Fix DK node at Ebon Hold and Shadow Vault flight master
            if (i == 315 || i == 333)
                ((TaxiNodesEntry*)node)->MountCreatureID[1] = 32981;
        }
    }


    uint32 pathCount = sTaxiPathStore.GetNumRows();

    for (uint32 i = 1; i < pathCount; ++i)
        if (TaxiPathEntry const* entry = sTaxiPathStore.LookupEntry(i))
            sTaxiPathSetBySource[entry->from][entry->to] = TaxiPathBySourceAndDestination(entry->ID, entry->price);

    // Calculate path nodes count
    std::vector<uint32> pathLength;
    pathLength.resize(pathCount);                           // 0 and some other indexes not used
    for (uint32 l_I = 1; l_I < sTaxiPathNodeStore.GetNumRows(); ++l_I)
    if (TaxiPathNodeEntry const* entry = sTaxiPathNodeStore.LookupEntry(l_I))
    {
        if (pathLength[entry->path] < entry->index + 1)
            pathLength[entry->path] = entry->index + 1;
    }

    // Set path length
    sTaxiPathNodesByPath.resize(pathCount);                 // 0 and some other indexes not used
    for (uint32 l_I = 1; l_I < sTaxiPathNodesByPath.size(); ++l_I)
        sTaxiPathNodesByPath[l_I].resize(pathLength[l_I]);

    // fill data
    for (uint32 l_I = 1; l_I < sTaxiPathNodeStore.GetNumRows(); ++l_I)
        if (TaxiPathNodeEntry const* entry = sTaxiPathNodeStore.LookupEntry(l_I))
            sTaxiPathNodesByPath[entry->path].set(entry->index, entry);

    for (uint32 l_ID = 0; l_ID < sHeirloomStore.GetNumRows(); ++l_ID)
    {
        HeirloomEntry const* l_Heirloom = sHeirloomStore.LookupEntry(l_ID);

        if (!l_Heirloom)
            continue;

        HeirloomEntryByItemID.insert({ l_Heirloom->ItemID, l_Heirloom });

        for (uint32 l_X = 0; l_X < 2; l_X++)
            if (uint32 l_OlderItemID = l_Heirloom->OldHeirloomID[l_X])
                HeirloomEntryByItemID.insert({ l_OlderItemID, l_Heirloom});

        if (uint32 l_HeroicID = l_Heirloom->HeroicVersion)
            HeirloomEntryByItemID.insert({ l_HeroicID, l_Heirloom});
    }

    // error checks
    if (bad_db2_files.size() >= DB2FilesCount)
    {
        sLog->outError(LOG_FILTER_GENERAL, "\nIncorrect DataDir value in worldserver.conf or ALL required *.db2 files (%d) not found by path: %sdb2", DB2FilesCount, dataPath.c_str());
        exit(1);
    }
    else if (!bad_db2_files.empty())
    {
        std::string str;
        for (std::list<std::string>::iterator i = bad_db2_files.begin(); i != bad_db2_files.end(); ++i)
            str += *i + "\n";

        sLog->outError(LOG_FILTER_GENERAL, "\nSome required *.db2 files (%u from %d) not found or not compatible:\n%s", (uint32)bad_db2_files.size(), DB2FilesCount,str.c_str());
        exit(1);
    }

    // Check loaded DB2 files proper version
    if (!sItemStore.LookupEntry(106130)             ||      // last item added in 5.4 (17371)
        !sItemExtendedCostStore.LookupEntry(5268)  )        // last item extended cost added in 5.4 (17371)
    {
        sLog->outError(LOG_FILTER_GENERAL, "Please extract correct db2 files from client 5.4 17371.");
        exit(1);
    }

    sLog->outInfo(LOG_FILTER_GENERAL, ">> Initialized %d DB2 data stores.", DB2FilesCount);
}

std::vector<ItemBonusEntry const*> const* GetItemBonusesByID(uint32 Id)
{
    std::map<uint32, std::vector<ItemBonusEntry const*>>::const_iterator iter = sItemBonusesByID.find(Id);
    return iter != sItemBonusesByID.end() ? &(iter->second) : nullptr;
}

uint32 GetHeirloomItemLevel(uint32 curveId, uint32 level)
{
    // Assuming linear item level scaling for heirlooms
    auto itr = HeirloomCurvePoints.find(curveId);
    if (itr == HeirloomCurvePoints.end())
        return 0;

    auto it2 = itr->second.begin(); // Highest scaling point
    if (level >= it2->second->X)
        return it2->second->Y;

    auto previousItr = it2++;
    for (; it2 != itr->second.end(); ++it2, ++previousItr)
        if (level >= it2->second->X)
            return uint32((previousItr->second->Y - it2->second->Y) / (previousItr->second->X - it2->second->X) * (float(level) - it2->second->X) + it2->second->Y);

    return uint32(previousItr->second->Y);  // Lowest scaling point
}

HeirloomEntry const* GetHeirloomEntryByItemID(uint32 p_ItemID)
{
    std::unordered_map<uint32, HeirloomEntry const*>::const_iterator l_Iter =  HeirloomEntryByItemID.find(p_ItemID);
    return l_Iter != HeirloomEntryByItemID.end() ? l_Iter->second : nullptr;
};

std::vector<TaxiNodesEntry const*> const* GetTaxiNodesForMapId(uint32 l_MapID)
{
    TaxiNodesByMap::const_iterator l_Iter = sTaxiNodesByMap.find(l_MapID);
    return l_Iter != sTaxiNodesByMap.end() ? &l_Iter->second : nullptr;
}

// basic pathing algorithm which paths from the start to the destination
uint32 TaxiPath::CalculateTaxiPath(uint32 startId, uint32 destId, Player* player)
{
    clear();

    uint32 res = 0;
    TaxiNode* startNode = sObjectMgr->GetTaxiNodeByID(startId);
    TaxiNode* destNode = sObjectMgr->GetTaxiNodeByID(destId);
    if (!startNode || !destNode)
    {
        res |= TAXIPATH_RES_NO_LINKED_NODES;
        return res;
    }

    push_back(startNode);

    TaxiNode* currentNode = startNode;
    std::set<uint32> closed;
    while (1)
    {
        // path complete
        if (currentNode == destNode)
            break;

        if (currentNode == startNode && !closed.empty())
        {
            res |= TAXIPATH_RES_NO_PATH;
            break;
        }

        TaxiNode* nextNode = currentNode->GetClosestNodeTo(destNode, closed, player);

        if (!nextNode)
        {
            closed.insert(currentNode->GetID());
            currentNode = back();
            pop_back();
            continue;
        }

        // 6.1.0: players can path to unknown nodes if it is on the way to their dest
        //if (!player->KnowsNode(nextNode))
        //{
            //closed.insert(nextNode->GetID())
            //continue;
        //}

        closed.insert(currentNode->GetID());
        push_back(nextNode);
        currentNode = nextNode;
    }

    if (res & TAXIPATH_RES_NO_PATH)
    {
        clear();
        push_back(startNode);
        push_back(destNode);
        return res;
    }

    res |= TAXIPATH_RES_SUCCESS;
    //push_back(destNode);

    return res;
}

TaxiNode* TaxiNode::GetClosestNodeTo(TaxiNode* node, std::set<uint32>& closed, Player* player)
{
    float dist = -1.f;//GetPosition()->GetExactDist2d(node->GetPosition());
    TaxiNode* heuristic = nullptr;
    for (std::set<uint32>::iterator itr = m_connectedNodes.begin(); itr != m_connectedNodes.end(); ++itr)
    {
        TaxiNode* connectedNode = sObjectMgr->GetTaxiNodeByID(*itr);
        TaxiNodesEntry const* l_TaxiNode = node->GetTaxiNodesEntry();

        if (node == connectedNode)
        {
            dist = 0.0f;
            heuristic = connectedNode;
            break;
        }

        // I don't think this is possible
        if (!connectedNode || !l_TaxiNode)
            continue;

        uint8  field   = (uint8)((*itr - 1) / 8);
        uint32 submask = 1 << ((*itr-1) % 8);

        if (!l_TaxiNode->MountCreatureID[player->GetTeam() == ALLIANCE ? 1 : 0] && !(sDeathKnightTaxiNodesMask[field] & submask && player->getClass() == CLASS_DEATH_KNIGHT)) // dk flight)
        {
            closed.insert(connectedNode->GetID());
            continue;
        }

        // skip not taxi network nodes
        if ((sTaxiNodesMask[field] & submask) == 0)
        {
            closed.insert(connectedNode->GetID());
            continue;
        }

        /// All taxi path with flag == 0 is quest taxi, event or transport, we can skip it
        if (l_TaxiNode->m_Flags == 0)
        {
            closed.insert(connectedNode->GetID());
            continue;
        }

        if (closed.find(*itr) != closed.end())
            continue;

        float nodeDist = connectedNode->GetPosition()->GetExactDist2d(node->GetPosition());

        if (nodeDist < dist || dist < 0.f)
        {
            dist = nodeDist;
            heuristic = connectedNode;
        }
    }

    return heuristic;
}
