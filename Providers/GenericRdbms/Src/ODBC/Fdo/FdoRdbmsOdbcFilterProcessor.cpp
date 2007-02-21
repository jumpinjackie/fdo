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
#include "FdoRdbmsOdbcFilterProcessor.h"
#include "../../Fdo/Schema/FdoRdbmsSchemaUtil.h"
#include <Geometry/Fgf/Factory.h>

#include "SpatialManager/FdoRdbmsSpatialManager.h"
#include "FdoRdbmsOdbcConnection.h"
#include "FdoCommonOSUtil.h"


// This list includes all the ODBC aggregate functions. Not all functions are officially supported
// but they are not being prevented either.
static wchar_t* odbcAggregateFunctions[] = {
    L"AVG",   // supported
    L"COUNT", // supported
    L"MAX",   // supported
    L"MIN",   // supported
    L"SUM",   // supported
    NULL};

    
FdoRdbmsOdbcFilterProcessor::FdoRdbmsOdbcFilterProcessor(FdoRdbmsConnection *connection):
FdoRdbmsFilterProcessor( connection )
{
}

FdoRdbmsOdbcFilterProcessor::~FdoRdbmsOdbcFilterProcessor(void)
{
}

const FdoSmLpGeometricPropertyDefinition* FdoRdbmsOdbcFilterProcessor::GetGeometricProperty( const FdoSmLpClassDefinition* currentClass, const wchar_t *geomPropName )
{
    const FdoSmLpGeometricPropertyDefinition* geom = NULL;

	if( currentClass != NULL )
	{
        if ( geomPropName == NULL ) 
        {
            // No property name specified, default to the GeometryProperty
            const FdoSmLpFeatureClass* feat = 
                FdoSmLpFeatureClass::Cast(currentClass);

            if ( feat ) 
                geom = feat->RefGeometryProperty();
        }
        else {
            geom = FdoSmLpGeometricPropertyDefinition::Cast(currentClass->RefProperties()->RefItem(geomPropName));
        }
	}

	return geom;
}

/************************************************************************/
/* Get name of geometry column for the classname                                                                     */
/************************************************************************/
#define   NOTINDEXED_SUFFIX     L"_NIDX" 

FdoStringP FdoRdbmsOdbcFilterProcessor::GetGeometryColumnNameForProperty(const FdoSmLpGeometricPropertyDefinition* pGeomProp, bool bChangeFilter)
{
	FdoStringP retVal;

    if( pGeomProp != NULL )
    {
	    retVal = pGeomProp->GetColumnName();

   		if ( bChangeFilter) 
        {
            size_t szRetVal = retVal.GetLength();
            size_t szSuff = wcslen(NOTINDEXED_SUFFIX);

    	    FdoStringP suffixName;
            if (szRetVal > szSuff)
                suffixName = FdoStringP::Format( L"%ls", (FdoString *) retVal.Mid( szRetVal - szSuff, szRetVal));

            // Strip the suffix from the column name
            if ( suffixName.ICompare(NOTINDEXED_SUFFIX) == 0 )
                retVal = FdoStringP::Format(L"%ls", (FdoString *) retVal.Mid( 0, szRetVal - szSuff));
        }
	}

	return retVal;
}

/************************************************************************/
/* Process distance condition to emit spatial query string for direct   */
/* consumption by Odbc-SDO                                            */
/* Currently this function processes distance query and creates an SDO  */
/* query string that can be directly issued to a database using SQLPlus */
/************************************************************************/
void FdoRdbmsOdbcFilterProcessor::ProcessDistanceCondition(FdoDistanceCondition& filter)
{

}


/************************************************************************/
/* Process spatial condition to emit spatial query string for direct    */
/* consumption by Odbc                                            */
/************************************************************************/
void FdoRdbmsOdbcFilterProcessor::ProcessSpatialCondition(FdoSpatialCondition& filter)
{
    DbiConnection  *mDbiConnection = mFdoConnection->GetDbiConnection();
    const FdoSmLpClassDefinition *classDefinition = mDbiConnection->GetSchemaUtil()->GetClass(mCurrentClassName);
    if ( classDefinition == NULL ||  classDefinition->GetClassType() != FdoClassType_FeatureClass )
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_230, "Spatial condition can only be used with feature classes"));

    const FdoSmLpGeometricPropertyDefinition* geomProp = GetGeometricProperty(classDefinition, FdoPtr<FdoIdentifier>(filter.GetPropertyName())->GetName());
    const FdoString* classTableName = classDefinition->GetDbObjectName();
    const FdoString* tableName = geomProp ? geomProp->GetContainingDbObjectName() : L""; // The geometry table name
    FdoStringP columnName = GetGeometryColumnNameForProperty(geomProp, true);
    FdoStringP columnName2 = GetGeometryColumnNameForProperty(geomProp, false);

    FdoStringP spatialClause;
    FdoPtr<FdoGeometryValue> geom = dynamic_cast<FdoGeometryValue*>(filter.GetGeometry());
    FdoPtr<FdoByteArray>     geomFgf;
    FdoPtr<FdoIGeometry>     geometryObj;

    geomFgf = geom->GetGeometry();

    if (geomFgf == NULL)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_46, "No geometry value"));

    // Geometry factory
    FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();

    geometryObj = gf->CreateGeometryFromFgf(geomFgf);

    FdoGeometryType gType = geometryObj->GetDerivedType();

    FdoRdbmsSpatialManagerP spatialManager = this->mFdoConnection->GetSpatialManager();
    FdoRdbmsSpatialSqlFilterP spatialSqlFilter = spatialManager->GetSqlFilter(geomProp, &filter);
    if (NULL == spatialSqlFilter.p)
        AppendString("1=1");    // No spatial index.
    else
        AppendString(spatialSqlFilter->FilterToSql());
}

void FdoRdbmsOdbcFilterProcessor::AppendTablesHints( SqlCommandType cmdType, bool forUpdate )
{

    if( cmdType == SqlCommandType_Select && forUpdate )
    {
        AppendString(" with (UPDLOCK) ");
    }

    //... other cases here.
}

bool FdoRdbmsOdbcFilterProcessor::IsAggregateFunctionName(FdoString* wFunctionName) const
{
    for(int i=0; odbcAggregateFunctions[i]; i++)
        if( FdoCommonOSUtil::wcsicmp(odbcAggregateFunctions[i], wFunctionName) == 0 )
            return true;

    return false;
}

void FdoRdbmsOdbcFilterProcessor::ProcessDateTimeValue(FdoDateTimeValue& expr)
{
    // NOTE: we don't place single quotes around the time/date/timestamp, contrary to the parent class' behavior:
    AppendString( ((FdoRdbmsOdbcConnection*)mFdoConnection)->FdoToDbiTimeFilter( expr.GetDateTime() ) );
}

const wchar_t* FdoRdbmsOdbcFilterProcessor::FilterToSql( 
    FdoFilter  *filter, 
    const wchar_t *className, 
    SqlCommandType cmdType, 
    FdoCommandType callerFdoCommand, 
    FdoRdbmsFilterUtilConstrainDef *inFilterConstrain,
    bool forUpdate,
    FdoInt16 callerId )
{
    const wchar_t * ret = NULL;

    // If no properties are selected (meaning the user wants them all),
    // specify all of them.  This avoids a translation of "select * ...",
    // which can fail if there are columns of unsupported type.

    FdoRdbmsFilterUtilConstrainDef    filterConst;
    FdoRdbmsFilterUtilConstrainDef   *filterConstraint;
    if( inFilterConstrain != NULL )
        filterConstraint = inFilterConstrain;
    else
        filterConstraint = &filterConst;

    bool addedPropDefs = false;
    FdoPtr<FdoIdentifierCollection> allIds;

    if( SqlCommandType_Select == cmdType && NULL == filterConstraint->selectedProperties )
    {
        allIds = FdoIdentifierCollection::Create();
        DbiConnection  *mDbiConnection = mFdoConnection->GetDbiConnection();
        const FdoSmLpClassDefinition *classDef = mDbiConnection->GetSchemaUtil()->GetClass(className);
        const FdoSmLpPropertyDefinitionCollection* propDefs = classDef->RefProperties();
        FdoInt32 numProps = propDefs->GetCount();
        for (FdoInt32 i=0;  i < numProps;  i++)
        {
            const FdoSmLpPropertyDefinition * propDef = propDefs->RefItem(i);
            FdoPtr<FdoIdentifier> newId = FdoIdentifier::Create(propDef->GetName());
            allIds->Add(newId);
        }
        filterConstraint->selectedProperties = allIds;
        addedPropDefs = true;
    }

    ret = FdoRdbmsFilterProcessor::FilterToSql(filter, className, cmdType, callerFdoCommand, filterConstraint, forUpdate, callerId);

    if (addedPropDefs)
        filterConstraint->selectedProperties = NULL;

    return ret;
}

