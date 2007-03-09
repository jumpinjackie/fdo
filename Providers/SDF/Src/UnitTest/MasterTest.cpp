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
#include <math.h>
#include "MasterTest.h"
#include "FdoCommonFile.h"
#include "SDF/SdfCommandType.h"
#include "SDF/ICreateSDFFile.h"
#include "UnitTestUtil.h"
#include <cppunit/extensions/HelperMacros.h>
const wchar_t* DEST_PATH = L"../../TestData/PARCEL_linuxtest.SDX";
const wchar_t* SHP_PATH = L"../../TestData/World_Countries.sdf";
const wchar_t* SHP_PATH2 = L"../../TestData/province.sdf";
const wchar_t* AGGR_PATH = L"../../TestData/TestAggregates.sdf";

// Replace the text "TestExample" with your own class name
CPPUNIT_TEST_SUITE_REGISTRATION(MasterTest);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( MasterTest, "MasterTest");


MasterTest::MasterTest()
{

}


MasterTest::~MasterTest()
{
}

FdoIConnection* MasterTest::CreateConnection()
{
    FdoPtr<IConnectionManager> manager = FdoFeatureAccessManager::GetConnectionManager ();
    return (manager->CreateConnection (L"OSGeo.SDF.3.3"));
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
        TestCommonFail( ex );
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
        TestCommonFail( ex );
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
        TestCommonFail( ex );
    }
}

void MasterTest::keyFilterBeforeDelete()
{
    try
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
    catch ( FdoException* ex ) {
        TestCommonFail( ex );
    }
}


void MasterTest::keyFilterAfterDelete()
{
    try
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
    catch ( FdoException* ex ) {
        TestCommonFail( ex );
    }
}


void MasterTest::computedPropTest()
{
    try
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
    catch ( FdoException* ex ) {
        TestCommonFail( ex );
    }
}



void MasterTest::stringFilter()
{
    try
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
    catch ( FdoException* ex ) {
        TestCommonFail( ex );
    }
}

void MasterTest::rtreeFilter()
{
    try
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
    catch ( FdoException* ex ) {
        TestCommonFail( ex );
    }
}


void MasterTest::spatialFilter()
{
    try
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
    catch ( FdoException* ex ) {
        TestCommonFail( ex );
    }
}

void MasterTest::spatialInsideFilter()
{
    FdoPtr<FdoIConnection> conn = CreateConnection();

    openConnection(conn, SHP_PATH);    
    
    FdoPtr<FdoISelect> select = (FdoISelect*)conn->CreateCommand(FdoCommandType_Select); 

    select->SetFeatureClassName(L"World_Countries");

   
    FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
    // 
    double coords[] = { -18.6448,49.1377,
                        35.8678, 49.1377, 
                        35.8678,11.7581, 
                        -18.6448, 11.7581, 
                        -18.6448,49.1377 }; //last pt equals first for rings

    FdoPtr<FdoILinearRing> outer = gf->CreateLinearRing(0, 10, coords);
    FdoPtr<FdoIPolygon> poly = gf->CreatePolygon(outer, NULL);


    FdoPtr<FdoByteArray> polyfgf = gf->GetFgf(poly);
    FdoPtr<FdoGeometryValue> gv = FdoGeometryValue::Create(polyfgf);
    FdoPtr<FdoSpatialCondition> filter = FdoSpatialCondition::Create(L"SHPGEOM", FdoSpatialOperations_Inside, gv);
    select->SetFilter(filter);
    FdoPtr<FdoIFeatureReader> rdr = select->Execute();
    int count2 = 0;
    while (rdr->ReadNext())
        count2++;

    rdr->Close();
    conn->Close();

    CPPUNIT_ASSERT(count2 == 53);
}

void MasterTest::coordSysTest()
{
    try
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
    catch ( FdoException* ex ) {
        TestCommonFail( ex );
    }
}


void MasterTest::selectDistinctTests()
{
    try
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
    catch ( FdoException* ex ) {
        TestCommonFail( ex );
    }
}



void MasterTest::selectAggregatesTest()
{
    try
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
    catch ( FdoException* ex ) {
        TestCommonFail( ex );
    }
}

void MasterTest::selectSpatialExtentsTest()
{
    printf("Testing Select SpatialExtents:\n");

    try 
	{
        FdoPtr<FdoISelectAggregates> selAggr;
        FdoPtr<FdoIDataReader> rdr;
        FdoPtr<FdoIdentifierCollection> ids;
        FdoPtr<FdoExpression> expr;
        FdoPtr<FdoComputedIdentifier> cid;
        int count = 0;

        FdoPtr<FdoIConnection> conn = CreateConnection();
        openConnection(conn, SHP_PATH2, true);
		CPPUNIT_ASSERT(conn->Open() == FdoConnectionState_Open);

        FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
		
        // test optimized SelectAggregates(SpatialExtents) -- NO FILTER
        FdoPtr<FdoISelectAggregates> advsel = (FdoISelectAggregates*)(conn->CreateCommand(FdoCommandType_SelectAggregates));
        advsel->SetFeatureClassName(L"province");
	    ids = advsel->GetPropertyNames();
	    expr = FdoExpression::Parse(L"SpatialExtents(SHPGEOM)");
	    cid = FdoComputedIdentifier::Create(L"MBR", expr);
	    ids->Add(cid);
	    rdr = advsel->Execute();

	    count = 0;
	    FdoPtr<FdoIEnvelope> envelopeAllWithoutFilter;
	    while (rdr->ReadNext())
	    {
		    if ( rdr->IsNull(L"MBR") )
			    CPPUNIT_FAIL("NULL MBR geometry returned for SpatialExtents()");
            
		    FdoPtr<FdoByteArray> geomBytes = rdr->GetGeometry(L"MBR");
		    FdoPtr<FdoIGeometry> geom = gf->CreateGeometryFromFgf(geomBytes);

            FdoGeometryType geomType = geom->GetDerivedType();
            if (geomType != FdoGeometryType_Polygon)
			    CPPUNIT_FAIL("Expected Polygon geometry for SpatialExtents() result");

		    envelopeAllWithoutFilter = geom->GetEnvelope();

		    if (envelopeAllWithoutFilter->GetIsEmpty())
			    CPPUNIT_FAIL("Expected non-empty envelope for SpatialExtents() result");

		    count++;
	    }
	    CPPUNIT_ASSERT_MESSAGE("Expected exactly one row of aggregate data", count==1);

	    rdr->Close();

	    CPPUNIT_ASSERT_MESSAGE("SpatialExtents results don't match (MinX)", fabs(envelopeAllWithoutFilter->GetMinX() - (-141.003)) < 0.001);
	    CPPUNIT_ASSERT_MESSAGE("SpatialExtents results don't match (MinY)", fabs(envelopeAllWithoutFilter->GetMinY() - (41.913)) < 0.001);
	    CPPUNIT_ASSERT_MESSAGE("SpatialExtents results don't match (MaxX)", fabs(envelopeAllWithoutFilter->GetMaxX() - (-52.620)) < 0.001);
	    CPPUNIT_ASSERT_MESSAGE("SpatialExtents results don't match (MaxY)", fabs(envelopeAllWithoutFilter->GetMaxY() - (83.108)) < 0.001);

        // test non-optimized SelectAggregates(SpatialExtents) -- WITH FILTER
        advsel = (FdoISelectAggregates*)(conn->CreateCommand(FdoCommandType_SelectAggregates));
        advsel->SetFeatureClassName(L"province");
	    ids = advsel->GetPropertyNames();
	    expr = FdoExpression::Parse(L"SpatialExtents(SHPGEOM)");
	    cid = FdoComputedIdentifier::Create(L"MBR", expr);
	    ids->Add(cid);
        advsel->SetFilter(L"AREA > 0");
	    rdr = advsel->Execute();

	    count = 0;
	    while (rdr->ReadNext())
	    {
		    if ( rdr->IsNull(L"MBR") )
			    CPPUNIT_FAIL("NULL MBR geometry returned for SpatialExtents()");
            
		    FdoPtr<FdoByteArray> geomBytes = rdr->GetGeometry(L"MBR");
		    FdoPtr<FdoIGeometry> geom = gf->CreateGeometryFromFgf(geomBytes);

            FdoGeometryType geomType = geom->GetDerivedType();
            if (geomType != FdoGeometryType_Polygon)
			    CPPUNIT_FAIL("Expected Polygon geometry for SpatialExtents() result");

		    envelopeAllWithoutFilter = geom->GetEnvelope();

		    if (envelopeAllWithoutFilter->GetIsEmpty())
			    CPPUNIT_FAIL("Expected non-empty envelope for SpatialExtents() result");

		    count++;
	    }
	    CPPUNIT_ASSERT_MESSAGE("Expected exactly one row of aggregate data", count==1);

	    rdr->Close();

	    CPPUNIT_ASSERT_MESSAGE("SpatialExtents results don't match (MinX)", fabs(envelopeAllWithoutFilter->GetMinX() - (-141.003)) < 0.001);
	    CPPUNIT_ASSERT_MESSAGE("SpatialExtents results don't match (MinY)", fabs(envelopeAllWithoutFilter->GetMinY() - (41.913)) < 0.001);
	    CPPUNIT_ASSERT_MESSAGE("SpatialExtents results don't match (MaxX)", fabs(envelopeAllWithoutFilter->GetMaxX() - (-52.620)) < 0.001);
	    CPPUNIT_ASSERT_MESSAGE("SpatialExtents results don't match (MaxY)", fabs(envelopeAllWithoutFilter->GetMaxY() - (83.108)) < 0.001);

        // test expected error conditions
        bool failed = false;
        try
        {
            advsel = (FdoISelectAggregates*)(conn->CreateCommand(FdoCommandType_SelectAggregates));
            advsel->SetFeatureClassName(L"foo");
	        ids = advsel->GetPropertyNames();
	        expr = FdoExpression::Parse(L"SpatialExtents(SHPGEOM)");
	        cid = FdoComputedIdentifier::Create(L"MBR", expr);
	        ids->Add(cid);
	        rdr = advsel->Execute();
        }
        catch ( FdoException *ex )
        {
            ex->Release();
            failed = true;
        }

        if (!failed) {
            CPPUNIT_FAIL("FAILED - SelectAggregates allowed accessing an invalid class\n");
        }

        failed = false;
        try
        {
            advsel = (FdoISelectAggregates*)(conn->CreateCommand(FdoCommandType_SelectAggregates));
            advsel->SetFeatureClassName(L"province");
	        ids = advsel->GetPropertyNames();
	        expr = FdoExpression::Parse(L"SpatialExtents(foo)");
	        cid = FdoComputedIdentifier::Create(L"MBR", expr);
	        ids->Add(cid);
	        rdr = advsel->Execute();
        }
        catch ( FdoException *ex )
        {
            ex->Release();
            failed = true;
        }

        if (!failed) {
            CPPUNIT_FAIL("FAILED - SelectAggregates allowed accessing an invalid property\n");
        }

        failed = false;
        try
        {
            advsel = (FdoISelectAggregates*)(conn->CreateCommand(FdoCommandType_SelectAggregates));
            advsel->SetFeatureClassName(L"province");
	        ids = advsel->GetPropertyNames();
	        expr = FdoExpression::Parse(L"foo(SHPGEOM)");
	        cid = FdoComputedIdentifier::Create(L"MBR", expr);
	        ids->Add(cid);
	        rdr = advsel->Execute();
        }
        catch ( FdoException *ex )
        {
            ex->Release();
            failed = true;
        }

        if (!failed) {
            CPPUNIT_FAIL("FAILED - SelectAggregates allowed accessing an invalid function\n");
        }

        failed = false;
        try
        {
            advsel = (FdoISelectAggregates*)(conn->CreateCommand(FdoCommandType_SelectAggregates));
            advsel->SetFeatureClassName(L"province");

	        ids = advsel->GetPropertyNames();
	        expr = FdoExpression::Parse(L"SpatialExtents(SHPGEOM)");
	        cid = FdoComputedIdentifier::Create(L"MBR", expr);
	        ids->Add(cid);

            double coords[] = {7.2, 43.6, 7.2, 43.6, 7.2, 43.7, 7.2, 43.7, 7.2, 43.6}; 
            FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
            FdoPtr<FdoILinearRing> outer = gf->CreateLinearRing(0, 10, coords);
            FdoPtr<FdoIPolygon> poly = gf->CreatePolygon(outer, NULL);
            FdoPtr<FdoByteArray> polyfgf = gf->GetFgf(poly);
            FdoPtr<FdoGeometryValue> gv = FdoGeometryValue::Create(polyfgf);
            FdoPtr<FdoSpatialCondition> filter = FdoSpatialCondition::Create(L"Data", FdoSpatialOperations_EnvelopeIntersects, gv);
            advsel->SetFilter(filter);

	        rdr = advsel->Execute();
        }
        catch ( FdoException *ex )
        {
            ex->Release();
            failed = true;
        }

        if (!failed) {
            CPPUNIT_FAIL("FAILED - SelectAggregates allowed specifiecation of a spatial filter\n");
        }
    }
	catch( FdoException *ex )
    {
        CPPUNIT_FAIL((const char*)(FdoStringP(ex->GetExceptionMessage())));
        ex->Release();
    }
}


void MasterTest::descReadOnly()
{
    try
    {
        printf("Testing describeschema with read-only connection:\n");

        // open connection as read-only:
        FdoPtr<FdoIConnection> conn = CreateConnection();
        openConnection(conn, SHP_PATH, true);

        // Verify that classes are read-only too:
        FdoPtr<FdoIDescribeSchema> descSchema = static_cast<FdoIDescribeSchema*>(conn->CreateCommand(FdoCommandType_DescribeSchema));
        FdoPtr<FdoFeatureSchemaCollection> schemas = descSchema->Execute();
        for (int s=0; s<schemas->GetCount(); s++)
        {
            FdoPtr<FdoFeatureSchema> schema = schemas->GetItem(s);
            FdoPtr<FdoClassCollection> classes = schema->GetClasses();
            for (int c=0; c<classes->GetCount(); c++)
            {
                FdoPtr<FdoClassDefinition> classDef = classes->GetItem(c);
                FdoPtr<FdoClassCapabilities> classCapabilities = classDef->GetCapabilities();
                CPPUNIT_ASSERT_MESSAGE("class should be read-only", !classCapabilities->SupportsWrite());
            }
        }

        // open connection as read-write:
        conn = CreateConnection();
        openConnection(conn, SHP_PATH, false);

        // Verify that classes are read-write too:
        descSchema = static_cast<FdoIDescribeSchema*>(conn->CreateCommand(FdoCommandType_DescribeSchema));
        schemas = descSchema->Execute();
        for (int s=0; s<schemas->GetCount(); s++)
        {
            FdoPtr<FdoFeatureSchema> schema = schemas->GetItem(s);
            FdoPtr<FdoClassCollection> classes = schema->GetClasses();
            for (int c=0; c<classes->GetCount(); c++)
            {
                FdoPtr<FdoClassDefinition> classDef = classes->GetItem(c);
                FdoPtr<FdoClassCapabilities> classCapabilities = classDef->GetCapabilities();
                CPPUNIT_ASSERT_MESSAGE("class should be read-write", classCapabilities->SupportsWrite());
            }
        }
    }
    catch ( FdoException* ex ) {
        TestCommonFail( ex );
    }
}


void MasterTest::selectAggregatesFailureTests()
{
    try
    {
        printf("Testing select aggregates:\n");

        FdoPtr<FdoIConnection> conn = CreateConnection();

        openConnection(conn, SHP_PATH2, true);

        FdoPtr<FdoISelectAggregates> advsel = (FdoISelectAggregates*)(conn->CreateCommand(FdoCommandType_SelectAggregates));

        advsel->SetFeatureClassName(L"province");
        
        FdoPtr<FdoIdentifierCollection> ids = advsel->GetPropertyNames();
        FdoPtr<FdoExpression> expr;
        FdoPtr<FdoComputedIdentifier> cid;

        expr = FdoExpression::Parse(L"Avg(CODE)");
        cid = FdoComputedIdentifier::Create(L"AvgCode", expr);
        ids->Add(cid);

        try
        {
            FdoPtr<FdoIDataReader> rdr = advsel->Execute();
            CPPUNIT_FAIL("Expected exception when doing AVG() of a String property, but didn't get one");
        }
        catch (FdoException *e)
        {
            // We expect to fall into here, so absorb the exception quietly
            e->Release();
        }
    }
    catch ( FdoException* ex ) {
        TestCommonFail( ex );
    }
}



void MasterTest::test_aggregates_datetime_string(void)
{
    try
    {
        FdoPtr<FdoIConnection> connection = CreateConnection();

        ////////////////////////////////////////////////////////////////////////////////////////////
        // Create a new SDF File:
        ////////////////////////////////////////////////////////////////////////////////////////////

        FdoCommonFile::Delete(AGGR_PATH, true);
	    FdoPtr<FdoICreateSDFFile> crsdf = (FdoICreateSDFFile*)(connection->CreateCommand(SdfCommandType_CreateSDFFile));
	    crsdf->SetCoordinateSystemWKT(L"[LL84]");
	    crsdf->SetFileName(AGGR_PATH);
	    crsdf->SetSpatialContextDescription(L"World Coordinate System, Degrees, what else do you need to know?");
	    crsdf->SetSpatialContextName(L"World Geodetic Coordinate System, 1984");
	    crsdf->SetXYTolerance(17.0);
	    crsdf->SetZTolerance(3.14159);
	    crsdf->Execute();

        // Connect to new file:
        wchar_t connString[500];
        swprintf(connString, 500, L"File=%ls", AGGR_PATH);
        connection->SetConnectionString(connString);
        connection->Open();


        ////////////////////////////////////////////////////////////////////////////////////////////
        // Create a new schema and class in the new SDF file:
        ////////////////////////////////////////////////////////////////////////////////////////////

        // Create new schema:
        FdoPtr<FdoFeatureSchema> pSchema = FdoFeatureSchema::Create(L"MySchema", L"");

	    // Create a new Feature Class:
	    FdoPtr<FdoFeatureClass> pClass = FdoFeatureClass::Create( L"MyClass", L"" );
	    pClass->SetIsAbstract(false);

	    // Add featid property

	    FdoPtr<FdoDataPropertyDefinition> pProp = FdoDataPropertyDefinition::Create( L"FeatId", L"id" );
	    pProp->SetDataType( FdoDataType_Int32 );
	    pProp->SetNullable(false);
        pProp->SetIsAutoGenerated(true);
	    FdoPropertiesP(pClass->GetProperties())->Add( pProp );
	    FdoDataPropertiesP(pClass->GetIdentityProperties())->Add( pProp );

	    // Add geometry property

	    FdoPtr<FdoGeometricPropertyDefinition> pGeomProp = FdoGeometricPropertyDefinition::Create( L"Geometry", L"location and shape" );
	    pGeomProp->SetGeometryTypes( FdoGeometricType_Point | FdoGeometricType_Curve );
	    pGeomProp->SetHasElevation(true);
        FdoPropertiesP(pClass->GetProperties())->Add( pGeomProp );
	    pClass->SetGeometryProperty( pGeomProp );

	    // Add data properties of various types

	    pProp = FdoDataPropertyDefinition::Create( L"MyString", L"A, B or C" );
	    pProp->SetDataType( FdoDataType_String );
	    pProp->SetLength(200);
	    pProp->SetNullable(false);
	    FdoPropertiesP(pClass->GetProperties())->Add( pProp );

	    pProp = FdoDataPropertyDefinition::Create( L"MyDate", L"" );
	    pProp->SetDataType( FdoDataType_DateTime );
	    pProp->SetNullable(true);
	    FdoPropertiesP(pClass->GetProperties())->Add( pProp );

	    FdoClassesP(pSchema->GetClasses())->Add( pClass );

        // Now do ApplySchema:
	    FdoPtr<FdoIApplySchema>  pCmd = (FdoIApplySchema*) connection->CreateCommand(FdoCommandType_ApplySchema);
	    pCmd->SetFeatureSchema( pSchema );
	    pCmd->Execute();


        ////////////////////////////////////////////////////////////////////////////////////////////
        // Insert some test data:
        ////////////////////////////////////////////////////////////////////////////////////////////

        FdoPtr<FdoIInsert> ins = (FdoIInsert*)connection->CreateCommand(FdoCommandType_Insert);
        ins->SetFeatureClassName(L"MyClass");

        FdoPtr<FdoPropertyValueCollection> propVals = ins->GetPropertyValues();
        FdoPtr<FdoPropertyValue> propVal;
        FdoPtr<FdoIFeatureReader> reader;

        propVals->Clear();
        propVal = FdoPropertyValue::Create(L"MyString", FdoPtr<FdoValueExpression>((FdoValueExpression*)FdoValueExpression::Parse(L"'Potato'")));
        propVals->Add(propVal);
        propVal = FdoPropertyValue::Create(L"MyDate", FdoPtr<FdoValueExpression>((FdoValueExpression*)FdoValueExpression::Parse(L"DATE'1957-3-2'")));
        propVals->Add(propVal);
        reader = ins->Execute();

        propVals->Clear();
        propVal = FdoPropertyValue::Create(L"MyString", FdoPtr<FdoValueExpression>((FdoValueExpression*)FdoValueExpression::Parse(L"'Tomato'")));
        propVals->Add(propVal);
        propVal = FdoPropertyValue::Create(L"MyDate", FdoPtr<FdoValueExpression>((FdoValueExpression*)FdoValueExpression::Parse(L"DATE'1967-5-30'")));
        propVals->Add(propVal);
        reader = ins->Execute();


        ////////////////////////////////////////////////////////////////////////////////////////////
        // Select aggregates on datetime values:
        ////////////////////////////////////////////////////////////////////////////////////////////

        FdoPtr<FdoISelectAggregates> selAggr = (FdoISelectAggregates*)connection->CreateCommand(FdoCommandType_SelectAggregates);
        selAggr->SetFeatureClassName(L"MySchema:MyClass");
        FdoPtr<FdoIdentifierCollection> ids = selAggr->GetPropertyNames();
        ids->Add(FdoPtr<FdoComputedIdentifier>(FdoComputedIdentifier::Create(L"MinDate", FdoPtr<FdoExpression>(FdoExpression::Parse(L"Min(MyDate)")))));
        ids->Add(FdoPtr<FdoComputedIdentifier>(FdoComputedIdentifier::Create(L"MaxDate", FdoPtr<FdoExpression>(FdoExpression::Parse(L"Max(MyDate)")))));
        ids->Add(FdoPtr<FdoComputedIdentifier>(FdoComputedIdentifier::Create(L"MinString", FdoPtr<FdoExpression>(FdoExpression::Parse(L"Min(MyString)")))));
        ids->Add(FdoPtr<FdoComputedIdentifier>(FdoComputedIdentifier::Create(L"MaxString", FdoPtr<FdoExpression>(FdoExpression::Parse(L"Max(MyString)")))));
        FdoPtr<FdoIDataReader> dr = selAggr->Execute();
        CPPUNIT_ASSERT_MESSAGE("Should have one row of data", dr->ReadNext());

        FdoDateTime mindt = dr->GetDateTime(L"MinDate");
        CPPUNIT_ASSERT_MESSAGE("wrong min date year", mindt.year==1957);
        CPPUNIT_ASSERT_MESSAGE("wrong min date month", mindt.month==3);
        CPPUNIT_ASSERT_MESSAGE("wrong min date day", mindt.day==2);

        FdoDateTime maxdt = dr->GetDateTime(L"MaxDate");
        CPPUNIT_ASSERT_MESSAGE("wrong max date year", maxdt.year==1967);
        CPPUNIT_ASSERT_MESSAGE("wrong max date month", maxdt.month==5);
        CPPUNIT_ASSERT_MESSAGE("wrong max date day", maxdt.day==30);

        FdoString* minString = dr->GetString(L"MinString");
        CPPUNIT_ASSERT_MESSAGE("wrong min string", 0==wcscmp(minString, L"Potato"));

        FdoString* maxString = dr->GetString(L"MaxString");
        CPPUNIT_ASSERT_MESSAGE("wrong max string", 0==wcscmp(maxString, L"Tomato"));

        CPPUNIT_ASSERT_MESSAGE("Should only have one row of data", !dr->ReadNext());
    }
    catch ( FdoException* ex ) {
        TestCommonFail( ex );
    }
}
void MasterTest::selectFunctionTest()
{
	try
	{
		FdoPtr<FdoIConnection> conn = CreateConnection();

		openConnection(conn, SHP_PATH, true);

		FdoPtr<FdoISelect> select = (FdoISelect*)(conn->CreateCommand(FdoCommandType_Select));

		select->SetFeatureClassName(L"World_Countries");
	    
		select->SetFilter(L"Upper(NAME) LIKE '%TU%'");
		FdoPtr<FdoIReader> rdr = select->Execute();
		int count = 0;
		while (rdr->ReadNext())
		{
			//printf("%ls\n", rdr->GetString(L"NAME"));
			count++;
		}
		rdr->Close();
		
		//printf("Upper count: %d\n", count);
		CPPUNIT_ASSERT_MESSAGE("Unexpected number of records returned by a Upper filter", count==14);

		select->SetFilter(L"Lower(NAME) LIKE '%tu%'");
		rdr = select->Execute();
		count = 0;
		while (rdr->ReadNext())
		{
			//printf("%ls\n", rdr->GetString(L"NAME"));
			count++;
		}
		rdr->Close();
		//printf("Upper count: %d\n", count);
		CPPUNIT_ASSERT_MESSAGE("Unexpected number of records returned by a Lower filter", count==14);
	    
		select->SetFilter(L"Ceil(Autogenerated_ID) < 10");
		rdr = select->Execute();
		count = 0;
		while (rdr->ReadNext())
		{
			//printf("%ls\n", rdr->GetString(L"NAME"));
			count++;
		}
		rdr->Close();
		//printf("Ceil count: %d\n", count);
		CPPUNIT_ASSERT_MESSAGE("Unexpected number of records returned by a Lower filter", count==9);

		select->SetFilter(L"Floor(Autogenerated_ID) < 10");
		rdr = select->Execute();
		count = 0;
		while (rdr->ReadNext())
		{
			//printf("%ls\n", rdr->GetString(L"NAME"));
			count++;
		}
		rdr->Close();
		//printf("Floor count: %d\n", count);
		CPPUNIT_ASSERT_MESSAGE("Unexpected number of records returned by a Floor filter", count==9);

		conn->Close();    
	}
	catch(FdoException *exp )
	{
		printf("Function test failed: %ls\n", exp->GetExceptionMessage() );
		exp->Release();
		CPPUNIT_FAIL("Function test failed");
	}
}

