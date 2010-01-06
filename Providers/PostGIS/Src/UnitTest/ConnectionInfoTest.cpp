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
#include "ConnectionInfoTest.h"
#include "BaseTestCase.h"
#include "TestConfig.h"
using namespace fdo::postgis::test;
#include <iostream>

CPPUNIT_TEST_SUITE_REGISTRATION(ConnectionInfoTest);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ConnectionInfoTest, "ConnectionInfoTest");

ConnectionInfoTest::ConnectionInfoTest()
{
}

ConnectionInfoTest::~ConnectionInfoTest()
{
}

void ConnectionInfoTest::testProviderName()
{
    try
    {
        FdoPtr<FdoIConnection> conn = GetConnection();
        FdoPtr<FdoIConnectionInfo> info = conn->GetConnectionInfo();

        CPPUNIT_ASSERT_EQUAL(gTestConfig.getProviderName(),
            FdoStringP(info->GetProviderName()));
    }
    catch (FdoException* ex)
    {
        BaseTestCase::fail(ex);
    }
}

void ConnectionInfoTest::testProviderVersion()
{
    try
    {
        FdoPtr<FdoIConnection> conn = GetConnection();
        FdoPtr<FdoIConnectionInfo> info = conn->GetConnectionInfo();

        CPPUNIT_ASSERT_EQUAL(gTestConfig.getProviderVersion(),
            FdoStringP(info->GetProviderVersion()));
    }
    catch (FdoException* ex)
    {
        BaseTestCase::fail(ex);
    }
}

void ConnectionInfoTest::testFeatureDataObjectsVersion()
{
    try
    {
        FdoPtr<FdoIConnection> conn = GetConnection();
        FdoPtr<FdoIConnectionInfo> info = conn->GetConnectionInfo();

        CPPUNIT_ASSERT_EQUAL(gTestConfig.getFdoVersion(),
            FdoStringP(info->GetFeatureDataObjectsVersion()));
    }
    catch (FdoException* ex)
    {
        BaseTestCase::fail(ex);
    }
}

void ConnectionInfoTest::testConnectionProperties()
{
    try
    {
        FdoPtr<FdoIConnection> conn = GetConnection();
        conn->SetConnectionString(L"service=mydb@name.domain.net:2345;username=root;password=secret;datastore=myfdo");
        FdoPtr<FdoIConnectionInfo> info = conn->GetConnectionInfo();
        FdoPtr<FdoIConnectionPropertyDictionary> dict = 
            info->GetConnectionProperties();

        FdoInt32 size = 0;
        FdoString** names = dict->GetPropertyNames(size);
        CPPUNIT_ASSERT(NULL != names);

        for (FdoInt32 i = 0; i < size; i++)
        {
            FdoStringP name(names[i]);

            if (name == L"service")
            {
                CPPUNIT_ASSERT_MESSAGE("Incorrect service",
                    FdoStringP(L"mydb@name.domain.net:2345") == dict->GetProperty(name));
            }
            else if (name == L"username")
            {
                CPPUNIT_ASSERT_MESSAGE("Incorrect username",
                    FdoStringP(L"root") == dict->GetProperty(name));
            }
            else if (name == L"password")
            {
                CPPUNIT_ASSERT_MESSAGE("Incorrect password",
                    FdoStringP(L"secret") == dict->GetProperty(name));
            }
            else if (name == L"datastore")
            {
                CPPUNIT_ASSERT_MESSAGE("Incorrect datastore",
                    FdoStringP(L"myfdo") == dict->GetProperty(name));
            }
        }
    }
    catch (FdoException* ex)
    {
        BaseTestCase::fail(ex);
    }
}

void ConnectionInfoTest::testProviderDatastoreType()
{
    try
    {
        FdoPtr<FdoIConnection> conn = GetConnection();
        FdoPtr<FdoIConnectionInfo> info = conn->GetConnectionInfo();

        CPPUNIT_ASSERT_EQUAL_MESSAGE("Incorrect datastore type",
            FdoProviderDatastoreType_DatabaseServer,
            info->GetProviderDatastoreType());
    }
    catch (FdoException* ex)
    {
        BaseTestCase::fail(ex);
    }
}

void ConnectionInfoTest::testDependentFileNames()
{
    try
    {
        FdoPtr<FdoIConnection> conn = GetConnection();
        FdoPtr<FdoIConnectionInfo> info = conn->GetConnectionInfo();

        CPPUNIT_ASSERT_MESSAGE("Expected empty list of dependant files",
            NULL == info->GetDependentFileNames());
    }
    catch (FdoException* ex)
    {
        BaseTestCase::fail(ex);
    }
}
