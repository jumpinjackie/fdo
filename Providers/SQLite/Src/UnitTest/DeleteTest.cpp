// Copyright (C) 2004-2006  Autodesk, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

#include "stdafx.h"
#include "DeleteTest.h"
#include "UnitTestUtil.h"
#include <ctime>
#include <cppunit/extensions/HelperMacros.h>
#include "FdoCommonFile.h"

#ifdef _WIN32
static const wchar_t* SC_TEST_FILE = L"..\\..\\TestData\\DeleteTest.slt";
static const wchar_t* SRC_TEST_FILE = L"..\\..\\TestData\\PARCEL_Source.SLT";
#else
#include <unistd.h>
static const wchar_t* SC_TEST_FILE = L"../../TestData/DeleteTest.slt";
static const wchar_t* SRC_TEST_FILE = L"../../TestData/PARCEL_Source.SLT";
#endif


CPPUNIT_TEST_SUITE_REGISTRATION( DeleteTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( DeleteTest, "DeleteTest");

DeleteTest::DeleteTest(void)
{
    
}

DeleteTest::~DeleteTest(void)
{
    
}


void DeleteTest::setUp ()
{

}

void DeleteTest::tearDown ()
{
}

void DeleteTest::TestSimpleDelete ()
{
    FdoPtr<FdoIConnection> conn;

    try
    {
        if (FdoCommonFile::FileExists(SC_TEST_FILE))
            FdoCommonFile::Delete(SC_TEST_FILE, true);
        FdoCommonFile::Copy(SRC_TEST_FILE, SC_TEST_FILE);

        conn = UnitTestUtil::OpenConnection( SC_TEST_FILE, false, false );
        
	    FdoPtr<FdoIDelete> deleteCmd = (FdoIDelete*)conn->CreateCommand(FdoCommandType_Delete); 
	    deleteCmd->SetFeatureClassName(L"DaKlass");

        FdoPtr<FdoFilter> filter = FdoFilter::Parse(L"ID < 30");
        deleteCmd->SetFilter(filter);

        FdoPtr<FdoITransaction> tr = conn->BeginTransaction();
        FdoInt32 rez = deleteCmd->Execute();
        tr->Commit();
        printf ("Deleted features : %d\n", rez);
        CPPUNIT_ASSERT(rez == 29);
    }
    catch ( FdoException* e )
	{
		TestCommonFail( e );
	}
	catch ( CppUnit::Exception e ) 
	{
		throw;
	}
   	catch (...)
   	{
   		CPPUNIT_FAIL ("caught unexpected exception");
   	}
	printf( "Done\n" );
}


void DeleteTest::TestSpatialDelete ()
{
    FdoPtr<FdoIConnection> conn;

    try
    {
        if (FdoCommonFile::FileExists(SC_TEST_FILE))
            FdoCommonFile::Delete(SC_TEST_FILE, true);
        FdoCommonFile::Copy(SRC_TEST_FILE, SC_TEST_FILE);

        conn = UnitTestUtil::OpenConnection( SC_TEST_FILE, false, false );
        
	    FdoPtr<FdoIDelete> deleteCmd = (FdoIDelete*)conn->CreateCommand(FdoCommandType_Delete); 
	    deleteCmd->SetFeatureClassName(L"DaKlass");

        FdoPtr<FdoFilter> filter = FdoFilter::Parse(L"Data2 INSIDE GeomFromText('POLYGON XYZ ((7.1770013502456 43.7501967446194 0, 7.1770013502456 43.6912771493358 0, 7.27407112243824 43.6912771493358 0, 7.27407112243824 43.7501967446194 0, 7.1770013502456 43.7501967446194 0))')");
        deleteCmd->SetFilter(filter);

        FdoPtr<FdoITransaction> tr = conn->BeginTransaction();
        FdoInt32 rez = deleteCmd->Execute();
        tr->Commit();
        printf ("Deleted features : %d\n", rez);
        CPPUNIT_ASSERT(rez == 21884);
    }
    catch ( FdoException* e )
	{
		TestCommonFail( e );
	}
	catch ( CppUnit::Exception e ) 
	{
		throw;
	}
   	catch (...)
   	{
   		CPPUNIT_FAIL ("caught unexpected exception");
   	}
	printf( "Done\n" );
}

void DeleteTest::TestComplexDelete ()
{
    FdoPtr<FdoIConnection> conn;

    try
    {
        if (FdoCommonFile::FileExists(SC_TEST_FILE))
            FdoCommonFile::Delete(SC_TEST_FILE, true);
        FdoCommonFile::Copy(SRC_TEST_FILE, SC_TEST_FILE);

        conn = UnitTestUtil::OpenConnection( SC_TEST_FILE, false, false );
        
	    FdoPtr<FdoIDelete> deleteCmd = (FdoIDelete*)conn->CreateCommand(FdoCommandType_Delete); 
	    deleteCmd->SetFeatureClassName(L"DaKlass");

        FdoPtr<FdoFilter> filter = FdoFilter::Parse(L"ID < 10000 AND Data2 INSIDE GeomFromText('POLYGON XYZ ((7.1770013502456 43.7501967446194 0, 7.1770013502456 43.6912771493358 0, 7.27407112243824 43.6912771493358 0, 7.27407112243824 43.7501967446194 0, 7.1770013502456 43.7501967446194 0))')");
        deleteCmd->SetFilter(filter);

        FdoPtr<FdoITransaction> tr = conn->BeginTransaction();
        FdoInt32 rez = deleteCmd->Execute();
        tr->Commit();
        printf ("Deleted features : %d\n", rez);
        CPPUNIT_ASSERT(rez == 9471);
    }
    catch ( FdoException* e )
	{
		TestCommonFail( e );
	}
	catch ( CppUnit::Exception e ) 
	{
		throw;
	}
   	catch (...)
   	{
   		CPPUNIT_FAIL ("caught unexpected exception");
   	}
	printf( "Done\n" );
}
