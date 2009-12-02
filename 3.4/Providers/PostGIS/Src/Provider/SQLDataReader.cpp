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
#include <cstdio> // scanf()
#include <sstream>
#include <string>

namespace fdo { namespace postgis {

SQLDataReader::SQLDataReader()
    : mCurrentTuple(0)
{
	assert(false);
}

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
    // TODO - mloskot: Consider refactoring to make this function less bloated
    // and reuse some variables.

    try
    {
        PgCursor::ResultPtr pgRes = mCursor->GetFetchResult();
        FdoInt32 const fnumber = static_cast<int>(mCursor->GetFieldNumber(columnName));
        std::string sval(PQgetvalue(pgRes, static_cast<int>(mCurrentTuple), fnumber));
        if (sval.empty())
        {
            return FdoDateTime();
        }
        try
        {
            // NOTE: We expect the default SQL date and time output defined in ISO 8601.
            //       http://en.wikipedia.org/wiki/ISO_8601

            //
            // Detect datetime, date or time data
            //
            FdoDateTime fdt;

            if (std::string::npos != sval.find_first_of('-')
                && std::string::npos != sval.find_first_of(':'))
            {
                // Accepted format is:
                // [YYYY]-[MM]-[DD] [hh]:[mm]:[ss]
                // [YYYY]-[MM]-[DD]T[hh]:[mm]:[ss]±[hh]:[mm]
                // for tolerate T use *[Tt ]

                int y, m, d, hh, mm, ss;
                y = m = d = hh = mm = ss = 0;
                int const count = std::sscanf(sval.c_str(),
                    "%4d-%02d-%02d %02d:%02d:%02d", &y, &m, &d, &hh, &mm, &ss);
                assert(6 == count);

                fdt.year = static_cast<FdoInt16>(y);
                fdt.month = static_cast<FdoInt8>(m);
                fdt.day = static_cast<FdoInt8>(d);
                fdt.hour = static_cast<FdoInt8>(hh);
                fdt.minute = static_cast<FdoInt8>(mm);
                fdt.seconds = static_cast<FdoFloat>(ss);

                assert(fdt.IsDateTime());
            }
            else if (std::string::npos != sval.find_first_of('-') && sval.size() <= 12)
            {
                // Accepted format is:
                // [YYYY]-[MM]-[DD]
                int y, m, d;
                y = m = d = 0;
                int const count = std::sscanf(sval.c_str(), "%4d-%02d-%02d", &y, &m, &d);
                assert(3 == count);

                fdt.year = static_cast<FdoInt16>(y);
                fdt.month = static_cast<FdoInt8>(m);
                fdt.day = static_cast<FdoInt8>(d);                
                assert(fdt.IsDate());
            }
            else if (std::string::npos != sval.find_first_of(':'))
            {
                // Accepted format is:
                // [hh]:[mm]:[ss]
                int y, m, d, hh, mm, ss;
                y = m = d = hh = mm = ss = 0;
                int const count = std::sscanf(sval.c_str(),
                    "%02d:%02d:%02d", &hh, &mm, &ss);
                assert(3 == count);

                fdt.hour = static_cast<FdoInt8>(hh);
                fdt.minute = static_cast<FdoInt8>(mm);
                fdt.seconds = static_cast<FdoFloat>(ss);
                assert(fdt.IsTime());
            }
            else
            {
                assert(!"NEVER SHOULD GET HERE");
            }

            return fdt;
        }
        catch (std::exception& e)
        {
            FDOLOG_WRITE("ERROR: Date '%' conversion failed: %s", sval.c_str(), e.what());
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
    //FDOLOG_MARKER("SQLDataReader::GetGeometry");
    //FDOLOG_WRITE(L"Column: %s", columnName);

    // TODO: These 3 exceptions are here temporarily

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
        if (NULL == fdoGeom)
        {
            FDOLOG_WRITE("ERROR - FDO PostGIS - ewkb::CreateGeometryFromExtendedWkb returned NULL");
            throw FdoCommandException::Create(L"ERROR - FDO PostGIS - ewkb::CreateGeometryFromExtendedWkb returned NULL");
        }

        FdoPtr<FdoFgfGeometryFactory> factory(FdoFgfGeometryFactory::GetInstance());
        if (NULL == factory)
        {
            FDOLOG_WRITE("ERROR - FDO PostGIS - Geometry factory is NULL");
            throw FdoCommandException::Create(L"ERROR - FDO PostGIS - Geometry factory is NULL");
        }

        FdoPtr<FdoByteArray> fgfBytes = factory->GetFgf(fdoGeom);
        if (NULL == fgfBytes)
        {
            FDOLOG_WRITE("ERROR - FDO PostGIS - Geometry to FGF conversion failed!");
            throw FdoCommandException::Create(L"ERROR - FDO PostGIS - Geometry to FGF conversion failed");
        }
        
        //FDOLOG_WRITE(L"Geometry WKT:\n%s", fdoGeom->GetText());

        FDO_SAFE_ADDREF(fgfBytes.p);
        return fgfBytes.p;
    }
    catch (FdoException* e)
    {
        FDOLOG_WRITE("ERROR: Geometry retrival failed");

        FdoCommandException* ne = NULL;
        ne = FdoCommandException::Create(L"Geometry", e);
        e->Release();
        throw ne;
    }
}

bool SQLDataReader::ReadNext()
{
    bool eof = true;

    try
    {
      FdoSize nbTupples = mCursor->GetTuplesCount();
      //if(nbTupples == 0) return eof;
      if ((mCurrentTuple + 1 )>= nbTupples)
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
    }
    catch (FdoException* e)
    {
       FDOLOG_WRITE("ERROR: SQLDataReader::ReadNext");
       e->Release();
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
