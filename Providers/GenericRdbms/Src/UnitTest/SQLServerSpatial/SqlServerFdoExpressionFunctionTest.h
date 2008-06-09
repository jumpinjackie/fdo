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

#ifndef SQLSERVER_ExpressionFunctionTest_H
#define SQLSERVER_ExpressionFunctionTest_H

#include "FdoExpressionFunctionTest.h"


class SqlServerFdoExpressionFunctionTest : public FdoExpressionFunctionTest
{

    CPPUNIT_TEST_SUB_SUITE (SqlServerFdoExpressionFunctionTest, FdoExpressionFunctionTest);
    CPPUNIT_TEST_SUITE_END ();

public:

    //-------------------------------------------------------------------------
    //                       Test Environment Setup Functions
    //-------------------------------------------------------------------------

    //  GetDimensionality:
    //      The function returns the dimensionality the provider supports.

    FdoInt32 GetDimensionality ();   

    //  SetProvider:
    //      Sets the provider for the current test. Any Provider that supports
    //      these tests need to implement it.

    void SetProvider ();


    //-------------------------------------------------------------------------
    //                           Unit Test Functions
    //-------------------------------------------------------------------------

    // ======================== OPTIMIZED READER TESTS =========================

    //  TestSpatialExtents:
    //      The function executes the test for the expression engine function
    //      SPATIALEXTENTS when used as a select-parameter.

    virtual void TestSpatialExtents ();


    // ======================== CONVERSION FUNCTIONS =========================

    //  TestToDateFunction:
    //      The function executes the test for the expression engine function
    //      TODATE when used as a select-parameter.

    void TestToDateFunction ();

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
    //                       General Supporting Functions
    //-------------------------------------------------------------------------

    //  CheckReaderStddev:
    //      This function represents a special cross-check function for the
    //      expression function STDDEV. It is required because the behavior
    //      of the function for a special use-case (where the function is
    //      executed on a single row only) is different in SQL Server from
    //      the behavior in MySQL, Oracle or the standard implementation of
    //      the expression function in the Expression Engine.

    void CheckReaderStddev (FdoIDataReader *data_reader,
                            FdoDouble      expected_cmp_id_value);

    //  GetExpectedValue:
    //      The function returns the expected value for a computed identifier
    //      in case the expression function request is treated differently in
    //      MySQL, Oracle, SQL Server and the standard implementation.

    FdoDouble GetExpectedValue (FdoInt16 test_case_id);

    void CheckReaderGeometryAndCount (
                                      FdoIDataReader *data_reader,
                                      FdoInt32       expected_count,
                                      FdoDouble      expected_min_x,
                                      FdoDouble      expected_min_y,
                                      FdoDouble      expected_max_x,
                                      FdoDouble      expected_max_y);

};  //  class SqlServerFdoExpressionFunctionTest

#endif
