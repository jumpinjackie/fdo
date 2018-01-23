#include "StdAfx.h"

#include "c_KgInfoUtil.h"
#include "c_KgInfSchemaTest.h"


CPPUNIT_TEST_SUITE_REGISTRATION(c_KgInfSchemaTest);



c_KgInfSchemaTest::c_KgInfSchemaTest(void)
{
}

c_KgInfSchemaTest::~c_KgInfSchemaTest(void)
{
}




void c_KgInfSchemaTest::ApplySchema()
{
try
{
  // Connect:
        FdoPtr<FdoIConnection> connection = c_KgInfoUtil::OpenUnitTestConnection();
        

        // Define one new schema:
        FdoPtr<FdoFeatureSchema> schema = FdoFeatureSchema::Create(L"lowercaseschema", L"");
        FdoPtr<FdoClassCollection> classes = schema->GetClasses();

        // Define one new class with X/Y/Z lowercase property names (and an identity property and a geometry property):
        FdoPtr<FdoFeatureClass> newClass = FdoFeatureClass::Create(L"lowercaseclass", L"");
        FdoPtr<FdoPropertyDefinitionCollection> newProps = newClass->GetProperties();
        FdoPtr<FdoDataPropertyDefinitionCollection> newIds = newClass->GetIdentityProperties();
        FdoPtr<FdoDataPropertyDefinition> dataPropID = FdoDataPropertyDefinition::Create(L"id", L"");
        dataPropID->SetDataType(FdoDataType_Int32);
        dataPropID->SetNullable(false);
        newProps->Add(dataPropID);
        newIds->Add(dataPropID);
        FdoPtr<FdoDataPropertyDefinition> dataPropX = FdoDataPropertyDefinition::Create(L"x", L"");
        dataPropX->SetDataType(FdoDataType_Double);
        dataPropX->SetNullable(false);
        newProps->Add(dataPropX);
        FdoPtr<FdoDataPropertyDefinition> dataPropY = FdoDataPropertyDefinition::Create(L"y", L"");
        dataPropY->SetDataType(FdoDataType_Double);
        dataPropY->SetNullable(false);
        newProps->Add(dataPropY);
        FdoPtr<FdoDataPropertyDefinition> dataPropZ = FdoDataPropertyDefinition::Create(L"z", L"");
        dataPropZ->SetDataType(FdoDataType_Double);
        dataPropZ->SetNullable(false);
        newProps->Add(dataPropZ);
        FdoPtr<FdoGeometricPropertyDefinition> dataPropGeom = FdoGeometricPropertyDefinition::Create(L"realgeometry", L"");
        dataPropGeom->SetGeometryTypes(FdoGeometricType_Point);
        newProps->Add(dataPropGeom);
        newClass->SetGeometryProperty(dataPropGeom);
        classes->Add(newClass);

        // Create the new class:
        FdoPtr<FdoIApplySchema> as = (FdoIApplySchema*)connection->CreateCommand(FdoCommandType_ApplySchema);
        as->SetFeatureSchema(schema);
        as->Execute();
         
        connection->Close();            

}
catch(FdoException* ex)
{
  FdoStringP str = ex->GetExceptionMessage();
  ex->Release();
  CPPUNIT_FAIL( (const char*)str );
}
  
}//end of c_KgInfSchemaTest::PerfomanceTest