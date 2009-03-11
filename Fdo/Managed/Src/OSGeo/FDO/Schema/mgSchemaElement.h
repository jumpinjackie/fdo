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

#pragma once

#include "FDO\schema\mgSchemaElementState.h"

class FdoSchemaElement;

BEGIN_NAMESPACE_OSGEO_FDO_SCHEMA
ref class FeatureSchema;
ref class SchemaAttributeDictionary;

/// \ingroup (OSGeoFDOSchema)
/// \brief
/// SchemaElement is an abstract class that acts as a base class for all classes
/// in the Schema package.
public ref class SchemaElement : public NAMESPACE_OSGEO_COMMON_XML::XmlSaxHandler
{
public:
    /// \brief
    /// Gets the parent of this SchemaElement or null if this object has not
    /// been added to a parent object.
    /// 
    /// \return
    /// Returns the SchemaElement of the parent
    /// 
    property NAMESPACE_OSGEO_FDO_SCHEMA::SchemaElement^ Parent
    {
        NAMESPACE_OSGEO_FDO_SCHEMA::SchemaElement^ get();
    }

    /// \brief
    /// Gets the FeatureSchema this object is a part of. Returns null if this
    /// object has not been added to a feature schema.
    /// 
    /// \return
    /// Returns the FeatureSchema this object is a part of
    /// 
    property NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchema^ FeatureSchema
    {
        NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchema^ get();
    }

    /// \brief
    /// Returns the current state of the schema element.
    /// 
    /// \return
    /// Returns the current state of the schema element
    /// 
    property NAMESPACE_OSGEO_FDO_SCHEMA::SchemaElementState ElementState
    {
        NAMESPACE_OSGEO_FDO_SCHEMA::SchemaElementState get();
    }

    /// \brief
    /// Gets an SchemaAttributeDictionary that contains the attributes 
    /// associated with this SchemaElement.
    /// 
    /// \return
    /// Returns an SchemaAttributeDictionary
    /// 
    property NAMESPACE_OSGEO_FDO_SCHEMA::SchemaAttributeDictionary^ Attributes
    {
        NAMESPACE_OSGEO_FDO_SCHEMA::SchemaAttributeDictionary^ get();
    }

    /// \brief
    /// Sets the name of the schema object.
    /// 
    /// \param value 
    /// Input the name of the schema object
    /// 
    /// \brief
    /// Gets the name of the schema object.
    /// 
    /// \return
    /// Returns the name of the schema object
    /// 
    property System::String^ Name
    {
        System::String^ get();
        System::Void set(System::String^ value);
    }

    /// \brief
    /// Gets the fully qualified name of this schema element
    /// 
    /// \return
    /// Default implementation simply returns the element name
    /// 
    property System::String^ QualifiedName
    {
        System::String^ get();
    }

    /// \brief
    /// Indicates whether the name of the Element can be changed once
    /// it has been created.
    /// 
    /// \return
    /// Returns true if the Element name can be changed; otherwise false.
    /// 
    property System::Boolean CanSetName
    {
        System::Boolean get();
    }

    /// \brief
    /// Gets the description of the schema object.
    /// 
    /// \return
    /// Returns the description of the schema object
    /// 
    /// \brief
    /// Sets the description of the schema object.
    /// 
    /// \param value 
    /// Input the description of the schema object
    /// 
    property System::String^ Description
    {
        System::String^ get();
        System::Void set(System::String^ value);
    }

    /// \brief
    /// Marks the schema element for deletion by setting the element state to
    /// SchemaElementState_Deleted. If the schema elements current state is
    /// SchemaElementState_Added then the element is physically removed. If
    /// the current state is SchemaElementState_Detached this method does
    /// nothing.
    /// 
	System::Void Delete();

    /// \brief
    /// Constructs a SchemaElement object based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    /// Input A Pointer to the unmanaged object.
    /// 
    /// \param autoDelete 
    /// Input Indicates if the constructed object should be automatically deleted 
    /// once it no longer referenced.
    /// 
	SchemaElement(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_COMMON_XML::XmlSaxHandler(unmanaged, autoDelete)
	{

	}

internal:
	inline FdoSchemaElement* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_SCHEMA


