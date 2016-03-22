#include "DataLogger.h"

DataLogger::DataLogger()
{
	isLogging = false;
}

DataLogger::~DataLogger()
{
	StopLogging();
}


void DataLogger::SetLogfile(std::string filename)
{
	this->file = filename;
}
// Start logging data to the specified file
// if the file exist, logging does not start
void DataLogger::StartLogging()
{
	if (!isLogging)
	{
		// Check if file exists
		if (!std::ifstream(file.c_str()))
		{
			logFile.open(file.c_str(), std::ios::binary | std::ios::out);
			if (logFile.is_open())
				isLogging = true;
		}
	}
}

void DataLogger::StopLogging()
{
	if (isLogging)
	{
		logFile.close();
		isLogging = false;
	}
}

void DataLogger::LogData(double data)
{
	if (isLogging)
		logFile.write((char*)&data, sizeof(double));
}

void DataLogger::LogData(float data)
{
	if (isLogging)
		logFile.write((char*)&data, sizeof(float));
}

void DataLogger::LogData(bool data)
{
	if (isLogging)
		logFile.write((char*)&data, sizeof(bool));
}

void DataLogger::LogData(unsigned int data)
{
	if (isLogging)
		logFile.write((char*)&data, sizeof(unsigned int));
}