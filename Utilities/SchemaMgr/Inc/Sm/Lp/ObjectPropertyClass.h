#ifndef FDORDBMSLPOBJECTPROPERTYCLASS_H
#define FDORDBMSLPOBJECTPROPERTYCLASS_H		1
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

#include <Sm/Lp/Class.h>
#include <Sm/Lp/ObjectPropertyDefinition.h>
#include <Sm/Lp/PropertyMappingType.h>

// The Object Property Class is a special wrapper class for an object
// property. It wraps around the table containing the object property
// values.

class FdoSmLpObjectPropertyClass : public FdoSmLpClass
{
public:
    /// Returns the local Id property for the corresponding object property.
    /// The local id distinguishes the members of a collection object property.
    /// NULL if this property does not exist. 
	const FdoSmLpDataPropertyDefinition* RefLocalIdProperty() const;
	FdoSmLpDataPropertyP GetLocalIdProperty();

    /// Get the source properties. Source and target properties are used to 
    /// join the containing class and object property tables. The 
    /// source properties are the 
    /// properties in the object property containing class which corresponding
    /// to the target properties in the object property's table.
    /// Return value is always valid (never null).
	const FdoSmLpDataPropertyDefinitionCollection* RefSourceProperties() const;
	FdoSmLpDataPropertiesP GetSourceProperties();

    /// Get the target properties. The target properties are the 
    /// properties in the object property table which corresponding
    /// to the source properties in the object property's containing class.
    /// Return value is always valid (never null).
	const FdoSmLpDataPropertyDefinitionCollection* RefTargetProperties() const;
	FdoSmLpDataPropertiesP GetTargetProperties();
	
    /// Get this class's object property.
	const FdoSmLpObjectPropertyDefinition* RefObjectProperty() const;

    /// Get this class's object property.
	FdoSmLpObjectPropertyP GetObjectProperty();

    /// Get the class that contains this class's object property.
	const FdoSmLpClassDefinition* RefContainingClass() const;

    virtual void Commit();

    /// Write out any object property attributes defined at the object property
    /// class level.
    virtual void WriteDb( FdoSmPhPropertyWriterP pWriter ) const;

protected:
    //Unused constructor needed only to build on Linux
    FdoSmLpObjectPropertyClass() {}

    /// Create an Object Property Class.
	//
    /// Parameters:
    /// 	pParent: the object property
    /// 	pParentType: the class definition for the object property's type.
    /// 	pMapping: the table mapping definition for the object property.
    /// 	mappingType: the table mapping type for the object property.
    ///      pOverrides: Fdo to physical mapping overrides.
	FdoSmLpObjectPropertyClass(
		FdoSmLpObjectPropertyDefinition* pParent, 
		FdoSmLpClassDefinition* pParentType,
		FdoSmLpPropertyMappingDefinition* pMapping,
		FdoSmLpPropertyMappingType mappingType,
        FdoPhysicalClassMapping* pOverrides
	);

    /// Create an Object Property Class from a base class. This is called when 
    /// an object property is inherited.
	//
    /// Parameters:
    /// 	pBase: the base class.
    /// 	pParent: the object property
    /// 	pParentType: the class definition for the object property's type.
    /// 	pMapping: the table mapping definition for the object property.
    /// 	mappingType: the table mapping type for the object property.
    ///      pOverrides: Fdo to physical mapping overrides.
	FdoSmLpObjectPropertyClass(
		FdoPtr<FdoSmLpObjectPropertyClass> pBase,
		FdoSmLpObjectPropertyDefinition* pParent, 
		FdoSmLpClassDefinition* pParentType,
		FdoSmLpPropertyMappingDefinition* pMapping,
		FdoSmLpPropertyMappingType mappingType, 
        FdoPhysicalClassMapping* pOverrides
	);

	virtual ~FdoSmLpObjectPropertyClass(void);

	const FdoSmPhDependency* RefDependency() const;

    /// Returns true if this class created its table. If the class created the table then
    /// the table will be dropped when the class is deleted.
    virtual bool IsTableCreator() const;
    
    /// Initializes the list of nested properties for this class.
	//
    /// Parameters:
    /// 	pParent: the object property for this object property class.
    /// 	pParentType: the class definition for the object property's type.
    /// 	mappingType: the table mapping type for the object property.
	void InitNestedProperties (
		const FdoSmLpObjectPropertyDefinition* pParent, 
		const FdoSmLpClassDefinition* pParentType,
		FdoSmLpPropertyMappingType mappingType 
	);

    /// Initializes the list of nested properties by copying them from a base class.
	//
    /// Parameters:
    /// 	pBase: the class for the base property for the object property
    /// 		for this class. 
	void InitNestedProperties (
		const FdoSmLpObjectPropertyClass* pBase
	);

    /// Initializes the local id property when the object property is a
    /// collection.
	//
    /// Parameters:
    /// 	pParent: the object property for this object property class.
    /// 	pParentType: the class definition for the object property's type.
    /// 	mappingType: the table mapping type for the object property.
	void InitLocalIdProperty (
		const FdoSmLpObjectPropertyDefinition* pParent, 
		const FdoSmLpClassDefinition* pParentType,
		FdoSmLpPropertyMappingType mappingType 
	);

    /// Initializes the local id property by copying it from a base class.
	//
    /// Parameters:
    /// 	pBase: the class for the base property for the object property
    /// 		for this class. 
	void InitLocalIdProperty (
		const FdoSmLpObjectPropertyClass* pBase
	);

    /// Initializes the identity properties for this class.
	//
    /// Parameters:
    /// 	pParent: the object property for this object property class.
    /// 	pParentType: the class definition for the object property's type.
    /// 	pMapping: the table mapping definition for the object property.
    /// 	mappingType: the table mapping type for the object property.
	void InitIdProperties (
		const FdoSmLpObjectPropertyDefinition* pParent, 
		const FdoSmLpClassDefinition* pParentType,
		FdoSmLpPropertyMappingDefinition* pMapping,
		FdoSmLpPropertyMappingType mappingType 
	);

    /// Initializes the identity properties by copying them from a base class.
	//
    /// Parameters:
    /// 	pBase: the class for the base property for the object property
    /// 		for this class. 
	void InitIdProperties (
		const FdoSmLpObjectPropertyClass* pBase
	);

    /// Sets up table location.
	//
    /// Parameters:
    /// 	pParent: the object property for this object property class.
    void InitTable(
		const FdoSmLpObjectPropertyDefinition* pParent
    );

    /// Generates a name for this Object Property class. The name is
    /// "[containing class name].[object property name]".
	//
    /// Parameters:
    /// 	pParent: the object property for this object property class.
	FdoStringP MakeName( const FdoSmLpObjectPropertyDefinition* pParent );

    /// Finalize this class. Creates a link from the table for this class 
    /// (corresponding Object Property's containing table) and the table for the
    /// Object Property's containing class.
	virtual void PostFinalize();

	void AddJoinMismatchError(	
		const FdoSmLpObjectPropertyDefinition* pParent
	);

	void AddSourcePropertyMissingError(
		const FdoSmLpObjectPropertyDefinition* pParent,
		FdoStringP sourceColumn
	);

	void AddTargetColumnMissingError
	(
		const FdoSmLpObjectPropertyDefinition* pParent,
		const FdoSmLpDataPropertyDefinition* pProp
	);

	void AddTargetConflictError
	(
		const FdoSmLpObjectPropertyDefinition* pParent,
		const FdoSmLpDataPropertyDefinition* pProp,
		const FdoSmLpClassDefinition* pParentType
	);

	void AddTargetPropertyMissingError(
		const FdoSmLpObjectPropertyDefinition* pParent,
		FdoStringP targetColumn
	);

private:
    /// Various helpers for logging errors.

	void AddIdPropNotFoundError(	
		const FdoSmLpObjectPropertyDefinition* pParent,
		const FdoSmLpDataPropertyDefinition* pProp
	);

	void AddOrderByMissingError
	(
		const FdoSmLpObjectPropertyDefinition* pParent,
		const FdoSmLpClassDefinition* pParentType,
		const wchar_t* orderByColumn
	);

	void AddCollectionIDError
	(
		const FdoSmLpObjectPropertyDefinition* pParent
	);

	FdoSmLpDataPropertyP mpLocalIdProperty;

	FdoSmLpDataPropertiesP mSourceProperties;
	FdoSmLpDataPropertiesP mTargetProperties;

	const FdoSmLpClassDefinition* mpContainingClass;

    /// Back-pointer, no reference held
	FdoSmLpObjectPropertyDefinition* mpObjectProperty;

	FdoSmLpPropertyMappingType mMappingType;

	const FdoSmPhDependency* mpDependency;

};

typedef FdoPtr<FdoSmLpObjectPropertyClass> FdoSmLpObjectPropertyClassP;

#endif


