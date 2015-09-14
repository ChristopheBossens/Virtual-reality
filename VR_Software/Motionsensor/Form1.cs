using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Motionsensor
{
    enum SensorState
    {
        CLOSED,
        UNINITIALIZED,
        INITIALIZED,
        READING
    };

    
    public partial class Form1 : Form
    {
        int xTicks, yTicks;
        double xVelocity, yVelocity, xScaling, yScaling;
        double[] xVelocityBuffer, yVelocityBuffer;
        double xDistance, yDistance, xTotalDistance, yTotalDistance;
        int bufferSize = 100;       

        System.Diagnostics.Stopwatch stopwatch = new System.Diagnostics.Stopwatch();
        long lastTime, elapsedTime;

        private byte[] byteBuffer = new byte[5];
        SensorState sensorState = SensorState.CLOSED;

        public Form1()
        {
            InitializeComponent();
        }

        private void connect_com_Click(object sender, EventArgs e)
        {
            if (sensorState == SensorState.CLOSED)
            {
                try
                {
                    sensor_serial = new System.IO.Ports.SerialPort();
                    sensor_serial.ReadTimeout = 2000;
                    sensor_serial.PortName = comPort.Text;
                    sensor_serial.BaudRate = 9600;
                    sensor_serial.Open();
                    SetButtonState(1, 1, 0, 0, 0);
                    sensorState = SensorState.UNINITIALIZED;
                    connect_com.Text = "Disconnect COM";
                }
                catch (Exception exception)
                {
                    MessageBox.Show(exception.Message);
                }
            }
            else
            {
                if (sensorState == SensorState.READING)
                {
                    sensor_timer.Enabled = false;
                    gui_timer.Enabled = false;
                }
                try
                {
                    sensor_serial.Close();
                }
                catch
                { }

                sensor_serial.Dispose();
                sensorState = SensorState.CLOSED;
                connect_com.Text = "Connect COM";
                SetButtonState(1, 0, 0, 0, 0);
            }
        }

        private void SetButtonState(int button1, int button2, int button3, int button4, int button5)
        {
            connect_com.Enabled = (button1 == 1);
            init_sensor.Enabled = (button2 == 1);
            set_parameters.Enabled = (button3 == 1);
            get_parameters.Enabled = (button4 == 1);
            start_sampling.Enabled = (button5 == 1);
        }

        private void init_sensor_Click(object sender, EventArgs e)
        {
            byteBuffer[0] = 0x01;
            try
            {
                sensor_serial.Write(byteBuffer, 0, 1);
                sensor_serial.Read(byteBuffer, 0, 1);

                if (byteBuffer[0] == 0x01)
                {
                    sensorState = SensorState.INITIALIZED;
                    SetButtonState(1, 0, 1, 1, 0);
                }
            }
            catch (Exception exception)
            {
                MessageBox.Show(exception.Message);
            }

        }

        private void set_parameters_Click(object sender, EventArgs e)
        {
            try
            {
                byte dpi1 = Convert.ToByte(this.dpi1.Text);
                byte dpi2 = Convert.ToByte(this.dpi2.Text);
                byte ld1 = Convert.ToByte(this.ld1.Text);
                byte ld2 = Convert.ToByte(this.ld2.Text);

                byteBuffer[0] = 0x04;
                byteBuffer[1] = dpi1;
                byteBuffer[2] = dpi2;
                byteBuffer[3] = ld1;
                byteBuffer[4] = ld2;

                sensor_serial.Write(byteBuffer, 0, 5);
                SetButtonState(1, 0, 1, 1, 1);
            }
            catch (Exception exception)
            {
                MessageBox.Show(exception.Message);
            }
        }

        private void get_parameters_Click(object sender, EventArgs e)
        {
            try
            {
                byteBuffer[0] = 0x03;
                sensor_serial.Write(byteBuffer, 0, 1);
                sensor_serial.Read(byteBuffer, 0, 1);
                sensor_serial.Read(byteBuffer, 1, 1);
                sensor_serial.Read(byteBuffer, 2, 1);
                sensor_serial.Read(byteBuffer, 3, 1);

                this.dpi1.Text = byteBuffer[0].ToString();
                this.dpi2.Text = byteBuffer[1].ToString();
                this.ld1.Text = byteBuffer[2].ToString();
                this.ld2.Text = byteBuffer[3].ToString();

                SetButtonState(1, 0, 1, 1, 1);
            }
            catch (Exception exception)
            {
                MessageBox.Show(exception.Message);
            }
        }

        private void start_sampling_Click(object sender, EventArgs e)
        {
            if (sensorState != SensorState.READING)
            {
                Initialize();
                
                byteBuffer[0] = 0x05;
                sensor_serial.Write(byteBuffer, 0, 1);
                
                stopwatch.Start();
                lastTime = stopwatch.ElapsedMilliseconds;

                sensor_timer.Enabled = true;
                gui_timer.Enabled = true;
                SetButtonState(1, 0, 0, 0, 1);
                start_sampling.Text = "Stop sampling";
                sensorState = SensorState.READING;
            }
            else
            {
                sensor_timer.Enabled = false;
                gui_timer.Enabled = false;
                stopwatch.Stop();

                byteBuffer[0] = 0x06;
                sensor_serial.Write(byteBuffer, 0, 1);
                
                SetButtonState(1, 0, 1, 1, 1);
                start_sampling.Text = "Start sampling";
                sensorState = SensorState.INITIALIZED;
            }
        }

        private void sensor_timer_Tick(object sender, EventArgs e)
        {
            byteBuffer[0] = 0x07;
            sensor_serial.Write(byteBuffer, 0, 1);
            sensor_serial.Read(byteBuffer, 0, 1);
            sensor_serial.Read(byteBuffer, 1, 1);
            sensor_serial.Read(byteBuffer, 2, 1);
            sensor_serial.Read(byteBuffer, 3, 1);
            
            xTicks = BitConverter.ToInt16(byteBuffer, 0);
            yTicks = BitConverter.ToInt16(byteBuffer, 2);

            CalculateVelocity(xTicks, yTicks);
        }

        private void chart_timer_Tick(object sender, EventArgs e)
        {
            UpdateGUI();
        }

        private void CalculateVelocity(int xTicks, int yTicks)
        {
            elapsedTime = stopwatch.ElapsedMilliseconds - lastTime;
            lastTime = stopwatch.ElapsedMilliseconds;

            xDistance = (double)xTicks * xScaling;
            yDistance = (double)yTicks * yScaling;

            xVelocity = xDistance * (1000.0/elapsedTime);
            yVelocity = yDistance * (1000.0/elapsedTime);

            xTotalDistance += xDistance;
            yTotalDistance += yDistance;

            Buffer.BlockCopy(xVelocityBuffer, sizeof(double), xVelocityBuffer, 0, sizeof(double) * (bufferSize - 1));
            Buffer.BlockCopy(yVelocityBuffer, sizeof(double), yVelocityBuffer, 0, sizeof(double) * (bufferSize - 1));
            xVelocityBuffer[bufferSize - 1] = xVelocity;
            yVelocityBuffer[bufferSize - 1] = yVelocity;
        }

        private void Initialize()
        {
            // Parameters for converting sensor counts to velocity
            xTotalDistance = 0.0;
            yTotalDistance = 0.0;

            xScaling = 2.54 / (200.0 * Convert.ToDouble(this.dpi1.Text));
            yScaling = 2.54 / (200.0 * Convert.ToDouble(this.dpi2.Text));

            xVelocityBuffer = new double[bufferSize];
            yVelocityBuffer = new double[bufferSize];

            // Initialization of GUI-elements
            chart_sensor1.Series[0].Points.Clear();
            chart_sensor2.Series[0].Points.Clear();

            for (int i = 0; i < bufferSize; ++i)
            {
                chart_sensor1.Series[0].Points.AddY(0);
                chart_sensor2.Series[0].Points.AddY(0);
            }
            UpdateGUI();
        }

        private void UpdateGUI()
        {
            for (int i = 0; i < bufferSize; ++i)
            {
                chart_sensor1.Series[0].Points[i].SetValueY(xVelocityBuffer[i]);
                chart_sensor2.Series[0].Points[i].SetValueY(yVelocityBuffer[i]);
            }

            xDistanceLabel.Text = String.Format("{0:F4}", xTotalDistance);
            yDistanceLabel.Text = String.Format("{0:F4}", yTotalDistance);

            chart_sensor1.Invalidate();
            chart_sensor2.Invalidate();
        }
    }
}
