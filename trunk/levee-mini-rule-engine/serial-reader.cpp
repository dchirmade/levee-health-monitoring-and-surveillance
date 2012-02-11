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

#include "serial-reader.hpp"

using namespace LibSerial;
using namespace std;
using namespace xercesc;

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

   // Initialize Xerces libraries  
   try{
    
       XMLPlatformUtils::Initialize();
   } 
   catch( XMLException & error ){ // Throw and exception if any 
       
       char * errorMessage = XMLString::transcode( error.getMessage() ); // Get an error message  
       printDebugMessages( errorMessage );  
       XMLString::release( &errorMessage ); 
   }

   // Assign new instance of DOM parser 
   daResponseIndexParser = new XercesDOMParser; 
   
}

//
// Desc: Some cleanup part for serial communicator   
// Arguments: Nothing, void 
// Returns: Nothing, void 
//  

SerialCommunicator::~SerialCommunicator( void ){

   // Close serial handle if any! 
   serialHandle.Close(  );

   // Free the XML DOM instance before terminating XMLPlatformUtils 
   if( daResponseIndexParser ) 
     delete daResponseIndexParser;

   // Terminate and release any allocated memories and libraries 
   try{

       XMLPlatformUtils::Terminate(); 
   }
   catch( xercesc::XMLException & error ){
       char * errorMessage = XMLString::transcode( error.getMessage() ); // Get an error message  
       printDebugMessages( errorMessage );  
       XMLString::release( &errorMessage );
   } 
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

//
// Desc: This should extract the text content of DOM element.   
// Arguments: DOMElement, DOM element of an XML tree to which text content to be fetched. 
//            String, Name of a tag in XML tree
// Returns: String, Returns extracted text content of a desire DOM element
//   
 
string SerialCommunicator::GetTextContentOfAnElement ( 
                       DOMElement* tCurrentElementLevel2,
                       string tagName 
                       ){

    string tStringAttribute = ""; 
    try{
 
    // Find text content of a given tag 
    XMLCh* tTAG = XMLString::transcode( tagName.c_str() );
    if( XMLString::equals( tCurrentElementLevel2->getTagName(), tTAG) )
    {
     
      // Fetch the parameters at Level 2 children than parents  
      char *tCharAttribute = NULL;
      const XMLCh* tCTAG = NULL;
      tCTAG = tCurrentElementLevel2->getTextContent(); 
      tCharAttribute = XMLString::transcode(tCTAG);
                       
      if( tCharAttribute != NULL )
          tStringAttribute.assign( tCharAttribute );
      if( tCharAttribute ) XMLString::release( &tCharAttribute );
     
     }
     else tStringAttribute.assign( "_NO_TAG_FOUND_" );
     
     XMLString::release( &tTAG );
     }
     catch( ... )
     {
       printDebugMessages( "Exception encountered in getting text content." ); 
     }

     return tStringAttribute;  
}

//
// Desc: Checks if there is any change in x,y,z reading
// Arguments: String, String, String, String,  XYZ reading and alerting trigger  
// Returns: bool, true if change detected else false

bool SerialCommunicator::checkIfChangeInReadingDetected( 
                                                         string tXReading, 
                                                         string tYReading, 
                                                         string tZReading, 
                                                         string tAlertingTrigger 
                                                       ){
 
     bool tReturn = false;

     // Check if there is any change in readings 
     if(  (
          ( ::strtod( tXReading.c_str(), 0 ) - ::strtod( daEventResponseIndex.sensorReadingX.c_str(), 0 ) ) > ::strtod( tAlertingTrigger.c_str(), 0 ) || 
          ( ::strtod( tXReading.c_str(), 0 ) - ::strtod( daEventResponseIndex.sensorReadingX.c_str(), 0 ) ) < ( ::strtod( tAlertingTrigger.c_str(), 0 ) * -1) ) || (
          ( ::strtod( tYReading.c_str(), 0 ) - ::strtod( daEventResponseIndex.sensorReadingY.c_str(), 0 ) ) > ::strtod( tAlertingTrigger.c_str(), 0 ) || 
          ( ::strtod( tYReading.c_str(), 0 ) - ::strtod( daEventResponseIndex.sensorReadingY.c_str(), 0 ) ) < ( ::strtod( tAlertingTrigger.c_str(), 0 ) * -1) ) || (
          ( ::strtod( tZReading.c_str(), 0 ) - ::strtod( daEventResponseIndex.sensorReadingZ.c_str(), 0 ) ) > ::strtod( tAlertingTrigger.c_str(), 0 ) || 
          ( ::strtod( tZReading.c_str(), 0 ) - ::strtod( daEventResponseIndex.sensorReadingZ.c_str(), 0 ) ) < ( ::strtod( tAlertingTrigger.c_str(), 0 ) * -1) )
       ){
          printDebugMessages( "Change in reading of X or Y or Z axis is detected!" );
          tReturn = true;
     } 
  
     return tReturn;
}

//
// Desc: Get current status of all sensors parameters 
// Arguments: void, nothing  
// Returns: string, Current sensor status string dump 
//   

string SerialCommunicator::getParsedDAResponse( void ){

    string tParsedDAResponse = ""; 
    tParsedDAResponse += "\nSensor Type        : " + daEventResponseIndex.sensorType; 
    tParsedDAResponse += "\nSensor Description : " + daEventResponseIndex.sensorDescription; 
    tParsedDAResponse += "\nSensor Function    : " + daEventResponseIndex.sensorFunction; 
    tParsedDAResponse += "\nSensor Status      : " + daEventResponseIndex.sensorStatus; 
    tParsedDAResponse += "\nSensor Event       : " + daEventResponseIndex.sensorEvent; 
    tParsedDAResponse += "\nSensor Event Type  : " + daEventResponseIndex.sensorEventType; 
    tParsedDAResponse += "\nSensor Reading X   : " + daEventResponseIndex.sensorReadingX; 
    tParsedDAResponse += "\nSensor Reading Y   : " + daEventResponseIndex.sensorReadingY; 
    tParsedDAResponse += "\nSensor Reading Z   : " + daEventResponseIndex.sensorReadingZ; 
    tParsedDAResponse += "\nSensor Time Stamp  : " + daEventResponseIndex.sensorTimeStamp; 
    tParsedDAResponse += "\nSensor Mount Ports : " + daEventResponseIndex.sensorMountPorts; 

    return tParsedDAResponse;
}

//
// Desc: This should read and parse DA response.    
// Arguments: String, DA response 
// Returns: Bool, true if all is good otherwise false
//   
 
bool SerialCommunicator::readAndParseDAFeeds( string tDAFeeds ){
 
   bool tReturn = false;
   string daXmlFile = "/tmp/daXmlFile.xml";
   
   if( tDAFeeds.empty() ) 
       return tReturn; 

   // Redirect the response to file
   string tRedirectDAString = "echo '" + tDAFeeds + "' >" + daXmlFile;
   system( tRedirectDAString.c_str() );

   // Configure the DOM parser 

   daResponseIndexParser->setValidationScheme( XercesDOMParser::Val_Never );
   daResponseIndexParser->setDoNamespaces( false );
   daResponseIndexParser->setDoSchema( false );
   daResponseIndexParser->setLoadExternalDTD( false );

   try
   {
      // Parse it from memory rather than file  
      daResponseIndexParser->parse( daXmlFile.c_str() );

      // No need to free this pointer - owned by the parent parser object 
      DOMDocument* tXMLDocument = daResponseIndexParser->getDocument();

      // Get the top-level element. 
      DOMElement* tElementRoot = tXMLDocument->getDocumentElement();
      if( !tElementRoot ){
  
          printDebugMessages( "Well, Looks like DA response is not good!" );         
          return tReturn;  
      }

      DOMNodeList* tChildren = tElementRoot->getChildNodes();
      const  XMLSize_t tNodeCount = tChildren->getLength();

      // For all nodes, children of "hardwaresensor" in the XML tree.  
      for( XMLSize_t xx = 0; xx < tNodeCount; ++xx ){
           DOMNode* tCurrentNode = tChildren->item( xx );

         if( tCurrentNode->getNodeType() &&  // True is not NULL 
             tCurrentNode->getNodeType() == DOMNode::ELEMENT_NODE ) // is element   
         {
            // Found node is an Element. Re-cast node as element  
             DOMElement* tCurrentElement
                        = dynamic_cast< xercesc::DOMElement* >( tCurrentNode );
                     
             string tStringBuffer = "";
             tStringBuffer =  GetTextContentOfAnElement( tCurrentElement, string( "sensortype" ) ); 
             if( tStringBuffer.compare( "_NO_TAG_FOUND_" ) != 0 )
                 daEventResponseIndex.sensorType = tStringBuffer;  
             tStringBuffer =  GetTextContentOfAnElement( tCurrentElement, string( "sensordescription" ) ); 
             if( tStringBuffer.compare( "_NO_TAG_FOUND_" ) != 0 )
                 daEventResponseIndex.sensorDescription = tStringBuffer;  
             tStringBuffer =  GetTextContentOfAnElement( tCurrentElement, string( "sensorfunction" ) ); 
             if( tStringBuffer.compare( "_NO_TAG_FOUND_" ) != 0 )
                 daEventResponseIndex.sensorFunction = tStringBuffer;  
             tStringBuffer =  GetTextContentOfAnElement( tCurrentElement, string( "sensorstatus" ) ); 
             if( tStringBuffer.compare( "_NO_TAG_FOUND_" ) != 0 )
                 daEventResponseIndex.sensorStatus = tStringBuffer;  
             tStringBuffer =  GetTextContentOfAnElement( tCurrentElement, string( "sensorevent" ) ); 
             if( tStringBuffer.compare( "_NO_TAG_FOUND_" ) != 0 )
                 daEventResponseIndex.sensorEvent = tStringBuffer;  
             tStringBuffer =  GetTextContentOfAnElement( tCurrentElement, string( "sensoreventtype" ) ); 
             if( tStringBuffer.compare( "_NO_TAG_FOUND_" ) != 0 )
                 daEventResponseIndex.sensorEventType = tStringBuffer;  
             tStringBuffer =  GetTextContentOfAnElement( tCurrentElement, string( "sensorreadingx" ) ); 
             if( tStringBuffer.compare( "_NO_TAG_FOUND_" ) != 0 )
                 daEventResponseIndex.sensorReadingX = tStringBuffer;  
             tStringBuffer =  GetTextContentOfAnElement( tCurrentElement, string( "sensorreadingy" ) ); 
             if( tStringBuffer.compare( "_NO_TAG_FOUND_" ) != 0 )
                 daEventResponseIndex.sensorReadingY = tStringBuffer;  
             tStringBuffer =  GetTextContentOfAnElement( tCurrentElement, string( "sensorreadingz" ) ); 
             if( tStringBuffer.compare( "_NO_TAG_FOUND_" ) != 0 )
                 daEventResponseIndex.sensorReadingZ = tStringBuffer;  
             tStringBuffer =  GetTextContentOfAnElement( tCurrentElement, string( "sensortimestamp" ) ); 
             if( tStringBuffer.compare( "_NO_TAG_FOUND_" ) != 0 )
                 daEventResponseIndex.sensorTimeStamp = tStringBuffer;  
             tStringBuffer =  GetTextContentOfAnElement( tCurrentElement, string( "sensormountports" ) ); 
             if( tStringBuffer.compare( "_NO_TAG_FOUND_" ) != 0 )
                 daEventResponseIndex.sensorMountPorts = tStringBuffer;  
        }
      }
   }
   catch( xercesc::XMLException& error ) {
  
      char* errorMessage = xercesc::XMLString::transcode( error.getMessage() );
      string debugLineString = "Error while parsing the XML:";
      debugLineString.append ( errorMessage );         
      printDebugMessages( debugLineString ); 
      XMLString::release( &errorMessage );
   } 

   return tReturn; 
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
    
    if( serialChatTerminal.initializeSerialTerminal( ) ){ // Start reading some data over serial 

         serialChatTerminal.writeToSerialOverUSB( "1" );
         string tDAResponse = serialChatTerminal.readFromSerialOverUSB();
         serialChatTerminal.printDebugMessages( tDAResponse );

         // Parse DA response 
         serialChatTerminal.readAndParseDAFeeds( tDAResponse );
         serialChatTerminal.printDebugMessages( serialChatTerminal.getParsedDAResponse());
    }
    else serialChatTerminal.printDebugMessages( "Some is wrong with serial port settings!" );
   } 
   else serialChatTerminal.printDebugMessages( "Well. It looks like something is wrong with serial port!" ); 

   return EXIT_SUCCESS; 
}

#endif 
