#include "StdAfx.h"

#include "c_KgOraUtil.h"
#include "ut_KgOraSchema.h"
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



CPPUNIT_TEST_SUITE_REGISTRATION(ut_KgOraSchema);



ut_KgOraSchema::ut_KgOraSchema(void)
{
}

ut_KgOraSchema::~ut_KgOraSchema(void)
{
}




void ut_KgOraSchema::DescribeSchema()
{
try
{
  FdoPtr<FdoIConnection> conn = c_KgOraUtil::OpenUnitTestConnection_10_2();
  
  c_KgOraConnection* kingora_conn = (c_KgOraConnection*)conn.p;
  kingora_conn->ClearCachedSchemaDesc();
  
  FdoPtr<FdoIDescribeSchema> comm = (FdoIDescribeSchema*)conn->CreateCommand(FdoCommandType_DescribeSchema);

  
  FdoPtr<FdoFeatureSchemaCollection> schemas = comm->Execute();
  
  CPPUNIT_ASSERT_MESSAGE( "FdoIDescribeSchema:Execute returns NULL schema collection" , schemas );
    

  long count_schema =schemas->GetCount();
  
  FdoPtr<FdoFeatureSchema> schema = schemas->GetItem(0);
  
  FdoPtr<FdoClassCollection> coll_class = schema->GetClasses();
  long count_classes = coll_class->GetCount();
  
  conn->Close();
  
  
  //CPPUNIT_ASSERT_MESSAGE( "FdoIDescribeSchema:Execute returns 0 spatial contexts " , count );
}
catch(FdoException* ex)
{
  FdoStringP str = ex->GetExceptionMessage();
  ex->Release();
  CPPUNIT_FAIL( (const char*)str );
}
  
}//end of ut_KgOraSchema::DescribeSchema



void ut_KgOraSchema::DifferentOwner_Prepare()
{
  FdoPtr<FdoIConnection> conn = c_KgOraUtil::OpentConnection(L"Username=unittest;Password=unittest;Service=//test10gxe/xe"); 
   
  FdoPtr<FdoISQLCommand> sqlcomm = (FdoISQLCommand*)conn->CreateCommand( FdoCommandType_SQLCommand );
 
try
{  
  c_KgOraUtil::RecreateTable(conn,L"Test1");
  
  sqlcomm->SetSQLStatement(L"CREATE USER ut_Owner IDENTIFIED BY ut_Owner");
  sqlcomm->ExecuteNonQuery();
}  
catch(FdoException* ex)
{
  FdoStringP str = ex->GetExceptionMessage();
  ex->Release();
} 
try
{

  
  sqlcomm->SetSQLStatement(L"GRANT connect,resource TO ut_Owner");
  sqlcomm->ExecuteNonQuery();
  
  sqlcomm->SetSQLStatement(L"GRANT select ON unittest.Test1  TO ut_Owner");    
  sqlcomm->ExecuteNonQuery();
  
  conn->Close();
 
}  
catch(FdoException* ex)
{
  FdoStringP str = ex->GetExceptionMessage();
  ex->Release();
}  

}//end of ut_KgOraSchema::DifferentOwner_Prepare

void ut_KgOraSchema::DifferentOwner()
{
try
{
  DifferentOwner_Prepare();
  
  //FdoPtr<FdoIConnection> conn = c_KgOraUtil::OpentConnection(L"Username=ut_Owner;Password=ut_Owner;Service=//test10gxe/xe;OracleSchema=unittest"); 
  FdoPtr<FdoIConnection> conn = c_KgOraUtil::OpentConnection(L"Username=ut_Owner;Password=ut_Owner;Service=//test10gxe/xe"); 

  FdoPtr<FdoIDescribeSchema> comm = (FdoIDescribeSchema*)conn->CreateCommand(FdoCommandType_DescribeSchema);
  FdoPtr<FdoFeatureSchemaCollection> schemas = comm->Execute();
  
  FdoPtr<FdoFeatureSchema> schema = schemas->GetItem(0);

  FdoPtr<FdoClassCollection> coll_class = schema->GetClasses();
  long count_classes = coll_class->GetCount();
    
  if( count_classes != 1)
  {
    conn->Close();
    CPPUNIT_FAIL("Should be one class!");
  }
  
  FdoPtr<FdoClassDefinition> ut_class = coll_class->GetItem(0);
  
  FdoPtr<FdoPropertyDefinitionCollection> prop_coll = ut_class->GetProperties();
  if( prop_coll->GetCount() != 3)
  {
    conn->Close();
    CPPUNIT_FAIL("Should be three properties!");
  }
  
  FdoPtr<FdoDataPropertyDefinitionCollection> ident_coll = ut_class->GetIdentityProperties();
  if( ident_coll->GetCount() != 1)
  {
    conn->Close();
    CPPUNIT_FAIL("Should be one identity property!");
  }
  
  conn->Close();
}  
catch(FdoException* ex)
{
  FdoStringP str = ex->GetExceptionMessage();
  ex->Release();
}   
  
  
try
{
  DifferentOwner_Prepare();

  //FdoPtr<FdoIConnection> conn = c_KgOraUtil::OpentConnection(L"Username=ut_Owner;Password=ut_Owner;Service=//test10gxe/xe;OracleSchema=unittest"); 
  FdoPtr<FdoIConnection> conn = c_KgOraUtil::OpentConnection(L"Username=ut_Owner;Password=ut_Owner;Service=//test10gxe/xe;OracleSchema=unittest"); 

  FdoPtr<FdoIDescribeSchema> comm = (FdoIDescribeSchema*)conn->CreateCommand(FdoCommandType_DescribeSchema);
  FdoPtr<FdoFeatureSchemaCollection> schemas = comm->Execute();

  FdoPtr<FdoFeatureSchema> schema = schemas->GetItem(0);

  FdoPtr<FdoClassCollection> coll_class = schema->GetClasses();
  long count_classes = coll_class->GetCount();

  if( count_classes != 1)
  {
    conn->Close();
    CPPUNIT_FAIL("Should be one class!");
  }

  FdoPtr<FdoClassDefinition> ut_class = coll_class->GetItem(0);

  FdoPtr<FdoPropertyDefinitionCollection> prop_coll = ut_class->GetProperties();
  if( prop_coll->GetCount() != 3)
  {
    conn->Close();
    CPPUNIT_FAIL("Should be three properties!");
  }

  FdoPtr<FdoDataPropertyDefinitionCollection> ident_coll = ut_class->GetIdentityProperties();
  if( ident_coll->GetCount() != 1)
  {
    conn->Close();
    CPPUNIT_FAIL("Should be one identity property!");
  }

  conn->Close();
}  
catch(FdoException* ex)
{
  FdoStringP str = ex->GetExceptionMessage();
  ex->Release();
}   
  
}