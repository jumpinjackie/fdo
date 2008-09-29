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
#include "OdbcFdoSqlCmdTest.h"
#include "UnitTestUtil.h"
#define  SQLCMD_8BIT_TABLE_NAME  L"FdöSqlTest"

#ifdef _WIN32
CPPUNIT_TEST_SUITE_REGISTRATION( OdbcSqlServerFdoSqlCmdTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcSqlServerFdoSqlCmdTest, "FdoSqlCmdTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcSqlServerFdoSqlCmdTest, "OdbcSqlServerFdoSqlCmdTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcSqlServerFdoSqlCmdTest, "OdbcSqlServerTests");

CPPUNIT_TEST_SUITE_REGISTRATION( OdbcAccessFdoSqlCmdTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcAccessFdoSqlCmdTest, "FdoSqlCmdTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcAccessFdoSqlCmdTest, "OdbcAccessFdoSqlCmdTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcAccessFdoSqlCmdTest, "OdbcAccessTests");

void OdbcSqlServerFdoSqlCmdTest::set_provider()
{
	UnitTestUtil::SetProvider( "OdbcSqlServer" );
}

FdoString * OdbcSqlServerFdoSqlCmdTest::GetConnectString()
{
	// here we use DSN, we can not call UnitTestUtil::GetConnectionString
	static wchar_t connectString[200];
	connectString[0] = L'\0';
    FdoStringP dsnname = UnitTestUtil::GetEnviron("dsnname");
    FdoStringP username = UnitTestUtil::GetEnviron("username");
    FdoStringP password = UnitTestUtil::GetEnviron("password");
	swprintf( 
        connectString, 
        sizeof(connectString)/sizeof(wchar_t), 
        L"DataSourceName=%ls;UserId=%ls;Password=%ls;", 
        (FdoString*) dsnname, 
        (FdoString*) username, 
        (FdoString*) password
    );
	return connectString;
}

void OdbcSqlServerFdoSqlCmdTest::connect ()
{
    try
    {
        mConnection = UnitTestUtil::GetProviderConnectionObject();
        mConnection->SetConnectionString(GetConnectString());
        mConnection->Open();
    }
    catch (FdoException *ex)
    {
        mConnection= NULL;
        TestCommonFail (ex);
    }
}

void OdbcSqlServerFdoSqlCmdTest::CreateAndDrop8bitTable ()
{
    FdoPtr<FdoISQLCommand> sqlCmd;

    if( mConnection != NULL )
    {
        try
        {
            sqlCmd = (FdoISQLCommand*)mConnection->CreateCommand( FdoCommandType_SQLCommand );

            wchar_t *number_type = get_number_type();
            FdoStringP sql = FdoStringP::Format(L"create table %ls (Name varchar(64) not null, id %ls(10,0), amount %ls(38,8), bool_val %ls(1), date_val %ls )", 
                                SQLCMD_8BIT_TABLE_NAME, number_type, number_type, number_type, get_datetime_type());
            sqlCmd->SetSQLStatement( sql );
            sqlCmd->ExecuteNonQuery();

            sqlCmd = (FdoISQLCommand*)mConnection->CreateCommand( FdoCommandType_SQLCommand );
            sql = FdoStringP::Format(L"drop table %ls", SQLCMD_8BIT_TABLE_NAME);
            sqlCmd->SetSQLStatement( (FdoString *)sql );
            sqlCmd->ExecuteNonQuery();
        }
        catch( FdoException *ex )
        {
			TestCommonFail(ex);
        }
    }

}

void OdbcAccessFdoSqlCmdTest::set_provider()
{
	UnitTestUtil::SetProvider( "OdbcAccess" );
}

void OdbcAccessFdoSqlCmdTest::EmployeesTest()
{
    if( mConnection != NULL )
    {
        try
        {
            FdoPtr<FdoISQLCommand> sqlCmd = (FdoISQLCommand*)mConnection->CreateCommand(FdoCommandType_SQLCommand);

            sqlCmd->SetSQLStatement(L"select distinct(JOBTITLE) from EMPLOYEES where Salary >= 40000.0");

            // execute the command
            FdoPtr<FdoISQLDataReader> reader = sqlCmd->ExecuteReader();

            // Exercise the metadata methods:
            FdoInt32 count = reader->GetColumnCount();
            for (FdoInt32 i=0; i<count; i++)
            {
                FdoString *colName = reader->GetColumnName(i);
                FdoDataType dataType = reader->GetColumnType(colName);
            }

            // read through all the features
            int numFeatures = 0;
            bool bFoundBoxFiller = false;
            bool bFoundBoxFlattener = false;
            bool bFoundBoxAssembler = false;
            bool bFoundBoxArtist = false;
            while (reader->ReadNext())
            {
                numFeatures++;

                // Check JobTitle:
                FdoString *jobTitle = reader->GetString(L"JOBTITLE");
                if (0==wcscmp(jobTitle, L"Box Filler"))
                    bFoundBoxFiller = true;
                else if (0==wcscmp(jobTitle, L"Box Flattener"))
                    bFoundBoxFlattener = true;
                else if (0==wcscmp(jobTitle, L"Box Assembler"))
                    bFoundBoxAssembler = true;
                else if (0==wcscmp(jobTitle, L"Box Artist"))
                    bFoundBoxArtist = true;
            }

            printf("   %i feature(s) read\n", numFeatures);

            CPPUNIT_ASSERT_MESSAGE("Didn't find 'Box Flattener' in results", bFoundBoxFlattener);
            CPPUNIT_ASSERT_MESSAGE("Didn't find 'Box Assembler' in results", bFoundBoxAssembler);
            CPPUNIT_ASSERT_MESSAGE("Didn't find 'Box Artist' in results", bFoundBoxArtist);

            // close the reader
            reader->Close();
        }
        catch (FdoException* e)
        {
            TestCommonFail (e);
        }
    }
}

void OdbcAccessFdoSqlCmdTest::Table1Test()
{
    if( mConnection != NULL )
    {
        try
        {
            FdoPtr<FdoISQLCommand> sqlCmd = (FdoISQLCommand*)mConnection->CreateCommand(FdoCommandType_SQLCommand);

            sqlCmd->SetSQLStatement(L"select NAME, X, Y from TABLE1");

            // execute the command
            FdoPtr<FdoISQLDataReader> reader = sqlCmd->ExecuteReader();

            // Exercise the metadata methods:
            FdoInt32 count = reader->GetColumnCount();
            for (FdoInt32 i=0; i<count; i++)
            {
                FdoString *colName = reader->GetColumnName(i);
                FdoDataType dataType = reader->GetColumnType(colName);
                FdoPropertyType propertyType = reader->GetPropertyType(colName);
            }

            // read through all the features
            int numFeatures = 0;
            while (reader->ReadNext())
            {
                numFeatures++;

                FdoString *name = reader->GetString(L"NAME");
                FdoInt32 x = reader->GetInt32(L"X");
                FdoInt32 y = reader->GetInt32(L"Y");
                printf("   name='%ls' (x,y)=(%d,%d)\n", name, x, y);
            }

            printf("   %i feature(s) read\n", numFeatures);

            // close the reader
            reader->Close();
        }
        catch (FdoException* e)
        {
            TestCommonFail (e);
        }
    }
}

void OdbcAccessFdoSqlCmdTest::connect ()
{
    try
    {
        mConnection = UnitTestUtil::GetProviderConnectionObject();
        mConnection->SetConnectionString(UnitTestUtil::GetConnectionString());
        mConnection->Open();
    }
    catch (FdoException *ex)
    {
        mConnection= NULL;
        TestCommonFail (ex);
    }
}
#endif
