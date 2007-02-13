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
#include "FDO\Schema\mgPropertyType.h"

class FdoPropertyDefinition;

BEGIN_NAMESPACE_OSGEO_FDO_SCHEMA

/// \brief
/// PropertyDefinition is an abstract class that derives from SchemaElement.
/// PropertyDefinition is the base class of DataPropertyDefinition, 
/// GeometricPropertyDefinition, and ObjectPropertyDefinition.
public __gc class PropertyDefinition : public NAMESPACE_OSGEO_FDO_SCHEMA::SchemaElement
{
public:
    /// \brief
    /// Abstract operation that must be implemented by derived classes to return the concrete property type.
    /// 
    /// \return
    /// Returns the property type
    /// 
	__property NAMESPACE_OSGEO_FDO_SCHEMA::PropertyType get_PropertyType();

    /// \brief
    /// Gets the fully qualified name of this property
    /// 
    /// \return
    /// Returns {schema_name}:{class_name}.{property_name}
    /// 
	__property System::String* get_QualifiedName();

    /// \brief
    /// Returns a Boolean value that indicates if this is a system generated property.
    /// 
    /// \return
    /// Returns true if this property is system generated, false otherwise.
    /// 
    /// System properties are not written out to the XML schema file, which remains provider-portable.
	__property System::Boolean get_IsSystem();

    /// \brief
    /// Sets whether this is a system property.
    /// This function must only be called by an FDO Provider. Typically, it would be 
    /// called by the DescribeSchema command implementation.
    /// 
    /// \param value 
    /// Input true if this is a system property.
    /// 
	__property System::Void set_IsSystem(System::Boolean value);

    /// \brief
    /// Constructs a PropertyDefinition object based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    /// Input A Pointer to the unmanaged object.
    /// 
    /// \param autoDelete 
    /// Input Indicates if the constructed object should be automatically deleted 
    /// once it no longer referenced.
    /// 
	PropertyDefinition(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_SCHEMA::SchemaElement(unmanaged, autoDelete)
	{
		
	}

public private:
	inline FdoPropertyDefinition* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_SCHEMA


