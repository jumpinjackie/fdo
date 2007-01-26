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
#include "OdbcFdoSchemaTest.h"
#include "UnitTestUtil.h"
#include "OdbcBaseSetup.h"

#define DB_SUFFIX L"_nometaschema"

CPPUNIT_TEST_SUITE_REGISTRATION( OdbcMySqlFdoSchemaTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcMySqlFdoSchemaTest, "FdoSchemaTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcMySqlFdoSchemaTest, "OdbcMySqlFdoSchemaTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcMySqlFdoSchemaTest, "OdbcMySqlTests");

#ifdef _WIN32
CPPUNIT_TEST_SUITE_REGISTRATION( OdbcSqlServerFdoSchemaTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcSqlServerFdoSchemaTest, "FdoSchemaTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcSqlServerFdoSchemaTest, "OdbcSqlServerFdoSchemaTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcSqlServerFdoSchemaTest, "OdbcSqlServerTests");

CPPUNIT_TEST_SUITE_REGISTRATION( OdbcAccessFdoSchemaTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcAccessFdoSchemaTest, "FdoSchemaTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcAccessFdoSchemaTest, "OdbcAccessFdoSchemaTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcAccessFdoSchemaTest, "OdbcAccessTests");
#endif

void OdbcFdoSchemaTest::DropDatastore()
{
    // Drop the old datastore, if it exists:
    try
    {
        FdoStringP userConnectString = UnitTestUtil::GetConnectionString(Connection_NoDatastore);
        FdoPtr<FdoIConnection> connection = UnitTestUtil::GetProviderConnectionObject();
        connection->SetConnectionString( userConnectString );
        connection->Open();
		m_OdbcSetup.DestroyDataStore( connection.p, DB_SUFFIX );
        connection->Close();
    }
    catch(...) { }
}

void OdbcFdoSchemaTest::CreateFreshDb()
{
    if (!m_bDatabaseCreated)
    {
		DropDatastore();
        // Create a datastore (will create metaschema tables automatically, unfortunately):
		try
		{
			FdoStringP userConnectString = UnitTestUtil::GetConnectionString(Connection_NoDatastore);
			FdoPtr<FdoIConnection> connection = UnitTestUtil::GetProviderConnectionObject();
			connection->SetConnectionString( userConnectString );
			connection->Open();
			m_OdbcSetup.CreateDataStore( connection.p, DB_SUFFIX );
			connection->Close();
		}
		catch(...) { }

        // Connect:
        FdoPtr<FdoIConnection> connection = UnitTestUtil::GetProviderConnectionObject();
        if (connection == NULL)
            CPPUNIT_FAIL("FAILED - CreateConnection returned NULL\n");
        connection->SetConnectionString(UnitTestUtil::GetConnectionString(Connection_WithDatastore, DB_SUFFIX));
        connection->Open();

		if (DataBaseType_MySQL == m_OdbcSetup.GetTypeDB())
			UnitTestUtil::Sql2Db( (const wchar_t**) mApplySchemaMySqlCt, connection.p );
		else 
			if (DataBaseType_SqlServer == m_OdbcSetup.GetTypeDB())
			UnitTestUtil::Sql2Db( (const wchar_t**) mApplySchemaSqlServerCt, connection.p );

        // Insert some data:
		FdoPtr<FdoISQLCommand> sql = (FdoISQLCommand*)connection->CreateCommand(FdoCommandType_SQLCommand);
        sql->SetSQLStatement(L"insert into lowercaseclass (x, y, z, id, revisionnumber, classid) values (5.0, 5.0, 0.0, 1, 0, 1)");
        sql->ExecuteNonQuery();
        sql->SetSQLStatement(L"insert into lowercaseclass (x, y, z, id, revisionnumber, classid) values (-5.0, -5.0, 0.0, 2, 0, 1)");
        sql->ExecuteNonQuery();
        // Create two tables with odd ascii characters (like '+' and '='):
        try
        {
            sql->SetSQLStatement(L"drop table \"TESTODDNAME1+\"");
            sql->ExecuteNonQuery();
        }
        catch (FdoException *e)
        {
            e->Release();
        }
        sql->SetSQLStatement(L"create table \"TESTODDNAME1+\" (mynum INT)");
        sql->ExecuteNonQuery();
        try
        {
            sql->SetSQLStatement(L"drop table \"TESTODDNAME2=\"");
            sql->ExecuteNonQuery();
        }
        catch (FdoException *e)
        {
            e->Release();
        }
        sql->SetSQLStatement(L"create table \"TESTODDNAME2=\" (mynum INT)");
        sql->ExecuteNonQuery();

        // Disconnect:
        connection->Close();
        connection = NULL;

        // Remember that we created the database successfully:
        m_bDatabaseCreated = true;
    }
}

const wchar_t* OdbcFdoSchemaTest::mApplySchemaSqlServerCt[] = {
	L"create table lowercaseclass (",
	L"classid bigint NOT NULL ,",
	L"featid bigint IDENTITY NOT NULL,",
	L"revisionnumber float NOT NULL,",
	L"realgeometry image NULL,",
	L"REALGEOMETRY_SI_1 varchar (255),",
	L"REALGEOMETRY_SI_2 varchar (255),",
	L"z float NOT NULL,",
	L"y float NOT NULL,",
	L"x float NOT NULL,",
	L"id int NOT NULL", 
	L");",
    NULL
};

const wchar_t* OdbcFdoSchemaTest::mApplySchemaMySqlCt[] = {
	L"create table lowercaseclass (",
	L"classid bigint NOT NULL,",
	L"featid bigint NOT NULL auto_increment,",
	L"revisionnumber double NOT NULL,",
	L"z double NOT NULL,",
	L"y double NOT NULL,",
	L"x double NOT NULL,",
	L"id int NOT NULL,",
	L"realgeometry geometry default NULL,",
	L"PRIMARY KEY  (id),",
	L"UNIQUE KEY featid (featid)",
	L")ENGINE=MyISAM;",
    NULL
};

void OdbcMySqlFdoSchemaTest::set_provider()
{
	UnitTestUtil::SetProvider( "OdbcMySql" );
}
#ifdef _WIN32
void OdbcSqlServerFdoSchemaTest::set_provider()
{
	UnitTestUtil::SetProvider( "OdbcSqlServer" );
}

void OdbcSqlServerFdoSchemaTest::LowerCaseXYZ()
{
    try {
        CreateFreshDb();

        // Connect:
        FdoPtr<FdoIConnection> connection = UnitTestUtil::GetProviderConnectionObject();
        if (connection == NULL)
            CPPUNIT_FAIL("FAILED - CreateConnection returned NULL\n");
        connection->SetConnectionString(UnitTestUtil::GetConnectionString(Connection_WithDatastore, DB_SUFFIX));
        connection->Open();

        // DescribeSchema:
        FdoPtr<FdoIDescribeSchema> descSchema2 = (FdoIDescribeSchema*)connection->CreateCommand(FdoCommandType_DescribeSchema);
        FdoPtr<FdoFeatureSchemaCollection> schemas2 = descSchema2->Execute();
        FdoPtr<FdoFeatureSchema> schema2 = schemas2->GetItem(0);
        FdoPtr<FdoClassCollection> classes2 = schema2->GetClasses();
        FdoPtr<FdoClassDefinition> classDef = classes2->GetItem(L"lowercaseclass");
        FdoStringP geomName;
        if (classDef->GetClassType() == FdoClassType_FeatureClass)
        {
            FdoFeatureClass* featureClass = static_cast<FdoFeatureClass*>(classDef.p);
            FdoPtr<FdoGeometricPropertyDefinition> geomProp = featureClass->GetGeometryProperty();
            geomName = geomProp->GetName();    // without the fix, get error here due to geomProp==NULL
        }

        // Select:
        FdoPtr<FdoISelect> select = (FdoISelect*)connection->CreateCommand(FdoCommandType_Select);
        select->SetFeatureClassName(L"lowercaseclass");
        FdoPtr<FdoIFeatureReader> reader = select->Execute();
        FdoInt32 featCount = 0;
        while (reader->ReadNext())
        {
            FdoPtr<FdoClassDefinition> classDef = reader->GetClassDefinition();
            featCount++;
        }
        reader = NULL;

        // Spatial query:
        select->SetFilter(L"Geometry INTERSECTS GEOMFROMTEXT('POLYGON XY ((0.0 0.0, 100.0 0.0, 100.0 100.0, 0.0 100.0, 0.0 0.0))')");
        reader = select->Execute();
        FdoInt32 featCountSpatialQuery = 0;
        while (reader->ReadNext())
        {
            featCountSpatialQuery++;
        }
        reader = NULL;
        CPPUNIT_ASSERT_MESSAGE("Wrong result from spatial query", featCountSpatialQuery==1);
    }
    catch ( FdoException* e ) {
		UnitTestUtil::FailOnException( e );
    }
}

void OdbcAccessFdoSchemaTest::set_provider()
{
	UnitTestUtil::SetProvider( "OdbcAccess" );
}

void OdbcAccessFdoSchemaTest::SchemaMappingTest()
{
    try
    {
        FdoPtr<FdoIConnection> connection = UnitTestUtil::GetProviderConnectionObject();
        if (connection == NULL)
            CPPUNIT_FAIL("FAILED - CreateConnection returned NULL\n");

        FdoIoFileStreamP fileStream = FdoIoFileStream::Create(GetConfigFile(), L"r");

        // Import the configuration file, which contains a schema mapping.
        connection->SetConfiguration(fileStream);

		connection->SetConnectionString(UnitTestUtil::GetConnectionString(Connection_WithDSN));
        connection->Open();

        FdoPtr<FdoIDescribeSchemaMapping> describeSchemaMappingCmd =
            (FdoIDescribeSchemaMapping*)connection->CreateCommand(FdoCommandType_DescribeSchemaMapping);
        FdoSchemaMappingsP mappings = describeSchemaMappingCmd->Execute();

        mappings->WriteXml(UnitTestUtil::GetOutputFileName( L"DescribeSchemaMapping.xml" ) );

        connection->Close();
    }
    catch (FdoException* e)
    {
		TestCommonFail(e);
    }
}
#endif
