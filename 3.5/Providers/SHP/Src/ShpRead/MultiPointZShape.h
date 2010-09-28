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

#ifndef MULTIPOINTZSHAPE_H
#define MULTIPOINTZSHAPE_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include "MultiPointMShape.h"
#include "MZData.h"

class MultiPointZShape : public MultiPointMShape
{
protected:
    typedef MultiPointMShape superclass;

public:
    MultiPointZShape (int nRecordNumber, void* pMemory, bool bOverlay, int nPoints = -1, BoundingBoxEx *box = NULL, bool has_m = false);
    static MultiPointZShape* NewMultiPointZShape (int nRecordNumber, int nPoints, BoundingBoxEx *box = NULL, bool has_m = false);
    virtual ~MultiPointZShape ();

    void GetBoundingBoxEx (BoundingBoxEx& box);
    virtual FdoByteArray* GetGeometry ();
    static int GetSize (int nPoints, bool has_m);
    virtual int GetContentLength ();
    virtual void DebugPrintDetails ();

    virtual MZData* GetZData ();
    virtual bool HasMeasure ();

protected:
    MZData mZData;
    bool mHasM;
};

inline void MultiPointZShape::GetBoundingBoxEx (BoundingBoxEx& box)
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

inline int MultiPointZShape::GetSize (int nPoints, bool has_m)
{
    return (
        superclass::GetSize (nPoints)
        + (has_m ? (
            // bounding z range
            (2 * sizeof (double))
            // z values
            + (nPoints * sizeof (double))) : 0));
}

inline int MultiPointZShape::GetContentLength ()
{
    return (GetSize (GetNumPoints (), HasMeasure ()) / WORD_SIZE_IN_BYTES);
}

inline MZData* MultiPointZShape::GetZData ()
{
    return (&mZData);
}

inline bool MultiPointZShape::HasMeasure ()
{
    return (mHasM);
}

#endif // MULTIPOINTZSHAPE_H

