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
#include "Connection.h"
#include "ConnectionInfo.h"

#include <cassert>  

// External access to connection for client services
extern "C" FDOPOSTGIS_API FdoIConnection* CreateConnection()
{
    return (new fdo::postgis::Connection());
}

namespace fdo { namespace postgis {

Connection::Connection()
{
}

Connection::~Connection()
{
}

///////////////////////////////////////////////////////////////////////////////
// FdoIDisposable interface
///////////////////////////////////////////////////////////////////////////////

void Connection::Dispose()
{
    delete this;
}

///////////////////////////////////////////////////////////////////////////////
// FdoIConnection interface
///////////////////////////////////////////////////////////////////////////////

FdoIConnectionCapabilities* Connection::GetConnectionCapabilities()
{
    assert(!"NOT IMPLEMENTED");
    return NULL;
}

FdoISchemaCapabilities* Connection::GetSchemaCapabilities()
{
    assert(!"NOT IMPLEMENTED");
    return NULL;
}

FdoICommandCapabilities* Connection::GetCommandCapabilities()
{
    assert(!"NOT IMPLEMENTED");
    return NULL;
}

FdoIFilterCapabilities* Connection::GetFilterCapabilities()
{
    assert(!"NOT IMPLEMENTED");
    return NULL;
}

FdoIExpressionCapabilities* Connection::GetExpressionCapabilities()
{
    assert(!"NOT IMPLEMENTED");
    return NULL;
}

FdoIRasterCapabilities* Connection::GetRasterCapabilities()
{
    assert(!"NOT IMPLEMENTED");
    return NULL;
}

FdoITopologyCapabilities* Connection::GetTopologyCapabilities()
{
    assert(!"NOT IMPLEMENTED");
    return NULL;
}

FdoIGeometryCapabilities* Connection::GetGeometryCapabilities()
{
    assert(!"NOT IMPLEMENTED");
    return NULL;
}

FdoString* Connection::GetConnectionString()
{
    assert(!"NOT IMPLEMENTED");
    return NULL;
}

void Connection::SetConnectionString(FdoString* value)
{
    assert(!"NOT IMPLEMENTED");
}

FdoIConnectionInfo* Connection::GetConnectionInfo()
{
    if (NULL == mConnectionInfo)
        mConnectionInfo = new ConnectionInfo(this);
    
    FDO_SAFE_ADDREF(mConnectionInfo.p);
    return NULL;
}

FdoConnectionState Connection::GetConnectionState()
{
    assert(!"NOT IMPLEMENTED");
    return FdoConnectionState_Closed;
}

FdoInt32 Connection::GetConnectionTimeout()
{
    assert(!"NOT IMPLEMENTED");
    return 0;
}

void Connection::SetConnectionTimeout(FdoInt32 value)
{
    assert(!"NOT IMPLEMENTED");
}

FdoConnectionState Connection::Open()
{
    assert(!"NOT IMPLEMENTED");
    return FdoConnectionState_Closed;
}

void Connection::Close()
{
    assert(!"NOT IMPLEMENTED");
}

FdoITransaction* Connection::BeginTransaction()
{
    assert(!"NOT IMPLEMENTED");
    return NULL;
}

FdoICommand* Connection::CreateCommand(FdoInt32 type)
{
    assert(!"NOT IMPLEMENTED");
    return NULL;
}

FdoPhysicalSchemaMapping* Connection::CreateSchemaMapping()
{
    assert(!"NOT IMPLEMENTED");
    return NULL;
}

void Connection::SetConfiguration(FdoIoStream* configStream)
{
    assert(!"NOT IMPLEMENTED");
}

void Connection::Flush()
{
    assert(!"NOT IMPLEMENTED");
}

}} // namespace fdo::postgis
