////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

#include "shadowmoon_burial_grounds.hpp"

enum eNerzulSpells
{
    // Ritual of Bones: 76518   
    SpellMalevolance = 154442,
    SpellOmenOfDeathPeriodicSpell = 175988,
    SpellOmenOfDeathSummon = 154350,
    SpellOmenOfDeathVisualRune = 154351,
    SpellOmenOfDeathLightningt = 154352,
    SpellOmenOfDeathLightningDamageTick = 154353,
    SpellOmenOfDeathBreath = 175988,
    SpellOmenOfDeathDummy = 177691,
    SpellRitualOfBones = 154469,
    SpellRitualOfBonesDamage = 154468,
    SpellRitualOfBonesDot = 154469,
    SpellRitualOfBonesThirdVisual = 154559,
    SpellRitualOfBonesPeriodic = 156312,
    SpellRitualOfBonesWeirdVisualPoop = 160445,
    SpellRitualOfBonesInvisibiltiy = 160537,

    // RP
    SpellNerzulChannel = 160674,
};

enum eNerzulEvents
{
    // Ritual of Bones: 76518 
    EventMalevolance,
    EventRitualOfSouls,
    EventOmenOfDeath,
};

enum eNerzulTalks
{
    TalkAggro = 1, ///< The Shadowmoon cannot be defeated.. we walk in the realm of death itself!  [43682]
    TalkDeath,     ///< My power... will repel across eternity.. [43683]
    TalkIntro01,   ///< The darkness it flows within me.. it seeps from every vein. Come stranger gaze into the eye of the void! Tender your souls... to ME! *Nerz'ul laughs* [43684]
    TalkIntro02,   ///< Peer into the gateway.. I command! cross into the realm of the shadowland.. they should deliever you to your death! [43685]
    TalkKill01,    ///< You soul.. shall serve me... [43686]
    TalkKill02,    ///< You bend to my will...  [43687]
    TalkSpell01,   ///< I am the herald of the END! [43688]
    TalkSpell02,   ///< A kindeling of bones and shadows.. army of the dead.. ARISE! [43689]
    TalkSpell03,   ///< Into the ABYSS! [43690]
    TalkSpell04,   ///< Feel the void, cold embrace.. [43691]
}; 

enum eNerzulCreatures
{
    CreatureOmenOfDeath                  = 76462,
    CreatureRitualOfBones                = 76518,
    CreatureRitualOfBonesDarknessTrigger = 534556,
    CreatureVoidDoorSpawnCounter         = 645456,
};

enum eNerzulActions
{
    ActionNerzulPropIntroduction = 1,
};

Position l_NerzulTeleportPosition = {1723.754f, -799.859f, 73.735f, 4.222427f};

Position l_InitialPositionRight = { 1694.928f, -785.322f, 73.735f, 4.234663f };
Position l_InitialPositionLeft  = { 1677.648f, -825.009f, 73.306f, 0.882368f };

/// Nerz'ul - 76407
class boss_nerzul : public CreatureScript
{
public:
    boss_nerzul() : CreatureScript("boss_nerzul") { }

    struct boss_nerzulAI : public BossAI
    {
        boss_nerzulAI(Creature* p_Creature) : BossAI(p_Creature, eShadowmoonBurialGroundsDatas::DataBossNerzul)
        {
            m_Instance = me->GetInstanceScript();
        }

        InstanceScript* m_Instance;
        float m_X;
        float m_Y;

        void Reset() override
        {
            events.Reset();
            me->setFaction(16);

            m_X = 10.375f;
            m_Y = 4.538f;
        }

        void JustReachedHome() override
        {
            _JustReachedHome();
            summons.DespawnAll();

            DespawnCreaturesInArea(eNerzulCreatures::CreatureOmenOfDeath, me);
            DespawnCreaturesInArea(eNerzulCreatures::CreatureRitualOfBones, me);
            DespawnCreaturesInArea(eNerzulCreatures::CreatureRitualOfBonesDarknessTrigger, me);
        }

        void EnterCombat(Unit* p_Who) override
        {
            _EnterCombat();

            Talk(eNerzulTalks::TalkAggro);

            if (m_Instance != nullptr)
            {
                m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_ENGAGE, me);
                m_Instance->SetBossState(eShadowmoonBurialGroundsDatas::DataBossNerzul, EncounterState::IN_PROGRESS);

                DoZoneInCombat();
            }

            events.ScheduleEvent(eNerzulEvents::EventOmenOfDeath, TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eNerzulEvents::EventRitualOfSouls, TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eNerzulEvents::EventMalevolance, TimeConstants::IN_MILLISECONDS);
        }

        void KilledUnit(Unit* p_Who) override
        {
            if (p_Who->GetTypeId() == TypeID::TYPEID_PLAYER)
            {
                switch (urand(0, 1))
                {
                case 0:
                    Talk(eNerzulTalks::TalkKill01);
                    break;
                case 1:
                    Talk(eNerzulTalks::TalkKill02);
                    break;
                }
            }
        }

        void JustDied(Unit* /*p_Killer*/) override
        {
            _JustDied();

            Talk(eNerzulTalks::TalkDeath);
        }

        void UpdateAI(uint32 const p_Diff) override
        {
            events.Update(p_Diff);

            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                return;

            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case eNerzulEvents::EventMalevolance:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true))
                            me->CastSpell(l_Target, eNerzulSpells::SpellMalevolance);

                        Talk(eNerzulTalks::TalkSpell01);

                        events.ScheduleEvent(eNerzulEvents::EventMalevolance, TimeConstants::IN_MILLISECONDS);
                        break;
                    case eNerzulEvents::EventOmenOfDeath:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true))
                            me->CastSpell(l_Target, eNerzulSpells::SpellOmenOfDeathSummon);

                        Talk(eNerzulTalks::TalkSpell03);

                        events.ScheduleEvent(eNerzulEvents::EventOmenOfDeath, TimeConstants::IN_MILLISECONDS);
                        break;
                    case eNerzulEvents::EventRitualOfSouls:
                    {
                        Talk(eNerzulTalks::TalkSpell02);

                        switch (urand(0, 1))
                        {
                            case 0: // Right
                            {
                                for (int i = 0; i <= 6; i++)
                                {
                                    me->SummonCreature(eNerzulCreatures::CreatureRitualOfBones, l_InitialPositionRight.GetPositionX() + (m_X * i),
                                        l_InitialPositionRight.GetPositionY() + (m_Y * i), l_InitialPositionRight.GetPositionZ(),
                                        l_InitialPositionRight.GetOrientation(), TEMPSUMMON_TIMED_DESPAWN, 40 * TimeConstants::IN_MILLISECONDS);
                                }
                                break;
                            }
                            case 1: // Left
                            {
                                for (int i = 0; i <= 6; i++)
                                {
                                    me->SummonCreature(eNerzulCreatures::CreatureRitualOfBones, l_InitialPositionLeft.GetPositionX() + (m_X * i),
                                        l_InitialPositionLeft.GetPositionY() + (m_Y * i), l_InitialPositionLeft.GetPositionZ(),
                                        l_InitialPositionLeft.GetOrientation(), TEMPSUMMON_TIMED_DESPAWN, 40 * TimeConstants::IN_MILLISECONDS);
                                }
                                break;
                            }
                        }
                        break;
                    }
                }
            }

            DoMeleeAttackIfReady();
        }

        static void DespawnCreaturesInArea(uint32 entry, WorldObject* object)
        {
            std::list<Creature*> creatures;
            GetCreatureListWithEntryInGrid(creatures, object, entry, 5000.0f);
            if (creatures.empty())
                return;

            for (std::list<Creature*>::iterator iter = creatures.begin(); iter != creatures.end(); ++iter)
                (*iter)->DespawnOrUnsummon();
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new boss_nerzulAI(p_Creature);
    }
};

/// Omen Of Death - 76462
class shadowmoon_burial_grounds_omen_of_death : public CreatureScript
{
public:
    shadowmoon_burial_grounds_omen_of_death() : CreatureScript("shadowmoon_burial_grounds_omen_of_death") {}

    struct shadowmoon_burial_grounds_creaturesAI : public Scripted_NoMovementAI
    {
        shadowmoon_burial_grounds_creaturesAI(Creature* p_Creature) : Scripted_NoMovementAI(p_Creature)
        {
            m_Instance = p_Creature->GetInstanceScript();
        }

        InstanceScript* m_Instance;
        uint32 m_Diff;
        uint32 l_Periodic = NULL;

        void Reset() override
        {
            me->SetReactState(ReactStates::REACT_PASSIVE);
            me->SetFlag(UNIT_FIELD_FLAGS2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE);

            me->CastSpell(me, eNerzulSpells::SpellOmenOfDeathVisualRune);
            me->CastSpell(me, eNerzulSpells::SpellOmenOfDeathLightningt);
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new shadowmoon_burial_grounds_creaturesAI(p_Creature);
    }
};

/// Ritual Of Bones - 76518
class shadowmoon_burial_grounds_ritual_of_bones : public CreatureScript
{
public:
    shadowmoon_burial_grounds_ritual_of_bones() : CreatureScript("shadowmoon_burial_grounds_ritual_of_bones") {}

    struct shadowmoon_burial_grounds_creaturesAI : public Scripted_NoMovementAI
    {
        shadowmoon_burial_grounds_creaturesAI(Creature* p_Creature) : Scripted_NoMovementAI(p_Creature)
        {
            m_Instance = p_Creature->GetInstanceScript();
        }

        InstanceScript* m_Instance;
        uint32 m_Diff;
        uint32 l_Periodic = NULL;

        void Reset() override
        {
            {
                const SpellInfo* l_SpellInfo = sSpellMgr->GetSpellInfo(eNerzulSpells::SpellRitualOfBonesPeriodic);

                if (!l_SpellInfo)
                    return;

                uint32 l_Periodic = l_SpellInfo->Effects[0].Amplitude;

                m_Diff = l_Periodic;
            }

            me->RemoveAllAuras();
            me->SetSpeed(UnitMoveType::MOVE_RUN, 0.5f);
            me->SetReactState(ReactStates::REACT_PASSIVE);
            me->SetFlag(UNIT_FIELD_FLAGS2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);
            me->AddUnitMovementFlag(MovementFlags::MOVEMENTFLAG_FORWARD);

            me->CastSpell(me, eNerzulSpells::SpellRitualOfBonesWeirdVisualPoop);
        }

        void UpdateAI(uint32 const p_Diff) override
        {
            events.Update(p_Diff);

            if (m_Diff <= p_Diff)
            {
                me->SummonCreature(eNerzulCreatures::CreatureRitualOfBonesDarknessTrigger, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation(), TEMPSUMMON_MANUAL_DESPAWN);
                m_Diff = l_Periodic;
            }
            else
                m_Diff -= p_Diff;
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new shadowmoon_burial_grounds_creaturesAI(p_Creature);
    }
};

/// Ritual Of Bones Darkness Trigger - 534556
class shadowmoon_burial_grounds_ritual_of_bones_darkness_trigger : public CreatureScript
{
public:
    shadowmoon_burial_grounds_ritual_of_bones_darkness_trigger() : CreatureScript("shadowmoon_burial_grounds_ritual_of_bones_darkness_trigger") {}

    struct shadowmoon_burial_grounds_creaturesAI : public Scripted_NoMovementAI
    {
        shadowmoon_burial_grounds_creaturesAI(Creature* p_Creature) : Scripted_NoMovementAI(p_Creature)
        {
            m_Instance = p_Creature->GetInstanceScript();
        }

        InstanceScript* m_Instance;

        void Reset() override
        {
            // Orientation check
            if (TempSummon* l_Tempo = me->ToTempSummon())
            {
                if (Unit* l_Summoner = l_Tempo->ToTempSummon())
                {
                    me->SetFacingTo(l_Summoner->GetOrientation());
                }
            }

            me->SetDisplayId(11686);
            me->setFaction(35);
            me->SetReactState(ReactStates::REACT_PASSIVE);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_DISABLE_MOVE);

            me->CastSpell(me, eNerzulSpells::SpellRitualOfBonesThirdVisual);
            me->CastSpell(me, eNerzulSpells::SpellRitualOfBonesWeirdVisualPoop);
        }

        void UpdateAI(uint32 const p_Diff) override
        {
            events.Update(p_Diff);

            std::list<Player*> l_ListPlayer;
            me->GetPlayerListInGrid(l_ListPlayer, 100.0f);

            if (l_ListPlayer.empty())
                return;

            for (auto itr : l_ListPlayer)
            {
                if (itr->isInBack(me))
                {
                    itr->AddAura(eNerzulSpells::SpellRitualOfBonesDot, itr);

                    if (AuraPtr l_AuraPtr = itr->GetAura(eNerzulSpells::SpellRitualOfBonesDot))
                    {
                        l_AuraPtr->SetDuration(1);
                    }
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new shadowmoon_burial_grounds_creaturesAI(p_Creature);
    }
};

/// Nerzul initial Teleport - 342521
class shadowmoon_burial_grounds_initial_teleport : public CreatureScript
{
public:
    shadowmoon_burial_grounds_initial_teleport() : CreatureScript("shadowmoon_burial_grounds_initial_teleport") {}

    struct shadowmoon_burial_grounds_creaturesAI : public Scripted_NoMovementAI
    {
        shadowmoon_burial_grounds_creaturesAI(Creature* p_Creature) : Scripted_NoMovementAI(p_Creature)
        {
            m_Instance = p_Creature->GetInstanceScript();
            m_HasOpened = false;
        }

        InstanceScript* m_Instance;

        uint32 m_Counter;
        bool m_HasOpened;

        void Reset() override
        {
            me->SetDisplayId(11686);
            me->setFaction(35);
            me->SetReactState(ReactStates::REACT_PASSIVE);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
        }

        void UpdateAI(uint32 const p_Diff) override
        {
            events.Update(p_Diff);
            
            if (m_Instance == nullptr)
                return;

            if (Creature* l_Nerzul = m_Instance->instance->GetCreature(m_Instance->GetData64(eShadowmoonBurialGroundsDatas::DataBossNerzul)))
            {
                if (l_Nerzul->isInCombat())
                    return;

                if (Player* l_NearestPlayer = me->FindNearestPlayer(5.0f, true))
                {
                    l_NearestPlayer->NearTeleportTo(l_NerzulTeleportPosition.GetPositionX(), l_NerzulTeleportPosition.GetPositionY(), l_NerzulTeleportPosition.GetPositionZ(), l_NerzulTeleportPosition.GetOrientation());
                }
            }
        }

        void DoAction(int32 const p_Action) override
        {
            switch (p_Action)
            {
                case eShadowmoonBurialGroundsActions::ActionCountVoidSpawnNearNerzulGate:
                {
                    if (!m_HasOpened)
                    {
                        if (m_Counter < 2)
                        {
                            m_Counter++;
                        }
                        else
                        {
                            m_HasOpened = true;

                            if (m_Instance != nullptr)
                            {
                                if (GameObject* l_NerzulDoor = m_Instance->instance->GetGameObject(m_Instance->GetData64(eShadowmoonBurialGroundsDatas::DataNerzulDoor)))
                                {
                                    l_NerzulDoor->SetLootState(LootState::GO_READY);
                                    l_NerzulDoor->UseDoorOrButton(10 * TimeConstants::IN_MILLISECONDS, false, me);


                                    if (Creature* l_NerzulProp = m_Instance->instance->GetCreature(m_Instance->GetData64(eShadowmoonBurialGroundsDatas::DataNerzulProp)))
                                    {
                                        if (l_NerzulProp->GetAI())
                                            l_NerzulProp->GetAI()->DoAction(eNerzulActions::ActionNerzulPropIntroduction);
                                    }
                                }
                            }
                        }
                    }
                    break;
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new shadowmoon_burial_grounds_creaturesAI(p_Creature);
    }
};

/// Nerzul prop - 79497
class shadowmoon_burial_grounds_nerzul_prop : public CreatureScript
{
public:
    shadowmoon_burial_grounds_nerzul_prop() : CreatureScript("shadowmoon_burial_grounds_nerzul_prop") {}

    struct shadowmoon_burial_grounds_creaturesAI : public Scripted_NoMovementAI
    {
        shadowmoon_burial_grounds_creaturesAI(Creature* p_Creature) : Scripted_NoMovementAI(p_Creature)
        {
            m_Instance = p_Creature->GetInstanceScript();
            m_Intro = false;
        }

        InstanceScript* m_Instance;
        bool m_Intro;

        void Reset() override
        {
            me->SetDisplayId(11686);
            me->setFaction(35);
            me->SetReactState(ReactStates::REACT_PASSIVE);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
            me->SetDisableGravity(true);
            me->SetCanFly(true);

           // me->CastSpell(me, eNerzulSpells::);
        }

        void DoAction(int32 const p_Action) override
        {
            switch (p_Action)
            {
                case eNerzulActions::ActionNerzulPropIntroduction:
                    if (!m_Intro)
                    {
                        events.ScheduleEvent(eShadowmoonBurialGroundsEvents::EventTalk01, 1 * TimeConstants::IN_MILLISECONDS);
                    }
                    break;
            }
        }

        void UpdateAI(uint32 const p_Diff) override
        {
            events.Update(p_Diff);

            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case eShadowmoonBurialGroundsEvents::EventTalk01:
                        Talk(eNerzulTalks::TalkIntro01);

                        events.ScheduleEvent(eShadowmoonBurialGroundsEvents::EventTalk02, 10 * TimeConstants::IN_MILLISECONDS);
                        break;
                    case eShadowmoonBurialGroundsEvents::EventTalk02:
                        Talk(eNerzulTalks::TalkIntro02);
                        break;
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new shadowmoon_burial_grounds_creaturesAI(p_Creature);
    }
};

void AddSC_nerzul()
{
    new boss_nerzul();

    new shadowmoon_burial_grounds_nerzul_prop();
    new shadowmoon_burial_grounds_initial_teleport();
    new shadowmoon_burial_grounds_omen_of_death();
    new shadowmoon_burial_grounds_ritual_of_bones();
    new shadowmoon_burial_grounds_ritual_of_bones_darkness_trigger();
}