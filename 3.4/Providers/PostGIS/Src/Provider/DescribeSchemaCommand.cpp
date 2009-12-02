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
    : mConn(conn), mSchemaName(L"FdoPostGIS"), mClassNames(NULL)
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
    FDOLOG_MARKER("DescribeSchemaCommand::+GetTransaction");
    FDOLOG_WRITE("NOT SUPPORTED");
    return NULL;
}

void DescribeSchemaCommand::SetTransaction(FdoITransaction* value)
{
    FDOLOG_MARKER("DescribeSchemaCommand::+SetTransaction");
    FDOLOG_WRITE("NOT SUPPORTED");
}

FdoInt32 DescribeSchemaCommand::GetCommandTimeout()
{
    FDOLOG_MARKER("DescribeSchemaCommand::+GetCommandTimeout");
    FDOLOG_WRITE("NOT SUPPORTED");

    return 0;
}

void DescribeSchemaCommand::SetCommandTimeout(FdoInt32 value)
{
    FDOLOG_MARKER("DescribeSchemaCommand::+SetCommandTimeout");
    FDOLOG_WRITE("NOT SUPPORTED");
}

FdoParameterValueCollection* DescribeSchemaCommand::GetParameterValues()
{
    FDOLOG_MARKER("DescribeSchemaCommand::+GetParameterValues");
    FDOLOG_WRITE("NOT SUPPORTED");

    return NULL;
}

void DescribeSchemaCommand::Prepare()
{
    FDOLOG_MARKER("DescribeSchemaCommand::+Prepare");
    FDOLOG_WRITE("NOT SUPPORTED");
}

void DescribeSchemaCommand::Cancel()
{
    FDOLOG_MARKER("DescribeSchemaCommand::+Cancel");
    FDOLOG_WRITE("NOT SUPPORTED");
}

///////////////////////////////////////////////////////////////////////////////
// FdoIDescribeSchema interface
///////////////////////////////////////////////////////////////////////////////

FdoString* DescribeSchemaCommand::GetSchemaName()
{
    // TODO: The FdoPostGIS is always returned for now.
    // This is name of default feature schema.
    // Currently, the provider allows to work with single-schema datastores.

    return mSchemaName;
}

void DescribeSchemaCommand::SetSchemaName(FdoString* name)
{
    assert(NULL != name);
    mSchemaName = name;
}

FdoStringCollection* DescribeSchemaCommand::GetClassNames()
{
    return mClassNames;
}

void DescribeSchemaCommand::SetClassNames(FdoStringCollection* value)
{
    // Do nothing.
    // This method is not implemented.  DescribeSchema command
    // will describe all classes.
}

FdoFeatureSchemaCollection* DescribeSchemaCommand::Execute()
{
    FDOLOG_MARKER("DescribeSchemaCommand::+Execute");

    try
    {
        FdoPtr<FdoFeatureSchemaCollection> logicalSchema = mConn->GetLogicalSchema();

        FDOLOG_WRITE("Number of schema elements fetched: %d", logicalSchema->GetCount());

        // need to create a copy to return beacuse of issue in MapGuide 
        // ( when decribe scchema command is running than MG serialize and temporary removes (and returns back ) 
        // class from this schema.
        // and if multisession's (multhi threading) application is running and if they share this same schema
        // then it got's exception.
        // If I create a copy and for every request return copy than it is OK.
        // and some other callers (FME) may change it and that is nt ok for us then
        
        FdoFeatureSchemaCollection* ret = FdoCommonSchemaUtil::DeepCopyFdoFeatureSchemas(logicalSchema);
        return ret;  
        //FDO_SAFE_ADDREF(logicalSchema.p);
        //return logicalSchema.p;
    }
    catch (FdoException* e)
    {
        FDOLOG_WRITE("The execution of DescribeSchema command failed.");
        FDOLOG_WRITE(L" - schema name: %ls", GetSchemaName());

        FdoCommandException* ne = NULL;
        ne = FdoCommandException::Create(
            NlsMsgGet(MSG_POSTGIS_DESCRIBE_SCHEMA_COMMAND_FAILED,
                "The describe schema command failed for '%1$ls' schema.",
                GetSchemaName()),
                e);
        e->Release();
        throw ne;
    }
}

}} // namespace fdo::postgis
