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
#include "OdbcFdoInsertTest.h"
#include "UnitTestUtil.h"
#include "OdbcBaseSetup.h"

CPPUNIT_TEST_SUITE_REGISTRATION( OdbcOracleFdoInsertTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcOracleFdoInsertTest, "FdoInsertTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcOracleFdoInsertTest, "OdbcOracleFdoInsertTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcOracleFdoInsertTest, "OdbcOracleTests");

CPPUNIT_TEST_SUITE_REGISTRATION( OdbcMySqlFdoInsertTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcMySqlFdoInsertTest, "FdoInsertTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcMySqlFdoInsertTest, "OdbcMySqlFdoInsertTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcMySqlFdoInsertTest, "OdbcMySqlTests");

#ifdef _WIN32
CPPUNIT_TEST_SUITE_REGISTRATION( OdbcAccessFdoInsertTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcAccessFdoInsertTest, "FdoInsertTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcAccessFdoInsertTest, "OdbcAccessFdoInsertTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcAccessFdoInsertTest, "OdbcAccessTests");

CPPUNIT_TEST_SUITE_REGISTRATION( OdbcExcelFdoInsertTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcExcelFdoInsertTest, "FdoInsertTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcExcelFdoInsertTest, "OdbcExcelFdoInsertTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcExcelFdoInsertTest, "OdbcExcelTests");
#endif

void OdbcOracleFdoInsertTest::set_provider()
{
	UnitTestUtil::SetProvider( "OdbcOracle" );
}

void OdbcOracleFdoInsertTest::insertCities()
{
    try
    {
		OdbcBaseSetup pOdbcSetup(DataBaseType_Oracle);
		FdoStringP userConnectString = UnitTestUtil::GetConnectionString(Connection_OraSetup, "");
		FdoPtr<FdoIConnection> connection = UnitTestUtil::GetProviderConnectionObject();
		connection->SetConnectionString ( userConnectString);
		connection->Open();
		pOdbcSetup.CreateDataStore(connection.p, "");
		connection->Close();

		connection->SetConnectionString ( UnitTestUtil::GetConnectionString(Connection_WithDSN, "") );
		connection->Open();
        try
        {
            UnitTestUtil::CreateCityTable(connection);

            FdoIInsert *insertCommand =
                (FdoIInsert *) connection->CreateCommand(FdoCommandType_Insert);
            insertCommand->SetFeatureClassName(L"CITIES");
            FdoPtr<FdoPropertyValueCollection> propertyValues =
                insertCommand->GetPropertyValues();

            printf("start insert non-feature class\n");
            FdoPtr<FdoDataValue> dataValue;
            FdoPtr<FdoPropertyValue> propertyValue;

            try {
                UnitTestUtil::Sql2Db(L"delete from CITIES where CITY = 'FakeVille'", static_cast<FdoIConnection *>(connection));
            }catch (FdoException *ex){ex->Release();}

            dataValue = FdoDataValue::Create(50);
			propertyValue = FdoInsertTest::AddNewProperty( propertyValues, L"CITYID");
            propertyValue->SetValue(dataValue);

            dataValue = FdoDataValue::Create(L"Carleton");
            propertyValue = FdoInsertTest::AddNewProperty( propertyValues, L"NAME");
            propertyValue->SetValue(dataValue);

            dataValue = FdoDataValue::Create(L"FakeVille");
            propertyValue = FdoInsertTest::AddNewProperty( propertyValues, L"CITY");
            propertyValue->SetValue(dataValue);

            FdoPtr<FdoIFeatureReader> reader = insertCommand->Execute();

            insertCommand->Release();
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

void OdbcOracleFdoInsertTest::insertTable1()
{
#if 0
    // Test not converted for Oracle yet.

    try
    {
		// Create/open primary connection:
		FdoPtr<FdoIConnection> connection = UnitTestUtil::GetProviderConnectionObject();
		connection->SetConnectionString ( UnitTestUtil::GetConnectionString(Connection_WithDSN, "") );
		connection->Open();
        try
        {
            UnitTestUtil::CreateTable1(connection);

            FdoIInsert *insertCommand =
                (FdoIInsert *) connection->CreateCommand(FdoCommandType_Insert);
            insertCommand->SetFeatureClassName(L"TABLE1");
            FdoPtr<FdoPropertyValueCollection> propertyValues =
                insertCommand->GetPropertyValues();

            FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();

            printf("start insert feature class\n");
            FdoPtr<FdoDataValue> dataValue;
            FdoPtr<FdoPropertyValue> propertyValue;

            try {
                UnitTestUtil::Sql2Db(L"delete from TABLE1 where NAME = 'FakeName'", static_cast<FdoIConnection *>(connection));
            }catch (FdoException *ex){ex->Release();}

            dataValue = FdoDataValue::Create(50);
            propertyValue = FdoInsertTest::AddNewProperty( propertyValues, L"FEATID1");
            propertyValue->SetValue(dataValue);

            dataValue = FdoDataValue::Create(L"FakeName");
            propertyValue = FdoInsertTest::AddNewProperty( propertyValues, L"NAME");
            propertyValue->SetValue(dataValue);

            propertyValue = FdoInsertTest::AddNewProperty( propertyValues, L"Geometry" );
            FdoPtr<FdoIDirectPosition> pos = gf->CreatePositionXY(50, 60);
            FdoPtr<FdoIPoint> pt = gf->CreatePoint(pos);
            FdoPtr<FdoByteArray> byteArray = gf->GetFgf(pt);
            FdoPtr<FdoGeometryValue> geometryValue = FdoGeometryValue::Create(byteArray);
            propertyValue->SetValue(geometryValue);

            FdoPtr<FdoIFeatureReader> reader = insertCommand->Execute();

            insertCommand->Release();
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

void OdbcMySqlFdoInsertTest::set_provider()
{
	UnitTestUtil::SetProvider( "OdbcMySql" );
}

void OdbcMySqlFdoInsertTest::insert()
{
	clock_t start, finish;
	try
    {
		// Create/open primary connection:
		FdoPtr<FdoIConnection> connection = UnitTestUtil::GetProviderConnectionObject();
		connection->SetConnectionString ( UnitTestUtil::GetConnectionString(Connection_NoDatastore, "") );
		connection->Open();
		OdbcBaseSetup pSetup;
		pSetup.CreateDataStore(connection.p, "");
		pSetup.CreateAcadSchema(connection.p);

		connection->Close();
		connection->SetConnectionString ( UnitTestUtil::GetConnectionString(Connection_WithDSN, "") );
		connection->Open();
        try
        {
            FdoIInsert *insertCommand =
                (FdoIInsert *) connection->CreateCommand(FdoCommandType_Insert);
            insertCommand->SetFeatureClassName(L"acdb3dpolyline");
            FdoPtr<FdoPropertyValueCollection> propertyValues =
                insertCommand->GetPropertyValues();

			start = clock();
            printf("start insert non-feature class\n");
            FdoPtr<FdoDataValue> dataValue;
            FdoPtr<FdoPropertyValue> propertyValue;

            try {
                UnitTestUtil::Sql2Db(L"delete from acdb3dpolyline where featid = '10000'", static_cast<FdoIConnection *>(connection));
            }catch (FdoException *ex){ex->Release();}

            //dataValue = FdoDataValue::Create(10000);
            //propertyValue = AddNewProperty( propertyValues, L"featid");
            //propertyValue->SetValue(dataValue);

            dataValue = FdoDataValue::Create(11);
            propertyValue = AddNewProperty( propertyValues, L"classid");
            propertyValue->SetValue(dataValue);

            dataValue = FdoDataValue::Create(0);
            propertyValue = AddNewProperty( propertyValues, L"revisionnumber");
            propertyValue->SetValue(dataValue);

            FdoPtr<FdoIFeatureReader> reader = insertCommand->Execute();

            insertCommand->Release();

			finish = clock();
			printf( "Elapsed: %f seconds\n", ((double)(finish - start) / CLOCKS_PER_SEC) );
        }
        catch (...)
        {
            throw;
        }
		pSetup.DestroyDataStore(connection.p, "");
		connection->Close();
    }
    catch (FdoException *ex)
    {
        CPPUNIT_FAIL (UnitTestUtil::w2a(ex->GetExceptionMessage()));
    }
}

#ifdef _WIN32
void OdbcAccessFdoInsertTest::set_provider()
{
	UnitTestUtil::SetProvider( "OdbcAccess" );
}

void OdbcAccessFdoInsertTest::insertCities()
{
	clock_t start, finish;
    try
    {
		// Create/open primary connection:
		FdoPtr<FdoIConnection> connection = UnitTestUtil::GetProviderConnectionObject();
		connection->SetConnectionString ( UnitTestUtil::GetConnectionString(Connection_WithDSN, "") );
		connection->Open();
        try
        {
            FdoIInsert *insertCommand =
                (FdoIInsert *) connection->CreateCommand(FdoCommandType_Insert);
            insertCommand->SetFeatureClassName(L"Cities");
            FdoPtr<FdoPropertyValueCollection> propertyValues =
                insertCommand->GetPropertyValues();

            start = clock();
            printf("start insert non-feature class\n");
            FdoPtr<FdoDataValue> dataValue;
            FdoPtr<FdoPropertyValue> propertyValue;

            try {
                UnitTestUtil::Sql2Db(L"delete from Cities where City = 'FakeVille'", static_cast<FdoIConnection *>(connection));
            }catch (FdoException *ex){ex->Release();}

            dataValue = FdoDataValue::Create(50);
            propertyValue = AddNewProperty( propertyValues, L"CityId");
            propertyValue->SetValue(dataValue);

            dataValue = FdoDataValue::Create(L"Carleton");
            propertyValue = AddNewProperty( propertyValues, L"Name");
            propertyValue->SetValue(dataValue);

            dataValue = FdoDataValue::Create(L"FakeVille");
            propertyValue = AddNewProperty( propertyValues, L"City");
            propertyValue->SetValue(dataValue);

            FdoPtr<FdoIFeatureReader> reader = insertCommand->Execute();

            insertCommand->Release();

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

void OdbcAccessFdoInsertTest::insertTable1()
{
	clock_t start, finish;
    try
    {
		// Create/open primary connection:
		FdoPtr<FdoIConnection> connection = UnitTestUtil::GetProviderConnectionObject();
		connection->SetConnectionString ( UnitTestUtil::GetConnectionString(Connection_WithDSN, "") );
		connection->Open();
        try
        {
            FdoIInsert *insertCommand =
                (FdoIInsert *) connection->CreateCommand(FdoCommandType_Insert);
            insertCommand->SetFeatureClassName(L"TABLE1");
            FdoPtr<FdoPropertyValueCollection> propertyValues =
                insertCommand->GetPropertyValues();

            FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();

            start = clock();
            printf("start insert feature class\n");
            FdoPtr<FdoDataValue> dataValue;
            FdoPtr<FdoPropertyValue> propertyValue;

            try {
                UnitTestUtil::Sql2Db(L"delete from TABLE1 where NAME = 'FakeName'", static_cast<FdoIConnection *>(connection));
            }catch (FdoException *ex){ex->Release();}

            dataValue = FdoDataValue::Create(50);
            propertyValue = AddNewProperty( propertyValues, L"FEATID1");
            propertyValue->SetValue(dataValue);

            dataValue = FdoDataValue::Create(L"FakeName");
            propertyValue = AddNewProperty( propertyValues, L"NAME");
            propertyValue->SetValue(dataValue);

            propertyValue = AddNewProperty( propertyValues, L"Geometry" );
            FdoPtr<FdoIDirectPosition> pos = gf->CreatePositionXY(50, 60);
            FdoPtr<FdoIPoint> pt = gf->CreatePoint(pos);
            FdoPtr<FdoByteArray> byteArray = gf->GetFgf(pt);
            FdoPtr<FdoGeometryValue> geometryValue = FdoGeometryValue::Create(byteArray);
            propertyValue->SetValue(geometryValue);

            FdoPtr<FdoIFeatureReader> reader = insertCommand->Execute();

            insertCommand->Release();

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


void OdbcAccessFdoInsertTest::insertLidar()
{
	clock_t start, finish;
    try
    {
        FdoPtr<FdoIConnection> connection = UnitTestUtil::GetProviderConnectionObject();
        if (connection == NULL)
            CPPUNIT_FAIL("FAILED - CreateConnection returned NULL\n");

        connection->SetConnectionString(GetConnectString3());
        connection->Open();

        try
        {
            FdoIInsert *insertCommand =
                (FdoIInsert *) connection->CreateCommand(FdoCommandType_Insert);
            insertCommand->SetFeatureClassName(L"LIDAR");
            FdoPtr<FdoPropertyValueCollection> propertyValues =
                insertCommand->GetPropertyValues();

            FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();

			start = clock();
            printf("start insert feature class LIDAR\n");
            FdoPtr<FdoDataValue> dataValue;
            FdoPtr<FdoPropertyValue> propertyValue;

            // Note that we do NOT set the ID column (example below), because
            // it is auto-generated.
            // dataValue = FdoDataValue::Create(50);
            // propertyValue = AddNewProperty( propertyValues, L"ID");
            // propertyValue->SetValue(dataValue);

            dataValue = FdoDataValue::Create(L"100");
            propertyValue = AddNewProperty( propertyValues, L"ELEVATION");
            propertyValue->SetValue(dataValue);

            propertyValue = AddNewProperty( propertyValues, L"Geometry" );
            FdoPtr<FdoIDirectPosition> pos = gf->CreatePositionXY(50, 60);
            FdoPtr<FdoIPoint> pt = gf->CreatePoint(pos);
            FdoPtr<FdoByteArray> byteArray = gf->GetFgf(pt);
            FdoPtr<FdoGeometryValue> geometryValue = FdoGeometryValue::Create(byteArray);
            propertyValue->SetValue(geometryValue);

            FdoPtr<FdoIFeatureReader> reader = insertCommand->Execute();
            CPPUNIT_ASSERT( reader != NULL );

            FdoInt32 numInserted = 0;
            while ( reader->ReadNext() )
            {
                FdoInt64 id = reader->GetInt64(L"ID");
                numInserted++;
                // Test DB leaves off at 65535.  Also do sanity check for 100000.
                CPPUNIT_ASSERT( id > 65535 && id < 100000 );
            }
            CPPUNIT_ASSERT( numInserted == 1 );

            insertCommand->Release();

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

void OdbcExcelFdoInsertTest::set_provider()
{
	UnitTestUtil::SetProvider( "OdbcExcel" );
}

void OdbcExcelFdoInsertTest::insertTable1()
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
            FdoIInsert *insertCommand =
                (FdoIInsert *) connection->CreateCommand(FdoCommandType_Insert);
            insertCommand->SetFeatureClassName(L"TABLE1");
            FdoPtr<FdoPropertyValueCollection> propertyValues =
                insertCommand->GetPropertyValues();

            FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();

            start = clock();
            printf("start insert feature class\n");
            FdoPtr<FdoDataValue> dataValue;
            FdoPtr<FdoPropertyValue> propertyValue;

            try {
                UnitTestUtil::Sql2Db(L"delete from TABLE1 where NAME = 'FakeName'", static_cast<FdoIConnection *>(connection));
            }catch (FdoException *ex){ex->Release();}

            dataValue = FdoDataValue::Create(50);
            propertyValue = AddNewProperty( propertyValues, L"FEATID1");
            propertyValue->SetValue(dataValue);

            dataValue = FdoDataValue::Create(L"FakeName");
            propertyValue = AddNewProperty( propertyValues, L"NAME");
            propertyValue->SetValue(dataValue);

            propertyValue = AddNewProperty( propertyValues, L"Geometry" );
            FdoPtr<FdoIDirectPosition> pos = gf->CreatePositionXY(50, 60);
            FdoPtr<FdoIPoint> pt = gf->CreatePoint(pos);
            FdoPtr<FdoByteArray> byteArray = gf->GetFgf(pt);
            FdoPtr<FdoGeometryValue> geometryValue = FdoGeometryValue::Create(byteArray);
            propertyValue->SetValue(geometryValue);

            FdoPtr<FdoIFeatureReader> reader = insertCommand->Execute();
            insertCommand->Release();

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

void OdbcExcelFdoInsertTest::insertPoints()
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
            FdoIInsert *insertCommand =
                (FdoIInsert *) connection->CreateCommand(FdoCommandType_Insert);
            insertCommand->SetFeatureClassName(L"POINTS");
            FdoPtr<FdoPropertyValueCollection> propertyValues =
                insertCommand->GetPropertyValues();

            FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();

            start = clock();
            printf("start insert feature class\n");
            FdoPtr<FdoDataValue> dataValue;
            FdoPtr<FdoPropertyValue> propertyValue;

            try {
                UnitTestUtil::Sql2Db(L"delete from POINTS where PNO = '3000'", static_cast<FdoIConnection *>(connection));
            }catch (FdoException *ex){ex->Release();}

            dataValue = FdoDataValue::Create(3000);
            propertyValue = AddNewProperty( propertyValues, L"PNO");
            propertyValue->SetValue(dataValue);

            dataValue = FdoDataValue::Create(L"Fake");
            propertyValue = AddNewProperty( propertyValues, L"DSC");
            propertyValue->SetValue(dataValue);

            propertyValue = AddNewProperty( propertyValues, L"Geometry" );
            FdoPtr<FdoIDirectPosition> pos = gf->CreatePositionXYZ(50, 60, 70);
            FdoPtr<FdoIPoint> pt = gf->CreatePoint(pos);
            FdoPtr<FdoByteArray> byteArray = gf->GetFgf(pt);
            FdoPtr<FdoGeometryValue> geometryValue = FdoGeometryValue::Create(byteArray);
            propertyValue->SetValue(geometryValue);

            FdoPtr<FdoIFeatureReader> reader = insertCommand->Execute();
            insertCommand->Release();

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
