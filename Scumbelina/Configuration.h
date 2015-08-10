// Configuration.h

#ifndef _CONFIGURATION_h
#define _CONFIGURATION_h

#include "HardwareConfig.h"

// 
// DESCRIPTION::
// 
// Configuration stored in EEPROM
// and updated from the SerialPort
//
class ConfigurationClass
{
public:
	struct Config {
		unsigned long screenSaverTimeout;
		unsigned long meterPollFrequency;
		char maxVolts[3];
		char minVolts[3];
		char maxAmps[3];
		unsigned long loggingFrequency;
	};

	void init();
	Config& getConfig() { return configuration;  }
	void saveConfig();

protected:
	void loadConfig();

private:
	Config configuration;
};


#endif

