#ifndef FDOSMPHFKEY_H
#define FDOSMPHFKEY_H		1
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

#include <Sm/Ph/SchemaElement.h>
#include <Sm/Ph/ColumnCollection.h>

class FdoSmPhMgr;
class FdoSmPhTable;

// This class represents a foreign key relationship between two tables.
class FdoSmPhFkey : public FdoSmPhDbElement
{
public:
    /// Get the Primary Table ...

    /// as read-only pointer
    const FdoSmPhTable* RefPkeyTable() const;

    /// as read-write smart pointer.
    FdoPtr<FdoSmPhTable> GetPkeyTable();

    /// Get the Foreign Key Table ...

    /// as read-only pointer
    const FdoSmPhTable* RefFkeyTable() const
    {
        return (const FdoSmPhTable*) GetParent();
    }

    /// as read-write smart pointer.
    FdoPtr<FdoSmPhTable> GetFkeyTable();

    /// Get the foreign key columns (columns in foreign table ) ...

    /// as read-only pointer
    const FdoSmPhColumnCollection* RefFkeyColumns() const;

    /// as read-write smart pointer.
    FdoSmPhColumnsP GetFkeyColumns();

    /// Get all the primary key columns (corresponding columns in primary table) ...

    /// as read-only pointer
    const FdoSmPhColumnCollection* RefPkeyColumns() const;

    /// as read-write smart pointer.
    FdoSmPhColumnsP GetPkeyColumns();

    // Reverse-engineer an association property name from this foreign key's name.
    virtual FdoStringP GetBestPropertyName() const;

    /// Add a column to this Foreign Key.
    void AddFkeyColumn( FdoSmPhColumnP fkeyColumn, FdoStringP pkeyColumnName  );

    /// Get a constraint clause for adding this foreign key to a table.
    FdoStringP GetAddSql();

protected:
    /// unused constructor only for building on Linux
    FdoSmPhFkey() {}

    /// Constructs an instance of a Foreign Key object.
    /// 
    /// Parameters:
    /// 	name: Foreign Key  name
    ///      pFkeytable: the Foreign Table.
    /// 	pkeyTableName: name of primary table referenced by this foreign key.
    /// 	pkeyTableOwner: name of primary table owner.
    ///      elementState: indicates whether foreign key is new or already exists.

    FdoSmPhFkey(
        FdoStringP name, 
        const FdoSmPhTable* pFkeyTable,
        FdoStringP pkeyTableName,
        FdoStringP pkeyTableOwner,
		FdoSchemaElementState elementState = FdoSchemaElementState_Added
    );

	~FdoSmPhFkey(void);

private:

    /// Retrieve and cache the primary table.
    void LoadPkeyTable();

    void AddPkeyColumnError(FdoStringP columnName);

    FdoStringP              mPkeyTableName;
    FdoStringP              mPkeyTableOwner;
    FdoPtr<FdoSmPhTable>    mPkeyTable;

    FdoSmPhColumnsP         mFkeyColumns;
    FdoStringsP             mPkeyColumnNames;
    FdoSmPhColumnsP         mPkeyColumns;

};

typedef FdoPtr<FdoSmPhFkey> FdoSmPhFkeyP;

#endif


