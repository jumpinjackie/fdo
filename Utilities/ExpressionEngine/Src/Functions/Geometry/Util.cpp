/*
* 
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
* 
*/

#include <stdafx.h>
#include <Functions/Geometry/Util.h>
#include <Spatial/SpatialStd.h>
#include <Spatial/SpatialUtility.h>		// for TesselateCurve()
#include <math.h>

/************************************************************************/
/* ComputeGeometryLength												*/
/************************************************************************/
void FdoExpressionEngineGeometryUtil::ComputeGeometryLength(FdoBoolean computeGeodetic, FdoBoolean compute3D, FdoIGeometry* geometry, FdoDouble *length)
{
    FdoInt32 dimensionality = 0;
    FdoInt32 numPositions = 0;
    FdoInt32 numOrdsPerPos = 0;
    FdoInt32 numOrds = 0;
    const double * ordinates = NULL;
    FdoInt32 numRings = 0;
    FdoInt32 numSubGeometries = 0;
    FdoInt32 numCurveSegments = 0;
    FdoPtr<FdoIRing> ring;
    FdoPtr<FdoILinearRing> lRing;
    FdoPtr<FdoIDirectPosition> pos;
    FdoILineString * ls;
    FdoIPolygon * poly;
    FdoIMultiLineString * mls;
    FdoIMultiPolygon * mpoly;
    FdoICurveString * cs;
    FdoIMultiCurveString * mcs;
    FdoICurvePolygon * cpoly;
    FdoIMultiCurvePolygon * mcpoly;
    FdoIMultiGeometry * mgeom;
    FdoInt32 i;

    FdoGeometryType geometryType = geometry->GetDerivedType();

    switch (geometryType)
    {
	case FdoGeometryType_LineString:
        ls = static_cast<FdoILineString *>(geometry);
	    dimensionality = ls->GetDimensionality();
	    numPositions = ls->GetCount();
        numOrdsPerPos = FdoExpressionEngineGeometryUtil::DimensionalityToNumOrdinates(dimensionality);
        numOrds = numPositions * numOrdsPerPos;
        ordinates = ls->GetOrdinates();
		*length += FdoExpressionEngineGeometryUtil::ComputeLength(computeGeodetic, compute3D, numOrdsPerPos, numOrds, ordinates);
		break;

	case FdoGeometryType_Point:
	case FdoGeometryType_MultiPoint:
		// Do nothing
		break;

	case FdoGeometryType_Polygon:
        poly = static_cast<FdoIPolygon *>(geometry);
	    dimensionality = poly->GetDimensionality();
	    numRings = poly->GetInteriorRingCount();
	    lRing = poly->GetExteriorRing();
	    FdoExpressionEngineGeometryUtil::ComputeLinearRingLength(computeGeodetic, compute3D, lRing, length);
	    for (i=0; i<numRings; i++)
	    {
		    lRing = poly->GetInteriorRing(i);
		    FdoExpressionEngineGeometryUtil::ComputeLinearRingLength(computeGeodetic, compute3D, lRing, length);
	    }
		break;

	case FdoGeometryType_MultiLineString:
        mls = static_cast<FdoIMultiLineString *>(geometry);
	    numSubGeometries = mls->GetCount();
	    for (i=0; i<numSubGeometries; i++)
        {
            FdoPtr<FdoILineString> lineString = mls->GetItem(i);
            FdoExpressionEngineGeometryUtil::ComputeGeometryLength(computeGeodetic, compute3D, lineString, length);
        }
		break;

	case FdoGeometryType_MultiPolygon:
        mpoly = static_cast<FdoIMultiPolygon *>(geometry);
	    numSubGeometries = mpoly->GetCount(); 
	    for (i=0; i<numSubGeometries; i++)
        {
            FdoPtr<FdoIPolygon> polygon = mpoly->GetItem(i);
            FdoExpressionEngineGeometryUtil::ComputeGeometryLength(computeGeodetic, compute3D, polygon, length);
        }
		break;

	case FdoGeometryType_CurveString:
        cs = static_cast<FdoICurveString *>(geometry);
	    dimensionality = cs->GetDimensionality();
	    pos = cs->GetStartPosition();
	    numCurveSegments = cs->GetCount();
	    for (i=0; i<numCurveSegments; i++)
	    {
		    FdoPtr<FdoICurveSegmentAbstract> curveSeg = cs->GetItem(i);
		    FdoExpressionEngineGeometryUtil::ComputeCurveSegmentLength(computeGeodetic, compute3D, curveSeg, length);
	    }
		break;

	case FdoGeometryType_MultiCurveString:
        mcs = static_cast<FdoIMultiCurveString *>(geometry);
	    numSubGeometries = mcs->GetCount();
	    for (i=0; i<numSubGeometries; i++)
	    {
		    FdoPtr<FdoICurveString> curveString = mcs->GetItem(i);
            FdoExpressionEngineGeometryUtil::ComputeGeometryLength(computeGeodetic, compute3D, curveString, length);
	    }
		break;

	case FdoGeometryType_CurvePolygon:
        cpoly = static_cast<FdoICurvePolygon *>(geometry);
	    dimensionality = cpoly->GetDimensionality();
	    numRings = cpoly->GetInteriorRingCount();
	    ring = cpoly->GetExteriorRing();
	    FdoExpressionEngineGeometryUtil::ComputeRingLength(computeGeodetic, compute3D, ring, length);

	    // Interior rings
	    for (i=0; i<numRings; i++)
	    {
		    ring = cpoly->GetInteriorRing(i);
		    FdoExpressionEngineGeometryUtil::ComputeRingLength(computeGeodetic, compute3D, ring, length);
	    }
		break;

	case FdoGeometryType_MultiCurvePolygon:
        mcpoly = static_cast<FdoIMultiCurvePolygon *>(geometry);
	    numSubGeometries = mcpoly->GetCount();
	    for (i=0; i<numSubGeometries; i++)
	    {
		    FdoPtr<FdoICurvePolygon> curvePolygon = mcpoly->GetItem(i);
            FdoExpressionEngineGeometryUtil::ComputeGeometryLength(computeGeodetic, compute3D, curvePolygon, length);
	    }
		break;

	case FdoGeometryType_MultiGeometry:
        mgeom = static_cast<FdoIMultiGeometry *>(geometry);
	    numSubGeometries = mgeom->GetCount(); 
	    for (i=0; i<numSubGeometries; i++)
	    {
            FdoPtr<FdoIGeometry> geom = mgeom->GetItem(i);
            FdoExpressionEngineGeometryUtil::ComputeGeometryLength(computeGeodetic, compute3D, geom, length);
	    }
		break;

    default:
    	throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_UNKNOWN_GEOMETRY_TYPE), 
                                                               L"FdoExpressionEngineGeometryUtil::ComputeGeometryLength",
                                                               geometryType));
    }
}

/************************************************************************/
/* ComputeLinearRingLength                                              */
/************************************************************************/
void FdoExpressionEngineGeometryUtil::ComputeLinearRingLength(FdoBoolean computeGeodetic, FdoBoolean compute3D, FdoILinearRing* lineRing, FdoDouble *length)
{
	// Write the number of positions in this ring
	FdoInt32 numPositions = lineRing->GetCount();
	FdoInt32 dimensionality = lineRing->GetDimensionality();
    FdoInt32 numOrdsPerPos = FdoExpressionEngineGeometryUtil::DimensionalityToNumOrdinates(dimensionality);
    FdoInt32 numOrds = numPositions * numOrdsPerPos;
    const double * ordinates = lineRing->GetOrdinates();

	*length += FdoExpressionEngineGeometryUtil::ComputeLength(computeGeodetic, compute3D, numOrdsPerPos, numOrds, ordinates);
}


/************************************************************************/
/* ComputeRingLength                                                    */
/************************************************************************/
void FdoExpressionEngineGeometryUtil::ComputeRingLength(FdoBoolean computeGeodetic, FdoBoolean compute3D, FdoIRing* ring, FdoDouble *length)
{
	if (NULL == ring) 
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_INVALID_INPUT_ON_CLASS_FUNCTION),
                                                               L"FdoExpressionEngineGeometryUtil::ComputeRingLength",
                                                               L"ring"));

    // NumElements (segments)
	FdoInt32 numCurveSegs = ring->GetCount();

	// CurveSegs
	for (FdoInt32 i=0; i<numCurveSegs; i++)
	{
		FdoPtr<FdoICurveSegmentAbstract> curveSeg = ring->GetItem(i);
		FdoExpressionEngineGeometryUtil::ComputeCurveSegmentLength(computeGeodetic, compute3D, curveSeg, length);
	} 
}


/************************************************************************/
/* ComputeCurveSegmentLength                                            */
/************************************************************************/
void FdoExpressionEngineGeometryUtil::ComputeCurveSegmentLength(FdoBoolean computeGeodetic, FdoBoolean compute3D, FdoICurveSegmentAbstract* curveSeg, FdoDouble *length)
{
	if ( NULL == curveSeg )
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_INVALID_INPUT_ON_CLASS_FUNCTION),
                                                                   L"FdoExpressionEngineGeometryUtil::ComputeCurveSegmentLength",
                                                                   L"curveSeg"));

	FdoGeometryComponentType geomType = curveSeg->GetDerivedType();

	switch (geomType)
	{
	case FdoGeometryComponentType_CircularArcSegment:
		{			
			// Needs tesselation. Create a geometry out of this curve.
			FdoPtr<FdoCurveSegmentCollection>	segs = FdoCurveSegmentCollection::Create();
            segs->Add(curveSeg);

			FdoPtr<FdoFgfGeometryFactory>	gf = FdoFgfGeometryFactory::GetInstance();
            FdoPtr<FdoIGeometry> geom = gf->CreateCurveString(segs);

			FdoPtr<FdoIGeometry> curve = FdoSpatialUtility::TesselateCurve(geom);
			FdoExpressionEngineGeometryUtil::ComputeGeometryLength( computeGeodetic, compute3D, curve, length );

			break;
		}

	case FdoGeometryComponentType_LineStringSegment:
		{
            FdoILineStringSegment * lss = (FdoILineStringSegment*)curveSeg;
        	FdoInt32 dimensionality = lss->GetDimensionality();
			FdoInt32 numPositions = lss->GetCount();
            FdoInt32 numOrdsPerPos = FdoExpressionEngineGeometryUtil::DimensionalityToNumOrdinates(dimensionality);
            FdoInt32 numOrds = (numPositions-1) * numOrdsPerPos;
            const double * ordinates = lss->GetOrdinates();
            *length += FdoExpressionEngineGeometryUtil::ComputeLength(computeGeodetic, compute3D, numOrdsPerPos, numOrds, ordinates);

			break;
		}
	default:
		{
    		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_UNKNOWN_GEOMETRY_COMPONENT_TYPE),
                                                                   L"FdoExpressionEngineGeometryUtil::::WriteCurveSegment",
                                                                   geomType));
		}
	} // of switch
}

/************************************************************************/
/* ComputeLength                                                         */
/************************************************************************/
FdoDouble FdoExpressionEngineGeometryUtil::ComputeLength(FdoBoolean computeGeodetic, FdoBoolean compute3D, FdoInt32 numOrdsPerPos, FdoInt32 numOrds, const double *ordinates)
{
	if ( compute3D )
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_3_NOTIMPLEMENTED)));
	else
		return FdoExpressionEngineGeometryUtil::ComputeLength2D(computeGeodetic, numOrdsPerPos, numOrds, ordinates);
}

/************************************************************************/
/* ComputeLength2D                                                      */
/************************************************************************/
FdoDouble FdoExpressionEngineGeometryUtil::ComputeLength2D(FdoBoolean computeGeodetic, FdoInt32 numOrdsPerPos, FdoInt32 numOrds, const double *ordinates)
{
	FdoDouble	length = 0.0;
	double		x1, y1, x2, y2;

	for ( FdoInt32 i = 0; i < numOrds - numOrdsPerPos;  )
	{
		x1 = ordinates[i];
		y1 = ordinates[i + 1];
		i += numOrdsPerPos;  // advance

		x2 = ordinates[i];
		y2 = ordinates[i + 1];

		if ( computeGeodetic )
			length += FdoExpressionEngineGeometryUtil::ComputeGeodeticDistance2D( y1, x1, y2, x2);
		else
			length += FdoExpressionEngineGeometryUtil::ComputeEuclidianDistance2D( x1, y1, x2, y2);
	}

	return length;
}

/************************************************************************/
/* ComputeEuclidianDistance                                             */
/************************************************************************/
FdoDouble FdoExpressionEngineGeometryUtil::ComputeEuclidianDistance2D(FdoDouble x1, FdoDouble y1, FdoDouble x2, FdoDouble y2 )
{
	FdoDouble	dx = x1 - x2;
	FdoDouble	dy = y1 - y2;
	return sqrt( dx*dx + dy*dy );
}

/************************************************************************/
/*	ComputeGeodeticDistance												*/
/*																		*/
/* Credits: the code has been ported from http://www.codeguru.com/Cpp/Cpp/algorithms/article.php/c5115/
/* It includes an optimization for small angles:
/*		One (1st edition) reader provided an alternate distance formula that is better for small angles. 
/*
/*		dlon = lon2 - lon1
/*		dlat = lat2 - lat1
/*		a = (sin(dlat/2))^2 + cos(lat1) * cos(lat2) * (sin(dlon/2))^2
/*		c = 2 * arcsin(min(1,sqrt(a)))
/*		distance = (Earth Radius) * c
/* 
/*		
/* The algorithm coincides with PROJ4 implementation for geodetic distance.
/************************************************************************/
#define DE2RA             0.01745329252
#define	RA2DE             57.2957795129
#define	ERADM             6378135.0			
#define	AVG_ERAD          6371000.0			// Average radius
#define	FLATTENING        1.0/298.257223563 // Earth flattening (WGS '84)
#define	EPS               0.000000000005

#define	GEODETIC_DIST_OPTIMIZATION	false	// TODO: doesn't work yet

FdoDouble FdoExpressionEngineGeometryUtil::ComputeGeodeticDistance2D(FdoDouble lat1, FdoDouble lon1, FdoDouble lat2, FdoDouble lon2 )
{
	FdoDouble	lat1r		= lat1 * DE2RA;
	FdoDouble	lon1r		= lon1 * DE2RA;
	FdoDouble	lat2r		= lat2 * DE2RA;
	FdoDouble	lon2r		= lon2 * DE2RA;
	FdoDouble	deltaLat	= lat1r - lat2r; 
	FdoDouble	deltaLon	= lon1r - lon2r; 

	FdoDouble	sin1		= sin(lat1r);
	FdoDouble	sin2		= sin(lat2r);
	FdoDouble	cos1		= cos(lat1r);
	FdoDouble	cos2		= cos(lat2r);

	FdoDouble	d = 0.0;

	if ( !GEODETIC_DIST_OPTIMIZATION )
	{
		d = sin1*sin2 + cos1*cos2*cos(deltaLon);
	}
	else if ( fabs(deltaLat) > 1. || fabs(deltaLon) > 1. )
	{
		d = sin1*sin2 + cos1*cos2*cos(deltaLon);
	}
	else
	{
		// alternate distance formula that is better for small angles
		FdoDouble	deltaLat	= lat2r - lat1r; 
		FdoDouble	deltaLon	= lon2r - lon1r; 

		FdoDouble	sinDlat = sin(deltaLat/2);
		FdoDouble	sinDlon = sin(deltaLon/2);

		FdoDouble a = sinDlat*sinDlat + cos1*cos2*sinDlon*sinDlon;
        FdoDouble aSqrt =  sqrt(a);
        FdoDouble minValue = (1 < aSqrt) ? 1 : aSqrt; 
		d = 2 * asin(minValue);
	}

	return (ERADM * acos(d));
}

/************************************************************************/
/* ComputeGeometryArea													*/
/************************************************************************/
void FdoExpressionEngineGeometryUtil::ComputeGeometryArea(FdoBoolean computeGeodetic, FdoBoolean compute3D, FdoIGeometry* geometry, FdoDouble *area)
{
    FdoInt32 dimensionality = 0;
    FdoInt32 numPositions = 0;
    FdoInt32 numOrdsPerPos = 0;
    FdoInt32 numOrds = 0;
    const double * ordinates = NULL;
    FdoInt32 numRings = 0;
    FdoInt32 numSubGeometries = 0;
    FdoInt32 numCurveSegments = 0;
    FdoPtr<FdoIRing> ring;
    FdoPtr<FdoILinearRing> lRing;
    FdoPtr<FdoIDirectPosition> pos;
    FdoIPolygon * poly;
    FdoIMultiPolygon * mpoly;
    FdoICurveString * cs;
    FdoIMultiCurveString * mcs;
    FdoICurvePolygon * cpoly;
    FdoIMultiCurvePolygon * mcpoly;
    FdoIMultiGeometry * mgeom;
    FdoInt32 i;

    FdoGeometryType geometryType = geometry->GetDerivedType();

    switch (geometryType)
    {
	case FdoGeometryType_LineString:
	case FdoGeometryType_MultiLineString:
	case FdoGeometryType_Point:
	case FdoGeometryType_MultiPoint:
		// Do nothing
		break;

	case FdoGeometryType_Polygon:
        poly = static_cast<FdoIPolygon *>(geometry);
	    dimensionality = poly->GetDimensionality();
	    numRings = poly->GetInteriorRingCount();
	    lRing = poly->GetExteriorRing();
	    FdoExpressionEngineGeometryUtil::ComputeLinearRingArea(computeGeodetic, compute3D, true, lRing, area);
	    for (i=0; i<numRings; i++)
	    {
		    lRing = poly->GetInteriorRing(i);
		    FdoExpressionEngineGeometryUtil::ComputeLinearRingArea(computeGeodetic, compute3D, false, lRing, area);
	    }
		break;

	case FdoGeometryType_MultiPolygon:
        mpoly = static_cast<FdoIMultiPolygon *>(geometry);
	    numSubGeometries = mpoly->GetCount(); 
	    for (i=0; i<numSubGeometries; i++)
        {
            FdoPtr<FdoIPolygon> polygon = mpoly->GetItem(i);
            FdoExpressionEngineGeometryUtil::ComputeGeometryArea(computeGeodetic, compute3D, polygon, area);
        }
		break;

	case FdoGeometryType_CurveString:
        cs = static_cast<FdoICurveString *>(geometry);
	    dimensionality = cs->GetDimensionality();
	    pos = cs->GetStartPosition();
	    numCurveSegments = cs->GetCount();
	    for (i=0; i<numCurveSegments; i++)
	    {
		    FdoPtr<FdoICurveSegmentAbstract> curveSeg = cs->GetItem(i);
		    FdoExpressionEngineGeometryUtil::ComputeCurveSegmentArea(computeGeodetic, compute3D, curveSeg, area);
	    }
		break;

	case FdoGeometryType_MultiCurveString:
        mcs = static_cast<FdoIMultiCurveString *>(geometry);
	    numSubGeometries = mcs->GetCount();
	    for (i=0; i<numSubGeometries; i++)
	    {
		    FdoPtr<FdoICurveString> curveString = mcs->GetItem(i);
            FdoExpressionEngineGeometryUtil::ComputeGeometryLength(computeGeodetic, compute3D, curveString, area);
	    }
		break;

	case FdoGeometryType_CurvePolygon:
        cpoly = static_cast<FdoICurvePolygon *>(geometry);
	    dimensionality = cpoly->GetDimensionality();
	    numRings = cpoly->GetInteriorRingCount();
	    ring = cpoly->GetExteriorRing();
	    FdoExpressionEngineGeometryUtil::ComputeRingArea(computeGeodetic, compute3D, true, ring, area);

	    // Interior rings
	    for (i=0; i<numRings; i++)
	    {
		    ring = cpoly->GetInteriorRing(i);
		    FdoExpressionEngineGeometryUtil::ComputeRingArea(computeGeodetic, compute3D, false, ring, area);
	    }
		break;

	case FdoGeometryType_MultiCurvePolygon:
        mcpoly = static_cast<FdoIMultiCurvePolygon *>(geometry);
	    numSubGeometries = mcpoly->GetCount();
	    for (i=0; i<numSubGeometries; i++)
	    {
		    FdoPtr<FdoICurvePolygon> curvePolygon = mcpoly->GetItem(i);
            FdoExpressionEngineGeometryUtil::ComputeGeometryArea(computeGeodetic, compute3D, curvePolygon, area);
	    }
		break;

	case FdoGeometryType_MultiGeometry:
        mgeom = static_cast<FdoIMultiGeometry *>(geometry);
	    numSubGeometries = mgeom->GetCount(); 
	    for (i=0; i<numSubGeometries; i++)
	    {
            FdoPtr<FdoIGeometry> geom = mgeom->GetItem(i);
            FdoExpressionEngineGeometryUtil::ComputeGeometryArea(computeGeodetic, compute3D, geom, area);
	    }
		break;

    default:
    	throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_UNKNOWN_GEOMETRY_TYPE), 
                                                               L"FdoExpressionEngineGeometryUtil::ComputeGeometryArea",
                                                               geometryType));
    }
}

/************************************************************************/
/* ComputeLinearRingArea                                                */
/************************************************************************/
void FdoExpressionEngineGeometryUtil::ComputeLinearRingArea(FdoBoolean computeGeodetic, FdoBoolean compute3D, FdoBoolean isExterior, FdoILinearRing* lineRing, FdoDouble *area)
{
	// Write the number of positions in this ring
	FdoInt32 numPositions = lineRing->GetCount();
	FdoInt32 dimensionality = lineRing->GetDimensionality();
    FdoInt32 numOrdsPerPos = FdoExpressionEngineGeometryUtil::DimensionalityToNumOrdinates(dimensionality);
    FdoInt32 numOrds = numPositions * numOrdsPerPos;
    const double * ordinates = lineRing->GetOrdinates();

	double area_ = 	FdoExpressionEngineGeometryUtil::ComputeArea(computeGeodetic, compute3D, numOrdsPerPos, numOrds, ordinates);

	area_ = fabs(area_)/2.0;
	area_ = isExterior ? area_ : - area_;
	*area += area_;
}


/************************************************************************/
/*	ComputeRingArea                                                     */
/************************************************************************/
void FdoExpressionEngineGeometryUtil::ComputeRingArea(FdoBoolean computeGeodetic, FdoBoolean compute3D, FdoBoolean isExterior, FdoIRing* ring, FdoDouble *area)
{
	if (NULL == ring) 
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_INVALID_INPUT_ON_CLASS_FUNCTION),
                                                               L"FdoExpressionEngineGeometryUtil::ComputeRingArea",
                                                               L"ring"));

    // NumElements (segments)
	FdoInt32 numCurveSegs = ring->GetCount();

	FdoDouble	area_ = 0.0;
	// CurveSegs
	for (FdoInt32 i=0; i<numCurveSegs; i++)
	{
		FdoPtr<FdoICurveSegmentAbstract> curveSeg = ring->GetItem(i);
		FdoExpressionEngineGeometryUtil::ComputeCurveSegmentArea(computeGeodetic, compute3D, curveSeg, &area_);
	} 
	
	area_ = fabs(area_)/2.0;
	area_ = isExterior ? area_ : - area_;
	*area += area_;
}

/************************************************************************/
/* ComputeCurveSegmentArea                                              */
/************************************************************************/
void FdoExpressionEngineGeometryUtil::ComputeCurveSegmentArea(FdoBoolean computeGeodetic, FdoBoolean compute3D, FdoICurveSegmentAbstract* curveSeg, FdoDouble *area)
{
	/*
	*  Note: This function **doesn't** write the first position
	*  of the segment being written. This is to make it compatible with
	*  memory layout of CurveString FGF.
	*/
	if ( NULL == curveSeg )
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_INVALID_INPUT_ON_CLASS_FUNCTION),
                                                                   L"FdoExpressionEngineGeometryUtil::ComputeCurveSegmentArea",
                                                                   L"curveSeg"));

	FdoGeometryComponentType geomType = curveSeg->GetDerivedType();

	switch (geomType)
	{
	case FdoGeometryComponentType_CircularArcSegment:
		{			
			// Needs tesselation. Create a geometry out of this curve.
			FdoPtr<FdoCurveSegmentCollection>	segs = FdoCurveSegmentCollection::Create();
            segs->Add(curveSeg);

			FdoPtr<FdoFgfGeometryFactory>	gf = FdoFgfGeometryFactory::GetInstance();
            FdoPtr<FdoIGeometry> geom = gf->CreateCurveString(segs);

			FdoPtr<FdoIGeometry> geometry = FdoSpatialUtility::TesselateCurve(geom);

			// If the geometry is a simple line string, process here to avoid recursion.
			if ( geometry->GetDerivedType() == FdoGeometryType_LineString )
			{
				FdoILineString * ls = static_cast<FdoILineString *>(geometry.p);
        		FdoInt32 dimensionality = ls->GetDimensionality();
				FdoInt32 numPositions = ls->GetCount();
				FdoInt32 numOrdsPerPos = FdoExpressionEngineGeometryUtil::DimensionalityToNumOrdinates(dimensionality);
				FdoInt32 numOrds = (numPositions-1) * numOrdsPerPos;
				const double * ordinates = ls->GetOrdinates();
				*area += FdoExpressionEngineGeometryUtil::ComputeArea(computeGeodetic, compute3D, numOrdsPerPos, numOrds, ordinates);
			}
			else if ( geometry->GetDerivedType() == FdoGeometryType_MultiLineString )
			{
			    FdoIMultiLineString * mls = static_cast<FdoIMultiLineString *>(geometry.p);
				for ( int i = 0; i < mls->GetCount(); i++ )
				{
					FdoPtr<FdoILineString>	ls = mls->GetItem(i);
    				FdoInt32 dimensionality = ls->GetDimensionality();
					FdoInt32 numPositions = ls->GetCount();
					FdoInt32 numOrdsPerPos = FdoExpressionEngineGeometryUtil::DimensionalityToNumOrdinates(dimensionality);
					FdoInt32 numOrds = (numPositions-1) * numOrdsPerPos;
					const double * ordinates = ls->GetOrdinates();
					*area += FdoExpressionEngineGeometryUtil::ComputeArea(computeGeodetic, compute3D, numOrdsPerPos, numOrds, ordinates);
				}
			}
			else
			{
				// Must be a polygon or multipolygon
				FdoExpressionEngineGeometryUtil::ComputeGeometryArea( computeGeodetic, compute3D, geometry, area );
			}
			break;
		}

	case FdoGeometryComponentType_LineStringSegment:
		{
            FdoILineStringSegment * lss = (FdoILineStringSegment*)curveSeg;
        	FdoInt32 dimensionality = lss->GetDimensionality();
			FdoInt32 numPositions = lss->GetCount();
            FdoInt32 numOrdsPerPos = FdoExpressionEngineGeometryUtil::DimensionalityToNumOrdinates(dimensionality);
            FdoInt32 numOrds = (numPositions-1) * numOrdsPerPos;
            const double * ordinates = lss->GetOrdinates();
            *area += FdoExpressionEngineGeometryUtil::ComputeArea(computeGeodetic, compute3D, numOrdsPerPos, numOrds, ordinates);

			break;
		}
	default:
		{
    		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_UNKNOWN_GEOMETRY_COMPONENT_TYPE),
                                                                   L"FdoExpressionEngineGeometryUtil::::ComputeCurveSegmentArea",
                                                                   geomType));
		}
	} 
}

/************************************************************************/
/* ComputeArea                                                          */
/************************************************************************/
FdoDouble FdoExpressionEngineGeometryUtil::ComputeArea(FdoBoolean computeGeodetic, FdoBoolean compute3D, FdoInt32 numOrdsPerPos, FdoInt32 numOrds, const double *ordinates)
{
	if ( compute3D )
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_3_NOTIMPLEMENTED)));
	else
		return FdoExpressionEngineGeometryUtil::ComputeArea2D(computeGeodetic, numOrdsPerPos, numOrds, ordinates);
}

/************************************************************************/
/* ComputeArea2D                                                        */
/************************************************************************/
FdoDouble FdoExpressionEngineGeometryUtil::ComputeArea2D(FdoBoolean computeGeodetic, FdoInt32 numOrdsPerPos, FdoInt32 numOrds, const double *ordinates)
{
	FdoDouble	area = 0.0;
	double		x1, y1, x2, y2;
	FdoDouble	minLat = 0.0;

	if ( computeGeodetic )
	{
		for ( FdoInt32 i = 0; i < numOrds - numOrdsPerPos; i += numOrdsPerPos )
		{
			FdoDouble lat = ordinates[i+1];

			if ( i == 0 )
				minLat = lat;
			else
				minLat = ( lat < minLat ) ? lat : minLat;
		}
	}

	for ( FdoInt32 i = 0; i < numOrds - numOrdsPerPos;  )
	{
		x1 = ordinates[i];
		y1 = ordinates[i + 1];
		i += numOrdsPerPos;  // advance

		x2 = ordinates[i];
		y2 = ordinates[i + 1];

		if ( computeGeodetic )
			area += FdoExpressionEngineGeometryUtil::ComputeGeodeticTrapezoidArea2D( y1, x1, y2, x2, minLat ); 
		else
			area += FdoExpressionEngineGeometryUtil::ComputeEuclidianTrapezoidArea2D( x1, y1, x2, y2);
	}

	return area;
}

/************************************************************************/
/* ComputeEuclidianTrapezoidArea2D                                      */
/************************************************************************/
FdoDouble FdoExpressionEngineGeometryUtil::ComputeEuclidianTrapezoidArea2D(FdoDouble x1, FdoDouble y1, FdoDouble x2, FdoDouble y2 )
{
	// Multiplied by 2
	return (x2 - x1) * (y2 + y1 );
}

/************************************************************************/
/* ComputeGeodeticTrapezoidArea2D										*/
/*																		*/
/* Use an appproximate formula: A = b*h + (B - b)*h/2					*/
/************************************************************************/
FdoDouble FdoExpressionEngineGeometryUtil::ComputeGeodeticTrapezoidArea2D(FdoDouble lat1, FdoDouble lon1, FdoDouble lat2, FdoDouble lon2, FdoDouble minLat )
{
	// Compute the distances to the Ecuator
	FdoDouble dist1 = FdoExpressionEngineGeometryUtil::ComputeGeodeticDistance2D( lat1, lon1, minLat, lon1 );
	FdoDouble dist2 = FdoExpressionEngineGeometryUtil::ComputeGeodeticDistance2D( lat2, lon2, minLat, lon2 );
	FdoDouble minDist = min(dist1, dist2);

	// Compute the distance between points at the same latitude. Take the average
	FdoDouble distLat1 = FdoExpressionEngineGeometryUtil::ComputeGeodeticDistance2D( lat1, lon1, lat1, lon2 );
	FdoDouble distLat2 = FdoExpressionEngineGeometryUtil::ComputeGeodeticDistance2D( lat2, lon1, lat2, lon2 );
	FdoDouble h = (distLat1 + distLat2)/2;

	// Multiplied by 2
	FdoDouble area = minDist * h * 2 + fabs(dist1 - dist2) * h;

	if ( lon2 < lon1 )
		area = -area;

	return area;
}

/************************************************************************/
/* DimensionalityToNumOrdinates                                         */
/************************************************************************/
FdoInt32 FdoExpressionEngineGeometryUtil::DimensionalityToNumOrdinates(FdoInt32 dimensionality)
{
    FdoInt32 value = 0;

    if ( dimensionality == FdoDimensionality_XY )
        value = 2;
    else if ( dimensionality == ( FdoDimensionality_XY | FdoDimensionality_Z ) )
        value = 3;
    else if ( dimensionality == ( FdoDimensionality_XY | FdoDimensionality_M ) )
        value = 3;
    else if ( dimensionality == ( FdoDimensionality_XY | FdoDimensionality_Z | FdoDimensionality_M ) )
        value = 4;
    else
		throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_INVALID_INPUT_ON_CLASS_FUNCTION),
                                                               L"FdoExpressionEngineGeometryUtil::DimensionalityToNumOrdinates",
                                                               L"dimensionality"));
    return value;
}
