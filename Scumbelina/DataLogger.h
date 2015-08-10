// DataLogger.h

#ifndef _DATALOGGER_h
#define _DATALOGGER_h
#include "HardwareConfig.h"
#include "BatteryMeter.h"
#include "SdFat.h"
#include <stdio.h>


namespace Scumulator {
	class DataLoggerTests;
}

class DataLoggerClass
{
	friend class Scumulator::DataLoggerTests;

public:
	typedef void(*updateEventHandler)(char** values, int8_t numValues, int8_t errorCode);

protected:
	const byte cs_pin = 4;
	static char loggingFilename[];
	bool has_write_error = false;
	bool is_initialised = false;
	SdFat SD;
	File log_file;
	void setError(const char* msg);	// Note this must be a PROGMEM string
	void clearError();
	void rtcInit();
	void initLogFile(time_t timestamp);
	void initLogFile(uint16_t year, uint8_t month);
	void setLogFileName(uint16_t year, uint8_t month);
	void setLogFileName(char* fn, uint16_t year, uint8_t month);
	void checkWriteError(int8_t val);
	void dumpLogFile(updateEventHandler handler);
	char* getCsvString(char* buf, int startPos, int& nextPos);
	uint16_t logYear = -1;
	uint8_t logMonth = -1;
	time_t lastMillis = 0;
public:
	DataLoggerClass() {};
	~DataLoggerClass();
	
	void init();
	void setTime(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second);
	void newMeasurement(const BatteryMeasurement& value);
	//void dumpToSerial();
	void dumpTo(uint32_t startDate, uint32_t endDate, updateEventHandler handler);
	void logRawData(const char* buf, int len);
	void resetLog();
	void reset();
};

#endif

