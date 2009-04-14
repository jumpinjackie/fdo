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

#include "FDO\Commands\Feature\mgIFeatureReader.h"
#include "FDO\Xml\mgXmlFeatureHandler.h"

class FdoXmlFeatureReader;

BEGIN_NAMESPACE_OSGEO_COMMON_XML
public __gc class XmlReader;
END_NAMESPACE_OSGEO_COMMON_XML

BEGIN_NAMESPACE_OSGEO_FDO_SCHEMA
public __gc class FeatureSchemaCollection;
END_NAMESPACE_OSGEO_FDO_SCHEMA

BEGIN_NAMESPACE_OSGEO_FDO_XML
public __gc class XmlFeatureFlags;
public __gc class XmlFeaturePropertyReader;

/// \ingroup (OSGeoFDOXml)
/// \brief
/// XmlFeatureReader reads GML format features from a XML
/// document. The reading is done procedurally, rather than through events.
/// \remarks
/// Each feature can be read in 3 steps:
/// <ol>
///     <li>call ReadNext() to get the next feature.
///     <li>call GetClassDefinition() to get the current feature's property list.
///     <li>call GetProperty() for each feature property to retrieve.
/// </ol>
/// This class allows random access to the properties of the current feature, meaning that 
/// ReadNext() accumulates a list of these properties. This may have slight performance implications.
/// If performance is a concern then XmlFeaturePropertyReader should be used instead.
public __gc class XmlFeatureReader : public NAMESPACE_OSGEO_FDO_XML::XmlFeatureHandler, public NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IFeatureReader
{
public:
    /// \brief
    /// creates a Feature Reader for reading features from XML.
    /// 
    /// \param reader 
    /// Input XML document reader. Please refer to the reader parameter of 
    /// XmlFeaturePropertyReader::Create() for a description of how the features are
    /// read from the document. An XmlFeaturePropertyReader is automatically wrapped 
    /// around this reader. This Feature Property Reader can be retrieved by calling 
    /// GetFeaturePropertyReader().
    /// 
	XmlFeatureReader(NAMESPACE_OSGEO_COMMON_XML::XmlReader* reader);

    /// \brief
    /// creates a Feature Reader for reading features from XML.
    /// 
    /// \param reader 
    /// Input XML document reader. Please refer to the reader parameter of 
    /// XmlFeaturePropertyReader::Create() for a description of how the features are
    /// read from the document. An XmlFeaturePropertyReader is automatically wrapped 
    /// around this reader. This Feature Property Reader can be retrieved by calling 
    /// GetFeaturePropertyReader().
    /// \param flags 
    /// Input options for controlling the 
    /// deserializing of the features. If NULL then the 
    /// default flags are used.
    /// 
	XmlFeatureReader(NAMESPACE_OSGEO_COMMON_XML::XmlReader* reader, NAMESPACE_OSGEO_FDO_XML::XmlFeatureFlags* flags);

    /// \brief
    /// Gets the feature property reader that was passed to this object.
    /// 
    /// \return
    /// Returns XmlFeaturePropertyReader
    /// 
	NAMESPACE_OSGEO_FDO_XML::XmlFeaturePropertyReader* GetFeaturePropertyReader();

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
    /// The feature schemas
    /// provide directions on how to deserialize the features and convert their property
    /// values from the strings in the XML document to their proper types. If any 
    /// feature, whose class definition is not present in these schemas, is encountered 
    /// then one of the following is done:
    /// <ul>
    /// 	<li> an exception is thrown when flags->errorLevel is Normal or higher. The 
    ///       exception reports all such features.
    /// 	<li> these features are silently skipped When the flags->errorLevel is below 
    ///       Normal.
    /// </ul>
    /// 
    /// \param schemas 
    /// Input the feature schemas
    /// 
	__property System::Void set_FeatureSchemas(NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection* schemas);

	/*
		IFeatureReader implementation
	*/

    /// \brief
    /// Gets the definition of the object currently being read. If the user
    /// has requested only a subset of the class properties, the class 
    /// definition reflects what the user has asked, rather than the full class 
    /// definition.
    /// 
    /// \return
    /// Returns the class definition object.
    /// 
	NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition* GetClassDefinition();

    /// \brief
    /// Gets a value indicating the depth of nesting for the current reader.
    /// The depth value increases each time GetFeatureObject is called and a new 
    /// reader is returned. The outermost reader has a depth of 0.
    /// 
    /// \return
    /// Returns the depth
    /// 
	System::Int32 GetDepth();

    /// \brief
    /// Gets a reference to an IFeatureReader to read the data contained in
    /// the object or object collection property. If the property is not an
    /// object property, an exception is thrown.
    /// 
    /// \param propertyName 
    /// Input the property name.
    /// 
    /// \return
    /// Returns the nested feature reader
    /// 
	NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IFeatureReader* GetFeatureObject(System::String* propertyName);

	/*
		IReader implementation
	*/

    /// \brief
    /// Gets the Boolean value of the specified property. No conversion is
    /// performed, thus the property must be DataType_Boolean or an 
    /// exception is thrown.
    /// 
    /// \param name 
    /// Input the property name.
    /// 
    /// \return
    /// Returns the Boolean value.
    /// 
	System::Boolean GetBoolean(System::String* name);

    /// \brief
    /// Gets the byte value of the specified property. No conversion is 
    /// performed, thus the property must be DataType_Byte or an 
    /// exception is thrown.
    /// 
    /// \param name 
    /// Input the property name.
    /// 
    /// \return
    /// Returns the byte value.
    /// 
	System::Byte GetByte(System::String* name);

    /// \brief
    ///  Gets the date and time value of the specified property. No conversion is 
    /// performed, thus the property must be DataType_DateTime or an 
    /// exception is thrown.
    /// 
    /// \param name 
    /// Input the property name.
    /// 
    /// \return
    /// Returns the date and time value.
    /// 
	System::DateTime GetDateTime(System::String* name);

    /// \brief
    /// Gets the double-precision floating point value of the specified property. No
    /// conversion is performed, thus the property must be DataType_Double
    /// or an exception is thrown.
    /// 
    /// \param name 
    /// Input the property name.
    /// 
    /// \return
    /// Returns the double floating point value
    /// 
	System::Double GetDouble(System::String* name);

    /// \brief
    /// Gets the 16-bit integer value of the specified property. No conversion is
    /// performed, thus the property must be DataType_Int16 or an exception
    /// is thrown.
    /// 
    /// \param name 
    /// Input the property name.
    /// 
    /// \return
    /// Returns the System::Int16 value.
    /// 
	System::Int16 GetInt16(System::String* name);

    /// \brief
    /// Gets the 32-bit integer value of the specified property. No conversion is
    /// performed, thus the property must be DataType_Int32 or an exception
    /// is thrown.
    /// 
    /// \param name 
    /// Input the property name.
    /// 
    /// \return
    /// Returns the System::Int32 value
    /// 
	System::Int32 GetInt32(System::String* name);

    /// \brief
    /// Gets the 64-bit integer value of the specified property. No conversion is
    /// performed, thus the property must be DataType_Int64 or an exception
    /// is thrown.
    /// 
    /// \param name 
    /// Input the property name.
    /// 
    /// \return
    /// Returns the System::Int63 value.
    /// 
	System::Int64 GetInt64(System::String* name);

    /// \brief
    /// Gets the Single floating point value of the specified property. No
    /// conversion is performed, thus the property must be DataType_Single
    /// or an exception is thrown.
    /// 
    /// \param name 
    /// Input the property name.
    /// 
    /// \return
    /// Returns the single value
    /// 
	System::Single GetSingle(System::String* name);

    /// \brief
    /// Gets the string value of the specified property. No conversion is
    /// performed, thus the property must be DataType_String or an exception
    /// is thrown.
    /// 
    /// \param name 
    /// Input the property name.
    /// 
    /// \return
    /// Returns the string value
    /// 
	System::String* GetString(System::String* name);

    /// \brief
    /// Gets a LOBValue reference. The LOB is fully read in and data available.
    /// Because no conversion is performed, the property must be DataType_BLOB or
    /// DataType_CLOB etc. (a LOB type)
    /// 
    /// \param name 
    /// Input the property name.
    /// 
    /// \return
    /// Returns the reference to LOBValue
    /// 
	NAMESPACE_OSGEO_FDO_EXPRESSION::LOBValue* GetLOB(System::String* name);

    /// \brief
    /// Gets a reference of the specified LOB property as a BLOBStreamReader or
    /// CLOBStreamReader etc. to allow reading in blocks of data.
    /// Because no conversion is performed, the property must be DataType_BLOB 
    /// or DataType_CLOB etc. (a LOB type)
    /// Cast the IStreamReader to the appropiate LOB Stream Reader.
    /// 
    /// \param name 
    /// Input the property name.
    /// 
    /// \return
    /// Returns a reference to a LOB stream reader
    /// 
	NAMESPACE_OSGEO_COMMON::IStreamReader* GetLOBStreamReader(System::String* name);

    /// \brief
    /// Returns true if the value of the specified property is null.
    /// 
    /// \param name 
    /// Input the property name.
    /// 
    /// \return
    /// Returns true if the value is null.
    /// 
	System::Boolean IsNull(System::String* name);

    /// \brief
    /// Gets the geometry value of the specified property as a byte array in 
    /// FGF format. Because no conversion is performed, the property must be
    /// of Geometric type; otherwise, an exception is thrown.
    /// 
    /// \param name 
    /// Input the property name.
    /// 
    /// \return
    /// Returns the byte array in FGF format.
    /// 
	System::Byte GetGeometry(System::String* name) [];

    /// \brief
    /// Gets the raster object of the specified property.
    /// Because no conversion is performed, the property must be
    /// of Raster type; otherwise, an exception is thrown.
    /// 
    /// \param name 
    /// Input the property name.
    /// 
    /// \return
    /// Returns the raster object.
    /// 
	NAMESPACE_OSGEO_FDO_RASTER::IRaster* GetRaster(System::String* name);

    /// \brief
    /// Advances the reader to the next item and returns true if there is
    /// another object to read or false if reading is complete. The default
    /// position of the reader is prior to the first item. Thus you must
    /// call ReadNext to begin accessing any data.
    /// 
    /// \return
    /// Returns true if there is a next item.
    /// 
	System::Boolean ReadNext();

    /// \brief
    /// Closes the IFeatureReader object, freeing any resources it may be holding.
    /// 
	System::Void Close();

public private:
	XmlFeatureReader(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_XML::XmlFeatureHandler(unmanaged, autoDelete)
	{
		
	}

	inline FdoXmlFeatureReader* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_XML


