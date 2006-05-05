#ifndef FDOSMLPGRDGEOMETRICPROPERTYDEFINITION_H
#define FDOSMLPGRDGEOMETRICPROPERTYDEFINITION_H		1
/*
 * Copyright (C) 2004-2006  Autodesk, Inc.
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of version 2.1 of the GNU Lesser
 * General Public License as published by the Free Software Foundation.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#ifdef _WIN32
#pragma once
#endif

#include <Sm/Lp/GeometricPropertyDefinition.h>
#include <Rdbms/Override/RdbmsOvGeometricPropertyDefinition.h>

// GeometricPropertyDefinition derives from SchemaObject and defines a
// geometric representation of a SpatialFeatureClass. Geometric properties can
// take on any of the geometry types listed in the GeometryType enumeration.
// Note that geometric properties do not actually store the coordinate system
// or measure units, this is to enable the use of the same schema in different
// geometric locals. TODO: How is the coordinate system and measure unit
// defined for a given connection/database/file? Can this be standardized or is
// it provider specific.
class FdoSmLpGrdGeometricPropertyDefinition : public FdoSmLpGeometricPropertyDefinition
{
public:
	// Update from generic class overrides
    //
    // Parameters
    //      pFdoProp: the FDO element.
    //      elementState: the modification state for this data property.
    //      pClassOverrides: the class overrides
    //      bIgnoreStates: true if the element state on the FDO element is 
    //          to be ignored.
	virtual void Update(
        FdoPropertyDefinition* pFdoProp,
        FdoSchemaElementState elementState,
        FdoPhysicalClassMapping* pClassOverrides,
        bool bIgnoreStates
    ); 

	// Update from generic property overrides
    //
    // Parameters
    //      pFdoProp: the FDO element.
    //      elementState: the modification state for this data property.
    //      pPropOverrides: the property overrides
    //      bIgnoreStates: true if the element state on the FDO element is 
    //          to be ignored.
	virtual void Update(
        FdoPropertyDefinition* pFdoProp,
        FdoSchemaElementState elementState,
        FdoPhysicalPropertyMapping* pPropOverrides,
        bool bIgnoreStates
    ); 

	// Update from generic property overrides
    //
    // Parameters
    //      pPropOverrides: the property overrides
	virtual void Update(
        FdoPhysicalPropertyMapping* pPropOverrides
    ); 

	// Update from generic column overrides
    //
    // Parameters
    //      pColumnOverrides: Fdo to physical mapping overrides.
	virtual void Update(
        FdoRdbmsOvGeometricColumn* pColumnOverrides
    ); 
    
protected:
    // unused constructor needed only to build Linux
    FdoSmLpGrdGeometricPropertyDefinition() {}
    FdoSmLpGrdGeometricPropertyDefinition(FdoSmPhClassPropertyReaderP propReader, FdoSmLpClassDefinition* parent);

    FdoSmLpGrdGeometricPropertyDefinition::FdoSmLpGrdGeometricPropertyDefinition(
        FdoGeometricPropertyDefinition* pFdoProp, 
        bool bIgnoreStates,
        FdoSmLpClassDefinition* parent
    );

    FdoSmLpGrdGeometricPropertyDefinition::FdoSmLpGrdGeometricPropertyDefinition(
        FdoSmLpGeometricPropertyP pBaseProperty,
        FdoSmLpClassDefinition* pTargetClass,
        FdoStringP logicalName,
        FdoStringP physicalName,
        bool bInherit,
        FdoPhysicalPropertyMapping* pPropOverrides
    );

private:
};

typedef FdoPtr<FdoSmLpGrdGeometricPropertyDefinition> FdoSmLpGrdGeometricPropertyP;

#endif
