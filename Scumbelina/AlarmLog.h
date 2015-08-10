// AlarmLog.h

#ifndef _ALARMLOG_h
#define _ALARMLOG_h

#include "HardwareConfig.h"
#define MAX_ALARMS 3
#define MAX_ALARM_TRIGGERS 5
#define AL_NO_TRIGGER 0
#define AL_MAXVOLT_TRIGGER 1
#define AL_MINVOLT_TRIGGER 2
#define AL_MAXAMP_TRIGGER 3
#define AL_BATTERYMETER_TRIGGER 4
#include <Time.h>
#include "BatteryMeter.h"

class Alarm
{
public:
	char message[18];
	time_t timestamp;
};

namespace Scumulator {
	class AlarmLogTests;
}

class AlarmLogClass
{
	friend class Scumulator::AlarmLogTests;
 protected:
	 byte alarmCount = 0;
	 bool newAlarm = false;
	 void substituteString(char* str, const char* val, const char replaceChar) const;

 public:
	 AlarmLogClass();
	 void init() { }
	 void reset() { clearAllAlarms(); }
	 void newMeasurement(const BatteryMeasurement& value);

	 void raiseAlarm(byte trigger, time_t timestamp, const __FlashStringHelper* msg, const char* limit = 0, const MeterReading *value = 0);
	 void clearAllAlarms();
	 byte getAlarmCount() { return alarmCount;  }
	 bool hasNewAlarm(bool resetNew = false) {
		 if (newAlarm) {
			 if (resetNew) {
				 newAlarm = false;
			 }
			 return true;
		 }
		 return false;
	 }
	 Alarm alarms[MAX_ALARMS];
	 bool triggered[MAX_ALARM_TRIGGERS]; // 2 * volt alarms, 1 * amp alarm, data logger and battery meter
};

#endif

