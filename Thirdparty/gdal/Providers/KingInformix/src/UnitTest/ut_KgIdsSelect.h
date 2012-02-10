#pragma once

class ut_KgIdsSelect : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( ut_KgIdsSelect );
    CPPUNIT_TEST( SelectOrderBy );    
    CPPUNIT_TEST( SelectEnvelope );
    //CPPUNIT_TEST( SelectQueryAllSpatialFilters );    
    CPPUNIT_TEST( SelectQuerySpatialCross );
    CPPUNIT_TEST( SelectQuerySpatialOverlaps );
    CPPUNIT_TEST( SelectQuerySpatialInside );
  CPPUNIT_TEST_SUITE_END();
  
public:
  ut_KgIdsSelect(void);
public:
  ~ut_KgIdsSelect(void);

  void SelectOrderBy();
  void SelectEnvelope();
  void SelectQueryAllSpatialFilters();
  void SelectQuerySpatialCross();
  void SelectQuerySpatialOverlaps();
  void SelectQuerySpatialInside();
  
protected:
  void InsertRow(FdoIConnection* Conn,FdoString* ClassName,double* Ordinates,long OrdinatesCount,long Id,FdoString* Text);  
};
