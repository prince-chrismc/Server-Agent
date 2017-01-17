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

#ifdef _WIN32
#include <windows.h>
const DWORD MS_VC_EXCEPTION = 0x406D1388;

#pragma pack(push,8)
typedef struct tagTHREADNAME_INFO
{
	DWORD dwType; // Must be 0x1000.
	LPCSTR szName; // Pointer to name (in user addr space).
	DWORD dwThreadID; // Thread ID (-1=caller thread).
	DWORD dwFlags; // Reserved for future use, must be zero.
} THREADNAME_INFO;
#pragma pack(pop)


void Thread::SetThreadName(uint32_t dwThreadID, const char* threadName)
{

	// DWORD dwThreadID = ::GetThreadId( static_cast<HANDLE>( t.native_handle() ) );

	THREADNAME_INFO info;
	info.dwType = 0x1000;
	info.szName = threadName;
	info.dwThreadID = dwThreadID;
	info.dwFlags = 0;

	__try
	{
		RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR), (ULONG_PTR*)&info);
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
	}
}

void Thread::SetThreadName(const char* threadName)
{
	DWORD threadId = ::GetThreadId(static_cast<HANDLE>(m_thread.native_handle()));
	SetThreadName(threadId, threadName);
}

#else
void Thread::SetThreadName(const char* threadName)
{
	auto handle = m_thread.native_handle();
	pthread_setname_np(handle, threadName);
}
#endif

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
