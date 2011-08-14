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
 

#ifndef NOAA_SOFTWARE_WATER_SENSORS_CPP
#define NOAA_SOFTWARE_WATER_SENSORS_CPP

#include <string>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <fstream>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <cstdlib> 

#include "noaa-software-water-sensors.hpp"

// Currently only few stations nearby to New Orleans are considered for demo. At later stage all water rss feeds 
// e.g. http://water.weather.gov/ahps2/rss/ will be considered.    

#define __Lakefront "http://water.weather.gov/ahps2/rss/obs/nwcl1.rss"
#define __LaBranche "http://water.weather.gov/ahps2/rss/obs/labl1.rss"
#define __ShellBeach "http://water.weather.gov/ahps2/rss/obs/shbl1.rss"

using namespace std;

//
// Desc: This should do initialization e.g. enable debugger etc   
// Arguments: void, nothing 
// Returns: Nothing, void 
//  

WaterSensors::WaterSensors( void ){

   // Do initialization neeed for weather sensor 
   isDebugEnabled = true;

}

//
// Desc: This should do de-allocation if any    
// Arguments: void, nothing 
// Returns: Nothing, void 
// 

WaterSensors::~WaterSensors( void ){

   // Nothing so far   
}

//
// Desc: This should print debug messages only if it is enabled.   
// Arguments: String, Lines which needs to be printed on standard console.
//            Bool, Show debug messages forcefully 
// Returns: Nothing, void 
//   
 
void WaterSensors::printDebugMessages( 
                                       string debugLines,
                                       bool isDebugForced = false
                                     ){

   // Get current time and stamp it with logs 
   time_t tRawTime;
   time ( &tRawTime );

   string tLocaltime = asctime( localtime( &tRawTime ) );
   tLocaltime.replace( tLocaltime.find( "\n" ), 1 ,"\0" ); // Replace \n at the end of time string


   // Print debug messages to standard console. 
   if( isDebugEnabled || isDebugForced )
       cout << "<br>" << tLocaltime << " :" << debugLines << endl; 

   return; 
}
#endif

//
// Desc: Downloads water rss feeds  
// Arguments: string, Complete RSS URL updated by station 
// Returns: Nothing, void 
// 

void WaterSensors::downloadWaterRSSfeeds( 
                                         string waterRssStationURL  
				        ){

    while( true ){  // non infinite loop! 

     if( waterRssStationURL.length() == 0 )
         break; // An empty URL or rss station. Skip it!  
  
     // Build wget payload
     printDebugMessages( "Downloading XML RSS water station feeds from: " + parseURLandPullOutStationName( waterRssStationURL ) );
     string tDownloadXMLFeedsstringBuffer = "/usr/bin/wget --tries=3 --wait=1 --quiet --directory-prefix=dump " + 
                                            waterRssStationURL + " >>/dev/null" + " 2>>/dev/null";
     system( tDownloadXMLFeedsstringBuffer.c_str() );

     break;
   }
   return; 
}

//
// Desc: This method extracts station name from RSS URL 
// Arguments: string, Complete RSS URL 
// Returns: string, Extracted station name. Generally is in short forms! 
// 
string WaterSensors::parseURLandPullOutStationName( string RSSUrl ){

   string stationName = "";

   // Do String-tok and pull out station name 
   if( RSSUrl.length() != 0 )
       stationName = RSSUrl.substr( RSSUrl.rfind( "/" ) + 1 );

   return stationName;
}

//
// Desc: This should do crawling though all fetched RSS URL
// Arguments: bool, is it needed to update all RSS feeds or not? 
// Returns: bool, true if all is well otherwise false
// 

bool WaterSensors::crawlThroughStationsData( bool isUpdateNeeded = false ){

   bool tReturn = false;

   // Delete all old rss dump files if update needed
   if( isUpdateNeeded == true ){
       system( "/bin/rm ./dump/*.rss 2>>/dev/null >>/dev/null" );
       system( "/bin/rm ./dump/*.rss.* 2>>/dev/null >>/dev/null" );

      // Download RSS feeds only for few locations for the time being. 
      downloadWaterRSSfeeds( __Lakefront );  
      downloadWaterRSSfeeds( __LaBranche );
      downloadWaterRSSfeeds( __ShellBeach );
      
    }

      string tStationName = "./dump/" + parseURLandPullOutStationName( __Lakefront );
      if( tStationName.length() != 0 )
         readAndParsePerWaterStationResponse( tStationName );
      else printDebugMessages( "Opps! Couldn't fetch station name from RSSURL!" );

      tStationName = "./dump/" + parseURLandPullOutStationName( __LaBranche );
      if( tStationName.length() != 0 )
         readAndParsePerWaterStationResponse( tStationName );
      else printDebugMessages( "Opps! Couldn't fetch station name from RSSURL!" );

      tStationName = "./dump/" + parseURLandPullOutStationName( __ShellBeach );
      if( tStationName.length() != 0 )
         readAndParsePerWaterStationResponse( tStationName );
      else printDebugMessages( "Opps! Couldn't fetch station name from RSSURL!" );
     
   // Just assume all is good! 
   return tReturn;
}

//
// Desc: This method does parsing and extraction of demanded attributes 
// Arguments: string, Path to fetched RSS file 
// Returns: Nothing, void 
// 

void WaterSensors::readAndParsePerWaterStationResponse( string waterStationRssFeeds){

  int tRssTextLength = 0;
  char * tStringBuffer = NULL;

  // Create new file instance to store rss processed output 
  system( "/bin/rm /tmp/rss.txt >> /dev/null 2>>/dev/null; /usr/bin/touch /tmp/rss.txt >>/dev/null 2>>/dev/null" ); 

  // Do some RSS file processing
  // Get title of the station  
  string tExecCommand = "cat " + waterStationRssFeeds + " | grep '<title>' | tail -1  | cut -f3 -d'&' > /tmp/rss.txt";
  system( tExecCommand.c_str() ); 
  // Get observation time 
  tExecCommand = "cat " + waterStationRssFeeds + " | grep 'Observation Time:' | tail -1 | cut -f1 -d'&' >> /tmp/rss.txt";
  system( tExecCommand.c_str() ); 
  // Get Tide hight in fts
  tExecCommand = "cat " + waterStationRssFeeds + " | grep 'Latest Observation:' | cut -f3 -d'&' >> /tmp/rss.txt";
  system( tExecCommand.c_str() ); 

  ifstream fileHandle;
  fileHandle.open ("/tmp/rss.txt", ios::binary );
 

  // Get length of file
  fileHandle.seekg (0, ios::end);
  tRssTextLength = fileHandle.tellg();
  fileHandle.seekg (0, ios::beg);

  if( tRssTextLength <= 0  ){

     printDebugMessages( "Failed to process RSS files or RSS file is empty!" );
     fileHandle.close();
     return; 
  }

  // Allocate memory according to file length  
  tStringBuffer = new char [ tRssTextLength + 8 ];

  if( tStringBuffer ){
    // Read data as a block:
    fileHandle.read ( tStringBuffer, tRssTextLength );

    // Fixme! Process extracted out put to cross verify with knowledge base at later state 
    // cout.write ( tStringBuffer, tRssTextLength );
    if( tRssTextLength )
        tStringBuffer[ tRssTextLength + 1 ] = '\0' ;
    printDebugMessages( tStringBuffer );
 
    delete [] tStringBuffer;
    fileHandle.close();
   }
   
   return; 
}

//
// Desc: Main for this code base.   
// Arguments: Nothing, void
// Returns: int, Program exit status. 
//   
 
#ifdef WATER_MAIN 
int main( void ){

  // Create water sensor's instance and see if it is fetching the reading or not... 
  WaterSensors waterSensor;    
  waterSensor.crawlThroughStationsData( true );
  return EXIT_SUCCESS; 
} 

#endif
