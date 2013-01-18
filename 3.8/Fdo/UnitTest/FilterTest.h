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

#ifndef CPP_UNIT_FILTERTEST_H
#define CPP_UNIT_FILTERTEST_H

// Test cases for Filters
class FilterTest : public CppUnit::TestCase
{
    FDO_CPPUNIT_DEFINE(testBinaryLogicalOperator);
    FDO_CPPUNIT_DEFINE(testComparisonCondition);
    FDO_CPPUNIT_DEFINE(testDistanceCondition);
    FDO_CPPUNIT_DEFINE(testInCondition);
    FDO_CPPUNIT_DEFINE(testNullCondition);
    FDO_CPPUNIT_DEFINE(testSpatialCondition);
	FDO_CPPUNIT_DEFINE(testConditionRoundtrip);
    FDO_CPPUNIT_DEFINE(testUnaryLogicalOperator);
    FDO_CPPUNIT_DEFINE(testComputedIdentifier);

	CPPUNIT_TEST_SUITE(FilterTest);
//TODO:RAA need valid FGF    CPPUNIT_TEST(testFilterTree);
    CPPUNIT_TEST(testBinaryLogicalOperator);
    CPPUNIT_TEST(testComparisonCondition);
    CPPUNIT_TEST(testDistanceCondition);
    CPPUNIT_TEST(testInCondition);
    CPPUNIT_TEST(testNullCondition);
    CPPUNIT_TEST(testSpatialCondition);
	CPPUNIT_TEST(testConditionRoundtrip);
    CPPUNIT_TEST(testUnaryLogicalOperator);
    CPPUNIT_TEST(testComputedIdentifier);
    CPPUNIT_TEST_SUITE_END();

public:
    FilterTest(void);
    virtual ~FilterTest(void);
    void setUp ();

protected:
    void testFilterTree();
    void testBinaryLogicalOperator();
    void testComparisonCondition();
    void testDistanceCondition();
    void testInCondition();
    void testNullCondition();
	void testConditionRoundtrip();
    void testSpatialCondition();
    void testUnaryLogicalOperator();
    void testComputedIdentifier();

	void doRoundtrip( char *testName, FdoFilter *filter);
};

#endif

