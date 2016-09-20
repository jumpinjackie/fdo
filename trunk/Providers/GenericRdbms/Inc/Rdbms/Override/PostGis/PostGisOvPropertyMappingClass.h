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
#ifndef FDOPOSTGISOVPROPERTYMAPPINGCLASS_H
#define FDOPOSTGISOVPROPERTYMAPPINGCLASS_H

#include <Rdbms/Override/PostGis/PostGisOv.h>
#include <Rdbms/Override/PostGis/PostGisOvPropertyMappingRelation.h>
#include <Rdbms/Override/RdbmsOvPropertyMappingClass.h>

/// \brief
/// Concrete class defining the physical schema overrides for the Class
/// type object property mapping.
/// This class is used in PostGIS schema overrides.
///
class FdoPostGISOvPropertyMappingClass :
    public FdoPostGISOvPropertyMappingRelation,
    public FdoRdbmsOvPropertyMappingClass
{
public:

    /// Named constructor for FdoPostGISOvPropertyMappingClass.
    /// 
    /// \return
    /// Pointer to new instance of FdoPostGISOvPropertyMappingClass.
    /// 
    FDOPOSTGIS_OV_API static FdoPostGISOvPropertyMappingClass* Create();

protected:

    /// Default constructor.
    FdoPostGISOvPropertyMappingClass();

    /// Destructor.
    virtual ~FdoPostGISOvPropertyMappingClass();

    /// Named destructor.
    /// Responsible for final destruction calling
    /// \code delete this \endcode.
    virtual void Dispose();

};

/// \brief
/// Declaration of smart-pointer type for FdoPostGISOvPropertyMappingClass.
/// Provided for convenience.
///
typedef FdoPtr<FdoPostGISOvPropertyMappingClass>
    FdoPostGISOvPropertyMappingClassP;

#endif // FDOPOSTGISOVPROPERTYMAPPINGCLASS_H
