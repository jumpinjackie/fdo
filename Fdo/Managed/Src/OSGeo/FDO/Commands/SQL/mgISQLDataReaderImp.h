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

#include "FDO\Commands\SQL\mgISQLDataReader.h"
#include "FDO\Schema\mgDataType.h"
#include "FDO\Schema\mgPropertyType.h"

class FdoISQLDataReader;

BEGIN_NAMESPACE_OSGEO_FDO_EXPRESSION
ref class LOBValue;
END_NAMESPACE_OSGEO_FDO_EXPRESSION

BEGIN_NAMESPACE_OSGEO_COMMON
interface class IStreamReader;
END_NAMESPACE_OSGEO_COMMON

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_SQL

/// \ingroup (OSGeoFDOCommandsSQL)
/// \brief
/// The ISQLDataReaderImp class is a concrete implementation of ISQLDataReader.
/// The ISQLDataReader interface provides a forward-only, read-only
/// iterator for reading relational table data. A reference to an
/// ISQLDataReader is returned from the SQLCommands ExecuteReader method.
/// The initial position of the ISQLDataReader interface is prior to the first item.
/// Thus, you must call ReadNext to begin accessing any data.
private ref class ISQLDataReaderImp : public NAMESPACE_OSGEO_RUNTIME::Disposable, 
                                       public NAMESPACE_OSGEO_FDO_COMMANDS_SQL::ISQLDataReader
{
public:
    /// \brief
    /// Gets the number of columns in the result set.
    /// 
    /// \return
    /// Returns the number of columns.
    /// 
	virtual System::Int32 GetColumnCount();

    /// \brief
    /// Gets the name of the column at the given ordinal position.
    /// 
    /// \param index 
    /// Input the position of the column.
    /// 
    /// \return
    /// Returns the column name
    /// 
	virtual System::String^ GetColumnName(System::Int32 index);

    /// \brief
    /// Gets the index of the column with the specified column name.
    /// 
    /// \param name 
    /// Input the name of the column.
    /// 
    /// \return
    /// Returns the column index
    /// 
    virtual System::Int32 GetColumnIndex(System::String^ name);

    /// \brief
    /// Gets the data type of the column with the specified name.
    /// 
    /// \param name 
    /// Input the column name.
    /// 
    /// \return
    /// Returns the type of the column.
    /// 
	virtual NAMESPACE_OSGEO_FDO_SCHEMA::DataType GetColumnType(System::String^ name);

    /// \brief
    /// Gets  property type of the column with the specified name. This is used
    /// to indicate if a given column is a geometric property or a data property. If the column is
    /// a PropertyType_DataProperty, then GetColumnType can be used to find the data type of the column.
    /// 
    /// \param name 
    /// Input the column name.
    /// 
    /// \return
    /// Returns  property type of the column.
    /// 
	virtual NAMESPACE_OSGEO_FDO_SCHEMA::PropertyType GetPropertyType(System::String^ name);

    /// \brief
    /// Gets the Boolean value of the specified column. No conversion is
    /// performed, thus the column must be DataType_Boolean or an
    /// exception is thrown.
    /// 
    /// \param name 
    /// Input the column name.
    /// 
    /// \return
    /// Returns the Boolean value
    /// 
	virtual System::Boolean GetBoolean(System::String^ name);

    /// \brief
    /// Gets the byte value of the specified column. No conversion is
    /// performed, thus the column must be DataType_Byte or an
    /// exception is thrown.
    /// 
    /// \param name 
    /// Input the column name.
    /// 
    /// \return
    /// Returns the byte value.
    /// 
	virtual System::Byte GetByte(System::String^ name);

    /// \brief
    /// Gets the date time value of the specified column. No conversion
    /// is performed, thus the column must be DataType_DateTime or
    /// an exception is thrown.
    /// 
    /// \param name 
    /// Input the column name.
    /// 
    /// \return
    /// Returns the date and time value.
    /// 
	virtual System::DateTime GetDateTime(System::String^ name);

    /// \brief
    /// Gets the double-precision floating point value of the specified column.
    /// No conversion is performed, thus the column must be of type
    /// Double or an exception is thrown.
    /// 
    /// \param name 
    /// Input the column name.
    /// 
    /// \return
    /// Returns the double value.
    /// 
	virtual System::Double GetDouble(System::String^ name);

    /// \brief
    /// Gets the signed 16-bit integer value of the specified column. No conversion is
    /// performed, thus the column must be DataType_Int16 or an
    /// exception is thrown.
    /// 
    /// \param name 
    /// Input the column name.
    /// 
    /// \return
    /// Returns the Int16 value.
    /// 
	virtual System::Int16 GetInt16(System::String^ name);

    /// \brief
    /// Gets the signed 32-bit integer value of the specified column. No conversion is
    /// performed, thus the column must be DataType_Int32 or an
    /// exception is thrown.
    /// 
    /// \param name 
    /// Input the column name.
    /// 
    /// \return
    /// Returns the Int32 value.
    /// 
	virtual System::Int32 GetInt32(System::String^ name);

    /// \brief
    /// Gets the signed 64-bit integer value of the specified column. No conversion
    /// is performed, thus the column must be DataType_Int64 or an
    /// exception is thrown.
    /// 
    /// \param name 
    /// Input the column name.
    /// 
    /// \return
    /// Returns the Int64 value.
    /// 
	virtual System::Int64 GetInt64(System::String^ name);

    /// \brief
    /// Gets the single-precision floating point value of the specified column.
    /// No conversion is performed, thus the column must be DataType_Single
    /// or an exception is thrown.
    /// 
    /// \param name 
    /// Input the column name.
    /// 
    /// \return
    /// Returns the single value
    /// 
	virtual System::Single GetSingle(System::String^ name);

    /// \brief
    /// Gets the string value of the specified column. No conversion is
    /// performed, thus the column must be DataType_String or an
    /// exception is thrown.
    /// 
    /// \param name 
    /// Input the column name.
    /// 
    /// \return
    /// Returns the string value.
    /// 
	virtual System::String^ GetString(System::String^ name);

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
	virtual NAMESPACE_OSGEO_FDO_EXPRESSION::LOBValue^ GetLOB(System::String^ name);

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
	virtual NAMESPACE_OSGEO_COMMON::IStreamReader^ GetLOBStreamReader(System::String^ name);

    /// \brief
    /// Returns true if the value of the specified column is null.
    /// 
    /// \param name 
    /// Input the column name.
    /// 
    /// \return
    /// Returns true if the value is null.
    /// 
	virtual System::Boolean IsNull(System::String^ name);

    /// \brief
    /// Gets the geometry value of the specified column as a byte array
    /// in FGF format. No conversion is performed, thus the column
    /// must be of Geometric type or an exception is thrown.
    /// 
    /// \param name 
    /// Input the column name.
    /// 
    /// \return
    /// Returns the FGF byte array value.
    /// 
	virtual array<System::Byte>^ GetGeometry(System::String^ name);

    /// \brief
    /// Gets the data type of the column at the specified index.
    /// 
    /// \param index 
    /// Input the position of the column.   
    /// 
    /// \return
    /// Returns the type of the column.
    /// 
    virtual NAMESPACE_OSGEO_FDO_SCHEMA::DataType GetColumnType(System::Int32 index);

    /// \brief
    /// Gets  property type of the column at the specified index. This is used
    /// to indicate if a given column is a geometric property or a data property. If the column is
    /// a PropertyType_DataProperty, then GetColumnType can be used to find the data type of the column.
    /// 
    /// \param index 
    /// Input the position of the column.   
    /// 
    /// \return
    /// Returns  property type of the column.
    /// 
    virtual NAMESPACE_OSGEO_FDO_SCHEMA::PropertyType GetPropertyType(System::Int32 index);

    /// \brief
    /// Gets the Boolean value at the specified index. No conversion is
    /// performed, thus the column must be DataType_Boolean or an
    /// exception is thrown.
    /// 
    /// \param index 
    /// Input the position of the column.   
    /// 
    /// \return
    /// Returns the Boolean value
    /// 
    virtual System::Boolean GetBoolean(System::Int32 index);

    /// \brief
    /// Gets the byte value of the column at the specified index. No conversion is
    /// performed, thus the column must be DataType_Byte or an
    /// exception is thrown.
    /// 
    /// \param index 
    /// Input the position of the column.   
    /// 
    /// \return
    /// Returns the byte value.
    /// 
    virtual System::Byte GetByte(System::Int32 index);

    /// \brief
    /// Gets the date time value of the column at the specified index. No conversion
    /// is performed, thus the column must be DataType_DateTime or
    /// an exception is thrown.
    /// 
    /// \param index 
    /// Input the position of the column.   
    /// 
    /// \return
    /// Returns the date and time value.
    /// 
    virtual System::DateTime GetDateTime(System::Int32 index);

    /// \brief
    /// Gets the double-precision floating point value of the column at the specified index.
    /// No conversion is performed, thus the column must be of type
    /// Double or an exception is thrown.
    /// 
    /// \param index 
    /// Input the position of the column.   
    /// 
    /// \return
    /// Returns the double value.
    /// 
    virtual System::Double GetDouble(System::Int32 index);

    /// \brief
    /// Gets the signed 16-bit integer value of the column at the specified index. No conversion is
    /// performed, thus the column must be DataType_Int16 or an
    /// exception is thrown.
    /// 
    /// \param index 
    /// Input the position of the column.   
    /// 
    /// \return
    /// Returns the Int16 value.
    /// 
    virtual System::Int16 GetInt16(System::Int32 index);

    /// \brief
    /// Gets the signed 32-bit integer value of the column at the specified index. No conversion is
    /// performed, thus the column must be DataType_Int32 or an
    /// exception is thrown.
    /// 
    /// \param index 
    /// Input the position of the column.   
    /// 
    /// \return
    /// Returns the Int32 value.
    /// 
    virtual System::Int32 GetInt32(System::Int32 index);

    /// \brief
    /// Gets the signed 64-bit integer value of the column at the specified index. No conversion
    /// is performed, thus the column must be DataType_Int64 or an
    /// exception is thrown.
    /// 
    /// \param index 
    /// Input the position of the column.   
    /// 
    /// \return
    /// Returns the Int64 value.
    /// 
    virtual System::Int64 GetInt64(System::Int32 index);

    /// \brief
    /// Gets the single-precision floating point value of the column at the specified index.
    /// No conversion is performed, thus the column must be DataType_Single
    /// or an exception is thrown.
    /// 
    /// \param index 
    /// Input the position of the column.   
    /// 
    /// \return
    /// Returns the single value
    /// 
    virtual System::Single GetSingle(System::Int32 index);

    /// \brief
    /// Gets the string value of the column at the specified index. No conversion is
    /// performed, thus the column must be DataType_String or an
    /// exception is thrown.
    /// 
    /// \param index 
    /// Input the position of the column.   
    /// 
    /// \return
    /// Returns the string value.
    /// 
    virtual System::String^ GetString(System::Int32 index);

    /// \brief
    /// Gets a LOBValue reference. The LOB is fully read in and data available.
    /// Because no conversion is performed, the property must be DataType_BLOB or
    /// DataType_CLOB etc. (a LOB type)
    /// 
    /// \param index 
    /// Input the position of the column.   
    /// 
    /// \return
    /// Returns the reference to LOBValue
    /// 
    virtual NAMESPACE_OSGEO_FDO_EXPRESSION::LOBValue^ GetLOB(System::Int32 index);

    /// \brief
    /// Gets a reference of the specified LOB property as a BLOBStreamReader or
    /// CLOBStreamReader etc. to allow reading in blocks of data.
    /// Because no conversion is performed, the property must be DataType_BLOB 
    /// or DataType_CLOB etc. (a LOB type)
    /// Cast the IStreamReader to the appropiate LOB Stream Reader.
    /// 
    /// \param index 
    /// Input the position of the column.   
    /// 
    /// \return
    /// Returns a reference to a LOB stream reader
    /// 
    virtual NAMESPACE_OSGEO_COMMON::IStreamReader^ GetLOBStreamReader(System::Int32 index);

    /// \brief
    /// Returns true if the value of the column at the specified index is null.
    /// 
    /// \param index 
    /// Input the position of the column.   
    /// 
    /// \return
    /// Returns true if the value is null.
    /// 
    virtual System::Boolean IsNull(System::Int32 index);

    /// \brief
    /// Gets the geometry value of the column at the specified index as a byte array
    /// in FGF format. No conversion is performed, thus the column
    /// must be of Geometric type or an exception is thrown.
    /// 
    /// \param index 
    /// Input the position of the column.   
    /// 
    /// \return
    /// Returns the FGF byte array value.
    /// 
    virtual array<System::Byte>^ GetGeometry(System::Int32 index);

    /// \brief
    /// Advances the reader to the next item. The default position of the
    /// reader is prior to the first item. Thus, you must call ReadNext
    /// to begin accessing any data.
    /// 
    /// \return
    /// Returns true if there is a next item.
    /// 
	virtual System::Boolean ReadNext();

    /// \brief
    /// Closes the ISQLDataReader object, freeing any resources it may be holding.
    /// 
    /// \return
    /// Returns nothing
    /// 
	virtual System::Void Close();

internal:
	ISQLDataReaderImp(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_RUNTIME::Disposable(unmanaged, autoDelete)
	{

	}

	inline FdoISQLDataReader* GetImpObj();
public:
    virtual IntPtr GetDisposableObject() override;
};

END_NAMESPACE_OSGEO_FDO_COMMANDS_SQL


