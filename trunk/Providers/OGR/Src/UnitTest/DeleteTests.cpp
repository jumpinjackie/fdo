#include "stdafx.h"
#include "DeleteTests.h"
#include "UnitTestUtil.h"
#include <FdoCommonFile.h>
#include "TestCommonMiscUtil.h"

#define LOCATION_SOURCE     L"../../TestData/World_Countries_SHP"
#define LOCATION_ALL        L"../../TestData/TestDeleteAll"
#define LOCATION_FILTERED   L"../../TestData/TestDeleteFilter"

CPPUNIT_TEST_SUITE_REGISTRATION (DeleteTests);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION (DeleteTests, "DeleteTests");

DeleteTests::DeleteTests()
{
    
}

DeleteTests::~DeleteTests()
{

}

void DeleteTests::TestCase_DeleteFiltered()
{
    if (FdoCommonFile::FileExists(LOCATION_FILTERED))
        FdoCommonFile::RmDir(LOCATION_FILTERED);

    if (!FdoCommonFile::FileExists(LOCATION_FILTERED))
        FdoCommonFile::MkDir(LOCATION_FILTERED);

    CopySourceFilesTo(LOCATION_FILTERED);

    FdoPtr<FdoIConnection> conn;
    
    try
    {
        conn = UnitTestUtil::CreateOgrConnection(LOCATION_FILTERED, true);
        CPPUNIT_ASSERT_MESSAGE("Expected open connection state", conn->Open() == FdoConnectionState_Open);
        FdoPtr<FdoIDelete> deleteCmd = static_cast<FdoIDelete*>(conn->CreateCommand(FdoCommandType_Delete));
        deleteCmd->SetFeatureClassName(L"World_Countries");
        deleteCmd->SetFilter(L"NAME = 'Canada'");

        FdoInt32 deleted = deleteCmd->Execute();
        CPPUNIT_FAIL("Delete should fail because ReadOnly = true");
    }
    catch (FdoException* ex)
    {
        printf("Caught FdoException: %S\n", ex->GetExceptionMessage());
        FDO_SAFE_RELEASE(ex);
    }

    if (NULL != conn.p)
        conn->Close();

    try
    {
        conn = UnitTestUtil::CreateOgrConnection(LOCATION_FILTERED, false);
        CPPUNIT_ASSERT_MESSAGE("Expected open connection state", conn->Open() == FdoConnectionState_Open);
        FdoPtr<FdoIDelete> deleteCmd = static_cast<FdoIDelete*>(conn->CreateCommand(FdoCommandType_Delete));
        deleteCmd->SetFeatureClassName(L"World_Countries");
        deleteCmd->SetFilter(L"NAME = 'Canada'");

        FdoInt32 deleted = deleteCmd->Execute();
        CPPUNIT_ASSERT_MESSAGE("Expected 66 features deleted", 66 == deleted);

        FdoPtr<FdoISelect> selectCmd = static_cast<FdoISelect*>(conn->CreateCommand(FdoCommandType_Select));
        selectCmd->SetFeatureClassName(L"World_Countries");
        selectCmd->SetFilter(L"NAME = 'Canada'");

        FdoPtr<FdoIFeatureReader> reader = selectCmd->Execute();
        FdoInt32 count = 0;
        while(reader->ReadNext())
            count++;

        reader->Close();
        CPPUNIT_ASSERT_MESSAGE("Expected no features from Canada", 0 == count);
    }
    catch (FdoException* ex)
    {
        TestCommonFail(ex);
    }

    if (NULL != conn.p)
        conn->Close();

    if (FdoCommonFile::FileExists(LOCATION_FILTERED))
        FdoCommonFile::RmDir(LOCATION_FILTERED);
}

void DeleteTests::TestCase_DeleteAll()
{
    if (FdoCommonFile::FileExists(LOCATION_ALL))
        FdoCommonFile::RmDir(LOCATION_ALL);

    if (!FdoCommonFile::FileExists(LOCATION_ALL))
        FdoCommonFile::MkDir(LOCATION_ALL);

    CopySourceFilesTo(LOCATION_ALL);

    FdoPtr<FdoIConnection> conn;
    try
    {
        conn = UnitTestUtil::CreateOgrConnection(LOCATION_ALL, true);
        CPPUNIT_ASSERT_MESSAGE("Expected open connection state", conn->Open() == FdoConnectionState_Open);
        FdoPtr<FdoIDelete> deleteCmd = static_cast<FdoIDelete*>(conn->CreateCommand(FdoCommandType_Delete));
        deleteCmd->SetFeatureClassName(L"World_Countries");

        FdoInt32 deleted = deleteCmd->Execute();
        CPPUNIT_FAIL("Delete should fail because ReadOnly = true");
    }
    catch (FdoException* ex)
    {
        printf("Caught FdoException: %S\n", ex->GetExceptionMessage());
        FDO_SAFE_RELEASE(ex);
    }

    if (NULL != conn.p)
        conn->Close();

    try
    {
        conn = UnitTestUtil::CreateOgrConnection(LOCATION_ALL, false);
        CPPUNIT_ASSERT_MESSAGE("Expected open connection state", conn->Open() == FdoConnectionState_Open);
        
        FdoPtr<FdoISelect> countCmd = static_cast<FdoISelect*>(conn->CreateCommand(FdoCommandType_Select));
        countCmd->SetFeatureClassName(L"World_Countries");
        FdoPtr<FdoIFeatureReader> rd = countCmd->Execute();
        FdoInt32 total = 0;
        while(rd->ReadNext())
            total++;

        FdoPtr<FdoIDelete> deleteCmd = static_cast<FdoIDelete*>(conn->CreateCommand(FdoCommandType_Delete));
        deleteCmd->SetFeatureClassName(L"World_Countries");

        FdoInt32 deleted = deleteCmd->Execute();
        CPPUNIT_ASSERT_MESSAGE("Expected all features deleted", total == deleted);

        FdoPtr<FdoISelect> selectCmd = static_cast<FdoISelect*>(conn->CreateCommand(FdoCommandType_Select));
        selectCmd->SetFeatureClassName(L"World_Countries");

        FdoPtr<FdoIFeatureReader> reader = selectCmd->Execute();
        FdoInt32 count = 0;
        while(reader->ReadNext())
            count++;

        reader->Close();
        CPPUNIT_ASSERT_MESSAGE("Expected no features", 0 == count);
    }
    catch (FdoException* ex)
    {
        TestCommonFail(ex);
    }

    if (NULL != conn.p)
        conn->Close();

    if (FdoCommonFile::FileExists(LOCATION_ALL))
        FdoCommonFile::RmDir(LOCATION_ALL);
}

void DeleteTests::CopySourceFilesTo(FdoString* dir)
{
    FdoPtr<FdoStringCollection> fileNames = FdoStringCollection::Create();
    fileNames->Add(L"World_Countries.cpg");
    fileNames->Add(L"World_Countries.dbf");
    fileNames->Add(L"World_Countries.idx");
    fileNames->Add(L"World_Countries.shp");
    fileNames->Add(L"World_Countries.shx");
    UnitTestUtil::CopySourceFilesTo(LOCATION_SOURCE, dir, fileNames);
}