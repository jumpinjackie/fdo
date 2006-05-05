#ifndef FDOSMPHRDOWNERREADER_H
#define FDOSMPHRDOWNERREADER_H		1
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
#include <Sm/Ph/Reader.h>
#include <Sm/Ph/RowCollection.h>

// This abstract class reads datastores (owners). Each provider must 
// provide its own implementing class.

class FdoSmPhRdOwnerReader : public FdoSmPhReader
{
public:
    /// Get name of current owner
    FdoStringP GetName() 
    {
        return GetString( L"", L"name" );
    }

    /// Returns true if current owner has MetaSchema tables.
    virtual FdoBoolean GetHasMetaSchema()
    {
        return ( GetString( L"", L"schemas_table") != L"" );
    }

    /// Returns the description of the current owner from F_SCHEMAINFO
	virtual FdoStringP GetDescription() = 0;

protected:
    //Unused constructor needed only to build on Linux
    FdoSmPhRdOwnerReader() {}

    /// Creates an owner reader.
	//
	FdoSmPhRdOwnerReader(
        FdoSmPhReaderP   reader,      // inner reader created by implementing class
        FdoSmPhDatabaseP database,    // get the owners for this database instance
        FdoStringP ownerName = L""    // owner name. When set, this reader
    /// retrieves a single row for this owner. When L""
    /// all objects in the database instance are read.
    );

    FdoSmPhRdOwnerReader(
        FdoSmPhMgrP         mgr,                // Physical Schema Manager
        FdoSmPhRowsP        rows,               // row and field layout
        FdoSmPhDatabaseP    database,           // get the owners for this database instance
        FdoStringP          ownerName = L""     // owner name. When set, this reader
    /// retrieves a single row for this owner. When L""
    /// all owners in the database instance are read.
    );

    /// Deactivates the reader.
	~FdoSmPhRdOwnerReader(void);

    /// Creates a set of rows describing the fields for this
    /// reader. There is one field per database object attribute.
    FdoSmPhRowsP MakeRows( FdoSmPhMgrP mgr );

    FdoSmPhDatabaseP mDatabase;
    FdoStringP mOwnerName;
};

typedef FdoPtr<FdoSmPhRdOwnerReader> FdoSmPhRdOwnerReaderP;

#endif


