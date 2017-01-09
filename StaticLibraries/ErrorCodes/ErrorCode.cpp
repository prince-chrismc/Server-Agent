#include "ErrorCode.h"

ErrorCode::ErrorCode()
{
	m_Error = NOERROR;
}

ErrorCode::ErrorCode(HRESULT h) : m_Error(h)
{
}

ErrorCode ErrorCode::operator=(const ErrorCode & err)
{
	return ErrorCode(err.m_Error);
}

void ErrorCode::operator=(const HRESULT &hr)
{
	m_Error = hr;
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
