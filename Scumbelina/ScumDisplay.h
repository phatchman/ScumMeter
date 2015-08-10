// ScumDisplayh

#ifndef _SCUMDISPLAY_h
#define _SCUMDISPLAY_h
#include "HardwareConfig.h"
#include <FTOLED.h>
#include "BatteryMeter.h"

#include <fonts/Arial_Black_16_Custom.h>
#define Arial_Black_16 Arial_Black_16_Custom
#include <fonts/Arial14_Custom.h>
#define Arial14 Arial14_Custom

namespace Scumulator {
	class ScumDisplayTests;
}


//
// DESCRIPTION::
//
// KNOW ISSUES::
//
// Don't display correctly when the alarm page scrolls while we are viewing it.
// Don't think this is worth fixing as unlikely to be viewing the screen at the time.
// could do a clearScreen on new alarms as a simple fix.
//

static const byte pin_cs = 7;
static const byte pin_dc = 2;
static const byte pin_reset = 3;

class ScumDisplayClass
{
	friend class Scumulator::ScumDisplayTests;

private:
protected:
	uint8_t currentPage = 0;
	OLED oled;
	unsigned long turn_off_millis;

	bool measurementChanged = true;
	bool alarmChanged = true;
	bool display_on = true;

	void initMainPage();
	void updateMainPage();
	void initAlarmPage();
	void updateAlarmPage();

	void formatTime(char* buf, time_t value);
	void formatDateTime(char* buf, time_t value);

public:
	ScumDisplayClass() :
		oled(pin_cs,pin_dc,pin_reset)
	{
	}

	void init();
	void newMeasurement(const BatteryMeasurement& value);
	void process(void);
	void reset();
	void showDisplay(bool show);
	void showNextPage();
	void toggleDisplay();
	byte getCurrentPage() { return currentPage;  }

	//void displayError(char* error);
	// Ping the display to keep it on.
	// If inactive, the screen saver will activate
	void keepAlive();

};


#endif

