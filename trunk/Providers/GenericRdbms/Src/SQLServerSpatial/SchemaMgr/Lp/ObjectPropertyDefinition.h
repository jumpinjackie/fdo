#ifndef FDOSMLPSQSOBJECTPROPERTYDEFINITION_H
#define FDOSMLPSQSOBJECTPROPERTYDEFINITION_H		1
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


#ifdef _WIN32
#pragma once
#endif

#include "../../../SchemaMgr/Lp/ObjectPropertyDefinition.h"

// ObjectPropertyDefinition derives from PropertyDefinition and represents
// containment of a named instance of a class within another class. The
// contained class must already be defined in the feature schema and cannot be
// abstract.
class FdoSmLpSqsObjectPropertyDefinition : public FdoSmLpGrdObjectPropertyDefinition
{
public:
	// Constructs an instance of an ObjectPropertyDefinition from a 
	// class property reader
    FdoSmLpSqsObjectPropertyDefinition(FdoSmPhClassPropertyReaderP propReader, FdoSmLpClassDefinition* parent);
    
	// Constructs an instance of a LogicalPhysical ObjectPropertyDefinition from an
	// FDO ObjectPropertyDefinition
    FdoSmLpSqsObjectPropertyDefinition(
        FdoObjectPropertyDefinition* pFdoProp, 
        bool bIgnoreStates,
        FdoSmLpClassDefinition* parent
    );

	// Constructs an instance of an ObjectPropertyDefinition from a 
	// base property.
	// Used mainly by CreateInherited() and CreateCopy().
    //
	// Parameters:
	//		pBaseProperty: initialize the new property from this base property.
	//		pTargetClass: the class that will contain the new property.
	//		logicalName: The name for the new property.
	//			If L"" then the new property has the same name as this
	//			property.
	//		physicalName: not applicable to object properties
	//		bInherit:
	//			True: the new property inherits from the base property.
	//			False: the new property is just a copy of the base property.
    //      pPropOverrides: Fdo to physical mapping overrides for created property.
    FdoSmLpSqsObjectPropertyDefinition(
        FdoPtr<FdoSmLpObjectPropertyDefinition> pBaseProperty, 
        FdoSmLpClassDefinition* pTargetClass, 
        FdoStringP logicalName, 
        FdoStringP physicalName, 
        bool bInherit,
        FdoPhysicalPropertyMapping* pPropOverrides = NULL
    );

    // Add this property's physical mappings to the given class mappings
    virtual bool AddSchemaMappings( FdoPhysicalClassMappingP schemaMapping, bool bIncludeDefaults ) const;

    // Get the Schema Mappings for this object property
	//
	// Parameters:
	//		bIncludeDefaults - 
    //          true: include all schema mappings.
    //          false: include only non-default mappings.
    // Returns:
    //      The Schema Mappings.
    //      NULL if there are no mappings.
    virtual FdoPhysicalPropertyMappingP GetSchemaMappings( bool bIncludeDefaults ) const;

	// Create an Object Property Class.
	//
	// Parameters:
	//		pParent: the object property
	//		pParentType: the class definition for the object property's type.
	//		pMapping: the table mapping definition for the object property.
	//		mappingType: the table mapping type for the object property.
    //      pOverrides: Fdo to physical mapping overrides.
	virtual FdoSmLpObjectPropertyClassP NewClass(
		FdoSmLpObjectPropertyDefinition* pParent, 
		FdoSmLpClassDefinition* pParentType,
		FdoSmLpPropertyMappingDefinition* pMapping,
		FdoSmLpPropertyMappingType mappingType,
        FdoPhysicalClassMapping* pOverrides
	);

	// Create an Object Property Class from a base class. This is called when 
	// an object property is inherited.
	//
	// Parameters:
	//		pBase: the base class.
	//		pParent: the object property
	//		pParentType: the class definition for the object property's type.
	//		pMapping: the table mapping definition for the object property.
	//		mappingType: the table mapping type for the object property.
    //      pOverrides: Fdo to physical mapping overrides.
	virtual FdoSmLpObjectPropertyClassP NewClass(
    	FdoSmLpObjectPropertyClassP pBase,
		FdoSmLpObjectPropertyDefinition* pParent, 
		FdoSmLpClassDefinition* pParentType,
		FdoSmLpPropertyMappingDefinition* pMapping,
		FdoSmLpPropertyMappingType mappingType, 
        FdoPhysicalClassMapping* pOverrides
	);

    // Creates a new object property table
    virtual FdoSmPhDbObjectP NewTable( 
        FdoSmPhOwnerP owner,
        FdoString* tableName
    );

/*    
	// Apply Schema Overrides to this property 
    //
    // Parameters
    //      pPropOverrides: Fdo to physical mapping overrides.
    virtual void Update(
        FdoPhysicalPropertyMapping* pPropOverrides
    ); 
*/
protected:
	~FdoSmLpSqsObjectPropertyDefinition(void);

    virtual void FinalizeTableOverrides();

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

    // Creates a new Object Property Override.
    virtual FdoRdbmsOvObjectPropertyDefinition* NewMappingOverrides();

private:
};

typedef FdoPtr<FdoSmLpSqsObjectPropertyDefinition> FdoSmLpSqsObjectPropertyP;

#endif
