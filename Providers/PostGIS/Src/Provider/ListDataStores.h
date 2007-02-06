//
// Copyright (C) 2007 Refractions Research, Inc. 
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
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
//
#ifndef FDOPOSTGIS_LISTDATASTORES_H_INCLUDED
#define FDOPOSTGIS_LISTDATASTORES_H_INCLUDED

#include "Command.h"

//
// Forward declarations
//
class FdoIDataStoreReader;


namespace fdo { namespace postgis {

// Forward declarations of internal types
class Connection;

/// Implementation of list datastores command from a particular
/// PostgreSQL instance.
///
class ListDataStores : public Command<FdoIListDataStores>
{
public:

    /// Default constructor.
    ListDataStores();

    /// Constructor creates command for given connection.
    ListDataStores(Connection* conn);

    /// Destructor.
    virtual ~ListDataStores();

    //
    // FdoIListDataStores interface
    //

    /// Check if non-FDO datastores have been requested in the returned list.
	bool GetIncludeNonFdoEnabledDatastores();

    /// Set flag indicating that non-FDO datastores should be included
    /// in the returned list.
	void SetIncludeNonFdoEnabledDatastores(bool includeNonFdo);

    /// Executes the FdoIListDataStores command.
    FdoIDataStoreReader* Execute();

protected:

    bool mIncludeNonFdo;

private:

    typedef Command<FdoIListDataStores> Base;
};

}} // namespace fdo::postgis

#endif // FDOPOSTGIS_LISTDATASTORES_H_INCLUDED
