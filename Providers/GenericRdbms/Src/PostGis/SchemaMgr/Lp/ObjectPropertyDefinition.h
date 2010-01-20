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
#ifndef FDOSMLPPOSTGISOBJECTPROPERTYDEFINITION_H
#define FDOSMLPPOSTGISOBJECTPROPERTYDEFINITION_H

#include "../../../SchemaMgr/Lp/ObjectPropertyDefinition.h"

/// This is a specialization of PropertyDefinition class and represents
/// containment of a named instance of a class within another class.
/// The contained class must already be defined in the feature schema and
/// cannot be abstract.
/// This class is used by PostGIS provider.
///
class FdoSmLpPostGisObjectPropertyDefinition :
    public FdoSmLpGrdObjectPropertyDefinition
{
public:

	/// Constructs an instance of an ObjectPropertyDefinition from a 
	/// class property reader.
    /// \todo To be documented.
    FdoSmLpPostGisObjectPropertyDefinition(FdoSmPhClassPropertyReaderP reader,
        FdoSmLpClassDefinition* parent);
    
	/// Constructs an instance of a LogicalPhysical ObjectPropertyDefinition
    /// from an FDO ObjectPropertyDefinition.
    /// \todo To be documented.
    FdoSmLpPostGisObjectPropertyDefinition(FdoObjectPropertyDefinition* fdoProp,
        bool ignoreStates,
        FdoSmLpClassDefinition* parent);

	/// Constructs an instance of an ObjectPropertyDefinition from a 
	/// base property.
	/// Used mainly by CreateInherited() and CreateCopy().
    ///
	/// Parameters:
	///		pBaseProperty: initialize the new property from this base property.
	///		pTargetClass: the class that will contain the new property.
	///		logicalName: The name for the new property.
	///			If L"" then the new property has the same name as this
	///			property.
	///		physicalName: not applicable to object properties
	///		bInherit:
	///			True: the new property inherits from the base property.
	///			False: the new property is just a copy of the base property.
    ///      pPropOverrides: Fdo to physical mapping overrides for created property.
    ///
    /// \todo To be documented.
    ///
    FdoSmLpPostGisObjectPropertyDefinition(
        FdoPtr<FdoSmLpObjectPropertyDefinition> base, 
        FdoSmLpClassDefinition* targetClass, 
        FdoStringP logicalName, 
        FdoStringP physicalName, 
        bool inherit,
        FdoPhysicalPropertyMapping* propOverrides = NULL);

    // Add this property's physical mappings to the given class mappings.
    virtual bool AddSchemaMappings(FdoPhysicalClassMappingP mapping,
        bool includeDefaults) const;

    /// Get the Schema Mappings for this object property.
	///
	/// Parameters:
	///		bIncludeDefaults - 
    ///          true: include all schema mappings.
    ///          false: include only non-default mappings.
    /// Returns:
    ///      The Schema Mappings.
    ///      NULL if there are no mappings.
    ///
    /// \todo To be documented.
    ///
    virtual FdoPhysicalPropertyMappingP GetSchemaMappings(
        bool includeDefaults) const;

	/// Create an Object Property Class.
	///
	/// Parameters:
	///		parent: the object property
	///		parentType: the class definition for the object property's type.
	///		pMapping: the table mapping definition for the object property.
	///		mappingType: the table mapping type for the object property.
    ///      pOverrides: Fdo to physical mapping overrides.
    ///
    /// \todo To be documented.
    ///
	virtual FdoSmLpObjectPropertyClassP NewClass(
		FdoSmLpObjectPropertyDefinition* parent, 
		FdoSmLpClassDefinition* parentType,
		FdoSmLpPropertyMappingDefinition* mapping,
		FdoSmLpPropertyMappingType mappingType,
        FdoPhysicalClassMapping* overrides);

	/// Create an Object Property Class from a base class.
    /// This is called when an object property is inherited.
	///
	/// Parameters:
	///		pBase: the base class.
	///		parent: the object property
	///		parentType: the class definition for the object property's type.
	///		pMapping: the table mapping definition for the object property.
	///		mappingType: the table mapping type for the object property.
    ///      pOverrides: Fdo to physical mapping overrides.
    ///
    /// \todo To be documented.
    ///
	virtual FdoSmLpObjectPropertyClassP NewClass(
    	FdoSmLpObjectPropertyClassP base,
		FdoSmLpObjectPropertyDefinition* parent, 
		FdoSmLpClassDefinition* parentType,
		FdoSmLpPropertyMappingDefinition* mapping,
		FdoSmLpPropertyMappingType mappingType, 
        FdoPhysicalClassMapping* overrides);
    
protected:

    /// \todo To be documented.
    ~FdoSmLpPostGisObjectPropertyDefinition();

    /// Create an inherited copy of this property.
    ///
    /// Parameters:
    ///      pSubClass: The class that will inherit the property.
    ///
    /// \todo To be documented.
    ///
    virtual FdoSmLpPropertyP NewInherited(
        FdoSmLpClassDefinition* subClass) const;

    /// Create a non-inherited copy of this property.
    ///
    /// Parameters:
    ///      pTargetClass: The class that will contain the new property.
    ///      logicalName: The name for the new property.
    ///          If L"" then the new property has the same name as this
    ///          property.
    ///      physicalName: the name of the new property's column.
    ///          If L"" then the column is the same as the property name.
    ///      pPropOverrides: Fdo to physical mapping overrides for created property.
    ///
    /// \todo To be documented.
    ///
    virtual FdoSmLpPropertyP NewCopy(FdoSmLpClassDefinition* targetClass,
        FdoStringP logicalName,
        FdoStringP physicalName,
        FdoPhysicalPropertyMapping* propOverrides) const;

    /// Creates a new Object Property Override.
    ///
    /// \todo To be documented.
    ///
    virtual FdoRdbmsOvObjectPropertyDefinition* NewMappingOverrides();

private:

};

/// \brief
/// Declaration of smart-pointer type.
/// Provided for convenience.
///
typedef FdoPtr<FdoSmLpPostGisObjectPropertyDefinition>
    FdoSmLpPostGisObjectPropertyP;

#endif // FDOSMLPPOSTGISOBJECTPROPERTYDEFINITION_H