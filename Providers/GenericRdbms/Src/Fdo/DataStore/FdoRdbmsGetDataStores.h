#ifndef _FDORDBMSGETDATASTORES_H_
#define _FDORDBMSGETDATASTORES_H_
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

#ifdef _WIN32
#pragma once
#endif

#include "FdoRdbms.h"
#include "Std.h"

#include "FdoRdbmsCommand.h"
#include "FdoRdbmsDataStoreReader.h"
#include "Fdo/Commands/DataStore/IListDataStores.h"

class DbiConnection;
typedef  FdoArray<FdoString *> FdoRdbmsDataStoreNames;

/// <summary>This command returns the list of datastores an opened
/// connection can activate.</summary>
class FdoRdbmsGetDataStores: public FdoRdbmsCommand<FdoIListDataStores> 
{
    friend class FdoRdbmsConnection;
    friend class FdoRdbmsDataStorePropertyDictionary;

protected:
    DbiConnection* mConnection;

    //    Constructs a default instance of a FdoRdbmsGetDatastores.
    FdoRdbmsGetDataStores ();

    //    Constructs an instance of a FdoRdbmsGetDatastores using the
    //    specified arguments.
    FdoRdbmsGetDataStores (FdoIConnection *connection);

    virtual ~FdoRdbmsGetDataStores();

public:

	// From FDO

	///<summary>Gets the flag that indicates whether to include or
	///not include Non-FDO enabled datastores in the returned list.
	///These are also referred to as “Foreign” datastores.</summary>
	/// <returns>Returns the flag</returns>
	virtual bool GetIncludeNonFdoEnabledDatastores();

	///<summary>Sets the flag to indicate whether to include or not include
	///Non-FDO enabled datastores in the returned list. These are also
	///referred to as “Foreign” datastores.</summary>
	/// <returns>Returns nothing</returns>
	virtual void SetIncludeNonFdoEnabledDatastores(bool include);

	/// <summary>Executes the FdoIListDataStores command.</summary>
	/// <returns>Returns nothing</returns> 
	virtual FdoIDataStoreReader* Execute();

    /// <summary>Get the list of datastore names.</summary>
    /// <returns>Returns the list of datastores as a FeatureReader.</returns>
    virtual FdoRdbmsDataStoreNames* GetDataStoresNames();

    /// <summary>Get the datastore property name.</summary>
    /// <returns>Returns the datastore property name in the connection string
    virtual FdoString* GetDataStorePropertyName();

	virtual void Dispose();

private:
	FdoRdbmsConnection*	mFdoConnection;
	bool				mIncludeNonFdoEnabledDatastores;
};
#endif

