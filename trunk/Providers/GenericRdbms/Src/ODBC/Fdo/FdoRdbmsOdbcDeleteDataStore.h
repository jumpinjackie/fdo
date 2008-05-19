#ifndef _FDORDBMSODBCDELETEDATASTORE_H_
#define _FDORDBMSODBCDELETEDATASTORE_H_
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
#ifdef _WIN32
#pragma once
#endif

#include "../../Fdo/DataStore/FdoRdbmsDeleteDataStore.h"
#include "../../Fdo/FeatureCommands/FdoRdbmsCommand.h"

/// <summary>This command creates a new Odbc datastore.</summary>
class FdoRdbmsOdbcDeleteDataStore: public FdoRdbmsDeleteDataStore
{
    friend class FdoRdbmsOdbcConnection;

protected:

    //    Constructs a default instance of a FdoRdbmsDeleteDatastores.
    FdoRdbmsOdbcDeleteDataStore () { }
    FdoRdbmsOdbcDeleteDataStore (FdoRdbmsOdbcConnection* connection)
    {
        mConnection = connection;
		mDataStorePropertyDictionary = new FdoCommonDataStorePropDictionary(mConnection);
        
        FdoPtr<ConnectionProperty> newProp;
        newProp = new ConnectionProperty (FDO_RDBMS_CONNECTION_DATASTORE, NlsMsgGet(FDORDBMS_117, "DataStore"), L"", true, false, false, false, false, true, false, 0, NULL);
        mDataStorePropertyDictionary->AddProperty(newProp);

        newProp = new ConnectionProperty (FDO_RDBMS_CONNECTION_PASSWORD, NlsMsgGet(FDORDBMS_119, "Password"), L"", true, true, false, false, false, false, false, 0, NULL);
        mDataStorePropertyDictionary->AddProperty(newProp);
	}

public:

	///	<summary>Executes the FdoIDestroyDataStore command.</summary>
	///	<returns>Returns nothing</returns> 
	virtual	void Execute() 
	{
		if ( !mConnection )
            throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_13, "Connection not established"));

		mConnection->DeleteDb( 	
						 mDataStorePropertyDictionary->GetProperty(FDO_RDBMS_CONNECTION_DATASTORE),
						 mDataStorePropertyDictionary->GetProperty(FDO_RDBMS_CONNECTION_PASSWORD),
						 L""	// service 
						 );
	} 
};
#endif
