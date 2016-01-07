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

#ifndef POLYLINEZSHAPE_H
#define POLYLINEZSHAPE_H

#include "PolylineMShape.h"
#include "MZData.h"

class PolylineZShape : public PolylineMShape
{
protected:
    typedef PolylineMShape superclass;

public:
    PolylineZShape (int nRecordNumber, void* pMemory, bool bOverlay, int nParts = -1, int nPoints = -1, BoundingBoxEx *box = NULL, bool has_m = false);
    static PolylineZShape* NewPolylineZShape (int nRecordNumber, int nParts, int nPoints, BoundingBoxEx *box = NULL, bool has_m = false);
    virtual ~PolylineZShape ();

    virtual void GetBoundingBoxEx (BoundingBoxEx& box);
    virtual FdoByteArray* GetGeometry ();
    static int GetSize (int nParts, int nPoints, bool has_m);
    virtual int GetContentLength ();
    virtual void DebugPrintDetails ();

    virtual MZData* GetZData ();
    virtual bool HasMeasure ();

protected:
    MZData mZData;
    bool mHasM;
};

inline void PolylineZShape::GetBoundingBoxEx (BoundingBoxEx& box)
{
    superclass::GetBoundingBoxEx (box, mHasM);
    box.zMin = GetZData ()->GetRangeMin ();
    box.zMax = GetZData ()->GetRangeMax ();
    if (!HasMeasure ())
    {
        box.mMin = fNO_DATA;
        box.mMax = fNO_DATA;
    }
}

inline int PolylineZShape::GetSize (int nParts, int nPoints, bool has_m)
{
    return (
        superclass::GetSize (nParts, nPoints)
        + (has_m ? (
            //  bounding z range
            + (2 * sizeof (double))
            // z values
            + (nPoints * sizeof (double))) : 0));
}

inline int PolylineZShape::GetContentLength ()
{
    return (GetSize (GetNumParts (), GetNumPoints (), HasMeasure ()) / WORD_SIZE_IN_BYTES);
}

inline MZData* PolylineZShape::GetZData ()
{
    return (&mZData);
}

inline bool PolylineZShape::HasMeasure ()
{
    return (mHasM);
}

#endif // POLYLINEZSHAPE_H

