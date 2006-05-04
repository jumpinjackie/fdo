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
public __gc class FeatureSchema;
public __gc class SchemaAttributeDictionary;

/// \brief
/// FdoSchemaElement is an abstract class that acts as a base class for all classes
/// in the Schema package.
public __gc class SchemaElement : public NAMESPACE_OSGEO_COMMON_XML::XmlSaxHandler
{
public:
    /// \brief
    /// Gets the parent of this FdoSchemaElement or null if this object has not
    /// been added to a parent object.
    /// 
    /// \return
    /// Returns FdoSchemaElement of parent
    /// 
	__property NAMESPACE_OSGEO_FDO_SCHEMA::SchemaElement* get_Parent();

    /// \brief
    /// Gets the FdoFeatureSchema this object is a part of. Returns null if this
    /// object has not been added to a feature schema.
    /// 
    /// \return
    /// Returns FdoFeatureSchema this object is a part of
    /// 
	__property NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchema* get_FeatureSchema();

    /// \brief
    /// Returns the current state of the schema element.
    /// 
    /// \return
    /// Returns the current state of the schema element
    /// 
	__property NAMESPACE_OSGEO_FDO_SCHEMA::SchemaElementState get_ElementState();

    /// \brief
    /// Gets an FdoSchemaAttributeDictionary that contains the attributes 
    /// associated with this FdoSchemaElement.
    /// 
    /// \return
    /// Returns an FdoSchemaAttributeDictionary
    /// 
	__property NAMESPACE_OSGEO_FDO_SCHEMA::SchemaAttributeDictionary* get_Attributes();

    /// \brief
    /// Gets the name of the schema object.
    /// 
    /// \return
    /// Returns the name of the schema object
    /// 
	__property System::String* get_Name();

    /// \brief
    /// Gets the fully qualified name of this schema element
    /// 
    /// \return
    /// Default implementation simply returns the element name
    /// 
	__property System::String* get_QualifiedName();

    /// \brief
    /// Sets the name of the schema object.
    /// 
    /// \param value 
    /// Input the name of the schema object
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_Name(System::String* value);

    /// \brief
    /// Sets the name of the schema object.
    /// 
    /// \param value 
    /// Input the name of the schema object
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Boolean get_CanSetName();

    /// \brief
    /// Gets the description of the schema object.
    /// 
    /// \return
    /// Returns the description of the schema object
    /// 
	__property System::String* get_Description();

    /// \brief
    /// Sets the description of the schema object.
    /// 
    /// \param value 
    /// Input the description of the schema object
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_Description(System::String* value);

    /// \brief
    /// Marks the schema element for deletion by setting the element state to
    /// FdoSchemaElementState_Deleted. If the schema elements current state is
    /// FdoSchemaElementState_Added then the element is physically removed. If
    /// the current state is FdoSchemaElementState_Detached this method does
    /// nothing.
    /// 
    /// \return
    /// Returns nothing
    /// 
	System::Void Delete();

	SchemaElement(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_COMMON_XML::XmlSaxHandler(unmanaged, autoDelete)
	{

	}

protected:
	System::Void Dispose(System::Boolean disposing);

public private:
	inline FdoSchemaElement* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_SCHEMA


