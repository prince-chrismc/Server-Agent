#include "ErrorCode.h"

ErrorCode::ErrorCode()
{
	m_Error = NO_ERROR;
}

ErrorCode::ErrorCode(const ERROR_CODES &err) : m_Error(err)
{
}

ErrorCode ErrorCode::operator=(const ErrorCode & err)
{
	return ErrorCode(err.m_Error);
}

ErrorCode ErrorCode::operator=(const ERROR_CODES &err)
{
	return ErrorCode(err);
}

bool ErrorCode::operator!=(const ERROR_CODES & err)
{
	return (m_Error != err);
}

bool ErrorCode::operator==(const ERROR_CODES & err)
{
	return (m_Error == err);
}

bool ErrorCode::DidSucced()
{
	return m_Error < E_ERROR_MIN;
}

bool ErrorCode::DidFail()
{
	return m_Error >= E_ERROR_MIN;
}

void ErrorCode::Reset()
{
	m_Error = NO_ERROR;
}
