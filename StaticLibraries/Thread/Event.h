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

	void Reset();

	void Wait();
	bool WaitFor(const uint32_t duration);

	bool IsSignaled();
	bool IsNotSignaled();
private:
	std::mutex* m_mu;
	std::condition_variable* m_cv;
	bool* m_tripped;
};

