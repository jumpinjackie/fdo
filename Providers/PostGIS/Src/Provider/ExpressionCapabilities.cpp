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

#include <cassert>

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

FdoExpressionType* GetExpressionTypes(FdoInt32& size)
{
    assert(!"NOT IMPLEMENTED");
    return NULL;
}

FdoFunctionDefinitionCollection* GetFunctions()
{
    assert(!"NOT IMPLEMENTED");
    return NULL;
}

}} // namespace fdo::postgis
