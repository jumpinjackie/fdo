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

#ifndef _NLSTEST_H_
#define _NLSTEST_H_

#ifdef _WIN32
#pragma once
#endif

#include "Fdo.h"
#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>
#include <map>

/* 
 * Tests classes with non-ASCII7 names.
 */

class NlsTest : public CppUnit::TestCase
{
  CPPUNIT_TEST_SUITE( NlsTest );
  CPPUNIT_TEST( Test1 );
  CPPUNIT_TEST( Test2 );
  CPPUNIT_TEST_SUITE_END();

public:
    NlsTest(void);
    virtual ~NlsTest(void);
    void setUp ();
	void tearDown ();

    // Test without reconnecting between create schema, insert features, select features
    void Test1 ();
    // Test with reconnecting between create schema, insert features, select features
    void Test2();

private:
    // Runs the test.
    void DoTest( bool reconnect );

    // Initiates the schema for the SDF file to created
    FdoFeatureSchemaP InitSchema ( FdoIConnection* connection );
    FdoFeatureClassP InitFeatureClass( FdoStringP className );

    // Adds features to the SDF file
    void AddFeatures( FdoIConnection* connection, FdoFeatureSchemaCollection* schemas, FdoStringP className );
    void AddFeature( FdoIInsert* insertCommand, FdoInt32 idx );

    // Selects and verifies features from the SDF file. 
    void SelectFeatures( FdoIConnection* connection, FdoStringP className, bool isSpatial = false );

    // Open Connection, optionally re-creating the SDF file
    FdoIConnection* openConnection( bool re_create );

    // General functions for non-ASCII7 class names.
    FdoStringP GetClass1Name();
    FdoStringP GetClass2Name();
};

#endif

