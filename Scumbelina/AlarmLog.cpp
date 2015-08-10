// 
// 
// 

#include "AlarmLog.h"
#include "Configuration.h"
AlarmLogClass::AlarmLogClass() {
	clearAllAlarms();
}

void AlarmLogClass::raiseAlarm(byte trigger, time_t timestamp, const __FlashStringHelper* msg, const char* limit, const MeterReading *value) {
	if (alarmCount == MAX_ALARMS) {
		// Shuffle alarms up by one, deleting the oldest one.
		memcpy(alarms, &alarms[1], sizeof(Alarm) * (MAX_ALARMS - 1));
		alarmCount = MAX_ALARMS - 1;
	}
	alarms[alarmCount].timestamp = timestamp;
	strlcpy_P(alarms[alarmCount].message, (const char *) msg, sizeof(alarms[0].message));
	if (limit && value) {
		TMPBUF_ACQUIRE;
		value->toString(TMPBUF, 2, 1);

		substituteString(alarms[alarmCount].message, limit, '%');
		substituteString(alarms[alarmCount].message, TMPBUF, '#');
		TMPBUF_RELEASE;
	}
	alarmCount++;
	triggered[trigger] = true;
	newAlarm = true;
}

void AlarmLogClass::newMeasurement(const BatteryMeasurement& value) {
	if (!value.is_set) return;

	const char* maxVolts = Configuration.getConfig().maxVolts;
	const char* minVolts = Configuration.getConfig().minVolts;
	const char* maxAmps = Configuration.getConfig().maxAmps;

	if (!triggered[AL_MAXVOLT_TRIGGER] && value.volts > MeterReading(maxVolts)) {
		raiseAlarm(AL_MAXVOLT_TRIGGER, value.timestamp, F("Volt > %% [####] "), maxVolts, &value.volts);
	}
	if (!triggered[AL_MINVOLT_TRIGGER] && value.volts  < MeterReading(minVolts)) {
		raiseAlarm(AL_MINVOLT_TRIGGER, value.timestamp, F("Volt < %% [####] "), minVolts, &value.volts);
 	}
	if (!triggered[AL_MAXAMP_TRIGGER] && value.amps > MeterReading(maxAmps)) {
		raiseAlarm(AL_MAXAMP_TRIGGER, value.timestamp, F("Amp > %% [####] "), maxAmps, &value.amps);
	}
}

void AlarmLogClass::substituteString(char* str, const char* val, const char replaceChar)  const {
	while (*str) {
		if (*str == replaceChar) {
			if (*val == 0) return;
			*str = *val++;
		}
		str++;
	}
}

void AlarmLogClass::clearAllAlarms() {
	alarmCount = 0;
	newAlarm = true;	// True so display knows to clear as well.
	for (uint8_t i = 0; i < MAX_ALARM_TRIGGERS; i++) {
		triggered[i] = false;
	}
}