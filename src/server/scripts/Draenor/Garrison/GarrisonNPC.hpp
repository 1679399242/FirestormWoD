////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef GARRISON_NPC_HPP_GARRISON
#define GARRISON_NPC_HPP_GARRISON

#include "GarrisonScriptData.hpp"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison 
{
    /// Garrison Ford
    class npc_GarrisonFord : public CreatureScript
    {
        public:
            /// Constructor
            npc_GarrisonFord();

            /// Called when a player opens a gossip dialog with the creature.
            /// @p_Player   : Source player instance
            /// @p_Creature : Target creature instance
            virtual bool OnGossipHello(Player * p_Player, Creature * p_Creature) override;
            /// Called when a player selects a gossip item in the creature's gossip menu.
            /// @p_Player   : Source player instance
            /// @p_Creature : Target creature instance
            /// @p_Sender   : Sender menu
            /// @p_Action   : Action
            virtual bool OnGossipSelect(Player * p_Player, Creature * p_Creature, uint32 p_Sender, uint32 p_Action) override;

    };

    /// Garrison ROPE owner
    class npc_GarrisonCartRope : public CreatureScript
    {
        public:
            /// Constructor
            npc_GarrisonCartRope();

            /// Called when a CreatureAI object is needed for the creature.
            /// @p_Creature : Target creature instance
            virtual CreatureAI * GetAI(Creature * p_Creature) const override;

            /// AI Script
            struct npc_GarrisonCartRopeAI : public CreatureAI
            {
                /// Constructor
                npc_GarrisonCartRopeAI(Creature * p_Creature);

                /// On reset
                virtual void Reset() override;
                /// On AI Update
                /// @p_Diff : Time since last update
                virtual void UpdateAI(const uint32 p_Diff) override;

                bool HasRope;   ///< Has rope
            };

    };

    /// Garrison assistant brightstone
    class npc_AssistantBrightstone : public CreatureScript
    {
        public:
            /// Constructor
            npc_AssistantBrightstone();

            /// Called when a player opens a gossip dialog with the creature.
            /// @p_Player   : Source player instance
            /// @p_Creature : Target creature instance
            virtual bool OnGossipHello(Player * p_Player, Creature * p_Creature) override;
            /// Called when a player selects a gossip item in the creature's gossip menu.
            /// @p_Player   : Source player instance
            /// @p_Creature : Target creature instance
            /// @p_Sender   : Sender menu
            /// @p_Action   : Action
            virtual bool OnGossipSelect(Player * p_Player, Creature * p_Creature, uint32 p_Sender, uint32 p_Action) override;

    };

    /// Garrison vindicator maraad
    class npc_VindicatorMaraad : public CreatureScript
    {
        public:
            /// Constructor
            npc_VindicatorMaraad();

            /// Called when a player accepts a quest from the creature.
            /// @p_Player   : Source player instance
            /// @p_Creature : Target creature instance
            /// @p_Quest    : Accepted quest
            virtual bool OnQuestAccept(Player * p_Player, Creature * p_Creature, const Quest * p_Quest) override;
            /// Called when a player completes a quest with the creature.
            /// @p_Player   : Source player instance
            /// @p_Creature : Target creature instance
            /// @p_Quest    : Completed quest
            virtual bool OnQuestComplete(Player * p_Player, Creature * p_Creature, const Quest * p_Quest) override;

    };

    /// Garrison shelly hamby
    class npc_ShellyHamby : public CreatureScript
    {
        public:
            /// Constructor
            npc_ShellyHamby();

            /// Called when a player opens a gossip dialog with the creature.
            /// @p_Player   : Source player instance
            /// @p_Creature : Target creature instance
            virtual bool OnGossipHello(Player * p_Player, Creature * p_Creature) override;
            /// Called when a player selects a gossip item in the creature's gossip menu.
            /// @p_Player   : Source player instance
            /// @p_Creature : Target creature instance
            /// @p_Sender   : Sender menu
            /// @p_Action   : Action
            virtual bool OnGossipSelect(Player * p_Player, Creature * p_Creature, uint32 p_Sender, uint32 p_Action) override;

    };

    /// Garrison baros alexston
    class npc_BarosAlexsom : public CreatureScript
    {
        public:
            /// Constructor
            npc_BarosAlexsom();

            /// Called when a player accepts a quest from the creature.
            /// @p_Player   : Source player instance
            /// @p_Creature : Target creature instance
            /// @p_Quest    : Accepted quest
            virtual bool OnQuestAccept(Player * p_Player, Creature * p_Creature, const Quest * p_Quest) override;
            /// Called when a player completes a quest with the creature.
            /// @p_Player   : Source player instance
            /// @p_Creature : Target creature instance
            /// @p_Quest    : Completed quest
            virtual bool OnQuestComplete(Player * p_Player, Creature * p_Creature, const Quest * p_Quest) override;

    };

    /// Lunarfall Laborer
    class npc_LunarfallLaborer : public CreatureScript
    {
        public:
            /// Constructor
            npc_LunarfallLaborer();

            /// Called when a CreatureAI object is needed for the creature.
            /// @p_Creature : Target creature instance
            virtual CreatureAI * GetAI(Creature * p_Creature) const;

            /// Creature AI
            struct npc_LunarfallLaborerAI : public CreatureAI
            {
                /// Constructor
                npc_LunarfallLaborerAI(Creature * p_Creature);

                /// On AI Update
                /// @p_Diff : Time since last update
                virtual void UpdateAI(const uint32 p_Diff) override;
                /// Set UInt32 value
                /// @p_ID    : Value ID
                /// @p_Value : Value
                virtual void SetData(uint32 p_ID, uint32 p_Value) override;

            };

    };

    /// Garrison shelly hamby
    class npc_Skaggit : public CreatureScript
    {
        public:
            /// Constructor
            npc_Skaggit();

            /// Called when a player selects a gossip item in the creature's gossip menu.
            /// @p_Player   : Source player instance
            /// @p_Creature : Target creature instance
            /// @p_Sender   : Sender menu
            /// @p_Action   : Action
            virtual bool OnGossipSelect(Player * p_Player, Creature * p_Creature, uint32 p_Sender, uint32 p_Action) override;

    };

    /// Lady sena
    class npc_LadySena : public CreatureScript
    {
        public:
            /// Constructor
            npc_LadySena();

            /// Called when a player opens a gossip dialog with the creature.
            /// @p_Player   : Source player instance
            /// @p_Creature : Target creature instance
            virtual bool OnGossipHello(Player * p_Player, Creature * p_Creature) override;

    };

    /// Sergeant Grimjaw
    class npc_SergeantGrimjaw : public CreatureScript
    {
        public:
            /// Constructor
            npc_SergeantGrimjaw();

            /// Called when a player opens a gossip dialog with the creature.
            /// @p_Player   : Source player instance
            /// @p_Creature : Target creature instance
            virtual bool OnGossipHello(Player * p_Player, Creature * p_Creature) override;

    };

    /// Senior Peon II
    class npc_SeniorPeonII : public CreatureScript
    {
        public:
            /// Constructor
            npc_SeniorPeonII();

            /// Called when a player opens a gossip dialog with the creature.
            /// @p_Player   : Source player instance
            /// @p_Creature : Target creature instance
            virtual bool OnGossipHello(Player * p_Player, Creature * p_Creature) override;
            /// Called when a player selects a gossip item in the creature's gossip menu.
            /// @p_Player   : Source player instance
            /// @p_Creature : Target creature instance
            /// @p_Sender   : Sender menu
            /// @p_Action   : Action
            virtual bool OnGossipSelect(Player * p_Player, Creature * p_Creature, uint32 p_Sender, uint32 p_Action) override;

            /// Called when a CreatureAI object is needed for the creature.
            /// @p_Creature : Target creature instance
            CreatureAI * GetAI(Creature * p_Creature) const;

            /// Creature AI
            struct npc_SeniorPeonIIAI : public CreatureAI
            {
                /// Constructor
                npc_SeniorPeonIIAI(Creature * p_Creature);

                /// Called at waypoint reached or point movement finished
                /// @p_Type : Movement Type
                /// @p_ID   : Misc ID
                virtual void MovementInform(uint32 p_Type, uint32 p_ID) override;

                /// On AI Update
                /// @p_Diff : Time since last update
                virtual void UpdateAI(const uint32 p_Diff) override;

                std::queue<std::function<void()>> m_DelayedOperations;  ///< Delayed operations
            };

    };

    /// Garlowe
    class npc_Gazlowe : public CreatureScript
    {
        public:
            /// Constructor
            npc_Gazlowe();

            /// Called when a CreatureAI object is needed for the creature.
            /// @p_Creature : Target creature instance
            CreatureAI * GetAI(Creature * p_Creature) const;

            /// Creature AI
            struct npc_GazloweAI : public CreatureAI
            {
                /// Constructor
                npc_GazloweAI(Creature * p_Creature);

                /// Called at waypoint reached or point movement finished
                /// @p_Type : Movement Type
                /// @p_ID   : Misc ID
                virtual void MovementInform(uint32 p_Type, uint32 p_ID) override;

                /// On AI Update
                /// @p_Diff : Time since last update
                virtual void UpdateAI(const uint32 p_Diff) override;

                std::queue<std::function<void()>> m_DelayedOperations;  ///< Delayed operations
            };

    };

    /// Frostwall Peon
    class npc_FrostwallPeon : public CreatureScript
    {
        public:
            /// Constructor
            /// @p_ScriptName : Script name
            npc_FrostwallPeon(const std::string & p_ScriptName);

            /// Called when a CreatureAI object is needed for the creature.
            /// @p_Creature : Target creature instance
            CreatureAI * GetAI(Creature * p_Creature) const;

            /// Creature AI
            struct npc_FrostwallPeonAI : public CreatureAI
            {
                /// Constructor
                npc_FrostwallPeonAI(Creature * p_Creature);

                /// Called at waypoint reached or point movement finished
                /// @p_Type : Movement Type
                /// @p_ID   : Misc ID
                virtual void MovementInform(uint32 p_Type, uint32 p_ID) override;

                /// On AI Update
                /// @p_Diff : Time since last update
                virtual void UpdateAI(const uint32 p_Diff) override;
                /// Set UInt32 value
                /// @p_ID    : Value ID
                /// @p_Value : Value
                virtual void SetData(uint32 p_ID, uint32 p_Value) override;

                /// Select a random tree ID
                uint32 SelectRandomTree();

                uint32 m_LastTalkTimer;             ///< Last talk timer
                uint32 m_TalkInterval;              ///< Talk interval
                uint32 m_Phase;                     ///< Current phase ID
                int32 m_ThreeID;                    ///< Current Tree ID

                int32 m_WoodCuttingRemainingTime;   ///< Wood cutting remaining animation time
                int32 m_WoodCuttingCycleCount;      ///< Wood cutting cycle count

                float m_SpawnX;                     ///< Spawn position X
                float m_SpawnY;                     ///< Spawn position Y
                float m_SpawnZ;                     ///< Spawn position Z
                float m_SpawnO;                     ///< Spawn position O

                bool m_BackToSpawnPos;              ///< He is back to spawn position
                bool m_IsDynamicScript;             ///< Is this an dynamic script instance
                bool m_Initialised;                 ///< Script is initialized

                std::queue<std::function<void()>> m_DelayedOperations;  ///< Delayed operations
            };

    };

}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_NPC_HPP_GARRISON