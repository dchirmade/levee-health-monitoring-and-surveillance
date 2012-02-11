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
// This will read or write raw data to or from D.A. Hardware
//

#ifndef SERIAL_READER_HPP 
#define SERIAL_READER_HPP

// Include files for XERCESC DOM parser and library 
#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMDocumentType.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMImplementationLS.hpp>
#include <xercesc/dom/DOMNodeIterator.hpp>
#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/dom/DOMText.hpp>

#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/util/XMLUni.hpp>
#include <xercesc/util/XMemory.hpp>

// Include files for string operations and serial communication if any!
#include <string>
#include <SerialStream.h>
#include <stdexcept>

using namespace LibSerial;
using namespace std;
using namespace xercesc;

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
      bool readAndParseDAFeeds( string );     // Parses DA XMl response 
      string GetTextContentOfAnElement( DOMElement*, string ); // Extracts the text content of an elements 
      bool checkIfChangeInReadingDetected( string, string, string, string ); // Checks if there is any change in XYZ reading 
      string getParsedDAResponse( void );     // Gets parsed DA string dump

private: 

      bool isDebugEnabled;                    // Is debugging enabled?   
      int charBlockOfDataSize;                // Char Block size for reading or writing data over serial 
      SerialStream serialHandle;              // Serial stream handle 
      
      // Handle for Xerces DOM Parser 
      xercesc::XercesDOMParser *daResponseIndexParser; 

      // DA Event response structure 
      struct daEventResponse{
         
        string sensorType;                    // Type of sensor : digital or analog
        string sensorDescription;             // Sensor's descriptions 
        string sensorFunction;                // Sensor's fuction 
        string sensorStatus;                  // Current sensor's status : active or in-active 
        string sensorEvent;                   // Kind of an event : changed or unchanged 
        string sensorEventType;               // Event Type :  Compulsory or Optional  
        string sensorReadingX;                // X-axis reading 
        string sensorReadingY;                // X-axis reading 
        string sensorReadingZ;                // Z-axis reading
        string sensorTimeStamp;               // Time stamp when this event has generated  
        string sensorMountPorts;              // DA ports on which sensor has been mounted     
 
      }daEventResponseIndex; 

};

#endif

