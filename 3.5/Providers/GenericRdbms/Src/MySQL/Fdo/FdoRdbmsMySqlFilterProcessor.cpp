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
#include "FdoRdbmsMySqlFilterProcessor.h"
#include "../../Fdo/Schema/FdoRdbmsSchemaUtil.h"
#include <Geometry/Fgf/Factory.h>
#include "FdoCommonOSUtil.h"
#include <math.h>   // For floor() and ceil()

#define NUMBER_FORMAT_SIZE      (100)

// The following list includes all aggregate functions MySQL supports. Not all
// functions are officially supported by FDO but they are not being prevented
// either.

static wchar_t* mySqlAggregateFunctions[] = {

    L"AVG",             // supported
    L"BIT_AND", 
    L"BIT_OR",
    L"BIT_XOR",
    L"COUNT",           // supported
    L"GROUP_CONCAT", 
    L"MAX",             // supported
    L"MIN",             // supported
    L"STD",             // supported
    L"STDDEV",          // supported
    L"STDDEV_POP",
    L"STDDEV_SAMP",
    L"SUM",             // supported
    L"VAR_POP",
    L"VAR_SAMP", 
    L"VARIANCE", 
    NULL
};

// FDO's list of expression function includes functions that do not have native
// support in MySQL. In case such a function is used, the expression cannot be
// handled by sending it to MySQL. Instead it needs to be handled in the Ex-
// pression Engine.
//
// NOTE:
//
//  ADDMONTHS, NULLVALUE, EXTRACT, ROUND, LPAD, RPAD, SUBSTR
//  --------------------------------------------------------
//  Although those expression functions have native support in MySQL, they are
//  listed as not natively supported because the generated SQL statement will
//  fail in MySQL as it contains double-quotes around property names that are
//  not valid in the case any of the functions are used. The following list an
//  example of the generated SQL statement:
//
//   "SELECT B."featid",
//           ( Lpad ( B."str2_val", 21.43, ' '  )   )  AS cmp_id,
//           B."id"
//       FROM "fdo_tok_ex_fct_b"."exfct_c1" B
//       WHERE  ( B."id" = 9  )"
//
//  The request, however, will work if the generated statement looks like this:
//
//   "SELECT B.featid,
//           ( Lpad(B.str2_val, 21.43, ' '  )   )  AS cmp_id,
//           B.id
//       FROM fdo_tok_ex_fct_b.exfct_c1 B
//       WHERE  ( B.id = 9  );"
//
//  TODATE
//  ------
//  The expression function TODATE converts a date given as a string into a
//  date/time object. Although there is native support for this expression
//  function in MySQL, the evaluation will still be redirected to the Expression
//  Engine. There are two reasons for this:
//    - There is the issue of the generated SQL statement as described above.
//    - If a format string is provided, the string needs to be evaluated and
//      the provided FDO format parameters need to be replaced with the corres-
//      ponding MySQL format parameters. This is not generally possible.
//
//  TOSTRING
//  --------
//  The expression function TOSTRING allows the convertion of either date or
//  a numeric value to a string. Although there is native support for this
//  expression function in MySQL, the expression function is listed as a not
//  natively supported function because there is no generic way of determining
//  the type of the first argument based on which a decision is made on what
//  native function to use: if it is a date, the MySQL function DATE_FORMAT is
//  required, if the argument is a numeric value, it is the function CONVERT
//  that needs to be issued.
//  The decision what function to use can be made if the first argument is not
//  another function call. However, this cannot be guaranteed and hence the 
//  evaluation of the function needs to be handled by the Expression Engine.

static wchar_t* mySqlUnsupportedFdoFunctions[] = {

    L"MEDIAN",
    L"SPATIALEXTENTS",
    L"NULLVALUE",
    L"TODATE",
    L"TOSTRING",
    L"ADDMONTHS",
    L"EXTRACT",
    L"EXTRACTTODOUBLE",
    L"EXTRACTTOINT",
    L"MONTHSBETWEEN",
    L"REMAINDER",
    L"ROUND",
    L"LPAD",
    L"RPAD",
    L"SUBSTR",
    L"TRANSLATE",
	L"LENGTH2D",
    L"AREA2D",
    L"M",
    L"Z",
    NULL
};

FdoRdbmsMySqlFilterProcessor::FdoRdbmsMySqlFilterProcessor (
                                                FdoRdbmsConnection *connection)
: FdoRdbmsFilterProcessor(connection)
{
}

FdoRdbmsMySqlFilterProcessor::~FdoRdbmsMySqlFilterProcessor (void)
{
}

const FdoSmLpGeometricPropertyDefinition 
                *FdoRdbmsMySqlFilterProcessor::GetGeometricProperty(
                                    const FdoSmLpClassDefinition *currentClass,
                                    const wchar_t                *geomPropName )
{
    const FdoSmLpGeometricPropertyDefinition *geom = NULL;

	if (currentClass != NULL)
	{
        if (geomPropName == NULL) 
        {
            // No property name specified, default to the GeometryProperty
            const FdoSmLpFeatureClass* feat = 
                FdoSmLpFeatureClass::Cast(currentClass);

            if (feat) 
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

FdoStringP FdoRdbmsMySqlFilterProcessor::GetGeometryColumnNameForProperty (
                            const FdoSmLpGeometricPropertyDefinition *pGeomProp,
                            bool                                      bChangeFilter)
{
	FdoStringP retVal;

    if (pGeomProp != NULL)
    {
	    retVal = pGeomProp->GetColumnName();

   		if (bChangeFilter) 
        {
            FdoStringP  tmpRetVal(retVal);

    	    FdoStringP suffixName = FdoStringP::Format(L"%ls",
		        	    (FdoString *) tmpRetVal.Mid( wcslen(retVal) - wcslen(NOTINDEXED_SUFFIX), wcslen(retVal)));

            // Strip the suffix from the column name
            if (suffixName.ICompare(NOTINDEXED_SUFFIX) == 0)
            {
                retVal = FdoStringP::Format(L"%ls", (FdoString *) tmpRetVal.Mid( 0, wcslen(retVal) - wcslen(NOTINDEXED_SUFFIX)));
            }
        }
	}

	return retVal;
}

/************************************************************************/
/* Process distance condition to emit spatial query string for direct   */
/* consumption by MySql-SDO                                             */
/* Currently this function processes distance query and creates an SDO  */
/* query string that can be directly issued to a database using SQLPlus */
/************************************************************************/
void FdoRdbmsMySqlFilterProcessor::ProcessDistanceCondition (FdoDistanceCondition &filter)
{

}



/************************************************************************/
/* Process spatial condition to emit spatial query string for direct    */
/* consumption by MySql                                                 */
/************************************************************************/
void FdoRdbmsMySqlFilterProcessor::ProcessSpatialCondition (FdoSpatialCondition &filter)
{
//TODO: This function's implementation is a temporary one used by the MySql demo. It must be replaced
// by a proper implementation when full spatial support is added to the MySql provider.
//
// However, the following illustrates a problem that was encountered. Spatial query result sets become
// unreliable ( sometimes they work and sometimes they return no rows when there should be some ) when
// the spatial area coordinates contain fractional parts. This is currently worked around by casting 
// these coordinates to long.
    DbiConnection  *mDbiConnection = mFdoConnection->GetDbiConnection();
    const FdoSmLpClassDefinition *classDefinition = mDbiConnection->GetSchemaUtil()->GetClass(mCurrentClassName);
    if (classDefinition == NULL ||  classDefinition->GetClassType() != FdoClassType_FeatureClass)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_230, "Spatial condition can only be used with feature classes"));

    const FdoSmLpGeometricPropertyDefinition* geomProp =
                    GetGeometricProperty(classDefinition,
                                         FdoPtr<FdoIdentifier>(filter.GetPropertyName())->GetName());
    const FdoString* classTableName = classDefinition->GetDbObjectName();
    const FdoString* tableName = geomProp ? geomProp->GetContainingDbObjectName() : L""; // The geometry table name
    FdoStringP columnName = GetGeometryColumnNameForProperty(geomProp, true);
    FdoStringP columnName2 = GetGeometryColumnNameForProperty(geomProp, false);

    FdoStringP spatialClause;
    FdoPtr<FdoGeometryValue> geom = dynamic_cast<FdoGeometryValue*>(filter.GetGeometry());
    FdoPtr<FdoFgfGeometryFactory>  gf;
    FdoPtr<FdoByteArray> geomFgf;
    FdoPtr<FdoIGeometry> geometryObj;

    geomFgf = geom->GetGeometry();

    if (geomFgf == NULL)
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_46, "No geometry value"));

    // Geometry factory
    gf = FdoFgfGeometryFactory::GetInstance();

    geometryObj = gf->CreateGeometryFromFgf(geomFgf);

    FdoPtr<FdoIEnvelope> env = geometryObj->GetEnvelope();

    double minx = env->GetMinX();
    double miny = env->GetMinY();
    double maxx = env->GetMaxX();
    double maxy = env->GetMaxY();

    wchar_t minxStr[NUMBER_FORMAT_SIZE];
    wchar_t minyStr[NUMBER_FORMAT_SIZE];
    wchar_t maxxStr[NUMBER_FORMAT_SIZE];
    wchar_t maxyStr[NUMBER_FORMAT_SIZE];

    FdoCommonStringUtil::FormatDouble(minx, minxStr, NUMBER_FORMAT_SIZE);
    FdoCommonStringUtil::FormatDouble(miny, minyStr, NUMBER_FORMAT_SIZE);
    FdoCommonStringUtil::FormatDouble(maxx, maxxStr, NUMBER_FORMAT_SIZE);
    FdoCommonStringUtil::FormatDouble(maxy, maxyStr, NUMBER_FORMAT_SIZE);

    // In MySQL 5.0, there really is no secondary filter -- the MBR family of functions
    // give the same results as the non-MBR functions, and often aren't even as selective
    // as advertised.  Plus, some clients pass in geometry conditions with extra dimensions.
    // For now, we'll take advantage of MySQL's limitations and reduce all geometry types
    // and dimensionalities to a 2D polygon with one ring, and just use the MBRIntersects operator.
    FdoStringP buf = FdoStringP::Format(L"MBRIntersects(GeomFromText('Polygon((%ls %ls,%ls %ls,%ls %ls,%ls %ls,%ls %ls))'),\"%ls\")",
            minxStr, minyStr,
            maxxStr, minyStr,
            maxxStr, maxyStr,
            minxStr, maxyStr,
            minxStr, minyStr,
            (FdoString*) columnName);
    AppendString((const wchar_t*)buf);
}

void FdoRdbmsMySqlFilterProcessor::ProcessFunction (FdoFunction &expr)
{
    FdoStringP funcName = expr.GetName();

    // Some of the FDO expression function require special processing. Those
    // functions are handled next.

    if ((FdoCommonOSUtil::wcsicmp(funcName, FDO_FUNCTION_AVG   ) == 0) ||
        (FdoCommonOSUtil::wcsicmp(funcName, FDO_FUNCTION_COUNT ) == 0) ||
        (FdoCommonOSUtil::wcsicmp(funcName, FDO_FUNCTION_MAX   ) == 0) ||
        (FdoCommonOSUtil::wcsicmp(funcName, FDO_FUNCTION_MIN   ) == 0) ||
        (FdoCommonOSUtil::wcsicmp(funcName, FDO_FUNCTION_STDDEV) == 0) ||
        (FdoCommonOSUtil::wcsicmp(funcName, FDO_FUNCTION_SUM   ) == 0)    )
        return ProcessAggregateFunction(expr);

    if ((FdoCommonOSUtil::wcsicmp(funcName, FDO_FUNCTION_TODOUBLE ) == 0) ||
        (FdoCommonOSUtil::wcsicmp(funcName, FDO_FUNCTION_TOFLOAT  ) == 0)    )
        return ProcessToDoubleFloatFunction(expr);

    if ((FdoCommonOSUtil::wcsicmp(funcName, FDO_FUNCTION_TOINT32 ) == 0) ||
        (FdoCommonOSUtil::wcsicmp(funcName, FDO_FUNCTION_TOINT64 ) == 0)    )
        return ProcessToInt32Int64Function(expr);

    if (FdoCommonOSUtil::wcsicmp(funcName, FDO_FUNCTION_CURRENTDATE) == 0)
        return ProcessCurrentDateFunction(expr);

    if (FdoCommonOSUtil::wcsicmp(funcName, FDO_FUNCTION_TRIM) == 0)
        return ProcessTrimFunction(expr);

    // The functions that do not require special handling use the
    // standard processing
    FdoRdbmsFilterProcessor::ProcessFunction(expr);
}

//
// Aggregate functions require special processing because of the optional
// first parameter. The value of this parameter may be ALL or DISTINCT.
void FdoRdbmsMySqlFilterProcessor::ProcessAggregateFunction (FdoFunction& expr)
{
    // Append the function name and the opening bracket.
    ProcessFunctionName(expr);
	AppendString( "( " );

    // Process the arguments. This is were the special processing is required as
    // it is required to have the parameters listed sequencially without a comma
    // between them.
    FdoPtr<FdoExpressionCollection> exprCol = expr.GetArguments();
    for (int i=0; i<exprCol->GetCount(); i++)
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

void FdoRdbmsMySqlFilterProcessor::ProcessToDoubleFloatFunction (FdoFunction& expr)
{
    // MySQL uses a different native function name for the expression functions
    // TODOUBLE, TOFLOAT. 
    AppendString(MYSQL_FUNCTION_TONUM);
    AppendString(OPEN_PARENTH);

    FdoPtr<FdoExpressionCollection> exprCol = expr.GetArguments();
    for(int i=0; i<exprCol->GetCount(); i++ )
    {
        if( i!= 0 )
            AppendString( L", " );

        FdoPtr<FdoExpression>exp = exprCol->GetItem( i );
        HandleExpr( exp );
    }

    // Add the keyword that performs the conversion and the closing bracket.
    AppendString( L", DECIMAL)" );
}

void FdoRdbmsMySqlFilterProcessor::ProcessToInt32Int64Function (FdoFunction& expr)
{
    // MySQL uses a different native function name for the expression functions
    // TOINT32, TOINT64. 
    AppendString(MYSQL_FUNCTION_TONUM);
    AppendString(OPEN_PARENTH);

    FdoPtr<FdoExpressionCollection> exprCol = expr.GetArguments();
    for(int i=0; i<exprCol->GetCount(); i++ )
    {
        if( i!= 0 )
            AppendString( L", " );

        FdoPtr<FdoExpression>exp = exprCol->GetItem( i );
        HandleExpr( exp );
    }

    // Add the keyword that performs the conversion and the closing bracket.
    AppendString( L", SIGNED)" );
}

void FdoRdbmsMySqlFilterProcessor::ProcessCurrentDateFunction (FdoFunction& expr)
{
    // MySQL uses a different native function name for the expression function
    // CurrentDate. 
    AppendString(MYSQL_FUNCTION_CURRENTDATE);
    AppendString(OPEN_PARENTH);

    // Note, that the function does not allow any arguments. However, if some
    // are given, they are added to the function call and it is left to MySQL
    // to deal with it.
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

//
// The TRIM function requires special processing.
void FdoRdbmsMySqlFilterProcessor::ProcessTrimFunction( FdoFunction& expr)
{
    // Append the function name and the opening bracket.
    ProcessFunctionName(expr);
	AppendString( "( " );

    // Process the arguments. This is were the special processing is required.
    // If the call includes an operator (BOTH, LEADING, TRAILING), it is required
    // to add a FROM clause after the operation keyword.
    FdoPtr<FdoExpressionCollection> exprCol = expr.GetArguments();
    for ( int i=0; i<exprCol->GetCount(); i++ )
    {
        FdoPtr<FdoExpression>exp = exprCol->GetItem( i );
        if ( (i == 0) && (IsDataValue( exp )) )
        {
            FdoDataValue *dataValue = (static_cast<FdoDataValue *>(exp.p) );
            if ( dataValue->GetDataType() == FdoDataType_String )
            {
                FdoStringValue *stringValue = static_cast<FdoStringValue *>(dataValue);
                AppendString( stringValue->GetString() );
                AppendString( " FROM " );	

            }
            else
                throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_29, "Unsupported FDO type in expression"));
        }
        else
            HandleExpr( exp );
	}
    AppendString( " )" );
}

bool FdoRdbmsMySqlFilterProcessor::IsDataValue (FdoExpression *expr)
{
    if (dynamic_cast<FdoDataValue *>(expr) != NULL)
        return true;

    return false;
}

void FdoRdbmsMySqlFilterProcessor::ProcessFunctionName (FdoFunction &expr)
{
    FdoRdbmsFilterProcessor::ProcessFunctionName(expr);
}

bool FdoRdbmsMySqlFilterProcessor::IsAggregateFunctionName(FdoString *wFunctionName) const
{
    for (int i=0; mySqlAggregateFunctions[i]; i++)
        if (FdoCommonOSUtil::wcsicmp(mySqlAggregateFunctions[i], wFunctionName) == 0)
            return true;

    return false;
}

//
// The function checks whether or not the provided function name has native
// support. 
bool FdoRdbmsMySqlFilterProcessor::IsNotNativeSupportedFunction(FdoString *wFunctionName) const
{
    for (int i=0; mySqlUnsupportedFdoFunctions[i]; i++)
        if (FdoCommonOSUtil::wcsicmp(mySqlUnsupportedFdoFunctions[i], wFunctionName) == 0)
            return true;

    return false;
}

//
// The function checks whether or not the function has a correct set of arguments.
bool FdoRdbmsMySqlFilterProcessor::HasNativeSupportedFunctionArguments(FdoFunction& expr) const
{
    // If the function needs argument checking, execute the verification and return
    // the result back to the calling routine. Otherwise, the arguments are always 
    // deemed valid and the corresponding indication is returned.

    if (FdoCommonOSUtil::wcsicmp(L"STDDEV", expr.GetName()) == 0)
    {
        // The signatures for the function STDDEV allow an optional first
        // parameter that identifies the operation type (ALL, DISTINCT). In
        // MySQL this is not natively supported. Therefore, instead of sending
        // the request to MySQL it needs to be redirected to the Expression
        // Engine.
        // The following checks for the number of arguments. If there are two
        // arguments, the request needs to be redirected to the Expression
        // Engine. 
        return (expr.GetArguments()->GetCount() > 1) ? false : true;
    }

    if (FdoCommonOSUtil::wcsicmp(L"TRUNC", expr.GetName()) == 0)
    {
        // The signatures for the function TRUNC allow the truncation of date
        // and numeric data. If the input is data then the request cannot be
        // handled by MySQL as this is not supported. In this case the request
        // needs to be handed to the Expression Engine.
        // NOTE: The current implementation hands the request to the Expression
        //       Engine in all cases. This needs to be updated later.
        return false;
    }

    return true;;
}
