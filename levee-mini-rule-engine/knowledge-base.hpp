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

using namespace std;

//
// Class, KnowledgeBase, This will hold all key attribute values needed by mini rule engine.  
//

class KnowledgeBase{

public: 

       KnowledgeBase( void );              // Initialization procedure 
       ~KnowledgeBase( void );             // Cleanup procedure
       void getADXL335axisPosition(        // Get x,y,z values for ADXL335 
                                       unsigned int &,  
                                       unsigned int &, 
                                       unsigned int & 
                                     ); 

       void setADXL335axisPosition(       // Set x,y,z values at the time of deployment
                                       unsigned int , 
                                       unsigned int , 
                                       unsigned int  
                                     ); 
private:
    
       // Position values of ADXL335 3-axis accelerometer while deploying sensors into levees.
       unsigned int ADXL335Xposition;        
       unsigned int ADXL335Yposition;        
       unsigned int ADXL335Zposition;        
};
#endif 