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

// FdoRdbmsGetDataStores
#include "FdoRdbmsGetDataStores.h"
#include "FdoRdbmsConnection.h"

// Constructor
FdoRdbmsGetDataStores::FdoRdbmsGetDataStores (): 
mConnection( NULL ),
mIncludeNonFdoEnabledDatastores (true)
{
}

// Constructor
FdoRdbmsGetDataStores::FdoRdbmsGetDataStores (FdoIConnection *connection) :
    mConnection ( NULL)

{
  mFdoConnection = static_cast<FdoRdbmsConnection*>(connection);
  if( mFdoConnection )
      mConnection = mFdoConnection->GetDbiConnection();
}

// Destructor
FdoRdbmsGetDataStores::~FdoRdbmsGetDataStores()
{
}

FdoString* FdoRdbmsGetDataStores::GetDataStorePropertyName()
{
    return L"DataStore";
}

bool FdoRdbmsGetDataStores::GetIncludeNonFdoEnabledDatastores()
{
	
	return mIncludeNonFdoEnabledDatastores;
}

void FdoRdbmsGetDataStores::SetIncludeNonFdoEnabledDatastores(bool include)
{
	mIncludeNonFdoEnabledDatastores = include;
}

FdoIDataStoreReader* FdoRdbmsGetDataStores::Execute()
{
	FdoSchemaManagerP		schema = mConnection->GetSchemaManager();
	FdoSmPhMgrP				physicalMgr = schema->GetPhysicalSchema();
	FdoSmPhDatabaseP		pDatabase = physicalMgr->GetDatabase();
    FdoSmPhRdOwnerReaderP	phOwnerReader = pDatabase->CreateOwnerReader();

	FdoIDataStoreReader* fdoReader = new FdoRdbmsDataStoreReader( mFdoConnection, 
                                                                  phOwnerReader,
																  mIncludeNonFdoEnabledDatastores );
 	return fdoReader;
}


FdoRdbmsDataStoreNames* FdoRdbmsGetDataStores::GetDataStoresNames()
{
    FdoRdbmsDataStoreNames *oDSNames = FdoRdbmsDataStoreNames::Create();

	FdoSchemaManagerP		schema = mConnection->GetSchemaManager();
	FdoSmPhMgrP				physicalMgr = schema->GetPhysicalSchema();
	FdoSmPhDatabaseP		pDatabase = physicalMgr->GetDatabase();
    FdoSmPhRdOwnerReaderP	reader = pDatabase->CreateOwnerReader();

    while ( reader->ReadNext() )
    {
        if ( reader->GetHasMetaSchema() ) {
            FdoStringP wdbnameP = reader->GetName();

            // Make a copy
            wchar_t *wdbname = new wchar_t[wdbnameP.GetLength() + 1];
            wcscpy( wdbname, (FdoString*) wdbnameP );

            // Append the pointer to this string 
            oDSNames = FdoRdbmsDataStoreNames::Append(oDSNames, 1, (FdoString**) &wdbname );
        }
    }

    return oDSNames;
}

void FdoRdbmsGetDataStores::Dispose()
{
	delete this;
}
