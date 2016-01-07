#ifndef FDOSMPHRDCOORDSYSREADER_H
#define FDOSMPHRDCOORDSYSREADER_H		1
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
#include <Sm/Ph/Mgr.h>

// This abstract class reads coordinate systems from the RDBMS. 
// By default, it reads nothing.
// Providers that support native coordinate systems must override
// this class.

class FdoSmPhRdCoordSysReader : public FdoSmPhReader
{
public:
    //Default reader reads nothing.
    FdoSmPhRdCoordSysReader() {}

protected:
    /// Creates a Coordinate System reader from an underlying reader.
    //
	FdoSmPhRdCoordSysReader(
        FdoSmPhReaderP reader,      // inner reader created by implementing class
        FdoSmPhOwnerP owner,        // get the coordinate systems for this schema
        FdoStringP csysName = L""   // if L"" then retrieve all coordinate systems.
    /// if set then retrieve just this coordinate system.
        );

    FdoSmPhRdCoordSysReader(
        FdoSmPhMgrP         mgr,    // Physical Schema Manager
        FdoSmPhRowsP        rows,   // row and field layout
        FdoSmPhOwnerP       owner,  // get the coordinate systems for this schema
        FdoStringP csysName = L""   // if L"" then retrieve all coordinate systems.
    /// if set then retrieve just this coordinate system.
    );

    /// Deactivates the reader.
	virtual ~FdoSmPhRdCoordSysReader(void);

    /// Creates a set of rows describing the fields for this
    /// reader. There is one field per coordinate system attribute.
    virtual FdoSmPhRowsP MakeRows( FdoSmPhMgrP mgr );

    FdoSmPhOwnerP mOwner;
    FdoStringP mCsysName;
};

typedef FdoPtr<FdoSmPhRdCoordSysReader> FdoSmPhRdCoordSysReaderP;

#endif


