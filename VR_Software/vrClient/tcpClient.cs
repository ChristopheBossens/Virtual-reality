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
        InitiateCommand,
        GetConfig,
        SendConfig,
        SendImage,
        ExperimentRunning
    };

    class ClientConnection
    {
        byte getConfigByte = 0x01;
        byte setConfigByte = 0x02;
        byte sendImageByte = 0x03;
        byte byteLF = 0x0a;

        TcpClient client;
        BinaryWriter binaryWriter;
        BinaryReader binaryReader;
        ClientState clientState;

        byte[] inputBuffer, outputBuffer;
        int bufferSize;
        object stateObject;
        bool clientConnected;

        public ClientConnection(string ip, int port)
        {
            try
            {
                client = new TcpClient();
                client.SendBufferSize = 81536;
                client.Connect(ip, port);
                binaryWriter = new BinaryWriter(client.GetStream());
                binaryReader = new BinaryReader(client.GetStream());

                bufferSize = 1024*4;
                inputBuffer = new byte[bufferSize];
                //client.Client.BeginReceive(inputBuffer,0, 1, SocketFlags.None,DataReceived,stateObject);

                clientConnected = true;

            }
            catch (Exception exception)
            {
                MessageBox.Show(exception.Message);
                clientConnected = false;
            }
        }

        public void RequestConfigFiles(out int nCount, out string[] configFiles)
        {
            // Send configuration request byte
            binaryWriter.Write(getConfigByte);
            binaryWriter.Write(byteLF);
            clientState = ClientState.GetConfig;
            binaryWriter.Flush();

            // Fetch incoming configuration files
            nCount = 0;
            configFiles = new string[0];
            client.Client.Receive(inputBuffer, 0, 1, SocketFlags.None);
            if (inputBuffer[0] == 1)
            {
                client.Client.Receive(inputBuffer, 2, SocketFlags.None);

                nCount = BitConverter.ToInt16(inputBuffer, 0);
                configFiles = new string[nCount];

                for (int configIndex = 0; configIndex < nCount; ++configIndex)
                {
                    client.Client.Receive(inputBuffer, 2, SocketFlags.None);
                    int nChars = BitConverter.ToInt16(inputBuffer, 0);
                    char[] charArray = new char[nChars];
                    charArray = binaryReader.ReadChars(nChars);
                    configFiles[configIndex] = new string(charArray);
                }
            }
        }

        public void UploadConfigFile(string filename)
        {
            // Send signal byte
            binaryWriter.Write(setConfigByte);
            binaryWriter.Write(byteLF);
            clientState = ClientState.SendConfig;
            binaryWriter.Flush();

            using (FileStream fileStream = new FileStream(filename, FileMode.Open, FileAccess.Read))
            {
                // Send filename length and filename
                string file = Path.GetFileNameWithoutExtension(filename);
                binaryWriter.Write(Convert.ToInt16(file.Length));
                binaryWriter.Write(file.ToCharArray());

                // Send file length and file
                outputBuffer = new byte[fileStream.Length];
                fileStream.Read(outputBuffer,0,(int)fileStream.Length);
                binaryWriter.Write(Convert.ToInt16(fileStream.Length));
                binaryWriter.Write(outputBuffer, 0, outputBuffer.Length);
                binaryWriter.Flush();
            }
            
        }

        public void SendConfigImage(string config, string[] imageLocations)
        {
            // Send signal byte
            binaryWriter.Write(sendImageByte);
            binaryWriter.Write(byteLF);
            clientState = ClientState.SendImage;
            binaryWriter.Flush();

            // Send config name length and config name
            binaryWriter.Write(Convert.ToInt16(config.Length));
            binaryWriter.Write(config.ToCharArray());

            // Send number of images
            binaryWriter.Write(Convert.ToInt16(imageLocations.Length));

            foreach (string imageLocation in imageLocations)
            {
                using(FileStream fileStream = new FileStream(imageLocation, FileMode.Open,FileAccess.Read))
                {
                    // send image name length and image name
                    string imageName = Path.GetFileName(imageLocation);
                    binaryWriter.Write(Convert.ToInt16(imageName.Length));
                    binaryWriter.Write(imageName.ToCharArray());

                    // send image length and image
                    outputBuffer = new byte[fileStream.Length];
                    fileStream.Read(outputBuffer, 0, (int)fileStream.Length);
                    binaryWriter.Write(Convert.ToInt32(fileStream.Length));
                    binaryWriter.Write(outputBuffer, 0, outputBuffer.Length);
                    binaryWriter.Flush();
                }
            }
        }

        public void SendImage(Image im, int index)
        {
            if (clientConnected)
            {
                Bitmap bmp = new Bitmap(im);
                MemoryStream ms = new MemoryStream();

                bmp.Save(ms, System.Drawing.Imaging.ImageFormat.Png);

                outputBuffer = ms.ToArray();

                binaryWriter.Write(outputBuffer, 0, outputBuffer.Length);
                binaryWriter.Flush();
            }
        }

        public void DataReceived(IAsyncResult ia)
        {
            int dataRead = client.Client.EndReceive(ia);

            if (dataRead == 0)
                return;

            string[] configFiles;
            // Get list of available configurations
            if (inputBuffer[0] == 0x0a)
            {
                client.Client.Receive(inputBuffer, 2, SocketFlags.None);

                int nCount = BitConverter.ToInt16(inputBuffer, 0);
                configFiles = new string[nCount];

                for (int configIndex = 0; configIndex < nCount;++configIndex)
                {
                    client.Client.Receive(inputBuffer, 2, SocketFlags.None);
                    int nChars = BitConverter.ToInt16(inputBuffer, 0);
                    char[] charArray = new char[nChars];
                    charArray = binaryReader.ReadChars(nChars);
                    configFiles[configIndex] = new string(charArray);
                }

            }
            
            byte[] byteData = ia.AsyncState as byte[];
            client.Client.BeginReceive(inputBuffer, 0, bufferSize, SocketFlags.None, DataReceived, stateObject);
        }

    }
}
