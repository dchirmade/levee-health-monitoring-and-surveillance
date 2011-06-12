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
// This is a core levee monitoring system base on mini rule engine. This system will fork actions 
// based on rule sequence and out-comes
// 

#ifndef LEVEE_MINI_RULE_ENGINE_HPP
#define LEVEE_MINI_RULE_ENGINE_HPP

// Include files for string and vector operations 
#include <string>
#include <vector>

// Move this include to .cpp file
#include <iostream>

using namespace std;

//
// Class, Mini Rule engine for levee monitoring, This will reposible for building and executing rules
// which will perform desire action(s).  
//

class LeveeMiniRuleEngine {

public:
       // Crude rule attributes  
       struct CrudeRuleBase{
       
        int ruleIndex;            // Rule index number 
        bool isRuleActive;        // If rule is active or inactive   
        string ruleName;          // Human readable rule name 
        string ruleDescription;   // Rule description if any 
        string ruleAction;        // Action attached with rule   
        string rulePayload;       // Any payload params needed by rule 
        int ruleDelay;            // Wait time in rule execution sequence 
        int nextRuleIndex;        // Next rule to be executed 
       }crudeRuleDefinition;   

       // Vectors of Crude rules 
       vector< CrudeRuleBase > vectorCrudeRuleDefinition; 

       LeveeMiniRuleEngine( void );        // Initiate rule engine  
       ~LeveeMiniRuleEngine( void );       // Reset and clean rule enigne  
       void printDebugMessages( string );  // Prints debug messages if enalbed 
       void initializeRuleBase ( void );   // Initializes and build rules     
       void appendNewRule ( int, bool,     // Insert rules to rule engines execution path 
                            string, string, 
                            string, string, 
                            int, int 
                          );
       void executeRuleEngine( void );     // Execute the rules per their definitions 
   
private: 
        // Is debugger enabled?  
        bool isDebugEnabled; 
};

#endif 

