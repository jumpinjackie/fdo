#pragma once


class ut_OCI : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( ut_OCI );
    CPPUNIT_TEST( OCI_OpenConn );    
    CPPUNIT_TEST( OCI_CreateTableData );    
    CPPUNIT_TEST( OCI_ReadGeoms );  
    CPPUNIT_TEST( OCI_ReadGeoms2 );  
    CPPUNIT_TEST( OCI_ReadGeomsSameConn );  
    CPPUNIT_TEST( OCI_DescribeScema );  
    CPPUNIT_TEST( OCI_TestSpeed_DescribeScema );  
    CPPUNIT_TEST( OCI_Test_c_Ora_Api ); 
    CPPUNIT_TEST( OCI_TestDefine ); 
    CPPUNIT_TEST( OCI_Test_c_FdoOra_API2 ); 
    CPPUNIT_TEST( OCI_Test_Bind ); 
    
    CPPUNIT_TEST( OCI_Test_ColumnNameLookup ); 
  CPPUNIT_TEST_SUITE_END();
public:
  ut_OCI(void);
public:
     ~ut_OCI(void);
  
  virtual void setUp();
  virtual void tearDown();
  
  void OCI_OpenConn();    
  void OCI_ReadGeoms();  
  void OCI_ReadGeoms2();
  void OCI_ReadGeomsSameConn();
  
  void OCI_CreateTableData();  
  
  void OCI_DescribeScema();  
  
  void OCI_Test_c_Ora_Api(); 
  
  void OCI_TestDefine(); 
  
  void OCI_Test_c_FdoOra_API2();
  
  void OCI_Test_Bind();
  
  void OCI_Test_ColumnNameLookup();
  
  void OCI_TestSpeed_DescribeScema();

  static void PrepareTable2D(c_Oci_Connection* Connection,const wchar_t*TableName);
  static void PrepareTable3D(c_Oci_Connection* Connection,const wchar_t*TableName);
  
protected:  
  
  c_Oci_Connection* OCI_CreateConnection();
  

  c_Oci_Connection* m_OciConn;
  
  
};


