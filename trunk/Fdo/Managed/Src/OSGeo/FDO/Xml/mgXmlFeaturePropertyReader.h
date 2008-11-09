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

class FdoXmlFeaturePropertyReader;

BEGIN_NAMESPACE_OSGEO_COMMON_XML
public __gc class XmlReader;
END_NAMESPACE_OSGEO_COMMON_XML

BEGIN_NAMESPACE_OSGEO_FDO_SCHEMA
public __gc class FeatureSchemaCollection;
END_NAMESPACE_OSGEO_FDO_SCHEMA

BEGIN_NAMESPACE_OSGEO_FDO_XML
public __gc class XmlFeatureContext;
public __gc class XmlFeatureHandler;
public __gc class XmlFeatureFlags;

/// \ingroup (OSGeoFDOXml)
/// \brief
/// XmlFeaturePropertyReader reads features from an XML document. The reading
/// is event driven. As features and properties within features are encountered, the 
/// invoker is notified through the XmlFeatureHandler interface. This class is 
/// similar to XmlFeatureReader except that it provides better performance.
public __gc class XmlFeaturePropertyReader : public NAMESPACE_OSGEO_COMMON_XML::XmlSaxHandler
{
public:
    /// \brief
    /// creates a Feature Property Reader.
    /// 
    /// \remarks
    /// The XML element, at the document reader's current position, is recognized as a feature 
    /// collection if it is one or more of the following: 
    /// <ul>
    /// 	<li> GML FeatureCollection element
    /// 	<li> the element specified by flags->collectionUri and flags->collectionName
    /// 	<li> it has a corresponding class definition, in the given schemas, that 
    ///   derives from the GML AbstractFeatureCollectionType.
    /// </ul>
    /// 
    /// \param reader 
    /// Input XML document reader.
    /// If this reader is currently positioned at a feature 
    /// collection type element then all features in this element are read. Otherwise, 
    /// it processes all Feature Collection type sub-elements of the current element 
    /// and reads their features. Therefore, if the reader is at the start of the XML 
    /// document, all top-level feature collections in the document are read.
    /// 
	XmlFeaturePropertyReader(NAMESPACE_OSGEO_COMMON_XML::XmlReader* reader);

    /// \brief
    /// creates a Feature Property Reader.
    /// 
    /// \remarks
    /// The XML element, at the document reader's current position, is recognized as a feature 
    /// collection if it is one or more of the following: 
    /// <ul>
    /// 	<li> GML FeatureCollection element
    /// 	<li> the element specified by flags->collectionUri and flags->collectionName
    /// 	<li> it has a corresponding class definition, in the given schemas, that 
    ///   derives from the GML AbstractFeatureCollectionType.
    /// </ul>
    /// 
    /// \param reader 
    /// Input XML document reader.
    /// If this reader is currently positioned at a feature 
    /// collection type element then all features in this element are read. Otherwise, 
    /// it processes all Feature Collection type sub-elements of the current element 
    /// and reads their features. Therefore, if the reader is at the start of the XML 
    /// document, all top-level feature collections in the document are read.
    /// \param flags 
    /// Input options for controlling the deserializing of the features. If NULL then 
    /// the default flags are used.
    /// 
	XmlFeaturePropertyReader(NAMESPACE_OSGEO_COMMON_XML::XmlReader* reader, NAMESPACE_OSGEO_FDO_XML::XmlFeatureFlags* flags);

    /// \brief
    /// Gets the Xml document reader that was passed to this object.
    /// 
    /// \return
    /// Returns XmlReader.
    /// 
	NAMESPACE_OSGEO_COMMON_XML::XmlReader* GetXmlReader();

    /// \brief
    /// Gets the feature schemas describing the features being read.
    /// 
    /// \return
    /// Returns FeatureSchemaCollection
    /// 
	__property NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection* get_FeatureSchemas();

    /// \brief
    /// Sets the feature schemas describing the features being read.
    /// 
    /// \remarks
    /// The feature schemas provide directions to this class on how to deserialize the 
    /// features and convert their property values from the strings in the XML document
    /// to their proper types. If any feature, whose class definition is not present 
    /// in these schemas, is encountered then one of the following is done:
    /// <ul>
    /// 	<li> an exception is thrown when flags->errorLevel is Normal or higher. The 
    ///   exception reports all such features.
    /// 	<li> the feature is read according to a best default translation when the 
    ///   flags->errorLevel is below Normal.
    /// </ul>
    /// 
    /// \param schemas 
    /// Input the feature schemas
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_FeatureSchemas(NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection* schemas);

    /// \brief
    /// Parses the XML document.
    /// 
    /// \return
    /// Returns false if no feature(s) were read.
    /// 
	System::Boolean Parse();

    /// \brief
    /// Parses the XML document.
    /// 
    /// \param featureHandler 
    /// Input handler to receive events. Parse() fires various events as features and
    /// properties are encountered.
    /// 
    /// \return
    /// Returns false if no feature(s) were read.
    /// 
    System::Boolean Parse(NAMESPACE_OSGEO_FDO_XML::XmlFeatureHandler* featureHandler);

    /// \brief
    /// Parses the XML document.
    /// 
    /// \param featureHandler 
    /// Input handler to receive events. Parse() fires various events as features and
    /// properties are encountered.
    /// \param featureContext 
    /// Input Caller-specific contextual information that is pass to the feature 
    /// handler event callbacks.
    /// 
    /// \return
    /// Returns false if no feature(s) were read.
    /// 
    System::Boolean Parse(NAMESPACE_OSGEO_FDO_XML::XmlFeatureHandler* featureHandler, NAMESPACE_OSGEO_FDO_XML::XmlFeatureContext* featureContext);

    /// \brief
    /// Parses the XML document.
    /// 
    /// \param featureHandler 
    /// Input handler to receive events. Parse() fires various events as features and
    /// properties are encountered.
    /// \param featureContext 
    /// Input Caller-specific contextual information that is pass to the feature 
    /// handler event callbacks.
    /// \param incremental 
    /// Input 
    /// true: an incremental (progressive) parse is performed. This function returns after the 
    /// next feature is read. 
    /// false: this function keeps going until all features have been read.
    /// 
    /// \return
    /// Returns false if no feature(s) were read.
    /// 
    System::Boolean Parse(NAMESPACE_OSGEO_FDO_XML::XmlFeatureHandler* featureHandler, NAMESPACE_OSGEO_FDO_XML::XmlFeatureContext* featureContext, System::Boolean incremental);

    /// \brief
    /// Indicates whether all features have been read
    /// 
    /// \return
    /// Returns true if there are no more features to read , false otherwise
    /// 
	System::Boolean GetEOF();

public private:
	XmlFeaturePropertyReader(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_COMMON_XML::XmlSaxHandler(unmanaged, autoDelete)
	{
		
	}

	inline FdoXmlFeaturePropertyReader* GetImpObj();

/// \cond DOXYGEN-IGNORE
protected:
	System::Void ReleaseUnmanagedObject();
/// \endcond
};

END_NAMESPACE_OSGEO_FDO_XML


