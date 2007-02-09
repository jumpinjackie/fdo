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
#ifndef FDOPOSTGIS_CAPABILITIESTEST_H_INCLUDED
#define FDOPOSTGIS_CAPABILITIESTEST_H_INCLUDED

#include "BaseTestCase.h"

// Test 
class CapabilitiesTest : public fdo::postgis::test::BaseTestCase
{
    CPPUNIT_TEST_SUITE(CapabilitiesTest);
    CPPUNIT_TEST(testConnectionCapabilities);
    CPPUNIT_TEST(testCommandsCapabilities);
    CPPUNIT_TEST(testSchemaCapabilities);
    //CPPUNIT_TEST(testFilterCapabilities);
    //CPPUNIT_TEST(testExpressionCapabilities);
    //CPPUNIT_TEST(testGeometryCapabilities);
    //CPPUNIT_TEST(testTopologyCapabilities);
    CPPUNIT_TEST_SUITE_END();

public:

    CapabilitiesTest();
    virtual ~CapabilitiesTest();

    //
    // Tests
    //

    void testConnectionCapabilities();
    void testCommandsCapabilities();
    void testSchemaCapabilities();
    void testFilterCapabilities();
    void testExpressionCapabilities();
    void testGeometryCapabilities();
    void testTopologyCapabilities();
};

#endif // FDOPOSTGIS_CAPABILITIESTEST_H_INCLUDED
