#include "AppIO.h"
// Initialize application parameters to default values
AppParameters AppIO::appParams;
RewardParameters AppIO::rewardParams;

// Reads the configuration file and fills in the parameter structure
void AppIO::ReadConfiguration()
{
	std::string nextLine;
	nextLine = AppIO::GetAppRootFolder() + "CONFIG.txt";
	std::ifstream configFile(AppIO::GetAppRootFolder() + "CONFIG.txt");
	
	if (configFile.is_open())
	{
		std::getline(configFile, nextLine);
		std::getline(configFile, nextLine);
		AppIO::appParams.screenWidth = std::stoi(nextLine);

		std::getline(configFile, nextLine);
		std::getline(configFile, nextLine);
		AppIO::appParams.screenHeight = std::stoi(nextLine);

		std::getline(configFile, nextLine);
		std::getline(configFile, nextLine);
		AppIO::appParams.motionPort = std::stoi(nextLine);

		std::getline(configFile, nextLine);
		std::getline(configFile, nextLine);
		AppIO::appParams.rewardPort = std::stoi(nextLine);

		std::getline(configFile, nextLine);
		std::getline(configFile, nextLine);
		AppIO::appParams.rewardSwitch = std::stoi(nextLine);

		std::getline(configFile, nextLine);
		std::getline(configFile, nextLine);
		AppIO::appParams.daqPort = std::stoi(nextLine);

		std::getline(configFile, nextLine);
		std::getline(configFile, nextLine);
		AppIO::appParams.rewardDuration = std::stoi(nextLine);
		configFile.close();
	}

	configFile.open(AppIO::GetAppRootFolder() + "REWARD.txt");
	if (configFile.is_open())
	{
		std::getline(configFile, nextLine);
		std::getline(configFile, nextLine);
		AppIO::rewardParams.runningThresholdVelocity = std::stod(nextLine);

		std::getline(configFile, nextLine);
		std::getline(configFile, nextLine);
		AppIO::rewardParams.runningThresholdDuration = std::stoul(nextLine);

		std::getline(configFile, nextLine);
		std::getline(configFile, nextLine);
		AppIO::rewardParams.rewardStateDuration = std::stoul(nextLine);

		std::getline(configFile, nextLine);
		std::getline(configFile, nextLine);
		AppIO::rewardParams.stationaryThresholdDuration = std::stoul(nextLine);

		std::getline(configFile, nextLine);
		std::getline(configFile, nextLine);
		AppIO::rewardParams.stationaryStateDuration = std::stoul(nextLine);
		configFile.close();
	}
}
// Returns the path in which the application executable is sitting
std::string AppIO::GetAppRootFolder()
{
	char pathname[MAX_PATH];
	DWORD pathLength;

	pathLength = GetModuleFileNameA(NULL, pathname,MAX_PATH);
	for (DWORD pathIdx = pathLength - 1; pathIdx >= 0; --pathIdx)
	{
		if (pathname[pathIdx] == '\\')
		{
			pathLength = (pathIdx+1);
			break;
		}
	}
	return std::string(pathname, pathLength);
}

// Returns a string representation (YYYY-MM-DD) of the current date
std::string AppIO::GetDateString()
{
	std::string dateString = "";
	time_t currentTime = time(0);

	struct tm * now = localtime(&currentTime);
	dateString += std::to_string(1900 + now->tm_year);

	if ((1 + now->tm_mon) < 10)
		dateString += "0";
	dateString += std::to_string(1 + now->tm_mon);

	if (now->tm_mday < 10)
		dateString += "0";
	dateString += std::to_string(now->tm_mday);

	return dateString;
}

// Returns the folder containing image data
std::string AppIO::GetImageFolder()
{
	std::string imageFolder = AppIO::GetAppRootFolder();
	imageFolder += "Images\\";
	return imageFolder;
}
// Tries to create a data folder followed by a subfolder labeled
// with the current date and returns this folder
std::string AppIO::CreateDataFolder()
{
	BOOL result;

	std::string dataFolder = AppIO::GetAppRootFolder() + "DATA\\";
	result = CreateDirectoryA(dataFolder.c_str(),NULL);
	
	dataFolder += AppIO::GetDateString();
	result = CreateDirectoryA(dataFolder.c_str(), NULL);

	return dataFolder + "\\";
}
// Returns true if the file specified as argument already exists
bool AppIO::FileExists(std::string file)
{
	DWORD fileAttribute = GetFileAttributesA(file.c_str());
	return (fileAttribute != INVALID_FILE_ATTRIBUTES);
}