#include "SelectTests.h"
#include "UnitTestUtil.h"
#include "Fdo.h"
#include "TestCommonMiscUtil.h"
#include "Util/FdoExpressionEngineUtilDataReader.h"
#include "Util/FdoExpressionEngineUtilFeatureReader.h"
#include <cstdio>

CPPUNIT_TEST_SUITE_REGISTRATION (SelectTests);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION (SelectTests, "SelectTests");

SelectTests::SelectTests()
{
}

SelectTests::~SelectTests()
{
}

void SelectTests::TestCase_SelectAll()
{
    try
    {
        FdoPtr<FdoIConnection> conn = UnitTestUtil::CreateOgrConnection(L"../../TestData/World_Countries/World_Countries.tab");
        FdoConnectionState state = conn->Open();
        CPPUNIT_ASSERT_MESSAGE("Expected open state", state == FdoConnectionState_Open);

        FdoPtr<FdoISelect> selectCmd = static_cast<FdoISelect*>(conn->CreateCommand(FdoCommandType_Select));
        selectCmd->SetFeatureClassName(L"World_Countries");

        FdoPtr<FdoIFeatureReader> reader = selectCmd->Execute();
        FdoExpressionEngineUtilFeatureReader* exprReader = dynamic_cast<FdoExpressionEngineUtilFeatureReader*>(reader.p);
        CPPUNIT_ASSERT_MESSAGE("Expected a plain feature reader", NULL == exprReader);

        FdoInt32 count = 0;
        FdoPtr<FdoClassDefinition> clsDef = reader->GetClassDefinition();
        FdoPtr<FdoPropertyDefinitionCollection> clsProps = clsDef->GetProperties();
        CPPUNIT_ASSERT_MESSAGE("Expected 5 properties in reader", 5 == clsProps->GetCount());
        /*
        for (FdoInt32 i = 0; i < clsProps->GetCount(); i++)
        {
            FdoPtr<FdoPropertyDefinition> propDef = clsProps->GetItem(i);
            printf("%S\n", propDef->GetName());
        }
        */
        while (reader->ReadNext())
        {
            count++;
        }
        reader->Close();
        CPPUNIT_ASSERT_MESSAGE("Expected 419 features", 419 == count);
        //Why are we executing again? Just to test we're not retaining state of any kind between command executions
        count = 0;
        reader = selectCmd->Execute();
        exprReader = dynamic_cast<FdoExpressionEngineUtilFeatureReader*>(reader.p);
        CPPUNIT_ASSERT_MESSAGE("Expected a plain feature reader", NULL == exprReader);
        while (reader->ReadNext())
        {
            count++;
        }
        reader->Close();
        CPPUNIT_ASSERT_MESSAGE("Expected 419 features", 419 == count);
        conn->Close();
    }
    catch (FdoException* ex)
    {
        TestCommonFail(ex);
    }
}

void SelectTests::TestCase_SelectAllExplicitPropertyList()
{
    try
    {
        FdoPtr<FdoIConnection> conn = UnitTestUtil::CreateOgrConnection(L"../../TestData/World_Countries/World_Countries.tab");
        FdoConnectionState state = conn->Open();
        CPPUNIT_ASSERT_MESSAGE("Expected open state", state == FdoConnectionState_Open);

        FdoPtr<FdoISelect> selectCmd = static_cast<FdoISelect*>(conn->CreateCommand(FdoCommandType_Select));
        selectCmd->SetFeatureClassName(L"World_Countries");

        FdoPtr<FdoIdentifierCollection> propNames = selectCmd->GetPropertyNames();
    
        FdoPtr<FdoIdentifier> prop1 = FdoIdentifier::Create(L"FID");
        FdoPtr<FdoIdentifier> prop2 = FdoIdentifier::Create(L"GEOMETRY");
        FdoPtr<FdoIdentifier> prop3 = FdoIdentifier::Create(L"NAME");
        propNames->Add(prop1);
        propNames->Add(prop2);
        propNames->Add(prop3);

        FdoPtr<FdoIFeatureReader> reader = selectCmd->Execute();
        FdoExpressionEngineUtilFeatureReader* exprReader = dynamic_cast<FdoExpressionEngineUtilFeatureReader*>(reader.p);
        CPPUNIT_ASSERT_MESSAGE("Expected a plain feature reader", NULL == exprReader);
        FdoInt32 count = 0;
        FdoPtr<FdoClassDefinition> clsDef = reader->GetClassDefinition();
        FdoPtr<FdoPropertyDefinitionCollection> clsProps = clsDef->GetProperties();
        CPPUNIT_ASSERT_MESSAGE("Expected 3 properties in reader", 3 == clsProps->GetCount());
        while (reader->ReadNext())
        {
            count++;
        }
        reader->Close();
        conn->Close();
        CPPUNIT_ASSERT_MESSAGE("Expected 419 features", 419 == count);
    }
    catch (FdoException* ex)
    {
        TestCommonFail(ex);
    }
}

void SelectTests::TestCase_SelectAllComputedProperties()
{
    try
    {
        FdoPtr<FdoIConnection> conn = UnitTestUtil::CreateOgrConnection(L"../../TestData/World_Countries/World_Countries.tab");
        FdoConnectionState state = conn->Open();
        CPPUNIT_ASSERT_MESSAGE("Expected open state", state == FdoConnectionState_Open);

        FdoPtr<FdoISelect> selectCmd = static_cast<FdoISelect*>(conn->CreateCommand(FdoCommandType_Select));
        selectCmd->SetFeatureClassName(L"World_Countries");

        FdoPtr<FdoIdentifierCollection> propNames = selectCmd->GetPropertyNames();
    
        FdoPtr<FdoIdentifier> prop1 = FdoIdentifier::Create(L"FID");
        FdoPtr<FdoIdentifier> prop2 = FdoIdentifier::Create(L"GEOMETRY");
        FdoPtr<FdoIdentifier> prop3 = FdoIdentifier::Create(L"NAME");
        propNames->Add(prop1);
        propNames->Add(prop2);
        propNames->Add(prop3);

        FdoPtr<FdoExpression> expr1 = FdoExpression::Parse(L"CurrentDate()");
        FdoPtr<FdoExpression> expr2 = FdoExpression::Parse(L"Area2D(GEOMETRY)");
        FdoPtr<FdoExpression> expr3 = FdoExpression::Parse(L"Round(Area2D(GEOMETRY))");
        FdoPtr<FdoExpression> expr4 = FdoExpression::Parse(L"Lower(NAME)");
        FdoPtr<FdoExpression> expr5 = FdoExpression::Parse(L"Upper(NAME)");
        FdoPtr<FdoExpression> expr6 = FdoExpression::Parse(L"Soundex(NAME)");

        FdoPtr<FdoComputedIdentifier> comp1 = FdoComputedIdentifier::Create(L"CURRENT_DATE" , expr1);
        FdoPtr<FdoComputedIdentifier> comp2 = FdoComputedIdentifier::Create(L"AREA"         , expr2);
        FdoPtr<FdoComputedIdentifier> comp3 = FdoComputedIdentifier::Create(L"AREA_ROUNDED" , expr3);
        FdoPtr<FdoComputedIdentifier> comp4 = FdoComputedIdentifier::Create(L"NAME_LOWER"   , expr4);
        FdoPtr<FdoComputedIdentifier> comp5 = FdoComputedIdentifier::Create(L"NAME_UPPER"   , expr5);
        FdoPtr<FdoComputedIdentifier> comp6 = FdoComputedIdentifier::Create(L"NAME_SOUNDEX" , expr6);
        propNames->Add(comp1);
        propNames->Add(comp2);
        propNames->Add(comp3);
        propNames->Add(comp4);
        propNames->Add(comp5);
        propNames->Add(comp6);

        FdoPtr<FdoIFeatureReader> reader = selectCmd->Execute();
        FdoExpressionEngineUtilFeatureReader* exprReader = dynamic_cast<FdoExpressionEngineUtilFeatureReader*>(reader.p);
        CPPUNIT_ASSERT_MESSAGE("Expected an EE feature reader", NULL != exprReader);
        FdoInt32 count = 0;
        FdoPtr<FdoClassDefinition> clsDef = reader->GetClassDefinition();
        FdoPtr<FdoPropertyDefinitionCollection> clsProps = clsDef->GetProperties();
        CPPUNIT_ASSERT_MESSAGE("Expected 9 properties in reader", 9 == clsProps->GetCount());
        while (reader->ReadNext())
        {
            FdoDateTime dtVal = reader->GetDateTime(L"CURRENT_DATE");
            double area = reader->GetDouble(L"AREA");
            double areaRd = reader->GetDouble(L"AREA_ROUNDED");
            //Multiple calls to various computed properties of the same underlying property seem
            //to return the same pointer (ie. The content will be the value of the last GetString() call). 
            //
            //Is this by design? Either way, copying the contents to separate string objects before invoking 
            //the next GetString() call and inspecting them in a debugger verifies that the strings that we 
            //get back are indeed different.
            std::wstring nameLower;
            std::wstring nameUpper;
            std::wstring nameSoundex;
            std::wstring name;
            nameLower += reader->GetString(L"NAME_LOWER");
            nameUpper += reader->GetString(L"NAME_UPPER");
            name += reader->GetString(L"NAME");
            nameSoundex += reader->GetString(L"NAME_SOUNDEX");
            count++;
        }
        reader->Close();
        conn->Close();
        CPPUNIT_ASSERT_MESSAGE("Expected 419 features", 419 == count);
    }
    catch (FdoException* ex)
    {
        TestCommonFail(ex);
    }
}

void SelectTests::TestCase_SelectAggregateCount()
{
    try
    {
        FdoPtr<FdoIConnection> conn = UnitTestUtil::CreateOgrConnection(L"../../TestData/World_Countries/World_Countries.tab");
        FdoConnectionState state = conn->Open();
        CPPUNIT_ASSERT_MESSAGE("Expected open state", state == FdoConnectionState_Open);

        FdoPtr<FdoISelectAggregates> selectCmd = static_cast<FdoISelectAggregates*>(conn->CreateCommand(FdoCommandType_SelectAggregates));
        selectCmd->SetFeatureClassName(L"World_Countries");

        FdoPtr<FdoExpression> expr1 = FdoExpression::Parse(L"Count(FID)");
        FdoPtr<FdoComputedIdentifier> comp1 = FdoComputedIdentifier::Create(L"TOTAL_COUNT" , expr1);
    
        FdoPtr<FdoIdentifierCollection> propNames = selectCmd->GetPropertyNames();
        propNames->Add(comp1);

        FdoPtr<FdoIDataReader> reader = selectCmd->Execute();
        FdoExpressionEngineUtilDataReader* exprReader = dynamic_cast<FdoExpressionEngineUtilDataReader*>(reader.p);
        CPPUNIT_ASSERT_MESSAGE("Expected a plain data reader", NULL == exprReader);
        FdoInt64 total = 0L;
        while (reader->ReadNext())
        {
            total += reader->GetInt64(L"TOTAL_COUNT");
        }
        reader->Close();
        CPPUNIT_ASSERT_MESSAGE("Expected total of 419", 419 == total);
        
        //Now try with filter. This will trigger the Expression Engine code path
        FdoPtr<FdoFilter> filter = FdoFilter::Parse(L"NAME = 'Canada'");
        selectCmd->SetFilter(filter);
        reader = selectCmd->Execute();
        exprReader = dynamic_cast<FdoExpressionEngineUtilDataReader*>(reader.p);
        CPPUNIT_ASSERT_MESSAGE("Expected an EE data reader", NULL != exprReader);
        total = 0L;
        while (reader->ReadNext())
        {
            total += reader->GetInt64(L"TOTAL_COUNT");
        }
        reader->Close();
        conn->Close();
        CPPUNIT_ASSERT_MESSAGE("Expected total of 66", 66 == total);
    }
    catch (FdoException* ex)
    {
        TestCommonFail(ex);
    }
}

void SelectTests::TestCase_SelectAggregateDistinct()
{
    try
    {
        FdoPtr<FdoIConnection> conn = UnitTestUtil::CreateOgrConnection(L"../../TestData/World_Countries/World_Countries.tab");
        FdoConnectionState state = conn->Open();
        CPPUNIT_ASSERT_MESSAGE("Expected open state", state == FdoConnectionState_Open);

        FdoPtr<FdoISelectAggregates> selectCmd = static_cast<FdoISelectAggregates*>(conn->CreateCommand(FdoCommandType_SelectAggregates));
        selectCmd->SetFeatureClassName(L"World_Countries");
        selectCmd->SetDistinct(true);

        FdoPtr<FdoIdentifier> ident = FdoIdentifier::Create(L"KEY");
        FdoPtr<FdoIdentifierCollection> propNames = selectCmd->GetPropertyNames();
        propNames->Add(ident);

        FdoPtr<FdoIDataReader> reader = selectCmd->Execute();
        FdoExpressionEngineUtilDataReader* exprReader = dynamic_cast<FdoExpressionEngineUtilDataReader*>(reader.p);
        CPPUNIT_ASSERT_MESSAGE("Expected a plain data reader", NULL == exprReader);
        FdoInt32 total = 0;
        while (reader->ReadNext())
        {
            FdoString* key = reader->GetString(L"KEY");
            total++;
        }
        reader->Close();
        CPPUNIT_ASSERT_MESSAGE("Expected total of 175", 175 == total);

        //Now try with filter. This will trigger the Expression Engine code path
        propNames->Clear();
        ident = FdoIdentifier::Create(L"MAPKEY");
        propNames->Add(ident);
        FdoPtr<FdoFilter> filter = FdoFilter::Parse(L"NAME = 'Canada'");
        selectCmd->SetFilter(filter);
        reader = selectCmd->Execute();
        exprReader = dynamic_cast<FdoExpressionEngineUtilDataReader*>(reader.p);
        CPPUNIT_ASSERT_MESSAGE("Expected an EE data reader", NULL != exprReader);
        total = 0;
        while (reader->ReadNext())
        {
            FdoString* mapkey = reader->GetString(L"MAPKEY");
            total++;
        }
        reader->Close();
        conn->Close();
        CPPUNIT_ASSERT_MESSAGE("Expected total of 66", 66 == total);
    }
    catch (FdoException* ex)
    {
        TestCommonFail(ex);
    }
}

void SelectTests::TestCase_SelectAggregateSpatialExtent()
{
    try
    {
        FdoPtr<FdoIConnection> conn = UnitTestUtil::CreateOgrConnection(L"../../TestData/World_Countries/World_Countries.tab");
        FdoConnectionState state = conn->Open();
        CPPUNIT_ASSERT_MESSAGE("Expected open state", state == FdoConnectionState_Open);

        FdoPtr<FdoISelectAggregates> selectCmd = static_cast<FdoISelectAggregates*>(conn->CreateCommand(FdoCommandType_SelectAggregates));
        selectCmd->SetFeatureClassName(L"World_Countries");

        FdoPtr<FdoExpression> expr1 = FdoExpression::Parse(L"SpatialExtents(GEOMETRY)");
        FdoPtr<FdoComputedIdentifier> comp1 = FdoComputedIdentifier::Create(L"EXTENTS" , expr1);
    
        FdoPtr<FdoIdentifierCollection> propNames = selectCmd->GetPropertyNames();
        propNames->Add(comp1);

        FdoPtr<FdoIDataReader> reader = selectCmd->Execute();
        FdoExpressionEngineUtilDataReader* exprReader = dynamic_cast<FdoExpressionEngineUtilDataReader*>(reader.p);
        CPPUNIT_ASSERT_MESSAGE("Expected a plain data reader", NULL == exprReader);
        FdoInt32 iterations = 0;
        FdoPtr<FdoByteArray> fgf;
        while (reader->ReadNext())
        {
            fgf = reader->GetGeometry(L"EXTENTS");
            iterations++;
        }
        reader->Close();
        conn->Close();
        CPPUNIT_ASSERT_MESSAGE("Expected non-null extents", NULL != fgf.p);
        CPPUNIT_ASSERT_MESSAGE("Expected 1 iteration", 1 == iterations);
        FdoPtr<FdoFgfGeometryFactory> geomFact = FdoFgfGeometryFactory::GetInstance();
        FdoPtr<FdoIGeometry> geom = geomFact->CreateGeometryFromFgf(fgf);
        FdoPtr<FdoIEnvelope> env = geom->GetEnvelope();
        printf("SpatialExtents() returned (%f, %f) (%f, %f)\n", env->GetMinX(), env->GetMinY(), env->GetMaxX(), env->GetMaxY());
    }
    catch (FdoException* ex)
    {
        TestCommonFail(ex);
    }
}
