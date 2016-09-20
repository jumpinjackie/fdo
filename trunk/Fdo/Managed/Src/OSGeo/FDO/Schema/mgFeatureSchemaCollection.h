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

class FdoFeatureSchemaCollection;

BEGIN_NAMESPACE_OSGEO_FDO
ref class IDisposableCollection;
END_NAMESPACE_OSGEO_FDO

BEGIN_NAMESPACE_OSGEO_FDO_XML
ref class XmlFlags;
END_NAMESPACE_OSGEO_FDO_XML

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA
ref class PhysicalSchemaMappingCollection;
END_NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA

BEGIN_NAMESPACE_OSGEO_FDO_SCHEMA
ref class FeatureSchema;
ref class SchemaElement;

/// \ingroup (OSGeoFDOSchema)
/// \brief
/// The FeatureSchemaCollection class represents a collection of FeatureSchema objects.
[System::Reflection::DefaultMemberAttribute("Item")]
public ref class FeatureSchemaCollection sealed : public NAMESPACE_OSGEO_COMMON::CollectionBase, public NAMESPACE_OSGEO_FDO_XML::IXmlDeserializable, public NAMESPACE_OSGEO_FDO_XML::IXmlSerializable
{
/// \cond DOXYGEN-IGNORE
internal:
	inline FdoFeatureSchemaCollection* GetImpObj();
public:
    virtual IntPtr GetDisposableObject() override;

private:
    virtual property System::Object^ IndexInternal[System::Int32]
    {
        private: System::Object^ get(System::Int32 index) sealed = IList::default::get;
        private: void set(System::Int32 index, System::Object^ value) sealed = IList::default::set;
    }

    // System::Collections::IList interface methods
    virtual System::Int32 Add(System::Object^ value) sealed = IList::Add;
    virtual System::Boolean Contains(System::Object^ value) sealed = IList::Contains;
    virtual System::Int32 IndexOf(System::Object^ value) sealed = IList::IndexOf;
    virtual System::Void Insert(System::Int32 index, System::Object^ value) sealed = IList::Insert;
    virtual System::Void Remove(System::Object^ value) sealed = IList::Remove;

/// \endcond

public:
    /// \brief
    /// Constructs a FeatureSchemaCollection object
    /// 
    /// \param parent 
    /// Input A Pointer to the parent schema object of the collection
    /// 
	FeatureSchemaCollection(NAMESPACE_OSGEO_FDO_SCHEMA::SchemaElement^ parent);

    /// \brief
    /// Constructs a FeatureSchemaCollection object based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    /// Input A Pointer to the unmanaged object.
    /// 
    /// \param autoDelete 
    /// Input Indicates if the constructed object should be automatically deleted 
    /// once it no longer referenced.
    /// 
    FeatureSchemaCollection(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_COMMON::CollectionBase(unmanaged, autoDelete)
	{
	}

    /// \brief
    /// Gets the count of items in collection.
    /// 
    /// \return
    /// Returns the number of items in the collection.
    /// 
    property System::Int32 Count
    {
        virtual System::Int32 get() override;
    }

    /// \brief
    /// Removes the index-th FeatureSchema from this collection.
    /// 
    /// \param index 
    /// Input index of the element to remove.
    /// 
    virtual System::Void RemoveAt(System::Int32 index) override;

    /// \brief
    /// Removes all elements from the collection.
    /// 
    virtual System::Void  Clear() override;

    /// \brief
    /// Adds a FeatureSchema object into the collection.
    /// 
    /// \param value 
    /// Input the FeatureSchema object to add.
    /// 
    /// \return
    /// Returns the position into which the new element was inserted.
    /// 
	System::Int32 Add(FeatureSchema^ value);

    /// \brief
    /// Determines the index of a specific FeatureSchema object.
    /// 
    /// \param value 
    /// Input the FeatureSchema object to locate in the collection.
    /// 
    /// \return
    /// The index of value if found in the collection; otherwise, -1.
    /// 
	System::Int32 IndexOf(FeatureSchema^ value);

    /// \brief
    /// Determines the index of a specific FeatureSchema object.
    /// 
    /// \param name 
    /// Input the name of the FeatureSchema object to locate in the collection.
    /// 
    /// \return
    /// The index of value if found in the collection; otherwise, -1.
    /// 
	System::Int32 IndexOf(String^ name);

    /// \brief
    /// Inserts a FeatureSchema object into the collection at the specified position.
    /// 
    /// \param index 
    /// Input the zero-based index at which value should be inserted.
    /// \param value 
    /// Input the FeatureSchema object to insert.
    /// 
	System::Void Insert(System::Int32 index, FeatureSchema^ value);

    /// \brief
    /// Removes the first occurrence of a specific FeatureSchema object.
    /// 
    /// \param value 
    /// Input the FeatureSchema object to remove from the collection.
    /// 
	System::Void Remove(FeatureSchema^ value);

    /// \brief
    /// Determines whether the collection contains a specific FeatureSchema object.
    /// 
    /// \param value 
    /// Input The FeatureSchema object to search in the collection.
    /// 
    /// \return
    /// Returns true if the value is found in the collection; otherwise, false.
    /// 
	System::Boolean Contains(FeatureSchema^ value);

    /// \brief
    /// Determines whether the collection contains a specific FeatureSchema object.
    /// 
    /// \param name 
    /// Input The name of the FeatureSchema object to search in the collection.
    /// 
    /// \return
    /// Returns true if the value is found in the collection; otherwise, false.
    /// 
	System::Boolean Contains(String^ name);

    /// \brief
    /// Copies the elements of the collection to an array.
    /// 
    /// \param array 
    /// Output the one-dimensional Array that is the destination of the elements copied from this collection.
    /// \param startAt 
    /// Input an integer that represents the index in array at which copying begins.
    /// 
    System::Void CopyTo(array<FeatureSchema^>^ pArray, System::Int32 index);

    /// \brief
    /// Gets the item in the collection by name. 
    /// 
    /// \param index 
    /// The name of the item in the collection.
    /// 
    /// \return
    /// Returns an instance of a the collected item.
    /// 
    property FeatureSchema^ Item[System::String^]
    {
        FeatureSchema^ get(System::String^ index);
    }

    /// \brief
    /// Sets the value of the item at the specified index
    /// 
    /// \param index 
    /// Input index of the item to set.
    /// 
    /// \param value 
    /// Input the value of the item
    /// 
    /// \brief
    /// Gets the item in the collection at the specified index. 
    /// 
    /// \param index 
    /// The index of the item in the collection. The index is 0 based.
    /// 
    /// \return
    /// Returns an instance of a the collected item.
    /// Throws an instance of Exception if the index is out of range or an error occurs.
    /// 
    property FeatureSchema^ Item[System::Int32]
    {
        FeatureSchema^ get(System::Int32 index);
        System::Void set(System::Int32 index, FeatureSchema^ value);
    }

	/*
		Additional interfaces
	*/

    /// \brief
    /// Gets the stylesheet for converting a Feature Schema collection from 
    /// internal to external XML format. The internal format is very similar to 
    /// the Feature Schema class hierarchy. The external format is OGC GML.
    /// 
    /// \return
    /// Returns the stylesheet
    /// 
	virtual NAMESPACE_OSGEO_COMMON_XML::XmlReader^ GetFromInternalStylesheet();

    /// \brief
    /// Gets the stylesheet for converting a Feature Schema collection from 
    /// external to internal XML format. The internal format is very similar to 
    /// the Feature Schema class hierarchy. The external format is OGC GML.
    /// 
    /// \return
    /// Returns the stylesheet
    /// 
	virtual NAMESPACE_OSGEO_COMMON_XML::XmlReader^ GetFromExternalStylesheet();

    /// \brief
    /// Takes a qualified or unqualified class name and return a collection of class definitions
    /// that match the class name. For qualified name, the collection contains at most one class definition.
    /// 
    /// \param className 
    /// The class name
    /// 
    /// \return
    /// Returns a collection of classes
    /// 
	virtual NAMESPACE_OSGEO_FDO::IDisposableCollection^ FindClass(System::String^ className);

    /// \brief
    /// Gets a Schema Mapping Collection
    /// that describes the mappings between the elements in the GML Schema that were read
    /// and the FDO Feature Schema Elements that were created. This function returns the 
    /// Schema Mapping Collection generated by the last call to ReadXml(). The collection
    /// contains one XmlSchemaMapping per schema that was read.
    /// 
    /// \return
    /// Returns PhysicalSchemaMappingCollection^. Returns nullptr if ReadXml() has not been
    /// called on this object.
    /// 
    property NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMappingCollection^ XmlSchemaMappings
    {
        virtual NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMappingCollection^ get();
    }

	/*
		IXmlSaxHandler Implemenations
	*/

    /// \brief
    /// Default Sax callback that is called when the XmlReader 
    /// starts reading an XML document. Does nothing.
    /// 
    /// \param context 
    /// Input caller specified contextual information
    /// 
    /// \return
    /// Returns the SAX Handler for the document's root element. If nullptr then 
    /// this SAX handler will handle the root element
    /// 
	virtual NAMESPACE_OSGEO_COMMON_XML::IXmlSaxHandler^ XmlStartDocument(NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext^ context);

    /// \brief
    /// Default Sax callback that is called when the XmlReader 
    /// finishes reading an XML document. Does nothing
    /// 
    /// \param context 
    /// Input caller specified contextual information
    /// 
	virtual System::Void XmlEndDocument(NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext^ context);

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
    /// Returns the SAX Handler for the element's sub-elements. If nullptr then 
    /// this SAX handler will handle the sub-elements
    /// 
	virtual NAMESPACE_OSGEO_COMMON_XML::IXmlSaxHandler^ XmlStartElement(NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext^ context, String^ resourceLocation, String^ name, String^ qualifiedName, NAMESPACE_OSGEO_COMMON_XML::XmlAttributeCollection^ attributes);

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
	virtual System::Boolean XmlEndElement(NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext^ context, System::String^ resourceLocation, System::String^ name, System::String^ qualifiedName);

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
	virtual System::Void XmlCharacters(NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext^ context, System::String^ characters);

	/*
		IXmlDeserializable Implemenations
	*/

    /// \brief
    /// Reads from an XML document in a file.
    /// 
    /// \param fileName 
    /// Input the file name. Must consist of an XML document.
    /// 
	virtual System::Void ReadXml(System::String^ fileName);

    /// \brief
    /// Reads from an XML document in a file.
    /// 
    /// \param fileName 
    /// Input the file name. Must consist of an XML document.
    /// \param flags 
    /// Input controls the reading of the elements from the document.
    /// If nullptr then the default flags are used (see NAMESPACE_OSGEO_FDO_XML::XmlFlags::Create())
    /// 
	virtual System::Void ReadXml(System::String^ fileName, NAMESPACE_OSGEO_FDO_XML::XmlFlags^ flags);

    /// \brief
    /// Reads an XML document from an XML reader.
    /// 
    /// \param xmlReader 
    /// Input the XML reader.
    /// 
	virtual System::Void ReadXml(NAMESPACE_OSGEO_COMMON_XML::XmlReader^ xmlReader);

    /// \brief
    /// Reads an XML document from an XML reader.
    /// 
    /// \param xmlReader 
    /// Input the XML reader.
    /// \param flags 
    /// Input controls the reading of the elements from the document. 
    /// If nullptr then the default flags are used (see NAMESPACE_OSGEO_FDO_XML::XmlFlags::Create())
    /// 
	virtual System::Void ReadXml(NAMESPACE_OSGEO_COMMON_XML::XmlReader^ xmlReader, NAMESPACE_OSGEO_FDO_XML::XmlFlags^ flags);

    /// \brief
    /// Reads an XML document from a text reader.
    /// 
    /// \param textReader 
    /// Input the text reader. Must be positioned at the
    /// start of an XML document.
    /// 
	virtual System::Void ReadXml(NAMESPACE_OSGEO_COMMON_IO::IoTextReader^ textReader);

    /// \brief
    /// Reads an XML document from a text reader.
    /// 
    /// \param textReader 
    /// Input the text reader. Must be positioned at the
    /// start of an XML document.
    /// \param flags 
    /// Input controls the reading of the elements from the document. 
    /// If nullptr then the default flags are used (see NAMESPACE_OSGEO_FDO_XML::XmlFlags::Create())
    /// 
	virtual System::Void ReadXml(NAMESPACE_OSGEO_COMMON_IO::IoTextReader^ textReader, NAMESPACE_OSGEO_FDO_XML::XmlFlags^ flags);

    /// \brief
    /// Reads an XML document from a stream.
    /// 
    /// \param stream 
    /// Input the stream. Must be positioned at the
    /// start of an XML document.
    /// 
	virtual System::Void ReadXml(NAMESPACE_OSGEO_COMMON_IO::IoStream^ stream);

    /// \brief
    /// Reads an XML document from a stream.
    /// 
    /// \param stream 
    /// Input the stream. Must be positioned at the
    /// start of an XML document.
    /// \param flags 
    /// Input controls the reading of the elements from the document. 
    /// If nullptr then the default flags are used (see NAMESPACE_OSGEO_FDO_XML::XmlFlags::Create())
    /// 
	virtual System::Void ReadXml(NAMESPACE_OSGEO_COMMON_IO::IoStream^ stream, NAMESPACE_OSGEO_FDO_XML::XmlFlags^ flags);

    /// \brief
    /// Gets the XML Flags that were passed to the ReadXml() 
    /// function that is currently being executed. This function would 
    /// typically be called by the XmlSaxHandler callbacks.
    /// 
    /// \return
    /// Returns the XML Flags
    /// 
	virtual NAMESPACE_OSGEO_FDO_XML::XmlFlags^ GetDeserializationFlags();

    /// \brief
    /// Gets the current XML Reader. This function would 
    /// typically be called by the XmlSaxHandler callbacks when 
    /// ReadXml() is being invoked. When ReadXml() reads from a file, stream,
    /// or text reader, a wrapping XML reader is automatically created.
    /// 
    /// \return
    /// Returns the XML reader
    /// 
	virtual NAMESPACE_OSGEO_COMMON_XML::XmlReader^ GetXmlReader();

    /// \brief
    /// Gets the stylesheet for converting the XML document from 
    /// external to internal format. When classes derived from XmlDeserializable
    /// define an internal format, they must override this function to return a
    /// stylesheet that does the conversion.
    /// 
    /// \return
    /// Returns nullptr by default (no internal format defined)
    /// 
    /// It has beed implemented in Additional Interfaces.
    // NAMESPACE_OSGEO_COMMON_XML::XmlReader^ GetFromExternalStylesheet();

    /// \brief
    /// Gets the SAX context to pass to the XmlSaxHandler callbacks.
    /// Classes derived from XmlDeserializable can override this function to 
    /// specify a SAX context with class-specific information.
    /// 
    /// \return
    /// Returns nullptr by default, the default NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext is used.
    /// The default provides basic error handling functionality.
    /// 
	virtual NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext^ GetSaxContext();

	/*
		IXmlSerializable Implemenations
	*/

    /// \brief
    /// Writes this object to a file. A complete XML document,
    /// containing this object, is written.
    /// 
    /// \param fileName 
    /// Input the file name.
    /// 
	virtual System::Void WriteXml(String^ fileName);

    /// \brief
    /// Writes this object to a file. A complete XML document,
    /// containing this object, is written.
    /// 
    /// \param fileName 
    /// Input the file name.
    /// \param flags 
    /// Input controls the writing of the elements to the document.
    /// If nullptr then the default flags are used (see XmlFlags::Create())
    /// 
	virtual System::Void WriteXml(String^ fileName, NAMESPACE_OSGEO_FDO_XML::XmlFlags^ flags);

    /// \brief
    /// Writes to an XML writer. This object is appended to the XML document
    /// being written. Unlike the other WriteXml() functions it is not necessarily 
    /// the only object in its document.
    /// 
    /// \param xmlWriter 
    /// Input the XML writer. When this function completes, the
    /// XML writer's current position will be just after this object.
    /// 
	virtual System::Void WriteXml(NAMESPACE_OSGEO_COMMON_XML::XmlWriter^ xmlWriter);

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
    /// If nullptr then the default flags are used (see XmlFlags::Create())
    /// 
	virtual System::Void WriteXml(NAMESPACE_OSGEO_COMMON_XML::XmlWriter^ xmlWriter, NAMESPACE_OSGEO_FDO_XML::XmlFlags^ flags);

    /// \brief
    /// Writes to a text writer. A complete XML document,
    /// containing this object, is written.
    /// 
    /// \param textWriter 
    /// Input the text writer.When this function completes, the
    /// text writer's current position will be just after this object.
    /// 
	virtual System::Void WriteXml(NAMESPACE_OSGEO_COMMON_IO::IoTextWriter^ textWriter);

    /// \brief
    /// Writes to a text writer. A complete XML document,
    /// containing this object, is written.
    /// 
    /// \param textWriter 
    /// Input the text writer.When this function completes, the
    /// text writer's current position will be just after this object.
    /// \param flags 
    /// Input controls the writing of the elements to the writer.
    /// If nullptr then the default flags are used (see XmlFlags::Create())
    /// 
	virtual System::Void WriteXml(NAMESPACE_OSGEO_COMMON_IO::IoTextWriter^ textWriter, NAMESPACE_OSGEO_FDO_XML::XmlFlags^ flags);

    /// \brief
    /// Writes to a stream. A complete XML document,
    /// containing this object, is written.
    /// 
    /// \param stream 
    /// Input the stream. When this function completes, the
    /// stream's current position will be just after this object.
    /// 
	virtual System::Void WriteXml(NAMESPACE_OSGEO_COMMON_IO::IoStream^ stream);

    /// \brief
    /// Writes to a stream. A complete XML document,
    /// containing this object, is written.
    /// 
    /// \param stream 
    /// Input the stream. When this function completes, the
    /// stream's current position will be just after this object.
    /// \param flags 
    /// Input controls the writing of the elements to the stream.
    /// If nullptr then the default flags are used (see XmlFlags::Create())
    /// 
	virtual System::Void WriteXml(NAMESPACE_OSGEO_COMMON_IO::IoStream^ stream, NAMESPACE_OSGEO_FDO_XML::XmlFlags^ flags);
};

END_NAMESPACE_OSGEO_FDO_SCHEMA


