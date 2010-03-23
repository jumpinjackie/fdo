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
#include <Sm/Ph/DbObject.h>
#include <Sm/Ph/Mgr.h>
#include <Sm/Collection.h>
#include <Sm/Ph/ColumnCollection.h>
#include <Sm/Ph/Table.h>
#include <Sm/Ph/Rd/TableJoin.h>

FdoSmPhRdTableJoin::FdoSmPhRdTableJoin(
    FdoStringP name,
    FdoSmPhColumnP tableNameColumn,
    FdoStringP whereClause
) :
	FdoSmPhRdJoin( name, MakeColumnList(tableNameColumn), whereClause )
{
}

FdoSmPhRdTableJoin::~FdoSmPhRdTableJoin(void)
{
}

FdoStringP FdoSmPhRdTableJoin::GetClause( 
    FdoSmPhMgrP mgr, 
    FdoStringP schemaColumn, 
    FdoStringP tableColumn 
)
{
    FdoStringP clause = FdoStringP::Format(
        L"JOIN %ls on %ls",
        (FdoString*) GetFrom(),
        (FdoString*) GetWhere(mgr, schemaColumn, tableColumn)
    );

    return clause;
}

FdoStringP FdoSmPhRdTableJoin::GetWhere( FdoStringP joinColumn )
{
    // This join derivation has only one join column. Convert column
    // to column list and call base function to generated where clause.
    FdoStringsP joinColumns = FdoStringCollection::Create();
    joinColumns->Add(joinColumn);

    return FdoSmPhRdJoin::GetWhere( joinColumns );
}

FdoStringP FdoSmPhRdTableJoin::GetWhere( FdoSmPhMgrP mgr, FdoStringP schemaColumn, FdoStringP tableColumn )
{
    // This join derivation has only one join column but it joins onto 2 columns
    // in the RDBMS data dictionary, where 1 column is the physical schema part
    // and the other is the table name part.
   FdoStringP whereClause = FdoStringP::Format( 
        L"  %ls and ((%ls = '%ls' and %ls = %ls) or ((%ls) = %ls))\n", 
        (FdoString*) FdoSmPhRdJoin::GetWhere(),
        (FdoString*) schemaColumn,
        (FdoString*) mgr->GetDefaultPhysicalSchemaName(),
        (FdoString*) tableColumn,
        (FdoString*) GetJoinColumn(),
        (FdoString*) mgr->FormatConcatSql( mgr->FormatConcatSql(schemaColumn,L"'.'"), tableColumn),
        (FdoString*) GetJoinColumn()
    );

    return whereClause;
}

FdoStringP FdoSmPhRdTableJoin::GetJoinColumn()
{
    // Only one join column so always the first one.
    return FdoSmPhRdJoin::GetJoinColumn(0);
}

FdoSmPhColumnsP FdoSmPhRdTableJoin::MakeColumnList( FdoSmPhColumnP tableNameColumn )
{
    FdoSmPhColumnsP columns = new FdoSmPhColumnCollection();
    columns->Add( tableNameColumn );

    return columns;
}

