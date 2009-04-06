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
		    mConnection->SetConnectionString ( UnitTestUtil::GetConnectionString(connectionType) );
		    mConnection->Open();
		    mSetup.CreateDataStore(mConnection, L"");
		    mSetup.CreateAcadSchema(mConnection);
		    mSetup.CreateNonAcadSchema(mConnection);

		    mConnection->Close();
        }
		mConnection->SetConnectionString ( UnitTestUtil::GetConnectionString(Connection_WithDSN) );
		mConnection->Open();
    }
    catch (FdoException *ex)
    {
        mConnection = NULL;
        TestCommonFail (ex);
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
        TestCommonFail (ex);
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

            FdoPtr<FdoIUpdate> updateCommand =
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

        }
        catch (...)
        {
            throw;
        }
    }
    catch (FdoException *ex)
    {
        TestCommonFail (ex);
    }
#endif
}

void OdbcBaseFdoUpdateTest::updateNonDblGeom()
{
    if (mConnection != NULL) try
    {
        UnitTestUtil::Sql2Db(L"delete from table6", static_cast<FdoIConnection *>(mConnection.p));
        UnitTestUtil::Sql2Db(L"delete from table7", static_cast<FdoIConnection *>(mConnection.p));
        UnitTestUtil::Sql2Db(L"delete from table8", static_cast<FdoIConnection *>(mConnection.p));

        insertNonDblGeomRow( mSetup.GetClassNameTable6(), 1, 123.456789123456, 123.456789123456, 123.456789123456 );
        insertNonDblGeomRow( mSetup.GetClassNameTable7(), 1, 123.456789123456, 123.456789123456, 123.456789123456 );
        insertNonDblGeomRow( mSetup.GetClassNameTable7(), 2, 18.9, -100.8, -11111.5 );
        insertNonDblGeomRow( mSetup.GetClassNameTable8(), 1, 123.456789123456, 123.456789123456, 123.456789123456 );

        // MySQL seems to round single precision numbers less precision that other RDBMS's. 
        // getTable6X accounts for different expected values for MySQL. 
        vldNonDblGeomRow( mSetup.GetClassNameTable6(), 1, getTable6X(true), 123.457, 123.456789123456 );
        vldNonDblGeomRow( mSetup.GetClassNameTable7(), 1, 123, 123, 123 );
        vldNonDblGeomRow( mSetup.GetClassNameTable7(), 2, 19, -101, -11112 );
        vldNonDblGeomRow( mSetup.GetClassNameTable8(), 1, 123, 123.456789123456, 123.456789 );

        updateNonDblGeomRow( mSetup.GetClassNameTable6(), 1, 1.12121212121212, 0.123456, 1e10 );
        updateNonDblGeomRow( mSetup.GetClassNameTable7(), 1, 4.8, 3.9, 2.99 );
        updateNonDblGeomRow( mSetup.GetClassNameTable8(), 1, 1e12, 1e-100, 0.987654321 );

        vldNonDblGeomRow( mSetup.GetClassNameTable6(), 1, getTable6X(false), 0.123, 1e10);
        vldNonDblGeomRow( mSetup.GetClassNameTable7(), 1, 5, 4, 3 );
        vldNonDblGeomRow( mSetup.GetClassNameTable7(), 2, 19, -101, -11112 );
        vldNonDblGeomRow( mSetup.GetClassNameTable8(), 1, 1e12, 1e-100, 0.987654 );
    }
    catch (FdoException *ex)
    {
        TestCommonFail (ex);
    }
}


void OdbcBaseFdoUpdateTest::insertNonDblGeomRow( FdoString* className, int FeatId, double x, double y, double z )
{
    FdoPtr<FdoIInsert> insertCommand =
        (FdoIInsert *) mConnection->CreateCommand(FdoCommandType_Insert);
    insertCommand->SetFeatureClassName(className);
    FdoPtr<FdoPropertyValueCollection> propertyValues =
        insertCommand->GetPropertyValues();

    FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();

    FdoPtr<FdoDataValue> dataValue;
    FdoPtr<FdoPropertyValue> propertyValue;

    dataValue = FdoDataValue::Create((FdoInt32) FeatId);
    propertyValue = AddNewProperty( propertyValues, mSetup.GetPropertyNameTable1FeatId());
    propertyValue->SetValue(dataValue);

    propertyValue = AddNewProperty( propertyValues, L"Geometry" );
    FdoPtr<FdoIDirectPosition> pos = gf->CreatePositionXYZ(x, y, z);
    FdoPtr<FdoIPoint> pt = gf->CreatePoint(pos);
    FdoPtr<FdoByteArray> byteArray = gf->GetFgf(pt);
    FdoPtr<FdoGeometryValue> geometryValue = FdoGeometryValue::Create(byteArray);
    propertyValue->SetValue(geometryValue);

    FdoPtr<FdoIFeatureReader> reader = insertCommand->Execute();
}


void OdbcBaseFdoUpdateTest::updateNonDblGeomRow( FdoString* className, int FeatId, double x, double y, double z )
{
    FdoPtr<FdoIUpdate> updateCommand = 
        (FdoIUpdate*)mConnection->CreateCommand(FdoCommandType_Update); 
    updateCommand->SetFeatureClassName(className);

    FdoPtr<FdoFilter> filter = FdoFilter::Parse( 
        FdoStringP::Format( 
            L"%ls = %d",
            mSetup.GetPropertyNameTable1FeatId(),
            FeatId
        )
    );

    updateCommand->SetFilter( filter );

    FdoPtr<FdoPropertyValueCollection> propertyValues =
        updateCommand->GetPropertyValues();

    FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();

    FdoPtr<FdoDataValue> dataValue;
    FdoPtr<FdoPropertyValue> propertyValue;

    propertyValue = AddNewProperty( propertyValues, L"Geometry" );
    FdoPtr<FdoIDirectPosition> pos = gf->CreatePositionXYZ(x, y, z);
    FdoPtr<FdoIPoint> pt = gf->CreatePoint(pos);
    FdoPtr<FdoByteArray> byteArray = gf->GetFgf(pt);
    FdoPtr<FdoGeometryValue> geometryValue = FdoGeometryValue::Create(byteArray);
    propertyValue->SetValue(geometryValue);

    updateCommand->Execute();
}


void OdbcBaseFdoUpdateTest::vldNonDblGeomRow( FdoString* className, int FeatId, double x, double y, double z )
{
    FdoPtr<FdoISelect> selectCommand = 
        (FdoISelect*)mConnection->CreateCommand(FdoCommandType_Select); 
    selectCommand->SetFeatureClassName(className);

    FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();

    FdoPtr<FdoFilter> filter = FdoFilter::Parse( 
        FdoStringP::Format( 
            L"%ls = %d",
            mSetup.GetPropertyNameTable1FeatId(),
            FeatId
        )
    );

    selectCommand->SetFilter( filter );

	FdoPtr<FdoIFeatureReader> rdr = selectCommand->Execute();

    CPPUNIT_ASSERT(rdr->ReadNext());
    CPPUNIT_ASSERT(!rdr->IsNull(L"Geometry"));

    FdoPtr<FdoByteArray> byteArray = rdr->GetGeometry(L"Geometry");
    FdoPtr<FdoIGeometry> geom = gf->CreateGeometryFromFgf(byteArray);
    FdoGeometryType geomType = geom->GetDerivedType();
    CPPUNIT_ASSERT(geomType == FdoGeometryType_Point);

    FdoIPoint* point = static_cast<FdoIPoint*>(geom.p);

    FdoPtr<FdoIDirectPosition> posn = point->GetPosition();


    CPPUNIT_ASSERT( compareDbl(posn->GetX(), x) == 0 );
    CPPUNIT_ASSERT( compareDbl(posn->GetY(), y) == 0 );
    CPPUNIT_ASSERT( compareDbl(posn->GetZ(), z) == 0 );
}

int OdbcBaseFdoUpdateTest::compareDbl( double num1, double num2 )
{
    char string1[50];
    char string2[50];

    // Different RDBMS's store slightly different values for double precision numbers.
    // Rounding to 15 digits eliminates these very slight differences. 
    sprintf( string1, "%.15g", num1 );
    sprintf( string2, "%.15g", num2 );

    return strcmp( string1, string2 );
}

#ifdef _WIN32

void OdbcAccessFdoUpdateTest::updateCities()
{
	clock_t start, finish;
    try
    {
        FdoPtr<FdoIConnection> connection = UnitTestUtil::GetProviderConnectionObject();
		connection->SetConnectionString(UnitTestUtil::GetConnectionString(Connection_WithDSN));
        connection->Open();
        FdoPtr<FdoIUpdate> updateCommand =
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
		finish = clock();
		printf( "Elapsed: %f seconds\n", ((double)(finish - start) / CLOCKS_PER_SEC) );
    }
    catch (FdoException *ex)
    {
        TestCommonFail (ex);
    }
}

void OdbcAccessFdoUpdateTest::updateTable1()
{
	clock_t start, finish;
    try
    {
        FdoPtr<FdoIConnection> connection = UnitTestUtil::GetProviderConnectionObject();
		connection->SetConnectionString(UnitTestUtil::GetConnectionString(Connection_WithDSN));
        connection->Open();
        try
        {
            FdoPtr<FdoIUpdate> updateCommand =
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
        TestCommonFail (ex);
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

        FdoPtr<FdoIUpdate> updateCommand =
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
        return;

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

		finish = clock();
		printf( "Elapsed: %f seconds\n", ((double)(finish - start) / CLOCKS_PER_SEC) );
    }
    catch (FdoException *ex)
    {
        TestCommonFail (ex);
    }
}

#endif
