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
