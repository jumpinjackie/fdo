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
#include "DescribeSchemaCommand.h"

#include <cassert>

namespace fdo { namespace postgis {

DescribeSchemaCommand::DescribeSchemaCommand()
{
}

DescribeSchemaCommand::~DescribeSchemaCommand()
{
}

///////////////////////////////////////////////////////////////////////////////
// FdoIDisposable interface
///////////////////////////////////////////////////////////////////////////////

void DescribeSchemaCommand::Dispose()
{
    delete this;
}

///////////////////////////////////////////////////////////////////////////////
// FdoICommand interface
///////////////////////////////////////////////////////////////////////////////

FdoIConnection* DescribeSchemaCommand::GetConnection()
{
    assert(!"NOT IMPLEMENTED");
    return NULL;
}

FdoITransaction* DescribeSchemaCommand::GetTransaction()
{
    assert(!"NOT IMPLEMENTED");
    return NULL;
}

void DescribeSchemaCommand::SetTransaction(FdoITransaction* value)
{
    assert(!"NOT IMPLEMENTED");
}

FdoInt32 DescribeSchemaCommand::GetCommandTimeout()
{
    assert(!"NOT IMPLEMENTED");
    return 0;
}

void DescribeSchemaCommand::SetCommandTimeout(FdoInt32 value)
{
    assert(!"NOT IMPLEMENTED");
}

FdoParameterValueCollection* DescribeSchemaCommand::GetParameterValues()
{
    assert(!"NOT IMPLEMENTED");
    return NULL;
}

void DescribeSchemaCommand::Prepare()
{
    assert(!"NOT IMPLEMENTED");
}

void DescribeSchemaCommand::Cancel()
{
    assert(!"NOT IMPLEMENTED");
}

///////////////////////////////////////////////////////////////////////////////
// FdoIDescribeSchema interface
///////////////////////////////////////////////////////////////////////////////

FdoString* DescribeSchemaCommand::GetSchemaName()
{
    assert(!"NOT IMPLEMENTED");
    return NULL;
}


void DescribeSchemaCommand::SetSchemaName(FdoString* value)
{
    assert(!"NOT IMPLEMENTED");
}

FdoFeatureSchemaCollection* DescribeSchemaCommand::Execute()
{
    assert(!"NOT IMPLEMENTED");
    return NULL;
}

}} // namespace fdo::postgis
