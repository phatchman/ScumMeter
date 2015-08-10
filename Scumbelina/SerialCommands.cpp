//
// 
// 
#include "HardwareConfig.h"
#include "DS3232RTC.h"

#include <Time.h>
#include "DataLogger.h"
#include "SerialCommands.h"
#include "ScumDisplay.h"
#include "Configuration.h"



CmdMessenger cmdMessenger = CmdMessenger(Serial);
static const char DEVICE_ID[] PROGMEM = "F21089D968C34F2E97F34FA6EB5AEDCA";
bool SerialCommandsClass::dumpError = false;


// ::TODO::
// Can save some ram by re-ordering commands
// with callbacks to the start of the enum.
// max callbacks is set based on the max command number with a callback.
enum
{
	kIdentify,           // Command to identify device
	kAcknowledge,        // Command to acknowledge a received command
	kError,              // Command to message that an error has occurred
	kSetDateTime,        // Command to set the date and time on the Arduino
	kRequestDataDownload,  // Command to request data download from arduino
	kDataDownloadStart,  // Command from arduino indicating the start of Data Download
	kDataDownloadItem,   // Command from arduino for a data line item
	kDataDownloadComplete, // Command from arduino indicating completion of the data download.
	kGetConfiguration,	// Get current config parameters
	kSetConfiguration,	// Set current config parameters
	kConfigurationData	// Config data.
};



void SerialCommandsClass::OnSetDateTime() {
	uint16_t yyyy = cmdMessenger.readInt16Arg();
	int8_t mm = (int8_t) cmdMessenger.readInt16Arg();
	int8_t dd = (int8_t) cmdMessenger.readInt16Arg();
	int8_t hh = (int8_t) cmdMessenger.readInt16Arg();
	int8_t MM = (int8_t) cmdMessenger.readInt16Arg();
	int8_t ss = (int8_t)cmdMessenger.readInt16Arg();
	if ((yyyy < 2000) || (yyyy > 9999) ||
		(mm < 1) || (mm > 12) ||
		(dd < 1) || (dd > 31) ||
		(hh < 0) || (hh > 23) ||
		(MM < 0) || (MM > 59) ||
		(ss < 0) || (ss > 59)) {
		cmdMessenger.sendCmd(kError);
	}
	else {
		DataLogger.setTime(yyyy, mm, dd, hh, MM, ss);
		cmdMessenger.sendCmd(kAcknowledge);
	}
}


void SerialCommandsClass::OnNewDataItem(char** values, int8_t numValues, int8_t error) {
	if (error == 0) {
		cmdMessenger.sendCmdStart(kDataDownloadItem);
		for (int i = 0; i < numValues; i++) {
			cmdMessenger.sendCmdArg(values[i]);
		}
		cmdMessenger.sendCmdEnd();
	}
	else {
		dumpError = error > 0;
	}
}

void SerialCommandsClass::OnDataDump()
{
	uint32_t startDate = cmdMessenger.readInt32Arg();
	uint32_t endDate = cmdMessenger.readInt32Arg();

	cmdMessenger.sendCmd(kDataDownloadStart);
	cmdMessenger.feedinSerialData();

	dumpError = false;

	DataLogger.dumpTo(startDate, endDate, OnNewDataItem);

	if (dumpError == 0) {
		cmdMessenger.sendCmd(kDataDownloadComplete);
	}
	else {
		cmdMessenger.sendCmd(kError);
	}
}

void SerialCommandsClass::OnGetConfiguration() {
	ConfigurationClass::Config& values = Configuration.getConfig();

	cmdMessenger.sendCmdStart(kConfigurationData);
	cmdMessenger.sendCmdArg((uint16_t)(values.screenSaverTimeout / 1000UL));
	cmdMessenger.sendCmdArg((uint16_t)(values.meterPollFrequency / 1000UL));
	cmdMessenger.sendCmdArg((uint16_t)(values.loggingFrequency / 1000UL));
	cmdMessenger.sendCmdArg(values.maxVolts);
	cmdMessenger.sendCmdArg(values.minVolts);
	cmdMessenger.sendCmdArg(values.maxAmps);
	cmdMessenger.sendCmdEnd();
}

void SerialCommandsClass::OnSetConfiguration() {
	ConfigurationClass::Config& values = Configuration.getConfig();
	values.screenSaverTimeout = (uint32_t)cmdMessenger.readInt32Arg() * 1000UL;
	values.meterPollFrequency = (uint32_t)cmdMessenger.readInt32Arg() * 1000UL;
	values.loggingFrequency = (uint32_t)cmdMessenger.readInt32Arg() * 1000UL;
	strlcpy(values.maxVolts, cmdMessenger.readStringArg(), sizeof(values.maxVolts));
	strlcpy(values.minVolts, cmdMessenger.readStringArg(), sizeof(values.maxVolts));
	strlcpy(values.maxAmps, cmdMessenger.readStringArg(), sizeof(values.maxVolts));

	// No validation on these values.. Be careful!!
	Configuration.saveConfig();
	cmdMessenger.sendCmd(kAcknowledge);
}

void SerialCommandsClass::OnUnknownCommand()
{
	cmdMessenger.sendCmd(kError);
}

void SerialCommandsClass::OnWatchdogRequest()
{
	// Will respond with same command ID and Unique device identifier.
	TMPBUF_ACQUIRE;
	strlcpy_P(TMPBUF, DEVICE_ID, sizeof(DEVICE_ID));
	cmdMessenger.sendCmd(kIdentify, TMPBUF);
	TMPBUF_RELEASE;
}

void SerialCommandsClass::attachCommandCallbacks()
{
	// Attach callback methods
	cmdMessenger.attach(OnUnknownCommand);
	cmdMessenger.attach(kIdentify, OnWatchdogRequest);
	cmdMessenger.attach(kSetDateTime, OnSetDateTime);
	cmdMessenger.attach(kRequestDataDownload, OnDataDump);
	cmdMessenger.attach(kGetConfiguration, OnGetConfiguration);
	cmdMessenger.attach(kSetConfiguration, OnSetConfiguration);
}


void SerialCommandsClass::init()
{
	cmdMessenger.printLfCr();

	// Attach my application's user-defined callback methods
	attachCommandCallbacks();
}

void SerialCommandsClass::process() {
	cmdMessenger.feedinSerialData();
}