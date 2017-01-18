// SQLServerUnitTest.cpp : Defines the entry point for the console application.
//

#include "SQL/Database.h"

int main()
{
	SQL::Database db("BUILDSERVER", SQL::OPEN_READWRITE);

    return 0;
}

