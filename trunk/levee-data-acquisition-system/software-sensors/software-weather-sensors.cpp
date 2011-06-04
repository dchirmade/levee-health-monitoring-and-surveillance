/**
 * 
 * Levee Health Monitoring And Surveillance System (Real-time)
 * Copyright (C) 2011 Fibercorps (http://www.fibercorps.com) 
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * GSoC11 Developer: Dipak 
 * GSoC11 Dashboard: http://www.google-melange.com/gsoc/project/google/gsoc2011/q1w2e3r4/12001
 * GSoC11 Mentor   : Dr.Raju Gottumukkala and Crawford Comeaux
 * 
 */

#ifndef SOFTWARE_WEATHER_SENSORS_CPP
#define SOFTWARE_WEATHER_SENSORS_CPP

#include <string>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

#include "software-weather-sensors.hpp"

using namespace xercesc;
using namespace std;

WeatherSensors::WeatherSensors( void ){

   /** Do initialization neeed for weather sensor */
   isDebugEnabled = true;
   weatherFeedsIndexParser = NULL; 
  
   /** Initialize Xerces libraries */ 
   try{
    
       XMLPlatformUtils::Initialize();
   } 
   catch( XMLException & error ){ /** Throw and exception if any */
       
       char * errorMessage = XMLString::transcode( error.getMessage() ); /** Get an error message */ 
       printDebugMessages( errorMessage );  
       XMLString::release( &errorMessage ); 
   }

   /** Assign new instance of DOM parser */
   weatherFeedsIndexParser = new XercesDOMParser; 
     
}

WeatherSensors::~WeatherSensors( void ){

   /** Free the XML DOM instance before terminating XMLPlatformUtils */
   if(weatherFeedsIndexParser) 
     delete weatherFeedsIndexParser;

   /* Terminate and release any allocated memories and libraries */
   try{

       XMLPlatformUtils::Terminate(); 
   }
   catch( xercesc::XMLException & error ){
       char * errorMessage = XMLString::transcode( error.getMessage() ); /** Get an error message */ 
       printDebugMessages( errorMessage );  
       XMLString::release( &errorMessage );
   } 
  
}

string WeatherSensors::GetTextContentOfAnElement ( 
                       DOMElement* tCurrentElementLevel2,
                       string tagName 
                       ){

    string tStringAttribute = ""; 
    try{
 
    /* Find text content of a given tag */
    XMLCh* tTAG = XMLString::transcode( tagName.c_str() );
    if( XMLString::equals( tCurrentElementLevel2->getTagName(), tTAG) )
    {
     
      /* Fetch the parameters at Level 2 children than parents */ 
      char *tCharAttribute = NULL;
      const XMLCh* tCTAG = NULL;
      tCTAG = tCurrentElementLevel2->getTextContent(); 
      tCharAttribute = XMLString::transcode(tCTAG);
                       
      if( tCharAttribute != NULL )
          cout << tCharAttribute << endl;
      if( tCharAttribute ) XMLString::release( &tCharAttribute );
     
     }
     
     XMLString::release( &tTAG );
     }
     catch( ... )
     {
       printDebugMessages( "Exception encountered in getting text content." ); 
     }

     return tStringAttribute;  
}

void WeatherSensors::printDebugMessages( string debugLines ){

   /** Print debug messages to standard console. */
   if( isDebugEnabled )
       cout << endl << debugLines << endl; 

   return; 
}

void WeatherSensors::readAndParseWeatherFeeds( string & tWeatherFeedsFile )
     throw( std::runtime_error ){
  
   /** Do some validation if weather feeds file is found in pointed out path */
   struct stat tFeedFileStatus;

   int returnFeedsFileStatus = stat( tWeatherFeedsFile.c_str(), &tFeedFileStatus );
   if( returnFeedsFileStatus == ENOENT )
      throw ( std::runtime_error( "Path file_name does not exist, or path is an empty string." ) );
   else if( returnFeedsFileStatus == ENOTDIR )
      throw ( std::runtime_error( "A component of the path is not a directory." ) );
   else if( returnFeedsFileStatus == ELOOP )
      throw ( std::runtime_error( "Too many symbolic links encountered while traversing the path." ) );
   else if( returnFeedsFileStatus == EACCES )
      throw ( std::runtime_error( "Permission denied." ) );
   else if( returnFeedsFileStatus == ENAMETOOLONG )
      throw ( std::runtime_error( "File can not be read.\n" ) );

   /** Configure the DOM parser */

   weatherFeedsIndexParser->setValidationScheme( XercesDOMParser::Val_Never );
   weatherFeedsIndexParser->setDoNamespaces( false );
   weatherFeedsIndexParser->setDoSchema( false );
   weatherFeedsIndexParser->setLoadExternalDTD( false );

   try
   {
      weatherFeedsIndexParser->parse( tWeatherFeedsFile.c_str() );

      /** No need to free this pointer - owned by the parent parser object */
      DOMDocument* tXMLDocument = weatherFeedsIndexParser->getDocument();

      /** Get the top-level element. */
      DOMElement* tElementRoot = tXMLDocument->getDocumentElement();
      if( !tElementRoot ) throw(std::runtime_error( "An empty XML document!" ));

      /** Parse XML file for tags of interest like "credit, station, image etc"
      Look one level nested within "wx_station_index" */ 

      DOMNodeList* tChildren = tElementRoot->getChildNodes();
      const  XMLSize_t tNodeCount = tChildren->getLength();

      /** For all nodes, children of "wx_station_index" in the XML tree. */ 
      for( XMLSize_t xx = 0; xx < tNodeCount; ++xx ){
           DOMNode* tCurrentNode = tChildren->item( xx );

         if( tCurrentNode->getNodeType() &&  /* true is not NULL */
             tCurrentNode->getNodeType() == DOMNode::ELEMENT_NODE ) /* is element */  
         {
            /* Found node which is an Element. Re-cast node as element */ 
             DOMElement* tCurrentElement
                        = dynamic_cast< xercesc::DOMElement* >( tCurrentNode );
           
            /* Get into all station node */ 
            XMLCh* TAG_STATION = XMLString::transcode("station");
            if( XMLString::equals( tCurrentElement->getTagName(), TAG_STATION ) )
            {
               
               /* Now parse the tree for level 2 */    
  	       DOMNodeList* tChildrenLevel2 = tCurrentElement->getChildNodes();
               const  XMLSize_t tNodeCountLevel2 = tChildrenLevel2->getLength();

 
               for( XMLSize_t xxLevel2 = 0; xxLevel2 < tNodeCountLevel2; ++xxLevel2 )
               {
                 /* Try to fetch children at level 2 */  
                     DOMNode* tCurrentNodeLevel2 = tChildrenLevel2->item( xxLevel2 );
                 if( tCurrentNodeLevel2->getNodeType() &&  /* true is not NULL */ 
                     tCurrentNodeLevel2->getNodeType() == DOMNode::ELEMENT_NODE ) /* is element */ 
 
                   {
                     /* Found node which is an Element. Re-cast node as element */ 
                     DOMElement* tCurrentElementLevel2
                      = dynamic_cast< xercesc::DOMElement* >( tCurrentNodeLevel2 );
                    
                     /* WIP! */ 
                     cout << GetTextContentOfAnElement( tCurrentElementLevel2, string( "station_id") ); 
                     cout << GetTextContentOfAnElement( tCurrentElementLevel2, string( "state") ); 
                     cout << GetTextContentOfAnElement( tCurrentElementLevel2, string( "station_name") ); 
                     cout << GetTextContentOfAnElement( tCurrentElementLevel2, string( "latitude") ); 
                     cout << GetTextContentOfAnElement( tCurrentElementLevel2, string( "longitude") ); 
                     cout << GetTextContentOfAnElement( tCurrentElementLevel2, string( "html_url") ); 
                     cout << GetTextContentOfAnElement( tCurrentElementLevel2, string( "rss_url") ); 
                     cout << GetTextContentOfAnElement( tCurrentElementLevel2, string( "xml_url") ); 
   
                   }       
                 }  
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
   return; 
}

#endif

#ifdef WEATHER_MAIN 
int main( void ){

  /*ToDo: To write a makefile for : g++ -g -Wall -pedantic -lxerces-c software-weather-sensors.cpp -DWEATHER_MAIN -o weather; */
  string weatherFeedsIndexFile = "index.xml"; /** Currently hardcoded */
 
  /** Parse and build weather feeds base into vectors */
  WeatherSensors NOAAWeatherFeeds;
  NOAAWeatherFeeds.readAndParseWeatherFeeds( weatherFeedsIndexFile ); 

  return 0; 
} 

#endif
