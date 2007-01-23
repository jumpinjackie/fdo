/*
 * Copyright (C) 2004-2006  Autodesk, Inc.
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of version 2.1 of the GNU Lesser
 * General Public License as published by the Free Software Foundation.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include "Pch.h"
#include "SelectTests.h"

CPPUNIT_TEST_SUITE_REGISTRATION (SelectTests);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION (SelectTests, "SelectTests");


FdoPtr<FdoIConnection> SelectTests::mConnection;

SelectTests::SelectTests (void) :
    nullableClass(NULL),
    property1(NULL),
    property0(NULL)
{
}

SelectTests::~SelectTests (void)
{
}

void SelectTests::setUp ()
{
    static bool bSchemaCreated = false;

    // Setup test data:
    property0 = new UnitTestData(
        L"Pointer",           // property_name
        L"Unique identifier", // property_description
        FdoDataType_Int32,    // property_type
        0,                    // property_length
        0,                    // property_precision
        0,                    // property_scale
        false,                // property_nullable
        true,                 // property_readonly
        true,                 // property_auto
        (wchar_t*)NULL, (wchar_t*)NULL, (wchar_t*)NULL); // values
    property1 = new UnitTestData(
        L"MyNullable",        // property_name
        L"Nullable property", // property_description
        FdoDataType_String,   // property_type
        512,                  // property_length
        0,                    // property_precision
        0,                    // property_scale
        true,                 // property_nullable
        false,                // property_readonly
        false,                // property_auto
        L"'some string'", L"null", (wchar_t*)NULL); // values
    nullableClass = new UnitTestClass(
        ArcSDETestConfig::QClassNameLumpy(),
        L"Test class definition.",
        0,
        false,
        true,
        property0, property1, NULL);

    // Connect:
    if (!CreateSchemaOnly() || !bSchemaCreated)
    {
	    mConnection = ArcSDETests::GetConnection ();
        mConnection->SetConnectionString (ArcSDETestConfig::ConnStringAustralia());
        mConnection->Open ();
    }

    // Create test classes, if requested:
    if (CreateSchemaOnly() && !bSchemaCreated)
    {
        // clean up previous tests
        CleanUpClass(mConnection, ArcSDETestConfig::ClassSchemaLumpy(), nullableClass->mName);

        // establish a class to play with
        ArcSDETests::CreateSchema (mConnection, nullableClass);

        // Create a table for performance testing:
        create_large_table_with_geom();

        bSchemaCreated = true;
    }
}

void SelectTests::tearDown ()
{
    if ((mConnection != NULL) && (mConnection->GetConnectionState() != FdoConnectionState_Closed))
        mConnection->Close ();

    // Cleanup test data:
    delete nullableClass;  nullableClass = NULL;
    delete property1;  property1 = NULL;
    delete property0;  property0 = NULL;
}

void SelectTests::simple ()
{
    if (CreateSchemaOnly()) return;

    try
    {
        double secs_start = ((double)(long)clock()) / 1000.0;
        printf("\nSelect and read all properties begin: clock=%f\n", secs_start);

        FdoPtr<FdoISelect> select = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
        select->SetFeatureClassName (ArcSDETestConfig::QClassNameOntarioRoads());
        FdoPtr<FdoIFeatureReader> reader = select->Execute ();
        FdoPtr<FdoFeatureClass> classDef = (FdoFeatureClass*)reader->GetClassDefinition();
        while (reader->ReadNext ())
            ProcessFeature (reader, classDef);
        reader->Close();

	    // test identifier collection:
	    FdoPtr<FdoIdentifierCollection> IDs = select->GetPropertyNames();
	    FdoPtr<FdoIdentifier> areaID = FdoIdentifier::Create(L"MNRCODE");
	    FdoPtr<FdoIdentifier> nameID = FdoIdentifier::Create(L"FCODE");
	    IDs->Add(areaID);
	    IDs->Add(nameID);
        reader = select->Execute ();
        classDef = (FdoFeatureClass*)reader->GetClassDefinition();
		long lCount=0;
        while (reader->ReadNext ())
		{
			lCount++;
            ProcessFeature (reader, classDef, IDs);
		}
		printf("\nFeatures Read=%d\n", lCount);
        reader->Close();

        double secs_end = ((double)(long)clock()) / 1000.0;
		printf("\nFeatures Read=%d\n", lCount);
        printf("\nSelect and read all properties end: clock=%f, elapsed time=%f, features per sec=%f\n", secs_end, secs_end-secs_start, (double)lCount/(secs_end-secs_start));
    }
    catch (FdoException *e)
    {
        fail(e);
    }
}

void SelectTests::get_class_before_readnext ()
{
    if (CreateSchemaOnly()) return;

    try
    {
        FdoPtr<FdoISelect> select = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
        select->SetFeatureClassName (ArcSDETestConfig::QClassNameParcels());
        FdoPtr<FdoIFeatureReader> reader = select->Execute ();
        bool accessible = false;
        try
        {
            FdoPtr<FdoClassDefinition> definition = reader->GetClassDefinition ();
            CPPUNIT_ASSERT_MESSAGE ("wrong class", 0 == wcscmp (ArcSDETestConfig::ClassNameParcels(), definition->GetName ()));
            accessible = true;

            // GetDepth() should also work before ReadNext():
            reader->GetDepth ();
        }
        catch (FdoException *e)
        {
            fail(e);
        }
        reader->Close();
        CPPUNIT_ASSERT_MESSAGE ("class definition is not accessible before ReadNext()", accessible);
    }
    catch (FdoException *e)
    {
        fail(e);
    }
}

/* Test attribute filter. */
void SelectTests::simple_filter ()
{
    if (CreateSchemaOnly()) return;

    try
    {
        FdoPtr<FdoISelect> select = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
        select->SetFeatureClassName (ArcSDETestConfig::QClassNameOntarioRoads());
        select->SetFilter (FdoPtr<FdoFilter>(FdoFilter::Parse (L"objectid < 3")));
        FdoPtr<FdoIFeatureReader> reader = select->Execute ();
        FdoPtr<FdoFeatureClass> classDef = (FdoFeatureClass*)reader->GetClassDefinition();
        while (reader->ReadNext ())
            ProcessFeature (reader, classDef);
        reader->Close();
    }
    catch (FdoException *e)
    {
        fail(e);
    }
}

/* Test string attribute filter containing a quote. */
void SelectTests::string_with_quote ()
{
    if (CreateSchemaOnly()) return;

    try
    {
        FdoPtr<FdoISelect> select = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
        select->SetFeatureClassName (ArcSDETestConfig::QClassNameParcels());
        select->SetFilter (FdoPtr<FdoFilter>(FdoFilter::Parse (L"RLDESCR4 = 'SW1/4 SEC 34; TH S 89*44''25\"'")));
        FdoPtr<FdoIFeatureReader> reader = select->Execute ();
        FdoPtr<FdoFeatureClass> classDef = (FdoFeatureClass*)reader->GetClassDefinition();
        bool found = false;
        while (reader->ReadNext ())
        {
            ProcessFeature (reader, classDef);
            found = true;
        }
        reader->Close();
        CPPUNIT_ASSERT_MESSAGE ("did not find string with a quote", found);
    }
    catch (FdoException *e)
    {
        fail(e);
    }
}

void SelectTests::simple_erroneous_filter ()
{
    if (CreateSchemaOnly()) return;

    try
    {
        FdoPtr<FdoISelect> select = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
        select->SetFeatureClassName (ArcSDETestConfig::QClassNameOntarioRoads());
        select->SetFilter (FdoPtr<FdoFilter>(FdoFilter::Parse (L"bogus < 3")));
        bool failed = true;
        try
        {
            FdoPtr<FdoIFeatureReader> reader = select->Execute (); // should get exception here
            FdoPtr<FdoFeatureClass> classDef = (FdoFeatureClass*)reader->GetClassDefinition();
            failed = false;
            while (reader->ReadNext ())
                ProcessFeature (reader, classDef);
            reader->Close();
        }
        catch (FdoException *e)
        {
            e->Release ();
            failed = true;  // NOTE: it seems that ArcSDE 9 on Oracle doesn't fail on SE_stream_execute(), but fails later on SE_stream_fetch() instead.
        }
        CPPUNIT_ASSERT_MESSAGE ("did not fail at Execute() with invalid filter", failed);
        failed = true;
        try
        {
            FdoPtr<FdoIFeatureReader> reader = select->ExecuteWithLock (); // should get exception here
            FdoPtr<FdoFeatureClass> classDef = (FdoFeatureClass*)reader->GetClassDefinition();
            failed = false;
            while (reader->ReadNext ()) // not here
                ProcessFeature (reader, classDef);
            reader->Close();
        }
        catch (FdoException *e)
        {
            e->Release ();
            failed = true;  // NOTE: it seems that ArcSDE 9 on Oracle doesn't fail on SE_stream_execute(), but fails later on SE_stream_fetch() instead.
        }
        CPPUNIT_ASSERT_MESSAGE ("did not fail at ExecuteWithLock() with invalid filter", failed);
    }
    catch (FdoException *e)
    {
        fail(e);
    }
}

/* Test attribute filter with multiple clauses. */
void SelectTests::multiple_filter ()
{
    if (CreateSchemaOnly()) return;

    try
    {
        // Test a simple attribute select:
        FdoPtr<FdoISelect> select = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
        select->SetFeatureClassName (ArcSDETestConfig::QClassNameParcels());
        select->SetFilter (FdoPtr<FdoFilter>(FdoFilter::Parse (L"RTYPE='RES'")));
        FdoPtr<FdoIFeatureReader> reader = select->Execute ();
        long lCount = 0L;
        while (reader->ReadNext ())
        {
            FdoString* strLandUse = reader->GetString(L"RTYPE");
            CPPUNIT_ASSERT_MESSAGE("Wrong land use", 0==wcscmp(strLandUse,L"RES"));
            lCount++;
        }
        reader->Close();
        CPPUNIT_ASSERT_MESSAGE ("wrong number of features selected", lCount==13968);

        // NOTE: the 'FID' column is ambiguous because it exists on both the user table and the SHAPE table;
        // So this leads to errors, which are not easy to fix in the provider; instead, query RLDESCR4:
        //select->SetFilter (FdoPtr<FdoFilter>(FdoFilter::Parse (L"(RTYPE = 'UNK') AND (FID < 200)")));
        select->SetFilter (FdoPtr<FdoFilter>(FdoFilter::Parse (L"(RTYPE = 'EXM') AND (RLDESCR4 < '(FARN') AND (RLDESCR4 <> '#1177384 ROD')")));
        reader = select->Execute ();
        lCount = 0L;
        while (reader->ReadNext ())
        {
            FdoString* strLandUse = reader->GetString (L"RTYPE");
            CPPUNIT_ASSERT_MESSAGE("Wrong land use", 0==wcscmp(strLandUse,L"EXM"));
            FdoString *parcelID = reader->GetString(L"RLDESCR4");
            CPPUNIT_ASSERT_MESSAGE("Wrong RLDESCR4", 0>wcscmp(parcelID, L"(FARN"));
            lCount++;
        }
        reader->Close();
        CPPUNIT_ASSERT_MESSAGE ("wrong number of features selected", lCount==229);
    }
    catch (FdoException* ge) 
    {
        fail (ge);
    }
}

/* Test spatial filter. */
void SelectTests::spatial_filter ()
{
    if (CreateSchemaOnly()) return;

    try
    {
        // do a spatial filtered select
        FdoPtr<FdoISelect> select = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
        select->SetFeatureClassName (ArcSDETestConfig::QClassNameSoils());
        select->SetFilter (FdoPtr<FdoFilter>(FdoFilter::Parse (L"SHAPE INTERSECTS GEOMFROMTEXT ('POLYGON XY (( -87.76 43.78, -87.75 43.77, -87.75 43.77, -87.76 43.77, -87.76 43.78 ))')")));
        FdoPtr<FdoIFeatureReader> reader = select->Execute ();
        long lCount = 0L;
        while (reader->ReadNext ())
            lCount++;
        reader->Close ();
        CPPUNIT_ASSERT_MESSAGE ("wrong number of features returned from spatial query", 17 == lCount);
    }
    catch (FdoException* ge) 
    {
        fail (ge);
    }
}

/* Test spatial filter with buffer. */
void SelectTests::spatial_filter_buffer ()
{
    if (CreateSchemaOnly()) return;

    try
    {
        // set up basic filter data:

        double dMinX = -87.74;
        double dMinY = 43.75;
        double dMaxX = -87.76;
        double dMaxY = 43.76;
        double dBufferSize1 = 0.003;
        double dBufferSize2 = 0.01;
        double dBufferSize3 = 0.03;
        wchar_t filter[1000];


        // do a spatial filtered select WITHOUT buffer:

        FdoPtr<FdoISelect> select = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
        select->SetFeatureClassName (ArcSDETestConfig::QClassNameSoils());
        FdoCommonOSUtil::swprintf(filter, ELEMENTS(filter), L"SHAPE WITHIN GEOMFROMTEXT ('POLYGON XY ((%lf %lf, %lf %lf, %lf %lf, %lf %lf, %lf %lf))')",
            dMinX,dMaxY, dMaxX,dMaxY, dMaxX,dMinY, dMinX,dMinY, dMinX,dMaxY);
        select->SetFilter (FdoPtr<FdoFilter>(FdoFilter::Parse (filter)));
        FdoPtr<FdoIFeatureReader> reader = select->Execute ();
        long lCountNonBuffered = 0L;
        while (reader->ReadNext ())
        {
            lCountNonBuffered++;
        }
        reader->Close();

        // do a spatial filtered select WITH dBufferSize1:

        FdoCommonOSUtil::swprintf(filter, ELEMENTS(filter), L"SHAPE WITHINDISTANCE GEOMFROMTEXT ('POLYGON XY ((%lf %lf, %lf %lf, %lf %lf, %lf %lf, %lf %lf))') %lf",
            dMinX,dMaxY,
            dMaxX,dMaxY,
            dMaxX,dMinY,
            dMinX,dMinY,
            dMinX,dMaxY,
            dBufferSize1);

        select = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
        select->SetFeatureClassName (ArcSDETestConfig::QClassNameSoils());
        select->SetFilter (FdoPtr<FdoFilter>(FdoFilter::Parse (filter)));
        reader = select->Execute ();
        long lCountBuffered1 = 0L;
        while (reader->ReadNext ())
        {
            lCountBuffered1++;
        }
        reader->Close();


        // do a spatial filtered select WITH dBufferSize2:

        FdoCommonOSUtil::swprintf(filter, ELEMENTS(filter), L"SHAPE WITHINDISTANCE GEOMFROMTEXT ('POLYGON XY ((%lf %lf, %lf %lf, %lf %lf, %lf %lf, %lf %lf))') %lf",
            dMinX,dMaxY,
            dMaxX,dMaxY,
            dMaxX,dMinY,
            dMinX,dMinY,
            dMinX,dMaxY,
            dBufferSize2);

        select = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
        select->SetFeatureClassName (ArcSDETestConfig::QClassNameSoils());
        select->SetFilter (FdoPtr<FdoFilter>(FdoFilter::Parse (filter)));
        reader = select->Execute ();
        long lCountBuffered2 = 0L;
        while (reader->ReadNext ())
        {
            lCountBuffered2++;
        }
        reader->Close();


        // do a spatial filtered select WITH dBufferSize3:

        FdoCommonOSUtil::swprintf(filter, ELEMENTS(filter), L"SHAPE WITHINDISTANCE GEOMFROMTEXT ('POLYGON XY ((%lf %lf, %lf %lf, %lf %lf, %lf %lf, %lf %lf))') %lf",
            dMinX,dMaxY,
            dMaxX,dMaxY,
            dMaxX,dMinY,
            dMinX,dMinY,
            dMinX,dMaxY,
            dBufferSize3);

        select = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
        select->SetFeatureClassName (ArcSDETestConfig::QClassNameSoils());
        select->SetFilter (FdoPtr<FdoFilter>(FdoFilter::Parse (filter)));
        reader = select->Execute ();
        long lCountBuffered3 = 0L;
        while (reader->ReadNext ())
        {
            lCountBuffered3++;
        }
        reader->Close();


        // Compare all the results so far:

        CPPUNIT_ASSERT_MESSAGE ("wrong number of features returned from spatial query", lCountBuffered1>lCountNonBuffered);
        CPPUNIT_ASSERT_MESSAGE ("wrong number of features returned from spatial query", lCountBuffered2>lCountBuffered1);
        CPPUNIT_ASSERT_MESSAGE ("wrong number of features returned from spatial query", lCountBuffered3>lCountBuffered2);



        // do a spatial filtered select WITH dBufferSize3 using BEYOND operator:

        FdoCommonOSUtil::swprintf(filter, ELEMENTS(filter), L"SHAPE BEYOND GEOMFROMTEXT ('POLYGON XY ((%lf %lf, %lf %lf, %lf %lf, %lf %lf, %lf %lf))') %lf",
            dMinX,dMaxY,
            dMaxX,dMaxY,
            dMaxX,dMinY,
            dMinX,dMinY,
            dMinX,dMaxY,
            dBufferSize1);

        select = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
        select->SetFeatureClassName (ArcSDETestConfig::QClassNameSoils());
        select->SetFilter (FdoPtr<FdoFilter>(FdoFilter::Parse (filter)));
        reader = select->Execute ();
        long lCountBufferedBeyond1 = 0L;
        while (reader->ReadNext ())
        {
            lCountBufferedBeyond1++;
        }
        reader->Close();


        // select ALL objects in this class (no filter of any kind):

        select = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
        select->SetFeatureClassName (ArcSDETestConfig::QClassNameSoils());
        //Don't set a filter == select all
        reader = select->Execute ();
        long lCountAll = 0L;
        while (reader->ReadNext ())
        {
            if (!reader->IsNull(L"SHAPE"))  // Skip null features since they wont be returned by any spatial query of any kind
                lCountAll++;
        }
        reader->Close();

        CPPUNIT_ASSERT_MESSAGE ("spatial beyond + spatial withindistance != all features", lCountAll == (lCountBuffered1+lCountBufferedBeyond1));


        // Test a supported combination of 2 spatial filters:

        double dMinX2 = dMinX + 1.0*((dMaxX-dMinX) / 3.0);
        double dMaxX2 = dMinX + 2.0*((dMaxX-dMinX) / 3.0);
        double dMinY2 = dMinY + 1.0*((dMaxY-dMinY) / 3.0);
        double dMaxY2 = dMinY + 2.0*((dMaxY-dMinY) / 3.0);
        FdoCommonOSUtil::swprintf(filter, ELEMENTS(filter), L"(SHAPE WITHINDISTANCE GEOMFROMTEXT ('POLYGON XY ((%lf %lf, %lf %lf, %lf %lf, %lf %lf, %lf %lf))') %lf) AND (SHAPE BEYOND GEOMFROMTEXT ('POLYGON XY ((%lf %lf, %lf %lf, %lf %lf, %lf %lf, %lf %lf))') %lf)",
            dMinX,dMaxY,
            dMaxX,dMaxY,
            dMaxX,dMinY,
            dMinX,dMinY,
            dMinX,dMaxY,
            dBufferSize3,
            dMinX2,dMaxY2,
            dMaxX2,dMaxY2,
            dMaxX2,dMinY2,
            dMinX2,dMinY2,
            dMinX2,dMaxY2,
            0.1);

        select = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
        select->SetFeatureClassName (ArcSDETestConfig::QClassNameSoils());
        select->SetFilter (FdoPtr<FdoFilter>(FdoFilter::Parse (filter)));
        reader = select->Execute ();
        long lCountCombination1 = 0;
        while (reader->ReadNext ())
        {
            lCountCombination1++;
        }
        reader->Close();


        // test a supported combination of attribute query and spatial query:

        FdoCommonOSUtil::swprintf(filter, ELEMENTS(filter), L"(NAME LIKE 'U%%') AND (SHAPE WITHINDISTANCE GEOMFROMTEXT ('POLYGON XY ((%lf %lf, %lf %lf, %lf %lf, %lf %lf, %lf %lf))') %lf)",
            dMinX,dMaxY,
            dMaxX,dMaxY,
            dMaxX,dMinY,
            dMinX,dMinY,
            dMinX,dMaxY,
            dBufferSize1);

        select = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
        select->SetFeatureClassName (ArcSDETestConfig::QClassNameSoils());
        select->SetFilter (FdoPtr<FdoFilter>(FdoFilter::Parse (filter)));
        reader = select->Execute ();
        long lCountCombination2 = 0L;
        while (reader->ReadNext ())
        {
            lCountCombination2++;
        }
        reader->Close();
    }
    catch (FdoException* ge) 
    {
        fail (ge);
    }
}


/* Test spatial filter failure conditions. */
void SelectTests::spatial_filter_failures ()
{
    if (CreateSchemaOnly()) return;

    try
    {
        // set up basic filter data:

        double dMinX = 2294758.66;
        double dMinY = 672533.80;
        double dMaxX = 2363872.53;
        double dMaxY = 753392.50;
        double dBufferSize1 =   1000.0;
        double dBufferSize2 =  10000.0;
        double dBufferSize3 = 100000.0;
        wchar_t filter[1000];


        // Test spatial query against non-spatial column NAME (should fail):

        FdoCommonOSUtil::swprintf(filter, ELEMENTS(filter), L"NAME WITHINDISTANCE GEOMFROMTEXT ('POLYGON XY ((%lf %lf, %lf %lf, %lf %lf, %lf %lf, %lf %lf))') %lf",
            dMinX,dMaxY,
            dMaxX,dMaxY,
            dMaxX,dMinY,
            dMinX,dMinY,
            dMinX,dMaxY,
            dBufferSize3);

        FdoPtr<FdoIFeatureReader> reader;
        FdoPtr<FdoISelect> select = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
        select->SetFeatureClassName (ArcSDETestConfig::QClassNameSoils());
        select->SetFilter (FdoPtr<FdoFilter>(FdoFilter::Parse (filter)));
        try
        {
            reader = select->Execute ();
            while (reader->ReadNext ());
            reader->Close();
            CPPUNIT_FAIL("Spatial query against non spatial column should have failed, but succeeded instead.");
        }
        catch (FdoException *e)
        {
            e->Release();
        }



        // Test unsupported combination of spatial and non-spatial filters:

        FdoCommonOSUtil::swprintf(filter, ELEMENTS(filter), L"(NAME LIKE 'U%%') OR (SHAPE WITHINDISTANCE GEOMFROMTEXT ('POLYGON XY ((%lf %lf, %lf %lf, %lf %lf, %lf %lf, %lf %lf))') %lf)",
            dMinX,dMaxY,
            dMaxX,dMaxY,
            dMaxX,dMinY,
            dMinX,dMinY,
            dMinX,dMaxY,
            dBufferSize3);

        select = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
        select->SetFeatureClassName (ArcSDETestConfig::QClassNameSoils());
        select->SetFilter (FdoPtr<FdoFilter>(FdoFilter::Parse (filter)));
        try
        {
            reader = select->Execute ();
            while (reader->ReadNext ());
            reader->Close();
            CPPUNIT_FAIL("Spatial query against unsupported mix of spatial and non-spatial filters should have failed, but succeeded instead.");
        }
        catch (FdoException *e)
        {
            e->Release();
        }


        // Test another unsupported combination of spatial and non-spatial filters:

        FdoCommonOSUtil::swprintf(filter, ELEMENTS(filter), L"(SHAPE WITHINDISTANCE GEOMFROMTEXT ('POLYGON XY ((%lf %lf, %lf %lf, %lf %lf, %lf %lf, %lf %lf))') %lf) OR (SHAPE BEYOND GEOMFROMTEXT ('POLYGON XY ((%lf %lf, %lf %lf, %lf %lf, %lf %lf, %lf %lf))') %lf)",
            dMinX,dMaxY,
            dMaxX,dMaxY,
            dMaxX,dMinY,
            dMinX,dMinY,
            dMinX,dMaxY,
            dBufferSize3,
            dMinX,dMaxY,
            dMaxX,dMaxY,
            dMaxX,dMinY,
            dMinX,dMinY,
            dMinX,dMaxY,
            dBufferSize3);

        select = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
        select->SetFeatureClassName (ArcSDETestConfig::QClassNameSoils());
        select->SetFilter (FdoPtr<FdoFilter>(FdoFilter::Parse (filter)));
        try
        {
            reader = select->Execute ();
            while (reader->ReadNext ());
            reader->Close();
            CPPUNIT_FAIL("Spatial query against unsupported mix of spatial and non-spatial filters should have failed, but succeeded instead.");
        }
        catch (FdoException *e)
        {
            e->Release();
        }
    }
    catch (FdoException* ge) 
    {
        fail (ge);
    }
}


void SelectTests::simple_filter2()
{
    if (CreateSchemaOnly()) return;

    try
    {
        // Clean up leftovers from previous tests:
        FdoPtr<FdoIDelete> deleteCmd = (FdoIDelete*)mConnection->CreateCommand (FdoCommandType_Delete);
        deleteCmd->SetFeatureClassName(ArcSDETestConfig::QClassNameSoils());
        deleteCmd->SetFilter(L"NAME like '%\\_%' ESCAPE '\\'");
        deleteCmd->Execute();

        // Insert some test data (there is no native data that has names containing "[" special character):
        FdoPtr<FdoIInsert> insertCmd = (FdoIInsert*)mConnection->CreateCommand (FdoCommandType_Insert);
        insertCmd->SetFeatureClassName(ArcSDETestConfig::QClassNameSoils());
        FdoPtr<FdoPropertyValueCollection> propValues = insertCmd->GetPropertyValues();
        propValues->Clear();
        FdoPtr<FdoValueExpression> valueExpr = (FdoValueExpression*)FdoExpression::Parse(L"'Name_Test'");
        FdoPtr<FdoPropertyValue> propValue = FdoPropertyValue::Create(L"NAME", valueExpr);
        propValues->Add(propValue);
        propValue = FdoPropertyValue::Create();
	    FdoPtr<FdoFgfGeometryFactory> geomFactory = FdoFgfGeometryFactory::GetInstance();
        FdoPtr<FdoGeometryValue> geomValue = FdoGeometryValue::Create();
        geomValue->SetNullValue();
        propValue->SetName(L"SHAPE");
        propValue->SetValue(geomValue);
        propValues->Add(propValue);
        FdoPtr<FdoIFeatureReader> reader = insertCmd->Execute();
        bool bResult = reader->ReadNext();
        FdoInt32 id1 = reader->GetInt32(L"FID");
        reader->Close();

        // Test LIKE operator with escape character:
        FdoPtr<FdoISelect> select = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
        select->SetFeatureClassName (ArcSDETestConfig::QClassNameSoils());
        select->SetFilter (FdoPtr<FdoFilter>(FdoFilter::Parse (L"NAME like '%\\_%' ESCAPE '\\'")));
        reader = select->Execute ();
        long lCount=0;
        FdoPtr<FdoFeatureClass> classDef = (FdoFeatureClass*)reader->GetClassDefinition();
        while (reader->ReadNext ())
        {
            CPPUNIT_ASSERT_MESSAGE("Wrong id retrieved", id1 == reader->GetInt32(L"FID"));
            ProcessFeature (reader, classDef);
            lCount++;
        }
        reader->Close();
        CPPUNIT_ASSERT_MESSAGE("'LIKE' operator failed to get proper number of rows", lCount==1);


        // Clean up after test:
        deleteCmd = (FdoIDelete*)mConnection->CreateCommand (FdoCommandType_Delete);
        deleteCmd->SetFeatureClassName(ArcSDETestConfig::QClassNameSoils());
        deleteCmd->SetFilter(L"NAME like '%\\_%' ESCAPE '\\'");
        deleteCmd->Execute();



        // Test ARITHMETIC operators:
        select = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
        select->SetFeatureClassName (ArcSDETestConfig::QClassNameOntarioRoads());
        select->SetFilter (FdoPtr<FdoFilter>(FdoFilter::Parse (L"objectid*5+2-1<20 and objectid>1")));
        reader = select->Execute ();
        classDef = (FdoFeatureClass*)reader->GetClassDefinition();
        lCount=0;
        while (reader->ReadNext ())
        {
            id1 = reader->GetInt32(L"OBJECTID");
            CPPUNIT_ASSERT_MESSAGE("Wrong id retrieved", (id1==2) || (id1==3));
            ProcessFeature (reader, classDef);
            lCount++;
        }
        reader->Close();
        CPPUNIT_ASSERT_MESSAGE("ARITHMETIC operators failed to get proper number of rows", lCount==2);


        // Test IN operator:
        select = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
        select->SetFeatureClassName (ArcSDETestConfig::QClassNameParcels());
        select->SetFilter (FdoPtr<FdoFilter>(FdoFilter::Parse (L"RTYPE in ('AGR', 'RES')")));
        reader = select->Execute ();
        classDef = (FdoFeatureClass*)reader->GetClassDefinition();
        lCount=0;
        while (reader->ReadNext ())
        {
            ProcessFeature (reader, classDef);
            lCount++;
        }
        reader->Close();
        CPPUNIT_ASSERT_MESSAGE("'IN' operator failed to get proper number of rows", lCount==13977);
        

        // Clean up before NULL test:
        deleteCmd = (FdoIDelete*)mConnection->CreateCommand (FdoCommandType_Delete);
        deleteCmd->SetFeatureClassName(ArcSDETestConfig::QClassNameOntarioRoads());
        deleteCmd->SetFilter(L"FNODE_ NULL OR FCODE NULL");
        deleteCmd->Execute();


        // Insert some data containing NULL values:
        insertCmd = (FdoIInsert*)mConnection->CreateCommand (FdoCommandType_Insert);
        insertCmd->SetFeatureClassName(ArcSDETestConfig::QClassNameOntarioRoads());
        propValues = insertCmd->GetPropertyValues();
        propValues->Clear();
        FdoPtr<FdoPropertyValue> propValueFnode = FdoPropertyValue::Create(L"FNODE_", NULL);
        propValues->Add(propValueFnode);
        FdoPtr<FdoPropertyValue> propValueFcode = FdoPropertyValue::Create(L"FCODE", NULL);
        propValues->Add(propValueFcode);
        reader = insertCmd->Execute();
        bResult = reader->ReadNext();
        FdoInt32 idNullAbbrNullName = reader->GetInt32(L"OBJECTID");
        reader->Close();

        propValueFnode->SetValue(L"1234.1");
        propValueFcode->SetValue(L"NULL");
        reader = insertCmd->Execute();
        bResult = reader->ReadNext();
        FdoInt32 idNonNullAbbrNullName = reader->GetInt32(L"OBJECTID");
        reader->Close();

        propValueFnode->SetValue(L"NULL");
        propValueFcode->SetValue(L"'SomeVal'");
        reader = insertCmd->Execute();
        bResult = reader->ReadNext();
        FdoInt32 idNullAbbrNonNullName = reader->GetInt32(L"OBJECTID");
        reader->Close();

        // Test NULL operator:
        select = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
        select->SetFeatureClassName (ArcSDETestConfig::QClassNameOntarioRoads());
        select->SetFilter (FdoPtr<FdoFilter>(FdoFilter::Parse (L"FNODE_ null and FCODE null")));
        reader = select->Execute ();
        classDef = (FdoFeatureClass*)reader->GetClassDefinition();
        lCount=0;
        while (reader->ReadNext ())
        {
            id1 = reader->GetInt32(L"OBJECTID");
            CPPUNIT_ASSERT_MESSAGE("Wrong row retrieved from NULL test", id1==idNullAbbrNullName);
            ProcessFeature (reader, classDef);
            lCount++;
        }
        reader->Close();
        CPPUNIT_ASSERT_MESSAGE("'NULL' operator failed to get proper number of rows", lCount==1);

        // Test NULL operator (part 2):
        select = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
        select->SetFeatureClassName (ArcSDETestConfig::QClassNameOntarioRoads());
        select->SetFilter (FdoPtr<FdoFilter>(FdoFilter::Parse (L"FNODE_ null OR FCODE null")));
        reader = select->Execute ();
        classDef = (FdoFeatureClass*)reader->GetClassDefinition();
        lCount=0;
        while (reader->ReadNext ())
        {
            id1 = reader->GetInt32(L"OBJECTID");
            CPPUNIT_ASSERT_MESSAGE("Wrong row retrieved from NULL test", (id1==idNullAbbrNullName) || (id1==idNullAbbrNonNullName) || (id1==idNonNullAbbrNullName));
            ProcessFeature (reader, classDef);
            lCount++;
        }
        reader->Close();
        CPPUNIT_ASSERT_MESSAGE("'NULL' operator failed to get proper number of rows", lCount==3);

        // Clean up after NULL test:
        deleteCmd = (FdoIDelete*)mConnection->CreateCommand (FdoCommandType_Delete);
        deleteCmd->SetFeatureClassName(ArcSDETestConfig::QClassNameOntarioRoads());
        deleteCmd->SetFilter(L"FNODE_ null OR FCODE null");
        deleteCmd->Execute();


        // Test negation:
        select = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
        select->SetFeatureClassName (ArcSDETestConfig::QClassNameOntarioRoads());
        select->SetFilter (FdoPtr<FdoFilter>(FdoFilter::Parse (L"(-LENGTH + MNRCODE) >= -300.0")));
        reader = select->Execute ();
        classDef = (FdoFeatureClass*)reader->GetClassDefinition();
        lCount=0;
        while (reader->ReadNext ())
        {
            id1 = reader->GetInt32(L"OBJECTID");
            CPPUNIT_ASSERT_MESSAGE("Wrong row retrieved from NULL test", (id1==5) || (id1==6) || (id1==792));
            ProcessFeature (reader, classDef);
            lCount++;
        }
        reader->Close();
        CPPUNIT_ASSERT_MESSAGE("negation operator test failed to get proper number of rows", lCount==3);


        // Test negation:
        select = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
        select->SetFeatureClassName (ArcSDETestConfig::QClassNameParcels());
        select->SetFilter (FdoPtr<FdoFilter>(FdoFilter::Parse (L"RLDESCR4 > '2' and RLDESCR4 <= '3' and RLDESCR4 != '276_0010005'")));
        reader = select->Execute ();
        classDef = (FdoFeatureClass*)reader->GetClassDefinition();
        lCount=0;
        while (reader->ReadNext ())
        {
            ProcessFeature (reader, classDef);
            lCount++;
        }
        reader->Close();
        CPPUNIT_ASSERT_MESSAGE("relational operator test failed to get proper number of rows", lCount==134);
    }
    catch (FdoException* ge) 
    {
        fail (ge);
    }
}


void SelectTests::datetime_filter ()
{
    if (CreateSchemaOnly()) return;

    try
    {
        // Open a secondary connection, to access a table with a datetime column:
        FdoPtr<FdoIConnection> conn2 = ArcSDETests::GetConnection ();
        conn2->SetConnectionString(ArcSDETestConfig::ConnStringMetadcovSingleDb());
        conn2->Open();

        // Clean up old test results:
        FdoPtr<FdoIDelete> deleteCmd = (FdoIDelete*)conn2->CreateCommand (FdoCommandType_Delete);
        deleteCmd->SetFeatureClassName(ArcSDETestConfig::QClassNameTestClassComplex());
        deleteCmd->Execute();

        // Test data:
        FdoDateTime fdoDate1(1999, 12, (FdoInt8)25);
        FdoDateTime fdoDate2(2000, 02, (FdoInt8)01);
        FdoDateTime fdoTime1(14, 4, 20.50f);
        FdoDateTime fdoTime2(23, 1, 55.01f);
        FdoDateTime fdoDateTime1(2004, 1, 30, 5, 59, 59.0f);  //NOTE: SQL Server doesn't seem to store milliseconds, so don't rely on them in tests
        FdoDateTime fdoDateTime2(1976,12, 27, 3, 33, 22.4f);
        int idDate1 = 0;
        int idDate2 = 0;
        int idTime1 = 0;
        int idTime2 = 0;
        int idDateTime1 = 0;
        int idDateTime2 = 0;
        bool bFoundDate1;
        bool bFoundDate2;
        bool bFoundTime1;
        bool bFoundTime2;
        bool bFoundDateTime1;
        bool bFoundDateTime2;

        // Insert data with dates
        FdoPtr<FdoIInsert> insertCmd = (FdoIInsert*)conn2->CreateCommand(FdoCommandType_Insert);
        insertCmd->SetFeatureClassName(ArcSDETestConfig::QClassNameTestClassComplex());
        FdoPtr<FdoPropertyValueCollection> propVals = insertCmd->GetPropertyValues();
        FdoPtr<FdoPropertyValue> propVal = FdoPropertyValue::Create();
        FdoPtr<FdoDateTimeValue> fdoDate;

        fdoDate = FdoDateTimeValue::Create(fdoDate1);
        propVals->Clear();
        propVal->SetName(L"MYDATE");
        propVal->SetValue(fdoDate);
        propVals->Add(propVal);
        FdoPtr<FdoIFeatureReader> reader = insertCmd->Execute();
        reader->ReadNext();
        idDate1 = reader->GetInt32(L"OBJECTID");
        reader->Close();

        fdoDate = FdoDateTimeValue::Create(fdoDate2);
        propVals->Clear();
        propVal->SetName(L"MYDATE");
        propVal->SetValue(fdoDate);
        propVals->Add(propVal);
        reader = insertCmd->Execute();
        reader->ReadNext();
        idDate2 = reader->GetInt32(L"OBJECTID");
        reader->Close();

        fdoDate = FdoDateTimeValue::Create(fdoTime1);
        propVals->Clear();
        propVal->SetName(L"MYDATE");
        propVal->SetValue(fdoDate);
        propVals->Add(propVal);
        reader = insertCmd->Execute();
        reader->ReadNext();
        idTime1 = reader->GetInt32(L"OBJECTID");
        reader->Close();

        fdoDate = FdoDateTimeValue::Create(fdoTime2);
        propVals->Clear();
        propVal->SetName(L"MYDATE");
        propVal->SetValue(fdoDate);
        propVals->Add(propVal);
        reader = insertCmd->Execute();
        reader->ReadNext();
        idTime2 = reader->GetInt32(L"OBJECTID");
        reader->Close();

        fdoDate = FdoDateTimeValue::Create(fdoDateTime1);
        propVals->Clear();
        propVal->SetName(L"MYDATE");
        propVal->SetValue(fdoDate);
        propVals->Add(propVal);
        reader = insertCmd->Execute();
        reader->ReadNext();
        idDateTime1 = reader->GetInt32(L"OBJECTID");
        reader->Close();

        fdoDate = FdoDateTimeValue::Create(fdoDateTime2);
        propVals->Clear();
        propVal->SetName(L"MYDATE");
        propVal->SetValue(fdoDate);
        propVals->Add(propVal);
        reader = insertCmd->Execute();
        reader->ReadNext();
        idDateTime2 = reader->GetInt32(L"OBJECTID");
        reader->Close();


        // Query using date in filter:
        FdoPtr<FdoISelect> selectCmd = (FdoISelect*)conn2->CreateCommand(FdoCommandType_Select);
        selectCmd->SetFeatureClassName(ArcSDETestConfig::QClassNameTestClassComplex());
        selectCmd->SetFilter(L"MYDATE < DATE '2000-01-01'");
        reader = selectCmd->Execute();
        bFoundDate1 = false;  bFoundDate2 = false;
        bFoundTime1 = false;  bFoundTime2 = false;
        bFoundDateTime1 = false;  bFoundDateTime2 = false;
        while (reader->ReadNext())
        {
            int id = reader->GetInt32(L"OBJECTID");
            if (id==idDate1) bFoundDate1 = true;
            if (id==idDate2) bFoundDate2 = true;
        }
        reader->Close();
        CPPUNIT_ASSERT_MESSAGE("DateTime filter is missing expected results", bFoundDate1);
        CPPUNIT_ASSERT_MESSAGE("DateTime filter got expected results", !bFoundDate2);


        /*  // NOTE: querying by time fails on Oracle, because we are forced by ArcSDE to attach a default date to
            //       'dateless' times.  This is due to ArcSDE relying on struct tm which cannot indicate
            //       the *absence* of a date (or time).

        // Query using time in filter:
        selectCmd = (FdoISelect*)conn2->CreateCommand(FdoCommandType_Select);
        selectCmd->SetFeatureClassName(ArcSDETestConfig::QClassNameTestClassComplex());
        selectCmd->SetFilter(L"MYDATE >= TIME '23:01:55'");
        reader = selectCmd->Execute();
        bFoundDate1 = false;  bFoundDate2 = false;
        bFoundTime1 = false;  bFoundTime2 = false;
        bFoundDateTime1 = false;  bFoundDateTime2 = false;
        while (reader->ReadNext())
        {
            int id = reader->GetInt32(L"OBJECTID");
            if (id==idTime1) bFoundTime1 = true;
            if (id==idTime2) bFoundTime2 = true;
        }
        reader->Close();
        CPPUNIT_ASSERT_MESSAGE("DateTime filter is missing expected results", bFoundTime2);
        CPPUNIT_ASSERT_MESSAGE("DateTime filter got expected results", !bFoundTime1);
        */

        // Query using date+time in filter:
        selectCmd = (FdoISelect*)conn2->CreateCommand(FdoCommandType_Select);
        selectCmd->SetFeatureClassName(ArcSDETestConfig::QClassNameTestClassComplex());
        selectCmd->SetFilter(L"MYDATE <> TIMESTAMP '2004-01-30 5:59:59'");
        reader = selectCmd->Execute();
        bFoundDate1 = false;  bFoundDate2 = false;
        bFoundTime1 = false;  bFoundTime2 = false;
        bFoundDateTime1 = false;  bFoundDateTime2 = false;
        while (reader->ReadNext())
        {
            int id = reader->GetInt32(L"OBJECTID");
            if (id==idDateTime1) bFoundDateTime1 = true;
            if (id==idDateTime2) bFoundDateTime2 = true;
        }
        reader->Close();
        CPPUNIT_ASSERT_MESSAGE("DateTime filter is missing expected results", bFoundDateTime2);
        CPPUNIT_ASSERT_MESSAGE("DateTime filter got expected results", !bFoundDateTime1);


        // Clean up entries:
        deleteCmd = (FdoIDelete*)conn2->CreateCommand (FdoCommandType_Delete);
        deleteCmd->SetFeatureClassName(ArcSDETestConfig::QClassNameTestClassComplex());
        deleteCmd->Execute();


        // Close the secondary connection:
        conn2->Close();
    }
    catch (FdoException* ge) 
    {
        fail (ge);
    }
}

void SelectTests::get_null ()
{
    if (CreateSchemaOnly()) return;

    try
    {
        // clean up previous tests
        CleanUpClass(mConnection, ArcSDETestConfig::ClassSchemaLumpy(), nullableClass->mName, true);

        FdoPtr <FdoITransaction> transaction;
        int item1[2];
        int item2[2];

        // insert a couple of objects
        transaction = mConnection->BeginTransaction ();
        item1[0] = 0;
        item1[1] = ArcSDETests::Insert (mConnection, nullableClass, item1[0]);
        item2[0] = 1;
        item2[1] = ArcSDETests::Insert (mConnection, nullableClass, item1[1]);
        transaction->Commit ();

        std::vector<int*> map;
        map.push_back (item1);
        map.push_back (item2);
        ArcSDETests::Expect (mConnection, nullableClass, &map);
    }
    catch (FdoException *e)
    {
        fail(e);
    }
}


void SelectTests::spatial_filter_outofbounds()
{
    if (CreateSchemaOnly()) return;

    try
    {
        // do a spatial filtered select
        FdoPtr<FdoISelect> select = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
        select->SetFeatureClassName (ArcSDETestConfig::QClassNameSoils());
        select->SetFilter (FdoPtr<FdoFilter>(FdoFilter::Parse (L"SHAPE INTERSECTS GEOMFROMTEXT ('POLYGON XY (( -97.76 43.78, -87.75 43.77, -87.75 43.77, -97.76 43.77, -97.76 43.78 ))')")));
        FdoPtr<FdoIFeatureReader> reader = select->Execute ();
        long lCount = 0L;
        while (reader->ReadNext ())
            lCount++;
        reader->Close ();
        CPPUNIT_ASSERT_MESSAGE ("wrong number of features returned from spatial query", 25 == lCount);
    }
    catch (FdoException* ge) 
    {
        fail (ge);
    }
}


struct TestFeature
{
public:
    long   id;
    double dMinX;
    double dMinY;
    double dMaxX;
    double dMaxY;
    bool   bFound;
};

void SelectTests::spatial_filter_within_inside_coveredby()
{
    if (CreateSchemaOnly()) return;

    double dGlobalMaxX = 0.0;
    double dGlobalMaxY = 0.0;
    double dGlobalMinX = 0.0;
    double dGlobalMinY = 0.0;
    long lWithinResults = 0;
    long lInsideResults = 0;
    long lCoveredByResults = 0;

    try
    {
        FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();


        // DYNAMICALLY DISCOVER TEST DATA:
        //////////////////////////////////////////////////////////////

        FdoPtr<FdoISelect> select = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
        select->SetFeatureClassName (ArcSDETestConfig::QClassNameSoils());
        FdoPtr<FdoIFeatureReader> reader = select->Execute();

        int counter=0;
        const int testDataAmount = 10;
        TestFeature testFeatures[testDataAmount];
        while (reader->ReadNext() && (counter<testDataAmount))
        {
            if (!reader->IsNull(L"SHAPE"))
            {
                FdoPtr<FdoByteArray> bytes = reader->GetGeometry(L"SHAPE");
                FdoPtr<FdoIGeometry> geom = gf->CreateGeometryFromFgf(bytes);
                FdoPtr<FdoIEnvelope> envelope = geom->GetEnvelope();
                testFeatures[counter].dMaxX = envelope->GetMaxX();
                testFeatures[counter].dMaxY = envelope->GetMaxY();
                testFeatures[counter].dMinX = envelope->GetMinX();
                testFeatures[counter].dMinY = envelope->GetMinY();
                testFeatures[counter].id = reader->GetInt32(L"FID");

                if ((counter==0) || (testFeatures[counter].dMaxX > dGlobalMaxX))
                    dGlobalMaxX = testFeatures[counter].dMaxX;
                if ((counter==0) || (testFeatures[counter].dMaxY > dGlobalMaxY))
                    dGlobalMaxY = testFeatures[counter].dMaxY;
                if ((counter==0) || (testFeatures[counter].dMinX < dGlobalMinX))
                    dGlobalMinX = testFeatures[counter].dMinX;
                if ((counter==0) || (testFeatures[counter].dMinY < dGlobalMinY))
                    dGlobalMinY = testFeatures[counter].dMinY;

                //printf("\nFID=%d;  MinX=%.20f, MinY=%.20f, MaxX=%.20f, MaxY=%.20f", testFeatures[counter].id, testFeatures[counter].dMinX, testFeatures[counter].dMinY, testFeatures[counter].dMaxX, testFeatures[counter].dMaxY);

                counter++;
            }
        }

        //printf("\nGLOBAL MINX=%.20f, MINY=%.20f, MAXX=%.20f, MAXY=%.20f", dGlobalMinX, dGlobalMinY, dGlobalMaxX, dGlobalMaxY);


        // Perform spatial query
        //////////////////////////////////////////////////////////////

        for (int scaleAttempt=0; scaleAttempt<5; scaleAttempt++)
        {
            for (int queryAttempt=0; queryAttempt<3; queryAttempt++)
            {
                // Determine the scale factor to use;
                // IMPORTANT: this should always be greater than 0.5 otherwise the query window
                // will be 'inverted' (i.e. minx>maxx && miny>maxy):
                double dScaleFactor = 0.0;
                switch (scaleAttempt)
                {
                case 0:  dScaleFactor = 1.01;  break;
                case 1:  dScaleFactor = 1.00;  break;
                case 2:  dScaleFactor = 0.99;  break;
                case 3:  dScaleFactor = 0.70;  break;
                case 4:  dScaleFactor = 0.55;  break;
                }

                // determine the spatial query type to use:
                wchar_t *sQueryType = NULL;
                switch (queryAttempt)
                {
                case 0:  sQueryType = L"WITHIN";     break;
                case 1:  sQueryType = L"INSIDE";     break;
                case 2:  sQueryType = L"COVEREDBY";  break;
                }

                // Determine the spatial query polygon dimensions, based on dScaleFactor:
                double dQueryMinX = dGlobalMinX - ((dScaleFactor-1.0) * (dGlobalMaxX-dGlobalMinX));
                double dQueryMaxX = dGlobalMaxX + ((dScaleFactor-1.0) * (dGlobalMaxX-dGlobalMinX));
                double dQueryMinY = dGlobalMinY - ((dScaleFactor-1.0) * (dGlobalMaxY-dGlobalMinY));
                double dQueryMaxY = dGlobalMaxY + ((dScaleFactor-1.0) * (dGlobalMaxY-dGlobalMinY));

                // Perform the spatial query:
                select = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
                select->SetFeatureClassName (ArcSDETestConfig::QClassNameSoils());
                wchar_t filter[200];
                FdoCommonOSUtil::swprintf(filter, ELEMENTS(filter), L"SHAPE %ls GEOMFROMTEXT('POLYGON XY ((%f %f, %f %f, %f %f, %f %f, %f %f))')",
                    sQueryType,
                    dQueryMinX, dQueryMinY,
                    dQueryMaxX, dQueryMinY,
                    dQueryMaxX, dQueryMaxY,
                    dQueryMinX, dQueryMaxY,
                    dQueryMinX, dQueryMinY);
                select->SetFilter(filter);
                reader = select->Execute();


                // Iterate the results:
                long lFoundCount = 0;
                for (int i=0; i<testDataAmount; i++)
                    testFeatures[i].bFound = false;
                while (reader->ReadNext())
                {
                    int idFound = reader->GetInt32(L"FID");
                    for (int i=0; i<testDataAmount; i++)
                    {
                        TestFeature &tf = testFeatures[i];
                        if (idFound == tf.id)
                        {
                            lFoundCount++;
                            testFeatures[i].bFound = true;
                            //printf("\n  FOUND FID=%d", idFound);
                            break;
                        }
                    }
                }
                printf("\n  Spatial query '%ls' on area (%.20f, %.20f)-(%.20f,%.20f) returned %d results", sQueryType, dQueryMinX, dQueryMinY, dQueryMaxX, dQueryMaxY, lFoundCount);

                // Verify the proper features were found (or not found):
                for (int i=0; i<testDataAmount; i++)
                {
                    TestFeature &tf = testFeatures[i];

                    if (0==FdoCommonOSUtil::wcsicmp(sQueryType,L"WITHIN"))
                    {
                        bool bIsWithin = (tf.dMinX>=dQueryMinX) && (tf.dMinY>=dQueryMinY)
                                      && (tf.dMaxX<=dQueryMaxX) && (tf.dMaxY<=dQueryMaxY);

                        CPPUNIT_ASSERT_MESSAGE("Spatial query WITHIN returned wrong results",
                            (bIsWithin && tf.bFound) || (!bIsWithin && !tf.bFound));
                    }
                    else if (0==FdoCommonOSUtil::wcsicmp(sQueryType,L"INSIDE"))
                    {
                        bool bIsInside = (tf.dMinX>dQueryMinX) && (tf.dMinY>dQueryMinY)
                                      && (tf.dMaxX<dQueryMaxX) && (tf.dMaxY<dQueryMaxY);

                        CPPUNIT_ASSERT_MESSAGE("Spatial query INSIDE returned wrong results",
                            (bIsInside && tf.bFound) || (!bIsInside && !tf.bFound));
                    }
                    else if (0==FdoCommonOSUtil::wcsicmp(sQueryType,L"COVEREDBY"))
                    {
                        bool bIsCoveredby = (tf.dMinX>=dQueryMinX) && (tf.dMinY>=dQueryMinY)
                                         && (tf.dMaxX<=dQueryMaxX) && (tf.dMaxY<=dQueryMaxY)
                                         && (    (tf.dMinX==dQueryMinX) || (tf.dMinY==dQueryMinY)
                                              || (tf.dMaxX==dQueryMaxX) || (tf.dMaxY==dQueryMaxY));

                        CPPUNIT_ASSERT_MESSAGE("Spatial query COVEREDBY returned wrong results",
                             (bIsCoveredby && tf.bFound) || (!bIsCoveredby && !tf.bFound));
                    }
                }

                // remember the results, so we can do a sanity check later:
                if (0==FdoCommonOSUtil::wcsicmp(sQueryType,L"WITHIN"))
                    lWithinResults = lFoundCount;
                else if (0==FdoCommonOSUtil::wcsicmp(sQueryType,L"INSIDE"))
                    lInsideResults = lFoundCount;
                else if (0==FdoCommonOSUtil::wcsicmp(sQueryType,L"COVEREDBY"))
                    lCoveredByResults = lFoundCount;
            }

            // Sanity check (within results = inside results + coveredby results):
            CPPUNIT_ASSERT_MESSAGE("Within results != Inside results + CoveredBy results", (lWithinResults==(lInsideResults+lCoveredByResults)));
        }
    }
    catch (FdoException* ge) 
    {
        fail (ge);
    }
}


void SelectTests::spatial_query_outside_coordsys_extents()
{
    if (CreateSchemaOnly()) return;

    try
    {
        FdoPtr<FdoIFeatureReader> reader;
        FdoPtr<FdoPropertyValueCollection> propValues;
        FdoPtr<FdoPropertyValue> propValue;

        // Cleanup data from previous tests:
        CleanUpClass(mConnection, ArcSDETestConfig::ClassSchemaDefect64335202(), ArcSDETestConfig::ClassNameDefect64335202(), true);


        // Insert some test data:
        FdoPtr<FdoIInsert> insert;
        insert = (FdoIInsert*)mConnection->CreateCommand (FdoCommandType_Insert);
        insert->SetFeatureClassName(ArcSDETestConfig::QClassNameDefect64335202());
        propValues = insert->GetPropertyValues();
        propValues->Clear();
        propValue = FdoPropertyValue::Create(L"SHAPE", FdoPtr<FdoValueExpression>((FdoValueExpression*)FdoExpression::Parse(L"GEOMFROMTEXT('POLYGON XY (( 50.0 50.0, 100.0 50.0, 100.0 100.0, 50.0 100.0, 50.0 50.0 ))')")));
        propValues->Add(propValue);
        reader = insert->Execute();
        FdoInt32 id1;
        while (reader->ReadNext())
        {
            id1 = reader->GetInt32(L"OBJECTID");
        }

        // Do spatial query *entirely* (not just partially) outside the spatial context extents:
        FdoPtr<FdoISelect> select = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
        select->SetFeatureClassName (ArcSDETestConfig::QClassNameDefect64335202());
        select->SetFilter(L"SHAPE INTERSECTS GEOMFROMTEXT ('POLYGON XY (( -150.0 -150.0, -150.0 -500.0, -500.0 -500.0, -500.0 -150.0, -150.0 -150.0 ))')");
        reader = select->Execute();
        FdoInt32 fetchedId = -1;
        FdoInt32 lCount = 0;
        while (reader->ReadNext())
        {
            fetchedId = reader->GetInt32(L"OBJECTID");
            lCount++;
        }
        CPPUNIT_ASSERT_MESSAGE("Wrong rowcount from spatial query outside coordsys extents", lCount==0);
    }
    catch (FdoException* ge) 
    {
        fail (ge);
    }
}


void SelectTests::multiple_databases()
{
#define INSERTVAL1 L"Value One"
#define INSERTVAL2 L"Value Two"
#define UPDATEVAL1 L"Value One Updated"
#define UPDATEVAL2 L"Value Two Updated"

    if (CreateSchemaOnly()) return;
    if (!ArcSDETestConfig::SupportsNamedDatabases()) return;

    try
    {
        // Run this test two ways: first, connect to "sde" database, then connect to "testmultidb" database:
        for (int i=0; i<2; i++)
        {
            mConnection->Close();
	        mConnection = ArcSDETests::GetConnection ();
            if (i==0)
                mConnection->SetConnectionString (ArcSDETestConfig::ConnStringAustralia());
            else
                mConnection->SetConnectionString (ArcSDETestConfig::ConnStringAustraliaMultiDb());
            mConnection->Open ();


            FdoPtr<FdoIInsert> insert1 = (FdoIInsert*)mConnection->CreateCommand(FdoCommandType_Insert);
            FdoPtr<FdoIInsert> insert2 = (FdoIInsert*)mConnection->CreateCommand(FdoCommandType_Insert);
            FdoPtr<FdoISelect> select1 = (FdoISelect*)mConnection->CreateCommand(FdoCommandType_Select);
            FdoPtr<FdoISelect> select2 = (FdoISelect*)mConnection->CreateCommand(FdoCommandType_Select);
            FdoPtr<FdoIUpdate> update1 = (FdoIUpdate*)mConnection->CreateCommand(FdoCommandType_Update);
            FdoPtr<FdoIUpdate> update2 = (FdoIUpdate*)mConnection->CreateCommand(FdoCommandType_Update);
            FdoPtr<FdoPropertyValueCollection> propValues;
            FdoPtr<FdoIFeatureReader> reader;
            FdoStringP testclasssimple1 = ArcSDETestConfig::QClassNameTestClassSimple();
            FdoStringP testclasssimple2 = ArcSDETestConfig::QClassNameTestClassSimpleMultiDb();
            FdoString* retVal1;
            FdoString* retVal2;

            CleanUpClass(mConnection, ArcSDETestConfig::ClassSchemaTestClassSimple(), ArcSDETestConfig::ClassNameTestClassSimple(), true);
            CleanUpClass(mConnection, ArcSDETestConfig::ClassSchemaTestClassSimpleMultiDb(), ArcSDETestConfig::ClassNameTestClassSimple(), true);

            insert1->SetFeatureClassName(testclasssimple1);
            propValues = insert1->GetPropertyValues();
            propValues->Clear();
            propValues->Add(FdoPtr<FdoPropertyValue>(FdoPropertyValue::Create(L"MYSTRING", FdoPtr<FdoValueExpression>((FdoValueExpression*)FdoValueExpression::Parse(L"'" INSERTVAL1 L"'")))));
            reader = insert1->Execute();
            reader->ReadNext();
            FdoInt32 id1 = reader->GetInt32(L"OBJECTID");
            reader->Close();

            insert2->SetFeatureClassName(testclasssimple2);
            propValues = insert2->GetPropertyValues();
            propValues->Clear();
            propValues->Add(FdoPtr<FdoPropertyValue>(FdoPropertyValue::Create(L"MYSTRING", FdoPtr<FdoValueExpression>((FdoValueExpression*)FdoValueExpression::Parse(L"'" INSERTVAL2 L"'")))));
            reader = insert2->Execute();
            reader->ReadNext();
            FdoInt32 id2 = reader->GetInt32(L"OBJECTID");
            reader->Close();

            select1->SetFeatureClassName(testclasssimple1);
            reader = select1->Execute();
            reader->ReadNext();
            retVal1 = reader->GetString(L"MYSTRING");
            CPPUNIT_ASSERT_MESSAGE("Wrong string value fetched", 0==wcscmp(retVal1, INSERTVAL1));

            select2->SetFeatureClassName(testclasssimple2);
            reader = select2->Execute();
            reader->ReadNext();
            retVal2 = reader->GetString(L"MYSTRING");
            CPPUNIT_ASSERT_MESSAGE("Wrong string value fetched", 0==wcscmp(retVal2, INSERTVAL2));

            update1->SetFeatureClassName(testclasssimple1);
            propValues = update1->GetPropertyValues();
            propValues->Clear();
            propValues->Add(FdoPtr<FdoPropertyValue>(FdoPropertyValue::Create(L"MYSTRING", FdoPtr<FdoValueExpression>((FdoValueExpression*)FdoValueExpression::Parse(L"'" UPDATEVAL1 L"'")))));
            update1->Execute();

            update2->SetFeatureClassName(testclasssimple2);
            propValues = update2->GetPropertyValues();
            propValues->Clear();
            propValues->Add(FdoPtr<FdoPropertyValue>(FdoPropertyValue::Create(L"MYSTRING", FdoPtr<FdoValueExpression>((FdoValueExpression*)FdoValueExpression::Parse(L"'" UPDATEVAL2 L"'")))));
            update2->Execute();

            select1->SetFeatureClassName(testclasssimple1);
            reader = select1->Execute();
            reader->ReadNext();
            retVal1 = reader->GetString(L"MYSTRING");
            CPPUNIT_ASSERT_MESSAGE("Wrong string value fetched", 0==wcscmp(retVal1, UPDATEVAL1));

            select2->SetFeatureClassName(testclasssimple2);
            reader = select2->Execute();
            reader->ReadNext();
            retVal2 = reader->GetString(L"MYSTRING");
            CPPUNIT_ASSERT_MESSAGE("Wrong string value fetched", 0==wcscmp(retVal2, UPDATEVAL2));
        }
    }
    catch (FdoException* ge) 
    {
        fail (ge);
    }
}


/* Test spatial filter with buffer. */
void SelectTests::spatial_filter_buffer_outside_extents ()
{
    if (CreateSchemaOnly()) return;

    try
    {
        FdoPtr<FdoISelect> select = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
        wchar_t filter[1000];
        double dMinX = 120000000.00;
        double dMinY = 12000000.00;
        double dMaxX = 130000000.00;
        double dMaxY = 13000000.00;

        // do a spatial filtered select WITH dBufferSize1:

        FdoCommonOSUtil::swprintf(filter, ELEMENTS(filter), L"SHAPE WITHINDISTANCE GEOMFROMTEXT ('POLYGON XY ((%lf %lf, %lf %lf, %lf %lf, %lf %lf, %lf %lf))') %lf",
            dMinX,dMaxY,
            dMaxX,dMaxY,
            dMaxX,dMinY,
            dMinX,dMinY,
            dMinX,dMaxY,
            1000.0);

        select->SetFeatureClassName (ArcSDETestConfig::QClassNameSoils());
        select->SetFilter (FdoPtr<FdoFilter>(FdoFilter::Parse (filter)));
        FdoPtr<FdoIFeatureReader> reader = select->Execute ();
        long lCountBuffered = 0L;
        while (reader->ReadNext ())
            lCountBuffered++;
        reader->Close();
        CPPUNIT_ASSERT_MESSAGE("Expected 0 features for buffered spatial query beyond the spatial extents", lCountBuffered==0);
    }
    catch (FdoException* ge) 
    {
        fail (ge);
    }
}



void SelectTests::select_large_table_with_geom()
{
    if (CreateSchemaOnly()) return;

    try
    {
        double secs_start = ((double)(long)clock()) / 1000.0;
        printf("\nSelect and read all properties begin: clock=%f\n", secs_start);

        FdoPtr<FdoISelect> select = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
        select->SetFeatureClassName (ArcSDETestConfig::QClassNameLargeWithGeom());
        FdoPtr<FdoIFeatureReader> reader = select->Execute ();
        FdoPtr<FdoFeatureClass> classDef = (FdoFeatureClass*)reader->GetClassDefinition();
        long lCount=0;
        while (reader->ReadNext ())
        {
			lCount++;
            ProcessFeature (reader, classDef, NULL, false);

            if (lCount % 10000 == 9999)
                printf("\nSelect and read all properties progress features=%d: clock=%f\n", lCount, ((double)(long)clock()) / 1000.0);
        }
        reader->Close();
        double secs_end = ((double)(long)clock()) / 1000.0;
		printf("\nFeatures Read=%d\n", lCount);
        printf("\nSelect and read all properties end: clock=%f, elapsed time=%f, features per sec=%f\n", secs_end, secs_end-secs_start, (double)lCount/(secs_end-secs_start));


        secs_start = ((double)(long)clock()) / 1000.0;
        printf("\nSelect but dont read all properties begin: clock=%f\n", secs_start);
        select = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
        select->SetFeatureClassName (ArcSDETestConfig::QClassNameLargeWithGeom());
        reader = select->Execute ();
        lCount=0;
        while (reader->ReadNext ())
        {
			lCount++;

            if (lCount % 10000 == 9999)
                printf("\nSelect and read all properties progress features=%d: clock=%f\n", lCount, ((double)(long)clock()) / 1000.0);
        }
        reader->Close();
        secs_end = ((double)(long)clock()) / 1000.0;
		printf("\nFeatures Read=%d\n", lCount);
        printf("\nSelect but dont read all properties end: clock=%f, elapsed time=%f, features per sec=%f\n", secs_end, secs_end-secs_start, (double)lCount/(secs_end-secs_start));
    }
    catch (FdoException* ge) 
    {
        fail (ge);
    }
}



void SelectTests::create_large_table_with_geom()
{
    const int num_records = 103100;

    try
    {
        // Clean up previous tests:
        CleanUpClass(mConnection, ArcSDETestConfig::ClassSchemaLargeWithGeom(), ArcSDETestConfig::ClassNameLargeWithGeom());

        FdoPtr<FdoIApplySchema> apply = (FdoIApplySchema*)mConnection->CreateCommand (FdoCommandType_ApplySchema);
        FdoPtr<FdoFeatureSchema> schema = ArcSDETests::GetDefaultSchema(mConnection);
        FdoPtr<FdoClassCollection> classes = schema->GetClasses ();

        // build the data properties
        // assemble the class
        FdoPtr<FdoFeatureClass> cls = FdoFeatureClass::Create (ArcSDETestConfig::ClassNameLargeWithGeom(), L"");
        FdoPtr<FdoPropertyDefinitionCollection> properties = cls->GetProperties ();
        FdoPtr<FdoDataPropertyDefinitionCollection> idProperties = cls->GetIdentityProperties ();
        FdoPtr<FdoDataPropertyDefinition> property;
        // Add read-only property:
        property = FdoDataPropertyDefinition::Create (L"ID", L"");
        property->SetDataType (FdoDataType_Int32);
        property->SetNullable (false);
        property->SetIsAutoGenerated (true);
        property->SetReadOnly (true);
        properties->Add (property);
        idProperties->Add(property);
        // Add geometry property:
        FdoPtr<FdoGeometricPropertyDefinition> geomProp = FdoGeometricPropertyDefinition::Create(L"GEOMETRY", L"");
        geomProp->SetGeometryTypes(FdoGeometricType_Curve);
        geomProp->SetHasElevation(false);
        geomProp->SetHasMeasure(false);
        properties->Add (geomProp);
        cls->SetGeometryProperty(geomProp);
        // Add two read-write properties with default values:
        property = FdoDataPropertyDefinition::Create (L"NAME", L"");
        property->SetDataType (FdoDataType_String);
        property->SetNullable (false);
        property->SetReadOnly (false);
        properties->Add (property);
        property = FdoDataPropertyDefinition::Create (L"AREA", L"");
        property->SetDataType (FdoDataType_Double);
        property->SetNullable (false);
        property->SetReadOnly (false);
        properties->Add (property);

        // submit the new schema
        classes->Add (cls);
        apply->SetFeatureSchema (schema);
        apply->Execute ();


        //////////////////////////////////////////////////////////////////////
        // Add some random data:
        //////////////////////////////////////////////////////////////////////

        printf("\nInsert data begin: clock=%f\n", ((double)(long)clock()) / 1000.0);

        FdoPtr<FdoIInsert> insert = (FdoIInsert*)mConnection->CreateCommand (FdoCommandType_Insert);
        insert->SetFeatureClassName (ArcSDETestConfig::QClassNameLargeWithGeom());
        FdoPtr<FdoPropertyValueCollection> values = insert->GetPropertyValues ();
        FdoPtr<FdoValueExpression> expression;
        FdoPtr<FdoPropertyValue> value;
        FdoPtr<FdoIFeatureReader> reader;
        int string_size;
        srand ((unsigned)time (NULL));  // randommize
        wchar_t string_value[100];
        double double_value;
        FdoPtr<FdoStringValue> strVal;
        FdoPtr<FdoDoubleValue> dblVal;
        FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();


        // Set up the property value collection:

        FdoPtr<FdoPropertyValue> propertyValue;
        FdoPtr<FdoParameter>  parameter;

        values->Clear(); // NOTE: this should probably not be necessary!

        parameter = FdoParameter::Create(L"Param0");
        propertyValue = FdoPropertyValue::Create(L"NAME", parameter);
        values->Add(propertyValue);

        parameter = FdoParameter::Create(L"Param1");
        propertyValue = FdoPropertyValue::Create(L"AREA", parameter);
        values->Add(propertyValue);

        parameter = FdoParameter::Create(L"ParamGeom");
        propertyValue = FdoPropertyValue::Create(L"GEOMETRY", parameter);
        values->Add(propertyValue);

        FdoPtr<FdoBatchParameterValueCollection> batchParameters = insert->GetBatchParameterValues();
        FdoPtr<FdoParameterValueCollection> parameterValues;
        FdoPtr<FdoParameterValue>           parameterValue;

        for (int i=0; i<num_records;i++)
        {
            // Generate a random string:
            string_size = 1 + (rand() % 60);
            for (int j=0; j<string_size; j++)
                string_value[j] = L'a' + (rand() % 26);
            string_value[string_size] = (wchar_t)0;
            strVal = FdoStringValue::Create(string_value);

            // generate a random double:
            double_value = (double)rand() / 1000.0;
            dblVal = FdoDoubleValue::Create(double_value);

            // generate a random geometry value:
            FdoInt32 numOrdinates = 2*(2 + rand() % 2); // 2 or 3 vertices
            double ordinates[2*10];
            for (int z=0; z<(numOrdinates); z++)
            {
                //OLD WAY: ordinates[z] = ((double)rand() / (double)RAND_MAX) * 1000.0;

                if (z%2 == 0)  // X VALUE
                    ordinates[z] = (double)(i % (num_records/50)) / 2.0;
                else  // Y VALUE
                    ordinates[z] = (double)(i / (num_records/50))*20.0 + ((double)z/(double)numOrdinates)*15.0;
            }
            FdoPtr<FdoILineString> lineString = gf->CreateLineString(FdoDimensionality_XY, numOrdinates, ordinates);
            FdoPtr<FdoByteArray> geomByteArray = gf->GetFgf(lineString);
            FdoPtr<FdoGeometryValue> geomValue = FdoGeometryValue::Create(geomByteArray);

            // Fill the batch parameter value collection with multiple rows of data:
            parameterValues = FdoParameterValueCollection::Create();
            parameterValue = FdoParameterValue::Create(L"Param0", strVal);
            parameterValues->Add(parameterValue);
            parameterValue = FdoParameterValue::Create(L"Param1", dblVal);
            parameterValues->Add(parameterValue);
            parameterValue = FdoParameterValue::Create(L"ParamGeom", geomValue);
            parameterValues->Add(parameterValue);
            batchParameters->Add(parameterValues);

            // Perform the batch insert:
            if (i % 100 == 99)
            {
                reader = insert->Execute();
                batchParameters->Clear();
            }

            if (i % 1000 == 999)
            {
                printf("\nInsert data progress = %d features inserted: clock=%f\n", i, ((double)(long)clock()) / 1000.0);
            }
        }
        printf("\nInsert data end: clock=%f\n", ((double)(long)clock()) / 1000.0);
    }
    catch (FdoException* ge) 
    {
        fail (ge);
    }
}
