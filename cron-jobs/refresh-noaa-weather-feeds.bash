#!/bin/bash

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
# Cron job to fetch noaa weather feeds in background. 
#

# Get the full path of current directory 
fullDirectoryPath="$(dirname "$(readlink -f ${BASH_SOURCE[0]})")"

# Change the directory to mini-rule-engine 
cd $fullDirectoryPath/../levee-mini-rule-engine

# Kill previous pending noaa process if any 
killall noaa-weather-sensor >>/dev/null 2>>/dev/null  

# Fork new process to fetch fresh weather feeds 
./noaa-weather-sensor >>/dev/null 2>>/dev/null 

