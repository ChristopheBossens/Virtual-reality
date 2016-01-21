#include <iostream>
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
	
	bool isNotified, threadRunning;
	void RewardDeliveryFunction(std::chrono::duration<int, std::milli> n);
public:
	RewardDelivery();
	~RewardDelivery();

	void SetRewardDuration(int n);

	void StartReward();
	void PulseReward();
	void StopReward();
};