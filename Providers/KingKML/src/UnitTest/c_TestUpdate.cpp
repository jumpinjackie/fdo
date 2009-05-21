#include "StdAfx.h"
#include "c_TestUpdate.h"

CPPUNIT_TEST_SUITE_REGISTRATION(c_TestUpdate);

c_TestUpdate::c_TestUpdate(void)
{
}

c_TestUpdate::~c_TestUpdate(void)
{
}

void c_TestUpdate::UpdateKML()
{

  try
  {
    c_Util::CreateTestKML();
    FdoPtr<FdoIConnection> conn = c_Util::OpenUnitTestConnection();
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

    FdoPtr<FdoPropertyDefinitionCollection> propdef_col = classdef->GetProperties();
    
    FdoPtr<FdoIUpdate> updatecomm = (FdoIUpdate*)conn->CreateCommand(FdoCommandType_Update);
    updatecomm->SetFeatureClassName(classdef->GetName());
    FdoPtr<FdoPropertyValueCollection> propvalue_coll = updatecomm->GetPropertyValues();
    
    FdoPtr<FdoPropertyValue> propval = FdoPropertyValue::Create();
    FdoPtr<FdoDataValue> value = FdoDataValue::Create(L"UpdateName 1");
    propval->SetName(L"KML_FDO_NAME");
    propval->SetValue(value);
    propvalue_coll->Add(propval);

    FdoStringP filterstr;
    filterstr = "KML_FDO_NAME";
    filterstr = filterstr + "='Name 1'";
    updatecomm->SetFilter(filterstr);
    
    int updated = updatecomm->Execute();
    if( updated != 1)
    {
      CPPUNIT_FAIL("Update failed");
    }

    
    conn->Close();
    
    // Now open KML again and check updatet data
    conn = c_Util::OpenUnitTestConnection();
    selcomm = (FdoISelect*)conn->CreateCommand(FdoCommandType_Select);  
    selcomm->SetFeatureClassName(classdef->GetName());
    filterstr = L"KML_FDO_NAME";
    filterstr = filterstr + "='UpdateName 1'";
    selcomm->SetFilter(filterstr);
    
    freader = selcomm->Execute();
    int count=0;
    while(freader->ReadNext()) count++;
    
    if( count != 1)
    {
      CPPUNIT_FAIL("Update failed");
    }
    
  }  
  catch(FdoException* ex)
  {
    FdoStringP str = ex->GetExceptionMessage();
    ex->Release();
    CPPUNIT_FAIL( (const char*)str );
  }

}