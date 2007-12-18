#ifndef FDORDBMSSQLSERVERFILTERPROCESSOR_H
#define FDORDBMSSQLSERVERFILTERPROCESSOR_H     1
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

#include "../../Fdo/Filter/FdoRdbmsFilterProcessor.h"

// FDO specifies a set of expression functions. The names of those functions may not
// match the function name in SQL Server and hence a mapping of the names must be executed.
// The following defines the SQL Server function name constants for the functions FDO supports
// that need name mappings.
//
// AGGREGATE FUNCTIONS
//
#define  SQLSERVER_FUNCTION_STDDEV         L"STDEV"          // Determines the sample deviation.
#define  SQLSERVER_FUNCTION_SPATIALEXTENTS L"STEnvelope"     // Evaluate the spatial extents.
//
// CONVERSION FUNCTIONS
//
#define  SQLSERVER_FUNCTION_NULLVALUE      L"ISNULL"         // Evaluates two expressions and returns the first one if it does not
                                                             // evaluate to NULL, the second otherwise
#define  SQLSERVER_FUNCTION_TODOUBLEFLOAT  L"CONVERT"        // Converts a numeric or string expression to a numeric value
#define  SQLSERVER_FUNCTION_TOINT          L"FLOOR(CONVERT"  // Converts a numeric or string expression to a numeric value
#define  SQLSERVER_FUNCTION_TOSTRING       L"CONVERT"        // Converts a numeric expression to a string
//
// DATE FUNCTIONS
//
#define  SQLSERVER_FUNCTION_ADDMONTHS      L"DATEADD"        // Adds a specified number of months to a date expression
#define  SQLSERVER_FUNCTION_CURRENTDATE    L"GETDATE"        // Returns the current date
#define  SQLSERVER_FUNCTION_EXTRACT        L"DATEPART"       // Extracts a specified portion of a date
#define  SQLSERVER_FUNCTION_MONTHSBETWEEN  L"DATEDIFF"       // Returns the number of months between two dates.
//
// MATHEMATIC FUNCTIONS
//
#define  SQLSERVER_FUNCTION_ATAN2          L"ATN2"           // Determines the arc tangent based on two numeric expressions.
#define  SQLSERVER_FUNCTION_LOG            L"LOG"            // Determines the logarithm of a numeric value on the provided
                                                             // base.
//
// NUMERIC FUNCTIONS
//
#define  SQLSERVER_FUNCTION_CEIL           L"CEILING"        // Determines the smallest integer greater than or equal to a numeric
                                                             // expression.
#define  SQLSERVER_FUNCTION_TRUNC          L"ROUND"          // Truncates a numeric expression.
//
// STRING FUNCTIONS
//
#define  SQLSERVER_FUNCTION_LENGTH         L"LEN"            // Returns the length of a string expression.


class FdoRdbmsSqlServerFilterProcessor: public FdoRdbmsFilterProcessor
{
public:
    FdoRdbmsSqlServerFilterProcessor(FdoRdbmsConnection *connection);

    ~FdoRdbmsSqlServerFilterProcessor(void);

	virtual void GetLtTableExpression( const FdoSmLpClassDefinition *classDefinition, FdoStringP &ltJoin, FdoStringP &ltTableExp, FdoCommandType callerFdoCommand );
	virtual void GetLtQualificationClause( const FdoSmLpClassDefinition *classDefinition, FdoStringP &ltQualificationClause );

protected:

    // SQL Server is using update hint. 
    virtual void AppendForUpdate() { } ;

    virtual void AppendTablesHints( SqlCommandType cmdType, bool forUpdate );

    virtual void ProcessDistanceCondition(FdoDistanceCondition& filter);

    virtual void ProcessSpatialCondition(FdoSpatialCondition& filter);

    const FdoSmLpGeometricPropertyDefinition* GetGeometricProperty( const FdoSmLpClassDefinition* currentClass, const wchar_t *geomPropName );
	
    FdoStringP GetGeometryColumnNameForProperty( const FdoSmLpGeometricPropertyDefinition *pGeomProp, bool bChangeFilter = false);

    virtual void ProcessFunction(FdoFunction& expr);
    void ProcessAggregateFunction(FdoFunction& expr);
    void ProcessNullValueFunction (FdoFunction& expr);
    void ProcessToDoubleFunction (FdoFunction& expr);
    void ProcessToFloatFunction (FdoFunction& expr);
    void ProcessToIntFunction (FdoFunction& expr);
    void ProcessAddMonthsFunction (FdoFunction& expr);
    void ProcessMonthsBetweenFunction (FdoFunction& expr);
    void ProcessLogFunction (FdoFunction& expr);
    void ProcessRoundFunction (FdoFunction& expr);
    void ProcessConcatFunction(FdoFunction& expr);
    void ProcessSpatialExtentsFunction(FdoFunction& expr);
    FdoString *MapFdoFunction2SqlServerFunction (FdoString *f_name);
    virtual void ProcessFunctionName(FdoFunction& expr);

    virtual bool IsAggregateFunctionName(FdoString* wFunctionName) const;
    bool IsDataValue (FdoExpression *expr);
    virtual bool IsNotNativeSupportedFunction(FdoString* wFunctionName) const;
    virtual bool HasNativeSupportedFunctionArguments(FdoFunction& expr) const;

    virtual void ProcessStringValue(FdoStringValue& expr);
    void ProcessDateTimeValue(FdoDateTimeValue& expr);

    virtual FdoStringP GetGeometryString( FdoString* columnName );
    virtual void PrependSelectStar(FdoString* tableName);
};

#endif
