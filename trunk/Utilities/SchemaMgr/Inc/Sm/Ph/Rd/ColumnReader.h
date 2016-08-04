#ifndef FDOSMPHRDCOLUMNREADER_H
#define FDOSMPHRDCOLUMNREADER_H		1
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
#include <Sm/Ph/Mgr.h>
#include <Sm/Ph/RowCollection.h>
#include <Sm/Ph/Coltype.h>

// This abstract class reads all of the columns for a given database object
// (table, view, etc.). Each provider must provide its own implementing class.

class FdoSmPhRdColumnReader : public FdoSmPhRdReader
{
public:
    /// Get the type of the current column
    virtual FdoSmPhColType GetType() = 0;

protected:
    //Unused constructor needed only to build on Linux
    FdoSmPhRdColumnReader() {}

    /// Create and execute the column reader:

    /// with inner reader.
	FdoSmPhRdColumnReader(
        FdoSmPhReaderP      reader,    // inner reader created by implementing class
        FdoSmPhDbObjectP    dbObject   // read columns for this database object
    );

    FdoSmPhRdColumnReader(
        FdoSmPhMgrP         mgr,        // Physical Schema Manager
        FdoSmPhRowsP        rows,       // Rows containing fields for column attributes
        FdoSmPhDbObjectP    dbObject    // read columns for this database object
    );

    /// Deactivates the reader.
	~FdoSmPhRdColumnReader(void);

protected:
    /// Creates a set of rows describing the fields for this
    /// reader. There is one field per column attribute.
    virtual FdoSmPhRowsP MakeRows( FdoSmPhMgrP mgr );

private:

   FdoSmPhDbObjectP mDbObject;
};

typedef FdoPtr<FdoSmPhRdColumnReader> FdoSmPhRdColumnReaderP;

#endif


