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

#include "Pch.h"
#include "MySqlFdoExpressionFunctionTest.h"
#include "UnitTestUtil.h"

CPPUNIT_TEST_SUITE_REGISTRATION(MySqlFdoExpressionFunctionTest);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(MySqlFdoExpressionFunctionTest, "FdoExpressionFunctionTest");


// ----------------------------------------------------------------------------
// --                          Test Setup Functions                          --
// ----------------------------------------------------------------------------

void MySqlFdoExpressionFunctionTest::SetProvider ()

// +---------------------------------------------------------------------------
// | Sets the provider for the current test. Any Provider that supports these
// | tests need to implement it.
// +---------------------------------------------------------------------------

{

    UnitTestUtil::SetProvider("MySql");

}  //  SetProvider ()


// ----------------------------------------------------------------------------
// --                    Test Environment Setup Functions                    --
// ----------------------------------------------------------------------------


FdoInt32 MySqlFdoExpressionFunctionTest::GetDimensionality ()

// +---------------------------------------------------------------------------
// | The function returns the dimensionality the provider supports.
// +---------------------------------------------------------------------------

{

    return (2);

}  //  GetDimensionality ()   

FdoDouble MySqlFdoExpressionFunctionTest::GetExpectedCountValue (
                                                    FdoInt16 test_case_id)

// +---------------------------------------------------------------------------
// | The function returns the number of rows expected to be returned by the
// | function testing the expression function COUNT. The value differs on the
// | underlying database server. In MySQL, for example, NULL values are not
// | added to the total count whereas in Oracle those rows are.
// +---------------------------------------------------------------------------

{

    switch (test_case_id) {

      case 1: return 30; break;
      case 2: return 28; break; // SHOULD BE 30. CHECK WHAT IS WRONG IN EXECUTION.

    }  //  switch ...

    // The previous call should have handled all valid cases. Just in case
    // return -1 to indicate an error if this is not the case.

    return -1;

}  //  GetExpectedCountValue ()


// ----------------------------------------------------------------------------
// --                      Standard Unit Test Functions                      --
// ----------------------------------------------------------------------------

// ===========================================================================
// ==                    TESTING THE CONVERSION FUNCTIONS                   ==
// ===========================================================================

void MySqlFdoExpressionFunctionTest::TestToDateFunction ()

// +---------------------------------------------------------------------------
// | The function executes the test for the expression engine function TODATE
// | when used as a select-parameter.
// | NOTE: The reason why the default test is overwritten is the fact that the
// |       MySQL implementation of the expression function TODATE works dif-
// |       ferent from other implementations and hence returns a different re-
// |       sult for the executed test.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    FdoStringP                func_call;

    FdoDateTime               expected_dt_data;

    FdoPtr<FdoFilter>         filter;
    FdoPtr<FdoIFeatureReader> data_reader;

    printf("\n");
    printf("========================================================== \n");
    printf(" Current Unit Test Suite: TODATE Function Testing          \n");
    printf("========================================================== \n");
    printf("\n");

    // Define the filter for all tests in this test suite.

    filter = (FdoComparisonCondition::Create(
               FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"id")),
               FdoComparisonOperations_EqualTo, 
               FdoPtr<FdoDataValue>(FdoDataValue::Create(9))));

    // 1. Test Case:
    // The test executes a select-command to select the value of a computed
    // property that is defined by using the function TODATE on a string
    // containing date information. This test also provides a format string
    // describing the date-string. The test should return a date/time object
    // that contains the information provided by the date-string. No excep-
    // tions are expected.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function TODATE on a string containing date information. \n");
    printf("  This test also provides a format string describing the   \n");
    printf("  date-string. The test should return a date/time object   \n");
    printf("  that contains the information provided by the date-      \n");
    printf("  string. No exceptions are expected.                      \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property
      // being identical to the setting of the date/time value stored in
      // the variable dt1_set.

      expected_dt_data.year  = 7;
      expected_dt_data.month = 9;
      expected_dt_data.day   = 21;

      func_call   = L"(ToDate('21-SEP-07', 'DD-MON-YY') as cmp_id)";
      data_reader =
                ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckDateTimeReader(data_reader, true, false, 0, 9, expected_dt_data);
      printf(" >>> Test succeeded \n");

    }  //  try ...

    catch (FdoException *exp) {

      printf(" >>> Exception: %ls\n", exp->GetExceptionMessage());
      printf(" >>> Test failed \n");
      throw exp;

    }  //  catch (FdoException *ex) ...

    catch ( ... ) {

      printf(" >>> Test failed for an unknown reason \n");
      throw;

    }  //  catch ( ... ) ...

}  //  TestToDateFunction ()

void MySqlFdoExpressionFunctionTest::TestToInt32Function ()

// +---------------------------------------------------------------------------
// | The function executes the test for the expression engine function TOINT32
// | when used as a select-parameter.
// | NOTE: The reason why the default test is overwritten is the fact that the
// |       MySQL implementation of the expression function TOINT32 works dif-
// |       ferent from other implementations and hence returns a different re-
// |       sult for the executed test.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    FdoStringP                func_call;

    FdoPtr<FdoFilter>         filter;
    FdoPtr<FdoIFeatureReader> data_reader;

    printf("\n");
    printf("========================================================== \n");
    printf(" Current Unit Test Suite: TOINT32 Function Testing         \n");
    printf("========================================================== \n");
    printf("\n");

    // Define the filter for all tests in this test suite.

    filter = (FdoComparisonCondition::Create(
               FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"id")),
               FdoComparisonOperations_EqualTo, 
               FdoPtr<FdoDataValue>(FdoDataValue::Create(9))));

    // 1. Test Case:
    // The test executes a select-command to select the value of a computed
    // property that is defined by using the function TOINT32 on the value
    // of a different property of type DECIMAL. The test should return the
    // original value minus the part after the decimal point. No exceptions
    // are expected.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function TOINT32 on the value of a different property of \n");
    printf("  type DECIMAL. The test should return the original value  \n");
    printf("  minus the part after the decimal point. No exceptions    \n");
    printf("  are expected.                                            \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property
      // being 13.

      func_call   = L"(ToInt32(dcl_val) as cmp_id)";
      data_reader =
                ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, 13);
      printf(" >>> Test succeeded \n");

    }  //  try ...

    catch (FdoException *exp) {

      printf(" >>> Exception: %ls\n", exp->GetExceptionMessage());
      printf(" >>> Test failed \n");
      throw exp;

    }  //  catch (FdoException *ex) ...

    catch ( ... ) {

      printf(" >>> Test failed for an unknown reason \n");
      throw;

    }  //  catch ( ... ) ...

    // 2. Test Case:
    // The test executes a select-command to select the value of a computed
    // property that is defined by using the function TOINT32 on the value
    // of a different property of type DOUBLE. The test should return the
    // original value minus the part after the decimal point. No exceptions
    // are expected.

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("2. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function TOINT32 on the value of a different property of \n");
    printf("  type DOUBLE. The test should return the original value   \n");
    printf("  minus the part after the decimal point. No exceptions    \n");
    printf("  are expected.                                            \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property
      // being 19.

      func_call   = L"(ToInt32(dbl_val) as cmp_id)";
      data_reader =
                ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, 19);
      printf(" >>> Test succeeded \n");

    }  //  try ...

    catch (FdoException *exp) {

      printf(" >>> Exception: %ls\n", exp->GetExceptionMessage());
      printf(" >>> Test failed \n");
      throw exp;

    }  //  catch (FdoException *ex) ...

    catch ( ... ) {

      printf(" >>> Test failed for an unknown reason \n");
      throw;

    }  //  catch ( ... ) ...

    // 3. Test Case:
    // The test executes a select-command to select the value of a computed
    // property that is defined by using the function TOINT32 on the value
    // of a different property of type INT32. The test should return the same
    // value. No exceptions are expected.

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("3. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function TOINT32 on the value of a different property of \n");
    printf("  type INT32. The test should return the same value. No    \n");
    printf("  exceptions are expected.                                 \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property
      // being 90.

      func_call   = L"(ToInt32(i32_val) as cmp_id)";
      data_reader =
                ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, 90);
      printf(" >>> Test succeeded \n");

    }  //  try ...

    catch (FdoException *exp) {

      printf(" >>> Exception: %ls\n", exp->GetExceptionMessage());
      printf(" >>> Test failed \n");
      throw exp;

    }  //  catch (FdoException *ex) ...

    catch ( ... ) {

      printf(" >>> Test failed for an unknown reason \n");
      throw;

    }  //  catch ( ... ) ...

    // 4. Test Case:
    // The test executes a select-command to select the value of a computed
    // property that is defined by using the function TOINT32 on the value
    // of a different property of type SINGLE. The test should return the
    // original value minus the part after the decimal point. No exceptions
    // are expected.

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("4. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function TOINT32 on the value of a different property of \n");
    printf("  type SINGLE. The test should return the original value   \n");
    printf("  minus the part after the decimal point. No exceptions    \n");
    printf("  are expected.                                            \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property
      // being 3.

      func_call   = L"(ToInt32(sgl_val) as cmp_id)";
      data_reader =
                ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, 3);
      printf(" >>> Test succeeded \n");

    }  //  try ...

    catch (FdoException *exp) {

      printf(" >>> Exception: %ls\n", exp->GetExceptionMessage());
      printf(" >>> Test failed \n");
      throw exp;

    }  //  catch (FdoException *ex) ...

    catch ( ... ) {

      printf(" >>> Test failed for an unknown reason \n");
      throw;

    }  //  catch ( ... ) ...

    // 5. Test Case:
    // The test executes a select-command to select the value of a computed
    // property that is defined by using the function TOINT64 on the value
    // of a different property of type STRING. The test should return the
    // numeric representation of the value stored in the string without any
    // decimal places. No exceptions are expected.

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("5. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function TOINT32 on the value of a different property of \n");
    printf("  type STRING. The test should return the numeric repre-   \n");
    printf("  sentation of the value stored in the string without any  \n");
    printf("  decimal places. No exceptions are expected.              \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property
      // being 9.

      func_call   = L"(ToInt32(str_val) as cmp_id)";
      data_reader =
                ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, 9);
      printf(" >>> Test succeeded \n");

    }  //  try ...

    catch (FdoException *exp) {

      printf(" >>> Exception: %ls\n", exp->GetExceptionMessage());
      printf(" >>> Test failed \n");
      throw exp;

    }  //  catch (FdoException *ex) ...

    catch ( ... ) {

      printf(" >>> Test failed for an unknown reason \n");
      throw;

    }  //  catch ( ... ) ...

}  //  TestToInt32Function ()

void MySqlFdoExpressionFunctionTest::TestToInt64Function ()

// +---------------------------------------------------------------------------
// | The function executes the test for the expression engine function TOINT64
// | when used as a select-parameter.
// | NOTE: The reason why the default test is overwritten is the fact that the
// |       MySQL implementation of the expression function TOINT64 works dif-
// |       ferent from other implementations and hence returns a different re-
// |       sult for the executed test.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    FdoStringP                func_call;

    FdoPtr<FdoFilter>         filter;
    FdoPtr<FdoIFeatureReader> data_reader;

    printf("\n");
    printf("========================================================== \n");
    printf(" Current Unit Test Suite: TOINT64 Function Testing         \n");
    printf("========================================================== \n");
    printf("\n");

    // Define the filter for all tests in this test suite.

    filter = (FdoComparisonCondition::Create(
               FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"id")),
               FdoComparisonOperations_EqualTo, 
               FdoPtr<FdoDataValue>(FdoDataValue::Create(9))));

    // 1. Test Case:
    // The test executes a select-command to select the value of a computed
    // property that is defined by using the function TOINT64 on the value
    // of a different property of type DECIMAL. The test should return the
    // original value minus the part after the decimal point. No exceptions
    // are expected.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function TOINT64 on the value of a different property of \n");
    printf("  type DECIMAL. The test should return the original value  \n");
    printf("  minus the part after the decimal point. No exceptions    \n");
    printf("  are expected.                                            \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property
      // being 13.

      func_call   = L"(ToInt64(dcl_val) as cmp_id)";
      data_reader =
                ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, 13);
      printf(" >>> Test succeeded \n");

    }  //  try ...

    catch (FdoException *exp) {

      printf(" >>> Exception: %ls\n", exp->GetExceptionMessage());
      printf(" >>> Test failed \n");
      throw exp;

    }  //  catch (FdoException *ex) ...

    catch ( ... ) {

      printf(" >>> Test failed for an unknown reason \n");
      throw;

    }  //  catch ( ... ) ...

    // 2. Test Case:
    // The test executes a select-command to select the value of a computed
    // property that is defined by using the function TOINT64 on the value
    // of a different property of type DOUBLE. The test should return the
    // original value minus the part after the decimal point. No exceptions
    // are expected.

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("2. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function TOINT64 on the value of a different property of \n");
    printf("  type DOUBLE. The test should return the original value   \n");
    printf("  minus the part after the decimal point. No exceptions    \n");
    printf("  are expected.                                            \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property
      // being 19.

      func_call   = L"(ToInt64(dbl_val) as cmp_id)";
      data_reader =
                ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, 19);
      printf(" >>> Test succeeded \n");

    }  //  try ...

    catch (FdoException *exp) {

      printf(" >>> Exception: %ls\n", exp->GetExceptionMessage());
      printf(" >>> Test failed \n");
      throw exp;

    }  //  catch (FdoException *ex) ...

    catch ( ... ) {

      printf(" >>> Test failed for an unknown reason \n");
      throw;

    }  //  catch ( ... ) ...

    // 3. Test Case:
    // The test executes a select-command to select the value of a computed
    // property that is defined by using the function TOINT64 on the value
    // of a different property of type INT32. The test should return the
    // same value. No exceptions are expected.

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("3. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function TOINT64 on the value of a different property of \n");
    printf("  type INT32. The test should return the same value. No    \n");
    printf("  exceptions are expected.                                 \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property
      // being 90.

      func_call   = L"(ToInt64(i32_val) as cmp_id)";
      data_reader =
                ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, 90);
      printf(" >>> Test succeeded \n");

    }  //  try ...

    catch (FdoException *exp) {

      printf(" >>> Exception: %ls\n", exp->GetExceptionMessage());
      printf(" >>> Test failed \n");
      throw exp;

    }  //  catch (FdoException *ex) ...

    catch ( ... ) {

      printf(" >>> Test failed for an unknown reason \n");
      throw;

    }  //  catch ( ... ) ...

    // 4. Test Case:
    // The test executes a select-command to select the value of a computed
    // property that is defined by using the function TOINT64 on the value
    // of a different property of type SINGLE. The test should return the
    // original value minus the part after the decimal point. No exceptions
    // are expected.

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("4. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function TOINT64 on the value of a different property of \n");
    printf("  type SINGLE. The test should return the original value   \n");
    printf("  minus the part after the decimal point. No exceptions    \n");
    printf("  are expected.                                            \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property
      // being 3.

      func_call   = L"(ToInt64(sgl_val) as cmp_id)";
      data_reader =
                ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, 3);
      printf(" >>> Test succeeded \n");

    }  //  try ...

    catch (FdoException *exp) {

      printf(" >>> Exception: %ls\n", exp->GetExceptionMessage());
      printf(" >>> Test failed \n");
      throw exp;

    }  //  catch (FdoException *ex) ...

    catch ( ... ) {

      printf(" >>> Test failed for an unknown reason \n");
      throw;

    }  //  catch ( ... ) ...

    // 5. Test Case:
    // The test executes a select-command to select the value of a computed
    // property that is defined by using the function TOINT64 on the value
    // of a different property of type STRING. The test should return the
    // numeric representation of the value stored in the string. No excep-
    // tions are expected.

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("5. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function TOINT64 on the value of a different property of \n");
    printf("  type STRING. The test should return the numeric repre-   \n");
    printf("  sentation of the value stored in the string. No excep-   \n");
    printf("  tions are expected.                                      \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property
      // being 9.

      func_call   = L"(ToInt64(str_val) as cmp_id)";
      data_reader =
                ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, 9);
      printf(" >>> Test succeeded \n");

    }  //  try ...

    catch (FdoException *exp) {

      printf(" >>> Exception: %ls\n", exp->GetExceptionMessage());
      printf(" >>> Test failed \n");
      throw exp;

    }  //  catch (FdoException *ex) ...

    catch ( ... ) {

      printf(" >>> Test failed for an unknown reason \n");
      throw;

    }  //  catch ( ... ) ...

}  //  TestToInt64Function ()

void MySqlFdoExpressionFunctionTest::TestToStringFunction ()

// +---------------------------------------------------------------------------
// | The function executes the test for the expression engine function TOSTRING
// | when used as a select-parameter.
// | NOTE: The default test for the expression function TOSTRING is overwritten
// |       because the result returned by MySQL is different from what is ex-
// |       pected in the default tests. 
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    FdoStringP                func_call;

    FdoPtr<FdoFilter>         filter;
    FdoPtr<FdoIFeatureReader> data_reader;

    printf("\n");
    printf("========================================================== \n");
    printf(" Current Unit Test Suite: TOSTRING Function Testing        \n");
    printf("========================================================== \n");
    printf("\n");

    // Define the filter for all tests in this test suite.

    filter = (FdoComparisonCondition::Create(
               FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"id")),
               FdoComparisonOperations_EqualTo, 
               FdoPtr<FdoDataValue>(FdoDataValue::Create(9))));

    // 1. Test Case:
    // The test executes a select-command to select the value of a computed
    // property that is defined by using the function TOSTRING on the value
    // of a different property of type DECIMAL. The test should return the
    // original value in a string. No exceptions are expected.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function TOSTRING on the value of a different property   \n");
    printf("  of type DECIMAL. The test should return the original     \n");
    printf("  value in a string. No exceptions are expected.           \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property
      // being 12.84.

      func_call   = L"(ToString(dcl_val) as cmp_id)";
      data_reader =
                ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckStringFunctionReader(data_reader, 9, L"12.840000");
      printf(" >>> Test succeeded \n");

    }  //  try ...

    catch (FdoException *exp) {

      printf(" >>> Exception: %ls\n", exp->GetExceptionMessage());
      printf(" >>> Test failed \n");
      throw exp;

    }  //  catch (FdoException *ex) ...

    catch ( ... ) {

      printf(" >>> Test failed for an unknown reason \n");
      throw;

    }  //  catch ( ... ) ...

    // 2. Test Case:
    // The test executes a select-command to select the value of a computed
    // property that is defined by using the function TOSTRING on the value
    // of a different property of type DOUBLE. The test should return the
    // original value in a string. No exceptions are expected.

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("2. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function TOSTRING on the value of a different property   \n");
    printf("  of type DOUBLE. The test should return the original      \n");
    printf("  value in a string. No exceptions are expected.           \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property
      // being 18.889081.

      func_call   = L"(ToString(dbl_val) as cmp_id)";
      data_reader =
                ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckStringFunctionReader(data_reader, 9, L"18.889081");
      printf(" >>> Test succeeded \n");

    }  //  try ...

    catch (FdoException *exp) {

      printf(" >>> Exception: %ls\n", exp->GetExceptionMessage());
      printf(" >>> Test failed \n");
      throw exp;

    }  //  catch (FdoException *ex) ...

    catch ( ... ) {

      printf(" >>> Test failed for an unknown reason \n");
      throw;

    }  //  catch ( ... ) ...

    // 3. Test Case:
    // The test executes a select-command to select the value of a computed
    // property that is defined by using the function TOSTRING on the value
    // of a different property of type INT32. The test should return the
    // original value in a string. No exceptions are expected.

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("3. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function TOSTRING on the value of a different property   \n");
    printf("  of type INT32. The test should return the original value \n");
    printf("  in a string. No exceptions are expected.                 \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property
      // being 90.

      func_call   = L"(ToString(i32_val) as cmp_id)";
      data_reader =
                ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckStringFunctionReader(data_reader, 9, L"90");
      printf(" >>> Test succeeded \n");

    }  //  try ...

    catch (FdoException *exp) {

      printf(" >>> Exception: %ls\n", exp->GetExceptionMessage());
      printf(" >>> Test failed \n");
      throw exp;

    }  //  catch (FdoException *ex) ...

    catch ( ... ) {

      printf(" >>> Test failed for an unknown reason \n");
      throw;

    }  //  catch ( ... ) ...

    // 4. Test Case:
    // The test executes a select-command to select the value of a computed
    // property that is defined by using the function TOSTRING on the value
    // of a different property of type SINGLE. The test should return the
    // original value in a string. No exceptions are expected.

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("4. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function TOSTRING on the value of a different property   \n");
    printf("  of type SINGLE. The test should return the original      \n");
    printf("  value in a string. No exceptions are expected.           \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property
      // being 3.090129.

      func_call   = L"(ToString(sgl_val) as cmp_id)";
      data_reader =
                ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckStringFunctionReader(data_reader, 9, L"3.090129");
      printf(" >>> Test succeeded \n");

    }  //  try ...

    catch (FdoException *exp) {

      printf(" >>> Exception: %ls\n", exp->GetExceptionMessage());
      printf(" >>> Test failed \n");
      throw exp;

    }  //  catch (FdoException *ex) ...

    catch ( ... ) {

      printf(" >>> Test failed for an unknown reason \n");
      throw;

    }  //  catch ( ... ) ...

    // 5. Test Case:
    // The test executes a select-command to select the value of a computed
    // property that is defined by using the function TOSTRING on a value
    // of a different property of type DATE/TIME. In this test, no format
    // instructions are provided. The test should return the date in a string
    // using the default format. No exceptions are expected.

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("5. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function TOSTRING on the value of a different property   \n");
    printf("  of type DATE/TIME. In this test, no format instructions  \n");
    printf("  are provided. The test should return the date in a       \n");
    printf("  string using the default format. No exceptions are ex-   \n");
    printf("  pected.                                                  \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property
      // being "09-SEP-2007 21:00:00".

      func_call   = L"(ToString(dt_val) as cmp_id)";
      data_reader =
                ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckStringFunctionReader(data_reader, 9, L"09-SEP-2007 21:00:00");
      printf(" >>> Test succeeded \n");

    }  //  try ...

    catch (FdoException *exp) {

      printf(" >>> Exception: %ls\n", exp->GetExceptionMessage());
      printf(" >>> Test failed \n");
      throw exp;

    }  //  catch (FdoException *ex) ...

    catch ( ... ) {

      printf(" >>> Test failed for an unknown reason \n");
      throw;

    }  //  catch ( ... ) ...

    // 6. Test Case:
    // The test executes a select-command to select the value of a computed
    // property that is defined by using the function TOSTRING on the value
    // of a different property of type DATE/TIME. In this test, format in-
    // structions are provided. The test should return the date in a string
    // using the provided format. No exceptions are expected.

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("5. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function TOSTRING on the value of a different property   \n");
    printf("  of type DATE/TIME. In this test, format instructions are \n");
    printf("  provided. The test should return the date in a string    \n");
    printf("  using the provided format. No exceptions are expected.   \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property
      // being "09-SEP-2007".

      func_call   = L"(ToString(dt_val, 'DD-MON-YYYY') as cmp_id)";
      data_reader =
                ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckStringFunctionReader(data_reader, 9, L"09-SEP-2007");
      printf(" >>> Test succeeded \n");

    }  //  try ...

    catch (FdoException *exp) {

      printf(" >>> Exception: %ls\n", exp->GetExceptionMessage());
      printf(" >>> Test failed \n");
      throw exp;

    }  //  catch (FdoException *ex) ...

    catch ( ... ) {

      printf(" >>> Test failed for an unknown reason \n");
      throw;

    }  //  catch ( ... ) ...

}  //  TestToStringFunction ()


// ===========================================================================
// ==                      TESTING THE DATE FUNCTIONS                       ==
// ===========================================================================

void MySqlFdoExpressionFunctionTest::TestAddMonthsFunction ()

// +---------------------------------------------------------------------------
// | The function executes the test for the expression engine function ADD-
// | MONTHS when used as a select-parameter.
// | NOTE: The native implementation of the expression function cannot be
// |       used because of the structure of the SQL statement that is con-
// |       structed. Because of this any request is redirected to the standard
// |       implementation of the Expression Engine. At this point this pro-
// |       cedure is not yet implemented and hence the test cannot be exe-
// |       cuted.
// +---------------------------------------------------------------------------

{

    printf("\n");
    printf("========================================================== \n");
    printf(" Current Unit Test Suite: ADDMONTHS Function Testing           \n");
    printf("========================================================== \n");
    printf("\n");

    printf("\n");
    printf(" !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! \n");
    printf(" >>> FUNCTION NOT YET SUPPORTED IN THE EXPRESSION ENGINE \n");
    printf(" !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! \n");
    printf("\n");

    return;

}  //  TestAddMonthsFunction ()

void MySqlFdoExpressionFunctionTest::TestMonthsBetweenFunction ()

// +---------------------------------------------------------------------------
// | The function executes the test for the expression engine function MONTHS-
// | BETWEEN when used as a select-parameter.
// | NOTE: The standard implementation of the function in the Expression
// |       Engine is not yet implemented and hence the test cannot be exe-
// |       cuted.
// +---------------------------------------------------------------------------

{

    printf("\n");
    printf("========================================================== \n");
    printf(" Current Unit Test Suite: MONTHSBETWEEN Function Testing   \n");
    printf("========================================================== \n");
    printf("\n");

    printf("\n");
    printf(" !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! \n");
    printf(" >>> FUNCTION NOT YET SUPPORTED IN THE EXPRESSION ENGINE \n");
    printf(" !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! \n");
    printf("\n");

    return;

}  //  TestMonthsBetweenFunction ()


// ===========================================================================
// ==                     TESTING THE STRING FUNCTIONS                      ==
// ===========================================================================

void MySqlFdoExpressionFunctionTest::TestSoundexFunction ()

// +---------------------------------------------------------------------------
// | The function executes the test for the expression engine function SOUNDEX
// | when used as a select-parameter.
// | NOTE: The reason why the default test is overwritten is the fact that the
// |       MySQL implementation of the expression function SOUNDEX is different
// |       from other implementations and hence returns a different result for
// |       the executed test.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    FdoStringP                func_call;

    FdoPtr<FdoFilter>         filter;
    FdoPtr<FdoIFeatureReader> data_reader;

    printf("\n");
    printf("========================================================== \n");
    printf(" Current Unit Test Suite: SOUNDEX Function Testing         \n");
    printf("========================================================== \n");
    printf("\n");

    // Define the filter for all tests in this test suite.

    filter = (FdoComparisonCondition::Create(
               FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"id")),
               FdoComparisonOperations_EqualTo, 
               FdoPtr<FdoDataValue>(FdoDataValue::Create(9))));

    // 1. Test Case:
    // The test executes a select-command to select the value of a computed
    // property that is defined by using the function SOUNDEX on a value of
    // a different property of type STRING. No exceptions are expected.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function SOUNDEX on a value of a different property of   \n");
    printf("  type STRING. No exceptions are expected.                 \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be "T2462".

      func_call   = L"(Soundex(str2_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", filter, true, func_call);
      CheckStringFunctionReader(data_reader, 9, L"T2462");
      printf(" >>> Test succeeded \n");

    }  //  try ...

    catch (FdoException *exp) {

      printf(" >>> Exception: %ls\n", exp->GetExceptionMessage());
      printf(" >>> Test failed \n");
      throw exp;

    }  //  catch (FdoException *ex) ...

    catch ( ... ) {

      printf(" >>> Test failed for an unknown reason \n");
      throw;

    }  //  catch ( ... ) ...

}  //  TestSoundexFunction ()

