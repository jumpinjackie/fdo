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

#ifndef MYSQL_ExpressionFunctionTest_H
#define MYSQL_ExpressionFunctionTest_H

#include "FdoExpressionFunctionTest.h"

class MySqlFdoExpressionFunctionTest : public FdoExpressionFunctionTest
{

    CPPUNIT_TEST_SUB_SUITE (MySqlFdoExpressionFunctionTest, FdoExpressionFunctionTest);
    CPPUNIT_TEST_SUITE_END ();

public:

    //-------------------------------------------------------------------------
    //                       Test Environment Setup Functions
    //-------------------------------------------------------------------------

    //  GetDimensionality:
    //      The function returns the dimensionality the provider supports.

    FdoInt32 GetDimensionality ();   

    //  GetExpectedValue:
    //      The function returns the expected value for a computed identifier
    //      in case the expression function request is treated differently in
    //      MySQL, Oracle, SQL Server and the standard implementation.

    FdoDouble GetExpectedValue (FdoInt16 test_case_id);

    //  SetProvider:
    //      Sets the provider for the current test. Any Provider that supports
    //      these tests need to implement it.

    void SetProvider ();


    //-------------------------------------------------------------------------
    //                           Unit Test Functions
    //-------------------------------------------------------------------------

    // ========================= AGGREGATE FUNCTIONS =========================

    //  TestSpatialExtents:
    //      The function executes the test for the expression engine function
    //      SPATIALEXTENTS when used as a select-parameter.

    void TestSpatialExtents ();


    // ======================== CONVERSION FUNCTIONS =========================

    //  TestToDateFunction:
    //      The function executes the test for the expression engine function
    //      TODATE when used as a select-parameter.

    void TestToDateFunction ();

    //  TestToInt32Function:
    //      The function executes the test for the expression engine function
    //      TOINT32 when used as a select-parameter.

    void TestToInt32Function ();

    //  TestToInt64Function:
    //      The function executes the test for the expression engine function
    //      TOINT64 when used as a select-parameter.

    void TestToInt64Function ();

    //  TestToStringFunction:
    //      The function executes the test for the expression engine function
    //      TOSTRING when used as a select-parameter.

    void TestToStringFunction ();


    // =========================== DATE FUNCTIONS ============================

    //  TestMonthsBetweenFunction:
    //      The function executes the test for the expression engine function
    //      MONTHSBETWEEN when used as a select-parameter.

    void TestMonthsBetweenFunction ();


    // ========================== STRING FUNCTIONS ===========================

    //  TestConcatFunction:
    //      The function executes the test for the expression engine function
    //      CONCAT when used as a select-parameter.

    void TestConcatFunction ();

    //  TestSoundexFunction:
    //      The function executes the test for the expression engine function
    //      SOUNDEX when used as a select-parameter.

    void TestSoundexFunction ();


    //-------------------------------------------------------------------------
    //                         Special Test Functions
    //-------------------------------------------------------------------------

    //  RunDateInTest:
    //      The function executes a select statement using a filter that
    //      selects data based on matching dates defined with a IN clause.

    void RunDateInTest ();

    //  RunTestsInPittsburghContext:
    //      This function executes tests in a given data store for which issues
    //      have been reported.

    void RunTestsInPittsburghContext ();


};  //  class MySqlFdoExpressionFunctionTest

#endif
