#ifndef FDO_XML_FEATUREHANDLER_H
#define FDO_XML_FEATUREHANDLER_H

//
// Copyright (C) 2004-2006  Autodesk, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
#ifdef _WIN32
#pragma once
#endif

#include <Std.h>

class FdoXmlFeatureContext;
class FdoClassDefinition;
class FdoIRaster;


/// \brief
/// FdoXmlFeatureHandler defines the callback interface for reading features from XML.
/// It is not an interface in the strict sense, since each function has a default 
/// implementation that does nothing. Implementors can override these callbacks to 
/// customize the handling of feature fragments parsed by FdoXmlFeaturePropertyReader.
/// Note that the default FeatureProperty implementations log errors in certain circumstances.
/// Therefore, these functions must be overridden to avoid these errors and indicate that 
/// these types of properties are being handled by the caller.
class FdoXmlFeatureHandler
{
public:

    /// \brief
    /// Default feature handler callback that is called when the 
    /// FdoXmlFeaturePropertyReader starts reading an XML document. Does nothing.
    /// 
    /// \param featureContext 
    /// Input caller specified contextual information
    /// 
    /// \return
    /// Returns the feature Handler for all sub-elements of the element that was current
    /// when the read started. If NULL, this feature handler remains in effect.
    /// 
    FDO_API virtual FdoXmlFeatureHandler* FeatureDocumentStart(FdoXmlFeatureContext*);

    /// \brief
    /// Default feature handler callback that is called when the 
    /// FdoXmlFeaturePropertyReader 
    /// finishes reading an XML document. Does nothing
    /// 
    /// \param featureContext 
    /// Input caller specified contextual information
    /// 
    FDO_API virtual void FeatureDocumentEnd(FdoXmlFeatureContext*);

    /// \brief
    /// Default feature handler callback that is called when the 
    /// FdoXmlFeaturePropertyReader encounters the start of a feature collection. 
    /// Does nothing.
    /// 
    /// \param featureContext 
    /// Input caller specified contextual information
    /// 
    /// \return
    /// Returns the feature Handler for the features in this collection.
    /// If NULL, this feature handler remains in effect.
    /// 
    FDO_API virtual FdoXmlFeatureHandler* FeatureCollectionStart(FdoXmlFeatureContext*,FdoClassDefinition*);

    /// \brief
    /// Default feature handler callback that is called when the 
    /// FdoXmlFeaturePropertyReader encounters the end of a feature collection. 
    /// Does nothing
    /// 
    /// \param featureContext 
    /// Input caller specified contextual information
    /// 
    /// \return
    /// Returning true causes the current parse to stop. Returning false causes the parse
    /// to continue. Return value is ignored if the current parse is not an incremental 
    /// parse ( see FdoXmlFeaturePropertyReader::Parse())
    /// 
    FDO_API virtual FdoBoolean FeatureCollectionEnd(FdoXmlFeatureContext*);

    /// \brief
    /// Default feature handler callback that is called when the 
    /// FdoXmlFeaturePropertyReader encounters the start of a feature. Does nothing.
    /// 
    /// \param featureContext 
    /// Input caller specified contextual information
    /// \param classDefinition 
    /// Input definition containing feature class name, schema name, valid property list,
    /// etc.
    /// 
    /// \return
    /// Returns the feature Handler for the feature's properties. If NULL, this feature 
    /// handler remains in effect.
    /// 
    FDO_API virtual FdoXmlFeatureHandler* FeatureStart(FdoXmlFeatureContext*,FdoClassDefinition*);

    /// \brief
    /// Default feature handler callback that is called when the 
    /// FdoXmlFeaturePropertyReader encounters the end of a feature. Does nothing.
    /// 
    /// \param featureContext 
    /// Input caller specified contextual information
    /// 
    /// \return
    /// Returning true causes the current parse to stop. Returning false causes the 
    /// parse to continue. Return value is ignored if the current parse is not an 
    /// incremental parse ( see FdoXmlFeaturePropertyReader::Parse())
    /// 
    FDO_API virtual FdoBoolean FeatureEnd(FdoXmlFeatureContext*);

    /// \brief
    /// Default feature handler callback that is called when the 
    /// FdoXmlFeaturePropertyReader encounters a property with boolean value. 
    /// Logs an error when the current ErrorLevel is High or Normal. An error is 
    /// also logged when the ErrorLevel is Low and the current property is not nullable.
    /// 
    /// \param featureContext 
    /// Input caller specified contextual information
    /// \param name 
    /// Input property name
    /// \param name 
    /// Input property value
    /// 
    /// \return
    /// Returning true causes the current parse to stop. Returning false causes
    /// the parse to continue. Return value is ignored if the current parse is not an
    /// incremental parse ( see FdoXmlFeaturePropertyReader::Parse())
    /// 
    FDO_API virtual FdoBoolean FeatureProperty(FdoXmlFeatureContext*, FdoString*, FdoBoolean);

    /// \brief
    /// Default feature handler callback that is called when the 
    /// FdoXmlFeaturePropertyReader encounters a property with single byte value. 
    /// Logs an error when the current ErrorLevel is High or Normal. An error is 
    /// also logged when the ErrorLevel is Low and the current property is not nullable.
    /// 
    /// \param featureContext 
    /// Input caller specified contextual information
    /// \param name 
    /// Input property name
    /// \param name 
    /// Input property value
    /// 
    /// \return
    /// Returning true causes the current parse to stop. Returning false causes
    /// the parse to continue. Return value is ignored if the current parse is not an
    /// incremental parse ( see FdoXmlFeaturePropertyReader::Parse())
    /// 
    FDO_API virtual FdoBoolean FeatureProperty(FdoXmlFeatureContext*, FdoString*, FdoByte);

    /// \brief
    /// Default feature handler callback that is called when the 
    /// FdoXmlFeaturePropertyReader encounters a property with DataTime value. 
    /// Logs an error when the current ErrorLevel is High or Normal. An error is 
    /// also logged when the ErrorLevel is Low and the current property is not nullable.
    /// 
    /// \param featureContext 
    /// Input caller specified contextual information
    /// \param name 
    /// Input property name
    /// \param name 
    /// Input property value
    /// 
    /// \return
    /// Returning true causes the current parse to stop. Returning false causes
    /// the parse to continue. Return value is ignored if the current parse is not an
    /// incremental parse ( see FdoXmlFeaturePropertyReader::Parse())
    /// 
    FDO_API virtual FdoBoolean FeatureProperty(FdoXmlFeatureContext*, FdoString*, FdoDateTime);

    /// \brief
    /// Default feature handler callback that is called when the 
    /// FdoXmlFeaturePropertyReader encounters a property with double value. 
    /// Logs an error when the current ErrorLevel is High or Normal. An error is 
    /// also logged when the ErrorLevel is Low and the current property is not nullable.
    /// 
    /// \param featureContext 
    /// Input caller specified contextual information
    /// \param name 
    /// Input property name
    /// \param name 
    /// Input property value
    /// 
    /// \return
    /// Returning true causes the current parse to stop. Returning false causes
    /// the parse to continue. Return value is ignored if the current parse is not an
    /// incremental parse ( see FdoXmlFeaturePropertyReader::Parse())
    /// 
    FDO_API virtual FdoBoolean FeatureProperty(FdoXmlFeatureContext*, FdoString*, FdoDouble);

    /// \brief
    /// Default feature handler callback that is called when the 
    /// FdoXmlFeaturePropertyReader encounters a property with int16 value. 
    /// Logs an error when the current ErrorLevel is High or Normal. An error is 
    /// also logged when the ErrorLevel is Low and the current property is not nullable.
    /// 
    /// \param featureContext 
    /// Input caller specified contextual information
    /// \param name 
    /// Input property name
    /// \param name 
    /// Input property value
    /// 
    /// \return
    /// Returning true causes the current parse to stop. Returning false causes
    /// the parse to continue. Return value is ignored if the current parse is not an
    /// incremental parse ( see FdoXmlFeaturePropertyReader::Parse())
    /// 
    FDO_API virtual FdoBoolean FeatureProperty(FdoXmlFeatureContext*, FdoString*, FdoInt16);

    /// \brief
    /// Default feature handler callback that is called when the 
    /// FdoXmlFeaturePropertyReader encounters a property with int32 value. 
    /// Logs an error when the current ErrorLevel is High or Normal. An error is 
    /// also logged when the ErrorLevel is Low and the current property is not nullable.
    /// 
    /// \param featureContext 
    /// Input caller specified contextual information
    /// \param name 
    /// Input property name
    /// \param name 
    /// Input property value
    /// 
    /// \return
    /// Returning true causes the current parse to stop. Returning false causes
    /// the parse to continue. Return value is ignored if the current parse is not an
    /// incremental parse ( see FdoXmlFeaturePropertyReader::Parse())
    /// 
    FDO_API virtual FdoBoolean FeatureProperty(FdoXmlFeatureContext*, FdoString*, FdoInt32);

    /// \brief
    /// Default feature handler callback that is called when the 
    /// FdoXmlFeaturePropertyReader encounters a property with int64 value. 
    /// Logs an error when the current ErrorLevel is High or Normal. An error is 
    /// also logged when the ErrorLevel is Low and the current property is not nullable.
    /// 
    /// \param featureContext 
    /// Input caller specified contextual information
    /// \param name 
    /// Input property name
    /// \param name 
    /// Input property value
    /// 
    /// \return
    /// Returning true causes the current parse to stop. Returning false causes
    /// the parse to continue. Return value is ignored if the current parse is not an
    /// incremental parse ( see FdoXmlFeaturePropertyReader::Parse())
    /// 
    FDO_API virtual FdoBoolean FeatureProperty(FdoXmlFeatureContext*, FdoString*, FdoInt64);

    /// \brief
    /// Default feature handler callback that is called when the 
    /// FdoXmlFeaturePropertyReader encounters a property with single precision value. 
    /// Logs an error when the current ErrorLevel is High or Normal. An error is 
    /// also logged when the ErrorLevel is Low and the current property is not nullable.
    /// 
    /// \param featureContext 
    /// Input caller specified contextual information
    /// \param name 
    /// Input property name
    /// \param name 
    /// Input property value
    /// 
    /// \return
    /// Returning true causes the current parse to stop. Returning false causes
    /// the parse to continue. Return value is ignored if the current parse is not an
    /// incremental parse ( see FdoXmlFeaturePropertyReader::Parse())
    /// 
    FDO_API virtual FdoBoolean FeatureProperty(FdoXmlFeatureContext*, FdoString*, FdoFloat);

    /// \brief
    /// Default feature handler callback that is called when the 
    /// FdoXmlFeaturePropertyReader encounters a property with string value. 
    /// Logs an error when the current ErrorLevel is High or Normal. An error is 
    /// also logged when the ErrorLevel is Low and the current property is not nullable.
    /// 
    /// \param featureContext 
    /// Input caller specified contextual information
    /// \param name 
    /// Input property name
    /// \param name 
    /// Input property value
    /// 
    /// \return
    /// Returning true causes the current parse to stop. Returning false causes
    /// the parse to continue. Return value is ignored if the current parse is not an
    /// incremental parse ( see FdoXmlFeaturePropertyReader::Parse())
    /// 
    FDO_API virtual FdoBoolean FeatureProperty(FdoXmlFeatureContext*, FdoString*, FdoString*);

    /// \brief
    /// Default feature handler callback that is called when the 
    /// FdoXmlFeaturePropertyReader encounters the start of a raster property.
    /// Logs an error when the current ErrorLevel is High or Normal. An error is 
    /// also logged when the ErrorLevel is Low and the current property is not nullable.
    /// The raster image is sent by subsequent calls to FeatureBinaryData().
    /// 
    /// \param featureContext 
    /// Input caller specified contextual information
    /// \param name 
    /// Input property name
    /// \param raster 
    /// Input raster image properties. This object always has a NULL stream reader. The image is
    /// handled by the FeatureBinaryData() callback.
    /// 
    /// \return
    /// Returning true causes the current parse to stop. Returning false causes the 
    /// parse to continue. Return value is ignored if the current parse is not an 
    /// incremental parse ( see FdoXmlFeaturePropertyReader::Parse())
    /// 
    FDO_API virtual FdoBoolean FeatureStartRasterProperty(FdoXmlFeatureContext*, FdoString*, FdoIRaster*);

    /// \brief
    /// Default feature handler callback that is called when the 
    /// FdoXmlFeaturePropertyReader encounters the end of a raster property.
    /// Does nothing
    /// 
    /// \param featureContext 
    /// Input caller specified contextual information
    /// 
    /// \return
    /// Returning true causes the current parse to stop. Returning false causes the parse
    /// to continue. Return value is ignored if the current parse is not an incremental 
    /// parse ( see FdoXmlFeaturePropertyReader::Parse())
    /// 
    FDO_API virtual FdoBoolean FeatureEndRasterProperty(FdoXmlFeatureContext*);

    /// \brief
    /// Default feature handler callback that is called when the 
    /// FdoXmlFeaturePropertyReader encounters  the start of a lob property. 
    /// Logs an error when the current ErrorLevel is High or Normal. 
    /// If this is a BLOB property then its value is sent through the FeatureBinaryData() callback.
    /// If this is a CLOB property then its value is sent through the FeatureCharacters() callback.
    /// 
    /// \param featureContext 
    /// Input caller specified contextual information
    /// \param name 
    /// Input property name
    /// 
    /// \return
    /// Returning true causes the current parse to stop. Returning false cause the parse
    /// to continue. Return value is ignored if the current parse is not an incremental 
    /// parse ( see FdoXmlFeaturePropertyReader::Parse())
    /// 
    FDO_API virtual FdoBoolean FeatureStartLobProperty(FdoXmlFeatureContext*, FdoString*);

    /// \brief
    /// Default feature handler callback that is called when the 
    /// FdoXmlFeaturePropertyReader encounters the end of a lob property.
    /// Does nothing
    /// 
    /// \param featureContext 
    /// Input caller specified contextual information
    /// 
    /// \return
    /// Returning true causes the current parse to stop. Returning false causes the parse
    /// to continue. Return value is ignored if the current parse is not an incremental 
    /// parse ( see FdoXmlFeaturePropertyReader::Parse())
    /// 
    FDO_API virtual FdoBoolean FeatureEndLobProperty(FdoXmlFeatureContext*);

    /// \brief
    /// Default feature handler callback that is called when the 
    /// FdoXmlFeaturePropertyReader encounters a geometric property.
    /// Logs an error when the current ErrorLevel is High or Normal. 
    /// 
    /// \param featureContext 
    /// Input caller specified contextual information
    /// \param name 
    /// Input property name
    /// \param bytes 
    /// Input pointer to the byte array defining the geometry value
    /// \param count 
    /// Input number of bytes in the byte array
    /// 
    /// \return
    /// Returning true causes the current parse to stop. Returning false causes the parse
    /// continue. Return value is ignored if the current parse is not an incremental 
    /// parse ( see FdoXmlFeaturePropertyReader::Parse())
    /// 
    FDO_API virtual FdoBoolean FeatureGeometricProperty(FdoXmlFeatureContext*, FdoString*, FdoByte*, FdoInt32);

    /// \brief
    /// Default feature handler callback that is called when the 
    /// FdoXmlFeaturePropertyReader encounters the start of an object property. 
    /// Does nothing.
    /// 
    /// \param featureContext 
    /// Input caller specified contextual information
    /// \param name 
    /// Input object property name
    /// \param classDefinition 
    /// Input definition for the object property's class.
    /// 
    /// \return
    /// Returns the feature Handler for the current object property's sub-properties. 
    /// If NULL, this feature handler remains in effect.
    /// 
    FDO_API virtual FdoXmlFeatureHandler* FeatureStartObjectProperty(FdoXmlFeatureContext*, FdoString*, FdoClassDefinition*);

    /// \brief
    /// Default feature handler callback that is called when the 
    /// FdoXmlFeaturePropertyReader encounters the end of an object property.
    /// Does nothing
    /// 
    /// \param featureContext 
    /// Input caller specified contextual information
    /// 
    /// \return
    /// Returning true causes the current parse to stop. Returning false causes the parse
    /// to continue. Return value is ignored if the current parse is not an incremental 
    /// parse ( see FdoXmlFeaturePropertyReader::Parse())
    /// 
    FDO_API virtual FdoBoolean FeatureEndObjectProperty(FdoXmlFeatureContext*);

    /// \brief
    /// Default feature handler callback that is called when the 
    /// FdoXmlFeaturePropertyReader encounters the start of an association property. 
    /// Does nothing by default.
    /// A number of other events are fired between FeatureStartAssociationProperty
    /// and FeatureEndAssociationProperty, to identify the associated feature 
    /// referenced by the current association property:
    /// <ul>
    /// 	<li> if the associated feature is specified by an xlink:href, then 
    ///       the href value is sent through a single FeatureProperty event.
    ///       The property is named "gml/id".
    /// 	<li> if the associated feature is specified by its identity property
    ///       values, then a FeatureProperty event is fired for each identity
    ///       property value.
    /// 	<li> if the associated feature is inline (specified by a sub-element of
    ///       current association property) then a set of FeatureStart, 
    ///       FeatureProperty, and FeatureEnd events are fired.
    /// </ul>
    /// 
    /// \param featureContext 
    /// Input caller specified contextual information
    /// \param name 
    /// Input association property name
    /// \param classDefinition 
    /// Input definition for the association property's 
    /// associated class.
    /// 
    /// \return
    /// Returns the feature Handler for the current association property's sub-properties. 
    /// If NULL, this feature handler remains in effect.
    /// 
    FDO_API virtual FdoXmlFeatureHandler* FeatureStartAssociationProperty(FdoXmlFeatureContext*, FdoString*, FdoClassDefinition*);

    /// \brief
    /// Default feature handler callback that is called when the 
    /// FdoXmlFeaturePropertyReader encounters the end of an association property.
    /// Does nothing
    /// 
    /// \param featureContext 
    /// Input caller specified contextual information
    /// 
    /// \return
    /// Returning true causes the current parse to stop. Returning false causes the parse
    /// to continue. Return value is ignored if the current parse is not an incremental 
    /// parse ( see FdoXmlFeaturePropertyReader::Parse())
    /// 
    FDO_API virtual FdoBoolean FeatureEndAssociationProperty(FdoXmlFeatureContext*);

    /// \brief
    /// Default Feature Handle callback that is called when the FdoXmlFeaturePropertyReader 
    /// reads a chunk of data for a CLOB property. Does nothing by default.
    /// This function may be called multiple times for the same property, if the 
    /// the property value is large. Applications must not make any assumptions about the
    /// chunk size or number of chunks for each property.
    /// 
    /// \param featureContext 
    /// Input caller specified contextual information
    /// \param chars 
    /// Input the next chunk of simple content
    /// 
    /// \return
    /// Returning true causes the current parse to stop. Returning false causes the parse
    /// to continue. Return value is ignored if the current parse is not an incremental 
    /// parse ( see FdoXmlFeaturePropertyReader::Parse())
    /// 
    FDO_API virtual FdoBoolean FeatureCharacters(FdoXmlFeatureContext*, FdoString*);

    /// \brief
    /// Default Feature callback that is called when the FdoXmlFeaturePropertyReader 
    /// reads a chunk of data for a Raster or BLOB property. 
    /// Does nothing by default.
    /// This function may be called multiple times for the same property, if the 
    /// the property value is large. Applications must not make any assumptions about the
    /// chunk size or number of chunks for each property.
    /// 
    /// \param featureContext 
    /// Input caller specified contextual information
    /// \param bytes 
    /// Input the next chunk of binary content in unencoded form. An exception is thrown
    /// if the content cannot be decoded.
    /// \param count 
    /// Input the number of bytes of binary content.
    /// 
    /// \return
    /// Returning true causes the current parse to stop. Returning false causes the parse
    /// to continue. Return value is ignored if the current parse is not an incremental 
    /// parse ( see FdoXmlFeaturePropertyReader::Parse())
    /// 
    FDO_API virtual FdoBoolean FeatureBinaryData(FdoXmlFeatureContext*, FdoByte*, FdoSize);

};

#endif


