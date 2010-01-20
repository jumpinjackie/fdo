//
// Copyright (C) 2006 Refractions Research, Inc. 
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
#include "stdafx.h"
#include "FdoRdbmsPostGisGeometryCapabilities.h"

FdoRdbmsPostGisGeometryCapabilities::FdoRdbmsPostGisGeometryCapabilities()
{
    // idle
}

FdoRdbmsPostGisGeometryCapabilities::~FdoRdbmsPostGisGeometryCapabilities()
{
    // idle
}

void FdoRdbmsPostGisGeometryCapabilities::Dispose()
{
    delete this;
}

FdoGeometryType* FdoRdbmsPostGisGeometryCapabilities::GetGeometryTypes(
    FdoInt32& length)
{
    // TODO: mloskot - Check these types for PostGIS

    static FdoGeometryType rdbmGeomTypes[] = 
    {
        FdoGeometryType_Point,
        FdoGeometryType_LineString,
        FdoGeometryType_Polygon,
        FdoGeometryType_MultiPoint,
        FdoGeometryType_MultiLineString,
        FdoGeometryType_MultiPolygon

        // FdoGeometryType_MultiGeometry,

        // TODO: PostGIS Provider (MySQL) currently doesn't support curve segments:
        //
        // FdoGeometryType_CurveString,
        // FdoGeometryType_CurvePolygon,
        // FdoGeometryType_MultiCurveString,
        // FdoGeometryType_MultiCurvePolygon

    };

    length = sizeof(rdbmGeomTypes) / sizeof(FdoGeometryType);
    return rdbmGeomTypes;
}

FdoGeometryComponentType* FdoRdbmsPostGisGeometryCapabilities::GetGeometryComponentTypes(
    FdoInt32& length)
{
    // TODO: mloskot - Check these types for PostGIS

    static FdoGeometryComponentType rdbmsGeomCompTypes[] =
    {

        // PostGIS Provider (MySQL) currently doesn't support curve segments
        FdoGeometryComponentType_LinearRing,
        FdoGeometryComponentType_LineStringSegment

        // PostGIS Provider (MySQL) currently doesn't support curve segments:
        // FdoGeometryComponentType_CircularArcSegment,
        // FdoGeometryComponentType_Ring

    };

    length = sizeof(rdbmsGeomCompTypes) / sizeof(FdoGeometryComponentType);
    return rdbmsGeomCompTypes;
}

FdoInt32 FdoRdbmsPostGisGeometryCapabilities::GetDimensionalities()
{
    return (0 | FdoDimensionality_XY);
}