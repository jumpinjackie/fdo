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

#include "FDO\Xml\mgIXmlDeserializable.h"
#include "FDO\Xml\mgIXmlSerializable.h"

class FdoPhysicalSchemaMappingCollection;

BEGIN_NAMESPACE_OSGEO_COMMON_XML
public __gc class XmlSaxContext;
public __gc class XmlSaxHandler;
public __gc class XmlAttributeCollection;
END_NAMESPACE_OSGEO_COMMON_XML

BEGIN_NAMESPACE_OSGEO_FDO_XML
public __gc class XmlFlags;
END_NAMESPACE_OSGEO_FDO_XML

BEGIN_NAMESPACE_OSGEO_FDO_CONNECTIONS
public __gc __interface IConnection;
END_NAMESPACE_OSGEO_FDO_CONNECTIONS

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA
public __gc class PhysicalSchemaMapping;

/// \brief
/// FdoPhysicalSchemaMappingCollection defines a collection of Schema Override
/// sets. This is a generic collection which can contain override sets for different 
/// FDO Providers.
[System::Reflection::DefaultMemberAttribute("RealTypeItem")]
public __sealed __gc class PhysicalSchemaMappingCollection : public NAMESPACE_OSGEO_RUNTIME::Disposable, public System::Collections::IList, public NAMESPACE_OSGEO_FDO_XML::IXmlDeserializable, public NAMESPACE_OSGEO_FDO_XML::IXmlSerializable
{
private:
    /// \brief
    /// 		nested class defined to provide Enumerator.
    /// 
	__gc class Enumerator : public System::Collections::IEnumerator
	{
	private:
		PhysicalSchemaMappingCollection* m_pCol;
		System::Int32 m_nIdx;

	public:
		Enumerator(PhysicalSchemaMappingCollection* elements) : m_pCol(elements), m_nIdx(-1) 
		{

		}

		__property System::Object* get_Current();

		System::Boolean MoveNext();

		System::Void Reset();
	};

public private:
	PhysicalSchemaMappingCollection(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_RUNTIME::Disposable(unmanaged, autoDelete)
	{

	}

	inline FdoPhysicalSchemaMappingCollection* GetImpObj();

private:
    // System::Collections::ICollection interfaces
	System::Void System::Collections::ICollection::CopyTo(System::Array* array,System::Int32 index);

	__property System::Object* System::Collections::ICollection::get_SyncRoot();

	__property System::Boolean System::Collections::ICollection::get_IsSynchronized();

    // System::Collections::IList interfaces
	__property System::Boolean System::Collections::IList::get_IsFixedSize();

	__property System::Boolean System::Collections::IList::get_IsReadOnly();

	System::Int32 System::Collections::IList::Add(Object* value);

	System::Boolean System::Collections::IList::Contains(Object* value);

	System::Int32 System::Collections::IList::IndexOf(Object* value);

	System::Void System::Collections::IList::Insert(System::Int32 index, Object* value);

	System::Void System::Collections::IList::Remove(Object* value);

	__property Object* System::Collections::IList::get_Item(System::Int32 index);

	__property System::Void  System::Collections::IList::set_Item(System::Int32 index, Object* value);

public:
    /// \brief
    ///     Constructs a new empty string collection
    /// 
    /// \return
    ///     Returns FdoStringCollection
    /// 
    ///  
	PhysicalSchemaMappingCollection();

    /// \brief
    ///    Gets the count of items in collection.
    /// 
	__property System::Int32 get_Count(System::Void);

    /// \brief
    ///    Gets an enumerator that can iterate through a collection.
    /// 
	__sealed System::Collections::IEnumerator* GetEnumerator(System::Void);

    /// \brief
    ///     Removes the index-th NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping from this collection.
    /// 
    /// \param index 
    /// Input index of the element to remove.
    /// 
	System::Void RemoveAt(System::Int32 index);

    /// \brief
    ///     Removes all elements from the collection.
    /// 
	System::Void  Clear();

    /// \brief
    ///     Adds a NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping object into the collection.
    /// 
    /// \param value 
    /// Input the NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping object to add.
    /// 
    /// \return
    /// 		Returns the position into which the new element was inserted.
    /// 
	System::Int32 Add(NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping* value);

    /// \brief
    ///     Determines the index of a specific NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping object.
    /// 
    /// \param value 
    /// Input the NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping object to locate in the collection.
    /// 
    /// \return
    /// 		The index of value if found in the collection; otherwise, -1.
    /// 
	System::Int32 IndexOf(NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping* value);

    /// \brief
    ///     Inserts a NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping object into the collection at the specified position.
    /// 
    /// \param index 
    /// Input the zero-based index at which value should be inserted.
    /// \param value 
    /// Input the NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping object to insert.
    /// 
	System::Void Insert(System::Int32 index, NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping* value);

    /// \brief
    ///     Removes the first occurrence of a specific NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping object.
    /// 
    /// \param value 
    /// Input the NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping object to remove from the collection.
    /// 
	System::Void Remove(NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping* value);

    /// \brief
    ///     Determines whether the collection contains a specific NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping object.
    /// 
    /// \param value 
    /// Input The NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping object to search in the collection.
    /// 
    /// \return
    /// 		Returns true if the value is found in the collection; otherwise, false.
    /// 
	System::Boolean Contains(NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping* value);

    /// \brief
    ///     Copies the elements of the collection to an array.
    /// 
    /// \param array 
    /// Output the one-dimensional Array that is the destination of the elements copied from this collection.
    /// \param startAt 
    /// Input an integer that represents the index in array at which copying begins.
    /// 
	System::Void CopyTo(NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping* array[],System::Int32 size);


    /// \brief
    ///     Gets or sets a NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping in the collection.
    /// 
    /// \param index 
    /// Input index of the NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping to retrieve or set (System::Int32).
    /// 
	__property NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping* get_RealTypeItem(System::Int32 index);

	__property System::Void  set_RealTypeItem(System::Int32 index, NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping* value);

	__property NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping* get_Item(System::Int32 index);

	__property System::Void  set_Item(System::Int32 index, NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping* value);

	/* 
	*	Special interface implementation
	*/

    /// \brief
    /// Gets the Schema Override set in the collection at the specified index. Throws an invalid argument exception if the index is out of range.
    /// 
    /// \param index 
    /// Input index
    /// 
    /// \return
    /// Returns the Schema Override set at the specified index
    /// 
	NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping* GetItem(System::Int32 item);

    /// \brief
    /// Gets the Schema Override set for the specified 
    /// FDO provider and Feature Schema.
    /// 
    /// \param providerName 
    /// Input FDO Provider name of the form "[Company].[Provider].[Version]".
    /// \param schemaName 
    /// Input Feature Schema name.
    /// 
    /// \return
    /// the Schema Override set for the given FDO Provider and Feature 
    /// Schema, where the  [company] and [provider] parts of the set’s providerName 
    /// and the given providerName match exactly. The set with the highest 
    /// [version], that is less than or equal to the [version] of the given provider, 
    /// is returned.  NULL is return if a set was not found.
    /// 
	NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping * GetItem(System::String* providerName, System::String* schemaName);

    /// \brief
    /// Gets the Schema Override set for the specified Feature Schema and the
    /// FDO provider for the specified connection. This function allows an application 
    /// to retrieve the Schema Overrides for an active provider without knowing the 
    /// provider name.
    /// 
    /// \param connection 
    /// Input the FDO connection.
    /// \param schemaName 
    /// Input Feature Schema name.
    /// 
    /// \return
    /// Returns the Schema Override set for the given Feature Schema and 
    /// the FDO Provider for the given connection. NULL is return if the set was not 
    /// found.
    /// 
	NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping * GetItem(NAMESPACE_OSGEO_FDO_CONNECTIONS::IConnection* connection, System::String* schemaName);

	/*
		IXmlSaxHandler Implemenations
	*/
    /// \brief
    /// Default Sax callback that is called when the FdoXmlReader 
    /// starts reading an XML document. Does nothing.
    /// 
    /// \param saxContext 
    /// Input caller specified contextual information
    /// 
    /// \return
    /// Returns the SAX Handler for the document's root element. If NULL then 
    /// this SAX handler will handle the root element
    /// 
	NAMESPACE_OSGEO_COMMON_XML::IXmlSaxHandler* XmlStartDocument(NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext* context);

    /// \brief
    /// Default Sax callback that is called when the FdoXmlReader 
    /// finishes reading an XML document. Does nothing
    /// 
    /// \param saxContext 
    /// Input caller specified contextual information
    /// 
	System::Void XmlEndDocument(NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext* context);

    /// \brief
    /// Default Sax callback that is called when the FdoXmlReader 
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
	NAMESPACE_OSGEO_COMMON_XML::IXmlSaxHandler* XmlStartElement(NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext* context, String* resourceLocation, String* name, String* qualifiedName, NAMESPACE_OSGEO_COMMON_XML::XmlAttributeCollection* attributes);

    /// \brief
    /// Default Sax callback that is called when the FdoXmlReader 
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
    /// is not an incremental parse ( see FdoXmlReader::Parse())
    /// 
	System::Boolean XmlEndElement(NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext* context, System::String* resourceLocation, System::String* name, System::String* qualifiedName);

    /// \brief
    /// Default Sax callback that is called when the FdoXmlReader 
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

	/*
		IXmlDeserializable Implemenations
	*/
    /// \brief
    /// Reads from an XML document in a file.
    /// 
    /// \param fileName 
    /// Input the file name. Must consist of an XML document.
    /// \param flags 
    /// Input controls the reading of the elements from the document.
    /// If NULL then the default flags are used (see NAMESPACE_OSGEO_FDO_XML::XmlFlags::Create())
    /// 
	System::Void ReadXml(System::String* fileName);

	System::Void ReadXml(System::String* fileName, NAMESPACE_OSGEO_FDO_XML::XmlFlags* flags);

    /// \brief
    /// Reads an XML document from an XML reader.
    /// 
    /// \param reader 
    /// Input the XML reader.
    /// \param flags 
    /// Input controls the reading of the elements from the document. 
    /// If NULL then the default flags are used (see NAMESPACE_OSGEO_FDO_XML::XmlFlags::Create())
    /// 
	System::Void ReadXml(NAMESPACE_OSGEO_COMMON_XML::XmlReader* xmlReader);

	System::Void ReadXml(NAMESPACE_OSGEO_COMMON_XML::XmlReader* xmlReader, NAMESPACE_OSGEO_FDO_XML::XmlFlags* flags);

    /// \brief
    /// Reads an XML document from a text reader.
    /// 
    /// \param reader 
    /// Input the text reader. Must be positioned at the
    /// start of an XML document.
    /// \param flags 
    /// Input controls the reading of the elements from the document. 
    /// If NULL then the default flags are used (see NAMESPACE_OSGEO_FDO_XML::XmlFlags::Create())
    /// 
	System::Void ReadXml(NAMESPACE_OSGEO_COMMON_IO::IoTextReader* textReader);

	System::Void ReadXml(NAMESPACE_OSGEO_COMMON_IO::IoTextReader* textReader, NAMESPACE_OSGEO_FDO_XML::XmlFlags* flags);

    /// \brief
    /// Reads an XML document from a stream.
    /// 
    /// \param reader 
    /// Input the stream. Must be positioned at the
    /// start of an XML document.
    /// \param flags 
    /// Input controls the reading of the elements from the document. 
    /// If NULL then the default flags are used (see NAMESPACE_OSGEO_FDO_XML::XmlFlags::Create())
    /// 
	System::Void ReadXml(NAMESPACE_OSGEO_COMMON_IO::IoStream* stream);

	System::Void ReadXml(NAMESPACE_OSGEO_COMMON_IO::IoStream* stream, NAMESPACE_OSGEO_FDO_XML::XmlFlags* flags);

    /// \brief
    /// Gets the XML Flags that were passed to the ReadXml() 
    /// function that is currently being executed. This function would 
    /// typically be called by the FdoXmlSaxHandler callbacks.
    /// 
    /// \return
    /// Returns the XML Flags
    /// 
	NAMESPACE_OSGEO_FDO_XML::XmlFlags* GetDeserializationFlags();

    /// \brief
    /// Gets the current XML Reader. This function would 
    /// typically be called by the FdoXmlSaxHandler callbacks when 
    /// ReadXml() is being invoked. When ReadXml() reads from a file, stream,
    /// or text reader, a wrapping XML reader is automatically created.
    /// 
    /// \return
    /// Returns the XML reader
    /// 
	NAMESPACE_OSGEO_COMMON_XML::XmlReader* GetXmlReader();

    /// \brief
    /// Gets the stylesheet for converting the XML document from 
    /// external to internal format. When classes derived from FdoXmlDeserializable
    /// define an internal format, they must override this function to return a
    /// stylesheet that does the conversion.
    /// 
    /// \return
    /// Returns NULL by default (no internal format defined)
    /// 
	NAMESPACE_OSGEO_COMMON_XML::XmlReader* GetFromExternalStylesheet();

    /// \brief
    /// Gets the SAX context to pass to the FdoXmlSaxHandler callbacks.
    /// Classes derived from FdoXmlDeserializable can override this function to 
    /// specify a SAX context with class-specific information.
    /// 
    /// \return
    /// Returns NULL by default, the default NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext is used.
    /// The default provides basic error handling functionality.
    /// 
	NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext* GetSaxContext();

	/*
		IXmlSerializable Implemenations
	*/
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

protected:
	System::Void Dispose(System::Boolean disposing);
};

END_NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA


