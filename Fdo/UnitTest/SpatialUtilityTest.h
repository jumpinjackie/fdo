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

#ifndef CPP_UNIT_SPATIALUTILITY_H
#define CPP_UNIT_SPATIALUTILITY_H

// Test cases for FdoSpatialUtility.
class SpatialUtilityTest : public CppUnit::TestCase
{
	FDO_CPPUNIT_DEFINE(testValidateGeometryByType);

	// FdoGeometryType_Point
	FDO_CPPUNIT_DEFINE(testLinearizePoint);

	// FdoGeometryType_LineString
	FDO_CPPUNIT_DEFINE(testLinearizeLineString);

	// FdoGeometryType_Polygon
	FDO_CPPUNIT_DEFINE(testLinearizePolygon);

	// FdoGeometryType_MultiPoint
	FDO_CPPUNIT_DEFINE(testLinearizeMultiPoint);

	// FdoGeometryType_MultiLineString
	FDO_CPPUNIT_DEFINE(testLinearizeMultiLineString);

	// FdoGeometryType_MultiPolygon
	FDO_CPPUNIT_DEFINE(testLinearizeMultiPolygon);

	// FdoGeometryType_MultiGeometry
	FDO_CPPUNIT_DEFINE(testLinearizeMultiGeometry);

	// FdoGeometryType_CurveString
	FDO_CPPUNIT_DEFINE(testLinearizeCurveString);

	// FdoGeometryType_CurvePolygon
	FDO_CPPUNIT_DEFINE(testLinearizeCurvePolygon);

	// FdoGeometryType_MultiCurveString
	FDO_CPPUNIT_DEFINE(testLinearizeMultiCurveString);

	// FdoGeometryType_MultiCurvePolygon
	FDO_CPPUNIT_DEFINE(testLinearizeMultiCurvePolygon);

    // Test FdoSpatialGridTransform
	FDO_CPPUNIT_DEFINE(testGridTransform);

    // Test FdoSpatialUtility::GetExtents
    FDO_CPPUNIT_DEFINE(testGetExtents);

    CPPUNIT_TEST_SUITE(SpatialUtilityTest);
	CPPUNIT_TEST(testValidateGeometryByType);
	CPPUNIT_TEST(testLinearizePoint);
	CPPUNIT_TEST(testLinearizeLineString);
	CPPUNIT_TEST(testLinearizePolygon);
	CPPUNIT_TEST(testLinearizeMultiPoint);
	CPPUNIT_TEST(testLinearizeMultiLineString);
	CPPUNIT_TEST(testLinearizeMultiPolygon);
	CPPUNIT_TEST(testLinearizeMultiGeometry);
	CPPUNIT_TEST(testLinearizeCurveString);
	CPPUNIT_TEST(testLinearizeCurvePolygon);
	CPPUNIT_TEST(testLinearizeMultiCurveString);
	CPPUNIT_TEST(testLinearizeMultiCurvePolygon);
	CPPUNIT_TEST(testGridTransform);
	CPPUNIT_TEST(testGetExtents);
    CPPUNIT_TEST_SUITE_END();

public:
    SpatialUtilityTest(void);
    virtual ~SpatialUtilityTest(void);
    void setUp();

protected:
	void testValidateGeometryByType();
	void testLinearizeLineString();
	void testLinearizePolygon();
	void testLinearizePoint();
	void testLinearizeMultiPoint();
	void testLinearizeMultiLineString();
	void testLinearizeMultiPolygon();
	void testLinearizeCurveString();
	void testLinearizeMultiCurveString();
	void testLinearizeCurvePolygon();
	void testLinearizeMultiCurvePolygon();
	void testLinearizeMultiGeometry();
	void testGridTransform();
	void testGetExtents();
};

#endif

