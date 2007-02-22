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

#include "mgSpatialGeometryValidity.h"

BEGIN_NAMESPACE_OSGEO_GEOMETRY
public __gc __interface IGeometry;
public __gc __interface ILinearRing;
public __gc __interface ICircularArcSegment;
public __gc class ObjectFactory;
public __gc class GeometryFactoryAbstract;
public __gc __interface IPolygon;
END_NAMESPACE_OSGEO_GEOMETRY

class FdoSpatialUtility;

BEGIN_NAMESPACE_OSGEO_SPATIAL

/// \ingroup (OSGeoFDOSpatial)
/// \brief
/// A Spatial utility class.
public __gc __sealed class SpatialUtility
{
public:
    /// \brief
    /// Approximates a geometry, which may contain parametric curve components,
    /// with a geometry that only contains straight, linear components.
    /// 
    /// \remarks
    /// If the given geometry does not require approximation, this method returns
    /// the input geometry, and thus avoids unnecessary object creation.
    /// Tolerances are in same units as the geometry's ordinates.
    /// If the given tolerances are both zero, a tesselated geometry is produced, 
    /// wherein arcs are represented using only their three defining positions.
    /// Math performed is all in implicit Cartesian (rectilinear) coordinate system.
    /// Measurement ("M") ordinates of interpolated positions undergo linear 
    /// interpolation from an arc's start position to end position (possibly disregarding
    /// an "M" ordinate in the arc's mid position).
    /// Unless the arc is too short for any interpolation, the "mid" point parameter
    /// used to define arcs is not guaranteed to appear among interpolated points.
    /// 
    /// \param geometry 
    /// Input Geometry to approximate
    /// \param maxSpacing 
    /// Input Maximum length of each line segment used to approximate parametric curves. Ignored if zero.
    /// \param maxOffset 
    /// Input Maximum distance that a line segment may be from the curve that it approximates (error tolerance). Ignored if zero.
    /// \param geometryFactory 
    /// Input Geometry factory for the creation of the approximated geometry.
    /// 
    /// \return
    /// Returns An approximation of the given geometry.
    /// 
	static NAMESPACE_OSGEO_GEOMETRY::IGeometry * ApproximateGeometryWithLineStrings(NAMESPACE_OSGEO_GEOMETRY::IGeometry * geometry, System::Double maxSpacing, System::Double maxOffset, NAMESPACE_OSGEO_GEOMETRY::GeometryFactoryAbstract * geometryFactory);

    /// \brief
    /// Tests whether a geometry can be supported using the given types and dimensionality.
    /// 
    /// \param geometry 
    /// Input Geometry to validate
    /// \param geometryTypes 
    /// Input Array of allowed geometry types. NULL is valid if geometryTypesCount is zero.
    /// \param geometryComponentTypes 
    /// Input Array of allowed geometry component types. NULL is valid if geometryComponentTypesCount is zero.
    /// \param dimensionality 
    /// Input Bit-mask of allowed dimensionalities (see Dimensionality).
    /// 
    /// \return
    /// Returns An enumeration indicating support for the given geometry.
    /// 
	static NAMESPACE_OSGEO_SPATIAL::SpatialGeometryValidity ValidateGeometryByType(NAMESPACE_OSGEO_GEOMETRY::IGeometry * geometry, NAMESPACE_OSGEO_COMMON::GeometryType geometryTypes [], NAMESPACE_OSGEO_COMMON::GeometryComponentType geometryComponentTypes [], System::Int32 dimensionality);

    /// \brief
    /// Evaluates if two FDO geometric objects spatially interact with each other based on a user supplied spatial operator.
    /// For example: Contains, Crosses, Disjoint, Equals, Intersects, Overlaps, Touches, Within, CoveredBy, Inside, EnvelopeIntersects.
    /// 
    /// \param g1 
    /// Input Left hand Geometry to Evaluate
    /// \param op 
    /// Input The spatial operation to apply to the left and right hand geometries 
    /// \param g2 
    /// Input Right hand Geometry to Evaluate
    /// 
    /// \return
    /// Returns The tesselated Geometry.
    /// 
	static System::Boolean Evaluate(NAMESPACE_OSGEO_GEOMETRY::IGeometry * g1, NAMESPACE_OSGEO_FDO_FILTER::SpatialOperations op, NAMESPACE_OSGEO_GEOMETRY::IGeometry * g2);

    /// \brief
    /// Tesselates a curve geometry into a set of line strings that approximate the curve geometry.
    /// 
    /// \param curve 
    /// Input Geometry to tesselate 
    /// 
    /// \return
    /// Returns The tesselated Geometry.
    /// 
	static NAMESPACE_OSGEO_GEOMETRY::IGeometry* TesselateCurve( NAMESPACE_OSGEO_GEOMETRY::IGeometry* curve);

    /// \brief
    /// Gets the extents of a geometry contained in a byte array.
    /// 
    /// \param bytes 
    /// Input Geometry whose extents will be returned
    /// \param minX 
    /// Output The minimim x position of the geometry
    /// \param minY 
    /// Output The minimim y position of the geometry
    /// \param maxX 
    /// Output The maximum x position of the geometry
    /// \param maxY 
    /// Output The maximum y position of the geometry
    /// 
	static System::Void GetExtents(System::Byte bytes [],  System::Double* minX, System::Double* minY, System::Double* maxX,  System::Double* maxY);

    /// \brief
    /// Gets the extents of a geometry contained in a byte array.
    /// 
    /// \param bytes 
    /// Input Geometry whose extents will be returned
    /// \param minX 
    /// Output The minimim x position of the geometry
    /// \param minY 
    /// Output The minimim y position of the geometry
    /// \param minZ 
    /// Output The minimim z position of the geometry
    /// \param maxX 
    /// Output The maximum x position of the geometry
    /// \param maxY 
    /// Output The maximum y position of the geometry
    /// \param maxZ 
    /// Output The maximum z position of the geometry
    /// 
	static System::Void GetExtents(System::Byte bytes [],  System::Double* minX, System::Double* minY, System::Double* minZ, System::Double* maxX,  System::Double* maxY, System::Double* maxZ);

    /// \brief
    /// Tests whether a point is within a ring or not.
    /// 
    /// \param ring 
    /// Input Ring to test
    /// \param coordinateX 
    /// Input X ordinate.
    /// \param coordinateY 
    /// Input Y ordinate.
    /// \param isOnBoundary 
    /// Output Specifies if the specified point is on the boundary of the ring.
    /// 
    /// \return
    /// Returns TRUE if the point is within ring or on its boundary, FALSE otherwise.
    /// 
	static System::Boolean PointInRing( NAMESPACE_OSGEO_GEOMETRY::ILinearRing* ring, System::Double coordinateX, System::Double coordinateY, System::Boolean* isOnBoundary);

    /// \brief
    /// Tests whether a point is within a ring or not.
    /// 
    /// \param ring 
    /// Input Ring to test
    /// \param coordinateX 
    /// Input X ordinate.
    /// \param coordinateY 
    /// Input Y ordinate.
    /// 
    /// \return
    /// Returns TRUE if the point is within ring or on its boundary, FALSE otherwise.
    /// 
    static System::Boolean PointInRing( NAMESPACE_OSGEO_GEOMETRY::ILinearRing* ring, System::Double coordinateX, System::Double coordinateY);

    /// \brief
    /// Tests whether a point is within a polygon (including its islands) or not.
    /// 
    /// \param polygon 
    /// Input Polygon to test
    /// \param coordinateX 
    /// Input X ordinate.
    /// \param coordinateY 
    /// Input Y ordinate.
    /// \param isOnExtBoundary 
    /// Output Specifies if the specified point is on the exterior boundary of the polygon.
    /// \param isOnInBoundary 
    /// Output Specifies if the specified point is on the interior boundary of the polygon.
    /// 
    /// \return
    /// Returns TRUE if the point is within polygon or on its boundary, FALSE otherwise.
    /// 
	static System::Boolean PointInPolygon(NAMESPACE_OSGEO_GEOMETRY::IPolygon* polygon, System::Double coordinateX, System::Double coordinateY, System::Boolean * isOnExtBoundary, System::Boolean * isOnInBoundary);

    /// \brief
    /// Tests whether a point is within a polygon (including its islands) or not.
    /// 
    /// \param polygon 
    /// Input Polygon to test
    /// \param coordinateX 
    /// Input X ordinate.
    /// \param coordinateY 
    /// Input Y ordinate.
    /// \param isOnExtBoundary 
    /// Output Specifies if the specified point is on the exterior boundary of the polygon.
    /// 
    /// \return
    /// Returns TRUE if the point is within polygon or on its boundary, FALSE otherwise.
    /// 
    static System::Boolean PointInPolygon(NAMESPACE_OSGEO_GEOMETRY::IPolygon* polygon, System::Double coordinateX, System::Double coordinateY, System::Boolean * isOnExtBoundary);

    /// \brief
    /// Tests whether a point is within a polygon (including its islands) or not.
    /// 
    /// \param polygon 
    /// Input Polygon to test
    /// \param coordinateX 
    /// Input X ordinate.
    /// \param coordinateY 
    /// Input Y ordinate.
    /// 
    /// \return
    /// Returns TRUE if the point is within polygon or on its boundary, FALSE otherwise.
    /// 
    static System::Boolean PointInPolygon(NAMESPACE_OSGEO_GEOMETRY::IPolygon* polygon, System::Double coordinateX, System::Double coordinateY);

    /// \brief
    /// Computes the area of a ring.
    /// 
    /// \param ring 
    /// Input Ring to test
    /// 
    /// \return
    /// Returns The area. The value may be positive (counterclockwise orientation) or
    /// negative (clockwise orientation) 
    /// 
	static System::Double ComputeLinearRingArea( NAMESPACE_OSGEO_GEOMETRY::ILinearRing* ring );

    /// \brief
    /// Computes the length (perimeter) of a ring.
    /// 
    /// \param ring 
    /// Input Ring to test
    /// 
    /// \return
    /// Returns The length.
    /// 
	static System::Double ComputeLinearRingLength( NAMESPACE_OSGEO_GEOMETRY::ILinearRing* ring );

    /// \brief
    /// Creates a Polygon or a MultiPolygon from a collection of rings.
    /// 
    /// \remarks
    /// /// 
    /// 
    /// \param rings 
    /// Input Collection of rings
    /// \param relateRings 
    /// Input Flag indicating if polygon formation is desired.
    /// If not desired (FALSE) a MultiPolygon containing only external polygons will be returned.
    /// When TRUE, the rings will be associated with each other, resulting in a polygon(s) with
    /// exterior/interior rings.
    /// 
    /// \return
    /// Returns The geometry containing the input rings.
    /// 
    /// Given a collection of rings the method may return, depending on flag relateRings:
	static NAMESPACE_OSGEO_GEOMETRY::IGeometry* CreateGeometryFromRings( NAMESPACE_OSGEO_GEOMETRY::LinearRingCollection* rings, System ::Boolean relateRings );

    /// \brief
    /// Checks the geometric validity of a circular arc.
    /// E.g. Checks whether the points describing the arc are collinear within a tolerance.
    /// 
    /// \param arc 
    /// Input Circular arc to test
    /// \param tolerance 
    /// Input Allowed tolerance
    /// 
    /// \return
    /// Returns Flag indicating whether the arc has collinear points.
    /// 
	static System::Boolean IsCircularArcValid( NAMESPACE_OSGEO_GEOMETRY::ICircularArcSegment *arc, System::Double tolerance );

private:
	SpatialUtility(){}
};

END_NAMESPACE_OSGEO_SPATIAL


