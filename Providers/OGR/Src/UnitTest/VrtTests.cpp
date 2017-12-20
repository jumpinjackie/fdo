#include "VrtTests.h"
#include "UnitTestUtil.h"
#include "Fdo.h"
#include "TestCommonMiscUtil.h"
#include <cstdio>

CPPUNIT_TEST_SUITE_REGISTRATION(VrtTests);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(VrtTests, "VrtTests");

VrtTests::VrtTests() { }

VrtTests::~VrtTests() { }

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