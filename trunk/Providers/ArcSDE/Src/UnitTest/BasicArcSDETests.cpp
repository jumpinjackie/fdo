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
#include "BasicArcSDETests.h"

CPPUNIT_TEST_SUITE_REGISTRATION (BasicArcSDETests);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION (BasicArcSDETests, "BasicArcSDETests");

//NO MULTIPLE SCHEMA SUPPORT: static const wchar_t*    schema_name = L"ArcSDESchema";
//NO MULTIPLE SCHEMA SUPPORT:static const wchar_t*    schema_description = L"Basic ArcSDE test schema.";
static const wchar_t*    class_description = L"Simple class definition.";

static UnitTestData _string (L"MyString", L"String property.", FdoDataType_String, 512,  0, 0, false, false, false, L"'hello world\n'", (wchar_t*)NULL);
static UnitTestData _single (L"MySingle", L"Single property.", FdoDataType_Single,   0, 6, 2, false, false, false, L"12345.67", (wchar_t*)NULL);
static UnitTestData _double (L"MyDouble", L"Double property.", FdoDataType_Double,   0, 38, 8, false, false, false, L"123456789e-7", (wchar_t*)NULL);
static UnitTestData _short (L"MyShort", L"Short property.", FdoDataType_Int16,   0, 0, 0, false, false, false, L"1234", (wchar_t*)NULL);
static UnitTestData _int (L"MyInteger", L"Integer property.", FdoDataType_Int32,   0, 0, 0, false, false, false, L"1234567", (wchar_t*)NULL);
static UnitTestData _date (L"MyDate", L"Date property.", FdoDataType_DateTime,   0, 0, 0, false, false, false, L"TIMESTAMP '2004-03-29 12:03:45.934'", (wchar_t*)NULL);
//static UnitTestData _decimal (L"MyDecimal", L"Decimal property.", FdoDataType_Decimal,   0, 10, 5, false, false, false, L"12345.67899", (wchar_t*)NULL);
static UnitTestData _blob (L"MyBlob", L"Blob property.", FdoDataType_BLOB,   0, 0, 0, true, false, false, L"1,5,234,101", (wchar_t*)NULL);
//static UnitTestData _bool (L"MyBoolean", L"Boolean property.", FdoDataType_Boolean,   0, 0, 0, false, false, false, L"true", (wchar_t*)NULL);
//TODO: If we support ApplySchema, need to test these types: FdoDataType_Byte, FdoDataType_Int64, FdoDataType_Boolean

static UnitTestData* Data[] =
{
    &_string,
    &_single,
    &_double,
    &_short,
    &_int,
    &_date,
//    &_decimal,
    &_blob,
//    &_bool,
};

static int PropertyCount = (sizeof (Data) / sizeof (UnitTestData*));

FdoPtr<FdoIConnection> BasicArcSDETests::mConnection;

BasicArcSDETests::BasicArcSDETests(void)
{
}

BasicArcSDETests::~BasicArcSDETests(void)
{
}

void BasicArcSDETests::setUp ()
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
        //TODO: for some reason, using the 3rd data property (the MyShort short int property) fails here.
		int iDataProperty = 4;  // 3 is the short property

        // Clean up previous tests:
        CleanUpClass(mConnection, ArcSDETestConfig::ClassSchemaRowIdTest(), ArcSDETestConfig::ClassNameRowIdTest());

        FdoPtr<FdoIApplySchema> apply = (FdoIApplySchema*)mConnection->CreateCommand (FdoCommandType_ApplySchema);
		FdoPtr<FdoFeatureSchema> schema = ArcSDETests::GetDefaultSchema(mConnection); //FdoFeatureSchema::Create (schema_name, schema_description);
        FdoPtr<FdoClassCollection> classes = schema->GetClasses ();

        // build a simple data property
        FdoPtr<FdoDataPropertyDefinition> property = FdoDataPropertyDefinition::Create (Data[iDataProperty]->mPropertyName, Data[iDataProperty]->mPropertyDescription);
        property->SetDataType (Data[iDataProperty]->mPropertyType);
        property->SetLength (Data[iDataProperty]->mPropertyLength);
        property->SetPrecision (Data[iDataProperty]->mPropertyPrecision);
        property->SetScale (Data[iDataProperty]->mPropertyScale);
        property->SetNullable (Data[iDataProperty]->mPropertyNullable);

        // assemble the class
        FdoPtr<FdoClass> cls = FdoClass::Create (ArcSDETestConfig::ClassNameRowIdTest(), class_description);
        FdoPtr<FdoPropertyDefinitionCollection> properties = cls->GetProperties ();
        properties->Add (property);
		FdoPtr<FdoDataPropertyDefinitionCollection> idProperties = cls->GetIdentityProperties();
		idProperties->Add(property);  // THIS IS THE MAIN PART OF THE TEST

        // submit the new schema
        classes->Add (cls);
        apply->SetFeatureSchema (schema);
        apply->Execute ();

        bSchemaCreated = true;
    }
}

void BasicArcSDETests::tearDown ()
{
    if (mConnection)
        mConnection->Close ();
}

/* Test basic ArcSDE rowid functionality. */
void BasicArcSDETests::rowid_tests ()
{
    if (CreateSchemaOnly())  return;

    try
    {
        //TODO: for some reason, using the 3rd data property (the MyShort short int property) fails here.
		int iDataProperty = 4;  // 3 is the short property

		// Clean up previous tests:
		CleanUpClass(mConnection, ArcSDETestConfig::ClassSchemaRowIdTest(), ArcSDETestConfig::ClassNameRowIdTest(), true);

        //TODO: test inserting/selecting against this user-defined SHORT identity column:

/*
        FdoPtr<FdoIInsert> insert = (FdoIInsert*)mConnection->CreateCommand (FdoCommandType_Insert);
        insert->SetFeatureClassName (ArcSDETestConfig::QClassNameRowIdTest()); // misnomer, it's not a feature class
        FdoPtr<FdoPropertyValueCollection> values = insert->GetPropertyValues ();
        FdoPtr<FdoPropertyValue> value = values->GetItem (Data[iDataProperty]->mPropertyName);
        FdoPtr<FdoValueExpression> expression = (FdoValueExpression*)FdoExpression::Parse (Data[iDataProperty]->mPropertyData);
        value->SetValue (expression);
        FdoPtr<FdoIFeatureReader> reader = insert->Execute ();
        // none returned: reader->Close ();

        // check by doing a select
        FdoPtr<FdoISelect> selectCmd = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
        selectCmd->SetFeatureClassName (ArcSDETestConfig::QClassNameRowIdTest());
        reader = selectCmd->Execute ();
        while (reader->ReadNext ())
        {
            FdoPtr<FdoStringValue> string = (FdoStringValue*)FdoExpression::Parse (Data[iDataProperty]->mPropertyData);
            CPPUNIT_ASSERT_MESSAGE ("incorrect value", 0 == wcscmp (string->GetString (), reader->GetString (Data[iDataProperty]->mPropertyName)));
        }
        reader->Close ();
*/
    }
    catch (FdoException* ge) 
    {
        fail (ge);
    }
}
