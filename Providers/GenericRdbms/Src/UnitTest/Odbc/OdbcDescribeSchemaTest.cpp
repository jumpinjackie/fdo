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
#include "OdbcDescribeSchemaTest.h"
#include "UnitTestUtil.h"

#define SUFFIX_TEST "_describeschema"

CPPUNIT_TEST_SUITE_REGISTRATION (OdbcMySqlDescribeSchemaTest);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcMySqlDescribeSchemaTest, "DescribeSchemaTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcMySqlDescribeSchemaTest, "OdbcMySqlDescribeSchemaTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcMySqlDescribeSchemaTest, "OdbcMySqlTests");

#ifdef _WIN32
CPPUNIT_TEST_SUITE_REGISTRATION (OdbcSqlServerDescribeSchemaTest);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcSqlServerDescribeSchemaTest, "DescribeSchemaTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcSqlServerDescribeSchemaTest, "OdbcSqlServerDescribeSchemaTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcSqlServerDescribeSchemaTest, "OdbcSqlServerTests");

CPPUNIT_TEST_SUITE_REGISTRATION (OdbcTextDescribeSchemaTest);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcTextDescribeSchemaTest, "DescribeSchemaTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcTextDescribeSchemaTest, "OdbcTextDescribeSchemaTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcTextDescribeSchemaTest, "OdbcTextTests");
#endif

void OdbcDescribeSchemaTest::LoadTestData(FdoIConnection* connection)
{
	UnitTestUtil::Sql2Db( (const wchar_t**) mInputSchema, connection );
}
void OdbcDescribeSchemaTest::describe()
{
    FdoPtr<FdoIConnection> connection;
    try
    {
        printf( "Initializing Connection ... \n" );
		FdoStringP userConnectString = UnitTestUtil::GetConnectionString(Connection_NoDatastore, "");
		// Create/open primary connection:
		connection = UnitTestUtil::GetProviderConnectionObject();
		connection->SetConnectionString ( userConnectString);
		connection->Open();

		m_OdbcSetup.CreateDataStore(connection.p, SUFFIX_TEST);

        printf( "Loading test data ... \n" );

        // Load data into newly created datastore
        LoadTestData(connection /*, sm*/ );

        printf( "Performing Describe ... \n" );
        FdoPtr<FdoIDescribeSchema>  pDescSchemaCmd = (FdoIDescribeSchema*) connection->CreateCommand(FdoCommandType_DescribeSchema);
        FdoFeatureSchemasP                     fsc = pDescSchemaCmd->Execute();

        fsc->WriteXml( UnitTestUtil::GetOutputFileName( L"describeSchemas.xml" ) );

        printf( "Closing Connection ... \n" );
		
		// close and open again to be able to erase the table.
		connection->Close();
		connection->SetConnectionString ( userConnectString);
		connection->Open();
		// erase the used table.
		m_OdbcSetup.DestroyDataStore(connection.p, SUFFIX_TEST);
		connection->Close();
    }
    catch ( FdoException* e )
    {
        try {
            if (connection)
                connection->Close();
        }
        catch ( ... )
        {
        }
        UnitTestUtil::FailOnException( e );
    }
    catch (...)
    {
        if (connection)
            connection->Close();
        throw;
    }
}

void OdbcMySqlDescribeSchemaTest::set_provider()
{
    mInputSchema = mSchema;
    mInputData = NULL;
    UnitTestUtil::SetProvider( "OdbcMySql" );
}

#ifdef _WIN32
void OdbcSqlServerDescribeSchemaTest::set_provider()
{
    mInputSchema = mSchema;
    mInputData = NULL;
    UnitTestUtil::SetProvider( "OdbcSqlServer" );
}

void OdbcTextDescribeSchemaTest::set_provider()
{
    UnitTestUtil::SetProvider( "OdbcText" );
}

void OdbcTextDescribeSchemaTest::describe()
{
    // This unit test is for defects 751218 and 670018.

    FdoPtr<FdoIConnection> connection;
    try
    {
        connection = UnitTestUtil::GetProviderConnectionObject();
		connection->SetConnectionString(UnitTestUtil::GetConnectionString(Connection_WithDSN, ""));
        connection->Open();

        // The data source has dot '.' and hyphen '-' characters that shows up in the
        // physical schema name.  Make sure that we correctly handle it.  The 
        // provider should map it to an acceptable FDO schema name, rather than 
        // throwing an exception to complain about the offending characters.

        FdoPtr<FdoIDescribeSchema> describeSchemaCmd = (FdoIDescribeSchema*)connection->CreateCommand(FdoCommandType_DescribeSchema);
        FdoPtr<FdoFeatureSchemaCollection> schemas = describeSchemaCmd->Execute();

        FdoInt32 numSchemas = schemas->GetCount();
        CPPUNIT_ASSERT( numSchemas == 1 );

        for (int i=0; i<numSchemas; i++) {
            FdoPtr<FdoFeatureSchema> schema = schemas->GetItem(i);

            FdoString* schemaName = schema->GetName();

            FdoPtr<FdoClassCollection> classes = schema->GetClasses();

            FdoInt32 numClasses = classes->GetCount();
            CPPUNIT_ASSERT( numClasses == 1 );

            for (int j=0; j<numClasses; j++) {
                FdoPtr<FdoFeatureClass> featureClass = (FdoFeatureClass*)classes->GetItem(j);

                // analyze the feature class
                FdoString* className = featureClass->GetName();
                FdoPtr<FdoISelect> select = (FdoISelect*)connection->CreateCommand (FdoCommandType_Select);
                select->SetFeatureClassName (className);
                select->Execute();
            }
        }
    }
    catch ( FdoException* e )
    {
        try {
            if (connection)
                connection->Close();
        }
        catch ( ... )
        {
        }
        UnitTestUtil::FailOnException( e );
    }
    catch (...)
    {
        if (connection)
            connection->Close();
        throw;
    }
}

#endif
