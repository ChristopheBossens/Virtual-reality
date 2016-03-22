#pragma once
#include <Windows.h>
#include <string>
#include <ctime>
#include <fstream>
struct AppParameters
{
	int screenWidth;
	int screenHeight;
	int motionPort;
	int rewardPort;
	int rewardSwitch;
	int daqPort;

	int rewardDuration;
};

struct RewardParameters
{
	double runningThresholdVelocity;
	unsigned int runningThresholdDuration;
	unsigned int rewardStateDuration;
	unsigned int stationaryThresholdDuration;
	unsigned int stationaryStateDuration;
};
// Header library for facilitaing IO operations on the system
class AppIO
{
public:
	static AppParameters appParams;
	static RewardParameters rewardParams;

	static std::string GetAppRootFolder();
	static std::string GetDateString();
	static std::string GetImageFolder();
	static std::string CreateDataFolder();

	static bool FileExists(std::string);
	static void ReadConfiguration();
};
