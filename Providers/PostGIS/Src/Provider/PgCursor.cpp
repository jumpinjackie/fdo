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
#include "PgCursor.h"
#include "Connection.h"
// std
#include <cassert>
#include <string>
// boost
#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>

namespace fdo { namespace postgis {

PgCursor::PgCursor(Connection* conn, std::string const& name)
    : mConn(conn), mName(name.c_str()), mDescRes(NULL), mFetchRes(NULL), mIsClosed(true)
{
    FDO_SAFE_ADDREF(mConn.p);

    ValidateConnectionState();

    // Cursor name is a regular SQL identifier and if unquoted it's implicitly
    // converted to lower-case. We need to make it lower-case to avoid problems
    // when using cursor name with libpq functions, ie. PQdescribePortal().
    FdoStringP tmp(mName);
    mName = tmp.Lower();
}

PgCursor::~PgCursor()
{
    assert(mIsClosed);
    assert(NULL == mDescRes);
    assert(NULL == mFetchRes);
}

///////////////////////////////////////////////////////////////////////////////
// FdoIDisposable interface
///////////////////////////////////////////////////////////////////////////////

void PgCursor::Dispose()
{
    // WARNING! This may throw, do NOT call from the destructor!
    Close();

    delete this;
}

///////////////////////////////////////////////////////////////////////////////
// PgCursor interface
///////////////////////////////////////////////////////////////////////////////

const char* PgCursor::GetName() const
{
    return (static_cast<char const*>(mName));
}

PgCursor::ResultPtr PgCursor::GetFetchResult() const
{
    ValidateFetchedState();
 
    return mFetchRes;
}

FdoSize PgCursor::GetTuplesCount() const
{
    ValidateDeclaredState();

    int ntuples = 0;
    if (NULL != mFetchRes && PGRES_TUPLES_OK == PQresultStatus(mFetchRes))
    {
        ntuples = PQntuples(mFetchRes);
    }

    return static_cast<FdoSize>(ntuples);
}

FdoSize PgCursor::GetFieldsCount() const
{
    ValidateDeclaredState();
    assert(NULL != mDescRes);

    int nfields = PQnfields(mDescRes);
    return static_cast<FdoSize>(nfields);
}

FdoStringP PgCursor::GetFieldName(FdoSize number) const
{
    ValidateDeclaredState();
    assert(NULL != mDescRes);

    if (GetFieldsCount() <= number)
    {
        throw FdoException::Create(NlsMsgGet(MSG_POSTGIS_COLUMN_INDEX_OUT_OF_RANGE,
            "The given column index '%1$d' is out of range (%2$d).",
            number, GetFieldsCount()));
    }

    char const* fname = PQfname(mDescRes, static_cast<int>(number));
    assert(NULL != fname);

    return FdoStringP(fname);
}

FdoSize PgCursor::GetFieldNumber(FdoStringP const& name) const
{
    ValidateDeclaredState();
    assert(NULL != mDescRes);

    int fnumber = PQfnumber(mDescRes, static_cast<char const*>(name));
    if (-1 == fnumber)
    {
        throw FdoException::Create(NlsMsgGet(MSG_POSTGIS_COLUMN_NAME_NOT_FOUND,
            "The column with given name '%1$ls' is not available in query result.",
            static_cast<FdoString*>(name)));
    }

    return fnumber;
}

FdoDataType PgCursor::GetFieldType(FdoStringP const& name) const
{
    FdoSize const fnumber = GetFieldNumber(name);
    return GetFieldType(fnumber);
}

FdoDataType PgCursor::GetFieldType(FdoSize number) const
{
    ValidateDeclaredState();
    assert(NULL != mDescRes);
    
    // TODO: Currently it's a simple mapping of hardcoded OIDs to FDO data types.
    //       In future, consider replacing it with SELECT * FROM pg_type; solution

    // NOTE: The following list of OIDs was taken from the pg_type table.
    //       We do not claim that this list is exhaustive, correct or even up to date.

    Oid const ftype = PQftype(mDescRes, static_cast<int>(number));

    FdoDataType fdoType = FdoDataType_String; // dummy initialization

    switch (ftype)
    {
    case 16:   // bool
        fdoType = FdoDataType_Boolean;
        break;
    case 21:   // int2
        fdoType = FdoDataType_Int16;
        break;
    case 23:   // int4
        fdoType = FdoDataType_Int32;
        break;
    case 20:   // int8
        fdoType = FdoDataType_Int64;
        break;
    case 700:  // float4, real
        fdoType = FdoDataType_Single;
        break;
    case 701:  // float8, double precision
        fdoType = FdoDataType_Double;
        break;
    case 1700: // numeric
        fdoType = FdoDataType_Decimal;
        break;
    case 18:   // char
    case 25:   // text
    case 1042: // bpchar
    case 1043: // varchar
    case 2275: // cstring
        fdoType = FdoDataType_String;
        break;
    case 702:  // abstime
    case 703:  // reltime
    case 1082: // date
    case 1083: // time
    case 1114: // timestamp
    case 1184: // timestamptz
    case 1266: // timetz
        fdoType = FdoDataType_DateTime;
        break;
    case 17:   // bytea
        FDOLOG_WRITE(" PgCursor::GetFieldType - BYTEA TYPE NOT SUPPORTED");
        assert(!"BYTEA TYPE NOT SUPPORTED");
        fdoType = FdoDataType_BLOB;
        break;
    case 26:   // oid
        FDOLOG_WRITE(" PgCursor::GetFieldType - OID TYPE IS NOT WELL TESTED");
        fdoType = FdoDataType_Int32;
        break;
    default:
        {
            // TODO: What about mapping of FdoDataType_Byte and FdoDataType_CLOB

            FdoStringP unknown(PQfname(mDescRes, static_cast<int>(number)));

            FDOLOG_WRITE("The column '%' (%d) is of unknown type",
                static_cast<FdoString*>(unknown), number);

            throw FdoException::Create(NlsMsgGet(MSG_POSTGIS_COLUMN_TYPE_UNKNOWN,
                "The type of column '%1$s' of number %2$d is unknown.",
                static_cast<FdoString*>(unknown), number));
        }
    }

    return fdoType;
}

bool PgCursor::IsFieldGeometryType(FdoSize number) const
{
    ///////////////////////////////////////////////////////////////////////////
    // TODO: Move the geometry OID retrival somewhere near Connection::Open
    //       and buffer the OID value then.
    //       This way it can be: FASTER and a kind of VALIDATION of PostGIS support
    ///////////////////////////////////////////////////////////////////////////

    ValidateDeclaredState();
    assert(NULL != mDescRes);

    char const* sql = "SELECT oid FROM pg_type WHERE typname = 'geometry'";
    boost::shared_ptr<PGresult> pgRes(mConn->PgExecuteQuery(sql), PQclear);
    
    if (PGRES_TUPLES_OK != PQresultStatus(pgRes.get()))
    {
        FDOLOG_WRITE(" *** EMERGENCY *** PostGIS data types are not installed! Run Forest, run!");
        throw FdoException::Create(L"Can not find 'geometry' type. Check if PostGIS engine is available.");
    }

    // TODO: If caught, it likely means the PostGIS support is not installed!
    assert(PGRES_TUPLES_OK == PQresultStatus(pgRes.get()) && 1 == PQntuples(pgRes.get()));
    
    bool isGeometry = false;
    Oid geometryType = 0;
    try
    {
        std::string val(PQgetvalue(pgRes.get(), 0, 0));
        geometryType = boost::lexical_cast<Oid>(val);
    }
    catch (boost::bad_lexical_cast& e)
    {
        geometryType = 0;
        FDOLOG_WRITE("OID type conversion failed: %s", e.what());
    }
    
    Oid const ftype = PQftype(mDescRes, static_cast<int>(number));
    if (ftype == geometryType)
    {
        isGeometry = true;
    }

    return isGeometry;
}

void PgCursor::Declare(char const* query)
{
    details::pgexec_params_t emptyParams(0);
    Declare(query, emptyParams);
}

void PgCursor::Declare(char const* query, details::pgexec_params_t const& params)
{
    FDOLOG_MARKER("PgCursor::+Declare");

    if (!mIsClosed)
    {
        mConn->PgFlushSoftTransaction();
        Close();
    }

    try
    {
        ValidateConnectionState();
        assert(NULL == mDescRes);
        assert(NULL == mFetchRes);

        // Begin transaction
        mConn->PgBeginSoftTransaction();

        // Declare cursor
        std::string sql("DECLARE ");
        sql += static_cast<char const*>(mName);
        sql += " CURSOR FOR ";
        sql += query;

        FdoSize affected = 0; // Not used

        mConn->PgExecuteCommand(sql.c_str(), params, affected);

        // Call for describe cursor results
        Describe();
    }
    catch (FdoException* e)
    {
        FdoException* ne = NULL;
        ne = FdoException::Create(NlsMsgGet(MSG_POSTGIS_CURSOR_CREATION_FAILED,
                "The creation of PostgreSQL cursor '%1$ls' failed.",
                static_cast<FdoString*>(mName)), e);
        e->Release();
        throw ne;
    }

    mIsClosed = false;
}

void PgCursor::Close()
{
    if (!mIsClosed)
    {
        // TODO: Throwing from here seems to be like walking on a thin line!
        // ValidateDeclaredState();
        assert(FdoConnectionState_Closed != mConn->GetConnectionState());

        ClearDescribeResult();
        ClearFetchResult();

        // Close cursor
        std::string sql("CLOSE ");
        sql += static_cast<char const*>(mName);

        mConn->PgExecuteCommand(sql.c_str());
        
        // End transaction
        mConn->PgCommitSoftTransaction();
        
        // Mark cursor as released
        mIsClosed = true;
    }
}

PgCursor::ResultPtr PgCursor::FetchNext()
{
    // TODO: Add try-catch and re-throw with specific message/code

    ValidateDeclaredState();
    ClearFetchResult();
    assert(NULL == mFetchRes);

    std::string sql("FETCH NEXT FROM ");
    sql += static_cast<char const*>(mName);

    mFetchRes = mConn->PgExecuteQuery(sql.c_str());
    return mFetchRes;
}

PgCursor::ResultPtr PgCursor::Fetch(std::size_t count)
{
    ValidateDeclaredState();
    ClearFetchResult();
    assert(NULL == mFetchRes);

    // TODO: Consider moving this logic to on-construction time.
    if (mSqlFetch.empty())
    {
        std::ostringstream os;
        os << "FETCH " << count << " FROM " << static_cast<char const*>(mName);
        mSqlFetch = os.str();
    }

    mFetchRes = mConn->PgExecuteQuery(mSqlFetch.c_str());
    return mFetchRes;
}

///////////////////////////////////////////////////////////////////////////////
// Private operations
///////////////////////////////////////////////////////////////////////////////

void PgCursor::Describe()
{
    ValidateConnectionState();

    if (NULL == mDescRes)
    {
        mDescRes = mConn->PgDescribeCursor(static_cast<char const*>(mName));
    }

    assert(NULL != mDescRes);
}

void PgCursor::ClearDescribeResult()
{
    if (NULL != mDescRes)
    {
        PQclear(mDescRes);
        mDescRes = NULL;
    }
}

void PgCursor::ClearFetchResult()
{
    if (NULL != mFetchRes)
    {
        PQclear(mFetchRes);
        mFetchRes = NULL;
    }
}

void PgCursor::ValidateConnectionState() const
{
    if (FdoConnectionState_Closed == mConn->GetConnectionState())
    {
        FDOLOG_WRITE("Connection is closed or invalid.");

        throw FdoException::Create(NlsMsgGet(MSG_POSTGIS_CONNECTION_INVALID,
            "Connection is closed or invalid."));
    }
}

void PgCursor::ValidateDeclaredState() const
{
    ValidateConnectionState();

    if (mIsClosed || NULL == mDescRes)
    {
        FDOLOG_WRITE("The PostgreSQL database cursor is not defined.");

        throw FdoException::Create(NlsMsgGet(MSG_POSTGIS_CURSOR_NOT_DEFINED,
            "The PostgreSQL database cursor is not defined."));
    }
}

void PgCursor::ValidateFetchedState() const
{
    ValidateDeclaredState();

    if (mIsClosed || NULL == mFetchRes)
    {
        FDOLOG_WRITE("The fetch command was not issued yet.");

        throw FdoException::Create(NlsMsgGet(MSG_POSTGIS_CURSOR_NOT_FETCHED,
            "The fetch command was not issued yet."));
    }
}

}} // namespace fdo::postgis
