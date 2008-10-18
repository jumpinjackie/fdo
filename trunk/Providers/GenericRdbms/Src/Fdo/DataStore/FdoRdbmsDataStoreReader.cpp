
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
#include "FdoRdbmsConnectionPropertyDictionary.h"

FdoRdbmsDataStoreReader::FdoRdbmsDataStoreReader( FdoRdbmsConnection *connection, FdoSmPhRdOwnerReaderP phReader, bool includeNonFdoDb ):
	mConnection( FDO_SAFE_ADDREF(connection) ),
    mPhOwnerReader ( phReader ),
	mIncludeNonFdoEnabledDatastores ( includeNonFdoDb )
{
	mDatastoreName = L"";
	mDatastoreDescription = L"";

    mConnectionCapabilities = connection->GetConnectionCapabilities();
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
    // Description can be expensive to retrieve so lazy load when needed
    LoadDescription();
	
    return mDatastoreDescription;
}

bool FdoRdbmsDataStoreReader::GetIsFdoEnabled()
{
 	return mPhOwnerReader->GetHasMetaSchema();
}

FdoIDataStorePropertyDictionary* FdoRdbmsDataStoreReader::GetDataStoreProperties()
{
    // Description, Long Transactiona and Lock modes can be expensive to retrieve so lazy load when needed
    LoadDescription();
    LoadLtLockMode();


	// Create a provider specific datastore dictionary property.
    if (mDatastoreProperty == NULL)
    {
	    mDatastoreProperty = mConnection->CreateDataStoreProperties( FDO_RDBMS_DATASTORE_FOR_READ );

	    int count;
	    FdoString **names = mDatastoreProperty->GetPropertyNames(count);

	    for (int i = 0; i < count; i++ )
	    {
		    if ( wcscmp( names[i], FDO_RDBMS_CONNECTION_DATASTORE ) == 0 )
			    mDatastoreProperty->SetProperty( names[i], mDatastoreName );
		    else if ( wcscmp( names[i], FDO_RDBMS_DATASTORE_DESCRIPTION ) == 0 )
			    mDatastoreProperty->SetProperty( names[i], mDatastoreDescription );
		    else if ( wcscmp( names[i], FDO_RDBMS_DATASTORE_LTMODE ) == 0 )
			    mDatastoreProperty->SetProperty( names[i], mLtMode );
		    else if ( wcscmp( names[i], FDO_RDBMS_DATASTORE_LOCKMODE ) == 0 )
			    mDatastoreProperty->SetProperty( names[i], mLockMode );
	    }
    }
	return FDO_SAFE_ADDREF(mDatastoreProperty.p);
}

bool FdoRdbmsDataStoreReader::ReadNext()
{
	bool found = false;

	while ( mPhOwnerReader && mPhOwnerReader->ReadNext() )
	{
        // at next row so reset "loaded" statuses
        mDescriptionLoaded = false;
        mLtLockModeLoaded = false;

		// Reject the non-fdo datastores if required.
		if ( mIncludeNonFdoEnabledDatastores ||
		   (!mIncludeNonFdoEnabledDatastores && mPhOwnerReader->GetHasMetaSchema() ) )
		{
			found = true;

			mDatastoreName = mPhOwnerReader->GetName();
 
 			break;
		} 
	} // while

	return found;
}

void FdoRdbmsDataStoreReader::Close()
{
	mPhOwnerReader = NULL;
}

void FdoRdbmsDataStoreReader::Dispose()
{
	delete this;
} 

void FdoRdbmsDataStoreReader::LoadDescription()
{
    if ( !mDescriptionLoaded ) 
    {
		mDatastoreDescription = L"";
        mDescriptionLoaded = true;

        // No description if no MetaSchema.
		if ( mPhOwnerReader->GetHasMetaSchema() )
		{
			try 
			{
				mDatastoreDescription = mPhOwnerReader->GetDescription();

			} catch (FdoException *ex) {
				// Old F_SCHEMAINFO might not contain 'description' field. 
				// Or the owner might not be "dbo" for the SQL Server.
				// Return "NONE" and continue.
				ex->Release();
			}
		}
    }
}

void FdoRdbmsDataStoreReader::LoadLtLockMode()
{
    if ( !mLtLockModeLoaded ) 
    {
    	mLtMode = L"NONE";
		mLockMode = L"NONE";

        mLtLockModeLoaded = true;

        // No need to check further unless provider supports locking or long transactions.
        if ( mConnectionCapabilities->SupportsLocking() || mConnectionCapabilities->SupportsLongTransactions() ) 
		{
            // f_options table won't be present when no metaschema.
      	    if ( mPhOwnerReader->GetHasMetaSchema() )
            {
                try 
                {
		            FdoPtr<FdoISQLCommand>	selCmd = (FdoISQLCommand*)mConnection->CreateCommand( FdoCommandType_SQLCommand );
            		
		            FdoStringP	sqlString = FdoStringP::Format(L"select name, value from %ls.f_options", (FdoString *)mDatastoreName);

		            selCmd->SetSQLStatement( sqlString );

		            FdoPtr<FdoISQLDataReader> optRdr = selCmd->ExecuteReader();

		            // read each option, looking for the long transaction and locking options.
		            while ( optRdr->ReadNext() ) 
		            {
			            FdoStringP	optName = optRdr->GetString(L"name");
			            FdoStringP  optValue;

			            if ( optName == L"LT_MODE" )
			            {
				            optValue = optRdr->GetString(L"value");
				            FdoLtLockModeType ltMode = (FdoLtLockModeType) optValue.ToLong();

				            if ( ltMode == FdoMode )
					            mLtMode = L"FDO";
				            else if ( ltMode == OWMMode )
					            mLtMode = L"OWM";
			            }
			            else if ( optName == L"LOCKING_MODE" ) 
			            {
				            optValue = optRdr->GetString(L"value");
				            FdoLtLockModeType lckMode = (FdoLtLockModeType)optValue.ToLong();
            				
				            if ( lckMode == FdoMode )
					            mLockMode = L"FDO";
				            else if ( lckMode == OWMMode )
					            mLockMode = L"OWM";
			            }
		            } // while

		        } catch (FdoException *ex) {
			        // Owner might not be "dbo" for the SQL Server.
			        // Return "NONE" and continue.
			        ex->Release();
		        }
            }
        }
    }
}
