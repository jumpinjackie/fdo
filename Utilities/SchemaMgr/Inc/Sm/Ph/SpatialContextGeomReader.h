#ifndef FDOSMPHSPATIALCONTEXTGEOMREADER_H
#define FDOSMPHSPATIALCONTEXTGEOMREADER_H		1
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

// Retrieves all spatial context associations to geometric properties
// for the current datastore. 

class FdoSmPhSpatialContextGeomReader :
	public FdoSmPhReader
{
public:
    /// Create and execute a query to get the spatial context associations.
    /// Call ReadNext() to read each association.
	//
    /// Parameters:
    /// 	mgr: holds the connection for the datastore
    /// 		from which to retrieve the associations.
	FdoSmPhSpatialContextGeomReader(FdoSmPhMgrP mgr);

    /// Deactivate the reader
	~FdoSmPhSpatialContextGeomReader(void);

    /// Various getters

    /// Spatial Context Identifier
    FdoInt64 GetScId();

    /// Geometry table name
	FdoStringP GetGeomTableName();

    /// Geometry column name
	FdoStringP GetGeomColumnName();

    /// Ordinate dimensionality of geometries.
    FdoInt32 GetDimensionality();

protected:
    /// Unused constructor needed only to build on Linux
    FdoSmPhSpatialContextGeomReader() {}

    /// Top level function for creating inner reader
    FdoSmPhReaderP MakeReader( FdoSmPhMgrP mgr );

    /// Create inner reader for MetaSchema
    FdoSmPhReaderP MakeMtReader( FdoSmPhRowsP rows, FdoSmPhMgrP mgr );

    /// Create inner reader for Physical Schema (When MetaSchema not present).
    FdoSmPhReaderP MakeRdReader( FdoSmPhRowsP rows, FdoSmPhMgrP mgr );

private:

};

typedef FdoPtr<FdoSmPhSpatialContextGeomReader> FdoSmPhSpatialContextGeomReaderP;

#endif


