#include "StdAfx.h"
#include "c_TestOpen.h"

CPPUNIT_TEST_SUITE_REGISTRATION(c_TestOpen);


c_TestOpen::c_TestOpen(void)
{
}

c_TestOpen::~c_TestOpen(void)
{
}

void c_TestOpen::OpenKML()
{
  try
  {
    c_Util::CreateTestKML();
    FdoPtr<FdoIConnection> conn = c_Util::OpenUnitTestConnection();
    conn->Close();
  }  
  catch(FdoException* ex)
  {
    FdoStringP str = ex->GetExceptionMessage();
    ex->Release();
    CPPUNIT_FAIL( (const char*)str );
  }
}



void c_TestOpen::DescribeSchema()
{
  try
  {
    c_Util::CreateTestKML();
    FdoPtr<FdoIConnection> conn = c_Util::OpenUnitTestConnection();
    FdoPtr<FdoIDescribeSchema> comm = (FdoIDescribeSchema*)conn->CreateCommand(FdoCommandType_DescribeSchema);
    FdoPtr<FdoFeatureSchemaCollection> schemas = comm->Execute();
    if( !schemas.p || schemas->GetCount() != 1 )
    {
      CPPUNIT_FAIL( "No schema collection or schema count is zero." );  
    }
    
    FdoPtr<FdoFeatureSchema> schema = schemas->GetItem(0);
    FdoPtr<FdoClassCollection> classes = schema->GetClasses();
    if( !classes.p || classes->GetCount() != 1 )
    {
      CPPUNIT_FAIL( "No classes collection or classes count is zero." );  
    }
    
    FdoPtr<FdoClassDefinition> fdoclass = classes->GetItem(0);
    
    conn->Close();
  }  
  catch(FdoException* ex)
  {
    FdoStringP str = ex->GetExceptionMessage();
    ex->Release();
    CPPUNIT_FAIL( (const char*)str );
  }
}


void c_TestOpen::ApplySchema()
{
  FdoPtr<FdoIConnection> conn;
  c_KgKmlConnection * kingkmlconn;
  try
  {
    c_Util::DeleteTestNewKML();
    conn = c_Util::OpenUnitTestConnection_NewKML();
    kingkmlconn = (c_KgKmlConnection*)conn.p;

    FdoPtr<FdoIApplySchema> comm = (FdoIApplySchema*)conn->CreateCommand(FdoCommandType_ApplySchema);
    FdoPtr<FdoFeatureSchema> testschema = c_Util::CreateTestFdoSchema();
    comm->SetFeatureSchema(testschema);
    comm->Execute();

    conn->Close();

    conn->Open();
    FdoPtr<FdoIDescribeSchema> comm_describe = (FdoIDescribeSchema*)conn->CreateCommand(FdoCommandType_DescribeSchema);
    FdoPtr<FdoFeatureSchemaCollection> schemas = comm_describe->Execute();
    FdoPtr<FdoFeatureSchema> schema = schemas->GetItem(0);
    
    if( !c_Util::CompareSchema(testschema,schema) )
    {
      CPPUNIT_FAIL( "c_TestOpen::ApplySchema - FDO Schemas are not same." );
    }

  }  
  catch(FdoException* ex)
  {
    FdoStringP str = ex->GetExceptionMessage();
    ex->Release();
    CPPUNIT_FAIL( (const char*)str );
  }
}

void c_TestOpen::OpenKMZ()
{
  try
  {
    
    FdoPtr<FdoIConnection> conn = c_Util::OpenUnitTestConnection_ParceleKMZ();
    
    c_KgKmlConnection * kingkmlconn = (c_KgKmlConnection*)conn.p;
    FdoPtr<c_KgKmlFdoData> kmlfdodata = kingkmlconn->GetKmlFdoData();

    FdoPtr<c_KgKmlFdoClassConfig> kmlfdocfg = kmlfdodata->GetKmlFdoClassConfig();

    FdoPtr<FdoIDescribeSchema> comm = (FdoIDescribeSchema*)conn->CreateCommand(FdoCommandType_DescribeSchema);
    FdoPtr<FdoFeatureSchemaCollection> schemas = comm->Execute();

    FdoPtr<FdoFeatureSchema> schema = schemas->GetItem(0);
    FdoPtr<FdoClassCollection> classes = schema->GetClasses();
    FdoPtr<FdoClassDefinition> classdef = classes->GetItem(0);
    // Read Data
    // select source data
    FdoPtr<FdoISelect> selcomm = (FdoISelect*)conn->CreateCommand(FdoCommandType_Select);  
    selcomm->SetFeatureClassName(classdef->GetName());

    FdoPtr<FdoIFeatureReader> freader = selcomm->Execute();
    
    int count=0;
    while(freader->ReadNext()) count++;
    
    conn->Close();
  }  
  catch(FdoException* ex)
  {
    FdoStringP str = ex->GetExceptionMessage();
    ex->Release();
    CPPUNIT_FAIL( (const char*)str );
  }
}