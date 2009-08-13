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
#include "TransactionTest.h"
#include "UnitTestUtil.h"
#include <ctime>
#include <cppunit/extensions/HelperMacros.h>
#ifdef _WIN32
static const wchar_t* SC_TEST_FILE = L"..\\..\\TestData\\TransactionTest.sqlite";
#else
#include <unistd.h>
static const wchar_t* SC_TEST_FILE = L"../../TestData/TransactionTest.sqlite";
#endif


CPPUNIT_TEST_SUITE_REGISTRATION( TransactionTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( TransactionTest, "TransactionTest");

TransactionTest::TransactionTest(void)
{
}

TransactionTest::~TransactionTest(void)
{
}

void TransactionTest::setUp()
{

}

void TransactionTest::tearDown()
{

}

void TransactionTest::InsertOneValue(FdoIConnection* conn, FdoString* nameProp, FdoValueExpression* value)
{
    FdoPtr<FdoIInsert> insCmd = static_cast<FdoIInsert*>(conn->CreateCommand(FdoCommandType_Insert));
    insCmd->SetFeatureClassName(L"ParcelChild");
    FdoPtr<FdoPropertyValueCollection> vals = insCmd->GetPropertyValues();
    FdoPtr<FdoPropertyValue> propIns = FdoPropertyValue::Create(nameProp, value);
    vals->Add(propIns);
    
    FdoPtr<FdoIFeatureReader> rdr = insCmd->Execute();
    CPPUNIT_ASSERT(rdr->ReadNext());
    rdr->Close();
}

bool TransactionTest::CheckForValue(FdoIConnection* conn, FdoString* filter)
{
	FdoPtr<FdoISelect> select = (FdoISelect*)conn->CreateCommand(FdoCommandType_Select); 
	select->SetFeatureClassName(L"ParcelChild");
    select->SetFilter(filter);
    FdoPtr<FdoIFeatureReader> rdr = select->Execute();
    return rdr->ReadNext();
}

// it checks if the user can open a transaction when an internal 
// transaction is already opened
// also it checks if the internal transaction is reopened after
// user closes it by opening a transaction
void TransactionTest::TestStartTransaction()
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
        conn->Close();
        conn->Open();

        // enforce transaction destroy for testing
        bool expectedExc = false;
 	    try
 	    {
            FdoPtr<FdoITransaction> trans = conn->BeginTransaction();
            FdoPtr<FdoStringValue> valIns = FdoStringValue::Create(L"testTr");
            InsertOneValue(conn, L"Name", valIns);
            expectedExc = true;
            FdoPtr<FdoITransaction> trans2 = conn->BeginTransaction();
        }
        catch(FdoException* exc)
        {
            if (!expectedExc)
            {
                UnitTestUtil::PrintException( exc, stdout, false);
                FDO_SAFE_RELEASE(exc);
                CPPUNIT_FAIL("TestStartTransaction failed");
            }
            FDO_SAFE_RELEASE(exc);
        }

        FdoPtr<FdoIInsert> insCmd = static_cast<FdoIInsert*>(conn->CreateCommand(FdoCommandType_Insert));
        insCmd->SetFeatureClassName(L"ParcelChild");
        FdoPtr<FdoPropertyValueCollection> vals = insCmd->GetPropertyValues();
        FdoPtr<FdoPropertyValue> propIns;
        
        FdoPtr<FdoStringValue> valPropName = FdoStringValue::Create(L"name1");
        propIns = FdoPropertyValue::Create(L"Name", valPropName);
        vals->Add(propIns);
        
        FdoPtr<FdoStringValue> valPropKey = FdoStringValue::Create(L"key1");
        propIns = FdoPropertyValue::Create(L"Key", valPropKey);
        vals->Add(propIns);
        
        FdoPtr<FdoDoubleValue> valPropNumb = FdoDoubleValue::Create(22);
        propIns = FdoPropertyValue::Create(L"Numb", valPropNumb);
        vals->Add(propIns);
        FdoPtr<FdoIFeatureReader> rdr = insCmd->Execute();
        CPPUNIT_ASSERT(rdr->ReadNext());
        rdr->Close();

        // enforce transaction destroy for testing
        {
            FdoPtr<FdoITransaction> trans = conn->BeginTransaction();
            FdoPtr<FdoStringValue> valIns = FdoStringValue::Create(L"Transaction");
            InsertOneValue(conn, L"Name", valIns);
            trans->Commit();
        }

        valPropName->SetString(L"name2");
        valPropKey = FdoStringValue::Create(L"key2");
        valPropNumb = FdoDoubleValue::Create(33);
        
        FdoPtr<FdoIFeatureReader> rdr2 = insCmd->Execute();
        CPPUNIT_ASSERT(rdr2->ReadNext());
        rdr2->Close();
 	}
 	catch(FdoException *exp )
 	{
        UnitTestUtil::PrintException( exp, stdout, false);
        FDO_SAFE_RELEASE(exp);
        CPPUNIT_FAIL("TestStartTransaction failed");
 	}
}

void TransactionTest::TestCommit()
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
        conn->Close();
        conn->Open();

        FdoPtr<FdoIInsert> insCmd = static_cast<FdoIInsert*>(conn->CreateCommand(FdoCommandType_Insert));
        insCmd->SetFeatureClassName(L"ParcelChild");
        FdoPtr<FdoPropertyValueCollection> vals = insCmd->GetPropertyValues();
        FdoPtr<FdoPropertyValue> propIns;
        
        FdoPtr<FdoStringValue> valPropName = FdoStringValue::Create(L"name1");
        propIns = FdoPropertyValue::Create(L"Name", valPropName);
        vals->Add(propIns);
        
        FdoPtr<FdoStringValue> valPropKey = FdoStringValue::Create(L"key1");
        propIns = FdoPropertyValue::Create(L"Key", valPropKey);
        vals->Add(propIns);
        
        FdoPtr<FdoDoubleValue> valPropNumb = FdoDoubleValue::Create(22);
        propIns = FdoPropertyValue::Create(L"Numb", valPropNumb);
        vals->Add(propIns);
        FdoPtr<FdoIFeatureReader> rdr = insCmd->Execute();
        CPPUNIT_ASSERT(rdr->ReadNext());
        rdr->Close();

        // enforce transaction destroy for testing
        {
            FdoPtr<FdoITransaction> trans = conn->BeginTransaction();
            FdoPtr<FdoStringValue> valIns = FdoStringValue::Create(L"Transaction");
            InsertOneValue(conn, L"Name", valIns);
            trans->Commit();
        }
        CPPUNIT_ASSERT(CheckForValue(conn, L"Name='name1'"));
        CPPUNIT_ASSERT(CheckForValue(conn, L"Name='Transaction'"));

        valPropName->SetString(L"name2");
        valPropKey = FdoStringValue::Create(L"key2");
        valPropNumb = FdoDoubleValue::Create(33);
        
        FdoPtr<FdoIFeatureReader> rdr2 = insCmd->Execute();
        CPPUNIT_ASSERT(rdr2->ReadNext());
        rdr2->Close();
        CPPUNIT_ASSERT(CheckForValue(conn, L"Name='name2'"));
 	}
 	catch(FdoException *exp )
 	{
        UnitTestUtil::PrintException( exp, stdout, false);
        FDO_SAFE_RELEASE(exp);
        CPPUNIT_FAIL("TestCommit failed");
 	}
}

void TransactionTest::TestRollback()
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
        conn->Close();
        conn->Open();

        FdoPtr<FdoIInsert> insCmd = static_cast<FdoIInsert*>(conn->CreateCommand(FdoCommandType_Insert));
        insCmd->SetFeatureClassName(L"ParcelChild");
        FdoPtr<FdoPropertyValueCollection> vals = insCmd->GetPropertyValues();
        FdoPtr<FdoPropertyValue> propIns;
        
        FdoPtr<FdoStringValue> valPropName = FdoStringValue::Create(L"name1");
        propIns = FdoPropertyValue::Create(L"Name", valPropName);
        vals->Add(propIns);
        
        FdoPtr<FdoStringValue> valPropKey = FdoStringValue::Create(L"key1");
        propIns = FdoPropertyValue::Create(L"Key", valPropKey);
        vals->Add(propIns);
        
        FdoPtr<FdoDoubleValue> valPropNumb = FdoDoubleValue::Create(22);
        propIns = FdoPropertyValue::Create(L"Numb", valPropNumb);
        vals->Add(propIns);
        FdoPtr<FdoIFeatureReader> rdr = insCmd->Execute();
        CPPUNIT_ASSERT(rdr->ReadNext());
        rdr->Close();

        // enforce transaction destroy for testing
        {
            FdoPtr<FdoITransaction> trans = conn->BeginTransaction();
            FdoPtr<FdoStringValue> valIns = FdoStringValue::Create(L"Transaction");
            InsertOneValue(conn, L"Name", valIns);
            trans->Rollback();
        }
        CPPUNIT_ASSERT(CheckForValue(conn, L"Name='name1'"));
        CPPUNIT_ASSERT(!CheckForValue(conn, L"Name='Transaction'"));

        valPropName->SetString(L"name2");
        valPropKey = FdoStringValue::Create(L"key2");
        valPropNumb = FdoDoubleValue::Create(33);
        
        FdoPtr<FdoIFeatureReader> rdr2 = insCmd->Execute();
        CPPUNIT_ASSERT(rdr2->ReadNext());
        rdr2->Close();
        CPPUNIT_ASSERT(CheckForValue(conn, L"Name='name2'"));
 	}
 	catch(FdoException *exp )
 	{
        UnitTestUtil::PrintException( exp, stdout, false);
        FDO_SAFE_RELEASE(exp);
        CPPUNIT_FAIL("TestRollback failed");
 	}
}
