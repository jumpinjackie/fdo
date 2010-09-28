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

#ifndef TESTDESCRIBESCHEMA_H
#define TESTDESCRIBESCHEMA_H

#ifdef WIN32
#pragma once
#endif

#include "WmsTests.h"

class WmsTestDescribeSchema : public WmsTests
{
	CPPUNIT_TEST_SUITE (WmsTestDescribeSchema);
#ifdef _FDO_RUN_INTERNAL_TEST
	CPPUNIT_TEST (testServer1);
	CPPUNIT_TEST (testInheritance1);
#endif
	CPPUNIT_TEST (testServer2);
	// This server cannot connect any more
    //CPPUNIT_TEST (testServer3);
	//CPPUNIT_TEST (testServer4);
	CPPUNIT_TEST (testServer5);
	CPPUNIT_TEST (testCeoware2);
    CPPUNIT_TEST (testKortPlandk);
	CPPUNIT_TEST (testSchemaMapping);
    // This server cannot connect any more
    //CPPUNIT_TEST (testLibCwms);
	CPPUNIT_TEST_SUITE_END ();

public:
	WmsTestDescribeSchema(void);
	~WmsTestDescribeSchema(void);

	void _setUp () {}
	void _tearDown () {}
	void testInheritance1 ();
	void testServer1 ();
	void testServer2 ();
	void testServer3 ();
	void testServer4 ();
	void testServer5 ();
	void testCeoware2 ();
	void testKortPlandk ();
	void testSchemaMapping ();
	void testLibCwms ();

private:
	// Helper to return the string presentation of the property type
	static FdoString* PropertyTypeToString (FdoPropertyType propType);
	// Helper to test and print out schema
	void TestServer (FdoString* featureServer, FdoInt32 numClasses = 0);
	// Helper to test interitance
	void TestInheritance (FdoString* featureServer);
};


#endif//TESTDESCRIBESCHEMA_H
