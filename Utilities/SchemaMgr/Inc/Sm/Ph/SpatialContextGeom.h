#ifndef FDOSMPHSPATIALCONTEXTGEOM_H
#define FDOSMPHSPATIALCONTEXTGEOM_H		1
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

#include <Sm/Ph/SpatialContext.h>

class FdoSmPhOwner;

// This physical class encapsulates information on spatial context geometries
class FdoSmPhSpatialContextGeom : public FdoSmPhSchemaElement
{
public:

    /// Constructs an instance from the given attributes.
	FdoSmPhSpatialContextGeom(
        const FdoSmPhOwner*     pOwner,
        FdoString*	            tableName,
        FdoString*	            columnName,
		bool                    hasElevation,
        bool                    hasMeasure,
        FdoInt32                geometryType,
        bool                    isDerived,
        FdoSmPhSpatialContextP  spatialContext,
        FdoSmPhSpatialContextsP spatialContexts
    ) :
        FdoSmPhSchemaElement( MakeName(tableName,columnName), L"", (FdoSmPhMgr*) NULL, (const FdoSmPhSchemaElement*) pOwner )
	{
		mTableName = tableName;
		mColumnName = columnName;
		mHasElevation = hasElevation;	
		mHasMeasure = hasMeasure;	
        mGeometryType = geometryType;
        mIsDerived = isDerived;
        mpOwner = (FdoSmPhOwner*) pOwner;
        mSpatialContext = spatialContext;
        mSpatialContexts = spatialContexts;
	}

	~FdoSmPhSpatialContextGeom(void) {}

    /// Geometry table name
	FdoStringP GetGeomTableName() { return mTableName; }

    /// Geometry column name
	FdoStringP GetGeomColumnName() { return mColumnName; }

    /// Ordinate dimensionality of geometries.
    bool GetHasElevation() { return mHasElevation; }

    bool GetHasMeasure() { return mHasMeasure; }

    // Allowed geometry types in hexcode format.
    FdoInt32 GetGeometryType() { return mGeometryType; }

    // Generates a unique name for this spatial context geometry.
    // The name is unique with owners (datastores).
    static FdoStringP MakeName( FdoString* tableName, FdoString* columnName ) 
    {
        return FdoStringP::Format( L"\"%ls\".\"%ls\"", tableName, columnName );
    }

    // Gets the associated spatial context
    FdoSmPhSpatialContextP GetSpatialContext();

protected:
    /// Unused constructor needed only to build on Linux
    FdoSmPhSpatialContextGeom() {}

	virtual void Dispose() { delete this; }

    // Resolves the associated spatial context. Spatial Context passed 
    // into constructor is a candidate. This function resolves the following:
    //    - Coalesces spatial contexts with identical attributes
    //    - if geometric column is derived then set spatial context to the 
    //      spatial context of the base column.
    virtual void Finalize();

private:

    FdoStringP		        mTableName;
    FdoStringP		        mColumnName;
	bool			        mHasElevation;
	bool			        mHasMeasure;
    bool                    mIsDerived;
    FdoInt32                mGeometryType;
    FdoSmPhOwner*           mpOwner;
    FdoSmPhSpatialContextP  mSpatialContext;
    FdoSmPhSpatialContextsP mSpatialContexts;
};

typedef FdoPtr<FdoSmPhSpatialContextGeom> FdoSmPhSpatialContextGeomP;


// Represents a collection of Spatial Contexts in Logical/Physical form.
class FdoSmPhSpatialContextGeomCollection : public FdoSmNamedCollection<FdoSmPhSpatialContextGeom>
{
public:
    /// Create an empty collection.
	FdoSmPhSpatialContextGeomCollection(void) {}

};

typedef FdoPtr<FdoSmPhSpatialContextGeomCollection> FdoSmPhSpatialContextGeomsP;

#endif


