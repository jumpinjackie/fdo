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
#ifndef FDOSMPHPOSTGISTEMPOBJECT_H
#define FDOSMPHPOSTGISTEMPOBJECT_H

#include "DbObject.h"
#include <Sm/Ph/TempObject.h>
#include <Sm/Ph/Mgr.h>

/// Definition of type for a temporary database object.
/// This type is used by PostGIS provider.
///
/// \todo TODO: mloskot - Does PostGIS provider need this class?
///
class FdoSmPhPostGisTempObject :
    public FdoSmPhTempObject,
    public FdoSmPhPostGisDbObject
{
public:

    /// Create an instance of a temporary database object..
    ///
    /// \param
    /// mgr [in] - pointer to Physical Schema Manager instance.
    ///
    FdoSmPhPostGisTempObject(FdoSmPhMgrP mgr);

    /// Destructor.
    /// Currently destructor is empty.
    ~FdoSmPhPostGisTempObject();

};

/// \brief
/// Declaration of smart-pointer type.
/// Provided for convenience.
///
typedef FdoPtr<FdoSmPhPostGisTempObject> FdoSmPhPostGisTempObjectP;

#endif // FDOSMPHPOSTGISTEMPOBJECT_H