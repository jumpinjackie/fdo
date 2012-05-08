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

#include "Pch.h"
#include "SpatialUtilityTest.h"
#include "UnitTestUtil.h"
#include <FdoGeometry.h>
#include <FdoSpatial.h>
#include <math.h>

#ifdef _DEBUG
//#define DEBUG_DETAIL	1
//#define DUMP_TO_GINA_FILE   1
#endif

#ifdef DUMP_TO_GINA_FILE
static FILE * ginaFile_S = NULL;
static long nextFeatNum_S = 2;
#endif

// This fixed-precision equality test allows slight drift.  It's only adequate here
// because we know the floating point numbers' scale in the tests.
// It is used to allow for drift when comparing hard-coded binary numbers with
// ones converted from hard-coded text representation.
#define FUZZY_EQUALS(a, b) (fabs(b-a) < 0.00000001)


CPPUNIT_TEST_SUITE_REGISTRATION (SpatialUtilityTest);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION (SpatialUtilityTest, "SpatialUtilityTest");

SpatialUtilityTest::SpatialUtilityTest(void)
{
}

SpatialUtilityTest::~SpatialUtilityTest(void)
{
}

void SpatialUtilityTest::setUp()
{
}

static char * GeometryValidityToString(FdoSpatialGeometryValidity validity)
{
    char * stringVal = NULL;
    switch(validity)
    {
    case FdoSpatialGeometryValidity_None:
        stringVal = "None";
        break;
    case FdoSpatialGeometryValidity_Valid:
        stringVal = "Valid";
        break;
    case FdoSpatialGeometryValidity_Invalid:
        stringVal = "Invalid";
        break;
    case FdoSpatialGeometryValidity_InvalidButCanBeApproximated:
        stringVal = "InvalidButCanBeApproximated";
        break;
    case FdoSpatialGeometryValidity_InvalidDueToDimensionality:
        stringVal = "InvalidDueToDimensionality";
        break;
    default:
        stringVal = "<Bad Validity value>";
    }
    return stringVal;
}

//////////////////////////////////////////////////////////////////////
// static Dumpers...

#ifdef DUMP_TO_GINA_FILE
static void GinaOpenFile()
{
    if (NULL == ginaFile_S)
    {
        ginaFile_S = fopen("SpatialUtilityTestOutput.gia", "w");
        FDO_CPPUNIT_ASSERT(ginaFile_S != NULL);
        fprintf(ginaFile_S, 
            "udb-start u v 1024\n"
            "udb-header 3.1 FDO_SPATIAL_TEST \"\"\n"
            "extent 1000 1000 10000 10000\n"
            "resolution 1 0.001\n");
        fflush(ginaFile_S);
    }
}
static void GinaStartNewFeature()
{
	char buffer[1000];

	sprintf( buffer, "feat %ld 0 0 prop_bound 4 3 1 l 0.000000 0.000000 0 0 n\n", nextFeatNum_S++);

	fputs( buffer, ginaFile_S );
    fflush(ginaFile_S);
}
static void GinaWritePositions(FdoDirectPositionCollection * positions, bool individualFeatures=false)
{
    FdoInt32 numPositions = positions->GetCount();
    FdoInt32 posIndex = 0;
    FdoInt32 perLine = individualFeatures ? 1 : 3;
    long    sequenceNum = 1;
    while (posIndex < numPositions)
    {
        char    buffer[4000];
        sprintf( buffer, "coor %ld 0 0 %d xy ", nextFeatNum_S, sequenceNum++ );
        for (FdoInt32 i = 0;  posIndex < numPositions && i < perLine;  i++, posIndex++)
        {
            char buffer2[100];
            FdoPtr<FdoIDirectPosition> pos = positions->GetItem(posIndex);
            sprintf(buffer2, "%.14f %.14f ", pos->GetX(), pos->GetY() );
            strcat(buffer, buffer2);
        }
        strcat(buffer, "\n");
        fputs( buffer, ginaFile_S );
        fflush(ginaFile_S);
        if (individualFeatures && posIndex < numPositions)
            GinaStartNewFeature();
    }
}
static void GinaWriteCurveSegmentCollection(
    FdoCurveSegmentCollection * curveSegments,
    bool positionsAsIndividualFeatures)
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
                FdoPtr<FdoDirectPositionCollection> positions = ls->GetPositions();
                GinaWritePositions(positions, positionsAsIndividualFeatures);
            }
            break;
        case FdoGeometryComponentType_CircularArcSegment:
            {
                FdoICircularArcSegment * arc = static_cast<FdoICircularArcSegment *> ((FdoICurveSegmentAbstract *)cs);
                FdoPtr<FdoIDirectPosition> start = arc->GetStartPosition();
                FdoPtr<FdoIDirectPosition> mid = arc->GetMidPoint();
                FdoPtr<FdoIDirectPosition> end = arc->GetEndPosition();
                FdoPtr<FdoDirectPositionCollection> positions = FdoDirectPositionCollection::Create();
                positions->Add(start);
                positions->Add(mid);
                positions->Add(end);
                GinaWritePositions(positions, positionsAsIndividualFeatures);
            }
            break;
        default:
            throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_10_UNSUPPORTEDGEOMETRYTYPE)));
        }
    }
}

#endif // DUMP_TO_GINA_FILE

static void DumpGeometryToGinaFile(FdoIGeometry * geometry, bool positionsAsIndividualFeatures)
{
#ifdef DUMP_TO_GINA_FILE
    GinaOpenFile();

    FdoGeometryType geomType = geometry->GetDerivedType();

    switch (geomType)
    {
	case FdoGeometryType_Point:
        {
            GinaStartNewFeature();
            FdoIPoint * geom = static_cast<FdoIPoint *>(geometry);
            FdoPtr<FdoDirectPositionCollection> positions = FdoDirectPositionCollection::Create();
            FdoPtr<FdoIDirectPosition> pos = geom->GetPosition();
            positions->Add(pos);
            GinaWritePositions(positions, positionsAsIndividualFeatures);
        }
        break;
	case FdoGeometryType_LineString:
        {
            GinaStartNewFeature();
            FdoPtr<FdoDirectPositionCollection> positions = static_cast<FdoILineString *>(geometry)->GetPositions();
            GinaWritePositions(positions, positionsAsIndividualFeatures);
        }
        break;
	case FdoGeometryType_CurveString:
		{
            GinaStartNewFeature();
            FdoICurveString * geom = static_cast<FdoICurveString *> (geometry);

            FdoPtr<FdoCurveSegmentCollection> csc = geom->GetCurveSegments();
            GinaWriteCurveSegmentCollection(csc, positionsAsIndividualFeatures);
		}
		break;
	case FdoGeometryType_Polygon:
		{
            GinaStartNewFeature();
            FdoIPolygon * geom = static_cast<FdoIPolygon *> (geometry);

            FdoPtr<FdoILinearRing> ring = geom->GetExteriorRing();
            FdoPtr<FdoDirectPositionCollection> positions = ring->GetPositions();
            GinaWritePositions(positions, positionsAsIndividualFeatures);

            for (FdoInt32 i = 0;  i < geom->GetInteriorRingCount();  i++)
            {
                ring = geom->GetInteriorRing(i);
                FdoPtr<FdoDirectPositionCollection> positions = ring->GetPositions();
                GinaWritePositions(positions, positionsAsIndividualFeatures);
            }
		}
		break;
	case FdoGeometryType_CurvePolygon:
		{
            GinaStartNewFeature();
            FdoICurvePolygon * geom = static_cast<FdoICurvePolygon *> (geometry);

            FdoPtr<FdoIRing> ring = geom->GetExteriorRing();
            FdoPtr<FdoCurveSegmentCollection> csc = ring->GetCurveSegments();
            GinaWriteCurveSegmentCollection(csc, positionsAsIndividualFeatures);

            for (FdoInt32 i = 0;  i < geom->GetInteriorRingCount();  i++)
            {
                ring = geom->GetInteriorRing(i);
                csc = ring->GetCurveSegments();
                GinaWriteCurveSegmentCollection(csc, positionsAsIndividualFeatures);
            }
		}
		break;
	case FdoGeometryType_MultiPoint:
	case FdoGeometryType_MultiGeometry:
	case FdoGeometryType_MultiLineString:
	case FdoGeometryType_MultiCurveString:
	case FdoGeometryType_MultiPolygon:
	case FdoGeometryType_MultiCurvePolygon:
        {
            FdoIGeometricAggregateAbstract * multiGeom =
                static_cast<FdoIGeometricAggregateAbstract *>(geometry);
            FdoInt32 numSubGeometries = multiGeom->GetCount();

            for (FdoInt32 i = 0;  i < numSubGeometries;  i++)
            {
                FdoPtr<FdoIGeometry> subGeom;
                switch (geomType)
                {
                case FdoGeometryType_MultiPoint:        subGeom = static_cast<FdoIMultiPoint *>(multiGeom)->GetItem(i);  break;
	            case FdoGeometryType_MultiGeometry:     subGeom = static_cast<FdoIMultiGeometry *>(multiGeom)->GetItem(i);  break;
	            case FdoGeometryType_MultiLineString:   subGeom = static_cast<FdoIMultiLineString *>(multiGeom)->GetItem(i);  break;
	            case FdoGeometryType_MultiCurveString:  subGeom = static_cast<FdoIMultiCurveString *>(multiGeom)->GetItem(i);  break;
	            case FdoGeometryType_MultiPolygon:      subGeom = static_cast<FdoIMultiPolygon *>(multiGeom)->GetItem(i);  break;
	            case FdoGeometryType_MultiCurvePolygon: subGeom = static_cast<FdoIMultiCurvePolygon *>(multiGeom)->GetItem(i);  break;
	            default: throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_10_UNSUPPORTEDGEOMETRYTYPE)));
                }
                DumpGeometryToGinaFile(subGeom, positionsAsIndividualFeatures);  // Recurse for sub-geometries.
            }
        }
        break;
	default:
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_10_UNSUPPORTEDGEOMETRYTYPE)));
	}
#endif // DUMP_TO_GINA_FILE
}


void static DumpPosition(FdoString* context, FdoIDirectPosition* p)
{
#ifdef DEBUG_DETAIL
	if (p->GetDimensionality() == FdoDimensionality_XY)
		printf(" %S XY=(%g,%g)\n", context, p->GetX(), p->GetY());
	else if (p->GetDimensionality() == FdoDimensionality_Z)
		printf(" %S XYZ=(%g,%g,%g)\n", context, p->GetX(), p->GetY(), p->GetZ());
	else if (p->GetDimensionality() == (FdoDimensionality_XY|FdoDimensionality_M))
		printf(" %S XYM=(%g,%g,%g)\n", context, p->GetX(), p->GetY(), p->GetM());
	else if (p->GetDimensionality() == (FdoDimensionality_Z|FdoDimensionality_M))
		printf(" %S XYZM=(%g,%g,%g,%g)\n", context, p->GetX(), p->GetY(), p->GetZ(), p->GetM());
	else
		CPPUNIT_ASSERT_MESSAGE("DumpPosition::UNKNOWN_DIMENSION_TYPE!\n", false);
#endif
}

void static DumpEnvelope(FdoIEnvelope* env)
{
#ifdef DEBUG_DETAIL
	printf(" Envelope=min(%g,%g,%g) max(%g,%g,%g)\n",
		env->GetMinX(), env->GetMinY(), env->GetMinZ(),
		env->GetMaxX(), env->GetMaxY(), env->GetMaxZ());
#endif
}

// static Dump FGFT
void static DumpFGFT(FdoIGeometry* p)
{
#ifdef DEBUG_DETAIL
    printf("FGFT = %S\n", p->GetText());
#endif
}

void static DumpLineString(FdoILineString* line)
{
#ifdef DEBUG_DETAIL
	printf("%S:count(%d) dimtype(%d) derivedtype(%d) closed(%d)\n",
		L"LINESTRING", line->GetCount(), line->GetDimensionality(), line->GetDerivedType(), line->GetIsClosed());

	FdoPtr<FdoIDirectPosition> pStart = line->GetStartPosition();
	DumpPosition(L"", pStart);

	FdoPtr<FdoIDirectPosition> pEnd = line->GetEndPosition();
	DumpPosition(L"", pEnd);

	FdoPtr<FdoIEnvelope> envl = line->GetEnvelope();
	DumpEnvelope(envl);

	for (FdoInt32 i=0; i<line->GetCount(); i++)
	{
		FdoPtr<FdoIDirectPosition> pos = line->GetItem(i);
		DumpPosition(L"", pos);
	}
	DumpFGFT(line);
#endif
}

void static DumpLinearRing(FdoILinearRing* lRing)
{
#ifdef DEBUG_DETAIL
	printf("%S:count(%d) dimtype(%d)\n",
		L"(LINEARRING)", lRing->GetCount(), lRing->GetDimensionality());

	FdoPtr<FdoIEnvelope> envl = lRing->GetEnvelope();
	DumpEnvelope(envl);

	for (FdoInt32 i=0; i<lRing->GetCount(); i++)
	{
		FdoPtr<FdoIDirectPosition> pos = lRing->GetItem(i);
		DumpPosition(L"", pos);
	}
#endif
}

void static DumpLineStringSegment(FdoILineStringSegment* lSeg)
{
#ifdef DEBUG_DETAIL
	printf("%S:count(%d) dimtype(%d) derivedtype(%d) closed(%d)\n",
		L"(LINESTRINGSEGMENT)", lSeg->GetCount(), lSeg->GetDimensionality(), lSeg->GetDerivedType(), lSeg->GetIsClosed());

	FdoPtr<FdoIDirectPosition> pStart = lSeg->GetStartPosition();
	DumpPosition(L"", pStart);

	FdoPtr<FdoIDirectPosition> pEnd = lSeg->GetEndPosition();
	DumpPosition(L"", pEnd);

	FdoPtr<FdoIEnvelope> envl = lSeg->GetEnvelope();
	DumpEnvelope(envl);

	for (FdoInt32 i=0; i<lSeg->GetCount(); i++)
	{
		FdoPtr<FdoIDirectPosition> pos = lSeg->GetItem(i);
		DumpPosition(L"", pos);
	}
#endif
}

void static DumpPolygon(FdoIPolygon* polygon)
{
#ifdef DEBUG_DETAIL
	printf("%S:interiorRingCount(%d) dimtype(%d) derivedtype(%d)\n",
		L"POLYGON", polygon->GetInteriorRingCount(), polygon->GetDimensionality(), polygon->GetDerivedType());

	FdoPtr<FdoILinearRing> extRing = polygon->GetExteriorRing();
	DumpLinearRing(extRing);

	for (FdoInt32 i = 0; i<polygon->GetInteriorRingCount(); i++)
	{
		FdoPtr<FdoILinearRing> intRing = polygon->GetInteriorRing(i);
		DumpLinearRing(intRing);
	}

	FdoPtr<FdoIEnvelope> envl = polygon->GetEnvelope();
	DumpEnvelope(envl);

    DumpFGFT(polygon);
#endif
}

void static DumpGeometry(FdoIGeometry* geometry)
{
#ifdef DEBUG_DETAIL
	printf("%S: dimtype(%d) derivedtype(%d)\n",
		L"GEOMETRY", geometry->GetDimensionality(), geometry->GetDerivedType());

    DumpFGFT(geometry);
#endif
}

void static DumpPoint(FdoIPoint* pnt)
{
#ifdef DEBUG_DETAIL
	printf("%S:dimtype(%d) derivedtype(%d)\n",
		L"POINT", pnt->GetDimensionality(), pnt->GetDerivedType());

	FdoPtr<FdoIEnvelope> envl = pnt->GetEnvelope();
	DumpEnvelope(envl);

	FdoPtr<FdoIDirectPosition> pos = pnt->GetPosition();
	DumpPosition(L"", pos);

    DumpFGFT(pnt);
#endif
}

void static DumpMultiPoint(FdoIMultiPoint* multiPnt)
{
#ifdef DEBUG_DETAIL
	printf("%S:count(%d) dimtype(%d) derivedtype(%d)\n",
		L"MULTIPOINT", multiPnt->GetCount(), multiPnt->GetDimensionality(), multiPnt->GetDerivedType());

	FdoPtr<FdoIEnvelope> envl = multiPnt->GetEnvelope();
	DumpEnvelope(envl);

	for (FdoInt32 i=0; i<multiPnt->GetCount(); i++)
	{
		FdoPtr<FdoIPoint> pnt = multiPnt->GetItem(i);
		DumpPoint(pnt);
	}
    DumpFGFT(multiPnt);
#endif
}

void static DumpMultiLineString(FdoIMultiLineString* multiLine)
{
#ifdef DEBUG_DETAIL
	printf("%S:count(%d) dimtype(%d) derivedtype(%d)\n",
		L"MULTILINESTRING", multiLine->GetCount(), multiLine->GetDimensionality(), multiLine->GetDerivedType());

	FdoPtr<FdoIEnvelope> envl = multiLine->GetEnvelope();
	DumpEnvelope(envl);

	for (FdoInt32 i=0; i<multiLine->GetCount(); i++)
	{
		FdoPtr<FdoILineString> lineString = multiLine->GetItem(i);
		DumpLineString(lineString);
	}
    DumpFGFT(multiLine);
#endif
}

void static DumpMultiPolygon(FdoIMultiPolygon* multiPoly)
{
#ifdef DEBUG_DETAIL
	printf("%S:count(%d) dimtype(%d) derivedtype(%d)\n",
		L"MULTIPOLYGON", multiPoly->GetCount(), multiPoly->GetDimensionality(), multiPoly->GetDerivedType());

	FdoPtr<FdoIEnvelope> envl = multiPoly->GetEnvelope();
	DumpEnvelope(envl);

	for (FdoInt32 i=0; i<multiPoly->GetCount(); i++)
	{
		FdoPtr<FdoIPolygon> polygon = multiPoly->GetItem(i);
		DumpPolygon(polygon);
	}
    DumpFGFT(multiPoly);
#endif
}

void static DumpCircularArcSegment(FdoICircularArcSegment* arcSeg)
{
#ifdef DEBUG_DETAIL
	printf("%S:dimtype(%d) derivedtype(%d) closed(%d)\n",
		L"(CIRCULARARCSEGMENT)", arcSeg->GetDimensionality(), arcSeg->GetDerivedType(), arcSeg->GetIsClosed());

	FdoPtr<FdoIEnvelope> envl = arcSeg->GetEnvelope();
	DumpEnvelope(envl);

	FdoPtr<FdoIDirectPosition> startPos = arcSeg->GetStartPosition();
	DumpPosition(L"", startPos);

	FdoPtr<FdoIDirectPosition> midPos = arcSeg->GetMidPoint();
	DumpPosition(L"", midPos);

	FdoPtr<FdoIDirectPosition> endPos = arcSeg->GetEndPosition();
	DumpPosition(L"", endPos);
#endif
}


void static DumpCurveString(FdoICurveString* curveString)
{
#ifdef DEBUG_DETAIL
	printf("%S:count(%d) dimtype(%d) derivedtype(%d) closed(%d)\n",
		L"CURVESTRING", curveString->GetCount(), curveString->GetDimensionality(), curveString->GetDerivedType(), curveString->GetIsClosed());

	FdoPtr<FdoIDirectPosition> startPos = curveString->GetStartPosition();
	DumpPosition(L"StartPoint", startPos);

	FdoPtr<FdoIDirectPosition> endPos = curveString->GetEndPosition();
	DumpPosition(L"EndPoint", endPos);

	FdoPtr<FdoIEnvelope> envl = curveString->GetEnvelope();
	DumpEnvelope(envl);

	for (FdoInt32 i=0; i<curveString->GetCount(); i++)
	{
		FdoPtr<FdoICurveSegmentAbstract> seg = curveString->GetItem(i);
		switch(seg->GetDerivedType())
		{
		case FdoGeometryComponentType_LineStringSegment:
			{
				DumpLineStringSegment((FdoILineStringSegment*)(seg.p));
				break;
			}
		case FdoGeometryComponentType_CircularArcSegment:
			{
				DumpCircularArcSegment((FdoICircularArcSegment*)(seg.p));
				break;
			}
		default:
			throw exception();
		} // of switch
	} // of for

    DumpFGFT(curveString);
#endif
}

void static DumpRing(FdoIRing* ring)
{
#ifdef DEBUG_DETAIL
	printf("%S:count(%d) dimtype(%d)\n",
		L"(RING)", ring->GetCount(), ring->GetDimensionality());

	FdoPtr<FdoIEnvelope> envl = ring->GetEnvelope();
	DumpEnvelope(envl);

	for (FdoInt32 i=0; i<ring->GetCount(); i++)
	{
		FdoPtr<FdoICurveSegmentAbstract> seg = ring->GetItem(i);
		switch(seg->GetDerivedType())
		{
		case FdoGeometryComponentType_LineStringSegment:
			{
				DumpLineStringSegment((FdoILineStringSegment*)(seg.p));
				break;
			}
		case FdoGeometryComponentType_CircularArcSegment:
			{
				DumpCircularArcSegment((FdoICircularArcSegment*)(seg.p));
				break;
			}
		default:
			throw exception();
		} // of switch
	} // of for

#endif
}

void static DumpCurvePolygon(FdoICurvePolygon* curvePoly)
{
#ifdef DEBUG_DETAIL
	printf("%S:interiorRingCount(%d) dimtype(%d) derivedtype(%d)\n",
		L"CURVEPOLYGON", curvePoly->GetInteriorRingCount(), curvePoly->GetDimensionality(), curvePoly->GetDerivedType());

	FdoPtr<FdoIRing> extRing = curvePoly->GetExteriorRing();
	DumpRing(extRing);

	for (FdoInt32 i=0; i<curvePoly->GetInteriorRingCount(); i++)
	{
		FdoPtr<FdoIRing> ring = curvePoly->GetInteriorRing(i);
		DumpRing(ring);
	}

	FdoPtr<FdoIEnvelope> envl = curvePoly->GetEnvelope();
	DumpEnvelope(envl);

    DumpFGFT(curvePoly);
#endif
}

void static DumpMultiCurvePolygon(FdoIMultiCurvePolygon* multiCurvePoly)
{
#ifdef DEBUG_DETAIL
	printf("%S:count(%d) dimtype(%d) derivedtype(%d)\n",
		L"MULTICURVEPOLYGON", multiCurvePoly->GetCount(), multiCurvePoly->GetDimensionality(), multiCurvePoly->GetDerivedType());

	for (FdoInt32 i=0; i<multiCurvePoly->GetCount(); i++)
	{
		FdoPtr<FdoICurvePolygon> curvePoly = multiCurvePoly->GetItem(i);
		DumpCurvePolygon(curvePoly);
	}

	FdoPtr<FdoIEnvelope> envl = multiCurvePoly->GetEnvelope();
	DumpEnvelope(envl);
    DumpFGFT(multiCurvePoly);
#endif
}

void static DumpMultiCurveString(FdoIMultiCurveString* multiCurveString)
{
#ifdef DEBUG_DETAIL
	printf("%S:count(%d) dimtype(%d) derivedtype(%d)\n",
		L"MULTICURVESTRING", multiCurveString->GetCount(), multiCurveString->GetDimensionality(), multiCurveString->GetDerivedType());

	for (FdoInt32 i=0; i<multiCurveString->GetCount(); i++)
	{
		FdoPtr<FdoICurveString> curveString = multiCurveString->GetItem(i);
		DumpCurveString(curveString);
	}

	FdoPtr<FdoIEnvelope> envl = multiCurveString->GetEnvelope();
	DumpEnvelope(envl);
    DumpFGFT(multiCurveString);
#endif
}

void static DumpMultiGeometry(FdoIMultiGeometry* multiGeometry)
{
#ifdef DEBUG_DETAIL
	printf("%S:count(%d) dimtype(%d) derivedtype(%d))",
		L"MULTIGEOMETRY", multiGeometry->GetCount(), multiGeometry->GetDimensionality(), multiGeometry->GetDerivedType());

	FdoPtr<FdoIEnvelope> envl = multiGeometry->GetEnvelope();
	DumpEnvelope(envl);

	for (FdoInt32 i=0; i<multiGeometry->GetCount(); i++)
	{
		FdoPtr<FdoIGeometry> geometry = multiGeometry->GetItem(i);
        switch (geometry->GetDerivedType())
        {
	    case FdoGeometryType_LineString:
		    DumpLineString((FdoILineString *)(geometry.p));
		    break;

	    case FdoGeometryType_Point:
		    DumpPoint((FdoIPoint*)(geometry.p));
		    break;

	    case FdoGeometryType_Polygon:
		    DumpPolygon((FdoIPolygon*)(geometry.p));
		    break;

	    case FdoGeometryType_CurveString:
		    DumpCurveString((FdoICurveString*)(geometry.p));
		    break;

	    case FdoGeometryType_CurvePolygon:
		    DumpCurvePolygon((FdoICurvePolygon*)(geometry.p));
		    break;

        default:
        	{
				char szError[100];
				sprintf(szError, "MultiGeometry test does not support item type of %d",
					(int)geometry->GetDerivedType());
				CPPUNIT_ASSERT_MESSAGE(szError, false);
			 }
        }
	}

    DumpFGFT(multiGeometry);
#endif
}

//////////////////////////////////////////////////////////////////////
// static Checkers...
//////////////////////////////////////////////////////////////////////

void static CheckPositionXY(FdoIDirectPosition* pos, double x, double y)
{
	CPPUNIT_ASSERT_MESSAGE("X value mismatch!", pos->GetX() == x);
	CPPUNIT_ASSERT_MESSAGE("Y value mismatch!", pos->GetY() == y);
}

void static CheckPositionXYM(FdoIDirectPosition* pos, double x, double y, double m)
{
	CheckPositionXY(pos, x, y);
	CPPUNIT_ASSERT_MESSAGE("M value mismatch!", pos->GetM() == m);
}

void static CheckPositionXYZ(FdoIDirectPosition* pos, double x, double y, double z)
{
	CheckPositionXY(pos, x, y);
	CPPUNIT_ASSERT_MESSAGE("Z value mismatch!", pos->GetZ() == z);
}

void static CheckPositionXYZM(FdoIDirectPosition* pos, double x, double y, double z, double m)
{
	CheckPositionXYZ(pos, x, y, z);
	CPPUNIT_ASSERT_MESSAGE("M value mismatch!", pos->GetM() == m);
}

void static CheckEnvelope(FdoIEnvelope* envl, double* ordsXY)
{
	CPPUNIT_ASSERT_MESSAGE("MinX mismatch!", envl->GetMinX() == ordsXY[0]);
	CPPUNIT_ASSERT_MESSAGE("MinY mismatch!", envl->GetMinY() == ordsXY[1]);
	CPPUNIT_ASSERT_MESSAGE("MaxX mismatch!", envl->GetMaxX() == ordsXY[2]);
	CPPUNIT_ASSERT_MESSAGE("MaxY mismatch!", envl->GetMaxY() == ordsXY[3]);
}

void static CheckEnvelope(FdoIEnvelope* envl, FdoIDirectPosition* pos1, FdoIDirectPosition* pos2)
{
	CPPUNIT_ASSERT_MESSAGE("MinX mismatch!", envl->GetMinX() == pos1->GetX());
	CPPUNIT_ASSERT_MESSAGE("MinY mismatch!", envl->GetMinY() == pos1->GetY());
    CPPUNIT_ASSERT_MESSAGE("MinZ mismatch!", (pos1->GetDimensionality() & FdoDimensionality_Z) == 0 || envl->GetMinZ() == pos1->GetZ());
	CPPUNIT_ASSERT_MESSAGE("MaxX mismatch!", envl->GetMaxX() == pos2->GetX());
	CPPUNIT_ASSERT_MESSAGE("MaxY mismatch!", envl->GetMaxY() == pos2->GetY());
	CPPUNIT_ASSERT_MESSAGE("MaxZ mismatch!", (pos2->GetDimensionality() & FdoDimensionality_Z) == 0 || envl->GetMaxZ() == pos2->GetZ());
}

void static CheckEnvelopeXY(FdoIEnvelope* envl, double minx, double miny, double maxx, double maxy)
{
	CPPUNIT_ASSERT_MESSAGE("MinX mismatch!", FUZZY_EQUALS(envl->GetMinX(), minx));
	CPPUNIT_ASSERT_MESSAGE("MinY mismatch!", FUZZY_EQUALS(envl->GetMinY(), miny));
	CPPUNIT_ASSERT_MESSAGE("MaxX mismatch!", FUZZY_EQUALS(envl->GetMaxX(), maxx));
	CPPUNIT_ASSERT_MESSAGE("MaxY mismatch!", FUZZY_EQUALS(envl->GetMaxY(), maxy));
}

void static CheckEnvelopeXYZ(FdoIEnvelope* envl, double minx, double miny, double minz, double maxx, double maxy, double maxz)
{
	CheckEnvelopeXY(envl, minx, miny, maxx, maxy);
	CPPUNIT_ASSERT_MESSAGE("MinZ mismatch!", envl->GetMinZ() == minz);
	CPPUNIT_ASSERT_MESSAGE("MaxZ mismatch!", envl->GetMaxZ() == maxz);
}

void static CheckEqualEnvelopes(FdoIEnvelope* envl1, FdoIEnvelope* envl2)
{
	CPPUNIT_ASSERT_MESSAGE("Envelope MinX mismatch!", envl1->GetMinX() == envl2->GetMinX());
	CPPUNIT_ASSERT_MESSAGE("Envelope MinY mismatch!", envl1->GetMinY() == envl2->GetMinY());
	CPPUNIT_ASSERT_MESSAGE("Envelope MinZ mismatch!", envl1->GetMinZ() == envl2->GetMinZ());
	CPPUNIT_ASSERT_MESSAGE("Envelope MaxX mismatch!", envl1->GetMaxX() == envl2->GetMaxX());
	CPPUNIT_ASSERT_MESSAGE("Envelope MaxY mismatch!", envl1->GetMaxY() == envl2->GetMaxY());
	CPPUNIT_ASSERT_MESSAGE("Envelope MaxZ mismatch!", envl1->GetMaxZ() == envl2->GetMaxZ());
}

//TODO: use parens in FGFT formatter
void static CheckFGFT(FdoIGeometry* geom, FdoString* fgft, FdoInt32 parens=2);
void static CheckFGFT(FdoIGeometry* geom, FdoString* fgft, FdoInt32 parens)
{
	DumpFGFT(geom);

    char    szError[4000+4000+30];
    FdoString * geomText = geom->GetText();
    sprintf(szError, "FGFT mismatch!\n<%S> should be <%S>\n", geomText, fgft);
	CPPUNIT_ASSERT_MESSAGE(szError, wcscmp(geomText, fgft) == 0);
}

void static CheckGeometryValidity(
    FdoSpatialGeometryValidity validity1,
    FdoSpatialGeometryValidity validity2)
{
	CPPUNIT_ASSERT_MESSAGE("Geometry Validity mismatch.", validity1 == validity2);
}

//////////////////////////////////////////////////////////////////////
// Helpers to create repetitive geometries

static FdoICurveString* CreateCurveString(double offset)
{
	// Create and return a curvestring consisting of
	// one circulararcsegment and one linearstringsegment

	// arcseg  = (0,0), (0,1), (1,2)
	// lineseg = (1,2), (3,0), (3,2)

	FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();

	FdoPtr<FdoIDirectPosition> startPos = gf->CreatePositionXY(offset+0.0, offset+0.0);
	FdoPtr<FdoIDirectPosition> midPos = gf->CreatePositionXY(offset+0.0, offset+1.0);
	FdoPtr<FdoIDirectPosition> endPos = gf->CreatePositionXY(offset+1.0, offset+2.0);

	FdoPtr<FdoICircularArcSegment> arcSeg = gf->CreateCircularArcSegment(startPos, midPos, endPos);

    FdoPtr<FdoDirectPositionCollection> points = FdoDirectPositionCollection::Create();
	FdoPtr<FdoIDirectPosition> pt1 = gf->CreatePositionXY(offset+1.0, offset+2.0);
	FdoPtr<FdoIDirectPosition> pt2 = gf->CreatePositionXY(offset+3.0, offset+0.0);
	FdoPtr<FdoIDirectPosition> pt3 = gf->CreatePositionXY(offset+3.0, offset+2.0);
    points->Add(pt1);
    points->Add(pt2);
	points->Add(pt3);

	FdoPtr<FdoILineStringSegment> lineSeg = gf->CreateLineStringSegment(points);

    FdoPtr<FdoCurveSegmentCollection> curveSegs = FdoCurveSegmentCollection::Create();
    curveSegs->Add(arcSeg);
    curveSegs->Add(lineSeg);

	FdoPtr<FdoICurveString> curveString = gf->CreateCurveString(curveSegs);

    FDO_SAFE_ADDREF(curveString.p);
	return curveString;
}

static FdoIRing* CreateRing(double offset)
{
	// Ring is a closed entity.
	// Create and return a ring consisting of
	// one circulararcsegment and one linearstringsegment

	// arcseg  = (0,0), (0,1), (1,2)
	// lineseg = (1,2), (0,0)

	FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();

	FdoPtr<FdoIDirectPosition> startPos = gf->CreatePositionXY(offset+0.0, offset+0.0);
	FdoPtr<FdoIDirectPosition> midPos = gf->CreatePositionXY(offset+0.0, offset+1.0);
	FdoPtr<FdoIDirectPosition> endPos = gf->CreatePositionXY(offset+1.0, offset+2.0);

	FdoPtr<FdoICircularArcSegment> arcSeg = gf->CreateCircularArcSegment(startPos, midPos, endPos);

    FdoPtr<FdoDirectPositionCollection > points = FdoDirectPositionCollection::Create();
	FdoPtr<FdoIDirectPosition> fromPt = gf->CreatePositionXY(offset+1.0, offset+2.0);
	FdoPtr<FdoIDirectPosition> toPt = gf->CreatePositionXY(offset+0.0, offset+0.0);
    points->Add(fromPt);
    points->Add(toPt);

	FdoPtr<FdoILineStringSegment> lineSeg = gf->CreateLineStringSegment(points);

    FdoPtr<FdoCurveSegmentCollection> curveSegs = FdoCurveSegmentCollection::Create();
	curveSegs->Add(arcSeg);
	curveSegs->Add(lineSeg);

	FdoPtr<FdoIRing> ring = gf->CreateRing(curveSegs);

    FDO_SAFE_ADDREF(ring.p);
	return ring;
}

static FdoICurvePolygon* CreateCurvePolygon(double offset)
{
	FdoPtr<FdoIRing> extRing = CreateRing(offset + 100);

	FdoInt32 numIntRings = 2;

    FdoPtr<FdoRingCollection> intRings = FdoRingCollection::Create();

	FdoPtr<FdoIRing> ring1 = CreateRing(offset + 200);
	FdoPtr<FdoIRing> ring2 = CreateRing(offset + 300);
	intRings->Add(ring1);
	intRings->Add(ring2);

	FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();

	FdoPtr<FdoICurvePolygon> curvePoly = gf->CreateCurvePolygon(extRing, intRings);

    FDO_SAFE_ADDREF(curvePoly.p);
	return curvePoly;
}

static FdoIMultiCurvePolygon* CreateMultiCurvePolygon(FdoInt32 numCurvePolys, double offset)
{
    FdoPtr<FdoCurvePolygonCollection> curvePolys = FdoCurvePolygonCollection::Create();

	for (FdoInt32 i=0; i<numCurvePolys; i++)
	{
		FdoPtr<FdoICurvePolygon> curvePoly = CreateCurvePolygon(i+offset);
		curvePolys->Add(curvePoly);
	}

	FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();

	FdoPtr<FdoIMultiCurvePolygon> multiCurvePoly = gf->CreateMultiCurvePolygon(curvePolys);

    FDO_SAFE_ADDREF(multiCurvePoly.p);
	return multiCurvePoly;
}

static FdoIMultiGeometry* CreateMultiGeometry()
{
    FdoPtr<FdoGeometryCollection> geometries = FdoGeometryCollection::Create();

	FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();

	FdoPtr<FdoIGeometry> geometry = NULL;

    // CurvePolygon
    geometry = CreateCurvePolygon(0);
	geometries->Add(geometry);

    // CurveString
    // Not doing CurveString because of some unfixed defect.
    // It may be the same one that sometimes affects MultiPolygon.
    geometry = CreateCurveString(100);
	geometries->Add(geometry);

    // LineString
	double ordsXY[6];
	ordsXY[0] = 0.0; ordsXY[1] = 1.0;
	ordsXY[2] = 2.0; ordsXY[3] = 3.0;
	ordsXY[4] = 4.0; ordsXY[5] = 5.0;
	geometry = gf->CreateLineString(FdoDimensionality_XY, 6, ordsXY);
	geometries->Add(geometry);

    // Point
	double ordsXYZ[3];
	ordsXYZ[0] = 5.0; ordsXYZ[1] = 3.0; ordsXYZ[2] = 2.0;
	geometry = gf->CreatePoint(FdoDimensionality_XY|FdoDimensionality_Z, ordsXYZ);
	geometries->Add(geometry);

    // Polygon
    // Not doing Polygon because of some unfixed defect.
    // It may be the same one that sometimes affects MultiPolygon.
	double ordsXYExt[10];
	ordsXYExt[0] = 0.0; ordsXYExt[1] = 0.0;
	ordsXYExt[2] = 5.0; ordsXYExt[3] = 0.0;
	ordsXYExt[4] = 5.0; ordsXYExt[5] = 5.0;
	ordsXYExt[6] = 0.0; ordsXYExt[7] = 5.0;
	ordsXYExt[8] = 0.0; ordsXYExt[9] = 0.0;
	double ordsXYInt1[8];
	ordsXYInt1[0] = 1.0; ordsXYInt1[1] = 1.0;
	ordsXYInt1[2] = 2.0; ordsXYInt1[3] = 1.0;
	ordsXYInt1[4] = 2.0; ordsXYInt1[5] = 2.0;
	ordsXYInt1[6] = 1.0; ordsXYInt1[7] = 1.0;
	double ordsXYInt2[8];
	ordsXYInt2[0] = 3.0; ordsXYInt2[1] = 3.0;
	ordsXYInt2[2] = 4.0; ordsXYInt2[3] = 3.0;
	ordsXYInt2[4] = 4.0; ordsXYInt2[5] = 4.0;
	ordsXYInt2[6] = 3.0; ordsXYInt2[7] = 3.0;
	FdoPtr<FdoILinearRing> extRing = gf->CreateLinearRing(FdoDimensionality_XY, 10, ordsXYExt);
	FdoPtr<FdoILinearRing> intRing1 = gf->CreateLinearRing(FdoDimensionality_XY, 8, ordsXYInt1);
	FdoPtr<FdoILinearRing> intRing2 = gf->CreateLinearRing(FdoDimensionality_XY, 8, ordsXYInt2);
    FdoPtr<FdoLinearRingCollection> intRings = FdoLinearRingCollection::Create();
	intRings->Add(intRing1);
	intRings->Add(intRing2);
	geometry = gf->CreatePolygon(extRing, intRings);
	geometries->Add(geometry);

    // Make MultiGeometry from the many geometries collected above.
	FdoPtr<FdoIMultiGeometry> multiGeometry = gf->CreateMultiGeometry(geometries);

    FDO_SAFE_ADDREF(multiGeometry.p);
	return multiGeometry;
}

//////////////////////////////////////////////////////////////////////
// Test geometry type validation...

void SpatialUtilityTest::testValidateGeometryByType()
{
    FdoPtr<FdoIGeometry> geomA = CreateCurvePolygon(0);

    FdoGeometryType      typesA[] =
    {
        FdoGeometryType_Point,
        FdoGeometryType_LineString,
        FdoGeometryType_Polygon,
        FdoGeometryType_MultiPoint,
        FdoGeometryType_MultiLineString,
        FdoGeometryType_MultiPolygon,
        FdoGeometryType_MultiGeometry,
        FdoGeometryType_CurveString,
        FdoGeometryType_CurvePolygon,
        FdoGeometryType_MultiCurveString,
        FdoGeometryType_MultiCurvePolygon
    };
    FdoInt32 numTypesA = sizeof(typesA)/sizeof(typesA[0]);

    FdoGeometryComponentType    compTypesA[] =
    {
        FdoGeometryComponentType_CircularArcSegment,
        FdoGeometryComponentType_LineStringSegment,
        FdoGeometryComponentType_LinearRing,
        FdoGeometryComponentType_Ring
    };
    FdoInt32 numCompTypesA = sizeof(compTypesA)/sizeof(compTypesA[0]);

    FdoInt32 dimensionalityA = 
        FdoDimensionality_XY|FdoDimensionality_Z|FdoDimensionality_M;

    FdoSpatialGeometryValidity validity =
        FdoSpatialUtility::ValidateGeometryByType(
            geomA,
            numTypesA, typesA,
            numCompTypesA, compTypesA,
            dimensionalityA);

    printf("validity=%s\n", GeometryValidityToString(validity));
    CheckGeometryValidity(validity, FdoSpatialGeometryValidity_Valid);

    // See if lack of arc support is noticed.

    FdoGeometryComponentType    compTypesB[] =
    {
        FdoGeometryComponentType_LineStringSegment,
        FdoGeometryComponentType_LinearRing
    };
    FdoInt32 numCompTypesB = sizeof(compTypesB)/sizeof(compTypesB[0]);

    validity =
        FdoSpatialUtility::ValidateGeometryByType(
            geomA,
            numTypesA, typesA,
            numCompTypesB, compTypesB,
            dimensionalityA);

    printf("validity=%s\n", GeometryValidityToString(validity));
    CheckGeometryValidity(validity, FdoSpatialGeometryValidity_InvalidButCanBeApproximated);

    // Try it without the needed approximation type, "Polygon".

    FdoGeometryType      typesB[] =
    {
        FdoGeometryType_Point,
        FdoGeometryType_LineString,
        //FdoGeometryType_Polygon,
        FdoGeometryType_MultiPoint,
        FdoGeometryType_MultiLineString,
        FdoGeometryType_MultiPolygon,
        FdoGeometryType_MultiGeometry,
        FdoGeometryType_CurveString,
        FdoGeometryType_CurvePolygon,
        FdoGeometryType_MultiCurveString,
        FdoGeometryType_MultiCurvePolygon
    };
    FdoInt32 numTypesB = sizeof(typesB)/sizeof(typesB[0]);

    validity =
        FdoSpatialUtility::ValidateGeometryByType(
            geomA,
            numTypesB, typesB,
            numCompTypesB, compTypesB,
            dimensionalityA);

    printf("validity=%s\n", GeometryValidityToString(validity));
    CheckGeometryValidity(validity, FdoSpatialGeometryValidity_Invalid);

    // Test dimensionality failure.

	FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
    FdoPtr<FdoIGeometry> geomB = gf->CreateGeometry(L"POINT XYZ (5 3 2)");
    FdoInt32 dimensionalityB = 
        FdoDimensionality_XY;

    validity =
        FdoSpatialUtility::ValidateGeometryByType(
            geomB,
            numTypesA, typesA,
            numCompTypesA, compTypesA,
            dimensionalityB);

    printf("validity=%s\n", GeometryValidityToString(validity));
    CheckGeometryValidity(validity, FdoSpatialGeometryValidity_InvalidDueToDimensionality);

    // Check that a failure due to dimensionality has precedence over a failure due
    // to typing.

    FdoGeometryType      typesC[] =
    {
        //FdoGeometryType_Point,
        FdoGeometryType_LineString,
        FdoGeometryType_Polygon,
        FdoGeometryType_MultiPoint,
        FdoGeometryType_MultiLineString,
        FdoGeometryType_MultiPolygon,
        FdoGeometryType_MultiGeometry,
        FdoGeometryType_CurveString,
        FdoGeometryType_CurvePolygon,
        FdoGeometryType_MultiCurveString,
        FdoGeometryType_MultiCurvePolygon
    };
    FdoInt32 numTypesC = sizeof(typesC)/sizeof(typesC[0]);

    validity =
        FdoSpatialUtility::ValidateGeometryByType(
            geomB,
            numTypesC, typesC,
            numCompTypesA, compTypesA,
            dimensionalityB);

    printf("validity=%s\n", GeometryValidityToString(validity));
    CheckGeometryValidity(validity, FdoSpatialGeometryValidity_InvalidDueToDimensionality);

    // Check that not specifying component types results in them all being accepted.

    validity = FdoSpatialUtility::ValidateGeometryByType(
            geomA,
            numTypesA, typesA,
            0, NULL,
            dimensionalityA);

    printf("validity=%s\n", GeometryValidityToString(validity));
    CheckGeometryValidity(validity, FdoSpatialGeometryValidity_Valid);

    // Check all supported geometry types.

    FdoPtr<FdoIGeometry> pt =   gf->CreateGeometry(L"POINT (5 3)");
    FdoPtr<FdoIGeometry> ls =   gf->CreateGeometry(L"LINESTRING (0 1, 2 3, 4 5)");
    FdoPtr<FdoIGeometry> pg =   gf->CreateGeometry(L"POLYGON ((0 0, 5 0, 5 5, 0 5, 0 0), (1 1, 2 1, 2 2, 1 1), (3 3, 4 3, 4 4, 3 3))");
    FdoPtr<FdoIGeometry> mpt =  gf->CreateGeometry(L"MULTIPOINT XYZ (1 2 3, 4 5 6, 7 8 9)");
    FdoPtr<FdoIGeometry> mls =  gf->CreateGeometry(L"MULTILINESTRING XYZ ((0 1 2, 3 4 5, 6 7 8), (9 10 11, 12 13 14, 15 16 17))");
    FdoPtr<FdoIGeometry> mpg =  gf->CreateGeometry(L"MULTIPOLYGON (((0 0, 5 0, 5 5, 0 5, 0 0), (1 1, 2 1, 2 2, 1 1), (3 3, 4 3, 4 4, 3 3)), ((0 0, 5 0, 5 5, 0 5, 0 0), (1 1, 2 1, 2 2, 1 1), (3 3, 4 3, 4 4, 3 3)))");
    FdoPtr<FdoIGeometry> mg =   gf->CreateGeometry(L"GEOMETRYCOLLECTION (CURVEPOLYGON ((100 100 (CIRCULARARCSEGMENT (100 101, 101 102), LINESTRINGSEGMENT (100 100))), (200 200 (CIRCULARARCSEGMENT (200 201, 201 202), LINESTRINGSEGMENT (200 200))), (300 300 (CIRCULARARCSEGMENT (300 301, 301 302), LINESTRINGSEGMENT (300 300)))), CURVESTRING (100 100 (CIRCULARARCSEGMENT (100 101, 101 102), LINESTRINGSEGMENT (103 100, 103 102))), LINESTRING (0 1, 2 3, 4 5), POINT XYZ (5 3 2), POLYGON ((0 0, 5 0, 5 5, 0 5, 0 0), (1 1, 2 1, 2 2, 1 1), (3 3, 4 3, 4 4, 3 3)))");
    FdoPtr<FdoIGeometry> cs =   gf->CreateGeometry(L"CURVESTRING (0 0 (CIRCULARARCSEGMENT (0 1, 1 2), LINESTRINGSEGMENT (3 0, 3 2)))");
    FdoPtr<FdoIGeometry> cpg =  gf->CreateGeometry(L"CURVEPOLYGON ((200 200 (CIRCULARARCSEGMENT (200 201, 201 202), LINESTRINGSEGMENT (200 200))), (300 300 (CIRCULARARCSEGMENT (300 301, 301 302), LINESTRINGSEGMENT (300 300))), (400 400 (CIRCULARARCSEGMENT (400 401, 401 402), LINESTRINGSEGMENT (400 400))))");
    FdoPtr<FdoIGeometry> mcs =  gf->CreateGeometry(L"MULTICURVESTRING ((100 100 (CIRCULARARCSEGMENT (100 101, 101 102), LINESTRINGSEGMENT (103 100, 103 102))), (200 200 (CIRCULARARCSEGMENT (200 201, 201 202), LINESTRINGSEGMENT (203 200, 203 202))), (300 300 (CIRCULARARCSEGMENT (300 301, 301 302), LINESTRINGSEGMENT (303 300, 303 302))))");
    FdoPtr<FdoIGeometry> mcpg = gf->CreateGeometry(L"MULTICURVEPOLYGON (((200 200 (CIRCULARARCSEGMENT (200 201, 201 202), LINESTRINGSEGMENT (200 200))), (300 300 (CIRCULARARCSEGMENT (300 301, 301 302), LINESTRINGSEGMENT (300 300))), (400 400 (CIRCULARARCSEGMENT (400 401, 401 402), LINESTRINGSEGMENT (400 400)))), ((201 201 (CIRCULARARCSEGMENT (201 202, 202 203), LINESTRINGSEGMENT (201 201))), (301 301 (CIRCULARARCSEGMENT (301 302, 302 303), LINESTRINGSEGMENT (301 301))), (401 401 (CIRCULARARCSEGMENT (401 402, 402 403), LINESTRINGSEGMENT (401 401)))), ((202 202 (CIRCULARARCSEGMENT (202 203, 203 204), LINESTRINGSEGMENT (202 202))), (302 302 (CIRCULARARCSEGMENT (302 303, 303 304), LINESTRINGSEGMENT (302 302))), (402 402 (CIRCULARARCSEGMENT (402 403, 403 404), LINESTRINGSEGMENT (402 402)))))");

    validity = FdoSpatialUtility::ValidateGeometryByType(pt,   numTypesA, typesA, numCompTypesA, compTypesA, dimensionalityA);
    CheckGeometryValidity(validity, FdoSpatialGeometryValidity_Valid);
    validity = FdoSpatialUtility::ValidateGeometryByType(ls,   numTypesA, typesA, numCompTypesA, compTypesA, dimensionalityA);
    CheckGeometryValidity(validity, FdoSpatialGeometryValidity_Valid);
    validity = FdoSpatialUtility::ValidateGeometryByType(pg,   numTypesA, typesA, numCompTypesA, compTypesA, dimensionalityA);
    CheckGeometryValidity(validity, FdoSpatialGeometryValidity_Valid);
    validity = FdoSpatialUtility::ValidateGeometryByType(mpt,  numTypesA, typesA, numCompTypesA, compTypesA, dimensionalityA);
    CheckGeometryValidity(validity, FdoSpatialGeometryValidity_Valid);
    validity = FdoSpatialUtility::ValidateGeometryByType(mls,  numTypesA, typesA, numCompTypesA, compTypesA, dimensionalityA);
    CheckGeometryValidity(validity, FdoSpatialGeometryValidity_Valid);
    validity = FdoSpatialUtility::ValidateGeometryByType(mpg,  numTypesA, typesA, numCompTypesA, compTypesA, dimensionalityA);
    CheckGeometryValidity(validity, FdoSpatialGeometryValidity_Valid);
    validity = FdoSpatialUtility::ValidateGeometryByType(mg,   numTypesA, typesA, numCompTypesA, compTypesA, dimensionalityA);
    CheckGeometryValidity(validity, FdoSpatialGeometryValidity_Valid);
    validity = FdoSpatialUtility::ValidateGeometryByType(cs,   numTypesA, typesA, numCompTypesA, compTypesA, dimensionalityA);
    CheckGeometryValidity(validity, FdoSpatialGeometryValidity_Valid);
    validity = FdoSpatialUtility::ValidateGeometryByType(cpg,  numTypesA, typesA, numCompTypesA, compTypesA, dimensionalityA);
    CheckGeometryValidity(validity, FdoSpatialGeometryValidity_Valid);
    validity = FdoSpatialUtility::ValidateGeometryByType(mcs,  numTypesA, typesA, numCompTypesA, compTypesA, dimensionalityA);
    CheckGeometryValidity(validity, FdoSpatialGeometryValidity_Valid);
    validity = FdoSpatialUtility::ValidateGeometryByType(mcpg, numTypesA, typesA, numCompTypesA, compTypesA, dimensionalityA);
    CheckGeometryValidity(validity, FdoSpatialGeometryValidity_Valid);
}

//////////////////////////////////////////////////////////////////////
// Test each geometry type...


//////////////////////////////////////////////////////////////////////
// geometries...
//
// FdoGeometryType_Point
// FdoGeometryType_LineString
// FdoGeometryType_Polygon
// FdoGeometryType_MultiPoint
// FdoGeometryType_MultiLineString
// FdoGeometryType_MultiPolygon
// FdoGeometryType_MultiGeometry
// FdoGeometryType_CurveString
// FdoGeometryType_CurvePolygon
// FdoGeometryType_MultiCurveString
// FdoGeometryType_MultiCurvePolygon
//////////////////////////////////////////////////////////////////////

void SpatialUtilityTest::testLinearizePoint()
{
	double		ordsXYZ[3];
	ordsXYZ[0] = 5.0; ordsXYZ[1] = 3.0; ordsXYZ[2] = 2.0;

	FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
	FdoPtr<FdoIPoint> pnt = gf->CreatePoint(FdoDimensionality_XY|FdoDimensionality_Z, ordsXYZ);
	DumpPoint(pnt);
	CheckFGFT(pnt, L"POINT XYZ (5 3 2)");

    FdoPtr<FdoIGeometry> linearizedGeometry =
        FdoSpatialUtility::ApproximateGeometryWithLineStrings(pnt, 1.0, 0.001, gf);

    // Should be unchanged.
	CheckFGFT(linearizedGeometry, L"POINT XYZ (5 3 2)");
}

void SpatialUtilityTest::testLinearizeLineString()
{
	FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();

	double ordsXY[6];
	ordsXY[0] = 0.0; ordsXY[1] = 1.0;
	ordsXY[2] = 2.0; ordsXY[3] = 3.0;
	ordsXY[4] = 4.0; ordsXY[5] = 5.0;
	FdoPtr<FdoILineString> line1 = gf->CreateLineString(FdoDimensionality_XY, 6, ordsXY);
	DumpLineString(line1);
	CheckFGFT(line1, L"LINESTRING (0 1, 2 3, 4 5)");

	FdoPtr<FdoByteArray> fgf = gf->GetFgf(line1);
	FdoPtr<FdoILineString> line2 = (FdoILineString *) gf->CreateGeometryFromFgf(fgf);
	CheckFGFT(line2, L"LINESTRING (0 1, 2 3, 4 5)");

	FdoPtr<FdoByteArray> wkb = gf->GetWkb(line1);
	FdoPtr<FdoILineString> line3 = (FdoILineString *) gf->CreateGeometryFromWkb(wkb);
	CheckFGFT(line3, L"LINESTRING (0 1, 2 3, 4 5)");

    FdoPtr<FdoIGeometry> linearizedGeometry =
        FdoSpatialUtility::ApproximateGeometryWithLineStrings(line1, 1.0, 0.001, gf);

    // Should be unchanged.
	CheckFGFT(linearizedGeometry, L"LINESTRING (0 1, 2 3, 4 5)");
}

void SpatialUtilityTest::testLinearizePolygon()
{
	FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
	double ordsXYExt[10];
	ordsXYExt[0] = 0.0; ordsXYExt[1] = 0.0;
	ordsXYExt[2] = 5.0; ordsXYExt[3] = 0.0;
	ordsXYExt[4] = 5.0; ordsXYExt[5] = 5.0;
	ordsXYExt[6] = 0.0; ordsXYExt[7] = 5.0;
	ordsXYExt[8] = 0.0; ordsXYExt[9] = 0.0;

	double ordsXYInt1[8];
	ordsXYInt1[0] = 1.0; ordsXYInt1[1] = 1.0;
	ordsXYInt1[2] = 2.0; ordsXYInt1[3] = 1.0;
	ordsXYInt1[4] = 2.0; ordsXYInt1[5] = 2.0;
	ordsXYInt1[6] = 1.0; ordsXYInt1[7] = 1.0;

	double ordsXYInt2[8];
	ordsXYInt2[0] = 3.0; ordsXYInt2[1] = 3.0;
	ordsXYInt2[2] = 4.0; ordsXYInt2[3] = 3.0;
	ordsXYInt2[4] = 4.0; ordsXYInt2[5] = 4.0;
	ordsXYInt2[6] = 3.0; ordsXYInt2[7] = 3.0;

	FdoPtr<FdoILinearRing> extRing = gf->CreateLinearRing(FdoDimensionality_XY, 10, ordsXYExt);
	FdoPtr<FdoILinearRing> intRing1 = gf->CreateLinearRing(FdoDimensionality_XY, 8, ordsXYInt1);
	FdoPtr<FdoILinearRing> intRing2 = gf->CreateLinearRing(FdoDimensionality_XY, 8, ordsXYInt2);

    FdoPtr<FdoLinearRingCollection> intRings = FdoLinearRingCollection::Create();
	intRings->Add(intRing1);
	intRings->Add(intRing2);

	FdoPtr<FdoIPolygon> polygon = gf->CreatePolygon(extRing, intRings);
	DumpPolygon(polygon);
	CheckFGFT(polygon, L"POLYGON ((0 0, 5 0, 5 5, 0 5, 0 0), (1 1, 2 1, 2 2, 1 1), (3 3, 4 3, 4 4, 3 3))");

    FdoPtr<FdoIGeometry> linearizedGeometry =
        FdoSpatialUtility::ApproximateGeometryWithLineStrings(polygon, 1.0, 0.001, gf);

    // Should be unchanged.
	CheckFGFT(linearizedGeometry, L"POLYGON ((0 0, 5 0, 5 5, 0 5, 0 0), (1 1, 2 1, 2 2, 1 1), (3 3, 4 3, 4 4, 3 3))");
}

void SpatialUtilityTest::testLinearizeMultiPoint()
{
	double		ordsXYZ1[3];
	ordsXYZ1[0] = 1.0; ordsXYZ1[1] = 2.0; ordsXYZ1[2] = 3.0;

	double		ordsXYZ2[3];
	ordsXYZ2[0] = 4.0; ordsXYZ2[1] = 5.0; ordsXYZ2[2] = 6.0;

	double		ordsXYZ3[3];
	ordsXYZ3[0] = 7.0; ordsXYZ3[1] = 8.0; ordsXYZ3[2] = 9.0;

	FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
	FdoPtr<FdoIPoint> pnt1 = gf->CreatePoint(FdoDimensionality_XY|FdoDimensionality_Z, ordsXYZ1);
	FdoPtr<FdoIPoint> pnt2 = gf->CreatePoint(FdoDimensionality_XY|FdoDimensionality_Z, ordsXYZ2);
	FdoPtr<FdoIPoint> pnt3 = gf->CreatePoint(FdoDimensionality_XY|FdoDimensionality_Z, ordsXYZ3);

    FdoPointCollection* pnts = FdoPointCollection::Create();
	pnts->Add(pnt1);
    pnts->Add(pnt2);
	pnts->Add(pnt3);

	FdoPtr<FdoIMultiPoint> multiPnt = gf->CreateMultiPoint(pnts);
	DumpMultiPoint(multiPnt);
	CheckFGFT(multiPnt, L"MULTIPOINT XYZ (1 2 3, 4 5 6, 7 8 9)");

    FdoPtr<FdoIGeometry> linearizedGeometry =
        FdoSpatialUtility::ApproximateGeometryWithLineStrings(multiPnt, 1.0, 0.01, gf);

    // Should be unchanged.
	CheckFGFT(linearizedGeometry, L"MULTIPOINT XYZ (1 2 3, 4 5 6, 7 8 9)");

    pnts->Release();
}

void SpatialUtilityTest::testLinearizeMultiLineString()
{
	double ordsXYZ1[9];
	ordsXYZ1[0] = 0.0; ordsXYZ1[1] = 1.0; ordsXYZ1[2] = 2.0;
	ordsXYZ1[3] = 3.0; ordsXYZ1[4] = 4.0; ordsXYZ1[5] = 5.0;
	ordsXYZ1[6] = 6.0; ordsXYZ1[7] = 7.0; ordsXYZ1[8] = 8.0;

	double ordsXYZ2[9];
	ordsXYZ2[0] = 9.0; ordsXYZ2[1] = 10.0; ordsXYZ2[2] = 11.0;
	ordsXYZ2[3] = 12.0; ordsXYZ2[4] = 13.0; ordsXYZ2[5] = 14.0;
	ordsXYZ2[6] = 15.0; ordsXYZ2[7] = 16.0; ordsXYZ2[8] = 17.0;

	FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();

	FdoPtr<FdoILineString> lineString1 = gf->CreateLineString(FdoDimensionality_XY|FdoDimensionality_Z, 9, ordsXYZ1);
	FdoPtr<FdoILineString> lineString2 = gf->CreateLineString(FdoDimensionality_XY|FdoDimensionality_Z, 9, ordsXYZ2);

    FdoPtr<FdoLineStringCollection> lineStrings = FdoLineStringCollection::Create();
	lineStrings->Add(lineString1);
	lineStrings->Add(lineString2);

	FdoPtr<FdoIMultiLineString> multiLine = gf->CreateMultiLineString(lineStrings);
	DumpMultiLineString(multiLine);
	CheckFGFT(multiLine, L"MULTILINESTRING XYZ ((0 1 2, 3 4 5, 6 7 8), (9 10 11, 12 13 14, 15 16 17))");

    FdoPtr<FdoIGeometry> linearizedGeometry =
        FdoSpatialUtility::ApproximateGeometryWithLineStrings(multiLine, 1.0, 0.01, gf);

    // Should be unchanged.
	CheckFGFT(linearizedGeometry, L"MULTILINESTRING XYZ ((0 1 2, 3 4 5, 6 7 8), (9 10 11, 12 13 14, 15 16 17))");
}

void SpatialUtilityTest::testLinearizeMultiPolygon()
{
	FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();

	// 1st polygon
	double ordsXYExt1[10];
	ordsXYExt1[0] = 0.0; ordsXYExt1[1] = 0.0;
	ordsXYExt1[2] = 5.0; ordsXYExt1[3] = 0.0;
	ordsXYExt1[4] = 5.0; ordsXYExt1[5] = 5.0;
	ordsXYExt1[6] = 0.0; ordsXYExt1[7] = 5.0;
	ordsXYExt1[8] = 0.0; ordsXYExt1[9] = 0.0;

	double ordsXYInt11[8];
	ordsXYInt11[0] = 1.0; ordsXYInt11[1] = 1.0;
	ordsXYInt11[2] = 2.0; ordsXYInt11[3] = 1.0;
	ordsXYInt11[4] = 2.0; ordsXYInt11[5] = 2.0;
	ordsXYInt11[6] = 1.0; ordsXYInt11[7] = 1.0;

	double ordsXYInt12[8];
	ordsXYInt12[0] = 3.0; ordsXYInt12[1] = 3.0;
	ordsXYInt12[2] = 4.0; ordsXYInt12[3] = 3.0;
	ordsXYInt12[4] = 4.0; ordsXYInt12[5] = 4.0;
	ordsXYInt12[6] = 3.0; ordsXYInt12[7] = 3.0;

	FdoPtr<FdoILinearRing> extRing1 = gf->CreateLinearRing(FdoDimensionality_XY, 10, ordsXYExt1);
	FdoPtr<FdoILinearRing> intRing11 = gf->CreateLinearRing(FdoDimensionality_XY, 8, ordsXYInt11);
	FdoPtr<FdoILinearRing> intRing12 = gf->CreateLinearRing(FdoDimensionality_XY, 8, ordsXYInt12);

    FdoPtr<FdoLinearRingCollection> intRings1 = FdoLinearRingCollection::Create();
	intRings1->Add(intRing11);
	intRings1->Add(intRing12);

	FdoPtr<FdoIPolygon> polygon1 = gf->CreatePolygon(extRing1, intRings1);

	// 2nd polygon
	double ordsXYExt2[10];
	ordsXYExt2[0] = 0.0; ordsXYExt2[1] = 0.0;
	ordsXYExt2[2] = 5.0; ordsXYExt2[3] = 0.0;
	ordsXYExt2[4] = 5.0; ordsXYExt2[5] = 5.0;
	ordsXYExt2[6] = 0.0; ordsXYExt2[7] = 5.0;
	ordsXYExt2[8] = 0.0; ordsXYExt2[9] = 0.0;

	double ordsXYInt21[8];
	ordsXYInt21[0] = 1.0; ordsXYInt21[1] = 1.0;
	ordsXYInt21[2] = 2.0; ordsXYInt21[3] = 1.0;
	ordsXYInt21[4] = 2.0; ordsXYInt21[5] = 2.0;
	ordsXYInt21[6] = 1.0; ordsXYInt21[7] = 1.0;

	double ordsXYInt22[8];
	ordsXYInt22[0] = 3.0; ordsXYInt22[1] = 3.0;
	ordsXYInt22[2] = 4.0; ordsXYInt22[3] = 3.0;
	ordsXYInt22[4] = 4.0; ordsXYInt22[5] = 4.0;
	ordsXYInt22[6] = 3.0; ordsXYInt22[7] = 3.0;

	FdoPtr<FdoILinearRing> extRing2 = gf->CreateLinearRing(FdoDimensionality_XY, 10, ordsXYExt2);
	FdoPtr<FdoILinearRing> intRing21 = gf->CreateLinearRing(FdoDimensionality_XY, 8, ordsXYInt21);
	FdoPtr<FdoILinearRing> intRing22 = gf->CreateLinearRing(FdoDimensionality_XY, 8, ordsXYInt22);

    FdoPtr<FdoLinearRingCollection> intRings2 = FdoLinearRingCollection::Create();
	intRings2->Add(intRing21);
	intRings2->Add(intRing22);

	FdoPtr<FdoIPolygon> polygon2 = gf->CreatePolygon(extRing2, intRings2);

    FdoPtr<FdoPolygonCollection> polygons = FdoPolygonCollection::Create();
	polygons->Add(polygon1);
	polygons->Add(polygon2);

	FdoPtr<FdoIMultiPolygon> multiPoly = gf->CreateMultiPolygon(polygons);
	DumpMultiPolygon(multiPoly);
	CheckFGFT(multiPoly, L"MULTIPOLYGON (((0 0, 5 0, 5 5, 0 5, 0 0), (1 1, 2 1, 2 2, 1 1), (3 3, 4 3, 4 4, 3 3)), ((0 0, 5 0, 5 5, 0 5, 0 0), (1 1, 2 1, 2 2, 1 1), (3 3, 4 3, 4 4, 3 3)))");

    FdoPtr<FdoIGeometry> linearizedGeometry =
        FdoSpatialUtility::ApproximateGeometryWithLineStrings(multiPoly, 1.0, 0.01, gf);

    // Should be unchanged.
	CheckFGFT(linearizedGeometry, L"MULTIPOLYGON (((0 0, 5 0, 5 5, 0 5, 0 0), (1 1, 2 1, 2 2, 1 1), (3 3, 4 3, 4 4, 3 3)), ((0 0, 5 0, 5 5, 0 5, 0 0), (1 1, 2 1, 2 2, 1 1), (3 3, 4 3, 4 4, 3 3)))");
}

void SpatialUtilityTest::testLinearizeMultiGeometry()
{
	FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();

	FdoPtr<FdoIMultiGeometry> multiGeometry = CreateMultiGeometry();
	DumpMultiGeometry(multiGeometry);
	CheckFGFT(multiGeometry, L"GEOMETRYCOLLECTION (CURVEPOLYGON ((100 100 (CIRCULARARCSEGMENT (100 101, 101 102), LINESTRINGSEGMENT (100 100))), (200 200 (CIRCULARARCSEGMENT (200 201, 201 202), LINESTRINGSEGMENT (200 200))), (300 300 (CIRCULARARCSEGMENT (300 301, 301 302), LINESTRINGSEGMENT (300 300)))), CURVESTRING (100 100 (CIRCULARARCSEGMENT (100 101, 101 102), LINESTRINGSEGMENT (103 100, 103 102))), LINESTRING (0 1, 2 3, 4 5), POINT XYZ (5 3 2), POLYGON ((0 0, 5 0, 5 5, 0 5, 0 0), (1 1, 2 1, 2 2, 1 1), (3 3, 4 3, 4 4, 3 3)))");

    FdoPtr<FdoIGeometry> linearizedGeometry =
        FdoSpatialUtility::ApproximateGeometryWithLineStrings(multiGeometry, 1.0, 0.01, gf);

    printf("Linearized 2D MultiGeometry:\n");
    // Geometry should be very changed (all linear -- no arcs).
	DumpGeometry(linearizedGeometry);

	CheckFGFT(linearizedGeometry,
        L"GEOMETRYCOLLECTION (POLYGON ((100 100, "
        L"99.9263476647491 100.346317444844, "
        L"99.9316048285876 100.700341174725, "
        L"100.015507875369 101.044318961554, "
        L"100.173849555978 101.361002322773, "
        L"100.398689957372 101.634511432295, "
        L"100.678754642975 101.851131401295, "
        L"101 102, "
        L"100 100), "
        L"(200 200, "
        L"199.926347664749 200.346317444844, "
        L"199.931604828588 200.700341174725, "
        L"200.015507875369 201.044318961554, "
        L"200.173849555978 201.361002322773, "
        L"200.398689957372 201.634511432295, "
        L"200.678754642975 201.851131401295, "
        L"201 202, "
        L"200 200), "
        L"(300 300, "
        L"299.926347664749 300.346317444844, "
        L"299.931604828588 300.700341174725, "
        L"300.015507875369 301.044318961554, "
        L"300.173849555978 301.361002322773, "
        L"300.398689957372 301.634511432295, "
        L"300.678754642975 301.851131401295, "
        L"301 302, "
        L"300 300)), "
        L"LINESTRING (100 100, "
        L"99.9263476647491 100.346317444844, "
        L"99.9316048285876 100.700341174725, "
        L"100.015507875369 101.044318961554, "
        L"100.173849555978 101.361002322773, "
        L"100.398689957372 101.634511432295, "
        L"100.678754642975 101.851131401295, "
        L"101 102, "
        L"103 100, "
        L"103 102), "
        L"LINESTRING (0 1, 2 3, 4 5), "
        L"POINT XYZ (5 3 2), "
        L"POLYGON ((0 0, 5 0, 5 5, 0 5, 0 0), (1 1, 2 1, 2 2, 1 1), (3 3, 4 3, 4 4, 3 3)))");
}

void SpatialUtilityTest::testLinearizeCurveString()
{
	FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();

	// Create a curve string that consists of an arcsegment and a linear segment

	// arcseg  = (0,0), (0,1), (1,2)
	// lineseg = (1,2), (3,0), (3,2)

	FdoPtr<FdoIDirectPosition> startPos = gf->CreatePositionXY(0.0, 0.0);
	FdoPtr<FdoIDirectPosition> midPos = gf->CreatePositionXY(0.0, 1.0);
	FdoPtr<FdoIDirectPosition> endPos = gf->CreatePositionXY(1.0, 2.0);

	FdoPtr<FdoICircularArcSegment> arcSeg = gf->CreateCircularArcSegment(startPos, midPos, endPos);

    FdoPtr<FdoDirectPositionCollection> points = FdoDirectPositionCollection::Create();
	FdoPtr<FdoIDirectPosition> pt1 = gf->CreatePositionXY(1.0, 2.0);
	FdoPtr<FdoIDirectPosition> pt2 = gf->CreatePositionXY(3.0, 0.0);
	FdoPtr<FdoIDirectPosition> pt3 = gf->CreatePositionXY(3.0, 2.0);
    points->Add(pt1);
    points->Add(pt2);
	points->Add(pt3);

	FdoPtr<FdoILineStringSegment> lineSeg = gf->CreateLineStringSegment(points);

    FdoPtr<FdoCurveSegmentCollection> curveSegs = FdoCurveSegmentCollection::Create();
    curveSegs->Add(arcSeg);
    curveSegs->Add(lineSeg);

	FdoPtr<FdoICurveString> curveString = gf->CreateCurveString(curveSegs);
	DumpCurveString(curveString);
    DumpGeometryToGinaFile(curveString, true);
	CheckFGFT(curveString, L"CURVESTRING (0 0 (CIRCULARARCSEGMENT (0 1, 1 2), LINESTRINGSEGMENT (3 0, 3 2)))");

    FdoPtr<FdoIGeometry> linearizedGeometry =
        FdoSpatialUtility::ApproximateGeometryWithLineStrings(curveString, 1.0, 0.001, gf);

    printf("Linearized 2D CurveString:\n");
    // Geometry should be very changed (all linear -- no arcs).
	DumpGeometry(linearizedGeometry);
    DumpGeometryToGinaFile(linearizedGeometry, false);
#pragma message ("TODO: Turn all of testLinearizeCurveString back on after investigating failure.")
#if 0
	CheckFGFT(linearizedGeometry,
        L"LINESTRING (0 0, "
        L"-0.030624360468144 0.103529235451737, "
        L"-0.05411224360262 0.208907000626204, "
        L"-0.070354138049801 0.315641975737789, "
        L"-0.079274316600211 0.423236513061636, "
        L"-0.080831189263724 0.531188957197069, "
        L"-0.0750174971813 0.638995984016635, "
        L"-0.061860346469139 0.74615494739554, "
        L"-0.04142108183745 0.852166222779906, "
        L"-0.013795000573097 0.95653553666708, "
        L"0.020889091780344 1.05877627113687, "
        L"0.062469481973487 1.15841173268892, "
        L"0.110752303032957 1.25497737480786, "
        L"0.165512438158711 1.3480229638936, "
        L"0.226494570322478 1.43711467845823, "
        L"0.293414372673605 1.52183713180202, "
        L"0.365959834202084 1.60179530873799, "
        L"0.443792714477911 1.67661640733506, "
        L"0.52655012068411 1.74595157709274, "
        L"0.613846199590564 1.80947754544319, "
        L"0.705273936579888 1.86689812499717, "
        L"0.800407053337464 1.91794559450637, "
        L"0.898801995357723 1.96238194710347, "
        L"1 2, 3 0, 3 2)");
#endif

    // Now try it again, in 3D.

	startPos = gf->CreatePositionXYZ(0.0, 0.0, 0.0);
	midPos = gf->CreatePositionXYZ(0.0, 1.0, 0.1);
	endPos = gf->CreatePositionXYZ(1.0, 2.0, 0.0);

	arcSeg = gf->CreateCircularArcSegment(startPos, midPos, endPos);

    points = FdoDirectPositionCollection::Create();
	pt1 = gf->CreatePositionXYZ(1.0, 2.0, 0.0);
	 pt2 = gf->CreatePositionXYZ(3.0, 0.0, 0.0);
	pt3 = gf->CreatePositionXYZ(3.0, 2.0, 0.0);
    points->Add(pt1);
    points->Add(pt2);
	points->Add(pt3);

	lineSeg = gf->CreateLineStringSegment(points);

    curveSegs = FdoCurveSegmentCollection::Create();
    curveSegs->Add(arcSeg);
    curveSegs->Add(lineSeg);

	curveString = gf->CreateCurveString(curveSegs);
	DumpCurveString(curveString);
	CheckFGFT(curveString, L"CURVESTRING XYZ (0 0 0 (CIRCULARARCSEGMENT (0 1 0.1, 1 2 0), LINESTRINGSEGMENT (3 0 0, 3 2 0)))");

    linearizedGeometry =
        FdoSpatialUtility::ApproximateGeometryWithLineStrings(curveString, 1.0, 0.001, gf);

    printf("Linearized 3D CurveString:\n");
    
    FdoString* linearizedGeometryTestText;
	
#ifdef _WIN64
	linearizedGeometryTestText =    
		L"LINESTRING XYZ (0 0 0, "
        L"-0.03085536765629 0.102689577511987 0.016440031282457, "
        L"-0.054535138833766 0.207452799842112 0.031652307750964, "
        L"-0.070924015276176 0.313779568444088 0.045562759899644, "
        L"-0.079942198540299 0.421152171769058 0.058103656884966, "
        L"-0.081545778539459 0.529047806034846 0.069213936311376, "
        L"-0.07572694734442 0.636941120789586 0.078839501547843, "
        L"-0.062514037200674 0.744306776875256 0.08693348512766, "
        L"-0.041971382577004 0.85062200433621 0.093456476949022, "
        L"-0.014199006917014 0.955369147818116 0.098376716165215, "
        L"0.020667864381145 1.05803818706358 0.101670245830129, "
        L"0.062459462385521 1.15812922023203 0.103321029546099, "
        L"0.110972301245689 1.25515489795237 0.103321029546099, "
        L"0.16597016897782 1.34864279625693 0.101670245830129, "
        L"0.227185277595298 1.43813771684274 0.098376716165215, "
        L"0.29431956698483 1.52320390345988 0.093456476949022, "
        L"0.367046156179391 1.60342716363539 0.08693348512766, "
        L"0.445010934961679 1.67841688540178 0.078839501547843, "
        L"0.527834288048447 1.74780793921066 0.069213936311376, "
        L"0.615112943460573 1.8112624557708 0.058103656884966, "
        L"0.706421936079024 1.86847147115449 0.045562759899644, "
        L"0.80131667682605 1.91915643116174 0.031652307750964, "
        L"0.899335117396636 1.96307054761784 0.016440031282457, "
        L"1 2 0, 3 0 0, 3 2 0)";
#else
	linearizedGeometryTestText =    
		L"LINESTRING XYZ (0 0 0, "
        L"-0.03085536765629 0.102689577511987 0.016440031282457, "
        L"-0.054535138833766 0.207452799842112 0.031652307750964, "
        L"-0.070924015276176 0.313779568444088 0.045562759899644, "
        L"-0.079942198540299 0.421152171769058 0.058103656884966, "
        L"-0.081545778539459 0.529047806034846 0.069213936311376, "
        L"-0.07572694734442 0.636941120789586 0.078839501547843, "
        L"-0.062514037200674 0.744306776875256 0.08693348512766, "
        L"-0.041971382577004 0.85062200433621 0.093456476949022, "
        L"-0.014199006917014 0.955369147818116 0.098376716165215, "
        L"0.020667864381145 1.05803818706358 0.101670245830129, "
        L"0.062459462385521 1.15812922023203 0.103321029546099, "
        L"0.110972301245689 1.25515489795237 0.103321029546099, "
        L"0.16597016897782 1.34864279625693 0.101670245830129, "
        L"0.227185277595298 1.43813771684274 0.098376716165215, "
        L"0.29431956698483 1.52320390345988 0.093456476949022, "
        L"0.367046156179391 1.60342716363539 0.08693348512766, "
        L"0.445010934961679 1.67841688540178 0.078839501547843, "
        L"0.527834288048447 1.74780793921066 0.069213936311376, "
        L"0.615112943460573 1.8112624557708 0.058103656884966, "
        L"0.706421936079024 1.86847147115449 0.045562759899644, "
        L"0.80131667682605 1.91915643116175 0.031652307750964, "
        L"0.899335117396636 1.96307054761784 0.016440031282457, "
        L"1 2 0, 3 0 0, 3 2 0)";
#endif

    // Geometry should be very changed (all linear -- no arcs).
	DumpGeometry(linearizedGeometry);
	CheckFGFT(linearizedGeometry, linearizedGeometryTestText);

	// Test 2D circle, centered at (2,0) and with radius 2.0.

	startPos = gf->CreatePositionXY(0.0, 0.0);
	midPos = gf->CreatePositionXY(4.0, 0.0);
	endPos = gf->CreatePositionXY(0.0, 0.0);

	arcSeg = gf->CreateCircularArcSegment(startPos, midPos, endPos);

    curveSegs = FdoCurveSegmentCollection::Create();
    curveSegs->Add(arcSeg);

	curveString = gf->CreateCurveString(curveSegs);
	DumpCurveString(curveString);
	CheckFGFT(curveString, L"CURVESTRING (0 0 (CIRCULARARCSEGMENT (4 0, 0 0)))");

    linearizedGeometry =
        FdoSpatialUtility::ApproximateGeometryWithLineStrings(curveString, 1.0, 0.01, gf);

    printf("Linearized 2D CurveString with circle:\n");
    // Geometry should be very changed (all linear -- no arcs).
	DumpGeometry(linearizedGeometry);
	CheckFGFT(linearizedGeometry,
        L"LINESTRING (0 0, "
        L"0.038429439193539 -0.390180644032257, "
        L"0.152240934977426 -0.765366864730179, "
        L"0.337060775394909 -1.1111404660392, "
        L"0.585786437626905 -1.41421356237309, "
        L"0.888859533960796 -1.66293922460509, "
        L"1.23463313526982 -1.84775906502257, "
        L"1.60981935596774 -1.96157056080646, "
        L"2 -2, "
        L"2.39018064403226 -1.96157056080646, "
        L"2.76536686473018 -1.84775906502257, "
        L"3.1111404660392 -1.66293922460509, "
        L"3.41421356237309 -1.4142135623731, "
        L"3.66293922460509 -1.1111404660392, "
        L"3.84775906502257 -0.765366864730181, "
        L"3.96157056080646 -0.390180644032257, "
        L"4 0, "
        L"3.96157056080646 0.390180644032257, "
        L"3.84775906502257 0.76536686473018, "
        L"3.66293922460509 1.1111404660392, "
        L"3.4142135623731 1.41421356237309, "
        L"3.1111404660392 1.66293922460509, "
        L"2.76536686473018 1.84775906502257, "
        L"2.39018064403226 1.96157056080646, "
        L"2 2, "
        L"1.60981935596774 1.96157056080646, "
        L"1.23463313526982 1.84775906502257, "
        L"0.888859533960797 1.66293922460509, "
        L"0.585786437626907 1.4142135623731, "
        L"0.33706077539491 1.1111404660392, "
        L"0.152240934977427 0.765366864730181, "
        L"0.038429439193539 0.390180644032256, "
        L"0 0)");

	// Test 3D circle (a tilted variation of the 2D circle test).

	startPos = gf->CreatePositionXYZ(0.0, 0.0, 0.0);
	midPos = gf->CreatePositionXYZ(4.0, 0.0, 0.1);
	endPos = gf->CreatePositionXYZ(0.0, 0.0, 0.0);

	arcSeg = gf->CreateCircularArcSegment(startPos, midPos, endPos);

    curveSegs = FdoCurveSegmentCollection::Create();
    curveSegs->Add(arcSeg);

	curveString = gf->CreateCurveString(curveSegs);
	DumpCurveString(curveString);
	CheckFGFT(curveString, L"CURVESTRING XYZ (0 0 0 (CIRCULARARCSEGMENT (4 0 0.1, 0 0 0)))");

    linearizedGeometry =
        FdoSpatialUtility::ApproximateGeometryWithLineStrings(curveString, 1.0, 0.01, gf);

    printf("Linearized 3D CurveString with circle:\n");
    // Geometry should be very changed (all linear -- no arcs).
	DumpGeometry(linearizedGeometry);
	CheckFGFT(linearizedGeometry,
        L"LINESTRING XYZ (0 0 0, "
        L"0.038429439193539 -0.390302556437679 0.000960735979838, "
        L"0.152240934977427 -0.765606004515653 0.003806023374436, "
        L"0.337060775394909 -1.11148764319688 0.008426519384873, "
        L"0.585786437626905 -1.41465543507951 0.014644660940673, "
        L"0.888859533960795 -1.66345881193994 0.02222148834902, "
        L"1.23463313526982 -1.84833639953597 0.030865828381746, "
        L"1.60981935596774 -1.96218345585682 0.040245483899194, "
        L"2 -2.00062490237426 0.05, "
        L"2.39018064403226 -1.96218345585682 0.059754516100806, "
        L"2.76536686473018 -1.84833639953597 0.069134171618254, "
        L"3.1111404660392 -1.66345881193994 0.07777851165098, "
        L"3.41421356237309 -1.41465543507951 0.085355339059327, "
        L"3.66293922460509 -1.11148764319688 0.091573480615127, "
        L"3.84775906502257 -0.765606004515653 0.096193976625564, "
        L"3.96157056080646 -0.390302556437679 0.099039264020161, "
        L"4 0 0.1, "
        L"3.96157056080646 0.390302556437679 0.099039264020161, "
        L"3.84775906502257 0.765606004515652 0.096193976625564, "
        L"3.66293922460509 1.11148764319688 0.091573480615127, "
        L"3.41421356237309 1.41465543507951 0.085355339059327, "
        L"3.1111404660392 1.66345881193994 0.07777851165098, "
        L"2.76536686473018 1.84833639953597 0.069134171618254, "
        L"2.39018064403226 1.96218345585682 0.059754516100806, "
        L"2 2.00062490237426 0.05, "
        L"1.60981935596774 1.96218345585682 0.040245483899194, "
        L"1.23463313526982 1.84833639953597 0.030865828381745, "
        L"0.888859533960796 1.66345881193994 0.02222148834902, "
        L"0.585786437626905 1.41465543507951 0.014644660940673, "
        L"0.337060775394909 1.11148764319688 0.008426519384873, "
        L"0.152240934977427 0.765606004515654 0.003806023374436, "
        L"0.038429439193539 0.39030255643768 0.000960735979838, "
        L"0 0 0)");

    // Test 2D curve with collinear arc positions.

    FdoPtr<FdoIGeometry> cs = gf->CreateGeometry(L"CURVESTRING (0 0 (CIRCULARARCSEGMENT (1 1, 2 2), LINESTRINGSEGMENT (3 0, 3 2)))");
	DumpGeometry(cs);
	CheckFGFT(cs, L"CURVESTRING (0 0 (CIRCULARARCSEGMENT (1 1, 2 2), LINESTRINGSEGMENT (3 0, 3 2)))");
    linearizedGeometry = FdoSpatialUtility::ApproximateGeometryWithLineStrings(cs, 1.0, 0.01, gf);
    printf("Linearized 2D CurveString with arc having collinear positions:\n");
    // Geometry should be somewhat changed (linestring with arc positions used directly with no interpolation).
	DumpGeometry(linearizedGeometry);
	CheckFGFT(linearizedGeometry, L"LINESTRING (0 0, 1 1, 2 2, 3 0, 3 2)");

    // Test 3D curve with collinear arc positions.

    cs = gf->CreateGeometry(L"CURVESTRING XYZ (0 0 0 (CIRCULARARCSEGMENT (1 1 1, 2 2 2), LINESTRINGSEGMENT (3 0 2, 3 2 1)))");
	DumpGeometry(cs);
	CheckFGFT(cs, L"CURVESTRING XYZ (0 0 0 (CIRCULARARCSEGMENT (1 1 1, 2 2 2), LINESTRINGSEGMENT (3 0 2, 3 2 1)))");
    linearizedGeometry = FdoSpatialUtility::ApproximateGeometryWithLineStrings(cs, 1.0, 0.01, gf);
    printf("Linearized 3D CurveString with arc having collinear positions:\n");
    // Geometry should be somewhat changed (linestring with arc positions used directly with no interpolation).
	DumpGeometry(linearizedGeometry);
	CheckFGFT(linearizedGeometry, L"LINESTRING XYZ (0 0 0, 1 1 1, 2 2 2, 3 0 2, 3 2 1)");

    // Test 2D curve with collinear arc positions forming a spike.

    cs = gf->CreateGeometry(L"CURVESTRING (0 0 (CIRCULARARCSEGMENT (2 2, 1 1), LINESTRINGSEGMENT (3 0, 3 2)))");
	DumpGeometry(cs);
	CheckFGFT(cs, L"CURVESTRING (0 0 (CIRCULARARCSEGMENT (2 2, 1 1), LINESTRINGSEGMENT (3 0, 3 2)))");
    linearizedGeometry = FdoSpatialUtility::ApproximateGeometryWithLineStrings(cs, 1.0, 0.01, gf);
    printf("Linearized 2D CurveString with arc having collinear positions:\n");
    // Geometry should be somewhat changed (linestring with arc positions used directly with no interpolation).
	DumpGeometry(linearizedGeometry);
	CheckFGFT(linearizedGeometry, L"LINESTRING (0 0, 2 2, 1 1, 3 0, 3 2)");

    // Test 3D curve with collinear arc positions forming a spike.

    cs = gf->CreateGeometry(L"CURVESTRING XYZ (0 0 0 (CIRCULARARCSEGMENT (2 2 2, 1 1 1), LINESTRINGSEGMENT (3 0 2, 3 2 1)))");
	DumpGeometry(cs);
	CheckFGFT(cs, L"CURVESTRING XYZ (0 0 0 (CIRCULARARCSEGMENT (2 2 2, 1 1 1), LINESTRINGSEGMENT (3 0 2, 3 2 1)))");
    linearizedGeometry = FdoSpatialUtility::ApproximateGeometryWithLineStrings(cs, 1.0, 0.01, gf);
    printf("Linearized 3D CurveString with arc having collinear positions:\n");
    // Geometry should be somewhat changed (linestring with arc positions used directly with no interpolation).
	DumpGeometry(linearizedGeometry);
	CheckFGFT(linearizedGeometry, L"LINESTRING XYZ (0 0 0, 2 2 2, 1 1 1, 3 0 2, 3 2 1)");

    // Test 2D curve with arc positions that are too close to interpolate (just use large tolerances).

    cs = gf->CreateGeometry(L"CURVESTRING (0 0 (CIRCULARARCSEGMENT (0 1, 1 2), LINESTRINGSEGMENT (3 0, 3 2)))");
	DumpGeometry(cs);
	CheckFGFT(cs, L"CURVESTRING (0 0 (CIRCULARARCSEGMENT (0 1, 1 2), LINESTRINGSEGMENT (3 0, 3 2)))");
    linearizedGeometry = FdoSpatialUtility::ApproximateGeometryWithLineStrings(cs, 2.0, 2.0, gf);
    printf("Linearized 2D CurveString with arc having positions too close to interpolate:\n");
    // Geometry should be somewhat changed (linestring with arc positions used directly with no interpolation).
	DumpGeometry(linearizedGeometry);
	CheckFGFT(linearizedGeometry, L"LINESTRING (0 0, 0 1, 1 2, 3 0, 3 2)");

    // Test 3D curve with arc positions that are too close to interpolate (just use large tolerances).

    cs = gf->CreateGeometry(L"CURVESTRING XYZ (0 0 0 (CIRCULARARCSEGMENT (0 1 0.1, 1 2 0), LINESTRINGSEGMENT (3 0 0, 3 2 0)))");
	DumpGeometry(cs);
	CheckFGFT(cs, L"CURVESTRING XYZ (0 0 0 (CIRCULARARCSEGMENT (0 1 0.1, 1 2 0), LINESTRINGSEGMENT (3 0 0, 3 2 0)))");
    linearizedGeometry = FdoSpatialUtility::ApproximateGeometryWithLineStrings(cs, 2.0, 2.0, gf);
    printf("Linearized 3D CurveString with arc having positions too close to interpolate:\n");
    // Geometry should be somewhat changed (linestring with arc positions used directly with no interpolation).
	DumpGeometry(linearizedGeometry);
	CheckFGFT(linearizedGeometry, L"LINESTRING XYZ (0 0 0, 0 1 0.1, 1 2 0, 3 0 0, 3 2 0)");

    // Test zero-valued parameters.

    cs = gf->CreateGeometry(L"CURVESTRING (0 0 (CIRCULARARCSEGMENT (0 1, 1 2), LINESTRINGSEGMENT (3 0, 3 2)))");
	DumpGeometry(cs);
	CheckFGFT(cs, L"CURVESTRING (0 0 (CIRCULARARCSEGMENT (0 1, 1 2), LINESTRINGSEGMENT (3 0, 3 2)))");
    linearizedGeometry = FdoSpatialUtility::ApproximateGeometryWithLineStrings(cs, 0.0, 0.001, gf);
    printf("Linearized 2D CurveString with zero maxSpacing:\n");
    // Geometry should be very changed (all linear -- no arcs -- same as first 2D case in this test).
	DumpGeometry(linearizedGeometry);
#pragma message ("TODO: Turn all of testLinearizeCurveString back on after investigating failure.")
#if 0
	CheckFGFT(linearizedGeometry,
        L"LINESTRING (0 0, "
        L"-0.030624360468144 0.103529235451737, "
        L"-0.05411224360262 0.208907000626204, "
        L"-0.070354138049801 0.315641975737789, "
        L"-0.079274316600211 0.423236513061636, "
        L"-0.080831189263724 0.531188957197069, "
        L"-0.0750174971813 0.638995984016635, "
        L"-0.061860346469139 0.74615494739554, "
        L"-0.04142108183745 0.852166222779906, "
        L"-0.013795000573097 0.95653553666708, "
        L"0.020889091780344 1.05877627113687, "
        L"0.062469481973487 1.15841173268892, "
        L"0.110752303032957 1.25497737480786, "
        L"0.165512438158711 1.3480229638936, "
        L"0.226494570322478 1.43711467845823, "
        L"0.293414372673605 1.52183713180202, "
        L"0.365959834202084 1.60179530873799, "
        L"0.443792714477911 1.67661640733506, "
        L"0.52655012068411 1.74595157709274, "
        L"0.613846199590564 1.80947754544319, "
        L"0.705273936579888 1.86689812499717, "
        L"0.800407053337464 1.91794559450637, "
        L"0.898801995357723 1.96238194710347, "
        L"1 2, 3 0, 3 2)");
#endif
    linearizedGeometry = FdoSpatialUtility::ApproximateGeometryWithLineStrings(cs, 0.2, 0.0, gf);
    printf("Linearized 2D CurveString with zero maxOffset:\n");
    // Geometry should be very changed (all linear -- no arcs -- similar to 2D case above).
	DumpGeometry(linearizedGeometry);
	CheckFGFT(linearizedGeometry,
        L"LINESTRING (0 0, "
        L"-0.049331651274743 0.184450583333957, "
        L"-0.076070558282857 0.37350258771831, "
        L"-0.07982680754939 0.564399209221096, "
        L"-0.060545624501699 0.754356745239048, "
        L"-0.018508172206063 0.940605187149905, "
        L"0.045672548622951 1.12042861327564, "
        L"0.131060638553256 1.29120479313625, "
        L"0.236410946956438 1.45044342547491, "
        L"0.360187229107742 1.59582245245793, "
        L"0.500584548093489 1.72522192050685, "
        L"0.655555594855637 1.8367548939953, "
        L"0.822840542567988 1.92879497101942, "
        L"1 2, 3 0, 3 2)");

    // Test non-zero maxOffset that is superceded by maxSpacing. (opposite precedence is covered in first case in this test).
    linearizedGeometry = FdoSpatialUtility::ApproximateGeometryWithLineStrings(cs, 0.2, 1.0, gf);
    printf("Linearized 2D CurveString with zero maxOffset:\n");
    // Geometry should be very changed (all linear -- no arcs -- as 2D case above with zero maxOffset).
	DumpGeometry(linearizedGeometry);
	CheckFGFT(linearizedGeometry,
        L"LINESTRING (0 0, "
        L"-0.049331651274743 0.184450583333957, "
        L"-0.076070558282857 0.37350258771831, "
        L"-0.07982680754939 0.564399209221096, "
        L"-0.060545624501699 0.754356745239048, "
        L"-0.018508172206063 0.940605187149905, "
        L"0.045672548622951 1.12042861327564, "
        L"0.131060638553256 1.29120479313625, "
        L"0.236410946956438 1.45044342547491, "
        L"0.360187229107742 1.59582245245793, "
        L"0.500584548093489 1.72522192050685, "
        L"0.655555594855637 1.8367548939953, "
        L"0.822840542567988 1.92879497101942, "
        L"1 2, 3 0, 3 2)");

    // Test interpolation of M ordinate.

    cs = gf->CreateGeometry(L"CURVESTRING XYM (0 0 5 (CIRCULARARCSEGMENT (0 1 7, 1 2 10), LINESTRINGSEGMENT (3 0 12, 3 2 15)))");
	DumpGeometry(cs);
	CheckFGFT(cs, L"CURVESTRING XYM (0 0 5 (CIRCULARARCSEGMENT (0 1 7, 1 2 10), LINESTRINGSEGMENT (3 0 12, 3 2 15)))");
    linearizedGeometry = FdoSpatialUtility::ApproximateGeometryWithLineStrings(cs, 0.0, 0.01, gf);
    printf("Linearized XYM CurveString:\n");
    // Geometry should be very changed (all linear -- no arcs -- same as first 2D case in this test).
	DumpGeometry(linearizedGeometry);
	CheckFGFT(linearizedGeometry,
        L"LINESTRING XYM (0 0 5, "
        L"-0.073652335250893 0.34631744484356 5.71428571428571, "
        L"-0.068395171412408 0.700341174725127 6.42857142857143, "
        L"0.015507875368588 1.04431896155409 7.14285714285714, "
        L"0.173849555977885 1.36100232277268 7.85714285714286, "
        L"0.398689957372453 1.63451143229485 8.57142857142857, "
        L"0.678754642974616 1.85113140129458 9.28571428571428, "
        L"1 2 10, 3 0 12, 3 2 15)");
    cs = gf->CreateGeometry(L"CURVESTRING XYZM (0 0 0 5 (CIRCULARARCSEGMENT (0 1 0.1 7, 1 2 0 10), LINESTRINGSEGMENT (3 0 0 12, 3 2 -0.5 15)))");
	DumpGeometry(cs);
	CheckFGFT(cs, L"CURVESTRING XYZM (0 0 0 5 (CIRCULARARCSEGMENT (0 1 0.1 7, 1 2 0 10), LINESTRINGSEGMENT (3 0 0 12, 3 2 -0.5 15)))");
    linearizedGeometry = FdoSpatialUtility::ApproximateGeometryWithLineStrings(cs, 0.0, 0.01, gf);
    printf("Linearized XYZM CurveString:\n");
    // Geometry should be very changed (all linear -- no arcs -- same as first 2D case in this test).
	DumpGeometry(linearizedGeometry);
	CheckFGFT(linearizedGeometry,
        L"LINESTRING XYZM (0 0 0 5, "
        L"-0.069277197762034 0.300420851841192 0.043897524736526 5.625, "
        L"-0.077876573725876 0.609996763299854 0.076574991075161 6.25, "
        L"-0.025453058417013 0.916305298429713 0.096721141526374 6.875, "
        L"0.085889735651764 1.20705513217412 0.103527566087059 7.5, "
        L"0.251683926206021 1.47057926767578 0.096721141526374 8.125, "
        L"0.465276645124591 1.69630320100079 0.076574991075161 8.75, "
        L"0.718096999869825 1.87516924710491 0.043897524736526 9.375, "
        L"1 2 0 10, 3 0 0 12, 3 2 -0.5 15)");

    // Flat circles (in XY plane) are treated distinctly, not just from general 3D case, but
    // w.r.t. orientation.  Make sure that the counterclockwise case works.

    cs = gf->CreateGeometry(L"CURVESTRING (1 2 (CIRCULARARCSEGMENT (0 1, 0 0)))");
	DumpGeometry(cs);
    DumpGeometryToGinaFile(cs, true);
	CheckFGFT(cs, L"CURVESTRING (1 2 (CIRCULARARCSEGMENT (0 1, 0 0)))");
    linearizedGeometry = FdoSpatialUtility::ApproximateGeometryWithLineStrings(cs, 0.0, 0.01, gf);
    printf("Linearized 2D CurveString with counterclockwise orientation:\n");
    // Geometry should be very changed (all linear -- no arcs).
	DumpGeometry(linearizedGeometry);
    DumpGeometryToGinaFile(linearizedGeometry, false);
	CheckFGFT(linearizedGeometry,
        L"LINESTRING (1 2, "
        L"0.678754642974617 1.85113140129458, "
        L"0.398689957372453 1.63451143229485, "
        L"0.173849555977885 1.36100232277268, "
        L"0.015507875368588 1.04431896155409, "
        L"-0.068395171412408 0.700341174725128, "
        L"-0.073652335250893 0.34631744484356, "
        L"0 0)");

    // Test degenerate conditions.  They should result in just using known 
    // positions, much like collinear cases.

    cs = gf->CreateGeometry(L"CURVESTRING (0 0 (CIRCULARARCSEGMENT (0 0, 0 0), LINESTRINGSEGMENT (3 0, 3 2)))");
	DumpGeometry(cs);
	CheckFGFT(cs, L"CURVESTRING (0 0 (CIRCULARARCSEGMENT (0 0, 0 0), LINESTRINGSEGMENT (3 0, 3 2)))");
    printf("CurveString with arc defined by three identical positions:\n");
    linearizedGeometry = FdoSpatialUtility::ApproximateGeometryWithLineStrings(cs, 0.0, 0.01, gf);
    printf("Linearized 2D CurveString with counterclockwise orientation:\n");
	CheckFGFT(linearizedGeometry, L"LINESTRING (0 0, 3 0, 3 2)");

    cs = gf->CreateGeometry(L"CURVESTRING (0 0 (CIRCULARARCSEGMENT (0 0, 1 2), LINESTRINGSEGMENT (3 0, 3 2)))");
	DumpGeometry(cs);
	CheckFGFT(cs, L"CURVESTRING (0 0 (CIRCULARARCSEGMENT (0 0, 1 2), LINESTRINGSEGMENT (3 0, 3 2)))");
    printf("CurveString with arc having identical start and mid positions:\n");
    linearizedGeometry = FdoSpatialUtility::ApproximateGeometryWithLineStrings(cs, 0.0, 0.01, gf);
    printf("Linearized 2D CurveString with counterclockwise orientation:\n");
	CheckFGFT(linearizedGeometry, L"LINESTRING (0 0, 1 2, 3 0, 3 2)");

    cs = gf->CreateGeometry(L"CURVESTRING (0 0 (CIRCULARARCSEGMENT (1 2, 1 2), LINESTRINGSEGMENT (3 0, 3 2)))");
	DumpGeometry(cs);
	CheckFGFT(cs, L"CURVESTRING (0 0 (CIRCULARARCSEGMENT (1 2, 1 2), LINESTRINGSEGMENT (3 0, 3 2)))");
    printf("CurveString with arc having identical mid and end positions:\n");
    linearizedGeometry = FdoSpatialUtility::ApproximateGeometryWithLineStrings(cs, 0.0, 0.01, gf);
    printf("Linearized 2D CurveString with counterclockwise orientation:\n");
	CheckFGFT(linearizedGeometry, L"LINESTRING (0 0, 1 2, 3 0, 3 2)");

    cs = gf->CreateGeometry(L"CURVESTRING XYZ (0 0 0 (CIRCULARARCSEGMENT (0 0 1, 0 0 0), LINESTRINGSEGMENT (3 0 0, 3 2 0)))");
	DumpGeometry(cs);
	CheckFGFT(cs, L"CURVESTRING XYZ (0 0 0 (CIRCULARARCSEGMENT (0 0 1, 0 0 0), LINESTRINGSEGMENT (3 0 0, 3 2 0)))");
    printf("CurveString with arc having identical start&end positions, and mid pos differing only in Z (degenerate 'vertical' circle):\n");
    linearizedGeometry = FdoSpatialUtility::ApproximateGeometryWithLineStrings(cs, 0.0, 0.01, gf);
    printf("Linearized 2D CurveString with counterclockwise orientation:\n");
	CheckFGFT(linearizedGeometry, L"LINESTRING XYZ (0 0 0, 0 0 1, 0 0 0, 3 0 0, 3 2 0)");

    // Test fail-safe limit of 5000 positions per arc (use a tiny tolerance).

    cs = gf->CreateGeometry(L"CURVESTRING (0 0 (CIRCULARARCSEGMENT (0 1, 1 2), LINESTRINGSEGMENT (3 0, 3 2)))");
	DumpGeometry(cs);
	CheckFGFT(cs, L"CURVESTRING (0 0 (CIRCULARARCSEGMENT (0 1, 1 2), LINESTRINGSEGMENT (3 0, 3 2)))");
    linearizedGeometry = FdoSpatialUtility::ApproximateGeometryWithLineStrings(cs, 0.0, 0.00000001, gf);
    printf("Linearized 2D CurveString with tiny maxOffset -- expect fail-safe 5000 positions for arc:\n");
    // Geometry should be a LineString with 5002 positions (5000 for arc, 2 
    // more for original linestringsegment).
    CPPUNIT_ASSERT_MESSAGE("Wrong type for linearized geometry.",
        linearizedGeometry->GetDerivedType() == FdoGeometryType_LineString);
    FdoILineString * ls = static_cast<FdoILineString *>(linearizedGeometry.p);
    FdoInt32 count = ls->GetCount();
    CPPUNIT_ASSERT_MESSAGE("Wrong # of positions for linearized geometry.", count == 5002);
}

void SpatialUtilityTest::testLinearizeCurvePolygon()
{
	FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();

	FdoPtr<FdoICurvePolygon> curvePoly = CreateCurvePolygon(100);
	DumpCurvePolygon(curvePoly);
	CheckFGFT(curvePoly, L"CURVEPOLYGON ((200 200 (CIRCULARARCSEGMENT (200 201, 201 202), LINESTRINGSEGMENT (200 200))), (300 300 (CIRCULARARCSEGMENT (300 301, 301 302), LINESTRINGSEGMENT (300 300))), (400 400 (CIRCULARARCSEGMENT (400 401, 401 402), LINESTRINGSEGMENT (400 400))))");

    FdoPtr<FdoIGeometry> linearizedGeometry =
        FdoSpatialUtility::ApproximateGeometryWithLineStrings(curvePoly, 1.0, 0.001, gf);

    printf("Linearized 2D CurvePolygon:\n");
    // Geometry should be very changed (all linear -- no arcs).
	DumpGeometry(linearizedGeometry);

	CheckFGFT(linearizedGeometry,
        L"POLYGON ((200 200, "
        L"199.969375639532 200.103529235452, "
        L"199.945887756397 200.208907000626, "
        L"199.92964586195 200.315641975738, "
        L"199.9207256834 200.423236513062, "
        L"199.919168810736 200.531188957197, "
        L"199.924982502819 200.638995984017, "
        L"199.938139653531 200.746154947396, "
        L"199.958578918163 200.85216622278, "
        L"199.986204999427 200.956535536667, "
        L"200.02088909178 201.058776271137, "
        L"200.062469481973 201.158411732689, "
        L"200.110752303033 201.254977374808, "
        L"200.165512438159 201.348022963894, "
        L"200.226494570322 201.437114678458, "
        L"200.293414372674 201.521837131802, "
        L"200.365959834202 201.601795308738, "
        L"200.443792714478 201.676616407335, "
        L"200.526550120684 201.745951577093, "
        L"200.613846199591 201.809477545443, "
        L"200.70527393658 201.866898124997, "
        L"200.800407053337 201.917945594506, "
        L"200.898801995358 201.962381947103, "
        L"201 202, 200 200), "
        L"(300 300, "
        L"299.969375639532 300.103529235452, "
        L"299.945887756397 300.208907000626, "
        L"299.92964586195 300.315641975738, "
        L"299.9207256834 300.423236513062, "
        L"299.919168810736 300.531188957197, "
        L"299.924982502819 300.638995984017, "
        L"299.938139653531 300.746154947396, "
        L"299.958578918163 300.85216622278, "
        L"299.986204999427 300.956535536667, "
        L"300.02088909178 301.058776271137, "
        L"300.062469481973 301.158411732689, "
        L"300.110752303033 301.254977374808, "
        L"300.165512438159 301.348022963894, "
        L"300.226494570322 301.437114678458, "
        L"300.293414372674 301.521837131802, "
        L"300.365959834202 301.601795308738, "
        L"300.443792714478 301.676616407335, "
        L"300.526550120684 301.745951577093, "
        L"300.613846199591 301.809477545443, "
        L"300.70527393658 301.866898124997, "
        L"300.800407053337 301.917945594506, "
        L"300.898801995358 301.962381947103, "
        L"301 302, "
        L"300 300), "
        L"(400 400, "
        L"399.969375639532 400.103529235452, "
        L"399.945887756397 400.208907000626, "
        L"399.92964586195 400.315641975738, "
        L"399.9207256834 400.423236513062, "
        L"399.919168810736 400.531188957197, "
        L"399.924982502819 400.638995984017, "
        L"399.938139653531 400.746154947396, "
        L"399.958578918163 400.85216622278, "
        L"399.986204999427 400.956535536667, "
        L"400.02088909178 401.058776271137, "
        L"400.062469481973 401.158411732689, "
        L"400.110752303033 401.254977374808, "
        L"400.165512438159 401.348022963894, "
        L"400.226494570322 401.437114678458, "
        L"400.293414372674 401.521837131802, "
        L"400.365959834202 401.601795308738, "
        L"400.443792714478 401.676616407335, "
        L"400.526550120684 401.745951577093, "
        L"400.613846199591 401.809477545443, "
        L"400.70527393658 401.866898124997, "
        L"400.800407053337 401.917945594506, "
        L"400.898801995358 401.962381947103, "
        L"401 402, 400 400))");
}

void SpatialUtilityTest::testLinearizeMultiCurveString()
{
	FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();

	FdoPtr<FdoICurveString> curveString1 = CreateCurveString(100);
	FdoPtr<FdoICurveString> curveString2 = CreateCurveString(200);
	FdoPtr<FdoICurveString> curveString3 = CreateCurveString(300);

    FdoPtr<FdoCurveStringCollection> curveStrings = FdoCurveStringCollection::Create();
	curveStrings->Add(curveString1);
	curveStrings->Add(curveString2);
	curveStrings->Add(curveString3);

	FdoPtr<FdoIMultiCurveString> multiCurveString = gf->CreateMultiCurveString(curveStrings);
	DumpMultiCurveString(multiCurveString);
	CheckFGFT(multiCurveString, L"MULTICURVESTRING ((100 100 (CIRCULARARCSEGMENT (100 101, 101 102), LINESTRINGSEGMENT (103 100, 103 102))), (200 200 (CIRCULARARCSEGMENT (200 201, 201 202), LINESTRINGSEGMENT (203 200, 203 202))), (300 300 (CIRCULARARCSEGMENT (300 301, 301 302), LINESTRINGSEGMENT (303 300, 303 302))))");

    FdoPtr<FdoIGeometry> linearizedGeometry =
        FdoSpatialUtility::ApproximateGeometryWithLineStrings(multiCurveString, 1.0, 0.01, gf);

    printf("Linearized 2D MultiCurveString:\n");
    // Geometry should be very changed (all linear -- no arcs).
	DumpGeometry(linearizedGeometry);

	CheckFGFT(linearizedGeometry,
        L"MULTILINESTRING ((100 100, "
        L"99.9263476647491 100.346317444844, "
        L"99.9316048285876 100.700341174725, "
        L"100.015507875369 101.044318961554, "
        L"100.173849555978 101.361002322773, "
        L"100.398689957372 101.634511432295, "
        L"100.678754642975 101.851131401295, "
        L"101 102, "
        L"103 100, "
        L"103 102), "
        L"(200 200, "
        L"199.926347664749 200.346317444844, "
        L"199.931604828588 200.700341174725, "
        L"200.015507875369 201.044318961554, "
        L"200.173849555978 201.361002322773, "
        L"200.398689957372 201.634511432295, "
        L"200.678754642975 201.851131401295, "
        L"201 202, "
        L"203 200, "
        L"203 202), "
        L"(300 300, "
        L"299.926347664749 300.346317444844, "
        L"299.931604828588 300.700341174725, "
        L"300.015507875369 301.044318961554, "
        L"300.173849555978 301.361002322773, "
        L"300.398689957372 301.634511432295, "
        L"300.678754642975 301.851131401295, "
        L"301 302, "
        L"303 300, "
        L"303 302))");
}

void SpatialUtilityTest::testLinearizeMultiCurvePolygon()
{
	FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();

	FdoPtr<FdoIMultiCurvePolygon> multiCurvePoly = CreateMultiCurvePolygon(3, 100);
	DumpMultiCurvePolygon(multiCurvePoly);
	CheckFGFT(multiCurvePoly, L"MULTICURVEPOLYGON (((200 200 (CIRCULARARCSEGMENT (200 201, 201 202), LINESTRINGSEGMENT (200 200))), (300 300 (CIRCULARARCSEGMENT (300 301, 301 302), LINESTRINGSEGMENT (300 300))), (400 400 (CIRCULARARCSEGMENT (400 401, 401 402), LINESTRINGSEGMENT (400 400)))), ((201 201 (CIRCULARARCSEGMENT (201 202, 202 203), LINESTRINGSEGMENT (201 201))), (301 301 (CIRCULARARCSEGMENT (301 302, 302 303), LINESTRINGSEGMENT (301 301))), (401 401 (CIRCULARARCSEGMENT (401 402, 402 403), LINESTRINGSEGMENT (401 401)))), ((202 202 (CIRCULARARCSEGMENT (202 203, 203 204), LINESTRINGSEGMENT (202 202))), (302 302 (CIRCULARARCSEGMENT (302 303, 303 304), LINESTRINGSEGMENT (302 302))), (402 402 (CIRCULARARCSEGMENT (402 403, 403 404), LINESTRINGSEGMENT (402 402)))))");

    FdoPtr<FdoIGeometry> linearizedGeometry =
        FdoSpatialUtility::ApproximateGeometryWithLineStrings(multiCurvePoly, 1.0, 0.01, gf);

    printf("Linearized 2D MultiCurvePolygon:\n");
    // Geometry should be very changed (all linear -- no arcs).
	DumpGeometry(linearizedGeometry);

	CheckFGFT(linearizedGeometry,
        L"MULTIPOLYGON (((200 200, "
        L"199.926347664749 200.346317444844, "
        L"199.931604828588 200.700341174725, "
        L"200.015507875369 201.044318961554, "
        L"200.173849555978 201.361002322773, "
        L"200.398689957372 201.634511432295, "
        L"200.678754642975 201.851131401295, "
        L"201 202, "
        L"200 200), "
        L"(300 300, "
        L"299.926347664749 300.346317444844, "
        L"299.931604828588 300.700341174725, "
        L"300.015507875369 301.044318961554, "
        L"300.173849555978 301.361002322773, "
        L"300.398689957372 301.634511432295, "
        L"300.678754642975 301.851131401295, "
        L"301 302, "
        L"300 300), "
        L"(400 400, "
        L"399.926347664749 400.346317444844, "
        L"399.931604828588 400.700341174725, "
        L"400.015507875369 401.044318961554, "
        L"400.173849555978 401.361002322773, "
        L"400.398689957372 401.634511432295, "
        L"400.678754642975 401.851131401295, "
        L"401 402, "
        L"400 400)), "
        L"((201 201, "
        L"200.926347664749 201.346317444844, "
        L"200.931604828588 201.700341174725, "
        L"201.015507875369 202.044318961554, "
        L"201.173849555978 202.361002322773, "
        L"201.398689957372 202.634511432295, "
        L"201.678754642975 202.851131401295, "
        L"202 203, "
        L"201 201), "
        L"(301 301, "
        L"300.926347664749 301.346317444844, "
        L"300.931604828588 301.700341174725, "
        L"301.015507875369 302.044318961554, "
        L"301.173849555978 302.361002322773, "
        L"301.398689957372 302.634511432295, "
        L"301.678754642975 302.851131401295, "
        L"302 303, "
        L"301 301), "
        L"(401 401, "
        L"400.926347664749 401.346317444844, "
        L"400.931604828588 401.700341174725, "
        L"401.015507875369 402.044318961554, "
        L"401.173849555978 402.361002322773, "
        L"401.398689957372 402.634511432295, "
        L"401.678754642975 402.851131401295, "
        L"402 403, "
        L"401 401)), "
        L"((202 202, "
        L"201.926347664749 202.346317444844, "
        L"201.931604828588 202.700341174725, "
        L"202.015507875369 203.044318961554, "
        L"202.173849555978 203.361002322773, "
        L"202.398689957372 203.634511432295, "
        L"202.678754642975 203.851131401295, "
        L"203 204, "
        L"202 202), "
        L"(302 302, "
        L"301.926347664749 302.346317444844, "
        L"301.931604828588 302.700341174725, "
        L"302.015507875369 303.044318961554, "
        L"302.173849555978 303.361002322773, "
        L"302.398689957372 303.634511432295, "
        L"302.678754642975 303.851131401295, "
        L"303 304, "
        L"302 302), "
        L"(402 402, "
        L"401.926347664749 402.346317444844, "
        L"401.931604828588 402.700341174725, "
        L"402.015507875369 403.044318961554, "
        L"402.173849555978 403.361002322773, "
        L"402.398689957372 403.634511432295, "
        L"402.678754642975 403.851131401295, "
        L"403 404, "
        L"402 402)))");
}

void SpatialUtilityTest::testGridTransform()
{
	FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();

    FdoPtr<FdoIEnvelope> extents = gf->CreateEnvelopeXY(0, 0, 1000, 1000);

    // With a tolerance of 0.5, the grid has a resolution of 1.0, effectively
    // making round-trips through it a rounding exercise.
    FdoSpatialGridTransformP gt = FdoSpatialGridTransform::Create(extents, 0.5);

    FdoInt64 ix = -1;
    FdoInt64 iy = -1;
    gt->TransformToGrid(10, 10, &ix, &iy);
    double x = -12345.0;
    double y = -67890.0;
    gt->TransformFromGrid(ix, iy, &x, &y);
	CPPUNIT_ASSERT_MESSAGE("X value mismatch", FUZZY_EQUALS(x, 10));
	CPPUNIT_ASSERT_MESSAGE("Y value mismatch", FUZZY_EQUALS(y, 10));

    gt->TransformToGrid(10.1, 10.1, &ix, &iy);
    gt->TransformFromGrid(ix, iy, &x, &y);
	CPPUNIT_ASSERT_MESSAGE("X value mismatch", FUZZY_EQUALS(x, 10));
	CPPUNIT_ASSERT_MESSAGE("Y value mismatch", FUZZY_EQUALS(y, 10));

    gt->TransformToGrid(9.9, 9.9, &ix, &iy);
    gt->TransformFromGrid(ix, iy, &x, &y);
	CPPUNIT_ASSERT_MESSAGE("X value mismatch", FUZZY_EQUALS(x, 10));
	CPPUNIT_ASSERT_MESSAGE("Y value mismatch", FUZZY_EQUALS(y, 10));

    // Note that the extents are just a parameter for determining scale.
    // The grid transform tool doesn't really check for exceeding the extents.
    gt->TransformToGrid(-10.1, -10.1, &ix, &iy);
    gt->TransformFromGrid(ix, iy, &x, &y);
	CPPUNIT_ASSERT_MESSAGE("X value mismatch", FUZZY_EQUALS(x, -10));
	CPPUNIT_ASSERT_MESSAGE("Y value mismatch", FUZZY_EQUALS(y, -10));

    gt->TransformToGrid(-9.9, -9.9, &ix, &iy);
    gt->TransformFromGrid(ix, iy, &x, &y);
	CPPUNIT_ASSERT_MESSAGE("X value mismatch", FUZZY_EQUALS(x, -10));
	CPPUNIT_ASSERT_MESSAGE("Y value mismatch", FUZZY_EQUALS(y, -10));
}


void SpatialUtilityTest::testGetExtents()
{
	FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();

    FdoPtr<FdoIGeometry> pt =   gf->CreateGeometry(L"POINT (5 3)");
    FdoPtr<FdoIGeometry> ls =   gf->CreateGeometry(L"LINESTRING (0 1, 2 3, 4 5)");
    FdoPtr<FdoIGeometry> pg =   gf->CreateGeometry(L"POLYGON ((0 0, 5 0, 5 5, 0 5, 0 0), (1 1, 2 1, 2 2, 1 1), (3 3, 4 3, 4 4, 3 3))");
    FdoPtr<FdoIGeometry> mpt =  gf->CreateGeometry(L"MULTIPOINT XYZ (1 2 3, 4 5 6, 7 8 9)");
    FdoPtr<FdoIGeometry> mls =  gf->CreateGeometry(L"MULTILINESTRING XYZ ((0 1 2, 3 4 5, 6 7 8), (9 10 11, 12 13 14, 15 16 17))");
    FdoPtr<FdoIGeometry> mpg =  gf->CreateGeometry(L"MULTIPOLYGON (((0 0, 5 0, 5 5, 0 5, 0 0), (1 1, 2 1, 2 2, 1 1), (3 3, 4 3, 4 4, 3 3)), ((0 0, 5 0, 5 5, 0 5, 0 0), (1 1, 2 1, 2 2, 1 1), (3 3, 4 3, 4 4, 3 3)))");
    FdoPtr<FdoIGeometry> mg =   gf->CreateGeometry(L"GEOMETRYCOLLECTION (CURVEPOLYGON ((100 100 (CIRCULARARCSEGMENT (100 101, 101 102), LINESTRINGSEGMENT (100 100))), (200 200 (CIRCULARARCSEGMENT (200 201, 201 202), LINESTRINGSEGMENT (200 200))), (300 300 (CIRCULARARCSEGMENT (300 301, 301 302), LINESTRINGSEGMENT (300 300)))), CURVESTRING (100 100 (CIRCULARARCSEGMENT (100 101, 101 102), LINESTRINGSEGMENT (103 100, 103 102))), LINESTRING (0 1, 2 3, 4 5), POINT XYZ (5 3 2), POLYGON ((0 0, 5 0, 5 5, 0 5, 0 0), (1 1, 2 1, 2 2, 1 1), (3 3, 4 3, 4 4, 3 3)))");
    FdoPtr<FdoIGeometry> cs =   gf->CreateGeometry(L"CURVESTRING (0 0 (CIRCULARARCSEGMENT (0 1, 1 2), LINESTRINGSEGMENT (3 0, 3 2)))");
    FdoPtr<FdoIGeometry> csFlatXYZ = gf->CreateGeometry(L"CURVESTRING XYZ (0 0 0 (CIRCULARARCSEGMENT (0 1 0, 1 2 0), LINESTRINGSEGMENT (3 0 0, 3 2 0)))");
    FdoPtr<FdoIGeometry> csFlatXYZ180 = gf->CreateGeometry(L"CURVESTRING XYZ (0 1 0 (CIRCULARARCSEGMENT (1 0 0, 0 -1 0)))");
    FdoPtr<FdoIGeometry> csXYZ = gf->CreateGeometry(L"CURVESTRING XYZ (0 0 -0.1 (CIRCULARARCSEGMENT (0 1 0, 1 2 0.1), LINESTRINGSEGMENT (3 0 0, 3 2 0)))");
    FdoPtr<FdoIGeometry> csColl = gf->CreateGeometry(L"CURVESTRING (0 0 (CIRCULARARCSEGMENT (1 1, 2 2), LINESTRINGSEGMENT (3 0, 3 2)))");
    FdoPtr<FdoIGeometry> csCollXYZ = gf->CreateGeometry(L"CURVESTRING XYZ (0 0 -0.1 (CIRCULARARCSEGMENT (1 1 0, 2 2 0.1), LINESTRINGSEGMENT (3 0 0, 3 2 0)))");
    FdoPtr<FdoIGeometry> cpg =  gf->CreateGeometry(L"CURVEPOLYGON ((200 200 (CIRCULARARCSEGMENT (200 201, 201 202), LINESTRINGSEGMENT (200 200))), (300 300 (CIRCULARARCSEGMENT (300 301, 301 302), LINESTRINGSEGMENT (300 300))), (400 400 (CIRCULARARCSEGMENT (400 401, 401 402), LINESTRINGSEGMENT (400 400))))");
    FdoPtr<FdoIGeometry> mcs =  gf->CreateGeometry(L"MULTICURVESTRING ((100 100 (CIRCULARARCSEGMENT (100 101, 101 102), LINESTRINGSEGMENT (103 100, 103 102))), (200 200 (CIRCULARARCSEGMENT (200 201, 201 202), LINESTRINGSEGMENT (203 200, 203 202))), (300 300 (CIRCULARARCSEGMENT (300 301, 301 302), LINESTRINGSEGMENT (303 300, 303 302))))");
    FdoPtr<FdoIGeometry> mcpg = gf->CreateGeometry(L"MULTICURVEPOLYGON (((200 200 (CIRCULARARCSEGMENT (200 201, 201 202), LINESTRINGSEGMENT (200 200))), (300 300 (CIRCULARARCSEGMENT (300 301, 301 302), LINESTRINGSEGMENT (300 300))), (400 400 (CIRCULARARCSEGMENT (400 401, 401 402), LINESTRINGSEGMENT (400 400)))), ((201 201 (CIRCULARARCSEGMENT (201 202, 202 203), LINESTRINGSEGMENT (201 201))), (301 301 (CIRCULARARCSEGMENT (301 302, 302 303), LINESTRINGSEGMENT (301 301))), (401 401 (CIRCULARARCSEGMENT (401 402, 402 403), LINESTRINGSEGMENT (401 401)))), ((202 202 (CIRCULARARCSEGMENT (202 203, 203 204), LINESTRINGSEGMENT (202 202))), (302 302 (CIRCULARARCSEGMENT (302 303, 303 304), LINESTRINGSEGMENT (302 302))), (402 402 (CIRCULARARCSEGMENT (402 403, 403 404), LINESTRINGSEGMENT (402 402)))))");
    FdoPtr<FdoIGeometry> nnullZ1 =   gf->CreateGeometry(L"CURVESTRING XYZ (0 0 10.2 (CIRCULARARCSEGMENT (0 1 10.2, 1 2 10.5)))");
    FdoPtr<FdoIGeometry> nullZ1 =    gf->CreateGeometry(L"CURVESTRING XYZ (0 0 -1.25e126 (CIRCULARARCSEGMENT (0 1 10.2, 1 2 10.5)))");
    FdoPtr<FdoIGeometry> nnullZ2 =   gf->CreateGeometry(L"CURVESTRING XYZ (0 0  10 (CIRCULARARCSEGMENT (0 1 10, 1 2 10.5)))");
    FdoPtr<FdoIGeometry> nullZ2 =    gf->CreateGeometry(L"CURVESTRING XYZ (0 0  10 (CIRCULARARCSEGMENT (0 1 -1.25e126, 1 2 10.5)))");
    FdoPtr<FdoIGeometry> nnullZ3 =   gf->CreateGeometry(L"CURVESTRING XYZ (0 0  10 (CIRCULARARCSEGMENT (0 1 10.2, 1 2 10.2)))");
    FdoPtr<FdoIGeometry> nullZ3 =    gf->CreateGeometry(L"CURVESTRING XYZ (0 0  10 (CIRCULARARCSEGMENT (0 1 10.2, 1 2 -1.25e126)))");
    FdoPtr<FdoIGeometry> nnullZ12 =  gf->CreateGeometry(L"CURVESTRING XYZ (0 0  10.5 (CIRCULARARCSEGMENT (0 1 10.5, 1 2 10.5)))");
    FdoPtr<FdoIGeometry> nullZ12 =   gf->CreateGeometry(L"CURVESTRING XYZ (0 0 -1.25e126 (CIRCULARARCSEGMENT (0 1 -1.25e126, 1 2 10.5)))");
    FdoPtr<FdoIGeometry> nnullZ13 =  gf->CreateGeometry(L"CURVESTRING XYZ (0 0 10.2 (CIRCULARARCSEGMENT (0 1 10.2, 1 2 10.2)))");
    FdoPtr<FdoIGeometry> nullZ13 =   gf->CreateGeometry(L"CURVESTRING XYZ (0 0 -1.25e126 (CIRCULARARCSEGMENT (0 1 10.2, 1 2 -1.25e126)))");
    FdoPtr<FdoIGeometry> nnullZ23 =  gf->CreateGeometry(L"CURVESTRING XYZ (0 0  10 (CIRCULARARCSEGMENT (0 1 10, 1 2 10)))");
    FdoPtr<FdoIGeometry> nullZ23 =   gf->CreateGeometry(L"CURVESTRING XYZ (0 0  10 (CIRCULARARCSEGMENT (0 1 -1.25e126, 1 2 -1.25e126)))");
    FdoPtr<FdoIGeometry> nnullZ123 = gf->CreateGeometry(L"CURVESTRING XYZ (0 0 0 (CIRCULARARCSEGMENT (0 1 0, 1 2 0)))");
    FdoPtr<FdoIGeometry> nullZ123 =  gf->CreateGeometry(L"CURVESTRING XYZ (0 0 -1.25e126 (CIRCULARARCSEGMENT (0 1 -1.25e126, 1 2 -1.25e126)))");

    double minX=0.0, minY=0.0, maxX=0.0, maxY=0.0, minZ=0.0, maxZ=0.0;
    double expMinX=0.0, expMinY=0.0, expMaxX=0.0, expMaxY=0.0;
    FdoInt32 numExceptions = 0;

    // Note:  It is invalid for a client to use Z at all for a geometry that lacks it.
    // However, we always check it here anyway, just to make sure that it is not junk.
    // GetExtents sets it to zero for 2D currently, though it might be corrected to NaN later.

    // Test Point
    FdoPtr<FdoByteArray> ba = gf->GetFgf(pt);
    FdoSpatialUtility::GetExtents(ba, minX, minY, maxX, maxY);
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(minX, 5.0));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(minY, 3.0));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(maxX, 5.0));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(maxY, 3.0));
    numExceptions = testGetExtentsMalformedSubsets(ba);
    CPPUNIT_ASSERT_MESSAGE("Wrong # of exceptions from malformed FGF test", 552==numExceptions);

    // Test LineString
    ba = gf->GetFgf(ls);
    FdoSpatialUtility::GetExtents(ba, minX, minY, maxX, maxY);
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(minX, 0.0));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(minY, 1.0));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(maxX, 4.0));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(maxY, 5.0));
    numExceptions = testGetExtentsMalformedSubsets(ba);
    CPPUNIT_ASSERT_MESSAGE("Wrong # of exceptions from malformed FGF test", 3460==numExceptions);

    // Test Polygon
    ba = gf->GetFgf(pg);
    FdoSpatialUtility::GetExtents(ba, minX, minY, maxX, maxY);
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(minX, 0.0));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(minY, 0.0));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(maxX, 5.0));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(maxY, 5.0));
    numExceptions = testGetExtentsMalformedSubsets(ba);
    CPPUNIT_ASSERT_MESSAGE("Wrong # of exceptions from malformed FGF test", 52392==numExceptions);

    // Test MultiPoint
    ba = gf->GetFgf(mpt);
    FdoSpatialUtility::GetExtents(ba, minX, minY, maxX, maxY);
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(minX, 1.0));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(minY, 2.0));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(maxX, 7.0));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(maxY, 8.0));
    FdoSpatialUtility::GetExtents(ba, minX, minY, minZ, maxX, maxY, maxZ);
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(minX, 1.0));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(minY, 2.0));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(minZ, 3.0));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(maxX, 7.0));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(maxY, 8.0));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(maxZ, 9.0));
    numExceptions = testGetExtentsMalformedSubsets(ba);
    CPPUNIT_ASSERT_MESSAGE("Wrong # of exceptions from malformed FGF test", 10136==numExceptions);

    // Test MultiLineString
    ba = gf->GetFgf(mls);
    FdoSpatialUtility::GetExtents(ba, minX, minY, maxX, maxY);
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(minX, 0.0));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(minY, 1.0));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(maxX, 15.0));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(maxY, 16.0));
    numExceptions = testGetExtentsMalformedSubsets(ba);
    CPPUNIT_ASSERT_MESSAGE("Wrong # of exceptions from malformed FGF test", 29736==numExceptions);

    // Test MultiPolygon
    ba = gf->GetFgf(mpg);
    FdoSpatialUtility::GetExtents(ba, minX, minY, maxX, maxY);
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(minX, 0.0));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(minY, 0.0));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(maxX, 5.0));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(maxY, 5.0));

    // Test MultiGeometry
    ba = gf->GetFgf(mg);
    FdoSpatialUtility::GetExtents(ba, minX, minY, maxX, maxY);
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(minX, 0.0));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(minY, 0.0));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(maxX, 301.0));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(maxY, 302.0));

    // Test CurveString
    ba = gf->GetFgf(cs);
    FdoSpatialUtility::GetExtents(ba, minX, minY, maxX, maxY);
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(minX, -0.081138830084189761));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(minY, 0.0));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(maxX, 3.0));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(maxY, 2.0));
    FdoSpatialUtility::GetExtents(ba, minX, minY, minZ, maxX, maxY, maxZ);
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(minX, -0.081138830084189761));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(minY, 0.0));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(minZ, 0.0));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(maxX, 3.0));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(maxY, 2.0));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(maxZ, 0.0));

    // Test CurveString with 3D, but is still "flat" (parallel to XY plane)
    ba = gf->GetFgf(csFlatXYZ);
    FdoSpatialUtility::GetExtents(ba, minX, minY, maxX, maxY);
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(minX, -0.081138830084189761));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(minY, 0.0));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(maxX, 3.0));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(maxY, 2.0));
    FdoSpatialUtility::GetExtents(ba, minX, minY, minZ, maxX, maxY, maxZ);
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(minX, -0.081138830084189761));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(minY, 0.0));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(minZ, 0.0));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(maxX, 3.0));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(maxY, 2.0));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(maxZ, 0.0));

    // Test CurveString with 180-degree 3D, but is still "flat" (parallel to XY plane)
    ba = gf->GetFgf(csFlatXYZ180);
    FdoSpatialUtility::GetExtents(ba, minX, minY, maxX, maxY);
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(minX, 0.0));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(minY, -1.0));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(maxX, 1.0));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(maxY, 1.0));
    FdoSpatialUtility::GetExtents(ba, minX, minY, minZ, maxX, maxY, maxZ);
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(minX, 0.0));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(minY, -1.0));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(minZ, 0.0));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(maxX, 1.0));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(maxY, 1.0));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(maxZ, 0.0));

    // Test CurveString with varying-Z arc
    ba = gf->GetFgf(csXYZ);
    FdoSpatialUtility::GetExtents(ba, minX, minY, maxX, maxY);
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(minX, -0.081414465184981299));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(minY, 0.0));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(maxX, 3.0));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(maxY, 2.0));
    FdoSpatialUtility::GetExtents(ba, minX, minY, minZ, maxX, maxY, maxZ);
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(minX, -0.081414465184981299));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(minY, 0.0));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(minZ, -0.1));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(maxX, 3.0));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(maxY, 2.0));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(maxZ, 0.1));

    // Test CurveString with arc defined by collinear positions.
    // It should fall back to using just the positions (not an exception).
    ba = gf->GetFgf(csColl);
    FdoSpatialUtility::GetExtents(ba, minX, minY, maxX, maxY);
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(minX, 0.0));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(minY, 0.0));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(maxX, 3.0));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(maxY, 2.0));
    FdoSpatialUtility::GetExtents(ba, minX, minY, minZ, maxX, maxY, maxZ);
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(minX, 0.0));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(minY, 0.0));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(minZ, 0.0));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(maxX, 3.0));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(maxY, 2.0));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(maxZ, 0.0));

    // Test CurveString with varying-Z arc, defined by collinear positions.
    // It should fall back to using just the positions (not an exception).
    ba = gf->GetFgf(csCollXYZ);
    FdoSpatialUtility::GetExtents(ba, minX, minY, maxX, maxY);
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(minX, 0));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(minY, 0.0));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(maxX, 3.0));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(maxY, 2.0));
    FdoSpatialUtility::GetExtents(ba, minX, minY, minZ, maxX, maxY, maxZ);
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(minX, 0));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(minY, 0.0));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(minZ, -0.1));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(maxX, 3.0));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(maxY, 2.0));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(maxZ, 0.1));

    // Test CurvePolygon
    ba = gf->GetFgf(cpg);
    FdoSpatialUtility::GetExtents(ba, minX, minY, maxX, maxY);
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(minX, 199.9188611699158));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(minY, 200.0));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(maxX, 401.0));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(maxY, 402.0));
    numExceptions = testGetExtentsMalformedSubsets(ba);
    CPPUNIT_ASSERT_MESSAGE("Wrong # of exceptions from malformed FGF test", 61508==numExceptions);

    // Test MultiCurveString
    ba = gf->GetFgf(mcs);
    FdoSpatialUtility::GetExtents(ba, minX, minY, maxX, maxY);
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(minX, 99.918861169915814));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(minY, 100.0));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(maxX, 303.0));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(maxY, 302.0));

    // Test MultiCurvePolygon
    ba = gf->GetFgf(mcpg);
    FdoSpatialUtility::GetExtents(ba, minX, minY, maxX, maxY);
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(minX, 199.9188611699158));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(minY, 200.0));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(maxX, 403.0));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(maxY, 404.0));

    // Test Arcs with null elevation(s)
    ba = gf->GetFgf(nnullZ1);
    FdoSpatialUtility::GetExtents(ba, expMinX, expMinY, expMaxX, expMaxY);
    ba = gf->GetFgf(nullZ1);
    FdoSpatialUtility::GetExtents(ba, minX, minY, maxX, maxY);
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(minX, expMinX));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(minY, expMinY));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(maxX, expMaxX));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(maxY, expMaxY));

    ba = gf->GetFgf(nnullZ2);
    FdoSpatialUtility::GetExtents(ba, expMinX, expMinY, expMaxX, expMaxY);
    ba = gf->GetFgf(nullZ2);
    FdoSpatialUtility::GetExtents(ba, minX, minY, maxX, maxY);
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(minX, expMinX));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(minY, expMinY));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(maxX, expMaxX));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(maxY, expMaxY));

    ba = gf->GetFgf(nnullZ3);
    FdoSpatialUtility::GetExtents(ba, expMinX, expMinY, expMaxX, expMaxY);
    ba = gf->GetFgf(nullZ3);
    FdoSpatialUtility::GetExtents(ba, minX, minY, maxX, maxY);
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(minX, expMinX));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(minY, expMinY));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(maxX, expMaxX));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(maxY, expMaxY));

    ba = gf->GetFgf(nnullZ12);
    FdoSpatialUtility::GetExtents(ba, expMinX, expMinY, expMaxX, expMaxY);
    ba = gf->GetFgf(nullZ12);
    FdoSpatialUtility::GetExtents(ba, minX, minY, maxX, maxY);
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(minX, expMinX));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(minY, expMinY));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(maxX, expMaxX));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(maxY, expMaxY));

    ba = gf->GetFgf(nnullZ13);
    FdoSpatialUtility::GetExtents(ba, expMinX, expMinY, expMaxX, expMaxY);
    ba = gf->GetFgf(nullZ13);
    FdoSpatialUtility::GetExtents(ba, minX, minY, maxX, maxY);
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(minX, expMinX));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(minY, expMinY));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(maxX, expMaxX));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(maxY, expMaxY));

    ba = gf->GetFgf(nnullZ23);
    FdoSpatialUtility::GetExtents(ba, expMinX, expMinY, expMaxX, expMaxY);
    ba = gf->GetFgf(nullZ23);
    FdoSpatialUtility::GetExtents(ba, minX, minY, maxX, maxY);
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(minX, expMinX));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(minY, expMinY));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(maxX, expMaxX));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(maxY, expMaxY));

    ba = gf->GetFgf(nnullZ123);
    FdoSpatialUtility::GetExtents(ba, expMinX, expMinY, expMaxX, expMaxY);
    ba = gf->GetFgf(nullZ123);
    FdoSpatialUtility::GetExtents(ba, minX, minY, maxX, maxY);
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(minX, expMinX));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(minY, expMinY));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(maxX, expMaxX));
	CPPUNIT_ASSERT_MESSAGE("Extent value mismatch", FUZZY_EQUALS(maxY, expMaxY));

}

FdoInt32 SpatialUtilityTest::testGetExtentsMalformedSubsets(FdoByteArray * ba)
{
    // This method puts GetExtents through some challenging tests with bad
    // input data.  GetExtents is the first FGF-interpreting function that
    // several FDO clients and providers use when reading FGF binary data,
    // so it is the most important place to check for malformed FGF.

    // We check the given byte array and all subsets of it, under the assumption
    // that most subsets will be invalid and generate an exception.  The number of
    // exceptions generated is returned.

    // Callers should check the returned value against a known outcome that was
    // computed while running under a memory-checking tool, such as Purify --
    // buffer overrun prevention is one error type that is likely to be triggered
    // here, so the attempts to cause it need verification by an external tool.

    // GetExtents is expected to throw an exception due to bad data.  The expected
    // exceptions are caught and checked here.  A unit test failure will result
    // if anything other than expected exceptions are thrown.

    FdoInt32 numExceptions = 0;

    unsigned char* bytes = ba->GetData();
    FdoInt32 byteCount = ba->GetCount();

    double minX=0.0, minY=0.0, maxX=0.0, maxY=0.0, minZ=0.0, maxZ=0.0;

    FdoStringP expected1 = FdoException::NLSGetMessage(FDO_15_UNSUPPORTEDGEOMETRYDATA, "Invalid Geometry.");
    FdoStringP expected2 = FdoException::NLSGetMessage(FDO_10_UNSUPPORTEDGEOMETRYTYPE, "Unsupported geometry type.");

    for (FdoInt32 startIndex=0;  startIndex < byteCount;  startIndex++)
    {
        for (FdoInt32 endIndex=startIndex+1;  endIndex < byteCount;  endIndex++)
        {
            FdoInt32 subsetByteCount = endIndex-startIndex;
            FdoPtr<FdoByteArray> ba = FdoByteArray::Create(bytes+startIndex, subsetByteCount);

            // Test the GetExtents with XY results.
            try
            {
                FdoSpatialUtility::GetExtents(ba, minX, minY, maxX, maxY);
            }
            catch(FdoException* ex)
            {
                FdoStringP message = ex->GetExceptionMessage();
                if (message.Contains(expected1) || message.Contains(expected2))
                    numExceptions++;
                ex->Release();
            }

            // Test the GetExtents with XYZ results.
            try
            {
                FdoSpatialUtility::GetExtents(ba, minX, minY, maxZ, maxX, maxY, maxZ);
            }
            catch(FdoException* ex)
            {
                FdoStringP message = ex->GetExceptionMessage();
                if (message.Contains(expected1) || message.Contains(expected2))
                    numExceptions++;
                ex->Release();
            }
        }
    }
    return numExceptions;
}

void SpatialUtilityTest::testGetExtentsMalformed()
{
	FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();

    double minX=0.0, minY=0.0, maxX=0.0, maxY=0.0, minZ=0.0, maxZ=0.0;

    // Test some hand-crafted FGF byte arrays that are malformed.

    // The following cases have incomplete metadata (e.g. array ends in middle
    // of a 32-bit integer).
    FdoByte bytes_1a [] = 
    {
        2, 0            // GeometryType == LineString, but incomplete integer
    };
    FdoByte bytes_1b [] = 
    {
        2, 0, 0, 0,     // GeometryType == LineString.
        0, 0            // Dimensionality == XY, but incomplete integer.
    };
    FdoByte bytes_1c [] = 
    {
        2, 0, 0, 0,     // GeometryType == LineString.
        0, 0, 0, 0,     // Dimensionality == XY.
        0, 0            // Partial integer for # of positions.
    };

    // The following cases have internal array sizes that are too big.
    FdoByte bytes_2a [] = 
    {
        2, 0, 0, 0,     // GeometryType == LineString.
        0, 0, 0, 0,     // Dimensionality == XY.
        0, 0, 0, 50     // Huge ordinate array (followed by no ordinates).
    };
    FdoByte bytes_2b [] = 
    {
        2, 0, 0, 0,     // GeometryType == LineString.
        0, 0, 0, 0,     // Dimensionality == XY.
        0, 0, 0, 50,    // Huge ordinate array (followed by insufficient ordinate data).
        0               // Incomplete ordinate.
    };
    FdoByte bytes_2c [] = 
    {
        2, 0, 0, 0,     // GeometryType == LineString.
        0, 0, 0, 0,     // Dimensionality == XY.
        0, 0, 0, 50,    // Huge ordinate array (followed by insufficient ordinate data).
        0,0,0,0,0,0,0,0 // Complete ordinate (8 bytes), but incomplete array.
    };

    FdoInt32 sizes [] =
    {
        sizeof(bytes_1a),
        sizeof(bytes_1b),
        sizeof(bytes_1c),
        sizeof(bytes_2a),
        sizeof(bytes_2b),
        sizeof(bytes_2c)
    };

    FdoByte * bytes [] =
    {
        bytes_1a,
        bytes_1b,
        bytes_1c,
        bytes_2a,
        bytes_2b,
        bytes_2c
    };

    FdoInt32 numArrays = sizeof(sizes) / sizeof(sizes[0]);

    for (FdoInt32 i=0;  i < numArrays;  i++)
    {
        try
        {
            FdoPtr<FdoByteArray> ba = FdoByteArray::Create(bytes[i], sizes[i]);
            FdoSpatialUtility::GetExtents(ba, minX, minY, maxX, maxY);
            UnitTestUtil::FailOnException( FdoException::Create(L"GetExtents should fail with 'Invalid Geometry.'"));
        }
        catch(FdoException* ex)
        {
            FdoStringP message = ex->GetExceptionMessage();
            FdoString * expected = FdoException::NLSGetMessage(FDO_15_UNSUPPORTEDGEOMETRYDATA, "Invalid Geometry.");
            if (message.Contains(expected))
            {
                printf("Expected exception: %ls\n", ex->GetExceptionMessage());
                ex->Release();
            }
            else
            {
                ex->Release();
                UnitTestUtil::FailOnException( FdoException::Create(L"GetExtents should fail with 'Invalid Geometry.'"));
            }
        }
    }
}

void SpatialUtilityTest::testFixPolygonVertexOrder()
{
    FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
    FdoPtr<FdoIGeometry> oldGeom;
    FdoPtr<FdoIGeometry> newGeom;

    /*************************** Test Polygon *************************/
    FdoStringP correctFgfTextWithCouterClockwiseOrder = L"POLYGON ((0 0, 5 0, 5 5, 0 5, 0 0), (1 1, 2 2, 2 1, 1 1), (3 3, 4 4, 4 3, 3 3))";
    FdoStringP correctFgfTextWithClockwiseOrder = L"POLYGON ((0 0, 0 5, 5 5, 5 0, 0 0), (1 1, 2 1, 2 2, 1 1), (3 3, 4 3, 4 4, 3 3))";
    FdoStringP wrongFgfText;

    oldGeom = gf->CreateGeometry(correctFgfTextWithCouterClockwiseOrder);
    CPPUNIT_ASSERT_MESSAGE("Faild to fix polygon!", FdoSpatialUtility::FixPolygonVertexOrder(oldGeom, FdoPolygonVertexOrderRule_None) == NULL);
    CPPUNIT_ASSERT_MESSAGE("Faild to fix polygon!", FdoSpatialUtility::FixPolygonVertexOrder(oldGeom, FdoPolygonVertexOrderRule_CCW) == NULL);

    newGeom = FdoSpatialUtility::FixPolygonVertexOrder(oldGeom, FdoPolygonVertexOrderRule_CW);
    CheckFGFT(newGeom, correctFgfTextWithClockwiseOrder);

    oldGeom = gf->CreateGeometry(correctFgfTextWithClockwiseOrder);
    CPPUNIT_ASSERT_MESSAGE("Faild to fix polygon!", FdoSpatialUtility::FixPolygonVertexOrder(oldGeom, FdoPolygonVertexOrderRule_CW) == NULL);
    CPPUNIT_ASSERT_MESSAGE("Faild to fix polygon!", FdoSpatialUtility::FixPolygonVertexOrder(oldGeom, FdoPolygonVertexOrderRule_None) == NULL);

    newGeom = FdoSpatialUtility::FixPolygonVertexOrder(oldGeom, FdoPolygonVertexOrderRule_CCW);
    CheckFGFT(newGeom, correctFgfTextWithCouterClockwiseOrder);

    // Exterior ring and interior rings are clock wise.
    wrongFgfText = L"POLYGON ((0 0, 0 5, 5 5, 5 0, 0 0), (1 1, 2 2, 2 1, 1 1), (3 3, 4 4, 4 3, 3 3))";
    oldGeom = gf->CreateGeometry(wrongFgfText);

    newGeom = FdoSpatialUtility::FixPolygonVertexOrder(oldGeom, FdoPolygonVertexOrderRule_CCW);
    CheckFGFT(newGeom, correctFgfTextWithCouterClockwiseOrder);

    newGeom = FdoSpatialUtility::FixPolygonVertexOrder(oldGeom, FdoPolygonVertexOrderRule_CW);
    CheckFGFT(newGeom, correctFgfTextWithClockwiseOrder);

    // Exterior ring and interior rings are all couter clock wise.
    wrongFgfText = L"POLYGON ((0 0, 5 0, 5 5, 0 5, 0 0), (1 1, 2 1, 2 2, 1 1), (3 3, 4 3, 4 4, 3 3))";
    oldGeom = gf->CreateGeometry(wrongFgfText);

    newGeom = FdoSpatialUtility::FixPolygonVertexOrder(oldGeom, FdoPolygonVertexOrderRule_CCW);
    CheckFGFT(newGeom, correctFgfTextWithCouterClockwiseOrder);

    newGeom = FdoSpatialUtility::FixPolygonVertexOrder(oldGeom, FdoPolygonVertexOrderRule_CW);
    CheckFGFT(newGeom, correctFgfTextWithClockwiseOrder);

    /*************************** Test MultiPolygon *************************/
    correctFgfTextWithCouterClockwiseOrder = L"MULTIPOLYGON (((0 0, 5 0, 5 5, 0 5, 0 0), (1 1, 2 2, 2 1, 1 1), (3 3, 4 4, 4 3, 3 3)), ((10 0, 20 0, 20 10, 10 10, 10 0)))";
    correctFgfTextWithClockwiseOrder = L"MULTIPOLYGON (((0 0, 0 5, 5 5, 5 0, 0 0), (1 1, 2 1, 2 2, 1 1), (3 3, 4 3, 4 4, 3 3)), ((10 0, 10 10, 20 10, 20 0, 10 0)))";

    oldGeom = gf->CreateGeometry(correctFgfTextWithCouterClockwiseOrder);
    CPPUNIT_ASSERT_MESSAGE("Faild to fix polygon!", FdoSpatialUtility::FixPolygonVertexOrder(oldGeom, FdoPolygonVertexOrderRule_None) == NULL);
    CPPUNIT_ASSERT_MESSAGE("Faild to fix polygon!", FdoSpatialUtility::FixPolygonVertexOrder(oldGeom, FdoPolygonVertexOrderRule_CCW) == NULL);

    newGeom = FdoSpatialUtility::FixPolygonVertexOrder(oldGeom, FdoPolygonVertexOrderRule_CW);
    CheckFGFT(newGeom, correctFgfTextWithClockwiseOrder);

    oldGeom = gf->CreateGeometry(correctFgfTextWithClockwiseOrder);
    CPPUNIT_ASSERT_MESSAGE("Faild to fix polygon!", FdoSpatialUtility::FixPolygonVertexOrder(oldGeom, FdoPolygonVertexOrderRule_CW) == NULL);
    CPPUNIT_ASSERT_MESSAGE("Faild to fix polygon!", FdoSpatialUtility::FixPolygonVertexOrder(oldGeom, FdoPolygonVertexOrderRule_None) == NULL);

    newGeom = FdoSpatialUtility::FixPolygonVertexOrder(oldGeom, FdoPolygonVertexOrderRule_CCW);
    CheckFGFT(newGeom, correctFgfTextWithCouterClockwiseOrder);

    // Exterior ring and interior rings are clock wise.
    wrongFgfText = L"MULTIPOLYGON (((0 0, 0 5, 5 5, 5 0, 0 0), (1 1, 2 2, 2 1, 1 1), (3 3, 4 4, 4 3, 3 3)), ((10 0, 10 10, 20 10, 20 0, 10 0)))";
    oldGeom = gf->CreateGeometry(wrongFgfText);

    newGeom = FdoSpatialUtility::FixPolygonVertexOrder(oldGeom, FdoPolygonVertexOrderRule_CCW);
    CheckFGFT(newGeom, correctFgfTextWithCouterClockwiseOrder);

    newGeom = FdoSpatialUtility::FixPolygonVertexOrder(oldGeom, FdoPolygonVertexOrderRule_CW);
    CheckFGFT(newGeom, correctFgfTextWithClockwiseOrder);

    // Exterior ring and interior rings are all couter clock wise.
    wrongFgfText = L"MULTIPOLYGON (((0 0, 5 0, 5 5, 0 5, 0 0), (1 1, 2 1, 2 2, 1 1), (3 3, 4 3, 4 4, 3 3)), ((10 0, 20 0, 20 10, 10 10, 10 0)))";
    oldGeom = gf->CreateGeometry(wrongFgfText);

    newGeom = FdoSpatialUtility::FixPolygonVertexOrder(oldGeom, FdoPolygonVertexOrderRule_CCW);
    CheckFGFT(newGeom, correctFgfTextWithCouterClockwiseOrder);

    newGeom = FdoSpatialUtility::FixPolygonVertexOrder(oldGeom, FdoPolygonVertexOrderRule_CW);
    CheckFGFT(newGeom, correctFgfTextWithClockwiseOrder);

    /*************************** Test CurvePolygon *************************/
    correctFgfTextWithCouterClockwiseOrder = L"CURVEPOLYGON ((0 0 (CIRCULARARCSEGMENT (100 -20, 200 0), LINESTRINGSEGMENT (200 200), CIRCULARARCSEGMENT (100 220, 0 200), LINESTRINGSEGMENT (0 0))), (50 50 (CIRCULARARCSEGMENT (75 75, 100 50), LINESTRINGSEGMENT (50 50))), (120 60 (CIRCULARARCSEGMENT (130 70, 140 60), LINESTRINGSEGMENT (120 60))))";
    correctFgfTextWithClockwiseOrder = L"CURVEPOLYGON ((0 0 (LINESTRINGSEGMENT (0 200), CIRCULARARCSEGMENT (100 220, 200 200), LINESTRINGSEGMENT (200 0), CIRCULARARCSEGMENT (100 -20, 0 0))), (50 50 (LINESTRINGSEGMENT (100 50), CIRCULARARCSEGMENT (75 75, 50 50))), (120 60 (LINESTRINGSEGMENT (140 60), CIRCULARARCSEGMENT (130 70, 120 60))))";

    oldGeom = gf->CreateGeometry(correctFgfTextWithCouterClockwiseOrder);
    CPPUNIT_ASSERT_MESSAGE("Faild to fix polygon!", FdoSpatialUtility::FixPolygonVertexOrder(oldGeom, FdoPolygonVertexOrderRule_None) == NULL);
    CPPUNIT_ASSERT_MESSAGE("Faild to fix polygon!", FdoSpatialUtility::FixPolygonVertexOrder(oldGeom, FdoPolygonVertexOrderRule_CCW) == NULL);

    newGeom = FdoSpatialUtility::FixPolygonVertexOrder(oldGeom, FdoPolygonVertexOrderRule_CW);
    CheckFGFT(newGeom, correctFgfTextWithClockwiseOrder);

    oldGeom = gf->CreateGeometry(correctFgfTextWithClockwiseOrder);
    CPPUNIT_ASSERT_MESSAGE("Faild to fix polygon!", FdoSpatialUtility::FixPolygonVertexOrder(oldGeom, FdoPolygonVertexOrderRule_CW) == NULL);
    CPPUNIT_ASSERT_MESSAGE("Faild to fix polygon!", FdoSpatialUtility::FixPolygonVertexOrder(oldGeom, FdoPolygonVertexOrderRule_None) == NULL);

    newGeom = FdoSpatialUtility::FixPolygonVertexOrder(oldGeom, FdoPolygonVertexOrderRule_CCW);
    CheckFGFT(newGeom, correctFgfTextWithCouterClockwiseOrder);

    // Exterior ring and interior rings are all clock wise.
    wrongFgfText = L"CURVEPOLYGON ((0 0 (LINESTRINGSEGMENT (0 200), CIRCULARARCSEGMENT (100 220, 200 200), LINESTRINGSEGMENT (200 0), CIRCULARARCSEGMENT (100 -20, 0 0))), (50 50 (CIRCULARARCSEGMENT (75 75, 100 50), LINESTRINGSEGMENT (50 50))), (120 60 (CIRCULARARCSEGMENT (130 70, 140 60), LINESTRINGSEGMENT (120 60))))";
    oldGeom = gf->CreateGeometry(wrongFgfText);

    newGeom = FdoSpatialUtility::FixPolygonVertexOrder(oldGeom, FdoPolygonVertexOrderRule_CCW);
    CheckFGFT(newGeom, correctFgfTextWithCouterClockwiseOrder);

    newGeom = FdoSpatialUtility::FixPolygonVertexOrder(oldGeom, FdoPolygonVertexOrderRule_CW);
    CheckFGFT(newGeom, correctFgfTextWithClockwiseOrder);

    // Exterior ring and interior rings are couter clock wise.
    wrongFgfText = L"CURVEPOLYGON ((0 0 (CIRCULARARCSEGMENT (100 -20, 200 0), LINESTRINGSEGMENT (200 200), CIRCULARARCSEGMENT (100 220, 0 200), LINESTRINGSEGMENT (0 0))), (50 50 (LINESTRINGSEGMENT (100 50), CIRCULARARCSEGMENT (75 75, 50 50))), (120 60 (LINESTRINGSEGMENT (140 60), CIRCULARARCSEGMENT (130 70, 120 60))))";
    oldGeom = gf->CreateGeometry(wrongFgfText);

    newGeom = FdoSpatialUtility::FixPolygonVertexOrder(oldGeom, FdoPolygonVertexOrderRule_CCW);
    CheckFGFT(newGeom, correctFgfTextWithCouterClockwiseOrder);

    newGeom = FdoSpatialUtility::FixPolygonVertexOrder(oldGeom, FdoPolygonVertexOrderRule_CW);
    CheckFGFT(newGeom, correctFgfTextWithClockwiseOrder);

    /*************************** Test MultiCurvePolygon *************************/
    correctFgfTextWithCouterClockwiseOrder = L"CURVEPOLYGON ((0 0 (CIRCULARARCSEGMENT (100 -20, 200 0), LINESTRINGSEGMENT (200 200), CIRCULARARCSEGMENT (100 220, 0 200), LINESTRINGSEGMENT (0 0))), (50 50 (CIRCULARARCSEGMENT (75 75, 100 50), LINESTRINGSEGMENT (50 50))), (120 60 (CIRCULARARCSEGMENT (130 70, 140 60), LINESTRINGSEGMENT (120 60))))";
    correctFgfTextWithClockwiseOrder = L"CURVEPOLYGON ((0 0 (LINESTRINGSEGMENT (0 200), CIRCULARARCSEGMENT (100 220, 200 200), LINESTRINGSEGMENT (200 0), CIRCULARARCSEGMENT (100 -20, 0 0))), (50 50 (LINESTRINGSEGMENT (100 50), CIRCULARARCSEGMENT (75 75, 50 50))), (120 60 (LINESTRINGSEGMENT (140 60), CIRCULARARCSEGMENT (130 70, 120 60))))";

    oldGeom = gf->CreateGeometry(correctFgfTextWithCouterClockwiseOrder);
    CPPUNIT_ASSERT_MESSAGE("Faild to fix polygon!", FdoSpatialUtility::FixPolygonVertexOrder(oldGeom, FdoPolygonVertexOrderRule_None) == NULL);
    CPPUNIT_ASSERT_MESSAGE("Faild to fix polygon!", FdoSpatialUtility::FixPolygonVertexOrder(oldGeom, FdoPolygonVertexOrderRule_CCW) == NULL);

    newGeom = FdoSpatialUtility::FixPolygonVertexOrder(oldGeom, FdoPolygonVertexOrderRule_CW);
    CheckFGFT(newGeom, correctFgfTextWithClockwiseOrder);

    oldGeom = gf->CreateGeometry(correctFgfTextWithClockwiseOrder);
    CPPUNIT_ASSERT_MESSAGE("Faild to fix polygon!", FdoSpatialUtility::FixPolygonVertexOrder(oldGeom, FdoPolygonVertexOrderRule_CW) == NULL);
    CPPUNIT_ASSERT_MESSAGE("Faild to fix polygon!", FdoSpatialUtility::FixPolygonVertexOrder(oldGeom, FdoPolygonVertexOrderRule_None) == NULL);

    newGeom = FdoSpatialUtility::FixPolygonVertexOrder(oldGeom, FdoPolygonVertexOrderRule_CCW);
    CheckFGFT(newGeom, correctFgfTextWithCouterClockwiseOrder);

    // Exterior ring and interior rings are all clock wise.
    wrongFgfText = L"CURVEPOLYGON ((0 0 (LINESTRINGSEGMENT (0 200), CIRCULARARCSEGMENT (100 220, 200 200), LINESTRINGSEGMENT (200 0), CIRCULARARCSEGMENT (100 -20, 0 0))), (50 50 (CIRCULARARCSEGMENT (75 75, 100 50), LINESTRINGSEGMENT (50 50))), (120 60 (CIRCULARARCSEGMENT (130 70, 140 60), LINESTRINGSEGMENT (120 60))))";
    oldGeom = gf->CreateGeometry(wrongFgfText);

    newGeom = FdoSpatialUtility::FixPolygonVertexOrder(oldGeom, FdoPolygonVertexOrderRule_CCW);
    CheckFGFT(newGeom, correctFgfTextWithCouterClockwiseOrder);

    newGeom = FdoSpatialUtility::FixPolygonVertexOrder(oldGeom, FdoPolygonVertexOrderRule_CW);
    CheckFGFT(newGeom, correctFgfTextWithClockwiseOrder);

    // Exterior ring and interior rings are couter clock wise.
    wrongFgfText = L"CURVEPOLYGON ((0 0 (CIRCULARARCSEGMENT (100 -20, 200 0), LINESTRINGSEGMENT (200 200), CIRCULARARCSEGMENT (100 220, 0 200), LINESTRINGSEGMENT (0 0))), (50 50 (LINESTRINGSEGMENT (100 50), CIRCULARARCSEGMENT (75 75, 50 50))), (120 60 (LINESTRINGSEGMENT (140 60), CIRCULARARCSEGMENT (130 70, 120 60))))";
    oldGeom = gf->CreateGeometry(wrongFgfText);

    newGeom = FdoSpatialUtility::FixPolygonVertexOrder(oldGeom, FdoPolygonVertexOrderRule_CCW);
    CheckFGFT(newGeom, correctFgfTextWithCouterClockwiseOrder);

    newGeom = FdoSpatialUtility::FixPolygonVertexOrder(oldGeom, FdoPolygonVertexOrderRule_CW);
    CheckFGFT(newGeom, correctFgfTextWithClockwiseOrder);
}

void SpatialUtilityTest::testReversePolygonVertexOrder()
{
    FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
    FdoPtr<FdoIGeometry> oldGeom;
    FdoPtr<FdoIGeometry> newGeom;

    /*************************** Test Polygon *************************/
    FdoStringP correctFgfTextWithCouterClockwiseOrder = L"POLYGON ((0 0, 5 0, 5 5, 0 5, 0 0), (1 1, 2 2, 2 1, 1 1), (3 3, 4 4, 4 3, 3 3))";
    FdoStringP correctFgfTextWithClockwiseOrder = L"POLYGON ((0 0, 0 5, 5 5, 5 0, 0 0), (1 1, 2 1, 2 2, 1 1), (3 3, 4 3, 4 4, 3 3))";

    oldGeom = gf->CreateGeometry(correctFgfTextWithCouterClockwiseOrder);
    newGeom = FdoSpatialUtility::ReversePolygonVertexOrder(oldGeom);
    CheckFGFT(newGeom, correctFgfTextWithClockwiseOrder);

    oldGeom = gf->CreateGeometry(correctFgfTextWithClockwiseOrder);
    newGeom = FdoSpatialUtility::ReversePolygonVertexOrder(oldGeom);
    CheckFGFT(newGeom, correctFgfTextWithCouterClockwiseOrder);

    /*************************** Test MultiPolygon *************************/
    correctFgfTextWithCouterClockwiseOrder = L"MULTIPOLYGON (((0 0, 5 0, 5 5, 0 5, 0 0), (1 1, 2 2, 2 1, 1 1), (3 3, 4 4, 4 3, 3 3)), ((10 0, 20 0, 20 10, 10 10, 10 0)))";
    correctFgfTextWithClockwiseOrder = L"MULTIPOLYGON (((0 0, 0 5, 5 5, 5 0, 0 0), (1 1, 2 1, 2 2, 1 1), (3 3, 4 3, 4 4, 3 3)), ((10 0, 10 10, 20 10, 20 0, 10 0)))";

    oldGeom = gf->CreateGeometry(correctFgfTextWithCouterClockwiseOrder);
    newGeom = FdoSpatialUtility::ReversePolygonVertexOrder(oldGeom);
    CheckFGFT(newGeom, correctFgfTextWithClockwiseOrder);

    oldGeom = gf->CreateGeometry(correctFgfTextWithClockwiseOrder);
    newGeom = FdoSpatialUtility::ReversePolygonVertexOrder(oldGeom);
    CheckFGFT(newGeom, correctFgfTextWithCouterClockwiseOrder);

    /*************************** Test CurvePolygon *************************/
    correctFgfTextWithCouterClockwiseOrder = L"CURVEPOLYGON ((0 0 (CIRCULARARCSEGMENT (100 -20, 200 0), LINESTRINGSEGMENT (200 200), CIRCULARARCSEGMENT (100 220, 0 200), LINESTRINGSEGMENT (0 0))), (50 50 (CIRCULARARCSEGMENT (75 75, 100 50), LINESTRINGSEGMENT (50 50))), (120 60 (CIRCULARARCSEGMENT (130 70, 140 60), LINESTRINGSEGMENT (120 60))))";
    correctFgfTextWithClockwiseOrder = L"CURVEPOLYGON ((0 0 (LINESTRINGSEGMENT (0 200), CIRCULARARCSEGMENT (100 220, 200 200), LINESTRINGSEGMENT (200 0), CIRCULARARCSEGMENT (100 -20, 0 0))), (50 50 (LINESTRINGSEGMENT (100 50), CIRCULARARCSEGMENT (75 75, 50 50))), (120 60 (LINESTRINGSEGMENT (140 60), CIRCULARARCSEGMENT (130 70, 120 60))))";

    oldGeom = gf->CreateGeometry(correctFgfTextWithCouterClockwiseOrder);
    newGeom = FdoSpatialUtility::ReversePolygonVertexOrder(oldGeom);
    CheckFGFT(newGeom, correctFgfTextWithClockwiseOrder);

    oldGeom = gf->CreateGeometry(correctFgfTextWithClockwiseOrder);
    newGeom = FdoSpatialUtility::ReversePolygonVertexOrder(oldGeom);
    CheckFGFT(newGeom, correctFgfTextWithCouterClockwiseOrder);

    /*************************** Test MultiCurvePolygon *************************/
    correctFgfTextWithCouterClockwiseOrder = L"CURVEPOLYGON ((0 0 (CIRCULARARCSEGMENT (100 -20, 200 0), LINESTRINGSEGMENT (200 200), CIRCULARARCSEGMENT (100 220, 0 200), LINESTRINGSEGMENT (0 0))), (50 50 (CIRCULARARCSEGMENT (75 75, 100 50), LINESTRINGSEGMENT (50 50))), (120 60 (CIRCULARARCSEGMENT (130 70, 140 60), LINESTRINGSEGMENT (120 60))))";
    correctFgfTextWithClockwiseOrder = L"CURVEPOLYGON ((0 0 (LINESTRINGSEGMENT (0 200), CIRCULARARCSEGMENT (100 220, 200 200), LINESTRINGSEGMENT (200 0), CIRCULARARCSEGMENT (100 -20, 0 0))), (50 50 (LINESTRINGSEGMENT (100 50), CIRCULARARCSEGMENT (75 75, 50 50))), (120 60 (LINESTRINGSEGMENT (140 60), CIRCULARARCSEGMENT (130 70, 120 60))))";

    oldGeom = gf->CreateGeometry(correctFgfTextWithCouterClockwiseOrder);
    newGeom = FdoSpatialUtility::ReversePolygonVertexOrder(oldGeom);
    CheckFGFT(newGeom, correctFgfTextWithClockwiseOrder);

    oldGeom = gf->CreateGeometry(correctFgfTextWithClockwiseOrder);
    newGeom = FdoSpatialUtility::ReversePolygonVertexOrder(oldGeom);
    CheckFGFT(newGeom, correctFgfTextWithCouterClockwiseOrder);
}

void SpatialUtilityTest::testCheckPolygonVertexOrder()
{
    FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
    FdoPtr<FdoIGeometry> geometry;
    FdoPolygonVertexOrderRule vertexOrderRule;
    FdoStringP wrongFgfText;

    /*************************** Test Polygon *************************/
    FdoStringP correctFgfTextWithCouterClockwiseOrder = L"POLYGON ((0 0, 5 0, 5 5, 0 5, 0 0), (1 1, 2 2, 2 1, 1 1), (3 3, 4 4, 4 3, 3 3))";
    FdoStringP correctFgfTextWithClockwiseOrder = L"POLYGON ((0 0, 0 5, 5 5, 5 0, 0 0), (1 1, 2 1, 2 2, 1 1), (3 3, 4 3, 4 4, 3 3))";

    geometry = gf->CreateGeometry(correctFgfTextWithCouterClockwiseOrder);
    vertexOrderRule = FdoSpatialUtility::CheckPolygonVertexOrder(geometry);
    CPPUNIT_ASSERT_MESSAGE("CCW vertex order is expected!", vertexOrderRule == FdoPolygonVertexOrderRule_CCW);

    geometry = gf->CreateGeometry(correctFgfTextWithClockwiseOrder);
    vertexOrderRule = FdoSpatialUtility::CheckPolygonVertexOrder(geometry);
    CPPUNIT_ASSERT_MESSAGE("CW vertex order is expected!", vertexOrderRule == FdoPolygonVertexOrderRule_CW);

    // Exterior ring and interior rings are clock wise.
    wrongFgfText = L"POLYGON ((0 0, 0 5, 5 5, 5 0, 0 0), (1 1, 2 2, 2 1, 1 1), (3 3, 4 4, 4 3, 3 3))";
    geometry = gf->CreateGeometry(wrongFgfText);
    vertexOrderRule = FdoSpatialUtility::CheckPolygonVertexOrder(geometry);
    CPPUNIT_ASSERT_MESSAGE("None vertex order is expected!", vertexOrderRule == FdoPolygonVertexOrderRule_None);

    // Exterior ring and interior rings are all couter clock wise.
    wrongFgfText = L"POLYGON ((0 0, 5 0, 5 5, 0 5, 0 0), (1 1, 2 1, 2 2, 1 1), (3 3, 4 3, 4 4, 3 3))";
    geometry = gf->CreateGeometry(wrongFgfText);
    vertexOrderRule = FdoSpatialUtility::CheckPolygonVertexOrder(geometry);
    CPPUNIT_ASSERT_MESSAGE("None vertex order is expected!", vertexOrderRule == FdoPolygonVertexOrderRule_None);

    /*************************** Test MultiPolygon *************************/
    correctFgfTextWithCouterClockwiseOrder = L"MULTIPOLYGON (((0 0, 5 0, 5 5, 0 5, 0 0), (1 1, 2 2, 2 1, 1 1), (3 3, 4 4, 4 3, 3 3)), ((10 0, 20 0, 20 10, 10 10, 10 0)))";
    correctFgfTextWithClockwiseOrder = L"MULTIPOLYGON (((0 0, 0 5, 5 5, 5 0, 0 0), (1 1, 2 1, 2 2, 1 1), (3 3, 4 3, 4 4, 3 3)), ((10 0, 10 10, 20 10, 20 0, 10 0)))";

    geometry = gf->CreateGeometry(correctFgfTextWithCouterClockwiseOrder);
    vertexOrderRule = FdoSpatialUtility::CheckPolygonVertexOrder(geometry);
    CPPUNIT_ASSERT_MESSAGE("CCW vertex order is expected!", vertexOrderRule == FdoPolygonVertexOrderRule_CCW);

    geometry = gf->CreateGeometry(correctFgfTextWithClockwiseOrder);
    vertexOrderRule = FdoSpatialUtility::CheckPolygonVertexOrder(geometry);
    CPPUNIT_ASSERT_MESSAGE("CW vertex order is expected!", vertexOrderRule == FdoPolygonVertexOrderRule_CW);

    // Exterior ring and interior rings are clock wise.
    wrongFgfText = L"MULTIPOLYGON (((0 0, 0 5, 5 5, 5 0, 0 0), (1 1, 2 2, 2 1, 1 1), (3 3, 4 4, 4 3, 3 3)), ((10 0, 10 10, 20 10, 20 0, 10 0)))";
    geometry = gf->CreateGeometry(wrongFgfText);
    vertexOrderRule = FdoSpatialUtility::CheckPolygonVertexOrder(geometry);
    CPPUNIT_ASSERT_MESSAGE("None vertex order is expected!", vertexOrderRule == FdoPolygonVertexOrderRule_None);

    // Exterior ring and interior rings are all couter clock wise.
    wrongFgfText = L"MULTIPOLYGON (((0 0, 5 0, 5 5, 0 5, 0 0), (1 1, 2 1, 2 2, 1 1), (3 3, 4 3, 4 4, 3 3)), ((10 0, 20 0, 20 10, 10 10, 10 0)))";
    geometry = gf->CreateGeometry(wrongFgfText);
    vertexOrderRule = FdoSpatialUtility::CheckPolygonVertexOrder(geometry);
    CPPUNIT_ASSERT_MESSAGE("None vertex order is expected!", vertexOrderRule == FdoPolygonVertexOrderRule_None);

    /*************************** Test CurvePolygon *************************/
    correctFgfTextWithCouterClockwiseOrder = L"CURVEPOLYGON ((0 0 (CIRCULARARCSEGMENT (100 -20, 200 0), LINESTRINGSEGMENT (200 200), CIRCULARARCSEGMENT (100 220, 0 200), LINESTRINGSEGMENT (0 0))), (50 50 (CIRCULARARCSEGMENT (75 75, 100 50), LINESTRINGSEGMENT (50 50))), (120 60 (CIRCULARARCSEGMENT (130 70, 140 60), LINESTRINGSEGMENT (120 60))))";
    correctFgfTextWithClockwiseOrder = L"CURVEPOLYGON ((0 0 (LINESTRINGSEGMENT (0 200), CIRCULARARCSEGMENT (100 220, 200 200), LINESTRINGSEGMENT (200 0), CIRCULARARCSEGMENT (100 -20, 0 0))), (50 50 (LINESTRINGSEGMENT (100 50), CIRCULARARCSEGMENT (75 75, 50 50))), (120 60 (LINESTRINGSEGMENT (140 60), CIRCULARARCSEGMENT (130 70, 120 60))))";

    geometry = gf->CreateGeometry(correctFgfTextWithCouterClockwiseOrder);
    vertexOrderRule = FdoSpatialUtility::CheckPolygonVertexOrder(geometry);
    CPPUNIT_ASSERT_MESSAGE("CCW vertex order is expected!", vertexOrderRule == FdoPolygonVertexOrderRule_CCW);

    geometry = gf->CreateGeometry(correctFgfTextWithClockwiseOrder);
    vertexOrderRule = FdoSpatialUtility::CheckPolygonVertexOrder(geometry);
    CPPUNIT_ASSERT_MESSAGE("CW vertex order is expected!", vertexOrderRule == FdoPolygonVertexOrderRule_CW);

    // Exterior ring and interior rings are all clock wise.
    wrongFgfText = L"CURVEPOLYGON ((0 0 (LINESTRINGSEGMENT (0 200), CIRCULARARCSEGMENT (100 220, 200 200), LINESTRINGSEGMENT (200 0), CIRCULARARCSEGMENT (100 -20, 0 0))), (50 50 (CIRCULARARCSEGMENT (75 75, 100 50), LINESTRINGSEGMENT (50 50))), (120 60 (CIRCULARARCSEGMENT (130 70, 140 60), LINESTRINGSEGMENT (120 60))))";
    geometry = gf->CreateGeometry(wrongFgfText);
    vertexOrderRule = FdoSpatialUtility::CheckPolygonVertexOrder(geometry);
    CPPUNIT_ASSERT_MESSAGE("None vertex order is expected!", vertexOrderRule == FdoPolygonVertexOrderRule_None);

    // Exterior ring and interior rings are couter clock wise.
    wrongFgfText = L"CURVEPOLYGON ((0 0 (CIRCULARARCSEGMENT (100 -20, 200 0), LINESTRINGSEGMENT (200 200), CIRCULARARCSEGMENT (100 220, 0 200), LINESTRINGSEGMENT (0 0))), (50 50 (LINESTRINGSEGMENT (100 50), CIRCULARARCSEGMENT (75 75, 50 50))), (120 60 (LINESTRINGSEGMENT (140 60), CIRCULARARCSEGMENT (130 70, 120 60))))";
    geometry = gf->CreateGeometry(wrongFgfText);
    vertexOrderRule = FdoSpatialUtility::CheckPolygonVertexOrder(geometry);
    CPPUNIT_ASSERT_MESSAGE("None vertex order is expected!", vertexOrderRule == FdoPolygonVertexOrderRule_None);

    /*************************** Test MultiCurvePolygon *************************/
    correctFgfTextWithCouterClockwiseOrder = L"CURVEPOLYGON ((0 0 (CIRCULARARCSEGMENT (100 -20, 200 0), LINESTRINGSEGMENT (200 200), CIRCULARARCSEGMENT (100 220, 0 200), LINESTRINGSEGMENT (0 0))), (50 50 (CIRCULARARCSEGMENT (75 75, 100 50), LINESTRINGSEGMENT (50 50))), (120 60 (CIRCULARARCSEGMENT (130 70, 140 60), LINESTRINGSEGMENT (120 60))))";
    correctFgfTextWithClockwiseOrder = L"CURVEPOLYGON ((0 0 (LINESTRINGSEGMENT (0 200), CIRCULARARCSEGMENT (100 220, 200 200), LINESTRINGSEGMENT (200 0), CIRCULARARCSEGMENT (100 -20, 0 0))), (50 50 (LINESTRINGSEGMENT (100 50), CIRCULARARCSEGMENT (75 75, 50 50))), (120 60 (LINESTRINGSEGMENT (140 60), CIRCULARARCSEGMENT (130 70, 120 60))))";

    geometry = gf->CreateGeometry(correctFgfTextWithCouterClockwiseOrder);
    vertexOrderRule = FdoSpatialUtility::CheckPolygonVertexOrder(geometry);
    CPPUNIT_ASSERT_MESSAGE("CCW vertex order is expected!", vertexOrderRule == FdoPolygonVertexOrderRule_CCW);

    geometry = gf->CreateGeometry(correctFgfTextWithClockwiseOrder);
    vertexOrderRule = FdoSpatialUtility::CheckPolygonVertexOrder(geometry);
    CPPUNIT_ASSERT_MESSAGE("CW vertex order is expected!", vertexOrderRule == FdoPolygonVertexOrderRule_CW);

    // Exterior ring and interior rings are all clock wise.
    wrongFgfText = L"CURVEPOLYGON ((0 0 (LINESTRINGSEGMENT (0 200), CIRCULARARCSEGMENT (100 220, 200 200), LINESTRINGSEGMENT (200 0), CIRCULARARCSEGMENT (100 -20, 0 0))), (50 50 (CIRCULARARCSEGMENT (75 75, 100 50), LINESTRINGSEGMENT (50 50))), (120 60 (CIRCULARARCSEGMENT (130 70, 140 60), LINESTRINGSEGMENT (120 60))))";
    geometry = gf->CreateGeometry(wrongFgfText);
    vertexOrderRule = FdoSpatialUtility::CheckPolygonVertexOrder(geometry);
    CPPUNIT_ASSERT_MESSAGE("None vertex order is expected!", vertexOrderRule == FdoPolygonVertexOrderRule_None);

    // Exterior ring and interior rings are couter clock wise.
    wrongFgfText = L"CURVEPOLYGON ((0 0 (CIRCULARARCSEGMENT (100 -20, 200 0), LINESTRINGSEGMENT (200 200), CIRCULARARCSEGMENT (100 220, 0 200), LINESTRINGSEGMENT (0 0))), (50 50 (LINESTRINGSEGMENT (100 50), CIRCULARARCSEGMENT (75 75, 50 50))), (120 60 (LINESTRINGSEGMENT (140 60), CIRCULARARCSEGMENT (130 70, 120 60))))";
    geometry = gf->CreateGeometry(wrongFgfText);
    vertexOrderRule = FdoSpatialUtility::CheckPolygonVertexOrder(geometry);
    CPPUNIT_ASSERT_MESSAGE("None vertex order is expected!", vertexOrderRule == FdoPolygonVertexOrderRule_None);
}

void SpatialUtilityTest::testGetPolygonVertexOrderAction()
{
    FdoPolygonVertexOrderAction action;
    action = FdoSpatialUtility::GetPolygonVertexOrderAction(
            FdoPolygonVertexOrderRule_CW,
            false,
            FdoPolygonVertexOrderRule_CW,
            false);
    CPPUNIT_ASSERT_MESSAGE("Action None is expected!",  action == FdoPolygonVertexOrderAction_None);

    action = FdoSpatialUtility::GetPolygonVertexOrderAction(
            FdoPolygonVertexOrderRule_CCW,
            false,
            FdoPolygonVertexOrderRule_CW,
            false);
    CPPUNIT_ASSERT_MESSAGE("Action Reverse is expected!",  action == FdoPolygonVertexOrderAction_Reverse);

    action = FdoSpatialUtility::GetPolygonVertexOrderAction(
            FdoPolygonVertexOrderRule_CW,
            false,
            FdoPolygonVertexOrderRule_CW,
            true);
    CPPUNIT_ASSERT_MESSAGE("Action CheckAndFix is expected!",  action == FdoPolygonVertexOrderAction_CheckAndReverse);
}
