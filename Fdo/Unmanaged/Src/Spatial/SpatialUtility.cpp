/*
 * 
* Copyright (C) 2004-2007  Autodesk, Inc.
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

#include "MathUtility.h"

#include <Spatial/SpatialStd.h>
#include <Spatial/SpatialUtility.h>
#include "SpatialUtilityCircularArc.h"
#include "SpatialUtilityGeometryExtents.h"

#include <Spatial/SpatialGeometryValidity.h>
#include <Fdo/Filter/SpatialCondition.h>

#include <vector>

///////////// Ported from SDFPlus provider //////////////////////////////////

//those numbers are very small and provide
//the tolerance needed for "exact" computations,
//i.e. CAD  style non-FDO geometry intersection.
#define EPSILON 1e-10
#define NEGEPSILON -EPSILON
#define SQREPSILON 1e-20

//relative fraction to be used for curve tesselation approximation
//This fraction is relative to the biggest dimension of the bounding box 
//of the curve.
#define CURVE_APPROX_FACTOR 1e-3

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

FdoIGeometry * FdoSpatialUtility::ApproximateGeometryWithLineStrings(
    FdoIGeometry * geometry, 
    double maxSpacing,
    double maxOffset,
    FdoGeometryFactoryAbstract * geometryFactory)
{
    FdoIGeometry * newGeometry = NULL;

    if ( geometry == NULL || maxSpacing < 0.0 || maxOffset < 0.0 )
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_INVALID_INPUT_ON_CLASS_FUNCTION),
                                                               L"FdoSpatialUtility::IsClosed",
                                                               L"geometry/maxSpacing/maxOffset"));

    FdoGeometryType geomType = geometry->GetDerivedType();

    switch (geomType)
    {
	case FdoGeometryType_LineString:
	case FdoGeometryType_Point:
	case FdoGeometryType_Polygon:
	case FdoGeometryType_MultiPoint:
	case FdoGeometryType_MultiLineString:
	case FdoGeometryType_MultiPolygon:
        // No approximation needed.
        newGeometry = geometry;
        FDO_SAFE_ADDREF(geometry);
        break;

	case FdoGeometryType_CurveString:
        {
            FdoICurveString * curve = static_cast<FdoICurveString *> (geometry);
            FdoPtr<FdoCurveSegmentCollection> csc = curve->GetCurveSegments();

            FdoPtr<FdoDirectPositionCollection> positions = FdoDirectPositionCollection::Create();

            AppendLinearizedCurveSegmentsToDistinctCollection(
                positions, csc, maxSpacing, maxOffset);

            newGeometry = geometryFactory->CreateLineString(positions);
        }
        break;

	case FdoGeometryType_CurvePolygon:
        {
            FdoICurvePolygon * geom = static_cast<FdoICurvePolygon *> (geometry);

            FdoPtr<FdoIRing> ring = geom->GetExteriorRing();
            FdoPtr<FdoCurveSegmentCollection> csc = ring->GetCurveSegments();
            FdoPtr<FdoDirectPositionCollection> positions = FdoDirectPositionCollection::Create();
            AppendLinearizedCurveSegmentsToDistinctCollection(
                positions, csc, maxSpacing, maxOffset);
            FdoPtr<FdoILinearRing> linearizedExteriorRing = geometryFactory->CreateLinearRing(positions);

            FdoPtr<FdoLinearRingCollection> linearizedInteriorRings = FdoLinearRingCollection::Create();

            for (FdoInt32 i = 0;  i < geom->GetInteriorRingCount();  i++)
            {
                ring = geom->GetInteriorRing(i);
                csc = ring->GetCurveSegments();
                positions->Clear();
                AppendLinearizedCurveSegmentsToDistinctCollection(
                    positions, csc, maxSpacing, maxOffset);
                FdoPtr<FdoILinearRing> linearizedInteriorRing = geometryFactory->CreateLinearRing(positions);
                linearizedInteriorRings->Add(linearizedInteriorRing);
            }

            newGeometry = geometryFactory->CreatePolygon(linearizedExteriorRing, linearizedInteriorRings);   
        }
        break;

	case FdoGeometryType_MultiGeometry:
        {
            FdoIMultiGeometry * geom = static_cast<FdoIMultiGeometry *> (geometry);

            FdoPtr<FdoGeometryCollection> linearizedGeometries = FdoGeometryCollection::Create();

            for (FdoInt32 i = 0;  i < geom->GetCount();  i++)
            {
                FdoPtr<FdoIGeometry> geomInside = geom->GetItem(i);
                FdoPtr<FdoIGeometry> linearizedGeomInside = ApproximateGeometryWithLineStrings(geomInside, maxSpacing, maxOffset, geometryFactory);
                linearizedGeometries->Add(linearizedGeomInside);
            }

            newGeometry = geometryFactory->CreateMultiGeometry(linearizedGeometries);   
        }
        break;

	case FdoGeometryType_MultiCurveString:
        {
            FdoIMultiCurveString * geom = static_cast<FdoIMultiCurveString *> (geometry);

            FdoPtr<FdoLineStringCollection> linearizedGeometries = FdoLineStringCollection::Create();

            for (FdoInt32 i = 0;  i < geom->GetCount();  i++)
            {
                FdoPtr<FdoICurveString> geomInside = geom->GetItem(i);
                FdoPtr<FdoIGeometry> linearizedGeomInside = ApproximateGeometryWithLineStrings(geomInside, maxSpacing, maxOffset, geometryFactory);
                FdoILineString * lls = static_cast<FdoILineString *> (linearizedGeomInside.p);
                linearizedGeometries->Add(lls);
            }

            newGeometry = geometryFactory->CreateMultiLineString(linearizedGeometries);   
        }
        break;

	case FdoGeometryType_MultiCurvePolygon:
        {
            FdoIMultiCurvePolygon * geom = static_cast<FdoIMultiCurvePolygon *> (geometry);

            FdoPtr<FdoPolygonCollection> linearizedGeometries = FdoPolygonCollection::Create();

            for (FdoInt32 i = 0;  i < geom->GetCount();  i++)
            {
                FdoPtr<FdoICurvePolygon> geomInside = geom->GetItem(i);
                FdoPtr<FdoIGeometry> linearizedGeomInside = ApproximateGeometryWithLineStrings(geomInside, maxSpacing, maxOffset, geometryFactory);
                FdoIPolygon * lp = static_cast<FdoIPolygon *> (linearizedGeomInside.p);
                linearizedGeometries->Add(lp);
            }

            newGeometry = geometryFactory->CreateMultiPolygon(linearizedGeometries);   
        }
        break;

    default:
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_10_UNSUPPORTEDGEOMETRYTYPE)));
    }

    return newGeometry;
}

bool FdoSpatialUtility::ArePositionsEqualXYZ(
    FdoIDirectPosition * p1,
    FdoIDirectPosition * p2)
{
    bool areEqual =
        FdoMathUtility::AreEqualAndMaybeNan(p1->GetX(), p2->GetX()) &&
        FdoMathUtility::AreEqualAndMaybeNan(p1->GetY(), p2->GetY()) &&
        FdoMathUtility::AreEqualAndMaybeNan(p1->GetZ(), p2->GetZ());
    return areEqual;
}

bool FdoSpatialUtility::ArePositionsEqualXY(
    FdoIDirectPosition * p1,
    FdoIDirectPosition * p2)
{
    bool areEqual =
        FdoMathUtility::AreEqualAndMaybeNan(p1->GetX(), p2->GetX()) &&
        FdoMathUtility::AreEqualAndMaybeNan(p1->GetY(), p2->GetY());
    return areEqual;
}

double FdoSpatialUtility::DistanceBetweenPositionsXYZ(
    FdoIDirectPosition * p1,
    FdoIDirectPosition * p2)
{
    double x1 = p1->GetX();
    double y1 = p1->GetY();
    double z1 = p1->GetZ();
    double x2 = p2->GetX();
    double y2 = p2->GetY();
    double z2 = p2->GetZ();

    double dx = x2 - x1;
    double dy = y2 - y1;
    bool zIsNan = ( FdoMathUtility::IsNan(z1) || FdoMathUtility::IsNan(z2) );
    double dz = ( zIsNan ? 0.0 : z2 - z1 );

    double distance = sqrt (dx*dx + dy*dy + dz*dz);

    return distance;
}


double FdoSpatialUtility::DistanceBetweenPositionsXY(
    FdoIDirectPosition * p1,
    FdoIDirectPosition * p2)
{
    double x1 = p1->GetX();
    double y1 = p1->GetY();
    double x2 = p2->GetX();
    double y2 = p2->GetY();

    double dx = x2 - x1;
    double dy = y2 - y1;

    double distance = sqrt (dx*dx + dy*dy);

    return distance;
}


void FdoSpatialUtility::AppendLinearizedCurveSegmentsToDistinctCollection(
    FdoDirectPositionCollection * distinctPositions,
    FdoCurveSegmentCollection * curveSegments,
    double maxSpacing,
    double maxOffset)
{
    for (FdoInt32 i = 0;  i < curveSegments->GetCount();  i++)
    {
        FdoPtr<FdoICurveSegmentAbstract> cs = curveSegments->GetItem(i);
        FdoGeometryComponentType gct = cs->GetDerivedType();

        switch (gct)
        {
        case FdoGeometryComponentType_LineStringSegment:
            {
                FdoILineStringSegment * ls = static_cast<FdoILineStringSegment *> ((FdoICurveSegmentAbstract *)cs);
                FdoPtr<FdoDirectPositionCollection> segmentPositions = ls->GetPositions();
                AppendPositionsToDistinctCollection(distinctPositions, segmentPositions);
            }
            break;
        case FdoGeometryComponentType_CircularArcSegment:
            {
                FdoICircularArcSegment * arc = static_cast<FdoICircularArcSegment *> ((FdoICurveSegmentAbstract *)cs);
                FdoPtr<FdoIDirectPosition> start = arc->GetStartPosition();
                FdoPtr<FdoIDirectPosition> mid = arc->GetMidPoint();
                FdoPtr<FdoIDirectPosition> end = arc->GetEndPosition();
                FdoSpatialUtilityCircularArc utilArc(start, mid, end);
                utilArc.Linearize(distinctPositions, maxSpacing, maxOffset);
            }
            break;
        default:
            throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_10_UNSUPPORTEDGEOMETRYTYPE)));
        }
    }
}


void FdoSpatialUtility::AppendPositionToDistinctCollection(
    FdoDirectPositionCollection * distinctPositions,
    FdoIDirectPosition * positionToAppend)
{
    bool isDistinct = true;
    FdoInt32 count = distinctPositions->GetCount();

    if (count > 0)
    {
        FdoPtr<FdoIDirectPosition> pLastPos = distinctPositions->GetItem(count-1);

        if (ArePositionsEqualXYZ(pLastPos, positionToAppend))
            isDistinct = false;
    }

    if (isDistinct)
        distinctPositions->Add(positionToAppend);
}

void FdoSpatialUtility::AppendPositionsToDistinctCollection(
    FdoDirectPositionCollection * distinctPositions,
    FdoDirectPositionCollection * positionsToAppend)
{
    // Slight performance might be gained here by assuming that source points
    // are themselves distinct.
    for (FdoInt32 i = 0;  i < positionsToAppend->GetCount();  i++)
    {
        FdoPtr<FdoIDirectPosition> pos = positionsToAppend->GetItem(i);
        AppendPositionToDistinctCollection(distinctPositions, pos);
    }
}

FdoDirectPositionImpl * FdoSpatialUtility::UnitNormalVector(
        FdoIDirectPosition * p1,
        FdoIDirectPosition * p2,
        FdoIDirectPosition * p3)
{
    // Take the cross product = (p2-p1) X (p3-p1);

    bool hasZ =
        p1->GetDimensionality() &
        p2->GetDimensionality() &
        p3->GetDimensionality() &
        FdoDimensionality_Z;

    double v21[3], v31[3];

	v21[0] =  p2->GetX() - p1->GetX();
	v21[1] =  p2->GetY() - p1->GetY();
    v21[2] =  hasZ ? ( p2->GetZ() - p1->GetZ() ) : 0.0;

	v31[0] =  p3->GetX() - p1->GetX();
	v31[1] =  p3->GetY() - p1->GetY();
    v31[2] =  hasZ ? ( p3->GetZ() - p1->GetZ() ) : 0.0;

    double normalX = FdoMathUtility::SnapToZero(v21[1] * v31[2] - v31[1] * v21[2]);
    double normalY = FdoMathUtility::SnapToZero(v21[2] * v31[0] - v31[2] * v21[0]);
    double normalZ = FdoMathUtility::SnapToZero(v21[0] * v31[1] - v31[0] * v21[1]);
	
    // Normalize

	double a;
	a = (normalX*normalX) + (normalY*normalY) + (normalZ*normalZ);
    a = FdoMathUtility::SnapToZero(a);
    if (0.0 != a)
    	a = 1 / sqrt ( a );

    FdoDirectPositionImpl * unitNormal = FdoDirectPositionImpl::Create(a * normalX, a * normalY, a * normalZ);

	return unitNormal;
}

// This implementation is a collapsed version of logic taken from Autodesk's AcGe package:
//     - AcGeImpEntity3d::rotateBy()
//     - AcGeMatrix3d::setToRotation()
//     - AcGeMatrix3d::preMultBy()
//     - AcGeQuaternion::setToAngleAndAxis()
//     - AcGeQuaternion::getMatrix()
//     - AcGePoint3d::transformBy()
FdoDirectPositionImpl * FdoSpatialUtility::RotatePosition(
    FdoIDirectPosition * position,
    double angle,
    FdoIDirectPosition * axisVector,
    FdoIDirectPosition * center)
{
    // Translate the rotation center to the origin.
    double tpx = position->GetX() - center->GetX(); // Translated Position's X ordinate
    double tpy = position->GetY() - center->GetY();
    double tpz = position->GetZ() - center->GetZ();

    // Create the Quaternion, with w, x, y, z dimensions;
    double halfAngle = angle / 2.0;
    double qw = cos(halfAngle);
    double qx = sin(halfAngle) * axisVector->GetX();
    double qy = sin(halfAngle) * axisVector->GetY();
    double qz = sin(halfAngle) * axisVector->GetZ();

    // Create a 3D transformation matrix from the Quaternion.
    // In a full implementation of a transformation matrix, its size would
    // be 4x4.  However, with no scaling needed, we just fill out a 3x3 size.

    double twoX = qx + qx;
    double twoY = qy + qy;
    double twoZ = qz + qz;

    double twoWX = qw * twoX;
    double twoXX = qx * twoX;

    double twoWY = qw * twoY;
    double twoXY = qx * twoY;
    double twoYY = qy * twoY;

    double twoWZ = qw * twoZ;
    double twoXZ = qx * twoZ;
    double twoYZ = qy * twoZ;
    double twoZZ = qz * twoZ;

    double m[3][3]; // Transformation matrix.
    m[0][0] = 1.0 - (twoYY + twoZZ);
    m[0][1] = twoXY - twoWZ;
    m[0][2] = twoXZ + twoWY;
    m[1][0] = twoXY + twoWZ;
    m[1][1] = 1.0 - (twoXX + twoZZ);
    m[1][2] = twoYZ - twoWX;
    m[2][0] = twoXZ - twoWY;
    m[2][1] = twoYZ + twoWX;
    m[2][2] = 1.0 - (twoXX + twoYY);

    // Rotate about the origin..
    double trpx = m[0][0] * tpx     // Translated, Rotated position's X ordinate.
                + m[0][1] * tpy
                + m[0][2] * tpz;

    double trpy = m[1][0] * tpx
                + m[1][1] * tpy
                + m[1][2] * tpz;

    double trpz = m[2][0] * tpx
                + m[2][1] * tpy
                + m[2][2] * tpz;

    // Translate back to respect the translation center.
    double trtpx = trpx + center->GetX();   // Translated, Rotated, Translated position's X ordinate
    double trtpy = trpy + center->GetY();
    double trtpz = trpz + center->GetZ();

    FdoDirectPositionImpl * trtp = FdoDirectPositionImpl::Create(trtpx, trtpy, trtpz);
    if (NULL == trtp)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));

    return trtp;
}

// Define "geometry type masks" -- bit masks for easier comparisons between requirements
// and allowed types.
#define GTM_POINT                   (1<<0)
#define GTM_LINESTRING              (1<<1)
#define GTM_POLYGON                 (1<<2)
#define GTM_MULTIPOINT              (1<<3)
#define GTM_MULTILINESTRING         (1<<4)
#define GTM_MULTIPOLYGON            (1<<5)
#define GTM_MULTIGEOMETRY           (1<<6)
#define GTM_CURVESTRING             (1<<7)
#define GTM_CURVEPOLYGON            (1<<8)
#define GTM_MULTICURVESTRING        (1<<9)
#define GTM_MULTICURVEPOLYGON       (1<<10)
#define GTM_CIRCULARARCSEGMENT      (1<<11)
#define GTM_LINESTRINGSEGMENT       (1<<12)
#define GTM_LINEARRING              (1<<13)
#define GTM_RING                    (1<<14)

// Compare bit-masks.
#define IS_ALLOWED(hopefullyAllowed, allThatAreAllowed) \
    (((hopefullyAllowed) & ~(allThatAreAllowed)) == 0)

static FdoInt32 ConvertGeometryTypeToMask(
    FdoGeometryType             geometryType)
{
    FdoInt32 mask = 0;

    switch (geometryType)
    {
    case FdoGeometryType_LineString:
        mask = GTM_LINESTRING;
        break;
    case FdoGeometryType_Point:
        mask = GTM_POINT;
        break;
    case FdoGeometryType_Polygon:
        mask = GTM_POLYGON;
        break;
    case FdoGeometryType_MultiPoint:
        mask = GTM_MULTIPOINT;
        break;
    case FdoGeometryType_MultiLineString:
        mask = GTM_MULTILINESTRING;
        break;
    case FdoGeometryType_MultiPolygon:
        mask = GTM_MULTIPOLYGON;
        break;
    case FdoGeometryType_CurveString:
        mask = GTM_CURVESTRING;
        break;
    case FdoGeometryType_CurvePolygon:
        mask = GTM_CURVEPOLYGON;
        break;
    case FdoGeometryType_MultiGeometry:
        mask = GTM_MULTIGEOMETRY;
        break;
    case FdoGeometryType_MultiCurveString:
        mask = GTM_MULTICURVESTRING;
        break;
    case FdoGeometryType_MultiCurvePolygon:
        mask = GTM_MULTICURVEPOLYGON;
        break;
    default:
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_10_UNSUPPORTEDGEOMETRYTYPE)));
    }
    return mask;
}

static FdoInt32 ConvertGeometryComponentTypeToMask(
    FdoGeometryComponentType    geometryComponentType)
{
    FdoInt32 mask = 0;

    switch (geometryComponentType)
    {
	case FdoGeometryComponentType_CircularArcSegment:
        mask = GTM_CIRCULARARCSEGMENT;
        break;
	case FdoGeometryComponentType_LineStringSegment:
        mask = GTM_LINESTRINGSEGMENT;
        break;
	case FdoGeometryComponentType_LinearRing:
        mask = GTM_LINEARRING;
        break;
	case FdoGeometryComponentType_Ring:
        mask = GTM_RING;
        break;
    default:
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_10_UNSUPPORTEDGEOMETRYTYPE)));
    }
    return mask;
}

static FdoInt32 ConvertTypesToMask(
    FdoInt32                    geometryTypesCount,
    FdoGeometryType *           geometryTypes,
    FdoInt32                    geometryComponentTypesCount,
    FdoGeometryComponentType *  geometryComponentTypes)
{
    FdoInt32 mask = 0;

    if (geometryTypesCount <= 0)
    {
        // No constraint.  Allow all geometry types supported by this package.
        mask |= GTM_POINT|GTM_LINESTRING|GTM_POLYGON|GTM_MULTIPOINT|GTM_MULTILINESTRING|
                GTM_MULTIPOLYGON|GTM_MULTIGEOMETRY|GTM_CURVESTRING|GTM_CURVEPOLYGON|
                GTM_MULTICURVESTRING|GTM_MULTICURVEPOLYGON;
    }
    else
    {
        for (FdoInt32 i = 0;  i < geometryTypesCount;  i++)
        {
            mask |= ConvertGeometryTypeToMask(geometryTypes[i]);
        }
    }

    if (geometryComponentTypesCount <= 0)
    {
        // No constraint.  Allow all geometry component types supported by this package.
        mask |= GTM_CIRCULARARCSEGMENT|GTM_LINESTRINGSEGMENT|
                GTM_LINEARRING|GTM_RING;
    }
    else
    {
        for (FdoInt32 i = 0;  i < geometryComponentTypesCount;  i++)
        {
            mask |= ConvertGeometryComponentTypeToMask(geometryComponentTypes[i]);
        }
    }

    return mask;
}

static FdoInt32 GetTypeMaskFromCurveSegments(
    FdoCurveSegmentCollection * curveSegments)
{
    FdoInt32 mask = 0;

    for (FdoInt32 i = 0;  i < curveSegments->GetCount();  i++)
    {
        FdoPtr<FdoICurveSegmentAbstract> cs = curveSegments->GetItem(i);
        FdoGeometryComponentType gct = cs->GetDerivedType();

        switch (gct)
        {
        case FdoGeometryComponentType_LineStringSegment:
             mask |= GTM_LINESTRINGSEGMENT;
            break;
        case FdoGeometryComponentType_CircularArcSegment:
             mask |= GTM_CIRCULARARCSEGMENT;
            break;
        default:
            throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_10_UNSUPPORTEDGEOMETRYTYPE)));
        }
    }
    return mask;
}

static FdoInt32 GetTypeMaskFromGeometry(
    FdoIGeometry *              geometry)
{
    FdoGeometryType geomType = geometry->GetDerivedType();
    FdoInt32        mask = ConvertGeometryTypeToMask(geomType);

    // Traverse all aggregate or composite types.
    // Note: ring component masks are added here because FdoIRingAbstract lacks
    // a GetDerivedType.  The ring types are fixed anyway.

    switch (geomType)
    {
    case FdoGeometryType_Polygon:
    case FdoGeometryType_MultiPolygon:
        mask |= GTM_LINEARRING;
        break;

	case FdoGeometryType_CurveString:
        {
            FdoICurveString * geom = static_cast<FdoICurveString *> (geometry);
            FdoPtr<FdoCurveSegmentCollection> csc = geom->GetCurveSegments();
            mask |= GetTypeMaskFromCurveSegments(csc);
        }
        break;

	case FdoGeometryType_CurvePolygon:
        {
            FdoICurvePolygon * geom = static_cast<FdoICurvePolygon *> (geometry);
            mask |= GTM_RING;

            FdoPtr<FdoIRing> ring = geom->GetExteriorRing();
            FdoPtr<FdoCurveSegmentCollection> csc = ring->GetCurveSegments();
            mask |= GetTypeMaskFromCurveSegments(csc);

            for (FdoInt32 i = 0;  i < geom->GetInteriorRingCount();  i++)
            {
                ring = geom->GetInteriorRing(i);
                csc = ring->GetCurveSegments();
                mask |= GetTypeMaskFromCurveSegments(csc);
            }
        }
        break;

	case FdoGeometryType_MultiGeometry:
        {
            FdoIMultiGeometry * geom = static_cast<FdoIMultiGeometry *> (geometry);

            for (FdoInt32 i = 0;  i < geom->GetCount();  i++)
            {
                FdoPtr<FdoIGeometry> geomInside = geom->GetItem(i);
                mask |= GetTypeMaskFromGeometry(geomInside);
            }
        }
        break;

	case FdoGeometryType_MultiCurveString:
        {
            FdoIMultiCurveString * geom = static_cast<FdoIMultiCurveString *> (geometry);

            for (FdoInt32 i = 0;  i < geom->GetCount();  i++)
            {
                FdoPtr<FdoICurveString> geomInside = geom->GetItem(i);
                mask |= GetTypeMaskFromGeometry(geomInside);
            }
        }
        break;

	case FdoGeometryType_MultiCurvePolygon:
        {
            FdoIMultiCurvePolygon * geom = static_cast<FdoIMultiCurvePolygon *> (geometry);
            mask |= GTM_RING;

            for (FdoInt32 i = 0;  i < geom->GetCount();  i++)
            {
                FdoPtr<FdoICurvePolygon> geomInside = geom->GetItem(i);
                mask |= GetTypeMaskFromGeometry(geomInside);
            }
        }
        break;

    default:
        ; // No error (this switch is only for a few types).
    }

    return mask;
}

// This function is useful as long as there is only a single way to approximate a geometry.
// Presently, there is only one way for each type (e.g. CurveString is always approximated by
// LineString, never by 1-to-1 substitution of curve segments).
static FdoInt32 FindApproximationTypeMask(
    FdoInt32                    geomTypeMask)
{
    FdoInt32                    mask = 0;

    // To guard against future geometry types and geometry component types, check all 
    // combinations.
    FdoInt32 allApproximateableTypesMask =
        GTM_POINT|GTM_LINESTRING|GTM_POLYGON|GTM_MULTIPOINT|GTM_MULTILINESTRING|
        GTM_MULTIPOLYGON|GTM_MULTIGEOMETRY|GTM_CURVESTRING|GTM_CURVEPOLYGON|
        GTM_MULTICURVESTRING|GTM_MULTICURVEPOLYGON|GTM_CIRCULARARCSEGMENT|
        GTM_LINESTRINGSEGMENT|GTM_LINEARRING|GTM_RING;

    if ( ! IS_ALLOWED(geomTypeMask, allApproximateableTypesMask) )
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_10_UNSUPPORTEDGEOMETRYTYPE)));

    // Now just map by geometry type (all geometry component types are supported in all approximations).
    if ( (geomTypeMask & GTM_CURVESTRING) != 0 )
        mask |= GTM_LINESTRING;
    if ( (geomTypeMask & GTM_CURVEPOLYGON) != 0 )
        mask |= GTM_POLYGON|GTM_LINEARRING;
    if ( (geomTypeMask & GTM_MULTICURVESTRING) != 0 )
        mask |= GTM_MULTILINESTRING;
    if ( (geomTypeMask & GTM_MULTICURVEPOLYGON) != 0 )
        mask |= GTM_MULTIPOLYGON|GTM_LINEARRING;
    if ( (geomTypeMask & GTM_MULTIGEOMETRY) != 0 )
        mask |= GTM_MULTIGEOMETRY;

    return mask;
}


FdoSpatialGeometryValidity FdoSpatialUtility::ValidateGeometryByType(
    FdoIGeometry *              geometry, 
    FdoInt32                    geometryTypesCount,
    FdoGeometryType *           geometryTypes,
    FdoInt32                    geometryComponentTypesCount,
    FdoGeometryComponentType *  geometryComponentTypes,
    FdoInt32                    dimensionality)
{
    FdoSpatialGeometryValidity validity = FdoSpatialGeometryValidity_Valid;

    // Check parameters.
    if (NULL == geometry ||
        (geometryTypesCount > 0 && NULL == geometryTypes) ||
        (geometryComponentTypesCount > 0 && NULL == geometryComponentTypes))
    {
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_INVALID_INPUT_ON_CLASS_FUNCTION),
                                                               L"FdoSpatialUtility::ValidateGeometryByType",
                                                               L"geometry/geometryTypes/geometryComponentTypes"));
    }
    FdoInt32 allDimensionalities = FdoDimensionality_XY | FdoDimensionality_Z | FdoDimensionality_M;
    if ( ! IS_ALLOWED(dimensionality, allDimensionalities) )
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_17_UNSUPPORTEDDIMENSIONALITY)));

    FdoInt32 dim = geometry->GetDimensionality();
    if ( ! IS_ALLOWED(dim, dimensionality) )
    {
        validity = FdoSpatialGeometryValidity_InvalidDueToDimensionality;
    }
    else
    {
        FdoGeometryType geomType = geometry->GetDerivedType();
        FdoInt32        geomTypeMask = GetTypeMaskFromGeometry(geometry);
        FdoInt32        allowedGeomTypeMask = ConvertTypesToMask(
            geometryTypesCount, geometryTypes, geometryComponentTypesCount, geometryComponentTypes);

        if ( ! IS_ALLOWED(geomTypeMask, allowedGeomTypeMask) )
        {
            FdoInt32 approximationTypeMask = FindApproximationTypeMask(geomTypeMask);

            if ( IS_ALLOWED(approximationTypeMask, allowedGeomTypeMask) )
            {
                validity = FdoSpatialGeometryValidity_InvalidButCanBeApproximated;
            }
            else
            {
                validity = FdoSpatialGeometryValidity_Invalid;
            }
        }
    }
    return validity;
}



#define GET_POSITION(p, xptr, yptr, dimptr) \
        {   \
            double dummy1; \
            (p)->GetPositionByMembers(xptr, yptr, &dummy1, &dummy1, dimptr); \
        }

#define GET_ITEM(p, i, xptr, yptr, dimptr)  \
        {                  \
            double dummy1; \
            (p)->GetItemByMembers(i, xptr, yptr, &dummy1, &dummy1, dimptr); \
        }

//------------------------------------------------------------------------------
//
// Low level geometry code.
//
//------------------------------------------------------------------------------

bool FdoSpatialUtility::pt_is_on_line(
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
    double      toleranceXY)
{
    bool        IsOnLine = false;

    *IsAtLinePt1_O = false;
    *IsAtLinePt2_O = false;

    /* If point is same as line's first point... */
    if ( fabs(PtX-LineX1) <= toleranceXY && fabs(PtY-LineY1) <= toleranceXY ) {

        IsOnLine = true;
        *IsAtLinePt1_O = true;

    /* Else if point is same as line's second point... */
    } else if ( fabs(PtX-LineX2) <= toleranceXY && fabs(PtY-LineY2) <= toleranceXY ) {

        IsOnLine = true;
        *IsAtLinePt2_O = true;

    /* Else use point-to-line calculations. */
    } else {

        double      Numerator1  = ((LineY1-PtY) * -Dy) - ((LineX1-PtX) * Dx);
        double      r           = Numerator1 / LenSq;

        /* If projected point is somewhere on the second line... */
        if ( r > 0.0 && r < 1.0 ) {

            double  Numerator2   = ((LineY1-PtY) * Dx) - ((LineX1-PtX) * Dy);
            double  PtToLineDist = Numerator2 / Len;

            /* If the given point is on the second line... */
            if ( fabs(PtToLineDist) <= toleranceXY ) {

                IsOnLine = true;
            }
        }
    }
    return IsOnLine;
}

// A wrapper function over pt_is_on_line()
bool FdoSpatialUtility::pt_is_on_line2(
    double      LineX1,
    double      LineY1,
    double      LineX2,
    double      LineY2,
    double      PtX,
    double      PtY,
    double      toleranceXY)
{
    double      Dx = (LineX2 - LineX1);
    double      Dy = (LineY2 - LineY1);
    double      LengthSq = ( Dx * Dx ) + ( Dy * Dy );
    double      Length = sqrt(LengthSq);
    bool        DummyBool;

    bool        IsOnLine = false;

    /* If the point is on the first line... */
    return pt_is_on_line( LineX1, LineY1, LineX2, LineY2, Dx, Dy,
                        Length, LengthSq, PtX, PtY, 
                        &DummyBool, &DummyBool,
                        toleranceXY);
}

int FdoSpatialUtility::find_xsect_seg_seg(
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
    double      toleranceXY)
{
    double      dx1 = x12 - x11;
    double      dy1 = y12 - y11;
    double      dx2 = x22 - x21;
    double      dy2 = y22 - y21;
    double      x11x21 = x11 - x21;
    double      y11y21 = y11 - y21;
    bool        IsDegenerate1 = ( fabs(dx1) <= toleranceXY && fabs(dy1) <= toleranceXY);
    bool        IsDegenerate2 = ( fabs(dx2) <= toleranceXY && fabs(dy2) <= toleranceXY);
    double      r, s;
    double      numerator1, numerator2;
    double      denominator;
    double      Length;
    double      LengthSq;
    bool        AreParallel = false;
    bool        AreCoincident = false;
    int         NumIntersections = 0;
    double      xi1 = HUGE_VAL;
    double      yi1 = HUGE_VAL;
    double      xi2 = HUGE_VAL;
    double      yi2 = HUGE_VAL;
    bool        IsInterp1 = false;
    bool        IsInterp2 = false;
    bool        DummyBool;

    /* If both segments are degenerate... */
    if ( IsDegenerate1 && IsDegenerate2 ) {

        /* If the points are the same... */
        if ( fabs(x11x21) <= toleranceXY && fabs(y11y21) <= toleranceXY ) {

            /* Just use the first point. */
            NumIntersections = 1;
            xi1 = x11;
            yi1 = y11;
        }

    /* Else if just the first segment is degenerate... */
    } else if ( IsDegenerate1 && !IsDegenerate2 ) {

        LengthSq = ( dx2 * dx2 ) + ( dy2 * dy2 );
        Length = sqrt(LengthSq);

        /* If the point is on the second line... */
        if ( pt_is_on_line( x21, y21, x22, y22, dx2, dy2,
                            Length, LengthSq, x11, y11, 
                            &DummyBool, &DummyBool, toleranceXY ) ) {

            /* Just use the point. */
            NumIntersections = 1;
            xi1 = x11;
            yi1 = y11;
        }

    /* Else if just the second segment is degenerate... */
    } else if ( !IsDegenerate1 && IsDegenerate2 ) {

        LengthSq = ( dx1 * dx1 ) + ( dy1 * dy1 );
        Length = sqrt(LengthSq);

        /* If the point is on the first line... */
        if ( pt_is_on_line( x11, y11, x12, y12, dx1, dy1,
                            Length, LengthSq, x21, y21, 
                            &DummyBool, &DummyBool, toleranceXY ) ) {

            /* Just use the point. */
            NumIntersections = 1;
            xi1 = x21;
            yi1 = y21;
        }

    /* Else both segments are non-degenerate... */
    } else /* !IsDegenerate1 && !IsDegenerate2 */ {

        bool    Pt11IsOnLine2;
        bool    Pt12IsOnLine2;
        bool    Pt21IsOnLine1;
        bool    Pt22IsOnLine1;

        bool    Pt11IsOnLine2Pt1;
        bool    Pt11IsOnLine2Pt2;
        bool    Pt12IsOnLine2Pt1;
        bool    Pt12IsOnLine2Pt2;
        bool    Pt21IsOnLine1Pt1;
        bool    Pt21IsOnLine1Pt2;
        bool    Pt22IsOnLine1Pt1;
        bool    Pt22IsOnLine1Pt2;

        LengthSq = ( dx2 * dx2 ) + ( dy2 * dy2 );
        Length = sqrt(LengthSq);

        Pt11IsOnLine2 = pt_is_on_line( x21, y21, x22, y22, dx2, dy2,
                                       Length, LengthSq, x11, y11, 
                                       &Pt11IsOnLine2Pt1, &Pt11IsOnLine2Pt2, toleranceXY );

        Pt12IsOnLine2 = pt_is_on_line( x21, y21, x22, y22, dx2, dy2,
                                       Length, LengthSq, x12, y12,
                                       &Pt12IsOnLine2Pt1, &Pt12IsOnLine2Pt2, toleranceXY );

        LengthSq = ( dx1 * dx1 ) + ( dy1 * dy1 );
        Length = sqrt(LengthSq);

        Pt21IsOnLine1 = pt_is_on_line( x11, y11, x12, y12, dx1, dy1,
                                       Length, LengthSq, x21, y21, 
                                       &Pt21IsOnLine1Pt1, &Pt21IsOnLine1Pt2, toleranceXY );

        Pt22IsOnLine1 = pt_is_on_line( x11, y11, x12, y12, dx1, dy1,
                                       Length, LengthSq, x22, y22, 
                                       &Pt22IsOnLine1Pt1, &Pt22IsOnLine1Pt2, toleranceXY );

        if ( Pt11IsOnLine2 && Pt12IsOnLine2 ) {

            /* Either the lines are identical, or line 1 is subset of
             * line 2.  Either way, the first line's endpoints denote
             * the two intersections. */

            NumIntersections = 2;
            xi1 = x11;
            yi1 = y11;
            xi2 = x12;
            yi2 = y12;
            AreParallel = true;
            AreCoincident = true;

        } else if ( Pt21IsOnLine1 && Pt22IsOnLine1 ) {

            /* Line 2 is a subset of line 1.  Use line 2's points. */
            NumIntersections = 2;
            xi1 = x21;
            yi1 = y21;
            xi2 = x22;
            yi2 = y22;
            AreParallel = true;
            AreCoincident = true;

        } else if ( Pt11IsOnLine2Pt1 || Pt11IsOnLine2Pt2 ) {

            /* Line 1's first point touches one end of Line 2. */
            NumIntersections = 1;
            xi1 = x11;
            yi1 = y11;
        
        } else if ( Pt12IsOnLine2Pt1 || Pt12IsOnLine2Pt2 ) {

            /* Line 1's second point touches one end of Line 2. */
            NumIntersections = 1;
            xi1 = x12;
            yi1 = y12;
        
        } else if ( Pt11IsOnLine2 && Pt21IsOnLine1 ) {

            /* Lines overlap across their first points. */
            NumIntersections = 2;
            xi1 = x21;
            yi1 = y21;
            xi2 = x11;
            yi2 = y11;
            AreParallel = true;
            AreCoincident = true;

        } else if ( Pt12IsOnLine2 && Pt21IsOnLine1 ) {

            /* Lines overlap across line 1's last and line 2's first pts. */
            NumIntersections = 2;
            xi1 = x21;
            yi1 = y21;
            xi2 = x12;
            yi2 = y12;
            AreParallel = true;
            AreCoincident = true;

        } else if ( Pt11IsOnLine2 && Pt22IsOnLine1 ) {

            /* Lines overlap across line 1's first and line 2's last pts. */
            NumIntersections = 2;
            xi1 = x22;
            yi1 = y22;
            xi2 = x11;
            yi2 = y11;
            AreParallel = true;
            AreCoincident = true;

        } else if ( Pt12IsOnLine2 && Pt22IsOnLine1 ) {

            /* Lines overlap across their last points. */
            NumIntersections = 2;
            xi1 = x12;
            yi1 = y12;
            xi2 = x22;
            yi2 = y22;
            AreParallel = true;
            AreCoincident = true;

        } else if ( Pt11IsOnLine2 ) {

            /* line 1's first point touches some point in line 2. */
            NumIntersections = 1;
            xi1 = x11;
            yi1 = y11;
            IsInterp1 = true;

        } else if ( Pt12IsOnLine2 ) {

            /* line 1's last point touches some point in line 2. */
            NumIntersections = 1;
            xi1 = x12;
            yi1 = y12;
            IsInterp1 = true;

        } else if ( Pt21IsOnLine1 ) {

            /* line 2's first point touches some point in line 1. */
            NumIntersections = 1;
            xi1 = x21;
            yi1 = y21;
            IsInterp1 = true;

        } else if ( Pt22IsOnLine1 ) {

            /* line 2's last point touches some point in line 1. */
            NumIntersections = 1;
            xi1 = x22;
            yi1 = y22;
            IsInterp1 = true;

        } else {

            /* There are no special endpoint conditions.  Perform a
             * line-to-line calculation.
             */

            numerator1 = (y11y21 * dx2) - (x11x21 * dy2);
            denominator = (dx1 * dy2) - (dy1 * dx2);

            AreParallel = ( fabs(denominator) < toleranceXY);

            /* If the lines are not parallel... */
            if ( !AreParallel ) {

                r = numerator1 / denominator;

                /* If the (possibly) projected intersection is on Line 1... */
                if ( r > 0.0 && r < 1.0 ) {

                    numerator2 = (y11y21 * dx1) - (x11x21 * dy1);
                    s = numerator2 / denominator;

                    /* If the intersection is on Line 2... */
                    if ( s > 0.0 && s < 1.0 ) {

                        /* Calculate intersection of the two lines. */
                        xi1 = x11 + ( r * dx1 );
                        yi1 = y11 + ( r * dy1 );

                        NumIntersections = 1;
                        IsInterp1 = true;
                    }
                }
            }
        }
    }

    if ( NumIntersections >= 1 ) {
        if ( xi1_O != NULL ) {
            *xi1_O = xi1;
        }
        if ( yi1_O != NULL ) {
            *yi1_O = yi1;
        }
        if ( IsInterp1_O != NULL ) {
            *IsInterp1_O = IsInterp1;
        }
    }

    if ( NumIntersections >= 2 ) {
        if ( xi2_O != NULL ) {
            *xi2_O = xi2;
        }
        if ( yi2_O != NULL ) {
            *yi2_O = yi2;
        }
        if ( IsInterp2_O != NULL ) {
            *IsInterp2_O = IsInterp2;
        }
    }

    return NumIntersections;
}



//returns 0 - no intersection, 1 - unique intersection, 2 - overlap (two intersection pts returned)
int FdoSpatialUtility::line_segment_intersect(
                           double* line0, //4 doubles
                           double* line1, //4 doubles
                           double* ret,   //4 doubles
                           double  toleranceXY)
{
    int         xflag;
    int         yflag;
    double      xmin0 = min(line0[0], line0[2]);
    double      ymin0 = min(line0[1], line0[3]);
    double      xmax0 = max(line0[0], line0[2]);
    double      ymax0 = max(line0[1], line0[3]);
    double      xmin1 = min(line1[0], line1[2]);
    double      ymin1 = min(line1[1], line1[3]);
    double      xmax1 = max(line1[0], line1[2]);
    double      ymax1 = max(line1[1], line1[3]);

    int         numItersections = 0;

    bool overlap = check_xtnt_tol (
                            xmin0, ymin0, xmax0, ymax0, 
                            xmin1, ymin1, xmax1, ymax1,
                            &xflag, &yflag, 
                            0.001 );
    if ( overlap )
    {
        numItersections = find_xsect_seg_seg(
                            line0[0], line0[1], line0[2], line0[3],
                            line1[0], line1[1], line1[2], line1[3],
                            &ret[0], &ret[1], NULL,
                            &ret[2], &ret[3], NULL, 
                            toleranceXY);
    }

    return numItersections;
}



//returns true if line1 overlaps line2 and does not go beyond its
//endpoints, false otherwise
bool FdoSpatialUtility::line_contains_line(double* line0, double* line1, double toleranceXY)
{
    double sqrEpsilon = toleranceXY*toleranceXY;
    double d0x = line0[2] - line0[0];
    double d0y = line0[3] - line0[1];

    double d1x = line1[2] - line1[0];
    double d1y = line1[3] - line1[1];

    double ex = line1[0] - line0[0];
    double ey = line1[1] - line0[1];

    double kross = d0x * d1y - d0y * d1x;

    double sqrLen0 = d0x * d0x + d0y * d0y;
    double sqrLen1 = d1x * d1x + d1y * d1y;

    double sqrkross = kross * kross;

    if (sqrkross > sqrEpsilon * sqrLen0 * sqrLen1)
    {
        //lines are not parallel
        return false;
    }

    //lines of the segments are parallel
    double sqrLenE = ex * ex + ey * ey;
    kross = ex * d0y - ey * d0x;
    double sqrKross = kross * kross;

    if (sqrKross >= sqrEpsilon * sqrLen0 * sqrLen1)
    {
        //lines of the segments are not the same
        return false;
    }

    //lines are same, test for overlap
    double s0 = (d0x * ex + d0y * ey) / sqrLen0;
    double s1 = s0 + (d0x * d1x + d0y * d1y) / sqrLen0;

    double smin = min(s0, s1);
    double smax = max(s0, s1);

    //line1 is inside the endpoints of line0
    if (smin >= 0.0 && smax <= 1.0)
        return true;

    return false;
}



//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------

bool FdoSpatialUtility::Evaluate(FdoIGeometry* g1, FdoSpatialOperations op, FdoIGeometry* g2)
{
    return Evaluate(g1, op, g2, EPSILON);
}

bool FdoSpatialUtility::Evaluate(FdoIGeometry* g1, FdoSpatialOperations op, FdoIGeometry* g2, double toleranceXY)
{
    return Evaluate(g1, op, g2, toleranceXY, EPSILON);
}

bool FdoSpatialUtility::Evaluate(FdoIGeometry* g1, FdoSpatialOperations op, FdoIGeometry* g2, double toleranceXY, double toleranceZ)
{
    if (toleranceXY <= 0.0)
        toleranceXY = EPSILON;
    if (toleranceZ <= 0.0)
        toleranceZ = EPSILON;

    switch (op)
    {
    case FdoSpatialOperations_Contains:     return Contains(g1, g2, toleranceXY); 
    case FdoSpatialOperations_Crosses:      return Crosses(g1, g2, toleranceXY);
    case FdoSpatialOperations_Disjoint:     return Disjoint(g1, g2, toleranceXY);
    case FdoSpatialOperations_Equals:       return Equals(g1, g2, toleranceXY); 
    case FdoSpatialOperations_Intersects:   return Intersects(g1, g2, toleranceXY);
    case FdoSpatialOperations_Overlaps:     return Overlaps(g1, g2, toleranceXY);
    case FdoSpatialOperations_Touches:      return Touches(g1, g2, toleranceXY); 
    case FdoSpatialOperations_Within:       return Within(g1, g2, toleranceXY); 
    case FdoSpatialOperations_CoveredBy:    return CoveredBy(g1, g2, toleranceXY); 
    case FdoSpatialOperations_Inside:       return Inside(g1, g2, toleranceXY);
    case FdoSpatialOperations_EnvelopeIntersects: return EnvelopeIntersects(g1, g2, toleranceXY);
    default: break;
    }

    return false;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------

bool FdoSpatialUtility::Intersects(FdoIGeometry* g1, FdoIGeometry* g2, double toleranceXY)
{
    switch (g1->GetDerivedType())
    {
    case FdoGeometryType_Point: 
        return PointIntersects((FdoIPoint*)g1, g2, toleranceXY);
    case FdoGeometryType_LineString : 
        return LineStringIntersects((FdoILineString*)g1, g2, toleranceXY);
    case FdoGeometryType_Polygon : 
        return PolygonIntersects((FdoIPolygon*)g1, g2, toleranceXY);
    
    case FdoGeometryType_MultiPoint : 
        return MultiPointIntersects((FdoIMultiPoint*)g1, g2, toleranceXY);
    case FdoGeometryType_MultiLineString : 
        return MultiLineStringIntersects((FdoIMultiLineString*)g1, g2, toleranceXY);
    case FdoGeometryType_MultiPolygon : 
        return MultiPolygonIntersects((FdoIMultiPolygon*)g1, g2, toleranceXY);

    case FdoGeometryType_CurveString : 
    case FdoGeometryType_CurvePolygon : 
    case FdoGeometryType_MultiCurveString :
    case FdoGeometryType_MultiCurvePolygon : 
        {
            FdoPtr<FdoIGeometry> flatgeom = TesselateCurve(g1);
            return Intersects(flatgeom, g2, toleranceXY);
        }
    default : break;
    }


    throw FdoException::Create(L"Spatial operation not supported");
    return false;
}

bool FdoSpatialUtility::Overlaps(FdoIGeometry* g1, FdoIGeometry* g2, double toleranceXY)
{
    throw FdoException::Create(L"Spatial operation not supported");
    return false;
}

bool FdoSpatialUtility::Touches(FdoIGeometry* g1, FdoIGeometry* g2, double toleranceXY)
{
    throw FdoException::Create(L"Spatial operation not supported");
    return false;
}


bool FdoSpatialUtility::Crosses(FdoIGeometry* g1, FdoIGeometry* g2, double toleranceXY)
{
    throw FdoException::Create(L"Spatial operation not supported");
    return false;
}

bool FdoSpatialUtility::Equals(FdoIGeometry* g1, FdoIGeometry* g2, double toleranceXY)
{
    throw FdoException::Create(L"Spatial operation not supported");
    return false;
}

bool FdoSpatialUtility::Within(FdoIGeometry* g1, FdoIGeometry* g2, double toleranceXY)
{
    return Contains(g2, g1, toleranceXY, false);
}

bool FdoSpatialUtility::CoveredBy(FdoIGeometry* g1, FdoIGeometry* g2, double toleranceXY)
{
    throw FdoException::Create(L"Spatial operation not supported");
    return false;
}

bool FdoSpatialUtility::Inside(FdoIGeometry* g1, FdoIGeometry* g2, double toleranceXY)
{
     return Contains(g2, g1, toleranceXY, true);
}

bool FdoSpatialUtility::Disjoint(FdoIGeometry* g1, FdoIGeometry* g2, double toleranceXY)
{
    return !Intersects(g1, g2, toleranceXY);
}

bool FdoSpatialUtility::Contains(FdoIGeometry* g1, FdoIGeometry* g2, double toleranceXY, bool strictInside)
{
    switch (g1->GetDerivedType())
    {
    case FdoGeometryType_Point: 
        return false;
    case FdoGeometryType_LineString : 
        return LineStringContains((FdoILineString*)g1, g2, toleranceXY);
    case FdoGeometryType_Polygon : 
        return PolygonContains((FdoIPolygon*)g1, g2, toleranceXY, strictInside);
    case FdoGeometryType_MultiPoint : 
        return MultiPointContains((FdoIMultiPoint*)g1, g2, toleranceXY);
    case FdoGeometryType_MultiLineString : 
        return MultiLineStringContains((FdoIMultiLineString*)g1, g2, toleranceXY);
    case FdoGeometryType_MultiPolygon : 
        return MultiPolygonContains((FdoIMultiPolygon*)g1, g2, toleranceXY);
    
    case FdoGeometryType_CurveString : 
    case FdoGeometryType_CurvePolygon : 
    case FdoGeometryType_MultiCurveString :
    case FdoGeometryType_MultiCurvePolygon : 
        {
            FdoPtr<FdoIGeometry> flatgeom = TesselateCurve(g1);
            return Contains(flatgeom, g2, toleranceXY);
        }
    default : break;
    }

    throw FdoException::Create(L"Spatial operation not supported");
    return false;
}

bool FdoSpatialUtility::EnvelopeIntersects(FdoIGeometry* g1, FdoIGeometry* g2, double toleranceXY)
{
    FdoPtr<FdoIEnvelope> e1 = g1->GetEnvelope();
    FdoPtr<FdoIEnvelope> e2 = g2->GetEnvelope();

    double minX1 = e1->GetMinX();
    double minY1 = e1->GetMinY();
    double maxX1 = e1->GetMaxX();
    double maxY1 = e1->GetMaxY();
    double minX2 = e2->GetMinX();
    double minY2 = e2->GetMinY();
    double maxX2 = e2->GetMaxX();
    double maxY2 = e2->GetMaxY();

    // We do not use chk_xtnt_tol here because of an unresolved issue
    // with extremely small tolerances.  The test below is taken
    // from SDF+'s RTreeInfrastructure.h.

    bool intersects =  !(  minX1 > maxX2 
                        || minY1 > maxY2 
                        || minX2 > maxX1 
                        || minY2 > maxY1);

    return intersects;
}

//-----------------------------------------------------------------------------
//
// PRIMITIVE INTERSECTION
//
//-----------------------------------------------------------------------------


bool FdoSpatialUtility::PolygonIntersects(FdoIPolygon* poly, FdoIGeometry* geom, double toleranceXY)
{
    switch(geom->GetDerivedType())
    {
    case FdoGeometryType_Point: 
        {
            FdoIPoint* pt = (FdoIPoint*)geom;
            double x, y;
            int dim;
            GET_POSITION(pt, &x, &y, &dim);
            return PointInPolygon(poly, x, y, toleranceXY);
        }
    case FdoGeometryType_LineString : 
        return PolygonIntersectsLineString(poly, (FdoILineString*)geom, toleranceXY);
    case FdoGeometryType_Polygon : 
        return PolygonsIntersect(poly, (FdoIPolygon*)geom, toleranceXY);

    case FdoGeometryType_MultiPoint : 
        return MultiPointIntersects((FdoIMultiPoint*)geom, poly, toleranceXY);
    case FdoGeometryType_MultiLineString : 
        return MultiLineStringIntersects((FdoIMultiLineString*)geom, poly, toleranceXY);
    case FdoGeometryType_MultiPolygon : 
        return MultiPolygonIntersects((FdoIMultiPolygon*)geom, poly, toleranceXY);
    
    case FdoGeometryType_CurveString : 
    case FdoGeometryType_CurvePolygon : 
    case FdoGeometryType_MultiCurveString :
    case FdoGeometryType_MultiCurvePolygon : 
        {
            FdoPtr<FdoIGeometry> flatgeom = TesselateCurve(geom);
            return PolygonIntersects(poly, flatgeom, toleranceXY);
        }
    
    default : break;
    }

    return false;
}


bool FdoSpatialUtility::LineStringIntersects(FdoILineString* line, FdoIGeometry* geom, double toleranceXY)
{
    switch(geom->GetDerivedType())
    {
    case FdoGeometryType_Point: 
        {
            FdoIPoint* pt = (FdoIPoint*)geom;

            double x, y;
            int dim;
            GET_POSITION(pt, &x, &y, &dim);
            return PointOnLine(line, x, y, toleranceXY);
        }
    case FdoGeometryType_LineString : 
        return LineStringsIntersect(line, (FdoILineString*)geom, toleranceXY);
    case FdoGeometryType_Polygon : 
        return PolygonIntersectsLineString((FdoIPolygon*)geom, line, toleranceXY);

    case FdoGeometryType_MultiPoint : 
        return MultiPointIntersects((FdoIMultiPoint*)geom, line, toleranceXY);
    case FdoGeometryType_MultiLineString : 
        return MultiLineStringIntersects((FdoIMultiLineString*)geom, line, toleranceXY);
    case FdoGeometryType_MultiPolygon : 
        return MultiPolygonIntersects((FdoIMultiPolygon*)geom, line, toleranceXY);
    
    case FdoGeometryType_CurveString : 
    case FdoGeometryType_CurvePolygon : 
    case FdoGeometryType_MultiCurveString :
    case FdoGeometryType_MultiCurvePolygon : 
        {
            FdoPtr<FdoIGeometry> flatgeom = TesselateCurve(geom);
            return LineStringIntersects(line, flatgeom, toleranceXY);
        }

    default : break;
    }

    return false;
}


bool FdoSpatialUtility::PointIntersects(FdoIPoint* point, FdoIGeometry* geom, double toleranceXY)
{
    switch(geom->GetDerivedType())
    {
    case FdoGeometryType_Point: 
        {
            double x1, y1;
            int dim1;
            GET_POSITION(point, &x1, &y1, &dim1);

            FdoIPoint* pt = (FdoIPoint*)geom;

            double x2, y2;
            int dim2;
            GET_POSITION(pt, &x2, &y2, &dim2);

            return (x1 == x2 && y1 == y2);
        }
    case FdoGeometryType_LineString : 
        {
            double x, y;
            int dim;
            GET_POSITION(point, &x, &y, &dim);
            return PointOnLine((FdoILineString*)geom, x, y, toleranceXY);
        }
    case FdoGeometryType_Polygon : 
        {
            double x, y;
            int dim;
            GET_POSITION(point, &x, &y, &dim);
            return PointInPolygon((FdoIPolygon*)geom, x, y, toleranceXY);
        }

    case FdoGeometryType_MultiPoint : 
        return MultiPointIntersects((FdoIMultiPoint*)geom, point, toleranceXY);
    case FdoGeometryType_MultiLineString : 
        return MultiLineStringIntersects((FdoIMultiLineString*)geom, point, toleranceXY);
    case FdoGeometryType_MultiPolygon : 
        return MultiPolygonIntersects((FdoIMultiPolygon*)geom, point, toleranceXY);

    case FdoGeometryType_CurveString : 
    case FdoGeometryType_CurvePolygon : 
    case FdoGeometryType_MultiCurveString :
    case FdoGeometryType_MultiCurvePolygon : 
        {
            FdoPtr<FdoIGeometry> flatgeom = TesselateCurve(geom);
            return PointIntersects(point, flatgeom, toleranceXY);
        }

    
    default : break;
    }

    return false;
}


//-----------------------------------------------------------------------------
//
// MULTI INTERSECTION
//
//-----------------------------------------------------------------------------


bool FdoSpatialUtility::MultiPolygonIntersects(FdoIMultiPolygon* mpoly, FdoIGeometry* geom, double toleranceXY)
{
    FdoPtr<FdoIPolygon> poly;

    int numpolys = mpoly->GetCount();

    for (int i=0; i<numpolys; i++)
    {
        poly = mpoly->GetItem(i);

        if (PolygonIntersects(poly, geom, toleranceXY))
            return true;
    }

    return false;
}

bool FdoSpatialUtility::MultiPointIntersects(FdoIMultiPoint* mpoint, FdoIGeometry* geom, double toleranceXY)
{
    FdoPtr<FdoIPoint> pt;
    FdoPtr<FdoIDirectPosition> pos;

    int numpts = mpoint->GetCount();

    for (int i=0; i<numpts; i++)
    {
        pt = mpoint->GetItem(i);

        if (PointIntersects(pt, geom, toleranceXY))
            return true;
    }

    return false;
}

bool FdoSpatialUtility::MultiLineStringIntersects(FdoIMultiLineString* mlines, FdoIGeometry* geom, double toleranceXY)
{
    FdoPtr<FdoILineString> ls;

    int numpolys = mlines->GetCount();

    for (int i=0; i<numpolys; i++)
    {
        ls = mlines->GetItem(i);

        if (LineStringIntersects(ls, geom, toleranceXY))
            return true;
    }

    return false;
}

//-----------------------------------------------------------------------------
//
// PRIMITIVE CONTAINS
//
//-----------------------------------------------------------------------------

bool FdoSpatialUtility::PolygonContains(FdoIPolygon* poly, FdoIGeometry* geom, double toleranceXY, bool strictInside)
{
    switch (geom->GetDerivedType())
    {
    case FdoGeometryType_Point: 
        {
            FdoIPoint* pt = (FdoIPoint*)geom;

            double x, y;
            int dim;
            GET_POSITION(pt, &x, &y, &dim);

            bool isOnExtBoundary = false;
            bool isInside = PointInPolygon(poly, x, y, toleranceXY, &isOnExtBoundary);
            if (isOnExtBoundary)
                isInside = !strictInside;
            return isInside;
        }
    case FdoGeometryType_LineString : 
        return PolygonContainsLineString(poly, (FdoILineString*)geom, toleranceXY, strictInside);
    case FdoGeometryType_Polygon : 
        return PolygonContainsPolygon(poly, (FdoIPolygon*)geom, toleranceXY, strictInside);
    case FdoGeometryType_MultiPoint : 
        {
            FdoIMultiPoint* mp = (FdoIMultiPoint*)geom;

            FdoPtr<FdoIPoint> pt;
            double x, y;
            int dim;

            int numpts = mp->GetCount();

            for (int i=0; i<numpts; i++)
            {
                pt = mp->GetItem(i);

                GET_POSITION(pt, &x, &y, &dim);

                if (!PointInPolygon(poly, x, y, toleranceXY))
                    return false;
            }
        }
        return true;
    case FdoGeometryType_MultiLineString : 
        {
            FdoIMultiLineString* mlines = (FdoIMultiLineString*)geom;

            FdoPtr<FdoILineString> ls;

            int numpolys = mlines->GetCount();

            for (int i=0; i<numpolys; i++)
            {
                ls = mlines->GetItem(i);

                if (!PolygonContainsLineString(poly, ls, toleranceXY))
                    return false;
            }
        }
        return true;
    case FdoGeometryType_MultiPolygon : 
        {
            FdoIMultiPolygon* mpoly = (FdoIMultiPolygon*)geom;

            FdoPtr<FdoIPolygon> p;

            int numpolys = mpoly->GetCount();

            for (int i=0; i<numpolys; i++)
            {
                p = mpoly->GetItem(i);

                if (!PolygonContains(poly, p, toleranceXY, strictInside))
                    return false;
            }
        }
        return true;

    case FdoGeometryType_CurveString : 
    case FdoGeometryType_CurvePolygon : 
    case FdoGeometryType_MultiCurveString :
    case FdoGeometryType_MultiCurvePolygon : 
        {
            FdoPtr<FdoIGeometry> flatgeom = TesselateCurve(geom);
            return PolygonContains(poly, flatgeom, toleranceXY, strictInside);
        }

    default : break;
    }

    throw FdoException::Create(L"Spatial operation not supported");
    return false;

}


bool FdoSpatialUtility::LineStringContains(FdoILineString* line, FdoIGeometry* geom, double toleranceXY)
{
    switch (geom->GetDerivedType())
    {
    case FdoGeometryType_Point: 
        {
            FdoIPoint* pt = (FdoIPoint*) geom;

            double x, y;
            int dim;
            GET_POSITION(pt, &x, &y, &dim);

            return PointOnLine(line, x, y, toleranceXY);
        }
    case FdoGeometryType_LineString : 
        return LineStringContainsLineString(line, (FdoILineString*)geom, toleranceXY);
    case FdoGeometryType_Polygon : 
        return false;
    case FdoGeometryType_MultiPoint : 
        {
            FdoIMultiPoint* mp = (FdoIMultiPoint*) geom;

            int numpts = mp->GetCount();

            FdoPtr<FdoIPoint> pt;
            double x, y;
            int dim;

            for (int i=0; i<numpts; i++)
            {
                pt = mp->GetItem(0);

                GET_POSITION(pt, &x, &y, &dim);

                if (!PointOnLine(line, x, y, toleranceXY))
                    return false;
            }

            return true;
        }
    case FdoGeometryType_MultiLineString : 
        {
            FdoIMultiLineString* mls = (FdoIMultiLineString*)geom;

            int numls = mls->GetCount();

            FdoPtr<FdoILineString> ls;

            for (int i=0; i<numls; i++)
            {
                ls = mls->GetItem(i);

                if (!LineStringContainsLineString(line, ls, toleranceXY))
                    return false;
            }

            return true;
        }
    case FdoGeometryType_MultiPolygon : 
    case FdoGeometryType_CurvePolygon : 
    case FdoGeometryType_MultiCurvePolygon : 
        return false;

    case FdoGeometryType_CurveString : 
    case FdoGeometryType_MultiCurveString :
        {
            FdoPtr<FdoIGeometry> flatgeom = TesselateCurve(geom);
            return LineStringContains(line, flatgeom, toleranceXY);
        }

    default : break;
    }

    throw FdoException::Create(L"Spatial operation not supported");
    return false;
}


bool FdoSpatialUtility::PointContains(FdoIPoint* point, FdoIGeometry* geom, double toleranceXY)
{
    switch (geom->GetDerivedType())
    {
    case FdoGeometryType_Point: 
        {
            FdoIPoint* pt2 = (FdoIPoint*)geom;

            double x1, y1;
            int dim1;
            GET_POSITION(point, &x1, &y1, &dim1);

            double x2, y2;
            int dim2;
            GET_POSITION(pt2, &x2, &y2, &dim2);
           
            return (x1 == x2 && y1 == y2);
        }
    case FdoGeometryType_LineString : 
        return false;
    case FdoGeometryType_Polygon : 
        return false;
    case FdoGeometryType_MultiPoint : 
        {
            FdoIMultiPoint* mpt = (FdoIMultiPoint*)geom;

            double x1, y1;
            int dim1;
            GET_POSITION(point, &x1, &y1, &dim1);

            FdoPtr<FdoIPoint> pt2;
            double x2, y2;
            int dim2;

            int numpts = mpt->GetCount();

            for (int i=0; i<numpts; i++)
            {
                pt2 = mpt->GetItem(i);

                GET_POSITION(pt2, &x2, &y2, &dim2);

                if (x1 != x2 || y1 != y2)
                    return false;
            }
        }
        return true;
    case FdoGeometryType_MultiLineString : 
    case FdoGeometryType_MultiPolygon : 
    case FdoGeometryType_CurveString : 
    case FdoGeometryType_CurvePolygon : 
    case FdoGeometryType_MultiCurveString :
    case FdoGeometryType_MultiCurvePolygon : 
        return false;

    default : break;
    }

    throw FdoException::Create(L"Spatial operation not supported");
    return false;
}


//-----------------------------------------------------------------------------
//
// MULTI CONTAINS
//
//-----------------------------------------------------------------------------

bool FdoSpatialUtility::MultiPolygonContains(FdoIMultiPolygon* mpoly, FdoIGeometry* geom, double toleranceXY)
{
    switch(geom->GetDerivedType())
    {
    case FdoGeometryType_Point: 
        {
            FdoIPoint* pt = (FdoIPoint*)geom;

            double x, y;
            int dim;
            GET_POSITION(pt, &x, &y, &dim);
            
            FdoPtr<FdoIPolygon> poly;

            int numpoly = mpoly->GetCount();

            for (int i=0; i<numpoly; i++)
            {
                poly = mpoly->GetItem(i);

                if (PointInPolygon(poly, x, y, toleranceXY))
                    return true;
            }
        }
        return false;
    case FdoGeometryType_LineString : 
        {
            FdoILineString* line = (FdoILineString*)geom;

            FdoPtr<FdoIPolygon> poly;

            int numpoly = mpoly->GetCount();

            for (int i=0; i<numpoly; i++)
            {
                poly = mpoly->GetItem(i);

                if (PolygonContainsLineString(poly, line, toleranceXY))
                    return true;
            }
        }
        return false;
    case FdoGeometryType_Polygon : 
        {
            FdoIPolygon* poly2 = (FdoIPolygon*)geom;

            FdoPtr<FdoIPolygon> poly;

            int numpoly = mpoly->GetCount();

            for (int i=0; i<numpoly; i++)
            {
                poly = mpoly->GetItem(i);

                if (PolygonContainsPolygon(poly, poly2, toleranceXY))
                    return true;
            }
        }
        return false;
    case FdoGeometryType_MultiPoint : 
        {
            FdoIMultiPoint* mp = (FdoIMultiPoint*) geom;

            FdoPtr<FdoIPoint> pt;

            int numpts = mp->GetCount();

            for (int i=0; i<numpts; i++)
            {
                pt = mp->GetItem(i);

                if (!MultiPolygonContains(mpoly, pt, toleranceXY))
                    return false;
            }
        }
        return true;
    case FdoGeometryType_MultiLineString : 
        {
            FdoIMultiLineString* ml = (FdoIMultiLineString*)geom;

            FdoPtr<FdoILineString> ls;

            int numls = ml->GetCount();

            for (int i=0; i<numls; i++)
            {
                ls = ml->GetItem(i);

                if (!MultiPolygonContains(mpoly, ls, toleranceXY))
                    return false;
            }
        }
        return true;
    case FdoGeometryType_MultiPolygon : 
        {
            FdoIMultiPolygon* mp = (FdoIMultiPolygon*)geom;

            FdoPtr<FdoIPolygon> poly;

            int numpoly = mp->GetCount();

            for (int i=0; i<numpoly; i++)
            {
                poly = mp->GetItem(i);

                if (!MultiPolygonContains(mpoly, poly, toleranceXY))
                    return false;
            }
        }
        return true;

    case FdoGeometryType_CurveString : 
    case FdoGeometryType_CurvePolygon : 
    case FdoGeometryType_MultiCurveString :
    case FdoGeometryType_MultiCurvePolygon : 
        {
            FdoPtr<FdoIGeometry> flatgeom = TesselateCurve(geom);
            return MultiPolygonContains(mpoly, flatgeom, toleranceXY);
        }

    default : break;
    }

    throw FdoException::Create(L"Spatial operation not supported");
    return false;

}


bool FdoSpatialUtility::MultiLineStringContains(FdoIMultiLineString* mline, FdoIGeometry* geom, double toleranceXY)
{
    switch (geom->GetDerivedType())
    {
    case FdoGeometryType_Point: 
        {
            FdoIPoint* pt = (FdoIPoint*)geom;
            
            FdoPtr<FdoILineString> ls;

            int numls = mline->GetCount();

            for (int i=0; i<numls; i++)
            {
                ls = mline->GetItem(i);

                if (LineStringContains(ls, pt, toleranceXY))
                    return true;
            }
        }
        return false;
    case FdoGeometryType_LineString : 
        {
            FdoILineString* line = (FdoILineString*)geom;
            
            FdoPtr<FdoILineString> ls;

            int numls = mline->GetCount();

            for (int i=0; i<numls; i++)
            {
                ls = mline->GetItem(i);

                if (LineStringContainsLineString(ls, line, toleranceXY))
                    return true;
            }
        }
        return false;
    case FdoGeometryType_Polygon : 
        return false;
    case FdoGeometryType_MultiPoint : 
        {
            FdoIMultiPoint* mp = (FdoIMultiPoint*) geom;

            FdoPtr<FdoIPoint> pt;

            int numpts = mp->GetCount();

            for (int i=0; i<numpts; i++)
            {
                pt = mp->GetItem(i);

                if (!MultiLineStringContains(mline, pt, toleranceXY))
                    return false;
            }
        }
        return true;
    case FdoGeometryType_MultiLineString : 
        {
            FdoIMultiLineString* ml = (FdoIMultiLineString*)geom;

            FdoPtr<FdoILineString> ls;

            int numls = ml->GetCount();

            for (int i=0; i<numls; i++)
            {
                ls = ml->GetItem(i);

                if (!MultiLineStringContains(mline, ls, toleranceXY))
                    return false;
            }
        }
        return true;
    case FdoGeometryType_MultiPolygon : 
        return false;

    case FdoGeometryType_CurveString : 
    case FdoGeometryType_MultiCurveString :
        {
            FdoPtr<FdoIGeometry> flatgeom = TesselateCurve(geom);
            return MultiLineStringContains(mline, flatgeom, toleranceXY);
        }

    case FdoGeometryType_CurvePolygon : 
    case FdoGeometryType_MultiCurvePolygon : 
        return false;

    default : break;
    }

    throw FdoException::Create(L"Spatial operation not supported");
    return false;
}

bool FdoSpatialUtility::MultiPointContains(FdoIMultiPoint* mpoint, FdoIGeometry* geom, double toleranceXY)
{
    switch (geom->GetDerivedType())
    {
    case FdoGeometryType_Point: 
        {
            int numpts = mpoint->GetCount();

            FdoPtr<FdoIPoint> pt;

            double x0, y0;
            int dim0;
            GET_POSITION((FdoIPoint*)geom, &x0, &y0, &dim0);

            double x1, y1;
            int dim1;

            for (int i=0; i<numpts; i++)
            {
                pt = mpoint->GetItem(i);

                GET_POSITION(pt, &x1, &y1, &dim1);

                if (x0 == x1 && y0 == y1)
                    return true;
            }
            return false;
        }
    case FdoGeometryType_LineString : 
        return false;
    case FdoGeometryType_Polygon : 
        return false;
    case FdoGeometryType_MultiPoint : 
        {
            FdoIMultiPoint* mp2 = (FdoIMultiPoint*)geom;

            FdoPtr<FdoIPoint> pt2;

            int numpts2 = mp2->GetCount();

            for (int i=0; i<numpts2; i++)
            {
                pt2 = mp2->GetItem(i);

                if (!MultiPointContains(mpoint, pt2, toleranceXY))
                    return false;
            }
        }
        return true;
    case FdoGeometryType_MultiLineString : 
    case FdoGeometryType_MultiPolygon : 
    case FdoGeometryType_CurveString : 
    case FdoGeometryType_MultiCurveString :
    case FdoGeometryType_CurvePolygon : 
    case FdoGeometryType_MultiCurvePolygon : 
        return false;

    default : break;
    }


    throw FdoException::Create(L"Spatial operation not supported");
    return false;
}


//-----------------------------------------------------------------------------
//
// ACTUAL CODE THAT DOES STUFF
//
//-----------------------------------------------------------------------------

bool FdoSpatialUtility::LineStringsIntersect(FdoILineString* line1, FdoILineString* line2, double toleranceXY)
{
    int numpts1 = line1->GetCount();
    int numpts2 = line2->GetCount();

    double pts[12];

    double x0, y0;
    double x1, y1;
    double x2, y2;
    double x3, y3;
    int dim_line1, dim_line2;

    GET_ITEM(line1, 0, &x0, &y0, &dim_line1);

    for (int i=1; i<numpts1; i++)
    {

        GET_ITEM(line1, i, &x1, &y1, &dim_line1);

        pts[0] = x0;
        pts[1] = y0;
        pts[2] = x1;
        pts[3] = y1;

        GET_ITEM(line2, 0, &x2, &y2, &dim_line2);

        for (int j=1; j<numpts2; j++)
        {
            GET_ITEM(line2, j, &x3, &y3, &dim_line2);

            pts[4] = x2;
            pts[5] = y2;
            pts[6] = x3;
            pts[7] = y3;

            if (line_segment_intersect(pts, pts+4, pts+8, toleranceXY) > 0)
                return true;

            x2 = x3;
            y2 = y3;
        }

        x0 = x1;
        y0 = y1;
    }

    return false;
}


bool FdoSpatialUtility::PolygonsIntersect(FdoIPolygon* poly1, FdoIPolygon* poly2, double toleranceXY)
{

    FdoPtr<FdoILinearRing> ring1;
    FdoPtr<FdoILinearRing> ring2;
    double x0, y0;
    double x1, y1;
    double x2, y2;
    double x3, y3;
    int dim_ring1, dim_ring2;
    
    double pts[12];

    int numContours1 = poly1->GetInteriorRingCount() + 1;
    int numContours2 = poly2->GetInteriorRingCount() + 1;

    //Here is the algorithm we are using here:
    //1. check if any points of p1 are inside p2
    //2. check if any points of p2 are inside p1
    //3. do pairwise edge intersection
    
    //1.
    for (int i=0; i<numContours1; i++)
    {
        if (i==0)
            ring1 = poly1->GetExteriorRing();
        else
            ring1 = poly1->GetInteriorRing(i-1);

        int numverts1 = ring1->GetCount();

        for (int j=0; j<numverts1-1; j++)
        {
            GET_ITEM(ring1, j, &x0, &y0, &dim_ring1);

            if (PointInPolygon(poly2, x0, y0, toleranceXY))
                return true;
        }
    }

    //2.
    for (int i=0; i<numContours2; i++)
    {
        if (i==0)
            ring2 = poly2->GetExteriorRing();
        else
            ring2 = poly2->GetInteriorRing(i-1);

        int numverts2 = ring2->GetCount();

        for (int j=0; j<numverts2-1; j++)
        {
            GET_ITEM(ring2, j, &x0, &y0, &dim_ring2);

            if (PointInPolygon(poly1, x0, y0, toleranceXY))
                return true;
        }
    }

    //3.
    for (int i=0; i<numContours1; i++)
    {
        if (i==0)
            ring1 = poly1->GetExteriorRing();
        else
            ring1 = poly1->GetInteriorRing(i-1);

        int numverts1 = ring1->GetCount();

        for (int j=0; j<numverts1-1; j++)
        {
            GET_ITEM(ring1, j, &x0, &y0, &dim_ring1);
            GET_ITEM(ring1, j+1, &x1, &y1, &dim_ring1);

            pts[0] = x0;
            pts[1] = y0;
            pts[2] = x1;
            pts[3] = y1;
            
            for (int k=0; k<numContours2; k++)
            {
                if (k==0)
                    ring2 = poly2->GetExteriorRing();
                else
                    ring2 = poly2->GetInteriorRing(k-1);

                int numverts2 = ring2->GetCount();

                for (int l=0; l<numverts2-1; l++)
                {
                    GET_ITEM(ring2, l, &x2, &y2, &dim_ring2);
                    GET_ITEM(ring2, l+1, &x3, &y3, &dim_ring2);
                    
                    pts[4] = x2;
                    pts[5] = y2;
                    pts[6] = x3;
                    pts[7] = y3;

                    if (line_segment_intersect(pts, pts+4, pts+8, toleranceXY) > 0)
                        return true;
                }
            }
        }
    }

    return false;
}

// Change the value for performance tests. 
#define CHECK_JUST_ONE_POINT_INSIDE_POLY true

bool FdoSpatialUtility::PolygonContainsLineString(FdoIPolygon* poly, FdoILineString* line, double toleranceXY, bool strictInside)
{
    //  CHECK_JUST_ONE_POINT_INSIDE_POLY=false
    //Here is the algorithm we are using here:
    //1. check if all points of line are inside poly 
    //2. if #1 succeeds do pairwise edge intersection for all the rings.

    //  CHECK_JUST_ONE_POINT_INSIDE_POLY=true
    // Changed for performance:
    //1. check if at least one point of line is inside polygon
    //2. if #1 succeeds do pairwise edge intersection for all the rings (segment by segment)
    //3. if intersections found check both end points of the segment to be inside polygon
    // Note: if no itersection for a segment is found we don't know whether it's 
    // completly inside or completly outside polygon. But will be at least one other segment 
    // that will intersect the polygon. But we know from 1. that at least one point is inside.

    FdoPtr<FdoILinearRing> ring1;
    double  x0, y0;
    double  x1, y1;
    double  x2, y2;
    double  x3, y3;
    int     dim_ring1, dim_line;
    bool    pt_on_ext;
    bool    pt_on_int;
    bool    pt1_on_ring;
    bool    pt2_on_ring;
    bool    pt_inside = true;

    double pts[12];

    int numContours = poly->GetInteriorRingCount() + 1;
    int numLinePts = line->GetCount();
   
    //1.
    for (int j = 0; j < numLinePts && pt_inside; j++)
    {
        GET_ITEM(line, j, &x0, &y0, &dim_line);

        // This checks the interior rings as well
        pt_inside = PointInPolygon( poly, x0, y0, toleranceXY, &pt_on_ext, &pt_on_int);

        // If the check is strict...
        if ( pt_inside && strictInside && ( pt_on_ext || pt_on_int ) )
            pt_inside = false;

        if ( pt_inside && CHECK_JUST_ONE_POINT_INSIDE_POLY )
            break;
    }

    // Not all the points are inside. The line is not contained.
    if ( !pt_inside )
        return false;

    //2.
    for (int i = 0; i < numContours; i++)
    {
        bool    is_ext_ring = ( i == 0 );

        ring1 = is_ext_ring ? poly->GetExteriorRing() : poly->GetInteriorRing(i-1);

        int numverts1 = ring1->GetCount();

        for (int j=0; j<numverts1-1; j++)
        {
            GET_ITEM(ring1, j, &x0, &y0, &dim_ring1);
            GET_ITEM(ring1, j+1, &x1, &y1, &dim_ring1);

            pts[0] = x0;
            pts[1] = y0;
            pts[2] = x1;
            pts[3] = y1;
            
            GET_ITEM(line, 0, &x2, &y2, &dim_line);

            for (int k=1; k<numLinePts; k++)
            {
                GET_ITEM(line, k, &x3, &y3, &dim_line);

                pts[4] = x2;
                pts[5] = y2;
                pts[6] = x3;
                pts[7] = y3;

                int numIntersections = line_segment_intersect(pts, pts+4, pts+8, toleranceXY);

                // The line segments found intersecting?
                if ( numIntersections > 0 && strictInside )
                    return false;

                if ( CHECK_JUST_ONE_POINT_INSIDE_POLY )
                {
                    if ( numIntersections == 1 )
                    {
                        // Both end points should be inside polygon
                        pt_inside = PointInRing2( ring1, is_ext_ring, strictInside, x2, y2, &pt1_on_ring, toleranceXY );

                        if ( pt_inside )
                            pt_inside = PointInRing2( ring1, is_ext_ring, strictInside, x3, y3, &pt2_on_ring, toleranceXY );

                        // Ambiguous case: line segment with both end points of the ring.
                        // Decide by checking the midpoint 
                        if ( pt_inside && pt1_on_ring && pt2_on_ring )
                        {
                            pt_inside = PointInRing2( ring1, is_ext_ring, strictInside, 
                                                        (x2+x3)/2.0, (y2+y3)/2.0, &pt2_on_ring, toleranceXY );  

                            // Imagine a "S" shape polygon: (start, mid, end) points are on the
                            // boundary. This makes the segment "outside".
                            if ( pt_inside && pt2_on_ring )
                                pt_inside = false;
                        }

                        return pt_inside;
                    }
                    else if ( numIntersections == 2 && !is_ext_ring )
                    {
                        // Collinear vertices, acceptable only on the external boundary
                        return false;
                    }
                }
                x2 = x3;
                y2 = y3;
            }
        }
    }

    return true;
}

bool FdoSpatialUtility::PolygonContainsPolygon(FdoIPolygon* poly1, FdoIPolygon* poly2, double toleranceXY, bool strictInside)
{
    bool    poly2_inside = true;
    int     numContours2 = poly2->GetInteriorRingCount() + 1;
    
    FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
        
    // All the linestrings of poly2 need to be inside poly1 (they may touch poly2,
    // depending on strictInside flag).

    for ( int i = 0; i < numContours2 && poly2_inside; i++)
    {
        FdoPtr<FdoILinearRing> ring2 = (i == 0) ? poly2->GetExteriorRing() : poly2->GetInteriorRing(i-1); 

        FdoPtr<FdoDirectPositionCollection> pos2 = ring2->GetPositions();
        FdoPtr<FdoILineString> line2 = gf->CreateLineString( pos2 );

        poly2_inside = PolygonContainsLineString( poly1, line2, toleranceXY, strictInside );
    }

    return poly2_inside;
}

bool FdoSpatialUtility::PolygonIntersectsLineString(FdoIPolygon* poly, FdoILineString* line, double toleranceXY)
{
    //Here is the algorithm we are using here:
    //1. check if any points of line are inside poly
    //2. if #1 fails, do pairwise edge intersection
    FdoPtr<FdoILinearRing> ring1;
    double x0, y0;
    double x1, y1;
    double x2, y2;
    double x3, y3;
    int dim_ring1, dim_line;

    double pts[12];

    int numContours = poly->GetInteriorRingCount() + 1;
    int numLinePts = line->GetCount();
   
    //1.
    for (int j=0; j<numLinePts; j++)
    {
        GET_ITEM(line, j, &x0, &y0, &dim_line);

        if (PointInPolygon(poly, x0, y0, toleranceXY))
            return true;
    }


    //2.
    for (int i=0; i<numContours; i++)
    {
        if (i==0)
            ring1 = poly->GetExteriorRing();
        else
            ring1 = poly->GetInteriorRing(i-1);

        int numverts1 = ring1->GetCount();

        for (int j=0; j<numverts1-1; j++)
        {
            GET_ITEM(ring1, j, &x0, &y0, &dim_ring1);
            GET_ITEM(ring1, j+1, &x1, &y1, &dim_ring1);

            pts[0] = x0;
            pts[1] = y0;
            pts[2] = x1;
            pts[3] = y1;
            
            GET_ITEM(line, 0, &x2, &y2, &dim_line);

            for (int k=1; k<numLinePts; k++)
            {
                GET_ITEM(line, k, &x3, &y3, &dim_line);

                pts[4] = x2;
                pts[5] = y2;
                pts[6] = x3;
                pts[7] = y3;

                if (line_segment_intersect(pts, pts+4, pts+8, toleranceXY) > 0)
                    return true;

                x2 = x3;
                y2 = y3;
            }
        }
    }

    return false;
}


bool FdoSpatialUtility::LineStringContainsLineString(FdoILineString* line1, FdoILineString* line2, double toleranceXY)
{
    //does line1 contain line2?
    
    int numpts1 = line1->GetCount();
    int numpts2 = line2->GetCount();
    
    double x0, y0;
    double x1, y1;
    double x2, y2;
    double x3, y3;
    int dim_line1, dim_line2;

    double pts[8];

    GET_ITEM(line2, 0, &x2, &y2, &dim_line2);
    
    //for each segment of line2
    for (int i=1; i<numpts2; i++)
    {
        GET_ITEM(line2, i, &x3, &y3, &dim_line2);

        pts[4] = x2;
        pts[5] = y2;
        pts[6] = x3;
        pts[7] = y3;

        GET_ITEM(line1, 0, &x0, &y0, &dim_line1);

        bool flag = false;

        //check if the current segment of line2 is
        //inside one of the segments of line1
        for (int j=1; j<numpts1; j++)
        {
            GET_ITEM(line1, j, &x1, &y1, &dim_line1);

            pts[0] = x0;
            pts[1] = y0;
            pts[2] = x1;
            pts[3] = y1;

            if (line_contains_line(pts, pts+4, toleranceXY))
            {
                flag = true;
                break;
            }

            x0 = x1;
            y0 = y1;
        }

        if (!flag)
            return false;

        x2 = x3;
        y2 = y3;
    }

    return true;
}



bool FdoSpatialUtility::PointOnLine(FdoILineString* line, double x, double y, double toleranceXY)
{
    double pts[12];

    pts[0] = x;
    pts[1] = y;
    pts[2] = x;
    pts[3] = y;

    int numpts = line->GetCount();

    double x0, y0;
    int dim_line;
    GET_ITEM(line, 0, &x0, &y0, &dim_line);

    double x1, y1;

    for (int i=1; i<numpts; i++)
    {
        GET_ITEM(line, i, &x1, &y1, &dim_line);

        pts[4] = x0;
        pts[5] = y0;
        pts[6] = x1;
        pts[7] = y1;

        if (line_segment_intersect(pts, pts+4, pts+8, toleranceXY) > 0)
            return true;
       
        x0 = x1;
        y0 = y1;
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////
//
// This is a point-in-loop test.  If `strict' is true, no on-edge conditions are 
// allowed.
//
////////////////////////////////////////////////////////////////////////////////

bool FdoSpatialUtility::point_in_ring(
    FdoILinearRing* ring, 
    double          x, 
    double          y, 
    bool            strict, 
    bool*           isOnBoundary,
    double          toleranceXY)
{
    int                      loop_p2_i;
    double                   pt1x, pt1y;             /* current pts in loop  */
    double                   pt2x, pt2y;             /* current pts in loop  */
    bool                     disqualified;
    int                      cross_count;
    bool                     isOn;                  /* Pt on loop?          */
    bool                     isIn;                  /* Point inside loop?   */
    bool                     isLeft;
    bool                     isAtLinePt1;
    bool                     isAtLinePt2;
    int                      dim_ring;
    int                      num_pts = ring->GetCount();;
    FdoPtr<FdoIEnvelope>     extent = ring->GetEnvelope();
  
    isIn = false;
 
    if( outcode( x, y, extent, toleranceXY ) == 0 ) {
 
        /* `cross_count' counts how many times a horizontal ray cast to the
         * right from the test point would cross loop.
         */
 
        // get the 1st point in the contour
        GET_ITEM( ring, 0, &pt1x, &pt1y, &dim_ring);
 
        loop_p2_i = 1;
        cross_count = 0;
        disqualified = false;
 
        while( !disqualified && (loop_p2_i < num_pts) ) {
            double dx, dy, len, lenSq;
 
            GET_ITEM( ring, loop_p2_i++, &pt2x, &pt2y, &dim_ring);

            dx = pt2x - pt1x;
            dy = pt2y - pt1y;

            lenSq = dx * dx + dy * dy;
            len = sqrt(lenSq);
 
            (void) PtIsOnOrLeftOfLineTol(
                            pt1x, pt1y, pt2x, pt2y,
                            dx, dy, len, lenSq,
                            x, y,
                            toleranceXY,
                            &isOn, &isLeft, &isAtLinePt1, &isAtLinePt2 );

            if ( isOn && isOnBoundary )
                *isOnBoundary = true;

            if ( isOn && strict ) {
                isIn = false;
                disqualified = true;
            } else if ( isLeft ) {
                cross_count++;
            } else if ( isOn && !strict ) {
                break;
            }
            pt1x = pt2x;
            pt1y = pt2y;
       }
       if( !disqualified ) {
            isIn = ( (cross_count % 2) == 1 );
            if ( !strict && isOn )
                isIn = true;
       }
    }
  
    return ( isIn );
}

///////////////////////////////////////////////////////////////////////////////
//
//	Determine  the	outcode  for  a  point	in	relation to a rectangular window,  
//  as  defined  in the Sutherland-Cohen algorithm.															*
///////////////////////////////////////////////////////////////////////////////
#define SU_ABOVE        0001    /* Define octal bit patterns */
#define SU_BELOW        0002
#define SU_RIGHT        0004
#define SU_LEFT         0010
#define SU_COLLINEAR    0020
#define SU_INSIDE       0040
#define SU_OUTSIDE      0100

int FdoSpatialUtility::outcode( 
    double			x,		
    double			y,		
    FdoIEnvelope*   extent,
    double          toleranceXY
)
{
	int 	outcode = 0;	/* outcode to be returned	*/

	if ( extent )
    { 		
	    if ((extent->GetMaxY() - y) < -toleranceXY) outcode |= SU_ABOVE;
	    if ((y - extent->GetMinY()) < -toleranceXY) outcode |= SU_BELOW;
	    if ((extent->GetMaxX() - x) < -toleranceXY) outcode |= SU_RIGHT;
	    if ((x - extent->GetMinX()) < -toleranceXY) outcode |= SU_LEFT;
    }
	return (outcode);
}

int FdoSpatialUtility::outcode2( 
    double			x,		
    double			y,		
    double          xmin,
    double          ymin,
    double          xmax,
    double          ymax,
    double          toleranceXY
)
{
	int 	outcode = 0;	/* outcode to be returned	*/

	if ((ymax - y) < -toleranceXY) outcode |= SU_ABOVE;
	if ((y - ymin) < -toleranceXY) outcode |= SU_BELOW;
	if ((xmax - x) < -toleranceXY) outcode |= SU_RIGHT;
	if ((x - xmin) < -toleranceXY) outcode |= SU_LEFT;

    return (outcode);
}

///////////////////////////////////////////////////////////////////////////////
//
//		Given two extents and a tolerance, return a flag indicating 	
//		how (if) they overlap in the  X direction and a second flag for 
//		the Y direction.												
///////////////////////////////////////////////////////////////////////////////
bool FdoSpatialUtility::check_xtnt_tol (
    double          xmin1,
    double          ymin1,
    double          xmax1,
    double          ymax1,
    double          xmin2,
    double          ymin2,
    double          xmax2,
    double          ymax2,
    int 			*xflag,
    int 			*yflag,
    double		   	tol
)
{
    bool   overlap = true;

	*xflag = ( ( xmin1 > xmin2 - tol ) && ( xmax1 < xmax2 + tol ) );
	*yflag = ( ( ymin1 > ymin2 - tol ) && ( ymax1 < ymax2 + tol ) );

	/*
	 * Return TRUE if both flags set ( guaranteed intersection ).
	 * Otherwise check 4 possible reject cases.
	 */
	if ( !(*xflag) || !(*yflag) ) 
    {
        if (    ( xmin1 >= xmax2 + tol ) ||
				( xmax1 <= xmin2 - tol ) ||
				( ymin1 >= ymax2 + tol ) ||
				( ymax1 <= ymin2 - tol ) ) 
        {
		    overlap = false;
        }
	}

	return (overlap);
}

//////////////////////////////////////////////////////////////////////////////////
//
// Returns a boolean 'point is on the left side of the line segment'
///////////////////////////////////////////////////////////////////////////////////
bool FdoSpatialUtility::PtIsOnOrLeftOfLineTol(
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
	bool *		IsAtLinePt2_O )
{
	bool		IsOnOrLeft = false;

	*IsOnLine_O = false;
	*IsLeftOfLine_O = false;
	*IsAtLinePt1_O = false;
	*IsAtLinePt2_O = false;

	if ( LineY1 > LineY2 ) {
		/* Swap points -- need y1 < y2 for correct "left" orientation. */
		double swapper;
		swapper = LineX1;
		LineX1 = LineX2;
		LineX2 = swapper;
		Dx = -Dx;
		swapper = LineY1;
		LineY1 = LineY2;
		LineY2 = swapper;
		Dy = -Dy;
        // Swapped line endpoints
	}

	/* If point is same as line's first point... */
	if ( fabs(PtX-LineX1) <= Tol && fabs(PtY-LineY1) <= Tol ) {

		*IsOnLine_O = true;
		*IsAtLinePt1_O = true;
        // same as line's first point
        
	/* Else if point is same as line's second point... */
	} else if ( fabs(PtX-LineX2) <= Tol && fabs(PtY-LineY2) <= Tol ) {

		*IsOnLine_O = true;
		*IsAtLinePt2_O = true;
		// same as line's second point;

	/* Else use point-to-line calculations. */
	} else {

		double		Numerator1	= ((LineY1-PtY) * -Dy) - ((LineX1-PtX) * Dx);
		double		r			= Numerator1 / LenSq;

		/* If projected point is somewhere on the second line... */
		if ( r > 0.0 && r < 1.0 ) {

			double	Numerator2	 = ((LineY1-PtY) * Dx) - ((LineX1-PtX) * Dy);
			double	PtToLineDist = Numerator2 / Len;

			/* If the given point is on the second line... */
			if ( fabs(PtToLineDist) <= Tol ) {

				*IsOnLine_O = true;
				// On line with r
			}
		} else {
			// Off line with r
		}

		/* If Point is not on line, find out if it's on the left */
        if (!(*IsOnLine_O))
        {
            bool     lineIsHorizontal = ( LineY1 == LineY2 ); /* No Tol! */

            if ( !lineIsHorizontal )	/* horiz would have r in (0,1) */
            {
                double      minY = min(LineY1, LineY2);
                double      maxY = max(LineY1, LineY2);

                if ((PtY <= maxY) && (PtY > minY))  /* Skip minY's to avoid double-counting contiguous line segments */
                {
                    double Numerator2 = ((LineY1-PtY) * Dx) - ((LineX1-PtX) * Dy);
                    double s	      = Numerator2 / LenSq;

                    if ( s < 0.0 ) {
                        *IsLeftOfLine_O = true;
						// Left of line with s
					} else {
						// Not left of line with s
					}
				} else {
					// Trivial reject by Y extent
                }
			} else {
				// Trivial reject by horizontal line
            }
        }
	}

	IsOnOrLeft = *IsOnLine_O || *IsLeftOfLine_O;

	return( IsOnOrLeft );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
#define USE_point_in_ring false  // apparently point_in_ring() variant is slower by aprox. 50%
bool FdoSpatialUtility::PointInPolygon(FdoIPolygon* poly, double x, double y, bool* isOnExtBoundary, bool* isOnIntBoundary)
{
    return PointInPolygon(poly, x, y, EPSILON, isOnExtBoundary, isOnIntBoundary);
}

bool FdoSpatialUtility::PointInPolygon(FdoIPolygon* poly, double x, double y, double toleranceXY, bool* isOnExtBoundary, bool* isOnIntBoundary)
{
    if (toleranceXY <= 0.0)
        toleranceXY = EPSILON;
    int numContours = poly->GetInteriorRingCount() + 1;

    bool ext_inside_flag = false;
    bool int_inside_flag = false;
    bool on_ext_boundary = false;
    bool on_int_boundary = false;
    bool on_ring_flag    = false;

    FdoPtr<FdoILinearRing> ring = poly->GetExteriorRing();
    
    if ( USE_point_in_ring )
        ext_inside_flag = point_in_ring( ring, x, y, false, &on_ext_boundary, toleranceXY );
    else
        ext_inside_flag = PointInRing( ring, x, y, toleranceXY, &on_ext_boundary );

    // The point should be inside the exterior ring and outside any interior ring
    if ( ext_inside_flag )
    {
        for (int i = 0; i < poly->GetInteriorRingCount() && !int_inside_flag; i++)
        {
            FdoPtr<FdoILinearRing> ring = poly->GetInteriorRing(i);
    
            if ( USE_point_in_ring )
                int_inside_flag = point_in_ring( ring, x, y, true, &on_ring_flag, toleranceXY );
            else
                int_inside_flag = PointInRing( ring, x, y, toleranceXY, &on_ring_flag );

            if ( on_ring_flag )
                on_int_boundary = true;
        }
    }

    if ( isOnExtBoundary != NULL )
        *isOnExtBoundary = on_ext_boundary;

    if ( isOnIntBoundary != NULL )
        *isOnIntBoundary = on_int_boundary;

    return ext_inside_flag && !int_inside_flag;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
bool FdoSpatialUtility::PointInRing(FdoILinearRing* ring, double x, double y, bool* isOnBoundary)
{
    return PointInRing(ring, x, y, EPSILON, isOnBoundary);
}

bool FdoSpatialUtility::PointInRing(FdoILinearRing* ring, double x, double y, double toleranceXY, bool* isOnBoundary)
{
    if (toleranceXY <= 0.0)
        toleranceXY = EPSILON;
    bool yflag0, yflag1;
    double vtx0X, vtx0Y, vtx1X, vtx1Y;
    int dim_ring;

    bool inside_flag = false;
    int count = 0;
   
    int numverts = ring->GetCount();

    if ( isOnBoundary != NULL )
        *isOnBoundary = false;

    // get the last point in the contour
    GET_ITEM(ring, numverts-1, &vtx0X, &vtx0Y, &dim_ring);

    // get test bit for above/below X axis
    yflag0 = (vtx0Y >= y);

    for (int j = 0; j < numverts; j++)
    {
        GET_ITEM(ring, j, &vtx1X, &vtx1Y, &dim_ring);

        // Check if the point is on the vertex. If so, the test is done.
        if ( j != 0 && outcode2( x, y, 
                                 min(vtx0X, vtx1X), min(vtx0Y, vtx1Y),
                                 max(vtx0X, vtx1X), max(vtx0Y, vtx1Y), toleranceXY) == 0 )
        {
            bool  isOn = pt_is_on_line2( vtx0X, vtx0Y, vtx1X, vtx1Y, x, y, toleranceXY );

            if ( isOnBoundary != NULL && isOn )
                *isOnBoundary = true;

            if ( isOn )
                return true;
        }
            
        yflag1 = (vtx1Y >= y);

        // Check if endpoints straddle (are on opposite sides) of X axis
        // (i.e. the Y's differ); if so, +X ray could intersect this edge.
        // The old test also checked whether the endpoints are both to the
        // right or to the left of the test point.  However, given the faster
        // intersection point computation used below, this test was found to
        // be a break-even proposition for most polygons and a loser for
        // triangles (where 50% or more of the edges which survive this test
        // will cross quadrants and so have to have the X intersection computed
        // anyway).  I credit Joseph Samosky with inspiring me to try dropping
        // the "both left or both right" part of my code.
        if (yflag0 != yflag1)
        {
            // Check intersection of pgon segment with +X ray.
            // Note if >= point's X; if so, the ray hits it.
            // The division operation is avoided for the ">=" test by checking
            // the sign of the first vertex wrto the test point; idea inspired
            // by Joseph Samosky's and Mark Haigh-Hutchinson's different
            // polygon inclusion tests.
            if (((vtx1Y-y)*(vtx0X-vtx1X) >=
                (vtx1X-x)*(vtx0Y-vtx1Y)) == yflag1)
            {
                inside_flag = !inside_flag;
            }
        }

        // move to the next pair of vertices, retaining info as possible
        yflag0 = yflag1;
        vtx0X = vtx1X;
        vtx0Y = vtx1Y;
    }

    return inside_flag;
}

////////////////////////////////////////////////////////////////////////////////
// Wrapper over PointInRing()
//
bool FdoSpatialUtility::PointInRing2( FdoILinearRing* ring, bool exteriorRing, bool strictInside, double x, double y, bool* pt_on_ring, double toleranceXY )
{
    bool    pt_inside_ring;

    pt_inside_ring = PointInRing( ring, x, y, toleranceXY, pt_on_ring );

    // If point is within an interior ring (or on its boundary), then it is considered to be
    // strictly outside the poly, and this test fails:
    if ( pt_inside_ring && !exteriorRing )
        pt_inside_ring = false;

    // If point is *on* the ring boundary and caller requires point to be strictly inside the ring, then this test fails:
    if ( *pt_on_ring && strictInside )
        pt_inside_ring = false;

    return pt_inside_ring;
 }

//
// The following code is cloned from FdoSpatialUtility::PointInRing(). 
// (Refer the original method for extensive commnents on the altgorithm)
// Optimized it for speed by skipping the "on boundary" test because 
// a) it is expensive and b) we assume the chosen point is not on the ring.

bool FdoSpatialUtility::PointInRingFast(FdoILinearRing* ring, double x, double y, double toleranceXY)
{
    bool yflag0, yflag1;
    double vtx0X, vtx0Y, vtx1X, vtx1Y;
    int dummydim;

    bool isInside = false;
    int count = 0;
   
    int numverts = ring->GetCount();

    // get the last point in the contour
    GET_ITEM(ring, numverts-1, &vtx0X, &vtx0Y, &dummydim);

    // get test bit for above/below X axis
    yflag0 = (vtx0Y >= y);

    for (int j = 0; j < numverts; j++)
    {
        GET_ITEM(ring, j, &vtx1X, &vtx1Y, &dummydim);
       
        yflag1 = (vtx1Y >= y);

        if (yflag0 != yflag1)
        {
            if (((vtx1Y-y)*(vtx0X-vtx1X) >=
                (vtx1X-x)*(vtx0Y-vtx1Y)) == yflag1)
            {
                isInside  = !isInside;
            }
        }

        // move to the next pair of vertices, retaining info as possible
        yflag0 = yflag1;
        vtx0X = vtx1X;
        vtx0Y = vtx1Y;
    }

    return isInside;
} 

///////////////////////////////////////////////////////////////////////////////
FdoIGeometry* FdoSpatialUtility::TesselateCurve(FdoIGeometry* curve)
{
    FdoPtr<FdoIEnvelope> env = curve->GetEnvelope();

    FdoPtr<FdoFgfGeometryFactory> factory = FdoFgfGeometryFactory::GetInstance();

    double maxdim = max(env->GetMaxX() - env->GetMinX(), env->GetMaxY() - env->GetMinY());
    
    return FdoSpatialUtility::ApproximateGeometryWithLineStrings(
        curve, maxdim, maxdim * CURVE_APPROX_FACTOR, factory);
}

#define SIGN_BIT(d) *((int*)&d+1) & 0x80000000;

static inline void PositionStreamCheckAndAdvance(
    FdoInt32 positionCount,
    FdoInt32 ordinatesPerPosition,
    FdoInt32 &byteCount)
{
    FdoInt32 bytesForPositions = positionCount * ordinatesPerPosition * sizeof(double);
    if (byteCount < bytesForPositions)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_15_UNSUPPORTEDGEOMETRYDATA)));
    byteCount -= bytesForPositions;
}

static inline void IntegerStreamSizeCheck(
    FdoInt32 *&ireader,
    FdoInt32 byteCount)
{
    // Note:  force signed comparison, in case byteCount becomes negative.
    if (byteCount < (FdoInt32) sizeof(FdoInt32))
    {
        FdoException * ex = FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_15_UNSUPPORTEDGEOMETRYDATA)));
        throw ex;
    }
}

static inline FdoInt32 IntegerFromStream(
    FdoInt32 *&ireader,
    FdoInt32 &byteCount)
{
    IntegerStreamSizeCheck(ireader, byteCount);
    FdoInt32 value = *ireader++;
    byteCount -= sizeof(FdoInt32);
    return value;
}

static bool MightHave3dArcs(unsigned char * fgf, FdoInt32 byteCount)
{
    // Decide whether the FGF geometry should be redirected to newer
    // (but possibly slower) code that computes arc extents in 3D.

    // This is a quick, non-trustworthy test that only checks
    // the first sub-geometry of any aggregate types.  Mixed dimensionality
    // is invalid anyway -- we just do not specifically test for it here.

    bool mightHave3dArcs = true;

    int* ireader = (int*)fgf;

    int geom_type = IntegerFromStream(ireader, byteCount);

    FdoDimensionality dim;

    switch (geom_type)
    {
    case FdoGeometryType_MultiLineString :
    case FdoGeometryType_MultiPolygon :
    case FdoGeometryType_MultiPoint :
    case FdoGeometryType_LineString :
    case FdoGeometryType_Polygon :
    case FdoGeometryType_Point :
    case FdoGeometryType_MultiGeometry :        // TODO: support MultiGeometry in newer 3D arc code
        mightHave3dArcs = false;                // No arcs in these types.
        break;

	case FdoGeometryType_CurveString:
	case FdoGeometryType_CurvePolygon:
        IntegerStreamSizeCheck(ireader, byteCount);
        dim = (FdoDimensionality) *ireader++;   // Check dimensionality.
        // Don't bother updating byteCount -- we are done!
        if (dim == FdoDimensionality_XY)
            mightHave3dArcs = false;            // No 3D in these types.
        break;

	case FdoGeometryType_MultiCurveString:
	case FdoGeometryType_MultiCurvePolygon:
        if (byteCount < ((FdoInt32)(3*sizeof(*ireader))))  // About to read 3 integers.
            throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_15_UNSUPPORTEDGEOMETRYDATA)));
        ireader++;                              // Skip subgeometry counter.
        ireader++;                              // Skip subgeometry type.
        dim = (FdoDimensionality) *ireader++;   // Check dimensionality of first subgeometry.
        // Don't bother updating byteCount -- we are done!
        if (dim == FdoDimensionality_XY)
            mightHave3dArcs = false;            // No 3D in these types.
        break;

    default:
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_10_UNSUPPORTEDGEOMETRYTYPE)));
    }

    return mightHave3dArcs;
}

static bool Is2d(unsigned char * fgf, FdoInt32 byteCount)
{
    // Test whether the geometry is two-dimensional.
    // It is specifically to decide whether the FGF geometry should be
    // redirected to newer (but possibly slower) code that computes
    // arc extents in 3D.

    // This is a quick, non-trustworthy test that only checks
    // the first sub-geometry of any aggregate types.  Mixed dimensionality
    // is invalid anyway -- we just do not specifically test for it here.

    bool is2D = true;

    int* ireader = (int*)fgf;

    int geom_type = (FdoGeometryType) IntegerFromStream(ireader, byteCount);

    FdoDimensionality dim;

    switch (geom_type)
    {
    // TODO: support MultiGeometry in newer 3D arc code.
    // For now, force it to the 2D case (which can actually handle Z and M).
    case FdoGeometryType_MultiGeometry :
        break;

    case FdoGeometryType_LineString :
    case FdoGeometryType_Polygon :
    case FdoGeometryType_Point :
	case FdoGeometryType_CurveString:
	case FdoGeometryType_CurvePolygon:
        IntegerStreamSizeCheck(ireader, byteCount);
        dim = (FdoDimensionality) *ireader++;
        // Don't bother updating fgfByteSize -- we are done!
        if (dim != FdoDimensionality_XY)
            is2D = false;
        break;

    case FdoGeometryType_MultiLineString :
    case FdoGeometryType_MultiPolygon :
    case FdoGeometryType_MultiPoint :
	case FdoGeometryType_MultiCurveString:
	case FdoGeometryType_MultiCurvePolygon:
        if (byteCount < ((FdoInt32)(3*sizeof(*ireader))))  // About to read 3 integers.
            throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_15_UNSUPPORTEDGEOMETRYDATA)));
        ireader++;                              // Skip subgeometry counter.
        ireader++;                              // Skip subgeometry type.
        dim = (FdoDimensionality) *ireader++;   // Check dimensionality of first subgeometry.
        // Don't bother updating byteCount -- we are done!
        if (dim != FdoDimensionality_XY)
            is2D = false;
        break;

    default:
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_10_UNSUPPORTEDGEOMETRYTYPE)));
    }

    return is2D;
}

void FdoSpatialUtility::GetExtents(FdoByteArray* fgfArray, double& outMinX, double& outMinY, double& outMaxX, double& outMaxY)
{  
    unsigned char* fgf = fgfArray->GetData();
    FdoInt32 byteCount = fgfArray->GetCount();

    int* ireader = (int*)fgf;

    if (MightHave3dArcs(fgf, byteCount))
    {
        double dummyMinz, dummyMaxz;
        FdoSpatialUtilityGeometryExtents::GetExtents(fgfArray, outMinX, outMinY, dummyMinz, outMaxX, outMaxY, dummyMaxz);
        return;
    }

    // the geometry type
    int geom_type = (FdoGeometryType) IntegerFromStream(ireader, byteCount);

    switch (geom_type)
    {
    case FdoGeometryType_MultiLineString :
    case FdoGeometryType_MultiPolygon :
    case FdoGeometryType_MultiPoint :
    case FdoGeometryType_LineString :
    case FdoGeometryType_Polygon :
    case FdoGeometryType_Point :
        {
            double minx =  DBL_MAX;
            int signbit_minx = SIGN_BIT(minx);
            
            double maxx = -DBL_MAX;
            int signbit_maxx = SIGN_BIT(maxx);

            double miny =  DBL_MAX;
            int signbit_miny = SIGN_BIT(miny);

            double maxy = -DBL_MAX;
            int signbit_maxy = SIGN_BIT(maxy);


            bool is_multi = (geom_type == FdoGeometryType_MultiLineString)
                || (geom_type == FdoGeometryType_MultiPolygon
                || (geom_type == FdoGeometryType_MultiPoint));

            // the coordinate type
            int skip_vals = 0; //0 = XY, 1 = XYZ or XYM, 2 = XYZM

            //temp variables used inside the loop
            int num_geoms = 1;
            double x;
            double y;
            int point_count;
            int contour_count;
            FdoDimensionality dim;
            double* dreader;

            //in case of multipolygon or multilinestring or multipoint, 
            //read poly or linestring count
            if (is_multi)
            {
                num_geoms = IntegerFromStream(ireader, byteCount);
            }

            for (int q=0; q<num_geoms; q++)
            {
                //skip past geometry type of subgeometry
                //we know it is LineString or Polygon or Point respectively
                if (is_multi)
                {
                    ireader++;
                    byteCount -= sizeof(*ireader);
                }

                //read cordinate typeB
                dim = (FdoDimensionality) IntegerFromStream(ireader, byteCount);

                skip_vals = 0;
                if (dim & FdoDimensionality_Z) skip_vals++;
                if (dim & FdoDimensionality_M) skip_vals++;

                // the number of contours in current polygon/linestring
                contour_count = 1; //for linestrings

                switch (geom_type)
                {
                case FdoGeometryType_Polygon:
                case FdoGeometryType_MultiPolygon:
                    contour_count = IntegerFromStream(ireader, byteCount);
                default: break;
                }

                for (int i=0; i<contour_count; i++)
                {
                    point_count = 1;

                    //point geoms do not have a point count, since 
                    //each piece is just one point each
                    if ((geom_type != FdoGeometryType_MultiPoint) 
                        && (geom_type != FdoGeometryType_Point))
                    {
                        point_count = IntegerFromStream(ireader, byteCount);
                    }

                    int ords_per_position = 2 /*X,Y*/ + skip_vals /*Z,M*/;
                    PositionStreamCheckAndAdvance(
                        point_count, ords_per_position, byteCount);

                    //*** ireader not valid from here down
                    dreader = (double*) ireader;

                    int num_pts_read = 0;
                   
                    while (num_pts_read  < point_count)
                    {
                        x = *dreader++;
                        y = *dreader++;

                        //skip past z and m
                        switch (skip_vals)
                        {
                        case 2: dreader++;
                        case 1: dreader++;
                        default:break;
                        }

#ifdef _WIN32
                      
                        //-------------------------------------------------
                        //
                        // Faster min/max computation, not using the FPU
                        // This code is 2.4 times faster than doing an FPU
                        // comparison on the latest Intel processor
                        // IN THE CONTEXT OF THIS LOOP, NOT IN GENERAL!
                        //
                        //-------------------------------------------------
                        int signbitx = SIGN_BIT(x);
                        int signbity = SIGN_BIT(y);

                        if (signbitx == signbit_minx)
                        {
                            //otherwise signs are the same and we can compare as ints
                            switch (signbitx)
                            {
                            case 0:
                                if (*(FdoInt64*)&x < *(FdoInt64*)&minx)
                                    minx = x;
                                break;
                            default:
                                if (*(FdoInt64*)&x > *(FdoInt64*)&minx)
                                    minx = x;
                                break;
                            }
                        }
                        else
                        {
                            //signs different

                            switch (signbitx)
                            {
                            case 0:
                                break;
                            default:
                                //is x the negative?
                                minx = x;
                                signbit_minx = signbitx;
                                break;
                            }
                        }

                        if (signbitx == signbit_maxx)
                        {
                            //otherwise signs are the same and we can compare as ints
                            switch (signbitx)
                            {
                            case 0:
                                if (*(FdoInt64*)&x > *(FdoInt64*)&maxx)
                                    maxx = x;
                                break;
                            default:
                                if (*(FdoInt64*)&x < *(FdoInt64*)&maxx)
                                    maxx = x;
                                break;
                            }
                        }
                        else
                        {
                            //signs different

                            switch (signbitx)
                            {
                            case 0:
                                //is x the positive?
                                maxx = x;
                                signbit_maxx = signbitx;
                                break;
                            default:
                                break;
                            }
                        }

                        if (signbity == signbit_miny)
                        {
                            //signs are the same and we can compare as ints
                            switch (signbity)
                            {
                            case 0:
                                if (*(FdoInt64*)&y < *(FdoInt64*)&miny)
                                    miny = y;
                                break;
                            default:
                                if (*(FdoInt64*)&y > *(FdoInt64*)&miny)
                                    miny = y;
                                break;
                            }
                        }
                        else
                        {
                            //signs different

                            switch (signbity)
                            {
                            case 0:
                                break;
                            default:
                                //is y the negative?
                                miny = y;
                                signbit_miny = signbity;
                                break;
                            }
                        }

                        if (signbity == signbit_maxy)
                        {
                            //signs are the same and we can compare as ints
                            switch (signbity)
                            {
                            case 0:
                                if (*(FdoInt64*)&y > *(FdoInt64*)&maxy)
                                    maxy = y;
                                break;
                            default:
                                if (*(FdoInt64*)&y < *(FdoInt64*)&maxy)
                                    maxy = y;
                                break;
                            }
                        }
                        else
                        {
                            //signs different
                            switch (signbity)
                            {
                            case 0:
                                //is y the positive?
                                maxy = y;
                                signbit_maxy = signbity;
                                break;
                            default:
                                break;
                            }
                        }
                        
#else
                        //----------------------------------
                        //
                        //original way of finding min and max in this loop
                        //
                        //----------------------------------
                        if (x > maxx)
                            maxx = x;
                        if (x < minx)
                            minx = x;
                        if (y > maxy)
                            maxy = y;
                        if (y < miny)
                            miny = y;
#endif
                        num_pts_read++;
                    }

                    ireader = (int*)dreader;
                    //*** ireader valid again
                }
            }

            outMinX = minx;
            outMinY = miny;
            outMaxX = maxx;
            outMaxY = maxy;
        }

        break;

    default:
    
        //all other geometry types
        FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
        FdoPtr<FdoIGeometry> fgfgeom = gf->CreateGeometryFromFgf(fgfArray);
        
        FdoPtr<FdoIEnvelope> envelope = fgfgeom->GetEnvelope();

        outMinX = envelope->GetMinX();
        outMinY = envelope->GetMinY();
        outMaxX = envelope->GetMaxX();
        outMaxY = envelope->GetMaxY();

        AdjustExtentsForCurves(fgfgeom, outMinX, outMinY, outMaxX, outMaxY);

        break;
    }
}


void FdoSpatialUtility::GetExtents(FdoByteArray* fgfArray, double& minx, double& miny, double& minz, double& maxx, double& maxy, double& maxz)
{
    // Check dimensionality.  If it appears to be 2D, branch off
    // to older code that is optimized for linear 2D types.  Otherwise, use newer 3D code
    // that also has built-in 3D arc extent handling.

    // In the 2D case, the Z limits are filled in here with 0.0 for performance.
    // NaN is the correct answer, but it requires an extra function call.
    // It is invalid for callers to check Z if it isn't useful.

    // TODO: integrate with FdoSpatialUtilityCircularArc logic.
    // That type currently takes only FdoGeometry types,
    // not FGF or basic types.

    // TODO: branch on dimensionality at a slightly lower level so
    // that the 2D code doesn't need to be slowed down to 
    // allow for Z or M at all.

    unsigned char* fgf = fgfArray->GetData();
    FdoInt32 byteCount = fgfArray->GetCount();

    int* ireader = (int*)fgf;

    if ( Is2d(fgf, byteCount) )
    {
        // Use 2D routine.
        minz = 0.0;
        maxz = 0.0;
        GetExtents(fgfArray, minx, miny, maxx, maxy);
    }
    else
    {
        FdoSpatialUtilityGeometryExtents::GetExtents(fgfArray, minx, miny, minz, maxx, maxy, maxz);
    }
}


void FdoSpatialUtility::AdjustExtentsForCurves(
    FdoIGeometry * geometry,
    double& minx, double& miny, double& maxx, double& maxy)
{
    // The given extent parameters are all for "update".  Only change them if the
    // possibly too-small results for non-linear shapes, e.g. circular arcs.
    // Such shapes may be too small due to approximations in FdoGeometry.

    FdoGeometryType geomType = geometry->GetDerivedType();

    switch (geomType)
    {
	case FdoGeometryType_CurveString:
        {
            FdoICurveString * curve = static_cast<FdoICurveString *> (geometry);
            FdoPtr<FdoCurveSegmentCollection> csc = curve->GetCurveSegments();
            AdjustExtentsForCurves(csc, minx, miny, maxx, maxy);
        }
        break;

	case FdoGeometryType_CurvePolygon:
        {
            FdoICurvePolygon * geom = static_cast<FdoICurvePolygon *> (geometry);

            FdoPtr<FdoIRing> ring = geom->GetExteriorRing();
            FdoPtr<FdoCurveSegmentCollection> csc = ring->GetCurveSegments();
            AdjustExtentsForCurves(csc, minx, miny, maxx, maxy);

            for (FdoInt32 i = 0;  i < geom->GetInteriorRingCount();  i++)
            {
                ring = geom->GetInteriorRing(i);
                csc = ring->GetCurveSegments();
                AdjustExtentsForCurves(csc, minx, miny, maxx, maxy);
            }
        }
        break;

	case FdoGeometryType_MultiGeometry:
	case FdoGeometryType_MultiCurveString:
	case FdoGeometryType_MultiCurvePolygon:
        {
            FdoIGeometricAggregateAbstract * multiGeom = static_cast<FdoIGeometricAggregateAbstract *> (geometry);
            FdoInt32 numSubGeometries = multiGeom->GetCount();

            for (FdoInt32 i = 0;  i < numSubGeometries;  i++)
            {
                FdoPtr<FdoIGeometry> subGeom;
                switch (geomType)
                {
	            case FdoGeometryType_MultiGeometry:     subGeom = static_cast<const FdoIMultiGeometry *>(multiGeom)->GetItem(i);  break;
                case FdoGeometryType_MultiCurveString:  subGeom = static_cast<const FdoIMultiCurveString *>(multiGeom)->GetItem(i);  break;
	            case FdoGeometryType_MultiCurvePolygon: subGeom = static_cast<const FdoIMultiCurvePolygon *>(multiGeom)->GetItem(i);  break;
	            default: throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_10_UNSUPPORTEDGEOMETRYTYPE)));
                }
                AdjustExtentsForCurves(subGeom, minx, miny, maxx, maxy);
            }
        }
        break;

    default:
        break;  // No adjustments for other types.  They do not contain arcs.
    }
}


void FdoSpatialUtility::AdjustExtentsForCurves(
    FdoCurveSegmentCollection * csc,
    double& minx, double& miny, double& maxx, double& maxy)
{
    for (FdoInt32 i = 0;  i < csc->GetCount();  i++)
    {
        FdoPtr<FdoICurveSegmentAbstract> cs = csc->GetItem(i);
        FdoGeometryComponentType gct = cs->GetDerivedType();

        switch (gct)
        {
        case FdoGeometryComponentType_CircularArcSegment:
            {
                FdoICircularArcSegment * arc = static_cast<FdoICircularArcSegment *> ((FdoICurveSegmentAbstract *)cs);
                FdoPtr<FdoIDirectPosition> start = arc->GetStartPosition();
                FdoPtr<FdoIDirectPosition> mid = arc->GetMidPoint();
                FdoPtr<FdoIDirectPosition> end = arc->GetEndPosition();
                FdoSpatialUtilityCircularArc utilArc(start, mid, end);
                if (utilArc.m_envelope != NULL)
                {
                    double eminX = utilArc.m_envelope->GetMinX();
                    double eminY = utilArc.m_envelope->GetMinY();
                    double emaxX = utilArc.m_envelope->GetMaxX();
                    double emaxY = utilArc.m_envelope->GetMaxY();

                    if (eminX < minx)   minx = eminX;
                    if (eminY < miny)   miny = eminY;
                    if (emaxX > maxx)   maxx = emaxX;
                    if (emaxY > maxy)   maxy = emaxY;
                }
            }
            break;

        case FdoGeometryComponentType_LineStringSegment:
        default:
            break;  // We only care about arcs (might handle more types here later though).
        }
    }
}


double FdoSpatialUtility::ComputeLinearRingArea( FdoILinearRing* ring )
{
    FdoInt32 count;
    double area;
    double x1;
    double y1;
    double z;
    double m;
    FdoInt32 dimensionality;
    double x2;
    double y2;

    count = ring->GetCount ();
    area = 0.0;
    // Traverse the ring compute area by adding each segment area.
    ring->GetItemByMembers (0, &x1, &y1, &z, &m, &dimensionality);
    for ( int i = 1; i < count; i++ )
    {
        ring->GetItemByMembers (i, &x2, &y2, &z, &m, &dimensionality);
        area += (x2 - x1) * (y2 + y1);
        x1 = x2; // advance
        y1 = y2;
    }

    return (area / 2.0);
}

double FdoSpatialUtility::ComputeLinearRingLength( FdoILinearRing* ring )
{
    double length = 0.0;

    // Traverse the ring compute length by adding each segment length.
    FdoPtr<FdoDirectPositionCollection> positions = ring->GetPositions();
    
    FdoPtr<FdoIDirectPosition> pos1 = positions->GetItem(0);
    FdoPtr<FdoIDirectPosition> pos2;

    for ( int i = 1; i < positions->GetCount(); i++ )
    {
        pos2 =  positions->GetItem(i);
        
        double dx = pos2->GetX() - pos1->GetX();
        double dy = pos2->GetY() - pos1->GetY();
        length += sqrt(dx*dx + dy*dy);
        pos1 = pos2; // advance 
    }

    return length;
}

bool FdoSpatialUtility::LinearRingIsClockwise( FdoILinearRing* ring )
{
    return ( ComputeLinearRingArea( ring ) > 0.0);
}

bool FdoSpatialUtility::RingIsClockwise( FdoIRing* ring )
{
    double area = 0.0;
    double x1(0.0), y1(0.0), x2(0.0), y2(0.0);

    FdoInt32 count = ring->GetCount ();
    for ( int i =  0; i < count; i++ )
    {
        FdoPtr<FdoICurveSegmentAbstract> curveSegment = ring->GetItem(i);
        FdoGeometryComponentType geoComponentType = curveSegment->GetDerivedType();
        if (geoComponentType == FdoGeometryComponentType_CircularArcSegment)
        {
            FdoIArcSegmentAbstract* arcSegment = dynamic_cast<FdoIArcSegmentAbstract*>(curveSegment.p);
            FdoPtr<FdoIDirectPosition> pos1 = arcSegment->GetStartPosition();
            FdoPtr<FdoIDirectPosition> pos2 = arcSegment->GetMidPoint();
            FdoPtr<FdoIDirectPosition> pos3 = arcSegment->GetEndPosition();
            double p1x = pos1->GetX();
            double p1y = pos1->GetY();
            double p2x = pos2->GetX();
            double p2y = pos2->GetY();
            double p3x = pos3->GetX();
            double p3y = pos3->GetY();
            
            if (i > 0)
                area += (p1x - x1) * (p1x + y1);
            area += (p2x - p1x) * (p2y + p1y) + (p3x - p2x) * (p3y + p2y);

            x1 = p3x;
            y1 = p3y;
        }
        else if (geoComponentType == FdoGeometryComponentType_LineStringSegment)
        {
            FdoILineStringSegment* lineStringSegment = dynamic_cast<FdoILineStringSegment*>(curveSegment.p);
            FdoInt32 count = lineStringSegment->GetCount ();
            FdoInt32 dimensionality = lineStringSegment->GetDimensionality();
            const double* data = lineStringSegment->GetOrdinates();

            FdoInt32 num = 0;
            if ( dimensionality == FdoDimensionality_XY )
                num = 2;
            else if ( dimensionality == ( FdoDimensionality_XY | FdoDimensionality_Z ) )
                num = 3;
            else if ( dimensionality == ( FdoDimensionality_XY | FdoDimensionality_M ) )
                num = 3;
            else if ( dimensionality == ( FdoDimensionality_XY | FdoDimensionality_Z | FdoDimensionality_M ) )
                num = 4;

            int pos = 0;
            if (i == 0)
            {
                x1 = data[0];
                y1 = data[1];
                pos = num;
                for ( int j = 1; j < count; j++ )
                {
                    x2 = data[pos];
                    y2 = data[pos + 1];
                    area += (x2 - x1) * (y2 + y1);
                    x1 = x2;
                    y1 = y2;
                    pos += num;
                }
            }
            else
            {
                for ( int j =  0; j < count; j++ )
                {
                    x2 = data[pos];
                    y2 = data[pos + 1];
                    area += (x2 - x1) * (y2 + y1);
                    x1 = x2;
                    y1 = y2;
                    pos += num;
                }
            }
        }
    }

    return (area > 0.0);
}

FdoIGeometry* FdoSpatialUtility::CreateGeometryFromRings( FdoLinearRingCollection* rings, bool relateRings )
{
    FdoPtr<FdoIPolygon>         polygon;
    FdoPtr<FdoIMultiPolygon>    multipolygon;

    FdoIGeometry                *geometry;
    FdoFgfGeometryFactory       *factory = FdoFgfGeometryFactory::GetInstance ();

    FdoInt32    numRings = rings->GetCount(); 
    
    // Just one ring case, quick exit.
    if ( numRings == 1 )
    {
        FdoPtr<FdoILinearRing> ring = rings->GetItem(0);

        polygon = factory->CreatePolygon( ring, NULL );
        geometry = polygon;

        FDO_SAFE_ADDREF( geometry );
        FDO_SAFE_RELEASE( factory );

        return geometry;
    }

    FdoPtr<FdoLinearRingCollection> intRings = FdoLinearRingCollection::Create (); 
    FdoPtr<FdoPolygonCollection>    polygons = FdoPolygonCollection::Create ();

    // When not relating rings just return either a Polygon or a Multipolygon
    if ( !relateRings )
    {
        for ( int i = 0; i < numRings; i++ )
        {
            FdoPtr<FdoILinearRing> extRing = rings->GetItem(i);
            polygon = factory->CreatePolygon( extRing, intRings );
            polygons->Add (polygon);
        }

        geometry = factory->CreateMultiPolygon (polygons);
    }
    else
    {
        // Relate (associate) rings: find exterior/interior rings
        // Ignore the rings orientation (cvw/ccw) 
        RingArea_def *ringsArea = new RingArea_def[numRings];

        // Cache all envelops
        std::vector<FdoIEnvelope *> allEnv;

        for ( int i = 0; i < numRings; i++ )
        { 
            FdoPtr<FdoILinearRing> ring = rings->GetItem(i);
            ringsArea[i].index = i;
            ringsArea[i].indexAssoc = -1; 
            ringsArea[i].area = fabs( ComputeLinearRingArea( ring ) );

            FdoIEnvelope* env = ring->GetEnvelope();
            allEnv.push_back(env);
        }

        qsort( (void *)ringsArea, numRings, sizeof(RingArea_def), CompareByArea );

        // Navigate backwards and associate with the smaller possible ring
        for ( int i = numRings-1; i > 0; i-- )
        { 
            FdoInt32 index1 = ringsArea[i].index; 
            FdoPtr<FdoILinearRing> ring1 = rings->GetItem(index1);

            // Pick the mid point of the segment instead of 1st point for "point-in-ring" testing. 
            // This checks unambiguosly "on boundary" cases where the rings are touching.
            double x0, y0, x1, y1;
            int    dummydim;

            GET_ITEM(ring1, 0, &x0, &y0, &dummydim);

            double x = x1 = x0;
            double y = y1 = y0;
            int  k = 1;

            while ((x0 == x1) && (y0 == y1) && (k < ring1->GetCount() - 1))
            {
                GET_ITEM(ring1, k, &x1, &y1, &dummydim);  
                k++;
            }

            // Take the mid of the segment
            x = (x0 + x1)/2;
            y = (y0 + y1)/2;

            for ( int j = i-1; j >=0; j-- )
            {
                FdoInt32 index2 = ringsArea[j].index; 

                FdoPtr<FdoILinearRing> ring2 = rings->GetItem(index2);
                FdoIEnvelope*   extent2 = allEnv.at(index2);

                // Check the point against the ring bounding box
                bool isInside = (x <= extent2->GetMaxX() && x >= extent2->GetMinX() && y <= extent2->GetMaxY() && y >= extent2->GetMinY());

                if (isInside)
                    isInside = FdoSpatialUtility::PointInRingFast(ring2, x, y, EPSILON);

                if (isInside) 
                {
                    ringsArea[i].indexAssoc = index2;
                    break;
                }
            }
        }

        // Cleanup
        for (size_t i = 0; i < allEnv.size(); i++) {

            FdoIEnvelope* ptr = allEnv.at(i);
            FDO_SAFE_RELEASE(ptr); 
        }

        // Create a new array in the original order
        RingArea_def *ringsOriginal = new RingArea_def[numRings];
        for (int i = 0; i < numRings; i++)
            ringsOriginal[ringsArea[i].index] = ringsArea[i];

        // At this point all the rings have been associated to the closest ring. Those not associated (-1) are external loops.
        // Check for nested loops and make corrections, i.e. turn interior rings into exterior if the case,
		for ( int i = 0; i < numRings; i++ )
		{ 
            int assocIndex = ringsOriginal[i].indexAssoc;

            // Skip those not associated
            if ( assocIndex == -1 )
                continue;

            int depth = 0;
            
            while (assocIndex != -1)
            {
                depth++;
                assocIndex = ringsOriginal[assocIndex].indexAssoc;
            }

            bool isExteriorRing = (depth % 2 == 0); // even number

            if (isExteriorRing)
                ringsOriginal[i].indexAssoc = -1;
        }

        // Create output polygons
		for ( int i = 0; i < numRings; i++ )
		{ 
			// External ring, look for its associated rings
			if ( ringsOriginal[i].indexAssoc == -1 )
			{
				FdoInt32  extRingIndex = ringsOriginal[i].index;
				FdoPtr<FdoILinearRing> extRing = rings->GetItem(extRingIndex);

				FdoPtr<FdoLinearRingCollection> intRings;
				
				for (int j = 0; j < numRings; j++)
				{ 
					if ( ringsOriginal[j].indexAssoc == extRingIndex)
 					{
                        if (intRings == NULL)
                            intRings = FdoLinearRingCollection::Create (); 

						FdoPtr<FdoILinearRing> intRing = rings->GetItem(ringsOriginal[j].index);
						intRings->Add( intRing );
					}
				}

				polygon = factory->CreatePolygon( extRing, intRings );
				polygons->Add (polygon);
			}
		}

        delete[] ringsArea;
        delete[] ringsOriginal;

        // Check the number of polyons created and return the appropiate geometry type.
        geometry = ( polygons->GetCount() == 1 ) ? (FdoIGeometry *) polygons->GetItem(0) :
                                                   (FdoIGeometry *) factory->CreateMultiPolygon (polygons);
    }

    FDO_SAFE_RELEASE( factory );

    return geometry;
}

int FdoSpatialUtility::CompareByArea( const void* elem1, const void* elem2)
{
    RingArea_def*   el1 = (RingArea_def*) elem1;
    RingArea_def*   el2 = (RingArea_def*) elem2;
    int             ret;
    
    if ( el1->area < el2->area )
        ret = 1;
    else if ( el1->area > el2->area )
        ret = -1;
    else
        ret = 0;

    return ret;
}

/*   The two end points of the line segment define a line with equation: *
*                                                                       *
*               a*x - b*y + c * 0                                       *
*                                                                       *
*       where:                                                          *
*               a * y_2 - y_1,                                          *
*               b * x_2 - x_1,                                          *
*               c * b*y_1 - a*x_1                                       *
*                                                                       *
*       The  perpendicular line from x_0,y_0 to the line  segment       *
*       is a line with equation:                                        *
*                                                                       *
*               b*x + a*y + d * 0                                       *
*                                                                       *
*       where:                                                          *
*               d * -b*x_0 - a*y_0                                      *
*                                                                       *
*       The intersection  of these two lines is the closest point       *
*       x_c,y_c on the line to the point x_0,y_0.                       *
*                                                                       *
*               x_c * (-b*d - a*c) / (a**2 + b**2)                      *
*               y_c * (-a*d + b*c) / (a**2 + b**2)                      *
*                                                                       *
*       Once the intersection  point  is  calculated,  it must be       *
*       determined  if the closest point is between  x_1,y_1  and       *
*       x_2,y_2.  This  is done by rotating the line segment from       *
*       x_1,y_1 to x_c,y_c  clockwise about x_1,y_1 so that it is       *
*       at 0 degrees to  the  x  axis.   These rotation parms are       *
*       already  known (a and b).  Only the x  component,  offset       *
*       from x_1,y_1 is needed.  This component should be between       *
*       0 and the distance between x_1,y_1 and x_2,y_2 (sqrt(a**2       *
*       + b**2) to be between x_1,y_1 and x_2,y_2.                      *
*                                                                       *
*************************************************************************/
double FdoSpatialUtility::pt_dist_to_seg(
    double      PtX,
    double      PtY,
    double      LineX1,
    double      LineY1,
    double      LineX2,
    double      LineY2,
    double      tolerance)
{
    double      x_0 = PtX;
    double      y_0 = PtY;
    double      x_1 = LineX1;
    double      y_1 = LineY1;
    double      x_2 = LineX2;
    double      y_2 = LineY2;

    double      x_c;            /* closest point to the line */
    double      y_c;
    double      distsqrd = 0.0;
    double      a;              /* line parms, a*x - b*y + c = 0  */
    double      b;
    double      c;
    double      d;              /* perp. line, b*x + a*y + d = 0 */
    double      denom;          /* denominator in calcs, = a**2 + b**2 */
    double      r;
    double      d2;
    int         perp;           /* perpendicular flag */

    a = y_2 - y_1;
    b = x_2 - x_1;
 
    denom = (a*a) + (b*b);
 
    if ( denom < tolerance*tolerance ) {     /* line segment degenerate, one point */
        x_c = x_1;
        y_c = y_1;
        perp = true;
    }
    else {                      /* calculate intersection of perp and line */
        c = b*y_1 - a*x_1;
        d = -b*x_0 - a*y_0;
        x_c = (-b*d - a*c) / denom;
        y_c = (b*c - a*d) / denom;
 
        r = b*(x_c-x_1) + a*(y_c-y_1);    /* dist within line seg * root a**2 + b**2 */
        perp = (r >= 0.0  &&  r <= denom);
    }
 
    if (perp) {
        distsqrd = (x_c-x_0)*(x_c-x_0) + (y_c-y_0)*(y_c-y_0);
    }
    else {
        distsqrd = (x_0-x_1)*(x_0-x_1) + (y_0-y_1)*(y_0-y_1);
        d2 = (x_0-x_2)*(x_0-x_2) + (y_0-y_2)*(y_0-y_2);
        if (d2 < distsqrd) {
            x_c = x_2;
            y_c = y_2;
            distsqrd = d2;
        }
        else {
            x_c = x_1;
            y_c = y_1;
        }
    }
 
    return ( sqrt (distsqrd) );
}

// Validity test for arc (may include other tests in the future)
// Generate a line string ("chord") from the first point in the arc to the last point in the arc. 
// Calculate the distance from the middle point specified in the arc to the chord (that connects 
// the first and last point).  If the distance is less than tolerance, the arc is invalid. 
bool FdoSpatialUtility::IsCircularArcValid( FdoICircularArcSegment *arc, double tolerance )
{
    FdoPtr<FdoIDirectPosition> start = arc->GetStartPosition();
    FdoPtr<FdoIDirectPosition> mid   = arc->GetMidPoint();
    FdoPtr<FdoIDirectPosition> end   = arc->GetEndPosition();

    double  distance = pt_dist_to_seg( mid->GetX(),   mid->GetY(),
                                       start->GetX(), start->GetY(),
                                       end->GetX(),   end->GetY(),
                                       tolerance);

    return ( distance >= tolerance ); 
}

FdoILinearRing* FdoSpatialUtility::ReverseLinearRingVertexOrder(FdoILinearRing* linearRing)
{
    FdoPtr<FdoFgfGeometryFactory> factory = FdoFgfGeometryFactory::GetInstance();
    FdoPtr<FdoDirectPositionCollection> positions = FdoDirectPositionCollection::Create();

    FdoInt32 count = linearRing->GetCount ();
    for ( int i =  count -1; i >= 0; i-- )
    {
        FdoPtr<FdoIDirectPosition> position = linearRing->GetItem(i);
        positions->Add(position);
    }

    FdoPtr<FdoILinearRing> newLinearRing = factory->CreateLinearRing(positions);
    return FDO_SAFE_ADDREF(newLinearRing.p);
}

FdoIRing* FdoSpatialUtility::ReverseRingVertexOrder(FdoIRing* ring)
{
    FdoPtr<FdoFgfGeometryFactory> factory = FdoFgfGeometryFactory::GetInstance();
    FdoPtr<FdoCurveSegmentCollection> curveSegments = FdoCurveSegmentCollection::Create();

    FdoInt32 segCount = ring->GetCount ();
    for ( FdoInt32 i =  segCount -1; i >= 0; i-- )
    {
        FdoPtr<FdoICurveSegmentAbstract> oldCurveSegment = ring->GetItem(i);
        FdoGeometryComponentType geoComponentType = oldCurveSegment->GetDerivedType();
        if (geoComponentType == FdoGeometryComponentType_CircularArcSegment)
        {
            FdoIArcSegmentAbstract* arcSegment = dynamic_cast<FdoIArcSegmentAbstract*>(oldCurveSegment.p);
            FdoPtr<FdoIDirectPosition> endPos = arcSegment->GetEndPosition();
            FdoPtr<FdoIDirectPosition> midPos = arcSegment->GetMidPoint();
            FdoPtr<FdoIDirectPosition> startPos = arcSegment->GetStartPosition();
            FdoPtr<FdoICurveSegmentAbstract> newCurveSegment = factory->CreateCircularArcSegment(endPos, midPos, startPos);
            curveSegments->Add(newCurveSegment);
        }
        else if (geoComponentType == FdoGeometryComponentType_LineStringSegment)
        {
            FdoILineStringSegment* lineStringSegment = dynamic_cast<FdoILineStringSegment*>(oldCurveSegment.p);
            FdoPtr<FdoDirectPositionCollection> positions = FdoDirectPositionCollection::Create();
            FdoInt32 posCount = lineStringSegment->GetCount ();
            for ( FdoInt32 j =  posCount -1; j >= 0; j-- )
            {
                FdoPtr<FdoIDirectPosition> position = lineStringSegment->GetItem(j);
                positions->Add(position);
            }
            FdoPtr<FdoILineStringSegment> newLineStringSegment = factory->CreateLineStringSegment(positions);
            curveSegments->Add(newLineStringSegment);
        }
    }

    FdoPtr<FdoIRing> newRing = factory->CreateRing(curveSegments);
    return FDO_SAFE_ADDREF(newRing.p);
}

FdoIPolygon* FdoSpatialUtility::ReversePolygonVertexOrder(FdoIPolygon* polygon)
{
    FdoPtr<FdoFgfGeometryFactory> factory = FdoFgfGeometryFactory::GetInstance();
    FdoPtr<FdoILinearRing> exteriorRing = polygon->GetExteriorRing();
    FdoPtr<FdoILinearRing> newExteriorRing = ReverseLinearRingVertexOrder(exteriorRing);

    FdoPtr<FdoLinearRingCollection> newInteriorRings = FdoLinearRingCollection::Create();
    for (FdoInt32 i = 0; i < polygon->GetInteriorRingCount(); i++)
    {
        FdoPtr<FdoILinearRing> interiorRing = polygon->GetInteriorRing(i);
        FdoPtr<FdoILinearRing> newInteriorRing = ReverseLinearRingVertexOrder(interiorRing);
        newInteriorRings->Add(newInteriorRing);
    }
    return factory->CreatePolygon(newExteriorRing, newInteriorRings);
}

FdoICurvePolygon* FdoSpatialUtility::ReversePolygonVertexOrder(FdoICurvePolygon* polygon)
{
    FdoPtr<FdoFgfGeometryFactory> factory = FdoFgfGeometryFactory::GetInstance();
    FdoPtr<FdoIRing> exteriorRing = polygon->GetExteriorRing();
    FdoPtr<FdoIRing> newExteriorRing = ReverseRingVertexOrder(exteriorRing);

    FdoPtr<FdoRingCollection> newInteriorRings = FdoRingCollection::Create();
    for (FdoInt32 i = 0; i < polygon->GetInteriorRingCount(); i++)
    {
        FdoPtr<FdoIRing> interiorRing = polygon->GetInteriorRing(i);
        FdoPtr<FdoIRing> newInteriorRing = ReverseRingVertexOrder(interiorRing);
        newInteriorRings->Add(newInteriorRing);
    }
    return factory->CreateCurvePolygon(newExteriorRing, newInteriorRings);
}

FdoIPolygon* FdoSpatialUtility::FixPolygonVertexOrder (
    FdoIPolygon * polygon, 
    FdoPolygonVertexOrderRule vertexOrderRule )
{
    // Check vertex order of exterior ring
    FdoPtr<FdoILinearRing> exteriorRing = polygon->GetExteriorRing();
    bool bClockwise = LinearRingIsClockwise(exteriorRing);
    bool bRevertExteriorRing = (bClockwise && vertexOrderRule == FdoPolygonVertexOrderRule_CCW) ||
                       (!bClockwise && vertexOrderRule == FdoPolygonVertexOrderRule_CW);

    // It is safer to check interior rings too because they may have
    // wrong vertex order too.
    bool bFixPolygon = bRevertExteriorRing;
    std::vector<bool> revertInteriorRings;
    for (FdoInt32 i = 0; i < polygon->GetInteriorRingCount(); i++)
    {
        FdoPtr<FdoILinearRing> interiorRing = polygon->GetInteriorRing(i);
        bClockwise = LinearRingIsClockwise(interiorRing);
        bool bRevertInteriorRing = (!bClockwise && vertexOrderRule == FdoPolygonVertexOrderRule_CCW) ||
                                  (bClockwise && vertexOrderRule == FdoPolygonVertexOrderRule_CW);
        revertInteriorRings.push_back(bRevertInteriorRing);
        bFixPolygon = bRevertInteriorRing || bFixPolygon;
    }

    if (bFixPolygon)
    {
        FdoPtr<FdoFgfGeometryFactory> factory = FdoFgfGeometryFactory::GetInstance();
        FdoPtr<FdoILinearRing> newExteriorRing;
        if (bRevertExteriorRing)
            newExteriorRing = ReverseLinearRingVertexOrder(exteriorRing);
        else
            newExteriorRing = exteriorRing;

        FdoPtr<FdoLinearRingCollection> newInteriorRings = FdoLinearRingCollection::Create();
        for (size_t i = 0; i < revertInteriorRings.size(); i++)
        {
            FdoPtr<FdoILinearRing> interiorRing = polygon->GetInteriorRing(i);
            if (revertInteriorRings[i])
            {
                FdoPtr<FdoILinearRing> newInteriorRing = ReverseLinearRingVertexOrder(interiorRing);
                newInteriorRings->Add(newInteriorRing);
            }
            else
                newInteriorRings->Add(interiorRing);
        }

        return factory->CreatePolygon(newExteriorRing, newInteriorRings);
    }
    return NULL;
}

FdoICurvePolygon* FdoSpatialUtility::FixPolygonVertexOrder (
    FdoICurvePolygon * polygon, 
    FdoPolygonVertexOrderRule vertexOrderRule )
{
    // Check vertex order of exterior ring
    FdoPtr<FdoIRing> exteriorRing = polygon->GetExteriorRing();
    bool bClockwise = RingIsClockwise(exteriorRing);
    bool bRevertExteriorRing = (bClockwise && vertexOrderRule == FdoPolygonVertexOrderRule_CCW) ||
                       (!bClockwise && vertexOrderRule == FdoPolygonVertexOrderRule_CW);

    // It is safer to check interior rings too because they may have
    // wrong vertex order too.
    bool bFixPolygon = bRevertExteriorRing;
    std::vector<bool> revertInteriorRings;
    for (FdoInt32 i = 0; i < polygon->GetInteriorRingCount(); i++)
    {
        FdoPtr<FdoIRing> interiorRing = polygon->GetInteriorRing(i);
        bClockwise = RingIsClockwise(interiorRing);
        bool bRevertInteriorRing = (!bClockwise && vertexOrderRule == FdoPolygonVertexOrderRule_CCW) ||
                                  (bClockwise && vertexOrderRule == FdoPolygonVertexOrderRule_CW);
        revertInteriorRings.push_back(bRevertInteriorRing);
        bFixPolygon = bRevertInteriorRing || bFixPolygon;
    }

    if (bFixPolygon)
    {
        FdoPtr<FdoFgfGeometryFactory> factory = FdoFgfGeometryFactory::GetInstance();
        FdoPtr<FdoIRing> newExteriorRing;
        if (bRevertExteriorRing)
            newExteriorRing = ReverseRingVertexOrder(exteriorRing);
        else
            newExteriorRing = exteriorRing;

        FdoPtr<FdoRingCollection> newInteriorRings = FdoRingCollection::Create();
        for (size_t i = 0; i < revertInteriorRings.size(); i++)
        {
            FdoPtr<FdoIRing> interiorRing = polygon->GetInteriorRing(i);
            if (revertInteriorRings[i])
            {
                FdoPtr<FdoIRing> newInteriorRing = ReverseRingVertexOrder(interiorRing);
                newInteriorRings->Add(newInteriorRing);
            }
            else
                newInteriorRings->Add(interiorRing);
        }

        return factory->CreateCurvePolygon(newExteriorRing, newInteriorRings);
    }
    return NULL;
}

FdoIGeometry* FdoSpatialUtility::FixPolygonVertexOrder (
    FdoIGeometry * geometry, 
    FdoPolygonVertexOrderRule vertexOrderRule )
{
    if (vertexOrderRule == FdoPolygonVertexOrderRule_None)
        return NULL;

    FdoGeometryType geomType = geometry->GetDerivedType();
    if (geomType == FdoGeometryType_Polygon)
    {
        FdoIPolygon* polygon = dynamic_cast<FdoIPolygon*>(geometry);
        return FixPolygonVertexOrder(polygon, vertexOrderRule);
    }
    else if (geomType == FdoGeometryType_MultiPolygon)
    {
        FdoIMultiPolygon* multiPolygon = dynamic_cast<FdoIMultiPolygon*>(geometry);
        
        bool bToBeBixed = false;
        FdoPtr<FdoPolygonCollection> polygons = FdoPolygonCollection::Create();
        for (FdoInt32 i = 0; i < multiPolygon->GetCount(); i++)
        {
            FdoPtr<FdoIPolygon> oldPolygon = multiPolygon->GetItem(i);
            FdoPtr<FdoIPolygon> newPolygon = FixPolygonVertexOrder(oldPolygon, vertexOrderRule);
            if (newPolygon == NULL)
                polygons->Add(oldPolygon);
            else
            {
                polygons->Add(newPolygon);
                bToBeBixed = true;
            }
        }

        if (bToBeBixed)
        {
            FdoPtr<FdoFgfGeometryFactory> factory = FdoFgfGeometryFactory::GetInstance();
            return factory->CreateMultiPolygon(polygons);
        }
    }
    else if (geomType == FdoGeometryType_CurvePolygon)
    {
        FdoICurvePolygon* curvePolygon = dynamic_cast<FdoICurvePolygon*>(geometry);
        return FixPolygonVertexOrder(curvePolygon, vertexOrderRule);
    }
    else if (geomType == FdoGeometryType_MultiCurvePolygon)
    {
        FdoIMultiCurvePolygon* multiCurvePolygon = dynamic_cast<FdoIMultiCurvePolygon*>(geometry);
        
        bool bToBeBixed = false;
        FdoPtr<FdoCurvePolygonCollection> curvePolygons = FdoCurvePolygonCollection::Create();
        for (FdoInt32 i = 0; i < multiCurvePolygon->GetCount(); i++)
        {
            FdoPtr<FdoICurvePolygon> oldCurvePolygon = multiCurvePolygon->GetItem(i);
            FdoPtr<FdoICurvePolygon> newCurvePolygon = FixPolygonVertexOrder(oldCurvePolygon, vertexOrderRule);
            if (newCurvePolygon == NULL)
                curvePolygons->Add(oldCurvePolygon);
            else
            {
                curvePolygons->Add(newCurvePolygon);
                bToBeBixed = true;
            }
        }

        if (bToBeBixed)
        {
            FdoPtr<FdoFgfGeometryFactory> factory = FdoFgfGeometryFactory::GetInstance();
            return factory->CreateMultiCurvePolygon(curvePolygons);
        }
    }
    return NULL;
}

FdoIGeometry* FdoSpatialUtility::ReversePolygonVertexOrder ( FdoIGeometry * geometry )
{
    FdoGeometryType geomType = geometry->GetDerivedType();
    if (geomType == FdoGeometryType_Polygon)
    {
        FdoIPolygon* polygon = dynamic_cast<FdoIPolygon*>(geometry);
        return ReversePolygonVertexOrder(polygon);
    }
    else if (geomType == FdoGeometryType_MultiPolygon)
    {
        FdoPtr<FdoFgfGeometryFactory> factory = FdoFgfGeometryFactory::GetInstance();
        FdoIMultiPolygon* multiPolygon = dynamic_cast<FdoIMultiPolygon*>(geometry);

        FdoPtr<FdoPolygonCollection> polygons = FdoPolygonCollection::Create();
        for (FdoInt32 i = 0; i < multiPolygon->GetCount(); i++)
        {
            FdoPtr<FdoIPolygon> oldPolygon = multiPolygon->GetItem(i);
            FdoPtr<FdoIPolygon> newPolygon = ReversePolygonVertexOrder(oldPolygon);
            polygons->Add(newPolygon);
        }
        return factory->CreateMultiPolygon(polygons);
    }
    else if (geomType == FdoGeometryType_CurvePolygon)
    {
        FdoICurvePolygon* curvePolygon = dynamic_cast<FdoICurvePolygon*>(geometry);
        return ReversePolygonVertexOrder(curvePolygon);
    }
    else if (geomType == FdoGeometryType_MultiCurvePolygon)
    {
        FdoPtr<FdoFgfGeometryFactory> factory = FdoFgfGeometryFactory::GetInstance();
        FdoIMultiCurvePolygon* multiCurvePolygon = dynamic_cast<FdoIMultiCurvePolygon*>(geometry);
        
        FdoPtr<FdoCurvePolygonCollection> curvePolygons = FdoCurvePolygonCollection::Create();
        for (FdoInt32 i = 0; i < multiCurvePolygon->GetCount(); i++)
        {
            FdoPtr<FdoICurvePolygon> oldCurvePolygon = multiCurvePolygon->GetItem(i);
            FdoPtr<FdoICurvePolygon> newCurvePolygon = ReversePolygonVertexOrder(oldCurvePolygon);
            curvePolygons->Add(newCurvePolygon);
        }
        return factory->CreateMultiCurvePolygon(curvePolygons);
    }
    return NULL;
}

FdoPolygonVertexOrderRule FdoSpatialUtility::CheckPolygonVertexOrder(FdoIPolygon* polygon)
{
    FdoPolygonVertexOrderRule vertexOrderRule = FdoPolygonVertexOrderRule_None;

    // Check vertex order of exterior ring
    FdoPtr<FdoILinearRing> exteriorRing = polygon->GetExteriorRing();
    bool bExteriorRingClockwise = LinearRingIsClockwise(exteriorRing);
    vertexOrderRule = bExteriorRingClockwise ? FdoPolygonVertexOrderRule_CW : FdoPolygonVertexOrderRule_CCW;

    // Check vertex order of interior rings
    for (FdoInt32 i = 0; i < polygon->GetInteriorRingCount(); i++)
    {
        FdoPtr<FdoILinearRing> interiorRing = polygon->GetInteriorRing(i);
        bool bInteriorRingClockwise = LinearRingIsClockwise(interiorRing);
        if ( bExteriorRingClockwise == bInteriorRingClockwise)
        {
            vertexOrderRule = FdoPolygonVertexOrderRule_None;
            break;
        }
    }

    return vertexOrderRule;
}

FdoPolygonVertexOrderRule FdoSpatialUtility::CheckPolygonVertexOrder(FdoICurvePolygon* polygon)
{
    FdoPolygonVertexOrderRule vertexOrderRule = FdoPolygonVertexOrderRule_None;

    // Check vertex order of exterior ring
    FdoPtr<FdoIRing> exteriorRing = polygon->GetExteriorRing();
    bool bExteriorRingClockwise = RingIsClockwise(exteriorRing);
    vertexOrderRule = bExteriorRingClockwise ? FdoPolygonVertexOrderRule_CW : FdoPolygonVertexOrderRule_CCW;

    // Check vertex order of interior rings
    for (FdoInt32 i = 0; i < polygon->GetInteriorRingCount(); i++)
    {
        FdoPtr<FdoIRing> interiorRing = polygon->GetInteriorRing(i);
        bool bInteriorRingClockwise = RingIsClockwise(interiorRing);
        if ( bExteriorRingClockwise == bInteriorRingClockwise)
        {
            vertexOrderRule = FdoPolygonVertexOrderRule_None;
            break;
        }
    }

    return vertexOrderRule;
}

FdoPolygonVertexOrderRule FdoSpatialUtility::CheckPolygonVertexOrder(FdoIGeometry* geometry)
{
    FdoPolygonVertexOrderRule vertexOrderRule = FdoPolygonVertexOrderRule_None;
    FdoGeometryType geomType = geometry->GetDerivedType();
    if (geomType == FdoGeometryType_Polygon)
    {
        FdoIPolygon* polygon = dynamic_cast<FdoIPolygon*>(geometry);
        vertexOrderRule = CheckPolygonVertexOrder(polygon);
    }
    else if (geomType == FdoGeometryType_MultiPolygon)
    {
        FdoIMultiPolygon* multiPolygon = dynamic_cast<FdoIMultiPolygon*>(geometry);
        FdoInt32 count = multiPolygon->GetCount();
        if (count > 0)
        {
            FdoPtr<FdoIPolygon> polygon = multiPolygon->GetItem(0);
            vertexOrderRule = CheckPolygonVertexOrder(polygon);
        }

        for (FdoInt32 i = 1; i < multiPolygon->GetCount(); i++)
        {
            FdoPtr<FdoIPolygon> polygon = multiPolygon->GetItem(i);
            if (vertexOrderRule != CheckPolygonVertexOrder(polygon))
            {
                vertexOrderRule = FdoPolygonVertexOrderRule_None;
                break;
            }
        }
    }
    else if (geomType == FdoGeometryType_CurvePolygon)
    {
        FdoICurvePolygon* curvePolygon = dynamic_cast<FdoICurvePolygon*>(geometry);
        vertexOrderRule = CheckPolygonVertexOrder(curvePolygon);
    }
    else if (geomType == FdoGeometryType_MultiCurvePolygon)
    {
        FdoIMultiCurvePolygon* multiCurvePolygon = dynamic_cast<FdoIMultiCurvePolygon*>(geometry);
        FdoInt32 count = multiCurvePolygon->GetCount();
        if (count > 0)
        {
            FdoPtr<FdoICurvePolygon> curvePolygon = multiCurvePolygon->GetItem(0);
            vertexOrderRule = CheckPolygonVertexOrder(curvePolygon);
        }

        FdoPtr<FdoCurvePolygonCollection> curvePolygons = FdoCurvePolygonCollection::Create();
        for (FdoInt32 i = 1; i < multiCurvePolygon->GetCount(); i++)
        {
            FdoPtr<FdoICurvePolygon> curvePolygon = multiCurvePolygon->GetItem(i);
            if (vertexOrderRule != CheckPolygonVertexOrder(curvePolygon))
            {
                vertexOrderRule = FdoPolygonVertexOrderRule_None;
                break;
            }
        }
    }
    return vertexOrderRule;
}

