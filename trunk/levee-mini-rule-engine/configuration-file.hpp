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
// This will take configurations from file  
//


#ifndef CONFIGURATION_FILE_HPP
#define CONFIGURATION_FILE_HPP

// Include files for string and io operations 
#include <string>
#include <iostream>

using namespace std;

//
// Class, ConfigurationFile, this class will parse configuration file and pull our desire key-values  
//

class ConfigurationFile{

public: 
          ConfigurationFile( void );             // Initialize   
          ~ConfigurationFile( void );            // Reset or cleanup if any
          void printDebugMessages( string );     // Prints debug messages 
          bool readConfigurationFile( string );  // Reads configurations into raw buffer 
          string getValueOfGivenKey( string );   // Extract values of desire (valid) keys

private:
         
          // Is debugger enabled?  
          bool isDebugEnabled; 
          // Raw string buffer to store configurations   
          string rawConfigurationFileBuffer;  
};
#endif 
