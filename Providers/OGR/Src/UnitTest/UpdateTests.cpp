#include "UpdateTests.h"
#include "UnitTestUtil.h"
#include "Fdo.h"
#include "TestCommonMiscUtil.h"
#include <cstdio>

#define LOCATION_SOURCE     L"../../TestData/World_Countries_SHP"
#define LOCATION_UPDATE     L"../../TestData/TestUpdate"
#define LOCATION_UPDATE_RO  L"../../TestData/TestUpdateReadOnly"
#define LOCATION_UPDATE_BAD L"../../TestData/TestUpdateBadClassName"

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
    FdoString* dir = LOCATION_UPDATE;
    if (FdoCommonFile::FileExists(dir))
        FdoCommonFile::RmDir(dir);

    if (!FdoCommonFile::FileExists(dir))
        FdoCommonFile::MkDir(dir);

    CopySourceFilesTo(dir);

    FdoPtr<FdoIConnection> conn;
    try
    {
        conn = UnitTestUtil::CreateOgrConnection(dir, false);
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

    if (FdoCommonFile::FileExists(dir))
        FdoCommonFile::RmDir(dir);
}

void UpdateTests::TestCase_UpdateBadClassName()
{
    FdoString* dir = LOCATION_UPDATE_BAD;
    if (FdoCommonFile::FileExists(dir))
        FdoCommonFile::RmDir(dir);

    if (!FdoCommonFile::FileExists(dir))
        FdoCommonFile::MkDir(dir);

    CopySourceFilesTo(dir);

    FdoPtr<FdoIConnection> conn;
    try
    {
        conn = UnitTestUtil::CreateOgrConnection(dir, false);
        CPPUNIT_ASSERT_MESSAGE("Expected open connection state", conn->Open() == FdoConnectionState_Open);

        FdoPtr<FdoIUpdate> updateCmd = static_cast<FdoIUpdate*>(conn->CreateCommand(FdoCommandType_Update));
        updateCmd->SetFeatureClassName(L"World_C");

        FdoPtr<FdoPropertyValueCollection> propVals = updateCmd->GetPropertyValues();
        FdoPtr<FdoStringValue> key = FdoStringValue::Create(L"TEST1");
        FdoPtr<FdoStringValue> mapkey = FdoStringValue::Create(L"TEST2");

        FdoPtr<FdoPropertyValue> pKey = FdoPropertyValue::Create(L"KEY", key);
        FdoPtr<FdoPropertyValue> pMapKey = FdoPropertyValue::Create(L"MAPKEY", mapkey);

        propVals->Add(pKey);
        propVals->Add(pMapKey);

        updateCmd->SetFilter(L"Name = 'Canada'");
        FdoInt32 updated = updateCmd->Execute();
        CPPUNIT_FAIL("Expected insert to throw");
    }
    catch (FdoException* ex)
    {
        FdoStringP msg = ex->GetExceptionMessage();
        FDO_SAFE_RELEASE(ex);
        CPPUNIT_ASSERT(msg == L"Class not found: World_C");
    }

    if (NULL != conn.p)
        conn->Close();

    if (FdoCommonFile::FileExists(dir))
        FdoCommonFile::RmDir(dir);
}

void UpdateTests::TestCase_UpdateReadOnly()
{
    FdoString* dir = LOCATION_UPDATE_RO;
    if (FdoCommonFile::FileExists(dir))
        FdoCommonFile::RmDir(dir);

    if (!FdoCommonFile::FileExists(dir))
        FdoCommonFile::MkDir(dir);

    CopySourceFilesTo(dir);

    FdoPtr<FdoIConnection> conn;
    try
    {
        conn = UnitTestUtil::CreateOgrConnection(dir, true);
        CPPUNIT_ASSERT_MESSAGE("Expected open connection state", conn->Open() == FdoConnectionState_Open);

        FdoPtr<FdoIUpdate> updateCmd = static_cast<FdoIUpdate*>(conn->CreateCommand(FdoCommandType_Update));
        updateCmd->SetFeatureClassName(L"World_Countries");

        FdoPtr<FdoPropertyValueCollection> propVals = updateCmd->GetPropertyValues();
        FdoPtr<FdoStringValue> key = FdoStringValue::Create(L"TEST1");
        FdoPtr<FdoStringValue> mapkey = FdoStringValue::Create(L"TEST2");

        FdoPtr<FdoPropertyValue> pKey = FdoPropertyValue::Create(L"KEY", key);
        FdoPtr<FdoPropertyValue> pMapKey = FdoPropertyValue::Create(L"MAPKEY", mapkey);

        propVals->Add(pKey);
        propVals->Add(pMapKey);

        updateCmd->SetFilter(L"Name = 'Canada'");
        FdoInt32 updated = updateCmd->Execute();
        if (NULL != conn.p)
            conn->Close();
        CPPUNIT_FAIL("Expected update failure because ReadOnly = true");
    }
    catch (FdoException* ex)
    {
        printf("Caught FdoException: %S\n", ex->GetExceptionMessage());
        FDO_SAFE_RELEASE(ex);
        if (NULL != conn.p)
            conn->Close();
    }

    if (FdoCommonFile::FileExists(dir))
        FdoCommonFile::RmDir(dir);
}

void UpdateTests::CopySourceFilesTo(const wchar_t* dir)
{
    FdoPtr<FdoStringCollection> fileNames = FdoStringCollection::Create();
    fileNames->Add(L"World_Countries.cpg");
    fileNames->Add(L"World_Countries.dbf");
    fileNames->Add(L"World_Countries.idx");
    fileNames->Add(L"World_Countries.shp");
    fileNames->Add(L"World_Countries.shx");
    UnitTestUtil::CopySourceFilesTo(LOCATION_SOURCE, dir, fileNames);
}