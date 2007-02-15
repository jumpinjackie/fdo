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
#include "ListDataStores.h"
#include "Connection.h"

#include <cassert>

namespace fdo { namespace postgis {

//ListDataStores::ListDataStores() : Base(NULL)
//{
//}

ListDataStores::ListDataStores(Connection* conn) : Base(conn)
{
}

ListDataStores::~ListDataStores()
{
}

bool ListDataStores::GetIncludeNonFdoEnabledDatastores()
{
    return mIncludeNonFdo;
}

void ListDataStores::SetIncludeNonFdoEnabledDatastores(bool includeNonFdo)
{
    mIncludeNonFdo = includeNonFdo;
}

FdoIDataStoreReader* ListDataStores::Execute()
{
    
    FdoSize cmdTuples = 0;
    ExecStatusType pgStatus = PGRES_FATAL_ERROR;
    pgStatus = mConn->PgExecuteCommand("BEGIN work", cmdTuples);

    std::string cursor("crsFdoListDatastore");
    std::string qry("SELECT ns.nspname AS schemaname, r.rolname AS ownername "
                    "FROM pg_catalog.pg_namespace ns "
                    "JOIN pg_catalog.pg_roles r ON ns.nspowner=r.oid"
                    "WHERE ns.nspname !~ \'^pg_\' AND ns.nspname != \'information_schema\'"
                    "ORDER BY 1");

    
    std::string sql("DECLARE " + cursor + " CURSOR FOR ");
    sql += qry;
    
    pgStatus = mConn->PgExecuteCommand(sql.c_str(), cmdTuples);


    return NULL;
}

}} // namespace fdo::postgis
