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
#include "SQLDataReader.h"
#include "Connection.h"
#include "PgCursor.h"
// std
#include <cassert>
// boost
#include <boost/lexical_cast.hpp>

namespace fdo { namespace postgis {

SQLDataReader::SQLDataReader(PgCursor* cursor)
    : mCursor(cursor), mCurrentTuple(0)
{
    assert(NULL != cursor);
    
    FDO_SAFE_ADDREF(mCursor.p);
}

SQLDataReader::~SQLDataReader()
{
}

///////////////////////////////////////////////////////////////////////////////
// FdoIDisposable interface
///////////////////////////////////////////////////////////////////////////////

void SQLDataReader::Dispose()
{
    FDOLOG_MARKER("SQLDataReader::#Dispose");

    Close();
    delete this;
}

///////////////////////////////////////////////////////////////////////////////
// FdoISQLDataReader interface
///////////////////////////////////////////////////////////////////////////////

FdoInt32 SQLDataReader::GetColumnCount()
{
    FdoSize const nfields = mCursor->GetFieldsCount();
    return static_cast<FdoInt32>(nfields);
}

FdoString* SQLDataReader::GetColumnName(FdoInt32 index)
{
    // Returning raw pointer requires buffered value
    mColumnName = mCursor->GetFieldName(static_cast<FdoSize>(index));
    return static_cast<FdoString*>(mColumnName);
}

FdoDataType SQLDataReader::GetColumnType(FdoString* columnName)
{
    return mCursor->GetFieldType(columnName);
}

FdoPropertyType SQLDataReader::GetPropertyType(FdoString* columnName)
{
    FdoSize fnumber = mCursor->GetFieldNumber(columnName);
    if (mCursor->IsFieldGeometryType(fnumber))
    {
        return FdoPropertyType_GeometricProperty;
    }
    
    return FdoPropertyType_DataProperty;
}

bool SQLDataReader::GetBoolean(FdoString* columnName)
{
    assert(!"NOT IMPLEMENTED");
    return 0;
}

FdoByte SQLDataReader::GetByte(FdoString* columnName)
{
    assert(!"NOT IMPLEMENTED");
    return 0;
}

FdoDateTime SQLDataReader::GetDateTime(FdoString* columnName)
{
    assert(!"NOT IMPLEMENTED");
    return FdoDateTime();
}

double SQLDataReader::GetDouble(FdoString* columnName)
{
    assert(!"NOT IMPLEMENTED");
    return 0;
}

FdoInt16 SQLDataReader::GetInt16(FdoString* columnName)
{
    assert(!"NOT IMPLEMENTED");
    return 0;
}

FdoInt32 SQLDataReader::GetInt32(FdoString* columnName)
{
    try
    {
        FdoSize const fnumber = mCursor->GetFieldNumber(columnName);
        PgCursor::ResultPtr pgRes = mCursor->GetFetchResult();

        FdoInt32 val = 0;
        try
        {
            char const* cval = PQgetvalue(pgRes, mCurrentTuple, fnumber);
            val = boost::lexical_cast<FdoInt32>(cval);
        }
        catch (boost::bad_lexical_cast& e)
        {
            e;
            throw FdoCommandException::Create(L"Field value conversion failed.");
        }

        return val;
    }
    catch (FdoException* e)
    {
        FdoCommandException* ne = NULL;
        ne = FdoCommandException::Create(L"Int32", e);
        e->Release();
        throw ne;
    }
}

FdoInt64 SQLDataReader::GetInt64(FdoString* columnName)
{
    assert(!"NOT IMPLEMENTED");
    return 0;
}

float SQLDataReader::GetSingle(FdoString* columnName)
{
    assert(!"NOT IMPLEMENTED");
    return 0;
}

FdoString* SQLDataReader::GetString(FdoString* columnName)
{
    assert(!"NOT IMPLEMENTED");
    return 0;
}

FdoLOBValue* SQLDataReader::GetLOB(FdoString* columnName)
{
    assert(!"NOT IMPLEMENTED");
    return 0;
}

FdoIStreamReader* SQLDataReader::GetLOBStreamReader(wchar_t const* columnName)
{
    assert(!"NOT IMPLEMENTED");
    return 0;
}

bool SQLDataReader::IsNull(FdoString* columnName)
{
    FdoSize const fnumber = mCursor->GetFieldNumber(columnName);
    PgCursor::ResultPtr pgRes = mCursor->GetFetchResult();

    // Returns 1 if the field is null and 0 if it contains a non-null value.
    // The PQgetvalue will return an empty string, not a null pointer, for a null field.

    int const isNull = PQgetisnull(pgRes, mCurrentTuple, fnumber);

    return (1 == isNull);
}

FdoByteArray* SQLDataReader::GetGeometry(FdoString* columnName)
{
    // TODO: How to detect and distinguish following cases:
    //1. SELECT geom FROM table
    //2. SELECT AsBinary(geom) FROM table


    assert(!"NOT IMPLEMENTED");
    return 0;
}

bool SQLDataReader::ReadNext()
{
    // TODO: Add fetching tuples in batches, ie. per 50 or 100

    bool eof = true;

    PgCursor::ResultPtr pgRes = mCursor->FetchNext();
    if (PGRES_TUPLES_OK == PQresultStatus(pgRes))
    {
        if (0 != PQntuples(pgRes))
        {
            eof = false;
        }
    }

    return (!eof);
}

void SQLDataReader::Close()
{
    FDOLOG_MARKER("SQLDataReader::+Close");

    // TODO: Temporary solution. Make ownership transfer exception safe.

    if (NULL != mCursor)
        mCursor->Close();
}

}} // namespace fdo::postgis

