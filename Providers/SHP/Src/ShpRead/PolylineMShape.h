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

#ifndef POLYLINEMSHAPE_H
#define POLYLINEMSHAPE_H

#include "PolylineShape.h"
#include "MZData.h"

class PolylineMShape : public PolylineShape
{
protected:
    typedef PolylineShape superclass;

public:
    PolylineMShape (int nRecordNumber, void* pMemory, bool bOverlay, void* pMValues = NULL, int nParts = -1, int nPoints = -1, BoundingBoxEx *box = NULL, bool initialize_m = true);
    static PolylineMShape* NewPolylineMShape (int nRecordNumber, int nParts, int nPoints, BoundingBoxEx *box = NULL, bool initialize_m = true);
    virtual ~PolylineMShape ();

    virtual void GetBoundingBoxEx (BoundingBoxEx& box, bool hasM);
    virtual void GetBoundingBoxEx (BoundingBoxEx& box);
    virtual FdoByteArray* GetGeometry ();
    static int GetSize (int nParts, int nPoints);
    virtual int GetContentLength ();
    virtual void DebugPrintDetails ();

    virtual MZData* GetMData ();
    virtual bool HasMeasure ();

protected:
    MZData mMData;
};

inline void PolylineMShape::GetBoundingBoxEx (BoundingBoxEx& box, bool hasM)
{
    superclass::GetBoundingBoxEx (box);
    if (hasM)
    {
        box.mMin = GetMData ()->GetRangeMin ();
        box.mMax = GetMData ()->GetRangeMax ();
    }
}

inline void PolylineMShape::GetBoundingBoxEx (BoundingBoxEx& box)
{
    GetBoundingBoxEx (box, true);
}

inline int PolylineMShape::GetSize (int nParts, int nPoints)
{
    return (
        superclass::GetSize (nParts, nPoints)
        //  bounding measure range
        + (2 * sizeof (double))
        // measures
        + (nPoints * sizeof (double)));
}

inline int PolylineMShape::GetContentLength ()
{
    return (GetSize (GetNumParts (), GetNumPoints ()) / WORD_SIZE_IN_BYTES);
}

inline MZData* PolylineMShape::GetMData ()
{
    return (&mMData);
}

inline bool PolylineMShape::HasMeasure ()
{
    return (true);
}

#endif // POLYLINEMSHAPE_H

