////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef GARRISON_BUILDING_HPP_GARRISON
#define GARRISON_BUILDING_HPP_GARRISON

#include <string>

namespace MS { namespace Garrison 
{
    /// Garrison building
    struct GarrisonBuilding
    {
        uint32 DatabaseID;      ///< Building DB ID
        uint32 PlotInstanceID;  ///< Plot instance ID
        uint32 BuildingID;      ///< Building ID (GarrBuilding.db2)
        uint32 SpecID;          ///< Building spec ID
        uint32 TimeBuiltStart;  ///< Time built start
        uint32 TimeBuiltEnd;    ///< Time build end

        std::string GatheringData;  ///< Building gathering data

        bool Active;            ///< Is building active
        bool BuiltNotified;     ///< Is built notified

        /// Reset building
        void Reset()
        {
            DatabaseID      = 0;
            PlotInstanceID  = 0;
            BuildingID      = 0;
            SpecID          = 0;
            TimeBuiltStart  = 0;
            TimeBuiltEnd    = 0;

            GatheringData = "";

            Active          = false;
            BuiltNotified   = false;
        }
    };

}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_BUILDING_HPP_GARRISON