#ifndef FDOSMPHSQSTABLE_H
#define FDOSMPHSQSTABLE_H       1
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

#ifdef _WIN32
#pragma once
#endif

#include "Fkey.h"
#include "DbObject.h"
#include "../../../SchemaMgr/Ph/Table.h"
#include "Index.h"

// SqlServer Provider implementation of a Table.
// Represents an SqlServer schema (user)
class FdoSmPhSqsTable : public FdoSmPhGrdTable, public FdoSmPhSqsDbObject
{
public:
    // Constructs an instance of a Table object.
    //
    // Parameters:
    //      name: table name
    //      pOwner: the table's owning schema.
    //      elementState: indicates whether this is a new or existing table.
    //      pkeyName: name for table's primary key. Applies only to new tables.
    //          Primary key name on existing tables not changed.
    FdoSmPhSqsTable(
        FdoStringP name,
        const FdoSmPhOwner* pOwner,
        FdoSchemaElementState elementState = FdoSchemaElementState_Added,
        FdoStringP pkeyName = L"",
        FdoSmPhRdDbObjectReader* reader = NULL
    );

    ~FdoSmPhSqsTable(void);

    // SqlServer does not allow not-null columns to be added to existing
    // tables (even when table is empty). Therefore, this function is 
    // overridden to return false.
    virtual bool SupportsAddNotNullColumn() const;

    // Implementations for constraint and
    // index readers.
    virtual FdoPtr<FdoSmPhRdConstraintReader> CreateConstraintReader( FdoString* type ) const;
    virtual FdoPtr<FdoSmPhRdIndexReader> CreateIndexReader() const;


private:
    virtual bool Add();
    virtual bool Modify();
    virtual bool Delete();

    // Index creator implementation
    virtual FdoSmPhIndexP NewIndex(
        FdoStringP name,
        bool isUnique,
        FdoSchemaElementState elementState = FdoSchemaElementState_Added
    );

    // Spatial Index creator implementation
    virtual FdoSmPhIndexP NewSpatialIndex(
        FdoStringP name,
        bool isUnique,
        FdoSchemaElementState elementState = FdoSchemaElementState_Added
    );

    // Foreign Key creator implementation
    virtual FdoSmPhFkeyP NewFkey(
        FdoStringP name,
        FdoStringP pkeyTableName,
        FdoStringP pkeyTableOwner,
        FdoSchemaElementState elementState = FdoSchemaElementState_Added
    );

    FdoStringP GetAddHdgSql();

    // Get SQL for setting storage parameters for table. 
    // Currently just selects the storage engine.
    FdoStringP GetAddStorageSql();

    // Get the entire SQL statement to create this table:
    FdoStringP GetAddSql();

	// Adjust CHECK clause syntax
    FdoStringP ConvertCkey( FdoDataValue *val );

	// Returns a CHECK() clause
	FdoStringP GetCkeyClause( FdoStringP columnName, FdoDataPropertyP fdoProp );

};

typedef FdoPtr<FdoSmPhSqsTable> FdoSmPhSqsTableP;

#endif
