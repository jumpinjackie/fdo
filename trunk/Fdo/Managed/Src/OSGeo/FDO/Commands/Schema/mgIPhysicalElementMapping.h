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

BEGIN_NAMESPACE_OSGEO_COMMON
ref class StringCollection;
END_NAMESPACE_OSGEO_COMMON

BEGIN_NAMESPACE_OSGEO_COMMON_XML
ref class XmlSaxContext;
ref class XmlWriter;
ref class XmlAttributeCollection;
END_NAMESPACE_OSGEO_COMMON_XML

BEGIN_NAMESPACE_OSGEO_FDO_XML
ref class XmlFlags;
END_NAMESPACE_OSGEO_FDO_XML

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA
interface class IPhysicalElementMapping;
ref class PhysicalSchemaMapping;

/// \ingroup (OSGeoFDOCommandsSchema)
/// \interface OSGeo::FDO::Commands::Schema::IPhysicalElementMapping
/// \brief
/// IPhysicalElementMapping is an abstract interface that acts as a base interface for all 
/// Physical Schema Mapping Override classes. FDO also provides a number of sub-classes
/// for particular types of Provider-specific override classes:
///
/// \li \c PhysicalClassMapping: All classes corresponding to Feature Classes
/// must be based on PhysicalClassMapping.
/// \li \c PhysicalPropertyMapping: Must be the base for all classes corresponding to 
/// Properties.
/// \li \c PhysicalSchemaMapping: Must be the base for all types of schema override sets.
/// \li \c Other types of schema override classes can be based directly on 
/// PhysicalElementMapping.
/// 
public interface class IPhysicalElementMapping : public NAMESPACE_OSGEO_COMMON_XML::IXmlSaxHandler
{
public:
    /// \brief
    /// Gets the parent of this PhysicalElementMapping or null if this object has not
    /// been added to a parent object.
    /// 
    /// \return
    /// Returns the PhysicalElementMapping of the parent
    /// 
    property NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::IPhysicalElementMapping^ Parent
    {
        NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::IPhysicalElementMapping^ get();
    }

    /// \brief
    /// Gets the PhysicalSchemaMapping that this element is a part of. Returns null if this
    /// object has not been added to a feature schema.
    /// 
    /// \return
    /// Returns PhysicalSchemaMapping this object is a part of
    /// 
    property NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping^ SchemaMapping
    {
        NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMapping^ get();
    }

    /// \brief
    /// Gets the name of this element.
    /// 
    /// \return
    /// Returns the name of this element.
    /// 
    /// \brief
    /// Gets the fully qualified name of this element
    /// 
    /// \return
    /// default implementation simply returns the element name
    /// 
    property System::String^ Name
    {
        System::String^ get();
        System::Void set(System::String^ value);
    }

    /// \brief
    /// Gets the fully qualified name of this element.
    /// 
    /// \return
    /// Returns the qualified name of this element. 
    /// 
    property System::String^ QualifiedName
    {
        System::String^ get();
    }

    /// \brief
    /// Indicates whether the name of the Element can be changed once
    /// it has been created.
    /// 
    /// \return
    /// Returns true if the Element name can be changed; otherwise false.
    /// 
    property System::Boolean CanSetName
    {
        System::Boolean get();
    }

    /// \brief
    /// Initializes this Physical Element Mapping from its XML attributes. Called when
    /// the element is deserialized from XML. Can be extended to handle 
    /// particular XML attributes for derived classes.
    /// 
    /// \param context 
    /// Input context contain information about the current deserialization operation
    /// \param attributes 
    /// Input the XML attributes.
    /// 
    /// \return
    /// Returns nothing
    /// 
	System::Void InitFromXml(NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext^ context, NAMESPACE_OSGEO_COMMON_XML::XmlAttributeCollection^ attributes);

    /// \brief
    /// Writes this Physical Mapping Element to XML. Called when
    /// the element is serialized to XML. Can be extended to handle 
    /// particular XML attributes and sub-elements for derived classes.
    /// 
    /// \param xmlWriter 
    /// Input write the element to this XML writer
    /// \param flags 
    /// Input flags that control the writing of the element.
    /// 
    /// \return
    /// Returns nothing
    /// 
	System::Void WriteXml(NAMESPACE_OSGEO_COMMON_XML::XmlWriter^ xmlWriter, NAMESPACE_OSGEO_FDO_XML::XmlFlags^ flags);

    /// \brief
    /// Error reporting function that are not yet part of the supported FDO API.
    /// Return a XmlSkipElementHandler, which can be used to skip over the element in error.
    ///
    /// \param context 
    /// Input context contain information about the current deserialization operation
    ///
    /// \param parentElement 
    /// Input the XML parent element
    ///
    /// \param subElement 
    /// Input the XML sub element
    ///
	NAMESPACE_OSGEO_COMMON_XML::IXmlSaxHandler^ SubElementError(NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext^ context, System::String^ parentElement, System::String^ subElement);

    /// \brief
    /// Error reporting function that are not yet part of the supported FDO API.
    /// Return a XmlSkipElementHandler, which can be used to skip over the element in error.
    ///
    /// \param context 
    /// Input context contain information about the current deserialization operation
    ///
    /// \param parentElement 
    /// Input the XML parent element
    ///
    /// \param subElement 
    /// Input the XML sub element
    ///
	NAMESPACE_OSGEO_COMMON_XML::IXmlSaxHandler^ MultiSubElementError(NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext^ context, System::String^ parentElement, System::String^ subElement);

    /// \brief
    /// Error reporting function that are not yet part of the supported FDO API.
    /// Return a XmlSkipElementHandler, which can be used to skip over the element in error.
    ///
    /// \param context 
    /// Input context contain information about the current deserialization operation
    ///
    /// \param parentElement 
    /// Input the XML parent element
    ///
    /// \param subElements 
    /// Input the list of the XML sub elements
    ///
	NAMESPACE_OSGEO_COMMON_XML::IXmlSaxHandler^ ChoiceSubElementError(NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext^ context, System::String^ parentElement, NAMESPACE_OSGEO_COMMON::StringCollection^ subElements);

    /// \brief
    /// Error reporting function that are not yet part of the supported FDO API.
    /// Return a XmlSkipElementHandler, which can be used to skip over the element in error.
    ///
    /// \param context 
    /// Input context contain information about the current deserialization operation
    ///
    /// \param parentElement 
    /// Input the XML parent element
    ///
    /// \param subElement 
    /// Input the XML sub element
    ///
    /// \param subElementName 
    /// Input the XML sub element name
    ///
	NAMESPACE_OSGEO_COMMON_XML::IXmlSaxHandler^ DuplicateSubElementError(NAMESPACE_OSGEO_COMMON_XML::XmlSaxContext^ context, System::String^ parentElement, System::String^ subElement, System::String^ subElementName);
};

END_NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA


