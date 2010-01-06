#pragma once


class ut_KgIdsSchema : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( ut_KgIdsSchema );
    CPPUNIT_TEST( DescribeSchema );    
    CPPUNIT_TEST( ApplySchema ); 
  CPPUNIT_TEST_SUITE_END();
public:
  ut_KgIdsSchema(void);
public:
     ~ut_KgIdsSchema(void);
  
  
  void DescribeSchema();    
  void ApplySchema();


  
};


