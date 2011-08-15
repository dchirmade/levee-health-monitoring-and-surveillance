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
//            string, Complete URL of a water station in question. 
// Returns: string, Water level in fts
// 

string WaterSensors::crawlThroughStationsData( 
                                            bool isUpdateNeeded = false,
                                            string stationUrl = ""
                                           ){

   string tWaterLevel = "-99";
   string tStationName = "./dump/" + parseURLandPullOutStationName( stationUrl );

   // Delete all old rss dump files if update needed
   if( isUpdateNeeded == true ){
       // Do not delete all rss feeds at a time. Delete them one at a time 
       // system( "/bin/rm ./dump/*.rss 2>>/dev/null >>/dev/null" );
       // system( "/bin/rm ./dump/*.rss.* 2>>/dev/null >>/dev/null" );
  
       string tDeleteCommand  = "/bin/rm " + tStationName + " 2>>/dev/null >>/dev/null"; 
       system( tDeleteCommand.c_str() );

       // Download RSS feeds  
       downloadWaterRSSfeeds( stationUrl );  
    }

   if( tStationName.length() != 0 )
       tWaterLevel = readAndParsePerWaterStationResponse( tStationName );
   else printDebugMessages( "Opps! Couldn't fetch station name from RSSURL!" );
     
   // Just assume all is good! 
   return tWaterLevel;
}

//
// Desc: This method does parsing and extraction of demanded attributes 
// Arguments: string, Path to fetched RSS file 
// Returns: string, Water level in fts 
// 

string WaterSensors::readAndParsePerWaterStationResponse( string waterStationRssFeeds){

  int tRssTextLength = 0;
  char * tStringBuffer = NULL;
  string tWaterLevel = "-99";

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
     return tWaterLevel; 
  }

  // Allocate memory according to file length  
  tStringBuffer = new char [ tRssTextLength + 1 ];

  if( tStringBuffer ){
    // Read data as a block:
    fileHandle.read ( tStringBuffer, tRssTextLength );

    if( tRssTextLength )
        tStringBuffer[ tRssTextLength ] = '\0' ;

    printDebugMessages( tStringBuffer );

    // Extract water level in fts 
    string tEntireString = tStringBuffer;  

    if( tEntireString.length() != 0 ) {
        tEntireString =  tEntireString.substr( tEntireString.rfind( ":" ) + 1 ); 
 
        // Replace spaces and 'ft' so that only reading could be fetched
        tEntireString.replace( tEntireString.find( " " ), 2 ,"" );
        tEntireString.replace( tEntireString.find( "ft" ), 2 ,"\0" );
        tWaterLevel = tEntireString;
     }
 
    delete [] tStringBuffer;
    fileHandle.close();
   }
   
   return tWaterLevel; 
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

  waterSensor.printDebugMessages ( "Water level at LakeFront  :" + waterSensor.crawlThroughStationsData( true, __Lakefront ) );
  waterSensor.printDebugMessages ( "Water level at LaBranche  :" + waterSensor.crawlThroughStationsData( true, __LaBranche ) );
  waterSensor.printDebugMessages ( "Water level at ShellBeach :" + waterSensor.crawlThroughStationsData( true, __ShellBeach ) );

  return EXIT_SUCCESS; 
} 

#endif
