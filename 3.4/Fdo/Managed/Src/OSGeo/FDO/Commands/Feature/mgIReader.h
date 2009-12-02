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

BEGIN_NAMESPACE_OSGEO_FDO_EXPRESSION
public __gc class LOBValue;
END_NAMESPACE_OSGEO_FDO_EXPRESSION

BEGIN_NAMESPACE_OSGEO_COMMON
public __gc __interface IStreamReader;
END_NAMESPACE_OSGEO_COMMON

BEGIN_NAMESPACE_OSGEO_FDO_RASTER
public __gc __interface IRaster;
END_NAMESPACE_OSGEO_FDO_RASTER

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE

/// \ingroup (OSGeoFDOCommandsFeature)
/// \interface OSGeo::FDO::Commands::Feature::IReader
/// \brief
/// The IReader interface provides a forward-only, read-only iterator
/// for reading data. Because the initial position of the
/// IReader is prior to the first item, you must call
/// ReadNext to begin accessing any data.
public __gc __interface IReader : public System::IDisposable
{
public:
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
    /// Closes the IReader object, freeing any resources it may be holding.
    /// 
    /// \return
    /// Returns nothing
    /// 
	System::Void Close();
};

END_NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE


