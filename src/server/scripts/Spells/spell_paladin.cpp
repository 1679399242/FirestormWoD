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
 * Scripts for spells with SPELLFAMILY_PALADIN and SPELLFAMILY_GENERIC spells used by paladin players.
 * Ordered alphabetically using scriptname.
 * Scriptnames of files in this file should be prefixed with "spell_pal_".
 */

#include "ScriptMgr.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "GridNotifiers.h"

enum PaladinSpells
{
    PALADIN_SPELL_JUDGMENT                      = 20271,
    PALADIN_SPELL_JUDGMENTS_OF_THE_WISE         = 105424,
    PALADIN_SPELL_TEMPLARS_VERDICT              = 85256,
    PALADIN_SPELL_PHYSICAL_VULNERABILITY        = 81326,
    PALADIN_SPELL_LONG_ARM_OF_THE_LAW           = 87172,
    PALADIN_SPELL_LONG_ARM_OF_THE_LAW_RUN_SPEED = 87173,
    PALADIN_SPELL_HOLY_SHOCK_R1                 = 20473,
    PALADIN_SPELL_HOLY_SHOCK_R1_DAMAGE          = 25912,
    PALADIN_SPELL_HOLY_SHOCK_R1_HEALING         = 25914,
    PALADIN_SPELL_HOLY_SHOCK_ENERGIZE           = 148976,
    SPELL_BLESSING_OF_LOWER_CITY_DRUID          = 37878,
    SPELL_BLESSING_OF_LOWER_CITY_PALADIN        = 37879,
    SPELL_BLESSING_OF_LOWER_CITY_PRIEST         = 37880,
    SPELL_BLESSING_OF_LOWER_CITY_SHAMAN         = 37881,
    SPELL_DIVINE_STORM                          = 53385,
    SPELL_DIVINE_STORM_DUMMY                    = 54171,
    SPELL_DIVINE_STORM_HEAL                     = 54172,
    SPELL_FORBEARANCE                           = 25771,
    PALADIN_SPELL_WORD_OF_GLORY                 = 85673,
    PALADIN_SPELL_WORD_OF_GLORY_HEAL            = 130551,
    PALADIN_SPELL_GLYPH_OF_WORD_OF_GLORY        = 54936,
    PALADIN_SPELL_GLYPH_OF_WORD_OF_GLORY_DAMAGE = 115522,
    PALADIN_SPELL_GLYPH_OF_HARSH_WORDS          = 54938,
    PALADIN_SPELL_HARSH_WORDS_DAMAGE            = 130552,
    PALADIN_SPELL_CONSECRATION_AREA_DUMMY       = 81298,
    PALADIN_SPELL_CONSECRATION_DAMAGE           = 81297,
    PALADIN_SPELL_HOLY_PRISM_ALLIES             = 114871,
    PALADIN_SPELL_HOLY_PRISM_ENNEMIES           = 114852,
    PALADIN_SPELL_HOLY_PRISM_DAMAGE_VISUAL      = 114862,
    PALADIN_SPELL_HOLY_PRISM_DAMAGE_VISUAL_2    = 114870,
    PALADIN_SPELL_HOLY_PRISM_HEAL_VISUAL        = 121551,
    PALADIN_SPELL_HOLY_PRISM_HEAL_VISUAL_2      = 121552,
    PALADIN_SPELL_ARCING_LIGHT_HEAL             = 119952,
    PALADIN_SPELL_ARCING_LIGHT_DAMAGE           = 114919,
    PALADIN_SPELL_EXECUTION_SENTENCE            = 114916,
    PALADIN_SPELL_STAY_OF_EXECUTION             = 114917,
    PALADIN_SPELL_EXECUTION_DISPEL_DAMAGE       = 146585,
    PALADIN_SPELL_EXECUTION_DISPEL_HEAL         = 146586,
    PALADIN_SPELL_GLYPH_OF_BLINDING_LIGHT       = 54934,
    PALADIN_SPELL_BLINDING_LIGHT_CONFUSE        = 105421,
    PALADIN_SPELL_BLINDING_LIGHT_STUN           = 115752,
    PALADIN_SPELL_EXORCISM                      = 879,
    PALADIN_SPELL_MASS_EXORCISM                 = 122032,
    PALADIN_SPELL_SACRED_SHIELD                 = 65148,
    PALADIN_SPELL_ARDENT_DEFENDER_HEAL          = 66235,
    PALADIN_SPELL_TOWER_OF_RADIANCE_ENERGIZE    = 88852,
    PALADIN_SPELL_BEACON_OF_LIGHT               = 53563,
    PALADIN_SPELL_BEACON_OF_FAITH               = 156910,
    PALADIN_SPELL_SELFLESS_HEALER_STACK         = 114250,
    PALADIN_SPELL_SELFLESS_HEALER               = 85804,
    PALADIN_SPELL_SHIELD_OF_THE_RIGHTEOUS_PROC  = 132403,
    PALADIN_SPELL_BASTION_OF_GLORY              = 114637,
    PALADIN_SPELL_DIVINE_PURPOSE_AURA           = 90174,
    PALADIN_SPELL_DIVINE_SHIELD                 = 642,
    PALADIN_SPELL_LAY_ON_HANDS                  = 633,
    PALADIN_SPELL_DIVINE_PROTECTION             = 498,
    PALADIN_SPELL_GLYPH_OF_AVENGING_WRATH       = 54927,
    PALADIN_SPELL_AVENGING_WRATH_REGEN_BY_GLYPH = 115547,
    PALADIN_SPELL_SACRED_CLEANSING              = 53551,
    PALADIN_SPELL_DAYBREAK_AURA                 = 88821,
    PALADIN_SPELL_DAYBREAK_PROC                 = 88819,
    PALADIN_SPELL_DAYBREAK_HEAL                 = 121129,
    PALADIN_SPELL_TOWER_OF_RADIANCE             = 85512,
    PALADIN_SPELL_HOLY_AVENGER                  = 105809,
    PALADIN_ITEM_PVP_HOLY_4P_BONUS              = 131665,
    PALADIN_SPELL_GLYPH_OF_DIVINE_STORM         = 63220,
    PALADIN_SPELL_GLYPH_OF_DIVINE_STORM_HEAL    = 115515,
    PALADIN_SPELL_GLYPH_OF_DENOUNCE             = 56420,
    PALADIN_SPELL_GLYPH_OF_DENOUNCE_PROC        = 115654,
    PALADIN_SPELL_GLYPH_OF_DEVOTION_AURA        = 146955,
    PALADIN_SPELL_SANCTIFIED_WRATH_TALENT       = 53376,
    PALADIN_SPELL_SANCTIFIED_WRATH_BONUS        = 114232,
    PALADIN_SPELL_AVENGING_WRATH                = 31884,
    PALADIN_SPELL_AVENGING_WRATH_HEAL           = 31842,
    PALADIN_SPELL_EXORCISM_ENERGIZE             = 147715,
    PALADIN_SPELL_GLYPH_OF_BURDEN_OF_GUILT      = 54931,
    PALADIN_SPELL_BURDEN_OF_GUILD               = 110300,
    PALADIN_SPELL_WEAKENED_BLOWS                = 115798,
    PALADIN_SPELL_SEAL_OF_JUSTICE               = 20164,
    PALADIN_SPELL_SEAL_OF_INSIHT                = 20165,
    PALADIN_SPELL_SEAL_OF_RIGHTEOUSNESS         = 20154,
    PALADIN_SPELL_SEAL_OF_TRUTH                 = 31801,
    PALADIN_SPELL_EMPOWERED_SEALS               = 152263,
    PALADIN_SPELL_TURALYONS_JUSTICE             = 156987,
    PALADIN_SPELL_UTHERS_INSIGHT                = 156988,
    PALADIN_SPELL_LIADRINS_RIGHTEOUSNESS        = 156989,
    PALADIN_SPELL_MARAADS_TRUTH                 = 156990,
    PALADIN_SPELL_ETERNAL_FLAME_PERIODIC_HEAL   = 156322,
    PALADIN_SPELL_ETERNAL_FLAME                 = 114163,
    PALADIN_SPELL_HAMMER_OF_WRATH               = 24275,
    PALADIN_SPELL_SANCTIFIED_WRATH_PROTECTION   = 171648,
    PALADIN_SPELL_EMPOWERED_DIVINE_STORM        = 174718,
    PALADIN_SPELL_DIVINE_CRUSADER               = 144595,
    PALADIN_ENHANCED_HOLY_SHOCK_PROC            = 160002,
    PALADIN_SPELL_SHIELD_OF_THE_RIGHTEOUS       = 53600,
    PALADIN_SPELL_LIGHT_OF_DAWN                 = 85222,
    PALADIN_SPELL_DIVINE_PURPOSE                = 86172,
    PALADIN_SPELL_HAMMER_OF_WRATH_POWER         = 141459,
    PALADIN_SPELL_SAVED_BY_THE_LIGHT            = 157047,
    PALADIN_SPELL_SAVED_BY_THE_LIGHT_PROC       = 157131,
    PALADIN_SPELL_SAVED_BY_THE_LIGHT_SHIELD     = 157128,
    PALADIN_SPELL_GLYPH_OF_TEMPLAR_VERDICT      = 54926,
    PALADIN_SPELL_GLYPH_OF_TEMPLAR_VERDICT_PROC = 115668,
    PALADIN_SPELL_GLYPH_OF_DIVINE_SHIELD        = 146956,
    PALADIN_SPELL_IMPROVED_DAYBREAK             = 157455,
    PALADIN_SPELL_FLASH_OF_LIGHT                = 19750,
    PALADIN_SPELL_HOLY_LIGHT                    = 13952,
    PALADIN_NPC_LIGHTS_HAMMER                   = 59738,
    PALADIN_SPELL_LIGHTS_HAMMER_TICK            = 114918,
    PALADIN_PVP_RETRIBUTION_2P_BONUS            = 165886,
    PALADIN_SPELL_RIGHTEOUS_DETERMINATION       = 165889,
    PALADIN_PVP_RETRIBUTION_4P_BONUS            = 165895,
    PALADIN_VINDICATORS_FURY                    = 165903
};

// Glyph of devotion aura - 146955
class spell_pal_glyph_of_devotian_aura: public SpellScriptLoader
{
    public:
        spell_pal_glyph_of_devotian_aura() : SpellScriptLoader("spell_pal_glyph_of_devotian_aura") { }

        class spell_pal_glyph_of_devotian_aura_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_glyph_of_devotian_aura_SpellScript);

            bool Load()
            {
                return true;
            }

            void CountTargets(std::list<WorldObject*>& targets)
            {
                if (GetCaster() && GetCaster()->HasAura(PALADIN_SPELL_GLYPH_OF_DEVOTION_AURA))
                {
                    targets.clear();
                    targets.push_back(GetCaster());
                }
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_pal_glyph_of_devotian_aura_SpellScript::CountTargets, EFFECT_0, TARGET_UNIT_CASTER_AREA_RAID);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_glyph_of_devotian_aura_SpellScript();
        }
};

class spell_pal_glyph_of_devotian_trigger_aura: public SpellScriptLoader
{
    public:
        spell_pal_glyph_of_devotian_trigger_aura() : SpellScriptLoader("spell_pal_glyph_of_devotian_trigger_aura") { }

        class spell_pal_glyph_of_devotian_aura_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_glyph_of_devotian_aura_SpellScript);

            bool Load()
            {
                return true;
            }

            void CountTargets(std::list<WorldObject*>& targets)
            {
                if (GetCaster() && GetCaster()->HasAura(PALADIN_SPELL_GLYPH_OF_DEVOTION_AURA))
                {
                    targets.clear();
                    targets.push_back(GetCaster());
                }
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_pal_glyph_of_devotian_aura_SpellScript::CountTargets, EFFECT_0, TARGET_UNIT_CASTER_AREA_RAID);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_pal_glyph_of_devotian_aura_SpellScript::CountTargets, EFFECT_1, TARGET_UNIT_CASTER_AREA_RAID);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_pal_glyph_of_devotian_aura_SpellScript::CountTargets, EFFECT_2, TARGET_UNIT_CASTER_AREA_RAID);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_glyph_of_devotian_aura_SpellScript();
        }
};

// Hammer of Wrath - 24275 - 158392 - 157496
class spell_pal_hammer_of_wrath: public SpellScriptLoader
{
public:
    spell_pal_hammer_of_wrath() : SpellScriptLoader("spell_pal_hammer_of_wrath") { }

    class spell_pal_hammer_of_wrath_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pal_hammer_of_wrath_SpellScript);

        void HandleAfterCast()
        {
            if (Player* l_Player = GetCaster()->ToPlayer())
                if (l_Player->GetSpecializationId(l_Player->GetActiveSpec()) == SPEC_PALADIN_RETRIBUTION)
                    l_Player->CastSpell(l_Player, PALADIN_SPELL_HAMMER_OF_WRATH_POWER, true);
        }

        void Register()
        {
            AfterCast += SpellCastFn(spell_pal_hammer_of_wrath_SpellScript::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_pal_hammer_of_wrath_SpellScript();
    }
};

/// Exorcism - 879, Mass Exorcism - 122032
class spell_pal_exorcism_energize: public SpellScriptLoader
{
    public:
        spell_pal_exorcism_energize() : SpellScriptLoader("spell_pal_exorcism_energize") { }

        class spell_pal_exorcism_energize_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_exorcism_energize_SpellScript);

            void HandleAfterCast()
            {
                Player* l_Player = GetCaster()->ToPlayer();

                if (l_Player == nullptr)
                    return;

                if (l_Player->GetSpecializationId(l_Player->GetActiveSpec()) != SPEC_PALADIN_RETRIBUTION)
                    return;
                
                l_Player->CastSpell(l_Player, PALADIN_SPELL_EXORCISM_ENERGIZE, true);
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_pal_exorcism_energize_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_exorcism_energize_SpellScript();
        }
};

// Called by Divine Storm - 53385
// Glyph of Divine Storm - 63220
class spell_pal_glyph_of_divine_storm: public SpellScriptLoader
{
    public:
        spell_pal_glyph_of_divine_storm() : SpellScriptLoader("spell_pal_glyph_of_divine_storm") { }

        class spell_pal_glyph_of_divine_storm_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_glyph_of_divine_storm_SpellScript);

            void HandleAfterCast()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (_player->HasAura(PALADIN_SPELL_GLYPH_OF_DIVINE_STORM))
                        _player->CastSpell(_player, PALADIN_SPELL_GLYPH_OF_DIVINE_STORM_HEAL, true);
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_pal_glyph_of_divine_storm_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_glyph_of_divine_storm_SpellScript();
        }
};

// Holy Radiance - 82327
class spell_pal_daybreak: public SpellScriptLoader
{
    public:
        spell_pal_daybreak() : SpellScriptLoader("spell_pal_daybreak") { }

        class spell_pal_daybreak_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_daybreak_SpellScript);

            uint32 m_TargetCount;
            uint64 m_TargetId;

            enum Constants : int32
            {
                FullHealTargetCount = 6, ///< After this amount of targets the spell will lose BasePoints% efficiency for each target
                HealDecrease        = 5, ///< % of heal lost per target over FullHealTargetCount
                MinimalHeal         = 5, ///< Minimal heal % applied on each target when a lot of target are hit
            };

            void FilterTargets(std::list<WorldObject*>& p_Targets)
            {
                m_TargetCount = p_Targets.size();

                if (!GetCaster())
                    return;
            }

            void HandleBeforeCast()
            {
                Unit* l_Target = GetExplTargetUnit();
                Unit* l_Caster = GetCaster();

                if (!l_Caster || !l_Target)
                    return;

                m_TargetId = l_Target->GetGUID();
            }

            void HandleOnHit()
            {
                Unit* l_Target = GetHitUnit();
                Unit* l_Caster = GetCaster();

                if (!l_Caster || !l_Target || !m_TargetId)  ///< m_TargetId can't be uninitialized if the caster and the target are valid
                    return;

                int32 l_HealValue = GetHitHeal();
                if (l_Target->GetGUID() != m_TargetId)
                {
                    int32 l_Pct = GetSpellInfo()->Effects[EFFECT_0].BasePoints;
                    if (m_TargetCount > Constants::FullHealTargetCount)
                        l_Pct = std::max(l_Pct - 5 * Constants::HealDecrease, static_cast<int32>(Constants::MinimalHeal));

                    ApplyPct(l_HealValue, l_Pct);
                }

                SetHitHeal(l_HealValue);
            }

            void HandleAfterCast()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (l_Caster->HasAura(PALADIN_SPELL_DAYBREAK_AURA))
                        l_Caster->CastSpell(l_Caster, PALADIN_SPELL_DAYBREAK_PROC, true);

                    l_Caster->CastSpell(l_Caster, PALADIN_SPELL_EXORCISM_ENERGIZE, true);
                }
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_pal_daybreak_SpellScript::HandleAfterCast);
                BeforeCast += SpellCastFn(spell_pal_daybreak_SpellScript::HandleBeforeCast);
                OnHit += SpellHitFn(spell_pal_daybreak_SpellScript::HandleOnHit);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_pal_daybreak_SpellScript::FilterTargets, EFFECT_1, TARGET_UNIT_DEST_AREA_ALLY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_daybreak_SpellScript();
        }
};

// Hand of Purity - 114039
class spell_pal_hand_of_purity: public SpellScriptLoader
{
    public:
        spell_pal_hand_of_purity() : SpellScriptLoader("spell_pal_hand_of_purity") { }

        class spell_pal_hand_of_purity_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_pal_hand_of_purity_AuraScript);

            uint32 healtPct;

            bool Load()
            {
                healtPct = GetSpellInfo()->Effects[EFFECT_0].CalcValue(GetCaster());
                return GetUnitOwner()->ToPlayer();
            }

            void CalculateAmount(constAuraEffectPtr /*auraEffect*/, int32& amount, bool& /*canBeRecalculated*/)
            {
                amount = -1;
            }

            void Absorb(AuraEffectPtr /*auraEffect*/, DamageInfo& dmgInfo, uint32& absorbAmount)
            {
                Unit* target = GetTarget();
                if (!target)
                    return;

                SpellInfo const* procSpell = dmgInfo.GetSpellInfo();
                if (!procSpell)
                    return;

                if (!procSpell->HasAura(SPELL_AURA_PERIODIC_DAMAGE) && !procSpell->HasAura(SPELL_AURA_PERIODIC_DAMAGE_PERCENT))
                    return;

                absorbAmount = CalculatePct(dmgInfo.GetDamage(), healtPct);
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_pal_hand_of_purity_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
                OnEffectAbsorb += AuraEffectAbsorbFn(spell_pal_hand_of_purity_AuraScript::Absorb, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_pal_hand_of_purity_AuraScript();
        }
};

// Called by Avenging Wrath - 31884
// Glyph of Avenging Wrath - 54927
class spell_pal_glyph_of_avenging_wrath: public SpellScriptLoader
{
    public:
        spell_pal_glyph_of_avenging_wrath() : SpellScriptLoader("spell_pal_glyph_of_avenging_wrath") { }

        class spell_pal_glyph_of_avenging_wrath_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_glyph_of_avenging_wrath_SpellScript);

            void HandleOnHit()
            {
                Player* l_Player = GetCaster()->ToPlayer();

                if (l_Player == nullptr)
                    return;

                if (l_Player->HasAura(PALADIN_SPELL_GLYPH_OF_AVENGING_WRATH))
                    l_Player->CastSpell(l_Player, PALADIN_SPELL_AVENGING_WRATH_REGEN_BY_GLYPH, true);

                if (l_Player->HasAura(PALADIN_PVP_RETRIBUTION_2P_BONUS))
                    l_Player->CastSpell(l_Player, PALADIN_SPELL_RIGHTEOUS_DETERMINATION, true);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_pal_glyph_of_avenging_wrath_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_glyph_of_avenging_wrath_SpellScript();
        }
};

// Shield of the Righteous - 53600
class spell_pal_shield_of_the_righteous: public SpellScriptLoader
{
    public:
        spell_pal_shield_of_the_righteous() : SpellScriptLoader("spell_pal_shield_of_the_righteous") { }

        class spell_pal_shield_of_the_righteous_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_shield_of_the_righteous_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* unitTarget = GetHitUnit())
                    {
                        // -20% damage taken for 3s
                        _player->CastSpell(_player, PALADIN_SPELL_SHIELD_OF_THE_RIGHTEOUS_PROC, true);
                        _player->CastSpell(_player, PALADIN_SPELL_BASTION_OF_GLORY, true);
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_pal_shield_of_the_righteous_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_shield_of_the_righteous_SpellScript();
        }
};

// Selfless healer - 85804
// Called by flash of light - 19750
class spell_pal_selfless_healer: public SpellScriptLoader
{
    public:
        spell_pal_selfless_healer() : SpellScriptLoader("spell_pal_selfless_healer") { }

        class spell_pal_selfless_healer_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_selfless_healer_SpellScript);

            void HandleHeal(SpellEffIndex /*l_EffIndex*/)
            {
                if (Player* l_Caster = GetCaster()->ToPlayer())
                {
                    if (Unit* l_Target = GetHitUnit())
                    {
                        if (l_Caster->HasAura(PALADIN_SPELL_SELFLESS_HEALER))
                            l_Caster->CastSpell(l_Caster, PALADIN_SPELL_SELFLESS_HEALER_STACK, true);
                        if (l_Caster->HasAura(PALADIN_SPELL_SELFLESS_HEALER_STACK))
                        {
                            int32 l_Charges = l_Caster->GetAura(PALADIN_SPELL_SELFLESS_HEALER_STACK)->GetStackAmount();

                            if (l_Caster->IsValidAssistTarget(l_Target) && l_Target != l_Caster)
                            {
                                if (l_Caster->GetSpecializationId(l_Caster->GetActiveSpec()) == SPEC_PALADIN_HOLY)
                                    SetHitHeal(int32(GetHitHeal() + ((GetHitHeal() * sSpellMgr->GetSpellInfo(PALADIN_SPELL_SELFLESS_HEALER_STACK)->Effects[EFFECT_3].BasePoints / 100) * l_Charges)));
                                else
                                    SetHitHeal(int32(GetHitHeal() + ((GetHitHeal() * sSpellMgr->GetSpellInfo(PALADIN_SPELL_SELFLESS_HEALER_STACK)->Effects[EFFECT_1].BasePoints / 100) * l_Charges)));
                            }
                        }
                    }
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_pal_selfless_healer_SpellScript::HandleHeal, EFFECT_0, SPELL_EFFECT_HEAL);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_selfless_healer_SpellScript();
        }
};

// Called by Flash of Light - 19750 and Divine Light - 82326
// Tower of Radiance - 85512
class spell_pal_tower_of_radiance: public SpellScriptLoader
{
    public:
        spell_pal_tower_of_radiance() : SpellScriptLoader("spell_pal_tower_of_radiance") { }

        class spell_pal_tower_of_radiance_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_tower_of_radiance_SpellScript);

            int32 m_TotalManaBeforeCast = 0;

            void HandleBeforeCast()
            {
                if (Unit* l_Caster = GetCaster())
                    m_TotalManaBeforeCast = l_Caster->GetPower(POWER_MANA);
            }

            void HandleOnHit()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (Unit* l_Target = GetHitUnit())
                    {
                        // Item - Paladin PvP Set Holy 4P Bonus
                        if (l_Caster->HasAura(PALADIN_ITEM_PVP_HOLY_4P_BONUS))
                            l_Caster->CastSpell(l_Caster, PALADIN_SPELL_EXORCISM_ENERGIZE, true);
                    }
                }
            }

            void HandleAfterCast()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (Unit* l_Target = GetExplTargetUnit())
                    {
                        if (l_Target->HasAura(PALADIN_SPELL_BEACON_OF_LIGHT, l_Caster->GetGUID()) || l_Target->HasAura(PALADIN_SPELL_BEACON_OF_FAITH, l_Caster->GetGUID()))
                        {
                            if (m_TotalManaBeforeCast && m_TotalManaBeforeCast > l_Caster->GetPower(POWER_MANA))
                            {
                                if (GetSpellInfo()->Id == PALADIN_SPELL_FLASH_OF_LIGHT)
                                    l_Caster->EnergizeBySpell(l_Caster, PALADIN_SPELL_TOWER_OF_RADIANCE_ENERGIZE, CalculatePct(m_TotalManaBeforeCast - l_Caster->GetPower(POWER_MANA), sSpellMgr->GetSpellInfo(PALADIN_SPELL_TOWER_OF_RADIANCE_ENERGIZE)->Effects[EFFECT_0].BasePoints), POWER_MANA);
                                else
                                    l_Caster->EnergizeBySpell(l_Caster, PALADIN_SPELL_TOWER_OF_RADIANCE_ENERGIZE, CalculatePct(m_TotalManaBeforeCast - l_Caster->GetPower(POWER_MANA), sSpellMgr->GetSpellInfo(PALADIN_SPELL_TOWER_OF_RADIANCE_ENERGIZE)->Effects[EFFECT_0].BasePoints), POWER_MANA);
                            }
                        }
                    }
                }
            }

            void Register()
            {
                BeforeCast += SpellCastFn(spell_pal_tower_of_radiance_SpellScript::HandleBeforeCast);
                OnHit += SpellHitFn(spell_pal_tower_of_radiance_SpellScript::HandleOnHit);
                AfterCast += SpellCastFn(spell_pal_tower_of_radiance_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_tower_of_radiance_SpellScript();
        }
};

// Sacred shield - 20925 and Sacred Shield (Holy) - 148039
class spell_pal_sacred_shield: public SpellScriptLoader
{
    public:
        spell_pal_sacred_shield() : SpellScriptLoader("spell_pal_sacred_shield") { }

        class spell_pal_sacred_shield_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_pal_sacred_shield_AuraScript);

            void OnTick(constAuraEffectPtr aurEff)
            {
                if (Unit* _player = GetCaster())
                    if (Unit* target = GetTarget())
                            _player->CastSpell(target, PALADIN_SPELL_SACRED_SHIELD, true);
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_pal_sacred_shield_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_pal_sacred_shield_AuraScript();
        }
};

// Sacred shield absorb - 65148
class spell_pal_sacred_shield_absorb: public SpellScriptLoader
{
    public:
        spell_pal_sacred_shield_absorb() : SpellScriptLoader("spell_pal_sacred_shield_absorb") { }

        class spell_pal_sacred_shield_absorb_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_pal_sacred_shield_absorb_AuraScript);

            void CalculateAmount(constAuraEffectPtr , int32 & amount, bool & )
            {
                if (GetCaster())
                    amount = int32(1 + GetCaster()->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_ALL) * 1.306f);
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_pal_sacred_shield_absorb_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_pal_sacred_shield_absorb_AuraScript();
        }
};

// Emancipate - 121783
class spell_pal_emancipate: public SpellScriptLoader
{
    public:
        spell_pal_emancipate() : SpellScriptLoader("spell_pal_emancipate") { }

        class spell_pal_emancipate_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_emancipate_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    std::list<AuraPtr> auraList;

                    for (auto itr : _player->GetAppliedAuras())
                    {
                        AuraPtr aura = itr.second->GetBase();
                        if (aura && aura->GetSpellInfo()->GetAllEffectsMechanicMask() & ((1<<MECHANIC_SNARE)|(1<<MECHANIC_ROOT)))
                            auraList.push_back(aura);
                    }

                    if (!auraList.empty())
                    {
                        JadeCore::Containers::RandomResizeList(auraList, 1);
                        _player->RemoveAura(*auraList.begin());
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_pal_emancipate_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_emancipate_SpellScript();
        }
};

// Exorcism!  - 59578
class spell_pal_art_of_war: public SpellScriptLoader
{
    public:
        spell_pal_art_of_war() : SpellScriptLoader("spell_pal_art_of_war") { }

        class spell_pal_art_of_war_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_art_of_war_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (_player->HasSpellCooldown(PALADIN_SPELL_EXORCISM))
                        _player->RemoveSpellCooldown(PALADIN_SPELL_EXORCISM, true);

                    // Glyph of Mass Exorcism
                    if (_player->HasSpellCooldown(PALADIN_SPELL_MASS_EXORCISM))
                        _player->RemoveSpellCooldown(PALADIN_SPELL_MASS_EXORCISM, true);
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_pal_art_of_war_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_art_of_war_SpellScript();
        }
};

// Seal of Insight - 20167
class spell_pal_seal_of_insight: public SpellScriptLoader
{
    public:
        spell_pal_seal_of_insight() : SpellScriptLoader("spell_pal_seal_of_insight") { }

        class spell_pal_seal_of_insight_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_seal_of_insight_SpellScript);

            void HandleOnHit(SpellEffIndex)
            {
                // Needs a glyph script later for now its disabled in spellmgr
            }

            void Register()
            {
            //    OnEffectHitTarget += SpellEffectFn(spell_pal_seal_of_insight_SpellScript::HandleOnHit, EFFECT_1, SPELL_EFFECT_HEAL);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_seal_of_insight_SpellScript();
        }
};

// Blinding Light - 115750
class spell_pal_blinding_light: public SpellScriptLoader
{
    public:
        spell_pal_blinding_light() : SpellScriptLoader("spell_pal_blinding_light") { }

        class spell_pal_blinding_light_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_blinding_light_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (_player->HasAura(PALADIN_SPELL_GLYPH_OF_BLINDING_LIGHT))
                            _player->CastSpell(target, PALADIN_SPELL_BLINDING_LIGHT_STUN, true);
                        else
                            _player->CastSpell(target, PALADIN_SPELL_BLINDING_LIGHT_CONFUSE, true);
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_pal_blinding_light_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_blinding_light_SpellScript();
        }
};

// Hand of Protection - 1022
class spell_pal_hand_of_protection: public SpellScriptLoader
{
    public:
        spell_pal_hand_of_protection() : SpellScriptLoader("spell_pal_hand_of_protection") { }

        class spell_pal_hand_of_protection_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_hand_of_protection_SpellScript);

            SpellCastResult CheckForbearance()
            {
                Unit* caster = GetCaster();
                if (Unit* target = GetExplTargetUnit())
                    if (target->HasAura(SPELL_FORBEARANCE))
                        return SPELL_FAILED_TARGET_AURASTATE;

                return SPELL_CAST_OK;
            }

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (Unit* target = GetHitUnit())
                        _player->CastSpell(target, SPELL_FORBEARANCE, true);
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_pal_hand_of_protection_SpellScript::CheckForbearance);
                OnHit += SpellHitFn(spell_pal_hand_of_protection_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_hand_of_protection_SpellScript();
        }
};

// Cleanse - 4987
class spell_pal_cleanse: public SpellScriptLoader
{
    public:
        spell_pal_cleanse() : SpellScriptLoader("spell_pal_cleanse") { }

        class spell_pal_cleanse_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_cleanse_SpellScript);

            SpellCastResult CheckCleansing()
            {
                if (Unit* caster = GetCaster())
                {
                    if (Unit* target = GetExplTargetUnit())
                    {
                        DispelChargesList dispelList;

                        // Create dispel mask by dispel type
                        for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
                        {
                            if (GetSpellInfo()->Effects[i].IsEffect())
                            {
                                uint32 dispel_type = GetSpellInfo()->Effects[i].MiscValue;
                                uint32 dispelMask = GetSpellInfo()->GetDispelMask(DispelType(dispel_type));

                                // Epuration can dispell Magic with Sacred Cleansing
                                if (dispelMask == DISPEL_MAGIC && !caster->HasAura(PALADIN_SPELL_SACRED_CLEANSING) && GetSpellInfo()->Id == 4987)
                                    continue;

                                target->GetDispellableAuraList(caster, dispelMask, dispelList);
                            }
                        }
                        if (dispelList.empty())
                            return SPELL_FAILED_NOTHING_TO_DISPEL;

                        return SPELL_CAST_OK;
                    }
                }

                return SPELL_CAST_OK;
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_pal_cleanse_SpellScript::CheckCleansing);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_cleanse_SpellScript();
        }
};

// Divine Shield - 642 and Divine Shield - 110700
class spell_pal_divine_shield: public SpellScriptLoader
{
    public:
        spell_pal_divine_shield() : SpellScriptLoader("spell_pal_divine_shield") { }

        class spell_pal_divine_shield_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_divine_shield_SpellScript);

            uint8 m_CoutNegativeAura = 0;

            SpellCastResult CheckForbearance()
            {
                if (Unit* l_Caster = GetCaster())
                    if (l_Caster->HasAura(SPELL_FORBEARANCE))
                        return SPELL_FAILED_TARGET_AURASTATE;

                return SPELL_CAST_OK;
            }

            void HandleOnHit()
            {
                if (Unit* l_Caster = GetCaster())
                      l_Caster->CastSpell(l_Caster, SPELL_FORBEARANCE, true);
            }

            void HandleBeforeHit()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (l_Caster->HasAura(PALADIN_SPELL_GLYPH_OF_DIVINE_SHIELD))
                    {
                        Unit::AuraApplicationMap l_AppliedAuras = l_Caster->GetAppliedAuras();
                        for (Unit::AuraApplicationMap::iterator itr = l_AppliedAuras.begin(); itr != l_AppliedAuras.end(); itr++)
                        {
                            AuraApplication const* l_AurApp = itr->second;
                            constAuraPtr aura = l_AurApp->GetBase();
                            if (!l_AurApp->IsPositive()
                                && !aura->IsPassive()
                                && !aura->IsDeathPersistent())
                                m_CoutNegativeAura++;
                        }
                    }

                }
            }

            void HandleHeal(SpellEffIndex /*effIndex*/)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (l_Caster->HasAura(PALADIN_SPELL_GLYPH_OF_DIVINE_SHIELD))
                    {
                        SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(PALADIN_SPELL_GLYPH_OF_DIVINE_SHIELD);

                        if (l_SpellInfo)
                        {
                            if (m_CoutNegativeAura > l_SpellInfo->Effects[EFFECT_1].BasePoints)
                                m_CoutNegativeAura = l_SpellInfo->Effects[EFFECT_1].BasePoints;
                            SetHitHeal((l_Caster->GetMaxHealth() * (m_CoutNegativeAura * l_SpellInfo->Effects[EFFECT_0].BasePoints)) / 100);
                        }
                    }
                }
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_pal_divine_shield_SpellScript::CheckForbearance);
                BeforeHit += SpellHitFn(spell_pal_divine_shield_SpellScript::HandleBeforeHit);
                OnEffectHitTarget += SpellEffectFn(spell_pal_divine_shield_SpellScript::HandleHeal, EFFECT_3, SPELL_EFFECT_HEAL_PCT);
                OnHit += SpellHitFn(spell_pal_divine_shield_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_divine_shield_SpellScript();
        }
};

/// last update : 6.1.2 19802
/// Execution Sentence - 114916 and Stay of Execution - 114917
class spell_pal_execution_sentence_dispel: public SpellScriptLoader
{
    public:
        spell_pal_execution_sentence_dispel() : SpellScriptLoader("spell_pal_execution_sentence_dispel") { }

        class spell_pal_execution_sentence_dispel_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_pal_execution_sentence_dispel_AuraScript);

            enum eSpells
            {
                ExecutionSentence = 114916,
                StayOfExecution = 114917
            };

            void HandleDispel(DispelInfo* dispelData)
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetAura()->GetOwner()->ToUnit();

                if (l_Caster == nullptr || l_Target == nullptr)
                    return;

                int32 l_SpellPowerBonus = int32(l_Caster->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_HOLY) * GetSpellInfo()->Effects[EFFECT_1].BasePoints / 1000);
                int32 l_Damage = l_SpellPowerBonus + 26.72716306f * GetSpellInfo()->Effects[EFFECT_0].BasePoints;
                l_Damage = int32(l_Damage * 0.444f); ///< Final: 44.4%

                if (GetSpellInfo()->Id == eSpells::ExecutionSentence)
                        l_Caster->CastCustomSpell(l_Target, PALADIN_SPELL_EXECUTION_DISPEL_DAMAGE, &l_Damage, NULL, NULL, true);
                else
                        l_Caster->CastCustomSpell(l_Target, PALADIN_SPELL_EXECUTION_DISPEL_HEAL, &l_Damage, NULL, NULL, true);
            }

            void OnTick(constAuraEffectPtr p_AurEff)
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetTarget();

                if (l_Caster == nullptr || l_Target == nullptr)
                    return;

                float m_TickMultiplier[11];

                /// Calculate Multiplier Coeff
                if (GetSpellInfo()->Id == eSpells::ExecutionSentence) ///< Damage increases over time
                {
                    m_TickMultiplier[1] = 1.0f;
                    for (int i = 2; i <= 10; ++i)
                        m_TickMultiplier[i] = m_TickMultiplier[i - 1] * 1.1;
                    m_TickMultiplier[10] *= 5;
                }
                else ///< Heal decreases over time
                {
                    m_TickMultiplier[10] = 1.0f;
                    for (int i = 9; i > 0; --i)
                        m_TickMultiplier[i] = m_TickMultiplier[i + 1] * 1.1;
                    m_TickMultiplier[1] *= 5;
                }

                /// 1.1 + 1.1^2 + ... + 1.1^9 + 1.1^9 * 5 = 26.727163056
                /// 1 / 26,727163056 = 0.0374151195, which is the factor to get from the whole spells SP scaling to the base scaling of the 0th tick
                float l_BaseValue = int32(l_Caster->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_HOLY) * GetSpellInfo()->Effects[EFFECT_1].BasePoints / 1000) * 0.0374151195;

                uint32 l_TickNumber = p_AurEff->GetTickNumber();

                if (AuraEffectPtr l_AuraEffect = l_Target->GetAuraEffect(GetSpellInfo()->Id, EFFECT_0))
                        l_AuraEffect->SetAmount(int32(l_BaseValue * (m_TickMultiplier[l_TickNumber])));
            }

            void Register()
            {
                switch (m_scriptSpellId)
                {
                case eSpells::ExecutionSentence:
                    OnEffectPeriodic += AuraEffectPeriodicFn(spell_pal_execution_sentence_dispel_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
                    break;
                case eSpells::StayOfExecution:
                    OnEffectPeriodic += AuraEffectPeriodicFn(spell_pal_execution_sentence_dispel_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_HEAL);
                    break;
                default:
                    break;
                }
                OnDispel += AuraDispelFn(spell_pal_execution_sentence_dispel_AuraScript::HandleDispel);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_pal_execution_sentence_dispel_AuraScript();
        }
};

// Execution Sentence - 114157
class spell_pal_execution_sentence: public SpellScriptLoader
{
    public:
        spell_pal_execution_sentence() : SpellScriptLoader("spell_pal_execution_sentence") { }

        class spell_pal_execution_sentence_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_execution_sentence_SpellScript);

            void HandleAfterCast()
            {
                if (Unit* caster = GetCaster())
                {
                    if (Unit* target = GetExplTargetUnit())
                    {
                        if (caster->IsValidAttackTarget(target))
                            caster->CastSpell(target, PALADIN_SPELL_EXECUTION_SENTENCE, true);
                        else if (caster->GetGUID() == target->GetGUID())
                            caster->CastSpell(caster, PALADIN_SPELL_STAY_OF_EXECUTION, true);
                        else
                            caster->CastSpell(target, PALADIN_SPELL_STAY_OF_EXECUTION, true);
                    }
                }
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_pal_execution_sentence_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_execution_sentence_SpellScript();
        }
};

// Light's Hammer (periodic dummy for npc) - 114918
class spell_pal_lights_hammer_tick: public SpellScriptLoader
{
    public:
        spell_pal_lights_hammer_tick() : SpellScriptLoader("spell_pal_lights_hammer_tick") { }

        class spell_pal_lights_hammer_tick_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_pal_lights_hammer_tick_AuraScript);

            void OnTick(constAuraEffectPtr /*aurEff*/)
            {
                if (Unit *l_Caster = GetCaster())
                {
                    if (l_Caster->GetOwner())
                    {
                        l_Caster->CastSpell(l_Caster->GetPositionX(), l_Caster->GetPositionY(), l_Caster->GetPositionZ(), PALADIN_SPELL_ARCING_LIGHT_HEAL, true, 0, NULLAURA_EFFECT, l_Caster->GetOwner()->GetGUID());
                        l_Caster->CastSpell(l_Caster->GetPositionX(), l_Caster->GetPositionY(), l_Caster->GetPositionZ(), PALADIN_SPELL_ARCING_LIGHT_DAMAGE, true, 0, NULLAURA_EFFECT, l_Caster->GetOwner()->GetGUID());
                    }
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_pal_lights_hammer_tick_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_pal_lights_hammer_tick_AuraScript();
        }
};

// Light's Hammer - 122773
class spell_pal_lights_hammer : public SpellScriptLoader
{
public:
    spell_pal_lights_hammer() : SpellScriptLoader("spell_pal_lights_hammer") { }

    class spell_pal_lights_hammer_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pal_lights_hammer_SpellScript);

        void HandleAfterCast()
        {
            if (Unit* l_Caster = GetCaster())
            {
                std::list<Creature*> l_TempList;
                std::list<Creature*> l_LightsHammerlist;

                l_Caster->GetCreatureListWithEntryInGrid(l_LightsHammerlist, PALADIN_NPC_LIGHTS_HAMMER, 500.0f);

                l_TempList = l_LightsHammerlist;

                for (std::list<Creature*>::iterator i = l_TempList.begin(); i != l_TempList.end(); ++i)
                {
                    Unit* l_Owner = (*i)->GetOwner();
                    if (l_Owner != nullptr && l_Owner->GetGUID() == l_Caster->GetGUID() && (*i)->isSummon())
                        continue;

                    l_LightsHammerlist.remove((*i));
                }

                for (std::list<Creature*>::iterator itr = l_LightsHammerlist.begin(); itr != l_LightsHammerlist.end(); ++itr)
                    (*itr)->CastSpell((*itr), PALADIN_SPELL_LIGHTS_HAMMER_TICK, true);
            }
        }

        void Register()
        {
            AfterCast += SpellCastFn(spell_pal_lights_hammer_SpellScript::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_pal_lights_hammer_SpellScript();
    }
};

// called by Holy Prism (damage) - 114852 or Holy Prism (heal) - 114871
// Holy Prism visual for other targets
class spell_pal_holy_prism_visual: public SpellScriptLoader
{
    public:
        spell_pal_holy_prism_visual() : SpellScriptLoader("spell_pal_holy_prism_visual") { }

        class spell_pal_holy_prism_visual_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_holy_prism_visual_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (_player->IsValidAttackTarget(target))
                            _player->CastSpell(target, PALADIN_SPELL_HOLY_PRISM_DAMAGE_VISUAL_2, true);
                        else
                            _player->CastSpell(target, PALADIN_SPELL_HOLY_PRISM_HEAL_VISUAL_2, true);
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_pal_holy_prism_visual_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_holy_prism_visual_SpellScript();
        }
};

// called by Holy Prism (visual damage) - 114862 or Holy Prism (visual heal) - 121551
// Holy Prism (damage) - 114852 or Holy Prism (heal) - 114871
class spell_pal_holy_prism_effect: public SpellScriptLoader
{
    public:
        spell_pal_holy_prism_effect() : SpellScriptLoader("spell_pal_holy_prism_effect") { }

        class spell_pal_holy_prism_effect_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_holy_prism_effect_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        // damage
                        if (GetSpellInfo()->Id == 114862)
                            _player->CastSpell(target, PALADIN_SPELL_HOLY_PRISM_ENNEMIES, true);
                        // heal
                        else if (GetSpellInfo()->Id == 121551)
                            _player->CastSpell(target, PALADIN_SPELL_HOLY_PRISM_ALLIES, true);
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_pal_holy_prism_effect_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_holy_prism_effect_SpellScript();
        }
};

// Holy Prism - 114165
class spell_pal_holy_prism: public SpellScriptLoader
{
    public:
        spell_pal_holy_prism() : SpellScriptLoader("spell_pal_holy_prism") { }

        class spell_pal_holy_prism_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_holy_prism_SpellScript);

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (_player->IsValidAttackTarget(target))
                        {
                            _player->CastSpell(target, PALADIN_SPELL_HOLY_PRISM_DAMAGE_VISUAL, true);
                            _player->CastSpell(target, PALADIN_SPELL_HOLY_PRISM_DAMAGE_VISUAL_2, true);
                        }
                        else
                        {
                            _player->CastSpell(target, PALADIN_SPELL_HOLY_PRISM_HEAL_VISUAL, true);
                            _player->CastSpell(target, PALADIN_SPELL_HOLY_PRISM_HEAL_VISUAL_2, true);
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_pal_holy_prism_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_holy_prism_SpellScript();
        }
};

// Consecration - 26573 (periodic dummy)
class spell_pal_consecration: public SpellScriptLoader
{
    public:
        spell_pal_consecration() : SpellScriptLoader("spell_pal_consecration") { }

        class spell_pal_consecration_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_pal_consecration_AuraScript);

            void OnTick(constAuraEffectPtr aurEff)
            {
                if (DynamicObject* dynObj = GetCaster()->GetDynObject(PALADIN_SPELL_CONSECRATION_AREA_DUMMY))
                    GetCaster()->CastSpell(dynObj->GetPositionX(), dynObj->GetPositionY(), dynObj->GetPositionZ(), PALADIN_SPELL_CONSECRATION_DAMAGE, true);
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_pal_consecration_AuraScript::OnTick, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_pal_consecration_AuraScript();
        }
};

// Consecration - 26573
class spell_pal_consecration_area: public SpellScriptLoader
{
    public:
        spell_pal_consecration_area() : SpellScriptLoader("spell_pal_consecration_area") { }

        class spell_pal_consecration_area_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_consecration_area_SpellScript);

            void HandleAfterCast()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    _player->CastSpell(_player, PALADIN_SPELL_CONSECRATION_AREA_DUMMY, true);
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_pal_consecration_area_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_consecration_area_SpellScript();
        }
};

//  Word of Glory (Heal) - 130551
class spell_pal_word_of_glory_heal: public SpellScriptLoader
{
public:
    spell_pal_word_of_glory_heal() : SpellScriptLoader("spell_pal_word_of_glory_heal") { }

    class spell_pal_word_of_glory_heal_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pal_word_of_glory_heal_SpellScript);

        void HandleHeal(SpellEffIndex /*effIndex*/)
        {
            if (Unit* l_Caster = GetCaster())
                if (Unit* l_Target = GetHitUnit())
                {
                    int32 l_Power = l_Caster->GetPower(POWER_HOLY_POWER);
                    if (l_Power > 3 || l_Caster->HasAura(PALADIN_SPELL_DIVINE_PURPOSE_AURA))
                        l_Power = 3;

                    SetHitHeal((GetHitHeal() / 3) * l_Power);

                    if (l_Target->GetGUID() == l_Caster->GetGUID() && l_Caster->HasAura(PALADIN_SPELL_BASTION_OF_GLORY))
                    {
                        if (AuraPtr l_Aura = l_Caster->GetAura(PALADIN_SPELL_BASTION_OF_GLORY))
                        {
                            SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(114637);
                            
                            if (l_SpellInfo != nullptr)
                            {
                                SetHitHeal(GetHitHeal() + CalculatePct(GetHitHeal(), l_SpellInfo->Effects[EFFECT_0].BasePoints * l_Aura->GetStackAmount()));
                                l_Caster->RemoveAurasDueToSpell(PALADIN_SPELL_BASTION_OF_GLORY);
                            }
                        }
                    }

                    if (!l_Caster->HasAura(PALADIN_SPELL_DIVINE_PURPOSE_AURA))
                        l_Caster->ModifyPower(POWER_HOLY_POWER, -l_Power);
                }
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_pal_word_of_glory_heal_SpellScript::HandleHeal, EFFECT_0, SPELL_EFFECT_HEAL);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_pal_word_of_glory_heal_SpellScript;
    }
};

//  Word of Glory (Damage) - 130552
class spell_pal_word_of_glory_damage: public SpellScriptLoader
{
public:
    spell_pal_word_of_glory_damage() : SpellScriptLoader("spell_pal_word_of_glory_damage") { }

    class spell_pal_word_of_glory_damage_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pal_word_of_glory_damage_SpellScript);

        void HandleDamage(SpellEffIndex /*effIndex*/)
        {
            if (Unit* l_Player = GetCaster())
                if (Unit* l_Target = GetHitUnit())
                {
                    int32 l_Power = l_Player->GetPower(POWER_HOLY_POWER);
                    if (l_Power > 3 || l_Player->HasAura(PALADIN_SPELL_DIVINE_PURPOSE_AURA))
                        l_Power = 3;

                    SetHitDamage((GetHitDamage() / 3) * l_Power);

                    if (!l_Player->HasAura(PALADIN_SPELL_DIVINE_PURPOSE_AURA))
                        l_Player->ModifyPower(POWER_HOLY_POWER, -l_Power);
                }
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_pal_word_of_glory_damage_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_pal_word_of_glory_damage_SpellScript;
    }
};

// Word of Glory - 85673 - 136494
class spell_pal_word_of_glory: public SpellScriptLoader
{
    public:
        spell_pal_word_of_glory() : SpellScriptLoader("spell_pal_word_of_glory") { }

        class spell_pal_word_of_glory_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_word_of_glory_SpellScript);

            int32 m_HolyPower = 0;

            bool Validate(SpellInfo const* /*spellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(PALADIN_SPELL_WORD_OF_GLORY))
                    return false;
                return true;
            }

            void HandleOnCast()
            {
                if (Unit* l_Caster = GetCaster())
                    m_HolyPower = l_Caster->GetPower(POWER_HOLY_POWER);
            }

            void HandleOnHit()
            {
                if (Player* l_Player = GetCaster()->ToPlayer())
                {
                    if (Unit* l_Target = GetHitUnit())
                    {
                        if (!l_Player->HasAura(PALADIN_SPELL_GLYPH_OF_HARSH_WORDS))
                            if ((l_Target->GetTypeId() != TYPEID_PLAYER && !l_Target->isPet()) || !l_Target->IsFriendlyTo(l_Player))
                                l_Target = l_Player;

                        // Set HolyPower with value of OnPrepare because some Holy Power are consumed before OnHit and we need it for calculate
                        // HolyPower are consume after it
                        l_Player->SetPower(POWER_HOLY_POWER, m_HolyPower);

                        if (l_Target->IsFriendlyTo(l_Player))
                            l_Player->CastSpell(l_Target, PALADIN_SPELL_WORD_OF_GLORY_HEAL, true);
                        else if (l_Player->HasAura(PALADIN_SPELL_GLYPH_OF_HARSH_WORDS))
                            l_Player->CastSpell(l_Target, PALADIN_SPELL_HARSH_WORDS_DAMAGE, true);

                        if (l_Player->HasAura(PALADIN_SPELL_GLYPH_OF_WORD_OF_GLORY))
                        {
                            AuraPtr l_Aura = l_Player->AddAura(PALADIN_SPELL_GLYPH_OF_WORD_OF_GLORY_DAMAGE, l_Player);

                            if (l_Aura)
                            {
                                if (m_HolyPower > 3 || l_Player->HasAura(PALADIN_SPELL_DIVINE_PURPOSE_AURA))
                                    m_HolyPower = 3;

                                l_Aura->GetEffect(0)->ChangeAmount(l_Aura->GetEffect(0)->GetAmount() * (m_HolyPower));
                                l_Aura->SetNeedClientUpdateForTargets();
                            }
                        }
                    }
                }
            }

            void Register()
            {
                OnCast += SpellCastFn(spell_pal_word_of_glory_SpellScript::HandleOnCast);
                OnHit += SpellHitFn(spell_pal_word_of_glory_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_word_of_glory_SpellScript();
        }
};

// Judgment - 20271
class spell_pal_judgment: public SpellScriptLoader
{
    public:
        spell_pal_judgment() : SpellScriptLoader("spell_pal_judgment") { }

        class spell_pal_judgment_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_judgment_SpellScript);

            bool Validate(SpellInfo const* /*spellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(PALADIN_SPELL_JUDGMENT))
                    return false;
                return true;
            }

            void HandleOnHit()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (Unit* l_Target = GetHitUnit())
                    {
                        if (l_Caster->HasSpell(PALADIN_SPELL_TEMPLARS_VERDICT) || l_Caster->HasAura(PALADIN_SPELL_JUDGMENTS_OF_THE_WISE))
                            l_Caster->ModifyPower(POWER_HOLY_POWER, 1);
                        if (l_Caster->HasAura(PALADIN_SPELL_HOLY_AVENGER))
                            l_Caster->ModifyPower(POWER_HOLY_POWER, 3);
                        if (l_Caster->HasAura(PALADIN_SPELL_LONG_ARM_OF_THE_LAW))
                            l_Caster->CastSpell(l_Caster, PALADIN_SPELL_LONG_ARM_OF_THE_LAW_RUN_SPEED, true);
                        if (l_Caster->HasAura(PALADIN_SPELL_GLYPH_OF_BURDEN_OF_GUILT))
                            l_Caster->CastSpell(l_Target, PALADIN_SPELL_BURDEN_OF_GUILD, true);

                        if (l_Caster->HasAura(PALADIN_SPELL_EMPOWERED_SEALS))
                        {
                            if (l_Caster->HasAura(PALADIN_SPELL_SEAL_OF_JUSTICE))
                                l_Caster->CastSpell(l_Caster, PALADIN_SPELL_TURALYONS_JUSTICE, true);
                            else if (l_Caster->HasAura(PALADIN_SPELL_SEAL_OF_INSIHT))
                                l_Caster->CastSpell(l_Caster, PALADIN_SPELL_UTHERS_INSIGHT, true);
                            else if (l_Caster->HasAura(PALADIN_SPELL_SEAL_OF_RIGHTEOUSNESS))
                                l_Caster->CastSpell(l_Caster, PALADIN_SPELL_LIADRINS_RIGHTEOUSNESS, true);
                            else if (l_Caster->HasAura(PALADIN_SPELL_SEAL_OF_TRUTH))
                                l_Caster->CastSpell(l_Caster, PALADIN_SPELL_MARAADS_TRUTH, true);
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_pal_judgment_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_judgment_SpellScript();
        }
};

// Ardent Defender - 31850
class spell_pal_ardent_defender: public SpellScriptLoader
{
    public:
        spell_pal_ardent_defender() : SpellScriptLoader("spell_pal_ardent_defender") { }

        class spell_pal_ardent_defender_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_pal_ardent_defender_AuraScript);

            uint32 absorbPct, healPct;

            bool Load()
            {
                healPct = GetSpellInfo()->Effects[EFFECT_1].CalcValue();
                absorbPct = GetSpellInfo()->Effects[EFFECT_0].CalcValue();
                return GetUnitOwner()->GetTypeId() == TYPEID_PLAYER;
            }

            void CalculateAmount(constAuraEffectPtr aurEff, int32 & amount, bool & canBeRecalculated)
            {
                // Set absorbtion amount to unlimited
                amount = -1;
            }

            void Absorb(AuraEffectPtr aurEff, DamageInfo & dmgInfo, uint32 & absorbAmount)
            {
                Unit* victim = GetTarget();
                int32 remainingHealth = victim->GetHealth() - dmgInfo.GetDamage();
                // If damage kills us
                if (remainingHealth <= 0 && !victim->ToPlayer()->HasSpellCooldown(PALADIN_SPELL_ARDENT_DEFENDER_HEAL))
                {
                    // Cast healing spell, completely avoid damage
                    absorbAmount = dmgInfo.GetDamage();

                    int32 healAmount = int32(victim->CountPctFromMaxHealth(healPct));
                    victim->CastCustomSpell(victim, PALADIN_SPELL_ARDENT_DEFENDER_HEAL, &healAmount, NULL, NULL, true, NULL, aurEff);
                }
                else
                    absorbAmount = CalculatePct(dmgInfo.GetDamage(), absorbPct);
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_pal_ardent_defender_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
                OnEffectAbsorb += AuraEffectAbsorbFn(spell_pal_ardent_defender_AuraScript::Absorb, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_pal_ardent_defender_AuraScript();
        }
};

// Blessing of faith - 37877
class spell_pal_blessing_of_faith: public SpellScriptLoader
{
    public:
        spell_pal_blessing_of_faith() : SpellScriptLoader("spell_pal_blessing_of_faith") { }

        class spell_pal_blessing_of_faith_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_blessing_of_faith_SpellScript);

            bool Validate(SpellInfo const* /*spellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_BLESSING_OF_LOWER_CITY_DRUID) || !sSpellMgr->GetSpellInfo(SPELL_BLESSING_OF_LOWER_CITY_PALADIN) || !sSpellMgr->GetSpellInfo(SPELL_BLESSING_OF_LOWER_CITY_PRIEST) || !sSpellMgr->GetSpellInfo(SPELL_BLESSING_OF_LOWER_CITY_SHAMAN))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (Unit* unitTarget = GetHitUnit())
                {
                    uint32 spell_id = 0;
                    switch (unitTarget->getClass())
                    {
                        case CLASS_DRUID:   spell_id = SPELL_BLESSING_OF_LOWER_CITY_DRUID; break;
                        case CLASS_PALADIN: spell_id = SPELL_BLESSING_OF_LOWER_CITY_PALADIN; break;
                        case CLASS_PRIEST:  spell_id = SPELL_BLESSING_OF_LOWER_CITY_PRIEST; break;
                        case CLASS_SHAMAN:  spell_id = SPELL_BLESSING_OF_LOWER_CITY_SHAMAN; break;
                        default: return;                    // ignore for non-healing classes
                    }
                    Unit* caster = GetCaster();
                    caster->CastSpell(caster, spell_id, true);
                }
            }

            void Register()
            {
                // add dummy effect spell handler to Blessing of Faith
                OnEffectHitTarget += SpellEffectFn(spell_pal_blessing_of_faith_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_blessing_of_faith_SpellScript();
        }
};

// Holy Shock (heal) - 25914
class spell_pal_holy_shock_heal: public SpellScriptLoader
{
    public:
        spell_pal_holy_shock_heal() : SpellScriptLoader("spell_pal_holy_shock_heal") { }

        class spell_pal_holy_shock_heal_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_holy_shock_heal_SpellScript);

            void HandleHeal(SpellEffIndex /*effIndex*/)
            {
                if (Unit* l_Caster = GetCaster())
                    if (Unit* l_Target = GetHitUnit())
                    {
                        if (l_Caster->HasAura(PALADIN_SPELL_DAYBREAK_PROC))
                        {
                            int32 l_Heal = 0;

                            SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(PALADIN_SPELL_DAYBREAK_PROC);
                            SpellInfo const* l_SpellInfoImproved = sSpellMgr->GetSpellInfo(PALADIN_SPELL_IMPROVED_DAYBREAK);

                            if (l_SpellInfo != nullptr)
                                l_Heal = CalculatePct(GetHitHeal(), l_SpellInfo->Effects[EFFECT_0].BasePoints);
                            if (l_Caster->HasAura(PALADIN_SPELL_IMPROVED_DAYBREAK) && l_SpellInfoImproved != nullptr)
                                l_Heal = AddPct(l_Heal, l_SpellInfoImproved->Effects[EFFECT_0].BasePoints);

                            l_Target->CastCustomSpell(l_Target, PALADIN_SPELL_DAYBREAK_HEAL, 0, &l_Heal, NULL, true);
                            l_Caster->RemoveAura(PALADIN_SPELL_DAYBREAK_PROC);
                        }
                    }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_pal_holy_shock_heal_SpellScript::HandleHeal, EFFECT_0, SPELL_EFFECT_HEAL);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_holy_shock_heal_SpellScript();
        }
};

// Holy Shock (damage) - 25912
class spell_pal_holy_shock_damage: public SpellScriptLoader
{
    public:
        spell_pal_holy_shock_damage() : SpellScriptLoader("spell_pal_holy_shock_damage") { }

        class spell_pal_holy_shock_damage_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_holy_shock_damage_SpellScript);

            void HandleOnHit()
            {
                if (Player* caster = GetCaster()->ToPlayer())
                {
                    if (Unit* unitTarget = GetHitUnit())
                    {
                        int32 damage = GetHitDamage();

                        if (caster->getLevel() < 85)
                        {
                            damage = int32(GetHitDamage() * 0.15f);
                            SetHitDamage(damage);
                        }
                        else if (caster->getLevel() < 90)
                        {
                            damage = int32(GetHitDamage() * 0.61f);
                            SetHitDamage(damage);
                        }
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_pal_holy_shock_damage_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_holy_shock_damage_SpellScript();
        }
};

// Holy Shock - 20473
class spell_pal_holy_shock: public SpellScriptLoader
{
    public:
        spell_pal_holy_shock() : SpellScriptLoader("spell_pal_holy_shock") { }

        class spell_pal_holy_shock_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_holy_shock_SpellScript);

            bool Validate(SpellInfo const* spell)
            {
                if (!sSpellMgr->GetSpellInfo(PALADIN_SPELL_HOLY_SHOCK_R1))
                    return false;

                // can't use other spell than holy shock due to spell_ranks dependency
                if (sSpellMgr->GetFirstSpellInChain(PALADIN_SPELL_HOLY_SHOCK_R1) != sSpellMgr->GetFirstSpellInChain(spell->Id))
                    return false;

                uint8 rank = sSpellMgr->GetSpellRank(spell->Id);
                if (!sSpellMgr->GetSpellWithRank(PALADIN_SPELL_HOLY_SHOCK_R1_DAMAGE, rank, true) || !sSpellMgr->GetSpellWithRank(PALADIN_SPELL_HOLY_SHOCK_R1_HEALING, rank, true))
                    return false;

                return true;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (Player* l_Caster = GetCaster()->ToPlayer())
                {
                    if (Unit* l_Target = GetHitUnit())
                    {
                        if (l_Caster->IsFriendlyTo(l_Target))
                            l_Caster->CastSpell(l_Target, PALADIN_SPELL_HOLY_SHOCK_R1_HEALING, true);
                        else
                            l_Caster->CastSpell(l_Target, PALADIN_SPELL_HOLY_SHOCK_R1_DAMAGE, true);

                        if (l_Caster->HasAura(PALADIN_ENHANCED_HOLY_SHOCK_PROC))
                            l_Caster->ToPlayer()->RemoveSpellCooldown(PALADIN_SPELL_HOLY_SHOCK_R1, true);
                        if (l_Caster->HasAura(PALADIN_SPELL_GLYPH_OF_DENOUNCE))
                            if (roll_chance_i(50))
                                l_Caster->CastSpell(l_Caster, PALADIN_SPELL_GLYPH_OF_DENOUNCE_PROC, true);

                        l_Caster->CastSpell(l_Caster, PALADIN_SPELL_HOLY_SHOCK_ENERGIZE, true);
                    }
                }
            }

            SpellCastResult CheckCast()
            {
                Unit* l_Caster = GetCaster();
                if (Unit* l_Target = GetExplTargetUnit())
                {
                    if (!l_Caster->IsFriendlyTo(l_Target))
                    {
                        if (!l_Caster->IsValidAttackTarget(l_Target))
                            return SPELL_FAILED_BAD_TARGETS;

                        if (!l_Caster->isInFront(l_Target))
                            return SPELL_FAILED_UNIT_NOT_INFRONT;
                    }
                }
                else
                    return SPELL_FAILED_BAD_TARGETS;
                return SPELL_CAST_OK;
            }

            void Register()
            {
                // add dummy effect spell handler to Holy Shock
                OnCheckCast += SpellCheckCastFn(spell_pal_holy_shock_SpellScript::CheckCast);
                OnEffectHitTarget += SpellEffectFn(spell_pal_holy_shock_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_holy_shock_SpellScript();
        }
};

// Lay on Hands - 633
class spell_pal_lay_on_hands: public SpellScriptLoader
{
    public:
        spell_pal_lay_on_hands() : SpellScriptLoader("spell_pal_lay_on_hands") { }

        class spell_pal_lay_on_hands_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_lay_on_hands_SpellScript);

            SpellCastResult CheckForbearance()
            {
                Unit* caster = GetCaster();
                if (Unit* target = GetExplTargetUnit())
                    if (target->HasAura(SPELL_FORBEARANCE))
                        return SPELL_FAILED_TARGET_AURASTATE;

                return SPELL_CAST_OK;
            }

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (Unit* target = GetHitUnit())
                        _player->CastSpell(target, SPELL_FORBEARANCE, true);
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_pal_lay_on_hands_SpellScript::CheckForbearance);
                OnHit += SpellHitFn(spell_pal_lay_on_hands_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_lay_on_hands_SpellScript();
        }
};

// Righteous defense - 31789
class spell_pal_righteous_defense: public SpellScriptLoader
{
    public:
        spell_pal_righteous_defense() : SpellScriptLoader("spell_pal_righteous_defense") { }

        class spell_pal_righteous_defense_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_righteous_defense_SpellScript);

            SpellCastResult CheckCast()
            {
                Unit* caster = GetCaster();
                if (caster->GetTypeId() != TYPEID_PLAYER)
                    return SPELL_FAILED_DONT_REPORT;

                if (Unit* target = GetExplTargetUnit())
                {
                    if (!target->IsFriendlyTo(caster) || target->getAttackers().empty())
                        return SPELL_FAILED_BAD_TARGETS;
                }
                else
                    return SPELL_FAILED_BAD_TARGETS;

                return SPELL_CAST_OK;
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_pal_righteous_defense_SpellScript::CheckCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_righteous_defense_SpellScript();
        }
};

// Eternal Flame - 114163
class spell_pal_eternal_flame : public SpellScriptLoader
{
public:
    spell_pal_eternal_flame() : SpellScriptLoader("spell_pal_eternal_flame") { }

    class spell_pal_eternal_flame_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pal_eternal_flame_SpellScript);

        int32 m_PowerUsed = 0;

        SpellCastResult CheckCast()
        {
            if (Player* l_Player = GetCaster()->ToPlayer())
                if (l_Player->GetPower(POWER_HOLY_POWER) < 1 && !l_Player->HasAura(PALADIN_SPELL_DIVINE_PURPOSE_AURA))
                    return SPELL_FAILED_NO_POWER;

            return SPELL_CAST_OK;
        }

        void HandleOnCast()
        {
            if (Unit* l_Caster = GetCaster())
                m_PowerUsed = l_Caster->GetPower(POWER_HOLY_POWER);
        }

        void HandleHeal(SpellEffIndex /*effIndex*/)
        {
            if (Unit* l_Caster = GetCaster())
                if (Unit* l_Target = GetHitUnit())
                {
                    l_Target->SetPower(POWER_HOLY_POWER, m_PowerUsed);

                    if (m_PowerUsed > 3 || l_Caster->HasAura(PALADIN_SPELL_DIVINE_PURPOSE_AURA))
                        m_PowerUsed = 3;

                    SetHitHeal((GetHitHeal() / 3) * m_PowerUsed);
                    if (l_Target->GetGUID() == l_Caster->GetGUID() && l_Caster->HasAura(PALADIN_SPELL_BASTION_OF_GLORY))
                    {
                        if (AuraPtr l_Aura = l_Caster->GetAura(PALADIN_SPELL_BASTION_OF_GLORY))
                        {
                            SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(114637);

                            if (l_SpellInfo != nullptr)
                            {
                                SetHitHeal(GetHitHeal() + CalculatePct(GetHitHeal(), l_SpellInfo->Effects[EFFECT_0].BasePoints * l_Aura->GetStackAmount()));
                                l_Caster->RemoveAurasDueToSpell(PALADIN_SPELL_BASTION_OF_GLORY);
                            }
                        }
                    }

                    l_Caster->CastSpell(l_Target, PALADIN_SPELL_ETERNAL_FLAME_PERIODIC_HEAL, true);

                    if (AuraPtr l_PeriodicHeal = l_Caster->GetAura(PALADIN_SPELL_ETERNAL_FLAME_PERIODIC_HEAL))
                    {
                        int32 l_Duration = (GetSpellInfo()->Effects[EFFECT_2].BasePoints / 3) * m_PowerUsed;
                        l_PeriodicHeal->SetDuration(l_Duration * IN_MILLISECONDS);
                    }

                    if (!l_Caster->HasAura(PALADIN_SPELL_DIVINE_PURPOSE_AURA))
                        l_Caster->ModifyPower(POWER_HOLY_POWER, -m_PowerUsed);
                }
        }

        void Register()
        {
            OnCheckCast += SpellCheckCastFn(spell_pal_eternal_flame_SpellScript::CheckCast);
            OnCast += SpellCastFn(spell_pal_eternal_flame_SpellScript::HandleOnCast);
            OnEffectHitTarget += SpellEffectFn(spell_pal_eternal_flame_SpellScript::HandleHeal, EFFECT_0, SPELL_EFFECT_HEAL);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_pal_eternal_flame_SpellScript();
    }
};


// Eternal Flame Aura periodic heal- 156322
class spell_pal_eternal_flame_periodic_heal: public SpellScriptLoader
{
    public:
        spell_pal_eternal_flame_periodic_heal() : SpellScriptLoader("spell_pal_eternal_flame_periodic_heal") { }

        class spell_pal_eternal_flame_periodic_heal_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_pal_eternal_flame_periodic_heal_AuraScript);

            void CalculateAmount(constAuraEffectPtr, int32 & amount, bool &)
            {
                if (Unit* l_Owner = GetOwner()->ToUnit())
                if (Unit* l_Caster = GetCaster())
                {
                    SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(PALADIN_SPELL_ETERNAL_FLAME);

                    int32 l_Heal = l_Owner->SpellBaseDamageBonusDone(SPELL_SCHOOL_MASK_ALL) * GetSpellInfo()->Effects[0].BonusMultiplier;

                    if (l_Owner->GetGUID() == l_Caster->GetGUID() && l_SpellInfo != nullptr)
                        AddPct(l_Heal, l_SpellInfo->Effects[1].BasePoints);

                    amount = l_Heal;
                }
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_pal_eternal_flame_periodic_heal_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_PERIODIC_HEAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_pal_eternal_flame_periodic_heal_AuraScript();
        }
};

/// last update : 6.1.2 19802
/// Holy Wrath - 119072
class spell_pal_holy_wrath: public SpellScriptLoader
{
public:
    spell_pal_holy_wrath() : SpellScriptLoader("spell_pal_holy_wrath") { }

    class spell_pal_holy_wrath_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pal_holy_wrath_SpellScript);

        enum eSpells
        {
            GlyphOfFinalWrath = 54935
        };

        void HandleDamage(SpellEffIndex /*effIndex*/)
        {
            Unit* l_Caster = GetCaster();
            Unit* l_Target = GetHitUnit();
            SpellInfo const* l_GlyphOfFinalWrath = sSpellMgr->GetSpellInfo(eSpells::GlyphOfFinalWrath);

            if (l_Target == nullptr)
                return;

            if (l_Caster->HasAura(PALADIN_SPELL_SANCTIFIED_WRATH_PROTECTION))
                l_Caster->SetPower(POWER_HOLY_POWER, l_Caster->GetPower(POWER_HOLY_POWER) + GetSpellInfo()->Effects[EFFECT_1].BasePoints);
            if (l_Caster->HasAura(eSpells::GlyphOfFinalWrath) && l_GlyphOfFinalWrath != nullptr && l_Target->GetHealthPct() < 20.0f)
                SetHitDamage(GetHitDamage() + CalculatePct(GetHitDamage(), l_GlyphOfFinalWrath->Effects[EFFECT_0].BasePoints));
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_pal_holy_wrath_SpellScript::HandleDamage, EFFECT_1, SPELL_EFFECT_SCHOOL_DAMAGE);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_pal_holy_wrath_SpellScript();
    }
};

// Empowered Divine Storm - 174718
class PlayerScript_empowered_divine_storm: public PlayerScript
{
public:
    PlayerScript_empowered_divine_storm() :PlayerScript("PlayerScript_empowered_divine_storm") {}

    void OnModifyPower(Player* p_Player, Powers p_Power, int32 p_OldValue, int32& p_NewValue, bool p_Regen)
    {
        if (p_Regen)
            return;

        if (p_Player->getClass() == CLASS_PALADIN && p_Player->GetSpecializationId(p_Player->GetActiveSpec()) == SPEC_PALADIN_RETRIBUTION && p_Power == POWER_HOLY_POWER)
        {
            // Get the power earn (if > 0 ) or consum (if < 0)
            int32 l_diffValue = p_NewValue - p_OldValue;

            if (l_diffValue < 0 && p_Player->HasAura(PALADIN_SPELL_EMPOWERED_DIVINE_STORM) && roll_chance_i(sSpellMgr->GetSpellInfo(PALADIN_SPELL_EMPOWERED_DIVINE_STORM)->Effects[EFFECT_0].BasePoints))
                p_Player->CastSpell(p_Player, PALADIN_SPELL_DIVINE_CRUSADER, true);
        }
    }
};

// Saved By The Light - 157047
class PlayerScript_saved_by_the_light : public PlayerScript
{
public:
    PlayerScript_saved_by_the_light() :PlayerScript("PlayerScript_saved_by_the_light") {}

    void OnModifyHealth(Player* p_Player, int32 p_Value)
    {
        if (((p_Player->HasAura(PALADIN_SPELL_BEACON_OF_FAITH) && p_Player->GetAura(PALADIN_SPELL_BEACON_OF_FAITH)->GetOwner()->ToPlayer() != nullptr && p_Player->GetAura(PALADIN_SPELL_BEACON_OF_FAITH)->GetOwner()->ToPlayer()->HasAura(PALADIN_SPELL_SAVED_BY_THE_LIGHT))
            || (p_Player->HasAura(PALADIN_SPELL_BEACON_OF_LIGHT) && p_Player->GetAura(PALADIN_SPELL_BEACON_OF_LIGHT)->GetOwner()->ToPlayer() != nullptr && p_Player->GetAura(PALADIN_SPELL_BEACON_OF_LIGHT)->GetOwner()->ToPlayer()->HasAura(PALADIN_SPELL_SAVED_BY_THE_LIGHT))
            || p_Player->HasAura(PALADIN_SPELL_SAVED_BY_THE_LIGHT)) && p_Player->HasAura(PALADIN_SPELL_SAVED_BY_THE_LIGHT_PROC) == false)
            if (const SpellInfo* l_SpellInfo = sSpellMgr->GetSpellInfo(PALADIN_SPELL_SAVED_BY_THE_LIGHT))
            { 
                // When you or your Beacon of Light target drop below 30 % health
                if (((p_Value * 100) / p_Player->GetMaxHealth()) < (uint32)l_SpellInfo->Effects[EFFECT_0].BasePoints)
                {
                    // instantly grant the injured target a protective shield, absorbing up to 30% of their maximum health
                    int32 l_Absorb = CalculatePct(p_Player->GetMaxHealth(), l_SpellInfo->Effects[EFFECT_0].BasePoints);
                    p_Player->CastCustomSpell(p_Player, PALADIN_SPELL_SAVED_BY_THE_LIGHT_SHIELD, &l_Absorb, NULL, NULL, true);
                    p_Player->CastSpell(p_Player, PALADIN_SPELL_SAVED_BY_THE_LIGHT_PROC, true);
                }
            }
    }
};

// Call by Templars Verdict 85256 - Divine storm 53385 - Eternal Flame 114163
// Call by Word of Glory 85673 - Shield of Righteous 53600
// Call by Light of dawn 85222
// Divine Purpose - 86172
class spell_pal_divine_purpose: public SpellScriptLoader
{
public:
    spell_pal_divine_purpose() : SpellScriptLoader("spell_pal_divine_purpose") { }

    class spell_pal_divine_purpose_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_pal_divine_purpose_SpellScript);

        void HandleAfterCast()
        {
            if (Player* l_Player = GetCaster()->ToPlayer())
            {
                if (l_Player->getLevel() >= 75 && l_Player->HasSpell(PALADIN_SPELL_DIVINE_PURPOSE))
                {
                    if (l_Player->GetSpecializationId(l_Player->GetActiveSpec()) == SPEC_PALADIN_RETRIBUTION && roll_chance_i(sSpellMgr->GetSpellInfo(PALADIN_SPELL_DIVINE_PURPOSE)->Effects[EFFECT_0].BasePoints))
                    {
                        if (GetSpellInfo()->Id == PALADIN_SPELL_WORD_OF_GLORY || GetSpellInfo()->Id == PALADIN_SPELL_TEMPLARS_VERDICT || GetSpellInfo()->Id == SPELL_DIVINE_STORM || GetSpellInfo()->Id == PALADIN_SPELL_ETERNAL_FLAME)
                            l_Player->CastSpell(l_Player, PALADIN_SPELL_DIVINE_PURPOSE_AURA, true);
                    }
                    else if (l_Player->GetSpecializationId(l_Player->GetActiveSpec()) == SPEC_PALADIN_PROTECTION && roll_chance_i(sSpellMgr->GetSpellInfo(PALADIN_SPELL_DIVINE_PURPOSE)->Effects[EFFECT_0].BasePoints))
                    {
                        if (GetSpellInfo()->Id == PALADIN_SPELL_WORD_OF_GLORY || GetSpellInfo()->Id == PALADIN_SPELL_SHIELD_OF_THE_RIGHTEOUS)
                            l_Player->CastSpell(l_Player, PALADIN_SPELL_DIVINE_PURPOSE_AURA, true);
                    }
                    else if (l_Player->GetSpecializationId(l_Player->GetActiveSpec()) == SPEC_PALADIN_HOLY && roll_chance_i(sSpellMgr->GetSpellInfo(PALADIN_SPELL_DIVINE_PURPOSE)->Effects[EFFECT_0].BasePoints))
                    {
                        if (GetSpellInfo()->Id == PALADIN_SPELL_WORD_OF_GLORY || GetSpellInfo()->Id == PALADIN_SPELL_LIGHT_OF_DAWN)
                            l_Player->CastSpell(l_Player, PALADIN_SPELL_DIVINE_PURPOSE_AURA, true);
                    }
                }
            }
        }

        void Register()
        {
            AfterCast += SpellCastFn(spell_pal_divine_purpose_SpellScript::HandleAfterCast);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_pal_divine_purpose_SpellScript();
    }
};

enum SealOfJusticeSpells
{
    SpellSealOfJusticeProc = 20170
};

/// Seal of Justice - 20164
class spell_pal_seal_of_justice : public SpellScriptLoader
{
    public:
        spell_pal_seal_of_justice() : SpellScriptLoader("spell_pal_seal_of_justice") { }

        class spell_pal_seal_of_justice_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_pal_seal_of_justice_AuraScript);

            void OnProc(constAuraEffectPtr p_AurEff, ProcEventInfo& p_EventInfo)
            {
                PreventDefaultAction();

                Unit* l_Caster = GetCaster();

                if (!l_Caster)
                    return;

                if (p_EventInfo.GetDamageInfo()->GetSpellInfo())
                {
                    if (!(p_EventInfo.GetDamageInfo()->GetSpellInfo()->DmgClass & SPELL_DAMAGE_CLASS_MELEE))
                        return;

                    if (p_EventInfo.GetDamageInfo()->GetSpellInfo()->Id == PALADIN_SPELL_JUDGMENT) ///< It does not apply on Judgements.
                        return;
                }

                l_Caster->CastSpell(p_EventInfo.GetDamageInfo()->GetVictim(), SealOfJusticeSpells::SpellSealOfJusticeProc, true);
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_pal_seal_of_justice_AuraScript::OnProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_pal_seal_of_justice_AuraScript();
        }
};

/// Holy Shield - 152261
class spell_pal_holy_shield: public SpellScriptLoader
{
    public:
        spell_pal_holy_shield() : SpellScriptLoader("spell_pal_holy_shield") { }

        class spell_pal_holy_shield_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_pal_holy_shield_AuraScript);

            void CalculateAmount(constAuraEffectPtr, int32 & amount, bool &)
            {
                amount = 0;
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_pal_holy_shield_AuraScript::CalculateAmount, EFFECT_2, SPELL_AURA_SCHOOL_ABSORB);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_pal_holy_shield_AuraScript();
        }
};

// Beacon of Faith - 156910
class spell_pal_beacon_of_faith: public SpellScriptLoader
{
    public:
        spell_pal_beacon_of_faith() : SpellScriptLoader("spell_pal_beacon_of_faith") { }

        class spell_pal_beacon_of_faith_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_beacon_of_faith_SpellScript);

            SpellCastResult CheckCast()
            {
                if (Unit* l_Caster = GetCaster())
                if (Unit* l_Target = GetExplTargetUnit())
                if (l_Target->HasAura(PALADIN_SPELL_BEACON_OF_LIGHT, l_Caster->GetGUID()))
                    return SPELL_FAILED_BAD_TARGETS;
                return SPELL_CAST_OK;
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_pal_beacon_of_faith_SpellScript::CheckCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_beacon_of_faith_SpellScript();
        }
};

// Light of Dawn - 85222
class spell_pal_light_of_dawn : public SpellScriptLoader
{
    public:
        spell_pal_light_of_dawn() : SpellScriptLoader("spell_pal_light_of_dawn") { }

        class spell_pal_light_of_dawn_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_light_of_dawn_SpellScript);

            int32 m_HolyPower = 0;

            void HandleOnCast()
            {
                if (Unit* l_Caster = GetCaster())
                    m_HolyPower = l_Caster->GetPower(POWER_HOLY_POWER);
            }

            void HandleHeal(SpellEffIndex /*effIndex*/)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    l_Caster->SetPower(POWER_HOLY_POWER, m_HolyPower);

                    if (m_HolyPower > 3 || l_Caster->HasAura(PALADIN_SPELL_DIVINE_PURPOSE_AURA))
                        m_HolyPower = 3;

                    SetHitHeal(GetHitHeal() * m_HolyPower);

                    if (!l_Caster->HasAura(PALADIN_SPELL_DIVINE_PURPOSE_AURA))
                        l_Caster->ModifyPower(POWER_HOLY_POWER, -m_HolyPower);
                }
            }

            void Register()
            {
                OnCast += SpellCastFn(spell_pal_light_of_dawn_SpellScript::HandleOnCast);
                OnEffectHitTarget += SpellEffectFn(spell_pal_light_of_dawn_SpellScript::HandleHeal, EFFECT_0, SPELL_EFFECT_HEAL);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_light_of_dawn_SpellScript();
        }
};

// Enhanced Holy Shock - 157478
class spell_pal_enhanced_holy_shock : public SpellScriptLoader
{
    public:
        spell_pal_enhanced_holy_shock() : SpellScriptLoader("spell_pal_enhanced_holy_shock") { }

        class spell_pal_enhanced_holy_shock_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_pal_enhanced_holy_shock_AuraScript);

            void OnProc(constAuraEffectPtr p_AurEff, ProcEventInfo& p_EventInfo)
            {
                PreventDefaultAction();

                Unit* l_Caster = GetCaster();
                if (!l_Caster)
                    return;

                if (p_EventInfo.GetActor()->GetGUID() != l_Caster->GetGUID() || p_EventInfo.GetDamageInfo()->GetSpellInfo() == nullptr)
                    return;

                if (p_EventInfo.GetDamageInfo()->GetSpellInfo()->Id != PALADIN_SPELL_FLASH_OF_LIGHT && p_EventInfo.GetDamageInfo()->GetSpellInfo()->Id != PALADIN_SPELL_HOLY_LIGHT)
                    return;

                l_Caster->CastSpell(l_Caster, PALADIN_ENHANCED_HOLY_SHOCK_PROC, true);
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_pal_enhanced_holy_shock_AuraScript::OnProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_pal_enhanced_holy_shock_AuraScript();
        }
};

/// last update : 6.1.2 19802
/// Sanctified Wrath - 53376
/// Called by Avenging Wrath - 31884
class spell_pal_sanctified_wrath : public SpellScriptLoader
{
    public:
        spell_pal_sanctified_wrath() : SpellScriptLoader("spell_pal_sanctified_wrath") { }

        class spell_pal_sanctified_wrath_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_pal_sanctified_wrath_AuraScript);

            void OnApply(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster == nullptr)
                    return;

                Player* l_Player = l_Caster->ToPlayer();

                if (l_Player == nullptr)
                    return;

                l_Player->CastSpell(l_Player, PALADIN_SPELL_SANCTIFIED_WRATH_BONUS, true);

                if (AuraPtr l_SanctifiedWrathAura = l_Player->GetAura(PALADIN_SPELL_SANCTIFIED_WRATH_BONUS))
                {
                    l_SanctifiedWrathAura->GetEffect(EFFECT_1)->SetAmount(0);
                    l_SanctifiedWrathAura->GetEffect(EFFECT_3)->SetAmount(0);
                    if (l_Player->GetSpecializationId(l_Player->GetActiveSpec()) == SPEC_PALADIN_RETRIBUTION)
                    {
                        l_SanctifiedWrathAura->GetEffect(EFFECT_0)->SetAmount(0);
                        l_SanctifiedWrathAura->GetEffect(EFFECT_4)->SetAmount(0);
                    }
                    else if (l_Player->GetSpecializationId(l_Player->GetActiveSpec()) == SPEC_PALADIN_HOLY)
                        l_SanctifiedWrathAura->GetEffect(EFFECT_2)->SetAmount(0);
                }

            }

            void OnRemove(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster == nullptr)
                    return;

                if (l_Caster->HasAura(PALADIN_SPELL_SANCTIFIED_WRATH_BONUS))
                    l_Caster->RemoveAurasDueToSpell(PALADIN_SPELL_SANCTIFIED_WRATH_BONUS);
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_pal_sanctified_wrath_AuraScript::OnApply, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_pal_sanctified_wrath_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_pal_sanctified_wrath_AuraScript();
        }
};

/// last update : 6.1.2 19802
/// Selfless Healer - 85804
class spell_pal_selfless_healer_proc : public SpellScriptLoader
{
    public:
        spell_pal_selfless_healer_proc() : SpellScriptLoader("spell_pal_selfless_healer_proc") { }

        class spell_pal_selfless_healer_proc_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_pal_selfless_healer_proc_AuraScript);

            void OnProc(constAuraEffectPtr p_AurEff, ProcEventInfo& p_EventInfo)
            {
                PreventDefaultAction();

                Unit* l_Caster = GetCaster();

                if (!l_Caster)
                    return;

                if (p_EventInfo.GetDamageInfo()->GetSpellInfo()->Id != PALADIN_SPELL_JUDGMENT)
                    return;

                if (!p_EventInfo.GetDamageInfo()->GetDamage())
                    return;

                l_Caster->CastSpell(l_Caster, PALADIN_SPELL_SELFLESS_HEALER_STACK, true);
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_pal_selfless_healer_proc_AuraScript::OnProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_pal_selfless_healer_proc_AuraScript();
        }
};

/// last update : 6.1.2 19802
/// Denounce - 2812
class spell_pal_denounce : public SpellScriptLoader
{
    public:
        spell_pal_denounce() : SpellScriptLoader("spell_pal_denounce") { }

        class spell_pal_denounce_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pal_denounce_SpellScript);

            enum eSpells
            {
                WoDPvPHoly2PBonusAura   = 170860,
                WoDPvPHoly2PBonus       = 170866
            };

            void HandleDamage(SpellEffIndex /*l_EffIndex*/)
            {
                Unit* l_Caster = GetCaster();

                if (l_Caster->HasAura(eSpells::WoDPvPHoly2PBonusAura))
                    l_Caster->CastSpell(l_Caster, eSpells::WoDPvPHoly2PBonus, true);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_pal_denounce_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_pal_denounce_SpellScript();
        }
};


/// Item - Paladin WoD PvP Retribution 4P Bonus - 165895
class PlayerScript_paladin_wod_pvp_4p_bonus : public PlayerScript
{
public:
    PlayerScript_paladin_wod_pvp_4p_bonus() :PlayerScript("PlayerScript_paladin_wod_pvp_4p_bonus") {}

    void OnModifyPower(Player* p_Player, Powers p_Power, int32 p_OldValue, int32& p_NewValue, bool p_Regen)
    {
        if (p_Regen)
            return;

        if (p_OldValue > p_NewValue && p_Player->getClass() == CLASS_PALADIN && p_Player->GetSpecializationId(p_Player->GetActiveSpec()) == SPEC_PALADIN_RETRIBUTION && p_Power == POWER_HOLY_POWER)
        {
            // Get the power earn (if > 0 ) or consum (if < 0)
            int32 l_SpentPower = p_OldValue - p_NewValue;

            if (p_Player->HasAura(PALADIN_PVP_RETRIBUTION_4P_BONUS))
            {
                /// Spending Holy Power increases your damage and healing by 2 % per Holy Power
                int32 l_EffectValue = l_SpentPower * sSpellMgr->GetSpellInfo(PALADIN_VINDICATORS_FURY)->Effects[EFFECT_0].BasePoints;
                p_Player->CastCustomSpell(p_Player, PALADIN_VINDICATORS_FURY, &l_EffectValue, &l_EffectValue, &l_EffectValue, true);
            }
        }
    }
};

void AddSC_paladin_spell_scripts()
{
    new spell_pal_denounce();
    new spell_pal_enhanced_holy_shock();
    new spell_pal_light_of_dawn();
    new spell_pal_word_of_glory_damage();
    new spell_pal_word_of_glory_heal();
    new spell_pal_beacon_of_faith();
    new spell_pal_holy_shield();
    new spell_pal_divine_purpose();
    new spell_pal_hammer_of_wrath();
    new spell_pal_holy_wrath();
    new spell_pal_eternal_flame_periodic_heal();
    new spell_pal_eternal_flame();
    new spell_pal_glyph_of_devotian_aura();
    new spell_pal_glyph_of_devotian_trigger_aura();
    new spell_pal_exorcism_energize();
    new spell_pal_glyph_of_divine_storm();
    new spell_pal_daybreak();
    new spell_pal_hand_of_purity();
    new spell_pal_glyph_of_avenging_wrath();
    new spell_pal_shield_of_the_righteous();
    new spell_pal_selfless_healer();
    new spell_pal_tower_of_radiance();
    new spell_pal_sacred_shield();
    new spell_pal_sacred_shield_absorb();
    new spell_pal_emancipate();
    new spell_pal_art_of_war();
    new spell_pal_seal_of_insight();
    new spell_pal_blinding_light();
    new spell_pal_hand_of_protection();
    new spell_pal_cleanse();
    new spell_pal_divine_shield();
    new spell_pal_execution_sentence_dispel();
    new spell_pal_execution_sentence();
    new spell_pal_lights_hammer_tick();
    new spell_pal_lights_hammer();
    new spell_pal_holy_prism_visual();
    new spell_pal_holy_prism_effect();
    new spell_pal_holy_prism();
    new spell_pal_consecration();
    new spell_pal_consecration_area();
    new spell_pal_word_of_glory();
    new spell_pal_judgment();
    new spell_pal_ardent_defender();
    new spell_pal_blessing_of_faith();
    new spell_pal_holy_shock_heal();
    new spell_pal_holy_shock_damage();
    new spell_pal_holy_shock();
    new spell_pal_lay_on_hands();
    new spell_pal_righteous_defense();
    new spell_pal_seal_of_justice();
    new spell_pal_sanctified_wrath();
    new spell_pal_selfless_healer_proc();

    // Player Script
    new PlayerScript_empowered_divine_storm();
    new PlayerScript_saved_by_the_light();
    new PlayerScript_paladin_wod_pvp_4p_bonus();
}
