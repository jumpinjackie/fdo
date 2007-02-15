#ifndef FDOSMPHRDBASEOBJECTREADER_H
#define FDOSMPHRDBASEOBJECTREADER_H		1
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
#include <Sm/Ph/DbObjType.h>
#include <Sm/Ph/Mgr.h>

// This abstract class reads base objects. Each provider must 
// provide its own implementing class.
// A base object is a database object that another database object
// depends on. Typically a view can be based on a table or another view.

class FdoSmPhRdBaseObjectReader : public FdoSmPhReader
{
public:

protected:
    //Unused constructor needed only to build on Linux
    FdoSmPhRdBaseObjectReader() {}

    // Creates a reader for all base object references in the given owner
	//
	FdoSmPhRdBaseObjectReader(
        FdoSmPhReaderP reader,      // inner reader created by implementing class
        FdoSmPhOwnerP owner        // get the base objects for this owner
    );

    // Creates a reader for all base objects for a given database object
	//
	FdoSmPhRdBaseObjectReader(
        FdoSmPhReaderP reader,      // inner reader created by implementing class
        FdoSmPhDbObjectP dbObject       // get the base objects for this database object
    );

    /// Deactivates the reader.
	virtual ~FdoSmPhRdBaseObjectReader(void);

    /// Creates a set of rows describing the fields for this
    /// reader. There is one field per base object attribute.
    virtual FdoSmPhRowsP MakeRows( FdoSmPhMgrP mgr );

    FdoSmPhOwnerP mOwner;
    FdoSmPhDbObjectP mDbObject;
};

typedef FdoPtr<FdoSmPhRdBaseObjectReader> FdoSmPhRdBaseObjectReaderP;

#endif


