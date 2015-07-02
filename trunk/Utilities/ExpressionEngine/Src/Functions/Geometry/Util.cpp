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
#include <float.h>                      // for _isnan()
#include <math.h>
#include <cstring>

#define	EPS2               0.00000001
#define MArePositionsEqualXY(x1, y1, x2, y2) ( (fabs(x1-x2) < EPS2) && (fabs(y1-y2) < EPS2) )
#define MDistanceBetweenPositionsXY(x1, y1, x2, y2) sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2))
#ifndef M_PI
#define M_PI		3.14159265358979323846
#endif

// Special constant for representing a null ordinate in FDO geometries.
// In the future, this should change to numeric_limits<double>::quiet_NaN(),
// once AutoCAD Map is able to handle NaN's.
const double FdoExpressionEngineGeometryUtil::m_nullOrd = -1.25e126;

// Begin clone code from 
// trunk\Providers\SQLite\Src\SpatialIndex\SltGeomUtils.cpp

struct ArcSegmentDetails
{
    bool isCounterClockWise;
    bool isCircle;
    double center[2];
    double length;
    double radius;
    double thetaEndAngle;
    double thetaStartAngle;
};

bool GetCircularArcSegmentDetails(const double *startpoint, const double *midpoint, const double *endpoint, ArcSegmentDetails* details);
bool ComputeCenterFromThreePositions(const double *startpoint, const double *midpoint, const double *endpoint, double* center);

#define MATRIX(i, j) ( *( ptr_matrix + ( (i)*dim + (j) ) ) )
#define VERY_SMALL (1.0e-17)
double SnapToZero(double n)
{
    return (fabs(n) <= VERY_SMALL)? 0.0 : n;
}

int LUDecompose(int n, double a[], int eindex[])
{
    /* 
     * Decompose a into LU, where L is lower triangular matrix and U is upper
     * triangular matrix. We also have L[i][i] = 1 for i=1,2,...
     */

    int dim = 0;
    double *ptr_matrix = NULL;
    int i, j, k, col_max;
    int sign = 1;   /* det(matrix) sign change as a result of row exchange */
    double largest; /* the pivot element */
    double tmp, *scale = NULL;

    /* initialize static vars */

    if ( n <= 0 || a == NULL || eindex == NULL )
        return 0;

    dim = n;
    ptr_matrix = a;

    /* fill scale for each row */   
    scale = new double[n];
    if (NULL == scale)
        return 0;

    for ( i = 0; i < n; ++i ) {
        largest = 0;
        for ( j = 0; j < n; ++j ) {
            if ( ( tmp=fabs(MATRIX(i,j)) ) > largest )
                largest = tmp;
        }
        if ( SnapToZero(largest) == 0.0 ) {
            delete [] scale;
            return 0;
        }
        scale[i] = 1.0 / largest;
    }

    for ( j = 0; j < n; ++j ) {
        for ( i = 0; i <= j; ++i ) {
            for ( k = 0; k < i; ++k ) {
                MATRIX(i,j) -= MATRIX(i,k) * MATRIX(k,j);
            }
        }
        col_max = j;
        largest = scale[j]*fabs(MATRIX(j,j));
        for ( i = j + 1; i < n; ++i ) {
            for ( k = 0; k < j; ++k ) {
                MATRIX(i,j) -= MATRIX(i,k) * MATRIX(k,j);
            }
            if ( ( tmp = scale[i]*fabs(MATRIX(i,j)) ) > largest ) {
                col_max = i;
                largest = tmp;
            }
        }
        if ( j != col_max ) {
            /* interchange rows */
            for ( k=0; k<n; ++k ) {
                tmp = MATRIX(j,k);
                MATRIX(j,k) = MATRIX(col_max, k);
                MATRIX(col_max, k) = tmp;
            }
            /* interchange the scale factor */
            scale[col_max] = scale[j];
            sign = -sign;
        }
        eindex[j] = col_max;
        if ( SnapToZero(largest) == 0.0 ) {
            /* if pivot element is near zero, matrix is singular */
            delete [] scale;
            return 0;
        }
        /* devided by pivot element */
        if ( j < n-1 ) {
            tmp = 1.0 / MATRIX(j,j);
            for ( i = j+1; i < n; ++i ) {
                MATRIX(i,j) *= tmp;
            }
        }
    }
    delete [] scale;
    return sign;
}

/************************************************************************
*         Find the center point of the circular arc determined by three *
*         given points                                                  *
*                                                                       *
*   Denote V, V1, V2, V3 the vectors of center, start, mid, end         *
*       respectively,                                                   *
*   then V = (x,y,z) satisfies the following equtions:                  *
*   ( V - ( V1 + V2 ) / 2 ) * ( V2 - V1 ) = 0                           *
*   ( V - ( V1 + V3 ) / 2 ) * ( V3 - V1 ) = 0                           *
*   ( V - V1 ) * ( ( V2 - V1 ) X ( V3 - V1 ) ) = 0                      *
*                                                                       *
*   i.e.,                                                               *
*                                                                       *
*   x21 * x + y21 * y + z21 * z = ( ||V2||^2 - ||V1||^2 ) / 2           *
*   x31 * x + y31 * y + z31 * z = ( ||V3||^2 - ||V1||^2 ) / 2           *
*   Dyz * x + Dzx * y + Dxy * z = Dxyz                                  *
*                                                                       *
*   where                                                               *
*   ( x21, y21, z21 ) = V2 - V1,                                        *
*   ( x31, y31, z31 ) = V3 - V1,                                        *
*   Dyz = y21 * z31 - y31 * z21,                                        *
*   Dzx = z21 * x31 - z31 * x21,                                        *
*   Dxy = x21 * y31 - x31 * y21,                                        *
*                                                                       *
*   Dxyz = V1 * ( V2 X V3 )                                             *
*                                                                       *
*         | x1  y1  z1  |                                               *
*       = | x21 y21 z21 | = x1 * Dyz + y1 * Dzx + z1 * Dxy              *
*         | x31 y31 z31 |                                               *
*                                                                       *
*   ||Vi||^2 = xi*xi + yi*yi + zi*zi                                    *
*                                                                       *
************************************************************************/
// no support for 3D yet
bool ComputeCenterFromThreePositions(const double *startpoint, const double *midpoint, const double *endpoint, double* center)
{
    double a[9], b[3], tmp;
    int eindex[3];
    int i;

    a[0] = *(midpoint) - *(startpoint); /* x21 */
    a[1] = *(midpoint+1) - *(startpoint+1); /* y21 */
    a[2] = 0.0; /* z21 */

    a[3] = *(endpoint) - *(startpoint); /* x31 */
    a[4] = *(endpoint+1) - *(startpoint+1); /* y31 */
    a[5] = 0.0; /* z31 */

              /* y21 * z31 - y31 * z21 */
    a[6] = a[1] * a[5] - a[4] * a[2]; /* Dyz */
              /* z21 * x31 - z31 * x21 */
    a[7] = a[2] * a[3] - a[5] * a[0]; /* Dzx */
              /* x21 * y31 - x31 * y21 */
    a[8] = a[0] * a[4] - a[3] * a[1]; /* Dxy */

    tmp = *(startpoint) * *(startpoint) + *(startpoint+1) * *(startpoint+1);

    b[0] = *(midpoint) * *(midpoint) + *(midpoint+1) * *(midpoint+1);

    b[1] = *(endpoint) * *(endpoint) + *(endpoint+1) * *(endpoint+1);

    b[0] -= tmp;
    b[1] -= tmp;
    b[0] *= 0.5;
    b[1] *= 0.5;
    /* Dxyz = x1 * Dyz + y1 * Dzx + z1 * Dxy */
    b[2] = *(startpoint) * a[6] + *(startpoint+1) * a[7];

    /* Decompose a into LU */
    if ( LUDecompose( 3, a, eindex ) == 0 )
    {
        // Zero result means that points were collinear.
        return false;
    }

    /* Perform row exchanges for b */
    for ( i = 0; i < 3; ++i ) {
        if ( eindex[i] != i ) {
            tmp = b[i];
            b[i] = b[eindex[i]];
            b[eindex[i]] = tmp;
        }
    }

    /* Use L (UV) = b to solve UV */
    b[1] -= a[3] * b[0];
    b[2] -= a[6] * b[0] + a[7] * b[1];

    /* Use UV = b to solve V, the center */
    *(center+1) = b[1] / a[4];
    *(center) = (b[0] - a[1] * *(center+1)) / a[0];

    return true;
}

bool IsDirectionCounterClockWise(const double *startpoint, const double *midpoint, const double *endpoint)
{
    double xVect1 = *midpoint - *startpoint;
    double yVect1 = *(midpoint+1) - *(startpoint+1);
    
    double xVect2 = *endpoint - *midpoint;
    double yVect2 = *(endpoint+1) - *(midpoint+1);

    return (xVect1*yVect2 - yVect1*xVect2) >= 0.0;
}

bool GetCircularArcSegmentDetails(const double *startpoint, const double *midpoint, const double *endpoint, ArcSegmentDetails* details)
{
    details->isCircle = false;
    // Special cases
    if (MArePositionsEqualXY(*startpoint, *(startpoint+1), *(endpoint), *(endpoint+1)))
    {
        // This might be a supported circle case.
        if (MArePositionsEqualXY(*startpoint, *(startpoint+1), *midpoint, *(midpoint+1)))
        {
            details->length = 0.0; // Degenerated arc, all 3 points are coincident.
		    return false;
        }
	    else
        {
            details->radius = MDistanceBetweenPositionsXY(*startpoint, *(startpoint+1), *midpoint, *(midpoint+1)) / 2.0;
            details->length = M_PI * details->radius;
            details->isCircle = true;
            details->center[0] = (*midpoint+*startpoint)*0.5;
            details->center[1] = (*(midpoint+1)+*(startpoint+1))*0.5;
		    return true; // Full circle.
        }
    }

    double* center = &details->center[0]; // 2D only
    if(ComputeCenterFromThreePositions(startpoint, midpoint, endpoint, &center[0]))
    {
        details->radius = MDistanceBetweenPositionsXY(*startpoint, *(startpoint+1), *center, *(center+1));
        details->thetaStartAngle = atan2(*(startpoint+1) - *(center+1), *startpoint - *center);
        if (details->thetaStartAngle == -M_PI)
            details->thetaStartAngle = M_PI;
        details->thetaEndAngle = atan2(*(endpoint+1) - *(center+1), *(endpoint) - *center);
        if (details->thetaEndAngle == -M_PI)
            details->thetaEndAngle = M_PI;

        details->isCounterClockWise = IsDirectionCounterClockWise(startpoint, midpoint, endpoint);

        // Make sure that angles' magnitudes reflect direction.
        if ( details->isCounterClockWise && details->thetaStartAngle > details->thetaEndAngle )
            details->thetaEndAngle += 2.0 * M_PI;
        else if ( !details->isCounterClockWise && details->thetaStartAngle < details->thetaEndAngle )
            details->thetaEndAngle -= 2.0 * M_PI;

        details->length = details->radius * fabs(details->thetaEndAngle - details->thetaStartAngle);
        return true;
    }
    details->length = 0.0;
    return false;
}

double ComputeArcSegmentArea(const double *startpoint, const double *midpoint, const double *endpoint)
{
    ArcSegmentDetails details;
    if(!GetCircularArcSegmentDetails(startpoint, midpoint, endpoint, &details))
        return 0.0;

    double radius = details.radius;
    double xCenterToStart = *startpoint - details.center[0];
    double yCenterToStart = *(startpoint+1) - details.center[1];
    
    double xCenterToEnd = *(endpoint) - details.center[0];
    double yCenterToEnd = *(endpoint+1) - details.center[1];
    
    double crossProduct = xCenterToStart * yCenterToEnd - xCenterToEnd * yCenterToStart;
    double dotProduct = xCenterToStart * xCenterToEnd + yCenterToStart * yCenterToEnd;   
    double radiusSquared = radius * radius;
    double alpha;

    if (fabs(crossProduct) < fabs(dotProduct))
    {
        // use cross product to get angle value (more accurate in this situation)
        // use sign of cross and dot product to decide which quadrant
        // alpha should not be NaN because we select the 'better' function
        alpha = asin(crossProduct / radiusSquared);
        if (!details.isCounterClockWise)
        {
            // alpha must be negative at end; also, alpha is the wrong part of the circle right now
            if (dotProduct <= 0)
                alpha = -M_PI - alpha;
            else if (alpha > 0)
                alpha -= 2.0*M_PI;
        }
        else
        {
            // alpha must be positive at end
            if (dotProduct <= 0)
                alpha = M_PI - alpha;
            else if (alpha < 0)
                alpha += 2.0*M_PI;
        }
    }
    else
    {
        // use dot product to get angle value (more accurate in this situation)
        // use sign of cross product to decide which half
        // alpha should not be NaN because we select the 'better' function
        alpha = acos(dotProduct / (radius * radius));
        if (!details.isCounterClockWise)
        {
            // alpha must be negative at end; also, alpha is the wrong part of the circle right now
            if (crossProduct >= 0)
                alpha -= 2.0*M_PI;
            else
                alpha = -alpha;
        }
        else if (crossProduct < 0) // alpha must be positive at end
            alpha = 2.0*M_PI - alpha;
    }

    // calculate it, using the cross product value for the area of the triangle
    double area = (alpha * radiusSquared - crossProduct)*0.5;
    return area;
}

// End clone code from 
// trunk\Providers\SQLite\Src\SpatialIndex\SltGeomUtils.cpp

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
            // TODO: at least for non-geodetic arc do exact calculations (unfortunately FdoSpatialUtilityCircularArc
            // is not exported).
		    FdoPtr<FdoCurveSegmentCollection>	segs = FdoCurveSegmentCollection::Create();
            segs->Add(curveSeg);

   		    FdoPtr<FdoFgfGeometryFactory>	gf = FdoFgfGeometryFactory::GetInstance();
            FdoPtr<FdoIGeometry> geom = gf->CreateCurveString(segs);

			if ( computeGeodetic )
			{
				FdoPtr<FdoIGeometry> curve = FdoSpatialUtility::TesselateCurve(geom);
				FdoExpressionEngineGeometryUtil::ComputeGeometryLength( computeGeodetic, compute3D, curve, length );
			}
			else
			{
				FdoICircularArcSegment* arcSeg = (FdoICircularArcSegment *)curveSeg;
				FdoExpressionEngineGeometryUtil::ComputeArcSegmentLength( computeGeodetic, compute3D, arcSeg, length );			
			}
			break;
		}

	case FdoGeometryComponentType_LineStringSegment:
		{
            FdoILineStringSegment * lss = (FdoILineStringSegment*)curveSeg;
        	FdoInt32 dimensionality = lss->GetDimensionality();
			FdoInt32 numPositions = lss->GetCount();
            FdoInt32 numOrdsPerPos = FdoExpressionEngineGeometryUtil::DimensionalityToNumOrdinates(dimensionality);
            FdoInt32 numOrds = numPositions * numOrdsPerPos;
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

    // Adjust the value to [0, 1] interval.
    if ( d > 1 )
        d = 1;

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
    FdoICurvePolygon * cpoly;
    FdoIMultiCurvePolygon * mcpoly;
    FdoIMultiGeometry * mgeom;
    FdoInt32 i;

    FdoGeometryType geometryType = geometry->GetDerivedType();

    switch (geometryType)
    {
	case FdoGeometryType_LineString:
	case FdoGeometryType_MultiLineString:
	case FdoGeometryType_CurveString:
	case FdoGeometryType_MultiCurveString:
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
            if (computeGeodetic)
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
				    FdoInt32 numOrds = numPositions * numOrdsPerPos;
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
					    FdoInt32 numOrds = numPositions * numOrdsPerPos;
					    const double * ordinates = ls->GetOrdinates();
					    *area += FdoExpressionEngineGeometryUtil::ComputeArea(computeGeodetic, compute3D, numOrdsPerPos, numOrds, ordinates);
				    }
			    }
			    else
			    {
				    // Must be a polygon or multipolygon
				    FdoExpressionEngineGeometryUtil::ComputeGeometryArea( computeGeodetic, compute3D, geometry, area );
			    }
            }
            else
            {
                FdoICircularArcSegment* arc = static_cast<FdoICircularArcSegment *>(curveSeg);
        		FdoInt32 dimensionality = arc->GetDimensionality();
				FdoInt32 numPositions = 2; // start & end pos
				FdoInt32 numOrdsPerPos = FdoExpressionEngineGeometryUtil::DimensionalityToNumOrdinates(dimensionality);
				FdoInt32 numOrds = numPositions * numOrdsPerPos;
                
                double ordinates[8]; // put max points possible
                FdoPtr<FdoIDirectPosition> midPos = arc->GetMidPoint();
                FdoPtr<FdoIDirectPosition> startPos = arc->GetStartPosition();
                memcpy(ordinates, startPos->GetOrdinates(), sizeof(double)*numOrdsPerPos);
                FdoPtr<FdoIDirectPosition> endPos = arc->GetEndPosition();
                memcpy(ordinates+numOrdsPerPos, endPos->GetOrdinates(), sizeof(double)*numOrdsPerPos);
                // first calculate area using a line between start and end pos
                *area += FdoExpressionEngineGeometryUtil::ComputeArea(computeGeodetic, compute3D, numOrdsPerPos, numOrds, ordinates);
                // now adjust the area depending of the arc
                *area -= (ComputeArcSegmentArea(startPos->GetOrdinates(), midPos->GetOrdinates(), endPos->GetOrdinates())*2.0);
            }
			break;
		}

	case FdoGeometryComponentType_LineStringSegment:
		{
            FdoILineStringSegment * lss = (FdoILineStringSegment*)curveSeg;
        	FdoInt32 dimensionality = lss->GetDimensionality();
			FdoInt32 numPositions = lss->GetCount();
            FdoInt32 numOrdsPerPos = FdoExpressionEngineGeometryUtil::DimensionalityToNumOrdinates(dimensionality);
            FdoInt32 numOrds = numPositions * numOrdsPerPos;
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
	FdoDouble minDist = (dist1 < dist2) ? (dist1) : (dist2);
 
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

/************************************************************************/
/*  ComputeArcSegmentLength                                             */
/************************************************************************/

void FdoExpressionEngineGeometryUtil::ComputeArcSegmentLength(FdoBoolean computeGeodetic, FdoBoolean compute3D, FdoICircularArcSegment* arcSeg, FdoDouble *length)
{    
    FdoPtr<FdoIDirectPosition> start = arcSeg->GetStartPosition();
    FdoPtr<FdoIDirectPosition> mid = arcSeg->GetMidPoint();
    FdoPtr<FdoIDirectPosition> end = arcSeg->GetEndPosition();

	// Special cases
    if ( ArePositionsEqualXY(start, end) )
    {
        // This might be a supported circle case.
        if ( ArePositionsEqualXY(start, mid) )
		{
			// Degenerated arc, all 3 points are coincident.
			return;
		}
		else
		{
			// Full circle. 
			double diameter = DistanceBetweenPositionsXY(start, mid);

			*length += diameter * M_PI;
			return;
		}
	}

	// We need 2 equal consecutive segments. In case the mid point is really in
	// the middle of the arc then we save the tesselation.
	double len1 = DistanceBetweenPositionsXY(start, mid);
	double len2 = DistanceBetweenPositionsXY(end, mid);

	// The length of an arc segment. 
	double	segLen = len1;

	// Number of segments approximating the arc.
	int		nSegs = 2;

	if ( fabs(len1 - len2) > EPS2 )
	{
		// The midpoint is not in the middle. Tesselate.
	    FdoPtr<FdoFgfGeometryFactory>	gf = FdoFgfGeometryFactory::GetInstance();
		FdoPtr<FdoCurveSegmentCollection>	segs = FdoCurveSegmentCollection::Create();
        segs->Add((FdoICurveSegmentAbstract *) arcSeg);
        FdoPtr<FdoIGeometry> geom = gf->CreateCurveString(segs);

		FdoPtr<FdoIGeometry> curve = FdoSpatialUtility::TesselateCurve(geom);
		FdoILineString	*line = (FdoILineString	*)(curve.p);

		// Assume the points are not collinear and adjancent segments are equal
		start = line->GetItem(0);
		mid = line->GetItem(1);

		// Compute the length of the arc segment (SM in the picture below)
		segLen = DistanceBetweenPositionsXY(start, mid);
		nSegs = line->GetCount() - 1;
	}

	/*************************************************************************
						   M
						   *
					   T   |        
				   S*------*------*E
					 \     | K   /
					  \    |    /
					   \   |   /
						\  |  /
						 \ | /
						  \|/
						   C 
    Legend: S, M, E are 3 points on the circle, at equal distance. 
            T is middle of SM segment, C the center of the circle.
            CM is the radius. CT is perpendicular on SM.
    ***************************************************************************/

	// Compute the distance between start and end points (the SE segment)
	double Dist2 = DistanceBetweenPositionsXY(start, end)/2;

	// Compute the distance between the mid point and the SE segment (the MK segment)
	double d = sqrt( segLen*segLen - Dist2*Dist2);

	// There are 2 similar triangles sharing the same angle SMK (alpha). 
	// In picture above the median of segment SM (in point T) intersects the center,
	// where CT is perpendicular on SM.
	// 
	// Triangles SMK and CTM are similar, therefore:
	//			segLen/2 / R = d /segLen

	// To compute the arc length we need the radius and the angle.
	double radius = (segLen * segLen)/(2 * d );

	// The angle MSK
	double alpha = atan2(d, Dist2);

	// Angle MSK = TCM = alpha. The angle at center encompassing one segment is 2*alpha.
	*length += radius * (nSegs * (2 * alpha));
}

double FdoExpressionEngineGeometryUtil::DistanceBetweenPositionsXY(
    FdoIDirectPosition * p1,
    FdoIDirectPosition * p2)
{
    double dx = p1->GetX() - p2->GetX();
    double dy = p1->GetY() - p2->GetY();

    return ( sqrt(dx*dx + dy*dy) );
}

bool FdoExpressionEngineGeometryUtil::ArePositionsEqualXY(
    FdoIDirectPosition * p1,
    FdoIDirectPosition * p2)
{
    double dx = p1->GetX() - p2->GetX();
    double dy = p1->GetY() - p2->GetY();

    return	( (fabs(dx) < EPS2) && (fabs(dy) < EPS2) );
}

bool FdoExpressionEngineGeometryUtil::IsOrdinateNull(const double ordinate)
{
    return 
#ifdef _WIN32
        _isnan(ordinate)
#else
	    isnan(ordinate)
#endif
        || (m_nullOrd == ordinate);
}

