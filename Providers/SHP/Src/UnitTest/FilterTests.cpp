/*
 * 
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
#include "FilterTests.h"

CPPUNIT_TEST_SUITE_REGISTRATION (FilterTests);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION (FilterTests, "FilterTests");

FdoPtr<FdoIConnection> FilterTests::mConnection;

FilterTests::FilterTests (void)
{
}

FilterTests::~FilterTests (void)
{
}

void FilterTests::setUp ()
{
    mConnection = ShpTests::GetConnection ();
#ifdef _WIN32
    mConnection->SetConnectionString (L"DefaultFileLocation=..\\..\\TestData\\Ontario");
#else
    mConnection->SetConnectionString (L"DefaultFileLocation=../../TestData/Ontario");
#endif
    CPPUNIT_ASSERT_MESSAGE ("connection state not open", FdoConnectionState_Open == mConnection->Open ());
}

void FilterTests::tearDown ()
{
    mConnection->Close ();
}

void FilterTests::equal ()
{
    try
    {
        FdoPtr<FdoISelect> select = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
        select->SetFeatureClassName (L"lakes");
		select->SetFilter (FdoPtr<FdoFilter>(FdoFilter::Parse (L"FeatId = 42")));
        FdoPtr<FdoIFeatureReader> reader = select->Execute ();
        int count = 0;
        while (reader->ReadNext ())
        {
            count++;
            reader->GetInt32 (L"FeatId");
            if (!reader->IsNull(L"AREA"))
                reader->GetDouble (L"AREA");
            if (!reader->IsNull(L"PERIMETER"))
                reader->GetDouble (L"PERIMETER");
            if (!reader->IsNull(L"DRAINAGE_"))
                reader->GetDouble (L"DRAINAGE_");
            if (!reader->IsNull(L"DRAINAGE_I"))
                reader->GetDouble (L"DRAINAGE_I");
            if (!reader->IsNull(L"HOTLINK"))
                reader->GetString (L"HOTLINK");
            if (!reader->IsNull(L"Geometry"))
                FdoPtr<FdoByteArray> geometry = reader->GetGeometry (L"Geometry");
        }
        CPPUNIT_ASSERT_MESSAGE ("not 1 feature selected", 1 == count);
    }
    catch (FdoException *e)
    {
        fail(e);
    }
}

void FilterTests::less ()
{
    try
    {
        FdoPtr<FdoISelect> select = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
        select->SetFeatureClassName (L"lakes");
        select->SetFilter (FdoPtr<FdoFilter>(FdoFilter::Parse (L"PERIMETER < 20000")));
        FdoPtr<FdoIFeatureReader> reader = select->Execute ();
        int count = 0;
        while (reader->ReadNext ())
        {
            count++;
            reader->GetInt32 (L"FeatId");
            if (!reader->IsNull(L"AREA"))
                reader->GetDouble (L"AREA");
            if (!reader->IsNull(L"PERIMETER"))
                reader->GetDouble (L"PERIMETER");
            if (!reader->IsNull(L"DRAINAGE_"))
                reader->GetDouble (L"DRAINAGE_");
            if (!reader->IsNull(L"DRAINAGE_I"))
                reader->GetDouble (L"DRAINAGE_I");
            if (!reader->IsNull(L"HOTLINK"))
                reader->GetString (L"HOTLINK");
            if (!reader->IsNull(L"Geometry"))
                FdoPtr<FdoByteArray> geometry = reader->GetGeometry (L"Geometry");
        }
        CPPUNIT_ASSERT_MESSAGE ("not 1 feature selected", 1 == count);
    }
    catch (FdoException *e)
    {
        fail(e);
    }
}

void FilterTests::greater ()
{
    try
    {
        FdoPtr<FdoISelect> select = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
        select->SetFeatureClassName (L"lakes");
        select->SetFilter (FdoPtr<FdoFilter>(FdoFilter::Parse (L"AREA > 55000000000")));
        FdoPtr<FdoIFeatureReader> reader = select->Execute ();
        int count = 0;
        while (reader->ReadNext ())
        {
            count++;
            reader->GetInt32 (L"FeatId");
            if (!reader->IsNull(L"AREA"))
                reader->GetDouble (L"AREA");
            if (!reader->IsNull(L"PERIMETER"))
                reader->GetDouble (L"PERIMETER");
            if (!reader->IsNull(L"DRAINAGE_"))
                reader->GetDouble (L"DRAINAGE_");
            if (!reader->IsNull(L"DRAINAGE_I"))
                reader->GetDouble (L"DRAINAGE_I");
            if (!reader->IsNull(L"HOTLINK"))
                reader->GetString (L"HOTLINK");
            if (!reader->IsNull(L"Geometry"))
                FdoPtr<FdoByteArray> geometry = reader->GetGeometry (L"Geometry");
        }
        CPPUNIT_ASSERT_MESSAGE ("not 1 feature selected", 1 == count);
    }
    catch (FdoException *e)
    {
        fail(e);
    }
}


void FilterTests::not_equal ()
{
    try
    {
        FdoPtr<FdoISelect> select = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
        select->SetFeatureClassName (L"lakes");
        select->SetFilter (FdoPtr<FdoFilter>(FdoFilter::Parse (L"HOTLINK != ''")));
        FdoPtr<FdoIFeatureReader> reader = select->Execute ();
        int count = 0;
        while (reader->ReadNext ())
        {
            count++;
            reader->GetInt32 (L"FeatId");
            if (!reader->IsNull(L"AREA"))
                reader->GetDouble (L"AREA");
            if (!reader->IsNull(L"PERIMETER"))
                reader->GetDouble (L"PERIMETER");
            if (!reader->IsNull(L"DRAINAGE_"))
                reader->GetDouble (L"DRAINAGE_");
            if (!reader->IsNull(L"DRAINAGE_I"))
                reader->GetDouble (L"DRAINAGE_I");
            if (!reader->IsNull(L"HOTLINK"))
                reader->GetString (L"HOTLINK");
            if (!reader->IsNull(L"Geometry"))
                FdoPtr<FdoByteArray> geometry = reader->GetGeometry (L"Geometry");
        }
        CPPUNIT_ASSERT_MESSAGE ("not 1 feature selected", 1 == count);
    }
    catch (FdoException *e)
    {
        fail(e);
    }
}

void FilterTests::in ()
{
    try
    {
        FdoPtr<FdoISelect> select = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
        select->SetFeatureClassName (L"lakes");
        select->SetFilter (FdoPtr<FdoFilter>(FdoFilter::Parse (L"FeatId in (42, 49)")));
        FdoPtr<FdoIFeatureReader> reader = select->Execute ();
        int count = 0;
        while (reader->ReadNext ())
        {
            count++;
            FdoInt32 id = reader->GetInt32 (L"FeatId");
            CPPUNIT_ASSERT_MESSAGE ("wrong feature selected", ((id == 42) || (id == 49)));
            if (!reader->IsNull(L"AREA"))
                reader->GetDouble (L"AREA");
            if (!reader->IsNull(L"PERIMETER"))
                reader->GetDouble (L"PERIMETER");
            if (!reader->IsNull(L"DRAINAGE_"))
                reader->GetDouble (L"DRAINAGE_");
            if (!reader->IsNull(L"DRAINAGE_I"))
                reader->GetDouble (L"DRAINAGE_I");
            if (!reader->IsNull(L"HOTLINK"))
                reader->GetString (L"HOTLINK");
            if (!reader->IsNull(L"Geometry"))
                FdoPtr<FdoByteArray> geometry = reader->GetGeometry (L"Geometry");
        }
        CPPUNIT_ASSERT_MESSAGE ("more than 2 features selected", 2 == count);
    }
    catch (FdoException *e)
    {
        fail(e);
    }
}

void FilterTests::is_null ()
{
    try
    {
        FdoPtr<FdoISelect> select = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
        select->SetFeatureClassName (L"lakes");

        select->SetFilter (FdoPtr<FdoFilter>(FdoFilter::Parse (L"HOTLINK NULL")));
        FdoPtr<FdoIFeatureReader> reader = select->Execute ();
        int count = 0;
        while (reader->ReadNext ())
        {
            count++;
            reader->GetInt32 (L"FeatId");
            if (!reader->IsNull(L"AREA"))
                reader->GetDouble (L"AREA");
            if (!reader->IsNull(L"PERIMETER"))
                reader->GetDouble (L"PERIMETER");
            if (!reader->IsNull(L"DRAINAGE_"))
                reader->GetDouble (L"DRAINAGE_");
            if (!reader->IsNull(L"DRAINAGE_I"))
                reader->GetDouble (L"DRAINAGE_I");
            CPPUNIT_ASSERT_MESSAGE("HOTLINK must be NULL", reader->IsNull(L"HOTLINK"));
            if (!reader->IsNull(L"Geometry"))
                FdoPtr<FdoByteArray> geometry = reader->GetGeometry (L"Geometry");
        }
        CPPUNIT_ASSERT_MESSAGE ("not 95 features selected", 95 == count);
    }
    catch (FdoException *e)
    {
        fail(e);
    }
}


void FilterTests::arithmetic_expression ()
{
    try
    {
        FdoPtr<FdoISelect> select = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
        select->SetFeatureClassName (L"lakes");
        select->SetFilter (FdoPtr<FdoFilter>(FdoFilter::Parse (L"AREA > 0.75 * 3.14159 * (-(PERIMETER / 3.14159 / -2 + 7) * (PERIMETER / 3.14159 / 2 - 4))")));
        FdoPtr<FdoIFeatureReader> reader = select->Execute ();
        int count = 0;
        while (reader->ReadNext ())
        {
            count++;
            reader->GetInt32 (L"FeatId");
            if (!reader->IsNull(L"AREA"))
                reader->GetDouble (L"AREA");
            if (!reader->IsNull(L"PERIMETER"))
                reader->GetDouble (L"PERIMETER");
            if (!reader->IsNull(L"DRAINAGE_"))
                reader->GetDouble (L"DRAINAGE_");
            if (!reader->IsNull(L"DRAINAGE_I"))
                reader->GetDouble (L"DRAINAGE_I");
            if (!reader->IsNull(L"HOTLINK"))
                reader->GetString (L"HOTLINK");
            if (!reader->IsNull(L"Geometry"))
                FdoPtr<FdoByteArray> geometry = reader->GetGeometry (L"Geometry");
        }
        CPPUNIT_ASSERT_MESSAGE ("not 1 feature selected", 1 == count);
    }
    catch (FdoException *e)
    {
        fail(e);
    }
}

void FilterTests::logical_expression ()
{
    try
    {
        FdoPtr<FdoISelect> select = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
        select->SetFeatureClassName (L"lakes");
        select->SetFilter (FdoPtr<FdoFilter>(FdoFilter::Parse (L"DRAINAGE_ > 178 and not AREA <= 130000000")));
        FdoPtr<FdoIFeatureReader> reader = select->Execute ();
        int count = 0;
        while (reader->ReadNext ())
        {
            count++;
            reader->GetInt32 (L"FeatId");
            if (!reader->IsNull(L"AREA"))
                reader->GetDouble (L"AREA");
            if (!reader->IsNull(L"PERIMETER"))
                reader->GetDouble (L"PERIMETER");
            if (!reader->IsNull(L"DRAINAGE_"))
                reader->GetDouble (L"DRAINAGE_");
            if (!reader->IsNull(L"DRAINAGE_I"))
                reader->GetDouble (L"DRAINAGE_I");
            if (!reader->IsNull(L"HOTLINK"))
                reader->GetString (L"HOTLINK");
            if (!reader->IsNull(L"Geometry"))
                FdoPtr<FdoByteArray> geometry = reader->GetGeometry (L"Geometry");
        }
        CPPUNIT_ASSERT_MESSAGE ("not 1 feature selected", 1 == count);
    }
    catch (FdoException *e)
    {
        fail(e);
    }
}

void FilterTests::string_expression ()
{
    try
    {
        FdoPtr<FdoISelect> select = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
        select->SetFeatureClassName (L"lakes");
        select->SetFilter (FdoPtr<FdoFilter>(FdoFilter::Parse (L"HOTLINK like '%_[tf][^tf][tf]'")));
        FdoPtr<FdoIFeatureReader> reader = select->Execute ();
        int count = 0;
        while (reader->ReadNext ())
        {
            count++;
            reader->GetInt32 (L"FeatId");
            if (!reader->IsNull(L"AREA"))
                reader->GetDouble (L"AREA");
            if (!reader->IsNull(L"PERIMETER"))
                reader->GetDouble (L"PERIMETER");
            if (!reader->IsNull(L"DRAINAGE_"))
                reader->GetDouble (L"DRAINAGE_");
            if (!reader->IsNull(L"DRAINAGE_I"))
                reader->GetDouble (L"DRAINAGE_I");
            if (!reader->IsNull(L"HOTLINK"))
                reader->GetString (L"HOTLINK");
            if (!reader->IsNull(L"Geometry"))
                FdoPtr<FdoByteArray> geometry = reader->GetGeometry (L"Geometry");
        }
        CPPUNIT_ASSERT_MESSAGE ("not 1 feature selected", 1 == count);
    }
    catch (FdoException *e)
    {
        fail(e);
    }
}

void FilterTests::functions ()
{
    try
    {
        FdoPtr<FdoISelect> select = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
        select->SetFeatureClassName (L"lakes");
        select->SetFilter (FdoPtr<FdoFilter>(FdoFilter::Parse (L"HOTLINK = Concat (Lower('C:\\BOB\\TEMP\\'), 'benchmark.tif')")));
        FdoPtr<FdoIFeatureReader> reader = select->Execute ();
        int count = 0;
        while (reader->ReadNext ())
        {
            count++;
            reader->GetInt32 (L"FeatId");
            if (!reader->IsNull(L"AREA"))
                reader->GetDouble (L"AREA");
            if (!reader->IsNull(L"PERIMETER"))
                reader->GetDouble (L"PERIMETER");
            if (!reader->IsNull(L"DRAINAGE_"))
                reader->GetDouble (L"DRAINAGE_");
            if (!reader->IsNull(L"DRAINAGE_I"))
                reader->GetDouble (L"DRAINAGE_I");
            if (!reader->IsNull(L"HOTLINK"))
                reader->GetString (L"HOTLINK");
            if (!reader->IsNull(L"Geometry"))
                FdoPtr<FdoByteArray> geometry = reader->GetGeometry (L"Geometry");
        }
        CPPUNIT_ASSERT_MESSAGE ("not 1 feature selected", 1 == count);
    }
    catch (FdoException *e)
    {
        fail(e);
    }
}

void FilterTests::computed ()
{
    try
    {
        FdoPtr<FdoISelect> select = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
        select->SetFeatureClassName (L"lakes");
        FdoPtr<FdoExpression> arg = FdoExpression::Parse (L"PERIMETER / 3.14159 / 2");
        FdoPtr<FdoComputedIdentifier> radius = FdoComputedIdentifier::Create (L"radius", arg/*s*/);
        FdoPtr <FdoIdentifier> id = FdoIdentifier::Create (L"FeatId");
        FdoPtr <FdoIdentifierCollection> ids = select->GetPropertyNames ();
        ids->Add (id);
        ids->Add (radius);
        select->SetFilter (FdoPtr<FdoFilter>(FdoFilter::Parse (L"AREA > 0.75 * 3.14159 * (radius * radius)")));
        FdoPtr<FdoIFeatureReader> reader = select->Execute ();
        int count = 0;
        while (reader->ReadNext ())
        {
            count++;
            reader->GetInt32 (L"FeatId");
            reader->GetDouble (L"radius");
        }
        CPPUNIT_ASSERT_MESSAGE ("not 1 feature selected", 1 == count);
    }
    catch (FdoException *e)
    {
        fail(e);
    }
}

void FilterTests::computed_geometry ()
{
    try
    {
        FdoPtr<FdoISelect> select = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
        select->SetFeatureClassName (L"lakes");
        FdoPtr<FdoComputedIdentifier> shape = FdoComputedIdentifier::Create (L"shape", FdoIdentifier::Create (L"Geometry"));
        FdoPtr <FdoIdentifier> id = FdoIdentifier::Create (L"FeatId");
        FdoPtr <FdoIdentifierCollection> ids = select->GetPropertyNames ();
        ids->Add (id);
        ids->Add (shape);
        select->SetFilter (FdoPtr<FdoFilter>(FdoFilter::Parse (L"FeatId = 42")));
        FdoPtr<FdoIFeatureReader> reader = select->Execute ();
        int count = 0;
        while (reader->ReadNext ())
        {
            count++;
            reader->GetInt32 (L"FeatId");
            FdoPtr<FdoByteArray> geometry = reader->GetGeometry (L"shape");
        }
        CPPUNIT_ASSERT_MESSAGE ("not 1 feature selected", 1 == count);
    }
    catch (FdoException *e)
    {
        fail(e);
    }
}

void FilterTests::non_existent_featid ()
{
    try
    {
        FdoPtr<FdoISelect> select = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
        select->SetFeatureClassName (L"lakes");

		// Try a non-existing feature. No exception should occur (defect #742445)
        select->SetFilter (FdoPtr<FdoFilter>(FdoFilter::Parse (L"FeatId = 100042")));

		try {
			FdoPtr<FdoIFeatureReader> reader = select->Execute ();
			int count = 0;
			while (reader->ReadNext ()) {
				count++;
			}
			CPPUNIT_ASSERT_MESSAGE ("Not 0 feature selected", 0 == count);
		}
		catch (FdoException *e)
		{
			e->Release();
			CPPUNIT_ASSERT_MESSAGE ("No exception should be thrown", false);
		}

		// Try a non-existing features. No exception should occur (defect #742445)
        select->SetFilter (FdoPtr<FdoFilter>(FdoFilter::Parse (L"FeatId in (42, 100042)")));

		try {
			FdoPtr<FdoIFeatureReader> reader = select->Execute ();
			int count = 0;
			while (reader->ReadNext ()) {
				count++;
			}
			CPPUNIT_ASSERT_MESSAGE ("Not 1 feature selected", 1 == count);
		}
		catch (FdoException *e)
		{
			e->Release();
			CPPUNIT_ASSERT_MESSAGE ("No exception should be thrown", false);
		}
    }
    catch (FdoException *e)
    {
        fail(e);
    }
}

///////////////////////////////////////////////////////////////////////////////////////
/// FeatId query optimizer tests
///////////////////////////////////////////////////////////////////////////////////////
#define VERBOSE false

int FilterTests::featid_roads_query (FdoString* query, int numRuns)
{
    int count = 0;

    FdoPtr<FdoISelect> select = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
    select->SetFeatureClassName (L"roads");

    FdoPtr<FdoFilter> filter = FdoFilter::Parse (query);
    select->SetFilter (filter);

    // Memory leak!
    //select->SetFilter (FdoPtr<FdoFilter>(FdoFilter::Parse (query)));

    for ( int i = 0; i < numRuns; i++ )
    {
        FdoPtr<FdoIFeatureReader> reader = select->Execute ();
        while (reader->ReadNext ())
        {
            count++;
            FdoInt32 fn = reader->GetInt32 (L"FeatId");
            /*if (VERBOSE)*/
                //printf("%d. %ld\n", count, fn);
        }
    }
    if (VERBOSE)
        printf("  >> count %ld\n", count );

    return count;
}

/////////////////////////////////////////////////////////////////////////////////////////
void FilterTests::featid_optimizer_tests ()
{
    try
    {
        int         count = 0;
        FdoString*  query;

        query = L"FeatId in (41, 42, 43) and not FeatId=42";
        count = featid_roads_query( query, 1 );
        CPPUNIT_ASSERT_MESSAGE ("1. not 2 feature selected", 2 == count);

        query = L"not FeatId in (41, 42, 43)";
        count = featid_roads_query( query, 1 );
        CPPUNIT_ASSERT_MESSAGE ("2. not 1085 feature selected", 1085 == count);

        query = L"FeatId=1 or FeatId=2 and FeatId in (2,3)";
        count = featid_roads_query( query, 1 );
        CPPUNIT_ASSERT_MESSAGE ("3. not 2 feature selected", 2 == count);
    }
    catch (FdoException *e)
    {
        fail(e);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////
#define NUM_RUNS    1000

void FilterTests::featid_performance_tests ()
{
    try
    {
        int         count = 0;
        FdoString*  query;

#ifdef _WIN32       
        CTime time1(CTime::GetCurrentTime());
#endif

        // This is the max featId, should return 1 row.
        query = L"FeatId = 1087";
        count = featid_roads_query( query, NUM_RUNS );
        CPPUNIT_ASSERT_MESSAGE ("P. not 2 feature selected", NUM_RUNS == count);

#ifdef _WIN32
        CTime time2(CTime::GetCurrentTime());
        CTimeSpan elapsed = time2 - time1;

        if (VERBOSE)
            printf("  >> elapsed: %s\n",elapsed.Format("%H:%M:%S" ) );
#endif
    }
    catch (FdoException *e)
    {
        fail(e);
    }
}


