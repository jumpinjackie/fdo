#pragma once

class c_TestConfigXML : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( c_TestConfigXML );
  CPPUNIT_TEST( ReadConfigXML );  
  CPPUNIT_TEST_SUITE_END();

public:
  c_TestConfigXML(void);
public:
  ~c_TestConfigXML(void);
  
  void ReadConfigXML();
};
