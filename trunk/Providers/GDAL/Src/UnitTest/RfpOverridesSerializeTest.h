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

#ifndef RFPOVERRIDESSERIALIZE_H
#define RFPOVERRIDESSERIALIZE_H

#ifdef _WIN32
#pragma once
#endif

class RfpOverridesSerializeTest : public RfpTestCase
{
	FDO_CPPUNIT_DEFINE(testSerialize1);
	FDO_CPPUNIT_DEFINE(testSimple);
	FDO_CPPUNIT_DEFINE(testSetConfiguration);

	CPPUNIT_TEST_SUITE(RfpOverridesSerializeTest);
	CPPUNIT_TEST(testSerialize1);
    CPPUNIT_TEST(testSimple);
    CPPUNIT_TEST(testSetConfiguration);
	CPPUNIT_TEST_SUITE_END();

public:
	RfpOverridesSerializeTest(void);
	virtual ~RfpOverridesSerializeTest(void);

public:
	void _setUp();
	void _tearDown();

public:	
	void testSerialize1();
    void testSimple();
    void testSetConfiguration();

private:
    void TestBaseProperties(FdoPhysicalElementMapping* elementMapping);
    void TestConfiguration(FdoGrfpPhysicalSchemaMapping* schemaMapping);
    void Stream2File(FdoIoStream* stream, FdoString* fileName);
};


#endif
