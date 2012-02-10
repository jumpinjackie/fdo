#include "StdAfx.h"
#include "c_TestInsert.h"


CPPUNIT_TEST_SUITE_REGISTRATION(c_TestInsert);


c_TestInsert::c_TestInsert(void)
{
}

c_TestInsert::~c_TestInsert(void)
{
}



void c_TestInsert::InsertNewKML()
{
    FdoPtr<FdoIConnection> conn;
    c_KgKmlConnection * kingkmlconn;
  try
  {
    c_Util::DeleteTestNewKML();
    conn = c_Util::OpenUnitTestConnection_NewKML();
    kingkmlconn = (c_KgKmlConnection*)conn.p;

  }  
  catch(FdoException* ex)
  {
    FdoStringP str = ex->GetExceptionMessage();
    ex->Release();
    CPPUNIT_FAIL( (const char*)str );
  }

  InsertTest(kingkmlconn);

  try
  {
    kingkmlconn->Close();

  }  
  catch(FdoException* ex)
  {
    FdoStringP str = ex->GetExceptionMessage();
    ex->Release();
    CPPUNIT_FAIL( (const char*)str );
  }
}

void c_TestInsert::InsertExistingKML()
{
   FdoPtr<FdoIConnection> conn;
  c_KgKmlConnection * kingkmlconn;
  try
  {
    c_Util::CreateTestKML();
    conn = c_Util::OpenUnitTestConnection();
    kingkmlconn = (c_KgKmlConnection*)conn.p;
    
  }  
  catch(FdoException* ex)
  {
    FdoStringP str = ex->GetExceptionMessage();
    ex->Release();
    CPPUNIT_FAIL( (const char*)str );
  }
  
  InsertTest(kingkmlconn);
  
  try
  {
    kingkmlconn->Close();

  }  
  catch(FdoException* ex)
  {
    FdoStringP str = ex->GetExceptionMessage();
    ex->Release();
    CPPUNIT_FAIL( (const char*)str );
  }
}

void c_TestInsert::InsertNewKML_ApplySchema()
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
    
    

  }  
  catch(FdoException* ex)
  {
    FdoStringP str = ex->GetExceptionMessage();
    ex->Release();
    CPPUNIT_FAIL( (const char*)str );
  }

  InsertTest(kingkmlconn);

  try
  {
    kingkmlconn->Close();

  }  
  catch(FdoException* ex)
  {
    FdoStringP str = ex->GetExceptionMessage();
    ex->Release();
    CPPUNIT_FAIL( (const char*)str );
  }
}//end of c_TestInsert::InsertNewKML_ApplySchema

void c_TestInsert::InsertTest(c_KgKmlConnection* Conn)
{
  try
  {
    FdoPtr<c_KgKmlFdoData> kmlfdodata = Conn->GetKmlFdoData();

    FdoPtr<c_KgKmlFdoClassConfig> kmlfdocfg = kmlfdodata->GetKmlFdoClassConfig();

    FdoPtr<FdoIDescribeSchema> comm = (FdoIDescribeSchema*)Conn->CreateCommand(FdoCommandType_DescribeSchema);
    FdoPtr<FdoFeatureSchemaCollection> schemas = comm->Execute();

    FdoPtr<FdoFeatureSchema> schema = schemas->GetItem(0);
    FdoPtr<FdoClassCollection> classes = schema->GetClasses();
    FdoPtr<FdoClassDefinition> classdef = classes->GetItem(0);
    // Read Data
    // select source data
    FdoPtr<FdoISelect> selcomm = (FdoISelect*)Conn->CreateCommand(FdoCommandType_Select);  
    selcomm->SetFeatureClassName(classdef->GetName());
    FdoPtr<FdoIFeatureReader> freader = selcomm->Execute();

    int count=0;
    while(freader->ReadNext()) count++;

    freader->Close();

    FdoPtr<FdoPropertyDefinitionCollection> propdef_col = classdef->GetProperties();

    FdoPtr<FdoIInsert> insertcomm = (FdoIInsert*)Conn->CreateCommand(FdoCommandType_Insert);
    insertcomm->SetFeatureClassName(classdef->GetName());
    FdoPtr<FdoPropertyValueCollection> propvalue_coll = insertcomm->GetPropertyValues();

    FdoPtr<FdoPropertyValue> propval = FdoPropertyValue::Create();
    FdoPtr<FdoDataValue> value = FdoDataValue::Create(L"Insert Name 1");
    propval->SetName(L"KML_FDO_NAME");
    propval->SetValue(value);
    propvalue_coll->Add(propval);

    insertcomm->Execute();

    Conn->Close();

    //conn = c_Util::OpenUnitTestConnection();
    
    Conn->Open();
    
    comm = (FdoIDescribeSchema*)Conn->CreateCommand(FdoCommandType_DescribeSchema);
    schemas = comm->Execute();

    schema = schemas->GetItem(0);
    classes = schema->GetClasses();
    classdef = classes->GetItem(0);
    
    selcomm = (FdoISelect*)Conn->CreateCommand(FdoCommandType_Select);  
    selcomm->SetFeatureClassName(classdef->GetName());
    freader = selcomm->Execute();
    int count2=0;
    while(freader->ReadNext()) count2++;
    freader->Close();
    if( count2 != (count+1) )
    {
      CPPUNIT_FAIL("Number of records after insert is not increased by 1");
    }


    FdoStringP filterstr;
    filterstr = L"KML_FDO_NAME";
    filterstr = filterstr + "='Insert Name 1'";
    selcomm->SetFilter(filterstr);
    freader = selcomm->Execute();
    int count3=0;
    while(freader->ReadNext()) count3++;
    if( count3 != 1 )
    {
      CPPUNIT_FAIL("Unable to select inserted record");
    }



  }  
  catch(FdoException* ex)
  {
    FdoStringP str = ex->GetExceptionMessage();
    ex->Release();
    CPPUNIT_FAIL( (const char*)str );
  }
}//end of  c_TestInsert::InsertTest

