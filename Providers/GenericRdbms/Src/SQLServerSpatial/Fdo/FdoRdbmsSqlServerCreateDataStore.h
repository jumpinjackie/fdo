#ifndef _FDORDBMSSQLSERVERCREATEDATASTORE_H_
#define _FDORDBMSSQLSERVERCREATEDATASTORE_H_
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
#ifdef _WIN32
#pragma once
#endif

#include "../../Fdo/DataStore/FdoRdbmsCreateDataStore.h"
#include "../../Fdo/FeatureCommands/FdoRdbmsCommand.h"

/// <summary>This command creates a new SqlServer datastore.</summary>
class FdoRdbmsSqlServerCreateDataStore: public FdoRdbmsCreateDataStore
{
    friend class FdoRdbmsSqlServerConnection;

protected:

    //    Constructs a default instance of a FdoRdbmsCreateDatastore.
    FdoRdbmsSqlServerCreateDataStore () { }
    FdoRdbmsSqlServerCreateDataStore (FdoRdbmsSqlServerConnection* connection)
    {
        mConnection = connection;
        mDataStorePropertyDictionary = (FdoCommonDataStorePropDictionary*)mConnection->CreateDataStoreProperties(FDO_RDBMS_DATASTORE_FOR_CREATE);
    }

public:
	///<summary>Gets the FdoIDataStorePropertyDictionary interface that	
	///can be used to dynamically query	and	set	the	properties required	
	///to destroy a datastore.</summary>
	///	<returns>Returns the property dictionary</returns>
	virtual	FdoIDataStorePropertyDictionary* GetDataStoreProperties() 
	{ 
		return (FdoIDataStorePropertyDictionary *)(FDO_SAFE_ADDREF(mDataStorePropertyDictionary.p));
	}

	///	<summary>Executes the FdoICreateDataStore command.</summary>
	///	<returns>Returns nothing</returns> 
	virtual	void Execute() 
	{
		if ( !mConnection )
            throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_13, "Connection not established"));
        /*
		FdoString *lckMode = mDataStorePropertyDictionary->GetProperty(FDO_RDBMS_DATASTORE_LOCKMODE);
		FdoString *ltMode  = mDataStorePropertyDictionary->GetProperty(FDO_RDBMS_DATASTORE_LTMODE);
		if (wcscmp( lckMode, L"FDO" ) != 0 && wcscmp( lckMode, L"NONE" ) != 0)
			mDataStorePropertyDictionary->SetProperty(FDO_RDBMS_DATASTORE_LOCKMODE, 
			               mDataStorePropertyDictionary->GetPropertyDefault(FDO_RDBMS_DATASTORE_LOCKMODE));
		if (wcscmp( lckMode, L"NONE" ) == 0)
			mDataStorePropertyDictionary->SetProperty(FDO_RDBMS_DATASTORE_LTMODE, L"NONE");
		else
		{
			if (wcscmp( ltMode, L"FDO" ) != 0 && wcscmp( ltMode, L"NONE" ) != 0)
				mDataStorePropertyDictionary->SetProperty(FDO_RDBMS_DATASTORE_LTMODE, 
							   mDataStorePropertyDictionary->GetPropertyDefault(FDO_RDBMS_DATASTORE_LTMODE));
		}
        */
		mConnection->CreateDb( 	
						 mDataStorePropertyDictionary->GetProperty(FDO_RDBMS_CONNECTION_DATASTORE),
						 mDataStorePropertyDictionary->GetProperty(FDO_RDBMS_DATASTORE_DESCRIPTION),      
						 L"",     // password
						 L"",	  // service 
						 L"NONE", // mDataStorePropertyDictionary->GetProperty(FDO_RDBMS_DATASTORE_LTMODE),
						 L"NONE"  // mDataStorePropertyDictionary->GetProperty(FDO_RDBMS_DATASTORE_LOCKMODE)
						 );
	} 
};
#endif
