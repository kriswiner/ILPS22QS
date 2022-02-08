/* ILPS22QS_Basic_Dragonfy.ino
 *  
 *  Created by Kris WIner, 02/08/2022  
 *  
 *  Copyright 2022 Tlera Corporation.  All rights reserved.
 *  
 *  Sketch demonstrating the configuration and data output for ST's ILPS22QS baormeter
 * 
 *  Data configuration incldes data mode (full-scale range) and rate, as well as low-pass
 *  filter and averaging selection. Dat is output as absolute pressure in mBAR and temperature
 *  in degrees C as well as an estimate of altitude made from the ressure output.
 * 
 *  Sketch designed to run on an STM32L476RE (Dragonfly) development board but should work on
 *  almost any MCU with a hardware I2C port.
 * 
 *  Sketch may be used by anyone without permission with proper attribution.
 */
 
#include "ILPS22QS.h"
#include <RTC.h>

#define I2C_BUS          Wire                          // Define the I2C bus (Wire instance) you wish to use

I2Cdev                   i2c_0(&I2C_BUS);              // Instantiate the I2Cdev object and point to the desired I2C bus


#define SerialDebug true  // set to true to get Serial output for debugging
#define myLed 26

const char        *build_date = __DATE__;   // 11 characters MMM DD YYYY
const char        *build_time = __TIME__;   // 8 characters HH:MM:SS

/* Specify sensor parameters (sample rate, averaging, mode) 
   Choices are:
       PODR = P_1Hz, P_4Hz, P_10Hz P_25 Hz, P_50Hz, P_75Hz, P_100Hz, and P_200Hz
       AVG = avg_4, avg_8, avg_16, avg_32, avg_64, avg_128, abd avg_512
       MODE = mode1 (1260 FS) or mode2 (4060 FS)
       LPF = lpf_odr/4, lpf_odr/9
 */
// set pressure amd temperature output data rate,data averaging and mode
uint8_t PODR = P_1Hz, AVG = avg_16, MODE = mode1, LPF = lpf_odr9;     
uint8_t ILPS22Qstatus;
float Temperature, Pressure, altitude;

ILPS22Q ILPS22Q(&i2c_0);


// RTC set time using STM32L4 natve RTC class
uint8_t seconds, minutes, hours, day, month, year;
uint8_t Seconds, Minutes, Hours, Day, Month, Year;
volatile bool alarmFlag = false; // for RTC alarm interrupt


void setup() {
  Serial.begin(115200);
  delay(4000);

  // Configure led
  pinMode(myLed, OUTPUT);
  digitalWrite(myLed, HIGH); // start with led off

  I2C_BUS.begin(TWI_PINS_20_21); // set master mode 
  I2C_BUS.setClock(400000); // I2C frequency at 400 kHz  
  delay(100);
 
  i2c_0.I2Cscan();

  Serial.println("ILPS22QB barometer...");
  uint8_t ILPS22QS_ChipID = ILPS22Q.getChipID();
  Serial.print("LLPS22QS "); Serial.print("I AM "); Serial.print(ILPS22QS_ChipID, HEX); Serial.print(" I should be "); Serial.println(0xB4, HEX);
  delay(1000); 
  

  if(ILPS22QS_ChipID == 0xB4) // check if all I2C sensors have acknowledged
  {
   Serial.println("ILPS22QS is online..."); Serial.println(" ");
   
   digitalWrite(myLed, LOW);

   ILPS22Q.reset();
   delay(100);
   ILPS22Q.Init(PODR, AVG, MODE, LPF);  // Initialize ILPS22Q altimeter
   delay(100);

   digitalWrite(myLed, HIGH);
   
  }
  else 
  {
  if(ILPS22QS_ChipID != 0xB4) Serial.println(" ILPS22QS not functioning!");
  while(1){};
  }

 // Set the time
  SetDefaultRTC();
  
  /* Set up the RTC alarm interrupt */
  RTC.enableAlarm(RTC.MATCH_ANY); // alarm once a second 
  RTC.attachInterrupt(alarmMatch); // interrupt every time the alarm sounds
}

/* End of setup */

void loop() {

    if(alarmFlag) { // update RTC output (serial display) whenever the RTC alarm condition is achieved and the MPU9250 is awake
       alarmFlag = false;

    // Read RTC
   if(SerialDebug)
    {
    Serial.println("RTC:");
    Day = RTC.getDay();
    Month = RTC.getMonth();
    Year = RTC.getYear();
    Seconds = RTC.getSeconds();
    Minutes = RTC.getMinutes();
    Hours   = RTC.getHours();     
    if(Hours < 10) {Serial.print("0"); Serial.print(Hours);} else Serial.print(Hours);
    Serial.print(":"); 
    if(Minutes < 10) {Serial.print("0"); Serial.print(Minutes);} else Serial.print(Minutes); 
    Serial.print(":"); 
    if(Seconds < 10) {Serial.print("0"); Serial.println(Seconds);} else Serial.println(Seconds);  

    Serial.print(Month); Serial.print("/"); Serial.print(Day); Serial.print("/"); Serial.println(Year);
    Serial.println(" ");
    }

    // get pressure and temperature from the ILPS22QB
    ILPS22Qstatus = ILPS22Q.status();

    if(ILPS22Qstatus & 0x01) { // if new pressure data available
    Pressure = (float) ILPS22Q.readAltimeterPressure()/4096.0f;
    Temperature = (float) ILPS22Q.readAltimeterTemperature()/100.0f; 
    
    altitude = 145366.45f*(1.0f - pow((Pressure/1013.25f), 0.190284f)); 

     if(SerialDebug) {
      Serial.print("Altimeter temperature = "); Serial.print( Temperature, 2); Serial.println(" C"); // temperature in degrees Celsius  
      Serial.print("Altimeter temperature = "); Serial.print(9.0f*Temperature/5.0f + 32.0f, 2); Serial.println(" F"); // temperature in degrees Fahrenheit
      Serial.print("Altimeter pressure = "); Serial.print(Pressure, 2);  Serial.println(" mbar");// pressure in millibar
      Serial.print("Altitude = "); Serial.print(altitude, 2); Serial.println(" feet");
     }

     //For data plotting
//     Serial.print(millis()/1000);Serial.print(",");
//     Serial.print(Pressure, 2); Serial.print(","); Serial.print(Temperature, 2); Serial.print(","); Serial.println(altitude, 2);  
    }

    digitalWrite(myLed, LOW); delay(1); digitalWrite(myLed, HIGH);   
  }

}  /*  End of main loop */


void alarmMatch()
{
  alarmFlag = true;
}

void SetDefaultRTC()  // Sets the RTC to the FW build date-time...Courtesy Greg Tomasch
{
  char Build_mo[3];

  // Convert month string to integer

  Build_mo[0] = build_date[0];
  Build_mo[1] = build_date[1];
  Build_mo[2] = build_date[2];

  String build_mo = Build_mo;

  if(build_mo == "Jan")
  {
    month = 1;
  } else if(build_mo == "Feb")
  {
    month = 2;
  } else if(build_mo == "Mar")
  {
    month = 3;
  } else if(build_mo == "Apr")
  {
    month = 4;
  } else if(build_mo == "May")
  {
    month = 5;
  } else if(build_mo == "Jun")
  {
    month = 6;
  } else if(build_mo == "Jul")
  {
    month = 7;
  } else if(build_mo == "Aug")
  {
    month = 8;
  } else if(build_mo == "Sep")
  {
    month = 9;
  } else if(build_mo == "Oct")
  {
    month = 10;
  } else if(build_mo == "Nov")
  {
    month = 11;
  } else if(build_mo == "Dec")
  {
    month = 12;
  } else
  {
    month = 1;     // Default to January if something goes wrong...
  }

  // Convert ASCII strings to integers
  day     = (build_date[4] - 48)*10 + build_date[5] - 48;  // ASCII "0" = 48
  year    = (build_date[9] - 48)*10 + build_date[10] - 48;
  hours   = (build_time[0] - 48)*10 + build_time[1] - 48;
  minutes = (build_time[3] - 48)*10 + build_time[4] - 48;
  seconds = (build_time[6] - 48)*10 + build_time[7] - 48;

  // Set the date/time

  RTC.setDay(day);
  RTC.setMonth(month);
  RTC.setYear(year);
  RTC.setHours(hours);
  RTC.setMinutes(minutes);
  RTC.setSeconds(seconds);
}
  
