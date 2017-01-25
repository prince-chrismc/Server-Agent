#pragma once

#include "ErrorCodes/ErrorCode.h"
#include "SQL/MySQL/connection.h"
#include "SQL/MySQL/resultset.h"

class BuildServer
{
public:
	BuildServer();
	~BuildServer();

	sql::Connection* GetConnection() { return m_connection; }

	ErrorCode Execute(const char* statement, sql::ResultSet* out_result);

private:
	sql::Connection* m_connection;
};