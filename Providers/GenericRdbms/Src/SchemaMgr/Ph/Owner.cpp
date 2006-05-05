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
#include "Owner.h"
#include "Mgr.h"

FdoSmPhGrdOwner::FdoSmPhGrdOwner(
    FdoStringP name,
    bool hasMetaSchema,
    const FdoSmPhDatabase* pDatabase,
    FdoSchemaElementState elementState
) :
    FdoSmPhOwner(name, hasMetaSchema, pDatabase, elementState )
{
}

FdoSmPhGrdOwner::~FdoSmPhGrdOwner(void)
{
}

void FdoSmPhGrdOwner::AddMetaSchema( FdoStringsP keywords, bool IsSystem)
{
	FdoSmPhGrdMgrP mgr = GetManager()->SmartCast<FdoSmPhGrdMgr>();
    GdbiConnection* gdbiConn = mgr->GetGdbiConnection();
	if (IsSystem)
		mgr->ExecSchemaFile( L"fdosys_sys.sql", keywords );
	else	{
		mgr->ExecSchemaFile( L"fdo_sys.sql", keywords );
		mgr->ExecSchemaFile( L"fdo_sys_idx.sql", keywords );

        FdoStringP sqlStmt = FdoStringP::Format(
            L"update f_schemainfo set description = '%ls' where upper(schemaname) = upper('%ls')",
            GetDescription(),
            GetName()
        );
  
        gdbiConn->ExecuteNonQuery( (const char*) sqlStmt );
	}
}

void FdoSmPhGrdOwner::ActivateAndExecute( FdoStringP sqlStmt )
{
    FdoSmPhGrdMgrP          mgr             = GetManager()->SmartCast<FdoSmPhGrdMgr>();
    GdbiConnection*         gdbiConn        = mgr->GetGdbiConnection();
    FdoSmPhOwnerP           activeOwner     = mgr->FindOwner();
    bool                    ownerSwitched   = false;

    if ( !activeOwner || (GetQName().ICompare(activeOwner->GetQName()) != 0) ) {
        SetCurrent();
        ownerSwitched = true;
    }

    try {
        gdbiConn->ExecuteNonQuery( (const wchar_t*) sqlStmt, true );
    }
    catch ( ... ) {
        // Switch back to original current schema, if any
        if ( ownerSwitched && activeOwner && (wcslen(activeOwner->GetName()) > 0) ) 
            activeOwner->SetCurrent();
        else
            mgr->GetDatabase()->UnsetCurrentOwner();

        throw;
    }

    // Switch back to original current schema, if any
    if ( ownerSwitched ) {
        // TODO: shouldn't need to check owner name, however have to fix 
        // FdoSmLpDatabase::FindOwner to not create dummy owner when there
        // is no active owner.
        if ( activeOwner && (wcslen(activeOwner->GetName()) > 0) ) 
            activeOwner->SetCurrent();
        else
            mgr->GetDatabase()->UnsetCurrentOwner();
    }
}



