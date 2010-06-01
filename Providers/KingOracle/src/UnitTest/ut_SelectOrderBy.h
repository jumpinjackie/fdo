#pragma once

class ut_SelectOrderBy : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( ut_SelectOrderBy );
    CPPUNIT_TEST( SelectOrderBy );    
    CPPUNIT_TEST( SelectMultiPoint );  
  CPPUNIT_TEST_SUITE_END();
  
public:
  ut_SelectOrderBy(void);
public:
  ~ut_SelectOrderBy(void);

  void SelectOrderBy();
  void SelectMultiPoint();
  
protected:
  void InsertRow(FdoIConnection* Conn,FdoString* ClassName,double* Ordinates,long OrdinatesCount,long Id,FdoString* Text);  
};
