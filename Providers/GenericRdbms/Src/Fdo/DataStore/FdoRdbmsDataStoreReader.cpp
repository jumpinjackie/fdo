
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
#include "FdoRdbmsDataStoreReader.h"
#include "FdoRdbmsConnection.h"


FdoRdbmsDataStoreReader::FdoRdbmsDataStoreReader( FdoRdbmsConnection *connection, FdoSmPhRdOwnerReaderP phReader, bool includeNonFdoDb ):
	mConnection( FDO_SAFE_ADDREF(connection) ),
    mPhOwnerReader ( phReader ),
	mIncludeNonFdoEnabledDatastores ( includeNonFdoDb )
{
	FDO_SAFE_ADDREF(mPhOwnerReader.p);
	mIsFdoEnabled = true;
	mDatastoreName = L"";
	mDatastoreDescription = L"";
}

FdoRdbmsDataStoreReader::~FdoRdbmsDataStoreReader()
{
}

FdoString* FdoRdbmsDataStoreReader::GetName()
{
	return mDatastoreName;
}

FdoString* FdoRdbmsDataStoreReader::GetDescription()
{
	return mDatastoreDescription;
}

bool FdoRdbmsDataStoreReader::GetIsFdoEnabled()
{
 	return mIsFdoEnabled;
}

FdoIDataStorePropertyDictionary* FdoRdbmsDataStoreReader::GetDataStoreProperties()
{
	// Create a provider specific datastore dictionary property.
	mDatastoreProperty = (FdoRdbmsDataStorePropertyDictionary *)mConnection->CreateDataStoreProperties( FDO_RDBMS_DATASTORE_FOR_READ );

	int count;
	FdoString **names = mDatastoreProperty->GetPropertyNames(count);

	for (int i = 0; i < count; i++ )
	{
		FdoStringP name =  names[i];

		if ( wcscmp( name, FDO_RDBMS_CONNECTION_DATASTORE ) == 0 )
			mDatastoreProperty->SetProperty( name, mDatastoreName );
		else if ( wcscmp( name, FDO_RDBMS_DATASTORE_DESCRIPTION ) == 0 )
			mDatastoreProperty->SetProperty( name, mDatastoreDescription );
		else if ( wcscmp( name, FDO_RDBMS_DATASTORE_LTMODE ) == 0 )
			mDatastoreProperty->SetProperty( name, mLtMode );
		else if ( wcscmp( name, FDO_RDBMS_DATASTORE_LOCKMODE ) == 0 )
			mDatastoreProperty->SetProperty( name, mLockMode );
		else 
			;
	}
	return (FdoIDataStorePropertyDictionary *)FDO_SAFE_ADDREF(mDatastoreProperty.p);
}

bool FdoRdbmsDataStoreReader::ReadNext()
{
	bool found = false;

	while ( mPhOwnerReader->ReadNext() )
	{
		// We need to know if FDO in order to get the description or not
		mIsFdoEnabled = mPhOwnerReader->GetHasMetaSchema();

		// Reject the non-fdo datastores if required.
		if ( mIncludeNonFdoEnabledDatastores ||
		   (!mIncludeNonFdoEnabledDatastores && mIsFdoEnabled ) )
		{
			found = true;

			mDatastoreName = mPhOwnerReader->GetName();
 
			mDatastoreDescription = L"";
			mLtMode = L"NONE";
			mLockMode = L"NONE";

			if ( mIsFdoEnabled )
			{
				try 
				{
					mDatastoreDescription = mPhOwnerReader->GetDescription();

					// Create another reader for F_OFTIONS table
					FdoSmPhOptionsReaderP optRdr = mPhOwnerReader->GetManager()->CreateOptionsReader(mDatastoreName);

					// read each option, looking for the long transaction and locking options.
					while ( optRdr->ReadNext() ) 
					{
						FdoStringP optName = optRdr->GetName();

						if ( optName == L"LT_MODE" )
						{
							FdoLtLockModeType ltMode = (FdoLtLockModeType)(optRdr->GetValue().ToLong());

							if ( ltMode == FdoMode )
								mLtMode = L"FDO";
							else if ( ltMode == OWMMode )
								mLtMode = L"OWM";
						}
						else if ( optName == L"LOCKING_MODE" ) 
						{
							FdoLtLockModeType lckMode = (FdoLtLockModeType)(optRdr->GetValue().ToLong());
							
							if ( lckMode == FdoMode )
								mLockMode = L"FDO";
							else if ( lckMode == OWMMode )
								mLockMode = L"OWM";
						}
					} // while
				} catch (FdoException *ex) {
					// Old F_SCHEMAINFO might not contain 'description' field. 
					// Or the owner might not be "dbo" for the SQL Server.
					// Return "NONE" and continue.
					ex->Release();
				}
			}
 			break;
		} 
	} // while

	return found;
}

void FdoRdbmsDataStoreReader::Close()
{
	//Should close PhOwnerReader? how?
}

void FdoRdbmsDataStoreReader::Dispose()
{
	delete this;
} 
