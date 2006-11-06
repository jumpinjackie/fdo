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
#include "MySqlFdoDatastoreTest.h"
#include "UnitTestUtil.h"

CPPUNIT_TEST_SUITE_REGISTRATION( MySqlFdoDatastoreTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( MySqlFdoDatastoreTest, "FdoDatastoreTest");

void MySqlFdoDatastoreTest::set_provider()
{
	UnitTestUtil::SetProvider( "MySql" );
}

void MySqlFdoDatastoreTest::ReservedName(FdoIConnection* connection)
{
    bool failed = false;

    try {
        CreateDatastore(connection, L"longblob");
    }
    catch (FdoException *ex)
    {
        wchar_t expectedErrMsg[] = L"Cannot create datastore, name 'longblob' is a reserved word ";
        FdoString *excMsg = ex->GetExceptionMessage();
        // eliminate a space from the end if is the case
        if (excMsg[wcslen(excMsg) - 1] != L' ')
            expectedErrMsg[wcslen(expectedErrMsg)-1] = L'\0';

        if ( wcscmp(excMsg, expectedErrMsg) != 0 )
            throw;
        failed = true;
    }

    if ( !failed ) 
        CPPUNIT_FAIL("Create datastore with reserved name was supposed to fail");
}
