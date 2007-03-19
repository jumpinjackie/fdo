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
#include "PgGeometry.h"
// std
#include <cassert>
#include <string>
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
    //FDOLOG_MARKER("SQLDataReader::#Dispose");

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

FdoBoolean SQLDataReader::GetBoolean(FdoString* columnName)
{
    try
    {
        FdoInt32 const fnumber = static_cast<int>(mCursor->GetFieldNumber(columnName));
        PgCursor::ResultPtr pgRes = mCursor->GetFetchResult();

        FdoBoolean val = false;
        
        std::string sval(PQgetvalue(pgRes, static_cast<int>(mCurrentTuple), fnumber));
        assert(1 == sval.size());

        if (sval == "t")
            val = true;

        return val;
    }
    catch (FdoException* e)
    {
        FdoCommandException* ne = NULL;
        ne = FdoCommandException::Create(L"GetBoolean", e);
        e->Release();
        throw ne;
    }
}

FdoByte SQLDataReader::GetByte(FdoString* columnName)
{
    try
    {
        return GetValue<FdoByte>(columnName);
    }
    catch (FdoException* e)
    {
        FdoCommandException* ne = NULL;
        ne = FdoCommandException::Create(L"GetBoolean", e);
        e->Release();
        throw ne;
    }
}

FdoInt16 SQLDataReader::GetInt16(FdoString* columnName)
{
    try
    {
        return GetValue<FdoInt16>(columnName);      
    }
    catch (FdoException* e)
    {
        FdoCommandException* ne = NULL;
        ne = FdoCommandException::Create(L"Int32", e);
        e->Release();
        throw ne;
    }
}

FdoInt32 SQLDataReader::GetInt32(FdoString* columnName)
{
    try
    {
        return GetValue<FdoInt32>(columnName);      
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
    try
    {
        return GetValue<FdoInt64>(columnName);      
    }
    catch (FdoException* e)
    {
        FdoCommandException* ne = NULL;
        ne = FdoCommandException::Create(L"Int32", e);
        e->Release();
        throw ne;
    }
}

FdoFloat SQLDataReader::GetSingle(FdoString* columnName)
{
    try
    {
        return GetValue<FdoFloat>(columnName);      
    }
    catch (FdoException* e)
    {
        FdoCommandException* ne = NULL;
        ne = FdoCommandException::Create(L"Int32", e);
        e->Release();
        throw ne;
    }
}

FdoDouble SQLDataReader::GetDouble(FdoString* columnName)
{
    try
    {
        return GetValue<FdoDouble>(columnName);      
    }
    catch (FdoException* e)
    {
        FdoCommandException* ne = NULL;
        ne = FdoCommandException::Create(L"Int32", e);
        e->Release();
        throw ne;
    }
}

FdoString* SQLDataReader::GetString(FdoString* columnName)
{
    try
    {
        FdoInt32 const fnumber = static_cast<int>(mCursor->GetFieldNumber(columnName));
        PgCursor::ResultPtr pgRes = mCursor->GetFetchResult();

        // Read string value and store it in cache buffer
        std::string sval(PQgetvalue(pgRes, static_cast<int>(mCurrentTuple), fnumber));
        mCacheString = sval.c_str();

        return mCacheString;
    }
    catch (FdoException* e)
    {
        FdoCommandException* ne = NULL;
        ne = FdoCommandException::Create(L"Int32", e);
        e->Release();
        throw ne;
    }
}

FdoDateTime SQLDataReader::GetDateTime(FdoString* columnName)
{
    assert(!"NOT IMPLEMENTED");
    return FdoDateTime();
}

FdoLOBValue* SQLDataReader::GetLOB(FdoString* columnName)
{
    assert(!"NOT IMPLEMENTED");
    return 0;
}

FdoIStreamReader* SQLDataReader::GetLOBStreamReader(FdoString* columnName)
{
    assert(!"NOT IMPLEMENTED");
    return 0;
}

FdoBoolean SQLDataReader::IsNull(FdoString* columnName)
{
    FdoInt32 const fnumber = static_cast<int>(mCursor->GetFieldNumber(columnName));
    PgCursor::ResultPtr pgRes = mCursor->GetFetchResult();

    // Returns 1 if the field is null and 0 if it contains a non-null value.
    // The PQgetvalue will return an empty string, not a null pointer, for a null field.

    int const isNull = PQgetisnull(pgRes, static_cast<int>(mCurrentTuple), fnumber);

    return (1 == isNull);
}

FdoByteArray* SQLDataReader::GetGeometry(FdoString* columnName)
{
    // TODO: Consider best strategy to handle NULL geometries
    //       It will also require some minor changes in EWKB parser.

    try
    {
        FdoInt32 const fnumber = static_cast<int>(mCursor->GetFieldNumber(columnName));
        PgCursor::ResultPtr pgRes = mCursor->GetFetchResult();

        // Read string value and store it in cache buffer
        std::string hexstring(PQgetvalue(pgRes, static_cast<int>(mCurrentTuple), fnumber));
        
        ewkb::ewkb_t ewkbData;
        ewkb::hex_to_bytes(hexstring, ewkbData);

        FdoPtr<FdoIGeometry> fdoGeom = ewkb::CreateGeometryFromExtendedWkb(ewkbData);
        assert(NULL != fdoGeom);

        FdoPtr<FdoFgfGeometryFactory> factory(FdoFgfGeometryFactory::GetInstance());
        assert(NULL != factory);

        FdoPtr<FdoByteArray> fgfBytes = factory->GetFgf(fdoGeom);
        assert(NULL != fgfBytes);
        
        FDO_SAFE_ADDREF(fgfBytes.p);
        return fgfBytes.p;
    }
    catch (FdoException* e)
    {
        FdoCommandException* ne = NULL;
        ne = FdoCommandException::Create(L"Int32", e);
        e->Release();
        throw ne;
    }
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
    // TODO: Temporary solution. Make ownership transfer exception safe.

    if (NULL != mCursor)
        mCursor->Close();
}

}} // namespace fdo::postgis
