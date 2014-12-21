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
 * Scripts for spells with SPELLFAMILY_DEATHKNIGHT and SPELLFAMILY_GENERIC spells used by deathknight players.
 * Ordered alphabetically using scriptname.
 * Scriptnames of files in this file should be prefixed with "spell_dk_".
 */

#include "ScriptMgr.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "Containers.h"

enum DeathKnightSpells
{
    DK_SPELL_RUNIC_POWER_ENERGIZE               = 49088,
    DK_SPELL_ANTI_MAGIC_SHELL_TALENT            = 51052,
    DK_SPELL_SCOURGE_STRIKE_TRIGGERED           = 70890,
    DK_SPELL_BLOOD_BOIL_TRIGGERED               = 65658,
    SPELL_DK_ITEM_T8_MELEE_4P_BONUS             = 64736,
    DK_SPELL_BLOOD_PLAGUE                       = 55078,
    DK_SPELL_FROST_FEVER                        = 55095,
    DK_SPELL_GHOUL_AS_GUARDIAN                  = 46585,
    DK_SPELL_GHOUL_AS_PET                       = 52150,
    DK_SPELL_BLOOD_BOIL                         = 50842,
    DK_SPELL_CHILBLAINS                         = 50041,
    DK_SPELL_CHAINS_OF_ICE_ROOT                 = 53534,
    DK_SPELL_PLAGUE_LEECH                       = 123693,
    DK_SPELL_PERDITION                          = 123981,
    DK_SPELL_SHROUD_OF_PURGATORY                = 116888,
    DK_SPELL_PURGATORY_INSTAKILL                = 123982,
    DK_SPELL_BLOOD_RITES                        = 50034,
    DK_SPELL_DEATH_SIPHON_HEAL                  = 116783,
    DK_SPELL_BLOOD_CHARGE                       = 114851,
    DK_SPELL_PILLAR_OF_FROST                    = 51271,
    DK_SPELL_SOUL_REAPER_HASTE                  = 114868,
    DK_SPELL_SOUL_REAPER_DAMAGE                 = 114867,
    DK_SPELL_REMORSELESS_WINTER_STUN            = 115001,
    DK_SPELL_REMORSELESS_WINTER                 = 115000,
    DK_SPELL_CONVERSION_REGEN                   = 119980,
    DK_SPELL_SCENT_OF_BLOOD                     = 49509,
    DK_SPELL_SCENT_OF_BLOOD_AURA                = 50421,
    DK_SPELL_CHAINS_OF_ICE                      = 45524,
    DK_SPELL_EBON_PLAGUEBRINGER                 = 51160,
    DK_SPELL_DESECRATED_GROUND                  = 118009,
    DK_SPELL_DESECRATED_GROUND_IMMUNE           = 115018,
    DK_SPELL_ASPHYXIATE                         = 108194,
    DK_SPELL_DARK_INFUSION_STACKS               = 91342,
    DK_SPELL_DARK_INFUSION_AURA                 = 93426,
    DK_NPC_WILD_MUSHROOM                        = 59172,
    DK_SPELL_RUNIC_EMPOWERMENT                  = 81229,
    DK_SPELL_GOREFIENDS_GRASP_GRIP_VISUAL       = 114869,
    DK_SPELL_DEATH_GRIP_ONLY_JUMP               = 146599,
    DK_SPELL_GLYPH_OF_CORPSE_EXPLOSION          = 127344,
    DK_SPELL_GLYPH_OF_HORN_OF_WINTER            = 58680,
    DK_SPELL_GLYPH_OF_HORN_OF_WINTER_EFFECT     = 121920,
    DK_SPELL_DEATH_COIL_DAMAGE                  = 47632,
    DK_SPELL_GLYPH_OF_DEATH_AND_DECAY           = 58629,
    DK_SPELL_DEATH_AND_DECAY_DECREASE_SPEED     = 143375,
    DK_SPELL_DEATH_STRIKE_HEAL                  = 45470,
    DK_SPELL_PLAGUEBEARER                       = 161497,
    DK_SPELL_NECROTIC_PLAGUE                    = 152281,
    DK_SPELL_NECROTIC_PLAGUE_APPLY_AURA         = 155159,
    DK_SPELL_RUNIC_CORRUPTION_AURA              = 51462,
    DK_SPELL_RUNIC_CORRUPTION                   = 51460,
    DK_SPELL_DEATH_PACT                         = 48743,
    DK_SPELL_ICY_TOUCH                          = 45477,
    DK_SPELL_CHILBLAINS_TRIGGER                 = 50435,
    DK_SPELL_REAPING                            = 56835
};

uint32 g_TabDeasesDK[3] = { DK_SPELL_FROST_FEVER, DK_SPELL_BLOOD_PLAGUE, DK_SPELL_NECROTIC_PLAGUE_APPLY_AURA };

// Death and Decay - 43265
class spell_dk_death_and_decay: public MS::Game::Scripting::Interfaces::SpellScriptLoader
{
    public:
        spell_dk_death_and_decay() : MS::Game::Scripting::Interfaces::SpellScriptLoader("spell_dk_death_and_decay") { }

        class spell_dk_death_and_decay_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_death_and_decay_SpellScript);

            void HandleAfterCast()
            {
                WorldLocation* dest = const_cast<WorldLocation*>(GetExplTargetDest());
                if (dest && GetCaster()->HasAura(DK_SPELL_GLYPH_OF_DEATH_AND_DECAY))
                    GetCaster()->CastSpell(dest->GetPositionX(), dest->GetPositionY(), dest->GetPositionZ(), DK_SPELL_DEATH_AND_DECAY_DECREASE_SPEED, true);
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_dk_death_and_decay_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_death_and_decay_SpellScript();
        }
};

// Death Barrier - 115635
class spell_dk_death_barrier: public MS::Game::Scripting::Interfaces::SpellScriptLoader
{
    public:
        spell_dk_death_barrier() : MS::Game::Scripting::Interfaces::SpellScriptLoader("spell_dk_death_barrier") { }

        class spell_dk_death_barrier_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dk_death_barrier_AuraScript);

            void CalculateAmount(constAuraEffectPtr aurEff, int32 & amount, bool & /*canBeRecalculated*/)
            {
                if (Unit* caster = GetCaster())
                {
                    amount += caster->GetTotalAttackPowerValue(WeaponAttackType::BaseAttack) * 0.514f;
                    amount = int32(caster->SpellDamageBonusDone(GetUnitOwner(), sSpellMgr->GetSpellInfo(DK_SPELL_DEATH_COIL_DAMAGE), amount, aurEff->GetEffIndex(), SPELL_DIRECT_DAMAGE));
                }
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_death_barrier_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dk_death_barrier_AuraScript();
        }
};

// Plague Strike - 45462
class spell_dk_plague_strike: public MS::Game::Scripting::Interfaces::SpellScriptLoader
{
    public:
        spell_dk_plague_strike() : MS::Game::Scripting::Interfaces::SpellScriptLoader("spell_dk_plague_strike") { }

        class spell_dk_plague_strike_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_plague_strike_SpellScript);

            void HandleDamage(SpellEffIndex effIndex)
            {
                if (Unit* caster = GetCaster())
                {
                    if (caster->HasAura(DK_SPELL_EBON_PLAGUEBRINGER) && GetHitUnit())
                        caster->CastSpell(GetHitUnit(), DK_SPELL_FROST_FEVER, true);
                }
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (!GetCaster() || !GetHitUnit())
                    return;

                GetCaster()->CastSpell(GetHitUnit(), DK_SPELL_BLOOD_PLAGUE, true);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_dk_plague_strike_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_NORMALIZED_WEAPON_DMG);
                OnEffectHitTarget += SpellEffectFn(spell_dk_plague_strike_SpellScript::HandleDummy, EFFECT_1, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_plague_strike_SpellScript();
        }
};



// Gorefiend's Grasp - 108199
class spell_dk_gorefiends_grasp: public MS::Game::Scripting::Interfaces::SpellScriptLoader
{
    public:
        spell_dk_gorefiends_grasp() : MS::Game::Scripting::Interfaces::SpellScriptLoader("spell_dk_gorefiends_grasp") { }

        class spell_dk_gorefiends_grasp_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_gorefiends_grasp_SpellScript);

            void HandleScript(SpellEffIndex effIndex)
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        std::list<Unit*> tempList;
                        std::list<Unit*> gripList;

                        _player->GetAttackableUnitListInRange(tempList, 20.0f);

                        for (auto itr : tempList)
                        {
                            if (itr->GetGUID() == _player->GetGUID())
                                continue;

                            if (!_player->IsValidAttackTarget(itr))
                                continue;

                            if (!itr->IsWithinLOSInMap(_player))
                                continue;

                            gripList.push_back(itr);
                        }

                        for (auto itr : gripList)
                        {
                            itr->CastSpell(target, DK_SPELL_DEATH_GRIP_ONLY_JUMP, true);
                            itr->CastSpell(target, DK_SPELL_GOREFIENDS_GRASP_GRIP_VISUAL, true);
                        }
                    }
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_dk_gorefiends_grasp_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_gorefiends_grasp_SpellScript();
        }
};

// Dark transformation - transform pet spell - 63560
class spell_dk_dark_transformation_form: public MS::Game::Scripting::Interfaces::SpellScriptLoader
{
    public:
        spell_dk_dark_transformation_form() : MS::Game::Scripting::Interfaces::SpellScriptLoader("spell_dk_dark_transformation_form") { }

        class spell_dk_dark_transformation_form_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_dark_transformation_form_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* pet = GetHitUnit())
                    {
                        if (pet->HasAura(DK_SPELL_DARK_INFUSION_STACKS))
                        {
                            _player->RemoveAura(DK_SPELL_DARK_INFUSION_STACKS);
                            pet->RemoveAura(DK_SPELL_DARK_INFUSION_STACKS);
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_dk_dark_transformation_form_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_dark_transformation_form_SpellScript();
        }
};

// Desecrated ground - 118009
class spell_dk_desecrated_ground: public MS::Game::Scripting::Interfaces::SpellScriptLoader
{
    public:
        spell_dk_desecrated_ground() : MS::Game::Scripting::Interfaces::SpellScriptLoader("spell_dk_desecrated_ground") { }

        class spell_dk_desecrated_ground_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dk_desecrated_ground_AuraScript);

            void OnTick(constAuraEffectPtr aurEff)
            {
                if (GetCaster())
                    if (DynamicObject* dynObj = GetCaster()->GetDynObject(DK_SPELL_DESECRATED_GROUND))
                        if (GetCaster()->GetDistance(dynObj) <= 8.0f)
                            GetCaster()->CastSpell(GetCaster(), DK_SPELL_DESECRATED_GROUND_IMMUNE, true);
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_dk_desecrated_ground_AuraScript::OnTick, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dk_desecrated_ground_AuraScript();
        }
};

// Festering Strike - 85948
class spell_dk_festering_strike: public MS::Game::Scripting::Interfaces::SpellScriptLoader
{
    public:
        spell_dk_festering_strike() : MS::Game::Scripting::Interfaces::SpellScriptLoader("spell_dk_festering_strike") { }

        class spell_dk_festering_strike_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_festering_strike_SpellScript);

            void HandleOnHit()
            {
                if (Player* l_Player = GetCaster()->ToPlayer())
                {
                    if (Unit* l_Target = GetHitUnit())
                    {
                        if (AuraPtr l_AuraBloodPlague = l_Target->GetAura(DK_SPELL_BLOOD_PLAGUE, l_Player->GetGUID()))
                        {
                            uint32 dur = l_AuraBloodPlague->GetDuration() + 6000;
                            l_AuraBloodPlague->SetDuration(dur);

                            if (dur > uint32(l_AuraBloodPlague->GetMaxDuration()))
                                l_AuraBloodPlague->SetMaxDuration(dur);
                        }

                        if (AuraPtr l_AuraFrostFever = l_Target->GetAura(DK_SPELL_FROST_FEVER, l_Player->GetGUID()))
                        {
                            uint32 dur = l_AuraFrostFever->GetDuration() + 6000;
                            l_AuraFrostFever->SetDuration(dur);

                            if (dur > uint32(l_AuraFrostFever->GetMaxDuration()))
                                l_AuraFrostFever->SetMaxDuration(dur);
                        }

                        if (AuraPtr l_AuraChainsOfIce = l_Target->GetAura(DK_SPELL_CHAINS_OF_ICE, l_Player->GetGUID()))
                        {
                            uint32 dur = l_AuraChainsOfIce->GetDuration() + 6000;
                            l_AuraChainsOfIce->SetDuration(dur);

                            if (dur > uint32(l_AuraChainsOfIce->GetMaxDuration()))
                                l_AuraChainsOfIce->SetMaxDuration(dur);
                        }
                    }
                }
            }
            void Register()
            {
                OnHit += SpellHitFn(spell_dk_festering_strike_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_festering_strike_SpellScript();
        }
};

// Death Strike heal - 45470
class spell_dk_death_strike_heal: public MS::Game::Scripting::Interfaces::SpellScriptLoader
{
    public:
        spell_dk_death_strike_heal() : MS::Game::Scripting::Interfaces::SpellScriptLoader("spell_dk_death_strike_heal") { }

        class spell_dk_death_strike_heal_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_death_strike_heal_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (AuraPtr scentOfBlood = _player->GetAura(DK_SPELL_SCENT_OF_BLOOD_AURA))
                        {
                            uint8 chg = scentOfBlood->GetStackAmount();
                            uint32 hl = GetHitHeal() * 0.2 * chg;
                            SetHitHeal(GetHitHeal() + hl);
                        }

                        _player->RemoveAura(DK_SPELL_SCENT_OF_BLOOD_AURA);
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_dk_death_strike_heal_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_death_strike_heal_SpellScript();
        }
};

// Howling Blast - 49184
class spell_dk_howling_blast: public MS::Game::Scripting::Interfaces::SpellScriptLoader
{
    public:
        spell_dk_howling_blast() : MS::Game::Scripting::Interfaces::SpellScriptLoader("spell_dk_howling_blast") { }

        class spell_dk_howling_blast_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_howling_blast_SpellScript);

            uint64 tar;

            void HandleBeforeCast()
            {
                Unit* target = GetExplTargetUnit();
                Unit* caster = GetCaster();

                if (!caster || !target)
                    return;

                tar = target->GetGUID();
            }

            void HandleOnHit()
            {
                Unit* target = GetHitUnit();
                Unit* caster = GetCaster();

                if (!caster || !target || !tar)
                    return;

                if (target->GetGUID() != tar)
                    SetHitDamage(GetHitDamage()/2);

                caster->CastSpell(target, DK_SPELL_FROST_FEVER, true);
            }

            void Register()
            {
                BeforeCast += SpellCastFn(spell_dk_howling_blast_SpellScript::HandleBeforeCast);
                OnHit += SpellHitFn(spell_dk_howling_blast_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_howling_blast_SpellScript();
        }
};

// Conversion - 119975
class spell_dk_conversion: public MS::Game::Scripting::Interfaces::SpellScriptLoader
{
public:
    spell_dk_conversion() : MS::Game::Scripting::Interfaces::SpellScriptLoader("spell_dk_conversion") { }

    class spell_dk_conversion_AuraScript : public AuraScript
    {
        PrepareAuraScript(spell_dk_conversion_AuraScript);

        void OnTick(constAuraEffectPtr aurEff)
        {
            if (Unit* l_unit = GetCaster())
                l_unit->CastSpell(l_unit, DK_SPELL_CONVERSION_REGEN, true);
        }

        void Register()
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(spell_dk_conversion_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_dk_conversion_AuraScript();
    }
};

// Remorseless Winter - 115000
class spell_dk_remorseless_winter: public MS::Game::Scripting::Interfaces::SpellScriptLoader
{
    public:
        spell_dk_remorseless_winter() : MS::Game::Scripting::Interfaces::SpellScriptLoader("spell_dk_remorseless_winter") { }

        class spell_dk_remorseless_winter_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_remorseless_winter_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (Unit* target = GetHitUnit())
                        if (AuraPtr remorselessWinter = target->GetAura(DK_SPELL_REMORSELESS_WINTER))
                            if (remorselessWinter->GetStackAmount() == 5 && !target->HasAura(DK_SPELL_REMORSELESS_WINTER_STUN))
                                _player->CastSpell(target, DK_SPELL_REMORSELESS_WINTER_STUN, true);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_dk_remorseless_winter_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_remorseless_winter_SpellScript();
        }
};

// Soul Reaper - 130736 (unholy) or 130735 (frost) or 114866 (blood)
class spell_dk_soul_reaper: public MS::Game::Scripting::Interfaces::SpellScriptLoader
{
    public:
        spell_dk_soul_reaper() : MS::Game::Scripting::Interfaces::SpellScriptLoader("spell_dk_soul_reaper") { }

        class spell_dk_soul_reaper_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dk_soul_reaper_AuraScript);

            void HandleRemove(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (GetCaster())
                {
                    AuraRemoveMode removeMode = GetTargetApplication()->GetRemoveMode();
                    if (removeMode == AURA_REMOVE_BY_DEATH)
                        GetCaster()->CastSpell(GetCaster(), DK_SPELL_SOUL_REAPER_HASTE, true);
                    else if (removeMode == AURA_REMOVE_BY_EXPIRE && GetTarget()->GetHealthPct() < 35.0f)
                        GetCaster()->CastSpell(GetTarget(), DK_SPELL_SOUL_REAPER_DAMAGE, true);
                }
            }

            void Register()
            {
                OnEffectRemove += AuraEffectApplyFn(spell_dk_soul_reaper_AuraScript::HandleRemove, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dk_soul_reaper_AuraScript();
        }
};

// Pillar of Frost - 51271
class spell_dk_pillar_of_frost: public MS::Game::Scripting::Interfaces::SpellScriptLoader
{
    public:
        spell_dk_pillar_of_frost() : MS::Game::Scripting::Interfaces::SpellScriptLoader("spell_dk_pillar_of_frost") { }

        class spell_dk_pillar_of_frost_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dk_pillar_of_frost_AuraScript);

            void OnRemove(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Player* _player = GetTarget()->ToPlayer())
                    _player->ApplySpellImmune(DK_SPELL_PILLAR_OF_FROST, IMMUNITY_MECHANIC, MECHANIC_KNOCKOUT, false);
            }

            void OnApply(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Player* _player = GetTarget()->ToPlayer())
                    _player->ApplySpellImmune(DK_SPELL_PILLAR_OF_FROST, IMMUNITY_MECHANIC, MECHANIC_KNOCKOUT, true);
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_dk_pillar_of_frost_AuraScript::OnApply, EFFECT_2, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_dk_pillar_of_frost_AuraScript::OnRemove, EFFECT_2, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dk_pillar_of_frost_AuraScript();
        }
};

// Blood Tap - 45529
class PlayerScript_Blood_Tap: public MS::Game::Scripting::Interfaces::PlayerScript
{
    public:
        PlayerScript_Blood_Tap() :PlayerScript("PlayerScript_Blood_Tap") {}

        uint16 m_RunicPower = 0;

        void OnModifyPower(Player* p_Player, Powers p_Power, int32 p_Value)
        {
            if (p_Player->getClass() != CLASS_DEATH_KNIGHT || p_Power != POWER_RUNIC_POWER || !p_Player->HasSpell(45529))
                return;

            // Only on use runic power
            if (p_Value > 0)
                return;

            m_RunicPower += -p_Value;

            if (m_RunicPower >= 150)
            {
                uint8 l_Stack = m_RunicPower / 150;

                for (uint8 l_I = 0; l_I < l_Stack; ++l_I)
                {
                    p_Player->CastSpell(p_Player, DK_SPELL_BLOOD_CHARGE, true);
                    m_RunicPower -= 150;
                }
            }
        }
};

// Blood Tap - 45529
class spell_dk_blood_tap: public MS::Game::Scripting::Interfaces::SpellScriptLoader
{
    public:
        spell_dk_blood_tap() : MS::Game::Scripting::Interfaces::SpellScriptLoader("spell_dk_blood_tap") { }

        class spell_dk_blood_tap_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_blood_tap_SpellScript);

            SpellCastResult CheckBloodCharges()
            {
                if (GetCaster()->ToPlayer())
                {
                    if (AuraPtr bloodCharges = GetCaster()->ToPlayer()->GetAura(DK_SPELL_BLOOD_CHARGE))
                    {
                        if (bloodCharges->GetStackAmount() < 5)
                            return SPELL_FAILED_DONT_REPORT;
                    }
                    else
                        return SPELL_FAILED_DONT_REPORT;

                    bool cooldown = false;

                    for (uint8 i = 0; i < MAX_RUNES; ++i)
                    {
                        if (GetCaster()->ToPlayer()->GetCurrentRune(i) == RUNE_DEATH || !GetCaster()->ToPlayer()->GetRuneCooldown(i))
                            continue;

                        cooldown = true;
                    }

                    if (!cooldown)
                        return SPELL_FAILED_DONT_REPORT;
                }

                return SPELL_CAST_OK;
            }

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (AuraPtr bloodCharges = _player->GetAura(DK_SPELL_BLOOD_CHARGE))
                        {
                            int32 newAmount = bloodCharges->GetStackAmount();

                            if ((newAmount - 5) <= 0)
                                _player->RemoveAura(DK_SPELL_BLOOD_CHARGE);
                            else
                                bloodCharges->SetStackAmount(newAmount - 5);
                        }
                    }
                }
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_dk_blood_tap_SpellScript::CheckBloodCharges);
                OnHit += SpellHitFn(spell_dk_blood_tap_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_blood_tap_SpellScript();
        }
};

// Death Siphon - 108196
class spell_dk_death_siphon: public MS::Game::Scripting::Interfaces::SpellScriptLoader
{
    public:
        spell_dk_death_siphon() : MS::Game::Scripting::Interfaces::SpellScriptLoader("spell_dk_death_siphon") { }

        class spell_dk_death_siphon_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_death_siphon_SpellScript);

            void HandleScriptEffect(SpellEffIndex /*effIndex*/)
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        int32 bp = GetHitDamage() * 3.35f;
                        _player->CastCustomSpell(_player, DK_SPELL_DEATH_SIPHON_HEAL, &bp, NULL, NULL, true);
                    }
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_dk_death_siphon_SpellScript::HandleScriptEffect, EFFECT_1, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_death_siphon_SpellScript();
        }
};

// Improved Blood Presence - 50371
class spell_dk_improved_blood_presence: public MS::Game::Scripting::Interfaces::SpellScriptLoader
{
    public:
        spell_dk_improved_blood_presence() : MS::Game::Scripting::Interfaces::SpellScriptLoader("spell_dk_improved_blood_presence") { }

        class spell_dk_improved_blood_presence_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_improved_blood_presence_SpellScript);

            void HandleAfterCast()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    _player->UpdateAllRunesRegen();
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_dk_improved_blood_presence_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_improved_blood_presence_SpellScript();
        }
};

// Unholy Presence - 48265 and Improved Unholy Presence - 50392
class spell_dk_unholy_presence: public MS::Game::Scripting::Interfaces::SpellScriptLoader
{
    public:
        spell_dk_unholy_presence() : MS::Game::Scripting::Interfaces::SpellScriptLoader("spell_dk_unholy_presence") { }

        class spell_dk_unholy_presence_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dk_unholy_presence_AuraScript);

            void OnRemove(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Player* _player = GetTarget()->ToPlayer())
                    _player->UpdateAllRunesRegen();
            }

            void OnApply(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Player* _player = GetTarget()->ToPlayer())
                    _player->UpdateAllRunesRegen();
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_dk_unholy_presence_AuraScript::OnApply, EFFECT_1, SPELL_AURA_MOD_INCREASE_SPEED, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_dk_unholy_presence_AuraScript::OnRemove, EFFECT_1, SPELL_AURA_MOD_INCREASE_SPEED, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dk_unholy_presence_AuraScript();
        }
};

// Death Strike - 49998
class spell_dk_death_strike: public MS::Game::Scripting::Interfaces::SpellScriptLoader
{
    public:
        spell_dk_death_strike() : MS::Game::Scripting::Interfaces::SpellScriptLoader("spell_dk_death_strike") { }

        class spell_dk_death_strike_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_death_strike_SpellScript);

            void HandleOnHit()
            {
                if (Player* l_Player = GetCaster()->ToPlayer())
                {
                    if (Unit* l_Target = GetHitUnit())
                    {
                        l_Player->CastSpell(l_Player, DK_SPELL_DEATH_STRIKE_HEAL, true);

                        // Apply Blood Rites effects
                        if (l_Player->HasAura(DK_SPELL_BLOOD_RITES))
                        {
                            bool l_RuneFrost = false;
                            bool l_RuneUnholy = false;

                            for (uint8 i = 0; i < MAX_RUNES; ++i)
                            {
                                if (l_Player->GetCurrentRune(i) == RUNE_DEATH
                                    || l_Player->GetCurrentRune(i) == RUNE_BLOOD
                                    || l_Player->GetBaseRune(i) == RUNE_BLOOD)
                                    continue;

                                if (l_RuneUnholy && l_Player->GetCurrentRune(i) == RUNE_UNHOLY)
                                    continue;

                                if (l_RuneFrost && l_Player->GetCurrentRune(i) == RUNE_FROST)
                                    continue;

                                if (l_Player->GetRuneCooldown(i))
                                {
                                    if (l_Player->GetCurrentRune(i) == RUNE_FROST)
                                        l_RuneFrost = true;
                                    else
                                        l_RuneUnholy = true;

                                    l_Player->ConvertRune(i, RUNE_DEATH);
                                }
                            }
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_dk_death_strike_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_death_strike_SpellScript();
        }
};

// Purgatory - 116888
class spell_dk_purgatory: public MS::Game::Scripting::Interfaces::SpellScriptLoader
{
    public:
        spell_dk_purgatory() : MS::Game::Scripting::Interfaces::SpellScriptLoader("spell_dk_purgatory") { }

        class spell_dk_purgatory_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dk_purgatory_AuraScript);

            void OnRemove(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Player* _player = GetTarget()->ToPlayer())
                {
                    AuraRemoveMode removeMode = GetTargetApplication()->GetRemoveMode();
                    if (removeMode == AURA_REMOVE_BY_EXPIRE)
                        _player->CastSpell(_player, DK_SPELL_PURGATORY_INSTAKILL, true);
                }
            }

            void Register()
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_dk_purgatory_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_SCHOOL_HEAL_ABSORB, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dk_purgatory_AuraScript();
        }
};

// Purgatory - 114556
class spell_dk_purgatory_absorb: public MS::Game::Scripting::Interfaces::SpellScriptLoader
{
    public:
        spell_dk_purgatory_absorb() : MS::Game::Scripting::Interfaces::SpellScriptLoader("spell_dk_purgatory_absorb") { }

        class spell_dk_purgatory_absorb_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dk_purgatory_absorb_AuraScript);

            void CalculateAmount(constAuraEffectPtr /*auraEffect*/, int32& amount, bool& /*canBeRecalculated*/)
            {
                amount = -1;
            }

            void Absorb(AuraEffectPtr /*auraEffect*/, DamageInfo& dmgInfo, uint32& absorbAmount)
            {
                Unit* target = GetTarget();

                if (dmgInfo.GetDamage() < target->GetHealth())
                    return;

                // No damage received under Shroud of Purgatory
                if (target->ToPlayer()->HasAura(DK_SPELL_SHROUD_OF_PURGATORY))
                {
                    absorbAmount = dmgInfo.GetDamage();
                    return;
                }

                if (target->ToPlayer()->HasAura(DK_SPELL_PERDITION))
                    return;

                int32 bp = dmgInfo.GetDamage();

                target->CastCustomSpell(target, DK_SPELL_SHROUD_OF_PURGATORY, &bp, NULL, NULL, true);
                target->CastSpell(target, DK_SPELL_PERDITION, true);
                target->SetHealth(1);
                absorbAmount = dmgInfo.GetDamage();
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_purgatory_absorb_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
                OnEffectAbsorb += AuraEffectAbsorbFn(spell_dk_purgatory_absorb_AuraScript::Absorb, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dk_purgatory_absorb_AuraScript();
        }
};

// Plague Leech - 123693
class spell_dk_plague_leech: public MS::Game::Scripting::Interfaces::SpellScriptLoader
{
    public:
        spell_dk_plague_leech() : MS::Game::Scripting::Interfaces::SpellScriptLoader("spell_dk_plague_leech") { }

        class spell_dk_plague_leech_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_plague_leech_SpellScript);

            std::list<uint8> m_LstRunesUsed;

            SpellCastResult CheckRunes()
            {
                if (GetCaster() && GetCaster()->ToPlayer())
                {
                    for (uint8 i = 0; i < MAX_RUNES; ++i)
                        if (GetCaster()->ToPlayer()->GetRuneCooldown(i))
                            m_LstRunesUsed.push_back(i);

                    if (m_LstRunesUsed.size() < 2)
                        return SPELL_FAILED_DONT_REPORT;
                    else
                        return SPELL_CAST_OK;

                    if (Unit* target = GetExplTargetUnit())
                    {
                        if (!target->HasAura(DK_SPELL_BLOOD_PLAGUE) || !target->HasAura(DK_SPELL_FROST_FEVER))
                            return SPELL_FAILED_DONT_REPORT;
                        else
                            return SPELL_CAST_OK;
                    }
                }
                else
                    return SPELL_FAILED_DONT_REPORT;
            }

            void HandleOnHit()
            {
                if (Player* l_Player = GetCaster()->ToPlayer())
                {
                    if (Unit* l_Target = GetHitUnit())
                    {
                        l_Target->RemoveAura(DK_SPELL_FROST_FEVER);
                        l_Target->RemoveAura(DK_SPELL_BLOOD_PLAGUE);

                        for (uint8 l_I = 0; l_I < 2; l_I++)
                        {
                            uint8 l_RuneRandom = JadeCore::Containers::SelectRandomContainerElement(m_LstRunesUsed);

                            if (l_Player->GetRuneCooldown(l_RuneRandom))
                            {
                                l_Player->SetRuneCooldown(l_RuneRandom, 0);
                                l_Player->ConvertRune(l_RuneRandom, RUNE_DEATH);
                            }

                            m_LstRunesUsed.remove(l_RuneRandom);
                        }
                        l_Player->ResyncRunes(MAX_RUNES);
                    }
                }
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_dk_plague_leech_SpellScript::CheckRunes);
                OnHit += SpellHitFn(spell_dk_plague_leech_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_plague_leech_SpellScript();
        }
};

// Unholy Blight - 115994
class spell_dk_unholy_blight: public MS::Game::Scripting::Interfaces::SpellScriptLoader
{
    public:
        spell_dk_unholy_blight() : MS::Game::Scripting::Interfaces::SpellScriptLoader("spell_dk_unholy_blight") { }

        class spell_dk_unholy_blight_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_unholy_blight_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        _player->CastSpell(target, DK_SPELL_BLOOD_PLAGUE, true);
                        _player->CastSpell(target, DK_SPELL_FROST_FEVER, true);
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_dk_unholy_blight_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_unholy_blight_SpellScript();
        }
};

// Called by Chains of Ice - 45524
// Chilblains - 50041
class spell_dk_chilblains: public MS::Game::Scripting::Interfaces::SpellScriptLoader
{
    public:
        spell_dk_chilblains() : MS::Game::Scripting::Interfaces::SpellScriptLoader("spell_dk_chilblains") { }

        class spell_dk_chilblains_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_chilblains_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (Unit* target = GetHitUnit())
                        if (_player->HasAura(DK_SPELL_CHILBLAINS))
                            _player->CastSpell(target, DK_SPELL_CHAINS_OF_ICE_ROOT, true);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_dk_chilblains_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_chilblains_SpellScript();
        }
};

// Outbreak - 77575
class spell_dk_outbreak: public MS::Game::Scripting::Interfaces::SpellScriptLoader
{
    public:
        spell_dk_outbreak() : MS::Game::Scripting::Interfaces::SpellScriptLoader("spell_dk_outbreak") { }

        class spell_dk_outbreak_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_outbreak_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        _player->CastSpell(target, DK_SPELL_BLOOD_PLAGUE, true);
                        _player->CastSpell(target, DK_SPELL_FROST_FEVER, true);
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_dk_outbreak_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_outbreak_SpellScript();
        }
};

// Raise Dead - 46584
class spell_dk_raise_dead: public MS::Game::Scripting::Interfaces::SpellScriptLoader
{
    public:
        spell_dk_raise_dead() : MS::Game::Scripting::Interfaces::SpellScriptLoader("spell_dk_raise_dead") { }

        class spell_dk_raise_dead_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_raise_dead_SpellScript);

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (Player* l_Player = GetCaster()->ToPlayer())
                {
                    if (l_Player->GetSpecializationId(l_Player->GetActiveSpec()) == SPEC_DK_UNHOLY)
                        l_Player->CastSpell(l_Player, DK_SPELL_GHOUL_AS_PET, true);
                    else
                        l_Player->CastSpell(l_Player, DK_SPELL_GHOUL_AS_GUARDIAN, true);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_dk_raise_dead_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_raise_dead_SpellScript();
        }
};

// 50462 - Anti-Magic Shell (on raid member)
class spell_dk_anti_magic_shell_raid: public MS::Game::Scripting::Interfaces::SpellScriptLoader
{
    public:
        spell_dk_anti_magic_shell_raid() : MS::Game::Scripting::Interfaces::SpellScriptLoader("spell_dk_anti_magic_shell_raid") { }

        class spell_dk_anti_magic_shell_raid_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dk_anti_magic_shell_raid_AuraScript);

            uint32 absorbPct;

            bool Load()
            {
                absorbPct = GetSpellInfo()->Effects[EFFECT_0].CalcValue(GetCaster());
                return true;
            }

            void CalculateAmount(constAuraEffectPtr /*aurEff*/, int32 & amount, bool & /*canBeRecalculated*/)
            {
                // TODO: this should absorb limited amount of damage, but no info on calculation formula
                amount = -1;
            }

            void Absorb(AuraEffectPtr /*aurEff*/, DamageInfo & dmgInfo, uint32 & absorbAmount)
            {
                 absorbAmount = CalculatePct(dmgInfo.GetDamage(), absorbPct);
            }

            void Register()
            {
                 DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_anti_magic_shell_raid_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
                 OnEffectAbsorb += AuraEffectAbsorbFn(spell_dk_anti_magic_shell_raid_AuraScript::Absorb, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dk_anti_magic_shell_raid_AuraScript();
        }
};

// 48707 - Anti-Magic Shell (on self)
class spell_dk_anti_magic_shell_self: public MS::Game::Scripting::Interfaces::SpellScriptLoader
{
    public:
        spell_dk_anti_magic_shell_self() : MS::Game::Scripting::Interfaces::SpellScriptLoader("spell_dk_anti_magic_shell_self") { }

        class spell_dk_anti_magic_shell_self_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dk_anti_magic_shell_self_AuraScript);

            uint32 absorbPct, hpPct;
            bool Load()
            {
                absorbPct = GetSpellInfo()->Effects[EFFECT_0].CalcValue(GetCaster());
                hpPct = GetSpellInfo()->Effects[EFFECT_1].CalcValue(GetCaster());
                return true;
            }

            bool Validate(SpellInfo const* /*spellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(DK_SPELL_RUNIC_POWER_ENERGIZE))
                    return false;
                return true;
            }

            void CalculateAmount(constAuraEffectPtr /*aurEff*/, int32 & amount, bool & /*canBeRecalculated*/)
            {
                if (GetCaster())
                    amount = GetCaster()->CountPctFromMaxHealth(hpPct);
            }

            void Absorb(AuraEffectPtr /*aurEff*/, DamageInfo & dmgInfo, uint32 & absorbAmount)
            {
                absorbAmount = std::min(CalculatePct(dmgInfo.GetDamage(), absorbPct), GetTarget()->CountPctFromMaxHealth(hpPct));
            }

            void Trigger(AuraEffectPtr aurEff, DamageInfo & /*dmgInfo*/, uint32 & absorbAmount)
            {
                Unit* target = GetTarget();
                // damage absorbed by Anti-Magic Shell energizes the DK with additional runic power.
                // This, if I'm not mistaken, shows that we get back ~20% of the absorbed damage as runic power.
                int32 bp = absorbAmount * 2 / 10;
                target->CastCustomSpell(target, DK_SPELL_RUNIC_POWER_ENERGIZE, &bp, NULL, NULL, true, NULL, aurEff);
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_anti_magic_shell_self_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
                OnEffectAbsorb += AuraEffectAbsorbFn(spell_dk_anti_magic_shell_self_AuraScript::Absorb, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
                AfterEffectAbsorb += AuraEffectAbsorbFn(spell_dk_anti_magic_shell_self_AuraScript::Trigger, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dk_anti_magic_shell_self_AuraScript();
        }
};

// 50461 - Anti-Magic Zone
class spell_dk_anti_magic_zone: public MS::Game::Scripting::Interfaces::SpellScriptLoader
{
    public:
        spell_dk_anti_magic_zone() : MS::Game::Scripting::Interfaces::SpellScriptLoader("spell_dk_anti_magic_zone") { }

        class spell_dk_anti_magic_zone_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dk_anti_magic_zone_AuraScript);

            uint32 absorbPct;

            bool Load()
            {
                absorbPct = GetSpellInfo()->Effects[EFFECT_0].CalcValue(GetCaster());
                return true;
            }

            bool Validate(SpellInfo const* /*spellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(DK_SPELL_ANTI_MAGIC_SHELL_TALENT))
                    return false;
                return true;
            }

            void CalculateAmount(constAuraEffectPtr /*aurEff*/, int32 & amount, bool & /*canBeRecalculated*/)
            {
                SpellInfo const* talentSpell = sSpellMgr->GetSpellInfo(DK_SPELL_ANTI_MAGIC_SHELL_TALENT);
                amount = 136800;
                if (Player* player = GetCaster()->ToPlayer())
                     amount += int32(player->GetStat(STAT_STRENGTH) * 4);
            }

            void Absorb(AuraEffectPtr /*aurEff*/, DamageInfo & dmgInfo, uint32 & absorbAmount)
            {
                 absorbAmount = CalculatePct(dmgInfo.GetDamage(), absorbPct);
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_anti_magic_zone_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
                OnEffectAbsorb += AuraEffectAbsorbFn(spell_dk_anti_magic_zone_AuraScript::Absorb, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dk_anti_magic_zone_AuraScript();
        }
};

// Death Gate - 53822
class spell_dk_death_gate_teleport: public MS::Game::Scripting::Interfaces::SpellScriptLoader
{
    public:
        spell_dk_death_gate_teleport() : MS::Game::Scripting::Interfaces::SpellScriptLoader("spell_dk_death_gate_teleport") {}

        class spell_dk_death_gate_teleport_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_death_gate_teleport_SpellScript);

            SpellCastResult CheckClass()
            {
                if (GetCaster()->getClass() != CLASS_DEATH_KNIGHT)
                {
                    SetCustomCastResultMessage(SPELL_CUSTOM_ERROR_MUST_BE_DEATH_KNIGHT);
                    return SPELL_FAILED_CUSTOM_ERROR;
                }
                return SPELL_CAST_OK;
            }

            void HandleAfterCast()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    _player->TeleportTo(0, 2359.41f, -5662.084f, 382.259f, 0.60f);
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_dk_death_gate_teleport_SpellScript::CheckClass);
                AfterCast += SpellCastFn(spell_dk_death_gate_teleport_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_death_gate_teleport_SpellScript();
        }
};

// Death Gate - 52751
class spell_dk_death_gate: public MS::Game::Scripting::Interfaces::SpellScriptLoader
{
    public:
        spell_dk_death_gate() : MS::Game::Scripting::Interfaces::SpellScriptLoader("spell_dk_death_gate") {}

        class spell_dk_death_gate_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_death_gate_SpellScript);

            SpellCastResult CheckClass()
            {
                if (GetCaster()->getClass() != CLASS_DEATH_KNIGHT)
                {
                    SetCustomCastResultMessage(SPELL_CUSTOM_ERROR_MUST_BE_DEATH_KNIGHT);
                    return SPELL_FAILED_CUSTOM_ERROR;
                }

                return SPELL_CAST_OK;
            }

            void HandleScript(SpellEffIndex effIndex)
            {
                PreventHitDefaultEffect(effIndex);
                if (Unit* target = GetHitUnit())
                    target->CastSpell(target, GetEffectValue(), false);
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_dk_death_gate_SpellScript::CheckClass);
                OnEffectHitTarget += SpellEffectFn(spell_dk_death_gate_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_death_gate_SpellScript();
        }
};

// Blood Boil - 50842
class spell_dk_blood_boil: public MS::Game::Scripting::Interfaces::SpellScriptLoader
{
    public:
        spell_dk_blood_boil() : MS::Game::Scripting::Interfaces::SpellScriptLoader("spell_dk_blood_boil") { }

        class spell_dk_blood_boil_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_blood_boil_SpellScript);

            int32 m_FrostFever  = 0;
            int32 m_BloodPlague = 0;

            void HandleBeforeCast()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (Player* l_Player = l_Caster->ToPlayer())
                    {
                        if (Unit* l_Target = l_Player->GetSelectedUnit())
                        {
                            if (!l_Player->IsValidAttackTarget(l_Target))
                                return;

                            if (AuraPtr l_AuraBloodPlague = l_Target->GetAura(DK_SPELL_BLOOD_PLAGUE, l_Caster->GetGUID()))
                                m_BloodPlague = l_AuraBloodPlague->GetDuration();

                            if (AuraPtr l_AuraFrostFever = l_Target->GetAura(DK_SPELL_FROST_FEVER, l_Caster->GetGUID()))
                                m_FrostFever = l_AuraFrostFever->GetDuration();
                        }
                    }
                }
            }

            void HandleHitTarget(SpellEffIndex /* effIndex */)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (Unit* l_Target = GetHitUnit())
                    {
                        if ((!l_Target->GetAura(DK_SPELL_FROST_FEVER, l_Caster->GetGUID()) || l_Target->GetAura(DK_SPELL_FROST_FEVER, l_Caster->GetGUID())->GetDuration() < m_FrostFever) && m_FrostFever > 0)
                        {
                            l_Caster->CastSpell(l_Target, DK_SPELL_FROST_FEVER, true);
                            if (AuraPtr l_AuraFrostFever = l_Target->GetAura(DK_SPELL_FROST_FEVER, l_Caster->GetGUID()))
                            {
                                // Don't Refresh diseases
                                if (!l_Caster->HasAura(DK_SPELL_SCENT_OF_BLOOD))
                                    l_AuraFrostFever->SetDuration(m_FrostFever);
                            }
                        }

                        // Blood plague
                        if ((!l_Target->GetAura(DK_SPELL_BLOOD_PLAGUE, l_Caster->GetGUID()) || l_Target->GetAura(DK_SPELL_BLOOD_PLAGUE, l_Caster->GetGUID())->GetDuration() < m_BloodPlague) && m_BloodPlague > 0)
                        {
                            l_Caster->CastSpell(l_Target, DK_SPELL_BLOOD_PLAGUE, true);
                            if (AuraPtr l_AuraBloodPlague = l_Target->GetAura(DK_SPELL_BLOOD_PLAGUE, l_Caster->GetGUID()))
                            {
                                // Don't Refresh diseases
                                if (!l_Caster->HasAura(DK_SPELL_SCENT_OF_BLOOD))
                                    l_AuraBloodPlague->SetDuration(m_BloodPlague);
                            }
                        }

                        l_Caster->CastSpell(l_Caster, DK_SPELL_BLOOD_BOIL_TRIGGERED, true);
                    }
                }
            }

            void HandleAfterCast()
            {
                if (Player* l_Player = GetCaster()->ToPlayer())
                {
                    if (l_Player->HasAura(DK_SPELL_SCENT_OF_BLOOD))
                        l_Player->CastSpell(l_Player, DK_SPELL_SCENT_OF_BLOOD_AURA, true);
                }
            }

            void Register()
            {
                BeforeCast += SpellCastFn(spell_dk_blood_boil_SpellScript::HandleBeforeCast);
                OnEffectHitTarget += SpellEffectFn(spell_dk_blood_boil_SpellScript::HandleHitTarget, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
                AfterCast += SpellCastFn(spell_dk_blood_boil_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_blood_boil_SpellScript();
        }
};

// Reaping - 56835
class spell_dk_reaping: public MS::Game::Scripting::Interfaces::SpellScriptLoader
{
    public:
        spell_dk_reaping() : MS::Game::Scripting::Interfaces::SpellScriptLoader("spell_dk_reaping") { }

        class spell_dk_reaping_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dk_reaping_AuraScript);

            void OnRemove(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (!GetCaster())
                    return;

                if (Player* l_Player = GetCaster()->ToPlayer())
                    l_Player->RemoveRunesBySpell(DK_SPELL_REAPING);
            }

            void Register()
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_dk_reaping_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dk_reaping_AuraScript();
        }
};

// Death Grip - 49560
class spell_dk_death_grip: public MS::Game::Scripting::Interfaces::SpellScriptLoader
{
    public:
        spell_dk_death_grip() : MS::Game::Scripting::Interfaces::SpellScriptLoader("spell_dk_death_grip") { }

        class spell_dk_death_grip_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_death_grip_SpellScript);

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                int32 damage = GetEffectValue();
                Position const* pos = GetExplTargetDest();
                if (Unit* target = GetHitUnit())
                {
                    if (!target->HasAuraType(SPELL_AURA_DEFLECT_SPELLS)) // Deterrence
                        target->CastSpell(pos->GetPositionX(), pos->GetPositionY(), pos->GetPositionZ(), damage, true);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_dk_death_grip_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_death_grip_SpellScript();
        }
};

// Corpse Explosion (Glyph) - 127344
class spell_dk_corpse_explosion: public MS::Game::Scripting::Interfaces::SpellScriptLoader
{
    public:
        spell_dk_corpse_explosion() : MS::Game::Scripting::Interfaces::SpellScriptLoader("spell_dk_corpse_explosion") { }

        class spell_dk_corpse_explosion_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_corpse_explosion_SpellScript);

            SpellCastResult CheckTarget()
            {
                // Any effect on Mechanical or Elemental units
                if (Unit* caster = GetCaster())
                {
                    Unit* target = GetExplTargetUnit();
                    if (!target)
                        return SPELL_FAILED_NO_VALID_TARGETS;

                    if (Creature* c = target->ToCreature())
                    {
                        if (c->GetCreatureTemplate() && (c->GetCreatureTemplate()->type == CREATURE_TYPE_MECHANICAL ||
                                                         c->GetCreatureTemplate()->type == CREATURE_TYPE_ELEMENTAL))
                            return SPELL_FAILED_BAD_TARGETS;
                        else if (c->IsDungeonBoss())
                            return SPELL_FAILED_BAD_TARGETS;
                    }
                    else if (target->GetGUID() == caster->GetGUID())
                        return SPELL_FAILED_BAD_TARGETS;

                    if (target->isAlive())
                        return SPELL_FAILED_BAD_TARGETS;
                }

                return SPELL_CAST_OK;
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_dk_corpse_explosion_SpellScript::CheckTarget);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_corpse_explosion_SpellScript();
        }
};

// Glyph of Corpse Explosion - 59336
class spell_dk_glyph_of_corpse_explosion: public MS::Game::Scripting::Interfaces::SpellScriptLoader
{
    public:
        spell_dk_glyph_of_corpse_explosion() : MS::Game::Scripting::Interfaces::SpellScriptLoader("spell_dk_glyph_of_corpse_explosion") { }

        class spell_dk_glyph_of_corpse_explosion_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dk_glyph_of_corpse_explosion_AuraScript);

            void OnApply(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Player* _player = GetTarget()->ToPlayer())
                    _player->learnSpell(DK_SPELL_GLYPH_OF_CORPSE_EXPLOSION, false);
            }

            void OnRemove(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Player* _player = GetTarget()->ToPlayer())
                    if (_player->HasSpell(DK_SPELL_GLYPH_OF_CORPSE_EXPLOSION))
                        _player->removeSpell(DK_SPELL_GLYPH_OF_CORPSE_EXPLOSION, false, false);
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_dk_glyph_of_corpse_explosion_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_dk_glyph_of_corpse_explosion_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dk_glyph_of_corpse_explosion_AuraScript();
        }
};

// Glyph of Horn of Winter - 58680
// Called by Horn of Winter - 57330
class spell_dk_glyph_of_horn_of_winter: public MS::Game::Scripting::Interfaces::SpellScriptLoader
{
    public:
        spell_dk_glyph_of_horn_of_winter() : MS::Game::Scripting::Interfaces::SpellScriptLoader("spell_dk_glyph_of_horn_of_winter") { }

        class spell_dk_glyph_of_horn_of_winter_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_glyph_of_horn_of_winter_SpellScript);

            void HandleAfterCast()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (!_player->isInCombat() && _player->HasAura(DK_SPELL_GLYPH_OF_HORN_OF_WINTER))
                        _player->CastSpell(_player, DK_SPELL_GLYPH_OF_HORN_OF_WINTER_EFFECT, true);
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_dk_glyph_of_horn_of_winter_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_glyph_of_horn_of_winter_SpellScript();
        }
};

// Icy touch 45477
class spell_dk_icy_touch: public MS::Game::Scripting::Interfaces::SpellScriptLoader
{
    public:
        spell_dk_icy_touch() : MS::Game::Scripting::Interfaces::SpellScriptLoader("spell_dk_icy_touch") { }

        class spell_dk_icy_touch_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_icy_touch_SpellScript);

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (!GetCaster() || !GetHitUnit())
                    return;

                GetCaster()->CastSpell(GetHitUnit(), DK_SPELL_FROST_FEVER, true);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_dk_icy_touch_SpellScript::HandleDummy, EFFECT_1, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_icy_touch_SpellScript();
        }
};

// Plaguebearer - 161497
// Called by Death Coil 47541 & Frost Strike 49143
class spell_dk_plaguebearer: public MS::Game::Scripting::Interfaces::SpellScriptLoader
{
    public:
        spell_dk_plaguebearer() : MS::Game::Scripting::Interfaces::SpellScriptLoader("spell_dk_plaguebearer") { }

        class spell_dk_plaguebearer_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_plaguebearer_SpellScript);

            void HandleOnHit()
            {
                if (Unit* l_Player = GetCaster())
                {
                    if (!l_Player->HasAura(DK_SPELL_PLAGUEBEARER))
                        return;

                    if (Unit* l_Target = GetHitUnit())
                    {
                        if (AuraPtr l_auraBloodPlague = GetHitUnit()->GetAura(DK_SPELL_BLOOD_PLAGUE, l_Player->GetGUID()))
                            l_auraBloodPlague->SetDuration(l_auraBloodPlague->GetDuration() + 4000);

                        if (AuraPtr l_AuraFrostFever = GetHitUnit()->GetAura(DK_SPELL_FROST_FEVER, l_Player->GetGUID()))
                            l_AuraFrostFever->SetDuration(l_AuraFrostFever->GetDuration() + 4000);

                        if (l_Player->HasAura(DK_SPELL_NECROTIC_PLAGUE))
                            l_Player->CastSpell(l_Target, DK_SPELL_NECROTIC_PLAGUE_APPLY_AURA);
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_dk_plaguebearer_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dk_plaguebearer_SpellScript();
        }
};

// Necrotic Plague - 155159
class spell_dk_necrotic_plague_aura: public MS::Game::Scripting::Interfaces::SpellScriptLoader
{
    public:
        spell_dk_necrotic_plague_aura() : MS::Game::Scripting::Interfaces::SpellScriptLoader("spell_dk_necrotic_plague_aura") { }

        class spell_dk_necrotic_plague_aura_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dk_necrotic_plague_aura_AuraScript);

            void OnTick(constAuraEffectPtr aurEff)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (Unit* l_Target = GetTarget())
                    {
                        if (AuraPtr l_AuraNecroticPlague = l_Target->GetAura(DK_SPELL_NECROTIC_PLAGUE_APPLY_AURA, l_Caster->GetGUID()))
                            l_AuraNecroticPlague->ModStackAmount(1);

                        if (Unit* l_NewTarget = l_Caster->SelectNearbyTarget(l_Target, 8.0f))
                            l_Caster->CastSpell(l_NewTarget, DK_SPELL_NECROTIC_PLAGUE_APPLY_AURA, true);
                    }
                }
            }

            bool CanRefreshProcDummy()
            {
                return false;
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_dk_necrotic_plague_aura_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
                CanRefreshProc += AuraCanRefreshProcFn(spell_dk_necrotic_plague_aura_AuraScript::CanRefreshProcDummy);
            }
        };

    AuraScript* GetAuraScript() const
    {
        return new spell_dk_necrotic_plague_aura_AuraScript();
    }
};

// Runic Empowerment - 81229
class PlayerScript_Runic_Empowerment: public MS::Game::Scripting::Interfaces::PlayerScript
{
    public:
        PlayerScript_Runic_Empowerment() :PlayerScript("PlayerScript_Runic_Empowerment") {}

        void OnModifyPower(Player* p_Player, Powers p_Power, int32 p_Value)
        {
            if (p_Player->getClass() != CLASS_DEATH_KNIGHT || p_Power != POWER_RUNIC_POWER || !p_Player->HasAura(DK_SPELL_RUNIC_EMPOWERMENT))
                return;

            // Only on use runic power
            if (p_Value > 0)
                return;

            if (AuraEffectPtr l_RunicEmpowerment = p_Player->GetAuraEffect(DK_SPELL_RUNIC_EMPOWERMENT, EFFECT_0))
            {
                // 1.50% chance per Runic Power spent
                float l_Amount = l_RunicEmpowerment->GetAmount();
                l_Amount /= 100.f;

                float l_Chance = l_Amount * (((float)-p_Value) / 10.f);

                if (roll_chance_f(l_Chance))
                {
                    std::list<uint8> l_LstRunesUsed;

                    for (uint8 i = 0; i < MAX_RUNES; ++i)
                    {
                        if (p_Player->GetRuneCooldown(i))
                            l_LstRunesUsed.push_back(i);
                    }

                    if (l_LstRunesUsed.empty())
                        return;

                    uint8 l_RuneRandom = JadeCore::Containers::SelectRandomContainerElement(l_LstRunesUsed);

                    p_Player->SetRuneCooldown(l_RuneRandom, 0);
                    p_Player->ResyncRunes(MAX_RUNES);
                }
            }
        }
};

// Runic Corruption - 51462
class PlayerScript_Corrupion_Runic: public MS::Game::Scripting::Interfaces::PlayerScript
{
    public:
        PlayerScript_Corrupion_Runic() :PlayerScript("PlayerScript_Corrupion_Runic") {}

        void OnModifyPower(Player* p_Player, Powers p_Power, int32 p_Value)
        {
            if (p_Player->getClass() != CLASS_DEATH_KNIGHT || p_Power != POWER_RUNIC_POWER || !p_Player->HasAura(DK_SPELL_RUNIC_CORRUPTION_AURA))
                return;

            // Only on use runic power
            if (p_Value > 0)
                return;

            if (AuraEffectPtr l_RunicCorruption = p_Player->GetAuraEffect(DK_SPELL_RUNIC_CORRUPTION_AURA, EFFECT_1))
            {
                // 1.50% chance per Runic Power spent
                float l_Amount = l_RunicCorruption->GetAmount();
                l_Amount /= 100.f;

                float l_Chance = l_Amount * (((float)-p_Value) / 10.f);

                if (roll_chance_f(l_Chance))
                    p_Player->CastSpell(p_Player, DK_SPELL_RUNIC_CORRUPTION, true);
            }
        }
};

// Death Pact - 48743
class spell_dk_death_pact: public MS::Game::Scripting::Interfaces::SpellScriptLoader
{
    public:
        spell_dk_death_pact() : MS::Game::Scripting::Interfaces::SpellScriptLoader("spell_dk_death_pact") { }

        class spell_dk_death_pact_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dk_death_pact_AuraScript);

            void CalculateAmount(constAuraEffectPtr /*p_AurEff*/, int32& p_Amount, bool& /*p_CanBeRecalculated*/)
            {
                if (Unit* l_Caster = GetCaster())
                    p_Amount = l_Caster->CountPctFromMaxHealth(p_Amount);
            }

            void OnAbsorb(AuraEffectPtr /*p_AurEff*/, DamageInfo& p_DmgInfo, uint32& p_AbsorbAmount)
            {
                if (Unit* l_Target = GetTarget())
                {
                    if (p_DmgInfo.GetSpellInfo() && p_DmgInfo.GetSpellInfo()->Id == GetSpellInfo()->Id)
                        p_AbsorbAmount = 0;
                }
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_death_pact_AuraScript::CalculateAmount, EFFECT_1, SPELL_AURA_SCHOOL_HEAL_ABSORB);
                OnEffectAbsorb += AuraEffectAbsorbFn(spell_dk_death_pact_AuraScript::OnAbsorb, EFFECT_1, SPELL_AURA_SCHOOL_HEAL_ABSORB);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dk_death_pact_AuraScript();
        }
};

// Chilblains - 50041
class spell_dk_chilblains_aura: public MS::Game::Scripting::Interfaces::SpellScriptLoader
{
    public:
        spell_dk_chilblains_aura() : MS::Game::Scripting::Interfaces::SpellScriptLoader("spell_dk_chilblains_aura") { }

        class spell_dk_chilblains_aura_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dk_chilblains_aura_AuraScript);

            void OnProc(constAuraEffectPtr aurEff, ProcEventInfo& p_EventInfo)
            {
                PreventDefaultAction();

                if (Unit* l_Caster = GetCaster())
                {
                    if (Unit* l_Target = p_EventInfo.GetProcTarget())
                    {
                        if (!p_EventInfo.GetDamageInfo()->GetSpellInfo())
                            return;

                        if (p_EventInfo.GetDamageInfo()->GetSpellInfo()->Id == DK_SPELL_FROST_FEVER || p_EventInfo.GetDamageInfo()->GetSpellInfo()->Id == DK_SPELL_CHAINS_OF_ICE
                            || p_EventInfo.GetDamageInfo()->GetSpellInfo()->Id == DK_SPELL_ICY_TOUCH)
                            l_Caster->CastSpell(l_Target, DK_SPELL_CHILBLAINS_TRIGGER, true);
                    }
                }
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_dk_chilblains_aura_AuraScript::OnProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dk_chilblains_aura_AuraScript();
        }
};

void AddSC_deathknight_spell_scripts()
{
    new spell_dk_death_and_decay();
    new spell_dk_death_barrier();
    new spell_dk_plague_strike();
    new spell_dk_gorefiends_grasp();
    new spell_dk_dark_transformation_form();
    new spell_dk_desecrated_ground();
    new spell_dk_festering_strike();
    new spell_dk_death_strike_heal();
    new spell_dk_howling_blast();
    new spell_dk_conversion();
    new spell_dk_remorseless_winter();
    new spell_dk_soul_reaper();
    new spell_dk_pillar_of_frost();
    new spell_dk_blood_tap();
    new spell_dk_death_siphon();
    new spell_dk_improved_blood_presence();
    new spell_dk_unholy_presence();
    new spell_dk_death_strike();
    new spell_dk_purgatory();
    new spell_dk_purgatory_absorb();
    new spell_dk_plague_leech();
    new spell_dk_unholy_blight();
    new spell_dk_chilblains();
    new spell_dk_outbreak();
    new spell_dk_raise_dead();
    new spell_dk_anti_magic_shell_raid();
    new spell_dk_anti_magic_shell_self();
    new spell_dk_anti_magic_zone();
    new spell_dk_death_gate_teleport();
    new spell_dk_death_gate();
    new spell_dk_blood_boil();
    new spell_dk_death_grip();
    new spell_dk_corpse_explosion();
    new spell_dk_glyph_of_corpse_explosion();
    new spell_dk_glyph_of_horn_of_winter();
    new spell_dk_icy_touch();
    new spell_dk_plaguebearer();
    new spell_dk_necrotic_plague_aura();
    new spell_dk_death_pact();
    new spell_dk_chilblains_aura();
    new spell_dk_reaping();

    /// Player script
    new PlayerScript_Blood_Tap();
    new PlayerScript_Runic_Empowerment();
    new PlayerScript_Corrupion_Runic();
}