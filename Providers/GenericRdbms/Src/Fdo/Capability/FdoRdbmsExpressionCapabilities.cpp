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
#include "FdoRdbmsExpressionCapabilities.h"

FdoRdbmsExpressionCapabilities::FdoRdbmsExpressionCapabilities(void)
{
}

FdoRdbmsExpressionCapabilities::~FdoRdbmsExpressionCapabilities(void)
{
}

FdoExpressionType* FdoRdbmsExpressionCapabilities::GetExpressionTypes(int& length)
{
    static FdoExpressionType expressionTypes[] = {  FdoExpressionType_Basic,
                                                    FdoExpressionType_Function,
                                                    FdoExpressionType_Parameter};
    length = sizeof(expressionTypes)/sizeof(FdoExpressionType);
    return expressionTypes;
}

FdoFunctionDefinitionCollection* FdoRdbmsExpressionCapabilities::GetFunctions()
{
    FdoFunctionDefinitionCollection* functions = FdoFunctionDefinitionCollection::Create();
    FdoPtr<FdoFunctionDefinitionCollection> fdoFuncs = GetWellKnownFunctions();

    functions->Add( FdoPtr<FdoFunctionDefinition>( fdoFuncs->GetItem( FDO_FUNCTION_CEIL ) ) );
    functions->Add( FdoPtr<FdoFunctionDefinition>( fdoFuncs->GetItem( FDO_FUNCTION_FLOOR ) ) );
    functions->Add( FdoPtr<FdoFunctionDefinition>( fdoFuncs->GetItem( FDO_FUNCTION_AVG ) ) );
    functions->Add( FdoPtr<FdoFunctionDefinition>( fdoFuncs->GetItem( FDO_FUNCTION_COUNT ) ) );
    functions->Add( FdoPtr<FdoFunctionDefinition>( fdoFuncs->GetItem( FDO_FUNCTION_MAX ) ) );
    functions->Add( FdoPtr<FdoFunctionDefinition>( fdoFuncs->GetItem( FDO_FUNCTION_MIN ) ) );
    functions->Add( FdoPtr<FdoFunctionDefinition>( fdoFuncs->GetItem( FDO_FUNCTION_SUM ) ) );
    functions->Add( FdoPtr<FdoFunctionDefinition>( fdoFuncs->GetItem( FDO_FUNCTION_LOWER ) ) );
    functions->Add( FdoPtr<FdoFunctionDefinition>( fdoFuncs->GetItem( FDO_FUNCTION_UPPER ) ) );
    functions->Add( FdoPtr<FdoFunctionDefinition>( fdoFuncs->GetItem( FDO_FUNCTION_CONCAT ) ) );

    return functions;
}

void FdoRdbmsExpressionCapabilities::Dispose()
{
    delete this;
}

