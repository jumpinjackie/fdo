//
// Copyright (C) 2006 Refractions Research, Inc.
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
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
#ifndef FDORDBMSPOSTGISCONNECTION_H
#define FDORDBMSPOSTGISCONNECTION_H

#include "FdoRdbms.h"

//
// Forward declarations
//
class FdoRdbmsPostGisFilterProcessor;


/// \brief
/// Implements FdoIConnection interface for PostGIS provider.
///
/// \sa FdoIConnection
///
class FdoRdbmsPostGisConnection: public FdoRdbmsConnection
{
public:
	
    /// Factory method that creates PostGIS connection instance.
    ///
    /// @return Pointer to PostGIS connecftion instance.
    ///
    static FdoRdbmsPostGisConnection* Create();
    
    /// Creates and returns the specified type of command object
    /// associated with the connection.
    ///
    /// @return Pointer to instance of newly created command.
    ///
    virtual FdoICommand *CreateCommand(FdoInt32 commandType);

    /// Gets Filter Processor instance for PostGIS provider.
    ///
    /// @return Pointer to filter instance instance.
    ///
    virtual FdoRdbmsFilterProcessor* GetFilterProcessor();

    /// Creates appropriate Schema Manager for passed
    /// connection's provider.
    ///
    /// @param gdbiConnection [in] - pointer to instance connection.
    /// @param currUser [in] - user name.
    /// @param currSchema [in] - schema name.
    /// @return Pointer to filter instance instance.
    ///
    virtual FdoSchemaManagerP NewSchemaManager(GdbiConnection* gdbiConnection,
        FdoStringP currUser,
        FdoStringP currSchema);

    /// Creates an empty Schema Mapping set appropriate for
    /// the PostGIS Provider.
    ///
    /// @return Pointer to filter instance instance.
    ///
    FDORDBMS_API virtual FdoPhysicalSchemaMapping* CreateSchemaMapping();

    /// Fetches details about feature provider of current connection.
    ///
    /// @return Pointer to interface describing connection
    /// and provider details.
    ///
    virtual FdoIConnectionInfo* GetConnectionInfo();

    /// Fetches capabilities of current connection.
    ///
    /// @return Pointer to interface describing connection capabilities.
    ///
    virtual FdoIConnectionCapabilities* GetConnectionCapabilities();

    /// Gets an FdoIGeometryCapabilities interface describing
    /// the provider's support for geometry.
    ///
    /// @return Pointer to interface providing access to
    /// Geometry capabilities.
    ///
    virtual FdoIGeometryCapabilities* GetGeometryCapabilities();

    /// Gets an FdoISchemaCapabilities interface describing
    /// the provider's support for the feature schema.
    ///
    /// @return Pointer to interface providing access to
    /// Schema capabilities.
    ///
	virtual FdoISchemaCapabilities* GetSchemaCapabilities();

    /// Converts string with PostGIS-specific date format to
    /// a FdoDateTime (time_t) format.
    /// 
    /// @param time [in] - pointer to buffer with date.
    /// @return FdoDateTime instance with converted date and time.
    ///
    virtual FdoDateTime DbiToFdoTime(const char* time);

    /// Convert time_t (FdoDateTime) to string in PostGIS
    /// specific date format.
    /// 
    /// @param FdoDateTime instance date and time in FDO
    /// specific format.
    /// @return Statically allocated storage that can be
    /// overwritten by subsequent
    ///
    virtual const char* FdoToDbiTime(FdoDateTime  time);

    /// Opens new connection with the settings specified
    /// by the ConnectionString attribute of the provider-specific
    /// feature connection object.
    ///
    /// @return State of connection.
    ///

    virtual FdoStringP GetBindString( int n, const FdoSmLpPropertyDefinition* prop = NULL );

	virtual FdoConnectionState Open();

    /// Closes the connection to the data store.
	virtual void Close();

    /// Creates a Long Transaction Manager and its corresponding
    /// Long Transaction Manager Service.
    ///
    /// @return Pointer to LT manager instance.
    ///
    /// \warning There is no LT support in MySQL, so in PostGIS.
    /// \todo Check the warning about no support of LT in PostGIS.
    ///
    FdoRdbmsLongTransactionManager* CreateLongTransactionManager();

	/// Creates new data store using PostGIS provider.
    ///
    /// @param action [in] - Type of create action.
    /// @return DataStorePropertyDictionary object for PostGIS provider.
    ///
	virtual FdoIDataStorePropertyDictionary* CreateDataStoreProperties(
        int action);

	/// Creates feature lock and its corresponding Lock Manager
    /// for PostGIS provider.
    ///
    /// @return Pointer to Lock Manager instance.
    ///
    /// \warning Currently returns NULL.
    /// \todo Check the warning for PostGIS capabilites.
    ///
	FdoRdbmsLockManager* CreateLockManager();

    /// Gets FdoICommandCapabilities interface describing the commands 
    /// supported by PostGIS provider. 
    ///
    /// @return Pointer to interface providing access to commands
    /// capabilities.
    ///
	virtual FdoICommandCapabilities* GetCommandCapabilities();

    /// Gets an FdoIConnectionCapabilities interface
    /// describing connection capabilities for PostGIS provider.
    ///
    /// @return Pointer to interface providing access to connection
    /// capabilities.
    ///
	virtual FdoIFilterCapabilities* GetFilterCapabilities();

protected:

    /// Default constructor.
    FdoRdbmsPostGisConnection();

    /// Destructor.
    virtual ~FdoRdbmsPostGisConnection();

private:

    /// \todo What's the role of this data member?
    FdoRdbmsPostGisFilterProcessor* mFilterProcessor;

    /// Stores and provides access to connection properties.
    FdoIConnectionInfo* mConnectionInfo;
};

#endif // FDORDBMSPOSTGISCONNECTION_H
