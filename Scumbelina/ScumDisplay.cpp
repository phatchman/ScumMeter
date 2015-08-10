#include "ScumDisplay.h"
#include "DataLogger.h"
#include "AlarmLog.h"
#include "Configuration.h"


void ScumDisplayClass::init()
{
	oled.begin();
	oled.setDimGrayscaleTable();
	oled.setOrientation(ROTATE_180);

	initMainPage();
	keepAlive();
}

void ScumDisplayClass::process(void) {
	if (display_on) {
		if (millis() > turn_off_millis) {
			showDisplay(false);
		}
		else {
			alarmChanged = alarmChanged || AlarmLog.hasNewAlarm(true);
			if (currentPage == 0) {
				updateMainPage();
			}
			else {
				updateAlarmPage();
			}
		}
	}
}

void ScumDisplayClass::reset(void) {
	display_on = true;
	measurementChanged = true;
	alarmChanged = true;
	currentPage = 0;
	// re-initialise here because sd card errors can
	// slow down the speed of the SPI bus.
	// re-initting the oled display will restore it to full speed.
	init();
	updateMainPage();
}

void ScumDisplayClass::showNextPage() {
	currentPage = !currentPage;
	if (currentPage == 0) {
		initMainPage();
		measurementChanged = true;
		alarmChanged = true;
	}
	else {
		alarmChanged = true;
		initAlarmPage();
	}
	keepAlive();
}

void ScumDisplayClass::initMainPage() {
	oled.clearScreen();
	oled.selectFont(Arial_Black_16);
	oled.drawString(0, 100, F("SCUM - METER"), RED, BLACK);

	oled.drawString(0, 60, F("Now"), RED, BLACK);
	oled.drawString(0, 42, F("Max"), RED, BLACK);
	oled.drawString(0, 24, F("Min"), RED, BLACK);

	oled.drawString(42, 78, F("Volt"), RED, BLACK);
	oled.drawString(84, 78, F("Amp"), RED, BLACK);

}

void ScumDisplayClass::updateMainPage() {

	if (measurementChanged) {
		oled.selectFont(Arial_Black_16);
		TMPBUF_ACQUIRE;
		// Now Values
		oled.drawString(42, 60, BatteryMeter.nowVal.volts.toString(TMPBUF, 2, 2, true, BatteryMeter.nowVal.is_set), GREEN, BLACK);
		oled.drawString(86, 60, BatteryMeter.nowVal.amps.toString(TMPBUF, 2, 2, true, BatteryMeter.nowVal.is_set), GREEN, BLACK);

		// Max Values
		oled.drawString(42, 42, BatteryMeter.maxVal.volts.toString(TMPBUF, 2, 2, true, BatteryMeter.maxVal.is_set), RED, BLACK);
		oled.drawString(86, 42, BatteryMeter.maxVal.amps.toString(TMPBUF, 2, 2, true, BatteryMeter.maxVal.is_set), RED, BLACK);

		// Min Values
		oled.drawString(42, 24, BatteryMeter.minVal.volts.toString(TMPBUF, 2, 2, true, BatteryMeter.minVal.is_set), RED, BLACK);
		oled.drawString(86, 24, BatteryMeter.minVal.amps.toString(TMPBUF, 2, 2, true, BatteryMeter.minVal.is_set), RED, BLACK);

		oled.selectFont(Arial14);

		formatTime(TMPBUF, BatteryMeter.nowVal.timestamp);

		oled.drawString(2, 2, TMPBUF, WHITE, BLACK);
		TMPBUF_RELEASE;
	}
	if (alarmChanged)
	{
		if (AlarmLog.getAlarmCount() == 0) {
			oled.drawString(100, 2, F("OK "), BLUE, BLACK);
		}
		else {
			oled.drawString(100, 2, F("AL "), BLUE, BLACK);
		}
	}

	measurementChanged = false;
	alarmChanged = false;
}

void ScumDisplayClass::initAlarmPage() {
	oled.clearScreen();
	oled.selectFont(Arial_Black_16);
	oled.drawString(25, 100, F("ALARMS"), RED, BLACK);
	updateAlarmPage();
}

void ScumDisplayClass::updateAlarmPage() {
	if (alarmChanged) {
		oled.selectFont(Arial14);
		uint8_t yval = 82;
		TMPBUF_ACQUIRE;
		for (uint8_t i = 0; i < AlarmLog.getAlarmCount(); i++) {
			formatDateTime(TMPBUF, AlarmLog.alarms[i].timestamp);
			oled.drawString(2, yval, TMPBUF, RED, BLACK);
			oled.drawString(2, yval - 14, AlarmLog.alarms[i].message, RED, BLACK);
			yval -= 34;
		}
		TMPBUF_RELEASE;
	}
	alarmChanged = false;
}

void ScumDisplayClass::newMeasurement(const BatteryMeasurement& value) {
	// Not updating the display here. Waiting for the poll loop to call process().
	// Probably can just update it directly??
	measurementChanged = true;
}

void ScumDisplayClass::showDisplay(bool show) {
	oled.setDisplayOn(show);
	if (show) {
		// Bit hacky.. needs cleanup. If they turn the display on, then
		// reset the off timer.
		turn_off_millis = millis() + Configuration.getConfig().screenSaverTimeout;
		measurementChanged = true;
		alarmChanged = true;
	}
	display_on = show;
}

void ScumDisplayClass::toggleDisplay() {
	showDisplay(!display_on);
}

void ScumDisplayClass::keepAlive() {
	turn_off_millis = millis() + Configuration.getConfig().screenSaverTimeout;;
	if (!display_on) {
		showDisplay(true);
	}
}


// Format is "hh:mm:ss" - Note buf must be 10 chars long
void ScumDisplayClass::formatTime(char* buf, time_t value) {
	int hours = hour(value);
	int mins = minute(value);
	int secs = second(value);

	buf[0] = '0' + (hours / 10);
	buf[1] = '0' + (hours % 10);
	buf[2] = ':';

	buf[3] = '0' + (mins / 10);
	buf[4] = '0' + (mins % 10);
	buf[5] = ':';
	buf[6] = '0' + (secs / 10);
	buf[7] = '0' + (secs % 10);
	buf[8] = ' ';
	buf[9] = 0;
}

// Format is DD/MM/YY hh:mm:ss - Buf must be 19 chars
void ScumDisplayClass::formatDateTime(char* buf, time_t value) {
	int years = year(value) % 100;
	int months = month(value);
	int days = day(value);	

	buf[0] = '0' + days / 10;
	buf[1] = '0' + (days % 10);
	buf[2] = '/';
	buf[3] = (months < 10) ? '0' : '1';
	buf[4] = '0' + (months % 10);
	buf[5] = '/';
	buf[6] = '0' + years / 10;
	buf[7] = '0' + years % 10;
	buf[8] = ' ';

	formatTime(&buf[9], value);
}