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

class FdoXmlFeaturePropertyWriter;

BEGIN_NAMESPACE_OSGEO_COMMON_XML
ref class XmlWriter;
END_NAMESPACE_OSGEO_COMMON_XML

BEGIN_NAMESPACE_OSGEO_COMMON
interface class IStreamReader;
END_NAMESPACE_OSGEO_COMMON

BEGIN_NAMESPACE_OSGEO_FDO_SCHEMA
ref class ClassDefinition;
END_NAMESPACE_OSGEO_FDO_SCHEMA

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS
ref class PropertyValue;
END_NAMESPACE_OSGEO_FDO_COMMANDS

BEGIN_NAMESPACE_OSGEO_FDO_RASTER
interface class IRaster;
END_NAMESPACE_OSGEO_FDO_RASTER

BEGIN_NAMESPACE_OSGEO_FDO_EXPRESSION
ref class LOBValue;
END_NAMESPACE_OSGEO_FDO_EXPRESSION

BEGIN_NAMESPACE_OSGEO_FDO_XML
ref class XmlFlags;

/// \ingroup (OSGeoFDOXml)
/// \brief
/// XmlFeaturePropertyWriter writes features in GML format. It is similar to 
/// XmlFeatureWriter, except that it provides better performance. However, the 
/// caller is responsible for ensuring that properties are written in proper order. 
public ref class XmlFeaturePropertyWriter : public NAMESPACE_OSGEO_RUNTIME::Disposable
{
public:
    /// \brief
    /// Creates a Feature Property Writer for writing FDO features to XML.
    /// 
    /// \param writer 
    /// Input XML document writer. Specifies the XML document that the features will be
    /// written to.
    /// 
	XmlFeaturePropertyWriter(NAMESPACE_OSGEO_COMMON_XML::XmlWriter^ writer);

    /// \brief
    /// Creates a Feature Property Writer for writing FDO features to XML.
    /// 
    /// \param writer 
    /// Input XML document writer. Specifies the XML document that the features will be
    /// written to.
    /// \param flags 
    /// Input options for controlling the writing of the features. If nullptr then the 
    /// default flags are used.
    /// 
	XmlFeaturePropertyWriter(NAMESPACE_OSGEO_COMMON_XML::XmlWriter^ writer, NAMESPACE_OSGEO_FDO_XML::XmlFlags^ flags);

    /// \brief
    /// Gets the underlying XML writer.
    /// 
    /// \return
    /// Returns the underlying XML writer.
    /// 
	NAMESPACE_OSGEO_COMMON_XML::XmlWriter^ GetXmlWriter();

    /// \brief
    /// Gets the class definition for the current feature being written.
    /// 
    /// \return
    /// Returns ClassDefinition
    /// 
    /// \brief
    /// Sets the class definition for the current feature being written.
    /// 
    /// \param classDefinition 
    /// Input the class definition
    /// 
    /// \return
    /// Returns nothing
    /// 
    property NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition^ ClassDefinition
    {
        NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition^ get();
        System::Void set(NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition^ classDefinition);
    }

    /// \brief
    /// Writes the start of a feature to XML. The start tags for the feature member 
    /// element and the feature's actual element are written. The feature element name
    /// is derived from the feature's class and schema names.
    /// 
    /// \param startTag 
    /// The start tag for the feature element. If it is null, the
    /// feature's class name used.
    /// 
    /// \return
    /// Returns nothing
    /// 
	System::Void WriteFeatureStart(System::String^ startTag);

    /// \brief
    /// Closes the current feature by writing the feature element and feature member 
    /// element end tags.
    /// 
    /// \return
    /// Returns nothing
    /// 
	System::Void WriteFeatureEnd();

    /// \brief
    /// Writes a feature property to XML.
    /// 
    /// \param name 
    /// Input the property name
    /// \param value 
    /// Input the property value as an FDO property
    /// 
    /// \return
    /// Returns nothing
    /// 
	System::Void WriteProperty(System::String^ name, NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValue^ value);

    /// \brief
    /// Writes a feature property to XML.
    /// 
    /// \param name 
    /// Input the property name
    /// \param value 
    /// Input the property value as an FDO property
    /// \param valueOnly 
    /// false: wrap the property value in a property element
    /// true: just write the property value
    /// 
    /// \return
    /// Returns nothing
    /// 
	System::Void WriteProperty(System::String^ name, NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValue^ value, System::Boolean valueOnly);

    /// \brief
    /// Writes a feature property to XML.
    /// 
    /// \param name 
    /// Input the property name
    /// \param value 
    /// Input the property value as a string
    /// 
    /// \return
    /// Returns nothing
    /// 
	System::Void WriteProperty(System::String^ name, System::String^ value);

    /// \brief
    /// Writes a feature property to XML.
    /// 
    /// \param name 
    /// Input the property name
    /// \param value 
    /// Input the property value as a string
    /// \param valueOnly 
    /// false: wrap the property value in a property element
    /// true: just write the property value
    /// 
    /// \return
    /// Returns nothing
    /// 
	System::Void WriteProperty(System::String^ name, System::String^ value, System::Boolean valueOnly);

    // TODO: do we need functions for other basic types or make the caller do the
    // conversion to string. 

    /// \brief
    /// Writes a raster property to XML.
    /// 
    /// \param name 
    /// Input the property name
    /// \param value 
    /// Input the raster property value
    /// 
    /// \return
    /// Returns nothing
    /// 
	System::Void WriteProperty(System::String^ name, NAMESPACE_OSGEO_FDO_RASTER::IRaster^ value);

    /// \brief
    /// Writes a raster property to XML.
    /// 
    /// \param name 
    /// Input the property name
    /// \param value 
    /// Input the raster property value
    /// \param valueOnly 
    /// false: wrap the property value in a property element
    /// true: just write the property value
    /// 
    /// \return
    /// Returns nothing
    /// 
	System::Void WriteProperty(System::String^ name, NAMESPACE_OSGEO_FDO_RASTER::IRaster^ value, System::Boolean valueOnly);

    /// \brief
    /// Writes a lob property to XML.
    /// 
    /// \param name 
    /// Input the property name
    /// \param value 
    /// Input the lob property value
    /// 
    /// \return
    /// Returns nothing
    /// 
	System::Void WriteProperty(System::String^ name, NAMESPACE_OSGEO_FDO_EXPRESSION::LOBValue^ value);

    /// \brief
    /// Writes a lob property to XML.
    /// 
    /// \param name 
    /// Input the property name
    /// \param value 
    /// Input the lob property value
    /// \param valueOnly 
    /// false: wrap the property value in a property element
    /// true: just write the property value
    /// 
    /// \return
    /// Returns nothing
    /// 
	System::Void WriteProperty(System::String^ name, NAMESPACE_OSGEO_FDO_EXPRESSION::LOBValue^ value, System::Boolean valueOnly);

    /// \brief
    /// Writes a LOB feature property from a stream reader to XML.
    /// 
    /// \param name 
    /// Input the property name
    /// \param lobReader 
    /// Input the reader for retrieving the lob
    /// 
    /// \return
    /// Returns nothing
    /// 
	System::Void WriteProperty(System::String^ name, NAMESPACE_OSGEO_COMMON::IStreamReader^ lobReader);

    /// \brief
    /// Writes a LOB feature property from a stream reader to XML.
    /// 
    /// \param name 
    /// Input the property name
    /// \param lobReader 
    /// Input the reader for retrieving the lob
    /// \param valueOnly 
    /// false: wrap the property value in a property element
    /// true: just write the property value
    /// 
    /// \return
    /// Returns nothing
    /// 
	System::Void WriteProperty(System::String^ name, NAMESPACE_OSGEO_COMMON::IStreamReader^ lobReader, System::Boolean valueOnly);

    /// \brief
    /// Writes a geometric property to XML.
    /// 
    /// \param name 
    /// Input the property name
    /// \param buffer 
    /// Input the property value as an array of bytes
    /// 
    /// \return
    /// Returns nothing
    /// 
	System::Void WriteGeometricProperty(System::String^ name, array<System::Byte>^ buffer);

    /// \brief
    /// Writes a geometric property to XML.
    /// 
    /// \param name 
    /// Input the property name
    /// \param buffer 
    /// Input the property value as an array of bytes
    /// \param valueOnly 
    /// false: wrap the property value in a property element
    /// true: just write the property value
    /// 
    /// \return
    /// Returns nothing
    /// 
	System::Void WriteGeometricProperty(System::String^ name, array<System::Byte>^ buffer, System::Boolean valueOnly);

    /// \brief
    /// Writes start tag for an object property. The object property sub-properties
    /// can be written by subsequent calls to this class's Write functions.
    /// 
    /// \param name 
    /// Input the object property name
    /// 
    /// \return
    /// Returns nothing
    /// 
	System::Void WriteObjectPropertyStart(System::String^ name);

    /// \brief
    /// Closes the current object property by writing its end tag.
    /// 
    /// \return
    /// Returns nothing
    /// 
	System::Void WriteObjectPropertyEnd();

internal:
	XmlFeaturePropertyWriter(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_RUNTIME::Disposable(unmanaged, autoDelete)
	{
		
	}

	inline FdoXmlFeaturePropertyWriter* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_XML


