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
#ifndef FDOPOSTGIS_CONNECTION_H_INCLUDED
#define FDOPOSTGIS_CONNECTION_H_INCLUDED

#include <Fdo/Connections/IConnection.h>

// PostgreSQL client library
#include <libpq-fe.h>

namespace fdo { namespace postgis {

/// Implementation of connection interface for PostGIS provider.
/// It represents a unique session with FDO datastore.
///
/// \todo Make it noncopyable
///
class Connection : public FdoIConnection
{
public:

    /// Default constructor.
    Connection();

    //
    // FdoIConnection interface
    //

    /// Get pointer to FdoIConnectionCapabilities interface describing
    /// capabilities of the connection.
    FdoIConnectionCapabilities* GetConnectionCapabilities();

    /// Get pointer to FdoISchemaCapabilities interface describing
    /// support for the feature schema provided by PostGIS Provider.
    FdoISchemaCapabilities* GetSchemaCapabilities();
 	
    /// Get pointer to FdoICommandCapabilities interface describing supported commands.
    FdoICommandCapabilities* GetCommandCapabilities();

    /// Get pointer to FdoIFilterCapabilities interface describing 
    /// support for filters.
    FdoIFilterCapabilities* GetFilterCapabilities();

    /// Get pointer to FdoIExpressionCapabilities interface describing
    /// support for expressions.
    FdoIExpressionCapabilities* GetExpressionCapabilities();

    /// Get pointer to FdoIRasterCapabilities interface describing
    /// support for raster images.
    FdoIRasterCapabilities* GetRasterCapabilities();

    /// Get pointer to FdoITopologyCapabilities interface describing support for topology.
    FdoITopologyCapabilities* GetTopologyCapabilities();

    /// Get pointer to FdoIGeometryCapabilities interface describing support for geometry.
    FdoIGeometryCapabilities* GetGeometryCapabilities();

    /// Get connection string used to establish connection with a datastore.
    FdoString* GetConnectionString();

    /// Set connection string used to establish connection with a datastore.
    void SetConnectionString(FdoString* value);

    /// Get pointer to FdoIConnectionInfo interface that can be used to interrogate
    /// and set connection properties.
    FdoIConnectionInfo* GetConnectionInfo();

    /// Get state of current connection.
    FdoConnectionState GetConnectionState();

    /// Get number of milliseconds to wait while trying to establish a connection
    /// before terminating the attempt and generating an error.
    FdoInt32 GetConnectionTimeout();

    /// Set number of milliseconds to wait while trying to establish a connection
    /// before terminating the attempt and generating an error.
    void SetConnectionTimeout(FdoInt32 value);

    /// Establish connection with a datastore using settings
    /// specified by ConnectionString attribute.
    FdoConnectionState Open();

    /// Close current connection.
    void Close();

    /// Begin transaction and return pointer to FdoITransaction interface realizing
    /// transaction operations support.
    FdoITransaction* BeginTransaction();

    /// Create command object of specified type and return pointer to interface realizing
    /// command capabilities associated with the connection.
    FdoICommand* CreateCommand(FdoInt32 type);

    /// Factory function that creates an empty Schema Override mapping specific
    /// for PostGIS Provider.
    FdoPhysicalSchemaMapping* CreateSchemaMapping();

    /// Set XML configuration stream used to configure connected datastore.
    void SetConfiguration(FdoIoStream* configStream);

    /// Force to write any cached data to currently connected datastore
    void Flush();

protected:

    /// Destructor.
    virtual ~Connection();

    //
    // FdoIDisposable interface
    //

    /// Dispose this object.
    void Dispose();

private:

    // Connection information object.
    // It's of lazy-creation object, delayed until first request,
    // on GetConnectionInfo() call.
    FdoPtr<FdoIConnectionInfo> mConnInfo;

    // Connection string.
    FdoStringP mConnString;

    // Connection state flag (closed, pending, open).
    FdoConnectionState mConnState;

    // Pointer to PostgreSQL connection object of current session.
    PGconn* mPgConn;

};

}} // namespace fdo::postgis

#endif // FDOPOSTGIS_CONNECTION_H_INCLUDED