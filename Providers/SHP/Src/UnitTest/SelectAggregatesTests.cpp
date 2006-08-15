/*
 * 
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
#include "SelectAggregatesTests.h"

#include <FdoSpatial.h>
#include <FdoCommonFile.h>

#ifdef _WIN32
#define LOCATION L"..\\..\\TestData\\Testing"
#else
#define LOCATION L"../../TestData/Testing"
#endif

CPPUNIT_TEST_SUITE_REGISTRATION (SelectAggregatesTests);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION (SelectAggregatesTests, "SelectAggregatesTests");

FdoPtr<FdoIConnection> SelectAggregatesTests::mConnection;

SelectAggregatesTests::SelectAggregatesTests (void)
{
}

SelectAggregatesTests::~SelectAggregatesTests (void)
{
}

void SelectAggregatesTests::setUp ()
{
    if (!FdoCommonFile::FileExists (LOCATION))
        FdoCommonFile::MkDir (LOCATION);
    mConnection = ShpTests::GetConnection ();
    mConnection->SetConnectionString (L"DefaultFileLocation=" LOCATION);
    CPPUNIT_ASSERT_MESSAGE ("connection state not open", FdoConnectionState_Open == mConnection->Open ());
}

void SelectAggregatesTests::tearDown ()
{
    // Delete old class, if its there:
    CleanUpClass (mConnection, NULL, L"MyClass");

    mConnection->Close ();
	FDO_SAFE_RELEASE(mConnection.p);
	
    if (FdoCommonFile::FileExists (LOCATION L"schema.xml"))
        FdoCommonFile::Delete (LOCATION L"schema.xml");
    if (FdoCommonFile::FileExists (LOCATION))
        FdoCommonFile::RmDir (LOCATION);
}

void SelectAggregatesTests::select_distinct ()
{
    try
    {
        //////////////////////////////////////////////////////////////////////
        // Create a SHP file:
        //////////////////////////////////////////////////////////////////////

        FdoString *className  = L"MyClass";
        FdoString *schemaName = L"MySchema";

        create_schema(schemaName, className, FdoGeometricType_Curve, false, false, true);


        //////////////////////////////////////////////////////////////////////
        // Select some data via distinct:
        //////////////////////////////////////////////////////////////////////

        FdoPtr<FdoISelectAggregates> select = (FdoISelectAggregates*)mConnection->CreateCommand (FdoCommandType_SelectAggregates);
        select->SetFeatureClassName (className);
        select->SetDistinct(true);
        FdoPtr<FdoIdentifierCollection> selectedIds = select->GetPropertyNames();
        selectedIds->Clear();
        selectedIds->Add(FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"Street")));
        FdoPtr<FdoIDataReader> datareader = select->Execute ();
        long count = 0;
        while (datareader->ReadNext ())
        {
            FdoString* streetname = datareader->GetString(L"Street");
            count++;
        }
        CPPUNIT_ASSERT_MESSAGE("Wrong distinct street count", count==3);
        datareader->Close();
        datareader = NULL;

        selectedIds->Clear();
        selectedIds->Add(FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"Date")));
        datareader = select->Execute ();
        count = 0;
        while (datareader->ReadNext ())
        {
            FdoDateTime aDate = datareader->GetDateTime(L"Date");
            count++;
        }
        CPPUNIT_ASSERT_MESSAGE("Wrong distinct date count", count==2);
        datareader->Close();
        datareader = NULL;
    }
    catch (FdoException* e)
    {
        fail(e);
    }
}



void SelectAggregatesTests::select_aggregates ()
{
    try
    {
        //////////////////////////////////////////////////////////////////////
        // Create a SHP file:
        //////////////////////////////////////////////////////////////////////

        FdoString *className  = L"MyClass";
        FdoString *schemaName = L"MySchema";

        create_schema(schemaName, className, FdoGeometricType_Curve, false, false, true);


        //////////////////////////////////////////////////////////////////////
        // Select some data via aggregate sum:
        //////////////////////////////////////////////////////////////////////

        FdoPtr<FdoISelectAggregates> select = (FdoISelectAggregates*)mConnection->CreateCommand (FdoCommandType_SelectAggregates);
        select->SetFeatureClassName (className);
        select->SetDistinct(false);
        FdoPtr<FdoIdentifierCollection> selectedIds = select->GetPropertyNames();
        selectedIds->Clear();
        selectedIds->Add(FdoPtr<FdoComputedIdentifier>(FdoComputedIdentifier::Create(L"HeadTally", FdoPtr<FdoExpression>(FdoExpression::Parse(L"5+Sum(HeadCount+1)-Min(HeadCount-1)")))));
        FdoPtr<FdoIDataReader> datareader = select->Execute ();
        long count = 0;
        FdoInt32 headcounttally = 0;
        while (datareader->ReadNext ())
        {
            headcounttally = (FdoInt32)datareader->GetDouble(L"HeadTally");
            count++;
        }
        CPPUNIT_ASSERT_MESSAGE("Wrong sum rowcount", count==1);
        CPPUNIT_ASSERT_MESSAGE("Wrong headcount sum", headcounttally==26);
        datareader->Close();
        datareader = NULL;
    }
    catch (FdoException* e)
    {
        fail(e);
    }
}



void SelectAggregatesTests::select_aggregates_null ()
{
    try
    {
        //////////////////////////////////////////////////////////////////////
        // Create a SHP file with 0 rows:
        //////////////////////////////////////////////////////////////////////

        FdoString *className  = L"MyClass";
        FdoString *schemaName = L"MySchema";

        create_schema(schemaName, className, FdoGeometricType_Curve, false, false, true);
        CleanUpClass(mConnection, schemaName, className, true);


        //////////////////////////////////////////////////////////////////////
        // Try select aggregates on 0 rows
        //////////////////////////////////////////////////////////////////////

        FdoPtr<FdoISelectAggregates> select = (FdoISelectAggregates*)mConnection->CreateCommand (FdoCommandType_SelectAggregates);
        select->SetFeatureClassName (className);
        select->SetDistinct(false);
        FdoPtr<FdoIdentifierCollection> selectedIds = select->GetPropertyNames();
        selectedIds->Clear();
        selectedIds->Add(FdoPtr<FdoComputedIdentifier>(FdoComputedIdentifier::Create(L"HeadTally", FdoPtr<FdoExpression>(FdoExpression::Parse(L"5+Sum(HeadCount+1)-Min(HeadCount-1)")))));
        FdoPtr<FdoIDataReader> datareader = select->Execute ();
        CPPUNIT_ASSERT_MESSAGE("Expected 1 aggregate result", datareader->ReadNext ());
        CPPUNIT_ASSERT_MESSAGE("Expected NULL result for aggregate on 0 rows", datareader->IsNull(L"HeadTally"));
        CPPUNIT_ASSERT_MESSAGE("Expected 1 aggregate result", !datareader->ReadNext ());
        datareader->Close();
        datareader = NULL;


        //////////////////////////////////////////////////////////////////////
        // Insert some test data (with NULL values):
        //////////////////////////////////////////////////////////////////////

        FdoPtr<FdoIInsert> insert = (FdoIInsert*)mConnection->CreateCommand(FdoCommandType_Insert);
        insert->SetFeatureClassName(className);
        FdoPtr<FdoPropertyValueCollection> propVals = insert->GetPropertyValues();
        FdoPtr<FdoPropertyValue> propVal;
        FdoPtr<FdoIFeatureReader> reader;
        FdoPtr<FdoExpression> expr;

        propVals->Clear();
        expr = FdoExpression::Parse(L"'Slater'");
        propVal = FdoPropertyValue::Create(L"Street", (FdoValueExpression*)expr.p);
        propVals->Add(propVal);
        expr = FdoDateTimeValue::Create();  // NULL
        propVal = FdoPropertyValue::Create(L"Date", (FdoValueExpression*)expr.p);
        propVals->Add(propVal);
        expr = FdoExpression::Parse(L"GEOMFROMTEXT('LINESTRING XY (1.0 1.0, 2.0 2.0, 1.0 3.0)')");
        propVal = FdoPropertyValue::Create(L"Geometry", (FdoValueExpression*)expr.p);
        propVals->Add(propVal);
        expr = ShpTests::ParseByDataType(L"5", FdoDataType_Decimal);
        propVal = FdoPropertyValue::Create(L"HeadCount", (FdoValueExpression*)expr.p);
        propVals->Add(propVal);
        expr = ShpTests::ParseByDataType(L"7500.1", FdoDataType_Decimal);
        propVal = FdoPropertyValue::Create(L"LotSize", (FdoValueExpression*)expr.p);
        propVals->Add(propVal);
        reader = insert->Execute();

        propVals->Clear();
        expr = FdoExpression::Parse(L"'Slater'");
        propVal = FdoPropertyValue::Create(L"Street", (FdoValueExpression*)expr.p);
        propVals->Add(propVal);
        expr = FdoExpression::Parse(L"DATE '2003-05-21'");
        propVal = FdoPropertyValue::Create(L"Date", (FdoValueExpression*)expr.p);
        propVals->Add(propVal);
        expr = FdoExpression::Parse(L"GEOMFROMTEXT('LINESTRING XY (10.0 10.0, 20.0 20.0, 10.0 30.0)')");
        propVal = FdoPropertyValue::Create(L"Geometry", (FdoValueExpression*)expr.p);
        propVals->Add(propVal);
        expr = FdoDecimalValue::Create();  //NULL
        propVal = FdoPropertyValue::Create(L"HeadCount", (FdoValueExpression*)expr.p);
        propVals->Add(propVal);
        expr = ShpTests::ParseByDataType(L"5500.5", FdoDataType_Decimal);
        propVal = FdoPropertyValue::Create(L"LotSize", (FdoValueExpression*)expr.p);
        propVals->Add(propVal);
        reader = insert->Execute();

        propVals->Clear();
        expr = FdoExpression::Parse(L"'Albert'");
        propVal = FdoPropertyValue::Create(L"Street", (FdoValueExpression*)expr.p);
        propVals->Add(propVal);
        expr = FdoExpression::Parse(L"DATE '2003-05-21'");
        propVal = FdoPropertyValue::Create(L"Date", (FdoValueExpression*)expr.p);
        propVals->Add(propVal);
        expr = FdoExpression::Parse(L"GEOMFROMTEXT('LINESTRING XY (3.0 3.0, 4.0 4.0, 5.0 5.0)')");
        propVal = FdoPropertyValue::Create(L"Geometry", (FdoValueExpression*)expr.p);
        propVals->Add(propVal);
        expr = ShpTests::ParseByDataType(L"4", FdoDataType_Decimal);
        propVal = FdoPropertyValue::Create(L"HeadCount", (FdoValueExpression*)expr.p);
        propVals->Add(propVal);
        expr = ShpTests::ParseByDataType(L"10000.0", FdoDataType_Decimal);
        propVal = FdoPropertyValue::Create(L"LotSize", (FdoValueExpression*)expr.p);
        propVals->Add(propVal);
        reader = insert->Execute();

        propVals->Clear();
        expr = FdoExpression::Parse(L"'Queen'");
        propVal = FdoPropertyValue::Create(L"Street", (FdoValueExpression*)expr.p);
        propVals->Add(propVal);
        expr = FdoDateTimeValue::Create();  // NULL
        propVal = FdoPropertyValue::Create(L"Date", (FdoValueExpression*)expr.p);
        propVals->Add(propVal);
        expr = FdoExpression::Parse(L"GEOMFROMTEXT('LINESTRING XY (1.0 1.0, -2.0 -2.0, -1.0 -3.0)')");
        propVal = FdoPropertyValue::Create(L"Geometry", (FdoValueExpression*)expr.p);
        propVals->Add(propVal);
        expr = ShpTests::ParseByDataType(L"7", FdoDataType_Decimal);
        propVal = FdoPropertyValue::Create(L"HeadCount", (FdoValueExpression*)expr.p);
        propVals->Add(propVal);
        expr = FdoDecimalValue::Create();  //NULL
        propVal = FdoPropertyValue::Create(L"LotSize", (FdoValueExpression*)expr.p);
        propVals->Add(propVal);
        reader = insert->Execute();


        //////////////////////////////////////////////////////////////////////
        // Try select aggregates on rows that contain NULL values
        //////////////////////////////////////////////////////////////////////

        select = (FdoISelectAggregates*)mConnection->CreateCommand (FdoCommandType_SelectAggregates);
        select->SetFeatureClassName (className);
        select->SetDistinct(false);
        selectedIds = select->GetPropertyNames();
        selectedIds->Clear();
        selectedIds->Add(FdoPtr<FdoComputedIdentifier>(FdoComputedIdentifier::Create(L"HeadTally", FdoPtr<FdoExpression>(FdoExpression::Parse(L"5+Sum(HeadCount+1)-Min(LotSize-1)")))));
        datareader = select->Execute ();
        CPPUNIT_ASSERT_MESSAGE("Expected 1 aggregate result", datareader->ReadNext ());
        CPPUNIT_ASSERT_MESSAGE("Didn't get expected aggregate result", datareader->GetDouble(L"HeadTally")==-5475.5); // 5+19-5499.5 == -5475.5
        CPPUNIT_ASSERT_MESSAGE("Expected 1 aggregate result", !datareader->ReadNext ());
        datareader->Close();
        datareader = NULL;
    }
    catch (FdoException* e)
    {
        fail(e);
    }
}


void SelectAggregatesTests::create_schema (FdoString *schemaName, FdoString* className, FdoGeometricType type, bool elevation, bool measure, bool bInsertTestData)
{
    // Clean up leftover class, if any:
    CleanUpClass(mConnection, NULL, className);

    //////////////////////////////////////////////////////////////////////
    // Create the schema:
    //////////////////////////////////////////////////////////////////////

    // create the class
    FdoPtr<FdoFeatureSchema> schema = FdoFeatureSchema::Create (schemaName, L" test schema");
    FdoPtr<FdoClassCollection> classes = schema->GetClasses ();

    FdoPtr<FdoDataPropertyDefinition> featid = FdoDataPropertyDefinition::Create (L"FeatId", L"integer");
    featid->SetDataType (FdoDataType_Int32);
    featid->SetIsAutoGenerated (true);
    featid->SetNullable (false);

    FdoPtr<FdoDataPropertyDefinition> street = FdoDataPropertyDefinition::Create (L"Street", L"text");
    street->SetDataType (FdoDataType_String);
    street->SetLength (64);

    FdoPtr<FdoDataPropertyDefinition> lotsize = FdoDataPropertyDefinition::Create (L"LotSize", L"size");
    lotsize->SetDataType (FdoDataType_Decimal);
    lotsize->SetPrecision (20);
    lotsize->SetScale (8);

    FdoPtr<FdoDataPropertyDefinition> headcount = FdoDataPropertyDefinition::Create (L"HeadCount", L"size");
    headcount->SetDataType (FdoDataType_Decimal);
    headcount->SetPrecision(10);
    headcount->SetScale(0);

    FdoPtr<FdoDataPropertyDefinition> date = FdoDataPropertyDefinition::Create (L"Date", L"date");
    date->SetDataType (FdoDataType_DateTime);

    // build a location geometry property
    FdoPtr<FdoGeometricPropertyDefinition> location = FdoGeometricPropertyDefinition::Create (L"Geometry", L"geometry");
    location->SetGeometryTypes (type);
    location->SetHasElevation (elevation);
    location->SetHasMeasure (measure);

    //// assemble the feature class
    FdoPtr<FdoFeatureClass> feature = FdoFeatureClass::Create (className, L"test class created with apply schema");
    FdoPtr<FdoPropertyDefinitionCollection> properties = feature->GetProperties ();
    FdoPtr<FdoDataPropertyDefinitionCollection> identities = feature->GetIdentityProperties ();
    properties->Add (featid);
    identities->Add (featid);
    properties->Add (street);
    properties->Add (date);
    properties->Add (lotsize);
    properties->Add (headcount);
    properties->Add (location);
    feature->SetGeometryProperty (location);

    // submit the new schema
    classes->Add (feature);
    FdoPtr<FdoIApplySchema> apply = (FdoIApplySchema*)mConnection->CreateCommand (FdoCommandType_ApplySchema);
    apply->SetFeatureSchema (schema);
    apply->Execute ();


    //////////////////////////////////////////////////////////////////////
    // Insert some test data (with redundant values):
    //////////////////////////////////////////////////////////////////////

    if (bInsertTestData)
    {
        FdoPtr<FdoIInsert> insert = (FdoIInsert*)mConnection->CreateCommand(FdoCommandType_Insert);
        insert->SetFeatureClassName(className);
        FdoPtr<FdoPropertyValueCollection> propVals = insert->GetPropertyValues();
        FdoPtr<FdoPropertyValue> propVal;
        FdoPtr<FdoIFeatureReader> reader;
        FdoPtr<FdoExpression> expr;

        propVals->Clear();
        expr = FdoExpression::Parse(L"'Slater'");
        propVal = FdoPropertyValue::Create(L"Street", (FdoValueExpression*)expr.p);
        propVals->Add(propVal);
        expr = FdoExpression::Parse(L"DATE '1999-12-31'");
        propVal = FdoPropertyValue::Create(L"Date", (FdoValueExpression*)expr.p);
        propVals->Add(propVal);
        expr = FdoExpression::Parse(L"GEOMFROMTEXT('LINESTRING XY (1.0 1.0, 2.0 2.0, 1.0 3.0)')");
        propVal = FdoPropertyValue::Create(L"Geometry", (FdoValueExpression*)expr.p);
        propVals->Add(propVal);
        expr = ShpTests::ParseByDataType(L"5", FdoDataType_Decimal);
        propVal = FdoPropertyValue::Create(L"HeadCount", (FdoValueExpression*)expr.p);
        propVals->Add(propVal);
        expr = ShpTests::ParseByDataType(L"7500.1", FdoDataType_Decimal);
        propVal = FdoPropertyValue::Create(L"LotSize", (FdoValueExpression*)expr.p);
        propVals->Add(propVal);
        reader = insert->Execute();

        propVals->Clear();
        expr = FdoExpression::Parse(L"'Slater'");
        propVal = FdoPropertyValue::Create(L"Street", (FdoValueExpression*)expr.p);
        propVals->Add(propVal);
        expr = FdoExpression::Parse(L"DATE '2003-05-21'");
        propVal = FdoPropertyValue::Create(L"Date", (FdoValueExpression*)expr.p);
        propVals->Add(propVal);
        expr = FdoExpression::Parse(L"GEOMFROMTEXT('LINESTRING XY (10.0 10.0, 20.0 20.0, 10.0 30.0)')");
        propVal = FdoPropertyValue::Create(L"Geometry", (FdoValueExpression*)expr.p);
        propVals->Add(propVal);
        expr = ShpTests::ParseByDataType(L"3", FdoDataType_Decimal);
        propVal = FdoPropertyValue::Create(L"HeadCount", (FdoValueExpression*)expr.p);
        propVals->Add(propVal);
        expr = ShpTests::ParseByDataType(L"5500.5", FdoDataType_Decimal);
        propVal = FdoPropertyValue::Create(L"LotSize", (FdoValueExpression*)expr.p);
        propVals->Add(propVal);
        reader = insert->Execute();

        propVals->Clear();
        expr = FdoExpression::Parse(L"'Albert'");
        propVal = FdoPropertyValue::Create(L"Street", (FdoValueExpression*)expr.p);
        propVals->Add(propVal);
        expr = FdoExpression::Parse(L"DATE '2003-05-21'");
        propVal = FdoPropertyValue::Create(L"Date", (FdoValueExpression*)expr.p);
        propVals->Add(propVal);
        expr = FdoExpression::Parse(L"GEOMFROMTEXT('LINESTRING XY (3.0 3.0, 4.0 4.0, 5.0 5.0)')");
        propVal = FdoPropertyValue::Create(L"Geometry", (FdoValueExpression*)expr.p);
        propVals->Add(propVal);
        expr = ShpTests::ParseByDataType(L"4", FdoDataType_Decimal);
        propVal = FdoPropertyValue::Create(L"HeadCount", (FdoValueExpression*)expr.p);
        propVals->Add(propVal);
        expr = ShpTests::ParseByDataType(L"10000.0", FdoDataType_Decimal);
        propVal = FdoPropertyValue::Create(L"LotSize", (FdoValueExpression*)expr.p);
        propVals->Add(propVal);
        reader = insert->Execute();

        propVals->Clear();
        expr = FdoExpression::Parse(L"'Queen'");
        propVal = FdoPropertyValue::Create(L"Street", (FdoValueExpression*)expr.p);
        propVals->Add(propVal);
        expr = FdoExpression::Parse(L"DATE '2003-05-21'");
        propVal = FdoPropertyValue::Create(L"Date", (FdoValueExpression*)expr.p);
        propVals->Add(propVal);
        expr = FdoExpression::Parse(L"GEOMFROMTEXT('LINESTRING XY (1.0 1.0, -2.0 -2.0, -1.0 -3.0)')");
        propVal = FdoPropertyValue::Create(L"Geometry", (FdoValueExpression*)expr.p);
        propVals->Add(propVal);
        expr = ShpTests::ParseByDataType(L"7", FdoDataType_Decimal);
        propVal = FdoPropertyValue::Create(L"HeadCount", (FdoValueExpression*)expr.p);
        propVals->Add(propVal);
        expr = ShpTests::ParseByDataType(L"2702.7", FdoDataType_Decimal);
        propVal = FdoPropertyValue::Create(L"LotSize", (FdoValueExpression*)expr.p);
        propVals->Add(propVal);
        reader = insert->Execute();
    }
}




void SelectAggregatesTests::select_orderby ()
{
    try
    {
        //////////////////////////////////////////////////////////////////////
        // Create a SHP file:
        //////////////////////////////////////////////////////////////////////

        FdoString *className  = L"MyClass";
        FdoString *schemaName = L"MySchema";

        create_schema(schemaName, className, FdoGeometricType_Curve, false, false, true);


        //////////////////////////////////////////////////////////////////////
        // Select some data via orderby:
        //////////////////////////////////////////////////////////////////////

        FdoPtr<FdoISelectAggregates> select = (FdoISelectAggregates*)mConnection->CreateCommand (FdoCommandType_SelectAggregates);
        select->SetFeatureClassName (className);
        select->SetDistinct(false);
        select->SetOrderingOption(FdoOrderingOption_Descending);
        FdoPtr<FdoIdentifierCollection> orderingIds = select->GetOrdering();
        orderingIds->Add( FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"Street")) );
        FdoPtr<FdoIdentifierCollection> selectedIds = select->GetPropertyNames();
        selectedIds->Clear();
        selectedIds->Add( FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"Street")) );
        FdoPtr<FdoIDataReader> datareader = select->Execute ();
        long count = 0;
        FdoStringP street;
        FdoStringP previousStreet;
        while (datareader->ReadNext ())
        {
            previousStreet = street;
            street = datareader->GetString(L"Street");
            if (previousStreet != NULL)
                CPPUNIT_ASSERT(0>=wcscmp(street, previousStreet));
            count++;
        }
        CPPUNIT_ASSERT_MESSAGE("Wrong orderby rowcount", count==4);
        datareader->Close();
        datareader = NULL;
    }
    catch (FdoException* e)
    {
        fail(e);
    }
}

void SelectAggregatesTests::count ()
{
    try
    {
        //////////////////////////////////////////////////////////////////////
        // Create a SHP file:
        //////////////////////////////////////////////////////////////////////

        FdoString *className  = L"MyClass";
        FdoString *schemaName = L"MySchema";

        create_schema(schemaName, className, FdoGeometricType_Curve, false, false, true);


        //////////////////////////////////////////////////////////////////////
        // Try function count():
        //////////////////////////////////////////////////////////////////////

        FdoPtr<FdoISelectAggregates> select = (FdoISelectAggregates*)mConnection->CreateCommand (FdoCommandType_SelectAggregates);
        select->SetFeatureClassName (className);
        select->SetDistinct(false);
        FdoPtr<FdoIdentifierCollection> selectedIds = select->GetPropertyNames();
        selectedIds->Clear();
        selectedIds->Add(FdoPtr<FdoComputedIdentifier>(FdoComputedIdentifier::Create(L"Total", FdoPtr<FdoExpression>(FdoExpression::Parse(L"Count(\"Date\")")))));
        FdoPtr<FdoIDataReader> datareader = select->Execute ();
        long count = 0;
        while (datareader->ReadNext ())
        {
            FdoInt64 total = datareader->GetInt64(L"Total");
            CPPUNIT_ASSERT_MESSAGE("Count wrong", 4L == total);
            count++;
        }
        CPPUNIT_ASSERT_MESSAGE("Wrong count(*) rowcount", count==1);
        datareader->Close();
        datareader = NULL;
    }
    catch (FdoException* e)
    {
        fail(e);
    }
}

void SelectAggregatesTests::ceil_floor ()
{
    try
    {
        //////////////////////////////////////////////////////////////////////
        // Create a SHP file:
        //////////////////////////////////////////////////////////////////////

        FdoString *className  = L"MyClass";
        FdoString *schemaName = L"MySchema";

        create_schema(schemaName, className, FdoGeometricType_Curve, false, false, true);


        //////////////////////////////////////////////////////////////////////
        // Try numerical function ceil():
        //////////////////////////////////////////////////////////////////////

        FdoPtr<FdoISelect> select = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);

        select->SetFeatureClassName (className);
        FdoPtr<FdoIdentifierCollection> selectedIds = select->GetPropertyNames();
        selectedIds->Clear();

        FdoPtr<FdoComputedIdentifier> cid = (FdoComputedIdentifier*)FdoExpression::Parse(L"(Ceil(LotSize)) AS TestCeil");
		selectedIds->Add(cid);

		FdoPtr<FdoIReader> datareader = select->Execute ();

		// [2702.7, 10000]
        long count = 0;
        while (datareader->ReadNext ())
        {
            double result = datareader->GetDouble(L"TestCeil");
            CPPUNIT_ASSERT_MESSAGE("Ceil wrong", result >= 2703 && result <= 10000);
            count++;
        }
        CPPUNIT_ASSERT_MESSAGE("Wrong count(*) rowcount", count==4);
        datareader->Close();
        datareader = NULL;

        //////////////////////////////////////////////////////////////////////
        // Try numerical function floor():
        //////////////////////////////////////////////////////////////////////
        selectedIds = select->GetPropertyNames();
        selectedIds->Clear();

        cid = (FdoComputedIdentifier*)FdoExpression::Parse(L"(Floor(LotSize)) AS TestFloor");
		selectedIds->Add(cid);

		datareader = select->Execute ();

		// [2702.7, 10000]
        count = 0;
        while (datareader->ReadNext ())
        {
            double result = datareader->GetDouble(L"TestFloor");
            CPPUNIT_ASSERT_MESSAGE("Floor wrong",  result >= 2702 && result <= 10000);
            count++;
        }
        CPPUNIT_ASSERT_MESSAGE("Wrong count(*) rowcount", count==4);
        datareader->Close();
        datareader = NULL;

        //////////////////////////////////////////////////////////////////////
        // Try numerical function floor() with Int32:
        //////////////////////////////////////////////////////////////////////
        selectedIds->Clear();

        cid = (FdoComputedIdentifier*)FdoExpression::Parse(L"(Floor(FeatId)) AS TestFloor");
		selectedIds->Add(cid);

		datareader = select->Execute ();

        count = 0;
        while (datareader->ReadNext ())
        {
            double result = datareader->GetDouble(L"TestFloor");
            CPPUNIT_ASSERT_MESSAGE("Floor(FeatId) wrong",  result >= 1 && result <= 4);
            count++;
        }
        CPPUNIT_ASSERT_MESSAGE("Wrong count(*) rowcount", count==4);
        datareader->Close();
        datareader = NULL;

        //////////////////////////////////////////////////////////////////////
        // Try some wrong datatypes:
        //////////////////////////////////////////////////////////////////////

		bool error = false;
		try
		{
			selectedIds->Clear();

			FdoPtr<FdoComputedIdentifier> cid = (FdoComputedIdentifier*)FdoExpression::Parse(L"(Ceil(Street)) AS TestCeil");
			selectedIds->Add(cid);

			datareader = select->Execute ();
			while (datareader->ReadNext ())
			{
				double result = datareader->GetDouble(L"TestCeil");
			}
		}
		catch (FdoException* e)
		{
			//printf("Expected: %ls\n", e->GetExceptionMessage());
			e->Release();
			error = true;
		}
		datareader->Close();
		datareader = NULL;

		CPPUNIT_ASSERT_MESSAGE("Ceil succeeded with wrong datatype", error == true );
    }
    catch (FdoException* e)
    {
        fail(e);
    }
}

void SelectAggregatesTests::upper_lower ()
{
    try
    {
        //////////////////////////////////////////////////////////////////////
        // Create a SHP file:
        //////////////////////////////////////////////////////////////////////

        FdoString *className  = L"MyClass";
        FdoString *schemaName = L"MySchema";

        create_schema(schemaName, className, FdoGeometricType_Curve, false, false, true);

        //////////////////////////////////////////////////////////////////////
        // Try numerical function upper():
        //////////////////////////////////////////////////////////////////////

        FdoPtr<FdoISelect> select = (FdoISelect*)mConnection->CreateCommand (FdoCommandType_Select);
        select->SetFeatureClassName (className);
        FdoPtr<FdoIdentifierCollection> selectedIds = select->GetPropertyNames();
        selectedIds->Clear();

        FdoPtr<FdoComputedIdentifier> cid = (FdoComputedIdentifier*)FdoExpression::Parse(L"(Upper(Street)) AS TestUpper");
		selectedIds->Add(cid);

		FdoPtr<FdoIReader>  datareader = select->Execute ();

        long count = 0;
        while (datareader->ReadNext ())
        {
            FdoString *street = datareader->GetString(L"TestUpper");
            CPPUNIT_ASSERT_MESSAGE("Upper wrong",          
				(0==wcscmp(street, L"SLATER")) || (0==wcscmp(street, L"ALBERT")) || (0==wcscmp(street, L"QUEEN")) );

            count++;
        }
        CPPUNIT_ASSERT_MESSAGE("Wrong count(*) rowcount", count==4);
        datareader->Close();
        datareader = NULL;


        //////////////////////////////////////////////////////////////////////
        // Try numerical function lower():
        //////////////////////////////////////////////////////////////////////
        selectedIds = select->GetPropertyNames();
        selectedIds->Clear();

        cid = (FdoComputedIdentifier*)FdoExpression::Parse(L"(Lower(Street)) AS TestLower");

		selectedIds->Add(cid);

		datareader = select->Execute ();

        count = 0;
        while (datareader->ReadNext ())
        {
            FdoString *street = datareader->GetString(L"TestLower");
            CPPUNIT_ASSERT_MESSAGE("Lower wrong",          
				(0==wcscmp(street, L"slater")) || (0==wcscmp(street, L"albert")) || (0==wcscmp(street, L"queen")) );

            count++;
        }
        CPPUNIT_ASSERT_MESSAGE("Wrong count(*) rowcount", count==4);
        datareader->Close();
        datareader = NULL;

        //////////////////////////////////////////////////////////////////////
        // Try some wrong datatypes:
        //////////////////////////////////////////////////////////////////////

		bool error = false;
		try
		{
			selectedIds->Clear();

			FdoPtr<FdoComputedIdentifier> cid = (FdoComputedIdentifier*)FdoExpression::Parse(L"(Upper(LotSize)) AS TestUpper");

			selectedIds->Add(cid);

			datareader = select->Execute ();
			while (datareader->ReadNext ())
			{
				double result = datareader->GetDouble(L"TestUpper");
			}
		}
		catch (FdoException* e)
		{
			//printf("Expected: %ls\n", e->GetExceptionMessage());
			e->Release();
			error = true;
		}
		datareader->Close();
		datareader = NULL;

		CPPUNIT_ASSERT_MESSAGE("Upper succeeded with wrong datatype", error == true );
    }
    catch (FdoException* e)
    {
        fail(e);
    }
}

void SelectAggregatesTests::select_orderby_decimal ()
{
    try
    {
        //////////////////////////////////////////////////////////////////////
        // Create a SHP file:
        //////////////////////////////////////////////////////////////////////

        FdoString *className  = L"MyClass";
        FdoString *schemaName = L"MySchema";

        create_schema(schemaName, className, FdoGeometricType_Curve, false, false, true);


        //////////////////////////////////////////////////////////////////////
        // Select some data via orderby:
        //////////////////////////////////////////////////////////////////////

        FdoPtr<FdoISelectAggregates> select = (FdoISelectAggregates*)mConnection->CreateCommand (FdoCommandType_SelectAggregates);
        select->SetFeatureClassName (className);
        select->SetDistinct(false);
        select->SetOrderingOption(FdoOrderingOption_Descending);
        FdoPtr<FdoIdentifierCollection> orderingIds = select->GetOrdering();
        orderingIds->Add( FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"LotSize")) );
        FdoPtr<FdoIdentifierCollection> selectedIds = select->GetPropertyNames();
        selectedIds->Clear();
        selectedIds->Add( FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"LotSize")) );
        FdoPtr<FdoIDataReader> datareader = select->Execute ();
        long count = 0;
        double size = 0.0;
        double previous_size = 0.0;
        while (datareader->ReadNext ())
        {
            previous_size = size;
            size = datareader->GetDouble (L"LotSize");
            if (previous_size != 0.0)
                CPPUNIT_ASSERT_MESSAGE("out of order", size < previous_size);
            count++;
        }
        CPPUNIT_ASSERT_MESSAGE("Wrong orderby rowcount", count==4);
        datareader->Close();
        datareader = NULL;
    }
    catch (FdoException* e)
    {
        fail(e);
    }
}


void SelectAggregatesTests::select_orderby_scrambled_property_ids ()
{
    try
    {
        //////////////////////////////////////////////////////////////////////
        // Create a SHP file:
        //////////////////////////////////////////////////////////////////////

        FdoString *className  = L"MyClass";
        FdoString *schemaName = L"MySchema";

        create_schema(schemaName, className, FdoGeometricType_Curve, false, false, true);

        //////////////////////////////////////////////////////////////////////
        // Test:
        //////////////////////////////////////////////////////////////////////

        // DescribeSchema:
        FdoPtr<FdoIDescribeSchema> getSchemaCmd = static_cast<FdoIDescribeSchema*>(mConnection->CreateCommand(FdoCommandType_DescribeSchema));
        FdoPtr<FdoFeatureSchemaCollection> schemas = getSchemaCmd->Execute();

        FdoPtr<FdoFeatureSchema> schema = schemas->GetItem(schemaName);
        FdoPtr<FdoClassCollection> classes = schema->GetClasses();
        FdoPtr<FdoClassDefinition> klass = classes->GetItem(className);

        // Get a feature reader for a selection ordered by the first identy property of the class.
        FdoPtr<FdoISelectAggregates> select = (FdoISelectAggregates*) mConnection->CreateCommand (FdoCommandType_SelectAggregates);
        select->SetFeatureClassName (klass->GetName());
        select->SetDistinct(false);
        select->SetOrderingOption(FdoOrderingOption_Descending);
        FdoPtr<FdoIdentifierCollection> orderingIds = select->GetOrdering();
        FdoPtr<FdoDataPropertyDefinitionCollection> idProps = klass->GetIdentityProperties();
        FdoPtr<FdoDataPropertyDefinition> idProp = idProps->GetItem(0);
        FdoString* firstIdentPropName = idProp->GetName();
        orderingIds->Add( FdoPtr<FdoIdentifier>(FdoIdentifier::Create(firstIdentPropName)) );
        FdoPtr<FdoIdentifierCollection> selectedIds = select->GetPropertyNames();

        // insert property names in 'SCRAMBLED' order (actually, reverse order of FdoClassDefinition):
        selectedIds->Clear();
        selectedIds->Add( FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"Geometry")) );
        selectedIds->Add( FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"Date")) );
        selectedIds->Add( FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"HeadCount")) );
        selectedIds->Add( FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"LotSize")) );
        selectedIds->Add( FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"Street")) );
        selectedIds->Add( FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"FeatId")) );

        FdoPtr<FdoIReader> reader = select->Execute ();

        int count = 1;
        while (reader->ReadNext())
        {
            FdoString* street = reader->GetString(L"Street");

            CPPUNIT_ASSERT_MESSAGE("Incorrect street name", 
                (0==wcscmp(street, L"Slater")) || (0==wcscmp(street, L"Albert")) || (0==wcscmp(street, L"Queen")) );
        }
    }
    catch (FdoException* e)
    {
        fail(e);
    }
}


void SelectAggregatesTests::select_orderby_empty_select_list ()
{
    try
    {
        //////////////////////////////////////////////////////////////////////
        // Create a SHP file:
        //////////////////////////////////////////////////////////////////////

        FdoString *className  = L"MyClass";
        FdoString *schemaName = L"MySchema";

        create_schema(schemaName, className, FdoGeometricType_Curve, false, false, true);


        //////////////////////////////////////////////////////////////////////
        // Select some data via orderby:
        //////////////////////////////////////////////////////////////////////

        FdoPtr<FdoISelectAggregates> select = (FdoISelectAggregates*)mConnection->CreateCommand (FdoCommandType_SelectAggregates);
        select->SetFeatureClassName (className);
        select->SetDistinct(false);
        select->SetOrderingOption(FdoOrderingOption_Descending);
        FdoPtr<FdoIdentifierCollection> orderingIds = select->GetOrdering();
        orderingIds->Add( FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"Street")) );
        FdoPtr<FdoIdentifierCollection> selectedIds = select->GetPropertyNames();
        selectedIds->Clear();
        // KEEP THE SELECT LIST EMPTY TO TEST THIS BEHAVIOR CORRECTLY
        //selectedIds->Add( FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"Street")) );
        FdoPtr<FdoIDataReader> datareader = select->Execute ();
        long count = 0;
        FdoStringP street;
        FdoStringP previousStreet;
        while (datareader->ReadNext ())
        {
            previousStreet = street;
            street = datareader->GetString(L"Street");
            if (previousStreet != NULL)
                CPPUNIT_ASSERT(0>=wcscmp(street, previousStreet));

            FdoDateTime datetime = datareader->GetDateTime(L"Date");
            double lotsize = datareader->GetDouble(L"LotSize");
            double headcount = datareader->GetDouble(L"HeadCount");
            FdoPtr<FdoByteArray> geom = datareader->GetGeometry(L"Geometry");

            count++;
        }
        CPPUNIT_ASSERT_MESSAGE("Wrong orderby rowcount", count==4);
        datareader->Close();
        datareader = NULL;
    }
    catch (FdoException* e)
    {
        fail(e);
    }
}


// This test is ported from SDF Provider:
void SelectAggregatesTests::selectAggregatesSpatialExtentsTest()
{
    FdoPtr<FdoISelectAggregates> selAggr;
    FdoPtr<FdoIDataReader> rdr;
    FdoPtr<FdoIdentifierCollection> ids;
    FdoPtr<FdoExpression> expr;
    FdoPtr<FdoComputedIdentifier> cid;
    int count;
    FdoPtr<FdoIConnection> conn = FDO_SAFE_ADDREF(mConnection.p);


    // Create class with some test data in it:
    FdoString *className  = L"MyClass";
    FdoString *schemaName = L"MySchema";
    create_schema(schemaName, className, FdoGeometricType_Curve, false, false, true);

    FdoPtr<FdoISelectAggregates> advsel = (FdoISelectAggregates*)(conn->CreateCommand(FdoCommandType_SelectAggregates));

    advsel->SetFeatureClassName(L"MyClass");
    
    ids = advsel->GetPropertyNames();

    expr = FdoExpression::Parse(L"SpatialExtents(Geometry)");
    cid = FdoComputedIdentifier::Create(L"MBR", expr);
    ids->Add(cid);


    // Test the optimized case (no filter, no other identifiers in select property list):
    ////////////////////////////////////////////////////////////////////////////////////

    FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
    rdr = advsel->Execute();

    count = 0;

    FdoPtr<FdoIEnvelope> envelopeAllWithoutFilter;
    while (rdr->ReadNext())
    {
        FdoPtr<FdoByteArray> geomBytes = rdr->GetGeometry(L"MBR");
        FdoPtr<FdoIGeometry> geom = gf->CreateGeometryFromFgf(geomBytes);
        envelopeAllWithoutFilter = geom->GetEnvelope();
        if (envelopeAllWithoutFilter->GetIsEmpty())
            CPPUNIT_FAIL("Expected non-empty envelope for SpatialExtents() result");
        count++;
    }
    CPPUNIT_ASSERT_MESSAGE("Expected exactly one row of aggregate data", count==1);

    rdr->Close();


    // Test the non-optimized case (with a filter provided):
    ////////////////////////////////////////////////////////////////////////////////////

    advsel->SetFilter(L"LotSize != -1.0");  // this is an arbitrary value that should allow all rows through
    rdr = advsel->Execute();

    count = 0;
    FdoPtr<FdoIEnvelope> envelopeAllWithFilter;
    while (rdr->ReadNext())
    {
        FdoPtr<FdoByteArray> geomBytes = rdr->GetGeometry(L"MBR");
        FdoPtr<FdoIGeometry> geom = gf->CreateGeometryFromFgf(geomBytes);
        envelopeAllWithFilter = geom->GetEnvelope();
        if (envelopeAllWithFilter->GetIsEmpty())
            CPPUNIT_FAIL("Expected non-empty envelope for SpatialExtents() result");

        count++;
    }
    CPPUNIT_ASSERT_MESSAGE("Expected exactly one row of aggregate data", count==1);

    rdr->Close();

    // Compare the two "all" results to make sure they are equivalent:
    CPPUNIT_ASSERT_MESSAGE("SpatialExtents results don't match (MinX)", envelopeAllWithoutFilter->GetMinX() == envelopeAllWithFilter->GetMinX());
    CPPUNIT_ASSERT_MESSAGE("SpatialExtents results don't match (MinY)", envelopeAllWithoutFilter->GetMinY() == envelopeAllWithFilter->GetMinY());
    CPPUNIT_ASSERT_MESSAGE("SpatialExtents results don't match (MinZ)", FdoCommonOSUtil::_isnan(envelopeAllWithoutFilter->GetMinZ()) && FdoCommonOSUtil::_isnan(envelopeAllWithFilter->GetMinZ()));
    CPPUNIT_ASSERT_MESSAGE("SpatialExtents results don't match (MaxX)", envelopeAllWithoutFilter->GetMaxX() == envelopeAllWithFilter->GetMaxX());
    CPPUNIT_ASSERT_MESSAGE("SpatialExtents results don't match (MaxY)", envelopeAllWithoutFilter->GetMaxY() == envelopeAllWithFilter->GetMaxY());
    CPPUNIT_ASSERT_MESSAGE("SpatialExtents results don't match (MaxZ)", FdoCommonOSUtil::_isnan(envelopeAllWithoutFilter->GetMaxZ()) && FdoCommonOSUtil::_isnan(envelopeAllWithFilter->GetMaxZ()));


    // Test the non-optimized case (filter that returns NULL):
    ////////////////////////////////////////////////////////////////////////////////////

    advsel->SetFilter(L"LotSize = -1");  // will select 0 rows
    rdr = advsel->Execute();

    count = 0;
    while (rdr->ReadNext())
    {
        CPPUNIT_ASSERT_MESSAGE("Expected NULL result from SpatialExtents with filter returning 0 rows", rdr->IsNull(L"MBR"));
        count++;
    }
    CPPUNIT_ASSERT_MESSAGE("Expected exactly one row of aggregate data", count==1);

    rdr->Close();


    // Test the non-optimized case (no filter, more than one thing in the identifier property list):
    ////////////////////////////////////////////////////////////////////////////////////

    advsel->SetFilter((FdoFilter*)NULL);  // no filter
    ids->Add(FdoPtr<FdoIdentifier>(FdoComputedIdentifier::Create(L"MyCount",
                FdoPtr<FdoExpression>(FdoExpression::Parse(L"Count(Geometry)"))
             )));
    rdr = advsel->Execute();

    count = 0;
    FdoPtr<FdoIEnvelope> envelopeAllWithoutFilterMultipleIds;
    while (rdr->ReadNext())
    {
        FdoPtr<FdoByteArray> geomBytes = rdr->GetGeometry(L"MBR");
        FdoPtr<FdoIGeometry> geom = gf->CreateGeometryFromFgf(geomBytes);
        envelopeAllWithoutFilterMultipleIds = geom->GetEnvelope();
        if (envelopeAllWithoutFilterMultipleIds->GetIsEmpty())
            CPPUNIT_FAIL("Expected non-empty envelope for SpatialExtents() result");

        FdoInt64 mycount = rdr->GetInt64(L"MyCount");
        CPPUNIT_ASSERT_MESSAGE("Wrong row count", 4 == mycount);

        count++;
    }
    CPPUNIT_ASSERT_MESSAGE("Expected exactly one row of aggregate data", count==1);

    rdr->Close();

    // Compare the two "all" results to make sure they are equivalent:
    CPPUNIT_ASSERT_MESSAGE("SpatialExtents results don't match (MinX)", envelopeAllWithoutFilter->GetMinX() == envelopeAllWithoutFilterMultipleIds->GetMinX());
    CPPUNIT_ASSERT_MESSAGE("SpatialExtents results don't match (MinY)", envelopeAllWithoutFilter->GetMinY() == envelopeAllWithoutFilterMultipleIds->GetMinY());
    CPPUNIT_ASSERT_MESSAGE("SpatialExtents results don't match (MinZ)", FdoCommonOSUtil::_isnan(envelopeAllWithoutFilter->GetMinZ()) && FdoCommonOSUtil::_isnan(envelopeAllWithoutFilterMultipleIds->GetMinZ()));
    CPPUNIT_ASSERT_MESSAGE("SpatialExtents results don't match (MaxX)", envelopeAllWithoutFilter->GetMaxX() == envelopeAllWithoutFilterMultipleIds->GetMaxX());
    CPPUNIT_ASSERT_MESSAGE("SpatialExtents results don't match (MaxY)", envelopeAllWithoutFilter->GetMaxY() == envelopeAllWithoutFilterMultipleIds->GetMaxY());
    CPPUNIT_ASSERT_MESSAGE("SpatialExtents results don't match (MaxZ)", FdoCommonOSUtil::_isnan(envelopeAllWithoutFilter->GetMaxZ()) && FdoCommonOSUtil::_isnan(envelopeAllWithoutFilterMultipleIds->GetMaxZ()));


    // Create new empty SHP file :
    ////////////////////////////////////////////////////////////////////////////////////

    create_schema(schemaName, className, FdoGeometricType_Curve, false, false, false);


    // Test the optimized case (no filter, no rows of data):
    ////////////////////////////////////////////////////////////////////////////////////

    selAggr = (FdoISelectAggregates*)conn->CreateCommand(FdoCommandType_SelectAggregates);
    selAggr->SetFeatureClassName(L"MyClass");
    ids = selAggr->GetPropertyNames();

    expr = FdoExpression::Parse(L"SpatialExtents(Geometry)");
    cid = FdoComputedIdentifier::Create(L"MBR", expr);
    ids->Add(cid);

    rdr = selAggr->Execute();
    count = 0;
    while (rdr->ReadNext())
    {
        CPPUNIT_ASSERT_MESSAGE("Expected NULL result from SpatialExtents with filter returning 0 rows", rdr->IsNull(L"MBR"));
        count++;
    }
    CPPUNIT_ASSERT_MESSAGE("Expected exactly one row of aggregate data", count==1);

    rdr->Close();


    // Insert many rows of data, for performance tests :
    ////////////////////////////////////////////////////////////////////////////////////

    const int PERF_TEST_NUM_ROWS = 4000;
    clock_t start;
    clock_t finish;
    FdoPtr<FdoIFeatureReader> featRdr;

    FdoPtr<FdoIInsert> ins = (FdoIInsert*)conn->CreateCommand(FdoCommandType_Insert);
    ins->SetFeatureClassName(L"MyClass");
    FdoPtr<FdoPropertyValueCollection> propVals = ins->GetPropertyValues();
    FdoPtr<FdoFgfGeometryFactory> fgf = FdoFgfGeometryFactory::GetInstance();
    double ordinates[2*2];  //2 XY coordinates
    ordinates[0] =  10.0; ordinates[1] =  10.0;
    ordinates[2] = 100.0; ordinates[3] = 100.0;
    FdoPtr<FdoIGeometry> geom = fgf->CreateLineString(FdoDimensionality_XY, ELEMENTS(ordinates), ordinates);
    FdoPtr<FdoByteArray> geomBytes = fgf->GetFgf(geom);
    FdoPtr<FdoGeometryValue> geomVal = FdoGeometryValue::Create(geomBytes);
    FdoPtr<FdoPropertyValue> propVal = FdoPropertyValue::Create(L"Geometry", geomVal);
    propVals->Add(propVal);
    for (int i=0; i<PERF_TEST_NUM_ROWS; i++)
    {
        featRdr = ins->Execute();
        featRdr->Close();
    }


    // Test the optimized case for performance (no filter, PERF_TEST_NUM_ROWS rows of data):
    ////////////////////////////////////////////////////////////////////////////////////

    selAggr = (FdoISelectAggregates*)conn->CreateCommand(FdoCommandType_SelectAggregates);
    selAggr->SetFeatureClassName(L"MyClass");
    ids = selAggr->GetPropertyNames();

    expr = FdoExpression::Parse(L"SpatialExtents(Geometry)");
    cid = FdoComputedIdentifier::Create(L"MBR", expr);
    ids->Add(cid);

    start = clock();
    rdr = selAggr->Execute();
    count = 0;
    while (rdr->ReadNext())
    {
        CPPUNIT_ASSERT_MESSAGE("Expected not-null result from SpatialExtents with filter returning all rows", !rdr->IsNull(L"MBR"));
        FdoPtr<FdoByteArray> bytes = rdr->GetGeometry(L"MBR");
        count++;
    }
    CPPUNIT_ASSERT_MESSAGE("Expected exactly one row of aggregate data", count==1);
    rdr->Close();
    finish = clock();

    wprintf(L"Seconds to read SpatialExtents from SHP file containing %d records, using optimization: %g\n", PERF_TEST_NUM_ROWS, (double)(finish-start)/CLOCKS_PER_SEC);


    // Test the non-optimized case for performance (no filter, PERF_TEST_NUM_ROWS rows of data):
    ////////////////////////////////////////////////////////////////////////////////////

    selAggr = (FdoISelectAggregates*)conn->CreateCommand(FdoCommandType_SelectAggregates);
    selAggr->SetFeatureClassName(L"MyClass");
    selAggr->SetFilter(L"FeatId > -1");
    ids = selAggr->GetPropertyNames();

    expr = FdoExpression::Parse(L"SpatialExtents(Geometry)");
    cid = FdoComputedIdentifier::Create(L"MBR", expr);
    ids->Add(cid);

    start = clock();
    rdr = selAggr->Execute();
    count = 0;
    while (rdr->ReadNext())
    {
        CPPUNIT_ASSERT_MESSAGE("Expected not-null result from SpatialExtents with filter returning all rows", !rdr->IsNull(L"MBR"));
        FdoPtr<FdoByteArray> bytes = rdr->GetGeometry(L"MBR");
        count++;
    }
    CPPUNIT_ASSERT_MESSAGE("Expected exactly one row of aggregate data", count==1);
    rdr->Close();
    finish = clock();

    wprintf(L"Seconds to read SpatialExtents from SHP file containing %d records, using no optimization: %g\n", PERF_TEST_NUM_ROWS, (double)(finish-start)/CLOCKS_PER_SEC);
}
