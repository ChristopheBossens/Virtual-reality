using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Net;
using System.Net.Sockets;
using System.Drawing;

namespace vrClient
{
    enum ClientState
    {
        Disconnected,
        Idle,
        AwaitLoadConfirmation,
        ReadyForExperiment,
        ExperimentRunning,

        StartMapping,
        StopMapping,
        ReadyForCoordinate,
        ProcessingCoordinate
    };

    class ClientConnection
    {
        const byte getConfigByte = 0x01;
        const byte setConfigByte = 0x02;
        const byte sendImageByte = 0x03;
        const byte byteLF = 0x0a;
        const byte loadExperimentByte = 0x05;
        const byte runExperimentByte = 0x06;
        const byte experimentUpdateByte = 0x07;
        const byte disconnectByte = 0x08;
        const byte runRFMapping = 0x09;

        TcpClient client;
        BinaryWriter binaryWriter;
        BinaryReader binaryReader;
        ClientState clientState;

        byte[] inputBuffer, outputBuffer;
        int bufferSize;
        object stateObject;
        bool clientConnected;

        ListboxMessenger messenger;

        int lastX, lastY, lastOr, lastFreq;

        public ClientConnection(string ip, int port)
        {
            Connect(ip, port);
        }
        ~ClientConnection()
        {
            if (client != null & clientConnected)
                client.Close();
        }

        // Connection and configuration functions
        public void Connect(string ip, int port)
        {
            clientConnected = true;
            try
            {
                
                byte[] localAddressBytes = new byte[] { 169, 254, 152, 204 };
                byte[] remoteAddressBytes = new byte[] { 169, 254, 152, 203 };

                //IPAddress localAddress = Dns.GetHostAddresses("127.0.0.1");
                //IPAddress remoteAddress = IPAddress.Parse(ip);
               
                IPAddress localAddress = new IPAddress(localAddressBytes);
                IPAddress remoteAddress = new IPAddress(remoteAddressBytes);

                IPEndPoint localEndPoint = new IPEndPoint(localAddress, 6788);
                IPEndPoint remoteEndPoint = new IPEndPoint(remoteAddress, port);

                client = new TcpClient(localEndPoint);
                client.Client.Connect(remoteEndPoint);
                client.SendBufferSize = 1024;

                binaryWriter = new BinaryWriter(client.GetStream());
                binaryReader = new BinaryReader(client.GetStream());

                bufferSize = 1024 * 4;
                inputBuffer = new byte[bufferSize];
                outputBuffer = new byte[1024];
            }
            catch (Exception exception)
            {
                MessageBox.Show(exception.Message);
                clientConnected = false;

            }
        }
        public void Disconnect()
        {
            if (client.Connected && clientState == ClientState.Idle)
            {
                binaryWriter.Write(disconnectByte);
                binaryWriter.Write(byteLF);
                binaryWriter.Flush();

                client.Close();
                clientState = ClientState.Disconnected;
            }
        }
        public void SetMessenger(ListboxMessenger lb) {   messenger = lb;}
        public bool IsConnected() { return clientConnected; }
        
        
        // Send signal to matlab client for loading a selected experiment configuration file   
        public bool LoadExperimentConfig(string filename)
        {
            binaryWriter.Write(loadExperimentByte);
            binaryWriter.Write(byteLF);
            clientState = ClientState.AwaitLoadConfirmation;

            binaryWriter.Write(Convert.ToInt16(filename.Length));
            binaryWriter.Write(filename.ToCharArray());
            binaryWriter.Flush();

            client.Client.Receive(inputBuffer, 2, SocketFlags.None);
            clientState = ClientState.Idle;
            if (inputBuffer[0] == loadExperimentByte && inputBuffer[1] == 0x02)
                return true;
            else
                return false;
        }
        public void RunExperiment(string filename)
        {
            binaryWriter.Write(runExperimentByte);
            binaryWriter.Write(byteLF);
            clientState = ClientState.ExperimentRunning;

            binaryWriter.Write(Convert.ToInt16(filename.Length));
            binaryWriter.Write(filename.ToCharArray());
            binaryWriter.Flush();

            // During an experiment the client is put into asynchronous mode for receiving updates
            client.Client.BeginReceive(inputBuffer, 0, 2, SocketFlags.None, DataReceived, stateObject);
        }


        // Functions for controlling receptive field mapping
        public void StartMapping()
        {
            binaryWriter.Write(runRFMapping);
            binaryWriter.Write(byteLF);

            clientState = ClientState.ProcessingCoordinate;
            client.Client.BeginReceive(inputBuffer, 0, 2, SocketFlags.None, CoordinateReceived, stateObject);
        }
        public void StopMapping()
        {
            clientState = ClientState.StopMapping;
        }
        public void UpdateRFData(int x, int y, int orientation, int frequency)
        {
            if (clientState != ClientState.StopMapping)
            {
                Byte[] xByte = BitConverter.GetBytes(x);
                Byte[] yByte = BitConverter.GetBytes(y);
                Byte[] orByte = BitConverter.GetBytes(orientation);
                Byte[] freqByte = BitConverter.GetBytes(frequency);

                Buffer.BlockCopy(xByte, 0, outputBuffer, 0, 2);
                Buffer.BlockCopy(yByte, 0, outputBuffer, 2, 2);
                Buffer.BlockCopy(orByte, 0, outputBuffer, 4, 2);
                Buffer.BlockCopy(freqByte, 0, outputBuffer, 6, 2);
             }
        }       
        private void SendRFData()
        {
            if (clientState == ClientState.ReadyForCoordinate)
            {
                clientState = ClientState.ProcessingCoordinate;
                client.Client.Send(outputBuffer, 8, SocketFlags.None);
                client.Client.BeginReceive(inputBuffer, 0, 1, SocketFlags.None, CoordinateReceived, stateObject);
            }

            if (clientState == ClientState.StopMapping)
            {
                lastX = -1;
                lastY = -1;
                binaryWriter.Write(Convert.ToInt16(lastX));
                binaryWriter.Write(Convert.ToInt16(lastY));
                binaryWriter.Write(Convert.ToInt16(lastOr));
                binaryWriter.Write(Convert.ToInt16(lastFreq));
                binaryWriter.Flush();

                client.Client.BeginReceive(inputBuffer, 0, 1, SocketFlags.None, CoordinateReceived, stateObject);
            }
        }
        public void SendStopSignal()
        {
            if (clientState == ClientState.StopMapping)
            {
                lastX = -1;
                lastY = -1;
                binaryWriter.Write(Convert.ToInt16(lastX));
                binaryWriter.Write(Convert.ToInt16(lastY));
                binaryWriter.Write(Convert.ToInt16(lastOr));
                binaryWriter.Write(Convert.ToInt16(lastFreq));
                binaryWriter.Flush();
            }
        }

        // Asynchronous callback functions
        public void CoordinateReceived(IAsyncResult ia)
        {
            int dataRead = client.Client.EndReceive(ia);
            if (dataRead == 0)
                return;

            if (clientState == ClientState.ProcessingCoordinate)
            {
                if (inputBuffer[0] == 1)
                {
                    clientState = ClientState.ReadyForCoordinate;
                    SendRFData();
                    return;
                }
            }

            if (clientState == ClientState.StopMapping)
            {
                if (inputBuffer[0] == 1)
                    SendStopSignal();
                clientState = ClientState.Idle;
            }
        }
        public void DataReceived(IAsyncResult ia)
        {
            int dataRead = client.Client.EndReceive(ia);

            if (dataRead == 0)
                return;


            switch (clientState)
            {
                case ClientState.AwaitLoadConfirmation:
                    if (inputBuffer[0] == loadExperimentByte)
                    {
                        if (inputBuffer[1] == 0x02)
                        {
                            clientState = ClientState.ReadyForExperiment;
                        }
                    }
                    break;

                case ClientState.ExperimentRunning:
                    if (inputBuffer[0] == experimentUpdateByte)
                    {
                        switch (inputBuffer[1])
                        {
                            case 0x01: // Experiment has ended
                                clientState = ClientState.Idle;
                                messenger.AddMessage("Experiment finished!");
                                return;
                            case 0x02: // Receive trial information
                                binaryReader.Read(inputBuffer, 2, 4);
                                int trialIndex = BitConverter.ToInt16(inputBuffer, 2);
                                int totalTrials = BitConverter.ToInt16(inputBuffer, 4);
                                messenger.AddMessage("Presenting trial: " + trialIndex.ToString() + " / " + totalTrials.ToString());
                                break;

                        }
                    }
                    break;
            }
            client.Client.BeginReceive(inputBuffer, 0, bufferSize, SocketFlags.None, DataReceived, stateObject);
        }

    }
}
