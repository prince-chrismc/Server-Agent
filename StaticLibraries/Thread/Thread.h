#pragma once
#include <thread>
#include <mutex>
#include <condition_variable>
#include <cstdint>

struct StartDelay;
typedef void(*FUNCTIONPOINTER)(void*, StartDelay*, bool*);
typedef void(*FUNCTIONPOINTER2)(void*, void*, StartDelay*, bool*);
typedef void(*FUNCTIONPOINTER3)(void*, void*, void*, StartDelay*, bool*);

struct StartDelay
{
	std::mutex* mu;
	std::condition_variable* cv;

	StartDelay()
	{
		mu = new std::mutex();
		cv = new std::condition_variable();
	}

	~StartDelay()
	{
		delete mu, cv;
	}
};

class Thread
{
private:
	enum STATUSCODES { INVALID, CREATED, DETACHED, JOINED = 2, RUNNING = 2, COMPLETED};
	
	std::thread m_thread;
	std::thread::id m_id;
	STATUSCODES m_status;
	StartDelay* m_start;
	bool* m_completed;

public:
	Thread(FUNCTIONPOINTER function, void* prama);
	Thread(FUNCTIONPOINTER2 function, void* prama, void* prama2);
	Thread(FUNCTIONPOINTER3 function, void* prama, void* prama2, void* prama3);
	virtual ~Thread();

	void Start();
	void WaitOn();
	bool HasNotLaunched();
	bool IsRunning();
	bool IsComplete();
	uint16_t GetID();

	//////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//  Used In Thread Functions
	//
	//////////////////////////////////////////////////////////////////////////////////////////////////
	static void SetAsCompleted(bool* out_done);
	//Used In Thread Functions
	static void WaitToStart(StartDelay* delayed);
};
