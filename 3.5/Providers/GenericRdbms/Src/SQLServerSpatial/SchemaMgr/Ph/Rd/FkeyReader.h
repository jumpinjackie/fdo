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
        FdoSmPhMgrP mgr,
        FdoSmPhDbObjectP    dbObject
    );

    FdoSmPhRdSqsFkeyReader(
        FdoSmPhMgrP mgr,
        FdoSmPhOwnerP    owner
    );

    // Deactivates the foreign key reader.
    ~FdoSmPhRdSqsFkeyReader(void);

    virtual FdoStringP GetString( FdoStringP tableName, FdoStringP fieldName );

private:

    FdoSmPhReaderP MakeReader(
        FdoSmPhMgrP         mgr,
        const FdoSmPhOwner* owner,
        FdoSmPhDbObjectP    dbObject
    );

    FdoSmPhDbObjectP mDbObject;
};

#endif
