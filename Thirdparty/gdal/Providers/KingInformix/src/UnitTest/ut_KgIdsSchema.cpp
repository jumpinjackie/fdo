#include "StdAfx.h"

#include "c_KgInfoUtil.h"
#include "ut_KgIdsSchema.h"


CPPUNIT_TEST_SUITE_REGISTRATION(ut_KgIdsSchema);



ut_KgIdsSchema::ut_KgIdsSchema(void)
{
}

ut_KgIdsSchema::~ut_KgIdsSchema(void)
{
}




void ut_KgIdsSchema::DescribeSchema()
{
try
{
  FdoPtr<FdoIConnection> conn = c_KgInfoUtil::OpenUnitTestConnection();
  
  FdoPtr<FdoIDescribeSchema> comm = (FdoIDescribeSchema*)conn->CreateCommand(FdoCommandType_DescribeSchema);
  
  FdoPtr<FdoFeatureSchemaCollection> schemas = comm->Execute();
  
  CPPUNIT_ASSERT_MESSAGE( "FdoIDescribeSchema:Execute returns NULL schema collection" , schemas );
    

  long count=schemas->GetCount();
  
  conn->Close();
  
  
  //CPPUNIT_ASSERT_MESSAGE( "FdoIDescribeSchema:Execute returns 0 spatial contexts " , count );
}
catch(FdoException* ex)
{
  FdoStringP str = ex->GetExceptionMessage();
  ex->Release();
  CPPUNIT_FAIL( (const char*)str );
}
  
}//end of ut_KgIdsSchema::DescribeSchema

void ut_KgIdsSchema::ApplySchema()
{
  try
  {
    FdoPtr<FdoIConnection> conn = c_KgInfoUtil::OpenUnitTestConnection();

    FdoPtr<FdoIApplySchema> comm = (FdoIApplySchema*)conn->CreateCommand(FdoCommandType_ApplySchema);
    
    FdoPtr<FdoFeatureSchema> schema = FdoFeatureSchema::Create(L"UnitTest",L"Test Schema for unit test of apply schema command");
    
    FdoPtr<FdoFeatureClass> classdef = FdoFeatureClass::Create(L"TestClass1",L"Test Class for UnitTest of apply schema command");
    FdoPtr<FdoGeometricPropertyDefinition> geom = FdoGeometricPropertyDefinition::Create(L"Geom",L"");
    geom->SetGeometryTypes(FdoGeometryType_Point);
    classdef->SetGeometryProperty(geom);
    
    FdoPtr<FdoPropertyDefinitionCollection> propcoll = classdef->GetProperties();
    FdoPtr<FdoDataPropertyDefinition> prop_int32 = FdoDataPropertyDefinition::Create(L"Prop_Int32",L"");
    prop_int32->SetDataType(FdoDataType_Int32);
    propcoll->Add(prop_int32);
    
    FdoPtr<FdoDataPropertyDefinition> prop_string = FdoDataPropertyDefinition::Create(L"Prop_String",L"");
    prop_string->SetDataType(FdoDataType_String);
    propcoll->Add(prop_string);
    
    FdoPtr<FdoClassCollection> classes = schema->GetClasses();
    classes->Add(classdef);
    
    comm->SetFeatureSchema(schema);

    comm->Execute();

    
    conn->Close();


    //CPPUNIT_ASSERT_MESSAGE( "FdoIDescribeSchema:Execute returns 0 spatial contexts " , count );
  }
  catch(FdoException* ex)
  {
    FdoStringP str = ex->GetExceptionMessage();
    ex->Release();
    CPPUNIT_FAIL( (const char*)str );
  }

}//end of ut_KgIdsSchema::ApplySchema