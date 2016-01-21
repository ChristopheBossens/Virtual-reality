#include "RewardDelivery.h"
#include <iostream>
using namespace std;

RewardDelivery::RewardDelivery()
{
	this->rewardDuration = std::chrono::duration<int, std::milli>(50);
	this->isNotified = false;
	this->threadRunning = false;
}

RewardDelivery::~RewardDelivery()
{
	this->StopReward();
}
void RewardDelivery::SetRewardDuration(int n)
{
	if( !this->threadRunning)
		this->rewardDuration = std::chrono::duration<int, std::milli>(n);
}
void RewardDelivery::StartReward()
{
	if (!this->threadRunning)
	{
		threadRunning = true;
		rewardThread = std::thread(&RewardDelivery::RewardDeliveryFunction,this,this->rewardDuration);

		cout << "Reward thread started" << endl;
	}
	else
		cout << "Thread already running" << endl;
}

void RewardDelivery::StopReward()
{
	if(this->threadRunning)
	{
		isNotified = true;
		threadRunning = false;

		rewardThreadSignal.notify_one();
		rewardThread.join();
		
		cout << "Reward thread stopped" << endl;
	}
	else
		cout << "Thread not running" << endl;
}

void RewardDelivery::PulseReward()
{
	if(this->threadRunning)
	{
		std::unique_lock<std::mutex> inputLock(threadMutex);
		isNotified = true;
		rewardThreadSignal.notify_one();

		cout << "Reward thread pulsed" << endl;
	}
	else
		cout << "Thread not running" << endl;
}

void RewardDelivery::RewardDeliveryFunction(std::chrono::duration<int, std::milli> n)
{
	cout << "In reward delivery thread" << endl;
	while(threadRunning)
	{
		std::unique_lock<std::mutex> threadLock(threadMutex);
		while(!isNotified)
			rewardThreadSignal.wait(threadLock);

		if(!threadRunning)
			break;

		cout << "Open relay" << endl;
		std::this_thread::sleep_for(n);
		cout << "Close relay" << endl;

		isNotified = false;
	}
	cout << "Exiting reward delivery thread" <<endl;
}