/*
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
#include "FdoRdbmsOdbcExpressionCapabilities.h"

FdoRdbmsOdbcExpressionCapabilities::FdoRdbmsOdbcExpressionCapabilities(void) 
{
}

FdoRdbmsOdbcExpressionCapabilities::~FdoRdbmsOdbcExpressionCapabilities(void)
{
}

FdoExpressionType* FdoRdbmsOdbcExpressionCapabilities::GetExpressionTypes(int& length)
{    
    static FdoExpressionType expressionTypes[] =
    {
        FdoExpressionType_Basic,
        FdoExpressionType_Function
    };

    length = sizeof(expressionTypes)/sizeof(FdoExpressionType);
    return expressionTypes;
}

FdoFunctionDefinitionCollection* FdoRdbmsOdbcExpressionCapabilities::GetFunctions()
{
    // NOTE: the function capabilities list, like other capabilities, is static.
    //   That is to say, we don't return different function capabilities depending on which driver we are
    //   connected to.  This is consistent with other providers.

    FdoFunctionDefinitionCollection * ret = NULL;
    {
        FdoPtr<FdoFunctionDefinitionCollection> fdoFuncs = GetWellKnownFunctions();
        FdoPtr<FdoFunctionDefinitionCollection> functions = FdoFunctionDefinitionCollection::Create();

        // Add all FDO "well known function" definitions that ODBC can support:
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

        // For some reason (possibly a Visual Studio compiler bug), the popular
        // "return FDO_SAFE_ADDREF(smartpointer.p)" results in an extra Release().
        // Using "ret" and an extra code block here works around it.
        FDO_SAFE_ADDREF(functions.p);
        ret = functions;
    }

    return ret;
}

void FdoRdbmsOdbcExpressionCapabilities::Dispose()
{
    delete this;
}
