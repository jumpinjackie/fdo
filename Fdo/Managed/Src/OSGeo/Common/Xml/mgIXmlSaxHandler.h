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

BEGIN_NAMESPACE_OSGEO_COMMON_XML
public __gc class XmlAttributeCollection;
public __gc class XmlSaxContext;

/// \brief
/// IXmlSaxHandler defines the SAX Handler callback interface.
/// It is not an interface in the strict sense, since each function has a 
/// default implementation that does nothing. Implementors can override these
/// callbacks to customize the handling of XML fragments parsed by XmlReader.
public __gc __interface IXmlSaxHandler : public System::IDisposable
{
public:
    /// \brief
    /// Default Sax callback that is called when the XmlReader 
    /// starts reading an XML document. Does nothing.
    /// 
    /// \param context 
    /// Input caller specified contextual information
    /// 
    /// \return
    /// Returns the SAX Handler for the document's root element. If NULL then 
    /// this SAX handler will handle the root element
    /// 
	NAMESPACE_OSGEO_COMMON_XML::IXmlSaxHandler* XmlStartDocument(NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext* context);

    /// \brief
    /// Default Sax callback that is called when the XmlReader 
    /// finishes reading an XML document. Does nothing
    /// 
    /// \param context 
    /// Input caller specified contextual information
    /// 
	System::Void XmlEndDocument(NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext* context);

    /// \brief
    /// Default Sax callback that is called when the XmlReader 
    /// reads the start tag for an XML element in the document. Does nothing.
    /// 
    /// \param context 
    /// Input caller specified contextual information
    /// \param resourceLocation 
    /// Input the element's Universal Resource Indicator
    /// \param name 
    /// Input the unqualified element name (doesn't include namespace)
    /// \param qualifiedName 
    /// Input the qualified element name (includes namespace)
    /// \param attributes 
    /// Input the attributes for the element.
    /// 
    /// \return
    /// Returns the SAX Handler for the element's sub-elements. If NULL then 
    /// this SAX handler will handle the sub-elements
    /// 
	NAMESPACE_OSGEO_COMMON_XML::IXmlSaxHandler* XmlStartElement(NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext* context, System::String* resourceLocation, System::String* name, System::String* qualifiedName, NAMESPACE_OSGEO_COMMON_XML::XmlAttributeCollection* attributes);

    /// \brief
    /// Default Sax callback that is called when the XmlReader 
    /// reads the end tag for an XML element in the document. Does nothing.
    /// 
    /// \param context 
    /// Input caller specified contextual information
    /// \param resourceLocation 
    /// Input the element's Universal Resource Indicator
    /// \param name 
    /// Input the unqualified element name (doesn't include namespace)
    /// \param qualifiedName 
    /// Input the qualified element name (includes namespace)
    /// 
    /// \return
    /// Returning true causes the current parse to stop. Returning false
    /// cause the parse to continue. Return value is ignored if the current parse
    /// is not an incremental parse ( see XmlReader::Parse())
    /// 
	System::Boolean XmlEndElement(NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext* context, System::String* resourceLocation, System::String* name, System::String* qualifiedName);

    /// \brief
    /// Default Sax callback that is called when the XmlReader 
    /// reads a chunk of simple content for the current element. Does nothing by default.
    /// This function may be called multiple times for the same element, if the 
    /// the content is long. Applications must not make any assumptions about the
    /// chunk size or number of chunks for each element
    /// 
    /// \param context 
    /// Input caller specified contextual information
    /// \param characters 
    /// Input the next chunk of simple content
    /// 
	System::Void XmlCharacters(NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext* context, System::String* characters);
};
END_NAMESPACE_OSGEO_COMMON_XML


