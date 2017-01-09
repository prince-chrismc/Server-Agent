// SocketUnitTest.cpp : Defines the entry point for the console application.
//

#include "Thread\Thread.h"
#include "Socket\ActiveSocket.h"
#include "Socket\PassiveSocket.h"

#define MAX_PACKET 4096
static std::mutex mu;
void print(const char* output);
void LaunchServer(void* param, StartDelay* delayed, bool* done);
void LaunchClient();

int main()
{
	Thread server(LaunchServer, NULL);
	server.Start();

	LaunchClient();
	
	return 1;
}

void LaunchServer(void* param, StartDelay* delayed, bool* done)
{
	Thread::WaitToStart(delayed);
	CPassiveSocket socket;
	CActiveSocket *pClient = NULL;

	//--------------------------------------------------------------------------
	// Initialize our socket object 
	//--------------------------------------------------------------------------
	socket.Initialize();

	socket.Listen("127.0.0.1", 6789);

	while(true)
	{
		if((pClient = socket.Accept()) != NULL)
		{
			//----------------------------------------------------------------------
			// Receive request from the client.
			//----------------------------------------------------------------------
			if(pClient->Receive(MAX_PACKET))
			{
				//------------------------------------------------------------------
				// Send response to client and close connection to the client.
				//------------------------------------------------------------------
				pClient->Send(pClient->GetData(), pClient->GetBytesReceived());
				pClient->Close();
			}
		}
		delete pClient;
		Sleep(200);
	}

	//-----------------------------------------------------------------------------
	// Receive request from the client.
	//-----------------------------------------------------------------------------
	socket.Close();
}

void LaunchClient()
{
	CActiveSocket socket;       // Instantiate active socket object (defaults to TCP).
	char          time[50];

	memset(&time, 0, 50);

	//--------------------------------------------------------------------------
	// Initialize our socket object 
	//--------------------------------------------------------------------------
	socket.Initialize();

	//--------------------------------------------------------------------------
	// Create a connection to the time server so that data can be sent
	// and received.
	//--------------------------------------------------------------------------
	if(socket.Open("127.0.0.1", 6789))
	{
		//----------------------------------------------------------------------
		// Send a requtest the server requesting the current time.
		//----------------------------------------------------------------------
		if(socket.Send((const uint8 *)"hello world", 12))
		{
			//----------------------------------------------------------------------
			// Receive response from the server.
			//----------------------------------------------------------------------
			socket.Receive(49);
			memcpy(&time, socket.GetData(), 49);
			print(time);

			//----------------------------------------------------------------------
			// Close the connection.
			//----------------------------------------------------------------------
			socket.Close();
		}
	}
}

void print(const char * output)
{
	mu.lock();
	printf(output);
	mu.unlock();
}