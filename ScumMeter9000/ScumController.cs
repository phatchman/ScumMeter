// *** TemperatureContro ***

// This example example is where all previously described features come together in one full application
// and wait for a response from the Arduino. The Arduino will start sending temperature data and the heater steering
// value data which the PC will plot in a chart. With a slider we can set the goal temperature, which will make the
// PID software on the controller adjust the setting of the heater.
// 
// This example shows how to design a responsive performance UI that sends and receives commands
// - Send queued commands
// - Manipulate the send and receive queue
// - Add queue strategies
// - Use serial or bluetooth connection
// - Use auto scanning and connecting
// - Use the autoconnect and watchdog 

using System;
using CommandMessenger;
using CommandMessenger.Queue;
using CommandMessenger.Transport;
using CommandMessenger.Transport.Serial;
using CommandMessenger.Transport.File;
using System.Threading;
using System.ComponentModel;
using System.Collections.ObjectModel;

namespace DataLogging
{
    enum Command
    {
        Identify,           // Command to identify device
        Acknowledge,        // Command to acknowledge a received command
        Error,              // Command to message that an error has occurred
        SetDateTime,        // Command to set the date and time on the Arduino
        RequestDataDownload,  // Command to request data download from arduino
        DataDownloadStart,  // Command from arduino indicating the start of Data Download
        DataDownloadItem,   // Command from arduino for a data line item
        DataDownloadComplete, // Command from arduino indicating completion of the data download.
        kGetConfiguration,	// Get current config parameters
        kSetConfiguration,	// Set current config parameters
        kConfigurationData	// Config data.
    };

    public class BatteryMeasurement
    {

        public DateTime Timestamp { get; private set; }
        public double Volts { get; private set; }
        public double Amps { get; private set; }

        public BatteryMeasurement(DateTime timestamp, double volts, double amps)
        {
            Timestamp = timestamp;
            Volts = volts;
            Amps = amps;
        }
    }


    public class ScumController
    {
        private bool _OFFLINE_TESTING = false;
        private bool _USE_FILE_TRANSPORT = true;
        private const string UniqueDeviceId = "F21089D968C34F2E97F34FA6EB5AEDCA";

        private ITransport            _transport;
        private CmdMessenger          _cmdMessenger;
        private ConnectionManager     _connectionManager;
        private ChartForm             _chartForm;


        public BindingList<BatteryMeasurement> _batteryMeasurementModel = new BindingList<BatteryMeasurement>();

        public BindingList<BatteryMeasurement> BatteryMeasurementModel
        {
            get { return _batteryMeasurementModel; }
        }
        //public ObservableCollection<BatteryMeasurement> BatteryMeasurementModel
        //{
        //    get { return _batteryMeasurementModel; }
        //}
        //// ------------------ MAIN  ----------------------

        // TODO: put some bool in here for when we are downloading data
        // to make sure we clean up if there is a transfer error.
        //public bool AcquisitionStarted { get; set; }
        

        // Setup function
        public void Setup(ChartForm chartForm)
        {
            
            // getting the chart control on top of the chart form.
            _chartForm = chartForm;            
            // Set up chart
            _chartForm.SetupChart();

            // Connect slider to GoalTemperatureChanged
            //GoalTemperatureChanged += () => _chartForm.GoalTemperatureTrackBarScroll(null, null);
            if (!_USE_FILE_TRANSPORT)
            {
                _transport = new SerialTransport
                 {
                     CurrentSerialSettings = { PortName = "COM8", BaudRate = 115200, DtrEnable = false } // some boards (e.g. Sparkfun Pro Micro) DtrEnable may need to be true.      
                 };
                // We do not need to set serial port and baud rate: it will be found by the connection manager                                                           
            }
            else
            {
                _transport = new FileTransport();
            }
            // Initialize the command messenger with one of the two transport layers
            // Set if it is communicating with a 16- or 32-bit Arduino board
            _cmdMessenger = new CmdMessenger(_transport, BoardType.Bit32)
            {
                PrintLfCr = false            // Do not print newLine at end of command, to reduce data being sent
            };

            // Tell CmdMessenger to "Invoke" commands on the thread running the WinForms UI
            _cmdMessenger.ControlToInvokeOn = chartForm;

            // TODO: Perhaps set a top command strategy so we can can interrrupt downloads etc in future.
            //_cmdMessenger.AddReceiveCommandStrategy(new StaleGeneralStrategy(1000));          

            // Attach the callbacks to the Command Messenger
            AttachCommandCallBacks();

            // Attach to NewLinesReceived for logging purposes
            _cmdMessenger.NewLineReceived += NewLineReceived;

            // Attach to NewLineSent for logging purposes
            _cmdMessenger.NewLineSent     += NewLineSent;                       

            // Set up connection manager, corresponding to the transportMode
            if (!_USE_FILE_TRANSPORT)
            {
                _connectionManager = new SerialConnectionManager((_transport as SerialTransport), _cmdMessenger, (int)Command.Identify, UniqueDeviceId);
            }
            else
            {
                _connectionManager = new FileConnectionManager((_transport as FileTransport), _cmdMessenger, (int)Command.Identify, UniqueDeviceId);
            }
            // TODO: Enable watchdog functionality.
            //_connectionManager.WatchdogEnabled = true;
 
            // Event when the connection manager finds a connection
            _connectionManager.ConnectionFound += ConnectionFound;

            // Event when the connection manager watchdog notices that the connection is gone
            _connectionManager.ConnectionTimeout += ConnectionTimeout;
            
            // Event notifying on scanning process
            _connectionManager.Progress += LogProgress;

            // Initialize the application
            Initialize(); 

            // Start scanning for ports/devices
            if (!_OFFLINE_TESTING)
                _connectionManager.StartConnectionManager();           
        }

        private void Initialize()
        {
            _chartForm.SetDisConnected();
        }

        // Exit function
        public void Exit()
        {
            // Disconnect ConnectionManager
            _connectionManager.Progress          -= LogProgress;
            _connectionManager.ConnectionTimeout -= ConnectionTimeout;
            _connectionManager.ConnectionFound   -= ConnectionFound;

            // Dispose ConnectionManager
            _connectionManager.Dispose();

            // Disconnect Command Messenger
            _cmdMessenger.Disconnect();           

            // Dispose Command Messenger
            _cmdMessenger.Dispose();

            // Dispose transport layer
            if (!_OFFLINE_TESTING)
                _transport.Dispose();
        }

        /// Attach command call backs. 
        private void AttachCommandCallBacks()
        {
            _cmdMessenger.Attach(OnUnknownCommand);
            _cmdMessenger.Attach((int)Command.Acknowledge, OnAcknowledge);
            _cmdMessenger.Attach((int)Command.Error, OnError);
            _cmdMessenger.Attach((int)Command.DataDownloadItem, OnDataDownloadItem);
            //_cmdMessenger.Attach((int)Command.DataDownloadStart, OnDataDownloadStart);
            _cmdMessenger.Attach((int)Command.DataDownloadComplete, OnDataDownloadEnd);
//            _cmdMessenger.Attach((int)Command.kConfigurationData, OnReceiveConfiguration); 
        }

        // ------------------  CALLBACKS ---------------------

        // Called when a received command has no attached function.
        // In a WinForm application, console output gets routed to the output panel of your IDE
        void OnUnknownCommand(ReceivedCommand arguments)
        {
            _chartForm.LogMessage(@"Command without attached callback received");
        }

        // Callback function that prints that the Arduino has acknowledged
        void OnAcknowledge(ReceivedCommand arguments)
        {
            _chartForm.LogMessage(@"Arduino acknowledged");
        }

        // Callback function that prints that the Arduino has experienced an error
        void OnError(ReceivedCommand arguments)
        {
            _chartForm.LogMessage(@"Arduino has experienced an error");
        }

        /* TODO: This is not called, DataDownloadStart is an ack to a command.
         * Is it needed for anything. Do we just send a normal ack then a start?
         * If we did send a start message, then we could send the number of 
         * entries to implement a progress bar or similar.
         private void OnDataDownloadStart(ReceivedCommand arguments)
        {
 
            // TODO: Set a Flag so that EndDataDownload is always called, even on errors.
            _chartForm.BeginDataDownload();
        }
        */
        private void OnDataDownloadEnd(ReceivedCommand arguments)
        {           
            // TODO: Set a Flag so that EndDataDownload is always called, even on errors.

            _chartForm.EndDataDownload();
        }

        // Set the current date and time on the embedded controller
        public bool SetDateTime()
        {
            DateTime now = DateTime.Now;
            var command = new SendCommand((int)Command.SetDateTime, (int)Command.Acknowledge, 500);
            command.AddArgument(now.Year);
            command.AddArgument(now.Month);
            command.AddArgument(now.Day);
            command.AddArgument(now.Hour);
            command.AddArgument(now.Minute);
            command.AddArgument(now.Second);

            var receivedCommand = _cmdMessenger.SendCommand(command);

            if (!receivedCommand.Ok)
            {
                _chartForm.LogMessage(@" Failure > no OK received from controller");
            }
            return receivedCommand.Ok;
        }

        // Upload the default configuration
        public bool SetDefaultConfiguration()
        {
            var command = new SendCommand((int)Command.kSetConfiguration, (int)Command.Acknowledge, 500);
            command.AddArgument(60); // 1 minute
            command.AddArgument(5); // 5 secs
            command.AddArgument(30); // 30 secs
            command.AddArgument("16"); // maxVolts
            command.AddArgument("12"); // minVolts
            command.AddArgument("10"); // maxAmps

            var receivedCommand = _cmdMessenger.SendCommand(command);

            if (!receivedCommand.Ok)
            {
                _chartForm.LogMessage(@" Failure > no OK received from controller");
            }
            return receivedCommand.Ok;
        }

        public bool GetConfiguration()
        {
            var command = new SendCommand((int)Command.kGetConfiguration, (int)Command.kConfigurationData, 500);
            var receivedCommand = _cmdMessenger.SendCommand(command);

            if (!receivedCommand.Ok)
            {
                _chartForm.LogMessage(@" Failure > no OK received from controller");
            }
            else
            {
                _chartForm.LogMessage(String.Format("Display Timeout {0}", receivedCommand.ReadUInt32Arg()));
                _chartForm.LogMessage(String.Format("Meter Poll Interval {0}", receivedCommand.ReadUInt32Arg()));
                _chartForm.LogMessage(String.Format("Data Log Interval {0}", receivedCommand.ReadUInt32Arg()));
                _chartForm.LogMessage(String.Format("Max Volts {0}", receivedCommand.ReadStringArg()));
                _chartForm.LogMessage(String.Format("Min Volts {0}", receivedCommand.ReadStringArg()));
                _chartForm.LogMessage(String.Format("Max Amps {0}", receivedCommand.ReadStringArg()));
            }
            return receivedCommand.Ok;
        }

        void OnReceiveConfiguration(ReceivedCommand arguments)
        {
            _chartForm.LogMessage(String.Format("Display Timeout {0}", arguments.ReadUInt32Arg()));
            _chartForm.LogMessage(String.Format("Meter Poll Interval {0}", arguments.ReadUInt32Arg()));
            _chartForm.LogMessage(String.Format("Data Log Interval {0}", arguments.ReadUInt32Arg()));
            _chartForm.LogMessage(String.Format("Max Volts {0}", arguments.ReadStringArg()));
            _chartForm.LogMessage(String.Format("Min Volts {0}", arguments.ReadStringArg()));
            _chartForm.LogMessage(String.Format("Max Amps {0}", arguments.ReadStringArg()));
        }

        public bool RequestDataDownload()
        {
            _batteryMeasurementModel.Clear();

            if (_OFFLINE_TESTING)
            {
                for (int i = 0; i < 1000; i++)
                {
                    _batteryMeasurementModel.Add(new BatteryMeasurement(DateTime.Now.AddMinutes(i * 15), i % 20, (i + 1) % 40));
                    _chartForm.EndDataDownload();
                }
                return true;
            }
            var command = new SendCommand((int)Command.RequestDataDownload, (int)Command.DataDownloadStart, 500);
            // Download all values.
            command.AddArgument(0);
            command.AddArgument(999999);
            var receivedCommand = _cmdMessenger.SendCommand(command, SendQueue.ClearQueue, ReceiveQueue.ClearQueue);
            if (!receivedCommand.Ok)
            {
                _chartForm.LogMessage(@" Failure > no OK received from controller");
            }
            _batteryMeasurementModel.Clear();
            _chartForm.BeginDataDownload();
            return receivedCommand.Ok;
        }


        private void OnDataDownloadItem(ReceivedCommand arguments)
        {
            ulong timestamp = arguments.ReadUInt32Arg();
            System.DateTime dtDateTime = new DateTime(1970, 1, 1, 0, 0, 0, 0, System.DateTimeKind.Utc);
            dtDateTime = dtDateTime.AddSeconds(timestamp);
            double volts = arguments.ReadDoubleArg();            
            double amps = arguments.ReadDoubleArg();
            BatteryMeasurementModel.Add(new BatteryMeasurement(dtDateTime, volts, amps));
            _chartForm.UpdateDataItem(dtDateTime, volts, amps);
        }


        // Log received line to console
        private void NewLineReceived(object sender, CommandEventArgs e)
        {
            _chartForm.LogMessage(@"Received > " + e.Command.CommandString());
          //  Console.WriteLine(@"Received > " + e.Command.CommandString());
        }

        // Log sent line to console
        private void NewLineSent(object sender, CommandEventArgs e)
        {
            _chartForm.LogMessage(@"Sent > " + e.Command.CommandString());
           // Console.WriteLine(@"Sent > " + e.Command.CommandString());
        }

        // Log connection manager progress to status bar
        void LogProgress(object sender, ConnectionManagerProgressEventArgs e)
        {
            if (e.Level <= 2) { _chartForm.SetStatus(e.Description); }
            _chartForm.LogMessage(e.Description);
           // Console.WriteLine(e.Level + @" :" + e.Description);
        }

        private void ConnectionTimeout(object sender, EventArgs e)
        {           
            // Connection time-out!
            // Disable UI ..                 
            _chartForm.SetStatus(@"Connection timeout, attempting to reconnect");           
            _chartForm.SetDisConnected();
        }

        private void ConnectionFound(object sender, EventArgs e)
        {
            //We have been connected! 
            
            // Enable UI
            _chartForm.SetConnected();
            
            // Send command to set goal Temperature
            // SetGoalTemperature(_goalTemperature);
            // SetDateTime();

            // Yield time slice in order to get UI updated
            Thread.Yield();
        }

        // Set the goal temperature on the embedded controller
        public void SetGoalTemperature(double goalTemperature) 
        {
   /*         _goalTemperature = goalTemperature;

            // Create command to start sending data
             var command = new SendCommand((int)Command.SetGoalTemperature);
            
            // Make sure to be explicit if sending float or double
             command.AddBinArgument((float)_goalTemperature);

            // Collapse this command if needed using CollapseCommandStrategy
            // This strategy will avoid duplicates of this command on the queue: if a SetGoalTemperature command is
            // already on the queue when a new one is added, it will be replaced at its current queue-position. 
            // Otherwise the command will be added to the back of the queue. 
            // 
            // This will make sure that when the slider raises a lot of events that each set a new goal temperature, the 
            // controller will not start lagging.
             _chartForm.LogMessage(@"Queue command - SetGoalTemperature");
            _cmdMessenger.QueueCommand(new CollapseCommandStrategy(command));
     */   }


    }
}
