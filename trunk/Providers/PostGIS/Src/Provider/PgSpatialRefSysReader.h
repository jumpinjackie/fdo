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
#ifndef FDOPOSTGIS_PGSPATIALREFSYSREADER_H_INCLUDED
#define FDOPOSTGIS_PGSPATIALREFSYSREADER_H_INCLUDED

#include "Connection.h"
// std
#include <string>
// boost
#include <boost/noncopyable.hpp>

namespace fdo { namespace postgis {

/// Implementation of reader of PostGIS spatial_ref_sys table.
///
/// \todo XXX - This is only a stub for concept, not finished yet
///
class PgSpatialRefSysReader :
    public FdoIDisposable,
    private boost::noncopyable
{
public:

    /// Type of FDO smart pointer for the class.
    typedef FdoPtr<PgSpatialRefSysReader> Ptr;

    /// Constructor creates new reader instance associated with given connection.
    PgSpatialRefSysReader(Connection* conn);

    bool Find(FdoInt32 const& srid);
    
protected:

	/// Default constructor (DO NOT USE -- Only defined to make Linux Build Happy).
    PgSpatialRefSysReader();

	/// Destructor closes reader on destroy.
    virtual ~PgSpatialRefSysReader();

    //
    // FdoIDisposable interface
    //

    /// Dispose this object.
    void Dispose();

private:

    //
    // Private data members
    //

    // Handle to connection for current session.
    Connection::Ptr mConn;

    //
    // Private operations
    //

    void ValidateConnectionState() const;
};

}} // namespace fdo::postgis

#endif // FDOPOSTGIS_PGSPATIALREFSYSREADER_H_INCLUDED
