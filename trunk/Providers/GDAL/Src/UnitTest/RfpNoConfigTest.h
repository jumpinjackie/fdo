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

#ifndef RFPNOCONFIGTEST_H
#define RFPNOCONFIGTEST_H

#ifdef _WIN32
#pragma once
#endif //_WIN32 

class RfpNoConfigTest : public RfpTestCase
{
	FDO_CPPUNIT_DEFINE(testDescribeSchema);
	FDO_CPPUNIT_DEFINE(testSpatialContext);
	FDO_CPPUNIT_DEFINE(testSchemaOverrides);
	FDO_CPPUNIT_DEFINE(testSelect);

	CPPUNIT_TEST_SUITE(RfpNoConfigTest);
	CPPUNIT_TEST(testDescribeSchema);
	CPPUNIT_TEST(testSpatialContext);
	CPPUNIT_TEST(testSchemaOverrides);
	CPPUNIT_TEST(testSelect);
	CPPUNIT_TEST_SUITE_END();

public:
	RfpNoConfigTest(void);
	~RfpNoConfigTest(void);

public:
	void _setUp();
	void _tearDown();

//test cases
public:
	void testDescribeSchema();
	void testSpatialContext();
	void testSchemaOverrides();
	void testSelect();

private:
	FdoPtr<FdoIConnection> m_connection;

};
#endif
