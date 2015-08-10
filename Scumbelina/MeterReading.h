#pragma once
#include<inttypes.h>


//
// DESCRIPTION:
//
// This is a 3.3 fixed floating point class that stores numbers
// as a simple string.
//
// While it means using 7 bytes per number and no math operations
// It does save on 3Kb of SRAM required for floating point support.
// For our purposes, all we need to know is if one number is 
// greater or less than the other number. We need to store and transmit as a string
// so conversion back and forth to a floating point has little value
//

class MeterReading
{
#define METER_READING_NUM_INT 2
#define METER_READING_NUM_DEC 2
#define METER_READING_ERROR_DIGIT '*'
#define METER_READING_ERROR_DECPL 'x'

#define METER_READING_STRLEN METER_READING_NUM_INT + METER_READING_NUM_DEC + 2 

public:

	MeterReading();
	MeterReading(const char* str);
	MeterReading(const MeterReading& rhs);

	bool operator<(const MeterReading& rhs) const;
	bool operator>(const MeterReading& rhs) const;
	bool operator==(const MeterReading& rhs) const;

	bool operator<(const char* rhs) const;
	bool operator>(const char* rhs) const;
	bool operator==(const char* rhs) const;

	const char* parseString(const char* str);
	const char* toString() const;
	const char* toString(char* buffer, uint8_t num_int, uint8_t num_dec, bool spacePad = false, bool is_set = true) const;

private:
	char value[METER_READING_STRLEN];
	static const char unset_value[METER_READING_STRLEN];
};

