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
#include <Sm/Ph/Mgr.h>
#include <Sm/Ph/DbObject.h>
#include <Sm/Ph/Owner.h>
#include <Sm/Ph/ColumnCollection.h>
#include <Sm/Ph/Mt/ClassTableJoin.h>

FdoSmPhMtClassTableJoin::FdoSmPhMtClassTableJoin(
    FdoSmPhOwnerP owner,
    FdoStringP schemaName
) :
    // Note that f_classdefinition alias is set to classdefinition
	FdoSmPhRdTableJoin( L"classdefinition", GetColumn(owner), MakeWhere(owner, schemaName) )
{
}

FdoSmPhMtClassTableJoin::~FdoSmPhMtClassTableJoin(void)
{
}

FdoSmPhDbObjectP FdoSmPhMtClassTableJoin::GetTable(
    FdoSmPhOwnerP owner
)
{
	FdoSmPhMgrP	mgr = owner->GetManager();
    return owner->GetDbObject( mgr->GetDcDbObjectName(L"f_classdefinition") );
}

FdoSmPhColumnP FdoSmPhMtClassTableJoin::GetColumn(
    FdoSmPhOwnerP owner
)
{
    FdoSmPhDbObjectP	classDefTable = GetTable(owner);
	FdoSmPhColumnsP		columns = classDefTable->GetColumns();
	FdoSmPhMgrP			mgr = owner->GetManager();

    return columns->GetItem( mgr->GetDcColumnName(L"tablename") );
}

FdoStringP FdoSmPhMtClassTableJoin::MakeWhere(
    FdoSmPhOwnerP owner,
    FdoStringP schemaName
)
{
    FdoSmPhDbObjectP classDefTable = GetTable(owner);

    // Build where clause for selecting only f_classdefinition rows for the given feature schema.
    // f_classdefinition's alias is classdefinition.
	FdoSmPhMgrP	mgr = classDefTable->GetManager();

    FdoStringP whereClause = FdoStringP::Format(
        L"classdefinition.schemaname = %ls",
        (FdoString*) mgr->FormatSQLVal( schemaName, FdoSmPhColType_String )
    );

    return whereClause;
}
