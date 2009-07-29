// Copyright (C) 2004-2006  Autodesk, Inc.
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
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

#ifndef UPDATE_TEST_H_
#define UPDATE_TEST_H_

#ifdef _WIN32
#pragma once
#endif

#include "Fdo.h"
#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>
#include <map>

/* 
 * Tests spatial contexts.
 * This is just an initial version to test a defect regression.
 * More tests would need to be added to provide full coverage.
 */

class UpdateTest : public CppUnit::TestCase
{
  CPPUNIT_TEST_SUITE( UpdateTest );
  CPPUNIT_TEST( TestSimpleUpdate );
  CPPUNIT_TEST( TestSpatialUpdate );
  CPPUNIT_TEST( TestComplexUpdate );
  CPPUNIT_TEST_SUITE_END();

public:
    UpdateTest(void);
    virtual ~UpdateTest(void);
    void setUp ();
	void tearDown ();

    void TestSimpleUpdate ();
    void TestSpatialUpdate ();
    void TestComplexUpdate ();
};

#endif
