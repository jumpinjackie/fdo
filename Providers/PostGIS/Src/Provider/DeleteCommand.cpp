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
#include "DeleteCommand.h"
#include "FeatureCommand.h"
#include "Connection.h"
#include "FilterProcessor.h"
// std
#include <cassert>
#include <string>

namespace fdo { namespace postgis {

DeleteCommand::DeleteCommand(Connection* conn) : FeatureCommand<FdoIDelete>(conn)
{
    assert(NULL != mConn);
}

DeleteCommand::~DeleteCommand()
{
}

///////////////////////////////////////////////////////////////////////////////
// FdoIDelete interface
///////////////////////////////////////////////////////////////////////////////

FdoInt32 DeleteCommand::Execute()
{
    FDOLOG_MARKER("DeleteCommand::+Execute");

    //
    // Collect schema details required to build SQL command
    //
    SchemaDescription::Ptr schemaDesc(SchemaDescription::Create());
    schemaDesc->DescribeSchema(mConn, NULL);

    FdoPtr<FdoIdentifier> classIdentifier(GetFeatureClassName());
    FdoPtr<FdoClassDefinition> classDef(schemaDesc->FindClassDefinition(mClassIdentifier));    
    if (!classDef) 
    {
        throw FdoCommandException::Create(L"[PostGIS] DeleteCommand can not find class definition");
    }

    ov::ClassDefinition::Ptr phClass(schemaDesc->FindClassMapping(mClassIdentifier));
    FdoStringP tablePath(phClass->GetTablePath());

    //
    // Build WHERE clause if only subset of rows is selected
    //
    FilterProcessor::Ptr filterProc(new FilterProcessor());

    std::string sqlWhere;
    if (NULL != mFilter)
    {
        mFilter->Process(filterProc);

        std::string sqlFilter(filterProc->GetFilterStatement());
        if (!sqlFilter.empty())
        {
            sqlWhere = " WHERE " + sqlFilter;
        }
    }

    //
    // Build DELETE command and execute it
    //
    std::string sql("DELETE FROM ");
    sql += static_cast<char const*>(tablePath);
    sql += sqlWhere;
    
    FdoSize affected = 0;
    mConn->PgExecuteCommand(sql.c_str(), affected);

    return static_cast<FdoInt32>(affected);
}

FdoILockConflictReader* DeleteCommand::GetLockConflicts()
{
    FDOLOG_MARKER("DeleteCommand::+GetLockConflicts");
    FDOLOG_WRITE("*** NOT IMPLEMENTED ***");

    return NULL;
}

}} // namespace fdo::postgis
