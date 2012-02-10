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
#ifndef FDODEFAULTSQLDATAREADER_H
#define FDODEFAULTSQLDATAREADER_H

class FdoDefaultSqlDataReader : public FdoISQLDataReader
{
public:
    /// \brief
    /// Gets the data type of the column with the specified name.
    /// 
    /// \param columnName 
    /// Input the column name.
    /// 
    /// \return
    /// Returns the type of the column.
    /// 
    virtual FdoDataType GetColumnType(FdoString* columnName) = 0;

    /// \brief
    /// Gets the FDO property type of the column with the specified name. This is used
    ///  to indicate if a given column is a geometric property or a data property. If the column is
    ///  a FdoPropertyType_DataProperty, then GetColumnType can be used to find the data type of the column.
    /// 
    /// \param columnName 
    /// Input the column name.
    /// 
    /// \return
    /// Returns the FDO property type of the column.
    /// 
    virtual FdoPropertyType GetPropertyType(FdoString* columnName) = 0;

    /// \brief
    /// Gets the Boolean value of the specified column. No conversion is
    /// performed, thus the column must be FdoDataType_Boolean or an
    /// exception is thrown.
    /// 
    /// \param columnName 
    /// Input the column name.
    /// 
    /// \return
    /// Returns the Boolean value
    /// 
    virtual bool GetBoolean(FdoString* columnName) = 0;

    /// \brief
    /// Gets the byte value of the specified column. No conversion is
    /// performed, thus the column must be FdoDataType_Byte or an
    /// exception is thrown.
    /// 
    /// \param columnName 
    /// Input the column name.
    /// 
    /// \return
    /// Returns the byte value.
    /// 
    virtual FdoByte GetByte(FdoString* columnName) = 0;

    /// \brief
    /// Gets the date time value of the specified column. No conversion
    /// is performed, thus the column must be FdoDataType_DateTime or
    /// an exception is thrown.
    /// 
    /// \param columnName 
    /// Input the column name.
    /// 
    /// \return
    /// Returns the date and time value.
    /// 
    virtual FdoDateTime GetDateTime(FdoString* columnName) = 0;

    /// \brief
    /// Gets the double-precision floating point value of the specified column.
    /// No conversion is performed, thus the column must be of type
    /// Double or an exception is thrown.
    /// 
    /// \param columnName 
    /// Input the column name.
    /// 
    /// \return
    /// Returns the double value.
    /// 
    virtual double GetDouble(FdoString* columnName) = 0;

    /// \brief
    /// Gets the signed 16-bit integer value of the specified column. No conversion is
    /// performed, thus the column must be FdoDataType_Int16 or an
    /// exception is thrown.
    /// 
    /// \param columnName 
    /// Input the column name.
    /// 
    /// \return
    /// Returns the FdoInt16 value.
    /// 
    virtual FdoInt16 GetInt16(FdoString* columnName) = 0;

    /// \brief
    /// Gets the signed 32-bit integer value of the specified column. No conversion is
    /// performed, thus the column must be FdoDataType_Int32 or an
    /// exception is thrown.
    /// 
    /// \param columnName 
    /// Input the column name.
    /// 
    /// \return
    /// Returns the FdoInt32 value.
    /// 
    virtual FdoInt32 GetInt32(FdoString* columnName) = 0;

    /// \brief
    /// Gets the signed 64-bit integer value of the specified column. No conversion
    /// is performed, thus the column must be FdoDataType_Int64 or an
    /// exception is thrown.
    /// 
    /// \param columnName 
    /// Input the column name.
    /// 
    /// \return
    /// Returns the FdoInt64 value.
    /// 
    virtual FdoInt64 GetInt64(FdoString* columnName) = 0;

    /// \brief
    /// Gets the single-precision floating point value of the specified column.
    /// No conversion is performed, thus the column must be FdoDataType_Single
    /// or an exception is thrown.
    /// 
    /// \param columnName 
    /// Input the column name.
    /// 
    /// \return
    /// Returns the single value
    /// 
    virtual float GetSingle(FdoString* columnName) = 0;

    /// \brief
    /// Gets the string value of the specified column. No conversion is
    /// performed, thus the column must be FdoDataType_String or an
    /// exception is thrown.
    /// 
    /// \param columnName 
    /// Input the column name.
    /// 
    /// \return
    /// Returns the string value.
    /// 
    virtual FdoString* GetString(FdoString* columnName) = 0;

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
    /// Returns true if the value of the specified column is null.
    /// 
    /// \param columnName 
    /// Input the column name.
    /// 
    /// \return
    /// Returns true if the value is null.
    /// 
    virtual bool IsNull(FdoString* columnName) = 0;

    /// \brief
    /// Gets the geometry value of the specified column as a byte array
    /// in FGF format. No conversion is performed, thus the column
    /// must be of Geometric type or an exception is thrown.
    /// 
    /// \param columnName 
    /// Input the column name.
    /// 
    /// \return
    /// Returns the FGF byte array value.
    /// 
    virtual FdoByteArray* GetGeometry(FdoString* columnName) = 0;


    /// \brief
    /// Gets the data type of the column at the specified index.
    /// 
    /// \param index 
    /// Input the position of the column.   
    /// 
    /// \return
    /// Returns the type of the column.
    /// 
    virtual FdoDataType GetColumnType(FdoInt32 index);

    /// \brief
    /// Gets the FDO property type of the column at the specified index. This is used
    /// to indicate if a given column is a geometric property or a data property. If the column is
    /// a FdoPropertyType_DataProperty, then GetColumnType can be used to find the data type of the column.
    /// 
    /// \param index 
    /// Input the position of the column.   
    /// 
    /// \return
    /// Returns the FDO property type of the column.
    /// 
    virtual FdoPropertyType GetPropertyType(FdoInt32 index);

    /// \brief
    /// Gets the Boolean value of the specified column. No conversion is
    /// performed, thus the column must be FdoDataType_Boolean or an
    /// exception is thrown.
    /// 
    /// \param index 
    /// Input the position of the column.   
    /// 
    /// \return
    /// Returns the Boolean value
    /// 
    virtual FdoBoolean GetBoolean(FdoInt32 index);

    /// \brief
    /// Gets the byte value of the specified column. No conversion is
    /// performed, thus the column must be FdoDataType_Byte or an
    /// exception is thrown.
    /// 
    /// \param index 
    /// Input the position of the column.   
    /// 
    /// \return
    /// Returns the byte value.
    /// 
    virtual FdoByte GetByte(FdoInt32 index);

    /// \brief
    /// Gets the date time value of the specified column. No conversion
    /// is performed, thus the column must be FdoDataType_DateTime or
    /// an exception is thrown.
    /// 
    /// \param index 
    /// Input the position of the column.   
    /// 
    /// \return
    /// Returns the date and time value.
    /// 
    virtual FdoDateTime GetDateTime(FdoInt32 index);

    /// \brief
    /// Gets the double-precision floating point value of the specified column.
    /// No conversion is performed, thus the column must be of type
    /// Double or an exception is thrown.
    /// 
    /// \param index 
    /// Input the position of the column.   
    /// 
    /// \return
    /// Returns the double value.
    /// 
    virtual FdoDouble GetDouble(FdoInt32 index);

    /// \brief
    /// Gets the signed 16-bit integer value of the specified column. No conversion is
    /// performed, thus the column must be FdoDataType_Int16 or an
    /// exception is thrown.
    /// 
    /// \param index 
    /// Input the position of the column.   
    /// 
    /// \return
    /// Returns the FdoInt16 value.
    /// 
    virtual FdoInt16 GetInt16(FdoInt32 index);

    /// \brief
    /// Gets the signed 32-bit integer value of the specified column. No conversion is
    /// performed, thus the column must be FdoDataType_Int32 or an
    /// exception is thrown.
    /// 
    /// \param index 
    /// Input the position of the column.   
    /// 
    /// \return
    /// Returns the FdoInt32 value.
    /// 
    virtual FdoInt32 GetInt32(FdoInt32 index);

    /// \brief
    /// Gets the signed 64-bit integer value of the specified column. No conversion
    /// is performed, thus the column must be FdoDataType_Int64 or an
    /// exception is thrown.
    /// 
    /// \param index 
    /// Input the position of the column.   
    /// 
    /// \return
    /// Returns the FdoInt64 value.
    /// 
    virtual FdoInt64 GetInt64(FdoInt32 index);

    /// \brief
    /// Gets the single-precision floating point value of the specified column.
    /// No conversion is performed, thus the column must be FdoDataType_Single
    /// or an exception is thrown.
    /// 
    /// \param index 
    /// Input the position of the column.   
    /// 
    /// \return
    /// Returns the single value
    /// 
    virtual FdoFloat GetSingle(FdoInt32 index);

    /// \brief
    /// Gets the string value of the specified column. No conversion is
    /// performed, thus the column must be FdoDataType_String or an
    /// exception is thrown.
    /// 
    /// \param index 
    /// Input the position of the column.   
    /// 
    /// \return
    /// Returns the string value.
    /// 
    virtual FdoString* GetString(FdoInt32 index);

    /// \brief
    /// Gets a LOBValue reference. The LOB is fully read in and data available.
    /// Because no conversion is performed, the property must be FdoDataType_BLOB or
    /// FdoDataType_CLOB etc. (a LOB type)
    /// 
    /// \param index 
    /// Input the position of the column.   
    /// 
    /// \return
    /// Returns the reference to LOBValue
    /// 
    virtual FdoLOBValue* GetLOB(FdoInt32 index);

    /// \brief
    /// Gets a reference of the specified LOB property as a FdoBLOBStreamReader or
    /// FdoCLOBStreamReader etc. to allow reading in blocks of data. Because 
    /// no conversion is performed, the property must be FdoDataType_BLOB 
    /// or FdoDataType_CLOB etc. (a LOB type) Cast the FdoIStreamReader 
    /// to the appropiate LOB Stream Reader.
    /// 
    /// \param index 
    /// Input the position of the column.   
    /// 
    /// \return
    /// Returns a reference to a LOB stream reader
    /// 
    virtual FdoIStreamReader* GetLOBStreamReader(FdoInt32 index);

    /// \brief
    /// Returns true if the value of the specified column is null.
    /// 
    /// \param index 
    /// Input the position of the column.   
    /// 
    /// \return
    /// Returns true if the value is null.
    /// 
    virtual FdoBoolean IsNull(FdoInt32 index);

    /// \brief
    /// Gets the geometry value of the specified column as a byte array
    /// in FGF format. No conversion is performed, thus the column
    /// must be of Geometric type or an exception is thrown.
    /// 
    /// \param index 
    /// Input the position of the column.   
    /// 
    /// \return
    /// Returns the FGF byte array value.
    /// 
    virtual FdoByteArray* GetGeometry(FdoInt32 index);

protected:

    FdoDefaultSqlDataReader();
    virtual ~FdoDefaultSqlDataReader();
};

#endif // FDODEFAULTSQLDATAREADER_H


