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

#ifndef DOUBLEPOINT_H_
#define DOUBLEPOINT_H_

#ifdef _WIN32
#pragma once
#endif // _WIN32

//-------------------------------------------------------------------------
//
// Class: DoublePoint.
//
// Purpose: Stores coordinates of a point in double precision.
//
//-------------------------------------------------------------------------

struct DoublePoint {
    double x;
    double y;

    DoublePoint() : x(0.0), y(0.0) {}
    DoublePoint(double x, double y) : x(x), y(y) {}
    int operator==(const DoublePoint &point) const;
    int operator!=(const DoublePoint &point) const;
};


//-------------------------------------------------------------------------
//
// DoublePoint::operator==() - Function to test two points for equality
//
//-------------------------------------------------------------------------

inline int DoublePoint::operator==(const DoublePoint &point) const
{
    return (x != point.x || y != point.y ? 0 : 1);
}


//-------------------------------------------------------------------------
//
// DoublePoint::operator!=() - Function to test two points for inequality
//
//-------------------------------------------------------------------------

inline int DoublePoint::operator!=(const DoublePoint &point) const
{
    return (x != point.x || y != point.y ? 1 : 0);
}

#endif // DOUBLEPOINT_H_

