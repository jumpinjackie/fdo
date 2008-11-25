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
#include <FdoExpressionEngine.h>
#include <Functions/Geometry/FdoFunctionX.h>
#include <Functions/Geometry/FdoFunctionY.h>
#include <Functions/Geometry/FdoFunctionZ.h>
#include <Functions/Geometry/FdoFunctionM.h>


ShpExpressionCapabilities::ShpExpressionCapabilities ()
{
}

ShpExpressionCapabilities::~ShpExpressionCapabilities ()
{
}

void ShpExpressionCapabilities::Dispose ()
{
    delete this;
}

/// <summary>Returns an array of FdoExpressionType objects the feature provider supports.</summary>
/// <param name="length">Input the number of expression types</param> 
/// <returns>Returns the list of expression types</returns> 
FdoExpressionType* ShpExpressionCapabilities::GetExpressionTypes (FdoInt32& length)
{
    static FdoExpressionType types[] =
    {
        FdoExpressionType_Basic,
        FdoExpressionType_Function,
    };

    length = sizeof (types) / sizeof (FdoExpressionType);

    return (types);
}

/// <summary>Returns a collection of FdoFunctionDefinition objects the feature provider supports within expressions.</summary>
/// <returns>Returns the collection of function definitions</returns> 
FdoFunctionDefinitionCollection* ShpExpressionCapabilities::GetFunctions()
{
    if (m_supportedFunctions == NULL)
    {
        m_supportedFunctions = FdoExpressionEngine::GetStandardFunctions();
		
		// Add function X to the list of supported function
		FdoPtr<FdoExpressionEngineIFunction> funcX = FdoFunctionX::Create();
		m_supportedFunctions->Add(FdoPtr<FdoFunctionDefinition>(funcX->GetFunctionDefinition()));
		
		// Add function Y to the list of supported function
		FdoPtr<FdoExpressionEngineIFunction> funcY = FdoFunctionY::Create();
		m_supportedFunctions->Add(FdoPtr<FdoFunctionDefinition>(funcY->GetFunctionDefinition()));
		
		// Add function Z to the list of supported function
		FdoPtr<FdoExpressionEngineIFunction> funcZ = FdoFunctionZ::Create();
		m_supportedFunctions->Add(FdoPtr<FdoFunctionDefinition>(funcZ->GetFunctionDefinition()));
		
		// Add function M to the list of supported function
		FdoPtr<FdoExpressionEngineIFunction> funcM = FdoFunctionM::Create();
		m_supportedFunctions->Add(FdoPtr<FdoFunctionDefinition>(funcM->GetFunctionDefinition()));

    }

    return FDO_SAFE_ADDREF(m_supportedFunctions.p);
}
