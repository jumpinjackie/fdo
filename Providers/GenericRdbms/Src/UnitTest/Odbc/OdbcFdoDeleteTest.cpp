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
#include "UnitTestUtil.h"

CPPUNIT_TEST_SUITE_REGISTRATION( OdbcOracleFdoDeleteTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcOracleFdoDeleteTest, "FdoDeleteTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcOracleFdoDeleteTest, "OdbcOracleFdoDeleteTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcOracleFdoDeleteTest, "OdbcOracleTests");

#ifdef _WIN32
CPPUNIT_TEST_SUITE_REGISTRATION( OdbcAccessFdoDeleteTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcAccessFdoDeleteTest, "FdoDeleteTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcAccessFdoDeleteTest, "OdbcAccessFdoDeleteTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcAccessFdoDeleteTest, "OdbcAccessTests");
#endif

void OdbcOracleFdoDeleteTest::set_provider()
{
	UnitTestUtil::SetProvider( "OdbcOracle" );
}

void OdbcOracleFdoDeleteTest::FeatureDelete ()
{
#if 0
    // Test not converted for Oracle yet.

    FdoPtr<FdoIConnection> connection;

    try
    {
        connection = UnitTestUtil::GetProviderConnectionObject();
        connection->SetConnectionString(UnitTestUtil::GetConnectionString());
        connection->Open();

        // Create a filter
        FdoPtr<FdoIdentifier> pIdent = FdoIdentifier::Create(L"NAME"); 
        FdoPtr<FdoDataValue> pParam = FdoDataValue::Create(L"FakeName"); 

        FdoPtr<FdoComparisonCondition> filter = FdoComparisonCondition::Create(pIdent, FdoComparisonOperations_EqualTo, pParam); 

        int count;

	    FdoPtr<FdoIDelete> deleteCommand = (FdoIDelete *) connection->CreateCommand(FdoCommandType_Delete);

        deleteCommand->SetFeatureClassName(L"TABLE1");
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
#endif
}

#ifdef _WIN32
void OdbcAccessFdoDeleteTest::set_provider()
{
	UnitTestUtil::SetProvider( "OdbcAccess" );
}

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
