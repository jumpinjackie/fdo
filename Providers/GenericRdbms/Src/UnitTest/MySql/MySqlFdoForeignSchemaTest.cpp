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
#include "MySqlFdoForeignSchemaTest.h"
#include "UnitTestUtil.h"

#define   DB_NAME_SUFFIX            L"_fs"

CPPUNIT_TEST_SUITE_REGISTRATION( MySqlFdoForeignSchemaTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( MySqlFdoForeignSchemaTest, "FdoForeignSchemaTest");

void MySqlFdoForeignSchemaTest::set_provider()
{
	UnitTestUtil::SetProvider( "MySql" );
}
void MySqlFdoForeignSchemaTest::create_foreign_datastore()
{
	bool dbExists = UnitTestUtil::DatastoreExists(DB_NAME_SUFFIX);
	FdoStringP userConnectString = UnitTestUtil::GetConnectionString(Connection_NoDatastore, L"");
	FdoIConnection* connection = UnitTestUtil::GetProviderConnectionObject();

    connection->SetConnectionString ( userConnectString);
    connection->Open();
	FdoStringP sqlStmt;

	if (dbExists)
	{
		sqlStmt = FdoStringP::Format(L"drop database %ls", (FdoString*) UnitTestUtil::GetEnviron("datastore",DB_NAME_SUFFIX));
		UnitTestUtil::Sql2Db(sqlStmt, connection);
	}
	sqlStmt = FdoStringP::Format(
		L"create database %ls character set latin1 collate latin1_bin", (FdoString*) UnitTestUtil::GetEnviron("datastore",DB_NAME_SUFFIX));
   
	UnitTestUtil::Sql2Db(sqlStmt, connection);

	userConnectString = UnitTestUtil::GetConnectionString(Connection_WithDatastore, DB_NAME_SUFFIX);
    connection->SetConnectionString (userConnectString);
    connection->Open ();

	sqlStmt = FdoStringP::Format(
		L"create table device_tbl (id decimal(20), string varchar(64), install_date datetime, int32 integer(5), constraint device_tbl_pk primary key (id, string, install_date))");
	UnitTestUtil::Sql2Db(sqlStmt, connection);

	sqlStmt = FdoStringP::Format(
		L"create table point (entityid bigint not null auto_increment, geometry1 point, geometry2 point, pname varchar(64), geometry3 point, constraint point_pk primary key (entityid))");
	UnitTestUtil::Sql2Db(sqlStmt, connection);
	// insert one row with non 0 SRIDS
	sqlStmt = FdoStringP::Format(
		L"insert into point( pname, geometry1, geometry2, geometry3) values \n"
		L"('Toronto', GeomFromText('POINT(20 30)', 101),\n"
		L"GeomFromText('POINT(120 130)', 102),\n"
		L"GeomFromText('POINT(220 230)', 103))");
	UnitTestUtil::Sql2Db(sqlStmt, connection);

	connection->Close();
}
void MySqlFdoForeignSchemaTest::insert()
{
	FdoPtr<FdoIConnection> connection = UnitTestUtil::GetConnection(DB_NAME_SUFFIX, false);
	try
	{
		FdoIInsert *insertCommand = (FdoIInsert *) connection->CreateCommand(FdoCommandType_Insert);
        insertCommand->SetFeatureClassName(L"device_tbl");
        FdoPtr<FdoPropertyValueCollection> propertyValues = insertCommand->GetPropertyValues();

		FdoPtr<FdoDataValue> dataValue;
        FdoPtr<FdoPropertyValue> propertyValue;
		FdoInt64 idVal = 12;
		dataValue = FdoDataValue::Create(idVal);
        propertyValue = AddNewProperty( propertyValues,L"id"); 
        propertyValue->SetValue(dataValue);

		dataValue = FdoDataValue::Create(L"CANADA");
        propertyValue = AddNewProperty( propertyValues,L"string"); 
        propertyValue->SetValue(dataValue);
		
		FdoDateTime       currentDateTime;
		currentDateTime.year = 2006;
		currentDateTime.month = 11;
		currentDateTime.day = 18;
		currentDateTime.hour = 12;
		currentDateTime.minute = 45;
		currentDateTime.seconds = 15;

		dataValue = FdoDataValue::Create(currentDateTime);
        propertyValue = AddNewProperty( propertyValues,L"install_date"); 
        propertyValue->SetValue(dataValue);

		dataValue = FdoDataValue::Create(L"255");
        propertyValue = AddNewProperty( propertyValues,L"int32"); 
        propertyValue->SetValue(dataValue);

		{
		FdoIFeatureReader *myReader = insertCommand->Execute();

		CPPUNIT_ASSERT(myReader != NULL);
		// verify that the identity properties have the right values
		while (myReader->ReadNext())
		{
			CPPUNIT_ASSERT((int)(myReader->GetDouble(L"id")) == 12);
			CPPUNIT_ASSERT( FdoStringP(myReader->GetString(L"string")).ICompare(L"CANADA") == 0);
			FdoDateTime dateTime = myReader->GetDateTime(L"install_date");
			CPPUNIT_ASSERT(dateTime.year == 2006);
			CPPUNIT_ASSERT(dateTime.month == 11);
			CPPUNIT_ASSERT(dateTime.day == 18);
			CPPUNIT_ASSERT(dateTime.hour == 12);
			CPPUNIT_ASSERT(dateTime.minute == 45);
			CPPUNIT_ASSERT(dateTime.seconds == 15);
		}
		myReader->Close();
		}
		connection->Close();
	}
	catch (FdoCommandException *ex)
	{
		CPPUNIT_FAIL (UnitTestUtil::w2a(ex->GetExceptionMessage()));
	}
   	catch (FdoException *ex)
	{
		CPPUNIT_FAIL (UnitTestUtil::w2a(ex->GetExceptionMessage()));
	}
}

void MySqlFdoForeignSchemaTest::insertSRID ()
{
    try
    {
        FdoPtr<FdoIConnection> connection = UnitTestUtil::GetConnection(DB_NAME_SUFFIX, false);

        try
        {

            FdoPtr<FdoPropertyValue> propertyValue;

            FdoPtr<FdoIInsert> insertCommand = (FdoIInsert *) connection->CreateCommand(FdoCommandType_Insert);
            insertCommand->SetFeatureClassName(L"point");
            FdoPtr<FdoPropertyValueCollection> propertyValues = insertCommand->GetPropertyValues();
			FdoPtr<FdoDataValue> dataValue;
			dataValue = FdoDataValue::Create(L"Ottawa");
            propertyValue = AddNewProperty( propertyValues, L"pname");
            propertyValue->SetValue(dataValue);

            FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
			FdoPtr<FdoIDirectPosition> pos = gf->CreatePositionXY(50, 60);

			// first geometry column
            propertyValue = FdoPropertyValue::Create();
			propertyValue->SetName(L"geometry1");
			propertyValues->Add( propertyValue );
		
            FdoPtr<FdoIPoint> point1 = gf->CreatePoint(pos);
            FdoPtr<FdoByteArray> byteArray = gf->GetFgf(point1);

            FdoPtr<FdoGeometryValue> geometryValue = FdoGeometryValue::Create(byteArray);
            propertyValue->SetValue(geometryValue);
			
			// second geometry column
			pos = gf->CreatePositionXY(20, 90);
			propertyValue = FdoPropertyValue::Create();
			propertyValue->SetName(L"geometry2");
			propertyValues->Add( propertyValue );
		
            point1 = gf->CreatePoint(pos);
            byteArray = gf->GetFgf(point1);

            geometryValue = FdoGeometryValue::Create(byteArray);
            propertyValue->SetValue(geometryValue);

			// third geometry column
			pos = gf->CreatePositionXY(20, 70);
			propertyValue = FdoPropertyValue::Create();
			propertyValue->SetName(L"geometry3");
			propertyValues->Add( propertyValue );
		
            point1 = gf->CreatePoint(pos);
            byteArray = gf->GetFgf(point1);

            geometryValue = FdoGeometryValue::Create(byteArray);
            propertyValue->SetValue(geometryValue);

			FdoPtr<FdoIFeatureReader> featureReader = insertCommand->Execute();
			CPPUNIT_ASSERT(featureReader != NULL);
			if (featureReader->ReadNext())
				CPPUNIT_ASSERT(!featureReader->IsNull(L"entityid"));
			FdoInt64 id = featureReader->GetInt64(L"entityid");;
			featureReader->Close();
			// verify SRIDs
			FdoPtr<FdoISQLCommand> selCmd = (FdoISQLCommand*)connection->CreateCommand( FdoCommandType_SQLCommand );
            FdoStringP sql = FdoStringP::Format(L"select srid(geometry1) as srid1, srid(geometry2) as srid2, srid(geometry3) as srid3 from point where entityid = %ld", id);

            selCmd->SetSQLStatement( (FdoString *)sql );
            FdoPtr<FdoISQLDataReader> myReader = selCmd->ExecuteReader();
            if( myReader != NULL  )
            {
				//valid values for SRID are
				// srid1 = 101
				// srid2 = 102
				// srid3 = 103
				if (myReader->ReadNext())
				{
					if (!myReader->IsNull(L"srid1"))
					{
						CPPUNIT_ASSERT(myReader->GetInt64(L"srid1") == 101);
						CPPUNIT_ASSERT(myReader->GetInt64(L"srid2") == 102);
						CPPUNIT_ASSERT(myReader->GetInt64(L"srid3") == 103);
					}
				}
			}
                
			myReader->Close();
			connection->Close();
        }
        catch (...)
        {
            if (connection)
                connection->Close ();
            throw;
        }
        if (connection)
            connection->Close();
    }
    catch (FdoCommandException *ex)
    {
        UnitTestUtil::FailOnException(ex);
    }
    catch (FdoException *ex)
    {
        UnitTestUtil::FailOnException(ex);
    }
}
void MySqlFdoForeignSchemaTest::updateGeomWithSRID()
{
	try
    {
        FdoPtr<FdoIConnection> connection = UnitTestUtil::GetConnection(DB_NAME_SUFFIX, false);

        try
        {
			FdoInt64 id;
            FdoPtr<FdoPropertyValue> propertyValue;

			// find entityid of the row to update
			FdoPtr<FdoISQLCommand> selCmd = (FdoISQLCommand*)connection->CreateCommand( FdoCommandType_SQLCommand );
            FdoStringP sql = FdoStringP::Format(L"select entityid as id from point");

            selCmd->SetSQLStatement( (FdoString *)sql );
            FdoPtr<FdoISQLDataReader> myReader = selCmd->ExecuteReader();
			CPPUNIT_ASSERT(myReader != NULL);
			if (myReader->ReadNext())
				id = myReader->GetInt64(L"id");
			myReader->Close();

			FdoPtr<FdoIUpdate> updCmd = (FdoIUpdate *) connection->CreateCommand(FdoCommandType_Update);
            updCmd->SetFeatureClassName(L"point");
			FdoPtr<FdoFilter>   filter = FdoComparisonCondition::Create(
                FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"entityid") ),
                FdoComparisonOperations_EqualTo,
                FdoPtr<FdoDataValue>(FdoDataValue::Create(id) ) );
			updCmd->SetFilter( filter );
            FdoPtr<FdoPropertyValueCollection> propertyValues = updCmd->GetPropertyValues();
			
            FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
			FdoPtr<FdoIDirectPosition> pos = gf->CreatePositionXY(150, 160);

			// first geometry column
            propertyValue = FdoPropertyValue::Create();
			propertyValue->SetName(L"geometry1");
			propertyValues->Add( propertyValue );

			FdoPtr<FdoIPoint> point1 = gf->CreatePoint(pos);
            FdoPtr<FdoByteArray> byteArray = gf->GetFgf(point1);

            FdoPtr<FdoGeometryValue> geometryValue = FdoGeometryValue::Create(byteArray);
            propertyValue->SetValue(geometryValue);

			int numRows = updCmd->Execute();
			// check srid, it should be 101
			sql = FdoStringP::Format(L"select srid(geometry1) as srid1 from point where entityid = %ld", id);

            selCmd->SetSQLStatement( (FdoString *)sql );
            myReader = selCmd->ExecuteReader();
			CPPUNIT_ASSERT(myReader != NULL);
			if (myReader->ReadNext())
			{
				if (!myReader->IsNull(L"srid1"))
					CPPUNIT_ASSERT(myReader->GetInt64(L"srid1") == 101);
			}
			myReader->Close();
			connection->Close();
		}
		catch (...)
        {
            if (connection)
                connection->Close ();
            throw;
        }
        if (connection)
            connection->Close();
    }
    catch (FdoCommandException *ex)
    {
        UnitTestUtil::FailOnException(ex);
    }
    catch (FdoException *ex)
    {
        UnitTestUtil::FailOnException(ex);
	}
}
