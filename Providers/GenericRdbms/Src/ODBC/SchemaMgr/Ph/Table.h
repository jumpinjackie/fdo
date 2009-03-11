#ifndef FDOSMPHODBCTABLE_H
#define FDOSMPHODBCTABLE_H       1
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

// Odbc Provider implementation of a Table.
// Represents an Odbc schema (user)
class FdoSmPhOdbcTable : public FdoSmPhGrdTable, public FdoSmPhOdbcDbObject
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
    FdoSmPhOdbcTable(
        FdoStringP name,
        const FdoSmPhOwner* pOwner,
        FdoSchemaElementState elementState = FdoSchemaElementState_Added,
        FdoStringP pkeyName = L"",
        FdoSmPhRdDbObjectReader* reader = NULL
    );

    ~FdoSmPhOdbcTable(void);

    // Implementations for creating primary key, foreign key and
    // index readers.
    virtual FdoPtr<FdoSmPhRdPkeyReader> CreatePkeyReader() const;
    virtual FdoPtr<FdoSmPhRdFkeyReader> CreateFkeyReader() const;
    virtual FdoPtr<FdoSmPhRdConstraintReader> CreateConstraintReader( FdoString* type ) const;


private:
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

    FdoStringP GetAddHdgSql();

    // Get SQL for setting storage parameters for table. 
    // Currently just selects the storage engine.
    FdoStringP GetAddStorageSql();

    // Get the entire SQL statement to create this table:
    FdoStringP GetAddSql();
};

typedef FdoPtr<FdoSmPhOdbcTable> FdoSmPhOdbcTableP;

#endif
