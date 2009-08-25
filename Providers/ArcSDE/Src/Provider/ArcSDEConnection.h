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

#ifndef ARCSDECONNECTION_H
#define ARCSDECONNECTION_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include <sdetype.h>

class ArcSDETransaction;

class ArcSDEConnection : public FdoIConnection
{
protected:
    /**
     * The connection string containing details about how to connect to ArcSDE.
     */
    FdoStringP mConnectionString;

    /**
     * The connection object.
     */
    SE_CONNECTION mConnection;
    bool mPartialConnection;

    /**
     * The active version id.
     */
    LONG mActiveVersion;

    /**
     * The active state id.
     */
    LONG mActiveState;

    /**
     * The active spatial context.
     */
    FdoStringP mActiveSpatialContext;

    /**
     * Connection information object.
     */
    FdoPtr<FdoIConnectionInfo> mConnectionInfo;

    /**
     * Cached schema information.
     */
    FdoPtr<FdoFeatureSchemaCollection> mSchemaCollection;
	bool mSchemaCollectionFullyLoaded;
    SE_REGINFO *mCachedRegistrations;
    LONG mCachedRegistrationCount;
    SE_METADATAINFO *mCachedMetadataList;
    LONG mCachedMetadataListCount;
    SE_LAYERINFO *mCachedLayerList;
    LONG mCachedLayerListCount;
    SE_SPATIALREFINFO *mCachedSpatialRefList;
    LONG* mCachedSpatialRefSRIDList;
    LONG mCachedSpatialRefListCount;
    bool mCachedSpatialRefListCreatedByUs;

    /**
     * Cached schema mapping information.
     */
    FdoPtr<FdoPhysicalSchemaMappingCollection> mSchemaMappingCollection;

    /**
     * Cached RDBMS-specific information:
     */
    LONG m_lCachedRdbmsId;
    LONG m_lCachedRdbmsBehaviours;
    FdoStringP m_sCachedRdbmsSystemTablePrefix;

    /*
     * The current transaction.
     */
    ArcSDETransaction* mTransaction; // can't use FdoPtr<> because of circular reference

    /*
     * Cached server at connection time.
     */
    CHAR m_mbServer[SE_MAX_SERVER_LEN];

    /*
     * Cached instance at connection time.
     */
    CHAR m_mbInstance[SE_MAX_INSTANCE_LEN];

    /*
     * Cached database (empty if database not provided) at connection time.
     */
    CHAR m_mbDatabaseName[SE_MAX_DATABASE_LEN];

    /*
     * Cached user name at connection time.
     */
    CHAR m_mbUserName[SE_MAX_DATABASE_LEN];

    /*
     * Cached password at connection time.
     */
    CHAR m_mbPassword[SE_MAX_PASSWORD_LEN];


	/*
	 * object handle to create uuids used by uuid types
	 */ 
	SE_UUIDGENERATOR m_uuidGenerator;

	/*
	 * Set if the generator object is initialized
	 */
	bool m_uuidGeneratorCreated;

public:

    /*
     * Geometry conversion buffers, for performance improvements.
     * Also cached geometry factory.
     */
    FdoFgfGeometryFactory *mGeomFactory;
	double *mGeomBuffer_ordinates;
	unsigned long mGeomBuffer_ordinates_cursize;
    LONG *mGeomBuffer_part_offsets;
	unsigned long mGeomBuffer_part_offsets_cursize;
    LONG *mGeomBuffer_subpart_offsets;
	unsigned long mGeomBuffer_subpart_offsets_cursize;
    LONG *mGeomBuffer_offsets;
	unsigned long mGeomBuffer_offsets_cursize;
    SE_POINT *mGeomBuffer_pointsXY;
	unsigned long mGeomBuffer_pointsXY_cursize;
    LFLOAT *mGeomBuffer_pointsZ;
	unsigned long mGeomBuffer_pointsZ_cursize;
    LFLOAT *mGeomBuffer_pointsM;
	unsigned long mGeomBuffer_pointsM_cursize;


public:
     ArcSDEConnection (void);
     virtual ~ArcSDEConnection(void);

    // FdoIDisposable interface

    // <summary>Dispose this object.</summary>
    // <returns>Returns nothing</returns> 
    void Dispose ();

    // FdoIConnection overrides

    /// <summary>Gets an FdoIConnectionCapabilities interface describing the capabilities
    /// of the connection.</summary>
    /// <returns>Returns the connection capabilities</returns> 
    FdoIConnectionCapabilities* GetConnectionCapabilities ();

    /// <summary>Gets an FdoISchemaCapabilities interface describing the provider's support for the feature schema.</summary>
    /// <returns>Returns schema capabilities</returns> 
    FdoISchemaCapabilities* GetSchemaCapabilities ();

    /// <summary>Gets an FdoICommandCapabilities interface describing the commands a provider supports.</summary>
    /// <returns>Returns the command capabilities</returns> 
    FdoICommandCapabilities* GetCommandCapabilities ();

    /// <summary>Gets an FdoIFilterCapabilities interface describing the provider's support for filters.</summary>
    /// <returns>Returns the filter capabilities</returns> 
    FdoIFilterCapabilities* GetFilterCapabilities ();

    /// <summary>Gets an FdoIExpressionCapabilities interface describing the provider's support for expressions.</summary>
    /// <returns>Returns the expression capabilities</returns> 
    FdoIExpressionCapabilities* GetExpressionCapabilities ();

    /// <summary>Gets an FdoIRasterCapabilities interface describing the provider's support for raster images.</summary>
    /// <returns>Returns the raster capabilities</returns> 
    FdoIRasterCapabilities* GetRasterCapabilities ();

    /// <summary>Gets an FdoITopologyCapabilities interface describing the provider's support for topology.</summary>
    /// <returns>Returns the topology capabilities</returns> 
    FdoITopologyCapabilities* GetTopologyCapabilities ();

    /// <summary>Gets an FdoIGeometryCapabilities interface describing the provider's support for geometry.</summary>
    /// <returns>Returns the geometry capabilities</returns> 
    virtual FdoIGeometryCapabilities* GetGeometryCapabilities();

    /// <summary>Gets the connection string used to open a DataStore.</summary>
    /// <returns>Returns the connection string</returns> 
    FdoString* GetConnectionString ();

    /// <summary>Sets the connection string used to open a DataStore. SetConnectionString can only be set while the
    /// connection is closed.</summary>
    /// <param name="value">Input the connection string</param> 
    /// <returns>Returns nothing</returns> 
    void SetConnectionString (FdoString* value);

    /// <summary>Gets an FdoIConnectionInfo interface that can be used to interrogate and set connection properties.</summary>
    /// <returns>Returns the connection info</returns> 
    FdoIConnectionInfo* GetConnectionInfo ();

    /// <summary>Gets the current state of the connection.</summary>
    /// <returns>Returns the current state of the connection</returns> 
    FdoConnectionState GetConnectionState ();

    /// <summary>Gets the number of milliseconds to wait while trying to establish a
    /// connection before terminating the attempt and generating an error. If
    /// the provider does not support the timeout capability 0 will be returned</summary>
    /// <returns>Returns the time to wait (in milliseconds)</returns> 
    FdoInt32 GetConnectionTimeout ();

    /// <summary>Sets the number of milliseconds to wait while trying to establish a
    /// connection before terminating the attempt and generating an error. If
    /// the provider does not support the timeout capability then attempting to
    /// set a timeout will result in an exception</summary>
    /// <param name="value">Input the time to wait (in milliseconds)</param> 
    /// <returns>Returns nothing</returns> 
    void SetConnectionTimeout (FdoInt32 value);

    /// <summary>Opens a feature connection with the settings specified by the
    /// ConnectionString attribute of the provider-specific feature connection
    /// object.</summary>
    /// <returns>Returns nothing</returns> 
    FdoConnectionState Open ();

    /// <summary>Closes the connection to the DataStore</summary>
    /// <returns>Returns nothing</returns> 
    void Close ();

    /// <summary>Begins a transaction and returns an object that realizes
    /// FdoITransaction.</summary>
    /// <returns>Returns the transaction</returns> 
    FdoITransaction* BeginTransaction ();

    /// <summary>Creates and returns the specified type of command object associated with
    /// the connection.</summary>
    /// <param name="commandType">Input the command type to be created</param> 
    /// <returns>Returns the command</returns> 
    FdoICommand* CreateCommand (FdoInt32 commandType);

    /// <summary>Factory function that creates an empty Schema Override set specific
    /// to this FDO Provider.</summary>
    /// <returns>Returns FdoPhysicalSchemaMapping</returns> 
    FdoPhysicalSchemaMapping* CreateSchemaMapping();

    /// <summary>Sets the XML configuration stream used to configure the Data Store. 
    /// SetConfiguration can only be called while the connection is closed.</summary>
    /// <param name="configStream">Input the XML configuration stream</param> 
    /// <returns>Returns nothing</returns> 
    void SetConfiguration(FdoIoStream* configStream);

    //
    // non-exposed functionality
    //

    /// <summary>Get at the underlying SDE connection.</summary>
    /// <returns>Returns the ArcSDE connection object.</returns> 
    //NOTE: this method is exposed for unit test purposes; it isn't accessible to end users since ArcSDEConnection.h is not published
    FDOSDE_API SE_CONNECTION GetConnection ();  

    // Establish the initial connection to ArcSDE itself:
    LONG ConnectToArcSDE(const CHAR* server, const CHAR* instance, const CHAR* datastore,
                           const CHAR* username, const CHAR* password, SE_ERROR *error, SE_CONNECTION *connection);


    // Returns the (only) class definition having the specified name.
    // Returns NULL if no class with the specified name is found in the current connection.
    FdoClassDefinition* GetRequestedClassDefinition (FdoIdentifier* name);

    // Returns the current schema.
    FdoFeatureSchemaCollection* GetSchemaCollection (FdoIdentifier* name = NULL, bool bAutoLoad = true, bool* bIsFullyLoaded=NULL);

    // Stores the schema collection as the current schema collection.
    void SetSchemaCollection (FdoFeatureSchemaCollection* schemaCollection, bool bFullyLoaded);

    // Returns the current schema mapping collection.
    FdoPhysicalSchemaMappingCollection* GetSchemaMappingCollection (FdoString* fdoSchemaName=NULL, FdoString* fdoClassName=NULL, bool bAutoLoad = true);

    // Sets the current schema mapping collection.
    void SetSchemaMappingCollection (FdoPhysicalSchemaMappingCollection* schemaCollection);

    // Empty all schema-related caches.  This is called by ApplySchema and when the connection is closed.
    void DecacheSchema();

    // Empty spatial context cache.  This is called by Create/DestroySpatialContext and when the connection is closed.
    void DecacheSpatialContexts();

    // Schema Mapping utility methods:
    ArcSDESchemaMapping* GetSchemaMapping (FdoString *fdoSchemaName, FdoString* fdoClassName=NULL, bool bAutoLoad=true);
    ArcSDEClassMapping* GetClassMapping (FdoString *fdoSchemaName, FdoString *fdoClassName, bool bAutoLoad=true);
    ArcSDEPropertyMapping* GetPropertyMapping (FdoClassDefinition* definition, FdoString *fdoPropertyName, bool bAutoLoad=true);
    ArcSDEIndexMapping* GetIndexMapping (FdoString *fdoSchemaName, FdoString *fdoClassName, FdoString *arcSdeIndexName);
    void ClassToTable (CHAR* table, FdoClassDefinition* definition);
    void PropertyToColumn (CHAR* column, FdoClassDefinition* definition, FdoIdentifier* property);
    FdoClassDefinition* TableToClass (FdoString* wQualifiedTableName);
    FdoString* ColumnToProperty (FdoClassDefinition* definition, FdoString* columnName, bool bAutoLoad=true);

    // Schema utility methods:
    FdoPropertyDefinition* GetProperty (FdoClassDefinition* definition, FdoString *fdoPropertyName);
    // Returns the user name corresponding to a given FDO class;
    // NOTE: userName must already be of size >= SE_MAX_OWNER_LEN+1
    void ClassToUser(CHAR* userName, FdoClassDefinition* classDef);
    // Returns the database name corresponding to a given FDO class;
    // NOTE: databaseName must already be of size >= SE_MAX_DATABASE_LEN+1
    void ClassToDatabase(CHAR* databaseName, FdoClassDefinition* classDef);

    // long transaction support
    LONG GetActiveVersion ();
    void SetActiveVersion (LONG id);
    LONG GetActiveState ();
    void SetActiveState (LONG id);
    virtual void Flush (); // save current edit session

    // Spatial context support:
    FdoString* GetActiveSpatialContext(void);
    void SetActiveSpatialContext(FdoString* name);
    void SetActiveSpatialContextToDefault(void);

    // RDBMS-specific info:
    void GetRdbmsInfo(void);
    bool RdbmsHasBehaviour(LONG lBehaviour);
    long RdbmsId(void);
    int RdbmsNamesMatch(const char* name1, const char* name2);
    int RdbmsNamesMatch(const wchar_t* name1, const wchar_t* name2);
    FdoString* RdbmsSystemTablePrefix();

    // Transaction support
    FdoITransaction* GetTransaction ();
    void SetTransaction (FdoITransaction* transaction);

	// Get cached registration list:
	void GetArcSDERegistrationList(SE_REGINFO** registrations, long *count);

    // Get cached metadata list:
    void GetArcSDEMetadataList(SE_METADATAINFO** pMetadataList, long *pCount);

    // Get cached layer information:
    long GetArcSDELayerInfo(SE_LAYERINFO &pLayerInfo, const CHAR* tableName, const CHAR* columnName);

    // Get cached spatial reference information;
    // NOTE: when retrieving SRIDs, if *pSpatialRefSrids!=NULL then rely on the SRIDs returned in pSpatialRefSrids as opposed
    // to those in pSpatialRefInfo since they may not be set correctly:
    void GetArcSDESpatialRefList(SE_SPATIALREFINFO** pSpatialRefInfoList, LONG** pSpatialRefSridList, long *count);

    // Returns true if-and-only-if the given schemaname and classname are not NULL and have already been cached:
    bool ClassAlreadyLoaded(FdoString* fdoSchemaName, FdoString* fdoClassName);

    // Returns the proper system column name to use depending on which RDBMS is in use:
    FdoStringP AdjustSystemColumnName(FdoString *name);


	void GetUuidGenerator(SE_UUIDGENERATOR &uuidGenerator);

    // Creates a log for the given table.
    void MakeLog (SE_LOG* log, const CHAR* table);
};

#endif // ARCSDECONNECTION_H


