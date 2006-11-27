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

class FdoXmlFeatureWriter;

BEGIN_NAMESPACE_OSGEO_COMMON_XML
public __gc class XmlWriter;
END_NAMESPACE_OSGEO_COMMON_XML

BEGIN_NAMESPACE_OSGEO_FDO_SCHEMA
public __gc class ClassDefinition;
END_NAMESPACE_OSGEO_FDO_SCHEMA

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS
public __gc class PropertyValue;
END_NAMESPACE_OSGEO_FDO_COMMANDS

BEGIN_NAMESPACE_OSGEO_FDO_XML
public __gc class XmlFeaturePropertyWriter;
public __gc class XmlFeatureFlags;

/// \brief
/// FdoXmlFeatureWriter writes features to an XML document. The features are 
/// written in GML format. Each feature is written in 3 steps:
/// <ol>
/// 	<li> call SetClassDefintion() to define the feature's class name,
///     schema name, and list of valid properties.
/// 	<li> call SetProperty() for each feature property value to set.
/// 	<li> call WriteFeature() to write the feature with the current 
///     property values. The feature's element name is derived from the
///     class and schema name.
/// </ol>
/// WriteFeature() ensures that the properties are written in their proper order. This 
/// may have slight performance implications since this writer has to accumulate the 
/// property values before writing them. If performance is a concern then 
/// FdoXmlFeaturePropertyWriter should be used instead.
public __gc class XmlFeatureWriter : public NAMESPACE_OSGEO_RUNTIME::Disposable
{
public:
    /// \brief
    /// Creates a Feature Writer for writing FDO features to XML.
    /// 
    /// \param writer 
    /// Input Feature Property  Writer. Specifies the XML
    /// document that the features will be written to.
    /// \param flags 
    /// Input options for controlling the writing of the features. If NULL then the 
    /// flags passed to the document writer are used.
    /// 
    /// \return
    /// Returns FdoXmlFeatureWriter
    /// 
	XmlFeatureWriter(NAMESPACE_OSGEO_FDO_XML::XmlFeaturePropertyWriter* writer, NAMESPACE_OSGEO_FDO_XML::XmlFeatureFlags* flags);
	XmlFeatureWriter(NAMESPACE_OSGEO_FDO_XML::XmlFeaturePropertyWriter* writer);

    /// \brief
    /// Creates a Feature Writer for writing FDO features to XML.
    /// 
    /// \param writer 
    /// Input XML document writer. Specifies the XML document that the features will be written    
    /// to . An FdoXmlFeaturePropertyWriter is automatically wrapped 
    /// around this writer. This Feature Property Writer can be retrieved by calling 
    /// GetFeaturePropertyWriter().
    /// \param flags 
    /// Input options for controlling the writing of the features. If NULL then the 
    /// flags passed to the document writer are used.
    /// 
    /// \return
    /// Returns FdoXmlFeatureWriter
    /// 
	XmlFeatureWriter(NAMESPACE_OSGEO_COMMON_XML::XmlWriter* writer, NAMESPACE_OSGEO_FDO_XML::XmlFeatureFlags* flags);
	XmlFeatureWriter(NAMESPACE_OSGEO_COMMON_XML::XmlWriter* writer);

    /// \brief
    /// Gets the feature property writer that was passed to this object.
    /// 
    /// \return
    /// Returns FdoXmlFeaturePropertyWriter
    /// 
	__property NAMESPACE_OSGEO_FDO_XML::XmlFeaturePropertyWriter* get_FeaturePropertyWriter();

    /// \brief
    /// Gets the class definition for the current feature being written.
    /// 
    /// \return
    /// Returns FdoClassDefinition
    /// 
	__property NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition* get_ClassDefinition();

    /// \brief
    /// Sets the class definition for the current feature being written.
    /// 
    /// \param value 
    /// Input the class definition
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_ClassDefinition(NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition* classDefinition);

    /// \brief
    /// Sets a feature property.
    /// 
    /// \param propertyValue 
    /// Input the property name and value
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_Property(NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValue* propertyValue);

    /// \brief
    /// Gets a reference to an FdoXmlFeatureWriter to write the data contained
    /// in a collection object property. If the property is not an object property, an 
    /// exception is thrown.
    /// 
    /// \param propertyName 
    /// Input the object property name.
    /// 
    /// \return
    /// Returns the nested XML feature writer
    /// 
	__property NAMESPACE_OSGEO_FDO_XML::XmlFeatureWriter* get_ObjectWriter(System::String* propertyName);

    /// \brief
    /// Gets a reference to an FdoXmlFeatureWriter to write the data contained
    /// in an association property. If the property is not an association property, an 
    /// exception is thrown.
    /// 
    /// \param propertyName 
    /// Input the association property name.
    /// 
    /// \return
    /// Returns the nested XML feature writer
    /// 
	__property NAMESPACE_OSGEO_FDO_XML::XmlFeatureWriter* get_AssociationWriter(System::String* propertyName);
    
    /// \brief
    /// Writes the current feature to the XML document.
    /// If all features being written are of the same class then SetClassDefinition()
    /// can be called once and the WriteFeature() can be call repeatedly. In other words,
    /// the current class definition persists across WriteFeature() calls. Similarly,
    /// Property Values also persist across WriteFeature() calls. If the next feature
    /// to write has a property with different value from current feature, then 
    /// SetProperty() must be called, to change the value, before next call to 
    /// WriteFeature().
    /// 
    /// \param elementTag 
    /// The tag for the output feature/object. If elementTag is null, the class name
    /// will be used as the element tag.
    /// 
	System::Void WriteFeature(System::String* elementTag);
	System::Void WriteFeature();

public private:
	XmlFeatureWriter(System::IntPtr unmanaged, System::Boolean autoDelete);

	inline FdoXmlFeatureWriter* GetImpObj();

protected:
    System::Void ReleaseUnmanagedObject();
};

END_NAMESPACE_OSGEO_FDO_XML


