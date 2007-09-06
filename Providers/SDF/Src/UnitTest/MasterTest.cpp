// Copyright (C) 2004-2007  Autodesk, Inc.
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

#include "stdafx.h"
#include <ctime>
#include <math.h>
#include "MasterTest.h"
#include "FdoCommonFile.h"
#include "SDF/SdfCommandType.h"
#include "SDF/ICreateSDFFile.h"
#include "UnitTestUtil.h"
#include <cppunit/extensions/HelperMacros.h>
#include "FdoExpressionEngine.h"
#include "FdoCommonOSUtil.h"
#include "FdoCommonMiscUtil.h"
const wchar_t* CONSTRAINS_FILE = L"../../TestData/Constrains.sdf";
const wchar_t* SHP_PATH = L"../../TestData/World_Countries.sdf";
const wchar_t* SHP_PATH2 = L"../../TestData/province.sdf";
const wchar_t* AGGR_PATH = L"../../TestData/TestAggregates.sdf";
const wchar_t* DEST_PATH2 = L"../../TestData/TestSdf.sdf";

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

        openConnection(conn, DESTINATION_FILE);    
       
        FdoPtr<FdoIDelete> del = (FdoIDelete*)conn->CreateCommand(FdoCommandType_Delete); 

        del->SetFeatureClassName(L"Parcel");

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

        openConnection(conn, DESTINATION_FILE);    


        FdoPtr<FdoIUpdate> update = (FdoIUpdate*)conn->CreateCommand(FdoCommandType_Update); 

        update->SetFeatureClassName(L"Parcel");

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
    FdoPtr<FdoIConnection> update_conn = CreateConnection();
    openConnection(update_conn, DESTINATION_FILE);    


    FdoPtr<FdoIUpdate> update = (FdoIUpdate*)update_conn->CreateCommand(FdoCommandType_Update); 

    update->SetFeatureClassName(L"Parcel");

    FdoPtr<FdoFilter> filter = FdoFilter::Parse(L"(Key = 'DI0022')");// or (Key = 'LN0316')");

    update->SetFilter(filter);

    FdoPtr<FdoStringValue> sval = FdoStringValue::Create(L"Cool");
    FdoPtr<FdoPropertyValue> pv = FdoPropertyValue::Create(L"Name", sval);

    FdoPtr<FdoPropertyValueCollection> pvc = update->GetPropertyValues();

    pvc->Add(pv);

    printf("\n\n\nTesting Update");
       
    int count = update->Execute();

	printf("\n\nUpdated %d features\n", count);

	UnitTestUtil::CreateData( false, NULL, 200 );

    update_conn->Close();
}

void MasterTest::concurencyRtreeTest()
{
	FdoPtr<FdoIConnection> conn1 = CreateConnection();

    FdoPtr<FdoIConnection> conn = CreateConnection();
	UnitTestUtil::CreateData( true , conn, 0);
    conn->Close();
	openConnection(conn, DESTINATION_FILE);

	UnitTestUtil::CreateData( false , conn1, 4000);

	conn1->Close();
	openConnection(conn1, DESTINATION_FILE);    

    FdoPtr<FdoISelect> select = (FdoISelect*)conn->CreateCommand(FdoCommandType_Select); 

    select->SetFeatureClassName(L"Parcel");
    
    FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();

    double coords[] = { 7.2068, 43.7556, 
                        7.2088, 43.7556, 
                        7.2088, 43.7574, 
                        7.2068, 43.7574, 
                        7.2068, 43.7556 }; //last pt equals first for rings

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
        count2++;

    rdr->Close();
    

    finish = clock ();
    printf("Count = %d  time: %2.3f seconds \n", count2, (double)(finish - start) / CLOCKS_PER_SEC);
    //correct R-Tree will return that many features!
    CPPUNIT_ASSERT(count2 == 20);
	conn->Close();
}

void MasterTest::keyFilterBeforeDelete()
{
    try
    {
        FdoPtr<FdoIConnection> conn = CreateConnection();

        openConnection(conn, DESTINATION_FILE);
           
        FdoPtr<FdoISelect> select = (FdoISelect*)conn->CreateCommand(FdoCommandType_Select); 

        select->SetFeatureClassName(L"Parcel");

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

        openConnection(conn, DESTINATION_FILE);
           
        FdoPtr<FdoISelect> select = (FdoISelect*)conn->CreateCommand(FdoCommandType_Select); 

        select->SetFeatureClassName(L"Parcel");

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

        openConnection(conn, DESTINATION_FILE);    
       
        FdoPtr<FdoISelect> select = (FdoISelect*)conn->CreateCommand(FdoCommandType_Select); 

        select->SetFeatureClassName(L"Parcel");


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

            FdoInt64 something = rdr->GetInt32(L"ComputedInt");
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

        openConnection(conn, DESTINATION_FILE);    
       
        FdoPtr<FdoISelect> select = (FdoISelect*)conn->CreateCommand(FdoCommandType_Select); 

        select->SetFeatureClassName(L"Parcel");

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

        openConnection(conn, DESTINATION_FILE);    
        
        FdoPtr<FdoISelect> select = (FdoISelect*)conn->CreateCommand(FdoCommandType_Select); 

        select->SetFeatureClassName(L"Parcel");

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
        select->SetFeatureClassName(L"Parcel");
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

        openConnection(conn, DESTINATION_FILE);    
        
        FdoPtr<FdoISelect> select = (FdoISelect*)conn->CreateCommand(FdoCommandType_Select); 

        select->SetFeatureClassName(L"Parcel");

       
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

        openConnection(conn, DESTINATION_FILE);

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
void MasterTest::rtreeCacheTest()
{
		try
	{
		FdoPtr<FdoIConnection> conn = CreateConnection();

	    
		UnitTestUtil::CreateData( true , conn, 4000);

		//Enabling the next 2 lines would make the select read from the file and not from the cached data.
		//conn->Close();
		//openConnection(conn, DESTINATION_FILE);    

		FdoPtr<FdoISelect> select = (FdoISelect*)conn->CreateCommand(FdoCommandType_Select); 

		select->SetFeatureClassName(L"Parcel");
	    
		FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();

		double coords[] = { 7.2068, 43.7556, 
							7.2088, 43.7556, 
							7.2088, 43.7574, 
							7.2068, 43.7574, 
							7.2068, 43.7556 }; //last pt equals first for rings

		FdoPtr<FdoILinearRing> outer = gf->CreateLinearRing(0, 10, coords);

		FdoPtr<FdoIPolygon> poly = gf->CreatePolygon(outer, NULL);

		FdoPtr<FdoByteArray> polyfgf = gf->GetFgf(poly);
		FdoPtr<FdoGeometryValue> gv = FdoGeometryValue::Create(polyfgf);
		FdoPtr<FdoSpatialCondition> filter = FdoSpatialCondition::Create(L"Data", FdoSpatialOperations_EnvelopeIntersects, gv);
	    
		printf("\n\n\nR-Tree filter");

		select->SetFilter(filter);
	   
		FdoPtr<FdoIFeatureReader> rdr = select->Execute();

		int count2 = 0;
		bool found = false;
		while (rdr->ReadNext())
		{
			const wchar_t* something = rdr->GetString(L"Name");
			if( wcscmp( something, L"AB0039") == 0 )
				found = true;
			count2++;
		}
		rdr->Close();
		CPPUNIT_ASSERT(found);

		FdoPtr<FdoIDelete> del = (FdoIDelete*)conn->CreateCommand(FdoCommandType_Delete); 
		del->SetFeatureClassName(L"Parcel");
		FdoPtr<FdoFilter> delfilter = FdoFilter::Parse(L"Name ='AB0039'");
		del->SetFilter(delfilter);
		CPPUNIT_ASSERT( del->Execute() == 1 );

		// Let's make sure the feature is removed
		rdr = select->Execute();
		count2 = 0;
		found = false;
		while (rdr->ReadNext())
		{
			const wchar_t* something = rdr->GetString(L"Name");
			if( wcscmp( something, L"AB0039") == 0 )
				found = true;
			count2++;
		}
		rdr->Close();
		CPPUNIT_ASSERT(!found);    
		CPPUNIT_ASSERT(count2 == 18);
		conn->Close();
	}
	catch(FdoException *e )
	{
		wprintf(L"%s\n", e->GetExceptionMessage());
        e->Release();
		CPPUNIT_FAIL("Unexpected FdoException");
	}
}

void MasterTest::CreateEmptyShpFileWithConstraints(FdoIConnection* conn)
{
    FdoPtr<FdoDataPropertyDefinition> dpd;

    //first delete the destination file if it exists already -- cleanup that is
#ifdef _WIN32
    SetFileAttributesW(CONSTRAINS_FILE, FILE_ATTRIBUTE_NORMAL);
	DeleteFileW( CONSTRAINS_FILE );

    wchar_t fullpath[1024];
    _wfullpath(fullpath, CONSTRAINS_FILE, 1024);

#else
    char cpath[1024];
    char cfullpath[1024];
    wcstombs(cpath, CONSTRAINS_FILE, 1024);
    unlink(cpath);
    realpath(cpath, cfullpath);
    wchar_t fullpath[1024];
    mbstowcs(fullpath, cfullpath, 1024);
#endif
	FdoPtr<FdoICreateDataStore>	pCreateCmd = (FdoICreateDataStore*) conn->CreateCommand(FdoCommandType_CreateDataStore);

	FdoPtr<FdoIDataStorePropertyDictionary> dictionary = pCreateCmd->GetDataStoreProperties();

	int	count;
	FdoString **names = dictionary->GetPropertyNames(count);

	CPPUNIT_ASSERT_MESSAGE("Wrong number of properties for create cmd", count==1 );

	dictionary->SetProperty( names[0], fullpath );

	pCreateCmd->Execute();	
	openConnection(conn, CONSTRAINS_FILE);   
	FdoPtr<FdoICreateSpatialContext>	pCreateCreateSpatialContext = (FdoICreateSpatialContext*) conn->CreateCommand(FdoCommandType_CreateSpatialContext);
	pCreateCreateSpatialContext->SetCoordinateSystemWkt(L"[LL84]");
	pCreateCreateSpatialContext->SetDescription(L"World Coordinate System, Degrees, what else do you need to know?" );
	pCreateCreateSpatialContext->SetName( L"World Geodetic Coordinate System, 1984" );
	pCreateCreateSpatialContext->SetXYTolerance( 17.0 );
	pCreateCreateSpatialContext->SetZTolerance(3.14159);
	pCreateCreateSpatialContext->Execute();
	

    //apply schema
    FdoPtr<FdoIApplySchema> applyschema = (FdoIApplySchema*)conn->CreateCommand(FdoCommandType_ApplySchema);

    FdoPtr<FdoFeatureSchema> schema = FdoFeatureSchema::Create(L"MySchema", L"");
    FdoPtr<FdoFeatureClass> clas = FdoFeatureClass::Create(L"MyClass",L"");
    FdoPtr<FdoClassCollection>(schema->GetClasses())->Add(clas);
    FdoPtr<FdoPropertyDefinitionCollection> properties = clas->GetProperties();
    FdoPtr<FdoGeometricPropertyDefinition> geometry = FdoGeometricPropertyDefinition::Create(L"Geom", L"");
    geometry->SetGeometryTypes(7); // Point, Line, Polygon
    properties->Add(geometry);
    clas->SetGeometryProperty(geometry);
    dpd = FdoDataPropertyDefinition::Create(L"NotNullString", L"");
    dpd->SetNullable(false);
    dpd->SetDataType(FdoDataType_String);
    properties->Add(dpd);

    dpd = FdoDataPropertyDefinition::Create(L"StringRange", L"");
    FdoPtr<FdoPropertyValueConstraintRange> rangeConstraint = FdoPropertyValueConstraintRange::Create();
    rangeConstraint->SetMaxInclusive(false);
    rangeConstraint->SetMaxValue(FdoPtr<FdoDataValue>(FdoDataValue::Create(L"Rich")));
    rangeConstraint->SetMinInclusive(true);
    rangeConstraint->SetMinValue(FdoPtr<FdoDataValue>(FdoDataValue::Create(L"Poor")));
    dpd->SetValueConstraint(rangeConstraint);
    dpd->SetNullable(true);
    dpd->SetDataType(FdoDataType_String);
    properties->Add(dpd);

    dpd = FdoDataPropertyDefinition::Create(L"Int32Range", L"The key of the object");
    dpd->SetDataType(FdoDataType_Int32);
    rangeConstraint = FdoPropertyValueConstraintRange::Create();
    rangeConstraint->SetMaxInclusive(true);
    rangeConstraint->SetMaxValue(FdoPtr<FdoDataValue>(FdoDataValue::Create(10000)));
    rangeConstraint->SetMinInclusive(true);
    //rangeConstraint->SetMinValue(FdoPtr<FdoDataValue>(FdoDataValue::Create(0)));
    dpd->SetNullable(true);
    dpd->SetValueConstraint(rangeConstraint);
    properties->Add(dpd);
    FdoPtr<FdoDataPropertyDefinitionCollection>(clas->GetIdentityProperties())->Add(dpd);

    dpd = FdoDataPropertyDefinition::Create(L"StringEnum", L"");
    FdoPtr<FdoPropertyValueConstraintList> listConstraint = FdoPropertyValueConstraintList::Create();
    FdoPtr<FdoDataValueCollection> listValues = listConstraint->GetConstraintList();
    listValues->Add(FdoPtr<FdoDataValue>(FdoDataValue::Create(L"Up")));
    listValues->Add(FdoPtr<FdoDataValue>(FdoDataValue::Create(L"Down")));
    listValues->Add(FdoPtr<FdoDataValue>(FdoDataValue::Create(L"Left")));
    listValues->Add(FdoPtr<FdoDataValue>(FdoDataValue::Create(L"Right")));
    dpd->SetNullable(true);
    dpd->SetValueConstraint(listConstraint);
    dpd->SetDataType(FdoDataType_String);
    properties->Add(dpd);

    dpd = FdoDataPropertyDefinition::Create(L"Int32Enum", L"");
    listConstraint = FdoPropertyValueConstraintList::Create();
    listValues = listConstraint->GetConstraintList();
    listValues->Add(FdoPtr<FdoDataValue>(FdoDataValue::Create(1)));
    listValues->Add(FdoPtr<FdoDataValue>(FdoDataValue::Create(2)));
    listValues->Add(FdoPtr<FdoDataValue>(FdoDataValue::Create(3)));
    listValues->Add(FdoPtr<FdoDataValue>(FdoDataValue::Create(5)));
    listValues->Add(FdoPtr<FdoDataValue>(FdoDataValue::Create(7)));
    listValues->Add(FdoPtr<FdoDataValue>(FdoDataValue::Create(9)));
    dpd->SetNullable(true);
    dpd->SetValueConstraint(listConstraint);
    dpd->SetDataType(FdoDataType_Int32);
    properties->Add(dpd);

    applyschema->SetFeatureSchema(schema);
    applyschema->Execute();
}

void MasterTest::testConstraints()
{
	FdoPtr<FdoIConnection> conn = CreateConnection();
    FdoPtr<FdoDataPropertyDefinition> dpd;

    CreateEmptyShpFileWithConstraints(conn);

    //try out describe schema
    FdoPtr<FdoIDescribeSchema> desc = (FdoIDescribeSchema*)conn->CreateCommand(FdoCommandType_DescribeSchema);
    FdoPtr<FdoFeatureSchemaCollection> schemas = desc->Execute();
    FdoPtr<FdoFeatureSchema> schema2 = schemas->GetItem(0);
    FdoPtr<FdoClassCollection> ccol = schema2->GetClasses();
    FdoPtr<FdoClassDefinition> cdef = ccol->GetItem(L"MyClass");
    FdoPtr<FdoPropertyDefinitionCollection> pdc = cdef->GetProperties();


    FdoPtr<FdoPropertyDefinition> pd = pdc->GetItem(L"NotNullString");
    CPPUNIT_ASSERT_MESSAGE("Wrong property type", pd->GetPropertyType() == FdoPropertyType_DataProperty);
    dpd = FDO_SAFE_ADDREF(static_cast<FdoDataPropertyDefinition*>(pd.p));
    CPPUNIT_ASSERT_MESSAGE("Wrong data property type", dpd->GetDataType() == FdoDataType_String);
    CPPUNIT_ASSERT_MESSAGE("Wrong nullability", dpd->GetNullable() == false);

    pd = pdc->GetItem(L"StringRange");
    CPPUNIT_ASSERT_MESSAGE("Wrong property type", pd->GetPropertyType() == FdoPropertyType_DataProperty);
    dpd = FDO_SAFE_ADDREF(static_cast<FdoDataPropertyDefinition*>(pd.p));
    FdoPtr<FdoPropertyValueConstraint> valCons = dpd->GetValueConstraint();
    CPPUNIT_ASSERT_MESSAGE("Wrong constraint type", valCons->GetConstraintType() == FdoPropertyValueConstraintType_Range);
    FdoPtr<FdoPropertyValueConstraintRange> rangeValCons = FDO_SAFE_ADDREF(static_cast<FdoPropertyValueConstraintRange*>(valCons.p));
    CPPUNIT_ASSERT_MESSAGE("Wrong max inclusive type", rangeValCons->GetMaxInclusive() == false);
    CPPUNIT_ASSERT_MESSAGE("Wrong min inclusive type", rangeValCons->GetMinInclusive() == true);
    FdoCommonMiscUtil::IsEqualTo(FdoPtr<FdoDataValue>(rangeValCons->GetMaxValue()), FdoPtr<FdoDataValue>(FdoDataValue::Create(L"Rich")));
    FdoCommonMiscUtil::IsEqualTo(FdoPtr<FdoDataValue>(rangeValCons->GetMinValue()), FdoPtr<FdoDataValue>(FdoDataValue::Create(L"Poor")));

    pd = pdc->GetItem(L"Int32Range");
    CPPUNIT_ASSERT_MESSAGE("Wrong property type", pd->GetPropertyType() == FdoPropertyType_DataProperty);
    dpd = FDO_SAFE_ADDREF(static_cast<FdoDataPropertyDefinition*>(pd.p));
    valCons = dpd->GetValueConstraint();
    CPPUNIT_ASSERT_MESSAGE("Wrong constraint type", valCons->GetConstraintType() == FdoPropertyValueConstraintType_Range);
    rangeValCons = FDO_SAFE_ADDREF(static_cast<FdoPropertyValueConstraintRange*>(valCons.p));
    CPPUNIT_ASSERT_MESSAGE("Wrong max inclusive type", rangeValCons->GetMaxInclusive() == true);
    CPPUNIT_ASSERT_MESSAGE("Wrong min inclusive type", rangeValCons->GetMinInclusive() == true);
    FdoCommonMiscUtil::IsEqualTo(FdoPtr<FdoDataValue>(rangeValCons->GetMaxValue()), FdoPtr<FdoDataValue>(FdoDataValue::Create(10000)));
    CPPUNIT_ASSERT_MESSAGE("Wrong min inclusive", rangeValCons->GetMinValue() == NULL);
    // FdoExpressionEngine::IsEqualTo(FdoPtr<FdoDataValue>(rangeValCons->GetMinValue()), FdoPtr<FdoDataValue>(FdoDataValue::Create(0)));

    pd = pdc->GetItem(L"StringEnum");
    CPPUNIT_ASSERT_MESSAGE("Wrong property type", pd->GetPropertyType() == FdoPropertyType_DataProperty);
    dpd = FDO_SAFE_ADDREF(static_cast<FdoDataPropertyDefinition*>(pd.p));
    valCons = dpd->GetValueConstraint();
    CPPUNIT_ASSERT_MESSAGE("Wrong constraint type", valCons->GetConstraintType() == FdoPropertyValueConstraintType_List);
    FdoPtr<FdoPropertyValueConstraintList> listValCons = FDO_SAFE_ADDREF(static_cast<FdoPropertyValueConstraintList*>(valCons.p));
    FdoPtr<FdoDataValueCollection> dataValues = listValCons->GetConstraintList();
    FdoCommonMiscUtil::IsEqualTo(FdoPtr<FdoDataValue>(dataValues->GetItem(0)), FdoPtr<FdoDataValue>(FdoDataValue::Create(L"Up")));
    FdoCommonMiscUtil::IsEqualTo(FdoPtr<FdoDataValue>(dataValues->GetItem(1)), FdoPtr<FdoDataValue>(FdoDataValue::Create(L"Down")));
    FdoCommonMiscUtil::IsEqualTo(FdoPtr<FdoDataValue>(dataValues->GetItem(2)), FdoPtr<FdoDataValue>(FdoDataValue::Create(L"Left")));
    FdoCommonMiscUtil::IsEqualTo(FdoPtr<FdoDataValue>(dataValues->GetItem(3)), FdoPtr<FdoDataValue>(FdoDataValue::Create(L"Right")));

    pd = pdc->GetItem(L"Int32Enum");
    CPPUNIT_ASSERT_MESSAGE("Wrong property type", pd->GetPropertyType() == FdoPropertyType_DataProperty);
    dpd = FDO_SAFE_ADDREF(static_cast<FdoDataPropertyDefinition*>(pd.p));
    valCons = dpd->GetValueConstraint();
    CPPUNIT_ASSERT_MESSAGE("Wrong constraint type", valCons->GetConstraintType() == FdoPropertyValueConstraintType_List);
    listValCons = FDO_SAFE_ADDREF(static_cast<FdoPropertyValueConstraintList*>(valCons.p));
    dataValues = listValCons->GetConstraintList();
    FdoCommonMiscUtil::IsEqualTo(FdoPtr<FdoDataValue>(dataValues->GetItem(0)), FdoPtr<FdoDataValue>(FdoDataValue::Create(1)));
    FdoCommonMiscUtil::IsEqualTo(FdoPtr<FdoDataValue>(dataValues->GetItem(1)), FdoPtr<FdoDataValue>(FdoDataValue::Create(2)));
    FdoCommonMiscUtil::IsEqualTo(FdoPtr<FdoDataValue>(dataValues->GetItem(2)), FdoPtr<FdoDataValue>(FdoDataValue::Create(3)));
    FdoCommonMiscUtil::IsEqualTo(FdoPtr<FdoDataValue>(dataValues->GetItem(3)), FdoPtr<FdoDataValue>(FdoDataValue::Create(5)));
    FdoCommonMiscUtil::IsEqualTo(FdoPtr<FdoDataValue>(dataValues->GetItem(3)), FdoPtr<FdoDataValue>(FdoDataValue::Create(7)));
    FdoCommonMiscUtil::IsEqualTo(FdoPtr<FdoDataValue>(dataValues->GetItem(3)), FdoPtr<FdoDataValue>(FdoDataValue::Create(9)));


    //set up the insert command
    //we will execute it multiple times with different property values
    FdoPtr<FdoIInsert> insert = (FdoIInsert*)conn->CreateCommand(FdoCommandType_Insert);
    insert->SetFeatureClassName(L"MyClass");        

    FdoPtr<FdoPropertyValueCollection> propvals = insert->GetPropertyValues();

    FdoPtr<FdoStringValue> notnullstring = FdoStringValue::Create(L"");
    FdoPtr<FdoStringValue> stringrange = FdoStringValue::Create(L"");
    FdoPtr<FdoStringValue> stringenum = FdoStringValue::Create(L"");
    FdoPtr<FdoInt32Value> int32range = FdoInt32Value::Create(0);
    FdoPtr<FdoInt32Value> int32enum = FdoInt32Value::Create(0);
    FdoPtr<FdoGeometryValue> geomval = FdoGeometryValue::Create(NULL);

    FdoPtr<FdoPropertyValue> pvnotnullstring = FdoPropertyValue::Create(L"NotNullString", notnullstring);
    FdoPtr<FdoPropertyValue> pvstringrange = FdoPropertyValue::Create(L"StringRange", stringrange);
    FdoPtr<FdoPropertyValue> pvstringenum = FdoPropertyValue::Create(L"StringEnum", stringenum);
    FdoPtr<FdoPropertyValue> pvint32range = FdoPropertyValue::Create(L"Int32Range", int32range);
    FdoPtr<FdoPropertyValue> pvint32enum = FdoPropertyValue::Create(L"Int32Enum", int32enum);
    FdoPtr<FdoPropertyValue> pvgeom = FdoPropertyValue::Create(L"Geom", geomval);
        
    propvals->Add(pvnotnullstring);
    propvals->Add(pvstringrange);
    propvals->Add(pvstringenum);
    propvals->Add(pvint32range);
    propvals->Add(pvint32enum);
    propvals->Add(pvgeom);

    // Insert correct not-null string:
    notnullstring->SetString(L"I'm not null!");
    stringrange->SetNull();
    stringenum->SetNull();
    int32range->SetNull();
    int32enum->SetNull();
    FdoPtr<FdoIFeatureReader> reader = insert->Execute();

    // Insert incorrect not-null string:
    notnullstring->SetNull();
    stringrange->SetNull();
    stringenum->SetNull();
    int32range->SetNull();
    int32enum->SetNull();
    try
    {
        reader = insert->Execute();
        CPPUNIT_FAIL("Didnt receive expected exception");
    }
    catch (FdoException *e)
    {
        e->Release(); // we want an exception here, so dont fail
    }

    // Insert correct string range:
    notnullstring->SetString(L"I'm not null!");
    stringrange->SetString(L"Poor");
    stringenum->SetNull();
    int32range->SetNull();
    int32enum->SetNull();
    reader = insert->Execute();

    // Insert incorrect string range:
    notnullstring->SetString(L"I'm not null!");
    stringrange->SetString(L"Rich");
    stringenum->SetNull();
    int32range->SetNull();
    int32enum->SetNull();
    try
    {
        reader = insert->Execute();
        CPPUNIT_FAIL("Didnt receive expected exception");
    }
    catch (FdoException *e)
    {
        e->Release(); // we want an exception here, so dont fail
    }

    // Insert correct string enum:
    notnullstring->SetString(L"I'm not null!");
    stringrange->SetNull();
    stringenum->SetString(L"Down");
    int32range->SetNull();
    int32enum->SetNull();
    reader = insert->Execute();

    // Insert incorrect string enum:
    notnullstring->SetString(L"I'm not null!");
    stringrange->SetNull();
    stringenum->SetString(L"Bottom");
    int32range->SetNull();
    int32enum->SetNull();
    try
    {
        reader = insert->Execute();
        CPPUNIT_FAIL("Didnt receive expected exception");
    }
    catch (FdoException *e)
    {
        e->Release(); // we want an exception here, so dont fail
    }

    // Insert correct int32 range:
    notnullstring->SetString(L"I'm not null!");
    stringrange->SetNull();
    stringenum->SetNull();
    int32range->SetInt32(333);
    int32enum->SetNull();
    reader = insert->Execute();

    // Insert incorrect int32 range:
    notnullstring->SetString(L"I'm not null!");
    stringrange->SetNull();
    stringenum->SetNull();
    int32range->SetInt32(10001);
    int32enum->SetNull();
    try
    {
        reader = insert->Execute();
        CPPUNIT_FAIL("Didnt receive expected exception");
    }
    catch (FdoException *e)
    {
        e->Release(); // we want an exception here, so dont fail
    }

    // Insert correct int32 enum:
    notnullstring->SetString(L"I'm not null!");
    stringrange->SetNull();
    stringenum->SetNull();
    int32range->SetNull();
    int32enum->SetInt32(9);
    reader = insert->Execute();

    // Insert incorrect int32 enum:
    notnullstring->SetString(L"I'm not null!");
    stringrange->SetNull();
    stringenum->SetNull();
    int32range->SetNull();
    int32enum->SetInt32(11);
    try
    {
        reader = insert->Execute();
        CPPUNIT_FAIL("Didnt receive expected exception");
    }
    catch (FdoException *e)
    {
        e->Release(); // we want an exception here, so dont fail
    }


	conn->Close();
}

void MasterTest::noGeomObject()
{
    FdoPtr<FdoIConnection> conn = CreateConnection();
	UnitTestUtil::CreateData( true , conn, 10 );

	// Let's insert more feature with no geometry
	FdoPtr<FdoIInsert> insert = (FdoIInsert*)conn->CreateCommand(FdoCommandType_Insert);

    FdoPtr<FdoPropertyValueCollection> propvals = insert->GetPropertyValues();

	FdoPtr<FdoStringValue> svname = FdoStringValue::Create(L"");
    FdoPtr<FdoStringValue> svkey = FdoStringValue::Create(L"");
	FdoPtr<FdoPropertyValue> pvname = FdoPropertyValue::Create(L"Name", svname);
    FdoPtr<FdoPropertyValue> pvkey = FdoPropertyValue::Create(L"Key", svkey);

	FdoPtr<FdoGeometryValue> gvgeom = FdoGeometryValue::Create();
	gvgeom->SetNullValue();
	FdoPtr<FdoPropertyValue> pvgeom = FdoPropertyValue::Create(L"Data", gvgeom);

	
	insert->SetFeatureClassName(L"Parcel");  

	propvals->Add(pvname);
    propvals->Add(pvkey);
	propvals->Add(pvgeom);

	svname->SetString(L"My Test Object");
	svkey->SetString(L"AB002X3B");
	FdoPtr<FdoIFeatureReader>rdr = insert->Execute();
	CPPUNIT_ASSERT_MESSAGE("Unable to create an object with no geometry", rdr->ReadNext() );
	printf("Insert new object: ID(%d)\n", rdr->GetInt32(L"ID") );
    
	FdoPtr<FdoClassDefinition>cls = rdr->GetClassDefinition();
	CPPUNIT_ASSERT_MESSAGE("Wrong class name",wcscmp(L"Parcel",cls->GetName())==0);
	
	conn->Close();
}

void MasterTest::numberFilter()
{
    FdoPtr<FdoIConnection> conn = CreateConnection();
	UnitTestUtil::CreateData( true , conn, 10 );
	conn->Close();
	UnitTestUtil::CreateData( false , conn, 10, L"ParcelChild" );
    conn->Close();
	UnitTestUtil::CreateData( false , conn, 400 );
	conn->Close();
	openConnection(conn, DESTINATION_FILE);

    FdoPtr<FdoISelect> select = (FdoISelect*)conn->CreateCommand(FdoCommandType_Select); 

    select->SetFeatureClassName(L"Parcel");

    FdoPtr<FdoFilter> filter = FdoFilter::Parse(L"ID <= 300");


    select->SetFilter(filter);
    
    FdoPtr<FdoIFeatureReader> rdr = select->Execute();

    int count2 = 0;


    FdoPtr<FdoByteArray> geom;
	int id = 0;
    while (rdr->ReadNext())
    {
		FdoPtr<FdoPropertyDefinition> gp;
		FdoPtr<FdoClassDefinition>cls = rdr->GetClassDefinition();
        const wchar_t* something = rdr->GetString(L"Key");
		if( (id=rdr->GetInt32(L"ID")) <= 10 )
		{
			CPPUNIT_ASSERT_MESSAGE("Wrong class name",wcscmp(L"Parcel",cls->GetName())==0);
		    gp = (FdoPtr<FdoPropertyDefinitionCollection>(cls->GetProperties()))->GetItem(L"Data");
		}
		else if ( (id=rdr->GetInt32(L"ID")) <= 20  )
		{
			CPPUNIT_ASSERT_MESSAGE("Wrong class name",wcscmp(L"ParcelChild",cls->GetName())==0);
			gp = (FdoPtr<FdoReadOnlyPropertyDefinitionCollection>(cls->GetBaseProperties()))->GetItem(L"Data");
		}
		//printf("key(%ls) ID(%d) number(%d) \n",something, rdr->GetInt32(L"ID"), rdr->GetInt32(L"Numb"));
	    if( gp != NULL )
		{
			FdoGeometricPropertyDefinition *geom = (FdoGeometricPropertyDefinition*)gp.p;
			CPPUNIT_ASSERT_MESSAGE("Expected a not null geometry property Spatial context", geom->GetSpatialContextAssociation() != NULL );
		}
        count2++;
    }


    rdr->Close();
    conn->Close();

	CPPUNIT_ASSERT_MESSAGE("Wrong number of returned objects", count2==300 );
}

void MasterTest::inFilter()
{
    FdoPtr<FdoIConnection> conn = CreateConnection();
	UnitTestUtil::CreateData( true , conn, 200 );
	UnitTestUtil::CreateData( false , conn, 200, L"ParcelChild" );
	UnitTestUtil::CreateData( false , conn, 400 );
   
    FdoPtr<FdoISelect> select = (FdoISelect*)conn->CreateCommand(FdoCommandType_Select); 

    select->SetFeatureClassName(L"ParcelChild");
	FdoPtr<FdoIdentifierCollection> props = select->GetPropertyNames();
	props->Add(FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"DblNumb1")) );
	props->Add(FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"Key")) );
	props->Add(FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"ID")) );
	props->Add(FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"Numb")) );
    select->SetFilter(FdoPtr<FdoFilter>(FdoFilter::Parse(L"Numb in (120,150,170,191)")));
    
    FdoPtr<FdoIFeatureReader> rdr = select->Execute();

    int count2 = 0;

    FdoPtr<FdoByteArray> geom;

    while (rdr->ReadNext())
    {
		CPPUNIT_ASSERT_MESSAGE("Property should be NULL", rdr->IsNull(L"DblNumb1") );
		
        const wchar_t* something = rdr->GetString(L"Key");
		FdoPtr<FdoClassDefinition>cls = rdr->GetClassDefinition();
		CPPUNIT_ASSERT_MESSAGE("Wrong class name",wcscmp(L"ParcelChild",cls->GetName())==0);
		//printf("Class name = %ls Numb=%d\n", cls->GetName(), rdr->GetInt32(L"Numb") );
		printf("key(%ls) ID(%d) number(%d) \n",something, rdr->GetInt32(L"ID"), rdr->GetInt32(L"Numb"));
        count2++;
		CPPUNIT_ASSERT_MESSAGE( "The property should be NULL", rdr->IsNull(L"DblNumb2") );
		
	}

    rdr->Close();
    conn->Close();

	CPPUNIT_ASSERT_MESSAGE("Wrong number of returned objects", count2==4 );
}

void MasterTest::likeFilter()
{
    FdoPtr<FdoIConnection> conn = CreateConnection();
	UnitTestUtil::CreateData( true , conn, 200 );
	UnitTestUtil::CreateData( false , conn, 200, L"ParcelChild" );
	UnitTestUtil::CreateData( false , conn, 400 );
   
    FdoPtr<FdoISelect> select = (FdoISelect*)conn->CreateCommand(FdoCommandType_Select); 

    select->SetFeatureClassName(L"ParcelChild");
	FdoPtr<FdoIdentifierCollection> props = select->GetPropertyNames();
	props->Add(FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"DblNumb1")) );
	props->Add(FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"Key")) );
	props->Add(FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"ID")) );
	props->Add(FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"Numb")) );
    select->SetFilter(FdoPtr<FdoFilter>(FdoFilter::Parse(L"Key like 'AB007%'")));
    
    FdoPtr<FdoIFeatureReader> rdr = select->Execute();

    int count2 = 0;

    FdoPtr<FdoByteArray> geom;

    while (rdr->ReadNext())
    {
		CPPUNIT_ASSERT_MESSAGE("Property should be NULL", rdr->IsNull(L"DblNumb1") );
		
        const wchar_t* something = rdr->GetString(L"Key");
		FdoPtr<FdoClassDefinition>cls = rdr->GetClassDefinition();
		CPPUNIT_ASSERT_MESSAGE("Wrong class name",wcscmp(L"ParcelChild",cls->GetName())==0);
		//printf("Class name = %ls Numb=%d\n", cls->GetName(), rdr->GetInt32(L"Numb") );
		printf("key(%ls) ID(%d) number(%d) \n",something, rdr->GetInt32(L"ID"), rdr->GetInt32(L"Numb"));
        count2++;
		CPPUNIT_ASSERT_MESSAGE( "The property should be NULL", rdr->IsNull(L"DblNumb2") );
		
	}

    rdr->Close();
    conn->Close();

	CPPUNIT_ASSERT_MESSAGE("Wrong number of returned objects", count2==10 );
}

void MasterTest::orFilter()
{
    FdoPtr<FdoIConnection> conn = CreateConnection();
	UnitTestUtil::CreateData( true , conn, 200 );
	UnitTestUtil::CreateData( false , conn, 200, L"ParcelChild" );
	UnitTestUtil::CreateData( false , conn, 400 );
   
    FdoPtr<FdoISelect> select = (FdoISelect*)conn->CreateCommand(FdoCommandType_Select); 

    select->SetFeatureClassName(L"ParcelChild");
	FdoPtr<FdoIdentifierCollection> props = select->GetPropertyNames();
	props->Add(FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"DblNumb1")) );
	props->Add(FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"Key")) );
	props->Add(FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"ID")) );
	props->Add(FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"Numb")) );
	select->SetFilter(FdoPtr<FdoFilter>(FdoFilter::Parse(L" 1=0 or (Numb = 120 and Key like 'AB007%') or (Numb = 150 and Key like 'AB0%') or Numb = 170 or Numb = 191")));
    
    FdoPtr<FdoIFeatureReader> rdr = select->Execute();

    int count2 = 0;

    FdoPtr<FdoByteArray> geom;

    while (rdr->ReadNext())
    {
		CPPUNIT_ASSERT_MESSAGE("Property should be NULL", rdr->IsNull(L"DblNumb1") );
		
        const wchar_t* something = rdr->GetString(L"Key");
		FdoPtr<FdoClassDefinition>cls = rdr->GetClassDefinition();
		CPPUNIT_ASSERT_MESSAGE("Wrong class name",wcscmp(L"ParcelChild",cls->GetName())==0);
		//printf("Class name = %ls Numb=%d\n", cls->GetName(), rdr->GetInt32(L"Numb") );
		printf("key(%ls) ID(%d) number(%d) \n",something, rdr->GetInt32(L"ID"), rdr->GetInt32(L"Numb"));
        count2++;
		CPPUNIT_ASSERT_MESSAGE( "The property should be NULL", rdr->IsNull(L"DblNumb2") );
		
	}

    rdr->Close();
    conn->Close();

	CPPUNIT_ASSERT_MESSAGE("Wrong number of returned objects", count2==4 );
}

void MasterTest::dateFilter()
{
    FdoPtr<FdoIConnection> conn = CreateConnection();
	UnitTestUtil::CreateData( true , conn, 200 );
	//conn->Close();
	UnitTestUtil::CreateData( false , conn, 200, L"ParcelChild" );
    //conn->Close();
   
    FdoPtr<FdoISelect> select = (FdoISelect*)conn->CreateCommand(FdoCommandType_Select); 
    try
	{
		select->SetFeatureClassName(L"ParcelChild");  
		select->SetFilter(FdoPtr<FdoFilter>(FdoFilter::Parse(L"datetime = TIMESTAMP '2006-04-21 19:40:10.0001'")));
	/*
	    // The object version of the same filter
		FdoPtr<FdoComparisonCondition>pCompare = FdoComparisonCondition::Create(
							FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"datetime")), 
							FdoComparisonOperations_EqualTo, 
							FdoPtr<FdoDateTimeValue>( FdoDateTimeValue::Create(FdoDateTime(2006,4,21,19,40,10.0001) ) ) ); 
		select->SetFilter(pCompare);
	*/
	    
		int   gold_data[] = {100,160,220,280};
		int count = 0;
		FdoPtr<FdoIFeatureReader> rdr = select->Execute();
		while (rdr->ReadNext())
		{
			if( count < 4 )
				CPPUNIT_ASSERT_MESSAGE("Wrong return value", gold_data[count++]==rdr->GetInt32(L"Numb") );
		}
		CPPUNIT_ASSERT_MESSAGE("Wrong number of returned objects", count==4 );
	}
	catch(FdoException *exp )
	{
		printf("Exception: %ls \n", exp->GetExceptionMessage() );
		throw;
	}
    conn->Close();
}

void MasterTest::testSpecialClassNames()
{        
#ifdef _WIN32
    DeleteFileW( DEST_PATH2 );
#else
	size_t len = wcstombs(NULL, DEST_PATH2, 0);
    char* mbsPath = new char[len+1];
    wcstombs(mbsPath, DEST_PATH2, len+1);
	unlink( mbsPath );
	delete mbsPath;
#endif
	try
	{
		FdoPtr<FdoIConnection> conn = CreateConnection();
		FdoPtr<FdoICreateDataStore>	pCreateCmd = (FdoICreateDataStore*) conn->CreateCommand(FdoCommandType_CreateDataStore);
		FdoPtr<FdoIDataStorePropertyDictionary> dictionary = pCreateCmd->GetDataStoreProperties();

		int	count;
		FdoString **names = dictionary->GetPropertyNames(count);
		dictionary->SetProperty( names[0], DEST_PATH2 );
		pCreateCmd->Execute();	


		openConnection(conn, DEST_PATH2); 
		FdoPtr<FdoIApplySchema> applyschema = (FdoIApplySchema*)conn->CreateCommand(FdoCommandType_ApplySchema);
		
		FdoPtr<FdoFeatureSchema> schema = FdoFeatureSchema::Create(L"SDFTestSchema",L"Schema to test special class names");

		FdoPtr<FdoFeatureClass> clas = FdoFeatureClass::Create(L"special-class",L"");    
		FdoPtr<FdoClassCollection>(schema->GetClasses())->Add(clas);
	        
		FdoPtr<FdoPropertyDefinitionCollection> properties = clas->GetProperties();

		FdoPtr<FdoDataPropertyDefinition> dpd = FdoDataPropertyDefinition::Create(L"Name", L"The name of the object");
		dpd->SetDataType(FdoDataType_String);
		properties->Add(dpd);

		dpd = FdoDataPropertyDefinition::Create(L"ID", L"The autogenerated sequence ID of the object");
		dpd->SetDataType(FdoDataType_Int32);
		dpd->SetIsAutoGenerated(true);
		properties->Add(dpd);
		FdoPtr<FdoDataPropertyDefinitionCollection>(clas->GetIdentityProperties())->Add(dpd);

		clas = FdoFeatureClass::Create(L"special*class",L"");    
		FdoPtr<FdoClassCollection>(schema->GetClasses())->Add(clas);
	        
		properties = clas->GetProperties();

		dpd = FdoDataPropertyDefinition::Create(L"Name", L"The name of the object");
		dpd->SetDataType(FdoDataType_String);
		properties->Add(dpd);

		dpd = FdoDataPropertyDefinition::Create(L"ID", L"The autogenerated sequence ID of the object");
		dpd->SetDataType(FdoDataType_Int32);
		dpd->SetIsAutoGenerated(true);
		properties->Add(dpd);
		FdoPtr<FdoDataPropertyDefinitionCollection>(clas->GetIdentityProperties())->Add(dpd);

		applyschema->SetFeatureSchema(schema);
		applyschema->Execute();
	}
	catch(FdoException* e)
    {
        wprintf(L"%s\n", e->GetExceptionMessage());
        e->Release();
		CPPUNIT_FAIL("Unexpected FdoException");
    }
}

void MasterTest::selectAggregatesSpatialExtentsTest()
{
    printf("Testing select aggregates -- Spatial Extents:\n");

    FdoPtr<FdoISelectAggregates> selAggr;
    FdoPtr<FdoIDataReader> rdr;
    FdoPtr<FdoIdentifierCollection> ids;
    FdoPtr<FdoExpression> expr;
    FdoPtr<FdoComputedIdentifier> cid;
    int count;
    FdoPtr<FdoIConnection> conn = CreateConnection();


    openConnection(conn, SHP_PATH2, true);

    FdoPtr<FdoISelectAggregates> advsel = (FdoISelectAggregates*)(conn->CreateCommand(FdoCommandType_SelectAggregates));

    advsel->SetFeatureClassName(L"province");
    
    ids = advsel->GetPropertyNames();

    expr = FdoExpression::Parse(L"SpatialExtents(SHPGEOM)");
    cid = FdoComputedIdentifier::Create(L"MBR", expr);
    ids->Add(cid);


    // Test the optimized case (no filter, no other identifiers in select property list):
    ////////////////////////////////////////////////////////////////////////////////////

    FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
    rdr = advsel->Execute();

    count = 0;

    FdoPtr<FdoIEnvelope> envelopeAllWithoutFilter;
    while (rdr->ReadNext())
    {
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


    // Test the non-optimized case (with a filter provided):
    ////////////////////////////////////////////////////////////////////////////////////

    advsel->SetFilter(L"AREA != -1.0");  // this is an arbitrary value that should allow all rows through
    rdr = advsel->Execute();

    count = 0;
    FdoPtr<FdoIEnvelope> envelopeAllWithFilter;
    while (rdr->ReadNext())
    {
        FdoPtr<FdoByteArray> geomBytes = rdr->GetGeometry(L"MBR");
        FdoPtr<FdoIGeometry> geom = gf->CreateGeometryFromFgf(geomBytes);

        FdoGeometryType geomType = geom->GetDerivedType();
        if (geomType != FdoGeometryType_Polygon)
		    CPPUNIT_FAIL("Expected Polygon geometry for SpatialExtents() result");

        envelopeAllWithFilter = geom->GetEnvelope();
        if (envelopeAllWithFilter->GetIsEmpty())
            CPPUNIT_FAIL("Expected non-empty envelope for SpatialExtents() result");

        count++;
    }
    CPPUNIT_ASSERT_MESSAGE("Expected exactly one row of aggregate data", count==1);

    rdr->Close();

    // Compare the two "all" results to make sure they are equivalent:
    CPPUNIT_ASSERT_MESSAGE("SpatialExtents results don't match (MinX)", envelopeAllWithoutFilter->GetMinX() == envelopeAllWithFilter->GetMinX());
    CPPUNIT_ASSERT_MESSAGE("SpatialExtents results don't match (MinY)", envelopeAllWithoutFilter->GetMinY() == envelopeAllWithFilter->GetMinY());
    CPPUNIT_ASSERT_MESSAGE("SpatialExtents results don't match (MinZ)", FdoCommonOSUtil::_isnan(envelopeAllWithoutFilter->GetMinZ()) && FdoCommonOSUtil::_isnan(envelopeAllWithFilter->GetMinZ()));
    CPPUNIT_ASSERT_MESSAGE("SpatialExtents results don't match (MaxX)", envelopeAllWithoutFilter->GetMaxX() == envelopeAllWithFilter->GetMaxX());
    CPPUNIT_ASSERT_MESSAGE("SpatialExtents results don't match (MaxY)", envelopeAllWithoutFilter->GetMaxY() == envelopeAllWithFilter->GetMaxY());
    CPPUNIT_ASSERT_MESSAGE("SpatialExtents results don't match (MaxZ)", FdoCommonOSUtil::_isnan(envelopeAllWithoutFilter->GetMaxZ()) && FdoCommonOSUtil::_isnan(envelopeAllWithFilter->GetMaxZ()));


    // Test the non-optimized case (filter that returns NULL):
    ////////////////////////////////////////////////////////////////////////////////////

    advsel->SetFilter(L"AREA = -1");  // will select 0 rows
    rdr = advsel->Execute();

    count = 0;
    while (rdr->ReadNext())
    {
        CPPUNIT_ASSERT_MESSAGE("Expected NULL result from SpatialExtents with filter returning 0 rows", rdr->IsNull(L"MBR"));
        count++;
    }
    CPPUNIT_ASSERT_MESSAGE("Expected exactly one row of aggregate data", count==1);

    rdr->Close();


    // Test the non-optimized case (no filter, more than one thing in the identifier property list):
    ////////////////////////////////////////////////////////////////////////////////////

    advsel->SetFilter((FdoFilter*)NULL);  // no filter
    ids->Add(FdoPtr<FdoIdentifier>(FdoComputedIdentifier::Create(L"MyCount",
                FdoPtr<FdoExpression>(FdoExpression::Parse(L"Count(AREA)"))
             )));
    rdr = advsel->Execute();

    count = 0;
    FdoPtr<FdoIEnvelope> envelopeAllWithoutFilterMultipleIds;
    while (rdr->ReadNext())
    {
        FdoPtr<FdoByteArray> geomBytes = rdr->GetGeometry(L"MBR");
        FdoPtr<FdoIGeometry> geom = gf->CreateGeometryFromFgf(geomBytes);

        FdoGeometryType geomType = geom->GetDerivedType();
        if (geomType != FdoGeometryType_Polygon)
		    CPPUNIT_FAIL("Expected Polygon geometry for SpatialExtents() result");

        envelopeAllWithoutFilterMultipleIds = geom->GetEnvelope();
        if (envelopeAllWithoutFilterMultipleIds->GetIsEmpty())
            CPPUNIT_FAIL("Expected non-empty envelope for SpatialExtents() result");

        FdoInt64 mycount = rdr->GetInt64(L"MyCount");
        CPPUNIT_ASSERT_MESSAGE("Wrong row count", 12 == mycount);

        count++;
    }
    CPPUNIT_ASSERT_MESSAGE("Expected exactly one row of aggregate data", count==1);

    rdr->Close();

    // Compare the two "all" results to make sure they are equivalent:
    CPPUNIT_ASSERT_MESSAGE("SpatialExtents results don't match (MinX)", envelopeAllWithoutFilter->GetMinX() == envelopeAllWithoutFilterMultipleIds->GetMinX());
    CPPUNIT_ASSERT_MESSAGE("SpatialExtents results don't match (MinY)", envelopeAllWithoutFilter->GetMinY() == envelopeAllWithoutFilterMultipleIds->GetMinY());
    CPPUNIT_ASSERT_MESSAGE("SpatialExtents results don't match (MinZ)", FdoCommonOSUtil::_isnan(envelopeAllWithoutFilter->GetMinZ()) && FdoCommonOSUtil::_isnan(envelopeAllWithoutFilterMultipleIds->GetMinZ()));
    CPPUNIT_ASSERT_MESSAGE("SpatialExtents results don't match (MaxX)", envelopeAllWithoutFilter->GetMaxX() == envelopeAllWithoutFilterMultipleIds->GetMaxX());
    CPPUNIT_ASSERT_MESSAGE("SpatialExtents results don't match (MaxY)", envelopeAllWithoutFilter->GetMaxY() == envelopeAllWithoutFilterMultipleIds->GetMaxY());
    CPPUNIT_ASSERT_MESSAGE("SpatialExtents results don't match (MaxZ)", FdoCommonOSUtil::_isnan(envelopeAllWithoutFilter->GetMaxZ()) && FdoCommonOSUtil::_isnan(envelopeAllWithoutFilterMultipleIds->GetMaxZ()));


    // Create new empty SHP file :
    ////////////////////////////////////////////////////////////////////////////////////

    conn->Close();
    CreateEmptyShpFileWithConstraints(conn);


    // Test the optimized case (no filter, no rows of data):
    ////////////////////////////////////////////////////////////////////////////////////

    selAggr = (FdoISelectAggregates*)conn->CreateCommand(FdoCommandType_SelectAggregates);
    selAggr->SetFeatureClassName(L"MyClass");
    ids = selAggr->GetPropertyNames();

    expr = FdoExpression::Parse(L"SpatialExtents(Geom)");
    cid = FdoComputedIdentifier::Create(L"MBR", expr);
    ids->Add(cid);

    ids->Add(FdoPtr<FdoIdentifier>(FdoComputedIdentifier::Create(L"MyCount",
                FdoPtr<FdoExpression>(FdoExpression::Parse(L"Count(AREA)"))
             )));

    rdr = selAggr->Execute();
    count = 0;
    while (rdr->ReadNext())
    {
        CPPUNIT_ASSERT_MESSAGE("Expected NULL result from SpatialExtents with filter returning 0 rows", rdr->IsNull(L"MBR"));
        count++;
    }
    CPPUNIT_ASSERT_MESSAGE("Expected exactly one row of aggregate data", count==1);

    rdr->Close();


    // Insert many rows of data, for performance tests :
    ////////////////////////////////////////////////////////////////////////////////////

    const int PERF_TEST_NUM_ROWS = 4000;
    clock_t start;
    clock_t finish;
    FdoPtr<FdoIFeatureReader> featRdr;

    FdoPtr<FdoIInsert> ins = (FdoIInsert*)conn->CreateCommand(FdoCommandType_Insert);
    ins->SetFeatureClassName(L"MyClass");
    FdoPtr<FdoPropertyValueCollection> propVals = ins->GetPropertyValues();
    FdoPtr<FdoFgfGeometryFactory> fgf = FdoFgfGeometryFactory::GetInstance();
    double ordinates[2*2];  //2 XY coordinates
    ordinates[0] =  10.0; ordinates[1] =  10.0;
    ordinates[2] = 100.0; ordinates[3] = 100.0;
    FdoPtr<FdoIGeometry> geom = fgf->CreateLineString(FdoDimensionality_XY, ELEMENTS(ordinates), ordinates);
    FdoPtr<FdoByteArray> geomBytes = fgf->GetFgf(geom);
    FdoPtr<FdoGeometryValue> geomVal = FdoGeometryValue::Create(geomBytes);
    FdoPtr<FdoPropertyValue> propVal = FdoPropertyValue::Create(L"Geom", geomVal);
    propVals->Add(propVal);
    propVals->Add(FdoPtr<FdoPropertyValue>(FdoPropertyValue::Create(L"NotNullString", FdoPtr<FdoValueExpression>(FdoStringValue::Create(L"Some String")))));
    for (int i=0; i<PERF_TEST_NUM_ROWS; i++)
    {
        FdoPtr<FdoPropertyValue> propVal = FdoPropertyValue::Create(L"Int32Range", FdoPtr<FdoValueExpression>(FdoInt32Value::Create(i)));
        propVals->Add(propVal);
        featRdr = ins->Execute();
        featRdr->Close();
        propVals->Remove(propVal);
    }


    // Test the optimized case for performance (no filter, PERF_TEST_NUM_ROWS rows of data):
    ////////////////////////////////////////////////////////////////////////////////////

    selAggr = (FdoISelectAggregates*)conn->CreateCommand(FdoCommandType_SelectAggregates);
    selAggr->SetFeatureClassName(L"MyClass");
    ids = selAggr->GetPropertyNames();

    expr = FdoExpression::Parse(L"SpatialExtents(Geom)");
    cid = FdoComputedIdentifier::Create(L"MBR", expr);
    ids->Add(cid);
    ids->Add(FdoPtr<FdoIdentifier>(FdoComputedIdentifier::Create(L"MyCount",
                FdoPtr<FdoExpression>(FdoExpression::Parse(L"Count(AREA)"))
             )));
    start = clock();
    rdr = selAggr->Execute();
    count = 0;
    while (rdr->ReadNext())
    {
        CPPUNIT_ASSERT_MESSAGE("Expected not-null result from SpatialExtents with filter returning all rows", !rdr->IsNull(L"MBR"));
        FdoPtr<FdoByteArray> bytes = rdr->GetGeometry(L"MBR");
        count++;
    }
    CPPUNIT_ASSERT_MESSAGE("Expected exactly one row of aggregate data", count==1);
    rdr->Close();
    finish = clock();

    wprintf(L"Seconds to read SpatialExtents from SHP file containing %d records, using optimization: %g\n", PERF_TEST_NUM_ROWS, (double)(finish-start)/CLOCKS_PER_SEC);


    // Test the non-optimized case for performance (no filter, PERF_TEST_NUM_ROWS rows of data):
    ////////////////////////////////////////////////////////////////////////////////////

    selAggr = (FdoISelectAggregates*)conn->CreateCommand(FdoCommandType_SelectAggregates);
    selAggr->SetFeatureClassName(L"MyClass");
    selAggr->SetFilter(L"NotNullString != 'Garbage'");
    ids = selAggr->GetPropertyNames();

    expr = FdoExpression::Parse(L"SpatialExtents(Geom)");
    cid = FdoComputedIdentifier::Create(L"MBR", expr);
    ids->Add(cid);

    start = clock();
    rdr = selAggr->Execute();
    count = 0;
    while (rdr->ReadNext())
    {
        CPPUNIT_ASSERT_MESSAGE("Expected not-null result from SpatialExtents with filter returning all rows", !rdr->IsNull(L"MBR"));
        FdoPtr<FdoByteArray> bytes = rdr->GetGeometry(L"MBR");
        count++;
    }
    CPPUNIT_ASSERT_MESSAGE("Expected exactly one row of aggregate data", count==1);
    rdr->Close();
    finish = clock();

    wprintf(L"Seconds to read SpatialExtents from SHP file containing %d records, using no optimization: %g\n", PERF_TEST_NUM_ROWS, (double)(finish-start)/CLOCKS_PER_SEC);


    // Done all tests; let's clean up:
    ////////////////////////////////////////////////////////////////////////////////////

    conn->Close();    
}
