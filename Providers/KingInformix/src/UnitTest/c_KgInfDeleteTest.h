#pragma once


class c_KgInfDeleteTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( c_KgInfDeleteTest );
    CPPUNIT_TEST( DeleteTest );    
     
  CPPUNIT_TEST_SUITE_END();
public:
  c_KgInfDeleteTest(void);
public:
     ~c_KgInfDeleteTest(void);
  
  
  void DeleteTest();   
 
  

protected:
  void SetupSchema();
  
};


