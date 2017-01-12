#include "Event.h"



Event::Event()
{
	mu = new std::mutex();
	cv = new std::condition_variable();
}


Event::~Event()
{
	delete mu, cv;
}

void Event::SignalOne()
{
	cv->notify_one();
}

void Event::SignalAll()
{
	cv->notify_all();
}

void Event::Wait()
{
	std::unique_lock<std::mutex> lk(*mu);
	cv->wait(lk);
}

bool Event::WaitFor(const uint32_t duration)
{
	std::unique_lock<std::mutex> lk(*mu);
	std::cv_status result = cv->wait_for(lk, std::chrono::milliseconds(duration));
	
	switch(result)
	{
	case std::cv_status::no_timeout:
		return true;
	case std::cv_status::timeout:
		return false;
	default:
		return false;
	}
}

bool Event::IsSignaled()
{
	return WaitFor(0);
}
