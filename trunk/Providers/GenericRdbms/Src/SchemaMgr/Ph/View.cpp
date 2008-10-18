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
#include "View.h"
#include "Mgr.h"

FdoSmPhGrdView::FdoSmPhGrdView(
    FdoStringP viewName,
    FdoStringP rootDatabase,
    FdoStringP rootOwner,
    FdoStringP rootObjectName,
    const FdoSmPhOwner* pOwner,
    FdoSchemaElementState elementState
) :
    FdoSmPhView(viewName, rootDatabase, rootOwner, rootObjectName, pOwner, elementState),
    FdoSmPhDbObject(viewName, pOwner, elementState)
{
}

FdoSmPhGrdView::~FdoSmPhGrdView(void)
{
}

bool FdoSmPhGrdView::Add()
{
    FdoSmPhGrdMgrP mgr = GetManager()->SmartCast<FdoSmPhGrdMgr>();
    GdbiConnection* gdbiConn = mgr->GetGdbiConnection();

    FdoStringP sqlStmt = FdoStringP::Format(
        L"create view %ls ( %ls ) as %ls",
        (FdoString*) GetDbQName(),
        (FdoString*) GetAddColsSql()->ToString(),
        (FdoString*) GetAddRootSql()
    );

    gdbiConn->ExecuteNonQuery( (const char*) sqlStmt, true );

    return true;
}

bool FdoSmPhGrdView::Modify()
{   
    
    bool actionComplete = Delete();

    if ( actionComplete )
        actionComplete = Add();

    return actionComplete;
}

bool FdoSmPhGrdView::Delete()
{
    FdoSmPhGrdMgrP mgr = GetManager()->SmartCast<FdoSmPhGrdMgr>();
    GdbiConnection* gdbiConn = mgr->GetGdbiConnection();

    FdoStringP sqlStmt = GetDeleteSql();

    gdbiConn->ExecuteNonQuery( (const char*) sqlStmt, true );

    return true;
}

