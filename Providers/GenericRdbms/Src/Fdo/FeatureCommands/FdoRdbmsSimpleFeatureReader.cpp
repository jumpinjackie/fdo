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
#include "FdoRdbmsSimpleFeatureReader.h"
#include "FdoRdbmsDescribeSchemaCommand.h"
#include "FdoRdbmsUtil.h"
#include "FdoRdbmsSchemaUtil.h"
#include "FdoRdbmsFilterProcessor.h"
#include "FdoRdbmsBLOBStreamReader.h"
#include "FdoRdbmsSimpleBLOBStreamReader.h"
#include "FdoCommonSchemaUtil.h"

#include "Inc/ut.h"

#include "FdoCommonOSUtil.h"
#include "FdoCommonMiscUtil.h"

#include <limits>       // For quiet_NaN()

static  char*  _noMoreRows = "End of rows or ReadNext not called"; // error message that repeats
static  char*  _strNUllColumnExp = "Column '%1$ls' value is NULL; use IsNull method before trying to access this column value";

#define GET_VALUE( type, colIdx, FN ) { \
    type    value; \
    bool  isNULL = false; \
    if( ! mHasMoreRows ) \
        throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_62, _noMoreRows)); \
    ValidateIndex(colIdx);\
    GdbiColumnDetail* pCol = mColList[colIdx]; \
    value = FN(pCol->idxStmt+1, &isNULL, NULL); \
    if( isNULL ) \
        throw FdoCommandException::Create(NlsMsgGet1( FDORDBMS_386, _strNUllColumnExp,  pCol->col.c_alias)); \
    return value; \
}

FdoRdbmsSimpleFeatureReader::FdoRdbmsSimpleFeatureReader (FdoIConnection *connection, GdbiQueryResult* queryResult, bool isFeatureQuery, const FdoSmLpClassDefinition *classDef, FdoFeatureSchemaCollection *schmCol, FdoIdentifierCollection *properties) :
mQueryResult( queryResult ),
mSchemaCollection(schmCol ),
mFdoConnection( static_cast<FdoRdbmsConnection*>(connection) ),
mClassDefinition( classDef ),
mProperties(properties),
mIsFeatureQuery( isFeatureQuery ),
mFdoClassDefinition(NULL),
mMainClassDefinition(NULL),
mConnection( NULL ),
mHasMoreRows( false ),
mColCount( 0 ),
mWkbBuffer(NULL),
mWkbBufferLen(0),
mWkbGeomLen(0),
mSprops(NULL),
mGeomIdx(-1)
{
    if( mFdoConnection )
    {
        mFdoConnection->AddRef();
        mConnection = mFdoConnection->GetDbiConnection();
    }
    if( mProperties )
        mProperties->AddRef();

    // should we delay this?
    // if yes we will need to add a check for each method from below
    GenerateInternalMapping();
}

FdoRdbmsSimpleFeatureReader::~FdoRdbmsSimpleFeatureReader()
{
    Close();

    if (mFdoClassDefinition)
        mFdoClassDefinition->Release();

    if( mProperties )
        mProperties->Release();

    delete mQueryResult;

    for (LinkColumnList::iterator it = mColList.begin(); it < mColList.end(); it++)
        delete *it;
    mColList.clear();

    if( mFdoConnection )
        mFdoConnection->Release();
	
    delete[] mSprops;
    delete[] mWkbBuffer;
}

void FdoRdbmsSimpleFeatureReader::Dispose()
{
    delete this;
}

void FdoRdbmsSimpleFeatureReader::GenerateInternalMapping()
{
    mColCount = mQueryResult->GetColumnCount();
    if( mColCount == 0 )
        return;

    int selPropsCnt = (mProperties != NULL) ? mProperties->GetCount() : 0;
    for (int i = 0; i < mColCount; i++)
    {
        GdbiColumnDetail* pCol = new GdbiColumnDetail();
        mColList.push_back(pCol);
        pCol->idxStmt = i;
        *pCol->col.column = *pCol->col.c_alias = '\0';
        mQueryResult->GetColumnDesc (i+1, pCol->col);

        if (mProperties)
        {
            // in case we have extra properties selected process them later
            if (i < selPropsCnt)
            {
                FdoPtr<FdoIdentifier> item = mProperties->GetItem(i);
                wcscpy(pCol->col.c_alias, item->GetName());
            }
        }
    }

    FdoPtr<FdoClassDefinition> clsDef = GetClassDefinition();
    for (int i = 0; i < mColCount; i++)
    {
        GdbiColumnDetail* pCol = mColList[i];
        mColMap[pCol->col.c_alias] = std::make_pair(pCol, i);
    }
    mSprops = new StringRec[mColCount];
}

int FdoRdbmsSimpleFeatureReader::NameToIndex(FdoString* name) 
{
    GdbiColumnDetailToIdx::iterator it = mColMap.find(name);
    if (it != mColMap.end())
        return it->second.second;
    throw FdoCommandException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_74_PROPERTY_NAME_NOT_FOUND), name));
}

FdoClassDefinition* FdoRdbmsSimpleFeatureReader::GetClassDefinition()
{
    if (mFdoClassDefinition != NULL)
        return FDO_SAFE_ADDREF(mFdoClassDefinition);

    FdoStringP clsName = mClassDefinition->GetQName();

    FdoClassDefinitionP classDef;
    FdoClassDefinition *returnClassDef = NULL;

    const FdoSmLpClassDefinition* pClass = mClassDefinition;
    FdoPtr<FdoIdentifier> className = FdoIdentifier::Create( clsName );
    int leng;
    if( className->GetScope(leng) != NULL && leng != 0 && mClassDefinition->GetParent() && mClassDefinition->GetParent()->GetParent() )
    {
        const FdoSmLpObjectPropertyDefinition* objProp = static_cast<const FdoSmLpObjectPropertyDefinition*>(mClassDefinition->GetParent()->GetParent());
        pClass = objProp->RefClass();
    }

    clsName = pClass->GetQName();
    if( mSchemaCollection == NULL )
    {
        FdoPtr<FdoRdbmsDescribeSchemaCommand>  pDescSchemaCmd = new FdoRdbmsDescribeSchemaCommand( mConnection );
        pDescSchemaCmd->SetSchemaName(pClass->RefLogicalPhysicalSchema()->GetName());
        FdoStringsP classNames = FdoStringCollection::Create();
        classNames->Add(clsName);
        pDescSchemaCmd->SetClassNames(classNames);
        mSchemaCollection = pDescSchemaCmd->Execute();
    }

    FdoPtr<FdoFeatureSchema> schm = mSchemaCollection->FindItem( pClass->RefLogicalPhysicalSchema()->GetName() );
    if( schm )
    {
        FdoPtr<FdoClassCollection> classes = schm->GetClasses();
        classDef = classes->FindItem( pClass->GetName() );
    }

    if( classDef )
    {
        if (mClassDefinition != pClass)
        {
            mMainClassDefinition = mClassDefinition;
            mClassDefinition = pClass;
        }

        returnClassDef = FilterClassDefinition( classDef );
        mFdoClassDefinition = FDO_SAFE_ADDREF(returnClassDef);
    }

    return returnClassDef;
}

FdoPropertyDefinition* FdoRdbmsSimpleFeatureReader::GetPropertyFromComputed(FdoClassDefinition* classDef, GdbiColumnDesc* colDesc, FdoComputedIdentifier* cidf)
{
    FdoPropertyDefinition* retVal = NULL;
    FdoPropertyType propType;
    FdoDataType dataType;
    try
    {
        FdoPtr<FdoIExpressionCapabilities> expressionCaps = mFdoConnection->GetExpressionCapabilities();
        FdoPtr<FdoFunctionDefinitionCollection> functions = expressionCaps->GetFunctions();
        FdoPtr<FdoExpression> expr = cidf->GetExpression();
        FdoCommonMiscUtil::GetExpressionType(functions, classDef, expr, propType, dataType);
    }
    catch (FdoException *e)
    {
        e->Release();

        // The expression was not recognized, use RDBI to determine types:
        if (colDesc->datatype != RDBI_GEOMETRY)
        {
            propType = FdoPropertyType_DataProperty;
            dataType = FdoRdbmsUtil::DbiToFdoType (colDesc->datatype);
        }
        else
            propType = FdoPropertyType_GeometricProperty;
    }
    switch(propType)
    {
    case FdoPropertyType_DataProperty:
        {
            FdoDataPropertyDefinition* pProp = FdoDataPropertyDefinition::Create (cidf->GetName(), L"Computed Property");
            pProp->SetDataType (dataType);
            retVal = pProp;
        }
        break;
    case FdoPropertyType_GeometricProperty:
        retVal = FdoGeometricPropertyDefinition::Create(cidf->GetName(), L"Computed Property" );
        break;
    default :
        throw FdoCommandException::Create (NlsMsgGet (FDORDBMS_116, "Unsupported geometry type"));
        break;
    }
    return retVal;
}

FdoPropertyDefinition* FdoRdbmsSimpleFeatureReader::GetClonePropertyByName(FdoClassDefinition* classDef, FdoPropertyDefinitionCollection* pcoll, GdbiColumnDesc* colDesc, FdoIdentifier* idf)
{
    if (idf != NULL)
    {
        FdoString* idfName = idf->GetName();
        FdoPtr<FdoPropertyDefinition> prop = pcoll->FindItem(idfName);
        if (prop == NULL)
        {
            FdoPtr<FdoClassDefinition> baseClsDef = classDef->GetBaseClass();            
            if (baseClsDef != NULL)
            {
                FdoPtr<FdoPropertyDefinitionCollection> pBaseColl = baseClsDef->GetProperties();
                return GetClonePropertyByName(baseClsDef, pBaseColl, colDesc, idf);
            }
            // it must be ClassId/RevisionNumber or an invalid property
            if (_wcsicmp(L"RevisionNumber", idfName) == 0 || _wcsicmp(L"ClassId", idfName) == 0)
            {
                FdoDataPropertyDefinition* retVal = FdoDataPropertyDefinition::Create(idfName, L"", true);
                retVal->SetReadOnly(true);
                retVal->SetDataType(FdoDataType_Int32);
                return retVal;
            }
        }
        else
            return FdoCommonSchemaUtil::DeepCopyFdoPropertyDefinition(prop);
    }
    else
    {
        int cnt = pcoll->GetCount();
        FdoPropertyDefinition* prop = NULL;
        for (int i = 0; i < cnt; i++)
        {
            FdoPtr<FdoPropertyDefinition> item = pcoll->GetItem(i);
            if (_wcsicmp(colDesc->column, item->GetName()) == 0)
            {
                prop = item.p;
                break;
            }
        }
        if (prop == NULL)
        {
            FdoPtr<FdoClassDefinition> baseClsDef = classDef->GetBaseClass();            
            if (baseClsDef != NULL)
            {
                FdoPtr<FdoPropertyDefinitionCollection> pBaseColl = baseClsDef->GetProperties();
                return GetClonePropertyByName(baseClsDef, pBaseColl, colDesc);
            }
        }
        else
            return FdoCommonSchemaUtil::DeepCopyFdoPropertyDefinition(prop);
    }
    return NULL;
}

void FdoRdbmsSimpleFeatureReader::AddToPkIfNeeded(FdoDataPropertyDefinitionCollection* idProps, FdoDataPropertyDefinition* dp, FdoClassDefinition* classDef)
{
    FdoPtr<FdoDataPropertyDefinitionCollection> idp = classDef->GetIdentityProperties();
    int cnt = idp->GetCount();
    for (int i = 0; i < cnt; i++)
    {
        FdoPtr<FdoDataPropertyDefinition> ldp = idp->GetItem(i);
        // data type will avoid making too many string compare
        if (ldp->GetDataType() == dp->GetDataType() && _wcsicmp(ldp->GetName(), dp->GetName()) == 0)
        {
            idProps->Add(dp);
            return;
        }
    }
    FdoPtr<FdoClassDefinition> baseCls = classDef->GetBaseClass();
    if (baseCls != NULL)
        AddToPkIfNeeded(idProps, dp, baseCls);
}

FdoClassDefinition* FdoRdbmsSimpleFeatureReader::FilterClassDefinition(FdoClassDefinition* classDef)
{
    FdoClassDefinition *returnClassDef = NULL;

    int cntProps = mProperties  != NULL ? mProperties->GetCount() : 0;
    if (cntProps)
    {
        bool isComputed = false;
        FdoPtr<FdoGeometricPropertyDefinition> geomProperty;
        FdoGeometricPropertyDefinition* retGeomProperty = NULL;

        if (classDef->GetClassType() == FdoClassType_FeatureClass)
            geomProperty = ((FdoFeatureClass*)classDef)->GetGeometryProperty();

        if (!mIsFeatureQuery)
        {
            FdoClass *clas = FdoClass::Create(classDef->GetName(), classDef->GetDescription());
            returnClassDef = clas;
        }
        else
        {
            FdoFeatureClass* fc = FdoFeatureClass::Create(classDef->GetName(), classDef->GetDescription());
            returnClassDef = fc;
        }

        FdoPtr<FdoPropertyDefinitionCollection> clsProps = returnClassDef->GetProperties();
        FdoPtr<FdoDataPropertyDefinitionCollection> idProps = returnClassDef->GetIdentityProperties();
        FdoPtr<FdoPropertyDefinitionCollection> pSrcColl = classDef->GetProperties();

        FdoPtr<FdoIdentifier> id;
        for (int i = 0; i < cntProps; i++)
        {
            GdbiColumnDetail* colDet = mColList[i];
            id = mProperties->GetItem(i);
            if (id->GetExpressionType() == FdoExpressionItemType_ComputedIdentifier)
            {
                isComputed = true;
                FdoPtr<FdoPropertyDefinition> dp = GetPropertyFromComputed(classDef, &colDet->col, static_cast<FdoComputedIdentifier*>(id.p));
                clsProps->Add(dp);
                if (*colDet->col.c_alias == '\0')
                    wcscpy(colDet->col.c_alias, dp->GetName());
            }
            else
            {
                FdoPtr<FdoPropertyDefinition> dp = GetClonePropertyByName(classDef, pSrcColl, &colDet->col, id);
                if (dp == NULL)
                    dp = pSrcColl->GetItem(colDet->col.column); // this will generate an exception (we should never be here)
                else
                    clsProps->Add(dp);

                if (*colDet->col.c_alias == '\0')
                    wcscpy(colDet->col.c_alias, dp->GetName());
                if (dp->GetPropertyType() == FdoPropertyType_GeometricProperty && geomProperty != NULL && _wcsicmp(geomProperty->GetName(), dp->GetName()) == 0)
                {
                    FdoGeometricPropertyDefinition* gprop = static_cast<FdoGeometricPropertyDefinition*>(dp.p);
                    gprop->SetGeometryTypes(geomProperty->GetGeometryTypes());
                    gprop->SetHasElevation(geomProperty->GetHasElevation());
                    gprop->SetHasMeasure(geomProperty->GetHasMeasure());
                    gprop->SetSpatialContextAssociation(geomProperty->GetSpatialContextAssociation());
                    FdoInt32 length = 0;
                    gprop->SetSpecificGeometryTypes(geomProperty->GetSpecificGeometryTypes(length), length);
                    retGeomProperty = gprop;
                }
                else if (dp->GetPropertyType() == FdoPropertyType_DataProperty)
                    AddToPkIfNeeded(idProps, static_cast<FdoDataPropertyDefinition*>(dp.p), classDef);
            }
        }
        // handle extra properties added
        bool notFoundProp = false;
        for (int i = cntProps; i < mColCount; i++)
        {
            GdbiColumnDetail* colDet = mColList[i];
            FdoPtr<FdoPropertyDefinition> dp = (!notFoundProp) ? GetClonePropertyByName(classDef, pSrcColl, &colDet->col) : NULL;
            if (dp == NULL)
            {
                // do not add extra properties if are not part of the class (those can be hidden properties)
                notFoundProp = true;
                if (*colDet->col.c_alias == '\0')
                    wcscpy(colDet->col.c_alias, colDet->col.column);
                continue;
            }
            clsProps->Add(dp);
            if (*colDet->col.c_alias == '\0')
                wcscpy(colDet->col.c_alias, dp->GetName());
            if (dp->GetPropertyType() == FdoPropertyType_GeometricProperty && geomProperty != NULL && _wcsicmp(geomProperty->GetName(), dp->GetName()) == 0)
            {
                FdoGeometricPropertyDefinition* gprop = static_cast<FdoGeometricPropertyDefinition*>(dp.p);
                gprop->SetGeometryTypes(geomProperty->GetGeometryTypes());
                gprop->SetHasElevation(geomProperty->GetHasElevation());
                gprop->SetHasMeasure(geomProperty->GetHasMeasure());
                gprop->SetSpatialContextAssociation(geomProperty->GetSpatialContextAssociation());
                FdoInt32 length = 0;
                gprop->SetSpecificGeometryTypes(geomProperty->GetSpecificGeometryTypes(length), length);
                retGeomProperty = gprop;
            }
            else if (dp->GetPropertyType() == FdoPropertyType_DataProperty)
                AddToPkIfNeeded(idProps, static_cast<FdoDataPropertyDefinition*>(dp.p), classDef);
        }
        if (mIsFeatureQuery && retGeomProperty != NULL)
            ((FdoFeatureClass*)returnClassDef)->SetGeometryProperty(retGeomProperty);

        returnClassDef->SetIsAbstract(classDef->GetIsAbstract());

        if( isComputed )
            returnClassDef->SetIsComputed( true );
    }
    else
    {
        // since select * from class returns properties in a different order than FDO class
        // we need to match properties.
        LinkColumnList tmpColList;
        const FdoSmLpClassDefinition* pClass = mClassDefinition;
        FdoPtr<FdoClassDefinition> tmpClsDef = FDO_SAFE_ADDREF(classDef);
        while (pClass && tmpClsDef)
        {
            const FdoSmLpPropertyDefinitionCollection *propertyDefinitions = pClass->RefProperties();
            FdoPtr<FdoPropertyDefinitionCollection> pSrcColl = tmpClsDef->GetProperties();
            cntProps = pSrcColl->GetCount();
            for (int i = 0; i < cntProps; i++)
            {
                FdoPtr<FdoPropertyDefinition> propDef = pSrcColl->GetItem(i);
                FdoString* colName = NULL;
                FdoString* propName = propDef->GetName();
                const FdoSmLpPropertyDefinition *propertyDefinition = propertyDefinitions->RefItem(propName);
                if (propertyDefinition)
                {
                    FdoPropertyType propType = propertyDefinition->GetPropertyType();
		            if (propType == FdoPropertyType_DataProperty || propType == FdoPropertyType_GeometricProperty)
		            {
			            const FdoSmLpSimplePropertyDefinition* dataProp = static_cast<const FdoSmLpSimplePropertyDefinition*>(propertyDefinition);
			            const FdoSmPhColumn* column = dataProp->RefColumn();
                        if (column != NULL)
                            colName = column->GetName();
                        else
                            colName = dataProp->GetColumnName();
                    }
                }
                
                if (colName)
                {
                    for (size_t k = 0; k < mColList.size(); k++)
                    {
                        GdbiColumnDetail* colDet = mColList[k];
                        if (_wcsicmp(colDet->col.column, colName) == 0)
                        {
                            tmpColList.push_back(colDet);
                            if (*colDet->col.c_alias == '\0')
                                wcscpy(colDet->col.c_alias, propName);
                            mColList.erase(mColList.begin()+k);
                            break;
                        }
                    }
                }
                else // colName should never be NULL (added just to be aware in case this happen)
                    throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_52, "Index out of range"));
            }
            pClass = pClass->RefBaseClass();
            tmpClsDef = tmpClsDef->GetBaseClass();
        }
        // in case we still have left elements move them
        pClass = mMainClassDefinition;
        const FdoSmLpPropertyDefinitionCollection* propertyDefinitionsColl = NULL;
        if (mColList.size() != 0 && pClass)
            propertyDefinitionsColl = pClass->RefProperties();

        while(mColList.size() != 0)
        {
            GdbiColumnDetail* colDet = mColList[0];
            tmpColList.push_back(colDet);
            if (propertyDefinitionsColl && *colDet->col.c_alias == '\0')
            {
                // this part will be executed only in case we have a class with a scope (className->GetScope(leng) != NULL)
                // we select from a subclass
                cntProps = propertyDefinitionsColl->GetCount();
                for (int i = 0; i < cntProps; i++)
                {
                    const FdoSmLpPropertyDefinition *propertyDefinition = propertyDefinitionsColl->RefItem(i);
                    FdoPropertyType propType = propertyDefinition->GetPropertyType();
	                if (propType == FdoPropertyType_DataProperty || propType == FdoPropertyType_GeometricProperty)
	                {
		                const FdoSmLpSimplePropertyDefinition* dataProp = static_cast<const FdoSmLpSimplePropertyDefinition*>(propertyDefinition);
                        FdoString* colNameProp = dataProp->GetColumnName();
                        if (colNameProp && _wcsicmp(colDet->col.column, colNameProp) == 0)
                        {
                            wcscpy(colDet->col.c_alias, propertyDefinition->GetName());
                            break;
                        }
                    }
                }
            }
            if (*colDet->col.c_alias == '\0')
                wcscpy(colDet->col.c_alias, colDet->col.column);
            mColList.erase(mColList.begin());
        }
        mColList.insert(mColList.begin(), tmpColList.begin(), tmpColList.end());
        returnClassDef = FDO_SAFE_ADDREF(classDef);
    }

    return returnClassDef;
}

FdoInt32 FdoRdbmsSimpleFeatureReader::GetDepth()
{
    return 0;
}

FdoString* FdoRdbmsSimpleFeatureReader::GetPropertyName(FdoInt32 index)
{
    ValidateIndex(index);
    return mColList[index]->col.c_alias;
}

FdoInt32 FdoRdbmsSimpleFeatureReader::GetPropertyIndex(FdoString* propertyName)
{
    return NameToIndex(propertyName);
}

FdoPropertyType FdoRdbmsSimpleFeatureReader::GetPropertyType(FdoString* propertyName)
{
    return FdoRdbmsSimpleFeatureReader::GetPropertyType(NameToIndex(propertyName));
}

FdoPropertyType FdoRdbmsSimpleFeatureReader::GetPropertyType(FdoInt32 index)
{
    ValidateIndex(index);
    if( mColList[index]->col.datatype == RDBI_GEOMETRY )
        return FdoPropertyType_GeometricProperty;
    else
        return FdoPropertyType_DataProperty;
}

bool FdoRdbmsSimpleFeatureReader::GetBoolean(FdoString* propertyName)
{
    return FdoRdbmsSimpleFeatureReader::GetBoolean(NameToIndex(propertyName));
}

FdoBoolean FdoRdbmsSimpleFeatureReader::GetBoolean(FdoInt32 index)
{
    GET_VALUE (bool, index, mQueryResult->GetBoolean);
}

FdoByte FdoRdbmsSimpleFeatureReader::GetByte(FdoString* propertyName)
{
    return (FdoByte)FdoRdbmsSimpleFeatureReader::GetInt16(NameToIndex(propertyName));
}

FdoByte FdoRdbmsSimpleFeatureReader::GetByte(FdoInt32 index)
{
    return (FdoByte)FdoRdbmsSimpleFeatureReader::GetInt16(index);
}

FdoDateTime FdoRdbmsSimpleFeatureReader::GetDateTime(FdoString* propertyName)
{
    return mFdoConnection->DbiToFdoTime(FdoRdbmsSimpleFeatureReader::GetString(NameToIndex(propertyName)));
}

FdoDateTime FdoRdbmsSimpleFeatureReader::GetDateTime(FdoInt32 index)
{
    return mFdoConnection->DbiToFdoTime(FdoRdbmsSimpleFeatureReader::GetString(index));
}

double FdoRdbmsSimpleFeatureReader::GetDouble(FdoString* propertyName)
{
    return FdoRdbmsSimpleFeatureReader::GetDouble(NameToIndex(propertyName));
}

double FdoRdbmsSimpleFeatureReader::GetDouble(FdoInt32 index)
{
    GET_VALUE (double, index, mQueryResult->GetDouble);
}

FdoInt16 FdoRdbmsSimpleFeatureReader::GetInt16(FdoString* propertyName)
{
    return FdoRdbmsSimpleFeatureReader::GetInt16(NameToIndex(propertyName));
}

FdoInt16 FdoRdbmsSimpleFeatureReader::GetInt16(FdoInt32 index)
{
    GET_VALUE (short, index, mQueryResult->GetInt16);
}

FdoInt32 FdoRdbmsSimpleFeatureReader::GetInt32(FdoString* propertyName)
{
    return FdoRdbmsSimpleFeatureReader::GetInt32(NameToIndex(propertyName));
}

FdoInt32 FdoRdbmsSimpleFeatureReader::GetInt32(FdoInt32 index)
{
    GET_VALUE (int, index, mQueryResult->GetInt32);
}

FdoInt64 FdoRdbmsSimpleFeatureReader::GetInt64(FdoString* propertyName)
{
    return FdoRdbmsSimpleFeatureReader::GetInt64(NameToIndex(propertyName));
}

FdoInt64 FdoRdbmsSimpleFeatureReader::GetInt64(FdoInt32 index)
{
    GET_VALUE (FdoInt64, index, mQueryResult->GetInt64);
}

float FdoRdbmsSimpleFeatureReader::GetSingle(FdoString* propertyName)
{
    return FdoRdbmsSimpleFeatureReader::GetSingle(NameToIndex(propertyName));
}

float FdoRdbmsSimpleFeatureReader::GetSingle(FdoInt32 index)
{
    GET_VALUE (float, index, mQueryResult->GetFloat);
}

FdoString* FdoRdbmsSimpleFeatureReader::GetString(FdoString* propertyName)
{
    return FdoRdbmsSimpleFeatureReader::GetString(NameToIndex(propertyName));
}

FdoString* FdoRdbmsSimpleFeatureReader::GetString(FdoInt32 index)
{
    bool isNULL = false;
    if (! mHasMoreRows)
        throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_62, _noMoreRows));
    ValidateIndex(index);

    int i = index;
	if (mSprops[i].valid)
		return mSprops[i].data;

    GdbiColumnDetail* pCol = mColList[index];
    const wchar_t* tmpVal = NULL;
    if (pCol->col.datatype == RDBI_WSTRING_ULEN)
    {
        FdoByteArray* arr = NULL;
        mQueryResult->GetBinaryValue (pCol->idxStmt+1, sizeof(FdoByteArray*), (char*)&arr, &isNULL, NULL);
        if (!isNULL && arr != NULL && arr->GetCount() != 0)
        {
            int sizeW = (int)(arr->GetCount()/sizeof(wchar_t));
            mSprops[i].EnsureSize(sizeW + 1);
            memcpy(mSprops[i].data, arr->GetData(), arr->GetCount());
            *(mSprops[i].data+sizeW) = L'\0';
            mSprops[i].valid = 1;
            return mSprops[i].data;
        }
    }
    else if (pCol->col.datatype == RDBI_STRING_ULEN)
    {
        FdoByteArray* arr = NULL;
        mQueryResult->GetBinaryValue (pCol->idxStmt+1, sizeof(FdoByteArray*), (char*)&arr, &isNULL, NULL);
        if (!isNULL && arr != NULL && arr->GetCount() != 0)
        {
            int cntArr = arr->GetCount();
            mSprops[i].EnsureSize(2*cntArr + 1);
            char* startCh = (char*)(mSprops[i].data+cntArr);
            memcpy(startCh, arr->GetData(), cntArr);
            *(startCh+cntArr) = L'\0'; // add string ending
            ut_utf8_to_unicode(startCh, mSprops[i].data, cntArr+1);
            mSprops[i].valid = 1;
            return mSprops[i].data;
        }
    }
    else
        tmpVal = mQueryResult->GetString(pCol->idxStmt+1, &isNULL, NULL);

    if (isNULL || tmpVal == NULL)
    {
        mSprops[i].EnsureSize(1);
        mSprops[i].data = L'\0';
        mSprops[i].valid = 1;
        throw FdoCommandException::Create(NlsMsgGet1( FDORDBMS_386, _strNUllColumnExp, pCol->col.c_alias ));
    }
    
    size_t sz = wcslen(tmpVal);
    mSprops[i].EnsureSize(sz+1);
    wcscpy(mSprops[i].data, tmpVal);
    mSprops[i].valid = 1;

    return mSprops[i].data;
}

FdoLOBValue* FdoRdbmsSimpleFeatureReader::GetLOB(FdoString* propertyName)
{
    return FdoRdbmsSimpleFeatureReader::GetLOB(NameToIndex(propertyName));
}

FdoLOBValue* FdoRdbmsSimpleFeatureReader::GetLOB(FdoInt32 index)
{
    ValidateIndex(index);

    bool isNull = false;
    GdbiColumnDetail* pCol = mColList[index];
    if (pCol->col.datatype == RDBI_BLOB_ULEN)
    {
        FdoByteArray* arr = NULL;
        mQueryResult->GetBinaryValue (pCol->idxStmt+1, sizeof(FdoByteArray*), (char*)&arr, &isNull, NULL);
        if (!isNull && arr != NULL && arr->GetCount() != 0)
            return static_cast<FdoLOBValue*>(FdoDataValue::Create(arr->GetData(), arr->GetCount(), FdoDataType_BLOB));
    }
    else if (pCol->col.size > 0)
    {
        FdoByte* vblob = new FdoByte[pCol->col.size];
        try
        {
            mQueryResult->GetBinaryValue (pCol->idxStmt+1, pCol->col.size, (char*)vblob, &isNull, NULL);
            FdoLOBValue* retVal = (!isNull) ? (FdoLOBValue*)FdoDataValue::Create(vblob, pCol->col.size, FdoDataType_BLOB) : NULL;
            delete[] vblob;
            return retVal;
        }
        catch(...)
        {
            delete[] vblob;
            throw;
        }
    }

    throw FdoCommandException::Create(NlsMsgGet1( FDORDBMS_386, _strNUllColumnExp, pCol->col.c_alias ));
}

FdoIStreamReader* FdoRdbmsSimpleFeatureReader::GetLOBStreamReader(FdoString* propertyName)
{
    return FdoRdbmsSimpleFeatureReader::GetLOBStreamReader(NameToIndex(propertyName));
}

FdoIStreamReader* FdoRdbmsSimpleFeatureReader::GetLOBStreamReader(FdoInt32 index)
{
    FdoPtr<FdoLOBValue> blob = FdoRdbmsSimpleFeatureReader::GetLOB(index);
    return new FdoRdbmsSimpleBLOBStreamReader(blob); 
}

bool FdoRdbmsSimpleFeatureReader::IsNull(FdoString* propertyName)
{
    return FdoRdbmsSimpleFeatureReader::IsNull(NameToIndex(propertyName));
}

bool FdoRdbmsSimpleFeatureReader::IsNull(FdoInt32 index)
{
    bool isNull = true;
    if( ! mHasMoreRows )
        throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_62, _noMoreRows));
    ValidateIndex(index);

    GdbiColumnDetail* pCol = mColList[index];
    switch(pCol->col.datatype)
    {
    case RDBI_GEOMETRY:
        {
            FdoInt32 len = 0;
            GetGeometry(index, &len, true);
            isNull = (len == 0);
        }
        break;
    case RDBI_BLOB_ULEN:
    case RDBI_WSTRING_ULEN:
    case RDBI_STRING_ULEN:
        {
            FdoByteArray* arr = NULL;
            bool isNullArr = false;
            mQueryResult->GetBinaryValue (pCol->idxStmt+1, sizeof(FdoByteArray*), (char*)&arr, &isNullArr, NULL);
            isNull = (isNullArr || arr == NULL || arr->GetCount() == 0);
        }
        break;
    default:
        isNull = mQueryResult->GetIsNull(pCol->idxStmt+1);
        break;
    }
    return isNull;
}

const FdoByte* FdoRdbmsSimpleFeatureReader::GetGeometry(FdoInt32 index, FdoInt32* count)
{
    return (const FdoByte*)FdoRdbmsSimpleFeatureReader::GetGeometry(index, count, false);
}

const void* FdoRdbmsSimpleFeatureReader::GetGeometry(FdoInt32 index, FdoInt32* count, bool noExcOnInvalid)
{
    ValidateIndex(index);
    if( ! mHasMoreRows )
        throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_62, _noMoreRows));

    GdbiColumnDetail* pCol = mColList[index];
    if (mGeomIdx != index)
    {
        FdoIGeometry* geom = NULL;
        bool isSupportedType = false;
        bool isNull = false;

        mGeomIdx = index;
        if (mWkbBuffer)
            *mWkbBuffer = 0;
        mQueryResult->GetBinaryValue (pCol->idxStmt+1, sizeof(FdoIGeometry *), (char*)&geom, &isNull, NULL);

        if ( !isNull && geom && geom->GetDerivedType() != FdoGeometryType_None )
            isSupportedType = true;

        if ( !isNull && geom != NULL )
        {
            if ( isSupportedType )
            {
                FdoPtr<FdoFgfGeometryFactory>  gf = FdoFgfGeometryFactory::GetInstance();
                FdoPtr<FdoByteArray> fgf = gf->GetFgf(geom);
                if (fgf != NULL && fgf->GetCount() != 0)
                {
                    mWkbGeomLen = fgf->GetCount();
                    if (mWkbBufferLen < mWkbGeomLen)
                    {
                        delete[] mWkbBuffer;
                        mWkbBufferLen = mWkbGeomLen;
                        mWkbBuffer = new unsigned char[mWkbGeomLen];
                    }
                    memcpy(mWkbBuffer, fgf->GetData(), mWkbGeomLen);
                }
                else
                    mWkbGeomLen = 0;
            }
            else
                mWkbGeomLen = -1;
        }
        else // isNull indicator is not set by dbi_get_val_b for geometry columns
            mWkbGeomLen = 0;
    }
    *count = mWkbGeomLen;
    if (*count <= 0)
    {
        if (noExcOnInvalid)
            return NULL;
        else if (*count == 0)
            throw FdoCommandException::Create(NlsMsgGet1( FDORDBMS_385, "Property '%1$ls' value is NULL; use IsNull method before trying to access the property value", pCol->col.c_alias ));
        else
            throw FdoCommandException::Create( NlsMsgGet(FDORDBMS_116, "Unsupported geometry type" ) );
    }

    return mWkbBuffer;
}

FdoByteArray* FdoRdbmsSimpleFeatureReader::GetGeometry(FdoInt32 index)
{
	int len = 0;
    const void* ptr = FdoRdbmsSimpleFeatureReader::GetGeometry(index, &len, false);
    return (len) ? FdoByteArray::Create((unsigned char*)ptr, len) : NULL;
}

FdoByteArray* FdoRdbmsSimpleFeatureReader::GetGeometry(FdoString* propertyName)
{
	int len = 0;
    const void* ptr = FdoRdbmsSimpleFeatureReader::GetGeometry(NameToIndex(propertyName), &len, false);
    return (len) ? FdoByteArray::Create((unsigned char*)ptr, len) : NULL;
}

const FdoByte* FdoRdbmsSimpleFeatureReader::GetGeometry(FdoString* propertyName, FdoInt32* count)
{
    return (const FdoByte*)FdoRdbmsSimpleFeatureReader::GetGeometry(NameToIndex(propertyName), count, false);
}

FdoIFeatureReader* FdoRdbmsSimpleFeatureReader::GetFeatureObject(FdoString* propertyName)
{
    return FdoRdbmsSimpleFeatureReader::GetFeatureObject(NameToIndex(propertyName));
}

FdoIFeatureReader* FdoRdbmsSimpleFeatureReader::GetFeatureObject(FdoInt32 index)
{
    throw FdoException::Create(L"GetFeatureObject() is not implemented by this provider.");
}

FdoIRaster* FdoRdbmsSimpleFeatureReader::GetRaster(FdoString* propertyName)
{
    throw FdoException::Create(L"GetRaster() is not implemented by this provider.");
}

FdoIRaster* FdoRdbmsSimpleFeatureReader::GetRaster(FdoInt32 index)
{
    throw FdoException::Create(L"GetRaster() is not implemented by this provider.");
}

bool FdoRdbmsSimpleFeatureReader::ReadNext()
{
    if( mQueryResult == NULL )
        throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_43, "Query ended"));

    mHasMoreRows = false;
    mGeomIdx = -1;

    if ( ! mQueryResult->ReadNext() )
    {
        Close();
        return false;
    }

 	for (int i=0; i < mColCount; i++)
		mSprops[i].valid = 0;

    return (mHasMoreRows = true);
}

void FdoRdbmsSimpleFeatureReader::Close()
{
    // Free all the cursors
    if ( mQueryResult != NULL )
    {
        mHasMoreRows = false;
        mQueryResult->Close();
        delete mQueryResult;
        mQueryResult = NULL;
    }
}