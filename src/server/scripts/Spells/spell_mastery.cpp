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
 * Scripts for spells with MASTERY.
 * Ordered alphabetically using scriptname.
 * Scriptnames of files in this file should be prefixed with "spell_mastery_".
 */

#include "ScriptMgr.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"

enum MasterySpells
{
    MASTERY_SPELL_LIGHTNING_BOLT        = 45284,
    MASTERY_SPELL_CHAIN_LIGHTNING       = 45297,
    MASTERY_SPELL_LAVA_BURST            = 77451,
    MASTERY_SPELL_ELEMENTAL_BLAST       = 120588,
    MASTERY_SPELL_HAND_OF_LIGHT         = 96172,
    MASTERY_SPELL_IGNITE_AURA           = 12654,
    MASTERY_SPELL_BLOOD_SHIELD          = 77535,
    MASTERY_SPELL_DISCIPLINE_SHIELD     = 77484,
    SPELL_DK_SCENT_OF_BLOOD             = 50421,
    SPELL_MAGE_MASTERY_ICICLES          = 76613,
    SPELL_MAGE_ICICLE_DAMAGE            = 148022,
    SPELL_MAGE_ICICLE_PERIODIC_TRIGGER  = 148023,
    SPELL_PRIEST_ECHO_OF_LIGHT          = 77489,
    SPELL_WARRIOR_WEAPONS_MASTER        = 76838,
    SPELL_WARLOCK_METAMORPHIS           = 103958,
    SPELL_WARLOCK_MASTER_DEMONOLOGIST   = 77219,
    SPELL_PRIEST_MENTAL_ANGUISH         = 77486,
    MASTERY_SPELL_IGNITE                = 12846
};

enum MoltenEarthSpells
{
    MoltenEarthDamage   = 170379,
    MoltenEarthPeriodic = 170377,
    MoltenEarthAura     = 170374
};

/// Molten Earth - 170374
class spell_mastery_molten_earth : public SpellScriptLoader
{
    public:
        spell_mastery_molten_earth() : SpellScriptLoader("spell_mastery_molten_earth") { }

        class spell_mastery_molten_earth_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mastery_molten_earth_AuraScript);

            void OnProc(constAuraEffectPtr, ProcEventInfo& p_EventInfo)
            {
                PreventDefaultAction();

                if (p_EventInfo.GetDamageInfo()->GetSpellInfo()->Id == MoltenEarthSpells::MoltenEarthDamage)
                    return;

                Unit* l_Caster = GetCaster();
                Unit* l_Target = p_EventInfo.GetDamageInfo()->GetVictim();
                if (l_Caster == nullptr || l_Target == nullptr)
                    return;

                /// Assuming it's a 33.33% proc chance
                if (!roll_chance_f(33.33f))
                    return;

                if (l_Target->HasAura(MoltenEarthSpells::MoltenEarthPeriodic, l_Caster->GetGUID()))
                {
                    if (AuraPtr l_PeriodicAura = l_Target->GetAura(MoltenEarthSpells::MoltenEarthPeriodic, l_Caster->GetGUID()))
                        l_PeriodicAura->RefreshDuration();
                }
                else
                    l_Caster->AddAura(MoltenEarthSpells::MoltenEarthPeriodic, l_Target);
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_mastery_molten_earth_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_mastery_molten_earth_AuraScript();
        }
};

/// Molten Earth Periodic - 170377
class spell_mastery_molten_earth_periodic: public SpellScriptLoader
{
    public:
        spell_mastery_molten_earth_periodic() : SpellScriptLoader("spell_mastery_molten_earth_periodic") { }

        class spell_mastery_molten_earth_periodic_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mastery_molten_earth_periodic_AuraScript);

            void HandleEffectPeriodic(constAuraEffectPtr)
            {
                PreventDefaultAction();

                if (GetOwner() == nullptr)
                    return;

                if (Unit* l_Owner = GetOwner()->ToUnit())
                {
                    if (Unit* l_Caster = GetCaster())
                    {
                        uint8 l_Count = irand(1, 2);

                        for (uint8 l_I = 0; l_I < l_Count; l_I++)
                            l_Caster->CastSpell(l_Owner, MoltenEarthSpells::MoltenEarthDamage, true);
                    }
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_mastery_molten_earth_periodic_AuraScript::HandleEffectPeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_mastery_molten_earth_periodic_AuraScript();
        }
};

/// Mastery: Sniper Training - 76659
class spell_mastery_sniper_training : public SpellScriptLoader
{
    public:
        spell_mastery_sniper_training() : SpellScriptLoader("spell_mastery_sniper_training") { }

        enum Masteries
        {
            RecentlyMoved       = 168809,
            SniperTrainingAura  = 168811
        };

        class spell_mastery_sniper_training_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mastery_sniper_training_AuraScript);

            void OnUpdate(uint32, AuraEffectPtr)
            {
                if (!GetUnitOwner())
                    return;

                if (Player* l_Player = GetUnitOwner()->ToPlayer())
                {
                    if (AuraPtr l_Aura = l_Player->GetAura(Masteries::SniperTrainingAura))
                    {
                        if (l_Player->isMoving() && l_Aura->GetDuration() == -1)
                        {
                            l_Aura->SetDuration(6000);
                            l_Aura->SetMaxDuration(6000);
                        }
                        else if (!l_Player->isMoving() && l_Aura->GetDuration() != -1 && !l_Player->HasAura(Masteries::RecentlyMoved))
                            l_Player->CastSpell(l_Player, Masteries::RecentlyMoved, true);
                    }
                    else if (!l_Player->isMoving() && !l_Player->HasAura(Masteries::RecentlyMoved))
                        l_Player->CastSpell(l_Player, Masteries::RecentlyMoved, true);
                }
            }

            void OnRemove(constAuraEffectPtr, AuraEffectHandleModes)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    l_Caster->RemoveAura(Masteries::RecentlyMoved);
                    l_Caster->RemoveAura(Masteries::SniperTrainingAura);
                }
            }

            void Register()
            {
                OnEffectUpdate += AuraEffectUpdateFn(spell_mastery_sniper_training_AuraScript::OnUpdate, EFFECT_2, SPELL_AURA_DUMMY);
                OnEffectRemove += AuraEffectRemoveFn(spell_mastery_sniper_training_AuraScript::OnRemove, EFFECT_2, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_mastery_sniper_training_AuraScript();
        }
};

/// last update : 6.1.2 19802
/// Sniper Training: Recently Moved - 168809
class spell_mastery_recently_moved : public SpellScriptLoader
{
    public:
        spell_mastery_recently_moved() : SpellScriptLoader("spell_mastery_recently_moved") { }

        enum Masteries
        {
            SniperTrainingAura = 168811
        };

        class spell_mastery_recently_moved_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mastery_recently_moved_AuraScript);

            void OnRemove(constAuraEffectPtr, AuraEffectHandleModes)
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster == nullptr)
                    return;

                AuraRemoveMode l_RemoveMode = GetTargetApplication()->GetRemoveMode();
                if (l_RemoveMode == AuraRemoveMode::AURA_REMOVE_BY_EXPIRE)
                    l_Caster->CastSpell(l_Caster, Masteries::SniperTrainingAura, true);
            }

            void Register()
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_mastery_recently_moved_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_mastery_recently_moved_AuraScript();
        }
};

/// Sniper Training - 168811
class spell_mastery_sniper_training_aura : public SpellScriptLoader
{
    public:
        spell_mastery_sniper_training_aura() : SpellScriptLoader("spell_mastery_sniper_training_aura") { }

        class spell_mastery_sniper_training_aura_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mastery_sniper_training_aura_AuraScript);

            void OnUpdate(uint32, AuraEffectPtr p_AurEff)
            {
                if (!GetUnitOwner())
                    return;

                if (Player* l_Player = GetUnitOwner()->ToPlayer())
                {
                    if (l_Player->GetSpecializationId(l_Player->GetActiveSpec()) != SpecIndex::SPEC_HUNTER_MARKSMANSHIP)
                        p_AurEff->GetBase()->Remove();

                    float l_Mastery = l_Player->GetFloatValue(EPlayerFields::PLAYER_FIELD_MASTERY) * 0.625f;
                    int32 l_BasePoints = l_Mastery;

                    if (AuraPtr l_Aura = p_AurEff->GetBase())
                    {
                        for (uint8 l_I = 0; l_I < 4; ++l_I)
                        {
                            if (AuraEffectPtr l_Effect = l_Aura->GetEffect(l_I))
                                l_Effect->ChangeAmount(l_BasePoints, true, true);
                        }
                    }
                }
            }

            void Register()
            {
                OnEffectUpdate += AuraEffectUpdateFn(spell_mastery_sniper_training_aura_AuraScript::OnUpdate, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_mastery_sniper_training_aura_AuraScript();
        }
};

/// last update : 6.1.2
/// Mastery: Echo of Light - 77485
class spell_mastery_echo_of_light: public SpellScriptLoader
{
    public:
        spell_mastery_echo_of_light() : SpellScriptLoader("spell_mastery_echo_of_light") { }

        class spell_mastery_echo_of_light_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mastery_echo_of_light_AuraScript);

            void OnProc(constAuraEffectPtr aurEff, ProcEventInfo& eventInfo)
            {
                PreventDefaultAction();

                if (!GetCaster())
                    return;

                if (!eventInfo.GetHealInfo() || !eventInfo.GetHealInfo()->GetHeal())
                    return;

                Unit* unitTarget = eventInfo.GetActionTarget();
                Player* plr = GetCaster()->ToPlayer();
                if (!unitTarget || !plr)
                    return;

                float Mastery = plr->GetFloatValue(PLAYER_FIELD_MASTERY) * 1.25f / 100.0f;
                int32 bp = (Mastery * eventInfo.GetHealInfo()->GetHeal()) / 2;

                bp += unitTarget->GetRemainingPeriodicAmount(plr->GetGUID(), SPELL_PRIEST_ECHO_OF_LIGHT, SPELL_AURA_PERIODIC_HEAL);
                plr->CastCustomSpell(unitTarget, SPELL_PRIEST_ECHO_OF_LIGHT, &bp, NULL, NULL, true);
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_mastery_echo_of_light_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_mastery_echo_of_light_AuraScript();
        }
};
const int IcicleAuras[5] = { 148012, 148013, 148014, 148015, 148016 };
const int IcicleHits[5] = { 148017, 148018, 148019, 148020, 148021 };

/// last update : 6.1.2 19802
/// Mastery: Icicles - 76613
class spell_mastery_icicles_proc : public SpellScriptLoader
{
    public:
        spell_mastery_icicles_proc() : SpellScriptLoader("spell_mastery_icicles_proc") { }

        class spell_mastery_icicles_proc_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mastery_icicles_proc_AuraScript);

            enum eSpells
            {
                FrosBolt        = 116,
                FrostfireBolt   = 44614
            };

            void OnProc(constAuraEffectPtr p_AurEff, ProcEventInfo& p_EventInfo)
            {
                SpellInfo const* l_SpellInfo = p_EventInfo.GetDamageInfo()->GetSpellInfo();

                if (l_SpellInfo == nullptr)
                    return;

                if (l_SpellInfo->Id != eSpells::FrosBolt && l_SpellInfo->Id != eSpells::FrostfireBolt)
                    return;

                Unit* l_Target = p_EventInfo.GetDamageInfo()->GetVictim();
                Unit* l_Caster = p_EventInfo.GetDamageInfo()->GetAttacker();

                if (l_Target == nullptr || l_Caster == nullptr)
                    return;

                Player* l_Player = l_Caster->ToPlayer();

                if (l_Player == nullptr)
                    return;

                /// Calculate damage
                int32 l_HitDamage = p_EventInfo.GetDamageInfo()->GetDamage() + p_EventInfo.GetDamageInfo()->GetAbsorb();

                /// if l_HitDamage == 0 we have a miss, so we need to except this variant
                if (l_HitDamage != 0)
                {
                    l_HitDamage *= (l_Player->GetFloatValue(PLAYER_FIELD_MASTERY) * 2.0f) / 100.0f;

                    /// Prevent huge hits on player after hitting low level creatures
                    if (l_Player->getLevel() > l_Target->getLevel())
                        l_HitDamage = std::min(uint32(l_HitDamage), l_Target->GetMaxHealth());

                    /// We need to get the first free icicle slot
                    int8 l_IcicleFreeSlot = -1; ///< -1 means no free slot
                    for (int8 l_I = 0; l_I < 5; ++l_I)
                    {
                        if (!l_Player->HasAura(IcicleAuras[l_I]))
                        {
                            l_IcicleFreeSlot = l_I;
                            break;
                        }
                    }

                    switch (l_IcicleFreeSlot)
                    {
                    case -1:
                    {
                        // We need to find the icicle with the smallest duration.
                        int8 l_SmallestIcicle = 0;
                        int32 l_MinDuration = 0xFFFFFF;
                        for (int8 i = 0; i < 5; i++)
                        {
                            if (AuraPtr l_TmpCurrentAura = l_Player->GetAura(IcicleAuras[i]))
                            {
                                if (l_MinDuration > l_TmpCurrentAura->GetDuration())
                                {
                                    l_MinDuration = l_TmpCurrentAura->GetDuration();
                                    l_SmallestIcicle = i;
                                }
                            }
                        }

                        /// Launch the icicle with the smallest duration
                        if (AuraEffectPtr l_CurrentIcicleAuraEffect = l_Player->GetAuraEffect(IcicleAuras[l_SmallestIcicle], EFFECT_0))
                        {
                            int32 l_BasePoints = l_CurrentIcicleAuraEffect->GetAmount();
                            l_Player->CastSpell(l_Target, IcicleHits[l_SmallestIcicle], true);
                            l_Player->CastCustomSpell(l_Target, SPELL_MAGE_ICICLE_DAMAGE, &l_BasePoints, NULL, NULL, true);
                            l_Player->RemoveAura(IcicleAuras[l_SmallestIcicle]);
                        }

                        l_IcicleFreeSlot = l_SmallestIcicle;
                        /// No break because we'll add the icicle in the next case
                    }
                    case 0:
                    case 1:
                    case 2:
                    case 3:
                    case 4:
                    {
                        if (AuraPtr l_CurrentIcicleAura = l_Player->AddAura(IcicleAuras[l_IcicleFreeSlot], l_Player))
                        {
                            if (AuraEffectPtr l_Effect = l_CurrentIcicleAura->GetEffect(EFFECT_0))
                                l_Effect->SetAmount(l_HitDamage);
                        }

                        break;
                    }
                    }
                }
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_mastery_icicles_proc_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_mastery_icicles_proc_AuraScript();
        }
};

/// Ice Lance - 30455
class spell_mastery_icicles_trigger : public SpellScriptLoader
{
    public:
        spell_mastery_icicles_trigger() : SpellScriptLoader("spell_mastery_icicles_trigger") { }

        class spell_mastery_icicles_trigger_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mastery_icicles_trigger_SpellScript);

            void HandleAfterHit()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (Unit* l_HitUnit = GetHitUnit())
                    {
                        if (l_HitUnit->isAlive())
                        {
                            l_Caster->SetIciclesTarget(l_HitUnit->GetGUID());
                            l_Caster->CastSpell(l_Caster, SPELL_MAGE_ICICLE_PERIODIC_TRIGGER, true);
                        }
                    }
                }
            }

            void Register()
            {
                AfterHit += SpellHitFn(spell_mastery_icicles_trigger_SpellScript::HandleAfterHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_mastery_icicles_trigger_SpellScript();
        }
};

/// Icicles (periodic) - 148023
class spell_mastery_icicles_periodic : public SpellScriptLoader
{
    public:
        spell_mastery_icicles_periodic() : SpellScriptLoader("spell_mastery_icicles_periodic") { }

        class spell_mastery_icicles_periodic_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mastery_icicles_periodic_AuraScript);

            uint32 m_Icicles[5];
            int32 m_IcicleCount;

            void OnApply(constAuraEffectPtr /*l_AurEff*/, AuraEffectHandleModes /*l_Mode*/)
            {
                m_IcicleCount = 0;
                if (Unit* l_Caster = GetCaster())
                {
                    for (uint32 l_I = 0; l_I < 5; ++l_I)
                    {
                        if (AuraPtr l_Icicle = l_Caster->GetAura(IcicleAuras[l_I]))
                            m_Icicles[m_IcicleCount++] = IcicleAuras[l_I];
                    }
                }
            }

            void OnTick(constAuraEffectPtr aurEff)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (AuraEffectPtr l_Aura = l_Caster->GetAuraEffect(GetSpellInfo()->Id, EFFECT_0))
                    {                        
                        // Maybe not the good target selection ...
                        if (Unit* l_Target = ObjectAccessor::FindUnit(l_Caster->GetIciclesTarget()))
                        {
                            if (l_Target->isAlive())
                            {
                                int32 l_Amount = l_Aura->GetAmount();
                                if (AuraPtr l_CurrentIcicleAura = l_Caster->GetAura(m_Icicles[l_Amount]))
                                {
                                    int32 l_BasePoints = l_CurrentIcicleAura->GetEffect(0)->GetAmount();

                                    l_Caster->CastSpell(l_Target, IcicleHits[l_Amount], true);
                                    l_Caster->CastCustomSpell(l_Target, SPELL_MAGE_ICICLE_DAMAGE, &l_BasePoints, NULL, NULL, true);
                                    l_Caster->RemoveAura(IcicleAuras[l_Amount]);
                                }

                                if (++l_Amount >= m_IcicleCount)
                                    l_Caster->RemoveAura(l_Aura->GetBase());
                                else
                                    l_Aura->SetAmount(l_Amount);
                            }
                            else
                                l_Caster->RemoveAura(l_Aura->GetBase());
                        }
                    }
                }
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_mastery_icicles_periodic_AuraScript::OnApply, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_mastery_icicles_periodic_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_mastery_icicles_periodic_AuraScript();
        }
};

/// last update : 6.1.2 19802
// Called by 45470 - Death Strike (Heal)
// 77513 - Mastery : Blood Shield
class spell_mastery_blood_shield: public SpellScriptLoader
{
    public:
        spell_mastery_blood_shield() : SpellScriptLoader("spell_mastery_blood_shield") { }

        class spell_mastery_blood_shield_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mastery_blood_shield_SpellScript);

            enum eSpells
            {
                BloodPresence = 48263,
                MasteryAura = 77513
            };

            void HandleAfterHit()
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetHitUnit();

                if (l_Target == nullptr)
                    return;

                if (!l_Caster->HasAura(eSpells::BloodPresence))
                    return;

                if (l_Caster->GetGUID() != l_Target->GetGUID()) ///< Only when you heal yourself
                    return;

                if (AuraEffectPtr l_MasteryBloodShield = l_Caster->GetAuraEffect(eSpells::MasteryAura, EFFECT_0))
                {
                    float l_Mastery = l_MasteryBloodShield->GetAmount();

                    int32 l_Bp = -int32(GetHitDamage() * (l_Mastery / 100));

                    if (AuraEffectPtr l_ActualBloodShield = l_Caster->GetAuraEffect(MASTERY_SPELL_BLOOD_SHIELD, EFFECT_0))
                        l_Bp += l_ActualBloodShield->GetAmount();

                    l_Bp = std::min(uint32(l_Bp), l_Caster->GetMaxHealth());

                    l_Caster->CastCustomSpell(l_Caster, MASTERY_SPELL_BLOOD_SHIELD, &l_Bp, NULL, NULL, true);
                }
            }

            void Register()
            {
                AfterHit += SpellHitFn(spell_mastery_blood_shield_SpellScript::HandleAfterHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_mastery_blood_shield_SpellScript();
        }
};

// Called by 133 - Fireball, 44614 - Frostfire Bolt, 108853 - Inferno Blast, 2948 - Scorch, 11366 - Pyroblast and 153564 - Meteor
// 12846 - Mastery : Ignite
class spell_mastery_ignite: public SpellScriptLoader
{
    public:
        spell_mastery_ignite() : SpellScriptLoader("spell_mastery_ignite") { }

        class spell_mastery_ignite_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mastery_ignite_SpellScript);

            enum eSpell
            {
                GlyphOfIgnite = 61205,
                GlyphOfIgniteEffect = 182287
            };

            void HandleAfterHit()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (Unit* l_Target = GetHitUnit())
                    {
                        if (l_Caster->GetTypeId() == TYPEID_PLAYER && l_Caster->HasAura(MASTERY_SPELL_IGNITE) && l_Caster->getLevel() >= 80)
                        {
                            const SpellInfo *l_SpellInfo = sSpellMgr->GetSpellInfo(MASTERY_SPELL_IGNITE_AURA);
                            if (GetSpellInfo()->Id != MASTERY_SPELL_IGNITE_AURA && l_SpellInfo != nullptr)
                            {
                                float l_Value = l_Caster->GetFloatValue(PLAYER_FIELD_MASTERY) * 1.5f;

                                int32 l_Bp = GetHitDamage() + GetAbsorbedDamage();
                                if (l_Bp)
                                {
                                    l_Bp = int32(CalculatePct(l_Bp, l_Value));

                                    if (l_SpellInfo->Effects[EFFECT_0].Amplitude > 0)
                                        l_Bp = l_Bp / (l_SpellInfo->GetMaxDuration() / l_SpellInfo->Effects[EFFECT_0].Amplitude);
                                    
                                    if (AuraPtr l_PreviousIgnite = l_Target->GetAura(MASTERY_SPELL_IGNITE_AURA, l_Caster->GetGUID()))
                                    {
                                        if (AuraEffectPtr l_Effect = l_PreviousIgnite->GetEffect(EFFECT_0))
                                        {
                                            if (uint32 l_Amplitude = l_Effect->GetAmplitude())
                                            {
                                                if (uint32 l_Stacks = l_PreviousIgnite->GetDuration() / l_Amplitude)
                                                    l_Bp += l_Target->GetRemainingPeriodicAmount(l_Caster->GetGUID(), MASTERY_SPELL_IGNITE_AURA, SPELL_AURA_PERIODIC_DAMAGE);
                                            }
                                        }
                                    }

                                    l_Caster->CastCustomSpell(l_Target, MASTERY_SPELL_IGNITE_AURA, &l_Bp, NULL, NULL, true);

                                    /// Glyph of Ignite
                                    if (l_Caster->HasAura(eSpell::GlyphOfIgnite))
                                        l_Caster->CastSpell(l_Target, eSpell::GlyphOfIgniteEffect, true);
                                }
                            }
                        }
                    }
                }
            }

            void Register()
            {
                AfterHit += SpellHitFn(spell_mastery_ignite_SpellScript::HandleAfterHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_mastery_ignite_SpellScript();
        }
};

/// Mastery : Hand of Light - 76672
class spell_mastery_hand_of_light: public SpellScriptLoader
{
    public:
        spell_mastery_hand_of_light() : SpellScriptLoader("spell_mastery_hand_of_light") { }

        class spell_mastery_hand_of_light_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mastery_hand_of_light_AuraScript);

            enum eSpells
            {
                CrusaderStrike          = 35395,
                HammeroftheRighteous    = 53595,
                HammerofWrath           = 24275,
                TemplarsVerdict         = 85256,
                DivineStorm             = 53385,
                FinalVerdict            = 157048,
                Inquisition             = 111341,
                HandOfLightEffect       = 96172,
                EmpoweredHammerofWrath  = 158392
            };

            void OnProc(constAuraEffectPtr p_AurEff, ProcEventInfo& p_EventInfo)
            {
                PreventDefaultAction();

                SpellInfo const* l_SpellInfo = p_EventInfo.GetDamageInfo()->GetSpellInfo();

                if (l_SpellInfo == nullptr)
                    return;

                if (l_SpellInfo->Id != eSpells::CrusaderStrike && l_SpellInfo->Id != eSpells::HammeroftheRighteous 
                    && l_SpellInfo->Id != eSpells::HammerofWrath && l_SpellInfo->Id != eSpells::TemplarsVerdict 
                    && l_SpellInfo->Id != eSpells::DivineStorm && l_SpellInfo->Id != eSpells::FinalVerdict &&
                    l_SpellInfo->Id != eSpells::EmpoweredHammerofWrath)
                    return;

                Unit* l_Target = p_EventInfo.GetDamageInfo()->GetVictim();
                Unit* l_Caster = p_EventInfo.GetDamageInfo()->GetAttacker();

                if (l_Target == nullptr || l_Caster == nullptr)
                    return;

                Player* l_Player = l_Caster->ToPlayer();

                if (l_Player == nullptr)
                    return;

                int32 l_TotalDamage = p_EventInfo.GetDamageInfo()->GetDamage() + p_EventInfo.GetDamageInfo()->GetAbsorb();
                float l_MasteryValue = p_AurEff->GetAmount();
                int32 l_Bp = int32(CalculatePct(l_TotalDamage, l_MasteryValue));

                SpellInfo const* l_Inquisition = sSpellMgr->GetSpellInfo(eSpells::Inquisition);
                // [Inquisition - 84963] does increase the holy damage done by Mastery : Hand of Light - 76672
                if (l_Caster->HasAura(eSpells::Inquisition))
                    AddPct(l_Bp, l_Inquisition->Effects[EFFECT_0].BasePoints);

                l_Caster->CastCustomSpell(l_Target, eSpells::HandOfLightEffect, &l_Bp, NULL, NULL, true);
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_mastery_hand_of_light_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_mastery_hand_of_light_AuraScript();
        }
};

// Called by 403 - Lightning Bolt, 421 - Chain Lightning, 51505 - Lava Burst and 117014 - Elemental Blast
// 77222 - Mastery : Elemental Overload
class spell_mastery_elemental_overload: public SpellScriptLoader
{
    public:
        spell_mastery_elemental_overload() : SpellScriptLoader("spell_mastery_elemental_overload") { }

        class spell_mastery_elemental_overload_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mastery_elemental_overload_SpellScript);

            bool Validate(SpellInfo const* /*spellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(403) || !sSpellMgr->GetSpellInfo(421) || !sSpellMgr->GetSpellInfo(51505) || !sSpellMgr->GetSpellInfo(117014))
                    return false;
                return true;
            }

            void HandleOnHit()
            {
                SpellInfo const* procSpell = GetSpellInfo();

                if (procSpell)
                {
                    if (Unit* caster = GetCaster())
                    {
                        if (Unit* unitTarget = GetHitUnit())
                        {
                            if (caster->GetTypeId() == TYPEID_PLAYER && caster->HasAura(77222))
                            {
                                // Every Lightning Bolt, Chain Lightning and Lava Burst spells have duplicate vs 75% damage and no cost
                                switch (procSpell->Id)
                                {
                                    // Lava Burst
                                    case 51505:
                                    {
                                        float Mastery = caster->GetFloatValue(PLAYER_FIELD_MASTERY) * 2.0f;

                                        if (roll_chance_f(Mastery))
                                            caster->CastSpell(unitTarget, MASTERY_SPELL_LAVA_BURST, true);

                                        break;
                                    }
                                    // Lightning Bolt
                                    case 403:
                                    {
                                        float Mastery = caster->GetFloatValue(PLAYER_FIELD_MASTERY) * 2.0f;

                                        if (roll_chance_f(Mastery))
                                            caster->CastSpell(unitTarget, MASTERY_SPELL_LIGHTNING_BOLT, true);

                                        break;
                                    }
                                    // Chain Lightning
                                    case 421:
                                    {
                                        float Mastery = caster->GetFloatValue(PLAYER_FIELD_MASTERY) * 2.0f;

                                        if (roll_chance_f(Mastery))
                                            caster->CastSpell(unitTarget, MASTERY_SPELL_CHAIN_LIGHTNING, true);

                                        break;
                                    }
                                    // Elemental Blast
                                    case 117014:
                                    {
                                        float Mastery = caster->GetFloatValue(PLAYER_FIELD_MASTERY) * 2.0f;

                                        if (roll_chance_f(Mastery))
                                        {
                                            caster->CastSpell(unitTarget, MASTERY_SPELL_ELEMENTAL_BLAST, true);
                                            caster->CastSpell(unitTarget, 118517, true); // Nature visual
                                            caster->CastSpell(unitTarget, 118515, true); // Frost visual
                                        }

                                        break;
                                    }
                                    default: break;
                                }
                            }
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_mastery_elemental_overload_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_mastery_elemental_overload_SpellScript();
        }
};

/// Mastrey: Primal Tenacity - 159195
class spell_mastery_primal_tenacity : public SpellScriptLoader
{
public:
    spell_mastery_primal_tenacity() : SpellScriptLoader("spell_mastery_primal_tenacity") { }

    class spell_mastery_primal_tenacity_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_mastery_primal_tenacity_AuraScript);

        enum eSpells
        {
            SPELL_DRU_PRIMAL_TENACITY = 155784
        };

        void OnUpdate(uint32 /*diff*/, AuraEffectPtr aurEff)
        {
            if (!GetCaster())
                return;

            if (Unit* l_Player = GetCaster()->ToPlayer())
            {
                l_Player->CastSpell(l_Player, SPELL_DRU_PRIMAL_TENACITY, true);

                int32 l_Mastery = int32(l_Player->GetFloatValue(EPlayerFields::PLAYER_FIELD_MASTERY) * 1.5f);

                /// Update tooltip information
                if (AuraEffectPtr l_PrimalTenacityEffect = l_Player->GetAuraEffect(SPELL_DRU_PRIMAL_TENACITY, EFFECT_1))
                    l_PrimalTenacityEffect->SetAmount(l_Mastery);
            }
        }

        void OnRemove(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            if (Player* l_Player = GetTarget()->ToPlayer())
            {
                if (l_Player->HasAura(SPELL_DRU_PRIMAL_TENACITY))
                    l_Player->RemoveAura(SPELL_DRU_PRIMAL_TENACITY);
            }
        }

        void Register()
        {
            OnEffectUpdate += AuraEffectUpdateFn(spell_mastery_primal_tenacity_AuraScript::OnUpdate, EFFECT_0, SPELL_AURA_MOD_ATTACK_POWER_PCT);
            OnEffectRemove += AuraEffectRemoveFn(spell_mastery_primal_tenacity_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_MOD_ATTACK_POWER_PCT, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_mastery_primal_tenacity_AuraScript();
    }
};

void AddSC_mastery_spell_scripts()
{
    new spell_mastery_icicles_proc();
    new spell_mastery_molten_earth();
    new spell_mastery_molten_earth_periodic();
    new spell_mastery_sniper_training();
    new spell_mastery_recently_moved();
    new spell_mastery_sniper_training_aura();
    new spell_mastery_echo_of_light();
    new spell_mastery_icicles_trigger();
    new spell_mastery_icicles_periodic();
    new spell_mastery_blood_shield();
    new spell_mastery_ignite();
    new spell_mastery_hand_of_light();
    new spell_mastery_elemental_overload();
    new spell_mastery_primal_tenacity();
}