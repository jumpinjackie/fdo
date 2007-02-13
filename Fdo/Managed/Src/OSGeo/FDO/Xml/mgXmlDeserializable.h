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

#include "FDO\Xml\mgIXmlDeserializable.h"

class FdoXmlDeserializable;

BEGIN_NAMESPACE_OSGEO_FDO_XML
public __gc class XmlFlags;

/// \brief
/// XmlDeserializable provides the ability to deserialize an FDO object 
/// from an XML Document. Deserialization support can be added to any class by basing
/// it on XmlDeserializable and implementing the XmlSaxHander callbacks.
public __gc class XmlDeserializable : public NAMESPACE_OSGEO_COMMON_XML::XmlSaxHandler, public NAMESPACE_OSGEO_FDO_XML::IXmlDeserializable
{
public:
    /// \brief
    /// Reads from an XML document in a file.
    /// 
    /// \param fileName 
    /// Input the file name. Must consist of an XML document.
    /// 
	System::Void ReadXml(System::String* fileName);

    /// \brief
    /// Reads from an XML document in a file.
    /// 
    /// \param fileName 
    /// Input the file name. Must consist of an XML document.
    /// \param flags 
    /// Input controls the reading of the elements from the document.
    /// If NULL then the default flags are used (see NAMESPACE_OSGEO_FDO_XML::XmlFlags::Create())
    /// 
	System::Void ReadXml(System::String* fileName, NAMESPACE_OSGEO_FDO_XML::XmlFlags* flags);

    /// \brief
    /// Reads an XML document from an XML reader.
    /// 
    /// \param xmlReader 
    /// Input the XML reader.
    /// 
	System::Void ReadXml(NAMESPACE_OSGEO_COMMON_XML::XmlReader* xmlReader);

    /// \brief
    /// Reads an XML document from an XML reader.
    /// 
    /// \param xmlReader 
    /// Input the XML reader.
    /// \param flags 
    /// Input controls the reading of the elements from the document. 
    /// If NULL then the default flags are used (see NAMESPACE_OSGEO_FDO_XML::XmlFlags::Create())
    /// 
	System::Void ReadXml(NAMESPACE_OSGEO_COMMON_XML::XmlReader* xmlReader, NAMESPACE_OSGEO_FDO_XML::XmlFlags* flags);

    /// \brief
    /// Reads an XML document from a text reader.
    /// 
    /// \param textReader 
    /// Input the text reader. Must be positioned at the
    /// start of an XML document.
    /// 
	System::Void ReadXml(NAMESPACE_OSGEO_COMMON_IO::IoTextReader* textReader);

    /// \brief
    /// Reads an XML document from a text reader.
    /// 
    /// \param textReader 
    /// Input the text reader. Must be positioned at the
    /// start of an XML document.
    /// \param flags 
    /// Input controls the reading of the elements from the document. 
    /// If NULL then the default flags are used (see NAMESPACE_OSGEO_FDO_XML::XmlFlags::Create())
    /// 
	System::Void ReadXml(NAMESPACE_OSGEO_COMMON_IO::IoTextReader* textReader, NAMESPACE_OSGEO_FDO_XML::XmlFlags* flags);

    /// \brief
    /// Reads an XML document from a stream.
    /// 
    /// \param stream 
    /// Input the stream. Must be positioned at the
    /// start of an XML document.
    /// 
	System::Void ReadXml(NAMESPACE_OSGEO_COMMON_IO::IoStream* stream);

    /// \brief
    /// Reads an XML document from a stream.
    /// 
    /// \param stream 
    /// Input the stream. Must be positioned at the
    /// start of an XML document.
    /// \param flags 
    /// Input controls the reading of the elements from the document. 
    /// If NULL then the default flags are used (see NAMESPACE_OSGEO_FDO_XML::XmlFlags::Create())
    /// 
    /// 
	System::Void ReadXml(NAMESPACE_OSGEO_COMMON_IO::IoStream* stream, NAMESPACE_OSGEO_FDO_XML::XmlFlags* flags);

    /// \brief
    /// Gets the XML Flags that were passed to the ReadXml() 
    /// function that is currently being executed. This function would 
    /// typically be called by the XmlSaxHandler callbacks.
    /// 
    /// \return
    /// Returns the XML Flags
    /// 
	NAMESPACE_OSGEO_FDO_XML::XmlFlags* GetDeserializationFlags();

    /// \brief
    /// Gets the current XML Reader. This function would 
    /// typically be called by the XmlSaxHandler callbacks when 
    /// ReadXml() is being invoked. When ReadXml() reads from a file, stream,
    /// or text reader, a wrapping XML reader is automatically created.
    /// 
    /// \return
    /// Returns the XML reader
    /// 
	NAMESPACE_OSGEO_COMMON_XML::XmlReader* GetXmlReader();

    /// \brief
    /// Gets the stylesheet for converting the XML document from 
    /// external to internal format. When classes derived from XmlDeserializable
    /// define an internal format, they must override this function to return a
    /// stylesheet that does the conversion.
    /// 
    /// \return
    /// Returns NULL by default (no internal format defined)
    /// 
	NAMESPACE_OSGEO_COMMON_XML::XmlReader* GetFromExternalStylesheet();

    /// \brief
    /// Gets the SAX context to pass to the XmlSaxHandler callbacks.
    /// Classes derived from XmlDeserializable can override this function to 
    /// specify a SAX context with class-specific information.
    /// 
    /// \return
    /// Returns NULL by default, the default NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext is used.
    /// The default provides basic error handling functionality.
    /// 
	NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext* GetSaxContext();

public private:
	XmlDeserializable(System::IntPtr unmanaged, System::Boolean autoDelete);

	inline FdoXmlDeserializable* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_XML


