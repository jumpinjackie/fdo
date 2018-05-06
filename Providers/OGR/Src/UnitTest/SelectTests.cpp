#include "SelectTests.h"
#include "UnitTestUtil.h"
#include "Fdo.h"
#include "TestCommonMiscUtil.h"
#include "FdoExpressionEngine.h"
#include "FdoExpressionEngineFunctionCollection.h"
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

void SelectTests::TestCase_EvalQuotedIdentifier()
{
    try
    {
        FdoPtr<FdoIConnection> conn = UnitTestUtil::CreateOgrConnection(L"../../TestData/World_Countries/World_Countries.tab");
        FdoConnectionState state = conn->Open();
        CPPUNIT_ASSERT_MESSAGE("Expected open state", state == FdoConnectionState_Open);

        FdoPtr<FdoISelect> selectCmd = static_cast<FdoISelect*>(conn->CreateCommand(FdoCommandType_Select));
        selectCmd->SetFeatureClassName(L"World_Countries");

        FdoPtr<FdoIFeatureReader> reader = selectCmd->Execute();
        FdoPtr<FdoExpressionEngineFunctionCollection> functions = FdoExpressionEngineFunctionCollection::Create();
        FdoPtr<FdoClassDefinition> clsDef = reader->GetClassDefinition();

        FdoPtr<FdoExpressionEngine> exec = FdoExpressionEngine::Create(reader, clsDef, functions);
        // The string that's giving us grief
        FdoPtr<FdoExpression> expr = FdoExpression::Parse(L"\"Something like this 'bla bla' R. G. 'bla bla'\"");
        FdoPtr<FdoLiteralValue> lval = exec->Evaluate(expr); //Boom?

        CPPUNIT_FAIL("Should've thrown FdoException (identifier not found)");
    }
    catch (FdoException* ex) //The right kind of boom, not segfault boom
    {
        FdoStringP msg = ex->GetExceptionMessage();
        FDO_SAFE_RELEASE(ex);
        //It should be complaining about some part of this string
        CPPUNIT_ASSERT(msg.Contains(L"bla bla"));
    }
}

void SelectTests::TestCase_SelectMixedAttributeAndSpatialFilter()
{
    try
    {
        FdoPtr<FdoIConnection> conn = UnitTestUtil::CreateOgrConnection(L"../../TestData/World_Countries/World_Countries.tab");
        FdoConnectionState state = conn->Open();
        CPPUNIT_ASSERT_MESSAGE("Expected open state", state == FdoConnectionState_Open);

        FdoPtr<FdoISelect> selectCmd = static_cast<FdoISelect*>(conn->CreateCommand(FdoCommandType_Select));
        
        FdoStringP wkt = L"POLYGON ((32.95257287189878781 17.52213437146155073, 17.64294717229646636 15.85994643836187024, 11.95651476958703086 4.22463090666410324, 15.28089063578639184 -1.11186719434013526, 21.5797080664799239 -3.73637445712910932, 32.69012214561989538 -5.22359523937618775, 36.88933376608224535 0.90025504046474225, 40.03874248142901138 9.56112900766834173, 32.95257287189878781 17.52213437146155073))";
        
        FdoPtr<FdoFgfGeometryFactory> geometryFactory = FdoFgfGeometryFactory::GetInstance();
        FdoPtr<FdoIGeometry> geom = geometryFactory->CreateGeometry(wkt);
        FdoPtr<FdoByteArray> fgf = geometryFactory->GetFgf(geom);
        FdoPtr<FdoGeometryValue> geomVal = FdoGeometryValue::Create(fgf);

        FdoPtr<FdoSpatialCondition> spatialCondition = FdoSpatialCondition::Create(L"GEOMETRY", FdoSpatialOperations_Intersects, geomVal);
        FdoPtr<FdoFilter> regularFilter = FdoFilter::Parse(L"NAME LIKE 'C%'");
        FdoPtr<FdoFilter> combinedFilter = FdoFilter::Combine(regularFilter, FdoBinaryLogicalOperations_And, spatialCondition);
        selectCmd->SetFilter(combinedFilter);

        selectCmd->SetFeatureClassName(L"World_Countries");

        FdoPtr<FdoIFeatureReader> reader = selectCmd->Execute();
        FdoInt32 count = 0;
        while (reader->ReadNext())
        {
            CPPUNIT_ASSERT(!reader->IsNull(L"NAME"));
            FdoString* name = reader->GetString(L"NAME");
            printf("Country: %S\n", name);
            count++;
        }
        reader->Close();
        CPPUNIT_ASSERT(count == 4);
    }
    catch (FdoException* ex)
    {
        TestCommonFail(ex);
    }
}

void SelectTests::TestCase_SelectWithBadClassName()
{
    try 
    {
        FdoPtr<FdoIConnection> conn = UnitTestUtil::CreateOgrConnection(L"../../TestData/World_Countries/World_Countries.tab");
        FdoConnectionState state = conn->Open();
        CPPUNIT_ASSERT_MESSAGE("Expected open state", state == FdoConnectionState_Open);

        FdoPtr<FdoISelect> selectCmd = static_cast<FdoISelect*>(conn->CreateCommand(FdoCommandType_Select));
        selectCmd->SetFeatureClassName(L"World_C");

        FdoPtr<FdoIFeatureReader> reader = selectCmd->Execute();
        reader->Close();
        CPPUNIT_FAIL("Execpted select to throw");
    }
    catch (FdoException* ex) 
    {
        FdoStringP msg = ex->GetExceptionMessage();
        FDO_SAFE_RELEASE(ex);
        CPPUNIT_ASSERT(msg == L"Class not found: World_C");
    }
}

void SelectTests::TestCase_SelectAggregatesWithBadClassName()
{
    try
    {
        FdoPtr<FdoIConnection> conn = UnitTestUtil::CreateOgrConnection(L"../../TestData/World_Countries/World_Countries.tab");
        FdoConnectionState state = conn->Open();
        CPPUNIT_ASSERT_MESSAGE("Expected open state", state == FdoConnectionState_Open);

        FdoPtr<FdoISelectAggregates> selectCmd = static_cast<FdoISelectAggregates*>(conn->CreateCommand(FdoCommandType_SelectAggregates));
        selectCmd->SetFeatureClassName(L"World_C");

        FdoPtr<FdoIDataReader> reader = selectCmd->Execute();
        reader->Close();
        CPPUNIT_FAIL("Execpted select to throw");
    }
    catch (FdoException* ex)
    {
        FdoStringP msg = ex->GetExceptionMessage();
        FDO_SAFE_RELEASE(ex);
        CPPUNIT_ASSERT(msg == L"Class not found: World_C");
    }
}

void SelectTests::TestCase_SelectWithBadPropertyNames()
{
    try
    {
        FdoPtr<FdoIConnection> conn = UnitTestUtil::CreateOgrConnection(L"../../TestData/World_Countries/World_Countries.tab");
        FdoConnectionState state = conn->Open();
        CPPUNIT_ASSERT_MESSAGE("Expected open state", state == FdoConnectionState_Open);

        FdoPtr<FdoISelect> selectCmd = static_cast<FdoISelect*>(conn->CreateCommand(FdoCommandType_Select));
        selectCmd->SetFeatureClassName(L"World_Countries");

        FdoPtr<FdoIdentifierCollection> propNames = selectCmd->GetPropertyNames();

        FdoPtr<FdoIdentifier> prop1 = FdoIdentifier::Create(L"Foobar");;
        propNames->Add(prop1);

        FdoPtr<FdoIFeatureReader> reader = selectCmd->Execute();
        reader->Close();
        CPPUNIT_FAIL("Expected this to throw");
    }
    catch (FdoException* ex)
    {
        FdoStringP msg = ex->GetExceptionMessage();
        FDO_SAFE_RELEASE(ex);
        CPPUNIT_ASSERT(msg == L"Property not found: Foobar");
    }
}

void SelectTests::TestCase_SelectWithComputedPropertiesReferencingBadPropertyNames()
{
    try
    {
        FdoPtr<FdoIConnection> conn = UnitTestUtil::CreateOgrConnection(L"../../TestData/World_Countries/World_Countries.tab");
        FdoConnectionState state = conn->Open();
        CPPUNIT_ASSERT_MESSAGE("Expected open state", state == FdoConnectionState_Open);

        FdoPtr<FdoISelect> selectCmd = static_cast<FdoISelect*>(conn->CreateCommand(FdoCommandType_Select));
        selectCmd->SetFeatureClassName(L"World_Countries");

        FdoPtr<FdoIdentifierCollection> propNames = selectCmd->GetPropertyNames();

        FdoPtr<FdoExpression> expr = FdoExpression::Parse(L"Area2D(Geom)");

        FdoPtr<FdoComputedIdentifier> comp = FdoComputedIdentifier::Create(L"AREA", expr);

        propNames->Add(comp);

        FdoPtr<FdoIFeatureReader> reader = selectCmd->Execute();
        reader->Close();
        CPPUNIT_FAIL("Expected this to throw");
    }
    catch (FdoException* ex)
    {
        FdoStringP msg = ex->GetExceptionMessage();
        FDO_SAFE_RELEASE(ex);
        CPPUNIT_ASSERT(msg == L"Property not found: Geom");
    }
}

void SelectTests::TestCase_SelectAggregateWithBadPropertyNames()
{
    try
    {
        FdoPtr<FdoIConnection> conn = UnitTestUtil::CreateOgrConnection(L"../../TestData/World_Countries/World_Countries.tab");
        FdoConnectionState state = conn->Open();
        CPPUNIT_ASSERT_MESSAGE("Expected open state", state == FdoConnectionState_Open);

        FdoPtr<FdoISelectAggregates> selectCmd = static_cast<FdoISelectAggregates*>(conn->CreateCommand(FdoCommandType_SelectAggregates));
        selectCmd->SetFeatureClassName(L"World_Countries");

        FdoPtr<FdoIdentifierCollection> propNames = selectCmd->GetPropertyNames();

        FdoPtr<FdoIdentifier> prop1 = FdoIdentifier::Create(L"Foobar");;
        propNames->Add(prop1);

        FdoPtr<FdoIDataReader> reader = selectCmd->Execute();
        reader->Close();
        CPPUNIT_FAIL("Expected this to throw");
    }
    catch (FdoException* ex)
    {
        FdoStringP msg = ex->GetExceptionMessage();
        FDO_SAFE_RELEASE(ex);
        CPPUNIT_ASSERT(msg == L"Property not found: Foobar");
    }
}

void SelectTests::TestCase_SelectAggregateWithBadSpatialExtentExpr()
{
    try
    {
        FdoPtr<FdoIConnection> conn = UnitTestUtil::CreateOgrConnection(L"../../TestData/World_Countries/World_Countries.tab");
        FdoConnectionState state = conn->Open();
        CPPUNIT_ASSERT_MESSAGE("Expected open state", state == FdoConnectionState_Open);

        FdoPtr<FdoISelectAggregates> selectCmd = static_cast<FdoISelectAggregates*>(conn->CreateCommand(FdoCommandType_SelectAggregates));
        selectCmd->SetFeatureClassName(L"World_Countries");

        FdoPtr<FdoExpression> expr1 = FdoExpression::Parse(L"SpatialExtents(Geom)");
        FdoPtr<FdoComputedIdentifier> comp1 = FdoComputedIdentifier::Create(L"EXTENTS", expr1);

        FdoPtr<FdoIdentifierCollection> propNames = selectCmd->GetPropertyNames();
        propNames->Add(comp1);

        FdoPtr<FdoIDataReader> reader = selectCmd->Execute();
        reader->Close();
        CPPUNIT_FAIL("Expected this to throw");
    }
    catch (FdoException* ex)
    {
        FdoStringP msg = ex->GetExceptionMessage();
        FDO_SAFE_RELEASE(ex);
        CPPUNIT_ASSERT(msg == L"Property not found: Geom");
    }
}

void SelectTests::TestCase_SelectAggregateWithBadCountExpr()
{
    try
    {
        FdoPtr<FdoIConnection> conn = UnitTestUtil::CreateOgrConnection(L"../../TestData/World_Countries/World_Countries.tab");
        FdoConnectionState state = conn->Open();
        CPPUNIT_ASSERT_MESSAGE("Expected open state", state == FdoConnectionState_Open);

        FdoPtr<FdoISelectAggregates> selectCmd = static_cast<FdoISelectAggregates*>(conn->CreateCommand(FdoCommandType_SelectAggregates));
        selectCmd->SetFeatureClassName(L"World_Countries");

        FdoPtr<FdoExpression> expr1 = FdoExpression::Parse(L"Count(IDontExist)");
        FdoPtr<FdoComputedIdentifier> comp1 = FdoComputedIdentifier::Create(L"TOTAL_COUNT", expr1);

        FdoPtr<FdoIdentifierCollection> propNames = selectCmd->GetPropertyNames();
        propNames->Add(comp1);

        FdoPtr<FdoIDataReader> reader = selectCmd->Execute();
        reader->Close();
        CPPUNIT_FAIL("Expected this to throw");
    }
    catch (FdoException* ex)
    {
        FdoStringP msg = ex->GetExceptionMessage();
        FDO_SAFE_RELEASE(ex);
        CPPUNIT_ASSERT(msg == L"Property not found: IDontExist");
    }
}

void SelectTests::TestCase_SelectBadReaderProperties()
{
    try
    {
        FdoPtr<FdoIConnection> conn = UnitTestUtil::CreateOgrConnection(L"../../TestData/World_Countries/World_Countries.tab");
        FdoConnectionState state = conn->Open();
        CPPUNIT_ASSERT_MESSAGE("Expected open state", state == FdoConnectionState_Open);

        FdoPtr<FdoISelect> selectCmd = static_cast<FdoISelect*>(conn->CreateCommand(FdoCommandType_Select));
        selectCmd->SetFeatureClassName(L"World_Countries");

        FdoPtr<FdoIFeatureReader> reader = selectCmd->Execute();
        if (reader->ReadNext())
        {
            try
            {
                reader->GetPropertyIndex(L"IDontExist");
            }
            catch (FdoException* ex)
            {
                FdoStringP msg = ex->GetExceptionMessage();
                FDO_SAFE_RELEASE(ex);
                CPPUNIT_ASSERT(msg == L"The property 'IDontExist' was not found. ");
            }

            try
            {
                reader->GetString(L"Foobar");
                CPPUNIT_FAIL("Reader should've thrown");
            }
            catch (FdoCommandException* ex)
            {
                FdoStringP msg = ex->GetExceptionMessage();
                FDO_SAFE_RELEASE(ex);
            }
        }
        reader->Close();
        conn->Close();
    }
    catch (FdoException* ex)
    {
        TestCommonFail(ex);
    }
}

void SelectTests::TestCase_SelectAggregateBadReaderProperties()
{
    try
    {
        FdoPtr<FdoIConnection> conn = UnitTestUtil::CreateOgrConnection(L"../../TestData/World_Countries/World_Countries.tab");
        FdoConnectionState state = conn->Open();
        CPPUNIT_ASSERT_MESSAGE("Expected open state", state == FdoConnectionState_Open);

        FdoPtr<FdoISelectAggregates> selectCmd = static_cast<FdoISelectAggregates*>(conn->CreateCommand(FdoCommandType_SelectAggregates));
        selectCmd->SetFeatureClassName(L"World_Countries");

        FdoPtr<FdoIDataReader> reader = selectCmd->Execute();
        if (reader->ReadNext())
        {
            try
            {
                reader->GetDataType(L"IDontExist");
            }
            catch (FdoException* ex)
            {
                FdoStringP msg = ex->GetExceptionMessage();
                FDO_SAFE_RELEASE(ex);
                CPPUNIT_ASSERT(msg == L"The property 'IDontExist' was not found. ");
            }

            try
            {
                reader->GetString(L"Foobar");
                CPPUNIT_FAIL("Reader should've thrown");
            }
            catch (FdoException* ex)
            {
                FdoStringP msg = ex->GetExceptionMessage();
                FDO_SAFE_RELEASE(ex);
                CPPUNIT_ASSERT(msg == L"The property 'Foobar' was not found. ");
            }
        }
        reader->Close();
        conn->Close();
    }
    catch (FdoException* ex)
    {
        TestCommonFail(ex);
    }
}