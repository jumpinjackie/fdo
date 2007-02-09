//
// Copyright (C) 2007 Refractions Research, Inc. 
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
//
#include "Pch.h"
#include "ConnectionTest.h"
#include "TestConfig.h"

CPPUNIT_TEST_SUITE_REGISTRATION(ConnectionTest);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ConnectionTest, "ConnectionTest");

using namespace fdo::postgis::test;

ConnectionTest::ConnectionTest() : mConn(NULL)
{
}

ConnectionTest::~ConnectionTest()
{
}

void ConnectionTest::setUp()
{
}

void ConnectionTest::tearDown()
{
    FDO_SAFE_RELEASE(mConn.p);

    mConn = NULL;
    FdoPtr<IConnectionManager> mgr = FdoFeatureAccessManager::GetConnectionManager();
    mgr->FreeLibrary(gTestConfig.getProviderFullName());
}

void ConnectionTest::testGetConnectionManager()
{
    FdoPtr<IConnectionManager> mgr = FdoFeatureAccessManager::GetConnectionManager();
    CPPUNIT_ASSERT_MESSAGE("Connection manager is NULL", NULL != mgr);
}

void ConnectionTest::testCreateConnection()
{
    FdoPtr<IConnectionManager> mgr = FdoFeatureAccessManager::GetConnectionManager();
    FdoPtr<FdoIConnection> tmpConn =
        mgr->CreateConnection(gTestConfig.getProviderFullName());
    CPPUNIT_ASSERT_MESSAGE("Connection is NULL", NULL != tmpConn);
}

void ConnectionTest::testConnect()
{  
    FdoPtr<IConnectionManager> mgr = FdoFeatureAccessManager::GetConnectionManager();
    mConn = mgr->CreateConnection(gTestConfig.getProviderFullName());
    CPPUNIT_ASSERT_MESSAGE("Connection is NULL", NULL != mConn);
    CPPUNIT_ASSERT_MESSAGE("Connection not closed",
        FdoConnectionState_Closed == mConn->GetConnectionState());

    try
    {
        mConn->SetConnectionString(gTestConfig.getConnectionString());
        CPPUNIT_ASSERT_MESSAGE("Connection not closed",
            FdoConnectionState_Closed == mConn->GetConnectionState());
        
        FdoConnectionState state = mConn->Open();
        CPPUNIT_ASSERT_MESSAGE("Connection not open",
            FdoConnectionState_Open == state);
        CPPUNIT_ASSERT_MESSAGE("Connection not open",
            FdoConnectionState_Open == mConn->GetConnectionState());

        mConn->Close();
        CPPUNIT_ASSERT_MESSAGE("Connection not closed",
            FdoConnectionState_Closed == mConn->GetConnectionState());
    }
    catch (FdoException* e)
    {
        BaseTestCase::fail(e);
    }
    catch (std::runtime_error& e)
    {
        BaseTestCase::fail(e);
    }
}

