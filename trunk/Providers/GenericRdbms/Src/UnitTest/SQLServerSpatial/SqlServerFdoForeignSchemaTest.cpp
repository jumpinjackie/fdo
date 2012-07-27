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
#include "SqlServerFdoForeignSchemaTest.h"
#include "UnitTestUtil.h"

#define   DB_NAME_SUFFIX            L"_fs"

CPPUNIT_TEST_SUITE_REGISTRATION( SqlServerFdoForeignSchemaTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( SqlServerFdoForeignSchemaTest, "FdoForeignSchemaTest");

void SqlServerFdoForeignSchemaTest::set_provider()
{
	UnitTestUtil::SetProvider( "SQLServerSpatial" );
}
void SqlServerFdoForeignSchemaTest::create_foreign_datastore()
{
	bool dbExists = UnitTestUtil::DatastoreExists(DB_NAME_SUFFIX);
	FdoStringP userConnectString = UnitTestUtil::GetConnectionString(Connection_NoDatastore);
	FdoPtr<FdoIConnection> connection = UnitTestUtil::GetProviderConnectionObject();

    connection->SetConnectionString ( userConnectString);
    connection->Open();
	FdoStringP sqlStmt;
    FdoStringP databaseName = (FdoString*)(UnitTestUtil::GetEnviron("datastore", DB_NAME_SUFFIX));

	if (dbExists)
	{
		sqlStmt = FdoStringP::Format(L"drop database \"%ls\"", (FdoString*)databaseName);
		UnitTestUtil::Sql2Db(sqlStmt, connection);
	}
	sqlStmt = FdoStringP::Format(
		L"create database \"%ls\" ", (FdoString*)(UnitTestUtil::GetEnviron("datastore", DB_NAME_SUFFIX)));
   
	UnitTestUtil::Sql2Db(sqlStmt, connection);

	userConnectString = UnitTestUtil::GetConnectionString(Connection_WithDatastore, DB_NAME_SUFFIX);
    connection->SetConnectionString (userConnectString);
    connection->Open ();
    
    try
    {
        databaseName += L".dbo";
        sqlStmt = FdoStringP::Format(
		    L"create table %ls.device_tbl (id decimal(20), string varchar(64), install_date datetime, int32 int, bigstring nvarchar(max), bigchars varchar(max) constraint device_tbl_pk primary key (id, string, install_date))", (FdoString*)databaseName);
	    UnitTestUtil::Sql2Db(sqlStmt, connection);
    }
    catch(FdoException* e)
    {
        printf ("Exceptio: %ls\n", e->GetExceptionMessage());
        e->Release();
    }
    catch(...)
    {
        printf ("Unknown exceptio!\n");
    }
	connection->Close();
}

void SqlServerFdoForeignSchemaTest::insert()
{
	FdoPtr<FdoIConnection> connection = UnitTestUtil::GetConnection(DB_NAME_SUFFIX, false);
    FdoPtr<FdoIInsert> insertCommand;
    FdoPtr<FdoISelect> selectCommand;
    FdoPtr<FdoIFeatureReader> myReader;

    try
	{
		insertCommand = (FdoIInsert *) connection->CreateCommand(FdoCommandType_Insert);
        insertCommand->SetFeatureClassName(L"device_tbl");
        FdoPtr<FdoPropertyValueCollection> propertyValues = insertCommand->GetPropertyValues();

		FdoPtr<FdoDataValue> dataValue;
        FdoPtr<FdoPropertyValue> propertyValue;
		FdoInt64 idVal = 12;
		dataValue = FdoDataValue::Create(idVal);
        propertyValue = AddNewProperty( propertyValues,L"id"); 
        propertyValue->SetValue(dataValue);

		dataValue = FdoDataValue::Create(L"CANADA");
        propertyValue = AddNewProperty( propertyValues,L"string"); 
        propertyValue->SetValue(dataValue);
		
		FdoDateTime       currentDateTime;
		currentDateTime.year = 2006;
		currentDateTime.month = 11;
		currentDateTime.day = 18;
		currentDateTime.hour = 12;
		currentDateTime.minute = 45;
		currentDateTime.seconds = 15;

		dataValue = FdoDataValue::Create(currentDateTime);
        propertyValue = AddNewProperty( propertyValues,L"install_date"); 
        propertyValue->SetValue(dataValue);

		dataValue = FdoDataValue::Create(L"255");
        propertyValue = AddNewProperty( propertyValues,L"int32"); 
        propertyValue->SetValue(dataValue);

        FdoStringP bigSubString = L"ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmnopqrstuvwxyz";
        FdoStringP bigStringValue;

        int idx;
        for ( idx = 0; idx < 250; idx++ ) 
            bigStringValue += bigSubString;

		dataValue = FdoDataValue::Create(bigStringValue);
        propertyValue = AddNewProperty( propertyValues,L"bigstring"); 
        propertyValue->SetValue(dataValue);

		dataValue = FdoDataValue::Create(bigStringValue);
        propertyValue = AddNewProperty( propertyValues,L"bigchars"); 
        propertyValue->SetValue(dataValue);

        {
		myReader = insertCommand->Execute();

		CPPUNIT_ASSERT(myReader != NULL);
		// verify that the identity properties have the right values
		while (myReader->ReadNext())
		{
			CPPUNIT_ASSERT((int)(myReader->GetDouble(L"id")) == 12);
			CPPUNIT_ASSERT( FdoStringP(myReader->GetString(L"string")).ICompare(L"CANADA") == 0);
			FdoDateTime dateTime = myReader->GetDateTime(L"install_date");
			CPPUNIT_ASSERT(dateTime.year == 2006);
			CPPUNIT_ASSERT(dateTime.month == 11);
			CPPUNIT_ASSERT(dateTime.day == 18);
			CPPUNIT_ASSERT(dateTime.hour == 12);
			CPPUNIT_ASSERT(dateTime.minute == 45);
			CPPUNIT_ASSERT(dateTime.seconds == 15);
		}
		myReader->Close();
		}

        {
        selectCommand = (FdoISelect *) connection->CreateCommand(FdoCommandType_Select);
        selectCommand->SetFeatureClassName(L"device_tbl");

        myReader = selectCommand->Execute();
		// verify that bigstring has the right values
		while (myReader->ReadNext())
		{
			CPPUNIT_ASSERT( FdoStringP(myReader->GetString(L"bigstring")).ICompare(bigStringValue) == 0);
			CPPUNIT_ASSERT( FdoStringP(myReader->GetString(L"bigchars")).ICompare(bigStringValue) == 0);
		}
		myReader->Close();
        }

		connection->Close();
	}
	catch (FdoCommandException *ex)
	{
		CPPUNIT_FAIL (UnitTestUtil::w2a(ex->GetExceptionMessage()));
	}
   	catch (FdoException *ex)
	{
		CPPUNIT_FAIL (UnitTestUtil::w2a(ex->GetExceptionMessage()));
	}
}


