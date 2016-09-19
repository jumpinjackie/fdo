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

#ifndef POINTMSHAPE_H
#define POINTMSHAPE_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include "PointShape.h"
#include "MZDataBase.h"

class PointMShape : public PointShape
{
protected:
    typedef PointShape superclass;

public:
    PointMShape (int nRecordNumber, void* pMemory, bool bOverlay, void* pMValues = NULL, BoundingBoxEx *box = NULL);
    static PointMShape* NewPointMShape (int nRecordNumber, BoundingBoxEx *box = NULL);
    virtual ~PointMShape ();

    virtual void GetBoundingBoxEx (BoundingBoxEx& box, bool hasM);
    virtual void GetBoundingBoxEx (BoundingBoxEx& box);
    virtual FdoByteArray* GetGeometry ();
    static int GetSize ();
    virtual int GetContentLength ();
    virtual void DebugPrintDetails ();

    virtual MZDataBase* GetMData ();
    virtual bool HasMeasure ();

protected:
    MZDataBase mMData;
};

inline void PointMShape::GetBoundingBoxEx (BoundingBoxEx& box, bool hasM)
{
    superclass::GetBoundingBoxEx (box);
    if (hasM)
    {
        box.mMin = GetMData ()->GetRangeMin ();
        box.mMax = GetMData ()->GetRangeMax ();
    }
}

inline void PointMShape::GetBoundingBoxEx (BoundingBoxEx& box)
{
    GetBoundingBoxEx (box, true);
}

inline int PointMShape::GetSize ()
{
    return (
        superclass::GetSize ()
        // M
        + sizeof (double));
}

inline int PointMShape::GetContentLength ()
{
    return (GetSize () / WORD_SIZE_IN_BYTES);
}

inline MZDataBase* PointMShape::GetMData ()
{
    return (&mMData);
}

inline bool PointMShape::HasMeasure ()
{
    return (true);
}

#endif // POINTMSHAPE_H

