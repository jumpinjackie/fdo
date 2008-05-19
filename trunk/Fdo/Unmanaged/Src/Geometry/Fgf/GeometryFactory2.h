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


FDOPOOL_DEFINE(FdoPoolFgfByteArray,         FdoByteArray)
FDOPOOL_DEFINE(FdoPoolFgfLinearRing,        FdoFgfLinearRing)
FDOPOOL_DEFINE(FdoPoolFgfLineString,        FdoFgfLineString)
FDOPOOL_DEFINE(FdoPoolFgfPoint,             FdoFgfPoint)
FDOPOOL_DEFINE(FdoPoolFgfPolygon,           FdoFgfPolygon)
FDOPOOL_DEFINE(FdoPoolFgfMultiPoint,        FdoFgfMultiPoint)
FDOPOOL_DEFINE(FdoPoolFgfMultiGeometry,     FdoFgfMultiGeometry)
FDOPOOL_DEFINE(FdoPoolFgfMultiLineString,   FdoFgfMultiLineString)
FDOPOOL_DEFINE(FdoPoolFgfMultiPolygon,      FdoFgfMultiPolygon)
FDOPOOL_DEFINE(FdoPoolFgfCurveString,       FdoFgfCurveString)
FDOPOOL_DEFINE(FdoPoolFgfMultiCurveString,  FdoFgfMultiCurveString)
FDOPOOL_DEFINE(FdoPoolFgfCurvePolygon,      FdoFgfCurvePolygon)
FDOPOOL_DEFINE(FdoPoolFgfMultiCurvePolygon, FdoFgfMultiCurvePolygon)

#define FDO_GEOM_POOL_DECLARE(type) FdoPtr<FdoPoolFgf##type>        m_Pool##type;
#define FDO_GEOM_POOL_NULLIFY(type) m_Pool##type = NULL;

/// <summary>This defines private data for the FdoFgfGeometryFactory class.
/// Changes can be made here without affecting the public interface.
/// </summary>
class FdoFgfGeometryPools : public FdoDisposable
{
public:
    FDO_GEOM_POOL_DECLARE(ByteArray);
    FDO_GEOM_POOL_DECLARE(LinearRing);
    FDO_GEOM_POOL_DECLARE(LineString);
    FDO_GEOM_POOL_DECLARE(Point);
    FDO_GEOM_POOL_DECLARE(Polygon);
    FDO_GEOM_POOL_DECLARE(MultiPoint);
    FDO_GEOM_POOL_DECLARE(MultiGeometry);
    FDO_GEOM_POOL_DECLARE(MultiLineString);
    FDO_GEOM_POOL_DECLARE(MultiPolygon);
    FDO_GEOM_POOL_DECLARE(CurveString);
    FDO_GEOM_POOL_DECLARE(MultiCurveString);
    FDO_GEOM_POOL_DECLARE(CurvePolygon);
    FDO_GEOM_POOL_DECLARE(MultiCurvePolygon);

#ifdef EXTRA_DEBUG
    FdoInt32 m_numByteArraysCreated;
    void PrintStats(FILE * fileHandle);
    FILE * m_fdoDebugFile;
#endif
    FdoFgfGeometryPools()
#ifdef EXTRA_DEBUG
        : m_numByteArraysCreated(0),
          m_fdoDebugFile(NULL)
#endif
    {
    };
    ~FdoFgfGeometryPools()
    {
        Clear();
    };

    void Clear()
    {
        // Geometry object destructors may try to hand back byte arrays
        // to the pool.  Rather than let automatic destruction do this
        // in an undefined order, we'll do it manually here to ensure that
        // the byte array pool is still defined until the last step.
        FDO_GEOM_POOL_NULLIFY(LinearRing);
        FDO_GEOM_POOL_NULLIFY(LineString);
        FDO_GEOM_POOL_NULLIFY(Point);
        FDO_GEOM_POOL_NULLIFY(Polygon);
        FDO_GEOM_POOL_NULLIFY(MultiPoint);
        FDO_GEOM_POOL_NULLIFY(MultiGeometry);
        FDO_GEOM_POOL_NULLIFY(MultiLineString);
        FDO_GEOM_POOL_NULLIFY(MultiPolygon);
        FDO_GEOM_POOL_NULLIFY(CurveString);
        FDO_GEOM_POOL_NULLIFY(MultiCurveString);
        FDO_GEOM_POOL_NULLIFY(CurvePolygon);
        FDO_GEOM_POOL_NULLIFY(MultiCurvePolygon);
        FDO_GEOM_POOL_NULLIFY(ByteArray);
    }

    // Internal methods for FdoFgfGeometryFactory's use.

#define DECLARE_CREATE_POOLED_GEOMETRY(type) \
    FdoI##type * Create##type( \
        FdoFgfGeometryFactory * factory, \
        FdoFgfGeometryPools * pools, \
        FdoByteArray * byteArray, \
        const FdoByte * data, \
        FdoInt32 count \
        )

    DECLARE_CREATE_POOLED_GEOMETRY(LineString);
    DECLARE_CREATE_POOLED_GEOMETRY(Point);
    DECLARE_CREATE_POOLED_GEOMETRY(Polygon);
    DECLARE_CREATE_POOLED_GEOMETRY(MultiPoint);
    DECLARE_CREATE_POOLED_GEOMETRY(MultiGeometry);
    DECLARE_CREATE_POOLED_GEOMETRY(MultiLineString);
    DECLARE_CREATE_POOLED_GEOMETRY(MultiPolygon);
    DECLARE_CREATE_POOLED_GEOMETRY(CurveString);
    DECLARE_CREATE_POOLED_GEOMETRY(MultiCurveString);
    DECLARE_CREATE_POOLED_GEOMETRY(CurvePolygon);
    DECLARE_CREATE_POOLED_GEOMETRY(MultiCurvePolygon);

    /// Get an empty (possibly pooled) byte array.
    virtual FdoByteArray * GetByteArray();

    /// Pool this array, released from some geometry.
    virtual void TakeReleasedByteArray(FdoByteArray * byteArray);

};

struct FdoFgfGeometryFactory2
{
    FdoPtr<FdoFgfGeometryPools>  m_geometryPools;
    bool                         m_useThreadLocal;

    FdoFgfGeometryFactory2(bool useThreadLocal);

    inline FdoFgfGeometryPools * GetPoolsForGeomCtor()
    {
        FdoFgfGeometryPools * pools = NULL;    // Lightweight (no reference count)
        if (!m_useThreadLocal)
            pools = m_geometryPools;
        return pools;
    }

    virtual ~FdoFgfGeometryFactory2();
};

#endif

