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
// This will take configurations from file  
//

#ifndef CONFIGURATION_FILE_CPP
#define CONFIGURATION_FILE_CPP

// Include files for string and io operations 
#include <string>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <cstdlib>
#include <sstream>

#include "configuration-file.hpp"

using namespace std;

//
// Desc: Initializes few variables  
// Arguments: Nothing, void  
// Returns: Nothing, void 
//  

ConfigurationFile::ConfigurationFile( void ){

    // Do initialization if any!
    isDebugEnabled = true;
    rawConfigurationFileBuffer.empty();     
}

//
// Desc: Reset if any. Not in use as of now!
// Arguments: Nothing, void  
// Returns: Nothing, void 
//  

ConfigurationFile::~ConfigurationFile( void ){

}

//
// Desc: Extracts values of valid given key 
// Arguments: String, Key of which value need to be extracted.
// Returns: String, Value of a valid given key 
//  

string ConfigurationFile::getValueOfGivenKey( string key ){
   
     string tValueString = ""; 
     string tToken = "";
      
     // Process raw configuration file buffer to pullout key-values. 
     istringstream iss( rawConfigurationFileBuffer );    
     while( getline( iss, tToken, '\n' ) )
     {
       // Find the key and get the value 
       if( tToken.rfind( key ) != string::npos ) {
           
          tValueString = tToken.substr( tToken.find_last_of( ":=" ) + 1 ); 
       }
     }

     return tValueString;  
}

//
// Desc: Reads configurations into raw string buffer file
// Arguments: String, configuration file name.
// Returns: bool, True if all is good otherwise false
//  

bool ConfigurationFile::readConfigurationFile( string fileName = "configuration.conf" ){
 
      bool tReturn = false;
      string tLineStringBuffer = "";  

      // Open the configuration file and do reading 
      ifstream fileHandle ( fileName.c_str() );
      if( fileHandle.is_open() )
      {
       
        // Flush the raw buffer
        rawConfigurationFileBuffer.empty(); 

        while ( fileHandle.good() )
       {
         getline ( fileHandle, tLineStringBuffer );
         rawConfigurationFileBuffer += tLineStringBuffer + "\n";
         tReturn = true; 
       }
         fileHandle.close();
      }
      else{
           printDebugMessages( "Opps! Unable to open configuration file: " + fileName );
           tReturn = false;    
      } 
      
      return tReturn;       
}

//
// Desc: This should print debug messages only if it is enabled.   
// Arguments: String, Lines which needs to be printed on standard console. 
// Returns: Nothing, void 
//   
 
void ConfigurationFile::printDebugMessages( string debugLines ){

   // Get current time and stamp it with logs 
   time_t tRawTime;
   time ( &tRawTime );
 
   string tLocaltime = asctime( localtime( &tRawTime ) );
   tLocaltime.replace( tLocaltime.find( "\n" ), 1 ,"\0" ); // Replace \n at the end of time string
  
   // Print debug messages to standard console. 
   if( isDebugEnabled )
       cout << "<br>" << tLocaltime << " : " << debugLines << endl; 

   return; 
}

#endif

//
// Desc: Main for configuration file   
// Arguments: Nothing, void 
// Returns: Nothing, void 
//   

#ifdef CONFIGURATION_FILE_MAIN 
int main( void ){

    ConfigurationFile configurationFile;  

    if( !configurationFile.readConfigurationFile( "configuration.conf" ) ) 
        return EXIT_FAILURE;
    else{ 
         // Get default key value configuration pair...
         configurationFile.printDebugMessages( 
                                               "Value for Key 'ADXL335-X-position' is " + 
                                               configurationFile.getValueOfGivenKey( "ADXL335-X-position" ) 
                                             );
         configurationFile.printDebugMessages( 
                                               "Value for Key 'ADXL335-Y-position' is " + 
                                               configurationFile.getValueOfGivenKey( "ADXL335-Y-position" ) 
                                             );
         configurationFile.printDebugMessages( 
                                               "Value for Key 'ADXL335-Z-position' is " + 
                                               configurationFile.getValueOfGivenKey( "ADXL335-Z-position" ) 
                                             );
         configurationFile.printDebugMessages( 
                                               "Value for Key 'ADXL335-alerting-trigger' is " + 
                                               configurationFile.getValueOfGivenKey( "ADXL335-alerting-trigger" ) 
                                             );
         configurationFile.printDebugMessages( 
                                               "Value for Key 'ADXL335-serial-port' is " + 
                                               configurationFile.getValueOfGivenKey( "ADXL335-serial-port" ) 
                                             );
         configurationFile.printDebugMessages( 
                                               "Value for Key 'nooa-alerting-water-level' is " + 
                                               configurationFile.getValueOfGivenKey( "noaa-alerting-water-level" ) 
                                             );
         configurationFile.printDebugMessages( 
                                               "Value for Key 'noaa-alerting-weather-type' is " + 
                                               configurationFile.getValueOfGivenKey( "noaa-alerting-weather-type" ) 
                                             );
         configurationFile.printDebugMessages( 
                                               "Value for Key 'noaa-alerting-weather-location' is " + 
                                               configurationFile.getValueOfGivenKey( "noaa-alerting-weather-location" ) 
                                             );
         configurationFile.printDebugMessages( 
                                               "Value for Key 'notification-email-addresses' is " + 
                                               configurationFile.getValueOfGivenKey( "notification-email-addresses" ) 
                                             );
        
    }

    return EXIT_SUCCESS; 
}
#endif 

