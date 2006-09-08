/*
 * Copyright (C) 2004-2006  Autodesk, Inc.
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
#include "FdoRdbmsMySqlConnectionInfo.h"
#include "FdoRdbmsMySqlProviderInfo.h"

FdoRdbmsMySqlConnectionInfo::FdoRdbmsMySqlConnectionInfo(FdoRdbmsConnection *connection) :
mPropertyDictionary(NULL),
mConnection(connection)
{

}

FdoRdbmsMySqlConnectionInfo::~FdoRdbmsMySqlConnectionInfo()
{
}

void FdoRdbmsMySqlConnectionInfo::Dispose()
{
    delete this;
}

const wchar_t* FdoRdbmsMySqlConnectionInfo::GetProviderName()
{
    return RDBMS_MYSQL_PROVIDER_NAME;
}

const wchar_t* FdoRdbmsMySqlConnectionInfo::GetProviderDisplayName()
{
    return NlsMsgGet(FDORDBMS_472, RDBMS_MYSQL_PROVIDER_DEFAULT_DISPLAY_NAME);
}

const wchar_t* FdoRdbmsMySqlConnectionInfo::GetProviderDescription()
{
    return NlsMsgGet(FDORDBMS_473, RDBMS_MYSQL_PROVIDER_DEFAULT_DESCRIPTION);
}

const wchar_t* FdoRdbmsMySqlConnectionInfo::GetProviderVersion()
{
    return RDBMS_MYSQL_PROVIDER_VERSION;
}


const wchar_t* FdoRdbmsMySqlConnectionInfo::GetFeatureDataObjectsVersion()
{
    return RDBMS_MYSQL_FDO_VERSION;
}

FdoIConnectionPropertyDictionary* FdoRdbmsMySqlConnectionInfo::GetConnectionProperties()
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
    }
    FDO_SAFE_ADDREF(mPropertyDictionary.p);
    return mPropertyDictionary;
}
