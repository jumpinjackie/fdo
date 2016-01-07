/*
 * Copyright (C) 2006 Refractions Research, Inc.
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
#include "PostGisConnectTests.h"
#include "UnitTestUtil.h"
extern initializer postgis_rdbi_init;

CPPUNIT_TEST_SUITE_REGISTRATION(PostGisConnectTests);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(PostGisConnectTests, "ConnectTests");

void PostGisConnectTests::set_provider()
{
	UnitTestUtil::SetProvider("PostGIS");
}

int PostGisConnectTests::do_rdbi_init(rdbi_context_def** rdbi_context)
{
#ifdef RDBI_STATIC
        return (rdbi_init(rdbi_context, postgis_rdbi_init));
#else
        return (rdbi_initialize(rdbi_context, "PostGisDriver"));
#endif
}

int PostGisConnectTests::do_rdbi_connect(rdbi_context_def* rdbi_context, int& id)
{
    int ret = RDBI_GENERIC_ERROR;

    ret = rdbi_connect(rdbi_context,
                       UnitTestUtil::GetEnviron("service"),
                       UnitTestUtil::GetEnviron("username"),
                       UnitTestUtil::GetEnviron("password"), 
                       &id);

    return ret;
}
