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
#include "SQLTests.h"

CPPUNIT_TEST_SUITE_REGISTRATION (SQLTests);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION (SQLTests, "SQLTests");

FdoPtr<FdoIConnection> SQLTests::mConnection;

SQLTests::SQLTests (void)
{
}

SQLTests::~SQLTests (void)
{
}

void SQLTests::setUp ()
{
    if (!CreateSchemaOnly() /* || !bSchemaCreated */ )
    {
        mConnection = ArcSDETests::GetConnection ();
        mConnection->SetConnectionString (ArcSDETestConfig::ConnStringOzzie());
        mConnection->Open ();
    }
}

void SQLTests::tearDown ()
{
    if (mConnection)
        mConnection->Close ();
}

void SQLTests::non_query ()
{
    if (CreateSchemaOnly())  return;

    try
    {
        FdoPtr<FdoISQLCommand> sqlCmd = (FdoISQLCommand*)mConnection->CreateCommand (FdoCommandType_SQLCommand);
        sqlCmd->SetSQLStatement (L"create table foo (name varchar(32))");
        sqlCmd->ExecuteNonQuery ();
        sqlCmd->SetSQLStatement (L"drop table foo");
        sqlCmd->ExecuteNonQuery ();
    }
    catch (FdoException *e)
    {
        fail(e);
    }
}

void SQLTests::query ()
{
    if (CreateSchemaOnly())  return;

    try
    {
        FdoPtr<FdoISQLCommand> sqlCmd = (FdoISQLCommand*)mConnection->CreateCommand (FdoCommandType_SQLCommand);

        // Clean up previous tests (ignore exceptions, we'll get one if the table doesn't actually exist):
        try
        {
            sqlCmd->SetSQLStatement (L"drop table bar");
            sqlCmd->ExecuteNonQuery ();
        }
        catch (FdoException *e)
        {
            e->Release();
        }

        // Execute various tests:
        sqlCmd->SetSQLStatement (ArcSDETestConfig::SqlStmtCreateTable());
        sqlCmd->ExecuteNonQuery ();
        sqlCmd->SetSQLStatement (ArcSDETestConfig::SqlStmtInsert1());
        sqlCmd->ExecuteNonQuery ();
        sqlCmd->SetSQLStatement (L"select * from bar");
        {
            FdoPtr<FdoISQLDataReader> reader = sqlCmd->ExecuteReader ();

            // Test accessing SQLDataReader's metadata BEFORE calling ReadNext():
            IterateSQLDataReaderProperties(reader);

            reader->ReadNext ();
            CPPUNIT_ASSERT_MESSAGE ("int16 wrong", 42 == reader->GetInt16 (L"ID"));
            CPPUNIT_ASSERT_MESSAGE ("int32 wrong", 8272772 == reader->GetInt32 (L"COUNT"));
            float x = (float)reader->GetSingle (L"SCALE");
            float diff = x - 1e-2f;
            if (0 > diff)
                diff = -diff;
            CPPUNIT_ASSERT_MESSAGE ("float wrong", 1e-9 > diff);
            double y = reader->GetDouble (L"LENGTH");
            double difference = y - 10280288.29929;
            if (0 > difference)
                difference = -difference;
            CPPUNIT_ASSERT_MESSAGE ("double wrong", 1e-4 > difference);
            CPPUNIT_ASSERT_MESSAGE ("string wrong", 0 == wcscmp (L"the quick brown fox jumps over a lazy dog", reader->GetString(L"DESCRIPTION")));

            FdoPtr<FdoBLOBValue> blobValue = static_cast<FdoBLOBValue*>(reader->GetLOB(L"DATA"));
            FdoByteArray* data = blobValue->GetData();
            FdoByte test[] = { 0x25, 0x2f, 0x82, 0xe3 };
            for (int i = 0; i < data->GetCount (); i++)
                CPPUNIT_ASSERT_MESSAGE ("blob wrong", test[i] == (*data)[i]);
            data->Release ();

            FdoDateTime now = reader->GetDateTime (L"MODIFIED");
            struct tm systime;
#ifdef _WIN32
#pragma warning(disable : 4996)
            _getsystime (&systime);
#pragma warning(default : 4996)
#else
            time_t current;
            time (&current);
            localtime_r (&current, &systime);
#endif
            CPPUNIT_ASSERT_MESSAGE ("year wrong (NOTE: THIS IS MAY BE CAUSED BY TIME ZONE DIFFERENCE BETWEEN SERVER AND CLIENT MACHINES)", now.year == systime.tm_year + 1900);
            CPPUNIT_ASSERT_MESSAGE ("month wrong (NOTE: THIS IS MAY BE CAUSED BY TIME ZONE DIFFERENCE BETWEEN SERVER AND CLIENT MACHINES)", now.month == systime.tm_mon + 1);
            CPPUNIT_ASSERT_MESSAGE ("day wrong (NOTE: THIS IS OFTEN CAUSED BY TIME ZONE DIFFERENCE BETWEEN SERVER AND CLIENT MACHINES)", now.day == systime.tm_mday);
            //THE FOLLOWING TEST FAILS FREQUENTLY, SINCE THE CLIENT'S TIME AND SERVER'S TIME ARE USUALLY NOT IN SYNC:
            //CPPUNIT_ASSERT_MESSAGE ("hour wrong", now.hour == systime.tm_hour);
        }

        // Test aggregate functions in select clause:
        sqlCmd->SetSQLStatement (ArcSDETestConfig::SqlStmtInsert2());
        sqlCmd->ExecuteNonQuery ();
        sqlCmd->SetSQLStatement (L"select count(*) from bar");
        FdoPtr<FdoISQLDataReader> reader = sqlCmd->ExecuteReader();
        CPPUNIT_ASSERT_MESSAGE("Expected 1 row, got 0 rows instead.", reader->ReadNext ());
        FdoInt32 colCount = reader->GetColumnCount();
        CPPUNIT_ASSERT_MESSAGE ("column count wrong", colCount == 1);
        FdoString *colName = reader->GetColumnName(0);
        CPPUNIT_ASSERT_MESSAGE ("column name wrong", 0==FdoCommonOSUtil::wcsicmp(colName, ArcSDETestConfig::SqlCountStarColumnName()));
        FdoDataType colType = reader->GetColumnType(colName);
        if (colType==FdoDataType_Int32)
        {
            FdoInt32 iRowCount = reader->GetInt32(colName);
            CPPUNIT_ASSERT_MESSAGE("count(*) value wrong", iRowCount == 2);
        }
        else if (colType==FdoDataType_Double)
        {
            double dRowCount = reader->GetDouble(colName);
            CPPUNIT_ASSERT_MESSAGE("count(*) value wrong", dRowCount == 2);
        }
        CPPUNIT_ASSERT_MESSAGE("Expected 1 row, got 2 or more rows instead.", !reader->ReadNext ());

        // Test functions in where clause:
        sqlCmd->SetSQLStatement (ArcSDETestConfig::SqlStmtAggrQuery1());
        reader = sqlCmd->ExecuteReader();
        CPPUNIT_ASSERT_MESSAGE("Expected 1 row, got 0 rows instead.", reader->ReadNext ());
        colCount = reader->GetColumnCount();
        CPPUNIT_ASSERT_MESSAGE ("column count wrong", colCount == 1);
        colName = reader->GetColumnName(0);
        CPPUNIT_ASSERT_MESSAGE ("column name wrong", 0==FdoCommonOSUtil::wcsicmp(colName, ArcSDETestConfig::SqlAggrColumnName()));
        colType = reader->GetColumnType(colName);
        CPPUNIT_ASSERT_MESSAGE ("column type wrong", colType==FdoDataType_Double);
        double dResult = reader->GetDouble(colName);
        CPPUNIT_ASSERT_MESSAGE("ABS(SUM(length)) value wrong", ArcSDETests::fuzzyEqual(dResult, ArcSDETestConfig::SqlStmtAggrQuery1Result()));
        CPPUNIT_ASSERT_MESSAGE("Expected 1 row, got 2 or more rows instead.", !reader->ReadNext ());


        // Clean up:
        sqlCmd->SetSQLStatement (L"drop table bar");
        sqlCmd->ExecuteNonQuery ();
    }
    catch (FdoException *e)
    {
        fail(e);
    }
}



void SQLTests::IterateSQLDataReaderProperties(FdoISQLDataReader* sqlDataReader)
{
    FdoInt32   colCount = sqlDataReader->GetColumnCount();
    for (FdoInt32 i=0; i<colCount; i++)
    {
        FdoString *colName = sqlDataReader->GetColumnName(i);
        FdoDataType colType = sqlDataReader->GetColumnType(colName);
    }
}
