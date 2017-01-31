#pragma once
#ifdef  _WIN32
	#include <winerror.h>
#endif //_WIN32

#ifdef _LINUX
	typedef long HRESULT;
#endif //_LINUX

#define NOERROR					((HRESULT)0x00000000L)
#define SUCCED					((HRESULT)0x00000001L)

#define E_SOCKET_UNKNOWN_FAIL			((HRESULT)0x10000122L)
#define E_SOCKET_FAILED_INITIALIZE		((HRESULT)0x10000123L)
#define E_SOCKET_FAILED_OPEN			((HRESULT)0x10000124L)
#define E_SOCKET_NO_CONNECTION			((HRESULT)0x10000126L)
#define E_SOCKET_FAILED_RECEIVE			((HRESULT)0x10000127L)
#define E_SOCKET_FAILED_SEND			((HRESULT)0x10000128L)
#define E_SOCKET_FAILED_LISTEN			((HRESULT)0x10000129L)
#define E_SOCKET_FAILED_SHUTDOWN		((HRESULT)0x10000130L)
#define E_SOCKET_FAILED_SELECT			((HRESULT)0x10000131L)
#define E_SOCKET_FAILED_CLOSE			((HRESULT)0x10000132L)
#define E_SQL_FAILED_EXECUTE			((HRESULT)0x10000257L)
		
class ErrorCode
{
private:
	HRESULT m_Error;

public:
	ErrorCode();
	ErrorCode(const HRESULT &h);

	ErrorCode operator=(const ErrorCode &err);
	ErrorCode operator=(const HRESULT &hr);
	bool operator!=(const HRESULT &hr);
	bool operator==(const HRESULT &hr);

	bool DidSucced();
	bool DidFail();
	void Reset();
};
