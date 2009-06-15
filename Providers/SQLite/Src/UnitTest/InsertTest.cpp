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
#include "InsertTest.h"
#include "UnitTestUtil.h"
#include <ctime>
#include <cppunit/extensions/HelperMacros.h>
#ifdef _WIN32
static const wchar_t* SC_TEST_FILE = L"..\\..\\TestData\\InsertTest.slt";
#else
#include <unistd.h>
static const wchar_t* SC_TEST_FILE = L"../../TestData/InsertTest.slt";
#endif


CPPUNIT_TEST_SUITE_REGISTRATION( InsertTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( InsertTest, "InsertTest");

InsertTest::InsertTest(void)
{
    
}

InsertTest::~InsertTest(void)
{
    
}

void InsertTest::setUp ()
{

}

void InsertTest::tearDown ()
{
}

void InsertTest::TestInsInvalidGeomTypes ()
{
	try
	{
        FdoPtr<FdoIConnection> conn = UnitTestUtil::CreateConnection();

		UnitTestUtil::OpenConnection( SC_TEST_FILE, true, true, conn );
		 //apply schema
		FdoPtr<FdoIApplySchema> applyschema = static_cast<FdoIApplySchema*>(conn->CreateCommand(FdoCommandType_ApplySchema));
        FdoPtr<FdoFeatureSchema> schema = UnitTestUtil::CreateSLTSchema(FdoGeometryType_Point);
		applyschema->SetFeatureSchema(schema);
		applyschema->Execute();

        UnitTestUtil::CreateData(false, conn, 100);
        conn->Close();
        conn->Open();

        FdoPtr<FdoIDescribeSchema> descschema = static_cast<FdoIDescribeSchema*>(conn->CreateCommand(FdoCommandType_DescribeSchema));
        FdoPtr<FdoFeatureSchemaCollection> schColl = descschema->Execute();
        CPPUNIT_ASSERT(schColl->GetCount() != 0);
        FdoPtr<FdoFeatureSchema> sch = schColl->GetItem(0);
        FdoPtr<FdoClassCollection> classColl = sch->GetClasses();
        FdoPtr<FdoClassDefinition> clsDef = classColl->GetItem(L"Parcel");
        CPPUNIT_ASSERT(clsDef->GetClassType() == FdoClassType_FeatureClass);
        FdoFeatureClass* fclsDef = static_cast<FdoFeatureClass*>(clsDef.p);
        FdoPtr<FdoGeometricPropertyDefinition> geomProp = fclsDef->GetGeometryProperty();
        CPPUNIT_ASSERT(geomProp->GetGeometryTypes() == FdoGeometricType_Point);

        FdoInt32 length = 0;
        FdoGeometryType* types = geomProp->GetSpecificGeometryTypes(length);
        CPPUNIT_ASSERT(length == 1);
        CPPUNIT_ASSERT(*types == FdoGeometryType_Point);        
	}
	catch(FdoException *exp )
	{
		UnitTestUtil::PrintException( exp, stdout, false);
        FDO_SAFE_RELEASE(exp);
        CPPUNIT_FAIL("Insert failed");
	}
}