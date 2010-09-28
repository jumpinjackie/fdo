#ifndef FDOSMPHMYSQLTABLE_H
#define FDOSMPHMYSQLTABLE_H       1
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

#include "DbObject.h"
#include "../../../SchemaMgr/Ph/Table.h"
#include "Fkey.h"

// MySql Provider implementation of a Table.
// Represents an MySql schema (user)
class FdoSmPhMySqlTable : public FdoSmPhGrdTable, public FdoSmPhMySqlDbObject
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
    FdoSmPhMySqlTable(
        FdoStringP name,
        const FdoSmPhOwner* pOwner,
        FdoSchemaElementState elementState = FdoSchemaElementState_Added,
        FdoStringP pkeyName = L"",
        FdoSmPhRdDbObjectReader* reader = NULL
    );

    ~FdoSmPhMySqlTable(void);

    // Returns NULL, size = 0.
    // No lock types currently supported in MySql.
	virtual const FdoLockType* GetLockTypes(FdoInt32& size) const;

    // Implementations for creating constraint and
    // index readers.
    virtual FdoPtr<FdoSmPhRdConstraintReader> CreateConstraintReader( FdoString* type ) const;

	/// Get SQL clause for dropping a constraint
	virtual FdoStringP GetDropConstraintSql(FdoStringP constraintName);

private:
    FdoSmPhMySqlTable() {};

    virtual bool Add();
    virtual bool Modify();
    virtual bool Delete();

    // Foreign Key creator implementation
    virtual FdoSmPhFkeyP NewFkey(
        FdoStringP name,
        FdoStringP pkeyTableName,
        FdoStringP pkeyTableOwner,
        FdoSchemaElementState elementState = FdoSchemaElementState_Added
    );

    // Get SQL for setting storage parameters for table. 
    // Currently just selects the storage engine.
    FdoStringP GetAddStorageSql();
};

typedef FdoPtr<FdoSmPhMySqlTable> FdoSmPhMySqlTableP;

#endif
