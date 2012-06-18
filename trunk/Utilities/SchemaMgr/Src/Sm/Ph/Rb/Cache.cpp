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
#include <Sm/Ph/Rb/Cache.h>

FdoSmPhRbCache::FdoSmPhRbCache(void) :
    FdoSmSchemaElement( L"", L"" )
{
}

FdoSmPhRbCache::~FdoSmPhRbCache(void)
{
}

void FdoSmPhRbCache::AddTable(FdoString* name, FdoSchemaElementState elementState)
{
    // Find the table.
    FdoSmPhRbTableP pRbTable = GetTables()->FindItem(name);

    if ( pRbTable == NULL ) {
        // Table not present so add it.
		pRbTable = new FdoSmPhRbTable( name, (FdoSmSchemaElement*) this, elementState );
		GetTables()->Add( pRbTable );
	}

    // Record the modification state of the table.
	pRbTable->SetElementState( elementState );
}

void FdoSmPhRbCache::AddColumn(FdoString* tableName, FdoString* columnName, FdoSchemaElementState elementState)
{
    // Make sure the column's table is added.
    AddTable( tableName, FdoSchemaElementState_Unchanged );

    FdoSmPhRbTableP pRbTable = GetTables()->FindItem(tableName);

    // Find the column.
    FdoSmPhRbColumnP pRbColumn = pRbTable->GetColumns()->FindItem(columnName);

    if ( pRbColumn == NULL ) {
        // Column not present so add it.
	    pRbColumn = new FdoSmPhRbColumn( columnName, pRbTable, elementState );
	    pRbTable->GetColumns()->Add( pRbColumn );
    }

    // Record the modification state of the column.
    pRbColumn->SetElementState( elementState );
}

void FdoSmPhRbCache::Clear()
{
    GetTables()->Clear();
}

bool FdoSmPhRbCache::HasEntries()
{
    return( GetTables()->GetCount() > 0 );
}

const FdoSmPhRbTable* FdoSmPhRbCache::RefTable(FdoString* name)
{
	return RefTables()->RefItem(name);
}

const FdoSmPhRbColumn* FdoSmPhRbCache::RefColumn(FdoString* tableName, FdoString* columnName)
{
	const FdoSmPhRbTable* pRbTable = RefTable(tableName);

	return( pRbTable ? pRbTable->RefColumns()->RefItem(columnName) : NULL);
}
