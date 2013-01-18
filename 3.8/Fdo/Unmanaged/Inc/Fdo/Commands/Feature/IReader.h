#ifndef _IREADER_H_
#define _IREADER_H_
// 

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

#include <FdoStd.h>
#include <Fdo/Schema/ClassDefinition.h>
#include <Fdo/Raster/IRaster.h>
#include <Fdo/Expression/LOBValue.h>

/// \brief
/// The FdoIFeatureReader interface provides a forward-only, read-only iterator
/// for reading feature data.  A reference to an FdoIFeatureReader is returned
/// from the Select and SelectAndLock commands. Because the initial position of the
/// FdoIFeatureReader is prior to the first item, you must call
/// ReadNext to begin accessing any data.
class FdoIReader: public FdoIDisposable
{
public:
    /// \brief
    /// Gets the name of the property at the given ordinal position.
    /// 
    /// \param index 
    /// Input the position of the property.
    /// 
    /// \return
    /// Returns the property name
    /// 
    FDO_API virtual FdoString* GetPropertyName(FdoInt32 index) = 0;

    /// \brief
    /// Gets the index of the property with the specified name.
    /// 
    /// \param propertyName 
    /// Input the property name.
    /// 
    /// \return
    /// Returns the property index
    /// 
    FDO_API virtual FdoInt32 GetPropertyIndex(FdoString* propertyName) = 0;

    /// \brief
    /// Gets the Boolean value of the specified property. No conversion is
    /// performed, thus the property must be FdoDataType_Boolean or an 
    /// exception is thrown.
    /// 
    /// \param propertyName 
    /// Input the property name.
    /// 
    /// \return
    /// Returns the Boolean value.
    /// 
    FDO_API virtual bool GetBoolean(FdoString* propertyName) = 0;

    /// \brief
    /// Gets the byte value of the specified property. No conversion is 
    /// performed, thus the property must be FdoDataType_Byte or an 
    /// exception is thrown.
    /// 
    /// \param propertyName 
    /// Input the property name.
    /// 
    /// \return
    /// Returns the byte value.
    /// 
    FDO_API virtual FdoByte GetByte(FdoString* propertyName) = 0;

    /// \brief
    ///  Gets the date and time value of the specified property. No conversion is 
    /// performed, thus the property must be FdoDataType_DateTime or an 
    /// exception is thrown.
    /// 
    /// \param propertyName 
    /// Input the property name.
    /// 
    /// \return
    /// Returns the date and time value.
    /// 
    FDO_API virtual FdoDateTime GetDateTime(FdoString* propertyName) = 0;

    /// \brief
    /// Gets the double-precision floating point value of the specified property. No
    /// conversion is performed, thus the property must be FdoDataType_Double
    /// or an exception is thrown.
    /// 
    /// \param propertyName 
    /// Input the property name.
    /// 
    /// \return
    /// Returns the double floating point value
    /// 
    FDO_API virtual double GetDouble(FdoString* propertyName) = 0;

    /// \brief
    /// Gets the 16-bit integer value of the specified property. No conversion is
    /// performed, thus the property must be FdoDataType_Int16 or an exception
    /// is thrown.
    /// 
    /// \param propertyName 
    /// Input the property name.
    /// 
    /// \return
    /// Returns the FdoInt16 value.
    /// 
    FDO_API virtual FdoInt16 GetInt16(FdoString* propertyName) = 0;

    /// \brief
    /// Gets the 32-bit integer value of the specified property. No conversion is
    /// performed, thus the property must be FdoDataType_Int32 or an exception
    /// is thrown.
    /// 
    /// \param propertyName 
    /// Input the property name.
    /// 
    /// \return
    /// Returns the FdoInt32 value
    /// 
    FDO_API virtual FdoInt32 GetInt32(FdoString* propertyName) = 0;

    /// \brief
    /// Gets the 64-bit integer value of the specified property. No conversion is
    /// performed, thus the property must be FdoDataType_Int64 or an exception
    /// is thrown.
    /// 
    /// \param propertyName 
    /// Input the property name.
    /// 
    /// \return
    /// Returns the FdoInt64 value.
    /// 
    FDO_API virtual FdoInt64 GetInt64(FdoString* propertyName) = 0;

    /// \brief
    /// Gets the Single floating point value of the specified property. No
    /// conversion is performed, thus the property must be FdoDataType_Single
    /// or an exception is thrown.
    /// 
    /// \param propertyName 
    /// Input the property name.
    /// 
    /// \return
    /// Returns the single value
    /// 
    FDO_API virtual float GetSingle(FdoString* propertyName) = 0;

    /// \brief
    /// Gets the string value of the specified property. No conversion is
    /// performed, thus the property must be FdoDataType_String or an exception
    /// is thrown.
    /// 
    /// \param propertyName 
    /// Input the property name.
    /// 
    /// \return
    /// Returns the string value
    /// 
    FDO_API virtual FdoString* GetString(FdoString* propertyName) = 0;

    /// \brief
    /// Gets a LOBValue pointer. The LOB is fully read in and data available.
    /// Because no conversion is performed, the property must be FdoDataType_BLOB or
    /// FdoDataType_CLOB etc. (a LOB type)
    /// 
    /// \param propertyName 
    /// Input the property name.
    /// 
    /// \return
    /// Returns the pointer to LOBValue
    /// 
    FDO_API virtual FdoLOBValue* GetLOB(FdoString* propertyName) = 0;

    /// \brief
    /// Gets a reference of the specified LOB property as a FdoBLOBStreamReader or
    /// FdoCLOBStreamReader etc. to allow reading in blocks of data.
    /// Because no conversion is performed, the property must be FdoDataType_BLOB 
    /// or FdoDataType_CLOB etc. (a LOB type)
    /// Cast the FdoIStreamReader to the appropriate LOB Stream Reader.
    /// 
    /// \param propertyName 
    /// Input the property name.
    /// 
    /// \return
    /// Returns a pointer to a LOB stream reader
    /// 
    FDO_API virtual FdoIStreamReader* GetLOBStreamReader(const wchar_t* propertyName ) = 0;

    /// \brief
    /// Returns true if the value of the specified property is null.
    /// 
    /// \param propertyName 
    /// Input the property name.
    /// 
    /// \return
    /// Returns true if the value is null.
    /// 
    FDO_API virtual bool IsNull(FdoString* propertyName) = 0;

    /// \brief
    /// Gets the geometry value of the specified property as a byte array in 
    /// FGF format. Because no conversion is performed, the property must be
    /// of Geometric type; otherwise, an exception is thrown.
    /// 
    /// \param propertyName 
    /// Input the property name.
    /// 
    /// \return
    /// Returns the byte array in FGF format.
    /// 
    FDO_API virtual FdoByteArray* GetGeometry(FdoString* propertyName) = 0;

    /// \brief
    /// Gets the raster object of the specified property.
    /// Because no conversion is performed, the property must be
    /// of Raster type; otherwise, an exception is thrown.
    /// 
    /// \param propertyName 
    /// Input the property name.
    /// 
    /// \return
    /// Returns the raster object.
    /// 
    FDO_API virtual FdoIRaster* GetRaster(FdoString* propertyName) = 0;

    /// \brief
    /// Gets the Boolean value of the property specified at the index position. 
    /// No conversion is performed, thus the property must be FdoDataType_Boolean 
    /// or an exception is thrown.
    /// 
    /// \param index 
    /// Input the index of the property.
    /// 
    /// \return
    /// Returns the Boolean value.
    /// 
    FDO_API virtual FdoBoolean GetBoolean(FdoInt32 index) = 0;

    /// \brief
    /// Gets the Byte value of the property specified at the index position. 
    /// No conversion is performed, thus the property must be FdoDataType_Byte 
    /// or an exception is thrown.
    /// 
    /// \param index 
    /// Input the index of the property.
    /// 
    /// \return
    /// Returns the byte value.
    /// 
    FDO_API virtual FdoByte GetByte(FdoInt32 index) = 0;

    /// \brief
    /// Gets the date and time value of the of the property specified at 
    /// the index position. No conversion is performed, thus the property 
    /// must be FdoDataType_DateTime or an exception is thrown.
    /// 
    /// \param index 
    /// Input the index of the property.
    /// 
    /// \return
    /// Returns the date and time value.
    /// 
    FDO_API virtual FdoDateTime GetDateTime(FdoInt32 index) = 0;

    /// \brief
    /// Gets the double-precision floating point value of the property specified at 
    /// the index position. No conversion is performed, thus the property must be 
    /// FdoDataType_Double or an exception is thrown.
    /// 
    /// \param index 
    /// Input the index of the property.
    /// 
    /// \return
    /// Returns the double floating point value
    /// 
    FDO_API virtual FdoDouble GetDouble(FdoInt32 index) = 0;

    /// \brief
    /// Gets the 16-bit integer value of the property specified at 
    /// the index position. No conversion is performed, thus the 
    /// property must be FdoDataType_Int16 or an exception is thrown.
    /// 
    /// \param index 
    /// Input the index of the property.
    /// 
    /// \return
    /// Returns the FdoInt16 value.
    /// 
    FDO_API virtual FdoInt16 GetInt16(FdoInt32 index) = 0;

    /// \brief
    /// Gets the 32-bit integer value of the property specified at 
    /// the index position. No conversion is performed, thus the 
    /// property must be FdoDataType_Int32 or an exception is thrown.
    /// 
    /// \param index 
    /// Input the index of the property.
    /// 
    /// \return
    /// Returns the FdoInt32 value
    /// 
    FDO_API virtual FdoInt32 GetInt32(FdoInt32 index) = 0;

    /// \brief
    /// Gets the 64-bit integer value of the property specified at 
    /// the index position. No conversion is performed, thus the 
    /// property must be FdoDataType_Int64 or an exception is thrown.
    /// 
    /// \param index 
    /// Input the index of the property.
    /// 
    /// \return
    /// Returns the FdoInt64 value.
    /// 
    FDO_API virtual FdoInt64 GetInt64(FdoInt32 index) = 0;

    /// \brief
    /// Gets the Single floating point value of the property specified at 
    /// the index position. No conversion is performed, thus the property 
    /// must be FdoDataType_Single or an exception is thrown.
    /// 
    /// \param index 
    /// Input the index of the property.
    /// 
    /// \return
    /// Returns the single value
    /// 
    FDO_API virtual FdoFloat GetSingle(FdoInt32 index) = 0;

    /// \brief
    /// Gets the string value of the property specified at the index
    /// position. No conversion is performed, thus the property must
    /// be FdoDataType_String or an exception is thrown.
    /// 
    /// \param index 
    /// Input the index of the property.
    /// 
    /// \return
    /// Returns the string value
    /// 
    FDO_API virtual FdoString* GetString(FdoInt32 index) = 0;

    /// \brief
    /// Gets a LOBValue pointer to the property specified at the index
    /// position. The LOB is fully read in and data available.
    /// Because no conversion is performed, the property must be 
    /// FdoDataType_BLOB or FdoDataType_CLOB etc. (a LOB type)
    /// 
    /// \param index 
    /// Input the index of the property.
    /// 
    /// \return
    /// Returns the pointer to LOBValue
    /// 
    FDO_API virtual FdoLOBValue* GetLOB(FdoInt32 index) = 0;

    /// \brief
    /// Gets a pointer to the specified LOB property, specified at the index
    /// position. The reference is returned as an FdoBLOBStreamReader or an 
    /// FdoCLOBStreamReader, to allow reading in blocks of data. Because 
    /// no conversion is performed, the property must be FdoDataType_BLOB 
    /// or FdoDataType_CLOB etc. (a LOB type) Cast the FdoIStreamReader 
    /// to the appropriate LOB Stream Reader.
    /// 
    /// \param index 
    /// Input the index of the property.
    /// 
    /// \return
    /// Returns a pointer to a LOB stream reader
    /// 
    FDO_API virtual FdoIStreamReader* GetLOBStreamReader(FdoInt32 index) = 0;

    /// \brief
    /// Returns true if the value of the property at the specified 
    /// index is null.
    /// 
    /// \param index 
    /// Input the index of the property.
    /// 
    /// \return
    /// Returns true if the value is null.
    /// 
    FDO_API virtual FdoBoolean IsNull(FdoInt32 index) = 0;

    /// \brief
    /// Gets the geometry value of the property, at the specified index, as  
    /// a byte array in FGF format. Because no conversion is performed, the 
    /// property must be of Geometric type; otherwise, an exception is thrown.
    /// 
    /// \param index 
    /// Input the index of the property.
    /// 
    /// \return
    /// Returns the byte array in FGF format.
    /// 
    FDO_API virtual FdoByteArray* GetGeometry(FdoInt32 index) = 0;

    /// \brief
    /// Gets the raster object of the property at the specified index.
    /// Because no conversion is performed, the property must be
    /// of Raster type; otherwise, an exception is thrown.
    /// 
    /// \param index 
    /// Input the index of the property.
    /// 
    /// \return
    /// Returns the raster object.
    /// 
    FDO_API virtual FdoIRaster* GetRaster(FdoInt32 index) = 0;

    /// \brief
    /// Advances the reader to the next item and returns true if there is
    /// another object to read or false if reading is complete. The default
    /// position of the reader is prior to the first item. Thus you must
    /// call ReadNext to begin accessing any data.
    /// 
    /// \return
    /// Returns true if there is a next item.
    /// 
    FDO_API virtual bool ReadNext() = 0;

    /// \brief
    /// Closes the FdoIFeatureReader object, freeing any resources it may be holding.
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDO_API virtual void Close() = 0;
};
#endif


