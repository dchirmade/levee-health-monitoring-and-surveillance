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
// This is a core levee monitoring system base on mini rule engine. This system will fork actions 
// based on rule sequence and out-comes
// 

#ifndef LEVEE_MINI_RULE_ENGINE_CPP
#define LEVEE_MINI_RULE_ENGINE_CPP

// Include files for string and vector operations 
#include <string>
#include <vector>
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <time.h>

#include "mini-rule-engine.hpp"
#include "noaa-software-weather-sensors.hpp"
#include "serial-reader.hpp"
#include "knowledge-base.hpp"

// Define rule actions 
#define __nothing 		     0 
#define __ADXL335HardwareSensorHook  1 
#define __NOAASoftwareSensorHook     2 

using namespace std;

//
// Desc: This should initialize and execute mini rule engine.     
// Arguments: Nothing, void 
// Returns: Nothing, void 
//   

LeveeMiniRuleEngine::LeveeMiniRuleEngine( void ){

    // Do initialization neeed for weather sensor 
    isDebugEnabled = true;

    // Initialize the rule base 
    initializeRuleBase();
     
    // Execute the Rule Engine 
    executeRuleEngine(); 
}

//
// Desc: This should reset mini rule engine   
// Arguments: Nothing, void 
// Returns: Nothing, void 
//  

LeveeMiniRuleEngine::~LeveeMiniRuleEngine( void ){

    return;
}

//
// Desc: This should print debug messages only if it is enabled.   
// Arguments: String, Lines which needs to be printed on standard console. 
// Returns: Nothing, void 
//   
 
void LeveeMiniRuleEngine::printDebugMessages( string debugLines ){

   // Get current time and stamp it with logs 
   time_t tRawTime;
   time ( &tRawTime );
 
   string tLocaltime = asctime( localtime( &tRawTime ) );
   tLocaltime.replace( tLocaltime.find( "\n" ), 1 ,"\0" ); // Replace \n at the end of time string
  
   // Print debug messages to standard console. 
   if( isDebugEnabled )
       cout << tLocaltime << " : " << debugLines << endl; 

   return; 
}

//
// Desc: This method will add new crude rule into rule base. 
// Arguments: Rule index,  This is a main rule index and is unique. 
//            Is rule active, Decides if rule is need to be executed or what.  
//            Rule name, Human readable rule name.  
//            Rule description, Rule description.   
//            Rule Action, Action to be performed per defined by Rule 
//            Rule payload, Payload or excess of arguments needed by Rule 
//            Rule delay, Delay time in between two rules  
//            Next rule index, Index to next rule to be executed. 
// Returns: Nothing, void 
//  

void LeveeMiniRuleEngine::appendNewRule( 
                                        int tRuleIndex, 
                                        bool tIsRuleActive,
                                        string tRuleName, 
                                        string tRuleDescription, 
                                        unsigned int tRuleAction, 
                                        string tRulePayLoad,
                                        int tRuleDelay, 
                                        int tNextRuleIndex 
                                       ){
  
    // Build Crude rule  
    crudeRuleDefinition.ruleIndex = tRuleIndex; 
    crudeRuleDefinition.isRuleActive = tIsRuleActive; 
    crudeRuleDefinition.ruleName = tRuleName; 
    crudeRuleDefinition.ruleDescription = tRuleDescription; 
    crudeRuleDefinition.ruleAction = tRuleAction; 
    crudeRuleDefinition.rulePayload = tRulePayLoad; 
    crudeRuleDefinition.ruleDelay = tRuleDelay; 
    crudeRuleDefinition.nextRuleIndex = tNextRuleIndex; 
   
    // Append the rule into rule vector  
    vectorCrudeRuleDefinition.push_back( crudeRuleDefinition );

    printDebugMessages ("Appended new rule: " + tRuleName);
    return;   
}

//
// Desc: This method will initialize rule base by building new rules per proposed rule-sequence 
// Arguments: Nothing, void  
// Returns: Nothing, void 
//  

void LeveeMiniRuleEngine::initializeRuleBase( void ){
   
    // Start point 
    // Rule to get start! 
    appendNewRule(
                  vectorCrudeRuleDefinition.size(), // Indix is auto generated. Please do not modify. 
                  true,
                  "Start Point!",
                  "This is a start point for rule executer.",
                  __nothing,
                  "nothing",
                  1, 
                  vectorCrudeRuleDefinition.size() + 1  // Jump index is auto generated. Please do not modify. 
                 );
      
    // Always add rules in between start and end points to avoid infinite rule dead lock 

    // Step 1. Try to fetch events from ADXL335 hardware sensor  
    appendNewRule(
                  vectorCrudeRuleDefinition.size(), // Indix is auto generated. Please do not modify. 
                  true,
                  "ADXL335",
                  "This will hook-up ADXL335 sensor to rule engine.",
                  __ADXL335HardwareSensorHook,
                  "nothing",
                  1, 
                  vectorCrudeRuleDefinition.size() + 1  // Jump index is auto generated. Please do not modify. 
                 );
  
    // End point

    // Step 2. Try to fetch raw data from weather station (software sensors)
    appendNewRule(
                  vectorCrudeRuleDefinition.size(), // Indix is auto generated. Please do not modify. 
                  true,
                  "NOAA Weather feeds",
                  "This will hook-up NOAA Weather feeds (kind of software sensor!) to rule engine.",
                  __NOAASoftwareSensorHook,
                  "nothing",
                  1, 
                  vectorCrudeRuleDefinition.size() + 1  // Jump index is auto generated. Please do not modify. 
                 );
  

    // Rule to either stop or loop!     
    appendNewRule(
                  vectorCrudeRuleDefinition.size(),
                  true,
                  "End Point!",
                  "This is a end point for rule executer.",
                  __nothing,
                  "nothing",
                  1, 
                  0  // This index always be 0 to form a loop. Please do not modify.         
                 );
    return; 
}

//
// Desc: This will execute rules infinity per provided sequence. 
// Arguments: Nothing, void  
// Returns: Nothing, void 
//  

void LeveeMiniRuleEngine::executeRuleEngine( void ){

    // Initial rule index of rule executer is always 0 unless for some custom case
    int tCurrentRuleIndex = 0; 

    // Rule engine should run infinitly 
    while( true ){

     printDebugMessages( "Executing current rule : " + vectorCrudeRuleDefinition[tCurrentRuleIndex].ruleName );       
     printDebugMessages( 
                         "Next rule in queue     : " + 
                         vectorCrudeRuleDefinition[vectorCrudeRuleDefinition[tCurrentRuleIndex].nextRuleIndex].ruleName 
                       );       
  
      // Decide what action to be taken!
      switch( vectorCrudeRuleDefinition[tCurrentRuleIndex].ruleAction ){
 
      case __nothing:
              // Execute action attached to nothing!  
              voidAction( vectorCrudeRuleDefinition[tCurrentRuleIndex].rulePayload );
              break;            
      case __ADXL335HardwareSensorHook:
              // Execute action attached to ADXL335HardwareSensorHook!  
              hookupADXL335Sensor( vectorCrudeRuleDefinition[tCurrentRuleIndex].rulePayload );
              break;            
      case __NOAASoftwareSensorHook:
              // Execute action attached to NOAASoftwareSensorHook!  
              hookupNOAAWeatherSensor( vectorCrudeRuleDefinition[tCurrentRuleIndex].rulePayload );
              break;            
      default: 
              printDebugMessages( "No valid action assigned. Sliding to next rule by skipping this one!" );
              break;
      } 
   
      // Wait for some time per defined in rule
      sleep( vectorCrudeRuleDefinition[tCurrentRuleIndex].ruleDelay ); 

      // Jump to next rule in sequence per defined in rule definition. 
      if( 
          vectorCrudeRuleDefinition[tCurrentRuleIndex].nextRuleIndex >= 0 && 
          vectorCrudeRuleDefinition[tCurrentRuleIndex].nextRuleIndex <= ( int signed ) vectorCrudeRuleDefinition.size()
        )
          tCurrentRuleIndex = vectorCrudeRuleDefinition[tCurrentRuleIndex].nextRuleIndex; 
      else tCurrentRuleIndex = 0; // For any error in indexing, fallback to start point
    }
  
    return; 
}

//
// Desc: Rule Action: voidAction, actually it does nothing! 
// Arguments: string, Payload to action if any!  
// Returns: Nothing, void 
//  

void LeveeMiniRuleEngine::voidAction( string tPayLoad ){
 
    printDebugMessages( "Executing void action with payload: " + tPayLoad );
    return;  
} 


//
// Desc: NOAA Weather sensor's execution procedure.   
// Arguments: string, Payload to action if any!  
// Returns: Nothing, void 
//  

void LeveeMiniRuleEngine::hookupNOAAWeatherSensor( string tPayLoad ){
 
    printDebugMessages( "Executing 'NOAA Weather sensor' action with payload: " + tPayLoad );

    string weatherFeedsIndexFile = "./dump/MAIN.LIST"; // Fixme: Currently hardcoded 

    // Parse and build weather feeds base into vectors 
    WeatherSensors NOAAWeatherFeeds;
    if( NOAAWeatherFeeds.readAndParseWeatherFeeds( weatherFeedsIndexFile ) ){


     // Get per station data
     if( NOAAWeatherFeeds.crawlThroughStationsData( false ) == true ){
        // WIP! Do some actions if needed
     }

     // Print all stations attributes. Just for testing!  
     NOAAWeatherFeeds.printAllStationsData();

     // Print all stations located in New Orleans
     NOAAWeatherFeeds.printAllStationsDataPerLocation("New Orleans");
    }
    return;  
} 

//
// Desc: ADXL335 sensor's execution procedure.   
// Arguments: string, Payload to action if any!  
// Returns: Nothing, void 
//  

void LeveeMiniRuleEngine::hookupADXL335Sensor( string tPayLoad ){
 
    printDebugMessages( "Executing 'ADXL335' sensor action with payload: " + tPayLoad );

    // All right, start operating hardware sensors here
    // Fixme! Perform operating on knowledge-base here
    
    // Fetch events from hardware sensors over serial terminal 
    SerialCommunicator serialChatTerminal;

    // Open Serial terminal and initialize the same
    // Fixme: Remove hardcoded terminal path
    if( serialChatTerminal.openSerialTerminal( "/dev/ttyUSB0" ) ){

    if( serialChatTerminal.initializeSerialTerminal( ) ){ // Start reading some data over serial 

         // Ask for the data first then read
         serialChatTerminal.writeToSerialOverUSB( "1" );  
         serialChatTerminal.printDebugMessages( serialChatTerminal.readFromSerialOverUSB( ));
    }
    else serialChatTerminal.printDebugMessages( "Some is wrong with serial port settings!" );
    }
    else serialChatTerminal.printDebugMessages( "Well. It looks like something is wrong with serial port!" ); 
    
    return;  
} 

#endif

//
// Desc: Main for this code base.   
// Arguments: Nothing, void
// Returns: int, Program exit status. 
//   
 
#ifdef MINI_RULE_ENGINE_MAIN 
int main( void ){

   LeveeMiniRuleEngine RuleEgnine; 
   return EXIT_SUCCESS;
}
#endif 

