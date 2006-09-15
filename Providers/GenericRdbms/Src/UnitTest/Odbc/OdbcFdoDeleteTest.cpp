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
#include "OdbcFdoDeleteTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION( OdbcOracleFdoDeleteTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcOracleFdoDeleteTest, "FdoDeleteTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcOracleFdoDeleteTest, "OdbcOracleFdoDeleteTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcOracleFdoDeleteTest, "OdbcOracleTests");

CPPUNIT_TEST_SUITE_REGISTRATION( OdbcMySqlFdoDeleteTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcMySqlFdoDeleteTest, "FdoDeleteTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcMySqlFdoDeleteTest, "OdbcMySqlFdoDeleteTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcMySqlFdoDeleteTest, "OdbcMySqlTests");

#ifdef _WIN32
CPPUNIT_TEST_SUITE_REGISTRATION( OdbcSqlServerFdoDeleteTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcSqlServerFdoDeleteTest, "FdoDeleteTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcSqlServerFdoDeleteTest, "OdbcSqlServerFdoDeleteTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcSqlServerFdoDeleteTest, "OdbcSqlServerTests");

CPPUNIT_TEST_SUITE_REGISTRATION( OdbcAccessFdoDeleteTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcAccessFdoDeleteTest, "FdoDeleteTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcAccessFdoDeleteTest, "OdbcAccessFdoDeleteTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcAccessFdoDeleteTest, "OdbcAccessTests");
#endif

void OdbcBaseFdoDeleteTest::setUp ()
{
    set_provider();
    connect();
}

void OdbcBaseFdoDeleteTest::tearDown ()
{
    if (mConnection != NULL)
    {
        mConnection->Close();
        mConnection = NULL;
    }
}

void OdbcBaseFdoDeleteTest::connect ()
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


void OdbcBaseFdoDeleteTest::FeatureDelete ()
{
    if (mConnection != NULL) try
    {
        // Create a filter
        FdoPtr<FdoIdentifier> pIdent = FdoIdentifier::Create(mSetup.GetPropertyNameCitiesName()); 
        FdoPtr<FdoDataValue> pParam = FdoDataValue::Create(L"Boop");

        FdoPtr<FdoComparisonCondition> filter = FdoComparisonCondition::Create(pIdent, FdoComparisonOperations_EqualTo, pParam); 

        FdoPtr<FdoIDelete> deleteCommand = (FdoIDelete *) mConnection->CreateCommand(FdoCommandType_Delete);

        deleteCommand->SetFeatureClassName(mSetup.GetClassNameCities());
        deleteCommand->SetFilter( filter );

        int count = deleteCommand->Execute();
        CPPUNIT_ASSERT( count == 1 );
    }
    catch (FdoException *ex)
    {
        CPPUNIT_FAIL (UnitTestUtil::w2a(ex->GetExceptionMessage()));
    }
}

void OdbcMySqlFdoDeleteTest::ConfigFileTest()
{
    if (mConnection != NULL) try
    {
        // Re-open the connection with a configuration document in place.
        mConnection->Close();
        FdoIoFileStreamP fileStream = FdoIoFileStream::Create(GetConfigFile2(), L"r");
        mConnection->SetConfiguration(fileStream);
        mConnection->Open();

        // Create a filter
        FdoPtr<FdoIdentifier> pIdent = FdoIdentifier::Create(L"Id"); 
        FdoPtr<FdoDataValue> pParam = FdoDataValue::Create(L"2");

        FdoPtr<FdoComparisonCondition> filter = FdoComparisonCondition::Create(pIdent, FdoComparisonOperations_EqualTo, pParam); 

        FdoPtr<FdoIDelete> deleteCommand = (FdoIDelete *) mConnection->CreateCommand(FdoCommandType_Delete);

        deleteCommand->SetFeatureClassName(L"Acdb:Polyline");
        deleteCommand->SetFilter( filter );

        int count = deleteCommand->Execute();
        CPPUNIT_ASSERT( count == 1 );

        // Set the connection back to having no configuration document.
        mConnection->Close();
        mConnection->SetConfiguration(NULL);
        mConnection->Open();
    }
    catch (FdoException *ex)
    {
        CPPUNIT_FAIL (UnitTestUtil::w2a(ex->GetExceptionMessage()));
    }
}

#ifdef _WIN32

void OdbcAccessFdoDeleteTest::FeatureDelete ()
{
    FdoPtr<FdoIConnection> connection;
	clock_t start, finish;
    try
    {
        connection = UnitTestUtil::GetProviderConnectionObject();
        connection->SetConnectionString(UnitTestUtil::GetConnectionString());
        connection->Open();

        // Create a filter
        FdoPtr<FdoIdentifier> pIdent = FdoIdentifier::Create(L"JOBTITLE"); 
        FdoPtr<FdoDataValue> pParam = FdoDataValue::Create(L"Box Filler"); 

        FdoPtr<FdoComparisonCondition> filter = FdoComparisonCondition::Create(pIdent, FdoComparisonOperations_EqualTo, pParam); 

		start = clock();
		int count;

	    FdoPtr<FdoIDelete> deleteCommand = (FdoIDelete *) connection->CreateCommand(FdoCommandType_Delete);

        deleteCommand->SetFeatureClassName(L"EMPLOYEES");
	    deleteCommand->SetFilter( filter );

        count = deleteCommand->Execute();

    }
	catch ( FdoException* e ) 
	{
		try {
			if ( connection) connection->Close(); 
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
        CPPUNIT_FAIL ("delete failed");
    }
	connection->Close();

	finish = clock();
	printf( "Elapsed: %f seconds\n", ((double)(finish - start) / CLOCKS_PER_SEC) );
}

#endif
