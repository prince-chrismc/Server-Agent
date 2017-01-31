#include "ErrorCode.h"

#ifdef _LINUX
	#define SUCCEEDED(hr) (((HRESULT)hr) >= 0)
	#define FAILED(hr) (((HRESULT)hr) < 0)
#endif //_LINUX

ErrorCode::ErrorCode()
{
	m_Error = NOERROR;
}

ErrorCode::ErrorCode(const HRESULT &h) : m_Error(h)
{
}

ErrorCode ErrorCode::operator=(const ErrorCode & err)
{
	return ErrorCode(err.m_Error);
}

ErrorCode ErrorCode::operator=(const HRESULT &hr)
{
	return ErrorCode(hr);
}

bool ErrorCode::operator!=(const HRESULT & hr)
{
	return (m_Error != hr);
}

bool ErrorCode::operator==(const HRESULT & hr)
{
	return (m_Error == hr);
}

bool ErrorCode::DidSucced()
{
	return SUCCEEDED(m_Error);
}

bool ErrorCode::DidFail()
{
	return FAILED(m_Error);
}

void ErrorCode::Reset()
{
	m_Error = NOERROR;
}
