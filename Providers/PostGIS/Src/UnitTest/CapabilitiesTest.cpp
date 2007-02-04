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
        CPPUNIT_ASSERT(true);
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
