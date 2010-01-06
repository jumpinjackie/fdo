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
#include "FdoRdbmsSqlServerExpressionCapabilities.h"
#include "FdoRdbmsSqlServerFunctionIsValid.h"
#include <FdoExpressionEngineFunctionCollection.h>

//-------------------------------------------------------
// Constructor / destructor
//-------------------------------------------------------

// default constructor
FdoRdbmsSqlServerExpressionCapabilities::FdoRdbmsSqlServerExpressionCapabilities()
{
}


// default destructor
FdoRdbmsSqlServerExpressionCapabilities::~FdoRdbmsSqlServerExpressionCapabilities()
{
}


//-------------------------------------------------------
// FdoIDisposable implementation
//-------------------------------------------------------


// dispose this object
void FdoRdbmsSqlServerExpressionCapabilities::Dispose()
{
    delete this;
}


//-------------------------------------------------------
// FdoIExpressionCapabilities implementation
//-------------------------------------------------------


// Returns an array of the ExpressionTypes the feature provider supports.
// The length parameter gives the number of expressions types in the array.
FdoExpressionType* FdoRdbmsSqlServerExpressionCapabilities::GetExpressionTypes(FdoInt32& length)
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
FdoFunctionDefinitionCollection* FdoRdbmsSqlServerExpressionCapabilities::GetFunctions()
{
    if (NULL == m_supportedFunctions)
    {
        m_supportedFunctions = FdoExpressionEngine::GetStandardFunctions();
  		FdoPtr<FdoExpressionEngineFunctionCollection>   customFuncs = FdoExpressionEngineFunctionCollection::Create();

        FdoPtr<FdoExpressionEngineIFunction>  func1 = FdoRdbmsSqlServerFunctionIsValid::Create();

        m_supportedFunctions->Add(FdoPtr<FdoFunctionDefinition>(func1->GetFunctionDefinition()));

        customFuncs->Add( func1 );

        // Add more here ...

        FdoExpressionEngine::RegisterFunctions( customFuncs );
    }

    return FDO_SAFE_ADDREF(m_supportedFunctions.p);
}

