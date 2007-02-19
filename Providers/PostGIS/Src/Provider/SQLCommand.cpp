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
#include "Connection.h"

#include <cassert>
#include <string>

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
        throw FdoException::Create(NlsMsgGet(MSG_POSTGIS_COMMAND_SQL_INVALID,
            "The passed SQL statement is invalid or empty: '%1$ls'.",
            L""));
    }
    
    mSql = sql;
    
    FDOLOG_WRITE("SQL: %s", static_cast<char const*>(mSql));
}
    
FdoInt32 SQLCommand::ExecuteNonQuery()
{
    FDOLOG_MARKER("SQLCommand::+ExecuteNonQuery");

    // TODO: Add support for collection of input parameters
    //FdoPtr<FdoParameterValueCollection> cmdParams = GetParameterValues();

    ExecStatusType pgStatus = PGRES_FATAL_ERROR;
    FdoSize cmdTuples = 0;
    
    try
    {
        std::string sql(static_cast<char const*>(mSql));
        pgStatus = mConn->PgExecuteCommand(sql.c_str(), cmdTuples);
        assert(PGRES_COMMAND_OK == pgStatus);
    }
    catch (FdoException* e)
    {
        FdoCommandException* de = NULL;
        de = FdoCommandException::Create(NlsMsgGet(MSG_POSTGIS_COMMAND_SQL_FAILED,
            "The execution of SQL command failed."), e);
        e->Release();
        throw de;
    }

    

    return static_cast<FdoInt32>(cmdTuples);
}

FdoISQLDataReader* SQLCommand::ExecuteReader()
{
    assert(!"NOT IMPLEMENTED");
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
// Private operations
///////////////////////////////////////////////////////////////////////////////


}} // namespace fdo::postgis
