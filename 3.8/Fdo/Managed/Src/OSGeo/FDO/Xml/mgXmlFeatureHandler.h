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

class FdoXmlFeatureHandler;

BEGIN_NAMESPACE_OSGEO_FDO_RASTER
interface class IRaster;
END_NAMESPACE_OSGEO_FDO_RASTER

BEGIN_NAMESPACE_OSGEO_FDO_SCHEMA
ref class ClassDefinition;
END_NAMESPACE_OSGEO_FDO_SCHEMA

BEGIN_NAMESPACE_OSGEO_FDO_XML
ref class XmlFeatureContext;

/// \ingroup (OSGeoFDOXml)
/// \brief
/// XmlFeatureHandler defines the callback interface for reading features from XML.
/// It is not an interface in the strict sense, since each function has a default 
/// implementation that does nothing. Implementors can override these callbacks to 
/// customize the handling of feature fragments parsed by XmlFeaturePropertyReader.
/// Note that the default FeatureProperty implementations log errors in certain circumstances.
/// Therefore, these functions must be overridden to avoid these errors and indicate that 
/// these types of properties are being handled by the caller.
public ref class XmlFeatureHandler : public NAMESPACE_OSGEO_RUNTIME::Disposable
{
public:
    /// \brief
    /// Default feature handler callback that is called when the 
    /// XmlFeaturePropertyReader starts reading an XML document. Does nothing.
    /// 
    /// \param context 
    /// Input caller specified contextual information
    /// 
    /// \return
    /// Returns the feature Handler for all sub-elements of the element that was current
    /// when the read started. If nullptr, this feature handler remains in effect.
    /// 
	NAMESPACE_OSGEO_FDO_XML::XmlFeatureHandler^ FeatureDocumentStart(NAMESPACE_OSGEO_FDO_XML::XmlFeatureContext^ context);

    /// \brief
    /// Default feature handler callback that is called when the XmlFeaturePropertyReader 
    /// finishes reading an XML document. Does nothing
    /// 
    /// \param context 
    /// Input caller specified contextual information
    /// 
	System::Void FeatureDocumentEnd(NAMESPACE_OSGEO_FDO_XML::XmlFeatureContext^ context);

    /// \brief
    /// Default feature handler callback that is called when the 
    /// XmlFeaturePropertyReader encounters the start of a feature collection. 
    /// Does nothing.
    /// 
    /// \param context 
    /// Input caller specified contextual information
    /// \param definition 
    /// Input definition containing feature class name, schema name, valid property list,
    /// etc.
    /// 
    /// \return
    /// Returns the feature Handler for the features in this collection.
    /// If nullptr, this feature handler remains in effect.
    /// 
	NAMESPACE_OSGEO_FDO_XML::XmlFeatureHandler^ FeatureCollectionStart(NAMESPACE_OSGEO_FDO_XML::XmlFeatureContext^ context, NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition^ definition);

    /// \brief
    /// Default feature handler callback that is called when the 
    /// XmlFeaturePropertyReader encounters the end of a feature collection. 
    /// Does nothing
    /// 
    /// \param context 
    /// Input caller specified contextual information
    /// 
    /// \return
    /// Returning true causes the current parse to stop. Returning false causes the parse
    /// to continue. Return value is ignored if the current parse is not an incremental 
    /// parse ( see XmlFeaturePropertyReader::Parse())
    /// 
	System::Boolean FeatureCollectionEnd(NAMESPACE_OSGEO_FDO_XML::XmlFeatureContext^ context);

    /// \brief
    /// Default feature handler callback that is called when the 
    /// XmlFeaturePropertyReader encounters the start of a feature. Does nothing.
    /// 
    /// \param context 
    /// Input caller specified contextual information
    /// \param definition 
    /// Input definition containing feature class name, schema name, valid property list,
    /// etc.
    /// 
    /// \return
    /// Returns the feature Handler for the feature's properties. If nullptr, this feature 
    /// handler remains in effect.
    /// 
	NAMESPACE_OSGEO_FDO_XML::XmlFeatureHandler^ FeatureStart(NAMESPACE_OSGEO_FDO_XML::XmlFeatureContext^ context, NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition^ definition);

    /// \brief
    /// Default feature handler callback that is called when the 
    /// XmlFeaturePropertyReader encounters the end of a feature. Does nothing.
    /// 
    /// \param context 
    /// Input caller specified contextual information
    /// 
    /// \return
    /// Returning true causes the current parse to stop. Returning false causes the 
    /// parse to continue. Return value is ignored if the current parse is not an 
    /// incremental parse ( see XmlFeaturePropertyReader::Parse())
    /// 
	System::Boolean FeatureEnd(NAMESPACE_OSGEO_FDO_XML::XmlFeatureContext^ context);

    /// \brief
    /// Default feature handler callback that is called when the 
    /// XmlFeaturePropertyReader encounters a property with boolean value. 
    /// Logs an error when the current ErrorLevel is High or Normal. An error is 
    /// also logged when the ErrorLevel is Low and the current property is not nullable.
    /// 
    /// \param context 
    /// Input caller specified contextual information
    /// \param name 
    /// Input property name
    /// \param value 
    /// Input property value
    /// 
    /// \return
    /// Returning true causes the current parse to stop. Returning false causes
    /// the parse to continue. Return value is ignored if the current parse is not an
    /// incremental parse ( see XmlFeaturePropertyReader::Parse())
    /// 
	System::Boolean FeatureProperty(NAMESPACE_OSGEO_FDO_XML::XmlFeatureContext^ context, System::String^ name, System::Boolean value);

    /// \brief
    /// Default feature handler callback that is called when the 
    /// XmlFeaturePropertyReader encounters a property with single byte value. 
    /// Logs an error when the current ErrorLevel is High or Normal. An error is 
    /// also logged when the ErrorLevel is Low and the current property is not nullable.
    /// 
    /// \param context 
    /// Input caller specified contextual information
    /// \param name 
    /// Input property name
    /// \param value 
    /// Input property value
    /// 
    /// \return
    /// Returning true causes the current parse to stop. Returning false causes
    /// the parse to continue. Return value is ignored if the current parse is not an
    /// incremental parse ( see XmlFeaturePropertyReader::Parse())
    /// 
	System::Boolean FeatureProperty(NAMESPACE_OSGEO_FDO_XML::XmlFeatureContext^ context, System::String^ name, System::Byte value);

    /// \brief
    /// Default feature handler callback that is called when the 
    /// XmlFeaturePropertyReader encounters a property with DataTime value. 
    /// Logs an error when the current ErrorLevel is High or Normal. An error is 
    /// also logged when the ErrorLevel is Low and the current property is not nullable.
    /// 
    /// \param context 
    /// Input caller specified contextual information
    /// \param name 
    /// Input property name
    /// \param value 
    /// Input property value
    /// 
    /// \return
    /// Returning true causes the current parse to stop. Returning false causes
    /// the parse to continue. Return value is ignored if the current parse is not an
    /// incremental parse ( see XmlFeaturePropertyReader::Parse())
    /// 
	System::Boolean FeatureProperty(NAMESPACE_OSGEO_FDO_XML::XmlFeatureContext^ context, System::String^ name, System::DateTime value);

    /// \brief
    /// Default feature handler callback that is called when the 
    /// XmlFeaturePropertyReader encounters a property with double value. 
    /// Logs an error when the current ErrorLevel is High or Normal. An error is 
    /// also logged when the ErrorLevel is Low and the current property is not nullable.
    /// 
    /// \param context 
    /// Input caller specified contextual information
    /// \param name 
    /// Input property name
    /// \param value 
    /// Input property value
    /// 
    /// \return
    /// Returning true causes the current parse to stop. Returning false causes
    /// the parse to continue. Return value is ignored if the current parse is not an
    /// incremental parse ( see XmlFeaturePropertyReader::Parse())
    /// 
	System::Boolean FeatureProperty(NAMESPACE_OSGEO_FDO_XML::XmlFeatureContext^ context, System::String^ name, System::Double value);

    /// \brief
    /// Default feature handler callback that is called when the 
    /// XmlFeaturePropertyReader encounters a property with int16 value. 
    /// Logs an error when the current ErrorLevel is High or Normal. An error is 
    /// also logged when the ErrorLevel is Low and the current property is not nullable.
    /// 
    /// \param context 
    /// Input caller specified contextual information
    /// \param name 
    /// Input property name
    /// \param value 
    /// Input property value
    /// 
    /// \return
    /// Returning true causes the current parse to stop. Returning false causes
    /// the parse to continue. Return value is ignored if the current parse is not an
    /// incremental parse ( see XmlFeaturePropertyReader::Parse())
    /// 
	System::Boolean FeatureProperty(NAMESPACE_OSGEO_FDO_XML::XmlFeatureContext^ context, System::String^ name, System::Int16 value);

    /// \brief
    /// Default feature handler callback that is called when the 
    /// XmlFeaturePropertyReader encounters a property with int32 value. 
    /// Logs an error when the current ErrorLevel is High or Normal. An error is 
    /// also logged when the ErrorLevel is Low and the current property is not nullable.
    /// 
    /// \param context 
    /// Input caller specified contextual information
    /// \param name 
    /// Input property name
    /// \param value 
    /// Input property value
    /// 
    /// \return
    /// Returning true causes the current parse to stop. Returning false causes
    /// the parse to continue. Return value is ignored if the current parse is not an
    /// incremental parse ( see XmlFeaturePropertyReader::Parse())
    /// 
	System::Boolean FeatureProperty(NAMESPACE_OSGEO_FDO_XML::XmlFeatureContext^ context, System::String^ name, System::Int32 value);

    /// \brief
    /// Default feature handler callback that is called when the 
    /// XmlFeaturePropertyReader encounters a property with int64 value. 
    /// Logs an error when the current ErrorLevel is High or Normal. An error is 
    /// also logged when the ErrorLevel is Low and the current property is not nullable.
    /// 
    /// \param context 
    /// Input caller specified contextual information
    /// \param name 
    /// Input property name
    /// \param value 
    /// Input property value
    /// 
    /// \return
    /// Returning true causes the current parse to stop. Returning false causes
    /// the parse to continue. Return value is ignored if the current parse is not an
    /// incremental parse ( see XmlFeaturePropertyReader::Parse())
    /// 
	System::Boolean FeatureProperty(NAMESPACE_OSGEO_FDO_XML::XmlFeatureContext^ context, System::String^ name, System::Int64 value);

    /// \brief
    /// Default feature handler callback that is called when the 
    /// XmlFeaturePropertyReader encounters a property with single precision value. 
    /// Logs an error when the current ErrorLevel is High or Normal. An error is 
    /// also logged when the ErrorLevel is Low and the current property is not nullable.
    /// 
    /// \param context 
    /// Input caller specified contextual information
    /// \param name 
    /// Input property name
    /// \param value 
    /// Input property value
    /// 
    /// \return
    /// Returning true causes the current parse to stop. Returning false causes
    /// the parse to continue. Return value is ignored if the current parse is not an
    /// incremental parse ( see XmlFeaturePropertyReader::Parse())
    /// 
	System::Boolean FeatureProperty(NAMESPACE_OSGEO_FDO_XML::XmlFeatureContext^ context, System::String^ name, System::Single value);

    /// \brief
    /// Default feature handler callback that is called when the 
    /// XmlFeaturePropertyReader encounters a property with string value. 
    /// Logs an error when the current ErrorLevel is High or Normal. An error is 
    /// also logged when the ErrorLevel is Low and the current property is not nullable.
    /// 
    /// \param context 
    /// Input caller specified contextual information
    /// \param name 
    /// Input property name
    /// \param value 
    /// Input property value
    /// 
    /// \return
    /// Returning true causes the current parse to stop. Returning false causes
    /// the parse to continue. Return value is ignored if the current parse is not an
    /// incremental parse ( see XmlFeaturePropertyReader::Parse())
    /// 
	System::Boolean FeatureProperty(NAMESPACE_OSGEO_FDO_XML::XmlFeatureContext^ context, System::String^ name, System::String^ value);

    /// \brief
    /// Default feature handler callback that is called when the 
    /// XmlFeaturePropertyReader encounters the start of a raster property.
    /// Logs an error when the current ErrorLevel is High or Normal. An error is 
    /// also logged when the ErrorLevel is Low and the current property is not nullable.
    /// The raster image is sent by subsequent calls to FeatureBinaryData().
    /// 
    /// \param context 
    /// Input caller specified contextual information
    /// \param name 
    /// Input property name
    /// \param value 
    /// Input raster image properties. This object always has a nullptr stream reader. The image is
    /// handled by the FeatureBinaryData() callback.
    /// 
    /// \return
    /// Returning true causes the current parse to stop. Returning false causes the 
    /// parse to continue. Return value is ignored if the current parse is not an 
    /// incremental parse ( see XmlFeaturePropertyReader::Parse())
    /// 
	System::Boolean FeatureStartRasterProperty(NAMESPACE_OSGEO_FDO_XML::XmlFeatureContext^ context, System::String^ name, NAMESPACE_OSGEO_FDO_RASTER::IRaster^ value);

    /// \brief
    /// Default feature handler callback that is called when the 
    /// XmlFeaturePropertyReader encounters the end of a raster property.
    /// Does nothing
    /// 
    /// \param context 
    /// Input caller specified contextual information
    /// 
    /// \return
    /// Returning true causes the current parse to stop. Returning false causes the parse
    /// to continue. Return value is ignored if the current parse is not an incremental 
    /// parse ( see XmlFeaturePropertyReader::Parse())
    /// 
	System::Boolean FeatureEndRasterProperty(NAMESPACE_OSGEO_FDO_XML::XmlFeatureContext^ context);

    /// \brief
    /// Default feature handler callback that is called when the 
    /// XmlFeaturePropertyReader encounters  the start of a lob property. 
    /// Logs an error when the current ErrorLevel is High or Normal. 
    /// If this is a BLOB property then its value is sent through the FeatureBinaryData() callback.
    /// If this is a CLOB property then its value is sent through the FeatureCharacters() callback.
    /// 
    /// \param context 
    /// Input caller specified contextual information
    /// \param name 
    /// Input property name
    /// 
    /// \return
    /// Returning true causes the current parse to stop. Returning false cause the parse
    /// to continue. Return value is ignored if the current parse is not an incremental 
    /// parse ( see XmlFeaturePropertyReader::Parse())
    /// 
	System::Boolean FeatureStartLobProperty(NAMESPACE_OSGEO_FDO_XML::XmlFeatureContext^ context, System::String^ name);

    /// \brief
    /// Default feature handler callback that is called when the 
    /// XmlFeaturePropertyReader encounters the end of a lob property.
    /// Does nothing
    /// 
    /// \param context 
    /// Input caller specified contextual information
    /// 
    /// \return
    /// Returning true causes the current parse to stop. Returning false causes the parse
    /// to continue. Return value is ignored if the current parse is not an incremental 
    /// parse ( see XmlFeaturePropertyReader::Parse())
    /// 
	System::Boolean FeatureEndLobProperty(NAMESPACE_OSGEO_FDO_XML::XmlFeatureContext^ context);

    /// \brief
    /// Default feature handler callback that is called when the 
    /// XmlFeaturePropertyReader encounters a geometric property.
    /// Logs an error when the current ErrorLevel is High or Normal. 
    /// 
    /// \param context 
    /// Input caller specified contextual information
    /// \param name 
    /// Input property name
    /// \param buffer 
    /// Input pointer to the byte array defining the geometry value
    /// 
    /// \return
    /// Returning true causes the current parse to stop. Returning false causes the parse
    /// continue. Return value is ignored if the current parse is not an incremental 
    /// parse ( see XmlFeaturePropertyReader::Parse())
    /// 
	System::Boolean FeatureGeometricProperty(NAMESPACE_OSGEO_FDO_XML::XmlFeatureContext^ context, System::String^ name, array<System::Byte>^ buffer);

    /// \brief
    /// Default feature handler callback that is called when the 
    /// XmlFeaturePropertyReader encounters the start of an object property. 
    /// Does nothing.
    /// 
    /// \param context 
    /// Input caller specified contextual information
    /// \param name 
    /// Input object property name
    /// \param value 
    /// Input definition for the object property's class.
    /// 
    /// \return
    /// Returns the feature Handler for the current object property's sub-properties. 
    /// If nullptr, this feature handler remains in effect.
    /// 
	NAMESPACE_OSGEO_FDO_XML::XmlFeatureHandler^ FeatureStartObjectProperty(NAMESPACE_OSGEO_FDO_XML::XmlFeatureContext^ context, System::String^ name, NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition^ value);

    /// \brief
    /// Default feature handler callback that is called when the 
    /// XmlFeaturePropertyReader encounters the end of an object property.
    /// Does nothing
    /// 
    /// \param context 
    /// Input caller specified contextual information
    /// 
    /// \return
    /// Returning true causes the current parse to stop. Returning false causes the parse
    /// to continue. Return value is ignored if the current parse is not an incremental 
    /// parse ( see XmlFeaturePropertyReader::Parse())
    /// 
	System::Boolean FeatureEndObjectProperty(NAMESPACE_OSGEO_FDO_XML::XmlFeatureContext^ context);

    /// \brief
    /// Default feature handler callback that is called when the 
    /// XmlFeaturePropertyReader encounters the start of an association property. 
    /// Does nothing by default.
    /// A number of other events are fired between FeatureStartAssociationProperty
    /// and FeatureEndAssociationProperty, to identify the associated feature 
    /// referenced by the current association property:
    /// <ul>
    /// 	<li> if the associated feature is specified by an xlink:href, then 
    ///   the href value is sent through a single FeatureProperty event.
    ///   The property is named "gml/id".
    /// 	<li> if the associated feature is specified by its identity property
    ///   values, then a FeatureProperty event is fired for each identity
    ///   property value.
    /// 	<li> if the associated feature is inline (specified by a sub-element of
    ///   current association property) then a set of FeatureStart, 
    ///   FeatureProperty, and FeatureEnd events are fired.
    /// </ul>
    /// 
    /// \param context 
    /// Input caller specified contextual information
    /// \param name 
    /// Input association property name
    /// \param value 
    /// Input definition for the association property's 
    /// associated class.
    /// 
    /// \return
    /// Returns the feature Handler for the current association property's sub-properties. 
    /// If nullptr, this feature handler remains in effect.
    /// 
	NAMESPACE_OSGEO_FDO_XML::XmlFeatureHandler^ FeatureStartAssociationProperty(NAMESPACE_OSGEO_FDO_XML::XmlFeatureContext^ context, System::String^ name, NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition^ value);

    /// \brief
    /// Default feature handler callback that is called when the 
    /// XmlFeaturePropertyReader encounters the end of an association property.
    /// Does nothing
    /// 
    /// \param context 
    /// Input caller specified contextual information
    /// 
    /// \return
    /// Returning true causes the current parse to stop. Returning false causes the parse
    /// to continue. Return value is ignored if the current parse is not an incremental 
    /// parse ( see XmlFeaturePropertyReader::Parse())
    /// 
	System::Boolean FeatureEndAssociationProperty(NAMESPACE_OSGEO_FDO_XML::XmlFeatureContext^ context);

    /// \brief
    /// Default Feature Handle callback that is called when the XmlFeaturePropertyReader 
    /// reads a chunk of data for a CLOB property. Does nothing by default.
    /// This function may be called multiple times for the same property, if the 
    /// the property value is large. Applications must not make any assumptions about the
    /// chunk size or number of chunks for each property.
    /// 
    /// \param context 
    /// Input caller specified contextual information
    /// \param value 
    /// Input the next chunk of simple content
    /// 
    /// \return
    /// Returning true causes the current parse to stop. Returning false causes the parse
    /// to continue. Return value is ignored if the current parse is not an incremental 
    /// parse ( see XmlFeaturePropertyReader::Parse())
    /// 
	System::Boolean FeatureCharacters(NAMESPACE_OSGEO_FDO_XML::XmlFeatureContext^ context, System::String^ value);

    /// \brief
    /// Default Feature callback that is called when the XmlFeaturePropertyReader 
    /// reads a chunk of data for a Raster or BLOB property. 
    /// Does nothing by default.
    /// This function may be called multiple times for the same property, if the 
    /// the property value is large. Applications must not make any assumptions about the
    /// chunk size or number of chunks for each property.
    /// 
    /// \param context 
    /// Input caller specified contextual information
    /// \param buffer 
    /// Input the next chunk of binary content in unencoded form. An exception is thrown
    /// if the content cannot be decoded.
    /// 
    /// \return
    /// Returning true causes the current parse to stop. Returning false causes the parse
    /// to continue. Return value is ignored if the current parse is not an incremental 
    /// parse ( see XmlFeaturePropertyReader::Parse())
    /// 
	System::Boolean FeatureBinaryData(NAMESPACE_OSGEO_FDO_XML::XmlFeatureContext^ context, array<System::Byte>^ buffer);

internal:
	XmlFeatureHandler(System::IntPtr unmanaged, System::Boolean autoDelete);

	inline FdoXmlFeatureHandler* GetImpObj();
public:
    virtual IntPtr GetDisposableObject() override;
};

END_NAMESPACE_OSGEO_FDO_XML


