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

class FdoRdbmsPvdBindDef
{
    std::vector<FdoRdbmsPvcBindDef*> params;
    size_t usedCnt;
public:
    FdoRdbmsPvdBindDef()
    {
        usedCnt = 0;
    }
    FdoRdbmsPvdBindDef(size_t cnt)
    {
        params.reserve(cnt);
        usedCnt = 0;
    }
    ~FdoRdbmsPvdBindDef()
    {
        Clear();
        std::vector<FdoRdbmsPvcBindDef*>::iterator it;
        for (it = params.begin(); it < params.end(); it++)
            delete *it;
        params.clear();
    }
    void Clear()
    {
        for (size_t idx = 0; idx < usedCnt; idx++)
        {
            FdoRdbmsPvcBindDef* pBind = params.at(idx);
            if (pBind->null_ind)
                free(pBind->null_ind);

            // the BLOB value was not allocated
            if (NULL != pBind->value.strvalue && pBind->type != FdoDataType_BLOB)
            {
                if (pBind->type == FdoRdbmsDataType_Geometry)
			    {
                    ((FdoIGeometry*)pBind->value.strvalue)->Release ();
				    pBind->value.strvalue = NULL;
			    }
                else if( pBind->valueNeedsFree )
			    {
				    delete[] (char*)pBind->value.strvalue;
				    pBind->value.strvalue = NULL;
				    pBind->valueNeedsFree = false;
			    }
            }
            memset (pBind, sizeof(FdoRdbmsPvcBindDef), 0x00);
        }
        usedCnt = 0;
    }
    size_t size()
    {
        return usedCnt;
    }
    FdoRdbmsPvcBindDef* at(size_t idx)
    {
        return params[idx];
    }
    FdoRdbmsPvcBindDef* push_back()
    {
        if (usedCnt < params.size())
            return params.at(usedCnt++);

        FdoRdbmsPvcBindDef* item = new FdoRdbmsPvcBindDef();
        memset (item, sizeof(FdoRdbmsPvcBindDef), 0x00);
        params.push_back(item);
        usedCnt++;
        return item;
    }
};

FdoRdbmsSelectCommand::FdoRdbmsSelectCommand (): mConnection( NULL ), mIdentifiers(NULL),
  mGroupingCol(NULL),
  mGroupingFilter(NULL),
  mOrderingIdentifiers(NULL),
  mBindParams(NULL)
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
    mBindParams(NULL)
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
    delete mBindParams;
}

FdoIFeatureReader *FdoRdbmsSelectCommand::Execute( bool distinct, FdoInt16 callerId  )
{
    // Flush out any outstanding modifications before selecting; so that the select
    // sees a current picture of the RDBMS.
    mIConnection->Flush();

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
        FdoPtr<FdoParameterValueCollection> params = GetParameterValues();
        flterProcessor->SetParameterValues(params);

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

        GdbiStatement* statement = mConnection->GetGdbiConnection()->Prepare( sqlString );

        std::vector<std::pair<FdoLiteralValue*, FdoInt64>>* paramsUsed = flterProcessor->GetUsedParameterValues();
        
        BindParameters(statement, paramsUsed);

        GdbiQueryResult *queryRslt = statement->ExecuteQuery();

        delete statement;

        if (mBindParams != NULL)
            mBindParams->Clear();

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

void FdoRdbmsSelectCommand::BindParameters(GdbiStatement* statement, std::vector<std::pair<FdoLiteralValue*, FdoInt64>>* params)
{
    size_t cntParams = (params != NULL) ? params->size() : 0;
    if (cntParams == 0)
        return;
    
    if (mBindParams == NULL)
        mBindParams = new FdoRdbmsPvdBindDef(cntParams);
    else
        mBindParams->Clear();

    char temp[32];
    GdbiCommands* cmds = mConnection->GetGdbiCommands();
    for (int idx = 0; idx < cntParams; idx++)
    {
        FdoRdbmsPvcBindDef* bind = mBindParams->push_back();
        cmds->alcnullind(1, &(bind->null_ind));
        cmds->set_null(bind->null_ind, 0, 0 );

        std::vector<std::pair<FdoLiteralValue*, FdoInt64>>::const_reference it = params->at(idx);
        FdoLiteralValue* val = it.first;
        switch (val->GetExpressionType())
        {
        case FdoExpressionItemType_DataValue:
            {
                FdoDataValue* dval = static_cast<FdoDataValue*>(val);
                FdoDataType dataType = dval->GetDataType();
                // Initialize the bind item
                *bind->name = L'\0';
                *bind->propertyName = L'\0';
                bind->type = dataType;
                bind->len = 64;     // Set the length to be 64
                bind->reader = NULL;
                bind->value.strvalue = NULL;
				bind->valueNeedsFree = false;
                sprintf(temp, "%d", idx+1); // Parm name are one based
                bool isNull = dval->IsNull();

                switch(dataType)
                {
                case FdoDataType_BLOB:
                    if (!isNull)
                    {
                        // TODO
                    }
                    break;
                case FdoDataType_DateTime:
                    if (!isNull)
                    {
                        FdoDateTimeValue* v = static_cast<FdoDateTimeValue*>(dval);
                        bind->value.strvalue = (void*)mFdoConnection->FdoToDbiTime(v->GetDateTime());
                        statement->Bind(idx+1, (int)strlen((char*)bind->value.strvalue), (char*)bind->value.strvalue);
                    }
                    else
                        statement->Bind(idx+1, 0, (char*)NULL, bind->null_ind);
                    break;
                case FdoDataType_Boolean:
                    if (!isNull)
                    {
                        FdoBooleanValue* v = static_cast<FdoBooleanValue*>(dval);
                        bind->value.lvalue = (int)v->GetBoolean() ? 1 : 0;
                        statement->Bind(idx+1, &bind->value.lvalue);
                    }
                    else
                        statement->Bind(idx+1, (int*)NULL, bind->null_ind);
                    break;
                case FdoDataType_Byte:
                    if (!isNull)
                    {
                        FdoByteValue* v = static_cast<FdoByteValue*>(dval);
                        bind->value.svalue = (short)v->GetByte();
                        statement->Bind(idx+1, &bind->value.svalue);
                    }
                    else
                        statement->Bind(idx+1, (short*)NULL, bind->null_ind);
                    break;
                case FdoDataType_Decimal:
                    if (!isNull)
                    {
                        FdoDecimalValue* v = static_cast<FdoDecimalValue*>(dval);
                        bind->value.gvalue = v->GetDecimal();
                        statement->Bind(idx+1, &bind->value.gvalue);
                    }
                    else
                        statement->Bind(idx+1, (double*)NULL, bind->null_ind);
                    break;
                case FdoDataType_Double:
                    if (!isNull)
                    {
                        FdoDoubleValue* v = static_cast<FdoDoubleValue*>(dval);
                        bind->value.gvalue = v->GetDouble();
                        statement->Bind(idx+1, &bind->value.gvalue);
                    }
                    else
                        statement->Bind(idx+1, (double*)NULL, bind->null_ind);
                    break;
                case FdoDataType_Int16:
                    if (!isNull)
                    {
                        FdoInt16Value* v = static_cast<FdoInt16Value*>(dval);
                        bind->value.svalue = v->GetInt16();
                        statement->Bind(idx+1, &bind->value.svalue);
                    }
                    else
                        statement->Bind(idx+1, (short*)NULL, bind->null_ind);
                    break;
                case FdoDataType_Int32:
                    if (!isNull)
                    {
                        FdoInt32Value* v = static_cast<FdoInt32Value*>(dval);
                        bind->value.lvalue = v->GetInt32();
                        statement->Bind(idx+1, &bind->value.lvalue);
                    }
                    else
                        statement->Bind(idx+1, (int*)NULL, bind->null_ind);
                    break;
                case FdoDataType_Int64:
                    if (!isNull)
                    {
                        FdoInt64Value* v = static_cast<FdoInt64Value*>(dval);
                        bind->value.llvalue = v->GetInt64();
                        statement->Bind(idx+1, &bind->value.llvalue);
                    }
                    else
                        statement->Bind(idx+1, (FdoInt64*)NULL, bind->null_ind);
                    break;
                case FdoDataType_Single:
                    if (!isNull)
                    {
                        FdoSingleValue* v = static_cast<FdoSingleValue*>(dval);
                        bind->value.fvalue = v->GetSingle();
                        statement->Bind(idx+1, &bind->value.fvalue);
                    }
                    else
                        statement->Bind(idx+1, (float*)NULL, bind->null_ind);
                    break;
                case FdoDataType_String:
                    if (!isNull)
                    {
                        FdoStringValue* v = static_cast<FdoStringValue*>(dval);
                        bind->value.strvalue = (void*)v->GetString();
                        bind->len = (int)(sizeof(FdoString) * (wcslen((FdoString*)bind->value.strvalue) + 1));
                        statement->Bind(idx+1, bind->len, (FdoString*)bind->value.strvalue);
                    }
                    else
                        statement->Bind(idx+1, 0, (FdoString*)NULL, bind->null_ind);
                    break;
                }
            }
            break;
        case FdoExpressionItemType_GeometryValue:
            {
                FdoGeometryValue* gval = static_cast<FdoGeometryValue*>(val);
                if (!gval->IsNull())
                {
                    FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
                    sprintf(temp, "%d", idx+1); // Parm name are one based
                    cmds->geom_srid_set(statement->GetQueryId(), temp, (long)it.second);
                    cmds->geom_version_set(statement->GetQueryId(), temp, mFdoConnection->GetSpatialGeometryVersion());
                    cmds->set_nnull(bind->null_ind, 0, 0 );
                    FdoPtr<FdoByteArray> ba = gval->GetGeometry();
                    FdoIGeometry* g = gf->CreateGeometryFromFgf(ba);
                    bind->type = FdoRdbmsDataType_Geometry;
                    bind->value.strvalue = (char*)g;
                    bind->len = sizeof(gval); // 4 / 8 depends of OS
                    statement->Bind((int)(idx+1), (int)(RDBI_GEOMETRY), (int)(sizeof(FdoIGeometry)), (char*)&bind->value.strvalue, bind->null_ind);
                }
                else
                {
                    bind->type = FdoRdbmsDataType_Geometry;
                    bind->value.strvalue = NULL;
                    bind->len = 0;
                    statement->Bind((int)(idx+1), (int)(RDBI_GEOMETRY), (int)(sizeof(FdoIGeometry)), (char*)&bind->value.strvalue, bind->null_ind);
                }
            }
            break;
        default:
            throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));
        }
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
