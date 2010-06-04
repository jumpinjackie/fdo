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

#ifndef FDORDBMSMYSQLFILTERPROCESSOR_H_
#define FDORDBMSMYSQLFILTERPROCESSOR_H_

#ifdef _WIN32
#pragma once
#endif

#include "../../Fdo/Filter/FdoRdbmsFilterProcessor.h"

// FDO specifies a set of expression functions. The names of those functions may not
// match the function name in MySQL and hence a mapping of the names must be executed.
// The following defines the MySQL function name constants for the functions FDO supports
// that need name mappings.
//
// CONVERSION FUNCTIONS
//
#define  MYSQL_FUNCTION_NULLVALUE     L"IFNULL"         // Evaluates two expressions and returns the first one if it does not
                                                        // evaluate to NULL, the second otherwise
#define  MYSQL_FUNCTION_TODATE_A      L"CONVERT"        // Converts a string to a date if no format information is present
#define  MYSQL_FUNCTION_TODATE_B      L"STR_TO_DATE"    // Converts a string to a date if format information is present
#define  MYSQL_FUNCTION_TONUM         L"CONVERT"        // Converts a numeric or string expression to a numeric value
#define  MYSQL_FUNCTION_TOSTRING_DATE L"DATE_FORMAT"    // Converts a date expression to a string
#define  MYSQL_FUNCTION_TOSTRING_NUM  L"CONVERT"        // Converts a numeric expression to a string
//
// DATE FUNCTIONS
//
#define  MYSQL_FUNCTION_ADDMONTHS      L"ADDDATE"       // Adds a specified number of months to a date expression
#define  MYSQL_FUNCTION_CURRENTDATE    L"CURRENT_DATE"  // Returns the current date
//
// NUMERIC FUNCTIONS
//
#define  MYSQL_FUNCTION_TRUNC_NUM      L"TRUNCATE"      // Truncates a numeric expression
//
// STRING FUNCTIONS
//
#define  MYSQL_FUNCTION_SUBSTR         L"SUBSTRING"     // Extracts a substring from a string expression.


class FdoRdbmsMySqlFilterProcessor: public FdoRdbmsFilterProcessor
{

public:

    FdoRdbmsMySqlFilterProcessor(FdoRdbmsConnection *connection);
    ~FdoRdbmsMySqlFilterProcessor(void);

protected:

    virtual void ProcessDistanceCondition (FdoDistanceCondition &filter);
    virtual void ProcessSpatialCondition (FdoSpatialCondition &filter);
    const FdoSmLpGeometricPropertyDefinition *GetGeometricProperty (
                                const FdoSmLpClassDefinition *currentClass,
                                const wchar_t                *geomPropName);
    FdoStringP GetGeometryColumnNameForProperty (
                            const FdoSmLpGeometricPropertyDefinition *pGeomProp,
                            bool                                      bChangeFilter = false);
    void ProcessFunction (FdoFunction &expr);
    virtual void ProcessAggregateFunction (FdoFunction& expr);
    void ProcessToDoubleFloatFunction (FdoFunction& expr);
    void ProcessToInt32Int64Function (FdoFunction& expr);
    void ProcessCurrentDateFunction (FdoFunction& expr);
    void ProcessTrimFunction (FdoFunction& expr);
    void ProcessFunctionName (FdoFunction &expr);
    virtual bool IsAggregateFunctionName (FdoString *wFunctionName) const;
    virtual bool IsNotNativeSupportedFunction(FdoString* wFunctionName) const;
    virtual bool HasNativeSupportedFunctionArguments(FdoFunction& expr) const;

};

#endif // FDORDBMSMYSQLFILTERPROCESSOR_H_
