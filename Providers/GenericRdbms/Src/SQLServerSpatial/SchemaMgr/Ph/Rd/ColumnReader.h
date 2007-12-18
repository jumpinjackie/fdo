#ifndef FDOSMPHRDSQSCOLUMNREADER_H
#define FDOSMPHRDSQSCOLUMNREADER_H      1
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

#include <Sm/Ph/Rd/ColumnReader.h>
#include <Inc/Rdbi/context.h>
#include <Sm/Ph/Rd/TableJoin.h>

// SqlServer column reader implementation
class FdoSmPhRdSqsColumnReader : public FdoSmPhRdColumnReader
{
public:
    FdoSmPhRdSqsColumnReader (FdoSmPhMgrP mgr, FdoSmPhDbObjectP dbObject);

    FdoSmPhRdSqsColumnReader(
        FdoSmPhOwnerP owner,
        FdoStringsP objectNames
    );

    FdoSmPhRdSqsColumnReader(
        FdoSmPhOwnerP owner,
        FdoSmPhRdTableJoinP join
    );

    // Deactivates the query.
    ~FdoSmPhRdSqsColumnReader(void);

    FdoStringP GetString( FdoStringP tableName, FdoStringP fieldName );

	// Generates the Schema Manager type for each row retrieved, from the 
    // type retrieved from SqlServer.
	virtual bool ReadNext();

    // Get the type enum for the current column
    virtual FdoSmPhColType GetType();

private:
    // Create the column reader
    FdoSmPhReaderP MakeQueryReader (
        FdoSmPhMgrP mgr, 
        const FdoSmPhOwner* owner,
        FdoStringsP objectNames,
        FdoSmPhRdTableJoinP join = (FdoSmPhRdTableJoin*) NULL
    );

    FdoStringsP DbObject2Objects( FdoSmPhDbObjectP dbObject );
    
    FdoSmPhColType      mColType;
};

#endif
