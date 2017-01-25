#include "BuildServer.h"

#include <ostream>
#include "SQL/MySQL/exception.h"
#include "SQL/MySQL/driver.h"
#include "SQL/MySQL/statement.h"


BuildServer::BuildServer()
{
	try
	{
		/* Create a connection */
		sql::Driver* driver = get_driver_instance(); //cannot be delete!
		m_connection = driver->connect("tcp://127.0.0.1:3306", "cmcarthur", "Acertuba");

		m_connection->setSchema("sakila");
	}
	catch(sql::SQLException &e)
	{
		std::cout << "# ERR: SQLException in " << __FILE__;
		std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
	}
}


BuildServer::~BuildServer()
{
	delete m_connection;
}

ErrorCode BuildServer::Execute(const char * statement, sql::ResultSet* out_result)
{
	try
	{

		sql::Statement* stmt = m_connection->createStatement();
		out_result = stmt->executeQuery(statement);
		return NOERROR;
	}
	catch(sql::SQLException &e)
	{
		std::cout << "# ERR: SQLException in " << __FILE__;
		std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;

		return E_SQL_FAILED_EXECUTE;
	}
}
