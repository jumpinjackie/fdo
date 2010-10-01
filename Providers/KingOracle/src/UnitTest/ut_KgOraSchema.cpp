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
  
}//end of ut_KgOraSchema::DifferentOwner

void Prepare_TestNamesWith_( FdoIConnection* Connection, const wchar_t*TableName )
{

  // create test table with different data types
  FdoPtr<FdoISQLCommand> sqlcomm = (FdoISQLCommand*)Connection->CreateCommand( FdoCommandType_SQLCommand );

  FdoStringP tname = TableName;
  try
  {  
    FdoStringP sql = FdoStringP::Format(L"DROP TABLE %s",tname);


    sqlcomm->SetSQLStatement(sql);
    sqlcomm->ExecuteNonQuery();
  }  
  catch(FdoException* ex)
  {
    ex->Release();
  } 
  try
  {  
    // drop sequnce
    FdoStringP sql = FdoStringP::Format(L"DROP SEQUENCE %s_FDOSEQ",tname);
    sqlcomm->SetSQLStatement(sql);
    sqlcomm->ExecuteNonQuery();
  }  
  catch(FdoException* ex  )
  {
    ex->Release();
  } 
  try
  {  
    FdoStringP sql = FdoStringP::Format(L"CREATE TABLE %s( \
                                         fid NUMBER(10,0) \
                                         ,SDATE DATE\
                                         ,SITE   VARCHAR2(254 BYTE)\
                                         ,DESC_ VARCHAR2(254 BYTE)\
                                         ,geom SDO_GEOMETRY,name VARCHAR2(100) )",tname);
    sqlcomm->SetSQLStatement(sql);
    sqlcomm->ExecuteNonQuery();
  }  
  catch(FdoException* ex)
  {
    ex->Release();
  } 
  try
  {  
    // create primary key
    FdoStringP sql = FdoStringP::Format(L"alter table %s add constraint %s_pk primary key (fid)",TableName,TableName);
    sqlcomm->SetSQLStatement(sql);
    sqlcomm->ExecuteNonQuery();
  }  
  catch(FdoException* ex)
  {
    ex->Release();
  } 
  try
  {  
    // create sequnce
    FdoStringP sql = FdoStringP::Format(L"CREATE SEQUENCE %s_FDOSEQ",tname);
    sqlcomm->SetSQLStatement(sql);
    sqlcomm->ExecuteNonQuery();
  }  
  catch(FdoException* ex)
  {
    ex->Release();
  } 
  try
  {  
    // insert user_sdo_geom_metadata
    FdoStringP sql = FdoStringP::Format(L"INSERT INTO user_SDO_GEOM_METADATA  VALUES ('%s'\
                                         , 'GEOM',SDO_DIM_ARRAY(SDO_DIM_ELEMENT('X', -1000000, 1000000, 0.005),SDO_DIM_ELEMENT('Y', -1000000, 1000000, 0.005))\
                                         ,NULL)",tname);
    sqlcomm->SetSQLStatement(sql);

    sqlcomm->ExecuteNonQuery();
  }
  catch(FdoException* ex)
  {
    ex->Release();
  } 
  try
  {  
    //-- Create index
    FdoStringP sql = FdoStringP::Format(L"INSERT INTO user_SDO_GEOM_METADATA  VALUES ('%s'\
                                         , 'GEOM',SDO_DIM_ARRAY(SDO_DIM_ELEMENT('X', -1000000, 1000000, 0.005),SDO_DIM_ELEMENT('Y', -1000000, 1000000, 0.005))\
                                         ,NULL)",tname);
    sqlcomm->SetSQLStatement(sql);

    sqlcomm->ExecuteNonQuery();
  }
  catch(FdoException* ex)
  {
    ex->Release();
  }  

}//end of Prepare_TestNamesWith_

void ut_KgOraSchema::TestNamesWith_()
{
  FdoPtr<FdoIConnection> conn;
try
{
  //conn = c_KgOraUtil::OpenUnitTestConnection_10_2();
  conn = c_KgOraUtil::OpenUnitTestConnection_10_2();
  Prepare_TestNamesWith_(conn,L"Underscore");  
  conn->Close();
}
catch(FdoException* ex)
{
  ex->Release();
  CPPUNIT_FAIL("Exception!");
}
  
conn = c_KgOraUtil::OpenUnitTestConnection_10_2();
  
try
{ 
  FdoStringP src_classname = L"UNITTEST~UNDERSCORE~GEOM";
  
  FdoPtr<FdoIDescribeSchema> comm_schema = (FdoIDescribeSchema*)conn->CreateCommand(FdoCommandType_DescribeSchema);
  FdoPtr<FdoFeatureSchemaCollection> schemas = comm_schema->Execute();
  FdoPtr<FdoIDisposableCollection> coll = schemas->FindClass(src_classname);
  FdoPtr<FdoClassDefinition> classdef = (FdoClassDefinition*)coll->GetItem(0);
  
  
  FdoPtr<FdoISelect> comm_select = (FdoISelect*)conn->CreateCommand(FdoCommandType_Select);
 
  
 
  comm_select->SetFeatureClassName(src_classname);
  FdoPtr<FdoIFeatureReader> reader = comm_select->Execute();
  reader->ReadNext();
  reader->Close();
  
  FdoPtr<FdoIInsert> comm_insert = (FdoIInsert*)conn->CreateCommand(FdoCommandType_Insert);
  comm_insert->SetFeatureClassName( src_classname );
  FdoPtr<FdoPropertyValueCollection> propcol = comm_insert->GetPropertyValues();
  FdoPtr<FdoDataValue> fid_val=  FdoDataValue::Create(3);
  FdoPtr<FdoPropertyValue> propval_fid = FdoPropertyValue::Create(L"FID",fid_val);

  // Geometry property
  FdoPtr<FdoFgfGeometryFactory> fgf = FdoFgfGeometryFactory::GetInstance();

  double ordinates[10] = { 1,1 ,10,1 ,10,10 ,1,10 ,1,1 }; 

  FdoPtr<FdoILinearRing> ring = fgf->CreateLinearRing(0,10,ordinates);
  FdoPtr<FdoIPolygon> polygon = fgf->CreatePolygon(ring,NULL);

  FdoPtr<FdoByteArray> barray = fgf->GetFgf(polygon);

  FdoPtr<FdoGeometryValue> geom_val=  FdoGeometryValue::Create(barray);

  FdoPtr<FdoPropertyValue> propval_geom = FdoPropertyValue::Create(L"GEOM",geom_val);
  
  
  // add null insert values for other properties
  FdoPtr<FdoPropertyDefinitionCollection> props = classdef->GetProperties();
  for(int ind=0; ind<props->GetCount();ind++)
  {
    FdoPtr<FdoPropertyDefinition> propdef = props->GetItem(ind);
    if( propdef->GetPropertyType() == FdoPropertyType_DataProperty)
    {
      FdoDataPropertyDefinition* dataprop = (FdoDataPropertyDefinition*)propdef.p;
      FdoPtr<FdoDataValue> ins_val=  FdoDataValue::Create(dataprop->GetDataType());
      FdoPtr<FdoPropertyValue> ins_propval = FdoPropertyValue::Create(dataprop->GetName(),ins_val);
      propcol->Add(ins_propval);      
    }
  }
  
  
  
  
  propcol->Add(propval_geom);
  FdoPtr<FdoIFeatureReader> insreader = comm_insert->Execute();
  
  conn->Close();
  
}
catch(FdoException* ex)
{
  ex->Release();
  CPPUNIT_FAIL("Exception!");
}  
}