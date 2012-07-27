#ifndef FDOSMLPMYSQLGEOMETRICPROPERTYDEFINITION_H
#define FDOSMLPMYSQLGEOMETRICPROPERTYDEFINITION_H     1
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

#include "../../../SchemaMgr/Lp/GeometricPropertyDefinition.h"
#include <Rdbms/Override/MySQL/MySqlOvGeometricPropertyDefinition.h>
#include <Rdbms/Override/MySQL/MySqlOvClassDefinition.h>

// MySql Provider's LogicalPhysical GeometricPropertyDefinition
class FdoSmLpMySqlGeometricPropertyDefinition : public FdoSmLpGrdGeometricPropertyDefinition
{
public:

    // Construct from a property reader
    FdoSmLpMySqlGeometricPropertyDefinition(FdoSmPhClassPropertyReaderP propReader, FdoSmLpClassDefinition* parent);

	// Construct from an FDO property definition
    FdoSmLpMySqlGeometricPropertyDefinition(
        FdoGeometricPropertyDefinition* pFdoProp, 
        bool bIgnoreStates,
        FdoSmLpClassDefinition* parent
    );

    // Constructs an instance of a DataPropertyDefinition from the given parameters.
    // Used mainly by CreateInherited() and CreateCopy().
    //
    // Parameters:
    //      pBaseProperty: initialize the new property from this base property.
    //      pTargetClass: the class that will contain the new property.
    //      logicalName: The name for the new property.
    //          If L"" then the new property has the same name as this
    //          property.
    //      physicalName: the name of the new property's column.
    //          If L"" then the column is the same as the base property column name.
    //      bInherit:
    //          True: the new property inherits from the base property.
    //          False: the new property is just a copy of the base property.
    //      pPropOverrides: Fdo to physical mapping overrides for created property.
    FdoSmLpMySqlGeometricPropertyDefinition(
        FdoPtr<FdoSmLpGeometricPropertyDefinition> pBaseProperty,
        FdoSmLpClassDefinition* pTargetClass,
        FdoStringP logicalName,
        FdoStringP physicalName,
        bool bInherit,
        FdoPhysicalPropertyMapping* pPropOverrides = NULL
    );

    // Add this property's physical mappings to the given class mappings
    virtual bool AddSchemaMappings( FdoPhysicalClassMappingP schemaMapping, bool bIncludeDefaults ) const;

    // Generates the physical mappings for this property
    virtual FdoPhysicalPropertyMappingP GetSchemaMappings( bool bIncludeDefaults ) const;
    
    // Generates the column mappings for this property
    virtual FdoMySQLOvGeometricColumnP GetColumnMapping( bool bIncludeDefaults ) const;

    virtual void Update( FdoPropertyDefinition* pFdoProp,       
        FdoSchemaElementState elementState,
        FdoPhysicalPropertyMapping* pPropOverrides,
        bool bIgnoreStates);

	virtual void Update(
        FdoRdbmsOvGeometricColumn* pColumnOverrides
    )
    {
        FdoSmLpGrdGeometricPropertyDefinition::Update(pColumnOverrides);
    }

    virtual bool CheckGeomPropShapeType (FdoGeometryType geomType) const;

protected:
    // Create an inherited copy of this property.
    //
    // Parameters:
    //      pSubClass: The class that will inherit the property.
    virtual FdoSmLpPropertyP NewInherited( FdoSmLpClassDefinition* pSubClass ) const;

    // Create a non-inherited copy of this property.
    //
    // Parameters:
    //      pTargetClass: The class that will contain the new property.
    //      logicalName: The name for the new property.
    //          If L"" then the new property has the same name as this
    //          property.
    //      physicalName: the name of the new property's column.
    //          If L"" then the column is the same as the property name.
    //      pPropOverrides: Fdo to physical mapping overrides for created property.
    virtual FdoSmLpPropertyP NewCopy(
        FdoSmLpClassDefinition* pTargetClass,
        FdoStringP logicalName,
        FdoStringP physicalName,
        FdoPhysicalPropertyMapping* pPropOverrides
    ) const;

};

typedef FdoPtr<FdoSmLpMySqlGeometricPropertyDefinition> FdoSmLpMySqlGeometricPropertyP;

#endif
