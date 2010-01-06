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
#include "MySqlSelectTests.h"
#include "UnitTestUtil.h"
extern initializer mysql_rdbi_init;

CPPUNIT_TEST_SUITE_REGISTRATION( MySqlSelectTests );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( MySqlSelectTests, "SelectTests");

void MySqlSelectTests::set_provider()
{
	UnitTestUtil::SetProvider( "MySql" );
}

int MySqlSelectTests::do_rdbi_init ()
{
#ifdef RDBI_STATIC
    return (rdbi_init (&mRdbiContext, mysql_rdbi_init));
#else
    return (rdbi_initialize (&mRdbiContext, "MySqlDriver"));
#endif
}

int MySqlSelectTests::do_rdbi_connect (const char* dataStoreName, const char* userName, const char* userPassword)
{
    return (rdbi_connect (
        mRdbiContext,
        UnitTestUtil::GetEnviron("service"),
        UnitTestUtil::GetEnviron("username"),
        UnitTestUtil::GetEnviron("password"),
        &mId));
}

char* MySqlSelectTests::get_date_time (const struct tm *when)
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
