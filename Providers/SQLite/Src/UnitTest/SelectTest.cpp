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
#include "SelectTest.h"
#include "UnitTestUtil.h"
#include <ctime>
#include <cppunit/extensions/HelperMacros.h>
#include "FdoCommonFile.h"

#ifdef _WIN32
static const wchar_t* SC_TEST_FILE = L"..\\..\\TestData\\SelectTest.sqlite";
static const wchar_t* SRC_TEST_FILE = L"..\\..\\TestData\\PARCEL_Source.sqlite";
#else
#include <unistd.h>
static const wchar_t* SC_TEST_FILE = L"../../TestData/SelectTest.sqlite";
static const wchar_t* SRC_TEST_FILE = L"../../TestData/PARCEL_Source.sqlite";
#endif


CPPUNIT_TEST_SUITE_REGISTRATION( SelectTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( SelectTest, "SelectTest");

SelectTest::SelectTest(void)
{
    
}

SelectTest::~SelectTest(void)
{
    
}


void SelectTest::setUp ()
{

}

void SelectTest::tearDown ()
{
}

void SelectTest::TestSimpleSelect ()
{
    FdoPtr<FdoIConnection> conn;

    try
    {
        if (FdoCommonFile::FileExists(SC_TEST_FILE))
            FdoCommonFile::Delete(SC_TEST_FILE, true);
        FdoCommonFile::Copy(SRC_TEST_FILE, SC_TEST_FILE);

        conn = UnitTestUtil::OpenConnection( SC_TEST_FILE, false, false );
        
	    FdoPtr<FdoISelect> SelectCmd = (FdoISelect*)conn->CreateCommand(FdoCommandType_Select); 
	    SelectCmd->SetFeatureClassName(L"DaKlass");

        FdoPtr<FdoFilter> filter = FdoFilter::Parse(L"ID < 30");
        SelectCmd->SetFilter(filter);

        FdoPtr<FdoIFeatureReader>reader = SelectCmd->Execute();
        int rez = 0;
        while(reader->ReadNext())rez++;
        printf ("Selectd features : %d\n", rez);
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

void SelectTest::TestBindSelect ()
{
    FdoPtr<FdoIConnection> conn;

    try
    {
        if (FdoCommonFile::FileExists(SC_TEST_FILE))
            FdoCommonFile::Delete(SC_TEST_FILE, true);
        FdoCommonFile::Copy(SRC_TEST_FILE, SC_TEST_FILE);

        conn = UnitTestUtil::OpenConnection( SC_TEST_FILE, false, false );
        
	    FdoPtr<FdoISelect> SelectCmd = (FdoISelect*)conn->CreateCommand(FdoCommandType_Select); 
	    SelectCmd->SetFeatureClassName(L"DaKlass");

        FdoPtr<FdoFilter> filter = FdoFilter::Parse(L"ID < :parm");
        SelectCmd->SetFilter(filter);

        FdoPtr<FdoParameterValueCollection>parms = SelectCmd->GetParameterValues();
        FdoPtr<FdoInt32Value> intval = FdoInt32Value::Create(30);
        FdoPtr<FdoParameterValue>parm = FdoParameterValue::Create(L"parm",intval);
        parms->Add(parm);
        FdoPtr<FdoIFeatureReader>reader = SelectCmd->Execute();
        int rez = 0;
        while(reader->ReadNext())rez++;
        printf ("Selectd features : %d\n", rez);
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

void SelectTest::TestSpatialSelect ()
{
    FdoPtr<FdoIConnection> conn;

    try
    {
        if (FdoCommonFile::FileExists(SC_TEST_FILE))
            FdoCommonFile::Delete(SC_TEST_FILE, true);
        FdoCommonFile::Copy(SRC_TEST_FILE, SC_TEST_FILE);

        conn = UnitTestUtil::OpenConnection( SC_TEST_FILE, false, false );
        
	    FdoPtr<FdoISelect> SelectCmd = (FdoISelect*)conn->CreateCommand(FdoCommandType_Select); 
	    SelectCmd->SetFeatureClassName(L"DaKlass");

        FdoPtr<FdoFilter> filter = FdoFilter::Parse(L"ID > :lowerbound AND ID < :upperbound AND Data2 INSIDE GeomFromText('POLYGON XYZ ((7.1770013502456 43.7501967446194 0, 7.1770013502456 43.6912771493358 0, 7.27407112243824 43.6912771493358 0, 7.27407112243824 43.7501967446194 0, 7.1770013502456 43.7501967446194 0))')");
        SelectCmd->SetFilter(filter);
        
        FdoPtr<FdoParameterValueCollection>parms = SelectCmd->GetParameterValues();
        FdoPtr<FdoInt32Value> intval = FdoInt32Value::Create(5000);
        FdoPtr<FdoParameterValue>parm = FdoParameterValue::Create(L"lowerbound",intval);
        parms->Add(parm);
        intval = FdoInt32Value::Create(10000);
        parm = FdoParameterValue::Create(L"upperbound",intval);
        parms->Add(parm);
        FdoPtr<FdoIFeatureReader>reader = SelectCmd->Execute();
        int rez = 0;
        while(reader->ReadNext())rez++;
        
        printf ("Selectd features : %d\n", rez);
        CPPUNIT_ASSERT(rez == 4954);
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

void SelectTest::TestComplexSelect ()
{
    FdoPtr<FdoIConnection> conn;

    try
    {
        if (FdoCommonFile::FileExists(SC_TEST_FILE))
            FdoCommonFile::Delete(SC_TEST_FILE, true);
        FdoCommonFile::Copy(SRC_TEST_FILE, SC_TEST_FILE);

        conn = UnitTestUtil::OpenConnection( SC_TEST_FILE, false, false );
        
	    FdoPtr<FdoISelect> SelectCmd = (FdoISelect*)conn->CreateCommand(FdoCommandType_Select); 
	    SelectCmd->SetFeatureClassName(L"DaKlass");

        FdoPtr<FdoFilter> filter = FdoFilter::Parse(L"ID < 10000 AND Data2 INSIDE GeomFromText('POLYGON XYZ ((7.1770013502456 43.7501967446194 0, 7.1770013502456 43.6912771493358 0, 7.27407112243824 43.6912771493358 0, 7.27407112243824 43.7501967446194 0, 7.1770013502456 43.7501967446194 0))')");
        SelectCmd->SetFilter(filter);

        FdoPtr<FdoIFeatureReader>reader = SelectCmd->Execute();
        int rez = 0;
        while(reader->ReadNext())rez++;
        printf ("Selectd features : %d\n", rez);
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

void SelectTest::TestComplexWithBindSelect ()
{
    FdoPtr<FdoIConnection> conn;

    try
    {
        if (FdoCommonFile::FileExists(SC_TEST_FILE))
            FdoCommonFile::Delete(SC_TEST_FILE, true);
        FdoCommonFile::Copy(SRC_TEST_FILE, SC_TEST_FILE);

        conn = UnitTestUtil::OpenConnection( SC_TEST_FILE, false, false );
        
	    FdoPtr<FdoISelect> SelectCmd = (FdoISelect*)conn->CreateCommand(FdoCommandType_Select); 
	    SelectCmd->SetFeatureClassName(L"DaKlass");

        FdoPtr<FdoFilter> filter = FdoFilter::Parse(L"ID < :parm AND Data2 INSIDE GeomFromText('POLYGON XYZ ((7.1770013502456 43.7501967446194 0, 7.1770013502456 43.6912771493358 0, 7.27407112243824 43.6912771493358 0, 7.27407112243824 43.7501967446194 0, 7.1770013502456 43.7501967446194 0))')");
        SelectCmd->SetFilter(filter);

        FdoPtr<FdoParameterValueCollection>parms = SelectCmd->GetParameterValues();
        FdoPtr<FdoInt32Value> intval = FdoInt32Value::Create(10000);
        FdoPtr<FdoParameterValue>parm = FdoParameterValue::Create(L"parm",intval);
        parms->Add(parm);
        FdoPtr<FdoIFeatureReader>reader = SelectCmd->Execute();
        int rez = 0;
        while(reader->ReadNext())rez++;
        printf ("Selectd features : %d\n", rez);
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