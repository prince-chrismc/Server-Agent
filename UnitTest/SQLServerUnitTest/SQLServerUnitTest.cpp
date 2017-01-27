// SQLServerUnitTest.cpp : Defines the entry point for the console application.
//

#include "SQL/MySQL/driver.h"
#include "SQL/MySQL/exception.h"
#include "SQL/MySQL/statement.h"

#include "SQL/BuildServer.h"

using namespace std;

int basetest();

int main(void)
{
	//basetest();

	BuildServer bs;

	sql::ResultSet* rs = bs.Execute("SELECT * FROM customer");
	/*ErrorCode err = bs.Execute("SELECT * FROM customer", rs);

	if (err.DidFail())
		return -1;*/

	while (rs->next())
	{
		cout << "... MySQL replies: ";
		/* Access column data by alias or column name */
		for (int i = 1; i < 6; i++)
		{
			cout << rs->getString(i) << "\t ";
		}
		cout << endl;
	}

}

int basetest()
{
	cout << endl;
	cout << "Running 'SELECT 'Hello World!' » AS _message'..." << endl;

	try
	{
		sql::Driver *driver;
		sql::Connection *con;
		sql::Statement *stmt;
		sql::ResultSet *res;

		/* Create a connection */
		driver = get_driver_instance();
		con = driver->connect("tcp://127.0.0.1:3306", "cmcarthur", "Acertuba");
		/* Connect to the MySQL test database */
		con->setSchema("sakila");

		stmt = con->createStatement();
		res = stmt->executeQuery("SELECT * FROM customer");
		while (res->next())
		{
			cout << "... MySQL replies: ";
			/* Access column data by alias or column name */
			for (int i = 1; i < 6; i++)
			{
				cout << res->getString(i) << "\t ";
			}

			cout << endl;
		}
		delete res;
		delete stmt;
		delete con;
	}
	catch (sql::SQLException &e)
	{
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
	}

	cout << endl;

	return EXIT_SUCCESS;
}