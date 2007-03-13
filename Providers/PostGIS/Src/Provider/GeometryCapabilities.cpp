//
// Copyright (C) 2007 Refractions Research, Inc. 
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
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
//
#include "stdafx.h"

#include "PostGisProvider.h"
#include "GeometryCapabilities.h"

#include <cassert>

namespace fdo { namespace postgis {

GeometryCapabilities::GeometryCapabilities()
{
    FDOLOG_WRITE("GeometryCapabilities created");
}

GeometryCapabilities::~GeometryCapabilities()
{
}

///////////////////////////////////////////////////////////////////////////////
// FdoIDisposable interface
///////////////////////////////////////////////////////////////////////////////

void GeometryCapabilities::Dispose()
{
    delete this;
}

///////////////////////////////////////////////////////////////////////////////
// FdoIGeometryCapabilities interface
///////////////////////////////////////////////////////////////////////////////

FdoGeometryType* GeometryCapabilities::GetGeometryTypes(FdoInt32& size)
{
    // NOTE: The FdoGeometryType_None does not represent an instantiable type.
    // An FDO client should not expect an FDO provider to list support
    // for it in its capabilities.

    static FdoGeometryType types[] =
    {
        FdoGeometryType_Point,
        FdoGeometryType_LineString,
        FdoGeometryType_Polygon,
        FdoGeometryType_MultiPoint,
        FdoGeometryType_MultiLineString,
        FdoGeometryType_MultiPolygon,
        FdoGeometryType_MultiGeometry,
        
        // TODO: Waiting for curves supports
        //FdoGeometryType_CurveString,
        //FdoGeometryType_CurvePolygon,
        //FdoGeometryType_MultiCurveString,
        //FdoGeometryType_MultiCurvePolygon
    };

    size = (sizeof(types) / sizeof(FdoClassType));
    return types;
}

FdoGeometryComponentType* GeometryCapabilities::GetGeometryComponentTypes(FdoInt32& size)
{
    static FdoGeometryComponentType types[] =
    {
        FdoGeometryComponentType_LinearRing,
        
        // TODO: Waiting for curves supports
        //FdoGeometryComponentType_CircularArcSegment,
        //FdoGeometryComponentType_LineStringSegment,
        //FdoGeometryComponentType_Ring,
    };
    
    size = (sizeof(types) / sizeof(FdoClassType));
    return types;    
}

FdoInt32 GeometryCapabilities::GetDimensionalities()
{
    return (FdoDimensionality_XY | FdoDimensionality_Z | FdoDimensionality_M);
}


}} // namespace fdo::postgis
