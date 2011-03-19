// #############################################################################
// #
// # Scriptname : DS1307new.h
// # Author     : Peter Schmelzer
// # Contributor: Oliver Kraus
// # contact    : schmelle2@googlemail.com
// # Date       : 2010-11-01
// # Version    : 0.2
// # License    : cc-by-sa-3.0
// #
// # Description:
// # Headerfile for the DS1307new Library
// # 
// #############################################################################
// *********************************************
// DEFINE
// *********************************************
#ifndef DS1307new_h
#define DS1307new_h

// *********************************************
// INCLUDE
// *********************************************
#include "WProgram.h"

// *********************************************
// Library interface description
// *********************************************
class DS1307new
{
  public:
    DS1307new();
    void startClock(void);
    void stopClock(void);
    void setTime(void);
    void getTime(void);
    void getCTRL(void);
    void setCTRL(void);
    void getRAM(uint8_t rtc_addr, uint8_t * rtc_ram, uint8_t rtc_quantity);
    void setRAM(uint8_t rtc_addr, uint8_t * rtc_ram, uint8_t rtc_quantity);
    uint8_t second;
    uint8_t minute;
    uint8_t hour; 
    uint8_t dow;
    uint8_t date;
    uint8_t month;
    uint16_t year;
    uint8_t ctrl;

  private:
    uint8_t dec2bcd(uint8_t num);
    uint8_t bcd2dec(uint8_t num);
};

extern DS1307new RTC;

#endif
