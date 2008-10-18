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

#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

/// <summary>Stores coordinates of a lower-left and upper-right corners of a shape.</summary>
struct BoundingBox
{
    // note: these must be kept in this order
    // it is read/written directly by the i/o code
    double xMin;
    double yMin;
    double xMax;
    double yMax;

    BoundingBox ();
    BoundingBox (const DoublePoint *point);
    void Update (const DoublePoint *point);
    bool Intersects (const BoundingBox *box) const ;
    bool Contains (const BoundingBox *box, bool strict = false) const;
    double Width () const;
    double Height () const;
    double Area () const;
    void operator= (const BoundingBox &box);
    bool operator== (const BoundingBox &box) const;
    bool operator!= (const BoundingBox &box) const;
};

/// <summary>Construct an empty bounding box.</summary>
inline BoundingBox::BoundingBox () :
    xMin(0.0),
    yMin(0.0),
    xMax(0.0),
    yMax(0.0)
{
}

/// <summary>Construct a bounding box for a point.</summary>
/// <param name="point">Input the point around which to construct the bounding box.</param> 
inline BoundingBox::BoundingBox (const DoublePoint *point)
{
    xMin = xMax = point->x;
    yMin = yMax = point->y;
}

/// <summary>Update the extents of a bounding box based on a point.</summary>
/// <param name="point">Input the point to incorporate.</param> 
inline void BoundingBox::Update (const DoublePoint *point)
{
    if (point->x < xMin)
        xMin = point->x;
    else if (point->x > xMax)
        xMax = point->x;

    if (point->y < yMin)
        yMin = point->y;
    else if (point->y > yMax)
        yMax = point->y;
}

/// <summary>Determine if this bounding box intersects another one.</summary>
/// <param name="box">Input the other box to check against this one.</param> 
/// <returns>Returns true if the boxes intersect.</returns> 
inline bool BoundingBox::Intersects (const BoundingBox *box) const
{
    return ((xMin <= box->xMax) && (xMax >= box->xMin)
            && (yMin <= box->yMax) && (yMax >= box->yMin));
}

/// <summary>Determine if this bounding box contains another one.</summary>
/// <param name="box">Input the other box to check against this one.</param> 
/// <returns>Returns true if this box contains the other.</returns> 
inline bool BoundingBox::Contains (const BoundingBox *box, bool strict) const
{
    if (strict)
        return ((xMin < box->xMin) && (xMax > box->xMax)
                && (yMin < box->yMin) && (yMax > box->yMax));
    else
        return ((xMin <= box->xMin) && (xMax >= box->xMax)
                && (yMin <= box->yMin) && (yMax >= box->yMax));
}

/// <summary>Get the width of this box.</summary>
/// <returns>Returns the box width.</returns> 
inline double BoundingBox::Width () const
{
    return (xMax - xMin);
}

/// <summary>Get the height of this box.</summary>
/// <returns>Returns the box height.</returns> 
inline double BoundingBox::Height() const
{
    return (yMax - yMin);
}

/// <summary>Get the area enclosed by this box.</summary>
/// <returns>Returns the box area.</returns> 
inline double BoundingBox::Area () const
{
    return (Width() * Height());
}

inline void BoundingBox::operator= (const BoundingBox &box)
{
    xMin = box.xMin;
    yMin = box.yMin;
    xMax = box.xMax;
    yMax = box.yMax;
}

/// <summary>Check this box with another for equality.</summary>
/// <returns>Returns true if the boxes are equal.</returns> 
inline bool BoundingBox::operator== (const BoundingBox &box) const
{
    return ((xMin == box.xMin) && (yMin == box.yMin)
            && (xMax == box.xMax) && (yMax == box.yMax));
}

/// <summary>Check this box with another for inequality.</summary>
/// <returns>Returns true if the boxes are not equal.</returns> 
inline bool BoundingBox::operator!= (const BoundingBox &box) const
{
    return (!(*this == box));
}

#endif // BOUNDINGBOX_H

