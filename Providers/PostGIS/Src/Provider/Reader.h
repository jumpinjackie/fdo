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
    Reader(Connection* conn);

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
    virtual FdoByteArray* GetGeometry(FdoString* propertyName);

    /// Get raster object of the specified property.
    virtual FdoIRaster* GetRaster(FdoString* propertyName);

    /// Advance the reader iterator to next item.
    /// \return true if there is another object to read, false if reading is complete.
    virtual bool ReadNext();

    /// Close the reader object and release any of its resources.
    virtual void Close();

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
	
}; // class Reader

template <typename T>
Reader<T>::Reader(Connection* conn) : mConn(conn)
{
    assert(!"TODO - Add missing parameters");
}

template <typename T>
Reader<T>::~Reader()
{
}

///////////////////////////////////////////////////////////////////////////////
// FdoIDisposable interface
///////////////////////////////////////////////////////////////////////////////

template <typename T>
void Reader<T>::Dispose()
{
    delete this;
}

///////////////////////////////////////////////////////////////////////////////
// FdoIReader interface
///////////////////////////////////////////////////////////////////////////////

template <typename T>
bool Reader<T>::GetBoolean(FdoString* propertyName)
{
    assert(!"NOT IMPLEMENTED");
    return 0;
}

template <typename T>
FdoByte Reader<T>::GetByte(FdoString* propertyName)
{
    assert(!"NOT IMPLEMENTED");
    return 0;
}

template <typename T>
FdoDateTime Reader<T>::GetDateTime(FdoString* propertyName)
{
    assert(!"NOT IMPLEMENTED");
    return FdoDateTime();
}

template <typename T>
double Reader<T>::GetDouble(FdoString* propertyName)
{
    assert(!"NOT IMPLEMENTED");
    return 0;
}

template <typename T>
FdoInt16 Reader<T>::GetInt16(FdoString* propertyName)
{
    assert(!"NOT IMPLEMENTED");
    return 0;
}

template <typename T>
FdoInt32 Reader<T>::GetInt32(FdoString* propertyName)
{
    assert(!"NOT IMPLEMENTED");
    return 0;
}

template <typename T>
FdoInt64 Reader<T>::GetInt64(FdoString* propertyName)
{
    assert(!"NOT IMPLEMENTED");
    return 0;
}

template <typename T>
float Reader<T>::GetSingle(FdoString* propertyName)
{
    assert(!"NOT IMPLEMENTED");
    return 0;
}

template <typename T>
FdoString* Reader<T>::GetString(FdoString* propertyName)
{
    assert(!"NOT IMPLEMENTED");
    return 0;
}

template <typename T>
FdoLOBValue* Reader<T>::GetLOB(FdoString* propertyName)
{
    assert(!"NOT IMPLEMENTED");
    return 0;
}

template <typename T>
FdoIStreamReader* Reader<T>::GetLOBStreamReader(wchar_t const* propertyName)
{
    assert(!"NOT IMPLEMENTED");
    return 0;
}

template <typename T>
bool Reader<T>::IsNull(FdoString* propertyName)
{
    assert(!"NOT IMPLEMENTED");
    return 0;
}

template <typename T>
FdoByteArray* Reader<T>::GetGeometry(FdoString* propertyName)
{
    assert(!"NOT IMPLEMENTED");
    return 0;
}

template <typename T>
FdoIRaster* Reader<T>::GetRaster(FdoString* propertyName)
{
    assert(!"NOT IMPLEMENTED");
    return 0;
}

template <typename T>
bool Reader<T>::ReadNext()
{
    assert(!"NOT IMPLEMENTED");
    return 0;
}

template <typename T>
void Reader<T>::Close()
{
    assert(!"NOT IMPLEMENTED");
}

}} // namespace fdo::postgis

#endif // FDOPOSTGIS_READER_H_INCLUDED
