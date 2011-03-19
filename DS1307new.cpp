// #############################################################################
// #
// # Scriptname : DS1307new.cpp
// # Author     : Peter Schmelzer
// # Contributor: Oliver Kraus
// # contact    : info@schmelle2.de
// # Date       : 2010-11-01
// # Version    : 1.00
// # License    : cc-by-sa-3.0
// #
// # Description:
// # The DS1307new Library
// # 
// #############################################################################
// *********************************************
// INCLUDE
// *********************************************
#include "Wire.h"
#include "WProgram.h"
#include "DS1307new.h"

// *********************************************
// DEFINE
// *********************************************
#define DS1307_ID 0x68

// *********************************************
// Public functions
// *********************************************
DS1307new::DS1307new()
{
  Wire.begin();
}

void DS1307new::stopClock(void)         // set the ClockHalt bit high to stop the rtc
{
  Wire.beginTransmission(DS1307_ID);
  Wire.send(0x00);                      // Register 0x00 holds the oscillator start/stop bit
  Wire.endTransmission();
  Wire.requestFrom(DS1307_ID, 1);
  second = Wire.receive() | 0x80;       // save actual seconds and OR sec with bit 7 (sart/stop bit) = clock stopped
  Wire.beginTransmission(DS1307_ID);
  Wire.send(0x00);
  Wire.send(second);                    // write seconds back and stop the clock
  Wire.endTransmission();
}

void DS1307new::startClock(void)        // set the ClockHalt bit low to start the rtc
{
  Wire.beginTransmission(DS1307_ID);
  Wire.send(0x00);                      // Register 0x00 holds the oscillator start/stop bit
  Wire.endTransmission();
  Wire.requestFrom(DS1307_ID, 1);
  second = Wire.receive() & 0x7f;       // save actual seconds and AND sec with bit 7 (sart/stop bit) = clock started
  Wire.beginTransmission(DS1307_ID);
  Wire.send(0x00);
  Wire.send(second);                    // write seconds back and start the clock
  Wire.endTransmission();
}

// Aquire time from the RTC chip in BCD format and convert it to DEC
void DS1307new::getTime(void)
{
  Wire.beginTransmission(DS1307_ID);
  Wire.send(0x00);
  Wire.endTransmission();
  Wire.requestFrom(DS1307_ID, 7);       // request secs, min, hour, dow, date, month, year
  second = bcd2dec(Wire.receive() & 0x7f);// aquire seconds...
  minute = bcd2dec(Wire.receive());     // aquire minutes
  hour = bcd2dec(Wire.receive());       // aquire hours
  dow = bcd2dec(Wire.receive());        // aquire dow (Day Of Week)
  date = bcd2dec(Wire.receive());       // aquire date
  month = bcd2dec(Wire.receive());      // aquire month
  year = bcd2dec(Wire.receive());       // aquire year...
  year = year + 2000;                   // ...and assume that we are in 21st century!
}

// Set time to the RTC chip in BCD format
void DS1307new::setTime(void)
{
  year = year - 2000;                   // ...back to the root's :D
  Wire.beginTransmission(DS1307_ID);
  Wire.send(0x00);
  Wire.send(dec2bcd(second) | 0x80);   // set seconds (clock is stopped!)
  Wire.send(dec2bcd(minute));           // set minutes
  Wire.send(dec2bcd(hour) & 0x3f);      // set hours (24h clock!)
  Wire.send(dec2bcd(dow));              // set dow (Day Of Week)
  Wire.send(dec2bcd(date));             // set date
  Wire.send(dec2bcd(month));            // set month
  Wire.send(dec2bcd(year));             // set year
  Wire.endTransmission();
}

// Aquire data from the CTRL Register of the DS1307 (0x07)
void DS1307new::getCTRL(void)
{
  Wire.beginTransmission(DS1307_ID);
  Wire.send(0x07);                      // set CTRL Register Address
  Wire.endTransmission();
  Wire.requestFrom(DS1307_ID, 1);       // read only CTRL Register
  while(!Wire.available())
  {
    // waiting
  }
  ctrl = Wire.receive();                // ... and store it in ctrl
}

// Set data to CTRL Register of the DS1307 (0x07)
void DS1307new::setCTRL(void)
{
  Wire.beginTransmission(DS1307_ID);
  Wire.send(0x07);                      // set CTRL Register Address
  Wire.send(ctrl);                      // set CTRL Register
  Wire.endTransmission();
}

// Aquire data from RAM of the RTC Chip (max 56 Byte)
void DS1307new::getRAM(uint8_t rtc_addr, uint8_t * rtc_ram, uint8_t rtc_quantity)
{
  Wire.beginTransmission(DS1307_ID);
  rtc_addr &= 63;                       // avoid wrong adressing. Adress 0x08 is now address 0x00...
  rtc_addr += 8;                        // ... and address 0x3f is now 0x38
  Wire.send(rtc_addr);                  // set CTRL Register Address
  Wire.endTransmission();
  Wire.requestFrom(DS1307_ID, (int)rtc_quantity);
  while(!Wire.available())
  {
    // waiting
  }
  for(int i=0; i<rtc_quantity; i++)     // Read x data from given address upwards...
  {
    rtc_ram[i] = Wire.receive();        // ... and store it in rtc_ram
  }
}

// Write data into RAM of the RTC Chip
void DS1307new::setRAM(uint8_t rtc_addr, uint8_t * rtc_ram, uint8_t rtc_quantity)
{
  Wire.beginTransmission(DS1307_ID);
  rtc_addr &= 63;                       // avoid wrong adressing. Adress 0x08 is now address 0x00...
  rtc_addr += 8;                        // ... and address 0x3f is now 0x38
  Wire.send(rtc_addr);                  // set RAM start Address 
  for(int i=0; i<rtc_quantity; i++)     // Send x data from given address upwards...
  {
    Wire.send(rtc_ram[i]);              // ... and send it from rtc_ram to the RTC Chip
  }
  Wire.endTransmission();
}

// *********************************************
// Private functions
// *********************************************
// Convert Decimal to Binary Coded Decimal (BCD)
uint8_t DS1307new::dec2bcd(uint8_t num)
{
  return ((num/10 * 16) + (num % 10));
}

// Convert Binary Coded Decimal (BCD) to Decimal
uint8_t DS1307new::bcd2dec(uint8_t num)
{
  return ((num/16 * 10) + (num % 16));
}

// *********************************************
// Define user object
// *********************************************
class DS1307new RTC;
