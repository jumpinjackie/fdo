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

#pragma  once

#include "FDO\Schema\mgSchemaElement.h"
#include "FDO\Schema\mgClassType.h"
#include "FDO\Schema\mgUniqueConstraintCollection.h"

class FdoClassDefinition;

BEGIN_NAMESPACE_OSGEO_FDO_SCHEMA
public __gc class PropertyDefinitionCollection;
public __gc class ReadOnlyPropertyDefinitionCollection;
public __gc class DataPropertyDefinitionCollection;
public __gc class ReadOnlyDataPropertyDefinitionCollection;
public __gc class ClassCapabilities;

/// \brief
/// ClassDefinition is an abstract class that derives from SchemaElement.
/// ClassDefinition is a base class for all concrete class types (e.g., FdoClass,
/// FeatureClass) in the Schema package.
public __gc class ClassDefinition : public NAMESPACE_OSGEO_FDO_SCHEMA::SchemaElement
{
public:
    /// \brief
    /// This is an abstract operation that must be implemented by derived classes to return
    /// the concrete class type.
    /// 
    /// \return
    /// Returns the ClassType
    /// 
	__property NAMESPACE_OSGEO_FDO_SCHEMA::ClassType get_ClassType();

    /// \brief
    /// Gets the base ClassDefinition this class is derived from.
    /// 
    /// \return
    /// Returns the base ClassDefinition
    /// 
	__property NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition* get_BaseClass();

    /// \brief
    /// Sets the base ClassDefinition this class is derived from.
    /// 
    /// \param value 
    /// Input ClassDefinition this class is derived from
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_BaseClass(NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition* value);

    /// \brief
    /// Gets the abstraction state of ClassDefinition. Classes marked as abstract
    /// cannot be created via FDO commands and cannot be used as properties of
    /// another class.
    /// 
    /// \return
    /// Returns true if the ClassDefinition is abstract
    /// 
	__property System::Boolean get_IsAbstract();

    /// \brief
    /// Sets the abstraction state of ClassDefinition. Classes marked as abstract
    /// cannot be created via FDO commands and cannot be used as properties of
    /// another class.
    /// 
    /// \param value 
    /// Input the abstraction state
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_IsAbstract(System::Boolean value);

    /// \brief
    /// Sets the base properties of this class.  All elements of the 
    /// input collection must also be properties of the class.
    /// 
    /// \param value 
    /// Input collection of PropertyDefinition values
    /// 
    /// \return
    /// Returns nothing
    /// 

    /// TODO: Should be changed into property. But this will raise a problem
    /// because of the difference between the return type of getter and the parameter type of setter.
	System::Void SetBaseProperties(NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinitionCollection* value);

    /// \brief
    /// Returns an ReadOnlyPropertyDefinitionCollection containing the properties
    /// defined by base classes or by the concrete class type, itself. This
    /// collection is only populated when ClassDefinition is returned by the
    /// DescribeSchema command.
    /// 
    /// \return
    /// Returns ReadOnlyPropertyDefinitionCollection
    /// 
	NAMESPACE_OSGEO_FDO_SCHEMA::ReadOnlyPropertyDefinitionCollection* GetBaseProperties();

    /// \brief
    /// Returns an DataPropertyDefinitionCollection containing references to the
    /// data properties that can be used to uniquely identify instances of the
    /// class. The contained properties must already be defined in either the
    /// properties or baseProperties collection. If GetBaseClass() returns non
    /// null then this collection must be empty.
    /// 
    /// \return
    /// Returns DataPropertyDefinitionCollection
    /// 
	__property  NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinitionCollection* get_IdentityProperties();

    /// \brief
    /// Returns an ReadOnlyDataPropertyDefinitionCollection containing the identity
    /// properties for the base class.
    /// 
    /// \return
    /// Returns ReadOnlyDataPropertyDefinitionCollection
    /// 
	__property  NAMESPACE_OSGEO_FDO_SCHEMA::ReadOnlyDataPropertyDefinitionCollection* get_BaseIdentityProperties();

    /// \brief
    /// Returns an PropertyCollection containing DataProperties,
    /// ClassProperties, and GeometricProperties defined for this class.
    /// 
    /// \return
    /// Returns PropertyDefinitionCollection
    /// 
	__property  NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinitionCollection* get_Properties();

    /// \brief
    /// Gets the fully qualified name of this class
    /// 
    /// \return
    /// Returns {schema_name}:{class_name}
    /// 
	__property  System::String* get_QualifiedName();

    /// \brief
    /// Gets the computed state of ClassDefinition. Computed classes cannot be made persistent or added to 
    /// FDO schema. The computed classes are used as a transient classes that can be returned by a feature or data reader. 
    /// They describe the result of a select expression, select function and select distinct.
    /// 
    /// \return
    /// Returns true if the ClassDefinition is a computed class
    /// 
	__property  System::Boolean get_IsComputed();

    /// \brief
    /// Sets the computed state of ClassDefinition. This is an internal method that 
    /// can only be called by providers. Application should not use this method.
    /// 
    /// \param value 
    /// Input the computed state
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property  System::Void set_IsComputed(System::Boolean value);

    /// \brief
    /// Gets the class-specific capabilities for this class.
    /// 
    /// \return
    /// Returns the capabilities. The Class Capabilities are connection
    /// datastore dependent so they are only defined when this object was 
    /// created by a DescribeSchema command. The returned capabilities are the
    /// ones for this command's connection.
    /// Returns NULL if this object was not created by a DescribeSchema command.
    /// 
	__property  NAMESPACE_OSGEO_FDO_SCHEMA::ClassCapabilities* get_Capabilities();
	
    /// \brief
    /// Sets the class-specific capabilities for this class.
    /// This function must only be called by an FDO Provider. It would be 
    /// typically called by the DescribeSchema command implementation.
    /// 
    /// \param value 
    /// Input the class capabilities.
    /// 
	__property  System::Void set_Capabilities(NAMESPACE_OSGEO_FDO_SCHEMA::ClassCapabilities* value);

    /// \brief
    /// Gets a UniqueConstraintCollection containing all unique constraints applied to properties of this class.
    /// The contained properties must already be defined in either the
    /// properties or baseProperties collections.
    /// 
    /// \return
    /// A UniqueConstraintCollection containing all unique constraints defined for properties of this class.
    /// 
	__property  NAMESPACE_OSGEO_FDO_SCHEMA::UniqueConstraintCollection* get_UniqueConstraints();

    /// \brief
    /// Constructs a ClassDefinition object based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    /// Input A Pointer to the unmanaged object.
    /// 
    /// \param autoDelete 
    /// Input Indicates if the constructed object should be automatically deleted 
    /// once it no longer referenced.
    /// 
	ClassDefinition(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_SCHEMA::SchemaElement(unmanaged, autoDelete)
	{

	}

public private:
	inline FdoClassDefinition* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_SCHEMA


