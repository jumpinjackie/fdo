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
#include "Table.h"
#include "Mgr.h"

FdoSmPhGrdIndex::FdoSmPhGrdIndex(
        FdoStringP name,
        FdoSmPhDbObject* pParent,
        bool isUnique,
        FdoSchemaElementState elementState
) :
    FdoSmPhIndex( name, pParent, isUnique, elementState ),
    FdoSmPhDbObject( name, (const FdoSmPhOwner*) pParent->GetParent() )
{
}

FdoSmPhGrdIndex::~FdoSmPhGrdIndex(void)
{
}

bool FdoSmPhGrdIndex::Add()
{
    FdoSmPhDbObjectP dbObject = GetDbObject();

    FdoStringP sqlStmt = FdoStringP::Format(
        L"create %lsindex %ls on %ls ( %ls )",
        GetIsUnique() ? L"unique " : L"",
        (FdoString*) GetDDLQName(),
        (FdoString*) dbObject->GetDDLQName(),
        (FdoString*) GetKeyColsSql(GetColumns())->ToString( L", " )
    );

    dbObject->ExecuteDDL( sqlStmt );

    return true;
}

bool FdoSmPhGrdIndex::Modify()
{
    // Dropping and re-creating the index is the simplest way to handle 
    // modifications.
    Delete();
    SetElementState( FdoSchemaElementState_Added );
    return false;
}

bool FdoSmPhGrdIndex::Delete()
{
    FdoSmPhDbObjectP dbObject = GetDbObject();

    FdoStringP sqlStmt = FdoStringP::Format(
        L"drop index %ls",
        (FdoString*) GetDDLQName()
    );

    dbObject->ExecuteDDL( sqlStmt );

    return true;
}

