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
#include "FdoRdbmsFunctionIsValid.h"
#include <FdoExpressionEngineFunctionCollection.h>
#include "FdoRdbmsSqlServerConnection.h"

//-------------------------------------------------------
// Constructor / destructor
//-------------------------------------------------------

// default constructor
FdoRdbmsSqlServerExpressionCapabilities::FdoRdbmsSqlServerExpressionCapabilities(FdoRdbmsSqlServerConnection* conn)
{
    m_conn = FDO_SAFE_ADDREF(conn);
}


// default destructor
FdoRdbmsSqlServerExpressionCapabilities::~FdoRdbmsSqlServerExpressionCapabilities()
{
    FDO_SAFE_RELEASE(m_conn);
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

        FdoPtr<FdoExpressionEngineIFunction>  func1 = FdoRdbmsFunctionIsValid::Create();

        m_supportedFunctions->Add(FdoPtr<FdoFunctionDefinition>(func1->GetFunctionDefinition()));

        customFuncs->Add( func1 );
        FdoExpressionEngine::RegisterFunctions( customFuncs );
    }
    if (m_serverFunctions == NULL)
    {
        m_serverFunctions = FdoFunctionDefinitionCollection::Create();
        bool rVal = m_conn->GetServerSideFunctionCollection(m_serverFunctions);
        for(int idx = 0; idx < m_serverFunctions->GetCount(); idx++)
        {
            FdoPtr<FdoFunctionDefinition> fct = m_serverFunctions->GetItem(idx);
            m_supportedFunctions->Add(fct);
        }
        // was server ready to provide them?
        if (!rVal)
            m_serverFunctions = NULL;
    }

    return FDO_SAFE_ADDREF(m_supportedFunctions.p);
}

void FdoRdbmsSqlServerExpressionCapabilities::ForceRemoveServerFunctions()
{
    int cnt = (m_serverFunctions == NULL) ? 0 : m_serverFunctions->GetCount();
    if (cnt == 0 || m_supportedFunctions == NULL)
    {
        m_serverFunctions = NULL;
        return;
    }
    for (int i = 0; i < cnt; i++)
        m_supportedFunctions->RemoveAt(m_supportedFunctions->GetCount()-1);
}
