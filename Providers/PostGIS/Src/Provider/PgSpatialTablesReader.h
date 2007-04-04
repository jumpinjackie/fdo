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
#ifndef FDOPOSTGIS_PGSPATIALTABLESREADER_H_INCLUDED
#define FDOPOSTGIS_PGSPATIALTABLESREADER_H_INCLUDED

#include "Connection.h"
#include "SQLDataReader.h"
#include "PgGeometryColumn.h"
// FDO
#include <Fdo/Commands/Sql/ISQLCommand.h>
// std
#include <string>
#include <vector>
// boost
#include <boost/noncopyable.hpp>

namespace fdo { namespace postgis {

/// Implementation of reader for PostGIS-enabled spatial tables.
/// Tables are read using current connection established to a FDO datastore.
/// This class is dedicated for <strong>internal use only</strong> and
/// it's <strong>not</strong> exposed to FDO clients.
///
class PgSpatialTablesReader :
    public FdoIDisposable,
    private boost::noncopyable
{
public:

    /// Type of FDO smart pointer for the class.
    typedef FdoPtr<PgSpatialTablesReader> Ptr;
    
    /// Type of geometry columns collection
    typedef std::vector<PgGeometryColumn::Ptr> columns_t;
    
    /// Constructor creates new reader instance associated with given connection.
    PgSpatialTablesReader(Connection* conn);

    /// Get name of schema to which a spatial table belongs.
    FdoStringP GetSchemaName() const;
    
    /// Get name of a spatial table.
    FdoStringP GetTableName() const;

    /// Get collection of geometry columns.
    /// Every column is described by following elements:
    ///  - name
    ///  - coordinates dimension
    ///  - geometry type
    ///  - SRID
    columns_t GetGeometryColumns() const;
    
    /// Open spatial tables reader.
    /// This operation mimics FDO command usually creating instance of a reader.
    /// There is no command dedicated for PgSpatialTableReader instantiation,
    /// so this function behaves like a command preparing reader instance.
    void Open();

    /// Advance the reader to next spatial table.
    /// \return
    /// true if there is another table to read or false if iterator reached
    /// end of the result set.
    bool ReadNext();
    
    /// Close reader instance releasing all associated resources.
    void Close();
    
protected:

    /// Destructor closes reader on destroy.
    virtual ~PgSpatialTablesReader();

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
    
    // Handle to result reader used internally.
    // The PgSpatialTablesReader forwards calls to the SQLDataReader.
    //SQLDataReader::Ptr mReader;
    FdoPtr<FdoISQLDataReader> mReader;
    
    // Command used internally to feed reader with query results.
    FdoPtr<FdoISQLCommand> mCmd;
    
    // Name of current PostgreSQL schema (FDO datastore).
    std::string mCurrentSchema;
    
    // Name of a table currently read and cached.
    std::string mTableCached;
    
    //
    // Private operations
    //
    
    // Validates basic state of the reader instance.
    void ValidateConnectionState() const;

    // Executes query to estimate spatial extent of features
    // in given geometry column in a table.
    // Table is used with full path: schema.table.
    FdoPtr<FdoEnvelopeImpl> EstimateColumnExtent(
        std::string const& column) const;
};

}} // namespace fdo::postgis

#endif // FDOPOSTGIS_PGSPATIALTABLESREADER_H_INCLUDED
