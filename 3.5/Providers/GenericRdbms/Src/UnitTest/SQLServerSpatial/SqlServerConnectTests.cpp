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
#include "SqlServerConnectTests.h"
#include "UnitTestUtil.h"
#include "../../ODBCDriver/context.h"
int odbcdr_rdbi_init( odbcdr_context_def **, rdbi_methods	methods );

CPPUNIT_TEST_SUITE_REGISTRATION (SqlServerConnectTests);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION (SqlServerConnectTests, "ConnectTests");

void SqlServerConnectTests::set_provider()
{
	UnitTestUtil::SetProvider( "SQLServerSpatial" );
}

int SqlServerConnectTests::do_rdbi_init (rdbi_context_def** rdbi_context)
{
#ifdef RDBI_STATIC
    return (rdbi_init (rdbi_context, (initializer*)odbcdr_rdbi_init));
#else
    return (rdbi_initialize (rdbi_context, "SqlServerDriver"));
#endif
}

int SqlServerConnectTests::do_rdbi_connect (rdbi_context_def* rdbi_context, int& id)
{
    FdoStringP odbcConnectString = FdoStringP::Format(L"DRIVER={SQL Server}; SERVER=%ls", (FdoString*)(UnitTestUtil::GetEnviron("service")));
    if (rdbi_context->dispatch.capabilities.supports_unicode == 1)
    {
        return (rdbi_connectW (
            rdbi_context,
            odbcConnectString,
            L"nm_demo", // not used
            L"test",
            &id));
    }
    else
    {
        return (rdbi_connect (
            rdbi_context,
            odbcConnectString,
            "nm_demo",
            "test", // not used??
            &id));
    }
}
