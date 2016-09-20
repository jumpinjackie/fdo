// 
//  
//  Copyright (C) 2004-2006  Autodesk, Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//  
#include "stdafx.h"
#include "SdfGeometryCapabilities.h"


SdfGeometryCapabilities::SdfGeometryCapabilities()
{
}

SdfGeometryCapabilities::~SdfGeometryCapabilities()
{
}

void SdfGeometryCapabilities::Dispose()
{
    delete this;
}

FdoGeometryType* SdfGeometryCapabilities::GetGeometryTypes( FdoInt32& length)
{
    //any of these geometry types can be put into an SDF,
    //(as long as a bounding box can be computed)
    static FdoGeometryType geomTypes[] = 
    {
        FdoGeometryType_Point,
        FdoGeometryType_LineString,
        FdoGeometryType_Polygon,
        FdoGeometryType_MultiPoint,
        FdoGeometryType_MultiLineString,
        FdoGeometryType_MultiPolygon,
        FdoGeometryType_MultiGeometry,
        FdoGeometryType_CurveString,
        FdoGeometryType_CurvePolygon,
        FdoGeometryType_MultiCurveString,
        FdoGeometryType_MultiCurvePolygon
    };

    length = sizeof(geomTypes) / sizeof(FdoGeometryType);
    return geomTypes;
}

FdoGeometryComponentType* SdfGeometryCapabilities::GetGeometryComponentTypes( FdoInt32& length )
{
    static FdoGeometryComponentType compTypes[] = 
    {
        FdoGeometryComponentType_LinearRing,
        FdoGeometryComponentType_CircularArcSegment,
        FdoGeometryComponentType_LineStringSegment,
        FdoGeometryComponentType_Ring
    };

    length = sizeof(compTypes) / sizeof(FdoGeometryComponentType);
    return compTypes;
}

FdoInt32 SdfGeometryCapabilities::GetDimensionalities()
{
    //SDF does not care about dimensionality, you can
    //put anything in it
    return FdoDimensionality_XY | FdoDimensionality_Z | FdoDimensionality_M;
}

