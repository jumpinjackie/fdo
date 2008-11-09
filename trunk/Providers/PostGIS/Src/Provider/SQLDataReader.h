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
// std
#include <string>
// boost
#include <boost/lexical_cast.hpp>

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
/// \todo Add new constructor accepting custom coursor page size.
///       Consider moving the cursor page responsibility to command/PgCursor. 
///
class SQLDataReader : public FdoISQLDataReader
{
public:

    /// Type of FDO smart pointer for the class.
    typedef FdoPtr<SQLDataReader> Ptr;

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
    FdoBoolean GetBoolean(FdoString* columnName);
    
    /// Get byte value of the specified column.
    FdoByte GetByte(FdoString* columnName);
    
    /// Get signed 16-bit integer value of the specified column.
    FdoInt16 GetInt16(FdoString* columnName);
    
    /// Get signed 32-bit integer value of the specified column.
    FdoInt32 GetInt32(FdoString* columnName);
    
    /// Get signed 64-bit integer value of the specified column.
    FdoInt64 GetInt64(FdoString* columnName);
    
    /// Get single-precision floating point value of the specified column.
    FdoFloat GetSingle(FdoString* columnName);

    /// Get double-precision floating point value of the specified column.
    FdoDouble GetDouble(FdoString* columnName);

    /// Get string value of the specified column.
    FdoString* GetString(FdoString* columnName);
    
    /// Get date time value of the specified column.
    FdoDateTime GetDateTime(FdoString* columnName);

    /// Get LOBValue reference.
    FdoLOBValue* GetLOB(FdoString* columnName);
    
    /// Get reference of the specified LOB property as an instance of
    /// type of FdoBLOBStreamReader or FdoCLOBStreamReader etc.
    FdoIStreamReader* GetLOBStreamReader(FdoString* columnName);
    
    /// Return true if the value of the specified column is null.
    FdoBoolean IsNull(FdoString* columnName);
    
    /// Get geometry value of the specified column as a byte array in FGF format.
    FdoByteArray* GetGeometry(FdoString* columnName);
    
    /// Advance the reader to the next item.
    FdoBoolean ReadNext();
    
    /// Close the reader object and release any resources it's holding.
    void Close();

protected:

	/// Default constructor (DO NOT USE -- Only defined to make Linux Build Happy).
    SQLDataReader();

	/// Destructor.
    virtual ~SQLDataReader();

    //
    // FdoIDisposable interface
    //

    virtual void Dispose();

private:

    //
    // Private data members
    //

    // Handler to cursor instance defined for current reader.
    PgCursor::Ptr mCursor;

    // Index of current tuple.
    // Used if FETCH command returns more than 1 tuple.
    FdoSize mCurrentTuple;

    // Buffer for most recently read name of column.
    FdoStringP mColumnName;

    // Buffer for string value cache read from a column.
    // The reader returns raw pointer to a string, but pointee has to be cached somewhere.
    FdoStringP mCacheString;

    // Number of rows fetched per every FETCH command.
    static std::size_t const mCoursorPageSize = 500;

    //
    // Private operations
    //

    // Column value reader for simple types.
    // Internally, it does conversion from characters string to target type
    // using boost::lexical_cast.
    template <typename T>
    T GetValue(FdoString* columnName);

}; // SQLDataReader

///////////////////////////////////////////////////////////////////////////////
// Private operations
///////////////////////////////////////////////////////////////////////////////

template <typename T>
T SQLDataReader::GetValue(FdoString* columnName)
{
    FdoInt32 const fnumber = static_cast<int>(mCursor->GetFieldNumber(columnName));
    PgCursor::ResultPtr pgRes = mCursor->GetFetchResult();

    try
    {
        // Empty cval indicates NULL value

        T val = T();
        std::string cval(PQgetvalue(pgRes, static_cast<int>(mCurrentTuple), fnumber));
        if (!cval.empty())
        {
            val = boost::lexical_cast<T>(cval);
        }
        return val;
    }
    catch (boost::bad_lexical_cast& e)
    {
        FDOLOG_WRITE(L"SQLDataReader can not read value of column '%s'", columnName);
        FDOLOG_WRITE("ERROR: %s", e.what());

        throw FdoCommandException::Create(L"Field value conversion failed.");
    }
}

}} // namespace fdo::postgis

#endif // FDOPOSTGIS_SQLDATAREADER_H_INCLUDED
