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

#ifndef SHAPE_H
#define SHAPE_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include <Fdo.h>

#include "DoublePoint.h"
#include "BoundingBox.h"
#include "BoundingBoxEx.h"
#include "Constants.h"

class Shape
{
public:
    /// <summary>Constructor for overlaying on an existing memory image.</summary>
    /// <param name="nRecordNumber">Shape record number (one based).</param>
    /// <param name="pMemory">Pointer to shape record.</param>
    /// <param name="bOverlay">Flag that if true indicates the memory is communal.</param>
    Shape (int nRecordNumber, void* pMemory, bool bOverlay, DoublePoint* pPoints);
    /// <summary>Destructor.</summary>
    virtual ~Shape ();

    virtual int GetRecordNum ();
    virtual void SetRecordNum (int num);
    virtual eShapeTypes GetShapeType ();
    virtual void SetShapeType (eShapeTypes type);
    virtual void* GetContent ();
    virtual DoublePoint* GetPoints ();

    // Pure virtual methods
    virtual BoundingBox* GetBoundingBox () = 0;
    virtual void GetBoundingBoxEx (BoundingBoxEx& box) = 0;
    virtual int GetNumPoints () = 0;
    /// <summary>This method gets the OGIS WKB representation of this geometry.</summary>
    /// <returns>Returns the well known binary form of the shape.</returns> 
    virtual FdoByteArray* GetGeometry () = 0;
    virtual int GetContentLength () = 0;
    virtual void DebugPrintDetails () = 0;
    bool GetOverlay () { return (mOverlay); };
    void SetOverlay (bool overlay) { mOverlay = overlay; };
    virtual bool HasMeasure ();

protected:
    int mRecordNumber;
    void* mMemory;
    DoublePoint* mPoints;
    bool mOverlay; // true if this object does not own it's memory
};

inline int Shape::GetRecordNum ()
{
    return (mRecordNumber);
}

inline void Shape::SetRecordNum (int num)
{
    mRecordNumber = num;
}

inline eShapeTypes Shape::GetShapeType ()
{
	return ( ( mMemory && mPoints ) ? ((eShapeTypes)(*((int*)mMemory))) : eNullShape );
}

inline void Shape::SetShapeType (eShapeTypes type)
{
    *((int*)mMemory) = type;
}

inline void* Shape::GetContent ()
{
    return (mMemory);
};

inline DoublePoint* Shape::GetPoints ()
{
    return (mPoints);
}

inline bool Shape::HasMeasure ()
{
    return (false);
}

#endif // SHAPE_H

