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
#ifndef FDOPOSTGIS_INSERTCOMMAND_H_INCLUDED
#define FDOPOSTGIS_INSERTCOMMAND_H_INCLUDED

#include "FeatureCommand.h"
#include <Fdo/Commands/Feature/IInsert.h>

//
// Forward declarations
//
class fdo::postgis::Connection;

namespace fdo { namespace postgis {

/// Implementation of interface of Insert command for PostGIS provider.
/// The Insert command inserts a new instance of a given class to the datastore.
///
class InsertCommand : public FeatureCommand<FdoIInsert>
{
public:

    /// Create command for given connection instance.
    InsertCommand(Connection* conn);

protected:

private:

    typedef FeatureCommand<FdoIInsert> Base;

    /// Destructor.
    virtual ~InsertCommand();
};

}} // namespace fdo::postgis

#endif // FDOPOSTGIS_INSERTCOMMAND_H_INCLUDED
