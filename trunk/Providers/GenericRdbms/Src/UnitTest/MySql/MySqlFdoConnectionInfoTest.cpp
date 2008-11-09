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
#include "MySqlFdoConnectionInfoTest.h"
#include "UnitTestUtil.h"

CPPUNIT_TEST_SUITE_REGISTRATION(MySqlFdoConnectionInfoTest);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(MySqlFdoConnectionInfoTest, "FdoConnectionInfoTest");


void MySqlFdoConnectionInfoTest::set_provider ()
{

	UnitTestUtil::SetProvider("MySql");

}  //  set_provider ()

void MySqlFdoConnectionInfoTest::SetExpectedFileList ()
{

    // This is a database provider and hence does not have a list of
    // dependent files.

}  //  SetExpectedFileList ()

void MySqlFdoConnectionInfoTest::TestProviderInfo ()
{

    try {

        // The test is to establish a connection and get the connection info.
        // Then the connection info APIs to retrieve the provider type and the
        // list of dependent files is executed. For this test it is expected
        // that the provider type is a "Database Server" and that there are
        // no dependent files. Any other result should trigger exceptions.

		mConnection = UnitTestUtil::GetProviderConnectionObject();
        FdoStringP userConnectionString =
                UnitTestUtil::GetConnectionString(Connection_WithDatastore);
        mConnection->SetConnectionString(userConnectionString);
        mConnection->Open();

        CheckDatabaseBasedProviderInfo ();

        mConnection->Close();

    }  //  try ...

	catch (FdoException *e)
    {
        if (mConnection)
            mConnection->Close();
        TestCommonFail(e);
	}  //  catch ...

}  //  TestProviderInfo ()
