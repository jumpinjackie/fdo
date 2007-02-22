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

#include "FDO\Schema\mgPropertyDefinition.h"
#include "FDO\Schema\mgDataType.h"

class FdoDataPropertyDefinition;

BEGIN_NAMESPACE_OSGEO_FDO_SCHEMA

/// \ingroup (OSGeoFDOSchema)
/// \brief
/// The DataPropertyDefinition class derives PropertyDefinition and represents simple
/// values or collections of simple values. DataPropertyDefinitions can take on
/// any of the data types listed in DataType enumeration.
public __gc class DataPropertyDefinition : public NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinition
{
public:
    /// \brief
    /// Constructs a default instance of an DataPropertyDefinition.
    /// 
	DataPropertyDefinition();

    /// \brief
    /// Constructs an instance of an DataPropertyDefinition using the specified arguments
    /// 
    /// \param name 
    /// Input name
    /// \param description 
    /// Input description
    /// 
	DataPropertyDefinition(System::String* name, System::String* description);

    /// \brief
    /// Constructs an instance of an DataPropertyDefinition using the specified arguments
    /// 
    /// \param name 
    /// Input name
    /// \param description 
    /// Input description
    /// \param system 
    /// Input true if this is a system generated property, otherwise falseds.
    /// 
	DataPropertyDefinition(System::String* name, System::String* description, System::Boolean system);

    /// \brief
    /// Gets the concrete property type.
    /// \return
    /// Returns the concrete property type.
	__property NAMESPACE_OSGEO_FDO_SCHEMA::PropertyType get_PropertyType();

    /// \brief
    /// Gets the DataType of this property.
    /// 
    /// \return
    /// Returns the DataType of this property
    /// 
	__property NAMESPACE_OSGEO_FDO_SCHEMA::DataType get_DataType();

    /// \brief
    /// Sets the DataType of this property.
    /// 
    /// \param value 
    /// Input the DataType of this property
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_DataType(NAMESPACE_OSGEO_FDO_SCHEMA::DataType value);

    /// \brief
    /// Returns a Boolean value that indicates if this property is read-only.
    /// 
    /// \return
    /// Returns a Boolean value
    /// 
	__property System::Boolean get_ReadOnly();

    /// \brief
    /// Sets a Boolean value that indicates if this property is read-only.
    /// 
    /// \param value 
    /// Input a Boolean value that indicates if this property is read-only
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_ReadOnly(System::Boolean value);

    /// \brief
    /// Gets the length of a String, BLOB, or CLOB data property. This value
    /// does not apply to any other DataType.
    /// 
    /// \return
    /// Returns the length of a String, BLOB, or CLOB data property
    /// 
	__property System::Int32 get_Length();

    /// \brief
    /// Sets the length of a String, BLOB, or CLOB data property. This value is
    /// ignored for all other DataType values.
    /// 
    /// \param value 
    /// Input the length of a String, BLOB, or CLOB data property
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_Length(System::Int32 value);

    /// \brief
    /// Gets the precision (total number of digits) of a decimal data property.
    /// This value does not apply to any other DataType.
    /// 
    /// \return
    /// Returns the precision
    /// 
	__property System::Int32 get_Precision();

    /// \brief
    /// Sets the precision (total number of digits) of a decimal data property.
    /// This value is ignored for all other DataType values.
    /// 
    /// \param value 
    /// Input the precision
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_Precision(System::Int32 value);

    /// \brief
    /// Gets the scale (number of digits to the right of the decimal point) of a
    /// decimal data property. This value does not apply to any other DataType.
    /// 
    /// \return
    /// Returns the scale
    /// 
	__property System::Int32 get_Scale();

    /// \brief
    /// Sets the scale (number of digits to the right of the decimal point) of a
    /// decimal data property. This value is ignored for all other DataType values.
    /// 
    /// \param value 
    /// Input the scale
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_Scale(System::Int32 value);

    /// \brief
    /// Returns a Boolean value that indicates if this property's value can be
    /// null.
    /// 
    /// \return
    /// Returns a Boolean value
    /// 
	__property System::Boolean get_Nullable();

    /// \brief
    /// Sets a Boolean value that indicates if this property's value can be
    /// null.
    /// 
    /// \param value 
    /// Input a Boolean value that indicates if this property's value can be
    /// null
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_Nullable(System::Boolean value);

    /// \brief
    /// Gets the default value for this property. The default value is used when
    /// an instance of the containing class is created without specifying a value
    /// for this property.
    /// 
    /// \return
    /// Returns the default value
    /// 
	__property System::String* get_DefaultValue();

    /// \brief
    /// Sets the default value for this property. The default value is used when
    /// an instance of the containing class is created without specifying a value
    /// for this property.
    /// 
    /// \param value 
    /// Input the default value
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_DefaultValue(System::String* value);

    /// \brief
    /// Sets a Boolean value that indicates if this is an autogenerated property.
    /// 
    /// \param value 
    /// Input a Boolean value that indicates if this property should
    /// be automatically generated by the provider. If true, then the provider will auto-generate
    /// unique id values for objects of the class containing this property.
    /// The provider will set the attribute properties to read-only.
    /// If the provider does not support auto-generation or if the data types of the
    /// property is not supported for unique id generation by the provider, then the provider will
    /// throw an AutoGenerationException. If false, then the provider will not autogenerate values.
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_IsAutoGenerated(System::Boolean value);

    /// \brief
    /// Returns a Boolean value that indicates if this is an autogenerated property.
    /// 
    /// \return
    /// Returns a Boolean value.
    /// 
	__property System::Boolean get_IsAutoGenerated();

    /// \brief
    /// Returns the value constraint of this data property.
    /// 
    /// \return
    /// Returns PropertyValueConstraint or NULL if not set.
    /// 
	__property NAMESPACE_OSGEO_FDO_SCHEMA::PropertyValueConstraint* get_ValueConstraint();

    /// \brief
    /// Sets the value constraint of this data property.
    /// 
    /// \param value 
    /// Set the value constraint of this data property.
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_ValueConstraint(NAMESPACE_OSGEO_FDO_SCHEMA::PropertyValueConstraint* value );

    /// \brief
    /// Constructs a DataPropertyDefinition object based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    /// Input A Pointer to the unmanaged object.
    /// 
    /// \param autoDelete 
    /// Input Indicates if the constructed object should be automatically deleted 
    /// once it no longer referenced.
    /// 
	DataPropertyDefinition(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinition(unmanaged, autoDelete)
	{

	}

public private:
	inline FdoDataPropertyDefinition* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_SCHEMA


