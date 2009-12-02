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

#ifndef CPP_UNIT_EXPRESSIONTEST_H
#define CPP_UNIT_EXPRESSIONTEST_H

// Test cases for Expressions.
class ExpressionTest : public CppUnit::TestCase
{
    FDO_CPPUNIT_DEFINE(testBinaryExpression);
    FDO_CPPUNIT_DEFINE(testDataValue);
    FDO_CPPUNIT_DEFINE(testFunction);
    FDO_CPPUNIT_DEFINE(testGeometryValue);
    FDO_CPPUNIT_DEFINE(testIdentifier);
    FDO_CPPUNIT_DEFINE(testParameter);
    FDO_CPPUNIT_DEFINE(testUnaryExpression);
    FDO_CPPUNIT_DEFINE(testComputedIdentifier);

    CPPUNIT_TEST_SUITE(ExpressionTest);
//TODO:RAA need valid FGF    CPPUNIT_TEST(testExpressionTree);
    CPPUNIT_TEST(testBinaryExpression);
    CPPUNIT_TEST(testDataValue);
    CPPUNIT_TEST(testFunction);
    CPPUNIT_TEST(testGeometryValue);
    CPPUNIT_TEST(testIdentifier);
    CPPUNIT_TEST(testParameter);
    CPPUNIT_TEST(testUnaryExpression);
    CPPUNIT_TEST(testComputedIdentifier);
    CPPUNIT_TEST_SUITE_END();

public:
    ExpressionTest(void);
    virtual ~ExpressionTest(void);
    void setUp ();

protected:
    void testExpressionTree();
    void testBinaryExpression();
    void testDataValue();
    void testFunction();
    void testGeometryValue();
    void testIdentifier();
    void testParameter();
    void testUnaryExpression();
    void testComputedIdentifier();
};

#endif

