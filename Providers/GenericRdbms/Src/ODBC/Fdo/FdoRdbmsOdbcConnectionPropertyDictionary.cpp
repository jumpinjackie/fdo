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
#include "FdoRdbmsOdbcConnectionPropertyDictionary.h"
#include "FdoRdbmsOdbcProviderInfo.h"
#include <Sm/Ph/Rd/OwnerReader.h>
#include "FdoCommonStringUtil.h"


FdoRdbmsOdbcConnectionPropertyDictionary::FdoRdbmsOdbcConnectionPropertyDictionary()
{
}

FdoRdbmsOdbcConnectionPropertyDictionary::FdoRdbmsOdbcConnectionPropertyDictionary(FdoRdbmsConnection *connection):
	FdoRdbmsPropertyDictionary(connection) 
{
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
    dataSourceNames = (wchar_t **) new wchar_t[sizeof(wchar_t *) * dataSourceNameCount]; // Deallocation will be handled by destructor
    if (NULL == dataSourceNames)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));
    for (FdoInt32 i=0; i<dataSourceNameCount; i++) {
		FdoStringElementP nameElement = dataSourceNameCollection->GetItem(i);
		FdoStringP name = nameElement->GetString();
        dataSourceNames[i] = new wchar_t[name.GetLength()+1];
        if (NULL == dataSourceNames[i])
            throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));
        wcscpy(dataSourceNames[i], (const wchar_t*)name);
    }


    AddItem(
        g_OdbcConnectionParmeter_UserId,
        NlsMsgGet(FDORDBMS_462, "UserId"),
        false, L"", false, false, false, 0, NULL, false, false, false);

    AddItem(
        g_OdbcConnectionParmeter_Password,
        NlsMsgGet(FDORDBMS_119, "Password"),
        false, L"", false, true, false, 0, NULL, false, false, false);

    AddItem(
        g_OdbcConnectionParmeter_DataSourceName,
        NlsMsgGet(FDORDBMS_463, "DataSourceName"),
        false, L"", false, false, true,
        dataSourceNameCount, (FdoString**)dataSourceNames,
        false, false, false);

    AddItem(
        g_OdbcConnectionParmeter_ConnectionString,
        NlsMsgGet(FDORDBMS_464, "ConnectionString"),
        false, L"", false, false, false, 0, NULL, false, false, false, true);

    AddItem(
        g_OdbcConnectionParmeter_GenerateGeometry,
        NlsMsgGet(FDORDBMS_465, "GenerateDefaultGeometryProperty"),
        false, L"true", false, false, false, 0, NULL, false, false, false);
}

