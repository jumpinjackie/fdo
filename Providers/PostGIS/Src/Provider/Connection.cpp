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
#include "CommandCapabilities.h"
#include "ConnectionCapabilities.h"
#include "ExpressionCapabilities.h"
#include "FilterCapabilities.h"
#include "GeometryCapabilities.h"
#include "RasterCapabilities.h"
#include "SchemaCapabilities.h"
#include "TopologyCapabilities.h"
#include "ListDataStores.h"
#include "CreateDataStore.h"
#include "DestroyDataStore.h"
#include "ApplySchemaCommand.h"
#include "DescribeSchemaCommand.h"
#include "GetSpatialContextsCommand.h"
#include "InsertCommand.h"
#include "UpdateCommand.h"
#include "DeleteCommand.h"
#include "SelectCommand.h"
#include "SelectAggregatesCommand.h"
#include "SQLCommand.h"
#include "Transaction.h"
#include "PgCursor.h"
#include "PgUtility.h"
#include "md5.hpp"
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
#include <boost/md5.hpp>
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
    FDOLOG_WRITE("Connection instance ready.");
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
    Close();
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
    return (new ExpressionCapabilities());
}

FdoIRasterCapabilities* Connection::GetRasterCapabilities()
{
    return (new RasterCapabilities());
}

FdoITopologyCapabilities* Connection::GetTopologyCapabilities()
{
    return (new TopologyCapabilities());
}

FdoIGeometryCapabilities* Connection::GetGeometryCapabilities()
{
    return (new GeometryCapabilities());
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
        FDOLOG_WRITE("ERROR: Connection string is empty");
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
        FDOLOG_WRITE("ERROR: Connection is already open.");
        throw FdoException::Create(NlsMsgGet(MSG_POSTGIS_CONNECTION_ALREADY_OPEN,
                                   "Connection is already open."));
    }

    FDOLOG_WRITE("Connection string is valid");
}

FdoIConnectionInfo* Connection::GetConnectionInfo()
{
    if (NULL == mConnInfo)
    {
        mConnInfo = new ConnectionInfo(this);
    }

    FDO_SAFE_ADDREF(mConnInfo.p);
    return mConnInfo.p;
}

FdoConnectionState Connection::GetConnectionState()
{
    return mConnState;
}

FdoInt32 Connection::GetConnectionTimeout()
{
    FDOLOG_MARKER("Connection::+GetConnectionTimeout");
    FDOLOG_WRITE(L"NOT SUPPORTED");
    return 0;
}

void Connection::SetConnectionTimeout(FdoInt32 value)
{
    FDOLOG_MARKER("Connection::+SetConnectionTimeout");
    FDOLOG_WRITE(L"NOT SUPPORTED");
}

FdoConnectionState Connection::Open()
{
    FDOLOG_MARKER("Connection::+Open");

    if (FdoConnectionState_Open == GetConnectionState())
    {
        FDOLOG_WRITE("ERROR: The connection is already open.");
        throw FdoConnectionException::Create(
            NlsMsgGet(MSG_POSTGIS_CONNECTION_ALREADY_OPEN,
                "The connection is already open."));
    }

    //
    // Validate connection properties
    //
    FDOLOG_WRITE(L"- Start validation of connection properties");

    ValidateConnectionString();
    ValidateRequiredProperties();
    
    FDOLOG_WRITE(L"- OK");

    //
    // Attempt to establish connection
    //
    FdoPtr<FdoIConnectionInfo> info = GetConnectionInfo();
    FdoPtr<FdoCommonConnPropDictionary> dict = 
        static_cast<FdoCommonConnPropDictionary*>(info->GetConnectionProperties());

    //
    // Step 1 - Connect to PostgreSQL server
    //
    if (FdoConnectionState_Closed == GetConnectionState())
    {
        FDOLOG_WRITE(L"Step 1 - start - connecting to PostgreSQL server");
        
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

            FDOLOG_WRITE(L"ERROR: Connection open failed: %s", static_cast<FdoString*>(msg));
            throw FdoConnectionException::Create(msg);
        }

        mConnState = FdoConnectionState_Pending;
        
        FDOLOG_WRITE(L"Step 1 - finish - connection is pending");
    }

    //
    // Step 2 - open FDO datastore (PostgreSQL schema)
    //
    
    if (FdoConnectionState_Pending == GetConnectionState())
    {
        FDOLOG_WRITE(L"Step 2 - start - opening FDO datastore");
        
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
                
                FDOLOG_WRITE(L"Step 2 - FDO datastore open: %s",
                    static_cast<FdoString*>(datastore));
            }
            else
            {
                // 1st call, so return to user requesting for a datastore name
                prop->SetIsPropertyRequired(true);
                
                FDOLOG_WRITE(L"Step 2 - FDO datastore is required");

                assert(FdoConnectionState_Pending == GetConnectionState());
            }

            FDOLOG_WRITE(L"Step 2 - finish");
        }
    }

    return GetConnectionState();
}

void Connection::Close()
{
    FDOLOG_MARKER("Connection::+Close");

    if (FdoConnectionState_Closed != GetConnectionState())
    {
        // Kill all active transactions
        PgCommitSoftTransaction();
    }

    // Close connection to the server
    PQfinish(mPgConn);

    mPgConn = NULL;
    mConnState = FdoConnectionState_Closed;

    FDOLOG_WRITE("Connection is closed.");
}

FdoITransaction* Connection::BeginTransaction()
{
    FDOLOG_MARKER("Connection::+BeginTransaction");

    // TODO: Need to test the solution for unsupported nested transactions in PostgreSQL

    PgBeginSoftTransaction();
    if (mSoftTransactionLevel > 0)
    {
        Transaction::Ptr trans(new Transaction(this));

        FDO_SAFE_ADDREF(trans.p);
        return trans.p;
    }

    // No transaction has been started. A bug?
    return NULL;
}

FdoICommand* Connection::CreateCommand(FdoInt32 type)
{
    FDOLOG_MARKER("Connection::+CreateCommand");
    FDOLOG_WRITE("Command type enumerator: %d", type);

    // NOTE: A minimum required connection state for a datastore command is pending.
    // A minimum required state for feature command is open.
    // More details available in following discussion:
    // http://lists.osgeo.org/pipermail/fdo-internals/2007-February/000805.html

    // TODO: Verify what connection state is required for what commands
    //        || FdoConnectionState_Pending == GetConnectionState())
    if (FdoConnectionState_Closed == GetConnectionState())
    {
        FDOLOG_WRITE("ERROR: Connection is closed or invalid");
        throw FdoConnectionException::Create(
            NlsMsgGet(MSG_POSTGIS_CONNECTION_INVALID,
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
    case FdoCommandType_SelectAggregates:
        cmd = new SelectAggregatesCommand(this);
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
    case FdoCommandType_ApplySchema:
        cmd = new ApplySchemaCommand(this);
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
            FDOLOG_WRITE("ERROR: Unsupported command requested.");

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
    FDOLOG_MARKER("Connection::+SetConfiguration");
    FDOLOG_WRITE("NOT IMPLEMENTED");
}

void Connection::Flush()
{
    FDOLOG_MARKER("Connection::+Flush");
    FDOLOG_WRITE("NOT IMPLEMENTED");
}

///////////////////////////////////////////////////////////////////////////////
// Connection custom interface
///////////////////////////////////////////////////////////////////////////////

FdoFeatureSchemaCollection* Connection::GetLogicalSchema()
{
    FDOLOG_MARKER("Connection::+GetLogicalSchema");
    
    SchemaDescription::Ptr sc(DescribeSchema());

    return sc->GetLogicalSchemas();
}

ov::PhysicalSchemaMapping* Connection::GetPhysicalSchemaMapping()
{
    FDOLOG_MARKER("Connection::+GetLogicalSchema");

    SchemaDescription::Ptr sc(DescribeSchema());

    return sc->GetSchemaMapping();
}

SpatialContextCollection* Connection::GetSpatialContexts()
{
    FDOLOG_MARKER("Connection::+GetLogicalSchema");

    SchemaDescription::Ptr sc(DescribeSchema());

    return sc->GetSpatialContexts();
}

void Connection::PgExecuteCommand(char const* sql)
{
    FdoSize cmdTuples = 0;
    PgExecuteCommand(sql, cmdTuples);
}

void Connection::PgExecuteCommand(char const* sql, FdoSize& affected)
{
    FDOLOG_MARKER("Connection::+PgExecuteCommand");
    FDOLOG_WRITE("SQL:\n\t%s", sql);

    ValidateConnectionState();

    affected = 0;

    boost::shared_ptr<PGresult> pgRes(PQexec(mPgConn, sql), PQclear);

    ExecStatusType pgStatus = PQresultStatus(pgRes.get());
    if (PGRES_COMMAND_OK != pgStatus && PGRES_TUPLES_OK != pgStatus)
    {
        FdoStringP errCode(PQresStatus(pgStatus));
        FdoStringP errMsg(PQresultErrorMessage(pgRes.get()));

        FDOLOG_WRITE(L"SQL command failed: [%s] %s",
            static_cast<FdoString*>(errCode), static_cast<FdoString*>(errMsg));

        // TODO: Consider translation of PostgreSQL status to FDO exception (new types?)
        throw FdoCommandException::Create(NlsMsgGet(MSG_POSTGIS_SQL_STATEMENT_EXECUTION_FAILED,
            "The execution of SQL statement failed with PostgreSQL error code: %1$ls, %2$ls.",
            static_cast<FdoString*>(errCode), static_cast<FdoString*>(errMsg)));
    }

    try
    {
        std::string num(PQcmdTuples(pgRes.get()));
        if (!num.empty())
        {
            affected = boost::lexical_cast<std::size_t>(num);
        }
    }
    catch (boost::bad_lexical_cast& e)
    {
        affected = 0;
        FDOLOG_WRITE("ERROR: Type conversion failed: %s", e.what());
    }

    FDOLOG_WRITE("Affected tuples: %u", affected);
}

void Connection::PgExecuteCommand(char const* sql,
                                  details::pgexec_params_t const& params,
                                  FdoSize& affected)
{
    FDOLOG_MARKER("Connection::+PgExecuteCommand");
    FDOLOG_WRITE("SQL:\n\t%s\nNumber of parameters: %u", sql, params.size());

    ValidateConnectionState();

    affected = 0;
    
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
    // Check status of command execution
    //
    ExecStatusType pgStatus = PQresultStatus(pgRes.get());
    if (PGRES_COMMAND_OK != pgStatus && PGRES_TUPLES_OK != pgStatus)
    {
        FdoStringP errCode(PQresStatus(pgStatus));
        FdoStringP errMsg(PQresultErrorMessage(pgRes.get()));

        FDOLOG_WRITE(L"ERROR: SQL command failed: [%s] %s",
            static_cast<FdoString*>(errCode), static_cast<FdoString*>(errMsg));

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
        if (!num.empty())
        {
            affected = boost::lexical_cast<std::size_t>(num);
        }
    }
    catch (boost::bad_lexical_cast& e)
    {
        affected = 0;
        FDOLOG_WRITE("ERROR: Type conversion failed: %s", e.what());
    }

    FDOLOG_WRITE("Affected tuples: %u", affected);
}

PGresult* Connection::PgExecuteQuery(char const* sql)
{
    FDOLOG_MARKER("Connection::+PgExecuteQuery");
    FDOLOG_WRITE("SQL:\n\t%s", sql);

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
        FdoStringP errCode(PQresStatus(pgStatus));
        FdoStringP errMsg(PQresultErrorMessage(pgRes));

        // Free query result
        PQclear(pgRes);
        pgRes = NULL;

        FDOLOG_WRITE(L"ERROR: SQL query failed: [%s] %s",
            static_cast<FdoString*>(errCode), static_cast<FdoString*>(errMsg));

        // TODO: Consider translation of PostgreSQL status to FDO exception (new types?)
        throw FdoCommandException::Create(NlsMsgGet(MSG_POSTGIS_SQL_STATEMENT_EXECUTION_FAILED,
            "The execution of SQL statement failed with PostgreSQL error code: %1$ls, %2$ls.",
            static_cast<FdoString*>(errCode), static_cast<FdoString*>(errMsg)));
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
    std::time_t t;
    std::time(&t);
    std::string tstr(str(boost::format("%d") % t));
    fdo::postgis::md5 md5sum(tstr.c_str());
    std::string suffix(md5sum.digest().hex_str_value());
   
    std::string cursorName(name);
    cursorName += "_" + suffix;
   
    //assert(cursorName.size() < 64);    
    FDOLOG_WRITE("Cursor name: %s", cursorName.c_str());
    
    //
    // Create new instance of cursor
    // NOTE: This operation may throw
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
        FdoStringP errCode(PQresStatus(pgStatus));
        FdoStringP errMsg(PQresultErrorMessage(pgRes));

        // Free query result
        PQclear(pgRes);
        pgRes = NULL;

        FDOLOG_WRITE(L"ERROR: Describe portal command failed: [%s] %s",
            static_cast<FdoString*>(errCode), static_cast<FdoString*>(errMsg));

        throw FdoCommandException::Create(NlsMsgGet(MSG_POSTGIS_CURSOR_DESCRIBE_FAILED,
            "The describe portal command on PostgreSQL cursor failed: %1$ls, %2$ls.",
            static_cast<FdoString*>(errCode), static_cast<FdoString*>(errMsg)));
    }

    assert(NULL != pgRes);
    return pgRes;
}

void Connection::PgBeginSoftTransaction()
{
    FDOLOG_MARKER("Connection::+PgBeginSoftTransaction");
    assert(FdoConnectionState_Closed != GetConnectionState());
    
    mSoftTransactionLevel++;
    if (1 == mSoftTransactionLevel)
    {
        FDOLOG_WRITE("BEGIN TRANSACTION");
        
        boost::shared_ptr<PGresult> pgRes(PQexec(mPgConn, "BEGIN"), PQclear);
        if (PGRES_COMMAND_OK != PQresultStatus(pgRes.get()))
        {
            FDOLOG_WRITE("ERROR: BEGIN command failed: %s", PQerrorMessage(mPgConn));
            assert(!"BEGIN FAILED - TO BE REPLACED WITH EXCEPTION");
        }
    }
}

void Connection::PgCommitSoftTransaction()
{
    FDOLOG_MARKER("Connection::+PgCommitSoftTransaction");
    assert(FdoConnectionState_Closed != GetConnectionState());
    
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
                // TODO: Throw an exception on error

                FDOLOG_WRITE("ERROR: COMMIT command failed: %s", PQerrorMessage(mPgConn));
                assert(!"COMMIT FAILED - TO BE REPLACED WITH EXCEPTION");
            }        
        }
    }
}
void Connection::PgRollbackSoftTransaction()
{
    FDOLOG_MARKER("Connection::+PgRollbackSoftTransaction");
    assert(FdoConnectionState_Closed != GetConnectionState());
    
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
            // TODO: Throw an exception on error

            FDOLOG_WRITE("ERROR: COMMIT command failed: %s", PQerrorMessage(mPgConn));
            assert(!"ROLLBACK FAILED - TO BE REPLACED WITH EXCEPTION");
        } 
    }
}
void Connection::PgFlushSoftTransaction()
{
    FDOLOG_MARKER("Connection::+PgFlushSoftTransaction");
    assert(FdoConnectionState_Closed != GetConnectionState());
    
    // Force unwinding and commit of any active transaction
    //if (mSoftTransactionLevel > 1)
        mSoftTransactionLevel = 1;
        
    PgCommitSoftTransaction();
}

///////////////////////////////////////////////////////////////////////////////
// Private operations
///////////////////////////////////////////////////////////////////////////////

SchemaDescription* Connection::DescribeSchema()
{
    FDOLOG_MARKER("Connection::-DescribeSchema");

    if (NULL == mSchemaDesc)
    {
        // TODO: Add support of describing selected schema instead of all
        
        // TODO: Should we rename default schema name FdoPostGIS to 
        //       name of current PostgreSQL schema?
        //       What is best convention for naming FDO schema name in PostGIS provider?

        // FdoStringP schemaName(GetPgCurrentSchema());
        FdoStringP schemaName(L"FdoPostGIS");

        try
        {
            mSchemaDesc = SchemaDescription::Create();
            mSchemaDesc->DescribeSchema(this, static_cast<FdoString*>(schemaName));
        }
        catch (FdoException* e)
        {
            FDOLOG_WRITE(L"ERROR: Describe operation for '%s' failed",
                static_cast<FdoString*>(schemaName));

            FdoCommandException* ne = NULL;
            ne = FdoCommandException::Create(L"The execution of describe schema operation failed.", e);
            e->Release();
            throw ne;
        }
    }

    assert(mSchemaDesc->IsDescribed());

    FDO_SAFE_ADDREF(mSchemaDesc.p);
    return mSchemaDesc.p;
}

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
        FDOLOG_WRITE("ERROR: Invalid state of PostgreSQL connection.");
        throw FdoException::Create(NlsMsgGet(MSG_POSTGIS_INVALID_PGSQL_CONNECTION_STATE,
            "Invalid state of PostgreSQL connection."));
    }
}

void Connection::ValidateConnectionString()
{
    FdoStringP connStr(GetConnectionString());
    if (connStr.GetLength() <= 0)
    {
        FDOLOG_WRITE("ERROR: Connection string is empty.");
        throw FdoException::Create(NlsMsgGet(MSG_POSTGIS_EMPTY_CONNECTION_STRING,
            "Connection string is empty."));
    }

    FdoCommonConnStringParser parser(NULL, connStr);
    if (!parser.IsConnStringValid())
    {
        FDOLOG_WRITE(L"ERROR: Invalid connection string: %s",
            static_cast<FdoString*>(connStr));
        
        throw FdoException::Create(NlsMsgGet(MSG_POSTGIS_INVALID_CONNECTION_STRING,
            "Invalid connection string '%1$ls'", connStr));
    }

    FdoPtr<FdoIConnectionInfo> info = GetConnectionInfo();
    FdoPtr<FdoCommonConnPropDictionary> dict = 
        static_cast<FdoCommonConnPropDictionary*>(info->GetConnectionProperties());

    if (parser.HasInvalidProperties(dict))
    {
        FDOLOG_WRITE(L"ERROR: Invalid connection property name: %s",
            parser.GetFirstInvalidPropertyName(dict));
        
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
                FDOLOG_WRITE(L"ERROR: The connection property '%s' required but not set.",
                    propNames[i]);
                    
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
    FDOLOG_MARKER("Connection::-SetPgCurrentSchema");
    
    // If false, then we have a serious bug in Connection::Open()
    assert(schema.GetLength() > 0);

    ValidateConnectionState();

    // Schema is activated by setting the schema search path:
    // SET search_path TO <new_schema_name>

    // NOTE: 'public' schema is used as a common place where PostGIS metadata live.

    // TODO: Check if schema == 'public' and remove double: public,public.
    // TODO: Quote schema name!

    std::string sql("SET search_path TO ");
    sql += static_cast<const char*>(schema);
    sql += ", public";
    
    FDOLOG_WRITE("%s", sql.c_str());
    
    boost::shared_ptr<PGresult> pgRes(PQexec(mPgConn, sql.c_str()), PQclear);
    ExecStatusType pgResStatus = PQresultStatus(pgRes.get());
    if (PGRES_COMMAND_OK != pgResStatus)
    {
        FdoStringP errStatus(PQresStatus(pgResStatus));
        FdoStringP errMsg(PQresultErrorMessage(pgRes.get()));

        FDOLOG_WRITE("ERROR: SET search_path TO ... failed");

        throw FdoException::Create(NlsMsgGet(MSG_POSTGIS_SQL_STATEMENT_EXECUTION_FAILED,
            "SQL command failed with PostgreSQL error code: %1$ls. %2$ls.",
            static_cast<FdoString*>(errStatus), static_cast<FdoString*>(errMsg)));
    }
}

FdoStringP Connection::GetPgCurrentSchema()
{
    FDOLOG_MARKER("Connection::-GetPgCurrentSchema");
    
    ValidateConnectionState();

    // Schema is activated by setting the schema search path:
    // SET search_path TO <new_schema_name>

    char const* sql = "SELECT current_schema()";
    boost::shared_ptr<PGresult> pgRes(PgExecuteQuery(sql), PQclear);
    
    FdoStringP schemaName;
    if (PGRES_TUPLES_OK == PQresultStatus(pgRes.get()))
    {
        schemaName = PQgetvalue(pgRes.get(), 0, 0);
    }

    FDOLOG_WRITE(L"Current schema: %s", static_cast<FdoString*>(schemaName));

    return schemaName;
}

}} // namespace fdo::postgis
