    //
// Copyright (C) 2007 Refractions Research, Inc. 
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
//
#include "stdafx.h"
#include "PostGisProvider.h"
#include "ExpressionCapabilities.h"
#include <FdoExpressionEngine.h>
#include <Functions/Geometry/FdoFunctionX.h>
#include <Functions/Geometry/FdoFunctionY.h>
#include <Functions/Geometry/FdoFunctionZ.h>
#include <Functions/Geometry/FdoFunctionM.h>

namespace fdo { namespace postgis {

ExpressionCapabilities::ExpressionCapabilities()
{
}

ExpressionCapabilities::~ExpressionCapabilities()
{
}

///////////////////////////////////////////////////////////////////////////////
// FdoIDisposable interface
///////////////////////////////////////////////////////////////////////////////

void ExpressionCapabilities::Dispose()
{
    delete this;
}

///////////////////////////////////////////////////////////////////////////////
// FdoIExpressionCapabilities interface
///////////////////////////////////////////////////////////////////////////////

FdoExpressionType* ExpressionCapabilities::GetExpressionTypes(FdoInt32& size)
{
    // TODO: Update the list when parameters expression are implemented.

    static FdoExpressionType types[] =
    {
        FdoExpressionType_Basic,
        FdoExpressionType_Function,
    };

    size = sizeof(types) / sizeof(FdoExpressionType);

    return types;
}

FdoFunctionDefinitionCollection* ExpressionCapabilities::GetFunctions()
{    
   FdoPtr<FdoFunctionDefinitionCollection> supportedFunctions;
	 supportedFunctions = FdoExpressionEngine::GetStandardFunctions();

   //Removing the Extract function that return Dates
   //User will use ExtractToDouble or ExtractToInt
   FdoInt32 index = supportedFunctions->IndexOf(FDO_FUNCTION_EXTRACT);
   if(index >=0)
   {
     supportedFunctions->RemoveAt(index);
   }

   // Add function X to the list of supported function
   FdoPtr<FdoExpressionEngineIFunction> funcX = FdoFunctionX::Create();
   supportedFunctions->Add(FdoPtr<FdoFunctionDefinition>(funcX->GetFunctionDefinition()));

   // Add function Y to the list of supported function
   FdoPtr<FdoExpressionEngineIFunction> funcY = FdoFunctionY::Create();
   supportedFunctions->Add(FdoPtr<FdoFunctionDefinition>(funcY->GetFunctionDefinition()));

   // Add function Z to the list of supported function
   FdoPtr<FdoExpressionEngineIFunction> funcZ = FdoFunctionZ::Create();
   supportedFunctions->Add(FdoPtr<FdoFunctionDefinition>(funcZ->GetFunctionDefinition()));

   // Add function M to the list of supported function
   FdoPtr<FdoExpressionEngineIFunction> funcM = FdoFunctionM::Create();
   supportedFunctions->Add(FdoPtr<FdoFunctionDefinition>(funcM->GetFunctionDefinition()));

   return (FDO_SAFE_ADDREF (supportedFunctions.p));

}

}} // namespace fdo::postgis
