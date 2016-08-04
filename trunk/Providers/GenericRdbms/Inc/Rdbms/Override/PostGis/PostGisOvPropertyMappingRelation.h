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
#ifndef FDOPOSTGISOVPROPERTYMAPPINGRELATION_H
#define FDOPOSTGISOVPROPERTYMAPPINGRELATION_H

#include <Rdbms/Override/RdbmsOvPropertyMappingConcrete.h>
#include <Rdbms/Override/PostGis/PostGisOv.h>
#include <Rdbms/Override/PostGis/PostGisOvPropertyMappingDefinition.h>

class FdoPostGISOvClassDefinition;

/// \brief
/// Abstract class defining the physical schema overrides for
/// relation type object property table mappings.
/// This class is used in PostGIS schema overrides.
///
class FdoPostGISOvPropertyMappingRelation :
    public FdoPostGISOvPropertyMappingDefinition,
    public virtual FdoRdbmsOvPropertyMappingRelation
{
public:

    /// Named constructor for FdoPostGISOvPropertyMappingRelation.
    /// 
    /// \return
    /// Pointer to new instance of FdoPostGISOvPropertyMappingRelation.
    /// 
    FDOPOSTGIS_OV_API FdoPostGISOvClassDefinition* GetInternalClass();

    /// \brief
    /// Sets the internal class.
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDOPOSTGIS_OV_API void SetInternalClass(
        FdoPostGISOvClassDefinition* classDef);

protected:

    /// Default constructor.
    FdoPostGISOvPropertyMappingRelation();

    /// Destructor.
    virtual ~FdoPostGISOvPropertyMappingRelation();

    /// Creates and optionally attaches an internal class override object.
    ///
    /// \param
    /// attach [in] - 
    ///
    virtual FdoRdbmsOvClassDefinition* CreateInternalClass(bool attach);

};

/// \brief
/// Declaration of smart-pointer type for FdoPostGISOvPropertyMappingRelation.
/// Provided for convenience.
///
typedef FdoPtr<FdoPostGISOvPropertyMappingRelation>
    FdoPostGISOvPropertyMappingRelationP;

#endif // FDOPOSTGISOVPROPERTYMAPPINGRELATION_H
