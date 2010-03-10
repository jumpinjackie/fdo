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
#include "PostGisFdoInsertTest.h"
#include "UnitTestUtil.h"
#include "ConnectionUtil.h"
#include "PostGisConnectionUtil.h"

#define UNSIGNED_SUFFIX L"_unsigned"

CPPUNIT_TEST_SUITE_REGISTRATION( PostGisFdoInsertTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( PostGisFdoInsertTest, "FdoInsertTest");

void PostGisFdoInsertTest::set_provider()
{
	UnitTestUtil::SetProvider( "PostGIS" );
}

void PostGisFdoInsertTest::insertDate ()
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
                // PostgreSQL timestamp must have date part. There doesn't appear to be a 
                // type that can accept date, date/time, or just time.
                wprintf(L"  > (3) Complete time, but no date specification: should fail.\n");
                dateTime.hour    = 1;
                dateTime.minute  = 11;
                dateTime.seconds = 0;
                exceptionExpected = true;
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

            FDO_SAFE_RELEASE(ex);
        }

        catch ( ... )
        {
            wprintf(L"    Unexpected Exception in insertDate()\n");
            connection->Close();
            throw;
        }

    }

    wprintf(L"  > Test Verification.\n");
    insertDateVerification(connection, 2);

    wprintf(L"  > Test Cleanup.\n");
    FdoInsertTest::insertDateCleanUp(connection);

    connection->Close();
}

FdoFloat PostGisFdoInsertTest::GetMinSingleValue()
{
    // PostgreSQL real has only 6 digits of precision
    return (FdoFloat) -3.40282e38;
}

FdoDouble PostGisFdoInsertTest::GetMinDoubleValue()
{
    return (FdoDouble) -1.7976931348623099e308;
}

FdoFloat PostGisFdoInsertTest::GetMaxSingleValue()
{
    // PostgreSQL real has only 6 digits of precision
    return (FdoFloat) 3.40282e38;
}

FdoDouble PostGisFdoInsertTest::GetMaxDoubleValue()
{
    return (FdoDouble) 1.7976931348623099e308;
}

FdoFloat PostGisFdoInsertTest::GetSmallestSingleValue()
{
    // PostgreSQL real has only 6 digits of precision
    return (FdoFloat) 1.17550e-38;
}


