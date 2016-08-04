#ifndef FDORDBMSPHRDORAODBCPKEYREADER_H
#define FDORDBMSPHRDORAODBCPKEYREADER_H     1
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
//
// To perform the query, the caller creates this object with a
// SQL statement, which is parsed and executed. The caller then
// can call ReadNext() to get each row returned by the query and
// the various Get functions to get the row data.
//
// The query is initially positioned before the first row, so
// ReadNext must be called before data can be retrieved.

class FdoSmPhRdOraOdbcPkeyReader : public FdoSmPhRdPkeyReader
{
public:
    // Create reader to get the primary key for a particular table.
    //
    // Parameters:
    //      mgr: physical schema manager
    //      dbObject: get primary key for this table.
    FdoSmPhRdOraOdbcPkeyReader(
        FdoSmPhMgrP mgr,
        FdoSmPhDbObjectP    dbObject
    );

    // Create reader to get all primary keys for an owner (datastore)
    // keys are ordered by table.
    //
    // Parameters:
    //      mgr: physical schema manager
    //      owner: get primary keys for this owner.
    FdoSmPhRdOraOdbcPkeyReader(
        FdoSmPhMgrP mgr,
        FdoSmPhOwnerP    owner
    );

    // Deactivates the query.
    ~FdoSmPhRdOraOdbcPkeyReader(void);

protected:
    // unused constructor needed only to build on Linux
    FdoSmPhRdOraOdbcPkeyReader() {}

private:

    FdoSmPhReaderP MakeReader(
        FdoSmPhMgrP mgr,
        const FdoSmPhOwner* owner,
        FdoSmPhDbObjectP    dbObject
    );

    FdoSmPhDbObjectP mDbObject;
};

#endif
