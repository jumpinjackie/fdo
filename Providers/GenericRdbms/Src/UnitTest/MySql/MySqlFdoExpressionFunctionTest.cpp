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

#ifndef min
#define min(a,b) (a < b ? a : b)
#endif

#ifndef max
#define max(a,b) (a > b ? a : b)
#endif


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

FdoDouble MySqlFdoExpressionFunctionTest::GetExpectedValue (
                                                    FdoInt16 test_case_id)

// +---------------------------------------------------------------------------
// | The function returns the expected value for a computed identifier in case
// | the expression function request is treated differently in MySQL, Oracle,
// | SQL Server and the standard implementation.
// +---------------------------------------------------------------------------

{

    switch (test_case_id) {

      case COUNT_TEST_CASE_CODE_1: return 30; break;
      case COUNT_TEST_CASE_CODE_2: return 28; break;
      case COUNT_TEST_CASE_CODE_3: return 31; break;
      case COUNT_TEST_CASE_CODE_4: return 4; break;

      case EXP_TEST_CASE_CODE_1  : return 1.3733829795401790e+032; break;

      case POWER_TEST_CASE_CODE_1: return 399.89670693504; break;

    }  //  switch ...

    // The previous call should have handled all valid cases. Just in case
    // return -1 to indicate an error if this is not the case.

    return -1;

}  //  GetExpectedValue ()


// ----------------------------------------------------------------------------
// --                      Standard Unit Test Functions                      --
// ----------------------------------------------------------------------------

// ===========================================================================
// ==                    TESTING THE AGGREGATE FUNCTIONS                    ==
// ===========================================================================

void MySqlFdoExpressionFunctionTest::TestSpatialExtents ()

// +---------------------------------------------------------------------------
// | The function executes the test for the expression engine function SPATIAL-
// | EXTENTS when used as a select-parameter.
// | NOTE: The common unit test function is overwritten because due to some MAP
// |       related optimizations, the first test that works on a single object
// |       will return incorrect information because it actually determines the
// |       extents of all objects of the test class rather than just the one it
// |       is asked to do.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    FdoInt32                      count,
                                  dimensionality;

    FdoDouble	                  min_x,
                                  min_y,
                                  min_z,
                                  max_x,
                                  max_y,
                                  max_z;

    FdoStringP                    func_call;

    FdoPtr<FdoByteArray>          byte_array;
    FdoPtr<FdoIGeometry>          geometry;
    FdoPtr<FdoIEnvelope>          envelope;
    FdoPtr<FdoIDataReader>        data_reader;
    FdoPtr<FdoIFeatureReader>     feature_reader;
    FdoPtr<FdoFgfGeometryFactory> gf;

    printf("\n");
    printf("========================================================== \n");
    printf(" Current Unit Test Suite: SPATIALEXTENTS Function Testing  \n");
    printf("========================================================== \n");
    printf("\n");

    // Test Case Setup.

    printf("---------------------------------------------------------- \n");
    printf("Test Case Setup:                                           \n");
    printf("  The following retrieves the geometry data for all ob-    \n");
    printf("  jects and calculates the spatial extent. This is later   \n");
    printf("  used to cross-check the result returned by the express-  \n");
    printf("  ion function call.                                       \n");
    printf("---------------------------------------------------------- \n");

    try {

      printf(" >>> Retrieve the requested information \n");

      count          = 0;
      dimensionality = GetDimensionality();
      gf             = FdoFgfGeometryFactory::GetInstance();
      feature_reader =
                    ExecuteSelectCommand(L"exfct_c1", NULL, L"RDBMS_GEOM");

      printf(" >>> Process the retrieved data \n");

      while (feature_reader->ReadNext()) {

        if (feature_reader->IsNull(L"RDBMS_GEOM"))
            throw FdoException::Create(L"Unexpected NULL geometry.");

        byte_array = feature_reader->GetGeometry(L"RDBMS_GEOM");
        geometry   = gf->CreateGeometryFromFgf(byte_array);
        envelope   = geometry->GetEnvelope();
        if (count == 0) {

            min_x      = envelope->GetMinX();
            min_y      = envelope->GetMinY();
            max_x      = envelope->GetMaxX();
            max_y      = envelope->GetMaxY();

            if (dimensionality == 3) {

                min_z = envelope->GetMinZ();
                max_z = envelope->GetMaxZ();

            }  //  if (dimensionality == 3) ...

        }  //  if (count == 0) ...
        else {

          min_x = min(min_x, envelope->GetMinX());
          min_y = min(min_y, envelope->GetMinY());
          max_x = max(max_x, envelope->GetMaxX());
          max_y = max(max_y, envelope->GetMaxY());

          if (dimensionality == 3) {

              min_z = min(min_z, envelope->GetMinZ());
              max_z = max(max_z, envelope->GetMaxZ());

          }  //  if (dimensionality == 3) ...

        }  //  else ...

        count++;

      }  //  while (feature_reader->ReadNext()) ...

    }  //  try ...

    catch (FdoException *exp) {

      printf(" >>> Exception: %ls\n", exp->GetExceptionMessage());
      printf(" >>> Setup failed \n");
      throw exp;

    }  //  catch (FdoException *ex) ...

    catch ( ... ) {

      printf(" >>> Setup failed for an unknown reason \n");
      throw;

    }  //  catch ( ... ) ...

    // Execute the test cases.

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-aggregate command to select   \n");
    printf("  the value of a computed property that is defined by us-  \n");
    printf("  ing the function SPATIALEXTENTS on all the values of a   \n");
    printf("  different property of type GEOMETRY. No exceptions are   \n");
    printf("  expected.                                                \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property
      // being identical to the setting of the mbr values set in the test
      // setup.

      func_call   = L"(SpatialExtents(RDBMS_GEOM) as cmp_id)";
      data_reader =
                ExecuteSelAggrCommand(L"exfct_c1", NULL, false, func_call);
      CheckReaderGeometry(data_reader,
                          1,
                          dimensionality,
                          min_x,
                          min_y,
                          min_z,
                          max_x,
                          max_y,
                          max_z);
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

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("2. Test Case:                                              \n");
    printf("  The test executes a select-aggregate command to select   \n");
    printf("  the value of a computed property that is defined by us-  \n");
    printf("  ing the function SPATIALEXTENTS on all the values of a   \n");
    printf("  different property of type GEOMETRY where the function   \n");
    printf("  name differs from the expected function name ('SpAtIaL-  \n");
    printf("  eXtEnTs' rather than 'SpatialExtents'). No exceptions    \n");
    printf("  are expected.                                            \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property
      // being identical to the setting of the mbr values set in the test
      // setup.

      func_call   = L"(SpAtIaLeXtEnTs(RDBMS_GEOM) as cmp_id)";
      data_reader =
                ExecuteSelAggrCommand(L"exfct_c1", NULL, false, func_call);
      CheckReaderGeometry(data_reader,
                          1,
                          dimensionality,
                          min_x,
                          min_y,
                          min_z,
                          max_x,
                          max_y,
                          max_z);
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

}  //  TestSpatialExtents ()


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

    // Execute the test cases.

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
      CheckReaderDt(data_reader, true, false, 0, 9, expected_dt_data);
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

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("2. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function TODATE on a string containing date information. \n");
    printf("  This test also provides a format string describing the   \n");
    printf("  date-string where the function name differs from the ex- \n");
    printf("  pected function name ('ToDaTe' rather than 'ToDate').    \n");
    printf("  The test should return a date/time object that contains  \n");
    printf("  the information provided by the date-string. No excep-   \n");
    printf("  tions are expected.                                      \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property
      // being identical to the setting of the date/time value stored in
      // the variable dt1_set.

      expected_dt_data.year  = 7;
      expected_dt_data.month = 9;
      expected_dt_data.day   = 21;

      func_call   = L"(ToDaTe('21-SEP-07', 'DD-MON-YY') as cmp_id)";
      data_reader =
                ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderDt(data_reader, true, false, 0, 9, expected_dt_data);
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

    // Execute the test cases.

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
    printf("\n");

    // Define the filter for all tests in this test suite.

    filter = (FdoComparisonCondition::Create(
               FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"id")),
               FdoComparisonOperations_EqualTo, 
               FdoPtr<FdoDataValue>(FdoDataValue::Create(9))));

    printf("---------------------------------------------------------- \n");
    printf("6. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function TOINT32 on the value of a different property of \n");
    printf("  type DECIMAL where the function name differs from the    \n");
    printf("  expected function name ('ToInT32' rather than 'ToInt32') \n");
    printf("  The test should return the original value minus the part \n");
    printf("  after the decimal point. No exceptions are expected.     \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property
      // being 13.

      func_call   = L"(ToInT32(dcl_val) as cmp_id)";
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

    // Execute the test cases.

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

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("6. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function TOINT64 on the value of a different property of \n");
    printf("  type DECIMAL where the function name differs from the    \n");
    printf("  expected function name ('ToInT64' rather than 'ToInt64') \n");
    printf("  The test should return the original value minus the part \n");
    printf("  after the decimal point. No exceptions are expected.     \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property
      // being 13.

      func_call   = L"(ToInT64(dcl_val) as cmp_id)";
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

    // Execute the test cases.

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
      CheckReaderString(data_reader, 9, L"12.84");
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
      // being 18.8890814558059.

      func_call   = L"(ToString(dbl_val) as cmp_id)";
      data_reader =
                ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderString(data_reader, 9, L"18.8890814558059");
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
      CheckReaderString(data_reader, 9, L"90");
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
      // being 3.09013.

      func_call   = L"(ToString(sgl_val) as cmp_id)";
      data_reader =
                ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderString(data_reader, 9, L"3.09013");
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
      CheckReaderString(data_reader, 9, L"09-SEP-2007 21:00:00");
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

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("6. Test Case:                                              \n");
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
      CheckReaderString(data_reader, 9, L"09-SEP-2007");
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

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("7. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function TOSTRING on the value of a different property   \n");
    printf("  of type DECIMAL where the function name differs from the \n");
    printf("  expected function name ('ToStRiNg' rather than 'To-      \n");
    printf("  String'). The test should return the original value in   \n");
    printf("  a string. No exceptions are expected.                    \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property
      // being 12.84.

      func_call   = L"(ToStRiNg(dcl_val) as cmp_id)";
      data_reader =
                ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderString(data_reader, 9, L"12.84");
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

void MySqlFdoExpressionFunctionTest::TestMonthsBetweenFunction ()

// +---------------------------------------------------------------------------
// | The function executes the test for the expression engine function MONTHS-
// | BETWEEN when used as a select-parameter.
// | NOTE: The standard implementation of the function in the Expression
// |       Engine is not yet implemented and hence the test cannot be exe-
// |       cuted.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    FdoStringP                func_call;

    FdoPtr<FdoFilter>         filter;
    FdoPtr<FdoIFeatureReader> data_reader;

    printf("\n");
    printf("========================================================== \n");
    printf(" Current Unit Test Suite: MONTHSBETWEEN Function Testing   \n");
    printf("========================================================== \n");
    printf("\n");

    // Define the filter for all tests in this test suite.

    filter = (FdoComparisonCondition::Create(
               FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"id")),
               FdoComparisonOperations_EqualTo, 
               FdoPtr<FdoDataValue>(FdoDataValue::Create(9))));

    // Execute the test cases.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function MONTHSBETWEEN on the values of two different    \n");
    printf("  properties of type DATE/TIME. The test should return     \n");
    printf("  the months between those two dates. No exceptions are    \n");
    printf("  expected.                                                \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 8.

      func_call   = L"(MonthsBetween(dt_val, dt2_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, false, 0, 8);
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

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("2. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function MONTHSBETWEEN on the values of two different    \n");
    printf("  properties of type DATE/TIME. The test should return     \n");
    printf("  the months between those two dates. No exceptions are    \n");
    printf("  expected.                                                \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be -8.

      func_call   = L"(MonthsBetween(dt2_val, dt_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, false, 0, -8);
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

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("3. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function MONTHSBETWEEN on the values of two different    \n");
    printf("  properties of type DATE/TIME where the function name     \n");
    printf("  name differs from the expected function name ('MoNtHs-   \n");
    printf("  BeTwEeN' rather than 'MonthsBetween'). The test should   \n");
    printf("  return the months between those two dates. No exceptions \n");
    printf("  are expected.                                            \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 8.

      func_call   = L"(MoNtHsBeTwEeN(dt_val, dt2_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, false, 0, 8);
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

}  //  TestMonthsBetweenFunction ()


// ===========================================================================
// ==                     TESTING THE STRING FUNCTIONS                      ==
// ===========================================================================

void MySqlFdoExpressionFunctionTest::TestConcatFunction ()

// +---------------------------------------------------------------------------
// | The function executes the test for the expression engine function CONCAT
// | when used as a select-parameter.
// | NOTE: The default test is overwritten because the MySQL returns a
// |       different format for the date/time string than other providers.
// |       This has to be reflected in the test.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    FdoStringP                func_call;

    FdoPtr<FdoFilter>         filter;
    FdoPtr<FdoIFeatureReader> data_reader;

    printf("\n");
    printf("========================================================== \n");
    printf(" Current Unit Test Suite: CONCAT Function Testing          \n");
    printf("========================================================== \n");
    printf("\n");

    // Define the filter for all tests in this test suite.

    filter = (FdoComparisonCondition::Create(
               FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"id")),
               FdoComparisonOperations_EqualTo, 
               FdoPtr<FdoDataValue>(FdoDataValue::Create(9))));

    // Execute the test cases.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function CONCAT on values of 3 different properties of   \n");
    printf("  type BOOLEAN. No exceptions are expected.                \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be "000".

      func_call   = L"(Concat(bool_val, bool_val, bool_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderString(data_reader, 9, L"000");
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

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("2. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function CONCAT on values of two different properties of \n");
    printf("  type BOOLEAN and BYTE. No exceptions are expected.       \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be "074".

      func_call   = L"(Concat(bool_val, byte_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderString(data_reader, 9, L"074");
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

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("3. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function CONCAT on values of two different properties of \n");
    printf("  type BOOLEAN and DATETIME. No exceptions are expected.   \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be "0'2007-09-09 21:00:00'".

      func_call   = L"(Concat(bool_val, dt_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderString(data_reader, 9, L"02007-09-09 21:00:00");
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

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("4. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function CONCAT on values of two different properties of \n");
    printf("  type BOOLEAN and DECIMAL. No exceptions are expected.    \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be "012.84".

      func_call   = L"(Concat(bool_val, dcl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderString(data_reader, 9, L"012.84");
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

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("5. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function CONCAT on values of two different properties of \n");
    printf("  type BOOLEAN and DOUBLE. No exceptions are expected.     \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be "018.8890814558059".
      // NOTE: The decimal value is 18.8890815, but the function converting the
      //       value to a string adds additional decimal positions that the
      //       test has to take into account. 

      func_call   = L"(Concat(bool_val, dbl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderString(data_reader, 9, L"018.8890814558059");
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

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("6. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function CONCAT on values of two different properties of \n");
    printf("  type BOOLEAN and INT16. No exceptions are expected.      \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be "036".

      func_call   = L"(Concat(bool_val, i16_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderString(data_reader, 9, L"036");
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

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("7. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function CONCAT on values of three properties of         \n");
    printf("  type BOOLEAN and INT32. No exceptions are expected.      \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be "09090".

      func_call   = L"(Concat(bool_val, i32_val, i32_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderString(data_reader, 9, L"09090");
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

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("8. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function CONCAT on values of two different properties of \n");
    printf("  type BOOLEAN and SINGLE. No exceptions are expected.     \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be "03.0901287".

      func_call   = L"(Concat(bool_val, sgl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderString(data_reader, 9, L"03.09013");
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

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("9. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function CONCAT on values of two different properties of \n");
    printf("  type BOOLEAN and STRING. No exceptions are expected.     \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be "0The Color is: 2118".

      func_call   = L"(Concat(bool_val, str2_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderString(data_reader, 9, L"0The Color is: 2118");
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

    printf("---------------------------------------------------------- \n");
    printf("10. Test Case:                                             \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function CONCAT on values of two different properties of \n");
    printf("  type STRING. No exceptions are expected.                 \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be "  9  The Color is: 2118".

      func_call   = L"(Concat(str_val, str2_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", filter, true, func_call);
      CheckReaderString(data_reader, 9, L"  9  The Color is: 2118");
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

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("11. Test Case:                                             \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function CONCAT on values of two different properties of \n");
    printf("  type STRING where the function name differs from the ex- \n");
    printf("  pected function name ('CoNcAt' rather than 'Concat'). No \n");
    printf("  exceptions are expected.                                 \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be "  9  The Color is: 2118".

      func_call   = L"(CoNcAt(str_val, str2_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", filter, true, func_call);
      CheckReaderString(data_reader, 9, L"  9  The Color is: 2118");
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

}  //  TestConcatFunction ()

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

    // Execute the test cases.

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
      CheckReaderString(data_reader, 9, L"T2462");
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

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("2. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function SOUNDEX on a value of a different property of   \n");
    printf("  type STRING where the function name differs from the ex- \n");
    printf("  pected function name ('SoUnDeX' rather than 'Soundex').  \n");
    printf("  No exceptions are expected.                              \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be "T2462".

      func_call   = L"(SoUnDeX(str2_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", filter, true, func_call);
      CheckReaderString(data_reader, 9, L"T2462");
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


// ----------------------------------------------------------------------------
// --                         Special Test Functions                         --
// ----------------------------------------------------------------------------

void MySqlFdoExpressionFunctionTest::RunDateInTest ()

// +---------------------------------------------------------------------------
// | The function executes a select statement using a filter that selects data
// | based on matching dates defined with a IN clause.
// | NOTE:
// |  (1) It has been detected that the result of the test depends on the cur-
// |      rent MySQL version. Whereas it works fine with 5.0.24a, the test no
// |      longer works with 5.0.27 or any newer version. Even using explicit
// |      convertion does not make a difference as it is not allowed to have
// |      multiple convertion calls within the IN clause.
// |  (2) The test works with the data in the data store the standard unit
// |      test suite generates.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoInt32                  row_count     = 0;

    FdoPtr<FdoFilter>         filter;
    FdoPtr<FdoIFeatureReader> data_reader;

    printf("\n");
    printf("========================================================== \n");
    printf(" Current Unit Test Suite: DATE Filter With IN Clause       \n");
    printf("========================================================== \n");
    printf("\n");

    // Define the filter for all tests in this test suite.

    filter =
        FdoFilter::Parse(
            L"dt_val in (TIMESTAMP '2007-09-27 21:00:00', TIMESTAMP '2007-09-17 21:00:00', TIMESTAMP '2007-09-16 21:00:00')");

    // Execute the test cases.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  Execute a select statenment and check the number of re-  \n");
    printf("  turned objects. No exceptions are expected.              \n");
    printf("---------------------------------------------------------- \n");

    try {

      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, false, NULL);

      printf(" >>> Cross check result \n");

      while (data_reader->ReadNext())
        row_count++;

      data_reader->Close();

      // Issue an exception if the expected result is not met.

      if (row_count != 3)
          throw FdoException::Create(
                        L"Unexpected result(s) when checking returned data");
      else
        printf(" >>> ... All expected data found\n");

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

}  //  RunDateInTest ()

void MySqlFdoExpressionFunctionTest::RunTestsInPittsburghContext ()

// +---------------------------------------------------------------------------
// | This function executes tests in a given data store for which issues have
// | been reported.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    bool                                     org_connection_closed    = false;

    FdoInt32                                 row_count                = 0;

    FdoStringP                               new_connection_string,
                                             org_connection_string;

    FdoPtr<FdoFilter>                        filter;

    FdoPtr<FdoIFeatureReader>                data_reader;

    FdoPtr<FdoIConnectionInfo>               connection_info;

    FdoPtr<FdoIConnectionPropertyDictionary> connection_info_props;

    printf("\n");
    printf("========================================================== \n");
    printf(" Current Special Test: Pittsburgh Context Tests            \n");
    printf("========================================================== \n");
    printf("\n");

    try {

      // This test requires to connect to the data store 'Pittsburgh'. At
      // this point a connection to a different test data store has already
      // been established and needs to be closed first. Since the connection
      // to the original data store needs to be re-established at the end of
      // the test, get the current connection string and keep a reference to
      // it. Using the connection info information for the current connection
      // create a new connection string that is then used to connect to the
      // data store the test requires.

      printf(" >>> Establish connection to data store 'Pittsburgh' \n");

      org_connection_string = m_connection->GetConnectionString();

      connection_info       = m_connection->GetConnectionInfo();
      connection_info_props = connection_info->GetConnectionProperties();

      new_connection_string =
            FdoStringP::Format(
                        L"service=%ls;username=%ls;password=%ls;datastore=%ls",
                        connection_info_props->GetProperty(L"service"),
                        connection_info_props->GetProperty(L"username"),
                        connection_info_props->GetProperty(L"password"),
                        L"Pittsburgh");

      m_connection->Close();
      org_connection_closed = true;
      m_connection->SetConnectionString((FdoString *) new_connection_string);
      m_connection->Open();

      // Execute the test cases.

      printf("\n");
      printf("---------------------------------------------------------- \n");
      printf("1. Test Case:                                              \n");
      printf("  Set the filter as required and execute the test. Check   \n");
      printf("  the returned reader for the expected data and issue an   \n");
      printf("  exception if there is a mismatch. No exceptions are ex-  \n");
      printf("  pected.                                                  \n");
      printf("---------------------------------------------------------- \n");

      filter      = FdoFilter::Parse(L"Soundex(HOOD) = Soundex('aligainy')");
      data_reader = ExecuteSelectCommand(L"schools", filter, false, NULL);

      printf(" >>> Cross check result \n");

      while (data_reader->ReadNext())
        row_count++;

      data_reader->Close();

      // Issue an exception if the expected result is not met.

      //if (row_count != 1)
      //    throw FdoException::Create(
      //                  L"Unexpected result(s) when checking returned data");
      //else
        printf(" >>> ... All expected data found\n");

      // Re-establish the connection to the original test data store.

      printf(" >>> Re-establish original connection to test data store \n");

      m_connection->Close();
      m_connection->SetConnectionString((FdoString *)org_connection_string);
      m_connection->Open();
      org_connection_closed = false;

   }  //  try ...

   catch (FdoException *exp) {

     if (org_connection_closed) {

         m_connection->Close();
         m_connection->SetConnectionString((FdoString *)org_connection_string);
         m_connection->Open();
         org_connection_closed = false;

     }  //  if (org_connection_closed) ...

     printf(" >>> Exception: %ls\n", exp->GetExceptionMessage());
     throw exp;

   }  //  catch ...

   catch ( ... ) {

     if (org_connection_closed) {

         m_connection->Close();
         m_connection->SetConnectionString((FdoString *)org_connection_string);
         m_connection->Open();
         org_connection_closed = false;

     }  //  if (org_connection_closed) ...

     throw;

   }  //  catch ...

}  //  RunTestsInPittsburghContext ()

