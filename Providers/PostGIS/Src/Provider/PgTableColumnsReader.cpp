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
#include "PgTableColumnsReader.h"
#include "PgUtility.h"
#include "Connection.h"
// std
#include <cassert>
#include <string>
// boost
#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>

namespace fdo { namespace postgis {

PgTableColumnsReader::PgTableColumnsReader(Connection* conn,
                                           FdoStringP schemaName,
                                           FdoStringP tableName)
    : mConn(conn), mReader(NULL), mCmd(NULL), mSchema(schemaName), mTable(tableName)
{
    FDO_SAFE_ADDREF(mConn.p);

    ValidateConnectionState();
}

PgTableColumnsReader::~PgTableColumnsReader()
{
}

///////////////////////////////////////////////////////////////////////////////
// FdoIDisposable interface
///////////////////////////////////////////////////////////////////////////////

void PgTableColumnsReader::Dispose()
{
    //FDOLOG_MARKER("PgTableColumnsReader::#Dispose");

    // This function might throw, do NOT call from the destructor!
    Close();

    delete this;
}

///////////////////////////////////////////////////////////////////////////////
// PgTableColumnsReader interface
///////////////////////////////////////////////////////////////////////////////

FdoStringP PgTableColumnsReader::GetSchemaName() const
{
    return mSchema;
}

FdoStringP PgTableColumnsReader::GetTableName() const
{
    return mTable;
}

FdoStringP PgTableColumnsReader::GetColumnName() const
{
    return mReader->GetString(L"column_name");
}

FdoStringP PgTableColumnsReader::GetColumnDescription() const
{
    return L"";
}

FdoInt32 PgTableColumnsReader::GetColumnPosition() const
{
    return mReader->GetInt32(L"ordinal_position");
}

FdoDataType PgTableColumnsReader::GetColumnType() const
{
    FdoStringP pgTypeName(mReader->GetString(L"data_type"));
    return details::FdoTypeFromPgTypeName(static_cast<char const*>(pgTypeName));
}

FdoInt32 PgTableColumnsReader::GetColumnSize() const
{
    FdoInt32 maxSize = 0;
    if (FdoDataType_String == GetColumnType())
    {
        // TODO: Check this Arbitrarily taken max size of TEXT with unspecified length
        maxSize = 65536;
        
        int const fmod = mReader->GetInt32(L"modifier");
        if (-1 != fmod)
        {
            maxSize = details::GetTypeMaxSize(fmod);
        }
        assert(-1 != maxSize && "MAX LENGHT NOT SPECIFIED");
    }
    else
    {
        maxSize = mReader->GetInt32(L"character_maximum_length");
    }
    return maxSize;
}

FdoInt32 PgTableColumnsReader::GetColumnPrecision() const
{
    FdoInt32 precision = 0;
    if (FdoDataType_Decimal == GetColumnType())
    {
        int const fmod = mReader->GetInt32(L"modifier");
        return details::GetTypePrecision(fmod);
    }    
    return precision;
}

FdoInt32 PgTableColumnsReader::GetColumnScale() const
{
    FdoInt32 scale = 0;
    if (FdoDataType_Decimal == GetColumnType())
    {
        int const fmod = mReader->GetInt32(L"modifier");
        return details::GetTypeScale(fmod);
    }    
    return scale;
}

bool PgTableColumnsReader::GetColumnNullability() const
{
    bool isNullable = true;

    // Check if a column can NOT be NULL.
    FdoStringP const notNull(mReader->GetString(L"notnull"));
    if (notNull == L"t")
    {   
        isNullable = false;
    }

    return isNullable;
}

void PgTableColumnsReader::Open()
{
    FDOLOG_MARKER("PgTableColumnsReader::+Open");

    assert(mSchema.GetLength() > 0);
    assert(mTable.GetLength() > 0);

    std::string schema(static_cast<char const*>(mSchema));
    std::string table(static_cast<char const*>(mTable));
    std::string sql(
        "SELECT a.attnum AS ordinal_position,a.attname AS column_name,"
        "t.typname AS data_type,a.attlen AS character_maximum_length,"
        "a.atttypmod AS modifier,a.attnotnull AS notnull,a.atthasdef AS hasdefault "
        "FROM pg_class c, pg_attribute a, pg_type t, pg_namespace n "
        "WHERE a.attnum > 0 AND a.attrelid = c.oid "
        "AND a.atttypid = t.oid AND c.relnamespace = n.oid AND t.typname !~ '^geom' "
        "AND c.relname = '" + table + "' AND n.nspname = '" + schema + "' ORDER BY a.attnum;");
        
    // Query schema details of given table and attach results to the SQL data reader
    mCmd = static_cast<FdoISQLCommand*>(mConn->CreateCommand(FdoCommandType_SQLCommand));
    assert(NULL != mCmd);

    FdoStringP tmp(sql.c_str());
    mCmd->SetSQLStatement(tmp);

    mReader = mCmd->ExecuteReader();
    assert(NULL != mReader);
}

bool PgTableColumnsReader::ReadNext()
{
    bool hasTuples = mReader->ReadNext();
    return hasTuples;
}

void PgTableColumnsReader::Close()
{
    FDOLOG_MARKER("PgTableColumnsReader::+Close");

    if (NULL != mReader)
        mReader->Close();        
}

///////////////////////////////////////////////////////////////////////////////
// Private operations
///////////////////////////////////////////////////////////////////////////////

void PgTableColumnsReader::ValidateConnectionState() const
{
    if (FdoConnectionState_Closed == mConn->GetConnectionState())
    {
        FDOLOG_WRITE("Connection is closed or invalid.");

        throw FdoException::Create(NlsMsgGet(MSG_POSTGIS_CONNECTION_INVALID,
            "Connection is closed or invalid."));
    }
}

}} // namespace fdo::postgis
