#include "ErrorCode.h"

ErrorCode::ErrorCode()
{
	m_Error = NOERROR;
}

ErrorCode::ErrorCode(const ERROR &h) : m_Error(h)
{
}

ErrorCode ErrorCode::operator=(const ErrorCode & err)
{
	return ErrorCode(err.m_Error);
}

ErrorCode ErrorCode::operator=(const ERROR &hr)
{
	return ErrorCode(hr);
}

bool ErrorCode::operator!=(const ERROR & hr)
{
	return (m_Error != hr);
}

bool ErrorCode::operator==(const ERROR & hr)
{
	return (m_Error == hr);
}

bool ErrorCode::DidSucced()
{
	return m_Error >= 0;
}

bool ErrorCode::DidFail()
{
	return m_Error < 0;
}

void ErrorCode::Reset()
{
	m_Error = NOERROR;
}
