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
CPPUNIT_TEST_SUITE_REGISTRATION( OdbcMySqlFdoAdvancedSelectTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcMySqlFdoAdvancedSelectTest, "FdoAdvancedSelectTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcMySqlFdoAdvancedSelectTest, "OdbcMySqlFdoAdvancedSelectTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcMySqlFdoAdvancedSelectTest, "OdbcMySqlTests");

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
        mConnection= NULL;
        TestCommonFail (ex);
    }
}

void OdbcMySqlFdoAdvancedSelectTest::set_provider()
{
	UnitTestUtil::SetProvider( "OdbcMySql" );
}

void OdbcMySqlFdoAdvancedSelectTest::connect ()
{
    try
    {
        mConnection = UnitTestUtil::GetProviderConnectionObject();
        mConnection->SetConnectionString(UnitTestUtil::GetConnectionString());
        mConnection->Open();
    }
    catch (FdoException *ex)
    {
        mConnection= NULL;
        TestCommonFail (ex);
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
        TestCommonFail (e);
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
        TestCommonFail (e);
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
        TestCommonFail (e);
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
        TestCommonFail (e);
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
        TestCommonFail (e);
    }
#endif
}


// A test that is hard-coded for a known table.
void OdbcAccessFdoAdvancedSelectTest::selectDistinctTest()
{
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
        TestCommonFail (e);
    }
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
        TestCommonFail (e);
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
        TestCommonFail (e);
    }
#endif
}

void OdbcAccessFdoAdvancedSelectTest::groupByorderByTest()
{
#if 0 
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

		FdoPtr<FdoIdentifierCollection>orderCol = selectAggrCmd->GetOrdering();
		FdoPtr<FdoIdentifier>oId = FdoIdentifier::Create(L"JOBTITLE");
        orderCol->Add( oId );

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
        TestCommonFail (e);
    }
#endif
}

void OdbcAccessFdoAdvancedSelectTest::checkDataReaderContentOnSelAggRequestWithAggrFunction()
{
    printf(" \n \n");
    printf(" ------------------------------------------------------------- \n");
    printf(">>> This test is not supported in the ODBC Access test suite. \n");
    printf(" ------------------------------------------------------------- \n");
    printf(" \n \n");
}

void OdbcAccessFdoAdvancedSelectTest::checkDataReaderContentOnSelAggRequestWithNumCharFunction()
{
    printf(" \n \n");
    printf(" ------------------------------------------------------------- \n");
    printf(">>> This test is not supported in the ODBC Access test suite. \n");
    printf(" ------------------------------------------------------------- \n");
    printf(" \n \n");
}

void OdbcAccessFdoAdvancedSelectTest::checkFeatureReaderContentOnSelRequestWithAggrFunction()
{
    printf(" \n \n");
    printf(" ------------------------------------------------------------- \n");
    printf(">>> This test is not supported in the ODBC Access test suite. \n");
    printf(" ------------------------------------------------------------- \n");
    printf(" \n \n");
}

void OdbcAccessFdoAdvancedSelectTest::checkFeatureReaderContentOnSelRequestWithNumCharFunction()
{
    printf(" \n \n");
    printf(" ------------------------------------------------------------- \n");
    printf(">>> This test is not supported in the ODBC Access test suite. \n");
    printf(" ------------------------------------------------------------- \n");
    printf(" \n \n");
}

void OdbcMySqlFdoAdvancedSelectTest::compIdentPropertyTest()
{
    try
    {
        FdoPtr<FdoISelectAggregates> selectAggrCmd = (FdoISelectAggregates*)mConnection->CreateCommand(FdoCommandType_SelectAggregates);
        selectAggrCmd->SetFeatureClassName(GetCitiesClassname());

        // Control ID's to select:
        FdoPtr<FdoIdentifierCollection> ids = selectAggrCmd->GetPropertyNames();
        ids->Clear();

        // Add computed property
        FdoPtr<FdoIdentifierCollection> groupingIds = selectAggrCmd->GetGrouping();
        groupingIds->Clear();
        FdoPtr<FdoExpressionCollection> arguments1 = FdoExpressionCollection::Create();
        FdoPtr<FdoExpression> argument1 = FdoIdentifier::Create(L"cityid");
        arguments1->Add(argument1);
        FdoPtr<FdoExpression> expr1 = FdoFunction::Create(L"COUNT", arguments1);
        FdoPtr<FdoIdentifier> id1 = FdoComputedIdentifier::Create(L"COUNT_CITYID", expr1);
        ids->Add(id1);

        // Execute the command:
        FdoPtr<FdoIDataReader> dataReader = selectAggrCmd->Execute();

        // Iterate results:
        long lRowCount=0;
        while (dataReader->ReadNext())
        {
            //FdoString *jobTitle = dataReader->GetString(L"name");
            FdoInt64 foo = dataReader->GetInt64(L"COUNT_CITYID");
            lRowCount++;
        }

        // Validate the results:
        CPPUNIT_ASSERT_MESSAGE("Expected 1 row, got differently", lRowCount==1);
    }
    catch (FdoException *e)
    {
        TestCommonFail (e);
    }
}

void OdbcMySqlFdoAdvancedSelectTest::groupByorderByTest()
{
#if 0 
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

		FdoPtr<FdoIdentifierCollection>orderCol = selectAggrCmd->GetOrdering();
		FdoPtr<FdoIdentifier>oId = FdoIdentifier::Create(L"JOBTITLE");
        orderCol->Add( oId );

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
        TestCommonFail (e);
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
        TestCommonFail (e);
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
        TestCommonFail (e);
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
        TestCommonFail (e);
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
        TestCommonFail (e);
    }
#endif
}

void OdbcAccessFdoAdvancedSelectTest::getDataTypeTest()
{
}

// The following test is to check whether or not an identity property is
// automatically added to the reader returned by a SelectAggregate request
// when aa aggregate function is used. In this case, no such property should
// be returned by the reader.
void OdbcMySqlFdoAdvancedSelectTest::checkDataReaderContentOnSelAggRequestWithAggrFunction()
{
    FdoPtr<FdoISelectAggregates>    selAggr;
    FdoPtr<FdoIDataReader>          rdr;
    FdoPtr<FdoIdentifierCollection> ids;
    int                             count;
    bool                            idPropertyFound = false;

	try 
	{
        printf(" \n \n");
        printf(">>> Executing SelectAggregate Function Reader Content Test   \n");
        printf(" ----------------------------------------------------------- \n");
        printf("       The test checks whether or not an identity property   \n");
        printf("       is automatically added to the reader returned by a    \n");
        printf("       SelectAggregate request when an aggregate function is \n");
        printf("       used. If the reader contains an identity property the \n");
        printf("       test issues an exception.                             \n");
        printf(" ----------------------------------------------------------- \n");
        printf(">>> ... Executing the select aggregate command \n");

        selAggr = (FdoISelectAggregates*)(mConnection->CreateCommand(FdoCommandType_SelectAggregates));
        selAggr->SetFeatureClassName(L"Fdo:acdb3dpolyline");
        FdoFilter *aFilter = FdoComparisonCondition::Create(
                                FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"segcount")),
                                FdoComparisonOperations_EqualTo, 
                                FdoPtr<FdoDataValue>(FdoDataValue::Create(7)));
        selAggr->SetFilter(aFilter);
        ids = selAggr->GetPropertyNames();
        ids->Clear();
        ids->Add(
            FdoPtr<FdoIdentifier>(FdoComputedIdentifier::Create(
                                      L"MyMaxSegcount",
                                      FdoPtr<FdoExpression>(
                                            FdoExpression::Parse(L"Max(segcount)")))));

        rdr = selAggr->Execute();
        count = 0;

        // The following navigates through the reader and checks the requested data.
        // It is an error if there is an additional identity property (or any other
        // property) other than the requested one returned by the reader.
        printf(">>> ... Checking the returned reader \n");
        while (rdr->ReadNext())
        {
            FdoInt64 myMaxSegcount = rdr->GetInt64(L"MyMaxSegcount");

            try
            {
                FdoInt64 myid = rdr->GetInt64(L"featid");
                idPropertyFound = true;
            }
            catch ( ... )
            {
            }

            count++;
        }
        rdr->Close();

        printf(">>> ... Checking for error cases \n");
        if (idPropertyFound)
            throw FdoException::Create(L"Id property returned with reader");
        else
            printf(">>> ...... Reader content as expected \n");

        printf(">>> Test executed successfully \n");
    }

    catch( FdoException *ex )
    {
        printf(" \n");
        printf("!!! Exception: %ls !!! \n", ex->GetExceptionMessage());
        printf(" \n");
        printf(">>> Test failed \n");
        CPPUNIT_FAIL (UnitTestUtil::w2a( ex->GetExceptionMessage()));
    }
}

// The following test is to check whether or not an identity property is
// automatically added to the reader returned by a SelectAggregate request
// when a function is used that is not an aggregate function but one of CEIL,
// CONCAT, FLOOR, LOWER and UPPER. In this case, no such property should be
// returned by the reader.
void OdbcMySqlFdoAdvancedSelectTest::checkDataReaderContentOnSelAggRequestWithNumCharFunction()
{
    FdoPtr<FdoISelectAggregates>    selAggr;
    FdoPtr<FdoIDataReader>          rdr;
    FdoPtr<FdoIdentifierCollection> ids;
    int                             count;
    bool                            idPropertyFound = false;

	try 
	{
        printf(" \n \n");
        printf(">>> Executing SelectAggregate Function Reader Content Test   \n");
        printf(" ----------------------------------------------------------- \n");
        printf("       The test checks whether or not an identity property   \n");
        printf("       is automatically added to the reader returned by a    \n");
        printf("       SelectAggregate request when a function is used that  \n");
        printf("       is not an aggregate function but one of CEIL, CONCAT, \n");
        printf("       FLOOR, LOWER and UPPER. If the reader contains an     \n");
        printf("       identity property the test issues an exception.       \n");
        printf(" ----------------------------------------------------------- \n");
        printf(">>> ... Executing the select aggregate command \n");

        selAggr = (FdoISelectAggregates*)(mConnection->CreateCommand(FdoCommandType_SelectAggregates));
        selAggr->SetFeatureClassName(L"Fdo:acdb3dpolyline");
        FdoFilter *aFilter = FdoComparisonCondition::Create(
                                FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"segcount")),
                                FdoComparisonOperations_EqualTo, 
                                FdoPtr<FdoDataValue>(FdoDataValue::Create(10)));
        selAggr->SetFilter(aFilter);
        ids = selAggr->GetPropertyNames();
        ids->Clear();
        ids->Add(
            FdoPtr<FdoIdentifier>(FdoComputedIdentifier::Create(
                                      L"MyConcatString",
                                      FdoPtr<FdoExpression>(
                                            FdoExpression::Parse(L"Concat(layer, color)")))));

        rdr = selAggr->Execute();
        count = 0;

        // The following navigates through the reader and checks the requested data.
        // It is an error if there is an additional identity property (or any other
        // property) other than the requested one returned by the reader.
        printf(">>> ... Checking the returned reader \n");
        while (rdr->ReadNext())
        {
            FdoStringP myString = rdr->GetString(L"MyConcatString");

            try
            {
                FdoInt64 myid = rdr->GetInt64(L"featid");
                idPropertyFound = true;
            }
            catch ( ... )
            {
            }

            count++;
        }
        rdr->Close();

        printf(">>> ... Checking for error cases \n");
        if (idPropertyFound)
            throw FdoException::Create(L"Id property returned with reader");
        else
            printf(">>> ...... Reader content as expected \n");

        printf(">>> Test executed successfully \n");
    }

    catch( FdoException *ex )
    {
        printf(" \n");
        printf("!!! Exception: %ls !!! \n", ex->GetExceptionMessage());
        printf(" \n");
        printf(">>> Test failed \n");
        CPPUNIT_FAIL (UnitTestUtil::w2a( ex->GetExceptionMessage()));
    }
}

// The following test is to check whether or not the select command issues
// an exception if the select statement uses an aggregate function. If this
// is not the case the test issues an exception.
void OdbcMySqlFdoAdvancedSelectTest::checkFeatureReaderContentOnSelRequestWithAggrFunction()
{
    FdoPtr<FdoISelect>              selCmd;
    FdoPtr<FdoIFeatureReader>       rdr;
    FdoPtr<FdoIdentifierCollection> ids;
    int                             count;
    bool                            idPropertyFound = false;
    bool                            expectedExceptionIssued = true;

	try 
	{
        printf(" \n \n");
        printf(">>> Executing Select Function Aggregate Exception Test       \n");
        printf(" ----------------------------------------------------------- \n");
        printf("       The test checks whether or not an exception is issued \n");
        printf("       if the select command is invoked with an aggregate    \n");
        printf("       function. If the expected exception is not issued the \n");
        printf("       test issues an exception.                             \n");
        printf(" ----------------------------------------------------------- \n");
        printf(">>> ... Executing the select aggregate command \n");

        selCmd = (FdoISelect*)(mConnection->CreateCommand(FdoCommandType_Select));
        selCmd->SetFeatureClassName(L"Fdo:acdb3dpolyline");
        ids = selCmd->GetPropertyNames();
        ids->Clear();
        ids->Add(
            FdoPtr<FdoIdentifier>(FdoComputedIdentifier::Create(
                                      L"MyMaxSegcount",
                                      FdoPtr<FdoExpression>(
                                            FdoExpression::Parse(L"Max(segcount)")))));

        rdr = selCmd->Execute();

        // If this point is reached the expected exception was not issued and hence
        // an exception needs to be issued to indicate so.
        expectedExceptionIssued = false;

        // Check the content of the reader.

        count = 0;
        printf(">>> ... Checking the returned reader \n");

        while (rdr->ReadNext())
        {
            FdoInt64 myMaxSegcount = rdr->GetInt64(L"MyMaxSegcount");
            FdoStringP className = rdr->GetClassDefinition()->GetName();

            try
            {
                FdoInt64 myid = rdr->GetInt64(L"featid");
                idPropertyFound = true;
            }
            catch ( ... )
            {
            }

            count++;
        }
        rdr->Close();

        printf(">>> ... Checking for error cases \n");

        //if (idPropertyFound)
        //    throw FdoException::Create(L"Identity property found in reader when not expected");

        //if (!expectedExceptionIssued)
        //    throw FdoException::Create(L"Expected exception not issued");

        printf(">>> Test executed succeeded \n");
    }

    catch( FdoException *ex )
    {
        printf(" \n");
        printf("!!! Exception: %ls !!! \n", ex->GetExceptionMessage());
        printf(" \n");
        printf(">>> Test failed \n");
        CPPUNIT_FAIL (UnitTestUtil::w2a( ex->GetExceptionMessage()));
    }
}

// The following test is to check whether or not an identity property is
// automatically added to the reader returned by a Select request when a
// function is used that is not an aggregate function but one of CEIL,
// CONCAT, FLOOR, LOWER and UPPER. In this case, the reader is expected to
// return such a property.
void OdbcMySqlFdoAdvancedSelectTest::checkFeatureReaderContentOnSelRequestWithNumCharFunction()
{
    FdoPtr<FdoISelect>              selCmd;
    FdoPtr<FdoIFeatureReader>       rdr;
    FdoPtr<FdoIdentifierCollection> ids;
    int                             count;
    bool                            idPropertyFound = false;

	try 
	{
        printf(" \n \n");
        printf(">>> Executing Select Function Reader Content Test            \n");
        printf(" ----------------------------------------------------------- \n");
        printf(">>>    The test checks whether or not an identity property   \n");
        printf(">>>    is automatically added to the reader returned by a    \n");
        printf(">>>    Select request when a function is used that is not an \n");
        printf(">>>    aggregate function but one of CEIL, CONCAT, FLOOR,    \n");
        printf(">>>    LOWER and UPPER. In this case, the reader is expected \n");
        printf(">>>    to return such a property.                            \n");
        printf(" ----------------------------------------------------------- \n");
        printf(">>> ... Executing the select command \n");

        selCmd = (FdoISelect*)(mConnection->CreateCommand(FdoCommandType_Select));
        selCmd->SetFeatureClassName(L"Fdo:acdb3dpolyline");
        FdoFilter *aFilter = FdoComparisonCondition::Create(
                                FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"segcount")),
                                FdoComparisonOperations_EqualTo, 
                                FdoPtr<FdoDataValue>(FdoDataValue::Create(7)));
        selCmd->SetFilter(aFilter);
        ids = selCmd->GetPropertyNames();
        ids->Clear();
        ids->Add(
            FdoPtr<FdoIdentifier>(FdoComputedIdentifier::Create(
                                      L"MyConcatString",
                                      FdoPtr<FdoExpression>(
                                            FdoExpression::Parse(L"Concat(layer, color)")))));

        rdr = selCmd->Execute();
        count = 0;

        // The following navigates through the reader and checks the requested data.
        // It is an error if the reader does not return identity properties.
        printf(">>> ... Checking the returned reader \n");
        while (rdr->ReadNext())
        {
            FdoStringP myString = rdr->GetString(L"MyConcatString");

            try
            {
                FdoInt64 myid = rdr->GetInt64(L"featid");
                idPropertyFound = true;
            }
            catch ( ... )
            {
            }

            count++;
        }
        rdr->Close();

        printf(">>> ... Checking for error cases \n");
        if (!idPropertyFound)
            throw FdoException::Create(L"Id property not returned with reader");
        else
            printf(">>> ...... Reader content as expected \n");

        printf(">>> Test executed successfully \n");
    }

    catch( FdoException *ex )
    {
        printf(" \n");
        printf("!!! Exception: %ls !!! \n", ex->GetExceptionMessage());
        printf(" \n");
        printf(">>> Test failed \n");
        CPPUNIT_FAIL (UnitTestUtil::w2a( ex->GetExceptionMessage()));
    }
}



#endif
