/*
 * Copyright (C) 2004-2008  Autodesk, Inc.
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of version 2.1 of the GNU Lesser
 * General Public License as published by the Free Software Foundation.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include "stdafx.h"
#include "FdoRdbmsSqlServerConnectionInfo.h"
#include "FdoRdbmsSqlServerProviderInfo.h"

FdoRdbmsSqlServerConnectionInfo::FdoRdbmsSqlServerConnectionInfo(FdoRdbmsConnection *connection) :
mPropertyDictionary(NULL),
mConnection(connection)
{

}

FdoRdbmsSqlServerConnectionInfo::~FdoRdbmsSqlServerConnectionInfo()
{
}


const wchar_t* FdoRdbmsSqlServerConnectionInfo::GetProviderName()
{
    return RDBMS_SQLSERVER_PROVIDER_NAME;
}

const wchar_t* FdoRdbmsSqlServerConnectionInfo::GetProviderDisplayName()
{
    return NlsMsgGet(FDORDBMS_541, RDBMS_SQLSERVER_PROVIDER_DEFAULT_DISPLAY_NAME);
}

const wchar_t* FdoRdbmsSqlServerConnectionInfo::GetProviderDescription()
{
    return NlsMsgGet(FDORDBMS_542, RDBMS_SQLSERVER_PROVIDER_DEFAULT_DESCRIPTION);
}

const wchar_t* FdoRdbmsSqlServerConnectionInfo::GetProviderVersion()
{
    return RDBMS_SQLSERVER_PROVIDER_VERSION;
}


const wchar_t* FdoRdbmsSqlServerConnectionInfo::GetFeatureDataObjectsVersion()
{
    return RDBMS_SQLSERVER_FDO_VERSION;
}

FdoIConnectionPropertyDictionary* FdoRdbmsSqlServerConnectionInfo::GetConnectionProperties()
{
    if (mPropertyDictionary == NULL)
    {
        mPropertyDictionary = new FdoRdbmsConnectionPropertyDictionary((FdoIConnection*)mConnection);
        
        FdoPtr<ConnectionProperty> pProp;
        pProp = new ConnectionProperty (FDO_RDBMS_CONNECTION_USERNAME, NlsMsgGet(FDORDBMS_118, "Username"), L"", true, false, false, false, false, false, false, 0, NULL); 
        mPropertyDictionary->AddProperty(pProp);
        pProp = new ConnectionProperty (FDO_RDBMS_CONNECTION_PASSWORD, NlsMsgGet(FDORDBMS_119, "Password"), L"", true, true, false, false, false, false, false, 0, NULL); 
        mPropertyDictionary->AddProperty(pProp);
        pProp = new ConnectionProperty (FDO_RDBMS_CONNECTION_SERVICE, NlsMsgGet(FDORDBMS_120, "Service"), L"", true, false, false, false, false, false, false, 0, NULL); 
        mPropertyDictionary->AddProperty(pProp);
        pProp = new ConnectionProperty (FDO_RDBMS_CONNECTION_DATASTORE, NlsMsgGet(FDORDBMS_117, "DataStore"), L"", false, false, true, false, false, true, false, 0, NULL); 
        mPropertyDictionary->AddProperty(pProp);
        pProp = new ConnectionProperty (FDO_RDBMS_CONNECTION_CONNSTRING, NlsMsgGet(FDORDBMS_464, "ConnectionString"), L"", false, true, false, false, false, false, true, 0, NULL); 
        mPropertyDictionary->AddProperty(pProp);
    }
    FDO_SAFE_ADDREF(mPropertyDictionary.p);
    return mPropertyDictionary;
}

FdoProviderDatastoreType FdoRdbmsSqlServerConnectionInfo::GetProviderDatastoreType()
{
    return FdoProviderDatastoreType_DatabaseServer;
}

FdoStringCollection* FdoRdbmsSqlServerConnectionInfo::GetDependentFileNames()
{
    return NULL;
}

void FdoRdbmsSqlServerConnectionInfo::Dispose()
{
    delete this;
}


