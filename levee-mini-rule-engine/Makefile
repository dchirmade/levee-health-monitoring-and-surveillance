 ##
 # 
 # Levee Health Monitoring And Surveillance System (Real-time)
 # Copyright (C) 2011 Fibercorps (http://www.fibercorps.com) 
 # 
 # This program is free software: you can redistribute it and/or modify
 # it under the terms of the GNU General Public License as published by
 # the Free Software Foundation, either version 3 of the License, or
 # (at your option) any later version.
 # 
 # This program is distributed in the hope that it will be useful,
 # but WITHOUT ANY WARRANTY; without even the implied warranty of
 # MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 # GNU General Public License for more details.
 # 
 # You should have received a copy of the GNU General Public License
 # along with this program.  If not, see <http://www.gnu.org/licenses/>.
 # 
 # This is a makefile for Core code base of levee health monitoring system.  
 ## :

all: 
	g++ -g -Wall -DMINI_RULE_ENGINE_MAIN noaa-software-weather-sensors.cpp noaa-software-water-sensors.cpp \
		      mini-rule-engine.cpp serial-reader.cpp knowledge-base.cpp configuration-file.cpp \
		      -pedantic -lxerces-c -lserial -o mini-rule-engine
	g++ -g -Wall -DSERIAL_MAIN -lserial -pedantic -lxerces-c serial-reader.cpp -o serial-reader
	g++ -g -Wall -DKNOWLEDGE_BASE_MAIN configuration-file.cpp knowledge-base.cpp -o knowledge-base
	g++ -g -Wall -DCONFIGURATION_FILE_MAIN configuration-file.cpp -o configuration-file
	g++ -g -Wall -pedantic -lxerces-c -DWEATHER_MAIN noaa-software-weather-sensors.cpp knowledge-base.cpp configuration-file.cpp -o noaa-weather-sensor
	g++ -g -Wall -pedantic -DWATER_MAIN noaa-software-water-sensors.cpp -o noaa-water-sensor
clean: 
	rm -rf mini-rule-engine serial-reader knowledge-base configuration-file noaa-weather-sensor noaa-water-sensor
