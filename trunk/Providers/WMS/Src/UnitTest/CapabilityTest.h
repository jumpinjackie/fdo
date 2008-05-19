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

#ifdef _WIN32
#pragma once
#endif

#ifndef WMS_CPP_UNIT_CAPABILITYTEST_H
#define WMS_CPP_UNIT_CAPABILITYTEST_H

#include "WmsTests.h"

class CapabilityTest : public WmsTests
{
  CPPUNIT_TEST_SUITE( CapabilityTest );
  CPPUNIT_TEST( TestCapabilities );
  CPPUNIT_TEST_SUITE_END();

public:
    CapabilityTest(void);
    virtual ~CapabilityTest(void);

protected:
    void TestCapabilities ();
};

#endif // WMS_CPP_UNIT_CAPABILITYTEST_H
