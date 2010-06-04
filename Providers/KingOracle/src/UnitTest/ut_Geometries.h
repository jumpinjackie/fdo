#pragma once

class ut_Geometries : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( ut_Geometries );
    CPPUNIT_TEST( GeometryTypes );    
    CPPUNIT_TEST( TestCurvedSegment );  
    CPPUNIT_TEST( TestPolygons3D );  
    CPPUNIT_TEST( TestHasElevation );
    CPPUNIT_TEST( TestGeometryCollection ); 
  CPPUNIT_TEST_SUITE_END();
  
public:
  ut_Geometries(void);
public:
  ~ut_Geometries(void);
  
public:
  void GeometryTypes();  
  
  void TestCurvedSegment();
  void TestPolygons3D();
  void TestHasElevation();
  void TestGeometryCollection();
};


