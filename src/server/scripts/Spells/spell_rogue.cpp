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
 * Scripts for spells with SPELLFAMILY_ROGUE and SPELLFAMILY_GENERIC spells used by rogue players.
 * Ordered alphabetically using scriptname.
 * Scriptnames of files in this file should be prefixed with "spell_rog_".
 */

#include "ScriptMgr.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "GridNotifiers.h"

enum RogueSpells
{
    ROGUE_SPELL_DEADLY_POISON                   = 2823,
    ROGUE_SPELL_CRIPPLING_POISON                = 3408,
    ROGUE_SPELL_CRIPPLING_POISON_DEBUFF         = 3409,
    ROGUE_SPELL_LEECHING_POISON                 = 108211,
    ROGUE_SPELL_LEECHING_POISON_DEBUFF          = 112961,
    ROGUE_SPELL_DEBILITATING_POISON             = 115196,
    ROGUE_SPELL_LEECH_VITALITY                  = 116921,
    ROGUE_SPELL_DEADLY_POISON_DOT               = 2818,
    ROGUE_SPELL_DEADLY_POISON_INSTANT_DAMAGE    = 113780,
    ROGUE_SPELL_CRIMSON_TEMPEST_DOT             = 122233,
    ROGUE_SPELL_SHROUD_OF_CONCEALMENT_AURA      = 115834,
    ROGUE_SPELL_GARROTE_DOT                     = 703,
    ROGUE_SPELL_RUPTURE_DOT                     = 1943,
    ROGUE_SPELL_CUT_TO_THE_CHASE_AURA           = 51667,
    ROGUE_SPELL_ADRENALINE_RUSH                 = 13750,
    ROGUE_SPELL_KILLING_SPREE                   = 51690,
    ROGUE_SPELL_SPRINT                          = 2983,
    ROGUE_SPELL_HEMORRHAGE                      = 16511,
    ROGUE_SPELL_SANGUINARY_VEIN_DEBUFF          = 124271,
    ROGUE_SPELL_NIGHTSTALKER_AURA               = 14062,
    ROGUE_SPELL_NIGHTSTALKER_DAMAGE_DONE        = 130493,
    ROGUE_SPELL_SHADOW_FOCUS_AURA               = 108209,
    ROGUE_SPELL_SHADOW_FOCUS_COST_PCT           = 112942,
    ROGUE_SPELL_NERVE_STRIKE_AURA               = 108210,
    ROGUE_SPELL_NERVE_STRIKE_REDUCE_DAMAGE_DONE = 112947,
    ROGUE_SPELL_COMBAT_READINESS                = 74001,
    ROGUE_SPELL_COMBAT_INSIGHT                  = 74002,
    ROGUE_SPELL_BLADE_FLURRY_AURA               = 13877,
    ROGUE_SPELL_BLADE_FLURRY_DAMAGE             = 22482,
    ROGUE_SPELL_CHEAT_DEATH_REDUCE_DAMAGE       = 45182,
    ROGUE_SPELL_CHEATED_DEATH_MARKER            = 45181,
    ROGUE_SPELL_DEADLY_BREW                     = 51626,
    ROGUE_SPELL_GLYPH_OF_HEMORRHAGE             = 56807,
    ROGUE_SPELL_CLOAK_AND_DAGGER                = 138106,
    ROGUE_SPELL_SHADOWSTEP_TELEPORT_ONLY        = 36563,
    ROGUE_SPELL_MARKED_FOR_DEATH                = 137619,
    ROGUE_SPELL_SHURIKEN_TOSS_CHANGE_MELEE      = 137586,
    ROGUE_SPELL_GLYPH_OF_DECOY                  = 56800,
    ROGUE_SPELL_DECOY_SUMMON                    = 89765,
    ROGUE_SPELL_KILLING_SPREE_TELEPORT          = 57840,
    ROGUE_SPELL_KILLING_SPREE_DAMAGES           = 57841,
    ROGUE_SPELL_GLYPH_OF_HEMORRHAGING_VEINS     = 146631,
    ROGUE_SPELL_GLYPH_OF_RECUPERATE             = 56806,
    ROGUE_SPELL_BURST_OF_SPEED                  = 137573,
    ROGUE_SPELL_INTERNAL_BLEEDING               = 154953,
    ROGUE_SPELL_INTERNAL_BLEEDING_AURA          = 154904,
    ROGUE_SPELL_COMBO_POINT_DELAYED             = 139569,
    ROGUE_SPELL_RUTHLESSNESS                    = 14161,
    ROGUE_SPELL_DEADLY_THROW_INTERRUPT          = 137576,
    ROGUE_SPELL_WOD_PVP_SUBTLETY_4P             = 170877,
    ROGUE_SPELL_WOD_PVP_SUBTLETY_4P_EFFECT      = 170879,
    ROGUE_SPELL_FIND_WEAKNESS                   = 91023,
    ROGUE_SPELL_FIND_WEAKNESS_PROC              = 91021
};

/// Anticipation - 114015
class spell_rog_anticipation : public SpellScriptLoader
{
    public:
        spell_rog_anticipation() : SpellScriptLoader("spell_rog_anticipation") { }

        class spell_rog_anticipation_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_rog_anticipation_AuraScript);

            enum eSpells
            {
                AnticipationProc        = 115189,
                SinisterStrike          = 1752,
                SinisterStrikeEnabler   = 79327,
                MutilateMainHand        = 5374,
                MutilateOffHand         = 27576
            };

            void OnProc(constAuraEffectPtr p_AurEff, ProcEventInfo& p_EventInfo)
            {
                PreventDefaultAction();

                SpellInfo const* l_SpellInfo = p_EventInfo.GetDamageInfo()->GetSpellInfo();
                if (l_SpellInfo == nullptr)
                    return;

                if (Unit* l_Caster = GetCaster())
                {
                    if (!l_SpellInfo->HasEffect(SPELL_EFFECT_ADD_COMBO_POINTS))
                        return;

                    int32 l_NewCombo = l_Caster->GetPower(Powers::POWER_COMBO_POINT);

                    for (uint8 i = 0; i < l_SpellInfo->EffectCount; ++i)
                    {
                        if (l_SpellInfo->Effects[i].IsEffect(SPELL_EFFECT_ADD_COMBO_POINTS))
                        {
                            l_NewCombo += l_SpellInfo->Effects[i].BasePoints;
                            break;
                        }
                    }

                    if (l_SpellInfo->Id == eSpells::SinisterStrike && l_Caster->HasAura(eSpells::SinisterStrikeEnabler))
                        l_NewCombo += 2;

                    if (l_SpellInfo->Id == eSpells::MutilateMainHand || l_SpellInfo->Id == eSpells::MutilateOffHand)
                        l_NewCombo += 1;

                    if (l_NewCombo <= 5)
                        return;

                    uint8 l_CastCount = l_NewCombo - 5;
                    /// Need to add one additional charge if it's critical hit
                    if (p_EventInfo.GetHitMask() & PROC_EX_CRITICAL_HIT)
                        ++l_CastCount;

                    for (uint8 l_I = 0; l_I < l_CastCount; ++l_I)
                        l_Caster->CastSpell(l_Caster, eSpells::AnticipationProc, true);
                }
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_rog_anticipation_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_rog_anticipation_AuraScript();
        }
};

/// Called by Deadly Poison - 2818, Crippling Poison - 3409, Wound Poison - 8680 and Leeching Poison - 112961
/// Venom Rush - 152152
class spell_rog_venom_rush : public SpellScriptLoader
{
    public:
        spell_rog_venom_rush() : SpellScriptLoader("spell_rog_venom_rush") { }

        enum eSpells
        {
            WoundPoison     = 8680,
            VenomRushAura   = 152152,
            VenomRushProc   = 156719
        };

        class spell_rog_venom_rush_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_rog_venom_rush_AuraScript);

            void OnApply(constAuraEffectPtr, AuraEffectHandleModes)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (!l_Caster->HasAura(eSpells::VenomRushAura))
                    {
                        if (l_Caster->HasAura(eSpells::VenomRushProc))
                            l_Caster->RemoveAura(eSpells::VenomRushProc);
                    }

                    if (Unit* l_Target = GetTarget())
                    {
                        bool l_MustCast = !l_Caster->HasPoisonTarget(l_Target->GetGUIDLow()) && l_Caster->HasAura(eSpells::VenomRushAura);
                        if (l_Caster->AddPoisonTarget(GetSpellInfo()->Id, l_Target->GetGUIDLow()) && l_MustCast)
                            l_Caster->CastSpell(l_Caster, eSpells::VenomRushProc, true);
                    }
                }
            }

            void OnRemove(constAuraEffectPtr, AuraEffectHandleModes)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (!l_Caster->HasAura(eSpells::VenomRushAura))
                    {
                        if (l_Caster->HasAura(eSpells::VenomRushProc))
                            l_Caster->RemoveAura(eSpells::VenomRushProc);
                    }

                    if (Unit* l_Target = GetTarget())
                    {
                        l_Caster->RemovePoisonTarget(l_Target->GetGUIDLow(), GetSpellInfo()->Id);
                        if (l_Caster->HasPoisonTarget(l_Target->GetGUIDLow()) || !l_Caster->HasAura(eSpells::VenomRushAura))
                            return;

                        if (AuraPtr l_Aura = l_Caster->GetAura(eSpells::VenomRushProc))
                            l_Aura->DropStack();
                    }
                }
            }

            void Register()
            {
                switch (m_scriptSpellId)
                {
                    case ROGUE_SPELL_DEADLY_POISON_DOT:
                        OnEffectApply += AuraEffectApplyFn(spell_rog_venom_rush_AuraScript::OnApply, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
                        OnEffectRemove += AuraEffectRemoveFn(spell_rog_venom_rush_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
                        break;
                    case ROGUE_SPELL_CRIPPLING_POISON_DEBUFF:
                        OnEffectApply += AuraEffectApplyFn(spell_rog_venom_rush_AuraScript::OnApply, EFFECT_0, SPELL_AURA_MOD_DECREASE_SPEED, AURA_EFFECT_HANDLE_REAL);
                        OnEffectRemove += AuraEffectRemoveFn(spell_rog_venom_rush_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_MOD_DECREASE_SPEED, AURA_EFFECT_HANDLE_REAL);
                        break;
                    case eSpells::WoundPoison:
                        OnEffectApply += AuraEffectApplyFn(spell_rog_venom_rush_AuraScript::OnApply, EFFECT_1, SPELL_AURA_MOD_HEALING_PCT, AURA_EFFECT_HANDLE_REAL);
                        OnEffectRemove += AuraEffectRemoveFn(spell_rog_venom_rush_AuraScript::OnRemove, EFFECT_1, SPELL_AURA_MOD_HEALING_PCT, AURA_EFFECT_HANDLE_REAL);
                        break;
                    case ROGUE_SPELL_LEECHING_POISON_DEBUFF:
                        OnEffectApply += AuraEffectApplyFn(spell_rog_venom_rush_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                        OnEffectRemove += AuraEffectRemoveFn(spell_rog_venom_rush_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                        break;
                    default:
                        break;
                }
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_rog_venom_rush_AuraScript();
        }
};

/// Death from Above (Jump back to target) - 178236
class spell_rog_death_from_above_return : public SpellScriptLoader
{
    public:
        spell_rog_death_from_above_return() : SpellScriptLoader("spell_rog_death_from_above_return") { }

        class spell_rog_death_from_above_return_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_rog_death_from_above_return_SpellScript);

            enum eSpells
            {
                DeathFromAboveBonus = 163786,
                DeathFromAboveJump  = 178236
            };

            void ChangeJumpDestination(SpellEffIndex p_EffIndex)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    PreventHitDefaultEffect(p_EffIndex);

                    SpellInfo const* l_SpellInfo = GetSpellInfo();
                    float l_X = l_Caster->GetPositionX() + (5.0f * cos(l_Caster->GetOrientation() + M_PI));
                    float l_Y = l_Caster->GetPositionY() + (5.0f * sin(l_Caster->GetOrientation() + M_PI));
                    float l_Z = l_Caster->GetPositionZ() + 15.0f;

                    float l_SpeedXY, l_SpeedZ;

                    if (l_SpellInfo->Effects[p_EffIndex].MiscValue)
                        l_SpeedZ = float(l_SpellInfo->Effects[p_EffIndex].MiscValue) / 10;
                    else if (l_SpellInfo->Effects[p_EffIndex].MiscValueB)
                        l_SpeedZ = float(l_SpellInfo->Effects[p_EffIndex].MiscValueB) / 10;
                    else
                        l_SpeedZ = 10.0f;

                    if (l_SpellInfo->Effects[p_EffIndex].ValueMultiplier)
                        l_SpeedXY = l_SpellInfo->Effects[p_EffIndex].ValueMultiplier;
                    else
                        l_SpeedXY = l_Caster->GetExactDist2d(l_X, l_Y) * 10.0f / l_SpeedZ;

                    l_Caster->GetMotionMaster()->MoveJump(l_X, l_Y, l_Z, l_SpeedXY, l_SpeedZ, l_Caster->GetOrientation(), l_SpellInfo->Id);
                }
            }

            void Register()
            {
                OnEffectLaunch += SpellEffectFn(spell_rog_death_from_above_return_SpellScript::ChangeJumpDestination, EFFECT_0, SPELL_EFFECT_JUMP_DEST);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_rog_death_from_above_return_SpellScript();
        }
};

/// Death from Above - 152150
class spell_rog_death_from_above : public SpellScriptLoader
{
    public:
        spell_rog_death_from_above() : SpellScriptLoader("spell_rog_death_from_above") { }

        class spell_rog_death_from_above_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_rog_death_from_above_SpellScript);

            enum eSpells
            {
                DeathFromAboveBonus = 163786,
                DeathFromAboveJump  = 178236
            };

            void HandleOnHit(SpellEffIndex)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (Unit* l_Target = GetHitUnit())
                    {
                        l_Caster->CastSpell(l_Caster, eSpells::DeathFromAboveJump, true);
                        l_Caster->CastSpell(l_Caster, eSpells::DeathFromAboveBonus, true);
                    }
                }
            }

            void HandleRegisterCombo(SpellEffIndex p_EffIndex)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (Unit* l_Target = GetHitUnit())
                    {
                        if (AuraPtr l_Aura = l_Target->GetAura(GetSpellInfo()->Id, l_Caster->GetGUID()))
                        {
                            if (AuraEffectPtr l_AuraEffect = l_Aura->GetEffect(p_EffIndex))
                                l_AuraEffect->ChangeAmount(l_Caster->GetPower(Powers::POWER_COMBO_POINT));
                        }
                    }
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_rog_death_from_above_SpellScript::HandleOnHit, EFFECT_0, SPELL_EFFECT_DUMMY);
                OnEffectHitTarget += SpellEffectFn(spell_rog_death_from_above_SpellScript::HandleRegisterCombo, EFFECT_5, SPELL_EFFECT_APPLY_AURA);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_rog_death_from_above_SpellScript();
        }

        class spell_rog_death_from_above_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_rog_death_from_above_AuraScript);

            enum eSpells
            {
                DeathFromAboveJump   = 178153,
                SpellRogueEnvenom    = 32645,
                SpellRogueEviscerate = 2098
            };

            void OnTick(constAuraEffectPtr p_AurEff)
            {
                if (GetCaster() == nullptr)
                    return;

                if (Player* l_Player = GetCaster()->ToPlayer())
                {
                    if (Unit* l_Target = GetTarget())
                    {
                        l_Player->CastSpell(l_Target, eSpells::DeathFromAboveJump, true);
                        l_Player->SetPower(Powers::POWER_COMBO_POINT, p_AurEff->GetAmount());

                        if (l_Player->GetSpecializationId(l_Player->GetActiveSpec()) == SpecIndex::SPEC_ROGUE_ASSASSINATION)
                            l_Player->CastSpell(l_Target, eSpells::SpellRogueEnvenom, true);
                        else
                            l_Player->CastSpell(l_Target, eSpells::SpellRogueEviscerate, true);

                        l_Player->SetPower(Powers::POWER_COMBO_POINT, 0);
                    }
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_rog_death_from_above_AuraScript::OnTick, EFFECT_5, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_rog_death_from_above_AuraScript();
        }
};

/// Shadow Reflection (aura proc) - 152151
class spell_rog_shadow_reflection_proc : public SpellScriptLoader
{
    public:
        spell_rog_shadow_reflection_proc() : SpellScriptLoader("spell_rog_shadow_reflection_proc") { }

        class spell_rog_shadow_reflection_proc_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_rog_shadow_reflection_proc_AuraScript);

            enum eDatas
            {
                NpcShadowReflection = 77726,
                AddSpellToQueue     = 0,
                FinishFirstPhase    = 1
            };

            void OnProc(constAuraEffectPtr p_AurEff, ProcEventInfo& p_EventInfo)
            {
                PreventDefaultAction();

                if (p_EventInfo.GetDamageInfo()->GetSpellInfo() == nullptr)
                    return;

                if (Unit* l_Caster = GetCaster())
                {
                    if (Unit* l_Target = p_EventInfo.GetActionTarget())
                    {
                        for (Unit::ControlList::const_iterator l_Iter = l_Caster->m_Controlled.begin(); l_Iter != l_Caster->m_Controlled.end(); ++l_Iter)
                        {
                            if ((*l_Iter)->GetEntry() != eDatas::NpcShadowReflection)
                                continue;

                            if (Creature* l_Creature = (*l_Iter)->ToCreature())
                            {
                                if (!l_Creature->IsAIEnabled)
                                    break;

                                uint64 l_Data;
                                ((uint32*)(&l_Data))[0] = p_EventInfo.GetDamageInfo()->GetSpellInfo()->Id;
                                ((uint32*)(&l_Data))[1] = p_AurEff->GetBase()->GetMaxDuration() - p_AurEff->GetBase()->GetDuration();

                                l_Creature->AI()->SetGUID(l_Data, eDatas::AddSpellToQueue);
                                break;
                            }
                        }
                    }
                }
            }

            void OnUpdate(uint32 p_Diff)
            {
                if ((GetDuration() - p_Diff) <= 8000)
                {
                    if (Unit* l_Caster = GetCaster())
                    {
                        for (Unit::ControlList::const_iterator l_Iter = l_Caster->m_Controlled.begin(); l_Iter != l_Caster->m_Controlled.end(); ++l_Iter)
                        {
                            if ((*l_Iter)->GetEntry() != eDatas::NpcShadowReflection)
                                continue;

                            if (Creature* l_Creature = (*l_Iter)->ToCreature())
                            {
                                if (!l_Creature->IsAIEnabled)
                                    break;

                                l_Creature->AI()->SetGUID(0, eDatas::FinishFirstPhase);
                                return;
                            }
                        }
                    }
                }
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_rog_shadow_reflection_proc_AuraScript::OnProc, EFFECT_1, SPELL_AURA_DUMMY);
                OnAuraUpdate += AuraUpdateFn(spell_rog_shadow_reflection_proc_AuraScript::OnUpdate);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_rog_shadow_reflection_proc_AuraScript();
        }
};

/// Shadow Reflection - 156744
class spell_rog_shadow_reflection : public SpellScriptLoader
{
    public:
        spell_rog_shadow_reflection() : SpellScriptLoader("spell_rog_shadow_reflection") { }

        class spell_rog_shadow_reflection_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_rog_shadow_reflection_SpellScript);

            enum eSpells
            {
                ShadowReflectionClone = 168691
            };

            void HandleOnHit(SpellEffIndex)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (Unit* l_Target = GetHitUnit())
                        l_Target->CastSpell(l_Caster, eSpells::ShadowReflectionClone, true);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_rog_shadow_reflection_SpellScript::HandleOnHit, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_rog_shadow_reflection_SpellScript();
        }
};

/// Vendetta - 79140
class spell_rog_enhanced_vendetta : public SpellScriptLoader
{
    public:
        spell_rog_enhanced_vendetta() : SpellScriptLoader("spell_rog_enhanced_vendetta") { }

        class spell_rog_enhanced_vendetta_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_rog_enhanced_vendetta_SpellScript);

            enum eSpells
            {
                EnhancedVendetta     = 157514,
                EnhancedVendettaBuff = 158108
            };

            void HandleOnHit(SpellEffIndex)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (!l_Caster->HasAura(eSpells::EnhancedVendetta))
                        return;

                    l_Caster->CastSpell(l_Caster, eSpells::EnhancedVendettaBuff, true);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_rog_enhanced_vendetta_SpellScript::HandleOnHit, EFFECT_0, SPELL_EFFECT_APPLY_AURA);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_rog_enhanced_vendetta_SpellScript();
        }
};

/// Killing Spree - 51690
class spell_rog_killing_spree: public SpellScriptLoader
{
    public:
        spell_rog_killing_spree() : SpellScriptLoader("spell_rog_killing_spree") { }

        class spell_rog_killing_spree_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_rog_killing_spree_AuraScript);

            enum eSpell
            {
                KillingSpreeDeselect = 61851
            };

            void OnApply(constAuraEffectPtr p_AurEff, AuraEffectHandleModes p_Mode)
            {
                if (Unit* l_Caster = GetCaster())
                    l_Caster->CastSpell(l_Caster, eSpell::KillingSpreeDeselect, true);
            }

            void OnTick(constAuraEffectPtr)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (!l_Caster->HasAura(ROGUE_SPELL_BLADE_FLURRY_AURA))
                    {
                        Unit* l_Target = l_Caster->getVictim();
                        if (!l_Target && l_Caster->ToPlayer())
                            l_Target = l_Caster->ToPlayer()->GetSelectedUnit();
                        if (!l_Target)
                            return;

                        l_Caster->CastSpell(l_Target, ROGUE_SPELL_KILLING_SPREE_TELEPORT, true);
                        l_Caster->CastSpell(l_Target, ROGUE_SPELL_KILLING_SPREE_DAMAGES, true);
                    }
                    else
                    {
                        std::list<Unit*> l_TargetList;
                        float l_Radius = 10.0f;

                        JadeCore::AnyUnfriendlyUnitInObjectRangeCheck l_Check(l_Caster, l_Caster, l_Radius);
                        JadeCore::UnitListSearcher<JadeCore::AnyUnfriendlyUnitInObjectRangeCheck> l_Searcher(l_Caster, l_TargetList, l_Check);
                        l_Caster->VisitNearbyObject(l_Radius, l_Searcher);

                        l_TargetList.remove_if([this, l_Caster](Unit* p_Unit) -> bool
                        {
                            if (p_Unit == nullptr || p_Unit->HasBreakableByDamageCrowdControlAura() || !l_Caster->IsValidAttackTarget(p_Unit))
                                return true;

                            return false;
                        });

                        if (l_TargetList.empty())
                            return;

                        Unit* l_Target = JadeCore::Containers::SelectRandomContainerElement(l_TargetList);
                        if (!l_Target)
                            return;

                        l_Caster->CastSpell(l_Target, ROGUE_SPELL_KILLING_SPREE_TELEPORT, true);
                        l_Caster->CastSpell(l_Target, ROGUE_SPELL_KILLING_SPREE_DAMAGES, true);
                    }
                }
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_rog_killing_spree_AuraScript::OnApply, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_rog_killing_spree_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_rog_killing_spree_AuraScript();
        }
};

/// Called by Vanish - 1856
/// Glyph of Decoy - 56800
class spell_rog_glyph_of_decoy: public SpellScriptLoader
{
    public:
        spell_rog_glyph_of_decoy() : SpellScriptLoader("spell_rog_glyph_of_decoy") { }

        class spell_rog_glyph_of_decoy_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_rog_glyph_of_decoy_SpellScript);

            void HandleOnHit()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (!l_Caster->HasAura(ROGUE_SPELL_GLYPH_OF_DECOY))
                        return;

                    l_Caster->SummonCreature(62261, *l_Caster);
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_rog_glyph_of_decoy_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_rog_glyph_of_decoy_SpellScript();
        }
};

/// Shuriken Toss - 114014
class spell_rog_shuriken_toss: public SpellScriptLoader
{
    public:
        spell_rog_shuriken_toss() : SpellScriptLoader("spell_rog_shuriken_toss") { }

        class spell_rog_shuriken_toss_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_rog_shuriken_toss_SpellScript);

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                    if (Unit* target = GetHitUnit())
                        if (caster->GetDistance(target) > 10.0f)
                            caster->CastSpell(caster, ROGUE_SPELL_SHURIKEN_TOSS_CHANGE_MELEE, true);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_rog_shuriken_toss_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_rog_shuriken_toss_SpellScript();
        }
};

/// Called by Marked for Death (check caster) - 140149
/// Marked for Death - 137619
class spell_rog_marked_for_death: public SpellScriptLoader
{
    public:
        spell_rog_marked_for_death() : SpellScriptLoader("spell_rog_marked_for_death") { }

        class spell_rog_marked_for_death_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_rog_marked_for_death_AuraScript);

            void OnRemove(constAuraEffectPtr aurEff, AuraEffectHandleModes mode)
            {
                if (!GetCaster())
                    return;

                Player* plr = GetCaster()->ToPlayer();
                if (!plr)
                    return;

                if (!GetTargetApplication())
                    return;

                if (!GetTargetApplication()->GetBase())
                    return;

                if (!GetTarget())
                    return;

                AuraRemoveMode removeMode = GetTargetApplication()->GetRemoveMode();
                if (removeMode == AURA_REMOVE_BY_DEATH)
                {
                    if (plr->HasSpellCooldown(ROGUE_SPELL_MARKED_FOR_DEATH))
                        plr->RemoveSpellCooldown(ROGUE_SPELL_MARKED_FOR_DEATH, true);

                    GetTarget()->RemoveAura(ROGUE_SPELL_MARKED_FOR_DEATH);
                }
            }

            void Register()
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_rog_marked_for_death_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_rog_marked_for_death_AuraScript();
        }
};

/// Called by Ambush - 8676, Garrote - 703 and Cheap Shot - 1833
/// Cloak and Dagger - 138106
class spell_rog_cloak_and_dagger: public SpellScriptLoader
{
    public:
        spell_rog_cloak_and_dagger() : SpellScriptLoader("spell_rog_cloak_and_dagger") { }

        class spell_rog_cloak_and_dagger_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_rog_cloak_and_dagger_SpellScript);

            enum eSpells
            {
                CloakAndDagger      = 138106,
                TeleportBack        = 36563,
                GarroteDot          = 703,
                FindWeekness        = 91023,
                FindWeeknessProc    = 91021
            };

            SpellCastResult CheckCast()
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetExplTargetUnit();
                float l_BasicRadius = 5.0f;

                if (l_Target == nullptr)
                    return SPELL_FAILED_BAD_TARGETS;

                if (l_Caster->HasUnitState(UNIT_STATE_ROOT) && l_Target->GetDistance(l_Caster) > l_BasicRadius)
                    return SPELL_FAILED_ROOTED;

                return SPELL_CAST_OK;
            }

            void HandleOnHit()
            {
                Player* l_Player = GetCaster()->ToPlayer();
                Unit* l_Target = GetHitUnit();

                if (l_Target == nullptr || l_Player == nullptr)
                    return;

                if (l_Player->HasTalent(eSpells::CloakAndDagger, l_Player->GetActiveSpec()) && !l_Player->HasUnitState(UNIT_STATE_ROOT))
                    l_Player->CastSpell(l_Target, eSpells::TeleportBack, true);

                if (GetSpellInfo()->Id == eSpells::GarroteDot && l_Player->HasAura(eSpells::FindWeekness))
                    l_Player->AddAura(eSpells::FindWeeknessProc, l_Target);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_rog_cloak_and_dagger_SpellScript::HandleOnHit);
                OnCheckCast += SpellCheckCastFn(spell_rog_cloak_and_dagger_SpellScript::CheckCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_rog_cloak_and_dagger_SpellScript();
        }
};

/// Cheat Death - 31230
class spell_rog_cheat_death: public SpellScriptLoader
{
    public:
        spell_rog_cheat_death() : SpellScriptLoader("spell_rog_cheat_death") { }

        class spell_rog_cheat_death_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_rog_cheat_death_AuraScript);

            void CalculateAmount(constAuraEffectPtr /*auraEffect*/, int32& amount, bool& /*canBeRecalculated*/)
            {
                amount = -1;
            }

            void Absorb(AuraEffectPtr /*auraEffect*/, DamageInfo& dmgInfo, uint32& absorbAmount)
            {
                if (!GetTarget())
                    return;

                if (Player* target = GetTarget()->ToPlayer())
                {
                    if (dmgInfo.GetDamage() < target->GetHealth())
                        return;

                    if (target->HasAura(ROGUE_SPELL_CHEATED_DEATH_MARKER))
                        return;

                    target->CastSpell(target, ROGUE_SPELL_CHEAT_DEATH_REDUCE_DAMAGE, true);
                    target->CastSpell(target, ROGUE_SPELL_CHEATED_DEATH_MARKER, true);

                    uint32 health10 = target->CountPctFromMaxHealth(10);

                    // hp > 10% - absorb hp till 10%
                    if (target->GetHealth() > health10)
                        absorbAmount = dmgInfo.GetDamage() - target->GetHealth() + health10;
                    // hp lower than 10% - absorb everything
                    else
                        absorbAmount = dmgInfo.GetDamage();
                }
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_rog_cheat_death_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
                OnEffectAbsorb += AuraEffectAbsorbFn(spell_rog_cheat_death_AuraScript::Absorb, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_rog_cheat_death_AuraScript();
        }
};

/// Blade Flurry - 13877
class spell_rog_blade_flurry: public SpellScriptLoader
{
    public:
        spell_rog_blade_flurry() : SpellScriptLoader("spell_rog_blade_flurry") { }

        class spell_rog_blade_flurry_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_rog_blade_flurry_AuraScript);

            void OnProc(constAuraEffectPtr p_AurEff, ProcEventInfo& p_EventInfo)
            {
                PreventDefaultAction();

                if (!GetTarget())
                    return;

                if (p_EventInfo.GetActor()->GetGUID() != GetTarget()->GetGUID())
                    return;

                if (Unit* l_Caster = GetCaster())
                {
                    SpellInfo const* l_ProcSpell = p_EventInfo.GetDamageInfo()->GetSpellInfo();
                    SpellInfo const* l_SpellInfo = GetSpellInfo();
                    int32 l_Damage = CalculatePct(p_EventInfo.GetDamageInfo()->GetDamage(), l_SpellInfo->Effects[EFFECT_2].BasePoints);

                    if (!l_Damage || p_EventInfo.GetDamageInfo()->GetDamageType() == DOT)
                        return;

                    if (l_ProcSpell && !l_ProcSpell->CanTriggerBladeFlurry())
                        return;

                    if (Unit* l_Target = l_Caster->SelectNearbyTarget(p_EventInfo.GetActionTarget(), NOMINAL_MELEE_RANGE, 0U, true, true, false, true))
                        l_Caster->CastCustomSpell(l_Target, ROGUE_SPELL_BLADE_FLURRY_DAMAGE, &l_Damage, NULL, NULL, true);
                }
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_rog_blade_flurry_AuraScript::OnProc, EFFECT_0, SPELL_AURA_MOD_POWER_REGEN_PERCENT);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_rog_blade_flurry_AuraScript();
        }
};

/// Cloak of Shadows - 31224
class spell_rog_cloak_of_shadows: public SpellScriptLoader
{
    public:
        spell_rog_cloak_of_shadows() : SpellScriptLoader("spell_rog_cloak_of_shadows") { }

        class spell_rog_cloak_of_shadows_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_rog_cloak_of_shadows_SpellScript);

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                {
                    uint32 dispelMask = SpellInfo::GetDispelMask(DISPEL_ALL);
                    Unit::AuraApplicationMap& Auras = caster->GetAppliedAuras();
                    for (Unit::AuraApplicationMap::iterator iter = Auras.begin(); iter != Auras.end();)
                    {
                        // remove all harmful spells on you...
                        SpellInfo const* spell = iter->second->GetBase()->GetSpellInfo();
                        if ((spell->DmgClass == SPELL_DAMAGE_CLASS_MAGIC // only affect magic spells
                            || (spell->GetDispelMask() & dispelMask) || (spell->GetSchoolMask() & SPELL_SCHOOL_MASK_MAGIC))
                            // ignore positive and passive auras
                            && !iter->second->IsPositive() && !iter->second->GetBase()->IsPassive() && GetSpellInfo()->CanDispelAura(spell)
                            && !(iter->second->GetBase()->GetCaster() && iter->second->GetBase()->GetCaster()->GetGUID() == caster->GetGUID()))
                        {
                            caster->RemoveAura(iter);
                        }
                        else
                            ++iter;
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_rog_cloak_of_shadows_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_rog_cloak_of_shadows_SpellScript();
        }
};

/// Combat Readiness - 74001
class spell_rog_combat_readiness: public SpellScriptLoader
{
    public:
        spell_rog_combat_readiness() : SpellScriptLoader("spell_rog_combat_readiness") { }

        class spell_rog_combat_readiness_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_rog_combat_readiness_AuraScript);

            uint32 update;
            bool hit;

            void HandleApply(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes mode)
            {
                if (GetCaster())
                {
                    update = 10000;
                    hit = false;
                }
            }

            void OnUpdate(uint32 diff)
            {
                update -= diff;

                if (Unit* caster = GetCaster())
                {
                    if (caster->HasAura(ROGUE_SPELL_COMBAT_INSIGHT))
                        hit = true;

                    if (update <= 0)
                        if (!hit)
                            caster->RemoveAura(ROGUE_SPELL_COMBAT_READINESS);
                }
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_rog_combat_readiness_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
                OnAuraUpdate += AuraUpdateFn(spell_rog_combat_readiness_AuraScript::OnUpdate);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_rog_combat_readiness_AuraScript();
        }
};

/// Called by Kidney Shot - 408 and Cheap Shot - 1833
/// Nerve Strike - 108210
class spell_rog_nerve_strike: public SpellScriptLoader
{
    public:
        spell_rog_nerve_strike() : SpellScriptLoader("spell_rog_nerve_strike") { }

        class spell_rog_combat_readiness_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_rog_combat_readiness_AuraScript);

            void HandleRemove(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes mode)
            {
                if (Unit* caster = GetCaster())
                    if (Unit* target = GetTarget())
                        if (caster->HasAura(ROGUE_SPELL_NERVE_STRIKE_AURA))
                            caster->CastSpell(target, ROGUE_SPELL_NERVE_STRIKE_REDUCE_DAMAGE_DONE, true);
            }

            void Register()
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_rog_combat_readiness_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_MOD_STUN, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_rog_combat_readiness_AuraScript();
        }
};

enum SanguinaryVein
{
    SpellRogueSanguinaryVein = 79147
};

/// Called by Rupture - 1943, Garrote - 703, Crimson Tempest - 122233
/// Sanguinary Vein - 79147
class spell_rog_sanguinary_vein: public SpellScriptLoader
{
    public:
        spell_rog_sanguinary_vein() : SpellScriptLoader("spell_rog_sanguinary_vein") { }

        class spell_rog_sanguinary_vein_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_rog_sanguinary_vein_AuraScript);

            void OnApply(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetTarget();

                if (l_Caster == nullptr || l_Target == nullptr)
                    return;

                if (!l_Caster->HasAura(SanguinaryVein::SpellRogueSanguinaryVein))
                    return;

                l_Caster->CastSpell(l_Target, ROGUE_SPELL_SANGUINARY_VEIN_DEBUFF, true);
            }

            void OnRemove(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetTarget();

                if (l_Caster == nullptr || l_Target == nullptr)
                    return;

                if (l_Target->HasAura(ROGUE_SPELL_HEMORRHAGE, l_Caster->GetGUID()) && l_Caster->HasAura(ROGUE_SPELL_GLYPH_OF_HEMORRHAGING_VEINS))
                    return;

                if (l_Target->HasAura(ROGUE_SPELL_RUPTURE_DOT, l_Caster->GetGUID()) ||
                    l_Target->HasAura(ROGUE_SPELL_GARROTE_DOT, l_Caster->GetGUID()) || l_Target->HasAura(ROGUE_SPELL_CRIMSON_TEMPEST_DOT, l_Caster->GetGUID()))
                    return;

                if (l_Target->HasAura(ROGUE_SPELL_SANGUINARY_VEIN_DEBUFF, l_Caster->GetGUID()))
                    l_Target->RemoveAura(ROGUE_SPELL_SANGUINARY_VEIN_DEBUFF, l_Caster->GetGUID());
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_rog_sanguinary_vein_AuraScript::OnApply, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_rog_sanguinary_vein_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_rog_sanguinary_vein_AuraScript();
        }
};

/// Last Update 6.1.2
/// Hemorrhage - 16511
class spell_rog_hemorrhage: public SpellScriptLoader
{
    public:
        spell_rog_hemorrhage() : SpellScriptLoader("spell_rog_hemorrhage") { }

        class spell_rog_hemorrhage_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_rog_hemorrhage_SpellScript);

            enum eSpells
            {
                GlyphofHemorrhage           = 56807,
                GlyphofHemorrhagingVeins    = 146631
            };

            void HandleOnHit()
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetHitUnit();

                if (l_Caster->HasAura(eSpells::GlyphofHemorrhagingVeins))
                    l_Caster->CastSpell(l_Target, eSpells::GlyphofHemorrhagingVeins, true);

                if (l_Caster->HasAura(eSpells::GlyphofHemorrhage))
                {
                    if (!l_Target->HasAuraState(AURA_STATE_BLEEDING))
                        return;

                    SetHitDamage(0);
                }

            }

            void Register()
            {
                OnHit += SpellHitFn(spell_rog_hemorrhage_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_rog_hemorrhage_SpellScript();
        }

        class spell_rog_hemorrhage_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_rog_hemorrhage_AuraScript);

            void OnApply(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetTarget();

                if (l_Caster == nullptr || l_Target == nullptr)
                    return;

                if (!l_Caster->HasAura(SanguinaryVein::SpellRogueSanguinaryVein))
                    return;

                if (!l_Caster->HasAura(ROGUE_SPELL_GLYPH_OF_HEMORRHAGING_VEINS))
                    return;

                l_Caster->CastSpell(l_Target, ROGUE_SPELL_SANGUINARY_VEIN_DEBUFF, true);
            }

            void OnRemove(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetTarget();

                if (l_Caster == nullptr || l_Target == nullptr)
                    return;

                if (l_Target->HasAura(ROGUE_SPELL_RUPTURE_DOT, l_Caster->GetGUID()) ||
                    l_Target->HasAura(ROGUE_SPELL_GARROTE_DOT, l_Caster->GetGUID()) || l_Target->HasAura(ROGUE_SPELL_CRIMSON_TEMPEST_DOT, l_Caster->GetGUID()))
                    return;

                if (l_Target->HasAura(ROGUE_SPELL_SANGUINARY_VEIN_DEBUFF, l_Caster->GetGUID()))
                    l_Target->RemoveAura(ROGUE_SPELL_SANGUINARY_VEIN_DEBUFF, l_Caster->GetGUID());
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_rog_hemorrhage_AuraScript::OnApply, EFFECT_3, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_rog_hemorrhage_AuraScript::OnRemove, EFFECT_3, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_rog_hemorrhage_AuraScript();
        }
};

/// Last Update 6.1.2
/// Called by Envenom - 32645
class spell_rog_envenom: public SpellScriptLoader
{
    public:
        spell_rog_envenom() : SpellScriptLoader("spell_rog_envenom") { }

        class spell_rog_envenom_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_rog_envenom_SpellScript);

            enum eSpells
            {
                SliceAndDice        = 5171,
                Tier5Bonus2P        = 37169,
                T17Assassination4P  = 166886,
                EnvenomComboPoint   = 167106,
                Ruthlessness        = 14161,
                RelentlessStrikes   = 58423,
                RelentlessStrikesProc = 14181
            };

            void HandleDamage(SpellEffIndex effIndex)
            {
                PreventHitDefaultEffect(effIndex);

                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetHitUnit();

                if (l_Target == nullptr)
                    return;

                int32 l_ComboPoint = l_Caster->GetPower(Powers::POWER_COMBO_POINT);
                int32 l_Damage = 0;

                if (l_ComboPoint)
                {
                    l_Damage += int32(1.05 * l_Caster->GetTotalAttackPowerValue(WeaponAttackType::BaseAttack) * 0.306 * l_ComboPoint + (l_ComboPoint * GetSpellInfo()->Effects[EFFECT_0].BasePoints));

                    /// Tier 5 Bonus 2 pieces
                    if (AuraEffectPtr l_Tier5Bonus2P = l_Caster->GetAuraEffect(eSpells::Tier5Bonus2P, EFFECT_0))
                        l_Damage += l_ComboPoint * l_Tier5Bonus2P->GetAmount();

                    if (AuraPtr l_SliceAndDice = l_Caster->GetAura(eSpells::SliceAndDice))
                        l_SliceAndDice->RefreshDuration();
                }

                l_Damage = l_Caster->SpellDamageBonusDone(l_Target, GetSpellInfo(), l_Damage, 0, SPELL_DIRECT_DAMAGE);
                l_Damage = l_Target->SpellDamageBonusTaken(l_Caster, GetSpellInfo(), l_Damage, SPELL_DIRECT_DAMAGE);

                SetHitDamage(l_Damage);

                /// Envenom refunds 1 Combo Point.
                if (l_Caster->HasAura(eSpells::T17Assassination4P))
                    l_Caster->CastSpell(l_Caster, eSpells::EnvenomComboPoint, true);

                /// Relentless Strikes
                if (l_Caster->HasAura(eSpells::RelentlessStrikes))
                {
                    if (SpellInfo const* l_RelentlessStrikes = sSpellMgr->GetSpellInfo(eSpells::Ruthlessness))
                        if (roll_chance_i(l_RelentlessStrikes->Effects[0].PointsPerComboPoint * l_ComboPoint))
                                l_Caster->CastSpell(l_Caster, eSpells::RelentlessStrikesProc, true);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_rog_envenom_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_rog_envenom_SpellScript();
        }
};

/// Called by Envenom - 32645 and Eviscerate - 2098
/// Cut to the Chase - 51667
class spell_rog_cut_to_the_chase: public SpellScriptLoader
{
    public:
        spell_rog_cut_to_the_chase() : SpellScriptLoader("spell_rog_cut_to_the_chase") { }

        class spell_rog_cut_to_the_chase_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_rog_cut_to_the_chase_SpellScript);

            enum eSpells
            {
                SliceAndDice = 5171,
            };

            void HandleOnHit()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (l_Caster->HasAura(ROGUE_SPELL_CUT_TO_THE_CHASE_AURA))
                    {
                        if (AuraPtr l_SliceAndDice = l_Caster->GetAura(eSpells::SliceAndDice, l_Caster->GetGUID()))
                        {
                            l_SliceAndDice->SetDuration(36 * IN_MILLISECONDS);
                            l_SliceAndDice->SetMaxDuration(36 * IN_MILLISECONDS);
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_rog_cut_to_the_chase_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_rog_cut_to_the_chase_SpellScript();
        }
};

/// Called by Rupture - 1943
/// Venomous Wounds - 79134
class spell_rog_venomous_wounds: public SpellScriptLoader
{
    public:
        spell_rog_venomous_wounds() : SpellScriptLoader("spell_rog_venomous_wounds") { }

        class spell_rog_venomous_wounds_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_rog_venomous_wounds_AuraScript);

            enum eSpells
            {
                VenomousWoundsDamage = 79136,
                VenomousVimEnergize  = 51637
            };

            void HandleEffectPeriodic(constAuraEffectPtr)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (Unit* l_Target = GetTarget())
                    {
                        if (!l_Caster->HasPoisonTarget(l_Target->GetGUIDLow()))
                            return;

                        if (l_Caster->ToPlayer()->GetSpecializationId(l_Caster->ToPlayer()->GetActiveSpec()) != SpecIndex::SPEC_ROGUE_ASSASSINATION)
                            return;

                        l_Caster->CastSpell(l_Target, eSpells::VenomousWoundsDamage, true);
                        l_Caster->EnergizeBySpell(l_Caster, eSpells::VenomousVimEnergize, 10, POWER_ENERGY);
                    }
                }
            }

            void OnRemove(constAuraEffectPtr p_AurEff, AuraEffectHandleModes)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    AuraRemoveMode l_RemoveMode = GetTargetApplication()->GetRemoveMode();
                    if (l_RemoveMode == AURA_REMOVE_BY_DEATH)
                    {
                        if (AuraPtr l_Rupture = p_AurEff->GetBase())
                        {
                            /// If an enemy dies while afflicted by your Rupture, you regain energy proportional to the remaining Rupture duration
                            /// 120 for max duration (12s)
                            int32 l_Duration = int32(l_Rupture->GetDuration() / 100 / 2);
                            l_Caster->CastCustomSpell(l_Caster, eSpells::VenomousVimEnergize, &l_Duration, NULL, NULL, true);
                        }
                    }
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_rog_venomous_wounds_AuraScript::HandleEffectPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
                OnEffectRemove += AuraEffectRemoveFn(spell_rog_venomous_wounds_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_rog_venomous_wounds_AuraScript();
        }
};

enum battleGroundsFlagsSpells
{
    BG_WS_SPELL_WARSONG_FLAG    = 23333,
    BG_WS_SPELL_SILVERWING_FLAG = 23335,
    BG_KT_SPELL_ORB_PICKED_UP_1 = 121164,
    BG_KT_SPELL_ORB_PICKED_UP_2 = 121175,
    BG_KT_SPELL_ORB_PICKED_UP_3 = 121176,
    BG_KT_SPELL_ORB_PICKED_UP_4 = 121177,
    BG_KT_ALLIANCE_INSIGNIA     = 131527,
    BG_KT_HORDE_INSIGNIA        = 131528
};

/// Shroud of Concealment - 115834
class spell_rog_shroud_of_concealment: public SpellScriptLoader
{
    public:
        spell_rog_shroud_of_concealment() : SpellScriptLoader("spell_rog_shroud_of_concealment") { }

        class spell_rog_shroud_of_concealment_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_rog_shroud_of_concealment_SpellScript);

            void SelectTargets(std::list<WorldObject*>& p_Targets)
            {
                Unit* l_Caster = GetCaster();
                
                if (l_Caster == nullptr)
                    return;

                std::list<WorldObject*> targetsToRemove;
                p_Targets.remove_if(JadeCore::UnitAuraCheck(true, BG_WS_SPELL_WARSONG_FLAG));
                p_Targets.remove_if(JadeCore::UnitAuraCheck(true, BG_WS_SPELL_SILVERWING_FLAG));
                p_Targets.remove_if(JadeCore::UnitAuraCheck(true, BG_KT_SPELL_ORB_PICKED_UP_1));
                p_Targets.remove_if(JadeCore::UnitAuraCheck(true, BG_KT_SPELL_ORB_PICKED_UP_2));
                p_Targets.remove_if(JadeCore::UnitAuraCheck(true, BG_KT_SPELL_ORB_PICKED_UP_3));
                p_Targets.remove_if(JadeCore::UnitAuraCheck(true, BG_KT_SPELL_ORB_PICKED_UP_4));
                p_Targets.remove_if(JadeCore::UnitAuraCheck(true, BG_KT_ALLIANCE_INSIGNIA));
                p_Targets.remove_if(JadeCore::UnitAuraCheck(true, BG_KT_HORDE_INSIGNIA));

                for (auto itr : p_Targets)
                {
                    if (Unit* l_Target = itr->ToUnit())
                    {
                        if ((!l_Target->IsInRaidWith(l_Caster) && !l_Target->IsInPartyWith(l_Caster)) ||
                            l_Target->isInCombat() || l_Target->HasUnitState(UNIT_STATE_CASTING))
                            targetsToRemove.push_back(itr);
                    }
                }

                for (auto itr : targetsToRemove)
                    p_Targets.remove(itr);
            }

            void HandleOnHit()
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetHitUnit();

                if (l_Target == nullptr)
                    return;

                if (AuraPtr l_ShroudOfConcealment = l_Target->GetAura(ROGUE_SPELL_SHROUD_OF_CONCEALMENT_AURA, l_Caster->GetGUID()))
                {
                    if ((!l_Target->IsInRaidWith(l_Caster) && !l_Target->IsInPartyWith(l_Caster)) ||
                        l_Target->isInCombat() || l_Target->HasUnitState(UNIT_STATE_CASTING) ||
                        l_Target->HasAura(BG_WS_SPELL_WARSONG_FLAG) || l_Target->HasAura(BG_WS_SPELL_SILVERWING_FLAG) ||
                        l_Target->HasAura(BG_KT_SPELL_ORB_PICKED_UP_1) || l_Target->HasAura(BG_KT_SPELL_ORB_PICKED_UP_2) ||
                        l_Target->HasAura(BG_KT_SPELL_ORB_PICKED_UP_3) || l_Target->HasAura(BG_KT_SPELL_ORB_PICKED_UP_4))
                    {
                        l_Target->RemoveAura(ROGUE_SPELL_SHROUD_OF_CONCEALMENT_AURA, l_Caster->GetGUID());
                    }
                }
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_rog_shroud_of_concealment_SpellScript::SelectTargets, EFFECT_0, TARGET_UNIT_CASTER_AREA_RAID);
                OnHit += SpellHitFn(spell_rog_shroud_of_concealment_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_rog_shroud_of_concealment_SpellScript();
        }
};

/// Crimson Tempest - 121411
class spell_rog_crimson_tempest: public SpellScriptLoader
{
    public:
        spell_rog_crimson_tempest() : SpellScriptLoader("spell_rog_crimson_tempest") { }

        class spell_rog_crimson_tempest_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_rog_crimson_tempest_SpellScript);

            enum eSpells
            {
                Ruthlessness = 14161,
                RelentlessStrikes = 58423,
                RelentlessStrikesProc = 14181
            };

            void HandleOnHit()
            {
                if (Player* l_Player = GetCaster()->ToPlayer())
                {
                    if (Unit* l_Target = GetHitUnit())
                    {
                        if (l_Player->GetGUID() == l_Target->GetGUID())
                            return;

                        uint8 l_ComboPoint = l_Player->GetPower(Powers::POWER_COMBO_POINT);
                        int32 l_Damage = 0;

                        if (l_ComboPoint)
                        {
                            float l_Ap = l_Player->GetTotalAttackPowerValue(WeaponAttackType::BaseAttack);
                            SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(ROGUE_SPELL_CRIMSON_TEMPEST_DOT);
                            int32 l_DamageDot = 0;

                            l_Damage += int32(3 + ((l_Ap * 0.0602f) * l_ComboPoint * 1.5f));

                            if (l_SpellInfo != nullptr)
                                l_DamageDot += CalculatePct(l_Damage, l_SpellInfo->Effects[EFFECT_0].BasePoints) / 6;

                            l_Player->CastCustomSpell(l_Target, ROGUE_SPELL_CRIMSON_TEMPEST_DOT, &l_DamageDot, NULL, NULL, true);
                        }

                        SetHitDamage(l_Damage);

                        /// Relentless Strikes
                        if (l_Player->HasAura(eSpells::RelentlessStrikes) && !l_Player->HasSpellCooldown(eSpells::RelentlessStrikesProc))
                        {
                            if (SpellInfo const* l_RelentlessStrikes = sSpellMgr->GetSpellInfo(eSpells::Ruthlessness))
                            {
                                if (roll_chance_i(l_RelentlessStrikes->Effects[0].PointsPerComboPoint * l_ComboPoint))
                                {
                                    l_Player->CastSpell(l_Player, eSpells::RelentlessStrikesProc, true);
                                    l_Player->AddSpellCooldown(eSpells::RelentlessStrikesProc, 0, 2 * IN_MILLISECONDS); ///< Custom cooldown to prevent multiprocs
                                }
                            }
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_rog_crimson_tempest_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_rog_crimson_tempest_SpellScript();
        }
};

/// Slice and Dice - 5171
class spell_rog_slice_and_dice: public SpellScriptLoader
{
    public:
        spell_rog_slice_and_dice() : SpellScriptLoader("spell_rog_slice_and_dice") { }

        class spell_rog_slice_and_dice_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_rog_slice_and_dice_SpellScript);

            enum eSpells
            {
                SliceAndDice = 5171,
            };

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                {
                    if (AuraPtr sliceAndDice = caster->GetAura(eSpells::SliceAndDice))
                    {
                        int32 duration = sliceAndDice->GetDuration();
                        int32 maxDuration = sliceAndDice->GetMaxDuration();

                        // Replace old duration of Slice and Dice by the new duration ...
                        // ... five combo points : 36s instead of 30s
                        if (maxDuration >= 30000)
                        {
                            sliceAndDice->SetDuration(duration + 6000);
                            sliceAndDice->SetMaxDuration(maxDuration + 6000);
                        }
                        // ... four combo points : 30s instead of 25s
                        else if (maxDuration >= 25000)
                        {
                            sliceAndDice->SetDuration(duration + 5000);
                            sliceAndDice->SetMaxDuration(maxDuration + 5000);
                        }
                        // ... three combo points : 24s instead of 20s
                        else if (maxDuration >= 20000)
                        {
                            sliceAndDice->SetDuration(duration + 4000);
                            sliceAndDice->SetMaxDuration(maxDuration + 4000);
                        }
                        // ... two combo points : 18s instead of 15s
                        else if (maxDuration >= 15000)
                        {
                            sliceAndDice->SetDuration(duration + 3000);
                            sliceAndDice->SetMaxDuration(maxDuration + 3000);
                        }
                        // ... one combo point : 12s instead of 10s
                        else
                        {
                            sliceAndDice->SetDuration(duration + 2000);
                            sliceAndDice->SetMaxDuration(maxDuration + 2000);
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_rog_slice_and_dice_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_rog_slice_and_dice_SpellScript();
        }
};

/// Called by Deadly Poison - 2818
/// Deadly Poison : Instant damage - 113780
class spell_rog_deadly_poison_instant_damage: public SpellScriptLoader
{
    public:
        spell_rog_deadly_poison_instant_damage() : SpellScriptLoader("spell_rog_deadly_poison_instant_damage") { }

        class spell_rog_deadly_poison_instant_damage_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_rog_deadly_poison_instant_damage_SpellScript);

            void HandleAfterHit()
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetHitUnit();

                if (l_Target == nullptr)
                    return;

                if (l_Target->HasAura(ROGUE_SPELL_DEADLY_POISON_DOT, l_Caster->GetGUID()))
                    l_Caster->CastSpell(l_Target, ROGUE_SPELL_DEADLY_POISON_INSTANT_DAMAGE, true);
            }

            void Register()
            {
                AfterHit += SpellHitFn(spell_rog_deadly_poison_instant_damage_SpellScript::HandleAfterHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_rog_deadly_poison_instant_damage_SpellScript();
        }
};

/// Shiv - 5938
class spell_rog_shiv: public SpellScriptLoader
{
    public:
        spell_rog_shiv() : SpellScriptLoader("spell_rog_shiv") { }

        class spell_rog_shiv_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_rog_shiv_SpellScript);

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (caster->HasAura(ROGUE_SPELL_CRIPPLING_POISON))
                            caster->CastSpell(target, ROGUE_SPELL_DEBILITATING_POISON, true);
                        else if (caster->HasAura(ROGUE_SPELL_LEECHING_POISON))
                            caster->CastSpell(caster, ROGUE_SPELL_LEECH_VITALITY, true);
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_rog_shiv_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_rog_shiv_SpellScript();
        }
};

// Recuperate - 73651
class spell_rog_recuperate : public SpellScriptLoader
{
    public:
        spell_rog_recuperate() : SpellScriptLoader("spell_rog_recuperate") { }

        class spell_rog_recuperate_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_rog_recuperate_SpellScript);

            enum eSpells
            {
                Recuperate         = 73651,
                GlyphOfRecuperate  = 56806,
                ImprovedRecuperate = 61249,
            };

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (AuraPtr recuperate = _player->GetAura(eSpells::Recuperate))
                    {
                        int32 bp;
                        bp = _player->CountPctFromMaxHealth(3);

                        if (_player->HasAura(eSpells::GlyphOfRecuperate))
                            bp += _player->CountPctFromMaxHealth(1);

                        if (_player->HasAura(eSpells::ImprovedRecuperate))
                            bp += _player->CountPctFromMaxHealth(1);

                        bp = _player->SpellHealingBonusDone(_player, GetSpellInfo(), bp, 0, HEAL);
                        bp = _player->SpellHealingBonusTaken(_player, GetSpellInfo(), bp, HEAL);
                        recuperate->GetEffect(0)->ChangeAmount(bp);
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_rog_recuperate_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_rog_recuperate_SpellScript();
        }
};

/// Preparation - 14185
class spell_rog_preparation: public SpellScriptLoader
{
    public:
        spell_rog_preparation() : SpellScriptLoader("spell_rog_preparation") { }

        class spell_rog_preparation_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_rog_preparation_SpellScript);

            void HandleOnHit()
            {
                if (GetCaster()->GetTypeId() != TYPEID_PLAYER)
                    return;

                Player* l_Caster = GetCaster()->ToPlayer();
                if (l_Caster == nullptr)
                    return;

                /// immediately finishes the cooldown on certain Rogue abilities
                l_Caster->RemoveSpellCooldown(5277, true); ///< Evasion
                l_Caster->RemoveSpellCooldown(1856, true); ///< Vanish
                l_Caster->RemoveSpellCooldown(2983, true); ///< Sprint
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_rog_preparation_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_rog_preparation_SpellScript();
        }
};

class spell_rog_deadly_poison: public SpellScriptLoader
{
    public:
        spell_rog_deadly_poison() : SpellScriptLoader("spell_rog_deadly_poison") { }

        class spell_rog_deadly_poison_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_rog_deadly_poison_SpellScript);

            bool Load()
            {
                _stackAmount = 0;
                // at this point CastItem must already be initialized
                return GetCaster()->GetTypeId() == TYPEID_PLAYER && GetCastItem();
            }

            void HandleBeforeHit()
            {
                if (Unit* target = GetHitUnit())
                    // Deadly Poison
                    if (constAuraEffectPtr aurEff = target->GetAuraEffect(SPELL_AURA_PERIODIC_DAMAGE, SPELLFAMILY_ROGUE, 0x10000, 0x80000, 0, GetCaster()->GetGUID()))
                        _stackAmount = aurEff->GetBase()->GetStackAmount();
            }

            void HandleAfterHit()
            {
                if (_stackAmount < 5)
                    return;

                Player* player = GetCaster()->ToPlayer();

                if (Unit* target = GetHitUnit())
                {
                    Item* item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND);

                    if (item == GetCastItem())
                        item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);

                    if (!item)
                        return;

                    // item combat enchantments
                    for (uint8 slot = 0; slot < MAX_ENCHANTMENT_SLOT; ++slot)
                    {
                        if (slot > PRISMATIC_ENCHANTMENT_SLOT || slot < PROP_ENCHANTMENT_SLOT_0)    // not holding enchantment id
                            continue;

                        SpellItemEnchantmentEntry const* enchant = sSpellItemEnchantmentStore.LookupEntry(item->GetEnchantmentId(EnchantmentSlot(slot)));
                        if (!enchant)
                            continue;

                        for (uint8 s = 0; s < 3; ++s)
                        {
                            if (enchant->type[s] != ITEM_ENCHANTMENT_TYPE_COMBAT_SPELL)
                                continue;

                            SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(enchant->spellid[s]);
                            if (!spellInfo)
                            {
                                sLog->outError(LOG_FILTER_SPELLS_AURAS, "Player::CastItemCombatSpell Enchant %i, player (Name: %s, GUID: %u) cast unknown spell %i", enchant->ID, player->GetName(), player->GetGUIDLow(), enchant->spellid[s]);
                                continue;
                            }

                            // Proc only rogue poisons
                            if (spellInfo->SpellFamilyName != SPELLFAMILY_ROGUE || spellInfo->Dispel != DISPEL_POISON)
                                continue;

                            // Do not reproc deadly
                            if (spellInfo->SpellFamilyFlags.IsEqual(0x10000, 0x80000, 0))
                                continue;

                            if (spellInfo->IsPositive())
                                player->CastSpell(player, enchant->spellid[s], true, item);
                            else
                                player->CastSpell(target, enchant->spellid[s], true, item);
                        }
                    }
                }
            }

            void Register()
            {
                BeforeHit += SpellHitFn(spell_rog_deadly_poison_SpellScript::HandleBeforeHit);
                AfterHit += SpellHitFn(spell_rog_deadly_poison_SpellScript::HandleAfterHit);
            }

            uint8 _stackAmount;
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_rog_deadly_poison_SpellScript();
        }
};

/// Shadowstep - 36554
class spell_rog_shadowstep: public SpellScriptLoader
{
    public:
        spell_rog_shadowstep() : SpellScriptLoader("spell_rog_shadowstep") { }

        class spell_rog_shadowstep_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_rog_shadowstep_SpellScript);

            SpellCastResult CheckCast()
            {
                if (GetCaster()->HasUnitState(UNIT_STATE_ROOT))
                    return SPELL_FAILED_ROOTED;
                if (GetExplTargetUnit() && GetExplTargetUnit() == GetCaster())
                    return SPELL_FAILED_BAD_TARGETS;

                return SPELL_CAST_OK;
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_rog_shadowstep_SpellScript::CheckCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_rog_shadowstep_SpellScript();
        }
};

/// Stealth - 1784, Subterfuge - 115191 and Subterfuge effect - 115192
class spell_rog_stealth: public SpellScriptLoader
{
    public:
        spell_rog_stealth() : SpellScriptLoader("spell_rog_stealth") { }

        enum eSpells
        {
            Stealth = 1784,
            StealthSubterfuge = 115191,
            StealthSubterfugeEffect = 115192,
            StealthTriggered1 = 158188,
            StealthTriggered2 = 158185
        };

        class spell_rog_stealth_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_rog_stealth_AuraScript);

            void OnApply(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (GetSpellInfo()->Id != eSpells::StealthSubterfugeEffect)
                    {
                        l_Caster->CastSpell(l_Caster, eSpells::StealthTriggered1, true);
                        l_Caster->CastSpell(l_Caster, eSpells::StealthTriggered2, true);

                        if (l_Caster->HasAura(ROGUE_SPELL_NIGHTSTALKER_AURA))
                            l_Caster->CastSpell(l_Caster, ROGUE_SPELL_NIGHTSTALKER_DAMAGE_DONE, true);

                        if (l_Caster->HasAura(ROGUE_SPELL_SHADOW_FOCUS_AURA))
                            l_Caster->CastSpell(l_Caster, ROGUE_SPELL_SHADOW_FOCUS_COST_PCT, true);
                    }
                    else
                    {
                        if (l_Caster->HasAura(eSpells::StealthSubterfuge))
                            l_Caster->RemoveAura(eSpells::StealthSubterfuge);
                    }
                }
            }

            void OnRemove(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    AuraRemoveMode removeMode = GetTargetApplication()->GetRemoveMode();
                    if (GetSpellInfo()->Id != eSpells::StealthSubterfuge || removeMode == AURA_REMOVE_BY_CANCEL)
                    {
                        l_Caster->RemoveAura(eSpells::StealthTriggered1);

                        if (AuraPtr l_Nightstalker = l_Caster->GetAura(ROGUE_SPELL_NIGHTSTALKER_DAMAGE_DONE))
                            l_Nightstalker->SetDuration(200);   ///< We can't remove it now

                        l_Caster->RemoveAura(ROGUE_SPELL_SHADOW_FOCUS_COST_PCT);
                    }

                    if (GetSpellInfo()->Id == eSpells::StealthSubterfuge)
                    {
                        if (!l_Caster->HasAura(eSpells::StealthSubterfugeEffect))
                            l_Caster->CastSpell(l_Caster, eSpells::StealthSubterfugeEffect, true);
                    }

                    l_Caster->RemoveAurasDueToSpell(eSpells::StealthTriggered2);
                    /*if (AuraPtr l_StealthEffect = l_Caster->GetAura(eSpells::StealthTriggered2))
                    {
                        l_StealthEffect->GetEffect(EFFECT_0)->SetAmount(0);
                        l_StealthEffect->Remove();
                    }*/
                }
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_rog_stealth_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_rog_stealth_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

    AuraScript* GetAuraScript() const
    {
        return new spell_rog_stealth_AuraScript();
    }
};

/// Vanish - 1856
/// Called by Vanish triggered spell - 131361
class spell_rog_vanish : public SpellScriptLoader
{
public:
    spell_rog_vanish() : SpellScriptLoader("spell_rog_vanish") { }

    enum eSpells
    {
        WodPvpCombat4pBonus = 182303,
        WodPvpCombat4pBonusTrigger = 182304,
        WodPvpAssassination4pBonus = 170883,
        WodPvpAssassination4pBonusTrigger = 170882,
        Stealth = 1784,
        StealthShapeshift = 158188,
        Subterfuge = 108208,
        StealthSubterfuge = 115191,
        StealthSubterfugeEffect = 115192,
        GlyphOfDisappearance = 159638,
        GlyphOfVanish = 89758
    };

    class spell_rog_vanish_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_rog_vanish_AuraScript);

        void CalculateAmount(constAuraEffectPtr p_AurEff, int32 & p_Amount, bool & /*canBeRecalculated*/)
        {
            if (Unit* l_Caster = GetCaster())
            {
                if (l_Caster->HasAura(eSpells::StealthSubterfugeEffect))
                    p_Amount = 1;
            }
        }

        void OnApply(constAuraEffectPtr p_AurEff, AuraEffectHandleModes /*mode*/)
        {
            if (Player* l_Player = GetCaster()->ToPlayer())
            {
                l_Player->RemoveMovementImpairingAuras();
                l_Player->RemoveAurasByType(SPELL_AURA_MOD_STALKED);

                l_Player->CastSpell(l_Player, eSpells::StealthShapeshift, true);

                /// Glyph of Vanish
                if (AuraEffectPtr l_AurGlyphOfVanish = l_Player->GetAuraEffect(eSpells::GlyphOfVanish, EFFECT_0))
                    p_AurEff->GetBase()->SetDuration(p_AurEff->GetBase()->GetDuration() + l_AurGlyphOfVanish->GetAmount());

                /// Item - Rogue WoD PvP Assassination 4P Bonus and Item - Rogue WoD PvP Combat 4P Bonus
                if (l_Player->getLevel() == 100)
                {
                    /// Assasination
                    if (l_Player->HasAura(eSpells::WodPvpAssassination4pBonus))
                        l_Player->CastSpell(l_Player, eSpells::WodPvpAssassination4pBonusTrigger, true);
                    /// Combat
                    else if (l_Player->HasAura(eSpells::WodPvpCombat4pBonus))
                        l_Player->CastSpell(l_Player, eSpells::WodPvpCombat4pBonusTrigger, true);
                }
            }
        }

        void OnRemove(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Player* l_Player = GetCaster()->ToPlayer())
            {
                /// Stealth should be applied just after Vanish buff remove
                int32 l_CurrentStealthId = l_Player->HasAura(eSpells::Subterfuge) ? eSpells::StealthSubterfuge : eSpells::Stealth;

                l_Player->RemoveSpellCooldown(l_CurrentStealthId, true);

                if (!l_Player->HasAura(eSpells::GlyphOfDisappearance))
                    l_Player->CastSpell(l_Player, l_CurrentStealthId, true);
                else
                    l_Player->RemoveAura(eSpells::StealthShapeshift);
            }
        }

        void Register()
        {
            DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_rog_vanish_AuraScript::CalculateAmount, EFFECT_1, SPELL_AURA_DUMMY);
            OnEffectApply += AuraEffectApplyFn(spell_rog_vanish_AuraScript::OnApply, EFFECT_0, SPELL_AURA_MOD_STEALTH, AURA_EFFECT_HANDLE_REAL);
            AfterEffectRemove += AuraEffectRemoveFn(spell_rog_vanish_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_MOD_STEALTH, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_rog_vanish_AuraScript();
    }
};

/// Burst of Speed - 108212
class spell_rog_burst_of_speed: public SpellScriptLoader
{
public:
    spell_rog_burst_of_speed() : SpellScriptLoader("spell_rog_burst_of_speed") { }

    class spell_rog_burst_of_speed_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_rog_burst_of_speed_SpellScript);

        void HandleOnHit()
        {
            if (Unit* l_Caster = GetCaster())
                l_Caster->CastSpell(l_Caster, ROGUE_SPELL_BURST_OF_SPEED, true);
        }

        void Register()
        {
            OnHit += SpellHitFn(spell_rog_burst_of_speed_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_rog_burst_of_speed_SpellScript();
    }
};

/// Last Upadate 6.1.2
/// Call by Kidney Shot - 408
/// Internal Bleeding - 154904
class spell_rog_internal_bleeding: public SpellScriptLoader
{
    public:
        spell_rog_internal_bleeding() : SpellScriptLoader("spell_rog_internal_bleeding") { }

        class spell_rog_internal_bleeding_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_rog_internal_bleeding_SpellScript);

            void HandleOnHit()
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetHitUnit();

                if (l_Target == nullptr)
                    return;

                if (l_Caster->HasAura(ROGUE_SPELL_INTERNAL_BLEEDING_AURA))
                    l_Caster->CastSpell(l_Target, ROGUE_SPELL_INTERNAL_BLEEDING, true);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_rog_internal_bleeding_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_rog_internal_bleeding_SpellScript();
        }
};

/// Last Upadate 6.1.2
/// Internal Bleeding (damage) - 154953
class spell_rog_internal_bleeding_damage : public SpellScriptLoader
{
    public:
        spell_rog_internal_bleeding_damage() : SpellScriptLoader("spell_rog_internal_bleeding_damage") { }

        class spell_rog_internal_bleeding_damage_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_rog_internal_bleeding_damage_SpellScript);

            enum eSpells
            {
                InternalBleeding = 154953
            };

            uint8 m_NbComboPoint = 0;

            void HandleOnHit()
            {
                Unit* l_Caster = GetCaster();
               
                m_NbComboPoint = l_Caster->GetPower(Powers::POWER_COMBO_POINT);
            }

            void HandleAfterHit()
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetHitUnit();

                if (l_Target == nullptr)
                    return;

                if (AuraEffectPtr l_AurEff = l_Target->GetAuraEffect(eSpells::InternalBleeding, EFFECT_0, l_Caster->GetGUID()))
                    l_AurEff->SetAmount(l_AurEff->GetAmount() * m_NbComboPoint);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_rog_internal_bleeding_damage_SpellScript::HandleOnHit);
                AfterHit += SpellHitFn(spell_rog_internal_bleeding_damage_SpellScript::HandleAfterHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_rog_internal_bleeding_damage_SpellScript();
        }
};

/// Last Upadate 6.1.2
/// Fan of Knives - 51723
class spell_rog_fan_of_knives: public SpellScriptLoader
{
    public:
        spell_rog_fan_of_knives() : SpellScriptLoader("spell_rog_fan_of_knives") { }

        class spell_rog_fan_of_knives_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_rog_fan_of_knives_SpellScript);

            bool m_HasAlredyBenefitOfBonus = false;

            enum eSpells
            {
                EmpoweredFanofKnives = 157671
            };

            void HandleAfterHit()
            {
                Unit* l_Caster = GetCaster();

                if (m_HasAlredyBenefitOfBonus && !l_Caster->HasAura(eSpells::EmpoweredFanofKnives))
                    return;

                l_Caster->AddComboPoints(GetSpellInfo()->Effects[EFFECT_1].BasePoints);
                m_HasAlredyBenefitOfBonus = true;
            }

            void Register()
            {
                AfterHit += SpellHitFn(spell_rog_fan_of_knives_SpellScript::HandleAfterHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_rog_fan_of_knives_SpellScript();
        }
};

/// Call by Kidney Shot 408 - Eviscerate 2098 - Recuperate 73651 - Slice and Dice 5171 - Deadly Throw 26679 - Rupture 1943 - Envenom 32645
/// Relentless Strikes - 58423
class spell_rog_relentless_strikes : public SpellScriptLoader
{
    public:
        spell_rog_relentless_strikes() : SpellScriptLoader("spell_rog_relentless_strikes") { }

        class spell_rog_relentless_strikes_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_rog_relentless_strikes_SpellScript);

            enum eSpells
            {
                KidneyShot             = 408,
                RuptureDot             = 1943,
                Evicerate              = 2098,
                SliceAndDice           = 5171,
                ReltentlessStrikesProc = 14181,
                Envenom                = 32645,
                ReltentlessStrikesAura = 58423,
                Recuperate             = 73651,
                DeadlyThrow            = 26679,
                RevealingStrike        = 84617
            };

            void HandleDamage(SpellEffIndex effIndex)
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetHitUnit();

                if (l_Target == nullptr)
                    return;

                if (AuraEffectPtr l_AurEff = l_Target->GetAuraEffect(eSpells::RevealingStrike, EFFECT_2, l_Caster->GetGUID()))
                {
                    int32 l_Damage = GetHitDamage();
                    AddPct(l_Damage, l_AurEff->GetAmount());
                    SetHitDamage(l_Damage);
                }
            }

            void HandleAfterHit()
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetHitUnit();

                if (l_Target == nullptr)
                    return;

                if (AuraEffectPtr l_RevealingStrike = l_Target->GetAuraEffect(eSpells::RevealingStrike, EFFECT_2, l_Caster->GetGUID()))
                {
                    switch (GetSpellInfo()->Id)
                    {
                        case eSpells::KidneyShot:
                        case eSpells::Recuperate:
                        case eSpells::SliceAndDice:
                        case eSpells::RuptureDot:
                        {
                            if (AuraPtr l_ModSpell = l_Target->GetAura(GetSpellInfo()->Id, l_Caster->GetGUID()))
                            {
                                int32 l_Duration = l_ModSpell->GetDuration();
                                AddPct(l_Duration, l_RevealingStrike->GetAmount());
                                if (l_Duration >= 6 * IN_MILLISECONDS && l_Target->GetTypeId() == TYPEID_PLAYER) ///< Can't be more than 6s on pvp
                                    l_Duration = 6 * IN_MILLISECONDS;
                                l_ModSpell->SetDuration(l_Duration);
                            }
                            break;
                        }
                    }
                }
            }

            void Register()
            {
                switch (m_scriptSpellId)
                {
                    case eSpells::Evicerate:
                    case eSpells::Envenom:
                    case eSpells::DeadlyThrow:
                        OnEffectHitTarget += SpellEffectFn(spell_rog_relentless_strikes_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
                        break;
                    case eSpells::KidneyShot:
                    case eSpells::Recuperate:
                    case eSpells::SliceAndDice:
                    case eSpells::RuptureDot:
                        AfterHit += SpellHitFn(spell_rog_relentless_strikes_SpellScript::HandleAfterHit);
                        break;
                    default:
                        break;
                }
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_rog_relentless_strikes_SpellScript();
        }
};

/// Relentless Strikes proc - 14181 
class spell_rog_relentless_strikes_proc : public SpellScriptLoader
{
    public:
        spell_rog_relentless_strikes_proc() : SpellScriptLoader("spell_rog_relentless_strikes_proc") { }

        class spell_rog_relentless_strikes_proc_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_rog_relentless_strikes_proc_SpellScript);

            void HandleOnHit()
            {
                if (Unit* l_Caster = GetCaster())
                    l_Caster->EnergizeBySpell(l_Caster, GetSpellInfo()->Id, GetSpellInfo()->Effects[EFFECT_0].BasePoints, POWER_ENERGY);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_rog_relentless_strikes_proc_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_rog_relentless_strikes_proc_SpellScript();
        }
};

/// Combo Point Delayed - 139569
class spell_rog_combo_point_delayed: public SpellScriptLoader
{
public:
    spell_rog_combo_point_delayed() : SpellScriptLoader("spell_rog_combo_point_delayed") { }

    class spell_rog_combo_point_delayed_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_rog_combo_point_delayed_SpellScript);

        void HandleOnHit()
        {
            if (Player* l_Player = GetCaster()->ToPlayer())
                l_Player->EnergizeBySpell(l_Player, GetSpellInfo()->Id, GetSpellInfo()->Effects[EFFECT_0].BasePoints, POWER_COMBO_POINT);
        }

        void Register()
        {
            OnHit += SpellHitFn(spell_rog_combo_point_delayed_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_rog_combo_point_delayed_SpellScript();
    }
};

/// Eviscerate - 2098
class spell_rog_evicerate : public SpellScriptLoader
{
    public:
        spell_rog_evicerate() : SpellScriptLoader("spell_rog_evicerate") { }

        class spell_rog_evicerate_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_rog_evicerate_SpellScript);

            enum eSpells
            {
                Tier5Bonus2P = 37169
            };

            void HandleDamage(SpellEffIndex effIndex)
            {
                PreventHitDefaultEffect(effIndex);

                Unit* l_Caster = GetCaster();

                uint8 l_ComboPoint = l_Caster->GetPower(Powers::POWER_COMBO_POINT);
                int32 l_Damage = 0;

                if (l_ComboPoint)
                {
                    l_Damage += int32((l_Caster->GetTotalAttackPowerValue(WeaponAttackType::BaseAttack) * 0.577f) * 0.88f * l_ComboPoint);

                    /// Tier 5 Bonus 2 pieces
                    if (AuraEffectPtr l_Tier5Bonus2P = l_Caster->GetAuraEffect(eSpells::Tier5Bonus2P, EFFECT_0))
                        l_Damage += l_ComboPoint * l_Tier5Bonus2P->GetAmount();
                }

                l_Damage = l_Caster->SpellDamageBonusDone(GetHitUnit(), GetSpellInfo(), l_Damage, 0, SPELL_DIRECT_DAMAGE);
                l_Damage = GetHitUnit()->SpellDamageBonusTaken(l_Caster, GetSpellInfo(), l_Damage, SPELL_DIRECT_DAMAGE);

                SetHitDamage(l_Damage);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_rog_evicerate_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_rog_evicerate_SpellScript();
        }
};

/// Deadly Throw - 26679
class spell_rog_deadly_throw : public SpellScriptLoader
{
    public:
        spell_rog_deadly_throw() : SpellScriptLoader("spell_rog_deadly_throw") { }

        class spell_rog_deadly_throw_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_rog_deadly_throw_SpellScript);

            void HandleDamage(SpellEffIndex)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    uint8 l_ComboPoint = l_Caster->GetPower(Powers::POWER_COMBO_POINT);
                    int32 l_Damage = 0;

                    if (l_ComboPoint)
                    {
                        float l_Ap = l_Caster->GetTotalAttackPowerValue(WeaponAttackType::BaseAttack);

                        l_Damage += int32((l_Ap * 0.178f) * 1 * l_ComboPoint);
                    }

                    if (l_ComboPoint >= 5)
                    {
                        if (Unit* l_Target = GetHitUnit())
                            l_Caster->CastSpell(l_Target, ROGUE_SPELL_DEADLY_THROW_INTERRUPT, true);
                    }

                    SetHitDamage(l_Damage);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_rog_deadly_throw_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_rog_deadly_throw_SpellScript();
        }
};

class PlayerScript_ruthlessness : public PlayerScript
{
    public:
        PlayerScript_ruthlessness() : PlayerScript("PlayerScript_ruthlessness") { }

        enum eSpells
        {
            RuthlessnessEnergy  = 14181,
            T17Combat4P         = 165478,
            Deceit              = 166878,
            ShadowStrikesAura   = 166881,
            ShadowStrikesProc   = 170107
        };

        void OnModifyPower(Player* p_Player, Powers p_Power, int32 p_OldValue, int32& p_NewValue, bool p_Regen)
        {
            if (p_Regen || p_Power != POWER_COMBO_POINT || p_Player->getClass() != CLASS_ROGUE || !p_Player->HasAura(ROGUE_SPELL_RUTHLESSNESS))
                return;

            /// Get the power earn (if > 0 ) or consum (if < 0)
            int32 l_DiffVal = p_NewValue - p_OldValue;

            if (l_DiffVal < 0)
            {
                if (p_Player->HasAura(ROGUE_SPELL_RUTHLESSNESS))
                {
                    int32 l_Duration = sSpellMgr->GetSpellInfo(ROGUE_SPELL_RUTHLESSNESS)->Effects[EFFECT_2].BasePoints;
                    if (p_Player->HasSpellCooldown(ROGUE_SPELL_ADRENALINE_RUSH))
                        p_Player->ReduceSpellCooldown(ROGUE_SPELL_ADRENALINE_RUSH, -(l_Duration * l_DiffVal));
                    if (p_Player->HasSpellCooldown(ROGUE_SPELL_KILLING_SPREE))
                        p_Player->ReduceSpellCooldown(ROGUE_SPELL_KILLING_SPREE, -(l_Duration * l_DiffVal));
                    if (p_Player->HasSpellCooldown(ROGUE_SPELL_SPRINT))
                        p_Player->ReduceSpellCooldown(ROGUE_SPELL_SPRINT, -(l_Duration * l_DiffVal));

                    if (roll_chance_i(20))
                    {
                        p_NewValue += 1; ///< Restore 1 combo point
                        p_Player->CastSpell(p_Player, eSpells::RuthlessnessEnergy, true);  ///< Give 25 Energy
                    }
                }

                if (p_Player->HasAura(eSpells::T17Combat4P))
                {
                    /// Your finishing moves have a 4% chance per combo point to generate 5 combo points and cause your next Eviscerate to consume no Energy.
                    if (roll_chance_i(4 * -l_DiffVal))
                        p_Player->CastSpell(p_Player, eSpells::Deceit, true);
                }

                /// When Shadow Dance expires, your next finishing move refunds 5 combo points.
                if (p_Player->HasAura(eSpells::ShadowStrikesAura))
                {
                    p_Player->RemoveAura(eSpells::ShadowStrikesAura);
                    p_Player->CastSpell(p_Player, eSpells::ShadowStrikesProc, true);
                }
            }
        }
};

/// Feint - 1966
class spell_rog_feint : public SpellScriptLoader
{
public:
    spell_rog_feint() : SpellScriptLoader("spell_rog_feint") { }

    class spell_rog_feint_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_rog_feint_SpellScript);

        void HandleOnHit()
        {
            if (Player* l_Player = GetCaster()->ToPlayer())
            {
                if (l_Player->GetSpecializationId() == SPEC_ROGUE_SUBTLETY && l_Player->HasAura(ROGUE_SPELL_WOD_PVP_SUBTLETY_4P))
                {
                    l_Player->CastSpell(l_Player, ROGUE_SPELL_WOD_PVP_SUBTLETY_4P_EFFECT, true);
                }
            }
        }

        void Register()
        {
            OnHit += SpellHitFn(spell_rog_feint_SpellScript::HandleOnHit);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_rog_feint_SpellScript();
    }
};

/// Backstab - 53
class spell_rog_backstab : public SpellScriptLoader
{
public:
    spell_rog_backstab() : SpellScriptLoader("spell_rog_backstab") { }

    class spell_rog_backstab_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_rog_backstab_SpellScript);

        SpellCastResult CheckCast()
        {
            if (Unit* l_Caster = GetCaster())
            {
                if (Unit* l_Target = GetExplTargetUnit())
                {
                    if (l_Target->isInFront(l_Caster))
                        return SPELL_FAILED_NOT_BEHIND;
                }
            }

            return SPELL_CAST_OK;
        }

        void Register()
        {
            OnCheckCast += SpellCheckCastFn(spell_rog_backstab_SpellScript::CheckCast);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_rog_backstab_SpellScript();
    }
};

/// Bandit's Guile - 84654
/// Call by Sinister Strike - 1752
class spell_rog_bandits_guile : public SpellScriptLoader
{
    public:
        spell_rog_bandits_guile() : SpellScriptLoader("spell_rog_bandits_guile") { }

        class spell_rog_bandits_guile_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_rog_bandits_guile_SpellScript);

            enum eSpells
            {
                ShallowInsight  = 84745, //< Green
                ModerateInsight = 84746, //< Yellow
                DeepInsight     = 84747  //< Red
            };

            void HandleOnHit()
            {
                Unit* l_Caster = GetCaster();

                l_Caster->SetInsightCount(l_Caster->GetInsightCount() + 1);

                /// it takes a total of 4 strikes to get a proc, or a level up
                if (l_Caster->GetInsightCount() < 4)
                {
                    /// Each strike refresh the duration of Shallow Insight or Moderate Insight
                    /// but you can't refresh Deep Insight without starting from Shallow Insight.
                    if (AuraPtr l_ShallowInsight = l_Caster->GetAura(eSpells::ShallowInsight))
                        l_ShallowInsight->RefreshDuration();
                    else if (AuraPtr l_ModerateInsight = l_Caster->GetAura(eSpells::ModerateInsight))
                        l_ModerateInsight->RefreshDuration();
                }
                else
                {
                    l_Caster->SetInsightCount(0);

                    /// it takes 4 strikes to get Shallow Insight
                    /// then 4 strikes to get Moderate Insight
                    /// and then 4 strikes to get Deep Insight

                    if (AuraPtr l_ShallowInsight = l_Caster->GetAura(eSpells::ShallowInsight))
                    {
                        l_ShallowInsight->Remove();
                        l_Caster->CastSpell(l_Caster, eSpells::ModerateInsight, true);
                    }
                    else if (AuraPtr l_ModerateInsight = l_Caster->GetAura(eSpells::ModerateInsight))
                    {
                        l_ModerateInsight->Remove();
                        l_Caster->CastSpell(l_Caster, eSpells::DeepInsight, true);
                    }
                    else if (!l_Caster->HasAura(eSpells::DeepInsight))
                        l_Caster->CastSpell(l_Caster, eSpells::ShallowInsight, true);
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_rog_bandits_guile_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_rog_bandits_guile_SpellScript();
        }
};

/// Deep Insight - 84747
class spell_rog_deep_insight : public SpellScriptLoader
{
    public:
        spell_rog_deep_insight() : SpellScriptLoader("spell_rog_deep_insight") { }

        class spell_rog_deep_insight_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_rog_deep_insight_AuraScript);

            enum eSpells
            {
                EmpoweredBanditsGuile = 157581
            };

            void CalculateAmount(constAuraEffectPtr /*p_AuraEffect*/, int32& p_Amount, bool& /*p_CanBeRecalculated*/)
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster == nullptr)
                    return;

                if (AuraEffectPtr l_EmpoweredBanditsGuile = l_Caster->GetAuraEffect(eSpells::EmpoweredBanditsGuile, EFFECT_0))
                    p_Amount += l_EmpoweredBanditsGuile->GetAmount();
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_rog_deep_insight_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_rog_deep_insight_AuraScript();
        }
};

/// Glyph of Energy Flows - 159636
class spell_rog_glyph_of_energy_flows : public SpellScriptLoader
{
public:
    spell_rog_glyph_of_energy_flows() : SpellScriptLoader("spell_rog_glyph_of_energy_flows") { }

    class spell_rog_glyph_of_energy_flows_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_rog_glyph_of_energy_flows_AuraScript);

        enum eSpells
        {
            EVASION = 5277,
            GLYPH_OF_ENERGY_FLOWS_PROC = 159637
        };

        void OnProc(constAuraEffectPtr p_AurEff, ProcEventInfo& p_EventInfo)
        {
            PreventDefaultAction();

            if (Unit* l_Caster = GetCaster())
            {
                if (l_Caster->HasAura(eSpells::EVASION))
                    l_Caster->CastSpell(l_Caster, eSpells::GLYPH_OF_ENERGY_FLOWS_PROC, true);
            }
        }

        void Register()
        {
            OnEffectProc += AuraEffectProcFn(spell_rog_glyph_of_energy_flows_AuraScript::OnProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_rog_glyph_of_energy_flows_AuraScript();
    }
};

/// Find Weakness - 91023
class spell_rog_find_weakness : public SpellScriptLoader
{
    public:
        spell_rog_find_weakness() : SpellScriptLoader("spell_rog_find_weakness") { }

        class spell_rog_find_weakness_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_rog_find_weakness_AuraScript);

            enum eSpells
            {
                Ambush = 8676,
                Garrote = 703,
                CheapShot = 1833
            };

            void OnProc(constAuraEffectPtr p_AurEff, ProcEventInfo& p_EventInfo)
            {
                PreventDefaultAction();

                SpellInfo const* l_SpellInfo = p_EventInfo.GetDamageInfo()->GetSpellInfo();
                if (l_SpellInfo == nullptr)
                    return;

                int32 l_ProcSpells[] = { eSpells::Ambush, eSpells::Garrote, eSpells::CheapShot };

                if (Unit* l_Caster = GetCaster())
                {
                    if (Unit* l_Target = p_EventInfo.GetActionTarget())
                    {
                        if (l_Caster->GetGUID() == l_Target->GetGUID())
                            return;

                        for (int l_I = 0; l_I < sizeof(l_ProcSpells) / sizeof(int); l_I++)
                        {
                            if (l_SpellInfo->Id == l_ProcSpells[l_I])
                                l_Caster->AddAura(ROGUE_SPELL_FIND_WEAKNESS_PROC, l_Target);
                        }
                    }
                }
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_rog_find_weakness_AuraScript::OnProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_rog_find_weakness_AuraScript();
        }
};

/// Call by Leech Vitality - 116921
/// Glyph of Recovery - 146625
class spell_rog_glyph_of_recovery : public SpellScriptLoader
{
    public:
        spell_rog_glyph_of_recovery() : SpellScriptLoader("spell_rog_glyph_of_recovery") { }

        class spell_rog_glyph_of_recovery_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_rog_glyph_of_recovery_SpellScript);

            enum eSpells
            {
                GlyphofRecovery = 146625
            };

            void HandleOnHit()
            {
                Unit* l_Caster = GetCaster();

                if (!l_Caster->HasAura(eSpells::GlyphofRecovery))
                    return;

                if (AuraPtr l_Aura = l_Caster->GetAura(eSpells::GlyphofRecovery))
                    SetHitHeal(GetHitHeal() + CalculatePct(GetHitHeal(), l_Aura->GetEffect(EFFECT_0)->GetAmount()));
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_rog_glyph_of_recovery_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_rog_glyph_of_recovery_SpellScript();
        }
};

/// Last Update 6.1.2
/// Sinister Calling - 31220
class spell_rog_sinister_calling : public SpellScriptLoader
{
    public:
        spell_rog_sinister_calling() : SpellScriptLoader("spell_rog_sinister_calling") { }

        class spell_rog_sinister_calling_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_rog_sinister_calling_AuraScript);

            enum eSpells
            {
                Garrote = 703,
                Rupture = 1943,
                Hemorrhage = 16511,
                Ambush = 8676,
                Backstab = 53
            };

            void OnProc(constAuraEffectPtr p_AurEff, ProcEventInfo& p_ProcInfos)
            {
                if (p_ProcInfos.GetDamageInfo() == nullptr)
                    return;

                Unit* l_Caster = GetCaster();
                Unit* l_Target = p_ProcInfos.GetDamageInfo()->GetVictim();

                if (l_Caster == nullptr || l_Target == nullptr)
                    return;

                if (p_ProcInfos.GetDamageInfo()->GetSpellInfo() == nullptr)
                    return;

                if (p_ProcInfos.GetDamageInfo()->GetSpellInfo()->Id != eSpells::Backstab && p_ProcInfos.GetDamageInfo()->GetSpellInfo()->Id != eSpells::Ambush)
                    return;

                if (!(p_ProcInfos.GetHitMask() & PROC_EX_INTERNAL_MULTISTRIKE))
                    return;

                std::list<uint32> l_DotBleedList;
                l_DotBleedList.push_back(eSpells::Garrote);
                l_DotBleedList.push_back(eSpells::Rupture);
                l_DotBleedList.push_back(eSpells::Hemorrhage);

                for (std::list<uint32>::iterator it = l_DotBleedList.begin(); it != l_DotBleedList.end(); ++it)
                {
                    if (AuraPtr l_BleedAura = l_Target->GetAura(*it, l_Caster->GetGUID()))
                    {
                        switch (*it)
                        {
                        case eSpells::Garrote:
                        case eSpells::Rupture:
                            if (AuraEffectPtr l_BleedEffect = l_BleedAura->GetEffect(EFFECT_0))
                                l_BleedEffect->HandlePeriodicDamageAurasTick(l_Target, l_Caster);
                            break;
                        case eSpells::Hemorrhage:
                            if (AuraEffectPtr l_BleedEffect = l_BleedAura->GetEffect(EFFECT_3))
                                l_BleedEffect->HandlePeriodicDamageAurasTick(l_Target, l_Caster);
                            break;
                        default:
                                break;
                        }
                        l_BleedAura->SetDuration(l_BleedAura->GetDuration() - p_AurEff->GetAmount() * IN_MILLISECONDS);
                    }
                }
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_rog_sinister_calling_AuraScript::OnProc, EFFECT_2, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_rog_sinister_calling_AuraScript();
        }
};

/// Increased 40% if a dagger is equipped
/// Call by Ambush - 8676, Hemorrhage - 16511
class spell_rog_dagger_bonus : public SpellScriptLoader
{
    public:
        spell_rog_dagger_bonus() : SpellScriptLoader("spell_rog_dagger_bonus") { }

        class spell_rog_dagger_bonus_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_rog_dagger_bonus_SpellScript);

            void HandleDamage(SpellEffIndex)
            {
                Player* l_Player = GetCaster()->ToPlayer();

                if (l_Player == nullptr)
                    return;

                Item* l_MainItem = l_Player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND);
                Item* l_OffItem = l_Player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);

                if (((l_MainItem && l_MainItem->GetTemplate()->SubClass == ITEM_SUBCLASS_WEAPON_DAGGER) || (l_OffItem && l_OffItem->GetTemplate()->SubClass == ITEM_SUBCLASS_WEAPON_DAGGER)))
                {
                    int32 l_Damage = GetHitDamage();
                    SetHitDamage(l_Damage + CalculatePct(l_Damage, 40));
                }

            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_rog_dagger_bonus_SpellScript::HandleDamage, EFFECT_1, SPELL_EFFECT_WEAPON_PERCENT_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_rog_dagger_bonus_SpellScript();
        }
};

/// last update : 6.1.2 19802
/// Glyph of Detection - 125044
class spell_rog_gyph_of_detection : public SpellScriptLoader
{
    public:
        spell_rog_gyph_of_detection() : SpellScriptLoader("spell_rog_gyph_of_detection") { }

        class spell_rog_gyph_of_detection_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_rog_gyph_of_detection_AuraScript);

            enum eSpells
            {
                Detection = 56814
            };

            void OnApply(constAuraEffectPtr /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                Player* l_Player = GetTarget()->ToPlayer();

                if (l_Player == nullptr)
                    return;

                if (!l_Player->HasSpell(eSpells::Detection))
                    l_Player->learnSpell(eSpells::Detection, false);
            }

            void OnRemove(constAuraEffectPtr /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                Player* l_Player = GetTarget()->ToPlayer();

                if (l_Player == nullptr)
                    return;

                if (l_Player->HasSpell(eSpells::Detection))
                    l_Player->removeSpell(eSpells::Detection, false);
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_rog_gyph_of_detection_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_rog_gyph_of_detection_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_rog_gyph_of_detection_AuraScript();
        }
};

/// Item - Rogue T17 Assassination 2P Bonus - 165516
class spell_rog_item_t17_assassination_2p_bonus : public SpellScriptLoader
{
    public:
        spell_rog_item_t17_assassination_2p_bonus() : SpellScriptLoader("spell_rog_item_t17_assassination_2p_bonus") { }

        class spell_rog_item_t17_assassination_2p_bonus_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_rog_item_t17_assassination_2p_bonus_AuraScript);

            enum eSpells
            {
                Dispatch            = 111240,
                DispatchOverrided   = 157957,
                MutilateRightHand   = 5374,
                MutilateLeftHand    = 27576,
                MutilateEnergizer   = 166885
            };

            void OnProc(constAuraEffectPtr p_AurEff, ProcEventInfo& p_EventInfo)
            {
                PreventDefaultAction();

                Unit* l_Caster = GetCaster();
                if (!l_Caster)
                    return;

                SpellInfo const* l_ProcSpell = p_EventInfo.GetDamageInfo()->GetSpellInfo();
                if (!l_ProcSpell)
                    return;

                uint32 const l_AllowedSpells[4] = { eSpells::Dispatch, eSpells::DispatchOverrided, eSpells::MutilateRightHand, eSpells::MutilateLeftHand };

                bool l_Found = false;
                for (uint8 l_I = 0; l_I < 4; ++l_I)
                {
                    if (l_ProcSpell->Id == l_AllowedSpells[l_I])
                    {
                        l_Found = true;
                        break;
                    }
                }

                if (!l_Found)
                    return;

                if (!(p_EventInfo.GetHitMask() & ProcFlagsExLegacy::PROC_EX_CRITICAL_HIT))
                    return;

                /// Mutilate and Dispatch critical strikes restore 7 energy.
                l_Caster->CastSpell(l_Caster, eSpells::MutilateEnergizer, true);
            }

            void Register() override
            {
                OnEffectProc += AuraEffectProcFn(spell_rog_item_t17_assassination_2p_bonus_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_rog_item_t17_assassination_2p_bonus_AuraScript();
        }
};

/// Item - Rogue T17 Subtlety 2P Bonus - 165482
class spell_rog_item_t17_subtlety_2p_bonus : public SpellScriptLoader
{
    public:
        spell_rog_item_t17_subtlety_2p_bonus() : SpellScriptLoader("spell_rog_item_t17_subtlety_2p_bonus") { }

        class spell_rog_item_t17_subtlety_2p_bonus_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_rog_item_t17_subtlety_2p_bonus_AuraScript);

            enum eSpells
            {
                ShadowDance = 51713,
                QuickBlades = 165509
            };

            void OnProc(constAuraEffectPtr p_AurEff, ProcEventInfo& p_EventInfo)
            {
                PreventDefaultAction();

                Unit* l_Caster = GetCaster();
                if (!l_Caster)
                    return;

                SpellInfo const* l_ProcSpell = p_EventInfo.GetDamageInfo()->GetSpellInfo();
                if (!l_ProcSpell || l_ProcSpell->Id != eSpells::ShadowDance)
                    return;

                /// When you activate Shadow Dance, you gain 60 Energy.
                l_Caster->CastSpell(l_Caster, eSpells::QuickBlades, true);
            }

            void Register() override
            {
                OnEffectProc += AuraEffectProcFn(spell_rog_item_t17_subtlety_2p_bonus_AuraScript::OnProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_rog_item_t17_subtlety_2p_bonus_AuraScript();
        }
};

/// Called by Shadow Dance - 51713
/// Item - Rogue T17 Subtlety 4P Bonus - 165514
class spell_rog_item_t17_subtlety_4p_bonus : public SpellScriptLoader
{
    public:
        spell_rog_item_t17_subtlety_4p_bonus() : SpellScriptLoader("spell_rog_item_t17_subtlety_4p_bonus") { }

        class spell_rog_item_t17_subtlety_4p_bonus_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_rog_item_t17_subtlety_4p_bonus_AuraScript);

            enum eSpells
            {
                ShadowStrikes = 166881
            };

            void AfterRemove(constAuraEffectPtr p_AurEff, AuraEffectHandleModes p_Mode)
            {
                if (Unit* l_Caster = GetCaster())
                    l_Caster->CastSpell(l_Caster, eSpells::ShadowStrikes, true);
            }

            void Register() override
            {
                AfterEffectRemove += AuraEffectApplyFn(spell_rog_item_t17_subtlety_4p_bonus_AuraScript::AfterRemove, EFFECT_0, SPELL_AURA_MOD_IGNORE_SHAPESHIFT, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_rog_item_t17_subtlety_4p_bonus_AuraScript();
        }
};

void AddSC_rogue_spell_scripts()
{
    new spell_rog_gyph_of_detection();
    new spell_rog_dagger_bonus();
    new spell_rog_sinister_calling();
    new spell_rog_glyph_of_recovery();
    new spell_rog_anticipation();
    new spell_rog_venom_rush();
    new spell_rog_death_from_above_return();
    new spell_rog_death_from_above();
    new spell_rog_shadow_reflection_proc();
    new spell_rog_shadow_reflection();
    new spell_rog_enhanced_vendetta();
    new spell_rog_deadly_throw();
    new spell_rog_evicerate();
    new spell_rog_envenom();
    new spell_rog_combo_point_delayed();
    new spell_rog_relentless_strikes_proc();
    new spell_rog_relentless_strikes();
    new spell_rog_fan_of_knives();
    new spell_rog_internal_bleeding();
    new spell_rog_burst_of_speed();
    new spell_rog_killing_spree();
    new spell_rog_glyph_of_decoy();
    new spell_rog_shuriken_toss();
    new spell_rog_marked_for_death();
    new spell_rog_cloak_and_dagger();
    new spell_rog_cheat_death();
    new spell_rog_blade_flurry();
    new spell_rog_cloak_of_shadows();
    new spell_rog_combat_readiness();
    new spell_rog_nerve_strike();
    new spell_rog_sanguinary_vein();
    new spell_rog_hemorrhage();
    new spell_rog_cut_to_the_chase();
    new spell_rog_venomous_wounds();
    new spell_rog_shroud_of_concealment();
    new spell_rog_crimson_tempest();
    new spell_rog_slice_and_dice();
    new spell_rog_deadly_poison_instant_damage();
    new spell_rog_shiv();
    new spell_rog_recuperate();
    new spell_rog_preparation();
    new spell_rog_deadly_poison();
    new spell_rog_shadowstep();
    new spell_rog_stealth();
    new spell_rog_vanish();
    new spell_rog_internal_bleeding_damage();
    new spell_rog_feint();
    new spell_rog_backstab();
    new spell_rog_bandits_guile();
    new spell_rog_deep_insight();
    new spell_rog_glyph_of_energy_flows();
    new spell_rog_find_weakness();
    new spell_rog_item_t17_assassination_2p_bonus();
    new spell_rog_item_t17_subtlety_2p_bonus();
    new spell_rog_item_t17_subtlety_4p_bonus();

    /// Player Scripts
    new PlayerScript_ruthlessness();
}