/*
 * 
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
* 
 */

#ifndef SHPCONNECTION_H
#define SHPCONNECTION_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include <ShpFileSet.h>
#include <ShpSpatialContext.h>
#include <ShpLpFeatureSchema.h>

#include <FdoCommonThreadMutex.h>

class ShpConnection : public FdoIConnection
{
    friend class ShpDescribeSchemaCommand;
    friend class ShpApplySchemaCommand;
    friend class ShpSelectCommand;
    friend class ShpSelectAggregates;
    friend class ShpGetSpatialContextsCommand;
    friend class ShpQueryOptimizer;
    friend class ShpFeatureReader;

protected:
    /**
     * The connection string containing details about how to connect to Shp.
     */
    FdoStringP mConnectionString;

    /**
     * The connection state (closed, pending, open).
     */
    FdoConnectionState mConnectionState;

    /**
     * The active spatial context.
     */
    FdoStringP mActiveSpatialContext;

    /**
     * Connection information object.
     */
    FdoPtr<FdoIConnectionInfo> mConnectionInfo;

    /**
     * The directory where the file sets are.
     */
    FdoStringP mDirectory;

    /**
     * The specified fileset the user connected to (set to "" if specified a directory or if specified nothing).
     */
    FdoStringP mFile;

    /**
     * A temporary directory or L"" if none is specified.
     */
    FdoStringP mTemporary;

	/**
	 * Last edited fileset
	 */
	ShpFileSet *mLastEditedFileSet;

    /**
     * Configuration file elements (logical schema + schema mappings + singleton spatial context):
     */
	FdoFeatureSchemasP mConfigLogicalSchemas;
	FdoSchemaMappingsP mConfigSchemaMappings;
 	ShpSpatialContextCollectionP mSpatialContextColl;

    /**
     * Cached Logical/Physical, and Physical schemas:
     */
    ShpLpFeatureSchemaCollectionP mLpSchemas;
    ShpPhysicalSchemaP            mPhysicalSchema;

    /**
     * Flag true when configuration file is specified.
     */
    bool mConfigured;

	/**
	 * Globals to keep track of opened connections and the files to compress after
	 * delete command. The compression is triggered on the last connection close().
	 */
	static	FdoCommonThreadMutex mMutex;
	static	int mGlobalRefCount;


private:

	/// <summary>Sets the connection paths
	/// the File, Directory and Temporary</summary>
	/// <param name="value">None</param> 
	/// <returns>Returns nothing</returns> 
	void InitConnectionPaths();

	/// <summary>Compresses all the file sets</summary>
	/// <returns>Returns nothing</returns> 
	void CompressFileSets ();

	/// <summary>Compresses a file set (eliminates the deleted rows)</summary>
	/// <param name="baseName">Fileset full path</param> 
	/// <returns>Returns nothing</returns> 
	void CompressFileSet (const wchar_t* baseName);

public:
    ShpConnection (void);
    virtual ~ShpConnection(void);

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


    ////////////////////////////////////////////////////////////////////////
    // Non-published schema management APIs:
    ////////////////////////////////////////////////////////////////////////

    ShpLpFeatureSchemaCollection* GetLpSchemas(void);
    ShpLpFeatureSchemaCollection* GetLpSchema(const wchar_t *logicalSchemaName);

    ShpPhysicalSchema* GetPhysicalSchema(void);


    ////////////////////////////////////////////////////////////////////////
    // Non-published misc APIs:
    ////////////////////////////////////////////////////////////////////////

	ShpSpatialContextCollection* GetSpatialContexts(bool bDynamic = false);
    FdoString* GetDirectory ();
    FdoString* GetFile ();
    FdoString* GetTemporary (); // this will return NULL if no temporary directory was specified
    bool IsConfigured (); // true if SetConfiguration() was done by the client
	void SetLastEditedFileSet( ShpFileSet *fileset ) { mLastEditedFileSet = fileset; };
	ShpFileSet *GetLastEditedFileSet() { return mLastEditedFileSet; };
	void AddPhysicalShapefileNames (FdoStringsP& physicalShapefileNames);
};

#endif // SHPCONNECTION_H
