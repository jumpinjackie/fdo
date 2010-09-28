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

#ifndef POINTSHAPE_H
#define POINTSHAPE_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include "Shape.h"
#include "Constants.h"

class PointShape : public Shape
{
protected:
    typedef Shape superclass;

public:
    PointShape (int nRecordNumber, void* pMemory, bool bOverlay, BoundingBoxEx *box = NULL);
    static PointShape* NewPointShape (int nRecordNumber, BoundingBoxEx *box = NULL);
    virtual ~PointShape ();

    virtual int GetNumPoints ();
    virtual BoundingBox* GetBoundingBox ();
    virtual void GetBoundingBoxEx (BoundingBoxEx& box);
    virtual FdoByteArray* GetGeometry ();
    virtual int GetContentLength ();
    static int GetSize ();
    virtual void DebugPrintDetails ();

protected:
    BoundingBox mBoundingBox;
};

inline int PointShape::GetNumPoints ()
{
    return (1);
}

inline BoundingBox* PointShape::GetBoundingBox ()
{
    return (&mBoundingBox);
}

inline void PointShape::GetBoundingBoxEx (BoundingBoxEx& box)
{
    box = *GetBoundingBox ();
}

inline int PointShape::GetSize ()
{
    return (
        // shape type
        sizeof (int)
        // X,Y
        + (2 * sizeof (double)));
}

inline int PointShape::GetContentLength ()
{
    return (GetSize () / WORD_SIZE_IN_BYTES);
}

#endif // POINTSHAPE_H

