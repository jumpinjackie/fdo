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
#include "Transaction.h"
#include "Connection.h"
// std
#include <cassert>

namespace fdo { namespace postgis {

Transaction::Transaction(Connection* conn) : mConn(conn)
{
  assert(NULL != mConn);
  FDO_SAFE_ADDREF(mConn.p);
}

Transaction::~Transaction()
{
}

///////////////////////////////////////////////////////////////////////////////
// FdoIDisposable interface
///////////////////////////////////////////////////////////////////////////////

void Transaction::Dispose()
{
    delete this;
}

///////////////////////////////////////////////////////////////////////////////
// FdoITransaction interface
///////////////////////////////////////////////////////////////////////////////

FdoIConnection* Transaction::GetConnection()
{
    FDO_SAFE_ADDREF(mConn.p);
    return mConn.p;
}

void Transaction::Commit()
{
    FDOLOG_MARKER("Transaction::+Commit");
    assert(NULL != mConn);


    // TODO: Test it carefully
    mConn->PgCommitSoftTransaction();
}

void Transaction::Rollback()
{
    FDOLOG_MARKER("Transaction::+Rollback");
    assert(NULL != mConn);

    // TODO: Test it carefully
    mConn->PgRollbackSoftTransaction();
}

}} // namespace fdo::postgis
