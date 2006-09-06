//  
//  Copyright (C) 2004-2006  Autodesk, Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//  
#ifndef SDFCONNECTION_H
#define SDFCONNECTION_H

//#include "SdfExtHashMap.h"


#include "RTree.h"
#include "SDF/SdfCompareHandler.h"
#include "SdfSchemaMergeContext.h"

//forward declare
class SdfConnectionInfo;
class PropertyIndex;
class SchemaDb;
class DataDb;
class KeyDb;

class SdfConnection : public FdoIConnection, public SdfISchemaMergeContextFactory
{

    //-------------------------------------------------------
    // Constructor / destructor
    //-------------------------------------------------------

public:

    SDF_API SdfConnection();

protected:
    SDF_API ~SdfConnection();

public:

    SDF_API static SdfConnection* Create();


    //-------------------------------------------------------
    // FdoIDisposable implementation
    //-------------------------------------------------------

protected:
    // dispose this object
    SDF_API virtual void Dispose();

    //-------------------------------------------------------
    // FdoIConnection implementation
    //-------------------------------------------------------

public:
    // Returns an IConnectionCapabilities interface describing the
    // capabilities of the connection.
    SDF_API virtual FdoIConnectionCapabilities* GetConnectionCapabilities();

    // Returns an ISchemaCapabilities interface describing the
    // provider's support for the feature schema.
    SDF_API virtual FdoISchemaCapabilities* GetSchemaCapabilities();

    // Returns an ICommandCapabilities interface describing the
    // commands a provider supports.
    SDF_API virtual FdoICommandCapabilities* GetCommandCapabilities();

    // Returns an IFilterCapabilities interface describing the
    // provider's support for filters.
    SDF_API virtual FdoIFilterCapabilities* GetFilterCapabilities();

    // Returns an IExpressionCapabilities interface describing the
    // provider's support for expressions.
    SDF_API virtual FdoIExpressionCapabilities* GetExpressionCapabilities();

    /// <summary>Gets an FdoIRasterCapabilities interface describing the provider's support for raster images.</summary>
    /// <returns>Returns the raster capabilities</returns> 
    SDF_API virtual FdoIRasterCapabilities* GetRasterCapabilities();

    /// <summary>Gets an FdoITopologyCapabilities interface describing the provider's support for topology.</summary>
    /// <returns>Returns the topology capabilities</returns> 
    SDF_API virtual FdoITopologyCapabilities* GetTopologyCapabilities();

    /// <summary>Gets an FdoIGeometryCapabilities interface describing the provider's support for geometry.</summary>
    /// <returns>Returns the geometry capabilities</returns> 
    virtual FdoIGeometryCapabilities* GetGeometryCapabilities();
 
    // Gets the connection string used to open a data store.
    SDF_API virtual FdoString* GetConnectionString();

    // Sets the connection string used to open a data store.  Can
    // only be set while the connection is closed.
    SDF_API virtual void SetConnectionString(FdoString* value);

    // Returns an IConnectionInfo interface that can be used to
    // interrogate and set connection properties.
    SDF_API virtual FdoIConnectionInfo* GetConnectionInfo();

    // Gets the current state of the connection.
    SDF_API virtual FdoConnectionState GetConnectionState();

    // Gets the number of milliseconds to wait while trying to establish a
    // connection before terminating the attempt and generating an error.  If
    // the provider does not support the timeout capability 0 will be returned.
    SDF_API virtual FdoInt32 GetConnectionTimeout();

    // Sets the number of milliseconds to wait while trying to establish a
    // connection before terminating the attempt and generating an error.  If
    // the provider does not support the timeout capability then attempting
    // to set a timeout will result in an exception.
    SDF_API virtual void SetConnectionTimeout(FdoInt32 value);

    // Opens a feature connection with the settings specified by the
    // ConnectionString attribute of the provider-specific feature connection
    // object.
    SDF_API virtual FdoConnectionState Open();

    // Closes the connection to the data store.
    SDF_API virtual void Close();

    // Begins a transaction and returns an ITransaction object.
    SDF_API virtual FdoITransaction* BeginTransaction();

    // Creates and returns the specified type of command object
    // associated with the connection.
    SDF_API virtual FdoICommand* CreateCommand(FdoInt32 commandType);

    /// <summary>Factory function that creates an empty Schema Override set specific
    /// to this FDO Provider.</summary>
    /// <returns>Returns FdoPhysicalSchemaMapping</returns> 
    SDF_API virtual FdoPhysicalSchemaMapping* CreateSchemaMapping();

    SDF_API virtual void SetConfiguration(FdoIoStream* stream);
public:

    // Updates the connection string without doing validation or notification.
    void UpdateConnectionString();
    void SetCreateSDF(bool bCreate = true){m_bCreate = bCreate;};
    bool GetCreateSDF(){return m_bCreate;};
    void FlushAll( FdoClassDefinition *clas, bool forUpdate=false );
    char* GetFilename();
    char* GetPath();
    bool GetReadOnly();
    DataDb* GetDataDb(FdoClassDefinition* clas);
    KeyDb* GetKeyDb(FdoClassDefinition* clas);
    SchemaDb* GetSchemaDb();
    FdoFeatureSchema* GetSchema();
    void SetSchema(FdoFeatureSchema* schema, bool ignoreStates);
    SdfRTree* GetRTree(FdoClassDefinition* clas);
    PropertyIndex* GetPropertyIndex(FdoClassDefinition* clas);

    FdoByteArray* GetDataExtents();

    SQLiteDataBase* GetDataBase() { return m_env; }

    // Get the major/minor verion of the SDF file we are currently connected to:
    static bool VersionIsAtLeast(unsigned char actualMajorVersion, unsigned char actualMinorVersion, unsigned char requiredMajorVersion, unsigned char requiredMinorVersion);
    static bool VersionIsAtMost(unsigned char actualMajorVersion, unsigned char actualMinorVersion, unsigned char requiredMajorVersion, unsigned char requiredMinorVersion);

    // Used to re-sync with the data on disk. This is needed if 2 or more connections are writting to the same file
    // Before a new connection writes to the file it needs to re-synch with the id pool. Otherwise the ids may overlap
    // with the existing ids.
    void ReSyncData();

    FdoConnectionState Open( SdfCompareHandler* cmpHandler );

    DataDb* CreateNewDataDb( FdoClassDefinition* clas );
    DataDb* CreateNewDataDb( FdoClassDefinition* clas, FdoString* dbName, PropertyIndex* pi );

    // Creates a context for merging a schema, passed to the ApplySchema command,
    // into the current schemas.
    virtual SdfSchemaMergeContext* CreateMergeContext( 
        FdoFeatureSchemaCollection* oldSchemas,         // current schema
        FdoFeatureSchema* newSchema,                    // applied schema
        bool ignoreStates                               // See FdoIApplySchema
    );

private:

    void CloseDatabases();

    void InitDatabases();
    void DestroyDatabases();

    void RegenIndex( FdoClassDefinition *clas, KeyDb* keys, DataDb  *dataDb );

    SQLiteDataBase* m_env;
    
    //path for temporary db environment files
    char* m_mbsEnvPath;
    
    //full absolute path to SDF file, including name and extension
    char* m_mbsFullPath;
  
    bool m_bNoEnvPath;

    bool m_bReadOnly;

    FdoConnectionState m_connState;
    SdfConnectionInfo* m_connInfo;

    SchemaDb* m_dbSchema;
    
    FdoPtr<SdfCompareHandler> m_CompareHandler;

    bool m_bCreate;
    /*
    stdext::hash_map<FdoClassDefinition*, SdfRTree*> m_hRTrees;    
    stdext::hash_map<FdoClassDefinition*, PropertyIndex*> m_hPropertyIndices;
    stdext::hash_map<FdoClassDefinition*, DataDb*> m_hDataDbs;
    stdext::hash_map<FdoClassDefinition*, KeyDb*> m_hKeyDbs;
    */
    //Note: can't use above because g++ complains under Linux, because
    //hash_map is non-standard so they haven't bothered implementing
    //it for pointers -- see beginning of this file for implementation
    //of hash for void*
    stdext::hash_map<void*, void*> m_hRTrees;    
    stdext::hash_map<void*, void*> m_hPropertyIndices;
    stdext::hash_map<void*, void*> m_hDataDbs;
    stdext::hash_map<void*, void*> m_hKeyDbs;
protected:
    /**
     * The connection string containing details about how to connect to Shp.
     */
    FdoStringP mConnectionString;
};


#endif
