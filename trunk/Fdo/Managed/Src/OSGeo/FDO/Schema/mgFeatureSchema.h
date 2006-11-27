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
/// The FdoFeatureSchema class derives from FdoSchemaElement.
/// A feature schema contains all of the classes and relationships
/// that make up a particular data model. The FdoFeatureSchema class can be used to
/// either create a new schema or to browse the schema end of a
/// connection. In the later case, the FdoFeatureSchema instance is created by the
/// DescribeSchema command. In this case the schema objects have additional properties,
/// such as coordinate system definitions that can be useful to the
/// application when placed in context with the schema objects.
public __gc class FeatureSchema : public NAMESPACE_OSGEO_FDO_SCHEMA::SchemaElement, public NAMESPACE_OSGEO_FDO_XML::IXmlSerializable
{
public:
    /// \brief
    /// Constructs a default empty instance of an FdoFeatureSchema.
    /// 
    /// \return
    /// Returns FdoFeatureSchema
    /// 
	FeatureSchema();

    /// \brief
    /// Constructs an instance of an FdoFeatureSchema using the specified arguments.
    /// 
    /// \param name 
    /// Input name
    /// \param description 
    /// Input description
    /// 
    /// \return
    /// Returns FdoFeatureSchema
    /// 
	FeatureSchema(System::String* name, System::String* description);

    /// \brief
    /// Returns an FdoClassCollection that contains all of the classes defined in
    /// the feature schema.
    /// 
    /// \return
    /// Returns an FdoClassCollection
    /// 
	__property NAMESPACE_OSGEO_FDO_SCHEMA::ClassCollection* get_Classes();

    /// \brief
    /// Returns an FdoRelationCollection that contains all of the relations defined
    /// in the feature schema.
    /// 
    /// \return
    /// Returns nothing
    /// 
    // TODO: I can't find the delcaration and definition files of class FdoRelationCollection in
    //       in unmanaged FDO API.
    // FDO_API FdoRelationCollection* GetRelations();

    /// \brief
    /// Accepts all of the changes that have been made to the feature schema.
    /// All child elements with a schema element state set to
    /// FdoSchemaElementState_Deleted are physically removed from the schema. All
    /// other child elements have their schema element state values set to
    /// FdoSchemaElementState_Unchanged.
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
    /// FdoSchemaElementState_Added are physically removed from the schema. All other 
    /// child elements are restored to their original states and have their schema 
    /// element state values set to FdoSchemaElementState_Unchanged.
    /// 
    /// \return
    /// Returns nothing
    /// 
	System::Void RejectChanges();

	FeatureSchema(System::IntPtr unmanaged, System::Boolean autoDelete) :NAMESPACE_OSGEO_FDO_SCHEMA::SchemaElement(unmanaged, autoDelete)
	{
		
	}

	/*
		Implementation of XmlSerializable
	*/

	System::Void WriteXml(String* fileName);

	System::Void WriteXml(String* fileName, NAMESPACE_OSGEO_FDO_XML::XmlFlags* flags);

	System::Void WriteXml(NAMESPACE_OSGEO_COMMON_XML::XmlWriter* xmlWriter);

	System::Void WriteXml(NAMESPACE_OSGEO_COMMON_XML::XmlWriter* xmlWriter, NAMESPACE_OSGEO_FDO_XML::XmlFlags* flags);

	System::Void WriteXml(NAMESPACE_OSGEO_COMMON_IO::IoTextWriter* textWriter);

	System::Void WriteXml(NAMESPACE_OSGEO_COMMON_IO::IoTextWriter* textWriter, NAMESPACE_OSGEO_FDO_XML::XmlFlags* flags);

	System::Void WriteXml(NAMESPACE_OSGEO_COMMON_IO::IoStream* stream);

	System::Void WriteXml(NAMESPACE_OSGEO_COMMON_IO::IoStream* stream, NAMESPACE_OSGEO_FDO_XML::XmlFlags* flags);

	NAMESPACE_OSGEO_COMMON_XML::XmlReader* GetFromInternalStylesheet();

public private:
	inline FdoFeatureSchema* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_SCHEMA


