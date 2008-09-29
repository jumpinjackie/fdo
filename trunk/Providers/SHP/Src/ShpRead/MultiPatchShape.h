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

#ifndef MULTIPATCHSHAPE_H
#define MULTIPATCHSHAPE_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include "PolygonZShape.h"

struct DoublePoint;

class MultiPatchShape : public PolygonZShape
{
protected:
    typedef PolygonZShape superclass;

private:
    // The order of parts that are rings in the points array is significant: Inner Rings must
    // follow their Outer Ring; a sequence of Rings representing a single surface patch must
    // start with a ring of the type First Ring.
    // So we use a three state machine:
    typedef enum State
    {
        NOT_IN_RING_COMPLEX = 0,
        IN_OUTER_RING_COMPLEX = 1,
        IN_FIRST_RING_COMPLEX = 2,
    };

public:
    MultiPatchShape (int nRecordNumber, void* pMemory, bool bOverlay, int nParts = -1, int nPoints = -1, BoundingBoxEx* box = NULL, bool has_m = false);
    static MultiPatchShape* NewMultiPatchShape (int nRecordNumber, int nParts, int nPoints, BoundingBoxEx* box = NULL, bool has_m = false);
    virtual ~MultiPatchShape ();

    virtual FdoByteArray* GetGeometry ();
    static int GetSize (int nParts, int nPoints, bool has_m);
    virtual int GetContentLength ();
    virtual void DebugPrintDetails ();

    virtual int* GetPartTypes ();
    virtual void SetPartTypes (int* pPartTypes);

protected:
    int* mPartTypes;

private:
    void FinishPolygon (FdoILinearRing*& finalOuterRing, FdoLinearRingCollection* finalInnerRings, State& state, FdoLinearRingCollection* rings);
    void AddRings (FdoILinearRing*& finalOuterRing, FdoLinearRingCollection* finalInnerRings, FdoILinearRing* outerRing, FdoLinearRingCollection* innerRings);
};

inline int MultiPatchShape::GetSize (int nParts, int nPoints, bool has_m)
{
    return (
        superclass::GetSize (nParts, nPoints, has_m)
        // part types
        + (nParts * sizeof (int)));
}

inline int MultiPatchShape::GetContentLength ()
{
    return (GetSize (GetNumParts (), GetNumPoints (), HasMeasure ()) / WORD_SIZE_IN_BYTES);
}

inline int* MultiPatchShape::GetPartTypes ()
{
    return (mPartTypes);
}

inline void MultiPatchShape::SetPartTypes (int* pPartTypes)
{
    mPartTypes = pPartTypes;
}

inline void MultiPatchShape::AddRings(FdoILinearRing*& finalOuterRing, FdoLinearRingCollection* finalInnerRings, FdoILinearRing* outerRing, FdoLinearRingCollection* innerRings)
{
    // The old approach would potentially generate multipolygons; ECO 10400 says we can only output polygons:
    /*
    FdoPtr<FdoIPolygon> polygon = factory->CreatePolygon (ring, rings);
    polygons->Add (polygon);
    */

    // Add outerRing:
    if (finalOuterRing == NULL)
        finalOuterRing = FDO_SAFE_ADDREF(outerRing);
    else
        finalInnerRings->Add(outerRing);

    // Add innerRings:
    for (int i=0; i<innerRings->GetCount(); i++)
    {
        finalInnerRings->Add(FdoPtr<FdoILinearRing>(innerRings->GetItem(i)));
    }
}

#endif // MULTIPATCHSHAPE_H

