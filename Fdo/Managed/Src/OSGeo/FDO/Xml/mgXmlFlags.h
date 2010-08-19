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

#include <Fdo/Xml/Flags.h>
#include "FDO\Xml\mgXmlGmlVersion.h"

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA
ref class PhysicalSchemaMappingCollection;
END_NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA

BEGIN_NAMESPACE_OSGEO_FDO_XML
ref class PhysicalElementMapping;
enum class GmlVersion;

/// \ingroup (OSGeoFDOXml)
/// \brief
/// XmlFlags defines various options for serializing and deserializing
/// FDO element to and from XML Documents.
public ref class XmlFlags : public NAMESPACE_OSGEO_RUNTIME::Disposable
{
public:
    /// \brief
    /// The XmlFlags::ErrorLevel determines how strict the error reporting 
    /// is when a Feature Schema is read from an XML Document. For each errorLevel a 
    /// certain level of round-trip fidelity is guaranteed if the read succeeds.
    /// 
    /// \param ErrorLevel_High 
    /// the read fails with an exception when the 
    /// Feature Schema cannot be read without alteration. If the Feature Schema 
    /// is read successfully from document A and then written to document B, 
    /// documents A and B are guaranteed to be identical. This errorLevel should be 
    /// used when no alterations to the Feature Schema can be tolerated
    /// \param ErrorLevel_Normal 
    /// fails if the Feature Schema cannot be read 
    /// without changing the domain of its conforming instance documents. If the 
    /// Feature Schema is read successfully from document A and then written to 
    /// document B, any instance document that conforms to the document A schema 
    /// will also conform to the document B schema. Any instance document that does
    /// not conform to A does not conform to B. However, documents A and B will not 
    /// necessarily be identical.
    /// \param ErrorLevel_Low 
    /// similar to ErrorLevel_Normal except that the 
    /// instance document domain is allowed to grow. If the Feature Schema is read 
    /// successfully from document A and then written to document B, any instance 
    /// document that conforms to the document A schema will also conform to the 
    /// document B schema. However a document that does not conform to the A schema 
    /// may or may not conform to the B schema.
    /// \param ErrorLevel_VeryLow 
    /// the read succeeds even if no level of 
    /// round-trip fidelity is maintained. If the Feature Schema is read successfully 
    /// from document A and then written to document B, any instance document that
    /// conforms to the document A schema may or may not conform to the document B 
    /// schema. A document that does not conform to the A schema may or may not 
    /// conform to the B schema. This errorLevel is useful for reading schemas from 
    /// external sources into FDO when it doesn’t matter how much the schemas are 
    /// altered during the read.
    /// 
	enum class ErrorLevel 
	{
		ErrorLevel_High = FdoXmlFlags::ErrorLevel_High,
		ErrorLevel_Normal = FdoXmlFlags::ErrorLevel_Normal,
		ErrorLevel_Low = FdoXmlFlags::ErrorLevel_Low,
		ErrorLevel_VeryLow = FdoXmlFlags::ErrorLevel_VeryLow
	};

    /// \brief
    /// Constructs an XmlFlags object.
    /// 
	XmlFlags();

    /// \brief
    /// Constructs an XmlFlags object.
    /// 
    /// \param location 
    /// Input When writing  Feature Schemas, this specifies the 
    /// prefix for the target namespace for any schemas that are written. The XML 
    /// format for Feature Schemas is OGC GML, so a targetNamespace for the 
    /// xs:schema element is required. This namespace will be http://[url]/[schema_name].
    /// 
	XmlFlags(System::String^ location);

    /// \brief
    /// Constructs an XmlFlags object.
    /// 
    /// \param location 
    /// Input When writing  Feature Schemas, this specifies the 
    /// prefix for the target namespace for any schemas that are written. The XML 
    /// format for Feature Schemas is OGC GML, so a targetNamespace for the 
    /// xs:schema element is required. This namespace will be http://[url]/[schema_name].
    /// \param errorLevel 
    /// Input The error level for reading feature schemas.
    /// 
	XmlFlags(System::String^ location, NAMESPACE_OSGEO_FDO_XML::XmlFlags::ErrorLevel errorLevel);

    /// \brief
    /// Constructs an XmlFlags object.
    /// 
    /// \param location 
    /// Input When writing  Feature Schemas, this specifies the 
    /// prefix for the target namespace for any schemas that are written. The XML 
    /// format for Feature Schemas is OGC GML, so a targetNamespace for the 
    /// xs:schema element is required. This namespace will be http://[url]/[schema_name].
    /// \param errorLevel 
    /// Input The error level for reading feature schemas.
    /// \param nameAdjust 
    /// Input true: apply name adjustment to all elements. 
    /// false: apply name adjustment only to elements with fdo:nameAdjust="true"
    /// 
	XmlFlags(System::String^ location, NAMESPACE_OSGEO_FDO_XML::XmlFlags::ErrorLevel errorLevel, System::Boolean nameAdjust);

    /// \brief
    /// Sets the target namespace prefix, see XmlFlags::Create().
    /// 
    /// \param location 
    /// Input the target namespace prefix.
    /// 

    /// \brief
    /// Gets the target namespace prefix, see XmlFlags::Create().
    /// 
    /// \return
    /// Returns the target namespace prefix.
    /// 
    property System::String^ Url
    {
        System::String^ get();
        System::Void set(System::String^ location);
    }

    /// \brief
    /// Sets the error level, see XmlFlags::Create().
    /// 
    /// \param value 
    /// Input the error level.
    /// 
    /// \brief
    /// Gets the current error level, see XmlFlags::Create().
    /// 
    /// \return
    /// Returns the error level.
    /// 
    property NAMESPACE_OSGEO_FDO_XML::XmlFlags::ErrorLevel Errorlevel
    {
        NAMESPACE_OSGEO_FDO_XML::XmlFlags::ErrorLevel get();
        System::Void set(NAMESPACE_OSGEO_FDO_XML::XmlFlags::ErrorLevel value);
    }

    /// \brief
    /// Sets the name adjustment flag, see XmlFlags::Create().
    /// 
    /// \param nameAdjust 
    /// Input the name adjustment flag.
    /// 
    /// \brief
    /// Gets the name adjustment flag, see XmlFlags::Create().
    /// 
    /// \return
    /// Returns the name adjustment flag.
    /// 
    property System::Boolean NameAdjust
    {
        System::Boolean get();
        System::Void set(System::Boolean nameAdjust);
    }

    /// \brief
    /// Sets the Schema Name as Prefix flag. This flag controls how
    /// an FDO Feature Schema name is generated when the schema is read
    /// from GML.
    /// 
    /// \param schemaNameAsPrefix 
    /// Input when true, the Feature Schema 
    /// name is set to the prefix from the xmlns namespace declaration for 
    /// the schema's targetNamespace. The target namespace prefix flag on this 
    /// object is ignored
    /// in this case. If no suitable namespace declaration can be found,
    /// the Feature Schema name is set as if this flag were false.
    /// Schemas cannot be round-tripped when this flag is true. Therefore, it
    /// may only be set to true when the current ErrorLevel is VeryLow.
    /// Caution must be observed when using this flag. Unpredictable results
    /// can occur if a GML Schema uses different prefixes for its 
    /// targetNamespace in different parts of the schema.
    /// 
    /// \brief
    /// Gets the Schema Name as Prefix flag.
    /// 
    /// \return
    /// Returns the Schema Name as Prefix flag.
    /// 
    property System::Boolean SchemaNameAsPrefix
    {
        System::Boolean get();
        System::Void set(System::Boolean schemaNameAsPrefix);
    }

    /// \brief
    /// Sets the Use GML ID flag.
    /// 
    /// \param id 
    /// Input when true, the feature id ( fid in GML 2, gml:id in GML 3)
    /// becomes the identity property of all feature classes.
    /// 
    /// \brief
    /// Gets the Use GML ID flag.
    /// 
    /// \return
    /// Returns the Use GML ID flag.
    /// 
    property System::Boolean UseGmlId
    {
        System::Boolean get();
        System::Void set(System::Boolean id);
    }

    /// \brief
    /// Sets the Use GML version.
    /// 
    /// \param gmlVersion 
    /// input the GML version
    /// 
    /// 
    /// \brief
	/// Gets the GML version (GML 2, GML 3).
    /// 
    /// \return
    /// Returns the Used GML version.
    ///  
	property NAMESPACE_OSGEO_FDO_XML::GmlVersion GmlVersion
	{
		NAMESPACE_OSGEO_FDO_XML::GmlVersion get();
		System::Void set(NAMESPACE_OSGEO_FDO_XML::GmlVersion value);
	}

    /// \brief
    /// Sets the Schema Mapping Overrides for translating schemas 
    /// between GML and FDO. 
    /// 
    /// \param mappings 
    /// Input the Schema Mapping Overrides. This collection
    /// can contain an XmlSchemaMapping element for each Feature Schema that
    /// may be written or read. When a Feature Schema is read or written to or 
    /// from GML, this collection is checked for an XmlSchemaMapping item named 
    /// the same as the feature schema. If present, the hints in this item 
    /// affect how the feature schema is read or written. The Schema Mapping 
    /// Overrides also control how features are read since they 
    /// can override the default correspondences between feature class names 
    /// and their GML element names.
    /// 
    /// \brief
    /// Gets the current Schema Mapping Overrides.
    /// 
    /// \return
    /// Returns XmlSchemaMappingCollection^.
    /// 
    property NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMappingCollection^ SchemaMappings
    {
        NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMappingCollection^ get();
        System::Void set(NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMappingCollection^ value);
    }

internal:
	XmlFlags(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_RUNTIME::Disposable(unmanaged, autoDelete)
	{

	}

	inline FdoXmlFlags* GetImpObj();
public:
    virtual IntPtr GetDisposableObject() override;
};

END_NAMESPACE_OSGEO_FDO_XML


