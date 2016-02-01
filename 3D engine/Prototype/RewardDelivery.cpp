#include <iostream>
#include "RewardDelivery.h"

RewardDelivery::RewardDelivery()
{
	this->rewardDuration = std::chrono::duration<int, std::milli>(50);
	this->isNotified = false;
	this->deviceConnected = false;
	this->threadRunning = false;
	this->portActive = false;

	revisionLevel = (float)CURRENTREVNUM;
	ULStat = cbDeclareRevision(&revisionLevel);
	cbErrHandling(DONTPRINT, DONTSTOP);
	
}

RewardDelivery::~RewardDelivery()
{
	if (this->threadRunning)
		this->StopReward();
}
bool RewardDelivery::Initialize(int boardNumber, int portNumber)
{
	bool initResult = false;
	if (!this->deviceConnected && !this->threadRunning)
	{
		this->boardNumber = boardNumber;
		this->portNumber = portNumber;
		char boardName[BOARDNAMELEN];
		errCode = cbGetBoardName(boardNumber, boardName);

		if (portNumber < 1 || portNumber > 8)
		{
			std::cout << "Error. Port is not within range [1, 8]" << std::endl;
			return false;
		}
		if (errCode != 0)
		{
			std::cout << "Device error: " << errCode << std::endl;
			return false;
		}

		deviceConnected = true;
		std::cout << "*******************************************************" << std::endl;
		std::cout << "REWARD DELIVERY STATUS " << std::endl;
		std::cout << "Board number: " << boardNumber << std::endl;
		std::cout << "Board name  : " << boardName << std::endl;
		std::cout << "Pulse time  : " << rewardDuration.count() << std::endl;
		std::cout << "Revision    : " << revisionLevel << std::endl;
		std::cout << "*******************************************************" << std::endl;
		initResult = true;
	}
	return initResult;
}

void RewardDelivery::SetRewardDuration(int n)
{
	this->rewardDuration = std::chrono::duration<int, std::milli>(n);
}

void RewardDelivery::StartReward()
{
	if (!this->threadRunning && this->deviceConnected)
	{
		isNotified = false;
		threadRunning = true;
		rewardThread = std::thread(&RewardDelivery::RewardDeliveryFunction, this, this->rewardDuration);
	}
}

void RewardDelivery::StopReward()
{
	if (this->threadRunning)
	{
		isNotified = true;
		threadRunning = false;

		rewardThreadSignal.notify_one();
		rewardThread.join();
	}
}

void RewardDelivery::PulseReward()
{
	if (this->threadRunning)
	{
		std::unique_lock<std::mutex> inputLock(threadMutex);
		isNotified = true;
		rewardThreadSignal.notify_one();
	}
}

void RewardDelivery::RewardDeliveryFunction(std::chrono::duration<int, std::milli> rewardTime)
{
	while (threadRunning)
	{
		std::unique_lock<std::mutex> threadLock(threadMutex);
		while (!isNotified)
			rewardThreadSignal.wait(threadLock);

		if (!threadRunning)
			break;

		cbDBitOut(this->boardNumber, FIRSTPORTA, this->portOffset + this->portNumber, 1);
		std::this_thread::sleep_for(rewardTime);
		cbDBitOut(this->boardNumber, FIRSTPORTA, this->portOffset + this->portNumber, 0);

		isNotified = false;
	}
}
void RewardDelivery::BlinkLed()
{
	if (deviceConnected)
		errCode = cbFlashLED(boardNumber);
}