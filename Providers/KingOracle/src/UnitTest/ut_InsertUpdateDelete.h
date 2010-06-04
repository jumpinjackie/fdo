#pragma once

class ut_InsertUpdateDelete : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( ut_InsertUpdateDelete );
    CPPUNIT_TEST( InsertUpdateDelete );    
    CPPUNIT_TEST( InsertCurvedPolygon );    
    CPPUNIT_TEST( InsertUnicode );    
    CPPUNIT_TEST( InsertMultiPoint );
    CPPUNIT_TEST( InsertMultiGeometry );
    CPPUNIT_TEST( Update_BLDG_POLYGON );
  CPPUNIT_TEST_SUITE_END();
  
public:
  ut_InsertUpdateDelete(void);
public:
  ~ut_InsertUpdateDelete(void);
  
  
  void InsertUpdateDelete();
  void InsertCurvedPolygon();
  void InsertUnicode();
  
  void InsertMultiPoint();
  void InsertMultiGeometry();
  
  void Update_BLDG_POLYGON();
  
protected:
  void PrepareTables(FdoIConnection* Connection);  
  

};
