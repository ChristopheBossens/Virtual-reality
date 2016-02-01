#pragma once
#include <iostream>
#include <Windows.h>

/* Class that interacts with the arduino motion sensor
   max cpi values and lift distance values are 40 and 31 respectively
*/
class MotionReader
{
private:
	int portNumber;
	HANDLE serialPortHandle;
	DWORD bytesSent, bytesReceived;
	byte* receiveBuffer;
	byte* sendBuffer;
	bool isInitialized, isReading;

	double xScaling;
	double yScaling;
	int scalingFactor;

	const byte INIT = 1;
	const byte GET_PARAM = 3;
	const byte SET_PARAM = 4;
	const byte START_READING = 5;
	const byte STOP_READING = 6;
	const byte POLL_SENSOR = 7;

	const byte INIT_OK = 1;

	const int RX_BUFFER_SIZE = 5;
	const int TX_BUFFER_SIZE = 5;

	void PrintReceiveBuffer();
	
public:
	MotionReader();
	~MotionReader();
	void Connect(int port);
	void Test();

	void GetParameters(byte& cpi1, byte& cpi2, byte& ld1, byte& ld2);
	void SetParameters(byte cpi1, byte cpi2, byte ld1, byte ld2, int scalingFactor);
	void StartReading();
	void StopReading();

	void PollSensor(double& x, double& y);
	int InputBytesPending();
};