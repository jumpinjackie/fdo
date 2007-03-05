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
#include "PgSpatialTablesReader.h"
#include "Connection.h"
// std
#include <cassert>
#include <string>
// boost
#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>

namespace fdo { namespace postgis {

PgSpatialTablesReader::PgSpatialTablesReader(Connection* conn)
    : mConn(conn), mReader(NULL), mCmd(NULL)
{
    FDO_SAFE_ADDREF(mConn.p);

    ValidateConnectionState();

    // Read datastore name representing physical schema in PostgreSQL database.
    // TODO: Consider replacing it with PostgreSQL function: current_schema()
    //       Currently, use current_schema() for validation purpose.

    FdoPtr<FdoIConnectionInfo> info = mConn->GetConnectionInfo();
    FdoPtr<FdoCommonConnPropDictionary> dict = 
        static_cast<FdoCommonConnPropDictionary*>(info->GetConnectionProperties());
        
    FdoStringP dsName = dict->GetProperty(PropertyDatastore);
    mCurrentSchema = static_cast<char const*>(dsName);
    
    // Manually validate current schema
    // TODO: This validation can be safely removed after testing period
    //       or leave it if you like it.
    {
        boost::shared_ptr<PGresult> pgRes(
            mConn->PgExecuteQuery("SELECT current_schema()"), PQclear);
        assert(PGRES_TUPLES_OK == PQntuples(pgRes.get()));

        std::string schemaName(PQgetvalue(pgRes.get(), 0, 0));
        assert(mCurrentSchema == schemaName);
    }
}

PgSpatialTablesReader::~PgSpatialTablesReader()
{
}

///////////////////////////////////////////////////////////////////////////////
// FdoIDisposable interface
///////////////////////////////////////////////////////////////////////////////

void PgSpatialTablesReader::Dispose()
{
    FDOLOG_MARKER("PgSpatialTablesReader::#Dispose");

    // May throw, do not call from the destructor!
    Close();

    delete this;
}

///////////////////////////////////////////////////////////////////////////////
// PgSpatialTablesReader interface
///////////////////////////////////////////////////////////////////////////////

FdoStringP PgSpatialTablesReader::GetSchemaName() const
{
    return mReader->GetString(L"schemaname");
}

FdoStringP PgSpatialTablesReader::GetTableName() const
{
    return mReader->GetString(L"tablename");
}

void PgSpatialTablesReader::Open()
{
    FDOLOG_MARKER("PgSpatialTablesReader::+Open");

    assert(!mCurrentSchema.empty());

//"g.f_geometry_column AS geomname, g.type, g.coord_dimension, g.srid "     
  
    std::string sql("SELECT n.nspname AS schemaname, c.relname AS tablename "
                    "FROM pg_class c, pg_namespace n, geometry_columns g "
                    "WHERE c.relkind = 'r' AND c.relname !~ '^(pg_|sql_)' "
                    "AND c.relnamespace = n.oid AND n.nspname = g.f_table_schema "
                    "AND c.relname::TEXT = g.f_table_name::TEXT "
                    "AND n.nspname = '" + mCurrentSchema + "'");
    
    // Query spatial tables and attach results to the SQL data reader
    mCmd = static_cast<FdoISQLCommand*>(mConn->CreateCommand(FdoCommandType_SQLCommand));
    assert(NULL != mCmd);
    
    FdoStringP tmp(sql.c_str());
    mCmd->SetSQLStatement(tmp);
    
    mReader = mCmd->ExecuteReader();
    assert(NULL != mReader);
}

bool PgSpatialTablesReader::ReadNext()
{
    return mReader->ReadNext();
}

void PgSpatialTablesReader::Close()
{
    FDOLOG_MARKER("PgSpatialTablesReader::+Close");
}

///////////////////////////////////////////////////////////////////////////////
// Private operations
///////////////////////////////////////////////////////////////////////////////

void PgSpatialTablesReader::ValidateConnectionState() const
{
    if (FdoConnectionState_Closed == mConn->GetConnectionState())
    {
        FDOLOG_WRITE("Connection is closed or invalid.");

        throw FdoException::Create(NlsMsgGet(MSG_POSTGIS_CONNECTION_INVALID,
            "Connection is closed or invalid."));
    }
}

}} // namespace fdo::postgis
