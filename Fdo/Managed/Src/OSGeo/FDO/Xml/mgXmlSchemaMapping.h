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


#include "FDO\Commands\Schema\mgPhysicalSchemaMapping.h"

class FdoXmlSchemaMapping;

BEGIN_NAMESPACE_OSGEO_FDO_XML
public __gc class XmlElementMappingCollection;
public __gc class XmlClassMappingCollection;

/// \brief
/// FdoXmlSchemaMapping specifies overrides for translating a feature schema between 
/// FDO and GML.
public __gc class XmlSchemaMapping : public NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping
{
public:
    /// \brief
    /// Gets the provider name.
    /// 
    /// \return
    /// Returns "OSGeo.GML.1.0". Note: this is the name given to this
    /// particular type of Schema Override set and is not the name of an actual FDO provider.
    /// 
    ///  
	System::String* GetProvider();

    /// \brief
    /// Sets the target namespace URI for the GML version of this schema.
    /// When the Feature Schema for these overrides is written to GML, its targetNamespace
    /// is set to this value.
    /// 
    /// \param package 
    /// Input the target namespace.
    /// 
	__property System::Void set_TargetNamespace(System::String* package);

    /// \brief
    /// Gets the target namespace URI for the GML version of this schema.
    /// 
    /// \return
    /// Returns the target namespace.
    /// 
	__property System::String* get_TargetNamespace();

    /// \brief
    /// Gets the mappings between GML global elements and their types as Fdo Classes. These
    /// mappings are used in two circumstances:
    /// <ul>
    ///     <li> when features are read from a GML Document, these mappings determine the Feature
    ///     class of each feature, based on its element name. If the element name is not 
    ///     in these mappings then the class name defaults to the element name.
    ///     <li> when the Feature Schema for these overrides is written to GML, a global element
    ///     is created from each item in element mappings collection. 
    /// </ul>
    /// 
    /// \return
    /// Returns FdoXmlElementMappingCollection*
    /// 
	NAMESPACE_OSGEO_FDO_XML::XmlElementMappingCollection* GetElementMappings();

    /// \brief
    /// Gets the mappings for FDO Classes.
    /// 
    /// \return
    /// Returns FdoXmlClassMappingCollection*
    /// 
	NAMESPACE_OSGEO_FDO_XML::XmlClassMappingCollection* GetClassMappings();

public private:
	XmlSchemaMapping(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping(unmanaged, autoDelete)
	{
		
	}

	inline FdoXmlSchemaMapping* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_XML


