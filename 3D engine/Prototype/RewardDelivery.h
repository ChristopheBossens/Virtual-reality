#pragma once
#include "cbw.h"
#include <mutex>
#include <condition_variable>
#include <thread>
#include <chrono>

class RewardDelivery
{
private:
	std::mutex threadMutex;
	std::condition_variable rewardThreadSignal;
	std::thread rewardThread;
	std::chrono::duration<int, std::milli> rewardDuration;

	bool deviceConnected, portActive;
	bool isNotified, threadRunning;

	int boardNumber;
	int portNumber;
	int errCode;

	int ULStat;
	float revisionLevel;
	const int portOffset = 15;

	void RewardDeliveryFunction(std::chrono::duration<int, std::milli> n);
public:
	RewardDelivery();
	~RewardDelivery();

	void SetRewardDuration(int n);
	bool Initialize(int boardNumber, int portNumber);
	
	void StartReward();
	void StopReward();
	void PulseReward();
	void BlinkLed();
};