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
#include "CapabilitiesTest.h"
#include "BaseTestCase.h"
#include "TestConfig.h"

CPPUNIT_TEST_SUITE_REGISTRATION(CapabilitiesTest);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(CapabilitiesTest, "CapabilitiesTest");

CapabilitiesTest::CapabilitiesTest()
{
}

CapabilitiesTest::~CapabilitiesTest()
{
}

void CapabilitiesTest::testConnectionCapabilities()
{
    try
    {
        FdoPtr<FdoIConnection> conn = GetConnection();
        FdoPtr<FdoIConnectionCapabilities> cc = conn->GetConnectionCapabilities();

        CPPUNIT_ASSERT_EQUAL_MESSAGE("Incorrect threading capability",
            FdoThreadCapability_PerConnectionThreaded,
            cc->GetThreadCapability());

        FdoInt32 size = 0;
        FdoSpatialContextExtentType* sct = cc->GetSpatialContextTypes(size);
        CPPUNIT_ASSERT(NULL != sct);
        CPPUNIT_ASSERT(1 == size);
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Incorrect spatial context type",
            FdoSpatialContextExtentType_Static, sct[0]);

        FdoLockType* lkt = cc->GetLockTypes(size);
        CPPUNIT_ASSERT(NULL != lkt);
        CPPUNIT_ASSERT(1 == size);
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Incorrect lock type",
            FdoLockType_Transaction, lkt[0]);

        CPPUNIT_ASSERT_EQUAL(false, cc->SupportsLocking());
        CPPUNIT_ASSERT_EQUAL(false, cc->SupportsTimeout());
        CPPUNIT_ASSERT_EQUAL(false, cc->SupportsTransactions());
        CPPUNIT_ASSERT_EQUAL(false, cc->SupportsLongTransactions());
        CPPUNIT_ASSERT_EQUAL(true, cc->SupportsSQL());
        CPPUNIT_ASSERT_EQUAL(false, cc->SupportsConfiguration());
        CPPUNIT_ASSERT_EQUAL(true, cc->SupportsMultipleSpatialContexts());
        CPPUNIT_ASSERT_EQUAL(false, cc->SupportsCSysWKTFromCSysName());
        CPPUNIT_ASSERT_EQUAL(false, cc->SupportsWrite());
        CPPUNIT_ASSERT_EQUAL(false, cc->SupportsMultiUserWrite());
        CPPUNIT_ASSERT_EQUAL(false, cc->SupportsFlush());
    }
    catch (FdoException* ex)
    {
        BaseTestCase::fail(ex);
    }
}

void CapabilitiesTest::testCommandsCapabilities()
{
    try
    {
        CPPUNIT_ASSERT(true);
    }
    catch (FdoException* ex)
    {
        BaseTestCase::fail(ex);
    }
}

void CapabilitiesTest::testSchemaCapabilities()
{
  CPPUNIT_FAIL("Not implemented");
}

void CapabilitiesTest::testFilterCapabilities()
{
  CPPUNIT_FAIL("Not implemented");
}

void CapabilitiesTest::testExpressionCapabilities()
{
  CPPUNIT_FAIL("Not implemented");
}

void CapabilitiesTest::testGeometryCapabilities()
{
  CPPUNIT_FAIL("Not implemented");
}

void CapabilitiesTest::testTopologyCapabilities()
{
  CPPUNIT_FAIL("Not implemented");
}
