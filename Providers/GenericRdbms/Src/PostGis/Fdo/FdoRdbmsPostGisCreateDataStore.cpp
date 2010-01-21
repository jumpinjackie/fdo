//
// Copyright (C) 2006 Refractions Research, Inc. 
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
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
#include "stdafx.h"
#include "FdoRdbmsPostGisConnection.h"
#include "FdoRdbmsPostGisCreateDataStore.h"
#include "../../Fdo/Connection/FdoRdbmsConnectionPropertyDictionary.h"

FdoRdbmsPostGisCreateDataStore::FdoRdbmsPostGisCreateDataStore()
{
    // idle
}

FdoRdbmsPostGisCreateDataStore::FdoRdbmsPostGisCreateDataStore(
    FdoRdbmsPostGisConnection* connection)
{
    // Assign connection
    mConnection = connection;

    // Instantiate dictionary
    mDataStorePropertyDictionary = (FdoCommonDataStorePropDictionary*)mConnection->CreateDataStoreProperties(FDO_RDBMS_DATASTORE_FOR_CREATE);
}

void FdoRdbmsPostGisCreateDataStore::Execute() 
{
    if (NULL == mConnection)
    {
        throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_13,
            "Connection not established"));
    }

    // Password is not required.
    // Service also is given as empty string

    FdoString* dsName = mDataStorePropertyDictionary->GetProperty(
        FDO_RDBMS_CONNECTION_DATASTORE);
    
    FdoString* dsDesc = mDataStorePropertyDictionary->GetProperty(
        FDO_RDBMS_DATASTORE_DESCRIPTION);

    FdoStringP fdoEnabledStr = mDataStorePropertyDictionary->GetProperty(FDO_RDBMS_DATASTORE_FDO_ENABLED);

    bool isFdoEnabled =  ( fdoEnabledStr == L"false" ) ? false : true;        

    mConnection->CreateDb(dsName, dsDesc, L"", L"", L"NONE", L"NONE", isFdoEnabled);
} 
