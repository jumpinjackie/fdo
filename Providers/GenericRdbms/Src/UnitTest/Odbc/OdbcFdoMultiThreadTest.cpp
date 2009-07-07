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
#include "OdbcFdoMultiThreadTest.h"
#include "UnitTestUtil.h"
#include "OdbcBaseSetup.h"

#ifdef _WIN32
CPPUNIT_TEST_SUITE_REGISTRATION( OdbcOracleFdoMultiThreadTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcOracleFdoMultiThreadTest, "FdoMultiThreadTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcOracleFdoMultiThreadTest, "OdbcOracleFdoMultiThreadTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcOracleFdoMultiThreadTest, "OdbcOracleTests");

CPPUNIT_TEST_SUITE_REGISTRATION( OdbcAccessFdoMultiThreadTest );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcAccessFdoMultiThreadTest, "FdoMultiThreadTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcAccessFdoMultiThreadTest, "OdbcAccessFdoMultiThreadTest");
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( OdbcAccessFdoMultiThreadTest, "OdbcAccessTests");
#endif

bool OdbcOracleFdoMultiThreadTest::m_SetupDone = false;

void OdbcOracleFdoMultiThreadTest::set_provider()
{
	UnitTestUtil::SetProvider( "OdbcOracle" );
}


#ifdef _WIN32
DWORD WINAPI StartOdbcQuery(LPVOID lpParameter)
#else
void * StartOdbcQuery(void * lpParameter)
#endif
{
    ConnectInfo  *cnInfo = (ConnectInfo*)lpParameter;
    
	FdoIConnection* mConnection = cnInfo->mConn;
	if( mConnection != NULL )
    {
        try
        {
            FdoPtr<FdoISelect> selectCmd = (FdoISelect*)mConnection->CreateCommand(FdoCommandType_Select);

            // must set the feature class name
            FdoStringP fcn = L"TABLE1";
            selectCmd->SetFeatureClassName(fcn);

            // execute the command
            FdoPtr<FdoIFeatureReader> reader = selectCmd->Execute();

            FdoPtr<FdoClassDefinition> classDef = reader->GetClassDefinition();
            CPPUNIT_ASSERT_MESSAGE("Class should not have IsComputed=true", !classDef->GetIsComputed());
            FdoFeatureSchemaP pSchema =  classDef->GetFeatureSchema(); 

            // read through all the features
            int numFeatures = 0;
            while (reader->ReadNext())
            {
                numFeatures++;
                UnitTestUtil::ProcessFeature(reader);
            }

            printf("Thread(%d):   %i feature(s) read\n",cnInfo->connectionId, numFeatures);

            // close the reader
            reader->Close();
        }
        catch (FdoException* e)
        {
            cnInfo->msgException = e->GetExceptionMessage();
            e->Release();
        }
    }
    return 0;
}

void OdbcOracleFdoMultiThreadTest::QueryTest()
{
    FdoPtr<FdoIConnection> connection = GetConnection();
    if( connection != NULL )
        UnitTestUtil::CreateTable1( connection );

    FunctionInfo funcInfo;

    funcInfo.Function1 = StartOdbcQuery;
    funcInfo.Function2 = StartOdbcQuery;

    StartTest( &funcInfo );
}

FdoIConnection * OdbcOracleFdoMultiThreadTest::GetConnection()
{
    FdoIConnection* mConnection = UnitTestUtil::GetProviderConnectionObject();
	// setup first
    if (!m_SetupDone)
    {
	    OdbcBaseSetup pOdbcSetup(DataBaseType_Oracle);
	    mConnection->SetConnectionString ( UnitTestUtil::GetConnectionString(Connection_OraSetup) );
	    mConnection->Open();
	    pOdbcSetup.CreateDataStore(mConnection, L"");
	    mConnection->Close();
        m_SetupDone = true;
    }
    // open with DSN
    mConnection->SetConnectionString(UnitTestUtil::GetConnectionString(Connection_WithDSN));
    mConnection->Open();
	return mConnection;
}

#ifdef _WIN32
void OdbcAccessFdoMultiThreadTest::set_provider()
{
	UnitTestUtil::SetProvider( "OdbcAccess" );
}

void OdbcAccessFdoMultiThreadTest::QueryTest()
{
#if 0
    // Hard-to-track occasional failures are happening.  They may originate
    // in the Microsoft ODBC Driver.
    CPPUNIT_FAIL("OdbcAccessFdoMultiThreadTest::QueryTest disable");
#else
    FunctionInfo funcInfo;

    funcInfo.Function1 = StartOdbcQuery;
    funcInfo.Function2 = StartOdbcQuery;

    StartTest( &funcInfo );
#endif
}

FdoIConnection * OdbcAccessFdoMultiThreadTest::GetConnection()
{
    FdoIConnection* mConnection = UnitTestUtil::GetProviderConnectionObject();
	mConnection->SetConnectionString(UnitTestUtil::GetConnectionString(Connection_WithDSN));
    mConnection->Open();
	return mConnection;
}

#endif
