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
#ifndef FDORDBMSPOSTGISCREATEDATASTORE_H
#define FDORDBMSPOSTGISCREATEDATASTORE_H

#include "../../Fdo/DataStore/FdoRdbmsCreateDatastore.h"

/// \brief
/// Defines command to create new PostGis datastore.
///
/// \sa FdoICreateDataStore
///
class FdoRdbmsPostGisCreateDataStore: public FdoRdbmsCreateDataStore
{
    friend class FdoRdbmsPostGisConnection;

protected:

    /// Default constructor.
    /// Creates default instance of FdoRdbmsGetDatastores.
    FdoRdbmsPostGisCreateDataStore();

    /// Custom constructor.
    /// Creates new datastore using given connection instance.
    /// Here, also datastore properties dictionary is created
    /// and populated with base properties
    ///
    /// @param connection [in] - pointer to existing instance of
    /// PostGIS connection.
    ///
    FdoRdbmsPostGisCreateDataStore(FdoRdbmsPostGisConnection* connection);

public:

	///	Executes the FdoICreateDataStore command.
	virtual	void Execute();

};

#endif // FDORDBMSPOSTGISCREATEDATASTORE_H
