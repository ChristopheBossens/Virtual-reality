/* class DataLogger
// 
*/
#pragma once
#include <fstream>
#include <string>
#include <vector>

class DataLogger
{
private:
	double* motionData;

	std::string file;
	std::ofstream logFile;
	bool isLogging;
public:
	DataLogger();
	~DataLogger();

	void SetLogfile(std::string filename);
	void StartLogging();
	void StopLogging();
	void LogData(double data);
	void LogData(float data);
	void LogData(bool data);
	void LogData(unsigned int data);
};