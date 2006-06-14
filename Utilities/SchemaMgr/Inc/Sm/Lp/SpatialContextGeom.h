#ifndef FDOSMLPSPATIALCONTEXTGEOM_H
#define FDOSMLPSPATIALCONTEXTGEOM_H		1
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

#include <Sm/Collection.h>

// This logical/physical class encapsulates information on spatial context geometries
class FdoSmLpSpatialContextGeom : public FdoIDisposable
{
	friend class FdoSmLpSpatialContextCollection;

    /// Constructs an instance from the given attributes.
	FdoSmLpSpatialContextGeom(
        FdoInt64	scId,
        FdoString*	tableName,
        FdoString*	columnName,
		int			dimensionality
    )
	{
		mScId = scId;
		mTableName = tableName;
		mColumnName = columnName;
		mDimensionality = dimensionality;	
	}

public:
	~FdoSmLpSpatialContextGeom(void) {}

    /// Spatial Context Identifier
	FdoInt64 GetScId() { return mScId; }

    /// Geometry table name
	FdoStringP GetGeomTableName() { return mTableName; }

    /// Geometry column name
	FdoStringP GetGeomColumnName() { return mColumnName; }

    /// Ordinate dimensionality of geometries.
    FdoInt32 GetDimensionality() { return mDimensionality; }


protected:
    /// Unused constructor needed only to build on Linux
    FdoSmLpSpatialContextGeom() {}

	virtual void Dispose() { delete this; }

private:

    FdoInt64        mScId;
    FdoStringP		mTableName;
    FdoStringP		mColumnName;
	int				mDimensionality;

};

typedef FdoPtr<FdoSmLpSpatialContextGeom> FdoSmLpSpatialContextGeomP;


// Represents a collection of Spatial Contexts in Logical/Physical form.
class FdoSmLpSpatialContextGeomCollection : public FdoSmCollection<FdoSmLpSpatialContextGeom>
{
public:
    /// Create an empty collection.
	FdoSmLpSpatialContextGeomCollection(void) {}

};

typedef FdoPtr<FdoSmLpSpatialContextGeomCollection> FdoSmLpSpatialContextGeomsP;

#endif


