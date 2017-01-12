#include "Thread.h"
#include <string>
#include <sstream>


Thread::Thread(FUNCTIONPOINTER function, void* prama)
{
	m_completed = new bool(false);
	m_start = new Event();
	m_thread = std::thread(function, prama, m_start, m_completed);
	m_id = m_thread.get_id();
	m_status = CREATED;
}
Thread::Thread(FUNCTIONPOINTER2 function, void * prama, void * prama2)
{
	m_completed = new bool(false);
	m_start = new Event();
	m_thread = std::thread(function, prama, prama2, m_start, m_completed);
	m_id = m_thread.get_id();
	m_status = CREATED;
}

Thread::Thread(FUNCTIONPOINTER3 function, void * prama, void * prama2, void * prama3)
{
	m_completed = new bool(false);
	m_start = new Event();
	m_thread = std::thread(function, prama, prama2, prama3, m_start, m_completed);
	m_id = m_thread.get_id();
	m_status = CREATED;
}

Thread::~Thread()
{
	delete m_completed;
	delete m_start;
}

void Thread::Start()
{
	m_status = DETACHED;
	m_start->SignalAll();
	m_thread.detach();
}

void Thread::WaitOn()
{
	m_status = JOINED;
	m_start->SignalAll();
	m_thread.join();
	m_status = COMPLETED;
}

bool Thread::HasNotLaunched()
{
	return m_thread.joinable();
}

bool Thread::IsRunning()
{
	return (m_status == RUNNING);
}

bool Thread::IsComplete()
{
	if (*m_completed)
		m_status = COMPLETED;

	return *m_completed;
}

uint16_t Thread::GetID()
{
	std::stringstream ss;
	ss << m_id;
	std::string s = ss.str();
	return atoi(s.c_str());
}

void Thread::SetAsCompleted(bool* out_done)
{
	*out_done = true;
}

void Thread::WaitToStart(Event* toStart)
{
	toStart->Wait();
}
