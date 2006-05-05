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
#include "Index.h"
#include "Mgr.h"

FdoSmPhMySqlIndex::FdoSmPhMySqlIndex(
        FdoStringP name,
        const FdoSmPhTable* pTable,
        bool isUnique,
        FdoSchemaElementState elementState,
        FdoSmPhRdDbObjectReader* reader
) :
    FdoSmPhGrdIndex( name, pTable, isUnique, elementState ),
    FdoSmPhMySqlDbObject( name, (const FdoSmPhOwner*) pTable->GetParent(), reader ),
    FdoSmPhDbObject( name, (const FdoSmPhOwner*) pTable->GetParent(), elementState )
{
}

FdoSmPhMySqlIndex::~FdoSmPhMySqlIndex(void)
{
}

FdoPtr<FdoSmPhRdColumnReader> FdoSmPhMySqlIndex::CreateColumnReader()
{
    return FdoSmPhIndex::CreateColumnReader();
}

bool FdoSmPhMySqlIndex::Add()
{
    const FdoSmPhTable* table = RefTable();

    FdoStringP sqlStmt = FdoStringP::Format(
        L"create %lsindex %ls on %ls ( %ls )",
        GetIsUnique() ? L"unique " : L"",
        (FdoString*) GetName(),
        (FdoString*) table->GetDbQName(),
        (FdoString*) GetRefColsSql()->ToString( L", " )
    );

    // MySql does not allow qualified name for index to create.
    // Therefore, must switch to the index's owning database before creating
    // it.
    ActivateOwnerAndExecute( sqlStmt );

    return true;
}

bool FdoSmPhMySqlIndex::Delete()
{
    FdoSmPhMySqlMgrP      mgr = GetManager()->SmartCast<FdoSmPhMySqlMgr>();
    GdbiConnection*     gdbiConn = mgr->GetGdbiConnection();
    const FdoSmPhTable* table = RefTable();

    FdoStringP sqlStmt = FdoStringP::Format(
        L"alter table %ls drop index %ls",
        (FdoString*) table->GetDbQName(),
        (FdoString*) GetName()
    );

    gdbiConn->ExecuteNonQuery( (const char*) sqlStmt, true );

    return true;
}

