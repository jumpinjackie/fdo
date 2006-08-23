// Copyright (C) 2004-2006  Autodesk, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

// This code is provided just for an example. You should delete it from
// the project once you are ready to compile your test suite.  More information
// about using CppUnit can be found on the CppUnit homepage.
// http://cppunit.sourceforge.net/doc/lastest/index.html

// List of all modules can be found here:
// http://cppunit.sourceforge.net/doc/lastest/modules.html

// Below are the macros for all Assertions available with CppUnit. If you want to create your own assertion
// go to: http://cppunit.sourceforge.net/doc/lastest/group___creating_new_assertions.html

/*
CPPUNIT_ASSERT(condition) 
  Assertions that a condition is true. 

CPPUNIT_ASSERT_MESSAGE(message, condition) 
  Assertion with a user specified message. 

CPPUNIT_FAIL(message) 
  Fails with the specified message. 

CPPUNIT_ASSERT_EQUAL(expected, actual) 
  Asserts that two values are equals. 
 
CPPUNIT_ASSERT_EQUAL_MESSAGE(message, expected, actual) 
  Asserts that two values are equals, provides additional messafe on failure. 
 
CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, actual, delta) 
  Macro for primitive value comparisons. 
 
CPPUNIT_ASSERT_THROW(expression, ExceptionType) 
  Asserts that the given expression throws an exception of the specified type. 
 
CPPUNIT_ASSERT_NO_THROW(expression) 
  Asserts that the given expression does not throw any exceptions. 
 
CPPUNIT_ASSERT_ASSERTION_FAIL(assertion)   CPPUNIT_ASSERT_THROW( assertion, CPPUNIT_NS::Exception ) 
  Asserts that an assertion fail. 
 
CPPUNIT_ASSERT_ASSERTION_PASS(assertion)   CPPUNIT_ASSERT_NO_THROW( assertion ) 
  Asserts that an assertion pass. 
*/

#include <ctime>
#include "MasterTest.h"
#include "UnitTestUtil.h"
#include <cppunit/extensions/HelperMacros.h>
const wchar_t* DEST_PATH = L"../../TestData/PARCEL_linuxtest.SDX";
const wchar_t* SHP_PATH = L"../../TestData/World_Countries.sdf";
const wchar_t* SHP_PATH2 = L"../../TestData/province.sdf";

// Replace the text "TestExample" with your own class name
CPPUNIT_TEST_SUITE_REGISTRATION(MasterTest);


MasterTest::MasterTest()
{

}


MasterTest::~MasterTest()
{
}

FdoIConnection* MasterTest::CreateConnection()
{
    FdoPtr<IConnectionManager> manager = FdoFeatureAccessManager::GetConnectionManager ();
    return (manager->CreateConnection (L"OSGeo.SDF.3.2"));
}

// This method is executed at the beginning of each test case.
// Use it to initialize any variables or objects that you will
// need for multiple test cases.
void MasterTest::setUp()
{

}

// This method is executed at the end of each test case.  It's
// purpose is to deallocate anything that was allocated in the
// setUp() method.
void MasterTest::tearDown()
{
}


void MasterTest::openConnection(FdoIConnection* conn, const wchar_t* path, bool readonly)
{
#ifdef _WIN32
    wchar_t fullpath[1024];
    _wfullpath(fullpath, path, 1024);
#else
    char cpath[1024];
    char cfullpath[1024];
    wcstombs(cpath, path, 1024);
    realpath(cpath, cfullpath);
    wchar_t fullpath[1024];
    mbstowcs(fullpath, cfullpath, 1024);
#endif

    std::wstring connStr = std::wstring(L"File=") + std::wstring(fullpath);
    if (readonly)
        connStr += std::wstring(L";ReadOnly=TRUE");

    conn->SetConnectionString(connStr.c_str());
    FdoPtr<FdoIConnectionInfo>info = conn->GetConnectionInfo();
    FdoPtr<FdoIConnectionPropertyDictionary> prop = info->GetConnectionProperties();
    conn->Open();
}


void MasterTest::deleteTest()
{
    FdoPtr<FdoIConnection> conn;

    try {
        conn = CreateConnection();

        openConnection(conn, DEST_PATH);    
       
        FdoPtr<FdoIDelete> del = (FdoIDelete*)conn->CreateCommand(FdoCommandType_Delete); 

        del->SetFeatureClassName(L"DaKlass");

        //delete two features from the thing
        FdoPtr<FdoFilter> filter = FdoFilter::Parse(L"(Key = 'DI0022') or (Key = 'LN0316')");

        del->SetFilter(filter);
        
        int count = del->Execute();

        printf ("\nDelete test, number of features deleted = %d\n", count);
        CPPUNIT_ASSERT(count == 2);

        conn->Close();
    }
    catch ( FdoException* ex ) {
        UnitTestUtil::FailOnException( ex );
    }
}


void MasterTest::updateTest()
{
    FdoPtr<FdoIConnection> conn;

    try {
        conn = CreateConnection();

        openConnection(conn, DEST_PATH);    


        FdoPtr<FdoIUpdate> update = (FdoIUpdate*)conn->CreateCommand(FdoCommandType_Update); 

        update->SetFeatureClassName(L"DaKlass");

        FdoPtr<FdoFilter> filter = FdoFilter::Parse(L"(Key = 'DI0022')");// or (Key = 'LN0316')");

        update->SetFilter(filter);

        FdoPtr<FdoStringValue> sval = FdoStringValue::Create(L"Cool");
        FdoPtr<FdoPropertyValue> pv = FdoPropertyValue::Create(L"Name", sval);

        FdoPtr<FdoPropertyValueCollection> pvc = update->GetPropertyValues();

        pvc->Add(pv);

        printf("\n\n\nTesting Update");
           
        int count = update->Execute();


        conn->Close();
    }
    catch ( FdoException* ex ) {
        UnitTestUtil::FailOnException( ex );
    }
}

void MasterTest::concurencyTest()
{
    FdoPtr<FdoIConnection> update_conn;
    
    try {
        update_conn = CreateConnection();
        openConnection(update_conn, DEST_PATH);    


        FdoPtr<FdoIUpdate> update = (FdoIUpdate*)update_conn->CreateCommand(FdoCommandType_Update); 

        update->SetFeatureClassName(L"DaKlass");

        FdoPtr<FdoFilter> filter = FdoFilter::Parse(L"(Key = 'DI0022')");// or (Key = 'LN0316')");

        update->SetFilter(filter);

        FdoPtr<FdoStringValue> sval = FdoStringValue::Create(L"Cool");
        FdoPtr<FdoPropertyValue> pv = FdoPropertyValue::Create(L"Name", sval);

        FdoPtr<FdoPropertyValueCollection> pvc = update->GetPropertyValues();

        pvc->Add(pv);

        printf("\n\n\nTesting Update");
           
        int count = update->Execute();

	    printf("\n\nUpdated %d features\n", count);

        update_conn->Close();
    }
    catch ( FdoException* ex ) {
        UnitTestUtil::FailOnException( ex );
    }
}

void MasterTest::keyFilterBeforeDelete()
{
    FdoPtr<FdoIConnection> conn = CreateConnection();

    openConnection(conn, DEST_PATH);
       
    FdoPtr<FdoISelect> select = (FdoISelect*)conn->CreateCommand(FdoCommandType_Select); 

    select->SetFeatureClassName(L"DaKlass");

    //FdoPtr<FdoFilter> filter = FdoFilter::Parse(L"(Key = 'DI0022') or (Key = 'LN0316')");
    FdoPtr<FdoFilter> filter = FdoFilter::Parse(L"Key LIKE 'DI%' or Key = 'LN0316' or Key = 'DI0022'");


    select->SetFilter(filter);
    
    FdoPtr<FdoIFeatureReader> rdr = select->Execute();

    int count2 = 0;

    FdoPtr<FdoByteArray> geom;
    
    printf("\n\n\nKey Filter");
    
    char tmp[1024];
    
    while (rdr->ReadNext())
    {
        const wchar_t* something = rdr->GetString(L"Name");
        wcstombs(tmp, something, 1024);
        printf("%s\n", tmp);
        count2++;
    }


    rdr->Close();

    conn->Close();

    printf ("\nKey filter, number of features returned = %d\n", count2);

    CPPUNIT_ASSERT(count2 == 154);
}


void MasterTest::keyFilterAfterDelete()
{
    FdoPtr<FdoIConnection> conn = CreateConnection();

    openConnection(conn, DEST_PATH);
       
    FdoPtr<FdoISelect> select = (FdoISelect*)conn->CreateCommand(FdoCommandType_Select); 

    select->SetFeatureClassName(L"DaKlass");

    //FdoPtr<FdoFilter> filter = FdoFilter::Parse(L"(Key = 'DI0022') or (Key = 'LN0316')");
    FdoPtr<FdoFilter> filter = FdoFilter::Parse(L"Key LIKE 'DI%' or Key = 'LN0316' or Key = 'DI0022'");


    select->SetFilter(filter);
    
    FdoPtr<FdoIFeatureReader> rdr = select->Execute();

    int count2 = 0;

    FdoPtr<FdoByteArray> geom;
    
    printf("\n\n\nKey Filter Again... after delete");
    
    char tmp[1024];

    while (rdr->ReadNext())
    {
        const wchar_t* something = rdr->GetString(L"Name");
        wcstombs(tmp, something, 1024);
        printf("%s\n", tmp);
        count2++;
    }

    rdr->Close();

    conn->Close();

    printf ("\nKey filter after delete, number of features returned = %d\n", count2);
    CPPUNIT_ASSERT(count2 == 152);
}


void MasterTest::computedPropTest()
{
    FdoPtr<FdoIConnection> conn = CreateConnection();

    openConnection(conn, DEST_PATH);    
   
    FdoPtr<FdoISelect> select = (FdoISelect*)conn->CreateCommand(FdoCommandType_Select); 

    select->SetFeatureClassName(L"DaKlass");


    FdoPtr<FdoIdentifierCollection> propnames = select->GetPropertyNames();

    FdoPtr<FdoExpression> expr = FdoExpression::Parse(L"2+3");
    FdoPtr<FdoComputedIdentifier> ci = FdoComputedIdentifier::Create(L"ComputedInt", expr);
    propnames->Add(ci);


    //computed identifier used as filter... hehe.
    FdoPtr<FdoFilter> filter = FdoFilter::Parse(L"(Name) As ComputedString, (ComputedString = 'MO0418') or (ComputedString = 'DE0065')");
    select->SetFilter(filter);

    
    FdoPtr<FdoIFeatureReader> rdr = select->Execute();

    int count2 = 0;

    FdoPtr<FdoByteArray> geom;

    FdoPtr<FdoClassDefinition> classDef = rdr->GetClassDefinition();
       
    char tmp[1024];
    
    while (rdr->ReadNext())
    {

        FdoInt64 something = rdr->GetInt64(L"ComputedInt");
        FdoString* something2 = rdr->GetString(L"Name");

        printf("Computed identifier 1:   %ld\n", something);
        
        wcstombs(tmp, something2, 1024);
        printf("Feature name:   %s\n", tmp);

        try 
        {
            rdr->GetString(L"whoop dee doo");
        }
        catch (FdoException* e)
        {
            printf("correctly caught exception about non-existing property\n");
            e->Release();
        }

        count2++;
    }


    rdr->Close();

    printf ("\nString filter, number of features returned = %d\n", count2);

    conn->Close();
    
}



void MasterTest::stringFilter()
{
    FdoPtr<FdoIConnection> conn = CreateConnection();

    openConnection(conn, DEST_PATH);    
   
    FdoPtr<FdoISelect> select = (FdoISelect*)conn->CreateCommand(FdoCommandType_Select); 

    select->SetFeatureClassName(L"DaKlass");

    FdoPtr<FdoFilter> filter = FdoFilter::Parse(L"(Name = 'MO0418') or (Name = 'DE0065')");


    select->SetFilter(filter);
    
    FdoPtr<FdoIFeatureReader> rdr = select->Execute();

    int count2 = 0;


    FdoPtr<FdoByteArray> geom;

    char tmp[1024];
    
    while (rdr->ReadNext())
    {

        const wchar_t* something = rdr->GetString(L"Name");

        try 
        {
            rdr->GetString(L"whoop dee doo");
        }
        catch (FdoException* e)
        {
            printf("correctly caught exception about non-existing property\n");
            e->Release();
        }

        wcstombs(tmp, something, 1024);
        printf("%s\n", tmp);

        count2++;
    }


    rdr->Close();

    printf ("\nString filter, number of features returned = %d\n", count2);

    conn->Close();
    
}

void MasterTest::rtreeFilter()
{
    FdoPtr<FdoIConnection> conn = CreateConnection();

    openConnection(conn, DEST_PATH);    
    
    FdoPtr<FdoISelect> select = (FdoISelect*)conn->CreateCommand(FdoCommandType_Select); 

    select->SetFeatureClassName(L"DaKlass");

    //FdoPtr<FdoFilter> filter = FdoFilter::Parse(L"(Name = 'MO0418') or (Name = 'DE0065')");

    
    FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();

    double coords[] = { 7.2230, 43.6861, 
                        7.2505, 43.6861, 
                        7.2505, 43.7134, 
                        7.2230, 43.7134, 
                        7.2230, 43.6861 }; //last pt equals first for rings

    FdoPtr<FdoILinearRing> outer = gf->CreateLinearRing(0, 10, coords);

    FdoPtr<FdoIPolygon> poly = gf->CreatePolygon(outer, NULL);

    FdoPtr<FdoByteArray> polyfgf = gf->GetFgf(poly);
    FdoPtr<FdoGeometryValue> gv = FdoGeometryValue::Create(polyfgf);
    FdoPtr<FdoSpatialCondition> filter = FdoSpatialCondition::Create(L"Data", FdoSpatialOperations_EnvelopeIntersects, gv);
    

    printf("\n\n\nR-Tree filter");

    select->SetFilter(filter);
   
    clock_t start;
    clock_t finish;
    start = clock ();
    FdoPtr<FdoIFeatureReader> rdr = select->Execute();

    int count2 = 0;

    FdoPtr<FdoByteArray> geom;

    while (rdr->ReadNext())
    {
        const wchar_t* something = rdr->GetString(L"Name");
        count2++;
    }

    rdr->Close();
    

    finish = clock ();
    printf("Count = %d  time: %2.3f seconds \n", count2, (double)(finish - start) / CLOCKS_PER_SEC);
    //correct R-Tree will return that many features!
    CPPUNIT_ASSERT(count2 == 4535);


    // Now do the selection again, using the non-default geometry property "Data2".

    select = (FdoISelect*)conn->CreateCommand(FdoCommandType_Select); 
    select->SetFeatureClassName(L"DaKlass");
    gv = FdoGeometryValue::Create(polyfgf);
    filter = FdoSpatialCondition::Create(L"Data2", FdoSpatialOperations_EnvelopeIntersects, gv);
    printf("\n\n\nEnvelopeIntersects filter (with no R-tree) on non-default geometry property");
    select->SetFilter(filter);
    start = clock ();
    rdr = select->Execute();
    count2 = 0;
    while (rdr->ReadNext())
    {
        const wchar_t* something = rdr->GetString(L"Name");
        count2++;
    }
    rdr->Close();
    finish = clock ();
    printf("Count = %d  time: %2.3f seconds \n", count2, (double)(finish - start) / CLOCKS_PER_SEC);
    //correct R-Tree will return that many features!
    CPPUNIT_ASSERT(count2 == 4535);


    conn->Close();
}


void MasterTest::spatialFilter()
{
    FdoPtr<FdoIConnection> conn = CreateConnection();

    openConnection(conn, DEST_PATH);    
    
    FdoPtr<FdoISelect> select = (FdoISelect*)conn->CreateCommand(FdoCommandType_Select); 

    select->SetFeatureClassName(L"DaKlass");

   
    FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();

    double coords[] = { 7.2230, 43.6861, 
                        7.2505, 43.6861, 
                        7.2505, 43.7134, 
                        7.2230, 43.7134, 
                        7.2230, 43.6861 }; //last pt equals first for rings

    FdoPtr<FdoILinearRing> outer = gf->CreateLinearRing(0, 10, coords);
    FdoPtr<FdoIPolygon> poly = gf->CreatePolygon(outer, NULL);

    //FdoPtr<FdoILineString> poly = gf->CreateLineString(0, 10, coords);


    FdoPtr<FdoByteArray> polyfgf = gf->GetFgf(poly);
    FdoPtr<FdoGeometryValue> gv = FdoGeometryValue::Create(polyfgf);
    FdoPtr<FdoSpatialCondition> filter = FdoSpatialCondition::Create(L"Data", FdoSpatialOperations_Intersects, gv);
    

    printf("\n\n\nSpatial filter");

    select->SetFilter(filter);
   
    
    FdoPtr<FdoIFeatureReader> rdr = select->Execute();

    int count2 = 0;

    while (rdr->ReadNext())
    {
        //const wchar_t* something = rdr->GetString(L"Name");
        //const wchar_t* something2 = rdr->GetString(L"Key");
        //const wchar_t* something3 = rdr->GetString(L"Name");
        
        //wprintf(L"%s\n", something);
        count2++;
    }

    rdr->Close();
    
   
    CPPUNIT_ASSERT(count2 == 4533);

        
    conn->Close();
}


void MasterTest::coordSysTest()
{
    FdoPtr<FdoIConnection> conn = CreateConnection();

    openConnection(conn, DEST_PATH);

    printf("\n\n\nCoord sys tests");
    
    FdoPtr<FdoIGetSpatialContexts> gsc = (FdoIGetSpatialContexts*)conn->CreateCommand(FdoCommandType_GetSpatialContexts);
    
    FdoPtr<FdoISpatialContextReader> rdr = gsc->Execute();

    bool hello = rdr->ReadNext();    

    CPPUNIT_ASSERT(hello);

    FdoPtr<FdoByteArray> extent = rdr->GetExtent();

    if (extent != NULL)
    {
        FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
        FdoPtr<FdoIGeometry> fgfgeom = gf->CreateGeometryFromFgf(extent);

        FdoIPolygon* poly = dynamic_cast<FdoIPolygon*> (fgfgeom.p);

        FdoPtr<FdoILinearRing> ring = poly->GetExteriorRing();

        double x, y, z, m; 
        int dim;

        ring->GetItemByMembers(0, &x, &y, &z, &m, &dim);
        printf("%lf,  %lf\n", x, y);

        ring->GetItemByMembers(1, &x, &y, &z, &m, &dim);
        printf("%lf,  %lf\n", x, y);

        ring->GetItemByMembers(2, &x, &y, &z, &m, &dim);
        printf("%lf,  %lf\n", x, y);

        ring->GetItemByMembers(3, &x, &y, &z, &m, &dim);
        printf("%lf,  %lf\n", x, y);
    }


    FdoString* desc = rdr->GetDescription();
    FdoString* cs = rdr->GetCoordinateSystem();

    CPPUNIT_ASSERT(rdr->GetXYTolerance() == 17.0);
    CPPUNIT_ASSERT(rdr->GetZTolerance() == 3.14159);

    conn->Close();
}


void MasterTest::selectDistinctTests()
{
    printf("Testing select distinct:\n");

    FdoPtr<FdoIConnection> conn = CreateConnection();

    openConnection(conn, SHP_PATH, true);

    /*
    FdoPtr<FdoISelectAggregates> advsel = dynamic_cast<FdoISelectAggregates*>(conn->CreateCommand(FdoCommandType_SelectAggregates));

    advsel->SetFeatureClassName(L"SHPClass");
    
    FdoPtr<FdoIdentifierCollection> ids = advsel->GetPropertyNames();

    FdoPtr<FdoComputedIdentifier> cid = FdoComputedIdentifier::Create(L"NumRecords");
    FdoPtr<FdoExpression> expr = FdoExpression::Parse(L"Count(MAPKEY)");
    cid->SetExpression(expr);
    ids->Add(cid);
    */
    
    //select distinct
    FdoPtr<FdoISelectAggregates> advsel = (FdoISelectAggregates*)(conn->CreateCommand(FdoCommandType_SelectAggregates));

    advsel->SetFeatureClassName(L"World_Countries");
    
    FdoPtr<FdoIdentifierCollection> ids = advsel->GetPropertyNames();

    FdoPtr<FdoIdentifier> id = FdoIdentifier::Create(L"KEY");
    ids->Add(id);

    id = FdoIdentifier::Create(L"NAME");
    ids->Add(id);

    advsel->SetDistinct(true);

    FdoPtr<FdoIDataReader> rdr = advsel->Execute();

    int count = 0;

    char tmp[1024];

    while (rdr->ReadNext())
    {
        FdoString* mk = rdr->GetString(L"KEY");
        wcstombs(tmp, mk, 1024);
        printf("%s\n", tmp);
        count++;
    }

    rdr->Close();

    printf("Distinct values returned: %d", count);
    //-------------
    
    conn->Close();    

}



void MasterTest::selectAggregatesTest()
{
    printf("Testing select aggregates:\n");

    FdoPtr<FdoIConnection> conn = CreateConnection();

    openConnection(conn, SHP_PATH2, true);

    FdoPtr<FdoISelectAggregates> advsel = (FdoISelectAggregates*)(conn->CreateCommand(FdoCommandType_SelectAggregates));

    advsel->SetFeatureClassName(L"province");
    
    FdoPtr<FdoIdentifierCollection> ids = advsel->GetPropertyNames();

    FdoPtr<FdoExpression> expr = FdoExpression::Parse(L"Count(AREA)");
    FdoPtr<FdoComputedIdentifier> cid = FdoComputedIdentifier::Create(L"NumRecords", expr);
    ids->Add(cid);

    expr = FdoExpression::Parse(L"Min(AREA)");
    cid = FdoComputedIdentifier::Create(L"MinArea", expr);
    ids->Add(cid);
   
    expr = FdoExpression::Parse(L"Max(AREA)");
    cid = FdoComputedIdentifier::Create(L"MaxArea", expr);
    ids->Add(cid);

    expr = FdoExpression::Parse(L"Avg(AREA)");
    cid = FdoComputedIdentifier::Create(L"AverageArea", expr);
    ids->Add(cid);


    FdoPtr<FdoIDataReader> rdr = advsel->Execute();

    int count = 0;

    while (rdr->ReadNext())
    {
        printf("Number of provinces: %d\n", rdr->GetInt64(L"NumRecords"));
        printf("Min area: %lf\n", rdr->GetDouble(L"MinArea"));
        printf("Max area: %lf\n", rdr->GetDouble(L"MaxArea"));
        printf("Ave area: %lf\n", rdr->GetDouble(L"AverageArea"));
        
        count++;
    }

    rdr->Close();

    
    conn->Close();    

}

