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

#ifndef RFPTESTTHOROUGH_H
#define RFPTESTTHOROUGH_H

#ifdef _WIN32
#pragma once
#endif //_WIN32

#include "RfpTestCase.h"

class RfpTestThorough :	public RfpTestCase
{
	FDO_CPPUNIT_DEFINE(testCreateConnection);
	FDO_CPPUNIT_DEFINE(testSetConfiguration);
	FDO_CPPUNIT_DEFINE(testOpenConnection);
	FDO_CPPUNIT_DEFINE(testCloseConnection);
	FDO_CPPUNIT_DEFINE(testCapabilities);
	FDO_CPPUNIT_DEFINE(testCreateCommand);
	FDO_CPPUNIT_DEFINE(testActivateSpatialContext);
	FDO_CPPUNIT_DEFINE(testGetSpatialContexts); 
        FDO_CPPUNIT_DEFINE(testManySpatialContexts);
	FDO_CPPUNIT_DEFINE(testGetTooManySpatialContexts); 
	FDO_CPPUNIT_DEFINE(testCreateSpatialContext); 
	FDO_CPPUNIT_DEFINE(testDestroySpatialContext);
	//FDO_CPPUNIT_DEFINE(testBeginTransaction);
	//FDO_CPPUNIT_DEFINE(testGetSetConnectionTimeout);
	FDO_CPPUNIT_DEFINE(testCreateSchemaMapping);
	FDO_CPPUNIT_DEFINE(testOpenClose);
	//...

	CPPUNIT_TEST_SUITE(RfpTestThorough);
	CPPUNIT_TEST(testCreateConnection);
	CPPUNIT_TEST(testSetConfiguration);
	CPPUNIT_TEST(testOpenConnection);
	CPPUNIT_TEST(testCloseConnection);
	CPPUNIT_TEST(testCapabilities);
	CPPUNIT_TEST(testCreateCommand);
	CPPUNIT_TEST(testActivateSpatialContext);
	CPPUNIT_TEST(testGetSpatialContexts);
    	CPPUNIT_TEST(testManySpatialContexts);
	CPPUNIT_TEST(testCreateSpatialContext);
	CPPUNIT_TEST(testDestroySpatialContext);
	//CPPUNIT_TEST_EXCEPTION(testBeginTransaction, FdoException*);
	//CPPUNIT_TEST(testGetSetConnectionTimeout);
	CPPUNIT_TEST(testCreateSchemaMapping);
	CPPUNIT_TEST(testOpenClose);
	//...
	CPPUNIT_TEST_SUITE_END();

public:
	RfpTestThorough(void);
	~RfpTestThorough(void);

public:
	//overrides.
	void _setUp();
	void _tearDown();

public:
	//test cases.
	void testCreateConnection();
	void testSetConfiguration();
	void testOpenConnection();
	void testCloseConnection();
	void testCreateCommand();
	void testActivateSpatialContext();
	void testCreateSpatialContext();
        void testManySpatialContexts();
	void testDestroySpatialContext();
	void testGetSpatialContexts();
    void testGetTooManySpatialContexts();

	// test Test Cases must cause exceptions.
	void testBeginTransaction();
	void testGetSetConnectionTimeout();

	// test connection capabilities.
	void testCapabilities();
	
	// schema mapping.
	void testCreateSchemaMapping();
	
	// connection string.
	// connection state.
    void testOpenClose();
};

// helper functions to compare the contonts of two arrays.
// 
template<typename ElemType>
inline bool ArrayEqual(const ElemType* ary1, const ElemType* ary2, int len)
{
	bool found;
	for (int i=0; i<len; i++)
	{
		found = false;
		int j;
		for (j=0; j<len; j++)
		{
			if (ary1[i] == ary2[j])
			{
				found = true;
				break;
			}
		}

		if (!found && j == len)
			return false;
	}

	return true;
}

#endif //RFPTESTTHOROUGH_H
