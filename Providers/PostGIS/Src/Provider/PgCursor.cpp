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

namespace fdo { namespace postgis {


PgCursor::PgCursor(Connection* conn, std::string const& name)
    : mConn(conn), mName(name), mPgResult(NULL)
{
    FDO_SAFE_ADDREF(mConn.p);
    Validate();
}

PgCursor::~PgCursor()
{
    Close();
}

void PgCursor::Declare(std::string const& query)
{
    Declare(query.c_str());
}

void PgCursor::Declare(char const* query)
{

}

void PgCursor::Close()
{

}

///////////////////////////////////////////////////////////////////////////////
// Private operations
///////////////////////////////////////////////////////////////////////////////

void PgCursor::Validate()
{
    if (FdoConnectionState_Open != mConn->GetConnectionState())
    {
        throw FdoException::Create(NlsMsgGet(MSG_POSTGIS_CONNECTION_INVALID,
            "Connection is invalid."));
    }
}

}} // namespace fdo::postgis
