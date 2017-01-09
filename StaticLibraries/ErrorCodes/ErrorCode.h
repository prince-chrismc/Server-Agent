#pragma once
#include <winerror.h>

#define NOERROR				((HRESULT)0x00000000L)
#define SUCCED				((HRESULT)0x00000001L)
#define E_UNKNOWN_FAIL		((HRESULT)0x10000001L)
#define E_FAILED_INITIALIZE		((HRESULT)0x10000123L)
#define E_FAILED_OPEN		((HRESULT)0x100000124)
#define E_FAILED_CONNECT		((HRESULT)0x10000125L)
#define E_NO_CONNECTION		((HRESULT)0x10000126L)
#define E_FAILED_RECIEVE		((HRESULT)0x10000127L)
#define E_FAILED_SEND		((HRESULT)0x10000128L)
#define E_FAILED_LISTEN		((HRESULT)0x10000129L)
#define E_FAILED_SHUTDOWN		((HRESULT)0x10000130L)
#define E_FAILED_SELECT		((HRESULT)0x10000131L)
#define E_FAILED_CLOSE		((HRESULT)0x10000132L)

class ErrorCode
{
	private:
		HRESULT m_Error;

	public:
		ErrorCode();
		ErrorCode(HRESULT h);
		ErrorCode operator=(const ErrorCode &err);
		bool DidSucced();
		bool DidFail();
		void Reset();
};
