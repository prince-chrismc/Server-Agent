#pragma once
#include <thread>
#include <cstdint>
#include "Event.h"

typedef void(*FUNCTIONPOINTER)(void*, Event*, bool*);
typedef void(*FUNCTIONPOINTER2)(void*, void*, Event*, bool*);
typedef void(*FUNCTIONPOINTER3)(void*, void*, void*, Event*, bool*);

class Thread
{
private:
	enum STATUSCODES { INVALID, CREATED, DETACHED, JOINED = 2, RUNNING = 2, COMPLETED};
	
	std::thread m_thread;
	std::thread::id m_id;
	STATUSCODES m_status;
	Event* m_start;
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
	static void WaitToStart(Event* toStart);
};
