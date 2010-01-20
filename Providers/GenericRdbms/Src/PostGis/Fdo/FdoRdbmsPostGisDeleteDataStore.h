//
// Copyright (C) 2006 Refractions Research, Inc. 
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
#ifndef FDORDBMSPOSTGISDELETEDATASTORE_H
#define FDORDBMSPOSTGISDELETEDATASTORE_H

#include "../../Fdo/DataStore/FdoRdbmsDeleteDataStore.h"

/// \brief
/// Implements command to delete PostGIS datastore.
///
/// \sa FdoRdbmsCommand - declares commands common interface
///
/// \todo Review assertion set in the second constructor (see pointer
/// check in Execute).
/// \todo Is this correct that ctor creates instance of FdoRdbmsGetDatastores?
///
class FdoRdbmsPostGisDeleteDataStore: public FdoRdbmsDeleteDataStore
{
    friend class FdoRdbmsPostGisConnection;

protected:

    /// Default constructor.
    /// Creates default instance of FdoRdbmsGetDatastores.
    FdoRdbmsPostGisDeleteDataStore();

    /// Custom constructor.
    /// @param connection [in] - pointer to existing instance of
    /// PostGIS connection.
    FdoRdbmsPostGisDeleteDataStore(FdoRdbmsPostGisConnection* connection);

public:

	/// Gets the FdoIDataStorePropertyDictionary interface
    /// that can be used to dynamically query and set the properties
    /// required to destroy a datastore.
    ///
	///	@return Returns the property dictionary.
    ///
	virtual	FdoIDataStorePropertyDictionary* GetDataStoreProperties();

	///	Executes the FdoIDestroyDataStore command.
	virtual	void Execute();

};

#endif // FDORDBMSPOSTGISDELETEDATASTORE_H
