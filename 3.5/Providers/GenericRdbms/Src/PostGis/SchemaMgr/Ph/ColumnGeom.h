//
// Copyright (C) 2006 Refractions Research, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
#ifndef FDOSMPHPOSTGISCOLUMNGEOM_H
#define FDOSMPHPOSTGISCOLUMNGEOM_H

#include <Inc/rdbi.h>
#include <Sm/Ph/ColumnGeom.h>
#include "Column.h"

/// Represents a geometric column type for the PostGIS provider.
///
class FdoSmPhPostGisColumnGeom :
    public FdoSmPhColumnGeom,
    public FdoSmPhPostGisColumn
{
public:
    // Default Constructor (Used for Linux compilation)
    FdoSmPhPostGisColumnGeom() {}

    FdoSmPhPostGisColumnGeom(FdoStringP columnName,
        FdoSchemaElementState state,
        FdoSmPhDbObject* parentObject,
        FdoSmPhScInfoP associatedSCInfo,
        bool isNullable = true,
        bool hasElevation = false,
        bool hasMeasure = false,
        FdoStringP rootColumnName = L"",
        FdoSmPhRdColumnReader* reader = NULL);

    /// Destructor.
    virtual ~FdoSmPhPostGisColumnGeom();

    /// Returns RDBI constant for byte type.
    /// RDBI type of this column type is RDBI_GEOMETRY.
    virtual int GetRdbType();

    virtual FdoInt64 GetSRID();

    virtual FdoStringP GetBestFdoType();

    virtual FdoStringP GetBestFdoGeometryType();

    // Allowed geometry types in hexcode format.
    FdoInt32 GetGeometryType();

protected:

    long mFdoGeomType;

    long mFdoGeometryType;

    /// 
    virtual void PostFinalize();

    virtual FdoStringP GetAddSql();

    FdoPtr<FdoSmPhSpatialIndex> CreateSpatialIndex( FdoStringP indexName = L"" );

    virtual bool Add();

private:
    // Load SRID and allowed geometry types from Spatial Context Geometry
    // association.
    void LoadScGeom();

   	FdoInt64 mSRID;

    bool mScGeomLoaded;
};

/// \brief
/// Declaration of smart-pointer type.
/// Provided for convenience.
/// 
typedef FdoPtr<FdoSmPhPostGisColumnGeom> FdoSmPhPostGisColumnGeomP;

#endif // FDOSMPHPOSTGISCOLUMNGEOM_H
