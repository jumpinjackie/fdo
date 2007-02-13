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


#include "FDO\Commands\Schema\mgPhysicalElementMapping.h"

class FdoPhysicalClassMapping;

BEGIN_NAMESPACE_OSGEO_COMMON_XML
public __gc class XmlSaxContext;
public __gc class XmlAttributeCollection;
public __gc class XmlWriter;
END_NAMESPACE_OSGEO_COMMON_XML

BEGIN_NAMESPACE_OSGEO_FDO_XML
public __gc class XmlFlags;
END_NAMESPACE_OSGEO_FDO_XML

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA

/// \brief
/// PhysicalClassMapping is an abstract class that acts as a base class for all 
/// Physical Schema Mapping class overrides.
public __gc class PhysicalClassMapping : public NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalElementMapping
{
public:
    /// \brief
    /// Initializes this class from its XML attributes. Called when
    /// the class is deserialized from XML. This function decodes any
    /// encoded class names (See _writeXml) and strips off the "Type" suffix if present.
    /// Can be extended to handle 
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
	System::Void InitFromXml(NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext* context, NAMESPACE_OSGEO_COMMON_XML::XmlAttributeCollection* attributes);

    /// \brief
    /// Writes this class to XML. Called when
    /// the class is serialized to XML. This function converts (encodes) 
    /// class names to be valid XML element names and appends the "Type"
    /// suffix to the name. Can be extended to handle 
    /// particular XML attributes for derived classes.
    /// <p><b>Note:</b> Schema Override elements that do not require name encoding
    /// should be based on PhysicalElementMapping instead of this class.
    /// 
    /// \param xmlWriter 
    /// Input write the class to this XML writer
    /// \param flags 
    /// Input flags that control the writing of the class.
    /// 
    /// \return
    /// Returns nothing
    /// 
    /// Write this element to XML.
	System::Void WriteXml(NAMESPACE_OSGEO_COMMON_XML::XmlWriter* xmlWriter, NAMESPACE_OSGEO_FDO_XML::XmlFlags* flags);

/// \cond DOXYGEN-IGNORE
public protected:
	PhysicalClassMapping(System::IntPtr unmanaged, System::Boolean autoDelete);
/// \endcond

public private:
	inline FdoPhysicalClassMapping* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA


