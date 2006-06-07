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
#include "BigPerformanceTests.h"

#include <FdoSpatial.h>
#include <FdoCommonFile.h>

#ifdef _WIN32
#define LOCATION L"..\\..\\TestData\\Sheboygan"
#else
#define LOCATION L"../../TestData/Sheboygan"
#endif

CPPUNIT_TEST_SUITE_REGISTRATION (BigPerformanceTests);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION (BigPerformanceTests, "BigPerformanceTests");

FdoPtr<FdoIConnection> BigPerformanceTests::mConnection;

BigPerformanceTests::BigPerformanceTests (void)
{
}

BigPerformanceTests::~BigPerformanceTests (void)
{
}

void BigPerformanceTests::setUp ()
{
    mConnection = ShpTests::GetConnection ();
    mConnection->SetConnectionString (L"DefaultFileLocation=" LOCATION);
    CPPUNIT_ASSERT_MESSAGE ("connection state not open", FdoConnectionState_Open == mConnection->Open ());
}

void BigPerformanceTests::tearDown ()
{
    mConnection->Close ();
}

void BigPerformanceTests::select ()
{
    clock_t start;
    clock_t restart;
    clock_t finish;
    int count;

    try
    {
        start = clock ();
        FdoPtr<FdoIDescribeSchema> describe = (FdoIDescribeSchema*)mConnection->CreateCommand (FdoCommandType_DescribeSchema);
        FdoPtr<FdoFeatureSchemaCollection> schemas = describe->Execute ();
        for (int i = 0; i < schemas->GetCount (); i++)
        {
            FdoPtr<FdoFeatureSchema> schema = schemas->GetItem (i);
            FdoPtr<FdoClassCollection> classes = schema->GetClasses ();
            FdoPtr<FdoISelect> select = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
            for (int j = 0; j < classes->GetCount (); j++)
            {
                FdoPtr<FdoClassDefinition> cls = classes->GetItem (j);
                restart = clock ();
                select->SetFeatureClassName (cls->GetName ());
                FdoPtr<FdoIFeatureReader> reader = select->Execute ();
                count = 0;
                while (reader->ReadNext ())
                {
                    FdoPtr<FdoByteArray> geometry = reader->GetGeometry (L"Geometry");
                    count++;
                }
                finish = clock ();
                if (VERBOSE)
                    printf ("%d features from class %ls in %2.3f seconds\n", count, cls->GetName (), (double)(finish - restart) / CLOCKS_PER_SEC);
            }
        }
        finish = clock ();
        if (VERBOSE)
            printf ("%2.3f seconds\n", (double)(finish - start) / CLOCKS_PER_SEC);
    }
    catch (FdoException* ge)
    {
        ShpTests::fail (ge);
    }
}

