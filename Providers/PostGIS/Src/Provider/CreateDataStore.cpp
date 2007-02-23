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
#include "CreateDataStore.h"
#include "Connection.h"

#include <cassert>

namespace fdo { namespace postgis {

CreateDataStore::CreateDataStore(Connection* conn) : Base(conn), mProps(NULL)
{
}

CreateDataStore::~CreateDataStore()
{
}

///////////////////////////////////////////////////////////////////////////////
// FdoICreateDataStore interface
///////////////////////////////////////////////////////////////////////////////

FdoIDataStorePropertyDictionary* CreateDataStore::GetDataStoreProperties()
{
    if (NULL == mProps)
    {
        FdoPtr<ConnectionProperty> prop;

        // TODO: Connection passed as a weak reference?
        mProps = new FdoCommonDataStorePropDictionary(mConn);

        // Datastore: isRequired
        prop = new ConnectionProperty(PropertyDatastoreName,
            NlsMsgGet(MSG_POSTGIS_PROPERTY_DATASTORE_NAME, "DataStore"),
            L"", true, false, false, false, false, true, false, 0, NULL);
        mProps->AddProperty(prop);

        // Description
        prop = new ConnectionProperty(PropertyDatastoreDescription,
            NlsMsgGet(MSG_POSTGIS_PROPERTY_DATASTORE_DESCRIPTION, "Description"),
            L"", false, false, false, false, false, false, false, 0, NULL);
        mProps->AddProperty(prop);
    }

    FDO_SAFE_ADDREF(mProps.p);
    return mProps.p;
}

void CreateDataStore::Execute()
{
    FDOLOG_MARKER("CreateDataStore::+Execute");

    ValidateRequiredProperties();

    //
    // Create new schema - FDO datastore
    //
    FdoStringP dsName(mProps->GetProperty(PropertyDatastoreName));
    assert(dsName.GetLength() > 0);
    
    std::string sql("CREATE SCHEMA ");
    sql += static_cast<char const*>(dsName);

    try
    {
        mConn->PgExecuteCommand(sql.c_str());
    }
    catch (FdoException* e)
    {
        FdoCommandException* ne = NULL;
        ne = FdoCommandException::Create(
            NlsMsgGet(MSG_POSTGIS_COMMAND_CREATEDATASTORE_FAILED,
                "Attempt to create new datastore with name '%1$ls' failed.",
                static_cast<FdoString*>(dsName)), e);
        e->Release();
        throw ne;
    }

    //
    // Insert datastore description as a comment for PostgreSQL schema
    //

    try
    {
        FdoStringP dsDescription(mProps->GetProperty(PropertyDatastoreDescription));
        if (dsDescription.GetLength() > 0)
        {
            sql = "COMMENT ON SCHEMA ";
            sql += static_cast<char const*>(dsName);
            sql += " IS \'";
            sql += static_cast<char const*>(dsDescription);
            sql += "\'";

            mConn->PgExecuteCommand(sql.c_str());
        }
    }
    catch (FdoException* e)
    {
        FdoCommandException* ne = NULL;
        ne = FdoCommandException::Create(NlsMsgGet(MSG_POSTGIS_COMMAND_COMMENT_FAILED,
                "Attempt to assign description for '%1$ls' object failed.",
                static_cast<FdoString*>(dsName)), e);
        e->Release();
        throw ne;
    }
}

///////////////////////////////////////////////////////////////////////////////
// Private operations
///////////////////////////////////////////////////////////////////////////////

void CreateDataStore::ValidateRequiredProperties() const
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
