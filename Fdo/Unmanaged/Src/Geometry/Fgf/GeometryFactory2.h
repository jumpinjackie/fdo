#ifndef _FGFGEOMETRYFACTORY2_H_
#define _FGFGEOMETRYFACTORY2_H_

//
// Copyright (C) 2004-2006  Autodesk, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//



#ifdef _WIN32
#pragma once
#endif

#include <Geometry/Fgf/Factory.h>
#include "Pool.h"
#include <Geometry/DirectPositionImpl.h>
#include <Geometry/EnvelopeImpl.h>
#include "LineString.h"
#include "Point.h"
#include "LinearRing.h"
#include "LineStringSegment.h"
#include "Polygon.h"
#include "MultiGeometry.h"
#include "MultiPoint.h"
#include "MultiLineString.h"
#include "MultiPolygon.h"
#include "CircularArcSegment.h"
#include "CurveString.h"
#include "MultiCurveString.h"
#include "Ring.h"
#include "CurvePolygon.h"
#include "MultiCurvePolygon.h"


FDOPOOL_DEFINE(FdoByteArrayPool,            FdoByteArray)
FDOPOOL_DEFINE(FdoFgfLinearRingPool,        FdoFgfLinearRing)
FDOPOOL_DEFINE(FdoPoolFgfLineString,        FdoFgfLineString)


/// <summary>This defines private data for the FdoFgfGeometryFactory class.
/// Changes can be made here without affecting the public interface.
/// </summary>
struct FdoFgfGeometryFactory2
{
    FdoPtr<FdoGeometryCollection>       m_geometryPool2;    // A few previous Geometries created; For fast re-creation.
    FdoPtr<FdoByteArrayPool>            m_byteArrayPool;    // For fast re-creation.
    FdoPtr<FdoFgfLinearRingPool>        m_linearRingPool;   // For fast re-creation.
    FdoPtr<FdoPoolFgfLineString>        m_PoolLineString;

#ifdef EXTRA_DEBUG
    FdoInt32 m_numByteArraysCreated;
    void PrintStats();
#endif
    FdoFgfGeometryFactory2()
#ifdef EXTRA_DEBUG
        : m_numByteArraysCreated(0)
#endif
    {
    };
    ~FdoFgfGeometryFactory2() {};

    // Internal methods for FdoFgfGeometryFactory's use.

#define CREATE_POOLED_GEOMETRY(type) \
    FdoI##type * Create##type( \
        FdoFgfGeometryFactory * factory, \
        FdoByteArray * byteArray, \
        const FdoByte * data, \
        FdoInt32 count \
        ) \
    { \
        FDOPOOL_CREATE_OBJECT( \
            m_Pool##type, FdoPoolFgf##type, 4, \
            FdoFgf##type, \
            FdoFgf##type(factory, byteArray, data, count), \
            Reset(byteArray, data, count) ); \
    }

    CREATE_POOLED_GEOMETRY(LineString);

};
#endif

