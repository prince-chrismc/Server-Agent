///////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Error Code Definition - Used to Standardize Error Handling.
//
///////////////////////////////////////////////////////////////////////////////////////////////////


#pragma once
typedef long long ERROR;

#define NOERROR					((ERROR)0x00000000L)
#define SUCCED					((ERROR)0x00000001L)

#define E_SOCKET_UNKNOWN_FAIL			((ERROR)0x10000122L)
#define E_SOCKET_FAILED_INITIALIZE		((ERROR)0x10000123L)
#define E_SOCKET_FAILED_OPEN			((ERROR)0x10000124L)
#define E_SOCKET_NO_CONNECTION			((ERROR)0x10000126L)
#define E_SOCKET_FAILED_RECEIVE			((ERROR)0x10000127L)
#define E_SOCKET_FAILED_SEND			((ERROR)0x10000128L)
#define E_SOCKET_FAILED_LISTEN			((ERROR)0x10000129L)
#define E_SOCKET_FAILED_SHUTDOWN		((ERROR)0x1000012AL)
#define E_SOCKET_FAILED_SELECT			((ERROR)0x1000012BL)
#define E_SOCKET_FAILED_CLOSE			((ERROR)0x1000012CL)

#define E_SQL_FAILED_EXECUTE			((ERROR)0x10000257L)
		
class ErrorCode
{
private:
	ERROR m_Error;

public:
	ErrorCode();
	ErrorCode(const ERROR &h);

	ErrorCode operator=(const ErrorCode &err);
	ErrorCode operator=(const ERROR &hr);
	bool operator!=(const ERROR &hr);
	bool operator==(const ERROR &hr);

	bool DidSucced();
	bool DidFail();
	void Reset();
};
