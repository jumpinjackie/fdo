//
// Copyright (C) 2007 Refractions Research, Inc. 
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
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
//
#ifndef FDOPOSTGIS_PGGEOMETRYTEST_H_INCLUDED
#define FDOPOSTGIS_PGGEOMETRYTEST_H_INCLUDED

class PgGeometryTest : public CppUnit::TestCase
{
    CPPUNIT_TEST_SUITE(PgGeometryTest);
    CPPUNIT_TEST(testPoint);
    CPPUNIT_TEST(testPointSRID);
    CPPUNIT_TEST(testPointZ);
    CPPUNIT_TEST(testPointSRIDZ);
    CPPUNIT_TEST(testPointM);
    CPPUNIT_TEST(testPointSRIDM);
    CPPUNIT_TEST(testPointZM);
    CPPUNIT_TEST(testPointSRIDZM);
    CPPUNIT_TEST(testLineString);
    CPPUNIT_TEST(testLineStringSRID);
    CPPUNIT_TEST(testLineStringZ);
    CPPUNIT_TEST(testLineStringSRIDZ);
    CPPUNIT_TEST(testLineStringM);
    CPPUNIT_TEST(testLineStringSRIDM);
    CPPUNIT_TEST(testLineStringZM);
    CPPUNIT_TEST(testLineStringSRIDZM);
    CPPUNIT_TEST(testPolygon);
    CPPUNIT_TEST(testPolygonSRID);
    CPPUNIT_TEST(testPolygonZ);
    CPPUNIT_TEST(testPolygonSRIDZ);
    CPPUNIT_TEST(testPolygonM);
    CPPUNIT_TEST(testPolygonSRIDM);
    CPPUNIT_TEST(testPolygonZM);
    CPPUNIT_TEST(testPolygonSRIDZM);
    CPPUNIT_TEST(testMultiPoint);
    CPPUNIT_TEST(testMultiPointSRID);
    CPPUNIT_TEST(testMultiPointZ);
    CPPUNIT_TEST(testMultiPointSRIDZ);
    CPPUNIT_TEST(testMultiPointM);
    CPPUNIT_TEST(testMultiPointSRIDM);
    CPPUNIT_TEST(testMultiPointZM);
    CPPUNIT_TEST(testMultiPointSRIDZM);

    CPPUNIT_TEST(testMultiLineString);
    CPPUNIT_TEST(testMultiLineStringSRID);
    CPPUNIT_TEST(testMultiLineStringZ);
    CPPUNIT_TEST(testMultiLineStringSRIDZ);
    CPPUNIT_TEST(testMultiLineStringM);
    CPPUNIT_TEST(testMultiLineStringSRIDM);
    CPPUNIT_TEST(testMultiLineStringZM);
    CPPUNIT_TEST(testMultiLineStringSRIDZM);

    CPPUNIT_TEST(testMultiPolygon);
    CPPUNIT_TEST(testMultiPolygonSRID);
    CPPUNIT_TEST(testMultiPolygonZ);
    CPPUNIT_TEST(testMultiPolygonSRIDZ);
    CPPUNIT_TEST(testMultiPolygonM);
    CPPUNIT_TEST(testMultiPolygonSRIDM);
    CPPUNIT_TEST(testMultiPolygonZM);
    CPPUNIT_TEST(testMultiPolygonSRIDZM);
    CPPUNIT_TEST_SUITE_END();

public:

    PgGeometryTest();
    virtual ~PgGeometryTest();

	virtual void setUp();
	virtual void tearDown();

    //
    // Tests
    //

    void testPoint();
    void testPointSRID();
    void testPointZ();
    void testPointSRIDZ();
    void testPointM();
    void testPointSRIDM();
    void testPointZM();
    void testPointSRIDZM();

    void testLineString();
    void testLineStringSRID();
    void testLineStringZ();
    void testLineStringSRIDZ();
    void testLineStringM();
    void testLineStringSRIDM();
    void testLineStringZM();
    void testLineStringSRIDZM();

    void testPolygon();
    void testPolygonSRID();
    void testPolygonZ();
    void testPolygonSRIDZ();
    void testPolygonM();
    void testPolygonSRIDM();
    void testPolygonZM();
    void testPolygonSRIDZM();

    void testMultiPoint();
    void testMultiPointSRID();
    void testMultiPointZ();
    void testMultiPointSRIDZ();
    void testMultiPointM();
    void testMultiPointSRIDM();
    void testMultiPointZM();
    void testMultiPointSRIDZM();

    void testMultiLineString();
    void testMultiLineStringSRID();
    void testMultiLineStringZ();
    void testMultiLineStringSRIDZ();
    void testMultiLineStringM();
    void testMultiLineStringSRIDM();
    void testMultiLineStringZM();
    void testMultiLineStringSRIDZM();

    void testMultiPolygon();
    void testMultiPolygonSRID();
    void testMultiPolygonZ();
    void testMultiPolygonSRIDZ();
    void testMultiPolygonM();
    void testMultiPolygonSRIDM();
    void testMultiPolygonZM();
    void testMultiPolygonSRIDZM();

private:

    std::string BuildTestGeometry(std::string const& ewkbhex);

};

#endif // FDOPOSTGIS_PGGEOMETRYTEST_H_INCLUDED
