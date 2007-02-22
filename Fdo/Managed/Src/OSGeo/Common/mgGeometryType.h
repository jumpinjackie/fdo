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

/// \cond DOXYGEN-IGNORE
BEGIN_NAMESPACE_OSGEO_COMMON
/// \endcond

/// \ingroup (OSGeoFDOCommon)
/// \brief
/// GeometryType is an enumeration to denote the type of a Geometry.
/// \remarks
/// This enumeration applies to types that derive from IGeometry.
public __value enum GeometryType
{
    /// Indicates no specific type; used for "unknown", "don't care" or an incompletely constructed Geometry object.
    GeometryType_None = FdoGeometryType_None,

    /// Point type 
    GeometryType_Point = FdoGeometryType_Point,

    /// LineString type 
    GeometryType_LineString = FdoGeometryType_LineString,

    /// Polygon type 
    GeometryType_Polygon = FdoGeometryType_Polygon,

    /// MultiPoint type 
    GeometryType_MultiPoint = FdoGeometryType_MultiPoint,
    
    /// MultiLineString type 
    GeometryType_MultiLineString = FdoGeometryType_MultiLineString,
    
    /// MultiPolygon type 
    GeometryType_MultiPolygon = FdoGeometryType_MultiPolygon,
    
    /// MultiGeometry type
    GeometryType_MultiGeometry = FdoGeometryType_MultiGeometry,
    
    /// CurveString type 
    GeometryType_CurveString = FdoGeometryType_CurveString,
    
    /// CurvePolygon type 
    GeometryType_CurvePolygon = FdoGeometryType_CurvePolygon,
    
    /// MultiCurveString type 
    GeometryType_MultiCurveString = FdoGeometryType_MultiCurveString,
    
    /// MultiCurvePolygon type 
    GeometryType_MultiCurvePolygon = FdoGeometryType_MultiCurvePolygon
};

/// \ingroup (OSGeoFDOCommon)
/// \brief
/// Enumeration for components of Geometry.
/// \remarks
/// This enumeration applies to certain helper types that are used to compose other types which derive from IGeometry.
public __value enum GeometryComponentType
{
    /// LinearRing type
    GeometryComponentType_LinearRing = FdoGeometryComponentType_LinearRing,
    
    /// CircularArcSegment type 
    GeometryComponentType_CircularArcSegment = FdoGeometryComponentType_CircularArcSegment,
    
    /// LineStringSegment type 
    GeometryComponentType_LineStringSegment = FdoGeometryComponentType_LineStringSegment,
    
    /// Ring type 
    GeometryComponentType_Ring = FdoGeometryComponentType_Ring
};

/// \cond DOXYGEN-IGNORE
END_NAMESPACE_OSGEO_COMMON
/// \endcond


