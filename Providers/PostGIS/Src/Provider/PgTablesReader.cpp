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
#include "PgTablesReader.h"
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

PgTablesReader::PgTablesReader()
{
	assert(false);
}

PgTablesReader::PgTablesReader(Connection* conn)
    : mConn(conn)
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

PgTablesReader::~PgTablesReader()
{
}

///////////////////////////////////////////////////////////////////////////////
// FdoIDisposable interface
///////////////////////////////////////////////////////////////////////////////

void PgTablesReader::Dispose()
{
    // This function might throw, do NOT call from the destructor!
    Close();

    delete this;
}

///////////////////////////////////////////////////////////////////////////////
// PgTablesReader interface
///////////////////////////////////////////////////////////////////////////////

FdoStringP PgTablesReader::GetSchemaName() const
{
    return mReader->GetString(L"schemaname");
}

FdoStringP PgTablesReader::GetTableName() const
{
    return mReader->GetString(L"tablename");
}

    // check if the table has a spatial column
bool PgTablesReader::CheckSpatialTable() const
{
    FDOLOG_MARKER("PgTablesReader::+CheckSpatialTable");

    assert(!mCurrentSchema.empty() && !mTableCached.empty());

    std::string sql("SELECT n.nspname AS schemaname, c.relname AS tablename"
                    " FROM pg_class c, pg_namespace n, geometry_columns g"
                    " WHERE c.relkind IN ('r','v') AND c.relname !~ '^(pg_|sql_)'"
                    " AND c.relnamespace = n.oid AND n.nspname = g.f_table_schema"
                    " AND c.relname::TEXT = g.f_table_name::TEXT"
                    " AND n.nspname = '" + mCurrentSchema + "'"
                    " AND c.relname = '" + mTableCached + "'"
                    " GROUP BY schemaname, tablename");
    //BUG ! utiliser QUE des varriable locale
    // Query spatial tables and attach results to the SQL data reader
/*--  FdoPtr<FdoISQLCommand> Cmd = static_cast<FdoISQLCommand*>(mConn->CreateCommand(FdoCommandType_SQLCommand));
    assert(NULL != Cmd);
    
    FdoStringP tmp(sql.c_str());
    Cmd->SetSQLStatement(tmp);
    
    FdoPtr<FdoISQLDataReader> reader = Cmd->ExecuteReader();
    assert(NULL != reader);

    bool hasTuples = reader->ReadNext();
//  if (NULL != reader) reader->Close();  --*/        

    boost::shared_ptr<PGresult> pgRes(mConn->PgExecuteQuery(sql.c_str()), PQclear);
    assert(PGRES_TUPLES_OK == PQresultStatus(pgRes.get()));
    int nb = PQntuples(pgRes.get());
    bool hasTuples = (nb > 0);

    return hasTuples;
}

PgTablesReader::columns_t PgTablesReader::GetGeometryColumns() const
{
    FDOLOG_MARKER("PgTablesReader::+GetGeometryColumns");

    columns_t columns;

    if (!mTableSpatialCached) {
      FDOLOG_WRITE("PgTablesReader::-SelectColumnExtent '%s' is not a geometrie table!", mTableCached);
      return columns;
    }
/* 2008/02
    std::string sql("SELECT g.f_geometry_column, g.type, g.coord_dimension, g.srid "
                    "FROM geometry_columns g "
                    "WHERE  g.f_table_schema = '"
                    + mCurrentSchema +
                    "' AND g.f_table_name = '"
                    + mTableCached + "'");
-*/
    // because geometry_columns table is out of date after a 'DROP TABLE...' !
    std::string sql("SELECT g.f_geometry_column, g.type, g.coord_dimension, g.srid"
	                  " FROM geometry_columns g, pg_attribute a, pg_class c, pg_type t, pg_namespace n"
                    " WHERE  g.f_table_schema = '" + mCurrentSchema + "'"
                    " AND g.f_table_name = '" + mTableCached + "'"
                  	" AND g.f_geometry_column = a.attname "
	                  " AND a.attnum > 0 "
	                  " AND a.attrelid = c.oid "
	                  " AND a.atttypid = t.oid "
	                  " AND c.relnamespace = n.oid "
	                  " AND c.relname = g.f_table_name "
	                  " AND n.nspname = g.f_table_schema "
	                  " ORDER BY a.attnum");

    // Here, we intentionally DO NOT use FdoISQLCommand to eliminate
    // unnecessary overhead. It's faster to work as close to libpq API as possible.

    // TODO: Add additional level of try-catch here

    boost::shared_ptr<PGresult> pgRes(mConn->PgExecuteQuery(sql.c_str()), PQclear);
    assert(PGRES_TUPLES_OK == PQresultStatus(pgRes.get()));
    assert(4 == PQnfields(pgRes.get()));

    int const ntuples = PQntuples(pgRes.get());

    // Prepare collection for geometry columns
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
            FdoPtr<FdoEnvelopeImpl> bbox;
            if(IsEstimateColumnExtentAvailable(static_cast<char const*>(name)))
            {
              bbox = EstimateColumnExtent(static_cast<char const*>(name));
            }
            else
            {
              bbox = SelectColumnExtent(static_cast<char const*>(name));
            }
            if (bbox->GetIsEmpty()) 
            {
              bbox = FdoEnvelopeImpl::Create(0.0, 0.0, 0.0, 0.0);
            }

            // Describe geometry column and add to the collection
            PgGeometryColumn::Ptr col(new PgGeometryColumn(name, type, dim, srid, bbox));
            columns.push_back(col);
        }

        FDOLOG_WRITE(" - Number of described columns: %d", columns.size());
    }
    catch (boost::bad_lexical_cast& e)
    {
        FDOLOG_WRITE("Field value conversion failed: %s", e.what());
        throw FdoException::Create(L"Error occured while reading schema of geometry column");
    }

    assert(ntuples == columns.size());
    return columns;
}

void PgTablesReader::Open()
{
    FDOLOG_MARKER("PgTablesReader::+Open");

    assert(!mCurrentSchema.empty());

    std::string sql("SELECT n.nspname AS schemaname,c.relname AS tablename"
                    " FROM pg_class c, pg_namespace n"
                    " WHERE c.relnamespace = n.oid"
                    " AND c.relkind IN ('r','v')"
                    " AND (c.relname NOT LIKE 'pg_%')"
				            " AND (c.relname NOT LIKE 'spatial_ref_sys%')"
				            " AND (c.relname NOT LIKE 'sql_%')"
				            " AND (c.relname NOT LIKE 'geom%')"
                    " AND n.nspname = '" + mCurrentSchema + "'"
				            " ORDER BY c.relname");
    
    // Query spatial tables and attach results to the SQL data reader
    mCmd = static_cast<FdoISQLCommand*>(mConn->CreateCommand(FdoCommandType_SQLCommand));
    assert(NULL != mCmd);
    
    FdoStringP tmp(sql.c_str());
    mCmd->SetSQLStatement(tmp);
    
    mReader = mCmd->ExecuteReader();
    assert(NULL != mReader);
}

bool PgTablesReader::ReadNext()
{
    bool hasTuples = mReader->ReadNext();
    
    // Cache table name
    if (hasTuples)
    {
        mTableCached = static_cast<char const*>(GetTableName());
        mTableSpatialCached = CheckSpatialTable();
    }
    
    return hasTuples;
}

void PgTablesReader::Close()
{
    if (NULL != mReader)
        mReader->Close();        
}

///////////////////////////////////////////////////////////////////////////////
// Private operations
///////////////////////////////////////////////////////////////////////////////

void PgTablesReader::ValidateConnectionState() const
{
    if (FdoConnectionState_Closed == mConn->GetConnectionState())
    {
        FDOLOG_WRITE("Connection is closed or invalid.");

        throw FdoException::Create(NlsMsgGet(MSG_POSTGIS_CONNECTION_INVALID,
            "Connection is closed or invalid."));
    }
}

bool PgTablesReader::IsEstimateColumnExtentAvailable(
    std::string const& column) const
{
    FDOLOG_MARKER("PgTablesReader::-IsEstimateColumnExtentAvailable");
    assert(!mCurrentSchema.empty() && !mTableCached.empty());

    if (!mTableSpatialCached) {
      return false;
    }

    std::string sql("SELECT count(*) FROM pg_statistic s, pg_class c, pg_attribute a, pg_namespace n WHERE c.relname = '"
                    + mTableCached + "' AND a.attrelid = c.oid AND a.attname = '"
                    + column + "' AND n.nspname = '"
                    + mCurrentSchema + "' AND c.relnamespace = n.oid AND s.starelid=c.oid AND s.staattnum = a.attnum AND staattnum = attnum");
    try
    {
        boost::shared_ptr<PGresult> pgRes(mConn->PgExecuteQuery(sql.c_str()), PQclear);
        assert(PGRES_TUPLES_OK == PQresultStatus(pgRes.get()));
        assert(1 == PQntuples(pgRes.get()));

        bool    nullVal = false;
        char const* cval = NULL;
        cval = PQgetvalue(pgRes.get(), 0, 0);
        if (!cval || !strlen(cval)) return false;
        int nbStats = StringConv<int>(cval);
        if(nbStats > 0) return true;
        return false;
    }
    catch (FdoException* e)
    {
      FDOLOG_WRITE(L"Warning! estimated_extent is empty! use VACUUM or ANALYZE");
      e->Release();
      return false;
    }
    catch (boost::bad_lexical_cast& e)
    {
      FDOLOG_WRITE("Extent coordinate value conversion failed: %s", e.what());
      throw FdoException::Create(L"Error occured while reading coordinate of estimated extent");
    }
}

FdoPtr<FdoEnvelopeImpl> PgTablesReader::EstimateColumnExtent(
    std::string const& column) const
{
    FDOLOG_MARKER("PgTablesReader::-EstimateColumnExtent");
    assert(!mCurrentSchema.empty() && !mTableCached.empty());

    if (!mTableSpatialCached) {
      FDOLOG_WRITE("PgTablesReader::-SelectColumnExtent '%s' is not a geometrie table!", mTableCached );
      FdoPtr<FdoEnvelopeImpl> extent;
      FDO_SAFE_ADDREF(extent.p);
      return extent.p;
    }

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

    try
    {
        boost::shared_ptr<PGresult> pgRes(mConn->PgExecuteQuery(sql.c_str()), PQclear);
        assert(PGRES_TUPLES_OK == PQresultStatus(pgRes.get()));
        assert(1 == PQntuples(pgRes.get()));

        bool    nullVal = false;
        char const* cval = NULL;
        
        // X MIN
        cval = PQgetvalue(pgRes.get(), 0, 0);
        if (!cval || !strlen(cval)) nullVal = true;
        double xmin = StringConv<double>(cval);
        // Y MIN
        cval = PQgetvalue(pgRes.get(), 0, 1);
        if (!cval || !strlen(cval)) nullVal = true;
        double ymin = StringConv<double>(cval);
        // X MAX
        cval = PQgetvalue(pgRes.get(), 0, 2);
        if (!cval || !strlen(cval)) nullVal = true;
        double xmax = StringConv<double>(cval);
        // Y MAX
        cval = PQgetvalue(pgRes.get(), 0, 3);
        if (!cval || !strlen(cval)) nullVal = true;
        double ymax = StringConv<double>(cval);

        // Build spatial envelope object
        FdoPtr<FdoEnvelopeImpl> extent;
        if (!nullVal) 
        {
          extent = FdoEnvelopeImpl::Create(xmin, ymin, xmax, ymax);
          FDOLOG_WRITE("Extent:\n\txmin = %.8f\n\tymin = %.8f\n\txmax = %.8f\n\tymax = %.8f",
              xmin, ymin, xmax, ymax);
        } 
        else 
        {
          extent = FdoEnvelopeImpl::Create(); // create empty envelope!
          FDOLOG_WRITE("Warning! estimated_extent is empty! use VACUUM or ANALYZE");
        }

        FDO_SAFE_ADDREF(extent.p);
        return extent.p;
    }
    catch (FdoException* e)
    {
      FdoPtr<FdoEnvelopeImpl> extent;
      extent = FdoEnvelopeImpl::Create(); // create empty envelope!
      FDOLOG_WRITE(L"Warning! estimated_extent is empty! use VACUUM or ANALYZE");
      //throw FdoException::Create(L"Error occured while reading coordinate of estimated extent");
      e->Release();
      FDO_SAFE_ADDREF(extent.p);
      return extent.p;
    }
    catch (boost::bad_lexical_cast& e)
    {
        FDOLOG_WRITE("Extent coordinate value conversion failed: %s", e.what());
        throw FdoException::Create(L"Error occured while reading coordinate of estimated extent");
    }
}

FdoPtr<FdoEnvelopeImpl> PgTablesReader::SelectColumnExtent(
    std::string const& column) const
{
    FDOLOG_MARKER("PgTablesReader::-SelectColumnExtent");
    assert(!mCurrentSchema.empty() && !mTableCached.empty());

    if (!mTableSpatialCached) {
      FDOLOG_WRITE("PgTablesReader::-SelectColumnExtent '%s' is not a geometrie table!", mTableCached );
      FdoPtr<FdoEnvelopeImpl> extent;
      FDO_SAFE_ADDREF(extent.p);
      return extent.p;
    }

    FDOLOG_WRITE("Geometry column: %s.%s.%s", mCurrentSchema.c_str(), 
        mTableCached.c_str(), column.c_str());

    // Query estimating spatial extent for given geometry column using table statistics.
    // For PostgreSQL>=8.0.0 statistics are gathered by VACUUM ANALYZE and resulting
    // extent will be about 95% of the real one.

    std::string sql("SELECT xmin(box.extent), xmin(box.extent), xmax(box.extent), ymax(box.extent) "
                     "FROM ( SELECT extent(" + column + ") FROM "
                     + mCurrentSchema + "."+ mTableCached // TODO add mBaseName
                     + ") AS box");

    try
    {
        boost::shared_ptr<PGresult> pgRes(mConn->PgExecuteQuery(sql.c_str()), PQclear);
        assert(PGRES_TUPLES_OK == PQresultStatus(pgRes.get()));
        assert(1 == PQntuples(pgRes.get()));

        bool    nullVal = false;
        char const* cval = NULL;
        
        // X MIN
        cval = PQgetvalue(pgRes.get(), 0, 0);
        if (!cval || !strlen(cval)) nullVal = true;
        double xmin = StringConv<double>(cval);
        // Y MIN
        cval = PQgetvalue(pgRes.get(), 0, 1);
        if (!cval || !strlen(cval)) nullVal = true;
        double ymin = StringConv<double>(cval);
        // X MAX
        cval = PQgetvalue(pgRes.get(), 0, 2);
        if (!cval || !strlen(cval)) nullVal = true;
        double xmax = StringConv<double>(cval);
        // Y MAX
        cval = PQgetvalue(pgRes.get(), 0, 3);
        if (!cval || !strlen(cval)) nullVal = true;
        double ymax = StringConv<double>(cval);

        // Build spatial envelope object
        FdoPtr<FdoEnvelopeImpl> extent;
        if (!nullVal)
        {
          FDOLOG_WRITE("Extent:\n\txmin = %.8f\n\tymin = %.8f\n\txmax = %.8f\n\tymax = %.8f",
            xmin, ymin, xmax, ymax);
          extent = FdoEnvelopeImpl::Create(xmin, ymin, xmax, ymax);
        } else  {
          extent = FdoEnvelopeImpl::Create(); // create empty envelope!
          FDOLOG_WRITE("Warning! No Extent selectd!");
        }

        FDO_SAFE_ADDREF(extent.p);
        return extent.p;
    }
    catch (FdoException* e)
    {
      FdoPtr<FdoEnvelopeImpl> extent;
      extent = FdoEnvelopeImpl::Create(); // create empty envelope!
      FDOLOG_WRITE(L"Warning! SelectColumnExtent failed, table must be empty");
      FDO_SAFE_ADDREF(extent.p);
      e->Release();
      return extent.p;
    }
    catch (boost::bad_lexical_cast& e)
    {
      FDOLOG_WRITE("Extent coordinate value conversion failed: %s", e.what());
      throw FdoException::Create(L"Error occured while select coordinate of extent");
    }
}

}} // namespace fdo::postgis
