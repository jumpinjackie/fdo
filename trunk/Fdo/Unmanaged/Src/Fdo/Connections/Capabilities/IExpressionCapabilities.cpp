// 
//  
//  Copyright (C) 2004-2006  Autodesk, Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//  
#include <Fdo/Connections/Capabilities/IExpressionCapabilities.h>


FdoFunctionDefinitionCollection *FdoIExpressionCapabilities::GetWellKnownFunctions ()
{
    // Create the basic argument collections being referenced in different places.
    FdoString *desc = FdoException::NLSGetMessage(FUNCTION_DBL_ARG,"Argument that represents a double");
    FdoPtr<FdoArgumentDefinition>doubleParm = FdoArgumentDefinition::Create(L"dblValue", desc, FdoDataType_Double);
    FdoPtr<FdoArgumentDefinitionCollection>doubleParms = FdoArgumentDefinitionCollection::Create();
    doubleParms->Add(doubleParm);

    desc = FdoException::NLSGetMessage(FUNCTION_INT_ARG, "Argument that represents an integer");
    FdoPtr<FdoArgumentDefinition>int64Parm = FdoArgumentDefinition::Create(L"intValue", desc, FdoDataType_Int64);
    FdoPtr<FdoArgumentDefinitionCollection> int64Parms = FdoArgumentDefinitionCollection::Create();
    int64Parms->Add(int64Parm);

    desc = FdoException::NLSGetMessage(FUNCTION_STR_ARG, "Argument that represents a string");
    FdoPtr<FdoArgumentDefinition>strParm = FdoArgumentDefinition::Create(L"strValue", desc, FdoDataType_String);
    FdoPtr<FdoArgumentDefinitionCollection>strParms = FdoArgumentDefinitionCollection::Create();
    strParms->Add(strParm);

    desc = FdoException::NLSGetMessage(FUNCTION_GEOM_ARG, "Argument that represents a geometry");
    FdoPtr<FdoArgumentDefinition>geomParm = FdoArgumentDefinition::Create(L"geomValue", desc, FdoPropertyType_GeometricProperty, (FdoDataType)-1);
    FdoPtr<FdoArgumentDefinitionCollection>geomParms = FdoArgumentDefinitionCollection::Create();
    geomParms->Add(geomParm);

    // Create the required function collection.
    FdoFunctionDefinitionCollection* functions = FdoFunctionDefinitionCollection::Create();

    // Add the functions CEIL, and FLOOR.
    //  ---- CEIL ----
    desc = FdoException::NLSGetMessage(FUNCTION_CEIL, "Returns the smallest integer greater than or equal to the expression value");
    FdoPtr<FdoSignatureDefinition> ceilSignatureDef = FdoSignatureDefinition::Create(FdoDataType_Int64, int64Parms);
    FdoPtr<FdoSignatureDefinitionCollection> ceilSignatureDefCol = FdoSignatureDefinitionCollection::Create();
    ceilSignatureDefCol->Add(ceilSignatureDef);
	FdoPtr<FdoFunctionDefinition> func =
        FdoFunctionDefinition::Create(FDO_FUNCTION_CEIL, desc, false, ceilSignatureDefCol, FdoFunctionCategoryType_Numeric);
    functions->Add(func);

    //  ---- FLOOR ----
    desc = FdoException::NLSGetMessage(FUNCTION_FLOOR, "Returns the largest integer equal to or less than the expression value");
    FdoPtr<FdoSignatureDefinition> floorSignatureDef = FdoSignatureDefinition::Create(FdoDataType_Int64, int64Parms);
    FdoPtr<FdoSignatureDefinitionCollection> floorSignatureDefCol = FdoSignatureDefinitionCollection::Create();
    floorSignatureDefCol->Add(floorSignatureDef);
    func = FdoFunctionDefinition::Create(FDO_FUNCTION_FLOOR, desc, false, floorSignatureDefCol, FdoFunctionCategoryType_Numeric);
    functions->Add(func);

    // Add the aggregate functions AVG, COUNT, MAX, MIN and SUM.
    //  ---- AVG ----
    desc = FdoException::NLSGetMessage(FUNCTION_AVG, "Returns the average value of an expression");
    FdoPtr<FdoSignatureDefinition> avgSignatureDef = FdoSignatureDefinition::Create(FdoDataType_Double, doubleParms);
    FdoPtr<FdoSignatureDefinitionCollection> avgSignatureDefCol = FdoSignatureDefinitionCollection::Create();
    avgSignatureDefCol->Add(avgSignatureDef);
    func = FdoFunctionDefinition::Create(FDO_FUNCTION_AVG, desc, true, avgSignatureDefCol, FdoFunctionCategoryType_Aggregate);
    functions->Add(func);

    //  ---- COUNT ----
    desc = FdoException::NLSGetMessage(FUNCTION_COUNT, "Returns the number of objects in the query");
    FdoPtr<FdoSignatureDefinition> countSignatureDef = FdoSignatureDefinition::Create(FdoDataType_Int64, int64Parms);
    FdoPtr<FdoSignatureDefinitionCollection> countSignatureDefCol = FdoSignatureDefinitionCollection::Create();
    countSignatureDefCol->Add(countSignatureDef);
    func = FdoFunctionDefinition::Create(FDO_FUNCTION_COUNT, desc, true, countSignatureDefCol, FdoFunctionCategoryType_Aggregate);
    functions->Add(func);

    //  ---- MAX ----
    desc = FdoException::NLSGetMessage(FUNCTION_MAX, "Returns the maximum value of an expression");
    FdoPtr<FdoSignatureDefinition> maxSignatureDef = FdoSignatureDefinition::Create(FdoDataType_Double, doubleParms);
    FdoPtr<FdoSignatureDefinitionCollection> maxSignatureDefCol = FdoSignatureDefinitionCollection::Create();
    maxSignatureDefCol->Add(maxSignatureDef);
    func = FdoFunctionDefinition::Create(FDO_FUNCTION_MAX, desc, true, maxSignatureDefCol, FdoFunctionCategoryType_Aggregate);
    functions->Add(func);

    //  ---- MIN ----
    desc = FdoException::NLSGetMessage(FUNCTION_MIN, "Returns the minimum value of an expression");
    FdoPtr<FdoSignatureDefinition> minSignatureDef = FdoSignatureDefinition::Create(FdoDataType_Double, doubleParms);
    FdoPtr<FdoSignatureDefinitionCollection> minSignatureDefCol = FdoSignatureDefinitionCollection::Create();
    minSignatureDefCol->Add(minSignatureDef);
    func = FdoFunctionDefinition::Create(FDO_FUNCTION_MIN, desc, true, minSignatureDefCol, FdoFunctionCategoryType_Aggregate);
    functions->Add(func);

    //  ---- SUM ----
    desc = FdoException::NLSGetMessage(FUNCTION_SUM, "Returns the sum of values of an expression");
    FdoPtr<FdoSignatureDefinition> sumSignatureDef = FdoSignatureDefinition::Create(FdoDataType_Double, doubleParms);
    FdoPtr<FdoSignatureDefinitionCollection> sumSignatureDefCol = FdoSignatureDefinitionCollection::Create();
    sumSignatureDefCol->Add(sumSignatureDef);
    func = FdoFunctionDefinition::Create(FDO_FUNCTION_SUM, desc, true, sumSignatureDefCol, FdoFunctionCategoryType_Aggregate);
    functions->Add(func);

    // Add the string functions LOWER and UPPER.
    desc = FdoException::NLSGetMessage(FUNCTION_LOWER, "Returns strings with all letters lowercase");
    FdoPtr<FdoSignatureDefinition> lowerSignatureDef = FdoSignatureDefinition::Create(FdoDataType_String, strParms);
    FdoPtr<FdoSignatureDefinitionCollection> lowerSignatureDefCol = FdoSignatureDefinitionCollection::Create();
    lowerSignatureDefCol->Add(lowerSignatureDef);
    func = FdoFunctionDefinition::Create(FDO_FUNCTION_LOWER, desc, false, lowerSignatureDefCol, FdoFunctionCategoryType_String);
    functions->Add(func);

    desc = FdoException::NLSGetMessage(FUNCTION_UPPER, "Returns strings with all letters uppercase");
    FdoPtr<FdoSignatureDefinition> upperSignatureDef = FdoSignatureDefinition::Create(FdoDataType_String, strParms);
    FdoPtr<FdoSignatureDefinitionCollection> upperSignatureDefCol = FdoSignatureDefinitionCollection::Create();
    upperSignatureDefCol->Add(upperSignatureDef);
    func = FdoFunctionDefinition::Create(FDO_FUNCTION_UPPER, desc, false, upperSignatureDefCol, FdoFunctionCategoryType_String);
    functions->Add(func);

    // Add the geometry function SPATIALEXTENTS.
    desc = FdoException::NLSGetMessage(FUNCTION_SPATIALEXTENTS, "Returns the spatial extents of values of a geometric expression");
    FdoPtr<FdoSignatureDefinition> spatextSignatureDef = FdoSignatureDefinition::Create(FdoPropertyType_GeometricProperty, (FdoDataType)-1, geomParms);
    FdoPtr<FdoSignatureDefinitionCollection> spatextSignatureDefCol = FdoSignatureDefinitionCollection::Create();
    spatextSignatureDefCol->Add(spatextSignatureDef);
    func = FdoFunctionDefinition::Create(FDO_FUNCTION_SPATIALEXTENTS, desc, true, spatextSignatureDefCol, FdoFunctionCategoryType_Aggregate);
    functions->Add(func);

    // Add the function CONCAT.
    strParms->Clear();
    desc = FdoException::NLSGetMessage(FUNCTION_STR_ARG, "Argument that represents a string", FdoDataType_String);
    strParm = FdoArgumentDefinition::Create(L"strValue1", desc, FdoDataType_String);
    strParms->Add(strParm);
    strParm = FdoArgumentDefinition::Create(L"strValue2", desc, FdoDataType_String);
    strParms->Add(strParm);
    desc = FdoException::NLSGetMessage(FUNCTION_CONCAT, "Returns strings with all letters lowercase");
    FdoPtr<FdoSignatureDefinition> concatSignatureDef = FdoSignatureDefinition::Create(FdoDataType_String, strParms);
    FdoPtr<FdoSignatureDefinitionCollection> concatSignatureDefCol = FdoSignatureDefinitionCollection::Create();
    concatSignatureDefCol->Add(concatSignatureDef);
    func = FdoFunctionDefinition::Create(FDO_FUNCTION_CONCAT, desc, false, concatSignatureDefCol, FdoFunctionCategoryType_String);
    functions->Add(func);

    // Return the function collection.
    return functions;

}  //  GetWellKnownFunctions ()

