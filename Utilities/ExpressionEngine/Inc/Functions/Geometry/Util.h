#ifndef _FDOEXPRESSIONENGINEGEOMETRYUTIL_H_
#define _FDOEXPRESSIONENGINEGEOMETRYUTIL_H_
/*
* Copyright (C) 2004-2008  Autodesk, Inc.
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

#ifdef _WIN32
#pragma once
#endif

#include <Geometry/GeometryStd.h>
#include <Geometry/Fgf/Factory.h>

class FdoExpressionEngineGeometryUtil
{
public:

    /// \brief
    /// Computes the length of a geometry.
    /// An exception is thrown in case the geometry is not valid.
    /// 
    /// \param computeGeodetic 
    /// Input compute the geodetic distance if TRUE. Otherwise compute the euclidian distance.
    /// 
	/// \param compute3D 
	/// Input compute the 3D distance if TRUE. Otherwise compute 2D distance.
    /// 
    /// \param geometry 
    /// Input the geometry.
    /// 
    /// \param length 
    /// Output computed length.
    /// 
    /// \return
    /// Returns nothing
    /// 
	static void ComputeGeometryLength(FdoBoolean computeGeodetic, FdoBoolean compute3D, FdoIGeometry* geometry, FdoDouble *length);

    /// \brief
    /// Computes the area of a geometry.
    /// An exception is thrown in case the geometry is not valid.
    /// 
    /// \param computeGeodetic 
    /// Input compute geodetic distances if TRUE. Otherwise compute the euclidian distances.
    /// 
	/// \param compute3D 
	/// Input compute the 3D distance if TRUE. Otherwise compute 2D distance.
    /// 
    /// \param geometry 
    /// Input the geometry.
    /// 
    /// \param area
    /// Output computed area.
    /// 
    /// \return
    /// Returns nothing
    /// 
	static void ComputeGeometryArea(FdoBoolean computeGeodetic, FdoBoolean compute3D, FdoIGeometry* geometry, FdoDouble *area);

private:
	static FdoInt32 DimensionalityToNumOrdinates(FdoInt32 dimensionality);

	// Length
	static void ComputeLinearRingLength(FdoBoolean computeGeodetic, FdoBoolean compute3D, FdoILinearRing* lineRing, FdoDouble *length);
	static void ComputeRingLength(FdoBoolean computeGeodetic, FdoBoolean compute3D, FdoIRing* ring, FdoDouble *length);
	static void ComputeCurveSegmentLength(FdoBoolean computeGeodetic, FdoBoolean compute3D, FdoICurveSegmentAbstract* curveSeg, FdoDouble *length);

	static FdoDouble ComputeEuclidianDistance2D(FdoDouble x1, FdoDouble y1, FdoDouble x2, FdoDouble y2 );
	static FdoDouble ComputeGeodeticDistance2D(FdoDouble lat1, FdoDouble lon1, FdoDouble lat2, FdoDouble lon2 );

	static FdoDouble ComputeLength(FdoBoolean computeGeodetic, FdoBoolean compute3D, FdoInt32 numOrdsPerPos, FdoInt32 numOrds, const double *ordinates);
	static FdoDouble ComputeLength2D(FdoBoolean computeGeodetic, FdoInt32 numOrdsPerPos, FdoInt32 numOrds, const double *ordinates);

	// Area
	static void ComputeLinearRingArea(FdoBoolean computeGeodetic, FdoBoolean compute3D, FdoBoolean isExterior, FdoILinearRing* lineRing, FdoDouble *area);
	static void ComputeRingArea(FdoBoolean computeGeodetic, FdoBoolean compute3D, FdoBoolean isExterior, FdoIRing* ring, FdoDouble *area);
	static void ComputeCurveSegmentArea(FdoBoolean computeGeodetic, FdoBoolean compute3D, FdoICurveSegmentAbstract* curveSeg, FdoDouble *area);

	static FdoDouble ComputeArea(FdoBoolean computeGeodetic, FdoBoolean compute3D, FdoInt32 numOrdsPerPos, FdoInt32 numOrds, const double *ordinates);
	static FdoDouble ComputeArea2D(FdoBoolean computeGeodetic, FdoInt32 numOrdsPerPos, FdoInt32 numOrds, const double *ordinates);

	static FdoDouble ComputeEuclidianTrapezoidArea2D(FdoDouble x1, FdoDouble y1, FdoDouble x2, FdoDouble y2 );
	static FdoDouble ComputeGeodeticTrapezoidArea2D(FdoDouble lat1, FdoDouble lon1, FdoDouble lat2, FdoDouble lon2, FdoDouble minLat );

};

#endif

