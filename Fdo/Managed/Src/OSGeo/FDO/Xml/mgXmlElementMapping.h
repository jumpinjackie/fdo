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
ref class XmlClassMapping;

/// \ingroup (OSGeoFDOXml)
/// \brief
/// XmlElementMapping describes GML to FDO mappings for a particular GML Schema
/// element. Only mappings that cannot be represented in FDO are described.
public ref class XmlElementMapping : public NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalElementMapping
{
public:
    /// \brief
    /// Constructs an object describing the GML-FDO mappings for a GML element
    /// 
    /// \param name 
    /// Input element name
    /// 
	XmlElementMapping(System::String^ name);

    /// \brief
    /// Sets the class mapping for this element. The class mapping provides
    /// information about the FDO Class for this element's type.
    /// 
    /// \param classMapping 
    /// Input the class mapping
    /// 
    /// \brief
    /// Gets the class mapping for this element. The class mapping provides
    /// information about the FDO Class for this element's type.
    /// 
    /// \return
    /// Returns NAMESPACE_OSGEO_FDO_XML::XmlClassMapping.
    /// 
    property NAMESPACE_OSGEO_FDO_XML::XmlClassMapping^ ClassMapping
    {
        NAMESPACE_OSGEO_FDO_XML::XmlClassMapping^ get();
        System::Void set(NAMESPACE_OSGEO_FDO_XML::XmlClassMapping^ classMapping);
    }
    /// \brief
    /// Sets the element class name to a dangling reference. This function can be
    /// used to reference a class that has no NAMESPACE_OSGEO_FDO_XML::XmlClassMapping object.
    /// 
    /// \param className 
    /// Input name of the FDO class corresponding to the 
    /// element's type.
    /// 
    /// \brief
    /// Gets the FDO Class name.
    /// 
    /// \return
    /// Returns System::String.
    /// 
    property System::String^ ClassName
    {
        System::String^ get();
        System::Void set(System::String^ className);
    }

    /// \brief
    /// Sets the schema name for a dangling reference to an element's class. 
    /// 
    /// \param schemaName 
    /// Input name of the Feature Schema containing the FDO class. If
    /// L"" then it defaults to the schema containing this element.
    /// 
    /// \brief
    /// Gets the Schema Name for the FDO Class.
    /// 
    /// \return
    /// Returns System::String.
    /// 
    property System::String^ SchemaName
    {
        System::String^ get();
        System::Void set(System::String^ value);
    }
    /// \brief
    /// Sets the namespace URI for this element. 
    /// 
    /// \param value 
    /// Input a URI. Defaults to the targetNamespace
    /// for the schema containing this element.
    /// 
    /// \brief
    /// Gets the URI for the referenced element. 
    /// 
    /// \return
    /// Returns System::String.
    /// 
    property System::String^ GmlUri
    {
        System::String^ get();
        System::Void set(System::String^ value);
    }

    /// \brief
    /// Sets the local name for this element. 
    /// 
    /// \param value 
    /// Input the local name (unprefixed name) of this
    /// element. Defaults to the element name.
    /// 
    /// \brief
    /// Gets the URI for the referenced element. 
    /// 
    /// \return
    /// Returns System::String. L"" if this element does not reference
    /// another element.
    /// 
    property System::String^ GmlLocalName
    {
        System::String^ get();
        System::Void set(System::String^ value);
    }

internal:
	XmlElementMapping(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalElementMapping(unmanaged, autoDelete)
	{
		
	}

	inline FdoXmlElementMapping* GetImpObj();
public:
    virtual IntPtr GetDisposableObject() override;
};

END_NAMESPACE_OSGEO_FDO_XML


