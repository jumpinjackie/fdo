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

#include "FDO\Commands\Schema\mgPhysicalElementMapping.h"

class FdoXmlElementMapping;

BEGIN_NAMESPACE_OSGEO_FDO_XML
public __gc class XmlClassMapping;

/// \ingroup (OSGeoFDOXml)
/// \brief
/// XmlElementMapping describes GML to FDO mappings for a particular GML Schema
/// element. Only mappings that cannot be represented in FDO are described.
public __gc class XmlElementMapping : public NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalElementMapping
{
public:
    /// \brief
    /// Constructs an object describing the GML-FDO mappings for a GML element
    /// 
    /// \param name 
    /// Input element name
    /// 
	XmlElementMapping(System::String* name);

    /// \brief
    /// Sets the class mapping for this element. The class mapping provides
    /// information about the FDO Class for this element's type.
    /// 
    /// \param classMapping 
    /// Input the class mapping
    /// 
	__property System::Void set_ClassMapping(NAMESPACE_OSGEO_FDO_XML::XmlClassMapping* classMapping);

    /// \brief
    /// Gets the class mapping for this element. The class mapping provides
    /// information about the FDO Class for this element's type.
    /// 
    /// \return
    /// Returns NAMESPACE_OSGEO_FDO_XML::XmlClassMapping.
    /// 
	__property NAMESPACE_OSGEO_FDO_XML::XmlClassMapping* get_ClassMapping();

    /// \brief
    /// Sets the element class name to a dangling reference. This function can be
    /// used to reference a class that has no NAMESPACE_OSGEO_FDO_XML::XmlClassMapping object.
    /// 
    /// \param className 
    /// Input name of the FDO class corresponding to the 
    /// element's type.
    /// 
	__property System::Void set_ClassName(System::String* className);

    /// \brief
    /// Gets the FDO Class name.
    /// 
    /// \return
    /// Returns System::String.
    /// 
	__property System::String* get_ClassName();

    /// \brief
    /// Sets the schema name for a dangling reference to an element's class. 
    /// 
    /// \param schemaName 
    /// Input name of the Feature Schema containing the FDO class. If
    /// L"" then it defaults to the schema containing this element.
    /// 
	__property System::Void set_SchemaName(System::String* schemaName);

    /// \brief
    /// Gets the Schema Name for the FDO Class.
    /// 
    /// \return
    /// Returns System::String.
    /// 
	__property System::String* get_SchemaName();

    /// \brief
    /// Sets the namespace URI for this element. 
    /// 
    /// \param value 
    /// Input a URI. Defaults to the targetNamespace
    /// for the schema containing this element.
    /// 
	__property System::Void set_GmlUri(System::String* value);

    /// \brief
    /// Gets the URI for the referenced element. 
    /// 
    /// \return
    /// Returns System::String.
    /// 
	__property System::String* get_GmlUri();

    /// \brief
    /// Sets the local name for this element. 
    /// 
    /// \param value 
    /// Input the local name (unprefixed name) of this
    /// element. Defaults to the element name.
    /// 
	__property System::Void set_GmlLocalName(System::String* value);

    /// \brief
    /// Gets the URI for the referenced element. 
    /// 
    /// \return
    /// Returns System::String. L"" if this element does not reference
    /// another element.
    /// 
	__property System::String* get_GmlLocalName();

public private:
	XmlElementMapping(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalElementMapping(unmanaged, autoDelete)
	{
		
	}

	inline FdoXmlElementMapping* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_XML


