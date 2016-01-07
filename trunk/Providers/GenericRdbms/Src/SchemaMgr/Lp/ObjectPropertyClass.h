#ifndef FDORDBMSLPGRDOBJECTPROPERTYCLASS_H
#define FDORDBMSLPGRDOBJECTPROPERTYCLASS_H		1
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

#include <Sm/Lp/ObjectPropertyClass.h>
#include <Rdbms/Override/RdbmsOvClassDefinition.h>

// The Object Property Class is a special wrapper class for an object
// property. It wraps around the table containing the object property
// values.

class FdoSmLpGrdObjectPropertyClass : public FdoSmLpObjectPropertyClass
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
	FdoSmLpGrdObjectPropertyClass(
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
	FdoSmLpGrdObjectPropertyClass(
    	FdoSmLpObjectPropertyClassP pBase,
		FdoSmLpObjectPropertyDefinition* pParent, 
		FdoSmLpClassDefinition* pParentType,
		FdoSmLpPropertyMappingDefinition* pMapping,
		FdoSmLpPropertyMappingType mappingType, 
        FdoPhysicalClassMapping* pOverrides
	);

	virtual void Update(
        FdoClassDefinition* pFdoClass,
        FdoSchemaElementState elementState,
        FdoPhysicalSchemaMapping* pOverrides,
        bool bIgnoreStates
    ); 

protected:
    // unused constructor needed only to build Linux
    FdoSmLpGrdObjectPropertyClass() {}
	virtual ~FdoSmLpGrdObjectPropertyClass(void);

    // Determines whether the given property can be inherited by an 
    // object property class. Makes sure that the object property class
    // does not inherit persistent locking properties.
    virtual bool CanInherit( const FdoSmLpPropertyDefinition* pProp );

private:

	// Initializes the list of properties for this class.
	//
	// Parameters:
	//		pParent: the object property for this object property class.
	//		pParentType: the class definition for the object property's type.
	//		pMapping: the table mapping definition for the object property.
	//		mappingType: the table mapping type for the object property.
    //      pOverrides: Fdo to physical mapping overrides.
	void InitProperties (
		FdoSmLpObjectPropertyDefinition* pParent, 
		FdoSmLpClassDefinition* pParentType,
		FdoSmLpPropertyMappingDefinition* pMapping,
	    FdoSmLpPropertyMappingType mappingType,
        FdoRdbmsOvClassDefinition* pOverrides
	);

	// Initializes the list of properties by copying them from a base class.
	//
	// Parameters:
	//		pBase: the class for the base property for the object property
	//			for this class. 
	//		pParent: the object property for this object property class.
	//		pMapping: the table mapping definition for the object property.
	//		mappingType: the table mapping type for the object property.
    //      pOverrides: Fdo to physical mapping overrides.
	void InitProperties (
    	FdoSmLpObjectPropertyClassP pBase,
		FdoSmLpObjectPropertyDefinition* pParent,
	    FdoSmLpPropertyMappingDefinition* pMapping,
	    FdoSmLpPropertyMappingType mappingType,
        FdoRdbmsOvClassDefinition* pOverrides
	);

};

#endif


