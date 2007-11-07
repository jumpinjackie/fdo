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

    // Test Case Setup:
    // The following retrieves the geometry data for all objects and calcu-
    // lates the spatial extent. This is later used to cross-check the result
    // returned by the expression function call.

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

    // 1. Test Case:
    // The test executes a select-aggregate command to select the value of a
    // computed property that is defined by using the function SPATIALEXTENTS
    // on all the values of a different property of type GEOMETRY. No excep-
    // tions are expected.

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

    // 2. Test Case:
    // The test executes a select-aggregate command to select the value of a
    // computed property that is defined by using the function SPATIALEXTENTS
    // on all the values of a different property of type GEOMETRY where the
    // function name differs from the expected function name ('SpAtIaLeXtEnTs'
    // rather than 'SpatialExtents'). No exceptions are expected.

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

    // 2. Test Case:
    // The test executes a select-command to select the value of a computed
    // property that is defined by using the function TODATE on a string
    // containing date information. This test also provides a format string
    // describing the date-string where the function name differs from the
    // expected function name ('ToDaTe' rather than 'ToDate'). The test should
    // return a date/time object that contains the information provided by the
    // date-string. No exceptions are expected.

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
    printf("\n");

    // Define the filter for all tests in this test suite.

    filter = (FdoComparisonCondition::Create(
               FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"id")),
               FdoComparisonOperations_EqualTo, 
               FdoPtr<FdoDataValue>(FdoDataValue::Create(9))));

    // 6. Test Case:
    // The test executes a select-command to select the value of a computed
    // property that is defined by using the function TOINT32 on the value
    // of a different property of type DECIMAL where the function name dif-
    // fers from the expected function name ('ToInT32' rather than 'ToInt32').
    // The test should return the original value minus the part after the
    // decimal point. No exceptions are expected.

    printf("---------------------------------------------------------- \n");
    printf("2. Test Case:                                              \n");
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

    // 6. Test Case:
    // The test executes a select-command to select the value of a computed
    // property that is defined by using the function TOINT64 on the value
    // of a different property of type DECIMAL where the function name dif-
    // fers from the expected function name ('ToInT64' rather than 'ToInt64').
    // The test should return the original value minus the part after the
    // decimal point. No exceptions are expected.

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
      CheckReaderString(data_reader, 9, L"12.840000");
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
      CheckReaderString(data_reader, 9, L"18.889081");
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
      CheckReaderString(data_reader, 9, L"3.090129");
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

    // 6. Test Case:
    // The test executes a select-command to select the value of a computed
    // property that is defined by using the function TOSTRING on the value
    // of a different property of type DATE/TIME. In this test, format in-
    // structions are provided. The test should return the date in a string
    // using the provided format. No exceptions are expected.

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

    // 7. Test Case:
    // The test executes a select-command to select the value of a computed
    // property that is defined by using the function TOSTRING on the value
    // of a different property of type DECIMAL where the function name dif-
    // fers from the expected function name ('ToStRiNg' rather than 'To-
    // String'). The test should return the original value in a string. No
    // exceptions are expected.

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
      CheckReaderString(data_reader, 9, L"12.840000");
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

    // 2. Test Case:
    // The test executes a select-command to select the value of a computed
    // property that is defined by using the function SOUNDEX on a value of
    // a different property of type STRING where the function name differs
    // from the expected function name ('SoUnDeX' rather than 'Soundex'). No
    // exceptions are expected.

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

