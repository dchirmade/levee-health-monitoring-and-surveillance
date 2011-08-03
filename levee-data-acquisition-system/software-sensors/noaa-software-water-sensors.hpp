//
// 
// Levee Health Monitoring And Surveillance System (Real-time)
// Copyright (C) 2011 Fibercorps (http://www.fibercorps.com) 
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
// 
// GSoC11 Developer: Dipak 
// GSoC11 Dashboard: http://www.google-melange.com/gsoc/project/google/gsoc2011/q1w2e3r4/12001
// GSoC11 Mentor   : Dr.Raju Gottumukkala and Crawford Comeaux
// 
// This software sensor will tune to water sensors near to New Orleans. Currently only few stations are sync-ed for demonstration. 
// Link Source: http://water.weather.gov 
//

#ifndef NOAA_SOFTWARE_WATER_SENSORS_HPP
#define NOAA_SOFTWARE_WATER_SENSORS_HPP

// Include files for string and vector operations 
#include <string>
#include <stdexcept>
#include <vector>

using namespace std;

//
//
// Desc: Class, Water Sensor, This class is responsible for fetching rss feeds from water stations          
// 
 
class WaterSensors {

public:

      WaterSensors( void );                               // Initialization if any! 
      ~WaterSensors( void );                              // Memory de-allocation if any! 
      void printDebugMessages( string, bool);             // Prints debug messages  
      void downloadWaterRSSfeeds( string );               // Downloads rss feeds from Hydrologic station  
      bool crawlThroughStationsData( bool );              // Does crawling on station's rss feeds   
      string parseURLandPullOutStationName( string );     // Extracts station name from RSS URL 
      void readAndParsePerWaterStationResponse( string ); // Does parsing and params extraction  
private:
    
      // Is debugger enabled?  
      bool isDebugEnabled; 
}; 

#endif
