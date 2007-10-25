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

    //  GetExpectedCountValue:
    //      The function returns the number of rows expected to be returned
    //      by the function testing the expression function COUNT. The value
    //      differs on the underlying database server. In MySQL, for example,
    //      NULL values are not added to the total count whereas in Oracle
    //      those rows are.

    FdoDouble GetExpectedCountValue (FdoInt16 test_case_id);

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

    //  TestAddMonthsFunction:
    //      The function executes the test for the expression engine function
    //      ADDMONTHS when used as a select-parameter.

    void TestAddMonthsFunction ();

    //  TestMonthsBetweenFunction:
    //      The function executes the test for the expression engine function
    //      MONTHSBETWEEN when used as a select-parameter.

    void TestMonthsBetweenFunction ();


    // ========================== STRING FUNCTIONS ===========================

    //  TestSoundexFunction:
    //      The function executes the test for the expression engine function
    //      SOUNDEX when used as a select-parameter.

    void TestSoundexFunction ();


};  //  class MySqlFdoExpressionFunctionTest

#endif
