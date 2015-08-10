// 
// 
//
#ifndef ARDUINO
// Only need to set this base configuration data once
// as it is stored in EEPROM.
// Need to always set it for WIN32
#define SET_INITIAL_DATA
#endif
#include "Configuration.h"
void ConfigurationClass::init()
{
	loadConfig();
#ifdef SET_INITIAL_DATA
	if (configuration.screenSaverTimeout == 0) {
		// set some defaults. These aren't saved in EEPROM
		configuration.screenSaverTimeout = 30000;
		configuration.meterPollFrequency = 5000;
		strlcpy(configuration.maxVolts, "15", 3);
		strlcpy(configuration.minVolts, "10", 3);
		strlcpy(configuration.maxAmps, "10", 3);
		configuration.loggingFrequency = configuration.meterPollFrequency;
	}
#endif
}
void ConfigurationClass::loadConfig() {
	eeprom_read_block(&configuration, 0, sizeof(configuration));
}

void ConfigurationClass::saveConfig() {
	eeprom_update_block(&configuration, 0, sizeof(configuration));
}
