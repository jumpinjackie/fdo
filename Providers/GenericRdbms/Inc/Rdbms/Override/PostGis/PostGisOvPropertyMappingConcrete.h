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
#ifndef FDOPOSTGISOVPROPERTYMAPPINGRCONCRETE_H
#define FDOPOSTGISOVPROPERTYMAPPINGRCONCRETE_H

#include <Rdbms/Override/PostGis/PostGisOv.h>
#include <Rdbms/Override/PostGis/PostGisOvPropertyMappingRelation.h>

/// \brief
/// Concrete class defining the physical schema overrides for
/// the concrete type object property mapping.
/// Under the concrete mapping type, each object property is
/// kept in its own table.
/// This class is used in PostGIS schema overrides.
///
class FdoPostGISOvPropertyMappingConcrete :
    public FdoPostGISOvPropertyMappingRelation,
    public FdoRdbmsOvPropertyMappingConcrete
{
public:

    /// Named constructor for FdoPostGISOvPropertyMappingConcrete.
    /// 
    /// \return
    /// Pointer to new instance of FdoPostGISOvPropertyMappingConcrete.
    /// 
    FDOPOSTGIS_OV_API static FdoPostGISOvPropertyMappingConcrete* Create();

protected:

    /// Default constructor.
    FdoPostGISOvPropertyMappingConcrete();

    /// Destructor.
    virtual ~FdoPostGISOvPropertyMappingConcrete();

    /// Named destructor.
    /// Responsible for final destruction calling
    /// \code delete this \endcode.
    virtual void Dispose();

private:
};

/// \brief
/// Declaration of smart-pointer type for FdoPostGISOvColumn.
/// Provided for convenience.
///
typedef FdoPtr<FdoPostGISOvPropertyMappingConcrete>
    FdoPostGISOvPropertyMappingConcreteP;

#endif // FDOPOSTGISOVPROPERTYMAPPINGRCONCRETE_H
