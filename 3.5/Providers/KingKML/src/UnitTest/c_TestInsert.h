#pragma once

class c_TestInsert : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( c_TestInsert );
  CPPUNIT_TEST( InsertExistingKML );
  CPPUNIT_TEST( InsertNewKML );  
  CPPUNIT_TEST( InsertNewKML_ApplySchema );     
  CPPUNIT_TEST_SUITE_END();
public:
  c_TestInsert(void);
public:
  ~c_TestInsert(void);
  
  void InsertExistingKML();
  void InsertNewKML();
  void InsertNewKML_ApplySchema();
  
protected:
  void InsertTest(c_KgKmlConnection* Conn);

};
