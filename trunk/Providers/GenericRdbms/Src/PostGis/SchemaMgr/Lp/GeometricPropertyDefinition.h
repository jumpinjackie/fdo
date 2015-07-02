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
#ifndef FDOSMLPPOSTGISGEOMETRICPROPERTYDEFINITION_H
#define FDOSMLPPOSTGISGEOMETRICPROPERTYDEFINITION_H

#include "../../../SchemaMgr/Lp/GeometricPropertyDefinition.h"
#include <Rdbms/Override/PostGis/PostGisOvGeometricPropertyDefinition.h>
#include <Rdbms/Override/PostGis/PostGisOvClassDefinition.h>

// PostGis Provider's LogicalPhysical GeometricPropertyDefinition
class FdoSmLpPostGisGeometricPropertyDefinition :
        public FdoSmLpGrdGeometricPropertyDefinition
{
public:

    /// Construct from a property reader
    /// \todo To be documented
    FdoSmLpPostGisGeometricPropertyDefinition(
        FdoSmPhClassPropertyReaderP reader,
        FdoSmLpClassDefinition* parent);

	/// Construct from an FDO property definition
    /// \todo To be documented
    FdoSmLpPostGisGeometricPropertyDefinition(
        FdoGeometricPropertyDefinition* fdoProp, 
        bool ignoreStates,
        FdoSmLpClassDefinition* parent);

    /// \todo To be documented
    FdoSmLpPostGisGeometricPropertyDefinition(
        FdoPtr<FdoSmLpGeometricPropertyDefinition> baseProperty,
        FdoSmLpClassDefinition* targetClass,
        FdoStringP logicalName,
        FdoStringP physicalName,
        bool inherit,
        FdoPhysicalPropertyMapping* propOverrides = NULL);

    /// Add this property's physical mappings to the given class mappings.
    /// \todo To be documented
    virtual bool AddSchemaMappings(FdoPhysicalClassMappingP schemaMapping,
        bool includeDefaults) const;

    /// Generates the physical mappings for this property.
    /// \todo To be documented
    virtual FdoPhysicalPropertyMappingP GetSchemaMappings(
        bool includeDefaults) const;
    
    /// Generates the column mappings for this property
    /// \todo To be documented
    virtual FdoPostGISOvGeometricColumnP GetColumnMapping(
        bool includeDefaults) const;

    /// \todo To be documented
    virtual void Update(FdoPropertyDefinition* fdoProp,       
        FdoSchemaElementState state,
        FdoPhysicalPropertyMapping* propOverrides,
        bool ignoreStates);

    /// \todo To be documented
	virtual void Update(FdoRdbmsOvGeometricColumn* columnOverrides);

    /// \todo To be documented
    virtual bool CheckGeomPropShapeType(FdoGeometryType type) const;

protected:
    
    /// Create an inherited copy of this property.
    /// \todo To be documented
    virtual FdoSmLpPropertyP NewInherited(
        FdoSmLpClassDefinition* subClass) const;

    /// Create a non-inherited copy of this property.
    /// \todo To be documented
    virtual FdoSmLpPropertyP NewCopy(FdoSmLpClassDefinition* targetClass,
        FdoStringP logicalName,
        FdoStringP physicalName,
        FdoPhysicalPropertyMapping* propOverrides) const;

};

/// \brief
/// Declaration of smart-pointer type.
/// Provided for convenience.
///
typedef FdoPtr<FdoSmLpPostGisGeometricPropertyDefinition> 
    FdoSmLpPostGisGeometricPropertyP;

#endif // FDOSMLPPOSTGISGEOMETRICPROPERTYDEFINITION_H
