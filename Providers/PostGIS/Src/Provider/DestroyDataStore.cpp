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
#include "DestroyDataStore.h"
#include "Connection.h"

#include <cassert>

namespace fdo { namespace postgis {

DestroyDataStore::DestroyDataStore(Connection* conn) : Base(conn)
{
    FDOLOG_WRITE("DestroyDataStore command created");
}

DestroyDataStore::~DestroyDataStore()
{
}

///////////////////////////////////////////////////////////////////////////////
// FdoIDestroyDataStore interface
///////////////////////////////////////////////////////////////////////////////

FdoIDataStorePropertyDictionary* DestroyDataStore::GetDataStoreProperties()
{
    if (NULL == mProps)
    {
        // TODO: Connection passed as a weak reference?
        mProps = new FdoCommonDataStorePropDictionary(mConn);

        // Datastore: isRequired
        FdoPtr<ConnectionProperty> prop(new ConnectionProperty(PropertyDatastoreName,
            NlsMsgGet(MSG_POSTGIS_PROPERTY_DATASTORE_NAME, "DataStore"),
            L"", true, false, false, false, false, true, false, 0, NULL));

        mProps->AddProperty(prop);
    }

    FDO_SAFE_ADDREF(mProps.p);
    return mProps.p;
}

void DestroyDataStore::Execute()
{
    FDOLOG_MARKER("DestroyDataStore::+Execute");

    ValidateRequiredProperties();

    FdoStringP dsName(mProps->GetProperty(PropertyDatastoreName));
    assert(dsName.GetLength() > 0);

    // TODO: Should we drop in CASCADE mode or leave it as RESTRICT mode?

    // NOTE: Don't use IF EXISTS test, because we want to get an error if a schema exists
    std::string sql("DROP SCHEMA ");
    sql += static_cast<char const*>(dsName);

    //
    // Destroy specified FDO datastore - drop a PostgreSQL schema
    //
    try
    {
        mConn->PgExecuteCommand(sql.c_str());
    }
    catch (FdoException* e)
    {
        FdoCommandException* ne = NULL;
        ne = FdoCommandException::Create(
                NlsMsgGet(MSG_POSTGIS_COMMAND_DESTROYDATASTORE_FAILED,
                    "Attempt to destroy datastore '%1$ls' failed.",
                    static_cast<FdoString*>(dsName)), e);
        e->Release();
        throw ne;
    }

    // NOTE: No need to remove a datastore description, because:
    // Comments are automatically dropped when the object is dropped.
}

///////////////////////////////////////////////////////////////////////////////
// Private operations
///////////////////////////////////////////////////////////////////////////////

void DestroyDataStore::ValidateRequiredProperties() const
{
    assert(NULL != mProps);

    FdoPtr<ConnectionProperty> prop(mProps->FindProperty(PropertyDatastoreName));
    if( prop && prop->GetIsPropertyRequired())
    {
        FdoStringP dsName(prop->GetValue());
        if (dsName.GetLength() <= 0)
        {
            throw FdoCommandException::Create(
                NlsMsgGet(MSG_POSTGIS_COMMAND_MISSING_REQUIRED_PROPERTY,
                "The command property '%1$ls' is required but wasn't specified.",
                PropertyDatastoreName));
        }
    }
}

}} // namespace fdo::postgis

