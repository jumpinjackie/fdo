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

#include "stdafx.h"
#include <FdoCommonConnStringParser.h>
#include "ArcSDEProvider.h"
#include "ArcSDEUtils.h"

#ifdef _WIN32
#include <DelayImp.h>  // For delay-load types
#endif

// external access to connection for client services
extern "C" FDOSDE_API FdoIConnection* CreateConnection ()
{
   return (new ArcSDEConnection ());
}

ArcSDEConnection::ArcSDEConnection (void) :
    mConnectionString ((wchar_t*)NULL),
    mConnection (NULL),
    mPartialConnection (false),
    mActiveVersion (-1L),
    mActiveState (-1L),
    m_lCachedRdbmsId (-2L),
    m_lCachedRdbmsBehaviours (0L),
    mTransaction (NULL),
	mSchemaCollectionFullyLoaded (false),
	mCachedRegistrations(NULL),
    mCachedRegistrationCount(0),
    mCachedMetadataListCount(0),
    mCachedMetadataList(0),
    mCachedLayerList(NULL),
    mCachedLayerListCount(0),
    mCachedSpatialRefList(NULL),
    mCachedSpatialRefSRIDList(NULL),
    mCachedSpatialRefListCount(0),
	mGeomBuffer_ordinates(NULL),
	mGeomBuffer_ordinates_cursize(0),
    mGeomBuffer_part_offsets(NULL),
	mGeomBuffer_part_offsets_cursize(0),
    mGeomBuffer_subpart_offsets(NULL),
	mGeomBuffer_subpart_offsets_cursize(0),
    mGeomBuffer_offsets(NULL),
	mGeomBuffer_offsets_cursize(0),
    mGeomBuffer_pointsXY(NULL),
	mGeomBuffer_pointsXY_cursize(0),
    mGeomBuffer_pointsZ(NULL),
	mGeomBuffer_pointsZ_cursize(0),
    mGeomBuffer_pointsM(NULL),
	mGeomBuffer_pointsM_cursize(0),
    mGeomFactory(NULL)
{
    mGeomFactory = FdoFgfGeometryFactory::GetInstance();
}

ArcSDEConnection::~ArcSDEConnection (void)
{
    if (mConnection != NULL)
    {
        /* ToDo: unlock?*/

        if (NULL != mTransaction && !mTransaction->IsSpent ())
            mTransaction->Rollback ();
        Close ();
    }
    else
        if (NULL != mTransaction)
        {
            mTransaction->SetConnection (NULL);
            mTransaction->Release ();
        }

    // Free up geometry-conversion buffers;
    // NOTE: we don't need to free up these buffers on Close() since they are not database-specific:
    FDO_SAFE_RELEASE(mGeomFactory);
	if (mGeomBuffer_part_offsets != NULL)
        free(mGeomBuffer_part_offsets);
    if (mGeomBuffer_subpart_offsets != NULL)
        free(mGeomBuffer_subpart_offsets);
	if (mGeomBuffer_offsets != NULL)
        free(mGeomBuffer_offsets);
    if (mGeomBuffer_pointsXY != NULL)
        free(mGeomBuffer_pointsXY);
    if (mGeomBuffer_pointsZ != NULL)
        free(mGeomBuffer_pointsZ);
    if (mGeomBuffer_pointsM != NULL)
        free(mGeomBuffer_pointsM);
}

// <summary>Dispose this object.</summary>
// <returns>Returns nothing</returns> 
void ArcSDEConnection::Dispose ()
{
    delete this;
}

/// <summary>Gets an FdoIConnectionCapabilities interface describing the capabilities
/// of the connection.</summary>
/// <returns>Returns the connection capabilities</returns> 
FdoIConnectionCapabilities* ArcSDEConnection::GetConnectionCapabilities ()
{
    return (new ArcSDEConnectionCapabilities ());
}

/// <summary>Gets an FdoISchemaCapabilities interface describing the provider's support for the feature schema.</summary>
/// <returns>Returns schema capabilities</returns> 
FdoISchemaCapabilities* ArcSDEConnection::GetSchemaCapabilities ()
{
    return (new ArcSDESchemaCapabilities ());
}

/// <summary>Gets an FdoICommandCapabilities interface describing the commands a provider supports.</summary>
/// <returns>Returns the command capabilities</returns> 
FdoICommandCapabilities* ArcSDEConnection::GetCommandCapabilities ()
{
    return (new ArcSDECommandCapabilities ());
}

/// <summary>Gets an FdoIFilterCapabilities interface describing the provider's support for filters.</summary>
/// <returns>Returns the filter capabilities</returns> 
FdoIFilterCapabilities* ArcSDEConnection::GetFilterCapabilities ()
{
    return (new ArcSDEFilterCapabilities ());
}

/// <summary>Gets an FdoIExpressionCapabilities interface describing the provider's support for expressions.</summary>
/// <returns>Returns the expression capabilities</returns> 
FdoIExpressionCapabilities* ArcSDEConnection::GetExpressionCapabilities ()
{
    return (new ArcSDEExpressionCapabilities ());
}

class ArcSDERasterCapabilities : public FdoIRasterCapabilities
{
    virtual bool SupportsRaster () { return (false); }
    virtual bool SupportsStitching () { return (false); }
    virtual bool SupportsSubsampling () { return (false); }
    virtual bool SupportsDataModel (FdoRasterDataModel* model) { return (false); }
    virtual void Dispose () { delete this; }
};

/// <summary>Gets an FdoIRasterCapabilities interface describing the provider's support for raster images.</summary>
/// <returns>Returns the raster capabilities</returns> 
FdoIRasterCapabilities* ArcSDEConnection::GetRasterCapabilities (void)
{
    return (new ArcSDERasterCapabilities ());
}

class ArcSDETopologyCapabilities : public FdoITopologyCapabilities
{
public:
    virtual bool SupportsTopology () { return (false); }
    virtual bool SupportsTopologicalHierarchy () { return (false); }
    virtual bool BreaksCurveCrossingsAutomatically () { return (false); }
    virtual bool ActivatesTopologyByArea (){ return (false); }
    virtual bool ConstrainsFeatureMovements (){ return (false); }
    virtual void Dispose () { delete this; }
};

/// <summary>Gets an FdoITopologyCapabilities interface describing the provider's support for topology.</summary>
/// <returns>Returns the topology capabilities</returns> 
FdoITopologyCapabilities* ArcSDEConnection::GetTopologyCapabilities ()
{
    return (new ArcSDETopologyCapabilities ());
}

/// <summary>Gets the connection string used to open a DataStore.</summary>
/// <returns>Returns the connection string</returns> 
FdoString* ArcSDEConnection::GetConnectionString ()
{
    return (mConnectionString);
}

/// <summary>Sets the connection string used to open a DataStore. SetConnectionString can only be set while the
/// connection is closed.</summary>
/// <param name="value">Input the connection string</param> 
/// <returns>Returns nothing</returns> 
void ArcSDEConnection::SetConnectionString (FdoString* value)
{
    if ((GetConnectionState() == FdoConnectionState_Closed) || (GetConnectionState() == FdoConnectionState_Pending))
    {
        // Update the connection string:
        mConnectionString = value;

        // Update the connection property dictionary:
        FdoPtr<FdoIConnectionInfo> connInfo = GetConnectionInfo();
        FdoPtr<FdoCommonConnPropDictionary> connDict = dynamic_cast<FdoCommonConnPropDictionary*>(connInfo->GetConnectionProperties());
        connDict->UpdateFromConnectionString(mConnectionString);
    }
    else
        throw FdoException::Create (NlsMsgGet(ARCSDE_CONNECTION_ALREADY_OPEN, "The connection is already open."));
}

/// <summary>Gets an FdoIConnectionInfo interface that can be used to interrogate and set connection properties.</summary>
/// <returns>Returns the connection info</returns> 
FdoIConnectionInfo* ArcSDEConnection::GetConnectionInfo ()
{
    if (mConnectionInfo == NULL)
        mConnectionInfo = new ArcSDEConnectionInfo (this);
    return FDO_SAFE_ADDREF(mConnectionInfo.p);
}

/// <summary>Gets the current state of the connection.</summary>
/// <returns>Returns the current state of the connection</returns> 
FdoConnectionState ArcSDEConnection::GetConnectionState ()
{
    /*todo: busy?*/
    if (NULL == mConnection)
        return FdoConnectionState_Closed;
    else if (mPartialConnection)
        return FdoConnectionState_Pending;
    else
        return FdoConnectionState_Open;
}

/// <summary>Gets the number of milliseconds to wait while trying to establish a
/// connection before terminating the attempt and generating an error. If
/// the provider does not support the timeout capability 0 will be returned</summary>
/// <returns>Returns the time to wait (in milliseconds)</returns> 
FdoInt32 ArcSDEConnection::GetConnectionTimeout ()
{
    return (0);
}

/// <summary>Sets the number of milliseconds to wait while trying to establish a
/// connection before terminating the attempt and generating an error. If
/// the provider does not support the timeout capability then attempting to
/// set a timeout will result in an exception</summary>
/// <param name="value">Input the time to wait (in milliseconds)</param> 
/// <returns>Returns nothing</returns> 
void ArcSDEConnection::SetConnectionTimeout (FdoInt32 value)
{
    throw FdoException::Create (NlsMsgGet(ARCSDE_CONNECTION_TIMEOUT_UNSUPPORTED, "Connection timeout is not supported."));
}

/// <summary>Opens a feature connection with the settings specified by the
/// ConnectionString attribute of the provider-specific feature connection
/// object.</summary>
/// <returns>Returns nothing</returns> 
FdoConnectionState ArcSDEConnection::Open ()
{
    const wchar_t* connection_string;
    SE_ERROR error;
    SE_CONNECTION connection;
    LONG result;
    FdoConnectionState ret;


    // Disconnect if currently 'partially' connected (but not if already 'fully' connected):
    if (GetConnectionState() == FdoConnectionState_Pending)
        Close();

    // Ensure connection is not already open:
    if (NULL != mConnection)
        throw FdoException::Create (NlsMsgGet(ARCSDE_CONNECTION_ALREADY_OPEN, "The connection is already open."));

    // Validate connection string:
    connection_string = GetConnectionString ();
    if ((NULL == connection_string) || (0 == wcslen (connection_string)))
        throw FdoException::Create (NlsMsgGet(ARCSDE_CONNECTION_STRING_NOT_SET, "Connection string is empty or was not set."));

    FdoPtr<FdoIConnectionInfo> info = GetConnectionInfo ();
    FdoPtr<FdoCommonConnPropDictionary> dictionary = dynamic_cast<FdoCommonConnPropDictionary*>(info->GetConnectionProperties ());

    FdoCommonConnStringParser parser (NULL, connection_string);
    // check the validity of the connection string, i.e. it doesn’t contain unknown properties
    // e.g. DefaultFLocation instead of DefaultFileLocation
    if (!parser.IsConnStringValid())
        throw FdoException::Create (NlsMsgGet1(ARCSDE_INVALID_CONNECTION_STRING, "Invalid connection string '%1$ls'", connection_string));
    if (parser.HasInvalidProperties(dictionary))
        throw FdoException::Create (NlsMsgGet1(ARCSDE_INVALID_CONNECTION_PROPERTY_NAME, "Invalid connection property name '%1$ls'", parser.GetFirstInvalidPropertyName(dictionary)));

    const char* server = parser.GetPropertyValue (CONNECTIONPROPERTY_SERVER);
    const char* instance = parser.GetPropertyValue (CONNECTIONPROPERTY_INSTANCE);
    const char* datastore = parser.GetPropertyValue (CONNECTIONPROPERTY_DATASTORE);
    const char* username = parser.GetPropertyValue (CONNECTIONPROPERTY_USERNAME);
    const char* password = parser.GetPropertyValue (CONNECTIONPROPERTY_PASSWORD);
    
    // Validate that all required connection properties are set:
    FdoInt32 propCount = 0;
    FdoString **propNames = dictionary->GetPropertyNames(propCount);
    for (FdoInt32 i=0; i<propCount; i++)
        if (dictionary->IsPropertyRequired(propNames[i]) && (NULL==parser.GetPropertyValue(propNames[i])))
        {
            if (0 != wcscmp (propNames[i], CONNECTIONPROPERTY_SERVER)) // server is optional at ArcSDE level
                throw FdoException::Create(NlsMsgGet1(ARCSDE_CONNECTION_MISSING_REQUIRED_PROPERTY, "The connection property '%1$ls' is required but wasn't set.", propNames[i]));
        }

    // Handle special datastore case (ArcSDEDefaultDataStore --> NULL database):
    FdoStringP datastoreW = parser.GetPropertyValueW(CONNECTIONPROPERTY_DATASTORE);
	if ((datastoreW.GetLength() != 0) && (0 == wcscmp(datastoreW, ArcSDEDefaultDataStore)))
        datastore = "";

    // Cache datastore & username:
	if (datastore == NULL || strlen(datastore) == 0)
        m_mbDatabaseName[0] = '\0';
    else
        strcpy(m_mbDatabaseName, datastore);
    strcpy(m_mbUserName, username);

    // Attempt to establish initial ArcSDE connection;
    // This is done in a separate method since we need to use __try/__except to catch the delay-loader's
    // Structured Exceptions, which is incompatible with having destructible objects on the stack:
    result = ConnectToArcSDE(server, instance, datastore, username, password, &error, &connection);

    // check for database supplied where none is allowed
    if (SE_DBMS_DOES_NOT_SUPPORT == result)
    {
        result = SE_connection_create (
            server,
            instance,
            NULL,
            username,
            password,
            &error,
            &connection);
    }
    handle_sde_err<FdoConnectionException>(connection, result, __FILE__, __LINE__, ARCSDE_OPEN_FAILED, "Failed to open ArcSDE connection.");

    // Cache connection:
    mConnection = connection;
    mPartialConnection = (datastoreW.GetLength() == 0);
    if (GetConnectionState() == FdoConnectionState_Open)
    {
        SetActiveVersion (SDE_DEFAULT);
        SetActiveSpatialContextToDefault();
    }

    // Update the Datastore connection property's enumerated list of datastores:
    LONG lNumDatabases = 0L;
    char** strDatabaseNames = NULL;
    result = SE_database_list(mConnection, &lNumDatabases, &strDatabaseNames);
    FdoPtr<ConnectionProperty> datastoreProperty = dictionary->FindProperty(CONNECTIONPROPERTY_DATASTORE);
    if (0L==lNumDatabases)
    {
        // Create an enum list with a single "Default Datastore" entry:
        wchar_t **enumArray = new wchar_t*[1];
        enumArray[0] = new wchar_t[wcslen(ArcSDEDefaultDataStore)+1];
        wcscpy(enumArray[0], ArcSDEDefaultDataStore);
        datastoreProperty->UpdateEnumerableProperties(1, (const wchar_t**)enumArray);
    }
    else
    {
        // Create an enum list with the list of ArcSDE databases:
        wchar_t **enumArray = new wchar_t*[lNumDatabases];
        for (int i=0; i<lNumDatabases; i++)
        {
            wchar_t *wDatabaseName = NULL;
            multibyte_to_wide(wDatabaseName, strDatabaseNames[i]);
            enumArray[i] = new wchar_t[wcslen(wDatabaseName)+1];
            wcscpy(enumArray[i], wDatabaseName);
        }
        datastoreProperty->UpdateEnumerableProperties((int)lNumDatabases, (const wchar_t**)enumArray);
    }

    ret = GetConnectionState ();

    if (FdoConnectionState_Pending == ret)
        datastoreProperty->SetIsPropertyRequired (true);

    // Validate that this connection is to an RDBMS we support:
    if (FdoConnectionState_Open == ret)
        if ( (RdbmsId() != SE_DBMS_IS_ORACLE) && (RdbmsId() != SE_DBMS_IS_SQLSERVER) )
            throw FdoException::Create (NlsMsgGet1(ARCSDE_RDMS_NOT_SUPPORTED, "The RDBMS '%1$ls' is not supported by this version of ArcSDE Provider.", RdbmsToString(RdbmsId())));

    return (ret);
}

/// <summary>Closes the connection to the DataStore</summary>
/// <returns>Returns nothing</returns> 
void ArcSDEConnection::Close ()
{
	// Clean up:
    DecacheSchema();
    DecacheSpatialContexts();
    if (NULL != mConnection)
    {
        SetActiveVersion (-1L);
        SE_connection_free (mConnection);
        mConnection = NULL;
    }
    mPartialConnection = false;
    m_lCachedRdbmsId = -2;  // -2 means not-yet-cached
    m_sCachedRdbmsSystemTablePrefix = L"";
    m_mbDatabaseName[0] = '\0';
    m_mbUserName[0] = '\0';
    SetTransaction (NULL);

    // Clear the Datastore connection property's enumerated list of datastores:
	FdoPtr<FdoIConnectionInfo> info = GetConnectionInfo ();
	FdoPtr<FdoCommonConnPropDictionary> dictionary = dynamic_cast<FdoCommonConnPropDictionary*>(info->GetConnectionProperties ());
	FdoPtr<ConnectionProperty> datastoreProperty = dictionary->FindProperty(CONNECTIONPROPERTY_DATASTORE);
    datastoreProperty->UpdateEnumerableProperties(0, NULL);
    datastoreProperty->SetIsPropertyRequired (false);
}

/// <summary>Begins a transaction and returns an object that realizes
/// FdoITransaction.</summary>
/// <returns>Returns the transaction</returns> 
FdoITransaction* ArcSDEConnection::BeginTransaction ()
{
    if (mTransaction != NULL)
        throw FdoException::Create(NlsMsgGet(ARCSDE_NESTED_TRANSACTIONS_NOT_SUPPORTED, "Nested transactions are not supported."));
    mTransaction = new ArcSDETransaction ();
    // the connection holds a hard reference to the transaction
    // the transaction holds a soft reference to the connection
    mTransaction->AddRef ();
    mTransaction->SetConnection (this);

    return (mTransaction);
}

/// <summary>Creates and returns the specified type of command object associated with
/// the connection.</summary>
/// <param name="commandType">Input the command type to be created</param> 
/// <returns>Returns the command</returns> 
FdoICommand* ArcSDEConnection::CreateCommand (FdoInt32 commandType)
{
    FdoPtr<FdoICommand> ret;

    switch (commandType)
    {
        case FdoCommandType_Select:
            ret = new ArcSDESelectCommand (this);
            break;
        case FdoCommandType_SelectAggregates:
            ret = new ArcSDESelectAggregatesCommand (this);
            break;
        case FdoCommandType_Insert:
            ret = new ArcSDEInsertCommand (this);
            break;
        case FdoCommandType_Delete:
            ret = new ArcSDEDeleteCommand (this);
            break;
        case FdoCommandType_Update:
            ret = new ArcSDEUpdateCommand (this);
            break;
        case FdoCommandType_DescribeSchema:
            ret = new ArcSDEDescribeSchemaCommand (this);
            break;
#ifdef _DEBUG  // Only allow ApplySchema/DestroySchema for test suite purposes
        case FdoCommandType_ApplySchema:
            ret = new ArcSDEApplySchemaCommand (this);
            break;
        case FdoCommandType_DestroySchema:
            ret = new ArcSDEDestroySchemaCommand (this);
            break;
#endif //_DEBUG
        case FdoCommandType_SQLCommand:
            ret = new ArcSDESQLCommand (this);
            break;
        case FdoCommandType_AcquireLock:
            ret = new ArcSDEAcquireLockCommand (this);
            break;
        case FdoCommandType_GetLockInfo:
            ret = new ArcSDEGetLockInfoCommand (this);
            break;
        case FdoCommandType_GetLockedObjects:
            ret = new ArcSDEGetLockedObjectsCommand (this);
            break;
        case FdoCommandType_GetLockOwners:
            ret = new ArcSDEGetLockOwnersCommand (this);
            break;
        case FdoCommandType_ReleaseLock:
            ret = new ArcSDEReleaseLockCommand (this);
            break;
        case FdoCommandType_ActivateLongTransaction:
            ret = new ArcSDEActivateLongTransactionCommand (this);
            break;
        case FdoCommandType_DeactivateLongTransaction:
            ret = new ArcSDEDeactivateLongTransactionCommand (this);
            break;
        case FdoCommandType_CommitLongTransaction:
            ret = new ArcSDECommitLongTransactionCommand (this);
            break;
        case FdoCommandType_CreateLongTransaction:
            ret = new ArcSDECreateLongTransactionCommand (this);
            break;
        case FdoCommandType_GetLongTransactions:
            ret = new ArcSDEGetLongTransactionsCommand (this);
            break;
        case FdoCommandType_RollbackLongTransaction:
            ret = new ArcSDERollbackLongTransactionCommand (this);
            break;
        case FdoCommandType_ActivateSpatialContext:
            ret = new ArcSDEActivateSpatialContext (this);
            break;
        case FdoCommandType_CreateSpatialContext:
            ret = new ArcSDECreateSpatialContext (this);
            break;
        case FdoCommandType_DestroySpatialContext:
            ret = new ArcSDEDestroySpatialContext (this);
            break;
        case FdoCommandType_GetSpatialContexts:
            ret = new ArcSDEGetSpatialContexts (this);
            break;
        case FdoCommandType_ListDataStores:
            ret = new ArcSDEListDataStores (this);
            break;

        default:
            throw FdoException::Create (NlsMsgGet1(ARCSDE_COMMAND_NOT_SUPPORTED, "The command %1$d is not supported.", (int)commandType));
    }

    return (FDO_SAFE_ADDREF (ret.p));
}


/// <summary>Factory function that creates an empty Schema Override set specific
/// to this FDO Provider.</summary>
/// <returns>Returns FdoPhysicalSchemaMapping</returns> 
FdoPhysicalSchemaMapping* ArcSDEConnection::CreateSchemaMapping()
{
    return ArcSDESchemaMapping::Create();
}


/// <summary>Get at the underlying SDE connection.</summary>
/// <returns>Returns the ArcSDE connection object.</returns> 
SE_CONNECTION ArcSDEConnection::GetConnection ()
{
    // Since all operations that go to the database must call this method,
    // we can throw appropriate exceptions if the connection is not open or only partially open:
    if (GetConnectionState() == FdoConnectionState_Closed)
        throw FdoException::Create(NlsMsgGet(ARCSDE_CONNECTION_NOT_OPEN, "The connection is currently closed; call the Open method to establish a connection to ArcSDE before attempting any further operations."));
    if (GetConnectionState() == FdoConnectionState_Pending)
        throw FdoException::Create(NlsMsgGet(ARCSDE_CONNECTION_PENDING, "The connection is currently pending; call the Open method again with all required connection properties before attempting any further operations."));

    // If we get here, then we are fully connected; let caller proceed:
    return (mConnection);
}

// Returns the class definition having the specified name.  Throws an exception
// if name is NULL or no class with the specified name is found in the current connection.
// NOTE: this method does NOT clone the class definition, in order to avoid setting the parent to NULL.
//       therefore, the caller should avoid modifying the returned class.
FdoClassDefinition* ArcSDEConnection::GetRequestedClassDefinition (FdoIdentifier* name)
{
    FdoString* schema_name;
    FdoPtr<FdoFeatureSchemaCollection> schemas;
    FdoPtr<FdoFeatureSchema> schema;
    FdoPtr<FdoClassCollection> classes;
    FdoPtr<FdoClassDefinition> classDef;
    FdoPtr<FdoClassDefinition> ret;

    // class name is required
    if (NULL == name)
        throw FdoException::Create (NlsMsgGet(ARCSDE_CLASS_NAME_EMPTY, "Unexpected empty or null FDO class name."));

    // Load information for the requested class only, to optimize performance:
    schemas = GetSchemaCollection (name);

    // get the schema name
    schema = NULL;
    schema_name = name->GetSchemaName ();
    if (0 != wcslen (schema_name))  // fully-qualified identifier
    {
        // check it's a schema
        schema = schemas->FindItem (schema_name);
        if (schema == NULL)
            throw FdoException::Create (NlsMsgGet1 (ARCSDE_SCHEMA_DOES_NOT_EXIST, "Schema '%1$ls' does not exist.", schema_name));

        // get the set of classes for this schema
        classes = schema->GetClasses();

        // check if the set of classes contains the requested class
        for (int j = 0; (j < classes->GetCount ()) && (ret == NULL); j++)
        {
            classDef = classes->GetItem (j);
            if (0 == wcscmp (classDef->GetName (), name->GetName ()))
                ret = classDef;  // automatic addref here, since FdoPtr's are of same type
        }
    }
    else  // not fully-qualified identifier
    {
        for (int i=0; i<schemas->GetCount(); i++)
        {
            schema = schemas->GetItem(i);
            classes = schema->GetClasses();
            for (int j=0; j<classes->GetCount(); j++)
            {
                classDef = classes->GetItem(j);
                if (0 == wcscmp(classDef->GetName(), name->GetName()))
                {
                    if (ret != NULL)
                        throw FdoException::Create (NlsMsgGet1(ARCSDE_SCHEMA_NON_UNIQUE_CLASS_NAME, "The unqualified class name '%1$ls' was found in more than one schema.", name->GetName()));

                    ret = classDef;  // automatic addref here, since FdoPtr's are of same type

                    // NOTE: we don't exit the two for loops here, since we must check for classes
                    //       with the same name in other schemas, in order to throw an exception.
                }
            }
        }
    }

    // Validate that we found the requested class:
    if (ret == NULL)
        throw FdoException::Create (NlsMsgGet1(ARCSDE_FEATURE_CLASS_NOT_FOUND, "FDO Class '%1$ls' not found in schema.", name->GetName()));

    return (FDO_SAFE_ADDREF(ret.p));
}

// Returns the current schema.
FdoFeatureSchemaCollection* ArcSDEConnection::GetSchemaCollection (FdoIdentifier* name, bool bAutoLoad, bool* bIsFullyLoaded)
{
    if (!mSchemaCollectionFullyLoaded && bAutoLoad && (name==NULL || !ClassAlreadyLoaded(name->GetSchemaName(), name->GetName())))
    {
        // use describe schema to get the list of all classes & schema mappings
        FdoPtr<FdoIDescribeSchema> describe = new ArcSDEDescribeSchemaCommand (this, name);
        FdoPtr<FdoFeatureSchemaCollection> dummy = describe->Execute ();
    }

    // Set to empty collection if not set:
	if (mSchemaCollection == NULL)
		mSchemaCollection = FdoFeatureSchemaCollection::Create (NULL);

	if (bIsFullyLoaded!=NULL)
		*bIsFullyLoaded = mSchemaCollectionFullyLoaded;
    return (FDO_SAFE_ADDREF(mSchemaCollection.p));
}

// Stores the schema as the current schema.
void ArcSDEConnection::SetSchemaCollection (FdoFeatureSchemaCollection* schemaCollection, bool bFullyLoaded)
{
    mSchemaCollection = FDO_SAFE_ADDREF(schemaCollection);
	mSchemaCollectionFullyLoaded = bFullyLoaded;
}

// Returns the current schema mapping collection.
FdoPhysicalSchemaMappingCollection* ArcSDEConnection::GetSchemaMappingCollection (FdoString* fdoSchemaName, FdoString* fdoClassName, bool bAutoLoad)
{
    // Create an empty collection if not yet done so.
    if (mSchemaMappingCollection == NULL)
        mSchemaMappingCollection = FdoPhysicalSchemaMappingCollection::Create();

    if (!mSchemaCollectionFullyLoaded && bAutoLoad && !ClassAlreadyLoaded(fdoSchemaName, fdoClassName))
    {
		// If a class name is specified, this is an optimized case:
		FdoPtr<FdoIdentifier> classId;
		if (fdoClassName != NULL)
		{
			FdoStringP qualifiedClassName = FdoStringP::Format(L"%ls:%ls", fdoSchemaName, fdoClassName);
			classId = FdoIdentifier::Create(qualifiedClassName);
		}

        // use describe schema to get the list of all classes & schema mappings
        FdoPtr<FdoIDescribeSchema> describe = new ArcSDEDescribeSchemaCommand (this, classId);
        FdoPtr<FdoFeatureSchemaCollection> dummy = describe->Execute ();
    }

    return (FDO_SAFE_ADDREF(mSchemaMappingCollection.p));
}

// Sets the current schema mapping collection.
void ArcSDEConnection::SetSchemaMappingCollection (FdoPhysicalSchemaMappingCollection* schemaMappingCollection)
{
    mSchemaMappingCollection = FDO_SAFE_ADDREF(schemaMappingCollection);
}

ArcSDESchemaMapping* ArcSDEConnection::GetSchemaMapping(FdoString* fdoSchemaName, FdoString* fdoClassName, bool bAutoLoad)
{
    FdoPtr<ArcSDESchemaMapping> schemaMapping;
    FdoPtr<FdoPhysicalSchemaMappingCollection> schemaMappings = this->GetSchemaMappingCollection(fdoSchemaName, fdoClassName, bAutoLoad);

    schemaMapping = (ArcSDESchemaMapping*)schemaMappings->GetItem(ARCSDE_PROVIDER_NAME, fdoSchemaName);
    if (schemaMapping == NULL)
    {
        schemaMapping = ArcSDESchemaMapping::Create();
        schemaMapping->SetName(fdoSchemaName);
        schemaMappings->Add(schemaMapping);
    }

    return FDO_SAFE_ADDREF(schemaMapping.p);
}

ArcSDEClassMapping* ArcSDEConnection::GetClassMapping(FdoString* fdoSchemaName, FdoString *fdoClassName, bool bAutoLoad)
{
    FdoPtr<ArcSDESchemaMapping> schemaMapping = GetSchemaMapping(fdoSchemaName, fdoClassName, bAutoLoad);
    FdoPtr<ArcSDEClassMappingCollection> classMappings = schemaMapping->GetClasses();
    FdoPtr<ArcSDEClassMapping> classMapping;

    if (classMappings->Contains(fdoClassName))
        classMapping = classMappings->GetItem(fdoClassName);
    else
    {
        classMapping = ArcSDEClassMapping::Create();
        classMapping->SetName(fdoClassName);
        classMappings->Add(classMapping);
    }

    return FDO_SAFE_ADDREF(classMapping.p);
}

ArcSDEPropertyMapping* ArcSDEConnection::GetPropertyMapping(FdoClassDefinition* definition, FdoString *fdoPropertyName, bool bAutoLoad)
{
    FdoPtr<FdoFeatureSchema> schema = definition->GetFeatureSchema ();
    FdoPtr<ArcSDEClassMapping> classMapping = GetClassMapping(schema->GetName (), definition->GetName (), bAutoLoad);
    FdoPtr<ArcSDEPropertyMappingCollection> propertyMappings = classMapping->GetProperties();
    FdoPtr<ArcSDEPropertyMapping> propertyMapping;

    if (propertyMappings->Contains(fdoPropertyName))
        propertyMapping = propertyMappings->GetItem(fdoPropertyName);
    else
    {
        propertyMapping = ArcSDEPropertyMapping::Create();
        propertyMapping->SetName(fdoPropertyName);
        propertyMappings->Add(propertyMapping);
    }

    return FDO_SAFE_ADDREF(propertyMapping.p);
}

ArcSDEIndexMapping* ArcSDEConnection::GetIndexMapping(FdoString* fdoSchemaName, FdoString *fdoClassName, FdoString *arcSdeIndexName)
{
    FdoPtr<ArcSDEClassMapping> classMapping = GetClassMapping(fdoSchemaName, fdoClassName);
    FdoPtr<ArcSDEIndexMappingCollection> indexMappings = classMapping->GetIndexes();
    FdoPtr<ArcSDEIndexMapping> indexMapping;

    if (indexMappings->Contains(arcSdeIndexName))
        indexMapping = indexMappings->GetItem(arcSdeIndexName);
    else
    {
        indexMapping = ArcSDEIndexMapping::Create();
        indexMapping->SetName(arcSdeIndexName);
        indexMappings->Add(indexMapping);
    }

    return FDO_SAFE_ADDREF(indexMapping.p);
}

void ArcSDEConnection::ClassToTable (CHAR* table, FdoClassDefinition* classDef)
{
    FdoString* unqualified;
    CHAR* table_name;
    LONG result;

    FdoPtr<FdoFeatureSchema> schema = classDef->GetFeatureSchema ();

    // Get database and owner names for this class:
    CHAR dbName[SE_MAX_DATABASE_LEN+1];
    CHAR ownerName[SE_MAX_DATABASE_LEN+1];
    ClassToDatabase(dbName, classDef);
    ClassToUser(ownerName, classDef);

    // Get unqualified table name for this class:
    FdoPtr<ArcSDEClassMapping> classMapping = GetClassMapping (schema->GetName (), classDef->GetName ());
    unqualified = classMapping->GetTableName ();
    if ((NULL == unqualified) || (0 == wcslen (unqualified)))
        unqualified = classDef->GetName ();
    wide_to_multibyte (table_name, unqualified);

    // Validate unqualified table name:
    if (SE_MAX_TABLE_LEN - 1 < strlen (table_name))
        throw FdoException::Create (NlsMsgGet1 (ARCSDE_TABLE_NAME_TOO_LONG, "Table name '%1$ls' is too long.", unqualified));

    // Build qualified table name, outputting it to given "table" buffer:
    result = SE_table_make_qualified_name (GetConnection (), dbName, ownerName, table_name, NULL, table);
    handle_sde_err<FdoCommandException> (GetConnection(), result, __FILE__, __LINE__, ARCSDE_QUALIFIED_TABLENAME_FAILURE, "Failed to make a qualified table name from table name '%1$ls' and schema name '%2$ls'.", unqualified, schema->GetName());
}

void ArcSDEConnection::PropertyToColumn (CHAR* column, FdoClassDefinition* definition, FdoIdentifier* property)
{
    FdoInt32 length;
    FdoString* name;
    FdoString* col;
    FdoString* schema;
    CHAR* mschema;
    CHAR* mcol;

    property->GetScope (length);
    if (0 != length)
        throw FdoException::Create (NlsMsgGet1(ARCSDE_IDENTIFIER_SCOPES_UNSUPPORTED, "FDO Identifier scopes are not supported; found scopes in identifier '%1$ls'.", property->ToString()));

    name = property->GetName (); // ToDo: qualified name in mapping table?
    FdoPtr<ArcSDEPropertyMapping> propertyMapping = GetPropertyMapping (definition, name);
    FdoString* sdePropertyName = propertyMapping->GetColumnName ();
    if ((NULL == sdePropertyName) || (0 == wcslen (sdePropertyName)))
        col = name;
    else
        col = sdePropertyName;
    schema = property->GetSchemaName ();
    if (0 != wcslen (schema))
    {
        wide_to_multibyte (mschema, schema);
        strcpy (column, mschema);
        strcat (column, ".");
    }
    else
        column[0] = '\0';
    wide_to_multibyte (mcol, col);
    strcat (column, mcol);
}

FdoClassDefinition* ArcSDEConnection::TableToClass (FdoString* wQualifiedTableName)
{
    CHAR mbOwnerName[SE_MAX_OWNER_LEN+1];
    CHAR mbTableName[SE_MAX_TABLE_LEN+1];
    CHAR mbDatabaseName[SE_MAX_DATABASE_LEN+1];
    CHAR *mbQualifiedTableName = NULL;
    wchar_t *wOwnerName = NULL;
    FdoStringP wSchemaName;
    wchar_t *wTableName = NULL;
    wchar_t *wDatabaseName = NULL;
    FdoString* wClassName;
    LONG result;
    FdoPtr<FdoFeatureSchemaCollection> schemas;
    FdoPtr<FdoFeatureSchema> schema;
    FdoPtr<FdoClassDefinition> ret;

    // Parse given wQualifiedTableName into wSchemaName and wTableName:
    wide_to_multibyte(mbQualifiedTableName, wQualifiedTableName);
    result = SE_table_parse_qualified_name(mConnection, mbQualifiedTableName, mbDatabaseName, mbOwnerName, mbTableName, NULL, FALSE);
    handle_sde_err<FdoException>(mConnection, result, __FILE__, __LINE__, ARCSDE_PARSE_TABLE_NAME_FAILED, "Failed to parse the qualified name '%1$ls'.", wQualifiedTableName);
    multibyte_to_wide(wOwnerName, mbOwnerName);
    multibyte_to_wide(wTableName, mbTableName);
    multibyte_to_wide(wDatabaseName, mbDatabaseName);

    // Get local cache of all schemas:
    schemas = GetSchemaCollection ();

    // Default schema name to [<DATABASENAME>_]<OWNERNAME>, and class name to table name:
    if ((wDatabaseName != NULL) && (wcslen(wDatabaseName)>0))
    {
        wSchemaName  = wDatabaseName;
        wSchemaName += SCHEMANAME_SEPARATOR;
        wSchemaName += wOwnerName;
    }
    else
    {
        wSchemaName = wOwnerName;
    }
    wClassName = wTableName;

    // Iterate all class mappings in all schemas, finding the one class that matches the given table (if any):
    wClassName = NULL;
    for (FdoInt32 i=0; (i<schemas->GetCount()) && (NULL == wClassName); i++)
    {
        FdoPtr<FdoFeatureSchema> schema = schemas->GetItem(i);
        FdoPtr<ArcSDESchemaMapping> schemaMapping = GetSchemaMapping (schema->GetName());
        FdoPtr<ArcSDEClassMappingCollection> classMappings = schemaMapping->GetClasses ();
        for (int i = 0; (i < classMappings->GetCount ()) && (NULL == wClassName); i++)
        {
            FdoPtr<ArcSDEClassMapping> classMapping = classMappings->GetItem (i);
            if (   (0 == RdbmsNamesMatch (wTableName, classMapping->GetTableName ()))
                && (0 == RdbmsNamesMatch (wOwnerName, classMapping->GetOwnerName ()))
                && (0 == RdbmsNamesMatch (wDatabaseName, classMapping->GetDatabaseName ())) )
            {
                wClassName = classMapping->GetName ();
                wSchemaName = schemaMapping->GetName();
            }
        }
    }


    // get the schema
    schema = schemas->FindItem (wSchemaName);
    if (schema == NULL)
        throw FdoException::Create (NlsMsgGet1 (ARCSDE_SCHEMA_DOES_NOT_EXIST, "Schema '%1$ls' does not exist.", (const wchar_t *) wSchemaName));

    // check if this schema contains the requested class
    FdoPtr<FdoClassCollection> classes = schema->GetClasses();
    for (int j = 0; (j < classes->GetCount ()) && (ret == NULL); j++)
    {
        FdoPtr<FdoClassDefinition> fdoClassDef = classes->FindItem (wClassName);
        if (fdoClassDef != NULL)
            ret = fdoClassDef;
    }

    // check if we found the requested class
    if (ret == NULL)
        throw FdoException::Create (NlsMsgGet1(ARCSDE_FEATURE_CLASS_NOT_FOUND, "FDO Class '%1$ls' not found in schema.", wClassName));

    return (FDO_SAFE_ADDREF(ret.p));
}

FdoString* ArcSDEConnection::ColumnToProperty (FdoClassDefinition* definition, FdoString* columnName, bool bAutoLoad)
{
    FdoString* ret;

    ret = NULL;

    // check overrides
    FdoPtr<FdoFeatureSchema> schema = definition->GetFeatureSchema ();
    FdoPtr<ArcSDESchemaMapping> schemaMapping = GetSchemaMapping (schema->GetName (), definition->GetName(), bAutoLoad);
    FdoPtr<ArcSDEClassMappingCollection> classMappings = schemaMapping->GetClasses ();
    ArcSDEClassMapping* classMapping = classMappings->FindItem (definition->GetName ());
    if (NULL != classMapping)
    {
        FdoPtr<ArcSDEPropertyMappingCollection> properties = classMapping->GetProperties ();
        for (int i = 0; (i < properties->GetCount ()) && (NULL == ret); i++)
        {
            FdoPtr<ArcSDEPropertyMapping> property = properties->GetItem (i);
            if (0 == RdbmsNamesMatch (columnName, property->GetColumnName ()))
                ret = property->GetName ();
        }
        classMapping->Release ();
    }
    // default to propertyname==columnname
    if (NULL == ret)
        ret = columnName;

    return (ret);
}

// Returns the requested fdo property;
// NOTE: doesn't look in baseProperties collection of given class.
FdoPropertyDefinition* ArcSDEConnection::GetProperty(FdoClassDefinition* definition, FdoString *fdoPropertyName)
{
    FdoPtr<FdoFeatureSchema> schema = definition->GetFeatureSchema ();
    FdoPtr<FdoClassCollection> classes = schema->GetClasses();
    FdoPtr<FdoClassDefinition> aClass = classes->GetItem(definition->GetName ());
    FdoPtr<FdoPropertyDefinitionCollection> properties = aClass->GetProperties();
    FdoPtr<FdoPropertyDefinition> property = properties->GetItem(fdoPropertyName);

    return FDO_SAFE_ADDREF(property.p);
}


// Returns the user name corresponding to a given FDO class;
// NOTE: userName must already be of size >= SE_MAX_OWNER_LEN+1
void ArcSDEConnection::ClassToUser(CHAR* userName, FdoClassDefinition* classDef)
{
    FdoString* wOwnerName;
    CHAR* mbOwnerName;

    // Get owner name from class mapping:
    FdoPtr<FdoFeatureSchema> schema = classDef->GetFeatureSchema ();
    FdoPtr<ArcSDEClassMapping> classMapping = GetClassMapping (schema->GetName (), classDef->GetName ());
    wOwnerName = classMapping->GetOwnerName ();

    // If no owner name in class mapping, default to connected user:
    if ((NULL == wOwnerName) || (0 == wcslen (wOwnerName)))
        mbOwnerName = m_mbUserName;
    else
        wide_to_multibyte (mbOwnerName, wOwnerName);

    // Validate owner name length:
    if (SE_MAX_OWNER_LEN < strlen (mbOwnerName))
    {
        wchar_t* wOwnerNameTemp = NULL;
        multibyte_to_wide(wOwnerNameTemp, mbOwnerName);
        throw FdoException::Create (NlsMsgGet1 (ARCSDE_OWNER_NAME_TOO_LONG, "Owner name '%1$ls' is too long.", wOwnerNameTemp));
    }

    // Output the user name:
    strcpy(userName, mbOwnerName);
}

// Returns the database name corresponding to a given FDO class;
// NOTE: databaseName must already be of size >= SE_MAX_DATABASE_LEN+1
void ArcSDEConnection::ClassToDatabase(CHAR* databaseName, FdoClassDefinition* classDef)
{
    FdoString* wDatabaseName;
    CHAR* mbDatabaseName;

    // Get database name from class mapping:
    FdoPtr<FdoFeatureSchema> schema = classDef->GetFeatureSchema ();
    FdoPtr<ArcSDEClassMapping> classMapping = GetClassMapping (schema->GetName (), classDef->GetName ());
    wDatabaseName = classMapping->GetDatabaseName ();

    // If no database name in class mapping, default to connected database:
    if ((NULL == wDatabaseName) || (0 == wcslen (wDatabaseName)))
        mbDatabaseName = m_mbDatabaseName;
    else
        wide_to_multibyte (mbDatabaseName, wDatabaseName);

    // Validate database name length:
    if (SE_MAX_DATABASE_LEN < strlen (mbDatabaseName))
    {
        wchar_t* wDatabaseNameTemp = NULL;
        multibyte_to_wide(wDatabaseNameTemp, mbDatabaseName);
        throw FdoException::Create (NlsMsgGet1 (ARCSDE_DATABASE_NAME_TOO_LONG, "Database name '%1$ls' is too long.", wDatabaseNameTemp));
    }

    // Output the database name:
    strcpy(databaseName, mbDatabaseName);
}



void ArcSDEConnection::GetRdbmsInfo(void)
{
    // Get Rdbms info, if not yet cached for this connection:
    if (m_lCachedRdbmsId == -2)  // -2 means not-yet-cached
    {
        SE_connection_get_dbms_info(GetConnection(), &m_lCachedRdbmsId, &m_lCachedRdbmsBehaviours);

        // Determine system table name prefix:
        if (m_lCachedRdbmsId == SE_DBMS_IS_ORACLE)
            m_sCachedRdbmsSystemTablePrefix = SDE_USER L".";
        else // not sure, determine dynamically; even SQL Server may vary, depending on if the user is connected to a multiple model database other than "sde" or not.
        {
            LONG result = SE_SUCCESS;
            SHORT numColumns = 0;
            SE_COLUMN_DEF *column_defs = NULL;

            FdoString* potentialPrefixesW[] = 
            {
                SDE_USER L".",  //without "SDE_" prefix within current database
                SDE_USER L"." OPTIONAL_SYSTEM_TABLE_PREFIX,  //with "SDE_" prefix within current database
                SDE_USER L"." SDE_USER L".",  //without "SDE_" prefix within central "sde" database
                SDE_USER L"." SDE_USER L"." OPTIONAL_SYSTEM_TABLE_PREFIX  //with "SDE_" prefix within central "sde" database
            };

            m_sCachedRdbmsSystemTablePrefix = SDE_USER L"."; // default to no prefix if any problem is encountered
            for (int i=0; i<ELEMENTS(potentialPrefixesW); i++)
            {
                FdoString* potentialPrefixW = potentialPrefixesW[i];
                FdoStringP potentialSysTableW = potentialPrefixW;
                potentialSysTableW += SDE_SYSTEM_TABLE_STATE_LINEAGES;
                char *potentialSysTableMB = NULL;
                wide_to_multibyte(potentialSysTableMB, potentialSysTableW);
                
                result = SE_table_describe(GetConnection(), potentialSysTableMB, &numColumns, &column_defs);
                if ((result == SE_SUCCESS) && (numColumns > 0))
                {
                    m_sCachedRdbmsSystemTablePrefix = potentialPrefixW;

                    // clean up memory allocated by SE_table_describe():
                    if (NULL != column_defs)
                        SE_table_free_descriptions(column_defs);
                    break;
                }
            }
        }
    }
}

bool ArcSDEConnection::RdbmsHasBehaviour(LONG lBehaviour)
{
    GetRdbmsInfo();
    return (m_lCachedRdbmsBehaviours & lBehaviour) > 0;
}


long ArcSDEConnection::RdbmsId(void)
{
    GetRdbmsInfo();
    return m_lCachedRdbmsId;
}


int ArcSDEConnection::RdbmsNamesMatch(const CHAR* name1, const CHAR* name2)
{
    if (RdbmsHasBehaviour(SE_DBMS_IS_CASE_SENSITIVE))
        return strcmp(name1, name2);
    else
        return FdoCommonOSUtil::stricmp(name1, name2);
}

int ArcSDEConnection::RdbmsNamesMatch(const wchar_t* name1, const wchar_t* name2)
{
    CHAR* mbsName1 = NULL;
    CHAR* mbsName2 = NULL;
    wide_to_multibyte(mbsName1, name1);
    wide_to_multibyte(mbsName2, name2);
    return RdbmsNamesMatch(mbsName1, mbsName2);
}


FdoString* ArcSDEConnection::RdbmsSystemTablePrefix(void)
{
    GetRdbmsInfo();
    return m_sCachedRdbmsSystemTablePrefix;
}


LONG ArcSDEConnection::GetActiveVersion ()
{
    return (mActiveVersion);
}

void ArcSDEConnection::SetActiveVersion (LONG id)
{
    SE_VERSIONINFO version;
    LONG result;
    wchar_t buffer[50];
    LONG access;

    // create the empty version object
    result = SE_versioninfo_create (&version);
    handle_sde_err<FdoCommandException> (mConnection, result, __FILE__, __LINE__, ARCSDE_VERSION_INFO_ALLOC, "Cannot initialize SE_VERSIONINFO structure.");

    Flush ();

    if (-1L != id) // ignore for final teardown
    {
        // get version info
        result = SE_version_get_info_by_id (mConnection, id, version);
        handle_sde_err<FdoCommandException> (mConnection, result, __FILE__, __LINE__, ARCSDE_VERSION_INFO, "Version info for '%1$ls' could not be retrieved.", FdoCommonOSUtil::itow (id, buffer, ELEMENTS(buffer)));

        // check the access
        result = SE_versioninfo_get_access (version, &access);
        handle_sde_err<FdoCommandException> (mConnection, result, __FILE__, __LINE__, ARCSDE_VERSION_INFO_ITEM, "Version info item '%1$ls' could not be retrieved.", L"access");
        if (!(SE_VERSION_ACCESS_PUBLIC == access))
            if (!ArcSDELongTransactionUtility::IsOurVersion (mConnection, version))
                throw FdoException::Create (NlsMsgGet (ARCSDE_NOT_OWNER, "User is not the version owner."));

        // lock the state
        SetActiveState (ArcSDELongTransactionUtility::LockVersion (this, version));
    }

    SE_versioninfo_free (version);

    // remember the id
    mActiveVersion = id;
}

LONG ArcSDEConnection::GetActiveState ()
{
    return (mActiveState);
}

void ArcSDEConnection::SetActiveState (LONG id)
{
    mActiveState = id;
}

void ArcSDEConnection::Flush ()
{
    LONG result;
    SE_VERSIONINFO version;
    wchar_t buffer[50];

    // if there is an active state, close it and move the active version to it
    if (-1L != GetActiveState ())
    {
        // create the empty version object
        result = SE_versioninfo_create (&version);
        handle_sde_err<FdoCommandException> (GetConnection (), result, __FILE__, __LINE__, ARCSDE_VERSION_INFO_ALLOC, "Cannot initialize SE_VERSIONINFO structure.");

        // get version info
        result = SE_version_get_info_by_id (GetConnection (), GetActiveVersion (), version);
        handle_sde_err<FdoCommandException> (GetConnection (), result, __FILE__, __LINE__, ARCSDE_VERSION_INFO, "Version info for '%1$ls' could not be retrieved.", FdoCommonOSUtil::itow (GetActiveVersion (), buffer, ELEMENTS(buffer)));

        // close the state and move the version to that state
        ArcSDELongTransactionUtility::UnlockVersion (GetConnection (), version, GetActiveState ());

        // free the version info structure
        SE_versioninfo_free (version);

        // no active state
        SetActiveState (-1L);
    }
}

void ArcSDEConnection::SetConfiguration(FdoIoStream* configStream)
{
    throw FdoException::Create(NlsMsgGet(ARCSDE_CONFIGURATION_FILE_NOT_SUPPORTED, "ArcSDE Provider does not support configuration files."));
}


FdoString* ArcSDEConnection::GetActiveSpatialContext(void)
{
    return mActiveSpatialContext;
}

void ArcSDEConnection::SetActiveSpatialContext(FdoString* name)
{
    mActiveSpatialContext = name;
}

void ArcSDEConnection::SetActiveSpatialContextToDefault(void)
{
    if (GetConnectionState() == FdoConnectionState_Open)
    {
        // Set the first spatial context to be the active one;
        // Do nothing if there are no spatial contexts:
        FdoPtr<ArcSDEGetSpatialContexts> getSpatialContexts = (ArcSDEGetSpatialContexts*)CreateCommand(FdoCommandType_GetSpatialContexts);
        getSpatialContexts->SetActiveOnly(false);
        FdoPtr<FdoISpatialContextReader> spatialContextReader = getSpatialContexts->Execute();
        if (spatialContextReader->ReadNext())
            mActiveSpatialContext = spatialContextReader->GetName();
        else
            mActiveSpatialContext = (FdoString*)NULL;  // no active spatial context
    }
}

FdoITransaction* ArcSDEConnection::GetTransaction ()
{
    return (FDO_SAFE_ADDREF (mTransaction));
}

void ArcSDEConnection::SetTransaction (FdoITransaction* transaction)
{
    if (NULL != transaction)
        throw FdoException::Create(NlsMsgGet(ARCSDE_SET_TRANSACTION_NOT_SUPPORTED, "Setting the transaction is not supported."));
    else
    {
        if (NULL != mTransaction)
        {
            mTransaction->SetConnection (NULL);
            mTransaction->Release ();
            mTransaction = NULL;
        }
    }
}

class ArcSDEGeometryCapabilities : public FdoIGeometryCapabilities
{
    virtual FdoGeometryType* GetGeometryTypes( FdoInt32& length)
    {
        static const FdoGeometryType geomTypes[] =
        {
            FdoGeometryType_Point,
            FdoGeometryType_LineString,
            FdoGeometryType_Polygon,
            FdoGeometryType_MultiPoint,
            FdoGeometryType_MultiLineString,
            FdoGeometryType_MultiPolygon
        };
        length = ELEMENTS(geomTypes);
        return (FdoGeometryType*)geomTypes;
    }

    virtual FdoGeometryComponentType* GetGeometryComponentTypes( FdoInt32& length )
    {
        static const FdoGeometryComponentType geomComponentTypes[] =
        {
            FdoGeometryComponentType_LinearRing,
            FdoGeometryComponentType_LineStringSegment
        };
        length = ELEMENTS(geomComponentTypes);
        return (FdoGeometryComponentType*)geomComponentTypes;
    }

    virtual FdoInt32 GetDimensionalities() { return FdoDimensionality_XY | FdoDimensionality_Z | FdoDimensionality_M; }
    virtual void Dispose () { delete this; }
};

/// Gets an FdoIGeometryCapabilities interface describing the provider's support for geometry
FdoIGeometryCapabilities* ArcSDEConnection::GetGeometryCapabilities()
{
    return (new ArcSDEGeometryCapabilities());
}


#ifdef _WIN32
LONG WINAPI DelayLoadDllExceptionFilter(PEXCEPTION_POINTERS pExcPointers, wchar_t* errorMessage)
{
    wchar_t *moduleName;
    wchar_t procName[512];
    LONG lDisposition = EXCEPTION_EXECUTE_HANDLER;  
    PDelayLoadInfo pDelayLoadInfo = 
        PDelayLoadInfo(pExcPointers->ExceptionRecord->ExceptionInformation[0]);

    switch (pExcPointers->ExceptionRecord->ExceptionCode) {
    case VcppException(ERROR_SEVERITY_ERROR, ERROR_MOD_NOT_FOUND):
        multibyte_to_wide(moduleName, pDelayLoadInfo->szDll);
        wcscpy(errorMessage, NlsMsgGet1(ARCSDE_RUNTIME_NOT_FOUND_MODULE, "The ArcSDE runtime was not found (module %1$ls).", moduleName));
        break;

    case VcppException(ERROR_SEVERITY_ERROR, ERROR_PROC_NOT_FOUND):
        multibyte_to_wide(moduleName, pDelayLoadInfo->szDll);
        if (pDelayLoadInfo->dlp.fImportByName)
            FdoCommonOSUtil::swprintf(procName, ELEMENTS(procName), L"%s", pDelayLoadInfo->dlp.szProcName);
        else
            FdoCommonOSUtil::swprintf(procName, ELEMENTS(procName), L"%d", pDelayLoadInfo->dlp.dwOrdinal);
        wcscpy(errorMessage, NlsMsgGet2(ARCSDE_RUNTIME_NOT_FOUND_PROC, "The ArcSDE runtime was not found (procedure %1$ls in module %2$ls).", procName, moduleName));
        break;

    default:
        wcscpy(errorMessage, NlsMsgGet(ARCSDE_RUNTIME_NOT_FOUND, "The ArcSDE runtime was not found."));
        break;
    }

    return(lDisposition);
}
#endif


LONG ArcSDEConnection::ConnectToArcSDE(const CHAR* server, const CHAR* instance, const CHAR* datastore,
                                       const CHAR* username, const CHAR* password, SE_ERROR *error, SE_CONNECTION *connection)
{
    LONG result;
    wchar_t errorMessage[1024];

    // attempt to connect and catch an exception,
    // which we presume means the ArcSDE .dll (or .so?) files are not found or
    // there was some problem with the function linkage
#ifdef _WIN32
    __try
    {
#endif
        result = SE_connection_create (
            server, // Host name of the server running ArcSDE.
            instance, // The ArcSDE instance name.
            datastore, // The database or data source name. Not applicable for some DBMS.
            username, // DBMS user's login name.
            password, // DBMS user's login password.
            error, // Any error code is returned here.
            connection); // Returned connection handle.
        // Note:
        // this bogus test and throw is added because of a bug in the Visual Studio C++ compiler
        // that optimizes out the try/catch(...) in release mode without this 
        if (result == 19191991)
            throw FdoException::Create (NlsMsgGet(ARCSDE_RUNTIME_NOT_FOUND, "The ArcSDE runtime was not found."));
#ifdef _WIN32
    }
    __except(DelayLoadDllExceptionFilter(GetExceptionInformation(), errorMessage))
    {
        throw FdoException::Create (errorMessage);
    }
#endif

    return result;
}


void ArcSDEConnection::GetArcSDERegistrationList(SE_REGINFO** registrations, long *count)
{
	// Load registration list, if required:
	if (mCachedRegistrations == NULL)
	{
	    long result = SE_registration_get_info_list (mConnection, &mCachedRegistrations, &mCachedRegistrationCount);
		handle_sde_err<FdoCommandException> (mConnection, result, __FILE__, __LINE__, ARCSDE_REGISTRATION_INFO_GET, "Table registration info could not be retrieved.");
	}

	// Return a pointer to internal cache:
	*registrations = mCachedRegistrations;
	*count = mCachedRegistrationCount;
}


void ArcSDEConnection::GetArcSDEMetadataList(SE_METADATAINFO** pMetadataList, long* count)
{
    if (mCachedMetadataList == NULL)
    {
        long result = SE_metadata_get_info_list (GetConnection (),
            "NOT CLASS_NAME='SDE internal'", &mCachedMetadataList, &mCachedMetadataListCount);
        if (result != SE_SUCCESS)  // NOTE: may get error here due to ArcSDE bugs; better to ignore the error than throw away the whole table
        {
            mCachedMetadataList = NULL;
            mCachedMetadataListCount = 0;
        }
    }

    *pMetadataList = mCachedMetadataList;
    *count = mCachedMetadataListCount;
}

long ArcSDEConnection::GetArcSDELayerInfo(SE_LAYERINFO &pLayerInfo, const CHAR* tableName, const CHAR* columnName)
{
    long result = SE_SUCCESS;

    pLayerInfo = NULL;

    if (mCachedLayerList == NULL)
    {
        result = SE_layer_get_info_list(GetConnection(), &mCachedLayerList, &mCachedLayerListCount);
    }

    if (result == SE_SUCCESS)
    {
        for (int i=0; i<mCachedLayerListCount; i++)
        {
            CHAR cachedTableName[SE_QUALIFIED_TABLE_NAME];
            CHAR cachedColumnName[SE_MAX_COLUMN_LEN];
            result = SE_layerinfo_get_spatial_column(mCachedLayerList[i], cachedTableName, cachedColumnName);
            if ((result==SE_SUCCESS) && (0==strcmp(tableName, cachedTableName)) && (0==strcmp(columnName, cachedColumnName)))
            {
                pLayerInfo = mCachedLayerList[i];
                break;
            }
        }
    }

    if ((pLayerInfo == NULL) && (result==SE_SUCCESS))
        result = SE_FAILURE;

    return result;
}


void ArcSDEConnection::GetArcSDESpatialRefList(SE_SPATIALREFINFO** pSpatialRefInfoList, LONG** pSpatialRefSridList, long *pCount)
{
    if (mCachedSpatialRefList == NULL)
    {
        LONG lResult = SE_SUCCESS;

        // fetch all ArcSDE Spatial Reference Systems (equivalent to FDO Spatial Contexts):
        mCachedSpatialRefListCreatedByUs = false;
        lResult = SE_spatialref_get_info_list(GetConnection(), &mCachedSpatialRefList, &mCachedSpatialRefListCount);
        if (lResult != SE_SUCCESS)
        {
            mCachedSpatialRefListCreatedByUs = true;
            FdoPtr<FdoISQLCommand> sql = (FdoISQLCommand*)CreateCommand(FdoCommandType_SQLCommand);
            sql->SetSQLStatement(FdoStringP::Format(L"select * from %lsSPATIAL_REFERENCES", RdbmsSystemTablePrefix()));
            FdoPtr<FdoISQLDataReader> sqlReader = sql->ExecuteReader();

            // Cache all rows:
            long lMaxArraySize = 20;
            mCachedSpatialRefListCount = 0;
            mCachedSpatialRefList = new SE_SPATIALREFINFO[lMaxArraySize];
            mCachedSpatialRefSRIDList = new long[lMaxArraySize];
            while (sqlReader->ReadNext())
            {
                // Dynamically resize array if needed:
                if (lMaxArraySize==mCachedSpatialRefListCount)
                {
                    lMaxArraySize = lMaxArraySize * 2;

                    SE_SPATIALREFINFO *newCachedSpatialRefList = new SE_SPATIALREFINFO[lMaxArraySize];
                    long *newCachedSpatialRefSRIDList = new long[lMaxArraySize];
                    for (int i=0; i<mCachedSpatialRefListCount; i++)
                    {
                        newCachedSpatialRefList[i] = mCachedSpatialRefList[i];
                        newCachedSpatialRefSRIDList[i] = mCachedSpatialRefSRIDList[i];
                    }
                    delete[] mCachedSpatialRefList;
                    delete[] mCachedSpatialRefSRIDList;
                    mCachedSpatialRefList = newCachedSpatialRefList;
                    mCachedSpatialRefSRIDList = newCachedSpatialRefSRIDList;
                }

                // Create one SE_SPATIALREFINFO for this row:
                lResult = SE_spatialrefinfo_create (&(mCachedSpatialRefList[mCachedSpatialRefListCount]));
                handle_sde_err<FdoException>(lResult, __FILE__, __LINE__, ARCSDE_FAILED_TO_READ_SRS, "Failed to get or set information for this ArcSDE Spatial Reference System.");

                // Read auth name:
                FdoStringP wAuthName;
                if (!sqlReader->IsNull(AdjustSystemColumnName(L"auth_name")))
                    wAuthName = sqlReader->GetString(AdjustSystemColumnName(L"auth_name"));
                lResult = SE_spatialrefinfo_set_auth_name(mCachedSpatialRefList[mCachedSpatialRefListCount], (const char*)wAuthName);
                handle_sde_err<FdoException>(lResult, __FILE__, __LINE__, ARCSDE_FAILED_TO_READ_SRS, "Failed to get or set information for this ArcSDE Spatial Reference System.");

                // Read SRID:
                long srid = 0;
                if (!sqlReader->IsNull(AdjustSystemColumnName(L"srid")))
                    srid = sqlReader->GetInt32(AdjustSystemColumnName(L"srid"));
                mCachedSpatialRefSRIDList[mCachedSpatialRefListCount] = srid;

                // Read description:
                FdoStringP desc;
                if (!sqlReader->IsNull(AdjustSystemColumnName(L"description")))
                    desc = sqlReader->GetString(AdjustSystemColumnName(L"description"));
                // NOTE: for some unknown scary reason, we get unexpected garbage at the end of the description,
                //    even though the description appears valid in the RDBMS, so we need to trim it:
                //TODO: see if we can fix this issue!!
                // NOTE: if FdoStringP::Left() doesnt find the given suffix (e.g. in the case of a natively-created spatial reference system),
                //    it returns the entire string unchanged.
                FdoStringP fixedDesc = desc.Left(SPATIALCONTEXT_DESC_SUFFIX);
                lResult = SE_spatialrefinfo_set_description(mCachedSpatialRefList[mCachedSpatialRefListCount], (const char*)fixedDesc);
                handle_sde_err<FdoException>(lResult, __FILE__, __LINE__, ARCSDE_FAILED_TO_READ_SRS, "Failed to get or set information for this ArcSDE Spatial Reference System.");

                // Create new empty coordref:
                SE_COORDREF coordref;
                lResult = SE_coordref_create(&coordref);
                handle_sde_err<FdoException>(lResult, __FILE__, __LINE__, ARCSDE_FAILED_TO_READ_SRS, "Failed to get or set information for this ArcSDE Spatial Reference System.");

                // Read coordsys srtext/description:
                FdoStringP coordsysWkt;
                if (!sqlReader->IsNull(AdjustSystemColumnName(L"srtext")))
                    coordsysWkt = sqlReader->GetString(AdjustSystemColumnName(L"srtext"));
                lResult = SE_coordref_set_by_description(coordref, coordsysWkt);
                handle_sde_err<FdoException>(lResult, __FILE__, __LINE__, ARCSDE_FAILED_TO_READ_SRS, "Failed to get or set information for this ArcSDE Spatial Reference System.");

                // Read coordsys extents:
                double dFalseX  = sqlReader->GetDouble(AdjustSystemColumnName(L"falsex"));
                double dFalseY  = sqlReader->GetDouble(AdjustSystemColumnName(L"falsey"));
                double dFalseZ  = sqlReader->GetDouble(AdjustSystemColumnName(L"falsez"));
                double dXYunits = sqlReader->GetDouble(AdjustSystemColumnName(L"xyunits"));
                double dZunits = 1.0;
                if (!sqlReader->IsNull(AdjustSystemColumnName(L"zunits")))
                    dZunits = sqlReader->GetDouble(AdjustSystemColumnName(L"zunits"));
                lResult = SE_coordref_set_xy(coordref, dFalseX, dFalseY, dXYunits);
                handle_sde_err<FdoException>(lResult, __FILE__, __LINE__, ARCSDE_FAILED_TO_READ_SRS, "Failed to get or set information for this ArcSDE Spatial Reference System.");
                lResult = SE_coordref_set_z(coordref, dFalseZ, dZunits);
                handle_sde_err<FdoException>(lResult, __FILE__, __LINE__, ARCSDE_FAILED_TO_READ_SRS, "Failed to get or set information for this ArcSDE Spatial Reference System.");

                // Store coordref onto cached spatialref:
                lResult = SE_spatialrefinfo_set_coordref(mCachedSpatialRefList[mCachedSpatialRefListCount], coordref);
                handle_sde_err<FdoException>(lResult, __FILE__, __LINE__, ARCSDE_FAILED_TO_READ_SRS, "Failed to get or set information for this ArcSDE Spatial Reference System.");

                // Done reading, increase count:
                mCachedSpatialRefListCount++;
            }
        }
    }

    // Return cached spatial context list to caller:
    *pSpatialRefInfoList = mCachedSpatialRefList;
    *pSpatialRefSridList = mCachedSpatialRefSRIDList;
    *pCount = mCachedSpatialRefListCount;
}

bool ArcSDEConnection::ClassAlreadyLoaded(FdoString* fdoSchemaName, FdoString* fdoClassName)
{
    if (mSchemaCollection && fdoSchemaName && fdoClassName)
    {
        FdoPtr<FdoFeatureSchema> alreadyLoadedSchema = mSchemaCollection->FindItem(fdoSchemaName);
        if (alreadyLoadedSchema)
        {
            FdoPtr<FdoClassCollection> classes = alreadyLoadedSchema->GetClasses();
            FdoPtr<FdoClassDefinition> alreadyLoadedClass = classes->FindItem(fdoClassName);
            if (alreadyLoadedClass)
                return true;
        }
    }

    return false;
}


void ArcSDEConnection::DecacheSchema()
{
	if (mCachedRegistrations != NULL)
	{
		SE_registration_free_info_list (mCachedRegistrationCount, mCachedRegistrations);
		mCachedRegistrationCount = 0;
		mCachedRegistrations = NULL;
	}
    if (mCachedMetadataList != NULL)
    {
        SE_table_metadata_free_info_list (mCachedMetadataListCount, mCachedMetadataList);
        mCachedMetadataList = NULL;
        mCachedMetadataListCount = 0;
    }
    if (mCachedLayerList != NULL)
    {
        SE_layer_free_info_list(mCachedLayerListCount, mCachedLayerList);
        mCachedLayerList = NULL;
        mCachedLayerListCount = 0;
    }

    mSchemaCollection = NULL;
	mSchemaCollectionFullyLoaded = false;
    mSchemaMappingCollection = NULL;
}


void ArcSDEConnection::DecacheSpatialContexts()
{
    if (mCachedSpatialRefList != NULL)
    {
        if (mCachedSpatialRefListCreatedByUs)
        {
            for (int i=0; i<mCachedSpatialRefListCount; i++)
                SE_spatialrefinfo_free(mCachedSpatialRefList[i]);
            delete[] mCachedSpatialRefList;
            delete[] mCachedSpatialRefSRIDList;
        }
        else
        {
            SE_spatialref_free_info_list(mCachedSpatialRefListCount, mCachedSpatialRefList);
        }
        mCachedSpatialRefList = NULL;
        mCachedSpatialRefSRIDList = NULL;
        mCachedSpatialRefListCount = 0;
    }
}

FdoStringP ArcSDEConnection::AdjustSystemColumnName(FdoString *name)
{
    // Different RDBMS's have different default case for column names
    FdoStringP nameCorrected = name;
    if (RdbmsId() == SE_DBMS_IS_SQLSERVER)
        return nameCorrected.Lower();
    else
        return nameCorrected.Upper();
}
