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
#include "MySqlFdoInsertTest.h"
#include "UnitTestUtil.h"
#include "ConnectionUtil.h"
#include "MySqlConnectionUtil.h"

#define UNSIGNED_SUFFIX L"_unsigned"
#define CHARSET_SUFFIX L"_charset"

CPPUNIT_TEST_SUITE_REGISTRATION( MySqlFdoInsertTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( MySqlFdoInsertTest, "FdoInsertTest");

void MySqlFdoInsertTest::set_provider()
{
	UnitTestUtil::SetProvider( "MySql" );
}

void MySqlFdoInsertTest::insertDate ()
{
    wprintf(L"InsertDate: Test insert command with different date/time settings.\n");
    FdoPtr<FdoIConnection> connection = UnitTestUtil::GetConnection(mSuffix, true);

    int         baseColorIndex    = 990,
                numberTestCases   = 9;
    bool        exceptionExpected = false;
    FdoStringP  colorIndex;
    FdoDateTime dateTime;

    for (int j = 0; j < numberTestCases; j++)
    {
        // Reinitialize the date-time object as defined by the constructor.

        dateTime.year    = -1;
        dateTime.month   = -1;
        dateTime.day     = -1;
        dateTime.hour    = -1;
        dateTime.minute  = -1;
        dateTime.seconds = 0.0f;;

        colorIndex = FdoStringP::Format(L"%d", (baseColorIndex+j));
        switch (j)
        {
            case 0:
                wprintf(L"  > (1) No date or time specification: should fail.\n");
                exceptionExpected = true;
                break;

            case 1:
                wprintf(L"  > (2)Complete date, but no time specification: should work.\n");
                dateTime.year    = 2003;
                dateTime.month   = 1;
                dateTime.day     = 1;
                exceptionExpected = false;
                break;

            case 2:
                wprintf(L"  > (3) Complete time, but no date specification: should work.\n");
                dateTime.hour    = 1;
                dateTime.minute  = 11;
                dateTime.seconds = 0;
                exceptionExpected = false;
                break;

            case 3:
                wprintf(L"  > (4) Complete date and time specification: should work.\n");
                dateTime.year    = 2003;
                dateTime.month   = 1;
                dateTime.day     = 1;
                dateTime.hour    = 1;
                dateTime.minute  = 11;
                dateTime.seconds = 11;
                exceptionExpected = false;
                break;

            case 4:
                wprintf(L"  > (5) Partial date, but no time specification: should fail.\n");
                dateTime.year     = 2003;
                dateTime.day      = 1;
                exceptionExpected = true;
                break;

            case 5:
                wprintf(L"  > (6) Partial time, but no date specification: should fail.\n");
                dateTime.hour     = 1;
                dateTime.seconds  = 0;
                exceptionExpected = true;
                break;

            case 6:
                wprintf(L"  > (7) Partial date and time specification: should fail.\n");
                dateTime.year     = 2003;
                dateTime.day      = 1;
                dateTime.hour     = 1;
                dateTime.seconds  = 0;
                exceptionExpected = true;
                break;

            case 7:
                wprintf(L"  > (8) Partial date and complete time specification: should fail.\n");
                dateTime.year     = 2003;
                dateTime.day      = 1;
                dateTime.hour     = 1;
                dateTime.minute   = 11;
                dateTime.seconds  = 0;
                exceptionExpected = true;
                break;

            case 8:
                wprintf(L"  > (9) Partial time and complete date specification: should fail.\n");
                dateTime.year     = 2003;
                dateTime.month    = 1;
                dateTime.day      = 1;
                dateTime.hour     = 1;
                dateTime.seconds  = 0;
                exceptionExpected = true;
                break;

        }

        // Attempt to insert a record using the constructed date-time object.
        try
        {
            FdoInsertTest::insertDate(connection, dateTime, colorIndex);
        }

        catch (FdoException *ex)
        {
            if (!exceptionExpected)
            {
                wprintf(L"    Unexpected Exception: %ls\n", ex->GetExceptionMessage());
                connection->Close();
                throw ex;
            }

            wchar_t expectedErrMsg[] = L"Incomplete date/time setting. ";
            FdoString *excMsg = ex->GetExceptionMessage();
            // eliminate a space from the end if is the case
            if (excMsg[wcslen(excMsg) - 1] != L' ')
                expectedErrMsg[wcslen(expectedErrMsg)-1] = L'\0';

            if ((wcscmp(excMsg, expectedErrMsg)) != 0)
            {
                wprintf(L"    Unexpected Exception: %ls\n", ex->GetExceptionMessage());
                connection->Close();
                throw ex;
            }
            else
                wprintf(L"    Expected Exception: %ls\n", ex->GetExceptionMessage());
        }

        catch ( ... )
        {
            wprintf(L"    Unexpected Exception in insertDate()\n");
            connection->Close();
            throw;
        }

    }

    wprintf(L"  > Test Verification.\n");
    insertDateVerification(connection, 3);

    wprintf(L"  > Test Cleanup.\n");
    FdoInsertTest::insertDateCleanUp(connection);

    connection->Close();
}

void MySqlFdoInsertTest::insertBoundaryUnsigned()
{
    StaticConnection* conn = new MySqlStaticConnection();

    try {

        UnitTestUtil::SetProvider( conn->GetServiceName() ); 

        conn->connect();

        FdoSchemaManagerP mgr = conn->CreateSchemaManager();

        FdoSmPhMgrP phMgr = mgr->GetPhysicalSchema();

        FdoStringP datastore = phMgr->GetDcOwnerName(
            UnitTestUtil::GetEnviron("datastore", UNSIGNED_SUFFIX)
        );

        FdoSmPhDatabaseP database = phMgr->GetDatabase();

        FdoSmPhOwnerP owner = phMgr->FindOwner( datastore, L"", false );
        if ( owner ) {
            owner->SetElementState( FdoSchemaElementState_Deleted );
            owner->Commit();
        }

        owner = database->CreateOwner(
            datastore, 
            false
        );
        owner->SetPassword( L"test" );

        FdoStringP tableName = L"unsigned_test";
            
        FdoSmPhTableP table = owner->CreateTable( tableName );
        table->SetPkeyName( tableName + L"_key" );
        FdoSmPhColumnP column = table->CreateColumnInt32(
            L"id",
            false
        );
        table->AddPkeyCol( column->GetName() );
        column = table->CreateColumnUnknown(
            L"uint_column",
            L"int unsigned",
            false,
            0,
            0
        );
        owner->Commit();
        
        phMgr = NULL;
        mgr = NULL;
        conn->disconnect();
        delete conn;

        FdoPtr<FdoIConnection> connection = UnitTestUtil::GetConnection(UNSIGNED_SUFFIX, false);
        FdoPtr<FdoITransaction> featureTransaction = connection->BeginTransaction();
        FdoIInsert *insertCommand = (FdoIInsert *) connection->CreateCommand(FdoCommandType_Insert);
        insertCommand->SetFeatureClassName(tableName);
        FdoPtr<FdoPropertyValueCollection> propertyValues = insertCommand->GetPropertyValues();

        FdoPtr<FdoDataValue> dataValue;
        dataValue = FdoDataValue::Create(L"1");
        FdoPtr<FdoPropertyValue> propertyValue = AddNewProperty( propertyValues, L"id");
        propertyValue->SetValue(dataValue);

        dataValue = FdoDataValue::Create(L"0");
        propertyValue = AddNewProperty( propertyValues, L"uint_column");
        propertyValue->SetValue(dataValue);

        FdoPtr<FdoIFeatureReader> reader = insertCommand->Execute();

        dataValue = FdoDataValue::Create(L"2");
        propertyValue = AddNewProperty( propertyValues, L"id");
        propertyValue->SetValue(dataValue);

        dataValue = FdoDataValue::Create(L"4294967295");
        propertyValue = AddNewProperty( propertyValues, L"uint_column");
        propertyValue->SetValue(dataValue);

        reader = insertCommand->Execute();

        featureTransaction->Commit();
        insertCommand->Release();

        // check 
    	FdoISelect* selectCmd = (FdoISelect *) connection->CreateCommand(FdoCommandType_Select);
	    selectCmd->SetFeatureClassName(tableName);

    	FdoPtr<FdoIFeatureReader> featureReader = selectCmd->Execute();
        FdoInt32 rowCount = 0;

        while ( featureReader->ReadNext() ) {
            rowCount++;

            switch ( featureReader->GetInt32(L"id") ) {
            case 1:
                CPPUNIT_ASSERT ( featureReader->GetInt64(L"uint_column") == 0 );
                break;

            case 2:
                CPPUNIT_ASSERT ( featureReader->GetInt64(L"uint_column") == 4294967295LL);
                break;
            }
        }

        CPPUNIT_ASSERT( rowCount == 2 );    
    }
    catch (FdoCommandException *ex)
    {
        UnitTestUtil::FailOnException(ex);
    }
    catch (FdoException *ex)
    {
        UnitTestUtil::FailOnException(ex);
    }
}

void MySqlFdoInsertTest::characterSets()
{
    StaticConnection* conn = new MySqlStaticConnection();

    try {

        UnitTestUtil::SetProvider( conn->GetServiceName() ); 

        conn->connect();

        FdoSchemaManagerP mgr = conn->CreateSchemaManager();

        FdoSmPhMgrP phMgr = mgr->GetPhysicalSchema();

        FdoStringP datastore = phMgr->GetDcOwnerName(
            UnitTestUtil::GetEnviron("datastore", CHARSET_SUFFIX)
        );

        FdoSmPhDatabaseP database = phMgr->GetDatabase();

        FdoSmPhOwnerP owner = phMgr->FindOwner( datastore, L"", false );
        if ( owner ) {
            owner->SetElementState( FdoSchemaElementState_Deleted );
            owner->Commit();
        }

        owner = database->CreateOwner(
            datastore, 
            false
        );
        owner->SetPassword( L"test" );
        owner->Commit();
        owner->SetCurrent();

        GdbiConnection* gdbiConnection = phMgr->SmartCast<FdoSmPhGrdMgr>()->GetGdbiConnection();

        // Create tables for various combinations of character set and types of characters
        // that will be stored in the table. 
        charSetCreateTable( gdbiConnection, L"latin1", L"ascii7" );
        charSetCreateTable( gdbiConnection, L"latin1", L"8bit" );
        charSetCreateTable( gdbiConnection, L"utf8", L"ascii7" );
        charSetCreateTable( gdbiConnection, L"utf8", L"8bit" );
        charSetCreateTable( gdbiConnection, L"utf8", L"japan" );
        charSetCreateTable( gdbiConnection, L"ucs2", L"ascii7" );
        charSetCreateTable( gdbiConnection, L"ucs2", L"8bit" );
        charSetCreateTable( gdbiConnection, L"ucs2", L"japan" );
        charSetCreateTable( gdbiConnection, L"cp932", L"ascii7" );
        charSetCreateTable( gdbiConnection, L"cp932", L"japan" );

        // A couple of combinations are not tried:
        //  - latin11 character set, Japanese characters
        //  = cp932 character set, 8 bit characters
        // since these are expected to fail (characters not in character set).

        FdoPtr<FdoIConnection> connection = UnitTestUtil::GetConnection(CHARSET_SUFFIX, false);

        // Round-trip insert/select test each table.
        charSetTestTable( connection, L"latin1", L"ascii7", L"abc", L"a" );
        charSetTestTable( connection, L"latin1", L"8bit", L"\x00e4\x00e5", L"\x00e4" );
        charSetTestTable( connection, L"utf8", L"ascii7", L"abc", L"a" );
        charSetTestTable( connection, L"utf8", L"8bit", L"\x00e4\x00e5", L"\x00e4" );
        charSetTestTable( connection, L"utf8", L"japan", L"\x30b0\x30b1", L"\x30b0" );
        charSetTestTable( connection, L"ucs2", L"ascii7", L"abc", L"a" );
        charSetTestTable( connection, L"ucs2", L"8bit", L"\x00e4\x00e5", L"\x00e4" );
        charSetTestTable( connection, L"ucs2", L"japan", L"\x30b0\x30b1", L"\x30b0" );
        charSetTestTable( connection, L"cp932", L"ascii7", L"abc", L"a" );
        charSetTestTable( connection, L"cp932", L"japan", L"\x30b0\x30b1", L"\x30b0" );
    }
    catch (FdoCommandException *ex)
    {
        UnitTestUtil::FailOnException(ex);
    }
    catch (FdoException *ex)
    {
        UnitTestUtil::FailOnException(ex);
    }
}

void MySqlFdoInsertTest::charSetCreateTable( GdbiConnection* gdbiConnection, FdoString* charSet, FdoString* dataCharSet )
{
    FdoStringP sqlStmt = FdoStringP::Format( 
        L"create table %ls_%ls ( data varchar(10) not null, chardata char(1), primary key (data)) character set %ls collate %ls_bin",
        charSet,
        dataCharSet,
        charSet,
        charSet
    );

    gdbiConnection->ExecuteNonQuery( (FdoString*) sqlStmt, true );
}

void MySqlFdoInsertTest::charSetTestTable( FdoIConnection* connection, FdoString* charSet, FdoString* dataCharSet, FdoString* stringVal, FdoString* charVal )
{
    FdoStringP className = FdoStringP::Format( L"%ls_%ls", charSet, dataCharSet );

    FdoPtr<FdoIInsert> insertCommand = (FdoIInsert *) connection->CreateCommand(FdoCommandType_Insert);
    FdoPtr<FdoPropertyValueCollection> propertyValues;
    FdoPtr<FdoDataValue> dataValue;
    FdoPtr<FdoPropertyValue> propertyValue;

    insertCommand->SetFeatureClassName(className);

    propertyValues = insertCommand->GetPropertyValues();
    propertyValue =  FdoPropertyValue::Create();
    propertyValue->SetName( L"data" );
    propertyValues->Add( propertyValue );

    dataValue = FdoDataValue::Create(stringVal);
    propertyValue->SetValue(dataValue);

    propertyValue =  FdoPropertyValue::Create();
    propertyValue->SetName( L"chardata" );
    propertyValues->Add( propertyValue );

    dataValue = FdoDataValue::Create(charVal);
    propertyValue->SetValue(dataValue);

    insertCommand->Execute();

  	FdoPtr<FdoISelect>selCmd = (FdoISelect*)connection->CreateCommand( FdoCommandType_Select );
    selCmd->SetFeatureClassName(className);

    FdoPtr<FdoIFeatureReader>myReader = selCmd->Execute();
    CPPUNIT_ASSERT( myReader->ReadNext() );
    charSetVldValue( className, stringVal, myReader->GetString(L"data") );
    charSetVldValue( className, charVal, myReader->GetString(L"chardata") );

    CPPUNIT_ASSERT( !(myReader->ReadNext()) );

}

void MySqlFdoInsertTest::charSetVldValue( FdoString* className, FdoString* expectedVal, FdoString* val )
{
    if ( wcscmp(expectedVal, val) != 0 )
    {
        FdoStringP msg = FdoStringP::Format( 
            L"Select failed: %ls: Unexpected value '%ls'", 
            (FdoString*) className,
            val 
        );

        printf( "%ls\n", (FdoString*) msg );
 
        printf( "   selValue %d: ", wcslen(val) );
        for ( int j = 0; j < wcslen(val); j++ ) 
            printf( "%x ", val[j] );

        printf( "\n" );

        CPPUNIT_FAIL( (const char*) msg );
    }
}
 
