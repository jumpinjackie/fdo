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
#ifndef FDOPOSTGIS_PGGEOMETRYCOLUMN_H_INCLUDED
#define FDOPOSTGIS_PGGEOMETRYCOLUMN_H_INCLUDED

#include "Connection.h"
// std
#include <string>
// boost
#include <boost/noncopyable.hpp>

namespace fdo { namespace postgis {

/// The PgGeometryColumn class describes geometry column from a PostGIS-enabled
/// spatial table.
///
class PgGeometryColumn : public FdoIDisposable, private boost::noncopyable
{
public:

    /// Type of FDO smart pointer for the class.
    typedef FdoPtr<PgGeometryColumn> Ptr;

    /// Constructor creates new reader instance associated with given connection.
    PgGeometryColumn(FdoString* name, FdoDimensionality dim, FdoGeometryType type,
                     FdoInt32 srid);

    FdoStringP GetName() const;

    FdoDimensionality GetDimensionality() const;
    
    FdoGeometryType GetGeometryType() const;
    
    FdoInt32 GetSRID() const;

protected:

    /// Destructor closes reader on destroy.
    virtual ~PgGeometryColumn();

    //
    // FdoIDisposable interface
    //

    /// Dispose this object.
    void Dispose();

private:

    //
    // Private data members
    //
    
    FdoStringP mName;
    FdoDimensionality mDim;
    FdoGeometryType mType;
    FdoInt32 mSRID;

    //
    // Private operations
    //

};

}} // namespace fdo::postgis

#endif // FDOPOSTGIS_PGGEOMETRYCOLUMN_H_INCLUDED
