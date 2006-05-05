#ifndef _FDORDBMSDATASTOREREADER_H_
#define _FDORDBMSDATASTOREREADER_H_

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
#include <Std.h>
#include "Fdo/Commands/DataStore/IDataStoreReader.h"
#include "FdoRdbmsDataStorePropertyDictionary.h"

#include <Sm/Ph/SchemaReader.h>
#include <Sm/Ph/Rd/OwnerReader.h>
#include <Sm/Ph/OptionsReader.h>

/// <summary>The FdoIDataStoreReader interface provides a forward-only, read-only iterator
/// for reading feature data.  A reference to an FdoIDataStoreReader is returned
/// from the IListDataStores command. Because the initial position of the
/// FdoIDataStoreReader is prior to the first item, you must call
/// ReadNext to begin accessing any data.</summary>

class FdoRdbmsDataStoreReader : public FdoIDataStoreReader
{
	friend class FdoRdbmsGetDataStores;

public:

    /// <summary>Gets the name of the datastore currently being read.</summary>
    /// <returns>Returns the description of the datastore.</returns> 
	virtual FdoString* GetName();

    /// <summary>Gets the description of the datastore currently being read.</summary>
    /// <returns>Returns the description of the datastore.</returns> 
	virtual FdoString* GetDescription();

	///<summary>Gets the flag that indicates whether the datastore is FDO enabled.
	///Non-FDO databases are also referred to as “Foreign” datastores.</summary>
	/// <returns>Returns the flag</returns>
	virtual bool GetIsFdoEnabled();

	///<summary>Gets the FdoIConnectionPropertyDictionary interface that 
	///includes the properties of the datastores set at datastore create 
	///time, with the exception of password.</summary>
	/// <returns>Returns the property dictionary</returns>
	virtual FdoIDataStorePropertyDictionary* GetDataStoreProperties();

    /// <summary>Advances the reader to the next item and returns true if there is
    /// another object to read or false if reading is complete. The default
    /// position of the reader is prior to the first item. Thus you must
    /// call ReadNext to begin accessing any data.</summary>
    /// <returns>Returns true if there is a next item.</returns> 
	virtual bool ReadNext();

    /// <summary>Closes the FdoIDataStoreReader object, freeing any resources it may be holding.</summary>
    /// <returns>Returns nothing</returns> 
	virtual void Close();

	virtual void Dispose();

protected:
	
	FdoRdbmsDataStoreReader();
	FdoRdbmsDataStoreReader( FdoRdbmsConnection *connection,
                             FdoSmPhRdOwnerReaderP phOwnerReader, 
							 bool includeNonFdoDb );

	~FdoRdbmsDataStoreReader();

private:
    FdoPtr<FdoRdbmsConnection> mConnection;
	FdoSmPhRdOwnerReaderP	mPhOwnerReader;
	bool					mIncludeNonFdoEnabledDatastores; 
	bool					mIsFdoEnabled;
	FdoStringP				mDatastoreName;
	FdoStringP				mDatastoreDescription;
	FdoStringP				mLtMode;
	FdoStringP				mLockMode;	
	FdoRdbmsDataStorePropertyDictionaryP mDatastoreProperty;
};

#endif
