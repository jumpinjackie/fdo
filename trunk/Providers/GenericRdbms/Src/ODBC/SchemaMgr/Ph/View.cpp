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
#include "Owner.h"
#include "Mgr.h"

FdoSmPhOdbcView::FdoSmPhOdbcView(
    FdoStringP viewName,
    FdoStringP rootDatabase,
    FdoStringP rootOwner,
    FdoStringP rootObjectName,
    const FdoSmPhOwner* pOwner,
    FdoSchemaElementState elementState,
    FdoSmPhRdDbObjectReader* reader
) :
    FdoSmPhGrdView(viewName, rootDatabase, rootOwner, rootObjectName, pOwner, elementState),
    FdoSmPhOdbcDbObject(viewName, pOwner, reader),
    FdoSmPhDbObject(viewName, pOwner, elementState)
{
    //TODO;  //TODO: read stuff from pDbObjectReader that is specific to ODBC views.
}

FdoSmPhOdbcView::~FdoSmPhOdbcView(void)
{
}

bool FdoSmPhOdbcView::Add()
{
    FdoStringP sqlStmt = FdoStringP::Format(
        L"create view %ls ( %ls ) as %ls",
        GetName(),
        (FdoString*) GetAddColsSql()->ToString(),
        (FdoString*) GetAddRootSql()
    );

    ActivateOwnerAndExecute( sqlStmt );

    return true;
}

bool FdoSmPhOdbcView::Delete()
{
    FdoStringP sqlStmt = FdoStringP::Format(
        L"drop view %ls",
        GetName()
    );

    ActivateOwnerAndExecute( sqlStmt );

    return true;
}

FdoStringP FdoSmPhOdbcView::GetRootNameSql()
{
    FdoStringP rootDatabase = this->GetRootDatabase();
    FdoStringP rootOwner = this->GetRootOwner();
    FdoStringP rootObject = this->GetRootObjectName();

    if ( rootDatabase != L"" ) 
        // TODO: proper message indicating that Odbc provider does not
        // support multiple database instances.
        throw FdoSchemaException::Create();

    if ( (rootOwner == L"") || (rootOwner.ICompare(GetManager()->GetDefaultOwnerName()) == 0) ) 
        return rootObject;
    else 
        return FdoStringP::Format(
            L"%ls.dbo.%ls",
            (FdoString*) rootOwner,
            (FdoString*) rootObject
        );
}
