#ifndef FDORDBMSCONNECTION_H
#define FDORDBMSCONNECTION_H    1
/*
 * Copyright (C) 2004-2006  Autodesk, Inc.
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of version 2.1 of the GNU Lesser
 * General Public License as published by the Free Software Foundation.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */


#include "FdoRdbms.h"
#include "../LongTransactionManager/LongTransactionManager.h"
#include "../LockManager/LockManager.h"
#include "../LongTransactionManager/LongTransactionManagerService.h"
#include <Sm/SchemaManager.h>
#include "SpatialManager/FdoRdbmsSpatialManager.h"
#include "../FeatureCommands/FdoRdbmsFeatureReader.h"

class FdoRdbmsFilterProcessor;


#include "DbiConnection.h"

class FdoRdbmsConnection: public FdoIConnection
{
private:

    FdoStringP                      mConnectionString; // connection string for the database
    int                             mConnectionTimeout; // connection attempt timeout in seconds
    FdoConnectionState              mState;
    DbiConnection                   *mDbiConnection; // Internal DBI connection
    bool                            mTransactionStarted;
    FdoRdbmsLongTransactionManager  *mLongTransactionManager;
	FdoRdbmsLockManagerP			mLockManager;
	int							    mUserNum;
    FdoInt64                        mUserSessionId;
    FdoRdbmsSpatialManagerP         mSpatialManager;
    FdoStringP                      mActiveSpatialContextName;
    FdoIoStreamP                    mConfigDoc;
    FdoFeatureSchemasP              mConfigSchemas;
    FdoSchemaMappingsP              mConfigMappings;
    FdoSmPhRbCacheP                 mSchemaRollbackCache;


protected:
    FdoIConnectionCapabilities      *mConnectionCapabilities;
    FdoICommandCapabilities         *mCommandCapabilities;
    FdoISchemaCapabilities          *mSchemaCapabilities;
    FdoIFilterCapabilities          *mFilterCapabilities;
    FdoIExpressionCapabilities      *mExpressionCapabilities;

    virtual ~FdoRdbmsConnection ();
    FdoRdbmsConnection ();

    FdoIGeometryCapabilities    *mGeometryCapabilities;

    /// <summary>
    /// The function sets the unique user number for the current user.
    /// </summary>
	void SetUserNum (int UserNum);

    /// <summary>
    /// The function sets the session id for the current user.
    /// </summary>
	void SetUserSessionId (int value);


public:

    bool GetIsTransactionStarted()
    {
        return mTransactionStarted;
    }

    void SetIsTransactionStarted(bool bTransactionStarted)
    {
        mTransactionStarted = bTransactionStarted;
    }

    virtual FdoRdbmsFilterProcessor* GetFilterProcessor() = 0;

	virtual bool SupportsInnerQuery() { return false; }
    /// <summary>
    /// The function returns the current user.
    /// </summary>
    virtual FdoStringP GetUser();

    /// <summary>
    /// The function returns the unique user number for the current user.
    /// </summary>
	int GetUserNum();

    /// <summary>
    /// The function returns the session id for the current user.
    /// </summary>
    FdoInt64 GetUserSessionId ();

    /// <summary>
    /// Gets the time to wait while trying to establish a connection before terminating the attempt and generating an error.
    /// The time (in seconds) to wait for a connection to open. The default value is 15 seconds.
    /// A value of 0 indicates no limit to the wait time, rather than no wait time, and should be avoided in a ConnectionString because it would cause the connection
    /// attempt to wait indefinitely.
    /// </summary>
    void set_ConnectionTimeout (int timeout);
    int get_ConnectionTimeout ();

    //  Gets the string used to open a data store.
    FDORDBMS_API virtual const wchar_t* GetConnectionString ();

    //  Sets the string used to open a data store. Can only be
    //  set while the connection is closed.
    FDORDBMS_API virtual void SetConnectionString (const wchar_t* value);

    //  Gets the time to wait while trying to establish a
    //  connection before terminating the attempt and generating
    //  an error.
    FDORDBMS_API virtual int GetConnectionTimeout ();

    //  Sets the time to wait while trying to establish a
    //  connection before terminating the attempt and generating
    //  an error.
    FDORDBMS_API virtual void SetConnectionTimeout (int value);

    //  Gets the current state of the connection.
    FDORDBMS_API virtual FdoConnectionState GetConnectionState ();

    /// <summary>
    /// Opens a feature connection with the settings specified
    /// by the ConnectionString attribute of the
    /// provider-specific feature connection object.
    /// </summary>
    FDORDBMS_API virtual FdoConnectionState Open ();

    virtual FdoConnectionState Open (bool skipPending);

    /// <summary>
    /// Closes the connection to the data store.
    /// </summary>
    FDORDBMS_API virtual void Close ();


    //  Begins a transaction and returns an object that realizes
    //  IFeatureTransaction.
    FDORDBMS_API virtual FdoITransaction *BeginTransaction ();

    //  Creates and returns the specified type of command object
    //  associated with the connection.
    FDORDBMS_API virtual FdoICommand *CreateCommand (FdoInt32 commandType);

    // A Provider has no schema mappings by default.
    // Each Provider with schema mappings must override this function.
    FDORDBMS_API virtual FdoPhysicalSchemaMapping* CreateSchemaMapping()
    {
        return NULL;
    }

    FDORDBMS_API virtual FdoIConnectionCapabilities* GetConnectionCapabilities();
    FDORDBMS_API virtual FdoICommandCapabilities* GetCommandCapabilities();
    FDORDBMS_API virtual FdoISchemaCapabilities* GetSchemaCapabilities();
    FDORDBMS_API virtual FdoIFilterCapabilities* GetFilterCapabilities();
    FDORDBMS_API virtual FdoIExpressionCapabilities* GetExpressionCapabilities();
    FDORDBMS_API virtual FdoIRasterCapabilities* GetRasterCapabilities ();
    FDORDBMS_API virtual FdoITopologyCapabilities* GetTopologyCapabilities();
    /// <summary>Gets an FdoIGeometryCapabilities interface describing the provider's support for geometry.</summary>
    /// <returns>Returns the geometry capabilities</returns>
    virtual FdoIGeometryCapabilities* GetGeometryCapabilities();

    // Sets the XML configuration stream used to configure a Data Store.
    // SetConfiguration can only be called while the connection is closed
    FDORDBMS_API virtual void SetConfiguration(FdoIoStream* configStream);

    FDORDBMS_API virtual void Dispose();

    //  Returns the specified capability set.
    DbiConnection    *GetDbiConnection() { return mDbiConnection; }

	void CreateSysDb( FdoString *dbName, FdoString *dbPassword, FdoString *connectString);
	void CreateDb( FdoString *dbName, FdoString *dbDescription, FdoString *dbPassword, FdoString *connectString, FdoString *ltMode, FdoString *lckMode );
	void DeleteDb( FdoString *dbName, FdoString *dbPassword, FdoString *connectString );

    // Gets the current Schema Manager for this connection
    FdoSchemaManagerP GetSchemaManager();

    // Creates a Schema Manager for the current provider.
    virtual FdoSchemaManagerP CreateSchemaManager();

	// Returns a DataStorePropertyDictionary object for this provider.
	virtual FdoIDataStorePropertyDictionary* CreateDataStoreProperties( int action ) = 0;

    //
    // Converts a dbi string date of a specific format to a FdoDateTime (time_t) format.
    // The string format have to be of the form: "YYYY-MM-DD-HH24-MI-SS"
    virtual FdoDateTime  DbiToFdoTime( const char* time ) = 0;

    //
    // Convert time_t( FdoDateTime ) to char of the form: "YYYY-MM-DD-HH24-MI-SS", See ORACLE_DATE_FORMAT define
    // It return a statically allocated storage that can be overwritten by subsequent call to this or other methods.
    virtual const char* FdoToDbiTime( FdoDateTime  time ) = 0;

    //
    // Returns the bind string used by the specific database engine; for example :1 for Oracle and ? for MySql.
    // By default ? is returned. If the database engine uses different syntax, then this method need to be ovewritten.
    virtual FdoStringP GetBindString( int n, const FdoSmLpPropertyDefinition* prop = NULL ) { return "?"; }

    // Workaround for SqlServer spatial bug: on Insert the geometries need to be bound last.
    virtual bool  BindGeometriesLast() { return false; }

    // Creates a Long Transaction Manager and its corresponding Long Transaction
    // Manager Service.
    virtual FdoRdbmsLongTransactionManager *CreateLongTransactionManager() = 0;

    // Returns the current Long Transaction Manager.
    FdoRdbmsLongTransactionManager *GetLongTransactionManager();

	// Lock Manager
	virtual FdoRdbmsLockManager *CreateLockManager() = 0;

	// Current Lock Manager
	FdoRdbmsLockManager *GetLockManager();

    // Sets the Spatial Manager.  TODO: do this via override, like LongTransactionManager.
    virtual void SetSpatialManager(FdoRdbmsSpatialManager * spatialManager);

    // Returns the current Spatial Manager.
    virtual FdoRdbmsSpatialManager *GetSpatialManager();

    FdoString * GetActiveSpatialContextName();
    void SetActiveSpatialContextName(FdoString * spatialContextName);
    void SetDefaultActiveSpatialContextName();

    virtual FdoRdbmsFeatureReader *GetOptimizedAggregateReader(const FdoSmLpClassDefinition* classDef, aggr_list *selAggrList) { return NULL; }

    virtual void Flush() {}

protected:
    //Instantiates the right Schema Manager for this connection's provider.
    virtual FdoSchemaManagerP NewSchemaManager(
        GdbiConnection* gdbiConnection, 
        FdoStringP currUser,
        FdoStringP currSchema
    ) = 0;

private:

    //
    // Prevent the use of the copy constructor by definning it and not implemeting it.
    // DO NOT IMPLEMENT
    FdoRdbmsConnection(const FdoRdbmsConnection &right);

    //
    // Prevent the use of the Assignment Operation by definning it and not implemeting it.
    // DO NOT IMPLEMENT
    FdoRdbmsConnection & operator=(const FdoRdbmsConnection &right);

    void DbiOpen(bool skipPending);
};

#endif // FDORDBMSCONNECTION_H

