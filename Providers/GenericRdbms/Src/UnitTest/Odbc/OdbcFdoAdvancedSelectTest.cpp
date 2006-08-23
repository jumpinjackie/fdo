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
#include "OdbcFdoAdvancedSelectTest.h"
#include "UnitTestUtil.h"

#ifdef _WIN32
#pragma message ("Info: Some OdbcAccessFdoAdvancedSelectTest tests are not yet enabled.")
CPPUNIT_TEST_SUITE_REGISTRATION( OdbcAccessFdoAdvancedSelectTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcAccessFdoAdvancedSelectTest, "FdoAdvancedSelectTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcAccessFdoAdvancedSelectTest, "OdbcAccessFdoAdvancedSelectTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcAccessFdoAdvancedSelectTest, "OdbcAccessTests");

void OdbcAccessFdoAdvancedSelectTest::set_provider()
{
	UnitTestUtil::SetProvider( "OdbcAccess" );
}

void OdbcAccessFdoAdvancedSelectTest::connect ()
{
    try
    {
        mConnection = UnitTestUtil::GetProviderConnectionObject();
        mConnection->SetConnectionString(UnitTestUtil::GetConnectionString());
        mConnection->Open();
    }
    catch (FdoException *ex)
    {
        if (mConnection)
        {
            mConnection->Release();
            mConnection= NULL;
        }
        UnitTestUtil::fail (ex);
    }
}

void OdbcAccessFdoAdvancedSelectTest::TestDefect785616()
{
    try
    {
        FdoPtr<FdoISelectAggregates> selectCmd = (FdoISelectAggregates*)mConnection->CreateCommand(FdoCommandType_SelectAggregates);

        selectCmd->SetFeatureClassName(L"CITIES_PRIMARY");

        FdoPtr<FdoIDataReader> reader = selectCmd->Execute();

        int numFeatures = 0;
        while (reader->ReadNext())
        {
            numFeatures++;
            UnitTestUtil::ProcessData(reader);
        }

        printf("   %i feature(s) read\n", numFeatures);

        // close the reader
        reader->Close();
    }
    catch (FdoException* e)
    {
        UnitTestUtil::fail (e);
    }
}

void OdbcAccessFdoAdvancedSelectTest::Table1Test()
{
    try
    {
        FdoPtr<FdoISelectAggregates> selectCmd = (FdoISelectAggregates*)mConnection->CreateCommand(FdoCommandType_SelectAggregates);

        // must set the feature class name
        selectCmd->SetFeatureClassName(GetTable1Classname());

        // execute the command
        FdoPtr<FdoIDataReader> reader = selectCmd->Execute();

        // read through all the features
        int numFeatures = 0;
        while (reader->ReadNext())
        {
            numFeatures++;
            UnitTestUtil::ProcessData(reader);
        }

        printf("   %i feature(s) read\n", numFeatures);

        // close the reader
        reader->Close();
    }
    catch (FdoException* e)
    {
        UnitTestUtil::fail (e);
    }
}

void OdbcAccessFdoAdvancedSelectTest::RestrictedPropertiesTable1Test()
{
    try
    {
        FdoPtr<FdoISelectAggregates> selectCmd = (FdoISelectAggregates*)mConnection->CreateCommand(FdoCommandType_SelectAggregates);

        // must set the feature class name
        selectCmd->SetFeatureClassName(GetTable1Classname());

        // restrict the set of properties to the named property
        FdoPtr<FdoIdentifier> propID1 = FdoIdentifier::Create(L"NAME");
        FdoPtr<FdoIdentifier> propID2 = FdoIdentifier::Create(L"Geometry");
        FdoPtr<FdoIdentifierCollection> propIDs = selectCmd->GetPropertyNames();
        propIDs->Add(propID1);
        propIDs->Add(propID2);

        // execute the command
        FdoPtr<FdoIDataReader> reader = selectCmd->Execute();
        // read through all the features
        int numFeatures = 0;
        while (reader->ReadNext())
        {
            numFeatures++;
            UnitTestUtil::ProcessData(reader);
        }

        printf("   %i feature(s) read\n", numFeatures);

        // close the reader
        reader->Close();
    }
    catch (FdoException* e)
    {
        UnitTestUtil::fail (e);
    }
}


void OdbcAccessFdoAdvancedSelectTest::ComparisonFilterTable1Test()
{
    try
    {
        FdoPtr<FdoISelectAggregates> selectCmd = (FdoISelectAggregates*)mConnection->CreateCommand(FdoCommandType_SelectAggregates);

        // must set the feature class name
        selectCmd->SetFeatureClassName(GetTable1Classname());
#if 0
        // create a comparison filter: Name == 'MyName'
        FdoPtr<FdoIdentifier>          dataPropID = FdoIdentifier::Create(L"NAME");
        FdoPtr<FdoDataValue>           dataValue  = FdoStringValue::Create(L"MyName");
        FdoPtr<FdoComparisonCondition> compCond   = FdoComparisonCondition::Create(
            dataPropID,
            FdoComparisonOperations_EqualTo,
            dataValue);

        // set the filter on the command
        selectCmd->SetFilter(compCond);
#else
        // create a comparison filter: Geometry within rectangle (8,8) to (17,17)
        FdoPtr<FdoIdentifier> geomPropID = FdoIdentifier::Create(L"Geometry");
        FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
        FdoPtr<FdoIEnvelope> pEnvelope = gf->CreateEnvelopeXY(8, 9, 17, 16);
        FdoPtr<FdoIGeometry> pGeometry = gf->CreateGeometry(pEnvelope);
        FdoPtr<FdoByteArray> pByteArray = gf->GetFgf(pGeometry);
        FdoPtr<FdoGeometryValue> pValue = FdoGeometryValue::Create(pByteArray);
        FdoPtr<FdoSpatialCondition> pSpatialCond = FdoSpatialCondition::Create(
            geomPropID, 
            FdoSpatialOperations_Within, 
            pValue);

        // set the filter on the command
        selectCmd->SetFilter(pSpatialCond);
#endif

        // execute the command
        FdoPtr<FdoIDataReader> reader = selectCmd->Execute();

        // read through all the features
        int numFeatures = 0;
        while (reader->ReadNext())
        {
            numFeatures++;
            UnitTestUtil::ProcessData(reader);
        }

        printf("   %i feature(s) read\n", numFeatures);

        // close the reader
        reader->Close();
    }
    catch (FdoException* e)
    {
        UnitTestUtil::fail (e);
    }
}


void OdbcAccessFdoAdvancedSelectTest::TestReaderDestructor()
{
#if 0
    // Test not enabled yet.  Try after fixing defect 673849.01.
    try
    {
        for (int i=0; i<5; i++)
        {
            FdoPtr<FdoISelectAggregates> selectCmd = (FdoISelectAggregates*)mConnection->CreateCommand(FdoCommandType_SelectAggregates);
            selectCmd->SetFeatureClassName(GetTable1Classname());
            FdoPtr<FdoIDataReader> reader = selectCmd->Execute();
            reader->Close();

            mConnection->Close();

            // Here, the reader smart pointer dies after the connection is closed, which leads to trouble
            // if our code tries to access (or free up) any connection-related resources
        }
    }
    catch (FdoException* e)
    {
        UnitTestUtil::fail (e);
    }
#endif
}


// A test that is hard-coded for a known table.
void OdbcAccessFdoAdvancedSelectTest::selectDistinctTest()
{
#if 0
    // Test not enabled yet.  Try after fixing defect 673849.01.
    try
    {
        FdoPtr<FdoISelectAggregates> selectAggrCmd = (FdoISelectAggregates*)mConnection->CreateCommand(FdoCommandType_SelectAggregates);
        selectAggrCmd->SetFeatureClassName(GetEmployeesClassname());
        selectAggrCmd->SetDistinct(true);
        FdoPtr<FdoIdentifierCollection> ids = selectAggrCmd->GetPropertyNames();
        ids->Clear();
        FdoPtr<FdoIdentifier> id = FdoIdentifier::Create(L"JOBTITLE");
        ids->Add(id);
        FdoPtr<FdoIDataReader> dataReader = selectAggrCmd->Execute();

        // Iterate results:
        long lRowCount = 0;
        bool bFoundBoxFiller = false;
        bool bFoundBoxFlattener = false;
        bool bFoundBoxSorter = false;
        bool bFoundBoxAssembler = false;
        bool bFoundBoxArtist = false;
        while (dataReader->ReadNext())
        {
            FdoString *jobTitle = dataReader->GetString(L"JOBTITLE");
            if (0==wcscmp(jobTitle, L"Box Filler"))
                bFoundBoxFiller = true;
            else if (0==wcscmp(jobTitle, L"Box Flattener"))
                bFoundBoxFlattener = true;
            else if (0==wcscmp(jobTitle, L"Box Sorter"))
                bFoundBoxSorter = true;
            else if (0==wcscmp(jobTitle, L"Box Assembler"))
                bFoundBoxAssembler = true;
            else if (0==wcscmp(jobTitle, L"Box Artist"))
                bFoundBoxArtist = true;
            lRowCount++;
        }

        // Check results:
        CPPUNIT_ASSERT_MESSAGE("Expected 5 rows of data, got different", lRowCount==5);
        CPPUNIT_ASSERT_MESSAGE("Expected to find 'Box Filler' but didn't", bFoundBoxFiller);
        CPPUNIT_ASSERT_MESSAGE("Expected to find 'Box Flattener' but didn't", bFoundBoxFlattener);
        CPPUNIT_ASSERT_MESSAGE("Expected to find 'Box Sorter' but didn't", bFoundBoxSorter);
        CPPUNIT_ASSERT_MESSAGE("Expected to find 'Box Assembler' but didn't", bFoundBoxAssembler);
        CPPUNIT_ASSERT_MESSAGE("Expected to find 'Box Artist' but didn't", bFoundBoxArtist);

        // Now let's try the query without specifying any properties.  This tests the no-property
        // case, as well as multiple properties, with distinctness.
        selectAggrCmd = (FdoISelectAggregates*)mConnection->CreateCommand(FdoCommandType_SelectAggregates);
        selectAggrCmd->SetFeatureClassName(GetEmployeesClassname());
        selectAggrCmd->SetDistinct(true);
        dataReader = selectAggrCmd->Execute();

        // Iterate results:
        lRowCount = 0;
        bFoundBoxFiller = false;
        bFoundBoxFlattener = false;
        bFoundBoxSorter = false;
        bFoundBoxAssembler = false;
        bFoundBoxArtist = false;
        while (dataReader->ReadNext())
        {
            FdoString *jobTitle = dataReader->GetString(L"JOBTITLE");
            if (0==wcscmp(jobTitle, L"Box Filler"))
                bFoundBoxFiller = true;
            else if (0==wcscmp(jobTitle, L"Box Flattener"))
                bFoundBoxFlattener = true;
            else if (0==wcscmp(jobTitle, L"Box Sorter"))
                bFoundBoxSorter = true;
            else if (0==wcscmp(jobTitle, L"Box Assembler"))
                bFoundBoxAssembler = true;
            else if (0==wcscmp(jobTitle, L"Box Artist"))
                bFoundBoxArtist = true;
            lRowCount++;
        }

        // Check results:
        CPPUNIT_ASSERT_MESSAGE("Expected 7 rows of data, got different", lRowCount==7);
        CPPUNIT_ASSERT_MESSAGE("Expected to find 'Box Filler' but didn't", bFoundBoxFiller);
        CPPUNIT_ASSERT_MESSAGE("Expected to find 'Box Flattener' but didn't", bFoundBoxFlattener);
        CPPUNIT_ASSERT_MESSAGE("Expected to find 'Box Sorter' but didn't", bFoundBoxSorter);
        CPPUNIT_ASSERT_MESSAGE("Expected to find 'Box Assembler' but didn't", bFoundBoxAssembler);
        CPPUNIT_ASSERT_MESSAGE("Expected to find 'Box Artist' but didn't", bFoundBoxArtist);
    }
    catch (FdoException *e)
    {
        UnitTestUtil::fail (e);
    }
#endif
}


// A test that is hard-coded for a known table.
void OdbcAccessFdoAdvancedSelectTest::orderByTest()
{
#if 0
    // Test not enabled yet.  Try after fixing defect 673849.01.
    try
    {
        FdoPtr<FdoISelectAggregates> selectAggrCmd = (FdoISelectAggregates*)mConnection->CreateCommand(FdoCommandType_SelectAggregates);
        selectAggrCmd->SetFeatureClassName(GetEmployeesClassname());

        // Setup ordering options, including 2 columns:
        selectAggrCmd->SetOrderingOption(FdoOrderingOption_Descending);
        FdoPtr<FdoIdentifierCollection> orderingIds = selectAggrCmd->GetOrdering();
        orderingIds->Clear();
        FdoPtr<FdoIdentifier> id = FdoIdentifier::Create(L"SALARY");
        orderingIds->Add(id);
        id = FdoIdentifier::Create(L"JOBTITLE");
        orderingIds->Add(id);

        // Execute command:
        FdoPtr<FdoIDataReader> dataReader = selectAggrCmd->Execute();

        // Iterate results:
        long lRowCount = 0;
        double dPreviousSalary = 0.0;
        FdoStringP sPreviousJobTitle;
        while (dataReader->ReadNext())
        {
            double dSalary = dataReader->GetDouble(L"SALARY");
            FdoStringP sJobTitle = dataReader->GetString(L"JOBTITLE");

            if (lRowCount>0)
            {
                CPPUNIT_ASSERT_MESSAGE("Wrong order for salary", dPreviousSalary>=dSalary);
                if (dPreviousSalary==dSalary)
                    CPPUNIT_ASSERT_MESSAGE("Wrong order for name", 0<wcscmp(sPreviousJobTitle,sJobTitle));
            }

            dPreviousSalary = dSalary;
            sPreviousJobTitle = sJobTitle;
            lRowCount++;
        }

        // Check results:
        CPPUNIT_ASSERT_MESSAGE("Expected 7 rows of data, got different", lRowCount==7);
    }
    catch (FdoException *e)
    {
        UnitTestUtil::fail (e);
    }
#endif
}


// A test that is hard-coded for a known table.
void OdbcAccessFdoAdvancedSelectTest::groupByTest()
{
#if 0
    // Test not enabled yet.  Try after fixing defect 673849.01.
    try
    {
        FdoPtr<FdoISelectAggregates> selectAggrCmd = (FdoISelectAggregates*)mConnection->CreateCommand(FdoCommandType_SelectAggregates);
        selectAggrCmd->SetFeatureClassName(GetEmployeesClassname());

        // Add ids to select:
        FdoPtr<FdoIdentifierCollection> ids = selectAggrCmd->GetPropertyNames();
        ids->Clear();
        FdoPtr<FdoIdentifier> id = FdoIdentifier::Create(L"JOBTITLE");
        ids->Add(id);

        // Add groupby info:
        FdoPtr<FdoIdentifierCollection> groupingIds = selectAggrCmd->GetGrouping();
        groupingIds->Clear();
        FdoPtr<FdoIdentifier> groupingId = FdoIdentifier::Create(L"JOBTITLE");
        groupingIds->Add(groupingId);

        // Add grouping filter:
        FdoPtr<FdoFilter> groupingFilter = FdoFilter::Parse(L"AVG(SALARY) >= 40000.00");
        selectAggrCmd->SetGroupingFilter(groupingFilter);

        // Execute the command:
        FdoPtr<FdoIDataReader> dataReader = selectAggrCmd->Execute();

        // Iterate results:
        long lRowCount=0;
        bool bFoundBoxAssembler = false;
        bool bFoundBoxFlattener = false;
        bool bFoundBoxArtist = false;
        while (dataReader->ReadNext())
        {
            FdoString *jobTitle = dataReader->GetString(L"JOBTITLE");

            if (0==wcscmp(jobTitle, L"Box Flattener"))
                bFoundBoxFlattener = true;
            else if (0==wcscmp(jobTitle, L"Box Assembler"))
                bFoundBoxAssembler = true;
            else if (0==wcscmp(jobTitle, L"Box Artist"))
                bFoundBoxArtist = true;

            lRowCount++;
        }

        // Validate the results:
        CPPUNIT_ASSERT_MESSAGE("Expected 3 rows, got differently", lRowCount==3);
        CPPUNIT_ASSERT_MESSAGE("Expected to find 'Box Flattener' but didn't", bFoundBoxFlattener);
        CPPUNIT_ASSERT_MESSAGE("Expected to find 'Box Assembler' but didn't", bFoundBoxAssembler);
        CPPUNIT_ASSERT_MESSAGE("Expected to find 'Box Artist' but didn't", bFoundBoxArtist);
    }
    catch (FdoException *e)
    {
        UnitTestUtil::fail (e);
    }
#endif
}


// A test that is hard-coded for a known table.
void OdbcAccessFdoAdvancedSelectTest::TestSelectExpressions()
{
    try
    {
        FdoPtr<FdoISelectAggregates> selectAggrCmd = (FdoISelectAggregates*)mConnection->CreateCommand(FdoCommandType_SelectAggregates);
        selectAggrCmd->SetFeatureClassName(GetEmployeesClassname());

        // Populate the select list:
        FdoPtr<FdoIdentifierCollection> ids = selectAggrCmd->GetPropertyNames();
        ids->Clear();
        FdoPtr<FdoExpressionCollection> arguments = FdoExpressionCollection::Create();
        FdoPtr<FdoExpression> argument = FdoIdentifier::Create(L"SALARY");
        arguments->Add(argument);
        FdoPtr<FdoExpression> expr = FdoFunction::Create(L"AVG", arguments);
        FdoPtr<FdoIdentifier> id = FdoComputedIdentifier::Create(L"MyAvg", expr);
        ids->Add(id);

        // Execute the command:
        FdoPtr<FdoIDataReader> dataReader = selectAggrCmd->Execute();
        
        // Iterate results:
        long lRowCount=0;
        double dAvgSalary = 0.0;
        while (dataReader->ReadNext())
        {
            dAvgSalary = dataReader->GetDouble(L"MyAvg");
            UnitTestUtil::ProcessData(dataReader);

            lRowCount++;
        }

        // Validate the results:
        CPPUNIT_ASSERT_MESSAGE("Expected 1 row, got differently", lRowCount==1);
        CPPUNIT_ASSERT_MESSAGE("Expected a different average salary", UnitTestUtil::fuzzyEqual(dAvgSalary, 39428.57142857142));
    }
    catch (FdoException *e)
    {
        UnitTestUtil::fail (e);
    }
}


// A test that is hard-coded for a known table.
void OdbcAccessFdoAdvancedSelectTest::TestCount()
{
    try
    {
        FdoPtr<FdoISelectAggregates> selectAggrCmd = (FdoISelectAggregates*)mConnection->CreateCommand(FdoCommandType_SelectAggregates);
        selectAggrCmd->SetFeatureClassName(GetEmployeesClassname());

        // Populate the select list:
        FdoPtr<FdoIdentifierCollection> ids = selectAggrCmd->GetPropertyNames();
        ids->Clear();
        FdoPtr<FdoExpressionCollection> arguments = FdoExpressionCollection::Create();
        FdoPtr<FdoExpression> argument = FdoIdentifier::Create(L"SALARY");
        arguments->Add(argument);
        FdoPtr<FdoExpression> expr = FdoFunction::Create(L"COUNT", arguments);
        FdoPtr<FdoIdentifier> id = FdoComputedIdentifier::Create(L"MyCount", expr);
        ids->Add(id);

        // Execute the command:
        FdoPtr<FdoIDataReader> dataReader = selectAggrCmd->Execute();
        
        FdoDataType propDataType = dataReader->GetDataType(L"MyCount");

        // Iterate results:
        long lRowCount = 0;
        long lMyCount = 0;
        double dMyCount = 0.0;
        while (dataReader->ReadNext())
        {
            // Some underlying databases map "SALARY" to integer, some to floating point.
            if (FdoDataType_Int32 == propDataType)
            {
                lMyCount = dataReader->GetInt32(L"MyCount");
            }
            else
            {
                dMyCount = dataReader->GetDouble(L"MyCount");
            }

            lRowCount++;
        }

        // Validate the results:
        CPPUNIT_ASSERT_MESSAGE("Expected 1 row, got differently", lRowCount==1);
        CPPUNIT_ASSERT_MESSAGE("Expected a different average salary",
            (FdoDataType_Int32 == propDataType) ? lMyCount==7 : dMyCount==7.0);
    }
    catch (FdoException *e)
    {
        UnitTestUtil::fail (e);
    }
}




// A test that is hard-coded for a known table.
void OdbcAccessFdoAdvancedSelectTest::TestUpperLower()
{
#if 0
    // Test not enabled yet.  Try after fixing defect 673849.01.
    try
    {
        FdoPtr<FdoISelectAggregates> selectAggrCmd = (FdoISelectAggregates*)mConnection->CreateCommand(FdoCommandType_SelectAggregates);
        selectAggrCmd->SetFeatureClassName(L"EMPLOYEES");

        FdoPtr<FdoIdentifierCollection> ids = selectAggrCmd->GetPropertyNames();
        ids->Clear();
        FdoPtr<FdoIdentifier> id = FdoIdentifier::Create(L"JOBTITLE");
        ids->Add(id);

        FdoPtr<FdoExpressionCollection> arguments = FdoExpressionCollection::Create();
        FdoPtr<FdoExpression> argument = FdoIdentifier::Create(L"JOBTITLE");
        arguments->Add(argument);
        FdoPtr<FdoExpression> expr = FdoFunction::Create(L"Lower", arguments);
        FdoPtr<FdoIdentifier> idComputed = FdoComputedIdentifier::Create(L"JOBTITLE_LOWER", expr);
        ids->Add(idComputed);

        arguments = FdoExpressionCollection::Create();
        argument = FdoIdentifier::Create(L"JOBTITLE");
        arguments->Add(argument);
        expr = FdoFunction::Create(L"Upper", arguments);
        idComputed = FdoComputedIdentifier::Create(L"JOBTITLE_UPPER", expr);
        ids->Add(idComputed);

        FdoPtr<FdoIDataReader> dataReader = selectAggrCmd->Execute();
        FdoString* jobtitleFetched;
        while (dataReader->ReadNext())
        {
            jobtitleFetched = dataReader->GetString(L"JOBTITLE_LOWER");
            for (unsigned int i=0; i<wcslen(jobtitleFetched); i++)
                CPPUNIT_ASSERT_MESSAGE("'Lower' function didn't give lowercase results", !iswupper(jobtitleFetched[i]));

            jobtitleFetched = dataReader->GetString(L"JOBTITLE_UPPER");
            for (unsigned int i=0; i<wcslen(jobtitleFetched); i++)
                CPPUNIT_ASSERT_MESSAGE("'Upper' function didn't give uppercase results", !iswlower(jobtitleFetched[i]));
        }
    }
    catch (FdoException *e)
    {
        UnitTestUtil::fail (e);
    }
#endif
}


// A test that is hard-coded for a known table.
void OdbcAccessFdoAdvancedSelectTest::TestConcat()
{
#if 0
    // Test not enabled yet.  Try after fixing defect 673849.01.
    try
    {
        FdoPtr<FdoISelectAggregates> selectAggrCmd = (FdoISelectAggregates*)mConnection->CreateCommand(FdoCommandType_SelectAggregates);
        selectAggrCmd->SetFeatureClassName(L"EMPLOYEES");

        FdoPtr<FdoIdentifierCollection> ids = selectAggrCmd->GetPropertyNames();
        ids->Clear();

        FdoPtr<FdoIdentifier> id = FdoIdentifier::Create(L"JOBTITLE");
        ids->Add(id);

        FdoPtr<FdoExpressionCollection> arguments = FdoExpressionCollection::Create();
        FdoPtr<FdoExpression> argument = FdoIdentifier::Create(L"JOBTITLE");
        arguments->Add(argument);
        argument = FdoStringValue::Create(L"_SUFFIX");
        arguments->Add(argument);
        FdoPtr<FdoExpression> expr = FdoFunction::Create(L"Concat", arguments);
        FdoPtr<FdoIdentifier> idComputed = FdoComputedIdentifier::Create(L"JOBTITLE_CONCAT", expr);
        ids->Add(idComputed);

        FdoPtr<FdoIDataReader> dataReader = selectAggrCmd->Execute();
        FdoString* jobtitle;
        FdoString* jobtitleConcat;
        while (dataReader->ReadNext())
        {
            jobtitle = dataReader->GetString(L"JOBTITLE");
            jobtitleConcat = dataReader->GetString(L"JOBTITLE_CONCAT");

            // Determine what the concatenated job title SHOULD look like:
            FdoStringP jobTitleConcatTest;
            jobTitleConcatTest = jobtitle;
            jobTitleConcatTest += L"_SUFFIX";

            CPPUNIT_ASSERT_MESSAGE("'Concat' function didn't give concatenated results",
                0==wcscmp((const wchar_t*)jobTitleConcatTest, jobtitleConcat));
        }
    }
    catch (FdoException *e)
    {
        UnitTestUtil::fail (e);
    }
#endif
}

#endif
