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

#ifndef SOFTWARE_WEATHER_SENSORS_HPP
#define SOFTWARE_WEATHER_SENSORS_HPP

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

#include <string>
#include <stdexcept>
#include <vector>

using namespace xercesc;
using namespace std;

class WeatherSensors {

public:

      /** Weather stations atrributes */
      struct weatherStationSoftwareSensor{
       
        string stationId;     /** Id of a weather station */ 
        string stationState;  /** State where a station is located */
        string stationName;   /** Name of a station */
        string stationLatitude; /** Latitude of a station */ 
        string stationLongitude; /** Longitude of a station */
        string stationHTMLUrl; /** HTML URL of a station */  
        string stationRSSUrl; /** RSS feeds URL of a station */ 
        string stationXMLUrl; /** XML URL of a station */
      }stationSensorIndex;

      /** Vector listing of all stations */
      vector< weatherStationSoftwareSensor > vectorStationSensorIndex; 

      WeatherSensors( void );        /** Initializes xerces-C libraries */	
      ~WeatherSensors( void );       /** Frees any memory allocated by xerces parser */
      void printDebugMessages( string );  /** Prints debug messages if enalbed */
      string GetTextContentOfAnElement ( DOMElement *, string ); /** Get the text contect of given dom element */
      void readAndParseWeatherFeeds ( string & ) throw(std::runtime_error); /** Reads and parse feeds per desire needs */       

private:
      /** Handle for Xerces DOM Parser */
      xercesc::XercesDOMParser *weatherFeedsIndexParser;    
      /** Is debugger enabled? */ 
      bool isDebugEnabled; 
}; 

#endif
