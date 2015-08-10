#include "HardwareConfig.h"
#ifndef ARDUINO
#include "..\SimulatedHardware\FTOLED.h"
#include "..\Scumbelina\BatteryMeter.h"
#include "..\Scumbelina\DataLogger.h"
#include "..\Scumbelina\ScumDisplay.h"
#include "..\SimulatedHardware\Button.h"
//NullSerial Serial;
FileEmulatedSerial Serial;
SimulatedSerialMeter Serial1;
unsigned long _millis_ADJ = 0;
long TMPBUF_LINE = -1;
wchar_t TMPBUF_FILE[1024] = L"";
bool TMPBUF_ACQUIRED = false;
#else
#include "BatteryMeter.h"
#include "DataLogger.h"
#ifndef NO_DISPLAY
#include "ScumDisplay.h"
#else
#include "ScumNoDisplay.h"
#endif
#include "Button.h"
SoftwareSerial Serial1(A1, A0);
#endif

#include "SerialCommands.h"
#include "Configuration.h"
#include "AlarmLog.h"

//
// The down-side of this is that construction order
// is not guaranteed. Any class must not access another 
// one of these global instances during its constructor
// and should do so by providing an init method instead.
//
ConfigurationClass Configuration;
BatteryMeterClass BatteryMeter;
DataLoggerClass DataLogger;
SerialCommandsClass SerialCommands;
AlarmLogClass AlarmLog;
#ifndef NO_DISPLAY
ScumDisplayClass ScumDisplay;
#else
ScumNoDisplayClass ScumDisplay;
#endif
Button DisplayButton(6);
Button MenuButton(5);
// Must be large enough to fit a full meter reading (23 chars)
// And to read/write to the datalogger (29 - 28 chars + \n)
char TMPBUF[32];
//time_t CURRENT_TIME;