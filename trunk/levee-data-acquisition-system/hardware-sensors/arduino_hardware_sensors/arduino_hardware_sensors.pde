/**

 Levee Health Monitoring And Surveillance System (Real-time)
 Copyright (C) 2011 Fibercorps (http://www.fibercorps.com) 
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 
 GSoC11 Developer: Dipak 
 GSoC11 Dashboard: http://www.google-melange.com/gsoc/project/google/gsoc2011/q1w2e3r4/12001
 
 */
 
 /**
 
 This module should generate an event to send to levee-data-acquisition system.  
 Event generated will be carrying information fetched from hardware sensors placed in infrastructure under the scan.     
 
 List of electronics components, kits: 
 1. Arduino Duemilanove Italian) ATmega328 mother-board http://www.arduino.cc/en/Main/ArduinoBoardDuemilanove 
 2. ADXL335 3-axis accelerometer daughter-board http://www.adafruit.com/products/163 
 3. LED 
 4. USB cable 
 5. Connecting wires 
 6. HXD buzzer
 
 FixMe: a. Connection diagram.
        b. Currently only one sensors is used to demonstrate hardware event base. 
           Other sensors could be covered at later stage of project. 
 
 */
 
 
 /**
   
  Arduino Pins assiggments ( Gnerally needed by all i/o boards )   
     
 */ 
 
 const int groundPin   = 19;    /*  Pull analog input pin 5 as common ground */
 const int powerPin    = 18;    /*  Pull analog input pin 4 as vcc/voltage */
 const int sensorXPin  = A2;    /*  X-asis of the accelerometer sensor */
 const int sensorYPin  = A1;    /*  Y-asis of the accelerometer sensor */
 const int sensorZPin  = A0;    /*  Z-asis of the accelerometer sensor */
 const int LEDPin      = 13;    /*  LED out pin on 13 as vcc/voltage */ 
 const int speakerPin  = 12;    /*  Speaker out pin on 12 as vcc/voltage */

 /**
   
  Temporary assignments. These should be calibered at later stage with Levee D.A. System.    
   
 */

 int xReadingToMonitor = 367; /* These readings are frozen when ADXL335 sensor is immersed in levees */ 
 int yReadingToMonitor = 433; 
 int zReadingToMonitor = 433; 
 
 int alertCount        = 3; /* This cound should decide if an alert event is generated or what  */ 
 int tolerance         = 5; /* This is needed to avoid any chnage in reading due to sensor's limitation */ 
 int daLineCount;           /* Needed to plot the graph and raw data processing */
 
 const String TAB = "\t";

/**

 Desc: To setup Ardunio hardware, Kind of RC.D...
 Params: void, nothing  
 Returns: void, nothing
 
*/

void setup( void )
{
  /* Initialize the serial communications */
  Serial.begin( 9600 );
  
  /* Setup i/o pins mode and power accordingly */
  pinMode( groundPin, OUTPUT );
  pinMode( powerPin, OUTPUT );
  digitalWrite( groundPin, LOW );
  digitalWrite( powerPin, HIGH );
  pinMode( LEDPin, OUTPUT );              /* Set pin to input */
  digitalWrite( LEDPin, HIGH );           /* Turn on pullup resistors */
  pinMode( speakerPin, OUTPUT );          /* Set pin to input */ 
  digitalWrite( speakerPin, HIGH );       /* Turn on pullup resistors */ 

  daLineCount = 0;        
}

/**

 Desc:  This is nothing but main() for Arduino! 
 Params: void, nothing  
 Returns: void, nothing
 
*/

void loop( void )
{
  /** 
   Fetch the values from sensors 
  */
  
  int tSensorsXReading = analogRead(sensorXPin);
  int tSensorsYReading = analogRead(sensorYPin);
  int tSensorsZReading = analogRead(sensorZPin);
 
  daLineCount++;  
   
  /* Send readings to serial terminal! */
  Serial.print( daLineCount  + TAB + tSensorsXReading + TAB + tSensorsYReading  + TAB + tSensorsZReading);
  Serial.println();

  /* Simple event detection trigger. This will send XML dump to D.A. at later stage! */
  if(
    ((tSensorsXReading - tSensorsXReading) > tolerance || (tSensorsXReading - tSensorsXReading) < ( tolerance * -1 )) ||
    ((tSensorsYReading - tSensorsYReading) > tolerance || (tSensorsYReading - tSensorsYReading) < ( tolerance * -1 )) ||
    ((tSensorsZReading - tSensorsZReading) > tolerance || (tSensorsZReading - tSensorsZReading) < ( tolerance * -1 ))  
    )
  {
 
    /* Perform some actions upon detected event like flash LED or Buzz the speaker */ 
    digitalWrite(speakerPin, HIGH);
    delay(500);
    digitalWrite(LEDPin, HIGH);
    digitalWrite(speakerPin, LOW);
    delay(500);
    digitalWrite(LEDPin, LOW);
  }

  /* Keep the speaker at low */
  digitalWrite(speakerPin, LOW); 
  delay(500);
}

