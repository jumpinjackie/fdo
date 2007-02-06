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
#ifndef FDOPOSTGIS_CONNECTIONINFOTEST_H_INCLUDED
#define FDOPOSTGIS_CONNECTIONINFOTEST_H_INCLUDED

#include "BaseTestCase.h"

// Test 
class ConnectionInfoTest : public fdo::postgis::test::BaseTestCase
{
    CPPUNIT_TEST_SUITE(ConnectionInfoTest);
    CPPUNIT_TEST(testProviderName);
    CPPUNIT_TEST(testProviderDisplayName);
    CPPUNIT_TEST(testProviderDescription);
    CPPUNIT_TEST(testProviderVersion);
    CPPUNIT_TEST(testFeatureDataObjectsVersion);
    CPPUNIT_TEST(testConnectionProperties);
    CPPUNIT_TEST(testProviderDatastoreType);
    CPPUNIT_TEST(testDependentFileNames);
    CPPUNIT_TEST_SUITE_END();

public:

    ConnectionInfoTest();
    virtual ~ConnectionInfoTest();

    //
    // Tests
    //

    void testProviderName();
    void testProviderDisplayName();
    void testProviderDescription();
    void testProviderVersion();
    void testFeatureDataObjectsVersion();
    void testConnectionProperties();
    void testProviderDatastoreType();
    void testDependentFileNames();

};

#endif // FDOPOSTGIS_CONNECTIONINFOTEST_H_INCLUDED
