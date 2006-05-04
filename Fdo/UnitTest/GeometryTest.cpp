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
#include "GeometryTest.h"
#include <Geometry.h>
#include <Common/StringP.h>
#include <../Src/Common/StringUtility.h>

using namespace std;

#ifdef _DEBUG
//#define DEBUG_DETAIL	1
#endif

void CheckWkb(FdoByte * inputData);

CPPUNIT_TEST_SUITE_REGISTRATION (GeometryTest);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION (GeometryTest, "GeometryTest");

GeometryTest::GeometryTest(void)
{
}

GeometryTest::~GeometryTest(void)
{
}

void GeometryTest::setUp()
{
    // nothing to do at setup time... yet
}

//////////////////////////////////////////////////////////////////////
// Dumpers...

void DumpPosition(FdoString* context, FdoIDirectPosition* p)
{
#ifdef DEBUG_DETAIL
	if (p->GetDimensionality() == FdoDimensionality_XY)
		printf(" %S XY=(%g,%g)\n", context, p->GetX(), p->GetY());
	else if (p->GetDimensionality() == (FdoDimensionality_XY|FdoDimensionality_Z))
		printf(" %S XYZ=(%g,%g,%g)\n", context, p->GetX(), p->GetY(), p->GetZ());
	else if (p->GetDimensionality() == (FdoDimensionality_XY|FdoDimensionality_M))
		printf(" %S XYM=(%g,%g,%g)\n", context, p->GetX(), p->GetY(), p->GetM());
	else if (p->GetDimensionality() == (FdoDimensionality_XY|FdoDimensionality_Z|FdoDimensionality_M))
		printf(" %S XYZM=(%g,%g,%g,%g)\n", context, p->GetX(), p->GetY(), p->GetZ(), p->GetM());
	else
		CPPUNIT_ASSERT_MESSAGE("DumpPosition::UNKNOWN_DIMENSION_TYPE!\n", false);
#endif
}

void DumpEnvelope(FdoIEnvelope* env)
{
#ifdef DEBUG_DETAIL
	printf(" Envelope=min(%g,%g,%g) max(%g,%g,%g)\n",
		env->GetMinX(), env->GetMinY(), env->GetMinZ(),
		env->GetMaxX(), env->GetMaxY(), env->GetMaxZ());
#endif
}

// Dump FGFT
void DumpFGFT(FdoIGeometry* p)
{
#ifdef DEBUG_DETAIL
    printf("FGFT = %S\n", p->GetText());
#endif
}

void DumpLineString(FdoILineString* line)
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

void DumpLinearRing(FdoILinearRing* lRing)
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

void DumpLineStringSegment(FdoILineStringSegment* lSeg)
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

void DumpPolygon(FdoIPolygon* polygon)
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

void DumpGeometry(FdoIGeometry* geometry)
{
#ifdef DEBUG_DETAIL
	printf("%S: dimtype(%d) derivedtype(%d)\n",
		L"GEOMETRY", geometry->GetDimensionality(), geometry->GetDerivedType());

    DumpFGFT(geometry);
#endif
}

void DumpPoint(FdoIPoint* pnt)
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

void DumpMultiPoint(FdoIMultiPoint* multiPnt)
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

void DumpMultiLineString(FdoIMultiLineString* multiLine)
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

void DumpMultiPolygon(FdoIMultiPolygon* multiPoly)
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

void DumpCircularArcSegment(FdoICircularArcSegment* arcSeg)
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


void DumpCurveString(FdoICurveString* curveString)
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

void DumpRing(FdoIRing* ring)
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

void DumpCurvePolygon(FdoICurvePolygon* curvePoly)
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

void DumpMultiCurvePolygon(FdoIMultiCurvePolygon* multiCurvePoly)
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

void DumpMultiCurveString(FdoIMultiCurveString* multiCurveString)
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

void DumpMultiGeometry(FdoIMultiGeometry* multiGeometry)
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
// Checkers...
//////////////////////////////////////////////////////////////////////

void CheckPositionXY(FdoIDirectPosition* pos, double x, double y)
{
	CPPUNIT_ASSERT_MESSAGE("X value mismatch!", pos->GetX() == x);
	CPPUNIT_ASSERT_MESSAGE("Y value mismatch!", pos->GetY() == y);
}

void CheckPositionXYM(FdoIDirectPosition* pos, double x, double y, double m)
{
	CheckPositionXY(pos, x, y);
	CPPUNIT_ASSERT_MESSAGE("M value mismatch!", pos->GetM() == m);
}

void CheckPositionXYZ(FdoIDirectPosition* pos, double x, double y, double z)
{
	CheckPositionXY(pos, x, y);
	CPPUNIT_ASSERT_MESSAGE("Z value mismatch!", pos->GetZ() == z);
}

void CheckPositionXYZM(FdoIDirectPosition* pos, double x, double y, double z, double m)
{
	CheckPositionXYZ(pos, x, y, z);
	CPPUNIT_ASSERT_MESSAGE("M value mismatch!", pos->GetM() == m);
}

void CheckEnvelope(FdoIEnvelope* envl, double* ordsXY)
{
	CPPUNIT_ASSERT_MESSAGE("MinX mismatch!", envl->GetMinX() == ordsXY[0]);
	CPPUNIT_ASSERT_MESSAGE("MinY mismatch!", envl->GetMinY() == ordsXY[1]);
	CPPUNIT_ASSERT_MESSAGE("MaxX mismatch!", envl->GetMaxX() == ordsXY[2]);
	CPPUNIT_ASSERT_MESSAGE("MaxY mismatch!", envl->GetMaxY() == ordsXY[3]);
}

void CheckEnvelope(FdoIEnvelope* envl, FdoIDirectPosition* pos1, FdoIDirectPosition* pos2)
{
	CPPUNIT_ASSERT_MESSAGE("MinX mismatch!", envl->GetMinX() == pos1->GetX());
	CPPUNIT_ASSERT_MESSAGE("MinY mismatch!", envl->GetMinY() == pos1->GetY());
    CPPUNIT_ASSERT_MESSAGE("MinZ mismatch!", (pos1->GetDimensionality() & FdoDimensionality_Z) == 0 || envl->GetMinZ() == pos1->GetZ());
	CPPUNIT_ASSERT_MESSAGE("MaxX mismatch!", envl->GetMaxX() == pos2->GetX());
	CPPUNIT_ASSERT_MESSAGE("MaxY mismatch!", envl->GetMaxY() == pos2->GetY());
	CPPUNIT_ASSERT_MESSAGE("MaxZ mismatch!", (pos2->GetDimensionality() & FdoDimensionality_Z) == 0 || envl->GetMaxZ() == pos2->GetZ());
}

void CheckEnvelopeXY(FdoIEnvelope* envl, double minx, double miny, double maxx, double maxy)
{
	CPPUNIT_ASSERT_MESSAGE("MinX mismatch!", envl->GetMinX() == minx);
	CPPUNIT_ASSERT_MESSAGE("MinY mismatch!", envl->GetMinY() == miny);
	CPPUNIT_ASSERT_MESSAGE("MaxX mismatch!", envl->GetMaxX() == maxx);
	CPPUNIT_ASSERT_MESSAGE("MaxY mismatch!", envl->GetMaxY() == maxy);
}

void CheckEnvelopeXYZ(FdoIEnvelope* envl, double minx, double miny, double minz, double maxx, double maxy, double maxz)
{
	CheckEnvelopeXY(envl, minx, miny, maxx, maxy);
	CPPUNIT_ASSERT_MESSAGE("MinZ mismatch!", envl->GetMinZ() == minz);
	CPPUNIT_ASSERT_MESSAGE("MaxZ mismatch!", envl->GetMaxZ() == maxz);
}

void CheckEqualEnvelopes(FdoIEnvelope* envl1, FdoIEnvelope* envl2)
{
	CPPUNIT_ASSERT_MESSAGE("Envelope MinX mismatch!", envl1->GetMinX() == envl2->GetMinX());
	CPPUNIT_ASSERT_MESSAGE("Envelope MinY mismatch!", envl1->GetMinY() == envl2->GetMinY());
	CPPUNIT_ASSERT_MESSAGE("Envelope MinZ mismatch!", envl1->GetMinZ() == envl2->GetMinZ());
	CPPUNIT_ASSERT_MESSAGE("Envelope MaxX mismatch!", envl1->GetMaxX() == envl2->GetMaxX());
	CPPUNIT_ASSERT_MESSAGE("Envelope MaxY mismatch!", envl1->GetMaxY() == envl2->GetMaxY());
	CPPUNIT_ASSERT_MESSAGE("Envelope MaxZ mismatch!", envl1->GetMaxZ() == envl2->GetMaxZ());
}

//TODO: use parens in FGFT formatter
void CheckFGFT(FdoIGeometry* geom, FdoString* fgft, FdoInt32 parens=2);
void CheckFGFT(FdoIGeometry* geom, FdoString* fgft, FdoInt32 parens)
{
	DumpFGFT(geom);

    char    szError[4000+4000+30];
    sprintf(szError, "FGFT mismatch!\n<%S> should be <%S>\n", geom->GetText(), fgft);
	CPPUNIT_ASSERT_MESSAGE(szError, wcscmp(geom->GetText(), fgft) == 0);
}

//////////////////////////////////////////////////////////////////////
// Helpers to create repetitive geometries

FdoICurveString* CreateCurveString(double offset)
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

	return FDO_SAFE_ADDREF(curveString.p);
}

FdoIRing* CreateRing(double offset)
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

    FdoPtr<FdoDirectPositionCollection> points = FdoDirectPositionCollection::Create();
	FdoPtr<FdoIDirectPosition> fromPt = gf->CreatePositionXY(offset+1.0, offset+2.0);
	FdoPtr<FdoIDirectPosition> toPt = gf->CreatePositionXY(offset+0.0, offset+0.0);
    points->Add(fromPt);
    points->Add(toPt);

	FdoPtr<FdoILineStringSegment> lineSeg = gf->CreateLineStringSegment(points);

    FdoPtr<FdoCurveSegmentCollection> curveSegs = FdoCurveSegmentCollection::Create();
	curveSegs->Add(arcSeg);
	curveSegs->Add(lineSeg);

	FdoPtr<FdoIRing> ring = gf->CreateRing(curveSegs);

	return FDO_SAFE_ADDREF(ring.p);
}

FdoICurvePolygon* CreateCurvePolygon(double offset)
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

	return FDO_SAFE_ADDREF(curvePoly.p);
}

FdoIMultiCurvePolygon* CreateMultiCurvePolygon(FdoInt32 numCurvePolys, double offset)
{
    FdoPtr<FdoCurvePolygonCollection> curvePolys = FdoCurvePolygonCollection::Create();

	for (FdoInt32 i=0; i<numCurvePolys; i++)
	{
		FdoPtr<FdoICurvePolygon> curvePoly = CreateCurvePolygon(i+offset);
		curvePolys->Add(curvePoly);
	}

	FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();

	FdoPtr<FdoIMultiCurvePolygon> multiCurvePoly = gf->CreateMultiCurvePolygon(curvePolys);

	return FDO_SAFE_ADDREF(multiCurvePoly.p);
}

FdoIMultiGeometry* CreateMultiGeometry()
{
    FdoPtr<FdoGeometryCollection> geometries = FdoGeometryCollection::Create();

	FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();

	FdoPtr<FdoIGeometry> geometry;

    // CurvePolygon
    geometry = CreateCurvePolygon(0);
	geometries->Add(geometry);

    // CurveString
    // Not doing CurveString because of some unfixed defect.
    // It may be the same one that sometimes affects MultiPolygon.
    geometry = CreateCurveString(100);
	geometries->Add(geometry);

    // LineString
	double* ordsXY = new double[6];
	ordsXY[0] = 0.0; ordsXY[1] = 1.0;
	ordsXY[2] = 2.0; ordsXY[3] = 3.0;
	ordsXY[4] = 4.0; ordsXY[5] = 5.0;
	geometry = gf->CreateLineString(FdoDimensionality_XY, 6, ordsXY);
	geometries->Add(geometry);
	delete [] ordsXY;

    // Point
	double*		ordsXYZ = new double[3];
	ordsXYZ[0] = 5.0; ordsXYZ[1] = 3.0; ordsXYZ[2] = 2.0;
    FdoPtr<FdoIPoint> pt = gf->CreatePoint(FdoDimensionality_XY|FdoDimensionality_Z, ordsXYZ);
    // This is a trivial test of FdoPtr's operator= for mismatched types.
    geometry = pt;
	geometries->Add(geometry);
	delete [] ordsXYZ;

    // Polygon
    // Not doing Polygon because of some unfixed defect.
    // It may be the same one that sometimes affects MultiPolygon.
	double* ordsXYExt = new double[10];
	ordsXYExt[0] = 0.0; ordsXYExt[1] = 0.0;
	ordsXYExt[2] = 5.0; ordsXYExt[3] = 0.0;
	ordsXYExt[4] = 5.0; ordsXYExt[5] = 5.0;
	ordsXYExt[6] = 0.0; ordsXYExt[7] = 5.0;
	ordsXYExt[8] = 0.0; ordsXYExt[9] = 0.0;
	double* ordsXYInt1 = new double[8];
	ordsXYInt1[0] = 1.0; ordsXYInt1[1] = 1.0;
	ordsXYInt1[2] = 2.0; ordsXYInt1[3] = 1.0;
	ordsXYInt1[4] = 2.0; ordsXYInt1[5] = 2.0;
	ordsXYInt1[6] = 1.0; ordsXYInt1[7] = 1.0;
	double* ordsXYInt2 = new double[8];
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
	delete [] ordsXYExt;
	delete [] ordsXYInt1;
	delete [] ordsXYInt2;

    // Make MultiGeometry from the many geometries collected above.
	FdoPtr<FdoIMultiGeometry> multiGeometry = gf->CreateMultiGeometry(geometries);

	return FDO_SAFE_ADDREF(multiGeometry.p);
}

//////////////////////////////////////////////////////////////////////
// Test each geometry type...

//////////////////////////////////////////////////////////////////////
// basics geometry types...

void GeometryTest::testDirectPosition()
{
	FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();

	FdoPtr<FdoIDirectPosition> pointXY = gf->CreatePositionXY(5.0, 6.0);
	DumpPosition(L"", pointXY);
	CheckPositionXY(pointXY, 5.0, 6.0);

	FdoPtr<FdoIDirectPosition> pointXYM = gf->CreatePositionXYM(1.0, 3.0, 5.0);
	DumpPosition(L"", pointXYM);
	CheckPositionXYM(pointXYM, 1.0, 3.0, 5.0);

	FdoPtr<FdoIDirectPosition> pointXYZ = gf->CreatePositionXYZ(2.0, 7.0, 10.0);
	DumpPosition(L"", pointXYZ);
	CheckPositionXYZ(pointXYZ, 2.0, 7.0, 10.0);

	FdoPtr<FdoIDirectPosition> pointXYZM = gf->CreatePositionXYZM(1.23, 4.26, 3.67, 3.14);
	DumpPosition(L"", pointXYZM);
	CheckPositionXYZM(pointXYZM, 1.23, 4.26, 3.67, 3.14);
}

void GeometryTest::testEnvelope()
{
	FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();

	double *ordsXY = new double[4];
	ordsXY[0] = -1.0;
	ordsXY[1] = -2.0;
	ordsXY[2] = 3.0;
	ordsXY[3] = 4.0;

	FdoPtr<FdoIEnvelope> envl1 = gf->CreateEnvelope(FdoDimensionality_XY, ordsXY);
	DumpEnvelope(envl1);
	CheckEnvelope(envl1, ordsXY);

	FdoPtr<FdoIDirectPosition> pos1 = gf->CreatePositionXY(5.0, 6.0);
	FdoPtr<FdoIDirectPosition> pos2 = gf->CreatePositionXY(10.0, 13.0);
	FdoPtr<FdoIEnvelope> envl2 = gf->CreateEnvelope(pos1, pos2);
	DumpEnvelope(envl2);
	CheckEnvelope(envl2, pos1, pos2);

	FdoPtr<FdoIEnvelope> envl3 = gf->CreateEnvelopeXY(4.0, 3.0, 12.0, 120.0);
	DumpEnvelope(envl3);
	CheckEnvelopeXY(envl3, 4.0, 3.0, 12.0, 120.0);

	FdoPtr<FdoIEnvelope> envl4 = gf->CreateEnvelopeXYZ(12.0, 45.0, 1.0, 34.0, 39.0, 2.0);
	DumpEnvelope(envl4);
	CheckEnvelopeXYZ(envl4, 12.0, 45.0, 1.0, 34.0, 39.0, 2.0);

	FdoPtr<FdoIEnvelope> envl5 = gf->CreateEnvelope(envl4);
	DumpEnvelope(envl5);
	CheckEqualEnvelopes(envl4, envl5);

	FdoPtr<FdoIEnvelope> envl6 = gf->CreateEnvelope(envl5);
	DumpEnvelope(envl5);
	CheckEqualEnvelopes(envl6, envl5);
	
	delete [] ordsXY;
}

void GeometryTest::testLinearRing()
{
	FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
	double* ordsXY = new double[10];
	ordsXY[0] = 0.0; ordsXY[1] = 0.0;
	ordsXY[2] = 1.0; ordsXY[3] = 0.0;
	ordsXY[4] = 1.0; ordsXY[5] = 1.0;
	ordsXY[6] = 0.0; ordsXY[7] = 1.0;
	ordsXY[8] = 0.0; ordsXY[9] = 0.0;

	FdoPtr<FdoILinearRing> lRing = gf->CreateLinearRing(FdoDimensionality_XY, 10, ordsXY);
	DumpLinearRing(lRing);

	delete [] ordsXY;
}

void GeometryTest::testLineStringSegment()
{
	FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
	double* ordsXY = new double[10];
	ordsXY[0] = 0.0; ordsXY[1] = 0.0;
	ordsXY[2] = 1.0; ordsXY[3] = 0.0;
	ordsXY[4] = 1.0; ordsXY[5] = 1.0;
	ordsXY[6] = 0.0; ordsXY[7] = 1.0;
	ordsXY[8] = 0.0; ordsXY[9] = 0.0;

	FdoPtr<FdoILineStringSegment> lSeg = gf->CreateLineStringSegment(FdoDimensionality_XY, 10, ordsXY);
	DumpLineStringSegment(lSeg);

	delete [] ordsXY;
}

void GeometryTest::testCircularArcSegment()
{
	FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();

	FdoPtr<FdoIDirectPosition> startPos = gf->CreatePositionXYZ(0.0, 0.0, 0.0);
	FdoPtr<FdoIDirectPosition> midPos = gf->CreatePositionXYZ(1.0, 1.0, 1.0);
	FdoPtr<FdoIDirectPosition> endPos = gf->CreatePositionXYZ(2.0, 2.0, 2.0);

	FdoPtr<FdoICircularArcSegment> arcSeg = gf->CreateCircularArcSegment(startPos, midPos, endPos);
	DumpCircularArcSegment(arcSeg);
}

void GeometryTest::testRing()
{
	FdoPtr<FdoIRing> ring = CreateRing(3.14);
	DumpRing(ring);
}

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

void GeometryTest::testPoint()
{
	double*		ordsXYZ = new double[3];
	ordsXYZ[0] = 5.0; ordsXYZ[1] = 3.0; ordsXYZ[2] = 2.0;

	FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
	FdoPtr<FdoIPoint> pnt = gf->CreatePoint(FdoDimensionality_XY|FdoDimensionality_Z, ordsXYZ);
	DumpPoint(pnt);
	CheckFGFT(pnt, L"POINT XYZ (5 3 2)");

	delete [] ordsXYZ;
}

void GeometryTest::testLineString()
{
	FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();

	double* ordsXY = new double[6];
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
	FdoPtr<FdoByteArray> wkb2 = gf->GetWkb(line1);
    CheckWkb(wkb2->GetData());

	delete [] ordsXY;
}

void GeometryTest::testPolygon()
{
	FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
	double* ordsXYExt = new double[10];
	ordsXYExt[0] = 0.0; ordsXYExt[1] = 0.0;
	ordsXYExt[2] = 5.0; ordsXYExt[3] = 0.0;
	ordsXYExt[4] = 5.0; ordsXYExt[5] = 5.0;
	ordsXYExt[6] = 0.0; ordsXYExt[7] = 5.0;
	ordsXYExt[8] = 0.0; ordsXYExt[9] = 0.0;

	double* ordsXYInt1 = new double[8];
	ordsXYInt1[0] = 1.0; ordsXYInt1[1] = 1.0;
	ordsXYInt1[2] = 2.0; ordsXYInt1[3] = 1.0;
	ordsXYInt1[4] = 2.0; ordsXYInt1[5] = 2.0;
	ordsXYInt1[6] = 1.0; ordsXYInt1[7] = 1.0;

	double* ordsXYInt2 = new double[8];
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

	FdoPtr<FdoByteArray> wkb = gf->GetWkb(polygon);
	FdoPtr<FdoIGeometry> g2 = gf->CreateGeometryFromWkb(wkb);
	CheckFGFT(g2, L"POLYGON ((0 0, 5 0, 5 5, 0 5, 0 0), (1 1, 2 1, 2 2, 1 1), (3 3, 4 3, 4 4, 3 3))");
	FdoPtr<FdoByteArray> wkb2 = gf->GetWkb(g2);
    CheckWkb(wkb2->GetData());

	delete [] ordsXYExt;
	delete [] ordsXYInt1;
	delete [] ordsXYInt2;
}

void GeometryTest::testMultiPoint()
{
	double*		ordsXYZ1 = new double[3];
	ordsXYZ1[0] = 1.0; ordsXYZ1[1] = 2.0; ordsXYZ1[2] = 3.0;

	double*		ordsXYZ2 = new double[3];
	ordsXYZ2[0] = 4.0; ordsXYZ2[1] = 5.0; ordsXYZ2[2] = 6.0;

	double*		ordsXYZ3 = new double[3];
	ordsXYZ3[0] = 7.0; ordsXYZ3[1] = 8.0; ordsXYZ3[2] = 9.0;

	FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
	FdoPtr<FdoIPoint> pnt1 = gf->CreatePoint(FdoDimensionality_XY|FdoDimensionality_Z, ordsXYZ1);
	FdoPtr<FdoIPoint> pnt2 = gf->CreatePoint(FdoDimensionality_XY|FdoDimensionality_Z, ordsXYZ2);
	FdoPtr<FdoIPoint> pnt3 = gf->CreatePoint(FdoDimensionality_XY|FdoDimensionality_Z, ordsXYZ3);

    FdoPtr<FdoPointCollection> pnts = FdoPointCollection::Create();
	pnts->Add(pnt1);
    pnts->Add(pnt2);
	pnts->Add(pnt3);

	FdoPtr<FdoIMultiPoint> multiPnt = gf->CreateMultiPoint(pnts);
	DumpMultiPoint(multiPnt);
	CheckFGFT(multiPnt, L"MULTIPOINT XYZ (1 2 3, 4 5 6, 7 8 9)");

	delete [] ordsXYZ1;
	delete [] ordsXYZ2;
	delete [] ordsXYZ3;
}

void GeometryTest::testMultiLineString()
{
	double* ordsXYZ1 = new double[9];
	ordsXYZ1[0] = 0.0; ordsXYZ1[1] = 1.0; ordsXYZ1[2] = 2.0;
	ordsXYZ1[3] = 3.0; ordsXYZ1[4] = 4.0; ordsXYZ1[5] = 5.0;
	ordsXYZ1[6] = 6.0; ordsXYZ1[7] = 7.0; ordsXYZ1[8] = 8.0;

	double* ordsXYZ2 = new double[9];
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

	delete [] ordsXYZ1;
	delete [] ordsXYZ2;
}

void GeometryTest::testMultiPolygon()
{
	FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();

	// 1st polygon
	double* ordsXYExt1 = new double[10];
	ordsXYExt1[0] = 0.0; ordsXYExt1[1] = 0.0;
	ordsXYExt1[2] = 5.0; ordsXYExt1[3] = 0.0;
	ordsXYExt1[4] = 5.0; ordsXYExt1[5] = 5.0;
	ordsXYExt1[6] = 0.0; ordsXYExt1[7] = 5.0;
	ordsXYExt1[8] = 0.0; ordsXYExt1[9] = 0.0;

	double* ordsXYInt11 = new double[8];
	ordsXYInt11[0] = 1.0; ordsXYInt11[1] = 1.0;
	ordsXYInt11[2] = 2.0; ordsXYInt11[3] = 1.0;
	ordsXYInt11[4] = 2.0; ordsXYInt11[5] = 2.0;
	ordsXYInt11[6] = 1.0; ordsXYInt11[7] = 1.0;

	double* ordsXYInt12 = new double[8];
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
	double* ordsXYExt2 = new double[10];
	ordsXYExt2[0] = 0.0; ordsXYExt2[1] = 0.0;
	ordsXYExt2[2] = 5.0; ordsXYExt2[3] = 0.0;
	ordsXYExt2[4] = 5.0; ordsXYExt2[5] = 5.0;
	ordsXYExt2[6] = 0.0; ordsXYExt2[7] = 5.0;
	ordsXYExt2[8] = 0.0; ordsXYExt2[9] = 0.0;

	double* ordsXYInt21 = new double[8];
	ordsXYInt21[0] = 1.0; ordsXYInt21[1] = 1.0;
	ordsXYInt21[2] = 2.0; ordsXYInt21[3] = 1.0;
	ordsXYInt21[4] = 2.0; ordsXYInt21[5] = 2.0;
	ordsXYInt21[6] = 1.0; ordsXYInt21[7] = 1.0;

	double* ordsXYInt22 = new double[8];
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

	FdoPtr<FdoByteArray> wkb = gf->GetWkb(multiPoly);
	FdoPtr<FdoIGeometry> g2 = gf->CreateGeometryFromWkb(wkb);
	CheckFGFT(g2, L"MULTIPOLYGON (((0 0, 5 0, 5 5, 0 5, 0 0), (1 1, 2 1, 2 2, 1 1), (3 3, 4 3, 4 4, 3 3)), ((0 0, 5 0, 5 5, 0 5, 0 0), (1 1, 2 1, 2 2, 1 1), (3 3, 4 3, 4 4, 3 3)))");
	FdoPtr<FdoByteArray> wkb2 = gf->GetWkb(g2);
    CheckWkb(wkb2->GetData());

	delete [] ordsXYExt1;
	delete [] ordsXYInt11;
	delete [] ordsXYInt12;
	delete [] ordsXYExt2;
	delete [] ordsXYInt21;
	delete [] ordsXYInt22;
}

void GeometryTest::testMultiGeometry()
{
	FdoPtr<FdoIMultiGeometry> multiGeometry = CreateMultiGeometry();
	DumpMultiGeometry(multiGeometry);
	CheckFGFT(multiGeometry, L"GEOMETRYCOLLECTION (CURVEPOLYGON ((100 100 (CIRCULARARCSEGMENT (100 101, 101 102), LINESTRINGSEGMENT (100 100))), (200 200 (CIRCULARARCSEGMENT (200 201, 201 202), LINESTRINGSEGMENT (200 200))), (300 300 (CIRCULARARCSEGMENT (300 301, 301 302), LINESTRINGSEGMENT (300 300)))), CURVESTRING (100 100 (CIRCULARARCSEGMENT (100 101, 101 102), LINESTRINGSEGMENT (103 100, 103 102))), LINESTRING (0 1, 2 3, 4 5), POINT XYZ (5 3 2), POLYGON ((0 0, 5 0, 5 5, 0 5, 0 0), (1 1, 2 1, 2 2, 1 1), (3 3, 4 3, 4 4, 3 3)))");
}

void GeometryTest::testCurveString()
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
	CheckFGFT(curveString, L"CURVESTRING (0 0 (CIRCULARARCSEGMENT (0 1, 1 2), LINESTRINGSEGMENT (3 0, 3 2)))");
}

void GeometryTest::testCurvePolygon()
{
	FdoPtr<FdoICurvePolygon> curvePoly = CreateCurvePolygon(100);
	DumpCurvePolygon(curvePoly);
	CheckFGFT(curvePoly, L"CURVEPOLYGON ((200 200 (CIRCULARARCSEGMENT (200 201, 201 202), LINESTRINGSEGMENT (200 200))), (300 300 (CIRCULARARCSEGMENT (300 301, 301 302), LINESTRINGSEGMENT (300 300))), (400 400 (CIRCULARARCSEGMENT (400 401, 401 402), LINESTRINGSEGMENT (400 400))))");
}

void GeometryTest::testMultiCurveString()
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
}

void GeometryTest::testMultiCurvePolygon()
{
	FdoPtr<FdoIMultiCurvePolygon> multiCurvePoly = CreateMultiCurvePolygon(3, 100);
	DumpMultiCurvePolygon(multiCurvePoly);
	CheckFGFT(multiCurvePoly, L"MULTICURVEPOLYGON (((200 200 (CIRCULARARCSEGMENT (200 201, 201 202), LINESTRINGSEGMENT (200 200))), (300 300 (CIRCULARARCSEGMENT (300 301, 301 302), LINESTRINGSEGMENT (300 300))), (400 400 (CIRCULARARCSEGMENT (400 401, 401 402), LINESTRINGSEGMENT (400 400)))), ((201 201 (CIRCULARARCSEGMENT (201 202, 202 203), LINESTRINGSEGMENT (201 201))), (301 301 (CIRCULARARCSEGMENT (301 302, 302 303), LINESTRINGSEGMENT (301 301))), (401 401 (CIRCULARARCSEGMENT (401 402, 402 403), LINESTRINGSEGMENT (401 401)))), ((202 202 (CIRCULARARCSEGMENT (202 203, 203 204), LINESTRINGSEGMENT (202 202))), (302 302 (CIRCULARARCSEGMENT (302 303, 303 304), LINESTRINGSEGMENT (302 302))), (402 402 (CIRCULARARCSEGMENT (402 403, 403 404), LINESTRINGSEGMENT (402 402)))))");
}

void GeometryTest::testGeometryFromEnvelope()
{
	FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();

    FdoPtr<FdoIEnvelope> envelope = gf->CreateEnvelopeXY(1.2345678, 1.2, 2, 2);
    FdoPtr<FdoIGeometry> geometry = gf->CreateGeometry(envelope);

	DumpGeometry(geometry);
	CheckFGFT(geometry, L"POLYGON ((1.2345678 1.2, 2 1.2, 2 2, 1.2345678 2, 1.2345678 1.2))");

    envelope = gf->CreateEnvelopeXYZ(1, 1, 0, 2, 2, 0);
    geometry = gf->CreateGeometry(envelope);

	DumpGeometry(geometry);
	CheckFGFT(geometry, L"POLYGON XYZ ((1 1 0, 2 1 0, 2 2 0, 1 2 0, 1 1 0))");
}


// This class shows how to use the Geometry API to analyse a geometry in detail.
// This example copies a geometry while applying a trivial transformation
// (swapping X and Y ordinates) to each ordinate.  It can also convert between
// dimensionalities.
class GeometryConverter
{
public:

    FdoIGeometry * ConvertOrdinates(
        FdoIGeometry * geometry,
        bool applyTargetDimensionality=false,
        FdoInt32 targetDimensionality=0,
        double padValueZ=0.0,
        double padValueM=0.0);

    // Replace this method with desired transformation.
    // Ideally, it would be inline (for speed), but testing showed a problem with overriding.
    virtual void ConvertPosition(
        double &x,
        double &y)
    {
        // A trivial transformation: swap x and y.
        double temp = x;
        x = y;
        y = temp;
    }

    // Replace this method with desired transformation.
    // Ideally, it would be inline (for speed), but testing showed a problem with overriding.
    virtual void ConvertPosition(
        double &x,
        double &y,
        double &z)
    {
        // A trivial transformation: swap x and y.
        double temp = x;
        x = y;
        y = temp;
    }

    GeometryConverter() {};
    virtual ~GeometryConverter() {};

protected:

    void ConvertOrdinates(
        FdoInt32 inputDim,
        FdoInt32 numPositions,
        const double * inputOrds,
        double padValueZ,
        double padValueM,
        FdoInt32 outputDim,
        double * outputOrds);

    FdoILinearRing * ConvertOrdinates(
        FdoILinearRing * ring,
        FdoInt32 outputDim,
        double padValueZ,
        double padValueM);

    FdoCurveSegmentCollection * ConvertOrdinates(
        FdoCurveSegmentCollection * csc,
        FdoInt32 outputDim,
        double padValueZ,
        double padValueM);

    FdoICurveSegmentAbstract * ConvertOrdinates(
        FdoICurveSegmentAbstract * cs,
        FdoInt32 outputDim,
        double padValueZ,
        double padValueM);

    double * AllocateOrdinates(FdoInt32 numOrdinates)
    {
        double * ordinates = new double[numOrdinates];
        if (NULL == ordinates)
            throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_BADALLOC)));
        return ordinates;
    }

    FdoInt32 DimensionalityToNumOrdinates(FdoInt32 dimensionality)
    {
        // This does depend on knowing the values of FdoDimensionality_*.
        static FdoInt32 ordinateCountTable[] = { 2, 3, 3, 4 };

        return ordinateCountTable[dimensionality];
    }
};

FdoIGeometry * GeometryConverter::ConvertOrdinates(
    FdoIGeometry * geometry,
    bool applyTargetDimensionality,
    FdoInt32 targetDimensionality,
    double padValueZ,
    double padValueM)
{
    FdoIGeometry * newGeometry = NULL;

    if ( geometry == NULL )
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_2_BADPARAMETER)));

	FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();

    const double * ordinates = NULL;
    double * newOrdinates = NULL;
    FdoInt32 numOrdinates;
    FdoInt32 numPositions;
    FdoInt32 dimensionality;
    FdoInt32 outputDim;
    FdoInt32 i;

    FdoGeometryType geomType = geometry->GetDerivedType();

    switch (geomType)
    {
    case FdoGeometryType_LineString:
        {
            FdoILineString * derivedGeom = static_cast<FdoILineString *>(geometry);
            dimensionality = derivedGeom->GetDimensionality();
            numPositions = derivedGeom->GetCount();
            outputDim = applyTargetDimensionality ? targetDimensionality : dimensionality;
            numOrdinates = numPositions * this->DimensionalityToNumOrdinates(outputDim);
            newOrdinates = AllocateOrdinates(numOrdinates);
            ordinates = derivedGeom->GetOrdinates();
            ConvertOrdinates(dimensionality, numPositions, ordinates, padValueZ, padValueM, outputDim, newOrdinates);
            newGeometry = gf->CreateLineString(outputDim, numOrdinates, newOrdinates);
            delete [] newOrdinates;
        }
        break;
    case FdoGeometryType_Point:
        {
            FdoIPoint * derivedGeom = static_cast<FdoIPoint *>(geometry);
            dimensionality = derivedGeom->GetDimensionality();
            numPositions = 1;
            outputDim = applyTargetDimensionality ? targetDimensionality : dimensionality;
            numOrdinates = numPositions * this->DimensionalityToNumOrdinates(outputDim);
            newOrdinates = AllocateOrdinates(numOrdinates);
            ordinates = derivedGeom->GetOrdinates();
            ConvertOrdinates(dimensionality, numPositions, ordinates, padValueZ, padValueM, outputDim, newOrdinates);
            newGeometry = gf->CreatePoint(outputDim, newOrdinates);
            delete [] newOrdinates;
        }
        break;
    case FdoGeometryType_Polygon:
        {
            FdoIPolygon * derivedGeom = static_cast<FdoIPolygon *>(geometry);
            outputDim = applyTargetDimensionality ? targetDimensionality : derivedGeom->GetDimensionality();
            FdoPtr<FdoLinearRingCollection> newInteriorRings = FdoLinearRingCollection::Create();

            FdoPtr<FdoILinearRing> ring = derivedGeom->GetExteriorRing();
            FdoPtr<FdoILinearRing> newExteriorRing = ConvertOrdinates(ring, outputDim, padValueZ, padValueM);

            for (i = 0;  i < derivedGeom->GetInteriorRingCount();  i++)
            {
                ring = derivedGeom->GetInteriorRing(i);
                FdoPtr<FdoILinearRing> newInteriorRing = ConvertOrdinates(ring, outputDim, padValueZ, padValueM);
                newInteriorRings->Add(newInteriorRing);
            }

            newGeometry = gf->CreatePolygon(newExteriorRing, newInteriorRings);   
        }
        break;
    case FdoGeometryType_CurveString:
        {
            FdoICurveString * derivedGeom = static_cast<FdoICurveString *>(geometry);
            outputDim = applyTargetDimensionality ? targetDimensionality : derivedGeom->GetDimensionality();
            FdoPtr<FdoCurveSegmentCollection> csc = derivedGeom->GetCurveSegments();
            FdoPtr<FdoCurveSegmentCollection> newCsc = ConvertOrdinates(csc, outputDim, padValueZ, padValueM);

            newGeometry = gf->CreateCurveString(newCsc);
        }
        break;

    case FdoGeometryType_CurvePolygon:
        {
            FdoICurvePolygon * derivedGeom = static_cast<FdoICurvePolygon *>(geometry);
            outputDim = applyTargetDimensionality ? targetDimensionality : derivedGeom->GetDimensionality();
            FdoPtr<FdoRingCollection> newInteriorRings = FdoRingCollection::Create();

            FdoPtr<FdoIRing> ring = derivedGeom->GetExteriorRing();
            FdoPtr<FdoCurveSegmentCollection> csc = ring->GetCurveSegments();
            FdoPtr<FdoCurveSegmentCollection> newCsc = ConvertOrdinates(csc, outputDim, padValueZ, padValueM);
            FdoPtr<FdoIRing> newExteriorRing = gf->CreateRing(newCsc);

            for (FdoInt32 i = 0;  i < derivedGeom->GetInteriorRingCount();  i++)
            {
                ring = derivedGeom->GetInteriorRing(i);
                csc = ring->GetCurveSegments();
                newCsc = ConvertOrdinates(csc, outputDim, padValueZ, padValueM);
                FdoPtr<FdoIRing> newInteriorRing = gf->CreateRing(newCsc);
                newInteriorRings->Add(newInteriorRing);
            }

            newGeometry = gf->CreateCurvePolygon(newExteriorRing, newInteriorRings);   
        }
        break;

    case FdoGeometryType_MultiPoint:
        {
            FdoIMultiPoint * derivedGeom = static_cast<FdoIMultiPoint *>(geometry);
            dimensionality = derivedGeom->GetDimensionality();
            numPositions = derivedGeom->GetCount();
            outputDim = applyTargetDimensionality ? targetDimensionality : dimensionality;
            numOrdinates = numPositions * this->DimensionalityToNumOrdinates(outputDim);
            newOrdinates = AllocateOrdinates(numOrdinates);
            ordinates = derivedGeom->GetOrdinates();
            ConvertOrdinates(dimensionality, numPositions, ordinates, padValueZ, padValueM, outputDim, newOrdinates);
            newGeometry = gf->CreateMultiPoint(outputDim, numOrdinates, newOrdinates);
        }
        break;
    case FdoGeometryType_MultiLineString:
        {
            FdoIMultiLineString * derivedGeom = static_cast<FdoIMultiLineString *>(geometry);
            FdoPtr<FdoLineStringCollection> newSubGeometries = FdoLineStringCollection::Create();

            FdoInt32 numSubGeometries = derivedGeom->GetCount();

            for (FdoInt32 i = 0;  i < numSubGeometries;  i++)
            {
                FdoPtr<FdoILineString> subGeom = derivedGeom->GetItem(i);
                FdoPtr<FdoIGeometry> newSubGeometry = ConvertOrdinates(subGeom, applyTargetDimensionality, targetDimensionality);
                newSubGeometries->Add(static_cast<FdoILineString *>(newSubGeometry.p));
            }
            newGeometry = gf->CreateMultiLineString(newSubGeometries);
        }
        break;
    case FdoGeometryType_MultiPolygon:
        {
            FdoIMultiPolygon * derivedGeom = static_cast<FdoIMultiPolygon *>(geometry);
            FdoPtr<FdoPolygonCollection> newSubGeometries = FdoPolygonCollection::Create();

            FdoInt32 numSubGeometries = derivedGeom->GetCount();

            for (FdoInt32 i = 0;  i < numSubGeometries;  i++)
            {
                FdoPtr<FdoIPolygon> subGeom = derivedGeom->GetItem(i);
                FdoPtr<FdoIGeometry> newSubGeometry = ConvertOrdinates(subGeom, applyTargetDimensionality, targetDimensionality);
                newSubGeometries->Add(static_cast<FdoIPolygon *>(newSubGeometry.p));
            }
            newGeometry = gf->CreateMultiPolygon(newSubGeometries);
        }
        break;
    case FdoGeometryType_MultiCurveString:
        {
            FdoIMultiCurveString * derivedGeom = static_cast<FdoIMultiCurveString *>(geometry);
            FdoPtr<FdoCurveStringCollection> newSubGeometries = FdoCurveStringCollection::Create();

            FdoInt32 numSubGeometries = derivedGeom->GetCount();

            for (FdoInt32 i = 0;  i < numSubGeometries;  i++)
            {
                FdoPtr<FdoICurveString> subGeom = derivedGeom->GetItem(i);
                FdoPtr<FdoIGeometry> newSubGeometry = ConvertOrdinates(subGeom, applyTargetDimensionality, targetDimensionality);
                newSubGeometries->Add(static_cast<FdoICurveString *>(newSubGeometry.p));
            }
            newGeometry = gf->CreateMultiCurveString(newSubGeometries);
        }
        break;
    case FdoGeometryType_MultiCurvePolygon:
        {
            FdoIMultiCurvePolygon * derivedGeom = static_cast<FdoIMultiCurvePolygon *>(geometry);
            FdoPtr<FdoCurvePolygonCollection> newSubGeometries = FdoCurvePolygonCollection::Create();

            FdoInt32 numSubGeometries = derivedGeom->GetCount();

            for (FdoInt32 i = 0;  i < numSubGeometries;  i++)
            {
                FdoPtr<FdoICurvePolygon> subGeom = derivedGeom->GetItem(i);
                FdoPtr<FdoIGeometry> newSubGeometry = ConvertOrdinates(subGeom, applyTargetDimensionality, targetDimensionality);
                newSubGeometries->Add(static_cast<FdoICurvePolygon *>(newSubGeometry.p));
            }
            newGeometry = gf->CreateMultiCurvePolygon(newSubGeometries);
        }
        break;
    case FdoGeometryType_MultiGeometry:
        {
            FdoIMultiGeometry * derivedGeom = static_cast<FdoIMultiGeometry *>(geometry);
            FdoPtr<FdoGeometryCollection> newSubGeometries = FdoGeometryCollection::Create();

            FdoInt32 numSubGeometries = derivedGeom->GetCount();

            for (FdoInt32 i = 0;  i < numSubGeometries;  i++)
            {
                FdoPtr<FdoIGeometry> subGeom = derivedGeom->GetItem(i);
                FdoPtr<FdoIGeometry> newSubGeometry = ConvertOrdinates(subGeom, applyTargetDimensionality, targetDimensionality);
                newSubGeometries->Add(static_cast<FdoIGeometry *>(newSubGeometry.p));
            }
            newGeometry = gf->CreateMultiGeometry(newSubGeometries);
        }
        break;
    default:
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_10_UNSUPPORTEDGEOMETRYTYPE)));
    }

    return newGeometry;
}


FdoILinearRing * GeometryConverter::ConvertOrdinates(
    FdoILinearRing * ring,
    FdoInt32 outputDim,
    double padValueZ,
    double padValueM)
{
	FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
    FdoPtr<FdoILinearRing> newRing;

    const double * ordinates = NULL;
    double * newOrdinates = NULL;
    FdoInt32 numOrdinates;
    FdoInt32 numPositions;
    FdoInt32 dimensionality = ring->GetDimensionality();

    numPositions = ring->GetCount();
    numOrdinates = numPositions * this->DimensionalityToNumOrdinates(outputDim);
    newOrdinates = AllocateOrdinates(numOrdinates);
    ordinates = ring->GetOrdinates();
    ConvertOrdinates(dimensionality, numPositions, ordinates, padValueZ, padValueM, outputDim, newOrdinates);
    newRing = gf->CreateLinearRing(outputDim, numOrdinates, newOrdinates);
    delete [] newOrdinates;

    return FDO_SAFE_ADDREF(newRing.p);
}

FdoCurveSegmentCollection * GeometryConverter::ConvertOrdinates(
    FdoCurveSegmentCollection * csc,
    FdoInt32 outputDim,
    double padValueZ,
    double padValueM)
{
    FdoPtr<FdoCurveSegmentCollection> newCsc = FdoCurveSegmentCollection::Create();

    FdoInt32 count = csc->GetCount();
    for (FdoInt32 i=0;  i < count;  i++)
    {
        FdoPtr<FdoICurveSegmentAbstract> cs = csc->GetItem(i);
        FdoPtr<FdoICurveSegmentAbstract> newCs = ConvertOrdinates(cs, outputDim, padValueZ, padValueM);
        newCsc->Add(newCs);
    }

    return FDO_SAFE_ADDREF(newCsc.p);
}

FdoICurveSegmentAbstract * GeometryConverter::ConvertOrdinates(
    FdoICurveSegmentAbstract * cs,
    FdoInt32 outputDim,
    double padValueZ,
    double padValueM)
{
	FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
    FdoPtr<FdoICurveSegmentAbstract> newCs;

    const double * ordinates = NULL;
    double * newOrdinates = NULL;
    FdoInt32 numOrdinates;
    FdoInt32 numPositions;
    FdoInt32 dimensionality = cs->GetDimensionality();
    bool paddingZ = ( (outputDim&FdoDimensionality_Z) != 0 && (dimensionality&FdoDimensionality_Z) == 0 );
    bool paddingM = ( (outputDim&FdoDimensionality_M) != 0 && (dimensionality&FdoDimensionality_M) == 0 );

    FdoGeometryComponentType gct = cs->GetDerivedType();

    switch (gct)
    {
    case FdoGeometryComponentType_LineStringSegment:
        {
            FdoILineStringSegment * ls = static_cast<FdoILineStringSegment *> ((FdoICurveSegmentAbstract *)cs);
            numPositions = ls->GetCount();
            numOrdinates = numPositions * this->DimensionalityToNumOrdinates(outputDim);
            newOrdinates = AllocateOrdinates(numOrdinates);
            ordinates = ls->GetOrdinates();
            ConvertOrdinates(dimensionality, numPositions, ordinates, padValueZ, padValueM, outputDim, newOrdinates);
            newCs = gf->CreateLineStringSegment(outputDim, numOrdinates, newOrdinates);
            delete [] newOrdinates;
        }
        break;
    case FdoGeometryComponentType_CircularArcSegment:
        {
            FdoICircularArcSegment * arc = static_cast<FdoICircularArcSegment *> ((FdoICurveSegmentAbstract *)cs);
            FdoPtr<FdoIDirectPosition> start = arc->GetStartPosition();
            FdoPtr<FdoIDirectPosition> mid = arc->GetMidPoint();
            FdoPtr<FdoIDirectPosition> end = arc->GetEndPosition();
            numPositions = 1;
            numOrdinates = this->DimensionalityToNumOrdinates(outputDim);
            double ords[4];

            ordinates = start->GetOrdinates();
            ConvertOrdinates(dimensionality, numPositions, ordinates, padValueZ, padValueM, outputDim, ords);
            FdoPtr<FdoDirectPositionImpl> newStart = FdoDirectPositionImpl::Create(outputDim, ords);

            ordinates = mid->GetOrdinates();
            ConvertOrdinates(dimensionality, numPositions, ordinates, padValueZ, padValueM, outputDim, ords);
            FdoPtr<FdoDirectPositionImpl> newMid = FdoDirectPositionImpl::Create(outputDim, ords);

            ordinates = end->GetOrdinates();
            ConvertOrdinates(dimensionality, numPositions, ordinates, padValueZ, padValueM, outputDim, ords);
            FdoPtr<FdoDirectPositionImpl> newEnd = FdoDirectPositionImpl::Create(outputDim, ords);

            newCs = gf->CreateCircularArcSegment(newStart, newMid, newEnd);
        }
        break;
    default:
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_10_UNSUPPORTEDGEOMETRYTYPE)));
    }

    return FDO_SAFE_ADDREF(newCs.p);
}


void GeometryConverter::ConvertOrdinates(
    FdoInt32 inputDim,
    FdoInt32 numPositions,
    const double * inputOrds,
    double padValueZ,
    double padValueM,
    FdoInt32 outputDim,
    double * outputOrds)
{
    FdoInt32 numInputOrds = numPositions * DimensionalityToNumOrdinates(inputDim);
    bool inputHasZ = (inputDim&FdoDimensionality_Z) != 0;
    bool inputHasM = (inputDim&FdoDimensionality_M) != 0;
    bool outputHasZ = (outputDim&FdoDimensionality_Z) != 0;
    bool outputHasM = (outputDim&FdoDimensionality_M) != 0;
    bool paddingZ = ( outputHasZ && !inputHasZ );
    bool paddingM = ( outputHasM && !inputHasM );

    double x, y;
    double z = padValueZ;
    double m = padValueM;

    // Assume that ordinate arrays are well-formed (no check for number of positions,
    // end of array at a position boundary, etc.).

    FdoInt32 ordIndex1 = 0;
    FdoInt32 ordIndex2 = 0;

    // Branch on input dimensionality so that we don't have to test for it for every position.
    // Performance could similarly be slightly improved by also branching
    // on output dimensionality (requires more cases).

    if (!inputHasZ && !inputHasM)
    {
        while (ordIndex1 < numInputOrds)
        {
            x = inputOrds[ordIndex1++];
            y = inputOrds[ordIndex1++];
            ConvertPosition(x, y);
            outputOrds[ordIndex2++] = x;
            outputOrds[ordIndex2++] = y;
            if (paddingZ)
                outputOrds[ordIndex2++] = padValueZ;
            if (paddingM)
                outputOrds[ordIndex2++] = padValueM;
        }
    }
    else if (inputHasZ && !inputHasM)
    {
        while (ordIndex1 < numInputOrds)
        {
            x = inputOrds[ordIndex1++];
            y = inputOrds[ordIndex1++];
            z = inputOrds[ordIndex1++];
            ConvertPosition(x, y, z);
            outputOrds[ordIndex2++] = x;
            outputOrds[ordIndex2++] = y;
            if (outputHasZ)
                outputOrds[ordIndex2++] = z;
            if (paddingM)
                outputOrds[ordIndex2++] = padValueM;
        }
    }
    else if (!inputHasZ && inputHasM)
    {
        while (ordIndex1 < numInputOrds)
        {
            x = inputOrds[ordIndex1++];
            y = inputOrds[ordIndex1++];
            m = inputOrds[ordIndex1++];
            ConvertPosition(x, y);
            outputOrds[ordIndex2++] = x;
            outputOrds[ordIndex2++] = y;
            if (paddingZ)
                outputOrds[ordIndex2++] = padValueZ;
            if (outputHasM)
                outputOrds[ordIndex2++] = m;
        }
    }
    else // inputHasZ && inputHasM
    {
        while (ordIndex1 < numInputOrds)
        {
            x = inputOrds[ordIndex1++];
            y = inputOrds[ordIndex1++];
            z = inputOrds[ordIndex1++];
            m = inputOrds[ordIndex1++];
            ConvertPosition(x, y, z);
            outputOrds[ordIndex2++] = x;
            outputOrds[ordIndex2++] = y;
            if (outputHasZ)
                outputOrds[ordIndex2++] = z;
            if (outputHasM)
                outputOrds[ordIndex2++] = m;
        }
    }
}



void GeometryTest::testGeometryConverter()
{
	FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
    GeometryConverter gconv;
    FdoPtr<FdoByteArray> fgf2;

    FdoPtr<FdoIDirectPosition> pos = gf->CreatePositionXY(50, 60);
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

    DumpGeometry(ls);
    FdoPtr<FdoIGeometry> ls2 = gconv.ConvertOrdinates(ls);
    fgf2 = gf->GetFgf(ls2);
	CheckFGFT(ls2, L"LINESTRING (1 0, 3 2, 5 4)");

    DumpGeometry(ls);
    FdoPtr<FdoIGeometry> ls3 = gconv.ConvertOrdinates(ls, true, FdoDimensionality_XY|FdoDimensionality_Z, 2.0);
    fgf2 = gf->GetFgf(ls3);
	CheckFGFT(ls3, L"LINESTRING XYZ (1 0 2, 3 2 2, 5 4 2)");

    DumpGeometry(pt);
    FdoPtr<FdoIGeometry> pt2 = gconv.ConvertOrdinates(pt);
    fgf2 = gf->GetFgf(pt2);
	CheckFGFT(pt2, L"POINT (3 5)");

    DumpGeometry(pg);
    FdoPtr<FdoIGeometry> pg2 = gconv.ConvertOrdinates(pg, false);
    fgf2 = gf->GetFgf(pg2);
	CheckFGFT(pg2, L"POLYGON ((0 0, 0 5, 5 5, 5 0, 0 0), (1 1, 1 2, 2 2, 1 1), (3 3, 3 4, 4 4, 3 3))");

    DumpGeometry(cs);
    FdoPtr<FdoIGeometry> cs2 = gconv.ConvertOrdinates(cs);
    fgf2 = gf->GetFgf(cs2);
	CheckFGFT(cs2, L"CURVESTRING (0 0 (CIRCULARARCSEGMENT (1 0, 2 1), LINESTRINGSEGMENT (0 3, 2 3)))");

    DumpGeometry(cpg);
    FdoPtr<FdoIGeometry> cpg2 = gconv.ConvertOrdinates(cpg);
    fgf2 = gf->GetFgf(cpg2);
	CheckFGFT(cpg2, L"CURVEPOLYGON ((200 200 (CIRCULARARCSEGMENT (201 200, 202 201), LINESTRINGSEGMENT (200 200))), (300 300 (CIRCULARARCSEGMENT (301 300, 302 301), LINESTRINGSEGMENT (300 300))), (400 400 (CIRCULARARCSEGMENT (401 400, 402 401), LINESTRINGSEGMENT (400 400))))");

    DumpGeometry(mpt);
    FdoPtr<FdoIGeometry> mpt2 = gconv.ConvertOrdinates(mpt);
    fgf2 = gf->GetFgf(mpt2);
	CheckFGFT(mpt2, L"MULTIPOINT XYZ (2 1 3, 5 4 6, 8 7 9)");

    DumpGeometry(mls);
    FdoPtr<FdoIGeometry> mls2 = gconv.ConvertOrdinates(mls);
    fgf2 = gf->GetFgf(mls2);
	CheckFGFT(mls2, L"MULTILINESTRING XYZ ((1 0 2, 4 3 5, 7 6 8), (10 9 11, 13 12 14, 16 15 17))");

    DumpGeometry(mls);
    FdoPtr<FdoIGeometry> mls3 = gconv.ConvertOrdinates(mls, true, FdoDimensionality_XY);
    fgf2 = gf->GetFgf(mls3);
	CheckFGFT(mls3, L"MULTILINESTRING ((1 0, 4 3, 7 6), (10 9, 13 12, 16 15))");

    DumpGeometry(mpg);
    FdoPtr<FdoIGeometry> mpg2 = gconv.ConvertOrdinates(mpg);
    fgf2 = gf->GetFgf(mpg2);
	CheckFGFT(mpg2, L"MULTIPOLYGON (((0 0, 0 5, 5 5, 5 0, 0 0), (1 1, 1 2, 2 2, 1 1), (3 3, 3 4, 4 4, 3 3)), ((0 0, 0 5, 5 5, 5 0, 0 0), (1 1, 1 2, 2 2, 1 1), (3 3, 3 4, 4 4, 3 3)))");

    DumpGeometry(mcs);
    FdoPtr<FdoIGeometry> mcs2 = gconv.ConvertOrdinates(mcs);
    fgf2 = gf->GetFgf(mcs2);
	CheckFGFT(mcs2, L"MULTICURVESTRING ((100 100 (CIRCULARARCSEGMENT (101 100, 102 101), LINESTRINGSEGMENT (100 103, 102 103))), (200 200 (CIRCULARARCSEGMENT (201 200, 202 201), LINESTRINGSEGMENT (200 203, 202 203))), (300 300 (CIRCULARARCSEGMENT (301 300, 302 301), LINESTRINGSEGMENT (300 303, 302 303))))");

    DumpGeometry(mcpg);
    FdoPtr<FdoIGeometry> mcpg2 = gconv.ConvertOrdinates(mcpg);
    fgf2 = gf->GetFgf(mcpg2);
	CheckFGFT(mcpg2, L"MULTICURVEPOLYGON (((200 200 (CIRCULARARCSEGMENT (201 200, 202 201), LINESTRINGSEGMENT (200 200))), (300 300 (CIRCULARARCSEGMENT (301 300, 302 301), LINESTRINGSEGMENT (300 300))), (400 400 (CIRCULARARCSEGMENT (401 400, 402 401), LINESTRINGSEGMENT (400 400)))), ((201 201 (CIRCULARARCSEGMENT (202 201, 203 202), LINESTRINGSEGMENT (201 201))), (301 301 (CIRCULARARCSEGMENT (302 301, 303 302), LINESTRINGSEGMENT (301 301))), (401 401 (CIRCULARARCSEGMENT (402 401, 403 402), LINESTRINGSEGMENT (401 401)))), ((202 202 (CIRCULARARCSEGMENT (203 202, 204 203), LINESTRINGSEGMENT (202 202))), (302 302 (CIRCULARARCSEGMENT (303 302, 304 303), LINESTRINGSEGMENT (302 302))), (402 402 (CIRCULARARCSEGMENT (403 402, 404 403), LINESTRINGSEGMENT (402 402)))))");

#if 0
    DumpGeometry(mg);
    FdoPtr<FdoIGeometry> mg2 = gconv.ConvertOrdinates(mg, false, 0);
    fgf2 = gf->GetFgf(mg2);
	CheckFGFT(mg2, L"GEOMETRYCOLLECTION (CURVEPOLYGON ((100 100 (CIRCULARARCSEGMENT (101 100, 102 101), LINESTRINGSEGMENT (100 100))), (200 200 (CIRCULARARCSEGMENT (201 200, 202 201), LINESTRINGSEGMENT (200 200))), (300 300 (CIRCULARARCSEGMENT (301 300, 302 301), LINESTRINGSEGMENT (300 300)))), CURVESTRING (100 100 (CIRCULARARCSEGMENT (101 100, 102 101), LINESTRINGSEGMENT (100 103, 102 103))), LINESTRING (1 0, 2 3, 5 4), POINT XYZ (3 5 2), POLYGON ((0 0, 0 5, 5 5, 5 0, 0 0), (1 1, 1 2, 2 2, 1 1), (3 3, 3 4, 4 4, 3 3)))");
#endif
}

////////////////////////////////////////////////////////////////////////////
// WKB test functions.  The logic for these are (loosely) adapted from an OGC
// conversion utility by Kurt Buehler, available at:
// http://www.opengeospatial.org/resources/?page=testing&view=sfsql.
// Because of this origin, this code should not be incorporated into released
// commercial software (i.e. it must remain in unreleased test code).

#undef BIG_ENDIAN
#define BIG_ENDIAN    (0)
#undef LITTLE_ENDIAN
#define LITTLE_ENDIAN    (1)

#define WKBMINTYPE 1

#define WKBPOINT 1
#define WKBLINESTRING 2
#define WKBPOLYGON 3
#define WKBMULTIPOINT 4
#define WKBMULTILINESTRING 5
#define WKBMULTIPOLYGON 6
#define WKBGEOMETRYCOLLECTION 7

#define WKBMAXTYPE 7

#define WKBGEOMETRY 0

int swaporder;

void get_geometry(FdoInt32 type, FdoByte ** inputData, wchar_t * outputText);
FdoInt32 get_wkbType(FdoByte ** inputData);
FdoByte get_hostbyteorder();
FdoByte get_inbyteorder(FdoByte ** inputData);

FdoString *wkbTypes[] = {
	L"",
	L"WKBPOINT",
	L"WKBLINESTRING",
	L"WKBPOLYGON",
	L"WKBMULTIPOINT",
	L"WKBMULTILINESTRING",
	L"WKBMULTIPOLYGON",
	L"WKBGEOMETRYCOLLECTION"
};

void
CheckWkb(FdoByte * inputData) 
{
    wchar_t outputText[100000];    // This is plenty to check this unit test's cases.
	FdoInt32 type;
    FdoByte inputbyteorder, hostbyteorder;

    wcscpy(outputText, L"");

	hostbyteorder = get_hostbyteorder();

	inputbyteorder = get_inbyteorder(&inputData);
	if (inputbyteorder == 255 || hostbyteorder != inputbyteorder)
        throw exception();

	type = get_wkbType(&inputData);
	if (type == -1)
        throw exception();

	get_geometry(type, &inputData, outputText);

#ifdef DEBUG_DETAIL
    printf("WKT=%ls", outputText);
#endif
}

void
writeXY(double x, double y, FdoByte ** inputData, wchar_t * outputText)
{
	wchar_t buffer1[100];
	wchar_t buffer2[100];
	FdoStringP buffer3;
    FdoInt32 maxLength = (sizeof(buffer1)-1)/sizeof(buffer1[1]);

    FdoStringUtility::FormatDouble(x, buffer1, maxLength);
    FdoStringUtility::FormatDouble(y, buffer2, maxLength);
    buffer3 = FdoStringP::Format(L"%ls %ls", buffer1, buffer2);

    wcscat(outputText, buffer3);
}

FdoInt32
readInt32(FdoByte ** inputData)
{
    FdoInt32 * ptr = (FdoInt32 *) (*inputData);
    FdoInt32 datum = *ptr;
    (*inputData) += sizeof(datum);
    return datum;
}

double
readDouble(FdoByte ** inputData)
{
    double * ptr = (double *) (*inputData);
    double datum = *ptr;
    (*inputData) += sizeof(datum);
    return datum;
}
void
get_geometry(FdoInt32 type, FdoByte ** inputData, wchar_t * outputText)
{
	int firstPart = 1;

	switch(type) {
		case WKBPOINT:
			{
                wcscat(outputText, L"POINT (");
				/* read the X */
				double x = readDouble(inputData);
				double y = readDouble(inputData);
				writeXY(x, y, inputData, outputText);
                wcscat(outputText, L")");
			}
			break;
		case WKBLINESTRING:
			{ 
				wcscat(outputText, L"LINESTRING (");
				FdoInt32 numPos = readInt32(inputData);

				for (int i = 0;  i < numPos;  i++)
                {
				    double x = readDouble(inputData);
				    double y = readDouble(inputData);
				    writeXY(x, y, inputData, outputText);
				    if (i < numPos-1)
                        wcscat(outputText, L", ");
				}
                wcscat(outputText, L")");
			}
			break;
		case WKBPOLYGON:
			{
				wcscat(outputText, L"POLYGON (");
				FdoInt32 numRings = readInt32(inputData);

				for (int i = 0;  i < numRings;  i++ )
                {
					wcscat(outputText, L" (");
    				FdoInt32 numPos = readInt32(inputData);
					for (int j = 0;  j < numPos;  j++ )
                    {
				        double x = readDouble(inputData);
				        double y = readDouble(inputData);
				        writeXY(x, y, inputData, outputText);
				        if (i < numPos-1)
                            wcscat(outputText, L", ");
                    }
					wcscat(outputText, L")");
					if ( i < numRings-1 )
                        wcscat(outputText, L", ");
				}
                wcscat(outputText, L" )");
			}
			break;
		case WKBMULTIPOINT:
			{
				wcscat(outputText, L"MULTIPOINT (");
				FdoInt32 numSubGeometries = readInt32(inputData);

				for (int i = 0;  i < numSubGeometries;  i++)
                {
					// Skip endian flag and type of contained position.
					FdoByte endian = get_inbyteorder(inputData);
					FdoByte type = get_wkbType(inputData);

					wcscat(outputText, L" (");

				    double x = readDouble(inputData);
				    double y = readDouble(inputData);
				    writeXY(x, y, inputData, outputText);

					wcscat(outputText, L") ");
				    if (i < numSubGeometries-1)
                        wcscat(outputText, L", ");
				}
				wcscat(outputText, L")");
			}
			break;
		case WKBMULTILINESTRING:
			{
				wcscat(outputText, L"MULTILINESTRING (");
				FdoInt32 numSubGeometries = readInt32(inputData);

				for (int i = 0;  i < numSubGeometries;  i++)
                {
					// Skip endian flag and type of contained position.
					FdoByte endian = get_inbyteorder(inputData);
					FdoByte type = get_wkbType(inputData);

					wcscat(outputText, L" (");

    				FdoInt32 numPos = readInt32(inputData);
					for (int j = 0;  j < numPos;  j++ )
                    {
				        double x = readDouble(inputData);
				        double y = readDouble(inputData);
				        writeXY(x, y, inputData, outputText);
				        if (i < numPos-1)
                            wcscat(outputText, L", ");
					}
    				wcscat(outputText, L")");
				    if (i < numSubGeometries-1)
                        wcscat(outputText, L", ");
				}
  				wcscat(outputText, L")");
			}
			break;
		case WKBMULTIPOLYGON:
			{
				wcscat(outputText, L"MULTIPOLYGON (");
				FdoInt32 numSubGeometries = readInt32(inputData);

				for (int i = 0;  i < numSubGeometries;  i++)
                {
					// Skip endian flag and type of contained position.
					FdoByte endian = get_inbyteorder(inputData);
					FdoByte type = get_wkbType(inputData);

					wcscat(outputText, L" (");

				    FdoInt32 numRings = readInt32(inputData);

				    for (int r = 0;  r < numRings;  r++ )
                    {
					    wcscat(outputText, L"( ");
    				    FdoInt32 numPos = readInt32(inputData);
					    for (int p = 0;  p < numPos;  p++ )
                        {
				            double x = readDouble(inputData);
				            double y = readDouble(inputData);
				            writeXY(x, y, inputData, outputText);
				            if (i < numPos-1)
                                wcscat(outputText, L", ");
                        }
					    wcscat(outputText, L" )");
					    if ( i < numRings-1 )
                            wcscat(outputText, L", ");
				    }
                    wcscat(outputText, L")");
				}
                wcscat(outputText, L")");
			}
			break;
		case WKBGEOMETRYCOLLECTION:
			{
				wcscat(outputText, L"GEOMETRYCOLLECTION (");
				FdoInt32 numSubGeometries = readInt32(inputData);

				for (int i = 0;  i < numSubGeometries;  i++)
                {
					FdoByte b = get_inbyteorder(inputData);
                    FdoByte type = get_wkbType(inputData);

					get_geometry(type, inputData, outputText);

				    if (i < numSubGeometries-1)
                        wcscat(outputText, L", ");
				}
                wcscat(outputText, L")");
			}
			break;
	}
}

FdoByte
get_hostbyteorder()
{
#if 0
    FdoInt16 host = 5;
	FdoInt16 net = htons(host);

	if ( net == host )
		return(BIG_ENDIAN);
	else
		return(LITTLE_ENDIAN);
#else
    return LITTLE_ENDIAN;
#endif
}

FdoByte
get_inbyteorder(FdoByte ** inputData)
{
    FdoByte b = **inputData;
    (*inputData)++;
	return(b);
}

FdoInt32
get_wkbType(FdoByte ** inputData)
{
    FdoInt32 type = readInt32(inputData);

	if (type > WKBMAXTYPE || type < WKBMINTYPE)
        throw exception();

	return(type);
}
