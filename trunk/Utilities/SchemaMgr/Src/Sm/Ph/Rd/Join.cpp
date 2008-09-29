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
#include <Sm/Ph/Rd/Join.h>

FdoSmPhRdJoin::FdoSmPhRdJoin(
    FdoStringP name,
    FdoSmPhColumnsP columns,
    FdoStringP whereClause
): 
	FdoSmPhSchemaElement( name, L"", FdoSmPhColumnP(columns->GetItem(0))->GetManager() ),
    mColumns(columns),
    mWhereClause(whereClause)
{
}


FdoSmPhRdJoin::~FdoSmPhRdJoin(void)
{
}


FdoStringP FdoSmPhRdJoin::GetFrom()
{
	FdoSmPhColumnP		pColumn = mColumns->GetItem(0);
    const FdoSmPhDbObject* dbObject = (const FdoSmPhDbObject*)(pColumn->GetParent());
    FdoStringP tableName = dbObject->GetDbQName();

    // table is aliases by this join's name.
    FdoStringP fromClause = tableName + L" " + GetName();

    return fromClause;
}

FdoStringP FdoSmPhRdJoin::GetWhere( FdoStringsP joinColumns )
{
    FdoInt32 ix;
    FdoStringsP clauses = FdoStringCollection::Create();

    if ( joinColumns->GetCount() != mColumns->GetCount() ) {
        //TODO: throw exception. Do when port to Granite, since
        // messages cannot be added to Reliant service packs.
    }

    // Join each given column to each join column for the join table.
    for ( ix = 0; ix < joinColumns->GetCount(); ix++ ) {
        clauses->Add( 
            FdoStringP::Format( 
                L"%ls = %ls",
                (FdoString*) joinColumns->GetString(ix),
                (FdoString*) GetJoinColumn(ix)
            )
        );
    }

    // Add this join's where clause.
    FdoStringP extraClause = GetWhere();

    if ( extraClause != L"" ) 
        clauses->Add( FdoStringP::Format(L"(%ls)", (FdoString*) extraClause) );

    return clauses->ToString( L" and " );
}

FdoStringP FdoSmPhRdJoin::GetWhere()
{
    return mWhereClause;
}

FdoSmPhDbObjectP FdoSmPhRdJoin::GetDbObject()
{
	FdoSmPhColumnP		pColumn = mColumns->GetItem(0);
    FdoSmPhDbObjectP dbObject = FDO_SAFE_ADDREF((FdoSmPhDbObject*)(FdoSmPhSchemaElement*)(pColumn->GetParent()));

    return dbObject;
}

FdoStringP FdoSmPhRdJoin::GetJoinColumn( FdoInt32 ix ) 
{
    FdoSmPhColumnP column = mColumns->GetItem(ix);

    return FdoStringP::Format( 
        L"%ls.%ls",
        GetName(),
        (FdoString*) column->GetDbName()
    );
}

