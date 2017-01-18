/**
 * @file    Column.cpp
 * @ingroup SQLiteCpp
 * @brief   Encapsulation of a Column in a row of the result pointed by the prepared SQLite::Statement.
 *
 * Copyright (c) 2012-2016 Sebastien Rombauts (sebastien.rombauts@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <iostream>
#include <SQL/sqlite3.h>
#include <SQL/Column.h>


namespace SQL
{

	const int INTEGER = SQLITE_INTEGER;
	const int FLOAT = SQLITE_FLOAT;
	const int TEXT = SQLITE_TEXT;
	const int BLOB = SQLITE_BLOB;
	const int Null = SQLITE_NULL;


	// Encapsulation of a Column in a row of the result pointed by the prepared Statement.
	Column::Column(Statement::Ptr& aStmtPtr, int aIndex) noexcept : // nothrow
	m_StmtPtr(aStmtPtr),
		m_Index(aIndex)
	{
	}

	// Finalize and unregister the SQL query from the SQLite Database Connection.
	Column::~Column() noexcept // nothrow
	{
		// the finalization will be done by the destructor of the last shared pointer
	}

	// Return the named assigned to this result column (potentially aliased)
	const char* Column::getName() const noexcept // nothrow
	{
		return sqlite3_column_name(m_StmtPtr, m_Index);
	}

#ifdef SQLITE_ENABLE_COLUMN_METADATA
	// Return the name of the table column that is the origin of this result column
	const char* Column::getOriginName() const noexcept // nothrow
	{
		return sqlite3_column_origin_name(m_StmtPtr, m_Index);
	}
#endif

	// Return the integer value of the column specified by its index starting at 0
	int Column::getInt() const noexcept // nothrow
	{
		return sqlite3_column_int(m_StmtPtr, m_Index);
	}

	// Return the unsigned integer value of the column specified by its index starting at 0
	unsigned Column::getUInt() const noexcept // nothrow
	{
		return static_cast<unsigned>(getInt64());
	}

	// Return the 64bits integer value of the column specified by its index starting at 0
	long long Column::getInt64() const noexcept // nothrow
	{
		return sqlite3_column_int64(m_StmtPtr, m_Index);
	}

	// Return the double value of the column specified by its index starting at 0
	double Column::getDouble() const noexcept // nothrow
	{
		return sqlite3_column_double(m_StmtPtr, m_Index);
	}

	// Return a pointer to the text value (NULL terminated string) of the column specified by its index starting at 0
	const char* Column::getText(const char* apDefaultValue /* = "" */) const noexcept // nothrow
	{
		const char* pText = reinterpret_cast<const char*>(sqlite3_column_text(m_StmtPtr, m_Index));
		return (pText ? pText : apDefaultValue);
	}

	// Return a pointer to the blob value (*not* NULL terminated) of the column specified by its index starting at 0
	const void* Column::getBlob() const noexcept // nothrow
	{
		return sqlite3_column_blob(m_StmtPtr, m_Index);
	}

	// Return a std::string to a TEXT or BLOB column
	std::string Column::getString() const noexcept // nothrow
	{
		// Note: using sqlite3_column_blob and not sqlite3_column_text
		// - no need for sqlite3_column_text to add a \0 on the end, as we're getting the bytes length directly
		const char *data = static_cast<const char *>(sqlite3_column_blob(m_StmtPtr, m_Index));

		// SQLite docs: "The safest policy is to invoke… sqlite3_column_blob() followed by sqlite3_column_bytes()"
		// Note: std::string is ok to pass nullptr as first arg, if length is 0
		return std::string(data, sqlite3_column_bytes(m_StmtPtr, m_Index));
	}

	// Return the type of the value of the column
	int Column::getType() const noexcept // nothrow
	{
		return sqlite3_column_type(m_StmtPtr, m_Index);
	}

	// Return the number of bytes used by the text value of the column
	int Column::getBytes() const noexcept // nothrow
	{
		return sqlite3_column_bytes(m_StmtPtr, m_Index);
	}

	// Standard std::ostream inserter
	std::ostream& operator<<(std::ostream& aStream, const Column& aColumn)
	{
		aStream << aColumn.getText();
		return aStream;
	}


}  // namespace SQLite
