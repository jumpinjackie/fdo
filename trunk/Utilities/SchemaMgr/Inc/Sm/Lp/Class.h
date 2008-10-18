#ifndef FDOSMLPCLASS_H
#define FDOSMLPCLASS_H		1
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
#include <Sm/Lp/ClassDefinition.h>

// Class derives from ClassObject and can be used to represent any type of
// non-spatial data within a LogicalPhysical schema. Typically classes are used in the
// LogicalPhysical schema to define complex properties of other feature types by
// containment. However they can also be used at global scope and hence
// instances can be created, deleted, updated, and selected through FDO
// commands. The Class type does not pre-define any base properties.
class FdoSmLpClass : public FdoSmLpClassDefinition
{
  public:

    virtual ~FdoSmLpClass();

    /// Returns the concrete class type, ClassType.Class.
    virtual FdoClassType GetClassType() const;

    /// Convenience function for casting a ClassDefinition to a Class class..
    /// Returns NULL if the ClassDefinition is not a Class class.
	static const FdoSmLpClass* Cast( const FdoSmLpClassDefinition* src )
	{
		return( (src  && (src->GetClassType() == FdoClassType_Class)) ? 
					(const FdoSmLpClass*) src : NULL
		);
	}

    // Non-feature classes can have only have geometry where there is no MetaSchema,
    // in which case, they turn into feature classes if geometric properties are added
    // to them.
    virtual bool SupportsGeometry() const { return !GetHasMetaSchema();} 


protected:
    //Unused constructor needed only to build on Linux
    FdoSmLpClass() {}

    /// Constructs a LogicalPhysical non-feature class from a class reader
    FdoSmLpClass(FdoSmPhClassReaderP classReader, FdoSmLpSchemaElement* parent);

    FdoSmLpClass(
        FdoClassDefinition* pFdoClass, 
        bool bIgnoreStates,
        FdoSmLpSchemaElement* parent
    );

    /// Constructs an instance of a non-feature Class.
    /// Used mainly for copying class class objects.
	//
    /// Parameters:
    /// 	name: class name.
    /// 	description: class description.
    /// 	parent: the parent element ( usually an FdoSmLpSchema ).
    /// 	pSrcClass: When this is an object property class, pSrcClass represents the 
    /// 		object property class for the associated object property's base property.
    /// 	tableMapping: Property to table mapping type.
    /// 	tableName: class table name.
    /// 	table: class table. 
    /// 	elementState: the modification state of the class.
	FdoSmLpClass(
		FdoString* name, 
		FdoString* description,
		FdoSmLpSchemaElement* parent,
		FdoSmLpClassDefinitionP pSrcClass,
		FdoSmOvTableMappingType tableMapping,
		FdoString* tableName,
		FdoSmPhDbObjectP table,
		FdoSchemaElementState elementState
	);
};

#endif


