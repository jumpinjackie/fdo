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
#ifndef FDOPOSTGIS_DELETECOMMAND_H_INCLUDED
#define FDOPOSTGIS_DELETECOMMAND_H_INCLUDED

#include "FeatureCommand.h"
#include <Fdo/Commands/Feature/IDelete.h>

//
// Forward declarations
//

namespace fdo { namespace postgis {

/// Implementation of interface of Delete command which deletes features.
/// The command deletes instances of a given class that match specified criteria.
///
class DeleteCommand : public FeatureCommand<FdoIDelete>
{
public:

    /// Construction of command operating on given connection.
    DeleteCommand(Connection* conn);

    //
    // FdoIDelete interface
    //

    /// Execute delete command and returns the number of affected instances.
    FdoInt32 Execute();

    /// Get a lock conflict reader providing access to conflicts that occured
    /// during delete command execution.
    ///
    /// Deleting objects might result in lock conflicts if objects to be
    /// deleted are not exclusively locked for the user attempting to
    /// delete the object. A lock conflict report is generated.
    ///
    FdoILockConflictReader* GetLockConflicts();

protected:

private:

    virtual ~DeleteCommand();
};

}} // namespace fdo::postgis

#endif // FDOPOSTGIS_DELETECOMMAND_H_INCLUDED
