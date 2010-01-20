/*
 * 
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
#include <Sm/Ph/Rd/OwnerReader.h>

// FdoRdbmsPostGisGetDataStores
#include "FdoRdbmsPostGisGetDataStores.h"
#include "FdoRdbmsConnection.h"

// Constructor
FdoRdbmsPostGisGetDataStores::FdoRdbmsPostGisGetDataStores ()
{
}

// Constructor
FdoRdbmsPostGisGetDataStores::FdoRdbmsPostGisGetDataStores (FdoIConnection *connection) :
    FdoRdbmsGetDataStores(connection)
{
	FdoRdbmsGetDataStores::SetIncludeNonFdoEnabledDatastores(true);
}

FdoRdbmsPostGisGetDataStores::~FdoRdbmsPostGisGetDataStores (void)
{
}

void FdoRdbmsPostGisGetDataStores::SetIncludeNonFdoEnabledDatastores(bool include)
{
	FdoRdbmsGetDataStores::SetIncludeNonFdoEnabledDatastores(true);
}

FdoRdbmsDataStoreNames* FdoRdbmsPostGisGetDataStores::GetDataStoresNames()
{
    FdoRdbmsDataStoreNames *oDSNames = FdoRdbmsDataStoreNames::Create();

	FdoSchemaManagerP		schema = mConnection->GetSchemaManager();
	FdoSmPhMgrP				physicalMgr = schema->GetPhysicalSchema();
	FdoSmPhDatabaseP		pDatabase = physicalMgr->GetDatabase();
    FdoSmPhRdOwnerReaderP	reader = pDatabase->CreateOwnerReader();

    while ( reader->ReadNext() )
    {
        FdoStringP wdbnameP = reader->GetName();

        // Make a copy
        wchar_t *wdbname = new wchar_t[wdbnameP.GetLength() + 1];
        wcscpy( wdbname, (FdoString*) wdbnameP );

        // Append the pointer to this string 
        oDSNames = FdoRdbmsDataStoreNames::Append(oDSNames, 1, (FdoString**) &wdbname );
    }

    return oDSNames;
}

