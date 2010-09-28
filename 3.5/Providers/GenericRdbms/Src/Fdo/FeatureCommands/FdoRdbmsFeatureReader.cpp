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
#include <assert.h>
#include "FdoCommon.h"
#include "DbiConnection.h"
#include "FdoRdbmsFeatureReader.h"
#include "FdoRdbmsDescribeSchemaCommand.h"
#include "FdoRdbmsUtil.h"
#include "FdoRdbmsSchemaUtil.h"
#include "FdoRdbmsFilterProcessor.h"
#include "FdoRdbmsBLOBStreamReader.h"

#include "Inc/ut.h"

#include "FdoCommonOSUtil.h"
#include "FdoCommonMiscUtil.h"

#include <limits>       // For quiet_NaN()
using namespace std;

//#define FDORDBMS_SHOW_CACHE_PERF

#define  PROPERTY2COLNAME( p1, p2 ) (Property2ColName( p1, p2, false))
#define  PROPERTY2COLNAME_IDX( p1, p2, p3, p4 ) (Property2ColName( p1, p2, false, p3, p4))
#define  PROPERTY2COLNAME_IDX_W( p1, p2, p3, p4 ) (Property2ColNameW( p1, p2, false, p3, p4))

static  char  *strEndOfRecordExp = "End of feature data or NextFeature not called";
static  char  *strObjPropetryExp = "Property '%1$ls' is an object property and cannot be returned through a basic type; use GetFeatureObject";
static  char  *strNUllPropetryExp = "Property '%1$ls' value is NULL; use IsNull method before trying to access the property value";

//
// This reader is used to read one row of object or association data.
// The row is already selected by the main query using the following form:
// select <class table name>.*, <object/associated table name>.* from <class table name>, <object/associated table name> ...
class FdoRdbmsObjectFeatureReader : public FdoRdbmsFeatureReader
{
public:
    FdoRdbmsObjectFeatureReader( FdoIConnection *connection, GdbiQueryResult *queryResult, bool isFeatureQuery, const FdoSmLpClassDefinition *classDef, FdoFeatureSchemaCollection *schmCol, FdoIdentifierCollection *properties, int level, FdoRdbmsSecondarySpatialFilterCollection * secondarySpatialFilters=NULL ):
    FdoRdbmsFeatureReader( connection, queryResult, isFeatureQuery, classDef, schmCol, properties, level, secondarySpatialFilters )
    {
        mInvoked = false;

        if( mSchemaCollection )
            mSchemaCollection->AddRef();

        if (mFdoClassDefinition)
            mFdoClassDefinition->AddRef();

        mNumPropertyInfoDefs = 0;
        mLastPropertyInfoDef = 0;
    }

    virtual bool     ReadNext( )
    {
        mHasMoreFeatures = false;
        if( mInvoked )
            return false;

        mAttrsQidIdx = 0;
        mAttrQueryCache[mAttrsQidIdx].query = mQueryResult;
        wcscpy( mLastClassName, mCurrentClassName );
        mPropertiesFetched = true;
        mInvoked = true;
        return mHasMoreFeatures = true;
    }

    virtual void     Close()
    {
        // Don't close the cursor since it is not owned by this reader.
        mInvoked = false;
    }

private:
    bool        mInvoked;
};

#define GET_ATTRIBUTE( function, type ) \
    type    value; \
    if( ! mHasMoreFeatures ) \
        throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_57, strEndOfRecordExp)); \
    FetchProperties( ); \
    if( mAttrQueryCache[mAttrsQidIdx].query == NULL ) \
        throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_57, strEndOfRecordExp)); \
    try \
    { \
        FdoPropertyType proptype; \
		int		cacheIndex; \
        const wchar_t *colNameW = PROPERTY2COLNAME_IDX_W( propertyName, &proptype, NULL, &cacheIndex );\
		if (colNameW == NULL) \
		{ \
			if( proptype != FdoPropertyType_DataProperty ) \
				throw FdoCommandException::Create(NlsMsgGet1( FDORDBMS_67, strObjPropetryExp,  propertyName )); \
			throw ""; \
		} \
        bool isNull = false; \
		value = function( GetPropertyInfoDef(cacheIndex)->columnPosition, &isNull, NULL ); \
        if( isNull ) \
            throw FdoCommandException::Create(NlsMsgGet1( FDORDBMS_385, strNUllPropetryExp,  propertyName )); \
    } \
    catch ( char * ) \
    { \
        ThrowPropertyNotFoundExp( propertyName ); \
        throw; \
    } \


#define FEATUREREADER_CLEANUP \
           if( mAttrQueryCache[mAttrsQidIdx].query != NULL ) \
            { \
                mAttrQueryCache[mAttrsQidIdx].query->Close(); \
                delete mAttrQueryCache[mAttrsQidIdx].query; \
                mAttrQueryCache[mAttrsQidIdx].query = NULL; \
                delete[] mAttrQueryCache[mAttrsQidIdx].mColList; \
            } \
            if( mAttrQueryCache[mAttrsQidIdx].statement != NULL ) \
            { \
                delete mAttrQueryCache[mAttrsQidIdx].statement; \
                mAttrQueryCache[mAttrsQidIdx].statement = NULL; \
            } 


// Class FdoRdbmsFeatureReader
FdoRdbmsFeatureReader::FdoRdbmsFeatureReader( FdoIConnection *connection, GdbiQueryResult* queryResult, bool  isFeatureQuery, const FdoSmLpClassDefinition *classDef, FdoFeatureSchemaCollection *schmCol, FdoIdentifierCollection *properties, int level, FdoRdbmsSecondarySpatialFilterCollection * secondarySpatialFilters, vector<int> *logicalOps):
  mQueryResult(queryResult),
  mNextQidToFree( 0 ),
  mFdoConnection( NULL ),
  mConnection( NULL ),
  mHasMoreFeatures( false ),
  mAttrsQidIdx( -1 ),
  mPropertiesFetched( false ),
  mIsFeatureQuery( isFeatureQuery ),
  mClassDefinition( classDef ),
  mCurrentRevisionNumber(0),
  mSchemaCollection( schmCol  ),
  mProperties(properties),
  mLevel(level),
  mFdoClassDefinition(NULL),
  mGeometryCache(NULL)
{
    mFdoConnection = dynamic_cast<FdoRdbmsConnection*>(connection);
    if( mFdoConnection )
    {
        mFdoConnection->AddRef();
        mConnection = mFdoConnection->GetDbiConnection();
    }

    memset( mAttrQueryCache, 0, sizeof(AttributeQueryDef)*QUERY_CACHE_SIZE );
    for( int i = 0; i<QUERY_CACHE_SIZE; i++ )
    {
        mAttrQueryCache[i].query = NULL;
        mAttrQueryCache[i].statement = NULL;
    }
    if( mProperties )
        mProperties->AddRef();
    mLastClassName[0] = '\0';
    mLastAttrQueryIdx = -1;
    wcsncpy( mCurrentClassName, classDef->GetQName(), GDBI_SCHEMA_ELEMENT_NAME_SIZE );
    mCurrentClassName[GDBI_SCHEMA_ELEMENT_NAME_SIZE-1] = '\0';
    mCurrentRevisionNumberValid = false;
    mColCount = -1;
    mUnskippedColCount = -1;
    mColList = NULL;

	// Columns cache for fast property-column match retrieval
    mNumPropertyInfoDefs = 0;
    mLastPropertyInfoDef = 0;

	m_cacheHits	= 0;
	m_cacheMissed1 = 0;
	m_cacheMissed2 = 0;

    mSecondarySpatialFilters = FDO_SAFE_ADDREF(secondarySpatialFilters);

	// Make a copy
	if ( logicalOps )
	{
		for ( size_t i = 0; i < logicalOps->size(); i++)
			mFilterLogicalOps.push_back( logicalOps->at(i) );
	}
    mComputedProperties = NULL;
    if (mProperties != NULL)
    {
        mComputedProperties = FdoIdentifierCollection::Create();
        for (int i = 0; i < mProperties->GetCount(); i++)
        {
            FdoPtr<FdoIdentifier> id = mProperties->GetItem(i);
            FdoComputedIdentifier* pComputedId = dynamic_cast<FdoComputedIdentifier *>( id.p );
            if (pComputedId != NULL)
                mComputedProperties->Add(pComputedId);
        }
    }
    // TODO: push down to Schema Manager, rather than hard-code property names.
    mClassIdPropName = L"ClassId";
    if(Property2ColName(mClassIdPropName, NULL, true) == NULL)
        mClassIdPropName = L"";

    mRevNumPropName = L"RevisionNumber";
    if(Property2ColName(mRevNumPropName, NULL, true) == NULL)
        mRevNumPropName = L"";
}


FdoRdbmsFeatureReader::~FdoRdbmsFeatureReader()
{
  if( mSchemaCollection )
      mSchemaCollection->Release();

  if (mFdoClassDefinition)
      mFdoClassDefinition->Release();

  if (mGeometryCache)
      mGeometryCache->Release();

  if( mProperties )
      mProperties->Release();

  if( mComputedProperties )
      mComputedProperties->Release();

  if( mColList != NULL )
      delete[] mColList;

  for(int i = 0; i < (int)mPropertyInfoDefs.size(); i++)
  {
      FdoRdbmsPropertyInfoDef* val = mPropertyInfoDefs.at(i);
      delete val;
  }
  mPropertyInfoDefs.clear();

#ifdef	FDORDBMS_SHOW_CACHE_PERF
  printf("[Columns cache: Hits=%ld Missed1==%ld Missed2==%ld]\n", m_cacheHits, m_cacheMissed1, m_cacheMissed2);
#endif

  if( mFdoConnection )
    mFdoConnection->Release();
}

const char* FdoRdbmsFeatureReader::Property2ColName( const wchar_t *propName, FdoPropertyType *type, bool systemOnly, bool *found, int *index )
{
	return Property2ColNameChar( propName, type, systemOnly, found, index );
}

const wchar_t* FdoRdbmsFeatureReader::Property2ColNameW( const wchar_t *propName, FdoPropertyType *type, bool systemOnly, bool *found, int *index )
{
	const char*  col = Property2ColNameChar( propName, type, systemOnly, found, index );
	return ( col ? GetPropertyInfoDef(*index)->columnNameW : (const wchar_t*)NULL );
}

const char* FdoRdbmsFeatureReader::Property2ColNameChar( const wchar_t *propName, FdoPropertyType *type, bool systemOnly, bool *found, int *index )
{
    const char*             string = NULL;
	FdoStringP				colName;
    FdoRdbmsPropertyInfoDef *cacheElem = NULL;
    bool                    found2 = false;
    int                     cacheIndex;
	FdoPropertyType			propType;

    if( mClassDefinition == NULL )
        return NULL;

    if( found )
        *found = false;

    // Optimize the linear search in the cache: 
    // Chances are it's the A) next property (usually fetched in order) 
    // or B) the current property (after IsNull)
    for ( int i = mLastPropertyInfoDef; !found2 && i < mNumPropertyInfoDefs; i++ )
    {
        cacheElem = GetPropertyInfoDef(i);
	
        found2 = ( wcscmp( propName, cacheElem->propertyName ) == 0) &&
            ( cacheElem->isSystem || (!systemOnly) );

       cacheIndex = i;	
    }

    for ( int i = 0; !found2 && i < mLastPropertyInfoDef; i++ )
    {
        cacheElem = GetPropertyInfoDef(i);

        found2 = ( wcscmp( propName, cacheElem->propertyName ) == 0) &&
            ( cacheElem->isSystem || (!systemOnly) );

       cacheIndex = i;
    }

    // Fast return if property found in the cache 
    if ( found2 )
    {
		m_cacheHits++;

        if ( found )
            *found = true;
        if ( type )
            *type = cacheElem->propertyType;
        if ( index )
            *index = cacheIndex;  

        mLastPropertyInfoDef = cacheIndex;

        if ( ( wcslen(cacheElem->columnPosition) == 0 ) && mQueryResult )
        {
            FdoStringP	colPos = FdoStringP::Format(L"%ld", GetColumnIndex(cacheElem->columnQName));
		    wcscpy( cacheElem->columnPosition, colPos );
        }
        return cacheElem->columnQName;
    }

    if( type )
        *type = FdoPropertyType_DataProperty;

    if ( mClassDefinition == NULL ) 
        return NULL;

    const FdoSmLpPropertyDefinitionCollection *propertyDefinitions = mClassDefinition->RefProperties();
    const FdoSmLpPropertyDefinition *propertyDefinition = propertyDefinitions->RefItem(propName);

    if ( propertyDefinition != NULL )
	{	
		m_cacheMissed1++;

        if( found )
            *found = true;

        propType = propertyDefinition->GetPropertyType();
        if( type != NULL )
            *type = propType;

		if ( propType == FdoPropertyType_DataProperty ||
             propType == FdoPropertyType_GeometricProperty)
		{
			const FdoSmLpSimplePropertyDefinition* dataProp = 
				static_cast<const FdoSmLpSimplePropertyDefinition*>(propertyDefinition);
			const FdoSmPhColumn *column = dataProp->RefColumn();
            if( column == NULL )
                return NULL;
			
			colName = FdoStringP(column->GetName());
            string = mConnection->GetUtility()->UnicodeToUtf8((const wchar_t *)colName);

            // Cache
            cacheElem = GetPropertyInfoDef(mNumPropertyInfoDefs);

            wcscpy( cacheElem->propertyName, propName );
            strcpy( cacheElem->columnQName, string );
			wcscpy( cacheElem->columnNameW, colName );
            cacheElem->propertyType = propType;
            cacheElem->isSystem = propertyDefinition->GetIsSystem();
			wcscpy(cacheElem->columnPosition, L""); 

			cacheIndex = mNumPropertyInfoDefs;

            // Remember this
            mLastPropertyInfoDef = mNumPropertyInfoDefs;

            mNumPropertyInfoDefs++;

            if ( cacheElem->isSystem || (!systemOnly)) 
            {
                if ( index )
                    *index = cacheIndex;
            }
            else
            {
                return NULL;
            }
		}
		else // Object, Association properties etc.
		{
			if ( index )
				*index = -1;  

			return NULL;
		}
	    // Initialize Gdbi column index in case not initialize before. 
	    // (Except for "classid" and "revisionumber" which may not be selected)
	    if ( mQueryResult )
	    {
            FdoStringP	colPos = FdoStringP::Format(L"%ld", GetColumnIndex(cacheElem->columnQName));
		    wcscpy( cacheElem->columnPosition, colPos );
	    }
	}
	else if ( systemOnly ) 
	{
        // If it is a system property, it can't be a computed property.
        return NULL;
    }
	else
	{
		m_cacheMissed2++;

		// May be a computed identifier.
		colName = GetDbAliasName( propName, &propType );

		// Use the propName as col name.
		if (colName == NULL )
			return NULL;

        string = mConnection->GetUtility()->UnicodeToUtf8((const wchar_t *)colName);
        
        // Cache
        cacheElem = GetPropertyInfoDef(mNumPropertyInfoDefs);

        wcscpy( cacheElem->propertyName, propName );
		strcpy( cacheElem->columnQName, string );
		wcscpy( cacheElem->columnNameW, colName );

        cacheElem->propertyType = propType;
		wcscpy(cacheElem->columnPosition, L""); 

		cacheIndex = mNumPropertyInfoDefs;

        if ( index )
            *index = cacheIndex;

		if( type != NULL )
			*type = propType;

        // Remember this
        mLastPropertyInfoDef = mNumPropertyInfoDefs;

        mNumPropertyInfoDefs++;
        // since is a calculation we need to handle that differently
        FdoStringP	colPos = FdoStringP::Format(L"%ld", GetColumnIndex(cacheElem->columnQName, false));
		wcscpy( cacheElem->columnPosition, colPos );
        return cacheElem->columnQName;
	}
	return cacheElem->columnQName;
}

int FdoRdbmsFeatureReader::GetColumnIndex(const char *propName, bool avoidCalculations)
{
    int retVal = -1;
    if ( mQueryResult )
    {
        if (mColCount == -1)
            GetPropertyCount();
        for ( int k=0; k<mColCount; k++ )
        {
            if (!avoidCalculations)
            {
                if(mColList[k].c_alias[0] != '\0' && (0 == strcmp(mColList[k].c_alias, propName)))
                    return k+1;
            }
            else
            {
                if(mColList[k].c_alias[0] == '\0' && (0 == FdoCommonOSUtil::stricmp(mColList[k].column, propName)))
                    return k+1;
            }
        }
    }
    return retVal;
}

const char* FdoRdbmsFeatureReader::GetDbAliasName( const wchar_t *propName, FdoPropertyType *type )
{
    if( mComputedProperties == NULL || mComputedProperties->GetCount() == 0 )
        return NULL;

	if ( type )
		*type = FdoPropertyType_DataProperty;

    for (int i=0; i<mComputedProperties->GetCount(); i++)
    {
        FdoPtr<FdoComputedIdentifier>compIdent = static_cast<FdoComputedIdentifier*>(mComputedProperties->GetItem( i ));
        if(0 == wcscmp(compIdent->GetName(), propName))
        {
            char colName[3*GDBI_MAXIMUM_TEXT_SIZE];

            // Returned buffer from MakeDbValidName is volatile and can be trashed by
            // calls to GetClassDefinition(). Save away buffer in local variable.
            const char* p = mConnection->GetSchemaUtil()->MakeDBValidName(propName);
            bool colNameNull = false;
            if ( p ) 
            {
                strcpy(colName, p);
            }
            else
            {
                colName[0] = '\0';
                colNameNull = true;
            }

            // Get the type from the function type 
			if ( type != NULL ) 
			{
				FdoPtr<FdoExpression>   expr = compIdent->GetExpression();
                FdoPtr<FdoClassDefinition> classDef = GetClassDefinition();
                FdoDataType tempDataType;
                GetExpressionType(mFdoConnection, classDef, colNameNull ? (const char*) NULL : colName, expr, *type, tempDataType);
			}
			
            if ( colNameNull ) 
                return NULL;

            // Copy colName back to volatile buffer. Caller responsible for using colName
            // before it gets trashed.
            char* p2 = mConnection->GetUtility()->newCharP();
            strcpy(p2, colName);

            return p2;
        }
    }
    return NULL;
}

void FdoRdbmsFeatureReader::GetExpressionType(FdoIConnection* connection, FdoClassDefinition* classDef, const char* colName, FdoExpression* expr, FdoPropertyType &propType, FdoDataType &dataType)
{
    try
    {
        FdoPtr<FdoIExpressionCapabilities> expressionCaps = connection->GetExpressionCapabilities();
        FdoPtr<FdoFunctionDefinitionCollection> functions = expressionCaps->GetFunctions();
        FdoCommonMiscUtil::GetExpressionType(functions, classDef, expr, propType, dataType);
    }
    catch (FdoException *e)
    {
        e->Release();

        // The expression was not recognized, use RDBI to determine types:
        for ( int k=0; k<mColCount; k++ ) 
        {
            if( strcmp(colName, mColList[k].c_alias) == 0 )
            {
                if (mColList[k].datatype == RDBI_GEOMETRY)
                {
                    propType = FdoPropertyType_GeometricProperty;
                }
                else
                {
                    propType = FdoPropertyType_DataProperty;
                    dataType = FdoRdbmsUtil::DbiToFdoType( mColList[k].datatype );
                }
                break;
            }
        }
   }
}

// This is an internal method to support the DataReader
FdoInt32 FdoRdbmsFeatureReader::GetPropertyCount()
{
    if( mUnskippedColCount != -1 )
        return mUnskippedColCount;

    // Not initialized yet
    mUnskippedColCount = 0;
    mColCount = mQueryResult->GetColumnCount( );
    mColList = new GdbiColumnDesc[mColCount];
    int  colIdx = 0;
    bool doIndexMatch = (mProperties != NULL && mComputedProperties != NULL && mComputedProperties->GetCount()) ? (mProperties->GetCount() <= mColCount) : false;
    std::vector<int> idxs;
    for ( int k=0; k<mColCount; k++ )
    {
        mColList[k].c_alias[0] = '\0';
        if( mQueryResult->GetColumnDesc( k+1, mColList[colIdx] ) )
            colIdx++;

        if ( ! SkipColumnForProperty( k ) )
        {
            // get only non-skippable columns 
            idxs.push_back(k);
            mUnskippedColCount++;
        }
    }
    if (doIndexMatch && idxs.size() != 0)
        ProcessCalculations(idxs);

    mColCount = colIdx;
    return mUnskippedColCount;
}

void FdoRdbmsFeatureReader::ProcessCalculations(std::vector<int>& idxs)
{
    int rowsToSkip = idxs.size() - mProperties->GetCount();
    if (rowsToSkip < 0)
        return;
    for (int k = 0; k < (int)idxs.size(); k++)
    {
        int colIdx = idxs[k];
        bool calcPropNotFound = false;
        if (rowsToSkip == 0 || (rowsToSkip - k) <= 0)
        {
            // properties come in inverse order
            int idxSelCol = mProperties->GetCount() - 1 - (k - rowsToSkip);
            // in case properties are mixed try old style select by setting calcPropNotFound = true
            if (idxSelCol >= 0 && idxSelCol < mProperties->GetCount())
            {
                FdoPtr<FdoIdentifier> pId = mProperties->GetItem(idxSelCol);
                FdoComputedIdentifier* pCompId = dynamic_cast<FdoComputedIdentifier*>(pId.p);
                if (pCompId)
                {
                    if (0==FdoCommonOSUtil::stricmp(mColList[colIdx].column, GetDbAliasName(pCompId->GetName())))
                        strcpy(mColList[colIdx].c_alias, GetDbAliasName(pCompId->GetName()));
                    else
                        calcPropNotFound = true; 
                }
            }
            else
                calcPropNotFound = true; 
        }
        // in case we cannot do index match try old method
        if (calcPropNotFound)
        {
            for (int i = 0; mComputedProperties && i < mComputedProperties->GetCount(); i++)
            {
                FdoPtr<FdoComputedIdentifier> pComputedId = static_cast<FdoComputedIdentifier*>(mComputedProperties->GetItem(i));
                if (0 == FdoCommonOSUtil::stricmp(mColList[k].column, GetDbAliasName(pComputedId->GetName())))
                {
                    strcpy(mColList[colIdx].c_alias, GetDbAliasName(pComputedId->GetName()));
                    break;
                }
            }
        }
    }
}

// This is an internal method to support the DataReader
FdoString* FdoRdbmsFeatureReader::GetPropertyNameForDataReader(FdoInt32 index)
{
    if( index >= GetPropertyCount() )
       throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_52, "Index out of range"));

    int  colIdx = 0;
    for ( int k=0; k<mUnskippedColCount && k<index; k++ )
    {
        colIdx++;   // Keep up with loop's iteration.
        while ( colIdx<mColCount && SkipColumnForProperty( colIdx ) )
            colIdx++;   // Skip over any skippable columns that are positioned before the one we want.
    }

    char* colName = mColList[colIdx].c_alias;
    if( colName != NULL && colName[0] != '\0' )
    {
        for (int i=0; mComputedProperties && i<mComputedProperties->GetCount(); i++)
        {
            FdoPtr<FdoComputedIdentifier> id = static_cast<FdoComputedIdentifier*>(mComputedProperties->GetItem(i));
            if( strcmp(GetDbAliasName(id->GetName()),  mColList[colIdx].c_alias) == 0 )
                return id->GetName();
        }
    }
    else
    {
        return mConnection->GetSchemaUtil()->ColName2Property(mClassDefinition->GetQName(), mConnection->GetUtility()->Utf8ToUnicode(mColList[colIdx].column) );
    }

    throw FdoCommandException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_73_PROPERTY_INDEXOUTOFBOUNDS), index));
    return NULL; // to suppress compiler warning
}

FdoInt32 FdoRdbmsFeatureReader::GetPropertyIndexForDataReader(FdoString* propertyName)
{
    // Initializes the column description if is the case
    if( mColCount == -1 )
        (void)GetPropertyCount(); 

    int  colIdx = 0;

    // try and see if it's a calculation
    const char *colName = FdoRdbmsFeatureReader::GetDbAliasName( propertyName, NULL );
    if (colName == NULL)
    {
        FdoPropertyType proptype;
        colName = PROPERTY2COLNAME( propertyName, &proptype );
        if (colName != NULL && *colName != '\0')
        {
            int i;

            // Get the column name
            for(i=(int)strlen(colName)-1; i>=0 && colName[i] != '.'; i--)
                ;
            if( i >=0 )
                colName = &colName[i+1];

            for ( int k=0; k<mColCount; k++ )
            {
                if (SkipColumnForProperty(k))
                    continue;

                if( FdoCommonOSUtil::stricmp(colName, mColList[k].column) == 0 )
                    return colIdx;

                colIdx++;
            }
        }
    }
    else
    {
        // It must be a computed identifier
        colName = GetDbAliasName(propertyName);
        for ( int k=0; k<mColCount; k++ )
        {
            if (SkipColumnForProperty(k))
                continue;

            if( strcmp(colName, mColList[k].c_alias) == 0 )
                return colIdx;

            colIdx++;
        }
    }

    throw FdoCommandException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_74_PROPERTY_NAME_NOT_FOUND), propertyName));
    return -1; // to suppress compiler warning
}

// This is an internal method to support the DataReader
FdoDataType FdoRdbmsFeatureReader::GetDataType(FdoString* propertyName)
{
    // Initializes the column description if is the case
    if( mColCount == -1 )
        (void)GetPropertyCount(); 

    // try and see if it's a calculation
    const char *colName = FdoRdbmsFeatureReader::GetDbAliasName( propertyName, NULL );
    if (colName == NULL)
    {
        FdoPropertyType proptype;
        colName = PROPERTY2COLNAME( propertyName, &proptype );
        if (colName != NULL && *colName != '\0')
        {
            int i;

            // Get the column name
            for(i=(int)strlen(colName)-1; i>=0 && colName[i] != '.'; i--)
                ;
            if( i >=0 )
                colName = &colName[i+1];

            // It must be a data property
            if( proptype != FdoPropertyType_DataProperty )
                return (FdoDataType)0;

            for ( int k=0; k<mColCount; k++ )
                if( FdoCommonOSUtil::stricmp(colName, mColList[k].column) == 0 )
                    return FdoRdbmsUtil::DbiToFdoType( mColList[k].datatype );
        }
    }
    else
    {
        // It must be a computed identifier
        colName = GetDbAliasName(propertyName);
        for ( int k=0; k<mColCount; k++ )
            if( strcmp(colName, mColList[k].c_alias) == 0 )
                return FdoRdbmsUtil::DbiToFdoType( mColList[k].datatype );
    }

    ThrowPropertyNotFoundExp( propertyName );
    return (FdoDataType)0; // not reached but supresses the compiler warning
}

FdoPropertyType FdoRdbmsFeatureReader::GetPropertyType(FdoString* propertyName)
{
    FdoPropertyType propType;
    bool foundPropType = false;
    FdoStringP className = mClassDefinition->GetQName();
    FdoRdbmsSchemaUtil * schemaUtil = mConnection->GetSchemaUtil();
    const FdoSmLpClassDefinition * classDefinition = schemaUtil->GetClass(className);
    if (NULL != classDefinition)
    {
        const FdoSmLpPropertyDefinitionCollection * propDefs = classDefinition->RefProperties();
        if (NULL != propDefs)
        {
            const FdoSmLpPropertyDefinition * propDef = propDefs->RefItem(propertyName);
            if (NULL != propDef)
            {
                propType = propDef->GetPropertyType();
                foundPropType = true;
            }
        }
    }

    if (!foundPropType)
    {
        if( mColCount == -1 )
            (void)GetPropertyCount(); // Initializes the column description

        // It must be a computed identifier
        const char * colName = GetDbAliasName(propertyName);
        for ( int k=0; k<mColCount; k++ )
        {
            if( NULL != colName && strcmp(colName, mColList[k].c_alias) == 0 )
            {
                if( mColList[k].datatype == RDBI_GEOMETRY )
                {
                    propType = FdoPropertyType_GeometricProperty;
                    foundPropType = true;
                }
                else
                {
                    propType = FdoPropertyType_DataProperty;
                    foundPropType = true;
                }
            }
        }
    }

    if (!foundPropType)
        ThrowPropertyNotFoundExp( propertyName );

    return propType;
}

void  FdoRdbmsFeatureReader::FetchProperties ()
{
    bool                res = false;
    static wchar_t      *gql_query = L"select * from %ls where %ls = :1";
    ColDef              *mColList = NULL;

    if( mPropertiesFetched )
        return;

    if( mConnection == NULL )
        throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_13, "Connection not established"));

    mAttrsQidIdx = GetAttributeQuery( mCurrentClassName );
    try
    {
        if( mAttrQueryCache[mAttrsQidIdx].query == NULL )
        {
            const FdoSmLpSchema *schema = mConnection->GetSchemaUtil()->GetSchema(mCurrentClassName);
            const FdoSmLpClassDefinition *classDefinition = schema->RefClasses()->RefItem(mCurrentClassName);
            FdoStringP tableName = mConnection->GetSchemaUtil()->GetDbObjectSqlName(classDefinition);

            const FdoSmLpPropertyDefinitionCollection *propertyDefinitions = classDefinition->RefProperties();

            mColList = new ColDef[propertyDefinitions->GetCount()];


            for (int i=0; i<propertyDefinitions->GetCount(); i++)
            {
                const FdoSmLpPropertyDefinition *propertyDefinition = propertyDefinitions->RefItem(i);

                if (propertyDefinition->GetPropertyType() == FdoPropertyType_DataProperty)
                {
                    const FdoSmLpDataPropertyDefinition* dataProp =
                        static_cast<const FdoSmLpDataPropertyDefinition*>(propertyDefinition);
                    const FdoSmPhColumn *column = dataProp->RefColumn();
                    FdoDataType dataType =dataProp->GetDataType();
                    int dbiDataType = FdoRdbmsUtil::FdoToDbiType(dataType);
                    const wchar_t *colName = column->GetName();
                    const char *colNameString = mConnection->GetUtility()->UnicodeToUtf8(colName);
                    strcpy( mColList[i].col_name, colNameString);
                    mColList[i].type = dbiDataType;
                    mColList[i].size = column->GetLength();
                }
                else {
                    mColList[i].col_name[0] = '\0';
                    mColList[i].type = 0;
                    mColList[i].size = 0;
                }
            }
            mAttrQueryCache[mAttrsQidIdx].mColCount = propertyDefinitions->GetCount();
            mAttrQueryCache[mAttrsQidIdx].mColList = mColList;

            //const char* tableNameString = mConnection->GetUtility()->UnicodeToUtf8(tableName);

            /* TODO: Abstract class query can no longer be done since feature table no longer supported.
             * However, we might someday implement class table mapping in which case this type of query
             * would be possible again. This would mean rewriting the following to bind in all identity
             * properties.
            mAttrQueryCache[mAttrsQidIdx].statement = mConnection->GetGdbiConnection()->Prepare( (const wchar_t*)FdoStringP::Format(gql_query,(FdoString *)tableName, primKey) );
            mAttrQueryCache[mAttrsQidIdx].statement->Bind( 1, &mFeatNum, NULL);
            */
        }

        mAttrQueryCache[mAttrsQidIdx].query = mAttrQueryCache[mAttrsQidIdx].statement->ExecuteQuery();

        if( mAttrQueryCache[mAttrsQidIdx].query->ReadNext() == RDBI_END_OF_FETCH )
        {
            mAttrQueryCache[mAttrsQidIdx].query->Close();
            delete mAttrQueryCache[mAttrsQidIdx].query;
            mAttrQueryCache[mAttrsQidIdx].query = NULL;

            if( mAttrQueryCache[mAttrsQidIdx].statement )
            {
                delete mAttrQueryCache[mAttrsQidIdx].statement;
                mAttrQueryCache[mAttrsQidIdx].statement = NULL;
            }
            delete[] mAttrQueryCache[mAttrsQidIdx].mColList;
            mAttrQueryCache[mAttrsQidIdx].mColList = NULL;
        }
    }
    catch (FdoCommandException *ex)
    {
        ex;
        FEATUREREADER_CLEANUP;
        throw;
    }

    catch (FdoException *ex)
    {
        FEATUREREADER_CLEANUP;
        throw FdoCommandException::Create(ex->GetExceptionMessage(), ex);
    }

    catch( ... )
    {
        FEATUREREADER_CLEANUP;
        throw;
    }

    mPropertiesFetched = true;
}

void FdoRdbmsFeatureReader::AddToList(FdoPropertyDefinitionCollection *propertyDefinitions, FdoPropertyDefinition *propertyDefinition)
{
    int i;
    for (i=0; i<propertyDefinitions->GetCount(); i++)
    {
        FdoPtr<FdoPropertyDefinition> item = propertyDefinitions->GetItem(i);
        if (wcscmp(propertyDefinition->GetName(), item->GetName()) == 0)
            break;
    }

    if (i == propertyDefinitions->GetCount())
        propertyDefinitions->Add(propertyDefinition);
}

FdoClassDefinition *FdoRdbmsFeatureReader::GetClassDefinition()
{
    FdoClassDefinitionP classDef;
    FdoClassDefinition *returnClassDef = NULL;
    if( mSchemaCollection == NULL )
    {
        FdoRdbmsDescribeSchemaCommand*  pDescSchemaCmd = new FdoRdbmsDescribeSchemaCommand( mConnection );
        pDescSchemaCmd->SetSchemaName(mClassDefinition->RefLogicalPhysicalSchema()->GetName());
        FdoStringsP classNames = FdoStringCollection::Create();
        classNames->Add(mCurrentClassName);
        pDescSchemaCmd->SetClassNames(classNames);
        mSchemaCollection = pDescSchemaCmd->Execute();
        pDescSchemaCmd->Release();
    }

    if (mFdoClassDefinition != NULL)
    {
        mFdoClassDefinition->AddRef();
        return mFdoClassDefinition;
    }

    const FdoSmLpClassDefinition* pClass = mClassDefinition;
    FdoPtr<FdoIdentifier> className = FdoIdentifier::Create( mCurrentClassName );
    int leng;
    if( className->GetScope(leng) != NULL && leng != 0 && mClassDefinition->GetParent() && mClassDefinition->GetParent()->GetParent() )
    {
        const FdoSmLpObjectPropertyDefinition* objProp = static_cast<const FdoSmLpObjectPropertyDefinition*>(mClassDefinition->GetParent()->GetParent());
        pClass = objProp->RefClass();
    }

    FdoPtr<FdoFeatureSchema> schm = mSchemaCollection->FindItem( pClass->RefLogicalPhysicalSchema()->GetName() );
    if( schm )
    {
        FdoPtr<FdoClassCollection> classes = schm->GetClasses();
        classDef = classes->FindItem( pClass->GetName() );
    }

    if( classDef )
    {
        returnClassDef = FilterClassDefinition( classDef );
        mFdoClassDefinition = returnClassDef;
        mFdoClassDefinition->AddRef();
    }

    return returnClassDef;
}

FdoClassDefinition *FdoRdbmsFeatureReader::FilterClassDefinition(
    FdoClassDefinition* classDef,
    bool isBaseClass
)
{
    FdoClassDefinition *returnClassDef = NULL;
    bool isComputed = false;

    if( mColCount == -1 )
        (void)GetPropertyCount(); // Initializes the column description

    if (mProperties && mProperties->GetCount() > 0)
    {
        FdoClassDefinitionP nextBaseClass = classDef->GetBaseClass();

        FdoPtr<FdoReadOnlyPropertyDefinitionCollection> baseProperties = classDef->GetBaseProperties();
        FdoPtr<FdoPropertyDefinitionCollection> subsetBaseProperties = FdoPropertyDefinitionCollection::Create(NULL);

        FdoPtr<FdoPropertyDefinitionCollection> properties = classDef->GetProperties();
        FdoPtr<FdoPropertyDefinitionCollection> subsetProperties = FdoPropertyDefinitionCollection::Create(NULL);

        FdoPtr<FdoDataPropertyDefinitionCollection> idProperties = classDef->GetIdentityProperties();
        FdoPtr<FdoDataPropertyDefinitionCollection> subsetIdProperties = FdoDataPropertyDefinitionCollection::Create(NULL);

        FdoPtr<FdoIdentifier> id;
        for (int i=0; i<mProperties->GetCount(); i++)
        {
            id = mProperties->GetItem(i);
            FdoStringsP strings = FdoStringCollection::Create(id->GetText(), L".");
            if (mLevel >= strings->GetCount())
                continue;
            const wchar_t* selectedName = strings->GetString(mLevel);

            FdoPtr<FdoDataPropertyDefinition> dataProperty;
            int j;
            for (j=0; j<idProperties->GetCount(); j++)
            {
                dataProperty = idProperties->GetItem(j);
                if (wcscmp(selectedName, dataProperty->GetName()) == 0)
                {
                    idProperties->RemoveAt(j);
                    properties->Remove(dataProperty);
                    subsetIdProperties->Add(dataProperty);
                    break;
                }
            }
            if( j != idProperties->GetCount() )
                continue;

            FdoPtr<FdoPropertyDefinition> property;

            // Skip base properties if class has base class.
            // They will be handled when base class is filtered.
            if ( !nextBaseClass ) 
            {
                for (j=0; j<baseProperties->GetCount(); j++)
                {
                    property = static_cast<FdoPropertyDefinition *> (baseProperties->GetItem(j));
                    if (wcscmp(selectedName, property->GetName()) == 0)
                    {
                        AddToList(subsetBaseProperties, property);
                        break;
                    }
                }
                if( j != baseProperties->GetCount() )
                    continue;
            }

            for (j=0; j<properties->GetCount(); j++)
            {
                property = static_cast<FdoPropertyDefinition *> (properties->GetItem(j));
                if (wcscmp(selectedName, property->GetName()) == 0)
                {
                    properties->RemoveAt(j);
                    subsetProperties->Add(property);
                    break;
                }
            }
            if( j != properties->GetCount() )
                continue;

            // See if we have any Computed Identifier. If so, we need to mark this class as Computed class and
            // create and add property for each computed identifier.
            if( dynamic_cast<FdoComputedIdentifier *>( id.p ) != NULL )
            {
                const char* computedIdentifierName = mConnection->GetSchemaUtil()->MakeDBValidName( id->GetText() );
                for ( int k=0; k<mColCount; k++ )
                {
                    if( strcmp(mColList[k].c_alias, computedIdentifierName) == 0 )
                    {
                        isComputed = true;
                        // Property was added for child class so no need
                        // to add computed properties to base classes.
                        if ( !isBaseClass ) {
                            FdoComputedIdentifier* compIdent = static_cast<FdoComputedIdentifier *>( id.p );
				            FdoPtr<FdoExpression>   expr = compIdent->GetExpression();
                            FdoPropertyType propType;
                            FdoDataType dataType;
                            GetExpressionType(mFdoConnection, classDef, mColList[k].c_alias, expr, propType, dataType);

                            if (propType == FdoPropertyType_GeometricProperty)
                            {
                                FdoPtr<FdoGeometricPropertyDefinition> pProp = FdoGeometricPropertyDefinition::Create( id->GetText(), L"Computed Property" );
                                subsetProperties->Add( pProp );
                            }
                            else
                            {
                                FdoPtr<FdoDataPropertyDefinition>pProp = FdoDataPropertyDefinition::Create( id->GetText(), L"Computed Property" );
                                pProp->SetDataType( dataType );
                                subsetProperties->Add( pProp );
                            }
                        }
                        break;
                    }
                }
            }
        }
        if (mIsFeatureQuery)
        {
            FdoFeatureClass *featureClass = FdoFeatureClass::Create(classDef->GetName(), classDef->GetDescription());
#if 0
            // The following is currently not needed because if the geometry is selected, all the properties the properties will be returned and this will never be called.
            FdoPtr<FdoGeometricPropertyDefinition> geoProperty = featureClass->GetGeometryProperty();
            for (int j=0; j<mProperties->GetCount(); j++)
            {
                id = mProperties->GetItem(j);
                if( geoProperty != NULL && wcscmp( geoProperty->GetName(), id->GetText()) == 0 )
                {
                    FdoFeatureClass *featureClass = static_cast<FdoFeatureClass *>(classDef);
                    featureClass->SetGeometryProperty(FdoPtr<FdoGeometricPropertyDefinition>(featureClass->GetGeometryProperty()));
                    break;
                }
            }
#endif
            returnClassDef = featureClass;
        }
        else
        {
            FdoClass *clas = FdoClass::Create(classDef->GetName(), classDef->GetDescription());
            returnClassDef = clas;
        }

        if ( nextBaseClass ) 
            returnClassDef->SetBaseClass(FdoPtr<FdoClassDefinition>(FilterClassDefinition(nextBaseClass,true)));
        
        returnClassDef->SetBaseProperties(subsetBaseProperties);
        returnClassDef->SetIsAbstract(classDef->GetIsAbstract());
        if( isComputed )
            returnClassDef->SetIsComputed( true );
        properties = returnClassDef->GetProperties();
        for (int i=0; i<subsetProperties->GetCount(); i++)
        {
            FdoPtr<FdoPropertyDefinition> item = subsetProperties->GetItem(i);
            properties->Add(item);
        }
        FdoPtr<FdoDataPropertyDefinitionCollection> retIdProperties = returnClassDef->GetIdentityProperties();
        for (int i=0; i<subsetIdProperties->GetCount(); i++)
        {
            FdoPtr<FdoDataPropertyDefinition> item = subsetIdProperties->GetItem(i);
            if (!properties->Contains(item))
                properties->Add(item);
            retIdProperties->Add(item);        }
    }
    else
    {
        returnClassDef = FDO_SAFE_ADDREF(classDef);
    }

    return returnClassDef;
}

int FdoRdbmsFeatureReader::GetDepth()
{
    return mLevel;
}


FdoIFeatureReader* FdoRdbmsFeatureReader::GetAssociatedObject( const FdoSmLpAssociationPropertyDefinition *propertyDefinition )
{
    const FdoSmLpClassDefinition *associatedClass = propertyDefinition->RefAssociatedClass();

    // TODO:
    // 1- Handle the case of restricted set of properties; when the user ask for specific properties as opposed to all properties.
    // 2- Remove the constrain on  the level. This is mostly done when building the select statement by drilling down the tree of
    //    value object properties untill we find association. At the same time we should handle value type object property the same
    //    way by creating a single select.

    if( mLevel > 0 || ! FdoPtr<FdoRdbmsFilterProcessor>(mFdoConnection->GetFilterProcessor())->CanOptimizeRelationQuery( mClassDefinition,  propertyDefinition) )
    {
        GdbiQueryResult *queryResult;
        FdoStringP selectString = FdoStringP::Format(L"select * from %ls where ", (FdoString*)(mConnection->GetSchemaUtil()->GetDbObjectSqlName(associatedClass)));
        const FdoSmPhColumnListP identCols = propertyDefinition->GetIdentityColumns();
        const FdoSmPhColumnListP revIdentCols = propertyDefinition->GetReverseIdentityColumns();
        char    **bindArray = new char*[identCols->GetCount()];
        for( int i=0; i<identCols->GetCount(); i++ )
        {
            if( i != 0 )
                selectString += L" and ";
            selectString += FdoStringP::Format(L"%ls=",(const wchar_t *)(identCols->GetDbString(i)));
			selectString += mFdoConnection->GetBindString( i+1 );
        }

        GdbiStatement *statement = mConnection->GetGdbiConnection()->Prepare( (const wchar_t *) selectString );
        for( int i=0; i<identCols->GetCount(); i++ )
        {
            FdoStringP colName = FdoStringP::Format(L"%ls.%ls",(const wchar_t *)mClassDefinition->GetDbObjectName(), (const wchar_t *)revIdentCols->GetString(i) );
			FdoString *strval = mAttrQueryCache[mAttrsQidIdx].query->GetString((const wchar_t*)colName, NULL, NULL );
			if( ! mConnection->GetGdbiCommands()->SupportsUnicode() )
			{
				FdoStringP  gisStr(strval);
				const char* tmpStr = (const char*)gisStr;
				bindArray[i] = new char[strlen(tmpStr) + 1];
				strcpy( bindArray[i], tmpStr );
				statement->Bind( i+1, (int)strlen(bindArray[i])+1, (char *) bindArray[i] );
			}
			else
			{
				bindArray[i] = new char[wcslen(strval)*sizeof(wchar_t) + sizeof(wchar_t)];
				wcscpy( (wchar_t*)bindArray[i], strval );
				statement->Bind( i+1, (int)wcslen(strval)+1, (wchar_t *) bindArray[i] );
			}
        }
       queryResult = statement->ExecuteQuery();

       delete statement;

        for( int i=0; i<identCols->GetCount(); i++ )
            delete[] bindArray[i];

        delete[] bindArray;

        if( mSchemaCollection )
            mSchemaCollection->AddRef();

        return new FdoRdbmsFeatureReader( mFdoConnection, queryResult, (associatedClass->GetClassType() == FdoClassType_FeatureClass), associatedClass, mSchemaCollection, mProperties, mLevel+1 );
    }

    // Otherwise the same query is used to return the columns of the associated object. The original select would have added
    // the associated class column to the column list.
    return new FdoRdbmsObjectFeatureReader( mFdoConnection, mAttrQueryCache[mAttrsQidIdx].query, false, associatedClass, mSchemaCollection, mProperties, mLevel+1 );

    return NULL;
}

FdoIFeatureReader* FdoRdbmsFeatureReader::GetFeatureObject( const wchar_t* propertyName )
{
    GdbiQueryResult*         query = NULL;

     if( ! mHasMoreFeatures )
        throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_17, "End of feature data or NextFeature not called"));

    const FdoSmLpPropertyDefinition *propertyDefinition = mClassDefinition->RefProperties()->RefItem( propertyName );
    if( propertyDefinition == NULL )
        throw FdoCommandException::Create(NlsMsgGet2( FDORDBMS_59, "Property '%1$ls' not defined for class '%2$ls'", propertyName, mLastClassName));

    if( propertyDefinition->GetPropertyType()  ==  FdoPropertyType_AssociationProperty )
        return GetAssociatedObject( (const FdoSmLpAssociationPropertyDefinition *)propertyDefinition );

    if( propertyDefinition->GetPropertyType()  !=  FdoPropertyType_ObjectProperty )
        throw FdoCommandException::Create( NlsMsgGet1( FDORDBMS_65, "Property '%1$ls' is not an object or association property type", propertyName ));

    // At this point we need to:
    // 1- Get the target class
    // 2- Find the primary pKey/fKey mapping from the current class to the target class
    // 3- Find the value of the pKeys from the current class
    // 4- Create a SQL select of the form: select * from <class table> where <class table>.fKey1 = <current object pKey1 value> and <class table>.fKey2 = <current object pKey2 value> and ...
    // 5- And return the reader for that query
    const FdoSmLpObjectPropertyDefinition* objProp = static_cast<const FdoSmLpObjectPropertyDefinition*>(propertyDefinition);
    const FdoSmLpClassDefinition* pTargetClass = objProp->RefTargetClass();
    const FdoSmLpClassDefinition* pPropertyClass = objProp->RefClass();
    if ( pTargetClass && pPropertyClass )
    {
        const FdoSmPhColumnCollection* pkCols = NULL;
        const FdoSmPhColumnCollection* fkCols = NULL;
        const FdoSmLpDbObject* pTargetTable = pTargetClass->RefDbObject();

        if ( pTargetTable != NULL )
        {
            pkCols = pTargetTable->RefTargetColumns();
            fkCols = pTargetTable->RefSourceColumns();
        }
        if( pkCols == NULL || fkCols == NULL || pkCols->GetCount() == 0 || pkCols->GetCount() != fkCols->GetCount() )
            throw FdoFilterException::Create( NlsMsgGet2( FDORDBMS_66, "Missing or badly defined target class for property '%1$ls' of class '%2$ls'", propertyName, mLastClassName));

        FdoStringP targetTableName = pTargetTable->RefDbObject()->GetDbQName();

        FdoStringP selectString;
		FdoStringP  ltWhereCondition;
		FdoStringP  ltTableExp;
		FdoPtr<FdoRdbmsFilterProcessor>flterProcessor = mFdoConnection->GetFilterProcessor();
        if (mProperties && mProperties->GetCount() > 0)
        {
            FdoStringP str;

            bool first = true;
            const FdoSmLpClassDefinition *currentClass;
            for (int i=0; i<mProperties->GetCount(); i++)
            {

                currentClass = mConnection->GetSchemaUtil()->GetClass(mCurrentClassName);
                FdoPtr<FdoIdentifier> property = mProperties->GetItem(i);
                if (wcschr(property->GetText(), L'.'))
                {
                    FdoStringsP strings = FdoStringCollection::Create(property->GetText(), L".");

                    for (int j=mLevel;j<strings->GetCount()-1; j++)
                    {
                        const wchar_t* objPropName = strings->GetString(j);
                        propertyDefinition = currentClass->RefProperties()->RefItem(objPropName);
                        if (propertyDefinition == NULL)
                            break;
                        const FdoSmLpObjectPropertyDefinition* objProp = static_cast<const FdoSmLpObjectPropertyDefinition*>(propertyDefinition);
                        const FdoSmLpPropertyMappingDefinition* mappping = objProp->RefMappingDefinition();
                        if( mappping->GetType() !=  FdoSmLpPropertyMappingType_Concrete )
                            throw FdoSchemaException::Create(NlsMsgGet(FDORDBMS_27, "Unsupported Property mapping type"));

                        const FdoSmLpPropertyMappingConcrete * concMapping = static_cast<const FdoSmLpPropertyMappingConcrete*>( mappping );
                        currentClass = (FdoSmLpClassDefinition*)concMapping->RefTargetClass();

                    }
                    if (currentClass && (wcscmp(currentClass->GetName(), pTargetClass->GetName()) == 0))
                    {
                        propertyDefinition = currentClass->RefProperties()->RefItem(strings->GetString(strings->GetCount()-1));

                        if (propertyDefinition->GetPropertyType() == FdoPropertyType_DataProperty)
                        {
                            const FdoSmLpDataPropertyDefinition* dataProp =
                                static_cast<const FdoSmLpDataPropertyDefinition*>(propertyDefinition);
                            FdoStringP colName = mConnection->GetSchemaUtil()->GetColumnSqlName(dataProp);

                            if (first == false)
                                str += ",";
							str += targetTableName;
							str += ".";
                            str += colName;
                            first = false;
                        }
                    }
                }
            }
            const FdoSmLpDataPropertyDefinitionCollection *properties = pTargetClass->RefIdentityProperties();
            for (int i=0; i<properties->GetCount(); i++)
            {
                const FdoSmLpDataPropertyDefinition *idProperty = properties->RefItem(i);
                const FdoSmLpDataPropertyDefinition* dataProp =
                    static_cast<const FdoSmLpDataPropertyDefinition*>(idProperty);
                FdoStringP colName = mConnection->GetSchemaUtil()->GetColumnSqlName(idProperty);
                if (first == false)
                    str += ",";
				str += targetTableName;
				str += ".";
                str += colName;
                first = false;

            }
			const wchar_t* comma = L"";
			flterProcessor->GetLtTableExpression( pTargetClass, ltWhereCondition, ltTableExp, FdoCommandType_Select );
			if( ((const wchar_t*)ltWhereCondition)[0] != '\0' )
				comma = L",";
            selectString = FdoStringP::Format(L"select %ls from %ls%ls%ls where ", (const wchar_t *) str, (const wchar_t *) targetTableName,comma,(const wchar_t*)(ltTableExp));

        }
        else
        {
			const wchar_t* comma = L"";
			flterProcessor->GetLtTableExpression( pTargetClass, ltWhereCondition, ltTableExp, FdoCommandType_Select );
			if( ((const wchar_t*)ltWhereCondition)[0] != '\0' )
				comma = L",";
            selectString = FdoStringP::Format(L"select * from %ls%ls%ls where ", (const wchar_t *)targetTableName,comma,(const wchar_t*)(ltTableExp));
        }

        char    **bindArray = new char*[pkCols->GetCount()];

        for( int i=0; i<pkCols->GetCount(); i++ )
        {
            if( i != 0 )
                selectString += L" and ";
            FdoStringP colName = FdoStringP::Format(L"%ls.%ls",(const wchar_t *)mClassDefinition->GetDbObjectName(), pkCols->RefItem(i)->GetName() );
            const wchar_t* tmpVal = mAttrQueryCache[mAttrsQidIdx].query->GetString( (const wchar_t *)colName,NULL, NULL);
            selectString += targetTableName;
			selectString += L".";
			selectString += fkCols->RefItem(i)->GetName();
            selectString += L" = ";
			selectString += mFdoConnection->GetBindString( i+1 );

            if ( mConnection->GetGdbiCommands()->SupportsUnicode() ) {
			    size_t size = wcslen(tmpVal) + 1;
                bindArray[i] = (char*)(new wchar_t[size]);
			    wcsncpy( (wchar_t*) bindArray[i], (const wchar_t*)FdoStringP( tmpVal, true), size );
			    ((wchar_t*)(bindArray[i]))[size-1]='\0'; 
            }
            else {
			    size_t size = wcslen(tmpVal)*3 + 1;
                bindArray[i] = new char[size];
			    strncpy( bindArray[i], (const char*)FdoStringP( tmpVal, true), size );
			    bindArray[i][size-1]='\0'; 
            }
        }
		if( ((const wchar_t*)ltWhereCondition)[0] != '\0' )
		{
			selectString += L" and ";
			selectString += ltWhereCondition;
		}
        if (objProp->GetObjectType() == FdoObjectType_OrderedCollection)
        {
            const FdoSmLpDataPropertyDefinition * id = objProp->RefIdentityProperty();
            if (id)
            {
                const FdoSmPhColumn* column = id->RefColumn();
                FdoStringP columnName = column ? column->GetDbName() : FdoStringP();
                if (columnName != L"")
                {
                    selectString += " order by ";
					selectString += targetTableName;
					selectString += L".";
                    selectString += columnName;
                    selectString += " ";
                    if (objProp->GetOrderType() == FdoOrderType_Descending)
                    {
                        selectString += "desc ";
                    }
                }
            }
        }
        GdbiStatement *statement = mConnection->GetGdbiConnection()->Prepare( (const wchar_t *) selectString );
        for( int i=0; i<pkCols->GetCount(); i++ )
        {
            if ( mConnection->GetGdbiCommands()->SupportsUnicode() )
                statement->Bind( i+1, (int)wcslen((wchar_t*)bindArray[i])+1, (wchar_t *) bindArray[i]);
            else
                statement->Bind( i+1, (int)strlen(bindArray[i])+1, (char *) bindArray[i]);
        }
        query = statement->ExecuteQuery();
        for( int i=0; i<pkCols->GetCount(); i++ )
            delete[] bindArray[i];

        delete statement;
        delete[] bindArray;

        if( mSchemaCollection )
            mSchemaCollection->AddRef();

        return new FdoRdbmsFeatureReader( mFdoConnection, query, false, pTargetClass, mSchemaCollection, mProperties, mLevel+1 );
    }

    return NULL;
}

bool FdoRdbmsFeatureReader::GetBoolean( const wchar_t *propertyName )
{
    GET_ATTRIBUTE( mAttrQueryCache[mAttrsQidIdx].query->GetBoolean, bool );
    return value;
}

FdoByte FdoRdbmsFeatureReader::GetByte( const wchar_t *propertyName )
{
    return (FdoByte)GetInt16( propertyName );
}


FdoDateTime FdoRdbmsFeatureReader::GetDateTime( const wchar_t *propertyName )
{
    return mFdoConnection->DbiToFdoTime( mConnection->GetUtility()->UnicodeToUtf8( GetString( propertyName ) ) );
}


double FdoRdbmsFeatureReader::GetDouble( const wchar_t *propertyName )
{
    GET_ATTRIBUTE( mAttrQueryCache[mAttrsQidIdx].query->GetDouble, double );
    return value;
}

const wchar_t* FdoRdbmsFeatureReader::GetString( const wchar_t *propertyName )
{
    bool isNull;
    wchar_t *propertyValue = NULL;

    if( ! mHasMoreFeatures )
        throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_17, "End of feature data or NextFeature not called"));

    // Get the next row if it's not already done.
    FetchProperties( );

    if( mAttrQueryCache[mAttrsQidIdx].query == NULL )
        throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_18, "End of feature data"));

    try
    {
        FdoPropertyType type;
		int				cacheIndex;
        const	char	*colName = PROPERTY2COLNAME_IDX( propertyName, &type, NULL, &cacheIndex );

        if ((colName == NULL) || (strlen(colName) == 0))
        {
			if( type != FdoPropertyType_DataProperty )
				throw FdoCommandException::Create(NlsMsgGet1( FDORDBMS_67, strObjPropetryExp, propertyName ));

			// The catch block will create the message string
			throw "";
		}

        const wchar_t* value = mAttrQueryCache[mAttrsQidIdx].query->GetString( GetPropertyInfoDef(cacheIndex)->columnPosition, &isNull, NULL );
        if( isNull )
            throw FdoCommandException::Create(NlsMsgGet1( FDORDBMS_385, strNUllPropetryExp,  propertyName ));

		propertyValue = mStringMap.AddtoMap( (char *)colName, value, mConnection->GetUtility() );

    }
    catch ( FdoCommandException* exc )
    {
        // Try to throw a better exception, otherwise re-throw the dbi exception
        // Note that we only do this when something goes wrong for performance reason; we don't want all that agly
        // string compares to happen every time.
        ThrowPropertyNotFoundExp( propertyName, exc );

        throw;
    }
    catch ( FdoException* exc)
    {
        // Try to throw a better exception, otherwise re-throw the dbi exception
        // Note that we only do this when something goes wrong for performance reason; we don't want all that agly
        // string compares to happen every time.
        ThrowPropertyNotFoundExp( propertyName, exc );

        throw;
    }
    catch ( ... )
    {
        // Try to throw a better exception, otherwise re-throw the dbi exception
        // Note that we only do this when something goes wrong for performance reason; we don't want all that agly
        // string compares to happen every time.
        ThrowPropertyNotFoundExp( propertyName );

        throw;
    }

    return propertyValue;
}

short FdoRdbmsFeatureReader::GetInt16( const wchar_t *propertyName )
{
    GET_ATTRIBUTE( mAttrQueryCache[mAttrsQidIdx].query->GetInt16, FdoInt16 );
    return value;
}

int FdoRdbmsFeatureReader::GetInt32( const wchar_t *propertyName )
{
    GET_ATTRIBUTE( mAttrQueryCache[mAttrsQidIdx].query->GetInt32, FdoInt32 );
    return value;
}

FdoInt64 FdoRdbmsFeatureReader::GetInt64( const wchar_t *propertyName )
{
    GET_ATTRIBUTE( mAttrQueryCache[mAttrsQidIdx].query->GetInt64, FdoInt64 );
    return value;
}

float FdoRdbmsFeatureReader::GetSingle( const wchar_t *propertyName )
{
    GET_ATTRIBUTE( mAttrQueryCache[mAttrsQidIdx].query->GetFloat, FdoFloat );
    return value;
}

///////////////////////////////////////////////////////////////////////////////
FdoIStreamReader* FdoRdbmsFeatureReader::GetLOBStreamReader(const wchar_t* propertyName)
{
    bool isNull = false;
    FdoIStreamReader *lobReader = NULL;

    if( ! mHasMoreFeatures )
        throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_57, strEndOfRecordExp));

    //FetchProperties( );
    if( mAttrQueryCache[mAttrsQidIdx].query == NULL )
        throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_57, strEndOfRecordExp));

    try
    {
        const char *colName = PROPERTY2COLNAME( propertyName, NULL );
        if (colName == NULL)
        {
            throw "";
        }

        void *lobLocator = NULL;
        mAttrQueryCache[mAttrsQidIdx].query->GetBinaryValue( (const wchar_t*)FdoStringP( colName ),sizeof(void *),(char*)&lobLocator, &isNull,NULL);
        if( isNull )
            throw FdoCommandException::Create(NlsMsgGet1( FDORDBMS_385, strNUllPropetryExp, propertyName ));
        // Look up the LOB type based on the property name - assume FdoDataType_BLOB for now
        lobReader = FdoRdbmsBLOBStreamReader::Create( mFdoConnection, mAttrQueryCache[mAttrsQidIdx].query, lobLocator );
    }
    catch ( char * )
    {
        ThrowPropertyNotFoundExp( propertyName );
        throw;
    }

    return lobReader;
}

///////////////////////////////////////////////////////////////////////////////
FdoLOBValue* FdoRdbmsFeatureReader::GetLOB(const wchar_t* propertyName)
{
    FdoLOBValue  * pLobVal = NULL;

    if( ! mHasMoreFeatures )
        throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_57, strEndOfRecordExp));

    //FetchProperties( );
    if( mAttrQueryCache[mAttrsQidIdx].query == NULL )
        throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_57, strEndOfRecordExp));

    try
    {
        const char *colName = PROPERTY2COLNAME( propertyName, NULL );
        if (colName == NULL)
        {
            throw "";
        }

        void *lobLocator = NULL;
        bool isNull = false;
        mAttrQueryCache[mAttrsQidIdx].query->GetBinaryValue( (const wchar_t*)FdoStringP(colName),sizeof(void *),(char*)&lobLocator,&isNull,NULL);
        if( isNull )
            throw FdoCommandException::Create(NlsMsgGet1( FDORDBMS_385, strNUllPropetryExp, propertyName ));

        // Look up the LOB type based on the property name - assume FdoDataType_BLOB for now
        FdoRdbmsBLOBStreamReader *blobReader = FdoRdbmsBLOBStreamReader::Create( mFdoConnection, mAttrQueryCache[mAttrsQidIdx].query, lobLocator );

        FdoInt64 lob_size = blobReader->GetLength();
        FdoByteArray * byteArray = FdoByteArray::Create( (FdoInt32)lob_size );
        blobReader->ReadNext( byteArray, 0, (FdoInt32)lob_size );

        blobReader->Release();

        pLobVal = FdoBLOBValue::Create( byteArray );

    }
    catch ( char * )
    {
        ThrowPropertyNotFoundExp( propertyName );
        throw;
    }

    return pLobVal;
}

bool FdoRdbmsFeatureReader::IsNull( const wchar_t *propertyName )
{
    if( ! mHasMoreFeatures )
        throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_17, "End of feature data or NextFeature not called"));

    // Get the next row if it's not already done.
    FetchProperties( );

    if( mAttrQueryCache[mAttrsQidIdx].query == NULL )
        return true;

    try
    {
        FdoPropertyType type;
		int				cacheIndex;

		const wchar_t *colNameW = PROPERTY2COLNAME_IDX_W( propertyName, &type, NULL, &cacheIndex );

        switch( type )
        {
            case FdoPropertyType_DataProperty:
                if (colNameW != NULL)
                    return mAttrQueryCache[mAttrsQidIdx].query->GetIsNull( GetPropertyInfoDef(cacheIndex)->columnPosition );

                throw "";

            case FdoPropertyType_GeometricProperty:
                try
                {
					FdoPtr<FdoByteArray> ba = this->GetGeometry( propertyName, true);

					return ( ba == NULL );
                }
                catch(FdoCommandException *exp )
                {
                    exp->Release();
                    return true;
                }
            case FdoPropertyType_ObjectProperty:
            {

                const FdoSmLpPropertyDefinition *propertyDefinition = mClassDefinition->RefProperties()->RefItem( propertyName );
                if( propertyDefinition == NULL )
                    return true;
                const FdoSmLpObjectPropertyDefinition* objProp = static_cast<const FdoSmLpObjectPropertyDefinition*>(propertyDefinition);
                const FdoSmLpClassDefinition* pTargetClass = objProp->RefTargetClass();
                if( pTargetClass == NULL )
                    return true;
                const FdoSmLpDbObject* pTargetTable = pTargetClass->RefDbObject();
                if( pTargetTable == NULL )
                    return true;

                const FdoSmPhColumnCollection* pkCols = NULL;
                pkCols = pTargetTable->RefTargetColumns();
                if( pkCols == NULL || pkCols->GetCount() == 0 )
                    return true;
                for( int i=0; i<pkCols->GetCount(); i++ )
                {
                    FdoStringP colName = FdoStringP::Format(L"%ls.%ls",(const wchar_t *)mClassDefinition->GetDbObjectName(), pkCols->RefItem(i)->GetName() );
                    if( mAttrQueryCache[mAttrsQidIdx].query->GetIsNull( (const wchar_t *)colName ) )
                        return true;
                }
                //
                // If we get here, it's a good bet that this object property has associated object(s)
                return false;
            }

            case FdoPropertyType_AssociationProperty:

            {
                const FdoSmLpPropertyDefinition *propertyDefinition = mClassDefinition->RefProperties()->RefItem( propertyName );
                if( propertyDefinition == NULL )
                    return true;
                const FdoSmLpAssociationPropertyDefinition* associProp = static_cast<const FdoSmLpAssociationPropertyDefinition*>(propertyDefinition);
                const FdoSmPhColumnListP identCol = associProp->GetReverseIdentityColumns();
                for( int i=0; i<identCol->GetCount(); i++ )
                {
                    FdoStringP colName = FdoStringP::Format(L"%ls.%ls",(const wchar_t *)mClassDefinition->GetDbObjectName(), (const wchar_t *)identCol->GetString(i) );
                    if( mAttrQueryCache[mAttrsQidIdx].query->GetIsNull( (const wchar_t *)colName ) )
                        return true;
                }
                //
                // If we get here, it's a good bet that this association property has associated object(s)
                return false;
            }
            default:
            throw ""; // Jump to the exception handling block.
        }

    }
    catch ( FdoCommandException* exc )
    {
        // Try to throw a better exception, otherwise re-throw the dbi exception
        // Note that we only do this when something goes wrong for performance reason; we don't want all that agly
        // string compares to happen every time.
        ThrowPropertyNotFoundExp( propertyName, exc );

        throw;
    }
    catch ( FdoException* exc )
    {
        // Try to throw a better exception, otherwise re-throw the dbi exception
        // Note that we only do this when something goes wrong for performance reason; we don't want all that agly
        // string compares to happen every time.
        ThrowPropertyNotFoundExp( propertyName, exc );

        throw;
    }
    catch ( ... )
    {
        // Try to throw a better exception, otherwise re-throw the dbi exception
        // Note that we only do this when something goes wrong for performance reason; we don't want all that agly
        // string compares to happen every time.
        ThrowPropertyNotFoundExp( propertyName );

        throw;
    }

   // return ( ccode != DBI_SUCCESS );
}

void FdoRdbmsFeatureReader::ThrowPropertyNotFoundExp( const wchar_t* propertyName, FdoException* exc)
{
    // See if this property is one returned by out query, in that case we will throw an intelligent error
    int  idx;
    char *tmpString = (char*)mConnection->GetUtility()->UnicodeToUtf8(propertyName);

    if (mProperties && mProperties->GetCount() > 0)
    {
        for (idx=0; idx<mProperties->GetCount(); idx++)
        {
            FdoPtr<FdoIdentifier> property = mProperties->GetItem(idx);
            if (wcscmp(propertyName, property->GetName()) == 0)
            {
                break;
            }
        }
        if (idx == mProperties->GetCount())
        {
            if (exc)
                exc->Release();
            throw FdoCommandException::Create(NlsMsgGet1(FDORDBMS_261, "Property '%1$ls' not selected", propertyName));
        }
    }

    FdoPropertyType type;
    bool found;
    const char *colName = PROPERTY2COLNAME_IDX( propertyName, &type, &found, NULL );
    if( colName == NULL )
    {
        if (exc)
            exc->Release();
        if( ! found )
            throw FdoCommandException::Create(NlsMsgGet2(FDORDBMS_59, "Property '%1$ls' not defined for class '%2$ls'", propertyName, mLastClassName));
        else
            throw FdoCommandException::Create(NlsMsgGet2(FDORDBMS_406, "Property '%1$ls' from class '%2$ls' has no database mapping", propertyName, mLastClassName));
    }
}

FdoByteArray* FdoRdbmsFeatureReader::GetGeometry(const wchar_t* propertyName)
{
    return GetGeometry( propertyName, false );
}

FdoByteArray* FdoRdbmsFeatureReader::GetGeometry(const wchar_t* propertyName, bool checkIsNullOnly)
{
    return GetGeometry( propertyName, checkIsNullOnly, mAttrQueryCache[mAttrsQidIdx].query );
}

FdoByteArray* FdoRdbmsFeatureReader::GetGeometry(const wchar_t* propertyName, bool checkIsNullOnly, GdbiQueryResult *query)
{
    FdoPtr<FdoIGeometry> pgeom;
    FdoByteArray	*byteArray = NULL;
    bool            unsupportedTypeExp = false;

    if( ! mIsFeatureQuery )
        throw FdoCommandException::Create(NlsMsgGet( FDORDBMS_64, "Geometry property not supported on non FeatureClass objects"));

    const FdoSmLpFeatureClass*  feat = static_cast<const FdoSmLpFeatureClass *>( mClassDefinition );
    const FdoSmLpPropertyDefinitionCollection *propertyDefinitions = mClassDefinition->RefProperties();
    const FdoSmLpPropertyDefinition *pPropertyDefinition = propertyDefinitions->RefItem(propertyName);
    const FdoSmLpGeometricPropertyDefinition* pGeometricProperty =
                            dynamic_cast<const FdoSmLpGeometricPropertyDefinition*>(pPropertyDefinition);
    // Don't know why we need non-const to call some const methods -- VS8 compiler bug?
    FdoSmLpGeometricPropertyDefinition* pGeometricPropertyNonConst =
                            (FdoSmLpGeometricPropertyDefinition*)(pGeometricProperty);

    if( pGeometricProperty != NULL && wcscmp( propertyName, pGeometricProperty->GetName() ) != 0 )
        throw FdoCommandException::Create(NlsMsgGet2(FDORDBMS_59, "Property '%1$ls' not defined for class '%2$ls'", propertyName, mLastClassName));

    if( ! mHasMoreFeatures )
        throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_17, "End of feature data or NextFeature not called"));

    try
    {
        FdoSmOvGeometricColumnType columnType = pGeometricPropertyNonConst ? pGeometricPropertyNonConst->GetGeometricColumnType() : FdoSmOvGeometricColumnType_Default;
        FdoSmOvGeometricContentType contentType = pGeometricPropertyNonConst ? pGeometricPropertyNonConst->GetGeometricContentType() : FdoSmOvGeometricContentType_Default;
        const char *    colName = NULL;
		const wchar_t	*colNameW = NULL;
        const char *    colNameX = NULL;
        const char *    colNameY = NULL;
        const char *    colNameZ = NULL;

		int		cacheIndex;

        if (FdoSmOvGeometricColumnType_Double != columnType)
        {
            FdoPropertyType type;
            colNameW = PROPERTY2COLNAME_IDX_W( propertyName, &type, NULL, &cacheIndex );

			// The catch block will create the message string
			if (colNameW == NULL)
				throw "";
		}
        else
        {
            FdoString *colNameXW = pGeometricProperty->GetColumnNameX();
            FdoString *colNameYW = pGeometricProperty->GetColumnNameY();
            FdoString *colNameZW = pGeometricProperty->GetColumnNameZ();
            if (NULL == colNameXW || L'\0' == colNameXW[0] || NULL == colNameYW || L'\0' == colNameYW[0] )
            {
                throw FdoRdbmsException::Create(NlsMsgGet1(
                        FDORDBMS_468,
                        "No column for geometric property '%1$ls'.",
                        pPropertyDefinition->GetName()));
            }
            colNameX = mConnection->GetUtility()->UnicodeToUtf8(colNameXW);
            colNameY = mConnection->GetUtility()->UnicodeToUtf8(colNameYW);
            colNameZ =
                (colNameZW==NULL || L'\0' == colNameZW[0]) ?
                NULL :
                mConnection->GetUtility()->UnicodeToUtf8(colNameZW);
        }

        if ( ( FdoSmOvGeometricColumnType_Default == columnType &&
               FdoSmOvGeometricContentType_Default == contentType ) ||
             ( FdoSmOvGeometricColumnType_Default == columnType &&
               FdoSmOvGeometricContentType_Default == contentType ) )
        {
        	byteArray = mFdoConnection->GetGeometryValue( query, pGeometricProperty, GetPropertyInfoDef(cacheIndex)->columnPosition, checkIsNullOnly, unsupportedTypeExp );
        }
        else if ( FdoSmOvGeometricColumnType_Double == columnType &&
                  FdoSmOvGeometricContentType_Ordinates == contentType )
        {
            bool isNullX = false;
            bool isNullY = false;
            bool isNullZ = false;   // 'true' only if there is a Z column, with NULL value.
            double x = query->GetDouble( (const wchar_t *) FdoStringP(colNameX), &isNullX, NULL );
            double y = query->GetDouble( (const wchar_t *) FdoStringP(colNameY), &isNullY, NULL );
            double z = numeric_limits<double>::quiet_NaN();
            if (NULL == colNameZ)
                z = numeric_limits<double>::quiet_NaN();
            else
                z = query->GetDouble( (const wchar_t *) FdoStringP(colNameZ), &isNullZ, NULL );

            if (isNullX || isNullY || isNullZ)
            {
                if (!checkIsNullOnly)
                {
                    throw FdoCommandException::Create(NlsMsgGet1( FDORDBMS_385, strNUllPropetryExp, propertyName ));
                }
            }
            else
            {
                FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
                double ordinates[3] = { x, y, z };
                FdoInt32 dimensionality = FdoDimensionality_XY;
                if (NULL != colNameZ)
                    dimensionality |= FdoDimensionality_Z;

                pgeom = gf->CreatePoint(dimensionality, ordinates);
				byteArray = gf->GetFgf( pgeom );
            }
        }
    }
    catch ( FdoCommandException* exc )
    {
        if ( !unsupportedTypeExp)
        {
            // Try to throw a better exception, otherwise re-throw the dbi exception
            // Note that we only do this when something goes wrong for performance reason; we don't want all that agly
            // string compares to happen every time.
            ThrowPropertyNotFoundExp( propertyName, exc );
        }
        throw;
    }
    catch ( FdoException* exc )
    {
        if ( !unsupportedTypeExp)
        {
            // Try to throw a better exception, otherwise re-throw the dbi exception
            // Note that we only do this when something goes wrong for performance reason; we don't want all that agly
            // string compares to happen every time.
            ThrowPropertyNotFoundExp( propertyName, exc );
        }
        throw;
    }
    catch(...)
    {
        if ( !unsupportedTypeExp)
        {
            // Try to throw a better exception, otherwise re-throw the dbi exception
            // Note that we only do this when something goes wrong for performance reason; we don't want all that agly
            // string compares to happen every time.
            ThrowPropertyNotFoundExp( propertyName );
        }
        throw;
    }

    return byteArray;
}

const FdoByte * FdoRdbmsFeatureReader::GetGeometry(const wchar_t* propertyName, FdoInt32 * count)
{
    FdoByte *      data = NULL;
    FdoByteArray * ba = this->GetGeometry(propertyName, false);
    if (NULL != ba)
    {
        // Manage the (exceedingly small) cache of last-fetched geometry.
        // This is required to keep an owner of the geometry, since this method
        // returns only a pointer to the actual array data, not the array object.
        // Ideally, the internal feature cache would own this geometry itself,
        // but the internal format is slightly different and a conversion is done
        // during every fetch.  The retention of a geometry cache here can be removed
        // if the internal cache's format were changed to be unmodified FGF.
        if (NULL != mGeometryCache)
        {
            mGeometryCache->Release();
            mGeometryCache = NULL;
        }
        mGeometryCache = ba;
        data = ba->GetData();
        *count = ba->GetCount();
    }
    return data;
}

/// <summary>Gets the raster object of the specified property.
/// Because no conversion is performed, the property must be
/// of Raster type; otherwise, an exception is thrown.</summary>
/// <param name="propertyName">Input the property name.</param>
/// <returns>Returns the raster object.</returns>
FdoIRaster* FdoRdbmsFeatureReader::GetRaster(FdoString* propertyName)
{
    return (NULL);
}

bool FdoRdbmsFeatureReader::ReadNextWithLocalFilter()
{
    // This applies any local filters to the query result, skipping any rows
    // that do not match.
    // Currently, only secondary spatial filtering is performed.

    mPropertiesFetched = false;
    bool noMore = false;

	// Do this only in the case of spatial filters only
	FdoInt32	numFilters = mSecondarySpatialFilters ? mSecondarySpatialFilters->GetCount() : 0;
	FdoInt32	numLogicalOps = (FdoInt32) mFilterLogicalOps.size();
	bool		doSecFilter = false;

	// Optimization: in case all the filters are primary filters, no further processing is needed.
    for (FdoInt32 i = 0;  i < numFilters && !doSecFilter;  i++)
    { 
        FdoPtr<FdoRdbmsSpatialSecondaryFilter> filter = mSecondarySpatialFilters->GetItem(i);
        doSecFilter = mFdoConnection->NeedsSecondaryFiltering( filter );
	}

    while (!mPropertiesFetched && !noMore)
    {
        mPropertiesFetched = ( this->mQueryResult->ReadNext() != 0 );

        if (mPropertiesFetched && ( numFilters > 0 ) && doSecFilter)
        {
			// Evaluate each filter 
			for (FdoInt32 i = 0;  i < numFilters;  i++)
			{
				FdoPtr<FdoRdbmsSpatialSecondaryFilter> filter = mSecondarySpatialFilters->GetItem(i);
				FdoString * geomPropName = filter->GetPropertyName();
				mHasMoreFeatures = true;    // Fake out flag so property fetch doesn't balk.
				FdoPtr<FdoByteArray> ba = this->GetGeometry(geomPropName, false, mQueryResult);
				mHasMoreFeatures = false;

				// Evaluate the current logical operation.
				int	currLogicalOp = (i == 0)? (int)FdoBinaryLogicalOperations_Or : 
											  mFilterLogicalOps.at(i-1);

				if (ba == NULL)
				{
					mPropertiesFetched = false;
				}
				else 
				{
					FdoPtr<FdoFgfGeometryFactory>	gf = FdoFgfGeometryFactory::GetInstance();
					FdoPtr<FdoIGeometry>			geom = gf->CreateGeometryFromFgf(ba);

					bool	pass = filter->MeetsCondition(geom);

					if ( pass && (currLogicalOp == FdoBinaryLogicalOperations_Or ) )
					{
						mPropertiesFetched = true;
					}
					else if ( !pass && (currLogicalOp == FdoBinaryLogicalOperations_And ) )
					{
						mPropertiesFetched = false;
					}
					else if ( pass && (currLogicalOp == FdoBinaryLogicalOperations_And ) )
					{
						mPropertiesFetched = mPropertiesFetched; // Unchanged
					}
					else // i.e. ( !pass && (currLogicalOp == FdoBinaryLogicalOperations_Or ) )
					{
						mPropertiesFetched = (i == 0)? false : mPropertiesFetched;
					}	
				}
			}
        }
        else
        {
            noMore = true;
        }
    }

    return mPropertiesFetched;
}

bool FdoRdbmsFeatureReader::SkipColumnForProperty(FdoInt32 index)
{
    bool skipColumn = false;

    // When perusing a column list and finding properties from it, we want to
    // skip columns that interfere with the 1-1 mapping (Y and Z ordinates,
    // SI columns).
    char* colName = mColList[index].column;
    FdoRdbmsSchemaUtil * schemaUtil = mConnection->GetSchemaUtil();
    FdoStringP className = mClassDefinition->GetQName();
    const FdoSmLpClassDefinition *classDefinition = schemaUtil->GetClass(className);
    const FdoSmLpPropertyDefinitionCollection *propertyDefinitions = classDefinition->RefProperties();
    FdoInt32 propertyCount = propertyDefinitions->GetCount();

    for ( FdoInt32 i=0 ;  i < propertyCount;  i++ )
    {
        const FdoSmLpPropertyDefinition *propertyDefinition = propertyDefinitions->RefItem(i);

        if (propertyDefinition->GetPropertyType() == FdoPropertyType_GeometricProperty )
        {
            const FdoSmLpGeometricPropertyDefinition* geomProp =
            static_cast<const FdoSmLpGeometricPropertyDefinition*>(propertyDefinition);
            const char * columnNameY = schemaUtil->MakeDBValidName(geomProp->GetColumnNameY());
            const char * columnNameZ = schemaUtil->MakeDBValidName(geomProp->GetColumnNameZ());
            const char * columnNameSi1 = schemaUtil->MakeDBValidName(geomProp->GetColumnNameSi1());
            const char * columnNameSi2 = schemaUtil->MakeDBValidName(geomProp->GetColumnNameSi2());
            if ( ( NULL != columnNameY && '\0' != columnNameY[0] &&
                   0 == FdoCommonOSUtil::stricmp(colName, columnNameY) ) ||
                 ( NULL != columnNameZ && '\0' != columnNameZ[0] &&
                   0 == FdoCommonOSUtil::stricmp(colName, columnNameZ) ) ||
                 ( NULL != columnNameSi1 && '\0' != columnNameSi1[0] &&
                   0 == FdoCommonOSUtil::stricmp(colName, columnNameSi1) ) ||
                 ( NULL != columnNameSi2 && '\0' != columnNameSi2[0] &&
                   0 == FdoCommonOSUtil::stricmp(colName, columnNameSi2) ) )
            {
                skipColumn = true;
            }
        }
    }
    return skipColumn;
}

bool FdoRdbmsFeatureReader::ReadNext( )
{
    bool    optimizedQuery = false;

    if( mQueryResult == NULL )
        return false;

    if (mGeometryCache)
    {
        mGeometryCache->Release();
        mGeometryCache = NULL;
    }

    //
    // NOTE: in the general case when dealing with feature query we would delay the selection of the attributes untill
    // we know the class. This is because the initial query may return more than one class type. This is particularly true
    // when executing a spatial query. However is the select command class is a concrete class, we may be dealing with a result
    // from a single class. For this particular case, we can optimize the query by combinning the attribute selection with the feature
    // query. The other part of this optimization is done by FdoRdbmsFilterUtil::FilterToSql.
    if( ! mClassDefinition->GetIsAbstract() )
        optimizedQuery = true;

    // End the select for the previous feature class attribute query
    if( ! optimizedQuery && mIsFeatureQuery && mAttrsQidIdx != -1 && mAttrQueryCache[mAttrsQidIdx].query == NULL )
    {
        mAttrQueryCache[mAttrsQidIdx].query->Close();
        delete mAttrQueryCache[mAttrsQidIdx].query;
        mAttrQueryCache[mAttrsQidIdx].query = NULL;

        if( mAttrQueryCache[mAttrsQidIdx].statement )
        {
            delete mAttrQueryCache[mAttrsQidIdx].statement;
            mAttrQueryCache[mAttrsQidIdx].statement = NULL;
        }
        mAttrsQidIdx = -1;
    }

    mHasMoreFeatures = false;
    if ( ! ReadNextWithLocalFilter() )
    {
        mQueryResult->Close();
        delete mQueryResult;
        mQueryResult = NULL;
        return false;
    }
    // A new property row is available. This get overwritten below when we are handling a feature query.
    // For feature query, we need to execute a secondary query against the class table to get the properties

    if( mIsFeatureQuery )
    {
        gdbi_feat_info_def  dbiFeature;
        bool        isNull = false;

        mPropertiesFetched = false;
		
		int cacheIndex;

		// Reinitialize cache cursor
		mLastPropertyInfoDef = 0;

        if ( mClassIdPropName == L"" )
            dbiFeature.classid = (long)mClassDefinition->GetId();
        else
		{
			mLastPropertyInfoDef++;
			PROPERTY2COLNAME_IDX( mClassIdPropName, NULL, NULL, &cacheIndex );
            mQueryResult->GetBinaryValue( GetPropertyInfoDef(cacheIndex)->columnPosition, sizeof(long),
                        (char *)&dbiFeature.classid, NULL, NULL );
		}

        if ( mRevNumPropName == L"" )
            dbiFeature.changeseq = 0;
        else
		{
			mLastPropertyInfoDef++;
			PROPERTY2COLNAME_IDX( mRevNumPropName, NULL, NULL, &cacheIndex );
            mQueryResult->GetBinaryValue( GetPropertyInfoDef(cacheIndex)->columnPosition, sizeof(long),
                        (char *)&dbiFeature.changeseq, &isNull, NULL );
		}

        // RevisionNumber could be null or RevisionColumn does not exist.
        if (isNull == false)
        {
            mCurrentRevisionNumber = dbiFeature.changeseq;
            mCurrentRevisionNumberValid = true;
        }
        else
            mCurrentRevisionNumberValid = false;


        if( optimizedQuery )
        {
            //
            // Use the feature query id to fetch the class properties. Mark the cache as if the query is already cached.
            mAttrsQidIdx = 0;
            mAttrQueryCache[mAttrsQidIdx].query = mQueryResult;
            wcscpy( mLastClassName, mCurrentClassName );
            mPropertiesFetched = true;
        }

    } // if( mIsFeatureQuery )
    else
    {
        mAttrsQidIdx = 0;
        mAttrQueryCache[mAttrsQidIdx].query = mQueryResult;
        wcscpy( mLastClassName, mCurrentClassName );
    }

    return (mHasMoreFeatures = true);
}

void FdoRdbmsFeatureReader::Close( )
{
  if (mGeometryCache)
  {
    mGeometryCache->Release();
    mGeometryCache = NULL;
  }

  // Free all the cursors and other resourses.
  if ( mQueryResult )
  {
    mQueryResult->Close();
    delete mQueryResult;
    mQueryResult = NULL;
  }

  for( int i = 1; i<QUERY_CACHE_SIZE; i++ )
  {
       if( mAttrQueryCache[i].query != NULL )
       {
            mAttrQueryCache[i].query->Close();
            delete mAttrQueryCache[i].query;
            mAttrQueryCache[i].query = NULL;
       }
       if( mAttrQueryCache[i].statement )
       {
            delete mAttrQueryCache[i].statement;
            mAttrQueryCache[i].statement = NULL;
       }
       if( mAttrQueryCache[i].mColList != NULL )
       {
            delete[] mAttrQueryCache[i].mColList;
            mAttrQueryCache[i].mColList = NULL;
       }
  }
}

void FdoRdbmsFeatureReader::Dispose()
{
    Close( );
    delete this;
}


int FdoRdbmsFeatureReader::GetAttributeQuery( wchar_t* className )
{
    int         i;

    if( wcscmp( mLastClassName, className ) == 0 )
        return mLastAttrQueryIdx;
    else
        wcscpy( mLastClassName, className );

    //
    // The first position [0] of the cache is used by non-feature class queries.
    for( i=1; i<QUERY_CACHE_SIZE; i++ )
    {
        if( FdoCommonOSUtil::stricmp( (char*)mConnection->GetUtility()->UnicodeToUtf8(className), mAttrQueryCache[i].class_name ) == 0 )
            break;
    }

    if( i != QUERY_CACHE_SIZE )
    {
        return (mLastAttrQueryIdx = i);
    }
    else
    {
        // Find a free slot
        for( i=1; i<QUERY_CACHE_SIZE; i++ )
        {
            if( mAttrQueryCache[i].query == NULL )
            {
                strncpy( mAttrQueryCache[i].class_name, mConnection->GetUtility()->UnicodeToUtf8(className), GDBI_SCHEMA_ELEMENT_NAME_SIZE );
                mAttrQueryCache[i].class_name[GDBI_SCHEMA_ELEMENT_NAME_SIZE - 1] = '\0';
                return  (mLastAttrQueryIdx = i);
            }
        }

        // We didn't find one free: Let's reuse an existing one. But which one??? Let's use a FIFO scheme
        int    nextIdx = mNextQidToFree%QUERY_CACHE_SIZE;
        mNextQidToFree ++;

        // Free the resources allocated by the previous query
        if( mAttrQueryCache[nextIdx].query != NULL )
            delete mAttrQueryCache[nextIdx].query;

        if( mAttrQueryCache[nextIdx].statement != NULL )
            delete mAttrQueryCache[nextIdx].statement;

       if( mAttrQueryCache[nextIdx].mColList != NULL )
            delete[] mAttrQueryCache[nextIdx].mColList;
        mAttrQueryCache[nextIdx].query = NULL;
        mAttrQueryCache[nextIdx].statement = NULL;

        // Mark it for the new class
        strncpy( mAttrQueryCache[nextIdx].class_name, mConnection->GetUtility()->UnicodeToUtf8(className), GDBI_SCHEMA_ELEMENT_NAME_SIZE );
        mAttrQueryCache[nextIdx].class_name[GDBI_SCHEMA_ELEMENT_NAME_SIZE - 1] = '\0';
        return (mLastAttrQueryIdx = nextIdx );
    }
}

