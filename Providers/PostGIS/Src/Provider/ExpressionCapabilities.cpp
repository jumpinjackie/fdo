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

namespace fdo { namespace postgis {

ExpressionCapabilities::ExpressionCapabilities()
{
    FDOLOG_WRITE("ExpressionCapabilities created");
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
    // TODO: Verify this list together with ExpressionProcessor definition.

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
    // Collect supported functions from the Well-Known-Functions collection
    FdoPtr<FdoFunctionDefinitionCollection> wkFunctions;
    wkFunctions = GetWellKnownFunctions();

    FdoPtr<FdoFunctionDefinitionCollection> supported;
    supported = FdoFunctionDefinitionCollection::Create();
    
    FdoPtr<FdoFunctionDefinition> wkFunction;
    
    // TODO: Add supported functions. 
    
    FDO_SAFE_ADDREF(supported.p);
    return supported.p;
}

}} // namespace fdo::postgis
