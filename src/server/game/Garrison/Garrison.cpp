#include "Garrison.h"
#include "Player.h"
#include "DatabaseEnv.h"
#include "ObjectMgr.h"
#include "ObjectAccessor.h"

uint32 gGarrisonInGarrisonAreaID[GARRISON_FACTION_COUNT] =
{
    7004,   ///< Horde
    7078,   ///< Alliance
};

uint32 gGarrisonEmptyPlotGameObject[GARRISON_PLOT_TYPE_MAX * GARRISON_FACTION_COUNT] =
{
    /// Horde
    233083,     ///< GARRISON_PLOT_TYPE_SMALL
    232425,     ///< GARRISON_PLOT_TYPE_MEDIUM
    233081,     ///< GARRISON_PLOT_TYPE_LARGE
    232415,     ///< GARRISON_PLOT_TYPE_FARM
    232447,     ///< GARRISON_PLOT_TYPE_MINE
    232426,     ///< GARRISON_PLOT_TYPE_FISHING_HUT
    231706,     ///< GARRISON_PLOT_TYPE_PET_MENAGERIE
    /// Alliance
    229501,     ///< GARRISON_PLOT_TYPE_SMALL
    232283,     ///< GARRISON_PLOT_TYPE_MEDIUM
    232143,     ///< GARRISON_PLOT_TYPE_LARGE
    232286,     ///< GARRISON_PLOT_TYPE_FARM
    233485,     ///< GARRISON_PLOT_TYPE_MINE
    237223,     ///< GARRISON_PLOT_TYPE_FISHING_HUT
    0,          ///< GARRISON_PLOT_TYPE_PET_MENAGERIE
};

uint32 gGarrisonBuildingPlotGameObject[GARRISON_PLOT_TYPE_MAX * GARRISON_FACTION_COUNT] =
{
    /// Horde
    233958,     ///< GARRISON_PLOT_TYPE_SMALL
    232373,     ///< GARRISON_PLOT_TYPE_MEDIUM
    232410,     ///< GARRISON_PLOT_TYPE_LARGE
    232373,     ///< GARRISON_PLOT_TYPE_FARM          same as GARRISON_PLOT_TYPE_MEDIUM
    232373,     ///< GARRISON_PLOT_TYPE_MINE          same as GARRISON_PLOT_TYPE_MEDIUM
    233958,     ///< GARRISON_PLOT_TYPE_FISHING_HUT   same as GARRISON_PLOT_TYPE_SMALL
    233958,     ///< GARRISON_PLOT_TYPE_PET_MENAGERIE same as GARRISON_PLOT_TYPE_SMALL
    /// Alliance
    233957,     ///< GARRISON_PLOT_TYPE_SMALL
    232409,     ///< GARRISON_PLOT_TYPE_MEDIUM
    232411,     ///< GARRISON_PLOT_TYPE_LARGE
    232409,     ///< GARRISON_PLOT_TYPE_FARM          same as GARRISON_PLOT_TYPE_MEDIUM
    232409,     ///< GARRISON_PLOT_TYPE_MINE          same as GARRISON_PLOT_TYPE_MEDIUM
    233957,     ///< GARRISON_PLOT_TYPE_FISHING_HUT   same as GARRISON_PLOT_TYPE_SMALL
    233957,     ///< GARRISON_PLOT_TYPE_PET_MENAGERIE same as GARRISON_PLOT_TYPE_SMALL
};

float gGarrisonBuildingPlotAABBDiminishReturnFactor[GARRISON_PLOT_TYPE_MAX * GARRISON_FACTION_COUNT] =
{
    /// Horde
    0,          ///< GARRISON_PLOT_TYPE_SMALL
    0,          ///< GARRISON_PLOT_TYPE_MEDIUM
    0,          ///< GARRISON_PLOT_TYPE_LARGE
    0,          ///< GARRISON_PLOT_TYPE_FARM          same as GARRISON_PLOT_TYPE_MEDIUM
    0,          ///< GARRISON_PLOT_TYPE_MINE          same as GARRISON_PLOT_TYPE_MEDIUM
    0,          ///< GARRISON_PLOT_TYPE_FISHING_HUT   same as GARRISON_PLOT_TYPE_SMALL
    0,          ///< GARRISON_PLOT_TYPE_PET_MENAGERIE same as GARRISON_PLOT_TYPE_SMALL
    /// Alliance
    10,         ///< GARRISON_PLOT_TYPE_SMALL
    16,         ///< GARRISON_PLOT_TYPE_MEDIUM
    24,         ///< GARRISON_PLOT_TYPE_LARGE
    16,         ///< GARRISON_PLOT_TYPE_FARM          same as GARRISON_PLOT_TYPE_MEDIUM
    16,         ///< GARRISON_PLOT_TYPE_MINE          same as GARRISON_PLOT_TYPE_MEDIUM
    10,         ///< GARRISON_PLOT_TYPE_FISHING_HUT   same as GARRISON_PLOT_TYPE_SMALL
    10,         ///< GARRISON_PLOT_TYPE_PET_MENAGERIE same as GARRISON_PLOT_TYPE_SMALL
};

uint32 gGarrisonBuildingActivationGameObject[GARRISON_FACTION_COUNT] = {
    233248,     ///< Horde
    233250      ///< Alliance
};

GarrisonPlotInstanceInfoLocation gGarrisonPlotInstanceInfoLocation[GARRISON_PLOT_INSTANCE_COUNT] = 
{
    /// SiteLevelID PlotInstanceID      X            Y            Z           O
    /// Alliance Level 1
    {       5,          19,         1829.896f,    197.5504f,    72.00920f,   1.8849560f  },
    {       5,          23,         1911.550f,    232.9792f,    76.65489f,   2.7952700f  },
    /// Alliance Level 2
    {     444,          18,         1819.583f,    231.2813f,    72.17403f,  -1.2915440f  },
    {     444,          19,         1829.896f,    197.5504f,    71.98585f,   1.8849560f  },
    {     444,          22,         1864.955f,    320.2083f,    81.66045f,  -1.4835300f  },
    {     444,          23,         1918.637f,    228.7674f,    76.63956f,   2.7750740f  },
    {     444,          59,         1845.083f,    146.2743f,    53.43811f,   0.3490658f  },
    {     444,          63,         1847.615f,    134.7257f,    78.10705f,   2.7052600f  },
    {     444,          67,         2031.594f,    174.4410f,    84.59409f,   2.8361600f  },
    /// Alliance Level 3
    {       6,          18,         1819.583f,    231.2813f,    72.17403f,  -1.2915440f  },
    {       6,          19,         1830.528f,    196.7465f,    71.98586f,   1.9024090f  },
    {       6,          20,         1804.333f,    189.1458f,    70.07482f,   1.9634950f  },
    {       6,          22,         1864.955f,    320.2083f,    81.66045f,  -1.4835300f  },
    {       6,          23,         1918.637f,    228.7674f,    76.63956f,   2.7750740f  },
    {       6,          24,         1814.323f,    286.3941f,    76.62489f,  -0.6544983f  },
    {       6,          25,         1893.337f,    185.1684f,    78.05621f,   2.5307280f  },
    {       6,          59,         1845.083f,    146.2743f,    53.41686f,   0.3490658f  },
    {       6,          63,         1847.615f,    134.7257f,    78.10705f,   2.7052600f  },
    {       6,          67,         2031.594f,    174.4410f,    84.36597f,   2.8361600f  },
    {       6,          81,         1914.083f,    354.1875f,    88.96585f,  -1.8763060f  },
    /// Horde Level 1
    {     258,          18,         5645.124f,   4508.9600f,   119.27010f,   2.0423500f  },
    {     258,          23,         5575.461f,   4459.3380f,   130.36810f,   0.9599311f  },
    /// Horde level 2
    {     445,          18,         5645.124f,   4508.9600f,   119.27010f,   2.0420350f  },
    {     445,          19,         5665.686f,   4549.2660f,   119.27020f,  -2.7488930f  },
    {     445,          22,         5525.882f,   4523.5690f,   131.71830f,  -0.0436332f  },
    {     445,          23,         5574.562f,   4460.0310f,   130.36800f,   0.9424779f  },
    {     445,          59,         5399.831f,   4465.9760f,   114.46130f,   0.8464847f  },
    {     445,          63,         5415.374f,   4586.4390f,   136.62080f,   2.3561950f  },
    {     445,          67,         5476.589f,   4622.7070f,   134.44980f,  -1.3962630f  },
    /// Horde level 3
    {     259,          18,         5643.449f,   4503.7780f,   119.27010f,   1.6318830f  },
    {     259,          19,         5665.686f,   4549.2660f,   119.26810f,  -2.7488930f  },
    {     259,          20,         5617.798f,   4510.2760f,   119.27020f,   0.8028514f  },
    {     259,          22,         5527.659f,   4523.2130f,   131.72760f,  -0.0436332f  },
    {     259,          23,         5575.183f,   4459.6670f,   130.36800f,   0.9424779f  },
    {     259,          24,         5651.279f,   4441.2680f,   130.52540f,   1.8936820f  },
    {     259,          25,         5693.363f,   4471.8970f,   130.52540f,   2.3300140f  },
    {     259,          59,         5399.831f,   4465.9760f,   114.53970f,   0.8464847f  },
    {     259,          63,         5415.374f,   4586.4390f,   136.58310f,   2.3561950f  },
    {     259,          67,         5476.589f,   4622.7070f,   134.44980f,  -1.3962630f  },
    {     259,          81,         5626.042f,   4660.6960f,   142.45800f,  -1.9024090f  }
};

GarrisonCacheInfoLocation gGarrisonCacheInfoLocation[GARRISON_FACTION_COUNT * GARRISON_MAX_LEVEL] =
{
    /// SiteLevelID         X          Y          Z         O
    /// Horde Level 1
    {     258,             0.f,       0.f,       0.f,      0.f        },
    /// Horde level 2
    {     445,             0.f,       0.f,       0.f,      0.f        },
    /// Horde level 3
    {     259,          5592.272,  4589.9390f, 136.66830f, 5.858787f  },
    /// Alliance Level 1
    {       5,          1893.729f,  208.8733f,  77.06371f, 1.685312f  },
    /// Alliance Level 2
    {     444,             0.f,       0.f,       0.f,      0.f        },
    /// Alliance Level 3
    {       6,             0.f,       0.f,       0.f,      0.f        },
};

uint32 gGarrisonCacheGameObjectID[GARRISON_FACTION_COUNT * 3] =
{
    /// Horde
    237191,         ///< Normal
    237720,         ///< Hefty
    237722,         ///< Full
    /// Alliance
    236916,         ///< Normal
    237723,         ///< Hefty
    237724,         ///< Full
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Constructor
Garrison::Garrison(Player * p_Owner)
    : m_Owner(p_Owner)
{
    m_GarrisonLevel     = 1;
    m_GarrisonLevelID   = 0;
    m_GarrisonSiteID    = 0;

    m_NumFollowerActivation                = 1;
    m_NumFollowerActivationRegenTimestamp  = time(0);
    m_CacheLastUsage                       = time(0);

    m_CacheGameObjectGUID = 0;

    m_CacheLastTokenAmount = 0;

    /// Select Garrison site ID
    switch (GetGarrisonFactionIndex())
    {
        case GARRISON_FACTION_ALLIANCE:
            m_GarrisonSiteID = 2;
            break;

        case GARRISON_FACTION_HORDE:
            m_GarrisonSiteID = 71;
            break;
    }
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Create the garrison
void Garrison::Create()
{
    PreparedStatement* l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_GARRISON);

    uint32 l_Index = 0;
    l_Stmt->setUInt32(l_Index++, m_Owner->GetGUIDLow());
    l_Stmt->setUInt32(l_Index++, m_GarrisonLevel);

    CharacterDatabase.Query(l_Stmt);

    l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_GARRISON_DB_ID);

    l_Index = 0;
    l_Stmt->setUInt32(l_Index++, m_Owner->GetGUIDLow());

    PreparedQueryResult l_Result = CharacterDatabase.Query(l_Stmt);

    if (!l_Result)
        assert(false && "Garrison::Create() failed to retrieve created garrison ID");

    Field * l_Fields = l_Result->Fetch();
    
    m_ID = l_Fields[0].GetUInt32();

    Init();
}
/// Load
bool Garrison::Load()
{
    PreparedStatement * l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_GARRISON);

    uint32 l_Index = 0;
    l_Stmt->setUInt32(0, m_Owner->GetGUIDLow());

    PreparedQueryResult l_Result = CharacterDatabase.Query(l_Stmt);

    if (l_Result)
    {
        Field * l_Fields = l_Result->Fetch();

        m_ID            = l_Fields[0].GetUInt32();
        m_GarrisonLevel = l_Fields[1].GetUInt32();

        if (!l_Fields[2].GetString().empty())
        {
            Tokenizer l_BluePrints(l_Fields[2].GetString(), ' ');

            for (Tokenizer::const_iterator l_It = l_BluePrints.begin(); l_It != l_BluePrints.end(); ++l_It)
                m_KnownBlueprints.push_back(atol(*l_It));
        }

        if (!l_Fields[3].GetString().empty())
        {
            Tokenizer l_Specializations(l_Fields[3].GetString(), ' ');

            for (Tokenizer::const_iterator l_It = l_Specializations.begin(); l_It != l_Specializations.end(); ++l_It)
                m_KnownSpecializations.push_back(atol(*l_It));
        }

        m_NumFollowerActivation                = l_Fields[4].GetUInt32();
        m_NumFollowerActivationRegenTimestamp  = l_Fields[5].GetUInt32();
        m_CacheLastUsage                       = l_Fields[6].GetUInt32();

        l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_GARRISON_BUILDING);
        l_Stmt->setUInt32(0, m_ID);

        l_Result = CharacterDatabase.Query(l_Stmt);

        if (l_Result)
        {
            do
            {
                l_Fields = l_Result->Fetch();

                GarrisonBuilding l_Building;
                l_Building.DB_ID            = l_Fields[0].GetUInt32();
                l_Building.PlotInstanceID   = l_Fields[1].GetUInt32();
                l_Building.BuildingID       = l_Fields[2].GetUInt32();
                l_Building.SpecID           = l_Fields[3].GetUInt32();
                l_Building.TimeBuiltStart   = l_Fields[4].GetUInt32();
                l_Building.TimeBuiltEnd     = l_Fields[5].GetUInt32();
                l_Building.Active           = l_Fields[6].GetBool();

                if (!l_Building.Active && time(0) > l_Building.TimeBuiltEnd)
                    l_Building.BuiltNotified = true;    ///< Auto notify by info packet

                m_Buildings.push_back(l_Building);
            } while (l_Result->NextRow());
        }

        l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_GARRISON_MISSION);
        l_Stmt->setUInt32(0, m_ID);

        l_Result = CharacterDatabase.Query(l_Stmt);

        if (l_Result)
        {
            do
            {
                l_Fields = l_Result->Fetch();

                GarrisonMission l_Mission;
                l_Mission.DB_ID             = l_Fields[0].GetUInt32();
                l_Mission.MissionID         = l_Fields[1].GetUInt32();
                l_Mission.OfferTime         = l_Fields[2].GetUInt32();
                l_Mission.OfferMaxDuration  = l_Fields[3].GetUInt32();
                l_Mission.StartTime         = l_Fields[4].GetUInt32();
                l_Mission.State             = (GarrisonMissionState)l_Fields[5].GetUInt32();

                if ((l_Mission.OfferTime + l_Mission.OfferMaxDuration) > time(0) || l_Mission.State == GARRISON_MISSION_IN_PROGRESS || l_Mission.State == GARRISON_MISSION_COMPLETE_SUCCESS)
                    m_Missions.push_back(l_Mission);
                else
                {
                    PreparedStatement * l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GARRISON_MISSION);
                    l_Stmt->setUInt32(0, l_Mission.DB_ID);
                    CharacterDatabase.AsyncQuery(l_Stmt);
                }

            } while (l_Result->NextRow());
        }

        l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_GARRISON_FOLLOWER);
        l_Stmt->setUInt32(0, m_ID);

        l_Result = CharacterDatabase.Query(l_Stmt);

        if (l_Result)
        {
            do
            {
                l_Fields = l_Result->Fetch();

                GarrisonFollower l_Follower;
                l_Follower.DB_ID             = l_Fields[0].GetUInt32();
                l_Follower.FollowerID        = l_Fields[1].GetUInt32();
                l_Follower.Level             = l_Fields[2].GetUInt32();
                l_Follower.XP                = l_Fields[3].GetUInt32();
                l_Follower.Quality           = l_Fields[4].GetUInt32();
                l_Follower.ItemLevelArmor    = l_Fields[5].GetUInt32();
                l_Follower.ItemLevelWeapon   = l_Fields[6].GetUInt32();
                l_Follower.CurrentMissionID  = l_Fields[7].GetUInt32();
                l_Follower.CurrentBuildingID = l_Fields[8].GetUInt32();

                if (!l_Fields[9].GetString().empty())
                {
                    Tokenizer l_Abilities(l_Fields[9].GetString(), ' ');

                    for (Tokenizer::const_iterator l_It = l_Abilities.begin(); l_It != l_Abilities.end(); ++l_It)
                        l_Follower.Abilities.push_back(atol(*l_It));
                }

                l_Follower.Flags = l_Fields[10].GetUInt32();

                m_Followers.push_back(l_Follower);
            } while (l_Result->NextRow());
        }

        Init();

        return true;
    }

    return false;
}
/// Save this garrison to DB
void Garrison::Save()
{
    std::ostringstream l_KnownBluePrintsStr;

    for (uint32 l_I = 0; l_I < m_KnownBlueprints.size(); ++l_I)
        l_KnownBluePrintsStr << m_KnownBlueprints[l_I] << ' ';

    std::ostringstream l_KnownSpecializationsStr;

    for (uint32 l_I = 0; l_I < m_KnownSpecializations.size(); ++l_I)
        l_KnownSpecializationsStr << m_KnownSpecializations[l_I] << ' ';

    PreparedStatement* l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_GARRISON);

    uint32 l_Index = 0;
    l_Stmt->setUInt32(l_Index++, m_GarrisonLevel);
    l_Stmt->setString(l_Index++, l_KnownBluePrintsStr.str());
    l_Stmt->setString(l_Index++, l_KnownSpecializationsStr.str());
    l_Stmt->setUInt32(l_Index++, m_NumFollowerActivation);
    l_Stmt->setUInt32(l_Index++, m_NumFollowerActivationRegenTimestamp);
    l_Stmt->setUInt32(l_Index++, m_CacheLastUsage);
    l_Stmt->setUInt32(l_Index++, m_ID);

    CharacterDatabase.AsyncQuery(l_Stmt);

    for (uint32 l_I = 0; l_I < m_Buildings.size(); ++l_I)
    {
        PreparedStatement* l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_GARRISON_BUILDING);

        uint32 l_Index = 0;
        l_Stmt->setUInt32(l_Index++, m_Buildings[l_I].PlotInstanceID);
        l_Stmt->setUInt32(l_Index++, m_Buildings[l_I].BuildingID);
        l_Stmt->setUInt32(l_Index++, m_Buildings[l_I].SpecID);
        l_Stmt->setUInt32(l_Index++, m_Buildings[l_I].TimeBuiltStart);
        l_Stmt->setUInt32(l_Index++, m_Buildings[l_I].TimeBuiltEnd);
        l_Stmt->setBool(l_Index++, m_Buildings[l_I].Active);

        l_Stmt->setUInt32(l_Index++, m_Buildings[l_I].DB_ID);
        l_Stmt->setUInt32(l_Index++, m_ID);

        CharacterDatabase.AsyncQuery(l_Stmt);
    }

    for (uint32 l_I = 0; l_I < m_Missions.size(); ++l_I)
    {
        if ((m_Missions[l_I].OfferTime + m_Missions[l_I].OfferMaxDuration) > time(0) || m_Missions[l_I].State == GARRISON_MISSION_IN_PROGRESS || m_Missions[l_I].State == GARRISON_MISSION_COMPLETE_SUCCESS)
        {
            PreparedStatement* l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_GARRISON_MISSION);

            uint32 l_Index = 0;
            l_Stmt->setUInt32(l_Index++, m_Missions[l_I].MissionID);
            l_Stmt->setUInt32(l_Index++, m_Missions[l_I].OfferTime);
            l_Stmt->setUInt32(l_Index++, m_Missions[l_I].OfferMaxDuration);
            l_Stmt->setUInt32(l_Index++, m_Missions[l_I].StartTime);
            l_Stmt->setUInt32(l_Index++, m_Missions[l_I].State);

            l_Stmt->setUInt32(l_Index++, m_Missions[l_I].DB_ID);
            l_Stmt->setUInt32(l_Index++, m_ID);

            CharacterDatabase.AsyncQuery(l_Stmt);
        }
        else
        {
            PreparedStatement * l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GARRISON_MISSION);
            l_Stmt->setUInt32(0, m_Missions[l_I].DB_ID);
            CharacterDatabase.AsyncQuery(l_Stmt);
        }
    }

    for (uint32 l_I = 0; l_I < m_Followers.size(); ++l_I)
    {
        std::ostringstream l_Abilities;

        for (uint32 l_Y = 0; l_Y < m_Followers[l_I].Abilities.size(); ++l_Y)
            l_Abilities << m_Followers[l_I].Abilities[l_Y] << ' ';

        PreparedStatement* l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_GARRISON_FOLLOWER);

        uint32 l_Index = 0;
        l_Stmt->setUInt32(l_Index++, m_Followers[l_I].FollowerID);
        l_Stmt->setUInt32(l_Index++, m_Followers[l_I].Level);
        l_Stmt->setUInt32(l_Index++, m_Followers[l_I].XP);
        l_Stmt->setUInt32(l_Index++, m_Followers[l_I].Quality);
        l_Stmt->setUInt32(l_Index++, m_Followers[l_I].ItemLevelArmor);
        l_Stmt->setUInt32(l_Index++, m_Followers[l_I].ItemLevelWeapon);
        l_Stmt->setUInt32(l_Index++, m_Followers[l_I].CurrentMissionID);
        l_Stmt->setUInt32(l_Index++, m_Followers[l_I].CurrentBuildingID);
        l_Stmt->setString(l_Index++, l_Abilities.str());
        l_Stmt->setUInt32(l_Index++, m_Followers[l_I].Flags);

        l_Stmt->setUInt32(l_Index++, m_Followers[l_I].DB_ID);
        l_Stmt->setUInt32(l_Index++, m_ID);

        CharacterDatabase.AsyncQuery(l_Stmt);
    }
}
/// Delete garrison
void Garrison::Delete(uint64 p_PlayerGUID, SQLTransaction p_Transation)
{
    PreparedStatement* l_Stmt;

    l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GARRISON_BUILDINGS);
    l_Stmt->setUInt32(0, p_PlayerGUID);
    p_Transation->Append(l_Stmt);

    l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GARRISON_MISSIONS);
    l_Stmt->setUInt32(0, p_PlayerGUID);
    p_Transation->Append(l_Stmt);

    l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GARRISON_FOLLOWERS);
    l_Stmt->setUInt32(0, p_PlayerGUID);
    p_Transation->Append(l_Stmt);
    
    l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GARRISON);
    l_Stmt->setUInt32(0, p_PlayerGUID);
    p_Transation->Append(l_Stmt);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Update the garrison
void Garrison::Update()
{
    /// Update building in construction
    for (uint32 l_I = 0; l_I < m_Buildings.size(); ++l_I)
    {
        GarrisonBuilding * l_Building = &m_Buildings[l_I];

        if (!l_Building->Active && !l_Building->BuiltNotified && time(0) > l_Building->TimeBuiltEnd)
        {
            l_Building->BuiltNotified = true;

            /// Nothing more needed, client auto deduce notification
            UpdatePlot(l_Building->PlotInstanceID);
        }
    }

    /// Update follower activation cost
    if (m_NumFollowerActivation < GARRISON_FOLLOWER_ACTIVATION_MAX_STACK && (time(0) - m_NumFollowerActivationRegenTimestamp) > DAY)
    {
        m_NumFollowerActivation++;
        m_NumFollowerActivationRegenTimestamp = time(0);

        WorldPacket l_Data(SMSG_GARRISON_UPDATE_FOLLOWER_ACTIVATION_COUNT, 4);
        l_Data << uint32(GetNumFollowerActivationsRemaining());

        m_Owner->SendDirectMessage(&l_Data);
    }

    /// Update garrison cache
    if (m_CacheGameObjectGUID && HashMapHolder<GameObject>::Find(m_CacheGameObjectGUID) == nullptr)
        m_CacheGameObjectGUID = 0;

    uint32 l_NumRessourceGenerated = std::min((uint32)((time(0) - m_CacheLastUsage) / GARRISON_CACHE_GENERATE_TICK), (uint32)GARRISON_CACHE_MAX_CURRENCY);

    if (l_NumRessourceGenerated != m_CacheLastTokenAmount)
    {
        m_CacheLastTokenAmount = l_NumRessourceGenerated;
        m_Owner->SendUpdateWorldState(GARRISON_WORLD_STATE_CACHE_NUM_TOKEN, l_NumRessourceGenerated);

        if (l_NumRessourceGenerated == (GARRISON_CACHE_MAX_CURRENCY - 1))
        {
            GameObject * l_Cache = HashMapHolder<GameObject>::Find(m_CacheGameObjectGUID);

            if (l_Cache)
            {
                l_Cache->DestroyForNearbyPlayers();
                l_Cache->CleanupsBeforeDelete();
                delete l_Cache;
            }

            uint32 l_DisplayIDOffset = l_NumRessourceGenerated == GARRISON_CACHE_MAX_CURRENCY ? 2 : ((l_NumRessourceGenerated > (GARRISON_CACHE_MAX_CURRENCY / 2)) ? 1 : 0);
            uint32 l_DisplayID = gGarrisonCacheGameObjectID[(GetGarrisonFactionIndex() * GARRISON_FACTION_COUNT) + l_DisplayIDOffset];

            if (m_Owner->GetMapId() == GetGarrisonSiteLevelEntry()->MapID)
            {
                GarrisonCacheInfoLocation & l_Location = gGarrisonCacheInfoLocation[(GetGarrisonFactionIndex() * GARRISON_MAX_LEVEL) + (m_GarrisonLevel - 1)];
                GameObject * l_Gob = m_Owner->SummonGameObject(l_DisplayID, l_Location.X, l_Location.Y, l_Location.Z, l_Location.O, 0, 0, 0, 0, 0);

                if (l_Gob)
                    m_CacheGameObjectGUID = l_Gob->GetGUID();
            }
        }

        if (!m_CacheGameObjectGUID && l_NumRessourceGenerated >= GARRISON_CACHE_MIN_CURRENCY)
        {
            uint32 l_DisplayIDOffset = l_NumRessourceGenerated == GARRISON_CACHE_MAX_CURRENCY ? 2 : ((l_NumRessourceGenerated > (GARRISON_CACHE_MAX_CURRENCY / 2)) ? 1 : 0);
            uint32 l_DisplayID = gGarrisonCacheGameObjectID[(GetGarrisonFactionIndex() * GARRISON_FACTION_COUNT) + l_DisplayIDOffset];

            if (m_Owner->GetMapId() == GetGarrisonSiteLevelEntry()->MapID)
            {
                GarrisonCacheInfoLocation & l_Location = gGarrisonCacheInfoLocation[(GetGarrisonFactionIndex() * GARRISON_MAX_LEVEL) + (m_GarrisonLevel - 1)];
                GameObject * l_Gob = m_Owner->SummonGameObject(l_DisplayID, l_Location.X, l_Location.Y, l_Location.Z, l_Location.O, 0, 0, 0, 0, 0);

                if (l_Gob)
                    m_CacheGameObjectGUID = l_Gob->GetGUID();
            }
        }
    }

    if (m_CacheGameObjectGUID && l_NumRessourceGenerated < GARRISON_CACHE_MIN_CURRENCY)
    {
        GameObject * l_Cache = HashMapHolder<GameObject>::Find(m_CacheGameObjectGUID);

        if (l_Cache)
        {
            l_Cache->DestroyForNearbyPlayers();
            l_Cache->CleanupsBeforeDelete();
            delete l_Cache;
        }

        m_CacheGameObjectGUID = 0;
    }
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Reward garrison cache content
void Garrison::RewardGarrisonCache()
{
    m_Owner->SendDisplayToast(GARRISON_CURRENCY_ID, m_CacheLastTokenAmount, DISPLAY_TOAST_METHOD_GARRISON_CACHE, TOAST_TYPE_NEW_CURRENCY, false, false);
    m_Owner->ModifyCurrency(GARRISON_CURRENCY_ID, m_CacheLastTokenAmount);

    m_CacheLastTokenAmount  = 0;
    m_CacheLastUsage        = time(0);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// When the garrison owner enter in the garrisson (@See Player::UpdateArea)
void Garrison::OnPlayerEnter()
{
    InitPlots();    ///< AKA update plots
}
/// When the garrison owner leave the garrisson (@See Player::UpdateArea)
void Garrison::OnPlayerLeave()
{
    UninitPlots();

    if (m_CacheGameObjectGUID)
    {
        GameObject * l_Cache = HashMapHolder<GameObject>::Find(m_CacheGameObjectGUID);

        if (l_Cache)
        {
            l_Cache->DestroyForNearbyPlayers();
            l_Cache->CleanupsBeforeDelete();
            delete l_Cache;
        }

        m_CacheGameObjectGUID = 0;
    }
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// set last used activation game  object
void Garrison::SetLastUsedActivationGameObject(uint64 p_Guid)
{
    m_LastUsedActivationGameObject = p_Guid;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Get GarrSiteLevelEntry for current garrison
const GarrSiteLevelEntry * Garrison::GetGarrisonSiteLevelEntry()
{
    return sGarrSiteLevelStore.LookupEntry(m_GarrisonLevelID);
}
/// Get Garrison Faction Index
GarrisonFactionIndex Garrison::GetGarrisonFactionIndex()
{
    assert(m_Owner);

    switch (m_Owner->GetTeam())
    {
        case ALLIANCE:
            return GARRISON_FACTION_ALLIANCE;

        case HORDE:
            return GARRISON_FACTION_HORDE;
    }

    return GARRISON_FACTION_HORDE;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Get slots for level
std::vector<GarrisonPlotInstanceInfoLocation> Garrison::GetPlots()
{
    return m_Plots;
}
/// Get plot by position
GarrisonPlotInstanceInfoLocation Garrison::GetPlot(float p_X, float p_Y, float p_Z)
{
    Position                            l_Position;
    GarrisonPlotInstanceInfoLocation    l_Plot;

    memset(&l_Plot, 0, sizeof(GarrisonPlotInstanceInfoLocation));

    l_Position.m_positionX = p_X;
    l_Position.m_positionY = p_Y;
    l_Position.m_positionZ = p_Z;

    for (uint32 l_I = 0; l_I < m_Plots.size(); ++l_I)
    {
        if (l_Position.GetExactDist2d(m_Plots[l_I].X, m_Plots[l_I].Y) < l_Position.GetExactDist2d(l_Plot.X, l_Plot.Y)
            && l_Position.GetExactDist2d(m_Plots[l_I].X, m_Plots[l_I].Y) < 35)
            l_Plot = m_Plots[l_I];
    }

    return l_Plot;
}
/// Get plot instance plot type
uint32 Garrison::GetPlotType(uint32 p_PlotInstanceID)
{
    const GarrPlotInstanceEntry * l_PlotInstanceEntry = sGarrPlotInstanceStore.LookupEntry(p_PlotInstanceID);

    if (!l_PlotInstanceEntry)
        return 0;

    const GarrPlotEntry * l_PlotEntry = sGarrPlotStore.LookupEntry(l_PlotInstanceEntry->PlotID);

    if (!l_PlotEntry)
        return 0;

    return l_PlotEntry->PlotType;
}
/// Plot is free ?
bool Garrison::PlotIsFree(uint32 p_PlotInstanceID)
{
    for (uint32 l_I = 0; l_I < m_Buildings.size(); ++l_I)
        if (m_Buildings[l_I].PlotInstanceID == p_PlotInstanceID)
            return false;

    return true;
}
/// Has plot instance
bool Garrison::HasPlotInstance(uint32 p_PlotInstanceID)
{
    for (uint32 l_I = 0; l_I < m_Plots.size(); ++l_I)
        if (m_Plots[l_I].PlotInstanceID == p_PlotInstanceID)
            return true;

    return false;
}
/// Get plot location
GarrisonPlotInstanceInfoLocation Garrison::GetPlot(uint32 p_PlotInstanceID)
{
    for (uint32 l_I = 0; l_I < m_Plots.size(); ++l_I)
    {
        if (m_Plots[l_I].SiteLevelID == m_GarrisonLevelID && m_Plots[l_I].PlotInstanceID == p_PlotInstanceID)
            return m_Plots[l_I];
    }

    return GarrisonPlotInstanceInfoLocation();
}
/// Get plot instance ID by activation game object
uint32 Garrison::GetPlotInstanceIDByActivationGameObject(uint64 p_Guid)
{
    for (std::map<uint32, uint64>::iterator l_It = m_PlotsActivateGob.begin(); l_It != m_PlotsActivateGob.end(); ++l_It)
        if (l_It->second == p_Guid)
            return l_It->first;

    return 0;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Add mission
bool Garrison::AddMission(uint32 p_MissionRecID)
{
    const GarrMissionEntry * l_MissionEntry = sGarrMissionStore.LookupEntry(p_MissionRecID);

    if (!l_MissionEntry)
        return false;

    if (HaveMission(p_MissionRecID))
        return false;

    if (l_MissionEntry->RequiredLevel > m_Owner->getLevel())
        return false;

    if (l_MissionEntry->RequiredItemLevel > m_Owner->GetAverageItemLevelEquipped())
        return false;

    GarrisonMission l_Mission;

    l_Mission.MissionID         = p_MissionRecID;
    l_Mission.OfferTime         = time(0);
    l_Mission.OfferMaxDuration  = l_MissionEntry->OfferTime;
    l_Mission.State             = GARRISON_MISSION_AVAILABLE;
    l_Mission.StartTime         = 0;

    PreparedStatement* l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_GARRISON_MISSION);

    uint32 l_Index = 0;
    l_Stmt->setUInt32(l_Index++, m_ID);
    l_Stmt->setUInt32(l_Index++, p_MissionRecID);

    CharacterDatabase.Query(l_Stmt);

    l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_GARRISON_MISSION_DB_ID);

    l_Index = 0;
    l_Stmt->setUInt32(l_Index++, m_ID);
    l_Stmt->setUInt32(l_Index++, p_MissionRecID);

    PreparedQueryResult l_Result = CharacterDatabase.Query(l_Stmt);

    if (!l_Result)
        assert(false && "Garrison::AddMission() failed to retrieve created garrison mission ID");

    Field * l_Fields = l_Result->Fetch();

    l_Mission.DB_ID = l_Fields[0].GetUInt32();

    m_Missions.push_back(l_Mission);

    WorldPacket l_AddMissionResult(SMSG_GARRISON_ADD_MISSION_RESULT, 40);
    l_AddMissionResult << uint32(GARRISON_PURCHASE_BUILDING_OK);
    l_AddMissionResult << uint64(l_Mission.DB_ID);
    l_AddMissionResult << uint32(l_Mission.MissionID);
    l_AddMissionResult << uint32(l_Mission.OfferTime);
    l_AddMissionResult << uint32(l_Mission.OfferMaxDuration);
    l_AddMissionResult << uint32(l_Mission.StartTime);
    l_AddMissionResult << uint32(0);   ///< Travel duration
    l_AddMissionResult << uint32(0);   ///< Mission duration
    l_AddMissionResult << uint32(l_Mission.State);

    m_Owner->SendDirectMessage(&l_AddMissionResult);

    return true;
}
/// Player have mission
bool Garrison::HaveMission(uint32 p_MissionRecID)
{
    for (uint32 l_I = 0; l_I < m_Missions.size(); ++l_I)
    {
        if (m_Missions[l_I].MissionID == p_MissionRecID)
        {
            if (m_Missions[l_I].State < GARRISON_MISSION_COMPLETE_SUCCESS
                && !((m_Missions[l_I].OfferTime + m_Missions[l_I].OfferMaxDuration) < time(0) && m_Missions[l_I].State == GARRISON_MISSION_AVAILABLE))
                return true;
        }
    }

    return false;
}
/// Start mission
void Garrison::StartMission(uint32 p_MissionRecID, std::vector<uint64> p_Followers)
{
    if (!HaveMission(p_MissionRecID))
    {
        StartMissionFailed();
        return;
    }

    if (GetActivatedFollowerCount() > m_Stat_MaxActiveFollower)
        return;

    const GarrMissionEntry * l_MissionTemplate = sGarrMissionStore.LookupEntry(p_MissionRecID);

    if (!m_Owner->HasCurrency(GARRISON_CURRENCY_ID, l_MissionTemplate->GarrisonCurrencyStartCost))
    {
        StartMissionFailed();
        return;
    }

    if (p_Followers.size() < l_MissionTemplate->RequiredFollowersCount)
    {
        StartMissionFailed();
        return;
    }

    for (uint32 l_I = 0; l_I < p_Followers.size(); ++l_I)
    {
        std::vector<GarrisonFollower>::iterator l_It = std::find_if(m_Followers.begin(), m_Followers.end(), [this, p_Followers, l_I](const GarrisonFollower p_Follower) -> bool
        {
            if (p_Follower.DB_ID == p_Followers[l_I])
                return true;

            return false;
        });

        if (l_It == m_Followers.end())
        {
            StartMissionFailed();
            return;
        }

        if (l_It->CurrentBuildingID != 0 || l_It->CurrentMissionID != 0)
        {
            StartMissionFailed();
            return;
        }

        /// Should not happen
        if (l_It->Flags & GARRISON_FOLLOWER_FLAG_INACTIVE)
            return;

        uint32 l_FollowerItemLevel = (l_It->ItemLevelWeapon + l_It->ItemLevelArmor) / 2;

        if (l_FollowerItemLevel < l_MissionTemplate->RequiredItemLevel)
        {
            StartMissionFailed();
            return;
        }
    }

    m_Owner->ModifyCurrency(GARRISON_CURRENCY_ID, -(int32)l_MissionTemplate->GarrisonCurrencyStartCost);

    for (uint32 l_I = 0; l_I < p_Followers.size(); ++l_I)
    {
        std::vector<GarrisonFollower>::iterator l_It = std::find_if(m_Followers.begin(), m_Followers.end(), [this, p_Followers, l_I](const GarrisonFollower p_Follower) -> bool
        {
            if (p_Follower.DB_ID == p_Followers[l_I])
                return true;

            return false;
        });

        l_It->CurrentMissionID = p_MissionRecID;
    }

    GarrisonMission * l_Mission;

    for (uint32 l_I = 0; l_I < m_Missions.size(); ++l_I)
    {
        if (m_Missions[l_I].MissionID == p_MissionRecID)
        {
            l_Mission = &m_Missions[l_I];
            break;
        }
    }

    if (l_Mission)
    {
        l_Mission->State        = GARRISON_MISSION_IN_PROGRESS;
        l_Mission->StartTime    = time(0);
        l_Mission->OfferTime    = time(0);

        uint32 l_TravelDuration  = GetMissionTravelDuration(p_MissionRecID);
        uint32 l_MissionDuration = GetMissionDuration(p_MissionRecID);

        WorldPacket l_Result(SMSG_GARRISON_START_MISSION_RESULT, 200);
        l_Result << uint32(0);    ///< Result (0 = OK, 1 = failed)

        l_Result << uint64(l_Mission->DB_ID);
        l_Result << uint32(l_Mission->MissionID);
        l_Result << uint32(l_Mission->OfferTime);
        l_Result << uint32(l_Mission->OfferMaxDuration);
        l_Result << uint32(l_Mission->StartTime);
        l_Result << uint32(l_TravelDuration);
        l_Result << uint32(l_MissionDuration);
        l_Result << uint32(l_Mission->State);

        l_Result << uint32(p_Followers.size());

        for (uint32 l_I = 0; l_I < p_Followers.size(); ++l_I)
            l_Result << uint64(p_Followers[l_I]);

        m_Owner->SendDirectMessage(&l_Result);
    }
}
/// Send mission start failed packet
void Garrison::StartMissionFailed()
{
    WorldPacket l_Data(SMSG_GARRISON_START_MISSION_RESULT, 200);
    l_Data << uint32(1);    ///< Result (0 = OK, 1 = failed)

    /// Mission details
    l_Data << uint64(0);
    l_Data << uint32(0);
    l_Data << uint32(0);
    l_Data << uint32(0);
    l_Data << uint32(0);
    l_Data << uint32(0);
    l_Data << uint32(0);
    l_Data << uint32(0);

    l_Data << uint32(0);    ///< Follower count

    m_Owner->SendDirectMessage(&l_Data);
}
/// Complete a mission
void Garrison::CompleteMission(uint32 p_MissionRecID)
{
    if (!HaveMission(p_MissionRecID))
        return;

    const GarrMissionEntry * l_MissionTemplate = sGarrMissionStore.LookupEntry(p_MissionRecID);

    GarrisonMission * l_Mission;

    for (uint32 l_I = 0; l_I < m_Missions.size(); ++l_I)
    {
        if (m_Missions[l_I].MissionID == p_MissionRecID)
        {
            l_Mission = &m_Missions[l_I];
            break;
        }
    }

    uint32 l_TravelDuration  = GetMissionTravelDuration(p_MissionRecID);
    uint32 l_MissionDuration = GetMissionDuration(p_MissionRecID);

    std::vector<GarrisonFollower*> l_MissionFollowers = GetMissionFollowers(p_MissionRecID);

    /// Hack attempt
    if ((l_Mission->StartTime + (l_TravelDuration + l_MissionDuration)) > time(0))
        return;

    uint32 l_ChestChance = GetMissionChestChance(p_MissionRecID);

    bool l_CanComplete = true;
    bool l_Succeeded   = roll_chance_i(l_ChestChance);  ///< Seems to be MissionChance == ChestChance

    l_Mission->State = l_Succeeded ? GARRISON_MISSION_COMPLETE_SUCCESS : GARRISON_MISSION_COMPLETE_FAILED;

    WorldPacket l_Result(SMSG_GARRISON_COMPLETE_MISSION_RESULT, 100);

    l_Result << uint32(!l_CanComplete);

    l_Result << uint64(l_Mission->DB_ID);
    l_Result << uint32(l_Mission->MissionID);
    l_Result << uint32(l_Mission->OfferTime);
    l_Result << uint32(l_Mission->OfferMaxDuration);
    l_Result << uint32(l_Mission->StartTime);
    l_Result << uint32(l_TravelDuration);
    l_Result << uint32(l_MissionDuration);
    l_Result << uint32(l_Mission->State);

    l_Result << uint32(l_Mission->MissionID);
    l_Result.WriteBit(l_Succeeded);
    l_Result.FlushBits();

    m_Owner->SendDirectMessage(&l_Result);

    if (l_Succeeded)
    {
        std::vector<uint32> l_PartyXPModifiersEffect = GetMissionFollowersAbilitiesEffects(p_MissionRecID, GARRISION_ABILITY_EFFECT_MOD_XP_GAIN, GARRISON_ABILITY_EFFECT_TARGET_MASK_UNK | GARRISON_ABILITY_EFFECT_TARGET_MASK_PARTY);
        std::vector<uint32> l_PassiveEffects         = GetBuildingsPassiveAbilityEffects();

        float l_XPModifier = 1.0f;
        for (uint32 l_I = 0; l_I < l_PartyXPModifiersEffect.size(); ++l_I)
        {
            const GarrAbilityEffectEntry * l_AbilityEffectEntry = sGarrAbilityEffectStore.LookupEntry(l_PartyXPModifiersEffect[l_I]);

            if (!l_AbilityEffectEntry)
                continue;

            l_XPModifier = (l_AbilityEffectEntry->Amount - 1.0) + l_XPModifier;
        }

        for (uint32 l_Y = 0; l_Y < l_PassiveEffects.size(); ++l_Y)
        {
            const GarrAbilityEffectEntry * l_AbilityEffectEntry = sGarrAbilityEffectStore.LookupEntry(l_PassiveEffects[l_Y]);

            if (!l_AbilityEffectEntry)
                continue;

            if (l_AbilityEffectEntry->EffectType == GARRISION_ABILITY_EFFECT_MOD_XP_GAIN && (l_AbilityEffectEntry->TargetMask == GARRISON_ABILITY_EFFECT_TARGET_MASK_PARTY || l_AbilityEffectEntry->TargetMask == GARRISON_ABILITY_EFFECT_TARGET_MASK_UNK))
                l_XPModifier = (l_AbilityEffectEntry->Amount - 1.0) + l_XPModifier;
        }

        float l_BonusXP = (l_XPModifier - 1.0f) * l_MissionTemplate->RewardFollowerExperience;

        for (uint32 l_FollowerIt = 0; l_FollowerIt < l_MissionFollowers.size(); ++l_FollowerIt)
        {
            float l_SecondXPModifier = 1.0f;

            for (uint32 l_AbilityIt = 0; l_AbilityIt < l_MissionFollowers[l_FollowerIt]->Abilities.size(); l_AbilityIt++)
            {
                uint32 l_CurrentAbilityID = l_MissionFollowers[l_FollowerIt]->Abilities[l_AbilityIt];

                for (uint32 l_EffectIt = 0; l_EffectIt < sGarrAbilityEffectStore.GetNumRows(); l_EffectIt++)
                {
                    const GarrAbilityEffectEntry * l_AbilityEffectEntry = sGarrAbilityEffectStore.LookupEntry(l_EffectIt);

                    if (!l_AbilityEffectEntry || l_AbilityEffectEntry->AbilityID != l_CurrentAbilityID)
                        continue;

                    if (l_AbilityEffectEntry->EffectType == GARRISION_ABILITY_EFFECT_MOD_XP_GAIN && l_AbilityEffectEntry->TargetMask == GARRISON_ABILITY_EFFECT_TARGET_MASK_SELF)
                        l_SecondXPModifier = (l_AbilityEffectEntry->Amount - 1.0) + l_SecondXPModifier;
                }
            }

            l_MissionFollowers[l_FollowerIt]->XP += (l_BonusXP + l_MissionTemplate->RewardFollowerExperience) * l_BonusXP;

            if (l_MissionFollowers[l_FollowerIt]->Level < GARRISON_MAX_FOLLOWER_LEVEL)
            {
                const GarrFollowerLevelXPEntry * l_LevelData = nullptr;

                for (uint32 l_I = 0; l_I < sGarrFollowerLevelXPStore.GetNumRows(); ++l_I)
                {
                    const GarrFollowerLevelXPEntry * l_CurrentLevelData = sGarrFollowerLevelXPStore.LookupEntry(l_I);

                    if (l_CurrentLevelData && l_CurrentLevelData->Level == l_MissionFollowers[l_FollowerIt]->Level)
                    {
                        l_LevelData = l_CurrentLevelData;
                        break;
                    }
                }

                if (l_LevelData && l_MissionFollowers[l_FollowerIt]->XP >= l_LevelData->RequiredExperience)
                {
                    l_MissionFollowers[l_FollowerIt]->Level++;
                    l_MissionFollowers[l_FollowerIt]->XP = l_MissionFollowers[l_FollowerIt]->XP - l_LevelData->RequiredExperience;
                }
            }
            else
                l_MissionFollowers[l_FollowerIt]->XP = 0;
        }
    }
}
/// Get followers on a mission
std::vector<GarrisonFollower*> Garrison::GetMissionFollowers(uint32 p_MissionRecID)
{
    std::vector<GarrisonFollower*> l_MissionFollowers;

    for (uint32 l_I = 0; l_I < m_Followers.size(); ++l_I)
    {
        if (m_Followers[l_I].CurrentMissionID == p_MissionRecID)
            l_MissionFollowers.push_back(&m_Followers[l_I]);
    }

    return l_MissionFollowers;
}
/// Get mission followers abilities effect
std::vector<uint32> Garrison::GetMissionFollowersAbilitiesEffects(uint32 p_MissionRecID)
{
    std::vector<GarrisonFollower*> l_MissionFollowers = GetMissionFollowers(p_MissionRecID);
    std::vector<uint32>            l_AbilitiesEffects;

    for (uint32 l_FollowerIt = 0; l_FollowerIt < l_MissionFollowers.size(); ++l_FollowerIt)
    {
        for (uint32 l_AbilityIt = 0; l_AbilityIt < l_MissionFollowers[l_FollowerIt]->Abilities.size(); l_AbilityIt++)
        {
            uint32 l_CurrentAbilityID = l_MissionFollowers[l_FollowerIt]->Abilities[l_AbilityIt];

            for (uint32 l_EffectIt = 0; l_EffectIt < sGarrAbilityEffectStore.GetNumRows(); l_EffectIt++)
            {
                const GarrAbilityEffectEntry * l_AbilityEffectEntry = sGarrAbilityEffectStore.LookupEntry(l_EffectIt);

                if (!l_AbilityEffectEntry || l_AbilityEffectEntry->AbilityID != l_CurrentAbilityID)
                    continue;

                l_AbilitiesEffects.push_back(l_AbilityEffectEntry->EffectID);
            }
        }
    }

    return l_AbilitiesEffects;
}
/// Get mission followers abilities effect
std::vector<uint32> Garrison::GetMissionFollowersAbilitiesEffects(uint32 p_MissionRecID, GarrisonAbilityEffectType p_Type, uint32 p_TargetMask)
{
    std::vector<GarrisonFollower*> l_MissionFollowers = GetMissionFollowers(p_MissionRecID);
    std::vector<uint32>            l_AbilitiesEffects;

    for (uint32 l_FollowerIt = 0; l_FollowerIt < l_MissionFollowers.size(); ++l_FollowerIt)
    {
        for (uint32 l_AbilityIt = 0; l_AbilityIt < l_MissionFollowers[l_FollowerIt]->Abilities.size(); l_AbilityIt++)
        {
            uint32 l_CurrentAbilityID = l_MissionFollowers[l_FollowerIt]->Abilities[l_AbilityIt];

            for (uint32 l_EffectIt = 0; l_EffectIt < sGarrAbilityEffectStore.GetNumRows(); l_EffectIt++)
            {
                const GarrAbilityEffectEntry * l_AbilityEffectEntry = sGarrAbilityEffectStore.LookupEntry(l_EffectIt);

                if (!l_AbilityEffectEntry || l_AbilityEffectEntry->AbilityID != l_CurrentAbilityID)
                    continue;

                if (l_AbilityEffectEntry->EffectType != p_Type || (l_AbilityEffectEntry->TargetMask != 0 && (l_AbilityEffectEntry->TargetMask & p_TargetMask) == 0))
                    continue;

                l_AbilitiesEffects.push_back(l_AbilityEffectEntry->EffectID);
            }
        }
    }

    return l_AbilitiesEffects;
}
/// Get the mission travel time
uint32 Garrison::GetMissionTravelDuration(uint32 p_MissionRecID)
{
    const GarrMissionEntry    * l_MissionTemplate   = sGarrMissionStore.LookupEntry(p_MissionRecID);
    std::vector<uint32>         l_AbilitiesEffects  = GetMissionFollowersAbilitiesEffects(p_MissionRecID);
    std::vector<uint32>         l_PassiveEffects    = GetBuildingsPassiveAbilityEffects();
    float                       l_MissionTravelTime = l_MissionTemplate->TravelTime;

    for (uint32 l_EffectI = 0; l_EffectI < l_AbilitiesEffects.size(); l_EffectI++)
    {
        const GarrAbilityEffectEntry * l_AbilityEffectEntry = sGarrAbilityEffectStore.LookupEntry(l_AbilitiesEffects[l_EffectI]);

        if (!l_AbilityEffectEntry)
            continue;

        if (l_AbilityEffectEntry->EffectType == GARRISION_ABILITY_EFFECT_MOD_TRAVEL_TIME)
            l_MissionTravelTime = l_MissionTravelTime * l_AbilityEffectEntry->Amount;
    }

    for (uint32 l_Y = 0; l_Y < l_PassiveEffects.size(); ++l_Y)
    {
        const GarrAbilityEffectEntry * l_AbilityEffectEntry = sGarrAbilityEffectStore.LookupEntry(l_PassiveEffects[l_Y]);

        if (!l_AbilityEffectEntry)
            continue;

        if (l_AbilityEffectEntry->EffectType == GARRISION_ABILITY_EFFECT_MOD_TRAVEL_TIME)
            l_MissionTravelTime = l_MissionTravelTime * l_AbilityEffectEntry->Amount;
    }

    return floorf(l_MissionTravelTime);
}
/// Get the mission duration
uint32 Garrison::GetMissionDuration(uint32 p_MissionRecID)
{
    const GarrMissionEntry    * l_MissionTemplate   = sGarrMissionStore.LookupEntry(p_MissionRecID);
    std::vector<uint32>         l_AbilitiesEffects  = GetMissionFollowersAbilitiesEffects(p_MissionRecID);
    float                       l_MissionDuration   = l_MissionTemplate->Duration;

    for (uint32 l_EffectI = 0; l_EffectI < l_AbilitiesEffects.size(); l_EffectI++)
    {
        const GarrAbilityEffectEntry * l_AbilityEffectEntry = sGarrAbilityEffectStore.LookupEntry(l_AbilitiesEffects[l_EffectI]);

        if (!l_AbilityEffectEntry)
            continue;

        if (l_AbilityEffectEntry->EffectType == GARRISION_ABILITY_EFFECT_MOD_MISSION_DURATION)
            l_MissionDuration = l_MissionDuration * l_AbilityEffectEntry->Amount;
    }

    return floorf(l_MissionDuration);
}
/// Get mission chest chance
uint32 Garrison::GetMissionChestChance(uint32 p_MissionRecID)
{
    const GarrMissionEntry * l_MissionTemplate = sGarrMissionStore.LookupEntry(p_MissionRecID);

    std::vector<uint32>                     l_Encounters;
    std::vector<std::pair<uint32, uint32>>  l_EncoutersMechanics;
    std::vector<GarrisonFollower*>          l_MissionFollowers = GetMissionFollowers(p_MissionRecID);
    std::vector<uint32>                     l_PassiveEffects = GetBuildingsPassiveAbilityEffects();
    std::map<uint64, uint32>                l_FollowersBiasMap;
    std::vector<uint32>                     l_CounterAbilityUsed; 

    for (uint32 l_I = 0; l_I < sGarrMissionXEncouterStore.GetNumRows(); ++l_I)
    {
        const GarrMissionXEncouterEntry * l_Encounter = sGarrMissionXEncouterStore.LookupEntry(l_I);

        if (l_Encounter && l_Encounter->MissionID == p_MissionRecID)
            l_Encounters.push_back(l_Encounter->EncounterID);
    }

    for (uint32 l_I = 0; l_I < l_Encounters.size(); l_I++)
    {
        for (uint32 l_Y = 0; l_Y < sGarrEncouterXMechanicStore.GetNumRows(); ++l_Y)
        {
            const GarrEncouterXMechanicEntry * l_EncounterMechanic = sGarrEncouterXMechanicStore.LookupEntry(l_Y);

            if (l_EncounterMechanic && l_EncounterMechanic->EncounterID == l_Encounters[l_I])
                l_EncoutersMechanics.push_back(std::make_pair(l_Encounters[l_I], l_EncounterMechanic->MechanicID));
        }
    }

    float l_Float8 = 100.f;
    float l_FloatC = 150.f;

    float l_V8  = l_MissionTemplate->RequiredFollowersCount * l_Float8;
    float l_V60 = l_MissionTemplate->RequiredFollowersCount * l_Float8;

    for (uint32 l_I = 0; l_I < l_EncoutersMechanics.size(); ++l_I)
    {
        const GarrMechanicEntry     * l_MechanicEntry       = sGarrMechanicStore.LookupEntry(l_EncoutersMechanics[l_I].second);
        const GarrMechanicTypeEntry * l_MechanicTypeEntry   = sGarrMechanicTypeStore.LookupEntry(l_MechanicEntry->MechanicTypeID);

        if (l_MechanicTypeEntry && l_MechanicTypeEntry->Unk1 != 2)
        {
            l_V8 = l_V60;
        }
        else
        {
            l_V8  = l_MechanicEntry->Unk2 + l_V60;
            l_V60 = l_MechanicEntry->Unk2 + l_V60;
        }
    }

    float l_CurrentAdditionalWinChance = 0;

    float v11 = 100.0f / l_V8;
    float v62 = 100.0f / l_V8;

    /// OK 100%
    #pragma region Followers Bias
    for (uint32 l_Y = 0; l_Y < l_MissionFollowers.size(); ++l_Y)
    {
        float l_FollowerBias = (l_MissionFollowers[l_Y]->Level - l_MissionTemplate->RequiredLevel) * 0.33333334;

        if (l_MissionTemplate->RequiredLevel == GARRISON_MAX_FOLLOWER_LEVEL)
        {
            if (l_MissionTemplate->RequiredItemLevel > 0)
            {
                uint32 l_TotalFollowerItemLevel = l_MissionFollowers[l_Y]->ItemLevelArmor + l_MissionFollowers[l_Y]->ItemLevelWeapon;
                l_FollowerBias = (((l_TotalFollowerItemLevel >> 1) - l_MissionTemplate->RequiredItemLevel) * 0.06666667) + l_FollowerBias;
            }
        }

        if (l_FollowerBias < -1.0f)
            l_FollowerBias = -1.0f;
        else if (l_FollowerBias > 1.0f)
            l_FollowerBias = 1.0f;

        l_FollowersBiasMap[l_MissionFollowers[l_Y]->DB_ID] = l_FollowerBias;

        float l_Seil = 0.f;

        if (l_FollowerBias >= 0.0)
            l_Seil = (l_FloatC - l_Float8) * l_FollowerBias + l_Float8;
        else
            l_Seil = (l_FollowerBias + 1.0) * l_Float8;

        l_CurrentAdditionalWinChance = (l_Seil * v11) + l_CurrentAdditionalWinChance;

        #ifdef GARRISON_CHEST_FORMULA_DEBUG
            printf("Added %.2f to success due to follower %d bias.\n", (l_Seil * v11), l_MissionFollowers[l_Y]->FollowerID);
        #endif // GARRISON_CHEST_FORMULA_DEBUG
    }
    #pragma endregion

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// OK 100%
    #pragma region Counter mechanic
    for (uint32 l_I = 0; l_I < l_EncoutersMechanics.size(); ++l_I)
    {
        const GarrMechanicEntry     * l_MechanicEntry       = sGarrMechanicStore.LookupEntry(l_EncoutersMechanics[l_I].second);
        const GarrMechanicTypeEntry * l_MechanicTypeEntry   = sGarrMechanicTypeStore.LookupEntry(l_MechanicEntry->MechanicTypeID);

        if (l_MechanicTypeEntry->Unk1 == 2)
        {
            float l_Unk1 = l_MechanicEntry->Unk2;
            float l_Unk2 = l_MechanicEntry->Unk2;

            if (l_MissionFollowers.size() > 0)
            {
                for (uint32 l_Y = 0; l_Y < l_MissionFollowers.size(); ++l_Y)
                {
                    for (uint32 l_Z = 0; l_Z < l_MissionFollowers[l_Y]->Abilities.size(); l_Z++)
                    {
                        uint32 l_CurrentAbilityID = l_MissionFollowers[l_Y]->Abilities[l_Z];

                        for (uint32 l_EffectI = 0; l_EffectI < sGarrAbilityEffectStore.GetNumRows(); l_EffectI++)
                        {
                            const GarrAbilityEffectEntry * l_AbilityEffectEntry = sGarrAbilityEffectStore.LookupEntry(l_EffectI);

                            if (l_AbilityEffectEntry && l_AbilityEffectEntry->CounterMechanicTypeID == l_EncoutersMechanics[l_I].second
                                && l_AbilityEffectEntry->AbilityID == l_CurrentAbilityID
                                && !(l_AbilityEffectEntry->Unk3 & 1))
                            {
                                l_Unk1 = l_Unk2;
                                if (l_Unk2 != 0.0)
                                {
                                    float l_Seil = 0;

                                    if (l_FollowersBiasMap[l_MissionFollowers[l_Y]->DB_ID] >= 0.0)
                                        l_Seil = (l_AbilityEffectEntry->ModMax - l_AbilityEffectEntry->ModMin) * l_FollowersBiasMap[l_MissionFollowers[l_Y]->DB_ID] + l_AbilityEffectEntry->ModMin;
                                    else
                                        l_Seil = (l_FollowersBiasMap[l_MissionFollowers[l_Y]->DB_ID] + 1.0) * l_AbilityEffectEntry->ModMin;

                                    l_Unk1 = l_MechanicEntry->Unk2;

                                    if (l_Seil <= l_Unk1)
                                        l_Unk1 = l_Seil;

                                    l_Unk2 = l_Unk2 - l_Unk1;
                                }
                            }
                        }
                    }
                }
            }

            if (l_Unk2 < 0.0f)
                l_Unk2 = 0.0f;

            l_Unk1 = l_MechanicEntry->Unk2;
            l_Unk1 = (l_Unk1 - l_Unk2) * v62;
            l_CurrentAdditionalWinChance = l_Unk1 + l_CurrentAdditionalWinChance;

            #ifdef GARRISON_CHEST_FORMULA_DEBUG
                printf("Added %.2f to success due to followers countering boss mechanic %d.\n", l_Unk1, l_EncoutersMechanics[l_I].second);
            #endif // GARRISON_CHEST_FORMULA_DEBUG
        }
    }
    #pragma endregion

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// UNTESTED
    #pragma region Race Ability Counter
    for (uint32 l_I = 0; l_I < l_EncoutersMechanics.size(); ++l_I)
    {
        const GarrMechanicEntry     * l_MechanicEntry       = sGarrMechanicStore.LookupEntry(l_EncoutersMechanics[l_I].second);
        const GarrMechanicTypeEntry * l_MechanicTypeEntry   = sGarrMechanicTypeStore.LookupEntry(l_MechanicEntry->MechanicTypeID);

        if (l_MechanicTypeEntry->Unk1 == 1)
        {
            for (uint32 l_Y = 0; l_Y < l_MissionFollowers.size(); ++l_Y)
            {
                for (uint32 l_Z = 0; l_Z < l_MissionFollowers[l_Y]->Abilities.size(); l_Z++)
                {
                    uint32 l_CurrentAbilityID = l_MissionFollowers[l_Y]->Abilities[l_Z];

                    for (uint32 l_EffectI = 0; l_EffectI < sGarrAbilityEffectStore.GetNumRows(); l_EffectI++)
                    {
                        const GarrAbilityEffectEntry * l_AbilityEffectEntry = sGarrAbilityEffectStore.LookupEntry(l_EffectI);

                        if (!l_AbilityEffectEntry || l_AbilityEffectEntry->AbilityID != l_CurrentAbilityID)
                            continue;

                        if (l_AbilityEffectEntry->CounterMechanicTypeID == l_MissionTemplate->GarrMechanicTypeRecID)
                        {
                            float l_Seil = 0.f;

                            if (l_FollowersBiasMap[l_MissionFollowers[l_Y]->DB_ID] >= 0.0)
                                l_Seil = (l_AbilityEffectEntry->ModMax - l_AbilityEffectEntry->ModMin) * l_FollowersBiasMap[l_MissionFollowers[l_Y]->DB_ID] + l_AbilityEffectEntry->ModMin;
                            else
                                l_Seil = (l_FollowersBiasMap[l_MissionFollowers[l_Y]->DB_ID] + 1.0) * l_AbilityEffectEntry->ModMin;

                            l_CurrentAdditionalWinChance = (l_Seil * v62) + l_CurrentAdditionalWinChance;

                            #ifdef GARRISON_CHEST_FORMULA_DEBUG
                                printf("Added %.2f to success due to follower %d enemy race ability %d.\n", (l_Seil * v62), 0, l_CurrentAbilityID);
                            #endif // GARRISON_CHEST_FORMULA_DEBUG
                        }
                    }
                }
            }
        }
    }
    #pragma endregion 

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// OK 100%
    #pragma region Environment Ability
    for (uint32 l_Y = 0; l_Y < l_MissionFollowers.size(); ++l_Y)
    {
        for (uint32 l_Z = 0; l_Z < l_MissionFollowers[l_Y]->Abilities.size(); l_Z++)
        {
            uint32 l_CurrentAbilityID = l_MissionFollowers[l_Y]->Abilities[l_Z];

            for (uint32 l_EffectI = 0; l_EffectI < sGarrAbilityEffectStore.GetNumRows(); l_EffectI++)
            {
                const GarrAbilityEffectEntry * l_AbilityEffectEntry = sGarrAbilityEffectStore.LookupEntry(l_EffectI);

                if (!l_AbilityEffectEntry || l_AbilityEffectEntry->AbilityID != l_CurrentAbilityID)
                    continue;

                if (l_AbilityEffectEntry->CounterMechanicTypeID == l_MissionTemplate->GarrMechanicTypeRecID)
                {
                    float l_Seil = 0.f;

                    if (l_FollowersBiasMap[l_MissionFollowers[l_Y]->DB_ID] >= 0.0)
                        l_Seil = (l_AbilityEffectEntry->ModMax - l_AbilityEffectEntry->ModMin) * l_FollowersBiasMap[l_MissionFollowers[l_Y]->DB_ID] + l_AbilityEffectEntry->ModMin;
                    else
                        l_Seil = (l_FollowersBiasMap[l_MissionFollowers[l_Y]->DB_ID] + 1.0) * l_AbilityEffectEntry->ModMin;

                    l_CurrentAdditionalWinChance = (l_Seil * v62) + l_CurrentAdditionalWinChance;

                    #ifdef GARRISON_CHEST_FORMULA_DEBUG
                        printf("Added %.2f to success due to follower %d environment ability %d.\n", (l_Seil * v62), l_MissionFollowers[l_Y]->FollowerID, l_CurrentAbilityID);
                    #endif // GARRISON_CHEST_FORMULA_DEBUG
                }
            }
        }
    }
    #pragma endregion 

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// OK 100%
    #pragma region Follower Trait
    float l_MissionDuration     = GetMissionDuration(p_MissionRecID);
    float l_MissionTravelTime   = GetMissionTravelDuration(p_MissionRecID);

    for (uint32 l_Y = 0; l_Y < l_MissionFollowers.size(); ++l_Y)
    {
        for (uint32 l_Z = 0; l_Z < l_MissionFollowers[l_Y]->Abilities.size(); l_Z++)
        {
            uint32 l_CurrentAbilityID = l_MissionFollowers[l_Y]->Abilities[l_Z];

            for (uint32 l_EffectI = 0; l_EffectI < sGarrAbilityEffectStore.GetNumRows(); l_EffectI++)
            {
                const GarrAbilityEffectEntry * l_AbilityEffectEntry = sGarrAbilityEffectStore.LookupEntry(l_EffectI);

                if (!l_AbilityEffectEntry || l_AbilityEffectEntry->AbilityID != l_CurrentAbilityID)
                    continue;

                bool l_Proc = true;

                switch (l_AbilityEffectEntry->EffectType)
                {
                    /// Proc if MissionFollowerCount == 1
                    case GARRISION_ABILITY_EFFECT_MOD_WIN_RATE_SOLO:
                        l_Proc = l_MissionFollowers.size() == 1;
                        break;

                    /// Proc every time, no condition
                    case GARRISION_ABILITY_EFFECT_MOD_WIN_RATE:
                        l_Proc = true;
                        break;

                    /// Proc if Find(MissionFollowers[Class], MiscValueA) != NULL
                    case GARRISION_ABILITY_EFFECT_MOD_WIN_RATE_CLASS:
                        for (uint32 l_W = 0; l_W < l_MissionFollowers.size(); ++l_W)
                        {
                            if (l_W != l_Y)
                            {
                                const GarrFollowerEntry * l_FollowerTemplate = sGarrFollowerStore.LookupEntry(l_MissionFollowers[l_W]->FollowerID);

                                if (l_FollowerTemplate && l_FollowerTemplate->Class[GetGarrisonFactionIndex()] == l_AbilityEffectEntry->MiscValueA)
                                {
                                    l_Proc = true;
                                    break;
                                }
                            }
                        }
                        break;

                    /// Proc if Duration > (3600 * MiscValueB)
                    case GARRISION_ABILITY_EFFECT_MOD_WIN_RATE_DURATION_MORE:
                        l_Proc = l_MissionDuration > (3600 * l_AbilityEffectEntry->MiscValueB);
                        break;

                    /// Proc if Duration < (3600 * MiscValueB)
                    case GARRISION_ABILITY_EFFECT_MOD_WIN_RATE_DURATION_LESS:
                        l_Proc = l_MissionDuration < (3600 * l_AbilityEffectEntry->MiscValueB);
                        break;

                    /// Proc if TravelDuration > (3600 * MiscValueB)
                    case GARRISION_ABILITY_EFFECT_MOD_WIN_RATE_TRAVEL_DURATION_MORE:
                        l_Proc = l_MissionTravelTime > (3600 * l_AbilityEffectEntry->MiscValueB);
                        break;

                    /// Proc if TravelDuration < (3600 * MiscValueB)
                    case GARRISION_ABILITY_EFFECT_MOD_WIN_RATE_TRAVEL_DURATION_LESS:
                        l_Proc = l_MissionTravelTime < (3600 * l_AbilityEffectEntry->MiscValueB);
                        break;

                    case GARRISION_ABILITY_EFFECT_UNK_0:
                    case GARRISION_ABILITY_EFFECT_MOD_TRAVEL_TIME:
                    case GARRISION_ABILITY_EFFECT_MOD_XP_GAIN:
                    case GARRISION_ABILITY_EFFECT_MOD_GARR_CURRENCY_DROP:
                    case GARRISION_ABILITY_EFFECT_UNK_11:
                    case GARRISION_ABILITY_EFFECT_MOD_DUMMY_PRODUCTION:
                    case GARRISION_ABILITY_EFFECT_MOD_BRONZE_TREASURE_DROP:
                    case GARRISION_ABILITY_EFFECT_MOD_SILVER_TREASURE_DROP:
                    case GARRISION_ABILITY_EFFECT_MOD_GOLD_TREASURE_DROP:
                    case GARRISION_ABILITY_EFFECT_MOD_CHEST_DROP_RATE:
                    case GARRISION_ABILITY_EFFECT_MOD_MISSION_DURATION:
                        break;

                    default:
                        break;

                }

                if (!l_Proc)
                    break;

                float l_Seil = 0.f;

                if (l_FollowersBiasMap[l_MissionFollowers[l_Y]->DB_ID] >= 0.0)
                    l_Seil = (l_AbilityEffectEntry->ModMax - l_AbilityEffectEntry->ModMin) * l_FollowersBiasMap[l_MissionFollowers[l_Y]->DB_ID] + l_AbilityEffectEntry->ModMin;
                else
                    l_Seil = (l_FollowersBiasMap[l_MissionFollowers[l_Y]->DB_ID] + 1.0) * l_AbilityEffectEntry->ModMin;

                l_CurrentAdditionalWinChance = (l_Seil * v62) + l_CurrentAdditionalWinChance;

                #ifdef GARRISON_CHEST_FORMULA_DEBUG
                    printf("Added %.2f to success due to follower %d trait %d.\n", (l_Seil * v62), l_MissionFollowers[l_Y]->FollowerID, l_AbilityEffectEntry->EffectType);
                #endif // GARRISON_CHEST_FORMULA_DEBUG
            }
        }
    }
    #pragma endregion 

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// UNTESTED
    #pragma region Passive Effect
    for (uint32 l_Y = 0; l_Y < l_PassiveEffects.size(); ++l_Y)
    {
        const GarrAbilityEffectEntry * l_AbilityEffectEntry = sGarrAbilityEffectStore.LookupEntry(l_PassiveEffects[l_Y]);

        if (!l_AbilityEffectEntry)
            continue;

        if (l_AbilityEffectEntry->EffectType == GARRISION_ABILITY_EFFECT_MOD_WIN_RATE)
        {
            l_CurrentAdditionalWinChance = (l_AbilityEffectEntry->ModMin * v62) + l_CurrentAdditionalWinChance;

            #ifdef GARRISON_CHEST_FORMULA_DEBUG
                printf("Added %.2f to success due to passive effect %d.\n", l_AbilityEffectEntry->ModMin * v62, l_AbilityEffectEntry->AbilityID);
            #endif // GARRISON_CHEST_FORMULA_DEBUG
        }
    }
    #pragma endregion 

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    l_CurrentAdditionalWinChance = (((100.0 - l_MissionTemplate->BaseBronzeChestChance) * l_CurrentAdditionalWinChance) * 0.0099999998) + l_MissionTemplate->BaseBronzeChestChance;

    if (l_CurrentAdditionalWinChance > 100.0)
        l_CurrentAdditionalWinChance = 100.0;

    #ifdef GARRISON_CHEST_FORMULA_DEBUG
        printf("Total success chance: %.2f.\n", l_CurrentAdditionalWinChance);
    #endif // GARRISON_CHEST_FORMULA_DEBUG

    return l_CurrentAdditionalWinChance;
}
/// Get missions
std::vector<GarrisonMission> Garrison::GetMissions()
{
    std::vector<GarrisonMission> l_Result;

    for (uint32 l_I = 0; l_I < m_Missions.size(); ++l_I)
    {
        if (m_Missions[l_I].State < GARRISON_MISSION_COMPLETE_SUCCESS
            && !((m_Missions[l_I].OfferTime + m_Missions[l_I].OfferMaxDuration) < time(0) && m_Missions[l_I].State == GARRISON_MISSION_AVAILABLE))
            l_Result.push_back(m_Missions[l_I]);
    }

    return l_Result;
}
/// Get all completed missions
std::vector<GarrisonMission> Garrison::GetCompletedMissions()
{
    std::vector<GarrisonMission> l_Result;

    for (uint32 l_I = 0; l_I < m_Missions.size(); ++l_I)
        if (m_Missions[l_I].State >= GARRISON_MISSION_COMPLETE_SUCCESS)
            l_Result.push_back(m_Missions[l_I]);

    return l_Result;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Add follower
bool Garrison::AddFollower(uint32 p_FollowerID)
{
    if (GetFollower(p_FollowerID).FollowerID != 0)
        return false;

    const GarrFollowerEntry * l_Entry = sGarrFollowerStore.LookupEntry(p_FollowerID);

    if (!l_Entry)
        return false;

    GarrisonFollower l_Follower;
    l_Follower.FollowerID           = p_FollowerID;
    l_Follower.Level                = l_Entry->Level;
    l_Follower.XP                   = 0;
    l_Follower.Quality              = l_Entry->Quality;
    l_Follower.ItemLevelArmor       = l_Entry->ItemLevelArmor;
    l_Follower.ItemLevelWeapon      = l_Entry->ItemLevelWeapon;
    l_Follower.CurrentBuildingID    = 0;
    l_Follower.CurrentMissionID     = 0;
    
    for (uint32 l_I = 0; l_I < sGarrFollowerXAbilityStore.GetNumRows(); ++l_I)
    {
        const GarrFollowerXAbilityEntry * l_Entry = sGarrFollowerXAbilityStore.LookupEntry(l_I);

        if (l_Entry && l_Entry->FollowerID == p_FollowerID && sGarrAbilityStore.LookupEntry(l_Entry->AbilityID) && l_Entry->FactionIndex == GetGarrisonFactionIndex())
            l_Follower.Abilities.push_back(l_Entry->AbilityID);
    }

    PreparedStatement* l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_GARRISON_FOLLOWER);
    
    uint32 l_Index = 0;
    l_Stmt->setUInt32(l_Index++, m_ID);
    l_Stmt->setUInt32(l_Index++, p_FollowerID);

    CharacterDatabase.Query(l_Stmt);

    l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_GARRISON_FOLLOWER_DB_ID);

    l_Index = 0;
    l_Stmt->setUInt32(l_Index++, m_ID);
    l_Stmt->setUInt32(l_Index++, p_FollowerID);

    PreparedQueryResult l_Result = CharacterDatabase.Query(l_Stmt);

    if (!l_Result)
        assert(false && "Garrison::AddFollower() failed to retrieve created garrison follower ID");

    Field * l_Fields = l_Result->Fetch();

    l_Follower.DB_ID = l_Fields[0].GetUInt32();

    m_Followers.push_back(l_Follower);

    WorldPacket l_AddFollowerResult(SMSG_GARRISON_ADD_FOLLOWER_RESULT, 64);
    l_AddFollowerResult << uint32(GARRISON_PURCHASE_BUILDING_OK);
    l_AddFollowerResult << uint64(l_Follower.DB_ID);
    l_AddFollowerResult << uint32(l_Follower.FollowerID);
    l_AddFollowerResult << uint32(l_Follower.Quality);
    l_AddFollowerResult << uint32(l_Follower.Level);
    l_AddFollowerResult << uint32(l_Follower.ItemLevelWeapon);
    l_AddFollowerResult << uint32(l_Follower.ItemLevelArmor);
    l_AddFollowerResult << uint32(l_Follower.XP);
    l_AddFollowerResult << uint32(l_Follower.CurrentBuildingID);
    l_AddFollowerResult << uint32(l_Follower.CurrentMissionID);

    l_AddFollowerResult << uint32(l_Follower.Abilities.size());
    l_AddFollowerResult << uint32(0);       ///< Unk

    for (uint32 l_Y = 0; l_Y < l_Follower.Abilities.size(); ++l_Y)
        l_AddFollowerResult << int32(l_Follower.Abilities[l_Y]);

    m_Owner->SendDirectMessage(&l_AddFollowerResult);

    return true;
}
/// Change follower activation state
void Garrison::ChangeFollowerActivationState(uint64 p_FollowerDBID, bool p_Active)
{
    GarrisonFollower * l_Follower = nullptr;

    if (p_Active)
    {
        if (!m_Owner->HasEnoughMoney((uint64)GARRISON_FOLLOWER_ACTIVATION_COST))
            return;

        if (GetNumFollowerActivationsRemaining() < 1)
            return;

        auto l_It = std::find_if(m_Followers.begin(), m_Followers.end(), [p_FollowerDBID](const GarrisonFollower & p_Follower) { return p_Follower.DB_ID == p_FollowerDBID; });

        if (l_It != m_Followers.end())
        {
            m_Owner->ModifyMoney(-GARRISON_FOLLOWER_ACTIVATION_COST);

            m_NumFollowerActivation--;
            m_NumFollowerActivationRegenTimestamp = time(0);

            l_It->Flags = l_It->Flags & ~GARRISON_FOLLOWER_FLAG_INACTIVE;
            l_Follower = &(*l_It);

            WorldPacket l_Data(SMSG_GARRISON_UPDATE_FOLLOWER_ACTIVATION_COUNT, 4);
            l_Data << uint32(GetNumFollowerActivationsRemaining());

            m_Owner->SendDirectMessage(&l_Data);
        }
    }
    else
    {
        if (!m_Owner->HasEnoughMoney((uint64)GARRISON_FOLLOWER_ACTIVATION_COST))
            return;

        auto l_It = std::find_if(m_Followers.begin(), m_Followers.end(), [p_FollowerDBID](const GarrisonFollower & p_Follower) { return p_Follower.DB_ID == p_FollowerDBID; });

        if (l_It != m_Followers.end())
        {
            m_Owner->ModifyMoney(-GARRISON_FOLLOWER_ACTIVATION_COST);

            l_It->Flags |= GARRISON_FOLLOWER_FLAG_INACTIVE;
            l_Follower = &(*l_It);
        }

        l_Follower = &*l_It;
    }

    if (!l_Follower)
        return;

    WorldPacket l_Update(SMSG_GARRISON_UPDATE_FOLLOWER, 500);
    l_Update << uint64(l_Follower->DB_ID);
    l_Update << uint32(l_Follower->FollowerID);
    l_Update << uint32(l_Follower->Quality);
    l_Update << uint32(l_Follower->Level);
    l_Update << uint32(l_Follower->ItemLevelWeapon);
    l_Update << uint32(l_Follower->ItemLevelArmor);
    l_Update << uint32(l_Follower->XP);
    l_Update << uint32(l_Follower->CurrentBuildingID);
    l_Update << uint32(l_Follower->CurrentMissionID);

    l_Update << uint32(l_Follower->Abilities.size());
    l_Update << uint32(l_Follower->Flags);

    for (uint32 l_Y = 0; l_Y < l_Follower->Abilities.size(); ++l_Y)
        l_Update << int32(l_Follower->Abilities[l_Y]);

    m_Owner->SendDirectMessage(&l_Update);
}
/// Get followers
std::vector<GarrisonFollower> Garrison::GetFollowers()
{
    return m_Followers;
}
/// Get follower
GarrisonFollower Garrison::GetFollower(uint32 p_FollowerID)
{
    for (uint32 l_I = 0; l_I < m_Followers.size(); l_I++)
    {
        if (m_Followers[l_I].FollowerID == p_FollowerID)
            return m_Followers[l_I];
    }

    GarrisonFollower l_FailResult;
    l_FailResult.FollowerID = 0;

    return l_FailResult;
}
/// Get activated followers count
uint32 Garrison::GetActivatedFollowerCount()
{
    uint32 l_ActivatedFollowerCount = 0;

    for (uint32 l_I = 0; l_I < m_Followers.size(); l_I++)
    {
        if ((m_Followers[l_I].Flags & GARRISON_FOLLOWER_FLAG_INACTIVE) == 0)
            l_ActivatedFollowerCount++;
    }

    return l_ActivatedFollowerCount;
}
/// Get num follower activation remaining
uint32 Garrison::GetNumFollowerActivationsRemaining()
{
    return m_NumFollowerActivation;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Can build building X at slot instance Y
bool Garrison::IsBuildingPlotInstanceValid(uint32 p_BuildingRecID, uint32 p_PlotInstanceID)
{
    const GarrBuildingEntry * l_BuildingEntry = sGarrBuildingStore.LookupEntry(p_BuildingRecID);

    if (!l_BuildingEntry)
        return false;

    const GarrPlotInstanceEntry * l_PlotInstanceEntry = sGarrPlotInstanceStore.LookupEntry(p_PlotInstanceID);

    if (!l_PlotInstanceEntry)
        return false;

    /// Search building plot ID 
    for (uint32 l_I = 0; l_I < sGarrPlotBuildingStore.GetNumRows(); ++l_I)
    {
        const GarrPlotBuildingEntry * l_PlotBuildingEntry = sGarrPlotBuildingStore.LookupEntry(l_I);

        if (l_PlotBuildingEntry && l_PlotBuildingEntry->PlotId == l_PlotInstanceEntry->PlotID && l_PlotBuildingEntry->BuildingID == p_BuildingRecID)
            return true;
    }

    return false;
}
/// Player fill all condition
GarrisonPurchaseBuildingResult Garrison::CanPurchaseBuilding(uint32 p_BuildingRecID)
{
    const GarrBuildingEntry * l_BuildingEntry = sGarrBuildingStore.LookupEntry(p_BuildingRecID);

    if (!l_BuildingEntry)
        return GARRISON_PURCHASE_BUILDING_INVALID_BUILDING_ID;

    if (l_BuildingEntry->BuildCostCurrencyID != 0)
    {
        if (!m_Owner->HasCurrency(l_BuildingEntry->BuildCostCurrencyID, l_BuildingEntry->BuildCostCurrencyAmount))
            return GARRISON_PURCHASE_BUILDING_NOT_ENOUGH_CURRENCY;
    }

    return GARRISON_PURCHASE_BUILDING_OK;
}
/// PurchaseBuilding
GarrisonBuilding Garrison::PurchaseBuilding(uint32 p_BuildingRecID, uint32 p_PlotInstanceID, bool p_Triggered)
{
    const GarrBuildingEntry * l_BuildingEntry = sGarrBuildingStore.LookupEntry(p_BuildingRecID);

    GarrisonBuilding l_Building;

    memset(&l_Building, 0, sizeof(l_Building));

    if (!l_BuildingEntry)
        return l_Building;

    if (!PlotIsFree(p_PlotInstanceID))
        DeleteBuilding(p_PlotInstanceID);

    if (l_BuildingEntry->BuildCostCurrencyID != 0 && !p_Triggered)
        m_Owner->ModifyCurrency(l_BuildingEntry->BuildCostCurrencyID, -(int32)l_BuildingEntry->BuildCostCurrencyAmount);

    if (l_BuildingEntry->MoneyCost != 0 && !p_Triggered)
        m_Owner->ModifyMoney(-(int32)l_BuildingEntry->MoneyCost);

    if (!p_Triggered)
    {
        WorldPacket l_PlotRemoved(SMSG_GARRISON_PLOT_REMOVED, 4);
        l_PlotRemoved << uint32(p_PlotInstanceID);
        m_Owner->SendDirectMessage(&l_PlotRemoved);
    }

    l_Building.BuildingID       = p_BuildingRecID;
    l_Building.PlotInstanceID   = p_PlotInstanceID;
    l_Building.TimeBuiltStart   = time(0);
    l_Building.TimeBuiltEnd     = time(0) + l_BuildingEntry->BuildTime;           ///< 5/5/1905 18:45:05
    l_Building.SpecID           = 0;
    l_Building.Active           = false;
    l_Building.BuiltNotified    = false;

    if (p_Triggered)
        l_Building.TimeBuiltEnd = l_Building.TimeBuiltStart;

    PreparedStatement* l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_GARRISON_BUILDING);

    uint32 l_Index = 0;
    l_Stmt->setUInt32(l_Index++, m_ID);
    l_Stmt->setUInt32(l_Index++, p_PlotInstanceID);
    l_Stmt->setUInt32(l_Index++, p_BuildingRecID);

    CharacterDatabase.Query(l_Stmt);

    l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_GARRISON_BUILDING_DB_ID);

    l_Index = 0;
    l_Stmt->setUInt32(l_Index++, m_ID);
    l_Stmt->setUInt32(l_Index++, p_PlotInstanceID);
    l_Stmt->setUInt32(l_Index++, p_BuildingRecID);

    PreparedQueryResult l_Result = CharacterDatabase.Query(l_Stmt);

    if (!l_Result)
        assert(false && "Garrison::PurchaseBuilding() failed to retrieve created garrison building ID");

    Field * l_Fields = l_Result->Fetch();

    l_Building.DB_ID = l_Fields[0].GetUInt32();

    m_Buildings.push_back(l_Building);

    UpdatePlot(p_PlotInstanceID);
    
    return l_Building;
}
/// Get building
GarrisonBuilding Garrison::GetBuilding(uint32 p_PlotInstanceID)
{
    for (uint32 l_I = 0; l_I < m_Buildings.size(); ++l_I)
        if (m_Buildings[l_I].PlotInstanceID == p_PlotInstanceID)
            return m_Buildings[l_I];

    return GarrisonBuilding();
}
/// Get buildings
std::vector<GarrisonBuilding> Garrison::GetBuildings()
{
    return m_Buildings;
}
/// Get building passive ability effects
std::vector<uint32> Garrison::GetBuildingsPassiveAbilityEffects()
{
    std::vector<uint32> l_PassiveEffects;

    for (uint32 l_I = 0; l_I < m_Buildings.size(); ++l_I)
    {
        const GarrBuildingEntry * l_BuildingTemplate = sGarrBuildingStore.LookupEntry(m_Buildings[l_I].BuildingID);

        if (l_BuildingTemplate && l_BuildingTemplate->PassiveEffect && sGarrAbilityEffectStore.LookupEntry(l_BuildingTemplate->PassiveEffect) != nullptr)
            l_PassiveEffects.push_back(l_BuildingTemplate->PassiveEffect);
    }

    return l_PassiveEffects;
}
/// Activate building
void Garrison::ActivateBuilding(uint32 p_PlotInstanceID)
{
    GarrisonBuilding * l_Building = nullptr;

    for (uint32 l_I = 0; l_I < m_Buildings.size(); ++l_I)
    {
        if (m_Buildings[l_I].PlotInstanceID == p_PlotInstanceID)
        {
            l_Building = &m_Buildings[l_I];
            break;
        }
    }

    if (!l_Building)
        return;

    const GarrBuildingEntry * l_BuildingEntry = sGarrBuildingStore.LookupEntry(l_Building->BuildingID);

    if (!l_BuildingEntry)
        return;

    l_Building->Active = true;

    UpdatePlot(p_PlotInstanceID);

    WorldPacket l_Packet(SMSG_GARRISON_BUILDING_ACTIVATED, 4);
    l_Packet << uint32(p_PlotInstanceID);
    m_Owner->SendDirectMessage(&l_Packet);

    UpdateStats();
}
/// Activate building
void Garrison::ActivateBuilding()
{
    uint32 l_PlotInstance = GetPlotInstanceIDByActivationGameObject(m_LastUsedActivationGameObject);

    if (l_PlotInstance)
        ActivateBuilding(l_PlotInstance);
}
/// Cancel construction
void Garrison::CancelConstruction(uint32 p_PlotInstanceID)
{
    if (!HasPlotInstance(p_PlotInstanceID))
        return;

    uint32 l_BuildingID = GetBuilding(p_PlotInstanceID).BuildingID;

    if (!l_BuildingID)
        return;

    const GarrBuildingEntry * l_BuildingEntry = sGarrBuildingStore.LookupEntry(l_BuildingID);

    if (!l_BuildingEntry)
        return;

    DeleteBuilding(p_PlotInstanceID);

    if (l_BuildingEntry->BuildCostCurrencyID != 0)
        m_Owner->ModifyCurrency(l_BuildingEntry->BuildCostCurrencyID, (int32)l_BuildingEntry->BuildCostCurrencyAmount);

    if (l_BuildingEntry->MoneyCost != 0)
        m_Owner->ModifyMoney(l_BuildingEntry->MoneyCost);
}
/// Delete building
void Garrison::DeleteBuilding(uint32 p_PlotInstanceID)
{
    if (!HasPlotInstance(p_PlotInstanceID))
        return;

    uint32 l_BuildingID = GetBuilding(p_PlotInstanceID).BuildingID;

    if (!l_BuildingID)
        return;

    const GarrBuildingEntry * l_BuildingEntry = sGarrBuildingStore.LookupEntry(l_BuildingID);

    if (!l_BuildingEntry)
        return;

    PreparedStatement * l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GARRISON_BUILDING);
    l_Stmt->setUInt32(0, GetBuilding(p_PlotInstanceID).DB_ID);
    CharacterDatabase.AsyncQuery(l_Stmt);

    for (std::vector<GarrisonBuilding>::iterator l_It = m_Buildings.begin(); l_It != m_Buildings.end(); ++l_It)
    {
        if (l_It->BuildingID == l_BuildingID)
        {
            m_Buildings.erase(l_It);
            break;
        }
    }

    UpdatePlot(p_PlotInstanceID);

    GarrisonPlotInstanceInfoLocation l_PlotLocation = GetPlot(p_PlotInstanceID);

    WorldPacket l_PlotPlacedPacket(SMSG_GARRISON_PLOT_PLACED, 24);
    l_PlotPlacedPacket << uint32(p_PlotInstanceID);
    l_PlotPlacedPacket << float(l_PlotLocation.X);
    l_PlotPlacedPacket << float(l_PlotLocation.Y);
    l_PlotPlacedPacket << float(l_PlotLocation.Z);
    l_PlotPlacedPacket << float(l_PlotLocation.O);
    l_PlotPlacedPacket << uint32(GetPlotType(p_PlotInstanceID));

    m_Owner->SendDirectMessage(&l_PlotPlacedPacket);

    WorldPacket l_BuildingRemovedPacket(SMSG_GARRISON_BUILDING_REMOVED, 12);
    l_BuildingRemovedPacket << uint32(GARRISON_PURCHASE_BUILDING_OK);
    l_BuildingRemovedPacket << uint32(p_PlotInstanceID);
    l_BuildingRemovedPacket << uint32(l_BuildingID);

    m_Owner->SendDirectMessage(&l_BuildingRemovedPacket);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Get known blueprints
std::vector<int32> Garrison::GetKnownBlueprints()
{
    return m_KnownBlueprints;
}
/// Learn blue print
bool Garrison::LearnBlueprint(uint32 p_BuildingRecID)
{
    GarrisonLearnBluePrintResult l_ResultCode;

    if (std::find(m_KnownBlueprints.begin(), m_KnownBlueprints.end(), p_BuildingRecID) == m_KnownBlueprints.end())
    {
        const GarrBuildingEntry * l_BuildingEntry = sGarrBuildingStore.LookupEntry(p_BuildingRecID);

        if (l_BuildingEntry)
        {
            m_KnownBlueprints.push_back(p_BuildingRecID);
            l_ResultCode = GARRISON_LEARN_BLUEPRINT_LEARNED;
        }
        else
        {
            l_ResultCode = GARRISON_LEARN_BLUEPRINT_UNABLE_TO_LEARN;
        }
    }
    else
    {
        l_ResultCode = GARRISON_LEARN_BLUEPRINT_KNOWN;
    }

    WorldPacket l_Result(SMSG_GARRISON_LEARN_BLUEPRINT_RESULT, 8);
    l_Result << uint32(l_ResultCode);
    l_Result << uint32(p_BuildingRecID);

    m_Owner->SendDirectMessage(&l_Result);

    return true;
}
/// Known blue print
bool Garrison::KnownBlueprint(uint32 p_BuildingRecID)
{
    return std::find(m_KnownBlueprints.begin(), m_KnownBlueprints.end(), p_BuildingRecID) != m_KnownBlueprints.end();
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Get known specializations
std::vector<int32> Garrison::GetKnownSpecializations()
{
    return m_KnownSpecializations;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Init
void Garrison::Init()
{
    InitDataForLevel();
    InitPlots();
    UpdateStats();
}
/// Init data for level
void Garrison::InitDataForLevel()
{
    const GarrSiteLevelEntry * l_SiteEntry = nullptr;

    /// Search garrison site entry by SideID & Level
    for (uint32 l_I = 0; l_I < sGarrSiteLevelStore.GetNumRows(); ++l_I)
    {
        const GarrSiteLevelEntry * l_CurrentSiteEntry = sGarrSiteLevelStore.LookupEntry(l_I);

        if (l_CurrentSiteEntry && l_CurrentSiteEntry->Level == m_GarrisonLevel && l_CurrentSiteEntry->SiteID == m_GarrisonSiteID)
        {
            l_SiteEntry = l_CurrentSiteEntry;
            break;
        }
    }

    if (!l_SiteEntry)
        assert(false && "Garrison::InitDataForLevel() not data found");

    m_GarrisonLevelID = l_SiteEntry->SiteLevelID;

    /// Find all plots for garrison level
    m_Plots.clear();

    for (uint32 l_I = 0; l_I < GARRISON_PLOT_INSTANCE_COUNT; ++l_I)
    {
        if (gGarrisonPlotInstanceInfoLocation[l_I].SiteLevelID == m_GarrisonLevelID)
            m_Plots.push_back(gGarrisonPlotInstanceInfoLocation[l_I]);
    }
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Init Game objects
void Garrison::InitPlots()
{
    if (m_Owner->GetMapId() != GetGarrisonSiteLevelEntry()->MapID)
        return;

    for (uint32 l_I = 0; l_I < m_Plots.size(); ++l_I)
        UpdatePlot(m_Plots[l_I].PlotInstanceID);
}
/// Uninit plots
void Garrison::UninitPlots()
{
    for (std::map<uint32, uint64>::iterator l_It = m_PlotsActivateGob.begin(); l_It != m_PlotsActivateGob.end(); ++l_It)
    {
        GameObject * l_Gob = HashMapHolder<GameObject>::Find(l_It->second);

        if (l_Gob)
        {
            l_Gob->DestroyForNearbyPlayers();
            l_Gob->CleanupsBeforeDelete();
            delete l_Gob;
        }
    }

    m_PlotsActivateGob.clear();

    for (std::map<uint32, uint64>::iterator l_It = m_PlotsGob.begin(); l_It != m_PlotsGob.end(); ++l_It)
    {
        GameObject * l_Gob = HashMapHolder<GameObject>::Find(l_It->second);

        if (l_Gob)
        {
            l_Gob->DestroyForNearbyPlayers();
            l_Gob->CleanupsBeforeDelete();
            delete l_Gob;
        }
    }

    m_PlotsGob.clear();

    for (std::map<uint32, std::vector<uint64>>::iterator l_It = m_PlotsBuildingCosmeticGobs.begin(); l_It != m_PlotsBuildingCosmeticGobs.end(); ++l_It)
    {
        for (uint32 l_Y = 0; l_Y < l_It->second.size(); ++l_Y)
        {
            GameObject * l_Gob = HashMapHolder<GameObject>::Find(l_It->second[l_Y]);

            if (l_Gob)
            {
                l_Gob->DestroyForNearbyPlayers();
                l_Gob->CleanupsBeforeDelete();
                delete l_Gob;
            }
        }
    }

    m_PlotsBuildingCosmeticGobs.clear();
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Update plot game object
void Garrison::UpdatePlot(uint32 p_PlotInstanceID)
{
    if (m_Owner->GetMapId() != GetGarrisonSiteLevelEntry()->MapID)
        return;

    GarrisonPlotInstanceInfoLocation l_PlotInfo = GetPlot(p_PlotInstanceID);
    
    if (m_PlotsGob[p_PlotInstanceID] != 0)
    {
        GameObject * l_Gob = HashMapHolder<GameObject>::Find(m_PlotsGob[p_PlotInstanceID]);

        if (l_Gob)
        {
            l_Gob->DestroyForNearbyPlayers();
            l_Gob->CleanupsBeforeDelete();
            delete l_Gob;
        }

        m_PlotsGob[p_PlotInstanceID] = 0;
    }
    
    uint32 l_GobEntry = 0;
    bool l_SpanwActivateGob = false;
    bool l_IsPlotBuilding = false;

    if (PlotIsFree(p_PlotInstanceID))
    {
        l_GobEntry = gGarrisonEmptyPlotGameObject[GetPlotType(p_PlotInstanceID) + (GetGarrisonFactionIndex() * GARRISON_PLOT_TYPE_MAX)];
    }
    else
    {
        GarrisonBuilding l_Building = GetBuilding(p_PlotInstanceID);

        if (!l_Building.Active)
        {
            l_GobEntry = gGarrisonBuildingPlotGameObject[GetPlotType(p_PlotInstanceID) + (GetGarrisonFactionIndex() * GARRISON_PLOT_TYPE_MAX)];

            if (time(0) > l_Building.TimeBuiltEnd)
                l_SpanwActivateGob  = true;

            l_IsPlotBuilding = true;
        }
        else
        {
            const GarrBuildingEntry * l_BuildingEntry = sGarrBuildingStore.LookupEntry(l_Building.BuildingID);
            
            if (!l_BuildingEntry)
                return;

            l_GobEntry = l_BuildingEntry->GameObjects[GetGarrisonFactionIndex()];
        }
    }

    if (l_GobEntry != 0)
    {
        GameObject * l_Gob = m_Owner->SummonGameObject(l_GobEntry, l_PlotInfo.X, l_PlotInfo.Y, l_PlotInfo.Z, l_PlotInfo.O, 0, 0, 0, 0, 0);
        
        if (l_Gob)
        {
            m_PlotsGob[p_PlotInstanceID] = l_Gob->GetGUID();

            if (l_IsPlotBuilding)
            {
                G3D::Vector3 l_NonRotatedPosition;

                {
                    G3D::Matrix3 l_Mat = G3D::Matrix3::identity();
                    l_Mat = l_Mat.fromAxisAngle(G3D::Vector3(0, 0, 1), -l_PlotInfo.O);

                    /// transform plot coord
                    l_NonRotatedPosition = l_Mat * G3D::Vector3(l_PlotInfo.X, l_PlotInfo.Y, l_PlotInfo.Z);
                }

                std::vector<GarrisonPlotBuildingContent> l_Contents = sObjectMgr->GetGarrisonPlotBuildingContent(GetPlotType(p_PlotInstanceID), GetGarrisonFactionIndex());

                for (uint32 l_I = 0; l_I < l_Contents.size(); ++l_I)
                {
                    G3D::Vector3 l_Position = G3D::Vector3(l_Contents[l_I].X, l_Contents[l_I].Y, 0);

                    G3D::Matrix3 l_Mat = G3D::Matrix3::identity();
                    l_Mat = l_Mat.fromAxisAngle(G3D::Vector3(0, 0, 1), l_PlotInfo.O);

                    l_Position.x += l_NonRotatedPosition.x;
                    l_Position.y += l_NonRotatedPosition.y;

                    l_Position = l_Mat * l_Position;

                    l_Position.z = l_Contents[l_I].Z + l_PlotInfo.Z;

                    if (l_Contents[l_I].CreatureOrGob > 0)
                    {
                        auto ee = m_Owner->SummonCreature(l_Contents[l_I].CreatureOrGob, l_Position.x, l_Position.y, l_Position.z, l_Contents[l_I].O, TEMPSUMMON_MANUAL_DESPAWN);
                        ee->RemoveFlag(UNIT_FIELD_FLAGS2, UNIT_FLAG2_UNK1);
                    }
                    else
                    {
                        GameObject * l_Cosmetic = m_Owner->SummonGameObject(-l_Contents[l_I].CreatureOrGob, l_Position.x, l_Position.y, l_Position.z, l_Contents[l_I].O, 0, 0, 0, 0, 0);

                        m_PlotsBuildingCosmeticGobs[p_PlotInstanceID].push_back(l_Cosmetic->GetGUID());
                    }
                }
            }
            else if (m_PlotsBuildingCosmeticGobs[p_PlotInstanceID].size() != 0)
            {
                for (uint32 l_I = 0; l_I < m_PlotsBuildingCosmeticGobs[p_PlotInstanceID].size(); ++l_I)
                {
                    GameObject * l_Gob = HashMapHolder<GameObject>::Find(m_PlotsBuildingCosmeticGobs[p_PlotInstanceID][l_I]);

                    if (l_Gob)
                    {
                        l_Gob->DestroyForNearbyPlayers();
                        l_Gob->CleanupsBeforeDelete();
                        delete l_Gob;
                    }
                }

                m_PlotsBuildingCosmeticGobs[p_PlotInstanceID].clear();
            }

            if (l_SpanwActivateGob)
            {
                /// For this part we use an matrix to transform plot coord in, order to get the position without the rotation
                /// Once we have the "non rotated" position, we compute activation game object position in a 2 dimensional system
                /// And after we reapply the rotation on coords to transform and get the correct final position
                G3D::Matrix3 l_Mat = G3D::Matrix3::identity();
                l_Mat = l_Mat.fromAxisAngle(G3D::Vector3(0, 0, 1), -l_PlotInfo.O);

                /// transform plot coord
                G3D::Vector3 l_NonRotatedPosition = l_Mat * G3D::Vector3(l_PlotInfo.X, l_PlotInfo.Y, l_PlotInfo.Z);

                const GameObjectDisplayInfoEntry * l_GobDispInfo = sGameObjectDisplayInfoStore.LookupEntry(l_Gob->GetDisplayId());

                /// Work with plot AABB
                if (l_GobDispInfo)
                {
                    /// Get AABB on X axis
                    float l_XAxisSize = fabs(l_GobDispInfo->maxX - l_GobDispInfo->minX) * l_Gob->GetFloatValue(OBJECT_FIELD_SCALE);

                    /// We use a "diminish return" on box size for big plots
                    l_NonRotatedPosition.x += l_XAxisSize * (gGarrisonBuildingPlotAABBDiminishReturnFactor[GetPlotType(p_PlotInstanceID) + (GetGarrisonFactionIndex() * GARRISON_PLOT_TYPE_MAX)] / l_XAxisSize);
                }

                l_Mat = G3D::Matrix3::identity();
                l_Mat = l_Mat.fromAxisAngle(G3D::Vector3(0, 0, 1), l_PlotInfo.O);

                /// Reapply the rotation on coords
                G3D::Vector3 l_FinalPosition = l_Mat * l_NonRotatedPosition;

                uint32 l_AnimProgress   = 0;
                uint32 l_Health         = 255;

                GameObject * l_ActivationGob = m_Owner->SummonGameObject(gGarrisonBuildingActivationGameObject[GetGarrisonFactionIndex()], l_FinalPosition.x, l_FinalPosition.y, l_FinalPosition.z, l_PlotInfo.O, 0, 0, 0, 0, 0, 0, 0, l_AnimProgress, l_Health);
                
                if (l_ActivationGob)
                    m_PlotsActivateGob[p_PlotInstanceID] = l_ActivationGob->GetGUID();
            }
            else if (m_PlotsActivateGob[p_PlotInstanceID] != 0)
            {
                GameObject * l_Gob = HashMapHolder<GameObject>::Find(m_PlotsActivateGob[p_PlotInstanceID]);

                if (l_Gob)
                {
                    l_Gob->CleanupsBeforeDelete();
                    delete l_Gob;
                }
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Update garrison stats
void Garrison::UpdateStats()
{
    uint32 l_BonusMaxActiveFollower = 0;

    /// Some of const values used here are unknown
    /// See GetFollowerSoftCap in client for more details
    for (uint32 l_I = 0; l_I < m_Buildings.size(); ++l_I)
    {
        const GarrBuildingEntry * l_Building = sGarrBuildingStore.LookupEntry(m_Buildings[l_I].BuildingID);

        if (!l_Building)
            continue;

        if (l_Building->Unk3 != 8)
            continue;

        l_BonusMaxActiveFollower = l_Building->Unk7;

        for (uint32 l_Y = 0; l_Y < sGarrSpecializationStore.GetNumRows(); ++l_Y)
        {
            const GarrSpecializationEntry * l_Specialization = sGarrSpecializationStore.LookupEntry(l_Y);

            if (!l_Specialization)
                continue;

            if (   l_Specialization->Unk2 == l_Building->Unk3
                && l_Specialization->Unk4 <= l_Building->BuildingLevel
                && l_Specialization->Unk3 == 10)
            {
                l_BonusMaxActiveFollower += floor(l_Specialization->BasePoint);
            }
        }
    }

    m_Stat_MaxActiveFollower = l_BonusMaxActiveFollower + GARRISON_DEFAULT_MAX_ACTIVE_FOLLOW;
}