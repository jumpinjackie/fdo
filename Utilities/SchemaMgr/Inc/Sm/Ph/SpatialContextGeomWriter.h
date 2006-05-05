#ifndef FDOSMPHSPATIALCONTEXTGEOMWRITER_H
#define FDOSMPHSPATIALCONTEXTGEOMWRITER_H		1
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

#include <Sm/Ph/Writer.h>
#include <Sm/Ph/Mgr.h>
#include <Sm/Ph/Row.h>

// This class writes spatial context associations 
// to the f_spatialcontextgeom table.
class FdoSmPhSpatialContextGeomWriter : public FdoSmPhWriter
{
public:

    /// Create a spatial context association writer.
	//
    /// Parameters:
    /// 	mgr: holds the connection for the datastore to query.
	FdoSmPhSpatialContextGeomWriter(FdoSmPhMgrP mgr);

	~FdoSmPhSpatialContextGeomWriter(void);
    
    /// Various field value getters

    /// Spatial Context Identifier
    FdoInt64 GetScId();

    /// Geometry table name
	FdoStringP GetGeomTableName();

    /// Geometry column name
	FdoStringP GetGeomColumnName();

    /// Ordinate dimensionality of geometries.
    FdoInt32 GetDimensionality();

    /// Corresponding setter functions.

    void SetScId( FdoInt64 iValue);

    void SetClassId( FdoInt64 iValue);

	void SetGeomTableName( FdoStringP sValue );

	void SetGeomColumnName( FdoStringP sValue );

    void SetDimensionality( FdoInt32 iValue);

    /// Add the current row as a new spatial context
	virtual void Add();

    /// Modify a spatial context association using the current field values.
    virtual void Modify( FdoStringP geomTableName, FdoStringP geomColumnName );

    /// Delete a spatial context using the current field values.
    virtual void Delete( FdoStringP geomTableName, FdoStringP geomColumnName );

    /// Get a field list for reading from or writing to 
    /// f_spatialcontextgeom.
	//
    /// Parameters:
    /// 	physicalSchema: holds the connection for the datastore to query.
    static FdoSmPhRowP MakeRow( FdoSmPhMgrP mgr );

protected:
    /// Unused constructor needed only to build on Linux
    FdoSmPhSpatialContextGeomWriter() {}

    FdoSmPhWriterP MakeWriter( FdoSmPhMgrP mgr );
};

typedef FdoPtr<FdoSmPhSpatialContextGeomWriter> FdoSmPhSpatialContextGeomWriterP;

#endif


