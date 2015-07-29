namespace DAQ
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
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea2 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Series series2 = new System.Windows.Forms.DataVisualization.Charting.Series();
            this.start_recording_button = new System.Windows.Forms.Button();
            this.data_chart = new System.Windows.Forms.DataVisualization.Charting.Chart();
            this.serial_port = new System.IO.Ports.SerialPort(this.components);
            this.update_timer = new System.Windows.Forms.Timer(this.components);
            this.label1 = new System.Windows.Forms.Label();
            this.transmission_frequency = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.trigger_count = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.byte_count = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.bytes_available = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.packets_received = new System.Windows.Forms.Label();
            this.start_udp_button = new System.Windows.Forms.Button();
            this.label7 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.baudRate = new System.Windows.Forms.TextBox();
            this.udpPort = new System.Windows.Forms.TextBox();
            this.label9 = new System.Windows.Forms.Label();
            this.comPort = new System.Windows.Forms.TextBox();
            ((System.ComponentModel.ISupportInitialize)(this.data_chart)).BeginInit();
            this.SuspendLayout();
            // 
            // start_recording_button
            // 
            this.start_recording_button.Location = new System.Drawing.Point(415, 269);
            this.start_recording_button.Name = "start_recording_button";
            this.start_recording_button.Size = new System.Drawing.Size(136, 54);
            this.start_recording_button.TabIndex = 0;
            this.start_recording_button.Text = "Start recording";
            this.start_recording_button.UseVisualStyleBackColor = true;
            this.start_recording_button.Click += new System.EventHandler(this.start_recording_button_Click);
            // 
            // data_chart
            // 
            chartArea2.AxisY.Maximum = 1200D;
            chartArea2.AxisY.Minimum = 0D;
            chartArea2.Name = "ChartArea1";
            this.data_chart.ChartAreas.Add(chartArea2);
            this.data_chart.Location = new System.Drawing.Point(12, 12);
            this.data_chart.Name = "data_chart";
            series2.ChartArea = "ChartArea1";
            series2.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line;
            series2.Name = "Series1";
            series2.YValueType = System.Windows.Forms.DataVisualization.Charting.ChartValueType.Int32;
            this.data_chart.Series.Add(series2);
            this.data_chart.Size = new System.Drawing.Size(539, 191);
            this.data_chart.TabIndex = 1;
            this.data_chart.Text = "chart1";
            // 
            // serial_port
            // 
            this.serial_port.BaudRate = 250000;
            this.serial_port.PortName = "COM3";
            this.serial_port.ReadTimeout = 100;
            this.serial_port.ReceivedBytesThreshold = 50;
            // 
            // update_timer
            // 
            this.update_timer.Interval = 50;
            this.update_timer.Tick += new System.EventHandler(this.update_timer_Tick);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(15, 224);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(121, 13);
            this.label1.TabIndex = 2;
            this.label1.Text = "Transmission frequency:";
            // 
            // transmission_frequency
            // 
            this.transmission_frequency.AutoSize = true;
            this.transmission_frequency.Location = new System.Drawing.Point(142, 224);
            this.transmission_frequency.Name = "transmission_frequency";
            this.transmission_frequency.Size = new System.Drawing.Size(35, 13);
            this.transmission_frequency.TabIndex = 3;
            this.transmission_frequency.Text = "label2";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(15, 244);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(85, 13);
            this.label2.TabIndex = 4;
            this.label2.Text = "Function triggers";
            // 
            // trigger_count
            // 
            this.trigger_count.AutoSize = true;
            this.trigger_count.Location = new System.Drawing.Point(142, 245);
            this.trigger_count.Name = "trigger_count";
            this.trigger_count.Size = new System.Drawing.Size(35, 13);
            this.trigger_count.TabIndex = 5;
            this.trigger_count.Text = "label2";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(15, 264);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(77, 13);
            this.label3.TabIndex = 6;
            this.label3.Text = "Bytes received";
            // 
            // byte_count
            // 
            this.byte_count.AutoSize = true;
            this.byte_count.Location = new System.Drawing.Point(142, 265);
            this.byte_count.Name = "byte_count";
            this.byte_count.Size = new System.Drawing.Size(35, 13);
            this.byte_count.TabIndex = 7;
            this.byte_count.Text = "label2";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(15, 284);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(78, 13);
            this.label4.TabIndex = 8;
            this.label4.Text = "Bytes available";
            // 
            // bytes_available
            // 
            this.bytes_available.AutoSize = true;
            this.bytes_available.Location = new System.Drawing.Point(142, 287);
            this.bytes_available.Name = "bytes_available";
            this.bytes_available.Size = new System.Drawing.Size(35, 13);
            this.bytes_available.TabIndex = 9;
            this.bytes_available.Text = "label2";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(15, 304);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(118, 13);
            this.label6.TabIndex = 11;
            this.label6.Text = "UDP packets received:";
            // 
            // packets_received
            // 
            this.packets_received.AutoSize = true;
            this.packets_received.Location = new System.Drawing.Point(142, 306);
            this.packets_received.Name = "packets_received";
            this.packets_received.Size = new System.Drawing.Size(35, 13);
            this.packets_received.TabIndex = 13;
            this.packets_received.Text = "label2";
            // 
            // start_udp_button
            // 
            this.start_udp_button.Location = new System.Drawing.Point(415, 209);
            this.start_udp_button.Name = "start_udp_button";
            this.start_udp_button.Size = new System.Drawing.Size(136, 54);
            this.start_udp_button.TabIndex = 14;
            this.start_udp_button.Text = "Start UDP";
            this.start_udp_button.UseVisualStyleBackColor = true;
            this.start_udp_button.Click += new System.EventHandler(this.start_udp_button_Click);
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(245, 224);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(53, 13);
            this.label7.TabIndex = 15;
            this.label7.Text = "Baud rate";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(245, 246);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(51, 13);
            this.label8.TabIndex = 16;
            this.label8.Text = "UDP port";
            // 
            // baudRate
            // 
            this.baudRate.Location = new System.Drawing.Point(340, 221);
            this.baudRate.Name = "baudRate";
            this.baudRate.Size = new System.Drawing.Size(54, 20);
            this.baudRate.TabIndex = 17;
            this.baudRate.Text = "250000";
            // 
            // udpPort
            // 
            this.udpPort.Location = new System.Drawing.Point(340, 243);
            this.udpPort.Name = "udpPort";
            this.udpPort.Size = new System.Drawing.Size(54, 20);
            this.udpPort.TabIndex = 18;
            this.udpPort.Text = "6789";
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(246, 269);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(52, 13);
            this.label9.TabIndex = 19;
            this.label9.Text = "COM port";
            // 
            // comPort
            // 
            this.comPort.Location = new System.Drawing.Point(340, 266);
            this.comPort.Name = "comPort";
            this.comPort.Size = new System.Drawing.Size(54, 20);
            this.comPort.TabIndex = 20;
            this.comPort.Text = "COM8";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(571, 334);
            this.Controls.Add(this.comPort);
            this.Controls.Add(this.label9);
            this.Controls.Add(this.udpPort);
            this.Controls.Add(this.baudRate);
            this.Controls.Add(this.label8);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.start_udp_button);
            this.Controls.Add(this.packets_received);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.bytes_available);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.byte_count);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.trigger_count);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.transmission_frequency);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.data_chart);
            this.Controls.Add(this.start_recording_button);
            this.Name = "Form1";
            this.Text = "Sensor measurement";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.Load += new System.EventHandler(this.Form1_Load);
            ((System.ComponentModel.ISupportInitialize)(this.data_chart)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button start_recording_button;
        private System.Windows.Forms.DataVisualization.Charting.Chart data_chart;
        private System.IO.Ports.SerialPort serial_port;
        private System.Windows.Forms.Timer update_timer;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label transmission_frequency;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label trigger_count;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label byte_count;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label bytes_available;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label packets_received;
        private System.Windows.Forms.Button start_udp_button;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.TextBox baudRate;
        private System.Windows.Forms.TextBox udpPort;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.TextBox comPort;
    }
}

