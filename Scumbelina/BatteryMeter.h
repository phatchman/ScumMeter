#ifndef _BATTERYMETER_h
#define _BATTERYMETER_h
#include "HardwareConfig.h"
#include "MeterReading.h"
#include <Time.h>
class BatteryMeasurement
{
public:
	typedef enum { Max, Min, Now } MeasurementType;

	MeasurementType type;
	time_t timestamp;
	MeterReading volts;
	MeterReading amps;
	MeterReading power;
	bool is_set;

	BatteryMeasurement() {
		is_set = false;
	}

	BatteryMeasurement(MeasurementType typeVal, time_t timestampVal, MeterReading& voltVal, 
		MeterReading& ampVal, MeterReading& powerVal) :
		type(typeVal), volts(voltVal), amps(ampVal), power(powerVal), 
		timestamp(timestampVal), is_set(true) {
	}
};


class BatteryMeterClass
{
public:
	typedef void(*updateEventHandler)(const BatteryMeasurement&);

	BatteryMeterClass() { updateCallback = 0; }

	void init();
	void reset();
	void process();

	void measurementHandler(updateEventHandler handler);

	// Note that min/max timestamp only refers to Volts, not amps.
	BatteryMeasurement minVal;
	BatteryMeasurement maxVal;
	BatteryMeasurement nowVal;

protected:
	updateEventHandler updateCallback;
	unsigned long last_millis;

	void updateMaxMin(BatteryMeasurement& measure);
	void updated();
	typedef enum { Volts, Amps, Power } State;

	// The values currently being parsed
	MeterReading incoming_volts;
	MeterReading incoming_amps;
	MeterReading incoming_power;

private:
	char buf[32];	// Each message is 23 chars long.
					// As buf needs to be persisted we can't use TMPBUF
	char* buf_ptr; // Pointer to current position in buffer;
	uint8_t buf_avail;
	bool readMeter = true;
};


#endif _BATTERYMETER_h
