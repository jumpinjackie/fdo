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

#ifndef POLYGONSHAPE_H
#define POLYGONSHAPE_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include "PolyShape.h"
#include "Constants.h"

class PolygonShape : public PolyShape
{
protected:
    typedef PolyShape superclass;

public:
    PolygonShape (int nRecordNumber, void* pMemory, bool bOverlay, int nParts = -1, int nPoints = -1, BoundingBoxEx *box = NULL);
    static PolygonShape* NewPolygonShape (int nRecordNumber, int nParts, int nPoints, BoundingBoxEx *box = NULL);
    virtual ~PolygonShape ();

    virtual BoundingBox* GetBoundingBox ();
    virtual void GetBoundingBoxEx (BoundingBoxEx& box);
    virtual int GetNumPoints ();
    static int GetSize (int nParts, int nPoints);
    virtual int GetContentLength ();
    virtual FdoByteArray* GetGeometry ();
    virtual void DebugPrintDetails ();

    virtual int GetNumParts ();

    FdoIGeometry* CreateGeometryFromRings( FdoLinearRingCollection* rings, bool relateRings );
    bool PointInRing(FdoILinearRing* ring, double x, double y);

protected:
    ESRIPolygonRecord* mRecord;
};

inline int PolygonShape::GetSize (int nParts, int nPoints)
{
    return (
    	// shape type 
        sizeof (int)
        // bounding box
        + (4 * sizeof (double))
        // number of parts
        + sizeof (int)
        // number of points
        + sizeof (int)
        // index to first point in part
        + (nParts * sizeof (int))
        // points for all parts
        + (nPoints * 2 * sizeof (double)));
}

inline int PolygonShape::GetContentLength ()
{
    return (GetSize (GetNumParts (), GetNumPoints ()) / WORD_SIZE_IN_BYTES);
}

inline BoundingBox* PolygonShape::GetBoundingBox ()
{
    return (&mRecord->boundingBox);
}

inline void PolygonShape::GetBoundingBoxEx (BoundingBoxEx& box)
{
    box = *GetBoundingBox ();
}

inline int PolygonShape::GetNumPoints ()
{
    return (mRecord->nNumPoints);
}

inline int PolygonShape::GetNumParts ()
{
    return (mRecord->nNumParts);
}

#endif // POLYGONSHAPE_H

