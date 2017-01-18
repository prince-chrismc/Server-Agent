/**
 * @file    Statement.cpp
 * @ingroup SQLiteCpp
 * @brief   A prepared SQLite Statement is a compiled SQL query ready to be executed, pointing to a row of result.
 *
 * Copyright (c) 2012-2016 Sebastien Rombauts (sebastien.rombauts@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#include <SQL/Statement.h>

#include <SQL/Database.h>
#include <SQL/Column.h>
#include <SQL/Assertion.h>
#include <SQL/Exception.h>

#include <SQL/sqlite3.h>

namespace SQL
{

	// Compile and register the SQL query for the provided SQLite Database Connection
	Statement::Statement(Database &aDatabase, const char* apQuery) :
		m_Query(apQuery),
		m_StmtPtr(aDatabase.m_pSQLite, m_Query), // prepare the SQL query, and ref count (needs Database friendship)
		m_ColumnCount(0),
		m_Ok(false),
		mbDone(false)
	{
		m_ColumnCount = sqlite3_column_count(m_StmtPtr);
	}

	// Compile and register the SQL query for the provided SQLite Database Connection
	Statement::Statement(Database &aDatabase, const std::string& aQuery) :
		m_Query(aQuery),
		m_StmtPtr(aDatabase.m_pSQLite, m_Query), // prepare the SQL query, and ref count (needs Database friendship)
		m_ColumnCount(0),
		m_Ok(false),
		mbDone(false)
	{
		m_ColumnCount = sqlite3_column_count(m_StmtPtr);
	}


	// Finalize and unregister the SQL query from the SQLite Database Connection.
	Statement::~Statement() noexcept // nothrow
	{
		// the finalization will be done by the destructor of the last shared pointer
	}

	// Reset the statement to make it ready for a new execution (see also #clearBindings() bellow)
	void Statement::reset()
	{
		m_Ok = false;
		mbDone = false;
		const int ret = sqlite3_reset(m_StmtPtr);
		check(ret);
	}

	// Clears away all the bindings of a prepared statement (can be associated with #reset() above).
	void Statement::clearBindings()
	{
		const int ret = sqlite3_clear_bindings(m_StmtPtr);
		check(ret);
	}

	// Bind an int value to a parameter "?", "?NNN", ":VVV", "@VVV" or "$VVV" in the SQL prepared statement
	void Statement::bind(const int aIndex, const int aValue)
	{
		const int ret = sqlite3_bind_int(m_StmtPtr, aIndex, aValue);
		check(ret);
	}

	// Bind a 32bits unsigned int value to a parameter "?", "?NNN", ":VVV", "@VVV" or "$VVV" in the SQL prepared statement
	void Statement::bind(const int aIndex, const unsigned aValue)
	{
		const int ret = sqlite3_bind_int64(m_StmtPtr, aIndex, aValue);
		check(ret);
	}

	// Bind a 64bits int value to a parameter "?", "?NNN", ":VVV", "@VVV" or "$VVV" in the SQL prepared statement
	void Statement::bind(const int aIndex, const long long aValue)
	{
		const int ret = sqlite3_bind_int64(m_StmtPtr, aIndex, aValue);
		check(ret);
	}

	// Bind a double (64bits float) value to a parameter "?", "?NNN", ":VVV", "@VVV" or "$VVV" in the SQL prepared statement
	void Statement::bind(const int aIndex, const double aValue)
	{
		const int ret = sqlite3_bind_double(m_StmtPtr, aIndex, aValue);
		check(ret);
	}

	// Bind a string value to a parameter "?", "?NNN", ":VVV", "@VVV" or "$VVV" in the SQL prepared statement
	void Statement::bind(const int aIndex, const std::string& aValue)
	{
		const int ret = sqlite3_bind_text(m_StmtPtr, aIndex, aValue.c_str(),
			static_cast<int>(aValue.size()), SQLITE_TRANSIENT);
		check(ret);
	}

	// Bind a text value to a parameter "?", "?NNN", ":VVV", "@VVV" or "$VVV" in the SQL prepared statement
	void Statement::bind(const int aIndex, const char* apValue)
	{
		const int ret = sqlite3_bind_text(m_StmtPtr, aIndex, apValue, -1, SQLITE_TRANSIENT);
		check(ret);
	}

	// Bind a binary blob value to a parameter "?", "?NNN", ":VVV", "@VVV" or "$VVV" in the SQL prepared statement
	void Statement::bind(const int aIndex, const void* apValue, const int aSize)
	{
		const int ret = sqlite3_bind_blob(m_StmtPtr, aIndex, apValue, aSize, SQLITE_TRANSIENT);
		check(ret);
	}

	// Bind a string value to a parameter "?", "?NNN", ":VVV", "@VVV" or "$VVV" in the SQL prepared statement
	void Statement::bindNoCopy(const int aIndex, const std::string& aValue)
	{
		const int ret = sqlite3_bind_text(m_StmtPtr, aIndex, aValue.c_str(),
			static_cast<int>(aValue.size()), SQLITE_STATIC);
		check(ret);
	}

	// Bind a text value to a parameter "?", "?NNN", ":VVV", "@VVV" or "$VVV" in the SQL prepared statement
	void Statement::bindNoCopy(const int aIndex, const char* apValue)
	{
		const int ret = sqlite3_bind_text(m_StmtPtr, aIndex, apValue, -1, SQLITE_STATIC);
		check(ret);
	}

	// Bind a binary blob value to a parameter "?", "?NNN", ":VVV", "@VVV" or "$VVV" in the SQL prepared statement
	void Statement::bindNoCopy(const int aIndex, const void* apValue, const int aSize)
	{
		const int ret = sqlite3_bind_blob(m_StmtPtr, aIndex, apValue, aSize, SQLITE_STATIC);
		check(ret);
	}

	// Bind a NULL value to a parameter "?", "?NNN", ":VVV", "@VVV" or "$VVV" in the SQL prepared statement
	void Statement::bind(const int aIndex)
	{
		const int ret = sqlite3_bind_null(m_StmtPtr, aIndex);
		check(ret);
	}


	// Bind an int value to a parameter "?NNN", ":VVV", "@VVV" or "$VVV" in the SQL prepared statement
	void Statement::bind(const char* apName, const int aValue)
	{
		const int index = sqlite3_bind_parameter_index(m_StmtPtr, apName);
		const int ret = sqlite3_bind_int(m_StmtPtr, index, aValue);
		check(ret);
	}

	// Bind a 32bits unsigned int value to a parameter "?NNN", ":VVV", "@VVV" or "$VVV" in the SQL prepared statement
	void Statement::bind(const char* apName, const unsigned aValue)
	{
		const int index = sqlite3_bind_parameter_index(m_StmtPtr, apName);
		const int ret = sqlite3_bind_int64(m_StmtPtr, index, aValue);
		check(ret);
	}

	// Bind a 64bits int value to a parameter "?NNN", ":VVV", "@VVV" or "$VVV" in the SQL prepared statement
	void Statement::bind(const char* apName, const long long aValue)
	{
		const int index = sqlite3_bind_parameter_index(m_StmtPtr, apName);
		const int ret = sqlite3_bind_int64(m_StmtPtr, index, aValue);
		check(ret);
	}

	// Bind a double (64bits float) value to a parameter "?NNN", ":VVV", "@VVV" or "$VVV" in the SQL prepared statement
	void Statement::bind(const char* apName, const double aValue)
	{
		const int index = sqlite3_bind_parameter_index(m_StmtPtr, apName);
		const int ret = sqlite3_bind_double(m_StmtPtr, index, aValue);
		check(ret);
	}

	// Bind a string value to a parameter "?NNN", ":VVV", "@VVV" or "$VVV" in the SQL prepared statement
	void Statement::bind(const char* apName, const std::string& aValue)
	{
		const int index = sqlite3_bind_parameter_index(m_StmtPtr, apName);
		const int ret = sqlite3_bind_text(m_StmtPtr, index, aValue.c_str(),
			static_cast<int>(aValue.size()), SQLITE_TRANSIENT);
		check(ret);
	}

	// Bind a text value to a parameter "?NNN", ":VVV", "@VVV" or "$VVV" in the SQL prepared statement
	void Statement::bind(const char* apName, const char* apValue)
	{
		const int index = sqlite3_bind_parameter_index(m_StmtPtr, apName);
		const int ret = sqlite3_bind_text(m_StmtPtr, index, apValue, -1, SQLITE_TRANSIENT);
		check(ret);
	}

	// Bind a binary blob value to a parameter "?NNN", ":VVV", "@VVV" or "$VVV" in the SQL prepared statement
	void Statement::bind(const char* apName, const void* apValue, const int aSize)
	{
		const int index = sqlite3_bind_parameter_index(m_StmtPtr, apName);
		const int ret = sqlite3_bind_blob(m_StmtPtr, index, apValue, aSize, SQLITE_TRANSIENT);
		check(ret);
	}

	// Bind a string value to a parameter "?NNN", ":VVV", "@VVV" or "$VVV" in the SQL prepared statement
	void Statement::bindNoCopy(const char* apName, const std::string& aValue)
	{
		const int index = sqlite3_bind_parameter_index(m_StmtPtr, apName);
		const int ret = sqlite3_bind_text(m_StmtPtr, index, aValue.c_str(),
			static_cast<int>(aValue.size()), SQLITE_STATIC);
		check(ret);
	}

	// Bind a text value to a parameter "?NNN", ":VVV", "@VVV" or "$VVV" in the SQL prepared statement
	void Statement::bindNoCopy(const char* apName, const char* apValue)
	{
		const int index = sqlite3_bind_parameter_index(m_StmtPtr, apName);
		const int ret = sqlite3_bind_text(m_StmtPtr, index, apValue, -1, SQLITE_STATIC);
		check(ret);
	}

	// Bind a binary blob value to a parameter "?NNN", ":VVV", "@VVV" or "$VVV" in the SQL prepared statement
	void Statement::bindNoCopy(const char* apName, const void* apValue, const int aSize)
	{
		const int index = sqlite3_bind_parameter_index(m_StmtPtr, apName);
		const int ret = sqlite3_bind_blob(m_StmtPtr, index, apValue, aSize, SQLITE_STATIC);
		check(ret);
	}

	// Bind a NULL value to a parameter "?NNN", ":VVV", "@VVV" or "$VVV" in the SQL prepared statement
	void Statement::bind(const char* apName)
	{
		const int index = sqlite3_bind_parameter_index(m_StmtPtr, apName);
		const int ret = sqlite3_bind_null(m_StmtPtr, index);
		check(ret);
	}


	// Execute a step of the query to fetch one row of results
	bool Statement::executeStep()
	{
		if(false == mbDone)
		{
			const int ret = sqlite3_step(m_StmtPtr);
			if(SQLITE_ROW == ret) // one row is ready : call getColumn(N) to access it
			{
				m_Ok = true;
			}
			else if(SQLITE_DONE == ret) // no (more) row ready : the query has finished executing
			{
				m_Ok = false;
				mbDone = true;
			}
			else
			{
				m_Ok = false;
				mbDone = false;
				throw SQL::Exception(m_StmtPtr, ret);
			}
		}
		else
		{
			throw SQL::Exception("Statement needs to be reseted.");
		}

		return m_Ok; // true only if one row is accessible by getColumn(N)
	}

	// Execute a one-step query with no expected result
	int Statement::exec()
	{
		if(false == mbDone)
		{
			const int ret = sqlite3_step(m_StmtPtr);
			if(SQLITE_DONE == ret) // the statement has finished executing successfully
			{
				m_Ok = false;
				mbDone = true;
			}
			else if(SQLITE_ROW == ret)
			{
				m_Ok = false;
				mbDone = false;
				throw SQL::Exception("exec() does not expect results. Use executeStep.");
			}
			else
			{
				m_Ok = false;
				mbDone = false;
				throw SQL::Exception(m_StmtPtr, ret);
			}
		}
		else
		{
			throw SQL::Exception("Statement need to be reseted.");
		}

		// Return the number of rows modified by those SQL statements (INSERT, UPDATE or DELETE)
		return sqlite3_changes(m_StmtPtr);
	}

	// Return a copy of the column data specified by its index starting at 0
	// (use the Column copy-constructor)
	Column Statement::getColumn(const int aIndex)
	{
		checkRow();
		checkIndex(aIndex);

		// Share the Statement Object handle with the new Column created
		return Column(m_StmtPtr, aIndex);
	}

	// Return a copy of the column data specified by its column name starting at 0
	// (use the Column copy-constructor)
	Column  Statement::getColumn(const char* apName)
	{
		checkRow();
		const int index = getColumnIndex(apName);

		// Share the Statement Object handle with the new Column created
		return Column(m_StmtPtr, index);
	}

	// Test if the column is NULL
	bool Statement::isColumnNull(const int aIndex) const
	{
		checkRow();
		checkIndex(aIndex);
		return (SQLITE_NULL == sqlite3_column_type(m_StmtPtr, aIndex));
	}

	bool Statement::isColumnNull(const char* apName) const
	{
		checkRow();
		const int index = getColumnIndex(apName);
		return (SQLITE_NULL == sqlite3_column_type(m_StmtPtr, index));
	}

	// Return the named assigned to the specified result column (potentially aliased)
	const char* Statement::getColumnName(const int aIndex) const
	{
		checkIndex(aIndex);
		return sqlite3_column_name(m_StmtPtr, aIndex);
	}

#ifdef SQLITE_ENABLE_COLUMN_METADATA
	// Return the named assigned to the specified result column (potentially aliased)
	const char* Statement::getColumnOriginName(const int aIndex) const
	{
		checkIndex(aIndex);
		return sqlite3_column_origin_name(m_StmtPtr, aIndex);
	}
#endif

	// Return the index of the specified (potentially aliased) column name
	int Statement::getColumnIndex(const char* apName) const
	{
		// Build the map of column index by name on first call
		if(m_ColumnNames.empty())
		{
			for(int i = 0; i < m_ColumnCount; ++i)
			{
				const char* pName = sqlite3_column_name(m_StmtPtr, i);
				m_ColumnNames[pName] = i;
			}
		}

		const TColumnNames::const_iterator iIndex = m_ColumnNames.find(apName);
		if(iIndex == m_ColumnNames.end())
		{
			throw SQL::Exception("Unknown column name.");
		}

		return (*iIndex).second;
	}

	// Return the numeric result code for the most recent failed API call (if any).
	int Statement::getErrorCode() const noexcept // nothrow
	{
		return sqlite3_errcode(m_StmtPtr);
	}
	// Return the extended numeric result code for the most recent failed API call (if any).
	int Statement::getExtendedErrorCode() const noexcept // nothrow
	{
		return sqlite3_extended_errcode(m_StmtPtr);
	}
	// Return UTF-8 encoded English language explanation of the most recent failed API call (if any).
	const char* Statement::getErrorMsg() const noexcept // nothrow
	{
		return sqlite3_errmsg(m_StmtPtr);
	}

	////////////////////////////////////////////////////////////////////////////////
	// Internal class : shared pointer to the sqlite3_stmt SQLite Statement Object
	////////////////////////////////////////////////////////////////////////////////

	/**
	 * @brief Prepare the statement and initialize its reference counter
	 *
	 * @param[in] apSQLite  The sqlite3 database connexion
	 * @param[in] aQuery    The SQL query string to prepare
	 */
	Statement::Ptr::Ptr(sqlite3* apSQLite, std::string& aQuery) :
		m_pSQLite(apSQLite),
		m_pStmt(NULL),
		mpRefCount(NULL)
	{
		const int ret = sqlite3_prepare_v2(apSQLite, aQuery.c_str(), static_cast<int>(aQuery.size()), &m_pStmt, NULL);
		if(SQLITE_OK != ret)
		{
			throw SQL::Exception(apSQLite, ret);
		}
		// Initialize the reference counter of the sqlite3_stmt :
		// used to share the m_StmtPtr between Statement and Column objects;
		// This is needed to enable Column objects to live longer than the Statement objet it refers to.
		mpRefCount = new unsigned int(1);  // NOLINT(readability/casting)
	}

	/**
	 * @brief Copy constructor increments the ref counter
	 *
	 * @param[in] aPtr Pointer to copy
	 */
	Statement::Ptr::Ptr(const Statement::Ptr& aPtr) :
		m_pSQLite(aPtr.m_pSQLite),
		m_pStmt(aPtr.m_pStmt),
		mpRefCount(aPtr.mpRefCount)
	{
		assert(NULL != mpRefCount);
		assert(0 != *mpRefCount);

		// Increment the reference counter of the sqlite3_stmt,
		// asking not to finalize the sqlite3_stmt during the lifetime of the new objet
		++(*mpRefCount);
	}

	/**
	 * @brief Decrement the ref counter and finalize the sqlite3_stmt when it reaches 0
	 */
	Statement::Ptr::~Ptr() noexcept // nothrow
	{
		assert(NULL != mpRefCount);
		assert(0 != *mpRefCount);

		// Decrement and check the reference counter of the sqlite3_stmt
		--(*mpRefCount);
		if(0 == *mpRefCount)
		{
			// If count reaches zero, finalize the sqlite3_stmt, as no Statement nor Column objet use it anymore.
			// No need to check the return code, as it is the same as the last statement evaluation.
			sqlite3_finalize(m_pStmt);

			// and delete the reference counter
			delete mpRefCount;
			mpRefCount = NULL;
			m_pStmt = NULL;
		}
		// else, the finalization will be done later, by the last object
	}


}  // namespace SQLite
