// SocketUnitTest.cpp : Defines the entry point for the console application.
//

#include "Thread\Thread.h"
#include "Socket\ActiveSocket.h"
#include "Socket\PassiveSocket.h"

#define MAX_PACKET 4096
static std::mutex mu;
void print(const char* output);
void LaunchServer(void* param, Event* delayed, bool* done);
void LaunchClient();

int main()
{
	print("\n -- Socket IO test app -- \n\n");
	Event* pKillEvent = new Event();
	Thread server(LaunchServer, pKillEvent);
	server.SetThreadName("server thread");
	server.Start();

	std::this_thread::sleep_for(std::chrono::milliseconds(50));

	print("Client: Launching\n");
	LaunchClient();
	
	pKillEvent->SignalAll();
	
	while(!server.IsComplete())
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
	
	delete pKillEvent;

	return 1;
}

void LaunchServer(void* param, Event* delayed, bool* done)
{
	Thread::WaitToStart(delayed);
	print("Server: Launching\n");
	CPassiveSocket socket;
	Event* pKillEvent = (Event*)param;
	CActiveSocket *pClient = NULL;
	ErrorCode err;

	//--------------------------------------------------------------------------
	// Initialize our socket object 
	//--------------------------------------------------------------------------
	err = socket.Initialize();

	if(err.DidSucced())
	{
		print("Server: Initialized\n");
		err = socket.Listen("127.0.0.1", 6789);
	}

	if(err.DidSucced())
		print("Server: Listening to port\n");

		if((pClient = socket.Accept()) != NULL)
		{
			print("Server: New Client accepted\n");
			//----------------------------------------------------------------------
			// Receive request from the client.
			//----------------------------------------------------------------------
			if(pClient->Receive(MAX_PACKET))
			{
				uint8_t* recieved = pClient->GetData();
				const char* rcv = (const char*)recieved;
				mu.lock();
				printf("Server Recieved: %s\n", rcv);
				mu.unlock();

				//------------------------------------------------------------------
				// Send response to client and close connection to the client.
				//------------------------------------------------------------------
				if(pClient->Send(recieved, pClient->GetBytesReceived()))
					print("Server: Echoed to client\n");
			}
		}
		delete pClient;

	//-----------------------------------------------------------------------------
	// Receive request from the client.
	//-----------------------------------------------------------------------------
	err = socket.Close();

	if(err.DidSucced())
		print("Server: closed\n");

	Thread::SetAsCompleted(done);
}

void LaunchClient()
{
	CActiveSocket socket;       // Instantiate active socket object (defaults to TCP).
	char          time[50];
	ErrorCode err;

	memset(&time, 0, 50);

	//--------------------------------------------------------------------------
	// Initialize our socket object 
	//--------------------------------------------------------------------------
	err = socket.Initialize();

	if(err.DidSucced())
		print("Client: initialized\n");

	//--------------------------------------------------------------------------
	// Create a connection to the time server so that data can be sent
	// and received.
	//--------------------------------------------------------------------------
	err = socket.Open("127.0.0.1", 6789);
	if(err.DidSucced())
	{
		print("Client: opened connection to server\n");
		//----------------------------------------------------------------------
		// Send a requtest the server requesting the current time.
		//----------------------------------------------------------------------
		if(socket.Send((const uint8 *)"hello world", 12))
		{
			print("Client: sent 'Hello world'\n");
			//----------------------------------------------------------------------
			// Receive response from the server.
			//----------------------------------------------------------------------
			socket.Receive(49);
			memcpy(&time, socket.GetData(), 49);
			mu.lock();
			printf("Client Recieved: %s\n", time);
			mu.unlock();

			//----------------------------------------------------------------------
			// Close the connection.
			//----------------------------------------------------------------------
			err = socket.Close();

			if(err.DidSucced())
				print("Client: closed\n");
		}
	}
}

void print(const char * output)
{
	mu.lock();
	printf(output);
	mu.unlock();
}