//
// Copyright (C) 2006 Refractions Research, Inc.
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
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
#include "stdafx.h"
#include "Sm/Ph/Mgr.h"
#include "Fkey.h"
#include "Table.h"

FdoSmPhPostGisFkey::FdoSmPhPostGisFkey(FdoStringP name,
    const FdoSmPhTable* fkeyTable,
    FdoStringP pkeyTableName,
    FdoStringP pkeyTableOwner,
    FdoSchemaElementState state)
    : FdoSmPhGrdFkey(name, fkeyTable, pkeyTableName, pkeyTableOwner, state)
{
    // idle
}

FdoSmPhPostGisFkey::~FdoSmPhPostGisFkey()
{
    // idle
}

FdoSmPhColumnP FdoSmPhPostGisFkey::FindPkeyColumn( FdoSmPhTableP pkTable, FdoStringP columnName )
{
    FdoSmPhColumnP column;
    FdoSmPhPostGisTableP pgPkTable = pkTable->SmartCast<FdoSmPhPostGisTable>();

    // For existing foreign keys, PostgreSQL provider tracks referenced column as 1-base position in referenced table.
    if ( columnName.IsNumber() )
    {
        FdoInt32 columnPosition = columnName.ToLong();
        column = pgPkTable->Position2Column(columnPosition);
    }
    else
    {
        column = pgPkTable->GetColumns()->FindItem(columnName);
    }

    return column;
}

