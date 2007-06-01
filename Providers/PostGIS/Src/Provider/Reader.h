//
// Copyright (C) 2007 Refractions Research, Inc. 
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
#ifndef FDOPOSTGIS_READER_H_INCLUDED
#define FDOPOSTGIS_READER_H_INCLUDED

#include "Connection.h"
#include "SQLDataReader.h"
#include "PgCursor.h"
#include <cassert>

namespace fdo { namespace postgis {

/// Implementation of interface providing a forward-only, read-only
/// iterator for reading data from FDO datastore using PostGIS provider.
///
template <typename T>
class Reader : public T
{
public:

    /// Constructor creating instance of reader associated with given
    /// connection, statement and query result.
    Reader(Connection* conn, PgCursor* cursor);

    //
    // FdoIReader interface.
    //

    /// Get Boolean value of the specified property.
    virtual bool GetBoolean(FdoString* propertyName);

    /// Get byte value of the specified property.
    virtual FdoByte GetByte(FdoString* propertyName);

    /// Get date and time value of the specified property.
    virtual FdoDateTime GetDateTime(FdoString* propertyName);

    /// Get double-precision floating point value of the specified property.
    virtual double GetDouble(FdoString* propertyName);

    /// Get 16-bit integer value of the specified property.
    virtual FdoInt16 GetInt16(FdoString* propertyName);

    /// Get 32-bit integer value of the specified property.
    virtual FdoInt32 GetInt32(FdoString* propertyName);

    /// Get 64-bit integer value of the specified property.
    virtual FdoInt64 GetInt64(FdoString* propertyName);

    /// Get Single floating point value of the specified property.
    virtual float GetSingle(FdoString* propertyName);

    /// Get string value of the specified property.
    virtual FdoString* GetString(FdoString* propertyName);

    /// Gets LOBValue reference.
    virtual FdoLOBValue* GetLOB(FdoString* propertyName);

    /// Get reference of the specified LOB property as of type of
    /// FdoBLOBStreamReader or FdoCLOBStreamReader etc.
    virtual FdoIStreamReader* GetLOBStreamReader(wchar_t const* propertyName);

    /// Check if the value of the specified property is null.
    virtual bool IsNull(FdoString* propertyName);

    /// Get geometry value of the specified property as a byte array in FGF format.
    FDOPOSTGIS_API virtual FdoByteArray* GetGeometry(FdoString* propertyName);

    /// Get geometry value of the specified property as a byte array in FGF format.
    /// This method is a language-specific performance optimization that returns
    /// pointer to the array data, rather than to an object that encapsulates the array.
    FDOPOSTGIS_API const FdoByte* GetGeometry(FdoString* propertyName, FdoInt32* count);

    /// Get raster object of the specified property.
    FDOPOSTGIS_API virtual FdoIRaster* GetRaster(FdoString* propertyName);

    /// Advance the reader iterator to next item.
    /// \return true if there is another object to read, false if reading is complete.
    FDOPOSTGIS_API virtual bool ReadNext();

    /// Close the reader object and release any of its resources.
    FDOPOSTGIS_API virtual void Close();

protected:

    /// Destructor.
    virtual ~Reader();

    //
    // FdoIDisposable interface
    //

    virtual void Dispose();

    //
    // Data members
    //

    // The connection this reader is associated with.
    FdoPtr<Connection> mConn;

private:
	
    // Handler to cursor instance defined for current reader.
    PgCursor::Ptr mCursor;

    SQLDataReader::Ptr mSQLReader;

    // Internal cache for FGF stream.
    // The array is only guaranteed to be valid until next call of ReadNext() or Close(),
    // or the disposal of this reader object.
    FdoPtr<FdoByteArray> mFgfGeometry;

}; // class Reader

template <typename T>
Reader<T>::Reader(Connection* conn, PgCursor* cursor) :
    mConn(conn), mCursor(cursor), mSQLReader(new SQLDataReader(mCursor)), mFgfGeometry(NULL)
{
    FDOLOG_MARKER("Created reader instance");

    assert(NULL != mConn);
    assert(NULL != mCursor);
    assert(NULL != mSQLReader);
    assert(NULL == mFgfGeometry);

    FDO_SAFE_ADDREF(mConn.p);
    FDO_SAFE_ADDREF(mCursor.p);
}

template <typename T>
Reader<T>::~Reader()
{
    // idle
}

///////////////////////////////////////////////////////////////////////////////
// FdoIDisposable interface
///////////////////////////////////////////////////////////////////////////////

template <typename T>
void Reader<T>::Dispose()
{
    Close();
    delete this;
}

///////////////////////////////////////////////////////////////////////////////
// FdoIReader interface
///////////////////////////////////////////////////////////////////////////////

template <typename T>
bool Reader<T>::GetBoolean(FdoString* propertyName)
{
    return mSQLReader->GetBoolean(propertyName);
}

template <typename T>
FdoByte Reader<T>::GetByte(FdoString* propertyName)
{
    return mSQLReader->GetByte(propertyName);
}

template <typename T>
FdoDateTime Reader<T>::GetDateTime(FdoString* propertyName)
{
    return mSQLReader->GetDateTime(propertyName);
    return FdoDateTime();
}

template <typename T>
double Reader<T>::GetDouble(FdoString* propertyName)
{
    return mSQLReader->GetDouble(propertyName);
}

template <typename T>
FdoInt16 Reader<T>::GetInt16(FdoString* propertyName)
{
    return mSQLReader->GetInt16(propertyName);
}

template <typename T>
FdoInt32 Reader<T>::GetInt32(FdoString* propertyName)
{
    return mSQLReader->GetInt32(propertyName);
}

template <typename T>
FdoInt64 Reader<T>::GetInt64(FdoString* propertyName)
{
    return mSQLReader->GetInt64(propertyName);
}

template <typename T>
float Reader<T>::GetSingle(FdoString* propertyName)
{
    return mSQLReader->GetSingle(propertyName);
}

template <typename T>
FdoString* Reader<T>::GetString(FdoString* propertyName)
{
    return mSQLReader->GetString(propertyName);
}

template <typename T>
FdoLOBValue* Reader<T>::GetLOB(FdoString* propertyName)
{
    FDOLOG_MARKER("Reader::GetLOB");
    FDOLOG_WRITE("NOT IMPLEMENTED");
    assert(!"NOT IMPLEMENTED");
    return NULL;
}

template <typename T>
FdoIStreamReader* Reader<T>::GetLOBStreamReader(wchar_t const* propertyName)
{
    FDOLOG_MARKER("Reader::GetLOBStreamReader");
    FDOLOG_WRITE("NOT IMPLEMENTED");
    assert(!"NOT IMPLEMENTED");
    return NULL;
}

template <typename T>
bool Reader<T>::IsNull(FdoString* propertyName)
{
    return mSQLReader->IsNull(propertyName);
}

template <typename T>
FdoByteArray* Reader<T>::GetGeometry(FdoString* propertyName)
{
    return mSQLReader->GetGeometry(propertyName);
}

template <typename T>
const FdoByte* Reader<T>::GetGeometry(FdoString* propertyName, FdoInt32* count)
{
    mFgfGeometry = GetGeometry(propertyName);
    *count = mFgfGeometry->GetCount();
    
    return mFgfGeometry->GetData();
}

template <typename T>
FdoIRaster* Reader<T>::GetRaster(FdoString* propertyName)
{
    // Raster support not available in PostGIS.
    return NULL;
}

template <typename T>
bool Reader<T>::ReadNext()
{
    bool hasTuples = mSQLReader->ReadNext();
    return hasTuples;
}

template <typename T>
void Reader<T>::Close()
{
    FDOLOG_MARKER("Reader::+Close");

    if (NULL != mSQLReader)
        mSQLReader->Close();

    if (NULL != mCursor)
        mCursor->Close();
}

}} // namespace fdo::postgis

#endif // FDOPOSTGIS_READER_H_INCLUDED
