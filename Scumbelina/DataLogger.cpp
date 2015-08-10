#include "DataLogger.h"
#include <DS3232RTC.h>
#include <Time.h>
#include "AlarmLog.h"
#include "Configuration.h"
// Error messages
extern const char ERROR_NO_SD_CARD[];
const char ERROR_NO_SD_CARD[] PROGMEM = "Insert SD"; 
static const char ERROR_NO_FILE[] PROGMEM = "SD r/w Error"; 
static const byte FILENAME_YEAR_START = 0;
char DataLoggerClass::loggingFilename[] = "yyyymm.csv";	// Modifyable.

#define ALTERNATIVE_LOGGER

DataLoggerClass::~DataLoggerClass() {
}

void DataLoggerClass::init()
{
	rtcInit();
	clearError();
	if (!is_initialised) {
		if (!SD.begin(cs_pin, SPI_FULL_SPEED)) {
			setError(ERROR_NO_SD_CARD);
			return;
		}
	}
	is_initialised = true;
}

void DataLoggerClass::rtcInit() {
	// Turn off unneeded outputs to save battery
	RTC.set33kHzOutput(false);
	RTC.setSQIMode(sqiModeNone);

	// set the time sync off the RTC.
	setSyncProvider(RTC.get);
}

void DataLoggerClass::setLogFileName(char* fn, uint16_t year, uint8_t month)
{
	fn[FILENAME_YEAR_START + 0] = (year / 1000) + '0';
	fn[FILENAME_YEAR_START + 1] = ((year / 100) % 10) + '0';
	fn[FILENAME_YEAR_START + 2] = ((year / 10) % 10) + '0';
	fn[FILENAME_YEAR_START + 3] = (year % 10) + '0';
	fn[FILENAME_YEAR_START + 4] = (month / 10) + '0';
	fn[FILENAME_YEAR_START + 5] = (month % 10) + '0';
}

void DataLoggerClass::setLogFileName(uint16_t year, uint8_t month) {
	setLogFileName(loggingFilename, year, month);
}

// only call if initialised
// creates a log file for each year/month
void DataLoggerClass::initLogFile(time_t timestamp) {
	initLogFile(year(timestamp), month(timestamp));
}

void DataLoggerClass::initLogFile(uint16_t year, uint8_t month) {
	if (logYear == year && logMonth == month){
		// log file already open
		return;
	}
	else {
		log_file.close();
		setLogFileName(year, month);
		
		if (!log_file.open(loggingFilename, O_RDWR | O_CREAT | O_AT_END)) {
			checkWriteError(-1);
			return;
		}
		logYear = year;
		logMonth = month;
	}
}


void DataLoggerClass::setTime(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second) {
	tmElements_t tm;
	tm.Year = CalendarYrToTm(year);
	tm.Month = month;
	tm.Day = day;
	tm.Hour = hour;
	tm.Minute = minute;
	tm.Second = second;
	RTC.write(tm);
	setSyncProvider(RTC.get);
}

void DataLoggerClass::setError(const char* msg) {
	AlarmLog.raiseAlarm(AL_NO_TRIGGER, now(), (__FlashStringHelper*)msg);
}

void DataLoggerClass::clearError() {
	has_write_error = false;
}

void DataLoggerClass::resetLog() {
	log_file.close();
	logYear = -1;
	logMonth = -1;
}

void DataLoggerClass::reset() {
	// This will cause the SD card to be reinitialised as well.
	is_initialised = false;
	resetLog();
	init();
}

void DataLoggerClass::checkWriteError(int8_t val) {
	if (val < 0)
	{
		if (!has_write_error) {
			// Don't keep logging write_errors
			has_write_error = true;
			setError(ERROR_NO_FILE);
		}
	}
	// Don't reset here as not all functions consistently return -1 on write errors
	// needs ot be reset to log new errors.
}

void DataLoggerClass::newMeasurement(const BatteryMeasurement& value) {
	
	if (!value.is_set) return;
	time_t currentMillis = millis();
	if (is_initialised && 
		value.is_set &&
		currentMillis - lastMillis >= Configuration.getConfig().loggingFrequency) {

		lastMillis = currentMillis;

		initLogFile(value.timestamp);
#ifndef ALTERNATIVE_LOGGER
		// format is timestamp,volts,amps,power
		//           1         2         3 
		// 01234567890123456789012345678901234
		// TTTTTTTTTT,VV.VV,AA.AA,PP.PP\0
		TMPBUF_ACQUIRE;
		ultoa(value.timestamp, TMPBUF, 10);
		TMPBUF[10] = ',';
		strlcpy(&TMPBUF[11], value.volts.toString(), METER_READING_STRLEN);
		TMPBUF[16] = ',';
		strlcpy(&TMPBUF[17], value.amps.toString(), METER_READING_STRLEN);
		TMPBUF[22] = ',';
		strlcpy(&TMPBUF[23], value.power.toString(), METER_READING_STRLEN);
		// strlcpy will null terminate.
		checkWriteError(log_file.println(TMPBUF));
		TMPBUF_RELEASE;
#else
	// NOTE the code below is smaller.
	// so keeping it in case we need to shrink ~100 bytes.
		checkWriteError(log_file.print(value.timestamp));
		checkWriteError(log_file.print(','));
		checkWriteError(log_file.print(value.volts.toString()));
		checkWriteError(log_file.print(','));
		checkWriteError(log_file.print(value.amps.toString()));
		checkWriteError(log_file.print(','));
		checkWriteError(log_file.println(value.power.toString()));
#endif

		log_file.flush();
	}
}

//
// Note that this requires a newline at the end of the line
// to ensure the last value is captured correctly.
//
char* DataLoggerClass::getCsvString(char* buf, int startPos, int& nextPos) {
	char* c = &buf[startPos];
	char* b = c;
	while (*c) {
		if (*c == ',' || *c == '\n') {
			*c = 0;
			nextPos = startPos + (c - b) + 1;
			return b;
		}
		c++;
	}
	nextPos = startPos;
	return 0;
}

//
// ::DESCRIPTION::
// startDate and endDate should be in yyyymm format.
// start date of 0 and end date of 0 means most recent file. 
// a start date of 0 means all files before the end date
// An end date of 999999 means all files after start date
// Thus start date of 0 and end date of 999999 means all files.
//
void DataLoggerClass::dumpTo(uint32_t startDate, uint32_t endDate, updateEventHandler handler) {
	if((startDate == 0) && (endDate == 0)) {
		// open the file for the current year/month
		initLogFile(now());
		log_file.seekSet(0);
		dumpLogFile(handler);
	}
	else {

		// Set up two filenames a start and an end one.
		// then do a string compare on the files in the directory
		// to see if file is between the two.
		char startFileName[sizeof(loggingFilename)];
		char endFileName[sizeof(loggingFilename)];
		strlcpy(startFileName, loggingFilename, sizeof(startFileName));
		strlcpy(endFileName, loggingFilename, sizeof(endFileName));
		setLogFileName(startFileName, startDate / 100, startDate % 100);
		setLogFileName(endFileName, endDate / 100, endDate % 100);

		// There's no guarantee files will be sent in order, but it's
		// pretty likely as files are created in order
		log_file.close();
		SD.chdir(true);	// The chdir forces a reset for the openNext

		while (log_file.openNext(SD.vwd())) {
			if (log_file.isFile()) {
				char sfn[13]; // short file name
				// Note that dumpLogFile uses TMPBUF
				// so cannot be used for sfn
				log_file.getSFN(sfn);
				if (strcmp(sfn, startFileName) >= 0 &&
					strcmp(sfn, endFileName) <= 0) {
					dumpLogFile(handler);
				}
			}
		}
	}
	resetLog();
}

void DataLoggerClass::dumpLogFile(updateEventHandler handler) {
	// Caller must set log position.
	// Note below is safe against missing values. Will just return null.
	char* timestamp;
	char* volts;
	char* amps;
	char* power;
	int pos;
	char* values[4];

	TMPBUF_ACQUIRE;
	while (log_file.fgets2(TMPBUF, sizeof(TMPBUF)) > 0) {
		pos = 0;

		timestamp = getCsvString(TMPBUF, pos, pos);
		volts = getCsvString(TMPBUF, pos, pos);
		amps = getCsvString(TMPBUF, pos, pos);
		power = getCsvString(TMPBUF, pos, pos);
		TMPBUF_RELEASE;	// Release before event handler is called.
		if (!timestamp || !volts || !amps || !power) {

			handler(0, 0, -1);
			break;
		}
		else {
			values[0] = timestamp;
			values[1] = volts;
			values[2] = amps;
			values[3] = power;
			handler(values, 4, 0);
		}
	}
					
	TMPBUF_RELEASE; // Release for the case we didn't read anything.
}


void DataLoggerClass::logRawData(const char* buf, int len) {
	resetLog();
	log_file.open("raw.log", O_RDWR | O_CREAT | O_AT_END);
	log_file.write(buf, len);
	log_file.close();
}