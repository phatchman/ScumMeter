namespace DataLogging
{
    partial class ChartForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;


        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea1 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Legend legend1 = new System.Windows.Forms.DataVisualization.Charting.Legend();
            System.Windows.Forms.DataVisualization.Charting.Series series1 = new System.Windows.Forms.DataVisualization.Charting.Series();
            System.Windows.Forms.DataVisualization.Charting.Series series2 = new System.Windows.Forms.DataVisualization.Charting.Series();
            System.Windows.Forms.DataVisualization.Charting.Series series3 = new System.Windows.Forms.DataVisualization.Charting.Series();
            System.Windows.Forms.DataVisualization.Charting.Title title1 = new System.Windows.Forms.DataVisualization.Charting.Title();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle1 = new System.Windows.Forms.DataGridViewCellStyle();
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle2 = new System.Windows.Forms.DataGridViewCellStyle();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ChartForm));
            this.buttonStopAcquisition = new System.Windows.Forms.Button();
            this.buttonStartAcquisition = new System.Windows.Forms.Button();
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.toolStripStatusLabelProgress = new System.Windows.Forms.ToolStripStatusLabel();
            this.toolStripStatusLabel1 = new System.Windows.Forms.ToolStripStatusLabel();
            this.btnDownloadData = new System.Windows.Forms.Button();
            this.chart1 = new System.Windows.Forms.DataVisualization.Charting.Chart();
            this.batteryMeasurementModelBindingSource = new System.Windows.Forms.BindingSource(this.components);
            this.dataGridView1 = new System.Windows.Forms.DataGridView();
            this.Column1 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Column2 = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.btnSave = new System.Windows.Forms.Button();
            this.voltsDataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.ampsDataGridViewTextBoxColumn = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.scumControllerBindingSource = new System.Windows.Forms.BindingSource(this.components);
            this.loggingView1 = new Tools.LoggingView();
            this.button1 = new System.Windows.Forms.Button();
            this.statusStrip1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.chart1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.batteryMeasurementModelBindingSource)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.scumControllerBindingSource)).BeginInit();
            this.SuspendLayout();
            // 
            // buttonStopAcquisition
            // 
            this.buttonStopAcquisition.Location = new System.Drawing.Point(117, 677);
            this.buttonStopAcquisition.Name = "buttonStopAcquisition";
            this.buttonStopAcquisition.Size = new System.Drawing.Size(98, 35);
            this.buttonStopAcquisition.TabIndex = 7;
            this.buttonStopAcquisition.Text = "Send Settings";
            this.buttonStopAcquisition.UseVisualStyleBackColor = true;
            this.buttonStopAcquisition.Click += new System.EventHandler(this.ButtonStopAcquisitionClick);
            // 
            // buttonStartAcquisition
            // 
            this.buttonStartAcquisition.Location = new System.Drawing.Point(13, 677);
            this.buttonStartAcquisition.Name = "buttonStartAcquisition";
            this.buttonStartAcquisition.Size = new System.Drawing.Size(98, 35);
            this.buttonStartAcquisition.TabIndex = 8;
            this.buttonStartAcquisition.Text = "Set Time";
            this.buttonStartAcquisition.UseVisualStyleBackColor = true;
            this.buttonStartAcquisition.Click += new System.EventHandler(this.ButtonStartAcquisitionClick);
            // 
            // statusStrip1
            // 
            this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripStatusLabelProgress,
            this.toolStripStatusLabel1});
            this.statusStrip1.Location = new System.Drawing.Point(0, 810);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(929, 22);
            this.statusStrip1.SizingGrip = false;
            this.statusStrip1.TabIndex = 9;
            this.statusStrip1.Text = "statusStrip1";
            // 
            // toolStripStatusLabelProgress
            // 
            this.toolStripStatusLabelProgress.Name = "toolStripStatusLabelProgress";
            this.toolStripStatusLabelProgress.Size = new System.Drawing.Size(0, 17);
            // 
            // toolStripStatusLabel1
            // 
            this.toolStripStatusLabel1.Name = "toolStripStatusLabel1";
            this.toolStripStatusLabel1.Size = new System.Drawing.Size(19, 17);
            this.toolStripStatusLabel1.Text = "    ";
            // 
            // btnDownloadData
            // 
            this.btnDownloadData.Location = new System.Drawing.Point(334, 677);
            this.btnDownloadData.Name = "btnDownloadData";
            this.btnDownloadData.Size = new System.Drawing.Size(98, 35);
            this.btnDownloadData.TabIndex = 13;
            this.btnDownloadData.Text = "Download Data";
            this.btnDownloadData.UseVisualStyleBackColor = true;
            this.btnDownloadData.Click += new System.EventHandler(this.btnDownloadData_Click);
            // 
            // chart1
            // 
            chartArea1.AxisX.IntervalOffsetType = System.Windows.Forms.DataVisualization.Charting.DateTimeIntervalType.Days;
            chartArea1.AxisX.LabelStyle.Format = "d";
            chartArea1.AxisX.MajorGrid.IntervalOffsetType = System.Windows.Forms.DataVisualization.Charting.DateTimeIntervalType.Days;
            chartArea1.AxisX.MajorTickMark.IntervalOffsetType = System.Windows.Forms.DataVisualization.Charting.DateTimeIntervalType.Days;
            chartArea1.AxisX.MinorGrid.IntervalOffsetType = System.Windows.Forms.DataVisualization.Charting.DateTimeIntervalType.Hours;
            chartArea1.AxisX.MinorTickMark.IntervalOffsetType = System.Windows.Forms.DataVisualization.Charting.DateTimeIntervalType.Hours;
            chartArea1.AxisX.ScaleView.MinSizeType = System.Windows.Forms.DataVisualization.Charting.DateTimeIntervalType.Days;
            chartArea1.AxisY.LabelStyle.Format = "g";
            chartArea1.AxisY.Maximum = 20D;
            chartArea1.AxisY.Minimum = 0D;
            chartArea1.AxisY2.Maximum = 40D;
            chartArea1.AxisY2.Minimum = 0D;
            chartArea1.Name = "ChartArea1";
            this.chart1.ChartAreas.Add(chartArea1);
            this.chart1.DataSource = this.batteryMeasurementModelBindingSource;
            legend1.Name = "Legend1";
            this.chart1.Legends.Add(legend1);
            this.chart1.Location = new System.Drawing.Point(12, -7);
            this.chart1.Name = "chart1";
            series1.BorderWidth = 5;
            series1.ChartArea = "ChartArea1";
            series1.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line;
            series1.Legend = "Legend1";
            series1.Name = "Volts";
            series1.XValueMember = "Timestamp";
            series1.XValueType = System.Windows.Forms.DataVisualization.Charting.ChartValueType.DateTime;
            series1.YValueMembers = "Volts";
            series2.BorderWidth = 5;
            series2.ChartArea = "ChartArea1";
            series2.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line;
            series2.Legend = "Legend1";
            series2.Name = "Amps";
            series2.XValueMember = "Timestamp";
            series2.XValueType = System.Windows.Forms.DataVisualization.Charting.ChartValueType.DateTime;
            series2.YAxisType = System.Windows.Forms.DataVisualization.Charting.AxisType.Secondary;
            series2.YValueMembers = "Amps";
            series3.ChartArea = "ChartArea1";
            series3.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line;
            series3.IsVisibleInLegend = false;
            series3.Legend = "Legend1";
            series3.Name = "Empty";
            this.chart1.Series.Add(series1);
            this.chart1.Series.Add(series2);
            this.chart1.Series.Add(series3);
            this.chart1.Size = new System.Drawing.Size(904, 348);
            this.chart1.TabIndex = 14;
            this.chart1.Text = "chart1";
            title1.Font = new System.Drawing.Font("Microsoft Sans Serif", 20F, System.Drawing.FontStyle.Bold);
            title1.Name = "Battery Meter";
            title1.Text = "Battery Meter";
            this.chart1.Titles.Add(title1);
            // 
            // batteryMeasurementModelBindingSource
            // 
            this.batteryMeasurementModelBindingSource.DataMember = "BatteryMeasurementModel";
            this.batteryMeasurementModelBindingSource.DataSource = this.scumControllerBindingSource;
            // 
            // dataGridView1
            // 
            this.dataGridView1.AllowUserToAddRows = false;
            this.dataGridView1.AllowUserToDeleteRows = false;
            this.dataGridView1.AllowUserToOrderColumns = true;
            this.dataGridView1.AutoGenerateColumns = false;
            this.dataGridView1.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGridView1.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.Column1,
            this.Column2,
            this.voltsDataGridViewTextBoxColumn,
            this.ampsDataGridViewTextBoxColumn});
            this.dataGridView1.DataSource = this.batteryMeasurementModelBindingSource;
            this.dataGridView1.Location = new System.Drawing.Point(12, 347);
            this.dataGridView1.Name = "dataGridView1";
            this.dataGridView1.ReadOnly = true;
            this.dataGridView1.Size = new System.Drawing.Size(904, 324);
            this.dataGridView1.TabIndex = 15;
            // 
            // Column1
            // 
            this.Column1.DataPropertyName = "Timestamp";
            dataGridViewCellStyle1.Format = "d";
            dataGridViewCellStyle1.NullValue = null;
            this.Column1.DefaultCellStyle = dataGridViewCellStyle1;
            this.Column1.HeaderText = "Date";
            this.Column1.Name = "Column1";
            this.Column1.ReadOnly = true;
            // 
            // Column2
            // 
            this.Column2.DataPropertyName = "Timestamp";
            dataGridViewCellStyle2.Format = "T";
            dataGridViewCellStyle2.NullValue = null;
            this.Column2.DefaultCellStyle = dataGridViewCellStyle2;
            this.Column2.HeaderText = "Time";
            this.Column2.Name = "Column2";
            this.Column2.ReadOnly = true;
            // 
            // btnSave
            // 
            this.btnSave.Location = new System.Drawing.Point(450, 677);
            this.btnSave.Name = "btnSave";
            this.btnSave.Size = new System.Drawing.Size(98, 35);
            this.btnSave.TabIndex = 16;
            this.btnSave.Text = "Save Data";
            this.btnSave.UseVisualStyleBackColor = true;
            this.btnSave.Click += new System.EventHandler(this.btnSave_Click);
            // 
            // voltsDataGridViewTextBoxColumn
            // 
            this.voltsDataGridViewTextBoxColumn.DataPropertyName = "Volts";
            this.voltsDataGridViewTextBoxColumn.HeaderText = "Volts";
            this.voltsDataGridViewTextBoxColumn.Name = "voltsDataGridViewTextBoxColumn";
            this.voltsDataGridViewTextBoxColumn.ReadOnly = true;
            // 
            // ampsDataGridViewTextBoxColumn
            // 
            this.ampsDataGridViewTextBoxColumn.DataPropertyName = "Amps";
            this.ampsDataGridViewTextBoxColumn.HeaderText = "Amps";
            this.ampsDataGridViewTextBoxColumn.Name = "ampsDataGridViewTextBoxColumn";
            this.ampsDataGridViewTextBoxColumn.ReadOnly = true;
            // 
            // scumControllerBindingSource
            // 
            this.scumControllerBindingSource.DataSource = typeof(DataLogging.ScumController);
            // 
            // loggingView1
            // 
            this.loggingView1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.loggingView1.DrawMode = System.Windows.Forms.DrawMode.OwnerDrawFixed;
            this.loggingView1.FollowLastItem = true;
            this.loggingView1.FormattingEnabled = true;
            this.loggingView1.Items.AddRange(new object[] {
            "Logging"});
            this.loggingView1.Location = new System.Drawing.Point(12, 722);
            this.loggingView1.MaxEntriesInListBox = 3000;
            this.loggingView1.Name = "loggingView1";
            this.loggingView1.Size = new System.Drawing.Size(905, 82);
            this.loggingView1.TabIndex = 11;
            this.loggingView1.SelectedIndexChanged += new System.EventHandler(this.loggingView1_SelectedIndexChanged);
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(226, 677);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(98, 35);
            this.button1.TabIndex = 17;
            this.button1.Text = "Get Settings";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // ChartForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(929, 832);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.btnSave);
            this.Controls.Add(this.dataGridView1);
            this.Controls.Add(this.chart1);
            this.Controls.Add(this.btnDownloadData);
            this.Controls.Add(this.loggingView1);
            this.Controls.Add(this.statusStrip1);
            this.Controls.Add(this.buttonStartAcquisition);
            this.Controls.Add(this.buttonStopAcquisition);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "ChartForm";
            this.SizeGripStyle = System.Windows.Forms.SizeGripStyle.Hide;
            this.Text = "Scum Meter 9000";
            this.Load += new System.EventHandler(this.ChartForm_Load);
            this.Shown += new System.EventHandler(this.ChartFormShown);
            this.statusStrip1.ResumeLayout(false);
            this.statusStrip1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.chart1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.batteryMeasurementModelBindingSource)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.scumControllerBindingSource)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button buttonStopAcquisition;
        private System.Windows.Forms.Button buttonStartAcquisition;
        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabelProgress;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel1;
        private Tools.LoggingView loggingView1;
        private System.Windows.Forms.Button btnDownloadData;
        private System.Windows.Forms.DataVisualization.Charting.Chart chart1;
        private System.Windows.Forms.BindingSource scumControllerBindingSource;
        private System.Windows.Forms.BindingSource batteryMeasurementModelBindingSource;
        private System.Windows.Forms.DataGridView dataGridView1;
        private System.Windows.Forms.DataGridViewTextBoxColumn Column1;
        private System.Windows.Forms.DataGridViewTextBoxColumn Column2;
        private System.Windows.Forms.DataGridViewTextBoxColumn voltsDataGridViewTextBoxColumn;
        private System.Windows.Forms.DataGridViewTextBoxColumn ampsDataGridViewTextBoxColumn;
        private System.Windows.Forms.Button btnSave;
        private System.Windows.Forms.Button button1;
    }
}

