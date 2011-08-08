/*
 * Copyright (C) 2004-2008  Autodesk, Inc.
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
#include "FdoRdbmsSqlServerFilterProcessor.h"
#include "../../Fdo/Schema/FdoRdbmsSchemaUtil.h"
#include <Geometry/Fgf/Factory.h>

#include "SpatialManager/FdoRdbmsSpatialManager.h"
#include "FdoRdbmsSqlServerConnection.h"
#include "FdoCommonOSUtil.h"
#include "FdoRdbmsFunctionIsValid.h"
#include "../SchemaMgr/Ph/SpatialIndex.h"
#include "../SchemaMgr/Ph/Mgr.h"
#include "FdoRdbmsSqlServerDateTimeUtils.h"
#include <ctype.h>

// This list includes all the SQL Server aggregate functions. Not all functions are officially supported
// but they are not being prevented either.
static wchar_t* sqlServerAggregateFunctions[] = {
    L"AVG",   // supported
    L"BINARY_CHECKSUM",
    L"CHECKSUM",
    L"CHECKSUM_AGG",
    L"COUNT", // supported
    L"COUNT_BIG",
    L"GROUPING",
    L"MAX", // supported
    L"MEDIAN", // supported
    L"MIN", // supported
    L"SUM", // supported
    L"STDEV",
    L"STDEVP",
    L"VAR",
	L"VARP",
    NULL};

// FDO's list of expression function includes functions that do not have native
// support in SQL Server. In case such a function is used, the expression cannot
// be handled by sending it to SQL Server. Instead it needs to be handled in the
// Expression Engine.
//

static wchar_t* sqlServerUnsupportedFdoFunctions[] = {

    L"MEDIAN",
    L"TRANSLATE",
    NULL
};

FdoRdbmsSqlServerFilterProcessor::FdoRdbmsSqlServerFilterProcessor(FdoRdbmsConnection *connection):
FdoRdbmsFilterProcessor( connection )
{
}

FdoRdbmsSqlServerFilterProcessor::~FdoRdbmsSqlServerFilterProcessor(void)
{
}

bool FdoRdbmsSqlServerFilterProcessor::SupportsSpatialOrNonSpatialOperator()
{
    return true;
}

const FdoSmLpGeometricPropertyDefinition* FdoRdbmsSqlServerFilterProcessor::GetGeometricProperty( const FdoSmLpClassDefinition* currentClass, const wchar_t *geomPropName ) const
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

FdoStringP FdoRdbmsSqlServerFilterProcessor::GetGeometryColumnNameForProperty(const FdoSmLpGeometricPropertyDefinition* pGeomProp, bool bChangeFilter)
{
	FdoStringP retVal;

    if( pGeomProp != NULL )
    {
	    retVal = pGeomProp->GetColumnName();

   		if ( bChangeFilter) 
        {
            FdoStringP  tmpRetVal(retVal);

    	    FdoStringP suffixName = FdoStringP::Format( L"%ls",
		        	    (FdoString *) tmpRetVal.Mid( wcslen(retVal) - wcslen(NOTINDEXED_SUFFIX), wcslen(retVal)));

            // Strip the suffix from the column name
            if ( suffixName.ICompare(NOTINDEXED_SUFFIX) == 0 )
            {
                retVal = FdoStringP::Format(L"%ls", (FdoString *) tmpRetVal.Mid( 0, wcslen(retVal) - wcslen(NOTINDEXED_SUFFIX)));
            }
        }
	}

	return retVal;
}

/************************************************************************/
/* Process distance condition to emit spatial query string for direct   */
/* consumption by SqlServer-SDO                                            */
/* Currently this function processes distance query and creates an SDO  */
/* query string that can be directly issued to a database using SQLPlus */
/************************************************************************/
void FdoRdbmsSqlServerFilterProcessor::ProcessDistanceCondition(FdoDistanceCondition& filter)
{

}


/************************************************************************/
/* Process spatial condition to emit spatial query string for direct    */
/* consumption by SqlServer                                            */
/************************************************************************/
void FdoRdbmsSqlServerFilterProcessor::ProcessSpatialCondition(FdoSpatialCondition& filter)
{
    DbiConnection  *mDbiConnection = mFdoConnection->GetDbiConnection();
    const FdoSmLpClassDefinition *classDefinition = mDbiConnection->GetSchemaUtil()->GetClass(mCurrentClassName);
    if ( classDefinition == NULL ||  classDefinition->GetClassType() != FdoClassType_FeatureClass )
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_230, "Spatial condition can only be used with feature classes"));

    const FdoSmLpGeometricPropertyDefinition* geomProp = GetGeometricProperty(classDefinition, FdoPtr<FdoIdentifier>(filter.GetPropertyName())->GetName());
    const FdoSmPhColumn* geomColumn = geomProp ? geomProp->RefColumn() : (const FdoSmPhColumn*) NULL;
    FdoStringP geomType = geomColumn ? geomColumn->GetTypeName() : FdoStringP(L"geometry");
    const FdoString* classTableName = classDefinition->GetDbObjectName();
    const FdoString* tableName = geomProp ? geomProp->GetContainingDbObjectName() : L""; // The geometry table name
    FdoStringP columnName = GetGeometryColumnNameForProperty(geomProp, true);
    FdoStringP columnName2 = GetGeometryColumnNameForProperty(geomProp, false);

    FdoStringP spatialClause;
    FdoPtr<FdoExpression> geomExpr = filter.GetGeometry();
    FdoGeometryValue *geom = dynamic_cast<FdoGeometryValue*>(geomExpr.p);

    FdoStringP buf(L"");

    // Delimit column name with []. Can't use " when part of function.
    buf += "[";
    buf += columnName;
    buf += "].";

    // What operation
    FdoSpatialOperations  spatialOp = filter.GetOperation();

    // Geography type does not have an STIsValid() function.
    if ( (geomType == L"geometry") && (spatialOp != FdoSpatialOperations_EnvelopeIntersects)) 
    {
        // Skip the invalid geometries otherwise any spatial query will fail.
        // The user should run "Select * where IsValid() = 0" in order to find out the offending geometries.
        buf += SQLSERVER_FUNCTION_ISVALID; 
        buf += L"() = 1 AND [";
        buf += columnName;
        buf += "].";
    }

    if ( geomType == L"geography" ) 
    {
        switch( spatialOp )
        {
            case FdoSpatialOperations_Disjoint:
                buf += "STDisjoint";
                break;
            case FdoSpatialOperations_Intersects:
                buf += "STIntersects";
                break;
            case FdoSpatialOperations_Contains:
            case FdoSpatialOperations_Crosses:
            case FdoSpatialOperations_CoveredBy:
            case FdoSpatialOperations_Equals:
            case FdoSpatialOperations_Inside:
            case FdoSpatialOperations_Overlaps:
            case FdoSpatialOperations_Touches:
            case FdoSpatialOperations_Within:
            case FdoSpatialOperations_EnvelopeIntersects:
                throw FdoFilterException::Create(
                    NlsMsgGet2(
                        FDORDBMS_44, 
                        "Geometry property '%1$ls' has geodetic coordinate system; cannot use %2$ls spatial operator in filter",
                        (FdoString*) geomProp->GetQName(),
                        (FdoString*) FdoCommonMiscUtil::FdoSpatialOperationsToString(spatialOp)
                    )
                );
                break;
            default:
                throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_111, "Unsupported spatial operation"));
        } // of switch Operation
    }
    else
    {
        switch( spatialOp )
        {
            case FdoSpatialOperations_Contains:
                buf += "STContains";
                break;
            case FdoSpatialOperations_Crosses:
                buf += "STCrosses";
                break;
            case FdoSpatialOperations_Disjoint:
                buf += "STDisjoint";
                break;
            case FdoSpatialOperations_Equals:
                buf += "STEquals";
                break;
            case FdoSpatialOperations_Intersects:
                buf += "STIntersects";
                break;
            case FdoSpatialOperations_Overlaps:
                buf += "STOverlaps";
                break;
            case FdoSpatialOperations_Touches:
                buf += "STTouches";
                break;
            case FdoSpatialOperations_Within:
                buf += "STWithin";
                break;
            case FdoSpatialOperations_Inside:
                buf += "STWithin";  // REALLY?
                break;
            case FdoSpatialOperations_CoveredBy:
                buf += "STOverlaps"; // REALLY?
                break;
            case FdoSpatialOperations_EnvelopeIntersects:
                buf += "Filter"; 
                break;
            default:
                throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_111, "Unsupported spatial operation"));
        } // of switch Operation
    }

    // Set the SRID
	const FdoSmPhColumnP gColumn = ((FdoSmLpSimplePropertyDefinition*)geomProp)->GetColumn();
    FdoSmPhColumnGeomP geomCol = gColumn.p->SmartCast<FdoSmPhColumnGeom>();
    mUsedParameterValues.push_back(std::make_pair(geom, geomCol->GetSRID()));

    buf += "(?)=1";


	if ( spatialOp == FdoSpatialOperations_EnvelopeIntersects)
	{
        mUsedParameterValues.push_back(std::make_pair(geom, geomCol->GetSRID()));
		buf += L" AND [";
		buf += columnName;
		buf += "].MakeValid().STEnvelope().STIntersects";
		buf += "(?)=1";

		// store only the first spatial index name
		if (mSpatialIndexName == L"")
		{
			FdoPtr<FdoSmPhSpatialIndex> si = geomCol->GetSpatialIndex();
			mSpatialIndexName = si->GetName();
		}
	}

    AppendString((const wchar_t*)buf);
}

void FdoRdbmsSqlServerFilterProcessor::AppendTablesHints( SqlCommandType cmdType, bool forUpdate )
{

	FdoStringP buf(L"");
	bool bAdded = false;

    if( cmdType == SqlCommandType_Select && forUpdate )
    {
		buf = L" with (UPDLOCK ";
		bAdded = true;
    }

    if(mSpatialIndexName != L"")
    {
		if (bAdded)
			buf += L", INDEX(";
		else
			buf =  L" with (INDEX(";
		buf += "[";
		buf += mSpatialIndexName;
		buf += L"]) ";
		bAdded = true;
		mSpatialIndexName = "";
    }

	if (bAdded)
	{
		buf += L")";
        PrependString((FdoString *)buf);
	}
}


void FdoRdbmsSqlServerFilterProcessor::ProcessFunction(FdoFunction& expr)
{
    FdoString *funcName = expr.GetName();

    // If the function is one of aggregate functions AVG, COUNT, MAX, MIN,
    // STDDEV or SUM special processing is required.

    if ((FdoCommonOSUtil::wcsicmp(funcName, FDO_FUNCTION_AVG   ) == 0) ||
        (FdoCommonOSUtil::wcsicmp(funcName, FDO_FUNCTION_COUNT ) == 0) ||
        (FdoCommonOSUtil::wcsicmp(funcName, FDO_FUNCTION_MAX   ) == 0) ||
        (FdoCommonOSUtil::wcsicmp(funcName, FDO_FUNCTION_MIN   ) == 0) ||
        (FdoCommonOSUtil::wcsicmp(funcName, FDO_FUNCTION_STDDEV) == 0) ||
        (FdoCommonOSUtil::wcsicmp(funcName, FDO_FUNCTION_SUM   ) == 0)    )
        return ProcessAggregateFunction(expr);

    if (FdoCommonOSUtil::wcsicmp(funcName, FDO_FUNCTION_TODOUBLE) == 0)
        return ProcessToDoubleFunction(expr);

    if (FdoCommonOSUtil::wcsicmp(funcName, FDO_FUNCTION_TOFLOAT) == 0)
        return ProcessToFloatFunction(expr);

    if ((FdoCommonOSUtil::wcsicmp(funcName, FDO_FUNCTION_TOINT32) == 0) ||
        (FdoCommonOSUtil::wcsicmp(funcName, FDO_FUNCTION_TOINT64) == 0)    )
        return ProcessToIntFunction(expr);

    if (FdoCommonOSUtil::wcsicmp(funcName, FDO_FUNCTION_ADDMONTHS) == 0)
        return ProcessAddMonthsFunction(expr);

    if (FdoCommonOSUtil::wcsicmp(funcName, FDO_FUNCTION_MONTHSBETWEEN) == 0)
        return ProcessMonthsBetweenFunction(expr);

    if (FdoCommonOSUtil::wcsicmp(funcName, FDO_FUNCTION_LOG) == 0)
        return ProcessLogFunction(expr);

    if (FdoCommonOSUtil::wcsicmp(funcName, FDO_FUNCTION_ROUND) == 0)
        return ProcessRoundFunction(expr);

    if (FdoCommonOSUtil::wcsicmp(funcName, FDO_FUNCTION_CONCAT) == 0)
        return ProcessConcatFunction(expr);

    if (FdoCommonOSUtil::wcsicmp(funcName, FDO_FUNCTION_SPATIALEXTENTS) == 0)
        return ProcessSpatialExtentsFunction(expr);

    if (FdoCommonOSUtil::wcsicmp(funcName, FDORDBMS_FUNCTION_ISVALID) == 0)
        return ProcessIsValidFunction(expr);

    if (FdoCommonOSUtil::wcsicmp(funcName, FDO_FUNCTION_Z) == 0)
        return ProcessZMFunction(expr);

    if (FdoCommonOSUtil::wcsicmp(funcName, FDO_FUNCTION_M) == 0)
        return ProcessZMFunction(expr);

    if (FdoCommonOSUtil::wcsicmp(funcName, FDO_FUNCTION_MOD) == 0)
        return ProcessModFunction(expr);

    if (FdoCommonOSUtil::wcsicmp(funcName, FDO_FUNCTION_TRUNC) == 0)
        return ProcessTruncFunction(expr);

    if (FdoCommonOSUtil::wcsicmp(funcName, FDO_FUNCTION_INSTR) == 0)
        return ProcessInstrFunction(expr);

    if (FdoCommonOSUtil::wcsicmp(funcName, FDO_FUNCTION_SUBSTR) == 0)
        return ProcessSubStrFunction(expr);

    if (FdoCommonOSUtil::wcsicmp(funcName, FDO_FUNCTION_TRIM) == 0)
        return ProcessTrimFunction(expr);

    if (FdoCommonOSUtil::wcsicmp(funcName, FDO_FUNCTION_REMAINDER) == 0)
        return ProcessRemainderFunction(expr);

    if (FdoCommonOSUtil::wcsicmp(funcName, FDO_FUNCTION_LPAD) == 0)
        return ProcessLpadFunction(expr);

    if (FdoCommonOSUtil::wcsicmp(funcName, FDO_FUNCTION_RPAD) == 0)
        return ProcessRpadFunction(expr);

    if (FdoCommonOSUtil::wcsicmp(funcName, FDO_FUNCTION_EXTRACTTOINT) == 0)
        return ProcessExtractToIntFunction(expr);

    if (FdoCommonOSUtil::wcsicmp(funcName, FDO_FUNCTION_EXTRACTTODOUBLE) == 0)
        return ProcessExtractToDblFunction(expr);

    if (FdoCommonOSUtil::wcsicmp(funcName, FDO_FUNCTION_EXTRACT) == 0)
        return ProcessExtractFunction(expr);

    if (FdoCommonOSUtil::wcsicmp(funcName, FDO_FUNCTION_TOSTRING) == 0)
        return ProcessToStringFunction(expr);

    if (FdoCommonOSUtil::wcsicmp(funcName, FDO_FUNCTION_X) == 0)
        return ProcessXFunction(expr);

    if (FdoCommonOSUtil::wcsicmp(funcName, FDO_FUNCTION_Y) == 0)
        return ProcessYFunction(expr);

    if (FdoCommonOSUtil::wcsicmp(funcName, FDO_FUNCTION_AREA2D) == 0)
        return ProcessArea2dFunction(expr);

    if (FdoCommonOSUtil::wcsicmp(funcName, FDO_FUNCTION_LENGTH2D) == 0)
        return ProcessLength2dFunction(expr);

    if (FdoCommonOSUtil::wcsicmp(funcName, FDO_FUNCTION_TODATE) == 0)
        return ProcessToDateFunction(expr);

    //if (FdoCommonOSUtil::wcsicmp(funcName, FDO_FUNCTION_MEDIAN) == 0)
    //    return ProcessMedianFunction(expr);

    // The functions that do not require special handling use the
    // standard processing
    FdoRdbmsFilterProcessor::ProcessFunction(expr);
}

void FdoRdbmsSqlServerFilterProcessor::ProcessToDateFunction(FdoFunction& expr)
{
    FdoPtr<FdoExpressionCollection> exprCol = expr.GetArguments();
    FdoInt32 colCount = exprCol->GetCount();
    if (colCount != 1 && colCount != 2)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));

    // later we need to call expression->Process()->ToString()
    // for now we support only a few cases
    FdoPtr<FdoExpression> exp0 = exprCol->GetItem(0);
    FdoExpressionItemType type = exp0->GetExpressionType();
    if (FdoExpressionItemType_DataValue == type)
    {
        FdoDataValue* dv = static_cast<FdoDataValue*>(exp0.p);
        if (dv == NULL || dv->IsNull() || dv->GetDataType() != FdoDataType_String)
            throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));
    }
    else
        if (FdoExpressionItemType_Function != type && FdoExpressionItemType_Identifier != type)
            throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));

    DateTokenFormat tf;
    bool processed = false;
    FdoString* sql = NULL;
    if (colCount == 2)
    {
        FdoPtr<FdoExpression> exp1 = exprCol->GetItem(1);
        if (FdoExpressionItemType_DataValue == exp1->GetExpressionType())
        {
            FdoDataValue* dv = static_cast<FdoDataValue*>(exp1.p);
            if (dv != NULL && dv->GetDataType() == FdoDataType_String)
            {
                FdoStringValue* strVal = static_cast<FdoStringValue*>(dv);
                if (strVal != NULL && !strVal->IsNull())
                {
                    tf.ProcessFormat(strVal->GetString());
                    sql = tf.ToDateTimeSQL(exp0->ToString());
                    processed = true;
                }
            }
        }
    }
    else
    {
        tf.ProcessFormat(NULL);
        sql = tf.ToDateTimeSQL(exp0->ToString());
        processed = true;
    }

    if (!processed || !sql)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));
    AppendString(sql);
}

void FdoRdbmsSqlServerFilterProcessor::ProcessMedianFunction(FdoFunction& expr)
{
    FdoPtr<FdoExpressionCollection> exprCol = expr.GetArguments();
    if (exprCol->GetCount() != 1)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));
    
    // Get the class definition for the identified class.
    DbiConnection *mDbiConnection = mFdoConnection->GetDbiConnection();
    const FdoSmLpClassDefinition *classDefinition = mDbiConnection->GetSchemaUtil()->GetClass(mCurrentClassName);
    FdoStringP tableName = mDbiConnection->GetSchemaUtil()->GetDbObjectSqlName(classDefinition);

    FdoPtr<FdoExpression> exp = exprCol->GetItem(0);
    AppendString(L"select max(medianVal) from (select distinct top 50 percent ");
    HandleExpr(exp);
    AppendString(L" as medianVal from ");
    AppendString((FdoString *)tableName);
    AppendString(L" as ");
    AppendString(GetTableAlias(tableName));
    AppendString(L" where ");
    HandleExpr(exp);
    AppendString(L" IS NOT NULL Order By ");
    HandleExpr(exp);
    AppendString(L") as medianTb");
}

void FdoRdbmsSqlServerFilterProcessor::ProcessArea2dFunction(FdoFunction& expr)
{
    FdoPtr<FdoExpressionCollection> exprCol = expr.GetArguments();
    if (exprCol->GetCount() != 1)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));
    FdoPtr<FdoIdentifier>   id = (FdoIdentifier *)(exprCol->GetItem(0));

    AppendString(L"\"");
    AppendString( PropertyNameToColumnName( id->GetName() ) );
    AppendString(L"\".");
    AppendString(L"STArea()");
}

void FdoRdbmsSqlServerFilterProcessor::ProcessLength2dFunction(FdoFunction& expr)
{
    FdoPtr<FdoExpressionCollection> exprCol = expr.GetArguments();
    if (exprCol->GetCount() != 1)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));
    FdoPtr<FdoIdentifier>   id = (FdoIdentifier *)(exprCol->GetItem(0));

    AppendString(L"\"");
    AppendString( PropertyNameToColumnName( id->GetName() ) );
    AppendString(L"\".");
    AppendString(L"STLength()");
}

void FdoRdbmsSqlServerFilterProcessor::ProcessXFunction(FdoFunction& expr)
{
    FdoPtr<FdoExpressionCollection> exprCol = expr.GetArguments();
    if (exprCol->GetCount() != 1)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));
    FdoPtr<FdoIdentifier>   id = (FdoIdentifier *)(exprCol->GetItem(0));

    DbiConnection  *mDbiConnection = mFdoConnection->GetDbiConnection();
    const FdoSmLpClassDefinition *classDefinition = mDbiConnection->GetSchemaUtil()->GetClass(mCurrentClassName);
    if ( classDefinition == NULL ||  classDefinition->GetClassType() != FdoClassType_FeatureClass )
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_230, "Spatial condition can only be used with feature classes"));

    const FdoSmLpGeometricPropertyDefinition* geomProp = GetGeometricProperty(classDefinition, id->GetName());
    const FdoSmPhColumn* geomColumn = geomProp ? geomProp->RefColumn() : (const FdoSmPhColumn*) NULL;
    FdoStringP geomType = geomColumn ? geomColumn->GetTypeName() : FdoStringP(L"geometry");
    
    AppendString(L"\"");
    AppendString( PropertyNameToColumnName( id->GetName() ) );
    AppendString(L"\".");
    if (geomType == L"geometry")
        AppendString(L"STX");
    else
        AppendString(L"Long");
}

void FdoRdbmsSqlServerFilterProcessor::ProcessYFunction(FdoFunction& expr)
{
    FdoPtr<FdoExpressionCollection> exprCol = expr.GetArguments();
    if (exprCol->GetCount() != 1)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));
    FdoPtr<FdoIdentifier>   id = (FdoIdentifier *)(exprCol->GetItem(0));

    DbiConnection  *mDbiConnection = mFdoConnection->GetDbiConnection();
    const FdoSmLpClassDefinition *classDefinition = mDbiConnection->GetSchemaUtil()->GetClass(mCurrentClassName);
    if ( classDefinition == NULL ||  classDefinition->GetClassType() != FdoClassType_FeatureClass )
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_230, "Spatial condition can only be used with feature classes"));

    const FdoSmLpGeometricPropertyDefinition* geomProp = GetGeometricProperty(classDefinition, id->GetName());
    const FdoSmPhColumn* geomColumn = geomProp ? geomProp->RefColumn() : (const FdoSmPhColumn*) NULL;
    FdoStringP geomType = geomColumn ? geomColumn->GetTypeName() : FdoStringP(L"geometry");
    
    AppendString(L"\"");
    AppendString( PropertyNameToColumnName( id->GetName() ) );
    AppendString(L"\".");
    if (geomType == L"geometry")
        AppendString(L"STY");
    else
        AppendString(L"Lat");
}

void FdoRdbmsSqlServerFilterProcessor::ProcessToStringFunction(FdoFunction& expr)
{
    FdoPtr<FdoExpressionCollection> exprCol = expr.GetArguments();
    FdoInt32 colCount = exprCol->GetCount();
    if (colCount != 1 && colCount != 2)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));

    FdoPtr<FdoExpression> exp0 = exprCol->GetItem(0);
    if (colCount == 2) // date->ToString()?
    {
        FdoPtr<FdoExpression> exp1 = exprCol->GetItem(1);
        bool processed = false;
        DateTokenFormat tf;
        if (FdoExpressionItemType_DataValue == exp1->GetExpressionType())
        {
            FdoDataValue* dv = static_cast<FdoDataValue*>(exp1.p);
            if (dv != NULL && dv->GetDataType() == FdoDataType_String)
            {
                FdoStringValue* strVal = static_cast<FdoStringValue*>(dv);
                if (strVal != NULL && !strVal->IsNull())
                {
                    FdoString* format = strVal->GetString();
                    tf.ProcessFormat(format);
                    processed = true;
                }
            }
        }
        // later we need to call expression->Process()->ToString()
        // for now we support only a few cases
        FdoString* sql = NULL;
        if (FdoExpressionItemType_Function == exp0->GetExpressionType())
            sql = tf.ToSQL(exp0->ToString());
        else if (FdoExpressionItemType_Identifier == exp0->GetExpressionType())
            sql = tf.ToSQL(PropertyNameToColumnName(exp0->ToString()));
        if (!processed || !sql)
            throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));
        AppendString(sql);
    }
    else
    {
        FdoPropertyType propType = FdoPropertyType_DataProperty;
        FdoDataType dataType = FdoDataType_Int32;
        FdoPtr<FdoClassDefinition> clsDef = mFdoConnection->GetClassDefinition(mCurrentClassName);
        FdoPtr<FdoIExpressionCapabilities> expressionCaps = mFdoConnection->GetExpressionCapabilities();
        FdoPtr<FdoFunctionDefinitionCollection> functions = expressionCaps->GetFunctions();
        try
        {
            FdoCommonMiscUtil::GetExpressionType(functions, clsDef, exp0, propType, dataType);
        }
        catch(FdoExpression* ex) { ex->Release(); }

        if (dataType == FdoDataType_DateTime)
        {
            DateTokenFormat tf;
            tf.ProcessFormat(NULL);
            // later we need to call expression->Process()->ToString()
            // for now we support only a few cases
            FdoString* sql = NULL;
            if (FdoExpressionItemType_Function == exp0->GetExpressionType())
                sql = tf.ToSQL(exp0->ToString());
            else if (FdoExpressionItemType_Identifier == exp0->GetExpressionType())
                sql = tf.ToSQL(PropertyNameToColumnName(exp0->ToString()));
            if (!sql)
                throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));
            AppendString(sql);
        }
        else
        {
            AppendString(L"convert(nvarchar,");
            HandleExpr(exp0);
            AppendString(L")");
        }
    }
}

void FdoRdbmsSqlServerFilterProcessor::ProcessExtractFunction(FdoFunction& expr)
{
    // Note: since we cannot generate invalid date time values, we will preserve the wanted value 
    // and tuncate the others except the year (to have a valid date), and caller can get the wanted value.
    FdoPtr<FdoExpressionCollection> exprCol = expr.GetArguments();
    if (exprCol->GetCount() != 2)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));

    FdoPtr<FdoExpression> exp0 = exprCol->GetItem(0);
    FdoPtr<FdoExpression> exp1 = exprCol->GetItem(1);

    bool processed = false;
    FdoPropertyType propType = FdoPropertyType_DataProperty;
    FdoDataType dataType = FdoDataType_Int32;
    FdoPtr<FdoClassDefinition> clsDef = mFdoConnection->GetClassDefinition(mCurrentClassName);
    FdoPtr<FdoIExpressionCapabilities> expressionCaps = mFdoConnection->GetExpressionCapabilities();
    FdoPtr<FdoFunctionDefinitionCollection> functions = expressionCaps->GetFunctions();
    try
    {
        FdoCommonMiscUtil::GetExpressionType(functions, clsDef, exp1, propType, dataType);
    }
    catch(FdoExpression* ex) { ex->Release(); }

    if (dataType != FdoDataType_DateTime)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));

    if (FdoExpressionItemType_DataValue == exp0->GetExpressionType())
    {
        FdoDataValue* dv = static_cast<FdoDataValue*>(exp0.p);
        if (dv != NULL && dv->GetDataType() == FdoDataType_String)
        {
            FdoStringValue* strVal = static_cast<FdoStringValue*>(dv);
            if (strVal != NULL && !strVal->IsNull())
            {
                FdoString* oper = strVal->GetString();
                if (wcscmp(oper, L"YEAR") == 0)
                {
                    processed = true;
                    AppendString(L"cast(DATEPART(YEAR,");
                    HandleExpr(exp1);
                    AppendString(L") as nvarchar) + N'-01-01 00:00:00.000'");
                }
                else if (wcscmp(oper, L"MONTH") == 0)
                {
                    processed = true;
                    AppendString(L"cast(DATEPART(YEAR,");
                    HandleExpr(exp1);
                    AppendString(L") as nvarchar) + N'-' + cast(DATEPART(MONTH,");
                    HandleExpr(exp1);
                    AppendString(L") as nvarchar) + N'-01 00:00:00.000'");
                }
                else if (wcscmp(oper, L"DAY") == 0)
                {
                    processed = true;
                    AppendString(L"cast(DATEPART(YEAR,");
                    HandleExpr(exp1);
                    AppendString(L") as nvarchar) + N'-01-' + cast(DATEPART(DAY,");
                    HandleExpr(exp1);
                    AppendString(L") as nvarchar) + N' 00:00:00.000'");
                }
                else if (wcscmp(oper, L"HOUR") == 0)
                {
                    processed = true;
                    AppendString(L"cast(DATEPART(YEAR,");
                    HandleExpr(exp1);
                    AppendString(L") as nvarchar) + N'-01-01 ' + cast(DATEPART(HOUR,");
                    HandleExpr(exp1);
                    AppendString(L") as nvarchar) + N':00:00.000'");
                }
                else if (wcscmp(oper, L"MINUTE") == 0)
                {
                    processed = true;
                    AppendString(L"cast(DATEPART(YEAR,");
                    HandleExpr(exp1);
                    AppendString(L") as nvarchar) + N'-01-01 00:' + cast(DATEPART(MINUTE,");
                    HandleExpr(exp1);
                    AppendString(L") as nvarchar) + N':00.000'");
                }
                else if (wcscmp(oper, L"SECOND") == 0)
                {
                    processed = true;
                    AppendString(L"cast(DATEPART(YEAR,");
                    HandleExpr(exp1);
                    AppendString(L") as nvarchar) + N'-01-01 00:00:' + cast((DATEPART(SECOND,");
                    HandleExpr(exp1);
                    AppendString(L")+DATEPART(MILLISECOND,");
                    HandleExpr(exp1);
                    AppendString(L")/1000.0) as nvarchar)");
                }
            }
        }
    }
    else
    {
        AppendString(L"case when ");
        HandleExpr(exp0);
        AppendString(L"=N'YEAR' then cast(DATEPART(YEAR,");
        HandleExpr(exp1);
        AppendString(L") as nvarchar) + N'-01-01 00:00:00.000'");
        AppendString(L" when ");
        HandleExpr(exp0);
        AppendString(L"=N'MONTH' then cast(DATEPART(YEAR,");
        HandleExpr(exp1);
        AppendString(L") as nvarchar) + N'-' + cast(DATEPART(MONTH,");
        HandleExpr(exp1);
        AppendString(L") as nvarchar) + N'-01 00:00:00.000'");
        AppendString(L" when ");
        HandleExpr(exp0);
        AppendString(L"=N'DAY' then cast(DATEPART(YEAR,");
        HandleExpr(exp1);
        AppendString(L") as nvarchar) + N'-01-' + cast(DATEPART(DAY,");
        HandleExpr(exp1);
        AppendString(L") as nvarchar) + N' 00:00:00.000'");
        AppendString(L" when ");
        HandleExpr(exp0);
        AppendString(L"=N'HOUR' then cast(DATEPART(YEAR,");
        HandleExpr(exp1);
        AppendString(L") as nvarchar) + N'-01-01 ' + cast(DATEPART(HOUR,");
        HandleExpr(exp1);
        AppendString(L") as nvarchar) + N':00:00.000'");
        AppendString(L" when ");
        HandleExpr(exp0);
        AppendString(L"=N'MINUTE' then cast(DATEPART(YEAR,");
        HandleExpr(exp1);
        AppendString(L") as nvarchar) + N'-01-01 00:' + cast(DATEPART(MINUTE,");
        HandleExpr(exp1);
        AppendString(L") as nvarchar) + N':00.000'");
        AppendString(L" when ");
        HandleExpr(exp0);
        AppendString(L"=N'SECOND' then cast(DATEPART(YEAR,");
        HandleExpr(exp1);
        AppendString(L") as nvarchar) + N'-01-01 00:00:' + cast((DATEPART(SECOND,");
        HandleExpr(exp1);
        AppendString(L")+DATEPART(MILLISECOND,");
        HandleExpr(exp1);
        AppendString(L")/1000.0) as nvarchar)");
        AppendString(L" end ");
        processed = true;
    }
    if (!processed)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));
}

void FdoRdbmsSqlServerFilterProcessor::ProcessExtractToIntFunction(FdoFunction& expr)
{
    FdoPtr<FdoExpressionCollection> exprCol = expr.GetArguments();
    if (exprCol->GetCount() != 2)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));

    FdoPtr<FdoExpression> exp0 = exprCol->GetItem(0);
    FdoPtr<FdoExpression> exp1 = exprCol->GetItem(1);

    bool processed = false;
    FdoPropertyType propType = FdoPropertyType_DataProperty;
    FdoDataType dataType = FdoDataType_Int32;
    FdoPtr<FdoClassDefinition> clsDef = mFdoConnection->GetClassDefinition(mCurrentClassName);
    FdoPtr<FdoIExpressionCapabilities> expressionCaps = mFdoConnection->GetExpressionCapabilities();
    FdoPtr<FdoFunctionDefinitionCollection> functions = expressionCaps->GetFunctions();
    try
    {
        FdoCommonMiscUtil::GetExpressionType(functions, clsDef, exp1, propType, dataType);
    }
    catch(FdoExpression* ex) { ex->Release(); }

    if (dataType != FdoDataType_DateTime)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));

    if (FdoExpressionItemType_DataValue == exp0->GetExpressionType())
    {
        FdoDataValue* dv = static_cast<FdoDataValue*>(exp0.p);
        if (dv != NULL && dv->GetDataType() == FdoDataType_String)
        {
            FdoStringValue* strVal = static_cast<FdoStringValue*>(dv);
            if (strVal != NULL && !strVal->IsNull())
            {
                FdoString* oper = strVal->GetString();
                if (wcscmp(oper, L"YEAR") == 0)
                {
                    processed = true;
                    AppendString(L"DATEPART(YEAR,");
                    HandleExpr(exp1);
                    AppendString(L")");
                }
                else if (wcscmp(oper, L"MONTH") == 0)
                {
                    processed = true;
                    AppendString(L"DATEPART(MONTH,");
                    HandleExpr(exp1);
                    AppendString(L")");
                }
                else if (wcscmp(oper, L"DAY") == 0)
                {
                    processed = true;
                    AppendString(L"DATEPART(DAY,");
                    HandleExpr(exp1);
                    AppendString(L")");
                }
                else if (wcscmp(oper, L"HOUR") == 0)
                {
                    processed = true;
                    AppendString(L"DATEPART(HOUR,");
                    HandleExpr(exp1);
                    AppendString(L")");
                }
                else if (wcscmp(oper, L"MINUTE") == 0)
                {
                    processed = true;
                    AppendString(L"DATEPART(MINUTE,");
                    HandleExpr(exp1);
                    AppendString(L")");
                }
                else if (wcscmp(oper, L"SECOND") == 0)
                {
                    processed = true;
                    AppendString(L"DATEPART(SECOND,");
                    HandleExpr(exp1);
                    AppendString(L")");
                }
            }
        }
    }
    else
    {
        AppendString(L"case when ");
        HandleExpr(exp0);
        AppendString(L"=N'YEAR' then DATEPART(YEAR,");
        HandleExpr(exp1);
        AppendString(L") when ");
        HandleExpr(exp0);
        AppendString(L"=N'MONTH' then DATEPART(MONTH,");
        HandleExpr(exp1);
        AppendString(L") when ");
        HandleExpr(exp0);
        AppendString(L"=N'DAY' then DATEPART(DAY,");
        HandleExpr(exp1);
        AppendString(L") when ");
        HandleExpr(exp0);
        AppendString(L"=N'HOUR' then DATEPART(HOUR,");
        HandleExpr(exp1);
        AppendString(L") when ");
        HandleExpr(exp0);
        AppendString(L"=N'MINUTE' then DATEPART(MINUTE,");
        HandleExpr(exp1);
        AppendString(L") when ");
        HandleExpr(exp0);
        AppendString(L"=N'SECOND' then DATEPART(SECOND,");
        HandleExpr(exp1);
        AppendString(L") end ");
        processed = true;
    }
    if (!processed)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));
}

void FdoRdbmsSqlServerFilterProcessor::ProcessExtractToDblFunction(FdoFunction& expr)
{
    FdoPtr<FdoExpressionCollection> exprCol = expr.GetArguments();
    if (exprCol->GetCount() != 2)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));

    FdoPtr<FdoExpression> exp0 = exprCol->GetItem(0);
    FdoPtr<FdoExpression> exp1 = exprCol->GetItem(1);

    bool processed = false;
    FdoPropertyType propType = FdoPropertyType_DataProperty;
    FdoDataType dataType = FdoDataType_Int32;
    FdoPtr<FdoClassDefinition> clsDef = mFdoConnection->GetClassDefinition(mCurrentClassName);
    FdoPtr<FdoIExpressionCapabilities> expressionCaps = mFdoConnection->GetExpressionCapabilities();
    FdoPtr<FdoFunctionDefinitionCollection> functions = expressionCaps->GetFunctions();
    try
    {
        FdoCommonMiscUtil::GetExpressionType(functions, clsDef, exp1, propType, dataType);
    }
    catch(FdoExpression* ex) { ex->Release(); }

    if (dataType != FdoDataType_DateTime)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));

    if (FdoExpressionItemType_DataValue == exp0->GetExpressionType())
    {
        FdoDataValue* dv = static_cast<FdoDataValue*>(exp0.p);
        if (dv != NULL && dv->GetDataType() == FdoDataType_String)
        {
            FdoStringValue* strVal = static_cast<FdoStringValue*>(dv);
            if (strVal != NULL && !strVal->IsNull())
            {
                FdoString* oper = strVal->GetString();
                if (wcscmp(oper, L"YEAR") == 0)
                {
                    processed = true;
                    AppendString(L"cast(DATEPART(YEAR,");
                    HandleExpr(exp1);
                    AppendString(L") as real)");
                }
                else if (wcscmp(oper, L"MONTH") == 0)
                {
                    processed = true;
                    AppendString(L"cast(DATEPART(MONTH,");
                    HandleExpr(exp1);
                    AppendString(L") as real)");
                }
                else if (wcscmp(oper, L"DAY") == 0)
                {
                    processed = true;
                    AppendString(L"cast(DATEPART(DAY,");
                    HandleExpr(exp1);
                    AppendString(L") as real)");
                }
                else if (wcscmp(oper, L"HOUR") == 0)
                {
                    processed = true;
                    AppendString(L"cast(DATEPART(HOUR,");
                    HandleExpr(exp1);
                    AppendString(L") as real)");
                }
                else if (wcscmp(oper, L"MINUTE") == 0)
                {
                    processed = true;
                    AppendString(L"cast(DATEPART(MINUTE,");
                    HandleExpr(exp1);
                    AppendString(L") as real)");
                }
                else if (wcscmp(oper, L"SECOND") == 0)
                {
                    processed = true;
                    AppendString(L"(DATEPART(SECOND,");
                    HandleExpr(exp1);
                    AppendString(L")+DATEPART(MILLISECOND,");
                    HandleExpr(exp1);
                    AppendString(L")/1000.0)");
                }
            }
        }
    }
    else
    {
        AppendString(L"case when ");
        HandleExpr(exp0);
        AppendString(L"=N'YEAR' then DATEPART(YEAR,");
        HandleExpr(exp1);
        AppendString(L") when ");
        HandleExpr(exp0);
        AppendString(L"=N'MONTH' then DATEPART(MONTH,");
        HandleExpr(exp1);
        AppendString(L") when ");
        HandleExpr(exp0);
        AppendString(L"=N'DAY' then DATEPART(DAY,");
        HandleExpr(exp1);
        AppendString(L") when ");
        HandleExpr(exp0);
        AppendString(L"=N'HOUR' then DATEPART(HOUR,");
        HandleExpr(exp1);
        AppendString(L") when ");
        HandleExpr(exp0);
        AppendString(L"=N'MINUTE' then DATEPART(MINUTE,");
        HandleExpr(exp1);
        AppendString(L") when ");
        HandleExpr(exp0);
        AppendString(L"=N'SECOND' then DATEPART(SECOND,");
        HandleExpr(exp1);
        AppendString(L")+DATEPART(MILLISECOND,");
        HandleExpr(exp1);
        AppendString(L")/1000.0) end ");
        processed = true;
    }
    if (!processed)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));
}

void FdoRdbmsSqlServerFilterProcessor::ProcessLpadFunction(FdoFunction& expr)
{
    FdoPtr<FdoExpressionCollection> exprCol = expr.GetArguments();
    FdoInt32 colCount = exprCol->GetCount();
    if (colCount != 2 && colCount != 3)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));
    FdoPtr<FdoExpression> exp0 = exprCol->GetItem(0);
    FdoPtr<FdoExpression> exp1 = exprCol->GetItem(1);

    AppendString(L"case when cast(");
    HandleExpr(exp1);
    AppendString(L" as bigint) <= len(convert(nvarchar,");
    HandleExpr(exp0);
    AppendString(L")) then left(convert(nvarchar,");
    HandleExpr(exp0);
    AppendString(L"),cast(");
    HandleExpr(exp1);
    if (colCount == 2)
    {
        AppendString(L" as bigint)) else left(REPLICATE(N' ',cast(");
        HandleExpr(exp1);
        AppendString(L" as bigint)), cast(");
        HandleExpr(exp1);
        AppendString(L" as bigint)-len(convert(nvarchar,");
        HandleExpr(exp0);
        AppendString(L"))) + convert(nvarchar,");
        HandleExpr(exp0);
        AppendString(L") end");
    }
    else
    {
        FdoPtr<FdoExpression> exp2 = exprCol->GetItem(2);
        AppendString(L" as bigint)) else left(REPLICATE(convert(nvarchar,");
        HandleExpr(exp2);
        AppendString(L"),cast(");
        HandleExpr(exp1);
        AppendString(L" as bigint)), cast(");
        HandleExpr(exp1);
        AppendString(L" as bigint)-len(convert(nvarchar,");
        HandleExpr(exp0);
        AppendString(L"))) + convert(nvarchar,");
        HandleExpr(exp0);
        AppendString(L") end");
    }
}

void FdoRdbmsSqlServerFilterProcessor::ProcessRpadFunction(FdoFunction& expr)
{
    FdoPtr<FdoExpressionCollection> exprCol = expr.GetArguments();
    FdoInt32 colCount = exprCol->GetCount();
    if (colCount != 2 && colCount != 3)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));
    FdoPtr<FdoExpression> exp0 = exprCol->GetItem(0);
    FdoPtr<FdoExpression> exp1 = exprCol->GetItem(1);
    if (colCount == 2)
    {
        AppendString(L"LEFT(convert(nvarchar,");
        HandleExpr(exp0);
        AppendString(L")+REPLICATE(N' ',cast(");
        HandleExpr(exp1);
        AppendString(L" as bigint)),cast(");
        HandleExpr(exp1);
        AppendString(L" as bigint))");
    }
    else
    {
        FdoPtr<FdoExpression> exp2 = exprCol->GetItem(2);
        AppendString(L"LEFT(convert(nvarchar,");
        HandleExpr(exp0);
        AppendString(L")+REPLICATE(convert(nvarchar,");
        HandleExpr(exp2);
        AppendString(L"),cast(");
        HandleExpr(exp1);
        AppendString(L" as bigint)),cast(");
        HandleExpr(exp1);
        AppendString(L" as bigint))");
    }
}

void FdoRdbmsSqlServerFilterProcessor::ProcessRemainderFunction(FdoFunction& expr)
{
    FdoPtr<FdoExpressionCollection> exprCol = expr.GetArguments();
    FdoInt32 colCount = exprCol->GetCount();
    if (colCount != 2)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));

    FdoPtr<FdoExpression> exp0 = exprCol->GetItem(0);
    FdoPtr<FdoExpression> exp1 = exprCol->GetItem(1);

    AppendString(L"ABS(");
    HandleExpr(exp0);
    AppendString(L")-ABS(");
    HandleExpr(exp1);
    AppendString(L")*(case when cast(ABS(");
    HandleExpr(exp0);
    AppendString(L") as real)/cast(ABS(");
    HandleExpr(exp1);
    AppendString(L") as real)-FLOOR(cast(ABS(");
    HandleExpr(exp0);
    AppendString(L") as real)/cast(ABS(");
    HandleExpr(exp1);
    AppendString(L") as real))<=0.5 then FLOOR(cast(ABS(");
    HandleExpr(exp0);
    AppendString(L") as real)/cast(ABS(");
    HandleExpr(exp1);
    AppendString(L") as real)) else CEILING(cast(ABS(");
    HandleExpr(exp0);
    AppendString(L") as real)/cast(ABS(");
    HandleExpr(exp1);
    AppendString(L") as real)) end) *(case when ");
    HandleExpr(exp0);
    AppendString(L"<0 THEN -1.0 ELSE 1.0 END)");
}

void FdoRdbmsSqlServerFilterProcessor::ProcessTrimFunction(FdoFunction& expr)
{
    FdoPtr<FdoExpressionCollection> exprCol = expr.GetArguments();
    FdoInt32 colCount = exprCol->GetCount();
    if (colCount != 1 && colCount != 2)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));

    FdoPtr<FdoExpression> mainexp = exprCol->GetItem(0);
    if (colCount == 1)
    {
        AppendString(L"LTRIM(RTRIM(convert(nvarchar,");
        HandleExpr(mainexp);
        AppendString(L")))");
    }
    else
    {
        bool processed = false;
        FdoPtr<FdoExpression> exp = exprCol->GetItem(0);
        mainexp = exprCol->GetItem(1);
        if (FdoExpressionItemType_DataValue == exp->GetExpressionType())
        {
            FdoDataValue* dv = static_cast<FdoDataValue*>(exp.p);
            if (dv != NULL && dv->GetDataType() == FdoDataType_String)
            {
                FdoStringValue* strVal = static_cast<FdoStringValue*>(dv);
                if (strVal != NULL && !strVal->IsNull())
                {
                    FdoString* oper = strVal->GetString();
                    if (wcscmp(oper, L"BOTH") == 0)
                    {
                        processed = true;
                        AppendString(L"LTRIM(RTRIM(convert(nvarchar,");
                        HandleExpr(mainexp);
                        AppendString(L")))");
                    }
                    else if (wcscmp(oper, L"LEADING") == 0)
                    {
                        processed = true;
                        AppendString(L"LTRIM(convert(nvarchar,");
                        HandleExpr(mainexp);
                        AppendString(L"))");
                    }
                    else if (wcscmp(oper, L"TRAILING") == 0)
                    {
                        processed = true;
                        AppendString(L"RTRIM(convert(nvarchar,");
                        HandleExpr(mainexp);
                        AppendString(L"))");
                    }
                }
            }
        }
        else
        {
            AppendString(L"case when ");
            HandleExpr(exp);
            AppendString(L"=N'BOTH' then LTRIM(RTRIM(convert(nvarchar,");
            HandleExpr(mainexp);
            AppendString(L"))) when ");
            HandleExpr(exp);
            AppendString(L"=N'LEADING' then LTRIM(convert(nvarchar,");
            HandleExpr(mainexp);
            AppendString(L")) when ");
            HandleExpr(exp);
            AppendString(L"=N'TRAILING' then RTRIM(convert(nvarchar,");
            HandleExpr(mainexp);
            AppendString(L")) end");
        }
    }
}

void FdoRdbmsSqlServerFilterProcessor::ProcessSubStrFunction(FdoFunction& expr)
{
    FdoPtr<FdoExpressionCollection> exprCol = expr.GetArguments();
    FdoInt32 colCount = exprCol->GetCount();
    if (colCount != 2 && colCount != 3)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));

    FdoPtr<FdoExpression> exp0 = exprCol->GetItem(0);
    FdoPtr<FdoExpression> exp1 = exprCol->GetItem(1);
    AppendString(L"(SUBSTRING(");
    HandleExpr(exp0);
    AppendString(L",(CASE WHEN cast(");
    HandleExpr(exp1);
    AppendString(L" as bigint)=0 THEN 1");
    AppendString(L" WHEN cast(");
    HandleExpr(exp1);
    AppendString(L" as bigint)<0 THEN (case when len(convert(nvarchar,");
    HandleExpr(exp0);
    AppendString(L"))+cast(");
    HandleExpr(exp1);
    AppendString(L" as bigint)<0 THEN len(convert(nvarchar,");
    HandleExpr(exp0);
    AppendString(L"))+1 else");
    AppendString(L" len(convert(nvarchar,");
    HandleExpr(exp0);
    AppendString(L"))+1+cast(");
    HandleExpr(exp1);
    AppendString(L" as bigint) END) ELSE cast(");
    HandleExpr(exp1);
    if (colCount == 3)
    {
        FdoPtr<FdoExpression> exp2 = exprCol->GetItem(2);
        AppendString(L" as bigint) END) ,(CASE WHEN cast(");
        HandleExpr(exp2);
        AppendString(L" as bigint)<0 THEN 0 ELSE cast(");
        HandleExpr(exp2);
        AppendString(L" as bigint) END)))");
    }
    else
    {
        AppendString(L" as bigint) END), len(convert(nvarchar,");
        HandleExpr(exp0);
        AppendString(L"))))");
    }
}

void FdoRdbmsSqlServerFilterProcessor::ProcessInstrFunction(FdoFunction& expr)
{
    FdoPtr<FdoExpressionCollection> exprCol = expr.GetArguments();
    FdoInt32 colCount = exprCol->GetCount();
    if (colCount != 2)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));

    AppendString(L"(CHARINDEX(convert(nvarchar,");
    FdoPtr<FdoExpression> exp = exprCol->GetItem(1);
    HandleExpr(exp);
    AppendString(L"),convert(nvarchar,");
    exp = exprCol->GetItem(0);
    HandleExpr(exp);
    AppendString(L")))");
}

void FdoRdbmsSqlServerFilterProcessor::ProcessTruncFunction(FdoFunction& expr)
{
    FdoPtr<FdoExpressionCollection> exprCol = expr.GetArguments();
    FdoInt32 colCount = exprCol->GetCount();
    if (colCount != 1 && colCount != 2)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));

    FdoPtr<FdoExpression> mainexp = exprCol->GetItem(0);
    if (colCount == 1)
    {
        AppendString(L"cast(cast(");
        HandleExpr(mainexp);
        AppendString(L" as bigint) as real)");
    }
    else
    {
        FdoPropertyType propType = FdoPropertyType_DataProperty;
        FdoDataType dataType = FdoDataType_Int32;
        FdoPtr<FdoClassDefinition> clsDef = mFdoConnection->GetClassDefinition(mCurrentClassName);
        FdoPtr<FdoIExpressionCapabilities> expressionCaps = mFdoConnection->GetExpressionCapabilities();
        FdoPtr<FdoFunctionDefinitionCollection> functions = expressionCaps->GetFunctions();
        try
        {
            FdoCommonMiscUtil::GetExpressionType(functions, clsDef, mainexp, propType, dataType);
        }
        catch(FdoExpression* ex) { ex->Release(); }

        if (dataType == FdoDataType_DateTime)
        {
            bool processed = false;
            FdoPtr<FdoExpression> exp = exprCol->GetItem(1);
            if (FdoExpressionItemType_DataValue == exp->GetExpressionType())
            {
                FdoDataValue* dv = static_cast<FdoDataValue*>(exp.p);
                if (dv != NULL && dv->GetDataType() == FdoDataType_String)
                {
                    FdoStringValue* strVal = static_cast<FdoStringValue*>(dv);
                    if (strVal != NULL && !strVal->IsNull())
                    {
                        FdoString* oper = strVal->GetString();
                        if (wcscmp(oper, L"YEAR") == 0)
                        {
                            processed = true;
                            AppendString(L"cast(DATEPART(YEAR,");
                            HandleExpr(mainexp);
                            AppendString(L") as nvarchar) + N'-01-01 00:00:00.000'");
                        }
                        else if (wcscmp(oper, L"MONTH") == 0)
                        {
                            processed = true;
                            AppendString(L"cast(DATEPART(YEAR,");
                            HandleExpr(mainexp);
                            AppendString(L") as nvarchar) + N'-' + cast(DATEPART(MONTH,");
                            HandleExpr(mainexp);
                            AppendString(L") as nvarchar) + N'-01 00:00:00.000'");
                        }
                        else if (wcscmp(oper, L"DAY") == 0)
                        {
                            processed = true;
                            HandleExpr(mainexp);
                        }
                        else if (wcscmp(oper, L"HOUR") == 0)
                        {
                            processed = true;
                            AppendString(L"cast(DATEPART(YEAR,");
                            HandleExpr(mainexp);
                            AppendString(L") as nvarchar) + N'-' + cast(DATEPART(MONTH,");
                            HandleExpr(mainexp);
                            AppendString(L") as nvarchar) + N'-' + cast(DATEPART(DAY,");
                            HandleExpr(mainexp);
                            AppendString(L") as nvarchar) + N' ' + cast(DATEPART(HOUR,");
                            HandleExpr(mainexp);
                            AppendString(L") as nvarchar) + N':00:00.000'");
                        }
                        else if (wcscmp(oper, L"MINUTE") == 0)
                        {
                            processed = true;
                            AppendString(L"cast(DATEPART(YEAR,");
                            HandleExpr(mainexp);
                            AppendString(L") as nvarchar) + N'-' + cast(DATEPART(MONTH,");
                            HandleExpr(mainexp);
                            AppendString(L") as nvarchar) + N'-' + cast(DATEPART(DAY,");
                            HandleExpr(mainexp);
                            AppendString(L") as nvarchar) + N' ' + cast(DATEPART(HOUR,");
                            HandleExpr(mainexp);
                            AppendString(L") as nvarchar) + N':' + cast(DATEPART(MINUTE,");
                            HandleExpr(mainexp);
                            AppendString(L") as nvarchar) + N':00.000'");
                        }
                    }
                }
            }
            else
            {
                propType = FdoPropertyType_DataProperty;
                dataType = FdoDataType_Int32;
                FdoCommonMiscUtil::GetExpressionType(functions, clsDef, mainexp, propType, dataType);
                if (dataType == FdoDataType_String)
                {
                    AppendString(L"case when ");
                    HandleExpr(exp);
                    AppendString(L"=N'YEAR' then ");
                    AppendString(L"cast(DATEPART(YEAR,");
                    HandleExpr(mainexp);
                    AppendString(L") as nvarchar) + N'-01-01 00:00:00.000'");
                    AppendString(L" when ");
                    HandleExpr(exp);
                    AppendString(L"=N'MONTH' then ");
                    AppendString(L"cast(DATEPART(YEAR,");
                    HandleExpr(mainexp);
                    AppendString(L") as nvarchar) + N'-' + cast(DATEPART(MONTH,");
                    HandleExpr(mainexp);
                    AppendString(L") as nvarchar) + N'-01 00:00:00.000' ");
                    AppendString(L" when ");
                    HandleExpr(exp);
                    AppendString(L"=N'DAY' then ");
                    HandleExpr(mainexp);
                    AppendString(L" when ");
                    HandleExpr(exp);
                    AppendString(L"=N'HOUR' then ");
                    AppendString(L"cast(DATEPART(YEAR,");
                    HandleExpr(mainexp);
                    AppendString(L") as nvarchar) + N'-' + cast(DATEPART(MONTH,");
                    HandleExpr(mainexp);
                    AppendString(L") as nvarchar) + N'-' + cast(DATEPART(DAY,");
                    HandleExpr(mainexp);
                    AppendString(L") as nvarchar) + N' ' + cast(DATEPART(HOUR,");
                    HandleExpr(mainexp);
                    AppendString(L") as nvarchar) + N':00:00.000'");
                    AppendString(L" when ");
                    HandleExpr(exp);
                    AppendString(L"=N'MINUTE' then ");
                    AppendString(L"cast(DATEPART(YEAR,");
                    HandleExpr(mainexp);
                    AppendString(L") as nvarchar) + N'-' + cast(DATEPART(MONTH,");
                    HandleExpr(mainexp);
                    AppendString(L") as nvarchar) + N'-' + cast(DATEPART(DAY,");
                    HandleExpr(mainexp);
                    AppendString(L") as nvarchar) + N' ' + cast(DATEPART(HOUR,");
                    HandleExpr(mainexp);
                    AppendString(L") as nvarchar) + N':' + cast(DATEPART(MINUTE,");
                    HandleExpr(mainexp);
                    AppendString(L") as nvarchar) + N':00.000' end ");
                }
            }
            if (!processed)
                throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));
        }
        else
        {
            AppendString(L"round(");
            HandleExpr(mainexp);
            AppendString(L", ");
            AppendString(L"cast(");
            FdoPtr<FdoExpression> exp = exprCol->GetItem(1);
            HandleExpr(exp);
            AppendString(L" as bigint), 1)");
        }
    }
}

void FdoRdbmsSqlServerFilterProcessor::ProcessModFunction(FdoFunction& expr)
{
    FdoPtr<FdoExpressionCollection> exprCol = expr.GetArguments();
    FdoInt32 colCount = exprCol->GetCount();
    if (colCount != 2)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));

    FdoPtr<FdoExpression> exp0 = exprCol->GetItem(0);
    FdoPtr<FdoExpression> exp1 = exprCol->GetItem(1);
    AppendString(L"ABS(");
    HandleExpr(exp0);
    AppendString(L")-ABS(");
    HandleExpr(exp1);
    AppendString(L")*FLOOR(ABS(");
    HandleExpr(exp0);
    AppendString(L")/ABS(");
    HandleExpr(exp1);
    AppendString(L"))*(case when ");
    HandleExpr(exp0);
    AppendString(L"<0 THEN -1.0 ELSE 1.0 END)");
}

//
// Aggregate functions require special processing because of the optional
// first parameter. The value of this parameter may be ALL or DISTINCT.
void FdoRdbmsSqlServerFilterProcessor::ProcessAggregateFunction (FdoFunction& expr)
{
    // Append the function name and the opening bracket.
    ProcessFunctionName(expr);
	AppendString("( ");

    // Process the arguments. This is were the special processing is required as
    // it is required to have the parameters listed sequencially without a comma
    // between them.
    FdoPtr<FdoExpressionCollection> exprCol = expr.GetArguments();
    for (int i = 0; i < exprCol->GetCount(); i++)
    {
        FdoPtr<FdoExpression>exp = exprCol->GetItem(i);
        if ((i == 0) && (IsDataValue(exp)))
        {
            FdoDataValue *dataValue = (static_cast<FdoDataValue *>(exp.p));
            if (dataValue->GetDataType() == FdoDataType_String)
            {
                // Omit ALL if specified as this keyword is not supported in
                // MySQL.
                FdoStringValue *stringValue = static_cast<FdoStringValue *>(dataValue);
                FdoStringP strValue = stringValue->GetString();
                if (FdoCommonOSUtil::wcsicmp(strValue, L"ALL") != 0)
                {
                    AppendString(stringValue->GetString());
                    AppendString(L" ");	
                }
            }
            else
                throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_29, "Unsupported FDO type in expression"));
        }
        else
            HandleExpr(exp);
	}
    AppendString(" )");
}

void FdoRdbmsSqlServerFilterProcessor::ProcessSpatialExtentsFunction (FdoFunction& expr)
{
    // SQL Server uses a different native function name for the expression function
    // SpatialExtents. 
    FdoPtr<FdoExpressionCollection> exprCol = expr.GetArguments();

    // Assume just one argument. TODO trow exception.
    FdoPtr<FdoIdentifier>   id = (FdoIdentifier *)(exprCol->GetItem(0));
    
    // Delimit column name with []. Can't use " when part of function.
    AppendString(L"[");
    AppendString( PropertyNameToColumnName( id->GetName() ) );
    AppendString(L"].");
    AppendString(SQLSERVER_FUNCTION_SPATIALEXTENTS);
    AppendString(OPEN_PARENTH);
    AppendString(CLOSE_PARENTH);
}

void FdoRdbmsSqlServerFilterProcessor::ProcessIsValidFunction (FdoFunction& expr)
{
    // SQL Server uses a different native function name for the expression function
    // IsValid. 
    FdoPtr<FdoExpressionCollection> exprCol = expr.GetArguments();

    // There are problems processing booleans. Use integers instead.
    AppendString(L"ABS(");

    // Assume just one argument. TODO throw exception if not geometry column.
    FdoPtr<FdoIdentifier>   id = (FdoIdentifier *)(exprCol->GetItem(0));
    
    AppendString( PropertyNameToColumnName( id->GetName() ) );
    AppendString(L".");
    AppendString(SQLSERVER_FUNCTION_ISVALID);
    AppendString(OPEN_PARENTH);
    AppendString(CLOSE_PARENTH);

    AppendString(CLOSE_PARENTH);
}

void FdoRdbmsSqlServerFilterProcessor::ProcessZMFunction (FdoFunction& expr)
{
    FdoPtr<FdoExpressionCollection> exprCol = expr.GetArguments();

    // Assume just one argument. TODO throw exception if not geometry column.
    AppendString(L"\"");
    FdoPtr<FdoIdentifier>   id = (FdoIdentifier *)(exprCol->GetItem(0));
    
    AppendString( PropertyNameToColumnName( id->GetName() ) );
    AppendString(L"\".");
    AppendString( expr.GetName() );

}


void FdoRdbmsSqlServerFilterProcessor::ProcessToDoubleFunction (FdoFunction& expr)
{
    FdoPtr<FdoExpressionCollection> exprCol = expr.GetArguments();
    FdoInt32 colCount = exprCol->GetCount();
    if (colCount != 1)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));

    FdoPtr<FdoExpression> exp0 = exprCol->GetItem(0);
    AppendString(L"convert(real,");
    HandleExpr(exp0);
    AppendString(L")");
}

void FdoRdbmsSqlServerFilterProcessor::ProcessToFloatFunction (FdoFunction& expr)
{
    FdoPtr<FdoExpressionCollection> exprCol = expr.GetArguments();
    FdoInt32 colCount = exprCol->GetCount();
    if (colCount != 1)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));

    FdoPtr<FdoExpression> exp0 = exprCol->GetItem(0);
    AppendString(L"convert(float,");
    HandleExpr(exp0);
    AppendString(L")");
}

void FdoRdbmsSqlServerFilterProcessor::ProcessToIntFunction (FdoFunction& expr)
{
    // To process the expression engine functions TOINT32 and TOINT64 in SQL
    // SERVER it is required to execute a sequence of function calls instead
    // as there is no native corresponding function.
    AppendString(SQLSERVER_FUNCTION_TOINT);
    AppendString(OPEN_PARENTH);
    AppendString(L"REAL, ");

    FdoPtr<FdoExpressionCollection> exprCol = expr.GetArguments();
    for (int i = 0; i < exprCol->GetCount(); i++ )
    {
        if (i != 0)
            AppendString(L", ");

        FdoPtr<FdoExpression>exp = exprCol->GetItem(i);
        HandleExpr(exp);
    }
    AppendString(CLOSE_PARENTH);
    AppendString(CLOSE_PARENTH);
}

void FdoRdbmsSqlServerFilterProcessor::ProcessAddMonthsFunction (FdoFunction& expr)
{
    FdoPtr<FdoExpressionCollection> exprCol = expr.GetArguments();
    FdoInt32 colCount = exprCol->GetCount();
    if (colCount != 2)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));

    FdoPtr<FdoExpression> exp0 = exprCol->GetItem(0);
    FdoPtr<FdoExpression> exp1 = exprCol->GetItem(1);

    AppendString(L"DATEADD(MONTH, convert(bigint,");
    HandleExpr(exp1);
    AppendString(L"),");
    HandleExpr(exp0);
    AppendString(L")");
}

void FdoRdbmsSqlServerFilterProcessor::ProcessMonthsBetweenFunction (FdoFunction& expr)
{
    FdoPtr<FdoExpressionCollection> exprCol = expr.GetArguments();
    FdoInt32 colCount = exprCol->GetCount();
    if (colCount != 2)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_103, "Invalid parameter"));

    FdoPtr<FdoExpression> exp0 = exprCol->GetItem(0);
    FdoPtr<FdoExpression> exp1 = exprCol->GetItem(1);

    AppendString(L"DATEDIFF(MONTH, convert(bigint,");
    HandleExpr(exp1);
    AppendString(L"),");
    HandleExpr(exp0);
    AppendString(L")");
}

void FdoRdbmsSqlServerFilterProcessor::ProcessLogFunction (FdoFunction& expr)
{
    // To process this function in SQL Server, there are a few of issues to consider:
    //
    //  - There is no native support for this expression function in SQL Server.
    //    Instead, the result is determined by dividing the natural logarithm of the
    //    numeric value by the natural logarithm of the base value.
    //  -  The expression function LOG expects the base value as the first argument
    //     and a numeric value as the second parameter. For the calculation, those
    //     arguments must be reversed.
    //
    // This function does not perform a syntax check for the SQL Server function. This
    // is left to the SQL Server validation. If the number of arguments are not as ex-
    // pected they are just added to the function call instead of handling them as
    // explained above.
    AppendString(OPEN_PARENTH);

    FdoPtr<FdoExpressionCollection> exprCol = expr.GetArguments();
    FdoInt32 exprColCount = exprCol->GetCount();
    if (exprColCount != 2) 
    {
        for (int i = 0; i < exprCol->GetCount(); i++ )
        {
            if (i != 0)
                AppendString(L", ");

            FdoPtr<FdoExpression>exp = exprCol->GetItem(i);
            HandleExpr(exp);
        }
    }
    else
    {
        FdoPtr<FdoExpression>exp0 = exprCol->GetItem(0);
        FdoPtr<FdoExpression>exp1 = exprCol->GetItem(1);
        AppendString(SQLSERVER_FUNCTION_LOG);
        AppendString(OPEN_PARENTH);
        HandleExpr(exp1);
        AppendString(CLOSE_PARENTH);
        AppendString(ARITHMETIC_DIV);
        AppendString(OPEN_PARENTH);
        AppendString(SQLSERVER_FUNCTION_LOG);
        AppendString(OPEN_PARENTH);
        HandleExpr(exp0);
        AppendString(CLOSE_PARENTH);
        AppendString(CLOSE_PARENTH);
    }
    AppendString(CLOSE_PARENTH);
}

void FdoRdbmsSqlServerFilterProcessor::ProcessRoundFunction (FdoFunction& expr)
{
    // The expression function ROUND allows for an optional second parameter.
    // The corresponding SQL Server function signature does not allow this and
    // requests the second parameter as a mandatory argument. Therefore, if
    // the number of arguments are less than expected, add a default value
    // as the second argument.
    FdoPtr<FdoExpressionCollection> exprCol = expr.GetArguments();
    FdoInt32 colCount = exprCol->GetCount();
    AppendString(expr.GetName());
    AppendString(OPEN_PARENTH);

    for (int i = 0; i < colCount; i++ )
    {
        if (i != 0)
            AppendString(L", ");

        FdoPtr<FdoExpression>exp = exprCol->GetItem(i);
        HandleExpr(exp);
    }

    if (colCount == 1)
        AppendString(L", 0");

    AppendString(CLOSE_PARENTH);
}

void FdoRdbmsSqlServerFilterProcessor::ProcessConcatFunction(FdoFunction& expr)
{
    // The SQL Server doesn't have a native 'CONCAT' method; instead, the string
    // concatenation operator "+" is used.
    AppendString(OPEN_PARENTH);
    FdoPtr<FdoExpressionCollection> exprCol = expr.GetArguments();
    for (int i = 0; i < exprCol->GetCount(); i++)
    {
        if (i != 0)
            AppendString(ARITHMETIC_PLUS);

        AppendString(L"convert(varchar, ");
        FdoPtr<FdoExpression>exp = exprCol->GetItem(i);
        HandleExpr(exp);
        AppendString(L")");
    }
    AppendString(CLOSE_PARENTH);
}

//
// Function to map an expression function name to the corresponding SQL Server
// function name. If the name does not needs to be mapped, the provided name is
// returned.
FdoString *FdoRdbmsSqlServerFilterProcessor::MapFdoFunction2SqlServerFunction (FdoString *f_name)
{
	if (FdoCommonOSUtil::wcsicmp(f_name, FDO_FUNCTION_STDDEV) == 0 )
        return SQLSERVER_FUNCTION_STDDEV;
	if (FdoCommonOSUtil::wcsicmp(f_name, FDO_FUNCTION_NULLVALUE) == 0 )
        return SQLSERVER_FUNCTION_NULLVALUE;
	if (FdoCommonOSUtil::wcsicmp(f_name, FDO_FUNCTION_CURRENTDATE) == 0 )
        return SQLSERVER_FUNCTION_CURRENTDATE;
	if (FdoCommonOSUtil::wcsicmp(f_name, FDO_FUNCTION_EXTRACT) == 0 )
        return SQLSERVER_FUNCTION_EXTRACT;
	if (FdoCommonOSUtil::wcsicmp(f_name, FDO_FUNCTION_ATAN2) == 0 )
        return SQLSERVER_FUNCTION_ATAN2;
	if (FdoCommonOSUtil::wcsicmp(f_name, FDO_FUNCTION_LN) == 0 )
        return SQLSERVER_FUNCTION_LOG;
	if (FdoCommonOSUtil::wcsicmp(f_name, FDO_FUNCTION_CEIL) == 0 )
        return SQLSERVER_FUNCTION_CEIL;
	if (FdoCommonOSUtil::wcsicmp(f_name, FDO_FUNCTION_LENGTH) == 0 )
        return SQLSERVER_FUNCTION_LENGTH;
	if (FdoCommonOSUtil::wcsicmp(f_name, L"IsValid") == 0 )
        return SQLSERVER_FUNCTION_ISVALID;

    return f_name;
}

// The function maps FDO expression function names that have native support and
// do not need special processing to the corresponding SQL Server function name.
void FdoRdbmsSqlServerFilterProcessor::ProcessFunctionName(FdoFunction& expr)
{
    FdoString* functionName = MapFdoFunction2SqlServerFunction (expr.GetName());
    AppendString(functionName);
}

bool FdoRdbmsSqlServerFilterProcessor::CanOptimizeRelationQuery( const FdoSmLpClassDefinition* pClass, const FdoSmLpPropertyDefinition* propertyDefinition )
{
    // Although the base function checks if two association properties on select class 
    // point to the same table, it doesn't cascade through all association properties
    // on associated classes. Therefore the base can still return true if another
    // association property references the same class as this one.
    //
    // There is also a problem when the select class and an association class 
    // each have a property of the same name. In this case the Feature Reader
    // retrieves the value for the associated class property when asked for the
    // select class's property
    //
    // A full fix would require some effort so temporarily return false for the
    // SQLServerSpatial provider. This prevents the generated SQL statement from 
    // doing any joins to associated tables. This impacts performance. Each time
    // the Feature Reader has to retrieve an Association Property sub-property, it 
    // must generate a separate SQL statement. However, assocation properties are
    // not used very often yet so this should not be a problem for now. 

    return false;
}


bool FdoRdbmsSqlServerFilterProcessor::IsAggregateFunctionName(FdoString* wFunctionName) const
{
    for(int i=0; sqlServerAggregateFunctions[i]; i++)
        if( FdoCommonOSUtil::wcsicmp(sqlServerAggregateFunctions[i], wFunctionName) == 0 )
            return true;

    return false;
}

bool FdoRdbmsSqlServerFilterProcessor::IsNotNativeSupportedFunction(FdoString *wFunctionName) const
{

    for (int i=0; sqlServerUnsupportedFdoFunctions[i]; i++)
        if (FdoCommonOSUtil::wcsicmp(sqlServerUnsupportedFdoFunctions[i], wFunctionName) == 0)
            return true;

    return false;
}

//
// The function checks whether or not the function has a correct set of arguments.
bool FdoRdbmsSqlServerFilterProcessor::HasNativeSupportedFunctionArguments(FdoFunction& expr) const
{
    // If the function needs argument checking, execute the verification and return
    // the result back to the calling routine. Otherwise, the arguments are always 
    // deemed valid and the corresponding indication is returned.

    return true;
}


void FdoRdbmsSqlServerFilterProcessor::GetLtQualificationClause( const FdoSmLpClassDefinition *classDefinition, FdoStringP &ltQualificationClause )
{
	ltQualificationClause = L"";
	FdoPtr<FdoRdbmsLongTransactionManager> ltManager = mFdoConnection->GetLongTransactionManager();
	if( ltManager == NULL || ! classDefinition->GetCapabilities()->SupportsLongTransactions() )
		return;

	const FdoSmLpDbObject* table = classDefinition->RefDbObject();
	const wchar_t* tableName = table->GetName();

    ltQualificationClause += ltManager->GetQueryQualification(tableName, false, false, FdoCommandType_Delete);
}


void FdoRdbmsSqlServerFilterProcessor::GetLtTableExpression( const FdoSmLpClassDefinition *classDefinition, FdoStringP &ltJoin, FdoStringP &ltTableExp, FdoCommandType callerFdoCommand )
{
	ltJoin = L"";
	ltTableExp = L"";
	FdoPtr<FdoRdbmsLongTransactionManager> ltManager = mFdoConnection->GetLongTransactionManager();
	if( ltManager == NULL || ! classDefinition->GetCapabilities()->SupportsLongTransactions() )
		return;

	// 
	// Here we build a join between the class table and the LT derived table.
	// The derived table name is formed as <class table name>_LTDERIVED 
	const FdoSmLpDbObject* table = classDefinition->RefDbObject();
	const wchar_t* tableName = table->GetName();
	FdoStringP ltTableName = tableName;
	ltTableName += L"_LTDERIVED";
    ltTableName = ltTableName.Replace(L".", L"_");

	ltTableExp = L"(SELECT LTID";
    FdoStringP qualifiedTableName = FdoStringP(table->RefDbObject()->GetDbName());
	const wchar_t* tabAlias = this->GetTableAlias( qualifiedTableName );

	ltJoin = ltTableName + L".LTID=";
	ltJoin += tabAlias;
	ltJoin += L".LTID";
	const FdoSmLpDataPropertyDefinitionCollection *idProps = classDefinition->RefIdentityProperties();
	for(int i=0; i<idProps->GetCount(); i++ )
	{
		
		ltJoin += L" AND ";
		const FdoSmLpDataPropertyDefinition *prop = (const FdoSmLpDataPropertyDefinition *)idProps->RefItem(i);
		ltJoin += ltTableName;
		ltJoin += L".";
		ltJoin += prop->GetColumnName();
		ltJoin += L"=";
		ltJoin += tabAlias;
		ltJoin += L".";
		ltJoin += prop->GetColumnName();

		ltTableExp += L",";
		ltTableExp += prop->GetColumnName();
	}
	ltTableExp += L" FROM ";
	ltTableExp += qualifiedTableName;
	ltTableExp += L" WHERE ";
	ltTableExp += ltManager->GetQueryQualification(tableName, false, false, callerFdoCommand);
	ltTableExp += L") ";
	ltTableExp += ltTableName;
}

void FdoRdbmsSqlServerFilterProcessor::ProcessStringValue(FdoStringValue& expr)
{
    AppendString( (FdoString*)(FdoStringP(L"N") + expr.ToString()) );
}

void FdoRdbmsSqlServerFilterProcessor::ProcessDateTimeValue(FdoDateTimeValue& expr)
{
    // NOTE: we don't place single quotes around the time/date/timestamp, contrary to the parent class' behavior:
    AppendString( ((FdoRdbmsSqlServerConnection*)mFdoConnection)->FdoToDbiTimeFilter( expr.GetDateTime() ) );
}

FdoStringP FdoRdbmsSqlServerFilterProcessor::GetGeometryString( FdoString* dbColumnName, bool inSelectList )
{ 
    FdoStringP   dbColumnNameP = FdoStringP(dbColumnName);

    // Remove the quotes (i.e. "<name>" around the column name. They are not supported.
    FdoStringP   columnName = dbColumnNameP.Mid(1, dbColumnNameP.GetLength() - 2);

    FdoStringP   wrappedName = FdoStringP();

    // Delimit column name with []. Can't use " when part of function.
    wrappedName += L"[";
    wrappedName += columnName;
    wrappedName += L"]";

    return wrappedName;
}

FdoStringP FdoRdbmsSqlServerFilterProcessor::GetGeometryTableString( FdoString* tableName )
{
    FdoStringP  tableName2 = tableName;
    // Drop the schema name.
    if ( tableName2.Contains(L".") )
        tableName2 = tableName2.Right(L".");

    return tableName2;
}
