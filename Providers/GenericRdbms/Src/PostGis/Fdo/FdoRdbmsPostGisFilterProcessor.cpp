//
// Copyright (C) 2006 Refractions Research, Inc. 
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
#include "stdafx.h"
#include "FdoRdbmsPostGisFilterProcessor.h"
#include "../../Fdo/Schema/FdoRdbmsSchemaUtil.h"
#include "../../Fdo/Other/FdoRdbmsSpatialGeometryConverter.h"
#include <Geometry/Fgf/Factory.h>
#include <FdoCommonOSUtil.h>
#include <cassert>
#include <cmath>   // floor() and ceil()

//
// Get name of geometry column for the classname                                                                     */
//
#define NOTINDEXED_SUFFIX   L"_NIDX" 


//
// Used by FdoRdbmsBaseFilterProcessor() belo
//
#define NUMBER_FORMAT_SIZE  (100)

#define SPATIAL_BBOX_INTERSECT L" && "
#define SPATIAL_BBOX_CONTAINED L" @ ";
#define SPATIAL_BBOX_CONTAINS  L" ~ ";
#define SPATIAL_BBOX_EQUAL     L" = ";
#define SPATIAL_GEOM_EQUAL     L" ~= ";


// TODO: mloskot - check this array
//
// This list includes all the PostGIS aggregate functions.
// Not all functions are officially supported but they are
// not being prevented either.
//
static wchar_t* mySqlAggregateFunctions[] = {

    L"AVG",  // supported
    L"BIT_AND", 
    L"BIT_OR",
    L"BIT_XOR",
    L"COUNT",  // supported
    L"GROUP_CONCAT", 
    L"MAX",   // supported
    L"MIN",   // supported
    L"STD",
    L"STDDEV_POP",
    L"STDDEV_SAMP",
    L"SUM",  // supported
    L"VAR_POP",
    L"VAR_SAMP", 
    L"VARIANCE", 
    NULL
};

void bytes_to_hex(FdoByte* bytes, wchar_t* hexstr, FdoInt32 bytesSize)
{
    const char hexChars[] = "0123456789ABCDEF";
    FdoInt32 hexPos = 0;

    for (FdoInt32 i = 0; i < bytesSize; ++i)
    {
        FdoByte byte = bytes[i];
        hexstr[hexPos++] = hexChars[(byte >> 4) & 0xf]; 
        hexstr[hexPos++] = hexChars[byte & 0xf];
    }
    hexstr[hexPos++] = 0;
}

FdoRdbmsPostGisFilterProcessor::FdoRdbmsPostGisFilterProcessor(FdoRdbmsConnection *connection)
    : FdoRdbmsFilterProcessor(connection)
{
    // idle
}

FdoRdbmsPostGisFilterProcessor::~FdoRdbmsPostGisFilterProcessor(void)
{
    // idle
}

const FdoSmLpGeometricPropertyDefinition* FdoRdbmsPostGisFilterProcessor::GetGeometricProperty(
        const FdoSmLpClassDefinition* currentClass, 
        const wchar_t* geomPropName)
{
    const FdoSmLpGeometricPropertyDefinition* geom = NULL;

	if (NULL != currentClass)
	{
        if (NULL == geomPropName) 
        {
            // No property name specified, default to the GeometryProperty
            const FdoSmLpFeatureClass* feat = FdoSmLpFeatureClass::Cast(currentClass);

            if (NULL != feat) 
            {
                geom = feat->RefGeometryProperty();
            }
        }
        else
        {
            geom = FdoSmLpGeometricPropertyDefinition::Cast(
                    currentClass->RefProperties()->RefItem(geomPropName));
        }
	}

	return geom;
}

FdoStringP FdoRdbmsPostGisFilterProcessor::GetGeometryColumnNameForProperty(
        const FdoSmLpGeometricPropertyDefinition* pGeomProp,
        bool bChangeFilter)
{
	FdoStringP retVal;

    if (NULL != pGeomProp)
    {
	    retVal = pGeomProp->GetColumnName();

   		if (bChangeFilter) 
        {
            FdoStringP  tmpRetVal(retVal);

    	    FdoStringP suffixName = FdoStringP::Format(L"%ls",
                    static_cast<FdoString*>(tmpRetVal.Mid(wcslen(retVal) - wcslen(NOTINDEXED_SUFFIX),
                            wcslen(retVal))));

            // Strip the suffix from the column name
            if (0 == suffixName.ICompare(NOTINDEXED_SUFFIX))
            {
                retVal = FdoStringP::Format(L"%ls",
                        static_cast<FdoString*>(tmpRetVal.Mid(0, 
                                wcslen(retVal) - wcslen(NOTINDEXED_SUFFIX))));
            }
        }
	}

	return retVal;
}

void FdoRdbmsPostGisFilterProcessor::ProcessBooleanValue(FdoBooleanValue& expr)
{
    if( expr.GetBoolean() )
        AppendString( L"TRUE" );   // Database bool true
    else
        AppendString( L"FALSE" );  // Database bool false
}

void FdoRdbmsPostGisFilterProcessor::ProcessDistanceCondition(FdoDistanceCondition& filter)
{
    ProcessSpatialDistanceCondition( &filter, false);
}

void FdoRdbmsPostGisFilterProcessor::ProcessSpatialCondition(FdoSpatialCondition& filter)
{
    ProcessSpatialDistanceCondition( &filter, true);
}

void FdoRdbmsPostGisFilterProcessor::ProcessSpatialDistanceCondition(FdoGeometricCondition* geomCondition, bool isSpatial )
{
    FdoSpatialCondition *spatialFilter = NULL;
    FdoDistanceCondition *distFilter = NULL;
    if( isSpatial)
        spatialFilter = (FdoSpatialCondition*)geomCondition;
    else
        distFilter = (FdoDistanceCondition*)geomCondition;

    DbiConnection* mDbiConnection = NULL;
    mDbiConnection = mFdoConnection->GetDbiConnection();

    const FdoSmLpClassDefinition* classDefinition = NULL;
    classDefinition = mDbiConnection->GetSchemaUtil()->GetClass(mCurrentClassName);

    if (NULL == classDefinition || FdoClassType_FeatureClass != classDefinition->GetClassType() )
    {
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_230,
                    "Spatial condition can only be used with feature classes"));
    }

    // FdoException is thrown if GetPropertyName() returns NULL.
    const FdoSmLpGeometricPropertyDefinition* geomProp = NULL;
    geomProp = GetGeometricProperty(classDefinition,
            FdoPtr<FdoIdentifier>(geomCondition->GetPropertyName())->GetName());


    FdoInt64 srid = -1;

    // Get the SRID
	const FdoSmPhColumnP gColumn = ((FdoSmLpSimplePropertyDefinition*)geomProp)->GetColumn();
    if ( gColumn )
    {
        FdoSmPhColumnGeomP geomCol = gColumn.p->SmartCast<FdoSmPhColumnGeom>();
        if ( geomCol )
            srid = geomCol->GetSRID();
    }

    const FdoString* classTableName = classDefinition->GetDbObjectName();

    // Name of geometry table
    const FdoString* tableName = NULL;
    tableName = geomProp ? geomProp->GetContainingDbObjectName() : L"";

    // Name of geometry column
    FdoStringP columnName = GetGeometryColumnNameForProperty(geomProp, true);
    FdoStringP columnName2 = GetGeometryColumnNameForProperty(geomProp, false);

    FdoStringP spatialClause;

    // Fetch geometry value
    FdoGeometryValue *geom = NULL;
    if( isSpatial ) // The GetGeometry should have been defined on FdoGeometricCondition
        geom = dynamic_cast<FdoGeometryValue*>(spatialFilter->GetGeometry());
    else
        geom = dynamic_cast<FdoGeometryValue*>(distFilter->GetGeometry());

    // Convert geometry to FGF stream representation
    FdoByteArray* geomFgf = NULL;
    geomFgf = geom->GetGeometry();
    if (NULL == geomFgf)
    {
        throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_46, "No geometry value"));
    }

    // Geometry factory
    FdoFgfGeometryFactory* gf = NULL;
    gf = FdoFgfGeometryFactory::GetInstance();

    assert(NULL != gf);

    // Create Geometry object from FGF stream
    FdoIGeometry* geometryObj = NULL;
    geometryObj = gf->CreateGeometryFromFgf(geomFgf);

    FdoPtr<FdoIGeometry> geom2D = geometryObj;
    if ( geometryObj->GetDimensionality() != FdoDimensionality_XY )
	{
		FdoSpatialGeometryConverter *gc = new FdoRdbmsSpatialGeometryConverter();

		geom2D = gc->ConvertOrdinates( geometryObj, true, FdoDimensionality_XY, 0.0, 0.0);
        delete gc;
	}

    FdoPtr<FdoByteArray> wkbBytes(gf->GetWkb(geom2D));
    FdoByte* bytes = wkbBytes->GetData();

    wchar_t* hexstr = (wchar_t*) alloca( (wkbBytes->GetCount() * 2 + 1) * sizeof(wchar_t) );
    bytes_to_hex(bytes, hexstr, wkbBytes->GetCount());

    // Cache the constructor of tested geometry
    // TODO: convert geom to bind variable to eliminate WKB->Hex->WKB translation
    FdoStringP geomFromText = FdoStringP::Format(
        L"GeomFromWKB(decode('%ls','hex'), %ls)",
        hexstr,
        (FdoString*) FdoCommonStringUtil::Int64ToString(srid)
    );

    //
    // Build node of WHERE clause of SQL SELECT query
    //
    columnName = FdoStringP(L"\"") + columnName + FdoStringP(L"\"");
    if( isSpatial )
        BuildSpatialFilter( columnName, geomFromText, spatialFilter );
    else
        BuildDistanceFilter( columnName, geomFromText, distFilter );
}
void FdoRdbmsPostGisFilterProcessor::BuildSpatialFilter( FdoStringP columnName, FdoStringP geomFromText, FdoSpatialCondition *spatialFilter)
{
    // TODO: FUTURE NOTE
    // It may be a good idea to consider following extensions:
    // Contains() => A ~ B AND Contains(A,B)
    // Within() => A @ B AND Within(A,B)
    // etc.

    FdoStringP buf = OPEN_PARENTH;
    FdoStringP spatialPred;

    if (FdoSpatialOperations_EnvelopeIntersects == spatialFilter->GetOperation())
    { 
        buf = buf + L" "+ columnName;
        buf += SPATIAL_BBOX_INTERSECT;
        buf += geomFromText;
    }
    else
    {        
        // Apply && operator to all predicates but Disjoint()
        //
        // PostGIS manual says:
        // This operator tells whether the bounding box of one
        // geometry intersects the bounding box of another.

        if (FdoSpatialOperations_Disjoint != spatialFilter->GetOperation())
        {
            buf = buf + L" " + columnName;
            buf += SPATIAL_BBOX_INTERSECT;
            buf += geomFromText;
            buf += LOGICAL_AND;
        }

        switch (spatialFilter->GetOperation())
        {
        case FdoSpatialOperations_Contains:
            spatialPred = L"Contains";
            break;
        case FdoSpatialOperations_Crosses:
            spatialPred = L"Crosses";
            break;
        case FdoSpatialOperations_Disjoint:
            spatialPred = L"Disjoint";
            break;
        case FdoSpatialOperations_Equals:
            spatialPred = L"Equals";
            break;
        case FdoSpatialOperations_Intersects:
            spatialPred = L"Intersects";
            break;
        case FdoSpatialOperations_Overlaps:
            spatialPred = L"Overlaps";
            break;
        case FdoSpatialOperations_Touches:
            spatialPred = L"Touches";
            break;
        case FdoSpatialOperations_Within:
        case FdoSpatialOperations_Inside:
            spatialPred = L"Within";
            break;
        default:
            throw FdoFilterException::Create(NlsMsgGet(FDORDBMS_111, "Unsupported spatial operation"));
        }

        buf += spatialPred + L"(" + columnName + L"," + geomFromText + L")";
    }

    buf += CLOSE_PARENTH;

    // Add envelope to the filter SQL buffer 
    AppendString((FdoString*) buf);
}

void FdoRdbmsPostGisFilterProcessor::BuildDistanceFilter( FdoStringP columnName, FdoStringP geomFromText, FdoDistanceCondition *distFilter)
{
    FdoStringP buf = OPEN_PARENTH;

    FdoStringP distTxt = FdoStringP::Format(L"%f",distFilter->GetDistance());
    switch(distFilter->GetOperation())
    {
    case FdoDistanceOperations_Within:
        {
            buf += (columnName + " && Expand(" + geomFromText + ", " + distTxt + ") ");
            buf += LOGICAL_AND;
            buf += "distance(";
            buf += (columnName + "," + geomFromText + ")");
            buf += LESS_OR_EQUAL_OP;
            buf += (distTxt);
        }
        break;
    case FdoDistanceOperations_Beyond:
        {
            buf += "distance(";
            buf += (columnName + "," + geomFromText + ")");
            buf += GREATER_THAN_OP;
            buf += (distTxt);
        }
        break;
    default:
        {
            throw FdoFilterException::Create(L"Unsupported Distance operation given in the filter");
        }
    }
    buf += CLOSE_PARENTH;

    AppendString((FdoString*) buf);
}

bool FdoRdbmsPostGisFilterProcessor::IsAggregateFunctionName(FdoString* wFunctionName) const
{
    for (int i = 0; mySqlAggregateFunctions[i]; ++i)
    {
        if (0 == FdoCommonOSUtil::wcsicmp(mySqlAggregateFunctions[i], wFunctionName))
        {
            return true;
        }
    }

    return false;
}

void FdoRdbmsPostGisFilterProcessor::ProcessFunction(FdoFunction& expr)
{
    FdoStringP name(expr.GetName());
    FdoPtr<FdoExpressionCollection> args(expr.GetArguments());
    FdoInt32 nbArgs = args->GetCount();
    std::string sep;
    std::string argSep;
    std::string argCast;
    bool processArgs = true;
    char const* sepComma     = " , ";

    if (0 == name.ICompare(FDO_FUNCTION_CONCAT))
    {
      argSep = "";
      AppendString(OPEN_PARENTH);
      AppendString(L"''||"); // make sure to start with a string.
      for (FdoInt32 i = 0; i < args->GetCount(); i++)
      {
          AppendString(argSep.c_str());
          FdoPtr<FdoExpression> subExpr(args->GetItem(i));
          subExpr->Process(this);
          argSep = " || ";
      }
      AppendString(CLOSE_PARENTH);
      processArgs = false;
    }
    else if ( (0 == name.ICompare(FDO_FUNCTION_COUNT)) ||
              (0 == name.ICompare(FDO_FUNCTION_AVG))   ||
              (0 == name.ICompare(FDO_FUNCTION_MAX))   ||
              (0 == name.ICompare(FDO_FUNCTION_MIN))   ||
              (0 == name.ICompare(FDO_FUNCTION_SUM))   ||
              (0 == name.ICompare(FDO_FUNCTION_STDDEV))
    ) {
        // The aggregate functions have optional 1st argument "DISTINCT" or "ALL", which is space-separated
        // from the argument being aggregated. ProcessAggregateFunction prevents optional 
        // argument from being quote delimited.
        ProcessAggregateFunction(expr);
        processArgs = false;
    }
    else if (0 == name.ICompare(FDO_FUNCTION_SPATIALEXTENTS))
    {
        processArgs = false;
        AppendString(L"Coalesce(GeomFromEwkb(AsEwkb(Extent(");
        FdoPtr<FdoExpression> geomExpr(args->GetItem(0));
        geomExpr->Process(this);
        AppendString(L"))),geomfromtext('POLYGON((0 0,0 50,50 50,50 0,0 0))'))");    
    }
    else if (0 == name.ICompare(FDO_FUNCTION_AREA2D))
    {
        AppendString(L"Area");
        argSep = sepComma;
    }
    else if (0 == name.ICompare(FDO_FUNCTION_NULLVALUE))
    {
        AppendString(L"Coalesce");
        argSep = sepComma;
    }
    else if (0 == name.ICompare(FDO_FUNCTION_INSTR))
    {
        AppendString(L"Strpos");
        argSep = sepComma;
    }
    else if (0 == name.ICompare(FDO_FUNCTION_TODATE))
    {
        if(nbArgs == 1)
        {
          AppendString(L"Cast");
          argCast = " as timestamp ";
        }
        else
        {
          AppendString(L"to_date");
          argSep = sepComma;
        }
    }
    else if (0 == name.ICompare(FDO_FUNCTION_TOSTRING))
    {
        if(nbArgs == 1)
        {
          AppendString(L"Cast");
          argCast = " as text ";
        }
        else
        {
          AppendString(L"to_char");
          argSep = sepComma;
        }
    }
    else if (0 == name.ICompare(FDO_FUNCTION_TODOUBLE))
    {
        AppendString(L"Cast");
        argCast = " as float8 ";
    }
    else if (0 == name.ICompare(FDO_FUNCTION_TOFLOAT))
    {
        AppendString(L"Cast");
        argCast = " as float4 ";
    }
    else if (0 == name.ICompare(FDO_FUNCTION_TOINT32))
    {
        // Fdo toint32 truncates non-integral values while
        // PosgreSQL cast rounds them. Add trunc function to get
        // FDO behaviour.
        processArgs = false;
        FdoPtr<FdoExpression> expr(args->GetItem(0));
        // Inner cast to numeric handles string argument.
        AppendString(L"cast(trunc(cast((");
        expr->Process(this);
        AppendString(L") as numeric)) as int4)");
    }
    else if (0 == name.ICompare(FDO_FUNCTION_TOINT64))
    {
        // Fdo toint64 truncates non-integral values while
        // PosgreSQL cast rounds them. Add trunc function to get
        // FDO behaviour.
        processArgs = false;
        FdoPtr<FdoExpression> expr(args->GetItem(0));
        // Inner cast to numeric handles string argument.
        AppendString(L"cast(trunc(cast((");
        expr->Process(this);
        AppendString(L") as numeric)) as int8)");
    }
    else if (0 == name.ICompare(FDO_FUNCTION_CURRENTDATE))
    {
        AppendString(L"now");
        argSep = sepComma;
    }
    else if (0 == name.ICompare(FDO_FUNCTION_EXTRACTTODOUBLE))
    {
        AppendString(L"Date_part");
        argSep = sepComma;
    }
    else if (0 == name.ICompare(FDO_FUNCTION_EXTRACTTOINT))
    {
        processArgs = false;
        AppendString(L"Cast(Date_part(");
        FdoPtr<FdoExpression> dateExpr1(args->GetItem(0));
        dateExpr1->Process(this);
        AppendString(L",");
        FdoPtr<FdoExpression> dateExpr2(args->GetItem(1));
        dateExpr2->Process(this);
        AppendString(L") as int4)");
    }

    else if (0 == name.ICompare(FDO_FUNCTION_ADDMONTHS))
    {
        //AddMonth(date,3) = date + interval '3 month'
        processArgs = false;
        AppendString(OPEN_PARENTH);
        FdoPtr<FdoExpression> dateExpr(args->GetItem(0));
        dateExpr->Process(this);
        AppendString(L" + ");
        FdoPtr<FdoExpression> numExpr(args->GetItem(1));
        FdoStringP numMonth(numExpr->ToString());
        AppendString(L"interval '");
        AppendString(static_cast<char const*>(numMonth));
        AppendString(L" month'");
        AppendString(CLOSE_PARENTH);
    }
    else if (0 == name.ICompare(FDO_FUNCTION_MONTHSBETWEEN))
    {
        //MonthBetween(date1,date2) = date_part('Month',age(date1,date2)) + date_part('Year',age(date1,date2)) * 12
        processArgs = false;
        FdoPtr<FdoExpression> date1Expr(args->GetItem(0));
        FdoPtr<FdoExpression> date2Expr(args->GetItem(1));

        AppendString(L"date_part('Month',age(");
        date2Expr->Process(this);
        AppendString(sepComma);
        date1Expr->Process(this);
        AppendString(CLOSE_PARENTH);
        AppendString(CLOSE_PARENTH);

        AppendString(L" + ");

        AppendString(L"date_part('Year',age(");
        date2Expr->Process(this);
        AppendString(sepComma);
        date2Expr->Process(this);
        AppendString(CLOSE_PARENTH);
        AppendString(CLOSE_PARENTH);
        AppendString(L" * 12 ");
    }
    //Math function with 2 args that only accept numeric 
    else if ((nbArgs == 2) &&
             (0 == name.ICompare(FDO_FUNCTION_LOG) ||
              0 == name.ICompare(FDO_FUNCTION_MOD)))
            
    {
        //Round(double,1) -> Round(cast(double as numeric) , 1)
        processArgs = false;
        AppendString(static_cast<char const*>(name));
        AppendString(OPEN_PARENTH);
        AppendString(L"Cast(");
        FdoPtr<FdoExpression> numExpr1(args->GetItem(0));
        numExpr1->Process(this);
        AppendString(L" as numeric)");
        AppendString(sepComma);
        AppendString(L"Cast(");
        FdoPtr<FdoExpression> numExpr2(args->GetItem(1));
        numExpr2->Process(this);
        AppendString(L" as numeric)");
        AppendString(CLOSE_PARENTH);
    }
    
    //Math function with 2 args that only accept numeric 
    else if ((nbArgs == 2) &&
              (0 == name.ICompare(FDO_FUNCTION_ROUND)))
    {
        //Round(double,1) -> Round(cast(double as numeric) , 1)
        processArgs = false;
        AppendString(static_cast<char const*>(name));
        AppendString(OPEN_PARENTH);
        AppendString(L"Cast(");
        FdoPtr<FdoExpression> numExpr1(args->GetItem(0));
        numExpr1->Process(this);
        AppendString(L" as numeric)");
        AppendString(sepComma);
        AppendString(L"Cast(Round(Cast(");
        FdoPtr<FdoExpression> numExpr2(args->GetItem(1));
        numExpr2->Process(this);
        AppendString(L" as numeric)) as int)");
        AppendString(CLOSE_PARENTH);
    }
    else if ((nbArgs == 2) &&
               (0 == name.ICompare(FDO_FUNCTION_TRUNC)))
    {
        FdoPtr<FdoExpression> numExpr1(args->GetItem(0));
        FdoPtr<FdoExpression> numExpr2(args->GetItem(1));
        bool dateTrunc = false;
        
        // Check the data type for precision:
        //     If numeric then assume truncating a number
        //     If string then assume truncating a datetime to 'year', 'month', or 'day', etc.
        //     If precision is not a simple data type then fall back to processing 
        //     through Expression Engine (See HasNativeSupportedFunctionArgument()). 
        FdoStringValue* precisionVal  = dynamic_cast<FdoStringValue*>(numExpr2.p);
        if ( precisionVal && !precisionVal->IsNull()) 
        {
            FdoStringP precisionStr = precisionVal->GetString();
            if ( !precisionStr.IsNumber() )
                dateTrunc = true;
        }

        processArgs = false;
        if ( dateTrunc )
        {
            //Trunc(datetime,str) -> Trunc(str, datetime)
            AppendString(L"date_trunc");
            AppendString(OPEN_PARENTH);
            numExpr2->Process(this);
            AppendString(sepComma);
            numExpr1->Process(this);
            AppendString(CLOSE_PARENTH);
        }
        else
        {
            //Trunc(double,1) -> Trunc(cast(double as numeric) , 1)
            AppendString(static_cast<char const*>(name));
            AppendString(OPEN_PARENTH);
            AppendString(L"Cast(");
            numExpr1->Process(this);
            AppendString(L" as numeric)");
            AppendString(sepComma);
            AppendString(L"Cast(Round(Cast(");
            numExpr2->Process(this);
            AppendString(L" as numeric)) as int)");
            AppendString(CLOSE_PARENTH);
        }
    }
    
    else
    {
        AppendString(static_cast<char const*>(name));
        argSep = sepComma;
    }


    if(processArgs)
    {
      AppendString(OPEN_PARENTH);

      for (FdoInt32 i = 0; i < args->GetCount(); i++)
      {
          AppendString(sep.c_str());
          FdoPtr<FdoExpression> subExpr(args->GetItem(i));
          subExpr->Process(this);
          if(argCast != "")
          {
            AppendString(argCast.c_str());
          }
          sep = argSep;
      }
      AppendString(CLOSE_PARENTH);
    }
}

//
// The function checks whether or not the provided function name has native
// support. 
bool FdoRdbmsPostGisFilterProcessor::IsNotNativeSupportedFunction(FdoString *wFunctionName) const
{
    for (int i=0; PostGisSupportedFunctions[i]; i++)
        if (FdoCommonOSUtil::wcsicmp(PostGisSupportedFunctions[i], wFunctionName) == 0)
            return false;
    return true;
}

//
// The function checks whether or not the function has a correct set of arguments.
bool FdoRdbmsPostGisFilterProcessor::HasNativeSupportedFunctionArguments(FdoFunction& expr) const
{
    bool ret = true;
    // If the function needs argument checking, execute the verification and return
    // the result back to the calling routine. Otherwise, the arguments are always 
    // deemed valid and the corresponding indication is returned.

    if ( FdoStringP(expr.GetName(),false).ICompare(FDO_FUNCTION_TRUNC) == 0 )
    {
        // Truncate function maps onto a different PostgreSQL function, depending on
        // whether truncating a numeric or datetime.
        FdoPtr<FdoExpressionCollection> args(expr.GetArguments());
        FdoInt32 nbArgs = args->GetCount();

        if ( nbArgs == 2 ) 
        {
            // 2nd argument is a string when truncating a datetime and a number when
            // truncating a number. If 2nd argument is a DataValue then we can easily
            // determine its type ( see ProcessFunction() ).
            FdoPtr<FdoExpression> subExpr(args->GetItem(1));
            FdoDataValue* dv = dynamic_cast<FdoDataValue*>(subExpr.p);

            if ( !dv ) 
                // 2nd argument is a more complex expression so we don't know whether
                // to translate to PostgreSQL trunc() or date_trunc() function.
                // Fall back to processing through Expression Engine.
                // TODO: Add the ability to determine resulting type of an expression
                // so that trunc can always be handled natively.
                ret = false;
        }
    }

    return ret;
}
