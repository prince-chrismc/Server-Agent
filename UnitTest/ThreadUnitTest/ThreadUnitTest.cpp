// ThreadUnitTest.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <iostream>
#include <chrono>
#include <ctime>
#include "Thread/Thread.h"

static std::mutex mu;
void print(const char* output);
void TestJoin();
void TestStart();
long fibonacci(unsigned n);
void runFib(void*, Event*, bool*);

int main()
{
	print("Thread class unit test app.\n");

	print("\ntesting wait... \n\n");
	TestJoin();

	print("\ntesting start... \n\n");
	TestStart();

	print("\n\ntest completed!\nPress any key to exit...");
	std::cin.get();
}

void print(const char * output)
{
	mu.lock();
	printf(output);
	mu.unlock();
}

void TestJoin()
{
	uint16_t* id1 = new uint16_t(0);
	uint16_t* id2 = new uint16_t(0);
	Thread Join1(runFib, id1);
	Thread Join2(runFib, id2);
	*id1 = Join1.GetID();
	*id2 = Join2.GetID();

	print("Initial Check...");
  	if (!Join1.IsRunning() &&  !Join1.IsComplete() && !Join2.IsRunning() && !Join2.IsComplete())
	{
		print(" Pass!\nRunning...\n");
		Join1.WaitOn();
		Join2.WaitOn();
		std::string result = (!Join1.IsRunning() && Join1.IsComplete() && !Join2.IsRunning() && Join2.IsComplete()) ? "Success\n" : "FAILED\n";
		print("\nResult: ");
		print(result.c_str());
	}
	else
	{
		print("FAILED!\n");
	}
	delete id1;
	delete id2;
}

void TestStart()
{
	uint16_t* id1 = new uint16_t(0);
	uint16_t* id2 = new uint16_t(0);
	Thread Start1(runFib, id1);
	Thread Start2(runFib, id2);
	*id1 = Start1.GetID();
	*id2 = Start2.GetID();

	print("Initial Check...");
	if (!Start1.IsRunning() && !Start1.IsComplete() && !Start2.IsRunning() && !Start2.IsComplete())
	{
		print(" Pass!\nRunning...");
		Start1.Start();
		Start2.Start();

		while (!Start1.IsComplete() || !Start2.IsComplete())
		{
			print("Still Running...\n");
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}

		std::string result = (!Start1.IsRunning() && Start1.IsComplete() && !Start2.IsRunning() && Start2.IsComplete()) ? "Success\n" : "FAILED\n";
		print("\n\nResult: ");
		print(result.c_str());
	}
	else
	{
		print("FAILED!\n");
	}
	delete id1;
	delete id2;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Sample Thread Code
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void runFib(void* param, Event* delayed, bool* done)
{
	Thread::WaitToStart(delayed);
	uint16_t* id = (uint16_t*)param;
	std::chrono::time_point<std::chrono::system_clock> start, end;
	std::time_t start_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

	mu.lock();
	std::cout << "Thread " << *id << ": starting at " << std::ctime(&start_time) << "\n";
	mu.unlock();

	start = std::chrono::system_clock::now();
	uint64_t result = fibonacci(32);
	end = std::chrono::system_clock::now();

	mu.lock();
	std::cout << "Thread " << *id << ": f(32) = " << result << "\n";
	mu.unlock();

	std::chrono::duration<double> elapsed_seconds = end - start;
	std::time_t end_time = std::chrono::system_clock::to_time_t(end);

	mu.lock();
	std::cout << "Thread " << *id << ": finished computation at " << std::ctime(&end_time)
		<< "elapsed time: " << elapsed_seconds.count() << "s\n";
	mu.unlock();
	Thread::SetAsCompleted(done);
}

long fibonacci(unsigned n)
{
	if (n < 2) return n;
	return fibonacci(n - 1) + fibonacci(n - 2);
}
