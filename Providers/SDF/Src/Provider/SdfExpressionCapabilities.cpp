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
#include "stdafx.h"
#include "SdfExpressionCapabilities.h"

//-------------------------------------------------------
// Constructor / destructor
//-------------------------------------------------------

// default constructor
SdfExpressionCapabilities::SdfExpressionCapabilities()
{
}


// default destructor
SdfExpressionCapabilities::~SdfExpressionCapabilities()
{
}


//-------------------------------------------------------
// FdoIDisposable implementation
//-------------------------------------------------------


// dispose this object
void SdfExpressionCapabilities::Dispose()
{
    delete this;
}


//-------------------------------------------------------
// FdoIExpressionCapabilities implementation
//-------------------------------------------------------


// Returns an array of the ExpressionTypes the feature provider supports.
// The length parameter gives the number of expressions types in the array.
FdoExpressionType* SdfExpressionCapabilities::GetExpressionTypes(FdoInt32& length)
{
    static FdoExpressionType expressionTypes[] =
    {
        FdoExpressionType_Basic,
        FdoExpressionType_Function,
//      FdoExpressionType_Parameter
    };

    length = sizeof(expressionTypes) / sizeof(FdoExpressionType);
    return expressionTypes;
}


// Returns an array of FunctionDefinitions the feature provider supports
// within expressions.  The length parameter gives the number of function
// definitions in the array.
FdoFunctionDefinitionCollection* SdfExpressionCapabilities::GetFunctions()
{
    if (NULL == m_supportedFunctions)
    {
        m_supportedFunctions = FdoFunctionDefinitionCollection::Create ();

        // Add well-known functions we support to the list:

        FdoPtr<FdoFunctionDefinitionCollection> wellKnownFunctions = GetWellKnownFunctions();
        FdoPtr<FdoFunctionDefinition> wellKnownFunction;
        wellKnownFunction = wellKnownFunctions->GetItem(FDO_FUNCTION_CONCAT);
        m_supportedFunctions->Add(wellKnownFunction);
        wellKnownFunction = wellKnownFunctions->GetItem(FDO_FUNCTION_SPATIALEXTENTS);
        m_supportedFunctions->Add(wellKnownFunction);


        // Add NOT-well-known functions we support to the list:

        FdoString* desc = FdoException::NLSGetMessage(FUNCTION_CEIL, "Returns the smallest integer greater than or equal to the expression value");
        FdoPtr<FdoFunctionDefinition> function = FdoCommonMiscUtil::CreateFunctionDefinition(FDO_FUNCTION_CEIL, desc, false, 3,
            FdoPropertyType_DataProperty, FdoDataType_Double, 1, FdoPropertyType_DataProperty, FdoDataType_Double,
            FdoPropertyType_DataProperty, FdoDataType_Double, 1, FdoPropertyType_DataProperty, FdoDataType_Decimal,
            FdoPropertyType_DataProperty, FdoDataType_Single, 1, FdoPropertyType_DataProperty, FdoDataType_Single);
        m_supportedFunctions->Add(function);

        desc = FdoException::NLSGetMessage(FUNCTION_FLOOR, "Returns the largest integer equal to or less than the expression value");
        function = FdoCommonMiscUtil::CreateFunctionDefinition(FDO_FUNCTION_FLOOR, desc, false, 3,
            FdoPropertyType_DataProperty, FdoDataType_Double, 1, FdoPropertyType_DataProperty, FdoDataType_Double,
            FdoPropertyType_DataProperty, FdoDataType_Double, 1, FdoPropertyType_DataProperty, FdoDataType_Decimal,
            FdoPropertyType_DataProperty, FdoDataType_Single, 1, FdoPropertyType_DataProperty, FdoDataType_Single);
        m_supportedFunctions->Add(function);

        desc = FdoException::NLSGetMessage(FUNCTION_LOWER, "Returns strings with all lowercase letters");
        function = FdoCommonMiscUtil::CreateFunctionDefinition(FDO_FUNCTION_LOWER, desc, false, 1,
            FdoPropertyType_DataProperty, FdoDataType_String, 1, FdoPropertyType_DataProperty, FdoDataType_String);
        m_supportedFunctions->Add(function);

        desc = FdoException::NLSGetMessage(FUNCTION_UPPER, "Returns strings with all letters uppercase");
        function = FdoCommonMiscUtil::CreateFunctionDefinition(FDO_FUNCTION_UPPER, desc, false, 1,
            FdoPropertyType_DataProperty, FdoDataType_String, 1, FdoPropertyType_DataProperty, FdoDataType_String);
        m_supportedFunctions->Add(function);

        desc = FdoException::NLSGetMessage(FUNCTION_SUM, "Returns the sum of values of an expression");
        function = FdoCommonMiscUtil::CreateFunctionDefinition(FDO_FUNCTION_SUM, desc, true, 7,
            FdoPropertyType_DataProperty, FdoDataType_Double, 1, FdoPropertyType_DataProperty, FdoDataType_Double, 
            FdoPropertyType_DataProperty, FdoDataType_Double, 1, FdoPropertyType_DataProperty, FdoDataType_Single, 
            FdoPropertyType_DataProperty, FdoDataType_Double, 1, FdoPropertyType_DataProperty, FdoDataType_Decimal, 
            FdoPropertyType_DataProperty, FdoDataType_Double, 1, FdoPropertyType_DataProperty, FdoDataType_Byte, 
            FdoPropertyType_DataProperty, FdoDataType_Double, 1, FdoPropertyType_DataProperty, FdoDataType_Int16, 
            FdoPropertyType_DataProperty, FdoDataType_Double, 1, FdoPropertyType_DataProperty, FdoDataType_Int32, 
            FdoPropertyType_DataProperty, FdoDataType_Double, 1, FdoPropertyType_DataProperty, FdoDataType_Int64);
        m_supportedFunctions->Add(function);

        desc = FdoException::NLSGetMessage(FUNCTION_COUNT, "Returns the number of objects in the query");
        function = FdoCommonMiscUtil::CreateFunctionDefinition(FDO_FUNCTION_COUNT, desc, true, 12,
            FdoPropertyType_DataProperty, FdoDataType_Int64, 1, FdoPropertyType_DataProperty, FdoDataType_Boolean, 
            FdoPropertyType_DataProperty, FdoDataType_Int64, 1, FdoPropertyType_DataProperty, FdoDataType_Double, 
            FdoPropertyType_DataProperty, FdoDataType_Int64, 1, FdoPropertyType_DataProperty, FdoDataType_Single, 
            FdoPropertyType_DataProperty, FdoDataType_Int64, 1, FdoPropertyType_DataProperty, FdoDataType_Decimal, 
            FdoPropertyType_DataProperty, FdoDataType_Int64, 1, FdoPropertyType_DataProperty, FdoDataType_Byte, 
            FdoPropertyType_DataProperty, FdoDataType_Int64, 1, FdoPropertyType_DataProperty, FdoDataType_DateTime, 
            FdoPropertyType_DataProperty, FdoDataType_Int64, 1, FdoPropertyType_DataProperty, FdoDataType_Int16, 
            FdoPropertyType_DataProperty, FdoDataType_Int64, 1, FdoPropertyType_DataProperty, FdoDataType_Int32, 
            FdoPropertyType_DataProperty, FdoDataType_Int64, 1, FdoPropertyType_DataProperty, FdoDataType_Int64,
            FdoPropertyType_DataProperty, FdoDataType_Int64, 1, FdoPropertyType_DataProperty, FdoDataType_String, 
            FdoPropertyType_DataProperty, FdoDataType_Int64, 1, FdoPropertyType_GeometricProperty, FdoDataType_Int32,
            FdoPropertyType_DataProperty, FdoDataType_Int64, 1, FdoPropertyType_AssociationProperty, FdoDataType_Int32);
        m_supportedFunctions->Add(function);

        desc = FdoException::NLSGetMessage(FUNCTION_MIN, "Returns the minimum value of an expression");
        function = FdoCommonMiscUtil::CreateFunctionDefinition(FDO_FUNCTION_MIN, desc, true, 7,
            FdoPropertyType_DataProperty, FdoDataType_Double, 1, FdoPropertyType_DataProperty, FdoDataType_Double, 
            FdoPropertyType_DataProperty, FdoDataType_Double, 1, FdoPropertyType_DataProperty, FdoDataType_Single, 
            FdoPropertyType_DataProperty, FdoDataType_Double, 1, FdoPropertyType_DataProperty, FdoDataType_Decimal, 
            FdoPropertyType_DataProperty, FdoDataType_Double, 1, FdoPropertyType_DataProperty, FdoDataType_Byte, 
            FdoPropertyType_DataProperty, FdoDataType_Double, 1, FdoPropertyType_DataProperty, FdoDataType_Int16, 
            FdoPropertyType_DataProperty, FdoDataType_Double, 1, FdoPropertyType_DataProperty, FdoDataType_Int32, 
            FdoPropertyType_DataProperty, FdoDataType_Double, 1, FdoPropertyType_DataProperty, FdoDataType_Int64);
        m_supportedFunctions->Add(function);

        desc = FdoException::NLSGetMessage(FUNCTION_AVG, "Returns the average value of an expression");
        function = FdoCommonMiscUtil::CreateFunctionDefinition(FDO_FUNCTION_AVG, desc, true, 7,
            FdoPropertyType_DataProperty, FdoDataType_Double, 1, FdoPropertyType_DataProperty, FdoDataType_Double, 
            FdoPropertyType_DataProperty, FdoDataType_Double, 1, FdoPropertyType_DataProperty, FdoDataType_Single, 
            FdoPropertyType_DataProperty, FdoDataType_Double, 1, FdoPropertyType_DataProperty, FdoDataType_Decimal, 
            FdoPropertyType_DataProperty, FdoDataType_Double, 1, FdoPropertyType_DataProperty, FdoDataType_Byte, 
            FdoPropertyType_DataProperty, FdoDataType_Double, 1, FdoPropertyType_DataProperty, FdoDataType_Int16, 
            FdoPropertyType_DataProperty, FdoDataType_Double, 1, FdoPropertyType_DataProperty, FdoDataType_Int32, 
            FdoPropertyType_DataProperty, FdoDataType_Double, 1, FdoPropertyType_DataProperty, FdoDataType_Int64);
        m_supportedFunctions->Add(function);

        desc = FdoException::NLSGetMessage(FUNCTION_MAX, "Returns the maximum value of an expression");
        function = FdoCommonMiscUtil::CreateFunctionDefinition(FDO_FUNCTION_MAX, desc, true, 7,
            FdoPropertyType_DataProperty, FdoDataType_Double, 1, FdoPropertyType_DataProperty, FdoDataType_Double, 
            FdoPropertyType_DataProperty, FdoDataType_Double, 1, FdoPropertyType_DataProperty, FdoDataType_Single, 
            FdoPropertyType_DataProperty, FdoDataType_Double, 1, FdoPropertyType_DataProperty, FdoDataType_Decimal, 
            FdoPropertyType_DataProperty, FdoDataType_Double, 1, FdoPropertyType_DataProperty, FdoDataType_Byte, 
            FdoPropertyType_DataProperty, FdoDataType_Double, 1, FdoPropertyType_DataProperty, FdoDataType_Int16, 
            FdoPropertyType_DataProperty, FdoDataType_Double, 1, FdoPropertyType_DataProperty, FdoDataType_Int32, 
            FdoPropertyType_DataProperty, FdoDataType_Double, 1, FdoPropertyType_DataProperty, FdoDataType_Int64);
        m_supportedFunctions->Add(function);
    }

    return FDO_SAFE_ADDREF(m_supportedFunctions.p);
}

