#ifndef FDOSMPHRDPKEYREADER_H
#define FDOSMPHRDPKEYREADER_H		1
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

// This abstract class a primary key. It retrieves one row per primaryKey-column
// combination. Each provider must provide its own implementing class.


class FdoSmPhRdPkeyReader : public FdoSmPhRdReader
{
protected:
    //Unused constructor needed only to build on Linux
    FdoSmPhRdPkeyReader() {}

    /// Create a primary key reader
	//
	FdoSmPhRdPkeyReader(
        FdoSmPhReaderP reader    // inner reader created by implementing class
    );

	FdoSmPhRdPkeyReader(
        FdoSmPhMgrP         mgr,                // Physical Schema Manager
        FdoSmPhRowsP        rows                // row and field layout
    );

    /// Deactivates the reader.
	~FdoSmPhRdPkeyReader(void);

    /// Creates a set of rows describing the fields for this
    /// reader. There is one field per primary key and primary key column 
    /// attribute.
    virtual FdoSmPhRowsP MakeRows( FdoSmPhMgrP mgr );

};

typedef FdoPtr<FdoSmPhRdPkeyReader> FdoSmPhRdPkeyReaderP;

#endif


