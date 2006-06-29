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
#include "BasicInsertTests.h"

CPPUNIT_TEST_SUITE_REGISTRATION (BasicInsertTests);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION (BasicInsertTests, "BasicInsertTests");


// NOTE: The data/schema required for this test suite was imported from TestGeoDb.mdb (form the repository)
//       into the database pointed to by ArcSDETestConfig::ConnStringMetadcov();
//       IMPORTANT: you need to manually reset every FLOAT field in TestGeoDb.mdb to a (precision,scale) of (6,2)
//       before exporting it to enterprise goedatabase, otherwise FLOATs become DOUBLEs! It doesn't seem that
//       Personal Geodatabases can store the precision/scale properly.
//NO MULTIPLE SCHEMA SUPPORT:static const wchar_t*    schema_name = L"InsertSchema";
//NO MULTIPLE SCHEMA SUPPORT:static const wchar_t*    schema_description = L"Basic insert test schema.";
static const wchar_t*    class_new_description = L"New class definition.";
static const wchar_t*    class_geom_zm_desc = L"Test class for testing geometry Z and M values";


static UnitTestData _string (L"MYSTRING", L"String property.", FdoDataType_String, 512,  0, 0, false, false, false,
                             L"'hello world\n'", L"'something'", (wchar_t*)NULL);
static UnitTestData _single (L"MYFLOAT", L"Single property 1.", FdoDataType_Single,   0, 6, 2, false, false, false,
                             L"1234.67", L"7632.22", (wchar_t*)NULL);
static UnitTestData _single2(L"MYFLOAT2", L"Single property 2.", FdoDataType_Single,   0, 6, 2, false, false, false,
                             L"1122.33", L"7811.14", (wchar_t*)NULL);
static UnitTestData _double (L"MYDOUBLE", L"Double property.", FdoDataType_Double,   0, 38, 8, true, false, false,
                             L"123456789e-7", L"NULL", (wchar_t*)NULL);
static UnitTestData _short  (L"MYSHORTINT", L"Short property.", FdoDataType_Int16,   0, 0, 0, true, false, false,
                             L"1234", L"NULL", (wchar_t*)NULL);
static UnitTestData _int    (L"MYINT", L"Integer property.", FdoDataType_Int32,   0, 0, 0, true, false, false,
                             L"1234567", L"NULL", (wchar_t*)NULL);
static UnitTestData _date   (L"MYDATE", L"Date property.", FdoDataType_DateTime,   0, 0, 0, true, false, false,
                             L"TIMESTAMP '2004-03-29 12:03:45.934'", L"NULL", (wchar_t*)NULL);
//static UnitTestData _bool   (L"MyBoolean", L"Boolean property.", FdoDataType_Boolean,   0, 0, 0, true, false, false,
//                             L"true", L"NULL", (wchar_t*)NULL);
static UnitTestData _blob   (L"MYBLOB", L"Blob property.", FdoDataType_BLOB,   0, 0, 0, true, false, false,
                             L"1,5,234,101", L"NULL", (wchar_t*)NULL);
//static UnitTestData _decimal(L"MyDecimal", L"Decimal property.", FdoDataType_Decimal,   0, 8, 3, true, false, false,
//                             L"92334.221", L"NULL", (wchar_t*)NULL);

static UnitTestData* Data[] =
{
    &_string,
    &_single,
    &_single2,
    &_double,
    &_short,
    &_int,
    &_date,
    &_blob,

//    &_decimal,
//    &_bool,
};

static int PropertyCount = (sizeof (Data) / sizeof (UnitTestData*));

static wchar_t* BatchData[5][2] = {
    {L"'Batch 1st line'", L"1001.32"},
    {L"'Batch 2nd line'", L"43.32"},
    {L"'Batch 3rd line'", L"3001.2"},
    {L"'Batch 4th line'", L"7747.1"},
    {L"'Batch 5th line'", L"555.55"}
};


FdoPtr<FdoIConnection> BasicInsertTests::mConnection;

BasicInsertTests::BasicInsertTests(void)
{
}

BasicInsertTests::~BasicInsertTests(void)
{
}

void BasicInsertTests::setUp ()
{
    static bool bSchemaCreated = false;
    if (CreateSchemaOnly() && !bSchemaCreated)
    {
        mConnection = ArcSDETests::GetConnection ();
        mConnection->SetConnectionString (ArcSDETestConfig::ConnStringMetadcov());
        mConnection->Open ();

        {
            // Clean up previous tests:
            CleanUpClass(mConnection, ArcSDETestConfig::ClassSchemaTestClassNew(), ArcSDETestConfig::ClassNameTestClassNew());

            FdoPtr<FdoIApplySchema> apply = (FdoIApplySchema*)mConnection->CreateCommand (FdoCommandType_ApplySchema);
            FdoPtr<FdoFeatureSchema> schema = ArcSDETests::GetDefaultSchema(mConnection);  //FdoFeatureSchema::Create (schema_name, schema_description);
            FdoPtr<FdoClassCollection> classes = schema->GetClasses ();

            // build the data properties
            // assemble the class
            FdoPtr<FdoClass> cls = FdoClass::Create (ArcSDETestConfig::ClassNameTestClassNew(), class_new_description);
            FdoPtr<FdoPropertyDefinitionCollection> properties = cls->GetProperties ();
            FdoPtr<FdoDataPropertyDefinition> property;
            // Add read-only property:
            property = FdoDataPropertyDefinition::Create (L"ReadOnlyInt", L"Read-only integer");
            property->SetDataType (FdoDataType_Int32);
            property->SetNullable (false);
            property->SetIsAutoGenerated (true);
            property->SetReadOnly (true);
            property->SetDefaultValue(L"255");
            properties->Add (property);
            // Add two read-write properties with default values:
            property = FdoDataPropertyDefinition::Create (L"StringWithDefault1", L"String 1 with default value");
            property->SetDataType (FdoDataType_String);
            property->SetNullable (false);
            property->SetReadOnly (false);
            property->SetDefaultValue(L"'Default String 1'");
            properties->Add (property);
            property = FdoDataPropertyDefinition::Create (L"StringWithDefault2", L"String 2 with default value");
            property->SetDataType (FdoDataType_String);
            property->SetNullable (false);
            property->SetReadOnly (false);
            property->SetDefaultValue(L"'Default String 2'");
            properties->Add (property);

            // submit the new schema
            classes->Add (cls);
            apply->SetFeatureSchema (schema);
            apply->Execute ();
        }

        if (ArcSDETestConfig::SupportsUserManagedStringIds())  // doesn't work on all configurations
        {
            // Clean up from previous tests:
            CleanUpClass(mConnection, ArcSDETestConfig::ClassSchemaTestUserManagedId(), ArcSDETestConfig::ClassNameTestUserManagedId());

            // Create the class:
            FdoPtr<FdoIApplySchema> apply = (FdoIApplySchema*)mConnection->CreateCommand (FdoCommandType_ApplySchema);
            FdoPtr<FdoFeatureSchema> schema = ArcSDETests::GetDefaultSchema(mConnection);  //FdoFeatureSchema::Create (schema_name, schema_description);
            FdoPtr<FdoClassCollection> classes = schema->GetClasses ();
            FdoPtr<FdoFeatureClass> cls = FdoFeatureClass::Create (ArcSDETestConfig::ClassNameTestUserManagedId(), L"");
            FdoPtr<FdoPropertyDefinitionCollection> properties = cls->GetProperties ();
            FdoPtr<FdoDataPropertyDefinitionCollection> ids = cls->GetIdentityProperties();

            // Add a USER-MANAGED STRING IDENTITY property:
            FdoPtr<FdoDataPropertyDefinition> property = FdoDataPropertyDefinition::Create (Data[0]->mPropertyName, Data[0]->mPropertyDescription);
            property->SetDataType (Data[0]->mPropertyType);
            property->SetLength (Data[0]->mPropertyLength);
            property->SetPrecision (Data[0]->mPropertyPrecision);
            property->SetScale (Data[0]->mPropertyScale);
            property->SetNullable (Data[0]->mPropertyNullable);
            property->SetIsAutoGenerated(false);
            properties->Add (property);
            ids->Add(property);

            // Add a plain non-identity FdoInt32 property:
            property = FdoDataPropertyDefinition::Create (Data[5]->mPropertyName, Data[5]->mPropertyDescription);
            property->SetDataType (Data[5]->mPropertyType);
            property->SetLength (Data[5]->mPropertyLength);
            property->SetPrecision (Data[5]->mPropertyPrecision);
            property->SetScale (Data[5]->mPropertyScale);
            property->SetNullable (Data[5]->mPropertyNullable);
            properties->Add (property);

            // Add a geometry property:
            FdoPtr<FdoGeometricPropertyDefinition> geomProperty = FdoGeometricPropertyDefinition::Create(L"MyGeometry", L"");
            geomProperty->SetGeometryTypes (FdoGeometricType_Point);
            geomProperty->SetHasElevation(false);
            geomProperty->SetHasMeasure(false);
            properties->Add (geomProperty);
            cls->SetGeometryProperty(geomProperty);

            // Submit the new class:
            classes->Add (cls);
            apply->SetFeatureSchema (schema);
            apply->Execute ();
        }

        {
            // Create spatial context:
            FdoString* SPATIALCONTEXTNAME = L"TestSC_GeomZM3";
            CreateOrUpdateSpatialContext(mConnection, SPATIALCONTEXTNAME, -10000.1, -10000.1, 11474.73648, 11474.73648, L"4196");
            

            // Clean up previous tests:
            CleanUpClass(mConnection, ArcSDETestConfig::ClassSchemaTestClassGeomZm3(), ArcSDETestConfig::ClassNameTestClassGeomZm3());

            FdoPtr<FdoIApplySchema> apply = (FdoIApplySchema*)mConnection->CreateCommand (FdoCommandType_ApplySchema);
            FdoPtr<FdoFeatureSchema> schema = ArcSDETests::GetDefaultSchema(mConnection);
            FdoPtr<FdoClassCollection> classes = schema->GetClasses ();

            // build the geometry property
            // assemble the class
            FdoPtr<FdoFeatureClass> feature = FdoFeatureClass::Create (ArcSDETestConfig::ClassNameTestClassGeomZm3(), class_geom_zm_desc);
            FdoPtr<FdoPropertyDefinitionCollection> properties = feature->GetProperties ();
            FdoPtr<FdoDataPropertyDefinition> property = FdoDataPropertyDefinition::Create (L"FeatId", L"Unique identifier");
            property->SetDataType (FdoDataType_Int32);
            property->SetNullable (false);
            property->SetIsAutoGenerated (true);
            properties->Add (property);
            FdoPtr<FdoGeometricPropertyDefinition> geometry = FdoGeometricPropertyDefinition::Create (L"MyGeometry", L"Simple geometric property");
            geometry->SetHasElevation(true);
            geometry->SetHasMeasure(true);
            geometry->SetGeometryTypes (FdoGeometricType_Point | FdoGeometricType_Curve | FdoGeometricType_Surface | FdoGeometricType_Solid);
            geometry->SetSpatialContextAssociation(SPATIALCONTEXTNAME);
            properties->Add (geometry);
            FdoPtr<FdoDataPropertyDefinitionCollection> identities = feature->GetIdentityProperties ();
            identities->Add (property);
            SetClassCapabilities(feature, true, false);  // need to support rowlocking here, otherwise get error 

            // submit the new schema
            classes->Add (feature);
            apply->SetFeatureSchema (schema);
            apply->Execute ();
        }

        {
            // Clean up previous tests:
            CleanUpClass(mConnection, ArcSDETestConfig::ClassSchemaTestClassNullable(), ArcSDETestConfig::ClassNameTestClassNullable());

            FdoPtr<FdoIApplySchema> apply = (FdoIApplySchema*)mConnection->CreateCommand (FdoCommandType_ApplySchema);
            FdoPtr<FdoFeatureSchema> schema = ArcSDETests::GetDefaultSchema(mConnection);  //FdoFeatureSchema::Create (schema_name, schema_description);
            FdoPtr<FdoClassCollection> classes = schema->GetClasses ();

            // build the data properties
            // assemble the class
            FdoPtr<FdoClass> cls = FdoClass::Create (ArcSDETestConfig::ClassNameTestClassNullable(), L"");
            FdoPtr<FdoPropertyDefinitionCollection> properties = cls->GetProperties ();
            FdoPtr<FdoDataPropertyDefinition> property;
            // Add one read-write nullable property:
            property = FdoDataPropertyDefinition::Create (L"String1", L"String 1");
            property->SetDataType (FdoDataType_String);
            property->SetNullable (true);
            property->SetReadOnly (false);
            properties->Add (property);
            // Add an int32 sde-managed identity property:
            property = FdoDataPropertyDefinition::Create (L"FeatId", L"Unique identifier");
            property->SetDataType (FdoDataType_Int32);
            property->SetNullable (false);
            property->SetIsAutoGenerated (true);
            properties->Add (property);
            FdoPtr<FdoDataPropertyDefinitionCollection> identities = cls->GetIdentityProperties ();
            identities->Add (property);

            // submit the new schema
            classes->Add (cls);
            apply->SetFeatureSchema (schema);
            apply->Execute ();
        }

        bSchemaCreated = true;
    }
}

void BasicInsertTests::tearDown ()
{
    if ((mConnection != NULL) && (mConnection->GetConnectionState() != FdoConnectionState_Closed))
        mConnection->Close ();
}

/* Test basic insert operation. */
void BasicInsertTests::simple_insert ()
{
    if (CreateSchemaOnly())  return;

    try
    {
        mConnection = ArcSDETests::GetConnection ();
        mConnection->SetConnectionString (ArcSDETestConfig::ConnStringMetadcov());
        mConnection->Open ();

        // Clean up previous tests:
        CleanUpClass(mConnection, ArcSDETestConfig::ClassSchemaTestClassSimple(), ArcSDETestConfig::ClassNameTestClassSimple(), true);

/*
        FdoPtr<FdoIApplySchema> apply = (FdoIApplySchema*)mConnection->CreateCommand (FdoCommandType_ApplySchema);
        FdoPtr<FdoFeatureSchema> schema = ArcSDETests::GetDefaultSchema(mConnection);  //FdoFeatureSchema::Create (schema_name, schema_description);
        FdoPtr<FdoClassCollection> classes = schema->GetClasses ();

        // build a simple data property
        FdoPtr<FdoDataPropertyDefinition> property = FdoDataPropertyDefinition::Create (Data[0]->mPropertyName, Data[0]->mPropertyDescription);
        property->SetDataType (Data[0]->mPropertyType);
        property->SetLength (Data[0]->mPropertyLength);
        property->SetPrecision (Data[0]->mPropertyPrecision);
        property->SetScale (Data[0]->mPropertyScale);
        property->SetNullable (Data[0]->mPropertyNullable);

        // assemble the class
        FdoPtr<FdoClass> cls = FdoClass::Create (class_name, class_description);
        FdoPtr<FdoPropertyDefinitionCollection> properties = cls->GetProperties ();
        properties->Add (property);

        // submit the new schema
        classes->Add (cls);
        apply->SetFeatureSchema (schema);
        apply->Execute ();
*/

        // Insert a row:
        FdoPtr<FdoIInsert> insert = (FdoIInsert*)mConnection->CreateCommand (FdoCommandType_Insert);
        insert->SetFeatureClassName (ArcSDETestConfig::QClassNameTestClassSimple()); // misnomer, it's not a feature class
        FdoPtr<FdoPropertyValueCollection> values = insert->GetPropertyValues ();
        FdoPtr<FdoValueExpression> expression = (FdoValueExpression*)ArcSDETests::ParseByDataType(Data[0]->mPropertyData[0], Data[0]->mPropertyType);
        FdoPtr<FdoPropertyValue> value = FdoPropertyValue::Create (Data[0]->mPropertyName, expression);
        values->Add (value);
        FdoPtr<FdoIFeatureReader> reader = insert->Execute ();
        // none returned: reader->Close ();

        // check by doing a select
        FdoPtr<FdoISelect> selectCmd = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
        selectCmd->SetFeatureClassName (ArcSDETestConfig::QClassNameTestClassSimple());
        reader = selectCmd->Execute ();
        while (reader->ReadNext ())
        {
            FdoPtr<FdoStringValue> string = (FdoStringValue*)ArcSDETests::ParseByDataType (Data[0]->mPropertyData[0], Data[0]->mPropertyType);
            FdoString *string1 = string->GetString();
            FdoString *string2 = reader->GetString (Data[0]->mPropertyName);
            CPPUNIT_ASSERT_MESSAGE ("incorrect value", 0 == wcscmp (string1, string2));
        }
        reader->Close ();

        // Clean up after test is done:
        CleanUpClass(mConnection, ArcSDETestConfig::ClassSchemaTestClassSimple(), ArcSDETestConfig::ClassNameTestClassSimple(), true);

/*
        // Delete schema:
        cls->Delete ();
        apply->SetFeatureSchema (schema);
        apply->Execute ();
*/
    }
    catch (FdoException* ge) 
    {
        fail (ge);
    }
}

/* Test insert operation with data too large for the field. */
void BasicInsertTests::toobig_insert ()
{
    if (CreateSchemaOnly())  return;

    try
    {
        mConnection = ArcSDETests::GetConnection ();
        mConnection->SetConnectionString (ArcSDETestConfig::ConnStringMetadcov());
        mConnection->Open ();

        // Clean up previous tests:
        CleanUpClass(mConnection, ArcSDETestConfig::ClassSchemaTestClassSimple(), ArcSDETestConfig::ClassNameTestClassSimple(), true);

        // Insert a row with too much data:
        FdoPtr<FdoIInsert> insert = (FdoIInsert*)mConnection->CreateCommand (FdoCommandType_Insert);
        insert->SetFeatureClassName (ArcSDETestConfig::QClassNameTestClassSimple());
        FdoPtr<FdoPropertyValueCollection> values = insert->GetPropertyValues ();
        FdoPtr<FdoValueExpression> expression = (FdoValueExpression*)ArcSDETests::ParseByDataType (
            // 51 characters would do it
            L"'The factory of the future will have only two employees, a man and a dog. The man will be there to feed the dog. The dog will be there to keep the man from touching the equipment. - Warren G. Bennis'",
            FdoDataType_String);
        FdoPtr<FdoPropertyValue> value = FdoPropertyValue::Create (Data[0]->mPropertyName, expression);
        values->Add (value);
        try
        {
            FdoPtr<FdoIFeatureReader> reader = insert->Execute ();
            // none returned: reader->Close ();
            CPPUNIT_FAIL ("insert of oversize character string succeeeded");
        }
        catch (FdoException* ge)
        {
            // expected rsult
            ge->Release ();
        }

        // Clean up after test is done:
        CleanUpClass(mConnection, ArcSDETestConfig::ClassSchemaTestClassSimple(), ArcSDETestConfig::ClassNameTestClassSimple(), true);
    }
    catch (FdoException* ge) 
    {
        fail (ge);
    }
}

/* Test all data types insert operation. */
void BasicInsertTests::full_insert ()
{
    if (CreateSchemaOnly())  return;

    try
    {
        mConnection = ArcSDETests::GetConnection ();
        mConnection->SetConnectionString (ArcSDETestConfig::ConnStringMetadcovSingleDb());
        mConnection->Open ();

        // Clean up previous tests:
        CleanUpClass(mConnection, ArcSDETestConfig::ClassSchemaTestClassComplex(), ArcSDETestConfig::ClassNameTestClassComplex(), true);


/*
        FdoPtr<FdoIApplySchema> apply = (FdoIApplySchema*)mConnection->CreateCommand (FdoCommandType_ApplySchema);
        FdoPtr<FdoFeatureSchema> schema = ArcSDETests::GetDefaultSchema(mConnection);  //FdoFeatureSchema::Create (schema_name, schema_description);
        FdoPtr<FdoClassCollection> classes = schema->GetClasses ();

        // build the data properties
        // assemble the class
        FdoPtr<FdoClass> cls = FdoClass::Create (class_name, class_description);
        FdoPtr<FdoPropertyDefinitionCollection> properties = cls->GetProperties ();
        for (int i = 0; i < PropertyCount; i++)
        {
            FdoPtr<FdoDataPropertyDefinition> property = FdoDataPropertyDefinition::Create (Data[i]->mPropertyName, Data[i]->mPropertyDescription);
            property->SetDataType (Data[i]->mPropertyType);
            property->SetLength (Data[i]->mPropertyLength);
            property->SetPrecision (Data[i]->mPropertyPrecision);
            property->SetScale (Data[i]->mPropertyScale);
            property->SetNullable (Data[i]->mPropertyNullable);
            properties->Add (property);
        }

        // submit the new schema
        classes->Add (cls);
        apply->SetFeatureSchema (schema);
        apply->Execute ();
*/

        // Insert one row, providing several values in the *same* order as class properties appear:
        FdoPtr<FdoIInsert> insert = (FdoIInsert*)mConnection->CreateCommand (FdoCommandType_Insert);
        insert->SetFeatureClassName (ArcSDETestConfig::QClassNameTestClassComplex()); // misnomer, it's not a feature class
        FdoPtr<FdoPropertyValueCollection> values = insert->GetPropertyValues ();
        for (int i = 0; i < PropertyCount; i++)
        {
            FdoPtr<FdoValueExpression> expression;
            expression = (FdoValueExpression*)ArcSDETests::ParseByDataType (Data[i]->mPropertyData[0], Data[i]->mPropertyType);
            FdoPtr<FdoPropertyValue> value = FdoPropertyValue::Create (Data[i]->mPropertyName, expression);
            values->Add (value);
        }
        FdoPtr<FdoIFeatureReader> reader = insert->Execute ();
        // none returned: reader->Close ();

        // check by doing a select
        FdoPtr<FdoISelect> selectCmd = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
        selectCmd->SetFeatureClassName (ArcSDETestConfig::QClassNameTestClassComplex());
        reader = selectCmd->Execute ();
        CPPUNIT_ASSERT_MESSAGE("Failed to read 1st inserted row", reader->ReadNext());
        for (int i = 0; i < PropertyCount; i++)
            checkEqual(reader, Data[i]->mPropertyName, Data[i]->mPropertyType, Data[i]->mPropertyData[0]);
        CPPUNIT_ASSERT_MESSAGE("Shouldn't be anymore rows", !reader->ReadNext());
        reader->Close ();


        // Clean up previous tests:
        CleanUpClass(mConnection, ArcSDETestConfig::ClassSchemaTestClassComplex(), ArcSDETestConfig::ClassNameTestClassComplex(), true);

        // Insert one row, providing several values in the *reverse* order as class properties appear:
        insert = (FdoIInsert*)mConnection->CreateCommand (FdoCommandType_Insert);
        insert->SetFeatureClassName (ArcSDETestConfig::QClassNameTestClassComplex()); // misnomer, it's not a feature class
        values = insert->GetPropertyValues ();
        for (int iPropertyIndex = 0; iPropertyIndex < PropertyCount; iPropertyIndex++)
        {
            int iDataIndex = (PropertyCount - 1) - iPropertyIndex;
            FdoPtr<FdoValueExpression> expression;
            expression = (FdoValueExpression*)ArcSDETests::ParseByDataType (Data[iDataIndex]->mPropertyData[0], Data[iDataIndex]->mPropertyType);
            FdoPtr<FdoPropertyValue> value = FdoPropertyValue::Create(Data[iDataIndex]->mPropertyName, expression);
            values->Add(value);
        }
        reader = insert->Execute ();
        // none returned: reader->Close ();

        // check by doing a select
        selectCmd = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
        selectCmd->SetFeatureClassName (ArcSDETestConfig::QClassNameTestClassComplex());
        reader = selectCmd->Execute ();
        CPPUNIT_ASSERT_MESSAGE("Failed to read next", reader->ReadNext());
        for (int i = 0; i < PropertyCount; i++)
            checkEqual(reader, Data[i]->mPropertyName, Data[i]->mPropertyType, Data[i]->mPropertyData[0]);
        CPPUNIT_ASSERT_MESSAGE("Shouldn't be anymore rows", !reader->ReadNext());
        reader->Close ();


        // Clean up previous tests:
        CleanUpClass(mConnection, ArcSDETestConfig::ClassSchemaTestClassComplex(), ArcSDETestConfig::ClassNameTestClassComplex(), true);

        // Insert one row, providing *mostly NULL value* in the *same* order as class properties appear:
        insert = (FdoIInsert*)mConnection->CreateCommand (FdoCommandType_Insert);
        insert->SetFeatureClassName (ArcSDETestConfig::QClassNameTestClassComplex()); // misnomer, it's not a feature class
        values = insert->GetPropertyValues ();
        FdoPtr<FdoPropertyValue> value;
        FdoPtr<FdoValueExpression> expression;
        for (int i = 0; i < PropertyCount; i++)
        {
            expression = (FdoValueExpression*)ArcSDETests::ParseByDataType (Data[i]->mPropertyData[1], Data[i]->mPropertyType);
            value = FdoPropertyValue::Create (Data[i]->mPropertyName, expression);
            values->Add (value);
        }
        // Add NULL geometry value:
        FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
        FdoPtr<FdoGeometryValue> geomExpr = FdoGeometryValue::Create();
        geomExpr->SetNullValue();
        value = FdoPropertyValue::Create (L"SHAPE", geomExpr);
        values->Add (value);
        // Do the insert:
        reader = insert->Execute ();
        // none returned: reader->Close ();

        // check by doing a select
        selectCmd = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
        selectCmd->SetFeatureClassName (ArcSDETestConfig::QClassNameTestClassComplex());
        reader = selectCmd->Execute ();
        CPPUNIT_ASSERT_MESSAGE("Failed to read next", reader->ReadNext());
        for (int i = 0; i < PropertyCount; i++)
            checkEqual(reader, Data[i]->mPropertyName, Data[i]->mPropertyType, Data[i]->mPropertyData[1]);
        CPPUNIT_ASSERT_MESSAGE("Geometry column should be NULL", reader->IsNull(L"SHAPE"));
        CPPUNIT_ASSERT_MESSAGE("Shouldn't be anymore rows", !reader->ReadNext());
        reader->Close ();

        // Clean up after test:
        CleanUpClass(mConnection, ArcSDETestConfig::ClassSchemaTestClassComplex(), ArcSDETestConfig::ClassNameTestClassComplex(), true);

/*
        // Delete schema:
        cls->Delete ();
        apply->SetFeatureSchema (schema);
        apply->Execute ();
*/
    }
    catch (FdoException* ge) 
    {
        fail (ge);
    }
}

/* Test geometry insert operation. */
void BasicInsertTests::geometry_insert ()
{
    if (CreateSchemaOnly())  return;

    try
    {
        mConnection = ArcSDETests::GetConnection ();
        mConnection->SetConnectionString (ArcSDETestConfig::ConnStringMetadcovSingleDb());
        mConnection->Open ();

        // Clean up previous tests:
        CleanUpClass(mConnection, ArcSDETestConfig::ClassSchemaTestClassComplex(), ArcSDETestConfig::ClassNameTestClassComplex(), true);

/*
        FdoPtr<FdoIApplySchema> apply = (FdoIApplySchema*)mConnection->CreateCommand (FdoCommandType_ApplySchema);
        FdoPtr<FdoFeatureSchema> schema = ArcSDETests::GetDefaultSchema(mConnection);  //FdoFeatureSchema::Create (schema_name, schema_description);
        FdoPtr<FdoClassCollection> classes = schema->GetClasses ();

        // build the geometry property
        // assemble the class
        FdoPtr<FdoFeatureClass> feature = FdoFeatureClass::Create (class_name, class_description);
        FdoPtr<FdoPropertyDefinitionCollection> properties = feature->GetProperties ();
        FdoPtr<FdoDataPropertyDefinition> property = FdoDataPropertyDefinition::Create (L"FeatId", L"Unique identifier");
        property->SetDataType (FdoDataType_Int32);
        property->SetNullable (false);
        property->SetIsAutoGenerated (true);
        properties->Add (property);
        FdoPtr<FdoGeometricPropertyDefinition> geometry = FdoGeometricPropertyDefinition::Create (L"MyGeometry", L"Simple geometric property");
        geometry->SetGeometryTypes (FdoGeometricType_Point);
        properties->Add (geometry);
        FdoPtr<FdoDataPropertyDefinitionCollection> identities = feature->GetIdentityProperties ();
        identities->Add (property);

        // submit the new schema
        classes->Add (feature);
        apply->SetFeatureSchema (schema);
        apply->Execute ();
*/

        FdoPtr<FdoIInsert> insert = (FdoIInsert*)mConnection->CreateCommand (FdoCommandType_Insert);
        insert->SetFeatureClassName (ArcSDETestConfig::QClassNameTestClassComplex());
        FdoPtr<FdoPropertyValueCollection> values = insert->GetPropertyValues ();
        FdoPtr<FdoGeometryValue> geomExpression = (FdoGeometryValue*)FdoExpression::Parse (L"GEOMFROMTEXT ('POLYGON XY (( 5190 5120, 5220 5150, 5190 5150, 5190 5120 ))')");
        FdoPtr<FdoPropertyValue> value = FdoPropertyValue::Create (L"SHAPE", geomExpression);
        values->Add (value);
        FdoPtr<FdoIFeatureReader> reader = insert->Execute ();
        // none returned: reader->Close ();

        // check by doing a select
        FdoPtr<FdoISelect> selectCmd = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
        selectCmd->SetFeatureClassName (ArcSDETestConfig::QClassNameTestClassComplex());
        reader = selectCmd->Execute ();
        while (reader->ReadNext ())
        {
            FdoPtr<FdoByteArray> fetched = reader->GetGeometry (L"SHAPE");
            FdoPtr<FdoByteArray> reference = geomExpression->GetGeometry ();
            for (int i = 0; i < fetched->GetCount (); i++)
                if ((*fetched)[i] != (*reference)[i])
                    CPPUNIT_FAIL ("incorrect geometry value");
        }
        reader->Close ();

        // Clean up after test:
        CleanUpClass(mConnection, ArcSDETestConfig::ClassSchemaTestClassComplex(), ArcSDETestConfig::ClassNameTestClassComplex(), true);

/*
        // Delete schema:
        feature->Delete ();
        apply->SetFeatureSchema (schema);
        apply->Execute ();
*/
    }
    catch (FdoException* ge) 
    {
        fail (ge);
    }
}


/* Failure tests */
void BasicInsertTests::failure_insert ()
{
    if (CreateSchemaOnly())  return;

    try
    {
        mConnection = ArcSDETests::GetConnection ();
        mConnection->SetConnectionString (ArcSDETestConfig::ConnStringMetadcovSingleDb());
        mConnection->Open ();

        ////////////////////////////////////////////////////////
        // Test inserting a feature with curved segments:
        ////////////////////////////////////////////////////////
        
        // Clean up previous tests:
        CleanUpClass(mConnection, ArcSDETestConfig::ClassSchemaTestClassComplex(), ArcSDETestConfig::ClassNameTestClassComplex(), true);

        // Insert one row:
        FdoPtr<FdoIInsert> insert = (FdoIInsert*)mConnection->CreateCommand (FdoCommandType_Insert);
        insert->SetFeatureClassName (ArcSDETestConfig::QClassNameTestClassComplex());
        FdoPtr<FdoPropertyValueCollection> values = insert->GetPropertyValues ();
        for (int i = 0; i < PropertyCount; i++)
        {
            FdoPtr<FdoValueExpression> expression;
            expression = (FdoValueExpression*)ArcSDETests::ParseByDataType (Data[i]->mPropertyData[0], Data[i]->mPropertyType);
            FdoPtr<FdoPropertyValue> value = FdoPropertyValue::Create (Data[i]->mPropertyName, expression);
            values->Add (value);
        }

        // add a curvestring with one circular arc segment:
        FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
        FdoPtr<FdoCurveSegmentCollection> curveSegments = FdoCurveSegmentCollection::Create();
        FdoPtr<FdoIDirectPosition> pos1 = gf->CreatePositionXY(0.0, 0.0);
        FdoPtr<FdoIDirectPosition> pos2 = gf->CreatePositionXY(1.0, 1.0);
        FdoPtr<FdoIDirectPosition> pos3 = gf->CreatePositionXY(2.0, 0.0);
        FdoPtr<FdoICircularArcSegment> arcSegment1 = gf->CreateCircularArcSegment(pos1, pos2, pos3);
        curveSegments->Add(arcSegment1);
        FdoPtr<FdoICurveString> curveString = (FdoICurveString*)gf->CreateCurveString( curveSegments );
        FdoByteArray *byteArray = gf->GetFgf(curveString);
        FdoPtr<FdoGeometryValue> geomVal = FdoGeometryValue::Create(byteArray);
        byteArray->Release ();
        FdoPtr<FdoPropertyValue> value = FdoPropertyValue::Create (L"SHAPE", geomVal);
        values->Add (value);

/*
        // add a curvepolygon with 2 circular arc segments:
        FdoPtr<FdoPropertyValue> value = values->GetItem(L"SHAPE");
        FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
        FdoPtr<FdoCurveSegmentCollection> curveSegments = FdoCurveSegmentCollection::Create();
        FdoPtr<FdoIDirectPosition> pos1 = gf->CreatePositionXY(0.0, 0.0);
        FdoPtr<FdoIDirectPosition> pos2 = gf->CreatePositionXY(1.0, 1.0);
        FdoPtr<FdoIDirectPosition> pos3 = gf->CreatePositionXY(2.0, 0.0);
        FdoPtr<FdoICircularArcSegment> arcSegment1 = gf->CreateCircularArcSegment(pos1, pos2, pos3);
        curveSegments->Add(arcSegment1);
        FdoPtr<FdoIDirectPosition> pos4 = gf->CreatePositionXY(2.0, 0.0);
        FdoPtr<FdoIDirectPosition> pos5 = gf->CreatePositionXY(1.0, -1.0);
        FdoPtr<FdoIDirectPosition> pos6 = gf->CreatePositionXY(0.0, 0.0);
        FdoPtr<FdoICircularArcSegment> arcSegment2 = gf->CreateCircularArcSegment(pos4, pos5, pos6);
        curveSegments->Add(arcSegment2);
        FdoPtr<FdoIRing> ring = (FdoIRing*)gf->CreateRing( curveSegments );
        FdoPtr<FdoICurvePolygon> curvePoly = gf->CreateCurvePolygon(ring, NULL);
        FdoByteArray *byteArray = gf->GetFgf(curvePoly);
        FdoPtr<FdoGeometryValue> geomVal = FdoGeometryValue::Create(byteArray);
        value->SetValue(geomVal);
*/

/*
        // add a multigeometry containing different geometry types:
        FdoPtr<FdoPropertyValue> value = values->GetItem(L"SHAPE");
        FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
        FdoPtr<FdoGeometryCollection> geomColl = FdoGeometryCollection::Create();

        // Create/add a point geometry:
        FdoPtr<FdoIDirectPosition> pos1 = gf->CreatePositionXY(0.0, 1.0);
        FdoPtr<FdoIPoint> point = gf->CreatePoint(pos1);
        geomColl->Add(point);

        // Create/add a linestring geometry:
        FdoPtr<FdoIDirectPosition> pos2 = gf->CreatePositionXY(0.0, 0.0);
        FdoPtr<FdoIDirectPosition> pos3 = gf->CreatePositionXY(1.0, 1.0);
        FdoPtr<FdoIDirectPosition> pos4 = gf->CreatePositionXY(2.0, 0.0);
        FdoPtr<FdoDirectPositionCollection> posColl = FdoDirectPositionCollection::Create();
        posColl->Add(pos2);
        posColl->Add(pos3);
        posColl->Add(pos4);
        FdoPtr<FdoILineString> lineString = gf->CreateLineString(posColl);
        geomColl->Add(lineString);

        // Create the multigeometry, and get its FGF:
        FdoPtr<FdoIMultiGeometry> multiGeom = gf->CreateMultiGeometry(geomColl);
        FdoByteArray *byteArray = gf->GetFgf(multiGeom);
        FdoPtr<FdoGeometryValue> geomVal = FdoGeometryValue::Create(byteArray);
        value->SetValue(geomVal);
*/

        try
        {
            FdoPtr<FdoIFeatureReader> reader = insert->Execute ();
            reader->Close ();
            CPPUNIT_FAIL("inserting a curve string succeeded; it should have failed");
        }
        catch (FdoException *e)
        {
            // We expect an exception here
            e->Release();
        }


        ////////////////////////////////////////////////////////
        // Test inserting a feature with wrong property name:
        ////////////////////////////////////////////////////////
        
        insert = (FdoIInsert*)mConnection->CreateCommand (FdoCommandType_Insert);
        insert->SetFeatureClassName (ArcSDETestConfig::QClassNameTestClassComplex());
        values = insert->GetPropertyValues ();
        value = FdoPropertyValue::Create (L"InvalidPropertyname", NULL);
        values->Add (value);
        try
        {
            FdoPtr<FdoIFeatureReader> reader = insert->Execute ();
            reader->Close ();
            CPPUNIT_FAIL("inserting a value with an invalid property name succeeded; it should have failed");
        }
        catch (FdoException *e)
        {
            // We expect an exception here
            e->Release();
        }

    }
    catch (FdoException* ge) 
    {
        // UNEXPECTED exceptions are treated as failures:
        fail (ge);
        ge->Release();
    }
}


/* Batch insert tests */
void BasicInsertTests::batch_insert ()
{
    if (CreateSchemaOnly())  return;

    try
    {
        mConnection = ArcSDETests::GetConnection ();
        mConnection->SetConnectionString (ArcSDETestConfig::ConnStringMetadcovSingleDb());
        mConnection->Open ();

        // Clean up previous tests:
        CleanUpClass(mConnection, ArcSDETestConfig::ClassSchemaTestClassComplex(), ArcSDETestConfig::ClassNameTestClassComplex(), true);

/*
        FdoPtr<FdoIApplySchema> apply = (FdoIApplySchema*)mConnection->CreateCommand (FdoCommandType_ApplySchema);
        FdoPtr<FdoFeatureSchema> schema = ArcSDETests::GetDefaultSchema(mConnection);  //FdoFeatureSchema::Create (schema_name, schema_description);
        FdoPtr<FdoClassCollection> classes = schema->GetClasses ();

        // build the data properties
        // assemble the class
        FdoPtr<FdoClass> cls = FdoClass::Create (class_name, class_description);
        FdoPtr<FdoPropertyDefinitionCollection> properties = cls->GetProperties ();
        for (int i = 0; i < PropertyCount; i++)
        {
            FdoPtr<FdoDataPropertyDefinition> property = FdoDataPropertyDefinition::Create (Data[i]->mPropertyName, Data[i]->mPropertyDescription);
            property->SetDataType (Data[i]->mPropertyType);
            property->SetLength (Data[i]->mPropertyLength);
            property->SetPrecision (Data[i]->mPropertyPrecision);
            property->SetScale (Data[i]->mPropertyScale);
            property->SetNullable (Data[i]->mPropertyNullable);
            properties->Add (property);
        }

        // submit the new schema
        classes->Add (cls);
        apply->SetFeatureSchema (schema);
        apply->Execute ();
*/

        FdoPtr<FdoIInsert> insert = (FdoIInsert*)mConnection->CreateCommand (FdoCommandType_Insert);
        insert->SetFeatureClassName (ArcSDETestConfig::QClassNameTestClassComplex()); // misnomer, it's not a feature class
        FdoPtr<FdoPropertyValueCollection> values = insert->GetPropertyValues ();
        values->Clear(); // NOTE: this should probably not be necessary!

        // Set up the property value collection:

        FdoPtr<FdoPropertyValue> propertyValue;
        FdoPtr<FdoLiteralValue> expression;
        FdoPtr<FdoParameter>  parameter;

        parameter = FdoParameter::Create(L"Param0");
        propertyValue = FdoPropertyValue::Create(Data[0]->mPropertyName, parameter);
        values->Add(propertyValue);

        parameter = FdoParameter::Create(L"Param1");
        propertyValue = FdoPropertyValue::Create(Data[1]->mPropertyName, parameter);
        values->Add(propertyValue);

        // NOTE: we're purposefully pointing the Data[2] property to Param1 parameter, to test the parameter logic
        parameter = FdoParameter::Create(L"Param1");
        propertyValue = FdoPropertyValue::Create(Data[2]->mPropertyName, parameter);
        values->Add(propertyValue);

        // NOTE: we're purposefully leaving the remaining properties (MyShort, MyInt, etc) empty


        // Fill the batch parameter value collection with multiple rows of data:
        FdoPtr<FdoBatchParameterValueCollection> batchParameters = insert->GetBatchParameterValues();
        for (int i=0; i<5; i++)
        {
            FdoPtr<FdoParameterValueCollection> parameterValues;
            FdoPtr<FdoParameterValue>           parameterValue;

            parameterValues = FdoParameterValueCollection::Create();

            expression = static_cast<FdoLiteralValue*>(ArcSDETests::ParseByDataType (BatchData[i][0], FdoDataType_String));
            parameterValue = FdoParameterValue::Create(L"Param0", expression);
            parameterValues->Add(parameterValue);

            expression = static_cast<FdoLiteralValue*>(ArcSDETests::ParseByDataType (BatchData[i][1], FdoDataType_Single));
            parameterValue = FdoParameterValue::Create(L"Param1", expression);
            parameterValues->Add(parameterValue);

            batchParameters->Add(parameterValues);
        }


        // Perform the batch insert:
        FdoPtr<FdoIFeatureReader> reader = insert->Execute ();
        // none returned: reader->Close ();


        // check by doing a select
        FdoPtr<FdoISelect> selectCmd = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
        selectCmd->SetFeatureClassName (ArcSDETestConfig::QClassNameTestClassComplex());
        reader = selectCmd->Execute ();
        int iRow = 0;
        while (reader->ReadNext ())
        {
            //NOTE: I'm making the assumption here that I'm receiving rows in the same order that I inserted them
            checkEqual(reader, Data[0]->mPropertyName, Data[0]->mPropertyType, BatchData[iRow][0]);
            checkEqual(reader, Data[1]->mPropertyName, Data[1]->mPropertyType, BatchData[iRow][1]);
            checkEqual(reader, Data[2]->mPropertyName, Data[2]->mPropertyType, BatchData[iRow][1]);
            iRow++;
        }
        reader->Close ();

        // Clean up after test:
        CleanUpClass(mConnection, ArcSDETestConfig::ClassSchemaTestClassComplex(), ArcSDETestConfig::ClassNameTestClassComplex(), true);

/*
        // Delete schema:
        cls->Delete ();
        apply->SetFeatureSchema (schema);
        apply->Execute ();
*/
    }
    catch (FdoException* ge) 
    {
        // UNEXPECTED exceptions are treated as failures:
        fail (ge);
        ge->Release();
    }
}



/* Test default values and read-only properties in insert operation. */
void BasicInsertTests::defaults_insert ()
{
    if (CreateSchemaOnly())  return;

    try
    {
        mConnection = ArcSDETests::GetConnection ();
        mConnection->SetConnectionString (ArcSDETestConfig::ConnStringMetadcov());
        mConnection->Open ();

        // Clean up previous tests:
        CleanUpClass(mConnection, ArcSDETestConfig::ClassSchemaTestClassNew(), ArcSDETestConfig::ClassNameTestClassNew(), true);

        // Insert one row, setting only 1 of the 3 properties, rely on default values for the other 2:
        FdoPtr<FdoIInsert> insert = (FdoIInsert*)mConnection->CreateCommand (FdoCommandType_Insert);
        insert->SetFeatureClassName (ArcSDETestConfig::QClassNameTestClassNew()); // misnomer, it's not a feature class
        FdoPtr<FdoPropertyValueCollection> values = insert->GetPropertyValues ();
        {
            FdoPtr<FdoPropertyValue> value;
            FdoPtr<FdoValueExpression> expression;
            expression = (FdoValueExpression*)ArcSDETests::ParseByDataType (L"'Inserted String 1'", FdoDataType_String);
            value = FdoPropertyValue::Create(L"StringWithDefault1", expression);
            values->Add(value);
        }
        FdoPtr<FdoIFeatureReader> reader = insert->Execute ();
        // none returned: reader->Close ();

        // check by doing a select
        FdoPtr<FdoISelect> selectCmd = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
        selectCmd->SetFeatureClassName (ArcSDETestConfig::QClassNameTestClassNew());
        reader = selectCmd->Execute ();
        CPPUNIT_ASSERT_MESSAGE("Failed to read 1st inserted row", reader->ReadNext());
        checkEqual(reader, L"ReadOnlyInt", FdoDataType_Int32, L"255");
        checkEqual(reader, L"StringWithDefault1", FdoDataType_String, L"'Inserted String 1'");
        checkEqual(reader, L"StringWithDefault2", FdoDataType_String, L"'Default String 2'");
        CPPUNIT_ASSERT_MESSAGE("Shouldn't be anymore rows", !reader->ReadNext());
        reader->Close ();



        // Now check for failure when trying to set a read-only property:
        insert = (FdoIInsert*)mConnection->CreateCommand (FdoCommandType_Insert);
        insert->SetFeatureClassName (ArcSDETestConfig::QClassNameTestClassNew()); // misnomer, it's not a feature class
        values = insert->GetPropertyValues ();
        {
            FdoPtr<FdoPropertyValue> value;
            FdoPtr<FdoValueExpression> expression;
            expression = (FdoValueExpression*)ArcSDETests::ParseByDataType (L"123", FdoDataType_Int32);
            value = FdoPropertyValue::Create(L"ReadOnlyInt", expression);
            values->Add(value);
        }
        try
        {
            FdoPtr<FdoIFeatureReader> reader = insert->Execute ();

            // If we get here, the test failed:
            CPPUNIT_ASSERT_MESSAGE("Expected exception when setting a read-only property", false);
        }
        catch (FdoException *e)
        {
            // If we get here, the test succeeded.
            e->Release();
        }
    }
    catch (FdoException* ge) 
    {
        fail (ge);
    }
}


void BasicInsertTests::geometry_insert_ZM()
{
    if (CreateSchemaOnly())  return;

    try
    {
        mConnection = ArcSDETests::GetConnection ();
        mConnection->SetConnectionString (ArcSDETestConfig::ConnStringMetadcov());
        mConnection->Open ();

        // Clean up previous tests:
        CleanUpClass(mConnection, ArcSDETestConfig::ClassSchemaTestClassGeomZm3(), ArcSDETestConfig::ClassNameTestClassGeomZm3(), true);

        // Create a bunch of geometries, collecting them in a FdoCollectionBase:
        ///////////////////////////////////////////////////////////////////

        FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
        FdoPtr<FdoGeometryCollection> collection = FdoGeometryCollection::Create();
        FdoPtr<FdoIDirectPosition> pos;
        FdoPtr<FdoIDirectPosition> pos2;
        FdoPtr<FdoIGeometry> geom;

        // Point:
        FdoPtr<FdoIPoint> point;
        pos = gf->CreatePositionXYZM(1.0, 522.5, 763.0, 223.0);
        geom = gf->CreatePoint(pos);
        collection->Add(geom);

        // MultiPoint:
        /* // NOTE: there seems to be problems at the ArcSDE level to roundtrip multipoint geometries, at least those with Z and M values
        FdoPtr<FdoPointCollection> points = FdoPointCollection::Create();
        pos = gf->CreatePositionXYZM(1.1, 2.2, 3.0, 4.0);
        point = gf->CreatePoint(pos);
        points->Add(point);
        pos = gf->CreatePositionXYZM(5.5, 6.6, 7.0, 8.0);
        point = gf->CreatePoint(pos);
        points->Add(point);
        pos = gf->CreatePositionXYZM(9.9, 10.10, 11.0, 12.0);
        point = gf->CreatePoint(pos);
        points->Add(point);
        geom = gf->CreateMultiPoint(points);
        collection->Add(geom);
        */

        // Line:
        FdoPtr<FdoDirectPositionCollection> positions = FdoDirectPositionCollection::Create();
        pos = gf->CreatePositionXYZM(232.2, 123.2, 0, 0);
        positions->Add(pos);
        pos = gf->CreatePositionXYZM(232.2, 1244.2, 5, 0);
        positions->Add(pos);
        pos = gf->CreatePositionXYZM(2232.2, 123.2, 0, 2);
        positions->Add(pos);
        geom = gf->CreateLineString(positions);
        collection->Add(geom);

        // MultiLine:
        FdoPtr<FdoLineStringCollection> lineStrings = FdoLineStringCollection::Create();
        positions = FdoDirectPositionCollection::Create();
        pos = gf->CreatePositionXYZM(232.2, 123.2, 0, 0);
        positions->Add(pos);
        pos = gf->CreatePositionXYZM(232.2, 1244.2, 5, 0);
        positions->Add(pos);
        pos = gf->CreatePositionXYZM(2232.2, 123.2, 0, 2);
        positions->Add(pos);
        FdoPtr<FdoILineString> lineString = gf->CreateLineString(positions);
        lineStrings->Add(lineString);

        FdoPtr<FdoDirectPositionCollection> positions2 = FdoDirectPositionCollection::Create();
        pos = gf->CreatePositionXYZM(111.2, 123.2, 0, 0);
        positions2->Add(pos);
        pos = gf->CreatePositionXYZM(232.2, 1233.2, 6, 0);
        positions2->Add(pos);
        pos = gf->CreatePositionXYZM(344.6, 123.2, 1, 2);
        positions2->Add(pos);
        FdoPtr<FdoILineString> lineString2 = gf->CreateLineString(positions2);
        lineStrings->Add(lineString2);

        geom = gf->CreateMultiLineString(lineStrings);
        collection->Add(geom);

        // Polygon:
        FdoPtr<FdoLinearRingCollection> intRings = FdoLinearRingCollection::Create();
        FdoPtr<FdoILinearRing> extRing = CreatePolygonRing(5200.0, 5130.0, 20, 100, true);
        FdoPtr<FdoILinearRing> intRing;
        intRing = CreatePolygonRing(5250.0, 5130.0, 20, 11, false);
        intRings->Add(intRing);
        intRing = CreatePolygonRing(5150.0, 5144.0, 20, 9, false);
        intRings->Add(intRing);
        geom = gf->CreatePolygon(extRing, intRings);
        collection->Add(geom);

        // MultiPolygon:
        FdoPtr<FdoPolygonCollection> polygons = FdoPolygonCollection::Create();

        intRings = FdoLinearRingCollection::Create();
        extRing = CreatePolygonRing(5200.0, 5130.0, 20, 100, true);
        intRing = CreatePolygonRing(5250.0, 5130.0, 20, 11, false);
        intRings->Add(intRing);
        intRing = CreatePolygonRing(5150.0, 5144.0, 20, 9, false);
        intRings->Add(intRing);
        FdoPtr<FdoIPolygon> polygon = gf->CreatePolygon(extRing, intRings);
        polygons->Add(polygon);

        intRings = FdoLinearRingCollection::Create();
        extRing = CreatePolygonRing(6200.0, 5130.0, 20, 100, true);
        intRing = CreatePolygonRing(6250.0, 5130.0, 20, 11, false);
        intRings->Add(intRing);
        intRing = CreatePolygonRing(6150.0, 5144.0, 20, 9, false);
        intRings->Add(intRing);
        polygon = gf->CreatePolygon(extRing, intRings);
        polygons->Add(polygon);

        geom = gf->CreateMultiPolygon(polygons);
        collection->Add(geom);


        // Test inserting each of these geometries, one at a time:
        ///////////////////////////////////////////////////////////////////

        for (int i=0; i<collection->GetCount(); i++)
        {
            // delete all this table's rows:
            CleanUpClass(mConnection, ArcSDETestConfig::ClassSchemaTestClassGeomZm3(), ArcSDETestConfig::ClassNameTestClassGeomZm3(), true);

            // insert a value:
            FdoPtr<FdoIInsert> insert = (FdoIInsert*)mConnection->CreateCommand (FdoCommandType_Insert);
            insert->SetFeatureClassName (ArcSDETestConfig::QClassNameTestClassGeomZm3());
            FdoPtr<FdoPropertyValueCollection> values = insert->GetPropertyValues ();
            FdoPtr<FdoIGeometry> aGeometry = collection->GetItem(i);
            FdoPtr<FdoByteArray> byteArray = gf->GetFgf(aGeometry);
            FdoPtr<FdoGeometryValue> geomExpression = FdoGeometryValue::Create(byteArray);
            FdoPtr<FdoPropertyValue> value = FdoPropertyValue::Create (L"MyGeometry", geomExpression);
            values->Add (value);
            FdoPtr<FdoIFeatureReader> reader = insert->Execute ();
            // none returned: reader->Close ();

            // check by doing a select
            FdoPtr<FdoISelect> selectCmd = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
            selectCmd->SetFeatureClassName (ArcSDETestConfig::QClassNameTestClassGeomZm3());
            reader = selectCmd->Execute ();
            CPPUNIT_ASSERT_MESSAGE("Should have retrieved 1 row, got 0 rows", reader->ReadNext());
            FdoPtr<FdoByteArray> fetched = reader->GetGeometry (L"MyGeometry");
            FdoPtr<FdoByteArray> reference = geomExpression->GetGeometry ();
            FdoPtr<FdoIGeometry> fetchedGeom = gf->CreateGeometryFromFgf(fetched);
            FdoPtr<FdoIGeometry> referenceGeom = gf->CreateGeometryFromFgf(reference);

            // Compare the two geometries:
            FdoString *fetchedText = fetchedGeom->GetText();
            FdoString *referenceText = referenceGeom->GetText();
            bool bEquivalent = GeometriesEquivalent(fetchedGeom, referenceGeom);
            CPPUNIT_ASSERT_MESSAGE("Queried geometry != inserted geometry", bEquivalent);

            // Finish up this row:
            CPPUNIT_ASSERT_MESSAGE("Should have retrieved 1 row, got 2 or more rows", !reader->ReadNext());
            reader->Close ();
        }
    }
    catch (FdoException* ge)
    {
        fail (ge);
    }
}


FdoILinearRing* BasicInsertTests::CreatePolygonRing(double x, double y, double z, double radius, bool bExterior)
{
    FdoPtr<FdoDirectPositionCollection> positions = FdoDirectPositionCollection::Create();
    FdoPtr<FdoIDirectPosition> pos;
    FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();

    double order = bExterior ? 1.0 : -1.0;
    pos = gf->CreatePositionXYZM(x - radius, y + order*radius, z, 1.0);
    positions->Add(pos);
    pos = gf->CreatePositionXYZM(x - radius, y - order*radius, z, 2.0);
    positions->Add(pos);
    pos = gf->CreatePositionXYZM(x + radius, y - order*radius, z, 3.0);
    positions->Add(pos);
    pos = gf->CreatePositionXYZM(x + radius, y + order*radius, z, 1.0);
    positions->Add(pos);
    pos = gf->CreatePositionXYZM(x - radius, y + order*radius, z, 1.0);
    positions->Add(pos);

    FdoILinearRing* ring = gf->CreateLinearRing(positions);
    return ring;
}



void BasicInsertTests::user_managed_ids()
{
    if (CreateSchemaOnly())  return;

    if (ArcSDETestConfig::SupportsUserManagedStringIds())
    {
        try
        {
            mConnection = ArcSDETests::GetConnection ();
            mConnection->SetConnectionString (ArcSDETestConfig::ConnStringMetadcov());
            mConnection->Open ();

            // Clean up from previous tests:
            CleanUpClass(mConnection, ArcSDETestConfig::ClassSchemaTestUserManagedId(), ArcSDETestConfig::ClassNameTestUserManagedId(), true);

            // Insert a couple rows of data:
            /////////////////////////////////////////////////////////////////////////

            FdoPtr<FdoIInsert> insert = (FdoIInsert*)mConnection->CreateCommand(FdoCommandType_Insert);
            FdoPtr<FdoPropertyValueCollection> propValues;
            FdoPtr<FdoPropertyValue> propVal;
            FdoPtr<FdoIFeatureReader> reader;
            FdoString* id1 = L"Bob";
            FdoString* id2 = L"George";
            FdoInt32  int1 = 78943;
            FdoInt32  int2 = -23943;
            FdoString* geom1 = L"GEOMFROMTEXT ('POINT XY (100 50)')";
            FdoString* geom2 = L"GEOMFROMTEXT ('POINT XY (100 100)')";
            FdoString *returnedId = NULL;

            // insert one row:
            insert->SetFeatureClassName(ArcSDETestConfig::QClassNameTestUserManagedId());
            propValues = insert->GetPropertyValues();
            FdoPtr<FdoStringValue> strValId1 = FdoStringValue::Create(id1);
            propVal = FdoPropertyValue::Create (Data[0]->mPropertyName, strValId1);
            propValues->Add (propVal);
            FdoPtr<FdoInt32Value> intValId1 = FdoInt32Value::Create(int1);
            propVal = FdoPropertyValue::Create (Data[5]->mPropertyName, intValId1);
            propValues->Add (propVal);
            FdoPtr<FdoExpression> valueExpr1 = FdoExpression::Parse(geom1);
            FdoGeometryValue* geomVal1 = dynamic_cast<FdoGeometryValue*>(valueExpr1.p);
            propVal = FdoPropertyValue::Create (L"MyGeometry", geomVal1);
            propValues->Add (propVal);
            reader = insert->Execute();
            CPPUNIT_ASSERT_MESSAGE("Expected to receive one id from FdoIInsert::Execute(), got none.", reader->ReadNext());
            returnedId = reader->GetString(Data[0]->mPropertyName);
            CPPUNIT_ASSERT_MESSAGE("Wrong id returned", 0==wcscmp(returnedId, id1));
            CPPUNIT_ASSERT_MESSAGE("Expected to receive one id from FdoIInsert::Execute(), got at least two.", !reader->ReadNext());

            // insert one row:
            propValues = insert->GetPropertyValues();
            propVal = propValues->GetItem(Data[0]->mPropertyName);
            FdoPtr<FdoStringValue> strValId2 = FdoStringValue::Create(id2);
            propVal->SetValue(strValId2);
            propVal = propValues->GetItem(Data[5]->mPropertyName);
            FdoPtr<FdoInt32Value> intValId2 = FdoInt32Value::Create(int2);
            propVal->SetValue(intValId2);
            propVal = propValues->GetItem(L"MyGeometry");
            FdoPtr<FdoExpression> valueExpr2 = FdoExpression::Parse(geom2);
            FdoGeometryValue* geomVal2 = dynamic_cast<FdoGeometryValue*>(valueExpr2.p);
            propVal->SetValue(geomVal2);
            reader = insert->Execute();
            CPPUNIT_ASSERT_MESSAGE("Expected to receive one id from FdoIInsert::Execute(), got none.", reader->ReadNext());
            returnedId = reader->GetString(Data[0]->mPropertyName);
            CPPUNIT_ASSERT_MESSAGE("Wrong id returned", 0==wcscmp(returnedId, id2));
            CPPUNIT_ASSERT_MESSAGE("Expected to receive one id from FdoIInsert::Execute(), got at least two.", !reader->ReadNext());


            // insert a row with a duplicate ID value (EXPECT ERROR):
            propValues = insert->GetPropertyValues();
            propVal = propValues->GetItem(Data[0]->mPropertyName);
            FdoPtr<FdoStringValue> strValId3 = FdoStringValue::Create(id2);
            propVal->SetValue(strValId3);
            propVal = propValues->GetItem(Data[5]->mPropertyName);
            FdoPtr<FdoInt32Value> intValId3 = FdoInt32Value::Create(int2);
            propVal->SetValue(intValId3);
            propVal = propValues->GetItem(L"MyGeometry");
            FdoPtr<FdoExpression> valueExpr3 = FdoExpression::Parse(geom2);
            FdoGeometryValue* geomVal3 = dynamic_cast<FdoGeometryValue*>(valueExpr3.p);
            propVal->SetValue(geomVal3);
            try
            {
                reader = insert->Execute();
                CPPUNIT_FAIL("Expected to receive an exception FdoIInsert::Execute(), due to duplicate ids, got none.");
            }
            catch (FdoException *e)
            {
                // We expect an exception here, so dont log a failure
                e->Release();
            }


            // Select all rows of data:
            /////////////////////////////////////////////////////////////////////////

            FdoPtr<FdoISelect> select = (FdoISelect*)mConnection->CreateCommand(FdoCommandType_Select);
            select->SetFeatureClassName(ArcSDETestConfig::QClassNameTestUserManagedId());
            reader = select->Execute();
            CPPUNIT_ASSERT_MESSAGE("Expected to receive two ids from FdoISelect::Execute(), got none.", reader->ReadNext());
            FdoString* selectedId1 = reader->GetString(Data[0]->mPropertyName);
            CPPUNIT_ASSERT_MESSAGE("Wrong id from FdoISelect::Execute().", 0==wcscmp(selectedId1, id1));
            try
            {
                FdoInt32 failureId1 = reader->GetInt32(Data[0]->mPropertyName);
                CPPUNIT_ASSERT_MESSAGE("Expected exception when reading invalid property type, but didn't get one.", false);
            }
            catch (FdoException *e)
            {
                // we expect to get here
                e->Release();
            }
            FdoInt32 selectedInt1 = reader->GetInt32(Data[5]->mPropertyName);
            CPPUNIT_ASSERT_MESSAGE("Wrong int from FdoISelect::Execute().", selectedInt1==int1);

            CPPUNIT_ASSERT_MESSAGE("Expected to receive two ids from FdoISelect::Execute(), got one.",  reader->ReadNext());

            FdoString* selectedId2 = reader->GetString(Data[0]->mPropertyName);
            CPPUNIT_ASSERT_MESSAGE("Wrong id from FdoISelect::Execute().", 0==wcscmp(selectedId2, id2));
            try
            {
                FdoInt32 failureId1 = reader->GetInt32(Data[0]->mPropertyName);
                CPPUNIT_ASSERT_MESSAGE("Expected exception when reading invalid property type, but didn't get one.", false);
            }
            catch (FdoException *e)
            {
                // we expect to get here
                e->Release();
            }
            FdoInt32 selectedInt2 = reader->GetInt32(Data[5]->mPropertyName);
            CPPUNIT_ASSERT_MESSAGE("Wrong int from FdoISelect::Execute().", selectedInt2==int2);

            CPPUNIT_ASSERT_MESSAGE("Expected to receive two ids from FdoISelect::Execute(), got more.", !reader->ReadNext());

            // NOTE: to avoid defect 666545 "special sequence of feature commands causes 'catastrophic network failure'",
            // we run an additional plain select command here.  Strange, but it works.
            select = (FdoISelect*)mConnection->CreateCommand(FdoCommandType_Select);
            select->SetFeatureClassName(ArcSDETestConfig::QClassNameTestUserManagedId());
            reader = select->Execute();
            while (reader->ReadNext())
                ;  // do nothing


            // Select some rows of data, via spatial query:
            /////////////////////////////////////////////////////////////////////////

            select->SetFilter(L"MyGeometry intersects GEOMFROMTEXT ('POLYGON XY (( 95 95, 105 95, 105 105, 95 105, 95 95 ))')");
            reader = select->Execute();
            CPPUNIT_ASSERT_MESSAGE("Expected to receive one row from FdoISelect::Execute(), got none.", reader->ReadNext());
            returnedId = reader->GetString(Data[0]->mPropertyName);
            CPPUNIT_ASSERT_MESSAGE("Wrong id retrieved from spatial query", 0==wcscmp(returnedId, id2));
            CPPUNIT_ASSERT_MESSAGE("Expected to receive one row from FdoISelect::Execute(), got more.", !reader->ReadNext());


            // Select all rows of data, via SelectAggregates (with spatial query):
            /////////////////////////////////////////////////////////////////////////

            FdoPtr<FdoISelectAggregates> selectAggregates = (FdoISelectAggregates*)mConnection->CreateCommand(FdoCommandType_SelectAggregates);
            selectAggregates->SetFeatureClassName(ArcSDETestConfig::QClassNameTestUserManagedId());
            selectAggregates->SetDistinct(false);
            selectAggregates->SetFilter(L"MyGeometry intersects GEOMFROMTEXT ('POLYGON XY (( 0 0, 200 0, 200 200, 0 200, 0 0 ))')");

            // Add one computed identifier of an aggregate function:
            FdoPtr<FdoIdentifierCollection> selectIds = selectAggregates->GetPropertyNames();
            FdoPtr<FdoExpressionCollection> args = FdoExpressionCollection::Create();
            FdoPtr<FdoExpression> arg = FdoIdentifier::Create(Data[5]->mPropertyName);
            args->Add(arg);
            FdoPtr<FdoExpression> computedExpr = FdoFunction::Create(L"Sum", args);
            FdoPtr<FdoComputedIdentifier> computedId = FdoComputedIdentifier::Create(L"MyId", computedExpr);
            selectIds->Add(computedId);

            // Execute the select:
            FdoPtr<FdoIDataReader> dataReader = selectAggregates->Execute();
            CPPUNIT_ASSERT_MESSAGE("Expected to receive one row from FdoISelectAggregates::Execute(), got none.", dataReader->ReadNext());
            double dSum = dataReader->GetDouble(L"MyId");
            CPPUNIT_ASSERT_MESSAGE("Wrong sum of rows", dSum == (double)(int1 + int2));
            CPPUNIT_ASSERT_MESSAGE("Expected to receive one row from FdoISelectAggregates::Execute(), got two or more.", !dataReader->ReadNext());


            // Update some rows of data:
            /////////////////////////////////////////////////////////////////////////

            // Update a row's non-identity int column, via spatial filter:
            FdoInt32 updatedInt2 = 7642;
            FdoPtr<FdoIUpdate> update = (FdoIUpdate*)mConnection->CreateCommand(FdoCommandType_Update);
            update->SetFeatureClassName(ArcSDETestConfig::QClassNameTestUserManagedId());
            wchar_t buff[1000];
            update->SetFilter(L"MyGeometry intersects GEOMFROMTEXT ('POLYGON XY (( 95 95, 105 95, 105 105, 95 105, 95 95 ))')");
            propValues = update->GetPropertyValues();
            FdoPtr<FdoInt32Value> intVal1 = FdoInt32Value::Create(updatedInt2);
            propVal = FdoPropertyValue::Create (Data[5]->mPropertyName, intVal1);
            propValues->Add (propVal);
            update->Execute();

            // Select back the result to see if it changed properly:
            FdoCommonOSUtil::swprintf(buff, ELEMENTS(buff), L"%ls = '%ls'", Data[0]->mPropertyName, id2);
            select->SetFilter(buff);
            reader = select->Execute();
            CPPUNIT_ASSERT_MESSAGE("Expected to receive one row from FdoISelect::Execute(), got none.", reader->ReadNext());
            FdoInt32 readerInt32 = reader->GetInt32(Data[5]->mPropertyName);
            CPPUNIT_ASSERT_MESSAGE("Wrong updated int32 value", readerInt32==updatedInt2);
            CPPUNIT_ASSERT_MESSAGE("Expected to receive one row from FdoISelect::Execute(), got at least two.", !reader->ReadNext());


            // Update a row's user-managed identity string column:
            FdoString* updatedId1 = L"BobTwo";
            update = (FdoIUpdate*)mConnection->CreateCommand(FdoCommandType_Update);
            update->SetFeatureClassName(ArcSDETestConfig::QClassNameTestUserManagedId());
            FdoCommonOSUtil::swprintf(buff, ELEMENTS(buff), L"%ls = '%ls'", Data[0]->mPropertyName, id1);
            update->SetFilter(buff);
            propValues = update->GetPropertyValues();
            FdoPtr<FdoStringValue> strVal1 = FdoStringValue::Create(updatedId1);
            propVal = FdoPropertyValue::Create (Data[0]->mPropertyName, strVal1);
            propValues->Add (propVal);
            update->Execute();

            // Select back the result to see if it changed properly:
            FdoCommonOSUtil::swprintf(buff, ELEMENTS(buff), L"%ls = '%ls'", Data[0]->mPropertyName, updatedId1);
            select->SetFilter(buff);
            reader = select->Execute();
            CPPUNIT_ASSERT_MESSAGE("Expected to receive one row from FdoISelect::Execute(), got none.", reader->ReadNext());
            FdoString* readerStr = reader->GetString(Data[0]->mPropertyName);
            CPPUNIT_ASSERT_MESSAGE("Wrong updated int32 value", 0==wcscmp(readerStr, updatedId1));
            CPPUNIT_ASSERT_MESSAGE("Expected to receive one row from FdoISelect::Execute(), got at least two.", !reader->ReadNext());


            // Delete some rows of data:
            /////////////////////////////////////////////////////////////////////////

            // delete the first row:
            FdoPtr<FdoIDelete> deleteCmd = (FdoIDelete*)mConnection->CreateCommand(FdoCommandType_Delete);
            deleteCmd->SetFeatureClassName(ArcSDETestConfig::QClassNameTestUserManagedId());
            FdoCommonOSUtil::swprintf(buff, ELEMENTS(buff), L"%ls = '%ls'", Data[0]->mPropertyName, updatedId1);
            deleteCmd->SetFilter(buff);
            deleteCmd->Execute();

            // select all rows, make sure we only get the second row back:
            select->SetFeatureClassName(ArcSDETestConfig::QClassNameTestUserManagedId());
            select->SetFilter((FdoFilter*)NULL);
            reader = select->Execute();
            CPPUNIT_ASSERT_MESSAGE("Expected to receive one row from FdoISelect::Execute(), got none.", reader->ReadNext());
            selectedId2 = reader->GetString(Data[0]->mPropertyName);
            CPPUNIT_ASSERT_MESSAGE("Wrong id from FdoISelect::Execute().", 0==wcscmp(selectedId2, id2));
            CPPUNIT_ASSERT_MESSAGE("Expected to receive one row from FdoISelect::Execute(), got more.", !reader->ReadNext());

            // delete the second row, via spatial query:
            deleteCmd->SetFilter(L"MyGeometry intersects GEOMFROMTEXT ('POLYGON XY (( 95 95, 105 95, 105 105, 95 105, 95 95 ))')");
            deleteCmd->Execute();      

            // select all rows, make sure there are no rows left:
            select->SetFeatureClassName(ArcSDETestConfig::QClassNameTestUserManagedId());
            select->SetFilter((FdoFilter*)NULL);
            reader = select->Execute();
            CPPUNIT_ASSERT_MESSAGE("Expected to receive 0 rows from FdoISelect::Execute(), got one or more.", !reader->ReadNext());
        }
        catch (FdoException* ge)
        {
            fail (ge);
        }
    }
}



/* Test a class with all nullable properties in insert operation. */
void BasicInsertTests::nullable_insert ()
{
    if (CreateSchemaOnly())  return;

    try
    {
        mConnection = ArcSDETests::GetConnection ();
        mConnection->SetConnectionString (ArcSDETestConfig::ConnStringMetadcov());
        mConnection->Open ();

        // Clean up previous tests:
        CleanUpClass(mConnection, ArcSDETestConfig::ClassSchemaTestClassNullable(), ArcSDETestConfig::ClassNameTestClassNullable(), true);

        // Insert one row, setting no properties:
        FdoPtr<FdoIInsert> insert = (FdoIInsert*)mConnection->CreateCommand (FdoCommandType_Insert);
        insert->SetFeatureClassName (ArcSDETestConfig::QClassNameTestClassNullable());
        FdoPtr<FdoIFeatureReader> reader = insert->Execute ();
        // none returned: reader->Close ();

        // check by doing a select
        FdoPtr<FdoISelect> selectCmd = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
        selectCmd->SetFeatureClassName (ArcSDETestConfig::QClassNameTestClassNullable());
        reader = selectCmd->Execute ();
        CPPUNIT_ASSERT_MESSAGE("Failed to read 1st inserted row", reader->ReadNext());
        checkEqual(reader, L"String1", FdoDataType_String, L"NULL");
        CPPUNIT_ASSERT_MESSAGE("Shouldn't be anymore rows", !reader->ReadNext());
        reader->Close ();
    }
    catch (FdoException* ge) 
    {
        fail (ge);
    }
}
