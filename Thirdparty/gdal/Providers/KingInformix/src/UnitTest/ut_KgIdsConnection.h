#pragma once








class ut_KgIdsConnection : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( ut_KgIdsConnection );
    CPPUNIT_TEST( OpenConnection );    
  CPPUNIT_TEST_SUITE_END();
public:
  ut_KgIdsConnection(void);
public:
     ~ut_KgIdsConnection(void);
  
  
  void OpenConnection();
  
  
  void PerfomanceTest();
  

  
};


