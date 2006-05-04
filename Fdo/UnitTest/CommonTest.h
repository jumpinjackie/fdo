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

#ifdef _WIN32
#pragma once
#endif

#ifndef CPP_UNIT_COMMONTEST_H
#define CPP_UNIT_COMMONTEST_H

// Test cases for FDOCommon
class CommonTest : public CppUnit::TestCase
{
    FDO_CPPUNIT_DEFINE(testTypes);
    FDO_CPPUNIT_DEFINE(testExceptions);
    FDO_CPPUNIT_DEFINE(testArrays);
    FDO_CPPUNIT_DEFINE(testCollections);
    FDO_CPPUNIT_DEFINE(testOutput);
    FDO_CPPUNIT_DEFINE( testString );
    FDO_CPPUNIT_DEFINE( testStringCollection );
    FDO_CPPUNIT_DEFINE( testDictionary );
    FDO_CPPUNIT_DEFINE( testVector );

    CPPUNIT_TEST_SUITE(CommonTest);
    CPPUNIT_TEST(testTypes);
    CPPUNIT_TEST(testExceptions);
    CPPUNIT_TEST(testArrays);
    CPPUNIT_TEST(testCollections);
    CPPUNIT_TEST(testOutput);
    CPPUNIT_TEST(testString);
    CPPUNIT_TEST(testStringCollection);
    CPPUNIT_TEST(testDictionary);
    CPPUNIT_TEST(testVector);
	//TODO: add other tests for IDisposable, other?
    CPPUNIT_TEST_SUITE_END();

public:
    CommonTest(void);
    virtual ~CommonTest(void);
    void setUp ();

protected:
    void testTypes();
    void testExceptions();
    void testArrays();
    void testCollections();
    void testOutput();
    void testString();
    void testStringCollection();
	void testDictionary();
    // Tests FdoVector, which is not yet part of the API but
    // is used in FDO.dll.
	void testVector();
};

#endif

