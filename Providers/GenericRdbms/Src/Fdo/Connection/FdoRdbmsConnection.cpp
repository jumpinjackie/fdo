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
#include "FdoRdbmsSchemaUtil.h"
#include <malloc.h>
#include <stdio.h>

#include "FdoRdbmsConnection.h"
#include "FdoRdbmsSelectCommand.h"
#include "FdoRdbmsSelectAggregates.h"
#include "FdoRdbmsInsertCommand.h"
#include "FdoRdbmsDeleteCommand.h"
#include "FdoRdbmsSQLCommand.h"
#include "FdoRdbmsFeatureTransaction.h"
#include "FdoRdbmsApplySchemaCommand.h"
#include "FdoRdbmsDescribeSchemaCommand.h"
#include "FdoRdbmsDescribeSchemaMappingCommand.h"
#include "FdoRdbmsDestroySchemaCommand.h"
#include "FdoRdbmsUpdateCommand.h"

#include "FdoRdbmsConnectionCapabilities.h"
#include "FdoRdbmsCommandCapabilities.h"
#include "FdoRdbmsExpressionCapabilities.h"
#include "FdoRdbmsFilterCapabilities.h"
#include "FdoRdbmsSchemaCapabilities.h"
#include "FdoRdbmsTopologyCapabilities.h"
#include "FdoRdbmsGeometryCapabilities.h"
#include "FdoRdbmsConnectionInfo.h"

#include "FdoRdbmsAcquireLock.h"
#include "FdoRdbmsGetLockOwners.h"
#include "FdoRdbmsGetLockInfo.h"
#include "FdoRdbmsReleaseLock.h"
#include "FdoRdbmsGetLockedObjects.h"

#include "FdoRdbmsActivateLongTransaction.h"
#include "FdoRdbmsCommitLongTransaction.h"
#include "FdoRdbmsCreateLongTransaction.h"
#include "FdoRdbmsDeactivateLongTransaction.h"
#include "FdoRdbmsGetLongTransactions.h"
#include "FdoRdbmsRollbackLongTransaction.h"

#include "Rdbms/FdoRdbmsCommandType.h"
#include "../DataStore/FdoRdbmsGetDataStores.h"
#include "FdoRdbmsCreateSpatialIndex.h"
#include "FdoRdbmsDestroySpatialIndex.h"
#include "FdoRdbmsGetSpatialIndexes.h"

#include "FdoRdbmsCreateSpatialContext.h"
#include "FdoRdbmsDestroySpatialContext.h"
#include "FdoRdbmsActivateSpatialContext.h"
#include "FdoRdbmsGetSpatialContexts.h"
#include "FdoRdbmsConnectionPropertyDictionary.h"

#include "Rdbms/FdoRdbmsCommandType.h"

#include "../SchemaMgr/Ph/Mgr.h"

#ifdef _WIN32
#include <FdoCommonOSUtil.h>
#include <FdoCommonMiscUtil.h>
#include <DelayImp.h>  // For delay-load types
#endif


FdoRdbmsConnection::FdoRdbmsConnection() :
    mConnectionString (NULL),
    mConnectionStringValid(false),
    mConnectionTimeout (15),
    mState (FdoConnectionState_Closed),
    mDbiConnection( NULL ),
    mConnectionCapabilities(NULL),
    mCommandCapabilities(NULL),
    mSchemaCapabilities(NULL),
    mFilterCapabilities(NULL),
    mExpressionCapabilities(NULL),
    mGeometryCapabilities(NULL),
	mTransactionStarted(false),
    mUserNum(-1),
    mUserSessionId(-1),
	mLongTransactionManager(NULL)
{
     mDbiConnection = new DbiConnection( );
}

FdoRdbmsConnection::~FdoRdbmsConnection ()
{
	// This should remain as the first object to be cleaned as it needs the mDbiConnection
	// for cleanup some db resources.
	FDO_SAFE_RELEASE(mLongTransactionManager);

    if (NULL != mConnectionString)
        delete [] mConnectionString;


    if( NULL != mDbiConnection ) {
        if ( mState != FdoConnectionState_Closed )
            // Bypass FdoRdbmsConnection::Close function to avoid 
            // pure virtual function call during destructor.
            mDbiConnection->Close();
        delete mDbiConnection;
        mDbiConnection = NULL;

    }


    FDO_SAFE_RELEASE(mConnectionCapabilities);
    FDO_SAFE_RELEASE(mCommandCapabilities);
    FDO_SAFE_RELEASE(mSchemaCapabilities);
    FDO_SAFE_RELEASE(mFilterCapabilities);
    FDO_SAFE_RELEASE(mExpressionCapabilities);
    FDO_SAFE_RELEASE(mGeometryCapabilities);
}

void FdoRdbmsConnection::set_ConnectionString (const wchar_t* connection)
{
    size_t length;

    if (NULL != mConnectionString)
        delete[] mConnectionString;
    length = wcslen (connection);
    mConnectionString = new wchar_t[length + 1];
    wcscpy (mConnectionString, connection);
}

const wchar_t* FdoRdbmsConnection::get_ConnectionString ()
{
    return (mConnectionString);
}

void FdoRdbmsConnection::set_ConnectionTimeout (int timeout)
{
    mConnectionTimeout = timeout;
}

int FdoRdbmsConnection::get_ConnectionTimeout ()
{
    return (mConnectionTimeout);
}

void FdoRdbmsConnection::SetConfiguration(FdoIoStream* configStream)
{
    // Keep a copy of the configuration document.  We might need it later
    // for spatial contexts.
    // Note: we initially allocate to the length of the entire config document; we
    // do not use the default because of a possible defect in FdoIoMemoryStream
    // when the document is bigger than the default size.
    FdoIoMemoryStreamP stream = FdoIoMemoryStream::Create((FdoSize)configStream->GetLength());
    configStream->Reset();
    stream->Write(configStream);
    mConfigDoc = FDO_SAFE_ADDREF(stream.p);

    if ( mConfigDoc ) {
        // Read the feature schemas and schema overrides from the config document.
        mConfigDoc->Reset();

        mConfigSchemas = FdoFeatureSchemaCollection::Create(NULL);
        mConfigSchemas->ReadXml(mConfigDoc);

        mConfigDoc->Reset();

        mConfigMappings = FdoPhysicalSchemaMappingCollection::Create();
        mConfigMappings->ReadXml( mConfigDoc );
    }
    else {
        mConfigSchemas = NULL;
        mConfigMappings = NULL;
    }
}

void FdoRdbmsConnection::Close ()
{
    if( mDbiConnection != NULL && mState != FdoConnectionState_Closed)
    {
        mState = FdoConnectionState_Closed;
        mDbiConnection->Close();

        FdoPtr<FdoIConnectionInfo> connInfo = GetConnectionInfo();
        FdoPtr<FdoRdbmsConnectionPropertyDictionary> connDict = (FdoRdbmsConnectionPropertyDictionary*)connInfo->GetConnectionProperties();
        FdoPtr<FdoRdbmsProperty> prop = connDict->FindPropertyObject(FDO_RDBMS_CONNECTION_DATASTORE);
        if( prop )
            prop->SetIsPropertyRequired( false );
    }
// For Debugging purposes. Uncomment to check for Schema
// Manager memory leaks. Also need to add SCHEMA_DEBUG preprocessor
// macro to this and SchemaMgr projects.
//    FdoSmBase::Report();
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
        wcscpy(errorMessage, NlsMsgGet1(FDORDBMS_481, "The runtime was not found (module %1$ls).", moduleName));
        break;

    case VcppException(ERROR_SEVERITY_ERROR, ERROR_PROC_NOT_FOUND):
        multibyte_to_wide(moduleName, pDelayLoadInfo->szDll);
        if (pDelayLoadInfo->dlp.fImportByName)
            FdoCommonOSUtil::swprintf(procName, ELEMENTS(procName), L"%s", pDelayLoadInfo->dlp.szProcName);
        else
            FdoCommonOSUtil::swprintf(procName, ELEMENTS(procName), L"%d", pDelayLoadInfo->dlp.dwOrdinal);
        wcscpy(errorMessage, NlsMsgGet2(FDORDBMS_482, "The runtime was not found (procedure %1$ls in module %2$ls).", procName, moduleName));
        break;

    default:
        wcscpy(errorMessage, L"");
        lDisposition = EXCEPTION_CONTINUE_SEARCH;
        break;
    }

    return(lDisposition);
}
#endif


FdoConnectionState FdoRdbmsConnection::Open ()
{
    return Open(false);
}

void FdoRdbmsConnection::DbiOpen(bool skipPending)
{
#ifdef _WIN32
    wchar_t errorMessage[1024];
    errorMessage[0] = L'';
    __try
    {
#endif
        mState = mDbiConnection->Open(skipPending);
        
#ifdef _WIN32
    }
    __except(DelayLoadDllExceptionFilter(GetExceptionInformation(), errorMessage))
    {
        throw FdoException::Create (errorMessage);
    }
#endif
}

FdoConnectionState FdoRdbmsConnection::Open (
    bool skipPending
    )
{
    char *user = NULL;

    if (mConnectionString == NULL)
        throw FdoConnectionException::Create(NlsMsgGet(FDORDBMS_5, "Connection string is not set"));

    if (mConnectionStringValid == false)
        throw FdoConnectionException::Create(NlsMsgGet(FDORDBMS_121, "Not all the required properties are set"));

    mDbiConnection->SetConnectionString( mConnectionString );
    if( mDbiConnection != NULL )
    {
        // This is done in a separate method since we need to use __try/__except to catch the delay-loader's
        // Structured Exceptions, which is incompatible with having destructible objects on the stack:
        DbiOpen(skipPending);
    }

    if( mState == FdoConnectionState_Open )
    {
        // Add a lock definition to the table F_LockName (this table will exist
        // in all environments) and activate the user lock.
        // NOTES:
        //
        //  - In a Workspace Manager environment, the table F_LockName will be
        //    maintained to be able to handle the command FdoIGetLockOwners().
        //    Although Workspace Manager offers a system view where this kind
        //    of information could be retrieved from, it will not be used as it
        //    requires the user to have the Workspace Manager administrative
        //    privilege being granted. Since it has been decided that this
        //    privilege will not be granted to a common user, the table
        //    F_LockName needs to be maintained in this environment as well.
        //
        //  - Activating the current lock is not required in a Workspace Manager
        //    environment. However, since this layer does not (and should not)
        //    know of the underlying system schema, the function needs to be
        //    called to ensure the lock is activated in case the schema is one
        //    using the non Workspace Manager style locking concept. DBI will identify
        //    the used locking concept and execute the request only if necessary.
        //    No error is reported if the environment is incompatible with the
        //    issued request.

	
    }
    else if( mState == FdoConnectionState_Pending )
    {
        FdoPtr<FdoIConnectionInfo> connInfo = GetConnectionInfo();
        FdoPtr<FdoRdbmsConnectionPropertyDictionary> connDict = (FdoRdbmsConnectionPropertyDictionary*)connInfo->GetConnectionProperties();
        FdoPtr<FdoRdbmsProperty> prop = connDict->FindPropertyObject(FDO_RDBMS_CONNECTION_DATASTORE);
        if( prop )
            prop->SetIsPropertyRequired( true );
    }

    mDbiConnection->SetSchemaManager( CreateSchemaManager() );

    if( mState == FdoConnectionState_Open )
    {
        mLongTransactionManager = CreateLongTransactionManager();
        SetDefaultActiveSpatialContextName();
		mLockManager = CreateLockManager();
        if (mLongTransactionManager != NULL)
            mLongTransactionManager->SetLockManager(mLockManager);
    }

    return mState;
}

char* FdoRdbmsConnection::GetUser()
{
    return (mDbiConnection == NULL) ? NULL : mDbiConnection->GetUser();
}

// The function returns the unique user number for the current user.
int FdoRdbmsConnection::GetUserNum ()
{
    return mUserNum;
}

// The function sets the unique user number for the current user.
void FdoRdbmsConnection::SetUserNum (int UserNum)
{
    mUserNum = UserNum;
}

// The function returns the session id for the current user.
FdoInt64 FdoRdbmsConnection::GetUserSessionId()
{

    int             ccode        = -1;

    bool    isNull;

    FdoInt64        sessionId    = -1;

    FdoStringP      sqlStmt;

    GdbiStatement   *gdbiStmt    = NULL;

    GdbiQueryResult *gdbiResult  = NULL;

    try {

      // If the user session id has not yet been retrieved, get it from the
      // table F_DBOPEN using the the unique user number stored with the
      // connection. Otherwise return the already retrieved value.

      if (mUserSessionId != -1)
          return mUserSessionId;

      if (mUserNum != -1)
      {

        sqlStmt = 
            FdoStringP::Format(L"select %ls from %ls where %ls = %d",
                              (FdoString*)FDO_SESSION_COLUMN,
                              (FdoString*)FDO_DBOPEN_TABLE, 
                              (FdoString*)FDO_USER_NUM_COLUMN,
                              mUserNum);
        gdbiStmt = 
            GetDbiConnection()->GetGdbiConnection()->Prepare(
                                                        (const wchar_t*)sqlStmt);
        gdbiResult = gdbiStmt->ExecuteQuery();
        if (gdbiResult->ReadNext())
            sessionId = gdbiResult->GetInt64("sessionid", &isNull, &ccode);

        gdbiResult->Close();
        gdbiResult = NULL;
        gdbiStmt->Free();
        gdbiStmt = NULL;

        // Save the retrieved value for the next time.

        mUserSessionId = sessionId;

	  }

      return sessionId;

    }
    catch ( ... )
    {

        if (gdbiResult != NULL)
        {
            gdbiResult->Close();
            gdbiResult = NULL;
        }

        if (gdbiStmt != NULL)
        {
            gdbiStmt->Free();
            gdbiStmt = NULL;
        }

        throw;
    }
}

// The function sets the session id for the current user.
void FdoRdbmsConnection::SetUserSessionId (int value)
{
    mUserSessionId = value;
}

const wchar_t* FdoRdbmsConnection::GetConnectionString ()
{
    return (wchar_t*) get_ConnectionString();
}

void FdoRdbmsConnection::SetConnectionString (const wchar_t* value)
{
    mConnectionStringValid = true;
    set_ConnectionString( (const wchar_t*) value );
}

int FdoRdbmsConnection::GetConnectionTimeout ()
{
    return 0;
}

void FdoRdbmsConnection::SetConnectionTimeout (int value)
{
}


FdoConnectionState FdoRdbmsConnection::GetConnectionState ()
{
    return mState;
}


FdoITransaction *FdoRdbmsConnection::BeginTransaction ()
{
    if( mTransactionStarted )
        throw FdoConnectionException::Create(NlsMsgGet(FDORDBMS_387, "Connection already has an active transaction"));

    return new FdoRdbmsFeatureTransaction(this);
}

FdoICommand *FdoRdbmsConnection::CreateCommand (FdoInt32 commandType)
{
    FdoICommand* ret;

    switch (commandType)
    {
        case FdoCommandType_Select:
            ret = new FdoRdbmsSelectCommand (this);
            break;

        case FdoCommandType_Insert:
             ret = new FdoRdbmsInsertCommand (this);
             break;

         case FdoCommandType_Delete:
             ret = new FdoRdbmsDeleteCommand (this);
             break;

         case FdoCommandType_ApplySchema:
             ret = new FdoRdbmsApplySchemaCommand (this);
             break;

         case FdoCommandType_DestroySchema:
             ret = new FdoRdbmsDestroySchemaCommand (this);
             break;

         case FdoCommandType_DescribeSchema:
             ret = new FdoRdbmsDescribeSchemaCommand (this);
             break;

         case FdoCommandType_DescribeSchemaMapping:
             ret = new FdoRdbmsDescribeSchemaMappingCommand (this);
             break;

         case FdoCommandType_SQLCommand:
             ret = new FdoRdbmsSQLCommand (this);
             break;

         case FdoCommandType_Update:
             ret = new FdoRdbmsUpdateCommand (this);
             break;

         case FdoCommandType_AcquireLock:
             ret = new FdoRdbmsAcquireLock (this);
             break;

         case FdoCommandType_GetLockedObjects:
             ret = new FdoRdbmsGetLockedObjects (this);
             break;

         case FdoCommandType_GetLockInfo:
             ret = new FdoRdbmsGetLockInfo (this);
             break;

         case FdoCommandType_GetLockOwners:
             ret = new FdoRdbmsGetLockOwners (this);
             break;

         case FdoCommandType_ReleaseLock:
             ret = new FdoRdbmsReleaseLock (this);
             break;

         case FdoRdbmsCommandType_CreateSpatialIndex:
             ret = new FdoRdbmsCreateSpatialIndex (this);
             break;

         case FdoRdbmsCommandType_DestroySpatialIndex:
            ret = new FdoRdbmsDestroySpatialIndex (this);
            break;

         case FdoRdbmsCommandType_GetSpatialIndexes:
            ret = new FdoRdbmsGetSpatialIndexes (this);
            break;

         case FdoCommandType_ActivateSpatialContext:
            ret = new FdoRdbmsActivateSpatialContext (this);
            break;

         case FdoCommandType_CreateSpatialContext:
            ret = new FdoRdbmsCreateSpatialContext (this);
            break;

         case FdoCommandType_DestroySpatialContext:
            ret = new FdoRdbmsDestroySpatialContext (this);
            break;

         case FdoCommandType_GetSpatialContexts:
            ret = new FdoRdbmsGetSpatialContexts (this);
            break;

         case FdoCommandType_SelectAggregates:
            ret = new FdoRdbmsSelectAggregates( this );
            break;

         case FdoCommandType_CreateLongTransaction:
            ret = new FdoRdbmsCreateLongTransaction( this );
            break;

         case FdoCommandType_ActivateLongTransaction:
            ret = new FdoRdbmsActivateLongTransaction( this );
            break;

         case FdoCommandType_DeactivateLongTransaction:
            ret = new FdoRdbmsDeactivateLongTransaction( this );
            break;

         case FdoCommandType_GetLongTransactions:
            ret = new FdoRdbmsGetLongTransactions( this );
            break;

         case FdoCommandType_RollbackLongTransaction:
            ret = new FdoRdbmsRollbackLongTransaction( this );
            break;

         case FdoCommandType_CommitLongTransaction:
            ret = new FdoRdbmsCommitLongTransaction( this );
            break;

         case FdoCommandType_ListDataStores:
            ret = new FdoRdbmsGetDataStores( this );
            break;

         default:
             throw FdoConnectionException::Create(NlsMsgGet(FDORDBMS_10, "Command not supported"));
    }
    return (ret);
}

void FdoRdbmsConnection::CreateSysDb( FdoString *dbName, FdoString *dbPassword, FdoString *connectString)
{
	FdoSmPhMgrP physicalMgr = GetSchemaManager()->GetPhysicalSchema();
	FdoSmPhOwnerP newSchema = physicalMgr->GetDatabase()->CreateOwner( dbName );
    newSchema->SetPassword( dbPassword );
    newSchema->SetDescription( L"FDOSYS database" );
	newSchema->SetIsSystem( true );

	newSchema->Commit();
}


void FdoRdbmsConnection::CreateDb( FdoString *dbName, FdoString *dbDescription, FdoString *dbPassword, FdoString *connectString, FdoString *ltMode, FdoString *lckMode )
{
    FdoSmPhMgrP physicalMgr = GetSchemaManager()->GetPhysicalSchema();

    if ( physicalMgr->IsOwnerNameReserved( dbName ) )
        throw FdoCommandException::Create( 
            NlsMsgGet1( FDORDBMS_3, "Cannot create datastore, name '%1$ls' is a reserved word", dbName) 
        );

// Create a new Owner (physical schema) in the current database instance
	FdoSmPhOwnerP newSchema = physicalMgr->GetDatabase()->CreateOwner( dbName );
    newSchema->SetPassword( dbPassword );
	newSchema->SetDescription( dbDescription );

	FdoLtLockModeType lt_mode = NoLtLock;
	if ( wcscmp( ltMode, L"FDO" ) == 0 )
		lt_mode = FdoMode;
	else if ( wcscmp( ltMode, L"OWM" ) == 0 )
		lt_mode = OWMMode;

	newSchema->SetLtMode( lt_mode );
	
	FdoLtLockModeType lck_mode = NoLtLock;
	if (  wcscmp( lckMode, L"FDO" ) == 0 )
		lck_mode = FdoMode;
	else if ( wcscmp( lckMode, L"OWM" ) == 0 )
		lck_mode = OWMMode;

    FdoPtr<FdoIConnectionCapabilities> connectionCapabilities = this->GetConnectionCapabilities();
	if (connectionCapabilities->SupportsLocking() ||
		connectionCapabilities->SupportsLongTransactions())
		newSchema->SetLckMode( lck_mode);
	else
		newSchema->SetLckMode( NoLtLock );

    // Post the new Owner to the RDBMS.
    newSchema->Commit();

	// Check if FDOSYS is needed
	if (newSchema->GetLtMode() == FdoMode ||
		newSchema->GetLckMode() == FdoMode)	{
		// Create FDOSYS
		FdoSmPhOwnerP fdoOwner = physicalMgr->GetDatabase()->FindOwner(FDOSYS_OWNER);
		if (!fdoOwner)	{
			CreateSysDb( FDOSYS_OWNER, L"", connectString);
		}
	}

}

void FdoRdbmsConnection::DeleteDb( FdoString *dbName, FdoString *dbPassword, FdoString *connectString )
{
    FdoSmPhMgrP physicalMgr = GetSchemaManager()->GetPhysicalSchema();

    // Get the Owner (physical schema) in the current database instance
    FdoSmPhOwnerP owner = physicalMgr->GetOwner( dbName, L"", false );

    owner->SetElementState(FdoSchemaElementState_Deleted);

    // Post the new Owner to the RDBMS.
    owner->Commit();

    // Now that the database is successfully deleted, remove obsolete data from
    // the long transaction manager system tables.

    if (mLongTransactionManager != NULL)
        mLongTransactionManager->DropDbCleanUp(owner->GetName());
}

FdoIConnectionCapabilities* FdoRdbmsConnection::GetConnectionCapabilities()
{
    if (mConnectionCapabilities == NULL)
        mConnectionCapabilities = new FdoRdbmsConnectionCapabilities();
    FDO_SAFE_ADDREF(mConnectionCapabilities);
    return mConnectionCapabilities;
}

FdoICommandCapabilities* FdoRdbmsConnection::GetCommandCapabilities()
{
    if (mCommandCapabilities == NULL)
        mCommandCapabilities = new FdoRdbmsCommandCapabilities();
    FDO_SAFE_ADDREF(mCommandCapabilities);
    return mCommandCapabilities;
}

FdoISchemaCapabilities* FdoRdbmsConnection::GetSchemaCapabilities()
{
    if (mSchemaCapabilities == NULL)
        mSchemaCapabilities = new FdoRdbmsSchemaCapabilities();
    FDO_SAFE_ADDREF(mSchemaCapabilities);
    return mSchemaCapabilities;
}

FdoIFilterCapabilities* FdoRdbmsConnection::GetFilterCapabilities()
{
    if (mFilterCapabilities == NULL)
        mFilterCapabilities = new FdoRdbmsFilterCapabilities();
    FDO_SAFE_ADDREF(mFilterCapabilities);
    return mFilterCapabilities;
}

FdoIExpressionCapabilities* FdoRdbmsConnection::GetExpressionCapabilities()
{
    if (mExpressionCapabilities == NULL)
        mExpressionCapabilities = new FdoRdbmsExpressionCapabilities();
    FDO_SAFE_ADDREF(mExpressionCapabilities);
    return mExpressionCapabilities;
}

class Nothing : public FdoIRasterCapabilities
{
    virtual bool SupportsRaster () { return (false); }
    virtual bool SupportsStitching () { return (false); }
    virtual bool SupportsSubsampling () { return (false); }
    virtual bool SupportsDataModel (FdoRasterDataModel* model) { return (false); }
    virtual void Dispose () { delete this; }
};

FdoIRasterCapabilities* FdoRdbmsConnection::GetRasterCapabilities (void)
{
    return (new Nothing ());
}

FdoITopologyCapabilities* FdoRdbmsConnection::GetTopologyCapabilities()
{
    return (new FdoRdbmsTopologyCapabilities ());
}

void FdoRdbmsConnection::Dispose()
{
    delete this;
}

/// Gets an FdoIGeometryCapabilities interface describing the provider's support for geometry
FdoIGeometryCapabilities* FdoRdbmsConnection::GetGeometryCapabilities()
{
    if( mGeometryCapabilities == NULL )
        mGeometryCapabilities = new FdoRdbmsGeometryCapabilities();

    return FDO_SAFE_ADDREF(mGeometryCapabilities);
}

FdoSchemaManagerP FdoRdbmsConnection::GetSchemaManager()
{
    return mDbiConnection->GetSchemaManager();
}

FdoSchemaManagerP FdoRdbmsConnection::CreateSchemaManager()
{
    char *user = GetDbiConnection()->GetUser();
    FdoStringP currUser = FdoStringP(user);
    delete [] user;
    FdoStringP currSchema = GetDbiConnection()->GetDbSchemaName();

    // There is one rollback cache per connection
    if ( !mSchemaRollbackCache )
        mSchemaRollbackCache = new FdoSmPhRbCache();

    FdoSchemaManagerP schMgr = NewSchemaManager(
        GetDbiConnection()->GetGdbiConnection(),
        currUser,
        currSchema
    );

    FdoSmPhGrdMgrP physMgr = schMgr->GetPhysicalSchema()->SmartCast<FdoSmPhGrdMgr>();
	
    // Pass the config document information to the Schema Manager
    physMgr->SetConfiguration( 
        FdoPhysicalSchemaMappingP(CreateSchemaMapping())->GetProvider(),
        mConfigDoc,
        mConfigSchemas,
        mConfigMappings
    );

    // Pass down the schema rollback cache for this connection.
    // The rollback cache tracks DDL changes done by an ApplySchema command,
    // so they can be undone on transaction rollback.
    physMgr->SetRollbackCache( mSchemaRollbackCache );

    return schMgr;
}

// Returns the current Long Transaction Manager.
FdoRdbmsLongTransactionManager *FdoRdbmsConnection::GetLongTransactionManager()
{
	FDO_SAFE_ADDREF(mLongTransactionManager);
    return mLongTransactionManager;
}

FdoRdbmsLockManager *FdoRdbmsConnection::GetLockManager()
{
	if (mState != FdoConnectionState_Open)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_4_UNREADY)));

	return FDO_SAFE_ADDREF(mLockManager.p);
}

void FdoRdbmsConnection::SetSpatialManager(FdoRdbmsSpatialManager * spatialManager)
{
    this->mSpatialManager = FDO_SAFE_ADDREF(spatialManager);
}

FdoRdbmsSpatialManager *
FdoRdbmsConnection::GetSpatialManager()
{
    if (mState != FdoConnectionState_Open)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_4_UNREADY)));

    return FDO_SAFE_ADDREF(mSpatialManager.p);
}


FdoString * FdoRdbmsConnection::GetActiveSpatialContextName()
{
    return mActiveSpatialContextName;
}


void FdoRdbmsConnection::SetActiveSpatialContextName(FdoString * spatialContextName)
{
    mActiveSpatialContextName = spatialContextName;
}

void FdoRdbmsConnection::SetDefaultActiveSpatialContextName()
{
    mActiveSpatialContextName = L"";

    FdoSchemaManagerP smgr = GetSchemaManager();
    FdoSmLpSpatialContextsP scs = smgr->GetLpSpatialContexts();

    // We only set an active spatial context if one with numeric ID #0 exists.
    // That is the initial one created with a datastore, or intuited from
    // a foreign schema.
    bool found = false;
    FdoInt32 count = scs->GetCount();
    for (FdoInt32 i=0;  !found && i < count;  i++)
    {
        FdoSmLpSpatialContextP sc = scs->GetItem(i);
        if (sc->GetId() == 0)
        {
            found = true;
            mActiveSpatialContextName = sc->GetName();
        }
    }

#pragma message ("TODO: Address MySql default spatial context # of 1.")
    // Kludge:  Unfortunately, MySQL seems to have a defect wherein the
    // initial value of an "auto-increment" type must be 1.  The documented
    // override does not work.  So, we'll try for that too, if there is no
    // spatial context #0.
    for (FdoInt32 i=0;  !found && i < count;  i++)
    {
        FdoSmLpSpatialContextP sc = scs->GetItem(i);
        if (sc->GetId() == 1)
        {
            found = true;
            mActiveSpatialContextName = sc->GetName();
        }
    }
}
