/*
 * Copyright (C) 2004-2007  Autodesk, Inc.
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
#include "UnitTestUtil.h"
#include "GeometryTests.h"

#define DB_SUFFIX L"_rdbitest"

#define RT_SUFFIX   L"_geometry"  //use "" and pass 'datastore=maptest' in the command line

#define VERBOSE         false

#define     TOLERANCE   6

#define     OP_FEAT_NEW            1
#define     OP_FEAT_UPDATE         3

// Keep the coords sys NULL until FdoRdbms API allows for passing in the CSYS name to dbi_create
#define     NULL_COORDS_SYS       true  // true = "" coord sys,  false = COORD_SYSTEM_NAME for db creation
#define     COORD_SYSTEM_NAME     L"Non-Earth (meters)"

////////  XY test data ///////////////////////////////////////////////////////////////////////
static double  LINE1_POINTS_XY[] = {1.9393939399393, 1.6346373583898, 
                                        2.2837613090393, 2.8353461722290, 
                                        3.3939392827376, 3.8383381288881, 
                                        4.8122343536289, 4.9933737237812, 
                                        5.9393787777727, 5.8383838128881,
                                        6.8383838383899, 6.8388338121117}; 

static double  LINE2_POINTS_XY[] = {10.939393939939, 10.634637358389, 
                                        20.283761309039, 20.835346172229, 
                                        30.393939282737, 30.838338128888, 
                                        40.812234353628, 40.993373723781, 
                                        50.939378777772, 50.838383812888,
                                        60.838383838389, 60.838833812111}; 

// exterior
static double  RING1_POINTS_XY[] = {1.0, 1.1, 
                                        21.0, 1.1, 
                                        21.0, 21.1, 
                                        1.0, 21.1, 
                                        1.0, 1.1};

// interior
static double  RING2_POINTS_XY[] = {9.0, 9.1, 
                                        9.0, 11.1, 
                                        11.0, 11.1, 
                                        11.0, 9.1, 
                                        9.0, 9.1};

// exterior
static double  RING3_POINTS_XY[] = {101.0, 101.1, 
                                        121.0, 101.1, 
                                        121.0, 121.1, 
                                        101.0, 121.1, 
                                        101.0, 101.1};

// interior
static double  RING4_POINTS_XY[] = {109.0, 109.1, 
                                        109.0, 111.1, 
                                        111.0, 111.1, 
                                        111.0, 109.1, 
                                        109.0, 109.1};


////////  XYZ test data ///////////////////////////////////////////////////////////////////////
static double  LINE1_POINTS_XYZ[] = {1.9393939399393, 1.6346373583898, 0.763383737, 
                                        2.2837613090393, 2.8353461722290, 0.923938383, 
                                        3.3939392827376, 3.8383381288881, 0.939399229, 
                                        4.8122343536289, 4.9933737237812, 0.838382761, 
                                        5.9393787777727, 5.8383838128881, 0.939399991,
                                        6.8383838383899, 6.8388338121117, 0.838383888}; 

static double  LINE2_POINTS_XYZ[] = {10.939393939939, 10.634637358389, 7.763383737, 
                                        20.283761309039, 20.835346172229, 7.923938383, 
                                        30.393939282737, 30.838338128888, 7.939399229, 
                                        40.812234353628, 40.993373723781, 7.838382761, 
                                        50.939378777772, 50.838383812888, 7.939399991,
                                        60.838383838389, 60.838833812111, 7.838383888}; 

// exterior
static double  RING1_POINTS_XYZ[] = {1.0, 1.1, 0.1, 
                                        21.0, 1.1, 0.1, 
                                        21.0, 21.1, 0.1, 
                                        1.0, 21.1, 0.1, 
                                        1.0, 1.1, 0.1};

// interior
static double  RING2_POINTS_XYZ[] = {9.0, 9.1, 0.1, 
                                        9.0, 11.1, 0.1, 
                                        11.0, 11.1, 0.1, 
                                        11.0, 9.1, 0.1, 
                                        9.0, 9.1, 0.1};

// exterior
static double  RING3_POINTS_XYZ[] = {101.0, 101.1, 0.1, 
                                        121.0, 101.1, 0.1, 
                                        121.0, 121.1, 0.1, 
                                        101.0, 121.1, 0.1, 
                                        101.0, 101.1, 0.1};

// interior
static double  RING4_POINTS_XYZ[] = {109.0, 109.1, 0.1, 
                                        109.0, 111.1, 0.1, 
                                        111.0, 111.1, 0.1, 
                                        111.0, 109.1, 0.1, 
                                        109.0, 109.1, 0.1};

////////  XYM test data ///////////////////////////////////////////////////////////////////////
static double  LINE1_POINTS_XYM[] = {1.9393939399393, 1.6346373583898, 0.0, 
                                        2.2837613090393, 2.8353461722290, 0.923938383, 
                                        3.3939392827376, 3.8383381288881, 0.939399229, 
                                        4.8122343536289, 4.9933737237812, 0.838382761, 
                                        5.9393787777727, 5.8383838128881, 0.939399991,
                                        6.8383838383899, 6.8388338121117, 0.838383888}; 

static double  LINE2_POINTS_XYM[] = {10.939393939939, 10.634637358389, 0.0, 
                                        20.283761309039, 20.835346172229, 7.923938383, 
                                        30.393939282737, 30.838338128888, 7.939399229, 
                                        40.812234353628, 40.993373723781, 7.838382761, 
                                        50.939378777772, 50.838383812888, 7.939399991,
                                        60.838383838389, 60.838833812111, 7.838383888}; 

// exterior
static double  RING1_POINTS_XYM[] = {1.0, 1.1, 0.1, 
                                        21.0, 1.1, 0.1, 
                                        21.0, 21.1, 0.1, 
                                        1.0, 21.1, 0.1, 
                                        1.0, 1.1, 0.1};

// interior
static double  RING2_POINTS_XYM[] = {9.0, 9.1, 0.1, 
                                        9.0, 11.1, 0.1, 
                                        11.0, 11.1, 0.1, 
                                        11.0, 9.1, 0.1, 
                                        9.0, 9.1, 0.1};

// exterior
static double  RING3_POINTS_XYM[] = {101.0, 101.1, 0.1, 
                                        121.0, 101.1, 0.1, 
                                        121.0, 121.1, 0.1, 
                                        101.0, 121.1, 0.1, 
                                        101.0, 101.1, 0.1};

// interior
static double  RING4_POINTS_XYM[] = {109.0, 109.1, 0.1, 
                                        109.0, 111.1, 0.1, 
                                        111.0, 111.1, 0.1, 
                                        111.0, 109.1, 0.1, 
                                        109.0, 109.1, 0.1};

////////  XYZM test data ///////////////////////////////////////////////////////////////////////
static double  LINE1_POINTS_XYZM[] = {1.9393939399393, 1.6346373583898, 0.763383737, 0.0, 
                                        2.2837613090393, 2.8353461722290, 0.923938383, 0.1, 
                                        3.3939392827376, 3.8383381288881, 0.939399229, 0.2, 
                                        4.8122343536289, 4.9933737237812, 0.838382761, 0.3, 
                                        5.9393787777727, 5.8383838128881, 0.939399991, 0.4,
                                        6.8383838383899, 6.8388338121117, 0.838383888, 0.5}; 

static double  LINE2_POINTS_XYZM[] = {10.939393939939, 10.634637358389, 7.92393838, 0.0, 
                                        20.283761309039, 20.835346172229, 7.923938383, 0.11, 
                                        30.393939282737, 30.838338128888, 7.939399229, 0.22, 
                                        40.812234353628, 40.993373723781, 7.838382761, 0.33, 
                                        50.939378777772, 50.838383812888, 7.939399991, 0.44,
                                        60.838383838389, 60.838833812111, 7.838383888, 0.55}; 

// exterior
static double  RING1_POINTS_XYZM[] = {1.0, 1.1, 0.11, 0.0, 
                                        21.0, 1.1, 0.22, 0.1, 
                                        21.0, 21.1, 0.33, 0.2, 
                                        1.0, 21.1, 0.44, 0.3, 
                                        1.0, 1.1, 0.11, 0.4};

// interior
static double  RING2_POINTS_XYZM[] = {9.0, 9.1, 0.11, 0.0, 
                                        9.0, 11.1, 0.22, 0.1, 
                                        11.0, 11.1, 0.33, 0.2, 
                                        11.0, 9.1, 0.44, 0.3, 
                                        9.0, 9.1, 0.11, 0.4};

// exterior
static double  RING3_POINTS_XYZM[] = {101.0, 101.1, 0.11, 0.0, 
                                        121.0, 101.1, 0.22, 0.1, 
                                        121.0, 121.1, 0.33, 0.2, 
                                        101.0, 121.1, 0.44, 0.3, 
                                        101.0, 101.1, 0.11, 0.1};

// interior
static double  RING4_POINTS_XYZM[] = {109.0, 109.1, 0.11, 0.0, 
                                        109.0, 111.1, 0.22, 0.2, 
                                        111.0, 111.1, 0.33, 0.3, 
                                        111.0, 109.1, 0.44, 0.4, 
                                        109.0, 109.1, 0.11, 0.5};

///////////////////////////////////////////////

static long NUM_POINTS_XY = sizeof(LINE1_POINTS_XY)/(2 * sizeof(double));
static long NUM_ORDINATES_XY = sizeof(LINE1_POINTS_XY)/sizeof(double);

static long NUM_POINTS_XYZ = sizeof(LINE1_POINTS_XYZ)/(3 * sizeof(double));
static long NUM_ORDINATES_XYZ = sizeof(LINE1_POINTS_XYZ)/sizeof(double);

static long NUM_POINTS_XYM = sizeof(LINE1_POINTS_XYM)/(3 * sizeof(double));
static long NUM_ORDINATES_XYM = sizeof(LINE1_POINTS_XYM)/sizeof(double);

static long NUM_POINTS_XYZM = sizeof(LINE1_POINTS_XYZM)/(4 * sizeof(double));
static long NUM_ORDINATES_XYZM = sizeof(LINE1_POINTS_XYZM)/sizeof(double);

// Super test
#define NUM_DIM_CASES 4

static int     FdoDimensionality[NUM_DIM_CASES] = { 
                                                    FdoDimensionality_XY, 
                                                    FdoDimensionality_XY|FdoDimensionality_Z,
                                                    FdoDimensionality_XY|FdoDimensionality_M,
                                                    FdoDimensionality_XY|FdoDimensionality_Z|FdoDimensionality_M
                                                  };

static int     NumPointOrdinates[NUM_DIM_CASES] = { 2, 3, 3, 4 };
static int     NumLINE1_ORDINATES[NUM_DIM_CASES] = {NUM_ORDINATES_XY, NUM_ORDINATES_XYZ, NUM_ORDINATES_XYM, NUM_ORDINATES_XYZM};

static double *LINE1_POINTS[NUM_DIM_CASES] = { LINE1_POINTS_XY, LINE1_POINTS_XYZ, LINE1_POINTS_XYM, LINE1_POINTS_XYZM};
static double *LINE2_POINTS[NUM_DIM_CASES] = { LINE2_POINTS_XY, LINE2_POINTS_XYZ, LINE2_POINTS_XYM, LINE2_POINTS_XYZM};
static double *RING1_POINTS[NUM_DIM_CASES] = { RING1_POINTS_XY, RING1_POINTS_XYZ, RING1_POINTS_XYM, RING1_POINTS_XYZM};
static double *RING2_POINTS[NUM_DIM_CASES] = { RING2_POINTS_XY, RING2_POINTS_XYZ, RING2_POINTS_XYM, RING2_POINTS_XYZM};
static double *RING3_POINTS[NUM_DIM_CASES] = { RING3_POINTS_XY, RING3_POINTS_XYZ, RING3_POINTS_XYM, RING3_POINTS_XYZM};
static double *RING4_POINTS[NUM_DIM_CASES] = { RING4_POINTS_XY, RING4_POINTS_XYZ, RING4_POINTS_XYM, RING4_POINTS_XYZM};




GeometryTests::GeometryTests (void)
{
}

GeometryTests::~GeometryTests (void)
{
}

void GeometryTests::setUp ()
{
    try
    {
        set_provider();

        static bool bDatastoreCreated = false;
        FdoStringP dataStoreName = UnitTestUtil::GetEnviron("datastore", DB_SUFFIX);
        FdoStringP userName = UnitTestUtil::GetEnviron("username");
        FdoStringP userPassword = UnitTestUtil::GetEnviron("password");
        if (!bDatastoreCreated)
        {
	        try
            {
		        FdoStringP userConnectString = UnitTestUtil::GetConnectionString(Connection_NoDatastore);
                FdoPtr<FdoIConnection> connection = UnitTestUtil::GetProviderConnectionObject();
                connection->SetConnectionString( userConnectString );
                connection->Open();
		        FdoPtr<FdoIDestroyDataStore> pDelCmd = (FdoIDestroyDataStore*)connection->CreateCommand( FdoCommandType_DestroyDataStore );
		        FdoPtr<FdoIDataStorePropertyDictionary> dictionary = pDelCmd->GetDataStoreProperties();
		        dictionary->SetProperty( L"DataStore",  dataStoreName);
		        pDelCmd->Execute();
		        connection->Close();
            }
            catch(...) { }

	        UnitTestUtil::CreateDB(false, false, DB_SUFFIX);
            bDatastoreCreated = true;
        }

        CPPUNIT_ASSERT_MESSAGE ("rdbi_initialize failed", RDBI_SUCCESS == do_rdbi_init ());
        try
        {
            CPPUNIT_ASSERT_MESSAGE ("rdbi_connect failed", RDBI_SUCCESS == do_rdbi_connect (dataStoreName, userName, userPassword));
            if ( mRdbiContext->dispatch.capabilities.supports_unicode == 1 )
                CPPUNIT_ASSERT_MESSAGE ("rdbi_set_schemaW failed", RDBI_SUCCESS == rdbi_set_schemaW (mRdbiContext, dataStoreName));
            else
                CPPUNIT_ASSERT_MESSAGE ("rdbi_set_schema failed", RDBI_SUCCESS == rdbi_set_schema (mRdbiContext, dataStoreName));
        }
        catch (CppUnit::Exception exception)
        {
            rdbi_term (&mRdbiContext);
            throw exception;
        }
    }
    catch (CppUnit::Exception exception)
    {
        throw exception;
    }
    catch (...)
    {
        CPPUNIT_FAIL ("unexpected exception encountered");
    }
}

void GeometryTests::tearDown ()
{
    try
    {
        try
        {
            CPPUNIT_ASSERT_MESSAGE ("rdbi_disconnect failed", RDBI_SUCCESS == rdbi_disconnect (mRdbiContext));
        }
        catch (CppUnit::Exception exception)
        {
            rdbi_term (&mRdbiContext);
            throw exception;
        }
        CPPUNIT_ASSERT_MESSAGE ("rdbi_term failed", RDBI_SUCCESS == rdbi_term (&mRdbiContext));
    }
    catch (CppUnit::Exception exception)
    {
        throw exception;
    }
    catch (...)
    {
        CPPUNIT_FAIL ("unexpected exception encountered");
    }
}

int GeometryTests::do_rdbi_init ()
{
    CPPUNIT_FAIL ("naked do_rdbi_init");
    return (0);
}

int GeometryTests::do_rdbi_connect (const char* dataStoreName, const char* userName, const char* userPassword)
{
    CPPUNIT_FAIL ("naked do_rdbi_connect");
    return (0);
}

char *GeometryTests::get_bind_var (int n)
{
    CPPUNIT_FAIL ("naked get_bind_var");
    return ("");
}

char *GeometryTests::get_date_time (const struct tm *when)
{
    CPPUNIT_FAIL ("naked get_date_time");
    return ("");
}

char *GeometryTests::get_geometry_type ()
{
    CPPUNIT_FAIL ("naked get_geometry_type");
    return ("");
}

char *GeometryTests::get_geometry_text (FdoIGeometry *geometry)
{
    CPPUNIT_FAIL ("naked get_geometry");
    return ("");
}

int  GeometryTests::do_insert_geometry( int cursor, FdoInt32 *featId, FdoIGeometry **geometry )
{
    CPPUNIT_FAIL ("naked do_insert_geometry");
    return 0;
}

int GeometryTests::rdbi_sql_Ex( rdbi_context_def *context, int sqlid, FdoStringP sql )
{
    if (context->dispatch.capabilities.supports_unicode == 1)
        return ::rdbi_sqlW( context, sqlid, sql );
    else
        return ::rdbi_sql( context, sqlid, sql );
}

void GeometryTests::ddl ()
{
    int cursor;
    char select[1024];

    cursor = -1;
    try
    {
        CPPUNIT_ASSERT_MESSAGE ("rdbi_est_cursor failed", RDBI_SUCCESS == rdbi_est_cursor (mRdbiContext, &cursor));

        // drop the table if it already exists... ignore errors
        sprintf (select, "drop table foo");
        CPPUNIT_ASSERT_MESSAGE ("rdbi_sql failed", RDBI_SUCCESS == rdbi_sql_Ex (mRdbiContext, cursor, select));
        rdbi_execute (mRdbiContext, cursor, 1, 0);

        sprintf (select, "create table foo (id decimal(10), position %s)", get_geometry_type ());
        CPPUNIT_ASSERT_MESSAGE ("rdbi_sql failed", RDBI_SUCCESS == rdbi_sql_Ex (mRdbiContext, cursor, select));
        CPPUNIT_ASSERT_MESSAGE ("rdbi_execute failed", RDBI_SUCCESS == rdbi_execute (mRdbiContext, cursor, 1, 0));

        sprintf (select, "drop table foo");
        CPPUNIT_ASSERT_MESSAGE ("rdbi_sql failed", RDBI_SUCCESS == rdbi_sql_Ex (mRdbiContext, cursor, select));
        CPPUNIT_ASSERT_MESSAGE ("rdbi_execute failed", RDBI_SUCCESS == rdbi_execute (mRdbiContext, cursor, 1, 0));

        CPPUNIT_ASSERT_MESSAGE ("rdbi_fre_cursor failed", RDBI_SUCCESS == rdbi_fre_cursor (mRdbiContext, cursor));
        cursor = -1;
    }
    catch (CppUnit::Exception exception)
    {
        const char *msg = exception.what ();
        char message[RDBI_MSG_SIZE];
        rdbi_get_msg (mRdbiContext);
#ifdef _WIN32
		WideCharToMultiByte ( CP_THREAD_ACP, 0, mRdbiContext->last_error_msg, (int)wcslen(mRdbiContext->last_error_msg), message, RDBI_MSG_SIZE, NULL,  NULL);
#else
		wcstombs ( message, mRdbiContext->last_error_msg, RDBI_MSG_SIZE);
#endif
        strcat (message, ": ");
        strcat (message, msg);
        if (-1 != cursor)
            rdbi_fre_cursor (mRdbiContext, cursor);
        throw CppUnit::Exception (message);
    }
    catch (...)
    {
        if (-1 != cursor)
            rdbi_fre_cursor (mRdbiContext, cursor);
        CPPUNIT_FAIL ("unexpected exception encountered");
    }
}

void GeometryTests::define ()
{
    int cursor;
    char select[1024];
    FdoPtr<FdoFgfGeometryFactory> factory;
    FdoPtr<FdoIDirectPosition> position;
    FdoPtr<FdoIGeometry> geometry;
    char *p;
    FdoIGeometry*	answer = NULL;
    FdoPtr<FdoIGeometry> panswer;
    char *q;
    int rc;
    int count;
    int rows;
    FdoInt32    featId = 42;

    cursor = -1;
    try
    {
        CPPUNIT_ASSERT_MESSAGE ("rdbi_est_cursor failed", RDBI_SUCCESS == rdbi_est_cursor (mRdbiContext, &cursor));

        // drop the table if it already exists... ignore errors
        sprintf (select, "drop table foo");
        CPPUNIT_ASSERT_MESSAGE ("rdbi_sql failed", RDBI_SUCCESS == rdbi_sql_Ex (mRdbiContext, cursor, select));
        rdbi_execute (mRdbiContext, cursor, 1, 0);

        sprintf (select, "create table foo (id decimal(10), position %s)", get_geometry_type ());
        CPPUNIT_ASSERT_MESSAGE ("rdbi_sql failed", RDBI_SUCCESS == rdbi_sql_Ex (mRdbiContext, cursor, select));
        CPPUNIT_ASSERT_MESSAGE ("rdbi_execute failed", RDBI_SUCCESS == rdbi_execute (mRdbiContext, cursor, 1, 0));

        // insert a row
        factory = FdoFgfGeometryFactory::GetInstance ();
        position = factory->CreatePositionXY (522.5, 763.0);
        geometry = factory->CreatePoint (position);

        p = get_geometry_text (geometry);

        CPPUNIT_ASSERT_MESSAGE ("do_insert_geometry", RDBI_SUCCESS == do_insert_geometry( cursor, &featId, &geometry.p )); 

        sprintf (select, "select position%s from foo where id=%ld", get_geom_conv(), featId);
        if (mRdbiContext->dispatch.capabilities.supports_unicode == 1){
            CPPUNIT_ASSERT_MESSAGE ("rdbi_sql_va failed", RDBI_SUCCESS == rdbi_sql_vaW (mRdbiContext, RDBI_VA_EXEC, cursor,
                FdoStringP(select), RDBI_VA_EOL, RDBI_GEOMETRY, sizeof (answer), &answer, RDBI_VA_EOL));
        }else{
            CPPUNIT_ASSERT_MESSAGE ("rdbi_sql_va failed", RDBI_SUCCESS == rdbi_sql_va (mRdbiContext, RDBI_VA_EXEC, cursor,
                FdoStringP(select), RDBI_VA_EOL, RDBI_GEOMETRY, sizeof (answer), &answer, RDBI_VA_EOL));
        }
        count = 0;
        do
        {
            rc = rdbi_fetch (mRdbiContext, cursor, 1, &rows);
            CPPUNIT_ASSERT_MESSAGE ("rdbi_fetch failed", (RDBI_SUCCESS == rc) || (RDBI_END_OF_FETCH == rc));
            if ((RDBI_END_OF_FETCH != rc) && (0 != rows))
            {
                q = get_geometry_text (answer);
                panswer = FDO_SAFE_ADDREF(answer);
                CPPUNIT_ASSERT_MESSAGE ("fetched value incorrect", 0 == strcmp (p, q));
                delete[] q;
                count++;
            }
        }
        while (RDBI_SUCCESS == rc);
        delete[] p;
        CPPUNIT_ASSERT_MESSAGE ("fetched count incorrect", 1 == count);
        CPPUNIT_ASSERT_MESSAGE ("rdbi_end_select failed", RDBI_SUCCESS == rdbi_end_select (mRdbiContext, cursor));

        sprintf (select, "drop table foo");
        CPPUNIT_ASSERT_MESSAGE ("rdbi_sql failed", RDBI_SUCCESS == rdbi_sql_Ex (mRdbiContext, cursor, select));
        CPPUNIT_ASSERT_MESSAGE ("rdbi_execute failed", RDBI_SUCCESS == rdbi_execute (mRdbiContext, cursor, 1, 0));

        CPPUNIT_ASSERT_MESSAGE ("rdbi_fre_cursor failed", RDBI_SUCCESS == rdbi_fre_cursor (mRdbiContext, cursor));
        cursor = -1;
    }
    catch (CppUnit::Exception exception)
    {
        const char *msg = exception.what ();
        char message[RDBI_MSG_SIZE];
        rdbi_get_msg (mRdbiContext);
#ifdef _WIN32
		WideCharToMultiByte ( CP_THREAD_ACP, 0, mRdbiContext->last_error_msg, (int)wcslen(mRdbiContext->last_error_msg), message, RDBI_MSG_SIZE, NULL,  NULL);
#else
		wcstombs ( message, mRdbiContext->last_error_msg, RDBI_MSG_SIZE);
#endif
        strcat (message, ": ");
        strcat (message, msg);
        if (-1 != cursor)
            rdbi_fre_cursor (mRdbiContext, cursor);
        throw CppUnit::Exception (message);
    }
    catch (...)
    {

        if (-1 != cursor)
            rdbi_fre_cursor (mRdbiContext, cursor);
        CPPUNIT_FAIL ("unexpected exception encountered");
    }
}

void GeometryTests::bind ()
{
    int cursor;
    char select[1024];
    int id;
    FdoPtr<FdoFgfGeometryFactory> factory;
    FdoPtr<FdoIDirectPosition> position;
    FdoPtr<FdoIGeometry> geometry;
    char *p;
    FdoIGeometry *answer = NULL;
    FdoPtr<FdoIGeometry> panswer;
    char *q;
    int rc;
    int count;
    int rows;

    cursor = -1;
    try
    {
        CPPUNIT_ASSERT_MESSAGE ("rdbi_est_cursor failed", RDBI_SUCCESS == rdbi_est_cursor (mRdbiContext, &cursor));

        // drop the table if it already exists... ignore errors
        sprintf (select, "drop table foo");
        CPPUNIT_ASSERT_MESSAGE ("rdbi_sql failed", RDBI_SUCCESS == rdbi_sql_Ex (mRdbiContext, cursor, select));
        rdbi_execute (mRdbiContext, cursor, 1, 0);

        sprintf (select, "create table foo (id decimal(10), position %s)", get_geometry_type ());
        CPPUNIT_ASSERT_MESSAGE ("rdbi_sql failed", RDBI_SUCCESS == rdbi_sql_Ex (mRdbiContext, cursor, select));
        CPPUNIT_ASSERT_MESSAGE ("rdbi_execute failed", RDBI_SUCCESS == rdbi_execute (mRdbiContext, cursor, 1, 0));

        // insert a row using bind
        sprintf (select, "insert into foo values (%s, %s)", get_bind_var (1), get_bind_var (2));
        id = 67;
        factory = FdoFgfGeometryFactory::GetInstance ();
        position = factory->CreatePositionXY (522.5, 763.0);
        geometry = factory->CreatePoint (position);
        p = get_geometry_text (geometry);
        if (mRdbiContext->dispatch.capabilities.supports_unicode == 1){
            CPPUNIT_ASSERT_MESSAGE ("rdbi_sql_va failed", RDBI_SUCCESS == rdbi_sql_vaW (mRdbiContext, RDBI_VA_EXEC, cursor,
                FdoStringP(select), RDBI_INT, sizeof(id), &id, RDBI_GEOMETRY, sizeof (geometry.p), &geometry, RDBI_VA_EOL, RDBI_VA_EOL));
        }else{
            CPPUNIT_ASSERT_MESSAGE ("rdbi_sql_va failed", RDBI_SUCCESS == rdbi_sql_va (mRdbiContext, RDBI_VA_EXEC, cursor,
                FdoStringP(select), RDBI_INT, sizeof(id), &id, RDBI_GEOMETRY, sizeof (geometry.p), &geometry, RDBI_VA_EOL, RDBI_VA_EOL));
        }

        // check it got inserted
        sprintf (select, "select position%s from foo where id=%s", get_geom_conv (), get_bind_var (1));
        if (mRdbiContext->dispatch.capabilities.supports_unicode == 1){
            CPPUNIT_ASSERT_MESSAGE ("rdbi_sql_va failed", RDBI_SUCCESS == rdbi_sql_vaW (mRdbiContext, RDBI_VA_EXEC, cursor,
            FdoStringP(select), RDBI_INT, sizeof(id), &id, RDBI_VA_EOL, RDBI_GEOMETRY, sizeof (answer), &answer, RDBI_VA_EOL));
        }else{
            CPPUNIT_ASSERT_MESSAGE ("rdbi_sql_va failed", RDBI_SUCCESS == rdbi_sql_va (mRdbiContext, RDBI_VA_EXEC, cursor,
            FdoStringP(select), RDBI_INT, sizeof(id), &id, RDBI_VA_EOL, RDBI_GEOMETRY, sizeof (answer), &answer, RDBI_VA_EOL));
        }
        count = 0;
        do
        {
            rc = rdbi_fetch (mRdbiContext, cursor, 1, &rows);
            CPPUNIT_ASSERT_MESSAGE ("rdbi_fetch failed", (RDBI_SUCCESS == rc) || (RDBI_END_OF_FETCH == rc));
            if ((RDBI_END_OF_FETCH != rc) && (0 != rows))
            {
                q = get_geometry_text (answer);
                panswer = FDO_SAFE_ADDREF(answer);
                CPPUNIT_ASSERT_MESSAGE ("fetched value incorrect", 0 == strcmp (p, q));
                delete[] q;
                count++;
            }
        }
        while (RDBI_SUCCESS == rc);
        delete[] p;
        CPPUNIT_ASSERT_MESSAGE ("fetched count incorrect", 1 == count);
        CPPUNIT_ASSERT_MESSAGE ("rdbi_end_select failed", RDBI_SUCCESS == rdbi_end_select (mRdbiContext, cursor));

        sprintf (select, "drop table foo");
        CPPUNIT_ASSERT_MESSAGE ("rdbi_sql failed", RDBI_SUCCESS == rdbi_sql_Ex (mRdbiContext, cursor, select));
        CPPUNIT_ASSERT_MESSAGE ("rdbi_execute failed", RDBI_SUCCESS == rdbi_execute (mRdbiContext, cursor, 1, 0));

        CPPUNIT_ASSERT_MESSAGE ("rdbi_fre_cursor failed", RDBI_SUCCESS == rdbi_fre_cursor (mRdbiContext, cursor));
        cursor = -1;
    }
    catch (CppUnit::Exception exception)
    {
        const char *msg = exception.what ();
        char message[RDBI_MSG_SIZE];
        rdbi_get_msg (mRdbiContext);
#ifdef _WIN32
		WideCharToMultiByte ( CP_THREAD_ACP, 0, mRdbiContext->last_error_msg, (int)wcslen(mRdbiContext->last_error_msg), message, RDBI_MSG_SIZE, NULL,  NULL);
#else
		wcstombs ( message, mRdbiContext->last_error_msg, RDBI_MSG_SIZE);
#endif
        strcat (message, ": ");
        strcat (message, msg);
        if (-1 != cursor)
            rdbi_fre_cursor (mRdbiContext, cursor);
        throw CppUnit::Exception (message);
    }
    catch (...)
    {
        if (-1 != cursor)
            rdbi_fre_cursor (mRdbiContext, cursor);
        CPPUNIT_FAIL ("unexpected exception encountered");
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////
void GeometryTests::RoundTripGeometries()
{
    int i;

    createDb();

    FdoPtr<FdoIGeometryCapabilities> geomCaps = mConn->GetGeometryCapabilities();

    FdoInt32 geomDims = geomCaps->GetDimensionalities();
    int length;
    FdoGeometryType* geomTypes = geomCaps->GetGeometryTypes( length );

    bool supportsPoint = false;
    bool supportsLine = false;
    bool supportsPolygon = false;
    bool supportsCurve = false;
    bool supportsCurvePolygon = false;
    bool supportsMultiPoint = false;
    bool supportsMultiLine = false;
    bool supportsMultiPolygon = false;
    bool supportsMultiCurve = false;
    bool supportsMultiCurvePolygon = false;

    for ( i = 0; i < length; i++ )
    {
        switch ( geomTypes[i] )
        {
        case FdoGeometryType_Point:
            supportsPoint = true;
            break;
        case FdoGeometryType_LineString:
            supportsLine = true;
            break;
        case FdoGeometryType_Polygon:
            supportsPolygon = true;
            break;
        case FdoGeometryType_MultiPoint:
            supportsMultiPoint = true;
            break;
        case FdoGeometryType_MultiLineString:
            supportsMultiLine = true;
            break;
        case FdoGeometryType_MultiPolygon:
            supportsMultiPolygon = true;
            break;
        case FdoGeometryType_CurveString:
            supportsCurve = true;
            break;
        case FdoGeometryType_CurvePolygon:
            supportsCurvePolygon = true;
            break;
        case FdoGeometryType_MultiCurveString:
            supportsMultiCurve = true;
            break;
        case FdoGeometryType_MultiCurvePolygon:
            supportsMultiCurvePolygon = true;
            break;
        }
    }

    // Test each dimensionality case
    for ( i = 0; i < NUM_DIM_CASES; i++ )
    {
        if ( ((i == 1) || (i == 3)) && !(geomDims & FdoDimensionality_Z) )
        {
            continue;
        }

        if ( ((i == 2) || (i == 3)) && !(geomDims & FdoDimensionality_M) )
        {
            continue;
        }

        mFdoDimensionality = FdoDimensionality[i];
        mNumPointOrdinates = NumPointOrdinates[i];
        mLINE1_POINTS = LINE1_POINTS[i];
        mNumLINE1_ORDINATES = NumLINE1_ORDINATES[i];

        mLINE2_POINTS = LINE2_POINTS[i];

        mRING1_POINTS = RING1_POINTS[i];
        mRING2_POINTS = RING2_POINTS[i];
        mRING3_POINTS = RING3_POINTS[i];
        mRING4_POINTS = RING4_POINTS[i];

        if ( supportsPoint ) 
            RoundTripGeometry_Point();
        if ( supportsMultiPoint ) 
            RoundTripGeometry_MultiPoint();

        if ( supportsLine ) 
            RoundTripGeometry_LineString();
        if ( supportsMultiLine ) 
            RoundTripGeometry_MultiLineString();

        if ( supportsCurve ) 
            RoundTripGeometry_CurveString();
        if ( supportsMultiCurve ) 
            RoundTripGeometry_MultiCurveString();

        if ( supportsPolygon ) 
            RoundTripGeometry_Polygon();
        if ( supportsMultiPolygon ) 
        {
            RoundTripGeometry_MultiPolygon();
            if ( i == 1 ) 
                RoundTripGeometry_RectangularMultiPolygon();
        }

        if ( supportsCurvePolygon ) 
            RoundTripGeometry_CurvePolygon();
        if ( supportsMultiCurvePolygon ) 
            RoundTripGeometry_MultiCurvePolygon();
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////
void GeometryTests::RoundTripGeometry_LineString()
{
    try
    {
        connect();
        RoundTripGeometry( FdoGeometryType_LineString );
        disconnect();
    }
    catch (FdoException *ex)
    {
        disconnect();
        CPPUNIT_FAIL (UnitTestUtil::w2a(ex->GetExceptionMessage()));
    }
    catch (...)
    {
        disconnect();
        CPPUNIT_FAIL ("non-FdoException");
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////
void GeometryTests::RoundTripGeometry_MultiLineString()
{
    try
    {
        connect();
        RoundTripGeometry( FdoGeometryType_MultiLineString );
        disconnect();
    }
    catch (FdoException *ex)
    {
        disconnect();
        CPPUNIT_FAIL (UnitTestUtil::w2a(ex->GetExceptionMessage()));
    }
    catch (...)
    {
        disconnect();
        CPPUNIT_FAIL ("non-FdoException");
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////
void GeometryTests::RoundTripGeometry_Point()
{
    try
    {
        connect();
        RoundTripGeometry( FdoGeometryType_Point );
        disconnect();
    }
    catch (FdoException *ex)
    {
        disconnect();
        CPPUNIT_FAIL (UnitTestUtil::w2a(ex->GetExceptionMessage()));
    }
    catch (...)
    {
        disconnect();
        CPPUNIT_FAIL ("non-FdoException");
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////
void GeometryTests::RoundTripGeometry_CurveString()
{
    try
    {
        connect();
        RoundTripGeometry( FdoGeometryType_CurveString );
        disconnect();
    }
    catch (FdoException *ex)
    {
        disconnect();
        CPPUNIT_FAIL (UnitTestUtil::w2a(ex->GetExceptionMessage()));
    }
    catch (...)
    {
        disconnect();
        CPPUNIT_FAIL ("non-FdoException");
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////
void GeometryTests::RoundTripGeometry_MultiCurveString()
{
    try
    {
        connect();
        RoundTripGeometry( FdoGeometryType_MultiCurveString );
        disconnect();
    }
    catch (FdoException *ex)
    {
        disconnect();
        CPPUNIT_FAIL (UnitTestUtil::w2a(ex->GetExceptionMessage()));
    }
    catch (...)
    {
        disconnect();
        CPPUNIT_FAIL ("non-FdoException");
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////
void GeometryTests::RoundTripGeometry_CurvePolygon()
{
    try
    {
        connect();
        RoundTripGeometry( FdoGeometryType_CurvePolygon );
        disconnect();
    }
    catch (FdoException *ex)
    {
        disconnect();
        CPPUNIT_FAIL (UnitTestUtil::w2a(ex->GetExceptionMessage()));
    }
    catch (...)
    {
        disconnect();
        CPPUNIT_FAIL ("non-FdoException");
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////
void GeometryTests::RoundTripGeometry_Polygon()
{
    try
    {
        connect();
        RoundTripGeometry( FdoGeometryType_Polygon );
        disconnect();
    }
    catch (FdoException *ex)
    {
        disconnect();
        CPPUNIT_FAIL (UnitTestUtil::w2a(ex->GetExceptionMessage()));
    }
    catch (...)
    {
        disconnect();
        CPPUNIT_FAIL ("non-FdoException");
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////
void GeometryTests::RoundTripGeometry_MultiPoint()
{
    try
    {
        connect();
        RoundTripGeometry( FdoGeometryType_MultiPoint );
        disconnect();
    }
    catch (FdoException *ex)
    {  
        disconnect();
        CPPUNIT_FAIL (UnitTestUtil::w2a(ex->GetExceptionMessage()));
    }
    catch (...)
    {
        disconnect();
        CPPUNIT_FAIL ("non-FdoException");
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////
void GeometryTests::RoundTripGeometry_MultiPolygon()
{
    try
    {
        connect();
        RoundTripGeometry( FdoGeometryType_MultiPolygon );
        disconnect();
    }
    catch (FdoException *ex)
    {
        disconnect();
        CPPUNIT_FAIL (UnitTestUtil::w2a(ex->GetExceptionMessage()));
    }
    catch (...)
    {
        disconnect();
        CPPUNIT_FAIL ("non-FdoException");
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////
void GeometryTests::RoundTripGeometry_RectangularMultiPolygon()
{
    try
    {
        connect();
        FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
        FdoPtr<FdoIGeometry> geom = gf->CreateGeometry(L"MULTIPOLYGON XYZ ((( 80 170 0 , 85 170 0 , 85 165 0 , 80 165 0 , 80 170 0)),(( 85 165 0 , 85 160 0 , 90 160 0 , 90 165 0 , 85 165 0)))");

        RoundTripGeometry( FdoGeometryType_MultiPolygon, geom );
        disconnect();
    }
    catch (FdoException *ex)
    {
        disconnect();
        CPPUNIT_FAIL (UnitTestUtil::w2a(ex->GetExceptionMessage()));
    }
    catch (...)
    {
        disconnect();
        CPPUNIT_FAIL ("non-FdoException");
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////
void GeometryTests::RoundTripGeometry_MultiCurvePolygon()
{
    try
    {
        connect();
        RoundTripGeometry( FdoGeometryType_MultiCurvePolygon );
        disconnect();
    }
    catch (FdoException *ex)
    {
        disconnect();
        CPPUNIT_FAIL (UnitTestUtil::w2a(ex->GetExceptionMessage()));
    }
    catch (...)
    {
        disconnect();
        CPPUNIT_FAIL ("non-FdoException");
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Validates a geometry: fetch the geometry and compare it against
// the one used for creation
//////////////////////////////////////////////////////////////////////////////////////////////
void GeometryTests::check_geom(long feat_num, FdoByteArray *in_ba)
{
    FdoIGeometry            *in_gba = 0;
    FdoIGeometry            *out_gba = 0;
    FdoByteArray            *out_ba = 0;
    FdoFgfGeometryFactory  *gf = 0;
    bool                    not_found = true;
    int                     err = 0;
    int                     rc = FALSE;

    try
    {
        //FdoPtr<FdoIConnection> connection = UnitTestUtil::GetConnection("", true);

        FdoPtr<FdoIFeatureReader> myReader;
	    FdoPtr<FdoISelect> selCmd;

        // Want to retrieve the geom with exactly the same dimensionality
        gf = FdoFgfGeometryFactory::GetInstance();
        in_gba = gf->CreateGeometryFromFgf(in_ba);

        selCmd = (FdoISelect*)mConn->CreateCommand( FdoCommandType_Select );
        selCmd->SetFeatureClassName(L"GeometryTestSchema:GeometryClass1");
        FdoPtr<FdoIdentifierCollection> names = selCmd->GetPropertyNames();

        FdoPtr<FdoIdentifier> name;
        name = FdoIdentifier::Create(L"Geometry");
        names->Add(name);

        FdoPtr<FdoFilter> filter = FdoComparisonCondition::Create(
	                FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"FeatId") ), 
	                FdoComparisonOperations_EqualTo, 
	                FdoPtr<FdoDataValue>(FdoDataValue::Create((FdoInt64)feat_num) ) ); 

        selCmd->SetFilter(filter);

        myReader = selCmd->Execute();

        if( myReader != NULL && myReader->ReadNext() )
        {
            not_found = false;
            out_ba = myReader->GetGeometry(L"Geometry");
        }

        if (not_found) 
        {
            printf("ERROR: check_geom() - feat %ld not found.\n", feat_num);
            throw 0;
        }
        if ( out_ba == 0 )
        {
            printf("ERROR: check_geom() retrieved NULL geom\n.");
            throw 0;
        }

        // Check results byte by byte
        FdoByte* in_bytes = in_ba->GetData();
        int   in_size = in_ba->GetCount();

        FdoByte* out_bytes = out_ba->GetData();
        int   out_size = out_ba->GetCount();

         if ( in_size != out_size ) 
        {
            printf("ERROR: check_geom() byteArray size mismatch (in: %d out: %d)\n", in_size, out_size);
            throw 0;
        }

        out_gba = gf->CreateGeometryFromFgf(out_ba);

        FdoStringP inWKT = in_gba->GetText();
        FdoStringP outWKT = out_gba->GetText();

        CPPUNIT_ASSERT( inWKT == outWKT );
    } 
    catch (FdoException *ex)
    {
        printf("ERROR in check_geom().\n");
        throw ex;
    }
    catch (...)
    {
        printf("ERROR in check_geom().\n");
        throw;
    }

    if (out_ba != 0 ) out_ba->Release();
    if (in_gba != 0 ) in_gba->Release();
    if (out_gba != 0 ) out_gba->Release();
 
    out_ba = NULL;
    in_gba = NULL;
    out_gba = NULL;
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Get a geometry from db. Then validate it.
//////////////////////////////////////////////////////////////////////////////////////////////
void GeometryTests::fetch_geom(FdoGeometryType geom_type, long feat_num, int num_ords)
{
     try
     {
        long    act_plan = 0; /* TODO mDbiConn->dbi_plan_active_get(); */
        long    act_ver = 0; // mDbiConn->dbi_version_active_get();
        int     geom_ok = FALSE;

        // Create a byte array 
  	    FdoFgfGeometryFactory * gf = FdoFgfGeometryFactory::GetInstance();
        FdoIGeometry           * geom;

        switch (geom_type) {
            case FdoGeometryType_LineString:
                geom = gf->CreateLineString( mFdoDimensionality, num_ords, (double *)mLINE1_POINTS );
                break;
            case FdoGeometryType_Point:
                geom = gf->CreatePoint( mFdoDimensionality, (double *)mLINE1_POINTS);
                break;
            case FdoGeometryType_CurveString:
                {
                    FdoIDirectPosition * start ;
                    FdoIDirectPosition * mid;
                    FdoIDirectPosition * end;
                    FdoICurveSegmentAbstract * seg1;
                    FdoICurveSegmentAbstract * seg2;

                    if ( mFdoDimensionality == FdoDimensionality_XY )
                    {
                        double *ords = mLINE1_POINTS;
                        start = gf->CreatePositionXY(ords[0], ords[1]);
                        mid   = gf->CreatePositionXY(ords[2], ords[3]);
                        end   = gf->CreatePositionXY(ords[4], ords[5]);
                    } 
                    else if ( mFdoDimensionality == (FdoDimensionality_XY|FdoDimensionality_Z) )
                    {
                        double *ords = mLINE1_POINTS;
                        start = gf->CreatePositionXYZ(ords[0], ords[1], ords[2]);
                        mid   = gf->CreatePositionXYZ(ords[3], ords[4], ords[5]);
                        end   = gf->CreatePositionXYZ(ords[6], ords[7], ords[8]);
                    }
                    else if ( mFdoDimensionality == (FdoDimensionality_XY|FdoDimensionality_M) )
                    {
                        double *ords = mLINE1_POINTS;
                        start = gf->CreatePositionXYM(ords[0], ords[1], ords[2]);
                        mid   = gf->CreatePositionXYM(ords[3], ords[4], ords[5]);
                        end   = gf->CreatePositionXYM(ords[6], ords[7], ords[8]);
                    }
                    else if ( mFdoDimensionality == (FdoDimensionality_XY|FdoDimensionality_Z|FdoDimensionality_M) )
                    {
                        double *ords = mLINE1_POINTS;
                        start = gf->CreatePositionXYZM(ords[0], ords[1], ords[2], ords[3]);
                        mid   = gf->CreatePositionXYZM(ords[4], ords[5], ords[6], ords[7]);
                        end   = gf->CreatePositionXYZM(ords[8], ords[9], ords[10], ords[11]);
                    }
                    else
                    {
                        // error
                    }

                    seg1 = gf->CreateCircularArcSegment(start, mid, end);
                    seg2 = gf->CreateLineStringSegment( mFdoDimensionality, 
                                                        num_ords - 4 * mNumPointOrdinates, 
                                                        (double *)&mLINE1_POINTS[ mNumPointOrdinates ]);

                    FdoCurveSegmentCollection * segs = FdoCurveSegmentCollection::Create();
                    segs->Add(seg1);
                    segs->Add(seg2);
                    geom = gf->CreateCurveString(segs);
                    segs->Release();
                }
                break;
            case FdoGeometryType_Polygon:
                {
                    // TBD (never gets here due to version failures).
                }
                break;
            case FdoGeometryType_CurvePolygon:
                {
                    // TBD (never gets here due to version failures).
                }
                break;
            case FdoGeometryType_MultiCurvePolygon:
                {
                    // TBD (never gets here due to version failures).
                }
                break;
            case FdoGeometryType_MultiPoint:
                {
                    // TBD (never gets here due to version failures).
                }
                break;
            default:
                printf("ERROR: fetch_geom() unknown geom_type %d\n", geom_type);
                throw 0;
        }

	    FdoByteArray* byteArray = gf->GetFgf(geom);

        geom->Release();
        gf->Release();
      
        ///////////////////////////////////
        check_geom(feat_num, byteArray);
        //////////////////////////////////

        byteArray->Release();

        geom = NULL;
        gf = NULL;
        byteArray = NULL;

        printf(". fetch/check fn %ld plan %ld vers %ld - SUCCESS\n", feat_num, act_plan, act_ver);
    } 
    catch (FdoException *ex)
    {
        printf("ERROR in fetch_geom().\n");
        throw ex;
    }
    catch (...)
    {
        printf("ERROR in fetch_geom().\n");
        throw;
    }
}

void GeometryTests::createDb ()
{
    try {
        // try to (re)create and connect
        mConn = UnitTestUtil::CreateConnection(true, true, RT_SUFFIX, NULL, NULL, 0, false);

        if ( !NULL_COORDS_SYS )
            updateSpatialContext_0();// L"Non-Earth (meters)"/*COORD_SYSTEM_NAME*/ );

        // Create schema if it isn't there already.
        FdoPtr<FdoIDescribeSchema> pDescCmd = (FdoIDescribeSchema*) mConn->CreateCommand(FdoCommandType_DescribeSchema);
	    FdoPtr<FdoFeatureSchemaCollection> pSchemas = pDescCmd->Execute();
	    FdoPtr<FdoFeatureSchema> pSchema = pSchemas->FindItem( L"GeometryTestSchema" );

        if (pSchema == NULL)
        {
		    FdoPtr<FdoIApplySchema>  pCmd = (FdoIApplySchema*) mConn->CreateCommand(FdoCommandType_ApplySchema);

		    FdoPtr<FdoFeatureSchema> pSchema = FdoFeatureSchema::Create( L"GeometryTestSchema", L"Geometry test schema" );

            FdoPtr<FdoFeatureClass> pGeometryClass1 = FdoFeatureClass::Create( L"GeometryClass1", L"Geometry test class 1" );
		    pGeometryClass1->SetIsAbstract(false);

	        FdoPtr<FdoDataPropertyDefinition> pProp = FdoDataPropertyDefinition::Create( L"FeatId", L"id" );
	        pProp->SetDataType( FdoDataType_Int64 );
	        pProp->SetNullable(false);
            pProp->SetIsAutoGenerated(true);
	        pGeometryClass1->GetProperties()->Add( pProp );
	        pGeometryClass1->GetIdentityProperties()->Add( pProp );

	        FdoPtr<FdoGeometricPropertyDefinition> pGeomProp = FdoGeometricPropertyDefinition::Create( L"Geometry", L"location and shape" );
	        pGeomProp->SetGeometryTypes( FdoGeometricType_Point | FdoGeometricType_Curve | FdoGeometricType_Surface );
            pGeomProp->SetHasElevation( true );
	        pGeometryClass1->GetProperties()->Add( pGeomProp );

	        FdoClassesP(pSchema->GetClasses())->Add( pGeometryClass1 );

            pGeometryClass1 = FdoFeatureClass::Create( L"ObsGeomClass1", L"Geometry test class 1" );
		    pGeometryClass1->SetIsAbstract(false);

	        pProp = FdoDataPropertyDefinition::Create( L"FeatId", L"id" );
	        pProp->SetDataType( FdoDataType_Int64 );
	        pProp->SetNullable(false);
            pProp->SetIsAutoGenerated(true);
	        pGeometryClass1->GetProperties()->Add( pProp );
	        pGeometryClass1->GetIdentityProperties()->Add( pProp );

	        pGeomProp = FdoGeometricPropertyDefinition::Create( L"Geometry2D", L"location and shape" );
	        pGeomProp->SetGeometryTypes( FdoGeometricType_Point | FdoGeometricType_Curve | FdoGeometricType_Surface );
            pGeomProp->SetHasElevation( false );
	        pGeometryClass1->GetProperties()->Add( pGeomProp );

	        pGeomProp = FdoGeometricPropertyDefinition::Create( L"Geometry3D", L"location and shape" );
	        pGeomProp->SetGeometryTypes( FdoGeometricType_Point | FdoGeometricType_Curve | FdoGeometricType_Surface );
            pGeomProp->SetHasElevation( true );
	        pGeometryClass1->GetProperties()->Add( pGeomProp );

	        pGeomProp = FdoGeometricPropertyDefinition::Create( L"NoIndex2D", L"location and shape" );
	        pGeomProp->SetGeometryTypes( FdoGeometricType_Point | FdoGeometricType_Curve | FdoGeometricType_Surface );
            pGeomProp->SetHasElevation( false );
	        pGeometryClass1->GetProperties()->Add( pGeomProp );

	        pGeomProp = FdoGeometricPropertyDefinition::Create( L"NoIndex3D", L"location and shape" );
	        pGeomProp->SetGeometryTypes( FdoGeometricType_Point | FdoGeometricType_Curve | FdoGeometricType_Surface );
            pGeomProp->SetHasElevation( true );
	        pGeometryClass1->GetProperties()->Add( pGeomProp );

	        FdoClassesP(pSchema->GetClasses())->Add( pGeometryClass1 );

            pCmd->SetFeatureSchema( pSchema );
	        pCmd->Execute();
        }
	}
	catch ( FdoException *ex )
	{
        if ( mConn )
        {
            mConn->Close ();
            mConn = NULL;
        }
        // Cannot continue ...
        printf("Failed to create database '%ls'.\n", (FdoString*)UnitTestUtil::GetEnviron("datastore", DB_SUFFIX));
        CPPUNIT_FAIL (UnitTestUtil::w2a(ex->GetExceptionMessage()));
  	}
    catch (...)
    {
        if ( mConn )
        {
      	    mConn->Close();
            mConn = NULL;
        }
        printf("Failed to create database '%ls'.\n", (FdoString*)UnitTestUtil::GetEnviron("datastore", DB_SUFFIX));
        CPPUNIT_FAIL ("non-FdoException");
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////
void GeometryTests::deleteDb ()
{
    try
    {
        UnitTestUtil::DropDb(DB_SUFFIX);
    }
    catch (FdoException *ex)
    {
        CPPUNIT_FAIL (UnitTestUtil::w2a(ex->GetExceptionMessage()));
    }
    catch (...)
    {
        CPPUNIT_FAIL ("non-FdoException");
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////
void GeometryTests::connect ()
{
    try
    {
        // Get connection
        mConn = UnitTestUtil::GetConnection( RT_SUFFIX );
    }
    catch (FdoException *ex)
    {
        if (mConn != NULL )
        {   
            mConn->Close();
            mConn = NULL;
        }
        throw ex;
    }
    catch (...)
    {
      	mConn->Close();
        mConn = NULL;
        throw;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////
void GeometryTests::disconnect ()
{
    try
    {
        if (mConn)
        {
      	    mConn->Close();
            mConn = NULL;
        }
    }
    catch (FdoException *ex)
    {
        throw ex;
    }
    catch (...)
    {
        throw;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Insert a feature with geometry into db. After flush (dbi_feat_close), do validation.
//////////////////////////////////////////////////////////////////////////////////////////////
void GeometryTests::set_geom_feat_fgf(int operation, FdoGeometryType geom_type, int num_ords, long* feat_num, FdoIGeometry * specialGeom )
{
    int                     rc = FALSE;

    FdoIGeometry            *geom;

    try
    {
	    FdoFgfGeometryFactory * gf = FdoFgfGeometryFactory::GetInstance();

        if (NULL != specialGeom)
        {
            geom = FDO_SAFE_ADDREF(specialGeom);
        }
        else
        {
            switch (geom_type) {
                case FdoGeometryType_Point:
                    geom = gf->CreatePoint(mFdoDimensionality, (double *)mLINE1_POINTS);
                    break;
                case FdoGeometryType_MultiPoint:
                    geom = gf->CreateMultiPoint(mFdoDimensionality, num_ords, (double *)mLINE1_POINTS);
                    break;
                case FdoGeometryType_LineString:
                    geom = gf->CreateLineString(mFdoDimensionality, num_ords, (double *)mLINE1_POINTS);
                    break;
                case FdoGeometryType_MultiLineString:
                    {
                        FdoLineStringCollection *lineStrings = FdoLineStringCollection::Create();

                        FdoILineString *lineString = gf->CreateLineString(mFdoDimensionality, num_ords, (double *)mLINE1_POINTS);
                        lineStrings->Add(lineString);
                        lineString->Release();

                        lineString = gf->CreateLineString(mFdoDimensionality, num_ords, (double *)mLINE2_POINTS);
                        lineStrings->Add(lineString);
                        lineString->Release();

                        geom = gf->CreateMultiLineString( lineStrings );
                        lineStrings->Release();
                    }
                    break;
                case FdoGeometryType_CurveString:
                    geom = createCurveString( gf, num_ords, mLINE1_POINTS );
                    break;
                case FdoGeometryType_MultiCurveString:
                    {
                        FdoCurveStringCollection *curveStrings = FdoCurveStringCollection::Create();

                        FdoICurveString *curveString = createCurveString( gf, num_ords, mLINE1_POINTS );
                        curveStrings->Add(curveString);
                        curveString->Release();

                        curveString = createCurveString( gf, num_ords, mLINE2_POINTS );
                        curveStrings->Add(curveString);
                        curveString->Release();

                        geom = gf->CreateMultiCurveString( curveStrings );
                        curveStrings->Release();
                    }
                    break;
                case FdoGeometryType_Polygon:
                    {
                        // Ignore given points and use ones dedicated to polygons.

                        FdoILinearRing* exteriorRing = gf->CreateLinearRing(mFdoDimensionality, mNumPointOrdinates*5, (double *)mRING1_POINTS);

                        FdoILinearRing* interiorRing = gf->CreateLinearRing(mFdoDimensionality, mNumPointOrdinates*5, (double *)mRING2_POINTS);
                        FdoLinearRingCollection * interiorRings = FdoLinearRingCollection::Create();
                        interiorRings->Add(interiorRing);

                        geom = gf->CreatePolygon(exteriorRing, interiorRings);

                        exteriorRing->Release();
                        interiorRing->Release();
                        interiorRings->Release();
                    }
                    break;
                case FdoGeometryType_CurvePolygon:
                    {
                        // Ignore given points and use ones dedicated to polygons.
                        FdoInt32 i;
                        double * ordinates;

                        FdoCurveSegmentCollection *exteriorCSC = FdoCurveSegmentCollection::Create();
                        for (i=0;  i < 4;  i++)
                        {
                            ordinates = (double *)&mRING1_POINTS[0+i*mNumPointOrdinates];
                            FdoILineStringSegment* exteriorLSS = gf->CreateLineStringSegment(mFdoDimensionality, mNumPointOrdinates*2, ordinates);
                            exteriorCSC->Add(exteriorLSS);
                            exteriorLSS->Release();
                        }
                        FdoIRing* exteriorRing = gf->CreateRing(exteriorCSC);

                        FdoCurveSegmentCollection* interiorCSC = FdoCurveSegmentCollection::Create();
                        for (i=0;  i < 4;  i++)
                        {
                            ordinates = (double *)&mRING2_POINTS[0+i*mNumPointOrdinates];
                            FdoILineStringSegment* interiorLSS = gf->CreateLineStringSegment(mFdoDimensionality, mNumPointOrdinates*2, ordinates);
                            interiorCSC->Add(interiorLSS);
                            interiorLSS->Release();
                        }
                        FdoIRing* interiorRing = gf->CreateRing(interiorCSC);
                        FdoRingCollection * interiorRings = FdoRingCollection::Create();
                        interiorRings->Add(interiorRing);

                        geom = gf->CreateCurvePolygon(exteriorRing, interiorRings);

                        exteriorCSC->Release();
                        exteriorRing->Release();
                        interiorCSC->Release();
                        interiorRing->Release();
                        interiorRings->Release();
                    }
                    break;
                case FdoGeometryType_MultiPolygon:
                    {
                        // Ignore given points and use ones dedicated to polygons.
                        FdoPolygonCollection * polygons = FdoPolygonCollection::Create();

                        // 1st polygon...

                        FdoILinearRing* exteriorRing = gf->CreateLinearRing(mFdoDimensionality, 5*mNumPointOrdinates, (double *)mRING1_POINTS);
                        FdoILinearRing* interiorRing = gf->CreateLinearRing(mFdoDimensionality, 5*mNumPointOrdinates, (double *)mRING2_POINTS);
                        FdoLinearRingCollection * interiorRings = FdoLinearRingCollection::Create();
                        interiorRings->Add(interiorRing);

                        FdoIPolygon * polygon = gf->CreatePolygon(exteriorRing, interiorRings);

                        exteriorRing->Release();
                        interiorRing->Release();
                        interiorRings->Release();

                        // Add 1st polygon to the list.
                        polygons->Add(polygon);
                        polygon->Release();

                        // 2nd polygon...

                        exteriorRing = gf->CreateLinearRing(mFdoDimensionality, 5*mNumPointOrdinates, (double *)mRING3_POINTS);
                        interiorRing = gf->CreateLinearRing(mFdoDimensionality, 5*mNumPointOrdinates, (double *)mRING4_POINTS);
                        interiorRings = FdoLinearRingCollection::Create();
                        interiorRings->Add(interiorRing);

                        polygon = gf->CreatePolygon(exteriorRing, interiorRings);

                        exteriorRing->Release();
                        interiorRing->Release();
                        interiorRings->Release();

                        // Add 1st polygon to the list.
                        polygons->Add(polygon);
                        polygon->Release();

                        geom = gf->CreateMultiPolygon(polygons);

                        polygons->Release();
                    }
                    break;

                case FdoGeometryType_MultiCurvePolygon:
                    {
                        // Ignore given points and use ones dedicated to polygons.
                        FdoInt32 i;
                        double * ordinates;

                        FdoCurvePolygonCollection * polygons = FdoCurvePolygonCollection::Create();

                        // 1st polygon...

                        FdoCurveSegmentCollection *exteriorCSC = FdoCurveSegmentCollection::Create();
                        for (i=0;  i < 4;  i++)
                        {
                            ordinates = (double *)&mRING1_POINTS[0+i*mNumPointOrdinates];
                            FdoILineStringSegment* exteriorLSS = gf->CreateLineStringSegment(mFdoDimensionality, 2*mNumPointOrdinates, ordinates);
                            exteriorCSC->Add(exteriorLSS);
                            exteriorLSS->Release();
                        }
                        FdoIRing* exteriorRing = gf->CreateRing(exteriorCSC);

                        FdoCurveSegmentCollection* interiorCSC = FdoCurveSegmentCollection::Create();
                        for (i=0;  i < 4;  i++)
                        {
                            ordinates = (double *)&mRING2_POINTS[0+i*mNumPointOrdinates];
                            FdoILineStringSegment* interiorLSS = gf->CreateLineStringSegment(mFdoDimensionality, 2*mNumPointOrdinates, ordinates);
                            interiorCSC->Add(interiorLSS);
                            interiorLSS->Release();
                        }
                        FdoIRing* interiorRing = gf->CreateRing(interiorCSC);
                        FdoRingCollection * interiorRings = FdoRingCollection::Create();
                        interiorRings->Add(interiorRing);

                        FdoICurvePolygon * polygon = gf->CreateCurvePolygon(exteriorRing, interiorRings);

                        exteriorCSC->Release();
                        exteriorRing->Release();
                        interiorCSC->Release();
                        interiorRing->Release();
                        interiorRings->Release();

                        // Add 1st polygon to the list.
                        polygons->Add(polygon);
                        polygon->Release();

                        // 2nd polygon ...

                        exteriorCSC = FdoCurveSegmentCollection::Create();
                        for (i=0;  i < 4;  i++)
                        {
                            ordinates = (double *)&mRING3_POINTS[0+i*mNumPointOrdinates];
                            FdoILineStringSegment* exteriorLSS = gf->CreateLineStringSegment(mFdoDimensionality, 2*mNumPointOrdinates, ordinates);
                            exteriorCSC->Add(exteriorLSS);
                            exteriorLSS->Release();
                        }
                        exteriorRing = gf->CreateRing(exteriorCSC);

                        interiorCSC = FdoCurveSegmentCollection::Create();
                        for (i=0;  i < 4;  i++)
                        {
                            ordinates = (double *)&mRING4_POINTS[0+i*mNumPointOrdinates];
                            FdoILineStringSegment* interiorLSS = gf->CreateLineStringSegment(mFdoDimensionality, 2*mNumPointOrdinates, ordinates);
                            interiorCSC->Add(interiorLSS);
                            interiorLSS->Release();
                        }
                        interiorRing = gf->CreateRing(interiorCSC);
                        interiorRings = FdoRingCollection::Create();
                        interiorRings->Add(interiorRing);

                        polygon = gf->CreateCurvePolygon(exteriorRing, interiorRings);

                        exteriorCSC->Release();
                        exteriorRing->Release();
                        interiorCSC->Release();
                        interiorRing->Release();
                        interiorRings->Release();

                        // Add 2nd polygon to the list.
                        polygons->Add(polygon);
                        polygon->Release();

                        geom = gf->CreateMultiCurvePolygon(polygons);

                        polygons->Release();
                    }
                    break;
                default:
                    printf("ERROR: unknown geom_type %d.\n", geom_type);
                    throw 0;
            }
        }
        FdoByteArray* byteArray = gf->GetFgf(geom);

        gf->Release();
        gf = NULL;

        // Create a new feature 
        if ( operation == OP_FEAT_NEW )
        {
            *feat_num = 0;

            FdoIInsert *insertCommand = (FdoIInsert *) mConn->CreateCommand(FdoCommandType_Insert);
            insertCommand->SetFeatureClassName(L"GeometryClass1");
	        FdoPtr<FdoPropertyValueCollection> propertyValues = insertCommand->GetPropertyValues();
            FdoPtr<FdoPropertyValue> propertyValue =  FdoPropertyValue::Create();
            propertyValue->SetName( L"Geometry" );
            propertyValues->Add( propertyValue );
            FdoPtr<FdoGeometryValue> geometryValue = FdoGeometryValue::Create(byteArray);
	        propertyValue->SetValue(geometryValue);
            FdoPtr<FdoIFeatureReader> reader = insertCommand->Execute();

            if( reader != NULL && reader->ReadNext() )
            {
                if( ! reader->IsNull( L"FeatId" ) )
                {
                    *feat_num = (long)reader->GetInt64(L"FeatId");
                }
            }
        }
        else if ( operation == OP_FEAT_UPDATE ) 
        {
            FdoPtr<FdoIUpdate> updateCommand = (FdoIUpdate *) mConn->CreateCommand(FdoCommandType_Update);
            updateCommand->SetFeatureClassName(L"GeometryTestSchema:GeometryClass1");
	        FdoPtr<FdoFilter> filter = FdoComparisonCondition::Create(
		                FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"FeatId") ), 
		                FdoComparisonOperations_EqualTo, 
		                FdoPtr<FdoDataValue>(FdoDataValue::Create((FdoInt64)*feat_num) ) ); 

	        updateCommand->SetFilter(filter);

            FdoPtr<FdoPropertyValueCollection> propertyValues = updateCommand->GetPropertyValues();
            FdoPtr<FdoPropertyValue> propertyValue =  FdoPropertyValue::Create();
            propertyValue->SetName( L"Geometry" );
            propertyValues->Add( propertyValue );
            FdoPtr<FdoGeometryValue> geometryValue = FdoGeometryValue::Create(byteArray);
	        propertyValue->SetValue(geometryValue);

            FdoInt32 numUpdated = updateCommand->Execute();
			CPPUNIT_ASSERT_MESSAGE("Wrong count of updated features", 1 == numUpdated);
        } 
        else 
        {
            printf("ERROR: unknown operation %d\n", operation);
            throw 0;
        }

        // Check results 
        check_geom( *feat_num, byteArray);
      
        geom->Release();
        geom = NULL;

        byteArray->Release();
        byteArray = NULL;
    }
    catch (FdoException *ex)
    {
      	printf("ERROR in set_geom_feat_fgf().\n");
        throw ex;
    }
    catch (...)
    {
      	printf("ERROR in set_geom_feat_fgf().\n");
        throw;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////
void GeometryTests::create_feat_fgf( FdoGeometryType geom_type, int num_ords, long *feat_num, FdoIGeometry * specialGeom )
{
    try
    {
        set_geom_feat_fgf( OP_FEAT_NEW, geom_type, num_ords, feat_num, specialGeom );

        if ( VERBOSE )
            printf(". create fn %ld - SUCCESS\n", *feat_num);
    }
    catch (FdoException *ex)
    {
      	printf("ERROR in create_feat_fgf().\n");
        throw ex;
    }
    catch (...)
    {
      	printf("ERROR in create_feat_fgf().\n");
        throw;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////
void GeometryTests::update_feat_fgf(FdoGeometryType geom_type, long feat_num, int num_ords, FdoIGeometry * specialGeom )
{
    long    in_feat_num = feat_num;
    int     geom_ok = FALSE;

    try
    {
        set_geom_feat_fgf(OP_FEAT_UPDATE, geom_type, num_ords, &in_feat_num, specialGeom );

        printf(". update fn %ld SUCCESS\n", feat_num );
    }
    catch (FdoException *ex)
    {
      	printf("ERROR in update_feat_fgf().\n");
        throw ex;
    }
    catch (...)
    {
      	printf("ERROR in update_feat_fgf().\n");
        throw;
    }

}

//////////////////////////////////////////////////////////////////////////////////////////////
void GeometryTests::RoundTripGeometry( FdoGeometryType geomType, FdoIGeometry * specialGeom )
{
    int                  errCode      = 0;

    printf("--- %s\n", geometryType2char(geomType) );

    try
    { 
        create_feat_fgf( geomType, mNumLINE1_ORDINATES - mNumPointOrdinates, &mFeatNum1, specialGeom );
 
        update_feat_fgf( geomType, mFeatNum1, mNumLINE1_ORDINATES, specialGeom );
    } 
    catch (FdoException *ex)
    {
      	printf("ERROR in RoundTripGeometry().\n");
        throw ex;
    }
    catch (...)
    {
      	printf("ERROR in RoundTripGeometry().\n");
        throw;
    }
}

void    GeometryTests::updateSpatialContext_0()
{
    FdoICreateSpatialContext *cscCmd = 0;

    try
    {
        cscCmd = (FdoICreateSpatialContext *)mConn->CreateCommand( FdoCommandType_CreateSpatialContext );

        cscCmd->SetName(L"Default"); 
        cscCmd->SetCoordinateSystem( COORD_SYSTEM_NAME );

        FdoFgfGeometryFactory * gf = FdoFgfGeometryFactory::GetInstance();
        FdoIEnvelope *   env = gf->CreateEnvelopeXY(-100000, -100000, 100000, 100000);

        FdoIGeometry * geom = gf->CreateGeometry(env); 
        cscCmd->SetExtent( gf->GetFgf( geom ) );
        geom->Release();
        env->Release();
        gf->Release();

        cscCmd->SetXYTolerance(.02);
        cscCmd->SetZTolerance(.001);

        // This will try to update rather than creating a new SC
        cscCmd->SetUpdateExisting(true);

        cscCmd->Execute();
    }
    catch (FdoException *ex )
    {
 	    if ( cscCmd != 0 )
            cscCmd->Release();

        printf("ERROR in updateSpatialContext_0: Failed to update 'Default'.\n");
        throw ex;
    }

 	if ( cscCmd != 0 )
        cscCmd->Release();
}

///////////////////////////////////////////////////////////////////////////////////////////
FdoICurveString * GeometryTests::createCurveString( FdoFgfGeometryFactory * gf, int num_ords, double * line_points )
{
    FdoIDirectPosition * start;
    FdoIDirectPosition * mid;
    FdoIDirectPosition * end;

    if ( mFdoDimensionality == FdoDimensionality_XY )
    {
        start = gf->CreatePositionXY(line_points[0], line_points[1]);
        mid   = gf->CreatePositionXY(line_points[2], line_points[3]);
        end   = gf->CreatePositionXY(line_points[4], line_points[5]);
    }
    else if ( mFdoDimensionality == (FdoDimensionality_XY|FdoDimensionality_Z) )
    {
        start = gf->CreatePositionXYZ(line_points[0], line_points[1], line_points[3]);
        mid   = gf->CreatePositionXYZ(line_points[3], line_points[4], line_points[5]);
        end   = gf->CreatePositionXYZ(line_points[6], line_points[7], line_points[8]);
    } 
    else if ( mFdoDimensionality == (FdoDimensionality_XY|FdoDimensionality_M) )
    {
        start = gf->CreatePositionXYM(line_points[0], line_points[1], line_points[3]);
        mid   = gf->CreatePositionXYM(line_points[3], line_points[4], line_points[5]);
        end   = gf->CreatePositionXYM(line_points[6], line_points[7], line_points[8]);
    } 
    else if ( mFdoDimensionality == (FdoDimensionality_XY|FdoDimensionality_Z|FdoDimensionality_M) )
    {
        start = gf->CreatePositionXYZM(line_points[0], line_points[1], line_points[2], line_points[3]);
        mid   = gf->CreatePositionXYZM(line_points[4], line_points[5], line_points[6], line_points[7]);
        end   = gf->CreatePositionXYZM(line_points[8], line_points[9], line_points[10], line_points[11]);
    } 
    else
    {
        // Error
    }

	// Add arcsegment in segments collection
    FdoICurveSegmentAbstract * seg = gf->CreateCircularArcSegment(start, mid, end);
	FdoCurveSegmentCollection* segs = FdoCurveSegmentCollection::Create();
	segs->Add(seg);
	seg->Release();

    start->Release();
    mid->Release();
    end->Release();
    start = mid = end = 0;

    FdoInt32 numOrdsLeft = num_ords - (2 * mNumPointOrdinates);
    FdoInt32 numPositions = numOrdsLeft / mNumPointOrdinates;
  
 	// Create a single LineStringSegment having numPositions
    double * ordinates; // offset due Arc above

    if ( mFdoDimensionality == FdoDimensionality_XY )
    {
        ordinates = &line_points[4]; // offset due Arc above
    }
    else if ( mFdoDimensionality == (FdoDimensionality_XY|FdoDimensionality_Z) )
    {
        ordinates = &line_points[6]; 
    }
    else if ( mFdoDimensionality == (FdoDimensionality_XY|FdoDimensionality_M) )
    {
        ordinates = &line_points[6]; 
    }
    else if ( mFdoDimensionality == (FdoDimensionality_XY|FdoDimensionality_Z|FdoDimensionality_M) )
    {
        ordinates = &line_points[8]; 
    }
    else
    {
        // Error
    }

	seg = gf->CreateLineStringSegment(mFdoDimensionality, numPositions*mNumPointOrdinates, ordinates);

	// Add it to segments collection
	segs->Add(seg);
	seg->Release();
	seg = NULL;
    
	// Create a curvestring from curvesegments
    FdoICurveString * curveString = gf->CreateCurveString(segs);
    segs->Release();
    segs = 0;

    return curveString;
}

char *GeometryTests::geometryType2char( FdoGeometryType geomType )
{
    static  char geom[40];
    static  char geom_dim[15];
    static  char geom_type[55];

    switch (geomType)
    {
        case FdoGeometryType_Point:
            strcpy(geom, "FdoGeometryType_Point");
            break;
        case FdoGeometryType_MultiPoint:
            strcpy(geom, "FdoGeometryType_MultiPoint");
            break;
        case FdoGeometryType_LineString:
            strcpy(geom, "FdoGeometryType_LineString");
            break;
        case FdoGeometryType_MultiLineString:
            strcpy(geom, "FdoGeometryType_MultiLineString");
            break;
        case FdoGeometryType_CurveString:
            strcpy(geom, "FdoGeometryType_CurveString");
            break;
        case FdoGeometryType_MultiCurveString:
            strcpy(geom, "FdoGeometryType_MultiCurveString");
            break;
        case FdoGeometryType_Polygon:
            strcpy(geom, "FdoGeometryType_Polygon");
            break;
        case FdoGeometryType_MultiPolygon:
            strcpy(geom, "FdoGeometryType_MultiPolygon");
            break;
        case FdoGeometryType_CurvePolygon:
            strcpy(geom, "FdoGeometryType_CurvePolygon");
            break;
        case FdoGeometryType_MultiCurvePolygon:
            strcpy(geom, "FdoGeometryType_MultiCurvePolygon");
            break;
        default:
            strcpy(geom, "INVALID TYPE");
    }
    
    switch (mFdoDimensionality)
    {
        case FdoDimensionality_XY:
            strcpy(geom_dim, "XY");
            break;
        case FdoDimensionality_XY | FdoDimensionality_Z:
            strcpy(geom_dim, "XYZ");
            break;
        case FdoDimensionality_XY | FdoDimensionality_M:
            strcpy(geom_dim, "XYM");
            break;
        case FdoDimensionality_XY | FdoDimensionality_Z | FdoDimensionality_M:
            strcpy(geom_dim, "XYZM");
            break;
        default:
            strcpy(geom, "INVALID DIM");
    }
    sprintf(geom_type, "%s: %s", geom_dim, geom );

    return geom_type;
}



