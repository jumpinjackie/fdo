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

#include "SpatialContextCollection.h"
#include "PgUtility.h"
#include <PostGIS/FdoPostGisProvider.h>
#include <PostGIS/Override/PhysicalSchemaMapping.h>
// FDO
#include <Fdo.h>
#include <FdoCommonConnPropDictionary.h>
// std
#include <string>
// libpq
#include <libpq-fe.h>


namespace fdo { namespace postgis {

// Forward declarations of internal types
class PgCursor;
class SpatialContextCollection;

/// Implementation of connection interface for PostGIS provider.
/// It represents a unique session with FDO datastore.
///
/// \todo Make it noncopyable
///
class Connection : public FdoIConnection
{
public:

    /// Type of FDO smart pointer for Connection class.
    typedef FdoPtr<Connection> Ptr;

    /// Default constructor.
    Connection();

    //
    // FdoIConnection interface
    //

    /// Get pointer to FdoIConnectionCapabilities interface describing
    /// capabilities of the connection.
    FDOPOSTGIS_API FdoIConnectionCapabilities* GetConnectionCapabilities();

    /// Get pointer to FdoISchemaCapabilities interface describing
    /// support for the feature schema provided by PostGIS Provider.
    FDOPOSTGIS_API FdoISchemaCapabilities* GetSchemaCapabilities();
 	
    /// Get pointer to FdoICommandCapabilities interface describing supported commands.
    FDOPOSTGIS_API FdoICommandCapabilities* GetCommandCapabilities();

    /// Get pointer to FdoIFilterCapabilities interface describing 
    /// support for filters.
    FDOPOSTGIS_API FdoIFilterCapabilities* GetFilterCapabilities();

    /// Get pointer to FdoIExpressionCapabilities interface describing
    /// support for expressions.
    FDOPOSTGIS_API FdoIExpressionCapabilities* GetExpressionCapabilities();

    /// Get pointer to FdoIRasterCapabilities interface describing
    /// support for raster images.
    FDOPOSTGIS_API FdoIRasterCapabilities* GetRasterCapabilities();

    /// Get pointer to FdoITopologyCapabilities interface describing support for topology.
    FDOPOSTGIS_API FdoITopologyCapabilities* GetTopologyCapabilities();

    /// Get pointer to FdoIGeometryCapabilities interface describing support for geometry.
    FDOPOSTGIS_API FdoIGeometryCapabilities* GetGeometryCapabilities();

    /// Get connection string used to establish connection with a datastore.
    FDOPOSTGIS_API FdoString* GetConnectionString();

    /// Set connection string used to establish connection with a datastore.
    FDOPOSTGIS_API void SetConnectionString(FdoString* value);

    /// Get pointer to FdoIConnectionInfo interface that can be used to interrogate
    /// and set connection properties.
    FDOPOSTGIS_API FdoIConnectionInfo* GetConnectionInfo();

    /// Get state of current connection.
    FDOPOSTGIS_API FdoConnectionState GetConnectionState();

    /// Get number of milliseconds to wait while trying to establish a connection
    /// before terminating the attempt and generating an error.
    FDOPOSTGIS_API FdoInt32 GetConnectionTimeout();

    /// Set number of milliseconds to wait while trying to establish a connection
    /// before terminating the attempt and generating an error.
    FDOPOSTGIS_API void SetConnectionTimeout(FdoInt32 value);

    /// Establish connection with a datastore using settings
    /// specified by ConnectionString attribute.
    FDOPOSTGIS_API FdoConnectionState Open();

    /// Close current connection.
    FDOPOSTGIS_API void Close();

    /// Begin transaction and return pointer to FdoITransaction interface realizing
    /// transaction operations support.
    FDOPOSTGIS_API FdoITransaction* BeginTransaction();

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

    //
    // Connection custom interface used internally by the provider
    //
    
    /// \todo To be documented.
    FdoFeatureSchemaCollection* GetFeatureSchema();
    
    /// \todo To be documented.
    ov::PhysicalSchemaMapping* GetPhysicalSchemaMapping();
    
    /// \todo To be documented.
    SpatialContextCollection* GetSpatialContexts();

    /// \todo To be documented.
    void PgExecuteCommand(char const* sql);

    /// \todo To be documented.
    void PgExecuteCommand(char const* sql, FdoSize& affected);
    
    /// \todo To be documented.
    void PgExecuteCommand(char const* sql, details::pgexec_params_t const& params, FdoSize& affected);

    /// \todo To be documented.
    PGresult* PgExecuteQuery(char const* sql);

    /// \todo To be documented.
    PgCursor* PgCreateCursor(char const* name);

    /// \todo To be documented.
    PGresult* PgDescribeCursor(char const* name);
    
    /// Create a transaction scope.
    /// If there is already a transaction active, calling this function
    /// does not begin a new one, but just increments the scope level counter.
    /// The idea behind soft transactions has been taken from OGR.
    ///
    /// \note PostgreSQL does not support nested transactions.
    ///
    void PgBeginSoftTransaction();
    
    /// Commit current transaction if the scope level counter hits the outer scope.
    void PgCommitSoftTransaction();
    
    /// Rollback current transaction if there is any transaction active.
    /// Rollback has effect even for nested level of transactions.
    void PgRollbackSoftTransaction();
    
    /// Force unwinding of any active transaction with commit execution.
    void PgFlushSoftTransaction();

protected:

    /// Destructor.
    virtual ~Connection();

    //
    // FdoIDisposable interface
    //

    /// Dispose this object.
    void Dispose();

private:

    // Connection is a noncopyable type
    Connection(Connection const&);
    Connection& operator=(Connection const&);

    //
    // Data members
    //

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

    // Pointer to PostgreSQL query result.
    PGresult* mPgResult;
    
    // Counter of soft transaction scopes.
    FdoInt32 mSoftTransactionLevel;
    
    FdoPtr<FdoFeatureSchemaCollection> mFeatureSchemas;
    ov::PhysicalSchemaMapping::Ptr mPhysicalSchemaMapping;
    SpatialContextCollection::Ptr mSpatialContexts;

    //
    // Private operations
    //

    // Check internal state of connection to PostgreSQL server.
    void ValidateConnectionState();

    // Validate connection string assigned to current connection instance.
    // The function throws exception on errors detected.
    void ValidateConnectionString();

    // Validate connection properties in ConnectionInfo dictionary.
    // The function throws exception on errors detected.
    void ValidateRequiredProperties();


    // Get PostgreSQL connection parameters from FDO connection properties.
    details::pgconn_params_t GetPgConnectionParams(FdoPtr<FdoCommonConnPropDictionary> dict);

    void SetPgCurrentSchema(FdoStringP schema);

};

}} // namespace fdo::postgis

#endif // FDOPOSTGIS_CONNECTION_H_INCLUDED