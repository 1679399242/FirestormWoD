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
#include "Item.h"
#include "ObjectMgr.h"
#include "WorldPacket.h"
#include "DatabaseEnv.h"
#include "ItemEnchantmentMgr.h"
#include "SpellMgr.h"
#include "SpellInfo.h"
#include "ScriptMgr.h"
#include "ConditionMgr.h"
#include "DB2Stores.h"

// @issue : #260
void AddItemsSetItem(Player* player, Item* item)
{
    ItemTemplate const* proto = item->GetTemplate();
    uint32 setid = proto->ItemSet;

    ItemSetEntry const* set = sItemSetStore.LookupEntry(setid);

    if (!set)
    {
        sLog->outError(LOG_FILTER_SQL, "Item set %u for item (id %u) not found, mods not applied.", setid, proto->ItemId);
        return;
    }

    ItemSetSpellsByItemID::const_iterator iter = sItemSetSpellsByItemIDStore.find(setid);
    if (iter == sItemSetSpellsByItemIDStore.end())
        return;

    std::list<ItemSetSpellEntry const*> const& setSpells = iter->second;

    if (set->required_skill_id && player->GetSkillValue(set->required_skill_id) < set->required_skill_value)
        return;

    ItemSetEffect* eff = NULL;

    for (size_t x = 0; x < player->ItemSetEff.size(); ++x)
    {
        if (player->ItemSetEff[x] && player->ItemSetEff[x]->setid == setid)
        {
            eff = player->ItemSetEff[x];
            break;
        }
    }

    if (!eff)
    {
        eff = new ItemSetEffect();
        eff->setid = setid;

        size_t x = 0;
        for (; x < player->ItemSetEff.size(); ++x)
            if (!player->ItemSetEff[x])
                break;

        if (x < player->ItemSetEff.size())
            player->ItemSetEff[x]=eff;
        else
            player->ItemSetEff.push_back(eff);
    }

    ++eff->item_count;

    int i = 0;
    for (auto setSpell : setSpells)
    {
        if (!setSpell->SpellID)
            continue;

        if (setSpell->SpecializationID && setSpell->SpecializationID != player->GetSpecializationId(player->GetActiveSpec()))
            continue;

        //not enough for  spell
        if (setSpell->PieceRequirement > eff->item_count)

        if (i < MAX_ITEM_SET_SPELLS)
            continue;

          //new spell
        for (uint32 y = 0; y < MAX_ITEM_SET_SPELLS; ++y)
        {
            if (!eff->spells[y])                             // free slot
            {
                SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(setSpell->SpellID);
                if (!spellInfo)
                {
                    sLog->outError(LOG_FILTER_PLAYER_ITEMS, "WORLD: unknown spell id %u in items set %u effects", setSpell->SpellID, setid);
                    break;
                }

                // spell casted only if fit form requirement, in other case will casted at form change
                player->ApplyEquipSpell(spellInfo, NULL, true);
                eff->spells[y] = spellInfo;
                break;
            }
        }
    }
}

void RemoveItemsSetItem(Player*player, ItemTemplate const* proto)
{
    uint32 setid = proto->ItemSet;

    ItemSetEntry const* set = sItemSetStore.LookupEntry(setid);

    if (!set)
    {
        sLog->outError(LOG_FILTER_SQL, "Item set #%u for item #%u not found, mods not removed.", setid, proto->ItemId);
        return;
    }

    ItemSetEffect* eff = NULL;
    size_t setindex = 0;
    for (; setindex < player->ItemSetEff.size(); setindex++)
    {
        if (player->ItemSetEff[setindex] && player->ItemSetEff[setindex]->setid == setid)
        {
            eff = player->ItemSetEff[setindex];
            break;
        }
    }

    // can be in case now enough skill requirement for set appling but set has been appliend when skill requirement not enough
    if (!eff)
        return;

    --eff->item_count;

    ItemSetSpellsByItemID::const_iterator iter = sItemSetSpellsByItemIDStore.find(setid);
    if (iter == sItemSetSpellsByItemIDStore.end())
        return;

    std::list<ItemSetSpellEntry const*> const& setSpells = iter->second;

    for (auto setSpell : setSpells)
    {
        if (!setSpell->SpellID)
            continue;

        if (setSpell->SpecializationID && setSpell->SpecializationID != player->GetSpecializationId(player->GetActiveSpec()))
            continue;

        // enough for spell
        if (setSpell->PieceRequirement <= eff->item_count)
            continue;

        for (uint32 z = 0; z < MAX_ITEM_SET_SPELLS; z++)
        {
            if (eff->spells[z] && eff->spells[z]->Id == setSpell->SpellID)
            {
                // spell can be not active if not fit form requirement
                player->ApplyEquipSpell(eff->spells[z], NULL, false);
                eff->spells[z]=NULL;
                break;
            }
        }
    }

    if (!eff->item_count)                                    //all items of a set were removed
    {
        ASSERT(eff == player->ItemSetEff[setindex]);
        delete eff;
        player->ItemSetEff[setindex] = NULL;
    }
}

bool ItemCanGoIntoBag(ItemTemplate const* pProto, ItemTemplate const* pBagProto)
{
    if (!pProto || !pBagProto)
        return false;

    switch (pBagProto->Class)
    {
        case ITEM_CLASS_CONTAINER:
            switch (pBagProto->SubClass)
            {
                case ITEM_SUBCLASS_CONTAINER:
                    return true;
                case ITEM_SUBCLASS_SOUL_CONTAINER:
                    if (!(pProto->BagFamily & BAG_FAMILY_MASK_SOUL_SHARDS))
                        return false;
                    return true;
                case ITEM_SUBCLASS_HERB_CONTAINER:
                    if (!(pProto->BagFamily & BAG_FAMILY_MASK_HERBS))
                        return false;
                    return true;
                case ITEM_SUBCLASS_ENCHANTING_CONTAINER:
                    if (!(pProto->BagFamily & BAG_FAMILY_MASK_ENCHANTING_SUPPLIES))
                        return false;
                    return true;
                case ITEM_SUBCLASS_MINING_CONTAINER:
                    if (!(pProto->BagFamily & BAG_FAMILY_MASK_MINING_SUPPLIES))
                        return false;
                    return true;
                case ITEM_SUBCLASS_ENGINEERING_CONTAINER:
                    if (!(pProto->BagFamily & BAG_FAMILY_MASK_ENGINEERING_SUPPLIES))
                        return false;
                    return true;
                case ITEM_SUBCLASS_GEM_CONTAINER:
                    if (!(pProto->BagFamily & BAG_FAMILY_MASK_GEMS))
                        return false;
                    return true;
                case ITEM_SUBCLASS_LEATHERWORKING_CONTAINER:
                    if (!(pProto->BagFamily & BAG_FAMILY_MASK_LEATHERWORKING_SUPPLIES))
                        return false;
                    return true;
                case ITEM_SUBCLASS_INSCRIPTION_CONTAINER:
                    if (!(pProto->BagFamily & BAG_FAMILY_MASK_INSCRIPTION_SUPPLIES))
                        return false;
                    return true;
                case ITEM_SUBCLASS_TACKLE_CONTAINER:
                    if (!(pProto->BagFamily & BAG_FAMILY_MASK_FISHING_SUPPLIES))
                        return false;
                    return true;
                case ITEM_SUBCLASS_COOKING_CONTAINER:
                    if (!(pProto->BagFamily & BAG_FAMILY_MASK_COOKING_SUPPLIES))
                        return false;
                    return true;
                default:
                    return false;
            }
        case ITEM_CLASS_QUIVER:
            switch (pBagProto->SubClass)
            {
                case ITEM_SUBCLASS_QUIVER:
                    if (!(pProto->BagFamily & BAG_FAMILY_MASK_ARROWS))
                        return false;
                    return true;
                case ITEM_SUBCLASS_AMMO_POUCH:
                    if (!(pProto->BagFamily & BAG_FAMILY_MASK_BULLETS))
                        return false;
                    return true;
                default:
                    return false;
            }
    }
    return false;
}

Item::Item()
{
    m_objectType |= TYPEMASK_ITEM;
    m_objectTypeId = TYPEID_ITEM;

    m_updateFlag = 0;

    m_valuesCount = ITEM_END;
    m_slot = 0;
    uState = ITEM_NEW;
    uQueuePos = -1;
    m_container = NULL;
    m_lootGenerated = false;
    mb_in_trade = false;
    m_lastPlayedTimeUpdate = time(NULL);

    m_refundRecipient = 0;
    m_paidMoney = 0;
    m_paidExtendedCost = 0;

    // Fuck default constructor, i don't trust it
    m_text = "";

    _dynamicValuesCount = ITEM_DYNAMIC_END;
}

Item::~Item()
{
}

bool Item::Create(uint32 guidlow, uint32 itemid, Player const* owner)
{
    Object::_Create(guidlow, 0, HIGHGUID_ITEM);

    SetEntry(itemid);
    SetObjectScale(1.0f);

    SetGuidValue(ITEM_FIELD_OWNER, owner ? owner->GetGUID() : 0);
    SetGuidValue(ITEM_FIELD_CONTAINED_IN, owner ? owner->GetGUID() : 0);

    ItemTemplate const* itemProto = sObjectMgr->GetItemTemplate(itemid);
    if (!itemProto)
        return false;

    loot.SetSource(GetGUID());

    // For Item Upgrade
    /*if (CanUpgrade())
    {
        if (IsPvPItem())
        {
            if (itemProto->Quality == ITEM_QUALITY_EPIC)
                SetDynamicUInt32Value(ITEM_DYNAMIC_MODIFIERS, 2, 456);
            else
                SetDynamicUInt32Value(ITEM_DYNAMIC_MODIFIERS, 2, 453);
        }
        else
        {
            if (itemProto->Quality == ITEM_QUALITY_EPIC)
                SetDynamicUInt32Value(ITEM_DYNAMIC_MODIFIERS, 2, 445);
            else if (itemProto->Quality == ITEM_QUALITY_LEGENDARY)
                SetDynamicUInt32Value(ITEM_DYNAMIC_MODIFIERS, 2, 465);
            else
                SetDynamicUInt32Value(ITEM_DYNAMIC_MODIFIERS, 2, 451);
        }

        SetFlag(ITEM_FIELD_MODIFIERS_MASK, 0x1 | ITEM_TRANSMOGRIFIED | 0x4);
    }*/

    SetUInt32Value(ITEM_FIELD_STACK_COUNT, 1);
    SetUInt32Value(ITEM_FIELD_MAX_DURABILITY, itemProto->MaxDurability);
    SetUInt32Value(ITEM_FIELD_DURABILITY, itemProto->MaxDurability);

    for (uint8 i = 0; i < MAX_ITEM_PROTO_SPELLS; ++i)
        SetSpellCharges(i, itemProto->Spells[i].SpellCharges);

    SetUInt32Value(ITEM_FIELD_EXPIRATION, itemProto->Duration);
    SetUInt32Value(ITEM_FIELD_CREATE_PLAYED_TIME, 0);
    return true;
}

// Returns true if Item is a bag AND it is not empty.
// Returns false if Item is not a bag OR it is an empty bag.
bool Item::IsNotEmptyBag() const
{
    if (Bag const* bag = ToBag())
        return !bag->IsEmpty();
    return false;
}

void Item::UpdateDuration(Player* owner, uint32 diff)
{
    if (!GetUInt32Value(ITEM_FIELD_EXPIRATION))
        return;

    sLog->outDebug(LOG_FILTER_PLAYER_ITEMS, "Item::UpdateDuration Item (Entry: %u Duration %u Diff %u)", GetEntry(), GetUInt32Value(ITEM_FIELD_EXPIRATION), diff);

    if (GetUInt32Value(ITEM_FIELD_EXPIRATION) <= diff)
    {
        sScriptMgr->OnItemExpire(owner, GetTemplate());
        owner->DestroyItem(GetBagSlot(), GetSlot(), true);
        return;
    }

    SetUInt32Value(ITEM_FIELD_EXPIRATION, GetUInt32Value(ITEM_FIELD_EXPIRATION) - diff);
    SetState(ITEM_CHANGED, owner);                          // save new time in database
}

void Item::SaveToDB(SQLTransaction& trans)
{
    bool isInTransaction = !(trans.null());
    if (!isInTransaction)
        trans = CharacterDatabase.BeginTransaction();

    uint32 guid = GetGUIDLow();
    switch (uState)
    {
        case ITEM_NEW:
        case ITEM_CHANGED:
        {
            uint8 index = 0;
            PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(uState == ITEM_NEW ? CHAR_REP_ITEM_INSTANCE : CHAR_UPD_ITEM_INSTANCE);
            stmt->setUInt32(  index, GetEntry());
            stmt->setUInt32(++index, GUID_LOPART(GetOwnerGUID()));
            stmt->setUInt32(++index, GUID_LOPART(GetGuidValue(ITEM_FIELD_CREATOR)));
            stmt->setUInt32(++index, GUID_LOPART(GetGuidValue(ITEM_FIELD_GIFT_CREATOR)));
            stmt->setUInt32(++index, GetCount());
            stmt->setUInt32(++index, GetUInt32Value(ITEM_FIELD_EXPIRATION));

            std::ostringstream ssSpells;
            for (uint8 i = 0; i < MAX_ITEM_PROTO_SPELLS; ++i)
                ssSpells << GetSpellCharges(i) << ' ';
            stmt->setString(++index, ssSpells.str());

            stmt->setUInt32(++index, GetUInt32Value(ITEM_FIELD_DYNAMIC_FLAGS));

            std::ostringstream ssEnchants;
            for (uint8 i = 0; i < MAX_ENCHANTMENT_SLOT; ++i)
            {
                ssEnchants << GetEnchantmentId(EnchantmentSlot(i)) << ' ';
                ssEnchants << GetEnchantmentDuration(EnchantmentSlot(i)) << ' ';
                ssEnchants << GetEnchantmentCharges(EnchantmentSlot(i)) << ' ';
            }
            stmt->setString(++index, ssEnchants.str());

            stmt->setInt16 (++index, GetItemRandomPropertyId());
            stmt->setUInt32(++index, GetDynamicValue(ITEM_DYNAMIC_FIELD_MODIFIERS, 0));

            std::ostringstream ssBonuses;
            std::vector<uint32> bonuses = GetAllItemBonuses();
            for (uint8 i = 0; i < bonuses.size(); ++i)
            {
                if (!bonuses[i])
                    continue;

                ssBonuses << bonuses[i] << ' ';
            }

            stmt->setString(++index, ssBonuses.str());
            stmt->setUInt32(++index, 0/*GetDynamicUInt32Value(ITEM_DYNAMIC_MODIFIERS, 2)*/); // itemUpgrade Id
            stmt->setUInt16(++index, GetUInt32Value(ITEM_FIELD_DURABILITY));
            stmt->setUInt32(++index, GetUInt32Value(ITEM_FIELD_CREATE_PLAYED_TIME));
            stmt->setString(++index, m_text);
            stmt->setUInt32(++index, guid);

            trans->Append(stmt);

            if ((uState == ITEM_CHANGED) && HasFlag(ITEM_FIELD_DYNAMIC_FLAGS, ITEM_FLAG_WRAPPED))
            {
                stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_GIFT_OWNER);
                stmt->setUInt32(0, GUID_LOPART(GetOwnerGUID()));
                stmt->setUInt32(1, guid);
                trans->Append(stmt);
            }
            break;
        }
        case ITEM_REMOVED:
        {
            PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_ITEM_INSTANCE);
            stmt->setUInt32(0, guid);
            trans->Append(stmt);

            if (HasFlag(ITEM_FIELD_DYNAMIC_FLAGS, ITEM_FLAG_WRAPPED))
            {
                stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GIFT);
                stmt->setUInt32(0, guid);
                trans->Append(stmt);
            }

            if (!isInTransaction)
                CharacterDatabase.CommitTransaction(trans);

            delete this;
            return;
        }
        case ITEM_UNCHANGED:
            break;
    }

    SetState(ITEM_UNCHANGED);

    if (!isInTransaction)
        CharacterDatabase.CommitTransaction(trans);
}

bool Item::LoadFromDB(uint32 guid, uint64 owner_guid, Field* fields, uint32 entry)
{
    //                                              0                1          2       3        4        5         6               7              8            9            10          11         12
    //result = CharacterDatabase.PQuery("SELECT creatorGuid, giftCreatorGuid, count, duration, charges, flags, enchantments, randomPropertyId, transmogrifyId, bonuses, upgradeId, durability, playedTime, text FROM item_instance WHERE guid = '%u'", guid);

    // create item before any checks for store correct guid
    // and allow use "FSetState(ITEM_REMOVED); SaveToDB();" for deleting item from DB
    Object::_Create(guid, 0, HIGHGUID_ITEM);

    // Set entry, MUST be before proto check
    SetEntry(entry);
    SetObjectScale(1.0f);

    ItemTemplate const* proto = GetTemplate();
    if (!proto)
        return false;

    // set owner (not if item is only loaded for gbank/auction/mail
    if (owner_guid != 0)
        SetOwnerGUID(owner_guid);

    bool need_save = false;                                 // need explicit save data at load fixes
    SetGuidValue(ITEM_FIELD_CREATOR, MAKE_NEW_GUID(fields[0].GetUInt32(), 0, HIGHGUID_PLAYER));
    SetGuidValue(ITEM_FIELD_GIFT_CREATOR, MAKE_NEW_GUID(fields[1].GetUInt32(), 0, HIGHGUID_PLAYER));
    SetCount(fields[2].GetUInt32());

    uint32 duration = fields[3].GetUInt32();
    SetUInt32Value(ITEM_FIELD_EXPIRATION, duration);
    // update duration if need, and remove if not need
    if ((proto->Duration == 0) != (duration == 0))
    {
        SetUInt32Value(ITEM_FIELD_EXPIRATION, proto->Duration);
        need_save = true;
    }

    Tokenizer tokens(fields[4].GetString(), ' ', MAX_ITEM_PROTO_SPELLS);
    if (tokens.size() == MAX_ITEM_PROTO_SPELLS)
        for (uint8 i = 0; i < MAX_ITEM_PROTO_SPELLS; ++i)
            SetSpellCharges(i, atoi(tokens[i]));

    SetUInt32Value(ITEM_FIELD_DYNAMIC_FLAGS, fields[5].GetUInt32());
    // Remove bind flag for items vs NO_BIND set
    if (IsSoulBound() && proto->Bonding == NO_BIND)
    {
        ApplyModFlag(ITEM_FIELD_DYNAMIC_FLAGS, ITEM_FLAG_SOULBOUND, false);
        need_save = true;
    }

    std::string enchants = fields[6].GetString();
    _LoadIntoDataField(enchants.c_str(), ITEM_FIELD_ENCHANTMENT, MAX_ENCHANTMENT_SLOT * MAX_ENCHANTMENT_OFFSET);

    if (uint32 transmogId = fields[8].GetInt32())
    {
        SetDynamicValue(ITEM_DYNAMIC_FIELD_MODIFIERS, 0, transmogId);
        SetFlag(ITEM_FIELD_MODIFIERS_MASK, ITEM_TRANSMOGRIFIED);
    }

    Tokenizer bonusTokens(fields[9].GetString(), ' ');
        for (uint8 i = 0; i < bonusTokens.size(); ++i)
            AddItemBonus(atoi(bonusTokens[i]));

    // uint32 upgradeId = fields[10].GetUInt32(); @TODO: Remove this DB field

    SetInt32Value(ITEM_FIELD_RANDOM_PROPERTIES_ID, fields[7].GetInt16());
    // recalculate suffix factor
    if (GetItemRandomPropertyId() < 0)
        UpdateItemSuffixFactor();

    uint32 durability = fields[11].GetUInt16();
    SetUInt32Value(ITEM_FIELD_DURABILITY, durability);
    // update max durability (and durability) if need
    SetUInt32Value(ITEM_FIELD_MAX_DURABILITY, proto->MaxDurability);
    if (durability > proto->MaxDurability)
    {
        SetUInt32Value(ITEM_FIELD_DURABILITY, proto->MaxDurability);
        need_save = true;
    }

    SetUInt32Value(ITEM_FIELD_CREATE_PLAYED_TIME, fields[12].GetUInt32());
    SetText(fields[13].GetString());

    if (need_save)                                           // normal item changed state set not work at loading
    {
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_ITEM_INSTANCE_ON_LOAD);
        stmt->setUInt32(0, GetUInt32Value(ITEM_FIELD_EXPIRATION));
        stmt->setUInt32(1, GetUInt32Value(ITEM_FIELD_DYNAMIC_FLAGS));
        stmt->setUInt32(2, GetUInt32Value(ITEM_FIELD_DURABILITY));
        stmt->setUInt32(3, guid);
        CharacterDatabase.Execute(stmt);
    }

    return true;
}

/*static*/
void Item::DeleteFromDB(SQLTransaction& trans, uint32 itemGuid)
{
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_ITEM_INSTANCE);
    stmt->setUInt32(0, itemGuid);
    trans->Append(stmt);
}

void Item::DeleteFromDB(SQLTransaction& trans)
{
    DeleteFromDB(trans, GetGUIDLow());
}

/*static*/
void Item::DeleteFromInventoryDB(SQLTransaction& trans, uint32 itemGuid)
{
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_INVENTORY_BY_ITEM);
    stmt->setUInt32(0, itemGuid);
    trans->Append(stmt);
}

void Item::DeleteFromInventoryDB(SQLTransaction& trans)
{
    DeleteFromInventoryDB(trans, GetGUIDLow());
}

ItemTemplate const* Item::GetTemplate() const
{
    return sObjectMgr->GetItemTemplate(GetEntry());
}

Player* Item::GetOwner()const
{
    return ObjectAccessor::FindPlayer(GetOwnerGUID());
}

uint32 Item::GetSkill() const
{
    return GetTemplate()->GetSkill();
}

int32 Item::GenerateItemRandomPropertyId(uint32 item_id)
{
    ItemTemplate const* itemProto = sObjectMgr->GetItemTemplate(item_id);

    if (!itemProto)
        return 0;

    // item must have one from this field values not null if it can have random enchantments
    if ((!itemProto->RandomProperty) && (!itemProto->RandomSuffix))
        return 0;

    // item can have not null only one from field values
    if ((itemProto->RandomProperty) && (itemProto->RandomSuffix))
    {
        sLog->outError(LOG_FILTER_SQL, "Item template %u have RandomProperty == %u and RandomSuffix == %u, but must have one from field =0", itemProto->ItemId, itemProto->RandomProperty, itemProto->RandomSuffix);
        return 0;
    }

    // RandomProperty case
    if (itemProto->RandomProperty)
    {
        uint32 randomPropId = GetItemEnchantMod(itemProto->RandomProperty, ENCHANTMENT_RANDOM_PROPERTY);
        ItemRandomPropertiesEntry const* random_id = sItemRandomPropertiesStore.LookupEntry(randomPropId);
        if (!random_id)
        {
            sLog->outError(LOG_FILTER_SQL, "Enchantment id #%u used but it doesn't have records in 'ItemRandomProperties.dbc'", randomPropId);
            return 0;
        }

        return random_id->ID;
    }
    // RandomSuffix case
    else
    {
        uint32 randomPropId = GetItemEnchantMod(itemProto->RandomSuffix, ENCHANTMENT_RANDOM_SUFFIX);
        ItemRandomSuffixEntry const* random_id = sItemRandomSuffixStore.LookupEntry(randomPropId);
        if (!random_id)
        {
            sLog->outError(LOG_FILTER_SQL, "Enchantment id #%u used but it doesn't have records in sItemRandomSuffixStore.", randomPropId);
            return 0;
        }

        return -int32(random_id->ID);
    }
}

void Item::SetItemRandomProperties(int32 randomPropId)
{
    if (!randomPropId)
        return;

    if (randomPropId > 0)
    {
        ItemRandomPropertiesEntry const* item_rand = sItemRandomPropertiesStore.LookupEntry(randomPropId);
        if (item_rand)
        {
            if (GetInt32Value(ITEM_FIELD_RANDOM_PROPERTIES_ID) != int32(item_rand->ID))
            {
                SetInt32Value(ITEM_FIELD_RANDOM_PROPERTIES_ID, item_rand->ID);
                SetState(ITEM_CHANGED, GetOwner());
            }
            for (uint32 i = PROP_ENCHANTMENT_SLOT_1; i < PROP_ENCHANTMENT_SLOT_1 + 3; ++i)
                SetEnchantment(EnchantmentSlot(i), item_rand->enchant_id[i - PROP_ENCHANTMENT_SLOT_1], 0, 0);
        }
    }
    else
    {
        ItemRandomSuffixEntry const* item_rand = sItemRandomSuffixStore.LookupEntry(-randomPropId);
        if (item_rand)
        {
            if (GetInt32Value(ITEM_FIELD_RANDOM_PROPERTIES_ID) != -int32(item_rand->ID) ||
                !GetItemSuffixFactor())
            {
                SetInt32Value(ITEM_FIELD_RANDOM_PROPERTIES_ID, -int32(item_rand->ID));
                UpdateItemSuffixFactor();
                SetState(ITEM_CHANGED, GetOwner());
            }

            for (uint32 i = PROP_ENCHANTMENT_SLOT_0; i <= PROP_ENCHANTMENT_SLOT_4; ++i)
                SetEnchantment(EnchantmentSlot(i), item_rand->enchant_id[i - PROP_ENCHANTMENT_SLOT_0], 0, 0);
        }
    }
}

void Item::UpdateItemSuffixFactor()
{
    uint32 suffixFactor = GenerateEnchSuffixFactor(GetEntry());
    if (GetItemSuffixFactor() == suffixFactor)
        return;
    SetUInt32Value(ITEM_FIELD_PROPERTY_SEED, suffixFactor);
}

void Item::SetState(ItemUpdateState state, Player* forplayer)
{
    if (uState == ITEM_NEW && state == ITEM_REMOVED)
    {
        // pretend the item never existed
        RemoveFromUpdateQueueOf(forplayer);
        forplayer->DeleteRefundReference(GetGUIDLow());
        delete this;
        return;
    }
    if (state != ITEM_UNCHANGED)
    {
        // new items must stay in new state until saved
        if (uState != ITEM_NEW)
            uState = state;

        AddToUpdateQueueOf(forplayer);
    }
    else
    {
        // unset in queue
        // the item must be removed from the queue manually
        uQueuePos = -1;
        uState = ITEM_UNCHANGED;
    }
}

void Item::AddToUpdateQueueOf(Player* player)
{
    if (IsInUpdateQueue())
        return;

    ASSERT(player != NULL);

    if (player->GetGUID() != GetOwnerGUID())
    {
        sLog->outDebug(LOG_FILTER_PLAYER_ITEMS, "Item::AddToUpdateQueueOf - Owner's guid (%u) and player's guid (%u) don't match!", GUID_LOPART(GetOwnerGUID()), player->GetGUIDLow());
        return;
    }

    if (player->m_itemUpdateQueueBlocked)
        return;

    player->m_itemUpdateQueue.push_back(this);
    uQueuePos = player->m_itemUpdateQueue.size()-1;
}

void Item::RemoveFromUpdateQueueOf(Player* player)
{
    if (!IsInUpdateQueue())
        return;

    //ASSERT(player != NULL)

    if (player && player->GetGUID() != GetOwnerGUID())
    {
        sLog->outDebug(LOG_FILTER_PLAYER_ITEMS, "Item::RemoveFromUpdateQueueOf - Owner's guid (%u) and player's guid (%u) don't match!", GUID_LOPART(GetOwnerGUID()), player->GetGUIDLow());
        return;
    }

    if (player && player->m_itemUpdateQueueBlocked)
        return;

    if (player)
        player->m_itemUpdateQueue[uQueuePos] = NULL;

    uQueuePos = -1;
}

uint8 Item::GetBagSlot() const
{
    return m_container ? m_container->GetSlot() : uint8(INVENTORY_SLOT_BAG_0);
}

bool Item::IsEquipped() const
{
    return !IsInBag() && m_slot < EQUIPMENT_SLOT_END;
}

bool Item::CanBeTraded(bool mail, bool trade) const
{
    if (m_lootGenerated)
        return false;

    if ((!mail || !IsBoundAccountWide()) && (IsSoulBound() && (!HasFlag(ITEM_FIELD_DYNAMIC_FLAGS, ITEM_FLAG_BOP_TRADEABLE) || !trade)))
        return false;

    if (IsBag() && (Player::IsBagPos(GetPos()) || !((Bag const*)this)->IsEmpty()))
        return false;

    if (Player* owner = GetOwner())
    {
        if (owner->CanUnequipItem(GetPos(), false) != EQUIP_ERR_OK)
            return false;
        if (owner->GetLootGUID() == GetGUID())
            return false;
    }

    if (IsBoundByEnchant())
        return false;

    return true;
}

bool Item::HasEnchantRequiredSkill(const Player* player) const
{
    // Check all enchants for required skill
    for (uint32 enchant_slot = PERM_ENCHANTMENT_SLOT; enchant_slot < MAX_ENCHANTMENT_SLOT; ++enchant_slot)
    {
        if (enchant_slot > PRISMATIC_ENCHANTMENT_SLOT && enchant_slot < PROP_ENCHANTMENT_SLOT_0)    // not holding enchantment id
            continue;

        if (uint32 enchant_id = GetEnchantmentId(EnchantmentSlot(enchant_slot)))
            if (SpellItemEnchantmentEntry const* enchantEntry = sSpellItemEnchantmentStore.LookupEntry(enchant_id))
                if (enchantEntry->requiredSkill && player->GetSkillValue(enchantEntry->requiredSkill) < enchantEntry->requiredSkillValue)
                    return false;
    }

  return true;
}

uint32 Item::GetEnchantRequiredLevel() const
{
    uint32 level = 0;

    // Check all enchants for required level
    for (uint32 enchant_slot = PERM_ENCHANTMENT_SLOT; enchant_slot < MAX_ENCHANTMENT_SLOT; ++enchant_slot)
    {
        if (enchant_slot > PRISMATIC_ENCHANTMENT_SLOT && enchant_slot < PROP_ENCHANTMENT_SLOT_0)    // not holding enchantment id
            continue;

        if (uint32 enchant_id = GetEnchantmentId(EnchantmentSlot(enchant_slot)))
            if (SpellItemEnchantmentEntry const* enchantEntry = sSpellItemEnchantmentStore.LookupEntry(enchant_id))
                if (enchantEntry->requiredLevel > level)
                    level = enchantEntry->requiredLevel;
    }

    return level;
}

bool Item::IsBoundByEnchant() const
{
    // Check all enchants for soulbound
    for (uint32 enchant_slot = PERM_ENCHANTMENT_SLOT; enchant_slot < MAX_ENCHANTMENT_SLOT; ++enchant_slot)
    {
        if (enchant_slot > PRISMATIC_ENCHANTMENT_SLOT && enchant_slot < PROP_ENCHANTMENT_SLOT_0)    // not holding enchantment id
            continue;

        if (uint32 enchant_id = GetEnchantmentId(EnchantmentSlot(enchant_slot)))
            if (SpellItemEnchantmentEntry const* enchantEntry = sSpellItemEnchantmentStore.LookupEntry(enchant_id))
                if (enchantEntry->slot & ENCHANTMENT_CAN_SOULBOUND)
                    return true;
    }

    return false;
}

InventoryResult Item::CanBeMergedPartlyWith(ItemTemplate const* proto) const
{
    // not allow merge looting currently items
    if (m_lootGenerated)
        return EQUIP_ERR_LOOT_GONE;

    // check item type
    if (GetEntry() != proto->ItemId)
        return EQUIP_ERR_CANT_STACK;

    // check free space (full stacks can't be target of merge
    if (GetCount() >= proto->GetMaxStackSize())
        return EQUIP_ERR_CANT_STACK;

    return EQUIP_ERR_OK;
}

bool Item::IsFitToSpellRequirements(SpellInfo const* spellInfo) const
{
    ItemTemplate const* proto = GetTemplate();

    if (spellInfo->EquippedItemClass != -1)                 // -1 == any item class
    {
        // Special case - accept vellum for armor/weapon requirements
        if ((spellInfo->EquippedItemClass == ITEM_CLASS_ARMOR ||
            spellInfo->EquippedItemClass == ITEM_CLASS_WEAPON) && proto->IsVellum())
            if (spellInfo->IsAbilityOfSkillType(SKILL_ENCHANTING)) // only for enchanting spells
                return true;

        if (spellInfo->EquippedItemClass != int32(proto->Class))
            return false;                                   //  wrong item class

        if (spellInfo->EquippedItemSubClassMask != 0)        // 0 == any subclass
        {
            if ((spellInfo->EquippedItemSubClassMask & (1 << proto->SubClass)) == 0)
                return false;                               // subclass not present in mask
        }
    }

    if (spellInfo->EquippedItemInventoryTypeMask != 0)       // 0 == any inventory type
    {
        // Special case - accept weapon type for main and offhand requirements
        if ((proto->InventoryType == INVTYPE_WEAPON || proto->InventoryType == INVTYPE_2HWEAPON) &&
            (spellInfo->EquippedItemInventoryTypeMask & (1 << INVTYPE_WEAPONMAINHAND) ||
             spellInfo->EquippedItemInventoryTypeMask & (1 << INVTYPE_WEAPONOFFHAND)))
            return true;
        else if ((spellInfo->EquippedItemInventoryTypeMask & (1 << proto->InventoryType)) == 0)
            return false;                                   // inventory type not present in mask
    }

    return true;
}

void Item::SetEnchantment(EnchantmentSlot slot, uint32 id, uint32 duration, uint32 charges)
{
    // Better lost small time at check in comparison lost time at item save to DB.
    if ((GetEnchantmentId(slot) == id) && (GetEnchantmentDuration(slot) == duration) && (GetEnchantmentCharges(slot) == charges))
        return;

    SetUInt32Value(ITEM_FIELD_ENCHANTMENT + slot*MAX_ENCHANTMENT_OFFSET + ENCHANTMENT_ID_OFFSET, id);
    SetUInt32Value(ITEM_FIELD_ENCHANTMENT + slot*MAX_ENCHANTMENT_OFFSET + ENCHANTMENT_DURATION_OFFSET, duration);
    SetUInt32Value(ITEM_FIELD_ENCHANTMENT + slot*MAX_ENCHANTMENT_OFFSET + ENCHANTMENT_CHARGES_OFFSET, charges);
    SetState(ITEM_CHANGED, GetOwner());
}

void Item::SetEnchantmentDuration(EnchantmentSlot slot, uint32 duration, Player* owner)
{
    if (GetEnchantmentDuration(slot) == duration)
        return;

    SetUInt32Value(ITEM_FIELD_ENCHANTMENT + slot*MAX_ENCHANTMENT_OFFSET + ENCHANTMENT_DURATION_OFFSET, duration);
    SetState(ITEM_CHANGED, owner);
    // Cannot use GetOwner() here, has to be passed as an argument to avoid freeze due to hashtable locking
}

void Item::SetEnchantmentCharges(EnchantmentSlot slot, uint32 charges)
{
    if (GetEnchantmentCharges(slot) == charges)
        return;

    SetUInt32Value(ITEM_FIELD_ENCHANTMENT + slot*MAX_ENCHANTMENT_OFFSET + ENCHANTMENT_CHARGES_OFFSET, charges);
    SetState(ITEM_CHANGED, GetOwner());
}

void Item::ClearEnchantment(EnchantmentSlot slot)
{
    if (!GetEnchantmentId(slot))
        return;

    for (uint8 x = 0; x < MAX_ENCHANTMENT_SPELLS; ++x)
        SetUInt32Value(ITEM_FIELD_ENCHANTMENT + slot*MAX_ENCHANTMENT_OFFSET + x, 0);
    SetState(ITEM_CHANGED, GetOwner());
}

bool Item::GemsFitSockets() const
{
    for (uint32 enchant_slot = SOCK_ENCHANTMENT_SLOT; enchant_slot < SOCK_ENCHANTMENT_SLOT+MAX_GEM_SOCKETS; ++enchant_slot)
    {
        uint8 SocketColor = GetTemplate()->Socket[enchant_slot-SOCK_ENCHANTMENT_SLOT].Color;

        if (!SocketColor) // no socket slot
            continue;

        uint32 enchant_id = GetEnchantmentId(EnchantmentSlot(enchant_slot));
        if (!enchant_id) // no gems on this socket
            return false;

        SpellItemEnchantmentEntry const* enchantEntry = sSpellItemEnchantmentStore.LookupEntry(enchant_id);
        if (!enchantEntry) // invalid gem id on this socket
            return false;

        uint8 GemColor = 0;

        uint32 gemid = enchantEntry->GemID;
        if (gemid)
        {
            ItemTemplate const* gemProto = sObjectMgr->GetItemTemplate(gemid);
            if (gemProto)
            {
                GemPropertiesEntry const* gemProperty = sGemPropertiesStore.LookupEntry(gemProto->GemProperties);
                if (gemProperty)
                    GemColor = gemProperty->color;
            }
        }

        if (!(GemColor & SocketColor)) // bad gem color on this socket
            return false;
    }
    return true;
}

uint8 Item::GetGemCountWithID(uint32 GemID) const
{
    uint8 count = 0;
    for (uint32 enchant_slot = SOCK_ENCHANTMENT_SLOT; enchant_slot < SOCK_ENCHANTMENT_SLOT+MAX_GEM_SOCKETS; ++enchant_slot)
    {
        uint32 enchant_id = GetEnchantmentId(EnchantmentSlot(enchant_slot));
        if (!enchant_id)
            continue;

        SpellItemEnchantmentEntry const* enchantEntry = sSpellItemEnchantmentStore.LookupEntry(enchant_id);
        if (!enchantEntry)
            continue;

        if (GemID == enchantEntry->GemID)
            ++count;
    }
    return count;
}

uint8 Item::GetGemCountWithLimitCategory(uint32 limitCategory) const
{
    uint8 count = 0;
    for (uint32 enchant_slot = SOCK_ENCHANTMENT_SLOT; enchant_slot < SOCK_ENCHANTMENT_SLOT+MAX_GEM_SOCKETS; ++enchant_slot)
    {
        uint32 enchant_id = GetEnchantmentId(EnchantmentSlot(enchant_slot));
        if (!enchant_id)
            continue;

        SpellItemEnchantmentEntry const* enchantEntry = sSpellItemEnchantmentStore.LookupEntry(enchant_id);
        if (!enchantEntry)
            continue;

        ItemTemplate const* gemProto = sObjectMgr->GetItemTemplate(enchantEntry->GemID);
        if (!gemProto)
            continue;

        if (gemProto->ItemLimitCategory == limitCategory)
            ++count;
    }
    return count;
}

bool Item::IsLimitedToAnotherMapOrZone(uint32 cur_mapId, uint32 cur_zoneId) const
{
    ItemTemplate const* proto = GetTemplate();
    return proto && ((proto->Map && proto->Map != cur_mapId) || (proto->Area && proto->Area != cur_zoneId));
}

void Item::SendTimeUpdate(Player* p_Owner)
{
    uint32 l_Duration = GetUInt32Value(ITEM_FIELD_EXPIRATION);
    if (!l_Duration)
        return;

    WorldPacket l_Data(SMSG_ITEM_TIME_UPDATE, (8+4));
    l_Data.appendPackGUID(GetGUID());
    l_Data << l_Duration;
    p_Owner->GetSession()->SendPacket(&l_Data);
}

Item* Item::CreateItem(uint32 item, uint32 count, Player const* player)
{
    if (count < 1)
        return NULL;                                        //don't create item at zero count

    ItemTemplate const* pProto = sObjectMgr->GetItemTemplate(item);
    if (pProto)
    {
        if (count > pProto->GetMaxStackSize())
            count = pProto->GetMaxStackSize();

        ASSERT(count != 0 && "pProto->Stackable == 0 but checked at loading already");

        Item* pItem = NewItemOrBag(pProto);
        if (pItem->Create(sObjectMgr->GenerateLowGuid(HIGHGUID_ITEM), item, player))
        {
            pItem->SetCount(count);
            return pItem;
        }
        else
            delete pItem;
    }
    else
        ASSERT(false);
    return NULL;
}

Item* Item::CloneItem(uint32 count, Player const* player) const
{
    Item* newItem = CreateItem(GetEntry(), count, player);
    if (!newItem)
        return NULL;

    newItem->SetGuidValue(ITEM_FIELD_CREATOR,           GetGuidValue(ITEM_FIELD_CREATOR));
    newItem->SetGuidValue(ITEM_FIELD_GIFT_CREATOR,      GetGuidValue(ITEM_FIELD_GIFT_CREATOR));
    newItem->SetUInt32Value(ITEM_FIELD_DYNAMIC_FLAGS,   GetUInt32Value(ITEM_FIELD_DYNAMIC_FLAGS) & ~(ITEM_FLAG_REFUNDABLE | ITEM_FLAG_BOP_TRADEABLE));
    newItem->SetUInt32Value(ITEM_FIELD_EXPIRATION,      GetUInt32Value(ITEM_FIELD_EXPIRATION));

    // player CAN be NULL in which case we must not update random properties because that accesses player's item update queue
    if (player)
        newItem->SetItemRandomProperties(GetItemRandomPropertyId());

    return newItem;
}

bool Item::IsBindedNotWith(Player const* player) const
{
    // not binded item
    if (!IsSoulBound())
        return false;

    // own item
    if (GetOwnerGUID() == player->GetGUID())
        return false;

    if (HasFlag(ITEM_FIELD_DYNAMIC_FLAGS, ITEM_FLAG_BOP_TRADEABLE))
        if (allowedGUIDs.find(player->GetGUIDLow()) != allowedGUIDs.end())
            return false;

    // BOA item case
    if (IsBoundAccountWide())
        return false;

    return true;
}

void Item::BuildUpdate(UpdateDataMapType& data_map)
{
    if (Player* owner = GetOwner())
        BuildFieldsUpdate(owner, data_map);
    ClearUpdateMask(false);
}

void Item::SaveRefundDataToDB()
{
    SQLTransaction trans = CharacterDatabase.BeginTransaction();

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_ITEM_REFUND_INSTANCE);
    stmt->setUInt32(0, GetGUIDLow());
    trans->Append(stmt);

    stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_ITEM_REFUND_INSTANCE);
    stmt->setUInt32(0, GetGUIDLow());
    stmt->setUInt32(1, GetRefundRecipient());
    stmt->setUInt32(2, GetPaidMoney());
    stmt->setUInt16(3, uint16(GetPaidExtendedCost()));
    trans->Append(stmt);

    CharacterDatabase.CommitTransaction(trans);
}

void Item::DeleteRefundDataFromDB(SQLTransaction* trans)
{
    if (trans && !trans->null())
    {
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_ITEM_REFUND_INSTANCE);
        stmt->setUInt32(0, GetGUIDLow());
        (*trans)->Append(stmt);

    }
}

void Item::SetNotRefundable(Player* owner, bool changestate /*=true*/, SQLTransaction* trans /*=NULL*/)
{
    if (!HasFlag(ITEM_FIELD_DYNAMIC_FLAGS, ITEM_FLAG_REFUNDABLE))
        return;

    RemoveFlag(ITEM_FIELD_DYNAMIC_FLAGS, ITEM_FLAG_REFUNDABLE);
    // Following is not applicable in the trading procedure
    if (changestate)
        SetState(ITEM_CHANGED, owner);

    SetRefundRecipient(0);
    SetPaidMoney(0);
    SetPaidExtendedCost(0);
    DeleteRefundDataFromDB(trans);

    owner->DeleteRefundReference(GetGUIDLow());
}

void Item::UpdatePlayedTime(Player* owner)
{
    /*  Here we update our played time
        We simply add a number to the current played time,
        based on the time elapsed since the last update hereof.
    */
    // Get current played time
    uint32 current_playtime = GetUInt32Value(ITEM_FIELD_CREATE_PLAYED_TIME);
    // Calculate time elapsed since last played time update
    time_t curtime = time(NULL);
    uint32 elapsed = uint32(curtime - m_lastPlayedTimeUpdate);
    uint32 new_playtime = current_playtime + elapsed;
    // Check if the refund timer has expired yet
    if (new_playtime <= 2*HOUR)
    {
        // No? Proceed.
        // Update the data field
        SetUInt32Value(ITEM_FIELD_CREATE_PLAYED_TIME, new_playtime);
        // Flag as changed to get saved to DB
        SetState(ITEM_CHANGED, owner);
        // Speaks for itself
        m_lastPlayedTimeUpdate = curtime;
        return;
    }
    // Yes
    SetNotRefundable(owner);
}

uint32 Item::GetPlayedTime()
{
    time_t curtime = time(NULL);
    uint32 elapsed = uint32(curtime - m_lastPlayedTimeUpdate);
    return GetUInt32Value(ITEM_FIELD_CREATE_PLAYED_TIME) + elapsed;
}

bool Item::IsRefundExpired()
{
    return (GetPlayedTime() > 2*HOUR);
}

void Item::SetSoulboundTradeable(AllowedLooterSet& allowedLooters)
{
    SetFlag(ITEM_FIELD_DYNAMIC_FLAGS, ITEM_FLAG_BOP_TRADEABLE);
    allowedGUIDs = allowedLooters;
}

void Item::ClearSoulboundTradeable(Player* currentOwner)
{
    RemoveFlag(ITEM_FIELD_DYNAMIC_FLAGS, ITEM_FLAG_BOP_TRADEABLE);
    if (allowedGUIDs.empty())
        return;

    allowedGUIDs.clear();
    SetState(ITEM_CHANGED, currentOwner);
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_ITEM_BOP_TRADE);
    stmt->setUInt32(0, GetGUIDLow());
    CharacterDatabase.Execute(stmt);
}

bool Item::CheckSoulboundTradeExpire()
{
    // called from owner's update - GetOwner() MUST be valid
    if (GetUInt32Value(ITEM_FIELD_CREATE_PLAYED_TIME) + 2*HOUR < GetOwner()->GetTotalPlayedTime())
    {
        ClearSoulboundTradeable(GetOwner());
        return true; // remove from tradeable list
    }

    return false;
}

bool Item::CanBeTransmogrified() const
{
    ItemTemplate const* proto = GetTemplate();

    if (!proto)
        return false;

    if (proto->Quality == ITEM_QUALITY_LEGENDARY)
        return false;

    if (proto->Class != ITEM_CLASS_ARMOR &&
        proto->Class != ITEM_CLASS_WEAPON)
        return false;

    if (proto->Class == ITEM_CLASS_WEAPON && proto->SubClass == ITEM_SUBCLASS_WEAPON_FISHING_POLE)
        return false;

    if (proto->Flags2 & ITEM_FLAGS_EXTRA_CANNOT_BE_TRANSMOG)
        return false;

    return true;
}

bool Item::CanTransmogrify() const
{
    ItemTemplate const* proto = GetTemplate();

    if (!proto)
        return false;

    if (proto->Flags2 & ITEM_FLAGS_EXTRA_CANNOT_TRANSMOG)
        return false;

    if (proto->Quality == ITEM_QUALITY_LEGENDARY)
        return false;

    if (proto->Class != ITEM_CLASS_ARMOR &&
        proto->Class != ITEM_CLASS_WEAPON)
        return false;

    if (proto->Class == ITEM_CLASS_WEAPON && proto->SubClass == ITEM_SUBCLASS_WEAPON_FISHING_POLE)
        return false;

    if (proto->Flags2 & ITEM_FLAGS_EXTRA_CAN_TRANSMOG)
        return true;

    return true;
}

bool Item::SubclassesCompatible(ItemTemplate const* proto1, ItemTemplate const* proto2) const
{
    //   Source     Destination
    if (!proto1 || !proto2)
        return false;

    // Patch 5.2 - Throne of Thunder
    // One-Handed
    // One-handed axes, maces, and swords can be Transmogrified to each other.
    if ((proto1->SubClass == ITEM_SUBCLASS_WEAPON_AXE ||
        proto1->SubClass == ITEM_SUBCLASS_WEAPON_MACE ||
        proto1->SubClass == ITEM_SUBCLASS_WEAPON_SWORD) &&
        (proto2->SubClass == ITEM_SUBCLASS_WEAPON_AXE ||
        proto2->SubClass == ITEM_SUBCLASS_WEAPON_MACE ||
        proto2->SubClass == ITEM_SUBCLASS_WEAPON_SWORD))
        return true;

    // Two-Handed
    // Two-handed axes, maces, and swords can be Transmogrified to each other.
    if ((proto1->SubClass == ITEM_SUBCLASS_WEAPON_AXE2 ||
        proto1->SubClass == ITEM_SUBCLASS_WEAPON_MACE2 ||
        proto1->SubClass == ITEM_SUBCLASS_WEAPON_SWORD2) &&
        (proto2->SubClass == ITEM_SUBCLASS_WEAPON_AXE2 ||
        proto2->SubClass == ITEM_SUBCLASS_WEAPON_MACE2 ||
        proto2->SubClass == ITEM_SUBCLASS_WEAPON_SWORD2))
        return true;

    // Ranged
    if ((proto1->SubClass == ITEM_SUBCLASS_WEAPON_BOW ||
        proto1->SubClass == ITEM_SUBCLASS_WEAPON_GUN ||
        proto1->SubClass == ITEM_SUBCLASS_WEAPON_CROSSBOW) &&
        (proto2->SubClass == ITEM_SUBCLASS_WEAPON_BOW ||
        proto2->SubClass == ITEM_SUBCLASS_WEAPON_GUN ||
        proto2->SubClass == ITEM_SUBCLASS_WEAPON_CROSSBOW))
        return true;

    // Polearm and Staff
    // Staves and polearms can be transmogrified to each other.
    if ((proto1->SubClass == ITEM_SUBCLASS_WEAPON_POLEARM ||
        proto1->SubClass == ITEM_SUBCLASS_WEAPON_STAFF) &&
        (proto2->SubClass == ITEM_SUBCLASS_WEAPON_POLEARM ||
        proto2->SubClass == ITEM_SUBCLASS_WEAPON_STAFF))
        return true;

    return false;
}

bool Item::CanTransmogrifyItemWithItem(Item* transmogrified, Item* transmogrifier)
{
    if (!transmogrifier || !transmogrified)
        return false;

    ItemTemplate const* proto1 = transmogrifier->GetTemplate(); // source
    ItemTemplate const* proto2 = transmogrified->GetTemplate(); // dest

    if (proto1->ItemId == proto2->ItemId)
        return false;

    if (!transmogrified->CanTransmogrify() || !transmogrifier->CanBeTransmogrified())
        return false;

    if (proto1->InventoryType == INVTYPE_BAG ||
        proto1->InventoryType == INVTYPE_RELIC ||
        proto1->InventoryType == INVTYPE_BODY ||
        proto1->InventoryType == INVTYPE_FINGER ||
        proto1->InventoryType == INVTYPE_TRINKET ||
        proto1->InventoryType == INVTYPE_AMMO ||
        proto1->InventoryType == INVTYPE_QUIVER ||
        proto1->InventoryType == INVTYPE_NON_EQUIP ||
        proto1->InventoryType == INVTYPE_TABARD)
        return false;

    if (proto2->InventoryType == INVTYPE_BAG ||
        proto2->InventoryType == INVTYPE_RELIC ||
        proto2->InventoryType == INVTYPE_BODY ||
        proto2->InventoryType == INVTYPE_FINGER ||
        proto2->InventoryType == INVTYPE_TRINKET ||
        proto2->InventoryType == INVTYPE_AMMO ||
        proto2->InventoryType == INVTYPE_QUIVER ||
        proto2->InventoryType == INVTYPE_NON_EQUIP ||
        proto2->InventoryType == INVTYPE_TABARD)
        return false;

    if (proto1->Class != proto2->Class)
        return false;

    if (proto1->SubClass != ITEM_SUBCLASS_ARMOR_COSMETIC && (proto1->Class != ITEM_CLASS_WEAPON || !proto2->IsRangedWeapon() || !proto1->IsRangedWeapon()) &&
        (proto1->SubClass != proto2->SubClass && !transmogrifier->SubclassesCompatible(proto1, proto2)))
        return false;

    if (proto1->InventoryType != proto2->InventoryType)
    {
        if (proto1->Class == ITEM_CLASS_WEAPON && proto2->Class == ITEM_CLASS_WEAPON)
        {
            if (!((proto1->InventoryType == INVTYPE_WEAPON || proto1->InventoryType == INVTYPE_WEAPONMAINHAND ||
                proto1->InventoryType == INVTYPE_WEAPONOFFHAND || proto1->InventoryType == INVTYPE_RANGED || proto1->InventoryType == INVTYPE_RANGEDRIGHT) &&
                (proto2->InventoryType == INVTYPE_WEAPON || proto2->InventoryType == INVTYPE_WEAPONMAINHAND ||
                proto2->InventoryType == INVTYPE_WEAPONOFFHAND || proto2->InventoryType == INVTYPE_RANGED || proto2->InventoryType == INVTYPE_RANGEDRIGHT)))
                return false;
        }
        else if (proto1->Class == ITEM_CLASS_ARMOR && proto2->Class == ITEM_CLASS_ARMOR)
        {
            if (!((proto1->InventoryType == INVTYPE_CHEST || proto1->InventoryType == INVTYPE_ROBE) &&
                (proto2->InventoryType == INVTYPE_CHEST || proto2->InventoryType == INVTYPE_ROBE)))
                return false;
        }
    }

    // Check armor types
    if (proto1->SubClass != ITEM_SUBCLASS_ARMOR_COSMETIC && (proto1->Class == ITEM_CLASS_ARMOR || proto2->Class == ITEM_CLASS_ARMOR))
    {
        uint32 skill1 = proto1->GetSkill();
        uint32 skill2 = proto2->GetSkill();

        if ((skill1 == SKILL_PLATE_MAIL || skill1 == SKILL_LEATHER ||
            skill1 == SKILL_MAIL || skill1 == SKILL_CLOTH) ||
            (skill2 == SKILL_PLATE_MAIL || skill2 == SKILL_LEATHER ||
            skill2 == SKILL_MAIL || skill2 == SKILL_CLOTH))
            if (skill1 != skill2)
                return false;
    }

    return true;
}

bool Item::HasStats() const
{
    if (GetItemRandomPropertyId() != 0)
        return true;

    ItemTemplate const* proto = GetTemplate();
    for (uint8 i = 0; i < MAX_ITEM_PROTO_STATS; ++i)
        if (proto->ItemStat[i].ItemStatValue != 0)
            return true;

    return false;
}

bool Item::HasSpells() const
{
    ItemTemplate const* proto = GetTemplate();
    for (uint8 i = 0; i < MAX_ITEM_PROTO_SPELLS; ++i)
        if (proto->Spells[i].SpellId != 0)
            return true;

    return false;
}

// used by mail items, transmog cost, stationeryinfo and others
uint32 Item::GetSellPrice(ItemTemplate const* proto, bool& normalSellPrice)
{
    normalSellPrice = true;

    if (proto->Flags2 & ITEM_FLAGS_EXTRA_HAS_NORMAL_PRICE)
    {
        return proto->BuyPrice;
    }
    else
    {
        ImportPriceQualityEntry const* qualityPrice = sImportPriceQualityStore.LookupEntry(proto->Quality + 1);
        ItemPriceBaseEntry const* basePrice = sItemPriceBaseStore.LookupEntry(proto->ItemLevel);

        if (!qualityPrice || !basePrice)
            return 0;

        float qualityFactor = qualityPrice->Factor;
        float baseFactor = 0.0f;

        uint32 inventoryType = proto->InventoryType;

        if (inventoryType == INVTYPE_WEAPON ||
            inventoryType == INVTYPE_2HWEAPON ||
            inventoryType == INVTYPE_WEAPONMAINHAND ||
            inventoryType == INVTYPE_WEAPONOFFHAND ||
            inventoryType == INVTYPE_RANGED ||
            inventoryType == INVTYPE_THROWN ||
            inventoryType == INVTYPE_RANGEDRIGHT)
            baseFactor = basePrice->WeaponFactor;
        else
            baseFactor = basePrice->ArmorFactor;

        if (inventoryType == INVTYPE_ROBE)
            inventoryType = INVTYPE_CHEST;

        float typeFactor = 0.0f;
        uint8 wepType = 0xFF;

        switch (inventoryType)
        {
            case INVTYPE_HEAD:
            case INVTYPE_SHOULDERS:
            case INVTYPE_CHEST:
            case INVTYPE_WAIST:
            case INVTYPE_LEGS:
            case INVTYPE_FEET:
            case INVTYPE_WRISTS:
            case INVTYPE_HANDS:
            case INVTYPE_CLOAK:
            {
                ImportPriceArmorEntry const* armorPrice = sImportPriceArmorStore.LookupEntry(inventoryType);
                if (!armorPrice)
                    return 0;

                switch (proto->SubClass)
                {
                    case ITEM_SUBCLASS_ARMOR_MISCELLANEOUS:
                    case ITEM_SUBCLASS_ARMOR_CLOTH:
                    {
                        typeFactor = armorPrice->ClothFactor;
                        break;
                    }
                    case ITEM_SUBCLASS_ARMOR_LEATHER:
                    {
                        typeFactor = armorPrice->ClothFactor;
                        break;
                    }
                    case ITEM_SUBCLASS_ARMOR_MAIL:
                    {
                        typeFactor = armorPrice->ClothFactor;
                        break;
                    }
                    case ITEM_SUBCLASS_ARMOR_PLATE:
                    {
                        typeFactor = armorPrice->ClothFactor;
                        break;
                    }
                    default:
                    {
                        return 0;
                    }
                }

                break;
            }
            case INVTYPE_SHIELD:
            {
                ImportPriceShieldEntry const* shieldPrice = sImportPriceShieldStore.LookupEntry(1); // it only has two rows, it's unclear which is the one used
                if (!shieldPrice)
                    return 0;

                typeFactor = shieldPrice->Factor;
                break;
            }
            case INVTYPE_WEAPONMAINHAND:
                wepType = 0;             // unk enum, fall back
            case INVTYPE_WEAPONOFFHAND:
                wepType = 1;             // unk enum, fall back
            case INVTYPE_WEAPON:
                wepType = 2;             // unk enum, fall back
            case INVTYPE_2HWEAPON:
                wepType = 3;             // unk enum, fall back
            case INVTYPE_RANGED:
            case INVTYPE_RANGEDRIGHT:
            case INVTYPE_RELIC:
            {
                wepType = 4;             // unk enum

                ImportPriceWeaponEntry const* weaponPrice = sImportPriceWeaponStore.LookupEntry(wepType + 1);
                if (!weaponPrice)
                    return 0;

                typeFactor = weaponPrice->Factor;
                break;
            }
            default:
                return proto->BuyPrice;
        }

        normalSellPrice = false;
        return (uint32)(qualityFactor * proto->Unk430_2 * proto->Unk430_1 * typeFactor * baseFactor);
    }
}

uint32 Item::GetSpecialPrice(ItemTemplate const* proto, uint32 minimumPrice /*= 10000*/)
{
    uint32 cost = 0;

    if (proto->Flags2 & ITEM_FLAGS_EXTRA_HAS_NORMAL_PRICE)
        cost = proto->SellPrice;
    else
    {
        bool normalPrice;
        cost = Item::GetSellPrice(proto, normalPrice);

        if (!normalPrice)
        {
            if (proto->BuyCount <= 1)
            {
                ItemClassEntry const* classEntry = sItemClassStore.LookupEntry(proto->Class);
                if (classEntry)
                    cost *= classEntry->PriceFactor;
                else
                    cost = 0;
            }
            else
                cost /= 4 * proto->BuyCount;
        }
        else
            cost = proto->SellPrice;
    }

    if (cost < minimumPrice)
        cost = minimumPrice;

    return cost;
}

int32 Item::GetReforgableStat(ItemModType statType) const
{
    ItemTemplate const* proto = GetTemplate();
    for (uint32 i = 0; i < MAX_ITEM_PROTO_STATS; ++i)
        if (proto->ItemStat[i].ItemStatType == (uint32)statType)
            return proto->ItemStat[i].ItemStatValue;

    int32 randomPropId = GetItemRandomPropertyId();
    if (!randomPropId)
        return 0;

    if (randomPropId < 0)
    {
        ItemRandomSuffixEntry const* randomSuffix = sItemRandomSuffixStore.LookupEntry(-randomPropId);
        if (!randomSuffix)
            return 0;

        for (uint32 e = PROP_ENCHANTMENT_SLOT_0; e <= PROP_ENCHANTMENT_SLOT_4; ++e)
        {
            if (SpellItemEnchantmentEntry const* enchant = sSpellItemEnchantmentStore.LookupEntry(GetEnchantmentId(EnchantmentSlot(e))))
            {
                for (uint32 f = 0; f < MAX_ENCHANTMENT_SPELLS; ++f)
                {
                    if (enchant->type[f] == ITEM_ENCHANTMENT_TYPE_STAT && enchant->spellid[f] == (uint32)statType)
                    {
                        for (int k = 0; k < 5; ++k)
                        {
                            if (randomSuffix->enchant_id[k] == enchant->ID)
                                return int32((randomSuffix->prefix[k] * GetItemSuffixFactor()) / 10000);
                        }
                    }
                }
            }
        }
    }
    else
    {
        ItemRandomPropertiesEntry const* randomProp = sItemRandomPropertiesStore.LookupEntry(randomPropId);
        if (!randomProp)
            return 0;

        for (uint32 e = PROP_ENCHANTMENT_SLOT_0; e <= PROP_ENCHANTMENT_SLOT_4; ++e)
        {
            if (SpellItemEnchantmentEntry const* enchant = sSpellItemEnchantmentStore.LookupEntry(GetEnchantmentId(EnchantmentSlot(e))))
            {
                for (uint32 f = 0; f < MAX_ENCHANTMENT_SPELLS; ++f)
                {
                    if (enchant->type[f] == ITEM_ENCHANTMENT_TYPE_STAT && enchant->spellid[f] == (uint32)statType)
                    {
                        for (int k = 0; k < MAX_ENCHANTMENT_SPELLS; ++k)
                        {
                            if (randomProp->enchant_id[k] == enchant->ID)
                                return int32(enchant->amount[k]);
                        }
                    }
                }
            }
        }
    }

    return 0;
}

bool Item::IsPotion() const
{
    return GetTemplate()->IsPotion();
}

bool Item::IsPvPItem() const
{
    ItemTemplate const* proto = GetTemplate();
    if (!proto)
        return false;

    for (uint8 i = 0; i < MAX_ITEM_PROTO_STATS; ++i)
    {
        uint32 stat = proto->ItemStat[i].ItemStatType;
        if (stat == ITEM_MOD_PVP_POWER || stat == ITEM_MOD_RESILIENCE_RATING)
            return true;
    }

    for (uint8 i = 0; i < MAX_ITEM_PROTO_SPELLS; ++i)
    {
        int32 spell = proto->Spells[i].SpellId;
        if (spell == 132586 || spell == 139891)
            return true;
    }

    return false;
}

bool Item::IsStuffItem() const
{
    ItemTemplate const* proto = GetTemplate();
    if (!proto)
        return false;

    uint32 invType = proto->InventoryType;

    switch (invType)
    {
        case INVTYPE_NON_EQUIP:
        case INVTYPE_BODY:
        case INVTYPE_BAG:
        case INVTYPE_TABARD:
        case INVTYPE_AMMO:
        case INVTYPE_THROWN:
        case INVTYPE_QUIVER:
        case INVTYPE_RELIC:
            return false;
        default:
            return true;
    }
}

bool Item::CanUpgrade() const
{
    ItemTemplate const* proto = GetTemplate();
    if (!proto)
        return false;

    if (proto->ItemLevel < 458)
        return false;

    if (proto->Quality == ITEM_QUALITY_LEGENDARY && !IsLegendaryCloak())
        return false;

    if (!IsStuffItem())
        return false;

    if (proto->Class == ITEM_CLASS_WEAPON && proto->SubClass == ITEM_SUBCLASS_WEAPON_FISHING_POLE)
        return false;

    if (!HasStats() && proto->InventoryType != INVTYPE_TRINKET)
        return false;

    // PvP item can't be upgraded after Season 12
    if (IsPvPItem() && proto->ItemLevel > 483)
        return false;

    return true;
}

bool Item::IsLegendaryCloak() const
{
    ItemTemplate const* proto = GetTemplate();
    if (!proto)
        return false;

    switch (proto->ItemId)
    {
        case 102245: // Qian-Le, Courage of Niuzao
        case 102246: // Xing-Ho, Breath of Yu'lon
        case 102247: // Jina-Kang, Kindness of Chi-Ji
        case 102248: // Fen-Yu, Fury of Xuen
        case 102249: // Gong-Lu, Strength of Xuen
        case 102250: // Qian-Ying, Fortitude of Niuzao
            return true;
        default:
            break;
    }

    return false;
}

float ItemTemplate::GetScalingDamageValue(uint32 ilvl) const
{
    if (Quality > ITEM_QUALITY_HEIRLOOM)
        return 0.f;

    ItemDamageEntry const* damageEntry = NULL;

    switch (InventoryType)
    {
        case INVTYPE_WEAPON:
        case INVTYPE_WEAPONMAINHAND:
        case INVTYPE_WEAPONOFFHAND:
            if (Flags2 & ITEM_FLAGS_EXTRA_CASTER_WEAPON)
            {
                damageEntry =  sItemDamageOneHandCasterStore.LookupEntry(ilvl);
                break;
            }
            damageEntry = sItemDamageOneHandStore.LookupEntry(ilvl);
            break;
    case INVTYPE_RANGED:
    case INVTYPE_THROWN:
    case INVTYPE_RANGEDRIGHT:
        if (SubClass < 4)
        {
            if (Flags2 & ITEM_FLAGS_EXTRA_CASTER_WEAPON)
            {
                damageEntry = sItemDamageTwoHandCasterStore.LookupEntry(ilvl);
                break;
            }
            damageEntry = sItemDamageTwoHandStore.LookupEntry(ilvl);
            break;
        }
        else if (SubClass == 19)
        {
            damageEntry = sItemDamageOneHandCasterStore.LookupEntry(ilvl);
            break;
        }
        else
        {
            if (Flags2 & ITEM_FLAGS_EXTRA_CASTER_WEAPON)
            {
                damageEntry = sItemDamageTwoHandCasterStore.LookupEntry(ilvl);
                break;
            }
            damageEntry = sItemDamageTwoHandStore.LookupEntry(ilvl);
            break;
        }
    case INVTYPE_AMMO:
        damageEntry = sItemDamageAmmoStore.LookupEntry(ilvl);
        break;
    case INVTYPE_2HWEAPON:
        if (Flags2 & ITEM_FLAGS_EXTRA_CASTER_WEAPON)
        {
            damageEntry = sItemDamageTwoHandCasterStore.LookupEntry(ilvl);
            break;
        }
        damageEntry = sItemDamageTwoHandStore.LookupEntry(ilvl);
        break;
    default:
        break;
    }
    return damageEntry ? damageEntry->DPS[Quality == ITEM_QUALITY_HEIRLOOM ? ITEM_QUALITY_RARE : Quality] : 0.f;
}

int32 ItemTemplate::GetRandomPointsOffset() const
{
    switch (InventoryType)
    {
        case INVTYPE_NECK:
        case INVTYPE_WRISTS:
        case INVTYPE_FINGER:
        case INVTYPE_SHIELD:
        case INVTYPE_CLOAK:
        case INVTYPE_HOLDABLE:
            return 2;
        case INVTYPE_SHOULDERS:
        case INVTYPE_WAIST:
        case INVTYPE_FEET:
        case INVTYPE_HANDS:
        case INVTYPE_TRINKET:
            return 1;
        case INVTYPE_WEAPON:
        case INVTYPE_WEAPONMAINHAND:
        case INVTYPE_WEAPONOFFHAND:
            return 3;
        case INVTYPE_RANGEDRIGHT:
            return 3 * (SubClass == 19 ? 1 : 0);
        case INVTYPE_RELIC:
            return 4;
        case INVTYPE_HEAD:
        case INVTYPE_BODY:
        case INVTYPE_CHEST:
        case INVTYPE_LEGS:
        case INVTYPE_RANGED:
        case INVTYPE_2HWEAPON:
        case INVTYPE_ROBE:
        case INVTYPE_THROWN:
            return 0;
        default:
            return -1;
    }
}

uint32 ItemTemplate::CalculateScalingStatDBCValue(uint32 ilvl) const
{
    int32 offset = GetRandomPointsOffset();
    if (offset == -1)
        return 0;

    RandomPropertiesPointsEntry const* randProperty = sRandomPropertiesPointsStore.LookupEntry(ilvl);

    if (!randProperty)
        return 0;

    switch (Quality)
    {
        case ITEM_QUALITY_UNCOMMON:
        return randProperty->UncommonPropertiesPoints[offset];
        case ITEM_QUALITY_RARE:
        case ITEM_QUALITY_HEIRLOOM:
            return randProperty->RarePropertiesPoints[offset];
        case ITEM_QUALITY_EPIC:
            return randProperty->EpicPropertiesPoints[offset];
        default:
            return 0;
    }
}

float ItemTemplate::GetSocketCost(uint32 ilvl) const
{
    gtItemSocketCostPerLevelEntry const* socket = sgtItemSocketCostPerLevelStore.LookupEntry(ilvl);
    return socket ? socket->cost : 0.f;
}

int32 ItemTemplate::CalculateStatScaling(uint32 index, uint32 ilvl) const
{
    _ItemStat const& itemStat = ItemStat[index];
    return CalculateStatScaling(itemStat.ScalingValue, itemStat.SocketCostRate, ilvl);
}

int32 ItemTemplate::CalculateStatScaling(int32 scalingValue, float socketCost, uint32 ilvl) const
{
    return floor((((float)scalingValue * (float)CalculateScalingStatDBCValue(ilvl) * 0.000099999997f) - (GetSocketCost(ilvl) * socketCost)) + 0.5f);
}

uint32 ItemTemplate::CalculateArmorScaling(uint32 ilvl) const
{
    if (ilvl == ItemLevel)
        return Armor;

    uint32 quality = Quality == ITEM_QUALITY_HEIRLOOM ? ITEM_QUALITY_RARE : Quality;

    if (Class != ITEM_CLASS_ARMOR || SubClass != ITEM_SUBCLASS_ARMOR_SHIELD)
    {
        if (InventoryType == 1 || InventoryType == 5 || InventoryType == 3 || InventoryType == 7 || InventoryType == 8 || InventoryType == 9 || InventoryType == 10 || InventoryType == 6 || InventoryType == 16 || InventoryType == 20)
        {
            ItemArmorQualityEntry const* armorQuality = sItemArmorQualityStore.LookupEntry(ilvl);
            ItemArmorTotalEntry const* armorTotal = sItemArmorTotalStore.LookupEntry(ilvl);
            ArmorLocationEntry const* armorLoc = sArmorLocationStore.LookupEntry(InventoryType == 20 ? 5 : InventoryType);

            if (SubClass == 0 || SubClass > 4)
                return 0.0f;

            return (int)floor(armorQuality->Value[quality] * armorTotal->Value[SubClass - 1] * armorLoc->Value[SubClass - 1] + 0.5f);
        }
        return 0;
    }
    else
    {
        ItemArmorShieldEntry const* shieldEntry = sItemArmorShieldStore.LookupEntry(ilvl);
        return shieldEntry->Value[quality];
    }
}

void ItemTemplate::CalculateMinMaxDamageScaling(uint32 ilvl, uint32& minDamage, uint32& maxDamage) const
{
    minDamage = 0;
    maxDamage = 0;

    if (ilvl == ItemLevel)
    {
        minDamage = DamageMin;
        maxDamage = DamageMax;
        return;
    }

    if (!IsWeapon())
        return;

    float weaponMinDamageCalc = (float)Delay * GetScalingDamageValue(ilvl) * 0.001f;
    float weaponMaxDamageCalc = (((StatScalingFactor * 0.5f) + 1.f) * weaponMinDamageCalc) + 0.5f;

    if (Delay != 0)
    {
        float delayModifier = 1000.0f / (float)Delay;
        float midCalc = (delayModifier * ((1.f - (StatScalingFactor * 0.5f)) * weaponMinDamageCalc)) + ArmorDamageModifier;
        midCalc = midCalc > 1.f ? midCalc : 1.f;
        float delayCoeff = 1.f / delayModifier;
        minDamage = floor((delayCoeff * midCalc) + 0.5f);
        maxDamage = floor((delayCoeff * ((delayModifier * weaponMaxDamageCalc) + ArmorDamageModifier)) + 0.5f);
    }
    else
    {
        maxDamage = floor(weaponMaxDamageCalc + 0.5f);
        minDamage = floor(((1.f - (StatScalingFactor * 0.5f)) * weaponMinDamageCalc) + 0.5f);
    }
}

bool Item::AddItemBonus(uint32 p_ItemBonusId)
{
    if (!GetItemBonusesByID(p_ItemBonusId))
        return false;

    if (HasItemBonus(p_ItemBonusId))
        return false;

    SetDynamicValue(ITEM_DYNAMIC_FIELD_BONUSLIST_IDS, GetAllItemBonuses().size(), p_ItemBonusId);
    return true;
}

void Item::AddItemBonuses(std::vector<uint32> const& p_ItemBonuses)
{
    if (!p_ItemBonuses.size())
        return;

    for (uint32 i = 0; i < p_ItemBonuses.size(); i++)
        AddItemBonus(p_ItemBonuses[i]);
}

bool Item::HasItemBonus(uint32 p_ItemBonusId) const
{
    std::vector<uint32> const& l_BonusList = GetAllItemBonuses();
    for (auto& l_Bonus : l_BonusList)
        if (l_Bonus == p_ItemBonusId)
            return true;
    return false;
}

bool Item::RemoveItemBonus(uint32 p_ItemBonusId)
{
    std::vector<uint32> const& l_BonusList = GetAllItemBonuses();
    for (uint32 i = 0; i < l_BonusList.size(); i++)
    {
        if (l_BonusList[i] == p_ItemBonusId && p_ItemBonusId)
        {
            SetDynamicValue(ITEM_DYNAMIC_FIELD_BONUSLIST_IDS, i, 0);
            return true;
        }
    }

    return false;
}

void Item::RemoveAllItemBonuses()
{
    std::vector<uint32> const& l_BonusList = GetAllItemBonuses();
    for (auto& l_Bonus : l_BonusList)
        if (l_Bonus)
            RemoveItemBonus(l_Bonus);
}

std::vector<uint32> const& Item::GetAllItemBonuses() const
{
    return GetDynamicValues(ITEM_DYNAMIC_FIELD_BONUSLIST_IDS);
}

uint32 Item::GetItemLevelBonusFromItemBonuses() const
{
    uint32 itemLevel = 0;
    for (auto l_Bonus : GetAllItemBonuses())
    {
        if (!l_Bonus)
            continue;

        std::vector<ItemBonusEntry const*> const* l_ItemBonus = GetItemBonusesByID(l_Bonus);
        if (!l_ItemBonus)
            continue;

        for (uint32 i = 0; i < l_ItemBonus->size(); i++)
        {
            ItemBonusEntry const* l_ItemSubBonus = (*l_ItemBonus)[i];
            if (!l_ItemSubBonus)
                break;

            if (l_ItemSubBonus->Type == ITEM_BONUS_MODIFY_ITEM_LEVEL)
                itemLevel += l_ItemSubBonus->Value[0];
        }
    }

    return itemLevel;
}
