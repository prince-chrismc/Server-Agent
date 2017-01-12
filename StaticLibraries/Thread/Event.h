#pragma once

#include <mutex>
#include <condition_variable>

class Event
{
public:
	Event();
	~Event();

	void SignalOne();
	void SignalAll();

	void Wait();
	bool WaitFor(const uint32_t duration);

	bool IsSignaled();

private:
	std::mutex* mu;
	std::condition_variable* cv;
};

