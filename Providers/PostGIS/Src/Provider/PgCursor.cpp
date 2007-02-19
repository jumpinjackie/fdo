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
    : mConn(conn), mName(name), mFetchRes(NULL), mIsClosed(true)
{
    FDO_SAFE_ADDREF(mConn.p);

    Validate();
}

PgCursor::~PgCursor()
{
}

///////////////////////////////////////////////////////////////////////////////
// FdoIDisposable interface
///////////////////////////////////////////////////////////////////////////////

void PgCursor::Dispose()
{
    FDOLOG_MARKER("PgCursor::#Dispose");

    Close();
    delete this;
}

///////////////////////////////////////////////////////////////////////////////
// PgCursor interface
///////////////////////////////////////////////////////////////////////////////

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
    FDOLOG_MARKER("PgCursor::+Declare");

    if (!mIsClosed)
    {
        Close();
    }

    Validate();
    assert(NULL == mFetchRes);

    ExecStatusType pgStatus = PGRES_FATAL_ERROR;

    // Begin transaction
    pgStatus = mConn->PgExecuteCommand("BEGIN");
    assert(PGRES_COMMAND_OK == pgStatus);

    // Declare cursor
    std::string sql("DECLARE " + mName);
    sql += " CURSOR FOR ";
    sql += query;

    pgStatus = mConn->PgExecuteCommand(sql.c_str());
    assert(PGRES_COMMAND_OK == pgStatus);

    mIsClosed = false;
}

void PgCursor::Close()
{
    FDOLOG_MARKER("PgCursor::+Close");

    if (!mIsClosed)
    {
        Validate();
        ClearFetchResult();

        ExecStatusType pgStatus = PGRES_FATAL_ERROR;

        // Close cursor
        std::string sql("CLOSE " + mName);
        pgStatus = mConn->PgExecuteCommand(sql.c_str());
        assert(PGRES_COMMAND_OK == pgStatus);

        // End transaction
        pgStatus = mConn->PgExecuteCommand("COMMIT");
        assert(PGRES_COMMAND_OK == pgStatus);

        // Mark cursor as released
        mIsClosed = true;
    }
}

PGresult const* PgCursor::FetchNext()
{
    assert(false == mIsClosed);

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
    FDOLOG_MARKER("PgCursor::Validate");

    if (FdoConnectionState_Closed == mConn->GetConnectionState())
    {
        FDOLOG_WRITE("Connection is closed or invalid.");

        throw FdoException::Create(NlsMsgGet(MSG_POSTGIS_CONNECTION_INVALID,
            "Connection is closed or invalid."));
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
