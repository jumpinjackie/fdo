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

#ifndef FDO_EXPRESSIONFUNCTIONTEST_H
#define FDO_EXPRESSIONFUNCTIONTEST_H

#include <TestCommonExpressionFunctionW.h>

// =============================================================================
// | The file contains the definition of the class FdoExpressionFunctionTest. It
// | defines the expression function tests
// =============================================================================


// ---------------------------------------------------------------------------
//                                  Constants                                 
// ---------------------------------------------------------------------------

//  DATASTORE_EXTENSION_VD:
//      The constants specifies the extension to the data store name to be
//      used when creating the data store

#ifdef _WIN32
static const wchar_t* DATASTORE_EXTENSION = L"..\\..\\TestData\\ExpressionEngineFunctionTest.SDX";
#else
#include <unistd.h>
static const wchar_t* DATASTORE_EXTENSION = L"../../TestData/ExpressionEngineFunctionTest.SDX";
#endif

class FdoExpressionFunctionTest : public TestCommonExpressionFunctionW
{

  CPPUNIT_TEST_SUB_SUITE(FdoExpressionFunctionTest, TestCommonExpressionFunctionW);
    // =========================================
    // ====         OPTIMIZER TEST          ====
    // =========================================
    CPPUNIT_TEST(RunOptimizerTest);
  CPPUNIT_TEST_SUITE_END();

public:
            FdoExpressionFunctionTest ();
    virtual ~FdoExpressionFunctionTest ();


protected:

    //  Connect:
    //      The function establishes the connection to the data store used in
    //      the executed test suites.

	void Connect ();

    //-------------------------------------------------------------------------
    //                           Unit Test Functions
    //-------------------------------------------------------------------------

    // ========================= AGGREGATE FUNCTIONS =========================

    //  TestAvgFunction:
    //      The function executes the test for the expression engine function
    //      AVG when used as a select-parameter.

    void TestAvgFunction ();

    //  TestCountFunction:
    //      The function executes the test for the expression engine function
    //      COUNT when used as a select-parameter.

    void TestCountFunction ();

    //  TestMaxFunction:
    //      The function executes the test for the expression engine function
    //      MAX when used as a select-parameter.

    void TestMaxFunction ();

    //  TestMedianFunction:
    //      The function executes the test for the expression engine function
    //      MEDIAN when used as a select-parameter.

    void TestMedianFunction ();

    //  TestMinFunction:
    //      The function executes the test for the expression engine function
    //      MIN when used as a select-parameter.

    void TestMinFunction ();

    //  TestStddevFunction:
    //      The function executes the test for the expression engine function
    //      STDDEV when used as a select-parameter.

    void TestStddevFunction ();

    //  TestSumFunction:
    //      The function executes the test for the expression engine function
    //      SUM when used as a select-parameter.

    void TestSumFunction ();


    // ======================== CONVERSION FUNCTIONS =========================

    //  TestNullValueFunction:
    //      The function executes the test for the expression engine function
    //      NULLVALUE when used as a select-parameter.

    void TestNullValueFunction ();

    //  TestToDateFunction:
    //      The function executes the test for the expression engine function
    //      TODATE when used as a select-parameter.

    void TestToDateFunction ();

    //  TestToDateFunctionAsFilter:
    //      The function executes the test for the expression engine function
    //      TODATE when used in a filter.

    void TestToDateFunctionAsFilter ();

    //  TestToDoubleFunction:
    //      The function executes the test for the expression engine function
    //      TODOUBLE when used as a select-parameter.

    void TestToDoubleFunction ();

    //  TestToDoubleFunctionAsFilter:
    //      The function executes the test for the expression engine function
    //      TODOUBLE when used in a filter.

    void TestToDoubleFunctionAsFilter ();

    //  TestToFloatFunction:
    //      The function executes the test for the expression engine function
    //      TOFLOAT when used as a select-parameter.

    void TestToFloatFunction ();

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

    //  TestCurrentDateFunction:
    //      The function executes the test for the expression engine function
    //      CURRENTDATE when used as a select parameter.

    void TestCurrentDateFunction ();

    //  TestExtractFunction:
    //      The function executes the test for the expression engine function
    //      EXTRACT when used as a select-parameter.

    void TestExtractFunction ();

    //  TestExtractToDoubleFunction:
    //      The function executes the test for the expression engine function
    //      EXTRACTTODOUBLE when used as a select-parameter.

    void TestExtractToDoubleFunction ();

    //  TestExtractToIntFunction:
    //      The function executes the test for the expression engine function
    //      EXTRACTTOINT when used as a select-parameter.

    void TestExtractToIntFunction ();

    //  TestMonthsBetweenFunction:
    //      The function executes the test for the expression engine function
    //      MONTHSBETWEEN when used as a select-parameter.

    void TestMonthsBetweenFunction ();


    // =========================== MATH FUNCTIONS ============================

    //  TestAbsFunction:
    //      The function executes the test for the expression engine function
    //      ABS when used as a select-parameter.

    void TestAbsFunction ();

    //  TestAcosFunction:
    //      The function executes the test for the expression engine function
    //      ACOS when used as a select-parameter.

    void TestAcosFunction ();

    //  TestAsinFunction:
    //      The function executes the test for the expression engine function
    //      ASIN when used as a select-parameter.

    void TestAsinFunction ();

    //  TestAtanFunction:
    //      The function executes the test for the expression engine function
    //      ATAN when used as a select-parameter.

    void TestAtanFunction ();

    //  TestAtan2Function:
    //      The function executes the test for the expression engine function
    //      ATAN2 when used as a select-parameter.

    void TestAtan2Function ();

    //  TestCosFunction:
    //      The function executes the test for the expression engine function
    //      COS when used as a select-parameter.

    void TestCosFunction ();

    //  TestExpFunction:
    //      The function executes the test for the expression engine function
    //      EXP when used as a select-parameter.

    void TestExpFunction ();

    //  TestLnFunction:
    //      The function executes the test for the expression engine function
    //      LN when used as a select-parameter.

    void TestLnFunction ();

    //  TestLogFunction:
    //      The function executes the test for the expression engine function
    //      LOG when used as a select-parameter.

    void TestLogFunction ();

    //  TestModFunction:
    //      The function executes the test for the expression engine function
    //      MOD when used as a select-parameter.

    void TestModFunction ();

    //  TestPowerFunction:
    //      The function executes the test for the expression engine function
    //      POWER when used as a select-parameter.

    void TestPowerFunction ();

    //  TestRemainderFunction:
    //      The function executes the test for the expression engine function
    //      REMAINDER when used as a select-parameter.

    void TestRemainderFunction ();

    //  TestSinFunction:
    //      The function executes the test for the expression engine function
    //      SIN when used as a select-parameter.

    void TestSinFunction ();

    //  TestSqrtFunction:
    //      The function executes the test for the expression engine function
    //      SQRT when used as a select-parameter.

    void TestSqrtFunction ();

    //  TestTanFunction:
    //      The function executes the test for the expression engine function
    //      TAN when used as a select-parameter.

    void TestTanFunction ();


    // ========================== NUMERIC FUNCTIONS ==========================

    //  TestCeilFunction:
    //      The function executes the test for the expression engine function
    //      CEIL when used as a select-parameter.

    void TestCeilFunction ();

    //  TestFloorFunction:
    //      The function executes the test for the expression engine function
    //      FLOOR when used as a select-parameter.

    void TestFloorFunction ();

    //  TestRoundFunction:
    //      The function executes the test for the expression engine function
    //      ROUND when used as a select-parameter.

    void TestRoundFunction ();

    //  TestSignFunction:
    //      The function executes the test for the expression engine function
    //      SIGN when used as a select-parameter.

    void TestSignFunction ();

    //  TestTuncFunction:
    //      The function executes the test for the expression engine function
    //      TRUNC when used as a select-parameter.

    void TestTruncFunction ();


    // ========================== STRING FUNCTIONS ===========================

    //  TestConcatFunction:
    //      The function executes the test for the expression engine function
    //      CONCAT when used as a select-parameter.

    void TestConcatFunction ();

    //  TestInstrFunction:
    //      The function executes the test for the expression engine function
    //      INSTR when used as a select-parameter.

    void TestInstrFunction ();

    //  TestInstrFunctionAsFilter:
    //      The function executes the test for the expression engine function
    //      INSTR when used in a filter.

    void TestInstrFunctionAsFilter ();

    //  TestLengthFunction:
    //      The function executes the test for the expression engine function
    //      LENGTH when used as a select-parameter.

    void TestLengthFunction ();

    //  TestLowerFunction:
    //      The function executes the test for the expression engine function
    //      LOWER when used as a select-parameter.

    void TestLowerFunction ();

    //  TestLpadFunction:
    //      The function executes the test for the expression engine function
    //      LPAD when used as a select-parameter.

    void TestLpadFunction ();

    //  TestLtrimFunction:
    //      The function executes the test for the expression engine function
    //      LTRIM when used as a select-parameter.

    void TestLtrimFunction ();

    //  TestRpadFunction:
    //      The function executes the test for the expression engine function
    //      RPAD when used as a select-parameter.

    void TestRpadFunction ();

    //  TestRtrimFunction:
    //      The function executes the test for the expression engine function
    //      RTRIM when used as a select-parameter.

    void TestRtrimFunction ();

    //  TestSoundexFunction:
    //      The function executes the test for the expression engine function
    //      SOUNDEX when used as a select-parameter.

    void TestSoundexFunction ();

    //  TestSubstrFunction:
    //      The function executes the test for the expression engine function
    //      SUBSTR when used as a select-parameter.

    void TestSubstrFunction ();

    //  TestTranslateFunction:
    //      The function executes the test for the expression engine function
    //      SUBSTR when used as a select-parameter.

    void TestTranslateFunction ();

    //  TestTrimFunction:
    //      The function executes the test for the expression engine function
    //      TRIM when used as a select-parameter.

    void TestTrimFunction ();

    //  TestUpperFunction:
    //      The function executes the test for the expression engine function
    //      UPPER when used as a select-parameter.

    void TestUpperFunction ();


    //-------------------------------------------------------------------------
    //                     Combination Unit Test Functions
    //-------------------------------------------------------------------------

    //  NumberToStringComparison:
    //      The function executes a filter on the test data set that uses the
    //      function TOSTRING on two different properties and compares the
    //      function results to determine if a data set qualifies as defined
    //      by the filter.

    void NumberToStringComparison ();

    //  RoundNumberToStringComparison:
    //      The function executes a filter on the test data set that uses the
    //      functions ROUND and TOSTRING on two different properties and
    //      compares the function results to determine if a data set qualifies
    //      as defined by the filter.

    void RoundNumberToStringComparison ();

    //-------------------------------------------------------------------------
    //                       Special Unit Test Functions
    //-------------------------------------------------------------------------

    //  ModuloOperation:
    //      The function executes a modulo operation on multiple rows. This is
    //      to verify that the returned data is accurate for each row.

    void ModuloOperation ();

    //  RunOptimizerTest:
    //      The function tests the expression optimizer for filters.

    void RunOptimizerTest();

    //  SoundexFilter:
    //      The function executes a specific test identified by QA that
    //      resulted in a MAP crash.

    void SoundexFilter ();

    //  SubstrOperation:
    //      The function executes a substring operation on multiple rows. This
    //      is to verify that the returned data is accurate for each row.

    void SubstrOperation ();

    //  ToDateOperation:
    //      The function tests the expression function TODATE as a filter
    //      when connected to a specific SDF file provided by QA. Apparently,
    //      no data is returned although at least one row qualifies.

    void ToDateOperation ();

};  //  class FdoExpressionFunctionTest

#endif
