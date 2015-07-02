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
#include "ComparisonTests.h"

#include <FdoSpatial.h>
#include <FdoCommonFile.h>

#ifdef _WIN32
#define LOCATION L"..\\..\\TestData\\Sheboygan"
#define CLASS1 L"\\Trees.sdf"
#define CLASS2 L"\\Parcels.sdf"
#else
#define LOCATION L"../../TestData/Sheboygan"
#define CLASS1 L"/Trees.sdf"
#define CLASS2 L"/Parcels.sdf"
#endif

CPPUNIT_TEST_SUITE_REGISTRATION (ComparisonTests);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION (ComparisonTests, "ComparisonTests");

ComparisonTests::ComparisonTests (void)
{
}

ComparisonTests::~ComparisonTests (void)
{
}

void ComparisonTests::setUp ()
{
}

void ComparisonTests::tearDown ()
{
}

void ComparisonTests::compare1 ()
{
    clock_t start;
    clock_t finish;
    int count;

    try
    {
        printf ("Trees\n");
        {
            // sdf
            FdoPtr<IConnectionManager> manager = FdoFeatureAccessManager::GetConnectionManager ();
            FdoPtr<FdoIConnection> connection = manager->CreateConnection (L"OSGeo.SDF");
            connection->SetConnectionString (L"File=" LOCATION CLASS1);
            CPPUNIT_ASSERT_MESSAGE ("connection state not open", FdoConnectionState_Open == connection->Open ());
            start = clock ();
            FdoPtr<FdoISelect> select = (FdoISelect*)connection->CreateCommand (FdoCommandType_Select);
            select->SetFeatureClassName (L"Trees");
            FdoPtr<FdoIFeatureReader> reader = select->Execute ();
            count = 0;
            while (reader->ReadNext ())
            {
                FdoPtr<FdoByteArray> geometry = reader->GetGeometry (L"SHPGEOM");
                count++;
            }
            finish = clock ();
            printf ("Sdf: %d features in %2.3f seconds\n", count, (double)(finish - start) / CLOCKS_PER_SEC);
            connection->Close ();
        }
        {
            // shp
            FdoPtr<FdoIConnection> connection = ShpTests::GetConnection ();
            connection->SetConnectionString (L"DefaultFileLocation=" LOCATION);
            CPPUNIT_ASSERT_MESSAGE ("connection state not open", FdoConnectionState_Open == connection->Open ());
            start = clock ();
            FdoPtr<FdoISelect> select = (FdoISelect*)connection->CreateCommand (FdoCommandType_Select);
            select->SetFeatureClassName (L"Trees");
            FdoPtr<FdoIFeatureReader> reader = select->Execute ();
            count = 0;
            while (reader->ReadNext ())
            {
                FdoPtr<FdoByteArray> geometry = reader->GetGeometry (L"Geometry");
                count++;
            }
            finish = clock ();
            printf ("Shp: %d features in %2.3f seconds\n", count, (double)(finish - start) / CLOCKS_PER_SEC);
            connection->Close ();
        }

    }
    catch (FdoException* ge)
    {
        TestCommonFail (ge);
    }
}

void ComparisonTests::compare2 ()
{
    clock_t start;
    clock_t finish;
    int count;

    try
    {
        printf ("Parcels\n");
        {
            // sdf
            FdoPtr<IConnectionManager> manager = FdoFeatureAccessManager::GetConnectionManager ();
            FdoPtr<FdoIConnection> connection = manager->CreateConnection (L"OSGeo.SDF");
            connection->SetConnectionString (L"File=" LOCATION CLASS2);
            CPPUNIT_ASSERT_MESSAGE ("connection state not open", FdoConnectionState_Open == connection->Open ());
            start = clock ();
            FdoPtr<FdoISelect> select = (FdoISelect*)connection->CreateCommand (FdoCommandType_Select);
            select->SetFeatureClassName (L"Parcels");
            FdoPtr<FdoIFeatureReader> reader = select->Execute ();
            count = 0;
            while (reader->ReadNext ())
            {
                FdoPtr<FdoByteArray> geometry = reader->GetGeometry (L"SHPGEOM");
                count++;
            }
            finish = clock ();
            printf ("Sdf: %d features in %2.3f seconds\n", count, (double)(finish - start) / CLOCKS_PER_SEC);
            connection->Close ();
        }
        {
            // shp
            FdoPtr<FdoIConnection> connection = ShpTests::GetConnection ();
            connection->SetConnectionString (L"DefaultFileLocation=" LOCATION);
            CPPUNIT_ASSERT_MESSAGE ("connection state not open", FdoConnectionState_Open == connection->Open ());
            start = clock ();
            FdoPtr<FdoISelect> select = (FdoISelect*)connection->CreateCommand (FdoCommandType_Select);
            select->SetFeatureClassName (L"Parcels");
            FdoPtr<FdoIFeatureReader> reader = select->Execute ();
            count = 0;
            while (reader->ReadNext ())
            {
                FdoPtr<FdoByteArray> geometry = reader->GetGeometry (L"Geometry");
                count++;
            }
            finish = clock ();
            printf ("Shp: %d features in %2.3f seconds\n", count, (double)(finish - start) / CLOCKS_PER_SEC);
            connection->Close ();
        }

    }
    catch (FdoException* ge)
    {
        TestCommonFail (ge);
    }
}

