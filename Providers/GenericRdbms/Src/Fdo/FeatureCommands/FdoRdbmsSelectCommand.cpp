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
 *
 */
#include "stdafx.h"
#include "FdoRdbmsSchemaUtil.h"

#include <malloc.h>
#ifdef _WIN32
#include <CRTDBG.H>
#endif

// FdoRdbmsSelectCommand
#include "FdoRdbmsSelectCommand.h"
#include "FdoRdbmsFeatureReader.h"
#include "FdoRdbmsFeatureSubsetReader.h"
#include "LockUtility.h"
#include "FdoRdbmsFilterProcessor.h"


#define SELECT_CLEANUP \
        if ( qid != -1) {\
        try { \
            (void) mConnection->GetGdbiCommands()->free_cursor(qid); \
        } catch( ... ) { qid = -1; } \
        }



FdoRdbmsSelectCommand::FdoRdbmsSelectCommand (): mConnection( NULL ), mIdentifiers(NULL),
  mGroupingCol(NULL),
  mGroupingFilter(NULL),
  mOrderingIdentifiers(NULL)
{
  mLockType           = FdoLockType_Exclusive;
  mLockStrategy       = FdoLockStrategy_Partial;
  mLockConflictReader = NULL;
  mIConnection        = NULL;
  mOrderingOption     = FdoOrderingOption_Ascending;
}

FdoRdbmsSelectCommand::FdoRdbmsSelectCommand (FdoIConnection *connection) :
    FdoRdbmsFeatureCommand<FdoISelect>(connection),
    mConnection ( NULL),
    mIdentifiers(NULL),
    mGroupingCol(NULL),
    mGroupingFilter(NULL),
    mOrderingIdentifiers(NULL)
{
  mConn = static_cast<FdoRdbmsConnection*>(connection);
  if( mConn )
      mConnection = mConn->GetDbiConnection();
  mLockType           = FdoLockType_Exclusive;
  mLockStrategy       = FdoLockStrategy_Partial;
  mLockConflictReader = NULL;
  mIConnection        = connection;
  mOrderingOption     = FdoOrderingOption_Ascending;
}

FdoRdbmsSelectCommand::~FdoRdbmsSelectCommand()
{
    FDO_SAFE_RELEASE(mLockConflictReader);
    FDO_SAFE_RELEASE(mIdentifiers);
    FDO_SAFE_RELEASE(mGroupingFilter);
    FDO_SAFE_RELEASE(mGroupingCol);
    FDO_SAFE_RELEASE(mOrderingIdentifiers);
}

FdoIFeatureReader *FdoRdbmsSelectCommand::Execute( bool distinct, FdoInt16 callerId  )
{
    int                 qid = -1;
    bool                res = false;
    const FdoSmLpClassDefinition *classDefinition = mConnection->GetSchemaUtil()->GetClass(this->GetClassNameRef()->GetText());

    bool isFeatureClass = ( classDefinition != NULL &&  classDefinition->GetClassType() == FdoClassType_FeatureClass );
    bool isForUpdate = HasLobProperty( classDefinition );

    if( mConnection == NULL )
        throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_13, "Connection not established"));
    try
    {
        FdoPtr<FdoRdbmsFilterProcessor>flterProcessor = FdoPtr<FdoRdbmsConnection>((FdoRdbmsConnection*)GetConnection())->GetFilterProcessor();

        FdoRdbmsFilterUtilConstrainDef filterConstrain;
        filterConstrain.distinct = distinct;
        filterConstrain.orderingOption = mOrderingOption;
        filterConstrain.selectedProperties = mIdentifiers;
        filterConstrain.groupByProperties = mGroupingCol;
        filterConstrain.orderByProperties = mOrderingIdentifiers;

        // Call FilterToSql just to populate the filter's list of geometric conditions;
        FdoString * sqlString = flterProcessor->FilterToSql( this->GetFilterRef(),
                                                             this->GetClassNameRef()->GetText(),
                                                             SqlCommandType_Select,
                                                             FdoCommandType_Select,
                                                             &filterConstrain,
                                                             isForUpdate,
                                                             callerId );

        FdoPtr<FdoRdbmsSecondarySpatialFilterCollection> geometricConditions = flterProcessor->GetGeometricConditions();
        FdoPtr<FdoIdentifierCollection> idsWithGeoms = FdoIdentifierCollection::Create();

        if (( mIdentifiers && mIdentifiers->GetCount() > 0) )
        {
            // Make sure that the properties are listed for any geometric conditions that require secondary filtering.
            if (geometricConditions != NULL)
            {
                for (FdoInt32 i=0;  i < geometricConditions->GetCount();  i++)
                {
                    FdoPtr<FdoRdbmsSpatialSecondaryFilter> ssf = geometricConditions->GetItem(i);
                    FdoString * ssfPropertyName = ssf->GetPropertyName();
                    FdoPtr<FdoIdentifier> ssfId = mIdentifiers->FindItem(ssfPropertyName);
                    if (ssfId == NULL)
                    {
                        // Property wasn't selected by the caller.
                        ssfId = FdoIdentifier::Create(ssfPropertyName);
                        idsWithGeoms->Add(ssfId);
                    }
                }
                // Convert to SQL again if we added geometric properties.
                if (idsWithGeoms->GetCount() > 0)
                {
                    // Make our own copy of the original list, to avoid a side effect of changing
                    // mIdentifiers, which the user can effectively see.  Perserve original order,
                    // effectively creating a new list that appends the geometry names.
                    for (FdoInt32 i = mIdentifiers->GetCount()-1;  i >= 0;  i--)
                    {
                        FdoPtr<FdoIdentifier> id = mIdentifiers->GetItem(i);
                        idsWithGeoms->Insert(0, id);
                    }
                    filterConstrain.selectedProperties = idsWithGeoms;
                    sqlString = flterProcessor->FilterToSql( this->GetFilterRef(),
                                                             this->GetClassNameRef()->GetText(),
                                                             SqlCommandType_Select,
                                                             FdoCommandType_Select,
                                                             &filterConstrain,
                                                             isForUpdate,
                                                             callerId );
                }
            }
        }

        GdbiQueryResult *queryRslt = mConnection->GetGdbiConnection()->ExecuteQuery( sqlString );

        if (( mIdentifiers && mIdentifiers->GetCount() > 0) )
            return new FdoRdbmsFeatureSubsetReader( FdoPtr<FdoIConnection>(GetConnection()), queryRslt, isFeatureClass, classDefinition, NULL, mIdentifiers, geometricConditions );
        else
            return new FdoRdbmsFeatureReader( FdoPtr<FdoIConnection>(GetConnection()), queryRslt, isFeatureClass, classDefinition, NULL, NULL, 0, geometricConditions ); // The feature reader should free the queryRslt

    }

    catch (FdoCommandException *ex)
    {
        ex;
        SELECT_CLEANUP;
        throw;
    }
    catch (FdoException *ex)
    {
        SELECT_CLEANUP;
        throw FdoCommandException::Create(ex->GetExceptionMessage(), ex);
    }

    catch ( ... )
    {
        SELECT_CLEANUP;
        throw;
    }
}

// Gets the IdentifierCollection that holds the list of property names to
// return with the result. If empty all properties of the specified class
// are returned.
FdoIdentifierCollection* FdoRdbmsSelectCommand::GetPropertyNames()
{
    if( NULL == mConnection )
         throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_13, "Connection not established"));

    if ( mIdentifiers == NULL )
        mIdentifiers = FdoIdentifierCollection::Create();

    mIdentifiers->AddRef();

    return mIdentifiers;
}

FdoIdentifierCollection* FdoRdbmsSelectCommand::GetOrdering()
{
    if( NULL == mConnection )
        throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_13, "Connection not established"));

    if( mOrderingIdentifiers == NULL )
        mOrderingIdentifiers = FdoIdentifierCollection::Create();

    mOrderingIdentifiers->AddRef();

    return mOrderingIdentifiers;
}

FdoIdentifierCollection* FdoRdbmsSelectCommand::GetGrouping()
{
    if( NULL == mConnection )
        throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_13, "Connection not established"));

    if( mGroupingCol == NULL)
        mGroupingCol = FdoIdentifierCollection::Create();

    mGroupingCol->AddRef();

    return mGroupingCol;
}

// Gets the LockType value.
FdoLockType FdoRdbmsSelectCommand::GetLockType()
{
    return mLockType;
}

// Sets the LockType value.
void FdoRdbmsSelectCommand::SetLockType(FdoLockType value)
{
#pragma message ("ToDo: FdoRdbmsSelectCommand::SetLockType")
#if 0
    unsigned long   vndr_db_version;
    dbi_context_def *current_context;

    // Throw an exception if the given lock type is not valid.

    if ((value == FdoLockType_None       ) ||
        (value == FdoLockType_Unsupported)    )
          throw FdoCommandException::Create(
                    LockUtility::GetExceptionMessage(
                                         LockUtility::LOCK_TYPE_INPUT_ERROR,
                                         ((value == FdoLockType_None)
                                            ? L"FdoLockType_None"
                                            : L"FdoLockType_Unsupported"),
                                         L"FdoISelect"));

    // Some of the available lock types are only available on an Oracle 10g
    // installation. The selection of those types in an environment that is
    // not an Oracle 10g installation must be denied.

    if ((current_context = mConnection->GetCtxt()) == NULL)
          throw FdoCommandException::Create(
                    LockUtility::GetExceptionMessage(
                                                LockUtility::INVALID_CONTEXT,
                                                L"FdoISelect"));

    vndr_db_version = current_context->adb->vndr_db_version;
    if (vndr_db_version < DBI_ORACLE_DBVERSION9)
        throw FdoCommandException::Create(
                    LockUtility::GetExceptionMessage(
                                       LockUtility::UNSUPPORTED_ORACLE_VERSION,
                                       L"FdoISelect"));
    else
      if ((vndr_db_version >= DBI_ORACLE_DBVERSION9                    ) &&
          (vndr_db_version <  DBI_ORACLE_DBVERSION10                   ) &&
          ((value          == FdoLockType_Exclusive               ) ||
           (value          == FdoLockType_LongTransactionExclusive)    )    )
            throw FdoCommandException::Create(
                    LockUtility::GetExceptionMessage(
                               LockUtility::UNSUPPORTED_LOCK_TYPE,
                               ((value == FdoLockType_Exclusive)
                                    ? L"FdoLockType_Exclusive"
                                    : L"FdoLockType_LongTransactionExclusive"),
                               L"FdoISelect"));

    mLockType = value;
#endif
}

// Gets the LockStrategy value (see "Feature Locking Commands").
FdoLockStrategy FdoRdbmsSelectCommand::GetLockStrategy()
{
    return mLockStrategy;
}

// Sets the LockStrategy value (see "Feature Locking Commands").
void FdoRdbmsSelectCommand::SetLockStrategy(FdoLockStrategy value)
{
    mLockStrategy = value;
}

// Executes the select command and returns a reference to an IFeatureReader.
// During execution an attempt is made to place a lock on each feature. If
// there are conflicts the conflicts are reported and made available to the
// user via a lock conflict reader. The user retrieves the lock conflict
// reader when executing the function "GetLockConflicts()".
// NOTE: The current implementation is a work-around due some timing issues as
//       explained: When using DBI to apply a lock on an object the lock is
//       actually applied only when the object is fetched via the procedure
//       dbi_fetch. In this command the fetching of the object is delayed until
//       the user actually queries the feature reader. That means that locks
//       are applied at that time. As a result the feature reader should have a
//       method to retrieve the lock conflicts because this object documents
//       them. This is not the case. Therefore, until a better solution is found
//       the work-around is to have two passes for the execution of the command:
//       the first one actually locks the objects and provides lock conflict
//       information whereas the second pass prepares the feature reader for the
//       user to retrieve the requested data.
FdoIFeatureReader* FdoRdbmsSelectCommand::ExecuteWithLock()
{

    FdoFilter       *tempFilter     = NULL;
    FdoIdentifier   *tempClassName  = NULL;
    FdoIAcquireLock *acquireLockCmd = NULL;

    // If a lock conflict reader already exists dispose it.

    FDO_SAFE_RELEASE(mLockConflictReader);

    // Create the command to lock the objects, set the command parameters
    // and execute it.

    acquireLockCmd = (FdoIAcquireLock *) mConn->CreateCommand(
                                                    FdoCommandType_AcquireLock);

    tempClassName = GetFeatureClassName();
    acquireLockCmd->SetFeatureClassName(tempClassName);
    tempClassName->Release();
    tempFilter = GetFilter();
    acquireLockCmd->SetFilter(tempFilter);
    acquireLockCmd->SetLockStrategy(mLockStrategy);
    acquireLockCmd->SetLockType(mLockType);

    mLockConflictReader = (FdoRdbmsLockConflictReader *)acquireLockCmd->Execute();

    acquireLockCmd->Release();
    acquireLockCmd = NULL;

    // Get the data as requested by the user.

    return (Execute());
}

// The function returns the lock conflict reader containing any lock conflicts
// as detected during the execution of the select-and-lock command.
FdoILockConflictReader* FdoRdbmsSelectCommand::GetLockConflicts()
{
    FdoRdbmsLockConflictReader *temp_lock_conflict_reader = mLockConflictReader;
    mLockConflictReader = NULL;
    return temp_lock_conflict_reader;
}

////////////////////////////////////////////////////////////////////////////////
bool FdoRdbmsSelectCommand::HasLobProperty( const FdoSmLpClassDefinition *classDefinition )
{
    bool    forUpdate = false;
    const   FdoSmLpPropertyDefinitionCollection *propertyDefinitions = classDefinition->RefProperties();

    for ( int i = 0; i < propertyDefinitions->GetCount(); i++ )
    {
        const FdoSmLpPropertyDefinition *propertyDefinition = propertyDefinitions->RefItem(i);

        const FdoSmLpDataPropertyDefinition* dataProp =
                    dynamic_cast<const FdoSmLpDataPropertyDefinition*>(propertyDefinition);

        // LOBs require "SELECT ... FOR UPDATE"
        if ( dataProp && dataProp->GetDataType() == FdoDataType_BLOB )
        {
            forUpdate = true;
            break;
        }
    }
    return forUpdate;
}
