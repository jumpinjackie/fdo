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

#ifndef POINTZSHAPE_H
#define POINTZSHAPE_H

#include "PointMShape.h"
#include "MZDataBase.h"

class PointZShape : public PointMShape
{
protected:
    typedef PointMShape superclass;

public:
    PointZShape (int nRecordNumber, void* pMemory, bool bOverlay, BoundingBoxEx *box = NULL, bool has_m = false);
    static PointZShape* NewPointZShape (int nRecordNumber, BoundingBoxEx *box = NULL, bool has_m = false);
    virtual ~PointZShape ();

    virtual void GetBoundingBoxEx (BoundingBoxEx& box);
    virtual FdoByteArray* GetGeometry ();
    static int GetSize ();
    virtual int GetContentLength ();
    virtual void DebugPrintDetails ();

    virtual MZDataBase* GetZData ();
    virtual bool HasMeasure ();

protected:
    MZDataBase mZData;
    bool mHasM;
};

inline void PointZShape::GetBoundingBoxEx (BoundingBoxEx& box)
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

inline int PointZShape::GetSize ()
{
    return (
        superclass::GetSize ()
        // Z
        + sizeof (double));
}

inline int PointZShape::GetContentLength ()
{
    return (GetSize () / WORD_SIZE_IN_BYTES);
}

inline MZDataBase* PointZShape::GetZData ()
{
    return (&mZData);
}

inline bool PointZShape::HasMeasure ()
{
    return (mHasM);
}

#endif // POINTZSHAPE_H

