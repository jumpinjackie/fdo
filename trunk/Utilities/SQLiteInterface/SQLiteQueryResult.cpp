// 
//  
//  Copyright (C) 2004-2005  Autodesk, Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//  
#include "stdafx.h"
#include "SQLiteQueryResult.h"

SQLiteQueryResult::SQLiteQueryResult(void):
m_pDB( NULL ),
m_pStmt( NULL )
{
    
}

SQLiteQueryResult::~SQLiteQueryResult(void)
{
    if( m_pStmt )
    {
        sqlite3_finalize(m_pStmt);
    }
}

SQLiteQueryResult::SQLiteQueryResult( sqlite3* pDB, sqlite3_stmt* pStmt ):
m_pDB( pDB ),
m_pStmt( pStmt )
{
    m_ColCount = sqlite3_column_count( pStmt );
}

int SQLiteQueryResult::ColumnCount()
{
    return m_ColCount;
}

int SQLiteQueryResult::Index(const char* szName, bool & bFound )
{
    bFound = false;

    if (szName == NULL )
        return SQLITE_ERROR;
	
	for (int indx = 0; indx < m_ColCount; indx++)
	{
		const char* szTemp = sqlite3_column_name(m_pStmt, indx);

		if (strcmp(szName, szTemp) == 0)
		{
            bFound = true;
			return indx;
		}
	}
	return SQLITE_ERROR;
}

const char* SQLiteQueryResult::ColumnName(int nCol, bool & bFound )
{
    if (nCol < 0 || nCol > m_ColCount-1)
	{
        bFound = false;
		return "";
	}

    bFound = true;
	return sqlite3_column_name(m_pStmt, nCol);
}

int SQLiteQueryResult::ColumnDataType(int nCol, bool & bFound )
{
    if (m_ColCount < 0 || nCol > m_ColCount-1)
	{
        bFound = false;
		return SQLITE_ERROR;  // types start at 1; SQLITE_ERROR == 0
	}

    bFound = true;
	return sqlite3_column_type(m_pStmt, nCol);
}

const char* SQLiteQueryResult::StringValue( int numb, bool &bIsNull ,bool & bFound )
{
    if (numb < 0 || numb > m_ColCount-1)
	{
		bFound = false;
        return "";
	}

	return (const char*)sqlite3_column_text(m_pStmt, numb);
}

const char* SQLiteQueryResult::StringValue(const char* szName,bool &bIsNull ,bool & bFound )
{
    int idx = Index(szName, bFound );
    if( ! bFound )
        return "";

	return (const char*)sqlite3_column_text(m_pStmt, idx);
}

int SQLiteQueryResult::IntValue(int numb, bool &bIsNull ,bool & bFound )
{
    if (ColumnDataType( numb, bFound ) == SQLITE_NULL)
	{
        bIsNull = true;
		return SQLITE_ERROR;
	}
	else
	{
        if( ! bFound )
            return SQLITE_ERROR;
        
        bIsNull = false;
		return sqlite3_column_int(m_pStmt, numb);
	}
}

int SQLiteQueryResult::IntValue(const char* szName, bool &bIsNull ,bool & bFound )
{

    int idx = Index(szName, bFound );
    if( ! bFound )
        return SQLITE_ERROR;

    return IntValue( idx, bIsNull, bFound);
}

double SQLiteQueryResult::DoubleValue(int numb, bool &bIsNull ,bool & bFound )
{
    if (ColumnDataType( numb, bFound ) == SQLITE_NULL )
	{
        bIsNull = true;
		return SQLITE_ERROR;
	}
	else
	{
        if( ! bFound )
            return SQLITE_ERROR;

        bIsNull = false;
		return sqlite3_column_double(m_pStmt, numb);
	}
}

double SQLiteQueryResult::DoubleValue(const char* szName, bool &bIsNull ,bool & bFound )
{
    int idx = Index(szName, bFound );
    if( ! bFound )
        return 0.0;

    return DoubleValue( idx, bIsNull, bFound);
}

const unsigned char* SQLiteQueryResult::BlobValue(int numb, int& len, bool &bIsNull ,bool & bFound )
{
    if (ColumnDataType( numb, bFound ) == SQLITE_NULL )
	{
        bIsNull = true;
		return (const unsigned char*)"";
	}
	else
	{
        if( ! bFound )
            return (const unsigned char*)"";

        bIsNull = false;
		len = sqlite3_column_bytes(m_pStmt, numb);
	    return (const unsigned char*)sqlite3_column_blob(m_pStmt, numb);
	}
}

const unsigned char* SQLiteQueryResult::BlobValue(const char* szName, int& len, bool &bIsNull ,bool & bFound )
{
    int idx = Index(szName, bFound );
    if( ! bFound )
        return (const unsigned char*)"";

    return BlobValue( idx, len, bIsNull, bFound);
}

bool SQLiteQueryResult::IsNull(int numb, bool & bFound )
{
    return (ColumnDataType( numb, bFound ) == SQLITE_NULL );
}

bool SQLiteQueryResult::IsNull(const char* szName, bool & bFound )
{
    int idx = Index(szName, bFound );
    if( ! bFound )
        return false;

    return IsNull( idx, bFound );
}


bool SQLiteQueryResult::NextRow()
{
    int rc = sqlite3_step(m_pStmt);

	if (rc == SQLITE_DONE)
	{
		return false;
	}
	else if (rc == SQLITE_ROW)
	{
		return true;
	}
	else
	{
		rc = sqlite3_finalize(m_pStmt);
		m_pStmt = NULL;
		return false;
	}
}

void SQLiteQueryResult::Close()
{
    if( m_pStmt == NULL )
        return;

    sqlite3_finalize(m_pStmt);
    m_pStmt = NULL;
}

