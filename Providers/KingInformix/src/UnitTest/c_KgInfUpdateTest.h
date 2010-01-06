#pragma once


class c_KgInfUpdateTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( c_KgInfUpdateTest );
    CPPUNIT_TEST( UpdateTest );    
     
  CPPUNIT_TEST_SUITE_END();
public:
  c_KgInfUpdateTest(void);
public:
     ~c_KgInfUpdateTest(void);
  
  
  void UpdateTest();   
 
  

protected:
  void SetupSchema();
  
};


