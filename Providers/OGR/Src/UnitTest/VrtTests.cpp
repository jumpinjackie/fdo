#include "VrtTests.h"
#include "UnitTestUtil.h"
#include "Fdo.h"
#include "TestCommonMiscUtil.h"
#include "Util/FdoExpressionEngineUtilDataReader.h"
#include "Util/FdoExpressionEngineUtilFeatureReader.h"
#include <cstdio>

CPPUNIT_TEST_SUITE_REGISTRATION(VrtTests);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(VrtTests, "VrtTests");

VrtTests::VrtTests() { }

VrtTests::~VrtTests() { }

void VrtTests::TestCase_DescribeDeparts()
{
    FdoPtr<FdoIConnection> conn;
    try
    {
        conn = UnitTestUtil::CreateOgrConnection(L"../../TestData/departs.vrt");
        FdoConnectionState state = conn->Open();
        CPPUNIT_ASSERT_MESSAGE("Expected open state", state == FdoConnectionState_Open);

        FdoPtr<FdoIDescribeSchema> describe = (FdoIDescribeSchema*)conn->CreateCommand(FdoCommandType_DescribeSchema);
        FdoPtr<FdoFeatureSchemaCollection> schemas = describe->Execute();

        CPPUNIT_ASSERT(1 == schemas->GetCount());
        CPPUNIT_ASSERT(schemas->IndexOf(L"OGRSchema") >= 0);

        FdoPtr<FdoFeatureSchema> schema = schemas->GetItem(0);
        FdoPtr<FdoClassCollection> classes = schema->GetClasses();

        CPPUNIT_ASSERT(1 == classes->GetCount());

        FdoPtr<FdoClassDefinition> klass = classes->GetItem(0);
        FdoStringP className = klass->GetName();
        CPPUNIT_ASSERT(className == L"départs");

        FdoPtr<FdoPropertyDefinitionCollection> props = klass->GetProperties();
        CPPUNIT_ASSERT(4 == props->GetCount());

        CPPUNIT_ASSERT(props->IndexOf(L"FID") >= 0);
        CPPUNIT_ASSERT(props->IndexOf(L"NOMDéPART") >= 0);
        CPPUNIT_ASSERT(props->IndexOf(L"ID") >= 0);
        CPPUNIT_ASSERT(props->IndexOf(L"GEOMETRY") >= 0);

        conn->Close();
    }
    catch (FdoException* ex)
    {
        TestCommonFail(ex);
    }
    if (NULL != conn.p)
        conn->Close();
}

void VrtTests::TestCase_DescribeInvalid()
{
    FdoPtr<FdoIConnection> conn;
    try
    {
        conn = UnitTestUtil::CreateOgrConnection(L"../../TestData/invalid.vrt");
        FdoConnectionState state = conn->Open();

        FdoPtr<FdoIDescribeSchema> describe = (FdoIDescribeSchema*)conn->CreateCommand(FdoCommandType_DescribeSchema);
        FdoPtr<FdoFeatureSchemaCollection> schemas = describe->Execute();

        CPPUNIT_FAIL("Expected exception");
    }
    catch (FdoException* ex)
    {
        FDO_SAFE_RELEASE(ex);
    }
}

void VrtTests::TestCase_DescribeInvalid2()
{
    FdoPtr<FdoIConnection> conn;
    try
    {
        conn = UnitTestUtil::CreateOgrConnection(L"../../TestData/invalid2.vrt");
        FdoConnectionState state = conn->Open();

        FdoPtr<FdoIDescribeSchema> describe = (FdoIDescribeSchema*)conn->CreateCommand(FdoCommandType_DescribeSchema);
        FdoPtr<FdoFeatureSchemaCollection> schemas = describe->Execute();

        CPPUNIT_FAIL("Expected exception");
    }
    catch (FdoException* ex)
    {
        FDO_SAFE_RELEASE(ex);
    }
}

void VrtTests::TestCase_DescribeInvalid3()
{
    FdoPtr<FdoIConnection> conn;
    try
    {
        conn = UnitTestUtil::CreateOgrConnection(L"../../TestData/invalid3.vrt");
        FdoConnectionState state = conn->Open();

        FdoPtr<FdoIDescribeSchema> describe = (FdoIDescribeSchema*)conn->CreateCommand(FdoCommandType_DescribeSchema);
        FdoPtr<FdoFeatureSchemaCollection> schemas = describe->Execute();

        CPPUNIT_FAIL("Expected exception");
    }
    catch (FdoException* ex)
    {
        FDO_SAFE_RELEASE(ex);
    }
}

void VrtTests::TestCase_DescribeSchema()
{
    FdoPtr<FdoIConnection> conn;
    try
    {
        conn = UnitTestUtil::CreateOgrConnection(L"../../TestData/poly_vrt.vrt");
        FdoConnectionState state = conn->Open();
        CPPUNIT_ASSERT_MESSAGE("Expected open state", state == FdoConnectionState_Open);

        FdoPtr<FdoIDescribeSchema> describe = (FdoIDescribeSchema*)conn->CreateCommand(FdoCommandType_DescribeSchema);
        FdoPtr<FdoFeatureSchemaCollection> schemas = describe->Execute();

        CPPUNIT_ASSERT(1 == schemas->GetCount());
        CPPUNIT_ASSERT(schemas->IndexOf(L"OGRSchema") >= 0);

        FdoPtr<FdoFeatureSchema> schema = schemas->GetItem(0);
        FdoPtr<FdoClassCollection> classes = schema->GetClasses();

        CPPUNIT_ASSERT(1 == classes->GetCount());

        FdoPtr<FdoClassDefinition> klass = classes->GetItem(0);
        FdoStringP className = klass->GetName();
        CPPUNIT_ASSERT(className == L"poly");

        FdoPtr<FdoPropertyDefinitionCollection> props = klass->GetProperties();
        CPPUNIT_ASSERT(5 == props->GetCount());

        CPPUNIT_ASSERT(props->IndexOf(L"FID") >= 0);
        CPPUNIT_ASSERT(props->IndexOf(L"EAS_ID") >= 0);
        CPPUNIT_ASSERT(props->IndexOf(L"AREA") >= 0);
        CPPUNIT_ASSERT(props->IndexOf(L"PRFEDEA") >= 0);
        CPPUNIT_ASSERT(props->IndexOf(L"GEOMETRY") >= 0);

        conn->Close();
    }
    catch (FdoException* ex)
    {
        TestCommonFail(ex);
    }
    if (NULL != conn.p)
        conn->Close();
}

void VrtTests::TestCase_DescribePolySql()
{
    FdoPtr<FdoIConnection> conn;
    try
    {
        conn = UnitTestUtil::CreateOgrConnection(L"../../TestData/poly_sql.vrt");
        FdoConnectionState state = conn->Open();
        CPPUNIT_ASSERT_MESSAGE("Expected open state", state == FdoConnectionState_Open);

        FdoPtr<FdoIDescribeSchema> describe = (FdoIDescribeSchema*)conn->CreateCommand(FdoCommandType_DescribeSchema);
        FdoPtr<FdoFeatureSchemaCollection> schemas = describe->Execute();

        CPPUNIT_ASSERT(1 == schemas->GetCount());
        CPPUNIT_ASSERT(schemas->IndexOf(L"OGRSchema") >= 0);

        FdoPtr<FdoFeatureSchema> schema = schemas->GetItem(0);
        FdoPtr<FdoClassCollection> classes = schema->GetClasses();

        CPPUNIT_ASSERT(1 == classes->GetCount());

        FdoPtr<FdoClassDefinition> klass = classes->GetItem(0);
        {
            FdoFeatureClass* fc = static_cast<FdoFeatureClass*>(klass.p);
            CPPUNIT_ASSERT(FdoClassType_FeatureClass == fc->GetClassType());
            FdoStringP className = fc->GetName();
            CPPUNIT_ASSERT(className == L"poly");

            FdoPtr<FdoPropertyDefinitionCollection> props = fc->GetProperties();
            CPPUNIT_ASSERT(5 == props->GetCount());

            CPPUNIT_ASSERT(props->IndexOf(L"FID") >= 0);
            CPPUNIT_ASSERT(props->IndexOf(L"p~EAS_ID") >= 0);
            CPPUNIT_ASSERT(props->IndexOf(L"p~AREA") >= 0);
            CPPUNIT_ASSERT(props->IndexOf(L"p~PRFEDEA") >= 0);

            FdoPtr<FdoGeometricPropertyDefinition> geomProp = fc->GetGeometryProperty();
            CPPUNIT_ASSERT(NULL != geomProp.p);
            CPPUNIT_ASSERT(props->IndexOf(geomProp->GetName()) >= 0);
        }
        conn->Close();
    }
    catch (FdoException* ex)
    {
        TestCommonFail(ex);
    }
    if (NULL != conn.p)
        conn->Close();
}

void VrtTests::TestCase_SelectPolySql()
{
    try
    {
        FdoPtr<FdoIConnection> conn = UnitTestUtil::CreateOgrConnection(L"../../TestData/poly_sql.vrt");
        FdoConnectionState state = conn->Open();
        CPPUNIT_ASSERT_MESSAGE("Expected open state", state == FdoConnectionState_Open);

        FdoPtr<FdoISelect> selectCmd = static_cast<FdoISelect*>(conn->CreateCommand(FdoCommandType_Select));
        selectCmd->SetFeatureClassName(L"poly");

        FdoPtr<FdoIFeatureReader> reader = selectCmd->Execute();
        FdoExpressionEngineUtilFeatureReader* exprReader = dynamic_cast<FdoExpressionEngineUtilFeatureReader*>(reader.p);
        CPPUNIT_ASSERT_MESSAGE("Expected a plain feature reader", NULL == exprReader);

        FdoInt32 count = 0;
        FdoPtr<FdoClassDefinition> clsDef = reader->GetClassDefinition();
        CPPUNIT_ASSERT(FdoClassType_FeatureClass == clsDef->GetClassType());
        {
            FdoFeatureClass* fc = static_cast<FdoFeatureClass*>(clsDef.p);
            FdoPtr<FdoGeometricPropertyDefinition> geomProp = fc->GetGeometryProperty();
            CPPUNIT_ASSERT(NULL != geomProp.p);
            FdoPtr<FdoPropertyDefinitionCollection> clsProps = fc->GetProperties();
            CPPUNIT_ASSERT_MESSAGE("Expected 5 properties in reader", 5 == clsProps->GetCount());
            /*
            for (FdoInt32 i = 0; i < clsProps->GetCount(); i++)
            {
            FdoPtr<FdoPropertyDefinition> propDef = clsProps->GetItem(i);
            printf("%S\n", propDef->GetName());
            }
            */
            CPPUNIT_ASSERT(reader->GetPropertyIndex(L"FID") >= 0);
            CPPUNIT_ASSERT(reader->GetPropertyIndex(L"p~EAS_ID") >= 0);
            CPPUNIT_ASSERT(reader->GetPropertyIndex(L"p~AREA") >= 0);
            CPPUNIT_ASSERT(reader->GetPropertyIndex(L"p~PRFEDEA") >= 0);
            CPPUNIT_ASSERT(reader->GetPropertyIndex(geomProp->GetName()) >= 0);

            while (reader->ReadNext())
            {
                count++;
            }
            reader->Close();
            CPPUNIT_ASSERT_MESSAGE("Expected 10 features", 10 == count);
            //Why are we executing again? Just to test we're not retaining state of any kind between command executions
            count = 0;
            reader = selectCmd->Execute();
            exprReader = dynamic_cast<FdoExpressionEngineUtilFeatureReader*>(reader.p);
            CPPUNIT_ASSERT_MESSAGE("Expected a plain feature reader", NULL == exprReader);

            CPPUNIT_ASSERT(reader->GetPropertyIndex(L"FID") >= 0);
            CPPUNIT_ASSERT(reader->GetPropertyIndex(L"p~EAS_ID") >= 0);
            CPPUNIT_ASSERT(reader->GetPropertyIndex(L"p~AREA") >= 0);
            CPPUNIT_ASSERT(reader->GetPropertyIndex(L"p~PRFEDEA") >= 0);
            CPPUNIT_ASSERT(reader->GetPropertyIndex(geomProp->GetName()) >= 0);
            while (reader->ReadNext())
            {
                count++;
            }
            reader->Close();
            CPPUNIT_ASSERT_MESSAGE("Expected 10 features", 10 == count);
        }
        conn->Close();
    }
    catch (FdoException* ex)
    {
        TestCommonFail(ex);
    }
}

void VrtTests::TestCase_SelectPolySqlExplicitProps()
{
    try
    {
        FdoPtr<FdoIConnection> conn = UnitTestUtil::CreateOgrConnection(L"../../TestData/poly_sql.vrt");
        FdoConnectionState state = conn->Open();
        CPPUNIT_ASSERT_MESSAGE("Expected open state", state == FdoConnectionState_Open);

        FdoPtr<FdoIDescribeSchema> describeCmd = static_cast<FdoIDescribeSchema*>(conn->CreateCommand(FdoCommandType_DescribeSchema));
        FdoPtr<FdoFeatureSchemaCollection> schemas = describeCmd->Execute();
        FdoPtr<FdoFeatureSchema> schema = schemas->GetItem(0);
        FdoPtr<FdoClassCollection> classes = schema->GetClasses();
        FdoPtr<FdoClassDefinition> classDef = classes->GetItem(0);
        {
            FdoFeatureClass* fc = static_cast<FdoFeatureClass*>(classDef.p);
            FdoPtr<FdoGeometricPropertyDefinition> geomProp = fc->GetGeometryProperty();

            FdoPtr<FdoISelect> selectCmd = static_cast<FdoISelect*>(conn->CreateCommand(FdoCommandType_Select));
            FdoPtr<FdoIdentifierCollection> props = selectCmd->GetPropertyNames();

            FdoPtr<FdoIdentifier> p1 = FdoIdentifier::Create(L"FID");
            //FdoPtr<FdoIdentifier> p2 = FdoIdentifier::Create(L"p~EAS_ID");
            FdoPtr<FdoIdentifier> p3 = FdoIdentifier::Create(L"p~AREA");
            FdoPtr<FdoIdentifier> p4 = FdoIdentifier::Create(L"p~PRFEDEA");
            FdoPtr<FdoIdentifier> p5 = FdoIdentifier::Create(geomProp->GetName());
            props->Add(p1);
            //props->Add(p2);
            props->Add(p3);
            props->Add(p4);
            props->Add(p5);

            selectCmd->SetFeatureClassName(L"poly");

            FdoPtr<FdoIFeatureReader> reader = selectCmd->Execute();
            FdoExpressionEngineUtilFeatureReader* exprReader = dynamic_cast<FdoExpressionEngineUtilFeatureReader*>(reader.p);
            CPPUNIT_ASSERT_MESSAGE("Expected a plain feature reader", NULL == exprReader);

            FdoInt32 count = 0;
            FdoPtr<FdoClassDefinition> clsDef = reader->GetClassDefinition();
            FdoPtr<FdoPropertyDefinitionCollection> clsProps = clsDef->GetProperties();
            CPPUNIT_ASSERT_MESSAGE("Expected 4 properties in reader", 4 == clsProps->GetCount());
            /*
            for (FdoInt32 i = 0; i < clsProps->GetCount(); i++)
            {
            FdoPtr<FdoPropertyDefinition> propDef = clsProps->GetItem(i);
            printf("%S\n", propDef->GetName());
            }
            */
            CPPUNIT_ASSERT(reader->GetPropertyIndex(L"FID") >= 0);
            //CPPUNIT_ASSERT(reader->GetPropertyIndex(L"p~EAS_ID") >= 0);
            CPPUNIT_ASSERT(reader->GetPropertyIndex(L"p~AREA") >= 0);
            CPPUNIT_ASSERT(reader->GetPropertyIndex(L"p~PRFEDEA") >= 0);
            CPPUNIT_ASSERT(reader->GetPropertyIndex(geomProp->GetName()) >= 0);

            while (reader->ReadNext())
            {
                count++;
            }
            reader->Close();
            CPPUNIT_ASSERT_MESSAGE("Expected 10 features", 10 == count);
            //Why are we executing again? Just to test we're not retaining state of any kind between command executions
            count = 0;
            reader = selectCmd->Execute();
            exprReader = dynamic_cast<FdoExpressionEngineUtilFeatureReader*>(reader.p);
            CPPUNIT_ASSERT_MESSAGE("Expected a plain feature reader", NULL == exprReader);

            CPPUNIT_ASSERT(reader->GetPropertyIndex(L"FID") >= 0);
            //CPPUNIT_ASSERT(reader->GetPropertyIndex(L"p~EAS_ID") >= 0);
            CPPUNIT_ASSERT(reader->GetPropertyIndex(L"p~AREA") >= 0);
            CPPUNIT_ASSERT(reader->GetPropertyIndex(L"p~PRFEDEA") >= 0);
            CPPUNIT_ASSERT(reader->GetPropertyIndex(geomProp->GetName()) >= 0);
            while (reader->ReadNext())
            {
                count++;
            }
            reader->Close();
            CPPUNIT_ASSERT_MESSAGE("Expected 10 features", 10 == count);
        }
        conn->Close();
    }
    catch (FdoException* ex)
    {
        TestCommonFail(ex);
    }
}