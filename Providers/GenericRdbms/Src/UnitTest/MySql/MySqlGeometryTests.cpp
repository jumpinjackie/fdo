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
#include "MySqlGeometryTests.h"
#include "UnitTestUtil.h"

CPPUNIT_TEST_SUITE_REGISTRATION( MySqlGeometryTests );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( MySqlGeometryTests, "GeometryTests");

void MySqlGeometryTests::set_provider()
{
	UnitTestUtil::SetProvider( "MySql" );
}

int MySqlGeometryTests::do_rdbi_connect (const char* dataStoreName, const char* userName, const char* userPassword)
{
    int cursor;
    int ret;

    ret = rdbi_connect (
        mRdbiContext,
        UnitTestUtil::GetEnviron("service"),
        UnitTestUtil::GetEnviron("username"),
        UnitTestUtil::GetEnviron("password"),
        &mId);

    if (RDBI_SUCCESS == ret)
    {
        // condition the connection to use MyIsam tables by default
        cursor = -1;
        try
        {
            CPPUNIT_ASSERT_MESSAGE ("rdbi_est_cursor failed", RDBI_SUCCESS == rdbi_est_cursor (mRdbiContext, &cursor));
            CPPUNIT_ASSERT_MESSAGE ("rdbi_sql failed", RDBI_SUCCESS == rdbi_sql (mRdbiContext, cursor, "set storage_engine=MyIsam"));
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
    return (ret);
}

char* MySqlGeometryTests::get_date_time (const struct tm *when)
{
    char *ret;

    if ((0 != when->tm_hour) || (0 != when->tm_min) || (0 != when->tm_sec))
    {
        // "1979-11-30 00:00:00"
        ret = new char [20];
        sprintf (ret, "%4d-%02d-%02d %02d:%02d:%02d",
            when->tm_year,
            when->tm_mon,
            when->tm_mday,
            when->tm_hour,
            when->tm_min,
            when->tm_sec);
    }
    else
    {
        // "1979-11-30"
        ret = new char [11];
        sprintf (ret, "%4d-%02d-%02d",
            when->tm_year,
            when->tm_mon,
            when->tm_mday);
    }

    return (ret);
}

char* MySqlGeometryTests::get_geometry_text (FdoIGeometry *geometry)
{
    const wchar_t *wchars;
    char *chars;
    char *ret;

    wchars = geometry->GetText ();
    wide_to_multibyte (chars, wchars);
    ret = new char[strlen (chars) + 1 + 17]; /* includes "GeomFromText('')" */
    strcpy (ret, "GeomFromText('");
    strcat (ret, chars);
    strcat (ret, "')");

    return (ret);
}

int MySqlGeometryTests::do_insert_geometry( int cursor, FdoInt32 *featId, FdoIGeometry **geometry )
{
    int     rc;

    sprintf (mSql, "insert into foo values (%ld, %s)", *featId, get_geometry_text (*geometry));   
    rc = rdbi_sql (mRdbiContext, cursor, mSql);
    if ( rc == RDBI_SUCCESS)
        rc = rdbi_execute (mRdbiContext, cursor, 1, 0);
    return rc;
}
