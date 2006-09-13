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
#include "FdoRdbmsExpressionCapabilities.h"

FdoRdbmsExpressionCapabilities::FdoRdbmsExpressionCapabilities(void)
{
}

FdoRdbmsExpressionCapabilities::~FdoRdbmsExpressionCapabilities(void)
{
}

FdoExpressionType* FdoRdbmsExpressionCapabilities::GetExpressionTypes(int& length)
{
    static FdoExpressionType expressionTypes[] = {  FdoExpressionType_Basic,
                                                    FdoExpressionType_Function,
                                                    FdoExpressionType_Parameter};
    length = sizeof(expressionTypes)/sizeof(FdoExpressionType);
    return expressionTypes;
}

FdoFunctionDefinitionCollection* FdoRdbmsExpressionCapabilities::GetFunctions()
{
    FdoString *desc = NULL;
    FdoPtr<FdoFunctionDefinition> func;

    // ------------------------------------------------------------------------------------
    // The following defines the functions and their signatures supported by the provider.
    // NOTE: The original code retrieves the list of well-known functions from FDO. The
    //       functions defined there provide a single signature for each of the specified
    //       routines. Because the signature list available in this procedure for each of
    //       the well-known functions is a read-only list and hence does not allow to add
    //       additional signatures to the list, the list of supported functions for this
    //       provider is constructed here.
    // ------------------------------------------------------------------------------------

    // Create the basic argument collections being referenced by the supported functions.
    desc = FdoException::NLSGetMessage(FUNCTION_BOOL_ARG,"Argument that represents a boolean");
    FdoPtr<FdoArgumentDefinition>boolParm = FdoArgumentDefinition::Create(L"boolValue", desc, FdoDataType_Boolean);
    FdoPtr<FdoArgumentDefinitionCollection>boolParms = FdoArgumentDefinitionCollection::Create();
    boolParms->Add(boolParm);

    desc = FdoException::NLSGetMessage(FUNCTION_BYTE_ARG,"Argument that represents a byte");
    FdoPtr<FdoArgumentDefinition>byteParm = FdoArgumentDefinition::Create(L"byteValue", desc, FdoDataType_Byte);
    FdoPtr<FdoArgumentDefinitionCollection>byteParms = FdoArgumentDefinitionCollection::Create();
    byteParms->Add(byteParm);

    desc = FdoException::NLSGetMessage(FUNCTION_DT_ARG,"Argument that represents a date-time");
    FdoPtr<FdoArgumentDefinition>dateTimeParm = FdoArgumentDefinition::Create(L"dateTimeValue", desc, FdoDataType_DateTime);
    FdoPtr<FdoArgumentDefinitionCollection>dateTimeParms = FdoArgumentDefinitionCollection::Create();
    dateTimeParms->Add(dateTimeParm);

    desc = FdoException::NLSGetMessage(FUNCTION_DEC_ARG,"Argument that represents a decimal");
    FdoPtr<FdoArgumentDefinition>decimalParm = FdoArgumentDefinition::Create(L"decimalValue", desc, FdoDataType_Decimal);
    FdoPtr<FdoArgumentDefinitionCollection>decimalParms = FdoArgumentDefinitionCollection::Create();
    decimalParms->Add(decimalParm);

    desc = FdoException::NLSGetMessage(FUNCTION_DBL_ARG,"Argument that represents a double");
    FdoPtr<FdoArgumentDefinition>doubleParm = FdoArgumentDefinition::Create(L"dblValue", desc, FdoDataType_Double);
    FdoPtr<FdoArgumentDefinitionCollection>doubleParms = FdoArgumentDefinitionCollection::Create();
    doubleParms->Add(doubleParm);

    desc = FdoException::NLSGetMessage(FUNCTION_INT16_ARG, "Argument that represents a 16-bit integer");
    FdoPtr<FdoArgumentDefinition>int16Parm = FdoArgumentDefinition::Create(L"intValue", desc, FdoDataType_Int16);
    FdoPtr<FdoArgumentDefinitionCollection> int16Parms = FdoArgumentDefinitionCollection::Create();
    int16Parms->Add(int16Parm);

    desc = FdoException::NLSGetMessage(FUNCTION_INT32_ARG, "Argument that represents a 32-bit integer");
    FdoPtr<FdoArgumentDefinition>int32Parm = FdoArgumentDefinition::Create(L"intValue", desc, FdoDataType_Int32);
    FdoPtr<FdoArgumentDefinitionCollection> int32Parms = FdoArgumentDefinitionCollection::Create();
    int32Parms->Add(int32Parm);

    desc = FdoException::NLSGetMessage(FUNCTION_INT64_ARG, "Argument that represents a 64-bit integer");
    FdoPtr<FdoArgumentDefinition>int64Parm = FdoArgumentDefinition::Create(L"intValue", desc, FdoDataType_Int64);
    FdoPtr<FdoArgumentDefinitionCollection> int64Parms = FdoArgumentDefinitionCollection::Create();
    int64Parms->Add(int64Parm);

    desc = FdoException::NLSGetMessage(FUNCTION_SINGLE_ARG, "Argument that represents a single");
    FdoPtr<FdoArgumentDefinition>singleParm = FdoArgumentDefinition::Create(L"singleValue", desc, FdoDataType_Single);
    FdoPtr<FdoArgumentDefinitionCollection>singleParms = FdoArgumentDefinitionCollection::Create();
    singleParms->Add(singleParm);

    desc = FdoException::NLSGetMessage(FUNCTION_STR_ARG, "Argument that represents a string");
    FdoPtr<FdoArgumentDefinition>strParm = FdoArgumentDefinition::Create(L"strValue", desc, FdoDataType_String);
    FdoPtr<FdoArgumentDefinitionCollection>strParms = FdoArgumentDefinitionCollection::Create();
    strParms->Add(strParm);

    FdoPtr<FdoArgumentDefinition>concatParm = FdoArgumentDefinition::Create(L"strValue1", desc, FdoDataType_String);
    FdoPtr<FdoArgumentDefinitionCollection>concatParms = FdoArgumentDefinitionCollection::Create();
    concatParms->Add(concatParm);
    concatParm = FdoArgumentDefinition::Create(L"strValue2", desc, FdoDataType_String);
    concatParms->Add(concatParm);

    // Create the required function collection.
    FdoFunctionDefinitionCollection* functions = FdoFunctionDefinitionCollection::Create();

    // Add the function AVG. The function accepts a parameter of type decimal, double, int16,
    // int32, int64 or single and always returns a double.
    FdoPtr<FdoSignatureDefinition> avgSignatureDef;
    FdoPtr<FdoSignatureDefinitionCollection> avgSignatureDefCol = FdoSignatureDefinitionCollection::Create();

    avgSignatureDef = FdoSignatureDefinition::Create(FdoDataType_Double, decimalParms);
    avgSignatureDefCol->Add(avgSignatureDef);
    avgSignatureDef = FdoSignatureDefinition::Create(FdoDataType_Double, doubleParms);
    avgSignatureDefCol->Add(avgSignatureDef);
    avgSignatureDef = FdoSignatureDefinition::Create(FdoDataType_Double, int16Parms);
    avgSignatureDefCol->Add(avgSignatureDef);
    avgSignatureDef = FdoSignatureDefinition::Create(FdoDataType_Double, int32Parms);
    avgSignatureDefCol->Add(avgSignatureDef);
    avgSignatureDef = FdoSignatureDefinition::Create(FdoDataType_Double, int64Parms);
    avgSignatureDefCol->Add(avgSignatureDef);
    avgSignatureDef = FdoSignatureDefinition::Create(FdoDataType_Double, singleParms);
    avgSignatureDefCol->Add(avgSignatureDef);

    desc = FdoException::NLSGetMessage(FUNCTION_AVG, "Returns the average value of an expression");
    func = FdoFunctionDefinition::Create(FDO_FUNCTION_AVG, desc, true, avgSignatureDefCol);
    functions->Add(func);

    // Add the function CEIL. The function accepts a parameter of type decimal, double or single
    // and returns the same data type as the input parameter.
    FdoPtr<FdoSignatureDefinition> ceilSignatureDef;
    FdoPtr<FdoSignatureDefinitionCollection> ceilSignatureDefCol = FdoSignatureDefinitionCollection::Create();

    ceilSignatureDef = FdoSignatureDefinition::Create(FdoDataType_Decimal, decimalParms);
    ceilSignatureDefCol->Add(ceilSignatureDef);
    ceilSignatureDef = FdoSignatureDefinition::Create(FdoDataType_Double, doubleParms);
    ceilSignatureDefCol->Add(ceilSignatureDef);
    ceilSignatureDef = FdoSignatureDefinition::Create(FdoDataType_Single, singleParms);
    ceilSignatureDefCol->Add(ceilSignatureDef);

    desc = FdoException::NLSGetMessage(FUNCTION_CEIL, "Returns the smallest integer greater than or equal to the expression value");
	func = FdoFunctionDefinition::Create(FDO_FUNCTION_CEIL, desc, false, ceilSignatureDefCol);
    functions->Add(func);

    // Add the function CONCAT. The function accepts two parameters of type string and always returns
    // a string.
    FdoPtr<FdoSignatureDefinition> concatSignatureDef;
    FdoPtr<FdoSignatureDefinitionCollection> concatSignatureDefCol = FdoSignatureDefinitionCollection::Create();

    concatSignatureDef = FdoSignatureDefinition::Create(FdoDataType_String, concatParms);
    concatSignatureDefCol->Add(concatSignatureDef);

    desc = FdoException::NLSGetMessage(FUNCTION_CONCAT, "Returns strings with all letters lowercase");
    func = FdoFunctionDefinition::Create(FDO_FUNCTION_CONCAT, desc, false, concatSignatureDefCol);
    functions->Add(func);

    // Add the function COUNT. The function accepts a parameter of type boolean, byte, datetime,
    // decimal, double, int16, int32, int64, single, string, BLOB or CLOB and always returns an int64.
    FdoPtr<FdoSignatureDefinition> countSignatureDef;
    FdoPtr<FdoSignatureDefinitionCollection> countSignatureDefCol = FdoSignatureDefinitionCollection::Create();

    countSignatureDef = FdoSignatureDefinition::Create(FdoDataType_Int64, boolParms);
    countSignatureDefCol->Add(countSignatureDef);
    countSignatureDef = FdoSignatureDefinition::Create(FdoDataType_Int64, byteParms);
    countSignatureDefCol->Add(countSignatureDef);
    countSignatureDef = FdoSignatureDefinition::Create(FdoDataType_Int64, dateTimeParms);
    countSignatureDefCol->Add(countSignatureDef);
    countSignatureDef = FdoSignatureDefinition::Create(FdoDataType_Int64, decimalParms);
    countSignatureDefCol->Add(countSignatureDef);
    countSignatureDef = FdoSignatureDefinition::Create(FdoDataType_Int64, doubleParms);
    countSignatureDefCol->Add(countSignatureDef);
    countSignatureDef = FdoSignatureDefinition::Create(FdoDataType_Int64, int16Parms);
    countSignatureDefCol->Add(countSignatureDef);
    countSignatureDef = FdoSignatureDefinition::Create(FdoDataType_Int64, int32Parms);
    countSignatureDefCol->Add(countSignatureDef);
    countSignatureDef = FdoSignatureDefinition::Create(FdoDataType_Int64, int64Parms);
    countSignatureDefCol->Add(countSignatureDef);
    countSignatureDef = FdoSignatureDefinition::Create(FdoDataType_Int64, singleParms);
    countSignatureDefCol->Add(countSignatureDef);
    countSignatureDef = FdoSignatureDefinition::Create(FdoDataType_Int64, strParms);
    countSignatureDefCol->Add(countSignatureDef);

    desc = FdoException::NLSGetMessage(FUNCTION_COUNT, "Returns the number of objects in the query");
    func = FdoFunctionDefinition::Create(FDO_FUNCTION_COUNT, desc, true, countSignatureDefCol);
    functions->Add(func);

    // Add the function FLOOR. The function accepts a parameter of type decimal, double or single
    // and returns the same data type as the input parameter.
    FdoPtr<FdoSignatureDefinition> floorSignatureDef;
    FdoPtr<FdoSignatureDefinitionCollection> floorSignatureDefCol = FdoSignatureDefinitionCollection::Create();

    floorSignatureDef = FdoSignatureDefinition::Create(FdoDataType_Decimal, decimalParms);
    floorSignatureDefCol->Add(floorSignatureDef);
    floorSignatureDef = FdoSignatureDefinition::Create(FdoDataType_Double, doubleParms);
    floorSignatureDefCol->Add(floorSignatureDef);
    floorSignatureDef = FdoSignatureDefinition::Create(FdoDataType_Single, singleParms);
    floorSignatureDefCol->Add(floorSignatureDef);

    desc = FdoException::NLSGetMessage(FUNCTION_FLOOR, "Returns the largest integer equal to or less than the expression value");
    func = FdoFunctionDefinition::Create(FDO_FUNCTION_FLOOR, desc, false, floorSignatureDefCol);
    functions->Add(func);

    // Add the function LOWER. The function accepts two parameters of type string and always returns
    // a string.
    FdoPtr<FdoSignatureDefinition> lowerSignatureDef;
    FdoPtr<FdoSignatureDefinitionCollection> lowerSignatureDefCol = FdoSignatureDefinitionCollection::Create();

    lowerSignatureDef = FdoSignatureDefinition::Create(FdoDataType_String, strParms);
    lowerSignatureDefCol->Add(lowerSignatureDef);

    desc = FdoException::NLSGetMessage(FUNCTION_LOWER, "Returns strings with all letters lowercase");
    func = FdoFunctionDefinition::Create(FDO_FUNCTION_LOWER, desc, false, lowerSignatureDefCol);
    functions->Add(func);

    // Add the function MAX. The function accepts a parameter of type byte, date-time, decimal,
    // double, int16, int32, int64, single or string and returns the same data type as the input
    // parameter.
    FdoPtr<FdoSignatureDefinition> maxSignatureDef;
    FdoPtr<FdoSignatureDefinitionCollection> maxSignatureDefCol = FdoSignatureDefinitionCollection::Create();

    maxSignatureDef = FdoSignatureDefinition::Create(FdoDataType_Byte, byteParms);
    maxSignatureDefCol->Add(maxSignatureDef);
    maxSignatureDef = FdoSignatureDefinition::Create(FdoDataType_DateTime, dateTimeParms);
    maxSignatureDefCol->Add(maxSignatureDef);
    maxSignatureDef = FdoSignatureDefinition::Create(FdoDataType_Decimal, decimalParms);
    maxSignatureDefCol->Add(maxSignatureDef);
    maxSignatureDef = FdoSignatureDefinition::Create(FdoDataType_Double, doubleParms);
    maxSignatureDefCol->Add(maxSignatureDef);
    maxSignatureDef = FdoSignatureDefinition::Create(FdoDataType_Int16, int16Parms);
    maxSignatureDefCol->Add(maxSignatureDef);
    maxSignatureDef = FdoSignatureDefinition::Create(FdoDataType_Int32, int32Parms);
    maxSignatureDefCol->Add(maxSignatureDef);
    maxSignatureDef = FdoSignatureDefinition::Create(FdoDataType_Int64, int64Parms);
    maxSignatureDefCol->Add(maxSignatureDef);
    maxSignatureDef = FdoSignatureDefinition::Create(FdoDataType_Single, singleParms);
    maxSignatureDefCol->Add(maxSignatureDef);
    maxSignatureDef = FdoSignatureDefinition::Create(FdoDataType_String, strParms);
    maxSignatureDefCol->Add(maxSignatureDef);

    desc = FdoException::NLSGetMessage(FUNCTION_MAX, "Returns the maximum value of an expression");
    func = FdoFunctionDefinition::Create(FDO_FUNCTION_MAX, desc, true, maxSignatureDefCol);
    functions->Add(func);

    // Add the function MIN. The function accepts a parameter of type byte, date-time, decimal,
    // double, int16, int32, int64, single or string and returns the same data type as the input
    // parameter.
    FdoPtr<FdoSignatureDefinition> minSignatureDef;
    FdoPtr<FdoSignatureDefinitionCollection> minSignatureDefCol = FdoSignatureDefinitionCollection::Create();

    minSignatureDef = FdoSignatureDefinition::Create(FdoDataType_Byte, byteParms);
    minSignatureDefCol->Add(minSignatureDef);
    minSignatureDef = FdoSignatureDefinition::Create(FdoDataType_DateTime, dateTimeParms);
    minSignatureDefCol->Add(minSignatureDef);
    minSignatureDef = FdoSignatureDefinition::Create(FdoDataType_Decimal, decimalParms);
    minSignatureDefCol->Add(minSignatureDef);
    minSignatureDef = FdoSignatureDefinition::Create(FdoDataType_Double, doubleParms);
    minSignatureDefCol->Add(minSignatureDef);
    minSignatureDef = FdoSignatureDefinition::Create(FdoDataType_Int16, int16Parms);
    minSignatureDefCol->Add(minSignatureDef);
    minSignatureDef = FdoSignatureDefinition::Create(FdoDataType_Int32, int32Parms);
    minSignatureDefCol->Add(minSignatureDef);
    minSignatureDef = FdoSignatureDefinition::Create(FdoDataType_Int64, int64Parms);
    minSignatureDefCol->Add(minSignatureDef);
    minSignatureDef = FdoSignatureDefinition::Create(FdoDataType_Single, singleParms);
    minSignatureDefCol->Add(minSignatureDef);
    minSignatureDef = FdoSignatureDefinition::Create(FdoDataType_String, strParms);
    minSignatureDefCol->Add(minSignatureDef);

    desc = FdoException::NLSGetMessage(FUNCTION_MIN, "Returns the minimum value of an expression");
    func = FdoFunctionDefinition::Create(FDO_FUNCTION_MIN, desc, true, minSignatureDefCol);
    functions->Add(func);

    // Add the function SUM. The function accepts a parameter of type decimal, double, int16,
    // int32, int64 or single and returns the same data type as the input parameter.
    FdoPtr<FdoSignatureDefinition> sumSignatureDef;
    FdoPtr<FdoSignatureDefinitionCollection> sumSignatureDefCol = FdoSignatureDefinitionCollection::Create();

    sumSignatureDef = FdoSignatureDefinition::Create(FdoDataType_Decimal, decimalParms);
    sumSignatureDefCol->Add(sumSignatureDef);
    sumSignatureDef = FdoSignatureDefinition::Create(FdoDataType_Double, doubleParms);
    sumSignatureDefCol->Add(sumSignatureDef);
    sumSignatureDef = FdoSignatureDefinition::Create(FdoDataType_Int16, int16Parms);
    sumSignatureDefCol->Add(sumSignatureDef);
    sumSignatureDef = FdoSignatureDefinition::Create(FdoDataType_Int32, int32Parms);
    sumSignatureDefCol->Add(sumSignatureDef);
    sumSignatureDef = FdoSignatureDefinition::Create(FdoDataType_Int64, int64Parms);
    sumSignatureDefCol->Add(sumSignatureDef);
    sumSignatureDef = FdoSignatureDefinition::Create(FdoDataType_Single, singleParms);
    sumSignatureDefCol->Add(sumSignatureDef);

    desc = FdoException::NLSGetMessage(FUNCTION_SUM, "Returns the sum of values of an expression");
    func = FdoFunctionDefinition::Create(FDO_FUNCTION_SUM, desc, true, sumSignatureDefCol);
    functions->Add(func);

    // Add the function UPPER. The function accepts two parameters of type string and always returns
    // a string.
    FdoPtr<FdoSignatureDefinition> upperSignatureDef;
    FdoPtr<FdoSignatureDefinitionCollection> upperSignatureDefCol = FdoSignatureDefinitionCollection::Create();

    upperSignatureDef = FdoSignatureDefinition::Create(FdoDataType_String, strParms);
    upperSignatureDefCol->Add(upperSignatureDef);

    desc = FdoException::NLSGetMessage(FUNCTION_UPPER, "Returns strings with all letters uppercase");
    func = FdoFunctionDefinition::Create(FDO_FUNCTION_UPPER, desc, false, upperSignatureDefCol);
    functions->Add(func);

    return functions;
}

void FdoRdbmsExpressionCapabilities::Dispose()
{
    delete this;
}

