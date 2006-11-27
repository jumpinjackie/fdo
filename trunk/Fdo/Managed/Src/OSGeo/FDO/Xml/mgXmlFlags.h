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

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA
public __gc class PhysicalSchemaMappingCollection;
END_NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA

BEGIN_NAMESPACE_OSGEO_FDO_XML
public __gc class PhysicalElementMapping;

/// \brief
/// FdoXmlFlags defines various options for serializing and deserializing
/// FDO element to and from XML Documents.
public __gc class XmlFlags : public NAMESPACE_OSGEO_RUNTIME::Disposable
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
	__value enum ErrorLevel 
	{
		ErrorLevel_High = FdoXmlFlags::ErrorLevel_High,
		ErrorLevel_Normal = FdoXmlFlags::ErrorLevel_Normal,
		ErrorLevel_Low = FdoXmlFlags::ErrorLevel_Low,
		ErrorLevel_VeryLow = FdoXmlFlags::ErrorLevel_VeryLow
	};

    /// \brief
    /// Constructs an FdoXmlFlags object.
    /// 
    /// \param url 
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
    /// \return
    /// Returns FdoXmlFlags
    /// 
	XmlFlags();

	XmlFlags(System::String* location);

	XmlFlags(System::String* location, NAMESPACE_OSGEO_FDO_XML::XmlFlags::ErrorLevel errorLevel);

	XmlFlags(System::String* location, NAMESPACE_OSGEO_FDO_XML::XmlFlags::ErrorLevel errorLevel, System::Boolean nameAdjust);

    /// \brief
    /// Sets the target namespace prefix, see FdoXmlFlags::Create().
    /// 
    /// \param location 
    /// Input the target namespace prefix.
    /// 
    __property System::Void set_Url(System::String* location);

    /// \brief
    /// Gets the target namespace prefix, see FdoXmlFlags::Create().
    /// 
    /// \return
    /// Returns the target namespace prefix.
    /// 
	__property System::String* get_Url();

    /// \brief
    /// Sets the error level, see FdoXmlFlags::Create().
    /// 
    /// \param value 
    /// Input the error level.
    /// 
	__property System::Void set_Errorlevel(NAMESPACE_OSGEO_FDO_XML::XmlFlags::ErrorLevel value);

    /// \brief
    /// Gets the current error level, see FdoXmlFlags::Create().
    /// 
    /// \return
    /// Returns the error level.
    /// 
	__property NAMESPACE_OSGEO_FDO_XML::XmlFlags::ErrorLevel get_Errorlevel();

    /// \brief
    /// Sets the name adjustment flag, see FdoXmlFlags::Create().
    /// 
    /// \param nameAdjust 
    /// Input the name adjustment flag.
    /// 
  	__property System::Void set_NameAdjust(System::Boolean nameAdjust);

    /// \brief
    /// Gets the name adjustment flag, see FdoXmlFlags::Create().
    /// 
    /// \return
    /// Returns the name adjustment flag.
    /// 
	__property System::Boolean get_NameAdjust();

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
  	__property System::Void set_SchemaNameAsPrefix(System::Boolean schemaNameAsPrefix);

    /// \brief
    /// Gets the Schema Name as Prefix flag.
    /// 
    /// \return
    /// Returns the Schema Name as Prefix flag.
    /// 
	__property System::Boolean get_SchemaNameAsPrefix();

    /// \brief
    /// Sets the Use GML ID flag.
    /// 
    /// \param schemaNameAsPrefix 
    /// Input when true, the feature id ( fid in GML 2, gml:id in GML 3)
    /// becomes the identity property of all feature classes.
    /// 
  	__property System::Void set_UseGmlId(System::Boolean id);

    /// \brief
    /// Gets the Use GML ID flag.
    /// 
    /// \return
    /// Returns the Use GML ID flag.
    /// 
	__property System::Boolean get_UseGmlId();

    /// \brief
    /// Sets the Schema Mapping Overrides for translating schemas 
    /// between GML and FDO. 
    /// 
    /// \param mappings 
    /// Input the Schema Mapping Overrides. This collection
    /// can contain an FdoXmlSchemaMapping element for each Feature Schema that
    /// may be written or read. When a Feature Schema is read or written to or 
    /// from GML, this collection is checked for an FdoXmlSchemaMapping item named 
    /// the same as the feature schema. If present, the hints in this item 
    /// affect how the feature schema is read or written. The Schema Mapping 
    /// Overrides also control how features are read since they 
    /// can override the default correspondences between feature class names 
    /// and their GML element names.
    /// 
  	__property System::Void set_SchemaMappings(NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMappingCollection* mappings);

    /// \brief
    /// Gets the current Schema Mapping Overrides.
    /// 
    /// \return
    /// Returns FdoXmlSchemaMappingCollection*.
    /// 
    __property NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMappingCollection* get_SchemaMappings();

public private:
	XmlFlags(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_RUNTIME::Disposable(unmanaged, autoDelete)
	{

	}

	inline FdoXmlFlags* GetImpObj();

protected:
    System::Void ReleaseUnmanagedObject();
};

END_NAMESPACE_OSGEO_FDO_XML


