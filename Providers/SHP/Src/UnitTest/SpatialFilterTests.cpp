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
#include "SpatialFilterTests.h"

#include <ShpSpatialIndex.h>
#include <ShapeFile.h>
#include <ShapeIndex.h>


CPPUNIT_TEST_SUITE_REGISTRATION (SpatialFilterTests);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION (SpatialFilterTests, "SpatialFilterTests");

#ifdef _WIN32
    #define DEFAULT_LOCATION    L"DefaultFileLocation=..\\..\\TestData\\Ontario"
    #define DEFAULT_PATH        L"..\\..\\TestData\\Ontario"

    #define DEFAULT_LOCATION_USA_3G		L"DefaultFileLocation=..\\..\\..\\..\\..\\bugs\\USA_3G"
    #define DEFAULT_PATH_USA_3G			L"..\\..\\..\\..\\..\\bugs\\USA_3G"
#else
    #define DEFAULT_LOCATION    L"DefaultFileLocation=../../TestData/Ontario"
    #define DEFAULT_PATH        L"../../TestData/Ontario"

    #define DEFAULT_LOCATION_USA_3G		L"DefaultFileLocation=../../../../../bugs/USA_3G"
    #define DEFAULT_PATH_USA_3G			L"../../../../../bugs/USA_3G"
#endif

#define CLASS_NAME_ROADS    L"roads"
#define CLASS_NAME_ONTARIO  L"ontario"
#define CLASS_NAME_LAKES    L"lakes"

#define CLASS_NAME_USA_3G   L"USA_S0_line"

#define MAX_FEATURES_ROADS              1088
#define MAX_FEATURES_WITHIN_ROADS       1084
#define MAX_FEATURES_INSIDE_ROADS       1084

#define MAX_FEATURES_LAKES              96
#define MAX_FEATURES_WITHIN_LAKES       77
#define MAX_FEATURES_INSIDE_LAKES       63


static double SHP_MAX_EXTENTS[] = {    0, 0, 
                                       0, 20000000, 
                                       2000000, 20000000, 
                                       2000000, 0, 
                                       0, 0 }; 
// fn17 (linestring)
static double SHP_POINTS1[] = { 250893.43750, 12616630.00000, 
                                250893.43750, 12617222.00000, 
                                252774.15625, 12617222.00000, 
                                252774.15625, 12616630.00000, 
                                250893.43750, 12616630.00000 }; 

static double SHP_POINTS2[] = { 250893.43750, 12616630.00000, 
                                250893.43750, 12636563.00000, 
                                260077.34375, 12636563.00000, 
                                260077.34375, 12616630.00000, 
                                250893.43750, 12616630.00000 }; 


FdoPtr<FdoIConnection> SpatialFilterTests::mConnection;

SpatialFilterTests::SpatialFilterTests (void)
{
}

SpatialFilterTests::~SpatialFilterTests (void)
{
}

void SpatialFilterTests::setUp ()
{
    mConnection = ShpTests::GetConnection ();

    mConnection->SetConnectionString (DEFAULT_LOCATION);

    CPPUNIT_ASSERT_MESSAGE ("connection state not open", FdoConnectionState_Open == mConnection->Open ());
}

void SpatialFilterTests::tearDown ()
{
    mConnection->Close ();
	FDO_SAFE_RELEASE(mConnection.p);
}

void SpatialFilterTests::SelectAll ()
{
    FdoString*  class_name = CLASS_NAME_ROADS;

    // Get the file name
    FdoStringP  file_name;

#ifdef _WIN32
    file_name = FdoStringP::Format(L"%ls%ls%ls%ls", DEFAULT_PATH, L"\\", class_name, L".shx" );
#else
    file_name = FdoStringP::Format(L"%ls%ls%ls%ls", DEFAULT_PATH, L"/", class_name, L".shx" );  
#endif

    ShapeIndex shpi (file_name);

    long     numObjs = shpi.GetNumObjects ();

    if (VERBOSE)
        printf("--- FILE: '%ls'  numObjects %ld\n", (const wchar_t*) file_name, numObjs);

    FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance ();

    FdoPtr<FdoISelect> select = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
    select->SetFeatureClassName( class_name );
    FdoPtr<FdoIFeatureReader> reader = select->Execute ();

    int count = 0;
    while (reader->ReadNext ())
    {
        count++;
        reader->GetInt32 (L"FeatId");
        FdoPtr<FdoByteArray> geom_fgf = reader->GetGeometry (L"Geometry");

        FdoPtr<FdoIGeometry>   geom = gf->CreateGeometryFromFgf( geom_fgf );
        FdoPtr<FdoIEnvelope>   ext = geom->GetEnvelope();

        if (VERBOSE)
        {
            printf("--FeatId %ld\n", count-1 );
            printf("  %s   extents (%lf, %lf)(%lf, %lf)\n", "xxx", 
                                    ext->GetMinX(), ext->GetMinY(), 
                                    ext->GetMaxX(), ext->GetMaxY());
        }
    }

	// Make sure this test is run first. Check the fact the spatial index was not populated
	// (the IDX file is not new)
 //   FdoPtr<ShpLpClassDefinition> lpClass = ShpSchemaUtilities::GetLpClassDefinition (mConnection, CLASS_NAME_ROADS));
 //   ShpFileSet* fileset = lpClass->GetPhysicalFileSet ();
 //   ShpSpatialIndex* ssi = fileset->GetSpatialIndex(false);
	//CPPUNIT_ASSERT_MESSAGE ("Spatial Index should not have been loaded", ssi->GetNObjects() == 0);

}

void SpatialFilterTests::SelectAll_USA_3G ()
{
    FdoString*  class_name = CLASS_NAME_USA_3G;

    // Get the file name
    FdoStringP  file_name;

#ifdef _WIN32
	file_name = FdoStringP::Format(L"%ls%ls%ls%ls", DEFAULT_PATH_USA_3G, L"\\", class_name, L".shx" );
#else
	file_name = FdoStringP::Format(L"%ls%ls%ls%ls", DEFAULT_PATH_USA_3G, L"/", class_name, L".shx" );  
#endif

	try
	{
		ShapeIndex shpi (file_name);

		long     numObjs = shpi.GetNumObjects ();

		if (VERBOSE)
			printf("--- FILE: '%ls'  numObjects %ld\n", (const wchar_t*) file_name, numObjs);

		FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance ();

#ifdef _WIN32       
		double time1 = TestCommonMiscUtil::GetTime_S();
#endif

		FdoPtr<FdoIConnection> connection = ShpTests::GetConnection ();
		connection->SetConnectionString (DEFAULT_LOCATION_USA_3G);

		CPPUNIT_ASSERT_MESSAGE ("connection state not open", FdoConnectionState_Open == connection->Open ());

		FdoPtr<FdoISelect> select = (FdoISelect*)connection->CreateCommand (FdoCommandType_Select);
		select->SetFeatureClassName( class_name );
		FdoPtr<FdoIFeatureReader> reader = select->Execute ();

#ifdef _WIN32       
		double time2 = TestCommonMiscUtil::GetTime_S();
		double elapsed = time2 - time1;
		printf("Elapsed select->Execute(): %lf sec\n",elapsed);
#endif

#ifdef _WIN32       
		time1 = TestCommonMiscUtil::GetTime_S();
#endif

		int count = 0;
		while (reader->ReadNext ())
		{
			count++;
			reader->GetInt32 (L"FeatId");
			FdoPtr<FdoByteArray> geom_fgf = reader->GetGeometry (L"Geometry");

			FdoPtr<FdoIGeometry>   geom = gf->CreateGeometryFromFgf( geom_fgf );
			FdoPtr<FdoIEnvelope>   ext = geom->GetEnvelope();

			if (VERBOSE)
			{
				printf("--FeatId %ld\n", count-1 );
				printf("  %s   extents (%lf, %lf)(%lf, %lf)\n", "xxx", 
										ext->GetMinX(), ext->GetMinY(), 
										ext->GetMaxX(), ext->GetMaxY());
			}
		}
#ifdef _WIN32       
		time2 = TestCommonMiscUtil::GetTime_S();
		elapsed = time2 - time1;
		printf("Elapsed select->ReadNext(): %lf sec\n",elapsed);
#endif
		connection->Close ();
	}
    catch( FdoException* ex)
    {
        TestCommonFail (ex);
    }
}

////////////////////////////////////////////////////////////////////////////////
int SpatialFilterTests::runSpatialQuery(FdoString* class_name, FdoSpatialOperations spOp, double *polyOrdsXY, int numOrds)
{
    // Create a polygon from the input points
    FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
    FdoPtr<FdoILinearRing> outer = gf->CreateLinearRing(0, numOrds, polyOrdsXY);
    FdoPtr<FdoIPolygon> poly = gf->CreatePolygon(outer, NULL);

    return runSpatialQuery( class_name, spOp, poly );
}

////////////////////////////////////////////////////////////////////////////////
int SpatialFilterTests::runSpatialQuery( FdoString* class_name, FdoSpatialOperations spOp, FdoIPolygon* poly)
{
    int count = 0;

    try
    {
        FdoPtr<FdoISelect> select = (FdoISelect*)mConnection->CreateCommand(FdoCommandType_Select); 

        select->SetFeatureClassName( class_name );
        
        // Create a polygon from the input points
        FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
        //FdoPtr<FdoILinearRing> outer = gf->CreateLinearRing(0, numOrds, polyOrdsXY);
        //FdoPtr<FdoIPolygon> poly = gf->CreatePolygon(outer, NULL);

        FdoPtr<FdoByteArray> polyfgf = gf->GetFgf(poly);
        FdoPtr<FdoGeometryValue> gv = FdoGeometryValue::Create(polyfgf);

        FdoPtr<FdoSpatialCondition> filter = FdoSpatialCondition::Create(L"Geometry", spOp, gv);

        select->SetFilter(filter);

#ifdef _WIN32       
        double time1 = TestCommonMiscUtil::GetTime_S();
#endif

        FdoPtr<FdoIFeatureReader> reader = select->Execute();

        while (reader->ReadNext())
        {
            FdoInt32 featId = reader->GetInt32 (L"FeatId");
            FdoPtr<FdoByteArray> geometry = reader->GetGeometry (L"Geometry");

            //if (VERBOSE)
                //printf("featid = %ld\n", featId);
            count++;
        }

        if (VERBOSE)
            printf("Count = %d\n", count);

#ifdef _WIN32       
        double time2 = TestCommonMiscUtil::GetTime_S();
        double elapsed = time2 - time1;
        if (VERBOSE)
			printf("Elapsed: %lf sec\n",elapsed);
#endif
    }
    catch( FdoException* ex)
    {
        TestCommonFail (ex);
    }
    catch( ... )
    {
        printf("ERROR runSpatialQuery()");
        throw 0;
    }

    return count;
}

////////////////////////////////////////////////////////////////////////////////
void SpatialFilterTests::PerformanceTests_Roads( )
{
    runPerformanceTests( CLASS_NAME_ROADS, MAX_FEATURES_ROADS, MAX_FEATURES_WITHIN_ROADS, MAX_FEATURES_INSIDE_ROADS );
}

void SpatialFilterTests::PerformanceTests_Lakes( )
{
    runPerformanceTests( CLASS_NAME_LAKES, MAX_FEATURES_LAKES, MAX_FEATURES_WITHIN_LAKES, MAX_FEATURES_INSIDE_LAKES );
}

////////////////////////////////////////////////////////////////////////////////
void SpatialFilterTests::runPerformanceTests( FdoString* class_name, int total_feats, int within_feats, int inside_feats )
{
    FdoString*  class_name2 = CLASS_NAME_ONTARIO;

    if (VERBOSE)
        printf("[ %ls  vs. %ls]\n", class_name, class_name2 );

    int count1 = runSpatialQueryWithPolygonFromFile( class_name, FdoSpatialOperations_EnvelopeIntersects, class_name2);
    CPPUNIT_ASSERT_MESSAGE ("Wrong number of selected features", count1 == total_feats);

    int count2 = runSpatialQueryWithPolygonFromFile( class_name, FdoSpatialOperations_Within, class_name2 );
    CPPUNIT_ASSERT_MESSAGE ("Wrong number of selected features", count2 == within_feats);

    int count3 = runSpatialQueryWithPolygonFromFile( class_name, FdoSpatialOperations_Inside, class_name2 );
    CPPUNIT_ASSERT_MESSAGE ("Wrong number of selected features", count3 == inside_feats);
}

////////////////////////////////////////////////////////////////////////////////
void SpatialFilterTests::EnvelopeIntersects()
{
    FdoSpatialOperations  spOp = FdoSpatialOperations_EnvelopeIntersects;

    int count1 = runSpatialQuery( CLASS_NAME_ROADS, spOp, SHP_MAX_EXTENTS, ELEMENTS(SHP_MAX_EXTENTS));
    CPPUNIT_ASSERT_MESSAGE ("Wrong number of selected features", count1 == MAX_FEATURES_ROADS);
    
    int count2 = runSpatialQuery( CLASS_NAME_ROADS, spOp, SHP_POINTS1, ELEMENTS(SHP_POINTS1));
    CPPUNIT_ASSERT_MESSAGE ("Wrong number of selected features", count2 == 4);
}

////////////////////////////////////////////////////////////////////////////////
void SpatialFilterTests::Within()
{
    FdoSpatialOperations spOp = FdoSpatialOperations_Within;

    int count1 = runSpatialQuery( CLASS_NAME_ROADS, spOp, SHP_POINTS2, ELEMENTS(SHP_POINTS2));
    CPPUNIT_ASSERT_MESSAGE ("Wrong number of selected features", count1 == 4);
}

////////////////////////////////////////////////////////////////////////////////
void SpatialFilterTests::Inside()
{
    FdoSpatialOperations spOp = FdoSpatialOperations_Inside;

    int count1 = runSpatialQuery( CLASS_NAME_ROADS, FdoSpatialOperations_Inside, SHP_POINTS1, ELEMENTS(SHP_POINTS1));
    CPPUNIT_ASSERT_MESSAGE ("Wrong number of selected features", count1 == 0);
}

////////////////////////////////////////////////////////////////////////////////
void SpatialFilterTests::Intersects()
{
    FdoSpatialOperations spOp = FdoSpatialOperations_Intersects;

    int count1 = runSpatialQuery( CLASS_NAME_ROADS, FdoSpatialOperations_Intersects, SHP_POINTS1, ELEMENTS(SHP_POINTS1));
    CPPUNIT_ASSERT_MESSAGE ("Wrong number of selected features", count1 == 3);
}

////////////////////////////////////////////////////////////////////////////////
int SpatialFilterTests::runSpatialQueryWithPolygonFromFile( FdoString* class_name1, FdoSpatialOperations spOp, FdoString* class_name2 )
{
    if ( spOp == FdoSpatialOperations_EnvelopeIntersects )
    {
        if (VERBOSE)
            printf( "  .EnvelopeIntersects\n");
    }
    else if ( spOp == FdoSpatialOperations_Within )
    {
        if (VERBOSE)
            printf( "  .Within\n");
    }
    else if ( spOp == FdoSpatialOperations_Inside )
    {
        if (VERBOSE)
            printf( "  .Inside\n");
    }
    else if ( spOp == FdoSpatialOperations_Intersects )
    {
        if (VERBOSE)
            printf( "  .Intersects\n");
    }
    else
    {
        printf( "Unsupported spatial operation.");
        throw;
    }
    
    // Get the file name
    FdoStringP  file_name;

#ifdef _WIN32
    file_name = FdoStringP::Format(L"%ls%ls%ls%ls", DEFAULT_PATH, L"\\", class_name2, L".shx" );
#else
    file_name = FdoStringP::Format(L"%ls%ls%ls%ls", DEFAULT_PATH, L"/", class_name2, L".shx" );  
#endif

    // Get the 1st polygon from file
    FdoIPolygon* poly = getPolygonFromFile (file_name, class_name2);

    int count = runSpatialQuery( class_name1, spOp, poly );

    poly->Release();

    return count;
}

////////////////////////////////////////////////////////////////////////////////
FdoIPolygon*  SpatialFilterTests::getPolygonFromFile (FdoString* file_name, FdoString* class_name)
{
    ShapeIndex shpi (file_name);
    long     numObjs = shpi.GetNumObjects ();
    
    //if (VERBOSE)
    //    printf("    FILE: '%ls'  numObjects %ld\n", file_name, numObjs);

    FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance ();

    FdoPtr<FdoISelect> select = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
    select->SetFeatureClassName (class_name);
    FdoPtr<FdoIFeatureReader> reader = select->Execute ();

    int             count = 0;
    FdoIGeometry*   geom = NULL;

    while (reader->ReadNext ())
    {
        count++;
        reader->GetInt32 (L"FeatId");
        FdoPtr<FdoByteArray> geom_fgf = reader->GetGeometry (L"Geometry");

        geom = gf->CreateGeometryFromFgf( geom_fgf );
        break;
    }
    return (FdoIPolygon*)geom;
}

// unsupported spatial operation
void SpatialFilterTests::Crosses()
{
    try
    {
        FdoSpatialOperations spOp = FdoSpatialOperations_Crosses;
        int count1 = runSpatialQuery (CLASS_NAME_ROADS, spOp, SHP_POINTS2, ELEMENTS(SHP_POINTS2));
        CPPUNIT_FAIL ("no exception for FdoSpatialOperations_Crosses");
    }
    catch (CppUnit::Exception error)
    {
        // check for expected message
        CPPUNIT_ASSERT_MESSAGE ("wrong exception", 0 == strncmp ("The spatial operation", error.what (), 21));
    }
}

// unsupported spatial operation
void SpatialFilterTests::Touches()
{
    try
    {
        FdoPtr<FdoISelect> select = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select); 
        select->SetFeatureClassName (CLASS_NAME_ROADS);
		select->SetFilter (L"Geometry TOUCHES GeomFromText('POLYGON XYZ ((754585.841880672 1379595.67650762 0, 754695.110872774 1379848.11708386 0, 754441.744600538 1379964.26325997 0, 754332.471512436 1379711.82268373 0, 754585.841880672 1379595.67650762 0))')");
        FdoPtr<FdoIFeatureReader> reader = select->Execute ();
		int count = 0;
		while (reader->ReadNext ())
        {
			wprintf (L"\n%s", reader->GetString (L"FCODE"));
			count++;
		}			
		wprintf (L"\nNumber of features returned: %ld", count);
		reader->Close ();
        CPPUNIT_FAIL ("no exception for FdoSpatialOperations_Touches");
    }
    catch (FdoException* ge)
    {
        // check for expected message
        CPPUNIT_ASSERT_MESSAGE ("wrong exception", 0 == wcsncmp (L"The spatial operation", ge->GetExceptionMessage (), 21));
        ge->Release ();
    }
}

// wrong geometry property name
void SpatialFilterTests::Bogus ()
{
    try
    {
        FdoPtr<FdoISelect> select = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select); 
        select->SetFeatureClassName (CLASS_NAME_ROADS);
		select->SetFilter (L"SHAPE WITHIN geomfromtext('POLYGON XY( ( 400000 1400000, 500000 1400000, 500000 1550000, 400000 1550000, 400000 1400000) )') and (FCODE like 'DA249%' or MNRCODE = 106)");
        FdoPtr<FdoIFeatureReader> reader = select->Execute ();
		int count = 0;
		while (reader->ReadNext ())
        {
			wprintf (L"\n%d", reader->GetInt32 (L"FeatId"));
			count++;
		}			
		wprintf (L"\nNumber of features returned: %ld", count);
		reader->Close ();
        CPPUNIT_FAIL ("no exception for invalid property name");
    }
    catch (FdoException* ge)
    {
        // check for expected message
        FdoStringP message = ge->GetExceptionMessage ();
        ge->Release ();
        if (   (NULL != wcsstr ((FdoString*)message, L"The property 'SHAPE' is not in the reader's selected property set."))
            && (NULL != wcsstr ((FdoString*)message, L"FDO_104_READER_PROPERTY_NOT_SELECTED")))
        {
            CPPUNIT_FAIL ("wrong exception regarding invalid selected property");
        }
    }
}

