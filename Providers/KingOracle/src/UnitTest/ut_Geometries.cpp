#include "StdAfx.h"
#include "ut_Geometries.h"

#include "c_OCI_API.h"
#include "ut_OCI.h"

// Provider specific headers
#include <KingOracle/FdoKgOraOverrides.h>
#include "KgOraProvider.h"
#include "../Message/inc/KgOraMessage.h"
#include <c_KgOraSridDesc.h>
#include <c_KgOraConnectionCapabilities.h>
#include <c_KgOraCommandCapabilities.h>
#include <c_KgOraExpressionCapabilities.h>
#include <c_KgOraFilterCapabilities.h>
#include <c_KgOraSchemaCapabilities.h>
#include <c_KgOraRasterCapabilities.h>
#include <c_KgOraTopologyCapabilities.h>
#include <c_KgOraGeometryCapabilities.h>
#include <c_KgOraConnection.h>
#include <c_KgOraConnectionInfo.h>
#include <c_KgOraFdoFeatureCommand.h>
#include <c_KgOraSchemaDesc.h>
#include <c_KgOraDescribeSchemaCommand.h>

CPPUNIT_TEST_SUITE_REGISTRATION(ut_Geometries);

#define D_CONN_USERNAME L"unittest"
#define D_CONN_PASS L"unittest"
#define D_CONN_SERVICE  L"//test10gxe/xe"

ut_Geometries::ut_Geometries(void)
{
}

ut_Geometries::~ut_Geometries(void)
{
}


void ut_Geometries::GeometryTypes(void)
{
}

void ut_Geometries::TestCurvedSegment()
{
try
{
  FdoStringP tablename = L"TESTCURVED";
  
  FdoStringP fdoclass_name = L"UNITTEST~TESTCURVED~GEOM";
  
  
 try
 {
 
  
  
  c_OCI_API::OciInit();
  c_Oci_Connection* ociconn = c_OCI_API::CreateConnection(D_CONN_USERNAME,D_CONN_PASS,D_CONN_SERVICE);
  ut_OCI::PrepareTable2D(ociconn,tablename);
  
  c_Oci_Statement* stm = ociconn->CreateStatement();
  FdoStringP sql_insert;
  
  sql_insert = L"INSERT INTO TESTCURVED(fid,geom) VALUES(1,MDSYS.SDO_GEOMETRY(2003,null,MDSYS.SDO_POINT_TYPE(null,null,null),MDSYS.SDO_ELEM_INFO_ARRAY(1,1005,3,1,2,1,11,2,2,15,2,1,23,2003,1),MDSYS.SDO_ORDINATE_ARRAY(431725.240757142,5446007.93990696,431775.775570248,5445990.17270006,431785.643603283,5446018.24009454,431740.280539093,5446034.18899981,431737.752025556,5446026.99721329,431721.862176078,5446032.5838126,431721.556338345,5446032.43079432,431721.251081057,5446032.27662136,431715.544405141,5446016.04528306,431726.710252552,5446012.11955525,431725.240757142,5446007.93990696,431727.510253872,5446018.02973043,431726.086231102,5446018.53039336,431727.001350085,5446021.13324292,431728.425372854,5446020.63258,431727.510253872,5446018.02973043)))";
  stm->Prepare(sql_insert);
  stm->ExecuteNonQuery();
  
  sql_insert = L"INSERT INTO TESTCURVED(fid,geom) VALUES(2,mdsys.sdo_geometry(2003,null,null,mdsys.sdo_elem_info_array(1,1005,4,1,2,1,7,2,2,11,2,1,25,2,2),mdsys.sdo_ordinate_array(-456.82452997,-293.42371003,-473.03587588,-277.7575737,-501.33016493,-307.03519122,-485.2705,-322.59751,-432.16524829259,-399.429363550798,-411.98386,-490.62154,-411.18518423,-535.45441589,-382.07041476,-534.93320052,-382.07184615,-534.85324374,-370.84965631,-534.66607206,-370.85157461,-534.55608879,-370.42164,-534.54859,-371.2235978,-489.63093805,-394.816561353499,-383.147509810187,-456.82452997,-293.42371003)))";
  stm->Prepare(sql_insert);
  stm->ExecuteNonQuery();
  
  
  
  
  ociconn->TerminateStatement(stm);
  c_OCI_API::CloseConnection(ociconn);
 } 
 catch (c_Oci_Exception* ex)
 {
  FdoStringP str = ex->GetErrorText();
  delete ex;
  CPPUNIT_FAIL( (const char*)str );  
 } 
  
  
  
  FdoPtr<FdoIConnection> conn  = c_KgOraUtil::OpenUnitTestConnection_10_2();
  c_KgOraConnection* kingora_conn = (c_KgOraConnection*)conn.p;
  kingora_conn->ClearCachedSchemaDesc();
  
  FdoPtr<FdoIDescribeSchema> comm_fdoschema = (FdoIDescribeSchema*)conn->CreateCommand(FdoCommandType_DescribeSchema);
  FdoPtr<FdoFeatureSchemaCollection> coll_schema = comm_fdoschema->Execute();
  
  FdoPtr<FdoIDisposableCollection> class_col = coll_schema->FindClass(fdoclass_name);
  FdoPtr<FdoClassDefinition> classdef;
  if( class_col->GetCount() > 0 ) classdef = (FdoClassDefinition*)class_col->GetItem(0);
  
  CPPUNIT_ASSERT( classdef.p );
  
  FdoPtr<FdoISelect> comm_select = (FdoISelect*)conn->CreateCommand(FdoCommandType_Select);
  comm_select->SetFeatureClassName(fdoclass_name);
  //comm_select->SetFilter(L"fid=1");
  
  FdoPtr<FdoIFeatureReader> reader = comm_select->Execute();
  if( reader->ReadNext() )
  {
    do 
    {
      FdoPtr<FdoByteArray> fgf = reader->GetGeometry(L"geom");
      FdoPtr<FdoGeometryValue> geomval = FdoGeometryValue::Create( fgf );
      FdoString *strval=geomval->ToString();
    } while (reader->ReadNext());
    
    
    
    
  }
  else
  {
    CPPUNIT_FAIL( "Empty Reader" );
  }
  
  
  reader->Close();
  
  
  conn->Close();
}
catch(FdoException* ex)
{
  FdoStringP str = ex->GetExceptionMessage();
  ex->Release();
  CPPUNIT_FAIL( (const char*)str );
}

}//end of ut_Geometries::TestCurvedSegment

void ut_Geometries::TestPolygons3D()
{
try
{
  FdoStringP tablename = L"TESTPOLYGONS";
  
  FdoStringP fdoclass_name = L"UNITTEST~TESTPOLYGONS~GEOM";
  
  
 try
 {
 
  c_OCI_API::OciInit();
  c_Oci_Connection* ociconn = c_OCI_API::CreateConnection(D_CONN_USERNAME,D_CONN_PASS,D_CONN_SERVICE);
  ut_OCI::PrepareTable3D(ociconn,tablename);
  
  c_Oci_Statement* stm = ociconn->CreateStatement();
  FdoStringP sql_insert;
  
  sql_insert = L"INSERT INTO TESTPOLYGONS(fid,geom) VALUES(2,SDO_GEOMETRY(3007, NULL, NULL,SDO_ELEM_INFO_ARRAY(1, 2003, 1, 16, 1003, 1),SDO_ORDINATE_ARRAY(297700, 137090, 0,297700, 137110, 0, 297720, 137110, 0, 297720,137090, 0, 297700, 137090, 0, 297690, 137080, 0,297730, 137080, 0, 297730, 137120, 0, 297690,137120, 0, 297690, 137080, 0)))";
  stm->Prepare(sql_insert);
  stm->ExecuteNonQuery();
  
  sql_insert = L"INSERT INTO TESTPOLYGONS(fid,geom) VALUES(3,SDO_GEOMETRY(3003, NULL, NULL,SDO_ELEM_INFO_ARRAY(1, 2003, 1, 16, 1003, 1),SDO_ORDINATE_ARRAY(297700, 137090, 0,297700, 137110, 0, 297720, 137110, 0, 297720,137090, 0, 297700, 137090, 0, 297690, 137080, 0,297730, 137080, 0, 297730, 137120, 0, 297690,137120, 0, 297690, 137080, 0)))";
  stm->Prepare(sql_insert);
  stm->ExecuteNonQuery();
  
  
  ociconn->TerminateStatement(stm);
  c_OCI_API::CloseConnection(ociconn);
 } 
 catch (c_Oci_Exception* ex)
 {
  FdoStringP str = ex->GetErrorText();
  delete ex;
  CPPUNIT_FAIL( (const char*)str );  
 } 
  
  
  
  FdoPtr<FdoIConnection> conn  = c_KgOraUtil::OpenUnitTestConnection_10_2();
  c_KgOraConnection* kingora_conn = (c_KgOraConnection*)conn.p;
  kingora_conn->ClearCachedSchemaDesc();
  
  FdoPtr<FdoIDescribeSchema> comm_fdoschema = (FdoIDescribeSchema*)conn->CreateCommand(FdoCommandType_DescribeSchema);
  FdoPtr<FdoFeatureSchemaCollection> coll_schema = comm_fdoschema->Execute();
  
  FdoPtr<FdoIDisposableCollection> class_col = coll_schema->FindClass(fdoclass_name);
  FdoPtr<FdoClassDefinition> classdef;
  if( class_col->GetCount() > 0 ) classdef = (FdoClassDefinition*)class_col->GetItem(0);
  
  CPPUNIT_ASSERT( classdef.p );
  
  FdoPtr<FdoISelect> comm_select = (FdoISelect*)conn->CreateCommand(FdoCommandType_Select);
  comm_select->SetFeatureClassName(fdoclass_name);
  //comm_select->SetFilter(L"fid=1");
  
  FdoPtr<FdoIFeatureReader> reader = comm_select->Execute();
  if( reader->ReadNext() )
  {
    do 
    {
      FdoPtr<FdoByteArray> fgf = reader->GetGeometry(L"geom");
      FdoPtr<FdoGeometryValue> geomval = FdoGeometryValue::Create( fgf );
      FdoString *strval=geomval->ToString();
    } while (reader->ReadNext());
    
    
    
    
  }
  else
  {
    CPPUNIT_FAIL( "Empty Reader" );
  }
  
  
  reader->Close();
  
  
  conn->Close();
}
catch(FdoException* ex)
{
  FdoStringP str = ex->GetExceptionMessage();
  ex->Release();
  CPPUNIT_FAIL( (const char*)str );
}

}//end of ut_Geometries::TestPolygons3D

void ut_Geometries::TestHasElevation()
{
try
{
  FdoStringP tablename = L"TESTLINESTRING";
  
  FdoStringP fdoclass_name = L"UNITTEST~TESTLINESTRING~GEOM";
  bool elevation = false;
  
 try
 {
 
  c_OCI_API::OciInit();
  c_Oci_Connection* ociconn = c_OCI_API::CreateConnection(D_CONN_USERNAME,D_CONN_PASS,D_CONN_SERVICE);
  ut_OCI::PrepareTable3D(ociconn,tablename);
  
  c_Oci_Statement* stm = ociconn->CreateStatement();
  FdoStringP sql_insert;
  
  sql_insert = L"INSERT INTO TESTLINESTRING(fid,geom) VALUES(2,MDSYS.SDO_GEOMETRY(3002,null,null,MDSYS.SDO_ELEM_INFO_ARRAY(1,2,1),MDSYS.SDO_ORDINATE_ARRAY(10,500,10,70,10,400)))";
  stm->Prepare(sql_insert);
  stm->ExecuteNonQuery();
  
  sql_insert = L"INSERT INTO TESTLINESTRING(fid,geom) VALUES(3,MDSYS.SDO_GEOMETRY(3002,null,null,MDSYS.SDO_ELEM_INFO_ARRAY(1,2,1),MDSYS.SDO_ORDINATE_ARRAY(5,10,1,800,900,100)))";
  stm->Prepare(sql_insert);
  stm->ExecuteNonQuery();
  
  
  ociconn->TerminateStatement(stm);
  c_OCI_API::CloseConnection(ociconn);
 } 
 catch (c_Oci_Exception* ex)
 {
  FdoStringP str = ex->GetErrorText();
  delete ex;
  CPPUNIT_FAIL( (const char*)str );  
 } 
  
  
  
  FdoPtr<FdoIConnection> conn  = c_KgOraUtil::OpenUnitTestConnection_10_2();
  c_KgOraConnection* kingora_conn = (c_KgOraConnection*)conn.p;
  kingora_conn->ClearCachedSchemaDesc();
  
  FdoPtr<FdoIDescribeSchema> comm_fdoschema = (FdoIDescribeSchema*)conn->CreateCommand(FdoCommandType_DescribeSchema);
  FdoPtr<FdoFeatureSchemaCollection> coll_schema = comm_fdoschema->Execute();
  
  FdoPtr<FdoIDisposableCollection> class_col = coll_schema->FindClass(fdoclass_name);
  FdoPtr<FdoClassDefinition> classdef;
  if( class_col->GetCount() > 0 ) classdef = (FdoClassDefinition*)class_col->GetItem(0);
  
  CPPUNIT_ASSERT( classdef.p );
  
  FdoPtr<FdoPropertyDefinitionCollection> props = classdef->GetProperties ();
  FdoPtr<FdoPropertyDefinition> prop = props->FindItem (L"GEOM");
  FdoGeometricPropertyDefinition* dp = (FdoGeometricPropertyDefinition*)prop.p;

  if(dp->GetHasElevation())
  {
    elevation = true;
  }
  else
  {
    CPPUNIT_FAIL( "No Elevation");
  }
 /* FdoPtr<FdoISelect> comm_select = (FdoISelect*)conn->CreateCommand(FdoCommandType_Select);
  comm_select->SetFeatureClassName(fdoclass_name);
  //comm_select->SetFilter(L"fid=1");
  
  FdoPtr<FdoIFeatureReader> reader = comm_select->Execute();
  if( reader->ReadNext() )
  {
    do 
    {
      FdoPtr<FdoByteArray> fgf = reader->GetGeometry(L"GEOM");
      FdoPtr<FdoGeometryValue> geomval = FdoGeometryValue::Create( fgf );
      FdoString *strval=geomval->ToString();
    } while (reader->ReadNext());
    
  
    
    
  }
  else
  {
    CPPUNIT_FAIL( "Empty Reader" );
  }
  
  
  reader->Close();
  
  */  
  conn->Close();
}
catch(FdoException* ex)
{
  FdoStringP str = ex->GetExceptionMessage();
  ex->Release();
  CPPUNIT_FAIL( (const char*)str );
}

}//end of ut_Geometries::TestHasElevation


void ut_Geometries::TestGeometryCollection()
{
try
{
  FdoStringP tablename = L"TESTGEOMCOLLECTION";
  
  FdoStringP fdoclass_name = L"UNITTEST~TESTGEOMCOLLECTION~GEOM";
  bool elevation = false;
  
 try
 {
 
  c_OCI_API::OciInit();
  c_Oci_Connection* ociconn = c_OCI_API::CreateConnection(D_CONN_USERNAME,D_CONN_PASS,D_CONN_SERVICE);
  ut_OCI::PrepareTable2D(ociconn,tablename);
  
  c_Oci_Statement* stm = ociconn->CreateStatement();
  FdoStringP sql_insert;
  
  sql_insert = L"INSERT INTO TESTGEOMCOLLECTION(fid,geom) VALUES(2,MDSYS.SDO_GEOMETRY(2004,null,null,MDSYS.SDO_ELEM_INFO_ARRAY(1,2,1,5,1003,1),MDSYS.SDO_ORDINATE_ARRAY(893387.1279764,530499.1798361,893387.2056353,530499.1405861,893371.6056161,530536.2878878,893359.8797596,530513.1144291,893381.936952,530501.8403549,893386.3799577,530510.5328853,893391.6481135,530507.8401773,893387.2056353,530499.1405861,893388.317921,530498.5784176,893418.2134005,530483.2911693,893427.7218373,530501.8030188,893423.6081282,530503.9056561,893426.9076444,530510.3610015,893389.362765,530529.5512911,893388.414628,530527.6963068,893371.6056161,530536.2878878)))";
  stm->Prepare(sql_insert);
  stm->ExecuteNonQuery();

  ociconn->TerminateStatement(stm);
  c_OCI_API::CloseConnection(ociconn);
 } 
 catch (c_Oci_Exception* ex)
 {
  FdoStringP str = ex->GetErrorText();
  delete ex;
  CPPUNIT_FAIL( (const char*)str );  
 } 
  
  
  
  FdoPtr<FdoIConnection> conn  = c_KgOraUtil::OpenUnitTestConnection_10_2();
  c_KgOraConnection* kingora_conn = (c_KgOraConnection*)conn.p;
  kingora_conn->ClearCachedSchemaDesc();
  
  FdoPtr<FdoIDescribeSchema> comm_fdoschema = (FdoIDescribeSchema*)conn->CreateCommand(FdoCommandType_DescribeSchema);
  FdoPtr<FdoFeatureSchemaCollection> coll_schema = comm_fdoschema->Execute();
  
  FdoPtr<FdoISelect> comm_select = (FdoISelect*)conn->CreateCommand(FdoCommandType_Select);
  comm_select->SetFeatureClassName(fdoclass_name);
  //comm_select->SetFilter(L"fid=1");
  
  FdoPtr<FdoIFeatureReader> reader = comm_select->Execute();
  if( reader->ReadNext() )
  {
    do 
    {
      FdoPtr<FdoByteArray> fgf = reader->GetGeometry(L"GEOM");
      FdoPtr<FdoGeometryValue> geomval = FdoGeometryValue::Create( fgf );
      FdoString *strval=geomval->ToString();
    } while (reader->ReadNext());
    
  
    
    
  }
  else
  {
    CPPUNIT_FAIL( "Empty Reader" );
  }
  
  
  reader->Close();
  
  
  conn->Close();
}
catch(FdoException* ex)
{
  FdoStringP str = ex->GetExceptionMessage();
  ex->Release();
  CPPUNIT_FAIL( (const char*)str );
}

}//end of ut_Geometries::TestGeometryCollection