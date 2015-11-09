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
#ifndef FDOPOSTGISOVOBJECTPROPERTYDEFINITION_H
#define FDOPOSTGISOVOBJECTPROPERTYDEFINITION_H

#include <Fdo/Commands/Schema/PhysicalElementMapping.h>
#include <Rdbms/Override/RdbmsOvObjectPropertyDefinition.h>
#include <Rdbms/Override/PostGis/PostGisOv.h>
#include <Rdbms/Override/PostGis/PostGisOvPropertyMappingDefinition.h>
#include <Rdbms/Override/PostGis/PostGisOvPropertyDefinition.h>
#include <Rdbms/Override/PostGis/PostGisOvPropertyMappingClass.h>
#include <Rdbms/Override/PostGis/PostGisOvPropertyMappingConcrete.h>
#include <Rdbms/Override/PostGis/PostGisOvPropertyMappingSingle.h>

/// \brief
/// Concrete class defining physical schema overrides for an object
/// property definition.
/// This class is used in PostGIS schema overrides.
/// 
class FdoPostGISOvObjectPropertyDefinition :
    public FdoRdbmsOvObjectPropertyDefinition,
    public FdoPostGISOvPropertyDefinition
{
public:

    /// \brief
    /// Constructs a default of an FdoPostGISOvObjectPropertyDefinition.
    /// 
    /// \return
    /// Returns FdoPostGISOvObjectPropertyDefinition
    /// 
    FDOPOSTGIS_OV_API static FdoPostGISOvObjectPropertyDefinition* Create();

    /// \brief
    /// Constructs an instance of an FdoPostGISOvObjectPropertyDefinition
    /// using the specified arguments.
    /// 
    /// \param name 
    /// Input name
    /// 
    /// \return
    /// Returns FdoPostGISOvObjectPropertyDefinition
    /// 
    FDOPOSTGIS_OV_API static FdoPostGISOvObjectPropertyDefinition* Create(
        FdoString* name);

    /// \brief
    /// Gets the mapping definition
    /// 
    /// \return
    /// Returns the mapping definition
    /// 
    FDOPOSTGIS_OV_API FdoPostGISOvPropertyMappingDefinition* GetMappingDefinition();

    /// \brief
    /// Sets the mapping definition
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDOPOSTGIS_OV_API void SetMappingDefinition(
        FdoPostGISOvPropertyMappingDefinition* mappingDef);

protected:

    FdoPostGISOvObjectPropertyDefinition();
    
    FdoPostGISOvObjectPropertyDefinition(FdoString* name);
    
    virtual ~FdoPostGISOvObjectPropertyDefinition();
    
    virtual void Dispose();

    /// Create and optionally attach a Single table property mapping.
    virtual FdoRdbmsOvPropertyMappingSingle* CreateSingleMapping(bool attach);

    /// Create and optionally attach a Class table property mapping.
    virtual FdoRdbmsOvPropertyMappingClass* CreateClassMapping(bool attach);

    /// Create and optionally attach a Concrete table property mapping.
    virtual FdoRdbmsOvPropertyMappingConcrete* CreateConcreteMapping(bool attach);

private:

};


/// \brief
/// Declaration of smart-pointer type for FdoPostGISOvColumn.
/// Provided for convenience.
///
typedef FdoPtr<FdoPostGISOvObjectPropertyDefinition> FdoPostGISOvObjectPropertyP;

#endif // FDOPOSTGISOVOBJECTPROPERTYDEFINITION_H
