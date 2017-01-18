/**
 * @file    Backup.cpp
 * @ingroup SQLiteCpp
 * @brief   Backup is used to backup a database file in a safe and online way.
 *
 * Copyright (c) 2015 Shibao HONG (shibaohong@outlook.com)
 * Copyright (c) 2015-2016 Sebastien Rombauts (sebastien.rombauts@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#include <SQL/Backup.h>
#include <SQL/Exception.h>

#include <SQL/sqlite3.h>

namespace SQL
{

	// Initialize resource for SQLite database backup
	Backup::Backup(Database&    aDestDatabase,
		const char*  apDestDatabaseName,
		Database&    aSrcDatabase,
		const char*  apSrcDatabaseName) :
		m_pSQLiteBackup(NULL)
	{
		m_pSQLiteBackup = sqlite3_backup_init(aDestDatabase.getHandle(),
			apDestDatabaseName,
			aSrcDatabase.getHandle(),
			apSrcDatabaseName);
		if(NULL == m_pSQLiteBackup)
		{
			// If an error occurs, the error code and message are attached to the destination database connection.
			throw SQL::Exception(aDestDatabase.getHandle());
		}
	}

	// Initialize resource for SQLite database backup
	Backup::Backup(Database&            aDestDatabase,
		const std::string&   aDestDatabaseName,
		Database&            aSrcDatabase,
		const std::string&   aSrcDatabaseName) :
		m_pSQLiteBackup(NULL)
	{
		m_pSQLiteBackup = sqlite3_backup_init(aDestDatabase.getHandle(),
			aDestDatabaseName.c_str(),
			aSrcDatabase.getHandle(),
			aSrcDatabaseName.c_str());
		if(NULL == m_pSQLiteBackup)
		{
			// If an error occurs, the error code and message are attached to the destination database connection.
			throw SQL::Exception(aDestDatabase.getHandle());
		}
	}

	// Initialize resource for SQLite database backup
	Backup::Backup(Database &aDestDatabase, Database &aSrcDatabase) :
		m_pSQLiteBackup(NULL)
	{
		m_pSQLiteBackup = sqlite3_backup_init(aDestDatabase.getHandle(),
			"main",
			aSrcDatabase.getHandle(),
			"main");
		if(NULL == m_pSQLiteBackup)
		{
			// If an error occurs, the error code and message are attached to the destination database connection.
			throw SQL::Exception(aDestDatabase.getHandle());
		}
	}

	// Release resource for SQLite database backup
	Backup::~Backup() noexcept
	{
		if(NULL != m_pSQLiteBackup)
		{
			sqlite3_backup_finish(m_pSQLiteBackup);
		}
	}

	// Execute backup step with a given number of source pages to be copied
	int Backup::executeStep(const int aNumPage /* = -1 */)
	{
		const int res = sqlite3_backup_step(m_pSQLiteBackup, aNumPage);
		if(SQLITE_OK != res && SQLITE_DONE != res && SQLITE_BUSY != res && SQLITE_LOCKED != res)
		{
			throw SQL::Exception(sqlite3_errstr(res), res);
		}
		return res;
	}

	// Get the number of remaining source pages to be copied in this backup process
	int Backup::getRemainingPageCount()
	{
		return sqlite3_backup_remaining(m_pSQLiteBackup);
	}

	// Get the number of total source pages to be copied in this backup process
	int Backup::getTotalPageCount()
	{
		return sqlite3_backup_pagecount(m_pSQLiteBackup);
	}

}  // namespace SQLite
