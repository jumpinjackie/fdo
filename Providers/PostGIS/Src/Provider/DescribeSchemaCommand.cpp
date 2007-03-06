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
#include "PgSpatialTablesReader.h"
#include "PgGeometry.h" // TODO: Remove it

#include <cassert>
#include <iostream>

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
    FDOLOG_MARKER("DescribeSchemaCommand::#Dispose()");

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
    // XXX - Just testing
    using std::wcout;
    
    PgSpatialTablesReader::Ptr reader(new PgSpatialTablesReader(mConn.p));
    reader->Open();
    
    while (reader->ReadNext())
    {
        wcout << reader->GetSchemaName() << L" - ";
        wcout << reader->GetTableName() << std::endl;
        
        PgSpatialTablesReader::columns_t cols;
        cols = reader->GetGeometryColumns();
       
        PgSpatialTablesReader::columns_t::const_iterator it;
        for (it = cols.begin(); it != cols.end(); ++it)
        {            
            wcout << (*it)->GetName() << L" - "
                  << (*it)->GetGeometryType() << L" - "
                  << ewkb::GetOrdinatesFromDimension((*it)->GetDimensionType()) << L" - "
                  << (*it)->GetSRID() << std::endl;
        }
        
        wcout << std::endl;
    }
    
    return NULL;
}

}} // namespace fdo::postgis
