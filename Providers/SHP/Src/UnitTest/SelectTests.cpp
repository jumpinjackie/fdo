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
#include "SelectTests.h"
#include <Spatial/SpatialStd.h>
#include <Spatial/SpatialUtility.h>

CPPUNIT_TEST_SUITE_REGISTRATION (SelectTests);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION (SelectTests, "SelectTests");

FdoPtr<FdoIConnection> SelectTests::mConnection;

SelectTests::SelectTests (void)
{
}

SelectTests::~SelectTests (void)
{
}

void SelectTests::setUp ()
{
    mConnection = ShpTests::GetConnection ();
#ifdef _WIN32
    mConnection->SetConnectionString (L"DefaultFileLocation=..\\..\\TestData\\Ontario");
#else
    mConnection->SetConnectionString (L"DefaultFileLocation=../../TestData/Ontario");
#endif
    CPPUNIT_ASSERT_MESSAGE ("connection state not open", FdoConnectionState_Open == mConnection->Open ());
}

void SelectTests::tearDown ()
{
    mConnection->Close ();
	FDO_SAFE_RELEASE(mConnection.p);
}

void SelectTests::select ()
{
    double  area;
    double  length;
    int count;

    try
    {
        FdoPtr<FdoISelect> select = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
        select->SetFeatureClassName (L"ontario");
        FdoPtr<FdoIFeatureReader> reader = select->Execute ();
        count = 0;
        while (reader->ReadNext ())
        {
            count++;
            reader->GetInt32 (L"FeatId");
            area = reader->GetDouble (L"AREA");
            length = reader->GetDouble (L"PERIMETER");
            reader->GetDouble (L"ONTARIO_");
            reader->GetDouble (L"ONTARIO_ID");
            FdoPtr<FdoByteArray> geometry = reader->GetGeometry (L"Geometry");

            AnalyzeGeometry( L"ontario", count, geometry, length, area);
        }
        CPPUNIT_ASSERT_MESSAGE ("no ontario features selected", 0 != count);

        select->SetFeatureClassName (L"roads");
        reader = select->Execute ();
        count = 0;
        while (reader->ReadNext ())
        {
            count++;
            reader->GetInt32 (L"FeatId");
            reader->GetDouble (L"FNODE_");
            reader->GetDouble (L"TNODE_");
            reader->GetDouble (L"LPOLY_");
            reader->GetDouble (L"RPOLY_");
            length = reader->GetDouble (L"LENGTH");
            reader->GetDouble (L"ROADWAYS_");
            reader->GetDouble (L"ROADWAYS_I");
            reader->GetDouble (L"MNRCODE");
            reader->GetString (L"FCODE");
            FdoPtr<FdoByteArray> geometry = reader->GetGeometry (L"Geometry");

            AnalyzeGeometry( L"ontario", count, geometry, length, area);
        }
        CPPUNIT_ASSERT_MESSAGE ("no roads features selected", 0 != count);

        select->SetFeatureClassName (L"lakes");
        reader = select->Execute ();
        count = 0;
        while (reader->ReadNext ())
        {
            count++;
            reader->GetInt32 (L"FeatId");
            area = reader->GetDouble (L"AREA");
            length = reader->GetDouble (L"PERIMETER");
            reader->GetDouble (L"DRAINAGE_");
            reader->GetDouble (L"DRAINAGE_I");
            if (!reader->IsNull(L"HOTLINK"))
                reader->GetString (L"HOTLINK");
            FdoPtr<FdoByteArray> geometry = reader->GetGeometry (L"Geometry");

            AnalyzeGeometry( L"ontario", count, geometry, length, area);
        }
        CPPUNIT_ASSERT_MESSAGE ("no lakes features selected", 0 != count);
    }
    catch (FdoException* ge)
    {
        fail(ge);
    }
    catch (CppUnit::Exception error)
    {
        throw error;
    }
    catch(...)
    {
        CPPUNIT_FAIL ("SelectTests::select() failed");
    }

}

void SelectTests::get_spatial_contexts()
{
    try
    {
        // Test #1. No PRJ file
        get_spatial_context( mConnection, L"", 1 );

        // Test #2. PRJ file present
        FdoPtr<FdoIConnection>  connection = ShpTests::GetConnection ();
#ifdef _WIN32
        connection->SetConnectionString (L"DefaultFileLocation=..\\..\\TestData\\Florida");
#else
        connection->SetConnectionString (L"DefaultFileLocation=../../TestData/Florida");
#endif
        CPPUNIT_ASSERT_MESSAGE ("connection state not open", FdoConnectionState_Open == connection->Open ());

        get_spatial_context( connection, L"NAD_1983_HARN_UTM_Zone_17N", 2);

        connection->Close();

		// Test #3. Multiple PRJ file present, but the same content
        connection = ShpTests::GetConnection ();
#ifdef _WIN32
        connection->SetConnectionString (L"DefaultFileLocation=..\\..\\TestData\\Sheboygan");
#else
        connection->SetConnectionString (L"DefaultFileLocation=../../TestData/Sheboygan");
#endif
        CPPUNIT_ASSERT_MESSAGE ("connection state not open", FdoConnectionState_Open == connection->Open ());

        get_spatial_context( connection, L"WGS84 Lat/Long's, Degrees, -180 ==> +180", 2 );

        connection->Close();

    }
    catch (FdoException* ge)
    {
        ge->Release ();
    }

}

void SelectTests::get_spatial_context (FdoIConnection* connection, FdoString* expected_cs_name, int expected_num_sc)
{
    FdoPtr<FdoIGetSpatialContexts> get;
    FdoPtr<FdoISpatialContextReader> reader;
    FdoPtr<FdoFgfGeometryFactory> factory = FdoFgfGeometryFactory::GetInstance ();

    int count;
    
    try
    {
        get = (FdoIGetSpatialContexts*)connection->CreateCommand(FdoCommandType_GetSpatialContexts);
        reader = get->Execute ();
        count = 0;
        while (reader->ReadNext ())
            count++;

		// The flag is ignored...
        get->SetActiveOnly (true);
        reader = get->Execute ();
        count = 0;
		bool found = false;
		FdoStringP  wkt_save;
        FdoString*  cs_save;

        while (reader->ReadNext () && !found)
        {
            FdoStringP  wkt = reader->GetCoordinateSystemWkt ();
            FdoString*  cs = reader->GetCoordinateSystem();

			if ( 0 == wcscmp (expected_cs_name, L"") )
				found = ( 0 == wcscmp( L"", cs ));
			else if ( !found )
				found = ( 0 == wcscmp( expected_cs_name, cs ));

			if ( found )
			{
				wkt_save = wkt;
				cs_save = cs;
			}
			FdoPtr<FdoByteArray>  fgf_box = reader->GetExtent();
			FdoPtr<FdoIGeometry>  geom_box = factory->CreateGeometryFromFgf( fgf_box );
			FdoPtr<FdoIEnvelope>  box = geom_box->GetEnvelope();       

			printf("%d. %ls= (%lf, %lf)(%lf, %lf)\n", count, (const wchar_t*)cs,
						box->GetMinX(), box->GetMinY(), box->GetMaxX(), box->GetMaxY());

            count++;
        }

		if ( !found )
			CPPUNIT_ASSERT_MESSAGE ("Coordinate System name not found", false);

		CPPUNIT_ASSERT_MESSAGE ("Coordinate System name is not set", 0 == wcscmp (expected_cs_name, cs_save));
		
		if ( 0 != wcscmp (expected_cs_name, L"") )
			CPPUNIT_ASSERT_MESSAGE ("Coordinate System WKT is not set", 0 != wcscmp (L"", wkt_save));
		
		CPPUNIT_ASSERT_MESSAGE ("Wrong number of Spatial Contexts", count == expected_num_sc );
    }
    catch (FdoException* ge)
    {
        fail(ge);
    }
    catch (CppUnit::Exception error)
    {
        throw error;
    }
    catch(...)
    {
        CPPUNIT_FAIL ("SelectTests::get_spatial_context() failed");
    }
}

void SelectTests::get_class_definition ()
{
    FdoPtr<FdoISelect> select = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
    select->SetFeatureClassName (L"ontario");
    FdoPtr<FdoIFeatureReader> reader = select->Execute ();
    reader->ReadNext ();
    FdoPtr<FdoClassDefinition> definition = reader->GetClassDefinition ();
    CPPUNIT_ASSERT_MESSAGE ("wrong class type", FdoClassType_FeatureClass == definition->GetClassType ());
    CPPUNIT_ASSERT_MESSAGE ("wrong class name", 0 == wcscmp (L"ontario", definition->GetName ()));
    FdoPtr<FdoClassCapabilities> caps = definition->GetCapabilities ();
    CPPUNIT_ASSERT_MESSAGE ("supports locking?", !caps->SupportsLocking ());
    CPPUNIT_ASSERT_MESSAGE ("supports long transactions?", !caps->SupportsLongTransactions ());
    FdoPtr<FdoDataPropertyDefinitionCollection> identities = definition->GetIdentityProperties ();
    CPPUNIT_ASSERT_MESSAGE ("too many id properties", 1 == identities->GetCount ());
    FdoPtr<FdoDataPropertyDefinition> id = identities->GetItem (0);
    CPPUNIT_ASSERT_MESSAGE ("id property wrong name", 0 == wcscmp (L"FeatId", id->GetName ()));
    CPPUNIT_ASSERT_MESSAGE ("id property wrong type", FdoDataType_Int32 == id->GetDataType ());
    CPPUNIT_ASSERT_MESSAGE ("id property is null", !reader->IsNull (id->GetName ()));
    // ToDo: check regular properties...
    FdoPtr<FdoGeometricPropertyDefinition> geometry = ((FdoFeatureClass*)(definition.p))->GetGeometryProperty ();
    CPPUNIT_ASSERT_MESSAGE ("geometry property wrong name", 0 == wcscmp (L"Geometry", geometry->GetName ()));
    CPPUNIT_ASSERT_MESSAGE ("geometry property wrong types", FdoGeometricType_Surface == geometry->GetGeometryTypes ());
}

void SelectTests::requery ()
{
    try
    {
        FdoPtr<FdoISelect> select = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
        select->SetFeatureClassName (L"lakes");
        FdoPtr<FdoIFeatureReader> reader = select->Execute ();
        int count1 = 0;
        while (reader->ReadNext ())
        {
            count1++;
        }
        reader = select->Execute ();
        int count2 = 0;
        while (reader->ReadNext ())
        {
            count2++;
        }
        CPPUNIT_ASSERT_MESSAGE ("feature counts differ", count1 == count2);
    }
    catch (FdoException* ge)
    {
        fail(ge);
    }
}

void SelectTests::select_subset ()
{
    FdoPtr<FdoISelect> select = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
    select->SetFeatureClassName (L"ontario");
    FdoPtr <FdoIdentifierCollection> ids = select->GetPropertyNames ();
    FdoPtr <FdoIdentifier> id = FdoIdentifier::Create (L"PERIMETER");
    ids->Add (id);
    FdoPtr<FdoIFeatureReader> reader = select->Execute ();
    reader->ReadNext ();
    FdoPtr<FdoClassDefinition> definition = reader->GetClassDefinition ();
    FdoPtr<FdoPropertyDefinitionCollection> properties = definition->GetProperties ();
    CPPUNIT_ASSERT_MESSAGE ("properties has wrong number of values", 1 == properties->GetCount ());  // 2 == the selected property (PERIMITER) + the identity properties (FEATID; they never get filtered out)
    FdoPtr<FdoPropertyDefinition> property = properties->GetItem (L"PERIMETER");
    CPPUNIT_ASSERT_MESSAGE ("property is not a data property", FdoPropertyType_DataProperty == property->GetPropertyType ());
    CPPUNIT_ASSERT_MESSAGE ("property is wrong", 0 == wcscmp (L"PERIMETER", property->GetName ()));
    double d = reader->GetDouble (L"PERIMETER");
    bool bad;
    try
    {
        d= reader->GetDouble (L"AREA");
        bad = true;
    }
    catch (FdoException* ge)
    {
        ge->Release();
        bad = false;
    }
    CPPUNIT_ASSERT_MESSAGE ("non-selected property is accessible", !bad);
}

void SelectTests::select_all ()
{
    double  area;
    double  length;
    int     count = 0;

    FdoPtr<FdoISelect> select = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
    select->SetFeatureClassName (L"ontario");
    FdoPtr <FdoIdentifierCollection> ids = select->GetPropertyNames ();
    FdoPtr<FdoIFeatureReader> reader = select->Execute ();
    while (reader->ReadNext ())
    {
        reader->GetInt32 (L"FeatId");
        area = reader->GetDouble (L"AREA");
        length = reader->GetDouble (L"PERIMETER");
        reader->GetDouble (L"ONTARIO_");
        reader->GetDouble (L"ONTARIO_ID");
        FdoPtr<FdoByteArray> geometry = reader->GetGeometry (L"Geometry");

        AnalyzeGeometry( L"ontario", count++, geometry, length, area);
    }
}




void SelectTests::select_mixed_decimals ()
{
    try
    {
        double  myfloat;
        double  mydouble;
        double  mysmallint;
        double  mylargeint;
        double  myobjectid;
        int count;

        FdoPtr<FdoISelect> select = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
        select->SetFeatureClassName (L"ShpMixedDecimals");
        FdoPtr<FdoIFeatureReader> reader = select->Execute ();
        count = 0;
        while (reader->ReadNext ())
        {
            count++;
            mylargeint = reader->GetDouble (L"MYLARGEINT");
            myfloat = reader->GetDouble (L"MYFLOAT");
            mydouble = reader->GetDouble (L"MYDOUBLE");
            mysmallint = reader->GetDouble (L"MYSMALLINT");
            myobjectid = reader->GetDouble (L"OBJECTID");
            FdoPtr<FdoByteArray> geometry = reader->GetGeometry (L"Geometry");
        }
        CPPUNIT_ASSERT_MESSAGE ("no features selected", 0 != count);
    }
    catch (FdoException *e)
    {
        fail(e);
    }
}

void SelectTests::get_spatial_context_text ()
{
    FdoPtr<FdoIGetSpatialContexts> get;
    FdoPtr<FdoISpatialContextReader> reader;
    
    try
    {
        mConnection->Close ();
        mConnection = ShpTests::GetConnection ();
#ifdef _WIN32
        mConnection->SetConnectionString (L"DefaultFileLocation=..\\..\\TestData\\Oahu");
#else
        mConnection->SetConnectionString (L"DefaultFileLocation=../../TestData/Oahu");
#endif
        CPPUNIT_ASSERT_MESSAGE ("connection state not open", FdoConnectionState_Open == mConnection->Open ());

        get = (FdoIGetSpatialContexts*)mConnection->CreateCommand(FdoCommandType_GetSpatialContexts);
        get->SetActiveOnly (true);
        reader = get->Execute ();
        FdoStringP text = L"PROJCS[\"NAD_1983_UTM_Zone_4N\",GEOGCS[\"GCS_North_American_1983\",DATUM[\"D_North_American_1983\",SPHEROID[\"GRS_1980\",6378137.0,298.257222101]],PRIMEM[\"Greenwich\",0.0],UNIT[\"Degree\",0.0174532925199433]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"False_Easting\",500000.0],PARAMETER[\"False_Northing\",0.0],PARAMETER[\"Central_Meridian\",-159.0],PARAMETER[\"Scale_Factor\",0.9996],PARAMETER[\"Latitude_Of_Origin\",0.0],UNIT[\"Meter\",1.0]]";

		bool		found = false;
		FdoStringP	wkt;
		while (reader->ReadNext () && !found)
        {
            wkt = reader->GetCoordinateSystemWkt ();
			found = ( 0 == wcscmp (wkt, text ) );
        }
		CPPUNIT_ASSERT_MESSAGE ("WKT not found", found );
		CPPUNIT_ASSERT_MESSAGE ("wrong WKT text length", wcslen (text) == wcslen (wkt));
        CPPUNIT_ASSERT_MESSAGE ("wrong WKT text", (0 == wcscmp (wkt, text)) );
    }
    catch (FdoException* ge)
    {
        fail(ge);
    }
    catch (CppUnit::Exception error)
    {
        throw error;
    }
    catch(...)
    {
        CPPUNIT_FAIL ("SelectTests::get_spatial_context_text() failed");
    }
}

void SelectTests::select_only_geometry ()
{
    int count;

    try
    {
        FdoPtr<FdoISelect> select = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
        select->SetFeatureClassName (L"ontario");
        FdoPtr <FdoIdentifierCollection> ids = select->GetPropertyNames ();
        FdoPtr <FdoIdentifier> id = FdoIdentifier::Create (L"Geometry");
        ids->Add (id);
        FdoPtr<FdoIFeatureReader> reader = select->Execute ();
        FdoPtr<FdoClassDefinition> definition = reader->GetClassDefinition ();
        FdoPtr<FdoPropertyDefinitionCollection> properties = definition->GetProperties ();
        count = properties->GetCount ();
        CPPUNIT_ASSERT_MESSAGE ("wrong number of properties", 1 == count);
        FdoPtr<FdoPropertyDefinition> property = properties->GetItem (0);
        CPPUNIT_ASSERT_MESSAGE ("wrong property", wcscmp (L"Geometry", property->GetName ()) == 0);
        count = 0;
        while (reader->ReadNext ())
        {
            count++;
            FdoPtr<FdoByteArray> geometry = reader->GetGeometry (L"Geometry");
            try
            {
                reader->GetInt32 (L"FeatId");
                CPPUNIT_FAIL ("FeatId present");
            }
            catch (FdoException* ge)
            {
                ge->Release ();
            }
        }
        CPPUNIT_ASSERT_MESSAGE ("no ontario features selected", 0 != count);
    }
    catch (FdoException* ge)
    {
        fail(ge);
    }
    catch (CppUnit::Exception error)
    {
        throw error;
    }
    catch(...)
    {
        CPPUNIT_FAIL ("SelectTests::select() failed");
    }

}

static FdoStringP FdoGeometryTypeToString (FdoGeometryType type)
{
    FdoStringP ret;

    switch (type)
    {
        case FdoGeometryType_None:
            ret = L"FdoGeometryType_None";
            break;
        case FdoGeometryType_Point:
            ret = L"FdoGeometryType_Point";
            break;
        case FdoGeometryType_LineString:
            ret = L"FdoGeometryType_LineString";
            break;
        case FdoGeometryType_Polygon:
            ret = L"FdoGeometryType_Polygon";
            break;
        case FdoGeometryType_MultiPoint:
            ret = L"FdoGeometryType_MultiPoint";
            break;
        case FdoGeometryType_MultiLineString:
            ret = L"FdoGeometryType_MultiLineString";
            break;
        case FdoGeometryType_MultiPolygon:
            ret = L"FdoGeometryType_MultiPolygon";
            break;
        case FdoGeometryType_MultiGeometry:
            ret = L"FdoGeometryType_MultiGeometry";
            break;
        case FdoGeometryType_CurveString:
            ret = L"FdoGeometryType_CurveString";
            break;
        case FdoGeometryType_CurvePolygon:
            ret = L"FdoGeometryType_CurvePolygon";
            break;
        case FdoGeometryType_MultiCurveString:
            ret = L"FdoGeometryType_MultiCurveString";
            break;
        case FdoGeometryType_MultiCurvePolygon:
            ret = L"FdoGeometryType_MultiCurvePolygon";
            break;
        default:
            {
                wchar_t buffer[9];

                FdoCommonOSUtil::swprintf (buffer, ELEMENTS(buffer), L"#%d", type);
                ret = buffer;
            }
            break;
    }

    return (ret);
}

void SelectTests::select_hawaii ()
{
    int count;

    try
    {
        mConnection->Close ();
        #ifdef _WIN32
        mConnection->SetConnectionString (L"DefaultFileLocation=..\\..\\TestData\\Hawaii");
        #else
        mConnection->SetConnectionString (L"DefaultFileLocation=../../TestData/Hawaii");
        #endif
        CPPUNIT_ASSERT_MESSAGE ("connection state not open", FdoConnectionState_Open == mConnection->Open ());

        FdoPtr<FdoISelect> select = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
        select->SetFeatureClassName (L"coast_n83");
        FdoPtr<FdoIFeatureReader> reader = select->Execute ();
        FdoPtr<FdoFgfGeometryFactory> factory = FdoFgfGeometryFactory::GetInstance ();
        count = 0;
        while (reader->ReadNext ())
        {
            count++;
            FdoInt32 id = reader->GetInt32 (L"FeatId");
            //FdoPtr<FdoByteArray> bytes = reader->GetGeometry (L"Geometry");
            FdoInt32 byte_count;
            const FdoByte* bytes = reader->GetGeometry (L"Geometry", &byte_count);
            CPPUNIT_ASSERT_MESSAGE ("null bytes", bytes != NULL);
            //FdoPtr<FdoIGeometry> geometry = factory->CreateGeometryFromFgf (bytes);
            FdoPtr<FdoIGeometry> geometry = factory->CreateGeometryFromFgf (bytes, byte_count);
            FdoGeometryType type = geometry->GetDerivedType ();
            if (VERBOSE)
                wprintf (L"\n%d %s", (int)id, (const wchar_t*)FdoGeometryTypeToString (type));
            CPPUNIT_ASSERT_MESSAGE ("not a polygon", FdoGeometryType_Polygon == type);
            FdoIPolygon* poly = (FdoIPolygon*)geometry.p;
            FdoPtr<FdoILinearRing> ring = poly->GetExteriorRing ();
            FdoPtr<FdoIEnvelope> envelope = ring->GetEnvelope ();
            if (VERBOSE)
            {
                wprintf (L" [%g,%g %g,%g]", envelope->GetMinX (), envelope->GetMinY (), envelope->GetMaxX (), envelope->GetMaxY ());
                wprintf (L" %d", poly->GetInteriorRingCount ());
            }
            for (int i = 0; i < poly->GetInteriorRingCount (); i++)
            {
                ring = poly->GetInteriorRing (i);
                envelope = ring->GetEnvelope ();
                if (VERBOSE)
                    wprintf (L" [%g,%g %g,%g]", envelope->GetMinX (), envelope->GetMinY (), envelope->GetMaxX (), envelope->GetMaxY ());
            }
        }
        CPPUNIT_ASSERT_MESSAGE ("no coast_n83 features selected", 0 != count);
    }
    catch (FdoException* ge)
    {
        fail(ge);
    }
    catch (CppUnit::Exception error)
    {
        throw error;
    }
    catch(...)
    {
        CPPUNIT_FAIL ("SelectTests::select_hawaii() failed");
    }

}

void SelectTests::select_states ()
{
    int count;

    try
    {
        mConnection->Close ();
        #ifdef _WIN32
        mConnection->SetConnectionString (L"DefaultFileLocation=..\\..\\TestData\\States");
        #else
        mConnection->SetConnectionString (L"DefaultFileLocation=../../TestData/States");
        #endif
        CPPUNIT_ASSERT_MESSAGE ("connection state not open", FdoConnectionState_Open == mConnection->Open ());


        FdoPtr<FdoISelect> select = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
        select->SetFeatureClassName (L"statesp020");
        FdoPtr<FdoIFeatureReader> reader = select->Execute ();
        FdoPtr<FdoFgfGeometryFactory> factory = FdoFgfGeometryFactory::GetInstance ();
        count = 0;
        while (reader->ReadNext ())
        {
            count++;
            FdoInt32 id = reader->GetInt32 (L"FeatId");
            //FdoPtr<FdoByteArray> bytes = reader->GetGeometry (L"Geometry");
            FdoInt32 byte_count;
            const FdoByte* bytes = reader->GetGeometry (L"Geometry", &byte_count);
            CPPUNIT_ASSERT_MESSAGE ("null bytes", bytes != NULL);
        }
        CPPUNIT_ASSERT_MESSAGE ("no statesp020 features selected", 0 != count);

		CPPUNIT_ASSERT_MESSAGE("This test should not succeed because of duplicated fields", false);
    }
    catch (FdoException* ge)
    {
		FdoStringP   msg = FdoStringP(ge->GetExceptionMessage());

		if ( msg.Contains(L"Duplicate field") ) {
			printf("Expected error '%ls'\n", ge->GetExceptionMessage());
			ge->Release();
		}
		else
			fail(ge);
    }
    catch (CppUnit::Exception error)
    {
        throw error;
    }
    catch(...)
    {
        CPPUNIT_FAIL ("SelectTests::select_hawaii() failed");
    }

}

void SelectTests::select_date_time ()
{
    try
    {
        mConnection->Close ();
#ifdef _WIN32
        mConnection->SetConnectionString (L"DefaultFileLocation=..\\..\\TestData\\Zones");
#else
        mConnection->SetConnectionString (L"DefaultFileLocation=../../TestData/Zones");
#endif
        CPPUNIT_ASSERT_MESSAGE ("connection state not open", FdoConnectionState_Open == mConnection->Open ());
        FdoPtr<FdoISelect> select = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
        select->SetFeatureClassName (L"ZONES");
        FdoPtr<FdoIFeatureReader> reader = select->Execute ();
        long count = 0;
        long nonnull = 0;
        while (reader->ReadNext ())
        {
            if (!reader->IsNull (L"REZONEDATE"))
            {
                FdoDateTime when = reader->GetDateTime (L"REZONEDATE");
                nonnull++;
            }
            count++;
        }
        CPPUNIT_ASSERT_MESSAGE("Wrong record count", 524 == count);
        CPPUNIT_ASSERT_MESSAGE("Wrong non-null record count", 25 == nonnull);
        reader->Close();
    }
    catch (FdoException* e)
    {
        fail(e);
    }
}


void SelectTests::analyze_polygons()
{
    try
    {
        FdoPtr<FdoFgfGeometryFactory> fgf = FdoFgfGeometryFactory::GetInstance();
        FdoPtr<FdoILinearRing> ring;
        FdoPtr<FdoLinearRingCollection> ringsBefore;
        FdoPtr<FdoLinearRingCollection> ringsAfter;


        // directories/filenames/geometrycolumn name:
        wchar_t* testData[][3] =
        {
            { L"../../TestData/Zones",     L"ZONES", L"Geometry" },
            { L"../../TestData/World",     L"world_adm0", L"Geometry" },
            //NO POLYGON DATA: { L"../../TestData/Testing",   L"TestModify", L"Geometry" },
            //DUPLICATE FIELD NAME CAUSES EXCEPTION: { L"../../TestData/States",    L"statesp020", L"Geometry" },
            { L"../../TestData/Sheboygan", L"BuildingOutlines", L"Geometry" },
            { L"../../TestData/Sheboygan", L"CityLimits", L"Geometry" },
            //NO POLYGON DATA: { L"../../TestData/Sheboygan", L"HydrographicLines", L"Geometry" },
            { L"../../TestData/Sheboygan", L"HydrographicPolygons", L"Geometry" },
            { L"../../TestData/Sheboygan", L"Islands", L"Geometry" },
            { L"../../TestData/Sheboygan", L"LandUse", L"Geometry" },
            { L"../../TestData/Sheboygan", L"Parcels", L"Geometry" },
            //NO POLYGON DATA: { L"../../TestData/Sheboygan", L"Rail", L"Geometry" },
            //NO POLYGON DATA: { L"../../TestData/Sheboygan", L"RoadCenterLines", L"Geometry" },
            { L"../../TestData/Sheboygan", L"Soils", L"Geometry" },
            //NO POLYGON DATA: { L"../../TestData/Sheboygan", L"Trees", L"Geometry" },
            { L"../../TestData/Sheboygan", L"VotingDistricts", L"Geometry" },
            { L"../../TestData/Ontario",   L"lakes", L"Geometry" },
            { L"../../TestData/Ontario",   L"ontario", L"Geometry" },
            //NO POLYGON DATA: { L"../../TestData/Ontario",   L"roads", L"Geometry" },
            //NO POLYGON DATA: { L"../../TestData/Ontario",   L"ShpMixedDecimals", L"Geometry" },
            { L"../../TestData/Oahu",      L"coundist01_n83", L"Geometry" },
            //NO POLYGON DATA: { L"../../TestData/Nanboku",   L"Nanboku_3dLine", L"Geometry" },
            { L"../../TestData/Large",     L"States96", L"Geometry" },
            { L"../../TestData/Hawaii",    L"coast_n83", L"Geometry" },
            { L"../../TestData/Florida",   L"plss24", L"Geometry" }
            //NO POLYGON DATA: { L"../../TestData",           L"AcDbARC", L"Geometry" }
        };

        for (int testIndex=0; testIndex<ELEMENTS(testData); testIndex++)
        {
            FdoString* testDir  = testData[testIndex][0];
            FdoString* testFile = testData[testIndex][1];
            FdoString* testCol  = testData[testIndex][2];

            mConnection->Close ();
            mConnection = ShpTests::GetConnection ();
            mConnection->SetConnectionString (FdoStringP::Format(L"DefaultFileLocation=%ls", testDir));
            try
            {
                CPPUNIT_ASSERT_MESSAGE ("connection state not open", FdoConnectionState_Open == mConnection->Open ());
            }
            catch (...)
            {
                continue; // skip this file
            }

            FdoPtr<FdoISelect> select = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
            select->SetFeatureClassName (testFile);
            FdoPtr<FdoIFeatureReader> reader = select->Execute ();
            int rowIndex = 0;
            int iTrueMultiPolygonCount = 0;
            int iPolygonWithMoreThanOneRingCount = 0;
            int iPolygonWithOneRingCount = 0;
            int iPolygonWithOpenRings = 0;
            int iOuterRingsWithIncorrectVertexOrder = 0;
            int iInnerRingsWithIncorrectVertexOrder = 0;
            int iPolygonsWithIncorrectRingOrder = 0;
            while (reader->ReadNext ())
            {
                if (!reader->IsNull (testCol))
                {
                    FdoPtr<FdoByteArray> geomBytes = reader->GetGeometry (testCol);

                    // Convert geomBytes to multipolygon;
                    // This approach should preserve the order of rings in the SHP file and the order of vertices within each ring:
                    FdoPtr<FdoIGeometry> geom = fgf->CreateGeometryFromFgf(geomBytes);
                    FdoPtr<FdoIMultiPolygon> multiPolygon;
                    if (geom->GetDerivedType() == FdoGeometryType_Polygon)
                    {
                        FdoPtr<FdoPolygonCollection> polys = FdoPolygonCollection::Create();
                        polys->Add((FdoIPolygon*)geom.p);
                        try
                        {
                            multiPolygon = fgf->CreateMultiPolygon(polys);
                        }
                        catch (FdoException *e)
                        {
                            // assume that if we get here, its a non-closed ring, although there's a chance that
                            // other exceptions could be thrown during this call:
                            e->Release();
                            iPolygonWithOpenRings++;
                            continue;
                        }
                    }
                    else if (geom->GetDerivedType() == FdoGeometryType_MultiPolygon)
                    {
                        // NOTE: due to ECO 10400, we no longer get true FDO multipolygons when reading from SHP files
                        multiPolygon = FDO_SAFE_ADDREF((FdoIMultiPolygon*)geom.p);
                    }
                    else
                        continue;

                    // Convert MultiPolygon to a flat ring collection:
                    ringsBefore = FdoLinearRingCollection::Create();
                    for (int i=0; i<multiPolygon->GetCount(); i++)
                    {
                        FdoPtr<FdoIPolygon> poly = multiPolygon->GetItem(i);

                        ring = poly->GetExteriorRing();
                        ringsBefore->Add(ring);

                        for (int j=0; j<poly->GetInteriorRingCount(); j++)
                        {
                            ring = poly->GetInteriorRing(j);
                            ringsBefore->Add(ring);
                        }
                    }

                    if (ringsBefore->GetCount() > 1)
                        iPolygonWithMoreThanOneRingCount++;
                    else
                        iPolygonWithOneRingCount++;

                    // Create multipolygon from ring collection;
                    // This operation may reorder/regroup rings (but not reorder vertices within rings):
                    FdoPtr<FdoIGeometry> newGeom = FdoSpatialUtility::CreateGeometryFromRings(ringsBefore, true);
                    if (geom->GetDerivedType() == FdoGeometryType_Polygon)
                    {
                        FdoPtr<FdoPolygonCollection> polys = FdoPolygonCollection::Create();
                        polys->Add((FdoIPolygon*)geom.p);
                        multiPolygon = fgf->CreateMultiPolygon(polys);
                    }
                    else
                    {
                        iTrueMultiPolygonCount++;
                        multiPolygon = FDO_SAFE_ADDREF((FdoIMultiPolygon*)geom.p);
                    }


                    // Convert MultiPolygon to a flat ring collection:
                    ringsAfter = FdoLinearRingCollection::Create();
                    for (int i=0; i<multiPolygon->GetCount(); i++)
                    {
                        FdoPtr<FdoIPolygon> poly = multiPolygon->GetItem(i);

                        ring = poly->GetExteriorRing();
                        ringsAfter->Add(ring);

                        if (FdoSpatialUtility::ComputeLinearRingArea(ring) < 0.0)
                        {
                            iOuterRingsWithIncorrectVertexOrder++;
                            //printf("Outer ring wrong vertex order, in %ls %ls, row %d\n", testDir, testFile, rowIndex);
                        }

                        for (int j=0; j<poly->GetInteriorRingCount(); j++)
                        {
                            ring = poly->GetInteriorRing(j);
                            ringsAfter->Add(ring);

                            if (FdoSpatialUtility::ComputeLinearRingArea(ring) > 0.0)
                            {
                                iInnerRingsWithIncorrectVertexOrder++;
                                //printf("Inner ring wrong vertex order, in %ls %ls, row %d\n", testDir, testFile, rowIndex);
                            }
                        }
                    }


                    // Compare ringsBefore to ringsAfter:
                    FdoPtr<FdoILinearRing> ringBefore;
                    FdoPtr<FdoILinearRing> ringAfter;
                    FdoPtr<FdoIPolygon> polyBefore;
                    FdoPtr<FdoIPolygon> polyAfter;
                    CPPUNIT_ASSERT_MESSAGE("non matching ring count!\n", ringsBefore->GetCount() == ringsAfter->GetCount());
                    FdoInt32 dim;
                    double x1,y1,x2,y2,dummy;
                    bool bIncorrectRingOrder = false;
                    for (int i=0; i<ringsBefore->GetCount(); i++)
                    {
                        ringBefore = ringsBefore->GetItem(i);
                        ringAfter = ringsAfter->GetItem(i);

                        // Compare the two:
                        bool bEqual = (ringBefore->GetCount() == ringAfter->GetCount());
                        for (int j=0; j<ringBefore->GetCount() && bEqual; j++)
                        {
                            ringBefore->GetItemByMembers(j, &x1,&y1,&dummy,&dummy,&dim);
                            ringAfter->GetItemByMembers(j, &x2,&y2,&dummy,&dummy,&dim);
                            bEqual = ((x1==x2) && (y1==y2));
                        }

                        if (!bEqual)
                        {
                            bIncorrectRingOrder = true;
                            //printf("not equal rings in %ls %ls, row %d\n", testDir, testFile, rowIndex);
                        }
                    }

                    if (bIncorrectRingOrder)
                        iPolygonsWithIncorrectRingOrder++;
                }

                rowIndex++;
            }
            reader->Close();

            printf("\n\nShape file %ls/%ls has the following statistics:"
                "\n   %d true FDO multipolygons"
                "\n   %d SHP polygons with 1 ring"
                "\n   %d SHP polygons with 2+ rings"
                "\n   %d SHP polygons with non-closed rings"
                "\n   %d outer rings with wrong vertex order"
                "\n   %d inner rings with wrong vertex order"
                "\n   %d polygons with incorrect ring order",
                testDir, testFile, iTrueMultiPolygonCount, iPolygonWithOneRingCount, iPolygonWithMoreThanOneRingCount, iPolygonWithOpenRings,
                iOuterRingsWithIncorrectVertexOrder, iInnerRingsWithIncorrectVertexOrder, iPolygonsWithIncorrectRingOrder );
        }
    }
    catch (FdoException* e)
    {
        fail(e);
    }
}




void SelectTests::select_large_geometries()
{
    try
    {
        FdoPtr<FdoFgfGeometryFactory> fgf = FdoFgfGeometryFactory::GetInstance();

        // directories/filenames/geometrycolumn name:
        wchar_t* testData[][3] =
        {
            { L"../../TestData/Large",     L"States96", L"Geometry" },
        };
        const int READ_TIMES = 1;

        for (int testIndex=0; testIndex<ELEMENTS(testData); testIndex++)
        {
            FdoString* testDir  = testData[testIndex][0];
            FdoString* testFile = testData[testIndex][1];
            FdoString* testCol  = testData[testIndex][2];

            mConnection->Close ();
            mConnection = ShpTests::GetConnection ();
            mConnection->SetConnectionString (FdoStringP::Format(L"DefaultFileLocation=%ls", testDir));
            try
            {
                CPPUNIT_ASSERT_MESSAGE ("connection state not open", FdoConnectionState_Open == mConnection->Open ());
            }
            catch (...)
            {
                continue; // skip this file
            }

            clock_t start = clock();

            for (int readAmount=0; readAmount<READ_TIMES; readAmount++)
            {
                FdoPtr<FdoISelect> select = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
                select->SetFeatureClassName (testFile);
                FdoPtr<FdoIFeatureReader> reader = select->Execute ();
                int rowIndex = 0;
                while (reader->ReadNext ())
                {
                    if (!reader->IsNull (testCol))
                        FdoPtr<FdoByteArray> geomBytes = reader->GetGeometry (testCol);
                    rowIndex++;
                }
                reader->Close();
            }

            clock_t finish = clock();
            printf("\n\n Elapsed time to read large geometry values: %2.4g", (double) (((double)(finish - start) / (double)CLOCKS_PER_SEC)) / (double)READ_TIMES);
        }
    }
    catch (FdoException* e)
    {
        fail(e);
    }
}
