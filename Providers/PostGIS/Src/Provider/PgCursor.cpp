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
    : mConn(conn), mName(name.c_str()), mDescRes(NULL), mFetchRes(NULL), mIsClosed(true)
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
    return (static_cast<char const*>(mName));
}

PgCursor::ResultPtr PgCursor::GetFetchResult() const
{
    // TODO: Handling of nothing fetched case.
    assert(NULL != mFetchRes);

    return mFetchRes;
}

FdoSize PgCursor::GetFieldsCount() const
{
    assert(NULL != mDescRes);
    int nfields = PQnfields(mDescRes);
    return static_cast<FdoSize>(nfields);
}

FdoStringP PgCursor::GetFieldName(FdoSize number) const
{
    if (GetFieldsCount() >= number)
    {
        // TODO: Throw about column index out of range
        assert(false);
    }

    char const* fname = PQfname(mDescRes, static_cast<int>(number));
    assert(NULL != fname);

    return FdoStringP(fname);
}

FdoSize PgCursor::GetFieldNumber(FdoStringP const& name) const
{
    int fnumber = PQfnumber(mDescRes, static_cast<const char*>(name));
    if (-1 == fnumber)
    {
        // -1 is returned if the given name does not match any column.
        // TODO: throw an exception
        assert(false);
    }

    return fnumber;
}

FdoDataType PgCursor::GetFieldType(FdoStringP const& name) const
{
    FdoSize fnumber = GetFieldNumber(name);
    Oid ftype = PQftype(mDescRes, static_cast<int>(fnumber));

    assert(!"NOT FINISHED YET");

    return FdoDataType_Int32;
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

    try
    {
        // Begin transaction
        mConn->PgExecuteCommand("BEGIN");

        // Declare cursor
        std::string sql("DECLARE ");
        sql += static_cast<char const*>(mName);
        sql += " CURSOR FOR ";
        sql += query;

        mConn->PgExecuteCommand(sql.c_str());

        // Call for describe cursor results
        Describe();
    }
    catch (FdoException* e)
    {
        throw FdoCommandException::Create(NlsMsgGet(MSG_POSTGIS_CURSOR_CREATION_FAILED,
            "The creation of PostgreSQL cursor '%1$ls' failed.",
            static_cast<FdoString*>(mName)), e);  
    }

    mIsClosed = false;
}

void PgCursor::Close()
{
    FDOLOG_MARKER("PgCursor::+Close");

    if (!mIsClosed)
    {
        Validate();
        ClearDescribeResult();
        ClearFetchResult();

        // Close cursor
        std::string sql("CLOSE ");
        sql += static_cast<char const*>(mName);

        mConn->PgExecuteCommand(sql.c_str());
        
        // End transaction
        mConn->PgExecuteCommand("COMMIT");
        
        // Mark cursor as released
        mIsClosed = true;
    }
}

PgCursor::ResultPtr  PgCursor::FetchNext()
{
    assert(false == mIsClosed);

    ClearFetchResult();
    assert(NULL == mFetchRes);

    std::string sql("FETCH NEXT FROM ");
    sql += static_cast<char const*>(mName);

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

void PgCursor::ClearDescribeResult()
{
    if (NULL != mDescRes)
    {
        PQclear(mDescRes);
        mDescRes = NULL;
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


void PgCursor::Describe()
{
    Validate();

    if (NULL == mDescRes)
    {
        mDescRes = mConn->PgDescribeCursor(static_cast<char const*>(mName));
    }
}

}} // namespace fdo::postgis
