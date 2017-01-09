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
	ErrorCode err;

	//--------------------------------------------------------------------------
	// Initialize our socket object 
	//--------------------------------------------------------------------------
	err = socket.Initialize();

	err = socket.Listen("127.0.0.1", 6789);

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
				err = pClient->Close();
			}
		}
		delete pClient;
		Sleep(200);
	}

	//-----------------------------------------------------------------------------
	// Receive request from the client.
	//-----------------------------------------------------------------------------
	err = socket.Close();
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

	//--------------------------------------------------------------------------
	// Create a connection to the time server so that data can be sent
	// and received.
	//--------------------------------------------------------------------------
	err = socket.Open("127.0.0.1", 6789);
	if(err.DidSucced())
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
			err = socket.Close();
		}
	}
}

void print(const char * output)
{
	mu.lock();
	printf(output);
	mu.unlock();
}