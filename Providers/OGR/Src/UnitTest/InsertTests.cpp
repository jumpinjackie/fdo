#include "InsertTests.h"
#include "UnitTestUtil.h"
#include "Fdo.h"
#include "TestCommonMiscUtil.h"
#include <cstdio>

#define LOCATION_SOURCE     L"../../TestData/World_Countries_SHP"
#define LOCATION_INSERT     L"../../TestData/TestInsert"
#define LOCATION_INSERT_RO  L"../../TestData/TestInsertReadOnly"
#define LOCATION_INSERT_BAD L"../../TestData/TestInsertBadClassNames"

CPPUNIT_TEST_SUITE_REGISTRATION (InsertTests);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION (InsertTests, "InsertTests");

InsertTests::InsertTests()
{
}

InsertTests::~InsertTests()
{
}

void InsertTests::TestCase_Insert()
{
    if (FdoCommonFile::FileExists(LOCATION_INSERT))
        FdoCommonFile::RmDir(LOCATION_INSERT);

    if (!FdoCommonFile::FileExists(LOCATION_INSERT))
        FdoCommonFile::MkDir(LOCATION_INSERT);

    CopySourceFilesTo(LOCATION_INSERT);

    FdoPtr<FdoIConnection> conn;
    try
    {
        conn = UnitTestUtil::CreateOgrConnection(LOCATION_INSERT, false);
        CPPUNIT_ASSERT_MESSAGE("Expected open connection state", conn->Open() == FdoConnectionState_Open);

        FdoPtr<FdoIInsert> insertCmd = static_cast<FdoIInsert*>(conn->CreateCommand(FdoCommandType_Insert));
        insertCmd->SetFeatureClassName(L"World_Countries");

        FdoPtr<FdoFgfGeometryFactory> geomFact = FdoFgfGeometryFactory::GetInstance();
        FdoPtr<FdoIGeometry> geometry = geomFact->CreateGeometry(L"POLYGON ((30 10, 40 40, 20 40, 10 20, 30 10))");
        FdoPtr<FdoByteArray> fgf = geomFact->GetFgf(geometry);

        FdoPtr<FdoPropertyValueCollection> propVals = insertCmd->GetPropertyValues();
        FdoPtr<FdoStringValue> key      = FdoStringValue::Create(L"TEST1");
        FdoPtr<FdoStringValue> name     = FdoStringValue::Create(L"Test Country");
        FdoPtr<FdoGeometryValue> geom   = FdoGeometryValue::Create(fgf);
        
        FdoPtr<FdoPropertyValue> pKey       = FdoPropertyValue::Create(L"KEY", key);
        FdoPtr<FdoPropertyValue> pName      = FdoPropertyValue::Create(L"NAME", name);
        FdoPtr<FdoPropertyValue> pGeom      = FdoPropertyValue::Create(L"GEOMETRY", geom);

        propVals->Add(pKey);
        propVals->Add(pName);
        propVals->Add(pGeom);

        FdoPtr<FdoIFeatureReader> reader = insertCmd->Execute();
        FdoInt32 count = 0;
        FdoInt32 fid = -1;
        while(reader->ReadNext())
        {
            fid = reader->GetInt32(L"FID");
            CPPUNIT_ASSERT_MESSAGE("Expected null MAPKEY", reader->IsNull(L"MAPKEY"));
            count++;
        }
        reader->Close();
        CPPUNIT_ASSERT_MESSAGE("Expected 1 inserted feature", 1 == count);
        CPPUNIT_ASSERT_MESSAGE("Expected an assigned FID", -1 != fid);

        FdoPtr<FdoISelect> selectCmd = static_cast<FdoISelect*>(conn->CreateCommand(FdoCommandType_Select));
        selectCmd->SetFeatureClassName(L"World_Countries");
        FdoPtr<FdoIFeatureReader> rd = selectCmd->Execute();
        count = 0;
        while(rd->ReadNext())
        {
            count++;
        }
        rd->Close();
        CPPUNIT_ASSERT_MESSAGE("Expected 420 features after insert", 420 == count);
        selectCmd->SetFilter(L"KEY = 'TEST1'");
        rd = selectCmd->Execute();
        count = 0;
        while(rd->ReadNext())
        {
            count++;
        }
        rd->Close();
        CPPUNIT_ASSERT_MESSAGE("Expected 1 feature", 1 == count);
    }
    catch (FdoException* ex)
    {
        TestCommonFail(ex);
    }

    if (NULL != conn.p)
        conn->Close();

    if (FdoCommonFile::FileExists(LOCATION_INSERT))
        FdoCommonFile::RmDir(LOCATION_INSERT);
}

void InsertTests::TestCase_InsertBadClassName()
{
    if (FdoCommonFile::FileExists(LOCATION_INSERT_BAD))
        FdoCommonFile::RmDir(LOCATION_INSERT_BAD);

    if (!FdoCommonFile::FileExists(LOCATION_INSERT_BAD))
        FdoCommonFile::MkDir(LOCATION_INSERT_BAD);

    CopySourceFilesTo(LOCATION_INSERT_BAD);

    FdoPtr<FdoIConnection> conn;
    try
    {
        conn = UnitTestUtil::CreateOgrConnection(LOCATION_INSERT_BAD, false);
        CPPUNIT_ASSERT_MESSAGE("Expected open connection state", conn->Open() == FdoConnectionState_Open);

        FdoPtr<FdoIInsert> insertCmd = static_cast<FdoIInsert*>(conn->CreateCommand(FdoCommandType_Insert));
        insertCmd->SetFeatureClassName(L"World_C");

        FdoPtr<FdoFgfGeometryFactory> geomFact = FdoFgfGeometryFactory::GetInstance();
        FdoPtr<FdoIGeometry> geometry = geomFact->CreateGeometry(L"POLYGON ((30 10, 40 40, 20 40, 10 20, 30 10))");
        FdoPtr<FdoByteArray> fgf = geomFact->GetFgf(geometry);

        FdoPtr<FdoPropertyValueCollection> propVals = insertCmd->GetPropertyValues();
        FdoPtr<FdoStringValue> key = FdoStringValue::Create(L"TEST1");
        FdoPtr<FdoStringValue> name = FdoStringValue::Create(L"Test Country");
        FdoPtr<FdoGeometryValue> geom = FdoGeometryValue::Create(fgf);

        FdoPtr<FdoPropertyValue> pKey = FdoPropertyValue::Create(L"KEY", key);
        FdoPtr<FdoPropertyValue> pName = FdoPropertyValue::Create(L"NAME", name);
        FdoPtr<FdoPropertyValue> pGeom = FdoPropertyValue::Create(L"GEOMETRY", geom);

        propVals->Add(pKey);
        propVals->Add(pName);
        propVals->Add(pGeom);

        FdoPtr<FdoIFeatureReader> reader = insertCmd->Execute();
        reader->Close();
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

    if (FdoCommonFile::FileExists(LOCATION_INSERT_BAD))
        FdoCommonFile::RmDir(LOCATION_INSERT_BAD);
}

void InsertTests::TestCase_InsertReadOnly()
{
    if (FdoCommonFile::FileExists(LOCATION_INSERT_RO))
        FdoCommonFile::RmDir(LOCATION_INSERT_RO);

    if (!FdoCommonFile::FileExists(LOCATION_INSERT_RO))
        FdoCommonFile::MkDir(LOCATION_INSERT_RO);

    CopySourceFilesTo(LOCATION_INSERT_RO);

    FdoPtr<FdoIConnection> conn;
    try
    {
        conn = UnitTestUtil::CreateOgrConnection(LOCATION_INSERT_RO, true);
        CPPUNIT_ASSERT_MESSAGE("Expected open connection state", conn->Open() == FdoConnectionState_Open);

        FdoPtr<FdoIInsert> insertCmd = static_cast<FdoIInsert*>(conn->CreateCommand(FdoCommandType_Insert));
        insertCmd->SetFeatureClassName(L"World_Countries");

        FdoPtr<FdoFgfGeometryFactory> geomFact = FdoFgfGeometryFactory::GetInstance();
        FdoPtr<FdoIGeometry> geometry = geomFact->CreateGeometry(L"POLYGON ((30 10, 40 40, 20 40, 10 20, 30 10))");
        FdoPtr<FdoByteArray> fgf = geomFact->GetFgf(geometry);

        FdoPtr<FdoPropertyValueCollection> propVals = insertCmd->GetPropertyValues();
        FdoPtr<FdoStringValue> key      = FdoStringValue::Create(L"TEST1");
        FdoPtr<FdoStringValue> mapkey   = FdoStringValue::Create(L"TEST2");
        FdoPtr<FdoStringValue> name     = FdoStringValue::Create(L"Test Country");
        FdoPtr<FdoGeometryValue> geom   = FdoGeometryValue::Create(fgf);
        
        FdoPtr<FdoPropertyValue> pKey       = FdoPropertyValue::Create(L"KEY", key);
        FdoPtr<FdoPropertyValue> pMapKey    = FdoPropertyValue::Create(L"MAPKEY", mapkey);
        FdoPtr<FdoPropertyValue> pName      = FdoPropertyValue::Create(L"NAME", name);
        FdoPtr<FdoPropertyValue> pGeom      = FdoPropertyValue::Create(L"GEOMETRY", geom);

        propVals->Add(pKey);
        propVals->Add(pMapKey);
        propVals->Add(pName);
        propVals->Add(pGeom);

        FdoPtr<FdoIFeatureReader> reader = insertCmd->Execute();
        reader->Close();
        if (NULL != conn.p)
            conn->Close();

        CPPUNIT_FAIL("Expected insert failure because ReadOnly = true");
    }
    catch (FdoException* ex)
    {
        printf("Caught FdoException: %S\n", ex->GetExceptionMessage());
        FDO_SAFE_RELEASE(ex);
        if (NULL != conn.p)
            conn->Close();
    }

    if (FdoCommonFile::FileExists(LOCATION_INSERT_RO))
        FdoCommonFile::RmDir(LOCATION_INSERT_RO);
}

void InsertTests::CopySourceFilesTo(const wchar_t* dir)
{
    FdoPtr<FdoStringCollection> fileNames = FdoStringCollection::Create();
    fileNames->Add(L"World_Countries.cpg");
    fileNames->Add(L"World_Countries.dbf");
    fileNames->Add(L"World_Countries.idx");
    fileNames->Add(L"World_Countries.shp");
    fileNames->Add(L"World_Countries.shx");
    UnitTestUtil::CopySourceFilesTo(LOCATION_SOURCE, dir, fileNames);
}