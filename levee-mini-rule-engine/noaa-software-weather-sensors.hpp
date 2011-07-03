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

#ifndef NOAA_SOFTWARE_WEATHER_SENSORS_HPP
#define NOAA_SOFTWARE_WEATHER_SENSORS_HPP

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

// Include files for string and vector operations 
#include <string>
#include <stdexcept>
#include <vector>

using namespace xercesc;
using namespace std;

//
//
// Desc: Class, Weather Sensor, This is reposible for fetching, building vector base of all 
// latest ( upto a delay of an hour ) weather feeds located in USA.         
// 
 

class WeatherSensors {

public:

      // Weather stations atrributes 
      struct weatherStationSoftwareSensor{
       
        string stationId;         // Id of a weather station  
        string stationState;      // State where a station is located 
        string stationName;       // Name of a station 
        string stationLatitude;   // Latitude of a station  
        string stationLongitude;  // Longitude of a station 
        string stationHTMLUrl;    // HTML URL of a station   
        string stationRSSUrl;     // RSS feeds URL of a station  
        string stationXMLUrl;     // XML URL of a station 
        
        struct drilledDownWeatherParameters{
        
          string observationTimeRfc822;  // Observation Time in RFC822 format
	  string weather;                // Weather type
    
          /* Following parameters are not used for far!  

	  string temperatureString;      // Temperature both F and C 
	  string tempF;                  // Temperature in fahrenheit
	  string tempC;                  // Temperature in Celsus
	  string relativeHumidity;       // Relative Humidity 
	  string windString;             // Wind parameters ( Degrees, Mph and Kt )
	  string windDirection;          // Wind flow direction
	  string windDegrees;            // Wind flow degrees  
	  string windMph;                // Wind speed in Mile per hour 
	  string windKt;                 // Wind speed in Knot
	  string dewpointString;         // Dew point in Both F and C 
	  string dewpointF;              // Dew point in fahrenheit
	  string dewpointC;              // Dew point in Celsus 
	  string heatIndexString;        // Heat index in both F and C 
      	  string heatIndexF;             // Heat index in fahrenheit 
      	  string heatIndexC;             // Heat index in Celsus

          */ 

        }drilledDownParameters;    
      }stationSensorIndex;

      // Vector listing of all stations 
      vector< weatherStationSoftwareSensor > vectorStationSensorIndex; 

      WeatherSensors( void );                // Initializes xerces-C libraries 	
      ~WeatherSensors( void );               // Frees any memory allocated by xerces parser 
      void printDebugMessages( string, bool ); // Prints debug messages if enalbed 
      string GetTextContentOfAnElement ( DOMElement *, string ); // Get the text contect of given dom element 
      bool readAndParseWeatherFeeds ( string & ) throw( std::runtime_error ); // Read and parse feeds per desire needs        
      bool readAndParsePerWeatherStationResponse( string & ) throw( std::runtime_error ); // Read and parse xml response obtained from station
      void printAllStationsData( void );     // Prints all fetched stations and substation data 
      void printAllStationsDataPerLocation( string ); // Prints all fetched stations and substation per givin location
      void downloadXMLFeeds( string );       // Download XML feeds and store them for parsing  
      bool crawlThroughStationsData( bool ); // Crawl through all stations and lookout for all stations   
      string parseURLandPullOutStationName( string );

private:
      // Handle for Xerces DOM Parser 
      xercesc::XercesDOMParser *weatherFeedsIndexParser;    
      // Is debugger enabled?  
      bool isDebugEnabled; 
}; 

#endif
