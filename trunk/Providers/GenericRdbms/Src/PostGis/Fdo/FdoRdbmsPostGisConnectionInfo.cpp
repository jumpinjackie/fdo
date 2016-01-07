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
#include "FdoRdbmsPostGisConnectionInfo.h"
#include "FdoRdbmsPostGisProviderInfo.h"
#include <Inc/Nls/rdbi_msg.h>

FdoRdbmsPostGisConnectionInfo::FdoRdbmsPostGisConnectionInfo(
    FdoRdbmsConnection *connection)
    : mConnection(connection)
{
    // idle
}

FdoRdbmsPostGisConnectionInfo::~FdoRdbmsPostGisConnectionInfo()
{
    // idle
}

void FdoRdbmsPostGisConnectionInfo::Dispose()
{
    delete this;
}

const wchar_t* FdoRdbmsPostGisConnectionInfo::GetProviderName()
{
    return RDBMS_POSTGIS_PROVIDER_NAME;
}

const wchar_t* FdoRdbmsPostGisConnectionInfo::GetProviderDisplayName()
{
    return NlsMsgGet(FDORDBMS_472,
        RDBMS_POSTGIS_PROVIDER_DEFAULT_DISPLAY_NAME);
}

const wchar_t* FdoRdbmsPostGisConnectionInfo::GetProviderDescription()
{
    return NlsMsgGet(FDORDBMS_473,
        RDBMS_POSTGIS_PROVIDER_DEFAULT_DESCRIPTION);
}

const wchar_t* FdoRdbmsPostGisConnectionInfo::GetProviderVersion()
{
    return RDBMS_POSTGIS_PROVIDER_VERSION;
}

const wchar_t* FdoRdbmsPostGisConnectionInfo::GetFeatureDataObjectsVersion()
{
    return RDBMS_POSTGIS_FDO_VERSION;
}

FdoIConnectionPropertyDictionary* FdoRdbmsPostGisConnectionInfo::GetConnectionProperties()
{
    if (NULL == mPropertyDictionary)
    {
        mPropertyDictionary =
            new FdoRdbmsConnectionPropertyDictionary(
                static_cast<FdoIConnection*>(mConnection));
        
        FdoPtr<ConnectionProperty> pProp;

        pProp = new ConnectionProperty(FDO_RDBMS_CONNECTION_USERNAME,
                NlsMsgGet(FDORDBMS_118, "Username"), L"",
                true, false, false, false, false, false, false, 0, NULL); 
        mPropertyDictionary->AddProperty(pProp);

        pProp = new ConnectionProperty(FDO_RDBMS_CONNECTION_PASSWORD,
                NlsMsgGet(FDORDBMS_119, "Password"), L"",
                true, true, false, false, false, false, false, 0, NULL); 
        mPropertyDictionary->AddProperty(pProp);

        pProp = new ConnectionProperty (FDO_RDBMS_CONNECTION_SERVICE, 
                NlsMsgGet(FDORDBMS_120, "Service"), L"", 
                true, false, false, false, false, false, false, 0, NULL); 
        mPropertyDictionary->AddProperty(pProp);

        pProp = new ConnectionProperty(FDO_RDBMS_CONNECTION_DATASTORE,
                NlsMsgGet(FDORDBMS_117, "DataStore"), L"",
                false, false, true, false, false, true, false, 0, NULL); 
        mPropertyDictionary->AddProperty(pProp);
    }

    FDO_SAFE_ADDREF(mPropertyDictionary.p);
    return mPropertyDictionary;
}

FdoProviderDatastoreType FdoRdbmsPostGisConnectionInfo::GetProviderDatastoreType()
{
    return FdoProviderDatastoreType_DatabaseServer;
}

FdoStringCollection* FdoRdbmsPostGisConnectionInfo::GetDependentFileNames()
{
    return NULL;
}
