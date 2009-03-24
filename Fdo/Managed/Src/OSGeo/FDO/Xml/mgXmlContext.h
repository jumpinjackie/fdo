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

class FdoXmlContext;

BEGIN_NAMESPACE_OSGEO_FDO_XML
ref class XmlFlags;

/// \ingroup (OSGeoFDOXml)
/// \brief
/// Default context for deserializing from XML documents via
/// XmlDeserializable.
public ref class XmlContext : public NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext
{
public:
    /// \brief
    /// Constructs a context for parsing FDO Deserializable items
    /// 
    /// \param flags 
    /// Input controls the reading of the elements from the document.
    /// 
    /// \param xmlReader 
    /// Input the XML reader.
    /// 
	XmlContext(NAMESPACE_OSGEO_FDO_XML::XmlFlags^ flags, NAMESPACE_OSGEO_COMMON_XML::XmlReader^ xmlReader);

    /// \brief
    /// Decodes any element name that was encoded to be XML-compatible.
    /// 
    /// \param name 
    /// Input the name to Decode.
    /// 
	System::String^ DecodeName (System::String^ name);

    /// \brief
    /// Retrieves the current XML flags.
    /// 
    /// \return
    /// Returns the XML flags associated to the Context
    /// 
	NAMESPACE_OSGEO_FDO_XML::XmlFlags^ GetFlags();

internal:
	XmlContext(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext(unmanaged, autoDelete)
	{
		
	}

	inline FdoXmlContext* GetImpObj();
public:
    virtual IntPtr GetDisposableObject() override;
};

END_NAMESPACE_OSGEO_FDO_XML


