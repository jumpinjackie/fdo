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
        printf("FdoException caught: %S\n", ex->GetExceptionMessage());
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
