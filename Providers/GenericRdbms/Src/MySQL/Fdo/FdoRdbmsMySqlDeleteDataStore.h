#ifndef _FDORDBMSMYSQLDELETEDATASTORE_H_
#define _FDORDBMSMYSQLDELETEDATASTORE_H_
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
#include "FdoRdbmsMySqlDataStorePropertyDictionary.h"

/// <summary>This command deletes a MySql datastore.</summary>

class FdoRdbmsMySqlDeleteDataStore: public FdoRdbmsDeleteDataStore
{
    friend class FdoRdbmsMySqlConnection;

protected:

    //    Constructs a default instance of a FdoRdbmsGetDatastores.
    FdoRdbmsMySqlDeleteDataStore () { }
    FdoRdbmsMySqlDeleteDataStore (FdoRdbmsMySqlConnection* connection)
    {
        mConnection = connection;
		mDataStorePropertyDictionary = new FdoRdbmsMySqlDataStorePropertyDictionary(NULL, FDO_RDBMS_DATASTORE_FOR_DELETE);
    }

public:

	///<summary>Gets the FdoIDataStorePropertyDictionary interface that	
	///can be used to dynamically query	and	set	the	properties required	
	///to destroy a datastore.</summary>
	///	<returns>Returns the property dictionary</returns>
	virtual	FdoIDataStorePropertyDictionary* GetDataStoreProperties() 
	{ 
		return (FdoIDataStorePropertyDictionary *)(mDataStorePropertyDictionary);
	}

	///	<summary>Executes the FdoIDestroyDataStore command.</summary>
	///	<returns>Returns nothing</returns> 
	virtual	void Execute() 
	{
		FdoRdbmsDataStorePropertyDictionary *p = (FdoRdbmsDataStorePropertyDictionary *)mDataStorePropertyDictionary;
 
		if ( !mConnection )
            throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_13, "Connection not established"));

		mConnection->DeleteDb( 	
						 p->GetProperty(FDO_RDBMS_CONNECTION_DATASTORE),
						 L"",	//password not required
						 L""	// service 
						 );
	} 
};
#endif
