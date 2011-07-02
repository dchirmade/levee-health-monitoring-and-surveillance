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
// This will read or write raw data to or from D.A. Hardware
// 

#ifndef SERIAL_READER_CPP 
#define SERIAL_READER_CPP

// Include file for SerialStream class and stadard library error methods
#include <SerialStream.h>
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <string>
#include <new>

using namespace LibSerial;
using namespace std;

#include "serial-reader.hpp"

//
// Desc: This should initialize serial communicator and few parameters   
// Arguments: Nothing, void 
// Returns: Nothing, void 
//  

SerialCommunicator::SerialCommunicator( void ){
 
   // Enable serial debugger 
   isDebugEnabled = true;

   // Set read/write buffer size to 256  
   charBlockOfDataSize = 256; 
   
}

//
// Desc: Some cleanup part for serial communicator   
// Arguments: Nothing, void 
// Returns: Nothing, void 
//  

SerialCommunicator::~SerialCommunicator( void ){

   // Close serial handle if any! 
   serialHandle.Close(  );
}

//
// Desc: This will open serial terminal 
// Arguments: string, serial port 
// Returns: bool, status if port is opened or not. 
//  

bool SerialCommunicator::openSerialTerminal( string tSerialTerminalHandle ){
 
    // Assume terminal is not opened! 
    bool tIsTerminalOpened = false;  

    // Try opening the serial handle     
    serialHandle.Open( tSerialTerminalHandle );

    if( !serialHandle.good( ) )
         printDebugMessages( "Opps! I'm sorry. Serial port couldn't be opened. :" + tSerialTerminalHandle );
    else tIsTerminalOpened = true; // All is well! 
 
    return tIsTerminalOpened;
}

//
// Desc: This should initialize serial port over USB. 
// Arguments: Nothing, void 
// Returns: Bool, returns status per incoming port settings 
//  

bool SerialCommunicator::initializeSerialTerminal( void ){
  
    // Assume the terminal had some problem while making settings
    bool tIsTerminalSettingsGood = false;

    while( true ){ // Cool down! This is not an infinite while loop 

     // Set the baud rate of the serial port.
     serialHandle.SetBaudRate( SerialStreamBuf::BAUD_9600 ) ;
     if ( ! serialHandle.good() ) 
     {
        printDebugMessages( "Error: Could not set the baud rate." );
        break;
     }

     // Set the number of data bits
     serialHandle.SetCharSize( SerialStreamBuf::CHAR_SIZE_8 ) ;
     if ( ! serialHandle.good( ) ) 
     {
        printDebugMessages( "Error: Could not set the character size." ) ;
        break;
     }
    
     // Disable parity.
     serialHandle.SetParity( SerialStreamBuf::PARITY_NONE ) ;
     if ( ! serialHandle.good() ) 
     {
        printDebugMessages( "Error: Could not disable the parity." );
        break;
     }

     // Set the number of stop bits.
     serialHandle.SetNumOfStopBits( 1 ) ;
     if ( ! serialHandle.good() ) 
     {
        printDebugMessages( "Error: Could not set the number of stop bits." );
        break;
     }

     // Turn off hardware flow control.
     serialHandle.SetFlowControl( SerialStreamBuf::FLOW_CONTROL_NONE ) ;
     if ( ! serialHandle.good() ) 
     {
        printDebugMessages( "Error: Could not use hardware flow control." );
        break;
     } 

       // All is well! 
       tIsTerminalSettingsGood = true;
       break;
    }
 
    return tIsTerminalSettingsGood; 
}

//
// Desc: This should print debug messages only if it is enabled.   
// Arguments: String, Lines which needs to be printed on standard console. 
// Returns: Nothing, void 
//   
 
void SerialCommunicator::printDebugMessages( string debugLines ){

   // Print debug messages to standard console. 
   if( isDebugEnabled )
       cout << debugLines << endl; 

   return; 
}

//
// Desc: This should fetch data from serial port in blocks.   
// Arguments: Nothing, void 
// Returns: String, Fetched data from serial handle. 
//  

string SerialCommunicator::readFromSerialOverUSB( void ){
    
   string stringBuffer = "\0";
   string tStopTag = "</hardwaresensor>"; 

   // Wait for some data to be available at the serial port.
   while( serialHandle.rdbuf()->in_avail() == 0 ) 
          usleep(100) ;

   // Read the data char by char! 
   while( true ) 
    {
        char next_byte;
        serialHandle.get(next_byte);
        stringBuffer += next_byte;
        if( stringBuffer.size() > tStopTag.size() )
        if( stringBuffer.compare( stringBuffer.size() - tStopTag.size(), tStopTag.size(), tStopTag )== 0 )
           break;
        usleep(100) ;
    } 
   return stringBuffer;
} 

//
// Desc: This should write data to serial port in blocks.   
// Arguments: String, Data that need to write on serial port
// Returns: Nothing, void 
//  

void SerialCommunicator::writeToSerialOverUSB( string tWriteDataBuffer ){
   
   serialHandle << tWriteDataBuffer.c_str();
} 

#endif

//
// Desc: Main for this code base.   
// Arguments: Nothing, void
// Returns: int, Program exit status. 
//   
 
#ifdef SERIAL_MAIN 
int main( void ){

   SerialCommunicator serialChatTerminal;

   // Open Serial terminal and initialize the same
   // Fixme: Remove hardcoded terminal path
   if( serialChatTerminal.openSerialTerminal( "/dev/ttyUSB0" ) ){
    
    if( serialChatTerminal.initializeSerialTerminal( ) ) // Start reading some data over serial 
         serialChatTerminal.printDebugMessages( serialChatTerminal.readFromSerialOverUSB( ));
    else serialChatTerminal.printDebugMessages( "Some is wrong with serial port settings!" );
   } 
   else serialChatTerminal.printDebugMessages( "Well. It looks like something is wrong with serial port!" ); 

   return EXIT_SUCCESS; 
}
#endif 
