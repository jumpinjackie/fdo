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

class FdoXmlWriter;

BEGIN_NAMESPACE_OSGEO_COMMON_IO
public __gc class IoStream;
public __gc class IoTextWriter;
END_NAMESPACE_OSGEO_COMMON_IO

using namespace NAMESPACE_OSGEO_COMMON_IO;

BEGIN_NAMESPACE_OSGEO_COMMON_XML

/// \ingroup (OSGeoFDOCommonXml)
/// \brief
/// XmlWriter writes an XML document to a text or binary stream. 
/// Note: The XML document is not completely written until this object
/// is destroyed by releasing all references to it. Therefore, this object
/// must be destroyed before reading back the document being written.
public __gc class XmlWriter : public NAMESPACE_OSGEO_RUNTIME::Disposable
{
public:
    /// \brief
    /// XmlWriter::LineFormat specifies whether the output XML document
    /// has line breaks or indentation.
    /// 
    /// \param LineFormat_None 
    /// The document is written without line breaks or element indentation.
    /// \param LineFormat_Break 
    /// The document is written with a line break after:
    /// <ul>
    /// <li> every end element tag
    /// <li> every start tag for elements with complex content (sub-elements)
    /// </ul>
    /// Elements are not indented.
    /// \param LineFormat_Indent 
    /// Same as LineFormat_Break, except that each element immediately following 
    /// a line break is indented by a certain number of spaces. The number of
    /// spaces is the element's nesting level within the document X 3. The root
    /// element has nesting level 0.
    /// 
	__value enum LineFormat 
	{
		LineFormat_None = FdoXmlWriter::LineFormat_None,
		LineFormat_Break = FdoXmlWriter::LineFormat_Break,
		LineFormat_Indent = FdoXmlWriter::LineFormat_Indent
	};

    /// \brief
    /// Constructs an XML writer on a file
    /// 
    /// \param fileName 
    /// Input name of the file to write.
    /// \param defaultRoot 
    /// true: all elements written are wrapped in a default root element, named "DataStore".
    /// false: the default root element is not written. In this case, the first 
    /// element written (via  WriteStartElement() ) becomes the root element. 
    /// The caller is responsible for ensuring that only one root element is written 
    /// (the XML standard disallows multiple root elements).
    /// \param lineFormat 
    /// Input indentation string for XML elements. If NULL
    /// the XML document contains no line breaks and elements are not indented. 
    /// if L"" then the XML document has a line break after every end element
    /// tag but no elements are indented. Otherwise, this string must only 
    /// contain spaces or tabs; the XML document has a line break after every end element
    /// and each element start is prepended with a repetition of this string.
    /// The string is repeated according to the element's nesting level.
    /// \param lineLength 
    /// Input maximum line length. If 0 there is 
    /// no maximum. Otherwise, a line break is added before an XML attribute is
    /// written, if the attribute would have caused the current line to exceed
    /// this length.
    /// 
	XmlWriter(System::String* fileName, System::Boolean defaultRoot, NAMESPACE_OSGEO_COMMON_XML::XmlWriter::LineFormat lineFormat, System::UInt32 lineLength);

    /// \brief
    /// Constructs an XML writer on a file
    /// 
    /// \param fileName 
    /// Input name of the file to write.
    /// \param defaultRoot 
    /// true: all elements written are wrapped in a default root element, named "DataStore".
    /// false: the default root element is not written. In this case, the first 
    /// element written (via  WriteStartElement() ) becomes the root element. 
    /// The caller is responsible for ensuring that only one root element is written 
    /// (the XML standard disallows multiple root elements).
    /// \param lineFormat 
    /// Input indentation string for XML elements. If NULL
    /// the XML document contains no line breaks and elements are not indented. 
    /// if L"" then the XML document has a line break after every end element
    /// tag but no elements are indented. Otherwise, this string must only 
    /// contain spaces or tabs; the XML document has a line break after every end element
    /// and each element start is prepended with a repetition of this string.
    /// The string is repeated according to the element's nesting level.
    /// 
    XmlWriter(System::String* fileName, System::Boolean defaultRoot, NAMESPACE_OSGEO_COMMON_XML::XmlWriter::LineFormat lineFormat);

    /// \brief
    /// Constructs an XML writer on a file
    /// 
    /// \param fileName 
    /// Input name of the file to write.
    /// \param defaultRoot 
    /// true: all elements written are wrapped in a default root element, named "DataStore".
    /// false: the default root element is not written. In this case, the first 
    /// element written (via  WriteStartElement() ) becomes the root element. 
    /// The caller is responsible for ensuring that only one root element is written 
    /// (the XML standard disallows multiple root elements).
    /// 
	XmlWriter(System::String* fileName, System::Boolean defaultRoot);

    /// \brief
    /// Constructs an XML writer on a file
    /// 
    /// \param fileName 
    /// Input name of the file to write.
    /// 
    XmlWriter(System::String* fileName);

    /// \brief
    /// Constructs an XML writer on a stream
    /// 
    /// \param stream 
    /// Input the stream to write.
    /// \param defaultRoot 
    /// true: all elements written are wrapped in a default root element, named "DataStore".
    /// false: the default root element is not written. In this case, the first 
    /// element written (via  WriteStartElement() ) becomes the root element. 
    /// The caller is responsible for ensuring that only one root element is written 
    /// (the XML standard disallows multiple root elements).
    /// \param lineFormat 
    /// Input indentation string for XML elements. If NULL
    /// the XML document contains no line breaks and elements are not indented. 
    /// if L"" then the XML document has a line break after every end element
    /// tag but no elements are indented. Otherwise, this string must only 
    /// contain spaces or tabs; the XML document has a line break after every end element
    /// and each element start is prepended with a repetition of this string.
    /// The string is repeated according to the element's nesting level.
    /// \param lineLength 
    /// Input maximum line length. If 0 there is 
    /// no maximum. Otherwise, a line break is added before an XML attribute is
    /// written, if the attribute would have caused the current line to exceed
    /// this length.
    /// 
	XmlWriter(NAMESPACE_OSGEO_COMMON_IO::IoStream* stream, System::Boolean defaultRoot, NAMESPACE_OSGEO_COMMON_XML::XmlWriter::LineFormat lineFormat, System::UInt32 lineLength);

    /// \brief
    /// Constructs an XML writer on a stream
    /// 
    /// \param stream 
    /// Input the stream to write.
    /// \param defaultRoot 
    /// true: all elements written are wrapped in a default root element, named "DataStore".
    /// false: the default root element is not written. In this case, the first 
    /// element written (via  WriteStartElement() ) becomes the root element. 
    /// The caller is responsible for ensuring that only one root element is written 
    /// (the XML standard disallows multiple root elements).
    /// \param lineFormat 
    /// 
    XmlWriter(NAMESPACE_OSGEO_COMMON_IO::IoStream* stream, System::Boolean defaultRoot, NAMESPACE_OSGEO_COMMON_XML::XmlWriter::LineFormat lineFormat);

    /// \brief
    /// Constructs an XML writer on a stream
    /// 
    /// \param stream 
    /// Input the stream to write.
    /// \param defaultRoot 
    /// true: all elements written are wrapped in a default root element, named "DataStore".
    /// false: the default root element is not written. In this case, the first 
    /// element written (via  WriteStartElement() ) becomes the root element. 
    /// The caller is responsible for ensuring that only one root element is written 
    /// (the XML standard disallows multiple root elements).
    /// 
    XmlWriter(NAMESPACE_OSGEO_COMMON_IO::IoStream* stream, System::Boolean defaultRoot);

    /// \brief
    /// Constructs an XML writer on a stream
    /// 
    /// \param stream 
    /// Input the stream to write.
    /// 
	XmlWriter(NAMESPACE_OSGEO_COMMON_IO::IoStream* stream);

    /// \brief
    /// Constructs an XML writer on a text writer
    /// 
    /// \param writer 
    /// Input the text writer.
    /// \param defaultRoot 
    /// true: all elements written are wrapped in a default root element, named "DataStore".
    /// false: the default root element is not written. In this case, the first 
    /// element written (via  WriteStartElement() ) becomes the root element. 
    /// The caller is responsible for ensuring that only one root element is written 
    /// (the XML standard disallows multiple root elements).
    /// \param lineFormat 
    /// Input indentation string for XML elements. If NULL
    /// the XML document contains no line breaks and elements are not indented. 
    /// if L"" then the XML document has a line break after every end element
    /// tag but no elements are indented. Otherwise, this string must only 
    /// contain spaces or tabs; the XML document has a line break after every end element
    /// and each element start is prepended with a repetition of this string.
    /// The string is repeated according to the element's nesting level.
    /// \param lineLength 
    /// Input maximum line length. If 0 there is 
    /// no maximum. Otherwise, a line break is added before an XML attribute is
    /// written, if the attribute would have caused the current line to exceed
    /// this length.
    /// 
	XmlWriter(NAMESPACE_OSGEO_COMMON_IO::IoTextWriter* writer, System::Boolean defaultRoot, NAMESPACE_OSGEO_COMMON_XML::XmlWriter::LineFormat lineFormat, System::UInt32 lineLength);

    /// \brief
    /// Constructs an XML writer on a text writer
    /// 
    /// \param writer 
    /// Input the text writer.
    /// \param defaultRoot 
    /// true: all elements written are wrapped in a default root element, named "DataStore".
    /// false: the default root element is not written. In this case, the first 
    /// element written (via  WriteStartElement() ) becomes the root element. 
    /// The caller is responsible for ensuring that only one root element is written 
    /// (the XML standard disallows multiple root elements).
    /// \param lineFormat 
    /// Input indentation string for XML elements. If NULL
    /// the XML document contains no line breaks and elements are not indented. 
    /// if L"" then the XML document has a line break after every end element
    /// tag but no elements are indented. Otherwise, this string must only 
    /// contain spaces or tabs; the XML document has a line break after every end element
    /// and each element start is prepended with a repetition of this string.
    /// The string is repeated according to the element's nesting level.
    /// 
    XmlWriter(NAMESPACE_OSGEO_COMMON_IO::IoTextWriter* writer, System::Boolean defaultRoot, NAMESPACE_OSGEO_COMMON_XML::XmlWriter::LineFormat lineFormat);

    /// \brief
    /// Constructs an XML writer on a text writer
    /// 
    /// \param writer 
    /// Input the text writer.
    /// \param defaultRoot 
    /// true: all elements written are wrapped in a default root element, named "DataStore".
    /// false: the default root element is not written. In this case, the first 
    /// element written (via  WriteStartElement() ) becomes the root element. 
    /// The caller is responsible for ensuring that only one root element is written 
    /// (the XML standard disallows multiple root elements).
    /// 
    XmlWriter(NAMESPACE_OSGEO_COMMON_IO::IoTextWriter* writer, System::Boolean defaultRoot);

    /// \brief
    /// Constructs an XML writer on a text writer
    /// 
    /// \param writer 
    /// Input the text writer.
    /// 
    XmlWriter(NAMESPACE_OSGEO_COMMON_IO::IoTextWriter* writer);

    /// \brief
    /// Gets the underlying text writer. If a text writer was passed to this object
    /// then this text writeer is returned.
    /// Otherwise, an auto-generated text writer is returned (a text writer
    /// wrapped around the file name or stream that was passed to this object)
    /// 
    /// \return
    /// Returns the underlying text writer
    /// 
	__property NAMESPACE_OSGEO_COMMON_IO::IoTextWriter* get_TextWriter();

    /// \brief
    /// Gets the underlying stream. If a text writer was passed to this object
    /// then the stream for this text writer is returned.
    /// If a stream was passed to this object then this stream is returned.
    /// If a file name as passed then a auto-generated stream (wrapped around
    /// the file) is returned.
    /// 
    /// \return
    /// Returns the underlying stream
    /// 
	__property NAMESPACE_OSGEO_COMMON_IO::IoStream* get_Stream();

    /// \brief
    /// Gets the current default root state.
    /// 
    /// \return
    /// Returns true if the default root element will be written, false otherwise.
    /// 
	__property System::Boolean get_DefaultRoot();

    /// \brief
    /// Sets whether to write the default root element.
    /// Must be called before the first element is written,
    /// otherwise an exception is thrown
    /// 
    /// \param defaultRoot 
    /// true: use the default root element ("DataStore") as the root element.
    /// false: caller is responsible for writing the root element.
    /// 
	__property System::Void  set_DefaultRoot(System::Boolean defaultRoot );

    /// \brief
    /// Closes this XML Writer by writing end tags for all
    /// elements currently open. Once this function is called, no more
    /// elements can be added to the output document.
    /// 
    System::Void Close();

    /// \brief
    /// Writes an element start tag to the document
    /// 
    /// \param elementName 
    /// Input the element name. Must be a valid
    /// XML 1.0 element name.
    /// 
	System::Void WriteStartElement(System::String* elementName );

    /// \brief
    /// Writes the end tag for the current element to the document.
    /// An Exception is thrown if there is no element to end.
    /// 
	System::Void WriteEndElement();

    /// \brief
    /// Writes an attribute to the current element.
    /// An Exception is thrown if this function is called immediately after
    /// WriteEndElement().
    /// 
    /// \param attributeName 
    /// Input the element name. Must be a valid
    /// XML 1.0 attribute name.
    /// \param attributeValue 
    /// Input the element value.
    /// 
	System::Void WriteAttribute( System::String* attributeName, System::String* attributeValue );

    /// \brief
    /// Writes simple (character) content for the current element. This function
    /// can be called multiple times for the same element. The characters are appended
    /// to the element's content. An Exception is thrown if there is no current 
    /// element.
    /// 
    /// \param characters 
    /// Input characters to append to the element content.
    /// 
	System::Void WriteCharacters( System::String* characters );

    /// \brief
    /// Writes arbitrary bytes to the XML Writer. Caller is responsible
    /// for ensuring that the text does not introduce any errors into the 
    /// XML document.
    /// 
    /// \param bytes 
    /// Input characters to write.
    /// 
    /// \param count 
    /// Input the number of bytes to write
    /// 
	System::Void WriteBytes( System::Byte bytes [],  System::UInt32 count );

    /// \brief
    /// utility function that converts FDO element names to valid XML
    /// element or attribute names. Conversion is done by changing each invalid 
    /// character to a hex pattern ( "-xnnnn-" ).
    /// 
    /// \param name 
    /// Input the name to encode (convert).
    /// 
    /// \return
    /// Returns the encoded name.
    /// 
    System::String* EncodeName(System::String* name);

    /// \brief
    /// Checks if a string is a valid XML 1.0 element or attribute name.
    /// 
    /// \param name 
    /// Input the string to check.
    /// 
    /// \return
    /// Returns true if the string is a valid name, false otherwise.
    /// 
    System::Boolean IsValidName(System::String* name);

    /// \brief
    /// Given an element's or attribute's globally unique name ( uri and local name ),
    /// this function returns its fully qualified name as per the
    /// XML document being written, or the unqualified name if the uri is 
    /// the default namespace. This is done by searching for the current 
    /// namespace declaration that references the uri. 
    /// 
    /// \remarks
    /// If a matching default namespace declaration ( "xmlns=<uri>" ) was
    /// found and the name is for an element then the localName is returned.
    /// \note
	/// According to the XML 1.0 specification, default namespaces do 
    /// not apply to attribute names, so the default namespace is ignored when the 
    /// given name is not for an element.
    /// \n
    /// Otherwise, if a matching namespace declaration was found then 
    /// "[namespace]:[localName]" is returned.
    /// \n
    /// Otherwise, L"" is returned (unable to determine qualified name).
    /// 
    /// \param location 
    /// Input the URI corresponding to an element.
    /// \param localName 
    /// Input the local name corresponding to an element.
    /// \param isElement 
    /// Input a boolean flag indicating that the URI corresponds to an element.
    /// 
    /// \return
    /// Returns the fully qualified name corresponding to the uri.
    /// 
	System::String* UriToQName( System::String* location, System::String* localName, System::Boolean isElement);
	System::String* UriToQName( System::String* location, System::String* localName);

/// \cond DOXYGEN-IGNORE
protected:
	System::Void ReleaseUnmanagedObject();

public private:
    XmlWriter(System::IntPtr unmanaged, System::Boolean autoDelete);

	inline FdoXmlWriter* GetImpObj();
/// \endcond
};
END_NAMESPACE_OSGEO_COMMON_XML


