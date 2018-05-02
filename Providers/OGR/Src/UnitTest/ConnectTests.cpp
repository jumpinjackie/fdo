#include "ConnectTests.h"
#include "UnitTestUtil.h"
#include "Fdo.h"
#include "TestCommonMiscUtil.h"
#include <cstdio>

CPPUNIT_TEST_SUITE_REGISTRATION (ConnectTests);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION (ConnectTests, "ConnectTests");

ConnectTests::ConnectTests() { }

ConnectTests::~ConnectTests() { }

void ConnectTests::TestCase_Empty()
{
    FdoPtr<FdoIConnection> conn = UnitTestUtil::CreateOgrConnection(NULL);
    try
    {
        FdoConnectionState state = conn->Open();
        CPPUNIT_FAIL("Expected FdoException to be thrown");
    }
    catch (FdoException* ex)
    {
        FdoStringP msg = ex->GetExceptionMessage();
        printf("FdoException caught: %S\n", (FdoString*)msg);
        CPPUNIT_ASSERT(msg == L"Missing required DataSource property");
        FDO_SAFE_RELEASE(ex);
    }
}

void ConnectTests::TestCase_DXF() 
{
    try
    {
        FdoPtr<FdoIConnection> conn = UnitTestUtil::CreateOgrConnection(L"../../TestData/assorted.dxf");
        FdoConnectionState state = conn->Open();
        CPPUNIT_ASSERT_MESSAGE("Expected open state", state == FdoConnectionState_Open);
        conn->Close();
    }
    catch (FdoException* ex)
    {
        TestCommonFail(ex);
    }
}

void ConnectTests::TestCase_MapInfo() 
{
    try
    {
        FdoPtr<FdoIConnection> conn = UnitTestUtil::CreateOgrConnection(L"../../TestData/utm31.TAB");
        FdoConnectionState state = conn->Open();
        CPPUNIT_ASSERT_MESSAGE("Expected open state", state == FdoConnectionState_Open);
        conn->Close();
    }
    catch (FdoException* ex)
    {
        TestCommonFail(ex);
    }
}

void ConnectTests::TestCase_SHP() 
{
    try
    {
        FdoPtr<FdoIConnection> conn = UnitTestUtil::CreateOgrConnection(L"../../TestData/poly.shp");
        FdoConnectionState state = conn->Open();
        CPPUNIT_ASSERT_MESSAGE("Expected open state", state == FdoConnectionState_Open);
        conn->Close();
    }
    catch (FdoException* ex)
    {
        TestCommonFail(ex);
    }
}

void ConnectTests::TestCase_GeoJSON() 
{ 
    try
    {
        FdoPtr<FdoIConnection> conn = UnitTestUtil::CreateOgrConnection(L"../../TestData/point.geojson");
        FdoConnectionState state = conn->Open();
        CPPUNIT_ASSERT_MESSAGE("Expected open state", state == FdoConnectionState_Open);
        conn->Close();
    }
    catch (FdoException* ex)
    {
        TestCommonFail(ex);
    }
}

void ConnectTests::TestCase_VRT() 
{ }

void ConnectTests::TestCase_DGN() 
{ 
    try
    {
        FdoPtr<FdoIConnection> conn = UnitTestUtil::CreateOgrConnection(L"../../TestData/smalltest.dgn");
        FdoConnectionState state = conn->Open();
        CPPUNIT_ASSERT_MESSAGE("Expected open state", state == FdoConnectionState_Open);
        conn->Close();
    }
    catch (FdoException* ex)
    {
        TestCommonFail(ex);
    }
}

void ConnectTests::TestCase_E00() 
{
    try
    {
        FdoPtr<FdoIConnection> conn = UnitTestUtil::CreateOgrConnection(L"../../TestData/test.e00");
        FdoConnectionState state = conn->Open();
        CPPUNIT_ASSERT_MESSAGE("Expected open state", state == FdoConnectionState_Open);
        conn->Close();
    }
    catch (FdoException* ex)
    {
        TestCommonFail(ex);
    }
}

void ConnectTests::TestCase_BNA() 
{
    try
    {
        FdoPtr<FdoIConnection> conn = UnitTestUtil::CreateOgrConnection(L"../../TestData/test.bna");
        FdoConnectionState state = conn->Open();
        CPPUNIT_ASSERT_MESSAGE("Expected open state", state == FdoConnectionState_Open);
        conn->Close();
    }
    catch (FdoException* ex)
    {
        TestCommonFail(ex);
    }
}

void ConnectTests::TestCase_MutableCaps()
{
    try
    {
        FdoPtr<FdoIConnection> conn = UnitTestUtil::CreateOgrConnection(L"../../TestData/poly.shp", false);
        FdoConnectionState state = conn->Open();
        CPPUNIT_ASSERT_MESSAGE("Expected open state", state == FdoConnectionState_Open);

        FdoPtr<FdoICommandCapabilities> cmdCaps = conn->GetCommandCapabilities();
        FdoInt32 size = -1;
        FdoInt32* cmds = cmdCaps->GetCommands(size);
        CPPUNIT_ASSERT(size > 0);

        bool canInsert = false;
        bool canUpdate = false;
        bool canDelete = false;
        for (FdoInt32 i = 0; i < size; i++)
        {
            FdoInt32 cmd = cmds[i];
            switch (cmd)
            {
                case FdoCommandType_Insert:
                    canInsert = true;
                    break;
                case FdoCommandType_Update:
                    canUpdate = true;
                    break;
                case FdoCommandType_Delete:
                    canDelete = true;
                    break;
            }
        }

        CPPUNIT_ASSERT(canInsert);
        CPPUNIT_ASSERT(canUpdate);
        CPPUNIT_ASSERT(canDelete);

        conn->Close();
    }
    catch (FdoException* ex)
    {
        TestCommonFail(ex);
    }
}

void ConnectTests::TestCase_ReadOnlyCaps()
{
    try
    {
        FdoPtr<FdoIConnection> conn = UnitTestUtil::CreateOgrConnection(L"../../TestData/poly.shp", true);
        FdoConnectionState state = conn->Open();
        CPPUNIT_ASSERT_MESSAGE("Expected open state", state == FdoConnectionState_Open);

        FdoPtr<FdoICommandCapabilities> cmdCaps = conn->GetCommandCapabilities();
        FdoInt32 size = -1;
        FdoInt32* cmds = cmdCaps->GetCommands(size);
        CPPUNIT_ASSERT(size > 0);

        bool canInsert = false;
        bool canUpdate = false;
        bool canDelete = false;
        for (FdoInt32 i = 0; i < size; i++)
        {
            FdoInt32 cmd = cmds[i];
            switch (cmd)
            {
                case FdoCommandType_Insert:
                    canInsert = true;
                    break;
                case FdoCommandType_Update:
                    canUpdate = true;
                    break;
                case FdoCommandType_Delete:
                    canDelete = true;
                    break;
            }
        }

        CPPUNIT_ASSERT(!canInsert);
        CPPUNIT_ASSERT(!canUpdate);
        CPPUNIT_ASSERT(!canDelete);

        conn->Close();
    }
    catch (FdoException* ex)
    {
        TestCommonFail(ex);
    }
}

void ConnectTests::TestCase_CustomDefaultSchema()
{
    try
    {
        FdoString* schemaName = L"DefaultSchema";
        FdoPtr<FdoIConnection> conn = UnitTestUtil::CreateOgrConnection(L"../../TestData/World_Countries/World_Countries.tab", true, schemaName);
        FdoConnectionState state = conn->Open();
        CPPUNIT_ASSERT_MESSAGE("Expected open state", state == FdoConnectionState_Open);

        FdoPtr<FdoIDescribeSchema> describe = static_cast<FdoIDescribeSchema*>(conn->CreateCommand(FdoCommandType_DescribeSchema));
        FdoPtr<FdoFeatureSchemaCollection> schemas = describe->Execute();
        CPPUNIT_ASSERT(1 == schemas->GetCount());
        FdoPtr<FdoFeatureSchema> schema = schemas->GetItem(0);
        CPPUNIT_ASSERT(0 == wcscmp(schemaName, schema->GetName()));

        conn->Close();
    }
    catch (FdoException* ex)
    {
        TestCommonFail(ex);
    }
}