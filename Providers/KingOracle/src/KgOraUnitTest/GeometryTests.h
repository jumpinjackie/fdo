#ifndef GEOMETRY_TESTS_H
#define GEOMETRY_TESTS_H

#include <cppunit/extensions/HelperMacros.h>

class GeometryTests : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(GeometryTests);
    CPPUNIT_TEST(TestSdoPointType);
    CPPUNIT_TEST(GeometryTypes);
    CPPUNIT_TEST(TestCurvedSegment);
    CPPUNIT_TEST(TestPolygons3D);
    CPPUNIT_TEST(TestHasElevation);
    CPPUNIT_TEST(TestGeometryCollection);
    CPPUNIT_TEST_SUITE_END();

  public:
    GeometryTests(void);

  public:
    ~GeometryTests(void);

  public:
    void GeometryTypes();

    void TestCurvedSegment();
    void TestPolygons3D();
    void TestHasElevation();
    void TestGeometryCollection();
    void TestSdoPointType();
};

#endif