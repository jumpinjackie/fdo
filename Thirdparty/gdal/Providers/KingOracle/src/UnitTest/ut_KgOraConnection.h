#pragma once








class ut_KgOraConnection : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( ut_KgOraConnection );
    CPPUNIT_TEST( OpenConnection );
    CPPUNIT_TEST( QueryData );
    CPPUNIT_TEST( QueryDataSameConnection );
    CPPUNIT_TEST( PerfomanceTest );
    CPPUNIT_TEST( MultithreadTest );
  CPPUNIT_TEST_SUITE_END();
public:
  ut_KgOraConnection(void);
public:
     ~ut_KgOraConnection(void);
  
  FdoPtr<FdoIConnection> m_Conn;
  
  void OpenConnection();
  void QueryData();
  void QueryDataSameConnection();
  
  
  void PerfomanceTest();
  void MultithreadTest();
  
   virtual void setUp();
  virtual void tearDown();
  
};


