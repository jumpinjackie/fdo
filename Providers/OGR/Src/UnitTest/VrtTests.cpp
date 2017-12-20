#include "VrtTests.h"
#include "UnitTestUtil.h"
#include "Fdo.h"
#include "TestCommonMiscUtil.h"
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