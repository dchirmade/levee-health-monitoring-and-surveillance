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
//
// This is knowledge base for mini-rule-engine. Currently this knowledge base has hardwared value 
// or value read from plain text file. In future this could be replaced my automated expert system 
// which could learn and build knowledge base on the fly!
// 


#ifndef KNOWLEDGE_BASE_CPP
#define KNOWLEDGE_BASE_CPP

// Include files for string and io operations 
#include <string>
#include <iostream>
#include <unistd.h>
#include <cstdlib>

#include "knowledge-base.hpp"
#include "configuration-file.hpp"

using namespace std;

//
// Desc: This should initialize the knowledge base.    
// Arguments: Nothing, void 
// Returns: Nothing, void 
//
   
KnowledgeBase::KnowledgeBase( void ){
 
     // Enable debug  mode 
     isDebugEnabled = true;  
  
     // Read the confiuration file and intialize static knowledge base accordingly 
     ConfigurationFile configurationFile;

    if( !configurationFile.readConfigurationFile( "configuration.conf" ) ){
        
        printDebugMessages( "Opps! There is an error while reading configuration file." );
    }
    else{

         // Form a vector of configuration key-values pair
         knowledgeKeyValueIndex.key = "ADXL335-X-position"; 
         knowledgeKeyValueIndex.value = configurationFile.getValueOfGivenKey( knowledgeKeyValueIndex.key ); 
         vectorKnowledgeKeyValueIndex.push_back( knowledgeKeyValueIndex );
         knowledgeKeyValueIndex.key = "ADXL335-Y-position"; 
         knowledgeKeyValueIndex.value = configurationFile.getValueOfGivenKey( knowledgeKeyValueIndex.key ); 
         vectorKnowledgeKeyValueIndex.push_back( knowledgeKeyValueIndex );
         knowledgeKeyValueIndex.key = "ADXL335-Z-position"; 
         knowledgeKeyValueIndex.value = configurationFile.getValueOfGivenKey( knowledgeKeyValueIndex.key ); 
         knowledgeKeyValueIndex.key = "ADXL335-serial-port"; 
         knowledgeKeyValueIndex.value = configurationFile.getValueOfGivenKey( knowledgeKeyValueIndex.key ); 
         vectorKnowledgeKeyValueIndex.push_back( knowledgeKeyValueIndex );
         knowledgeKeyValueIndex.key = "ADXL335-alerting-trigger"; 
         knowledgeKeyValueIndex.value = configurationFile.getValueOfGivenKey( knowledgeKeyValueIndex.key ); 
         vectorKnowledgeKeyValueIndex.push_back( knowledgeKeyValueIndex );
         knowledgeKeyValueIndex.key = "noaa-alerting-water-level"; 
         knowledgeKeyValueIndex.value = configurationFile.getValueOfGivenKey( knowledgeKeyValueIndex.key ); 
         vectorKnowledgeKeyValueIndex.push_back( knowledgeKeyValueIndex );
         knowledgeKeyValueIndex.key = "noaa-alerting-weather-type"; 
         knowledgeKeyValueIndex.value = configurationFile.getValueOfGivenKey( knowledgeKeyValueIndex.key ); 
         vectorKnowledgeKeyValueIndex.push_back( knowledgeKeyValueIndex );
         knowledgeKeyValueIndex.key = "noaa-alerting-weather-location"; 
         knowledgeKeyValueIndex.value = configurationFile.getValueOfGivenKey( knowledgeKeyValueIndex.key ); 
         vectorKnowledgeKeyValueIndex.push_back( knowledgeKeyValueIndex );
         knowledgeKeyValueIndex.key = "notification-email-addresses"; 
         knowledgeKeyValueIndex.value = configurationFile.getValueOfGivenKey( knowledgeKeyValueIndex.key ); 
         vectorKnowledgeKeyValueIndex.push_back( knowledgeKeyValueIndex );
 
         // Configurations flags needed by mini rule engine [ these are not taken from configuraton file ] 
         knowledgeKeyValueIndex.key = "noaa-water-alert-email-sent"; 
         knowledgeKeyValueIndex.value = "no"; 
         vectorKnowledgeKeyValueIndex.push_back( knowledgeKeyValueIndex );
         knowledgeKeyValueIndex.key = "noaa-weather-alert-email-sent"; 
         knowledgeKeyValueIndex.value = "no"; 
         vectorKnowledgeKeyValueIndex.push_back( knowledgeKeyValueIndex );
         knowledgeKeyValueIndex.key = "noaa-ADXL335-alert-email-sent"; 
         knowledgeKeyValueIndex.value = "no"; 
         vectorKnowledgeKeyValueIndex.push_back( knowledgeKeyValueIndex );
    }


         // Validate call! 
         if( validateAllKeyValuePair() == false ){
 
            // Do not use printDebugMessages over here
            cout << endl << "Validation error! Please check the configuration file. An empty fields/values are not allowed." << endl; 
           
            exit( 1 );
         }
}

//
// Desc: Any cleanup needed by knowledge base
// Arguments: Nothing, void 
// Returns: Nothing, void 
//   

KnowledgeBase::~KnowledgeBase( void ){

}

//
// Desc: Assigns new value of a given configuration key
// Arguments: string, Key of which value need to be set
//            string, New value that need to be assigned
// Returns: void, Nothing 
//
   
void KnowledgeBase::setValueOfaKey( string key, string newValue ){

   
     for( int keyList = 0 ; keyList < (int) vectorKnowledgeKeyValueIndex.size() ; keyList++ ){
      
      // For multiple entries of a same key, only last occurance of key will be considered. 
      if( vectorKnowledgeKeyValueIndex[keyList].key.find( key ) != string::npos )
          vectorKnowledgeKeyValueIndex[keyList].value = newValue;    
      else continue;    
      }

     return;
}

//
// Desc: Fetches value of a given  configuration key
// Arguments: string, Key of which value need to be fetched  
// Returns: string, returns R value of a key  
//
   
string KnowledgeBase::getValueOfaKey( string key ){

     string tValue = "";
   
     for( int keyList = 0 ; keyList < (int) vectorKnowledgeKeyValueIndex.size() ; keyList++ ){
      
      // For multiple entries of a same key, only last occurance of key will be considered. 
      if( vectorKnowledgeKeyValueIndex[keyList].key.find( key ) != string::npos )
          tValue = vectorKnowledgeKeyValueIndex[keyList].value;    
      else continue;    
      }

     return tValue;
}


//
// Desc: This will do a quick validation if any field remains void in configuration pairs  
// Arguments: Nothing, void 
// Returns: Bool, true if all is well other false
//

bool KnowledgeBase::validateAllKeyValuePair( void ){

   bool tReturn = true; 

   for( int keyList = 0 ; keyList < (int) vectorKnowledgeKeyValueIndex.size() ; keyList++ ){

      string tValue = vectorKnowledgeKeyValueIndex[keyList].value;

      if( tValue.empty() )
          tReturn = false;
   }

   return tReturn; 
}

//
// Desc: This should print all key value pair to console
// Arguments: Nothing, void 
// Returns: Nothing, void
//

void KnowledgeBase::printAllKeyValuePair( void ){

   for( int keyList = 0 ; keyList < (int) vectorKnowledgeKeyValueIndex.size() ; keyList++ ){

      printDebugMessages( 
                         "Configuration Key \"" + vectorKnowledgeKeyValueIndex[keyList].key + "\" = " +
                         vectorKnowledgeKeyValueIndex[keyList].value
                        );
   }

   return; 
}

//
// Desc: This should print debug messages only if it is enabled.
// Arguments: String, Lines which needs to be printed on standard console.
// Returns: Nothing, void
//

void KnowledgeBase::printDebugMessages(
                                       string debugLines
                                     ){

   // Get current time and stamp it with logs
   time_t tRawTime;
   time ( &tRawTime );

   string tLocaltime = asctime( localtime( &tRawTime ) );
   tLocaltime.replace( tLocaltime.find( "\n" ), 1 ,"\0" ); // Replace \n at the end of time string


   // Print debug messages to standard console.
   if( isDebugEnabled )
       cout << "<br>" << tLocaltime << " :" << debugLines << endl;

   return;
}


#endif

//
// Desc: Main for knowledge base   
// Arguments: Nothing, void 
// Returns: Nothing, void 
//   

#ifdef KNOWLEDGE_BASE_MAIN 
int main( void ){

    KnowledgeBase knowledge; 
    knowledge.printAllKeyValuePair();    

    return EXIT_SUCCESS; 
}
#endif 

