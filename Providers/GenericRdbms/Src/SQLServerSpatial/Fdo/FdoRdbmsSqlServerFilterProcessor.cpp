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

#include "StdAfx.h"
#include "FdoRdbmsSqlServerFilterProcessor.h"
#include "../../Fdo/Schema/FdoRdbmsSchemaUtil.h"
#include <Geometry/Fgf/Factory.h>

#include "SpatialManager/FdoRdbmsSpatialManager.h"
#include "FdoRdbmsSqlServerConnection.h"
#include "FdoCommonOSUtil.h"
#include "FdoRdbmsSqlServerSpatialGeometryConverter.h"
#include "FdoRdbmsFunctionIsValid.h"
#include "../SchemaMgr/Ph/SpatialIndex.h"
#include "../SchemaMgr/Ph/Mgr.h"

#define SQLSERVER_CONVERT_WKB L".STAsBinary()"

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
// NOTE:
//
//  TODATE
//  ------
//  The expression function TODATE allows the conversion of a string containing
//  date/time information into a date/time object. Its signatures allows the
//  specification of an optional format string that describes the structure of
//  the input string. Although SQL Server has native support to convert a string
//  with date/time information into a date/time object, it does not support the
//  specification of format information. Hence, any request to this function
//  must be redirected to the Expression Engine.
//
//  TOSTRING
//  --------
//  The expression function TOSTRING allows the conversion of either date or
//  a numeric value to a string. Although there is native support for this
//  expression function in SQL Server, the expression function is listed as
//  a not natively supported function because there is no generic way of
//  determining the kind of conversion that is needed.
//
//  EXTRACT
//  -------
//  In SQL Server, the native function processing the expression function
//  EXTRACT returns a number that cannot be correctly assigned to the cor-
//  responding object in FDO and hence the result is invalid. Therefore,
//  the evaluation is handed to the Expression Engine.
//
//  MOD
//  ---
//  SQL Server supports modulo operations by providing a modulo operator.
//  Unfortunately, it does not work with all data types that FDO can handle.
//  Hence, the corresponding expression function must be handled by the
//  Expression Engine.
//
//  TRUNC
//  -----
//  The expression function TRUNC truncates date or numeric expressions. In
//  SQL Server, there is native support for truncation of numeric values only.
//  Since it is not possible to generally determine based on the function
//  arguments whether or not there is a native support, the evaluation of the
//  function is redirected to the Expression Engine.
//
//  INSTR
//  -----
//  In SQL Server, the function that is the native representation of the
//  expression function INSTR requires the value of the search string to
//  have a leading and trailing percent sign. If the search string is
//  given as a literal, then this can be handled and the function will
//  execute correctly. However, if the search string is provided via a
//  property, most likely the string that is stored in the property will
//  not match the function's requirements. Hence, in those cases the 
//  function will not work. Therefore, the evaluation of such a function
//  is handed by the Expression Engine.
//
//  SUBSTR
//  ------
//  In SQL Server, the function that is the native representation of the
//  expression function SUBSTR cannot work with all the numeric data types
//  FDO supports. Therefore, the evaluation of the function is handed to
//  the Expression Engine.
//
//  X, Y
//  ------
//  In SQL Server, the function that is the native representation of the
//  expression functions X and Y cannot work with geography type columns.
//  Therefore, the evaluation of the function is handed to
//  the Expression Engine.

static wchar_t* sqlServerUnsupportedFdoFunctions[] = {

    L"MEDIAN",
    L"TODATE",
    L"TOSTRING",
    L"EXTRACT",
    L"EXTRACTTODOUBLE",
    L"EXTRACTTOINT",
    L"MOD",
    L"REMAINDER",
    L"TRUNC",
    L"INSTR",
    L"LPAD",
    L"RPAD",
    L"SUBSTR",
    L"TRANSLATE",
    L"TRIM",
	L"LENGTH2D",
    L"AREA2D",
    L"X",
    L"Y",
    NULL
};

    
FdoRdbmsSqlServerFilterProcessor::FdoRdbmsSqlServerFilterProcessor(FdoRdbmsConnection *connection):
FdoRdbmsFilterProcessor( connection )
{
}

FdoRdbmsSqlServerFilterProcessor::~FdoRdbmsSqlServerFilterProcessor(void)
{
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
    bool geogLatLong = false;
    const FdoString* classTableName = classDefinition->GetDbObjectName();
    const FdoString* tableName = geomProp ? geomProp->GetContainingDbObjectName() : L""; // The geometry table name
    FdoStringP columnName = GetGeometryColumnNameForProperty(geomProp, true);
    FdoStringP columnName2 = GetGeometryColumnNameForProperty(geomProp, false);

    FdoStringP spatialClause;
    FdoPtr<FdoExpression> geomExpr = filter.GetGeometry();
    FdoGeometryValue *geom = dynamic_cast<FdoGeometryValue*>(geomExpr.p);
    FdoPtr<FdoByteArray>    geomfgf;
    FdoIGeometry            *geometryObj = NULL;

    FdoStringP buf(L"");

    if ( geom ) 
        geomfgf = geom->GetGeometry();

    if (geomfgf == NULL)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_46, "No geometry value"));

    // Geometry factory
    FdoPtr<FdoFgfGeometryFactory>   gf = FdoFgfGeometryFactory::GetInstance();

    geometryObj = gf->CreateGeometryFromFgf(geomfgf);

    if ( geomColumn ) 
    {
        FdoSmPhSqsMgrP phMgr = ((FdoSmPhColumn*) geomColumn)->GetManager()->SmartCast<FdoSmPhSqsMgr>();
        geogLatLong = phMgr->IsGeogLatLong();
    }

    // SqlServer supports only 2D
    FdoPtr<FdoIGeometry> geom2D = geometryObj;
    if ( ((geomType == L"geography") && geogLatLong) || (geometryObj->GetDimensionality() != FdoDimensionality_XY) )
	{
		FdoSpatialGeometryConverter *gc = NULL;
        
        if ( (geomType == L"geography" ) && geogLatLong ) 
            gc = new FdoRdbmsSqlServerSpatialGeographyConverter();
        else
            gc = new FdoRdbmsSqlServerSpatialGeometryConverter();

		geom2D = gc->ConvertOrdinates( geometryObj, true, FdoDimensionality_XY, 0.0, 0.0);
        delete gc;
	}

    // When Coordinate System is geodetic, filter polygon outer ring must 
    // be CounterClockwise, and inner rings Clockwise.
    if ( geomType == L"geography" )
        geom2D = FdoCommonGeometryUtil::ModifyRingOrientation( geom2D );

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

    buf += "(";
    buf += geomType;
    buf += "::STGeomFromText('";
    buf += geom2D->GetText();

    // Set the SRID
	const FdoSmPhColumnP gColumn = ((FdoSmLpSimplePropertyDefinition*)geomProp)->GetColumn();
    FdoSmPhColumnGeomP geomCol = gColumn.p->SmartCast<FdoSmPhColumnGeom>();

    buf += FdoStringP::Format(L"', %ld)", geomCol->GetSRID());  

    buf += ")";
    buf += "=1";

	if ( spatialOp == FdoSpatialOperations_EnvelopeIntersects)
	{
		buf += L" AND [";
		buf += columnName;
		buf += "].MakeValid().STEnvelope().STIntersects";
		buf += "(";
		buf += geomType;
		buf += "::STGeomFromText('";
		buf += geom2D->GetText();

		buf += FdoStringP::Format(L"', %ld)", geomCol->GetSRID());  
		buf += ")";
		buf += "=1";

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

    // The functions that do not require special handling use the
    // standard processing
    FdoRdbmsFilterProcessor::ProcessFunction(expr);
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

    AppendString(SQLSERVER_CONVERT_WKB);
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
    // SQL Server uses a different native function name for the expression function
    // TODOUBLE. In addition, a specific keyword needs to be added.
    AppendString(SQLSERVER_FUNCTION_TODOUBLEFLOAT);
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
}

void FdoRdbmsSqlServerFilterProcessor::ProcessToFloatFunction (FdoFunction& expr)
{
    // SQL Server uses a different native function name for the expression function
    // TOFLOAT. In addition, a specific keyword needs to be added.
    AppendString(SQLSERVER_FUNCTION_TODOUBLEFLOAT);
    AppendString(OPEN_PARENTH);
    AppendString(L"FLOAT, ");

    FdoPtr<FdoExpressionCollection> exprCol = expr.GetArguments();
    for (int i = 0; i < exprCol->GetCount(); i++ )
    {
        if (i != 0)
            AppendString(L", ");

        FdoPtr<FdoExpression>exp = exprCol->GetItem(i);
        HandleExpr(exp);
    }
    AppendString(CLOSE_PARENTH);
}

void FdoRdbmsSqlServerFilterProcessor::ProcessToIntFunction (FdoFunction& expr)
{
    // To process the expression engine functions TOINT32 and TOINT64 in SQL
    // SERVER it is required to execute a sequence of function calls instead
    // as there is no native corresponding function.
    AppendString(SQLSERVER_FUNCTION_TOINT);
    AppendString(OPEN_PARENTH);
    AppendString(L"FLOAT, ");

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
    // To process this function in SQL Server, there are a few of issues to consider:
    //
    //  - The native function name in SQL Server for this expression function from
    //    the expression function name and hence needs to be mapped to the correct
    //    name.
    //  -  The expression function ADDMONTHS expects a date/time object as the first
    //     argument and a numeric value identifying the number of months to be added
    //     as the second parameter. For the SQL Server function, those arguments must
    //     be reversed.
    //  - The SQL Server function requires an additional keyword to be added.
    //
    // This function does not perform a syntax check for the SQL Server function. This
    // is left to the SQL Server validation. If the number of arguments are not as ex-
    // pected they are just added to the function call instead of handling them as
    // explained above.
    AppendString(SQLSERVER_FUNCTION_ADDMONTHS);
    AppendString(OPEN_PARENTH);
    AppendString(L"MONTH, ");

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
        HandleExpr(exp1);
        AppendString(L", ");
        HandleExpr(exp0);
    }
    AppendString(CLOSE_PARENTH);
}

void FdoRdbmsSqlServerFilterProcessor::ProcessMonthsBetweenFunction (FdoFunction& expr)
{
    // SQL Server uses a different native function name for the expression function
    // TOFLOAT. In addition, a specific keyword needs to be added.
    AppendString(SQLSERVER_FUNCTION_MONTHSBETWEEN);
    AppendString(OPEN_PARENTH);
    AppendString(L"MONTH, ");

    FdoPtr<FdoExpressionCollection> exprCol = expr.GetArguments();
    for (int i = 0; i < exprCol->GetCount(); i++ )
    {
        if (i != 0)
            AppendString(L", ");

        FdoPtr<FdoExpression>exp = exprCol->GetItem(i);
        HandleExpr(exp);
    }
    AppendString(CLOSE_PARENTH);
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

    // Add conversion to WKB only if this is a column in a select list.
    // For other cases (e.g. column is function argument), must not convert. 
    if ( inSelectList ) 
    {
        wrappedName += FdoStringP(SQLSERVER_CONVERT_WKB);
        
        // Use the column name as alias 
        wrappedName += L" as \"";
        wrappedName += columnName;
        wrappedName += L"\"";
    }

    return wrappedName;
}

FdoStringP FdoRdbmsSqlServerFilterProcessor::GetGeometryTableString( FdoString* tableName )
{
    FdoStringP  tableName2 = tableName;
    // 'dbo.acdb3dpolyline.geometry.STAsBinary()' syntax is not allowed.
    // Drop the schema name.
    if ( tableName2.Contains(L".") )
        tableName2 = tableName2.Right(L".");

    return tableName2;
}
