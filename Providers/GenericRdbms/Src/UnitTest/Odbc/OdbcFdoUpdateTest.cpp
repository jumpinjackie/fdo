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
#include "OdbcFdoUpdateTest.h"
#include "UnitTestUtil.h"
#include "OdbcBaseSetup.h"

CPPUNIT_TEST_SUITE_REGISTRATION( OdbcOracleFdoUpdateTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcOracleFdoUpdateTest, "FdoUpdateTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcOracleFdoUpdateTest, "OdbcOracleFdoUpdateTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcOracleFdoUpdateTest, "OdbcOracleTests");

CPPUNIT_TEST_SUITE_REGISTRATION( OdbcMySqlFdoUpdateTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcMySqlFdoUpdateTest, "FdoUpdateTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcMySqlFdoUpdateTest, "OdbcMySqlFdoUpdateTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcMySqlFdoUpdateTest, "OdbcMySqlTests");

#ifdef _WIN32
CPPUNIT_TEST_SUITE_REGISTRATION( OdbcSqlServerFdoUpdateTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcSqlServerFdoUpdateTest, "FdoUpdateTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcSqlServerFdoUpdateTest, "OdbcSqlServerFdoUpdateTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcSqlServerFdoUpdateTest, "OdbcSqlServerTests");

CPPUNIT_TEST_SUITE_REGISTRATION( OdbcAccessFdoUpdateTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcAccessFdoUpdateTest, "FdoUpdateTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcAccessFdoUpdateTest, "OdbcAccessFdoUpdateTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcAccessFdoUpdateTest, "OdbcAccessTests");

CPPUNIT_TEST_SUITE_REGISTRATION( OdbcDbaseFdoUpdateTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcDbaseFdoUpdateTest, "FdoUpdateTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcDbaseFdoUpdateTest, "OdbcDbaseFdoUpdateTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcDbaseFdoUpdateTest, "OdbcDbaseTests");

CPPUNIT_TEST_SUITE_REGISTRATION( OdbcExcelFdoUpdateTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcExcelFdoUpdateTest, "FdoUpdateTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcExcelFdoUpdateTest, "OdbcExcelFdoUpdateTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcExcelFdoUpdateTest, "OdbcExcelTests");
#endif

void OdbcBaseFdoUpdateTest::setUp ()
{
    set_provider();
    connect();
}

void OdbcBaseFdoUpdateTest::tearDown ()
{
    if (mConnection != NULL)
    {
        mConnection->Close();
        mConnection = NULL;
    }
}

void OdbcBaseFdoUpdateTest::connect ()
{
    try
    {
		mConnection = UnitTestUtil::GetProviderConnectionObject();
        if (DataBaseType_None != mSetup.GetTypeDB() )
        {
            // Set up databases that are not prefabricated.
            StringConnTypeRequest connectionType = Connection_NoDatastore;
            if (DataBaseType_Oracle == mSetup.GetTypeDB() )
                connectionType = Connection_OraSetup;
		    mConnection->SetConnectionString ( UnitTestUtil::GetConnectionString(connectionType, "") );
		    mConnection->Open();
		    mSetup.CreateDataStore(mConnection, "");
		    mSetup.CreateAcadSchema(mConnection);
		    mSetup.CreateNonAcadSchema(mConnection);

		    mConnection->Close();
        }
		mConnection->SetConnectionString ( UnitTestUtil::GetConnectionString(Connection_WithDSN, "") );
		mConnection->Open();
    }
    catch (FdoException *ex)
    {
        mConnection = NULL;
        UnitTestUtil::fail (ex);
    }

}


void OdbcBaseFdoUpdateTest::updateCities()
{
    if (mConnection != NULL) try
    {
        FdoPtr<FdoIUpdate> updateCommand =
            (FdoIUpdate *) mConnection->CreateCommand(FdoCommandType_Update);
        updateCommand->SetFeatureClassName(mSetup.GetClassNameCities());
        FdoPtr<FdoPropertyValueCollection> propertyValues =
            updateCommand->GetPropertyValues();

        FdoPtr<FdoDataValue> dataValue;
        FdoPtr<FdoPropertyValue> propertyValue;

        dataValue = FdoDataValue::Create(L"Carleton");
        propertyValue = AddNewProperty( propertyValues, mSetup.GetPropertyNameCitiesName());
        propertyValue->SetValue(dataValue);

        dataValue = FdoDataValue::Create(L"FakeVille");
        propertyValue = AddNewProperty( propertyValues, mSetup.GetPropertyNameCitiesCity());
        propertyValue->SetValue(dataValue);

        FdoPtr<FdoFilter> filter = FdoComparisonCondition::Create(
            FdoPtr<FdoIdentifier>(FdoIdentifier::Create(mSetup.GetPropertyNameCitiesName()) ),
            FdoComparisonOperations_EqualTo,
            FdoPtr<FdoDataValue>(FdoDataValue::Create(L"Boop") ) );

        updateCommand->SetFilter(filter);

        int count = updateCommand->Execute();
        CPPUNIT_ASSERT( count == 1 );

        // Now change it back so that other tests pass.
        updateCommand =
            (FdoIUpdate *) mConnection->CreateCommand(FdoCommandType_Update);
        updateCommand->SetFeatureClassName(mSetup.GetClassNameCities());
        propertyValues = updateCommand->GetPropertyValues();

        dataValue = FdoDataValue::Create(L"Boop");
        propertyValue = AddNewProperty( propertyValues, mSetup.GetPropertyNameCitiesName());
        propertyValue->SetValue(dataValue);

        dataValue = FdoDataValue::Create(L"San Bebop");
        propertyValue = AddNewProperty( propertyValues, mSetup.GetPropertyNameCitiesCity());
        propertyValue->SetValue(dataValue);

        filter = FdoComparisonCondition::Create(
            FdoPtr<FdoIdentifier>(FdoIdentifier::Create(mSetup.GetPropertyNameCitiesName()) ),
            FdoComparisonOperations_EqualTo,
            FdoPtr<FdoDataValue>(FdoDataValue::Create(L"Carleton") ) );

        updateCommand->SetFilter(filter);

        count = updateCommand->Execute();
        CPPUNIT_ASSERT( count == 1 );
    }
    catch (FdoException *ex)
    {
        CPPUNIT_FAIL (UnitTestUtil::w2a(ex->GetExceptionMessage()));
    }
}

void OdbcBaseFdoUpdateTest::updateTable1()
{
#if 0
    // Test not converted yet.

    try
    {
        FdoPtr<FdoIConnection> connection = UnitTestUtil::GetProviderConnectionObject();
		connection->SetConnectionString(UnitTestUtil::GetConnectionString(Connection_WithDSN, ""));
        connection->Open();
        try
        {
            UnitTestUtil::CreateTable1(connection);

            FdoIUpdate *updateCommand =
                (FdoIUpdate *) connection->CreateCommand(FdoCommandType_Update);
            updateCommand->SetFeatureClassName(L"TABLE1");
            FdoPtr<FdoPropertyValueCollection> propertyValues =
                updateCommand->GetPropertyValues();

            FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();

            printf("start update feature class\n");
            FdoPtr<FdoDataValue> dataValue;
            FdoPtr<FdoPropertyValue> propertyValue;

            dataValue = FdoDataValue::Create(L"FakeNameUpdate");
            propertyValue = AddNewProperty( propertyValues, L"NAME");
            propertyValue->SetValue(dataValue);

            propertyValue = AddNewProperty( propertyValues, L"Geometry" );
            FdoPtr<FdoIDirectPosition> pos = gf->CreatePositionXY(50, 60);
            FdoPtr<FdoIPoint> pt = gf->CreatePoint(pos);
            FdoPtr<FdoByteArray> byteArray = gf->GetFgf(pt);
            FdoPtr<FdoGeometryValue> geometryValue = FdoGeometryValue::Create(byteArray);
            propertyValue->SetValue(geometryValue);

            FdoPtr<FdoFilter> filter = FdoComparisonCondition::Create(
                FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"NAME") ),
                FdoComparisonOperations_EqualTo,
                FdoPtr<FdoDataValue>(FdoDataValue::Create(L"MyName") ) );

            updateCommand->SetFilter(filter);

             int count = updateCommand->Execute();
            CPPUNIT_ASSERT_MESSAGE ("Update failed to update 1 record", count==1);

            // Now change the values back so that subsequent runs of the test will succeed.
            // Make them not quite the golden values, so that we can view the changes after the test.

            propertyValues->Clear();

            dataValue = FdoDataValue::Create(L"MyName");
            propertyValue = AddNewProperty( propertyValues, L"NAME");
            propertyValue->SetValue(dataValue);

            propertyValue = AddNewProperty( propertyValues, L"Geometry" );
            pos = gf->CreatePositionXY(11, 21);
            pt = gf->CreatePoint(pos);
            byteArray = gf->GetFgf(pt);
            geometryValue = FdoGeometryValue::Create(byteArray);
            propertyValue->SetValue(geometryValue);

            filter = FdoComparisonCondition::Create(
                FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"NAME") ),
                FdoComparisonOperations_EqualTo,
                FdoPtr<FdoDataValue>(FdoDataValue::Create(L"FakeNameUpdate") ) );

            updateCommand->SetFilter(filter);

            count = updateCommand->Execute();
            CPPUNIT_ASSERT_MESSAGE ("Update failed to update 1 record", count==1);

            updateCommand->Release();
        }
        catch (...)
        {
            throw;
        }
    }
    catch (FdoException *ex)
    {
        CPPUNIT_FAIL (UnitTestUtil::w2a(ex->GetExceptionMessage()));
    }
#endif
}

#ifdef _WIN32

void OdbcAccessFdoUpdateTest::updateCities()
{
	clock_t start, finish;
    try
    {
        FdoPtr<FdoIConnection> connection = UnitTestUtil::GetProviderConnectionObject();
		connection->SetConnectionString(UnitTestUtil::GetConnectionString(Connection_WithDSN, ""));
        connection->Open();
        FdoIUpdate *updateCommand =
            (FdoIUpdate *) connection->CreateCommand(FdoCommandType_Update);
        updateCommand->SetFeatureClassName(GetClassNameCities());
        FdoPtr<FdoPropertyValueCollection> propertyValues =
            updateCommand->GetPropertyValues();

		start = clock();
        printf("start update non-feature class\n");
        FdoPtr<FdoDataValue> dataValue;
        FdoPtr<FdoPropertyValue> propertyValue;

        dataValue = FdoDataValue::Create(L"Carleton");
        propertyValue = AddNewProperty( propertyValues, GetPropertyNameCitiesName());
        propertyValue->SetValue(dataValue);

        dataValue = FdoDataValue::Create(L"FakeVille");
        propertyValue = AddNewProperty( propertyValues, GetPropertyNameCitiesCity());
        propertyValue->SetValue(dataValue);

        int count = updateCommand->Execute();
        updateCommand->Release();

		finish = clock();
		printf( "Elapsed: %f seconds\n", ((double)(finish - start) / CLOCKS_PER_SEC) );
    }
    catch (FdoException *ex)
    {
        CPPUNIT_FAIL (UnitTestUtil::w2a(ex->GetExceptionMessage()));
    }
}

void OdbcAccessFdoUpdateTest::updateTable1()
{
	clock_t start, finish;
    try
    {
        FdoPtr<FdoIConnection> connection = UnitTestUtil::GetProviderConnectionObject();
		connection->SetConnectionString(UnitTestUtil::GetConnectionString(Connection_WithDSN, ""));
        connection->Open();
        try
        {
            FdoIUpdate *updateCommand =
                (FdoIUpdate *) connection->CreateCommand(FdoCommandType_Update);
            updateCommand->SetFeatureClassName(L"TABLE1");
            FdoPtr<FdoPropertyValueCollection> propertyValues =
                updateCommand->GetPropertyValues();

            FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();

            start = clock();
            printf("start update feature class\n");
            FdoPtr<FdoDataValue> dataValue;
            FdoPtr<FdoPropertyValue> propertyValue;

            dataValue = FdoDataValue::Create(L"FakeNameUpdate");
            propertyValue = AddNewProperty( propertyValues, L"NAME");
            propertyValue->SetValue(dataValue);

            propertyValue = AddNewProperty( propertyValues, L"Geometry" );
            FdoPtr<FdoIDirectPosition> pos = gf->CreatePositionXY(50, 60);
            FdoPtr<FdoIPoint> pt = gf->CreatePoint(pos);
            FdoPtr<FdoByteArray> byteArray = gf->GetFgf(pt);
            FdoPtr<FdoGeometryValue> geometryValue = FdoGeometryValue::Create(byteArray);
            propertyValue->SetValue(geometryValue);

            FdoPtr<FdoFilter> filter = FdoComparisonCondition::Create(
                FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"NAME") ),
                FdoComparisonOperations_EqualTo,
                FdoPtr<FdoDataValue>(FdoDataValue::Create(L"MyName") ) );

            updateCommand->SetFilter(filter);

             int count = updateCommand->Execute();
            CPPUNIT_ASSERT_MESSAGE ("Update failed to update 1 record", count==1);

            // Now change the values back so that subsequent runs of the test will succeed.
            // Make them not quite the golden values, so that we can view the changes after the test.

            propertyValues->Clear();

            dataValue = FdoDataValue::Create(L"MyName");
            propertyValue = AddNewProperty( propertyValues, L"NAME");
            propertyValue->SetValue(dataValue);

            propertyValue = AddNewProperty( propertyValues, L"Geometry" );
            pos = gf->CreatePositionXY(11, 21);
            pt = gf->CreatePoint(pos);
            byteArray = gf->GetFgf(pt);
            geometryValue = FdoGeometryValue::Create(byteArray);
            propertyValue->SetValue(geometryValue);

            filter = FdoComparisonCondition::Create(
                FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"NAME") ),
                FdoComparisonOperations_EqualTo,
                FdoPtr<FdoDataValue>(FdoDataValue::Create(L"FakeNameUpdate") ) );

            updateCommand->SetFilter(filter);

            count = updateCommand->Execute();
            CPPUNIT_ASSERT_MESSAGE ("Update failed to update 1 record", count==1);

            updateCommand->Release();

			finish = clock();
			printf( "Elapsed: %f seconds\n", ((double)(finish - start) / CLOCKS_PER_SEC) );
        }
        catch (...)
        {
            throw;
        }
    }
    catch (FdoException *ex)
    {
        CPPUNIT_FAIL (UnitTestUtil::w2a(ex->GetExceptionMessage()));
    }
}


void OdbcExcelFdoUpdateTest::updateTable1()
{
	clock_t start, finish;
    try
    {
        FdoPtr<FdoIConnection> connection = UnitTestUtil::GetProviderConnectionObject();
        if (connection == NULL)
            CPPUNIT_FAIL("FAILED - CreateConnection returned NULL\n");
        FdoIoFileStreamP fileStream = FdoIoFileStream::Create(GetConfigFile(), L"r");
        connection->SetConfiguration(fileStream);

        // For Excel, we need a config file because Excel doesn't support
        // primary keys, and Schema Manager does not report tables that
        // lack a key (defect 706075).
        connection->SetConnectionString(GetConnectString());
        connection->Open();

        try
        {
            FdoIUpdate *updateCommand =
                (FdoIUpdate *) connection->CreateCommand(FdoCommandType_Update);
            updateCommand->SetFeatureClassName(L"TABLE1");
            FdoPtr<FdoPropertyValueCollection> propertyValues =
                updateCommand->GetPropertyValues();

            FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();

			start = clock();
            printf("start update feature class\n");
            FdoPtr<FdoDataValue> dataValue;
            FdoPtr<FdoPropertyValue> propertyValue;

            dataValue = FdoDataValue::Create(L"FakeNameUpdate");
            propertyValue = AddNewProperty( propertyValues, L"NAME");
            propertyValue->SetValue(dataValue);

            propertyValue = AddNewProperty( propertyValues, L"Geometry" );
            FdoPtr<FdoIDirectPosition> pos = gf->CreatePositionXY(50, 60);
            FdoPtr<FdoIPoint> pt = gf->CreatePoint(pos);
            FdoPtr<FdoByteArray> byteArray = gf->GetFgf(pt);
            FdoPtr<FdoGeometryValue> geometryValue = FdoGeometryValue::Create(byteArray);
            propertyValue->SetValue(geometryValue);

            FdoPtr<FdoFilter> filter = FdoComparisonCondition::Create(
                FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"NAME") ),
                FdoComparisonOperations_EqualTo,
                FdoPtr<FdoDataValue>(FdoDataValue::Create(L"MyName") ) );

            updateCommand->SetFilter(filter);

             int count = updateCommand->Execute();
            CPPUNIT_ASSERT_MESSAGE ("Update failed to update 1 record", count==1);

            // Now change the values back so that subsequent runs of the test will succeed.
            // Make them not quite the golden values, so that we can view the changes after the test.

            propertyValues->Clear();

            dataValue = FdoDataValue::Create(L"MyName");
            propertyValue = AddNewProperty( propertyValues, L"NAME");
            propertyValue->SetValue(dataValue);

            propertyValue = AddNewProperty( propertyValues, L"Geometry" );
            pos = gf->CreatePositionXY(11, 21);
            pt = gf->CreatePoint(pos);
            byteArray = gf->GetFgf(pt);
            geometryValue = FdoGeometryValue::Create(byteArray);
            propertyValue->SetValue(geometryValue);

            filter = FdoComparisonCondition::Create(
                FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"NAME") ),
                FdoComparisonOperations_EqualTo,
                FdoPtr<FdoDataValue>(FdoDataValue::Create(L"FakeNameUpdate") ) );

            updateCommand->SetFilter(filter);

            count = updateCommand->Execute();
            CPPUNIT_ASSERT_MESSAGE ("Update failed to update 1 record", count==1);

            updateCommand->Release();

			finish = clock();
			printf( "Elapsed: %f seconds\n", ((double)(finish - start) / CLOCKS_PER_SEC) );
        }
        catch (...)
        {
            throw;
        }
    }
    catch (FdoException *ex)
    {
        CPPUNIT_FAIL (UnitTestUtil::w2a(ex->GetExceptionMessage()));
    }
}
#endif
