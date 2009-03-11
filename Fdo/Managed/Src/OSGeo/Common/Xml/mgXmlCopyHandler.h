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

#include "Common\Xml\mgXmlSaxHandler.h"

class FdoXmlCopyHandler;

BEGIN_NAMESPACE_OSGEO_COMMON
ref class DictionaryElementCollection;
END_NAMESPACE_OSGEO_COMMON

BEGIN_NAMESPACE_OSGEO_COMMON_XML
ref class XmlWriter;
ref class XmlAttributeCollection;

/// \ingroup (OSGeoFDOCommonXml)
/// \brief
/// XmlCopyHandler can be used to copy a set of elements from one XML
/// document to another. It can be created and set as the SAX Handler for an XML Reader
/// on the document to copy from. An XML Writer to the document to copy to is 
/// passed to one of the Create methods below. The elements to copy will be inserted
/// at the XML writer's current position.
public ref class XmlCopyHandler sealed : public NAMESPACE_OSGEO_COMMON_XML::XmlSaxHandler
{
public:
    /// \brief
    /// Constructs a Copy Handler. When attached (as the SAX Handler) to 
    /// an XmlReader, this object copies all sub-elements of the current element
    /// being parsed.
    /// 
    /// \param writer 
    /// Input the sub-elements are written to this XML writer.
    /// 
    /// \return
    /// Returns XmlCopyHandler
    /// 
	XmlCopyHandler( NAMESPACE_OSGEO_COMMON_XML:: XmlWriter^ writer );

    /// \brief
    /// Constructs a Copy Handler. When attached (as the SAX Handler) to 
    /// a XmlReader, this object copies the current element
    /// being parsed, plus all of its sub-elements. 
    /// 
    /// \param writer 
    /// Input the sub-elements are written to this XML writer.
    /// \param resourceLocation 
    /// Input current element's Universal Resource Indicator
    /// \param name 
    /// Input the unqualified name of the current element (doesn't include namespace)
    /// \param qualifiedName 
    /// Input the qualified name of the current element(includes namespace)
    /// \param attributes 
    /// Input the attributes for the current element. 
    /// <p><b>Note:</b> If the 
    /// current element will become the root element in the destination 
    /// document, the caller is responsible for including all required xmlns: namespace
    /// declarations in the atts list.
    /// \param namespaces 
    /// Input list of XML namespace declarations to write to the 
    /// destination document.
    /// 
    /// \return
    /// Returns XmlCopyHandler
    /// 
	NAMESPACE_OSGEO_COMMON_XML::XmlCopyHandler(NAMESPACE_OSGEO_COMMON_XML::XmlWriter^ writer, String^ resourceLocation, String^ name,  String^ qualifiedName,  XmlAttributeCollection^ attributes, DictionaryElementCollection^ namespaces);
		
    /// \brief
    /// Constructs a CopyHandler based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    /// Input A Pointer to the unmanaged object.
    /// 
    /// \param autoDelete 
    /// Input Indicates if the constructed object should be automatically deleted 
    /// once it no longer referenced.
    /// 
	XmlCopyHandler(System::IntPtr unmanaged, System::Boolean autoDelete);

/// \cond DOXYGEN-IGNORE
internal:
	inline FdoXmlCopyHandler* GetImpObj();
/// \endcond
};

END_NAMESPACE_OSGEO_COMMON_XML


