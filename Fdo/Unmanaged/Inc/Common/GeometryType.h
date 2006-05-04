#ifndef _GEOMETRYTYPE_H_
#define _GEOMETRYTYPE_H_

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

#ifdef __cplusplus
    /// \brief
    ///  FdoGeometryType is an enumeration to denote the type of a Geometry.
    /// 
    /// \remarks
    ///  This enumeration applies to types that derive from FdoIGeometry.
    /// 
    /// \param FdoGeometryType_None 
    /// Indicates no specific type; used for "unknown", "don't care" or an incompletely constructed Geometry object.
    /// \param FdoGeometryType_Point 
    /// Point type (FdoIPoint).
    /// \param FdoGeometryType_LineString 
    /// LineString type (FdoILineString).
    /// \param FdoGeometryType_Polygon 
    /// Polygon type (FdoIPolygon).
    /// \param FdoGeometryType_MultiPoint 
    /// MultiPoint type (FdoIMultiPoint).
    /// \param FdoGeometryType_MultiLineString 
    /// MultiLineString type (FdoIMultiLineString).
    /// \param FdoGeometryType_MultiPolygon 
    /// MultiPolygon type (FdoIMultiPolygon).
    /// \param FdoGeometryType_MultiGeometry 
    /// MultiGeometry type (FdoIMultiGeometry).
    /// \param FdoGeometryType_CurveString 
    /// CurveString type (FdoICurveString).
    /// \param FdoGeometryType_CurvePolygon 
    /// CurvePolygon type (FdoICurvePolygon).
    /// \param FdoGeometryType_MultiCurveString 
    /// MultiCurveString type (FdoIMultiCurveString).
    /// \param FdoGeometryType_MultiCurvePolygon 
    /// MultiCurvePolygon type (FdoIMultiCurvePolygon).
    /// 
    enum FdoGeometryType
#else
    typedef enum
#endif
{
    FdoGeometryType_None = 0,

    FdoGeometryType_Point = 1,

    FdoGeometryType_LineString = 2,

    FdoGeometryType_Polygon = 3,

    FdoGeometryType_MultiPoint = 4,

    FdoGeometryType_MultiLineString = 5,

    FdoGeometryType_MultiPolygon = 6,

    FdoGeometryType_MultiGeometry = 7,

    FdoGeometryType_CurveString = 10,

    FdoGeometryType_CurvePolygon = 11,

    FdoGeometryType_MultiCurveString = 12,
    
    FdoGeometryType_MultiCurvePolygon = 13

#ifdef __cplusplus
};
#else
} FdoGeometryType;
#endif


#ifdef __cplusplus
    /// \brief
    ///  Enumeration for components of Geometry.
    /// 
    /// \remarks
    ///  This enumeration applies to certain helper types that are used to compose other types which derive from FdoIGeometry.
    /// 
    /// \param FdoGeometryComponentType_LinearRing 
    /// LinearRing type (FdoILinearRing).
    /// \param FdoGeometryComponentType_CircularArcSegment 
    /// CircularArcSegment type (FdoICircularArcSegment).
    /// \param FdoGeometryComponentType_LineStringSegment 
    /// LineStringSegment type (FdoILineStringSegment).
    /// \param FdoGeometryComponentType_Ring 
    /// Ring type (FdoIRing).
    /// 
    enum FdoGeometryComponentType
#else
    typedef enum
#endif
{
    FdoGeometryComponentType_LinearRing = 129,

    FdoGeometryComponentType_CircularArcSegment = 130,

    FdoGeometryComponentType_LineStringSegment = 131,

    FdoGeometryComponentType_Ring = 132

#ifdef __cplusplus
};
#else
} FdoGeometryComponentType;
#endif
#endif


