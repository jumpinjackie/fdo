//
// Copyright(C) 2007 Refractions Research, Inc. 
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
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
//
#ifndef FDOPOSTGIS_SQLDATAREADER_H_INCLUDED
#define FDOPOSTGIS_SQLDATAREADER_H_INCLUDED

#include "PgCursor.h"
#include "Connection.h"
#include <Fdo/Commands/Sql/ISQLDataReader.h>

//
// Forward declarations
//
class FdoLOBValue;
class FdoIStreamReader;

namespace fdo { namespace postgis {

/// Implementation of interface providing forward-only and read-only iterator
/// for reading relational table data.
/// This implementation is specific for PostGIS provider for FDO.
///
/// \todo Check and move functions to protected area.
///
class SQLDataReader : public FdoISQLDataReader
{
public:

    /// Constructor creates instance of SQL data reader associated
    /// with given connection.
    SQLDataReader(PgCursor* cursor);

    //
    // FdoISQLDataReader interface
    //

    /// Get number of columns in the result set.
    FdoInt32 GetColumnCount();
    
    /// Get name of the column at the given ordinal position.
    FdoString* GetColumnName(FdoInt32 index);
    
    /// Get data type of the column with the specified name.
    FdoDataType GetColumnType(FdoString* columnName);
    
    /// Get FDO property type of the column with the specified name.
    FdoPropertyType GetPropertyType(FdoString* columnName);
    
    /// Get Boolean value of the specified column.
    bool GetBoolean(FdoString* columnName);
    
    /// Get byte value of the specified column.
    FdoByte GetByte(FdoString* columnName);
    
    /// Get date time value of the specified column.
    FdoDateTime GetDateTime(FdoString* columnName);
    
    /// Get double-precision floating point value of the specified column.
    double GetDouble(FdoString* columnName);
    
    /// Get signed 16-bit integer value of the specified column.
    FdoInt16 GetInt16(FdoString* columnName);
    
    /// Get signed 32-bit integer value of the specified column.
    FdoInt32 GetInt32(FdoString* columnName);
    
    /// Get signed 64-bit integer value of the specified column.
    FdoInt64 GetInt64(FdoString* columnName);
    
    /// Get single-precision floating point value of the specified column.
    float GetSingle(FdoString* columnName);
    
    /// Get string value of the specified column.
    FdoString* GetString(FdoString* columnName);
    
    /// Get LOBValue reference.
    FdoLOBValue* GetLOB(FdoString* columnName);
    
    /// Get reference of the specified LOB property as an instance of
    /// type of FdoBLOBStreamReader or FdoCLOBStreamReader etc.
    FdoIStreamReader* GetLOBStreamReader(wchar_t const* columnName);
    
    /// Return true if the value of the specified column is null.
    bool IsNull(FdoString* columnName);
    
    /// Get geometry value of the specified column as a byte array in FGF format.
    FdoByteArray* GetGeometry(FdoString* columnName);
    
    /// Advance the reader to the next item.
    bool ReadNext();
    
    /// Close the reader object and release any resources it's holding.
    void Close();

protected:

    /// Destructor.
    virtual ~SQLDataReader();

    //
    // FdoIDisposable interface
    //

    virtual void Dispose();

private:

    PgCursor::Ptr mCursor;

};

}} // namespace fdo::postgis

#endif // FDOPOSTGIS_SQLDATAREADER_H_INCLUDED
