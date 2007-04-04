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
/// \todo TODO: Add reading column description from COMMENT.
///
class PgGeometryColumn :
    public FdoIDisposable,
    private boost::noncopyable
{
public:

    /// Type of FDO smart pointer for the class.
    typedef FdoPtr<PgGeometryColumn> Ptr;

    /// Constructor creates object representing PostGIS geometry column.
    PgGeometryColumn(FdoString* name, FdoGeometryType type, FdoInt32 dim, FdoInt32 srid);
    
    /// Constructor creates object representing PostGIS geometry column with
    /// estimated spatial extent.
    PgGeometryColumn(FdoString* name, FdoGeometryType type, FdoInt32 dim, FdoInt32 srid,
        FdoPtr<FdoEnvelopeImpl> bbox);

    /// Get name of geometry column.
    /// \return String with name of the geometry column.
    FdoStringP GetName() const;
    
    /// Get description of geometry column.
    /// The description is retrieved from COMMENT property supported by
    /// PostgreSQL database catalog.
    /// \return String with description of the geometry column.
    ///
    /// \todo TO BE IMPLEMENTED
    ///
    FdoStringP GetDescription() const;

    /// Get FDO type of geometry stored in the column.
    /// \return Enumerator of FDO geometry type.
    FdoGeometryType GetGeometryType() const;
    
    /// Get coordinates dimension of geometry stored in the column.
    /// \return Bit mask representing combination of FDO coordinates dimension types.
    /// \sa FdoDimensionality
    FdoInt32 GetDimensionType() const;
    
    /// Get SRID associated with the geometry column.
    /// \return Number representing SRID identifiers.
    FdoInt32 GetSRID() const;

    FdoPtr<FdoEnvelopeImpl> GetEnvelope() const;

protected:

    /// Destructor.
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
    FdoGeometryType mType;
    FdoInt32 mDim;
    FdoInt32 mSRID;
    FdoPtr<FdoEnvelopeImpl> mEnvelope;

    //
    // Private operations
    //

};

}} // namespace fdo::postgis

#endif // FDOPOSTGIS_PGGEOMETRYCOLUMN_H_INCLUDED
