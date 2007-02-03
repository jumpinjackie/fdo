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
#ifndef FDOPOSTGIS_UPDATECOMMAND_H_INCLUDED
#define FDOPOSTGIS_UPDATECOMMAND_H_INCLUDED

#include "FeatureCommand.h"
#include <Fdo/Commands/Feature/IUpdate.h>

//
// Forward declarations
//

namespace fdo { namespace postgis {


/// Implementation of interface of Update command which modifies features.
/// The command updates instances of a given class that match specified criteria.
///
class UpdateCommand : public FeatureCommand<FdoIUpdate>
{
public:

    UpdateCommand();
    virtual ~UpdateCommand();

    virtual void Dispose();

protected:

private:
	
};

}} // namespace fdo::postgis

#endif // FDOPOSTGIS_UPDATECOMMAND_H_INCLUDED
