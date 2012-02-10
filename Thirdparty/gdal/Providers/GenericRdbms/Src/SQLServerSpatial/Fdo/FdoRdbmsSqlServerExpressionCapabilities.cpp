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
    m_conn = conn; // no add ref since we create circular reference
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

class FdoRdbmsCustomFunction : public FdoExpressionEngineINonAggregateFunction
{
    public:
        static FdoRdbmsCustomFunction *Create (FdoFunctionDefinition* fctDef) { return new FdoRdbmsCustomFunction(fctDef); }
        virtual FdoRdbmsFunctionIsValid *CreateObject () { throw FdoException::Create(L"CreateObject() is not supported!"); }
        virtual FdoFunctionDefinition *GetFunctionDefinition () { return FDO_SAFE_ADDREF(m_fctDef.p); }
        virtual FdoLiteralValue* Evaluate(FdoLiteralValueCollection *literal_values)
        {
            std::wstring err (L"Function '");
            err.append(m_fctDef->GetName());
            err.append(L"' is executed only on the server side");
            throw FdoException::Create(err.c_str());
        }

	protected:
        FdoRdbmsCustomFunction (FdoFunctionDefinition* fctDef)
        {
            m_fctDef = FDO_SAFE_ADDREF(fctDef);
        }

        ~FdoRdbmsCustomFunction () {}

    private:
        virtual void Dispose () { delete this; };

        FdoPtr<FdoFunctionDefinition> m_fctDef;
};


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
        if (rVal && m_serverFunctions->GetCount())
        {
            FdoPtr<FdoExpressionEngineFunctionCollection> customFuncs = FdoExpressionEngineFunctionCollection::Create();
            for(int idx = 0; idx < m_serverFunctions->GetCount(); idx++)
            {
                FdoPtr<FdoFunctionDefinition> fctDef = m_serverFunctions->GetItem(idx);
                FdoPtr<FdoExpressionEngineIFunction> fct = FdoRdbmsCustomFunction::Create(fctDef);
                customFuncs->Add (fct);
                m_supportedFunctions->Add(fctDef);
            }
            FdoExpressionEngine::RegisterFunctions (customFuncs);
        }
        else // was server ready to provide them?
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
    {
        FdoPtr<FdoFunctionDefinition> fctDef = m_serverFunctions->GetItem(i);
        m_supportedFunctions->Remove(fctDef);
    }
    m_serverFunctions = NULL;
}
