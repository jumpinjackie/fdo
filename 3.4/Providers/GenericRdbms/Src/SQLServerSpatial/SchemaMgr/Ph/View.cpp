/*
 * Copyright (C) 2004-2008  Autodesk, Inc.
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

FdoSmPhSqsView::FdoSmPhSqsView(
    FdoStringP viewName,
    FdoStringP rootDatabase,
    FdoStringP rootOwner,
    FdoStringP rootObjectName,
    const FdoSmPhOwner* pOwner,
    FdoSchemaElementState elementState,
    FdoSmPhRdDbObjectReader* reader
) :
    FdoSmPhGrdView(viewName, rootDatabase, rootOwner, rootObjectName, pOwner, elementState),
    FdoSmPhSqsDbObject(viewName, pOwner, reader),
    FdoSmPhDbObject(viewName, pOwner, elementState)
{
    //TODO;  //TODO: read stuff from pDbObjectReader that is specific to sql server views.
}

FdoSmPhSqsView::~FdoSmPhSqsView(void)
{
}

bool FdoSmPhSqsView::Add()
{
    FdoStringP sqlStmt = FdoStringP::Format(
        L"create view %ls ( %ls ) as %ls",
        (FdoString*) GetDbName(),
        (FdoString*) GetAddColsSql()->ToString(),
        (FdoString*) GetAddRootSql()
    );

    ActivateOwnerAndExecute( sqlStmt );

    return true;
}

bool FdoSmPhSqsView::Delete()
{
    FdoStringP sqlStmt = FdoStringP::Format(
        L"drop view %ls",
        (FdoString*) GetDbName()
    );

    ActivateOwnerAndExecute( sqlStmt );

    return true;
}

FdoStringP FdoSmPhSqsView::GetRootNameSql()
{
    FdoStringP rootDatabase = this->GetRootDatabase();
    FdoStringP rootOwner = this->GetRootOwner();
    FdoStringP rootObject = this->GetRootObjectName();

    if ( rootDatabase != L"" ) 
        throw FdoSchemaException::Create( 
            NlsMsgGet4(
	            FDORDBMS_148,
                "Cannot create view on %1$ls.%2$ls.%3$ls; %4$ls Provider does not support views on objects on other database servers.",
                (FdoString*) rootDatabase,
                (FdoString*) rootOwner,
                (FdoString*) rootObject,
                L"Sql Server"
            )
       );

    if ( !rootObject.Contains(L".") ) 
        rootObject = FdoStringP(L"dbo.") + rootObject;

    if ( (rootOwner == L"") || (rootOwner.ICompare(GetManager()->GetDefaultOwnerName()) == 0) ) 
        return rootObject;
    else 
        return FdoStringP::Format(
            L"\"%ls\".\"%ls\"",
            (FdoString*) rootOwner,
            (FdoString*) rootObject.Replace(L".",L"\".\"")
        );
}
