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

#ifdef _WIN32
#pragma once
#endif

#ifndef CPP_UNIT_GEOMETRYTEST_H
#define CPP_UNIT_GEOMETRYTEST_H

// Test cases for FdoGeometry.
class GeometryTest : public CppUnit::TestCase
{
	// test basics
	FDO_CPPUNIT_DEFINE(testDirectPosition);
	FDO_CPPUNIT_DEFINE(testEnvelope);
	FDO_CPPUNIT_DEFINE(testLinearRing);
	FDO_CPPUNIT_DEFINE(testLineStringSegment);
	FDO_CPPUNIT_DEFINE(testCircularArcSegment);
	FDO_CPPUNIT_DEFINE(testRing);

	// FdoGeometryType_Point
	FDO_CPPUNIT_DEFINE(testPoint);

	// FdoGeometryType_LineString
	FDO_CPPUNIT_DEFINE(testLineString);

	// FdoGeometryType_Polygon
	FDO_CPPUNIT_DEFINE(testPolygon);

	// FdoGeometryType_MultiPoint
	FDO_CPPUNIT_DEFINE(testMultiPoint);

	// FdoGeometryType_MultiLineString
	FDO_CPPUNIT_DEFINE(testMultiLineString);

	// FdoGeometryType_MultiPolygon
	FDO_CPPUNIT_DEFINE(testMultiPolygon);

	// FdoGeometryType_MultiGeometry
	FDO_CPPUNIT_DEFINE(testMultiGeometry);

	// FdoGeometryType_CurveString
	FDO_CPPUNIT_DEFINE(testCurveString);

	// FdoGeometryType_CurvePolygon
	FDO_CPPUNIT_DEFINE(testCurvePolygon);

	// FdoGeometryType_MultiCurveString
	FDO_CPPUNIT_DEFINE(testMultiCurveString);

	// FdoGeometryType_MultiCurvePolygon
	FDO_CPPUNIT_DEFINE(testMultiCurvePolygon);

	// Geometry created from an Envelope.
	FDO_CPPUNIT_DEFINE(testGeometryFromEnvelope);

	// Test some example geometry manipulation code.
	FDO_CPPUNIT_DEFINE(testGeometryConverter);

    CPPUNIT_TEST_SUITE(GeometryTest);

	// test basics
	CPPUNIT_TEST(testDirectPosition);
	CPPUNIT_TEST(testEnvelope);
	CPPUNIT_TEST(testLinearRing);
	CPPUNIT_TEST(testLineStringSegment);
	CPPUNIT_TEST(testCircularArcSegment);
	CPPUNIT_TEST(testRing);

	// FdoGeometryType_Point
	CPPUNIT_TEST(testPoint);

	// FdoGeometryType_LineString
	CPPUNIT_TEST(testLineString);

	// FdoGeometryType_Polygon
	CPPUNIT_TEST(testPolygon);

	// FdoGeometryType_MultiPoint
	CPPUNIT_TEST(testMultiPoint);

	// FdoGeometryType_MultiLineString
	CPPUNIT_TEST(testMultiLineString);

	// FdoGeometryType_MultiPolygon
	CPPUNIT_TEST(testMultiPolygon);

	// FdoGeometryType_MultiGeometry
	CPPUNIT_TEST(testMultiGeometry);

	// FdoGeometryType_CurveString
	CPPUNIT_TEST(testCurveString);

	// FdoGeometryType_CurvePolygon
	CPPUNIT_TEST(testCurvePolygon);

	// FdoGeometryType_MultiCurveString
	CPPUNIT_TEST(testMultiCurveString);

	// FdoGeometryType_MultiCurvePolygon
	CPPUNIT_TEST(testMultiCurvePolygon);

	// Geometry created from an Envelope.
	CPPUNIT_TEST(testGeometryFromEnvelope);

	// Test some example geometry manipulation code.
	CPPUNIT_TEST(testGeometryConverter);

    CPPUNIT_TEST_SUITE_END();

public:
    GeometryTest(void);
    virtual ~GeometryTest(void);
    void setUp();

protected:
	void testDirectPosition();
	void testEnvelope();
	void testLineString();
	void testLinearRing();
	void testLineStringSegment();
	void testPolygon();
	void testPoint();
	void testMultiPoint();
	void testMultiLineString();
	void testMultiPolygon();
	void testCircularArcSegment();
	void testCurveString();
	void testMultiCurveString();
	void testRing();
	void testCurvePolygon();
	void testMultiCurvePolygon();
	void testMultiGeometry();
	void testGeometryFromEnvelope();
	void testGeometryConverter();
};

#endif

