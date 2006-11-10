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

ArcSDEExpressionCapabilities::ArcSDEExpressionCapabilities ()
{
}

ArcSDEExpressionCapabilities::~ArcSDEExpressionCapabilities ()
{
}

void ArcSDEExpressionCapabilities::Dispose ()
{
    delete this;
}

/// <summary>Returns an array of FdoExpressionType objects the feature provider supports.</summary>
/// <param name="length">Input the number of expression types</param> 
/// <returns>Returns the list of expression types</returns> 
FdoExpressionType* ArcSDEExpressionCapabilities::GetExpressionTypes (FdoInt32& length)
{
    static FdoExpressionType types[] =
    {
      FdoExpressionType_Basic,
      FdoExpressionType_Function  // Aggregate functions only at this point
// NOT IN R1: FdoExpressionType_Parameter
    };

    length = sizeof (types) / sizeof (FdoExpressionType);

    return (types);
}

/// <summary>Returns a collection of FdoFunctionDefinition objects the feature provider supports within expressions.</summary>
/// <returns>Returns the collection of function definitions</returns> 
FdoFunctionDefinitionCollection* ArcSDEExpressionCapabilities::GetFunctions()
{
    if (NULL == m_supportedFunctions)
    {
        m_supportedFunctions = FdoFunctionDefinitionCollection::Create ();
        FdoPtr<FdoFunctionDefinition> function;

        // Add well-known functions we support to the list:
        // None for now

        // Add NOT-well-known functions we support to the list:

        FdoString* desc = FdoException::NLSGetMessage(FUNCTION_SUM, "Returns the sum of values of an expression");
        function = FdoCommonMiscUtil::CreateFunctionDefinition(FDO_FUNCTION_SUM, desc, true, 4,
            FdoPropertyType_DataProperty, FdoDataType_Double, 1, FdoPropertyType_DataProperty, FdoDataType_Double, 
            FdoPropertyType_DataProperty, FdoDataType_Double, 1, FdoPropertyType_DataProperty, FdoDataType_Single, 
            FdoPropertyType_DataProperty, FdoDataType_Double, 1, FdoPropertyType_DataProperty, FdoDataType_Int16, 
            FdoPropertyType_DataProperty, FdoDataType_Double, 1, FdoPropertyType_DataProperty, FdoDataType_Int32);
        m_supportedFunctions->Add(function);

        desc = FdoException::NLSGetMessage(FUNCTION_COUNT, "Returns the number of objects in the query");
        function = FdoCommonMiscUtil::CreateFunctionDefinition(FDO_FUNCTION_COUNT, desc, true, 7,
            FdoPropertyType_DataProperty, FdoDataType_Int64, 1, FdoPropertyType_DataProperty, FdoDataType_BLOB, 
            FdoPropertyType_DataProperty, FdoDataType_Int64, 1, FdoPropertyType_DataProperty, FdoDataType_DateTime, 
            FdoPropertyType_DataProperty, FdoDataType_Int64, 1, FdoPropertyType_DataProperty, FdoDataType_Double, 
            FdoPropertyType_DataProperty, FdoDataType_Int64, 1, FdoPropertyType_DataProperty, FdoDataType_Int16, 
            FdoPropertyType_DataProperty, FdoDataType_Int64, 1, FdoPropertyType_DataProperty, FdoDataType_Int32, 
            FdoPropertyType_DataProperty, FdoDataType_Int64, 1, FdoPropertyType_DataProperty, FdoDataType_Single, 
            FdoPropertyType_DataProperty, FdoDataType_Int64, 1, FdoPropertyType_DataProperty, FdoDataType_String);
        m_supportedFunctions->Add(function);

        desc = FdoException::NLSGetMessage(FUNCTION_MIN, "Returns the minimum value of an expression");
        function = FdoCommonMiscUtil::CreateFunctionDefinition(FDO_FUNCTION_MIN, desc, true, 4,
            FdoPropertyType_DataProperty, FdoDataType_Double, 1, FdoPropertyType_DataProperty, FdoDataType_Double, 
            FdoPropertyType_DataProperty, FdoDataType_Double, 1, FdoPropertyType_DataProperty, FdoDataType_Single, 
            FdoPropertyType_DataProperty, FdoDataType_Double, 1, FdoPropertyType_DataProperty, FdoDataType_Int16, 
            FdoPropertyType_DataProperty, FdoDataType_Double, 1, FdoPropertyType_DataProperty, FdoDataType_Int32);
        m_supportedFunctions->Add(function);

        desc = FdoException::NLSGetMessage(FUNCTION_AVG, "Returns the average value of an expression");
        function = FdoCommonMiscUtil::CreateFunctionDefinition(FDO_FUNCTION_AVG, desc, true, 4,
            FdoPropertyType_DataProperty, FdoDataType_Double, 1, FdoPropertyType_DataProperty, FdoDataType_Double, 
            FdoPropertyType_DataProperty, FdoDataType_Double, 1, FdoPropertyType_DataProperty, FdoDataType_Single, 
            FdoPropertyType_DataProperty, FdoDataType_Double, 1, FdoPropertyType_DataProperty, FdoDataType_Int16, 
            FdoPropertyType_DataProperty, FdoDataType_Double, 1, FdoPropertyType_DataProperty, FdoDataType_Int32);
        m_supportedFunctions->Add(function);

        desc = FdoException::NLSGetMessage(FUNCTION_MAX, "Returns the maximum value of an expression");
        function = FdoCommonMiscUtil::CreateFunctionDefinition(FDO_FUNCTION_MAX, desc, true, 4,
            FdoPropertyType_DataProperty, FdoDataType_Double, 1, FdoPropertyType_DataProperty, FdoDataType_Double, 
            FdoPropertyType_DataProperty, FdoDataType_Double, 1, FdoPropertyType_DataProperty, FdoDataType_Single, 
            FdoPropertyType_DataProperty, FdoDataType_Double, 1, FdoPropertyType_DataProperty, FdoDataType_Int16, 
            FdoPropertyType_DataProperty, FdoDataType_Double, 1, FdoPropertyType_DataProperty, FdoDataType_Int32);
        m_supportedFunctions->Add(function);

        desc = NlsMsgGet(ARCSDE_FUNCTION_STDDEV_DESC, "Returns the standard deviation of an expression.");
        function = FdoCommonMiscUtil::CreateFunctionDefinition(ARCSDE_FUNCTION_STDDEV, desc, true, 4,
            FdoPropertyType_DataProperty, FdoDataType_Double, 1, FdoPropertyType_DataProperty, FdoDataType_Double, 
            FdoPropertyType_DataProperty, FdoDataType_Double, 1, FdoPropertyType_DataProperty, FdoDataType_Single, 
            FdoPropertyType_DataProperty, FdoDataType_Double, 1, FdoPropertyType_DataProperty, FdoDataType_Int16, 
            FdoPropertyType_DataProperty, FdoDataType_Double, 1, FdoPropertyType_DataProperty, FdoDataType_Int32);
        m_supportedFunctions->Add(function);
    }

    return (FDO_SAFE_ADDREF (m_supportedFunctions.p));
}

