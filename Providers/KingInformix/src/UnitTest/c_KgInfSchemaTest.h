#pragma once


class c_KgInfSchemaTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( c_KgInfSchemaTest );
    CPPUNIT_TEST( ApplySchema );    
  CPPUNIT_TEST_SUITE_END();
public:
  c_KgInfSchemaTest(void);
public:
     ~c_KgInfSchemaTest(void);
  
  
  void ApplySchema();    
  

  
};


