#include "BatteryMeter.h"
#include <math.h>
#include "Configuration.h"
#include "AlarmLog.h"
#include "DataLogger.h"
// :: TODO:: 
// make these static for the arduino version.
// same with the data logger errors.
extern const char ERROR_NO_READING[];
const char ERROR_NO_READING[] PROGMEM = "no meter";

static const int RESET_DELAY = 3000;  // Show unset values for 3 seconds.
								// Not configurable. It's just to make
								// the dispaly look nice on a reset.
//#define DEMO_MODE
#define LOG_RAW_DATA

void BatteryMeterClass::init()
{
	buf_ptr = buf;
	buf_avail = sizeof(buf);
	incoming_volts = incoming_amps = incoming_power = "";
	last_millis = millis() - Configuration.getConfig().meterPollFrequency + RESET_DELAY;
#ifdef DEMO_MODE
	srand(CURRENT_TIME);
#endif
}
#ifdef DEMO_MODE
const char* randomReading() {
	static char buf[5];
	buf[0] = rand() % 2 + '0';
	buf[1] = rand() % 10 + '0';
	buf[2] = '.';
	buf[3] = rand() % 10 + '0';

	return buf;
}
#endif

// Every 'x' seconds, send the
// GVCS command to the battery meter over the serial port
// to retrieve the read values for volts, amps and power.
//
void BatteryMeterClass::process() {
	// Dump if less than 2 available
	// because need to store at least 
	// 1 char and then the terminating null
	if (buf_avail <= 2) {
		// dump the buffer if haven't found 
		// anything to parse.
		buf_avail = sizeof(buf);
		buf_ptr = buf;
	}
#ifdef DEMO_MODE
	if (millis() - last_millis > Configuration.getConfig().meterPollFrequency) {
		MeterReading v(randomReading());
		MeterReading a(randomReading());
		MeterReading p(randomReading());

		last_millis = millis();
		nowVal = BatteryMeasurement(BatteryMeasurement::Now, CURRENT_TIME, v, a, p);
		updateMaxMin(nowVal);
		updated();
	}
#else
	uint8_t nbytes = (uint8_t) Serial1.available();
	if (nbytes > 0) {
		nbytes = Serial1.readBytes(buf_ptr, buf_avail - 1);
#ifdef LOG_RAW_DATA
		DataLogger.logRawData(buf_ptr, nbytes);
#endif
		buf_ptr[nbytes] = 0;
		buf_avail -= nbytes;
		buf_ptr += nbytes;
	}
	else {
		unsigned long current_millis = millis();
		if (current_millis - last_millis > Configuration.getConfig().meterPollFrequency) {
			if (!readMeter && !AlarmLog.triggered[AL_BATTERYMETER_TRIGGER]) {
				AlarmLog.raiseAlarm(AL_BATTERYMETER_TRIGGER, now(), (__FlashStringHelper*)ERROR_NO_READING);
			}
			Serial1.println(F("GVCW"));
			readMeter = false;
			last_millis = current_millis;
		}
		return;
	}

	char* s = buf; // Start of buffer
	char* c = buf; // Current position in buffer
	while (*c != 0) {
		if (*c == 'V') {
			// Found the Volt symbol. Null terminate the buffer here
			*c = 0;
			incoming_volts.parseString(s);
			s = c + 1;
		}
		else if (*c == 'A') {
			*c = 0;
			incoming_amps.parseString(s);
			s = c + 1;
		}
		else if (*c == 'W') {
			*c = 0;
			incoming_power.parseString(s);
			nowVal = BatteryMeasurement(BatteryMeasurement::Now, now(), incoming_volts, incoming_amps, incoming_power);
			readMeter = true;
			incoming_volts = incoming_amps = incoming_power = "";
			s = c + 1;
			updateMaxMin(nowVal);
			updated();
		}
		c++;
	}
	// If buffer is not fully parsed, copy remainder to start of buffer
	if (*s) {
		if (s != buf) {
			size_t n = strlen(s);
			memmove(buf, s, n + 1);

			buf_ptr = buf + n;
			buf_avail = sizeof(buf) - n;
		}
		// otherwise we've got leftover chars in the buffer
		// next read will append buffer
	}
	else {
		// buffer is empty and no unparsed chars waiting.
		// so reset to start of buffer
		buf_ptr = buf;
		buf_avail = sizeof(buf);
	}
#endif
}


void BatteryMeterClass::reset() {
	minVal.is_set = false;
	maxVal.is_set = false;
	nowVal.is_set = false;
	AlarmLog.triggered[AL_BATTERYMETER_TRIGGER] = false;
	last_millis = millis() - Configuration.getConfig().meterPollFrequency + RESET_DELAY;
	updated();
}

void BatteryMeterClass::measurementHandler(updateEventHandler handler) {
	updateCallback = handler;
}

void BatteryMeterClass::updated() {
	if (updateCallback) updateCallback(nowVal);
}

void BatteryMeterClass::updateMaxMin(BatteryMeasurement& measure) {

	if (!minVal.is_set || minVal.volts > nowVal.volts) {
		minVal.volts = nowVal.volts;
		minVal.timestamp = nowVal.timestamp;
	}
	if (!minVal.is_set || minVal.amps > nowVal.amps) {
		minVal.amps = nowVal.amps;
		minVal.is_set = true;	// NOTE: This must be set on the last value stored.
								// If we start recording power again, this should be removed
	}
	//if (!minVal.is_set || minVal.power > nowVal.power) {
	//	minVal.power = nowVal.power;
	//	minVal.is_set = true;
	//}

	if (!maxVal.is_set || maxVal.volts < nowVal.volts) {
		maxVal.volts = nowVal.volts;
		maxVal.timestamp = nowVal.timestamp;
	}
	if (!maxVal.is_set || maxVal.amps < nowVal.amps) {
		maxVal.amps = nowVal.amps;
		maxVal.is_set = true;	// NOTE: This must be set on the last value stored.
								// If we start recording power again, this should be removed
	}
	// We don't use min/max power figures
	// so no need to record them. Save some memory.
	//if (!maxVal.is_set || maxVal.power < nowVal.power) {
	//	maxVal.power = nowVal.power;
	//	maxVal.is_set = true;
	//}
}

