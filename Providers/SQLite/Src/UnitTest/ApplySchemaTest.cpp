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
#include "ApplySchemaTest.h"
#include "UnitTestUtil.h"
#include <ctime>
#include <cppunit/extensions/HelperMacros.h>
#include "FdoCommonFile.h"

#ifdef _WIN32
static const wchar_t* SC_TEST_FILE = L"..\\..\\TestData\\ApplySchemaTest.sqlite";
static const wchar_t* SRC_TEST_FILE = L"..\\..\\TestData\\PARCEL_Source.sqlite";
#else
#include <unistd.h>
static const wchar_t* SC_TEST_FILE = L"../../TestData/ApplySchemaTest.sqlite";
static const wchar_t* SRC_TEST_FILE = L"../../TestData/PARCEL_Source.sqlite";
#endif


CPPUNIT_TEST_SUITE_REGISTRATION( ApplySchemaTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( ApplySchemaTest, "ApplySchemaTest");

ApplySchemaTest::ApplySchemaTest(void)
{
    
}

ApplySchemaTest::~ApplySchemaTest(void)
{
    
}


void ApplySchemaTest::setUp ()
{

}

void ApplySchemaTest::tearDown ()
{
}

void ApplySchemaTest::TestClassRename ()
{
    FdoPtr<FdoIConnection> conn;

    try
    {
        if (FdoCommonFile::FileExists(SC_TEST_FILE))
            FdoCommonFile::Delete(SC_TEST_FILE, true);
        FdoCommonFile::Copy(SRC_TEST_FILE, SC_TEST_FILE);

        conn = UnitTestUtil::OpenConnection( SC_TEST_FILE, false, false );
        
        FdoPtr<FdoIDescribeSchema> descschema = static_cast<FdoIDescribeSchema*>(conn->CreateCommand(FdoCommandType_DescribeSchema));
        FdoPtr<FdoFeatureSchemaCollection> schColl = descschema->Execute();
        CPPUNIT_ASSERT(schColl->GetCount() != 0);
        FdoPtr<FdoFeatureSchema> schema = schColl->GetItem(0);
        
        FdoPtr<FdoClassCollection> classes = schema->GetClasses();
        FdoPtr<FdoClassDefinition> cls1 = classes->GetItem(L"DaKlass");
        FdoPtr<FdoPropertyDefinitionCollection> props = cls1->GetProperties();

        FdoPtr<FdoDataPropertyDefinition> propId = FdoDataPropertyDefinition::Create(L"NEW_PROP", L"");
        propId->SetDataType(FdoDataType_String);
		propId->SetDefaultValue(L"test");
        propId->SetNullable(true);
        props->Add(propId);
        
        FdoPtr<FdoIApplySchema> applyschema = static_cast<FdoIApplySchema*>(conn->CreateCommand(FdoCommandType_ApplySchema));
        applyschema->SetIgnoreStates(true);
        applyschema->SetFeatureSchema(schema);
        applyschema->Execute();
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
