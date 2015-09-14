namespace Motionsensor
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea3 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Series series3 = new System.Windows.Forms.DataVisualization.Charting.Series();
            System.Windows.Forms.DataVisualization.Charting.Title title3 = new System.Windows.Forms.DataVisualization.Charting.Title();
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea4 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Series series4 = new System.Windows.Forms.DataVisualization.Charting.Series();
            System.Windows.Forms.DataVisualization.Charting.Title title4 = new System.Windows.Forms.DataVisualization.Charting.Title();
            this.chart_sensor1 = new System.Windows.Forms.DataVisualization.Charting.Chart();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.yDistanceLabel = new System.Windows.Forms.Label();
            this.xDistanceLabel = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.get_parameters = new System.Windows.Forms.Button();
            this.ld2 = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.ld1 = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.dpi2 = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.dpi1 = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.start_sampling = new System.Windows.Forms.Button();
            this.set_parameters = new System.Windows.Forms.Button();
            this.init_sensor = new System.Windows.Forms.Button();
            this.connect_com = new System.Windows.Forms.Button();
            this.comPort = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.sensor_serial = new System.IO.Ports.SerialPort(this.components);
            this.sensor_timer = new System.Windows.Forms.Timer(this.components);
            this.gui_timer = new System.Windows.Forms.Timer(this.components);
            this.chart_sensor2 = new System.Windows.Forms.DataVisualization.Charting.Chart();
            ((System.ComponentModel.ISupportInitialize)(this.chart_sensor1)).BeginInit();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.chart_sensor2)).BeginInit();
            this.SuspendLayout();
            // 
            // chart_sensor1
            // 
            this.chart_sensor1.BackColor = System.Drawing.Color.Transparent;
            chartArea3.AxisX.MajorGrid.Enabled = false;
            chartArea3.AxisX.Title = "Time";
            chartArea3.AxisY.MajorGrid.Enabled = false;
            chartArea3.AxisY.MajorGrid.Interval = 0D;
            chartArea3.AxisY.MajorTickMark.Enabled = false;
            chartArea3.AxisY.Maximum = 20D;
            chartArea3.AxisY.Minimum = -20D;
            chartArea3.AxisY.Title = "Speed";
            chartArea3.BackColor = System.Drawing.Color.White;
            chartArea3.BorderColor = System.Drawing.Color.Transparent;
            chartArea3.Name = "ChartArea1";
            this.chart_sensor1.ChartAreas.Add(chartArea3);
            this.chart_sensor1.Location = new System.Drawing.Point(-7, -30);
            this.chart_sensor1.Margin = new System.Windows.Forms.Padding(0);
            this.chart_sensor1.Name = "chart_sensor1";
            series3.ChartArea = "ChartArea1";
            series3.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.FastLine;
            series3.Color = System.Drawing.Color.OrangeRed;
            series3.IsVisibleInLegend = false;
            series3.IsXValueIndexed = true;
            series3.Name = "Series1";
            series3.YValueType = System.Windows.Forms.DataVisualization.Charting.ChartValueType.Double;
            this.chart_sensor1.Series.Add(series3);
            this.chart_sensor1.Size = new System.Drawing.Size(461, 328);
            this.chart_sensor1.TabIndex = 0;
            this.chart_sensor1.Text = "sensor_1";
            this.chart_sensor1.TextAntiAliasingQuality = System.Windows.Forms.DataVisualization.Charting.TextAntiAliasingQuality.Normal;
            title3.Name = "Sensor 1";
            this.chart_sensor1.Titles.Add(title3);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.yDistanceLabel);
            this.groupBox1.Controls.Add(this.xDistanceLabel);
            this.groupBox1.Controls.Add(this.label7);
            this.groupBox1.Controls.Add(this.label6);
            this.groupBox1.Controls.Add(this.get_parameters);
            this.groupBox1.Controls.Add(this.ld2);
            this.groupBox1.Controls.Add(this.label5);
            this.groupBox1.Controls.Add(this.ld1);
            this.groupBox1.Controls.Add(this.label4);
            this.groupBox1.Controls.Add(this.dpi2);
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.dpi1);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.start_sampling);
            this.groupBox1.Controls.Add(this.set_parameters);
            this.groupBox1.Controls.Add(this.init_sensor);
            this.groupBox1.Controls.Add(this.connect_com);
            this.groupBox1.Controls.Add(this.comPort);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Location = new System.Drawing.Point(457, 12);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(330, 357);
            this.groupBox1.TabIndex = 1;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Sensor configuration";
            // 
            // yDistanceLabel
            // 
            this.yDistanceLabel.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.yDistanceLabel.Location = new System.Drawing.Point(160, 304);
            this.yDistanceLabel.Name = "yDistanceLabel";
            this.yDistanceLabel.Size = new System.Drawing.Size(70, 22);
            this.yDistanceLabel.TabIndex = 19;
            this.yDistanceLabel.Text = "0";
            // 
            // xDistanceLabel
            // 
            this.xDistanceLabel.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.xDistanceLabel.Location = new System.Drawing.Point(160, 245);
            this.xDistanceLabel.Name = "xDistanceLabel";
            this.xDistanceLabel.Size = new System.Drawing.Size(70, 22);
            this.xDistanceLabel.TabIndex = 18;
            this.xDistanceLabel.Text = "0";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(21, 305);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(133, 13);
            this.label7.TabIndex = 17;
            this.label7.Text = "Sensor 2 distance traveled";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(21, 246);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(133, 13);
            this.label6.TabIndex = 16;
            this.label6.Text = "Sensor 1 distance traveled";
            // 
            // get_parameters
            // 
            this.get_parameters.Enabled = false;
            this.get_parameters.Location = new System.Drawing.Point(207, 124);
            this.get_parameters.Name = "get_parameters";
            this.get_parameters.Size = new System.Drawing.Size(108, 29);
            this.get_parameters.TabIndex = 15;
            this.get_parameters.Text = "Get parameters";
            this.get_parameters.UseVisualStyleBackColor = true;
            this.get_parameters.Click += new System.EventHandler(this.get_parameters_Click);
            // 
            // ld2
            // 
            this.ld2.Location = new System.Drawing.Point(91, 148);
            this.ld2.Name = "ld2";
            this.ld2.Size = new System.Drawing.Size(60, 20);
            this.ld2.TabIndex = 14;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(21, 151);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(37, 13);
            this.label5.TabIndex = 13;
            this.label5.Text = "LD #2";
            // 
            // ld1
            // 
            this.ld1.Location = new System.Drawing.Point(91, 123);
            this.ld1.Name = "ld1";
            this.ld1.Size = new System.Drawing.Size(60, 20);
            this.ld1.TabIndex = 12;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(21, 126);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(37, 13);
            this.label4.TabIndex = 11;
            this.label4.Text = "LD #1";
            // 
            // dpi2
            // 
            this.dpi2.Location = new System.Drawing.Point(91, 100);
            this.dpi2.Name = "dpi2";
            this.dpi2.Size = new System.Drawing.Size(60, 20);
            this.dpi2.TabIndex = 10;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(21, 103);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(41, 13);
            this.label3.TabIndex = 9;
            this.label3.Text = "DPI #2";
            // 
            // dpi1
            // 
            this.dpi1.Location = new System.Drawing.Point(91, 78);
            this.dpi1.Name = "dpi1";
            this.dpi1.Size = new System.Drawing.Size(60, 20);
            this.dpi1.TabIndex = 8;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(21, 81);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(41, 13);
            this.label2.TabIndex = 7;
            this.label2.Text = "DPI #1";
            // 
            // start_sampling
            // 
            this.start_sampling.Enabled = false;
            this.start_sampling.Location = new System.Drawing.Point(207, 159);
            this.start_sampling.Name = "start_sampling";
            this.start_sampling.Size = new System.Drawing.Size(108, 29);
            this.start_sampling.TabIndex = 6;
            this.start_sampling.Text = "Start sampling";
            this.start_sampling.UseVisualStyleBackColor = true;
            this.start_sampling.Click += new System.EventHandler(this.start_sampling_Click);
            // 
            // set_parameters
            // 
            this.set_parameters.Enabled = false;
            this.set_parameters.Location = new System.Drawing.Point(207, 89);
            this.set_parameters.Name = "set_parameters";
            this.set_parameters.Size = new System.Drawing.Size(108, 29);
            this.set_parameters.TabIndex = 5;
            this.set_parameters.Text = "Set parameters";
            this.set_parameters.UseVisualStyleBackColor = true;
            this.set_parameters.Click += new System.EventHandler(this.set_parameters_Click);
            // 
            // init_sensor
            // 
            this.init_sensor.Enabled = false;
            this.init_sensor.Location = new System.Drawing.Point(207, 54);
            this.init_sensor.Name = "init_sensor";
            this.init_sensor.Size = new System.Drawing.Size(108, 29);
            this.init_sensor.TabIndex = 4;
            this.init_sensor.Text = "Initialize sensor";
            this.init_sensor.UseVisualStyleBackColor = true;
            this.init_sensor.Click += new System.EventHandler(this.init_sensor_Click);
            // 
            // connect_com
            // 
            this.connect_com.Location = new System.Drawing.Point(207, 19);
            this.connect_com.Name = "connect_com";
            this.connect_com.Size = new System.Drawing.Size(108, 29);
            this.connect_com.TabIndex = 3;
            this.connect_com.Text = "Connect COM";
            this.connect_com.UseVisualStyleBackColor = true;
            this.connect_com.Click += new System.EventHandler(this.connect_com_Click);
            // 
            // comPort
            // 
            this.comPort.Location = new System.Drawing.Point(91, 39);
            this.comPort.Name = "comPort";
            this.comPort.Size = new System.Drawing.Size(60, 20);
            this.comPort.TabIndex = 1;
            this.comPort.Text = "COM8";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(21, 42);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(64, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "COM-PORT";
            // 
            // sensor_timer
            // 
            this.sensor_timer.Interval = 16;
            this.sensor_timer.Tick += new System.EventHandler(this.sensor_timer_Tick);
            // 
            // gui_timer
            // 
            this.gui_timer.Interval = 50;
            this.gui_timer.Tick += new System.EventHandler(this.chart_timer_Tick);
            // 
            // chart_sensor2
            // 
            this.chart_sensor2.BackColor = System.Drawing.Color.Transparent;
            chartArea4.AxisX.MajorGrid.Enabled = false;
            chartArea4.AxisX.Title = "Time";
            chartArea4.AxisY.MajorGrid.Enabled = false;
            chartArea4.AxisY.MajorGrid.Interval = 0D;
            chartArea4.AxisY.MajorTickMark.Enabled = false;
            chartArea4.AxisY.Maximum = 20D;
            chartArea4.AxisY.Minimum = -20D;
            chartArea4.AxisY.Title = "Speed";
            chartArea4.BackColor = System.Drawing.Color.White;
            chartArea4.BorderColor = System.Drawing.Color.Transparent;
            chartArea4.Name = "ChartArea1";
            this.chart_sensor2.ChartAreas.Add(chartArea4);
            this.chart_sensor2.Location = new System.Drawing.Point(-7, 249);
            this.chart_sensor2.Margin = new System.Windows.Forms.Padding(0);
            this.chart_sensor2.Name = "chart_sensor2";
            series4.ChartArea = "ChartArea1";
            series4.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.FastLine;
            series4.Color = System.Drawing.Color.OrangeRed;
            series4.IsVisibleInLegend = false;
            series4.IsXValueIndexed = true;
            series4.Name = "Series1";
            series4.YValueType = System.Windows.Forms.DataVisualization.Charting.ChartValueType.Double;
            this.chart_sensor2.Series.Add(series4);
            this.chart_sensor2.Size = new System.Drawing.Size(461, 328);
            this.chart_sensor2.TabIndex = 2;
            this.chart_sensor2.Text = "sensor_1";
            this.chart_sensor2.TextAntiAliasingQuality = System.Windows.Forms.DataVisualization.Charting.TextAntiAliasingQuality.Normal;
            title4.Name = "Sensor 1";
            this.chart_sensor2.Titles.Add(title4);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(796, 530);
            this.Controls.Add(this.chart_sensor2);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.chart_sensor1);
            this.Name = "Form1";
            this.Text = "Motion sensor";
            ((System.ComponentModel.ISupportInitialize)(this.chart_sensor1)).EndInit();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.chart_sensor2)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.DataVisualization.Charting.Chart chart_sensor1;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.TextBox ld2;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox ld1;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox dpi2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox dpi1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button start_sampling;
        private System.Windows.Forms.Button set_parameters;
        private System.Windows.Forms.Button init_sensor;
        private System.Windows.Forms.Button connect_com;
        private System.Windows.Forms.TextBox comPort;
        private System.Windows.Forms.Label label1;
        private System.IO.Ports.SerialPort sensor_serial;
        private System.Windows.Forms.Timer sensor_timer;
        private System.Windows.Forms.Button get_parameters;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label yDistanceLabel;
        private System.Windows.Forms.Label xDistanceLabel;
        private System.Windows.Forms.Timer gui_timer;
        private System.Windows.Forms.DataVisualization.Charting.Chart chart_sensor2;
    }
}

