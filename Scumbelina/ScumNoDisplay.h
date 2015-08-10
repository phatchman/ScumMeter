// ScumDisplayh

#ifndef _SCUMNODISPLAY_h
#define _SCUMNODISPLAY_h
#include "HardwareConfig.h"



//
// DESCRIPTION::
//
// The display you have when you're not having a display
//
class BatteryMeasurement;

class ScumNoDisplayClass {

public:
	ScumNoDisplayClass() { }

	void init() { }
	void newMeasurement(const BatteryMeasurement& value) { }
	void process(void) { }
	void reset() { }
	void showDisplay(bool show) { }
	void showNextPage() { currentPage = !currentPage; }
	void toggleDisplay() { }
	byte getCurrentPage() { return currentPage;  }
	void keepAlive() { }
private:
	uint8_t currentPage;
};


#endif

