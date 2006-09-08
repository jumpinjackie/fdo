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
#include "FdoRdbmsOdbcConnectionInfo.h"
#include "FdoRdbmsOdbcProviderInfo.h"
#include <Sm/Ph/Rd/OwnerReader.h>
#include "FdoCommonStringUtil.h"

FdoRdbmsOdbcConnectionInfo::FdoRdbmsOdbcConnectionInfo(FdoRdbmsConnection *connection) :
mPropertyDictionary(NULL),
mConnection(connection)
{

}

FdoRdbmsOdbcConnectionInfo::~FdoRdbmsOdbcConnectionInfo()
{
}


const wchar_t* FdoRdbmsOdbcConnectionInfo::GetProviderName()
{
    return RDBMS_ODBC_PROVIDER_NAME;
}

const wchar_t* FdoRdbmsOdbcConnectionInfo::GetProviderDisplayName()
{
    return NlsMsgGet(FDORDBMS_476, RDBMS_ODBC_PROVIDER_DEFAULT_DISPLAY_NAME);
}

const wchar_t* FdoRdbmsOdbcConnectionInfo::GetProviderDescription()
{
    return NlsMsgGet(FDORDBMS_477, RDBMS_ODBC_PROVIDER_DEFAULT_DESCRIPTION);
}

const wchar_t* FdoRdbmsOdbcConnectionInfo::GetProviderVersion()
{
    return RDBMS_ODBC_PROVIDER_VERSION;
}

const wchar_t* FdoRdbmsOdbcConnectionInfo::GetFeatureDataObjectsVersion()
{
    return RDBMS_ODBC_FDO_VERSION;
}

FdoIConnectionPropertyDictionary* FdoRdbmsOdbcConnectionInfo::GetConnectionProperties()
{
    if (mPropertyDictionary == NULL)
    {
        mPropertyDictionary = new FdoCommonConnPropDictionary((FdoIConnection*)mConnection);
        
        // Allocate temporary collection to hold the set of SYSTEM/USER DSN's. 
        FdoStringsP dataSourceNameCollection = FdoStringCollection::Create();

        DbiConnection * dbiConnection = mConnection->GetDbiConnection();
        GdbiConnection * gdbiConnection = dbiConnection->GetGdbiConnection();
        rdbi_context_def * rdbiContext = gdbiConnection->GetRdbiContext();

        if (RDBI_SUCCESS == rdbi_stores_act(rdbiContext))
        {
            char  name[GDBI_MAXIMUM_STRING_SIZE+1];
            int   eof = FALSE;
            while (RDBI_SUCCESS == rdbi_stores_get(rdbiContext, name, &eof) && !eof)
            {
                dataSourceNameCollection->Add(name);
            }
            (void) rdbi_stores_deac(rdbiContext);
        }

        // Convert DSN collection to an array of strings needed for the dictionary entry.
        wchar_t **dataSourceNames = NULL;
        FdoInt32 dataSourceNameCount = dataSourceNameCollection->GetCount();
        dataSourceNames = (wchar_t **) new wchar_t[sizeof(wchar_t *) * dataSourceNameCount];
        for (FdoInt32 i=0; i<dataSourceNameCount; i++)
        {
		    FdoStringElementP nameElement = dataSourceNameCollection->GetItem(i);
		    FdoStringP name = nameElement->GetString();
            dataSourceNames[i] = new wchar_t[name.GetLength()+1];
            wcscpy(dataSourceNames[i], (const wchar_t*)name);
        }

        FdoPtr<ConnectionProperty> pProp;
        pProp = new ConnectionProperty (FDO_RDBMS_CONNECTION_USERID, NlsMsgGet(FDORDBMS_462, "UserId"), L"", false, false, false, false, false, false, false, 0, NULL); 
        mPropertyDictionary->AddProperty(pProp);
        
        pProp = new ConnectionProperty (FDO_RDBMS_CONNECTION_PASSWORD, NlsMsgGet(FDORDBMS_119, "Password"), L"", false, true, false, false, false, false, false, 0, NULL); 
        mPropertyDictionary->AddProperty(pProp);

        pProp = new ConnectionProperty (FDO_RDBMS_CONNECTION_DSN, NlsMsgGet(FDORDBMS_463, "DataSourceName"), L"", false, false, true, false, false, true, false, false, dataSourceNameCount, (const wchar_t **)dataSourceNames); 
        mPropertyDictionary->AddProperty(pProp);

        pProp = new ConnectionProperty (FDO_RDBMS_CONNECTION_CONNSTRING, NlsMsgGet(FDORDBMS_464, "ConnectionString"), L"", false, false, false, false, false, false, true, 0, NULL); 
        mPropertyDictionary->AddProperty(pProp);

        pProp = new ConnectionProperty (FDO_RDBMS_CONNECTION_GENDEFGEOMETRYPROP, NlsMsgGet(FDORDBMS_465, "GenerateDefaultGeometryProperty"), L"true", false, false, false, false, false, false, false, 0, NULL); 

        mPropertyDictionary->AddProperty(pProp);
    }
    FDO_SAFE_ADDREF(mPropertyDictionary.p);
    return mPropertyDictionary;
}

void FdoRdbmsOdbcConnectionInfo::Dispose()
{
    delete this;
}


