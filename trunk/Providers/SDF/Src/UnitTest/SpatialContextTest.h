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

#ifndef _SPATIALCONTEXTTEST_H_
#define _SPATIALCONTEXTTEST_H_

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

class SpatialContextTest : public CppUnit::TestCase
{
  CPPUNIT_TEST_SUITE( SpatialContextTest );
  CPPUNIT_TEST( TestFdo309 );
  CPPUNIT_TEST_SUITE_END();

public:
    SpatialContextTest(void);
    virtual ~SpatialContextTest(void);
    void setUp ();
	void tearDown ();

    // Tests FDO309 fix regression.
    void TestFdo309 ();

private:
    // Runs the test.
    void DoTest( bool reconnect );

    // Initiates the schema for the SDF file to created
    FdoFeatureSchemaP InitSchema ( FdoIConnection* connection );
    FdoFeatureClassP InitFeatureClass( FdoStringP className ) ;
    FdoClassP InitClass( FdoStringP className );
};

#endif

