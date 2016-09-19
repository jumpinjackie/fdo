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
#ifndef FDOPOSTGISOVPROPERTYMAPPINGSINGLE_H
#define FDOPOSTGISOVPROPERTYMAPPINGSINGLE_H

#include <FdoCommon.h>
#include <Rdbms/Override/RdbmsOvPropertyMappingSingle.h>
#include <Rdbms/Override/PostGis/PostGisOv.h>
#include <Rdbms/Override/PostGis/PostGisOvPropertyMappingDefinition.h>

/// \brief
/// Concrete class defining the physical schema overrides for the single
/// type object property mapping.
/// Under the Single mapping type, the object property is kept in the same
/// table as its containing class.
/// This class is used in PostGIS schema overrides.
///
class FdoPostGISOvPropertyMappingSingle :
    public FdoPostGISOvPropertyMappingDefinition,
    public FdoRdbmsOvPropertyMappingSingle
{
public:

    /// Named constructor for FdoPostGISOvPropertyMappingSingle.
    /// 
    /// \return
    /// Pointer to new instance of FdoPostGISOvPropertyMappingSingle.
    /// 
    FDOPOSTGIS_OV_API static FdoPostGISOvPropertyMappingSingle* Create();

protected:

    /// Default constructor.
    FdoPostGISOvPropertyMappingSingle();

    /// Destructor.
    virtual ~FdoPostGISOvPropertyMappingSingle();

    /// Named destructor.
    /// Responsible for final destruction calling
    /// \code delete this \endcode.
    virtual void Dispose();

};

/// \brief
/// Declaration of smart-pointer type for FdoPostGISOvPropertyMappingSingle.
/// Provided for convenience.
///
typedef FdoPtr<FdoPostGISOvPropertyMappingSingle>
    FdoPostGISOvPropertyMappingSingleP;

#endif // FDOPOSTGISOVPROPERTYMAPPINGSINGLE_H
