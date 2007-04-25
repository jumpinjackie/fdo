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
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

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
    FDOLOG_MARKER("SQLDataReader::GetDateTime");

    try
    {
        PgCursor::ResultPtr pgRes = mCursor->GetFetchResult();
        FdoInt32 const fnumber = static_cast<int>(mCursor->GetFieldNumber(columnName));
        std::string sval(PQgetvalue(pgRes, static_cast<int>(mCurrentTuple), fnumber));

        try
        {
            // The default output format of date is ISO.
            // The SQL standard requires the use of the ISO 8601 format

            //boost::posix_time::ptime t(boost::posix_time::time_from_string(sval));
            //boost::gregorian::date d(t.date());
            //boost::posix_time::time_duration td(t.time_of_day());

            boost::gregorian::date d(boost::gregorian::from_simple_string(sval));

            FdoDateTime dt(FdoInt16(d.year()), d.month(), d.day());

                           //td.hours(), td.minutes(), td.seconds());
            return dt;
        }
        catch (std::exception& e)
        {
            FDOLOG_WRITE("Date '%' conversion failed: %s", sval.c_str(), e.what());
            throw FdoCommandException::Create(L"Date conversion failed");
        }
    }
    catch (FdoException* e)
    {
        FdoCommandException* ne = NULL;
        ne = FdoCommandException::Create(L"DateTime", e);
        e->Release();
        throw ne;
    }
}

FdoLOBValue* SQLDataReader::GetLOB(FdoString* columnName)
{
    FDOLOG_WRITE("SQLDataReader::GetLOB() - NOT IMPLEMENTED");
    assert(!"NOT IMPLEMENTED");
    return 0;
}

FdoIStreamReader* SQLDataReader::GetLOBStreamReader(FdoString* columnName)
{
    FDOLOG_WRITE("SQLDataReader::GetLOBStreamReader() - NOT IMPLEMENTED");
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
        ne = FdoCommandException::Create(L"Geometry", e);
        e->Release();
        throw ne;
    }
}

bool SQLDataReader::ReadNext()
{
    bool eof = true;

    if ((mCurrentTuple + 1 )>= mCursor->GetTuplesCount())
    {
        PgCursor::ResultPtr pgRes = mCursor->Fetch(mCoursorPageSize);
        if (PGRES_TUPLES_OK == PQresultStatus(pgRes))
        {
            mCurrentTuple = 0;

            if (0 != PQntuples(pgRes))
            {
                eof = false;
            }
        }
    }
    else
    {
        eof = false;
        mCurrentTuple++;
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
