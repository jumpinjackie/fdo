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

void TransactionTest::TestCommitOnApplySchemaNoUsrTr()
{
    FdoPtr<FdoIConnection> conn;

    try
    {
        conn = UnitTestUtil::OpenConnection(SC_TEST_FILE, true, true);

        FdoPtr<FdoIDescribeSchema> descschema = static_cast<FdoIDescribeSchema*>(conn->CreateCommand(FdoCommandType_DescribeSchema));
        FdoPtr<FdoFeatureSchemaCollection> schColl = descschema->Execute();
        CPPUNIT_ASSERT(schColl->GetCount() != 0);
        FdoPtr<FdoFeatureSchema> sch = schColl->GetItem(0);
        FdoPtr<FdoClassCollection> classColl = sch->GetClasses();

        FdoPtr<FdoFeatureClass> fc1 = FdoFeatureClass::Create(L"FeatureClass1", L"None");
        FdoPtr<FdoPropertyDefinitionCollection> props1 = fc1->GetProperties();
        FdoPtr<FdoDataPropertyDefinitionCollection> propsId1 = fc1->GetIdentityProperties();
        
        FdoPtr<FdoGeometricPropertyDefinition> geom1 = FdoGeometricPropertyDefinition::Create(L"GEOM", L"geom prop");
        geom1->SetGeometryTypes(FdoGeometricType_All);
        props1->Add(geom1);
        fc1->SetGeometryProperty(geom1);
        FdoPtr<FdoDataPropertyDefinition> id1 = FdoDataPropertyDefinition::Create(L"FeatId", L"Id");
        id1->SetDataType(FdoDataType_Int32);
        id1->SetIsAutoGenerated(true);
        props1->Add(id1);
        propsId1->Add(id1);
        classColl->Add(fc1);
        
        FdoPtr<FdoFeatureClass> fc2 = FdoFeatureClass::Create(L"FeatureClass2", L"None");
        FdoPtr<FdoPropertyDefinitionCollection> props2 = fc2->GetProperties();
        FdoPtr<FdoDataPropertyDefinitionCollection> propsId2 = fc2->GetIdentityProperties();
        
        FdoPtr<FdoGeometricPropertyDefinition> geom2 = FdoGeometricPropertyDefinition::Create(L"GEOM", L"geom prop");
        geom2->SetGeometryTypes(FdoGeometricType_All);
        props2->Add(geom2);
        fc2->SetGeometryProperty(geom2);
        FdoPtr<FdoDataPropertyDefinition> id2 = FdoDataPropertyDefinition::Create(L"FeatId", L"Id");
        id2->SetDataType(FdoDataType_Int32);
        id2->SetIsAutoGenerated(true);
        props2->Add(id2);
        classColl->Add(fc2);

        bool failed = false;
		FdoPtr<FdoIApplySchema> applyschema = static_cast<FdoIApplySchema*>(conn->CreateCommand(FdoCommandType_ApplySchema));
		applyschema->SetFeatureSchema(sch);
        try
        {
		    applyschema->Execute();
        }
        catch ( FdoException* e )
	    {
            printf ("\nExpected exception: %ls\n", e->GetExceptionMessage());
            e->Release();
            failed = true;
	    }
	    catch ( CppUnit::Exception e ) 
	    {
            failed = true;
	    }
   	    catch (...)
   	    {
            failed = true;
   	    }
        if (failed)
        {
            // add auto-generated property to PK and try again
            propsId2->Add(id2);
            applyschema->Execute();
        }        
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

void TransactionTest::TestCommitOnApplySchemaUsrTr()
{
    FdoPtr<FdoIConnection> conn;

    try
    {
        conn = UnitTestUtil::OpenConnection(SC_TEST_FILE, true, true);
        FdoPtr<FdoITransaction> tr = conn->BeginTransaction();

        FdoPtr<FdoIDescribeSchema> descschema = static_cast<FdoIDescribeSchema*>(conn->CreateCommand(FdoCommandType_DescribeSchema));
        FdoPtr<FdoFeatureSchemaCollection> schColl = descschema->Execute();
        CPPUNIT_ASSERT(schColl->GetCount() != 0);
        FdoPtr<FdoFeatureSchema> sch = schColl->GetItem(0);
        FdoPtr<FdoClassCollection> classColl = sch->GetClasses();

        FdoPtr<FdoFeatureClass> fc1 = FdoFeatureClass::Create(L"FeatureClass1", L"None");
        FdoPtr<FdoPropertyDefinitionCollection> props1 = fc1->GetProperties();
        FdoPtr<FdoDataPropertyDefinitionCollection> propsId1 = fc1->GetIdentityProperties();
        
        FdoPtr<FdoGeometricPropertyDefinition> geom1 = FdoGeometricPropertyDefinition::Create(L"GEOM", L"geom prop");
        geom1->SetGeometryTypes(FdoGeometricType_All);
        props1->Add(geom1);
        fc1->SetGeometryProperty(geom1);
        FdoPtr<FdoDataPropertyDefinition> id1 = FdoDataPropertyDefinition::Create(L"FeatId", L"Id");
        id1->SetDataType(FdoDataType_Int32);
        id1->SetIsAutoGenerated(true);
        props1->Add(id1);
        propsId1->Add(id1);
        classColl->Add(fc1);
        
        FdoPtr<FdoFeatureClass> fc2 = FdoFeatureClass::Create(L"FeatureClass2", L"None");
        FdoPtr<FdoPropertyDefinitionCollection> props2 = fc2->GetProperties();
        FdoPtr<FdoDataPropertyDefinitionCollection> propsId2 = fc2->GetIdentityProperties();
        
        FdoPtr<FdoGeometricPropertyDefinition> geom2 = FdoGeometricPropertyDefinition::Create(L"GEOM", L"geom prop");
        geom2->SetGeometryTypes(FdoGeometricType_All);
        props2->Add(geom2);
        fc2->SetGeometryProperty(geom2);
        FdoPtr<FdoDataPropertyDefinition> id2 = FdoDataPropertyDefinition::Create(L"FeatId", L"Id");
        id2->SetDataType(FdoDataType_Int32);
        id2->SetIsAutoGenerated(true);
        props2->Add(id2);
        classColl->Add(fc2);

        bool failed = false;
		FdoPtr<FdoIApplySchema> applyschema = static_cast<FdoIApplySchema*>(conn->CreateCommand(FdoCommandType_ApplySchema));
		applyschema->SetFeatureSchema(sch);
        try
        {
		    applyschema->Execute();
        }
        catch ( FdoException* e )
	    {
            printf ("\nExpected exception: %ls\n", e->GetExceptionMessage());
            e->Release();
            tr->Rollback();
            failed = true;
	    }
	    catch ( CppUnit::Exception e ) 
	    {
            failed = true;
	    }
   	    catch (...)
   	    {
            failed = true;
   	    }
        if (failed)
        {
            // add auto-generated property to PK and try again
            tr = conn->BeginTransaction();
            propsId2->Add(id2);
            applyschema->Execute();
            tr->Commit();
        }
        else
        {
            CPPUNIT_FAIL ("Exception expected is missing");
        }            
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
