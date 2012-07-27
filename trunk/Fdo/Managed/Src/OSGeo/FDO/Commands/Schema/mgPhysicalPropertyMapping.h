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

#include "FDO\Commands\Schema\mgIPhysicalPropertyMapping.h"
#include "FDO\Commands\Schema\mgPhysicalElementMapping.h"

class FdoPhysicalPropertyMapping;

BEGIN_NAMESPACE_OSGEO_COMMON_XML
ref class XmlSaxContext;
ref class XmlAttributeCollection;
ref class XmlWriter;
END_NAMESPACE_OSGEO_COMMON_XML

BEGIN_NAMESPACE_OSGEO_FDO_XML
ref class XmlFlags;
END_NAMESPACE_OSGEO_FDO_XML

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA

/// \ingroup (OSGeoFDOCommandsSchema)
/// \brief
/// PhysicalPropertyMapping is an abstract class that acts as a base class for all 
/// Physical Schema Mapping property overrides.
public ref class PhysicalPropertyMapping : public NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalElementMapping, public NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::IPhysicalPropertyMapping
{
public:
    /// \brief
    /// Initializes this Physical Property Mapping from its XML attributes. Called when
    /// this element is deserialized from XML. This function decodes any
    /// encoded property names (See _writeXml). Can be extended to handle 
    /// particular XML attributes for derived classes.
    /// 
    /// \param context 
    /// Input context contain information about the current deserialization operation
    /// \param attributes 
    /// Input the XML attributes.
    /// 
    /// \return
    /// Returns nothing
    /// 
	virtual System::Void InitFromXml(NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext^ context, NAMESPACE_OSGEO_COMMON_XML::XmlAttributeCollection^ attributes) override;

    /// \brief
    /// Writes this Physical Property Mapping to XML. Called when
    /// this element is serialized to XML. This function converts (encodes) 
    /// property names to be valid XML element names. Can be extended to handle 
    /// particular XML attributes and sub-elements for derived classes.
    /// <p><b>Note:</b> Schema Override elements that do not require name encoding
    /// should be based on PhysicalElementMapping instead of this class.
    /// 
    /// \param xmlWriter 
    /// Input write the element to this XML writer
    /// \param flags 
    /// Input flags that control the writing of the element.
    /// 
    /// \return
    /// Returns nothing
    /// 
	virtual System::Void WriteXml(NAMESPACE_OSGEO_COMMON_XML::XmlWriter^ xmlWriter, NAMESPACE_OSGEO_FDO_XML::XmlFlags^ flags) override;

/// \cond DOXYGEN-IGNORE
public protected:
	PhysicalPropertyMapping(System::IntPtr unmanaged, System::Boolean autoDelete);
    
    inline FdoPhysicalPropertyMapping* GetImpObj();
public:
    virtual IntPtr GetDisposableObject() override;
/// \endcond
};

END_NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA


