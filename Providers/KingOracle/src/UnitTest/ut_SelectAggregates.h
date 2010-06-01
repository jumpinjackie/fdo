#pragma once



class ut_SelectAggregates : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( ut_SelectAggregates );
    CPPUNIT_TEST( SelectCount );    
    CPPUNIT_TEST( SelectDistinct );  
  CPPUNIT_TEST_SUITE_END();
  
public:
  ut_SelectAggregates(void);
public:
  ~ut_SelectAggregates(void);
  
  void SelectCount();   
  
  void SelectDistinct();   
  
protected:  
  void SelectDistinct_Prepare();
  void SelectDistinct_Prepare_Insert(FdoIConnection* Conn,int Fid,wchar_t*Name,FdoGeometryValue* Geom);
};
