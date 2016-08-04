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
        FdoSmPhDbObject* pParent,
        bool isUnique,
        FdoSchemaElementState elementState,
        FdoSmPhRdDbObjectReader* reader
) :
    FdoSmPhGrdIndex( name, pParent, isUnique, elementState ),
    FdoSmPhMySqlDbObject( name, (const FdoSmPhOwner*) pParent->GetParent(), reader ),
    FdoSmPhDbObject( name, (const FdoSmPhOwner*) pParent->GetParent(), elementState )
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
    FdoSmPhDbObjectP dbObject = GetDbObject();

    FdoStringP sqlStmt = FdoStringP::Format(
        L"create %lsindex %ls on %ls ( %ls )",
        GetIsUnique() ? L"unique " : L"",
        (FdoString*) GetDbName(),
        (FdoString*) dbObject->GetDbQName(),
        (FdoString*) GetKeyColsSql(GetColumns())->ToString( L", " )
    );

    // MySql does not allow qualified name for index to create.
    // Therefore, must switch to the index's owning database before creating
    // it.
    ActivateOwnerAndExecute( sqlStmt );

    return true;
}

bool FdoSmPhMySqlIndex::Delete()
{
    FdoSmPhDbObjectP    dbObject = GetDbObject();

    FdoStringP sqlStmt = FdoStringP::Format(
        L"alter table %ls drop index %ls",
        (FdoString*) dbObject->GetDbQName(),
        (FdoString*) GetDbName()
    );

    dbObject->ExecuteDDL( (const char*) sqlStmt, (FdoSmPhDbObject*) NULL, true );

    return true;
}

