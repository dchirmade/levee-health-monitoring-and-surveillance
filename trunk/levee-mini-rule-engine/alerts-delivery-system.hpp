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
// This will parse the xml events fetched from D.A. hardware or software sensor(s) and will take actions
// or will send an alert mail if any 

#include <iostream>
#include <cstdlib>

using namespace std;

//
// Desc: This will send an email notification to mentioned emergency notification addresses.  
// Arguments: string, Subject line to be sent  
//            string, To address of a recipient
//            string, Text body which need to be delivered  
// Returns: Nothing, void 
//

void sendEmailNotification(
                            string tSubjectLine,
                            string tToAddresses,
                            string tBodyText
                          ){

   // Build body part!  
   string tExecCommand = "/bin/echo \'" + tBodyText +  "\' >/tmp/notification.txt";
   system( tExecCommand.c_str() );

   // Form a command to send a notification alert
   tExecCommand = "/usr/bin/mail -s \"" + tSubjectLine +
                         "\" " + tToAddresses + " < /tmp/notification.txt >>/dev/null 2>>/dev/null";
   system( tExecCommand.c_str() );

   return;
}

