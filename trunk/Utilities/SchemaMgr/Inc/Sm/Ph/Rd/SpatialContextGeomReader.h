#ifndef FDOSMPHRDSPATIALCONTEXTGEOMREADER_H
#define FDOSMPHRDSPATIALCONTEXTGEOMREADER_H		1
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

#include <Sm/Ph/Reader.h>
#include <Sm/Ph/Mgr.h>
#include <Sm/Ph/RowCollection.h>

// This reader reads all of the spatial context associations to 
// geometric properties for a datastore, when no MetaSchema
// information is present. It reads any relevant spatial metadata in the datastore and 
// automatically wraps a spatial context association around it.
// If the database contains no spatial metadata, defaults are used.

class FdoSmPhRdSpatialContextGeomReader : public FdoSmPhReader
{
public:
    /// Create and execute the reader
	FdoSmPhRdSpatialContextGeomReader(
        FdoSmPhMgrP mgr             // Physical Schema Manager
    );

    /// Deactivates the query.
	~FdoSmPhRdSpatialContextGeomReader(void);

    /// Geometry table name
	virtual FdoStringP GetGeomTableName();

    /// Geometry column name
	virtual FdoStringP GetGeomColumnName();

    /// Ordinate dimensionality of geometries.
    virtual FdoInt32 GetDimensionality();

    /// Gets the name of the coordinate system of the spatial context currently
    /// being read. If the result is a null or empty string, the coordinate system is
    /// unknown to the spatial context.
    virtual FdoString* GetCoordinateSystem();

    /// Gets the coordinate system in OpenGIS SRS WKT format of the spatial context currently
    /// being read. If the result is an empty string, the coordinate system WKT description is
    /// not available.
    virtual FdoString* GetCoordinateSystemWkt();

    /// Spatial Reference ID (if not zero, must agree with CrsName).
	virtual FdoInt64 GetSrid();

    /// Gets the extent of the spatial context currently being read as a byte
    /// array in AGF format.
    virtual FdoByteArray* GetExtent();

    /// Gets the tolerance value for XY ordinates of the spatial context
    /// currently being read. 
    virtual const double GetXYTolerance();

     /// Gets the tolerance value for Z ordinates of the spatial context
    /// currently being read. 
    virtual const double GetZTolerance();

    /// Get the next row and make it current.
	virtual bool ReadNext();


protected:
	FdoSmPhRdSpatialContextGeomReader();

	virtual void Dispose() { delete this; }

	FdoStringP	mGeomTableName;
	FdoStringP	mGeomColumnName;
    FdoInt32	mDimensionality;

	FdoStringP	mCSname;
	FdoStringP	mWKT;
	FdoInt64	mSrid;
	double		mTolXY;
	double		mTolZ;

	FdoPtr<FdoByteArray> mExtents;

private:
};

// convenient smart pointer
typedef FdoPtr<FdoSmPhRdSpatialContextGeomReader> FdoSmPhRdSpatialContextGeomReaderP;

#endif


