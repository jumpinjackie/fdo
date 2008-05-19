/*
 * Copyright (C) 2004-2006  Autodesk, Inc.
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of version 2.1 of the GNU Lesser
 * General Public License as published by the Free Software Foundation.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include "stdafx.h"

#include "FdoRdbmsSQLCommand.h"
#include "FdoRdbmsSQLDataReader.h"
#include "FdoRdbmsUtil.h"

#define SQL_CLEANUP \
        if ( qid != -1) \
            (void) m_DbiConnection->dbi_free(qid, INULP);



FdoRdbmsSQLCommand::FdoRdbmsSQLCommand():
m_SqlString(NULL),
m_DbiConnection (NULL)
{
}

// Constructs an instance of a SQLCommand using the specified arguments.
FdoRdbmsSQLCommand::FdoRdbmsSQLCommand(FdoIConnection* connection):
m_SqlString( NULL ),
m_DbiConnection (NULL)
{
  mFdoConnection = static_cast<FdoRdbmsConnection*>(connection);
  if( mFdoConnection )
  {
      mFdoConnection->AddRef();
      m_DbiConnection = mFdoConnection->GetDbiConnection();
  }
}

// Default destructor for the SQLCommand command.
FdoRdbmsSQLCommand::~FdoRdbmsSQLCommand()
{
    if( m_SqlString )
        delete[] m_SqlString;
    if( mFdoConnection )
        mFdoConnection->Release();
}

// Gets the SQL statement to be executed as a string.
const wchar_t* FdoRdbmsSQLCommand::GetSQLStatement()
{
    return m_SqlString;
}

// Sets the SQL statement to be executed as a string.
void FdoRdbmsSQLCommand::SetSQLStatement(const wchar_t* value)
{
    if( m_SqlString )
    {
        delete[] m_SqlString;
        m_SqlString = NULL;
    }

    if( value != NULL )
    {
        m_SqlString = new wchar_t[ wcslen( value ) + 1];
        wcscpy( m_SqlString, value );
    }
}

// Executes the SQL statement against the connection object and returns
// the number of rows affected.
FdoInt32 FdoRdbmsSQLCommand::ExecuteNonQuery()
{
    bool                res = false;
    FdoInt32            numberOfRows = 0;


    if( m_DbiConnection == NULL )
        throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_13, "Connection not established"));

    if( m_SqlString == NULL )
       throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_41, "SQL string not initialized"));

    try
    {
        FdoStringP sqlString(m_SqlString);

        numberOfRows = m_DbiConnection->GetGdbiConnection()->ExecuteNonQuery( (wchar_t *)(const wchar_t *)sqlString );

    }
    catch (FdoException *ex)
    {
        FdoCommandException *exp = FdoCommandException::Create(ex->GetExceptionMessage(), ex);
        ex->Release();
        throw exp;
    }
    return numberOfRows;
}

// Executes the SQL statement against the connection object and returns
// an ISQLDataReader.
FdoISQLDataReader* FdoRdbmsSQLCommand::ExecuteReader()
{
    int                 qid = -1;
    bool                res = false;
    FdoInt32            numberOfRows = 0;

    if( m_DbiConnection == NULL )
        throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_13, "Connection not established"));

    if( m_SqlString == NULL )
        throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_41, "SQL string not initialized"));


    FdoStringP sqlString(m_SqlString);

    GdbiQueryResult *query = m_DbiConnection->GetGdbiConnection()->ExecuteQuery( (wchar_t *)(const wchar_t *) sqlString );

    return new FdoRdbmsSQLDataReader(mFdoConnection , query );
}

