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
#ifndef FDOPOSTGIS_SELECTCOMMAND_H_INCLUDED
#define FDOPOSTGIS_SELECTCOMMAND_H_INCLUDED

#include "FeatureCommand.h"
#include <Fdo/Commands/Feature/ISelect.h>

//
// Forward declarations
//
class fdo::postgis::Connection;

namespace fdo { namespace postgis {

/// Implementation of Select command interface for PostGIS provider.
/// The Select command queries for features of a given class that
/// match the specified criteria.
///
class SelectCommand : public FeatureCommand<FdoISelect>
{
public:

    SelectCommand(Connection* conn);

    //
    // FdoIDisposable interface
    //

    virtual void Dispose();

protected:

    /// Destructor.
    virtual ~SelectCommand();

private:

    typedef FeatureCommand<FdoISelect> Base;

};

}} // namespace fdo::postgis

#endif // FDOPOSTGIS_SELECTCOMMAND_H_INCLUDED
