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

#include "FDO\Commands\Schema\mgPhysicalClassMapping.h"

class FdoXmlClassMapping;

BEGIN_NAMESPACE_OSGEO_FDO_XML
public __gc class XmlElementMappingCollection;

/// \brief
/// XmlClassMapping describes GML to FDO mappings for a particular FDO class
/// Only mappings that cannot be represented in FDO are described.
public __gc class XmlClassMapping : public NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalClassMapping
{
public:
    /// \brief
    /// Constructs an object describing the GML-FDO mappings for an FDO class.
    /// 
    /// \param name 
    /// Input class name
    /// \param GmlName 
    /// Input corresponding type name in GML version of the
    /// class's schema.
    /// \param wkBaseName 
    /// Input name of the most specialized well-known type. This is 
    /// set when the type represents geometries and indicates what kind of geometries the type
    /// represents. 
    /// 
	XmlClassMapping(System::String* name, System::String* schemaName, System::String* wkBaseName);

	XmlClassMapping(System::String* name, System::String* schemaName, System::String* wkBaseName, System::String* wkSchemaName);
		
    /// \brief
    /// Gets the name of the class's corresponding GML complexType.
    /// 
    /// \return
    /// Returns System::String.
    /// 
	__property System::String* get_GmlName();

    /// \brief
    /// Gets the most specialized well-known base type
    /// 
    /// \return
    /// Returns System::String.
    /// 
	__property System::String* get_WkBaseName();

    /// \brief
    /// Gets the schema containing most specialized well-known base type
    /// 
    /// \return
    /// Returns System::String.
    /// 
	__property System::String* get_WkSchemaName();

    /// \brief
    /// Gets the mappings between GML elements for this class's properties and their
    /// Fdo Classes. 
    /// 
    /// \return
    /// Returns XmlElementMappingCollection*
    /// 
	__property NAMESPACE_OSGEO_FDO_XML::XmlElementMappingCollection* get_ElementMappings();

public private:
    XmlClassMapping(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalClassMapping(unmanaged, autoDelete)
	{
	}

	inline FdoXmlClassMapping* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_XML


