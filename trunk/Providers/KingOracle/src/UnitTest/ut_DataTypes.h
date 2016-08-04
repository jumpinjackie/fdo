#pragma once

class ut_DataTypes : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( ut_DataTypes );
    CPPUNIT_TEST( DataTypes );    
    CPPUNIT_TEST( GeometryTransform ); 
    CPPUNIT_TEST( elektro_lob ); 
  CPPUNIT_TEST_SUITE_END();
  
public:
  ut_DataTypes(void);
public:
  ~ut_DataTypes(void);
  
  void DataTypes();
  void GeometryTransform();
  void elektro_lob();
};
