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
#include "MySqlFdoConnectTest.h"
#include "UnitTestUtil.h"

CPPUNIT_TEST_SUITE_REGISTRATION( MySqlFdoConnectTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( MySqlFdoConnectTest, "FdoConnectTest");

void MySqlFdoConnectTest::set_provider()
{
	UnitTestUtil::SetProvider( "MySql" );
}

void MySqlFdoConnectTest::connectPropDictionaryTest()

{
	FdoPtr<FdoIConnection> oConn;
	int ret;
	ret = 1;

	try
	{
		oConn = UnitTestUtil::GetProviderConnectionObject();
		FdoPtr<FdoIConnectionInfo> oConnInfo = oConn->GetConnectionInfo();
		FdoPtr<FdoIConnectionPropertyDictionary> oConnProp = oConnInfo->GetConnectionProperties();

		FdoString ** propertyNames = NULL;
		FdoInt32 nameCount = 0;
		CPPUNIT_ASSERT( oConnProp->IsPropertyRequired(L"Username") == true);
		CPPUNIT_ASSERT( oConnProp->IsPropertyRequired(L"Password") == true);
		CPPUNIT_ASSERT( oConnProp->IsPropertyRequired(L"Service") == true);
		CPPUNIT_ASSERT( oConnProp->IsPropertyRequired(L"DataStore") == false);
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

