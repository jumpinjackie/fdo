#ifndef _FDORDBMSDESTROYDATASTORE_H_
#define _FDORDBMSDESTROYDATASTORE_H_

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
 */

/// <library>
/// FdoRdbms.lib
/// </library>

#ifdef _WIN32
#pragma once
#endif

#include "Fdo/Commands/DataStore/IDestroyDataStore.h"
#include "FdoRdbmsDataStorePropertyDictionary.h"
#include "FdoRdbmsConnection.h"
#include "FdoRdbmsCommand.h"

/// <summary>The FdoIDestroyDataStore interface defines the destroy datastore command
/// that can be used to remove and existing datastore. The input is provided
/// through a dictionary of name value pairs that is provider dependant. </summary>
class FdoRdbmsDeleteDataStore: public FdoRdbmsCommand<FdoIDestroyDataStore>
{
	friend class FdoRdbmsConnection;

public:

	///<summary>Gets the FdoIDataStorePropertyDictionary interface that	
	///can be used to dynamically query	and	set	the	properties required	
	///to destroy a datastore.</summary>
	///	<returns>Returns the property dictionary</returns>
	virtual	FdoIDataStorePropertyDictionary* GetDataStoreProperties() 
	{ 
		return (FdoIDataStorePropertyDictionary *)(mDataStorePropertyDictionary);
	}

	/// <summary>Executes the FdoIDeleteDataStore command.</summary>
	/// <returns>Returns nothing</returns> 
	virtual void Execute() = 0; 

protected:
	FdoRdbmsDataStorePropertyDictionary *mDataStorePropertyDictionary;
    FdoRdbmsConnection* mConnection;
};

#endif

