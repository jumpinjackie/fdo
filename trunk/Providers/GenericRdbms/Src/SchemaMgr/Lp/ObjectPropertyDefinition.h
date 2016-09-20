#ifndef FDOSMLPGRDOBJECTPROPERTYDEFINITION_H
#define FDOSMLPGRDOBJECTPROPERTYDEFINITION_H		1
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

#include <Sm/Lp/PropertyMappingConcrete.h>
#include <Sm/Lp/PropertyMappingSingle.h>
#include <Sm/Lp/ObjectPropertyDefinition.h>
#include <Sm/Lp/ObjectPropertyClass.h>
#include <Rdbms/Override/RdbmsOvPropertyMappingConcrete.h>
#include <Rdbms/Override/RdbmsOvPropertyMappingSingle.h>
#include <Rdbms/Override/RdbmsOvObjectPropertyDefinition.h>
#include <Rdbms/Override/RdbmsOvTable.h>
#include <Rdbms/Override/RdbmsOvClassDefinition.h>

// ObjectPropertyDefinition derives from PropertyDefinition and represents
// containment of a named instance of a class within another class. The
// contained class must already be defined in the feature schema and cannot be
// abstract.
class FdoSmLpGrdObjectPropertyDefinition : public FdoSmLpObjectPropertyDefinition
{
  public:
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

	// Copy updates from corresponding FDO property. 
    //
    // Parameters
    //      pFdoProp: the FDO element.
    //      elementState: the modification state for this object property
    //      pPropOverrides: Fdo to physical mapping overrides.
    //      bIgnoreStates: true if the element state on the FDO element is 
    //          to be ignored.
	virtual void Update(
        FdoPropertyDefinition* pFdoProp,
        FdoSchemaElementState elementState,
        FdoPhysicalPropertyMapping* pPropOverrides,
        bool bIgnoreStates
    ); 

	// Copy updates from corresponding FDO property. 
    //
    // Parameters
    //      pPropOverrides: Fdo to physical mapping overrides.
	virtual void Update(
        FdoPhysicalPropertyMapping* pPropOverrides
    ); 

protected:
    // unused constructor needed only to build Linux
    FdoSmLpGrdObjectPropertyDefinition() {}

	~FdoSmLpGrdObjectPropertyDefinition(void);

    // Get the overridden table name, L"" if not overridden
    virtual FdoStringP GetOvTableName();

    // Reads the single table mapping prefix from the given property
    // reader ( from the columnname field). 

    void ReadPrefix( 
        FdoSmPhClassPropertyReaderP propReader,
        FdoRdbmsOvObjectPropertyP propOverride // used as a factory to create
                                               // a single mapping overrride.
    );

    // Various Object Property table mapping creators.
    virtual FdoSmLpPropertyMappingSingleP NewPropertyMappingSingle( 
        const FdoSmLpPropertyMappingSingle* pBaseMapping,
        FdoRdbmsOvPropertyMappingSingle* pMappingOverrides
    );
    virtual FdoSmLpPropertyMappingSingleP NewPropertyMappingSingle(
        FdoRdbmsOvPropertyMappingSingle* pMappingOverrides
    );
    virtual FdoSmLpPropertyMappingConcreteP NewPropertyMappingConcrete( 
        const FdoSmLpPropertyMappingConcrete* pBaseMapping,
        FdoRdbmsOvPropertyMappingConcrete* pMappingOverrides
    );
    virtual FdoSmLpPropertyMappingConcreteP NewPropertyMappingConcrete(
        FdoRdbmsOvPropertyMappingConcrete* pMappingOverrides
    );

    FdoSmPhDbObjectP NewTable( 
        FdoSmPhOwnerP owner,
        FdoString* tableName
    );

    FdoSmLpGrdObjectPropertyDefinition(
        FdoSmPhClassPropertyReaderP propReader, 
        FdoSmLpClassDefinition* parent
    );
    
    FdoSmLpGrdObjectPropertyDefinition(
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
    FdoSmLpGrdObjectPropertyDefinition(
        FdoSmLpObjectPropertyP pBaseProperty, 
        FdoSmLpClassDefinition* pTargetClass, 
        FdoStringP logicalName, 
        FdoStringP physicalName, 
        bool bInherit,
        FdoPhysicalPropertyMapping* pPropOverrides = NULL
    );

    FdoRdbmsOvPropertyMappingP GetMappingOverrides();
    void SetMappingOverrides( FdoRdbmsOvPropertyMappingP mappingOverrides );

	// Set the table mapping type to Concrete.
	virtual void SetConcreteMapping();
    virtual void SetSingleMapping();

    // Creates a new object property schema override.
    // Must be overridden by all providers that support object properties.
    virtual FdoRdbmsOvObjectPropertyDefinition* NewMappingOverrides();

    // Set the generic parts of the schema overrides.
    virtual bool SetSchemaMappings( FdoPhysicalPropertyMappingP classMapping, bool bIncludeDefaults ) const;

private:
    FdoRdbmsOvClassP mOpOvClass;
    FdoStringP mOvTableName;
    bool mFixedDefaultName;
    FdoRdbmsOvPropertyMappingP mMappingOverrides;
};

typedef FdoPtr<FdoSmLpGrdObjectPropertyDefinition> FdoSmLpGrdObjectPropertyP;

#endif


