/*
* Copyright (C) 2007  Haris Kurtagic
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
*/
#ifndef _c_KgInfSQLDataReader_h
#define _c_KgInfSQLDataReader_h
#include "FdoDefaultSqlDataReader.h"

class c_KgInfSQLDataReader : public FdoDefaultSqlDataReader
{
public:
  c_KgInfSQLDataReader(c_KgInfConnection * Connection
                        ,otl_stream* OtlStream,c_KgOtlStreamReader*StreamReader);
  
public:
  ~c_KgInfSQLDataReader(void);
  
  virtual void Dispose();
  
protected:
  c_KgInfConnection * m_Connection;  
  otl_stream* m_OtlStream;
  c_KgOtlStreamReader* m_OtlStreamReader;

  
  vector<otl_column_desc*> m_MetaData;
  
  FdoPtr<FdoStringCollection> m_SqlColumns;
  vector<int> m_SqlColIndex; // represents index of coumn in resultset. array corespond to m_SqlColumns
                             // I need this one beacuse some column would not go into 
                             // m_SqlColumns - because of not recognised type
  
  //c_SdoGeomToAGF m_SdoAgfConv;
  FdoStringP m_CachedString; // buffer to save last retrived string with GetString
  
  
  FdoStringP m_ColName;// buffer to save last retrived string with GetColumnNAme
  
public:
    /// \brief
    /// Gets the number of columns in the result set.
    /// 
    /// \return
    /// Returns the number of columns.
    /// 
    FDOKGINF_API virtual FdoInt32 GetColumnCount();

    /// \brief
    /// Gets the name of the column at the given ordinal position.
    /// 
    /// \param index 
    /// Input the position of the column.
    /// 
    /// \return
    /// Returns the column name
    /// 
    FDOKGINF_API virtual FdoString* GetColumnName(FdoInt32 index);

    /// \brief
    /// Gets the index of the column with the given column name.
    /// 
    /// \param columnName 
    /// Input the column name.
    /// 
    /// \return
    /// Returns the column index
    /// 
    FDOKGINF_API virtual FdoInt32 GetColumnIndex(FdoString* columnName);

    /// \brief
    /// Gets the data type of the column with the specified name.
    /// 
    /// \param ColumnName 
    /// Input the column name.
    /// 
    /// \return
    /// Returns the type of the column.
    /// 
    FDOKGINF_API virtual FdoDataType GetColumnType(FdoString* ColumnName);

    /// \brief
    /// Gets the FDO property type of the column with the specified name. This is used
    ///  to indicate if a given column is a geometric property or a data property. If the column is
    ///  a FdoPropertyType_DataProperty, then GetColumnType can be used to find the data type of the column.
    /// 
    /// \param propertyName 
    /// Input the column name.
    /// 
    /// \return
    /// Returns the FDO property type of the column.
    /// 
    FDOKGINF_API virtual FdoPropertyType GetPropertyType(FdoString* ColumnName);

    /// \brief
    /// Gets the Boolean value of the specified column. No conversion is
    /// performed, thus the column must be FdoDataType_Boolean or an
    /// exception is thrown.
    /// 
    /// \param ColumnName 
    /// Input the column name.
    /// 
    /// \return
    /// Returns the Boolean value
    /// 
    FDOKGINF_API virtual bool GetBoolean(FdoString* ColumnName);

    /// \brief
    /// Gets the byte value of the specified column. No conversion is
    /// performed, thus the column must be FdoDataType_Byte or an
    /// exception is thrown.
    /// 
    /// \param ColumnName 
    /// Input the column name.
    /// 
    /// \return
    /// Returns the byte value.
    /// 
    FDOKGINF_API virtual FdoByte GetByte(FdoString* ColumnName);

    /// \brief
    /// Gets the date time value of the specified column. No conversion
    /// is performed, thus the column must be FdoDataType_DateTime or
    /// an exception is thrown.
    /// 
    /// \param ColumnName 
    /// Input the column name.
    /// 
    /// \return
    /// Returns the date and time value.
    /// 
    FDOKGINF_API virtual FdoDateTime GetDateTime(FdoString* ColumnName);

    /// \brief
    /// Gets the double-precision floating point value of the specified column.
    /// No conversion is performed, thus the column must be of type
    /// Double or an exception is thrown.
    /// 
    /// \param ColumnName 
    /// Input the column name.
    /// 
    /// \return
    /// Returns the double value.
    /// 
    FDOKGINF_API virtual double GetDouble(FdoString* ColumnName);

    /// \brief
    /// Gets the signed 16-bit integer value of the specified column. No conversion is
    /// performed, thus the column must be FdoDataType_Int16 or an
    /// exception is thrown.
    /// 
    /// \param ColumnName 
    /// Input the column name.
    /// 
    /// \return
    /// Returns the FdoInt16 value.
    /// 
    FDOKGINF_API virtual FdoInt16 GetInt16(FdoString* ColumnName);

    /// \brief
    /// Gets the signed 32-bit integer value of the specified column. No conversion is
    /// performed, thus the column must be FdoDataType_Int32 or an
    /// exception is thrown.
    /// 
    /// \param ColumnName 
    /// Input the column name.
    /// 
    /// \return
    /// Returns the FdoInt32 value.
    /// 
    FDOKGINF_API virtual FdoInt32 GetInt32(FdoString* ColumnName);

    /// \brief
    /// Gets the signed 64-bit integer value of the specified column. No conversion
    /// is performed, thus the column must be FdoDataType_Int64 or an
    /// exception is thrown.
    /// 
    /// \param ColumnName 
    /// Input the column name.
    /// 
    /// \return
    /// Returns the FdoInt64 value.
    /// 
    FDOKGINF_API virtual FdoInt64 GetInt64(FdoString* ColumnName);

    /// \brief
    /// Gets the single-precision floating point value of the specified column.
    /// No conversion is performed, thus the column must be FdoDataType_Single
    /// or an exception is thrown.
    /// 
    /// \param ColumnName 
    /// Input the column name.
    /// 
    /// \return
    /// Returns the single value
    /// 
    FDOKGINF_API virtual float GetSingle(FdoString* ColumnName);

    /// \brief
    /// Gets the string value of the specified column. No conversion is
    /// performed, thus the column must be FdoDataType_String or an
    /// exception is thrown.
    /// 
    /// \param ColumnName 
    /// Input the column name.
    /// 
    /// \return
    /// Returns the string value.
    /// 
    FDOKGINF_API virtual FdoString* GetString(FdoString* ColumnName);

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
    FDOKGINF_API virtual FdoLOBValue* GetLOB(FdoString* propertyName);

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
    FDOKGINF_API virtual FdoIStreamReader* GetLOBStreamReader(const wchar_t* propertyName);

    /// \brief
    /// Returns true if the value of the specified column is null.
    /// 
    /// \param ColumnName 
    /// Input the column name.
    /// 
    /// \return
    /// Returns true if the value is null.
    /// 
    FDOKGINF_API virtual bool IsNull(FdoString* ColumnName);

    /// \brief
    /// Gets the geometry value of the specified column as a byte array
    /// in AGF format. No conversion is performed, thus the column
    /// must be of Geometric type or an exception is thrown.
    /// 
    /// \param ColumnName 
    /// Input the column name.
    /// 
    /// \return
    /// Returns the AGF byte array value.
    /// 
    FDOKGINF_API virtual FdoByteArray* GetGeometry(FdoString* ColumnName);
    
    

    /// \brief
    /// Advances the reader to the next item. The default position of the
    /// reader is prior to the first item. Thus, you must call ReadNext
    /// to begin accessing any data.
    /// 
    /// \return
    /// Returns true if there is a next item.
    /// 
    FDOKGINF_API virtual bool ReadNext();

    /// \brief
    /// Closes the FdoISQLDataReader object, freeing any resources it may be holding.
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDOKGINF_API virtual void Close();     
  
protected:
  int ColumnNameToColumnIndex(FdoString* ColumnName);
};

#endif