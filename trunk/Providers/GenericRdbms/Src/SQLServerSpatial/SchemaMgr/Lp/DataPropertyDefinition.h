#ifndef FDOSMLPSQSDATAPROPERTYDEFINITION_H
#define FDOSMLPSQSDATAPROPERTYDEFINITION_H      1

/*
 * Copyright (C) 2004-2008  Autodesk, Inc.
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

#include "../../../SchemaMgr/Lp/DataPropertyDefinition.h"

// SqlServer Provider's LogicalPhysical DataPropertyDefinition
class FdoSmLpSqsDataPropertyDefinition : public FdoSmLpGrdDataPropertyDefinition
{
public:

    // Construct from a property reader
    FdoSmLpSqsDataPropertyDefinition(FdoSmPhClassPropertyReaderP propReader, FdoSmLpClassDefinition* parent);

	// Construct from an FDO property definition
    FdoSmLpSqsDataPropertyDefinition(
        FdoDataPropertyDefinition* pFdoProp, 
        bool bIgnoreStates,
        FdoSmLpClassDefinition* parent
    );

    // Inherit this property to a subclass. Creates a new instance of this
    // property, which can be attached to the subclass.
    //
    // Parameters:
    //      pSubClass: inherit to this subclass. Must be a subclass of this
    //          property's defining class.

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
    FdoSmLpSqsDataPropertyDefinition(
        FdoPtr<FdoSmLpDataPropertyDefinition> pBaseProperty,
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
    
    // Apply updates to this property.
    virtual void Update(
        FdoPropertyDefinition* pFdoProp,
        FdoSchemaElementState elementState,
        FdoPhysicalPropertyMapping* pPropOverrides,
        bool bIgnoreStates
    ); 

    virtual void Finalize();


    virtual FdoSmPhColumnP NewColumn( 
        FdoSmPhDbObjectP dbObject, 
        FdoStringP columnName,
        bool nullable,
        FdoStringP rootColumnName
    );

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

private:
    FdoStringP mComputedExpression;
};

typedef FdoPtr<FdoSmLpSqsDataPropertyDefinition> FdoSmLpSqsDataPropertyP;

#endif

