// 
//  Copyright (C) 2004-2006  Autodesk, Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
#ifndef FDODEFAULTDATAREADER_H
#define FDODEFAULTDATAREADER_H

class FdoDefaultDataReader : public FdoIDataReader
{
public:
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
    virtual bool GetBoolean(FdoString* propertyName) = 0;

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
    virtual FdoByte GetByte(FdoString* propertyName) = 0;

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
    virtual FdoDateTime GetDateTime(FdoString* propertyName) = 0;

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
    virtual double GetDouble(FdoString* propertyName) = 0;

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
    virtual FdoInt16 GetInt16(FdoString* propertyName) = 0;

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
    virtual FdoInt32 GetInt32(FdoString* propertyName) = 0;

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
    virtual FdoInt64 GetInt64(FdoString* propertyName) = 0;

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
    virtual float GetSingle(FdoString* propertyName) = 0;

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
    virtual FdoString* GetString(FdoString* propertyName) = 0;

    /// \brief
    /// Gets a LOBValue reference. The LOB is fully read in and data available.
    /// Because no conversion is performed, the property must be FdoDataType_BLOB or
    /// FdoDataType_CLOB etc. (a LOB type)
    /// 
    /// \param propertyName 
    /// Input the property name.
    /// 
    /// \return
    /// Returns the reference to LOBValue
    /// 
    virtual FdoLOBValue* GetLOB(FdoString* propertyName) = 0;

    /// \brief
    /// Gets a reference of the specified LOB property as a FdoBLOBStreamReader or
    /// FdoCLOBStreamReader etc. to allow reading in blocks of data.
    /// Because no conversion is performed, the property must be FdoDataType_BLOB 
    /// or FdoDataType_CLOB etc. (a LOB type)
    /// Cast the FdoIStreamReader to the appropiate LOB Stream Reader.
    /// 
    /// \param propertyName 
    /// Input the property name.
    /// 
    /// \return
    /// Returns a reference to a LOB stream reader
    /// 
    virtual FdoIStreamReader* GetLOBStreamReader(const wchar_t* propertyName ) = 0;

    /// \brief
    /// Returns true if the value of the specified property is null.
    /// 
    /// \param propertyName 
    /// Input the property name.
    /// 
    /// \return
    /// Returns true if the value is null.
    /// 
    virtual bool IsNull(FdoString* propertyName) = 0;

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
    virtual FdoByteArray* GetGeometry(FdoString* propertyName) = 0;


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
    virtual FdoIRaster* GetRaster(FdoString* propertyName) = 0;

    /// \brief
    /// Gets the data type of the property with the specified name.
    /// 
    /// \param propertyName 
    /// Input the property name.
    /// 
    /// \return
    /// Returns the type of the property.
    /// 
    virtual FdoDataType GetDataType(FdoString* propertyName) = 0;

    /// \brief
    /// Gets the FDO property type of a given property. This is used
    ///  to indicate if a given property is a geometric property or a data property. If the property is
    ///  a FdoPropertyType_DataProperty, then GetDataType can be used to to find the data type of the property.
    /// 
    /// \param propertyName 
    /// Input the property name.
    /// 
    /// \return
    /// Returns the FDO property type.
    /// 
    virtual FdoPropertyType GetPropertyType(FdoString* propertyName) = 0;


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
    virtual FdoBoolean GetBoolean(FdoInt32 index);

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
    virtual FdoByte GetByte(FdoInt32 index);

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
    virtual FdoDateTime GetDateTime(FdoInt32 index);

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
    virtual FdoDouble GetDouble(FdoInt32 index);

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
    virtual FdoInt16 GetInt16(FdoInt32 index);

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
    virtual FdoInt32 GetInt32(FdoInt32 index);

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
    virtual FdoInt64 GetInt64(FdoInt32 index);

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
    virtual FdoFloat GetSingle(FdoInt32 index);

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
    virtual FdoString* GetString(FdoInt32 index);

    /// \brief
    /// Gets a LOBValue reference to the property specified at the index
    /// position. The LOB is fully read in and data available.
    /// Because no conversion is performed, the property must be 
    /// FdoDataType_BLOB or FdoDataType_CLOB etc. (a LOB type)
    /// 
    /// \param index 
    /// Input the index of the property.
    /// 
    /// \return
    /// Returns the reference to LOBValue
    /// 
    virtual FdoLOBValue* GetLOB(FdoInt32 index);

    /// \brief
    /// Gets a reference to the specified LOB property, specified at the index
    /// position. The reference is returned as an FdoBLOBStreamReader or an 
    /// FdoCLOBStreamReader, to allow reading in blocks of data. Because 
    /// no conversion is performed, the property must be FdoDataType_BLOB 
    /// or FdoDataType_CLOB etc. (a LOB type) Cast the FdoIStreamReader 
    /// to the appropiate LOB Stream Reader.
    /// 
    /// \param index 
    /// Input the index of the property.
    /// 
    /// \return
    /// Returns a reference to a LOB stream reader
    /// 
    virtual FdoIStreamReader* GetLOBStreamReader(FdoInt32 index);

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
    virtual FdoBoolean IsNull(FdoInt32 index);

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
    virtual FdoByteArray* GetGeometry(FdoInt32 index);

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
    virtual FdoIRaster* GetRaster(FdoInt32 index);

    /// \brief
    /// Gets the data type of the property at the specified index position.
    /// 
    /// \param index 
    /// Input the index of the property.
    /// 
    /// \return
    /// Returns the type of the property.
    /// 
    virtual FdoDataType GetDataType(FdoInt32 index);

    /// \brief
    /// Gets the FDO property type of the property at the given index. This is used
    /// to indicate if a given property is a geometric property or a data property. 
    /// If the property is a FdoPropertyType_DataProperty, then GetDataType 
    /// can be used to to find the data type of the property.
    /// 
    /// \param index 
    /// Input the index of the property.
    /// 
    /// \return
    /// Returns the FDO property type.
    /// 
    virtual FdoPropertyType GetPropertyType(FdoInt32 index);

protected:

    FdoDefaultDataReader();
    virtual ~FdoDefaultDataReader();

};

#endif // FDODEFAULTDATAREADER_H
