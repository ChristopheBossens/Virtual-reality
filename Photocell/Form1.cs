using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;
using System.Windows.Forms;

namespace DAQ
{
    public partial class Form1 : Form
    {
        const int bufferSize = 1024, byteBufferSize = 2;
        int bufferCount = 0, triggerCount = 0, byteCount = 0;
        bool isRecording;
        short[] dataBuffer;
        Thread serialThread;

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            // Prepare data buffer and chart area
            dataBuffer = new short[bufferSize];
            data_chart.Series[0].Points.Clear();
            for (int i = 0; i < bufferSize; ++i)
                data_chart.Series[0].Points.AddY(0);
            
            // Clear GUI elements
            isRecording = false;
            transmission_frequency.Text = "";
            byte_count.Text = "";
            trigger_count.Text = "";
            bytes_available.Text = "";
        }

        private void start_recording_button_Click(object sender, EventArgs e)
        {
            if (isRecording)
            {
                isRecording = false;
                update_timer.Enabled = false;
                start_recording_button.Text = "Start recording";
                serialThread.Join();
                serial_port.Close();
            }
            else
            {
                isRecording = true;
                update_timer.Enabled = true;
                start_recording_button.Text = "Stop recording";

                serial_port.Open();
                serialThread = new Thread(SerialRead);
                serialThread.Name = "Serial logging thread";
                serialThread.Priority = ThreadPriority.Highest;
                serialThread.Start();
            }
        }
    
        // Update function that takes care of plotting the data
        private void update_timer_Tick(object sender, EventArgs e)
        {
            for (int i = 0; i < bufferSize; ++i)
                data_chart.Series[0].Points[i].SetValueY(dataBuffer[i]);
            
            transmission_frequency.Text = ((double)1000.0*bufferCount / update_timer.Interval).ToString();
            trigger_count.Text = triggerCount.ToString();
            byte_count.Text = byteCount.ToString();
            bytes_available.Text = serial_port.BytesToRead.ToString();
            bufferCount = 0;
            data_chart.Invalidate();
        }

        private void SerialRead()
        {
            // The code continuously looks for sync bytes, after which a valid measurement is 
            // added to the data buffer
            ++triggerCount;
            int shortBufferSize = byteBufferSize / 2;
            byte[] readBuffer = new byte[byteBufferSize];
            short[] convertBuffer = new short[shortBufferSize];

            while (isRecording)
            {
                if (serial_port.ReadByte() == 0xff)
                {
                    if (serial_port.ReadByte() == 0xff)
                    {
                        serial_port.Read(readBuffer,0,byteBufferSize);
                        Buffer.BlockCopy(readBuffer, 0, convertBuffer, 0, byteBufferSize);
                        Buffer.BlockCopy(dataBuffer, byteBufferSize, dataBuffer, 0, sizeof(short) * bufferSize - byteBufferSize);
                        dataBuffer[bufferSize-1] = convertBuffer[0];

                        bufferCount += shortBufferSize;
                        byteCount += byteBufferSize +2;
                    }
                }
            }
        }
    }
}
