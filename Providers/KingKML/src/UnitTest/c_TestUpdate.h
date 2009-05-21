#pragma once


class c_TestUpdate : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( c_TestUpdate );
  CPPUNIT_TEST( UpdateKML );     
  CPPUNIT_TEST_SUITE_END();
  
public:
  c_TestUpdate(void);
public:
  ~c_TestUpdate(void);
  
  public:
    void UpdateKML();
};
