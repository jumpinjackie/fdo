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
#include "PgCursor.h"
#include "Connection.h"
// std
#include <cassert>
#include <string>

namespace fdo { namespace postgis {

PgCursor::PgCursor(Connection* conn, std::string const& name)
    : mConn(conn), mName(name), mFetchRes(NULL)
{
    FDO_SAFE_ADDREF(mConn.p);

    Validate();
}

PgCursor::~PgCursor()
{
    Close();
}

const char* PgCursor::GetName() const
{
    return mName.c_str();
}

PGresult const* PgCursor::GetFetchResult() const
{
    // TODO: Handling of nothing fetched case.
    assert(NULL != mFetchRes);

    return mFetchRes;
}

void PgCursor::Declare(char const* query)
{
    assert(NULL == mFetchRes);
    Validate();

    ExecStatusType pgStatus = PGRES_FATAL_ERROR;

    // Begin transaction block
    pgStatus = mConn->PgExecuteCommand("BEGIN");
    assert(PGRES_COMMAND_OK == pgStatus);

    // Declare cursor
    std::string sql("DECLARE " + mName);
    sql += " CURSOR FOR ";
    sql += query;

    pgStatus = mConn->PgExecuteCommand(sql.c_str());
    assert(PGRES_COMMAND_OK == pgStatus);
}

void PgCursor::Close()
{
    Validate();

    ClearFetchResult();

    ExecStatusType pgStatus = PGRES_FATAL_ERROR;

    // End transaction block
    pgStatus = mConn->PgExecuteCommand("COMMIT");
    assert(PGRES_COMMAND_OK == pgStatus);

    // Close cursor
    std::string sql("CLOSE " + mName);
    pgStatus = mConn->PgExecuteCommand(sql.c_str());
    assert(PGRES_COMMAND_OK == pgStatus);
}

PGresult const* PgCursor::FetchNext()
{
    ClearFetchResult();

    assert(NULL == mFetchRes);

    std::string sql("FETCH NEXT FROM " + mName);
    mFetchRes = mConn->PgExecuteQuery(sql.c_str());
    return mFetchRes; 
}

///////////////////////////////////////////////////////////////////////////////
// Private operations
///////////////////////////////////////////////////////////////////////////////

void PgCursor::Validate()
{
    if (FdoConnectionState_Open != mConn->GetConnectionState())
    {
        throw FdoException::Create(NlsMsgGet(MSG_POSTGIS_CONNECTION_INVALID,
            "Connection is invalid."));
    }
}

void PgCursor::ClearFetchResult()
{
    if (NULL != mFetchRes)
    {
        PQclear(mFetchRes);
        mFetchRes = NULL;
    }
}

}} // namespace fdo::postgis
