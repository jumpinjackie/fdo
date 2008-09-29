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

#ifndef MULTIPOINTSHAPE_H
#define MULTIPOINTSHAPE_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include "Shape.h"
#include "Constants.h"

class MultiPointShape : public Shape
{
protected:
    typedef Shape superclass;

public:
    MultiPointShape (int nRecordNumber, void* pMemory, bool bOverlay, int nPoints = -1, BoundingBoxEx *box = NULL);
    static MultiPointShape* NewMultiPointShape (int nRecordNumber, int nPoints, BoundingBoxEx *box = NULL);
    virtual ~MultiPointShape ();

    virtual BoundingBox* GetBoundingBox ();
    virtual void GetBoundingBoxEx (BoundingBoxEx& box);
    virtual int GetNumPoints ();
    static int GetSize (int nPoints);
    virtual int GetContentLength ();
    virtual FdoByteArray* GetGeometry ();
    virtual void DebugPrintDetails ();

protected:
    ESRIMultiPointRecord* mRecord;
};

inline int MultiPointShape::GetSize (int nPoints)
{
    return (
    	// shape type 
        sizeof (int)
        // bounding box
        + (4 * sizeof (double))
        // number of points
        + sizeof (int)
        // points in the set
        + (nPoints * 2 * sizeof (double)));
}

inline int MultiPointShape::GetContentLength ()
{
    return (GetSize (GetNumPoints ()) / WORD_SIZE_IN_BYTES);
}

inline BoundingBox* MultiPointShape::GetBoundingBox ()
{
    return &(mRecord->boundingBox);
}

inline void MultiPointShape::GetBoundingBoxEx (BoundingBoxEx& box)
{
    box = *GetBoundingBox ();
}

inline int MultiPointShape::GetNumPoints ()
{
    return (mRecord->nNumPoints);
}

#endif // MULTIPOINTSHAPE_H

