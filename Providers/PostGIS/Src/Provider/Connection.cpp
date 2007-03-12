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
#include "FilterCapabilities.h"
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
#include "PgCursor.h"
#include "PgUtility.h"
// Message
#define FDOPOSTGIS_MESSAGE_DEFINE
#include "../Message/inc/PostGisMessage.h"
// Overrides
#include <PostGIS/Override/PhysicalSchemaMapping.h>
// std
#include <cassert>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
// boost
#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

// External access to connection for client services
extern "C" FDOPOSTGIS_API FdoIConnection* CreateConnection()
{
    FDOLOG_MARKER("fdo::postgis::CreateConnection");

    // Initialize random numbers generator
    std::time_t secs;
    std::time(&secs);
    std::srand(static_cast<unsigned int>(secs));
    
    return (new fdo::postgis::Connection());
}

namespace fdo { namespace postgis {

Connection::Connection() :
    mConnInfo(NULL),
    mConnState(FdoConnectionState_Closed),
    mPgConn(NULL),
    mPgResult(NULL),
    mSoftTransactionLevel(0),
    mSchemaDesc(NULL)
{
    // idle
}

Connection::~Connection()
{
    // idle
}

///////////////////////////////////////////////////////////////////////////////
// FdoIDisposable interface
///////////////////////////////////////////////////////////////////////////////

void Connection::Dispose()
{
    FDOLOG_MARKER("Connection::#Dispose");

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
    return (new FilterCapabilities());
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
    FDOLOG_MARKER("Connection::+SetConnectionString");

    // TODO: Replace with connection string parsing utils recommended by Romica
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
    FDOLOG_MARKER("Connection::+Open");

    if (FdoConnectionState_Open == GetConnectionState())
    {
        throw FdoConnectionException::Create(
            NlsMsgGet(MSG_POSTGIS_CONNECTION_ALREADY_OPEN,
                "The connection is already open."));
    }

    //
    // Validate connection properties
    //

    ValidateConnectionString();
    ValidateRequiredProperties();

    //
    // Attempt to establish connection
    //

    FdoPtr<FdoIConnectionInfo> info = GetConnectionInfo();
    //FdoPtr<FdoIConnectionPropertyDictionary> dict = info->GetConnectionProperties();
    FdoPtr<FdoCommonConnPropDictionary> dict = 
        static_cast<FdoCommonConnPropDictionary*>(info->GetConnectionProperties());

    //
    // Step 1 - Connect to PostgreSQL server
    //

    if (FdoConnectionState_Closed == GetConnectionState())
    {
        assert(FdoConnectionState_Open != GetConnectionState());
        assert(FdoConnectionState_Pending != GetConnectionState());

        // Generate PostgreSQL connection params from FDO connection properties
        details::pgconn_params_t pgParams = GetPgConnectionParams(dict);

        // Establish connection to database
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

        mConnState = FdoConnectionState_Pending;
    }

    //
    // Step 2 - open FDO datastore (PostgreSQL schema)
    //
    
    if (FdoConnectionState_Pending == GetConnectionState())
    {
        FdoStringP datastore;
        FdoPtr<ConnectionProperty> prop(dict->FindProperty(PropertyDatastore));
        if (NULL != prop)
        {
            datastore = prop->GetValue();
            if (datastore.GetLength() > 0)
            {
                // Set FDO datastore as a current schema for connection
                SetPgCurrentSchema(datastore);

                mConnState = FdoConnectionState_Open;
            }
            else
            {
                // 1st call, so return to user requesting for a datastore name
                prop->SetIsPropertyRequired(true);

                assert(FdoConnectionState_Pending == GetConnectionState());
            }
        }
    }

    return GetConnectionState();
}

void Connection::Close()
{
    FDOLOG_MARKER("Connection::+Close");
    
    // Kill all active transactions
    //PgFlushSoftTransaction();
    PgCommitSoftTransaction();

    // Close connection to the server
    PQfinish(mPgConn);

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
    FDOLOG_MARKER("Connection::+CreateCommand");
    FDOLOG_WRITE(L"Command type: %s",
        static_cast<FdoString*>(FdoCommonMiscUtil::FdoCommandTypeToString(type)));
    
    // NOTE: A minimum required connection state for a datastore command is pending.
    // A minimum required state for feature command is open.
    // More details available in following discussion:
    // http://lists.osgeo.org/pipermail/fdo-internals/2007-February/000805.html

    // TODO: Verify what connection state is required for what commands
    //        || FdoConnectionState_Pending == GetConnectionState())
    if (FdoConnectionState_Closed == GetConnectionState())
    {
        throw FdoException::Create(NlsMsgGet(MSG_POSTGIS_CONNECTION_INVALID,
                                   "Connection is closed or invalid."));
    }

    FdoPtr<FdoICommand> cmd;

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
// Connection custom interface
///////////////////////////////////////////////////////////////////////////////

FdoFeatureSchemaCollection* Connection::GetLogicalSchema()
{
    return NULL;
}

ov::PhysicalSchemaMapping* Connection::GetPhysicalSchemaMapping()
{
    return NULL;
}

SpatialContextCollection* Connection::GetSpatialContexts()
{
    return NULL;
}

void Connection::PgExecuteCommand(char const* sql)
{
    FdoSize cmdTuples = 0;
    PgExecuteCommand(sql, cmdTuples);
}

void Connection::PgExecuteCommand(char const* sql, FdoSize& affected)
{
    FDOLOG_MARKER("Connection::+PgExecuteCommand");
    FDOLOG_WRITE("SQL command: %s", sql);

    ValidateConnectionState();

    boost::shared_ptr<PGresult> pgRes(PQexec(mPgConn, sql), PQclear);

    ExecStatusType pgStatus = PQresultStatus(pgRes.get());
    if (PGRES_COMMAND_OK != pgStatus && PGRES_TUPLES_OK != pgStatus)
    {
        // TODO: What about automatically logging exception?
        // FDOLOG_WRITE("PostgreSQL failed executing SQL command:\n\tCODE: %s\n\t%s\n\tSQL: %s",
        //    PQresStatus(pgStatus), PQresultErrorMessage(pgRes.get()), sql);

        FdoStringP errCode(PQresStatus(pgStatus));
        FdoStringP errMsg(PQresultErrorMessage(pgRes.get()));

        // TODO: Consider translation of PostgreSQL status to FDO exception (new types?)
        throw FdoCommandException::Create(NlsMsgGet(MSG_POSTGIS_SQL_STATEMENT_EXECUTION_FAILED,
            "The execution of SQL statement failed with PostgreSQL error code: %1$ls, %2$ls.",
            static_cast<FdoString*>(errCode), static_cast<FdoString*>(errMsg)));
    }

    try
    {
        std::string num(PQcmdTuples(pgRes.get()));
        affected = boost::lexical_cast<std::size_t>(num);
    }
    catch (boost::bad_lexical_cast& e)
    {
        affected = 0;
        e; // Anti-warning hack
    }

    FDOLOG_WRITE("SQL affected tuples: %u", affected);
}

void Connection::PgExecuteCommand(char const* sql,
                                  details::pgexec_params_t const& params,
                                  FdoSize& affected)
{
    FDOLOG_MARKER("Connection::+PgExecuteCommand");
    FDOLOG_WRITE("SQL command: %s\nNumber of parameters: %u", sql, params.size());

    ValidateConnectionState();

    //
    // Re-write parameters to array of types accepted by PQexecParams.
    // The input collection 'params' is required to be ordered in the same way
    // as SQL placeholders ($1, $2, etc.).
    //
    typedef std::vector<char const*> params_t;
    typedef const char* const* params_ptr_t;

    params_t pgParams(0);

    for (details::pgexec_params_t::const_iterator it = params.begin();
         it != params.end(); ++it)
    {
        if (0 != it->second)
            pgParams.push_back(NULL);
        else
            pgParams.push_back(it->first.c_str());
    }
    assert(pgParams.size() == params.size());

    //
    // Execute the SQL statement and wrap the results with smart pointer
    //
    params_ptr_t paramsPtr = (true == pgParams.empty() ? NULL : &pgParams[0]);

    boost::shared_ptr<PGresult> pgRes(
        PQexecParams(mPgConn, sql,
            static_cast<int>(pgParams.size()), NULL, paramsPtr, NULL, NULL, 0), PQclear);

    paramsPtr = NULL; // Assure it won't be used

    //
    // Do some diagnostics
    //
    ExecStatusType pgStatus = PQresultStatus(pgRes.get());
    if (PGRES_COMMAND_OK != pgStatus && PGRES_TUPLES_OK != pgStatus)
    {
        FdoStringP errCode(PQresStatus(pgStatus));
        FdoStringP errMsg(PQresultErrorMessage(pgRes.get()));

        // TODO: Consider translation of PostgreSQL status to FDO exception (new types?)
        throw FdoCommandException::Create(NlsMsgGet(MSG_POSTGIS_SQL_STATEMENT_EXECUTION_FAILED,
            "The execution of SQL statement failed with PostgreSQL error code: %1$ls, %2$ls.",
            static_cast<FdoString*>(errCode), static_cast<FdoString*>(errMsg)));
    }

    //
    // Get number of affected tuples
    //
    try
    {
        std::string num(PQcmdTuples(pgRes.get()));
        affected = boost::lexical_cast<std::size_t>(num);
    }
    catch (boost::bad_lexical_cast& e)
    {
        affected = 0;
        e; // Anti-warning hack
    }

    FDOLOG_WRITE("SQL affected tuples: %u", affected);
}

PGresult* Connection::PgExecuteQuery(char const* sql)
{
    FDOLOG_MARKER("Connection::+PgExecuteQuery");
    FDOLOG_WRITE("SQL command: %s", sql);

    ValidateConnectionState();

    PGresult* pgRes = NULL;
    ExecStatusType pgStatus = PGRES_FATAL_ERROR;
    
    pgRes = PQexec(mPgConn, sql);
    if (NULL != pgRes)
    {
        pgStatus = PQresultStatus(pgRes);
    }
    
    if (PGRES_TUPLES_OK != pgStatus)
    {
        // TODO: Consider throwing an exception
        FDOLOG_WRITE("SQL query: [%s] %s",
            PQresStatus(pgStatus), PQresultErrorMessage(pgRes));
    }

    assert(NULL != pgRes);
    return pgRes;
}

fdo::postgis::PgCursor* Connection::PgCreateCursor(char const* name)
{
    FDOLOG_MARKER("Connection::+PgCreateCursor");

    ValidateConnectionState();

    //
    // Generate random suffix for cursor name to make name more unique
    //
    std::string suffix("1979"); // Just the year of birth, why not?
    try
    {
        int rn = std::rand() % (500 - 0 + 1) + 0;
        suffix = boost::lexical_cast<std::string>(rn);
    }
    catch (boost::bad_lexical_cast& e)
    {
        FDOLOG_WRITE("Types conversion failed: %s", e.what());
        assert(!"CONVERSION FAILED");
    }

    std::string cursorName(name);
    cursorName += "_" + suffix;
    FDOLOG_WRITE("Cursor name: %s", cursorName.c_str());
    
    //
    // Create new instance of cursor
    //
    PgCursor::Ptr cursor = new PgCursor(this, cursorName);

    FDO_SAFE_ADDREF(cursor.p);
    return cursor.p;
}

PGresult* Connection::PgDescribeCursor(char const* name)
{
    ValidateConnectionState();

    PGresult* pgRes = NULL;
    ExecStatusType pgStatus = PGRES_FATAL_ERROR;

    pgRes = PQdescribePortal(mPgConn, name);
    if (NULL != pgRes)
    {
        pgStatus = PQresultStatus(pgRes);
    }

    if (PGRES_COMMAND_OK != pgStatus)
    {
        // TODO: Consider throwing an exception
        FDOLOG_WRITE("Describe portal command failed: [%s] %s",
            PQresStatus(pgStatus), PQresultErrorMessage(pgRes));

        throw FdoException::Create(L"Describe portal command failed.");
    }

    assert(NULL != pgRes);
    return pgRes;
}

void Connection::PgBeginSoftTransaction()
{
    FDOLOG_MARKER("Connection::+PgBeginSoftTransaction");
    assert(FdoConnectionState_Open == GetConnectionState());
    
    mSoftTransactionLevel++;
    if (1 == mSoftTransactionLevel)
    {
        FDOLOG_WRITE("BEGIN TRANSACTION");
        
        boost::shared_ptr<PGresult> pgRes(PQexec(mPgConn, "BEGIN"), PQclear);
        if (PGRES_COMMAND_OK != PQresultStatus(pgRes.get()))
        {
            FDOLOG_WRITE("BEGIN Transaction failed: %s", PQerrorMessage(mPgConn));
            assert(!"BEGIN FAILED - TO BE REPLACED WITH EXCEPTION");
        }
    }
}

void Connection::PgCommitSoftTransaction()
{
    FDOLOG_MARKER("Connection::+PgCommitSoftTransaction");
    assert(FdoConnectionState_Open == GetConnectionState());
    
    if (0 >= mSoftTransactionLevel)
    {
        FDOLOG_WRITE("No active transaction to commit");
    }
    else
    {
        mSoftTransactionLevel--;
        if (0 == mSoftTransactionLevel)
        {
            boost::shared_ptr<PGresult> pgRes(PQexec(mPgConn, "COMMIT"), PQclear);
            if (PGRES_COMMAND_OK != PQresultStatus(pgRes.get()))
            {
                FDOLOG_WRITE("COMMIT Transaction failed: %s", PQerrorMessage(mPgConn));
                assert(!"COMMIT FAILED - TO BE REPLACED WITH EXCEPTION");
            }        
        }
    }
}
void Connection::PgRollbackSoftTransaction()
{
    FDOLOG_MARKER("Connection::+PgRollbackSoftTransaction");
    assert(FdoConnectionState_Open == GetConnectionState());
    
    if (0 >= mSoftTransactionLevel)
    {
        FDOLOG_WRITE("No active transaction to commit");
    }
    else
    {
        mSoftTransactionLevel = 0;
        boost::shared_ptr<PGresult> pgRes(PQexec(mPgConn, "ROLLBACK"), PQclear);
        if (PGRES_COMMAND_OK != PQresultStatus(pgRes.get()))
        {
            FDOLOG_WRITE("COMMIT Transaction failed: %s", PQerrorMessage(mPgConn));
            assert(!"ROLLBACK FAILED - TO BE REPLACED WITH EXCEPTION");
        } 
    }
}
void Connection::PgFlushSoftTransaction()
{
    FDOLOG_MARKER("Connection::+PgFlushSoftTransaction");
    assert(FdoConnectionState_Open == GetConnectionState());
    
    // Force unwinding and commit of any active transaction
    //if (mSoftTransactionLevel > 1)
        mSoftTransactionLevel = 1;
        
    PgCommitSoftTransaction();
}

///////////////////////////////////////////////////////////////////////////////
// Private operations
///////////////////////////////////////////////////////////////////////////////

void Connection::ValidateConnectionState()
{
    bool valid = false;

    if (NULL == mPgConn)
    {
        if (FdoConnectionState_Closed == GetConnectionState())
            valid = true;
    }
    else
    {
        if (FdoConnectionState_Open == GetConnectionState()
            || FdoConnectionState_Pending == GetConnectionState()
            || FdoConnectionState_Busy == GetConnectionState())
        {
            // Check PostgreSQL connection status
            if (CONNECTION_OK == PQstatus(mPgConn))
                valid = true;
        }    
    }

    if (!valid)
    {
        throw FdoException::Create(NlsMsgGet(MSG_POSTGIS_INVALID_PGSQL_CONNECTION_STATE,
            "Invalid state of PostgreSQL connection."));
    }
}

void Connection::ValidateConnectionString()
{
    FdoStringP connStr(GetConnectionString());
    if (connStr.GetLength() <= 0)
    {
        throw FdoException::Create(NlsMsgGet(MSG_POSTGIS_EMPTY_CONNECTION_STRING,
            "Connection string is empty."));
    }

    FdoCommonConnStringParser parser(NULL, connStr);
    if (!parser.IsConnStringValid())
    {
        throw FdoException::Create(NlsMsgGet(MSG_POSTGIS_INVALID_CONNECTION_STRING,
            "Invalid connection string '%1$ls'", connStr));
    }

    FdoPtr<FdoIConnectionInfo> info = GetConnectionInfo();
    FdoPtr<FdoCommonConnPropDictionary> dict = 
        static_cast<FdoCommonConnPropDictionary*>(info->GetConnectionProperties());

    if (parser.HasInvalidProperties(dict))
    {
        throw FdoException::Create(NlsMsgGet(MSG_POSTGIS_INVALID_CONNECTION_PROPERTY,
            "Invalid connection property name '%1$ls'",
             parser.GetFirstInvalidPropertyName(dict)));
    }
}

void Connection::ValidateRequiredProperties()
{
    FdoPtr<FdoIConnectionInfo> info = GetConnectionInfo();
    FdoPtr<FdoCommonConnPropDictionary> dict = 
        static_cast<FdoCommonConnPropDictionary*>(info->GetConnectionProperties());

    FdoInt32 size = 0;
    FdoString** propNames = dict->GetPropertyNames(size);
    assert(NULL != propNames);

    for (FdoInt32 i = 0; i < size; i++)
    {
        if (dict->IsPropertyRequired(propNames[i]))
        {
            FdoStringP propValue(dict->GetProperty(propNames[i]));
            if (propValue.GetLength() <= 0)
            {
                throw FdoException::Create(
                    NlsMsgGet(MSG_POSTGIS_CONNECTION_MISSING_REQUIRED_PROPERTY,
                    "The connection property '%1$ls' is required but wasn't set.",
                    propNames[i]));
            }
        }
    }
}

details::pgconn_params_t Connection::GetPgConnectionParams(
    FdoPtr<FdoCommonConnPropDictionary> dict)
{
    FDOLOG_MARKER("Connection::-GetPgConnectionParams");

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

    // database@host
    if (tokens.size() > 1)
    {
        host = tokens.at(1);
    
        // database@host:port
        if (tokens.size() > 2)
        {
            port = tokens.at(2);
        }
    }

    // NOTE: The pgconn_params_t has room for these values,
    //       but they are not used now, so empty strings are passed.
    std::string opts;
    std::string tty;

    FDOLOG_WRITE("Connection string:\n\thost=%s\n\tport=%s\n\tdb=%s\n\tlogin=%s",
                 host.c_str(), port.c_str(), db.c_str(), login.c_str());

    // pghost, pgport, pgoptions, pgtty, dbname, pglogin, pgpwd
    return make_tuple(host, port, opts, tty, db, login, pwd);
}

void Connection::SetPgCurrentSchema(FdoStringP schema)
{
    assert(schema.GetLength() > 0 && "Indicates serious bug in Connection::Open()");

    ValidateConnectionState();

    // Schema is activated by setting the schema search path:
    // SET search_path TO <new_schema_name>

    // NOTE: 'public' schema is used as a common place where PostGIS metadata live.

    std::string sql("SET search_path TO ");
    sql += static_cast<const char*>(schema);
    sql += ", public";

    boost::shared_ptr<PGresult> pgRes(PQexec(mPgConn, sql.c_str()), PQclear);

    ExecStatusType pgResStatus = PQresultStatus(pgRes.get());
    if (PGRES_COMMAND_OK != pgResStatus)
    {
        FdoStringP errStatus(PQresStatus(pgResStatus));
        FdoStringP errMsg(PQresultErrorMessage(pgRes.get()));

        throw FdoException::Create(NlsMsgGet(MSG_POSTGIS_SQL_STATEMENT_EXECUTION_FAILED,
            "SQL command failed with PostgreSQL error code: %1$ls. %2$ls.",
            static_cast<FdoString*>(errStatus), static_cast<FdoString*>(errMsg)));
    }
}

}} // namespace fdo::postgis
