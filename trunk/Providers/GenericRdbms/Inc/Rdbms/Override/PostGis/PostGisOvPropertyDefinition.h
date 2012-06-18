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
#ifndef FDOPOSTGISOVPROPERTYDEFINITION_H
#define FDOPOSTGISOVPROPERTYDEFINITION_H

#include <Rdbms/Override/RdbmsOvPropertyDefinition.h>
#include <Rdbms/Override/PostGis/PostGisOv.h>

/// \brief
/// Abstract class defining physical schema overrides
/// for a property definition.
/// This class is used in PostGIS schema overrides.
///
class FdoPostGISOvPropertyDefinition :
    public virtual FdoRdbmsOvPropertyDefinition
{
    friend class FdoPostGISOvClassDefinition;

protected:
    
    /// Default constructor.
    FdoPostGISOvPropertyDefinition();

    /// Alternative constructor.
    /// \param name [in] - name of class definition. 
    FdoPostGISOvPropertyDefinition(FdoString* name);

    /// Destructor.
    virtual ~FdoPostGISOvPropertyDefinition();

    /// Named destructor.
    /// Responsible for final destruction calling
    /// \code delete this \endcode.
    virtual void Dispose();

};

/// \brief
/// Declaration of smart-pointer type for FdoPostGISOvPropertyDefinition.
/// Provided for convenience.
///
typedef FdoPtr<FdoPostGISOvPropertyDefinition> FdoPostGISOvPropertyP;

#endif // FDOPOSTGISOVPROPERTYDEFINITION_H
