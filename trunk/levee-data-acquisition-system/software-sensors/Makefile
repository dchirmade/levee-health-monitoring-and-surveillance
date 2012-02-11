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
 # 
 # This is a makefile for software weather and water sensor based on NOAA (www.noaa.gov and water.weather.gov)'s feeds.  
 ## :

all: 
	g++ -g -Wall -pedantic -lxerces-c -DWEATHER_MAIN noaa-software-weather-sensors.cpp -o noaa-weather-sensor
	g++ -g -Wall -pedantic -DWATER_MAIN noaa-software-water-sensors.cpp -o noaa-water-sensor

clean: 
	rm -rf noaa-weather-sensor noaa-water-sensor 
