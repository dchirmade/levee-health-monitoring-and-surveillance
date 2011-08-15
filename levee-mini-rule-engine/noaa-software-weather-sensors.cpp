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
// This software sensor will tune to all weather station's, near to levee(s), listed 
// and updaed by NOAA http://www.noaa.gov/ and will generate an alert just in case of very bad weather conditions 
// 
// 
 

#ifndef NOAA_SOFTWARE_WEATHER_SENSORS_CPP
#define NOAA_SOFTWARE_WEATHER_SENSORS_CPP

#include <string>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

#include "noaa-software-weather-sensors.hpp"

using namespace xercesc;
using namespace std;

//
// Desc: Constructor for weather sensor. This should initialize xerces libraries and parser handle.  
// Arguments: Nothing, void  
// Returns: Not applicable  
//  
 
WeatherSensors::WeatherSensors( void ){

   // Do initialization neeed for weather sensor 
   isDebugEnabled = false;
   weatherFeedsIndexParser = NULL; 
  
   // Initialize Xerces libraries  
   try{
    
       XMLPlatformUtils::Initialize();
   } 
   catch( XMLException & error ){ // Throw and exception if any 
       
       char * errorMessage = XMLString::transcode( error.getMessage() ); // Get an error message  
       printDebugMessages( errorMessage, false );  
       XMLString::release( &errorMessage ); 
   }

   // Assign new instance of DOM parser 
   weatherFeedsIndexParser = new XercesDOMParser; 
     
}


//
// Desc: Distructor for weather sensor. This should release xerces libraries and parser handle.  
// Arguments: Nothing, void  
// Returns: Not applicable  
//
   
WeatherSensors::~WeatherSensors( void ){

   // Free the XML DOM instance before terminating XMLPlatformUtils 
   if(weatherFeedsIndexParser) 
     delete weatherFeedsIndexParser;

   // Terminate and release any allocated memories and libraries 
   try{

       XMLPlatformUtils::Terminate(); 
   }
   catch( xercesc::XMLException & error ){
       char * errorMessage = XMLString::transcode( error.getMessage() ); // Get an error message  
       printDebugMessages( errorMessage, false );  
       XMLString::release( &errorMessage );
   } 
  
}

//
// Desc: This should extract the text content of DOM element.   
// Arguments: DOMElement, DOM element of an XML tree to which text content to be fetched. 
//            String, Name of a tag in XML tree
// Returns: String, Returns extracted text content of a desire DOM element
//   
 
string WeatherSensors::GetTextContentOfAnElement ( 
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
       printDebugMessages( "Exception encountered in getting text content.", false ); 
     }

     return tStringAttribute;  
}

//
// Desc: This should print debug messages only if it is enabled.   
// Arguments: String, Lines which needs to be printed on standard console.
//            Bool, Show debug messages forcefully 
// Returns: Nothing, void 
//   
 
void WeatherSensors::printDebugMessages( 
                                         string debugLines,
                                         bool isDebugForced = false
                                       ){

   // Get current time and stamp it with logs 
   time_t tRawTime;
   time ( &tRawTime );

   string tLocaltime = asctime( localtime( &tRawTime ) );
   tLocaltime.replace( tLocaltime.find( "\n" ), 1 ,"\0" ); // Replace \n at the end of time string


   // Print debug messages to standard console. 
   if( isDebugEnabled || isDebugForced )
       cout << "<br>" << tLocaltime << " :" << debugLines << endl; 

   return; 
}

//
// Desc: Parses xml URL and return station dump file name from URL   
// Arguments: String, Full xml URL to be parsed  
// Returns: string, Dump file's name as a token  
//   
 
string WeatherSensors::parseURLandPullOutStationName( string XMLUrl ){

   string stationName = "";

   // Do String-tok and pull out station name 
   if( XMLUrl.length() != 0 )
       stationName = XMLUrl.substr( XMLUrl.rfind( "/" ) + 1 ); 

   return stationName; 
}

//
// Desc: Crawl through all weather stations and download xml dump for each station  
// Arguments: bool, Is there any need to update weather files? 
//            string, station name of which only feeds need to be downloaded. 
// Returns: bool, true if downloda for all stations is good
//   
 
bool WeatherSensors::crawlThroughStationsData( bool isUpdateNeeded, string stationLocation = "" ){

   bool tReturn = false; 

    
   // Delete all old xml dump files if update needed
   // Delayed download or slow download could leave system with no feeds if 
   // we delete them all! 
   // if( isUpdateNeeded == true ){ 
   //    system( "/bin/rm ./dump/*.xml 2>>/dev/null >>/dev/null" ); 
   //    system( "/bin/rm ./dump/*.xml.* 2>>/dev/null >>/dev/null" ); 
   // }

   // Pull out all stations data from vectors and dump xml stations accordingly. 
   for( int stationList = 0 ; stationList < (int) vectorStationSensorIndex.size() ; stationList++ ){
  
     string tStationName = "./dump/" + 
                           parseURLandPullOutStationName( 
                                                          vectorStationSensorIndex[stationList].stationXMLUrl
                                                        ); 
     if( vectorStationSensorIndex[stationList].stationName.find( stationLocation ) != string::npos ){ 

     if( isUpdateNeeded == true ) {
  
         // Delete old feed at a time 
         string tDeleteCommand = "/bin/rm " + tStationName + " 2>>/dev/null >>/dev/null";
         system( tDeleteCommand.c_str( ) );

         downloadXMLFeeds( vectorStationSensorIndex[stationList].stationXMLUrl );
      }

     if( tStationName.length() != 0 )
         readAndParsePerWeatherStationResponse( tStationName );
     else printDebugMessages( "Opps! Couldn't fetch station name from XMLURL!" );

     }
   }

   // Just assume all is good! 
   return tReturn; 
}

//
// This method is implemented for testing! This should go away soon once vector will sorted out... 
//
// Desc: This should print all weather station data per perticular location.   
// Arguments: String, Location name in which levee is built
// Returns: Nothing, void 
//   
 
void WeatherSensors::printAllStationsDataPerLocation( string tLocationName ){
 
   bool isStationFound = false; 
    
   // Pull out all stations data from vectors whoes location name matches with given location name 
   for( int stationList = 0 ; stationList < (int) vectorStationSensorIndex.size() ; stationList++ ){
  
     if( vectorStationSensorIndex[stationList].stationName.find( tLocationName ) != string::npos ){ 

      printDebugMessages( "Station ID        : " + vectorStationSensorIndex[stationList].stationId, true );
      printDebugMessages( "Station State     : " + vectorStationSensorIndex[stationList].stationState, true );
      printDebugMessages( "Station Name      : " + vectorStationSensorIndex[stationList].stationName, true );
      printDebugMessages( "Station Latitude  : " + vectorStationSensorIndex[stationList].stationLatitude, true );
      printDebugMessages( "Station Longitude : " + vectorStationSensorIndex[stationList].stationLongitude, true );
      printDebugMessages( "Station HTML URL  : " + vectorStationSensorIndex[stationList].stationHTMLUrl, true );
      printDebugMessages( "Station RSS URL   : " + vectorStationSensorIndex[stationList].stationRSSUrl, true );
      printDebugMessages( "Station XML URL   : " + vectorStationSensorIndex[stationList].stationXMLUrl, true );
      printDebugMessages( "Station RFC time  : " + vectorStationSensorIndex[stationList].drilledDownParameters.observationTimeRfc822, true ); 
      printDebugMessages( "Station Weather   : " + vectorStationSensorIndex[stationList].drilledDownParameters.weather, true );                
      printDebugMessages( "-------------------------------------------------------------------------", true );

      // Set the flag
      isStationFound = true;
     }
   }

   if( !isStationFound ) printDebugMessages( "No weather station found by querying:" + tLocationName );

   return; 
}

//
// Desc: This should print all weather sensor related attribute per weather location.   
// Arguments: Nothing, void
// Returns: Nothing, void 
//   
 
void WeatherSensors::printAllStationsData( void ){
  
   // Pull out all stations data from vectors and print it to console. 
   for( int stationList = 0 ; stationList < (int) vectorStationSensorIndex.size() ; stationList++ ){
   
      printDebugMessages( "Station ID        : " + vectorStationSensorIndex[stationList].stationId );
      printDebugMessages( "Station State     : " + vectorStationSensorIndex[stationList].stationState );
      printDebugMessages( "Station Name      : " + vectorStationSensorIndex[stationList].stationName );
      printDebugMessages( "Station Latitude  : " + vectorStationSensorIndex[stationList].stationLatitude );
      printDebugMessages( "Station Longitude : " + vectorStationSensorIndex[stationList].stationLongitude );
      printDebugMessages( "Station HTML URL  : " + vectorStationSensorIndex[stationList].stationHTMLUrl );
      printDebugMessages( "Station RSS URL   : " + vectorStationSensorIndex[stationList].stationRSSUrl );
      printDebugMessages( "Station XML URL   : " + vectorStationSensorIndex[stationList].stationXMLUrl );
      printDebugMessages( "Station RFC time  : " + vectorStationSensorIndex[stationList].drilledDownParameters.observationTimeRfc822 ); 
      printDebugMessages( "Station Weather   : " + vectorStationSensorIndex[stationList].drilledDownParameters.weather );                
      printDebugMessages( "-------------------------------------------------------------------------" );
   }

   return; 
}

//
// Desc: This should read, parse and build vector base for each weather station.    
// Arguments: String, XML file fetched from weather  
// Returns: Bool, true if all vectors built correctly or false otherwise
//   
 
bool WeatherSensors::readAndParsePerWeatherStationResponse( string & tWeatherFeedsFile )
     throw( std::runtime_error ){
 
   bool tReturn = false;
    
   // Do some validation if weather feeds file is found in pointed out path 
   struct stat tFeedFileStatus;

   int returnFeedsFileStatus = stat( tWeatherFeedsFile.c_str(), &tFeedFileStatus );
   if( returnFeedsFileStatus == 0 )
       tReturn = true;  
   else {
    if( returnFeedsFileStatus == ENOENT )
        printDebugMessages( "Path file_name does not exist, or path is an empty string." );
    else if( returnFeedsFileStatus == ENOTDIR )
        printDebugMessages( "A component of the path is not a directory." );
    else if( returnFeedsFileStatus == ELOOP )
        printDebugMessages( "Too many symbolic links encountered while traversing the path." );
    else if( returnFeedsFileStatus == EACCES )
        printDebugMessages( "Permission denied." );
    else if( returnFeedsFileStatus == ENAMETOOLONG )
        printDebugMessages( "File can not be read." );
    else printDebugMessages( "Can not open the file!" );
    return tReturn;  
   }

   // Configure the DOM parser 

   weatherFeedsIndexParser->setValidationScheme( XercesDOMParser::Val_Never );
   weatherFeedsIndexParser->setDoNamespaces( false );
   weatherFeedsIndexParser->setDoSchema( false );
   weatherFeedsIndexParser->setLoadExternalDTD( false );

   try
   {
      weatherFeedsIndexParser->parse( tWeatherFeedsFile.c_str() );

      // No need to free this pointer - owned by the parent parser object 
      DOMDocument* tXMLDocument = weatherFeedsIndexParser->getDocument();

      // Get the top-level element. 
      DOMElement* tElementRoot = tXMLDocument->getDocumentElement();
      if( !tElementRoot ){
  
          printDebugMessages( "Well, Looks like " + tWeatherFeedsFile + " weather station has changed it's xml response!" );         
          return tReturn;  
      }

      // Parse XML file for tags of interest like "credit, station, image etc"
      // Look one level nested within "current_observation"  

      DOMNodeList* tChildren = tElementRoot->getChildNodes();
      const  XMLSize_t tNodeCount = tChildren->getLength();

      // Drilled down parameters for per weather station 
      string tStringStationId( "" );      
      string tStringObservationTime( "" );      
      string tStringWeather( "" );      
 
      // For all nodes, children of "current_observation" in the XML tree.  
      for( XMLSize_t xx = 0; xx < tNodeCount; ++xx ){
           DOMNode* tCurrentNode = tChildren->item( xx );

         if( tCurrentNode->getNodeType() &&  // true is not NULL 
             tCurrentNode->getNodeType() == DOMNode::ELEMENT_NODE ) // is element   
         {
             // Found node is an Element. Re-cast node as element  
             DOMElement* tCurrentElement
                        = dynamic_cast< xercesc::DOMElement* >( tCurrentNode );
                     
             string tStringBuffer =  GetTextContentOfAnElement( tCurrentElement, string( "station_id") ); 
             if( tStringBuffer.compare( "_NO_TAG_FOUND_" ) != 0 )
                 tStringStationId = tStringBuffer;
             tStringBuffer =  GetTextContentOfAnElement( tCurrentElement, string( "observation_time_rfc822") ); 
             if( tStringBuffer.compare( "_NO_TAG_FOUND_" ) != 0 )
                 tStringObservationTime = tStringBuffer;
             tStringBuffer =  GetTextContentOfAnElement( tCurrentElement, string( "weather") ); 
             if( tStringBuffer.compare( "_NO_TAG_FOUND_" ) != 0 )
                 tStringWeather = tStringBuffer;
         }
      }
  
      // Search and sub station parameters in vector list  
      for( int stationList = 0 ; stationList < (int) vectorStationSensorIndex.size() ; stationList++ ){
   
           if( tStringStationId.compare( vectorStationSensorIndex[stationList].stationId ) == 0 ){
           
               // Initialize all sub weather parameters per station
               vectorStationSensorIndex[stationList].drilledDownParameters.observationTimeRfc822.assign( tStringObservationTime );                
               vectorStationSensorIndex[stationList].drilledDownParameters.weather.assign( tStringWeather );                
               break;
          }else continue;  
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

//
// Desc: This method will download xml feeds given URL  
// Arguments: String, Complete URL for XML feeds  
// Returns: Nothing, void
//  

void WeatherSensors::downloadXMLFeeds( string tDownloadXMLFeeds ){

   while( true ){  // non infinite loop! 
  
     if( tDownloadXMLFeeds.length() == 0 ) 
         break; // An empty URL   
      
     // Build wget payload
     printDebugMessages( "Downloading XML feeds from: " + tDownloadXMLFeeds );
     string tDownloadXMLFeedsstringBuffer = "/usr/bin/wget --tries=3 --wait=1 --quiet --directory-prefix=dump " + tDownloadXMLFeeds + 
                                            " >>/dev/null" + " 2>>/dev/null"; 
     system( tDownloadXMLFeedsstringBuffer.c_str() );
    
     break; 
   }
   return; 
}

//
// Desc: This should read, parse and build vector base of a weather feeds file.    
// Arguments: String, weather file name 
// Returns: Bool, true if all vectors built correctly or false otherwise
//   
 
bool WeatherSensors::readAndParseWeatherFeeds( string & tWeatherFeedsFile )
     throw( std::runtime_error ){
 
   bool tReturn = false;
    
   // Do some validation if weather feeds file is found in pointed out path 
   struct stat tFeedFileStatus;

   int returnFeedsFileStatus = stat( tWeatherFeedsFile.c_str(), &tFeedFileStatus );
   if( returnFeedsFileStatus == 0 )
       tReturn = true;  
   else {
    if( returnFeedsFileStatus == ENOENT )
        printDebugMessages( "Path file_name does not exist, or path is an empty string." );
    else if( returnFeedsFileStatus == ENOTDIR )
        printDebugMessages( "A component of the path is not a directory." );
    else if( returnFeedsFileStatus == ELOOP )
        printDebugMessages( "Too many symbolic links encountered while traversing the path." );
    else if( returnFeedsFileStatus == EACCES )
        printDebugMessages( "Permission denied." );
    else if( returnFeedsFileStatus == ENAMETOOLONG )
        printDebugMessages( "File can not be read." );
    else printDebugMessages( "Can not open the file!" );
    return tReturn;  
   }

   // Configure the DOM parser 

   weatherFeedsIndexParser->setValidationScheme( XercesDOMParser::Val_Never );
   weatherFeedsIndexParser->setDoNamespaces( false );
   weatherFeedsIndexParser->setDoSchema( false );
   weatherFeedsIndexParser->setLoadExternalDTD( false );

   try
   {
      weatherFeedsIndexParser->parse( tWeatherFeedsFile.c_str() );

      // No need to free this pointer - owned by the parent parser object 
      DOMDocument* tXMLDocument = weatherFeedsIndexParser->getDocument();

      // Get the top-level element. 
      DOMElement* tElementRoot = tXMLDocument->getDocumentElement();
      if( !tElementRoot ){
  
          printDebugMessages( "Well, Looks like weather station listing is not good!" );         
          return tReturn;  
      }

      // Parse XML file for tags of interest like "credit, station, image etc"
      // Look one level nested within "wx_station_index"  

      DOMNodeList* tChildren = tElementRoot->getChildNodes();
      const  XMLSize_t tNodeCount = tChildren->getLength();

      // For all nodes, children of "wx_station_index" in the XML tree.  
      for( XMLSize_t xx = 0; xx < tNodeCount; ++xx ){
           DOMNode* tCurrentNode = tChildren->item( xx );

         if( tCurrentNode->getNodeType() &&  // true is not NULL 
             tCurrentNode->getNodeType() == DOMNode::ELEMENT_NODE ) // is element   
         {
            // Found node is an Element. Re-cast node as element  
             DOMElement* tCurrentElement
                        = dynamic_cast< xercesc::DOMElement* >( tCurrentNode );
           
            // Get into all station node  
            XMLCh* TAG_STATION = XMLString::transcode("station");
            if( XMLString::equals( tCurrentElement->getTagName(), TAG_STATION ) )
            {
               
               // Now parse the tree for level 2     
  	       DOMNodeList* tChildrenLevel2 = tCurrentElement->getChildNodes();
               const  XMLSize_t tNodeCountLevel2 = tChildrenLevel2->getLength();

 
               for( XMLSize_t xxLevel2 = 0; xxLevel2 < tNodeCountLevel2; ++xxLevel2 )
               {
                 // Try to fetch children at level 2   
                     DOMNode* tCurrentNodeLevel2 = tChildrenLevel2->item( xxLevel2 );
                 if( tCurrentNodeLevel2->getNodeType() &&  // true is not NULL  
                     tCurrentNodeLevel2->getNodeType() == DOMNode::ELEMENT_NODE ) // is element  
 
                   {
                     // Found node which is an Element. Re-cast node as element  
                     DOMElement* tCurrentElementLevel2
                      = dynamic_cast< xercesc::DOMElement* >( tCurrentNodeLevel2 );
                    
                     // Now try to build vector array for all stations listed by NOAA  
                     string tStringBuffer = "";
                     tStringBuffer =  GetTextContentOfAnElement( tCurrentElementLevel2, string( "station_id") ); 
                     if( tStringBuffer.compare( "_NO_TAG_FOUND_" ) != 0 )
                         stationSensorIndex.stationId.assign( tStringBuffer ); 
                     tStringBuffer =  GetTextContentOfAnElement( tCurrentElementLevel2, string( "state") ); 
                     if( tStringBuffer.compare( "_NO_TAG_FOUND_" ) != 0 )
                         stationSensorIndex.stationState.assign( tStringBuffer ); 
                     tStringBuffer =  GetTextContentOfAnElement( tCurrentElementLevel2, string( "station_name") ); 
                     if( tStringBuffer.compare( "_NO_TAG_FOUND_" ) != 0 )
                         stationSensorIndex.stationName.assign( tStringBuffer ); 
                     tStringBuffer =  GetTextContentOfAnElement( tCurrentElementLevel2, string( "latitude") ); 
                     if( tStringBuffer.compare( "_NO_TAG_FOUND_" ) != 0 )
                         stationSensorIndex.stationLatitude.assign( tStringBuffer ); 
                     tStringBuffer =  GetTextContentOfAnElement( tCurrentElementLevel2, string( "longitude") ); 
                     if( tStringBuffer.compare( "_NO_TAG_FOUND_" ) != 0 )
                         stationSensorIndex.stationLongitude.assign( tStringBuffer ); 
                     tStringBuffer =  GetTextContentOfAnElement( tCurrentElementLevel2, string( "html_url") ); 
                     if( tStringBuffer.compare( "_NO_TAG_FOUND_" ) != 0 )
                         stationSensorIndex.stationHTMLUrl.assign( tStringBuffer ); 
                     tStringBuffer =  GetTextContentOfAnElement( tCurrentElementLevel2, string( "rss_url") ); 
                     if( tStringBuffer.compare( "_NO_TAG_FOUND_" ) != 0 )
                         stationSensorIndex.stationRSSUrl.assign( tStringBuffer ); 
                     tStringBuffer =  GetTextContentOfAnElement( tCurrentElementLevel2, string( "xml_url") ); 
                     if( tStringBuffer.compare( "_NO_TAG_FOUND_" ) != 0 )
                         stationSensorIndex.stationXMLUrl.assign( tStringBuffer ); 
      
                   }       
                 }  
                     
                 // Now push one station at a time into station vector 
                 vectorStationSensorIndex.push_back( stationSensorIndex );
         }
            XMLString::release( &TAG_STATION );
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
 
#ifdef WEATHER_MAIN 
int main( void ){

  // Fixed me: MAIN.LIST should be periodically updated from http://www.weather.gov/data/current_obs/index.xml
  string weatherFeedsIndexFile = "./dump/MAIN.LIST"; 
    
  // Parse and build weather feeds base into vectors 
  WeatherSensors NOAAWeatherFeeds;
  if( NOAAWeatherFeeds.readAndParseWeatherFeeds( weatherFeedsIndexFile ) ){

   
    // Get per station data
    if( NOAAWeatherFeeds.crawlThroughStationsData( true , "New Orleans" ) == true ){
       // WIP! Do some actions if needed
    }

    // Print all stations attributes. Just for testing!  
    NOAAWeatherFeeds.printAllStationsData(); 

    // Print all stations located in New Orleans
    NOAAWeatherFeeds.printAllStationsDataPerLocation("New Orleans"); 
  }
    
  return EXIT_SUCCESS; 
} 

#endif
