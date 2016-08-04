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
#include "FdoRdbmsPostGisDeleteDataStore.h"
#include "../../Fdo/Connection/FdoRdbmsConnectionPropertyDictionary.h"
#include "../../Fdo/DataStore/FdoRdbmsDeleteDataStore.h"
#include "../../Fdo/FeatureCommands/FdoRdbmsCommand.h"

FdoRdbmsPostGisDeleteDataStore::FdoRdbmsPostGisDeleteDataStore()
{
    // idle
}

FdoRdbmsPostGisDeleteDataStore::FdoRdbmsPostGisDeleteDataStore(
    FdoRdbmsPostGisConnection* connection)
{
    mConnection = connection;

    mDataStorePropertyDictionary = 
        static_cast<FdoCommonDataStorePropDictionary*>(
        mConnection->CreateDataStoreProperties(
            FDO_RDBMS_DATASTORE_FOR_DELETE));
}

FdoIDataStorePropertyDictionary* FdoRdbmsPostGisDeleteDataStore::GetDataStoreProperties()
{
    FdoIDataStorePropertyDictionary* dict = NULL;

    dict = static_cast<FdoIDataStorePropertyDictionary*>(
        FDO_SAFE_ADDREF(mDataStorePropertyDictionary.p));

    return dict;
}

void FdoRdbmsPostGisDeleteDataStore::Execute() 
{
    if (NULL == mConnection)
    {
        throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_13,
            "Connection not established"));
    }

    // Password is not required.
    // Service is empty string.
    FdoString* dsName = mDataStorePropertyDictionary->GetProperty(
        FDO_RDBMS_CONNECTION_DATASTORE);

    mConnection->DeleteDb(dsName, L"", L"");
} 
