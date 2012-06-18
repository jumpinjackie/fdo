// 
//  
//  Copyright (C) 2004-2011  Autodesk, Inc.
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
#include "FdoRdbmsPostGisExpressionCapabilities.h"
#include "FdoRdbmsFunctionIsValid.h"
#include <FdoExpressionEngineFunctionCollection.h>

//-------------------------------------------------------
// Constructor / destructor
//-------------------------------------------------------

// default constructor
FdoRdbmsPostGisExpressionCapabilities::FdoRdbmsPostGisExpressionCapabilities()
{
}


// default destructor
FdoRdbmsPostGisExpressionCapabilities::~FdoRdbmsPostGisExpressionCapabilities()
{
}


//-------------------------------------------------------
// FdoIDisposable implementation
//-------------------------------------------------------


// dispose this object
void FdoRdbmsPostGisExpressionCapabilities::Dispose()
{
    delete this;
}


//-------------------------------------------------------
// FdoIExpressionCapabilities implementation
//-------------------------------------------------------


// Returns an array of FunctionDefinitions the feature provider supports
// within expressions.  The length parameter gives the number of function
// definitions in the array.
FdoFunctionDefinitionCollection* FdoRdbmsPostGisExpressionCapabilities::GetFunctions()
{
    if (NULL == m_supportedFunctions)
    {
        m_supportedFunctions = FdoExpressionEngine::GetStandardFunctions();
  		FdoPtr<FdoExpressionEngineFunctionCollection>   customFuncs = FdoExpressionEngineFunctionCollection::Create();

        FdoPtr<FdoExpressionEngineIFunction>  func1 = FdoRdbmsFunctionIsValid::Create();

        m_supportedFunctions->Add(FdoPtr<FdoFunctionDefinition>(func1->GetFunctionDefinition()));

        customFuncs->Add( func1 );

        // Add more here ...

        FdoExpressionEngine::RegisterFunctions( customFuncs );
    }

    return FDO_SAFE_ADDREF(m_supportedFunctions.p);
}

