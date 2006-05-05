#ifndef _FDORDBMSMYSQLCREATEDATASTORE_H_
#define _FDORDBMSMYSQLCREATEDATASTORE_H_
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

#include "../../Fdo/DataStore/FdoRdbmsCreateDatastore.h"
#include "FdoRdbmsMySqlDataStorePropertyDictionary.h"


/// <summary>This command creates a new MySql datastore.</summary>
class FdoRdbmsMySqlCreateDataStore: public FdoRdbmsCreateDataStore
{
    friend class FdoRdbmsMySqlConnection;

protected:

    //    Constructs a default instance of a FdoRdbmsGetDatastores.
    FdoRdbmsMySqlCreateDataStore () { }
    FdoRdbmsMySqlCreateDataStore (FdoRdbmsMySqlConnection* connection)
    {
        mConnection = connection;
		mDataStorePropertyDictionary = new FdoRdbmsMySqlDataStorePropertyDictionary(NULL, FDO_RDBMS_DATASTORE_FOR_CREATE);
    }

public:

	///	<summary>Executes the FdoICreateDataStore command.</summary>
	///	<returns>Returns nothing</returns> 
	virtual	void Execute() 
	{
		FdoRdbmsDataStorePropertyDictionary *p = (FdoRdbmsDataStorePropertyDictionary *)mDataStorePropertyDictionary;
 
		if ( !mConnection )
            throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_13, "Connection not established"));

		mConnection->CreateDb( 	
						 p->GetProperty(FDO_RDBMS_CONNECTION_DATASTORE),
						 p->GetProperty(FDO_RDBMS_DATASTORE_DESCRIPTION),      
						 L"",   // password not required
						 L"",	// service 
						 L"NONE",
						 L"NONE"
						 );
	} 

};
#endif
