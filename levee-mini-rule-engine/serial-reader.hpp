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

#ifndef SERIAL_READER_HPP 
#define SERIAL_READER_HPP

// Include files for string operations and serial communication if any!
#include <string>
#include <SerialStream.h>

using namespace LibSerial;
using namespace std;

//
// Class, Serial communicator. This will read and write data serial data over USB port connected to
// D.A. Hardware
//

class SerialCommunicator {

public: 

      SerialCommunicator( void );             // Initiate serial communicator
      ~SerialCommunicator( void );            // Do some serial cleanup
      void printDebugMessages( string );      // Prints debug messages on standard console
      bool openSerialTerminal( string );      // Open serial communication and return  a handle  
      bool initializeSerialTerminal( void );  // Initialize serial port settings 
      string readFromSerialOverUSB( void );   // Read data from serial port over USB 
      void writeToSerialOverUSB( string );    // Write data to serial port over USB 

private: 
      bool isDebugEnabled;                    // Is debugging enabled?   
      int charBlockOfDataSize;                // Char Block size for reading or writing data over serial 
      SerialStream serialHandle;              // Serial stream handle 
};

#endif

