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
 */

#ifndef RFPTESTEXAMPLE1_H
#define RFPTESTEXAMPLE1_H

#ifdef _WIN32
#pragma once
#endif

class RfpTestExample1 : public RfpTestCase
{
	FDO_CPPUNIT_DEFINE(testDescribeSchema);
	FDO_CPPUNIT_DEFINE(testSelectCommand);
	FDO_CPPUNIT_DEFINE(testSpatialContext);
	FDO_CPPUNIT_DEFINE(testSelectWithFilter1);
	FDO_CPPUNIT_DEFINE(testSelectWithFilter2);
	FDO_CPPUNIT_DEFINE(testSelectWithFilter3);
	FDO_CPPUNIT_DEFINE(testSelectWithFilter4);
	FDO_CPPUNIT_DEFINE(testSelectWithFilter5);	

	CPPUNIT_TEST_SUITE(RfpTestExample1);
	CPPUNIT_TEST(testDescribeSchema);
	CPPUNIT_TEST(testSelectCommand);
	CPPUNIT_TEST(testSpatialContext);	
	CPPUNIT_TEST(testSelectWithFilter1);	
	CPPUNIT_TEST(testSelectWithFilter2);	
	CPPUNIT_TEST(testSelectWithFilter3);	
	CPPUNIT_TEST(testSelectWithFilter4);	
	CPPUNIT_TEST(testSelectWithFilter5);	
	CPPUNIT_TEST_SUITE_END();

public:
	RfpTestExample1(void);
	~RfpTestExample1(void);

public:
	void _setUp();
	void _tearDown();

public:	
	void testDescribeSchema();
	void testSpatialContext();
	void testSelectCommand();
	void testSelectWithFilter1();
	void testSelectWithFilter2();
	void testSelectWithFilter3();
	void testSelectWithFilter4();
	void testSelectWithFilter5();
	
private:
	typedef std::vector<std::wstring> StringVector;
	void testSelectWithFilter(const FdoPtr<FdoFilter>& filter, StringVector& result, FdoString* className = L"Photo", FdoString* propertyName = L"Id",  FdoString* propertyToSelect = L"Id");

private:
	FdoPtr<FdoIConnection> m_connection;
};

#endif //RFPTESTEXAMPLE1_H
