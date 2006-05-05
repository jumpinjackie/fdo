#ifndef FDOSMPHRDSPATIALCONTEXTREADER_H
#define FDOSMPHRDSPATIALCONTEXTREADER_H		1
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
#include <Sm/Ph/Mgr.h>
#include <Sm/Ph/RowCollection.h>

// This reader reads all of the spatial contexts for a datastore, when no MetaSchema
// information is present. It reads any relevant spatial metadata in the datastore and 
// automatically wraps a spatial context around it.
// If the database contains no spatial metadata, defaults are used.

class FdoSmPhRdSpatialContextReader : public FdoSmPhReader
{
public:
    /// Create and execute the reader
	FdoSmPhRdSpatialContextReader(
        FdoSmPhRowsP froms,         // rows and fields to retrieve.
        FdoSmPhMgrP mgr             // Physical Schema Manager
    );

    /// Deactivates the query.
	~FdoSmPhRdSpatialContextReader(void);

    /// Get the next row and make it current.
	virtual bool ReadNext();


protected:
private:
};

// convenient smart pointer
typedef FdoPtr<FdoSmPhRdSpatialContextReader> FdoSmPhRdSpatialContextReaderP;

#endif


