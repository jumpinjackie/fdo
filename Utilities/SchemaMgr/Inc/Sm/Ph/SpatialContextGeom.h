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

#include <Sm/Collection.h>

// This physical class encapsulates information on spatial context geometries
class FdoSmPhSpatialContextGeom : public FdoSmPhSchemaElement
{
public:

    /// Constructs an instance from the given attributes.
	FdoSmPhSpatialContextGeom(
        FdoSmPhMgrP phMgr,
        FdoInt64	scId,
        FdoString*	tableName,
        FdoString*	columnName,
		bool        hasElevation,
        bool        hasMeasure
    ) :
        FdoSmPhSchemaElement( MakeName(tableName,columnName), L"", phMgr )
	{
		mScId = scId;
		mTableName = tableName;
		mColumnName = columnName;
		mHasElevation = hasElevation;	
		mHasMeasure = hasMeasure;	
	}

	~FdoSmPhSpatialContextGeom(void) {}

    /// Spatial Context Identifier
	FdoInt64 GetScId() { return mScId; }

    /// Geometry table name
	FdoStringP GetGeomTableName() { return mTableName; }

    /// Geometry column name
	FdoStringP GetGeomColumnName() { return mColumnName; }

    /// Ordinate dimensionality of geometries.
    bool GetHasElevation() { return mHasElevation; }

    bool GetHasMeasure() { return mHasMeasure; }

    // Generates a unique name for this spatial context geometry.
    // The name is unique with owners (datastores).
    static FdoStringP MakeName( FdoString* tableName, FdoString* columnName ) 
    {
        return FdoStringP::Format( L"\"%ls\".\"%ls\"", tableName, columnName );
    }

protected:
    /// Unused constructor needed only to build on Linux
    FdoSmPhSpatialContextGeom() {}

	virtual void Dispose() { delete this; }

private:

    FdoInt64        mScId;
    FdoStringP		mTableName;
    FdoStringP		mColumnName;
	bool			mHasElevation;
	bool			mHasMeasure;

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


