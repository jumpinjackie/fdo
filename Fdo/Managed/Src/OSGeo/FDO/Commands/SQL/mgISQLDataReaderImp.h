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
public __gc class LOBValue;
END_NAMESPACE_OSGEO_FDO_EXPRESSION

BEGIN_NAMESPACE_OSGEO_COMMON
public __gc __interface IStreamReader;
END_NAMESPACE_OSGEO_COMMON

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_SQL

/// \brief
/// The ISQLDataReaderImp class is a concrete implementation of ISQLDataReader.
/// The ISQLDataReader interface provides a forward-only, read-only
/// iterator for reading relational table data. A reference to an
/// ISQLDataReader is returned from the SQLCommands ExecuteReader method.
/// The initial position of the ISQLDataReader interface is prior to the first item.
/// Thus, you must call ReadNext to begin accessing any data.
private __gc class ISQLDataReaderImp : public NAMESPACE_OSGEO_RUNTIME::Disposable, 
                                       public NAMESPACE_OSGEO_FDO_COMMANDS_SQL::ISQLDataReader
{
public:
    /// \brief
    /// Gets the number of columns in the result set.
    /// 
    /// \return
    /// Returns the number of columns.
    /// 
	System::Int32 GetColumnCount();

    /// \brief
    /// Gets the name of the column at the given ordinal position.
    /// 
    /// \param index 
    /// Input the position of the column.
    /// 
    /// \return
    /// Returns the column name
    /// 
	System::String* GetColumnName(System::Int32 index);

    /// \brief
    /// Gets the data type of the column with the specified name.
    /// 
    /// \param name 
    /// Input the column name.
    /// 
    /// \return
    /// Returns the type of the column.
    /// 
	NAMESPACE_OSGEO_FDO_SCHEMA::DataType GetColumnType(System::String* name);

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
	NAMESPACE_OSGEO_FDO_SCHEMA::PropertyType GetPropertyType(System::String* name);

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
	System::Boolean GetBoolean(System::String* name);

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
	System::Byte GetByte(System::String* name);

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
	System::DateTime GetDateTime(System::String* name);

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
	System::Double GetDouble(System::String* name);

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
	System::Int16 GetInt16(System::String* name);

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
	System::Int32 GetInt32(System::String* name);

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
	System::Int64 GetInt64(System::String* name);

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
	System::Single GetSingle(System::String* name);

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
    /// Returns true if the value of the specified column is null.
    /// 
    /// \param name 
    /// Input the column name.
    /// 
    /// \return
    /// Returns true if the value is null.
    /// 
	System::Boolean IsNull(System::String* name);

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
	System::Byte GetGeometry(System::String* name) [];

    /// \brief
    /// Advances the reader to the next item. The default position of the
    /// reader is prior to the first item. Thus, you must call ReadNext
    /// to begin accessing any data.
    /// 
    /// \return
    /// Returns true if there is a next item.
    /// 
	System::Boolean ReadNext();

    /// \brief
    /// Closes the ISQLDataReader object, freeing any resources it may be holding.
    /// 
    /// \return
    /// Returns nothing
    /// 
	System::Void Close();

/// \cond DOXYGEN-IGNORE
protected:
	__sealed System::Void ReleaseUnmanagedObject();
/// \endcond

public private:
	ISQLDataReaderImp(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_RUNTIME::Disposable(unmanaged, autoDelete)
	{

	}

	inline FdoISQLDataReader* GetImpObj()
	{
		return static_cast<FdoISQLDataReader*>(__super::UnmanagedObject.ToPointer());
	}
};

END_NAMESPACE_OSGEO_FDO_COMMANDS_SQL


