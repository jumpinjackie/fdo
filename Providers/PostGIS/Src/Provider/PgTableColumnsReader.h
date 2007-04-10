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
#ifndef FDOPOSTGIS_PGTABLECOLUMNSREADER_H_INCLUDED
#define FDOPOSTGIS_PGTABLECOLUMNSREADER_H_INCLUDED

#include "Connection.h"
#include "SQLDataReader.h"

// FDO
#include <Fdo/Commands/Sql/ISQLCommand.h>
// std
#include <string>
#include <vector>
// boost
#include <boost/noncopyable.hpp>

namespace fdo { namespace postgis {

/// Implementation of reader collecting details of a PostGIS table.
/// Table schema is read using current connection established to a FDO datastore.
/// This class is dedicated for <strong>internal use only</strong> and
/// it's <strong>not</strong> exposed to FDO clients.
///
///
/// \todo TODO: ADD VALIDATION OF STATE OF THE INTERNAL READER - High Priority!
///
///
class PgTableColumnsReader :
    public FdoIDisposable,
    private boost::noncopyable
{
public:

    /// Type of FDO smart pointer for the class.
    typedef FdoPtr<PgTableColumnsReader> Ptr;
    
    /// Constructor creates new reader instance associated with given connection.
    PgTableColumnsReader(Connection* conn, FdoStringP schemaName, FdoStringP tableName);

    /// Get name of schema which a table being described belongs to.
    FdoStringP GetSchemaName() const;
    
    /// Get name of a table being described.
    FdoStringP GetTableName() const;
    
    /// Get name of column currently pointed by the reader.
    FdoStringP GetColumnName() const;
    
    /// \todo TODO - Implement using COMMENT feature.
    FdoStringP GetColumnDescription() const;
    
    /// Get ordinal position of current column.
    FdoInt32 GetColumnPosition() const;
    
    /// Get type of current column.
    /// \return FDO enumerator describing column type.
    FdoDataType GetColumnType() const;
    
    /// Get size of current column.
    /// \return Size of column as number of bytes, characters,
    /// or digits depending on column type.
    FdoInt32 GetColumnSize() const;
    
    /// \todo to be documented
    FdoInt32 GetColumnPrecision() const;
    
    /// \todo to be documented
    FdoInt32 GetColumnScale() const;
    
    /// \todo to be documented
    bool GetColumnNullability() const;
    
    ///
    bool IsPrimaryKey() const;

    /// Open table reader.
    /// This operation mimics FDO command usually creating instance of a reader.
    /// There is no command dedicated for PgTableColumnsReader instantiation,
    /// so this function behaves like a command preparing reader instance.
    void Open();

    /// Advance the reader to next column of a table.
    /// \return
    /// true if there is another table to read or false if iterator reached
    /// end of the result set.
    bool ReadNext();
    
    /// Close reader instance releasing all associated resources.
    void Close();
    
protected:

    /// Destructor closes reader on destroy.
    virtual ~PgTableColumnsReader();

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
    // The PgTableColumnsReader forwards calls to the SQLDataReader.
    FdoPtr<FdoISQLDataReader> mReader;
    
    // Command used internally to feed reader with query results.
    FdoPtr<FdoISQLCommand> mCmd;
    
    // Name of PostgreSQL schema (FDO datastore) from which a table is read.
    FdoStringP mSchema;
    
    // Name of a table for which given reader has been created.
    FdoStringP mTable;
    
    //
    // Private operations
    //
    
    void ValidateConnectionState() const;
};

}} // namespace fdo::postgis

#endif // FDOPOSTGIS_PGTABLECOLUMNSREADER_H_INCLUDED
