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
#include "DataStoreReader.h"
#include "Connection.h"
#include "PgCursor.h"
// std
#include <cassert>

namespace fdo { namespace postgis {

DataStoreReader::DataStoreReader(PgCursor* cursor)
    : mCursor(cursor)
{
    assert(NULL != cursor);

    FDO_SAFE_ADDREF(mCursor.p);
    
    FDOLOG_WRITE("DataStoreReader created");
}

DataStoreReader::~DataStoreReader()
{
}

///////////////////////////////////////////////////////////////////////////////
// FdoIDisposable interface
///////////////////////////////////////////////////////////////////////////////

void DataStoreReader::Dispose()
{
    Close();
    delete this;
}


///////////////////////////////////////////////////////////////////////////////
// FdoIDataStoreReader interface
///////////////////////////////////////////////////////////////////////////////

FdoString* DataStoreReader::GetName()
{   
    ValidateReadableState();

    PGresult const* pgRes = mCursor->GetFetchResult();
    assert(PGRES_TUPLES_OK == PQresultStatus(pgRes) && 0 != PQntuples(pgRes));

    int const fn = PQfnumber(pgRes, "schemaname");
    assert(-1 != fn);

    std::string val(PQgetvalue(pgRes, 0, fn));
    mDsName = val.c_str();

    return mDsName;
}

FdoString* DataStoreReader::GetDescription()
{
    ValidateReadableState();

    PGresult const* pgRes = mCursor->GetFetchResult();
    assert(PGRES_TUPLES_OK == PQresultStatus(pgRes) && 0 != PQntuples(pgRes));

    int const fn = PQfnumber(pgRes, "description");
    assert(-1 != fn);

    std::string val(PQgetvalue(pgRes, 0, fn));
    mDsDesc = val.c_str();

    return mDsDesc;
}

bool DataStoreReader::GetIsFdoEnabled()
{
    ValidateReadableState();
    return false;
}

FdoIDataStorePropertyDictionary* DataStoreReader::GetDataStoreProperties()
{
    ValidateReadableState();

    // TODO: To be implemented
    // Create a provider specific datastore dictionary property.
    //if (NULL != mDsProps)
    //{
    //    mDsProps = mConn->CreateDataStoreProperties( FDO_RDBMS_DATASTORE_FOR_READ );
    //}
    //FDO_SAFE_ADDREF(mDsProps.p);

    return NULL;
}

bool DataStoreReader::ReadNext()
{
    // TODO: Add fetching tuples in batches, ie. per 50 or 100

    bool eof = true;

    PGresult const* pgRes = mCursor->FetchNext();
    if (PGRES_TUPLES_OK == PQresultStatus(pgRes))
    {
        if (0 != PQntuples(pgRes))
        {
            ValidateReadableState();
            eof = false;
        }
    }

    return (!eof);
}

void DataStoreReader::Close()
{
    // TODO: Temporary solution. Make ownership transfer exception safe.

    if (NULL != mCursor)
        mCursor->Close();
}

///////////////////////////////////////////////////////////////////////////////
// Private operations
///////////////////////////////////////////////////////////////////////////////

void DataStoreReader::ValidateReadableState()
{
    // TODO: Replace assertions to exceptions.

    PGresult const* pgRes = mCursor->GetFetchResult();
    assert(PGRES_TUPLES_OK == PQresultStatus(pgRes));
    assert(0 != PQntuples(pgRes));
}

}} // namespace fdo::postgis
