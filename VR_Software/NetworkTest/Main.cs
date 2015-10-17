using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Net;
using System.Net.Sockets;
using System.Diagnostics;

namespace NetworkTest
{
    public partial class Main : Form
    {
        TcpClient tcpClient;
        UdpClient udpClient;

        IPEndPoint udpEndpoint;
        IPAddress udpAddress;

        public Main()
        {
            InitializeComponent();
        }

        private void tcp_connect_Click(object sender, EventArgs e)
        {
            try
            {
                tcpClient = new TcpClient();
                tcpClient.Connect(tcp_ip.Text, Convert.ToInt32(tcp_port.Text));

                if (tcpClient.Connected)
                    tcp_info.Text = "Tcp client connected";
            }
            catch (Exception E)
            {
                MessageBox.Show(E.Message);
            }

        }

        private void udp_connect_Click(object sender, EventArgs e)
        {
            try
            {
                udpClient = new UdpClient();
                udpClient.Connect(udp_ip.Text, Convert.ToInt32(udp_port.Text));

                byte[] ipAddress = new byte[4]{169, 254, 152, 203};
                udpAddress = new IPAddress(ipAddress);
                udpEndpoint = new IPEndPoint(udpAddress, Convert.ToInt32(udp_port.Text));

                if (udpClient.Client.Connected)
                    udp_info.Text = "Connected to UDP";
            }
            catch (Exception E)
            {
                MessageBox.Show(E.Message);
            }
        }

        private void udp_send_byte_Click(object sender, EventArgs e)
        {
            byte[] testByte = new byte[2];
            testByte[0] = 0x05;
            testByte[1] = 0x0a;

            if (udpClient.Client.Connected)
            {
                udpClient.Send(testByte,testByte.Length);
                byte[] bytesReceived = udpClient.Receive(ref udpEndpoint);

                udp_info.Text = Convert.ToString(BitConverter.ToChar(bytesReceived, 0));
            }
        }

        private void tcp_send_byte_Click(object sender, EventArgs e)
        {
            byte[] testByte = new byte[2];
            testByte[0] = 0x05;
            testByte[1] = 0x0a;

            if (tcpClient.Connected)
            {
                tcpClient.Client.Send(testByte, testByte.Length, SocketFlags.None);
                tcpClient.Client.Receive(testByte, 2, SocketFlags.None);

                tcp_info.Text = Convert.ToString(BitConverter.ToChar(testByte, 0));
            }
        }

        private void tcp_run_test_Click(object sender, EventArgs e)
        {
            byte[] testByte = new byte[2];
            byte[] receiveBytes =new byte[2];
            testByte[0] = 0x05;
            testByte[1] = 0x0a;

            int nTests = 100;

            Stopwatch stopwatch = new Stopwatch();

            stopwatch.Start();

            long[] ticks = new long[3*nTests];

            if (tcpClient.Connected)
            {

                for (int i = 0; i < nTests; ++i)
                {
                    ticks[3 * i] = stopwatch.ElapsedMilliseconds;
                    tcpClient.Client.Send(testByte, testByte.Length, SocketFlags.None);
                    ticks[(3 * i) + 1] = stopwatch.ElapsedMilliseconds;
                    tcpClient.Client.Receive(receiveBytes, 2, SocketFlags.None);
                    ticks[(3 * i) + 2] = stopwatch.ElapsedMilliseconds;
                }

                tcp_info.Text = "Test completed\n";


                long sendCommand = 0;
                long receiveCommand = 0;
                long cycle = 0;
                for (int i = 0; i < (nTests-1);++i)
                {
                    sendCommand += (ticks[(3*i)+1] - ticks[(3*i)]);
                    receiveCommand += (ticks[(3*i)+2] - ticks[(3*i)+1]);
                    cycle += (ticks[3 * (i + 1)] - ticks[3 * i]);
                }

                double avSend = Convert.ToDouble(sendCommand) / (nTests - 1);
                double avReceive = Convert.ToDouble(receiveCommand) / (nTests - 1);
                double avCycle = Convert.ToDouble(cycle) / (nTests - 1);

                tcp_info.Text += "Average send duration: " + avSend.ToString() + "\n";
                tcp_info.Text += "Average receive duration: " + avReceive.ToString() + "\n";
                tcp_info.Text += "Average cycle duration: " + avCycle.ToString() + "\n";
            }
        }
    }
}
