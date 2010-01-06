#ifndef FDORDBMSLPSQSOBJECTPROPERTYCLASS_H
#define FDORDBMSLPSQSOBJECTPROPERTYCLASS_H		1

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

#include "../../../SchemaMgr/Lp/ObjectPropertyClass.h"
#include "ClassDefinition.h"

// The Object Property Class is a special wrapper class for an object
// property. It wraps around the table containing the object property
// values.

class FdoSmLpSqsObjectPropertyClass : public FdoSmLpGrdObjectPropertyClass, public FdoSmLpSqsClassDefinition
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
	FdoSmLpSqsObjectPropertyClass(
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
	FdoSmLpSqsObjectPropertyClass(
    	FdoSmLpObjectPropertyClassP pBase,
		FdoSmLpObjectPropertyDefinition* pParent, 
		FdoSmLpClassDefinition* pParentType,
		FdoSmLpPropertyMappingDefinition* pMapping,
		FdoSmLpPropertyMappingType mappingType, 
        FdoPhysicalClassMapping* pOverrides
	);

    // Apply updates to this object property class. 
	virtual void Update(
        FdoClassDefinition* pFdoClass,
        FdoSchemaElementState elementState,
        FdoPhysicalSchemaMapping* pOverrides,
        bool bIgnoreStates
    ); 

protected:
	virtual ~FdoSmLpSqsObjectPropertyClass(void);

    virtual void PostFinalize();

private:

};

#endif
