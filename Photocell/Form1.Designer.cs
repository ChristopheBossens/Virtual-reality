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
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea5 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Series series5 = new System.Windows.Forms.DataVisualization.Charting.Series();
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
            ((System.ComponentModel.ISupportInitialize)(this.data_chart)).BeginInit();
            this.SuspendLayout();
            // 
            // start_recording_button
            // 
            this.start_recording_button.Location = new System.Drawing.Point(415, 212);
            this.start_recording_button.Name = "start_recording_button";
            this.start_recording_button.Size = new System.Drawing.Size(136, 54);
            this.start_recording_button.TabIndex = 0;
            this.start_recording_button.Text = "Start recording";
            this.start_recording_button.UseVisualStyleBackColor = true;
            this.start_recording_button.Click += new System.EventHandler(this.start_recording_button_Click);
            // 
            // data_chart
            // 
            chartArea5.AxisY.Maximum = 1200D;
            chartArea5.AxisY.Minimum = 0D;
            chartArea5.Name = "ChartArea1";
            this.data_chart.ChartAreas.Add(chartArea5);
            this.data_chart.Location = new System.Drawing.Point(12, 12);
            this.data_chart.Name = "data_chart";
            series5.ChartArea = "ChartArea1";
            series5.ChartType = System.Windows.Forms.DataVisualization.Charting.SeriesChartType.Line;
            series5.Name = "Series1";
            series5.YValueType = System.Windows.Forms.DataVisualization.Charting.ChartValueType.Int32;
            this.data_chart.Series.Add(series5);
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
            this.label1.Location = new System.Drawing.Point(18, 212);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(121, 13);
            this.label1.TabIndex = 2;
            this.label1.Text = "Transmission frequency:";
            // 
            // transmission_frequency
            // 
            this.transmission_frequency.AutoSize = true;
            this.transmission_frequency.Location = new System.Drawing.Point(145, 212);
            this.transmission_frequency.Name = "transmission_frequency";
            this.transmission_frequency.Size = new System.Drawing.Size(35, 13);
            this.transmission_frequency.TabIndex = 3;
            this.transmission_frequency.Text = "label2";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(18, 233);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(85, 13);
            this.label2.TabIndex = 4;
            this.label2.Text = "Function triggers";
            // 
            // trigger_count
            // 
            this.trigger_count.AutoSize = true;
            this.trigger_count.Location = new System.Drawing.Point(145, 233);
            this.trigger_count.Name = "trigger_count";
            this.trigger_count.Size = new System.Drawing.Size(35, 13);
            this.trigger_count.TabIndex = 5;
            this.trigger_count.Text = "label2";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(18, 253);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(77, 13);
            this.label3.TabIndex = 6;
            this.label3.Text = "Bytes received";
            // 
            // byte_count
            // 
            this.byte_count.AutoSize = true;
            this.byte_count.Location = new System.Drawing.Point(145, 253);
            this.byte_count.Name = "byte_count";
            this.byte_count.Size = new System.Drawing.Size(35, 13);
            this.byte_count.TabIndex = 7;
            this.byte_count.Text = "label2";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(18, 275);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(78, 13);
            this.label4.TabIndex = 8;
            this.label4.Text = "Bytes available";
            // 
            // bytes_available
            // 
            this.bytes_available.AutoSize = true;
            this.bytes_available.Location = new System.Drawing.Point(145, 275);
            this.bytes_available.Name = "bytes_available";
            this.bytes_available.Size = new System.Drawing.Size(35, 13);
            this.bytes_available.TabIndex = 9;
            this.bytes_available.Text = "label2";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(563, 311);
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
    }
}

