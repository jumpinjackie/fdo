#ifndef FDOSMPHSPATIALCONTEXTGROUPREADER_H
#define FDOSMPHSPATIALCONTEXTGROUPREADER_H		1
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

#include <Sm/Ph/Reader.h>

// Retrieves all spatial contexts for the current datastore. 

class FdoSmPhSpatialContextGroupReader :
	public FdoSmPhReader
{
public:
    /// Create and execute a query to get the spatial context groups.
    /// Call ReadNext() to read each spatial context group.
    /// Spatial context groups are ordered by numeric identifier.
	//
    /// Parameters:
    /// 	mgr: holds the connection for the datastore
    /// 		from which to retrieve the spatial context groups.
	FdoSmPhSpatialContextGroupReader(FdoSmPhMgrP mgr);

    /// Deactivate the reader
	~FdoSmPhSpatialContextGroupReader(void);

    /// Various getters

    /// Spatial Context Group ID - internal to Provider
	FdoInt64 GetId();

    /// Coordinate Reference System name
	FdoStringP GetCrsName();

    /// Coordinate Reference System definition in OpenGIS WKT format
	FdoStringP GetCrsWkt();

    /// Spatial Reference ID (if not zero, must agree with CrsName).
	FdoInt64 GetSrid();

    /// Tolerances and extents
    double GetXTolerance();
    double GetZTolerance();
    double GetXMin();
    double GetYMin();
    double GetZMin();
    double GetXMax();
    double GetYMax();
    double GetZMax();

    /// Extent Type -- immutable 'S' (static) or updatable 'D' (dynamic)
    FdoStringP GetExtentType();

protected:
    /// Unused constructor needed only to build on Linux
    FdoSmPhSpatialContextGroupReader() {}


    /// Top level function for creating inner reader
    FdoSmPhReaderP MakeReader( FdoSmPhMgrP mgr );

    /// Create inner reader for MetaSchema
    FdoSmPhReaderP MakeMtReader( FdoSmPhRowsP rows, FdoSmPhMgrP mgr );

    /// Create inner reader for Physical Schema (When MetaSchema not present).
    FdoSmPhReaderP MakeRdReader( FdoSmPhRowsP rows, FdoSmPhMgrP mgr );

private:

};

typedef FdoPtr<FdoSmPhSpatialContextGroupReader> FdoSmPhSpatialContextGroupReaderP;

#endif


