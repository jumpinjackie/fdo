#ifndef FDORDBMSPHRDSQSPKEYREADER_H
#define FDORDBMSPHRDSQSPKEYREADER_H     1
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

#include <Sm/Ph/Rd/PkeyReader.h>
#include <Sm/Ph/Rd/TableJoin.h>

// SqlServer primary key reader implementation

class FdoSmPhRdSqsPkeyReader : public FdoSmPhRdPkeyReader
{
public:
    // Create and execute reader for primary keys for a database object.
    //
    // Parameters:
    //      dbObject: Retrieve primary key columns for this database object 
    //      (usually a table).
    FdoSmPhRdSqsPkeyReader(
        FdoSmPhDbObjectP dbObject
    );

    // Create and execute reader for all primary keys for a datastore
    //
    // Parameters:
    //      owner: The datastore
    FdoSmPhRdSqsPkeyReader(
        FdoSmPhOwnerP    owner
    );

    // Create and execute reader for primary keys for list of database objects
    //
    // Parameters:
    //      owner: Owner of the database objects
    //      objectNames: Retrieve primary key columns for these database objects
    //      (usually tables).
    FdoSmPhRdSqsPkeyReader(
        FdoSmPhOwnerP   owner,
        FdoStringsP     objectNames
    );

    // Create and execute reader for primary keys for a datastore. Limit the primary
    // keys fetched according to the given join
    //
    // Parameters:
    //      owner: Datastore to retrieve primary keys from.
    //      join: a join to another tables (typically f_classdefinition).
    FdoSmPhRdSqsPkeyReader(
        FdoSmPhOwnerP   owner,
        FdoSmPhRdTableJoinP join
    );

    // Deactivates the query.
    ~FdoSmPhRdSqsPkeyReader(void);

    virtual FdoStringP GetString( FdoStringP tableName, FdoStringP fieldName );

private:

    FdoSmPhReaderP MakeReader(
        FdoSmPhOwnerP owner,
        FdoStringsP objectNames,
        FdoSmPhRdTableJoinP join = (FdoSmPhRdTableJoin *) NULL
    );

    FdoSmPhDbObjectP mDbObject;
};

#endif
