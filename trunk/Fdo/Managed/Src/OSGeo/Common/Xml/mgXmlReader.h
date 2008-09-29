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

#include "Runtime\Disposable.h"

class FdoXmlReader; 

BEGIN_NAMESPACE_OSGEO_COMMON_IO
public __gc class IoStream;
public __gc class IoTextReader;
END_NAMESPACE_OSGEO_COMMON_IO

BEGIN_NAMESPACE_OSGEO_COMMON
public __gc class DictionaryElementCollection;
END_NAMESPACE_OSGEO_COMMON

using namespace NAMESPACE_OSGEO_COMMON_IO;
using namespace NAMESPACE_OSGEO_COMMON;

BEGIN_NAMESPACE_OSGEO_COMMON_XML
public __gc __interface IXmlSaxHandler;
public __gc class XmlSaxContext;

/// \ingroup (OSGeoFDOCommonXml)
/// \brief
/// XmlReader reads an XML document from a text or binary stream. As various
/// document fragments are read, it calls the appropriate SAX Handler callback on 
/// the current SAX Handler. This object maintains a stack of SAX Handlers, with the 
/// current one being the top handler in the stack. Callers provide this object with
/// SAX Handlers to customize the processing of the XML document.
/// SAXHandler callbacks also provide a means to push other handlers onto the stack.
/// For example, a SAX Handler for a particular XML element might push another handler
/// to read a particular sub-element.
public __gc class XmlReader : public NAMESPACE_OSGEO_RUNTIME::Disposable
{
public:
    /// \brief
    /// Constructs an XML reader on a file
    /// 
    /// \param fileName 
    /// Input name of the file to read.
    /// 
    /// \return
    /// Returns XmlReader
    /// 
	XmlReader( System::String* fileName );

    /// \brief
    /// Constructs an XML reader on a stream
    /// 
    /// \param stream 
    /// Input the stream to read.
    /// 
    /// \return
    /// Returns XmlReader
    /// 
	XmlReader(NAMESPACE_OSGEO_COMMON_IO::IoStream* stream);

    /// \brief
    /// Constructs an XML reader on a text reader
    /// 
    /// \param reader 
    /// Input the text reader.
    /// 
    /// \return
    /// Returns XmlReader
    /// 
	XmlReader( NAMESPACE_OSGEO_COMMON_IO::IoTextReader* reader );

    /// \brief
    /// Gets the underlying text reader. If a text reader was passed to this object
    /// then this text reader is returned.
    /// Otherwise, an auto-generated text reader is returned (a text reader
    /// wrapped around the file name or stream that was passed to this object)
    /// 
    /// \return
    /// Returns the underlying text reader
    /// 
	__property NAMESPACE_OSGEO_COMMON_IO::IoTextReader* get_TextReader();

    /// \brief
    /// Gets the underlying stream. If a text reader was passed to this object
    /// then the stream for this text reader is returned.
    /// If a stream was passed to this object then this stream is returned.
    /// If a file name as passed then a auto-generated stream (wrapped around
    /// the file) is returned.
    /// 
    /// \return
    /// Returns the underlying stream
    /// 
	__property NAMESPACE_OSGEO_COMMON_IO::IoStream* get_Stream();

    /// \brief
    /// Parses the XML document.
    /// 
    /// \return
    /// Returns true if the end of the document has not yet been reached
    /// 
    System::Boolean Parse();

    /// \brief
    /// Parses the XML document.
    /// 
    /// \param saxHandler 
    /// Input SAX Handler to receive the SaxHandler events.
    /// This object is pushed onto the SAX Handler stack when parse() starts and popped
    /// when parse() is finished. If NULL then no handler is pushed, meaning that
    /// the current top SAX Handler receives the events. If saxHander is NULL
    /// and there is on current top SAX Handler then this function does a 
    /// parse and reports syntax errors, but does no semantic processing.
    /// 
    /// \return
    /// Returns true if the end of the document has not yet been reached
    /// 
    System::Boolean Parse(NAMESPACE_OSGEO_COMMON_XML::IXmlSaxHandler* saxHandler);

    /// \brief
    /// Parses the XML document.
    /// 
    /// \param saxHandler 
    /// Input SAX Handler to receive the SaxHandler events.
    /// This object is pushed onto the SAX Handler stack when parse() starts and popped
    /// when parse() is finished. If NULL then no handler is pushed, meaning that
    /// the current top SAX Handler receives the events. If saxHander is NULL
    /// and there is on current top SAX Handler then this function does a 
    /// parse and reports syntax errors, but does no semantic processing.
    /// \param saxContext 
    /// Input Caller-specific contextual information that is 
    /// pass to all SAX callbacks.
    /// 
    /// \return
    /// Returns true if the end of the document has not yet been reached
    /// 
    System::Boolean Parse(NAMESPACE_OSGEO_COMMON_XML::IXmlSaxHandler* saxHandler, NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext* saxContext);

    /// \brief
    /// Parses the XML document.
    /// 
    /// \param saxHandler 
    /// Input SAX Handler to receive the SaxHandler events.
    /// This object is pushed onto the SAX Handler stack when parse() starts and popped
    /// when parse() is finished. If NULL then no handler is pushed, meaning that
    /// the current top SAX Handler receives the events. If saxHander is NULL
    /// and there is on current top SAX Handler then this function does a 
    /// parse and reports syntax errors, but does no semantic processing.
    /// \param saxContext 
    /// Input Caller-specific contextual information that is 
    /// pass to all SAX callbacks.
    /// \param incremental 
    /// Input 
    /// true: an incremental (progressive) parse is performed. The first call 
    /// to XmlReader::Parse() causes the XML document to be read from the current 
    /// position  until the XmlSaxHandler::EndElement() callback returns false 
    /// or the end of the document is reached. On subsequent calls to Parse() the 
    /// read continues where the previous call left off. 
    /// false: the whole document is parsed in a single call to XmlReader::Parse(). 
    /// The XmlSaxHandler::EndElement() return value is ignored. If a previous call 
    /// was made to XmlReader::Parse(), with incremental = true, then the rest of the 
    /// document is parsed ( the EndElement() return value is ignored ).
    /// 
    /// \return
    /// Returns true if the end of the document has not yet been reached
    /// 
    System::Boolean Parse(NAMESPACE_OSGEO_COMMON_XML::IXmlSaxHandler* saxHandler, NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext* saxContext, System::Boolean incremental);

    /// \brief
    /// Utility function that is typically called for element names or 
    /// name type attributes that were adjusted when they were written to XML. 
    /// FDO names that correspond to XML names, but aren't valid XML names, are 
    /// adjusted. This function undoes the name adjustment.
    /// 
    /// \param name 
    /// Input the name to decode.
    /// 
    /// \return
    /// Returns the decoded name.
    /// 
    System::String* DecodeName(System::String* name);

    /// \brief
    /// Indicates whether the end of the XML document has been reached.
    /// 
    /// \return
    /// Returns true if this reader is at the end of the document, false otherwise
    /// 
	__property System::Boolean get_EOD();

    /// \brief
    /// Returns all of the XML namespace declarations that are currently
    /// in-scope for the current position in the XML document being read.
    /// 
    /// \return
    /// Returns DictionaryElementCollection. The dictionary has one DictionaryElement
    /// entry per namespace declaration. DictionaryElement->GetName() returns
    /// the namespace prefix. DictionaryElement->GetValue() returns the URI.
    /// 
 	__property DictionaryElementCollection* get_Namespaces();

    /// \brief
    /// Gets the URI for a namespace prefix.
    /// 
    /// \param prefix 
    /// Input the namespace prefix.
    /// 
    /// \return
    /// Returns the URI; L"" if the prefix is not in-scope at the
    /// current position in the XML document.
    /// 
	System::String* PrefixToUri( System::String* prefix );	

/// \cond DOXYGEN-IGNORE
protected:
	System::Void ReleaseUnmanagedObject();

public private:
    XmlReader(System::IntPtr unmanaged, System::Boolean autoDelete);

	inline FdoXmlReader* GetImpObj();
/// \endcond
};

END_NAMESPACE_OSGEO_COMMON_XML


