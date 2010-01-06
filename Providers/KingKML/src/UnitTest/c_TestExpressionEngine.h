#pragma once

class c_TestExpressionEngine : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( c_TestExpressionEngine );
  CPPUNIT_TEST( TryEngine );
  CPPUNIT_TEST( TestMemory );
  CPPUNIT_TEST( TestSdfConn );
  CPPUNIT_TEST_SUITE_END();
public:
  c_TestExpressionEngine(void);
public:
  ~c_TestExpressionEngine(void);
  
  void TryEngine();
  
  void TestSdfConn();
  
   void TestMemory();
};
