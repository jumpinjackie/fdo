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
#include "SqlServerFdoDatastoreTest.h"
#include "UnitTestUtil.h"

#define DB_NOMETA_SUFFIX1 L"_dbcmd_nometa1"
#define DB_NOMETA_SUFFIX2 L"_dbcmd_nometa2"

CPPUNIT_TEST_SUITE_REGISTRATION( SqlServerFdoDatastoreTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( SqlServerFdoDatastoreTest, "FdoDatastoreTest");

void SqlServerFdoDatastoreTest::set_provider()
{
	UnitTestUtil::SetProvider( "SQLServerSpatial" );
}

void SqlServerFdoDatastoreTest::Cmd_CreateNoMeta()
{
    FdoPtr<FdoIConnection> connection;

    try
    {
		wchar_t *connectString = UnitTestUtil::GetConnectionString(Connection_NoDatastore);
		connection = UnitTestUtil::GetProviderConnectionObject();
		connection->SetConnectionString ( connectString);

		connection->Open();

        if ( UnitTestUtil::DatastoreExists( DB_NOMETA_SUFFIX1 ) )
            UnitTestUtil::DropDb( DB_NOMETA_SUFFIX1 );

        if ( UnitTestUtil::DatastoreExists( DB_NOMETA_SUFFIX2 ) )
            UnitTestUtil::DropDb( DB_NOMETA_SUFFIX2 );

		FdoStringP datastore1 = UnitTestUtil::GetEnviron("datastore", DB_NOMETA_SUFFIX1);
		FdoStringP datastore2 = UnitTestUtil::GetEnviron("datastore", DB_NOMETA_SUFFIX2);

        CreateDatastore( 
            connection,
    		datastore1,
            true,
            false
		);

        CreateDatastore( 
            connection,
    		datastore2,
            false,
            true
		);

        FdoPtr<FdoIListDataStores>		pListDataStoresCmd = (FdoIListDataStores*) connection->CreateCommand(FdoCommandType_ListDataStores);
   		pListDataStoresCmd->SetIncludeNonFdoEnabledDatastores( false );
		FdoPtr<FdoIDataStoreReader>	pReader = pListDataStoresCmd->Execute();

		while ( pReader->ReadNext() )
        {
            CPPUNIT_ASSERT( datastore1 != pReader->GetName() );
            CPPUNIT_ASSERT( datastore2 != pReader->GetName() );
        }

    }
    catch (FdoException *ex)
    {
        TestCommonFail(ex);
    }
}



void SqlServerFdoDatastoreTest::ReservedName(FdoIConnection* connection)
{
}
