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

#ifndef NULLSHAPE_H
#define NULLSHAPE_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include "Shape.h"
#include "Constants.h"

class NullShape : public Shape
{
protected:
    typedef Shape superclass;

public:
    NullShape (int nRecordNumber, void* pMemory, bool bOverlay);
    static NullShape* NewNullShape (int nRecordNumber);
    virtual ~NullShape ();

    virtual BoundingBox* GetBoundingBox ();
    virtual void GetBoundingBoxEx (BoundingBoxEx& box);
    virtual int GetNumPoints ();
    virtual FdoByteArray* GetGeometry ();
    static int GetSize ();
    virtual int GetContentLength ();
    virtual void DebugPrintDetails ();
};

inline BoundingBox* NullShape::GetBoundingBox ()
{
    return (NULL);
}

inline void NullShape::GetBoundingBoxEx (BoundingBoxEx& box)
{
}

inline int NullShape::GetNumPoints ()
{
    return (0);
}

inline int NullShape::GetSize ()
{
    // shape type
    return (sizeof (int));
}

inline int NullShape::GetContentLength ()
{
    return (GetSize () / WORD_SIZE_IN_BYTES);
}

#endif // NULLSHAPE_H

