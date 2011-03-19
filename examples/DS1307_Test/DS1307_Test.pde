// #############################################################################
// #
// # Scriptname : DS1307_Test.pde
// # Author     : Peter Schmelzer
// # Date       : 08.03.2011
// # Version    : 1.01
// # License    : cc-by-sa-3.0
// #
// # Description:
// # Test file for the DS1307new library. Assumes that you have a DS1307 
// # connected to the I2C-Bus of your Arduino and that it has a battery backup.
// # 
// #############################################################################
// *********************************************
// INCLUDE
// *********************************************
#include <Wire.h>                       // For some strange reasons, Wire.h must be included here
#include <DS1307new.h>

// *********************************************
// DEFINE
// *********************************************

// *********************************************
// VARIABLES
// *********************************************
uint16_t startAddr = 0x0000;            // Start address to store in the NV-RAM
uint16_t lastAddr;                      // new address for storing in NV-RAM
uint16_t TimeIsSet = 0xaa55;            // Helper that time must not set again

// *********************************************
// SETUP
// *********************************************
void setup()
{
  pinMode(2, INPUT);                    // Test of the SQW pin, D2 = INPUT
  digitalWrite(2, HIGH);                // Test of the SQW pin, D2 = Pullup on

/*
   PLEASE NOTICE: WE HAVE MADE AN ADDRESS SHIFT FOR THE NV-RAM!!!
                  NV-RAM ADDRESS 0x08 HAS TO ADDRESSED WITH ADDRESS 0x00=0
                  TO AVOID OVERWRITING THE CLOCK REGISTERS IN CASE OF
                  ERRORS IN YOUR CODE. SO THE LAST ADDRESS IS 0x38=56!
*/
  RTC.setRAM(0, (uint8_t *)&startAddr, sizeof(uint16_t));// Store startAddr in NV-RAM address 0x08 

/*
   Uncomment the next 2 lines if you want to SET the clock
   Comment them out if the clock is set.
   DON'T ASK ME WHY: YOU MUST UPLOAD THE CODE TWICE TO LET HIM WORK
   AFTER SETTING THE CLOCK ONCE.
*/
//  TimeIsSet = 0xffff;
//  RTC.setRAM(54, (uint8_t *)&TimeIsSet, sizeof(uint16_t));  

/*
  Control the clock.
  Clock will only be set if NV-RAM Address does not contain 0xaa.
  DS1307 should have a battery backup.
*/
  RTC.getRAM(54, (uint8_t *)&TimeIsSet, sizeof(uint16_t));
  if (TimeIsSet != 0xaa55)
  {
    RTC.stopClock();
    RTC.second = 0;                       // DON'T USE '00' IF YOU MEAN '0' SECONDS!!!
    RTC.minute = 46;
    RTC.hour = 12;
    RTC.dow = 1;                          // SUN=7, MON=1, TUE=2, WED=3, THU=4, FRI=5, SAT=6
    RTC.date = 1;
    RTC.month = 11;
    RTC.year = 2010;
    RTC.setTime();
    RTC.startClock();
    TimeIsSet = 0xaa55;
    RTC.setRAM(54, (uint8_t *)&TimeIsSet, sizeof(uint16_t));
  }

/*
   Control Register for SQW pin which can be used as an interrupt.
*/
  RTC.ctrl = 0x00;                      // 0x00=disable SQW pin, 0x10=1Hz,
                                        // 0x11=4096Hz, 0x12=8192Hz, 0x13=32768Hz
  RTC.setCTRL();

  Serial.begin(9600);
  Serial.println("DS1307 Testsketch");
  Serial.println("Format is \"hh:mm:ss dd-mm-yyyy DDD\"");
  Serial.println();
}

// *********************************************
// MAIN (LOOP)
// *********************************************
void loop()
{
  RTC.getTime();
  if (RTC.hour < 10)                    // correct hour if necessary
  {
    Serial.print("0");
    Serial.print(RTC.hour, DEC);
  } 
  else
  {
    Serial.print(RTC.hour, DEC);
  }
  Serial.print(":");
  if (RTC.minute < 10)                  // correct minute if necessary
  {
    Serial.print("0");
    Serial.print(RTC.minute, DEC);
  }
  else
  {
    Serial.print(RTC.minute, DEC);
  }
  Serial.print(":");
  if (RTC.second < 10)                  // correct second if necessary
  {
    Serial.print("0");
    Serial.print(RTC.second, DEC);
  }
  else
  {
    Serial.print(RTC.second, DEC);
  }
  Serial.print(" ");
  if (RTC.date < 10)                    // correct date if necessary
  {
    Serial.print("0");
    Serial.print(RTC.date, DEC);
  }
  else
  {
    Serial.print(RTC.date, DEC);
  }
  Serial.print("-");
  if (RTC.month < 10)                   // correct month if necessary
  {
    Serial.print("0");
    Serial.print(RTC.month, DEC);
  }
  else
  {
    Serial.print(RTC.month, DEC);
  }
  Serial.print("-");
  Serial.print(RTC.year, DEC);          // Year need not to be changed
  Serial.print(" ");
  switch (RTC.dow)                      // Friendly printout the weekday
  {
    case 1:
      Serial.print("MON");
      break;
    case 2:
      Serial.print("TUE");
      break;
    case 3:
      Serial.print("WED");
      break;
    case 4:
      Serial.print("THU");
      break;
    case 5:
      Serial.print("FRI");
      break;
    case 6:
      Serial.print("SAT");
      break;
    case 7:
      Serial.print("SUN");
      break;
  }
  Serial.print(" - Address in NV-RAM is: ");
  RTC.getRAM(0, (uint8_t *)&lastAddr, sizeof(uint16_t));
  Serial.print(lastAddr, HEX);
  lastAddr = lastAddr + 1;              // we want to use it as addresscounter for example
  RTC.setRAM(0, (uint8_t *)&lastAddr, sizeof(uint16_t));
  RTC.getRAM(54, (uint8_t *)&TimeIsSet, sizeof(uint16_t));
  if (TimeIsSet == 0xaa55)              // check if the clock was set or not
  {
    Serial.println(" - Clock was set!");
  }
  else
  {
    Serial.println(" - Clock was NOT set!");
  }    
  delay(1000);                          // wait a second
}
