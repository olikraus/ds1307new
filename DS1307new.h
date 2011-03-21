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
    uint8_t dow;			// day of week
    uint8_t day;
    uint8_t month;
    uint16_t year;
  
  
    uint8_t ctrl;

    void fillByCDN(uint16_t _cdn);
    void fillByTime2000(uint32_t _time2000);
    void fillByHMS(uint8_t h, uint8_t m, uint8_t s);
    void fillByYMD(uint8_t y, uint8_t m, uint8_t d);

    uint16_t ydn;		// day within the year (year day number)
    uint16_t cdn;		// days after 2000-01-01 (century day number)
    uint32_t time2000;	// seconds after 2000-01-01 00:00 (max value: 2136-02-07 06:28:15)
  
  private:
    uint8_t is_leap_year(void);
    void calculate_ydn(void);			// calculate ydn from year, month & day
    void calculate_cdn(void);			// calculate cdn from year & ydn
    void calculate_time2000(void);		// calculate time2000 from cdn, hour, minute & second

    uint16_t _corrected_year_day_number(void);
    void calculate_month_by_year_and_ydn(void);
    void calculate_day_by_month_year_and_ydn(void);

    uint8_t dec2bcd(uint8_t num);
    uint8_t bcd2dec(uint8_t num);
};

extern DS1307new RTC;

#endif
