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
#include "SQLCommand.h"
#include "DeleteCommand.h"
#include "InsertCommand.h"
#include "SelectCommand.h"
#include "UpdateCommand.h"
#include "DescribeSchemaCommand.h"
#include "GetSpatialContextsCommand.h"
#include "ListDataStores.h"
#include "CreateDataStore.h"
#include "DestroyDataStore.h"

#define FDOPOSTGIS_MESSAGE_DEFINE
#include "../Message/inc/PostGisMessage.h"

// std
#include <cassert>

// External access to connection for client services
extern "C" FDOPOSTGIS_API FdoIConnection* CreateConnection()
{
    return (new fdo::postgis::Connection());
}

namespace fdo { namespace postgis {

Connection::Connection() :
    mConnInfo(NULL),
    mConnState(FdoConnectionState_Closed),
    mPgConn(NULL)
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
    return mConnString;
}

void Connection::SetConnectionString(FdoString* value)
{
    if (FdoConnectionState_Closed == GetConnectionState()
        || FdoConnectionState_Pending == GetConnectionState())
    {
        // Assign new connection string
        mConnString = value;

        // Update the connection property dictionary
        FdoPtr<ConnectionInfo> info = static_cast<ConnectionInfo*>(GetConnectionInfo());
        
        FdoPtr<FdoCommonConnPropDictionary> dict = 
            static_cast<FdoCommonConnPropDictionary*>(info->GetConnectionProperties());
        dict->UpdateFromConnectionString(mConnString);
    }
    else
    {
        throw FdoException::Create(NlsMsgGet(MSG_POSTGIS_CONNECTION_ALREADY_OPEN,
                                   "The connection is already open."));
    }
}

FdoIConnectionInfo* Connection::GetConnectionInfo()
{
    if (NULL == mConnInfo)
        mConnInfo = new ConnectionInfo(this);
    
    FDO_SAFE_ADDREF(mConnInfo.p);
    return NULL;
}

FdoConnectionState Connection::GetConnectionState()
{
    return mConnState;
}

FdoInt32 Connection::GetConnectionTimeout()
{
    assert(!"NOT SUPPORTED");
    return 0;
}

void Connection::SetConnectionTimeout(FdoInt32 value)
{
    assert(!"NOT SUPPORTED");
}

FdoConnectionState Connection::Open()
{
    if (FdoConnectionState_Open == GetConnectionState())
    {
        throw FdoException::Create(NlsMsgGet(MSG_POSTGIS_CONNECTION_ALREADY_OPEN,
                                   "The connection is already open."));
    }

    FdoPtr<FdoIConnectionInfo> info = GetConnectionInfo();
    FdoPtr<FdoIConnectionPropertyDictionary> dict = info->GetConnectionProperties();
    
    FdoStringP username = dict->GetProperty(PropertyUsername);
    FdoStringP password = dict->GetProperty(PropertyPassword);
    FdoStringP service = dict->GetProperty(PropertyService);
    FdoStringP datastore = dict->GetProperty(PropertyDatastore);


    // Establish connection with PostgreSQL database
    //mPgConn = PQsetdbLogin(pghost, pgport, pgoptions, pgtty, dbname, pglogin, pgpwd);

    return mConnState;
}

void Connection::Close()
{
    PQfinish(mPgConn);

    // Must not be used again after PQfinish has been called
    mPgConn = NULL;

    mConnState = FdoConnectionState_Closed;
}

FdoITransaction* Connection::BeginTransaction()
{
    assert(!"NOT IMPLEMENTED");
    return NULL;
}

FdoICommand* Connection::CreateCommand(FdoInt32 type)
{
    FdoPtr<FdoICommand> cmd;

    if (FdoConnectionState_Closed == GetConnectionState()
        || FdoConnectionState_Pending == GetConnectionState())
    {
        throw FdoException::Create(NlsMsgGet(MSG_POSTGIS_CONNECTION_INVALID,
                                   "Connection is invalid."));
    }

    // Create command of requested type
    switch(type)
    {
    case FdoCommandType_SQLCommand:
        cmd = new SQLCommand(this);
        break;
    case FdoCommandType_Select:
        cmd = new SelectCommand(this);
        break;
    case FdoCommandType_Insert:
        cmd = new InsertCommand(this);
        break;
    case FdoCommandType_Update:
        cmd = new UpdateCommand(this);
        break;
    case FdoCommandType_Delete:
        cmd = new DeleteCommand(this);
        break;
    case FdoCommandType_DescribeSchema:
        cmd = new DescribeSchemaCommand(this);
        break;   
    case FdoCommandType_DestroySchema:
        //cmd = new DestroySchemaCommand(this);
        assert(!"NOT IMPLEMENTED YET");
        break;
    case FdoCommandType_CreateDataStore:
        cmd = new CreateDataStore(this);
        break;
    case FdoCommandType_DestroyDataStore:
        cmd = new DestroyDataStore(this);
        break;
    case FdoCommandType_ListDataStores:
        cmd = new ListDataStores(this);
        break;
    case FdoCommandType_GetSpatialContexts:
        cmd = new GetSpatialContextsCommand(this);
        break;
    default:
        {
            FdoStringP cmdString(FdoCommonMiscUtil::FdoCommandTypeToString(type));
            throw FdoException::Create(NlsMsgGet(MSG_POSTGIS_COMMAND_NOT_SUPPORTED,
                  "The command %1$ls is not supported.",
                  static_cast<FdoString*>(cmdString)));
        }
    }

    FDO_SAFE_ADDREF(cmd.p);
    return cmd;
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
