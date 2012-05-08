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

#ifndef BOUNDINGBOXEX_H
#define BOUNDINGBOXEX_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include "BoundingBox.h"
#include <FdoCommonOSUtil.h>

#include <limits>
#ifndef _WIN32
#include <math.h>
#endif
using namespace std;

/// <summary>Stores coordinates of a lower-left and upper-right corners of a shape,
/// plus the Z and M value limits and caches the area.</summary>
struct BoundingBoxEx : BoundingBox
{
protected:
    typedef BoundingBox superclass;

public:
    double zMin;
    double zMax;
    double mMin;
    double mMax;
    double mArea;

    BoundingBoxEx ();
    BoundingBoxEx (const DoublePoint *point, double* z, double* m);
    BoundingBoxEx (const BoundingBox& box);
    void Update (const DoublePoint *point);
    void UnionWith (const BoundingBoxEx *box);
    double Area () const;
    bool operator== (const BoundingBoxEx &box) const;
    bool operator!= (const BoundingBoxEx &box) const;
    void operator= (const BoundingBox &box);
    void operator= (const BoundingBoxEx &box);
};


/// <summary>Construct an empty extended bounding box.</summary>
/// <param name="point">Input the point around which to construct the bounding box.</param> 
inline BoundingBoxEx::BoundingBoxEx () :
#ifdef _WIN32
    zMin (numeric_limits<double>::signaling_NaN ()),
    mMin (numeric_limits<double>::signaling_NaN ()),
    zMax (numeric_limits<double>::signaling_NaN ()),
    mMax (numeric_limits<double>::signaling_NaN ()),
    mArea (numeric_limits<double>::signaling_NaN ())
#else
    zMin (NAN),
    mMin (NAN),
    zMax (NAN),
    mMax (NAN),
    mArea (NAN)
#endif
{
}

/// <summary>Construct an extended bounding box for a point.</summary>
/// <param name="point">Input the point around which to construct the bounding box.</param> 
/// <param name="z">Input the Z limits for the extended bounding box.</param> 
/// <param name="m">Input the M limits for the extended bounding box.</param> 
inline BoundingBoxEx::BoundingBoxEx (const DoublePoint *point, double* z, double* m) :
    BoundingBox (point),
#ifdef _WIN32
    zMin (NULL == z ? numeric_limits<double>::signaling_NaN () : *z),
    zMax (NULL == z ? numeric_limits<double>::signaling_NaN () : *(z + 1)),
    mMin (NULL == m ? numeric_limits<double>::signaling_NaN () : *m),
    mMax (NULL == m ? numeric_limits<double>::signaling_NaN () : *(m + 1)),
    mArea (numeric_limits<double>::signaling_NaN ())
#else
    zMin (NULL == z ? NAN : *z),
    zMax (NULL == z ? NAN : *(z + 1)),
    mMin (NULL == m ? NAN : *m),
    mMax (NULL == m ? NAN : *(m + 1)),
    mArea (NAN)
#endif
{
}

/// <summary>Construct an extended bounding box from a normal bounding box.</summary>
/// <param name="box">Input the box to copy.</param> 
inline BoundingBoxEx::BoundingBoxEx (const BoundingBox& box) :
    BoundingBox (box),
#ifdef _WIN32
    zMin (numeric_limits<double>::signaling_NaN ()),
    zMax (numeric_limits<double>::signaling_NaN ()),
    mMin (numeric_limits<double>::signaling_NaN ()),
    mMax (numeric_limits<double>::signaling_NaN ()),
    mArea (numeric_limits<double>::signaling_NaN ())
#else
    zMin (NAN),
    zMax (NAN),
    mMin (NAN),
    mMax (NAN),
    mArea (NAN)
#endif
{
}

/// <summary>Update the extents of a bounding box based on a point.</summary>
/// <param name="point">Input the point to incorporate.</param> 
inline void BoundingBoxEx::Update (const DoublePoint *point)
{
    if (point->x < xMin)
    {
        xMin = point->x;
#ifdef _WIN32
        mArea = numeric_limits<double>::signaling_NaN ();
#else
        mArea = NAN;
#endif
    }
    else if (point->x > xMax)
    {
        xMax = point->x;
#ifdef _WIN32
        mArea = numeric_limits<double>::signaling_NaN ();
#else
        mArea = NAN;
#endif
    }

    if (point->y < yMin)
    {
        yMin = point->y;
#ifdef _WIN32
        mArea = numeric_limits<double>::signaling_NaN ();
#else
        mArea = NAN;
#endif
    }
    else if (point->y > yMax)
    {
        yMax = point->y;
#ifdef _WIN32
        mArea = numeric_limits<double>::signaling_NaN ();
#else
        mArea = NAN;
#endif
    }
}

/// <summary>Generate the "union" of this box and
/// the specified box and store the result back to this
/// BoundingBox.</summary>
/// <remarks>The union of the two bounding boxes is the smallest
///  bounding box that encloses both of the original bounding boxes.</remarks>
/// <param name="box">Input the other box to use.</param> 
/// <returns>Returns nothing.</returns> 
inline void BoundingBoxEx::UnionWith (const BoundingBoxEx *box)
{
    if (box->xMin < xMin)
    {
        xMin = box->xMin;
#ifdef _WIN32
        mArea = numeric_limits<double>::signaling_NaN ();
#else
        mArea = NAN;

#endif
    }
    if (box->xMax > xMax)
    {
        xMax = box->xMax;
#ifdef _WIN32
        mArea = numeric_limits<double>::signaling_NaN ();
#else

        mArea = NAN;
#endif
    }
    if (box->yMin < yMin)
    {
        yMin = box->yMin;
#ifdef _WIN32
        mArea = numeric_limits<double>::signaling_NaN ();
#else
        mArea = NAN;
#endif
    }
    if (box->yMax > yMax)
    {
        yMax = box->yMax;
#ifdef _WIN32
        mArea = numeric_limits<double>::signaling_NaN ();
#else
        mArea = NAN;
#endif
    }
    if (box->zMin < zMin)
        zMin = box->zMin;
    if (box->zMax > zMax)
        zMax = box->zMax;
    if (box->mMin < mMin)
        mMin = box->mMin;
    if (box->mMax > mMax)
        mMax = box->mMax;
}

/// <summary>Get the area enclosed by this box.</summary>
/// <returns>Returns the box area.</returns> 
inline double BoundingBoxEx::Area () const
{
    if (FdoCommonOSUtil::_isnan (mArea))
        (((BoundingBoxEx*)this)->mArea) = superclass::Area (); // cheat here and cast away constness

    return (mArea);
}

inline void BoundingBoxEx::operator= (const BoundingBox &box)
{
    superclass::operator= (box);
#ifdef _WIN32
    zMin = numeric_limits<double>::signaling_NaN ();
    mMin = numeric_limits<double>::signaling_NaN ();
    zMax = numeric_limits<double>::signaling_NaN ();
    mMax = numeric_limits<double>::signaling_NaN ();
    mArea = numeric_limits<double>::signaling_NaN ();
#else
    zMin = NAN;
    mMin = NAN; 
    zMax = NAN; 
    mMax = NAN;
    mArea = NAN;
#endif
}

inline void BoundingBoxEx::operator= (const BoundingBoxEx &box)
{
    superclass::operator= (box);
    zMin = box.zMin;
    mMin = box.mMin;
    zMax = box.zMax;
    mMax = box.mMax;
    mArea = box.mArea;
}

inline bool BoundingBoxEx::operator== (const BoundingBoxEx &box) const
{
    return (superclass::operator== (box)
            && (zMin == box.zMin) && (zMax == box.zMax)
            && (mMin == box.mMin) && (mMax == box.mMax));
}

inline bool BoundingBoxEx::operator!= (const BoundingBoxEx &box) const
{
    return (!(*this == box));
}

#endif // BOUNDINGBOXEX_H

