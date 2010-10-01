#pragma once


class ut_KgOraSchema : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( ut_KgOraSchema );
    CPPUNIT_TEST( DescribeSchema );    
    CPPUNIT_TEST( DifferentOwner );    
    CPPUNIT_TEST( TestNamesWith_ ); 
  CPPUNIT_TEST_SUITE_END();
public:
  ut_KgOraSchema(void);
public:
     ~ut_KgOraSchema(void);
  
  
  void DescribeSchema();
  void DifferentOwner();
  void TestNamesWith_();
  
protected:  
  void DifferentOwner_Prepare();
  

  
};


