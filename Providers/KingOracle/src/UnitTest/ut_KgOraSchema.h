#pragma once


class ut_KgOraSchema : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( ut_KgOraSchema );
    CPPUNIT_TEST( DescribeSchema );    
    CPPUNIT_TEST( DifferentOwner );    
    CPPUNIT_TEST( JustMyTest ); 
  CPPUNIT_TEST_SUITE_END();
public:
  ut_KgOraSchema(void);
public:
     ~ut_KgOraSchema(void);
  
  
  void DescribeSchema();
  void DifferentOwner();
  
  
protected:  
  void DifferentOwner_Prepare();
  void JustMyTest();
  
};


