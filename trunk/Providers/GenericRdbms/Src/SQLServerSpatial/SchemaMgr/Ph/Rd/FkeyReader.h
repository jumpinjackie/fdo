#ifndef FDORDBMSPHRDSQSFKEYREADER_H
#define FDORDBMSPHRDSQSFKEYREADER_H     1
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

#include <Sm/Ph/Rd/FkeyReader.h>
#include <Sm/Ph/Rd/TableJoin.h>

// SqlServer foreign key reader implementation

class FdoSmPhRdSqsFkeyReader : public FdoSmPhRdFkeyReader
{
public:
    // Create and execute the foreign key reader
    //
    // Parameters:
    //      mgr: Physical Schema Manager
    //      dbObject: Retrieve foreign keys for this database object.
    FdoSmPhRdSqsFkeyReader(
        FdoSmPhOwnerP    owner,
        FdoSmPhDbObjectP    dbObject
    );

    FdoSmPhRdSqsFkeyReader(
        FdoSmPhOwnerP    owner,
        FdoStringsP objectNames
    );

    FdoSmPhRdSqsFkeyReader(
        FdoSmPhOwnerP    owner
    );

    FdoSmPhRdSqsFkeyReader(
        FdoSmPhOwnerP owner,
        FdoSmPhRdTableJoinP join
    );

    // Deactivates the foreign key reader.
    ~FdoSmPhRdSqsFkeyReader(void);

    virtual FdoStringP GetString( FdoStringP tableName, FdoStringP fieldName );

protected:
    // Adds table_schema to field list.
    virtual FdoSmPhRowsP MakeRows( FdoSmPhMgrP mgr );

private:

    FdoSmPhReaderP MakeReader(
        FdoSmPhMgrP         mgr,
        FdoSmPhOwnerP       owner,
        FdoStringsP         objectNames,
        FdoSmPhRdTableJoinP join = (FdoSmPhRdTableJoin*) NULL
    );

    FdoSmPhDbObjectP mDbObject;
};

#endif
