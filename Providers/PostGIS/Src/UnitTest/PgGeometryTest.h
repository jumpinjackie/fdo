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

};

#endif // FDOPOSTGIS_PGGEOMETRYTEST_H_INCLUDED
