#ifndef FDORDBMSPHRDMYSQLPKEYREADER_H
#define FDORDBMSPHRDMYSQLPKEYREADER_H     1
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

#include <Sm/Ph/Rd/PkeyReader.h>
//#include <Inc/Ocidr/context.h>

// MySql primary key reader implementation

class FdoSmPhRdMySqlPkeyReader : public FdoSmPhRdPkeyReader
{
public:
    // Create and execute the primary key reader
    //
    // Parameters:
    //      mgr: Physical Schema Manager
    //      dbObject: Retrieve primary key columns for this database object.
    FdoSmPhRdMySqlPkeyReader(
        FdoSmPhMgrP mgr,
        FdoSmPhDbObjectP    dbObject
    );

    FdoSmPhRdMySqlPkeyReader(
        FdoSmPhMgrP mgr,
        FdoSmPhOwnerP    owner
    );

    // Deactivates the query.
    ~FdoSmPhRdMySqlPkeyReader(void);

    // Some primary keys always end up being called PRIMARY. This function
    // blanks out these names to prevent them from getting into schema
    // overrides and causing SQL errors when applied to other tables. 
    FdoStringP GetString( FdoStringP tableName, FdoStringP fieldName );

private:

    FdoSmPhReaderP MakeReader(
        FdoSmPhMgrP mgr,
        const FdoSmPhOwner* owner,
        FdoSmPhDbObjectP    dbObject
    );

    FdoSmPhDbObjectP mDbObject;
};

#endif
