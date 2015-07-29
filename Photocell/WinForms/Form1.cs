using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Windows.Forms;

namespace DAQ
{
    public partial class Form1 : Form
    {
        const int bufferSize = 1024, byteBufferSize = 2;
        int bufferCount = 0, triggerCount = 0, byteCount = 0;
        int udpBytesReceived = 0;
        bool isRecording;
        short[] dataBuffer;

        //int UDPport = 6789;

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
            transmission_frequency.Text = "0";
            byte_count.Text = "0";
            trigger_count.Text = "0";
            bytes_available.Text = "0";
            packets_received.Text = "0";
        }

        private void start_recording_button_Click(object sender, EventArgs e)
        {
            if (isRecording)
            {
                comPort.Enabled = true;
                baudRate.Enabled = true;
                isRecording = false;
                update_timer.Enabled = false;
                start_recording_button.Text = "Start recording";
                serialThread.Join();
                serial_port.Close();
            }
            else
            {
                serial_port.PortName = comPort.Text;
                serial_port.BaudRate = Convert.ToInt32(baudRate.Text);
                try
                {
                    serial_port.Open();
                }
                catch (Exception exc)
                {
                    MessageBox.Show(exc.Message);
                    return;
                }
                isRecording = true;
                update_timer.Enabled = true;
                start_recording_button.Text = "Stop recording";

                comPort.Enabled = false;
                baudRate.Enabled = false;

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

        UdpClient UDPlistener;
        private void start_udp_button_Click(object sender, EventArgs e)
        {
            if (UDPlistener == null)
            {
                UDPlistener = new UdpClient(Convert.ToInt32(udpPort.Text));
                UDPlistener.BeginReceive(new AsyncCallback(ReceiveUDPMessage), null);
                udpPort.Enabled = false;
            }
        }

        private void ReceiveUDPMessage(IAsyncResult iaResult)
        {
            byte[] UDPbuffer;
            IPEndPoint endPoint = new IPEndPoint(IPAddress.Loopback, 6788); // Specify host and port from which we expect messages

            try
            {
                UDPbuffer = UDPlistener.EndReceive(iaResult, ref endPoint);
            }
            catch (ObjectDisposedException)
            {
                return;
            }

            udpBytesReceived += UDPbuffer.Length;
            this.Invoke((MethodInvoker)delegate()
            {
                packets_received.Text = udpBytesReceived.ToString();
            });

            MessageBox.Show(Encoding.ASCII.GetString(UDPbuffer, 0, UDPbuffer.Length));
            UDPlistener.BeginReceive(new AsyncCallback(ReceiveUDPMessage), null);
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (UDPlistener != null)
            {
                UDPlistener.Close();
            }

            if (isRecording)
            {
                isRecording = false;
                update_timer.Enabled = false;
                start_recording_button.Text = "Start recording";
                serialThread.Join();
                serial_port.Close();
            }
        }
    }
}
