#include "UpdateTests.h"
#include "UnitTestUtil.h"
#include "Fdo.h"
#include "TestCommonMiscUtil.h"
#include <cstdio>

#define LOCATION_SOURCE     L"../../TestData/World_Countries_SHP"
#define LOCATION_UPDATE     L"../../TestData/TestUpdate"
#define LOCATION_UPDATE_RO  L"../../TestData/TestUpdateReadOnly"

CPPUNIT_TEST_SUITE_REGISTRATION (UpdateTests);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION (UpdateTests, "UpdateTests");

UpdateTests::UpdateTests()
{
}

UpdateTests::~UpdateTests()
{
}

void UpdateTests::TestCase_Update()
{
    FdoPtr<FdoIConnection> conn;
    try
    {
        FdoPtr<FdoStringCollection> fileNames = FdoStringCollection::Create();
        fileNames->Add(L"World_Countries.cpg");
        fileNames->Add(L"World_Countries.dbf");
        fileNames->Add(L"World_Countries.idx");
        fileNames->Add(L"World_Countries.shp");
        fileNames->Add(L"World_Countries.shx");

        UnitTestUtil::CopySourceFilesTo(LOCATION_SOURCE, LOCATION_UPDATE, fileNames);

        conn = UnitTestUtil::CreateOgrConnection(LOCATION_UPDATE, false);
        CPPUNIT_ASSERT_MESSAGE("Expected open connection state", conn->Open() == FdoConnectionState_Open);

        FdoPtr<FdoIUpdate> updateCmd = static_cast<FdoIUpdate*>(conn->CreateCommand(FdoCommandType_Update));
        updateCmd->SetFeatureClassName(L"World_Countries");

        FdoPtr<FdoPropertyValueCollection> propVals = updateCmd->GetPropertyValues();
        FdoPtr<FdoStringValue> key      = FdoStringValue::Create(L"TEST1");
        FdoPtr<FdoStringValue> mapkey   = FdoStringValue::Create(L"TEST2");
        
        FdoPtr<FdoPropertyValue> pKey       = FdoPropertyValue::Create(L"KEY", key);
        FdoPtr<FdoPropertyValue> pMapKey    = FdoPropertyValue::Create(L"MAPKEY", mapkey);

        propVals->Add(pKey);
        propVals->Add(pMapKey);

        updateCmd->SetFilter(L"Name = 'Canada'");
        FdoInt32 updated = updateCmd->Execute();

        FdoPtr<FdoISelect> selectCmd = static_cast<FdoISelect*>(conn->CreateCommand(FdoCommandType_Select));
        selectCmd->SetFeatureClassName(L"World_Countries");
        selectCmd->SetFilter(L"Name = 'Canada'");
        FdoPtr<FdoIFeatureReader> rd = selectCmd->Execute();
        FdoInt32 count = 0;
        while(rd->ReadNext())
        {
            count++;
        }
        rd->Close();
        CPPUNIT_ASSERT_MESSAGE("Expected query count to match updated count", updated == count);
    }
    catch (FdoException* ex)
    {
        TestCommonFail(ex);
    }

    if (NULL != conn.p)
        conn->Close();
}

void UpdateTests::TestCase_UpdateReadOnly()
{
    try
    {
    }
    catch (FdoException* ex)
    {
        TestCommonFail(ex);
    }
}
