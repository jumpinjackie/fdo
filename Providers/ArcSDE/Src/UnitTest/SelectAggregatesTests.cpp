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
#include "SelectAggregatesTests.h"

CPPUNIT_TEST_SUITE_REGISTRATION (SelectAggregatesTests);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION (SelectAggregatesTests, "SelectAggregatesTests");

// NOTE: to generate the data required for the 'AIRPORT'-related features, you must
//    export all feature classes in <arcgis root folder>\ArcTutor\Map\airport.mdb to an enterprise
//    geodatabase that matches the ArcSDETestConfig::ConnStringAustralia().  The feature classes should be exported
//    with coordinat system with the following extents ("X/Y domain") so that all data can be fit:
//	       minx=2,100,000  maxx=2,600,000
//	       miny=  500,000  maxy=1,000,000
//    Any coordinate system can be used to store the data, as airport.mdb doesn't specify a coordinate system.


FdoPtr<FdoIConnection> SelectAggregatesTests::mConnection;

SelectAggregatesTests::SelectAggregatesTests (void)
{
}

SelectAggregatesTests::~SelectAggregatesTests (void)
{
}

void SelectAggregatesTests::setUp ()
{
    if (!CreateSchemaOnly() /* || !bSchemaCreated */ )
    {
        mConnection = ArcSDETests::GetConnection ();
        mConnection->SetConnectionString (ArcSDETestConfig::ConnStringAustralia());
        mConnection->Open ();
    }
}

void SelectAggregatesTests::tearDown ()
{
    if (mConnection)
        mConnection->Close ();
}

void SelectAggregatesTests::test_distinct ()
{
    if (CreateSchemaOnly()) return;

    try
    {
        ////////////////////////////////////////////////////////////////////////////
        // test distinct on one property:
        ////////////////////////////////////////////////////////////////////////////

        FdoPtr<FdoISelectAggregates> selectAggregates = (FdoISelectAggregates*)mConnection->CreateCommand (FdoCommandType_SelectAggregates);
        selectAggregates->SetFeatureClassName (ArcSDETestConfig::QClassNameParcels());
        selectAggregates->SetDistinct(true);
        FdoPtr<FdoIdentifierCollection> idColl = selectAggregates->GetPropertyNames();
        idColl->Add( FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"RTYPE")) );
        FdoPtr<FdoIDataReader> reader = selectAggregates->Execute ();

        // Test accessing DataReader's metadata BEFORE calling ReadNext():
        IterateDataReaderProperties(reader);

        long lCount=0;
        bool bBlankFound = false;
        bool bAgrFound = false;
        bool bExmFound = false;
        bool bMerFound = false;
        bool bMfgFound = false;
        bool bResFound = false;
        bool bSnwFound = false;
        bool bWtcFound = false;

        while (reader->ReadNext ())
        {
            FdoString *val = reader->GetString(L"RTYPE");

            if (0==wcscmp(val, L" "))  bBlankFound = true;
            if (0==wcscmp(val, L"AGR"))  bAgrFound = true;
            if (0==wcscmp(val, L"EXM"))  bExmFound = true;
            if (0==wcscmp(val, L"MER"))  bMerFound = true;
            if (0==wcscmp(val, L"MFG"))  bMfgFound = true;
            if (0==wcscmp(val, L"RES"))  bResFound = true;
            if (0==wcscmp(val, L"S&W"))  bSnwFound = true;
            if (0==wcscmp(val, L"WTC"))  bWtcFound = true;

            try
            {
                double dVal = reader->GetDouble(L"RTYPE");
                CPPUNIT_FAIL("Didn't receive expected exception when accessing incorrect reader property type");
            }
            catch (FdoException *e)
            {
                // We expect an exception here:
                e->Release();
            }

            lCount ++;
        }
        reader->Close();
        CPPUNIT_ASSERT_MESSAGE("Got wrong number of rows for select distinct", lCount==8);
        CPPUNIT_ASSERT_MESSAGE("Didn't receive expected row value ' '", bBlankFound);
        CPPUNIT_ASSERT_MESSAGE("Didn't receive expected row value 'AGR'", bAgrFound);
        CPPUNIT_ASSERT_MESSAGE("Didn't receive expected row value 'EXM'", bExmFound);
        CPPUNIT_ASSERT_MESSAGE("Didn't receive expected row value 'MER'", bMerFound);
        CPPUNIT_ASSERT_MESSAGE("Didn't receive expected row value 'MFG'", bMfgFound);
        CPPUNIT_ASSERT_MESSAGE("Didn't receive expected row value 'RES'", bResFound);
        CPPUNIT_ASSERT_MESSAGE("Didn't receive expected row value 'S&W'", bSnwFound);
        CPPUNIT_ASSERT_MESSAGE("Didn't receive expected row value 'WTC'", bWtcFound);


        ////////////////////////////////////////////////////////////////////////////
        // test distinct on more than one property (should fail):
        ////////////////////////////////////////////////////////////////////////////

        selectAggregates = (FdoISelectAggregates*)mConnection->CreateCommand (FdoCommandType_SelectAggregates);
        selectAggregates->SetFeatureClassName (ArcSDETestConfig::QClassNameParcels());
        selectAggregates->SetDistinct(true);
        idColl = selectAggregates->GetPropertyNames();
        idColl->Clear();
        try
        {
            reader = selectAggregates->Execute ();
            CPPUNIT_FAIL("Select distinct on more than one property should have failed but did not");
        }
        catch (FdoException *e)
        {
            // We expect to get here, so don't raise a cppunit error
            e->Release();
        }
        reader->Close();
    }
    catch (FdoException *e)
    {
        fail(e);
    }
}


void SelectAggregatesTests::test_aggregate_functions ()
{
    if (CreateSchemaOnly()) return;

    try
    {
        ////////////////////////////////////////////////////////////////////////////
        // test AVG aggregate function:
        ////////////////////////////////////////////////////////////////////////////

        FdoPtr<FdoISelectAggregates> selectAggregates = (FdoISelectAggregates*)mConnection->CreateCommand (FdoCommandType_SelectAggregates);
        selectAggregates->SetFeatureClassName (ArcSDETestConfig::QClassNameOntarioRoads());
        FdoPtr<FdoIdentifierCollection> idColl = selectAggregates->GetPropertyNames();

        // Add aggregate function:
        FdoPtr<FdoExpressionCollection> functionArgs = FdoExpressionCollection::Create();
        FdoPtr<FdoIdentifier> arg1 = FdoIdentifier::Create(L"LENGTH");
        functionArgs->Add(arg1);
        FdoPtr<FdoFunction> computedExpr = FdoFunction::Create(L"Avg", functionArgs);
        FdoPtr<FdoComputedIdentifier> computedId = FdoComputedIdentifier::Create(L"MyComputedId", computedExpr);
        idColl->Add(computedId);

        // perform query:
        FdoPtr<FdoIDataReader> reader = selectAggregates->Execute ();

        // Test accessing DataReader's metadata BEFORE calling ReadNext():
        IterateDataReaderProperties(reader);


        // Iterate and verify results:
        long lCount=0;
        while (reader->ReadNext ())
        {
            double val = reader->GetDouble(L"MyComputedId");
            CPPUNIT_ASSERT_MESSAGE("Didn't get correct average shape length", fuzzyEqual(val, 22258.7093888));

            try
            {
                double dValTest = reader->GetDouble(L"LENGTH");
                CPPUNIT_FAIL("Didn't receive expected exception when accessing incorrect reader property name");
            }
            catch (FdoException *e)
            {
                // We expect to get an exception here
                e->Release();
            }

            lCount ++;
        }
        reader->Close();
        CPPUNIT_ASSERT_MESSAGE("Got wrong number of rows for select aggregate", lCount==1);


        ////////////////////////////////////////////////////////////////////////////
        // test COUNT aggregate function (with filter):
        ////////////////////////////////////////////////////////////////////////////

        selectAggregates = (FdoISelectAggregates*)mConnection->CreateCommand (FdoCommandType_SelectAggregates);
        selectAggregates->SetFeatureClassName (ArcSDETestConfig::QClassNameOntarioRoads());
        idColl = selectAggregates->GetPropertyNames();

        // Add aggregate function:
        functionArgs = FdoExpressionCollection::Create();
        arg1 = FdoIdentifier::Create(L"LENGTH");
        functionArgs->Add(arg1);
        computedExpr = FdoFunction::Create(L"Count", functionArgs);
        computedId = FdoComputedIdentifier::Create(L"MyCount", computedExpr);
        idColl->Add(computedId);

        // Add filter:
        selectAggregates->SetFilter(L"LENGTH < 12000");

        // perform query:
        reader = selectAggregates->Execute ();

        // Test accessing DataReader's metadata BEFORE calling ReadNext():
        IterateDataReaderProperties(reader);

        // Iterate and verify results:
        lCount=0;
        while (reader->ReadNext ())
        {
            FdoInt64 val = reader->GetInt64(L"MyCount");
            CPPUNIT_ASSERT_MESSAGE("Didn't get correct count", val==440);

            try
            {
                double dValTest = reader->GetDouble(L"MyCount");
                CPPUNIT_FAIL("Didn't receive expected exception when accessing incorrect reader property type");
            }
            catch (FdoException *e)
            {
                // We expect to get an exception here
                e->Release();
            }

            lCount ++;
        }
        reader->Close();
        CPPUNIT_ASSERT_MESSAGE("Got wrong number of rows for select aggregate", lCount==1);


        ////////////////////////////////////////////////////////////////////////////
        // test SUM aggregate function:
        ////////////////////////////////////////////////////////////////////////////

        selectAggregates = (FdoISelectAggregates*)mConnection->CreateCommand (FdoCommandType_SelectAggregates);
        selectAggregates->SetFeatureClassName (ArcSDETestConfig::QClassNameOntarioRoads());
        idColl = selectAggregates->GetPropertyNames();

        // Add aggregate function:
        functionArgs = FdoExpressionCollection::Create();
        arg1 = FdoIdentifier::Create(L"LENGTH");
        functionArgs->Add(arg1);
        computedExpr = FdoFunction::Create(L"Sum", functionArgs);
        computedId = FdoComputedIdentifier::Create(L"MySum", computedExpr);
        idColl->Add(computedId);

        // perform query:
        reader = selectAggregates->Execute ();

        // Test accessing DataReader's metadata BEFORE calling ReadNext():
        IterateDataReaderProperties(reader);

        // Iterate and verify results:
        lCount=0;
        while (reader->ReadNext ())
        {
            double val = reader->GetDouble(L"MySum");
            CPPUNIT_ASSERT_MESSAGE("Didn't get correct sum", fuzzyEqual(val, 24217475.815));

            try
            {
                double valTest = reader->GetDouble(L"Sum");
                CPPUNIT_FAIL("Didn't receive expected exception when accessing incorrect reader property name");
            }
            catch (FdoException *e)
            {
                // We expect to get an exception here
                e->Release();
            }

            lCount ++;
        }
        reader->Close();
        CPPUNIT_ASSERT_MESSAGE("Got wrong number of rows for select aggregate", lCount==1);


        ////////////////////////////////////////////////////////////////////////////
        // test MIN + MAX + STDDEV aggregate functions altogether at once:
        ////////////////////////////////////////////////////////////////////////////

        selectAggregates = (FdoISelectAggregates*)mConnection->CreateCommand (FdoCommandType_SelectAggregates);
        selectAggregates->SetFeatureClassName (ArcSDETestConfig::QClassNameOntarioRoads());
        idColl = selectAggregates->GetPropertyNames();

        // Add aggregate function:
        functionArgs = FdoExpressionCollection::Create();
        arg1 = FdoIdentifier::Create(L"LENGTH");
        functionArgs->Add(arg1);
        computedExpr = FdoFunction::Create(L"Min", functionArgs);
        computedId = FdoComputedIdentifier::Create(L"MyMin", computedExpr);
        idColl->Add(computedId);

        // Add aggregate function:
        functionArgs = FdoExpressionCollection::Create();
        arg1 = FdoIdentifier::Create(L"LENGTH");
        functionArgs->Add(arg1);
        computedExpr = FdoFunction::Create(L"Max", functionArgs);
        computedId = FdoComputedIdentifier::Create(L"MyMax", computedExpr);
        idColl->Add(computedId);

        // Add aggregate function:
        functionArgs = FdoExpressionCollection::Create();
        arg1 = FdoIdentifier::Create(L"LENGTH");
        functionArgs->Add(arg1);
        computedExpr = FdoFunction::Create(L"StdDev", functionArgs);
        computedId = FdoComputedIdentifier::Create(L"MyStdDev", computedExpr);
        idColl->Add(computedId);

        // perform query:
        reader = selectAggregates->Execute ();

        // Test accessing DataReader's metadata BEFORE calling ReadNext():
        IterateDataReaderProperties(reader);

        // Iterate and verify results:
        lCount=0;
        while (reader->ReadNext ())
        {
            double valMin = reader->GetDouble(L"MyMin");
            CPPUNIT_ASSERT_MESSAGE("Didn't get correct min", fuzzyEqual(valMin, 305.748));

            double valMax = reader->GetDouble(L"MyMax");
            CPPUNIT_ASSERT_MESSAGE("Didn't get correct max", fuzzyEqual(valMax, 243623.594));

            double valStdDev = reader->GetDouble(L"MyStdDev");
            CPPUNIT_ASSERT_MESSAGE("Didn't get correct stddev", fuzzyEqual(valStdDev, 24567.8687));

            lCount ++;
        }
        reader->Close();
        CPPUNIT_ASSERT_MESSAGE("Got wrong number of rows for select aggregate", lCount==1);
    }
    catch (FdoException *e)
    {
        fail(e);
    }
}



void SelectAggregatesTests::test_orderby ()
{
    if (CreateSchemaOnly()) return;

    try
    {
        ////////////////////////////////////////////////////////////////////////////
        // test order by ascending on a double column:
        ////////////////////////////////////////////////////////////////////////////

        FdoPtr<FdoISelectAggregates> selectAggregates = (FdoISelectAggregates*)mConnection->CreateCommand (FdoCommandType_SelectAggregates);
        selectAggregates->SetFeatureClassName (ArcSDETestConfig::QClassNameOntarioRoads());
        selectAggregates->SetOrderingOption(FdoOrderingOption_Ascending);
        FdoPtr<FdoIdentifierCollection> orderingIds = selectAggregates->GetOrdering();
        FdoPtr<FdoIdentifier> orderingId = FdoIdentifier::Create(L"LENGTH");
        orderingIds->Add(orderingId);

        FdoPtr<FdoIDataReader> dataReader = selectAggregates->Execute();

        // Test accessing DataReader's metadata BEFORE calling ReadNext():
        IterateDataReaderProperties(dataReader);

        long lCount = 0;
        double dLastValue = 0.0;
        double dNewValue = 0.0;
        while (dataReader->ReadNext())
        {
            dLastValue = dNewValue;
            dNewValue = dataReader->GetDouble(L"LENGTH");
            CPPUNIT_ASSERT_MESSAGE("SelectAggregates ORDER BY ascending failed", (lCount==0) || (dNewValue >= dLastValue));

            lCount++;
        }
        CPPUNIT_ASSERT_MESSAGE("Got wrong number of rows for select aggregate with ORDER BY", lCount==1088);


        ////////////////////////////////////////////////////////////////////////////
        // test order by descending on a string column:
        ////////////////////////////////////////////////////////////////////////////

        selectAggregates = (FdoISelectAggregates*)mConnection->CreateCommand (FdoCommandType_SelectAggregates);
        selectAggregates->SetFeatureClassName (ArcSDETestConfig::QClassNameOntarioRoads());
        selectAggregates->SetOrderingOption(FdoOrderingOption_Descending);
        orderingIds = selectAggregates->GetOrdering();
        orderingId = FdoIdentifier::Create(L"FCODE");
        orderingIds->Add(orderingId);

        dataReader = selectAggregates->Execute();

        // Test accessing DataReader's metadata BEFORE calling ReadNext():
        IterateDataReaderProperties(dataReader);

        lCount = 0;
        FdoStringP lastValue;
        FdoStringP newValue;
        while (dataReader->ReadNext())
        {
            lastValue = newValue;
            newValue = dataReader->GetString(L"FCODE");
            CPPUNIT_ASSERT_MESSAGE("SelectAggregates ORDER BY descending failed", (lCount==0) || (newValue <= lastValue));

            lCount++;
        }
        CPPUNIT_ASSERT_MESSAGE("Got wrong number of rows for select aggregate with ORDER BY", lCount==1088);


        ////////////////////////////////////////////////////////////////////////////
        // test order by descending on a string column, PLUS spatial filter:
        ////////////////////////////////////////////////////////////////////////////

        selectAggregates = (FdoISelectAggregates*)mConnection->CreateCommand (FdoCommandType_SelectAggregates);
        selectAggregates->SetFeatureClassName (ArcSDETestConfig::QClassNameOntarioRoads());
        selectAggregates->SetFilter(L"GEOMETRY INTERSECTS GEOMFROMTEXT ('POLYGON XY (( 800000.0 12700000.0, 900000.0 12700000.0, 900000.0 12300000.0, 800000.0 12300000.0, 800000.0 12700000.0 ))')");
        selectAggregates->SetOrderingOption(FdoOrderingOption_Descending);
        orderingIds = selectAggregates->GetOrdering();
        orderingId = FdoIdentifier::Create(L"FCODE");
        orderingIds->Add(orderingId);

        dataReader = selectAggregates->Execute();

        // Test accessing DataReader's metadata BEFORE calling ReadNext():
        IterateDataReaderProperties(dataReader);

        lCount = 0;
        lastValue;
        newValue;
        while (dataReader->ReadNext())
        {
            lastValue = newValue;
            newValue = dataReader->GetString(L"FCODE");
            CPPUNIT_ASSERT_MESSAGE("SelectAggregates ORDER BY descending failed", (lCount==0) || (newValue <= lastValue));

            lCount++;
        }
        CPPUNIT_ASSERT_MESSAGE("Got wrong number of rows for select aggregate with ORDER BY", lCount==18);
    }
    catch (FdoException *e)
    {
        fail(e);
    }
}


void SelectAggregatesTests::IterateDataReaderProperties(FdoIDataReader* dataReader)
{
    FdoInt32   propCount = dataReader->GetPropertyCount();
    FdoString *propName = NULL;
    FdoDataType propDataType;
    FdoPropertyType propType;

    for (FdoInt32 i=0; i<propCount; i++)
    {
        propName = dataReader->GetPropertyName(i);
        propDataType = dataReader->GetDataType(propName);
        propType = dataReader->GetPropertyType(propName);
    }
}

