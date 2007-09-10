//
// Copyright (C) 2007 Refractions Research, Inc. 
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
//
#include "stdafx.h"

#include "PostGisProvider.h"
#include "SQLCommand.h"
#include "SQLDataReader.h"
#include "Connection.h"
// std
#include <cassert>
#include <string>
// boost
#include <boost/lexical_cast.hpp>

namespace fdo { namespace postgis {

SQLCommand::SQLCommand(Connection* conn) : Base(conn)
{
}

SQLCommand::~SQLCommand()
{
}

///////////////////////////////////////////////////////////////////////////////
// FdoISQLCommand interface
///////////////////////////////////////////////////////////////////////////////

FdoString* SQLCommand::GetSQLStatement()
{
    FDOLOG_MARKER("SQLCommand::+GetSQLStatement");

    return NULL;
}

void SQLCommand::SetSQLStatement(FdoString* sql)
{
    FDOLOG_MARKER("SQLCommand::+SetSQLStatement");

    if (NULL == sql)
    {
        FDOLOG_WRITE("ERROR: The passed SQL statement is NULL");

        throw FdoCommandException::Create(NlsMsgGet(MSG_POSTGIS_COMMAND_SQL_INVALID,
            "The passed SQL statement is invalid or empty: '%1$ls'.",
            L""));
    }
    
    mSql = sql;
    
    FDOLOG_WRITE("SQL:\n\t%s", static_cast<char const*>(mSql));
}
    
FdoInt32 SQLCommand::ExecuteNonQuery()
{
    FDOLOG_MARKER("SQLCommand::+ExecuteNonQuery");

    // TODO: Add support of DDL statements detection
    // TODO: Add DDL flag to PgExecuteCommand calls

    FdoSize cmdTuples = 0;

    try
    {
        std::string sql(static_cast<char const*>(mSql));

        // Collect bind parameters
        details::pgexec_params_t params;
        Base::PgGenerateExecParams(params);
        
        // Execute SQL statement
        mConn->PgExecuteCommand(sql.c_str(), params, cmdTuples);
    }
    catch (FdoException* e)
    {
        FDOLOG_WRITE("ERROR: The execution of SQL command failed");

        FdoCommandException* ne = NULL;
        ne = FdoCommandException::Create(NlsMsgGet(MSG_POSTGIS_COMMAND_SQL_FAILED,
            "The execution of SQL command failed."), e);
        e->Release();
        throw ne;
    }

    return (static_cast<FdoInt32>(cmdTuples));
}

FdoISQLDataReader* SQLCommand::ExecuteReader()
{
    FDOLOG_MARKER("SQLCommand::+ExecuteReader");

    PgCursor::Ptr cursor(NULL);

    try
    {
        // Create a cursor associated with query results reader
        cursor = mConn->PgCreateCursor("crsSQLCmd");

        // Collect bind parameters
        details::pgexec_params_t params;
        Base::PgGenerateExecParams(params);

        // Open new cursor
        std::string sql(static_cast<char const*>(mSql));
        cursor->Declare(sql.c_str(), params);

        assert(NULL != cursor && "Cursor is NULL");
        return (new SQLDataReader(cursor));
    }
    catch (FdoException* e)
    {
        FDOLOG_WRITE("ERROR: The execution of SQL command failed");

        FdoCommandException* ne = NULL;
        ne = FdoCommandException::Create(NlsMsgGet(MSG_POSTGIS_COMMAND_SQL_FAILED,
                "The execution of SQL command failed."), e);
        e->Release();
        throw ne;
    }
}

///////////////////////////////////////////////////////////////////////////////
// Private operations
///////////////////////////////////////////////////////////////////////////////


}} // namespace fdo::postgis
