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

// FdoRdbmsDeleteCommand
#include "LockUtility.h"
#include "FdoRdbmsDeleteCommand.h"
#include "FdoRdbmsFilterProcessor.h"
#include "FdoRdbmsFilterUtil.h"
#include "FdoCommonOSUtil.h"
#include <Sm/Lp/AssociationPropertyDefinition.h>
#include <Sm/Lp/ObjectPropertyDefinition.h>
#include <Sm/Lp/ObjectPropertyClass.h>
#include <Sm/Lp/PropertyMappingSingle.h>

#ifndef _WIN32
#define  strnicmp strncasecmp
#endif

static char* TRANSACTION_NAME = "FdoRdbmsDeleteCommand::Execute";

#define DELETE_CLEANUP \
            if( sqlFilter ) {\
                delete[] sqlFilter; sqlFilter = NULL; }\
            if (bBeginTransaction == true) \
            { \
                bBeginTransaction = false; \
                mConnection->GetGdbiCommands()->tran_rolbk(); \
            }

//
// This helper class is used to scan the filter and determine if the filter is a simple filter that uses
// class attributes only and not spatial conditions.
class SimpleFilterProcessor : public FdoIExpressionProcessor, public FdoIFilterProcessor
{
private:
    bool        simpleFilter;

    void HandleExpr( FdoExpression *exp )
    {
        exp->Process( this );
    }
    void HandleFilter( FdoFilter *filter )
    {
        filter->Process( this );
    }
public:
    SimpleFilterProcessor( )
    {
        simpleFilter = true;
    }

    bool IsSimple() { return simpleFilter; }

    void Dispose() { delete this; }

    void ProcessBinaryExpression(FdoBinaryExpression& expr)
    {
        HandleExpr( FdoPtr<FdoExpression>(expr.GetLeftExpression()) );
        HandleExpr( FdoPtr<FdoExpression>(expr.GetRightExpression()) );
    }
    void ProcessBooleanValue(FdoBooleanValue& expr) {  }
    void ProcessByteValue(FdoByteValue& expr){   }
    void ProcessDateTimeValue(FdoDateTimeValue& expr){   }
    void ProcessDoubleValue(FdoDoubleValue& expr){   }
    void ProcessDecimalValue(FdoDecimalValue& expr){   }
    void ProcessInt16Value(FdoInt16Value& expr){  }
    void ProcessInt32Value(FdoInt32Value& expr){  }
    void ProcessInt64Value(FdoInt64Value& expr){  }
    void ProcessSingleValue(FdoSingleValue& expr){  }
    void ProcessStringValue(FdoStringValue& expr){  }
    void ProcessBLOBValue(FdoBLOBValue& expr){   }
    void ProcessCLOBValue(FdoCLOBValue& expr){  }
    void ProcessFunction(FdoFunction& expr){   }
    void ProcessGeometryValue(FdoGeometryValue& expr){  }
    void ProcessParameter(FdoParameter& expr){  }
    void ProcessUnaryExpression(FdoUnaryExpression& expr)
    {
        HandleExpr( expr.GetExpression() );
    }

    void ProcessBinaryLogicalOperator(FdoBinaryLogicalOperator& filter)
    {
        HandleFilter( FdoPtr<FdoFilter>(filter.GetLeftOperand()) );
        HandleFilter( FdoPtr<FdoFilter>(filter.GetRightOperand()) );
    }
    void ProcessComparisonCondition(FdoComparisonCondition& filter)
    {
        HandleExpr( FdoPtr<FdoExpression>(filter.GetLeftExpression()) );
        HandleExpr( FdoPtr<FdoExpression>(filter.GetRightExpression()) );
    }
    void ProcessDistanceCondition(FdoDistanceCondition& filter)
    {
        // If the filter uses a distance condition, then this is not a simple filter.
        simpleFilter = false;
    }

    void ProcessInCondition(FdoInCondition& filter)
    {
        ProcessIdentifier( *(FdoPtr<FdoIdentifier>(filter.GetPropertyName())) );
    }
    void ProcessNullCondition(FdoNullCondition& filter)
    {
        ProcessIdentifier( *(FdoPtr<FdoIdentifier>(filter.GetPropertyName())) );
    }
    void ProcessSpatialCondition(FdoSpatialCondition& filter)
    {
        // If the filter uses a spatial condition, then this is not a simple filter.
        simpleFilter = false;
    }
    void ProcessUnaryLogicalOperator(FdoUnaryLogicalOperator& filter)
    {
        HandleFilter( FdoPtr<FdoFilter>(filter.GetOperand()) );

    }

    void ProcessIdentifier(FdoIdentifier& expr)
    {
        int length;
        // If any of the identifiers is a decorated name of the form <obj proper>.<property>, then this is
        // not a simple filter.
        if( expr.GetScope( length ) && length > 0 )
            simpleFilter = false;
    }

    void ProcessComputedIdentifier(FdoComputedIdentifier& expr)
    {
    }
};

FdoRdbmsDeleteCommand::FdoRdbmsDeleteCommand (): mConnection( NULL )
{
    mLockConflictReader = NULL;
}

FdoRdbmsDeleteCommand::FdoRdbmsDeleteCommand (FdoIConnection *connection):
    FdoRdbmsFeatureCommand<FdoIDelete>(connection),
    mConnection( NULL )
{
    mConn = dynamic_cast<FdoRdbmsConnection*>(connection);
    if( mConn )
       mConnection = mConn->GetDbiConnection();
    mLockConflictReader = NULL;
}

FdoRdbmsDeleteCommand::~FdoRdbmsDeleteCommand()
{
    FDO_SAFE_RELEASE(mLockConflictReader);
}

FdoInt32 FdoRdbmsDeleteCommand::Execute ()
{
#define     DELETE_BATCH_SIZE       200   // Number of object to delete at a time when using complex filters
    FdoIdentifier*      className;

    if( NULL == mConnection )
        throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_13, "Connection not established"));

    className = this->GetClassNameRef();
    if (!className)
        throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_35, "Class is null"));

    if( this->GetFilterRef() == NULL )
        return InternalExecute ();

    SimpleFilterProcessor  tester;
    this->GetFilterRef()->Process( &tester );

    if( tester.IsSimple() )
    {
        return InternalExecute ();
    }

    // If the filter is not simple, then we need convert that filter to a simple filter of the form: where identity1 in (....) and identity2 in (.....) ...
    // We do that by executing the query and buffering the result DELETE_BATCH_SIZE row at a time.
    FdoPtr<FdoIConnection> conn = GetConnection();
    if( conn == NULL )
        throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_13, "Connection not established"));

    FdoPtr<FdoISelect> selCmd = (FdoISelect*)conn->CreateCommand( FdoCommandType_Select );
    selCmd->SetFeatureClassName( className );
    selCmd->SetFilter( this->GetFilterRef() );

    // Limit the selected property to the identity properties
    FdoPtr<FdoIdentifierCollection> selPropertyNames = selCmd->GetPropertyNames();

    const FdoSmLpClassDefinition *classDefinition = mConnection->GetSchemaUtil()->GetClass(className->GetText());
    const FdoSmLpDataPropertyDefinitionCollection *properties = classDefinition->RefIdentityProperties();

    if( properties->GetCount() == 0 ) // We cannot continue if we have no identity properties; just use the filter as is
        return InternalExecute ();

    FdoInCondition   **inCnds = new FdoInCondition*[properties->GetCount()];
    int count = 0;

    FdoFilter *old_filter = NULL;
    FdoFilter *filter = NULL;
    try
    {
        for( int i =0; properties!= NULL && i<properties->GetCount(); i++ )
        {
            FdoIdentifier *name = FdoIdentifier::Create( properties->RefItem(i)->GetName() );
            selPropertyNames->Add( name );

            // Create an in condition filter for this property
            inCnds[i] = FdoInCondition::Create();
            inCnds[i]->SetPropertyName( name );

            name->Release();
        }
        filter = inCnds[0];
        filter->AddRef();
        for( int i =1; properties!= NULL && i<properties->GetCount(); i++ )
        {
            FdoFilter *tmp = FdoFilter::Combine(filter, FdoBinaryLogicalOperations_And , inCnds[i] );
            filter->Release();
            filter = tmp;
        }
        old_filter = GetFilter();
        SetFilter( filter );


        FdoPtr<FdoIFeatureReader> reader = selCmd->Execute();
        if( reader != NULL  )
        {
            int i,j;

            // If we have more than one identity property, then and'ing multiple IN condition may return the
            // the wrong data. In that case we only include one value in the IN condition which result in deleting one object at a time.
            int batch_size = ( properties->GetCount() > 1 )?1:DELETE_BATCH_SIZE;
            for ( j=0; reader->ReadNext(); )
            {

                for( i =0; properties!= NULL && i<properties->GetCount(); i++ )
                {
                    FdoPtr<FdoValueExpressionCollection> values = FdoPtr<FdoValueExpressionCollection>(inCnds[i]->GetValues());
                    values->Add( FdoPtr<FdoDataValue>(FdoDataValue::Create((const wchar_t*)reader->GetString( properties->RefItem(i)->GetName() ) ) ) );
                }

                if( (++j) == batch_size )
                {
                    j = 0;
                    count += InternalExecute ();
                    for( i =0; properties!= NULL && i<properties->GetCount(); i++ )
                    {
                        FdoPtr<FdoValueExpressionCollection> values = FdoPtr<FdoValueExpressionCollection>(inCnds[i]->GetValues());
                        values->Clear();
                    }
                }
            }
            if( j != 0 )
                count += InternalExecute ();

            SetFilter( old_filter );
            old_filter->Release();
            filter->Release();
        }

        for( int i =0; properties!= NULL && i<properties->GetCount(); i++ )
        {
            inCnds[i]->Release();
        }
        delete[] inCnds;

        return count;
    }
    catch( ... )
    {
        if( inCnds )
        {
            for( int i =0; properties!= NULL && i<properties->GetCount(); i++ )
            {
                inCnds[i]->Release();
            }
            delete[] inCnds;
        }
        FDO_SAFE_RELEASE( old_filter );
        FDO_SAFE_RELEASE( filter );


        throw;
    }

}

FdoInt32 FdoRdbmsDeleteCommand::InternalExecute ()
{
    FdoIdentifier* className;
    int            count = 0;
    bool           featureClass = false;
    bool           bBeginTransaction = false;
    wchar_t        *sqlFilter = NULL;
    bool           lockConflictsChecked = false;
    bool           throw_exception      = true;

    className = this->GetClassNameRef();

    const FdoSmLpClassDefinition *classDefinition = mConnection->GetSchemaUtil()->GetClass(className->GetText());

    if( ! CheckAssociationDependancies( L"", classDefinition, this->GetFilterRef() ) )
        throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_318, "Associated objects need to be deleted first" ) );

    if( classDefinition != NULL &&  classDefinition->GetClassType() == FdoClassType_FeatureClass )
        featureClass = true;

    const FdoSmLpClassDefinition* currentClass = mConnection->GetSchemaUtil()->GetClass(className->GetText());
    try
    {
        if (mConn->GetIsTransactionStarted() == false)
        {
            mConnection->GetGdbiCommands()->tran_begin(TRANSACTION_NAME);
            bBeginTransaction = true;
        }

        // Check the lock situation. If there is a lock conflict the delete
        // operation will be rejected.
        FdoPtr<FdoRdbmsFilterProcessor>flterProcessor = FdoPtr<FdoRdbmsConnection>((FdoRdbmsConnection*)GetConnection())->GetFilterProcessor();

        // The following sets up an identifier list containing the feature id property,
        // if the class is a feature class.
        // For feature classes, the sqlFilter is embedded in an IN clause that must
        // select the feature id.
        // For non-feature classes, selecting the identity properties is fine.
        FdoIdentifierCollection* sqlFilterProps = FdoRdbmsFilterUtil::GetFeatIdSelList( classDefinition );
        FdoRdbmsFilterUtilConstrainDef  filterConstrain;
        filterConstrain.selectedProperties = sqlFilterProps;
        const wchar_t *tmpSelect = flterProcessor->FilterToSql( this->GetFilterRef(), className->GetText(), sqlFilterProps ? SqlCommandType_Select : SqlCommandType_Update, FdoCommandType_Delete, &filterConstrain );
        sqlFilter = new wchar_t[wcslen(tmpSelect)+1];
        wcscpy( sqlFilter, tmpSelect);
        FDO_SAFE_RELEASE( sqlFilterProps );
       
        if (!CheckLocks(true, true))
        {
            DELETE_CLEANUP;
            return count;
        }
        lockConflictsChecked = true;

        count = DeleteRelatedObjects( NULL, currentClass, currentClass->RefIdentityProperties() );
        
        if ( bBeginTransaction )
        {
            bBeginTransaction = false;
            mConnection->GetGdbiCommands()->tran_end( TRANSACTION_NAME );
        }
        if( sqlFilter )
        {
            delete[] sqlFilter;
            sqlFilter = NULL;
        }

    }

    catch (FdoCommandException *ex)
    {
        ex;
        DELETE_CLEANUP;
        throw;
    }

    catch (FdoException *ex)
    {
        DELETE_CLEANUP;
        FdoCommandException *exp = FdoCommandException::Create(ex->GetExceptionMessage(), ex);
        ex->Release();
        throw exp;
    }

    catch ( ... )
    {
        DELETE_CLEANUP;
        throw;
    }

    return count;
}

// The function returns the handle to the lock conflicts. By doing this the user
// takes over ownership of the object and the internal pointer to the lock con-
// flict reader is removed. Hence, any subsequent request for the lock conflict
// reader will return a NULL pointer unless a new delete operation is executed.
FdoILockConflictReader* FdoRdbmsDeleteCommand::GetLockConflicts()
{
    FdoILockConflictReader *temp_lock_conflict_reader = mLockConflictReader;
    mLockConflictReader = NULL;
    return temp_lock_conflict_reader;
}

//
// This method make sure that we can procceed with the delete. Basically if one of the association properties
// has a delete rule of prevent and one or more associated objects still exist, then the delete will not proceed and the caller
// have to clean up those associated objects first.
// TODO: Need to detect circular dependancy cases;
bool FdoRdbmsDeleteCommand::CheckAssociationDependancies( const wchar_t* scope, const FdoSmLpClassDefinition* currentClass, FdoFilter *filter )
{
	GdbiQueryResult     *query = NULL;
    const FdoSmLpAssociationPropertyDefinition *associationPropertyDefinition = NULL;
    const FdoSmLpPropertyDefinitionCollection *properties = currentClass->RefProperties();
    for (int i=0; i<properties->GetCount(); i++)
    {
        const FdoSmLpPropertyDefinition *propertyDefinition = properties->RefItem(i);
        if(  propertyDefinition  == NULL )
            continue;

        FdoStringP propName = scope;
        if(propName.GetLength() != 0 )
        {
            propName += L".";
            propName += propertyDefinition->GetName();
        }
        else
            propName = propertyDefinition->GetName();

        if(propertyDefinition->GetPropertyType() == FdoPropertyType_AssociationProperty )
        {
            associationPropertyDefinition = static_cast<const FdoSmLpAssociationPropertyDefinition*>(propertyDefinition);

            if( associationPropertyDefinition->GetReadOnly() )
                continue;

            if( associationPropertyDefinition->GetDeleteRule() == FdoDeleteRule_Prevent )
            {
                int  qid = -1;
                int  ccode = -1;

                FdoPtr<FdoRdbmsFilterProcessor>flterProcessor = FdoPtr<FdoRdbmsConnection>((FdoRdbmsConnection*)GetConnection())->GetFilterProcessor();
                FdoPtr<FdoFilter> newFilter;
                const FdoSmLpDataPropertyDefinitionCollection *idProps = associationPropertyDefinition->RefAssociatedClass()->RefIdentityProperties();
                for(int j=0; j<idProps->GetCount(); j++)
                {
                    FdoStringP assPropName = propName;
                    assPropName += ".";
                    assPropName += idProps->RefItem(i)->GetName();
                    FdoPtr<FdoFilter> NotNullFilter =  FdoUnaryLogicalOperator::Create( FdoPtr<FdoFilter>(FdoNullCondition::Create( assPropName )), FdoUnaryLogicalOperations_Not );
                    if( newFilter == NULL )
                        newFilter = FdoFilter::Combine( filter,FdoBinaryLogicalOperations_And, NotNullFilter );
                    else
                        newFilter = FdoFilter::Combine( newFilter,FdoBinaryLogicalOperations_And, NotNullFilter );
                }
                try
                {
					query = mConnection->GetGdbiConnection()->ExecuteQuery( flterProcessor->FilterToSql( newFilter, this->GetClassNameRef()->GetText(), SqlCommandType_Update, FdoCommandType_Select, NULL, false ) );
                }
                catch(...)
                {
					if ( query  )
						delete query;
                    throw;
                }
                if ( query && query->ReadNext() )
                {
                    // The query returned one or more objects. No delete should happen in this case.
                    query->Close();
					delete query;
					query = NULL;
                    return false;
                }
                if ( query  )
                {
					query->Close();
					delete query;
					query = NULL;
				}
            }
            else if( associationPropertyDefinition->GetDeleteRule() == FdoDeleteRule_Cascade )
            {
                return CheckAssociationDependancies( propName, associationPropertyDefinition->RefAssociatedClass(), filter );
            }
        }
        else if(propertyDefinition->GetPropertyType() == FdoPropertyType_ObjectProperty )
        {
            const FdoSmLpObjectPropertyDefinition* objectPropertyDefinition = static_cast<const FdoSmLpObjectPropertyDefinition*>(propertyDefinition);
            const FdoSmLpPropertyMappingDefinition* mappping = objectPropertyDefinition->RefMappingDefinition();
            if( mappping != NULL )
            {
                if(  mappping->GetType() ==  FdoSmLpPropertyMappingType_Concrete ||
                     mappping->GetType() ==  FdoSmLpPropertyMappingType_Single  )
                {
                    return CheckAssociationDependancies( propName, objectPropertyDefinition->RefTargetClass(), filter );
                }
                else
                    throw FdoSchemaException::Create(NlsMsgGet(FDORDBMS_27, "Unsupported Property mapping type"));
              }
        }
    }
    return true;
}

//
// This method recursively follow all associations and object properties and
// delete the affected objects.
FdoInt32 FdoRdbmsDeleteCommand::DeleteRelatedObjects( const wchar_t* scope, const FdoSmLpClassDefinition* currentClass, const FdoSmLpDataPropertyDefinitionCollection *fkProps, bool followRelOnly )
{
    FdoInt32    deletedObjects = 0;
    int         count = 0;
    const FdoSmLpAssociationPropertyDefinition *associationPropertyDefinition = NULL;
    const FdoSmLpPropertyDefinitionCollection *properties = currentClass->RefProperties();
    for (int i=0; i<properties->GetCount(); i++)
    {
        const FdoSmLpPropertyDefinition *propertyDefinition = properties->RefItem(i);
        if(  propertyDefinition  == NULL )
            continue;

        FdoStringP propName;
        if( scope != NULL  )
        {
            propName = scope;
            propName += L".";
            propName += propertyDefinition->GetName();
        }
        else
            propName = propertyDefinition->GetName();

        if(propertyDefinition->GetPropertyType() == FdoPropertyType_AssociationProperty )
        {
            associationPropertyDefinition = static_cast<const FdoSmLpAssociationPropertyDefinition*>(propertyDefinition);

            if( associationPropertyDefinition->GetReadOnly() )
                continue;

            if( associationPropertyDefinition->GetDeleteRule() == FdoDeleteRule_Cascade )
            {
                const FdoSmLpDataPropertyDefinitionCollection  *identProps;
                FdoSmLpDataPropertyDefinitionCollection  props;
                if( associationPropertyDefinition->GetIdentityProperties()->GetCount() != 0 )
                {
                    for(int i=0; i<associationPropertyDefinition->GetIdentityProperties()->GetCount(); i++ )
                    {
                        FdoStringP proName = associationPropertyDefinition->GetIdentityProperties()->GetString( i );
                        const FdoSmLpDataPropertyDefinition  *prop = (const FdoSmLpDataPropertyDefinition  *)associationPropertyDefinition->RefAssociatedClass()->RefProperties()->RefItem( proName );
                        props.Add( (FdoSmLpDataPropertyDefinition  *)prop );
                    }
                    identProps = &props;
                }
                else
                { // Use the default identities; those of the associated class
                    identProps = associationPropertyDefinition->RefAssociatedClass()->RefIdentityProperties();
                }
                deletedObjects += DeleteRelatedObjects( propName, associationPropertyDefinition->RefAssociatedClass(), identProps );
            }
        }
        else if(propertyDefinition->GetPropertyType() == FdoPropertyType_ObjectProperty )
        {
            const FdoSmLpObjectPropertyDefinition* objectPropertyDefinition = static_cast<const FdoSmLpObjectPropertyDefinition*>(propertyDefinition);
            const FdoSmLpPropertyMappingDefinition* mappping = objectPropertyDefinition->RefMappingDefinition();
            if( mappping != NULL )
            {
                if(  mappping->GetType() ==  FdoSmLpPropertyMappingType_Concrete ||
                     mappping->GetType() ==  FdoSmLpPropertyMappingType_Single  )
                {
                    // If the mapping is single, then record of this object property is delete using
                    // the owner class since the object property columns are added to the owner class table.
                    const FdoSmLpClassDefinition* nextClass = objectPropertyDefinition->RefTargetClass();
                    deletedObjects +=  DeleteRelatedObjects( propName,nextClass , nextClass->RefIdentityProperties(),
                                        mappping->GetType() ==  FdoSmLpPropertyMappingType_Single );
                }
                else
                    throw FdoSchemaException::Create(NlsMsgGet(FDORDBMS_27, "Unsupported Property mapping type"));
             }
        }
    }

    //
    // Do the actual delete only if the follow relation only is set to false.
    if( followRelOnly )
        return deletedObjects;

    FdoPtr<FdoRdbmsFilterProcessor>flterProcessor = FdoPtr<FdoRdbmsConnection>((FdoRdbmsConnection*)GetConnection())->GetFilterProcessor();
    FdoPtr<FdoFilter> newFilter;
    if( scope != NULL )
    {
        FdoStringP propName = scope;
        propName += ".";
        propName += fkProps->RefItem(0)->GetName();
        FdoPtr<FdoFilter> NotNullFilter =  FdoUnaryLogicalOperator::Create( FdoPtr<FdoFilter>(FdoNullCondition::Create( propName )), FdoUnaryLogicalOperations_Not );

        if( this->GetFilterRef() != NULL )
            newFilter = FdoFilter::Combine( this->GetFilterRef(),FdoBinaryLogicalOperations_And, NotNullFilter );
        else
            newFilter = NotNullFilter;
    }
    else
    {
        // The scope is null is the case of deleting from the class table
        newFilter = this->GetFilter();
        if( newFilter == NULL ) // Special case of deleting all the records
        {
            FdoStringP  delAllStmt = "delete from ";
            FdoStringP  queyQualificationClause;
            delAllStmt += mConnection->GetSchemaUtil()->GetDbObjectSqlName(currentClass);
            GetLtQualification(currentClass, queyQualificationClause);
		    if( ((const wchar_t*)queyQualificationClause)[0] != '\0' )
		    {
			    delAllStmt += L" WHERE ";
                delAllStmt += (const wchar_t *)queyQualificationClause;
		    }

            count = mConnection->GetGdbiConnection()->ExecuteNonQuery( (FdoString*)delAllStmt );
            return (count+deletedObjects);
        }
    }

	// disable table aliases since some RDBMSs do not handle it with delete command
	flterProcessor->SetUseTableAlias( false );

    const wchar_t *sqlBuffer = flterProcessor->FilterToSql( newFilter, this->GetClassNameRef()->GetText(), SqlCommandType_Update, FdoCommandType_Delete, NULL, false );
    // The buffer contains a query of the form: "select classTab.id from classTab,assocTab,objTab where ...."
    int idx, size;

    size = (int)wcslen( sqlBuffer );
    if ( scope == NULL )
    {
        // Deleting from class for command, just need the where clause
        for( idx=0; idx<size && FdoCommonOSUtil::wcsnicmp(&sqlBuffer[idx],L" WHERE ",7); idx++ );
    }
    else
        // Deleting associated feature or object properties, also need from clause
        // for inner select
        for( idx=0; idx<size && FdoCommonOSUtil::wcsnicmp(&sqlBuffer[idx],L" FROM ",6); idx++ );

    if( idx == size )
        return deletedObjects;

    FdoStringP  tableName = mConnection->GetSchemaUtil()->GetDbObjectSqlName(currentClass);

    FdoStringP  delStmt = L"delete from ";

	// No object property or association
    if ( scope == NULL ) 
	{
		// MySql doesn't allow inner select to operate on same table as delete statement.
        // In this case just tack on where clause from sqlbuffer to generate:
        // "delete from tab where ..."
		delStmt += tableName;
		// Not using table aliases 
        delStmt += &sqlBuffer[idx];

		count = mConnection->GetGdbiConnection()->ExecuteNonQuery( (const wchar_t*)delStmt );
		return (count+deletedObjects);
	}

	// RDBMS support inner select; let's do the delete in a single statement
	if( mFdoConnection->SupportsInnerQuery() )
	{
        // For deleting an associated feature or object property
        // We replace the column spec (classTab.id) from sqlbuffer with assocTab.id1,assocTab.id2,... and use that select as an inner select for a delete similar to:
        // "delete from assocTab where (assocTab.id1,assocTab.id2,..) in (select assocTab.id1,assocTab.id2,.. from classTab,assocTab,objTab where ....)"
        delStmt += tableName;
		delStmt += L" where ";

		if( fkProps->GetCount() > 1 )
			delStmt += L"(";
        for( int k=0;k<fkProps->GetCount();k++)
        {
            if( k != 0 )
            {
                delStmt += L",";
            }
            delStmt += tableName;
            delStmt += L".";
            delStmt += mConnection->GetSchemaUtil()->GetColumnSqlName(fkProps->RefItem(k));
        }
		if( fkProps->GetCount() > 1 )
			delStmt += L")";
        delStmt += L" in ( select ";
        for( int k=0;k<fkProps->GetCount();k++)
        {
            if( k != 0 )
            {
                delStmt += L",";
            }
            delStmt += flterProcessor->GetTableAlias(tableName);
            delStmt += L".";
            delStmt += mConnection->GetSchemaUtil()->GetColumnSqlName(fkProps->RefItem(k));
        }

        delStmt += &sqlBuffer[idx];
        delStmt += L" )";

		count = mConnection->GetGdbiConnection()->ExecuteNonQuery( (const wchar_t*)delStmt );

		return (count+deletedObjects);
	}

	// If we get here then we are deleting object data and the RDBMS does not support inner select.
	// Let's delete one row at a time
	// First we build the delete string using bind variable
    delStmt += tableName;
	delStmt += L" where ";

	FdoStringP queryStmt = L"select ";
	for( int k=0;k<fkProps->GetCount();k++)
    {
        if( k != 0 )
        {
            queryStmt += L",";
        }
        queryStmt += flterProcessor->GetTableAlias(tableName);
        queryStmt += L".";
        queryStmt += mConnection->GetSchemaUtil()->GetColumnSqlName(fkProps->RefItem(k));
    }

    queryStmt += &sqlBuffer[idx];
	GdbiQueryResult *result = mConnection->GetGdbiConnection()->ExecuteQuery( (const wchar_t*)queryStmt );
	count = 0;
	while( result && result->ReadNext() )
	{
		FdoStringP subDelStmt = delStmt;
		for( int k=0;k<fkProps->GetCount();k++)
		{
			if( k != 0 )
			{
				subDelStmt += L" and ";
			}
			subDelStmt += tableName;
			subDelStmt += L".";
			subDelStmt += mConnection->GetSchemaUtil()->GetColumnSqlName(fkProps->RefItem(k));
			subDelStmt += L"='";
			subDelStmt += result->GetString( k+1, NULL, NULL );
			subDelStmt += L"'";
		}
		count += mConnection->GetGdbiConnection()->ExecuteNonQuery( (const wchar_t*)subDelStmt );
	}
	return (count+deletedObjects);
}

// Depending on the given parameters the function applies a transaction lock on
// the qualified objects and/or checks for version and lock conflicts.
bool FdoRdbmsDeleteCommand::CheckLocks(bool placeTransactionLock, bool checkForConflicts)
{

    bool lockConflicts = false;
    bool executionStatus = FALSE;

    // If there is already a lock conflict reader, the reader is a remainder
    // of a previous operation and can therefore be removed.
    FDO_SAFE_RELEASE(mLockConflictReader);

    // Handle all lock related issues. This includes:
    //  - Applying a transaction lock on all qualified objects.
    //  - Checking for lock conflicts .
    //  - Preparing the lock conflict reader.

    mLockConflictReader = LockUtility::HandleLocks (mConn,
                                                    this->GetClassNameRef(),
                                                    this->GetFilterRef(),
                                                    placeTransactionLock,
                                                    &lockConflicts,
                                                    &executionStatus);

    if (!executionStatus)
        throw FdoCommandException::Create(
                    NlsMsgGet(FDORDBMS_260,
                              "Unable to get exclusive access to one or more features"));

    // Return TRUE if the check did not discover lock conflicts, FALSE other-
    // wise.

    return (lockConflicts == false);

}

void FdoRdbmsDeleteCommand::GetLtQualification ( const FdoSmLpClassDefinition *classDefinition, FdoStringP &ltQualificationClause )
{
	ltQualificationClause = L"";
}

// The function returns an empty lock conflict reader.
void FdoRdbmsDeleteCommand::GetDefaultLockConflictReader()
{

    bool               executionStatus = FALSE;
    FdoRdbmsConnection *conn           = NULL;

    try
    {

        // Get a reference to the FDORDBMS connection object.
        conn = static_cast<FdoRdbmsConnection*>(GetConnection());

        // If there is already a lock conflict reader, the reader is a remainder
        // of a previous operation and can therefore be removed.
        FDO_SAFE_RELEASE(mLockConflictReader);

        // Get the default lock conflict reader.
        mLockConflictReader =
            LockUtility::GetDefaultLockConflictReader (conn,
                                                       this->GetClassNameRef(),
                                                       &executionStatus);

        // Release the reference to the FDORDBMS connection and process the
        // the result of the function handling locks.
        FDO_SAFE_RELEASE(conn);

    }

    catch ( ... )
    {
        FDO_SAFE_RELEASE(conn);
        throw;
    }

}
void FdoRdbmsDeleteCommand::SetLockConflictReader(FdoILockConflictReader *lockConflictReader)
{
	mLockConflictReader = lockConflictReader;
    mLockConflictReader->AddRef();
}

