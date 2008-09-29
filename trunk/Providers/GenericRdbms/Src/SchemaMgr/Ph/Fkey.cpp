/*
 * Copyright (C) 2004-2006  Autodesk, Inc.
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of version 2.1 of the GNU Lesser
 * General Public License as published by the Free Software Foundation.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 * 
 */

#include "stdafx.h"
#include "Fkey.h"
#include "Table.h"
#include "Mgr.h"
#include <Sm/Ph/Table.h>

FdoSmPhGrdFkey::FdoSmPhGrdFkey(
    FdoStringP name, 
    const FdoSmPhTable* pFkeyTable,
    FdoStringP pkeyTableName,
    FdoStringP pkeyTableOwner,
    FdoSchemaElementState elementState
) : 
    FdoSmPhFkey( name, pFkeyTable, pkeyTableName, pkeyTableOwner, elementState )
{
}

FdoSmPhGrdFkey::~FdoSmPhGrdFkey(void)
{
}

bool FdoSmPhGrdFkey::Add()
{
    FdoSmPhGrdMgrP mgr = GetManager()->SmartCast<FdoSmPhGrdMgr>();
    GdbiConnection* gdbiConn = mgr->GetGdbiConnection();

    FdoSmPhGrdTable* table = dynamic_cast<FdoSmPhGrdTable*>( (FdoSmSchemaElement*) GetParent() );
    FdoSmPhGrdTableP pkeyTable = GetPkeyTable().p->SmartCast<FdoSmPhGrdTable>();

    FdoStringP sqlStmt = FdoStringP::Format(
        L"alter table %ls add %ls",
        (FdoString*) table->GetDDLQName(),
        (FdoString*) GetAddSql()
    );

    table->ExecuteDDL( sqlStmt, pkeyTable );


    return true;
}

bool FdoSmPhGrdFkey::Modify()
{
    Delete();
    SetElementState( FdoSchemaElementState_Added );
    return false;
}

bool FdoSmPhGrdFkey::Delete()
{
    FdoSmPhGrdMgrP mgr = GetManager()->SmartCast<FdoSmPhGrdMgr>();
    GdbiConnection* gdbiConn = mgr->GetGdbiConnection();

    FdoSmPhGrdTable* table = dynamic_cast<FdoSmPhGrdTable*>( (FdoSmSchemaElement*) GetParent() );
    FdoSmPhGrdTableP pkeyTable = GetPkeyTable().p->SmartCast<FdoSmPhGrdTable>();

    FdoStringP sqlStmt = FdoStringP::Format(
        L"alter table %ls drop constraint %ls",
        (FdoString*) table->GetDDLQName(),
        GetName()
    );

    table->ExecuteDDL( sqlStmt, pkeyTable );

    return true;
}

