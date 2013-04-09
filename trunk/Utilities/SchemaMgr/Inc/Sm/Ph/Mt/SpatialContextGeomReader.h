#ifndef FDOSMPHMTSPATIALCONTEXTGEOMREADER_H
#define FDOSMPHMTSPATIALCONTEXTGEOMREADER_H		1
/*
 * Copyright (C) 2006  Autodesk, Inc.
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

#include <Sm/Ph/Reader.h>
#include <Sm/Ph/Mgr.h>
#include <Sm/Ph/RowCollection.h>

// Retrieves all persisted spatial context associations to 
// geometric properties for a given datastore. 

class FdoSmPhMtSpatialContextGeomReader :
	public FdoSmPhReader
{
public:
    /// Create a reader to get the association.
    /// Call ReadNext() to read each one.
	//
	FdoSmPhMtSpatialContextGeomReader( 
        FdoSmPhRowsP froms,         // defines the fields to read
        FdoSmPhMgrP mgr             // Physical Schema Manager
    );

    /// Deactivate the reader
	~FdoSmPhMtSpatialContextGeomReader(void);

private:
    /// Helper for creating inner query reader
    FdoSmPhReaderP MakeReader( FdoSmPhRowsP froms, FdoSmPhMgrP mgr );
};

typedef FdoPtr<FdoSmPhMtSpatialContextGeomReader> FdoSmPhMtSpatialContextGeomReaderP;

#endif


