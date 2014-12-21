/*
 * Copyright (C) 2012-2014 JadeCore <http://www.pandashan.com>
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2006-2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "siege_of_orgrimmar.h"

enum eSays
{
};

enum eSpells
{
    SPEll_ASHEN_WALL          = 144070,
    SPEll_ASHFLAR_TOTEM       = 144290,
    SPEll_BERSERK             = 47008,
    SPEll_BLOODLUST           = 144302,
    SPEll_FOUL_STREAM         = 144090,
    SPEll_FOULSTREAM_TOTEM    = 144289,
    SPEll_FROSTSTORM_STRIKE   = 144215,
    SPEll_IRON_TOMB           = 144328,
    SPEll_POISONMIST_TOTEM    = 144288,
    SPEll_RUSTED_IRON_TOTEM   = 144291,
    SPEll_TOXIC_MIST          = 144089
};

enum eEvents
{
};

class boss_earthbreaker_haromm : public MS::Game::Scripting::Interfaces::CreatureScript
{
    public:
        boss_earthbreaker_haromm() : MS::Game::Scripting::Interfaces::CreatureScript("boss_earthbreaker_haromm") { }

        struct boss_earthbreaker_harommAI : public BossAI
        {
            boss_earthbreaker_harommAI(Creature* creature) : BossAI(creature, DATA_KORKRON_DARK_SHAMANS)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;

            void Reset()
            {
                events.Reset();
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_earthbreaker_harommAI(creature);
        }
};

class boss_bloodclaw : public MS::Game::Scripting::Interfaces::CreatureScript
{
    public:
        boss_bloodclaw() : MS::Game::Scripting::Interfaces::CreatureScript("boss_bloodclaw") { }

        struct boss_bloodclawAI : public BossAI
        {
            boss_bloodclawAI(Creature* creature) : BossAI(creature, DATA_KORKRON_DARK_SHAMANS)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;

            void Reset()
            {
                events.Reset();
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_bloodclawAI(creature);
        }
};

class mob_darkfang : public MS::Game::Scripting::Interfaces::CreatureScript
{
    public:
        mob_darkfang() : MS::Game::Scripting::Interfaces::CreatureScript("mob_darkfang") { }

        struct mob_darkfangAI : public ScriptedAI
        {
            mob_darkfangAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;

		    void Reset()
            {
                events.Reset();
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_darkfangAI(creature);
        }
};

class mob_foul_slime : public MS::Game::Scripting::Interfaces::CreatureScript
{
    public:
        mob_foul_slime() : MS::Game::Scripting::Interfaces::CreatureScript("mob_foul_slime") { }

        struct mob_foul_slimeAI : public ScriptedAI
        {
            mob_foul_slimeAI(Creature* pCreature) : ScriptedAI(pCreature)
            {
                pInstance = pCreature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;

            void Reset()
            {
                events.Reset();
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;
            }
        };

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new mob_foul_slimeAI(pCreature);
        }
};

void AddSC_korkron_dark_shamans()
{
    new boss_earthbreaker_haromm();
    new boss_bloodclaw();
    new mob_darkfang();
    new mob_foul_slime();
}
