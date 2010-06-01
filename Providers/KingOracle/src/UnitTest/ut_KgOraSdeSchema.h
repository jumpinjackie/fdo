#pragma once


class ut_KgOraSdeSchema : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( ut_KgOraSdeSchema );
    CPPUNIT_TEST( DescribeSdeSchema );    
    CPPUNIT_TEST( SelectCommand );    
    CPPUNIT_TEST( SelectAggregate );
    CPPUNIT_TEST( TestPolygonWithHoles );
    CPPUNIT_TEST( ReadAllClassesGeomtries );    
    CPPUNIT_TEST( SelectCommand_RemoteSDE );
    CPPUNIT_TEST( SdeBinaryToFGF );
    
  CPPUNIT_TEST_SUITE_END();
public:
  ut_KgOraSdeSchema(void);
public:
     ~ut_KgOraSdeSchema(void);
  
  
  void DescribeSdeSchema();    
  
  void SelectCommand();    
  void ReadAllClassesGeomtries();
  void SelectCommand_RemoteSDE();
  void SelectAggregate();


  void TestPolygonWithHoles();
  void SdeBinaryToFGF();
protected:  
  void ReadAllProperties(FdoIFeatureReader* Reader);
  void ReadAllProperties(FdoIDataReader* Reader);
  void ReadDataProperty(FdoIReader* Reader,FdoDataType DataType,FdoString* Name);
  
  
};


