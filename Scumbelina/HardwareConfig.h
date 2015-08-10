#ifndef _Hardware_Config_h
#define _Hardware_Config_h

// Remove the display
// Leaves more memory to allow debugging of other components
// #define NO_DISPLAY

//
// DESCRIPTION:
//
// This header file has two purposes:
// (1) Hide most of the differences between the arduino and win32 build environments
// (2) Create all of the Global classes and allow them to be switched
//     out for other classes as required.
//


#ifndef ARDUINO
//#define _USE_32BIT_TIME_T
#include <time.h>
#include <Windows.h>
typedef unsigned char byte;

// No progmem strings.
#define F(x) x
typedef char __FlashStringHelper;
#define PROGMEM
#define strlcpy_P strlcpy

// no gcc extensions
#define __attribute__(x)
#include "..\SimulatedHardware\eeprom.h"
#include "..\SimulatedHardware\SoftwareSerial.h"
#include "..\SimulatedHardware\SimulatedSerialMeter.h"
extern class SimulatedSerialMeter Serial1;
// Use NULL Serial if no serial output is required.
//#include "..\SimulatedHardware\\NullSerial.h"
//extern class NullSerial Serial;
#include "..\SimulatedHardware\FileEmulatedSerial.h"
extern class FileEmulatedSerial Serial;

typedef class FTOLED OLED;


// 
// Time.h equivalent functions
//


// use MILLIS_ADJ() to test any time travel operations.
extern unsigned long _millis_ADJ;

typedef struct  { 
	uint8_t Second; 
	uint8_t Minute; 
	uint8_t Hour; 
	uint8_t Wday;   // day of week, sunday is day 1
	uint8_t Day;
	uint8_t Month; 
	uint8_t Year;   // offset from 1970; 
} 	tmElements_t, TimeElements, *tmElementsPtr_t;

#define  tmYearToCalendar(Y) ((Y) + 1970)  // full four digit year 
#define  CalendarYrToTm(Y)   ((Y) - 1970)
#define  tmYearToY2k(Y)      ((Y) - 30)    // offset is from 2000
#define  y2kYearToTm(Y)      ((Y) + 30)   

typedef enum {timeNotSet, timeNeedsSync, timeSet
}  timeStatus_t ;

inline unsigned long millis() {
	return GetTickCount() + _millis_ADJ;
}

inline void MILLIS_ADJ(const unsigned long val) {
	_millis_ADJ = val;
}

inline time_t now() {
	time_t t;
	return time(&t);
}

inline int	hour(time_t t) {
	struct tm* now_tm = localtime(&t);
	return now_tm->tm_hour;
}

inline int	minute(time_t t) {
	struct tm* now_tm = localtime(&t);
	return now_tm->tm_min;
}

inline int	second(time_t t) {
	struct tm* now_tm = localtime(&t);
	return now_tm->tm_sec;
}

inline int	year(time_t t) {
	struct tm* now_tm = localtime(&t);
	return now_tm->tm_year + 1900;
}

inline int	month(time_t t) {
	struct tm* now_tm = localtime(&t);
	return now_tm->tm_mon + 1;
}

inline int	day(time_t t) {
	struct tm* now_tm = localtime(&t);
	return now_tm->tm_mday;
}


inline void setTime(int hr, int min, int sec, int day, int month, int yr) {
	// do nothing for now.
}

inline timeStatus_t timeStatus() {
	return timeSet;
}

typedef time_t(*getExternalTime)();

inline void setSyncProvider(getExternalTime getTimeFunction) { 
}

//
// String functions
//
inline void strlcpy(char* dest, const char* src, int len) {
	strncpy(dest, src, len);
	dest[len-1] = 0;
}

// TMPBUF Debugging.
// Detect if TMPBUF is being used simultaneously.
//
#define WIDEN2(x) L ## x
#define WIDEN(x) WIDEN2(x)
#define WFILE WIDEN(__FILE__)

extern wchar_t TMPBUF_FILE[1024];
extern long TMPBUF_LINE;
extern bool TMPBUF_ACQUIRED;
#include <..\UnitTest\include\CppUnitTestAssert.h>
#include <string.h>
// ::TODO:: 
// TMPBUF debugging throwing up flase positives. 
// need ot trace through.
#if 1
#define TMPBUF_ACQUIRE { \
	wchar_t msg[1024]; \
	swprintf_s(msg, 1024, L"TMPBUF re-use detected [%s]:[%d]:[%d]", TMPBUF_FILE, TMPBUF_LINE, TMPBUF[0]); \
	Microsoft::VisualStudio::CppUnitTestFramework::Assert::IsFalse(TMPBUF_ACQUIRED, msg);\
	wcsncpy_s(TMPBUF_FILE, WFILE, 1024); \
	TMPBUF_LINE = __LINE__; \
	TMPBUF_ACQUIRED = true; \
}
#define TMPBUF_RELEASE { TMPBUF_ACQUIRED = false; wcscpy(TMPBUF_FILE, L"RELEASED"); TMPBUF_LINE = -1;}
#else
#define TMPBUF_ACQUIRE 
#define TMPBUF_RELEASE 
#endif

//
// Unneeded values on win32
//

#define SPI_FULL_SPEED 0
#else
#include <Arduino.h>
#include <avr\pgmspace.h>
#include <SoftwareSerial.h>
#include <DS3232RTC.h>

#define TMPBUF_ACQUIRE 
#define TMPBUF_RELEASE 


extern class SoftwareSerial Serial1;
#endif !ARDUINO

extern class BatteryMeterClass BatteryMeter;
extern class DataLoggerClass DataLogger;
extern class Button DisplayButton;
extern class Button MenuButton;
extern class AlarmLogClass AlarmLog;
extern class SerialCommandsClass SerialCommands;
extern class ConfigurationClass Configuration;
extern char TMPBUF[32];
#ifndef NO_DISPLAY
extern class ScumDisplayClass ScumDisplay;
#else 
#include "ScumNoDisplay.h"
extern class ScumNoDisplayClass ScumDisplay;
#endif
//extern time_t CURRENT_TIME;

#endif