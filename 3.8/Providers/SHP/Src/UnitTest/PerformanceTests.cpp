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
#include "PerformanceTests.h"

#include <FdoSpatial.h>
#include <FdoCommonFile.h>

#ifdef _WIN32
#define LOCATION L"..\\..\\TestData\\Florida"
#else
#define LOCATION L"../../TestData/Florida"
#endif

CPPUNIT_TEST_SUITE_REGISTRATION (PerformanceTests);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION (PerformanceTests, "PerformanceTests");

FdoPtr<FdoIConnection> PerformanceTests::mConnection;

PerformanceTests::PerformanceTests (void)
{
}

PerformanceTests::~PerformanceTests (void)
{
}

void PerformanceTests::setUp ()
{
    mConnection = ShpTests::GetConnection ();
    mConnection->SetConnectionString (L"DefaultFileLocation=" LOCATION);
    CPPUNIT_ASSERT_MESSAGE ("connection state not open", FdoConnectionState_Open == mConnection->Open ());
}

void PerformanceTests::tearDown ()
{
    mConnection->Close ();
	FDO_SAFE_RELEASE(mConnection.p);
}

void PerformanceTests::select ()
{
    clock_t start;
    clock_t finish;
    int count;

    try
    {
        start = clock ();
        FdoPtr<FdoISelect> select = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
        select->SetFeatureClassName (L"plss24");
        FdoPtr<FdoIFeatureReader> reader = select->Execute ();
        count = 0;
        while (reader->ReadNext ())
        {
            FdoPtr<FdoByteArray> geometry = reader->GetGeometry (L"Geometry");
            count++;
        }
        CPPUNIT_ASSERT_MESSAGE ("wrong number of plss24 features selected", 55772 == count);
        finish = clock ();
        if (VERBOSE)
            printf ("%d features in %2.3f seconds\n", count, (double)(finish - start) / CLOCKS_PER_SEC);
    }
    catch (FdoException* ge)
    {
        TestCommonFail (ge);
    }
}

