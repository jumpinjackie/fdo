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

CPPUNIT_TEST_SUITE_REGISTRATION( SqlServerFdoDatastoreTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( SqlServerFdoDatastoreTest, "FdoDatastoreTest");

void SqlServerFdoDatastoreTest::set_provider()
{
    UnitTestUtil::SetProvider( "SQLServerSpatial" );
}

void SqlServerFdoDatastoreTest::ReservedName(FdoIConnection* connection)
{
}

void SqlServerFdoDatastoreTest::ListDataStores_LimitedPrivilegeUser()
{
    FdoPtr<FdoIConnection> connection;
    try
    {
        printf(" >>> ... creating test database for limited user \n");
        if (UnitTestUtil::DatastoreExists(L"_limiteduser"))
            UnitTestUtil::DropDb(L"_limiteduser");
        UnitTestUtil::CreateDB(false, false, L"_limiteduser");

        // Let's make no assumptions about whether pre-existing dbs for other tests have been
        // created and make some ourselves
        if (UnitTestUtil::DatastoreExists(L"_limiteduser2"))
            UnitTestUtil::DropDb(L"_limiteduser2");
        UnitTestUtil::CreateDB(false, false, L"_limiteduser2");

        // Connect and create the test schema.
        connection = UnitTestUtil::GetConnection(L"_limiteduser");

        // Create our user if it doesn't exist and map it to this database
        printf(" >>> ... creating limited privilege user if it doesn't exist \n");

        FdoStringP sql = L"IF NOT EXISTS(SELECT principal_id FROM sys.server_principals WHERE name = 'fdolimitedprivilegeuser') BEGIN\n";
        sql += L"    CREATE LOGIN fdolimitedprivilegeuser\n";
        sql += L"    WITH PASSWORD = 'Fd0l!mitedprivilegeuser'\n";
        sql += L"END;\n";
        sql += L"IF NOT EXISTS(SELECT principal_id FROM sys.database_principals WHERE name = 'fdolimitedprivilegeuser') BEGIN\n";
        sql += L"    CREATE USER fdolimitedprivilegeuser FOR LOGIN fdolimitedprivilegeuser\n";
        sql += L"END;";

        UnitTestUtil::Sql2Db(sql, connection);

        connection->Close();

        FdoStringP service = UnitTestUtil::GetEnviron("service");
        // Now connect using our limited privilege user
        FdoStringP connStr = FdoStringP::Format(L"Service=%ls;Username=%ls;Password=%ls",
            (FdoString*)service,
            L"fdolimitedprivilegeuser",
            L"Fd0l!mitedprivilegeuser");

        connection->SetConnectionString(connStr);
        connection->Open();

        FdoPtr<FdoIListDataStores>  pListDataStoresCmd = (FdoIListDataStores*)connection->CreateCommand(FdoCommandType_ListDataStores);
        pListDataStoresCmd->SetIncludeNonFdoEnabledDatastores(true);
        FdoPtr<FdoIDataStoreReader>	pReader = pListDataStoresCmd->Execute();

        int count = 0;
        bool bFoundMaster = false;
        bool bFoundMsdb = false;
        bool bFoundTempdb = false;
        bool bFoundMyDb = false;

        FdoStringP mydb = UnitTestUtil::GetEnviron("datastore", L"_limiteduser");

        while (pReader->ReadNext())
        {
            count++;
            FdoString* name = pReader->GetName();
            printf("  Found db: %ls\n", name);
            if (FdoCommonOSUtil::wcsicmp(name, L"master") == 0)
            {
                bFoundMaster = true;
            }
            else if (FdoCommonOSUtil::wcsicmp(name, L"msdb") == 0)
            {
                bFoundMsdb = true;
            }
            else if (FdoCommonOSUtil::wcsicmp(name, L"tempdb") == 0)
            {
                bFoundTempdb = true;
            }
            else if (FdoCommonOSUtil::wcsicmp(name, mydb) == 0)
            {
                bFoundMyDb = true;
            }
        }
        pReader->Close();

        CPPUNIT_ASSERT(4 == count);
        CPPUNIT_ASSERT(bFoundMaster);
        CPPUNIT_ASSERT(bFoundMsdb);
        CPPUNIT_ASSERT(bFoundTempdb);
        CPPUNIT_ASSERT(bFoundMyDb);

        connection->Close();
    }
    catch (FdoException *exp)
    {
        printf(" >>> Exception: %ls\n", exp->GetExceptionMessage());
        if (connection)
            connection->Close();
        TestCommonFail(exp);
    }

    catch (...)
    {
        if (connection) connection->Close();
        throw;
    }
}