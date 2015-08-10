using System;
using System.Drawing;
using System.Windows.Forms;
using System.Globalization;
using CommandMessenger;
using System.IO;

namespace DataLogging
{
    public partial class ChartForm : Form
    {
        // In a small C# application all code would typically end up in this class.
        // For a cleaner, MVP-like setup I moved higher logic to TemperatureControl.cs,        
        
        private readonly ScumController _scumControl;
        private bool _connected;

        public ChartForm()
        {
            InitializeComponent();
            _scumControl = new ScumController();
            batteryMeasurementModelBindingSource.DataSource = _scumControl.BatteryMeasurementModel;
            chart1.Palette = System.Windows.Forms.DataVisualization.Charting.ChartColorPalette.Excel;
            DisplayEmptyChart();
        }

        private void ChartFormShown(object sender, EventArgs e)
        {
            // Run setup of view model
            _scumControl.Setup(this);
        }


        // ------------------  CHARTING ROUTINES ---------------------

        /// <summary> Sets up the chart. </summary>
        public void SetupChart()
        {
            
        }

        public void DisplayEmptyChart()
        {
            chart1.Series["Empty"].Points.AddXY(DateTime.Now, 0);
        }

        public void BeginDataDownload()
        {
 //           lstDataDisplay.Items.Clear();
 //           lstDataDisplay.BeginUpdate();
        }

        public void EndDataDownload()
        {
       //     lstDataDisplay.EndUpdate();
            if (_scumControl._batteryMeasurementModel.Count > 0)
            {
                DisplayEmptyChart();
            }
            else
            {
                chart1.Series["Empty"].Points.Clear();
            }

            chart1.DataBind();
        }

        public void UpdateDataItem(DateTime dt, double volts, double amps)
        {
//            lstDataDisplay.Items.Add(new ListViewItem(new [] {dt.ToShortDateString(), dt.ToShortTimeString(), volts.ToString(), amps.ToString()}));
        }

        // Update the graph with the data points
        public void SetConnected()
        {
            _connected = true;
            UpdateUi();
        }

        // Update the graph with the data points
        public void SetDisConnected()
        {
            _connected = false;
            UpdateUi();
        }

        /// <summary> Updates the user interface. </summary>
        private void UpdateUi()
        {
            buttonStartAcquisition.Enabled  = _connected;
            buttonStopAcquisition.Enabled   = _connected;
        }

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing)
            {
                _scumControl.Exit();
                if (components != null)
                    components.Dispose();
            }
            base.Dispose(disposing);
        }


        /// <summary>  Stop Acquisition. </summary>
        /// <param name="sender"> Source of the event. </param>
        /// <param name="e">      Event information. </param>
        private void ButtonStopAcquisitionClick(object sender, EventArgs e)
        {
            _scumControl.SetDefaultConfiguration();
        }

        /// <summary>  Start Acquisition. </summary>
        /// <param name="sender"> Source of the event. </param>
        /// <param name="e">      Event information. </param>
        private void ButtonStartAcquisitionClick(object sender, EventArgs e)
        {
     //       _scumControl.StartAcquisition();
             // Set the time
            _scumControl.SetDateTime();
        }

        /// <summary> Update status bar. </summary>
        /// <param name="description"> The message to show on the status bar. </param>
        public void SetStatus(string description)
        {
            toolStripStatusLabel1.Text = description;
        }

        private void listView1_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void loggingView1_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        public void LogMessage(string message)
        {
            loggingView1.AddEntry(message);
        }

        //private void button1_Click(object sender, EventArgs e)
        //{
        //    _scumControl.SetDateTime();
        //}

        private void btnDownloadData_Click(object sender, EventArgs e)
        {
            _scumControl.RequestDataDownload();
        }

        private void chartControl_Load(object sender, EventArgs e)
        {

        }

        private void btnSave_Click(object sender, EventArgs e)
        {
            var dlg = new SaveFileDialog();

            var saveFileDialog1 = new SaveFileDialog();

            saveFileDialog1.Filter = "csv files (*.csv)|*.csv|All files (*.*)|*.*"  ;
            //saveFileDialog1.FilterIndex = 2 ;
            //saveFileDialog1.RestoreDirectory = true ;

            if(saveFileDialog1.ShowDialog() == DialogResult.OK)
            {
                try
                {
                    using (var myStream = saveFileDialog1.OpenFile())
                    {
                        if (myStream == null) return;
                        var sw = new StreamWriter(myStream);

                        foreach (var m in _scumControl.BatteryMeasurementModel)
                        {
                            sw.WriteLine("{0:d},{1:t},{2:0.00},{3:0.00}", m.Timestamp, m.Timestamp, m.Volts, m.Amps);
                        }
                        sw.Close();
                        myStream.Close();
                    }
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message, "File Save Error", MessageBoxButtons.OK, MessageBoxIcon.Error);      
                }
            }
        }

        private void ChartForm_Load(object sender, EventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {
            _scumControl.GetConfiguration();
        }

    }
}
