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
#ifndef FDOPOSTGIS_DATAREADER_H_INCLUDED
#define FDOPOSTGIS_DATAREADER_H_INCLUDED

#include "Reader.h"

//
// Forward declarations
//
class FdoClassDefinition;

namespace fdo { namespace postgis {

// Forward declarations of internal types
class Connection;
class PgCursor;

/// Implementation of forward-only and read-only iterator for reading
/// relational table data.
/// This reader works against PostGIS datastore.
///
class DataReader : public Reader<FdoIDataReader>
{
public:

    /// Type of FDO smart pointer for the class.
    typedef FdoPtr<DataReader> Ptr;

    /// Default constructor.
    DataReader(Connection* conn, PgCursor* cursor);

    //
    // FdoIDataReader interface
    //

    /// Get number of properties in the result set.
    virtual FdoInt32 GetPropertyCount();

    /// Get name of property at given ordinal position.
    virtual FdoString* GetPropertyName(FdoInt32 index);

    /// Get data type of property with the specified name.
    virtual FdoDataType GetDataType(FdoString* propertyName);

    /// Get FDO property type of a given property.
    virtual FdoPropertyType GetPropertyType(FdoString* propertyName);

protected:

	/// Default constructor (DO NOT USE -- Only defined to make Linux Build Happy).
    DataReader();

	/// Destructor
    virtual ~DataReader();

    //
    // FdoIDisposable
    //
    virtual void Dispose();

private:

    //
    // Private data members
    //

    typedef Reader<FdoIDataReader> Base;

};

}} // namespace fdo::postgis

#endif // FDOPOSTGIS_DATAREADER_H_INCLUDED
