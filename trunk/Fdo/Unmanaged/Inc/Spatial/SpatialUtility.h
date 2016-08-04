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
 */
#ifndef FdoSpatialUtility___H
#define FdoSpatialUtility___H	1

#include <FdoGeometry.h>

#include <Spatial/SpatialGeometryValidity.h>
#include <Fdo/Filter/SpatialCondition.h>
#include <Fdo/Schema/PolygonVertexOrderRule.h>

typedef struct RingArea_def
{
    FdoInt32    index;
    FdoInt32    indexAssoc;
    double      area;
} RingArea_def;

/// \ingroup (enums)
/// \brief
/// FdoFixPolygonVertexOrderAction is an enumeration of the action taken 
/// when copying polygon from a source provider to a target provider.
///
enum FdoPolygonVertexOrderAction
{
    /// No processing.
    FdoPolygonVertexOrderAction_None = 0,

    /// Reverse polygon vertex.
    FdoPolygonVertexOrderAction_Reverse = 1,

    /// Check polygon vertex order and reverse it if necessary.
    FdoPolygonVertexOrderAction_CheckAndReverse = 2
};


/// \brief
/// Spatial utility class.
class FdoSpatialUtility
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
    FDO_SPATIAL_API static FdoIGeometry * ApproximateGeometryWithLineStrings(
        FdoIGeometry * geometry, 
        double maxSpacing,
        double maxOffset,
        FdoGeometryFactoryAbstract * geometryFactory);

    /// \brief
    /// Tests whether a geometry can be supported using the given types and dimensionality.
    /// 
    /// \param geometry 
    /// Input Geometry to validate
    /// \param geometryTypesCount 
    /// Input Number of elements in the array given by geometryTypes. If zero, no constraint is used.
    /// \param geometryTypes 
    /// Input Array of allowed geometry types. NULL is valid if geometryTypesCount is zero.
    /// \param geometryComponentTypesCount 
    /// Input Number of elements in the array given by geometryComponentTypes. If zero, no constraint is used.
    /// \param geometryComponentTypes 
    /// Input Array of allowed geometry component types. NULL is valid if geometryComponentTypesCount is zero.
    /// \param dimensionality 
    /// Input Bit-mask of allowed dimensionalities (see FdoDimensionality).
    /// 
    /// \return
    /// Returns An enumeration indicating support for the given geometry.
    /// 
    FDO_SPATIAL_API static FdoSpatialGeometryValidity ValidateGeometryByType(
        FdoIGeometry *              geometry, 
        FdoInt32                    geometryTypesCount,
        FdoGeometryType *           geometryTypes,
        FdoInt32                    geometryComponentTypesCount,
        FdoGeometryComponentType *  geometryComponentTypes,
        FdoInt32                    dimensionality);

/// \cond DOXYGEN-IGNORE
    static bool ArePositionsEqualXYZ(
        FdoIDirectPosition * p1,
        FdoIDirectPosition * p2);

    static bool ArePositionsEqualXY(
        FdoIDirectPosition * p1,
        FdoIDirectPosition * p2);

    static double DistanceBetweenPositionsXYZ(
        FdoIDirectPosition * p1,
        FdoIDirectPosition * p2);

    static double DistanceBetweenPositionsXY(
        FdoIDirectPosition * p1,
        FdoIDirectPosition * p2);

    /// Unit normal from plane defined by vectors p1p2 to p2p3 using right-hand rule.
    /// This will return (0,0,0) for parallel vectors (collinear points) or zero-length
    /// vectors.
    static FdoDirectPositionImpl * UnitNormalVector(
        FdoIDirectPosition * p1,
        FdoIDirectPosition * p2,
        FdoIDirectPosition * p3);

    static void AppendPositionToDistinctCollection(
        FdoDirectPositionCollection * distinctPositions,
        FdoIDirectPosition * positionToAppend);

    /// Return a new point with the location of the given point after rotation
    /// about the given axis and center.
    /// This method assumes that the involved data has XYZ dimensionality.
    /// \note
	/// The "axisVector" must be a unit normal to the plane of rotation.
    static FdoDirectPositionImpl * RotatePosition(
        FdoIDirectPosition * position,
        double angle,
        FdoIDirectPosition * axisVector,
        FdoIDirectPosition * center);

protected:

    FdoSpatialUtility() {};
    ~FdoSpatialUtility() {};

    static void AppendLinearizedCurveSegmentsToDistinctCollection(
        FdoDirectPositionCollection * distinctPositions,
        FdoCurveSegmentCollection * curveSegments,
        double maxSpacing,
        double maxOffset);

    static void AppendPositionsToDistinctCollection(
        FdoDirectPositionCollection * distinctPositions,
        FdoDirectPositionCollection * positionsToAppend);
/// \endcond

public:

    /// \brief
    /// Evaluates if two FDO geometric objects spatially interact with each other 
    /// based on a user supplied spatial operator. For example: Contains, Crosses, 
    /// Disjoint, Equals, Intersects, Overlaps, Touches, Within, CoveredBy, Inside, 
    /// EnvelopeIntersects.
    /// 
    /// \param g1 
    /// Input Left hand Geometry to Evaluate
    /// \param op 
    /// Input The spatial operation to apply to the left and right hand geometries 
    /// \param g2 
    /// Input Right hand Geometry to Evaluate
    /// 
    /// \return
    /// Returns the evaluation of spatial operation.
    FDO_SPATIAL_API static bool Evaluate(FdoIGeometry* g1, FdoSpatialOperations op, FdoIGeometry* g2);

    /// \brief
    /// Evaluates if two FDO geometric objects spatially interact with each other 
    /// based on a user supplied spatial operator. For example: Contains, Crosses, 
    /// Disjoint, Equals, Intersects, Overlaps, Touches, Within, CoveredBy, Inside, 
    /// EnvelopeIntersects.
    /// 
    /// \param g1 
    /// Input Left hand Geometry to Evaluate
    /// \param op 
    /// Input The spatial operation to apply to the left and right hand geometries 
    /// \param g2 
    /// Input Right hand Geometry to Evaluate
    /// 
    /// \param toleranceXY
    /// Input XY tolerance to evaluate the spatial condition
    /// Default tolerance used is 1e-10. Valid range is >0 
    /// If an invalid value is provided, the default then will be used
    /// 
    /// \return
    /// Returns the evaluation of spatial operation.
    FDO_SPATIAL_API static bool Evaluate(FdoIGeometry* g1, FdoSpatialOperations op, FdoIGeometry* g2, double toleranceXY); 

    /// \brief
    /// Evaluates if two FDO geometric objects spatially interact with each other 
    /// based on a user supplied spatial operator. For example: Contains, Crosses, 
    /// Disjoint, Equals, Intersects, Overlaps, Touches, Within, CoveredBy, Inside, 
    /// EnvelopeIntersects.
    /// 
    /// \param g1 
    /// Input Left hand Geometry to Evaluate
    /// \param op 
    /// Input The spatial operation to apply to the left and right hand geometries 
    /// \param g2 
    /// Input Right hand Geometry to Evaluate
    /// 
    /// \param toleranceXY
    /// Input XY tolerance to evaluate the spatial condition
    /// Default tolerance used is 1e-10. Valid range is >0 
    /// If an invalid value is provided, the default then will be used
    /// 
    /// \param toleranceZ
    /// Input Z tolerance to evaluate the spatial condition in case we have 3D geometries
    /// Default tolerance used is 1e-10. Valid range is >0
    /// If an invalid value is provided, the default then will be used
    /// 
    /// \return
    /// Returns the evaluation of spatial operation.
    FDO_SPATIAL_API static bool Evaluate(FdoIGeometry* g1, FdoSpatialOperations op, FdoIGeometry* g2, double toleranceXY, double toleranceZ); 

    FDO_SPATIAL_API static FdoIGeometry* TesselateCurve(FdoIGeometry* curve);

    FDO_SPATIAL_API static void GetExtents(FdoByteArray* fgf, double& minx, double& miny, double& maxx, double& maxy);

    FDO_SPATIAL_API static void GetExtents(FdoByteArray* fgf, double& minx, double& miny, double& minz, double& maxx, double& maxy, double& maxz);

    static void AdjustExtentsForCurves(FdoIGeometry * geometry, double& minx, double& miny, double& maxx, double& maxy);
    static void AdjustExtentsForCurves(FdoCurveSegmentCollection * csc, double& minx, double& miny, double& maxx, double& maxy);

    static bool Intersects(FdoIGeometry* g1, FdoIGeometry* g2, double toleranceXY);
    static bool Disjoint(FdoIGeometry* g1, FdoIGeometry* g2, double toleranceXY);
    static bool Overlaps(FdoIGeometry* g1, FdoIGeometry* g2, double toleranceXY);
    static bool Touches(FdoIGeometry* g1, FdoIGeometry* g2, double toleranceXY);
    static bool Contains(FdoIGeometry* g1, FdoIGeometry* g2, double toleranceXY, bool strictInside = false);
    static bool Crosses(FdoIGeometry* g1, FdoIGeometry* g2, double toleranceXY);
    static bool Equals(FdoIGeometry* g1, FdoIGeometry* g2, double toleranceXY);
    static bool Within(FdoIGeometry* g1, FdoIGeometry* g2, double toleranceXY);
    static bool CoveredBy(FdoIGeometry* g1, FdoIGeometry* g2, double toleranceXY);
    static bool Inside(FdoIGeometry* g1, FdoIGeometry* g2, double toleranceXY);
    static bool EnvelopeIntersects(FdoIGeometry* g1, FdoIGeometry* g2, double toleranceXY);

    //Intersects
    static bool MultiPolygonIntersects(FdoIMultiPolygon* mpoly, FdoIGeometry* geom, double toleranceXY);
    static bool MultiLineStringIntersects(FdoIMultiLineString* mline, FdoIGeometry* geom, double toleranceXY);
    static bool MultiPointIntersects(FdoIMultiPoint* mpoint, FdoIGeometry* geom, double toleranceXY);

    static bool PolygonIntersects(FdoIPolygon* poly, FdoIGeometry* geom, double toleranceXY);
    static bool LineStringIntersects(FdoILineString* line, FdoIGeometry* geom, double toleranceXY);
    static bool PointIntersects(FdoIPoint* point, FdoIGeometry* geom, double toleranceXY);

    //Contains
    static bool MultiPolygonContains(FdoIMultiPolygon* mpoly, FdoIGeometry* geom, double toleranceXY);
    static bool MultiLineStringContains(FdoIMultiLineString* mline, FdoIGeometry* geom, double toleranceXY);
    static bool MultiPointContains(FdoIMultiPoint* mpoly, FdoIGeometry* geom, double toleranceXY);

    static bool PolygonContains(FdoIPolygon* poly, FdoIGeometry* geom, double toleranceXY, bool strictInside = false);
    static bool LineStringContains(FdoILineString* line, FdoIGeometry* geom, double toleranceXY);
    static bool PointContains(FdoIPoint* point, FdoIGeometry* geom, double toleranceXY);

    //Utils

    /// \brief
    /// Tests whether a point is within a ring or not.
    /// 
    /// \param ring 
    /// Input Ring to test
    /// \param x 
    /// Input X ordinate.
    /// \param y 
    /// Input Y ordinate.
    /// 
    /// \return
    /// Returns TRUE if the point is within ring or on its boundary, FALSE otherwise.
    /// 
    FDO_SPATIAL_API static bool PointInRing(FdoILinearRing* ring, double x, double y, bool* isOnBoundary = NULL);

    /// \brief
    /// Tests whether a point is within a ring or not.
    /// 
    /// \param ring 
    /// Input Ring to test
    /// \param x 
    /// Input X ordinate.
    /// \param y 
    /// Input Y ordinate.
    /// \param toleranceXY
    /// Input XY tolerance to evaluate the spatial condition
    /// Default tolerance used is 1e-10. Valid range is >0
    /// If an invalid value is provided, the default then will be used
    /// 
    /// \return
    /// Returns TRUE if the point is within ring or on its boundary, FALSE otherwise.
    /// 
    FDO_SPATIAL_API static bool PointInRing(FdoILinearRing* ring, double x, double y, double toleranceXY, bool* isOnBoundary = NULL);

    /// \brief
    /// Tests whether a point is within a polygon (including its islands) or not.
    /// 
    /// \param poly 
    /// Input Polygon to test
    /// \param x 
    /// Input X ordinate.
    /// \param y 
    /// Input Y ordinate.
    /// 
    /// \return
    /// Returns TRUE if the point is within polygon or on its boundary, FALSE otherwise.
    /// 
    FDO_SPATIAL_API static bool PointInPolygon(FdoIPolygon* poly, double x, double y, bool* isOnExtBoundary = NULL, bool* isOnIntBoundary = NULL);

    /// \brief
    /// Tests whether a point is within a polygon (including its islands) or not.
    /// 
    /// \param poly 
    /// Input Polygon to test
    /// \param x 
    /// Input X ordinate.
    /// \param y 
    /// Input Y ordinate.
    /// \param toleranceXY
    /// Input XY tolerance to evaluate the spatial condition
    /// Default tolerance used is 1e-10. Valid range is >0
    /// If an invalid value is provided, the default then will be used
    /// 
    /// \return
    /// Returns TRUE if the point is within polygon or on its boundary, FALSE otherwise.
    /// 
    FDO_SPATIAL_API static bool PointInPolygon(FdoIPolygon* poly, double x, double y, double toleranceXY, bool* isOnExtBoundary = NULL, bool* isOnIntBoundary = NULL);

    static bool PointOnLine(FdoILineString* line, double x, double y, double toleranceXY);
    static bool PolygonsIntersect(FdoIPolygon* poly1, FdoIPolygon* poly2, double toleranceXY);
    static bool LineStringsIntersect(FdoILineString* line1, FdoILineString* line2, double toleranceXY);
    static bool PolygonIntersectsLineString(FdoIPolygon* poly, FdoILineString* line, double toleranceXY);
    static bool PolygonContainsLineString(FdoIPolygon* poly, FdoILineString* line, double toleranceXY, bool strictInside = false);
    static bool PolygonContainsPolygon(FdoIPolygon* poly1, FdoIPolygon* poly2, double toleranceXY, bool strictInside = false);
    static bool LineStringContainsLineString(FdoILineString* line1, FdoILineString* line2, double toleranceXY);
    static bool PointInRingFast(FdoILinearRing* ring, double x, double y, double toleranceXY);

    FDO_SPATIAL_API static bool LinearRingIsClockwise(FdoILinearRing* ring);

    static bool RingIsClockwise(FdoIRing* ring);

    /// \brief
    /// Computes the area of a ring.
    /// 
    /// \param poly 
    /// Input Ring to test
    /// 
    /// \return
    /// Returns The area. The value may be positive (counterclockwise orientation) or
    /// negative (clockwise orientation) 
    /// 
    FDO_SPATIAL_API static double ComputeLinearRingArea( FdoILinearRing* ring );

    /// \brief
    /// Computes the length (perimeter) of a ring.
    /// 
    /// \param poly 
    /// Input Ring to test
    /// 
    /// \return
    /// Returns The length.
    /// 
    FDO_SPATIAL_API static double ComputeLinearRingLength( FdoILinearRing* ring );

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
    FDO_SPATIAL_API static FdoIGeometry* CreateGeometryFromRings( FdoLinearRingCollection* rings, bool relateRings );

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
    FDO_SPATIAL_API static bool IsCircularArcValid( FdoICircularArcSegment *arc, double tolerance );

    /// \brief
    /// Checks whether the vertex order of the input polygon follows the specified
    /// vertex order rule. If not, fix it.
    ///
    /// \param geometry
    /// Input the polygon geometry to be fixed. It can be a polygon, multipolygon,
    /// curvepolygon, or multicurvepolygon.
    ///
    /// \return
    /// Returns the modified polygon.  
    ///
    FDO_SPATIAL_API static FdoIGeometry* FixPolygonVertexOrder (FdoIGeometry * geometry, FdoPolygonVertexOrderRule vertexOrderRule );

    /// \brief
    /// Reverses the vertex order of the input polygon.
    ///
    /// \param geometry
    /// Input the polygon geometry to be reversed. It can be a polygon, multipolygon,
    /// curvepolygon, or multicurvepolygon.
    ///
    /// \return
    /// Returns the modified polygon. 
    ///
    FDO_SPATIAL_API static FdoIGeometry* ReversePolygonVertexOrder ( FdoIGeometry * geometry );

    /// \brief
    /// Gets the vertex order of the input polygon.
    ///
    /// \param geometry
    /// Input geometry to be checked. It can be a polygon, multipolygon,
    /// curvepolygon, or multicurvepolygon.
    /// \return
    /// Returns the vertex order of the input polygon.
    /// 
    FDO_SPATIAL_API static FdoPolygonVertexOrderRule CheckPolygonVertexOrder(FdoIGeometry* geometry);

    /// \brief
    /// Given the vertex order and strictness rule of the source and the target,
    /// get the action taken when copying polygon from a source provider to 
    /// a target provider.
    ///
    /// \param sourceVertexOrderRule
    /// Input the vertex order rule of the source.
    /// \param sourceStrictnessRule
    /// Input the vertex order strictness rule of the source.
    /// \param targetVertexOrderRule
    /// Input the vertex order rule of the target.
    /// \param targetStrictnessRule
    /// Input the vertex order strictness rule of the target.
    /// 
    /// \return
    /// Returns the action taken when copying polygon from a source provider 
    /// to a target provider.
    /// 
    FDO_SPATIAL_API static FdoPolygonVertexOrderAction GetPolygonVertexOrderAction(
        FdoPolygonVertexOrderRule sourceVertexOrderRule,
        FdoBoolean sourceStrictnessRule,
        FdoPolygonVertexOrderRule targetVertexOrderRule,
        FdoBoolean targetStrictnessRule);

private:
    static bool pt_is_on_line(
                double      LineX1,
                double      LineY1,
                double      LineX2,
                double      LineY2,
                double      Dx,
                double      Dy,
                double      Len,
                double      LenSq,
                double      PtX,
                double      PtY,
                bool *      IsAtLinePt1_O,
                bool *      IsAtLinePt2_O,
                double      toleranceXY);

    static bool pt_is_on_line2(
                double      LineX1,
                double      LineY1,
                double      LineX2,
                double      LineY2,
                double      PtX,
                double      PtY,
                double      toleranceXY);

    static int find_xsect_seg_seg(
                double      x11,
                double      y11,
                double      x12,
                double      y12,
                double      x21,
                double      y21,
                double      x22,
                double      y22,
                double *    xi1_O,
                double *    yi1_O,
                int *       IsInterp1_O,
                double *    xi2_O,
                double *    yi2_O,
                int *       IsInterp2_O,
                double      toleranceXY,
                bool*       isIntersectOnly = NULL);

    static int line_segment_intersect(
                double* line0, 
                double* line1,  
                double* ret,
                double toleranceXY,
                bool*  isIntersectOnly = NULL);

    static bool line_contains_line(
                double* line0, 
                double* line1,
                double  toleranceXY,
                bool*   linesAreEq = NULL);

    static double pt_dist_to_seg(
                double      PtX,
                double      PtY,
                double      LineX1,
                double      LineY1,
                double      LineX2,
                double      LineY2,
                double      toleranceXY);

    static bool point_in_ring(
                FdoILinearRing* ring, 
                double      x, 
                double      y, 
                bool        strict, 
                bool*       isOnBoundary,
                double      toleranceXY);

    static bool PointInRing2( 
                FdoILinearRing* ring, 
                bool            exteriorRing, 
                bool            strictInside,
                double          x, 
                double          y, 
                bool*           pt_on_ring,
                double          toleranceXY);

    static  int outcode( 
                double			x,		
                double			y,		
                FdoIEnvelope*   extent,
                double          toleranceXY);	
    
    static  int outcode2( 
                double			x,		
                double			y,		
	            double		LineX1,
	            double		LineY1,
	            double		LineX2,
	            double		LineY2,
                double      toleranceXY);	

    static bool check_xtnt_tol (
                double      xmin1,
                double      ymin1,
                double      xmax1,
                double      ymax1,
                double      xmin2,
                double      ymin2,
                double      xmax2,
                double      ymax2,
                int 		*xflag,
                int 		*yflag,
                double		tol);

    static bool PtIsOnOrLeftOfLineTol(
	            double		LineX1,
	            double		LineY1,
	            double		LineX2,
	            double		LineY2,
	            double		Dx,
	            double		Dy,
	            double		Len,
	            double		LenSq,
	            double		PtX,
	            double		PtY,
	            double		Tol,
	            bool *		IsOnLine_O,
	            bool *		IsLeftOfLine_O,
	            bool *		IsAtLinePt1_O,
	            bool *		IsAtLinePt2_O );

    static int  CompareByArea( const void* elem1, const void* elem2 );

    static FdoILinearRing* ReverseLinearRingVertexOrder( FdoILinearRing* linearRing );

    static FdoIRing* ReverseRingVertexOrder(FdoIRing* ring);

    static FdoIPolygon* ReversePolygonVertexOrder(FdoIPolygon* polygon);

    static FdoICurvePolygon* ReversePolygonVertexOrder(FdoICurvePolygon* polygon);

    static FdoPolygonVertexOrderRule CheckPolygonVertexOrder(FdoIPolygon* polygon);

    static FdoPolygonVertexOrderRule CheckPolygonVertexOrder(FdoICurvePolygon* polygon);

    static FdoIPolygon* FixPolygonVertexOrder( FdoIPolygon * polygon, FdoPolygonVertexOrderRule vertexOrderRule );

    static FdoICurvePolygon* FixPolygonVertexOrder( FdoICurvePolygon * polygon, FdoPolygonVertexOrderRule vertexOrderRule );

    // PRIMITIVE Crosses
    // methods used to search avoiding touch or overlap, so search excluding start and end points
    static bool PointStrictInsideLine(double x, double y, FdoILineString* line, double toleranceXY);

    static bool PointStrictInsidePolygon(double x, double y, FdoIPolygon* poly, double toleranceXY);
    
    static bool LineStrictIntersectLine(FdoILineString* line1, FdoILineString* line2, double toleranceXY);

    static bool PolygonStrictIntersectsLine(FdoIPolygon* poly, FdoILineString* line, double toleranceXY);

    static bool PointStrictInsideGeometry(double x, double y, FdoIGeometry* g, double toleranceXY);
    
    static bool LineStrictIntersectGeometry(FdoILineString* line1, FdoIGeometry* g, double toleranceXY);

    // PRIMITIVE Overlaps
    static bool LineOverlapsLine(FdoILineString* line1, FdoILineString* line2, double toleranceXY);
    
    static bool PointStrictOutsideLine(double x, double y, FdoILineString* line, double toleranceXY);

    static bool PolygonOverlapsPolygon(FdoIPolygon* poly1, FdoIPolygon* poly2, double toleranceXY, bool* allowSameInt = NULL);

    static bool LineOverlaps(FdoILineString* line, FdoIGeometry* g, double toleranceXY);

    static bool PolygonOverlaps(FdoIPolygon* poly, FdoIGeometry* g, double toleranceXY);

    static int PolygonOverlapsLine(FdoIPolygon* poly, FdoILineString* line, double toleranceXY);

    static bool PolygonContainsPolygonSegment(FdoIPolygon* poly1, FdoIPolygon* poly2, double toleranceXY);

    // PRIMITIVE Touches
    static int PointTouchesGeometry(double x, double y, FdoIGeometry* geom, double toleranceXY);

    static int LineTouchesGeometry(FdoILineString* line, FdoIGeometry* geom, double toleranceXY);

    static int PolygonTouchesGeometry(FdoIPolygon* poly, FdoIGeometry* geom, double toleranceXY);

    static int PointTouchesLine(double x, double y, FdoILineString* line, double toleranceXY);

    static int LineTouchesPolygon(FdoILineString* line, FdoIPolygon* poly, double toleranceXY);

    static int PolygonTouchesPolygon(FdoIPolygon* poly1, FdoIPolygon* poly2, double toleranceXY);

    static int PointTouchesPolygon(double x, double y, FdoIPolygon* poly, double toleranceXY);

    static int LineTouchesLine(FdoILineString* line1, FdoILineString* line2, double toleranceXY);

    static bool SegmentTouchesPolygon(double* line, FdoIPolygon* poly, double toleranceXY);
};
#endif // FdoSpatialUtility___H


