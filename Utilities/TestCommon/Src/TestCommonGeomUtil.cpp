/*
 * 
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
 * Revision Control Modification History
 *
 *         $Id:  $
 *     $Author:  $
 *   $DateTime:  $
 *     $Change:  $
 *
 */

#include "stdafx.h"
#include <FdoSpatial.h>
#include "TestCommonGeomUtil.h"
#include "TestCommonMiscUtil.h"


bool TestCommonGeomUtil::GeometriesEquivalent(FdoIGeometry *geom1, FdoIGeometry *geom2)
{
    bool bRet = true;
    FdoGeometryType type1 = geom1->GetDerivedType();
    FdoGeometryType type2 = geom2->GetDerivedType();
    FdoDimensionality dim1 = (FdoDimensionality)geom1->GetDimensionality();
    FdoDimensionality dim2 = (FdoDimensionality)geom1->GetDimensionality();

    // Make sure types are the same:
    if (type1 != type2)
        return false;

    // Make sure dimensionalities are the same:
    if (dim1 != dim2)
        return false;

    // Make sure contents are the same:
    FdoPtr<FdoIDirectPosition> pos1;
    FdoPtr<FdoIDirectPosition> pos2;
    bool bHasZ = (dim1 & FdoDimensionality_Z) > 0;
    bool bHasM = (dim1 & FdoDimensionality_M) > 0;
    switch (type1)
    {
        case FdoGeometryType_Point:
        {
            FdoIPoint* p1 = (FdoIPoint*)geom1;
            FdoIPoint* p2 = (FdoIPoint*)geom2;
            pos1 = p1->GetPosition();
            pos2 = p2->GetPosition();
            if (!PointsEquivalent(pos1, pos2))
                return false;
        }
        break;

        case FdoGeometryType_LineString:
        {
            FdoILineString* l1 = (FdoILineString*)geom1;
            FdoILineString* l2 = (FdoILineString*)geom2;
            FdoInt32 iCount1 = l1->GetCount();
            FdoInt32 iCount2 = l2->GetCount();
            if (iCount1 != iCount2)
                return false;
            for (FdoInt32 i=0; i<iCount1; i++)
            {
                pos1 = l1->GetItem(i);
                pos2 = l2->GetItem(i);
                if (!PointsEquivalent(pos1, pos2))
                    return false;
            }
        }
        break;

        case FdoGeometryType_Polygon:
        {
            FdoIPolygon* p1 = (FdoIPolygon*)geom1;
            FdoIPolygon* p2 = (FdoIPolygon*)geom2;
            FdoPtr<FdoILinearRing> ring1 = p1->GetExteriorRing();
            FdoPtr<FdoILinearRing> ring2 = p2->GetExteriorRing();
            if (!LinearRingsEquivalent(ring1, ring2))
                return false;
            FdoInt32 iIntRingCount1 = p1->GetInteriorRingCount();
            FdoInt32 iIntRingCount2 = p2->GetInteriorRingCount();
            if (iIntRingCount1 != iIntRingCount2)
                return false;
            for (FdoInt32 i=0; i<iIntRingCount1; i++)
            {
                ring1 = p1->GetInteriorRing(i);
                ring2 = p2->GetInteriorRing(i);
                if (!LinearRingsEquivalent(ring1, ring2))
                    return false;
            }
        }
        break;

        case FdoGeometryType_MultiPoint:
        {
            FdoIMultiPoint* multi1 = (FdoIMultiPoint*)geom1;
            FdoIMultiPoint* multi2 = (FdoIMultiPoint*)geom2;
            FdoInt32 count1 = multi1->GetCount();
            FdoInt32 count2 = multi2->GetCount();
            if (count1 != count2)
                return false;
            for (FdoInt32 i=0; i<count1; i++)
            {
                FdoPtr<FdoIGeometry> item1 = multi1->GetItem(i);
                FdoPtr<FdoIGeometry> item2 = multi2->GetItem(i);
                if (!GeometriesEquivalent(item1,item2))
                    return false;
            }
        }
        break;

        case FdoGeometryType_MultiLineString:
        {
            FdoIMultiLineString* multi1 = (FdoIMultiLineString*)geom1;
            FdoIMultiLineString* multi2 = (FdoIMultiLineString*)geom2;
            FdoInt32 count1 = multi1->GetCount();
            FdoInt32 count2 = multi2->GetCount();
            if (count1 != count2)
                return false;
            for (FdoInt32 i=0; i<count1; i++)
            {
                FdoPtr<FdoIGeometry> item1 = multi1->GetItem(i);
                FdoPtr<FdoIGeometry> item2 = multi2->GetItem(i);
                if (!GeometriesEquivalent(item1,item2))
                    return false;
            }
        }
        break;

        case FdoGeometryType_MultiPolygon:
        {
            FdoIMultiPolygon* multi1 = (FdoIMultiPolygon*)geom1;
            FdoIMultiPolygon* multi2 = (FdoIMultiPolygon*)geom2;
            FdoInt32 count1 = multi1->GetCount();
            FdoInt32 count2 = multi2->GetCount();
            if (count1 != count2)
                return false;
            for (FdoInt32 i=0; i<count1; i++)
            {
                FdoPtr<FdoIGeometry> item1 = multi1->GetItem(i);
                FdoPtr<FdoIGeometry> item2 = multi2->GetItem(i);
                if (!GeometriesEquivalent(item1,item2))
                    return false;
            }
        }
        break;

        case FdoGeometryType_MultiGeometry:
        case FdoGeometryType_CurveString:
        case FdoGeometryType_CurvePolygon:
        case FdoGeometryType_MultiCurveString:
        case FdoGeometryType_MultiCurvePolygon:
        default:
            throw FdoException::Create(L"Don't know how to compare these 2 geometries");
        break;
    }

    return bRet;
}


bool TestCommonGeomUtil::PointsEquivalent(FdoIDirectPosition* pos1, FdoIDirectPosition* pos2)
{
    if (pos1->GetDimensionality() != pos2->GetDimensionality())
        return false;

    bool bHasZ = (pos1->GetDimensionality() & FdoDimensionality_Z) > 0;
    bool bHasM = (pos1->GetDimensionality() & FdoDimensionality_M) > 0;

    if (!TestCommonMiscUtil::FuzzyEqual(pos1->GetX(), pos2->GetX()))
        return false;
    if (!TestCommonMiscUtil::FuzzyEqual(pos1->GetY(), pos2->GetY()))
        return false;
    if (bHasZ && !TestCommonMiscUtil::FuzzyEqual(pos1->GetZ(),pos2->GetZ()))
        return false;
    if (bHasM && !TestCommonMiscUtil::FuzzyEqual(pos1->GetM(),pos2->GetM()))
        return false;

    return true;
}


bool TestCommonGeomUtil::LinearRingsEquivalent(FdoILinearRing* lr1, FdoILinearRing* lr2)
{
    FdoInt32 count1 = lr1->GetCount();
    FdoInt32 count2 = lr2->GetCount();
    if (count1 != count2)
        return false;

    // Compare all the points in the 2 linear rings:
    for (FdoInt32 i=0; i<count1; i++)
    {
        FdoPtr<FdoIDirectPosition> pos1 = lr1->GetItem(i);
        FdoPtr<FdoIDirectPosition> pos2 = lr2->GetItem(i);
        if (!PointsEquivalent(pos1,pos2))
            return false;
    }

    return true;
}



void TestCommonGeomUtil::PrintGeometryAnalysis( const wchar_t *class_name, int index, FdoByteArray * geom_fgf, double length0, double area0, bool verbose)
{
    FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance ();
    FdoIGeometry    *geom = gf->CreateGeometryFromFgf( geom_fgf );

    switch (geom->GetDerivedType())
    {
   	case FdoGeometryType_LineString:
        if (verbose)
            printf("%ls-%d. FdoGeometryType_LineString!\n", class_name, index);
		break;

	case FdoGeometryType_Point:
        if (verbose)
            printf("%ls-%d. FdoGeometryType_Point!\n", class_name, index);
		break;

	case FdoGeometryType_Polygon:
        {
            FdoIPolygon *poly = (FdoIPolygon *)geom;
            FdoILinearRing *extRing = poly->GetExteriorRing();
            double area = FdoSpatialUtility::ComputeLinearRingArea( extRing );
            double length = FdoSpatialUtility::ComputeLinearRingLength( extRing );
            if (verbose)
                printf("%ls-%d. FdoGeometryType_Polygon  length=%lf (%lf) area=%lf (%lf)\n", class_name, index, length, length0, area, area0);
            
            for ( int j = 0; j < poly->GetInteriorRingCount(); j++ )
            {
                FdoILinearRing *intRing = poly->GetInteriorRing(j);   
                area = FdoSpatialUtility::ComputeLinearRingArea( intRing );
                if (verbose)
                    printf("    %d. intPolygon  area=%lf\n", j, area);
                FDO_SAFE_RELEASE(intRing);
            }

            FDO_SAFE_RELEASE(extRing);
        }
		break;

	case FdoGeometryType_MultiPoint:
        if (verbose)
            printf("%ls-%d. FdoGeometryType_MultiPoint!\n", class_name, index);
		break;

	case FdoGeometryType_MultiGeometry:
        if (verbose)
            printf("%ls-%d. FdoGeometryType_MultiGeometry!\n", class_name, index);
		break;

	case FdoGeometryType_MultiLineString:
        if (verbose)
            printf("%ls-%d. FdoGeometryType_MultiLineString!\n", class_name, index);
		break;

	case FdoGeometryType_MultiPolygon:
        {
            if (verbose)
                printf("%ls-%d. FdoGeometryType_MultiPolygon (%lf)\n", class_name, index, area0);

            FdoIMultiPolygon *mpoly = (FdoIMultiPolygon *)geom;
            for (int i = 0; i < mpoly->GetCount(); i++ )
            {
                FdoIPolygon *poly = mpoly->GetItem(i);
                FdoILinearRing *extRing = poly->GetExteriorRing();
                double area = FdoSpatialUtility::ComputeLinearRingArea( extRing );
                double length = FdoSpatialUtility::ComputeLinearRingLength( extRing );
                if (verbose)
                    printf("    %d. extPolygon length=%lf (%lf) area=%lf (%lf) \n", i, length, length0, area, area0);
 
                for ( int j = 0; j < poly->GetInteriorRingCount(); j++ )
                {
                    FdoILinearRing *intRing = poly->GetInteriorRing(j);   
                    area = FdoSpatialUtility::ComputeLinearRingArea( intRing );
                    if (verbose)
                        printf("    %d. intPolygon  area=%lf\n", j, area);
                    FDO_SAFE_RELEASE(intRing);
                }
                FDO_SAFE_RELEASE(poly);
                FDO_SAFE_RELEASE(extRing);
            }
        }
		break;

	case FdoGeometryType_CurveString:
        if (verbose)
            printf("%ls-%d. FdoGeometryType_CurveString!\n", class_name, index);
		break;

	case FdoGeometryType_MultiCurveString:
        if (verbose)
            printf("%ls-%d. FdoGeometryType_MultiCurveString!\n", class_name, index);
		break;

	case FdoGeometryType_CurvePolygon:
        if (verbose)
            printf("%ls-%d. FdoGeometryType_CurvePolygon!\n", class_name, index);
		break;

	case FdoGeometryType_MultiCurvePolygon:
        if (verbose)
            printf("%ls-%d. FdoGeometryType_MultiCurvePolygon!\n", class_name, index);
		break;
 
    default:
        ;
    }

    FDO_SAFE_RELEASE(geom);
}

