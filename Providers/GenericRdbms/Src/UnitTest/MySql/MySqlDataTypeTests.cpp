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
#include "MySqlDataTypeTests.h"
#include "UnitTestUtil.h"
extern initializer mysql_rdbi_init;

CPPUNIT_TEST_SUITE_REGISTRATION (MySqlDataTypeTests);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION (MySqlDataTypeTests, "DataTypeTests");

void MySqlDataTypeTests::set_provider()
{
	UnitTestUtil::SetProvider( "MySql" );
}

int MySqlDataTypeTests::do_rdbi_init ()
{
#ifdef RDBI_STATIC
    return (rdbi_init (&mRdbiContext, mysql_rdbi_init));
#else
    return (rdbi_initialize (&mRdbiContext, "MySqlDriver"));
#endif
}

int MySqlDataTypeTests::do_rdbi_connect (const char* dataStoreName, const char* userName, const char* userPassword)
{
    return (rdbi_connect (
        mRdbiContext,
        NULL,  //TODO:             UnitTestUtil::GetEnviron("service") ?
        UnitTestUtil::GetEnviron("username"),
        UnitTestUtil::GetEnviron("password"),
        &mId));
}

bool MySqlDataTypeTests::is_datatype_equal(int rdbi_type, int type) 
{  
    bool    rc = true;
    // Also the precision is not realiable, therefore the FDO caller
    // should do a cast to the desired type. Duh!!
    if ( type == RDBI_DOUBLE )
        rc = true;
    else
        rc = (rdbi_type == type);
    return rc;
}

char *MySqlDataTypeTests::get_date_time (const struct tm *when)
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
