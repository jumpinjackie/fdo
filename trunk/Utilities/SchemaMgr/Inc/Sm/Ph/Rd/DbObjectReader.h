#ifndef FDOSMPHRDDBOBJECTREADER_H
#define FDOSMPHRDDBOBJECTREADER_H		1
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
#include <Sm/Ph/Rd/Reader.h>
#include <Sm/Ph/RowCollection.h>
#include <Sm/Ph/DbObjType.h>

// This abstract class reads database objects. Each provider must 
// provide its own implementing class.



class FdoSmPhRdDbObjectReader : public FdoSmPhRdReader
{
public:
    /// Get the type of the current database object.
    virtual FdoSmPhDbObjType GetType() = 0;

protected:
    //Unused constructor needed only to build on Linux
    FdoSmPhRdDbObjectReader() {}

    /// Creates a database object reader.
	//
	FdoSmPhRdDbObjectReader(
        FdoSmPhReaderP reader,      // inner reader created by implementing class
        FdoSmPhOwnerP owner,        // get the database objects for this schema
        FdoStringP objectName);

    FdoSmPhRdDbObjectReader(
        FdoSmPhMgrP         mgr,                // Physical Schema Manager
        FdoSmPhRowsP        rows,               // row and field layout
        FdoSmPhOwnerP       owner,              // get the database objects for this schema
        FdoStringP          objectName = L""    // database object name. When set, this reader
    /// retrieves a single row for this object. When L""
    /// all objects in the schema are read.
    );

    /// Deactivates the reader.
	virtual ~FdoSmPhRdDbObjectReader(void);

    /// Creates a set of rows describing the fields for this
    /// reader. There is one field per database object attribute.
    virtual FdoSmPhRowsP MakeRows( FdoSmPhMgrP mgr );

    FdoSmPhOwnerP mOwner;
    FdoStringP mObjectName;
};

typedef FdoPtr<FdoSmPhRdDbObjectReader> FdoSmPhRdDbObjectReaderP;

#endif


