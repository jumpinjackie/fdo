#ifndef _SQLITEQUERYRESULT_H
#define _SQLITEQUERYRESULT_H
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
#ifdef _WIN32
#pragma once
#endif

class SQLiteDataBase;

/*
** A query result class; wrapper for the SQLite query
*/
class SQLiteQueryResult
{
    friend class SQLiteDataBase;

private:
    sqlite3             *m_pDB;
    sqlite3_stmt        *m_pStmt;
    int                 m_ColCount;

private:
    SQLiteQueryResult();

    SQLiteQueryResult(const SQLiteQueryResult& rQuery);

    SQLiteQueryResult( sqlite3* pDB, sqlite3_stmt* pStmt );

    SQLiteQueryResult& operator=(const SQLiteQueryResult& rQuery);

public:
    ~SQLiteQueryResult(void);

    /*
    ** Returns the number of columns returned by the query.
    */
    int ColumnCount();

    /*
    ** Returns the index of a given column. bFound is set to false if the column is not found.
    */
    int Index(const char* szName, bool & bFound );

    /*
    ** Return the name of the column with index nCol. bFound is set to false if the column is not found.
    */
    const char* ColumnName(int nCol, bool & bFound );

    /*
    ** Return the column type: One of  SQLITE_INTEGER, SQLITE_FLOAT, SQLITE_TEXT, SQLITE_BLOB or SQLITE_NULL
    */
    int ColumnDataType(int nCol, bool & bFound );

    /*
    ** Returns the string value of the column identified by index num. 
    ** isNull is set to true if the column is null. 
    ** bFound is to false if the column is not found.
    */
    const char* StringValue( int numb ,bool &isNull ,bool & bFound  );

    /*
    ** Returns the string value of the column identified by name. 
    ** isNull is set to true if the column is null. 
    ** bFound is to false if the column is not found.
    */
    const char* StringValue(const char* szName, bool &isNull ,bool & bFound  );

    /*
    ** Returns the integer value of the column identified by index num. 
    ** isNull is set to true if the column is null. 
    ** bFound is to false if the column is not found.
    */
    int IntValue(int numb, bool &isNull ,bool & bFound );

    /*
    ** Returns the integer value of the column identified by name. 
    ** isNull is set to true if the column is null. 
    ** bFound is to false if the column is not found.
    */
    int IntValue(const char* szName, bool &isNull ,bool & bFound );

    /*
    ** Returns the double value of the column identified by index num. 
    ** isNull is set to true if the column is null. 
    ** bFound is to false if the column is not found.
    */
    double DoubleValue(int numb, bool &isNull ,bool & bFound );

    /*
    ** Returns the double value of the column identified by name. 
    ** isNull is set to true if the column is null. 
    ** bFound is to false if the column is not found.
    */
    double DoubleValue(const char* szName, bool &isNull ,bool & bFound );

    /*
    ** Returns the blob value of the column identified by index num. 
    ** isNull is set to true if the column is null. 
    ** bFound is to false if the column is not found.
    */
    const unsigned char* BlobValue(int numb, int& len, bool &isNull ,bool & bFound);

    /*
    ** Returns the blob value of the column identified by name. 
    ** isNull is set to true if the column is null. 
    ** bFound is to false if the column is not found.
    */
    const unsigned char* BlobValue(const char* szName, int& len, bool &isNull ,bool & bFound );

    /*
    ** Returns true if the column is null, false otherwise. bFound is to false if the column is not found.
    */
    bool IsNull(int numb, bool & bFound );

    /*
    ** Returns true if the column is null, false otherwise. bFound is to false if the column is not found.
    */
    bool IsNull(const char* szName, bool & bFound );

    /*
    ** Moves the cursor to the next row.
    */
    bool NextRow();

    /*
    ** Close the query object
    */
    void Close();
};
#endif

