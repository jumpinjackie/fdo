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

#include "FDO\Schema\mgSchemaElement.h"
#include "FDO\Xml\mgIXmlSerializable.h"

class FdoFeatureSchema;

BEGIN_NAMESPACE_OSGEO_FDO_XML
public __gc class XmlFlags;
END_NAMESPACE_OSGEO_FDO_XML

BEGIN_NAMESPACE_OSGEO_FDO_SCHEMA
public __gc class ClassCollection;

/// \brief
/// The FeatureSchema class derives from SchemaElement.
/// A feature schema contains all of the classes and relationships
/// that make up a particular data model. The FeatureSchema class can be used to
/// either create a new schema or to browse the schema end of a
/// connection. In the later case, the FeatureSchema instance is created by the
/// DescribeSchema command. In this case the schema objects have additional properties,
/// such as coordinate system definitions that can be useful to the
/// application when placed in context with the schema objects.
public __gc class FeatureSchema : public NAMESPACE_OSGEO_FDO_SCHEMA::SchemaElement, public NAMESPACE_OSGEO_FDO_XML::IXmlSerializable
{
public:
    /// \brief
    /// Constructs a default empty instance of an FeatureSchema.
    /// 
    /// \return
    /// Returns FeatureSchema
    /// 
	FeatureSchema();

    /// \brief
    /// Constructs an instance of a FeatureSchema using the specified arguments.
    /// 
    /// \param name 
    /// Input name
    /// \param description 
    /// Input description
    /// 
    /// \return
    /// Returns FeatureSchema
    /// 
	FeatureSchema(System::String* name, System::String* description);

    /// \brief
    /// Returns an ClassCollection that contains all of the classes defined in
    /// the feature schema.
    /// 
    /// \return
    /// Returns an ClassCollection
    /// 
	__property NAMESPACE_OSGEO_FDO_SCHEMA::ClassCollection* get_Classes();

    /// \brief
    /// Accepts all of the changes that have been made to the feature schema.
    /// All child elements with a schema element state set to
    /// SchemaElementState_Deleted are physically removed from the schema. All
    /// other child elements have their schema element state values set to
    /// SchemaElementState_Unchanged.
    /// This method would usually be only called from an FDO Provider (IApplySchema
    /// implementation). It would not normally called by an FDO Client.
    /// 
    /// \return
    /// Returns nothing
    /// 
	System::Void AcceptChanges();

    /// \brief
    /// Rejects all of the changes that have been made to the feature schema.
    /// All child elements with a schema element state set to
    /// SchemaElementState_Added are physically removed from the schema. All other 
    /// child elements are restored to their original states and have their schema 
    /// element state values set to FdoSchemaElementState_Unchanged.
    /// 
    /// \return
    /// Returns nothing
    /// 
	System::Void RejectChanges();

    /// \brief
    /// Constructs a FeatureSchema object based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    /// Input A Pointer to the unmanaged object.
    /// 
    /// \param autoDelete 
    /// Input Indicates if the constructed object should be automatically deleted 
    /// once it no longer referenced.
    /// 
	FeatureSchema(System::IntPtr unmanaged, System::Boolean autoDelete) :NAMESPACE_OSGEO_FDO_SCHEMA::SchemaElement(unmanaged, autoDelete)
	{
		
	}

	/*
		Implementation of XmlSerializable
	*/

    /// \brief
    /// Writes this object to a file. A complete XML document,
    /// containing this object, is written.
    /// 
    /// \param fileName 
    /// Input the file name.
    /// 
	System::Void WriteXml(String* fileName);

    /// \brief
    /// Writes this object to a file. A complete XML document,
    /// containing this object, is written.
    /// 
    /// \param fileName 
    /// Input the file name.
    /// \param flags 
    /// Input controls the writing of the elements to the document.
    /// If NULL then the default flags are used (see XmlFlags::Create())
    /// 
	System::Void WriteXml(String* fileName, NAMESPACE_OSGEO_FDO_XML::XmlFlags* flags);

    /// \brief
    /// Writes to an XML writer. This object is appended to the XML document
    /// being written. Unlike the other WriteXml() functions it is not necessarily 
    /// the only object in its document.
    /// 
    /// \param xmlWriter 
    /// Input the XML writer. When this function completes, the
    /// XML writer's current position will be just after this object.
    /// 
	System::Void WriteXml(NAMESPACE_OSGEO_COMMON_XML::XmlWriter* xmlWriter);

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
    /// If NULL then the default flags are used (see XmlFlags::Create())
    /// 
	System::Void WriteXml(NAMESPACE_OSGEO_COMMON_XML::XmlWriter* xmlWriter, NAMESPACE_OSGEO_FDO_XML::XmlFlags* flags);

    /// \brief
    /// Writes to a text writer. A complete XML document,
    /// containing this object, is written.
    /// 
    /// \param textWriter 
    /// Input the text writer.When this function completes, the
    /// text writer's current position will be just after this object.
    /// 
	System::Void WriteXml(NAMESPACE_OSGEO_COMMON_IO::IoTextWriter* textWriter);

    /// \brief
    /// Writes to a text writer. A complete XML document,
    /// containing this object, is written.
    /// 
    /// \param textWriter 
    /// Input the text writer.When this function completes, the
    /// text writer's current position will be just after this object.
    /// \param flags 
    /// Input controls the writing of the elements to the writer.
    /// If NULL then the default flags are used (see XmlFlags::Create())
    /// 
	System::Void WriteXml(NAMESPACE_OSGEO_COMMON_IO::IoTextWriter* textWriter, NAMESPACE_OSGEO_FDO_XML::XmlFlags* flags);

    /// \brief
    /// Writes to a stream. A complete XML document,
    /// containing this object, is written.
    /// 
    /// \param stream 
    /// Input the stream. When this function completes, the
    /// stream's current position will be just after this object.
    /// 
	System::Void WriteXml(NAMESPACE_OSGEO_COMMON_IO::IoStream* stream);

    /// \brief
    /// Writes to a stream. A complete XML document,
    /// containing this object, is written.
    /// 
    /// \param stream 
    /// Input the stream. When this function completes, the
    /// stream's current position will be just after this object.
    /// \param flags 
    /// Input controls the writing of the elements to the stream.
    /// If NULL then the default flags are used (see XmlFlags::Create())
    /// 
	System::Void WriteXml(NAMESPACE_OSGEO_COMMON_IO::IoStream* stream, NAMESPACE_OSGEO_FDO_XML::XmlFlags* flags);

    /// \brief
    /// Gets the stylesheet for converting the XML document from 
    /// internal to external format. When classes derived from XmlSerializable
    /// define an internal format, they must override this function to return a
    /// stylesheet that does the conversion.
    /// 
    /// \return
    /// Returns NULL by default (no internal format defined)
    /// 
	NAMESPACE_OSGEO_COMMON_XML::XmlReader* GetFromInternalStylesheet();

/// \cond DOXYGEN-IGNORE
public private:
	inline FdoFeatureSchema* GetImpObj();
/// \endcond
};

END_NAMESPACE_OSGEO_FDO_SCHEMA


