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
#include "Index.h"
#include "Mgr.h"

FdoSmPhPostGisIndex::FdoSmPhPostGisIndex(FdoStringP name,
    FdoSmPhDbObject* pParent,
    bool isUnique,
    FdoSchemaElementState state,
    FdoSmPhRdDbObjectReader* reader)
    : FdoSmPhGrdIndex(name, pParent, isUnique, state),
      FdoSmPhPostGisDbObject(name,
        static_cast<const FdoSmPhOwner*>(pParent->GetParent()), reader),
      FdoSmPhDbObject(name,
        static_cast<const FdoSmPhOwner*>(pParent->GetParent()), state)
{
    // idle
}

FdoSmPhPostGisIndex::~FdoSmPhPostGisIndex()
{
    // idle
}

FdoPtr<FdoSmPhRdColumnReader> FdoSmPhPostGisIndex::CreateColumnReader()
{
    return FdoSmPhIndex::CreateColumnReader();
}

bool FdoSmPhPostGisIndex::Add()
{
    FdoSmPhDbObjectP dbObject = GetDbObject();

    // NOTE on UNIQUE qualifier:
    // Only B-tree indexes can be declared unique.

    FdoStringP sqlStmt = FdoStringP::Format(
        L"CREATE %lsindex %ls ON %ls ( %ls )",
        (GetIsUnique() ? L"UNIQUE " : L""),
        static_cast<FdoString*>(GetDbName()),
        static_cast<FdoString*>(dbObject->GetDbQName()),
        static_cast<FdoString*>(GetKeyColsSql(GetColumns())->ToString(L", ")));

    //
    // TODO: mloskot - Verify the note below for PostgreSQL.
    //

    // PostGIS does not allow qualified name for index to create.
    // Therefore, must switch to the owning database of index
    // before creating it.
    ActivateOwnerAndExecute(sqlStmt);

    return true;
}

bool FdoSmPhPostGisIndex::Delete()
{
    FdoSmPhPostGisMgrP mgr(GetManager()->SmartCast<FdoSmPhPostGisMgr>());

    FdoSmPhDbObjectP    dbObject = GetDbObject();

    // XXX: mloskot - PostGIS uses extended syntax:
    // "ALTER TABLE %ls drop index %ls", table->GetDbQName(), GetDbName()

    // TODO: mloskot - What about CASCADE,RESTRICT modes?

    FdoStringP sqlStmt = FdoStringP::Format(
        L"DROP INDEX IF EXISTS %ls",
        static_cast<FdoString*>(GetDbName()));

    dbObject->ExecuteDDL( (const char*) sqlStmt, (FdoSmPhDbObject*) NULL, true );

    return true;
}

