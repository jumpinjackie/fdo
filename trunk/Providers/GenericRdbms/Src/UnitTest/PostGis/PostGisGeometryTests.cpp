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
#include "PostGisGeometryTests.h"
#include "UnitTestUtil.h"
#include "../../PostGis/Driver/rdbi_init.h"
int postgis_rdbi_init( void **, rdbi_methods	methods );

CPPUNIT_TEST_SUITE_REGISTRATION( PostGisGeometryTests );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( PostGisGeometryTests, "GeometryTests");

void PostGisGeometryTests::setUp ()
{
    try
    {
        set_provider();

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


void PostGisGeometryTests::set_provider()
{
	UnitTestUtil::SetProvider( "PostGIS" );
}

int PostGisGeometryTests::do_rdbi_init ()
{
#ifdef RDBI_STATIC
    return (rdbi_init (&mRdbiContext, (initializer*)postgis_rdbi_init));
#else
    return (rdbi_initialize (&mRdbiContext, "PostGisDriver"));
#endif
return 0;
}

int PostGisGeometryTests::do_rdbi_connect (const char* dataStoreName, const char* userName, const char* userPassword)
{
    FdoStringP postGisConnectString;
    
    if ( dataStoreName[0] == '\0' ) 
        postGisConnectString = UnitTestUtil::GetEnviron("service");
    else
        postGisConnectString = FdoStringP::Format(
            L"%hs%%ls",
            dataStoreName,
            (FdoString*)(UnitTestUtil::GetEnviron("service"))
        );
    
    int ret = rdbi_connect (
        mRdbiContext,
        UnitTestUtil::GetEnviron("service"),
        UnitTestUtil::GetEnviron("username"),
        UnitTestUtil::GetEnviron("password"),
        &mId);

    return ret;
}

char* PostGisGeometryTests::get_geometry_text (FdoIGeometry *geometry)
{
    const wchar_t *wchars;
    char *chars;
    char *ret;

    wchars = geometry->GetText ();
    wide_to_multibyte (chars, wchars);
    ret = new char[strlen (chars) + 10]; /* includes "''" */
    strcpy (ret, "'");
    strcat (ret, chars);
    strcat (ret, "'");

    return (ret);
}

int PostGisGeometryTests::do_insert_geometry( int cursor, FdoInt32 *featId, FdoIGeometry **geometry )
{
    int     rc;
    // Do bind instead of literals
    if (mRdbiContext->dispatch.capabilities.supports_unicode == 1){
        rc  = rdbi_sql_vaW (mRdbiContext, RDBI_VA_EXEC, cursor, L"insert into foo values (?, geometry::STGeomFromWKB(?, 0))",
            RDBI_LONG, sizeof(long), featId, RDBI_GEOMETRY, sizeof (geometry), geometry, RDBI_VA_EOL, RDBI_VA_EOL);
    }else{
        rc  = rdbi_sql_va (mRdbiContext, RDBI_VA_EXEC, cursor, "insert into foo values (?, geometry::STGeomFromWKB(?, 0))",
            RDBI_LONG, sizeof(long), featId, RDBI_GEOMETRY, sizeof (geometry), geometry, RDBI_VA_EOL, RDBI_VA_EOL);
    }
    return rc;
}

char* PostGisGeometryTests::mVar[] =
{
    ":0",
    ":1",
    ":2",
    ":3",
    ":4",
    ":5",
    ":6",
    ":7",
    ":8",
    ":9",
    ":10",
    ":11",
    ":12",
    ":13",
    ":14",
    ":15",
    ":16",
};

