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
#include <iostream>
#include <unistd.h>
#include <cstdlib>

#include "knowledge-base.hpp"

//
// Desc: This should initialize the knowledge base.    
// Arguments: Nothing, void 
// Returns: Nothing, void 
//
   
KnowledgeBase::KnowledgeBase( void ){

    // Initialize the x,y,z positions while deploying ADXL335 3-axis accelerometer into levees
    setADXL335axisPosition( 10, 20, 30);  
}

//
// Desc: Any cleanup needed by knowledge base
// Arguments: Nothing, void 
// Returns: Nothing, void 
//   
KnowledgeBase::~KnowledgeBase( void ){

}

//
// Desc: This should fetch x, y, z values for ADXL335 sensor    
// Arguments: unisgned int, x, y, z parameters 
// Returns: Nothing, void 
// 
  
void KnowledgeBase::getADXL335axisPosition(
                                               unsigned int &tXposition, 
                                               unsigned int &tYposition, 
                                               unsigned int &tZposition 
                                             ){

     // Set the position accordingly 
     tXposition = ADXL335Xposition;  
     tYposition = ADXL335Yposition;  
     tZposition = ADXL335Zposition;  
     return; 
}

//
// Desc: This should set x, y, z values for ADXL335 sensor while deploying the sensor into levees    
// Arguments: unisgned int, x, y, z parameters 
// Returns: Nothing, void 
// 
  
void KnowledgeBase::setADXL335axisPosition(
                                               unsigned int tXposition = 0, 
                                               unsigned int tYposition = 0, 
                                               unsigned int tZposition = 0 
                                             ){

     // Set the position accordingly 
     ADXL335Xposition = tXposition;  
     ADXL335Yposition = tYposition;  
     ADXL335Zposition = tZposition;  
     return; 
}

using namespace std;

#endif

//
// Desc: Main for knowledge base   
// Arguments: Nothing, void 
// Returns: Nothing, void 
//   

#ifdef KNOWLEDGE_BASE_MAIN 
int main( void ){

    KnowledgeBase knowledge; 
    return EXIT_SUCCESS; 
}
#endif 

