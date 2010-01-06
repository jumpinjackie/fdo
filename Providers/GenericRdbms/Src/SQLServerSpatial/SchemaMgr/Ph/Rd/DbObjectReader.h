#ifndef FDOSMPHRDSQSDBOBJECTREADER_H
#define FDOSMPHRDSQSDBOBJECTREADER_H        1
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
#include <Sm/Ph/Rd/DbObjectReader.h>
#include <Sm/Ph/Rd/TableJoin.h>

// SqlServer database object reader implementation

class FdoSmPhRdSqsDbObjectReader : public FdoSmPhRdDbObjectReader
{
public:
    // Create and execute the database object reader
    //
    // Parameters:
    //      owner: get objects owned by this owner.
    //      objectName: If L"" then get all objects for the owner.
    //          Otherwise retrieve only this object.
    FdoSmPhRdSqsDbObjectReader(
        FdoSmPhOwnerP owner,
        FdoStringP objectName = L""
    );

    FdoSmPhRdSqsDbObjectReader(
        FdoSmPhOwnerP owner,
        FdoStringsP objectNames
    );

    FdoSmPhRdSqsDbObjectReader(
        FdoSmPhOwnerP owner,
        FdoSmPhRdTableJoinP join
    );

    // Deactivates the database object reader.
    virtual ~FdoSmPhRdSqsDbObjectReader(void);

    virtual FdoSmPhDbObjType GetType();

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

typedef FdoPtr<FdoSmPhRdSqsDbObjectReader> FdoSmPhRdSqsDbObjectReaderP;

#endif
