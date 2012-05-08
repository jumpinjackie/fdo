/*
* Copyright (C) 2006  SL-King d.o.o
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
*/

#include "stdafx.h"

c_KgOraExpressionCapabilities::c_KgOraExpressionCapabilities ()
{
}

c_KgOraExpressionCapabilities::~c_KgOraExpressionCapabilities ()
{
}

void c_KgOraExpressionCapabilities::Dispose ()
{
    delete this;
}

/// <summary>Returns an array of FdoExpressionType objects the feature provider supports.</summary>
/// <param name="length">Input the number of expression types</param> 
/// <returns>Returns the list of expression types</returns> 
FdoExpressionType* c_KgOraExpressionCapabilities::GetExpressionTypes (FdoInt32& Length)
{
    static FdoExpressionType types[] =
    {
        FdoExpressionType_Basic,
        FdoExpressionType_Function,
    };

    Length = sizeof (types) / sizeof (FdoExpressionType);

    return (types);
}//end of c_KgOraExpressionCapabilities::GetExpressionTypes 

/// <summary>Returns a collection of FdoFunctionDefinition objects the feature provider supports within expressions.</summary>
/// <returns>Returns the collection of function definitions</returns> 
FdoFunctionDefinitionCollection* c_KgOraExpressionCapabilities::GetFunctions()
{
    FdoPtr<FdoFunctionDefinitionCollection> wellKnownFunctions;
    FdoPtr<FdoFunctionDefinition> wellKnownFunction;
    FdoPtr<FdoFunctionDefinitionCollection> ret;
    
    ret = FdoFunctionDefinitionCollection::Create ();

    // add the well-known functions we support to the list:
    wellKnownFunctions = GetWellKnownFunctions ();

    // Number functions
    wellKnownFunction = wellKnownFunctions->GetItem(FDO_FUNCTION_CEIL);
    ret->Add(wellKnownFunction);
    wellKnownFunction = wellKnownFunctions->GetItem(FDO_FUNCTION_FLOOR);
    ret->Add(wellKnownFunction);

    // Aggregates functions
    wellKnownFunction = wellKnownFunctions->GetItem(FDO_FUNCTION_SUM);
    ret->Add(wellKnownFunction);
    wellKnownFunction = wellKnownFunctions->GetItem(FDO_FUNCTION_COUNT);
    ret->Add(wellKnownFunction);
    wellKnownFunction = wellKnownFunctions->GetItem(FDO_FUNCTION_MIN);
    ret->Add(wellKnownFunction);
    wellKnownFunction = wellKnownFunctions->GetItem(FDO_FUNCTION_AVG);
    ret->Add(wellKnownFunction);
    wellKnownFunction = wellKnownFunctions->GetItem(FDO_FUNCTION_MAX);
    ret->Add(wellKnownFunction);
    
    wellKnownFunction = wellKnownFunctions->GetItem(FDO_FUNCTION_SPATIALEXTENTS);
    ret->Add(wellKnownFunction);

    // Strings functions
    wellKnownFunction = wellKnownFunctions->GetItem(FDO_FUNCTION_LOWER);
    ret->Add(wellKnownFunction);
    wellKnownFunction = wellKnownFunctions->GetItem(FDO_FUNCTION_UPPER);
    ret->Add(wellKnownFunction);
    wellKnownFunction = wellKnownFunctions->GetItem(FDO_FUNCTION_CONCAT);
    ret->Add(wellKnownFunction);

    return (FDO_SAFE_ADDREF (ret.p));
}//end of c_KgOraExpressionCapabilities::GetFunctions

