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
#include "PgGeometry.h"
#include "PgUtility.h"
#include "Connection.h"
// std
#include <cassert>
#include <cstring>
#include <string>
// boost
#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>

// Private utilities
namespace {

template <typename T>
T StringConv(char const* value)
{
    T ret = T();
    if (NULL != value && std::strlen(value) > 0)
    {
        ret = boost::lexical_cast<T>(value);
    }
    return ret;
}

} // anonymous namespace 

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
        assert(PGRES_TUPLES_OK == PQresultStatus(pgRes.get()));

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
    // This function might throw, do NOT call from the destructor!
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

PgSpatialTablesReader::columns_t PgSpatialTablesReader::GetGeometryColumns() const
{
    FDOLOG_MARKER("PgSpatialTablesReader::+GetGeometryColumns");

    std::string sql("SELECT g.f_geometry_column, g.type, g.coord_dimension, g.srid "
                    "FROM geometry_columns g "
                    "WHERE  g.f_table_schema = '"
                    + mCurrentSchema +
                    "' AND g.f_table_name = '"
                    + mTableCached + "'");

    // Here, we intentionally DO NOT use FdoISQLCommand to eliminate
    // unnecessary overhead. It's faster to work as close to libpq API as possible.

    // TODO: Add additional level of try-catch here

    boost::shared_ptr<PGresult> pgRes(mConn->PgExecuteQuery(sql.c_str()), PQclear);
    assert(PGRES_TUPLES_OK == PQresultStatus(pgRes.get()));
    assert(4 == PQnfields(pgRes.get()));

    int const ntuples = PQntuples(pgRes.get());

    // Prepare collection for geometry columns
    columns_t columns;
    columns.reserve(ntuples);
    
    //
    // Read properties of geometry columns in given table
    //
    FdoStringP name;
    FdoGeometryType type = FdoGeometryType_None;
    FdoInt32 dim = 0;
    FdoInt32 srid = 0;

    try
    {
        for (int ntuple = 0; ntuple < ntuples; ++ntuple)
        {
            // 0 - geometry_columns.f_geometry_name
            name = PQgetvalue(pgRes.get(), ntuple, 0);

            // 1 - geometry_columns.type
            std::string stype(PQgetvalue(pgRes.get(), ntuple, 1));
            type = ewkb::FdoGeometryTypeFromPgType(stype);

            // 2 - geometry_columns.coord_dimension
            char const* cdim = PQgetvalue(pgRes.get(), ntuple, 2);
            FdoInt32 tmp = boost::lexical_cast<FdoInt32>(cdim);
            dim = ewkb::FdoDimensionTypeFromPgType(tmp, stype);

            // 3 - geometry_columns.srid
            char const* csrid = PQgetvalue(pgRes.get(), ntuple, 3);
            srid = boost::lexical_cast<FdoInt32>(csrid);

            // Estimate bounding box of geometries in given column
            FdoPtr<FdoEnvelopeImpl> bbox = NULL;
            bbox = EstimateColumnExtent(static_cast<char const*>(name));

            // Describe geometry column and add to the collection
            PgGeometryColumn::Ptr col(new PgGeometryColumn(name, type, dim, srid, bbox));
            columns.push_back(col);
        }
    }
    catch (boost::bad_lexical_cast& e)
    {
        FDOLOG_WRITE("Field value conversion failed: %s", e.what());
        throw FdoException::Create(L"Error occured while reading schema of geometry column");
    }

    assert(ntuples == columns.size());
    return columns;
}

void PgSpatialTablesReader::Open()
{
    FDOLOG_MARKER("PgSpatialTablesReader::+Open");

    assert(!mCurrentSchema.empty());

    std::string sql("SELECT n.nspname AS schemaname, c.relname AS tablename "
                    "FROM pg_class c, pg_namespace n, geometry_columns g "
                    "WHERE c.relkind = 'r' AND c.relname !~ '^(pg_|sql_)' "
                    "AND c.relnamespace = n.oid AND n.nspname = g.f_table_schema "
                    "AND c.relname::TEXT = g.f_table_name::TEXT "
                    "AND n.nspname = '"
                    + mCurrentSchema +
                    "' GROUP BY schemaname, tablename");
    
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
    bool hasTuples = mReader->ReadNext();
    
    // Cache table name
    if (hasTuples)
    {
        mTableCached = static_cast<char const*>(GetTableName());
    }
    
    return hasTuples;
}

void PgSpatialTablesReader::Close()
{
    if (NULL != mReader)
        mReader->Close();        
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

FdoPtr<FdoEnvelopeImpl> PgSpatialTablesReader::EstimateColumnExtent(
    std::string const& column) const
{
    FDOLOG_MARKER("PgSpatialTablesReader::-EstimateColumnExtent");

    FDOLOG_WRITE("Geometry column: %s.%s.%s", mCurrentSchema.c_str(), 
        mTableCached.c_str(), column.c_str());

    // Query estimating spatial extent for given geometry column using table statistics.
    // For PostgreSQL>=8.0.0 statistics are gathered by VACUUM ANALYZE and resulting
    // extent will be about 95% of the real one.

    std::string sql("SELECT xmin(env), ymin(env), xmax(env), ymax(env) FROM ("
                     " SELECT estimated_extent('"
                     + mCurrentSchema + "', '"
                     + mTableCached + "', '"
                     + column + "') AS env) AS extentsub");

    // NOTE: The PgExecuteQuery throws on error, but if no exception occurs,
    //       valid query result is assumed.

    boost::shared_ptr<PGresult> pgRes(mConn->PgExecuteQuery(sql.c_str()), PQclear);
    assert(PGRES_TUPLES_OK == PQresultStatus(pgRes.get()));
    assert(1 == PQntuples(pgRes.get()));

    try
    {
        char const* cval = NULL;
        
        // X MIN
        cval = PQgetvalue(pgRes.get(), 0, 0);
        double xmin = StringConv<double>(cval);
        // Y MIN
        cval = PQgetvalue(pgRes.get(), 0, 1);
        double ymin = StringConv<double>(cval);
        // X MAX
        cval = PQgetvalue(pgRes.get(), 0, 2);
        double xmax = StringConv<double>(cval);
        // Y MAX
        cval = PQgetvalue(pgRes.get(), 0, 3);
        double ymax = StringConv<double>(cval);

        // Build spatial envelope object
        FdoPtr<FdoEnvelopeImpl> extent = NULL;
        extent = FdoEnvelopeImpl::Create(xmin, ymin, xmax, ymax);

        FDOLOG_WRITE("Extent:\n\txmin = %.8f\n\tymin = %.8f\n\txmax = %.8f\n\tymax = %.8f",
            xmin, ymin, xmax, ymax);

        FDO_SAFE_ADDREF(extent.p);
        return extent.p;
    }
    catch (boost::bad_lexical_cast& e)
    {
        FDOLOG_WRITE("Extent coordinate value conversion failed: %s", e.what());
        throw FdoException::Create(L"Error occured while reading coordinate of estimated extent");
    }
}

}} // namespace fdo::postgis
