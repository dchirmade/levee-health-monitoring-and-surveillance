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

// Currently only few stations nearby to New Orleans are considered for demo. At later stage all water rss feeds 
// e.g. http://water.weather.gov/ahps2/rss/ will be considered.    

#define __Lakefront "http://water.weather.gov/ahps2/rss/obs/nwcl1.rss"
#define __LaBranche "http://water.weather.gov/ahps2/rss/obs/labl1.rss"
#define __ShellBeach "http://water.weather.gov/ahps2/rss/obs/shbl1.rss"

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
      string crawlThroughStationsData( bool, string );      // Does crawling on station's rss feeds   
      string parseURLandPullOutStationName( string );     // Extracts station name from RSS URL 
      string readAndParsePerWaterStationResponse( string ); // Does parsing and params extraction  
private:
    
      // Is debugger enabled?  
      bool isDebugEnabled; 
}; 

#endif
