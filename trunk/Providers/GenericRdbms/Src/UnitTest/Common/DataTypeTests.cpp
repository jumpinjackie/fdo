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
#include "DataTypeTests.h"
#include "UnitTestUtil.h"

DataTypeTests::DataTypeTests (void)
{
}

DataTypeTests::~DataTypeTests (void)
{
}

void DataTypeTests::setUp ()
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

void DataTypeTests::tearDown ()
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

int DataTypeTests::do_rdbi_init ()
{
    CPPUNIT_FAIL ("naked do_rdbi_init");
    return (0);
}

int DataTypeTests::do_rdbi_connect (const char* dataStoreName, const char* userName, const char* userPassword)
{
    CPPUNIT_FAIL ("naked do_rdbi_connect");
    return (0);
}

char *DataTypeTests::get_bind_var (int n, int rdbiType)
{
    CPPUNIT_FAIL ("naked get_bind_var");
    return ("");
}

char *DataTypeTests::get_date_time (const struct tm *when)
{
    CPPUNIT_FAIL ("naked get_date_time");
    return ("");
}

int DataTypeTests::rdbi_sql_Ex( rdbi_context_def *context, int sqlid, FdoStringP sql )
{
    if (context->dispatch.capabilities.supports_unicode == 1)
        return ::rdbi_sqlW( context, sqlid, sql );
    else
        return ::rdbi_sql( context, sqlid, sql );
}

int DataTypeTests::rdbi_desc_slct_Ex( rdbi_context_def *context, int sqlid, int pos, int name_len, char *name, int *rdbi_type, int *binary_size, int *null_ok )
{
    if (context->dispatch.capabilities.supports_unicode == 1)
    {
        wchar_t wname[1024];
        wname[0] = L'\0';
        int rc = ::rdbi_desc_slctW( context, sqlid, pos, name_len, wname, rdbi_type, binary_size, null_ok );
        strcpy (name, FdoStringP(wname));
        return rc;
    }
    else
        return ::rdbi_desc_slct( context, sqlid, pos, name_len, name, rdbi_type, binary_size, null_ok );
}

void DataTypeTests::roundtrip_insert (
    char *sql_type,
    int rdbi_type,
    int rdbi_scale, // unused
    int rdbi_precision, // unused
    void *variable,
    int size,
    double tolerance,
    bool is_ora_int64_test,
    int out_rdbi_type)
{
    int cursor;
    char statement[1024];
    int type;
    int bytes;
    int null_ok;
    void *data1;
    void *data2;
    long null_ind1 = 0;
    long null_ind2 = 0;
    int rows;
	long *variable2 = 0; // NOTE: for geometries, we need null data.
    int desc_rdbi_type = (out_rdbi_type == -1) ? rdbi_type : out_rdbi_type;

    cursor = -1;
    try
    {
        CPPUNIT_ASSERT_MESSAGE ("rdbi_est_cursor failed", RDBI_SUCCESS == rdbi_est_cursor (mRdbiContext, &cursor));

        // drop the table if it already exists... ignore errors
        sprintf (statement, "drop table bar");
        CPPUNIT_ASSERT_MESSAGE ("rdbi_sql failed", RDBI_SUCCESS == rdbi_sql_Ex (mRdbiContext, cursor, statement));
		rdbi_execute (mRdbiContext, cursor, 1, 0);

        // create the table with 2 columns (not null/null)
        sprintf (statement, "create table bar (xyz1 %s, xyz2 %s null ) %s", sql_type, sql_type, get_geometry_storage());
        CPPUNIT_ASSERT_MESSAGE ("rdbi_sql failed", RDBI_SUCCESS == rdbi_sql_Ex (mRdbiContext, cursor, statement));
        CPPUNIT_ASSERT_MESSAGE ("rdbi_execute failed", RDBI_SUCCESS == rdbi_execute (mRdbiContext, cursor, 1, 0));		

        // insert a row with the data
        sprintf (statement, "insert into bar values (%s, %s)", get_bind_var (1,rdbi_type), get_bind_var (2,rdbi_type));

        // Set the null indicators
        (void) rdbi_set_nnull (mRdbiContext, (void *)&null_ind1, 0, 0 );
        (void) rdbi_set_null  (mRdbiContext, (void *)&null_ind2, 0, 0 );

        if (mRdbiContext->dispatch.capabilities.supports_unicode == 1){
            CPPUNIT_ASSERT_MESSAGE ("rdbi_sql_va failed", RDBI_SUCCESS == rdbi_sql_vaW (mRdbiContext,
                RDBI_VA_BNDNULLS|RDBI_VA_EXEC, cursor, FdoStringP(statement), rdbi_type, size, variable, &null_ind1,
                rdbi_type, size, &variable2, &null_ind2, RDBI_VA_EOL, RDBI_VA_EOL));
        }else{
            CPPUNIT_ASSERT_MESSAGE ("rdbi_sql_va failed", RDBI_SUCCESS == rdbi_sql_va (mRdbiContext,
                RDBI_VA_BNDNULLS|RDBI_VA_EXEC, cursor, FdoStringP(statement), rdbi_type, size, variable, &null_ind1,
                rdbi_type, size, &variable2, &null_ind2, RDBI_VA_EOL, RDBI_VA_EOL));
        }

        // describe the select
        sprintf (statement, get_select_statement(rdbi_type));

        CPPUNIT_ASSERT_MESSAGE ("rdbi_sql failed", RDBI_SUCCESS == rdbi_sql_Ex (mRdbiContext, cursor, statement));
        CPPUNIT_ASSERT_MESSAGE ("rdbi_desc_slct failed", RDBI_SUCCESS == rdbi_desc_slct_Ex (mRdbiContext, cursor, 1, sizeof (statement), statement, &type, &bytes, &null_ok));
		CPPUNIT_ASSERT_MESSAGE ("column wrong name", 0 == FdoCommonOSUtil::stricmp ("xyz1", statement));
        CPPUNIT_ASSERT_MESSAGE ("column wrong type", is_datatype_equal(desc_rdbi_type, type) );
        // CPPUNIT_ASSERT_MESSAGE ("column wrong size", rdbi_scale == bytes);
        // CPPUNIT_ASSERT_MESSAGE ("column not nullable", 0 != null_ok);

        // define the result variables
        if (is_ora_int64_test)
            bytes = size;

        data1 = alloca (bytes);
        memset( data1, '\0', bytes);//size
		void *addr1 = ( rdbi_type == RDBI_GEOMETRY )? &data1 : data1;
        CPPUNIT_ASSERT_MESSAGE ("rdbi_define failed", RDBI_SUCCESS == rdbi_define (mRdbiContext, cursor, "1", rdbi_type, bytes, (char*)addr1, (char*)&null_ind1));

        data2 = alloca (bytes);
        memset( data2, '\0', bytes);//size
		void *addr2 = ( rdbi_type == RDBI_GEOMETRY )? &data2 : data2;
        CPPUNIT_ASSERT_MESSAGE ("rdbi_define failed", RDBI_SUCCESS == rdbi_define (mRdbiContext, cursor, "2", rdbi_type, bytes, (char*)addr2, (char*)&null_ind2));

        // execute, fetch and check the values
        CPPUNIT_ASSERT_MESSAGE ("rdbi_execute failed", RDBI_SUCCESS == rdbi_execute (mRdbiContext, cursor, 1, 0));
        CPPUNIT_ASSERT_MESSAGE ("rdbi_fetch failed", RDBI_SUCCESS == rdbi_fetch (mRdbiContext, cursor, 1, &rows));
        CPPUNIT_ASSERT_MESSAGE ("wrong row count", 1 == rows);
        CPPUNIT_ASSERT_MESSAGE ("value is null", !rdbi_is_null(mRdbiContext, &null_ind1, 0));
        CPPUNIT_ASSERT_MESSAGE ("value is not null", rdbi_is_null(mRdbiContext, &null_ind2, 0));

        if (0.0 != tolerance) {
            double tol;
            if ( rdbi_type == RDBI_FLOAT )
                tol = fabs (*((float*)variable) - *((float*)data1));
            else
                tol = fabs (*((double*)variable) - *((double*)data1));

            CPPUNIT_ASSERT_MESSAGE ("column is wrong value", tolerance > tol);
        } 
		else if ( rdbi_type != RDBI_GEOMETRY )
		{
            CPPUNIT_ASSERT_MESSAGE ("column is wrong value", 0 == memcmp (variable, data1, (bytes < size) ? bytes:size));//size
        } 
		else if ( rdbi_type == RDBI_GEOMETRY ) {
			FdoIGeometry** pg = (FdoIGeometry**)(variable);
			FdoIPoint *     g = (FdoIPoint *) (*pg);
			FdoPtr<FdoIDirectPosition> p = g->GetPosition();
			double x = p->GetX();
			double y = p->GetY();

			FdoIGeometry** pg1 = (FdoIGeometry**)(addr1);
			FdoIPoint* g1 = (FdoIPoint *) (*pg1);
			FdoPtr<FdoIDirectPosition> p1 = g1->GetPosition();
			double x1 = p1->GetX();
			double y1 = p1->GetY();
			
			CPPUNIT_ASSERT_MESSAGE ("point X is wrong value", fabs(x1-x) < 0.0001);
			CPPUNIT_ASSERT_MESSAGE ("point Y is wrong value", fabs(y1-y) < 0.0001);
		}

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

void DataTypeTests::roundtrip_update (
    char *sql_type,
    int rdbi_type,
    int rdbi_scale, // unused
    int rdbi_precision, // unused
    void *variable,
    int size,
    double tolerance,
    bool is_ora_int64_test,
    int out_rdbi_type)
{
    int cursor;
    char statement[1024];
    int type;
    int bytes;
    int null_ok;
    void *data1;
    void *data2;
    long null_ind1 = 0;
    long null_ind2 = 0;
    int rows;
	long *variable2 = 0; // NOTE: for geometries, we need null data.
    int desc_rdbi_type = (out_rdbi_type == -1) ? rdbi_type : out_rdbi_type;

    cursor = -1;
    try
    {
        CPPUNIT_ASSERT_MESSAGE ("rdbi_est_cursor failed", RDBI_SUCCESS == rdbi_est_cursor (mRdbiContext, &cursor));

        // insert a row with the data
        sprintf (statement, "update bar set xyz1= %s, xyz2 = %s", get_bind_var (1,rdbi_type), get_bind_var (2,rdbi_type));

        // Set the null indicators
        (void) rdbi_set_nnull (mRdbiContext, (void *)&null_ind1, 0, 0 );
        (void) rdbi_set_null  (mRdbiContext, (void *)&null_ind2, 0, 0 );

        if (mRdbiContext->dispatch.capabilities.supports_unicode == 1){
            CPPUNIT_ASSERT_MESSAGE ("rdbi_sql_va failed", RDBI_SUCCESS == rdbi_sql_vaW (mRdbiContext,
                RDBI_VA_BNDNULLS|RDBI_VA_EXEC, cursor, FdoStringP(statement), rdbi_type, size, variable, &null_ind1,
                rdbi_type, size, &variable2, &null_ind2, RDBI_VA_EOL,RDBI_VA_EOL));
        }else{
            CPPUNIT_ASSERT_MESSAGE ("rdbi_sql_va failed", RDBI_SUCCESS == rdbi_sql_va (mRdbiContext,
                RDBI_VA_BNDNULLS|RDBI_VA_EXEC, cursor, FdoStringP(statement), rdbi_type, size, variable, &null_ind1,
                rdbi_type, size, &variable2, &null_ind2, RDBI_VA_EOL,RDBI_VA_EOL));
        }
        // describe the select
        sprintf (statement, "select * from bar");

        CPPUNIT_ASSERT_MESSAGE ("rdbi_sql failed", RDBI_SUCCESS == rdbi_sql_Ex (mRdbiContext, cursor, statement));
        CPPUNIT_ASSERT_MESSAGE ("rdbi_desc_slct failed", RDBI_SUCCESS == rdbi_desc_slct_Ex (mRdbiContext, cursor, 1, sizeof (statement), statement, &type, &bytes, &null_ok));
		CPPUNIT_ASSERT_MESSAGE ("column wrong name", 0 == FdoCommonOSUtil::stricmp ("xyz1", statement));
        CPPUNIT_ASSERT_MESSAGE ("column wrong type", is_datatype_equal(desc_rdbi_type, type) );
        // CPPUNIT_ASSERT_MESSAGE ("column wrong size", rdbi_scale == bytes);
        // CPPUNIT_ASSERT_MESSAGE ("column not nullable", 0 != null_ok);

        // define the result variables
        if (is_ora_int64_test)
            bytes = size;

        data1 = alloca (bytes);
        memset( data1, '\0', bytes);//size
		void *addr1 = ( rdbi_type == RDBI_GEOMETRY )? &data1 : data1;
        CPPUNIT_ASSERT_MESSAGE ("rdbi_define failed", RDBI_SUCCESS == rdbi_define (mRdbiContext, cursor, "1", rdbi_type, bytes, (char*)addr1, (char*)&null_ind1));

        data2 = alloca (bytes);
        memset( data2, '\0', bytes);//size
		void *addr2 = ( rdbi_type == RDBI_GEOMETRY )? &data2 : data2;
        CPPUNIT_ASSERT_MESSAGE ("rdbi_define failed", RDBI_SUCCESS == rdbi_define (mRdbiContext, cursor, "2", rdbi_type, bytes, (char*)addr2, (char*)&null_ind2));

        // execute, fetch and check the values
        CPPUNIT_ASSERT_MESSAGE ("rdbi_execute failed", RDBI_SUCCESS == rdbi_execute (mRdbiContext, cursor, 1, 0));
        CPPUNIT_ASSERT_MESSAGE ("rdbi_fetch failed", RDBI_SUCCESS == rdbi_fetch (mRdbiContext, cursor, 1, &rows));
        CPPUNIT_ASSERT_MESSAGE ("wrong row count", 1 == rows);
        CPPUNIT_ASSERT_MESSAGE ("value is null", !rdbi_is_null(mRdbiContext, &null_ind1, 0));
        CPPUNIT_ASSERT_MESSAGE ("value is not null", rdbi_is_null(mRdbiContext, &null_ind2, 0));

        if (0.0 != tolerance) {
            double tol;
            if ( rdbi_type == RDBI_FLOAT )
                tol = fabs (*((float*)variable) - *((float*)data1));
            else
                tol = fabs (*((double*)variable) - *((double*)data1));

            CPPUNIT_ASSERT_MESSAGE ("column is wrong value", tolerance > tol);
        } 
		else if ( rdbi_type != RDBI_GEOMETRY )
		{
            CPPUNIT_ASSERT_MESSAGE ("column is wrong value", 0 == memcmp (variable, data1, (bytes < size) ? bytes:size));//size
        }
		else if ( rdbi_type == RDBI_GEOMETRY ) {
			FdoIGeometry** pg = (FdoIGeometry**)(variable);
			FdoIPoint *     g = (FdoIPoint *) (*pg);
			FdoPtr<FdoIDirectPosition> p = g->GetPosition();
			double x = p->GetX();
			double y = p->GetY();

			FdoIGeometry** pg1 = (FdoIGeometry**)(addr1);
			FdoPtr<FdoIPoint> g1 = (FdoIPoint *) (*pg1);
			FdoPtr<FdoIDirectPosition> p1 = g1->GetPosition();
			double x1 = p1->GetX();
			double y1 = p1->GetY();
			
			CPPUNIT_ASSERT_MESSAGE ("point X is wrong value", fabs(x1-x) < 0.0001);
			CPPUNIT_ASSERT_MESSAGE ("point Y is wrong value", fabs(y1-y) < 0.0001);
		}

        // clean up
        sprintf (statement, "drop table bar");
        CPPUNIT_ASSERT_MESSAGE ("rdbi_sql failed", RDBI_SUCCESS == rdbi_sql_Ex (mRdbiContext, cursor, statement));
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

void DataTypeTests::geometry ()
{
    FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
    FdoPtr<FdoIDirectPosition> position = gf->CreatePositionXY (522.5, 763.0);
    FdoIGeometry *g = gf->CreatePoint (position);

    roundtrip_insert (get_geometry_type(), RDBI_GEOMETRY, 0, 0, &g, sizeof(g));

	g->Release();
}

void DataTypeTests::single_char ()
{
    char ch[2];

    ch[0] = 'K'; ch[1] = '\0'; // Null terminator
    roundtrip_insert ("CHAR", RDBI_CHAR, 1, 0, &ch, 1);
	ch[0] = 'F';
	roundtrip_update ("CHAR", RDBI_CHAR, 1, 0, &ch, 1);
}

void DataTypeTests::single_character ()
{
    char ch[2];

    ch[0] = 'S'; ch[1] = '\0'; // Null terminator
    roundtrip_insert ("CHARACTER", RDBI_CHAR, 1, 0, &ch, 1);
}

void DataTypeTests::dozen_char ()
{
    char ch[12];

    sprintf (ch, "initializer");
    roundtrip_insert ("CHAR (12)", RDBI_FIXED_CHAR, 12, 0, &ch, sizeof (ch));
	sprintf (ch, "1nitialize1");
    roundtrip_update ("CHAR (12)", RDBI_FIXED_CHAR, 12, 0, &ch, sizeof (ch));
}

void DataTypeTests::dozen_character ()
{
    char ch[12];

    sprintf (ch, "Enterprise2");
    roundtrip_insert ("CHARACTER (12)", RDBI_FIXED_CHAR, 12, 0, &ch, sizeof (ch));
}

void DataTypeTests::char_varying ()
{
    char ch[25];

    memset (ch, 0, sizeof (ch));
    sprintf (ch, "infrastructure");
    roundtrip_insert ("CHAR VARYING (25)", RDBI_STRING, 25, 0, &ch, (int)strlen (ch) + 1);
}

void DataTypeTests::character_varying ()
{
    char ch[25];

    memset (ch, 0, sizeof (ch));
    sprintf (ch, "hobbit");
    roundtrip_insert ("CHARACTER VARYING (25)", RDBI_STRING, 25, 0, &ch, (int)strlen (ch) + 1);
}

void DataTypeTests::varchar ()
{
    char ch[25];

    memset (ch, 0, sizeof (ch));
    sprintf (ch, "mechanism");
    roundtrip_insert ("VARCHAR (25)", RDBI_STRING, 25, 0, &ch, (int)strlen (ch) + 1);

	memset (ch, 0, sizeof (ch));
    sprintf (ch, "update test");
    roundtrip_update ("VARCHAR (25)", RDBI_STRING, 25, 0, &ch, (int)strlen (ch) + 1);
}

#pragma message ("ToDo: NATIONAL and NCHAR datatypes")

#pragma message ("ToDo: BIT datatype")

void DataTypeTests::numeric ()
{
    double d;

    d = 92929;
    roundtrip_insert ("NUMERIC", RDBI_DOUBLE, sizeof (d), 0, &d, sizeof (d));
}

void DataTypeTests::numeric_2_0 ()
{
    short s;

    s = 69;
    roundtrip_insert ("NUMERIC (2, 0)", RDBI_SHORT, sizeof (s), 0, &s, sizeof (s));
}

void DataTypeTests::numeric_6_0 ()
{
    int n;

    n = 2005;
    // really should be RDBI_INT
    roundtrip_insert ("NUMERIC (6, 0)", RDBI_LONG, sizeof (n), 0, &n, sizeof (n));
}

void DataTypeTests::numeric_6_2 ()
{
    double d;

    d = 92.8;
    roundtrip_insert ("NUMERIC (6, 2)", RDBI_DOUBLE, sizeof (d), 2, &d, sizeof (d));
}

void DataTypeTests::decimal ()
{
    double d;

    d = 92929;
    roundtrip_insert ("DECIMAL", RDBI_DOUBLE, sizeof (d), 0, &d, sizeof (d));
}

void DataTypeTests::decimal_2_0 ()
{
    short s;

    s = 69;
    roundtrip_insert ("DECIMAL (2, 0)", RDBI_SHORT, sizeof (s), 0, &s, sizeof (s));
}

void DataTypeTests::decimal_6_0 ()
{
    int n;

    n = 2005;
    // really should be RDBI_INT
    roundtrip_insert ("DECIMAL (6, 0)", RDBI_LONG, sizeof (n), 0, &n, sizeof (n));
}

void DataTypeTests::decimal_6_2 ()
{
    double d;

    d = 92.8;
    roundtrip_insert ("DECIMAL (6, 2)", RDBI_DOUBLE, sizeof (d), 2, &d, sizeof (d));
}

void DataTypeTests::dec ()
{
{
    double d;

    d = 92929;
    roundtrip_insert ("DEC", RDBI_DOUBLE, sizeof (d), 0, &d, sizeof (d));
}
}

void DataTypeTests::dec_2_0 ()
{
    short s;

    s = 69;
    roundtrip_insert ("DEC (2, 0)", RDBI_SHORT, sizeof (s), 0, &s, sizeof (s));
}

void DataTypeTests::dec_6_0 ()
{
    int n;

    n = 2005;
    // really should be RDBI_INT
    roundtrip_insert ("DEC (6, 0)", RDBI_LONG, sizeof (n), 0, &n, sizeof (n));
}

void DataTypeTests::dec_6_2 ()
{
    double d;

    d = 92.8;
    roundtrip_insert ("DEC (6, 2)", RDBI_DOUBLE, sizeof (d), 2, &d, sizeof (d));
}

void DataTypeTests::integer ()
{
    int d;

    d = 92929;
    roundtrip_insert ("INTEGER", RDBI_INT, sizeof (d), 0, &d, sizeof (d));
}

void DataTypeTests::smallint ()
{
    short d;

    d = 92;
    roundtrip_insert ("SMALLINT", RDBI_SHORT, sizeof (d), 0, &d, sizeof (d));
}

void DataTypeTests::floating ()
{
    double d;

    d = 929.282f; 
    roundtrip_insert ("FLOAT", RDBI_DOUBLE, sizeof (d), 0, &d, sizeof (d), 1e-3);
}

void DataTypeTests::floating_15 ()
{
    float d;

    d = 929.134233f;
    /* with 15 decimal points how come Oracle can only hold 2? */
    roundtrip_insert ("FLOAT (15)", RDBI_FLOAT, sizeof (d), 0, &d, sizeof (d), 1e-2);
}

void DataTypeTests::real ()
{
    float d;

    d = 929.134233f;
    roundtrip_insert ("REAL", RDBI_FLOAT, sizeof (d), 0, &d, sizeof (d), 1e-6);
}

void DataTypeTests::double_precision ()
{
    double d;

    d = 929.134233;
    roundtrip_insert ("DOUBLE PRECISION", RDBI_DOUBLE, sizeof (d), 0, &d, sizeof (d), 1e-6);
}

void DataTypeTests::longlong ()
{
#ifdef _WIN32
    __int64 i64;
#else
    int64_t i64;
#endif

    char ch[100];
    size_t ch_len = 0;

    i64 = 1234567890123456789LL;

    if (mRdbiContext->dispatch.capabilities.supports_int64_binding)
        roundtrip_insert (get_bigint_data_type(), RDBI_LONGLONG, sizeof (i64), 0, &i64, sizeof (i64));
    else {

#ifdef _WIN32
        sprintf(ch, "%I64d", i64);
#else
        sprintf(ch, "%lld", i64);
#endif
        ch_len = strlen (ch) + 1;
        roundtrip_insert (get_bigint_data_type(), RDBI_STRING, (int)ch_len, 0, &ch, (int)ch_len, 0, true);

    }
}

void DataTypeTests::date ()
{
    struct tm when;
    char *p;
    char when_string[64];

    memset (&when, 0, sizeof (struct tm));
    when.tm_year = 1979;
    when.tm_mon = 11;
    when.tm_mday = 30;
    p = get_date_time (&when);
    strcpy (when_string, p);
    delete[] p;

    roundtrip_insert (get_datetime_type(), RDBI_DATE, sizeof (when_string), 0, when_string, (int)strlen (when_string) + 1);
}

void DataTypeTests::time ()
{
    struct tm when;
    char *p;
    char when_string[64];

    memset (&when, 0, sizeof (struct tm));
    when.tm_year = 1979;
    when.tm_mon = 11;
    when.tm_mday = 30;
    p = get_date_time (&when);
    strcpy (when_string, p);
    delete[] p;

    roundtrip_insert (get_datetime_type(), RDBI_DATE, sizeof (when_string), 0, when_string, (int)strlen (when_string) + 1);
}

void DataTypeTests::timestamp ()
{
    struct tm when;
    char *p;
    char when_string[64];

    memset (&when, 0, sizeof (struct tm));
    when.tm_year = 1979;
    when.tm_mon = 11;
    when.tm_mday = 30;
    when.tm_hour = 18;
    when.tm_min = 45;
    when.tm_sec = 23;
    p = get_date_time (&when);
    strcpy (when_string, p);
    delete[] p;

    // SQL Server doesn't allow not null values, therefor don't do it.
    if ( allow_timestamp_notnull() )
        roundtrip_insert ("TIMESTAMP", RDBI_DATE, sizeof (when_string), 0, when_string, (int)strlen (when_string) + 1);
}

#pragma message ("ToDo: TIME and TIMESTAMP with precision and 'with time zone'")
