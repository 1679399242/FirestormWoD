/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
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

/*
 * Scripts for spells with SPELLFAMILY_SHAMAN and SPELLFAMILY_GENERIC spells used by shaman players.
 * Ordered alphabetically using scriptname.
 * Scriptnames of files in this file should be prefixed with "spell_sha_".
 */

#include "ScriptMgr.h"
#include "Cell.h"
#include "CellImpl.h"
#include "GridNotifiersImpl.h"
#include "GridNotifiers.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"

enum ShamanSpells
{
    SHAMAN_SPELL_SATED                          = 57724,
    SHAMAN_SPELL_EXHAUSTION                     = 57723,
    HUNTER_SPELL_INSANITY                       = 95809,
    MAGE_SPELL_TEMPORAL_DISPLACEMENT            = 80354,
    SPELL_SHA_LIGHTNING_SHIELD_AURA             = 324,
    SPELL_SHA_ASCENDANCE_ELEMENTAL              = 114050,
    SPELL_SHA_ASCENDANCE_RESTORATION            = 114052,
    SPELL_SHA_ASCENDANCE_ENHANCED               = 114051,
    SPELL_SHA_ASCENDANCE                        = 114049,
    SPELL_SHA_HEALING_RAIN                      = 142923,
    SPELL_SHA_HEALING_RAIN_TICK                 = 73921,
    SPELL_SHA_HEALING_RAIN_AURA                 = 73920,
    SPELL_SHA_ELEMENTAL_BLAST                   = 117014,
    SPELL_SHA_ELEMENTAL_BLAST_NATURE_VISUAL     = 118517,
    SPELL_SHA_ELEMENTAL_BLAST_FROST_VISUAL      = 118515,
    SPELL_SHA_LAVA_LASH                         = 60103,
    SPELL_SHA_FLAME_SHOCK                       = 8050,
    SPELL_SHA_STORMSTRIKE                       = 17364,
    SPELL_SHA_LIGHTNING_SHIELD_ORB_DAMAGE       = 26364,
    SPELL_SHA_HEALING_STREAM                    = 52042,
    SPELL_SHA_GLYPH_OF_HEALING_STREAM           = 119523,
    SPELL_SHA_LAVA_SURGE_CAST_TIME              = 77762,
    SPELL_SHA_FULMINATION                       = 88766,
    SPELL_SHA_FULMINATION_TRIGGERED             = 88767,
    SPELL_SHA_FULMINATION_INFO                  = 95774,
    SPELL_SHA_ROLLING_THUNDER_AURA              = 88764,
    SPELL_SHA_ROLLING_THUNDER_ENERGIZE          = 88765,
    SPELL_SHA_UNLEASH_ELEMENTS                  = 73680,
    SPELL_SHA_SEARING_FLAMES_DAMAGE_DONE        = 77661,
    SPELL_SHA_FIRE_NOVA                         = 1535,
    SPELL_SHA_FIRE_NOVA_TRIGGERED               = 8349,
    SPELL_SHA_TIDAL_WAVES                       = 51564,
    SPELL_SHA_TIDAL_WAVES_PROC                  = 53390,
    SPELL_SHA_MANA_TIDE                         = 16191,
    SPELL_SHA_FROST_SHOCK_FREEZE                = 63685,
    SPELL_SHA_FROZEN_POWER                      = 63374,
    SPELL_SHA_MAIL_SPECIALIZATION_AGI           = 86099,
    SPELL_SHA_MAIL_SPECIALISATION_INT           = 86100,
    SPELL_SHA_UNLEASHED_FURY_TALENT             = 117012,
    SPELL_SHA_UNLEASHED_FURY_FLAMETONGUE        = 118470,
    SPELL_SHA_UNLEASHED_FURY_WINDFURY           = 118472,
    SPELL_SHA_UNLEASHED_FURY_EARTHLIVING        = 118473,
    SPELL_SHA_UNLEASHED_FURY_FROSTBRAND         = 118474,
    SPELL_SHA_UNLEASHED_FURY_ROCKBITER          = 118475,
    SPELL_SHA_STONE_BULWARK_ABSORB              = 114893,
    SPELL_SHA_EARTHGRAB_IMMUNITY                = 116946,
    SPELL_SHA_EARTHBIND_FOR_EARTHGRAB_TOTEM     = 116947,
    SPELL_SHA_EARTHGRAB                         = 64695,
    SPELL_SHA_ECHO_OF_THE_ELEMENTS              = 108283,
    SPELL_SHA_ANCESTRAL_GUIDANCE                = 114911,
    SPELL_SHA_CONDUCTIVITY_TALENT               = 108282,
    SPELL_SHA_CONDUCTIVITY_HEAL                 = 118800,
    SPELL_SHA_GLYPH_OF_LAKESTRIDER              = 55448,
    SPELL_SHA_WATER_WALKING                     = 546,
    SPELL_SHA_GLYPH_OF_SHAMANISTIC_RAGE         = 63280,
    SPELL_SHA_SOLAR_BEAM                        = 113286,
    SPELL_SHA_SOLAR_BEAM_SILENCE                = 113288,
    SPELL_SHA_GHOST_WOLF                        = 2645,
    SPELL_SHA_ITEM_T14_4P                       = 123124,
    SPELL_SHA_GLYPH_OF_HEALING_STREAM_TOTEM     = 55456,
    SPELL_SHA_ITEM_S12_4P_ENHANCEMENT_BONUS     = 131554,
    SPELL_SHA_RESTORATIVE_MISTS                 = 114083,
    SPELL_SHA_FROST_SHOCK                       = 8056,
    SPELL_SHA_LAVA_SURGE_AURA                   = 77756,
    SPELL_SHA_LAVA_BURST                        = 51505,
    SPELL_SPIRIT_HUNT_HEAL                      = 58879,
    SPELL_SHA_WINDFURY_ATTACK                   = 25504,
    SPELL_SHA_LAMETONGUE_ATTACK                 = 10444,
    SPELL_SHA_PVP_BONUS_WOD_2                   = 166103,
    SPELL_SHA_PVP_BONUS_WOD_4                   = 171121,
    SPELL_SHA_LIGHTNING_SHIELD                  = 324,
    SPELL_SHA_IMPROVED_CHAIN_LIGHTNING          = 157766,
    SPELL_SHA_ECHO_OF_THE_ELEMENTS_ELEMENTAL    = 159101,
    SPELL_SHA_ECHO_OF_THE_ELEMENTS_ENHANCEMENT  = 159103,
    SPELL_SHA_ECHO_OF_THE_ELEMENTS_RESTORATION  = 159105,
    SPELL_SHA_LAVA_LASH_SPREAD                  = 105792,
    SPELL_SHA_LIQUID_MAGMA_DAMAGE               = 177601,
    SPELL_SHA_GLYPH_OF_GHOSTLY_SPEED            = 159642,
    SPELL_SHA_ELEMENTAL_BLAST_CRIT_BONUS        = 118522,
    SPELL_SHA_ELEMENTAL_BLAST_HASTE_BONUS       = 173183,
    SPELL_SHA_ELEMENTAL_BLAST_MASTERY_BONUS     = 173184,
    SPELL_SHA_ELEMENTAL_BLAST_MULTISTRIKE_BONUS = 173185,
    SPELL_SHA_ELEMENTAL_BLAST_AGILITY_BONUS     = 173186,
    SPELL_SHA_ELEMENTAL_BLAST_SPIRIT_BONUS      = 173187,
    SPELL_SHA_ELEMENTAL_FUSION                  = 152257,
    SPELL_SHA_ELEMENTAL_FUSION_PROC             = 157174,
    SPELL_SHA_IMPROVED_LIGHTNING_SHIELD         = 157774,
    SPELL_SHA_UNLEASH_FLAME_AURA                = 73683
};

/// Called by Unleash Flame - 165462, Unleash Life - 73685 and Unleash Elements - 73680
/// Unleashed Fury - 165477
class spell_sha_unleashed_fury : public SpellScriptLoader
{
    public:
        spell_sha_unleashed_fury() : SpellScriptLoader("spell_sha_unleashed_fury") { }

        class spell_sha_unleashed_fury_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_unleashed_fury_SpellScript);

            enum eSpells
            {
                UnleashedFuryTalentElemental    = 165477,
                UnleashedFuryTalentEnhancement  = 117012,
                UnleashedFuryTalentRestauration = 165479,
                /// Increases the damage dealt by your Lightning Bolt by 30% and Lava Burst by 10% for 10 sec.
                UnleashedFuryElemental          = 118470,
                /// Increases the damage dealt by your Lightning Bolt by 30% and increases your multistrike chance by 5% for 8 sec.
                UnleashedFuryEnhancement        = 118472,
                /// Increases the healing done by your next single-target heal by 50%, for 10 sec.
                UnleashedRestauration           = 118473,

                UnleashElements                 = 73680,
                UnleashLife                     = 73685,
                UnleashFlame                    = 165462
            };

            void HandleDummy(SpellEffIndex)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (!l_Caster->HasAura(eSpells::UnleashedFuryTalentElemental) && !l_Caster->HasAura(eSpells::UnleashedFuryTalentEnhancement) && !l_Caster->HasAura(eSpells::UnleashedFuryTalentRestauration))
                        return;

                    switch (GetSpellInfo()->Id)
                    {
                        case eSpells::UnleashElements:
                            l_Caster->CastSpell(l_Caster, eSpells::UnleashedFuryEnhancement, true);
                            break;
                        case eSpells::UnleashLife:
                            l_Caster->CastSpell(l_Caster, eSpells::UnleashedRestauration, true);
                            break;
                        case eSpells::UnleashFlame:
                            l_Caster->CastSpell(l_Caster, eSpells::UnleashedFuryElemental, true);
                            break;
                        default:
                            break;
                    }
                }
            }

            void Register()
            {
                switch (m_scriptSpellId)
                {
                    case eSpells::UnleashElements:
                    case eSpells::UnleashFlame:
                        OnEffectHitTarget += SpellEffectFn(spell_sha_unleashed_fury_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
                        break;
                    case eSpells::UnleashLife:
                        OnEffectHitTarget += SpellEffectFn(spell_sha_unleashed_fury_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_HEAL);
                        break;
                    default:
                        break;
                }
            }
        };

        class spell_sha_unleashed_fury_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sha_unleashed_fury_AuraScript);

            enum eSpells
            {
                UnleashLife = 73685,
                UnleashedFuryTalentRestauration = 165479,
                UnleashedRestauration = 118473
            };

            void CalculateAmount(constAuraEffectPtr /*p_AurEff*/, int32& p_Amount, bool& /*p_CanBeRecalculated*/)
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster == nullptr)
                    return;

                SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(eSpells::UnleashedRestauration);

                if (l_Caster->HasAura(eSpells::UnleashedFuryTalentRestauration))
                    p_Amount += l_SpellInfo->Effects[EFFECT_0].BasePoints;
            }

            void OnRemove(constAuraEffectPtr aurEff, AuraEffectHandleModes /*mode*/)
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster == nullptr)
                    return;

                if (l_Caster->HasAura(eSpells::UnleashedRestauration))
                    l_Caster->RemoveAura(eSpells::UnleashedRestauration);
            }

            void Register()
            {
                switch (m_scriptSpellId)
                {
                    case eSpells::UnleashLife:
                        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_sha_unleashed_fury_AuraScript::CalculateAmount, EFFECT_1, SPELL_AURA_ADD_PCT_MODIFIER);
                        DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_sha_unleashed_fury_AuraScript::CalculateAmount, EFFECT_2, SPELL_AURA_DUMMY);
                        OnEffectRemove += AuraEffectRemoveFn(spell_sha_unleashed_fury_AuraScript::OnRemove, EFFECT_1, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
                    break;
                    default:
                        break;
                }
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sha_unleashed_fury_AuraScript();
        }

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_unleashed_fury_SpellScript();
        }
};

/// Called by Chain Heal - 1064
/// High Tide - 157154
class spell_sha_high_tide : public SpellScriptLoader
{
    public:
        spell_sha_high_tide() : SpellScriptLoader("spell_sha_high_tide") { }

        class spell_sha_high_tide_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_high_tide_SpellScript);

            enum eSpells
            {
                SpellHighTide   = 157154,
                SpellRiptide    = 61295
            };

            void FilterTargets(std::list<WorldObject*>& p_Targets)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (!l_Caster->HasAura(eSpells::SpellHighTide))
                        return;

                    std::map<uint64, WorldObject*> l_TargetMap;
                    for (WorldObject* l_Object : p_Targets)
                        l_TargetMap.insert(std::make_pair(l_Object->GetGUID(), l_Object));

                    std::list<Unit*> l_TempList;
                    JadeCore::AnyFriendlyUnitInObjectRangeCheck l_Check(l_Caster, l_Caster, GetSpellInfo()->RangeEntry->maxRangeFriend);
                    JadeCore::UnitListSearcher<JadeCore::AnyFriendlyUnitInObjectRangeCheck> l_Searcher(l_Caster, l_TempList, l_Check);
                    l_Caster->VisitNearbyObject(GetSpellInfo()->RangeEntry->maxRangeFriend, l_Searcher);

                    l_TempList.remove_if([this, l_TargetMap, l_Caster](Unit* p_Unit) -> bool
                    {
                        if (p_Unit == nullptr || !p_Unit->HasAura(eSpells::SpellRiptide) || p_Unit == l_Caster)
                            return true;

                        /// Already in list
                        if (l_TargetMap.find(p_Unit->GetGUID()) != l_TargetMap.end())
                            return true;

                        return false;
                    });

                    if (l_TempList.empty())
                        return;

                    l_TempList.sort(JadeCore::HealthPctOrderPred());
                    uint8 l_TargetCount = GetSpellInfo()->Effects[EFFECT_1].BasePoints;

                    for (Unit* l_Unit : l_TempList)
                    {
                        if (!l_TargetCount)
                            break;

                        p_Targets.push_back(l_Unit);
                        --l_TargetCount;
                    }
                }
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sha_high_tide_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_TARGET_CHAINHEAL_ALLY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_high_tide_SpellScript();
        }
};

/// Totemic Projection - 108287
class spell_sha_totemic_projection: public SpellScriptLoader
{
    public:
        spell_sha_totemic_projection() : SpellScriptLoader("spell_sha_totemic_projection") { }

        class spell_sha_totemic_projection_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_totemic_projection_SpellScript);

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (Unit* caster = GetCaster())
                {
                    std::list<Creature*> leapList;
                    Unit* leapTarget = NULL;

                    caster->GetCreatureListWithEntryInGrid(leapList, 47319, 45.0f);

                    for (auto itr : leapList)
                    {
                        if (!itr->ToUnit() || !itr->ToTempSummon())
                            continue;

                        if (!itr->ToTempSummon()->GetSummoner())
                            continue;

                        if (itr->ToTempSummon()->GetSummoner()->GetGUID() != caster->GetGUID())
                            continue;

                        leapTarget = itr->ToUnit();
                        break;
                    }

                    if (!leapTarget)
                        return;

                    for (Unit::ControlList::const_iterator itr = caster->m_Controlled.begin(); itr != caster->m_Controlled.end(); ++itr)
                    {
                        if ((*itr)->isTotem())
                        {
                            SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo((*itr)->GetUInt32Value(UNIT_FIELD_CREATED_BY_SPELL));
                            if (!spellInfo)
                                continue;

                            float objSize = caster->GetObjectSize();
                            float angle = spellInfo->Effects[0].TargetA.CalcDirectionAngle();
                            float dist = spellInfo->Effects[0].CalcRadius(caster);

                            if (dist < objSize)
                                dist = objSize;

                            Position pos;
                            leapTarget->GetPosition(&pos);

                            angle += (*itr)->GetOrientation();
                            float destx, desty, destz, ground, floor;
                            destx = pos.m_positionX + dist * std::cos(angle);
                            desty = pos.m_positionY + dist * std::sin(angle);
                            ground = (*itr)->GetMap()->GetHeight((*itr)->GetPhaseMask(), destx, desty, MAX_HEIGHT, true);
                            floor = (*itr)->GetMap()->GetHeight((*itr)->GetPhaseMask(), destx, desty, pos.m_positionZ, true);
                            destz = fabs(ground - pos.m_positionZ) <= fabs(floor - pos.m_positionZ) ? ground : floor;

                            (*itr)->NearTeleportTo(destx, desty, destz, (*itr)->GetOrientation());
                        }
                    }
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_sha_totemic_projection_SpellScript::HandleDummy, EFFECT_1, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_totemic_projection_SpellScript();
        }
};

/// Water Ascendant - 114052
class spell_sha_water_ascendant: public SpellScriptLoader
{
    public:
        spell_sha_water_ascendant() : SpellScriptLoader("spell_sha_water_ascendant") { }

        class spell_sha_water_ascendant_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sha_water_ascendant_AuraScript);

            void OnProc(constAuraEffectPtr aurEff, ProcEventInfo& eventInfo)
            {
                PreventDefaultAction();

                if (!GetCaster())
                    return;

                Player* _player = GetCaster()->ToPlayer();
                if (!_player)
                    return;

                if (_player->HasSpellCooldown(SPELL_SHA_RESTORATIVE_MISTS))
                    return;

                if (eventInfo.GetActor()->GetGUID() != _player->GetGUID())
                    return;

                if (!eventInfo.GetDamageInfo()->GetSpellInfo())
                    return;

                if (eventInfo.GetDamageInfo()->GetSpellInfo()->Id == SPELL_SHA_RESTORATIVE_MISTS)
                    return;

                if (!(eventInfo.GetHealInfo()->GetHeal()))
                    return;

                if (!(eventInfo.GetDamageInfo()->GetSpellInfo()->IsPositive()))
                    return;

                if (Unit* target = eventInfo.GetActionTarget())
                {
                    std::list<Unit*> tempList;
                    std::list<Unit*> alliesList;
                    target->GetAttackableUnitListInRange(tempList, 15.0f);

                    for (auto itr : tempList)
                    {
                        if (!_player->IsWithinLOSInMap(itr))
                            continue;

                        if (itr->IsHostileTo(_player))
                            continue;

                        alliesList.push_back(itr);
                    }

                    if (!alliesList.empty())
                    {
                        // Heal amount distribued for all allies, caster included
                        int32 bp = eventInfo.GetHealInfo()->GetHeal() / alliesList.size();

                        if (bp > 0)
                            _player->CastCustomSpell((*alliesList.begin()), SPELL_SHA_RESTORATIVE_MISTS, &bp, NULL, NULL, true);   // Restorative Mists

                        _player->AddSpellCooldown(SPELL_SHA_RESTORATIVE_MISTS, 0, 1 * IN_MILLISECONDS);               // This prevent from multiple procs
                    }
                }
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_sha_water_ascendant_AuraScript::OnProc, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sha_water_ascendant_AuraScript();
        }
};

/// Called by Shamanistic Rage - 30823
/// Glyph of Shamanistic Rage - 63280
class spell_sha_glyph_of_shamanistic_rage: public SpellScriptLoader
{
    public:
        spell_sha_glyph_of_shamanistic_rage() : SpellScriptLoader("spell_sha_glyph_of_shamanistic_rage") { }

        class spell_sha_glyph_of_shamanistic_rage_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_glyph_of_shamanistic_rage_SpellScript);

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                {
                    if (caster->HasAura(SPELL_SHA_GLYPH_OF_SHAMANISTIC_RAGE))
                    {
                        DispelChargesList dispelList;
                        caster->GetDispellableAuraList(caster, DISPEL_ALL_MASK, dispelList);

                        for (auto itr : dispelList)
                            caster->RemoveAura(itr.first);
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_sha_glyph_of_shamanistic_rage_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_glyph_of_shamanistic_rage_SpellScript();
        }
};

/// Purge - 370
class spell_sha_purge : public SpellScriptLoader
{
    public:
        spell_sha_purge() : SpellScriptLoader("spell_sha_purge") { }

        class spell_sha_purge_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_purge_SpellScript);

            enum eSpells
            {
                GlyphOfPurging  = 147762,
                MaelstromWeapon = 53817
            };

            void HandleOnHit()
            {
                Unit* l_Caster = GetCaster();
                
                if (l_Caster->HasAura(eSpells::GlyphOfPurging))
                    l_Caster->CastSpell(l_Caster, eSpells::MaelstromWeapon, true);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_sha_purge_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_purge_SpellScript();
        }
};

/// Called by Ghost Wolf - 2645
/// Glyph of Lakestrider - 55448
class spell_sha_glyph_of_lakestrider: public SpellScriptLoader
{
    public:
        spell_sha_glyph_of_lakestrider() : SpellScriptLoader("spell_sha_glyph_of_lakestrider") { }

        class spell_sha_glyph_of_lakestrider_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_glyph_of_lakestrider_SpellScript);

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                    if (caster->HasAura(SPELL_SHA_GLYPH_OF_LAKESTRIDER))
                        caster->CastSpell(caster, SPELL_SHA_WATER_WALKING, true);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_sha_glyph_of_lakestrider_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_glyph_of_lakestrider_SpellScript();
        }
};

uint32 g_resetTotemCdSpells[] =
{
    108269, // Capacitor Totem
    2484,   // Earthbind Totem
    8177,   // Grounding Totem
    5394,   // Healing Stream Totem
    8143,   // Tremor Totem
    108270, // Stone Bulwark Totem
    51485,  // Earthgrab Totem
    108273  // Windwalk Totem
};

/// 108285 - Call of the Elements
class spell_sha_call_of_the_elements: public SpellScriptLoader
{
    public:
        spell_sha_call_of_the_elements() : SpellScriptLoader("spell_sha_call_of_the_elements") { }

        class spell_sha_call_of_the_elements_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_call_of_the_elements_SpellScript);

            void OnSpellHit()
            {
                Player* l_Player = GetCaster()->ToPlayer();

                if (!l_Player)
                    return;

                for (int l_I = 0; l_I < sizeof(g_resetTotemCdSpells) / sizeof(uint32); l_I++)
                    l_Player->RemoveSpellCooldown(g_resetTotemCdSpells[l_I], true);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_sha_call_of_the_elements_SpellScript::OnSpellHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_call_of_the_elements_SpellScript();
        }
};

/// Called by Healing Wave - 331, Greater Healing Wave - 77472 and Healing Surge - 8004
/// Called by Lightning Bolt - 403, Chain Lightning - 421, Earth Shock - 8042 and Stormstrike - 17364
/// Called by Lightning Bolt - 45284, Chain Lightning - 45297
/// Conductivity - 108282
class spell_sha_conductivity: public SpellScriptLoader
{
    public:
        spell_sha_conductivity() : SpellScriptLoader("spell_sha_conductivity") { }

        class spell_sha_conductivity_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_conductivity_SpellScript);

            void HandleAfterHit()
            {
                Unit* l_Caster = GetCaster();

                if (AuraPtr l_Conductivity = l_Caster->GetAura(SPELL_SHA_CONDUCTIVITY_TALENT))
                {
                    if (DynamicObject* l_DynObj = l_Caster->GetDynObject(SPELL_SHA_HEALING_RAIN))
                    {
                        int32 l_RemainingDuration = l_Conductivity->GetEffect(EFFECT_0)->GetAmount() * 10;
                        uint32 l_AddDuration = std::min(l_RemainingDuration, 4000);

                        l_DynObj->SetDuration(l_DynObj->GetDuration() + l_AddDuration);
                        l_Conductivity->GetEffect(EFFECT_0)->SetAmount((l_RemainingDuration - l_AddDuration) / 10);

                        if (AuraPtr l_HealingRain = l_Caster->GetAura(SPELL_SHA_HEALING_RAIN_AURA))
                        {
                            l_HealingRain->SetDuration(l_HealingRain->GetDuration() + l_AddDuration);
                            l_HealingRain->SetMaxDuration(l_HealingRain->GetMaxDuration() + l_AddDuration);
                        }
                    }
                }
            }

            void Register()
            {
                AfterHit += SpellHitFn(spell_sha_conductivity_SpellScript::HandleAfterHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_conductivity_SpellScript();
        }
};

/// Ancestral Guidance - 108281
class spell_sha_ancestral_guidance: public SpellScriptLoader
{
    public:
        spell_sha_ancestral_guidance() : SpellScriptLoader("spell_sha_ancestral_guidance") { }

        class spell_sha_ancestral_guidance_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sha_ancestral_guidance_AuraScript);

            void OnProc(constAuraEffectPtr aurEff, ProcEventInfo& eventInfo)
            {
                PreventDefaultAction();

                if (!GetCaster())
                    return;

                // Prevent inifinite loop
                if (eventInfo.GetDamageInfo()->GetSpellInfo() &&
                    (eventInfo.GetDamageInfo()->GetSpellInfo()->Id == GetSpellInfo()->Id ||
                    eventInfo.GetDamageInfo()->GetSpellInfo()->Id == SPELL_SHA_RESTORATIVE_MISTS ||
                    eventInfo.GetDamageInfo()->GetSpellInfo()->Id == SPELL_SHA_ANCESTRAL_GUIDANCE))
                    return;

                Player* _player = GetCaster()->ToPlayer();
                if (!_player)
                    return;

                if (Unit* target = eventInfo.GetActionTarget())
                {
                    int32 bp = eventInfo.GetDamageInfo()->GetDamage() > eventInfo.GetHealInfo()->GetHeal() ? eventInfo.GetDamageInfo()->GetDamage() : eventInfo.GetHealInfo()->GetHeal();
                    if (!bp)
                        return;

                    bp = int32(bp * aurEff->GetAmount() / 100);

                    _player->CastCustomSpell(target, SPELL_SHA_ANCESTRAL_GUIDANCE, &bp, NULL, NULL, true);
                }
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_sha_ancestral_guidance_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sha_ancestral_guidance_AuraScript();
        }
};

/// 64695 - Earthgrab
class spell_sha_earthgrab: public SpellScriptLoader
{
    public:
        spell_sha_earthgrab() : SpellScriptLoader("spell_sha_earthgrab") { }

        class spell_sha_earthgrab_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_earthgrab_SpellScript);

            void HitTarget(SpellEffIndex)
            {
                Unit* target = GetHitUnit();

                if (target->HasAura(SPELL_SHA_EARTHGRAB_IMMUNITY))
                {
                    GetCaster()->CastSpell(target, SPELL_SHA_EARTHBIND_FOR_EARTHGRAB_TOTEM, true);
                    PreventHitAura();
                    return;
                }

                if (Unit* target = GetHitUnit())
                    GetCaster()->CastSpell(target, SPELL_SHA_EARTHGRAB_IMMUNITY, true);
            }

            void FilterTargets(std::list<WorldObject*>& unitList)
            {
                for (std::list<WorldObject*>::iterator iter = unitList.begin(); iter != unitList.end();)
                    if (Unit* target = (*iter)->ToUnit())
                        if (target->HasAura(SPELL_SHA_EARTHGRAB))
                            iter = unitList.erase(iter);
                        else
                            iter++;
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_sha_earthgrab_SpellScript::HitTarget, EFFECT_0, SPELL_EFFECT_APPLY_AURA);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sha_earthgrab_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_earthgrab_SpellScript();
        }
};

/// Stone Bulwark - 114889
class spell_sha_stone_bulwark: public SpellScriptLoader
{
    public:
        spell_sha_stone_bulwark() : SpellScriptLoader("spell_sha_stone_bulwark") { }

        class spell_sha_stone_bulwark_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sha_stone_bulwark_AuraScript);

            void OnTick(constAuraEffectPtr p_AurEff)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (Unit* l_Owner = l_Caster->GetOwner())
                    {
                        float spellPower = spellPower = l_Owner->ToPlayer()->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_ALL);
                        int32 l_Amount = 0.875f * spellPower;

                        if (AuraPtr aura = l_Owner->GetAura(SPELL_SHA_STONE_BULWARK_ABSORB))
                            l_Amount += aura->GetEffect(EFFECT_0)->GetAmount();
                        else if (p_AurEff->GetTickNumber() == 1)
                            l_Amount *= 4.f;

                        if (AuraPtr aura = l_Caster->AddAura(SPELL_SHA_STONE_BULWARK_ABSORB, l_Owner))
                            aura->GetEffect(EFFECT_0)->SetAmount(l_Amount);

                        PreventDefaultAction();
                    }
                }

            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_sha_stone_bulwark_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sha_stone_bulwark_AuraScript();
        }
};

/// Frost Shock - 8056
class spell_sha_frozen_power: public SpellScriptLoader
{
    public:
        spell_sha_frozen_power() : SpellScriptLoader("spell_sha_frozen_power") { }

        class spell_sha_frozen_power_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_frozen_power_SpellScript);

            bool Validate(SpellInfo const* /*spellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_SHA_FROST_SHOCK))
                    return false;
                return true;
            }

            void HandleAfterHit()
            {
                if (Unit* caster = GetCaster())
                    if (Unit* target = GetHitUnit())
                        if (caster->HasAura(SPELL_SHA_FROZEN_POWER))
                            caster->CastSpell(target, SPELL_SHA_FROST_SHOCK_FREEZE, true);
            }

            void Register()
            {
                AfterHit += SpellHitFn(spell_sha_frozen_power_SpellScript::HandleAfterHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_frozen_power_SpellScript();
        }
};

/// Spirit Link - 98020 : triggered by 98017
/// Spirit Link Totem
class spell_sha_spirit_link: public SpellScriptLoader
{
    public:
        spell_sha_spirit_link() : SpellScriptLoader("spell_sha_spirit_link") { }

        class spell_sha_spirit_link_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_spirit_link_SpellScript);

            void HandleAfterCast()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (l_Caster->GetEntry() == 53006)
                    {
                        if (l_Caster->GetOwner())
                        {
                            if (Player* _player = l_Caster->GetOwner()->ToPlayer())
                            {
                                std::list<Unit*> l_MemberList;
                                _player->GetRaidMembers(l_MemberList);

                                float l_Radius = GetSpellInfo()->Effects[EFFECT_0].CalcRadius(l_Caster, GetSpell());

                                l_MemberList.remove_if([l_Caster, l_Radius](Unit const* p_Unit) -> bool
                                {
                                    if (p_Unit->GetDistance(*l_Caster) > l_Radius)
                                        return true;

                                    return false;
                                });

                                float l_TotalRaidHealthPct = 0;

                                for (auto l_Itr : l_MemberList)
                                    l_TotalRaidHealthPct += l_Itr->GetHealthPct();

                                l_TotalRaidHealthPct /= l_MemberList.size() * 100.0f;

                                for (auto l_Itr : l_MemberList)
                                    l_Itr->SetHealth(uint32(l_TotalRaidHealthPct * l_Itr->GetMaxHealth()));
                            }
                        }
                    }
                }
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_sha_spirit_link_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_spirit_link_SpellScript();
        }
};

/// Fire Nova - 1535
class spell_sha_fire_nova: public SpellScriptLoader
{
    public:
        spell_sha_fire_nova() : SpellScriptLoader("spell_sha_fire_nova") { }

        class spell_sha_fire_nova_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_fire_nova_SpellScript);

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                Unit* caster = GetCaster();
                if (Unit* target = GetHitUnit())
                    if (target->HasAura(SPELL_SHA_FLAME_SHOCK))
                        caster->CastSpell(target, SPELL_SHA_FIRE_NOVA_TRIGGERED, true);
            }

            SpellCastResult HandleCheckCast()
            {
                UnitList targets;
                JadeCore::AnyUnitHavingBuffInObjectRangeCheck u_check(GetCaster(), GetCaster(), 100, SPELL_SHA_FLAME_SHOCK, false);
                JadeCore::UnitListSearcher<JadeCore::AnyUnitHavingBuffInObjectRangeCheck> searcher(GetCaster(), targets, u_check);
                GetCaster()->VisitNearbyObject(100, searcher);

                return targets.size() == 0 ? SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW : SPELL_CAST_OK;
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_sha_fire_nova_SpellScript::HandleCheckCast);
                OnEffectHitTarget += SpellEffectFn(spell_sha_fire_nova_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_fire_nova_SpellScript();
        }
};

/// 88766 Fulmination handled in 8042 Earth Shock
class spell_sha_fulmination: public SpellScriptLoader
{
    public:
        spell_sha_fulmination() : SpellScriptLoader("spell_sha_fulmination") { }

        class spell_sha_fulmination_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_fulmination_SpellScript)

            void HandleAfterHit()
            {
                if (Unit* _player = GetCaster())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        AuraPtr lightningShield = _player->GetAura(SPELL_SHA_LIGHTNING_SHIELD_AURA);
                        if (!lightningShield)
                            return;

                        uint8 charges = lightningShield->GetCharges() - 1;
                        if (!charges)
                            return;

                        SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(SPELL_SHA_LIGHTNING_SHIELD_ORB_DAMAGE);
                        if (!spellInfo)
                            return;

                        int32 basePoints = _player->CalculateSpellDamage(target, spellInfo, EFFECT_0);
                        uint32 damage = charges * _player->SpellDamageBonusDone(target, spellInfo, basePoints, EFFECT_0, SPELL_DIRECT_DAMAGE);

                        _player->CastCustomSpell(SPELL_SHA_FULMINATION_TRIGGERED, SPELLVALUE_BASE_POINT0, damage, target, true);
                        lightningShield->SetCharges(1);

                        _player->RemoveAura(SPELL_SHA_FULMINATION_INFO);
                    }
                }
            }

            void Register()
            {
                AfterHit += SpellHitFn(spell_sha_fulmination_SpellScript::HandleAfterHit);
            }
        };

        SpellScript *GetSpellScript() const
        {
            return new spell_sha_fulmination_SpellScript();
        }
};

/// 77762 - Lava Surge
class spell_sha_lava_surge: public SpellScriptLoader
{
    public:
        spell_sha_lava_surge() : SpellScriptLoader("spell_sha_lava_surge") { }

        class spell_sha_lava_surge_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sha_lava_surge_AuraScript);

            enum eData
            {
                CategoryID = 1536
            };

            void OnApply(constAuraEffectPtr p_AurEff, AuraEffectHandleModes /*p_Mode*/)
            {
                if (GetCaster() == nullptr)
                    return;

                Player* l_Player = GetCaster()->ToPlayer();
                if (l_Player == nullptr)
                    return;

                l_Player->RestoreCharge(eData::CategoryID);
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_sha_lava_surge_AuraScript::OnApply, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sha_lava_surge_AuraScript();
        }
};

/// Healing Stream - 52042
class spell_sha_healing_stream: public SpellScriptLoader
{
    public:
        spell_sha_healing_stream() : SpellScriptLoader("spell_sha_healing_stream") { }

        class spell_sha_healing_stream_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_healing_stream_SpellScript);

            bool Validate(SpellInfo const* /*spell*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_SHA_HEALING_STREAM))
                    return false;
                return true;
            }

            void HandleOnHit()
            {
                if (!GetCaster()->GetOwner())
                    return;

                if (Unit* l_Owner = GetCaster()->GetOwner())
                {
                    if (Unit* l_Target = GetHitUnit())
                    {
                        /// Glyph of Healing Stream Totem
                        if (l_Owner->HasAura(SPELL_SHA_GLYPH_OF_HEALING_STREAM_TOTEM))
                            l_Owner->CastSpell(l_Target, SPELL_SHA_GLYPH_OF_HEALING_STREAM, true);
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_sha_healing_stream_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_healing_stream_SpellScript();
        }
};

/// Elemental Blast - 117014
class spell_sha_elemental_blast: public SpellScriptLoader
{
    public:
        spell_sha_elemental_blast() : SpellScriptLoader("spell_sha_elemental_blast") { }

        class spell_sha_elemental_blast_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_elemental_blast_SpellScript);

            bool Validate(SpellInfo const* /*spell*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_SHA_ELEMENTAL_BLAST))
                    return false;
                return true;
            }

            void HandleAfterCast()
            {
                if (Unit* caster = GetCaster())
                {
                    if (Unit* target = GetExplTargetUnit())
                    {
                        caster->CastSpell(target, SPELL_SHA_ELEMENTAL_BLAST_FROST_VISUAL, true);
                        caster->CastSpell(target, SPELL_SHA_ELEMENTAL_BLAST_NATURE_VISUAL, true);
                    }
                }
            }

            void HandleOnHit()
            {
                std::vector<uint32> l_SpellsToCast =
                {
                    SPELL_SHA_ELEMENTAL_BLAST_CRIT_BONUS,
                    SPELL_SHA_ELEMENTAL_BLAST_HASTE_BONUS,
                    SPELL_SHA_ELEMENTAL_BLAST_MASTERY_BONUS,
                    SPELL_SHA_ELEMENTAL_BLAST_MULTISTRIKE_BONUS
                };

                if (Player* l_Player = GetCaster()->ToPlayer())
                {
                    if (l_Player->GetSpecializationId(l_Player->GetActiveSpec()) == SPEC_SHAMAN_ENHANCEMENT)
                        l_SpellsToCast.push_back(SPELL_SHA_ELEMENTAL_BLAST_AGILITY_BONUS);
                    else if (l_Player->GetSpecializationId(l_Player->GetActiveSpec()) == SPEC_SHAMAN_RESTORATION)
                        l_Player->CastSpell(l_Player, SPELL_SHA_ELEMENTAL_BLAST_SPIRIT_BONUS);
                }

                GetCaster()->CastSpell(GetCaster(), l_SpellsToCast[urand(0, l_SpellsToCast.size() - 1)], true);
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_sha_elemental_blast_SpellScript::HandleAfterCast);
                OnHit += SpellHitFn(spell_sha_elemental_blast_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_elemental_blast_SpellScript();
        }
};

/// Earthquake - 61882
/// last update : 6.1.2 19802
class spell_sha_earthquake: public SpellScriptLoader
{
    public:
        spell_sha_earthquake() : SpellScriptLoader("spell_sha_earthquake") { }

        class spell_sha_earthquake_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sha_earthquake_AuraScript);

            enum eSpells
            {
                Earthquake     = 61882,
                EarthquakeTick = 77478
            };

            void OnTick(constAuraEffectPtr aurEff)
            {
                Unit* l_Caster = GetCaster();
                if (!l_Caster)
                    return;

                AreaTrigger* l_AreaTrigger = l_Caster->GetAreaTrigger(eSpells::Earthquake);
                if (!l_AreaTrigger)
                    return;

                /// dealing ${$SPN * 0.11 * 10 * (1 + $170374m3 / 100)} Physical damage over $d
                int32 l_Bp0 = l_Caster->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_SPELL) * 0.11 * 10;

                if (AuraPtr l_ChainLightning = l_Caster->GetAura(SPELL_SHA_IMPROVED_CHAIN_LIGHTNING))
                {
                    l_Bp0 += CalculatePct(l_Bp0, l_ChainLightning->GetEffect(EFFECT_0)->GetAmount());
                    l_ChainLightning->Remove();
                }

                l_Bp0 /= GetSpellInfo()->GetDuration() / IN_MILLISECONDS;

                l_Caster->CastCustomSpell(l_AreaTrigger->GetPositionX(), l_AreaTrigger->GetPositionY(), l_AreaTrigger->GetPositionZ(), eSpells::EarthquakeTick, &l_Bp0, nullptr, nullptr, true);
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_sha_earthquake_AuraScript::OnTick, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sha_earthquake_AuraScript();
        }
};

/// Earthquake : Ticks - 77478
/// last update : 6.1.2 19802
class spell_sha_earthquake_tick: public SpellScriptLoader
{
public:
    spell_sha_earthquake_tick() : SpellScriptLoader("spell_sha_earthquake_tick") { }

    class spell_sha_earthquake_tick_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_sha_earthquake_tick_SpellScript);

        enum eSpells
        {
            EarthquakeTick      = 77478,
            EarthquakeKnockDown = 77505
        };

        bool Validate(SpellInfo const* /*spell*/)
        {
            if (!sSpellMgr->GetSpellInfo(eSpells::EarthquakeTick))
                return false;
            return true;
        }

        void HandleOnHit()
        {
            Unit* l_Caster = GetCaster();
            Unit* l_Target = GetHitUnit();
            if (!l_Caster || !l_Target)
                return;

            /// 10% chance of knocking down affected targets
            if (roll_chance_i(GetSpellInfo()->Effects[EFFECT_1].BasePoints))
                l_Caster->CastSpell(l_Target, eSpells::EarthquakeKnockDown, true);
        }

        void Register()
        {
            OnHit += SpellHitFn(spell_sha_earthquake_tick_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_sha_earthquake_tick_SpellScript();
    }
};

/// Healing Rain - 73920
class spell_sha_healing_rain: public SpellScriptLoader
{
    public:
        spell_sha_healing_rain() : SpellScriptLoader("spell_sha_healing_rain") { }

        class spell_sha_healing_rain_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_healing_rain_SpellScript);

            void HandleOnHit()
            {
                if (WorldLocation const* loc = GetExplTargetDest())
                {
                    if (Unit* caster = GetCaster())
                    {
                        // Casting a second healing rain after prolonging the previous one using conductivity
                        // will replace the old healing rain with base amount of duration (in other words, you will not have 2 healing rains).
                        if (caster->GetDynObject(SPELL_SHA_HEALING_RAIN))
                            caster->RemoveDynObject(SPELL_SHA_HEALING_RAIN);

                        caster->CastSpell(loc->GetPositionX(), loc->GetPositionY(), loc->GetPositionZ(), SPELL_SHA_HEALING_RAIN, true);

                        if (AuraPtr conductivity = caster->GetAura(SPELL_SHA_CONDUCTIVITY_TALENT))
                            conductivity->GetEffect(EFFECT_0)->SetAmount(conductivity->GetSpellInfo()->Effects[EFFECT_0].BasePoints);
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_sha_healing_rain_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_healing_rain_SpellScript();
        }

        class spell_sha_healing_rain_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sha_healing_rain_AuraScript);

            void OnTick(constAuraEffectPtr aurEff)
            {
                if (!GetCaster())
                    return;

                if (DynamicObject* dynObj = GetCaster()->GetDynObject(SPELL_SHA_HEALING_RAIN))
                    GetCaster()->CastSpell(dynObj->GetPositionX(), dynObj->GetPositionY(), dynObj->GetPositionZ(), SPELL_SHA_HEALING_RAIN_TICK, true);
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_sha_healing_rain_AuraScript::OnTick, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sha_healing_rain_AuraScript();
        }
};

/// last update : 6.1.2 19802
/// Healing Rain (heal) - 73921
class spell_sha_healing_rain_heal : public SpellScriptLoader
{
    public:
        spell_sha_healing_rain_heal() : SpellScriptLoader("spell_sha_healing_rain_heal") { }

        class spell_sha_healing_rain_heal_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_healing_rain_heal_SpellScript);

            void FilterTargets(std::list<WorldObject*>& p_Targets)
            {
                /// Healing up to 6 allies 
                if (p_Targets.size() > 6)
                    JadeCore::RandomResizeList(p_Targets, 6);
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sha_healing_rain_heal_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ALLY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_healing_rain_heal_SpellScript();
        }
};

/// Ascendance - 114049
class spell_sha_ascendance: public SpellScriptLoader
{
    public:
        spell_sha_ascendance() : SpellScriptLoader("spell_sha_ascendance") { }

        class spell_sha_ascendance_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_ascendance_SpellScript);

            bool Validate(SpellInfo const* spellEntry)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_SHA_ASCENDANCE))
                    return false;
                return true;
            }

            SpellCastResult CheckCast()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (_player->GetSpecializationId(_player->GetActiveSpec()) == SPEC_NONE)
                    {
                        SetCustomCastResultMessage(SPELL_CUSTOM_ERROR_MUST_SELECT_TALENT_SPECIAL);
                        return SPELL_FAILED_CUSTOM_ERROR;
                    }
                    else
                        return SPELL_CAST_OK;
                }
                else
                    return SPELL_FAILED_DONT_REPORT;

                return SPELL_CAST_OK;
            }

            void HandleAfterCast()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    switch(_player->GetSpecializationId(_player->GetActiveSpec()))
                    {
                        case SPEC_SHAMAN_ELEMENTAL:
                            _player->CastSpell(_player, SPELL_SHA_ASCENDANCE_ELEMENTAL, true);
                            break;
                        case SPEC_SHAMAN_ENHANCEMENT:
                            _player->CastSpell(_player, SPELL_SHA_ASCENDANCE_ENHANCED, true);

                            if (_player->HasSpellCooldown(SPELL_SHA_STORMSTRIKE))
                                _player->RemoveSpellCooldown(SPELL_SHA_STORMSTRIKE, true);
                            break;
                        case SPEC_SHAMAN_RESTORATION:
                            _player->CastSpell(_player, SPELL_SHA_ASCENDANCE_RESTORATION, true);
                            break;
                        default:
                            break;
                    }
                }
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_sha_ascendance_SpellScript::CheckCast);
                AfterCast += SpellCastFn(spell_sha_ascendance_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_ascendance_SpellScript();
        }
};

class EarthenPowerTargetSelector
{
    public:
        EarthenPowerTargetSelector() { }

        bool operator() (WorldObject* target)
        {
            if (!target->ToUnit())
                return true;

            if (!target->ToUnit()->HasAuraWithMechanic(1 << MECHANIC_SNARE))
                return true;

            return false;
        }
};

/// Bloodlust - 2825
class spell_sha_bloodlust: public SpellScriptLoader
{
    public:
        spell_sha_bloodlust() : SpellScriptLoader("spell_sha_bloodlust") { }

        class spell_sha_bloodlust_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_bloodlust_SpellScript);

            bool Validate(SpellInfo const* /*spellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(SHAMAN_SPELL_SATED))
                    return false;
                return true;
            }

            void RemoveInvalidTargets(std::list<WorldObject*>& p_Targets)
            {
                p_Targets.remove_if(JadeCore::UnitAuraCheck(true, SHAMAN_SPELL_SATED));
                p_Targets.remove_if(JadeCore::UnitAuraCheck(true, HUNTER_SPELL_INSANITY));
                p_Targets.remove_if(JadeCore::UnitAuraCheck(true, SHAMAN_SPELL_EXHAUSTION));
                p_Targets.remove_if(JadeCore::UnitAuraCheck(true, MAGE_SPELL_TEMPORAL_DISPLACEMENT));
            }

            void ApplyDebuff()
            {
                if (Unit* l_Target = GetHitUnit())
                    l_Target->CastSpell(l_Target, SHAMAN_SPELL_SATED, true);
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sha_bloodlust_SpellScript::RemoveInvalidTargets, EFFECT_0, TARGET_UNIT_CASTER_AREA_RAID);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sha_bloodlust_SpellScript::RemoveInvalidTargets, EFFECT_1, TARGET_UNIT_CASTER_AREA_RAID);
                AfterHit += SpellHitFn(spell_sha_bloodlust_SpellScript::ApplyDebuff);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_bloodlust_SpellScript();
        }
};

/// Heroism - 32182
class spell_sha_heroism: public SpellScriptLoader
{
    public:
        spell_sha_heroism() : SpellScriptLoader("spell_sha_heroism") { }

        class spell_sha_heroism_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_heroism_SpellScript);

            SpellCastResult CheckCast()
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster->HasAura(SHAMAN_SPELL_EXHAUSTION))
                    return SPELL_FAILED_DONT_REPORT;

                return SPELL_CAST_OK;
            }

            bool Validate(SpellInfo const* /*spellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(SHAMAN_SPELL_EXHAUSTION))
                    return false;
                return true;
            }

            void RemoveInvalidTargets(std::list<WorldObject*>& p_Targets)
            {
                p_Targets.remove_if(JadeCore::UnitAuraCheck(true, SHAMAN_SPELL_EXHAUSTION));
                p_Targets.remove_if(JadeCore::UnitAuraCheck(true, HUNTER_SPELL_INSANITY));
                p_Targets.remove_if(JadeCore::UnitAuraCheck(true, SHAMAN_SPELL_SATED));
                p_Targets.remove_if(JadeCore::UnitAuraCheck(true, MAGE_SPELL_TEMPORAL_DISPLACEMENT));
            }

            void ApplyDebuff()
            {
                if (Unit* l_Target = GetHitUnit())
                    GetCaster()->CastSpell(l_Target, SHAMAN_SPELL_EXHAUSTION, true);
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_sha_heroism_SpellScript::CheckCast);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sha_heroism_SpellScript::RemoveInvalidTargets, EFFECT_0, TARGET_UNIT_CASTER_AREA_RAID);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sha_heroism_SpellScript::RemoveInvalidTargets, EFFECT_1, TARGET_UNIT_CASTER_AREA_RAID);
                AfterHit += SpellHitFn(spell_sha_heroism_SpellScript::ApplyDebuff);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_heroism_SpellScript();
        }
};

/// 58877 - Spirit Hunt
class spell_sha_spirit_hunt: public SpellScriptLoader
{
    public:
        spell_sha_spirit_hunt() : SpellScriptLoader("spell_sha_spirit_hunt") { }

        class spell_sha_spirit_hunt_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sha_spirit_hunt_AuraScript);

            void OnProc(constAuraEffectPtr aurEff, ProcEventInfo& eventInfo)
            {
                PreventDefaultAction();
                int32 bp0 = eventInfo.GetDamageInfo()->GetDamage();
                if (Unit* player = GetCaster()->GetOwner())
                    GetCaster()->CastCustomSpell(player, SPELL_SPIRIT_HUNT_HEAL, &bp0, NULL, NULL, true);
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_sha_spirit_hunt_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sha_spirit_hunt_AuraScript();
        }
};

/// 105792 - Lava Shock Spread
class spell_sha_lava_lash_spread: public SpellScriptLoader
{
    public:
        spell_sha_lava_lash_spread() : SpellScriptLoader("spell_sha_lava_lash_spread") { }

        class spell_sha_lava_lash_spread_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_lava_lash_spread_SpellScript);

            void HitTarget(SpellEffIndex)
            {
                if (Unit* target = GetHitUnit())
                    GetCaster()->AddAura(SPELL_SHA_FLAME_SHOCK, target);
            }

            void FilterTargets(std::list<WorldObject*>& unitList)
            {
                uint32 maxTargets = sSpellMgr->GetSpellInfo(SPELL_SHA_LAVA_LASH)->Effects[EFFECT_3].BasePoints;
                std::list<WorldObject*> finalList;

                for (std::list<WorldObject*>::const_iterator iter = unitList.begin(); iter != unitList.end(); iter++)
                    if (Unit* target = (*iter)->ToUnit())
                        if (finalList.size() < maxTargets)
                            if (!target->HasAura(SPELL_SHA_FLAME_SHOCK))
                                finalList.push_back(*iter);

                for (std::list<WorldObject*>::const_iterator iter = unitList.begin(); iter != unitList.end(); iter++)
                    if (Unit* target = (*iter)->ToUnit())
                        if (finalList.size() < maxTargets)
                            if (target->HasAura(SPELL_SHA_FLAME_SHOCK))
                                finalList.push_back(*iter);

                unitList = finalList;
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_sha_lava_lash_spread_SpellScript::HitTarget, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sha_lava_lash_spread_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_lava_lash_spread_SpellScript();
        }
};

/// Flame Shock - 8050
class spell_sha_flame_shock : public SpellScriptLoader
{
    public:
        spell_sha_flame_shock() : SpellScriptLoader("spell_sha_flame_shock") { }

        class spell_sha_flame_shock_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_flame_shock_SpellScript);

            bool m_HasUnleashFlame = false;

            void HitTarget(SpellEffIndex)
            {
                Unit* l_Caster = GetCaster();

                SpellInfo const* l_UnleashFlame = sSpellMgr->GetSpellInfo(SPELL_SHA_UNLEASH_FLAME_AURA);
                SpellInfo const* l_ElementalFusion = sSpellMgr->GetSpellInfo(SPELL_SHA_ELEMENTAL_FUSION_PROC);

                if (l_Caster->HasAura(SPELL_SHA_UNLEASH_FLAME_AURA) && l_UnleashFlame != nullptr)
                {
                    m_HasUnleashFlame = true;
                    SetHitDamage(GetHitDamage() + CalculatePct(GetHitDamage(), l_UnleashFlame->Effects[EFFECT_1].BasePoints));
                    l_Caster->RemoveAurasDueToSpell(SPELL_SHA_UNLEASH_FLAME_AURA);
                }
                if (l_Caster->HasAura(SPELL_SHA_ELEMENTAL_FUSION_PROC) && l_ElementalFusion != nullptr)
                {
                    SetHitDamage(GetHitDamage() + CalculatePct(GetHitDamage(), l_ElementalFusion->Effects[EFFECT_0].BasePoints));
                    l_Caster->RemoveAurasDueToSpell(SPELL_SHA_ELEMENTAL_FUSION_PROC);
                }
            }

            void HandleAfterHit()
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetHitUnit();

                SpellInfo const* l_UnleashFlame = sSpellMgr->GetSpellInfo(SPELL_SHA_UNLEASH_FLAME_AURA);

                if (l_Target == nullptr)
                    return;

                if (m_HasUnleashFlame && l_UnleashFlame != nullptr)
                {
                    if (AuraPtr l_Aura = l_Target->GetAura(GetSpellInfo()->Id))
                        l_Aura->GetEffect(EFFECT_1)->SetAmount(l_Aura->GetEffect(EFFECT_1)->GetAmount() + CalculatePct(l_Aura->GetEffect(EFFECT_1)->GetAmount(), l_UnleashFlame->Effects[EFFECT_1].BasePoints));

                }
            }

            void Register()
            {
                AfterHit += SpellHitFn(spell_sha_flame_shock_SpellScript::HandleAfterHit);
                OnEffectHitTarget += SpellEffectFn(spell_sha_flame_shock_SpellScript::HitTarget, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        class spell_sha_flame_shock_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sha_flame_shock_AuraScript);

            enum eSpells
            {
                GlyphOfFlameShock = 55447
            };

            void CalculateAmount(constAuraEffectPtr /*p_AurEff*/, int32& p_Amount, bool& /*p_CanBeRecalculated*/)
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster == nullptr)
                    return;

                SpellInfo const* l_ElementalFusion = sSpellMgr->GetSpellInfo(SPELL_SHA_ELEMENTAL_FUSION_PROC);

                if (l_Caster->HasAura(SPELL_SHA_ELEMENTAL_FUSION_PROC) && l_ElementalFusion != nullptr)
                {
                    p_Amount += CalculatePct(p_Amount, l_ElementalFusion->Effects[EFFECT_0].BasePoints);
                }
            }

            void OnTick(constAuraEffectPtr p_AurEff)
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster == nullptr)
                    return;

                SpellInfo const* l_GlyphOfFlameShock = sSpellMgr->GetSpellInfo(eSpells::GlyphOfFlameShock);

                if (l_Caster->HasAura(eSpells::GlyphOfFlameShock))
                {
                    int32 l_Healing = CalculatePct(p_AurEff->GetAmount(), l_GlyphOfFlameShock->Effects[EFFECT_0].BasePoints);
                    l_Caster->HealBySpell(l_Caster, GetSpellInfo(), l_Healing, false);
                }
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_sha_flame_shock_AuraScript::CalculateAmount, EFFECT_1, SPELL_AURA_PERIODIC_DAMAGE);
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_sha_flame_shock_AuraScript::OnTick, EFFECT_1, SPELL_AURA_PERIODIC_DAMAGE);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sha_flame_shock_AuraScript();
        }

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_flame_shock_SpellScript();
        }
};

/// Healing Wave - 77472
class spell_sha_healing_wave : public SpellScriptLoader
{
    public:
        spell_sha_healing_wave() : SpellScriptLoader("spell_sha_healing_wave") { }

        class spell_sha_healing_wave_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_healing_wave_SpellScript);

            enum eSpells
            {
                GlyphOfHealingWave = 55440,
                GlyphOfHealingWaveHeal = 55533
            };

            void HitTarget(SpellEffIndex)
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetHitUnit();
                
                if (l_Target == nullptr)
                    return;

                if (l_Caster->HasAura(eSpells::GlyphOfHealingWave)) ///< Glyph of Healing Wave
                {
                    SpellInfo const* l_GlyphOfHealingWave = sSpellMgr->GetSpellInfo(eSpells::GlyphOfHealingWave);

                    if (l_Caster->GetGUID() == l_Target->GetGUID() || l_GlyphOfHealingWave == nullptr) ///< When you heal someone else
                        return;

                    int32 l_HealAmount = (GetHitHeal() / l_GlyphOfHealingWave->Effects[EFFECT_0].BasePoints); ///< Your Healing Wave also heals you for 10 % of the healing effect
                    
                    l_Caster->CastCustomSpell(l_Caster, eSpells::GlyphOfHealingWaveHeal, &l_HealAmount, nullptr, nullptr, true);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_sha_healing_wave_SpellScript::HitTarget, EFFECT_0, SPELL_EFFECT_HEAL);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_healing_wave_SpellScript();
        }
};

/// Lava Lash - 60103
class spell_sha_lava_lash: public SpellScriptLoader
{
    public:
        spell_sha_lava_lash() : SpellScriptLoader("spell_sha_lava_lash") { }

        class spell_sha_lava_lash_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_lava_lash_SpellScript);

            void HitTarget(SpellEffIndex)
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetHitUnit();

                if (l_Caster == nullptr || l_Target == nullptr)
                    return;

                if (l_Target->HasAura(SPELL_SHA_FLAME_SHOCK))
                    l_Caster->CastSpell(l_Target, SPELL_SHA_LAVA_LASH_SPREAD, true);

                if (l_Caster->HasAura(SPELL_SHA_ELEMENTAL_FUSION))
                    l_Caster->CastSpell(l_Caster, SPELL_SHA_ELEMENTAL_FUSION_PROC, true);
                
                if (l_Caster->HasAura(SPELL_SHA_UNLEASH_FLAME_AURA))
                {
                    SpellInfo const* l_UnleashFlame = sSpellMgr->GetSpellInfo(SPELL_SHA_UNLEASH_FLAME_AURA);

                    if (l_UnleashFlame == nullptr)
                        return;

                    SetHitDamage(GetHitDamage() + CalculatePct(GetHitDamage(), l_UnleashFlame->Effects[EFFECT_1].BasePoints));
                    l_Caster->RemoveAurasDueToSpell(SPELL_SHA_UNLEASH_FLAME_AURA);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_sha_lava_lash_SpellScript::HitTarget, EFFECT_0, SPELL_EFFECT_WEAPON_PERCENT_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_lava_lash_SpellScript();
        }
};

/// 33757 - Windfury
class spell_sha_windfury: public SpellScriptLoader
{
    public:
        spell_sha_windfury() : SpellScriptLoader("spell_sha_windfury") { }

        class spell_sha_windfury_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sha_windfury_AuraScript);

            void OnProc(constAuraEffectPtr aurEff, ProcEventInfo& eventInfo)
            {
                Player* caster = GetCaster()->ToPlayer();
                PreventDefaultAction();

                if (!caster->HasSpellCooldown(GetSpellInfo()->Id))
                {
                    if (Unit* victim = eventInfo.GetActionTarget())
                    {
                        if (!victim->IsFriendlyTo(caster))
                        {
                            caster->AddSpellCooldown(GetSpellInfo()->Id, 0, 5 * IN_MILLISECONDS);

                            int count = 3; // Blame blizz

                            if (AuraPtr bonus = GetCaster()->GetAura(SPELL_SHA_PVP_BONUS_WOD_4))
                                count += bonus->GetEffect(EFFECT_0)->GetAmount();

                            for (int i = 0; i < count; i++)
                                caster->CastSpell(victim, SPELL_SHA_WINDFURY_ATTACK, true);
                        }
                    }
                }
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_sha_windfury_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sha_windfury_AuraScript();
        }
};

enum GlyphOfEternalEarth
{
    LightningBolt   = 403,
    EarthShield     = 974
};

/// Glyph of Eternal Earth (Restoration) - 147781
class spell_sha_glyph_of_eternal_earth : public SpellScriptLoader
{
    public:
        spell_sha_glyph_of_eternal_earth() : SpellScriptLoader("spell_sha_glyph_of_eternal_earth") { }

        class spell_sha_glyph_of_eternal_earth_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sha_glyph_of_eternal_earth_AuraScript);

            void OnProc(constAuraEffectPtr /*p_AurEff*/, ProcEventInfo& p_ProcInfo)
            {
                PreventDefaultAction();

                if (!p_ProcInfo.GetDamageInfo())
                    return;

                if (!p_ProcInfo.GetDamageInfo()->GetSpellInfo() || p_ProcInfo.GetDamageInfo()->GetSpellInfo()->Id != GlyphOfEternalEarth::LightningBolt)
                    return;

                if (Unit* l_Caster = p_ProcInfo.GetActor())
                {
                    if (AuraPtr l_EarthShield = l_Caster->GetAura(GlyphOfEternalEarth::EarthShield))
                    {
                        if (l_EarthShield->GetCharges() >= GetSpellInfo()->Effects[EFFECT_0].BasePoints) ///< This cannot cause Earth Shield to exceed 9 charges.
                            return;
                        else
                            l_EarthShield->ModCharges(1);
                    }
                }
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_sha_glyph_of_eternal_earth_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sha_glyph_of_eternal_earth_AuraScript();
        }
};

/// 10400 - Flametongue
class spell_sha_flametongue: public SpellScriptLoader
{
    public:
        spell_sha_flametongue() : SpellScriptLoader("spell_sha_flametongue") { }

        class spell_sha_flametongue_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sha_flametongue_AuraScript);

            void OnProc(constAuraEffectPtr aurEff, ProcEventInfo& eventInfo)
            {
                PreventDefaultAction();

                Unit* target = eventInfo.GetProcTarget();
                SpellInfo const* spellProto = GetSpellInfo();

                if (eventInfo.GetDamageInfo()->GetAttackType() == WeaponAttackType::OffAttack || spellProto)
                    GetCaster()->CastSpell(target, SPELL_SHA_LAMETONGUE_ATTACK, true);
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_sha_flametongue_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sha_flametongue_AuraScript();
        }
};

/// 157804 - Improved Flame Shock
class spell_sha_improoved_flame_shock: public SpellScriptLoader
{
    public:
        spell_sha_improoved_flame_shock() : SpellScriptLoader("spell_sha_improoved_flame_shock") { }

        class spell_sha_improoved_flame_shock_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sha_improoved_flame_shock_AuraScript);

            void OnProc(constAuraEffectPtr aurEff, ProcEventInfo& eventInfo)
            {
                PreventDefaultAction();

                if (Unit* l_Caster = GetCaster())
                    if (Player* l_Player = l_Caster->ToPlayer())
                        l_Player->RemoveSpellCooldown(SPELL_SHA_LAVA_LASH, true);
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_sha_improoved_flame_shock_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sha_improoved_flame_shock_AuraScript();
        }
};

/// 51533 - Feral Spirit
class spell_sha_feral_spirit: public SpellScriptLoader
{
    public:
        spell_sha_feral_spirit() : SpellScriptLoader("spell_sha_feral_spirit") { }

        class spell_sha_feral_spirit_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_feral_spirit_SpellScript);

            void OnLaunch(SpellEffIndex effIndex)
            {
                // Broken spellproc
                if (Unit* caster = GetCaster())
                    if (AuraPtr aura = caster->GetAura(SPELL_SHA_PVP_BONUS_WOD_2))
                        caster->CastSpell(caster, aura->GetSpellInfo()->Effects[0].TriggerSpell);
            }

            void Register()
            {
                OnEffectLaunch += SpellEffectFn(spell_sha_feral_spirit_SpellScript::OnLaunch, EFFECT_0, SPELL_EFFECT_SUMMON);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_feral_spirit_SpellScript();
        }
};

/// Spirit Hunt - 58877
class spell_sha_pet_spirit_hunt: public SpellScriptLoader
{
    public:
        spell_sha_pet_spirit_hunt() : SpellScriptLoader("spell_sha_pet_spirit_hunt") { }

        enum eSpells
        {
            SpiritHuntHeal = 58879
        };

        class spell_sha_pet_spirit_hunt_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sha_pet_spirit_hunt_AuraScript);

            void OnProc(constAuraEffectPtr p_AurEff, ProcEventInfo& p_EventInfo)
            {
                PreventDefaultAction();

                Unit* l_Caster = GetCaster();
                if (!l_Caster)
                    return;

                Unit* l_Owner = l_Caster->GetOwner();
                if (!l_Owner)
                    return;

                int32 l_TakenDamage = p_EventInfo.GetDamageInfo()->GetDamage();
                if (!l_TakenDamage)
                    return;

                int32 l_HealAmount = CalculatePct(l_TakenDamage, p_AurEff->GetAmount());
                l_Caster->CastCustomSpell(l_Owner, eSpells::SpiritHuntHeal, &l_HealAmount, nullptr, nullptr, true);
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_sha_pet_spirit_hunt_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sha_pet_spirit_hunt_AuraScript();
        }
};

/// 88766 - Fulmination
class spell_sha_fulmination_proc: public SpellScriptLoader
{
    public:
        spell_sha_fulmination_proc() : SpellScriptLoader("spell_sha_fulmination_proc") { }

        class spell_sha_fulmination_proc_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sha_fulmination_proc_AuraScript);


            void OnProc(constAuraEffectPtr aurEff, ProcEventInfo& eventInfo)
            {
                PreventDefaultAction();

                Unit* target = eventInfo.GetProcTarget();

                if (!target)
                    return;

                uint32 maxCharges = GetEffect(EFFECT_0)->CalculateAmount(GetCaster());
                if (AuraPtr aura = GetCaster()->GetAura(SPELL_SHA_LIGHTNING_SHIELD))
                {
                    if (aura->GetCharges() < maxCharges)
                        aura->SetCharges(aura->GetCharges() + 1);

                    if (aura->GetCharges() == maxCharges && !GetCaster()->HasAura(SPELL_SHA_FULMINATION_INFO))
                        GetCaster()->CastSpell(GetCaster(), SPELL_SHA_FULMINATION_INFO, true);
                }

            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_sha_fulmination_proc_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sha_fulmination_proc_AuraScript();
        }
};

/// 157765 Enhanced Chain Lightning
class spell_sha_enhanced_chain_lightning: public SpellScriptLoader
{
    public:
        spell_sha_enhanced_chain_lightning() : SpellScriptLoader("spell_sha_enhanced_chain_lightning") { }

        class spell_sha_enhanced_chain_lightning_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sha_enhanced_chain_lightning_AuraScript);


            void OnProc(constAuraEffectPtr aurEff, ProcEventInfo& eventInfo)
            {
                PreventDefaultAction();
                GetCaster()->CastSpell(GetCaster(), SPELL_SHA_IMPROVED_CHAIN_LIGHTNING, true);
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_sha_enhanced_chain_lightning_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sha_enhanced_chain_lightning_AuraScript();
        }
};

/// Echo of Elements - 108283
class spell_sha_echo_of_elements: public SpellScriptLoader
{
    public:
        spell_sha_echo_of_elements() : SpellScriptLoader("spell_sha_echo_of_elements") { }

        class spell_sha_echo_of_elements_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sha_echo_of_elements_AuraScript);

            void OnProc(constAuraEffectPtr /*aurEff*/, ProcEventInfo& /*eventInfo*/)
            {
                PreventDefaultAction();

                Player* l_Player = GetCaster()->ToPlayer();
                if (!l_Player)
                    return;

                switch (l_Player->GetSpecializationId(l_Player->GetActiveSpec()))
                {
                    case SPEC_SHAMAN_ELEMENTAL:
                        l_Player->CastSpell(l_Player, SPELL_SHA_ECHO_OF_THE_ELEMENTS_ELEMENTAL, true);
                        break;
                    case SPEC_SHAMAN_ENHANCEMENT:
                        l_Player->CastSpell(l_Player, SPELL_SHA_ECHO_OF_THE_ELEMENTS_ENHANCEMENT, true);
                        break;
                    case SPEC_SHAMAN_RESTORATION:
                        l_Player->CastSpell(l_Player, SPELL_SHA_ECHO_OF_THE_ELEMENTS_RESTORATION, true);
                        break;
                    default:
                        break;
                }
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_sha_echo_of_elements_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sha_echo_of_elements_AuraScript();
        }
};

/// 152255 Liquid Magma - a bit hacky, will do it properly later
class spell_sha_liquid_magma: public SpellScriptLoader
{
    public:
        spell_sha_liquid_magma() : SpellScriptLoader("spell_sha_liquid_magma") { }

        class spell_sha_liquid_magma_AuraScript: public AuraScript
        {
            PrepareAuraScript(spell_sha_liquid_magma_AuraScript);

            void OnTick(constAuraEffectPtr p_AurEff)
            {
                Unit* l_Caster = GetCaster();
                // hardcoded in the tooltip - no DBC data here
                Unit* l_Target = l_Caster->SelectNearbyTarget(nullptr, 40, 0, false);

                if (l_Target)
                    l_Caster->CastSpell(l_Target, GetSpellInfo()->Effects[p_AurEff->GetEffIndex()].TriggerSpell);
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_sha_liquid_magma_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }

        };

        class spell_sha_liquid_magma_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_liquid_magma_SpellScript);

            SpellCastResult HandleCheckCast()
            {
                if (Player* l_Player = GetCaster()->ToPlayer())
                    if (Creature* l_Totem = l_Player->GetMap()->GetCreature(l_Player->m_SummonSlot[SUMMON_SLOT_TOTEM]))
                        if (l_Totem && l_Totem->isTotem())
                            return SPELL_CAST_OK;

                if (GetCaster()->isTotem())
                    return SPELL_CAST_OK;

                SetCustomCastResultMessage(SPELL_CUSTOM_ERROR_MUST_HAVE_FIRE_TOTEM);
                return SPELL_FAILED_CUSTOM_ERROR;
            }

            void HandleApply(SpellEffIndex /*effIndex*/)
            {
                if (Player* l_Player = GetHitUnit()->ToPlayer())
                {
                    if (Creature* l_Totem = l_Player->GetMap()->GetCreature(l_Player->m_SummonSlot[SUMMON_SLOT_TOTEM]))
                    {
                        if (l_Totem && l_Totem->isTotem())
                        {
                            l_Totem->AddAura(GetSpellInfo()->Id, l_Totem);
                            PreventHitAura();
                        }
                    }
                }
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_sha_liquid_magma_SpellScript::HandleCheckCast);
                OnEffectHitTarget += SpellEffectFn(spell_sha_liquid_magma_SpellScript::HandleApply, EFFECT_0, SPELL_EFFECT_APPLY_AURA);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_liquid_magma_SpellScript();
        }

        AuraScript* GetAuraScript() const
        {
            return new spell_sha_liquid_magma_AuraScript();
        }
};

/// 157501 Liquid Magma
class spell_sha_liquid_magma_visual: public SpellScriptLoader
{
    public:
        spell_sha_liquid_magma_visual() : SpellScriptLoader("spell_sha_liquid_magma_visual") { }

        class spell_sha_liquid_magma_visual_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_liquid_magma_visual_SpellScript);

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                GetCaster()->CastSpell(GetHitUnit(), SPELL_SHA_LIQUID_MAGMA_DAMAGE, true);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_sha_liquid_magma_visual_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_liquid_magma_visual_SpellScript();
        }
};

/// 2645 Ghost Wolf
class spell_sha_ghost_wolf: public SpellScriptLoader
{
    public:
        spell_sha_ghost_wolf() : SpellScriptLoader("spell_sha_ghost_wolf") { }

        class spell_sha_ghost_wolf_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sha_ghost_wolf_AuraScript);

            void CalculateAmount(constAuraEffectPtr p_AurEff, int32& p_Amount, bool& p_CanBeRecalculated)
            {
                SpellInfo const* l_GhostlySpeed = sSpellMgr->GetSpellInfo(SPELL_SHA_GLYPH_OF_GHOSTLY_SPEED);
                if (! l_GhostlySpeed)
                    return;

                p_CanBeRecalculated = true;
                if (Player* l_Owner = GetOwner()->ToPlayer())
                {
                    if (l_Owner->HasGlyph(l_GhostlySpeed->Id))
                    {
                        if (!l_Owner->isInCombat())
                        {
                            if (Map* l_Map = l_Owner->GetMap())
                            {
                                if (!l_Map->IsBattlegroundOrArena())
                                {
                                    p_Amount += l_GhostlySpeed->Effects[EFFECT_0].BasePoints;
                                }
                            }
                        }
                    }
                }
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_sha_ghost_wolf_AuraScript::CalculateAmount, EFFECT_1, SPELL_AURA_MOD_SPEED_ALWAYS);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sha_ghost_wolf_AuraScript();
        }
};

/// last update : 6.1.2 19802
/// 51505 - Lava Burst
class spell_sha_lava_burst: public SpellScriptLoader
{
    public:
        spell_sha_lava_burst() : SpellScriptLoader("spell_sha_lava_burst") { }

        class spell_sha_lava_burst_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_lava_burst_SpellScript);

            void HitTarget(SpellEffIndex)
            {
                Player* l_Player = GetCaster()->ToPlayer();
                Unit* l_Target = GetHitUnit();

                if (l_Target == nullptr || l_Player == nullptr)
                    return;

                if (l_Player->HasAura(SPELL_SHA_ELEMENTAL_FUSION))
                    l_Player->CastSpell(l_Player, SPELL_SHA_ELEMENTAL_FUSION_PROC, true);
                if (l_Player->HasSpell(SPELL_SHA_IMPROVED_LIGHTNING_SHIELD) && l_Player->HasSpell(SPELL_SHA_FULMINATION))
                {
                    AuraPtr l_LightningShield = l_Player->GetAura(SPELL_SHA_LIGHTNING_SHIELD_AURA);
                    if (l_LightningShield != nullptr)
                    {
                        if (l_LightningShield->GetCharges() < 20)
                            l_LightningShield->SetCharges(l_LightningShield->GetCharges() + 1);
                    }
                }

                /// Lavaburst deals 50% more damage with Flame Shock on target
                /// HotFixe February 27, 2015 : Lava burst no longer deals extra damage in PvP combat for Restoration Shaman.
                if (l_Target->HasAura(SPELL_SHA_FLAME_SHOCK) && !(l_Player->GetSpecializationId(l_Player->GetActiveSpec()) == SPEC_SHAMAN_RESTORATION && l_Target->GetTypeId() == TYPEID_PLAYER))
                    SetHitDamage(int32(float(GetHitDamage()) * 1.5f));
            }

            void HandleAfterCast()
            {
                Player* l_Player = GetCaster()->ToPlayer();

                if (l_Player == nullptr)
                    return;

                if (l_Player->HasAura(SPELL_SHA_LAVA_SURGE_CAST_TIME))
                    l_Player->RestoreCharge(1536);
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_sha_lava_burst_SpellScript::HandleAfterCast);
                OnEffectHitTarget += SpellEffectFn(spell_sha_lava_burst_SpellScript::HitTarget, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_lava_burst_SpellScript();
        }
};

/// Call by Chain Heal - 1064, Riptide - 61295
/// Tidal Waves - 51564
class spell_sha_tidal_waves : public SpellScriptLoader
{
public:
    spell_sha_tidal_waves() : SpellScriptLoader("spell_sha_tidal_waves") { }

    class spell_sha_tidal_waves_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_sha_tidal_waves_SpellScript);

        void HandleAfterCast()
        {
            if (Unit* l_Caster = GetCaster())
            {
                if (l_Caster->HasAura(SPELL_SHA_TIDAL_WAVES))
                    l_Caster->CastSpell(l_Caster, SPELL_SHA_TIDAL_WAVES_PROC, true);
            }
        }

        void Register()
        {
            AfterCast += SpellCastFn(spell_sha_tidal_waves_SpellScript::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_sha_tidal_waves_SpellScript();
    }
};

/// 2645 Chain Heal
class spell_sha_chain_heal: public SpellScriptLoader
{
    public:
        spell_sha_chain_heal() : SpellScriptLoader("spell_sha_chain_heal") { }

        class spell_sha_chain_heal_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_chain_heal_SpellScript);

            enum eSpells
            {
                Riptide = 61295
            };

            void HandleHeal(SpellEffIndex /*effIndex*/)
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetHitUnit();
                if (!l_Target)
                    return;

                if (l_Target->HasAura(eSpells::Riptide))
                {
                    uint32 l_Heal = GetHitHeal();

                    SpellInfo const* l_Riptide = sSpellMgr->GetSpellInfo(eSpells::Riptide);
                    if (l_Riptide == nullptr)
                        return;

                    AddPct(l_Heal, l_Riptide->Effects[EFFECT_2].BasePoints);
                    SetHitHeal(l_Heal);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_sha_chain_heal_SpellScript::HandleHeal, EFFECT_0, SPELL_EFFECT_HEAL);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_chain_heal_SpellScript;
        }
};

/// Riptide - 61295
class spell_sha_riptide : public SpellScriptLoader
{
    public:
        spell_sha_riptide() : SpellScriptLoader("spell_sha_riptide") { }

        enum eSpells
        {
            UnleashLife = 73685
        };

        class spell_sha_riptide_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_riptide_SpellScript);

            void HandleHeal(SpellEffIndex /*effIndex*/)
            {
                Unit* l_Caster = GetCaster();
                SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(eSpells::UnleashLife);

                if (l_SpellInfo == nullptr)
                    return;

                if (l_Caster->HasAura(eSpells::UnleashLife))
                    SetHitHeal(GetHitHeal() + CalculatePct(GetHitHeal(), l_SpellInfo->Effects[EFFECT_2].BasePoints));
            }

            void HandleAfterHit()
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster->HasAura(eSpells::UnleashLife))
                    l_Caster->RemoveAurasDueToSpell(eSpells::UnleashLife);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_sha_riptide_SpellScript::HandleHeal, EFFECT_0, SPELL_EFFECT_HEAL);
                AfterHit += SpellHitFn(spell_sha_riptide_SpellScript::HandleAfterHit);
            }
        };

        class spell_sha_riptide_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sha_riptide_AuraScript);

            void CalculateAmount(constAuraEffectPtr /*auraEffect*/, int32& p_Amount, bool& /*canBeRecalculated*/)
            {
                Unit* l_Caster = GetCaster();
                SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(eSpells::UnleashLife);

                if (l_Caster == nullptr || l_SpellInfo == nullptr)
                    return;

                if (l_Caster->HasAura(eSpells::UnleashLife))
                {
                    p_Amount += CalculatePct(p_Amount, l_SpellInfo->Effects[EFFECT_2].BasePoints);
                }
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_sha_riptide_AuraScript::CalculateAmount, EFFECT_1, SPELL_AURA_PERIODIC_HEAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sha_riptide_AuraScript();
        }

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_riptide_SpellScript;
        }
};

// Maelstrom Weapon - 53817
uint32 g_MaelstromVisualSpellIds[] { 170588, 170587, 170586, 170585, 60349};
class spell_sha_maelstrom_weapon: public SpellScriptLoader
{
    public:
        spell_sha_maelstrom_weapon() : SpellScriptLoader("spell_sha_maelstrom_weapon") { }

        class spell_sha_maelstrom_weapon_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sha_maelstrom_weapon_AuraScript);

            void OnApply(constAuraEffectPtr aurEff, AuraEffectHandleModes /*mode*/)
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster == nullptr)
                    return;

                RemoveAllVisuals(l_Caster);
                l_Caster->AddAura(g_MaelstromVisualSpellIds[aurEff->GetBase()->GetStackAmount() - 1], l_Caster);
            }
            
            void OnRemove(constAuraEffectPtr aurEff, AuraEffectHandleModes /*mode*/)
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster == nullptr)
                    return;

                RemoveAllVisuals(l_Caster);
            }

            void RemoveAllVisuals(Unit* l_Caster)
            {
                for (uint32 l_I = 0; l_I < sizeof(g_MaelstromVisualSpellIds) / sizeof(uint32); l_I++)
                    if (AuraPtr l_Aura = l_Caster->GetAura(g_MaelstromVisualSpellIds[l_I]))
                        l_Aura->Remove();
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_sha_maelstrom_weapon_AuraScript::OnApply, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
                OnEffectRemove += AuraEffectRemoveFn(spell_sha_maelstrom_weapon_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sha_maelstrom_weapon_AuraScript();
        }
};

enum CloudburstTotemSpells
{
    SPELL_CLOUDBURST    = 157503,
};

/// Last updated : 6.1.2 19802
/// Cloudburst Totem - 157504
class spell_sha_cloudburst_totem: public SpellScriptLoader
{
    public:
        spell_sha_cloudburst_totem() : SpellScriptLoader("spell_sha_cloudburst_totem") { }

        class spell_sha_cloudburst_totem_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sha_cloudburst_totem_AuraScript);

            void OnProc(constAuraEffectPtr p_AurEff, ProcEventInfo& p_EventInfo)
            {
                PreventDefaultAction();

                HealInfo* l_HealInfo = p_EventInfo.GetHealInfo();

                if (!l_HealInfo)
                    return;

                if (SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(SPELL_CLOUDBURST))
                    GetEffect(p_AurEff->GetEffIndex())->SetAmount(p_AurEff->GetAmount() + CalculatePct(l_HealInfo->GetHeal(), l_SpellInfo->Effects[EFFECT_0].BasePoints));
            }

            void OnRemove(constAuraEffectPtr p_AurEff, AuraEffectHandleModes /* p_Mode */)
            {
                if (Unit* l_Owner = GetOwner()->ToUnit())
                {
                    if (int32 l_Amount = p_AurEff->GetAmount())
                    {
                        l_Owner->CastCustomSpell(l_Owner, SPELL_CLOUDBURST, &l_Amount, nullptr, nullptr, true);
                        GetEffect(p_AurEff->GetEffIndex())->SetAmount(0);
                    }
                }
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_sha_cloudburst_totem_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
                OnEffectRemove += AuraEffectRemoveFn(spell_sha_cloudburst_totem_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sha_cloudburst_totem_AuraScript();
        }
};

/// Last updated : 6.1.2 19802
/// Clodburst - 157503
class spell_sha_cloudburst: public SpellScriptLoader
{
    public:
        spell_sha_cloudburst() : SpellScriptLoader("spell_sha_cloudburst") { }

        class spell_sha_cloudburst_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sha_cloudburst_SpellScript);

            bool Load()
            {
                l_TargetCount = 0;
                return true;
            }

            void HandleHeal(SpellEffIndex p_EffIndex)
            {
                if (l_TargetCount)
                    SetHitHeal(GetHitHeal() / l_TargetCount);
            }

            void CountTargets(std::list<WorldObject*>& p_Targets)
            {
                for (auto l_Target : p_Targets)
                    ++l_TargetCount;
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sha_cloudburst_SpellScript::CountTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ALLY);
                OnEffectHitTarget += SpellEffectFn(spell_sha_cloudburst_SpellScript::HandleHeal, EFFECT_0, SPELL_EFFECT_HEAL);
            }

            uint8 l_TargetCount;
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sha_cloudburst_SpellScript;
        }
};

void AddSC_shaman_spell_scripts()
{
    new spell_sha_unleashed_fury();
    new spell_sha_high_tide();
    new spell_sha_tidal_waves();
    new spell_sha_totemic_projection();
    new spell_sha_water_ascendant();
    new spell_sha_glyph_of_shamanistic_rage();
    new spell_sha_glyph_of_lakestrider();
    new spell_sha_call_of_the_elements();
    new spell_sha_conductivity();
    new spell_sha_ancestral_guidance();
    new spell_sha_earthgrab();
    new spell_sha_stone_bulwark();
    new spell_sha_frozen_power();
    new spell_sha_spirit_link();
    new spell_sha_fire_nova();
    new spell_sha_fulmination();
    new spell_sha_lava_surge();
    new spell_sha_healing_stream();
    new spell_sha_elemental_blast();
    new spell_sha_earthquake_tick();
    new spell_sha_earthquake();
    new spell_sha_healing_rain();
    new spell_sha_healing_rain_heal();
    new spell_sha_ascendance();
    new spell_sha_bloodlust();
    new spell_sha_heroism();
    new spell_sha_spirit_hunt();
    new spell_sha_lava_lash_spread();
    new spell_sha_windfury();
    new spell_sha_flametongue();
    new spell_sha_improoved_flame_shock();
    new spell_sha_feral_spirit();
    new spell_sha_pet_spirit_hunt();
    new spell_sha_fulmination_proc();
    new spell_sha_enhanced_chain_lightning();
    new spell_sha_echo_of_elements();
    new spell_sha_lava_lash();
    new spell_sha_liquid_magma();
    new spell_sha_liquid_magma_visual();
    new spell_sha_ghost_wolf();
    new spell_sha_lava_burst();
    new spell_sha_chain_heal();
    new spell_sha_glyph_of_eternal_earth();
    new spell_sha_flame_shock();
    new spell_sha_purge();
    new spell_sha_healing_wave();
    new spell_sha_riptide();
    new spell_sha_maelstrom_weapon();
    new spell_sha_cloudburst_totem();
    new spell_sha_cloudburst();
}
