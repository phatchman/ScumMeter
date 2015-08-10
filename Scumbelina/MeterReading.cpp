#include "HardwareConfig.h"
#include "MeterReading.h"
#include <stdlib.h>
#include <string.h>

//#define max(a,b)    (((a) > (b)) ? (a) : (b))
//#define min(a,b)    (((a) < (b)) ? (a) : (b))

const char MeterReading::unset_value[] PROGMEM = "--.--" ;

MeterReading::MeterReading() : MeterReading("00.00")
{
}

MeterReading::MeterReading(const char* str)
{
	parseString(str);
}

MeterReading::MeterReading(const MeterReading& rhs)
{
	strlcpy(value, rhs.value, sizeof(value));
}


bool MeterReading::operator<(const MeterReading& rhs) const
{
	return strcmp(value, rhs.value) < 0;
}

bool MeterReading::operator>(const MeterReading& rhs) const
{
	return strcmp(value, rhs.value) > 0;
}

bool MeterReading::operator==(const MeterReading& rhs) const
{
	return strcmp(value, rhs.value) == 0;
}

bool MeterReading::operator<(const char* rhs) const
{
	return strcmp(value, rhs) < 0;
}

bool MeterReading::operator>(const char* rhs) const
{
	return strcmp(value, rhs) > 0;
}

bool MeterReading::operator==(const char* rhs) const
{
	return strcmp(value, rhs) == 0;
}


const char* MeterReading::parseString(const char* str)
{
	// First remove any leading rubbish
	while (*str) {
		if ((*str >= '0' && *str <= '9') || *str == '.') break;
		str++;
	}

	uint8_t dec_pos = METER_READING_STRLEN;
	uint8_t i;
	// Find where the decimal place is in the string.
	for (i = 0; i < METER_READING_STRLEN; i++) {
		if (str[i] == '.' || str[i] == 0) {
			dec_pos = i;
			break;
		}
	}

	for (i = 0; i < METER_READING_NUM_INT - dec_pos; i++) {
		value[i] = '0';
	}

	uint8_t str_idx = max(0, dec_pos - METER_READING_NUM_INT);

	for (; i < METER_READING_STRLEN; i++) {
		char val = str[str_idx];
		if (i == METER_READING_NUM_INT) {
			// always need a dec place at pos 2.
			// if also a dec point on the input, increment the string.
			if (val == '.' || val == 0) {
				value[i] = '.';
				str_idx++;
			}
			else {
				value[i] = METER_READING_ERROR_DECPL;
			}
		}
		else if (val == 0) {
			// Null string. Don't increment the str_idx so
			// we keep reading null until done.
			value[i] = '0';
		}
		else if (val >= '0' && val <= '9') {
			// This is a normal digit. just copy it accross.
			value[i] = str[str_idx++];
		}
		else if (val == ' ') {
			value[i] = '0';
			str_idx++;
		}
		else {
			// This is an invalid digit. 
			value[i] = METER_READING_ERROR_DIGIT;
			str_idx++;
		}
		value[METER_READING_STRLEN - 1] = 0;
	}
	return value;
}


const char* MeterReading::toString() const
{
	return value;
}

const char* MeterReading::toString(char* buffer, uint8_t num_int, uint8_t num_dec, bool spacePad, bool is_set) const
{
	num_int = max(0, min(METER_READING_NUM_INT, num_int));
	num_dec = max(0, min(METER_READING_NUM_DEC, num_dec));

	uint8_t start = METER_READING_NUM_INT - num_int;
	uint8_t len = num_int + num_dec + 1;
	
	// Allow space for the decimal place
	if (num_dec > 0) len++;

	if (is_set)
		strlcpy(buffer, value + start, len);
	else
		strlcpy_P(buffer, unset_value + start, len);

	if (spacePad) {
		// Space pad for display purposes
		buffer[len - 1] = ' ';
		buffer[len] = 0;
	}
	return buffer;
}

