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
#include "FdoExpressionFunctionTest.h"
#include "UnitTestUtil.h"


// ----------------------------------------------------------------------------
// --                            Helper functions                            --
// ----------------------------------------------------------------------------

static FdoPropertyValue *AddNewProperty (
                                FdoPropertyValueCollection *property_values,
                                FdoString                  *name )

// +---------------------------------------------------------------------------
// | The function searches for the named property. If it already exists, a
// | reference to it is returned back to the calling routine. Otherwise, a new
// | property is created, its value set and returned back to the calling pro-
// | cedure.
// +---------------------------------------------------------------------------

{

	// Declare and initialize all necessary local variables.

    FdoPropertyValue *property_value = NULL;

    // Serch for the named property. If it does not exist, create a new one
    // and set its value.

    property_value = property_values->FindItem(name);

    if (property_value == NULL) {

        property_value = FdoPropertyValue::Create();
        property_value->SetName(name);
        property_values->Add(property_value);

    }  //  if (property_value == NULL) ...
    
    return property_value;

}  //  AddNewProperty ()


// ----------------------------------------------------------------------------
// --                          Test Setup Functions                          --
// ----------------------------------------------------------------------------

FdoExpressionFunctionTest::FdoExpressionFunctionTest ()

// +---------------------------------------------------------------------------
// | The function represents the class constructor.
// +---------------------------------------------------------------------------

{

}  //  FdoExpressionFunctionTest ()

FdoExpressionFunctionTest::~FdoExpressionFunctionTest ()

// +---------------------------------------------------------------------------
// | The function represents the class destructor.
// +---------------------------------------------------------------------------

{
    m_connection = NULL;

}  //  ~FdoExpressionFunctionTest ()

void FdoExpressionFunctionTest::setUp ()

// +---------------------------------------------------------------------------
// | The function represents the class setup function.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoStringP datastore_name;

    try {

      // The following creates the data base used for the tests, opens a con-
      // nnection to it and applies a test schema.

      printf("\n");
      printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<< \n");
      printf("               EXPRESSION FUNCTION UNIT TESTS              \n");
      printf(" Status: Started                                           \n");
      printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<< \n");
      printf("\n");

      printf(" >>> Setting the provider \n");
      SetProvider(); 
      printf(" >>> Creating test database \n");
      CreateTestDataStore(DATASTORE_EXTENSION);
      printf(" >>> Establishing necessary connection \n");
      Connect();
      printf(" >>> Establishing test environment \n");
      SetupUnitTestEnvironment(m_connection);

      printf(" >>> Testing ... \n");
      printf("\n");
      printf(".......................................................... \n");
      printf("\n");

    }  //  try ...

    catch (FdoException *exp) {

      printf(" >>> Disconnecting \n");
      CloseConnection();
      printf(" >>> Removing test database \n");
      DropTestDataStore(DATASTORE_EXTENSION);
      printf(" >>> Exception: %ls\n", exp->GetExceptionMessage());

      printf("\n");
      printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<< \n");
      printf("               EXPRESSION FUNCTION UNIT TESTS              \n");
      printf(" Status: Failure in test setup                             \n");
      printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<< \n");
      printf("\n");

      throw exp;

    }  //  catch ...

    catch ( ... ) {

      printf(" >>> Disconnecting \n");
      CloseConnection();
      printf(" >>> Removing test database \n");
      DropTestDataStore(DATASTORE_EXTENSION);

      printf("\n");
      printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<< \n");
      printf("               EXPRESSION FUNCTION UNIT TESTS              \n");
      printf(" Status: Failure in test setup                             \n");
      printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<< \n");
      printf("\n");

      throw;

    }  //  catch ...

}  //  setUp ()

void FdoExpressionFunctionTest::tearDown ()

// +---------------------------------------------------------------------------
// | The function represents the class tear-down function.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    bool is_disconnected = false;

    try {

      printf("\n");
      printf(".......................................................... \n");
      printf("\n");
      printf(" >>> Testing completed \n");

      printf(" >>> Disconnecting \n");
      is_disconnected = true;
      CloseConnection();
      printf(" >>> Removing test database(s) \n");
      DropTestDataStore(DATASTORE_EXTENSION);

      printf("\n");
      printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<< \n");
      printf("               EXPRESSION FUNCTION UNIT TESTS              \n");
      printf(" Status: Tests successfully executed                       \n");
      printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<< \n");
      printf("\n");

    }  //  try ...

    catch (FdoException *exp) {

      printf(" >>> Exception: %ls\n", exp->GetExceptionMessage());

      if (!is_disconnected) {

          printf(" >>> Disconnecting \n");
          CloseConnection();
          printf(" >>> Removing test database(s) \n");
          DropTestDataStore(DATASTORE_EXTENSION);

      }  //  if (!is_disconnected) ...

      printf("\n");
      printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<< \n");
      printf("               EXPRESSION FUNCTION UNIT TESTS              \n");
      printf(" Status: Failure in test tear down                         \n");
      printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<< \n");
      printf("\n");

      throw exp;
    
    }  //  catch (FdoExcpetion ...

    catch ( ... ) {

      if (!is_disconnected) {

          printf(" >>> Disconnecting \n");
          CloseConnection();
          printf(" >>> Removing test database(s) \n");
          DropTestDataStore(DATASTORE_EXTENSION);

      }  //  if (!is_disconnected) ...

      printf("\n");
      printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<< \n");
      printf("               EXPRESSION FUNCTION UNIT TESTS              \n");
      printf(" Status: Failure in test tear down                         \n");
      printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<< \n");
      printf("\n");

      throw;

    }  //  catch ...

}  //  tearDown ()

void FdoExpressionFunctionTest::SetProvider ()

// +---------------------------------------------------------------------------
// | The function sets the provider property.
// +---------------------------------------------------------------------------

{

}  //  SetProvider ()


// ----------------------------------------------------------------------------
// --                      Standard Unit Test Functions                      --
// ----------------------------------------------------------------------------

void FdoExpressionFunctionTest::RunAllExpFctTests ()

// +---------------------------------------------------------------------------
// | The function controls the execution of all unit tests. This way, the test
// | setup is done only once.
// +---------------------------------------------------------------------------

{

    // Testing the aggregate functions.

    printf(" >>> ... Testing Aggregate Functions \n");
    printf("\n");
    TestAvgFunction();
    TestCountFunction();
    TestMaxFunction();
    TestMedianFunction();
    TestMinFunction();
    TestStddevFunction();
    TestSumFunction();

    // Testing the conversion functions.

    printf("\n");
    printf("\n");
    printf(" >>> ... Testing Conversion Functions \n");
    printf("\n");
    TestNullValueFunction();
    TestToDateFunction();
    TestToDoubleFunction();
    TestToFloatFunction();
    TestToInt32Function();
    TestToInt64Function();
    TestToStringFunction();

    // Testing the date functions.

    printf("\n");
    printf("\n");
    printf(" >>> ... Testing Date Functions \n");
    printf("\n");
    TestAddMonthsFunction();
    TestCurrentDateFunction();
    TestExtractFunction();
    TestMonthsBetweenFunction();

    // Testing the mathematical functions.

    printf("\n");
    printf("\n");
    printf(" >>> ... Testing Mathematical Functions \n");
    printf("\n");
    TestAbsFunction();
    TestAcosFunction();
    TestAsinFunction();
    TestAtanFunction();
    TestAtan2Function();
    TestCosFunction();
    TestExpFunction();
    TestLnFunction();
    TestLogFunction();
    TestModFunction();
    TestPowerFunction();
    TestRemainderFunction();
    TestSinFunction();
    TestSqrtFunction();
    TestTanFunction();

    // Testing the numeric functions.

    printf("\n");
    printf("\n");
    printf(" >>> ... Testing Numeric Functions \n");
    printf("\n");
    TestCeilFunction();
    TestFloorFunction();
    TestRoundFunction();
    TestSignFunction();
    TestTruncFunction();

    // Testing the string functions.

    printf("\n");
    printf("\n");
    printf(" >>> ... Testing String Functions \n");
    printf("\n");
    TestConcatFunction();
    TestInstrFunction();
    TestLengthFunction();
    TestLowerFunction();
    TestLpadFunction();
    TestLtrimFunction();
    TestRpadFunction();
    TestRtrimFunction();
    TestSoundexFunction();
    TestSubstrFunction();
    TestTranslateFunction();
    TestTrimFunction();
    TestUpperFunction();

}  //  RunAllExpFctTests ()

// ===========================================================================
// ==                    TESTING THE AGGREGATE FUNCTIONS                    ==
// ===========================================================================

void FdoExpressionFunctionTest::TestAvgFunction ()

// +---------------------------------------------------------------------------
// | The function executes the test for the expression engine function AVG
// | when used as a select-parameter.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    FdoStringP             func_call;

    FdoPtr<FdoIDataReader> data_reader;

    printf("\n");
    printf("========================================================== \n");
    printf(" Current Unit Test Suite: AVG Function Testing             \n");
    printf("========================================================== \n");
    printf("\n");

    // 1. Test Case:
    // The test executes a select-aggregate command to select the value of a
    // computed property that is defined by using the function AVG on the
    // value of a different property of type DECIMAL. No exceptions are ex-
    // pected.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-aggregate command to select   \n");
    printf("  the value of a computed property that is defined by us-  \n");
    printf("  ing the function AVG on the value of a different pro-    \n");
    printf("  perty of type DECIMAL. No exceptions are expected.       \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property being
      // 21.404516129032299. 

      func_call   = L"(Avg(dcl_val) as cmp_id)";
      data_reader = ExecuteSelAggrCommand(
                                        L"exfct_c1", NULL, false, func_call);
      CheckReader(data_reader, false, 0, 21.404516129032299);
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
    // computed property that is defined by using the function AVG on the
    // value of a different property of type DOUBLE. No exceptions are ex-
    // pected.

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("2. Test Case:                                              \n");
    printf("  The test executes a select-aggregate command to select   \n");
    printf("  the value of a computed property that is defined by us-  \n");
    printf("  ing the function AVG on the value of a different pro-    \n");
    printf("  perty of type DOUBLE. No exceptions are expected.        \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property being
      // 21.404516129032299. 

      func_call   = L"(Avg(dbl_val) as cmp_id)";
      data_reader = ExecuteSelAggrCommand(
                                        L"exfct_c1", NULL, false, func_call);
      CheckReader(data_reader, false, 0, 31.481802426343197);
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
    // The test executes a select-aggregate command to select the value of a
    // computed property that is defined by using the function AVG on the
    // value of a different property of type INT16. No exceptions are ex-
    // pected.

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("3. Test Case:                                              \n");
    printf("  The test executes a select-aggregate command to select   \n");
    printf("  the value of a computed property that is defined by us-  \n");
    printf("  ing the function AVG on the value of a different pro-    \n");
    printf("  perty of type INT16. No exceptions are expected.         \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property being
      // 60. 

      func_call   = L"(Avg(i16_val) as cmp_id)";
      data_reader = ExecuteSelAggrCommand(
                                        L"exfct_c1", NULL, false, func_call);
      CheckReader(data_reader, false, 0, 60);
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
    // The test executes a select-aggregate command to select the value of a
    // computed property that is defined by using the function AVG on the
    // value of a different property of type SINGLE. No exceptions are ex-
    // pected.

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("4. Test Case:                                              \n");
    printf("  The test executes a select-aggregate command to select   \n");
    printf("  the value of a computed property that is defined by us-  \n");
    printf("  ing the function AVG on the value of a different pro-    \n");
    printf("  perty of type SINGLE. No exceptions are expected.        \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property being
      // 5.1502145845161298. 

      func_call   = L"(Avg(sgl_val) as cmp_id)";
      data_reader = ExecuteSelAggrCommand(
                                        L"exfct_c1", NULL, false, func_call);
      CheckReader(data_reader, false, 0, 5.1502145845161298);
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
    // The test executes a select-aggregate command to select the value of a
    // computed property that is defined by using the function AVG on the
    // value of a different property of type SINGLE. This time, the test re-
    // quests the average on all distinct values only. No exceptions are ex-
    // pected.

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("5. Test Case:                                              \n");
    printf("  The test executes a select-aggregate command to select   \n");
    printf("  the value of a computed property that is defined by us-  \n");
    printf("  ing the function AVG on the value of a different pro-    \n");
    printf("  perty of type SINGLE. This time, the test requests the   \n");
    printf("  average on all distinct values only. No exceptions are   \n");
    printf("  expected.                                                \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property being
      // 5.1502145845161298. 

      func_call   = L"(Avg('distinct', sgl_val) as cmp_id)";
      data_reader = ExecuteSelAggrCommand(
                                        L"exfct_c1", NULL, false, func_call);
      CheckReader(data_reader, false, 0, 5.1502145845161298);
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

}  //  TestAvgFunction ()

void FdoExpressionFunctionTest::TestCountFunction ()

// +---------------------------------------------------------------------------
// | The function executes the test for the expression engine function COUNT
// | when used as a select-parameter.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    FdoDouble              expected_count = -1;

    FdoStringP             func_call;

    FdoPtr<FdoIDataReader> data_reader;

    printf("\n");
    printf("========================================================== \n");
    printf(" Current Unit Test Suite: COUNT Function Testing           \n");
    printf("========================================================== \n");
    printf("\n");

    // 1. Test Case:
    // The test executes a select-aggregate command to select the value of a
    // computed property that is defined by using the function COUNT on the
    // value of a different property of type DECIMAL. No exceptions are ex-
    // pected.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-aggregate command to select   \n");
    printf("  the value of a computed property that is defined by us-  \n");
    printf("  ing the function COUNT on the value of a different pro-  \n");
    printf("  perty of type DECIMAL. No exceptions are expected.       \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property being
      // 30 or 31 depending what database system is executed. 

      expected_count = GetExpectedCountValue(1);

      func_call   = L"(Count(dcl_val) as cmp_id)";
      data_reader = ExecuteSelAggrCommand(
                                        L"exfct_c1", NULL, false, func_call);
      CheckReader(data_reader, false, 0, expected_count);
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
    // computed property that is defined by using the function COUNT on the
    // value of a different property of type DATE/TIME. This time, the test
    // requests the count on all distinct values only. No exceptions are ex-
    // pected.

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("2. Test Case:                                              \n");
    printf("  The test executes a select-aggregate command to select   \n");
    printf("  the value of a computed property that is defined by us-  \n");
    printf("  ing the function COUNT on the value of a different pro-  \n");
    printf("  perty of type DATE/TIME. This time, the test requests    \n");
    printf("  the count on all distinct values only. No exceptions are \n");
    printf("  expected.                                                \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property to
      // be 30 or 31 depending what database system is executed. 

      expected_count = GetExpectedCountValue(2);

      func_call   = L"(Count('distinct', dt_val) as cmp_id)";
      data_reader = ExecuteSelAggrCommand(
                                        L"exfct_c1", NULL, false, func_call);
      CheckReader(data_reader, false, 0, expected_count);
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

}  //  TestCountFunction ()

void FdoExpressionFunctionTest::TestMaxFunction ()

// +---------------------------------------------------------------------------
// | The function executes the test for the expression engine function MAX
// | when used as a select-parameter.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    FdoStringP             func_call;

    FdoDateTime            expected_dt_data;

    FdoPtr<FdoIDataReader> data_reader;

    printf("\n");
    printf("========================================================== \n");
    printf(" Current Unit Test Suite: MAX Function Testing             \n");
    printf("========================================================== \n");
    printf("\n");

    // 1. Test Case:
    // The test executes a select-aggregate command to select the value of a
    // computed property that is defined by using the function MAX on the
    // value of a different property of type DECIMAL. No exceptions are ex-
    // pected.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-aggregate command to select   \n");
    printf("  the value of a computed property that is defined by us-  \n");
    printf("  ing the function MAX on the value of a different pro-    \n");
    printf("  perty of type DECIMAL. No exceptions are expected.       \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property being
      // 42.81. 

      func_call   = L"(Max(dcl_val) as cmp_id)";
      data_reader = ExecuteSelAggrCommand(
                                        L"exfct_c1", NULL, false, func_call);
      CheckReader(data_reader, false, 0, 42.81);
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
    // computed property that is defined by using the function MAX on the
    // value of a different property of type DATE/TIME. This time, the test
    // requests the calculation of the maximum value on all distinct values
    // only. No exceptions are expected.

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("2. Test Case:                                              \n");
    printf("  The test executes a select-aggregate command to select   \n");
    printf("  the value of a computed property that is defined by us-  \n");
    printf("  ing the function MAX on the value of a different pro-    \n");
    printf("  perty of type DATE/TIME. This time, the test requests    \n");
    printf("  the calculation of the maximum value on all distinct     \n");
    printf("  values only. No exceptions are expected.                 \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property to
      // indicate September 29th 2007. 

      expected_dt_data.year  = 2007;
      expected_dt_data.month = 9;
      expected_dt_data.day   = 29;

      func_call   = L"(Max('distinct', dt_val) as cmp_id)";
      data_reader = ExecuteSelAggrCommand(
                                        L"exfct_c1", NULL, false, func_call);
      CheckDateTimeReader(data_reader, false, false, 0, 0, expected_dt_data);
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

}  //  TestMaxFunction ()

void FdoExpressionFunctionTest::TestMedianFunction ()

// +---------------------------------------------------------------------------
// | The function executes the test for the expression engine function MEDIAN
// | when used as a select-parameter.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    FdoStringP             func_call;

    FdoPtr<FdoIDataReader> data_reader;

    printf("\n");
    printf("========================================================== \n");
    printf(" Current Unit Test Suite: MEDIAN Function Testing          \n");
    printf("========================================================== \n");
    printf("\n");

    // 1. Test Case:
    // The test executes a select-aggregate command to select the value of a
    // computed property that is defined by using the function MEDIAN on the
    // value of a different property of type DECIMAL. No exceptions are ex-
    // pected.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-aggregate command to select   \n");
    printf("  the value of a computed property that is defined by us-  \n");
    printf("  ing the function MEDIAN on the value of a different pro- \n");
    printf("  perty of type DECIMAL. No exceptions are expected.       \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property being
      // 21.4. 

      func_call   = L"(Median(dcl_val) as cmp_id)";
      data_reader = ExecuteSelAggrCommand(
                                        L"exfct_c1", NULL, false, func_call);
      CheckReader(data_reader, false, 0, 21.4);
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

}  //  TestMedianFunction ()

void FdoExpressionFunctionTest::TestMinFunction ()

// +---------------------------------------------------------------------------
// | The function executes the test for the expression engine function MIN
// | when used as a select-parameter.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    FdoStringP             func_call;

    FdoDateTime            expected_dt_data;

    FdoPtr<FdoIDataReader> data_reader;

    printf("\n");
    printf("========================================================== \n");
    printf(" Current Unit Test Suite: MIN Function Testing             \n");
    printf("========================================================== \n");
    printf("\n");

    // 1. Test Case:
    // The test executes a select-aggregate command to select the value of a
    // computed property that is defined by using the function MIN on the
    // value of a different property of type DECIMAL. No exceptions are ex-
    // pected.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-aggregate command to select   \n");
    printf("  the value of a computed property that is defined by us-  \n");
    printf("  ing the function MIN on the value of a different pro-    \n");
    printf("  perty of type DECIMAL. No exceptions are expected.       \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property being
      // 0. 

      func_call   = L"(Min(dcl_val) as cmp_id)";
      data_reader = ExecuteSelAggrCommand(
                                        L"exfct_c1", NULL, false, func_call);
      CheckReader(data_reader, false, 0, 0);
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
    // computed property that is defined by using the function MIN on the
    // value of a different property of type DATE/TIME. This time, the test
    // requests the calculation of the maximum value on all distinct values
    // only. No exceptions are expected.

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("2. Test Case:                                              \n");
    printf("  The test executes a select-aggregate command to select   \n");
    printf("  the value of a computed property that is defined by us-  \n");
    printf("  ing the function MIN on the value of a different pro-    \n");
    printf("  perty of type DATE/TIME. This time, the test requests    \n");
    printf("  the calculation of the minimum value on all distinct     \n");
    printf("  values only. No exceptions are expected.                 \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property to
      // indicate September 1st 2007. 

      expected_dt_data.year  = 2007;
      expected_dt_data.month = 9;
      expected_dt_data.day   = 1;

      func_call   = L"(Min('distinct', dt_val) as cmp_id)";
      data_reader = ExecuteSelAggrCommand(
                                        L"exfct_c1", NULL, false, func_call);
      CheckDateTimeReader(data_reader, false, false, 0, 0, expected_dt_data);
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

}  //  TestMinFunction ()

void FdoExpressionFunctionTest::TestStddevFunction ()

// +---------------------------------------------------------------------------
// | The function executes the test for the expression engine function STDDEV
// | when used as a select-parameter.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    FdoStringP             func_call;

    FdoPtr<FdoIDataReader> data_reader;

    printf("\n");
    printf("========================================================== \n");
    printf(" Current Unit Test Suite: STDDEV Function Testing          \n");
    printf("========================================================== \n");
    printf("\n");

    // 1. Test Case:
    // The test executes a select-aggregate command to select the value of a
    // computed property that is defined by using the function STDDEV on the
    // value of a different property of type DECIMAL. No exceptions are ex-
    // pected.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-aggregate command to select   \n");
    printf("  the value of a computed property that is defined by us-  \n");
    printf("  ing the function STDDEV on the value of a different pro- \n");
    printf("  perty of type DECIMAL. No exceptions are expected.       \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property being
      // 12.9744572. 

      func_call   = L"(Stddev(dcl_val) as cmp_id)";
      data_reader = ExecuteSelAggrCommand(
                                        L"exfct_c1", NULL, false, func_call);
      CheckReader(data_reader, false, 0, 12.9744572);
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
    // computed property that is defined by using the function STDDEV on the
    // value of a different property of type DECIMAL. In this case the re-
    // quest is to work on distinct values only. No exceptions are expected.

    printf("---------------------------------------------------------- \n");
    printf("2. Test Case:                                              \n");
    printf("  The test executes a select-aggregate command to select   \n");
    printf("  the value of a computed property that is defined by us-  \n");
    printf("  ing the function STDDEV on the value of a different pro- \n");
    printf("  perty of type DECIMAL. In this case the request is to    \n");
    printf("  work on distinct values only. No exceptions are expected \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property being
      // 12.9744572. 

      func_call   = L"(Stddev('distinct', dcl_val) as cmp_id)";
      data_reader = ExecuteSelAggrCommand(
                                        L"exfct_c1", NULL, false, func_call);
      CheckReader(data_reader, false, 0, 12.9744572);
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

}  //  TestStddevFunction ()

void FdoExpressionFunctionTest::TestSumFunction ()

// +---------------------------------------------------------------------------
// | The function executes the test for the expression engine function SUM when
// | used as a select-parameter.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    FdoStringP             func_call;

    FdoPtr<FdoIDataReader> data_reader;

    printf("\n");
    printf("========================================================== \n");
    printf(" Current Unit Test Suite: SUM Function Testing             \n");
    printf("========================================================== \n");
    printf("\n");

    // 1. Test Case:
    // The test executes a select-aggregate command to select the value of a
    // computed property that is defined by using the function SUM on the
    // value of a different property of type DECIMAL. No exceptions are ex-
    // pected.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-aggregate command to select   \n");
    printf("  the value of a computed property that is defined by us-  \n");
    printf("  ing the function SUM on the value of a different pro-    \n");
    printf("  perty of type DECIMAL. No exceptions are expected.       \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property being
      // 633.57. 

      func_call   = L"(Sum(dcl_val) as cmp_id)";
      data_reader = ExecuteSelAggrCommand(
                                        L"exfct_c1", NULL, false, func_call);
      CheckReader(data_reader, false, 0, 633.57);
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
    printf("  ing the function SUM on the value of a different pro-    \n");
    printf("  perty of type INT32. This time, the test requests the    \n");
    printf("  calculation of the sum on all distinct values only. No   \n");
    printf("  exceptions are expected.                                 \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property to
      // be 4650. 

      func_call   = L"(Sum('distinct', i32_val) as cmp_id)";
      data_reader = ExecuteSelAggrCommand(
                                        L"exfct_c1", NULL, false, func_call);
      CheckReader(data_reader, false, 0, 4650);
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

}  //  TestSumFunction ()


// ===========================================================================
// ==                    TESTING THE CONVERSION FUNCTIONS                   ==
// ===========================================================================

void FdoExpressionFunctionTest::TestNullValueFunction ()

// +---------------------------------------------------------------------------
// | The function executes the test for the expression engine function NULL-
// | VALUE when used as a select-parameter.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    FdoStringP                func_call;

    FdoDateTime               dt1_set,
                              dt2_set;

    FdoPtr<FdoFilter>         filter1,
                              filter2;
    FdoPtr<FdoIFeatureReader> data_reader;

    printf("\n");
    printf("========================================================== \n");
    printf(" Current Unit Test Suite: NULLVALUE Function Testing       \n");
    printf("========================================================== \n");
    printf("\n");

    // Define the filter for all tests in this test suite.

    filter1 = (FdoComparisonCondition::Create(
                FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"id")),
                FdoComparisonOperations_EqualTo, 
                FdoPtr<FdoDataValue>(FdoDataValue::Create(21))));

    filter2 = (FdoComparisonCondition::Create(
                FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"id")),
                FdoComparisonOperations_EqualTo, 
                FdoPtr<FdoDataValue>(FdoDataValue::Create(22))));

    // Some of the tests in this test suits deal with the processing of date/
    // time data. To ensure propper execution at all time, get the date/time
    // information that is set for the rows used in the following test.

    printf("---------------------------------------------------------- \n");
    printf("Test Case Setup:                                           \n");
    printf("  Some of the tests in this test suits deal with the pro-  \n");
    printf("  cessing of date/time data. To ensure propper execution   \n");
    printf("  at all time, get the date/time information that is set   \n");
    printf("  for the rows used in the following test.                 \n");
    printf("---------------------------------------------------------- \n");

    try {

      dt1_set = GetDate(L"exfct_c1", L"dt2_val", filter1);
      dt2_set = GetDate(L"exfct_c1", L"dt_val", filter2);

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

    // 1. Test Case:
    // The test executes a select-command to select the value of a computed
    // property that is defined by using the function NULLVALUE on the values
    // of two different properties of type DATE/TIME. In this test, the value
    // of the first property is NULL whereas it is set for the second property.
    // The test should return this date/time information. No exceptions are
    // expected.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function NULLVALUE on the values of two different pro-   \n");
    printf("  perties of type DATE/TIME. In this test, the value of    \n");
    printf("  the first property is NULL whereas it is set for the     \n");
    printf("  second property. The test should return this date/time   \n");
    printf("  information. No exceptions are expected.                 \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property
      // being identical to the setting of the date/time value stored in
      // the variable dt1_set.

      func_call   = L"(NullValue(dt_val, dt2_val) as cmp_id)";
      data_reader =
                ExecuteSelectCommand(L"exfct_c1", filter1, true, func_call);
      CheckDateTimeReader(data_reader, true, false, 0, 21, dt1_set);
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
    // property that is defined by using the function NULLVALUE on the values
    // of two different properties of type DATE/TIME. In this test, the value
    // of the first property is set whereas it is NULL for the second property.
    // The test should therefore return the date/time information for the first
    // property. No exceptions are expected.

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("2. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function NULLVALUE on the values of two different pro-   \n");
    printf("  perties of type DATE/TIME. In this test, the value of    \n");
    printf("  the first property is set whereas it is NULL for the     \n");
    printf("  second property. The test should therefore return the    \n");
    printf("  date/time information for the first property. no excep-  \n");
    printf("  tions are expected.                                      \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property
      // being identical to the setting of the date/time value stored in
      // the variable dt2_set.

      func_call   = L"(NullValue(dt_val, dt2_val) as cmp_id)";
      data_reader =
                ExecuteSelectCommand(L"exfct_c1", filter2, true, func_call);
      CheckDateTimeReader(data_reader, true, false, 0, 22, dt2_set);
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
    // property that is defined by using the function NULLVALUE on the values
    // of two different properties of type DECIMAL and DOUBLE. In this test,
    // the value of the first property is NULL whereas it is set for the second
    // property. The test should return this value. No exceptions are expected.

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("3. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function NULLVALUE on the values of two different pro-   \n");
    printf("  perties of type DECIMAL and DOUBLE. In this test, the    \n");
    printf("  value of the first property is NULL whereas it is set    \n");
    printf("  for the second property. The test should return this     \n");
    printf("  value. No exceptions are expected.                       \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property
      // being 44.0745234.

      func_call   = L"(NullValue(dcl_val, dbl_val) as cmp_id)";
      data_reader =
                ExecuteSelectCommand(L"exfct_c1", filter1, true, func_call);
      CheckReader(data_reader, true, 21, 44.0745234);
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
    // property that is defined by using the function NULLVALUE on the values
    // of two different properties of type DECIMAL and DOUBLE. In this test,
    // the value of the first property is set whereas it is NULL for the second
    // property. The test should therefore return the value of the first pro-
    // perty. No exceptions are expected.

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("4. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function NULLVALUE on the values of two different pro-   \n");
    printf("  perties of type DECIMAL and DOUBLE. In this test, the    \n");
    printf("  value of the first property is set whereas it is NULL    \n");
    printf("  for the second property. The test should therfore return \n");
    printf("  the value of the first property. No exceptions are ex-   \n");
    printf("  pected.                                                  \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property
      // being 31.39.

      func_call   = L"(NullValue(dcl_val, dbl_val) as cmp_id)";
      data_reader =
                ExecuteSelectCommand(L"exfct_c1", filter2, true, func_call);
      CheckReader(data_reader, true, 22, 31.39);
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

}  //  TestNullValueFunction ()

void FdoExpressionFunctionTest::TestToDateFunction ()

// +---------------------------------------------------------------------------
// | The function executes the test for the expression engine function TODATE
// | when used as a select-parameter.
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

      expected_dt_data.year  = 2007;
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

void FdoExpressionFunctionTest::TestToDoubleFunction ()

// +---------------------------------------------------------------------------
// | The function executes the test for the expression engine function TODOUBLE
// | when used as a select-parameter.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    FdoStringP                func_call;

    FdoPtr<FdoFilter>         filter;
    FdoPtr<FdoIFeatureReader> data_reader;

    printf("\n");
    printf("========================================================== \n");
    printf(" Current Unit Test Suite: TODOUBLE Function Testing        \n");
    printf("========================================================== \n");
    printf("\n");

    // Define the filter for all tests in this test suite.

    filter = (FdoComparisonCondition::Create(
               FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"id")),
               FdoComparisonOperations_EqualTo, 
               FdoPtr<FdoDataValue>(FdoDataValue::Create(9))));

    // 1. Test Case:
    // The test executes a select-command to select the value of a computed
    // property that is defined by using the function TODOUBLE on the value
    // of a different property of type DECIMAL. The test should return the
    // same value. No exceptions are expected.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function TODOUBLE on the value of a different property   \n");
    printf("  of type DECIMAL. The test should return the same value.  \n");
    printf("  No exceptions are expected.                              \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property
      // being 12.84.

      func_call   = L"(ToDouble(dcl_val) as cmp_id)";
      data_reader =
                ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, 12.84);
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
    // property that is defined by using the function TODOUBLE on the value
    // of a different property of type DOUBLE. The test should return the
    // same value. No exceptions are expected.

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("2. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function TODOUBLE on the value of a different property   \n");
    printf("  of type DOUBLE. The test should return the same value.   \n");
    printf("  No exceptions are expected.                              \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property
      // being 18.8890815.

      func_call   = L"(ToDouble(dbl_val) as cmp_id)";
      data_reader =
                ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, 18.8890815);
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
    // property that is defined by using the function TODOUBLE on the value
    // of a different property of type INT32. The test should return the same
    // value. No exceptions are expected.

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("3. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function TODOUBLE on the value of a different property   \n");
    printf("  of type INT32. The test should return the same value. No \n");
    printf("  exceptions are expected.                                 \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property
      // being 90.

      func_call   = L"(ToDouble(i32_val) as cmp_id)";
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
    // property that is defined by using the function TODOUBLE on the value
    // of a different property of type SINGLE. The test should return the
    // same value. No exceptions are expected.

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("4. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function TODOUBLE on the value of a different property   \n");
    printf("  of type SINGLE. The test should return the same value.   \n");
    printf("  No exceptions are expected.                              \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property
      // being 3.09012866.

      func_call   = L"(ToDouble(sgl_val) as cmp_id)";
      data_reader =
                ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, 3.09012866);
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
    // property that is defined by using the function TODOUBLE on the value
    // of a different property of type STRING. The test should return the
    // numeric representation of the value stored in the srring. No excep-
    // tions are expected.

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("5. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function TODOUBLE on the value of a different property   \n");
    printf("  of type STRING. The test should return the numeric re-   \n");
    printf("  presentation of the value stored in the string. No ex-   \n");
    printf("  ceptions are expected.                                   \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property
      // being 9.

      func_call   = L"(ToDouble(str_val) as cmp_id)";
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

}  //  TestToDoubleFunction ()

void FdoExpressionFunctionTest::TestToFloatFunction ()

// +---------------------------------------------------------------------------
// | The function executes the test for the expression engine function TOFLOAT
// | when used as a select-parameter.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    FdoStringP                func_call;

    FdoPtr<FdoFilter>         filter;
    FdoPtr<FdoIFeatureReader> data_reader;

    printf("\n");
    printf("========================================================== \n");
    printf(" Current Unit Test Suite: TOFLOAT Function Testing         \n");
    printf("========================================================== \n");
    printf("\n");

    // Define the filter for all tests in this test suite.

    filter = (FdoComparisonCondition::Create(
               FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"id")),
               FdoComparisonOperations_EqualTo, 
               FdoPtr<FdoDataValue>(FdoDataValue::Create(9))));

    // 1. Test Case:
    // The test executes a select-command to select the value of a computed
    // property that is defined by using the function TOFLOAT on the value
    // of a different property of type DECIMAL. The test should return the
    // same value. No exceptions are expected.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function TOFLOAT on the value of a different property of \n");
    printf("  type DECIMAL. The test should return the same value. No  \n");
    printf("  exceptions are expected.                                 \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property
      // being 12.84.

      func_call   = L"(ToFloat(dcl_val) as cmp_id)";
      data_reader =
                ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, 12.84);
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
    // property that is defined by using the function TOFLOAT on the value of
    // a different property of type DOUBLE. The test should return the same
    // value. No exceptions are expected.

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("2. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function TOFLOAT on the value of a different property of \n");
    printf("  type DOUBLE. The test should return the same value. No   \n");
    printf("  exceptions are expected.                                 \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property
      // being 18.8890815.

      func_call   = L"(ToFloat(dbl_val) as cmp_id)";
      data_reader =
                ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, 18.8890815);
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
    // property that is defined by using the function TOFLOAT on the value of
    // a different property of type INT32. The test should return the same
    // value. No exceptions are expected.

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("3. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function TOFLOAT on the value of a different property of \n");
    printf("  type INT32. The test should return the same value. No    \n");
    printf("  exceptions are expected.                                 \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property
      // being 90.

      func_call   = L"(ToFloat(i32_val) as cmp_id)";
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
    // property that is defined by using the function TOFLOAT on the value of
    // a different property of type SINGLE. The test should return the same
    // value. No exceptions are expected.

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("4. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function TOFLOAT on the value of a different property of \n");
    printf("  type SINGLE. The test should return the same value. No   \n");
    printf("  exceptions are expected.                                 \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property
      // being 3.09012866.

      func_call   = L"(ToFloat(sgl_val) as cmp_id)";
      data_reader =
                ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, 3.09012866);
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
    // property that is defined by using the function TOFLOAT on the value
    // of a different property of type STRING. The test should return the
    // numeric representation of the value stored in the string. No exceptions
    // are expected.

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("5. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function TOFLOAT on the value of a different property of \n");
    printf("  type STRING. The test should return the numeric repre-   \n");
    printf("  sentation of the value stored in the string. No excep-   \n");
    printf("  tions are expected.                                      \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property
      // being 9.

      func_call   = L"(ToFloat(str_val) as cmp_id)";
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

}  //  TestToFloatFunction ()

void FdoExpressionFunctionTest::TestToInt32Function ()

// +---------------------------------------------------------------------------
// | The function executes the test for the expression engine function TOINT32
// | when used as a select-parameter.
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
      // being 12.

      func_call   = L"(ToInt32(dcl_val) as cmp_id)";
      data_reader =
                ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, 12);
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
      // being 18.

      func_call   = L"(ToInt32(dbl_val) as cmp_id)";
      data_reader =
                ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, 18);
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

void FdoExpressionFunctionTest::TestToInt64Function ()

// +---------------------------------------------------------------------------
// | The function executes the test for the expression engine function TOINT64
// | when used as a select-parameter.
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
      // being 12.

      func_call   = L"(ToInt64(dcl_val) as cmp_id)";
      data_reader =
                ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, 12);
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
      // being 18.

      func_call   = L"(ToInt64(dbl_val) as cmp_id)";
      data_reader =
                ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, 18);
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

void FdoExpressionFunctionTest::TestToStringFunction ()

// +---------------------------------------------------------------------------
// | The function executes the test for the expression engine function TOSTRING
// | when used as a select-parameter.
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
      CheckStringFunctionReader(data_reader, 9, L"12.84");
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
      // being 18.88908145580589.

      func_call   = L"(ToString(dbl_val) as cmp_id)";
      data_reader =
                ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckStringFunctionReader(data_reader, 9, L"18.88908145580589");
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
      // being 3.09012866.

      func_call   = L"(ToString(sgl_val) as cmp_id)";
      data_reader =
                ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckStringFunctionReader(data_reader, 9, L"3.09012866");
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
      // being "2007-09-09-21-00-00".

      func_call   = L"(ToString(dt_val) as cmp_id)";
      data_reader =
                ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckStringFunctionReader(data_reader, 9, L"2007-09-09-21-00-00");
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
// ==                       TESTING THE DATE FUNCTIONS                      ==
// ===========================================================================

void FdoExpressionFunctionTest::TestAddMonthsFunction ()

// +---------------------------------------------------------------------------
// | The function executes the test for the expression engine function ADD-
// | MONTHS when used as a select-parameter.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    FdoStringP                func_call;

    FdoDateTime               dt_set,
                              expected_dt_data;

    FdoPtr<FdoFilter>         filter;
    FdoPtr<FdoIFeatureReader> data_reader;

    printf("\n");
    printf("========================================================== \n");
    printf(" Current Unit Test Suite: ADDMONTHS Function Testing       \n");
    printf("========================================================== \n");
    printf("\n");

    // Define the filter for all tests in this test suite.

    filter = (FdoComparisonCondition::Create(
               FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"id")),
               FdoComparisonOperations_EqualTo, 
               FdoPtr<FdoDataValue>(FdoDataValue::Create(9))));

    // This test suits deals with the processing of date/time data. To ensure
    // propper execution at all time, get the date/time information that is
    // set for the row used in the following test.

    printf("---------------------------------------------------------- \n");
    printf("Test Case Setup:                                           \n");
    printf("  The following retrieves date/time information used when  \n");
    printf("  cross-checking function results. No exceptions are ex-   \n");
    printf("  pected.                                                  \n");
    printf("---------------------------------------------------------- \n");

    try {

      dt_set = GetDate(L"exfct_c1", L"dt_val", filter);

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

    // 1. Test Case:
    // The test executes a select-command to select the value of a computed
    // property that is defined by using the function ADDMONTHS on the value
    // of a different property of type DATE/TIME. The test adds 12 months to
    // the date on which the function is based. No exceptions are expected.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function ADDMONTHS on the value of a different property  \n");
    printf("  of type DATE/TIME. The test adds 12 months to the date   \n");
    printf("  on which the function is based. No exceptions are ex-    \n");
    printf("  pected.                                                  \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The date value for the selected computed
      // property is expected to have added one year to the original date.

      expected_dt_data.year  = dt_set.year + 1;
      expected_dt_data.month = dt_set.month;
      expected_dt_data.day   = dt_set.day;

      func_call   = L"(AddMonths(dt_val, 12) as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", filter, true, func_call);
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

}  //  TestAddMonthsFunction ()

void FdoExpressionFunctionTest::TestCurrentDateFunction ()

// +---------------------------------------------------------------------------
// | The function executes the test for the expression engine function CURRENT-
// | DATE when used as a select-parameter.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    FdoStringP                func_call;

    FdoDateTime               dt_set,
                              expected_dt_data;

    FdoPtr<FdoIFeatureReader> data_reader;

    printf("\n");
    printf("========================================================== \n");
    printf(" Current Unit Test Suite: CURRENTDATE Function Testing     \n");
    printf("========================================================== \n");
    printf("\n");

    // This test suits deals with the processing of date/time data. To ensure
    // propper execution at all time, get the date/time information that is
    // set for the row used in the following test.

    //printf("---------------------------------------------------------- \n");
    //printf("Test Case Setup:                                           \n");
    //printf("  The following retrieves date/time information used when  \n");
    //printf("  cross-checking function results. No exceptions are ex-   \n");
    //printf("  pected.                                                  \n");
    //printf("---------------------------------------------------------- \n");

    //try {

    //  dt_set = GetDate(L"exfct_c1", L"dt_val", filter);

    //}  //  try ...

    //catch (FdoException *exp) {

    //  printf(" >>> Exception: %ls\n", exp->GetExceptionMessage());
    //  printf(" >>> Test failed \n");
    //  throw exp;

    //}  //  catch (FdoException *ex) ...

    //catch ( ... ) {

    //  printf(" >>> Test failed for an unknown reason \n");
    //  throw;

    //}  //  catch ( ... ) ...

    // 1. Test Case:
    // The test executes a select-command to select the value of a computed
    // property that is defined by using the function CURRENTDATE. No excep-
    // tions are expected.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function CURRENTDATE. No exceptions are expected.        \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The date value for the selected computed
      // property is expected to be the same as previously retrieved in the
      // test setup.

      //expected_dt_data.year  = dt_set.year + 1;
      //expected_dt_data.month = dt_set.month;
      //expected_dt_data.day   = dt_set.day;

      func_call   = L"(CurrentDate() as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", NULL, false, func_call);
      // CheckDateTimeReader(data_reader, true, false, 0, 9, expected_dt_data);
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

}  //  TestCurrentDateFunction ()

void FdoExpressionFunctionTest::TestExtractFunction ()

// +---------------------------------------------------------------------------
// | The function executes the test for the expression engine function EXTRACT
// | when used as a select-parameter.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    FdoStringP                func_call;

    FdoDateTime               dt_set,
                              expected_dt_data;

    FdoPtr<FdoFilter>         filter;
    FdoPtr<FdoIFeatureReader> data_reader;

    printf("\n");
    printf("========================================================== \n");
    printf(" Current Unit Test Suite: EXTRACT Function Testing         \n");
    printf("========================================================== \n");
    printf("\n");

    // Define the filter for all tests in this test suite.

    filter = (FdoComparisonCondition::Create(
               FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"id")),
               FdoComparisonOperations_EqualTo, 
               FdoPtr<FdoDataValue>(FdoDataValue::Create(9))));

    // This test suits deals with the processing of date/time data. To ensure
    // propper execution at all time, get the date/time information that is
    // set for the row used in the following test.

    printf("---------------------------------------------------------- \n");
    printf("Test Case Setup:                                           \n");
    printf("  The following retrieves date/time information used when  \n");
    printf("  cross-checking function results. No exceptions are ex-   \n");
    printf("  pected.                                                  \n");
    printf("---------------------------------------------------------- \n");
    printf("\n");

    try {

      dt_set = GetDate(L"exfct_c1", L"dt_val", filter);

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

    // 1. Test Case:
    // The test executes a select-command to select the value of a computed
    // property that is defined by using the function EXTRACT on the value
    // of a different property of type DATE/TIME. The test extracts the year
    // from the date on which the function operates. No exceptions are ex-
    // pected.

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function EXTRACT on the value of a different property of \n");
    printf("  type DATE/TIME. The test extracts the year from the date \n");
    printf("  on which the function operates. No exceptions are ex-    \n");
    printf("  pected.                                                  \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The year value for the selected computed
      // property is expected to be the value of the original date on which
      // the function is executed.

      expected_dt_data.year = dt_set.year;

      func_call   = L"(Extract('YEAR', dt_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", filter, true, func_call);
      CheckDateTimeReader(data_reader, true, true, 0, 9, expected_dt_data);
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
    // property that is defined by using the function EXTRACT on the value
    // of a different property of type DATE/TIME. The test extracts the month
    // from the date on which the function operates. No exceptions are ex-
    // pected.

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("2. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function EXTRACT on the value of a different property of \n");
    printf("  type DATE/TIME. The test extracts the month from the     \n");
    printf("  date on which the function operates. No exceptions are   \n");
    printf("  expected.                                                \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The month value for the selected computed
      // property is expected to be the value of the original date on which
      // the function is executed.

      expected_dt_data.month = dt_set.month;

      func_call   = L"(Extract('MONTH', dt_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", filter, true, func_call);
      CheckDateTimeReader(data_reader, true, true, 1, 9, expected_dt_data);
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
    // property that is defined by using the function EXTRACT on the value
    // of a different property of type DATE/TIME. The test extracts the day
    // from the date on which the function operates. No exceptions are ex-
    // pected.

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("3. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function EXTRACT on the value of a different property of \n");
    printf("  type DATE/TIME. The test extracts the day from the date  \n");
    printf("  on which the function operates. No exceptions are ex-    \n");
    printf("  pected.                                                  \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The day value for the selected computed
      // property is expected to be the value of the original date on which
      // the function is executed.

      expected_dt_data.day  = dt_set.day;

      func_call   = L"(Extract('DAY', dt_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", filter, true, func_call);
      CheckDateTimeReader(data_reader, true, true, 2, 9, expected_dt_data);
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

}  //  TestExtractFunction ()

void FdoExpressionFunctionTest::TestMonthsBetweenFunction ()

// +---------------------------------------------------------------------------
// | The function executes the test for the expression engine function MONTHS-
// | BETWEEN when used as a select-parameter.
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

    // 1. Test Case:
    // The test executes a select-command to select the value of a computed
    // property that is defined by using the function MONTHSBETWEEN on the
    // values of two different properties of type DATE/TIME. The test should
    // return the months between those two dates. No exceptions are expected.

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
      // is expected to be -8.

      func_call   = L"(MonthsBetween(dt_val, dt2_val) as cmp_id)";
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

}  //  TestMonthsBetweenFunction ()


// ===========================================================================
// ==                       TESTING THE MATH FUNCTIONS                      ==
// ===========================================================================

void FdoExpressionFunctionTest::TestAbsFunction ()

// +---------------------------------------------------------------------------
// | The function executes the test for the expression engine function ABS when
// | used as a select-parameter.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    FdoStringP                func_call;

    FdoPtr<FdoFilter>         filter;
    FdoPtr<FdoIFeatureReader> data_reader;

    printf("\n");
    printf("========================================================== \n");
    printf(" Current Unit Test Suite: ABS Function Testing             \n");
    printf("========================================================== \n");
    printf("\n");

    // Define the filter for all tests in this test suite.

    filter = (FdoComparisonCondition::Create(
               FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"id")),
               FdoComparisonOperations_EqualTo, 
               FdoPtr<FdoDataValue>(FdoDataValue::Create(9))));

    // 1. Test Case:
    // The test executes a select-command to select the value of a computed
    // property that is defined by using the function ABS on the positive
    // value of a different property of type INT32. No exceptions are ex-
    // pected.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function ABS on the positive value of a different pro-   \n");
    printf("  perty of type INT32. No exceptions are expected.         \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 90.

      func_call   = L"(Abs(i32_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", filter, true, func_call);
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

    // 2. Test Case:
    // The test executes a select-command to select the value of a computed
    // property that is defined by using the function ABS on the negative value
    // of a different property of type INT32. No exceptions are expected.

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("2. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function ABS on the negative value of a different pro-   \n");
    printf("  perty of type INT32. No exceptions are expected.         \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 90.

      func_call   = L"(Abs(i32_val*-1) as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", filter, true, func_call);
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

}  //  TestAbsFunction ()

void FdoExpressionFunctionTest::TestAcosFunction ()

// +---------------------------------------------------------------------------
// | The function executes the test for the expression engine function ACOS
// | when used as a select-parameter.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    FdoStringP                func_call;

    FdoPtr<FdoFilter>         filter;
    FdoPtr<FdoIFeatureReader> data_reader;

    printf("\n");
    printf("========================================================== \n");
    printf(" Current Unit Test Suite: ACOS Function Testing            \n");
    printf("========================================================== \n");
    printf("\n");

    // Define the filter for all tests in this test suite.

    filter = (FdoComparisonCondition::Create(
               FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"id")),
               FdoComparisonOperations_EqualTo, 
               FdoPtr<FdoDataValue>(FdoDataValue::Create(0))));

    // 1. Test Case:
    // The test executes a select-command to select the value of a computed
    // property that is defined by using the function ACOS on the value of
    // a different property of type DECIMAL. No exceptions are expected.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function ACOS on the value of a different property of    \n");
    printf("  type DECIMAL. No exceptions are expected.                \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 1.57079633.

      func_call   = L"(Acos(dcl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 0, 1.57079633);
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

}  //  TestAcosFunction ()

void FdoExpressionFunctionTest::TestAsinFunction ()

// +---------------------------------------------------------------------------
// | The function executes the test for the expression engine function ASIN
// | when used as a select-parameter.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    FdoStringP                func_call;

    FdoPtr<FdoFilter>         filter;
    FdoPtr<FdoIFeatureReader> data_reader;

    printf("\n");
    printf("========================================================== \n");
    printf(" Current Unit Test Suite: ASIN Function Testing            \n");
    printf("========================================================== \n");
    printf("\n");

    // Define the filter for all tests in this test suite.

    filter = (FdoComparisonCondition::Create(
               FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"id")),
               FdoComparisonOperations_EqualTo, 
               FdoPtr<FdoDataValue>(FdoDataValue::Create(0))));

    // 1. Test Case:
    // The test executes a select-command to select the value of a computed
    // property that is defined by using the function ASIN on the value of
    // a different property of type DECIMAL. No exceptions are expected.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function ASIN on the value of a different property of    \n");
    printf("  type DECIMAL. No exceptions are expected.                \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 0.

      func_call   = L"(Asin(dcl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 0, 0);
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

}  //  TestAsinFunction ()

void FdoExpressionFunctionTest::TestAtanFunction ()

// +---------------------------------------------------------------------------
// | The function executes the test for the expression engine function ATAN
// | when used as a select-parameter.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    FdoStringP                func_call;

    FdoPtr<FdoFilter>         filter;
    FdoPtr<FdoIFeatureReader> data_reader;

    printf("\n");
    printf("========================================================== \n");
    printf(" Current Unit Test Suite: ATAN Function Testing            \n");
    printf("========================================================== \n");
    printf("\n");

    // Define the filter for all tests in this test suite.

    filter = (FdoComparisonCondition::Create(
               FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"id")),
               FdoComparisonOperations_EqualTo, 
               FdoPtr<FdoDataValue>(FdoDataValue::Create(0))));

    // 1. Test Case:
    // The test executes a select-command to select the value of a computed
    // property that is defined by using the function ATAN on the value of
    // a different property of type DECIMAL. No exceptions are expected.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function ATAN on the value of a different property of    \n");
    printf("  type DECIMAL. No exceptions are expected.                \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 0.

      func_call   = L"(Atan(dcl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 0, 0);
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

}  //  TestAtanFunction ()

void FdoExpressionFunctionTest::TestAtan2Function ()

// +---------------------------------------------------------------------------
// | The function executes the test for the expression engine function ATAN2
// | when used as a select-parameter.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    FdoStringP                func_call;

    FdoPtr<FdoFilter>         filter;
    FdoPtr<FdoIFeatureReader> data_reader;

    printf("\n");
    printf("========================================================== \n");
    printf(" Current Unit Test Suite: ATAN2 Function Testing           \n");
    printf("========================================================== \n");
    printf("\n");

    // Define the filter for all tests in this test suite.

    filter = (FdoComparisonCondition::Create(
               FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"id")),
               FdoComparisonOperations_EqualTo, 
               FdoPtr<FdoDataValue>(FdoDataValue::Create(9))));

    // 1. Test Case:
    // The test executes a select-command to select the value of a computed
    // property that is defined by using the function ATAN2 on the values of
    // two different properties (of type DOUBLE, DECIMAL). No exceptions are
    // expected.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function ATAN2 on the values of two different properties \n");
    printf("  (of type DOUBLE, DECIMAL). No exceptions are expected.   \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 0.973785322.

      func_call   = L"(Atan2(dbl_val, dcl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, 0.973785322);
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

}  //  TestAtan2Function ()

void FdoExpressionFunctionTest::TestCosFunction ()

// +---------------------------------------------------------------------------
// | The function executes the test for the expression engine function COS when
// | used as a select-parameter.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    FdoStringP                func_call;

    FdoPtr<FdoFilter>         filter;
    FdoPtr<FdoIFeatureReader> data_reader;

    printf("\n");
    printf("========================================================== \n");
    printf(" Current Unit Test Suite: COS Function Testing             \n");
    printf("========================================================== \n");
    printf("\n");

    // Define the filter for all tests in this test suite.

    filter = (FdoComparisonCondition::Create(
               FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"id")),
               FdoComparisonOperations_EqualTo, 
               FdoPtr<FdoDataValue>(FdoDataValue::Create(9))));

    // 1. Test Case:
    // The test executes a select-command to select the value of a computed
    // property that is defined by using the function COS on the value of a
    // different property of type DECIMAL. No exceptions are expected.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function COS on the value of a different property of     \n");
    printf("  type DECIMAL. No exceptions are expected.                \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 0.96279648.

      func_call   = L"(Cos(dcl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, 0.96279648);
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

}  //  TestCosFunction ()

void FdoExpressionFunctionTest::TestExpFunction ()

// +---------------------------------------------------------------------------
// | The function executes the test for the expression engine function EXP when
// | used as a select-parameter.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    FdoStringP                func_call;

    FdoPtr<FdoFilter>         filter;
    FdoPtr<FdoIFeatureReader> data_reader;

    printf("\n");
    printf("========================================================== \n");
    printf(" Current Unit Test Suite: EXP Function Testing             \n");
    printf("========================================================== \n");
    printf("\n");

    // Define the filter for all tests in this test suite.

    filter = (FdoComparisonCondition::Create(
               FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"id")),
               FdoComparisonOperations_EqualTo, 
               FdoPtr<FdoDataValue>(FdoDataValue::Create(9))));

    // 1. Test Case:
    // The test executes a select-command to select the value of a computed
    // property that is defined by using the function EXP on the value of a
    // different property of type DECIMAL. No exceptions are expected.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function EXP on the value of a different property of     \n");
    printf("  type DECIMAL. No exceptions are expected.                \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 376999.824.

      func_call   = L"(Exp(dcl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, 376999.824);
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

}  //  TestExpFunction ()

void FdoExpressionFunctionTest::TestLnFunction ()

// +---------------------------------------------------------------------------
// | The function executes the test for the expression engine function LN when
// | used as a select-parameter.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    FdoStringP                func_call;

    FdoPtr<FdoFilter>         filter;
    FdoPtr<FdoIFeatureReader> data_reader;

    printf("\n");
    printf("========================================================== \n");
    printf(" Current Unit Test Suite: LN Function Testing              \n");
    printf("========================================================== \n");
    printf("\n");

    // Define the filter for all tests in this test suite.

    filter = (FdoComparisonCondition::Create(
               FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"id")),
               FdoComparisonOperations_EqualTo, 
               FdoPtr<FdoDataValue>(FdoDataValue::Create(9))));

    // 1. Test Case:
    // The test executes a select-command to select the value of a computed
    // property that is defined by using the function LN on the value of a
    // different property of type DECIMAL. No exceptions are expected.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function LN on the value of a different property of type \n");
    printf("  DECIMAL. No exceptions are expected.                     \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 2.5525653.

      func_call   = L"(Ln(dcl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, 2.5525653);
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

}  //  TestLnFunction ()

void FdoExpressionFunctionTest::TestLogFunction ()

// +---------------------------------------------------------------------------
// | The function executes the test for the expression engine function LOG when
// | used as a select-parameter.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    FdoStringP                func_call;

    FdoPtr<FdoFilter>         filter;
    FdoPtr<FdoIFeatureReader> data_reader;

    printf("\n");
    printf("========================================================== \n");
    printf(" Current Unit Test Suite: LOG Function Testing             \n");
    printf("========================================================== \n");
    printf("\n");

    // Define the filter for all tests in this test suite.

    filter = (FdoComparisonCondition::Create(
               FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"id")),
               FdoComparisonOperations_EqualTo, 
               FdoPtr<FdoDataValue>(FdoDataValue::Create(9))));

    // 1. Test Case:
    // The test executes a select-command to select the value of a computed
    // property that is defined by using the function LOG on the values of
    // two different properties (of type DOUBLE, DECIMAL). No exceptions are
    // expected.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function LOG on the values of two different properties   \n");
    printf("  (of type DOUBLE, DECIMAL). No exceptions are expected.   \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 0.868637837.

      func_call   = L"(Log(dbl_val, dcl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, 0.868637837);
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

}  //  TestLogFunction ()

void FdoExpressionFunctionTest::TestModFunction ()

// +---------------------------------------------------------------------------
// | The function executes the test for the expression engine function MOD when
// | used as a select-parameter.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    FdoStringP                func_call;

    FdoPtr<FdoFilter>         filter;
    FdoPtr<FdoIFeatureReader> data_reader;

    printf("\n");
    printf("========================================================== \n");
    printf(" Current Unit Test Suite: MOD Function Testing             \n");
    printf("========================================================== \n");
    printf("\n");

    // Define the filter for all tests in this test suite.

    filter = (FdoComparisonCondition::Create(
               FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"id")),
               FdoComparisonOperations_EqualTo, 
               FdoPtr<FdoDataValue>(FdoDataValue::Create(9))));

    // 1. Test Case:
    // The test executes a select-command to select the value of a computed
    // property that is defined by using the function MOD on the values of
    // two different properties (of type DOUBLE, DECIMAL). No exceptions are
    // expected.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function MOD on the values of two different properties   \n");
    printf("  (of type DOUBLE, DECIMAL). No exceptions are expected.   \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 6.04908146.

      func_call   = L"(Mod(dbl_val, dcl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, 6.04908146);
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

}  //  TestModFunction ()

void FdoExpressionFunctionTest::TestPowerFunction ()

// +---------------------------------------------------------------------------
// | The function executes the test for the expression engine function POWER
// | when used as a select-parameter.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    FdoStringP                func_call;

    FdoPtr<FdoFilter>         filter;
    FdoPtr<FdoIFeatureReader> data_reader;

    printf("\n");
    printf("========================================================== \n");
    printf(" Current Unit Test Suite: POWER Function Testing           \n");
    printf("========================================================== \n");
    printf("\n");

    // Define the filter for all tests in this test suite.

    filter = (FdoComparisonCondition::Create(
               FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"id")),
               FdoComparisonOperations_EqualTo, 
               FdoPtr<FdoDataValue>(FdoDataValue::Create(1))));

    // 1. Test Case:
    // The test executes a select-command to select the value of a computed
    // property that is defined by using the function MOD on the values of
    // two different properties (of type INT16, INT32). No exceptions are
    // expected.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function POWER on the values of two different properties \n");
    printf("  (of type INT16, INT32). No exceptions are expected.      \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 1048576.

      func_call   = L"(Power(i16_val, i32_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 1, 1048576);
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

}  //  TestPowerFunction ()

void FdoExpressionFunctionTest::TestRemainderFunction ()

// +---------------------------------------------------------------------------
// | The function executes the test for the expression engine function RE-
// | MAINDER when used as a select-parameter.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    FdoStringP                func_call;

    FdoPtr<FdoFilter>         filter;
    FdoPtr<FdoIFeatureReader> feature_reader;

    printf("\n");
    printf("========================================================== \n");
    printf(" Current Unit Test Suite: REMAINDER Function Testing       \n");
    printf("========================================================== \n");
    printf("\n");

    // Define the filter for all tests in this test suite.

    filter = (FdoComparisonCondition::Create(
               FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"id")),
               FdoComparisonOperations_EqualTo, 
               FdoPtr<FdoDataValue>(FdoDataValue::Create(9))));

    // 1. Test Case:
    // The test executes a select-command to select the value of a computed
    // property that is defined by using the function REMAINDER on the values
    // of two different properties (of type DOUBLE, DECIMAL). No exceptions
    // are expected.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function REMAINDER on the values of two different pro-   \n");
    printf("  perties (of type DOUBLE, DECIMAL). No exceptions are ex- \n");
    printf("  pected.                                                  \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 6.04908146.

      func_call   = L"(Remainder(dbl_val, dcl_val) as cmp_id)";
      feature_reader = ExecuteSelectCommand(
                                        L"exfct_c1", filter, true, func_call);
      CheckReader(feature_reader, true, 9, 6.04908146);
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

}  //  TestRemainderFunction ()

void FdoExpressionFunctionTest::TestSinFunction ()

// +---------------------------------------------------------------------------
// | The function executes the test for the expression engine function SIN when
// | used as a select-parameter.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    FdoStringP                func_call;

    FdoPtr<FdoFilter>         filter;
    FdoPtr<FdoIFeatureReader> data_reader;

    printf("\n");
    printf("========================================================== \n");
    printf(" Current Unit Test Suite: SIN Function Testing             \n");
    printf("========================================================== \n");
    printf("\n");

    // Define the filter for all tests in this test suite.

    filter = (FdoComparisonCondition::Create(
               FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"id")),
               FdoComparisonOperations_EqualTo, 
               FdoPtr<FdoDataValue>(FdoDataValue::Create(9))));

    // 1. Test Case:
    // The test executes a select-command to select the value of a computed
    // property that is defined by using the function SIN on the value of a
    // different property of type DECIMAL. No exceptions are expected.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function SIN on the value of a different property of     \n");
    printf("  type DECIMAL. No exceptions are expected.                \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 0.270227569.

      func_call   = L"(Sin(dcl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, 0.270227569);
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

}  //  TestSinFunction ()

void FdoExpressionFunctionTest::TestSqrtFunction ()

// +---------------------------------------------------------------------------
// | The function executes the test for the expression engine function SQRT
// | when used as a select-parameter.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    FdoStringP                func_call;

    FdoPtr<FdoFilter>         filter;
    FdoPtr<FdoIFeatureReader> data_reader;

    printf("\n");
    printf("========================================================== \n");
    printf(" Current Unit Test Suite: SQRT Function Testing            \n");
    printf("========================================================== \n");
    printf("\n");

    // Define the filter for all tests in this test suite.

    filter = (FdoComparisonCondition::Create(
               FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"id")),
               FdoComparisonOperations_EqualTo, 
               FdoPtr<FdoDataValue>(FdoDataValue::Create(9))));

    // 1. Test Case:
    // The test executes a select-command to select the value of a computed
    // property that is defined by using the function SQRT on the value of a
    // different property of type DECIMAL. No exceptions are expected.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function SQRT on the value of a different property of    \n");
    printf("  type DECIMAL. No exceptions are expected.                \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 3.58329457.

      func_call   = L"(Sqrt(dcl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, 3.58329457);
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

}  //  TestSqrtFunction ()

void FdoExpressionFunctionTest::TestTanFunction ()

// +---------------------------------------------------------------------------
// | The function executes the test for the expression engine function TAN when
// | used as a select-parameter.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    FdoStringP                func_call;

    FdoPtr<FdoFilter>         filter;
    FdoPtr<FdoIFeatureReader> data_reader;

    printf("\n");
    printf("========================================================== \n");
    printf(" Current Unit Test Suite: TAN Function Testing             \n");
    printf("========================================================== \n");
    printf("\n");

    // Define the filter for all tests in this test suite.

    filter = (FdoComparisonCondition::Create(
               FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"id")),
               FdoComparisonOperations_EqualTo, 
               FdoPtr<FdoDataValue>(FdoDataValue::Create(9))));

    // 1. Test Case:
    // The test executes a select-command to select the value of a computed
    // property that is defined by using the function TAN on the value of a
    // different property of type DECIMAL. No exceptions are expected.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function TAN on the value of a different property of     \n");
    printf("  type DECIMAL. No exceptions are expected.                \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 0.28066946.

      func_call   = L"(Tan(dcl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, 0.28066946);
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

}  //  TestTanFunction ()


// ===========================================================================
// ==                     TESTING THE NUMERIC FUNCTIONS                     ==
// ===========================================================================

void FdoExpressionFunctionTest::TestCeilFunction ()

// +---------------------------------------------------------------------------
// | The function executes the test for the expression engine function CEIL
// | when used as a select-parameter.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    FdoStringP                func_call;

    FdoPtr<FdoFilter>         filter;
    FdoPtr<FdoIFeatureReader> data_reader;

    printf("\n");
    printf("========================================================== \n");
    printf(" Current Unit Test Suite: CEIL Function Testing            \n");
    printf("========================================================== \n");
    printf("\n");

    // Define the filter for all tests in this test suite.

    filter = (FdoComparisonCondition::Create(
               FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"id")),
               FdoComparisonOperations_EqualTo, 
               FdoPtr<FdoDataValue>(FdoDataValue::Create(1))));

    // 1. Test Case:
    // The test executes a select-command to select the value of a computed
    // property that is defined by using the function CEIL on the value of a
    // different property of type DECIMAL. No exceptions are expected.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function CEIL on the value of a different property of    \n");
    printf("  type DECIMAL. No exceptions are expected.                \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 2.

      func_call   = L"(Ceil(dcl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 1, 2);
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
    // property that is defined by using the function CEIL on the value of a
    // different property of type DECIMAL. No exceptions are expected.

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("2. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function CEIL on the value of a different property of    \n");
    printf("  type DOUBLE. No exceptions are expected.                 \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 3.

      func_call   = L"(Ceil(dbl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 1, 3);
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
    // property that is defined by using the function CEIL on the value of a
    // different property of type INT32. No exceptions are expected.

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("3. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function CEIL on the value of a different property of    \n");
    printf("  type INT32. No exceptions are expected.                  \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 10.

      func_call   = L"(Ceil(i32_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 1, 10);
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
    // property that is defined by using the function CEIL on the value of a
    // different property of type SINGLE. No exceptions are expected.

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("4. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function CEIL on the value of a different property of    \n");
    printf("  type SINGLE. No exceptions are expected.                 \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that this
      // call returns 1 row. The value for the selected computed property is
      // expected to be 1.

      func_call   = L"(Ceil(sgl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 1, 1);
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

}  //  TestCeilFunction ()

void FdoExpressionFunctionTest::TestFloorFunction ()

// +---------------------------------------------------------------------------
// | The function executes the test for the expression engine function FLOOR
// | when used as a select-parameter.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    FdoStringP                func_call;

    FdoPtr<FdoFilter>         filter;
    FdoPtr<FdoIFeatureReader> data_reader;

    printf("\n");
    printf("========================================================== \n");
    printf(" Current Unit Test Suite: FLOOR Function Testing           \n");
    printf("========================================================== \n");
    printf("\n");

    // Define the filter for all tests in this test suite.

    filter = (FdoComparisonCondition::Create(
               FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"id")),
               FdoComparisonOperations_EqualTo, 
               FdoPtr<FdoDataValue>(FdoDataValue::Create(9))));

    // 1. Test Case:
    // The test executes a select-command to select the value of a computed
    // property that is defined by using the function FLOOR on the value of
    // a different property of type DECIMAL. No exceptions are expected.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function FLOOR on the value of a different property of   \n");
    printf("  type DECIMAL. No exceptions are expected.                \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 12.

      func_call   = L"(Floor(dcl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, 12);
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
    // property that is defined by using the function FLOOR on the value of
    // a different property of type DOUBLE. No exceptions are expected.

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("2. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function FLOOR on the value of a different property of   \n");
    printf("  type DOUBLE. No exceptions are expected.                 \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 18.

      func_call   = L"(Floor(dbl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, 18);
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
    // property that is defined by using the function FLOOR on the value of
    // a different property of type INT32. No exceptions are expected.

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("3. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function FLOOR on the value of a different property of   \n");
    printf("  type INT32. No exceptions are expected.                  \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 90.

      func_call   = L"(Floor(i32_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", filter, true, func_call);
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
    // property that is defined by using the function FLOOR on the value of
    // a different property of type SINGLE. No exceptions are expected.

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("4. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function FLOOR on the value of a different property of   \n");
    printf("  type SINGLE. No exceptions are expected.                 \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that this
      // call returns 1 row. The value for the selected computed property is
      // expected to be 3.

      func_call   = L"(Floor(sgl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", filter, true, func_call);
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

}  //  TestFloorFunction ()

void FdoExpressionFunctionTest::TestRoundFunction ()

// +---------------------------------------------------------------------------
// | The function executes the test for the expression engine function ROUND
// | when used as a select-parameter.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    FdoStringP                func_call;

    FdoPtr<FdoFilter>         filter;
    FdoPtr<FdoIFeatureReader> data_reader;

    printf("\n");
    printf("========================================================== \n");
    printf(" Current Unit Test Suite: ROUND Function Testing           \n");
    printf("========================================================== \n");
    printf("\n");

    // Define the filter for all tests in this test suite.

    filter = (FdoComparisonCondition::Create(
               FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"id")),
               FdoComparisonOperations_EqualTo, 
               FdoPtr<FdoDataValue>(FdoDataValue::Create(9))));

    // 1. Test Case:
    // The test executes a select-command to select the value of a computed
    // property that is defined by using the function ROUND on the value of
    // a different property of type DECIMAL. No exceptions are expected.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function ROUND on the value of a different property of   \n");
    printf("  type DECIMAL. No exceptions are expected.                \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 13.

      func_call   = L"(Round(dcl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", filter, true, func_call);
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
    // property that is defined by using the function ROUND on the value of
    // a different property of type DECIMAL. The test requests to round to
    // a certain scale depth. No exceptions are expected.

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("2. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function ROUND on the value of a different property of   \n");
    printf("  type DECIMAL. The test requests to round to a certain    \n");
    printf("  scale depth. No exceptions are expected.                 \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 12.8.

      func_call   = L"(Round(dcl_val, 1.223) as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, 12.8);
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
    // property that is defined by using the function ROUND on the value of
    // a different property of type DOUBLE. No exceptions are expected.

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("3. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function ROUND on the value of a different property of   \n");
    printf("  type DOUBLE. No exceptions are expected.                 \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 19.

      func_call   = L"(Round(dbl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", filter, true, func_call);
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

    // 4. Test Case:
    // The test executes a select-command to select the value of a computed
    // property that is defined by using the function ROUND on the value of
    // a different property of type DOUBLE. The test requests to round to a
    // certain scale depth. No exceptions are expected.

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("4. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function ROUND on the value of a different property of   \n");
    printf("  type DOUBLE. The test requests to round to a certain     \n");
    printf("  scale depth. No exceptions are expected.                 \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 18.9.

      func_call   = L"(Round(dbl_val, 1.223) as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, 18.9);
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
    // property that is defined by using the function ROUND on the value of
    // a different property of type INT32. No exceptions are expected.

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("5. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function ROUND on the value of a different property of   \n");
    printf("  type INT32. No exceptions are expected.                  \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 90.

      func_call   = L"(Round(i32_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", filter, true, func_call);
      CheckReader32(data_reader, true, 9, 90);
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
    // property that is defined by using the function ROUND on the value of
    // a different property of type INT32. The test requests to round to a
    // certain scale depth. No exceptions are expected.

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("6. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function ROUND on the value of a different property of   \n");
    printf("  type INT32. The test requests to round to a certain      \n");
    printf("  scale depth. No exceptions are expected.                 \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 90.

      func_call   = L"(Round(i32_val, 1.223) as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", filter, true, func_call);
      CheckReader32(data_reader, true, 9, 90);
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
    // property that is defined by using the function ROUND on the value of
    // a different property of type SINGLE. No exceptions are expected.

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("7. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function ROUND on the value of a different property of   \n");
    printf("  type SINGLE. No exceptions are expected.                 \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that this
      // call returns 1 row. The value for the selected computed property is
      // expected to be 3.

      func_call   = L"(Round(sgl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", filter, true, func_call);
      CheckReaderSgl(data_reader, true, 9, 3);
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

    // 8. Test Case:
    // The test executes a select-command to select the value of a computed
    // property that is defined by using the function ROUND on the value of
    // a different property of type SINGLE. The test requests to round to a
    // certain scale depth. No exceptions are expected.

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("8. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function ROUND on the value of a different property of   \n");
    printf("  type SINGLE. The test requests to round to a certain     \n");
    printf("  scale depth. No exceptions are expected.                 \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 3.1.

      func_call   = L"(Round(sgl_val, 1.223) as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", filter, true, func_call);
      CheckReaderSgl(data_reader, true, 9, (FdoFloat)3.1);
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

}  //  TestRoundFunction ()

void FdoExpressionFunctionTest::TestSignFunction ()

// +---------------------------------------------------------------------------
// | The function executes the test for the expression engine function SIGN
// | when used as a select-parameter.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    FdoStringP                func_call;

    FdoPtr<FdoFilter>         filter;
    FdoPtr<FdoIFeatureReader> data_reader;

    printf("\n");
    printf("========================================================== \n");
    printf(" Current Unit Test Suite: SIGN Function Testing            \n");
    printf("========================================================== \n");
    printf("\n");

    // Define the filter for all tests in this test suite.

    filter = (FdoComparisonCondition::Create(
               FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"id")),
               FdoComparisonOperations_EqualTo, 
               FdoPtr<FdoDataValue>(FdoDataValue::Create(9))));

    // 1. Test Case:
    // The test executes a select-command to select the value of a computed
    // property that is defined by using the function SIGN on the value of a
    // different property of type DECIMAL. No exceptions are expected.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function SIGN on the value of a different property of    \n");
    printf("  type DECIMAL. No exceptions are expected.                \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 1.

      func_call   = L"(Sign(dcl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, 1);
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
    // property that is defined by using the function SIGN on the value of a
    // different property of type DOUBLE. No exceptions are expected.

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("2. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function SIGN on the value of a different property of    \n");
    printf("  type DOUBLE. No exceptions are expected.                 \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be -1.

      func_call   = L"(Sign(dbl_val*-1) as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, -1);
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

}  //  TestSignFunction ()

void FdoExpressionFunctionTest::TestTruncFunction ()

// +---------------------------------------------------------------------------
// | The function executes the test for the expression engine function TRUNC
// | when used as a select-parameter.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    FdoStringP                func_call;

    FdoDateTime               dt_set,
                              expected_dt_data;

    FdoPtr<FdoFilter>         filter;
    FdoPtr<FdoIFeatureReader> data_reader;

    printf("\n");
    printf("========================================================== \n");
    printf(" Current Unit Test Suite: TRUNC Function Testing           \n");
    printf("========================================================== \n");
    printf("\n");

    // Define the filter for all tests in this test suite.

    filter = (FdoComparisonCondition::Create(
               FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"id")),
               FdoComparisonOperations_EqualTo, 
               FdoPtr<FdoDataValue>(FdoDataValue::Create(9))));

    // 1. Test Case:
    // The test executes a select-command to select the value of a computed
    // property that is defined by using the function TRUNC on the value of
    // a different property of type DECIMAL. No exceptions are expected.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function TRUNC on the value of a different property of   \n");
    printf("  type DECIMAL. No exceptions are expected.                \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 12.

      func_call   = L"(Trunc(dcl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, 12);
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
    // property that is defined by using the function TRUNC on the value of
    // a different property of type DECIMAL. The test requests to truncate
    // to a certain scale depth. No exceptions are expected.

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("2. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function TRUNC on the value of a different property of   \n");
    printf("  type DECIMAL. The test requests to truncate to a certain \n");
    printf("  scale depth. No exceptions are expected.                 \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 12.8.

      func_call   = L"(Trunc(dcl_val, 1.223) as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, 12.8);
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
    // property that is defined by using the function TRUNC on the value of
    // a different property of type DOUBLE. No exceptions are expected.

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("3. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function TRUNC on the value of a different property of   \n");
    printf("  type DOUBLE. No exceptions are expected.                 \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 18.

      func_call   = L"(Trunc(dbl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, 18);
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
    // property that is defined by using the function TRUNC on the value of
    // a different property of type DOUBLE. The test requests to truncate
    // to a certain scale depth. No exceptions are expected.

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("4. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function TRUNC on the value of a different property of   \n");
    printf("  type DOUBLE. The test requests to truncate to a certain  \n");
    printf("  scale depth. No exceptions are expected.                 \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 18.8.

      func_call   = L"(Trunc(dbl_val, 1.223) as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, 18.8);
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
    // property that is defined by using the function TRUNC on the value of
    // a different property of type INT32. No exceptions are expected.

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("5. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function TRUNC on the value of a different property of   \n");
    printf("  type INT32. No exceptions are expected.                  \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 90.

      func_call   = L"(Trunc(i32_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", filter, true, func_call);
      CheckReader32(data_reader, true, 9, 90);
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
    // property that is defined by using the function TRUNC on the value of
    // a different property of type INT32. The test requests to truncate to
    // a certain scale depth. No exceptions are expected.

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("6. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function TRUNC on the value of a different property of   \n");
    printf("  type INT32. The test requests to truncate to a certain   \n");
    printf("  scale depth. No exceptions are expected.                 \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 90.

      func_call   = L"(Trunc(i32_val, 1.223) as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", filter, true, func_call);
      CheckReader32(data_reader, true, 9, 90);
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
    // property that is defined by using the function TRUNC on the value of
    // a different property of type SINGLE. No exceptions are expected.

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("7. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function TRUNC on the value of a different property of   \n");
    printf("  type SINGLE. No exceptions are expected.                 \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that this
      // call returns 1 row. The value for the selected computed property is
      // expected to be 3.

      func_call   = L"(Trunc(sgl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", filter, true, func_call);
      CheckReaderSgl(data_reader, true, 9, 3);
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

    // 8. Test Case:
    // The test executes a select-command to select the value of a computed
    // property that is defined by using the function TRUNC on the value of
    // a different property of type SINGLE. The test requests to truncate to
    // a certain scale depth. No exceptions are expected.

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("8. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function TRUNC on the value of a different property of   \n");
    printf("  type SINGLE. The test requests to truncate to a certain  \n");
    printf("  scale depth. No exceptions are expected.                 \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 3.09.

      func_call   = L"(Trunc(sgl_val, 2.223) as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", filter, true, func_call);
      CheckReaderSgl(data_reader, true, 9, (FdoFloat)3.09);
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

    // The following tests deal with the truncation of date/time data. To
    // allow propper execution at all time, get the date/time information
    // that is set for the row used in the following test.

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("Test Case Setup:                                           \n");
    printf("  The following retrieves date/time information used when  \n");
    printf("  cross-checking function results. No exceptions are ex-   \n");
    printf("  pected.                                                  \n");
    printf("---------------------------------------------------------- \n");

    try {

      dt_set = GetDate(L"exfct_c1", L"dt_val", filter);

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

    // 9. Test Case:
    // The test executes a select-command to select the value of a computed
    // property that is defined by using the function TRUNC on the value of
    // a different property of type DATE/TIME. The test requests to truncate
    // to the year. No exceptions are expected.

    printf("---------------------------------------------------------- \n");
    printf("9. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function TRUNC on the value of a different property of   \n");
    printf("  type DATE/TIME. The test requests to truncate to the     \n");
    printf("  year. No exceptions are expected.                        \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to identify January 1st of the year set in the property.

      expected_dt_data.year  = dt_set.year;
      expected_dt_data.month = 1;
      expected_dt_data.day   = 1;

      func_call   = L"(Trunc(dt_val, 'YEAR') as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", filter, true, func_call);
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

    // 10. Test Case:
    // The test executes a select-command to select the value of a computed
    // property that is defined by using the function TRUNC on the value of
    // a different property of type DATE/TIME. The test requests to truncate
    // to the month. No exceptions are expected.

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("10. Test Case:                                             \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function TRUNC on the value of a different property of   \n");
    printf("  type DATE/TIME. The test requests to truncate to the     \n");
    printf("  month. No exceptions are expected.                       \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to identify January 1st of the year set in the pro-
      // perty.

      expected_dt_data.year  = dt_set.year;
      expected_dt_data.month = dt_set.month;
      expected_dt_data.day   = 1;

      func_call   = L"(Trunc(dt_val, 'MONTH') as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", filter, true, func_call);
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

}  //  TestTruncFunction ()


// ===========================================================================
// ==                     TESTING THE STRING FUNCTIONS                      ==
// ===========================================================================

void FdoExpressionFunctionTest::TestConcatFunction ()

// +---------------------------------------------------------------------------
// | The function executes the test for the expression engine function CONCAT
// | when used as a select-parameter.
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

    // 1. Test Case:
    // The test executes a select-command to select the value of a computed
    // property that is defined by using the function CONCAT on values of two
    // different properties of type STRING. No exceptions are expected.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
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
      CheckStringFunctionReader(data_reader, 9, L"  9  The Color is: 2118");
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

void FdoExpressionFunctionTest::TestInstrFunction ()

// +---------------------------------------------------------------------------
// | The function executes the test for the expression engine function INSTR
// | when used as a select-parameter.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    FdoStringP                func_call;

    FdoPtr<FdoFilter>         filter;
    FdoPtr<FdoIFeatureReader> data_reader;

    printf("\n");
    printf("========================================================== \n");
    printf(" Current Unit Test Suite: INSTR Function Testing           \n");
    printf("========================================================== \n");
    printf("\n");

    // Define the filter for all tests in this test suite.

    filter = (FdoComparisonCondition::Create(
               FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"id")),
               FdoComparisonOperations_EqualTo, 
               FdoPtr<FdoDataValue>(FdoDataValue::Create(2))));

    // 1. Test Case:
    // The test executes a select-command to select the value of a computed
    // property that is defined by using the function INSTR on values of two
    // different properties of type STRING. No exceptions are expected.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function INSTR on values of two different properties of  \n");
    printf("  type STRING. No exceptions are expected.                 \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 15 as it looks for the string "2" inside the string
      // "The Color is: 2111".

      func_call   = L"(Instr(str2_val, '2') as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", filter, true, func_call);
      CheckReader64(data_reader, true, 2, 15);
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

}  //  TestInstrFunction ()

void FdoExpressionFunctionTest::TestLengthFunction ()

// +---------------------------------------------------------------------------
// | The function executes the test for the expression engine function LENGTH
// | when used as a select-parameter.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    FdoStringP                func_call;

    FdoPtr<FdoFilter>         filter;
    FdoPtr<FdoIFeatureReader> data_reader;

    printf("\n");
    printf("========================================================== \n");
    printf(" Current Unit Test Suite: LENGTH Function Testing          \n");
    printf("========================================================== \n");
    printf("\n");

    // Define the filter for all tests in this test suite.

    filter = (FdoComparisonCondition::Create(
               FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"id")),
               FdoComparisonOperations_EqualTo, 
               FdoPtr<FdoDataValue>(FdoDataValue::Create(9))));

    // 1. Test Case:
    // The test executes a select-command to select the value of a computed
    // property that is defined by using the function LENGTH on a value of
    // a different property of type STRING. No exceptions are expected.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function LENGTH on a value of a different property of    \n");
    printf("  type STRING. No exceptions are expected.                 \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed pro-
      // perty is expected to be 18 (length of the value "The Color is: 2118"
      // for the selected property).

      func_call   = L"(Length(str2_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, 18);
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

}  //  TestLengthFunction ()

void FdoExpressionFunctionTest::TestLowerFunction ()

// +---------------------------------------------------------------------------
// | The function executes the test for the expression engine function LOWER
// | when used as a select-parameter.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    FdoStringP                func_call;

    FdoPtr<FdoFilter>         filter;
    FdoPtr<FdoIFeatureReader> data_reader;

    printf("\n");
    printf("========================================================== \n");
    printf(" Current Unit Test Suite: LOWER Function Testing           \n");
    printf("========================================================== \n");
    printf("\n");

    // Define the filter for all tests in this test suite.

    filter = (FdoComparisonCondition::Create(
               FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"id")),
               FdoComparisonOperations_EqualTo, 
               FdoPtr<FdoDataValue>(FdoDataValue::Create(9))));

    // 1. Test Case:
    // The test executes a select-command to select the value of a computed
    // property that is defined by using the function LOWER on a value of a
    // different property of type STRING. No exceptions are expected.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function LOWER on a value of a different property of     \n");
    printf("  type STRING. No exceptions are expected.                 \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be "the color is: 2118".

      func_call   = L"(Lower(str2_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", filter, true, func_call);
      CheckStringFunctionReader(data_reader, 9, L"the color is: 2118");
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

}  //  TestLowerFunction ()

void FdoExpressionFunctionTest::TestLpadFunction ()

// +---------------------------------------------------------------------------
// | The function executes the test for the expression engine function LPAD
// | when used as a select-parameter.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    FdoStringP                func_call;

    FdoPtr<FdoFilter>         filter;
    FdoPtr<FdoIFeatureReader> data_reader;

    printf("\n");
    printf("========================================================== \n");
    printf(" Current Unit Test Suite: LPAD Function Testing            \n");
    printf("========================================================== \n");
    printf("\n");

    // Define the filter for all tests in this test suite.

    filter = (FdoComparisonCondition::Create(
               FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"id")),
               FdoComparisonOperations_EqualTo, 
               FdoPtr<FdoDataValue>(FdoDataValue::Create(9))));

    // 1. Test Case:
    // The test executes a select-command to select the value of a computed
    // property that is defined by using the function LPAD on a value of a
    // different property of type STRING. For this test, no padstring is
    // provided. No exceptions are expected.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function LPAD on a value of a different property of type \n");
    printf("  STRING. For this test, no pad-string is provided. No ex- \n");
    printf("  ceptions are expected.                                   \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be "   The Color is: 2118" as the request asks for
      // standard padding (using blanks) to a length of 21.

      func_call   = L"(Lpad(str2_val, 21.43) as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", filter, true, func_call);
      CheckStringFunctionReader(data_reader, 9, L"   The Color is: 2118");
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
    // property that is defined by using the function LPAD on a value of a
    // different property of type STRING. For this test, a pad-string is
    // provided. No exceptions are expected.

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("2. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function LPAD on a value of a different property of type \n");
    printf("  STRING. For this test, a pad-string is provided. No ex-  \n");
    printf("  ceptions are expected.                                   \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be "  9The Color is: 2118" as the request asks for
      // padding using "  9  " to a length of 21.

      func_call   = L"(Lpad(str2_val, 21.43, str_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", filter, true, func_call);
      CheckStringFunctionReader(data_reader, 9, L"  9The Color is: 2118");
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

}  //  TestLpadFunction ()

void FdoExpressionFunctionTest::TestLtrimFunction ()

// +---------------------------------------------------------------------------
// | The function executes the test for the expression engine function LTRIM
// | when used as a select-parameter.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    FdoStringP                func_call;

    FdoPtr<FdoFilter>         filter;
    FdoPtr<FdoIFeatureReader> data_reader;

    printf("\n");
    printf("========================================================== \n");
    printf(" Current Unit Test Suite: LTRIM Function Testing           \n");
    printf("========================================================== \n");
    printf("\n");

    // Define the filter for all tests in this test suite.

    filter = (FdoComparisonCondition::Create(
               FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"id")),
               FdoComparisonOperations_EqualTo, 
               FdoPtr<FdoDataValue>(FdoDataValue::Create(9))));

    // 1. Test Case:
    // The test executes a select-command to select the value of a computed
    // property that is defined by using the function LTRIM on a value of a
    // different property of type STRING. No exceptions are expected.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function LTRIM on a value of a different property of     \n");
    printf("  type STRING. No exceptions are expected.                 \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be "9  ".

      func_call   = L"(Ltrim(str_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", filter, true, func_call);
      CheckStringFunctionReader(data_reader, 9, L"9  ");
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

}  //  TestLtrimFunction ()

void FdoExpressionFunctionTest::TestRpadFunction ()

// +---------------------------------------------------------------------------
// | The function executes the test for the expression engine function RPAD
// | when used as a select-parameter.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    FdoStringP                func_call;

    FdoPtr<FdoFilter>         filter;
    FdoPtr<FdoIFeatureReader> data_reader;

    printf("\n");
    printf("========================================================== \n");
    printf(" Current Unit Test Suite: RPAD Function Testing            \n");
    printf("========================================================== \n");
    printf("\n");

    // Define the filter for all tests in this test suite.

    filter = (FdoComparisonCondition::Create(
               FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"id")),
               FdoComparisonOperations_EqualTo, 
               FdoPtr<FdoDataValue>(FdoDataValue::Create(9))));

    // 1. Test Case:
    // The test executes a select-command to select the value of a computed
    // property that is defined by using the function RPAD on a value of a
    // different property of type STRING. For this test, no pad-string is
    // provided. No exceptions are expected.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function RPAD on a value of a different property of type \n");
    printf("  STRING. For this test, no pad-string is provided. No ex- \n");
    printf("  ceptions are expected.                                   \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be "The Color is: 2118   " as the request asks for
      // standard padding (using blanks) to a length of 21.

      func_call   = L"(Rpad(str2_val, 21.43) as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", filter, true, func_call);
      CheckStringFunctionReader(data_reader, 9, L"The Color is: 2118   ");
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
    // property that is defined by using the function RPAD on a value of a
    // different property of type STRING. For this test, a pad-string is
    // provided. No exceptions are expected.

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("2. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function RPAD on a value of a different property of type \n");
    printf("  STRING. For this test, a pad-string is provided. No ex-  \n");
    printf("  ceptions are expected.                                   \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be "The Color is: 2118  9" as the request asks for
      // padding using "  9  " to a length of 21.

      func_call   = L"(Rpad(str2_val, 21.43, str_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", filter, true, func_call);
      CheckStringFunctionReader(data_reader, 9, L"The Color is: 2118  9");
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

}  //  TestRpadFunction ()

void FdoExpressionFunctionTest::TestRtrimFunction ()

// +---------------------------------------------------------------------------
// | The function executes the test for the expression engine function RTRIM
// | when used as a select-parameter.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    FdoStringP                func_call;

    FdoPtr<FdoFilter>         filter;
    FdoPtr<FdoIFeatureReader> data_reader;

    printf("\n");
    printf("========================================================== \n");
    printf(" Current Unit Test Suite: RTRIM Function Testing           \n");
    printf("========================================================== \n");
    printf("\n");

    // Define the filter for all tests in this test suite.

    filter = (FdoComparisonCondition::Create(
               FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"id")),
               FdoComparisonOperations_EqualTo, 
               FdoPtr<FdoDataValue>(FdoDataValue::Create(9))));

    // 1. Test Case:
    // The test executes a select-command to select the value of a computed
    // property that is defined by using the function RTRIM on a value of a
    // different property of type STRING. No exceptions are expected.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function RTRIM on a value of a different property of     \n");
    printf("  type STRING. No exceptions are expected.                 \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be "  9".

      func_call   = L"(Rtrim(str_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", filter, true, func_call);
      CheckStringFunctionReader(data_reader, 9, L"  9");
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

}  //  TestRtrimFunction ()

void FdoExpressionFunctionTest::TestSoundexFunction ()

// +---------------------------------------------------------------------------
// | The function executes the test for the expression engine function SOUNDEX
// | when used as a select-parameter.
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
      // is expected to be "T246".

      func_call   = L"(Soundex(str2_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", filter, true, func_call);
      CheckStringFunctionReader(data_reader, 9, L"T246");
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

void FdoExpressionFunctionTest::TestSubstrFunction ()

// +---------------------------------------------------------------------------
// | The function executes the test for the expression engine function SUBSTR
// | when used as a select-parameter.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    FdoStringP                func_call;

    FdoPtr<FdoFilter>         filter;
    FdoPtr<FdoIFeatureReader> data_reader;

    printf("\n");
    printf("========================================================== \n");
    printf(" Current Unit Test Suite: SUBSTR Function Testing          \n");
    printf("========================================================== \n");
    printf("\n");

    // Define the filter for all tests in this test suite.

    filter = (FdoComparisonCondition::Create(
               FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"id")),
               FdoComparisonOperations_EqualTo, 
               FdoPtr<FdoDataValue>(FdoDataValue::Create(9))));

    // 1. Test Case:
    // The test executes a select-command to select the value of a computed
    // property that is defined by using the function SUBSTR on the value of
    // a different property of type STRING. In this case, the test requests
    // a sub-string from a position within the source string to the end of it
    // by not specifying the optional length parameter. No exceptions are ex-
    // pected.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function SUBSTR on the value of a different property of  \n");
    printf("  type STRING. In this case, the test requests a sub-      \n");
    printf("  string from a position within the source string to the   \n");
    printf("  end of it by not specifying the optional length parame-  \n");
    printf("  ter. No exceptions are expected.                         \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be "Color is: 2118".

      func_call   = L"(Substr(str2_val, 5.6) as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", filter, true, func_call);
      CheckStringFunctionReader(data_reader, 9, L"Color is: 2118");
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
    // property that is defined by using the function SUBSTR on the value of
    // a different property of type STRING. In this case, the test requests a
    // sub-string from a position within the source string for a predefined
    // length. No exceptions are expected.

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("2. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function SUBSTR on the value of a different property of  \n");
    printf("  type STRING. In this case, the test requests a sub-      \n");
    printf("  string from a position within the source string for a    \n");
    printf("  predefined length. No exceptions are expected.           \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be "Color".

      func_call   = L"(Substr(str2_val, 5.6, 5.6) as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", filter, true, func_call);
      CheckStringFunctionReader(data_reader, 9, L"Color");
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

}  //  TestSubstrFunction ()

void FdoExpressionFunctionTest::TestTranslateFunction ()

// +---------------------------------------------------------------------------
// | The function executes the test for the expression engine function TRANS-
// | LATE when used as a select-parameter.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    FdoStringP                func_call;

    FdoPtr<FdoFilter>         filter;
    FdoPtr<FdoIFeatureReader> data_reader;

    printf("\n");
    printf("========================================================== \n");
    printf(" Current Unit Test Suite: TRANSLATE Function Testing       \n");
    printf("========================================================== \n");
    printf("\n");

    // Define the filter for all tests in this test suite.

    filter = (FdoComparisonCondition::Create(
               FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"id")),
               FdoComparisonOperations_EqualTo, 
               FdoPtr<FdoDataValue>(FdoDataValue::Create(9))));

    // 1. Test Case:
    // The test executes a select-command to select the value of a computed
    // property that is defined by using the function TRANSLATE on the value
    // of a different property of type STRING. In this case, the test requests
    // to substitute all 'o' with an 'x' and all 'l' with a 'y'. No exceptions
    // are expected.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function TRANSLATE on the value of a different property  \n");
    printf("  of type STRING. In this case, the test requests to sub-  \n");
    printf("  stitute all 'o' with an 'x' and all 'l' with a 'y'. No   \n");
    printf("  exceptions are expected.                                 \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be "The Cxyxr is: 2118".

      func_call   = L"(Translate(str2_val, 'ol', 'xy') as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", filter, true, func_call);
      CheckStringFunctionReader(data_reader, 9, L"The Cxyxr is: 2118");
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

}  //  TestTranslateFunction ()

void FdoExpressionFunctionTest::TestTrimFunction ()

// +---------------------------------------------------------------------------
// | The function executes the test for the expression engine function TRIM
// | when used as a select-parameter.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    FdoStringP                func_call;

    FdoPtr<FdoFilter>         filter;
    FdoPtr<FdoIFeatureReader> data_reader;

    printf("\n");
    printf("========================================================== \n");
    printf(" Current Unit Test Suite: TRIM Function Testing            \n");
    printf("========================================================== \n");
    printf("\n");

    // Define the filter for all tests in this test suite.

    filter = (FdoComparisonCondition::Create(
               FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"id")),
               FdoComparisonOperations_EqualTo, 
               FdoPtr<FdoDataValue>(FdoDataValue::Create(9))));

    // 1. Test Case:
    // The test executes a select-command to select the value of a computed
    // property that is defined by using the function TRIM on a value of a
    // different property of type STRING. For this test, no execution operator
    // is set. No exceptions are expected.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function TRIM on a value of a different property of type \n");
    printf("  STRING. For the test, no execution operator is set. No   \n");
    printf("  exceptions are expected.                                 \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be "9" because the operation is called on the property
      // value "  9  " without specifying a operation mode and hence the de-
      // fault one should be used in which case leading and trailing blanks are
      // removed.

      func_call   = L"(Trim(str_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", filter, true, func_call);
      CheckStringFunctionReader(data_reader, 9, L"9");
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
    // property that is defined by using the function TRIM on a value of a
    // different property of type STRING. For this test, the execution operator
    // is set to BOTH. No exceptions are expected.

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("2. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function TRIM on a value of a different property of type \n");
    printf("  STRING. For the test, the execution operator is set to   \n");
    printf("  BOTH. No exceptions are expected.                        \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be "9" because the operation is called on the property
      // value "  9  " specifying the operation type BOTH in which case
      // leading and trailing blanks are removed.

      func_call   = L"(Trim('BOTH', str_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", filter, true, func_call);
      CheckStringFunctionReader(data_reader, 9, L"9");
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
    // property that is defined by using the function TRIM on a value of a
    // different property of type STRING. For this test, the execution operator
    // is set to LEADING. No exceptions are expected.

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("3. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function TRIM on a value of a different property of type \n");
    printf("  STRING. For the test, the execution operator is set to   \n");
    printf("  LEADING. No exceptions are expected.                     \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be "9  " because the operation is called on the property
      // value "  9  " specifying the operation type LEADING in which case any
      // leading blanks are removed.

      func_call   = L"(Trim('LEADING', str_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", filter, true, func_call);
      CheckStringFunctionReader(data_reader, 9, L"9  ");
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
    // property that is defined by using the function TRIM on a value of a
    // different property of type STRING. For this test, the execution operator
    // is set to TRAILING. No exceptions are expected.

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("4. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function TRIM on a value of a different property of type \n");
    printf("  STRING. For the test, the execution operator is set to   \n");
    printf("  TRAILING. No exceptions are expected.                    \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be "  9" because the operation is called on the property
      // value "  9  " specifying the operation type TRAILING in which case any
      // trailing blanks are removed.

      func_call   = L"(Trim('TRAILING', str_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", filter, true, func_call);
      CheckStringFunctionReader(data_reader, 9, L"  9");
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

}  //  TestTrimFunction ()

void FdoExpressionFunctionTest::TestUpperFunction ()

// +---------------------------------------------------------------------------
// | The function executes the test for the expression engine function UPPER
// | when used as a select-parameter.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    FdoStringP                func_call;

    FdoPtr<FdoFilter>         filter;
    FdoPtr<FdoIFeatureReader> data_reader;

    printf("\n");
    printf("========================================================== \n");
    printf(" Current Unit Test Suite: UPPER Function Testing           \n");
    printf("========================================================== \n");
    printf("\n");

    // Define the filter for all tests in this test suite.

    filter = (FdoComparisonCondition::Create(
               FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"id")),
               FdoComparisonOperations_EqualTo, 
               FdoPtr<FdoDataValue>(FdoDataValue::Create(9))));

    // 1. Test Case:
    // The test executes a select-command to select the value of a computed
    // property that is defined by using the function UPPER on a value of a
    // different property of type STRING. No exceptions are expected.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function UPPER on a value of a different property of     \n");
    printf("  type STRING. No exceptions are expected.                 \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be "THE COLOR IS: 2118".

      func_call   = L"(Upper(str2_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", filter, true, func_call);
      CheckStringFunctionReader(data_reader, 9, L"THE COLOR IS: 2118");
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

}  //  TestUpperFunction ()


// ----------------------------------------------------------------------------
// --                      General Supporting Functions                      --
// ----------------------------------------------------------------------------

void FdoExpressionFunctionTest::CheckDateTimeReader (
                                        FdoIDataReader *data_reader,
                                        bool           include_id_check,
                                        bool           is_extract_request,
                                        FdoInt16       extract_type_id,
                                        FdoInt32       expected_id_value,
                                        FdoDateTime    expected_cmp_id_value)

// +---------------------------------------------------------------------------
// | The function checks whether or not the provided reader contains the expec-
// | ted data and throws an exception if this is not the case.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    bool        is_valid_result = false;

    FdoInt32    data_count      = 0,
                id_prop_val;

    FdoDateTime cmp_id_val;

    // Navigate through the reader and perform the necessary checks.

    printf(" >>> Cross check result \n");

    while (data_reader->ReadNext()) {

      data_count++;

      if (include_id_check)
          id_prop_val = data_reader->GetInt32(L"id");
      cmp_id_val  = data_reader->GetDateTime(L"cmp_id");

      if (!is_extract_request)
          is_valid_result =
                    ((cmp_id_val.year  == expected_cmp_id_value.year ) &&
                     (cmp_id_val.month == expected_cmp_id_value.month) &&
                     (cmp_id_val.day   == expected_cmp_id_value.day  )    );
      else
        switch (extract_type_id) {

          case 0:  // Extract the YEAR portion
            is_valid_result = (cmp_id_val.year == expected_cmp_id_value.year);
            break;

          case 1:  // Extract the MONTH portion
            is_valid_result =
                            (cmp_id_val.month == expected_cmp_id_value.month);
            break;

          case 2:  // Extract the DAY portion
            is_valid_result = (cmp_id_val.day == expected_cmp_id_value.day);
            break;

          case 3:  // Extract the HOUR portion
            is_valid_result = (cmp_id_val.hour == expected_cmp_id_value.hour);
            break;

          case 4:  // Extract the MINUTE portion
            is_valid_result =
                        (cmp_id_val.minute == expected_cmp_id_value.minute);
            break;

          case 5:  // Extract the SECOND portion
            is_valid_result =
                        (cmp_id_val.seconds == expected_cmp_id_value.seconds);
            break;

        }  //  switch ...

      if (include_id_check)
          is_valid_result =
                        is_valid_result && (id_prop_val == expected_id_value);

      if (!is_valid_result)
          break;

    }  //  while (data_reader-> ...

    // Close the reader.

    data_reader->Close();

    // Issue an exception if the expected result is not met.

    if ((!is_valid_result) || (data_count != 1))
         throw FdoException::Create(
                        L"Unexpected result(s) when checking returned data");
    else
      printf(" >>> ... All expected data found\n");

}  //  CheckDateTimeReader ()

void FdoExpressionFunctionTest::CheckDateTimeReader (
                                    FdoIFeatureReader *data_reader,
                                    bool              include_id_check,
                                    bool              is_extract_request,
                                    FdoInt16          extract_type_id,
                                    FdoInt32          expected_id_value,
                                    FdoDateTime       expected_cmp_id_value)

// +---------------------------------------------------------------------------
// | The function checks whether or not the provided reader contains the expec-
// | ted data and throws an exception if this is not the case.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    bool        is_valid_result = false;

    FdoInt32    data_count      = 0,
                id_prop_val;

    FdoDateTime cmp_id_val;

    // Navigate through the reader and perform the necessary checks.

    printf(" >>> Cross check result \n");

    while (data_reader->ReadNext()) {

      data_count++;

      if (include_id_check)
          id_prop_val = data_reader->GetInt32(L"id");
      cmp_id_val  = data_reader->GetDateTime(L"cmp_id");

      if (!is_extract_request)
          is_valid_result =
                    ((cmp_id_val.year  == expected_cmp_id_value.year ) &&
                     (cmp_id_val.month == expected_cmp_id_value.month) &&
                     (cmp_id_val.day   == expected_cmp_id_value.day  )    );
      else
        switch (extract_type_id) {

          case 0:  // Extract the YEAR portion
            is_valid_result = (cmp_id_val.year == expected_cmp_id_value.year);
            break;

          case 1:  // Extract the MONTH portion
            is_valid_result =
                            (cmp_id_val.month == expected_cmp_id_value.month);
            break;

          case 2:  // Extract the DAY portion
            is_valid_result = (cmp_id_val.day == expected_cmp_id_value.day);
            break;

          case 3:  // Extract the HOUR portion
            is_valid_result = (cmp_id_val.hour == expected_cmp_id_value.hour);
            break;

          case 4:  // Extract the MINUTE portion
            is_valid_result =
                        (cmp_id_val.minute == expected_cmp_id_value.minute);
            break;

          case 5:  // Extract the SECOND portion
            is_valid_result =
                        (cmp_id_val.seconds == expected_cmp_id_value.seconds);
            break;

        }  //  switch ...

      if (include_id_check)
          is_valid_result =
                        is_valid_result && (id_prop_val == expected_id_value);

      if (!is_valid_result)
          break;

    }  //  while (data_reader-> ...

    // Close the reader.

    data_reader->Close();

    // Issue an exception if the expected result is not met.

    if ((!is_valid_result) || (data_count != 1))
         throw FdoException::Create(
                        L"Unexpected result(s) when checking returned data");
    else
      printf(" >>> ... All expected data found\n");

}  //  CheckDateTimeReader ()

void FdoExpressionFunctionTest::CheckReader (FdoIDataReader *data_reader,
                                             bool           include_id_check,
                                             FdoInt32       expected_id_value,
                                             FdoDouble      expected_value)

// +---------------------------------------------------------------------------
// | The function checks whether or not the provided reader contains the ex-
// | pected data and throws an exception if this is not the case.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    bool      is_valid_result = false;

    FdoInt32  data_count      = 0,
              id_prop_val;

    FdoDouble tolerance,
              cmp_id_val;

    // Navigate through the reader and perform the necessary checks.

    printf(" >>> Cross check result \n");

    while (data_reader->ReadNext()) {

      data_count++;

      if (include_id_check)
          id_prop_val = data_reader->GetInt32(L"id");
      cmp_id_val = data_reader->GetDouble(L"cmp_id");

      tolerance = (cmp_id_val - expected_value);
      if (tolerance < 0)
          tolerance = tolerance * -1;

      is_valid_result = (include_id_check)
            ? ((tolerance < 1) && (id_prop_val == expected_id_value))
            : (tolerance < 1);

      if (!is_valid_result)
          break;

    }  //  while (data_reader-> ...

    // Close the reader.

    data_reader->Close();

    // Issue an exception if the expected result is not met.

    if ((!is_valid_result) || (data_count != 1))
         throw FdoException::Create(
                        L"Unexpected result(s) when checking returned data");
    else
      printf(" >>> ... All expected data found\n");

}  //  CheckReader ()

void FdoExpressionFunctionTest::CheckReader (
                                        FdoIFeatureReader *data_reader,
                                        bool              include_id_check,
                                        FdoInt32          expected_id_value,
                                        FdoDouble         expected_value)

// +---------------------------------------------------------------------------
// | The function checks whether or not the provided reader contains the ex-
// | pected data and throws an exception if this is not the case.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    bool      is_valid_result = false;

    FdoInt32  data_count      = 0,
              id_prop_val;

    FdoDouble tolerance,
              cmp_id_val;

    // Navigate through the reader and perform the necessary checks.

    printf(" >>> Cross check result \n");

    while (data_reader->ReadNext()) {

      data_count++;

      if (include_id_check)
          id_prop_val = data_reader->GetInt32(L"id");
      cmp_id_val = data_reader->GetDouble(L"cmp_id");

      tolerance = (cmp_id_val - expected_value);
      if (tolerance < 0)
          tolerance = tolerance * -1;

      is_valid_result = (include_id_check)
            ? ((tolerance < 1) && (id_prop_val == expected_id_value))
            : (tolerance < 1);

      if (!is_valid_result)
          break;

    }  //  while (data_reader-> ...

    // Close the reader.

    data_reader->Close();

    // Issue an exception if the expected result is not met.

    if ((!is_valid_result) || (data_count != 1))
         throw FdoException::Create(
                        L"Unexpected result(s) when checking returned data");
    else
      printf(" >>> ... All expected data found\n");

}  //  CheckReader ()

void FdoExpressionFunctionTest::CheckReader32 (
                                            FdoIDataReader *data_reader,
                                            bool           include_id_check,
                                            FdoInt32       expected_id_value,
                                            FdoInt32       expected_value)

// +---------------------------------------------------------------------------
// | The function checks whether or not the provided reader contains the ex-
// | pected data and throws an exception if this is not the case.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    bool     is_valid_result = false;

    FdoInt32 tolerance,
             data_count      = 0,
             cmp_id_val,
             id_prop_val;

    // Navigate through the reader and perform the necessary checks.

    printf(" >>> Cross check result \n");

    while (data_reader->ReadNext()) {

      data_count++;

      if (include_id_check)
          id_prop_val = data_reader->GetInt32(L"id");
      cmp_id_val = data_reader->GetInt32(L"cmp_id");

      tolerance = (cmp_id_val - expected_value);
      if (tolerance < 0)
          tolerance = tolerance * -1;

      is_valid_result = (include_id_check)
            ? ((tolerance < 1) && (id_prop_val == expected_id_value))
            : (tolerance < 1);

      if (!is_valid_result)
          break;

    }  //  while (data_reader-> ...

    // Close the reader.

    data_reader->Close();

    // Issue an exception if the expected result is not met.

    if ((!is_valid_result) || (data_count != 1))
         throw FdoException::Create(
                        L"Unexpected result(s) when checking returned data");
    else
      printf(" >>> ... All expected data found\n");

}  //  CheckReader32 ()

void FdoExpressionFunctionTest::CheckReader32 (
                                        FdoIFeatureReader *data_reader,
                                        bool              include_id_check,
                                        FdoInt32          expected_id_value,
                                        FdoInt32          expected_value)

// +---------------------------------------------------------------------------
// | The function checks whether or not the provided reader contains the ex-
// | pected data and throws an exception if this is not the case.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    bool     is_valid_result = false;

    FdoInt32 tolerance,
             data_count      = 0,
             cmp_id_val,
             id_prop_val;

    // Navigate through the reader and perform the necessary checks.

    printf(" >>> Cross check result \n");

    while (data_reader->ReadNext()) {

      data_count++;

      if (include_id_check)
          id_prop_val = data_reader->GetInt32(L"id");
      cmp_id_val = data_reader->GetInt32(L"cmp_id");

      tolerance = (cmp_id_val - expected_value);
      if (tolerance < 0)
          tolerance = tolerance * -1;

      is_valid_result = (include_id_check)
            ? ((tolerance < 1) && (id_prop_val == expected_id_value))
            : (tolerance < 1);

      if (!is_valid_result)
          break;

    }  //  while (data_reader-> ...

    // Close the reader.

    data_reader->Close();

    // Issue an exception if the expected result is not met.

    if ((!is_valid_result) || (data_count != 1))
         throw FdoException::Create(
                        L"Unexpected result(s) when checking returned data");
    else
      printf(" >>> ... All expected data found\n");

}  //  CheckReader32 ()

void FdoExpressionFunctionTest::CheckReader64 (
                                            FdoIDataReader *data_reader,
                                            bool           include_id_check,
                                            FdoInt32       expected_id_value,
                                            FdoInt64       expected_value)

// +---------------------------------------------------------------------------
// | The function checks whether or not the provided reader contains the ex-
// | pected data and throws an exception if this is not the case.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    bool     is_valid_result = false;

    FdoInt32 data_count      = 0,
             id_prop_val;

    FdoInt64 tolerance,
             cmp_id_val;

    // Navigate through the reader and perform the necessary checks.

    printf(" >>> Cross check result \n");

    while (data_reader->ReadNext()) {

      data_count++;

      if (include_id_check)
          id_prop_val = data_reader->GetInt32(L"id");
      cmp_id_val = data_reader->GetInt64(L"cmp_id");

      tolerance = (cmp_id_val - expected_value);
      if (tolerance < 0)
          tolerance = tolerance * -1;

      is_valid_result = (include_id_check)
            ? ((tolerance < 1) && (id_prop_val == expected_id_value))
            : (tolerance < 1);

      if (!is_valid_result)
          break;

    }  //  while (data_reader-> ...

    // Close the reader.

    data_reader->Close();

    // Issue an exception if the expected result is not met.

    if ((!is_valid_result) || (data_count != 1))
         throw FdoException::Create(
                        L"Unexpected result(s) when checking returned data");
    else
      printf(" >>> ... All expected data found\n");

}  //  CheckReader64 ()

void FdoExpressionFunctionTest::CheckReader64 (
                                        FdoIFeatureReader *data_reader,
                                        bool              include_id_check,
                                        FdoInt32          expected_id_value,
                                        FdoInt64          expected_value)

// +---------------------------------------------------------------------------
// | The function checks whether or not the provided reader contains the ex-
// | pected data and throws an exception if this is not the case.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    bool     is_valid_result = false;

    FdoInt32 data_count      = 0,
             id_prop_val;

    FdoInt64 tolerance,
             cmp_id_val;

    // Navigate through the reader and perform the necessary checks.

    printf(" >>> Cross check result \n");

    while (data_reader->ReadNext()) {

      data_count++;

      if (include_id_check)
          id_prop_val = data_reader->GetInt32(L"id");
      cmp_id_val = data_reader->GetInt64(L"cmp_id");

      tolerance = (cmp_id_val - expected_value);
      if (tolerance < 0)
          tolerance = tolerance * -1;

      is_valid_result = (include_id_check)
            ? ((tolerance < 1) && (id_prop_val == expected_id_value))
            : (tolerance < 1);

      if (!is_valid_result)
          break;

    }  //  while (data_reader-> ...

    // Close the reader.

    data_reader->Close();

    // Issue an exception if the expected result is not met.

    if ((!is_valid_result) || (data_count != 1))
         throw FdoException::Create(
                        L"Unexpected result(s) when checking returned data");
    else
      printf(" >>> ... All expected data found\n");

}  //  CheckReader64 ()

void FdoExpressionFunctionTest::CheckReaderSgl (
                                            FdoIDataReader *data_reader,
                                            bool           include_id_check,
                                            FdoInt32       expected_id_value,
                                            FdoFloat       expected_value)

// +---------------------------------------------------------------------------
// | The function checks whether or not the provided reader contains the ex-
// | pected data and throws an exception if this is not the case.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    bool     is_valid_result = false;

    FdoInt32 data_count      = 0,
             id_prop_val;

    FdoFloat tolerance,
             cmp_id_val;

    // Navigate through the reader and perform the necessary checks.

    printf(" >>> Cross check result \n");

    while (data_reader->ReadNext()) {

      data_count++;

      if (include_id_check)
          id_prop_val = data_reader->GetInt32(L"id");
      cmp_id_val = data_reader->GetSingle(L"cmp_id");

      tolerance = (cmp_id_val - expected_value);
      if (tolerance < 0)
          tolerance = tolerance * -1;

      is_valid_result = (include_id_check)
            ? ((tolerance < 1) && (id_prop_val == expected_id_value))
            : (tolerance < 1);

      if (!is_valid_result)
          break;

    }  //  while (data_reader-> ...

    // Close the reader.

    data_reader->Close();

    // Issue an exception if the expected result is not met.

    if ((!is_valid_result) || (data_count != 1))
         throw FdoException::Create(
                        L"Unexpected result(s) when checking returned data");
    else
      printf(" >>> ... All expected data found\n");

}  //  CheckReaderSgl ()

void FdoExpressionFunctionTest::CheckReaderSgl (
                                        FdoIFeatureReader *data_reader,
                                        bool              include_id_check,
                                        FdoInt32          expected_id_value,
                                        FdoFloat          expected_value)

// +---------------------------------------------------------------------------
// | The function checks whether or not the provided reader contains the ex-
// | pected data and throws an exception if this is not the case.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    bool     is_valid_result = false;

    FdoInt32 data_count      = 0,
             id_prop_val;

    FdoFloat tolerance,
             cmp_id_val;

    // Navigate through the reader and perform the necessary checks.

    printf(" >>> Cross check result \n");

    while (data_reader->ReadNext()) {

      data_count++;

      if (include_id_check)
          id_prop_val = data_reader->GetInt32(L"id");
      cmp_id_val = data_reader->GetSingle(L"cmp_id");

      tolerance = (cmp_id_val - expected_value);
      if (tolerance < 0)
          tolerance = tolerance * -1;

      is_valid_result = (include_id_check)
            ? ((tolerance < 1) && (id_prop_val == expected_id_value))
            : (tolerance < 1);

      if (!is_valid_result)
          break;

    }  //  while (data_reader-> ...

    // Close the reader.

    data_reader->Close();

    // Issue an exception if the expected result is not met.

    if ((!is_valid_result) || (data_count != 1))
         throw FdoException::Create(
                        L"Unexpected result(s) when checking returned data");
    else
      printf(" >>> ... All expected data found\n");

}  //  CheckReaderSgl ()

void FdoExpressionFunctionTest::CheckStringFunctionReader (
                                    FdoIFeatureReader *data_reader,
                                    FdoInt32          expected_id_value,
                                    FdoString         *expected_cmp_id_value)

// +---------------------------------------------------------------------------
// | The function checks whether or not the provided reader contains the expec-
// | ted data and throws an exception if this is not the case.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    bool      is_valid_result = false;

    FdoInt32  data_count      = 0,
              id_prop_val;

    FdoString *cmp_id_val;

    // Navigate through the reader and perform the necessary checks.

    printf(" >>> Cross check result \n");

    while (data_reader->ReadNext()) {

      data_count++;

      id_prop_val     = data_reader->GetInt32(L"id");
      cmp_id_val      = data_reader->GetString(L"cmp_id");
      is_valid_result = (wcscmp(cmp_id_val, expected_cmp_id_value) == 0);
      if (!is_valid_result)
          break;

    }  //  while (data_reader-> ...

    // Close the reader.

    data_reader->Close();

    // Issue an exception if the expected result is not met.

    if ((!is_valid_result) || (data_count != 1))
         throw FdoException::Create(
                        L"Unexpected result(s) when checking returned data");
    else
      printf(" >>> ... All expected data found\n");

}  //  CheckStringFunctionReader ()

FdoIDataReader *FdoExpressionFunctionTest::ExecuteSelAggrCommand (
                                                        FdoString *class_name,
                                                        FdoFilter *filter,
                                                        bool      inc_id_prop,
                                                        FdoString *cmp_id_str)

// +---------------------------------------------------------------------------
// | The function executes a select-aggregate command using the provided data
// | and returns the generated reader back to the calling routine.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    FdoPtr<FdoIdentifier>           id_prop;

    FdoPtr<FdoISelectAggregates>    sel_aggreg_cmd;
    FdoPtr<FdoComputedIdentifier>   cmp_id;
    FdoPtr<FdoIdentifierCollection> id_col;

    // Create a select-aggregate function and set the class name and filter.

    printf(" >>> Setup the aggregate command \n");

    sel_aggreg_cmd =
            (FdoISelectAggregates*)m_connection->CreateCommand(
                                            FdoCommandType_SelectAggregates);
    sel_aggreg_cmd->SetFeatureClassName(class_name);
    if (filter != NULL)
        sel_aggreg_cmd->SetFilter(filter);
    id_col = sel_aggreg_cmd->GetPropertyNames();
    if (inc_id_prop) {

        id_prop = FdoIdentifier::Create(L"id");
        id_col->Add(id_prop);

    }  //  if (inc_id_prop) ...

    // Define the computed identity to be selected in this test if required.

    if (cmp_id_str != NULL) {

      cmp_id = (FdoComputedIdentifier*)FdoExpression::Parse(cmp_id_str);
      id_col->Add(cmp_id);

    }  //  if (cmp_id != NULL) ...

    // Execute the request and return the data reader back to the calling pro-
    // cedure.

    printf(" >>> Execute test \n");

    return (sel_aggreg_cmd->Execute());

}  //  ExecuteSelAggrCommand ()

FdoIFeatureReader *FdoExpressionFunctionTest::ExecuteSelectCommand (
                                                        FdoString *class_name,
                                                        FdoFilter *filter,
                                                        bool      inc_id_prop,
                                                        FdoString *cmp_id_str)

// +---------------------------------------------------------------------------
// | The function executes a select-command using the provided data and returns
// | the generated reader back to the calling routine.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    FdoPtr<FdoIdentifier>           id_prop;

    FdoPtr<FdoISelect>              select_cmd;
    FdoPtr<FdoComputedIdentifier>   cmp_id;
    FdoPtr<FdoIdentifierCollection> id_col;

    // Create a select function and set the class name and filter.

    printf(" >>> Setup the select command \n");

    select_cmd =
            (FdoISelect*)m_connection->CreateCommand(FdoCommandType_Select);
    select_cmd->SetFeatureClassName(class_name);
    if (filter != NULL)
        select_cmd->SetFilter(filter);
    id_col = select_cmd->GetPropertyNames();
    if (inc_id_prop) {

        id_prop = FdoIdentifier::Create(L"id");
        id_col->Add(id_prop);

    }  //  if (inc_id_prop) ...

    // Define the computed identity to be selected in this test if required.

    if (cmp_id_str != NULL) {

      cmp_id = (FdoComputedIdentifier*)FdoExpression::Parse(cmp_id_str);
      id_col->Add(cmp_id);

    }  //  if (cmp_id != NULL) ...

    // Execute the request and return the data reader back to the calling pro-
    // cedure.

    printf(" >>> Execute test \n");

    return (select_cmd->Execute());

}  //  ExecuteSelectCommand ()

FdoDateTime FdoExpressionFunctionTest::GetDate (FdoString *class_name,
                                                FdoString *property_name,
                                                FdoFilter *filter)

// +---------------------------------------------------------------------------
// | The function retrieves the date set for a property identified by the pro-
// | vided filter and returns this one back to the calling procedure.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    FdoDateTime                     dt_data;

    FdoPtr<FdoIdentifier>           id_prop;

    FdoPtr<FdoISelect>              select_cmd;
    FdoPtr<FdoIFeatureReader>       feature_reader;
    FdoPtr<FdoIdentifierCollection> id_col;

    // Create the select-command, set the properties and exeute it.

    select_cmd =
            (FdoISelect*)m_connection->CreateCommand(FdoCommandType_Select);

    select_cmd->SetFeatureClassName(class_name);
    select_cmd->SetFilter(filter);
    id_col = select_cmd->GetPropertyNames();
    id_prop = FdoIdentifier::Create(property_name);
    id_col->Add(id_prop);

    feature_reader = select_cmd->Execute();

    // Get the necessary information and return it.

    while (feature_reader->ReadNext())
      dt_data = feature_reader->GetDateTime(property_name);

    return dt_data;

}  //  GetDate ()

FdoDouble FdoExpressionFunctionTest::GetExpectedCountValue (
                                                    FdoInt16 test_case_id)

// +---------------------------------------------------------------------------
// | The function returns the number of rows expected to be returned by the
// | function testing the expression function COUNT. The value differs on the
// | underlying database server. In MySQL, for example, NULL values are not
// | added to the total count whereas in Oracle those rows are.
// +---------------------------------------------------------------------------

{

    switch (test_case_id) {

      case 1: return 31; break;
      case 2: return 31; break;

    }  //  switch ...

    // The previous call should have handled all valid cases. Just in case
    // return -1 to indicate an error if this is not the case.

    return -1;

}  //  GetExpectedCountValue ()


// ----------------------------------------------------------------------------
// --                    Test Environment Setup Functions                    --
// ----------------------------------------------------------------------------

void FdoExpressionFunctionTest::AddFeature (
                                        FdoIConnection *current_connection,
                                        FdoString      *class_name,
                                        int            index)

// +---------------------------------------------------------------------------
// | The function adds a new object for the specified class. The values being
// | added are predefined based on the predefined schema.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    double                     coordinate_buffer[7];

    FdoByte                    byte_value;

    FdoFloat                   flt_value;

    FdoInt32                   i                    = 0,
                               dimension_type,
                               dimensionality       = 3;

    FdoDouble                  dbl_value;

    FdoStringP                 id_str;

    FdoBoolean                 bool_value           = false;

    FdoIInsert                 *insert_command      = NULL;

    FdoDateTime                dt;

	FdoDataValue               *data_value          = NULL;

    FdoByteArray               *byte_array          = NULL;

    FdoILineString             *line_str            = NULL;

    FdoGeometryValue           *geometry_value      = NULL;

	FdoPropertyValue           *property_value      = NULL;

    FdoIFeatureReader          *feature_reader      = NULL;

    FdoFgfGeometryFactory      *geometry_factory    = NULL;

    FdoPropertyValueCollection *property_values     = NULL;

    try {

      // Create the FdoIInsert command and set the necessary command properties.

      insert_command = 
            (FdoIInsert *) current_connection->CreateCommand(
                                                        FdoCommandType_Insert);
      insert_command->SetFeatureClassName(class_name);

      // Get hold of the class property set.

	  property_values = insert_command->GetPropertyValues();

      // Add the geometry information for the new object.

      dimensionality = GetDimensionality();

      coordinate_buffer[i] = 100.0 + index;
      coordinate_buffer[i++] = 100.0 + index;
      if (dimensionality == 3)
          coordinate_buffer[i++] = 0;
      coordinate_buffer[i++] = 101.0 + index;
      coordinate_buffer[i++] = 101.0 + index;
      if (dimensionality == 3)
          coordinate_buffer[i] = 0;

      dimension_type = (dimensionality == 3)
                     ? FdoDimensionality_XY|FdoDimensionality_Z
                     : FdoDimensionality_XY;

      geometry_factory = FdoFgfGeometryFactory::GetInstance();
      line_str         = geometry_factory->CreateLineString(dimension_type,
                                                            i+1, 
                                                            coordinate_buffer);
      byte_array       = geometry_factory->GetFgf(line_str);
      geometry_value   = FdoGeometryValue::Create(byte_array);

      property_value = AddNewProperty(property_values, L"RDBMS_GEOM");
      property_value->SetValue(geometry_value);
      FDO_SAFE_RELEASE(geometry_value);
      FDO_SAFE_RELEASE(line_str);
      FDO_SAFE_RELEASE(byte_array);
      FDO_SAFE_RELEASE(geometry_value);
      FDO_SAFE_RELEASE(property_value);

      // Set the new object's properties.

      data_value     = FdoDataValue::Create(index);
      property_value = AddNewProperty(property_values, L"id");
      property_value->SetValue(data_value);
      FDO_SAFE_RELEASE(data_value);
      FDO_SAFE_RELEASE(property_value);

      bool_value     = ((index % 2) == 0);
      data_value     = FdoDataValue::Create(bool_value);
      property_value = AddNewProperty(property_values, L"bool_val");
      property_value->SetValue(data_value);
      FDO_SAFE_RELEASE(data_value);
      FDO_SAFE_RELEASE(property_value);

      byte_value     = index + 65;
      data_value     = FdoDataValue::Create(byte_value);
      property_value = AddNewProperty(property_values, L"byte_val");
      property_value->SetValue(data_value);
      FDO_SAFE_RELEASE(data_value);
      FDO_SAFE_RELEASE(property_value);

      if (index != 21) {

          dt.year        = 2007;
          dt.month       = 9;
          dt.day         = ((index % 30) == 0) ? 1 : (index % 30);
          dt.hour        = 21;
          dt.minute      = 0;
          dt.seconds     = 0;
          data_value     = FdoDataValue::Create(dt);
          property_value = AddNewProperty(property_values, L"dt_val");
          property_value->SetValue(data_value);
          FDO_SAFE_RELEASE(data_value);
          FDO_SAFE_RELEASE(property_value);

      }  //  if (index != 21) ...

      if (index != 22) {

          dt.year        = 2008;
          dt.month       = 5;
          dt.day         = ((index % 31) == 0) ? 1 : (index % 31);
          dt.hour        = 21;
          dt.minute      = 0;
          dt.seconds     = 0;
          data_value     = FdoDataValue::Create(dt);
          property_value = AddNewProperty(property_values, L"dt2_val");
          property_value->SetValue(data_value);
          FDO_SAFE_RELEASE(data_value);
          FDO_SAFE_RELEASE(property_value);

      }  //  if (index != 22) ...

      if (index != 21) {

          dbl_value      = index + (3.8 * index/8.9);
          data_value     = FdoDataValue::Create(dbl_value, FdoDataType_Decimal);
          property_value = AddNewProperty(property_values, L"dcl_val");
          property_value->SetValue(data_value);
          FDO_SAFE_RELEASE(data_value);
          FDO_SAFE_RELEASE(property_value);

      }  //  if (index != 21) ...

      if (index != 22) {

          dbl_value      = index + (6.34 * index/5.77);
          data_value     = FdoDataValue::Create(dbl_value, FdoDataType_Double);
          property_value = AddNewProperty(property_values, L"dbl_val");
          property_value->SetValue(data_value);
          FDO_SAFE_RELEASE(data_value);
          FDO_SAFE_RELEASE(property_value);

      }  //  if (index != 22) ...

      data_value     = FdoDataValue::Create((index * 4));
      property_value = AddNewProperty(property_values, L"i16_val");
      property_value->SetValue(data_value);
      FDO_SAFE_RELEASE(data_value);
      FDO_SAFE_RELEASE(property_value);

      data_value     = FdoDataValue::Create((index * 10));
      property_value = AddNewProperty(property_values, L"i32_val");
      property_value->SetValue(data_value);
      FDO_SAFE_RELEASE(data_value);
      FDO_SAFE_RELEASE(property_value);

      flt_value      = (FdoFloat) (2.4 * (index /6.99));
      data_value     = FdoDataValue::Create(flt_value);
      property_value = AddNewProperty(property_values, L"sgl_val");
      property_value->SetValue(data_value);
      FDO_SAFE_RELEASE(data_value);
      FDO_SAFE_RELEASE(property_value);

      id_str         = FdoStringP::Format(L"  %d  ", index);
      data_value     = FdoDataValue::Create(id_str);
      property_value = AddNewProperty(property_values, L"str_val");
      property_value->SetValue(data_value);
      FDO_SAFE_RELEASE(data_value);
      FDO_SAFE_RELEASE(property_value);

      id_str         = FdoStringP::Format(L"The Color is: %d", (index + 2109));
      data_value     = FdoDataValue::Create((FdoString *)id_str);
      property_value = AddNewProperty(property_values, L"str2_val");
      property_value->SetValue(data_value);
      FDO_SAFE_RELEASE(data_value);
      FDO_SAFE_RELEASE(property_value);

      // Execute the command.

      feature_reader = insert_command->Execute();

      // Clean up.

      FDO_SAFE_RELEASE(feature_reader);
      FDO_SAFE_RELEASE(property_values);
      FDO_SAFE_RELEASE(insert_command);

      return;
    
    }  //  try ...

   catch (FdoException *exp) {

      FDO_SAFE_RELEASE(feature_reader);
      FDO_SAFE_RELEASE(property_value);
      FDO_SAFE_RELEASE(geometry_value);
      FDO_SAFE_RELEASE(line_str);
      FDO_SAFE_RELEASE(byte_array);
      FDO_SAFE_RELEASE(geometry_value);
      FDO_SAFE_RELEASE(data_value);
      FDO_SAFE_RELEASE(property_values);
      FDO_SAFE_RELEASE(insert_command);
      throw exp;

    }  //  catch (FdoException *exp) ...

    catch ( ... ) {

      FDO_SAFE_RELEASE(feature_reader);
      FDO_SAFE_RELEASE(property_value);
      FDO_SAFE_RELEASE(geometry_value);
      FDO_SAFE_RELEASE(line_str);
      FDO_SAFE_RELEASE(byte_array);
      FDO_SAFE_RELEASE(geometry_value);
      FDO_SAFE_RELEASE(data_value);
      FDO_SAFE_RELEASE(property_values);
      FDO_SAFE_RELEASE(insert_command);
      throw FdoException::Create(L"Failed to add a feature");

    }  //  catch ...

}  //  AddFeature ()

void FdoExpressionFunctionTest::AddTestSchema (
                                        FdoIConnection *current_connection,
                                        FdoString      *schema_name)

// +---------------------------------------------------------------------------
// | The function establishes the test schema in the FDO data store the pro-
// | vided connection identifies.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoClass                           *schema_class         = NULL;

    FdoFeatureClass                    *schema_feature_class = NULL;

	FdoIApplySchema                    *apply_schema_cmd     = NULL;

    FdoClassCollection                 *classes              = NULL;

    FdoPtr<FdoFeatureSchemaCollection> schemas;

	FdoFeatureSchema                   *schema               = NULL;

    try {

      // Create a apply-schema command.

      apply_schema_cmd = 
        (FdoIApplySchema*) current_connection->CreateCommand(
                                                   FdoCommandType_ApplySchema);

      // Create the feature schema collection.

      schemas = FdoFeatureSchemaCollection::Create(NULL);

      // Create the lock test schema.

	  schema  = FdoFeatureSchema::Create(schema_name,
                                         L"Expression Function Test Schema");
      classes = schema->GetClasses();

      // Create the test classes for the unit test and add them to the schema.

      printf(" >>> ...... adding class exfct_c1 \n");
      schema_feature_class = CreateFdoFeatureClass(L"exfct_c1");
      classes->Add(schema_feature_class);
      FDO_SAFE_RELEASE(schema_feature_class);

      // Add the test schema to the schema collection.

      schemas->Add(schema);

      // Set the active schema and create it.

      printf(" >>> ...... creating the schema \n");
      apply_schema_cmd->SetFeatureSchema(schema);
      apply_schema_cmd->Execute();

      // Release the memory.

      FDO_SAFE_RELEASE(classes);
      FDO_SAFE_RELEASE(schema);
      FDO_SAFE_RELEASE(apply_schema_cmd);

    }  //  try ...

    catch ( ... ) {

      FDO_SAFE_RELEASE(classes);
      FDO_SAFE_RELEASE(schema);
      FDO_SAFE_RELEASE(apply_schema_cmd);

      throw;

    }  //  catch ...

}  //  AddTestSchema ()

void FdoExpressionFunctionTest::CloseConnection ()

// +---------------------------------------------------------------------------
// | The function closes all open connections to the data store used in the
// | executed unit tests.
// +---------------------------------------------------------------------------

{

    if (m_connection != NULL)
        m_connection->Close();

}  //  CloseConnection ()

void FdoExpressionFunctionTest::Connect ()

// +---------------------------------------------------------------------------
// | The function establishes the connection to the data store used in the
// | executed test suites.
// +---------------------------------------------------------------------------

{

      // Establish the connection.

      m_connection = UnitTestUtil::GetConnection(DATASTORE_EXTENSION);

}  //  Connect ()

FdoDataPropertyDefinition *FdoExpressionFunctionTest::CreateDataProperty (
                                                  FdoString     *property_name,
                                                  FdoDataType   data_type,
                                                  FdoInt32      data_size,
                                                  FdoInt32      scale,
                                                  bool          is_nullable)

// +---------------------------------------------------------------------------
// | The function creates a data property object based on the provided data and
// | returns it back to the calling routine.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoDataPropertyDefinition *data_property_definition = NULL;

    // Create the property object and initialize it using the given data.

	data_property_definition =
                            FdoDataPropertyDefinition::Create(property_name,
                                                              property_name);
	data_property_definition->SetDataType(data_type);

    if (data_type == FdoDataType_String)
        data_property_definition->SetLength(data_size);

    if (data_type == FdoDataType_Decimal) {

        data_property_definition->SetPrecision(data_size);
        data_property_definition->SetScale(scale);

    }  //  if (data_type == FdoDataType_Decimal) ...

	data_property_definition->SetNullable(is_nullable);

    // Return the handle on the created object.

    return data_property_definition;

}  //  CreateDataProperty ()

FdoFeatureClass *FdoExpressionFunctionTest::CreateFdoFeatureClass (
                                                    FdoString *class_name)

// +---------------------------------------------------------------------------
// | The function creates a predefined feature class with the caller identify-
// | ing class name.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoFeatureClass                     *the_class                     = NULL;

    FdoDataPropertyDefinition           *data_property_definition      = NULL;

    FdoGeometricPropertyDefinition      *geometric_property_definition = NULL;

    FdoPropertyDefinitionCollection     *data_property_definitions     = NULL;

    FdoDataPropertyDefinitionCollection *id_property_definitions       = NULL;

    try {

      // Create the class and set the abstract property.

	  the_class = FdoFeatureClass::Create(class_name, class_name);
	  the_class->SetIsAbstract(false);

      // Define each of the class properties and add it to the proper collection.
      // If the property identifies the primary key add it to the class' primary
      // key collection.

      data_property_definitions = the_class->GetProperties();
      id_property_definitions   = the_class->GetIdentityProperties();

      data_property_definition =
                CreateDataProperty(L"featid", FdoDataType_Int64, 0, 0, false);
      data_property_definition->SetIsAutoGenerated(true);
      data_property_definitions->Add(data_property_definition);
      id_property_definitions->Add(data_property_definition);
      FDO_SAFE_RELEASE(data_property_definition);

      data_property_definition =
           CreateDataProperty(L"bool_val", FdoDataType_Boolean, 0, 0, true);
	  data_property_definitions->Add(data_property_definition);
	  FDO_SAFE_RELEASE(data_property_definition);

      data_property_definition =
           CreateDataProperty(L"byte_val", FdoDataType_Byte, 0, 0, true);
	  data_property_definitions->Add(data_property_definition);
	  FDO_SAFE_RELEASE(data_property_definition);

      data_property_definition =
           CreateDataProperty(L"dt_val", FdoDataType_DateTime, 0, 0, true);
	  data_property_definitions->Add(data_property_definition);
	  FDO_SAFE_RELEASE(data_property_definition);

      data_property_definition =
           CreateDataProperty(L"dt2_val", FdoDataType_DateTime, 0, 0, true);
	  data_property_definitions->Add(data_property_definition);
	  FDO_SAFE_RELEASE(data_property_definition);

      data_property_definition =
           CreateDataProperty(L"dcl_val", FdoDataType_Decimal, 8, 2, true);
	  data_property_definitions->Add(data_property_definition);
	  FDO_SAFE_RELEASE(data_property_definition);

      data_property_definition =
           CreateDataProperty(L"dbl_val", FdoDataType_Double, 10, 5, true);
	  data_property_definitions->Add(data_property_definition);
	  FDO_SAFE_RELEASE(data_property_definition);

      data_property_definition =
           CreateDataProperty(L"i16_val", FdoDataType_Int16, 0, 0, true);
	  data_property_definitions->Add(data_property_definition);
	  FDO_SAFE_RELEASE(data_property_definition);

      data_property_definition =
           CreateDataProperty(L"i32_val", FdoDataType_Int32, 0, 0, true);
	  data_property_definitions->Add(data_property_definition);
	  FDO_SAFE_RELEASE(data_property_definition);

      data_property_definition =
           CreateDataProperty(L"sgl_val", FdoDataType_Single, 10, 3, true);
	  data_property_definitions->Add(data_property_definition);
	  FDO_SAFE_RELEASE(data_property_definition);

      data_property_definition =
           CreateDataProperty(L"str_val", FdoDataType_String, 30, 0, true);
	  data_property_definitions->Add(data_property_definition);
	  FDO_SAFE_RELEASE(data_property_definition);

      data_property_definition =
           CreateDataProperty(L"str2_val", FdoDataType_String, 30, 0, true);
	  data_property_definitions->Add(data_property_definition);
	  FDO_SAFE_RELEASE(data_property_definition);

      data_property_definition =
           CreateDataProperty(L"id", FdoDataType_Int32, 0, 0, true);
	  data_property_definitions->Add(data_property_definition);
	  FDO_SAFE_RELEASE(data_property_definition);

      geometric_property_definition = CreateGeometricProperty(L"RDBMS_GEOM");
	  data_property_definitions->Add(geometric_property_definition);
      the_class->SetGeometryProperty(geometric_property_definition);
	  FDO_SAFE_RELEASE(geometric_property_definition);

      FDO_SAFE_RELEASE(id_property_definitions);
      FDO_SAFE_RELEASE(data_property_definitions);

      the_class->SetBaseClass(NULL);

      // Return a handle to the created class.

      return the_class;

    }  //  try ...

    catch ( ... ) {

	  FDO_SAFE_RELEASE(geometric_property_definition);
	  FDO_SAFE_RELEASE(data_property_definition);
      FDO_SAFE_RELEASE(id_property_definitions);
      FDO_SAFE_RELEASE(data_property_definitions);

      throw;

    }  //  catch ...

}  //  CreateFdoFeatureClass ()

FdoGeometricPropertyDefinition
                *FdoExpressionFunctionTest::CreateGeometricProperty (
                                                    FdoString *property_name)

// +---------------------------------------------------------------------------
// | The function creates the named geometry property and returns it back to
// | the calling routine.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoGeometricPropertyDefinition *geometric_property_definition = NULL;

    // Create the property object and initialize it using the given data.

	geometric_property_definition = 
                        FdoGeometricPropertyDefinition::Create(property_name,
                                                               property_name);
    geometric_property_definition->SetGeometryTypes(
                                                FdoGeometricType_Point   | 
                                                FdoGeometricType_Curve   | 
                                                FdoGeometricType_Surface | 
                                                FdoGeometricType_Solid     );
    geometric_property_definition->SetHasElevation(true);

    // Return the handle on the created object.

    return geometric_property_definition;

}  //  CreateGeometricProperty ()

void FdoExpressionFunctionTest::CreateTestDataStore (
                                            FdoString *datastore_extension)

// +---------------------------------------------------------------------------
// | The function executes the creation of the test data store.
// +---------------------------------------------------------------------------

{

    // Check if the named data store exists. If it exists, drop it.

    if (UnitTestUtil::DatastoreExists(FdoStringP(datastore_extension))) {

        printf(" >>> ... dropping the data store with extension '%ls' \n",
               datastore_extension);
        UnitTestUtil::DropDb(FdoStringP(datastore_extension));

    }  //  if (UnitTestUtil::DatastoreExists(datastore_extension)) ...

    printf(" >>> ... creating the data store with extension '%ls' \n",
           datastore_extension);
    UnitTestUtil::CreateDB(false, false, datastore_extension);

}  //  CreateTestDataStore ()

FdoInt32 FdoExpressionFunctionTest::GetDimensionality ()

// +---------------------------------------------------------------------------
// | The function returns the dimensionality the provider supports.
// +---------------------------------------------------------------------------

{

    return (3);

}  //  GetGeometryTypes ()   

void FdoExpressionFunctionTest::DropTestDataStore (
                                            FdoString *datastore_extension)

// +---------------------------------------------------------------------------
// | The function drops the test data store.
// +---------------------------------------------------------------------------

{

    printf(" >>> ... dropping the data store with extension '%ls' \n",
           datastore_extension);
    UnitTestUtil::DropDb(FdoStringP(datastore_extension));

}  //  DropTestDataStore ()

void FdoExpressionFunctionTest::SetupUnitTestEnvironment (
                                        FdoIConnection *current_connection)

// +---------------------------------------------------------------------------
// | The function controls the establishing of the test environment in the FDO
// | data store the provided connection identifies.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    bool            transaction_started = false;

    FdoITransaction *fdo_i_transaction  = NULL;

    try {

      // Add the test schema.

      printf(" >>> ... adding test schema \n");
      AddTestSchema(current_connection, L"ExprFct");

      // Start a transaction to cover the following operations.

      if ((fdo_i_transaction = current_connection->BeginTransaction()) == NULL)
          throw FdoException::Create(
                  L"Unable to start transaction in SetupUnitTestEnvironment");

      transaction_started = true;

      // Load the features used in the unit tests.
      //   > The spatial features.

      printf(" >>> ... adding data for feature classes \n");
      printf(" >>> ...... for class exfct_c1 \n");
      for (int i = 0; i < 31; i++)
        AddFeature(current_connection, L"exfct_c1", i);

      // Commit the transaction.

      fdo_i_transaction->Commit();
      fdo_i_transaction->Release();

      transaction_started = false;

   }  //  try ...

   catch (FdoException *exp) {

     if (transaction_started) {

         fdo_i_transaction->Rollback();
         FDO_SAFE_RELEASE(fdo_i_transaction);

     }  //  if (transaction_started) ...

     printf(" >>> Exception: %ls\n", exp->GetExceptionMessage());
     throw exp;

   }  //  catch ...

   catch ( ... ) {

     if (transaction_started) {

         fdo_i_transaction->Rollback();
         FDO_SAFE_RELEASE(fdo_i_transaction);

     }  //  if (transaction_started) ...

     throw;

   }  //  catch ...

}  //  SetupUnitTestEnvironment ()

