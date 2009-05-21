#pragma once

class c_TestOpen : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( c_TestOpen );
  CPPUNIT_TEST( OpenKML );
  CPPUNIT_TEST( OpenKMZ );
  CPPUNIT_TEST( DescribeSchema );
  CPPUNIT_TEST( ApplySchema );     
  CPPUNIT_TEST_SUITE_END();
public:
  c_TestOpen(void );
public:
  ~c_TestOpen(void);
  
  void OpenKML();
  void OpenKMZ();
  void DescribeSchema();
  void ApplySchema();
};
  