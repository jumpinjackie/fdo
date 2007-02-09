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
#include "ConnectionCapabilities.h"
#include "CommandCapabilities.h"
#include "SchemaCapabilities.h"
#include "ListDataStores.h"
#include "CreateDataStore.h"
#include "DestroyDataStore.h"
#include "DescribeSchemaCommand.h"
#include "GetSpatialContextsCommand.h"
#include "DeleteCommand.h"
#include "InsertCommand.h"
#include "SelectCommand.h"
#include "UpdateCommand.h"
#include "SQLCommand.h"

// Message
#define FDOPOSTGIS_MESSAGE_DEFINE
#include "../Message/inc/PostGisMessage.h"
// Overrides
#include <PostGIS/Override/PhysicalSchemaMapping.h>
// std
#include <cassert>
#include <string>
#include <vector>
// boost
#include <boost/tuple/tuple.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
// PostgreSQL client library
#include <libpq-fe.h>


#include <fstream>
// External access to connection for client services
extern "C" FDOPOSTGIS_API FdoIConnection* CreateConnection()
{
    std::ofstream s("log.txt");
    s << "CreateConnection";

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
    return (new ConnectionCapabilities());
}

FdoISchemaCapabilities* Connection::GetSchemaCapabilities()
{
    return (new SchemaCapabilities());
}

FdoICommandCapabilities* Connection::GetCommandCapabilities()
{
    return (new CommandCapabilities());
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
    if (FdoStringP(value).GetLength() <= 0)
    {
        throw FdoException::Create(NlsMsgGet(MSG_POSTGIS_EMPTY_CONNECTION_STRING,
                                   "Connection string is empty."));
    }

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
                                   "Connection is already open."));
    }
}

FdoIConnectionInfo* Connection::GetConnectionInfo()
{
    if (NULL == mConnInfo)
        mConnInfo = new ConnectionInfo(this);
    
    FDO_SAFE_ADDREF(mConnInfo.p);
    return mConnInfo.p;
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
    
    // Establish connection with PostgreSQL database
    pgconn_params_t pgParams = GetPgConnectionParams(dict);

    mPgConn = PQsetdbLogin(pgParams.get<0>().c_str(),
                           pgParams.get<1>().c_str(),
                           NULL, NULL, 
                           pgParams.get<4>().c_str(),
                           pgParams.get<5>().c_str(),
                           pgParams.get<6>().c_str());

    if (CONNECTION_OK != PQstatus(mPgConn))
    {
        // Ask for failure reason
        FdoStringP msg(PQerrorMessage(mPgConn));

        // Release server-side resources
        Close();
        assert(NULL == mPgConn);

        throw FdoConnectionException::Create(msg);
    }

    // Connected
    mConnState = FdoConnectionState_Open;

    return GetConnectionState();
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
    return cmd.p;
}

FdoPhysicalSchemaMapping* Connection::CreateSchemaMapping()
{
    typedef fdo::postgis::ov::PhysicalSchemaMapping Mapping;
    Mapping::Ptr psm = Mapping::Create();
    
    FDO_SAFE_ADDREF(psm.p);
    return psm.p;
}

void Connection::SetConfiguration(FdoIoStream* configStream)
{
    assert(!"NOT IMPLEMENTED");
}

void Connection::Flush()
{
    assert(!"NOT IMPLEMENTED");
}

///////////////////////////////////////////////////////////////////////////////
// Private operations
///////////////////////////////////////////////////////////////////////////////

Connection::pgconn_params_t Connection::GetPgConnectionParams(
    FdoPtr<FdoIConnectionPropertyDictionary> dict)
{
    using boost::tuples::tuple;
    using boost::tuples::make_tuple;
    using boost::algorithm::split;
    using boost::algorithm::is_any_of;
    
    // Collect connection details
    FdoStringP fdoUsername = dict->GetProperty(PropertyUsername);
    FdoStringP fdoPassword = dict->GetProperty(PropertyPassword);
    FdoStringP fdoService = dict->GetProperty(PropertyService);
    FdoStringP fdoHostname = PropertyDefaultHostname;
    FdoStringP fdoPort = PropertyDefaultPort;

    // Convert to char-string
    std::string login(static_cast<const char*>(fdoUsername));
    std::string pwd(static_cast<const char*>(fdoPassword));
    std::string service(static_cast<const char*>(fdoService));

    // Split service string of possible formats:
    // - database - use default localhost and port (DEF_PGPORT_STR)
    // - database@host - use default port
    // - database@host:port

    std::vector<std::string> tokens;
    split(tokens, service, is_any_of("@:"));

    // TODO: Replace with FDO exception
    assert(1 <= tokens.size() && tokens.size() <= 3);

    std::string db(tokens.at(0));
    std::string host(static_cast<const char*>(fdoHostname));
    std::string port(static_cast<const char*>(fdoPort));

    // deatabase@host
    if (tokens.size() > 1)
    {
        host = tokens.at(1);
    
        // deatabase@host:port
        if (tokens.size() > 2)
        {
            port = tokens.at(2);
        }
    }

    // NOTE: The pgconn_params_t has room for these values,
    //       but they are not used now, so empty strings are passed.
    std::string opts;
    std::string tty;

    // pghost, pgport, pgoptions, pgtty, dbname, pglogin, pgpwd
    return make_tuple(host, port, opts, tty, db, login, pwd);
}

}} // namespace fdo::postgis

