#include "BuildServer.h"

#include <ostream>
#include "MySQL/exception.h"
#include "MySQL/driver.h"
#include "MySQL/statement.h"


BuildServer::BuildServer()
{
	try
	{
		/* Create a connection */
		sql::Driver* driver = get_driver_instance(); //cannot be delete!
		m_connection = driver->connect("tcp://127.0.0.1:3306", "cmcarthur", "Acertuba");
		
		m_connection->setSchema("build_server");

		m_insertArtifact = NULL; // m_connection->prepareStatement("INSERT INTO artifact_repositorty ");
		m_addAgent = m_connection->prepareStatement("INSERT INTO agents(id,name) VALUES( ?, ? )");
		m_updateAgent = m_connection->prepareStatement("UPDATE agents SET last_online = CURRENT_TIMESTAMP WHERE id = ?");
	}
	catch(sql::SQLException &e)
	{
		std::cout << "# ERR: SQLException in " << __FILE__;
		std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
	}
	catch (std::bad_alloc)
	{
		std::cout << "needs to be ran in debug mode" << std::endl;
	}
}


BuildServer::~BuildServer()
{
	delete m_connection;
	delete m_insertArtifact;
	delete m_addAgent;
	delete m_updateAgent;
}

sql::ResultSet* BuildServer::Execute(const char * statement)
{
	try
	{
		sql::Statement* stmt = m_connection->createStatement();
		sql::ResultSet* result = stmt->executeQuery(statement);
		delete stmt;
		return result;
	}
	catch(sql::SQLException &e)
	{
		std::cout << "# ERR: SQLException in " << __FILE__;
		std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;

		return NULL;
	}
}
