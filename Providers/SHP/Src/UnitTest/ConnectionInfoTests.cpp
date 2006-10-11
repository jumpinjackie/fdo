/*
 * 
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
#include "ConnectionInfoTests.h"
#include <Spatial/SpatialStd.h>
#include <Spatial/SpatialUtility.h>

CPPUNIT_TEST_SUITE_REGISTRATION (ConnectionInfoTests);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION (ConnectionInfoTests, "ConnectionInfoTests");

FdoPtr<FdoIConnection> ConnectionInfoTests::mConnection;

ConnectionInfoTests::ConnectionInfoTests ()
{
}

ConnectionInfoTests::~ConnectionInfoTests ()
{
}

void ConnectionInfoTests::setUp ()
{
    mConnection = ShpTests::GetConnection ();
#ifdef _WIN32
    mConnection->SetConnectionString (L"DefaultFileLocation=..\\..\\TestData\\Ontario");
#else
    mConnection->SetConnectionString (L"DefaultFileLocation=../../TestData/Ontario");
#endif
    CPPUNIT_ASSERT_MESSAGE ("connection state not open", FdoConnectionState_Open == mConnection->Open ());
}

void ConnectionInfoTests::tearDown ()
{
    mConnection->Close ();
	FDO_SAFE_RELEASE(mConnection.p);
}

#define VERBOSE false

void ConnectionInfoTests::TestProviderInfo ()
{
	if (VERBOSE)
        printf("Checking for Provider Type and Dependent List Info \n");

	try {

        FdoPtr<FdoIConnectionInfo> connectionInfo = mConnection->GetConnectionInfo();
	    if (VERBOSE)
            printf(" ...> Checking for Provider Type\n");
        FdoProviderDatastoreType providerType = connectionInfo->GetProviderDatastoreType();

        switch (providerType) {

            case FdoProviderDatastoreType_DatabaseServer:
	            if (VERBOSE)
                    printf(" ......> Found: 'Database Server' (unexpected result)\n");
                throw FdoException::Create(L"Unexpected provider type 'Database Server' when expecting 'File'");
                break;
            case FdoProviderDatastoreType_File:
            	if (VERBOSE)
                    printf(" ......> Found: 'File' (expected result)\n");
                break;
            case FdoProviderDatastoreType_WebServer:
	            if (VERBOSE)
                    printf(" ......> Found: 'Web Server' (unexpected result)\n");
                throw FdoException::Create(L"Unexpected provider type 'Web Server' when expecting 'File'");
                break;
            case FdoProviderDatastoreType_Unknown:
            	if (VERBOSE)
                    printf(" ......> Found: 'Unknown' (unexpected result)\n");
                throw FdoException::Create(L"Unexpected provider type 'Unknown' when expecting 'File'");
                break;

        }

	    if (VERBOSE)
            printf(" ...> Checking for list of dependent files\n");
        FdoPtr<FdoStringCollection> stringCollection = connectionInfo->GetDependentFileNames();
        if (stringCollection == NULL)
            throw FdoException::Create(L"Unexpected empty list of dependent files");

        FdoInt32 stringCollectionCount = stringCollection->GetCount();
        if (stringCollectionCount > 0) {

            for (FdoInt32 index = 0; index < stringCollectionCount; index++) {

                FdoPtr<FdoStringElement> stringCollectionElement = stringCollection->GetItem(index);
                FdoStringP dependentFileName = stringCollectionElement->GetString();
	            if (VERBOSE)
                    printf(" ......> Found: '%ls'\n", dependentFileName);

            }

        }
        else
            throw FdoException::Create(L"Unexpected empty list of dependent files");

    }

	catch (FdoException *e) 
	{
        printf( "Exception: %ls\n", e->GetExceptionMessage() );
	}

}

