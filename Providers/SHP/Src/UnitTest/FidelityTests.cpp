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
#include "FidelityTests.h"

#include <FdoCommonFile.h>

#ifdef _WIN32
#define LOCATION L"..\\..\\TestData\\Testing"
#define SCHEMA_NAME L"\\schema.xml"
#else
#define LOCATION L"../../TestData/Testing"
#define SCHEMA_NAME L"/schema.xml"
#endif

CPPUNIT_TEST_SUITE_REGISTRATION (FidelityTests);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION (FidelityTests, "FidelityTests");

FdoPtr<FdoIConnection> FidelityTests::mConnection;

FidelityTests::FidelityTests (void)
{
}

FidelityTests::~FidelityTests (void)
{
}

void FidelityTests::setUp ()
{
    try
    {
        if (!FdoCommonFile::FileExists (LOCATION))
            FdoCommonFile::MkDir (LOCATION);
        mConnection = ShpTests::GetConnection ();
        mConnection->SetConnectionString (L"DefaultFileLocation=" LOCATION);
		ShpTests::sLocation = LOCATION;
        CPPUNIT_ASSERT_MESSAGE ("connection state not open", FdoConnectionState_Open == mConnection->Open ());
    }
    catch (FdoException* ge)
    {
        TestCommonFail (ge);
    }
}

void FidelityTests::tearDown ()
{
    try
    {
        // delete the class, if its there:
        TestCommonSchemaUtil::CleanUpClass(mConnection, NULL, L"Test");
        TestCommonSchemaUtil::CleanUpClass(mConnection, NULL, L"coundist01_n83");

        mConnection->Close ();
		FDO_SAFE_RELEASE(mConnection.p);

    if (FdoCommonFile::FileExists (LOCATION SCHEMA_NAME))
        FdoCommonFile::Delete (LOCATION SCHEMA_NAME);
        if (FdoCommonFile::FileExists (LOCATION))
            FdoCommonFile::RmDir (LOCATION);
    }
    catch (FdoException *ge)
    {
        TestCommonFail (ge);
    }
}

void FidelityTests::compare (const wchar_t* target_class_name, const wchar_t* file_name, const wchar_t* extension, int skip)
{
    wchar_t* location;
    wchar_t separator;
    wchar_t* class_name;
    const wchar_t* format = L"%ls%lc%ls%ls";
    wchar_t* target;
    wchar_t* reference;
    size_t length;

    // split into location and class name
    location = (wchar_t*)alloca (sizeof(wchar_t) * (wcslen (file_name) + 1));
    wcscpy (location, file_name);
    separator = L'\\';
    class_name = wcsrchr (location, L'\\');
    if (NULL == class_name)
    {
        separator = L'/';
        class_name = wcsrchr (location, separator);
    }
    *class_name = L'\0';
    class_name++;

    length = sizeof(wchar_t) * (wcslen (location) + wcslen (format) + wcslen (class_name) + wcslen (extension));
    target = (wchar_t*)alloca (length);
    FdoCommonOSUtil::swprintf (target, length, format, location, separator, class_name, extension);
    length = sizeof(wchar_t) * (wcslen (LOCATION) + wcslen (format) + wcslen (target_class_name) + wcslen (extension));
    reference = (wchar_t*)alloca (length);
    FdoCommonOSUtil::swprintf (reference, length, format, LOCATION, separator, target_class_name, extension);
    
    TestCommonFileUtil::CompareFiles (target, reference, skip);
}

void FidelityTests::ontario ()
{
    #ifdef _WIN32
    const wchar_t* file_name = L"..\\..\\TestData\\Ontario\\ontario";
    #else
    const wchar_t* file_name =  L"../../TestData/Ontario/ontario";
    #endif

    try
    {
        copy (mConnection, L"Test", file_name);
        mConnection->Close ();
        compare (L"Test", file_name, L".shp", 100);
        //compare (L"Test", file_name, L".dbf", 32); // floating point differences linux-win32
        CPPUNIT_ASSERT_MESSAGE ("connection state not open", FdoConnectionState_Open == mConnection->Open ());
    }
    catch (FdoException* ge)
    {
        TestCommonFail (ge);
    }
}

void FidelityTests::lakes ()
{
    #ifdef _WIN32
    const wchar_t* file_name = L"..\\..\\TestData\\Ontario\\lakes";
    #else
    const wchar_t* file_name =  L"../../TestData/Ontario/lakes";
    #endif

    try
    {
        copy (mConnection, L"Test", file_name);
        mConnection->Close ();
        compare (L"Test", file_name, L".shp", 100);
        //compare (L"Test", file_name, L".dbf", 4); // too many justification differences in AREA
        CPPUNIT_ASSERT_MESSAGE ("connection state not open", FdoConnectionState_Open == mConnection->Open ());
    }
    catch (FdoException* ge)
    {
        TestCommonFail (ge);
    }
}

void FidelityTests::nanboku ()
{
    #ifdef _WIN32
    const wchar_t* file_name = L"..\\..\\TestData\\Nanboku\\Nanboku_3dLine";
    #else
    const wchar_t* file_name =  L"../../TestData/Nanboku/Nanboku_3dLine";
    #endif

    try
    {
        copy (mConnection, L"Test", file_name);
        mConnection->Close ();
        compare (L"Test", file_name, L".shp", 100);
        compare (L"Test", file_name, L".dbf", 4); // past date
        CPPUNIT_ASSERT_MESSAGE ("connection state not open", FdoConnectionState_Open == mConnection->Open ());
    }
    catch (FdoException* ge)
    {
        TestCommonFail (ge);
    }
}

void FidelityTests::ignore_states ()
{
    #ifdef _WIN32
    const wchar_t* file_name = L"..\\..\\TestData\\Oahu\\coundist01_n83";
    #else
    const wchar_t* file_name =  L"../../TestData/Oahu/coundist01_n83";
    #endif

    try
    {
        copy (mConnection, L"coundist01_n83", file_name, true);
        mConnection->Close ();
        compare (L"coundist01_n83", file_name, L".shp", 100);
        compare (L"coundist01_n83", file_name, L".dbf", 224); // past weird bytes in reserved Field data address
        CPPUNIT_ASSERT_MESSAGE ("connection state not open", FdoConnectionState_Open == mConnection->Open ());
    }
    catch (FdoException* ge)
    {
        TestCommonFail (ge);
    }
}

