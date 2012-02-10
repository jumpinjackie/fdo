#pragma once

class ut_InsertUpdateDelete : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( ut_InsertUpdateDelete );
    CPPUNIT_TEST( InsertUpdateDelete );    
    CPPUNIT_TEST( InsertUnicode );    
    
  CPPUNIT_TEST_SUITE_END();
  
public:
  ut_InsertUpdateDelete(void);
public:
  ~ut_InsertUpdateDelete(void);
  
  
  void InsertUpdateDelete();
  
  void InsertUnicode();
  
protected:
  void PrepareTables(FdoIConnection* Connection);  
};
