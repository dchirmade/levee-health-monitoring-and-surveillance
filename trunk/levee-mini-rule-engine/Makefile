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
 # GSoC11 Developer: Dipak 
 # GSoC11 Dashboard: http://www.google-melange.com/gsoc/project/google/gsoc2011/q1w2e3r4/12001
 # GSoC11 Mentor   : Dr.Raju Gottumukkala and Crawford Comeaux
 # 
 # This is a makefile for Core code base of levee health monitoring system.  
 ## :

all: 
	g++ -g -Wall -DMINI_RULE_ENGINE_MAIN noaa-software-weather-sensors.cpp\
		      mini-rule-engine.cpp serial-reader.cpp knowledge-base.cpp -pedantic -lxerces-c -lserial -o mini-rule-engine
	g++ -g -Wall -DSERIAL_MAIN -lserial serial-reader.cpp -o serial-reader
	g++ -g -Wall -DKNOWLEDGE_BASE_MAIN knowledge-base.cpp -o knowledge-base
	g++ -g -Wall -pedantic -lxerces-c -DWEATHER_MAIN noaa-software-weather-sensors.cpp -o noaa-weather-sensor
clean: 
	rm -rf mini-rule-engine serial-reader knowledge-base noaa-weather-sensor
