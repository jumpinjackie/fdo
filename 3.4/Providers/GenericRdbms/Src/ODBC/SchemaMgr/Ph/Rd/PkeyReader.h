#ifndef FDORDBMSPHRDODBCPKEYREADER_H
#define FDORDBMSPHRDODBCPKEYREADER_H     1
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
#include <Inc/Rdbi/context.h>

// Odbc primary key reader implementation

class FdoSmPhRdOdbcPkeyReader : public FdoSmPhRdPkeyReader
{
public:
    // Create and execute the primary key reader
    //
    // Parameters:
    //      mgr: Physical Schema Manager
    //      dbObject: Retrieve primary key columns for this database object.
    FdoSmPhRdOdbcPkeyReader(
        FdoSmPhMgrP mgr,
        FdoSmPhDbObjectP    dbObject
    );

    // Deactivates the query.
    ~FdoSmPhRdOdbcPkeyReader(void);

private:

    bool ReadNext();

    FdoSmPhDbObjectP mDbObject;

    rdbi_context_def    *rdbi_context;
};

#endif
