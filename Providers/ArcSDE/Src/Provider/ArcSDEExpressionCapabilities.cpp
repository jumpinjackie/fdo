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
    FdoPtr<FdoFunctionDefinitionCollection> ret = FdoFunctionDefinitionCollection::Create ();

    // Add well-known functions we support to the list:
    FdoPtr<FdoFunctionDefinitionCollection> wellKnownFunctions = GetWellKnownFunctions();
    FdoPtr<FdoFunctionDefinition> wellKnownFunction;
    wellKnownFunction = wellKnownFunctions->GetItem(FDO_FUNCTION_COUNT);
    ret->Add(wellKnownFunction);
    wellKnownFunction = wellKnownFunctions->GetItem(FDO_FUNCTION_MIN);
    ret->Add(wellKnownFunction);
    wellKnownFunction = wellKnownFunctions->GetItem(FDO_FUNCTION_AVG);
    ret->Add(wellKnownFunction);
    wellKnownFunction = wellKnownFunctions->GetItem(FDO_FUNCTION_MAX);
    ret->Add(wellKnownFunction);
    wellKnownFunction = wellKnownFunctions->GetItem(FDO_FUNCTION_SUM);
    ret->Add(wellKnownFunction);

    // Add NOT-well-known functions we support to the list:
    FdoPtr<FdoArgumentDefinitionCollection> args = FdoArgumentDefinitionCollection::Create();
    FdoPtr<FdoArgumentDefinition> dblArg = FdoArgumentDefinition::Create(ARCSDE_FUNCTION_DOUBLE_ARGUMENT_NAME, NlsMsgGet(ARCSDE_FUNCTION_DOUBLE_ARGUMENT_DESC, "An argument that accepts any expression that evaluates to a double value."), FdoDataType_Double);
    args->Add(dblArg);
    wellKnownFunction = FdoFunctionDefinition::Create(ARCSDE_FUNCTION_STDDEV, NlsMsgGet(ARCSDE_FUNCTION_STDDEV_DESC, "Returns the standard deviation of an expression."), FdoDataType_Double, args);
    ret->Add(wellKnownFunction);

    return (FDO_SAFE_ADDREF (ret.p));
}

