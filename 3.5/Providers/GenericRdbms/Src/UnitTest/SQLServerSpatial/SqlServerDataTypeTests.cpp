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
#include "SqlServerDataTypeTests.h"
#include "UnitTestUtil.h"
#include "../../ODBCDriver/context.h"
int odbcdr_rdbi_init( odbcdr_context_def **, rdbi_methods	methods );

CPPUNIT_TEST_SUITE_REGISTRATION (SqlServerDataTypeTests);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION (SqlServerDataTypeTests, "DataTypeTests");

void SqlServerDataTypeTests::set_provider()
{
	UnitTestUtil::SetProvider( "SQLServerSpatial" );
}

int SqlServerDataTypeTests::do_rdbi_init ()
{
#ifdef RDBI_STATIC
    return (rdbi_init (&mRdbiContext, (initializer*)odbcdr_rdbi_init));
#else
    return (rdbi_initialize (&mRdbiContext, "SqlServerDriver"));
#endif
}

int SqlServerDataTypeTests::do_rdbi_connect (const char* dataStoreName, const char* userName, const char* userPassword)
{
    FdoStringP odbcConnectString = FdoStringP::Format(
        L"DRIVER={SQL Server}; SERVER=%ls; UID=%hs; PWD=%hs", 
        (FdoString*)(UnitTestUtil::GetEnviron("service")), 
        userName, 
        userPassword
    );
    if (mRdbiContext->dispatch.capabilities.supports_unicode == 1)
    {
        return (rdbi_connectW (
            mRdbiContext,
            odbcConnectString,
            L"xxxx", // not used
            L"test",
            &mId));
    }
    else
    {
        return (rdbi_connect (
            mRdbiContext,
            odbcConnectString,
            "xxxx", // not used
            "test",
            &mId));
    }
}

char* SqlServerDataTypeTests::get_date_time (const struct tm *when)
{
    char *ret;

    // SQL Server always returns the full format. So, in order the test to pass
    // use the full format only.
    //"1979-11-30 00:00:00.000"	

    ret = new char [30];

    sprintf (ret, "%4d-%02d-%02d %02d:%02d:%02d.%03d",
        when->tm_year,
        when->tm_mon,
        when->tm_mday,
        when->tm_hour,
        when->tm_min,
        when->tm_sec,
        0); // no fractions of a second in struct tm

    return (ret);
}
