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
#include "BasicUpdateTests.h"

CPPUNIT_TEST_SUITE_REGISTRATION (BasicUpdateTests);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION (BasicUpdateTests, "BasicUpdateTests");

// NOTE: The data/schema required for this test suite was imported from TestGeoDb.mdb (from the repository)
//       using ArcCatalog into the database pointed to by ArcSDETestConfig::ConnStringMetadcov();
//       IMPORTANT: you need to manually reset every FLOAT field in TestGeoDb.mdb to a (precision,scale) of (6,2)
//       before exporting it to enterprise goedatabase, otherwise FLOATs become DOUBLEs! It doesn't seem that
//       Personal Geodatabases can store the precision/scale properly. To do this, delete the fields called
//       MYFLOAT and MYFLOAT2 and then create new ones with those names specifying the precision and scale.

FdoStringP GetIdPropName() { return L"OBJECTID"; }
FdoStringP GetGeomPropName() { return L"SHAPE"; }



static UnitTestData _string (L"MYSTRING", L"String property.", FdoDataType_String, 512,  0, 0, true, false, false,
                             L"'hello world\n'", L"'second string'", L"'third string'", L"NULL", (wchar_t*)NULL);
static UnitTestData _single (L"MYFLOAT", L"Single property.", FdoDataType_Single,   0, 6, 2, true, false, false,
                             L"1234.56", L"432.343", L"-8953.233", L"NULL", (wchar_t*)NULL);
static UnitTestData _double (L"MYDOUBLE", L"Double property.", FdoDataType_Double,   0, 38, 8, true, false, false,
                             L"123456789e-7", L"-43200333.12322", L"7623761.43", L"NULL", (wchar_t*)NULL);
static UnitTestData _short (L"MYSHORTINT", L"Short property.", FdoDataType_Int16,   0, 0, 0, true, false, false,
                            L"1234", L"1234", L"-1762", L"NULL", (wchar_t*)NULL);
static UnitTestData _int (L"MYINT", L"Integer property.", FdoDataType_Int32,   0, 0, 0, true, false, false,
                          L"1234567", L"-7942912", L"782312", L"NULL", (wchar_t*)NULL);
static UnitTestData _date (L"MYDATE", L"Date property.", FdoDataType_DateTime,   0, 0, 0, true, false, false,
                           L"TIMESTAMP '2004-03-29 12:03:45.934'", L"TIMESTAMP '2004-07-19 12:28:42.123'", L"TIMESTAMP '2004-07-20 9:45:10.432'", L"NULL", (wchar_t*)NULL);
static UnitTestData _blob (L"MYBLOB", L"Blob property.", FdoDataType_BLOB,   0, 0, 0, true, false, false,
                           L"45,121,43,233,1,52,233,5,12,152,2,111,222", L"53,234,65,23,123,56,1,14,242,1", L"1,2,3,4,5,6,7", L"NULL", (wchar_t*)NULL);
//static UnitTestData _decimal (L"MyDecimal", L"Decimal property.", FdoDataType_Decimal,   0, 8, 3, true, false, false,
//                              L"-53231.231", L"3222.220", L"7893.12", L"NULL", (wchar_t*)NULL);
//TODO: when ApplySchema is fully supported, test FdoDataType_Boolean, FdoDataType_Byte, FdoDataType_Int64

static UnitTestData* Data[] =
{
    &_string,
    &_single,
    &_double,
    &_short,
    &_int,
    &_date,
    &_blob,
//    &_decimal
};

static int PropertyCount = (sizeof (Data) / sizeof (UnitTestData*));

FdoPtr<FdoIConnection> BasicUpdateTests::mConnection;

BasicUpdateTests::BasicUpdateTests(void)
{
}

BasicUpdateTests::~BasicUpdateTests(void)
{
}

void BasicUpdateTests::setUp ()
{
}

void BasicUpdateTests::tearDown ()
{
    if ((mConnection != NULL) && (mConnection->GetConnectionState() != FdoConnectionState_Closed))
        mConnection->Close ();
}

/* Test basic update operation. */
void BasicUpdateTests::simple_update ()
{
    if (CreateSchemaOnly())  return;

    try
    {
        mConnection = ArcSDETests::GetConnection ();
        mConnection->SetConnectionString (ArcSDETestConfig::ConnStringMetadcov());
        mConnection->Open ();

		// Clean up previous tests:
		CleanUpClass(mConnection, ArcSDETestConfig::ClassSchemaTestClassSimple(), ArcSDETestConfig::ClassNameTestClassSimple(), true);

        // insert a feature
        FdoPtr<FdoIInsert> insert = (FdoIInsert*)mConnection->CreateCommand (FdoCommandType_Insert);
        insert->SetFeatureClassName (ArcSDETestConfig::QClassNameTestClassSimple());
        FdoPtr<FdoPropertyValueCollection> values = insert->GetPropertyValues ();
        FdoPtr<FdoValueExpression> expression = (FdoValueExpression*)FdoExpression::Parse (Data[0]->mPropertyData[0]);
        FdoPtr<FdoPropertyValue> value = FdoPropertyValue::Create (Data[0]->mPropertyName, expression);
        values->Add (value);
        expression = (FdoValueExpression*)FdoExpression::Parse (L"GeomFromText('POLYGON XY ((5108.8 5104.7, 5109 5104, 5109 5105, 5108.8 5104.7))')");
        value = FdoPropertyValue::Create ((FdoString*)GetGeomPropName(), expression);
        values->Add (value);
        FdoPtr<FdoIFeatureReader> reader = insert->Execute ();

        int id;
        while (reader->ReadNext ())
            id = reader->GetInt32 ((FdoString*)GetIdPropName());
        reader->Close();

        // update it
        FdoPtr<FdoIUpdate> update = (FdoIUpdate*)mConnection->CreateCommand (FdoCommandType_Update);
        update->SetFeatureClassName (ArcSDETestConfig::QClassNameTestClassSimple());
	    FdoPtr<FdoPropertyValueCollection> propertyValues = update->GetPropertyValues();
        wchar_t filter[1024];
        FdoCommonOSUtil::swprintf(filter, ELEMENTS(filter), L"%ls = %d", (FdoString*)GetIdPropName(), id);
        update->SetFilter (FdoPtr<FdoFilter>(FdoFilter::Parse (filter)));
	    values = update->GetPropertyValues ();
	    value = FdoPropertyValue::Create ();
        value->SetName (Data[0]->mPropertyName);
        //FdoPtr<FdoStringValue> _string = static_cast<FdoStringValue*>(value->GetValue ());
        //_string->SetNull ();
		value->SetValue (L"'All mimsy were the borogoves'");
        values->Add (value);
        if (1 != update->Execute ())
            CPPUNIT_FAIL ("update execute failed");

        // check by doing a select
        FdoPtr<FdoISelect> select = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
        select->SetFeatureClassName (ArcSDETestConfig::QClassNameTestClassSimple());
        reader = select->Execute ();
        while (reader->ReadNext ())
        {
            CPPUNIT_ASSERT_MESSAGE ("incorrect value", 0 == wcscmp (L"All mimsy were the borogoves", reader->GetString (Data[0]->mPropertyName)));
            // check geometry was not affected
            FdoPtr<FdoGeometryValue> geometry = (FdoGeometryValue*)FDO_SAFE_ADDREF(expression.p);
            FdoPtr<FdoByteArray> fetched = reader->GetGeometry ((FdoString*)GetGeomPropName());
            FdoString* geometryText = geometry->ToString();
            FdoPtr<FdoGeometryValue> fetchedGeom = FdoGeometryValue::Create (fetched);
            FdoString* fetchedText = fetchedGeom->ToString ();
            CPPUNIT_ASSERT_MESSAGE ("incorrect geometry value", 0==wcscmp(geometryText, fetchedText));
        }
        reader->Close();

		// Clean up after test:
		CleanUpClass(mConnection, ArcSDETestConfig::ClassSchemaTestClassSimple(), ArcSDETestConfig::ClassNameTestClassSimple(), true);
    }
    catch (FdoException* ge) 
    {
        fail (ge);
    }
}

/* Test all data types update operation. */
void BasicUpdateTests::full_update ()
{
    int iRow = 0;
    FdoPtr<FdoPropertyValueCollection> values;
    FdoPtr<FdoISelect> selectCmd;
    FdoPtr<FdoIFeatureReader> reader;

    if (CreateSchemaOnly())  return;

    try
    {
        mConnection = ArcSDETests::GetConnection ();
        mConnection->SetConnectionString (ArcSDETestConfig::ConnStringMetadcovSingleDb());
        mConnection->Open ();

		// Clean up previous tests:
		CleanUpClass(mConnection, ArcSDETestConfig::ClassSchemaTestClassComplex(), ArcSDETestConfig::ClassNameTestClassComplex(), true);

        // Insert 2 rows of data:
        while (iRow<=1)
        {
            FdoPtr<FdoIInsert> insert = (FdoIInsert*)mConnection->CreateCommand (FdoCommandType_Insert);
            insert->SetFeatureClassName (ArcSDETestConfig::QClassNameTestClassComplex()); // misnomer, it's not a feature class
            values = insert->GetPropertyValues ();
            for (int i = 0; i < PropertyCount; i++)
            {
                FdoPtr<FdoPropertyValue> value = FdoPropertyValue::Create ();
                value->SetName (Data[i]->mPropertyName);
                FdoPtr<FdoValueExpression> expression = (FdoValueExpression*)ArcSDETests::ParseByDataType (Data[i]->mPropertyData[iRow], Data[i]->mPropertyType);
                value->SetValue (expression);
                values->Add (value);
            }
            reader = insert->Execute ();
            // none returned: reader->Close ();

            iRow++;
        }
        // check by doing a select
        selectCmd = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
        selectCmd->SetFeatureClassName (ArcSDETestConfig::QClassNameTestClassComplex());
        reader = selectCmd->Execute ();
        FdoInt32 iSelectedRow = 0;
        while (reader->ReadNext ())
        {
            // NOTE: we're assuming we receive the rows in the same order we inserted them
            for (int i = 0; i < PropertyCount; i++)
                checkEqual(reader, Data[i]->mPropertyName, Data[i]->mPropertyType, Data[i]->mPropertyData[iSelectedRow]);
            iSelectedRow++;
        }
        reader->Close ();


        // Update both rows, providing several values in the *reverse* order as class properties appear:
        FdoPtr<FdoIUpdate> update = (FdoIUpdate*)mConnection->CreateCommand (FdoCommandType_Update);
        update->SetFeatureClassName (ArcSDETestConfig::QClassNameTestClassComplex()); // misnomer, it's not a feature class
        wchar_t filter[1024];
        wcscpy (filter, Data[3]->mPropertyName);
        wcscat (filter, L" = ");
        wcscat (filter, Data[3]->mPropertyData[0]);
        update->SetFilter (FdoPtr<FdoFilter>(FdoFilter::Parse (filter)));
        values = update->GetPropertyValues ();
        for (int iPropertyIndex = 0; iPropertyIndex < PropertyCount; iPropertyIndex++)
        {
            int iDataIndex = (PropertyCount - 1) - iPropertyIndex;
            FdoPtr<FdoValueExpression> expression;
            expression = (FdoValueExpression*)ArcSDETests::ParseByDataType (Data[iDataIndex]->mPropertyData[iRow], Data[iDataIndex]->mPropertyType);
            FdoPtr<FdoPropertyValue> value = FdoPropertyValue::Create (Data[iDataIndex]->mPropertyName, expression);
            values->Add (value);
        }
        FdoInt32 iUpdatedRows = update->Execute ();
        //NOTE: iUpdatedRows is currently always 1, regardless of the # of rows actually affected.

        // check by doing a select
        selectCmd = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
        selectCmd->SetFeatureClassName (ArcSDETestConfig::QClassNameTestClassComplex());
        reader = selectCmd->Execute ();
        while (reader->ReadNext ())
            for (int i = 0; i < PropertyCount; i++)
                checkEqual(reader, Data[i]->mPropertyName, Data[i]->mPropertyType, Data[i]->mPropertyData[iRow]);
        reader->Close ();

        iRow++;



        // Update both rows, providing *mostly NULL value* in the *same* order as class properties appear:
        update = (FdoIUpdate*)mConnection->CreateCommand (FdoCommandType_Update);
        update->SetFeatureClassName (ArcSDETestConfig::QClassNameTestClassComplex()); // misnomer, it's not a feature class
        wcscpy (filter, Data[3]->mPropertyName);
        wcscat (filter, L" = ");
        wcscat (filter, Data[3]->mPropertyData[iRow-1]);
        update->SetFilter (FdoPtr<FdoFilter>(FdoFilter::Parse (filter)));
        values = update->GetPropertyValues ();
        for (int i = 0; i < PropertyCount; i++)
        {
            FdoPtr<FdoValueExpression> expression;
            expression = (FdoValueExpression*)ArcSDETests::ParseByDataType (Data[i]->mPropertyData[iRow], Data[i]->mPropertyType);
            FdoPtr<FdoPropertyValue> value = FdoPropertyValue::Create (Data[i]->mPropertyName, expression);
            value->SetValue (expression);
            values->Add (value);
        }
        iUpdatedRows = update->Execute ();
        //NOTE: iUpdatedRows is currently always 1, regardless of the # of rows actually affected.

        // check by doing a select
        selectCmd = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
        selectCmd->SetFeatureClassName (ArcSDETestConfig::QClassNameTestClassComplex());
        reader = selectCmd->Execute ();
        while (reader->ReadNext ())
            for (int i = 0; i < PropertyCount; i++)
                checkEqual(reader, Data[i]->mPropertyName, Data[i]->mPropertyType, Data[i]->mPropertyData[iRow]);
        reader->Close ();


		// Clean up after test:
		CleanUpClass(mConnection, ArcSDETestConfig::ClassSchemaTestClassComplex(), ArcSDETestConfig::ClassNameTestClassComplex(), true);
    }
    catch (FdoException* ge) 
    {
        fail (ge);
    }
}

/* Test geometry update operation. */
void BasicUpdateTests::geometry_update ()
{
    if (CreateSchemaOnly())  return;

    try
    {
        mConnection = ArcSDETests::GetConnection ();
        mConnection->SetConnectionString (ArcSDETestConfig::ConnStringMetadcovSingleDb());
        mConnection->Open ();

		// Clean up previous tests:
		CleanUpClass(mConnection, ArcSDETestConfig::ClassSchemaTestClassComplex(), ArcSDETestConfig::ClassNameTestClassComplex(), true);

        FdoPtr<FdoIInsert> insert = (FdoIInsert*)mConnection->CreateCommand (FdoCommandType_Insert);
        insert->SetFeatureClassName (ArcSDETestConfig::QClassNameTestClassComplex()); // misnomer, it's not a feature class
        FdoPtr<FdoPropertyValueCollection> values = insert->GetPropertyValues ();
        FdoPtr<FdoValueExpression> expression = (FdoValueExpression*)FdoExpression::Parse (Data[0]->mPropertyData[0]);
        FdoPtr<FdoPropertyValue> value = FdoPropertyValue::Create (Data[0]->mPropertyName, expression);
        values->Add (value);
        expression = (FdoValueExpression*)FdoExpression::Parse (L"GeomFromText('POLYGON XY ((5010.282 5011.717, 5011 5012, 5013 5012, 5010.282 5011.717))')");
        FdoPtr<FdoPropertyValue> geometry = FdoPropertyValue::Create ((FdoString*)GetGeomPropName(), expression);
        values->Add (geometry);
        FdoPtr<FdoIFeatureReader> reader = insert->Execute ();

        int id;
        while (reader->ReadNext ())
            id = reader->GetInt32 ((FdoString*)GetIdPropName());
        reader->Close();

        value = values->GetItem (Data[0]->mPropertyName);
        expression = (FdoValueExpression*)FdoExpression::Parse (L"'John Smith'");
        value->SetValue (expression);
        value = values->GetItem ((FdoString*)GetGeomPropName());
        expression = (FdoValueExpression*)FdoExpression::Parse (L"GeomFromText('POLYGON XY ((5000.919 5000.277, 5005 5000, 5005 5005, 5000.919 5000.277))')");
        value->SetValue (expression);
        reader = insert->Execute ();
        int id2;
        while (reader->ReadNext ())
            id2 = reader->GetInt32 ((FdoString*)GetIdPropName());
        reader->Close();
        value = values->GetItem (Data[0]->mPropertyName);
        expression = (FdoValueExpression*)FdoExpression::Parse (L"'Mott the Hoople'");
        value->SetValue (expression);
        value = values->GetItem ((FdoString*)GetGeomPropName());
        expression = (FdoValueExpression*)FdoExpression::Parse (L"GeomFromText('POLYGON XY ((5014.262 5000.018, 5015 5005, 5016 5010, 5014.262 5000.018))')");
        value->SetValue (expression);
        reader = insert->Execute ();
        int id3;
        while (reader->ReadNext ())
            id3 = reader->GetInt32 ((FdoString*)GetIdPropName());
        reader->Close();

        // update it
        FdoPtr<FdoIUpdate> update = (FdoIUpdate*)mConnection->CreateCommand (FdoCommandType_Update);
        update->SetFeatureClassName (ArcSDETestConfig::QClassNameTestClassComplex());
	    FdoPtr<FdoPropertyValueCollection> propertyValues = update->GetPropertyValues();
        wchar_t filter[1024];
        FdoCommonOSUtil::swprintf(filter, ELEMENTS(filter), L"%ls = %d", (FdoString*)GetIdPropName(), id);
		FdoPtr<FdoFilter> fdoFilter = FdoFilter::Parse (filter);
        update->SetFilter (fdoFilter);
	    values = update->GetPropertyValues ();
        expression = (FdoValueExpression*)FdoExpression::Parse (L"GeomFromText('POLYGON XY ((5008.8 5004.7, 5010 5010, 5000 5005, 5008.8 5004.7))')");
        value = FdoPropertyValue::Create ((FdoString*)GetGeomPropName(), expression);
        values->Add (value);

        if (1 != update->Execute ())
            CPPUNIT_FAIL ("update execute failed");

        // check by doing a select
        FdoPtr<FdoISelect> select = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
        select->SetFeatureClassName (ArcSDETestConfig::QClassNameTestClassComplex());
		fdoFilter = FdoFilter::Parse (filter);
        select->SetFilter (fdoFilter);
        reader = select->Execute ();
        while (reader->ReadNext ())
        {
            FdoPtr<FdoGeometryValue> geometry = FDO_SAFE_ADDREF((FdoGeometryValue*)expression.p);
            FdoPtr<FdoByteArray> fetched = reader->GetGeometry ((FdoString*)GetGeomPropName());
            FdoString* referenceText = geometry->ToString();
            FdoPtr<FdoGeometryValue> fetchedGeom = FdoGeometryValue::Create (fetched);
            const wchar_t* fetchedText = fetchedGeom->ToString ();
            CPPUNIT_ASSERT_MESSAGE ("incorrect geometry value", 0==wcscmp(referenceText, fetchedText));
        }
        reader->Close();

		// Clean up after test:
		CleanUpClass(mConnection, ArcSDETestConfig::ClassSchemaTestClassComplex(), ArcSDETestConfig::ClassNameTestClassComplex(), true);
    }
    catch (FdoException* ge) 
    {
        fail (ge);
    }
}

/* Test spatial filter. */
void BasicUpdateTests::spatial_filter ()
{
    if (CreateSchemaOnly())  return;

    try
    {
        mConnection = ArcSDETests::GetConnection ();
        mConnection->SetConnectionString (ArcSDETestConfig::ConnStringMetadcovSingleDb());
        mConnection->Open ();

		// Clean up previous tests:
		CleanUpClass(mConnection, ArcSDETestConfig::ClassSchemaTestClassComplex(), ArcSDETestConfig::ClassNameTestClassComplex(), true);

        FdoPtr<FdoIInsert> insert = (FdoIInsert*)mConnection->CreateCommand (FdoCommandType_Insert);
        insert->SetFeatureClassName (ArcSDETestConfig::QClassNameTestClassComplex());
        FdoPtr<FdoPropertyValueCollection> values = insert->GetPropertyValues ();
        FdoPtr<FdoValueExpression> expression = (FdoValueExpression*)FdoExpression::Parse (Data[0]->mPropertyData[0]);
        FdoPtr<FdoPropertyValue> value = FdoPropertyValue::Create (Data[0]->mPropertyName, expression);
        values->Add (value);
        expression = (FdoValueExpression*)FdoExpression::Parse (L"GeomFromText('POLYGON XY ((5010.282 5011.717, 5010.4 5011.7, 5010.4 5011.3, 5010.282 5011.717))')");
        value = FdoPropertyValue::Create ((FdoString*)GetGeomPropName(), expression);
        values->Add (value);
        FdoPtr<FdoIFeatureReader> reader = insert->Execute ();

        int id1;
        while (reader->ReadNext ())
            id1 = reader->GetInt32 ((FdoString*)GetIdPropName());
        reader->Close();

        value = values->GetItem (Data[0]->mPropertyName);
        expression = (FdoValueExpression*)FdoExpression::Parse (L"'John Smith'");
        value->SetValue (expression);
        value = values->GetItem ((FdoString*)GetGeomPropName());
        expression = (FdoValueExpression*)FdoExpression::Parse (L"GeomFromText('POLYGON XY ((5000.919 5000.277, 5000.5 5000.2, 5000.5 5000.7, 5000.919 5000.277))')");
        value->SetValue (expression);
        reader = insert->Execute ();
        int id2;
        while (reader->ReadNext ())
            id2 = reader->GetInt32 ((FdoString*)GetIdPropName());
        reader->Close();

        value = values->GetItem (Data[0]->mPropertyName);
        expression = (FdoValueExpression*)FdoExpression::Parse (L"'Mott the Hoople'");
        value->SetValue (expression);
        value = values->GetItem ((FdoString*)GetGeomPropName());
        expression = (FdoValueExpression*)FdoExpression::Parse (L"GeomFromText('POLYGON XY ((5014.262 5015.018, 5014.3 5015.9, 5014.9 5015.9, 5014.262 5015.018))')");
        value->SetValue (expression);
        reader = insert->Execute ();
        int id3;
        while (reader->ReadNext ())
            id3 = reader->GetInt32 ((FdoString*)GetIdPropName());
        reader->Close();

        // do a spatial filtered update
        FdoPtr<FdoIUpdate> update = (FdoIUpdate*)mConnection->CreateCommand (FdoCommandType_Update);
        update->SetFeatureClassName (ArcSDETestConfig::QClassNameTestClassComplex());
        update->SetFilter (FdoPtr<FdoFilter>(FdoFilter::Parse (L"SHAPE INTERSECTS GEOMFROMTEXT ('POLYGON XY (( 5012 5012, 5020 5012, 5020 5016, 5012 5016, 5012 5012 ))')")));
	    values = update->GetPropertyValues ();
        expression = (FdoValueExpression*)FdoExpression::Parse (L"'Alice in Wonderland'");
        value = FdoPropertyValue::Create (Data[0]->mPropertyName, expression);
        values->Add (value);
        if (1 != update->Execute ())
            CPPUNIT_FAIL ("update execute failed");

        // check by doing a select
        FdoPtr<FdoISelect> select = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
        select->SetFeatureClassName (ArcSDETestConfig::QClassNameTestClassComplex());
        reader = select->Execute ();
        int id;
        while (reader->ReadNext ())
        {
            id = reader->GetInt32 ((FdoString*)GetIdPropName());
            FdoString* item = reader->GetString (Data[0]->mPropertyName);
            if (id == id3)
                CPPUNIT_ASSERT_MESSAGE ("value not changed", 0 == wcscmp (item, L"Alice in Wonderland"));
            else
                CPPUNIT_ASSERT_MESSAGE ("value mistakenly changed", 0 != wcscmp (item, L"Alice in Wonderland"));
        }
        reader->Close();

		// Clean up after test:
		CleanUpClass(mConnection, ArcSDETestConfig::ClassSchemaTestClassComplex(), ArcSDETestConfig::ClassNameTestClassComplex(), true);
    }
    catch (FdoException* ge) 
    {
        fail (ge);
    }
}



/* Test insert/update/select (but not setting/getting any lock info) on a table that supports locking. */
void BasicUpdateTests::update_on_lock_enabled_table ()
{
    if (CreateSchemaOnly())  return;

    try
    {
        mConnection = ArcSDETests::GetConnection ();
        mConnection->SetConnectionString (ArcSDETestConfig::ConnStringMetadcov());
        mConnection->Open ();

		// Clean up previous tests:
        CleanUpClass(mConnection, ArcSDETestConfig::ClassSchemaSample(), ArcSDETestConfig::ClassNameSample(), true);
        
        FdoPtr<FdoIInsert> insert = (FdoIInsert*)mConnection->CreateCommand (FdoCommandType_Insert);
        insert->SetFeatureClassName (ArcSDETestConfig::QClassNameSample());
        FdoPtr<FdoPropertyValueCollection> values = insert->GetPropertyValues ();
        FdoPtr<FdoStringValue> expression = FdoStringValue::Create (L"Zozo");
        FdoPtr<FdoPropertyValue> value = FdoPropertyValue::Create (L"LockProperty", expression);
        values->Add (value);
        FdoPtr<FdoIFeatureReader> reader = insert->Execute ();
        FdoInt32 newId = 0;
        if (reader->ReadNext())
            newId = reader->GetInt32(L"Id");

        FdoPtr<FdoIUpdate> update = (FdoIUpdate*)mConnection->CreateCommand (FdoCommandType_Update);
        update->SetFeatureClassName (ArcSDETestConfig::QClassNameSample());
        wchar_t filter[1024];
        FdoCommonOSUtil::swprintf(filter, ELEMENTS(filter), L"Id = %d", newId);
        update->SetFilter (FdoPtr<FdoFilter>(FdoFilter::Parse (filter)));
	    values = update->GetPropertyValues ();
	    value = FdoPropertyValue::Create ();
        value->SetName (L"LockProperty");
		value->SetValue (L"'All mimsy were the borogoves'");
        values->Add (value);
        if (1 != update->Execute ())
            CPPUNIT_FAIL ("update execute failed");

        // check by doing a select
        FdoPtr<FdoISelect> select = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
        select->SetFeatureClassName (ArcSDETestConfig::QClassNameSample());
        reader = select->Execute ();
        while (reader->ReadNext ())
        {
            CPPUNIT_ASSERT_MESSAGE ("incorrect value", 0 == wcscmp (L"All mimsy were the borogoves", reader->GetString (L"LockProperty")));
        }
        reader->Close();

		// Clean up after test:
		CleanUpClass(mConnection, ArcSDETestConfig::ClassSchemaTestClassSimple(), ArcSDETestConfig::ClassNameSample(), true);
        mConnection->Close();
    }
    catch (FdoException* ge) 
    {
        fail (ge);
    }
}
