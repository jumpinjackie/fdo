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
*
*/

#pragma once

BEGIN_NAMESPACE_OSGEO_COMMON

/// \brief
///  GeometryType is an enumeration to denote the type of a Geometry.
/// \remarks
///  This enumeration applies to types that derive from IGeometry.
/// <param name="GeometryType_None">
/// Indicates no specific type; used for "unknown", "don't care" or an incompletely constructed Geometry object.
/// </param>
/// <param name="GeometryType_Point">
/// Point type 
/// </param>
/// <param name="GeometryType_LineString">
/// LineString type 
/// </param>
/// <param name="GeometryType_Polygon">
/// Polygon type 
/// </param>
/// <param name="GeometryType_MultiPoint">
/// MultiPoint type 
/// </param>
/// <param name="GeometryType_MultiLineString">
/// MultiLineString type 
/// </param>
/// <param name="GeometryType_MultiPolygon">
/// MultiPolygon type 
/// </param>
/// <param name="GeometryType_MultiGeometry">
/// MultiGeometry type
/// </param>
/// <param name="GeometryType_CurveString">
/// CurveString type 
/// </param>
/// <param name="GeometryType_CurvePolygon">
/// CurvePolygon type 
/// </param>
/// <param name="GeometryType_MultiCurveString">
/// MultiCurveString type 
/// </param>
/// <param name="GeometryType_MultiCurvePolygon">
/// MultiCurvePolygon type 
/// </param>
public __value enum GeometryType
{
    GeometryType_None = FdoGeometryType_None,
    GeometryType_Point = FdoGeometryType_Point,
    GeometryType_LineString = FdoGeometryType_LineString,
    GeometryType_Polygon = FdoGeometryType_Polygon,
    GeometryType_MultiPoint = FdoGeometryType_MultiPoint,
    GeometryType_MultiLineString = FdoGeometryType_MultiLineString,
    GeometryType_MultiPolygon = FdoGeometryType_MultiPolygon,
    GeometryType_MultiGeometry = FdoGeometryType_MultiGeometry,
    GeometryType_CurveString = FdoGeometryType_CurveString,
    GeometryType_CurvePolygon = FdoGeometryType_CurvePolygon,
    GeometryType_MultiCurveString = FdoGeometryType_MultiCurveString,
    GeometryType_MultiCurvePolygon = FdoGeometryType_MultiCurvePolygon
};

/// \brief
///  Enumeration for components of Geometry.
/// \remarks
///  This enumeration applies to certain helper types that are used to compose other types which derive from IGeometry.
/// <param name="GeometryComponentType_LinearRing">
/// LinearRing type
/// </param>
/// <param name="GeometryComponentType_CircularArcSegment">
/// CircularArcSegment type 
/// </param>
/// <param name="GeometryComponentType_LineStringSegment">
/// LineStringSegment type 
/// </param>
/// <param name="GeometryComponentType_Ring">
/// Ring type 
/// </param>
public __value enum GeometryComponentType
{
    GeometryComponentType_LinearRing = FdoGeometryComponentType_LinearRing,
    GeometryComponentType_CircularArcSegment = FdoGeometryComponentType_CircularArcSegment,
    GeometryComponentType_LineStringSegment = FdoGeometryComponentType_LineStringSegment,
    GeometryComponentType_Ring = FdoGeometryComponentType_Ring
};

END_NAMESPACE_OSGEO_COMMON


