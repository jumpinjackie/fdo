/*
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
#include "BasicTransactionTests.h"

CPPUNIT_TEST_SUITE_REGISTRATION (BasicTransactionTests);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION (BasicTransactionTests, "BasicTransactionTests");

static const wchar_t*    class_description = L"Simple class definition.";

static UnitTestData _string (L"MyString", L"String property.", FdoDataType_String, 512,  0, 0, false, false, false, L"'hello world\n'", (wchar_t*)NULL);
static UnitTestData _single (L"MySingle", L"Single property.", FdoDataType_Single,   0, 6, 2, false, false, false, L"12345.67", (wchar_t*)NULL);
static UnitTestData _double (L"MyDouble", L"Double property.", FdoDataType_Double,   0, 38, 8, false, false, false, L"123456789e-7", (wchar_t*)NULL);
static UnitTestData _short (L"MyShort", L"Short property.", FdoDataType_Int16,   0, 0, 0, false, false, false, L"1234", (wchar_t*)NULL);
static UnitTestData _int (L"MyInteger", L"Integer property.", FdoDataType_Int32,   0, 0, 0, false, false, false, L"1234567", (wchar_t*)NULL);
static UnitTestData _date (L"MyDate", L"Date property.", FdoDataType_DateTime,   0, 0, 0, false, false, false, L"TIMESTAMP '2004-03-29 12:03:45.934'", (wchar_t*)NULL);
//static UnitTestData _bool (L"MyBoolean", L"Boolean property.", FdoDataType_Boolean,   0, 0, 0, false, false, false, L"true", (wchar_t*)NULL);

static UnitTestData* Data[] =
{
    &_string,
    &_single,
    &_double,
    &_short,
    &_int,
    &_date,
//    &_bool,
};

static int PropertyCount = (sizeof (Data) / sizeof (UnitTestData*));

FdoPtr<FdoIConnection> BasicTransactionTests::mConnection;

BasicTransactionTests::BasicTransactionTests(void)
{
}

BasicTransactionTests::~BasicTransactionTests(void)
{
}

void BasicTransactionTests::setUp ()
{
    static bool bSchemaCreated = false;

    if (!CreateSchemaOnly() || !bSchemaCreated)
    {
        mConnection = ArcSDETests::GetConnection ();
        mConnection->SetConnectionString (ArcSDETestConfig::ConnStringWheaton());
        mConnection->Open ();
    }

    if (CreateSchemaOnly() && !bSchemaCreated)
    {
  		// Clean up after previous tests:
        CleanUpClass(mConnection, ArcSDETestConfig::ClassSchemaGeodesy(), ArcSDETestConfig::ClassNameGeodesy());

        FdoPtr<FdoIApplySchema> apply = (FdoIApplySchema*)mConnection->CreateCommand (FdoCommandType_ApplySchema);
        FdoPtr<FdoFeatureSchema> schema = ArcSDETests::GetDefaultSchema(mConnection);
        FdoPtr<FdoClassCollection> classes = schema->GetClasses ();

        // build a simple data property on the main connection
        FdoPtr<FdoDataPropertyDefinition> property = FdoDataPropertyDefinition::Create (Data[0]->mPropertyName, Data[0]->mPropertyDescription);
        property->SetDataType (Data[0]->mPropertyType);
        property->SetLength (Data[0]->mPropertyLength);
        property->SetPrecision (Data[0]->mPropertyPrecision);
        property->SetScale (Data[0]->mPropertyScale);
        property->SetNullable (Data[0]->mPropertyNullable);

        // assemble the class
        FdoPtr<FdoClass> cls = FdoClass::Create (ArcSDETestConfig::ClassNameGeodesy(), class_description);
        FdoPtr<FdoPropertyDefinitionCollection> properties = cls->GetProperties ();
        properties->Add (property);

        // submit the new schema on the main connection
        classes->Add (cls);
        apply->SetFeatureSchema (schema);
		apply->Execute();

        bSchemaCreated = true;
    }
}

void BasicTransactionTests::tearDown ()
{
    if (mConnection)
        mConnection->Close ();
}

/* Test basic commit operation. */
void BasicTransactionTests::commit ()
{
    if (CreateSchemaOnly())  return;

    try
    {
        FdoITransaction* transaction;
        
        transaction = mConnection->BeginTransaction ();
        try
        {
            transaction->Commit ();
            transaction->Release ();
        }
        catch (...)
        {
            transaction->Rollback ();
            transaction->Release ();
        }
    }
    catch (FdoException* ge)
    {
        fail (ge);
    }
}

/* Test basic rollback operation. */
void BasicTransactionTests::rollback ()
{
    if (CreateSchemaOnly())  return;

    try
    {
        FdoITransaction* transaction;
        
        transaction = mConnection->BeginTransaction ();
        try
        {
            transaction->Rollback ();
            transaction->Release ();
        }
        catch (...)
        {
            transaction->Rollback ();
            transaction->Release ();
        }
    }
    catch (FdoException* ge)
    {
        fail (ge);
    }
}

/* Test isolation for commit. */
void BasicTransactionTests::isolation_commit ()
{
    if (CreateSchemaOnly())  return;
    if (!ArcSDETestConfig::SupportsTransactionIsolation()) return;

    try
    {
		// Clean up after previous tests:
		CleanUpClass(mConnection, ArcSDETestConfig::ClassSchemaGeodesy(), ArcSDETestConfig::ClassNameGeodesy(), true);

        FdoITransaction* transaction;

        // start a transaction on the main connection
        transaction = mConnection->BeginTransaction ();

        // open another connection
        FdoPtr<FdoIConnection> alternate_connection = ArcSDETests::GetConnection ();
        alternate_connection->SetConnectionString (ArcSDETestConfig::ConnStringWheaton());
        alternate_connection->Open ();


        FdoPtr<FdoIInsert> insert = (FdoIInsert*)mConnection->CreateCommand (FdoCommandType_Insert);
        insert->SetFeatureClassName (ArcSDETestConfig::QClassNameGeodesy());
        FdoPtr<FdoPropertyValueCollection> values = insert->GetPropertyValues ();
        FdoPtr<FdoValueExpression> expression = (FdoValueExpression*)FdoExpression::Parse (Data[0]->mPropertyData[0]);
        FdoPtr<FdoPropertyValue> value = FdoPropertyValue::Create (Data[0]->mPropertyName, expression);
        values->Add (value);
        FdoPtr<FdoIFeatureReader> reader = insert->Execute ();
        // none returned: reader->Close ();

        // check it's not commited yet by doing a select on the alternate connection
        FdoPtr<FdoISelect> selectCmd = (FdoISelect*)alternate_connection->CreateCommand (FdoCommandType_Select);
        selectCmd->SetFeatureClassName (ArcSDETestConfig::QClassNameGeodesy());
        reader = selectCmd->Execute ();
        while (reader->ReadNext ())
            CPPUNIT_FAIL ("transaction didn't provide isolation");
        reader->Close ();

        // commit on the main connection
        transaction->Commit ();

        // check it is commited by doing a select on the alternate connection
        reader = selectCmd->Execute ();
        while (reader->ReadNext ())
        {
            FdoPtr<FdoStringValue> string = (FdoStringValue*)FdoExpression::Parse (Data[0]->mPropertyData[0]);
            CPPUNIT_ASSERT_MESSAGE ("incorrect value", 0 == wcscmp (string->GetString (), reader->GetString (Data[0]->mPropertyName)));
        }
        reader->Close ();

        // close the alternate connection
        alternate_connection->Close ();

        transaction->Release ();
    }
    catch (FdoException* ge) 
    {
        fail (ge);
    }
}

/* Test isolation for rollback. */
void BasicTransactionTests::isolation_rollback ()
{
    if (CreateSchemaOnly())  return;
    if (!ArcSDETestConfig::SupportsTransactionIsolation()) return;

    try
    {
		// Clean up after previous tests:
		CleanUpClass(mConnection, ArcSDETestConfig::ClassSchemaGeodesy(), ArcSDETestConfig::ClassNameGeodesy(), true);

        FdoITransaction* transaction;

        // start a transaction on the main connection
        transaction = mConnection->BeginTransaction ();

        // open another connection
        FdoPtr<FdoIConnection> alternate_connection = ArcSDETests::GetConnection ();
        alternate_connection->SetConnectionString (ArcSDETestConfig::ConnStringWheaton());
        alternate_connection->Open ();


        FdoPtr<FdoIInsert> insert = (FdoIInsert*)mConnection->CreateCommand (FdoCommandType_Insert);
        insert->SetFeatureClassName (ArcSDETestConfig::QClassNameGeodesy());
        FdoPtr<FdoPropertyValueCollection> values = insert->GetPropertyValues ();
        FdoPtr<FdoValueExpression> expression = (FdoValueExpression*)FdoExpression::Parse (Data[0]->mPropertyData[0]);
        FdoPtr<FdoPropertyValue> value = FdoPropertyValue::Create (Data[0]->mPropertyName, expression);
        values->Add (value);
        FdoPtr<FdoIFeatureReader> reader = insert->Execute ();
        // none returned: reader->Close ();

        // check it's not commited yet by doing a select on the alternate connection
        FdoPtr<FdoISelect> selectCmd = (FdoISelect*)alternate_connection->CreateCommand (FdoCommandType_Select);
        selectCmd->SetFeatureClassName (ArcSDETestConfig::QClassNameGeodesy());
        reader = selectCmd->Execute ();
        while (reader->ReadNext ())
            CPPUNIT_FAIL ("transaction didn't provide isolation");
        reader->Close ();

        // rollback on the main connection
        transaction->Rollback ();

        // check it's not commited by doing a select on the alternate connection
        reader = selectCmd->Execute ();
        while (reader->ReadNext ())
            CPPUNIT_FAIL ("transaction didn't provide isolation");
        reader->Close ();

        // close the alternate connection
        alternate_connection->Close ();

        transaction->Release ();
    }
    catch (FdoException* ge) 
    {
        fail (ge);
    }
}

/* Test isolation for commit on update. */
void BasicTransactionTests::isolation_update_commit ()
{
    if (CreateSchemaOnly())  return;
    if (!ArcSDETestConfig::SupportsTransactionIsolation()) return;

    try
    {
		// Clean up after previous tests:
		CleanUpClass(mConnection, ArcSDETestConfig::ClassSchemaGeodesy(), ArcSDETestConfig::ClassNameGeodesy(), true);

        // open another connection
        FdoPtr<FdoIConnection> alternate_connection = ArcSDETests::GetConnection ();
        alternate_connection->SetConnectionString (ArcSDETestConfig::ConnStringWheaton());
        alternate_connection->Open ();


        FdoPtr<FdoIInsert> insert = (FdoIInsert*)mConnection->CreateCommand (FdoCommandType_Insert);
        insert->SetFeatureClassName (ArcSDETestConfig::QClassNameGeodesy());
        FdoPtr<FdoPropertyValueCollection> values = insert->GetPropertyValues ();
        FdoPtr<FdoValueExpression> expression = (FdoValueExpression*)FdoExpression::Parse (Data[0]->mPropertyData[0]);
        FdoPtr<FdoPropertyValue> value = FdoPropertyValue::Create (Data[0]->mPropertyName, expression);
        values->Add (value);
        FdoPtr<FdoIFeatureReader> reader = insert->Execute ();
        // none returned: reader->Close ();

        FdoITransaction* transaction;

        // start a transaction on the main connection
        transaction = mConnection->BeginTransaction ();

        // update the object (actually all objects)
        FdoPtr<FdoIUpdate> update = (FdoIUpdate*)mConnection->CreateCommand (FdoCommandType_Update);
        update->SetFeatureClassName (ArcSDETestConfig::QClassNameGeodesy());
	    values = update->GetPropertyValues ();
		value->SetValue (L"'This is new data'");
        values->Add (value);
        if (1 != update->Execute ())
            CPPUNIT_FAIL ("update execute failed");

        // check it's not commited yet by doing a select on the alternate connection
        FdoPtr<FdoISelect> selectCmd = (FdoISelect*)alternate_connection->CreateCommand (FdoCommandType_Select);
        selectCmd->SetFeatureClassName (ArcSDETestConfig::QClassNameGeodesy());
        reader = selectCmd->Execute ();
        while (reader->ReadNext ())
        {
            FdoPtr<FdoStringValue> string = (FdoStringValue*)FdoExpression::Parse (Data[0]->mPropertyData[0]);
            CPPUNIT_ASSERT_MESSAGE ("incorrect value", 0 == wcscmp (string->GetString (), reader->GetString (Data[0]->mPropertyName)));
        }
        reader->Close ();

        // commit on the main connection
        transaction->Commit ();

        // check it is commited by doing a select on the alternate connection
        reader = selectCmd->Execute ();
        while (reader->ReadNext ())
        {
            FdoPtr<FdoStringValue> string = (FdoStringValue*)FdoExpression::Parse (Data[0]->mPropertyData[0]);
            CPPUNIT_ASSERT_MESSAGE ("incorrect value", 0 == wcscmp (L"This is new data", reader->GetString (Data[0]->mPropertyName)));
        }
        reader->Close ();

        // close the alternate connection
        alternate_connection->Close ();

        transaction->Release ();
    }
    catch (FdoException* ge) 
    {
        fail (ge);
    }
}

/* Test that the transaction is available on all commands. */
void BasicTransactionTests::get_transaction ()
{
    if (CreateSchemaOnly())  return;

    try
    {
        FdoPtr<FdoITransaction> transaction;
        FdoPtr<FdoIInsert> insert;
        FdoPtr<FdoITransaction> t1;
        FdoPtr<FdoISelect> select;
        FdoPtr<FdoITransaction> t2;
        bool success;
        
        transaction = mConnection->BeginTransaction ();

        insert = (FdoIInsert*)mConnection->CreateCommand (FdoCommandType_Insert);
        t1 = insert->GetTransaction ();
        CPPUNIT_ASSERT_MESSAGE ("transactions differ", t1 == transaction);
        select = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
        t2 = select->GetTransaction ();
        CPPUNIT_ASSERT_MESSAGE ("transactions differ", t2 == transaction);

        transaction->Rollback ();

        try
        {
            t1->Commit ();
            success = true;
        }
        catch (FdoException* ge)
        {
            // expected
            ge->Release ();
            success = false;
        }
        CPPUNIT_ASSERT_MESSAGE ("commit on spent transaction succeeded", !success);

        try
        {
            t2->Rollback ();
            success = true;
        }
        catch (FdoException* ge)
        {
            // expected
            ge->Release ();
            success = false;
        }
        CPPUNIT_ASSERT_MESSAGE ("rollback on spent transaction succeeded", !success);
    }
    catch (FdoException* ge)
    {
        fail (ge);
    }
}
