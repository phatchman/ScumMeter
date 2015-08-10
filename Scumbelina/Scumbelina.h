#include "HardwareConfig.h"
#include <Button.h>
#include "BatteryMeter.h"
#include "ScumDisplay.h"
#include "DataLogger.h"
#include "SerialCommands.h"
#include "AlarmLog.h"
#include "Configuration.h"
// Function prototypes to support the WIN32 environment
void newBatteryMeasurement(const BatteryMeasurement& value);
void displayButtonClicked(Button& but);
void menuButtonPressed(Button& but);
void menuButtonHeld(Button& but);

// 
// PINS USED
//
// 0 - O/B SERIAL RX (N/C)
// 1 - O/B SERIAL TX (N/C)
// 2 - OLED DNC
// 3 - OLED RST
// 4 - SDCARD CS
// 5 - Menu Button
// 6 - Display Button
// 7 - OLED CS
// 8 - N/C
// 9 - USB INT
// 10 - SPI SS
// 11 - SPI MOSI
// 12 - SPI MISO
// 13 - SPI SCK
// A0 - SoftSerial RX
// A1 - SoftSerial TX
// A2 - N/C 
// A3 - N/C 
// A4 - RTC SDA
// A5 - RTC SCL
// N/C - RTC SQI 
//
// NOTES:
// 1 - SPI bus has been set to Full SPI speed. It defaults to half speed for the SD card
//     This should be fine as SD card is onboard and will improve speed of display
//     updates to the OLED screen
//
// 2 - USB not used and not initialised.
//     This leaves pin 9 free, but will need to cut the USB INT trade.
//		Could consider using this and AltSerial, but there is little 
//		memory usage difference between the two libraries
//     
// 3 - SQI of RTC is not connected. No alarms or interrups possible.
//     RTC library expects this on PIN 2 (INT). Already used by OLED, but
//     OLED can be reconfigured to a different pin if required.
//

//
// TODO LIST:
// - Completed (for now)
//


void setup() {
	Serial.begin(115200);
	Serial1.begin(9600);
//	CURRENT_TIME = now();

	// Note need to initialise the configuration first
	Configuration.init();
	AlarmLog.init();
	DisplayButton.clickHandler(displayButtonClicked);
	MenuButton.clickHandler(menuButtonPressed);
	MenuButton.holdHandler(menuButtonHeld, 1000);
	BatteryMeter.measurementHandler(newBatteryMeasurement);
	BatteryMeter.init();
	DataLogger.init();
	ScumDisplay.init();
	SerialCommands.init();
}

void displayButtonClicked(Button& but) {
	ScumDisplay.toggleDisplay();
}

void menuButtonPressed(Button& but) {
	ScumDisplay.showNextPage();
}

void newBatteryMeasurement(const BatteryMeasurement& value) {
	ScumDisplay.newMeasurement(value);
	DataLogger.newMeasurement(value);
	AlarmLog.newMeasurement(value);
}

void menuButtonHeld(Button& but) {
	ScumDisplay.keepAlive();
	if (ScumDisplay.getCurrentPage() == 0) {
		BatteryMeter.reset();
	} 
	else {
		AlarmLog.reset();
		DataLogger.reset();
	}
	ScumDisplay.reset();
}

void loop() {
//	CURRENT_TIME = now();

	DisplayButton.process();
	MenuButton.process();
	BatteryMeter.process();
	ScumDisplay.process();
	SerialCommands.process();
}
