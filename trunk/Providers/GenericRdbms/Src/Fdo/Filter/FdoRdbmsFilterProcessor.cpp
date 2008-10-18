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
#include "FdoRdbmsConnection.h"
#include "FdoRdbmsFilterProcessor.h"
#include <Sm/Lp/ObjectPropertyDefinition.h>
#include <Sm/Lp/ObjectPropertyClass.h>
#include <Sm/Lp/PropertyMappingSingle.h>
#include <Sm/Lp/AssociationPropertyDefinition.h>
#include <Geometry/GeometryStd.h>
#include <Geometry/Fgf/Factory.h>
#include "Inc/ut.h"
#include <FdoCommonOSUtil.h>


static  char*   FILTER_ERROR = "Filter processing error";


FdoRdbmsFilterProcessor::FdoRdbmsFilterProcessor():
 mSqlFilterText( NULL ),
 mSqlTextSize( 0 ),
 mFdoConnection( NULL ),
 mCurrentClassName( NULL ),
 mFirstTxtIndex( 0 ),
 mNextTxtIndex( 0 ),
 mNextTabAliasId ( 0 ),
 mUseTableAliases( true ),
 mUseNesting( true ),
 mAddNegationBracket( false )
{

}

FdoRdbmsFilterProcessor::FdoRdbmsFilterProcessor(FdoRdbmsConnection *connection):
 mSqlFilterText( NULL ),
 mSqlTextSize( 0 ),
 mFdoConnection( connection ),
 mCurrentClassName( NULL ),
 mFirstTxtIndex( 0 ),
 mNextTxtIndex( 0 ),
 mNextTabAliasId ( 0 ),
 mUseTableAliases( true ),
 mUseNesting( true ),
 mAddNegationBracket( false )
{

}

FdoRdbmsFilterProcessor::~FdoRdbmsFilterProcessor(void)
{
    if( mSqlFilterText )
        delete[] mSqlFilterText;

    if( mCurrentClassName )
        delete[] mCurrentClassName;

	mCurrentTableRelationArray.clear();
	mClassArray.clear();
	mFilterLogicalOps.clear();
}

//
// Reset the global buffer for use to process a new filter
void FdoRdbmsFilterProcessor::ResetBuffer( SqlCommandType cmdType )
{
    mCurrentCmdType = cmdType;

    if( mSqlFilterText == NULL )
        return;
    mFirstTxtIndex = mNextTxtIndex = mSqlTextSize/2;
    mSqlFilterText[mNextTxtIndex] = '\0';

    if (mSecondarySpatialFilters != NULL)
        mSecondarySpatialFilters->Clear();

   	mFilterLogicalOps.clear();

    if (mBoundGeometryValues != NULL)
        mBoundGeometryValues->Clear();
}

//
// Make sure that the buffer is large enought for an extra size chars.
void FdoRdbmsFilterProcessor::ReallocBuffer( size_t  size, bool atEnd )
{
    size_t extraSize;

    if( atEnd && (mNextTxtIndex + size) < mSqlTextSize )
        return;

    if( !atEnd && size < mFirstTxtIndex )
        return;

    extraSize = size * 2; // Need to append or prepend enough at both ends of the buffer

    if( mSqlFilterText == NULL )
    {
        mSqlTextSize = ( extraSize < MEM_BLOCK_ALLOC_SIZE )?MEM_BLOCK_ALLOC_SIZE:extraSize;
        mSqlFilterText = new wchar_t[mSqlTextSize];
        if( mSqlFilterText == NULL )
            throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_11, "Memory error"));
        mFirstTxtIndex = mNextTxtIndex = mSqlTextSize/2;
        mSqlFilterText[mNextTxtIndex] = '\0';
    }
    else
    {
        mSqlTextSize += (( extraSize < MEM_BLOCK_ALLOC_SIZE )?MEM_BLOCK_ALLOC_SIZE:extraSize);
        wchar_t  *tmp = new wchar_t[mSqlTextSize];
        if( tmp == NULL )
            throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_11, "Memory error"));
        size_t currentSize = wcslen( &mSqlFilterText[mFirstTxtIndex] );
        wcsncpy( &tmp[(mSqlTextSize-currentSize)/2], &mSqlFilterText[mFirstTxtIndex], currentSize );
        mFirstTxtIndex = (mSqlTextSize-currentSize)/2;
        mNextTxtIndex = mFirstTxtIndex + currentSize;
        tmp[mNextTxtIndex] = '\0';

        delete[] mSqlFilterText;
        mSqlFilterText = tmp;
    }
#if 0
    // Fill out string so we can see it in a debugger.
    if (mFirstTxtIndex > 0)
        for(size_t i=0;  i < mFirstTxtIndex;  i++)
            mSqlFilterText[i] = L'x';
#endif
}

//
// Add a string to the end of the buffer
void FdoRdbmsFilterProcessor::AppendString(const char *str)
{
    AppendString(mFdoConnection->GetDbiConnection()->GetUtility()->Utf8ToUnicode( str ) );
}

//
// Add a string to the biginning of the buffer
void FdoRdbmsFilterProcessor::PrependString(const char *str)
{
    PrependString(mFdoConnection->GetDbiConnection()->GetUtility()->Utf8ToUnicode( str ) );
}

//
// Add a string to the end of the buffer
void FdoRdbmsFilterProcessor::AppendString(const wchar_t *str)
{
    ReallocBuffer( wcslen( str ) + 1, true );
    wcscpy( &mSqlFilterText[mNextTxtIndex], str );
    mNextTxtIndex += wcslen( str );
}

//
// Add a string to the biginning of the buffer
void FdoRdbmsFilterProcessor::PrependString(const wchar_t *str)
{
    ReallocBuffer( wcslen( str ) + 1, false );
    mFirstTxtIndex -= wcslen( str );
    wcsncpy(&mSqlFilterText[mFirstTxtIndex], str, wcslen( str ) );
}

void FdoRdbmsFilterProcessor::AddNewClass( const FdoSmLpClassDefinition *smLpClass )
{
    for( size_t index = 0; index < mClassArray.size(); index++ )
		if( smLpClass == mClassArray[index] )
			return;

	mClassArray.push_back( smLpClass );
}

void FdoRdbmsFilterProcessor::AddNewTableRelation( const wchar_t *pkTab, const wchar_t *pkCol, const wchar_t *fkTab, const wchar_t *fkCol, bool userOuterJoin )
{
    FilterTableRelationDef  tabRelation;
    bool fkduplicate = false;
    //
    // Make sure that we did not add this relaton already
	size_t index = 1;
    for( ; index < mCurrentTableRelationArray.size(); index++ )
    {
        FilterTableRelationDef  tabRelation;
        tabRelation = mCurrentTableRelationArray[index];
        if( wcscmp( fkTab, tabRelation.fk_TableName ) == 0 )
        {
            fkduplicate = true;
            if( userOuterJoin && wcscmp( pkTab, tabRelation.pk_TableName ) == 0 )
            {
                if( wcscmp( pkCol, tabRelation.pk_ColumnName ) == 0 && wcscmp( fkCol, tabRelation.fk_ColumnName ) == 0 )
                {
                    mCurrentTableRelationArray[index].useOuterJoin = true;
                    return;
                }
            }
        }
    }

    wcsncpy( tabRelation.pk_TableName, pkTab, GDBI_TABLE_NAME_SIZE );
    tabRelation.pk_TableName[GDBI_TABLE_NAME_SIZE-1] = '\0';
    const wchar_t* tbal = GetTableAlias( tabRelation.pk_TableName );
	if( wcscmp(tbal,tabRelation.pk_TableName)==0)
	{
		tabRelation.pk_TabAlias[0] = 'A'+mNextTabAliasId;
		tabRelation.pk_TabAlias[1] = 0;
		mNextTabAliasId++;
		if( mNextTabAliasId > 26 )
			mNextTabAliasId = 0;
	}
	else
	{
		tabRelation.pk_TabAlias[0] = tbal[0];
		tabRelation.pk_TabAlias[1] = 0;
	}

    wcsncpy( tabRelation.pk_ColumnName, pkCol, GDBI_COLUMN_NAME_SIZE );
    tabRelation.pk_ColumnName[GDBI_COLUMN_NAME_SIZE-1] = '\0';

    wcsncpy( tabRelation.fk_TableName, fkTab, GDBI_TABLE_NAME_SIZE );
    tabRelation.fk_TableName[GDBI_TABLE_NAME_SIZE-1] = '\0';
	tbal = GetTableAlias( tabRelation.fk_TableName );
	if( wcscmp(tbal,tabRelation.fk_TableName)==0)
	{
		tabRelation.fk_TabAlias[0] = 'A'+mNextTabAliasId;
		tabRelation.fk_TabAlias[1] = 0;
		mNextTabAliasId++;
		if( mNextTabAliasId > 26 )
			mNextTabAliasId = 0;
	}
	else
	{
		tabRelation.fk_TabAlias[0] = tbal[0];
		tabRelation.fk_TabAlias[1] = 0;
	}

    wcsncpy( tabRelation.fk_ColumnName, fkCol, GDBI_COLUMN_NAME_SIZE );
    tabRelation.fk_ColumnName[GDBI_COLUMN_NAME_SIZE-1] = '\0';
	if( mNextTabAliasId > 13 )
		mNextTabAliasId = 0;
    tabRelation.useOuterJoin = userOuterJoin;
    tabRelation.duplicatefkTable = fkduplicate;
    if( mProcessingOrOperator )
    {
        tabRelation.useOuterJoin = true; // This is required since the actual join is added with the or condition
    }

    mCurrentTableRelationArray.push_back( tabRelation );
}

const wchar_t* FdoRdbmsFilterProcessor::GetTableAlias( const wchar_t* tabName )
{
	if( ! mUseTableAliases )
		return tabName;

    for( size_t index = 0; index < mCurrentTableRelationArray.size(); index++ )
    {
        if( wcscmp(mCurrentTableRelationArray[index].fk_TableName, tabName ) == 0 )
            return mCurrentTableRelationArray[index].fk_TabAlias;
		if( wcscmp(mCurrentTableRelationArray[index].pk_TableName, tabName ) == 0 )
			return mCurrentTableRelationArray[index].pk_TabAlias;
    }

	return tabName;
}

//
// Get the column name associated with a given class property
const wchar_t * FdoRdbmsFilterProcessor::PropertyNameToColumnName( const wchar_t *propName )
{
    const FdoSmLpClassDefinition* currentClass = NULL;
    DbiConnection  *mDbiConnection = mFdoConnection->GetDbiConnection();
    const FdoSmLpSchema* schema = mDbiConnection->GetSchema( mCurrentClassName );
    currentClass = mDbiConnection->GetSchemaUtil()->GetClass(mCurrentClassName);

    const FdoSmLpPropertyDefinition *propertyDefinition = currentClass->RefProperties()->RefItem( propName );
    if (propertyDefinition == NULL)
        throw FdoFilterException::Create(NlsMsgGet1(FDORDBMS_28, "Property '%1$ls' is not found", propName));

    switch( propertyDefinition->GetPropertyType()  )
    {
        case FdoPropertyType_DataProperty:
            {
                const FdoSmLpDataPropertyDefinition* dataProp =
                            static_cast<const FdoSmLpDataPropertyDefinition*>(propertyDefinition);
                const FdoSmPhColumn *column = dataProp->RefColumn();
                if (NULL == column)
                    throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_22, "Internal error"));
                return column->GetName();
            }
            break;

        case FdoPropertyType_ObjectProperty:
            {
                const FdoSmLpObjectPropertyDefinition* objProp =
                            static_cast<const FdoSmLpObjectPropertyDefinition*>(propertyDefinition);

                const FdoSmLpClassDefinition* pTargetClass = objProp->RefTargetClass();
                if ( pTargetClass ) {
                    const FdoSmLpDbObject* pTargetTable = pTargetClass->RefDbObject();
                    if ( pTargetTable ) {
                        const FdoSmPhColumnCollection* pkCols = pTargetTable->RefTargetColumns();
                        if( pkCols && pkCols->GetCount() != 0 )
                        {
                            if( pkCols->GetCount() != 1 )
                                throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_20, "Case not handled yet"));
                            return pkCols->RefItem(0)->GetName();
                        }
                    }
                }

                throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_22, "Internal error"));
            }
            break;

        case FdoPropertyType_GeometricProperty:
            {
                const FdoSmLpGeometricPropertyDefinition* geomProp =
                            static_cast<const FdoSmLpGeometricPropertyDefinition*>(propertyDefinition);

                // Make sure that no code that wasn't upgraded to handle ordinate column storage
                // doesn't crash by straying in here.
                FdoSmOvGeometricColumnType columnType = geomProp->GetGeometricColumnType();
                FdoSmOvGeometricContentType contentType = geomProp->GetGeometricContentType();
                if (FdoSmOvGeometricColumnType_Double == columnType &&
                    FdoSmOvGeometricContentType_Ordinates == contentType)
                {
                    throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_22, "Internal error"));
                }

                const FdoSmPhColumn *column = geomProp->RefColumn();
                if (NULL == column)
                    throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_22, "Internal error"));
                return  column->GetName();
            }
            break;

        default:
            throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_22, "Internal error"));
    }
}

void FdoRdbmsFilterProcessor::ProcessBinaryExpression(FdoBinaryExpression& expr)
{
    FdoPtr<FdoExpression>lftExpr = expr.GetLeftExpression();
    FdoPtr<FdoExpression>rgtExpr = expr.GetRightExpression();
    if( lftExpr == NULL  )
        throw FdoFilterException::Create(NlsMsgGet1(FDORDBMS_252, "%1$ls is missing the left expression", L"FdoBinaryExpression" ));
    if( rgtExpr == NULL )
        throw FdoFilterException::Create(NlsMsgGet1(FDORDBMS_253, "%1$ls is missing the right expression", L"FdoBinaryExpression" ));
    AppendString( OPEN_PARENTH );
    HandleExpr( lftExpr );
    switch( expr.GetOperation() )
    {
        case FdoBinaryOperations_Add: AppendString( ARITHMETIC_PLUS );
            break;
        case FdoBinaryOperations_Subtract: AppendString( ARITHMETIC_MINUS );
            break;

        case FdoBinaryOperations_Multiply: AppendString( ARITHMETIC_MULT );
            break;

        case FdoBinaryOperations_Divide: AppendString( ARITHMETIC_DIV );
            break;

        default:
            throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_63, FILTER_ERROR));
    }
    HandleExpr( rgtExpr );
    AppendString( CLOSE_PARENTH );
}

void FdoRdbmsFilterProcessor::ProcessUnaryExpression(FdoUnaryExpression& expr)
{
    FdoPtr<FdoExpression>uniExpr = expr.GetExpression();
    if( uniExpr == NULL )
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_256, "FdoUnaryExpression is missing the expression"));

    if( expr.GetOperation() == FdoUnaryOperations_Negate )
    {
        AppendString( L" ( - ( " );
        HandleExpr( uniExpr );
        AppendString( L" ) ) ");
    }
    else
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_257, "FdoUnaryExpression supports only the negate operation" ));
}

void FdoRdbmsFilterProcessor::ProcessFunction(FdoFunction& expr)
{
    ProcessFunctionName(expr);
    //AppendString( OPEN_PARENTH ); // MySql tripped on the leading space 
	AppendString(L"( "); 
    FdoPtr<FdoExpressionCollection> exprCol = expr.GetArguments();
    for(int i=0; i<exprCol->GetCount(); i++ )
    {
        if( i!= 0 )
            AppendString( L", " );

        FdoPtr<FdoExpression>exp = exprCol->GetItem( i );
        HandleExpr( exp );
    }
    AppendString( CLOSE_PARENTH );
}


void FdoRdbmsFilterProcessor::ProcessFunctionName(FdoFunction& expr)
{
    AppendString( expr.GetName() );
}


void FdoRdbmsFilterProcessor::ProcessComputedIdentifier(FdoComputedIdentifier& expr)
{
    FdoPtr<FdoExpression>pExpr = expr.GetExpression();
    if( pExpr == NULL )
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_256, "FdoComputedIdentifier is missing the expression"));

    AppendString( OPEN_PARENTH );
    HandleExpr( pExpr );
    AppendString( CLOSE_PARENTH );
}

void FdoRdbmsFilterProcessor::ProcessIdentifier( FdoIdentifier& expr, bool useOuterJoin )
{
    int     scopeLen;
    const FdoSmLpPropertyDefinition *propertyDefinition = NULL;
    const FdoSmLpObjectPropertyDefinition* objProp = NULL;
    const FdoSmLpAssociationPropertyDefinition* assocProp = NULL;
    const FdoSmLpClassDefinition* currentClass = NULL;

    DbiConnection  *mDbiConnection = mFdoConnection->GetDbiConnection();
    const FdoSmLpSchema* schema = mDbiConnection->GetSchema( mCurrentClassName );
    currentClass = mDbiConnection->GetSchemaUtil()->GetClass(mCurrentClassName);

    // Need to get the scope(s) and matched with table name from the schema manager
    const wchar_t** scopes = expr.GetScope( scopeLen );

    if( scopes != NULL )
    {
        for( int i = 0; i<scopeLen; i++ )
        {
            const wchar_t* propName = scopes[i];
            propertyDefinition = currentClass->RefProperties()->RefItem( propName );
            if( (propertyDefinition == NULL) )
                throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_24, "Expected object or association property"));

            switch( propertyDefinition->GetPropertyType() )
            {
                case FdoPropertyType_ObjectProperty:
                    {
                    objProp = static_cast<const FdoSmLpObjectPropertyDefinition*>(propertyDefinition);

                    // If one of the object properties is of collection type, then we may need to add a distinct key word to the select.
                    // This is required to avoid returning multiple copies of the same feature when the filter contains a condition using
                    // one or more properties of the collection.

//                    if( objProp->GetObjectType() == FdoObjectType_OrderedCollection || objProp->GetObjectType() == FdoObjectType_Collection )
//                        mRequiresDistinct = true;

                    FdoStringP pkTable = mDbiConnection->GetSchemaUtil()->GetDbObjectSqlName(currentClass);
                    FdoStringP fkTable = mDbiConnection->GetSchemaUtil()->GetDbObjectSqlName(objProp);
                    const FdoSmPhColumnCollection* pkCols = NULL;
                    const FdoSmPhColumnCollection* fkCols = NULL;
                    currentClass = objProp->RefTargetClass();
                    if ( currentClass ) {
                        const FdoSmLpDbObject* currentTable = currentClass->RefDbObject();
                        if ( currentTable ) {
                            pkCols = currentTable->RefTargetColumns();
                            fkCols = currentTable->RefSourceColumns();
                        }
                    }

                    if ( !pkCols || !fkCols )
                        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_25, "Primary key or foreign columns missing"));

                    if( pkCols && fkCols && pkCols->GetCount() != fkCols->GetCount() )
                        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_26, "Primary key and foreign column count should be the same"));

                    const FdoSmLpPropertyMappingDefinition* mappping = objProp->RefMappingDefinition();
                    switch( mappping->GetType() )
                    {
                        case FdoSmLpPropertyMappingType_Concrete:
                            for( int j=0; j<pkCols->GetCount(); j++ )
                            {
                                AddNewTableRelation(
                                            pkTable, pkCols->RefItem(j)->GetDbName(),
                                            fkTable, fkCols->RefItem(j)->GetDbName(), useOuterJoin
                                            );
                            }
							AddNewClass( currentClass );
                            break;
                        case FdoSmLpPropertyMappingType_Single:
                            break;

                        default:
                            throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_27, "Unsupported Property mapping type"));
                    }
                    }
                    break;

                case FdoPropertyType_AssociationProperty:
                    {
                    assocProp = static_cast<const FdoSmLpAssociationPropertyDefinition*>(propertyDefinition);


                    FdoStringP pkTable = mDbiConnection->GetSchemaUtil()->GetDbObjectSqlName(currentClass);
                    FdoStringP fkTable = mDbiConnection->GetSchemaUtil()->GetDbObjectSqlName(assocProp->RefAssociatedClass());
                    const FdoSmPhColumnListP revIdentCols = assocProp->GetReverseIdentityColumns();
                    const FdoSmPhColumnListP identCols = assocProp->GetIdentityColumns();
                    for( int i=0; i<identCols->GetCount(); i++ )
                    {
                        AddNewTableRelation(
                             pkTable, revIdentCols->GetDbString(i),
                             fkTable, identCols->GetDbString(i)
                        );
                    }
                    currentClass = assocProp->RefAssociatedClass();
					AddNewClass( currentClass );
                    }
                    break;

                default:
                    throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_24, "Expected object or association property"));
            }
        }
    }

    // Build the identifier with only the last table in the scope
    propertyDefinition = currentClass->RefProperties()->RefItem( expr.GetName() );
    if( propertyDefinition == NULL )
        throw FdoFilterException::Create(NlsMsgGet1(FDORDBMS_28, "Property '%1$ls' is not found", expr.GetName() ));

    switch( propertyDefinition->GetPropertyType()  )
    {
        case FdoPropertyType_DataProperty:
            {
                const FdoSmLpDataPropertyDefinition* dataProp =
                            static_cast<const FdoSmLpDataPropertyDefinition*>(propertyDefinition);
                FdoStringP tableName = mDbiConnection->GetSchemaUtil()->GetDbObjectSqlName(currentClass);
                const FdoSmPhColumn *column = dataProp->RefColumn();
                if (NULL == column)
                    throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_22, "Internal error"));
                AppendString(  GetTableAlias( tableName ) );
                AppendString( L"." );
                AppendString( (FdoString*)(column->GetDbName()) );
            }
            break;

        case FdoPropertyType_ObjectProperty:
            {
                const FdoSmLpObjectPropertyDefinition* objProp =
                            static_cast<const FdoSmLpObjectPropertyDefinition*>(propertyDefinition);
                const FdoSmLpClassDefinition* pTargetClass = objProp->RefTargetClass();
                if ( !pTargetClass )
                    throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_22, "Internal error"));

                const FdoSmLpDbObject* pTargetTable = pTargetClass->RefDbObject();
                if ( !pTargetTable )
                    throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_22, "Internal error"));

                const FdoSmPhColumnCollection* pkCols = pTargetTable->RefTargetColumns();

                if( !pkCols || pkCols->GetCount() == 0 )
                    throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_22, "Internal error"));

                if( pkCols->GetCount() != 1 )
                    throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_20, "Case not handled yet"));

                FdoStringP sqlTableName = mDbiConnection->GetSchemaUtil()->GetDbObjectSqlName(currentClass);
                AppendString( GetTableAlias( sqlTableName ) );
                AppendString( L"." );
                AppendString( pkCols->RefItem(0)->GetName() );

            }
            break;

        case FdoPropertyType_GeometricProperty:
            {
                const FdoSmLpGeometricPropertyDefinition* geomProp =
                            static_cast<const FdoSmLpGeometricPropertyDefinition*>(propertyDefinition);
                FdoSmOvGeometricColumnType columnType = geomProp->GetGeometricColumnType();
                FdoSmOvGeometricContentType contentType = geomProp->GetGeometricContentType();
                if (FdoSmOvGeometricColumnType_Double == columnType &&
                    FdoSmOvGeometricContentType_Ordinates == contentType)
                {
                    FdoStringP sqlTableName = mDbiConnection->GetSchemaUtil()->GetDbObjectSqlName(currentClass);
                    FdoString * tableAlias = GetTableAlias( sqlTableName );
                    const FdoSmPhColumn *columnX = geomProp->RefColumnX();
                    const FdoSmPhColumn *columnY = geomProp->RefColumnY();
                    const FdoSmPhColumn *columnZ = geomProp->RefColumnZ();
                    if (NULL == columnX || NULL == columnY)
                        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_22, "Internal error"));
                    AppendString( tableAlias );
                    AppendString( L"." );
                    AppendString( (FdoString*)(columnX->GetDbName()) );
                    AppendString( L"," );
                    AppendString( tableAlias );
                    AppendString( L"." );
                    AppendString( (FdoString*)(columnY->GetDbName()) );
                    if (NULL != columnZ)
                    {
                        AppendString( L"," );
                        AppendString( tableAlias );
                        AppendString( L"." );
                        AppendString( (FdoString*)(columnZ->GetDbName()) );
                    }
                }
                else // Single-column storage
                {
                    const FdoSmPhColumn *column = geomProp->RefColumn();
                    if (NULL == column)
                        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_22, "Internal error"));
                    FdoStringP sqlTableName = mDbiConnection->GetSchemaUtil()->GetDbObjectSqlName(currentClass);
                    FdoString * tableAlias = GetTableAlias( sqlTableName );
                    AppendString( tableAlias );
                    AppendString( L"." );
                    
                    FdoStringP  colName = GetGeometryString( (FdoString*)(column->GetDbName()) );
                    AppendString( (FdoString*)colName );
                }
            }
            break;

        case FdoPropertyType_AssociationProperty:
            throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_20, "Case not handled yet"));

        default:
            throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_22, "Internal error"));
    }
}

void FdoRdbmsFilterProcessor::ProcessParameter( FdoParameter& expr )
{
    AppendString( L":" );
    AppendString( PropertyNameToColumnName( expr.GetName() ) );
}

void FdoRdbmsFilterProcessor::ProcessBooleanValue(FdoBooleanValue& expr)
{
    if( expr.GetBoolean() )
        AppendString( L"1" );  // Database bool true
    else
        AppendString( L"0" );  // Database bool false
}

void FdoRdbmsFilterProcessor::ProcessByteValue(FdoByteValue& expr)
{
    wchar_t   val[2];
    val[0] = expr.GetByte();
    val[1] = '\0';
    AppendString( L"'" );
    AppendString( val );
    AppendString( L"'" );
}

void FdoRdbmsFilterProcessor::ProcessDateTimeValue(FdoDateTimeValue& expr)
{
    AppendString( L"'" );
    AppendString( mFdoConnection->FdoToDbiTime( expr.GetDateTime() ) );
    AppendString( L"'" );
}

void FdoRdbmsFilterProcessor::ProcessDoubleValue(FdoDoubleValue& expr)
{
    char    tmpValue[124];
    sprintf(tmpValue,"%.16g",expr.GetDouble());
    AppendString( tmpValue );
}

void FdoRdbmsFilterProcessor::ProcessDecimalValue(FdoDecimalValue& expr)
{
    char    tmpValue[124];
    sprintf(tmpValue,"%.8f",expr.GetDecimal());
    AppendString( tmpValue );
}

void FdoRdbmsFilterProcessor::ProcessInt16Value(FdoInt16Value& expr)
{
    char    tmpValue[124];
    AppendString( FdoCommonOSUtil::itoa( (int)expr.GetInt16(),tmpValue) );
}

void FdoRdbmsFilterProcessor::ProcessInt32Value(FdoInt32Value& expr)
{
    char    tmpValue[124];
    AppendString( FdoCommonOSUtil::itoa( (int)expr.GetInt32(),tmpValue) );
}

void FdoRdbmsFilterProcessor::ProcessInt64Value(FdoInt64Value& expr)
{
    char    tmpValue[124];

#ifdef _WIN32
	strncpy( tmpValue, _i64toa( (FdoInt64)(dynamic_cast<FdoInt64Value&>(expr)).GetInt64(),tmpValue, 10 ), 123 );
	tmpValue[123]='\0';
#else

	sprintf(tmpValue, "%lld", (FdoInt64)(dynamic_cast<FdoInt64Value&>(expr)).GetInt64());
#endif
    AppendString(tmpValue);

}

void FdoRdbmsFilterProcessor::ProcessSingleValue(FdoSingleValue& expr)
{
    char    tmpValue[124];
    sprintf(tmpValue,"%.8f",expr.GetSingle());
    AppendString( tmpValue );
}

void FdoRdbmsFilterProcessor::ProcessStringValue(FdoStringValue& expr)
{
    AppendString( expr.ToString() );
}

void FdoRdbmsFilterProcessor::ProcessBLOBValue(FdoBLOBValue& expr)
{
    throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_29, "Unsupported FDO type in expression"));
}

void FdoRdbmsFilterProcessor::ProcessCLOBValue(FdoCLOBValue& expr)
{
    throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_29, "Unsupported FDO type in expression"));
}

void FdoRdbmsFilterProcessor::ProcessGeometryValue(FdoGeometryValue& expr)
{
    throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_29, "Unsupported FDO type in expression"));
}

///////////////////////////////////////////////////////////////
//Filters implementation handler                             //
///////////////////////////////////////////////////////////////
void FdoRdbmsFilterProcessor::ProcessBinaryLogicalOperator(FdoBinaryLogicalOperator& filter)
{
    FdoPtr<FdoFilter>leftOperand = filter.GetLeftOperand();
    FdoPtr<FdoFilter>rightOperand = filter.GetRightOperand();
    if( leftOperand == NULL  )
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_254, "FdoBinaryLogicalOperator is missing the left operand" ));
    if( rightOperand == NULL )
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_255, "FdoBinaryLogicalOperator is missing the right operand" ));

    DbiConnection  *mDbiConnection = mFdoConnection->GetDbiConnection();
    const FdoSmLpClassDefinition *classDefinition = mDbiConnection->GetSchemaUtil()->GetClass(mCurrentClassName);
    const FdoSmLpDataPropertyDefinitionCollection *properties = classDefinition->RefIdentityProperties();

    if (mUseNesting)
        AppendString(OPEN_PARENTH);
    if( filter.GetOperation() == FdoBinaryLogicalOperations_And )
    {
        HandleFilter( leftOperand );
        AppendString( LOGICAL_AND );
        HandleFilter( rightOperand );
    }
    else
    {
        mProcessingOrOperator = true;
        HandleFilter( leftOperand );
        AppendString( LOGICAL_OR );
        HandleFilter( rightOperand );
    }

    if (mUseNesting)
        AppendString(CLOSE_PARENTH);

  	// Save 
	mFilterLogicalOps.push_back( filter.GetOperation() );
}

void FdoRdbmsFilterProcessor::ProcessComparisonCondition(FdoComparisonCondition& filter)
{
    FdoPtr<FdoExpression>lfExp = filter.GetLeftExpression();
    FdoPtr<FdoExpression>rtExp = filter.GetRightExpression();
    if( lfExp == NULL  )
        throw FdoFilterException::Create(NlsMsgGet1(FDORDBMS_252, "%1$ls is missing the left expression", L"FdoComparisonCondition" ));
    if( rtExp == NULL )
        throw FdoFilterException::Create(NlsMsgGet1(FDORDBMS_253, "%1$ls is missing the right expression", L"FdoComparisonCondition" ));

    AppendString( OPEN_PARENTH );
    HandleExpr( lfExp );

    switch ( filter.GetOperation() )
    {
        case FdoComparisonOperations_EqualTo: AppendString( EQUAL_OP );
            break;

        case FdoComparisonOperations_NotEqualTo: AppendString( NOT_EQUAL_OP );
            break;

        case FdoComparisonOperations_GreaterThan: AppendString( GREATER_THAN_OP );
            break;

        case FdoComparisonOperations_GreaterThanOrEqualTo: AppendString( GREATER_OR_EQUAL_OP );
            break;

        case FdoComparisonOperations_LessThan: AppendString( LESS_THAN_OP );
            break;

        case FdoComparisonOperations_LessThanOrEqualTo: AppendString( LESS_OR_EQUAL_OP );
            break;

        case FdoComparisonOperations_Like: AppendString( LIKE_OP );
            break;
        default:
            throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_63, FILTER_ERROR));
            break;
    }
    HandleExpr( rtExp );
    AppendString( CLOSE_PARENTH );
}

void FdoRdbmsFilterProcessor::ProcessInCondition(FdoInCondition& filter)
{
    int i;
    FdoPtr<FdoExpression>exp;
    FdoPtr<FdoIdentifier>id = filter.GetPropertyName();
    if( id == NULL )
        throw FdoFilterException::Create(NlsMsgGet1(FDORDBMS_248, "%1$ls is missing the property name", L"FdoInCondition"));

    FdoPtr<FdoValueExpressionCollection>expressions = filter.GetValues();
    if( expressions == NULL || expressions->GetCount() <= 0 )
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_249, "FdoInCondition has an empty value list"));

    AppendString(OPEN_PARENTH);
    ProcessIdentifier( *id );
    AppendString( L" IN " );
    AppendString(OPEN_PARENTH);
    for(i=0; i < expressions->GetCount()-1; i++ )
    {
        exp = expressions->GetItem(i);
        HandleExpr( exp );
        AppendString( L"," );
    }
    exp = expressions->GetItem( i );
    HandleExpr(exp);
    AppendString(CLOSE_PARENTH);
    AppendString(CLOSE_PARENTH);
}

void FdoRdbmsFilterProcessor::ProcessNullCondition(FdoNullCondition& filter)
{
    FdoPtr<FdoIdentifier>id = filter.GetPropertyName();
    if( id == NULL )
        throw FdoFilterException::Create(NlsMsgGet1(FDORDBMS_248, "%1$ls is missing the property name", L"FdoNullCondition"));

    AppendString(OPEN_PARENTH);
    ProcessIdentifier( *id );
    AppendString( L" IS NULL ");
    AppendString(CLOSE_PARENTH);
}

void FdoRdbmsFilterProcessor::ProcessUnaryLogicalOperator(FdoUnaryLogicalOperator& filter)
{
    FdoPtr<FdoFilter>unaryOp = filter.GetOperand();
    if( unaryOp == NULL )
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_250, "FdoUnaryLogicalOperator is missing the operand" ));

    AppendString(OPEN_PARENTH);
    if ( filter.GetOperation() == FdoUnaryLogicalOperations_Not )
    {
        AppendString( LOGICAL_NOT );
    }
    else
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_251, "FdoUnaryLogicalOperator supports only the 'Not' operation"));

    if (mAddNegationBracket)
        AppendString(OPEN_PARENTH);
    HandleFilter( unaryOp );
    if (mAddNegationBracket)
        AppendString(CLOSE_PARENTH);
    AppendString(CLOSE_PARENTH);

	// Save 
	mFilterLogicalOps.push_back( filter.GetOperation() );
}


bool FdoRdbmsFilterProcessor::CanOptimizeRelationQuery( const FdoSmLpClassDefinition* pClass, const FdoSmLpPropertyDefinition* propertyDefinition )
{
    if( propertyDefinition->GetPropertyType() == FdoPropertyType_AssociationProperty )
    {
        const FdoSmLpAssociationPropertyDefinition* assocProp = (const FdoSmLpAssociationPropertyDefinition*) propertyDefinition;

        // If this is not a read-only association and if reverse multiplicity is set to m, then multiple raws can be returned and as a result a dedicated secondary query is issued
        // Also if it's a feature class, then we need to perform a separate query for this class( which is done in the feature reader )
        if( assocProp->GetReadOnly() || wcscmp(assocProp->GetReverseMultiplicity(), L"m" ) == 0 || assocProp->RefAssociatedClass()->GetClassType() == FdoClassType_FeatureClass )
            return false;

        //
        // If one or more association to the same class exist, then it's not possible to optimize this query since more than one row need to
        // be returned for the associated class; One row for each association.
        const FdoSmLpPropertyDefinitionCollection* properties = pClass->RefProperties();
        for(int i=0; i<properties->GetCount(); i++ )
        {
            const FdoSmLpPropertyDefinition* prop = properties->RefItem(i);
            if( prop->GetPropertyType() == FdoPropertyType_AssociationProperty && prop !=  propertyDefinition )
            {
                const FdoSmLpAssociationPropertyDefinition* newAssProp = (const FdoSmLpAssociationPropertyDefinition*) prop;
                if( assocProp->RefAssociatedClass() == newAssProp->RefAssociatedClass() )
                    return false;
            }
        }
    }
    return true;
}

// This method is used to follow a value type object property or an m:1 association
// and add the necessary column spec and table mappings for joining them later.
void FdoRdbmsFilterProcessor::FollowRelation( FdoStringP    &relationColumns, const FdoSmLpPropertyDefinition* propertyDefinition, FdoIdentifierCollection *selectedProperties )
{
    DbiConnection  *mDbiConnection = mFdoConnection->GetDbiConnection();
    if( propertyDefinition->GetPropertyType() == FdoPropertyType_AssociationProperty )
    {
        const FdoSmLpAssociationPropertyDefinition* assocProp = (const FdoSmLpAssociationPropertyDefinition*) propertyDefinition;

        const FdoSmLpClassDefinition*     pClass = assocProp->RefParentClass();
		AddNewClass( pClass );
        for( int j=0; j<assocProp->GetIdentityColumns()->GetCount(); j++ )
        {
           FdoStringP pkTableName = mDbiConnection->GetSchemaUtil()->GetDbObjectSqlName(pClass);
           FdoStringP fkTableName = mDbiConnection->GetSchemaUtil()->GetDbObjectSqlName(assocProp->RefAssociatedClass());
           AddNewTableRelation(
                (const wchar_t*)pkTableName,
                (const wchar_t*)assocProp->GetReverseIdentityColumns()->GetDbString(j),
                (const wchar_t*)fkTableName,
                (const wchar_t*)assocProp->GetIdentityColumns()->GetDbString(j), true );

           FdoStringP fkAliasName = mUseTableAliases ? FdoStringP(GetTableAlias(fkTableName)) : fkTableName;

            if( j == 0 )
            {
                bool skip = false;
                // Make sure we didn't already add this entry
                if( relationColumns.GetLength() != 0 )
                {
                    const wchar_t* allTabs = (const wchar_t*)relationColumns;
                    const wchar_t* newTab = (const wchar_t*)fkAliasName;
                    for(int i=0; allTabs[i]!='\0';i++)
                    {
                        if( i==0 || allTabs[i-1]==',' )
                        {
                            if( wcsncmp(&allTabs[i],newTab, wcslen( newTab ) ) == 0 )
                            {
                                skip = true;
                                break;
                            }
                        }
                    }

                }
                if( !skip )
                {
                    relationColumns += fkAliasName;
                    relationColumns += L".*,";
                }
            }
        }
        // Visit the next level of association
        const FdoSmLpPropertyDefinitionCollection* properties = assocProp->RefAssociatedClass()->RefProperties();
        for(int i=0; i<properties->GetCount(); i++ )
        {
            if( properties->RefItem(i)->GetPropertyType() == FdoPropertyType_AssociationProperty )
            {
                if( ! CanOptimizeRelationQuery( assocProp->RefAssociatedClass(), properties->RefItem(i) ) )
                    return;
                FollowRelation( relationColumns, properties->RefItem(i), selectedProperties);
            }
        }
    }
    else if ( propertyDefinition->GetPropertyType() == FdoPropertyType_ObjectProperty )
    {
        const FdoSmLpObjectPropertyDefinition* objProp = (const FdoSmLpObjectPropertyDefinition*) propertyDefinition;
        if( objProp->GetObjectType() != FdoObjectType_Value )
            return;
    }
}

// Add the order by clause if it's required
void FdoRdbmsFilterProcessor::AppendOrderBy( FdoRdbmsFilterUtilConstrainDef *filterConstraint )
{
    if( filterConstraint == NULL || filterConstraint->orderByProperties == NULL || filterConstraint->orderByProperties->GetCount() == 0 )
        return;

    AppendString( L" ORDER BY " );
    for(int i=0; i<filterConstraint->orderByProperties->GetCount(); i++ )
    {
        if( i != 0 )
            AppendString( L", " );
        FdoPtr<FdoIdentifier>ident = filterConstraint->orderByProperties->GetItem( i );
        ProcessIdentifier( *ident, true );
        if( filterConstraint->orderingOption == FdoOrderingOption_Descending )
            AppendString( L" DESC " );
        else
            AppendString( L" ASC " );
    }
}

// Add the group by clause if it's required
void FdoRdbmsFilterProcessor::AppendGroupBy( FdoRdbmsFilterUtilConstrainDef *filterConstraint )
{
    if( filterConstraint == NULL || filterConstraint->groupByProperties == NULL || filterConstraint->groupByProperties->GetCount() == 0  )
        return;

    AppendString( L" GROUP BY " );
    for(int i=0; i<filterConstraint->groupByProperties->GetCount(); i++ )
    {
        if( i != 0 )
            AppendString( L", " );
        FdoPtr<FdoIdentifier>ident = filterConstraint->groupByProperties->GetItem( i );
        ProcessIdentifier( *ident, true );
    }
}

// Analyzes the filter and set flags that control the generation of the
// corresponding SQL statement.
void FdoRdbmsFilterProcessor::AnalyzeFilter (FdoFilter *filter)
{

    // The following defines the filter analyzer. The filter analyzer is used
    // to scan the filter for its content and set flags that control the
    // process of converting the filter into the corresponding SQL statement.
    // For example, it checks whether or not nesting of filter elememts is
    // required. 

    class FilterAnalyzer : public FdoRdbmsBaseFilterProcessor
    {

    public:

        //  containsBinaryLogicalOperatorAnd:
        //      The flag is set to TRUE if the filter contains the binary
        //      logical operator AND.
        bool containsBinaryLogicalOperatorAnd;

        //  containsBinaryLogicalOperatorOr:
        //      The flag is set to TRUE if the filter contains the binary
        //      logical operator OR.
		bool containsBinaryLogicalOperatorOr;

        //  containsUnaryLogicalOperatorNot:
        //      The flag is set to TRUE if the filter contains the unary
        //      logical operator NOT.
        bool containsUnaryLogicalOperatorNot;

        // Constructor.
        FilterAnalyzer() 
        { 
            containsBinaryLogicalOperatorAnd = false;
			containsBinaryLogicalOperatorOr  = false;
            containsUnaryLogicalOperatorNot  = false;
        }  

        // Processes a binary logical operator node. Depending on the used
        // operator, it sets the corresponding flag and then continues
        // analyzing the tree.
        virtual void ProcessBinaryLogicalOperator(
                                            FdoBinaryLogicalOperator& filter)
        {
            FdoBinaryLogicalOperations binaryLogicalOperator;
            binaryLogicalOperator = filter.GetOperation();

            if (binaryLogicalOperator == FdoBinaryLogicalOperations_And)
                containsBinaryLogicalOperatorAnd = true;
            if (binaryLogicalOperator == FdoBinaryLogicalOperations_Or)
                containsBinaryLogicalOperatorOr = true;

            if (filter.GetLeftOperand() != NULL)
                filter.GetLeftOperand()->Process(this);
            if (filter.GetRightOperand() != NULL)
                filter.GetRightOperand()->Process(this);
        }

        virtual void ProcessUnaryLogicalOperator(
                                            FdoUnaryLogicalOperator& filter)
        {
            containsUnaryLogicalOperatorNot = true;
            if (filter.GetOperand() != NULL)
                filter.GetOperand()->Process(this);
        }
    };

    // Initialize the member variables that are set by this routine. The default
    // value should reflect the current behavior.
    mUseNesting         = true;
    mAddNegationBracket = false;

    // Analyze the filter.
    FilterAnalyzer filterAnalyzer;
    filter->Process(&filterAnalyzer);

    // Check the result of the analyzing process and set the corresponding
    // member variables that control the generation of the SQL statement
    // from the given filter.
    if ((filterAnalyzer.containsBinaryLogicalOperatorAnd) ||
        (filterAnalyzer.containsBinaryLogicalOperatorOr)     )
    {
        mUseNesting = filterAnalyzer.containsBinaryLogicalOperatorAnd &&
                      filterAnalyzer.containsBinaryLogicalOperatorOr;
        mAddNegationBracket =
                        !mUseNesting &&
                        filterAnalyzer.containsUnaryLogicalOperatorNot;
    }
}

bool FdoRdbmsFilterProcessor::ContainsAggregateFunctions( FdoIdentifierCollection *identifiers )
{
    class FindAggregate : public FdoRdbmsBaseFilterProcessor
    {
    public:
        bool                            foundAggregate;
        const FdoRdbmsFilterProcessor*  mParentFilterProcessor;

        FindAggregate(const FdoRdbmsFilterProcessor* parentFilterProcessor)
        {
            mParentFilterProcessor = parentFilterProcessor;
            foundAggregate = false;
        }
        virtual void ProcessFunction(FdoFunction& expr)
        {
            if( foundAggregate )
                return;
            if (mParentFilterProcessor->IsAggregateFunctionName(expr.GetName()))
                foundAggregate = true;

            if( ! foundAggregate )
			{
				FdoPtr<FdoExpressionCollection>expCol =  expr.GetArguments();
				if( expCol != NULL )
				{
					for(int j=0;j<expCol->GetCount() && ! foundAggregate; j++ )
					{
						FdoPtr<FdoExpression>exp = expCol->GetItem(j);
						exp->Process( this );
					}
				}
			}
        }
    };

    if( identifiers == NULL )
        return false;

    FindAggregate  finder(this);
    for( int i=0; i<identifiers->GetCount(); i++ )
    {
        FdoPtr<FdoIdentifier>property = identifiers->GetItem(i);
        property->Process( &finder );
        if( finder.foundAggregate )
            return true;
    }

    return false;
}

void FdoRdbmsFilterProcessor::PrependTables()
{
    // Prepend the from clause as: table1,table2,table3 And make sure a given table will only appear once
    for( size_t index = 0; index < mCurrentTableRelationArray.size(); index++ )
    {
        FilterTableRelationDef  tabRelation;
        tabRelation = mCurrentTableRelationArray[index];
        size_t i;
        for( i = 0; i<index; i++ )
        {
            if( wcscmp(mCurrentTableRelationArray[i].fk_TableName, tabRelation.fk_TableName ) == 0 )
                break;
        }
        if( i == index )
        { // We didn't add this table name
            if( index != 0 )
                PrependString ( L"," );
			if( mUseTableAliases )
			{
				PrependString ( tabRelation.fk_TabAlias);
				PrependString ( L" " );
			}
            PrependString ( tabRelation.fk_TableName );
        }
    }
}

void FdoRdbmsFilterProcessor::PrependProperty( FdoIdentifier* property, bool scanForTableOnly )
{
    // If it's a computed identifier, then we dump the translated content in the from clause.
    // There may be alot of weird and wonderfull stuff in that expression that does not make sense. We'll leave it
    // to Oracle to tell us that
    // 1) We first save the sql buffer pointer and replace it with a new clean buffer.
    // 2) use processExpression on the identifier expression, that should any tables that we need to select from
    // 3) use the new buffer content to prepend to the existing sqlbuffer
    size_t  oldNextTxtIndex = mNextTxtIndex;
    size_t  oldSqlTextSize = mSqlTextSize;
    size_t  oldFirstTxtIndex = mFirstTxtIndex;
    wchar_t* oldSqlFilterText = mSqlFilterText;
    DbiConnection  *mDbiConnection = mFdoConnection->GetDbiConnection();

    mNextTxtIndex = mSqlTextSize = mFirstTxtIndex = 0;
    mSqlFilterText = NULL;
    if( dynamic_cast<FdoComputedIdentifier *>( property ) != NULL )
    {
        ProcessComputedIdentifier( *((FdoComputedIdentifier*)property) );
    }
    else
    {
        ProcessIdentifier( *property );
    }
    wchar_t* compIdentPseudoCol = &mSqlFilterText[mFirstTxtIndex];
    wchar_t* tmp = mSqlFilterText;

    // Reset the sql buffer
    mNextTxtIndex = oldNextTxtIndex;
    mSqlTextSize = oldSqlTextSize;
    mFirstTxtIndex = oldFirstTxtIndex;
    mSqlFilterText = oldSqlFilterText;

    // This utility can be called to scan the identifier to get the necessary table relation only.
    // In that case we don't want to add the column result yet. This method will be called again
    // to prepend those columns.
    if( ! scanForTableOnly )
    {
        if( dynamic_cast<FdoComputedIdentifier *>( property ) != NULL )
        {
            // Add the pseudo column for the computed identifier expression.
            PrependString( mDbiConnection->GetSchemaUtil()->MakeDBValidName(property->GetName()) );
            PrependString( L" AS " );
        }
        PrependString( compIdentPseudoCol );
    }

    // Free the temporary buffer
    if( tmp )
        delete[] tmp;
}


//
// Single mapped object property use the owner class idenitity properties.
const FdoSmLpDataPropertyDefinitionCollection *FdoRdbmsFilterProcessor::GetIdentityProperties(const wchar_t *className, const FdoSmLpClassDefinition **identClass )
{
    DbiConnection  *mDbiConnection = mFdoConnection->GetDbiConnection();
    const FdoSmLpClassDefinition *classDefinition = mDbiConnection->GetSchemaUtil()->GetClass(className);
    const FdoSmLpDataPropertyDefinitionCollection *identProperties = classDefinition->RefIdentityProperties();
    *identClass = classDefinition;

    if( identProperties->GetCount() == 0 )
    {
        // This may be a result of a single mapped object property
        FdoPtr<FdoIdentifier>classIdent = FdoIdentifier::Create(className);
        int   count;
        const wchar_t **scopes = classIdent->GetScope( count );
        if ( count == 0 )
            return identProperties;

        const FdoSmLpClassDefinition *currentClass = mDbiConnection->GetSchemaUtil()->GetClass(scopes[0]);
        identProperties = currentClass->RefIdentityProperties();
        *identClass = currentClass;
        for (int j=1; j<=count && currentClass; j++)
        {
            const wchar_t* objPropName;
            if( j == count )
                objPropName = classIdent->GetName();
            else
                objPropName = scopes[j];
            const FdoSmLpPropertyDefinition *propertyDefinition = currentClass->RefProperties()->RefItem( objPropName );
            if (!propertyDefinition)
                throw FdoSchemaException::Create(NlsMsgGet1(FDORDBMS_231, "Object property '%1$ls' not found", objPropName));
            if( propertyDefinition->GetPropertyType() != FdoPropertyType_ObjectProperty )
                throw FdoSchemaException::Create(NlsMsgGet(FDORDBMS_24, "Expected object property"));

            const FdoSmLpObjectPropertyDefinition* objProp = static_cast<const FdoSmLpObjectPropertyDefinition*>(propertyDefinition);
            currentClass = objProp->RefTargetClass();
            const FdoSmLpPropertyMappingDefinition* mappping = objProp->RefMappingDefinition();
            switch ( mappping->GetType() )
            {
                case FdoSmLpPropertyMappingType_Concrete:
                    identProperties = currentClass->RefIdentityProperties();
                    *identClass = currentClass;
                    break;
                case FdoSmLpPropertyMappingType_Single:
                    break;
                default:
                    throw FdoSchemaException::Create(NlsMsgGet(FDORDBMS_27, "Unsupported Property mapping type"));
            }

        }
    }
    return identProperties;
}

void FdoRdbmsFilterProcessor::GetLtTableExpression( const FdoSmLpClassDefinition *classDefinition, FdoStringP &ltJoin, FdoStringP &ltTableExp, FdoCommandType callerFdoCommand )
{
	ltJoin = L"";
	ltTableExp = L"";
}

void FdoRdbmsFilterProcessor::GetLtQualificationClause( const FdoSmLpClassDefinition *classDefinition, FdoStringP &ltQualificationClause )
{
	ltQualificationClause = L"";
}

//
// The function checks if the filter contains expression functions that have
// native support only.
bool FdoRdbmsFilterProcessor::IsValidExpression( FdoFilter *filter )
{
    class UsesNativeExpressionFunctions : public FdoRdbmsBaseFilterProcessor
    {
    public:
        bool                            foundNotNativeSupportedFunction;
        const FdoRdbmsFilterProcessor*  mParentFilterProcessor;

        UsesNativeExpressionFunctions(const FdoRdbmsFilterProcessor* parentFilterProcessor)
        {
            mParentFilterProcessor = parentFilterProcessor;
            foundNotNativeSupportedFunction = false;
        }
        virtual void ProcessFunction(FdoFunction& expr)
        {
            if( foundNotNativeSupportedFunction )
                return;
            if (mParentFilterProcessor->IsNotNativeSupportedFunction(expr.GetName()))
                foundNotNativeSupportedFunction = true;
            // Next check whether or not the argument list requires a redirection
            // to the Expression Engine.
            if ( !mParentFilterProcessor->HasNativeSupportedFunctionArguments(expr) )
                foundNotNativeSupportedFunction = true;

            if( !foundNotNativeSupportedFunction )
            {
                FdoPtr<FdoExpressionCollection>expCol =  expr.GetArguments();
                if( expCol != NULL )
                {
                    for(int j=0;j<expCol->GetCount() && !foundNotNativeSupportedFunction; j++ )
                    {
                        FdoPtr<FdoExpression>exp = expCol->GetItem(j);
                        exp->Process( this );
                    }
                }
            }
        }
    };

    if( filter == NULL )
        return true;

    UsesNativeExpressionFunctions finder(this);
    filter->Process(&finder);
    if (finder.foundNotNativeSupportedFunction)
        return false;
    else 
        return true;
}

//
// The function checks if the properties contain expression functions that have
// native support.
bool FdoRdbmsFilterProcessor::IsValidExpression( FdoIdentifierCollection *identifiers )
{
    class UsesNativeExpressionFunctions : public FdoRdbmsBaseFilterProcessor
    {
    public:
        bool                            foundNotNativeSupportedFunction;
        const FdoRdbmsFilterProcessor*  mParentFilterProcessor;

        UsesNativeExpressionFunctions(const FdoRdbmsFilterProcessor* parentFilterProcessor)
        {
            mParentFilterProcessor = parentFilterProcessor;
            foundNotNativeSupportedFunction = false;
        }
        virtual void ProcessFunction(FdoFunction& expr)
        {
            if( foundNotNativeSupportedFunction )
                return;
            if (mParentFilterProcessor->IsNotNativeSupportedFunction(expr.GetName()))
                foundNotNativeSupportedFunction = true;
            // Next check whether or not the argument list requires a redirection
            // to the Expression Engine.
            if ( !mParentFilterProcessor->HasNativeSupportedFunctionArguments(expr) )
                foundNotNativeSupportedFunction = true;

            if( !foundNotNativeSupportedFunction )
            {
                FdoPtr<FdoExpressionCollection>expCol =  expr.GetArguments();
                if( expCol != NULL )
                {
                    for(int j=0;j<expCol->GetCount() && !foundNotNativeSupportedFunction; j++ )
                    {
                        FdoPtr<FdoExpression>exp = expCol->GetItem(j);
                        exp->Process( this );
                    }
                }
            }
        }
    };

    if( identifiers == NULL )
        return true;

    UsesNativeExpressionFunctions finder(this);
    for( int i=0; i<identifiers->GetCount(); i++ )
    {
        FdoPtr<FdoIdentifier>property = identifiers->GetItem(i);
        property->Process( &finder );
        if( finder.foundNotNativeSupportedFunction )
            return false;
    }

    return true;
}

//
// The implementation of the public method that converts FDO filter to dbi SQL strings.
const wchar_t* FdoRdbmsFilterProcessor::FilterToSql( FdoFilter                      *filter,
                                                     const wchar_t                  *className,
                                                     SqlCommandType                 cmdType,
                                                     FdoCommandType                 callerFdoCommand,
                                                     FdoRdbmsFilterUtilConstrainDef *inFilterConstrain,
                                                     bool                           forUpdate,
                                                     FdoInt16                       callerId )

{
    // Before generating the SQL statement for the provided filter, it is
    // required to analyze the filter first. This basically checks the content
    // of the filter and sets flags which will later control the generation
    // of the SQL statement out of the filter. For example, if the filter
    // contains a list of elements that are combined by binary logical 
    // operators, it is not required to nest those elements in the generated
    // SQL statement unless different operators are used. 
    if (filter != NULL)
        AnalyzeFilter(filter);

    // Process the request.
    int j;
    bool ltQueryQualAdded = false;
    DbiConnection  *mDbiConnection = mFdoConnection->GetDbiConnection();
    const FdoSmLpClassDefinition *classDefinition = mDbiConnection->GetSchemaUtil()->GetClass(className);
    const FdoSmLpDataPropertyDefinition* pFeatIdProp = NULL;
    bool isFeatureClass = ( classDefinition != NULL &&  classDefinition->GetClassType() == FdoClassType_FeatureClass );
    ResetBuffer( cmdType );
    mCurrentTableRelationArray.clear();
	mClassArray.clear();
    FdoIdentifierCollection *selectedProperties = NULL;
    FdoRdbmsFilterUtilConstrainDef    filterConst;
    FdoRdbmsFilterUtilConstrainDef   *filterConstraint;
    if( inFilterConstrain != NULL )
    {
        filterConstraint = inFilterConstrain;
        mRequiresDistinct = filterConstraint->distinct;
        selectedProperties = filterConstraint->selectedProperties;
    }
    else
    {
        filterConstraint = &filterConst;
        mRequiresDistinct = false;
    }
    mProcessingOrOperator = false;

    // Make a copy of the class name
    if( mCurrentClassName )
    {
        delete[] mCurrentClassName;
    }
    mCurrentClassName = new wchar_t[ wcslen( className ) + 1];
    wcscpy( mCurrentClassName, className );

    const FdoSmLpClassDefinition *identClass = NULL;
    FdoStringP tableName = mDbiConnection->GetSchemaUtil()->GetDbObjectSqlName(classDefinition);
    const FdoSmLpDataPropertyDefinitionCollection *identProperties = GetIdentityProperties( mCurrentClassName, &identClass );

    // When class has no id, treat it as read-only.
    if((tableName == NULL) || ((cmdType != SqlCommandType_Select) && (identProperties->GetCount() == 0)) )
        throw FdoFilterException::Create(NlsMsgGet1(FDORDBMS_34, "Table name or Primary key for class '%1$ls' does not exist", className));

    const FdoSmLpFeatureClass*  feat = NULL;
    const FdoSmLpGeometricPropertyDefinition *geom = NULL;
    if (isFeatureClass)
    {
        feat = static_cast<const FdoSmLpFeatureClass *>( classDefinition );
        geom = feat->RefGeometryProperty();
        pFeatIdProp = classDefinition->RefFeatIdProperty();
        if ( pFeatIdProp && (!pFeatIdProp->RefColumn()) )
            pFeatIdProp = NULL;
    }
	AddNewClass( classDefinition );

    const FdoSmLpPropertyDefinitionCollection *propertyDefinitions = identClass->RefProperties();

    if ( identProperties->GetCount() > 0 )
    {
        for( int i =0; identProperties!= NULL && i<identProperties->GetCount(); i++ )
        {
            const FdoSmLpPropertyDefinition *prop = propertyDefinitions->RefItem( identProperties->RefItem(i)->GetName() );
            if( prop != NULL )
            {
                AddNewTableRelation( L"",L"", tableName, prop->GetName() );
            }
        }
    }
    else
    {
        // No identity properties, so add an id'less table entry
        AddNewTableRelation( L"",L"", tableName, L"" );
    }

    //
    // We may be able to select the associated object with the main query.
    FdoStringP    relationColumns;
    if( cmdType == SqlCommandType_Select )
    {
        // Find all association property and add their table mappings
        const FdoSmLpPropertyDefinitionCollection *properties = classDefinition->RefProperties();
        for(int i=0; i<properties->GetCount(); i++ )
        {
            if( properties->RefItem(i)->GetPropertyType() == FdoPropertyType_AssociationProperty )
            {
                if( ! CanOptimizeRelationQuery( mDbiConnection->GetSchemaUtil()->GetClass(mCurrentClassName), properties->RefItem(i)) )
                    break;
                FollowRelation( relationColumns, properties->RefItem(i), selectedProperties);
            }
        }

    }

    bool   addedWhereCriteria = false;
    if( filter != NULL )
    {
        HandleFilter( filter );
        addedWhereCriteria = true;
    }

    //
    // Scan the various identifier collections to add any missing table relations.
    FdoIdentifierCollection*  identColArray[3];
    identColArray[0] = filterConstraint->selectedProperties;
    identColArray[1] = filterConstraint->orderByProperties;
    identColArray[2] = filterConstraint->groupByProperties;
    for(j=0; j<3; j++ )
    {
        if( identColArray[j] == NULL )
            continue;

        for (int i=0; i<identColArray[j]->GetCount(); i++)
        {
            FdoPtr<FdoExpression> pExpr = identColArray[j]->GetItem(i);
            if (dynamic_cast<FdoIdentifier*>(pExpr.p) == NULL )
                throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_30, "Unknown expression"));
            PrependProperty( (FdoIdentifier*)pExpr.p, true );
        }
    }


    //
    // Build and prepend the join clause as <LEFT OUTER JOIN | FULL JOIN> ON tab1.c1 = tab2.c1 and tab1.c2 = tab2.c2
    bool first = true;
    FdoStringP   joinStatement="";
    FilterTableRelationDef  lasTabRelation;
    for( size_t index = 0; index < mCurrentTableRelationArray.size(); index++ )
    {
        FilterTableRelationDef  tabRelation;
        tabRelation = mCurrentTableRelationArray[index];
        if( index != 0 )
            lasTabRelation = mCurrentTableRelationArray[index-1];
        if( wcslen( tabRelation.fk_ColumnName ) != 0 &&
            wcslen( tabRelation.fk_TableName )  != 0 &&
            wcslen( tabRelation.pk_ColumnName ) != 0 &&
            wcslen( tabRelation.pk_TableName )  != 0 )
        {
            if( index == 0 || wcscmp(lasTabRelation.fk_TableName, tabRelation.fk_TableName) || wcscmp(lasTabRelation.pk_TableName, tabRelation.pk_TableName) )
            {
                if( tabRelation.useOuterJoin )
                    joinStatement += " LEFT OUTER JOIN ";
                else
                    joinStatement += " INNER JOIN ";
                joinStatement += tabRelation.fk_TableName ;
				if( mUseTableAliases )
				{
					joinStatement += " ";
					joinStatement += tabRelation.fk_TabAlias;
				}
				joinStatement += " ON ";

			}
            else
            {
             //   if( tabRelation.useOuterJoin )
                    joinStatement += L" AND ";
            }
			joinStatement += (( mUseTableAliases )?tabRelation.pk_TabAlias:tabRelation.pk_TableName);
            joinStatement += L"." ;
            joinStatement += tabRelation.pk_ColumnName;
            joinStatement += L" = " ;
			joinStatement += (( mUseTableAliases )?tabRelation.fk_TabAlias:tabRelation.fk_TableName);
            joinStatement += L".";
            joinStatement += tabRelation.fk_ColumnName;
        }
    }

    if( addedWhereCriteria && callerFdoCommand != FdoCommandType_Delete)
    {
        PrependString ( L" WHERE " );
        addedWhereCriteria = false;
    }

	for( size_t index = 0; index < mClassArray.size(); index++ )
	{
		FdoStringP  ltWhereCondition;
		FdoStringP  ltTableExp;

        if( callerFdoCommand == FdoCommandType_Delete )
        {
            GetLtQualificationClause( mClassArray[index], ltWhereCondition );
		    if( ((const wchar_t*)ltWhereCondition)[0] != '\0' )
		    {
			    PrependString (L" AND ");
			    PrependString ( (const wchar_t*)ltWhereCondition );
                addedWhereCriteria = true;
		    }
		    else
			    break; // No LT support
        }
        else
        {
		    GetLtTableExpression( mClassArray[index], ltWhereCondition, ltTableExp, callerFdoCommand );
		    if( ((const wchar_t*)ltWhereCondition)[0] != '\0' )
		    {
			    PrependString ( (const wchar_t*)ltWhereCondition );
			    PrependString (L" ON ");
			    PrependString ( (const wchar_t*)ltTableExp);
			    PrependString (L" INNER JOIN ");
		    }
		    else
			    break; // No LT support
        }
	}

    if( addedWhereCriteria )
        PrependString (L" WHERE ");

    if( ((const wchar_t*)joinStatement)[0] != '\0' )
       PrependString ( (const wchar_t*)joinStatement);

    //
    // Prepend the FROM clause
    FilterTableRelationDef  tabRelation;
    tabRelation = mCurrentTableRelationArray[0];
	
	// SQL Server needs a "WITH" hint
    AppendTablesHints( cmdType, forUpdate );

	if( mUseTableAliases )
	{
		PrependString( tabRelation.fk_TabAlias );
		PrependString ( " " );
	}
	PrependString( tabRelation.fk_TableName );
    
    PrependString ( L" FROM " );
	
    // If this filter applies to a concrete feature class(i.e. all the returned objects are instances of the same class), then we are
    // going to select all the attributes and geometry(for draw) using a single select. Othewise we need to defer selecting the properties
    // untill we know the class of the returned feature.(See the feature reader ReadNext method where this optimization is used.)
    bool bForDraw = true;
    first = true;
    bool useAggregateFunctions = ContainsAggregateFunctions( selectedProperties );

    if( ! isFeatureClass || ( ! classDefinition->GetIsAbstract() ) )
    {
        if ( (selectedProperties == NULL) || (selectedProperties->GetCount() == 0) )
        {
            if( cmdType == SqlCommandType_Update )
            {
                // Use only the identity columns.
                FdoStringP sqlTableName = mDbiConnection->GetSchemaUtil()->GetDbObjectSqlName(mDbiConnection->GetSchemaUtil()->GetClass(mCurrentClassName));
                const  wchar_t* table = GetTableAlias(sqlTableName);
                bool featIdAdded = false;
                for (j=0; j<identProperties->GetCount(); j++)
                {
                    if (first == false)
                        PrependString( L"," );
                    const FdoSmLpDataPropertyDefinition *idProperty = identProperties->RefItem(j);
                    const FdoSmPhColumn *column = idProperty->RefColumn();
                    PrependString( (FdoString*)(column->GetDbName()) );
                    PrependString(L".");
                    PrependString(table);
                    first = false;
                    if ( idProperty == pFeatIdProp )
                        featIdAdded = true;
                }

                // For Feature Classes, make sure feature id property is
                // include ( it might not be an identityProperty ). Feature Id
                // is needed for selection geometry or F_Feature records.
                if ( pFeatIdProp && !featIdAdded )
                {
                    if (first == false)
                        PrependString( L"," );
                    const FdoSmPhColumn *column = pFeatIdProp->RefColumn();
                    PrependString( (FdoString*)(column->GetDbName()) );
                    PrependString(L".");
                    PrependString(table);
                    first = false;
                }
            }
            else
            {
                if (first == false)
                    PrependString(L",");

                if( ! tabRelation.duplicatefkTable )
                {
                    const FdoSmLpClassDefinition *classDefinition = mDbiConnection->GetSchemaUtil()->GetClass(mCurrentClassName);
                    PrependSelectStar( classDefinition->GetDbObjectQName(true), mUseTableAliases ? tabRelation.fk_TabAlias : tabRelation.fk_TableName ); // i.e. ".*" or expand the list of properties 
                }
                if( relationColumns.GetLength() != 0 )
                    PrependString( (const wchar_t*)relationColumns );

                first = false;
            }
        }
        else
        {
            bool *bExist = new bool[identProperties->GetCount()];// Keeps track of all the identify properties.
                                                            // Set to true if the property exist in the select list.

            const wchar_t* featIdColName = NULL;
            bool isFeatIdOnlyQuery = false;
            if ( isFeatureClass )
            {
                const FdoSmLpDataPropertyDefinition* pFeatIdProp = classDefinition->RefFeatIdProperty();

                if ( pFeatIdProp && pFeatIdProp->RefColumn() )
                {
                    featIdColName = pFeatIdProp->RefColumn()->GetName();
                }
            }

            for (int i=0; i<identProperties->GetCount(); i++)
                bExist[i] = false;

            bForDraw = false;
            for (int i=0; i<selectedProperties->GetCount() && !(useAggregateFunctions && mRequiresDistinct); i++)
            {
                FdoPtr<FdoIdentifier> property = selectedProperties->GetItem(i);
                if (first == false)
                {
                    PrependString( L"," );
                }
                PrependProperty( property );

                first = false;
                for (j=0; j<identProperties->GetCount(); j++)
                {
                    if (FdoCommonOSUtil::wcsicmp(identProperties->RefItem(j)->GetName(), property->GetName()) == 0)
                    {
                        bExist[j] = true;
                        break;
                    }
                }
                if( featIdColName && FdoCommonOSUtil::wcsicmp( featIdColName, property->GetName() ) == 0 )
                    isFeatIdOnlyQuery = true;
            }

            // explictly add the identity columns that were not in the select list
            // If the query is only selecting featid column, then we do not add the rest of the
            // identity columns as this is a query used by internal component that only expect
            // the featid column.
            // Also if the we have a distinct clause, we should not be adding any columns as that will
            // change the returned result.
            if( (! isFeatIdOnlyQuery && ! filterConstraint->distinct) && 
                !( useAggregateFunctions && !mRequiresDistinct  ) && 
                !( callerId == FdoCommandType_SelectAggregates && !mRequiresDistinct ) )
            {
                FdoStringP sqlTableName = mDbiConnection->GetSchemaUtil()->GetDbObjectSqlName(mDbiConnection->GetSchemaUtil()->GetClass(mCurrentClassName));
                const  wchar_t* table = GetTableAlias(sqlTableName);
                for (j=0; j<identProperties->GetCount(); j++)
                {
                    if (bExist[j] == false)
                    {
                        if (first == false)
                            PrependString( L"," );
                        const FdoSmLpDataPropertyDefinition *idProperty = identProperties->RefItem(j);
                        const FdoSmPhColumn *column = idProperty->RefColumn();
                        PrependString( (FdoString*)(column->GetDbName()) );
                        PrependString(L".");
                        PrependString(table);
                        first = false;
                    }
                }
            }

            delete [] bExist;
        }
        //
        // We need to add the order by and group by columns if they are not already in the select list
        // Oracle does no like having oder by column that is not in the select list.
        // "Select table1.col1 from table1,table2 where table1.id=table2.id order by table2.col2" does no work
        // It should be:
        // "Select table1.col1,table2.col2 from table1,table2 where table1.id=table2.id order by table2.col2"

        // Order by and Group by are mutually exclusive: Only one can be found
        FdoIdentifierCollection *orderOrGroupByList = NULL;
        if( filterConstraint->orderByProperties != NULL )
            orderOrGroupByList = filterConstraint->orderByProperties;
        else if( filterConstraint->groupByProperties != NULL )
            orderOrGroupByList = filterConstraint->groupByProperties;

        if( orderOrGroupByList != NULL )
        {
            for (int i=0; i<orderOrGroupByList->GetCount(); i++)
            {
                FdoPtr<FdoIdentifier> property = orderOrGroupByList->GetItem(i);
                // Make sure this property was not already added by the user select list.
                if ( selectedProperties != NULL )
                {
                    for (j=0; j<selectedProperties->GetCount(); j++)
                    {
                        FdoPtr<FdoIdentifier> selectprop = selectedProperties->GetItem(j);
                        if (FdoCommonOSUtil::wcsicmp(selectprop->GetName(), property->GetName()) == 0)
                        {
                            break;
                        }
                    }
                    if( j != selectedProperties->GetCount() )
                        continue;
                    else
                    {
                        // May be it's an identity property which would be added too
                        for (j=0; j<identProperties->GetCount();    j++)
                        {
                            if (FdoCommonOSUtil::wcsicmp(identProperties->RefItem(j)->GetName(), property->GetName())    == 0)
                            {
                                break;
                            }
                        }
                        if( j != identProperties->GetCount() )
                            continue;
                    }
                }
                else
                {
                    // If this is a class property, then it's already covered by the table.* case
                    if (wcschr((wchar_t *)property->GetText(), L'.') == NULL)
                        continue;
                }
                if (first == false)
                {
                    PrependString( L"," );
                }
                PrependProperty( property );
                first = false;
            }
        }
    }
    else // if( ! isFeatureClass || ( ! classDefinition->GetIsAbstract() ) )
    {
        PrependString ( tabRelation.fk_ColumnName );
        PrependString ( L"." );
		if( mUseTableAliases )
			PrependString ( tabRelation.fk_TabAlias );
		else
			PrependString ( tabRelation.fk_TableName );

    }

    if( mRequiresDistinct )
    {
        if( ! useAggregateFunctions )
        {
            PrependString ( L"DISTINCT " );
        }
        else
        {
            // With aggregate functions, distinct will do nothing and we end up with the aggreagte function using
            // duplicate records. We just opened a big can of worm!!

            // Use select .. where EXISTS to get only those records required by the aggregate functions.
            PrependString ( L" WHERE EXISTS (SELECT DISTINCT " );

            // Need to add the from clause

            // clear the existing table relation; it's no longer needed. We can now use it to build the table list for aggregate functions.
            mCurrentTableRelationArray.clear();
			mClassArray.clear();
            // First pass build the table dependancies
            AddNewTableRelation( L"",L"", mDbiConnection->GetTable( mCurrentClassName ),L"NotUsed");
            for (int i=0; i<selectedProperties->GetCount(); i++)
            {
                FdoPtr<FdoIdentifier> property = selectedProperties->GetItem(i);
                PrependProperty( property, true );
            }
            // Add the table list
            PrependTables();
            PrependString ( L" FROM " );
            first = true;
            // And finaly add the aggregate functions
            for (int i=0; i<selectedProperties->GetCount(); i++)
            {
                FdoPtr<FdoIdentifier> property = selectedProperties->GetItem(i);

                if (first == false)
                {
                    PrependString(L",");
                }
                PrependProperty( property );

                first = false;
            }
            AppendString( L")" );
        }
    }

    // Add the group by clause if it's required
    AppendGroupBy( filterConstraint );

	// Add the order by clause if it's required
    AppendOrderBy( filterConstraint );

    PrependString ( L"SELECT " );

    if( cmdType == SqlCommandType_Select && forUpdate )
    {
        AppendForUpdate();
    }

    FdoStringsP strings = FdoStringCollection::Create(className, L".");
    if (strings->GetCount() != 1)
    {
        const FdoSmLpObjectPropertyDefinition* objProp = static_cast<const FdoSmLpObjectPropertyDefinition*>(classDefinition->GetParent()->GetParent());

        if (cmdType == SqlCommandType_Select && objProp->GetObjectType() == FdoObjectType_OrderedCollection)
        {
            const FdoSmLpDataPropertyDefinition * id = objProp->RefIdentityProperty();
            if (id)
            {
                const FdoSmPhColumn* column = id->RefColumn();
                FdoStringP columnName = column ? column->GetDbName() : FdoStringP();
                if (columnName != L"")
                {
                    bool added = false;
                    if( !( filterConstraint->orderByProperties != NULL  && filterConstraint->orderByProperties->GetCount() != 0) )
                    {
                        AppendString( L" order by " );
                    }
                    else
                    {
                        // Make sure that we didn't already include this property on the order by list
                        for(int i=0;i<filterConstraint->orderByProperties->GetCount();i++)
                        {
                            FdoPtr<FdoIdentifier>ident = filterConstraint->orderByProperties->GetItem(i);
                            if( FdoCommonOSUtil::wcsicmp(ident->GetName(), id->GetName() ) == 0 )
                            {
                                added = true;
                                break;
                            }
                        }
                        if( ! added )
                            AppendString( L", " );
                    }
                    if( ! added )
                    {
                        AppendString( (FdoString*)(columnName) );
                        AppendString( L" ");
                        if (objProp->GetOrderType() == FdoOrderType_Descending)
                        {
                            AppendString( L"desc ");
                        }
                    }
                }
            }
        }
    }

    return &mSqlFilterText[mFirstTxtIndex];
}


//
// This is a simplified version of the original FilterToSql implementation and used
// if the request has to be passed on to the Expression Engine. It generates a simplified
// SQL statement that is used to get the data to be handed over to the Expression Engine.
const wchar_t* FdoRdbmsFilterProcessor::FilterToSql( FdoFilter     *filter,
                                                     const wchar_t *className )

{
    // Reset the buffer.
    ResetBuffer( SqlCommandType_Select );

    // Make a copy of the class name
    if( mCurrentClassName )
    {
        delete[] mCurrentClassName;
    }
    mCurrentClassName = new wchar_t[ wcslen( className ) + 1];
    wcscpy( mCurrentClassName, className );

    // Get the class definition for the identified class.
    DbiConnection *mDbiConnection = mFdoConnection->GetDbiConnection();
    const FdoSmLpClassDefinition *classDefinition = mDbiConnection->GetSchemaUtil()->GetClass(className);

    // Generate the select-statement that selects the data to be used to create the
    // reader handed over to the Expression Engine.
    AppendString( L"SELECT " );

    FdoInt32 i;
    FdoStringsP all = FdoStringCollection::Create();
    const FdoSmLpPropertyDefinitionCollection *pCol = classDefinition->RefProperties();
    for( i = 0; i < pCol->GetCount(); i++ )
    {
        const FdoSmLpDataPropertyDefinition *dataPropertyDef = 
                                FdoSmLpDataPropertyDefinition::Cast(classDefinition->RefProperties()->RefItem(i));
        if( dataPropertyDef != NULL )
        {
            if(( FdoCommonOSUtil::wcsicmp( dataPropertyDef->GetColumnName(), L"classname" )  != 0 ) &&
               ( FdoCommonOSUtil::wcsicmp( dataPropertyDef->GetColumnName(), L"schemaname" ) != 0 )     )
                all->Add( dataPropertyDef->GetColumnName() );
        }
		else
		{
			const FdoSmLpGeometricPropertyDefinition *geomPropertyDef = 
								FdoSmLpGeometricPropertyDefinition::Cast(classDefinition->RefProperties()->RefItem(i));
			if ( geomPropertyDef != NULL )
			{
				if ( geomPropertyDef->RefColumn() )
				{
					const FdoSmPhColumn* column = geomPropertyDef->RefColumn();
					all->Add(GetGeometryString(column->GetDbName()));
				}
			}
		}
    }

    if (all->GetCount() > 0)
        AppendString( (FdoString *)all->ToString() );
    else
        AppendString( L" * ");

    FdoStringP tableName = mDbiConnection->GetSchemaUtil()->GetDbObjectSqlName(classDefinition);
    AppendString( L" FROM " );
    AppendString( (FdoString *)tableName );

    if( filter != NULL )
    {
        FdoString * tableAlias = GetTableAlias( tableName );
        if (wcscmp(tableAlias, tableName) != 0)
            AppendString(  GetTableAlias( tableName ) );
        AppendString( L" WHERE " );
        HandleFilter( filter );
    }

    return &mSqlFilterText[mFirstTxtIndex];
}

FdoStringP FdoRdbmsFilterProcessor::GetGeometryString( FdoString* columnName )
{ 
    return columnName; 
}

FdoStringP FdoRdbmsFilterProcessor::GetGeometryTableString( FdoString* tableName )
{ 
    return tableName; 
}

void FdoRdbmsFilterProcessor::PrependSelectStar( FdoStringP tableName, FdoString* tableAlias )
{ 
    DbiConnection  *mDbiConnection = mFdoConnection->GetDbiConnection();
    FdoSchemaManagerP sm = mDbiConnection->GetSchemaManager();
    FdoSmPhMgrP phMgr = sm->GetPhysicalSchema();
    FdoSmPhDbObjectP dbObject;
    
    if ( tableName.Contains(L"." ) )
        dbObject = phMgr->FindDbObject( tableName.Right(L"."), tableName.Left(L".") );
    else
        dbObject = phMgr->FindDbObject( tableName );

    if ( dbObject ) {
	    const FdoSmPhColumnCollection* columns = dbObject->RefColumns();
        bool    first = true;

        for (int i = 0; i < columns->GetCount(); i++)
        {
		    const FdoSmPhColumn* column = columns->RefItem(i);
		    FdoStringP colNameTmp = column->GetName();
		    FdoString *colName = colNameTmp;
            FdoSmPhColType colType = ((FdoSmPhColumn *)column)->GetType();

            if ( colType != FdoSmPhColType_Unknown ) 
            {
		        bool bGeometry = colType == FdoSmPhColType_Geom;

                if (!first )
                    PrependString( L"," );

                if( bGeometry ) 
                {
	                FdoStringP  colName = GetGeometryString( (FdoString*)(column->GetDbName()) );
                    PrependString( (FdoString*)colName );
                }
                else
                {
                    PrependString(L"\"");
                    PrependString(colName);
                    PrependString(L"\"");
                }

                PrependString(L".");

                if ( bGeometry )
                    PrependString( (FdoString*) GetGeometryTableString(tableAlias));
                else
                    PrependString(tableAlias);

                first = false;
            }
         }
    }
    else
    {
        // Schema Manager can't find table. Fall back to selecting
        // everything and hope it works. If not, the RDBMS will generate
        // error message indicating the cause of the problem.
        PrependString ( L"*" ); 
        PrependString ( L"." ); 
        PrependString (tableAlias);     
    }
}

FdoRdbmsFilterProcessor::BoundGeometry::BoundGeometry(
    FdoIGeometry* geometry,
    FdoInt64 srid
)
{
    mGeometry = FDO_SAFE_ADDREF(geometry);
    mSrid = srid;
}

FdoIGeometry* FdoRdbmsFilterProcessor::BoundGeometry::GetGeometry()
{
    return FDO_SAFE_ADDREF(mGeometry.p);
}

FdoInt64 FdoRdbmsFilterProcessor::BoundGeometry::GetSrid()
{
    return mSrid;
}

FdoRdbmsFilterProcessor::BoundGeometry::~BoundGeometry(void)
{
}
