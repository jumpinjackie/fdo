#pragma once


class c_KgInfInsertTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( c_KgInfInsertTest );
    CPPUNIT_TEST( InsertTest );
    CPPUNIT_TEST( InsertTestDecimal );    
    
  CPPUNIT_TEST_SUITE_END();
public:
  c_KgInfInsertTest(void);
public:
     ~c_KgInfInsertTest(void);
  
  
  void InsertTest();   
  void InsertTestDecimal();
  

protected:
  void SetupSchema();
  
};


