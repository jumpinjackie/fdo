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
#include "View.h"
#include "Mgr.h"

FdoSmPhPostGisView::FdoSmPhPostGisView(FdoStringP viewName,
    FdoStringP rootDatabase,
    FdoStringP rootOwner,
    FdoStringP rootObjectName,
    const FdoSmPhOwner* owner,
    FdoSchemaElementState state,
    FdoSmPhRdDbObjectReader* reader)
    : FdoSmPhGrdView(viewName, rootDatabase, rootOwner,
        rootObjectName, owner, state),
      FdoSmPhPostGisDbObject(viewName, owner, reader),
      FdoSmPhDbObject(viewName, owner, state)
{
    // idle
}

FdoSmPhPostGisView::~FdoSmPhPostGisView()
{
    // idle
}

FdoStringP FdoSmPhPostGisView::GetRootNameSql()
{
    FdoStringP rootDatabase = this->GetRootDatabase();
    FdoStringP rootOwner = this->GetRootOwner();
    FdoStringP rootObject = this->GetRootObjectName();

    if (rootDatabase != L"")
    {
        throw FdoSchemaException::Create(NlsMsgGet4(FDORDBMS_148,
                "Cannot create view on %1$ls.%2$ls.%3$ls; %4$ls "
                "Provider does not support views on objects on "
                "other database servers.",
                static_cast<FdoString*>(rootDatabase),
                static_cast<FdoString*>(rootOwner),
                static_cast<FdoString*>(rootObject),
                "PostGIS"));
    }

    FdoStringP rootName = FdoStringP::Format(
        L"%ls%ls%ls\"%ls\"",
        ((rootOwner == L"") ? L"" : L"\""),
        static_cast<FdoString*>(rootOwner),
        ((rootOwner == L"") ? L"" : L"\"."),
        static_cast<FdoString*>(rootObject));

    return rootName;
}
