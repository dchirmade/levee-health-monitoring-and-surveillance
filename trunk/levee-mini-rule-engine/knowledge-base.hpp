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

#ifndef KNOWLEDGE_BASE_HPP
#define KNOWLEDGE_BASE_HPP

// Include files for string and io operations 
#include <string>
#include <iostream>
#include <vector>

using namespace std;

//
// Class, KnowledgeBase, This will hold all key attribute values needed by mini rule engine.  
//

class KnowledgeBase{

public: 

       KnowledgeBase( void );                  // Initialization procedure 
       ~KnowledgeBase( void );                 // Cleanup procedure
       void printDebugMessages( string );      // Prints debug messages 
       void printAllKeyValuePair( void );      // Prints all configuration key values 
       bool validateAllKeyValuePair( void );   // Validates all configuration key values 
       string getValueOfaKey( string );        // Get a value of a configuration key
       void setValueOfaKey( string , string ); // Assigns new value to given configuration key

private:

       // Debug mode 
       bool isDebugEnabled;    
 
       // Key-Value holder
       struct knowledgeKeyValues{
           string key; 
           string value;  
       }knowledgeKeyValueIndex;

       // Key-value vector
       vector< knowledgeKeyValues > vectorKnowledgeKeyValueIndex; 
 
};
#endif 
