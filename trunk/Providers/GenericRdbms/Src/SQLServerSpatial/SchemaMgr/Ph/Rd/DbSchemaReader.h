#ifndef FDOSMPHRDSQSDBSCHEMAREADER_H
#define FDOSMPHRDSQSDBSCHEMAREADER_H		1
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
#include <Sm/Ph/Reader.h>

// This reader reads all of the physical (Sql Server) schemas for the 
// current datastore.

class FdoSmPhRdSqsDbSchemaReader : public FdoSmPhReader
{
public:
    /// Create the reader
	//
    /// Parameters:
    /// 	owner: retrieve schemas for this datastore.
    ///     schemaName: if set, retrieve only this schema.
    ///         If "", retrieve all schemas for the owner.
    FdoSmPhRdSqsDbSchemaReader(
        FdoSmPhOwnerP owner,
        FdoStringP schemaName = L""
    );

    /// Deactivates the reader
	~FdoSmPhRdSqsDbSchemaReader(void);

protected:
    //Unused constructor needed only to build on Linux
    FdoSmPhRdSqsDbSchemaReader() {}

    /// Create the sub-reader, which does the select against
    /// the RDBMS.
	//
    /// Parameters:
    /// 	owner: retrieve schemas for this datastore.
    ///     schemaName: if set, retrieve only this schema.
    ///         If "", retrieve all schemas for the owner.
    FdoSmPhReaderP MakeQueryReader(
        FdoSmPhOwnerP owner,
        FdoStringP schemaName
    );

    // Creates a set of rows describing the fields for this
    // reader. There is one field per database object attribute.
    FdoSmPhRowsP MakeRows( FdoSmPhMgrP mgr );

    // Creates a set of bind variables for this reader.
    FdoSmPhRowP MakeBinds( FdoSmPhMgrP mgr, FdoStringP schemaName );

private:
    FdoSmPhOwnerP   mOwner;
};

typedef FdoPtr<FdoSmPhRdSqsDbSchemaReader> FdoSmPhRdSqsDbSchemaReaderP;

#endif


