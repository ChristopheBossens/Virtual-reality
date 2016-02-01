#include "MotionReader.h"
#include <string>
using namespace std;

MotionReader::MotionReader()
{
	this->isInitialized = false;
	this->isReading = false;
	receiveBuffer = new byte[RX_BUFFER_SIZE];
	sendBuffer = new byte[TX_BUFFER_SIZE];
}

MotionReader::~MotionReader()
{
	if (this->isInitialized)
	{
		if (this->isReading)
			this->StopReading();

		CloseHandle(serialPortHandle);
	}
}

void MotionReader::Test()
{
	string userInput = "";
	while (userInput != "exit")
	{
		cout << "motion reader command> ";
		cin >> userInput;
		if (userInput == "Connect")
		{
			this->Connect(8);
			continue;
		}

		if (userInput == "GetParameters")
		{
			byte cpi1, cpi2, ld1, ld2;
			this->GetParameters(cpi1, cpi2, ld1, ld2);
			continue;
		}

		if (userInput == "SetParameters")
		{
			this->SetParameters(40, 40, 31, 31, 200);
		}
		if (userInput == "StartReading")
		{
			this->StartReading();
			continue;
		}

		if (userInput == "PollSensor")
		{
			double x, y;
			this->PollSensor(x, y);
			cout << x << endl;
		}

		if (userInput == "InputBuffer")
		{
			cout << "Bytes available: " << this->InputBytesPending() << endl;
		}
	}
}
void MotionReader::Connect(int portNumber)
{
	// Connect to serial port
	if (this->isInitialized | this->isReading)
	{
		std::cout << "Already connected to motion sensor" << std::endl;
		return;
	}

	this->portNumber = portNumber;	
	serialPortHandle = CreateFileA("\\\\.\\COM8", GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (serialPortHandle == INVALID_HANDLE_VALUE)
	{
		std::cout << "Invalid handle to serial port. Is the number correct?" << std::endl;
		return;
	}

	// Retrieve and set device connection parameters
	DCB dcb;
	if (!GetCommState(serialPortHandle, &dcb))
	{
		std::cout << "Failed to retrieve serial port device parameters" << std::endl;
		CloseHandle(serialPortHandle);
		return;
	}

	dcb.BaudRate = CBR_9600;
	dcb.ByteSize = 8;
	dcb.Parity = NOPARITY;
	dcb.StopBits = ONESTOPBIT;

	if (!SetCommState(serialPortHandle, &dcb))
	{
		std::cout << "Failed to set serial port device parameters" << std::endl;
		CloseHandle(serialPortHandle);
		return;
	}

	// Retrieve and set device timeout parameters
	COMMTIMEOUTS serialTimeouts;
	if (!GetCommTimeouts(serialPortHandle, &serialTimeouts))
	{
		std::cout << "Failed to retrieve device timeout parameters" << std::endl;
		CloseHandle(serialPortHandle);
		return;
	}

	std::cout << "Timeout parameters: " << std::endl;
	std::cout << serialTimeouts.ReadIntervalTimeout << std::endl;
	std::cout << serialTimeouts.ReadTotalTimeoutConstant << std::endl;
	std::cout << serialTimeouts.ReadTotalTimeoutMultiplier << std::endl;
	serialTimeouts.ReadIntervalTimeout = 2000; // 500
	serialTimeouts.ReadTotalTimeoutConstant = 2000;
	serialTimeouts.ReadTotalTimeoutMultiplier = 0; // 500
	serialTimeouts.WriteTotalTimeoutConstant = 1;
	serialTimeouts.WriteTotalTimeoutMultiplier = 1;
	if (!SetCommTimeouts(serialPortHandle, &serialTimeouts))
	{
		std::cout << "Failed to set device timeout parameters" << std::endl;
		CloseHandle(serialPortHandle);
		return;
	}

	// Flush any pending input
	int bytesPending = InputBytesPending();
	for (int i = 0; i < bytesPending;++i)
		ReadFile(serialPortHandle, receiveBuffer, 1,&bytesReceived, NULL);

	// Initialize the device
	WriteFile(serialPortHandle, &INIT, 1, &bytesSent, NULL);
	ReadFile(serialPortHandle, receiveBuffer, 1, &bytesReceived, NULL);
	if (*receiveBuffer == INIT_OK)
	{
		std::cout << "Succesfully Connected to motion reader." << std::endl;
		this->isInitialized = true;
	}
	else
	{
		std::cout << "Failed to initialize motion reader." << std::endl;
		CloseHandle(serialPortHandle);
	}

}

int MotionReader::InputBytesPending()
{
	COMSTAT comStat;
	ClearCommError(serialPortHandle, NULL, &comStat);
	return comStat.cbInQue;
}
void MotionReader::GetParameters(byte& cpi1, byte& cpi2, byte& ld1, byte& ld2)
{
	if (this->isInitialized & !this->isReading)
	{
		WriteFile(serialPortHandle, &GET_PARAM, 1, &bytesSent, NULL);
		ReadFile(serialPortHandle, receiveBuffer, 4, &bytesReceived, NULL);

		std::cout << "Wrote " << bytesSent << " byte(s) and received " << bytesReceived << " byte(s)" << std::endl;
		std::cout << "Bytes available: " << InputBytesPending() << std::endl;
		PrintReceiveBuffer();
		cpi1 = receiveBuffer[0];
		cpi2 = receiveBuffer[1];
		ld1 = receiveBuffer[2];
		ld2 = receiveBuffer[3];
	}
}

void MotionReader::SetParameters(byte cpi1, byte cpi2, byte ld1, byte ld2 , int scalingFactor = 200)
{
	if (this->isInitialized & !this->isReading)
	{
		*(sendBuffer + 0) = SET_PARAM;
		*(sendBuffer + 1) = cpi1;
		*(sendBuffer + 2) = cpi2;
		*(sendBuffer + 3) = ld1;
		*(sendBuffer + 4) = ld2;

		WriteFile(serialPortHandle, sendBuffer, 5, &bytesSent, NULL);
		cout << "Wrote " << bytesSent << "byte(s)" << endl;
		this->xScaling = 2.54 / ((int)cpi1 * 200.0);
		this->yScaling = 2.54 / ((int)cpi2 * 200.0);
	}
}
void MotionReader::StartReading()
{
	if (this->isInitialized & !this->isReading)
	{
		WriteFile(serialPortHandle, &START_READING, 1, &bytesSent, NULL);
		this->isReading = true;
	}
}
void MotionReader::StopReading()
{
	if (this->isInitialized & this->isReading)
	{
		WriteFile(serialPortHandle, &STOP_READING, 1, &bytesSent, NULL);
		this->isReading = false;
	}
}

void MotionReader::PollSensor(double& x, double& y)
{
	if (this->isInitialized & this->isReading)
	{
		WriteFile(serialPortHandle, &POLL_SENSOR, 1, &bytesSent, NULL);
		ReadFile(serialPortHandle, receiveBuffer, 1, &bytesReceived, NULL);
		ReadFile(serialPortHandle, receiveBuffer+1, 1, &bytesReceived, NULL);
		ReadFile(serialPortHandle, receiveBuffer+2, 1, &bytesReceived, NULL);
		ReadFile(serialPortHandle, receiveBuffer+3, 1, &bytesReceived, NULL);
		//PrintReceiveBuffer();
		// x = (short)((*receiveBuffer) + (*(receiveBuffer + 1)<<8))*xScaling;
		x = (short)( (*(receiveBuffer + 1)<<8) | (*receiveBuffer))*xScaling;
	}
}

void MotionReader::PrintReceiveBuffer()
{
	std::cout << "Receive buffer status: " << std::endl;
	for (int i = 0; i < RX_BUFFER_SIZE;++i)
		std::cout << "[" << i << "] = " << (int)receiveBuffer[i] << std::endl;
	std::cout << std::endl;
}