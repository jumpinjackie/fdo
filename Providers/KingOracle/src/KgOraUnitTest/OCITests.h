#ifndef OCI_TESTS_H
#define OCI_TESTS_H

#include <cppunit/extensions/HelperMacros.h>

class c_Oci_Connection;

class OCITests : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(OCITests);
  CPPUNIT_TEST(OCI_OpenConn);
  CPPUNIT_TEST(OCI_CreateTableData);
  // Don't have the bldg_polygon table (or the data behind it) as specified in the test
  //CPPUNIT_TEST(OCI_ReadGeomsSameConn);
  CPPUNIT_TEST(OCI_DescribeScema);
  // Don't have the test tables/sequences for this test
  //CPPUNIT_TEST(OCI_Test_c_Ora_Api);
  // Is mdsys.sdo_geom_metadata_table specific to Oracle Spatial and not Locator?
  //CPPUNIT_TEST(OCI_TestDefine);
  CPPUNIT_TEST(OCI_Test_c_FdoOra_API2);
  CPPUNIT_TEST(OCI_Test_Bind);
#ifdef INCLUDE_PERF_TESTS
  CPPUNIT_TEST(OCI_ReadGeoms);
  CPPUNIT_TEST(OCI_ReadGeoms2);
  CPPUNIT_TEST(OCI_TestSpeed_DescribeScema);
  CPPUNIT_TEST(OCI_Test_ColumnNameLookup);
#endif
  CPPUNIT_TEST_SUITE_END();

public:
  OCITests(void);

public:
  ~OCITests(void);

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

  static void PrepareTable2D(c_Oci_Connection *Connection, const wchar_t *TableName);
  static void PrepareTable3D(c_Oci_Connection *Connection, const wchar_t *TableName);

protected:
  c_Oci_Connection *OCI_CreateConnection();
  c_Oci_Connection *m_OciConn;
};
#endif