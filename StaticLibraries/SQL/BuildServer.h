#pragma once

#include "ErrorCodes/ErrorCode.h"
#include "MySQL/connection.h"
#include "MySQL/prepared_statement.h"
#include "MySQL/resultset.h"

class BuildServer
{
public:
	BuildServer();
	~BuildServer();

	sql::Connection* GetConnection() { return m_connection; }

	sql::ResultSet* Execute(const char* statement);

private:
	sql::Connection* m_connection;

	//prewritten SQL codes
	sql::PreparedStatement* m_insertArtifact;
	sql::PreparedStatement* m_addAgent;
	sql::PreparedStatement* m_updateAgent;
};