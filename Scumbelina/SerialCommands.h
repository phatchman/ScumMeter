// SerialCommands.h

#ifndef _SERIALCOMMANDS_h
#define _SERIALCOMMANDS_h
#include<CmdMessenger.h>

class SerialCommandsClass
{
private:
	static bool dumpError;

 protected:

	 static void attachCommandCallbacks();
	 // Callbacks
	 static void OnUnknownCommand();
	 static void OnSetDateTime();
	 static void OnDataDump();
	 static void OnGetConfiguration();
	 static void OnSetConfiguration();
	 static void OnWatchdogRequest();
	 

	 static void OnNewDataItem(char** values, int8_t numValues, int8_t error);

 public:
	void init();
	void process();
};

#endif

