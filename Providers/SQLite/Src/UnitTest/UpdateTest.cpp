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
#include "UpdateTest.h"
#include "UnitTestUtil.h"
#include <ctime>
#include <cppunit/extensions/HelperMacros.h>
#include "FdoCommonFile.h"

#ifdef _WIN32
static const wchar_t* SC_TEST_FILE = L"..\\..\\TestData\\UpdateTest.sqlite";
static const wchar_t* SRC_TEST_FILE = L"..\\..\\TestData\\PARCEL_Source.sqlite";
#else
#include <unistd.h>
static const wchar_t* SC_TEST_FILE = L"../../TestData/UpdateTest.sqlite";
static const wchar_t* SRC_TEST_FILE = L"../../TestData/PARCEL_Source.sqlite";
#endif


CPPUNIT_TEST_SUITE_REGISTRATION( UpdateTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( UpdateTest, "UpdateTest");

UpdateTest::UpdateTest(void)
{
    
}

UpdateTest::~UpdateTest(void)
{
    
}


void UpdateTest::setUp ()
{

}

void UpdateTest::tearDown ()
{
}

void UpdateTest::TestSimpleUpdate ()
{
    FdoPtr<FdoIConnection> conn;

    try
    {
        if (FdoCommonFile::FileExists(SC_TEST_FILE))
            FdoCommonFile::Delete(SC_TEST_FILE, true);
        FdoCommonFile::Copy(SRC_TEST_FILE, SC_TEST_FILE);

        conn = UnitTestUtil::OpenConnection( SC_TEST_FILE, false, false );
        
	    FdoPtr<FdoIUpdate> update = (FdoIUpdate*)conn->CreateCommand(FdoCommandType_Update); 
	    update->SetFeatureClassName(L"DaKlass");
        FdoPtr<FdoPropertyValueCollection> updColl = update->GetPropertyValues();

        FdoPtr<FdoStringValue> svurl = FdoStringValue::Create(L"ID<30");
        FdoPtr<FdoPropertyValue> pvurl = FdoPropertyValue::Create(L"Url", svurl);
        updColl->Add(pvurl);

        FdoPtr<FdoFilter> filter = FdoFilter::Parse(L"ID < 30");
        update->SetFilter(filter);

        FdoPtr<FdoITransaction> tr = conn->BeginTransaction();
        FdoInt32 rez = update->Execute();
        tr->Commit();
        printf ("Updated features : %d\n", rez);
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

void UpdateTest::TestParmBindUpdate()
{
    FdoPtr<FdoIConnection> conn;

    try
    {
        if (FdoCommonFile::FileExists(SC_TEST_FILE))
            FdoCommonFile::Delete(SC_TEST_FILE, true);
        FdoCommonFile::Copy(SRC_TEST_FILE, SC_TEST_FILE);

        conn = UnitTestUtil::OpenConnection( SC_TEST_FILE, false, false );
        
	    FdoPtr<FdoIUpdate> update = (FdoIUpdate*)conn->CreateCommand(FdoCommandType_Update); 
	    update->SetFeatureClassName(L"DaKlass");
        FdoPtr<FdoPropertyValueCollection> updColl = update->GetPropertyValues();

        FdoPtr<FdoStringValue> svurl = FdoStringValue::Create(L"ID<30");
        FdoPtr<FdoPropertyValue> pvurl = FdoPropertyValue::Create(L"Url", svurl);
        updColl->Add(pvurl);

        FdoPtr<FdoFilter> filter = FdoFilter::Parse(L"ID < :parm");
        update->SetFilter(filter);

        FdoPtr<FdoITransaction> tr = conn->BeginTransaction();

        FdoPtr<FdoParameterValueCollection>parms = update->GetParameterValues();
        FdoPtr<FdoInt32Value> intval = FdoInt32Value::Create(30);
        FdoPtr<FdoParameterValue>parm = FdoParameterValue::Create(L"parm",intval);
        parms->Add(parm);
        FdoInt32 rez = update->Execute();
        tr->Commit();
        printf ("Updated features : %d\n", rez);
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


void UpdateTest::TestSpatialUpdate ()
{
    FdoPtr<FdoIConnection> conn;

    try
    {
        if (FdoCommonFile::FileExists(SC_TEST_FILE))
            FdoCommonFile::Delete(SC_TEST_FILE, true);
        FdoCommonFile::Copy(SRC_TEST_FILE, SC_TEST_FILE);

        conn = UnitTestUtil::OpenConnection( SC_TEST_FILE, false, false );
        
	    FdoPtr<FdoIUpdate> update = (FdoIUpdate*)conn->CreateCommand(FdoCommandType_Update); 
	    update->SetFeatureClassName(L"DaKlass");
        FdoPtr<FdoPropertyValueCollection> updColl = update->GetPropertyValues();

        FdoPtr<FdoStringValue> svurl = FdoStringValue::Create(L"SpatialCond");
        FdoPtr<FdoPropertyValue> pvurl = FdoPropertyValue::Create(L"Url", svurl);
        updColl->Add(pvurl);

        FdoPtr<FdoFilter> filter = FdoFilter::Parse(L"Data2 INSIDE GeomFromText('POLYGON XYZ ((7.1770013502456 43.7501967446194 0, 7.1770013502456 43.6912771493358 0, 7.27407112243824 43.6912771493358 0, 7.27407112243824 43.7501967446194 0, 7.1770013502456 43.7501967446194 0))')");
        update->SetFilter(filter);

        FdoPtr<FdoITransaction> tr = conn->BeginTransaction();
        FdoInt32 rez = update->Execute();
        tr->Commit();
        printf ("Updated features : %d\n", rez);
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

void UpdateTest::TestComplexUpdate ()
{
    FdoPtr<FdoIConnection> conn;

    try
    {
        if (FdoCommonFile::FileExists(SC_TEST_FILE))
            FdoCommonFile::Delete(SC_TEST_FILE, true);
        FdoCommonFile::Copy(SRC_TEST_FILE, SC_TEST_FILE);

        conn = UnitTestUtil::OpenConnection( SC_TEST_FILE, false, false );
        
	    FdoPtr<FdoIUpdate> update = (FdoIUpdate*)conn->CreateCommand(FdoCommandType_Update); 
	    update->SetFeatureClassName(L"DaKlass");
        FdoPtr<FdoPropertyValueCollection> updColl = update->GetPropertyValues();

        FdoPtr<FdoStringValue> svurl = FdoStringValue::Create(L"SpatialCond");
        FdoPtr<FdoPropertyValue> pvurl = FdoPropertyValue::Create(L"Url", svurl);
        updColl->Add(pvurl);

        FdoPtr<FdoFilter> filter = FdoFilter::Parse(L"ID < 10000 AND Data2 INSIDE GeomFromText('POLYGON XYZ ((7.1770013502456 43.7501967446194 0, 7.1770013502456 43.6912771493358 0, 7.27407112243824 43.6912771493358 0, 7.27407112243824 43.7501967446194 0, 7.1770013502456 43.7501967446194 0))')");
        update->SetFilter(filter);

        FdoPtr<FdoITransaction> tr = conn->BeginTransaction();
        FdoInt32 rez = update->Execute();
        tr->Commit();
        printf ("Updated features : %d\n", rez);
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
