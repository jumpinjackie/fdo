#ifndef FDOSMPHRDSQSBASEOBJECTREADER_H
#define FDOSMPHRDSQSBASEOBJECTREADER_H        1
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
#include <Sm/Ph/Rd/BaseObjectReader.h>
#include <Sm/Ph/Rd/TableJoin.h>

// SqlServer database object reader implementation

class FdoSmPhRdSqsBaseObjectReader : public FdoSmPhRdBaseObjectReader
{
public:
    // Create and execute the base object reader
    //
    // Parameters:
    //      owner: get all base object references for this owner.
    FdoSmPhRdSqsBaseObjectReader(
        FdoSmPhOwnerP owner
    );

    // Create and execute the base object reader
    //
    // Parameters:
    //      dbObject: get all base object references for this database object.
    FdoSmPhRdSqsBaseObjectReader(
        FdoSmPhDbObjectP dbObject
    );

    // Create and execute the base object reader
    //
    // Parameters:
    //      objectNames: get all base object references for these database objects.
    FdoSmPhRdSqsBaseObjectReader(
        FdoSmPhOwnerP owner,
        FdoStringsP objectNames
    );

    // Create and execute the base object reader
    //
    // Parameters:
    //      owner: get base object references for this owner.
    //      join: limit references to retrieve by joining to another table.
    FdoSmPhRdSqsBaseObjectReader(
        FdoSmPhOwnerP owner,
        FdoSmPhRdTableJoinP join
    );

    // Deactivates the database object reader.
    virtual ~FdoSmPhRdSqsBaseObjectReader(void);

    // Extended function, which handles the name field specially.
    // The name field value is built by concatenating user (table_schema)
    // and object (table_name) names.
	virtual FdoStringP GetString( FdoStringP tableName, FdoStringP fieldName );

protected:
    FdoSmPhReaderP MakeQueryReader(
        FdoSmPhOwnerP owner,
        FdoStringsP objectNames,
        FdoSmPhRdTableJoinP join = (FdoSmPhRdTableJoin*) NULL
    );

    // Creates a set of rows describing the fields for this
    // reader. There is one field per database object attribute.
    FdoSmPhRowsP MakeRows( FdoSmPhMgrP mgr );
};

typedef FdoPtr<FdoSmPhRdSqsBaseObjectReader> FdoSmPhRdSqsBaseObjectReaderP;

#endif
