#include "Event.h"

Event::Event()
{
	m_mu = new std::mutex();
	m_cv = new std::condition_variable();
	m_tripped = new bool(false);
}


Event::~Event()
{
	delete m_mu;
	delete m_cv;
	delete m_tripped;
}

void Event::SignalOne()
{
	*m_tripped = true;
	m_cv->notify_one();
}

void Event::SignalAll()
{
	*m_tripped = true;
	m_cv->notify_all();
}

void Event::Reset()
{
	*m_tripped = false;
}

void Event::Wait()
{
	std::unique_lock<std::mutex> lk(*m_mu);
	m_cv->wait(lk);
}

bool Event::WaitFor(const uint32_t duration)
{
	std::unique_lock<std::mutex> lk(*m_mu);
	std::cv_status result = m_cv->wait_for(lk, std::chrono::milliseconds(duration));
	
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
	return *m_tripped;
}

bool Event::IsNotSignaled()
{
	return !IsSignaled();
}