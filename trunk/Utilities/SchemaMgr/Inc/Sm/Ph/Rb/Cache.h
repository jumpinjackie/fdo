#ifndef FDOSMPHRBCACHE_H
#define FDOSMPHRBCACHE_H		1
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

#ifdef _WIN32
#pragma once
#endif

#include <Sm/Ph/Rb/TableCollection.h>

// The rollback cache lists all tables and columns that have been modified
// by the Schema Manager. When a transaction rollback occurs, the metaschema 
// changes are rolled back but not the corresponding table and column changes.
//
// Therefore, the Schema Manager handles table and column rollbacks by re-synchronizing
// them with their metaschema elements. The rollback cache tells the Schema Manager
// which tables and columns might need to be rolled back.
//

class FdoSmPhRbCache : public FdoSmSchemaElement
{
public:
	FdoSmPhRbCache();
	~FdoSmPhRbCache(void);

    /// Add a table to the rollback cache.
    //
    /// Parameters:
    ///      name - the table name.
    ///      elementState - whether the table was created or deleted.
	void AddTable(FdoString* name, FdoSchemaElementState elementState);
	
    /// Add a table to the rollback cache.
    //
    /// Parameters:
    ///      tableName - the name of the column's table.
    ///      columnName - the column name.
    ///      elementState - whether the column was created or deleted.
    void AddColumn(FdoString* tableName, FdoString* columnName, FdoSchemaElementState elementState);
	
    /// Clears all rollback entries for the given connection.
    void Clear();

    /// Gets the rollback entries for the given connection.
    /// Returns NULL if there are no entries.
	bool HasEntries();

    /// Gets the rollback entry for the given table.
	const FdoSmPhRbTable* RefTable(FdoString* name);
 
    /// Gets the rollback entry for the given table and column.
	const FdoSmPhRbColumn* RefColumn(FdoString* tableName, FdoString* columnName);

private:
    /// Returns all table rollback entries.
    /// Returns non-modifiable object.
    const FdoSmPhRbTableCollection* RefTables() const
	{
		return &mTables;
	}

    /// Returns all table rollback entries
    FdoSmPhRbTableCollection* GetTables()
	{
		return &mTables;
	}


private:
    /// list of tables that have been modified.
	FdoSmPhRbTableCollection mTables;
};

typedef FdoPtr<FdoSmPhRbCache> FdoSmPhRbCacheP;

#endif


