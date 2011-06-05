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

/**
 * 
 * This module should generate an event to send to levee-data-acquisition system.  
 * Event generated will be carrying information fetched from hardware sensors placed in infrastructure under the scan.     
 * 
 * List of electronics components, kits: 
 * 1. Arduino Duemilanove Italian) ATmega328 mother-board http://www.arduino.cc/en/Main/ArduinoBoardDuemilanove 
 * 2. ADXL335 3-axis accelerometer daughter-board http://www.adafruit.com/products/163 
 * 3. LED 
 * 4. USB cable 
 * 5. Connecting wires 
 * 6. HXD buzzer
 * 
 * 
 * Steps to reuse this code (only tried on gnu/linux platform): 
 * 1. Download and install Arduino IDE with complier. 
 * On Ubuntu do: aptitude  update ; aptitude install arduino; 
 * 2. Open this file in Arduino IDE. 
 * 3. Configure  Arduino IDE for first time use by selecting correct serial port and 
 * board type in tools section. 
 * 4. Press compile and upload button to upload cross complied code to Arduino board. 
 * 5. Open serial terminal to see any responses from Arduino board. 
 * 
 * 
 * FixMe: 
 * Currently only one sensors is used to demonstrate hardware event base. 
 * Other sensors could be covered at later stage of project. 
 * 
 */


/**
 * Flags or hard values 
 */

#define ISONBOARDDEBUGENABLE  0     /* Enable or disable onboard debug messages */
#define LOWDELAY 100                /* Delay with low value*/ 
#define MEDIUMDELAY  500            /* Delay with medium value*/
#define HIGHDELAY  1000             /* Delay with high value*/ 
#define PORTBAUDRATE  9600

/**
 * 
 * Arduino Pins assiggments ( Gnerally needed by all i/o boards )   
 * 
 */

const int groundPin   = 19;    /*  Pull analog input pin 5 as common ground */
const int powerPin    = 18;    /*  Pull analog input pin 4 as vcc/voltage */
const int sensorXPin  = A2;    /*  X-asis of the accelerometer sensor */
const int sensorYPin  = A1;    /*  Y-asis of the accelerometer sensor */
const int sensorZPin  = A0;    /*  Z-asis of the accelerometer sensor */
const int LEDPin      = 13;    /*  LED out pin on 13 as vcc/voltage */
const int speakerPin  = 12;    /*  Speaker out pin on 12 as vcc/voltage */

/**
 * 
 * Temporary assignments. These should be calibered at later stage using Levee D.A. System.    
 * 
 */

int xReadingToMonitor = 367; /* These readings are frozen when ADXL335 sensor is immersed in levees */
int yReadingToMonitor = 433; 
int zReadingToMonitor = 433; 
int alertCount        = 3; /* This cound should decide if an alert event is generated or what  */
int tolerance         = 5; /* This is needed to avoid any chnage in reading due to sensor's limitation */
int daLineCount;           /* Needed to plot the graph and raw data processing */
const String TAB = "\t";


/* Set this to the number of seconds to adjust per day, 
 positive values speed up clock, negative values slow clock down */

int adjustSeconds = 120; /*system variables*/
unsigned long previousMillis = 0;
unsigned long systemTime = 0;  /*Increments once per second*/

unsigned long adjustedSystemTime = 0;  /*Increments once per second plus/minus adjsutment*/
unsigned long adjustedpreviousMillis = 0;

/**
 * 
 * Desc: Adjust the time by adding milliseconds times adjust seconds every 86 seconds. 
 * This may help corrent time-stepping based calculation at an ascend side.
 * [Source code of this method is taken from ASCEND's DA wrapper developed in 2009 as a part of GSOC09.]
 * Params: @param, unsigned long now, Current time that need to be corrected.  
 * Returns: void, nothing. 
 * 
 */

void AdjustTime(unsigned long now){

  if( now - adjustedpreviousMillis >= 1000){
    adjustedpreviousMillis += 1000;

    if( (++adjustedSystemTime) % 86 == 0)  {
      adjustedpreviousMillis += adjustSeconds;

      /*Print the debug messages only if they're enabled*/
      if(ISONBOARDDEBUGENABLE){
        Serial.print("At adjustedSystemTime= ");
        Serial.print(adjustedSystemTime,DEC);
        Serial.print(" now= ");
        Serial.print(now,DEC);
        Serial.print(" now - adj=");
        Serial.print(now - adjustedpreviousMillis, DEC);
        Serial.print(" adding ");
        Serial.print(adjustSeconds,DEC);
        Serial.print(" msecs, adjPrevMs= ");
        Serial.println(adjustedpreviousMillis,DEC);
      }
    }
  }
}


/**
 * 
 * Desc: This method should generate an event notification to send to D.A. system 
 * Params: @String tSensorType, Is this sensor digital or analog? 
 * @String tSensorDescription, Vendor id etc 
 * @String tSensorFunction, What does it do? 
 * @String tSensorStatus,Current sensor status active or inactive
 * @String tSensorEvent, Reading changed or not changed from last snap etc 
 * @String tSensorEventType, Is optional or compulsory 
 * @unsigned long tSensorReadingA,Value that need to be send to D.A. module 
 * @unsigned long tSensorReadingB, (Optional) Value that need to be send to D.A. module 
 * @unsigned long tSensorReadingC, (Optional) Value that need to be send to D.A. module 
 * @String tSensorMountPorts, Port numbers where the sensor is mounted 
 * Returns: void, nothing 
 * 
 */

void generateEventNotification(
String tSensorType,           /* Is sensor digital or analog? */
String tSensorDescription,    /* Vendor id etc */
String tSensorFunction,       /* What does it do? */
String tSensorStatus,         /* Current sensor status active or inactive */
String tSensorEvent,          /* Reading changed or not changed from last snap etc */
String tSensorEventType,      /* Is optional or compulsory */
unsigned long tSensorReadingA, /* Value that need to be send to D.A. module */
unsigned long tSensorReadingB, /* (Optional) Value that need to be send to D.A. module */
unsigned long tSensorReadingC, /* (Optional) Value that need to be send to D.A. module */
String tSensorMountPorts /* Port numbers where the sensor is mounted */
)
{

  /*Temporary variables*/
  int second = -1;
  int minute = -1;
  int hours = -1;
  unsigned long now;
  String XMLResponseToDA = String(""); 

  /**
   * Please do not use 'millis()' with interrupts enabled as after catching the 
   * interrupt, 'millis()' may return garbage value.  
   */

  cli();  /*Disable interrupts*/
  now = millis();/*Get system clocks bits*/
  sei();  /*Enable interrupts*/


  /**
   * Adjust the time ticks up to 1 sec precision.  
   */
  while( now - previousMillis >= 1000 ){

    systemTime++;
    previousMillis += 1000;
  }

  AdjustTime( now );

  /* Calculate hours, minutes and seconds */
  second = systemTime % 60;
  minute = (systemTime / 60) % 60;
  hours  = (systemTime  / 3600) % 24;


  /* Build XML sensor response and send response in small size */

  XMLResponseToDA = String( "<hardwaresensor>\n" );
  Serial.print(XMLResponseToDA);
  XMLResponseToDA = String( "<sensortype>" ) + tSensorType + String("</sensortype>\n" );
  Serial.print(XMLResponseToDA);
  XMLResponseToDA = String( "<sensordescription>" ) + tSensorDescription + String("</sensordescription>\n" );
  Serial.print(XMLResponseToDA);
  XMLResponseToDA = String( "<sensorfunction>" ) + tSensorFunction + String("</sensorfunction>\n" );
  Serial.print(XMLResponseToDA);
  XMLResponseToDA = String( "<sensorstatus>" ) + tSensorStatus + String("</sensorstatus>\n" );
  Serial.print(XMLResponseToDA);
  XMLResponseToDA = String( "<sensorevent>" ) + tSensorEvent + String("</sensorevent>\n" );
  Serial.print(XMLResponseToDA);
  XMLResponseToDA = String( "<sensoreventyype>" ) + tSensorEventType + String("</sensoreventtype>\n" );
  Serial.print(XMLResponseToDA);
  XMLResponseToDA = String( "<sensorreadinga>" ) + String( tSensorReadingA ) + String("</sensorreadinga>\n" );
  Serial.print(XMLResponseToDA);
  XMLResponseToDA = String( "<sensorreadingb>" ) + String( tSensorReadingB ) + String("</sensorreadingb>\n" );
  Serial.print(XMLResponseToDA);
  XMLResponseToDA = String( "<sensorreadingc>" ) + String( tSensorReadingC ) + String("</sensorreadingc>\n" );
  Serial.print(XMLResponseToDA);
  XMLResponseToDA = String( "<sensortimestamp>" ) + String( second ) + String( ":" ) + String( minute ) + String( ":" ) + String( hours ) + String("</sensortimestamp>\n" );
  Serial.print(XMLResponseToDA);
  XMLResponseToDA = String( "<sensormountports>" ) + tSensorMountPorts + String("</sensormountports>\n" );
  Serial.print(XMLResponseToDA);
  XMLResponseToDA = String( "</hardwaresensor>\n" );
  Serial.print(XMLResponseToDA);

  return; 
}

/**
 * Desc: This method should fetch reading from ADXL335 sensor. These 3-axis reading will be used by Levee D,A. Module 
 *       to detect if there is any change in levee-infrastructure's co-ordinate because of any break-down.  
 * Params: void, nothing 
 * Returns: boolean, true or false
 */

boolean fetchADXL3353AxisAccelerometerReading( void )
{

  boolean isSensorActive = false;   

  /** 
   * Fetch the values from sensors 
   */

  unsigned long tSensorsXReading = analogRead(sensorXPin);
  unsigned long tSensorsYReading = analogRead(sensorYPin);
  unsigned long tSensorsZReading = analogRead(sensorZPin);

  /* Check if sensor is active or plugged out! */
  if ( ( tSensorsXReading + tSensorsYReading + tSensorsZReading ) > 0 )
  {
    isSensorActive = true;

    /* Send readings to serial terminal! */
    if(ISONBOARDDEBUGENABLE) 
    {
      daLineCount++; 
      Serial.print( daLineCount  + TAB + tSensorsXReading + TAB + tSensorsYReading  + TAB + tSensorsZReading);
      Serial.println();
    }

    /* Generate an active event to send to D.A. module */
    generateEventNotification(
    String( "digital" ), 
    String( "ADXL335 3-Axis Accelerometer Sensors" ),
    String( "It gives a reading of acceleration per 3-axis" ),
    String( "active" ),
    String( "changed" ),
    String( "compulsory" ),
    tSensorsXReading,
    tSensorsYReading,
    tSensorsZReading,
    String( "A0,A1,A2,A3,AIN,AOUT" )
      );


    /* Simple onboard event detection trigger. This will send XML dump to D.A. at later stage! */
    if(
    ((tSensorsXReading - tSensorsXReading) > tolerance || (tSensorsXReading - tSensorsXReading) < ( tolerance * -1 )) ||
      ((tSensorsYReading - tSensorsYReading) > tolerance || (tSensorsYReading - tSensorsYReading) < ( tolerance * -1 )) ||
      ((tSensorsZReading - tSensorsZReading) > tolerance || (tSensorsZReading - tSensorsZReading) < ( tolerance * -1 ))  
      )
    {

      /* Perform some actions upon detected event like flash LED or Buzz the speaker */
      digitalWrite(speakerPin, HIGH);
      delay(MEDIUMDELAY);
      digitalWrite(LEDPin, HIGH);
      digitalWrite(speakerPin, LOW);
      delay(MEDIUMDELAY);
      digitalWrite(LEDPin, LOW);
    }
  }
  else 
  {
    /* Send in inactive event to D.A.*/
    /* Generate an active event to send to D.A. module */
    generateEventNotification(
    String( "digital" ), 
    String( "ADXL335 3-Axis Accelerometer Sensors" ),
    String( "It gives a reading of acceleration per 3-axis" ),
    String( "inactive" ),
    String( "notchanged" ),
    String( "compulsory" ),
    -1,
    -1,
    -1,
    String( "A0,A1,A2,A3,AIN,AOUT" )
      );

    return false;
  }


  return true; 
}

/**
 * 
 * Desc: To setup Ardunio hardware, Kind of RC.D...
 * Params: void, nothing  
 * Returns: void, nothing
 * 
 */

void setup( void )
{
  /* Initialize the serial communications */
  Serial.begin( PORTBAUDRATE );

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
 * 
 * Desc:  This is nothing but main() for Arduino! 
 * Params: void, nothing  
 * Returns: void, nothing
 * 
 */

void loop( void )
{

  /**
   * Flush the serial buffer if any (optional) and start reading serial data
   */

  Serial.flush();


  /** Fetch reading from ADXL335 sensor */
  if( !fetchADXL3353AxisAccelerometerReading() )
    if( ISONBOARDDEBUGENABLE ) 
      Serial.print( "Error! Arduino board couldn't fetch desire reading from ADXL335 sensor.\n" );


  /* Keep the speaker at low */
  digitalWrite(speakerPin, LOW); 
  delay(MEDIUMDELAY);
}





















