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
#include "PostGIS/FdoPostGisOverrides.h"
// std
#include <cassert>
#include <string>
// boost
#include <boost/lexical_cast.hpp>
#include <boost/shared_ptr.hpp>

namespace fdo { namespace postgis {

DescribeSchemaCommand::DescribeSchemaCommand(Connection* conn)
    : mConn(conn)
{
    FDO_SAFE_ADDREF(mConn.p);
}

DescribeSchemaCommand::~DescribeSchemaCommand()
{
}

///////////////////////////////////////////////////////////////////////////////
// FdoIDisposable interface
///////////////////////////////////////////////////////////////////////////////

void DescribeSchemaCommand::Dispose()
{
    //FDOLOG_MARKER("DescribeSchemaCommand::#Dispose()");

    delete this;
}

///////////////////////////////////////////////////////////////////////////////
// FdoICommand interface
///////////////////////////////////////////////////////////////////////////////

FdoIConnection* DescribeSchemaCommand::GetConnection()
{
    FDO_SAFE_ADDREF(mConn.p);
    return mConn.p;
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
    return mSchemaName;
}

void DescribeSchemaCommand::SetSchemaName(FdoString* name)
{
    assert(NULL != name);
    mSchemaName = name;
}

FdoFeatureSchemaCollection* DescribeSchemaCommand::Execute()
{
    FDOLOG_MARKER("DescribeSchemaCommand::+Execute");

    try
    {
        FdoPtr<FdoFeatureSchemaCollection> logicalSchema = mConn->GetLogicalSchema();

        FDOLOG_WRITE("Number of schema elements fetched: %d", logicalSchema->GetCount());

        FDO_SAFE_ADDREF(logicalSchema.p);
        return logicalSchema.p;
    }
    catch (FdoException* e)
    {
        FDOLOG_WRITE("The execution of DescribeSchema command failed.");

        FdoCommandException* ne = NULL;
        ne = FdoCommandException::Create(L"The execution of DescribeSchema command failed.", e);
        e->Release();
        throw ne;
    }

    return NULL;
}

}} // namespace fdo::postgis
