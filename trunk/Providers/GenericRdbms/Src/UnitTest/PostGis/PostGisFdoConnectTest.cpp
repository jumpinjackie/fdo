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
#include "PostGisFdoConnectTest.h"
#include "UnitTestUtil.h"

CPPUNIT_TEST_SUITE_REGISTRATION(PostGisFdoConnectTest);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(PostGisFdoConnectTest, "FdoConnectTest");

void PostGisFdoConnectTest::set_provider()
{
	UnitTestUtil::SetProvider("PostGIS");
}

void PostGisFdoConnectTest::connectPropDictionaryTest()

{
	FdoPtr<FdoIConnection> conn;
	int ret;
	ret = 1;

	try
	{
		conn = UnitTestUtil::GetProviderConnectionObject();
		FdoPtr<FdoIConnectionInfo> connInfo(conn->GetConnectionInfo());
		FdoPtr<FdoIConnectionPropertyDictionary> connProps(connInfo->GetConnectionProperties());

        CPPUNIT_ASSERT(true == connProps->IsPropertyRequired(L"Username"));
		CPPUNIT_ASSERT(true == connProps->IsPropertyRequired(L"Password"));
        CPPUNIT_ASSERT(true == connProps->IsPropertyRequired(L"Service"));
        CPPUNIT_ASSERT(false == connProps->IsPropertyRequired(L"DataStore"));
	}
	catch (FdoException* exception)
	{
        TestCommonFail(exception);
	}
	catch (...)
	{
        TestCommonFail(FdoException::Create(L"Unknown exception"));
	}
}

void PostGisFdoConnectTest::connectWithInvalidString()
{
    // TODO: Not implemented
}
