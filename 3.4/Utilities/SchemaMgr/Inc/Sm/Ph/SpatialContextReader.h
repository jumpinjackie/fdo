#ifndef FDOSMPHSPATIALCONTEXTREADER_H
#define FDOSMPHSPATIALCONTEXTREADER_H		1
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
 */

#ifdef _WIN32
#pragma once
#endif

#include <Sm/Ph/Reader.h>

// Retrieves all spatial contexts for the current datastore. 

class FdoSmPhSpatialContextReader :
	public FdoSmPhReader
{
public:
    /// Create and execute a query to get the spatial contexts.
    /// Call ReadNext() to read each spatial context.
    /// Spatial contexts are ordered by name.
	//
    /// Parameters:
    /// 	mgr: holds the connection for the datastore
    /// 		from which to retrieve the spatial contexts.
	FdoSmPhSpatialContextReader(FdoSmPhMgrP mgr);

    /// Deactivate the reader
	~FdoSmPhSpatialContextReader(void);

    /// Various getters

    /// Spatial Context ID - internal to Provider
	FdoInt64 GetId();

    /// Spatial Context name
	FdoStringP GetName();

    /// Class description
	FdoStringP GetDescription();

    /// Spatial Context Group ID - internal to Provider
	FdoInt64 GetGroupId();

protected:
    /// Unused constructor needed only to build on Linux
    FdoSmPhSpatialContextReader() {}


    /// Top level function for creating inner reader
    FdoSmPhReaderP MakeReader( FdoSmPhMgrP mgr );

    /// Create inner reader for MetaSchema
    FdoSmPhReaderP MakeMtReader( FdoSmPhRowsP rows, FdoSmPhMgrP mgr );

    /// Create inner reader for Physical Schema (When MetaSchema not present).
    FdoSmPhReaderP MakeRdReader( FdoSmPhMgrP mgr );

private:

};

typedef FdoPtr<FdoSmPhSpatialContextReader> FdoSmPhSpatialContextReaderP;

#endif


