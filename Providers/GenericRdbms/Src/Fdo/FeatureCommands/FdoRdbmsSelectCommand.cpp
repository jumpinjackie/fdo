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
#include <FdoCommonExpressionExecutor.h>
#include <FdoCommonSchemaUtil.h>
#include <FdoExpressionEngine.h>
#include "Util/FdoExpressionEngineUtilDataReader.h"
#include "Util/FdoExpressionEngineUtilFeatureReader.h"
#include <Functions/Geometry/FdoFunctionLength2D.h>
#include <Functions/Geometry/FdoFunctionArea2D.h>

#define SELECT_CLEANUP \
        if ( qid != -1) {\
        try { \
            (void) mConnection->GetGdbiCommands()->free_cursor(qid); \
        } catch( ... ) { qid = -1; } \
        }



FdoRdbmsSelectCommand::FdoRdbmsSelectCommand (): mConnection( NULL ), mIdentifiers(NULL),
  mGroupingCol(NULL),
  mGroupingFilter(NULL),
  mOrderingIdentifiers(NULL),
  mBoundGeometries(NULL),
  mBoundGeometryCount(0)
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
    mOrderingIdentifiers(NULL),
    mBoundGeometries(NULL),
    mBoundGeometryCount(0)
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
    FreeBoundSpatialGeoms();
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
        FdoPtr<FdoRdbmsFilterProcessor>flterProcessor = mFdoConnection->GetFilterProcessor();

        FdoRdbmsFilterUtilConstrainDef filterConstrain;
        filterConstrain.distinct = distinct;
        filterConstrain.orderingOption = mOrderingOption;
        filterConstrain.selectedProperties = mIdentifiers;
        filterConstrain.groupByProperties = mGroupingCol;
        filterConstrain.orderByProperties = mOrderingIdentifiers;

   		// Verify if this is a special case we can optimize (no filter, no grouping filter,
		// and only aggregate functions Count() and/or SpatialExtents())
        FdoRdbmsFeatureReader *reader = GetOptimizedFeatureReader( classDefinition );
        if ( reader )
            return reader;

        // FDO supports expression functions that may not have native support in the
        // underlying system. If this is the case then the request has to be handled
        // by the Expression Engine.

        bool isValidFilter = true,
             isValidSelectList = true;

        if ( this->GetFilterRef() != NULL )
            isValidFilter = flterProcessor->IsValidExpression( this->GetFilterRef() );
        isValidSelectList = flterProcessor->IsValidExpression( mIdentifiers );

        if ( ( !isValidFilter ) || ( !isValidSelectList ) )
        {
            // Either the selected property list of the the filter is invalid. In any case
            // create a SQL statement that selects all properties for the current class.
            // If the filter is valid it is used to narrow the selected amount of data.

            FdoString *sqlStatement =
                    flterProcessor->FilterToSql( ((isValidFilter)
                                                        ? this->GetFilterRef()
                                                        : NULL),
                                                 this->GetClassNameRef()->GetText() );

            GdbiQueryResult *queryRslt = mConnection->GetGdbiConnection()->ExecuteQuery( sqlStatement );
            FdoPtr<FdoIFeatureReader> featureReader = new FdoRdbmsFeatureReader( FdoPtr<FdoIConnection>(GetConnection()),
                                                                                 queryRslt,
                                                                                 isFeatureClass,
                                                                                 classDefinition,
                                                                                 NULL,
                                                                                 NULL,
                                                                                 0,
                                                                                 NULL );

            // The Expression Engine cannot work with the class definition of type
            // "FdoSmLpClassDefinition". Instead it is necessary to get the corresponding
            // definition of type "FdoClassDefinition". This is done next.

            const FdoSmLpSchema* schema = mConnection->GetSchema( this->GetClassNameRef()->GetText() );
            FdoFeatureSchemasP fdoFeatureSchemas = mFdoConnection->GetSchemaManager()->GetFdoSchemas( schema->GetName() );
            FdoClassesP classCol = (FdoClassCollection *)fdoFeatureSchemas->FindClass( this->GetClassNameRef()->GetText() );
            FdoClassDefinitionP classDef = classCol->GetItem(0);

			// Create the collection of custom functions.
			FdoSmLpSchemasP	schemas = ((FdoSmLpSchema *) schema)->GetSchemas();

			FdoExpressionEngineFunctionCollection *userDefinedFunctions = GetUserDefinedFunctions( schemas->GetSpatialContextMgr()->GetSpatialContexts(), classDef );

			return new FdoExpressionEngineUtilFeatureReader(
                                                        classDef,
                                                        featureReader, 
                                                        this->GetFilterRef(),
                                                        mIdentifiers,
                                                        userDefinedFunctions);
		}

        // The selected properties and the filter are both valid. Do the normal processing.

		// Validate the filter
		if ( this->GetFilterRef() != NULL )
		{
			FdoPtr<FdoIFilterCapabilities> filterCaps = mFdoConnection->GetFilterCapabilities();
	
			FdoExpressionEngine::ValidateFilter( NULL, this->GetFilterRef(), NULL, filterCaps);
		}

        // Call FilterToSql just to populate the filter's list of geometric conditions;
        FdoString * sqlString = flterProcessor->FilterToSql( this->GetFilterRef(),
                                                             this->GetClassNameRef()->GetText(),
                                                             SqlCommandType_Select,
                                                             FdoCommandType_Select,
                                                             &filterConstrain,
                                                             isForUpdate,
                                                             callerId );

        FdoPtr<FdoRdbmsFilterProcessor::BoundGeometryCollection> boundGeometries = flterProcessor->GetBoundGeometryValues();
        FdoPtr<FdoRdbmsSecondarySpatialFilterCollection> geometricConditions = flterProcessor->GetGeometricConditions();
   		vector<int> * logicalOps = flterProcessor->GetFilterLogicalOps();

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
            if (callerId == FdoCommandType_SelectAggregates)
            {
                FdoSchemaManagerP pschemaManager = mConnection->GetSchemaUtil()->GetSchemaManager();
                FdoPtr<FdoFeatureSchemaCollection> schemas = pschemaManager->GetFdoSchemas(L"");
                FdoPtr<FdoCommonExpressionExecutor> expVer = FdoCommonExpressionExecutor::Create(schemas, GetClassNameRef());
                FdoPtr<FdoIExpressionCapabilities> expCap = mFdoConnection->GetExpressionCapabilities();
                expVer->ValidateIdentifiers(mIdentifiers, expCap);
            }
        }

		// Test the spatial queries validity. Not all filters are currently supported.
		CheckSpatialFilters( geometricConditions, logicalOps );

        GdbiStatement* statement = mConnection->GetGdbiConnection()->Prepare( sqlString );

        BindSpatialGeoms( statement, boundGeometries );

        GdbiQueryResult *queryRslt = statement->ExecuteQuery();

        delete statement;

        if (( mIdentifiers && mIdentifiers->GetCount() > 0) )
            return new FdoRdbmsFeatureSubsetReader( FdoPtr<FdoIConnection>(GetConnection()), queryRslt, isFeatureClass, classDefinition, NULL, mIdentifiers, geometricConditions, logicalOps );
        else
            return new FdoRdbmsFeatureReader( FdoPtr<FdoIConnection>(GetConnection()), queryRslt, isFeatureClass, classDefinition, NULL, NULL, 0, geometricConditions, logicalOps ); // The feature reader should free the queryRslt

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
        // Wrap in FdoPtr to remove original reference to original exception
        throw FdoCommandException::Create(ex->GetExceptionMessage(), FdoPtr<FdoException>(ex));
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

FdoExpressionEngineFunctionCollection* FdoRdbmsSelectCommand::GetUserDefinedFunctions( FdoSmLpSpatialContextCollection *scColl, FdoClassDefinition *classDef )
{
	// Length2D and Area2D require to pass in the 'geodetic' flag.
	// Check the associated coordinate system. In case it is geodetic, create a custom function.
	
	FdoPtr<FdoExpressionEngineFunctionCollection> userDefinedFunctions;

	if (classDef->GetClassType() == FdoClassType_FeatureClass)
	{
        FdoPtr<FdoGeometricPropertyDefinition> gpd = ((FdoFeatureClass*)classDef)->GetGeometryProperty();

		if ( gpd )
		{
			FdoStringP	scname = gpd->GetSpatialContextAssociation();

			if ( scname.GetLength() != 0 )
			{
				FdoSmLpSpatialContextP  sc = scColl->FindItem( scname );
				FdoStringP	wkt = sc->GetCoordinateSystemWkt();
				
				if ( wkt.Contains( L"PROJCS" ) )
					; // do nothing
				else if ( wkt.Contains( L"GEOGCS" ) )
				{
					userDefinedFunctions = FdoExpressionEngineFunctionCollection::Create();
					userDefinedFunctions->Add( FdoFunctionLength2D::Create(true));
					userDefinedFunctions->Add( FdoFunctionArea2D::Create(true));
				}
			}
		}
	}
	return FDO_SAFE_ADDREF(userDefinedFunctions.p);
}


FdoRdbmsFeatureReader *FdoRdbmsSelectCommand::GetOptimizedFeatureReader( const FdoSmLpClassDefinition *classDefinition )
{

	// Verify if this is a special case we can optimize (no grouping filter,
	// and only aggregate functions Count() and/or SpatialExtents())
    FdoRdbmsFeatureReader *reader = NULL;
	bool        bOtherAggrSelected = false;
	aggr_list   *selAggrList = new aggr_list;

	if ( (classDefinition->GetClassType() == FdoClassType_FeatureClass ) && mIdentifiers && 
		!mGroupingCol)
	{
        for (int i = 0; i < mIdentifiers->GetCount() && !bOtherAggrSelected; i++ )
        {
			FdoPtr<FdoIdentifier> identifier = mIdentifiers->GetItem(i);
			FdoComputedIdentifier* computedIdentifier = dynamic_cast<FdoComputedIdentifier*>(identifier.p);
              
            if (computedIdentifier) 
            {
				FdoPtr<FdoExpression> expr = computedIdentifier->GetExpression();
                FdoFunction* func = dynamic_cast<FdoFunction*>(expr.p);

                if (func && 0==FdoCommonOSUtil::wcsicmp(func->GetName(), FDO_FUNCTION_SPATIALEXTENTS))
                {
					FdoPtr<FdoExpressionCollection> args = func->GetArguments();
                    FdoPtr<FdoExpression> arg = args->GetItem(0);
                    FdoIdentifier* argId = dynamic_cast<FdoIdentifier*>(arg.p);

					AggregateElement *id = new AggregateElement;
					id->propName = argId->GetName();
                    id->name = computedIdentifier->GetName();
                    id->type = FdoPropertyType_GeometricProperty;

                    selAggrList->push_back( id );
				}
                else if (func && 0 == FdoCommonOSUtil::wcsicmp(func->GetName(), FDO_FUNCTION_COUNT))
                {
                    // Only if the argument count for the function is 1 do some
                    // special handling.

                    FdoPtr<FdoExpressionCollection> exprArgColl = func->GetArguments();
                    if (exprArgColl->GetCount() == 1)
					{
                        AggregateElement *id = new AggregateElement;
					    id->name = computedIdentifier->GetName();
					    id->type = FdoPropertyType_DataProperty;

					    selAggrList->push_back( id );
                    }
                    else
                    {
                        bOtherAggrSelected = true;
                    }
                }
                else
                {
                    bOtherAggrSelected = true;
                }			
			}
		}
	}

	// Now perform the actual select aggregates and return the data reader:
	if ( !bOtherAggrSelected && ( selAggrList->size() > 0 ))
    {
		reader = mFdoConnection->GetOptimizedAggregateReader( classDefinition, selAggrList, GetFilterRef() ); // The reader takes ownership of the selAggrList
    }
    else
    {
		// Sorry, no optimization. Clean up.
        for ( size_t j = 0; j < selAggrList->size(); j++ )
			delete selAggrList->at(j);

        delete selAggrList;
    }

    return reader;
}

void FdoRdbmsSelectCommand::CheckSpatialFilters( FdoRdbmsSecondarySpatialFilterCollection * geometricConditions, vector<int> *logicalOps )
{
	// Do this only in the case of spatial filters only
	FdoInt32	numFilters = geometricConditions ? geometricConditions->GetCount() : 0;
	FdoInt32	numLogicalOps = logicalOps ? (FdoInt32) logicalOps->size() : 0;

	if ( numFilters > 0 )
	{	
		// NOT operators not supported with spatial filters.
		for ( FdoInt32 i = 0;  i < numLogicalOps;  i++ )
		{
			if ( logicalOps->at(i) == -1 /*FdoUnaryLogicalOperations_Not*/ )
				throw FdoCommandException::Create( NlsMsgGet(FDORDBMS_533, "NOT operator not supported with spatial filters" ) );
		}

		// The number of binary operators should match the number of spatial filters. 
		if ( numLogicalOps != (numFilters - 1) )
			throw FdoCommandException::Create( NlsMsgGet(FDORDBMS_532, "AND and OR not supported in a query when mixing property with spatial filters" ) );
	}
}

void  FdoRdbmsSelectCommand::BindSpatialGeoms( GdbiStatement* statement, FdoRdbmsFilterProcessor::BoundGeometryCollection* geometries )
{
    if ( geometries->GetCount() > 0 )
    {
        FdoInt32 idx;

        FreeBoundSpatialGeoms();

        mBoundGeometryCount = geometries->GetCount();
        mBoundGeometries = new void*[mBoundGeometryCount];

        for ( idx = 0; idx < mBoundGeometryCount; idx++ ) 
        {
            FdoPtr<FdoRdbmsFilterProcessor::BoundGeometry> boundGeometry = geometries->GetItem(idx);
            mBoundGeometries[idx] = NULL;   // in case BindSpatialGeometry throws an exception.
            mBoundGeometries[idx] = mFdoConnection->BindSpatialGeometry( statement, boundGeometry, idx + 1 ); 
        }
    }
}

void  FdoRdbmsSelectCommand::FreeBoundSpatialGeoms()
{
    FdoInt32 idx;

    if ( mBoundGeometries )
    {
        for ( idx = 0; idx < mBoundGeometryCount; idx++ ) 
            mFdoConnection->BindSpatialGeometryFree( mBoundGeometries[idx] );

        delete[] mBoundGeometries;
        mBoundGeometryCount = 0;
        mBoundGeometries = NULL;
    }
 }
