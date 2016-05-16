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

#ifndef MULTIPOINTMSHAPE_H
#define MULTIPOINTMSHAPE_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include "MultiPointShape.h"
#include "MZData.h"

class MultiPointMShape : public MultiPointShape
{
protected:
    typedef MultiPointShape superclass;

public:
    MultiPointMShape (int nRecordNumber, void* pMemory, bool bOverlay, void* pMValues = NULL, int nPoints = -1, BoundingBoxEx *box = NULL, bool has_m = false);
    static MultiPointMShape* NewMultiPointMShape (int nRecordNumber, int nPoints, BoundingBoxEx *box = NULL, bool has_m = false);
    virtual ~MultiPointMShape ();

    virtual void GetBoundingBoxEx (BoundingBoxEx& box, bool hasM);
    virtual void GetBoundingBoxEx (BoundingBoxEx& box);
    virtual FdoByteArray* GetGeometry ();
    static int GetSize (int nPoints);
    virtual int GetContentLength ();
    virtual void DebugPrintDetails ();

    virtual MZData* GetMData ();
    virtual bool HasMeasure ();

protected:
    MZData mMData;
};

inline void MultiPointMShape::GetBoundingBoxEx (BoundingBoxEx& box, bool hasM)
{
    superclass::GetBoundingBoxEx (box);
    if (hasM)
    {
        box.mMin = GetMData ()->GetRangeMin ();
        box.mMax = GetMData ()->GetRangeMax ();
    }
}

inline void MultiPointMShape::GetBoundingBoxEx (BoundingBoxEx& box)
{
    GetBoundingBoxEx (box, true);
}

inline int MultiPointMShape::GetSize (int nPoints)
{
    return (
        superclass::GetSize (nPoints)
        //  bounding measure range
        + (2 * sizeof (double))
        // measures
        + (nPoints * sizeof (double)));
}

inline int MultiPointMShape::GetContentLength ()
{
    return (GetSize (GetNumPoints ()) / WORD_SIZE_IN_BYTES);
}

inline MZData* MultiPointMShape::GetMData ()
{
    return (&mMData);
}

inline bool MultiPointMShape::HasMeasure ()
{
    return (true);
}

#endif // MULTIPOINTMSHAPE_H

