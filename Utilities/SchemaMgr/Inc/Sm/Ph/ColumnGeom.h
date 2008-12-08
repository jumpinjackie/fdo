#ifndef FDOSMPHCOLUMNGEOM_H
#define FDOSMPHCOLUMNGEOM_H		1
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

#include <Sm/Ph/Column.h>
#include <FdoCommonGeometryUtil.h>

class FdoSmPhSpatialIndex;
class FdoSmPhSpatialContext;

// Describes spatial context information without depending on
// logical/physical types.  Some overrides of FdoSmPhColumn may
// need spatial parameters.  If more parameters are needed,
// they can be added here without affecting method signatures.
struct FdoSmPhScInfo : FdoDisposable
{
    FdoInt64                    mSrid;
    FdoStringP                  mCoordSysName;
    FdoPtr<FdoByteArray>        mExtent;
    double                      mXYTolerance;
    double                      mZTolerance;

    FdoSmPhScInfo() : mSrid(-1), mXYTolerance(-1.0), mZTolerance(-1.0) {}

    static FdoSmPhScInfo * Create()
    {
        FdoSmPhScInfo * newObject = new FdoSmPhScInfo();
        if (NULL == newObject)
			throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));
        return newObject;
    }
};
typedef FdoPtr<FdoSmPhScInfo>   FdoSmPhScInfoP;


// This class represents a geometry type column
class FdoSmPhColumnGeom :
	public virtual FdoSmPhColumn
{
public:
    /// Creates a new Geometry Column Definition.
	//
    /// Parameters:
    /// 	bHasElevation: true if the column geometries can have a Z dimension.
    ///      bHasMeasure: true if the column geometries can have a measurement dimension.
    FdoSmPhColumnGeom(
        FdoSmPhScInfoP AssociatedSCInfo,
        bool bHasElevation = true, 
        bool bHasMeasure = false
	);

	virtual ~FdoSmPhColumnGeom(void);

    virtual FdoSmPhColType GetType()
    {
        return FdoSmPhColType_Geom;
    }

    /// returns whether this column type has elevations
    bool GetHasElevation()
    {
        Finalize();

        return mbHasElevation;
    }

    /// returns whether this column type has a measure dimension
    bool GetHasMeasure()
    {
        Finalize();

        return mbHasMeasure;
    }

    /// returns the associated Spatial Context info
    FdoSmPhScInfoP	GetSpatialContextInfo();

    /// Sets the associated Spatial Context info
    void	SetSpatialContextInfo(FdoSmPhScInfoP scInfo)
    {
        mScInfo = scInfo;
    }

    /// returns the associated Spatial Context
    FdoPtr<FdoSmPhSpatialContext> GetSpatialContext();

    // Returns:
    //  true: if this column is the main geometry for its table or view
    //  false: if it is not the main geometry
    bool    GetPrimary()
    {
        return mbIsPrimary;
    }

    // Modifies the main geometry status for this column
    void    SetPrimary( bool isPrimary );

    /// returns DBI-format column dimensionality, based on whether the column
    /// has elevation or measure dimensions.
    /// See inc/dbi/coord_ty.h for the list of values that can be returned.
    virtual int GetDimensionality();

    virtual FdoStringP GetBestFdoType()
    {
    /// Set to all types (except solid which is rare in GIS applications)
        long geomTypes = FdoGeometricType_Point | FdoGeometricType_Curve | FdoGeometricType_Surface;

        return FdoStringP::Format(L"%d", geomTypes);
    }

    virtual FdoStringP GetBestFdoGeometryType()
    {
    /// Set to all types (except solid which is rare in GIS applications)
        long geomTypes = FdoCommonGeometryUtil::GetAllGeometryTypesCode();

        return FdoStringP::Format(L"%d", geomTypes);
    }

    /// Post column modifications to the database.
	virtual void Commit( bool fromParent = false, bool isBeforeParent = false );

    /// Gets the spatial index for this column
    FdoPtr<FdoSmPhSpatialIndex> GetSpatialIndex();

    /// Remove current spatial index without deleting it from datastore.
    void DiscardSpatialIndex( FdoSmPhSpatialIndex* index );

    // Regenerates this column's spatial index. Actual processing is provider-specific
    // so default implementation does nothing.
    virtual void    RegenSpatialIndex();

	virtual FdoInt64 GetSRID()
	{
		return 0;
	}

    // Returns true if this column has the same definition (type,dimensionality,srid,nullability)
    // as the given column.
    virtual bool DefinitionEquals( FdoSmPhColumnP otherColumn );

protected:

    // unused constructor needed only to build on Linux
    FdoSmPhColumnGeom() {}

    /// Set this column's spatial index
    void SetSpatialIndex( FdoPtr<FdoSmPhSpatialIndex> spatialIndex ); 

    void SetHasMeasure( bool hasMeasure ) 
    {
        mbHasMeasure = hasMeasure;
    }

    void SetHasElevation( bool hasElevation ) 
    {
        mbHasElevation = hasElevation;
    }

    /// Generate a unique spatial index name for this column
    virtual FdoStringP UniqueIndexName();

    void Finalize();

    virtual void PostFinalize();

private:

    bool mbHasElevation;
    bool mbHasMeasure;
    bool mbIsPrimary;
    FdoSmPhScInfoP mScInfo;
    FdoPtr<FdoSmPhSpatialContext> mSpatialContext;
    FdoSmPhSpatialIndex* mSpatialIndex;
};

typedef FdoPtr<FdoSmPhColumnGeom> FdoSmPhColumnGeomP;

#endif


