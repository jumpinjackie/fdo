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

#include "FDO\Xml\mgIXmlSerializable.h"

class FdoXmlSerializable;

BEGIN_NAMESPACE_OSGEO_FDO_XML
public __gc class XmlFlags;

/// \brief
/// FdoXmlSerializable provides the ability to serialize an FDO object 
/// to an XML Document. Serialization support can be added to any class by basing
/// it on FdoXmlSerializable and implementing the _writeXml callback.
public __gc class XmlSerializable : public NAMESPACE_OSGEO_RUNTIME:: Disposable, public NAMESPACE_OSGEO_FDO_XML::IXmlSerializable
{
public:
    /// \brief
    /// Writes this object to a file. A complete XML document,
    /// containing this object, is written.
    /// 
    /// \param fileName 
    /// Input the file name.
    /// \param flags 
    /// Input controls the writing of the elements to the document.
    /// If NULL then the default flags are used (see FdoXmlFlags::Create())
    /// 
	System::Void WriteXml(String* fileName);

	System::Void WriteXml(String* fileName, NAMESPACE_OSGEO_FDO_XML::XmlFlags* flags);

    /// \brief
    /// Writes to an XML writer. This object is appended to the XML document
    /// being written. Unlike the other WriteXml() functions it is not necessarily 
    /// the only object in its document.
    /// 
    /// \param xmlWriter 
    /// Input the XML writer. When this function completes, the
    /// XML writer's current position will be just after this object.
    /// \param flags 
    /// Input controls the writing of the elements to the writer.
    /// If NULL then the default flags are used (see FdoXmlFlags::Create())
    /// 
	System::Void WriteXml(NAMESPACE_OSGEO_COMMON_XML::XmlWriter* xmlWriter);

	System::Void WriteXml(NAMESPACE_OSGEO_COMMON_XML::XmlWriter* xmlWriter, NAMESPACE_OSGEO_FDO_XML::XmlFlags* flags);

    /// \brief
    /// Writes to a text writer. A complete XML document,
    /// containing this object, is written.
    /// 
    /// \param textWriter 
    /// Input the text writer.When this function completes, the
    /// text writer's current position will be just after this object.
    /// \param flags 
    /// Input controls the writing of the elements to the writer.
    /// If NULL then the default flags are used (see FdoXmlFlags::Create())
    /// 
	System::Void WriteXml(NAMESPACE_OSGEO_COMMON_IO::IoTextWriter* textWriter);

	System::Void WriteXml(NAMESPACE_OSGEO_COMMON_IO::IoTextWriter* textWriter, NAMESPACE_OSGEO_FDO_XML::XmlFlags* flags);

    /// \brief
    /// Writes to a stream. A complete XML document,
    /// containing this object, is written.
    /// 
    /// \param stream 
    /// Input the stream. When this function completes, the
    /// stream's current position will be just after this object.
    /// \param flags 
    /// Input controls the writing of the elements to the stream.
    /// If NULL then the default flags are used (see FdoXmlFlags::Create())
    /// 
	System::Void WriteXml(NAMESPACE_OSGEO_COMMON_IO::IoStream* stream);

	System::Void WriteXml(NAMESPACE_OSGEO_COMMON_IO::IoStream* stream, NAMESPACE_OSGEO_FDO_XML::XmlFlags* flags);

    /// \brief
    /// Gets the stylesheet for converting the XML document from 
    /// internal to external format. When classes derived from FdoXmlSerializable
    /// define an internal format, they must override this function to return a
    /// stylesheet that does the conversion.
    /// 
    /// \return
    /// Returns NULL by default (no internal format defined)
    /// 
	NAMESPACE_OSGEO_COMMON_XML::XmlReader* GetFromInternalStylesheet();

public private:
    XmlSerializable(System::IntPtr unmanaged, System::Boolean autoDelete);

	inline FdoXmlSerializable* GetImpObj();

protected:
	System::Void ReleaseUnmanagedObject();
};

END_NAMESPACE_OSGEO_FDO_XML


