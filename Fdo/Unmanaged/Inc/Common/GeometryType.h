#ifndef _GEOMETRYTYPE_H_
#define _GEOMETRYTYPE_H_

//
// Copyright (C) 2004-2010  Autodesk, Inc.
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
    /// \ingroup (enums)
    /// \brief
    ///  FdoGeometryType is an enumeration to denote the type of a Geometry.
    /// 
    /// \remarks
    ///  This enumeration applies to types that derive from FdoIGeometry.
    enum FdoGeometryType
#else
    typedef enum
#endif
{
    /// Indicates no specific type; used for "unknown", "don't care" or 
    /// an incompletely constructed Geometry object.
    FdoGeometryType_None = 0,

    /// Point type (FdoIPoint).
    FdoGeometryType_Point = 1,

    /// LineString type (FdoILineString).
    FdoGeometryType_LineString = 2,

    /// Polygon type (FdoIPolygon).
    FdoGeometryType_Polygon = 3,

    /// MultiPoint type (FdoIMultiPoint).
    FdoGeometryType_MultiPoint = 4,

    /// MultiLineString type (FdoIMultiLineString).
    FdoGeometryType_MultiLineString = 5,

    /// MultiPolygon type (FdoIMultiPolygon).
    FdoGeometryType_MultiPolygon = 6,

    /// MultiGeometry type (FdoIMultiGeometry).
    FdoGeometryType_MultiGeometry = 7,

    /// CurveString type (FdoICurveString).
    FdoGeometryType_CurveString = 10,

    /// CurvePolygon type (FdoICurvePolygon).
    FdoGeometryType_CurvePolygon = 11,

    /// MultiCurveString type (FdoIMultiCurveString).
    FdoGeometryType_MultiCurveString = 12,
    
    /// MultiCurvePolygon type (FdoIMultiCurvePolygon).
    FdoGeometryType_MultiCurvePolygon = 13

#ifdef __cplusplus
};
#else
} FdoGeometryType;
#endif


#ifdef __cplusplus
    /// \ingroup (enums)
    /// \brief
    ///  Enumeration for components of Geometry.
    /// 
    /// \remarks
    ///  This enumeration applies to certain helper types that 
    ///  are used to compose other types which derive from FdoIGeometry.
    enum FdoGeometryComponentType
#else
    typedef enum
#endif
{
    /// LinearRing type (FdoILinearRing).
    FdoGeometryComponentType_LinearRing = 129,

    /// CircularArcSegment type (FdoICircularArcSegment).
    FdoGeometryComponentType_CircularArcSegment = 130,

    /// LineStringSegment type (FdoILineStringSegment).
    FdoGeometryComponentType_LineStringSegment = 131,

    /// Ring type (FdoIRing).
    FdoGeometryComponentType_Ring = 132,

    /// EllipticalArcSegment type (FdoIEllipticalArcSegment).
    FdoGeometryComponentType_EllipticalArcSegment = 133,

    /// ClothoidSegment type (FdoIClothoidSegment).
    FdoGeometryComponentType_ClothoidSegment = 134,

    /// CubicSplineSegment type (FdoICubicSplineSegment).
    FdoGeometryComponentType_CubicSplineSegment = 135,

    /// BSplineSegment type (FdoIBSplineSegment).
    FdoGeometryComponentType_BSplineSegment = 136,

    /// CircleSegment type (FdoICircleSegment).
    FdoGeometryComponentType_CircleSegment = 137

#ifdef __cplusplus
};
#else
} FdoGeometryComponentType;
#endif


#ifdef __cplusplus
    /// \ingroup (enums)
    /// \brief
    /// Enumeration used to identify the interpolation mechanism used 
    /// by a Geometry.
    /// 
    enum FdoGeometricInterpolationType
#else
    typedef enum
#endif
{
    /// CubicSpline type
    FdoGeometricInterpolationType_CubicSpline = 0,

    /// PolynomialSpline type
    FdoGeometricInterpolationType_PolynomialSpline = 1,

    /// RationalSpline type
    FdoGeometricInterpolationType_RationalSpline = 2

#ifdef __cplusplus
};
#else
} FdoGeometricInterpolationType;
#endif

#ifdef __cplusplus
    /// \ingroup (enums)
    /// \brief
    /// Enumeration used to identify the knot type used to 
    /// ineterpolate a B-Spline.  
    ///
    /// \remarks
    /// A B-spline is uniform if and only if 
    /// all knots are of multiplicity 1 and they differ by a positive 
    /// constant from the preceding knot. A B-spline is quasi-uniform if and 
    /// only if the knots are of multiplicity (degree+1) at the ends, 
    /// of multiplicity 1 elsewhere, and they differ by a positive constant 
    /// from the preceding knot. 
    /// 
    /// In general, in a piecewise Bezier knot vector, the first k knots are 
    /// the same, then each subsequent group of k-1 knots is the same, 
    /// until you get to the end. Note that a piecewise Bézier curve must 
    /// have nk–1 control points, where n is the number of segments 
    /// (i.e., the number of control points is one less than an even 
    /// multiple of the order).
    /// 
    enum FdoGeometricKnotType
#else
    typedef enum
#endif
{
    /// The knots are appropriate for a uniform B-spline. 
    /// e.g. [0 1 2 3 4 5]
    ///
    FdoGeometricKnotType_Uniform = 0,

    /// The knots are appropriate for a quasi-uniform B-spline. 
    /// e.g. [0 0 0 0 1 2 3 3 3 3]
    /// 
    FdoGeometricKnotType_QuasiUniform = 1,

    /// The knots are appropriate for a piecewise Bezier curve. 
    /// e.g. [0 0 0 1 1 2 2 3 3 3]
    ///
    FdoGeometricKnotType_PiecewiseBezier = 2,

#ifdef __cplusplus
};
#else
} FdoGeometricInterpolationType;
#endif

#endif


    