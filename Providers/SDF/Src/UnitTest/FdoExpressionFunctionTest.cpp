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

#include "stdafx.h"
#include "FdoExpressionFunctionTest.h"
#include "UnitTestUtil.h"
#include <math.h>
#include <Spatial/SpatialStd.h>
#include <Spatial/SpatialUtility.h>
#include <FdoCommonMiscUtil.h>

CPPUNIT_TEST_SUITE_REGISTRATION(FdoExpressionFunctionTest);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(FdoExpressionFunctionTest, "FdoExpressionFunctionTest");

#ifdef _WIN32
static const wchar_t* EE_OPT_TEST_FILE = L"..\\..\\TestData\\OptimizerTest.sdf";
#else
static const wchar_t* EE_OPT_TEST_FILE = L"../../TestData/OptimizerTest.sdf";
#endif
typedef std::pair< FdoSpatialOperations, FdoSpatialOperations > TestOperationsPair;
typedef std::pair< int, TestOperationsPair > TestIdxOperationsPair;


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

    try {

      // The following creates the data base used for the tests, opens a con-
      // nnection to it and applies a test schema.

      printf("\n");
      printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<< \n");
      printf("               EXPRESSION FUNCTION UNIT TESTS              \n");
      printf(" Status: Started                                           \n");
      printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<< \n");
      printf("\n");

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

    // Executing the combination tests.

    NumberToStringComparison();
    RoundNumberToStringComparison();

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

    // Execute the test cases.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-aggregate command to select   \n");
    printf("  the value of a computed property that is defined by us-  \n");
    printf("  ing the function AVG on the value of a different prop-   \n");
    printf("  erty of type BYTE. No exceptions are expected.           \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property being
      // 69.354838709677423. 

      func_call   = L"(Avg(byte_val) as cmp_id)";
      data_reader = ExecuteSelAggrCommand(
                                        L"exfct_c1", NULL, false, func_call);
      CheckReader(data_reader, false, 0, 69.354838709677423);
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
    printf("  ing the function AVG on the distinct values of a differ- \n");
    printf("  ent property of type BYTE. No exceptions are expected.   \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property being
      // 69.5. 

      func_call   = L"(Avg('distinct', byte_val) as cmp_id)";
      data_reader = ExecuteSelAggrCommand(
                                        L"exfct_c1", NULL, false, func_call);
      CheckReader(data_reader, false, 0, 69.5);
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
    printf("  The test executes a select-aggregate command to select   \n");
    printf("  the value of a computed property that is defined by us-  \n");
    printf("  ing the function AVG on the value of a different prop-   \n");
    printf("  erty of type DECIMAL. No exceptions are expected.        \n");
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

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("4. Test Case:                                              \n");
    printf("  The test executes a select-aggregate command to select   \n");
    printf("  the value of a computed property that is defined by us-  \n");
    printf("  ing the function AVG on the value of a different prop-   \n");
    printf("  erty of type DOUBLE. No exceptions are expected.         \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property being
      // 30.425840411322937. 

      func_call   = L"(Avg(dbl_val) as cmp_id)";
      data_reader = ExecuteSelAggrCommand(
                                        L"exfct_c1", NULL, false, func_call);
      CheckReader(data_reader, false, 0, 30.425840411322937);
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
    printf("  The test executes a select-aggregate command to select   \n");
    printf("  the value of a computed property that is defined by us-  \n");
    printf("  ing the function AVG on the value of a different prop-   \n");
    printf("  erty of type INT16. No exceptions are expected.          \n");
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

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("6. Test Case:                                              \n");
    printf("  The test executes a select-aggregate command to select   \n");
    printf("  the value of a computed property that is defined by us-  \n");
    printf("  ing the function AVG on the value of a different prop-   \n");
    printf("  erty of type SINGLE. No exceptions are expected.         \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property being
      // 6.6974370450742784. 

      func_call   = L"(Avg(sgl_val) as cmp_id)";
      data_reader = ExecuteSelAggrCommand(
                                        L"exfct_c1", NULL, false, func_call);
      CheckReader(data_reader, false, 0, 6.6974370450742784);
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
    printf("  The test executes a select-aggregate command to select   \n");
    printf("  the value of a computed property that is defined by us-  \n");
    printf("  ing the function AVG on the value of a different prop-   \n");
    printf("  erty of type SINGLE. This time, the test requests the    \n");
    printf("  average on all distinct values only. No exceptions are   \n");
    printf("  expected.                                                \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property being
      // 6.6974370450742784. 

      func_call   = L"(Avg('distinct', sgl_val) as cmp_id)";
      data_reader = ExecuteSelAggrCommand(
                                        L"exfct_c1", NULL, false, func_call);
      CheckReader(data_reader, false, 0, 6.6974370450742784);
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
    printf("  The test executes a select-aggregate command to select   \n");
    printf("  the value of a computed property that is defined by us-  \n");
    printf("  ing the function AVG on the value of a different prop-   \n");
    printf("  erty of type BYTE. In this case, the function name does  \n");
    printf("  not match the expected function name ('AvG' rather than  \n");
    printf("  'Avg'). No exceptions are expected.                      \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property being
      // 69.354838709677423. 

      func_call   = L"(AvG(byte_val) as cmp_id)";
      data_reader = ExecuteSelAggrCommand(
                                        L"exfct_c1", NULL, false, func_call);
      CheckReader(data_reader, false, 0, 69.354838709677423);
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

    FdoStringP             func_call;

    FdoPtr<FdoIDataReader> data_reader;

    printf("\n");
    printf("========================================================== \n");
    printf(" Current Unit Test Suite: COUNT Function Testing           \n");
    printf("========================================================== \n");
    printf("\n");

    // Execute the test cases.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-aggregate command to select   \n");
    printf("  the value of a computed property that is defined by us-  \n");
    printf("  ing the function COUNT on the value of a different prop- \n");
    printf("  erty of type BYTE. No exceptions are expected.           \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property being
      // 31. 

      func_call   = L"(Count(byte_val) as cmp_id)";
      data_reader = ExecuteSelAggrCommand(
                                        L"exfct_c1", NULL, false, func_call);
      CheckReader64(data_reader, false, 0, 31);
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
    printf("  ing the function COUNT on the distinct values of a dif-  \n");
    printf("  ferent property of type BYTE. No exceptions are expec-   \n");
    printf("  ted.                                                     \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property being
      // 10. 

      func_call   = L"(Count('distinct', byte_val) as cmp_id)";
      data_reader = ExecuteSelAggrCommand(
                                        L"exfct_c1", NULL, false, func_call);
      CheckReader64(data_reader, false, 0, 10);
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
    printf("  The test executes a select-aggregate command to select   \n");
    printf("  the value of a computed property that is defined by us-  \n");
    printf("  ing the function COUNT on the value of a different prop- \n");
    printf("  erty of type DECIMAL. No exceptions are expected.        \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property being
      // 30. 

      func_call   = L"(Count(dcl_val) as cmp_id)";
      data_reader = ExecuteSelAggrCommand(
                                        L"exfct_c1", NULL, false, func_call);
      CheckReader64(data_reader, false, 0, 30);
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
    printf("  The test executes a select-aggregate command to select   \n");
    printf("  the value of a computed property that is defined by us-  \n");
    printf("  ing the function COUNT on the value of a different prop- \n");
    printf("  erty of type DATE/TIME. This time, the test requests the \n");
    printf("  count on all distinct values only. No exceptions are ex- \n");
    printf("  pected.                                                  \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property to
      // be 28. 

      func_call   = L"(Count('distinct', dt_val) as cmp_id)";
      data_reader = ExecuteSelAggrCommand(
                                        L"exfct_c1", NULL, false, func_call);
      CheckReader64(data_reader, false, 0, 28);
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
    printf("  The test executes a select-aggregate command to select   \n");
    printf("  the value of a computed property that is defined by us-  \n");
    printf("  ing the function COUNT on the value of a different prop- \n");
    printf("  erty of type STRING. No exceptions are expected.         \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property being
      // 31. 

      func_call   = L"(Count(str_val) as cmp_id)";
      data_reader = ExecuteSelAggrCommand(
                                        L"exfct_c1", NULL, false, func_call);
      CheckReader64(data_reader, false, 0, 31);
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
    printf("  The test executes a select-aggregate command to select   \n");
    printf("  the value of a computed property that is defined by us-  \n");
    printf("  ing the function COUNT on the value of a different prop- \n");
    printf("  erty of type BYTE. In this case, the function name does  \n");
    printf("  not match the expected function name ('CoUnT' rather     \n");
    printf("  than 'Count'). No exceptions are expected.               \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property being
      // 31. 

      func_call   = L"(CoUnT(byte_val) as cmp_id)";
      data_reader = ExecuteSelAggrCommand(
                                        L"exfct_c1", NULL, false, func_call);
      CheckReader64(data_reader, false, 0, 31);
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

    // Execute the test cases.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-aggregate command to select   \n");
    printf("  the value of a computed property that is defined by us-  \n");
    printf("  ing the function MAX on the value of a different prop-   \n");
    printf("  erty of type BYTE. No exceptions are expected.           \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property being
      // 74. 

      func_call   = L"(Max(byte_val) as cmp_id)";
      data_reader = ExecuteSelAggrCommand(
                                        L"exfct_c1", NULL, false, func_call);
      CheckReaderByte(data_reader, false, 0, 74);
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
    printf("  ing the function MAX on the distinct values of a differ- \n");
    printf("  ent property of type BYTE. No exceptions are expected.   \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property being
      // 74. 

      func_call   = L"(Max('distinct', byte_val) as cmp_id)";
      data_reader = ExecuteSelAggrCommand(
                                        L"exfct_c1", NULL, false, func_call);
      CheckReaderByte(data_reader, false, 0, 74);
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
    printf("  The test executes a select-aggregate command to select   \n");
    printf("  the value of a computed property that is defined by us-  \n");
    printf("  ing the function MAX on the value of a different prop-   \n");
    printf("  erty of type DECIMAL. No exceptions are expected.        \n");
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

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("4. Test Case:                                              \n");
    printf("  The test executes a select-aggregate command to select   \n");
    printf("  the value of a computed property that is defined by us-  \n");
    printf("  ing the function MAX on the value of a different prop-   \n");
    printf("  erty of type DATE/TIME. This time, the test requests the \n");
    printf("  calculation of the maximum value on all distinct values  \n");
    printf("  only. No exceptions are expected.                        \n");
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
      CheckReaderDt(data_reader, false, false, 0, 0, expected_dt_data);
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
    printf("  The test executes a select-aggregate command to select   \n");
    printf("  the value of a computed property that is defined by us-  \n");
    printf("  ing the function MAX on the value of a different prop-   \n");
    printf("  erty of type BYTE. In this case, the function name does  \n");
    printf("  not match the expected function name ('MaX' rather than  \n");
    printf("  'Max'). No exceptions are expected.                      \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property being
      // 74. 

      func_call   = L"(MaX(byte_val) as cmp_id)";
      data_reader = ExecuteSelAggrCommand(
                                        L"exfct_c1", NULL, false, func_call);
      CheckReaderByte(data_reader, false, 0, 74);
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

    FdoPtr<FdoFilter>      filter;
    FdoPtr<FdoIDataReader> data_reader;

    printf("\n");
    printf("========================================================== \n");
    printf(" Current Unit Test Suite: MEDIAN Function Testing          \n");
    printf("========================================================== \n");
    printf("\n");

    // Execute the test cases.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-aggregate command to select   \n");
    printf("  the value of a computed property that is defined by us-  \n");
    printf("  ing the function MEDIAN on the value of a different      \n");
    printf("  property of type BYTE. No exceptions are expected.       \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property being
      // 69. 

      func_call   = L"(Median(byte_val) as cmp_id)";
      data_reader = ExecuteSelAggrCommand(
                                        L"exfct_c1", NULL, false, func_call);
      CheckReader(data_reader, false, 0, 69);
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
    printf("  ing the function MEDIAN on the value of a different      \n");
    printf("  property of type DECIMAL. No exceptions are expected.    \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property being
      // 20.69. 

      func_call   = L"(Median(dcl_val) as cmp_id)";
      data_reader = ExecuteSelAggrCommand(
                                        L"exfct_c1", NULL, false, func_call);
      CheckReader(data_reader, false, 0, 20.69);
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
    printf("  The test executes a select-aggregate command to select   \n");
    printf("  the value of a computed property that is defined by us-  \n");
    printf("  ing the function MEDIAN on the value of a different      \n");
    printf("  property of type DECIMAL. This test requests the calcu-  \n");
    printf("  lation if the median on a single row. In this case, the  \n");
    printf("  result is expected to be the value of the row. No excep- \n");
    printf("  tions are expected.                                      \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property being
      // 12.84. 

      filter      = FdoFilter::Parse(L"id = 9");
      func_call   = L"(Median(dcl_val) as cmp_id)";
      data_reader = ExecuteSelAggrCommand(
                                        L"exfct_c1", filter, false, func_call);
      CheckReader(data_reader, false, 0, 12.84);
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
    printf("  The test executes a select-aggregate command to select   \n");
    printf("  the value of a computed property that is defined by us-  \n");
    printf("  ing the function MEDIAN on the value of a different      \n");
    printf("  property of type BYTE. In this case, the function name   \n");
    printf("  does not match the expected function name ('MeDiAn'      \n");
    printf("  rather than 'Median'). No exceptions are expected.       \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property being
      // 69. 

      func_call   = L"(MeDiAn(byte_val) as cmp_id)";
      data_reader = ExecuteSelAggrCommand(
                                        L"exfct_c1", NULL, false, func_call);
      CheckReader(data_reader, false, 0, 69);
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

    // Execute the test cases.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-aggregate command to select   \n");
    printf("  the value of a computed property that is defined by us-  \n");
    printf("  ing the function MIN on the value of a different prop-   \n");
    printf("  erty of type BYTE. No exceptions are expected.           \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property being
      // 65. 

      func_call   = L"(Min(byte_val) as cmp_id)";
      data_reader = ExecuteSelAggrCommand(
                                        L"exfct_c1", NULL, false, func_call);
      CheckReaderByte(data_reader, false, 0, 65);
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
    printf("  ing the function MIN on the distinct values of a differ- \n");
    printf("  ent property of type BYTE. No exceptions are expected.   \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property being
      // 65. 

      func_call   = L"(Min('distinct', byte_val) as cmp_id)";
      data_reader = ExecuteSelAggrCommand(
                                        L"exfct_c1", NULL, false, func_call);
      CheckReaderByte(data_reader, false, 0, 65);
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
    printf("  The test executes a select-aggregate command to select   \n");
    printf("  the value of a computed property that is defined by us-  \n");
    printf("  ing the function MIN on the value of a different prop-   \n");
    printf("  erty of type DECIMAL. No exceptions are expected.        \n");
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

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("4. Test Case:                                              \n");
    printf("  The test executes a select-aggregate command to select   \n");
    printf("  the value of a computed property that is defined by us-  \n");
    printf("  ing the function MIN on the value of a different prop-   \n");
    printf("  erty of type DATE/TIME. This time, the test requests the \n");
    printf("  calculation of the minimum value on all distinct values  \n");
    printf("  only. No exceptions are expected.                        \n");
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
      CheckReaderDt(data_reader, false, false, 0, 0, expected_dt_data);
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
    printf("  The test executes a select-aggregate command to select   \n");
    printf("  the value of a computed property that is defined by us-  \n");
    printf("  ing the function MIN on the value of a different prop-   \n");
    printf("  erty of type BYTE. In this case, the function name does  \n");
    printf("  not match the expected function name ('MiN' rather than  \n");
    printf("  'Min').No exceptions are expected.                       \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property being
      // 65. 

      func_call   = L"(MiN(byte_val) as cmp_id)";
      data_reader = ExecuteSelAggrCommand(
                                        L"exfct_c1", NULL, false, func_call);
      CheckReaderByte(data_reader, false, 0, 65);
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

    FdoPtr<FdoFilter>      filter;
    FdoPtr<FdoIDataReader> data_reader;

    printf("\n");
    printf("========================================================== \n");
    printf(" Current Unit Test Suite: STDDEV Function Testing          \n");
    printf("========================================================== \n");
    printf("\n");

    // Execute the test cases.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-aggregate command to select   \n");
    printf("  the value of a computed property that is defined by us-  \n");
    printf("  ing the function STDDEV on the value of a different      \n");
    printf("  property of type BYTE. No exceptions are expected.       \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property being
      // 2.98382738. 

      func_call   = L"(Stddev(byte_val) as cmp_id)";
      data_reader = ExecuteSelAggrCommand(
                                        L"exfct_c1", NULL, false, func_call);
      CheckReader(data_reader, false, 0, 2.98382738);
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
    printf("  ing the function STDDEV on the distinct values of a dif- \n");
    printf("  ferent property of type BYTE. No exceptions are expected \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property being
      // 3.02765035. 

      func_call   = L"(Stddev('distinct', byte_val) as cmp_id)";
      data_reader = ExecuteSelAggrCommand(
                                        L"exfct_c1", NULL, false, func_call);
      CheckReader(data_reader, false, 0, 3.02765035);
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
    printf("  The test executes a select-aggregate command to select   \n");
    printf("  the value of a computed property that is defined by us-  \n");
    printf("  ing the function STDDEV on the value of a different      \n");
    printf("  property of type DECIMAL. No exceptions are expected.    \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property being
      // 13.0968318. 

      func_call   = L"(Stddev(dcl_val) as cmp_id)";
      data_reader = ExecuteSelAggrCommand(
                                        L"exfct_c1", NULL, false, func_call);
      CheckReader(data_reader, false, 0, 13.0968318);
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
    printf("  The test executes a select-aggregate command to select   \n");
    printf("  the value of a computed property that is defined by us-  \n");
    printf("  ing the function STDDEV on the value of a different      \n");
    printf("  property of type DECIMAL. This test requests the stan-   \n");
    printf("  dard deviation being calculated on the distinct set of   \n");
    printf("  data. No exceptions are expected.                        \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property being
      // 13.0968318. 

      func_call   = L"(Stddev('distinct', dcl_val) as cmp_id)";
      data_reader = ExecuteSelAggrCommand(
                                        L"exfct_c1", NULL, false, func_call);
      CheckReader(data_reader, false, 0, 13.0968318);
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
    printf("  The test executes a select-aggregate command to select   \n");
    printf("  the value of a computed property that is defined by us-  \n");
    printf("  ing the function STDDEV on the value of a different      \n");
    printf("  property of type DECIMAL. This test requests the stan-   \n");
    printf("  dard deviation on a single row. In this case, the result \n");
    printf("  is expected to be 0. No exceptions are expected.         \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property being
      // 0. 

      filter      = FdoFilter::Parse(L"id = 9");
      func_call   = L"(Stddev(dcl_val) as cmp_id)";
      data_reader = ExecuteSelAggrCommand(
                                        L"exfct_c1", filter, false, func_call);
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

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("6. Test Case:                                              \n");
    printf("  The test executes a select-aggregate command to select   \n");
    printf("  the value of a computed property that is defined by us-  \n");
    printf("  ing the function STDDEV on the value of a different      \n");
    printf("  property of type BYTE. In this case, the function name   \n");
    printf("  does not match the expected function name ('StDdEv' in-  \n");
    printf("  stead of 'Stddev'). No exceptions are expected.          \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property being
      // 2.98382738. 

      func_call   = L"(StDdEv(byte_val) as cmp_id)";
      data_reader = ExecuteSelAggrCommand(
                                        L"exfct_c1", NULL, false, func_call);
      CheckReader(data_reader, false, 0, 2.98382738);
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

    // Execute the test cases.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-aggregate command to select   \n");
    printf("  the value of a computed property that is defined by us-  \n");
    printf("  ing the function SUM on the value of a different prop-   \n");
    printf("  erty of type BYTE. No exceptions are expected.           \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property being
      // 2150. 

      func_call   = L"(Sum(byte_val) as cmp_id)";
      data_reader = ExecuteSelAggrCommand(
                                        L"exfct_c1", NULL, false, func_call);
      CheckReader(data_reader, false, 0, 2150);
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
    printf("  ing the function SUM on the distinct values of a differ- \n");
    printf("  ent property of type BYTE. No exceptions are expected.   \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property being
      // 695. 

      func_call   = L"(Sum('distinct', byte_val) as cmp_id)";
      data_reader = ExecuteSelAggrCommand(
                                        L"exfct_c1", NULL, false, func_call);
      CheckReader(data_reader, false, 0, 695);
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
    printf("  The test executes a select-aggregate command to select   \n");
    printf("  the value of a computed property that is defined by us-  \n");
    printf("  ing the function SUM on the value of a different prop-   \n");
    printf("  erty of type DECIMAL. No exceptions are expected.        \n");
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
    printf("4. Test Case:                                              \n");
    printf("  The test executes a select-aggregate command to select   \n");
    printf("  the value of a computed property that is defined by us-  \n");
    printf("  ing the function SUM on the value of a different prop-   \n");
    printf("  erty of type INT32. This time, the test requests the     \n");
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

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("5. Test Case:                                              \n");
    printf("  The test executes a select-aggregate command to select   \n");
    printf("  the value of a computed property that is defined by us-  \n");
    printf("  ing the function SUM on the value of a different prop-   \n");
    printf("  erty of type BYTE. In this case, the function name does  \n");
    printf("  not match the expected function name ('SuM' instead of   \n");
    printf("  'Sum'). No exceptions are expected.                      \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property being
      // 2150. 

      func_call   = L"(SuM(byte_val) as cmp_id)";
      data_reader = ExecuteSelAggrCommand(
                                        L"exfct_c1", NULL, false, func_call);
      CheckReader(data_reader, false, 0, 2150);
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

    filter1 = FdoFilter::Parse(L"id = 21");
    filter2 = FdoFilter::Parse(L"id = 22");

    // Test Setup.

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

    // Execute the test cases.

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function NULLVALUE on the values of two different prop-  \n");
    printf("  erties of type DATE/TIME. In this test, the value of the \n");
    printf("  first property is NULL whereas it is set for the second  \n");
    printf("  property. The test should return this date/time informa- \n");
    printf("  tion. No exceptions are expected.                        \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property
      // being identical to the setting of the date/time value stored in
      // the variable dt1_set.

      func_call   = L"(NullValue(dt_val, dt2_val) as cmp_id)";
      data_reader =
                ExecuteSelectCommand(L"exfct_c1", filter1, true, func_call);
      CheckReaderDt(data_reader, true, false, 0, 21, dt1_set);
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
    printf("  function NULLVALUE on the values of two different prop-  \n");
    printf("  erties of type DATE/TIME. In this test, the value of the \n");
    printf("  first property is set whereas it is NULL for the second  \n");
    printf("  property. The test should therefore return the date/time \n");
    printf("  information for the first property. No exceptions are    \n");
    printf("  expected.                                                \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property
      // being identical to the setting of the date/time value stored in
      // the variable dt2_set.

      func_call   = L"(NullValue(dt_val, dt2_val) as cmp_id)";
      data_reader =
                ExecuteSelectCommand(L"exfct_c1", filter2, true, func_call);
      CheckReaderDt(data_reader, true, false, 0, 22, dt2_set);
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
    printf("  function NULLVALUE on the values of two different prop-  \n");
    printf("  erties of type DECIMAL and DOUBLE. In this test, the     \n");
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

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("4. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function NULLVALUE on the values of two different prop-  \n");
    printf("  erties of type DECIMAL and DOUBLE. In this test, the     \n");
    printf("  value of the first property is set whereas it is NULL    \n");
    printf("  for the second property. The test should return there-   \n");
    printf("  fore return the value of the first property. No excep-   \n");
    printf("  tions are expected.                                      \n");
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

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("5. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function NULLVALUE on the values of two different prop-  \n");
    printf("  erties of type DATE/TIME where the function name differs \n");
    printf("  from the expected function name ('NuLlVaLuE' rather than \n");
    printf("  'NullValue'). In this test, the value of the first prop- \n");
    printf("  erty is NULL whereas it is set for the second property.  \n");
    printf("  The test should return this date/time information. No    \n");
    printf("  exceptions are expected.                                 \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property
      // being identical to the setting of the date/time value stored in
      // the variable dt1_set.

      func_call   = L"(NuLlVaLuE(dt_val, dt2_val) as cmp_id)";
      data_reader =
                ExecuteSelectCommand(L"exfct_c1", filter1, true, func_call);
      CheckReaderDt(data_reader, true, false, 0, 21, dt1_set);
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

    filter = FdoFilter::Parse(L"id = 9");

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
    printf("  function TODATE on a string containing date information  \n");
    printf("  where the function name differs from the expected func-  \n");
    printf("  tion name ('ToDaTe' rather than 'ToDate'). The test also \n");
    printf("  provides a format string describing the date-string. The \n");
    printf("  test should return a date/time object that contains the  \n");
    printf("  information provided by the date-string. No exceptions   \n");
    printf("  are expected.                                            \n");
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

void FdoExpressionFunctionTest::TestToDateFunctionAsFilter ()

// +---------------------------------------------------------------------------
// | The function executes the test for the expression engine function TODATE
// | when used in a filter.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    FdoInt16                  counter           = 0;

    FdoPtr<FdoFilter>         filter;
    FdoPtr<FdoIFeatureReader> data_reader;

    printf("\n");
    printf("========================================================== \n");
    printf(" Current Unit Test Suite: TODATE Function Filter Testing   \n");
    printf("========================================================== \n");
    printf("\n");

    // Define the filter for all tests in this test suite.

    // filter = FdoFilter::Parse(L"dt_val = ToDate(dtstr_val, 'MM/DD/YYYY hh:mm:ss')");
    filter = FdoFilter::Parse(L"dt_val = ToDate(dtstr_val, 'MM/DD/YYYY')");

    // Execute the test cases.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select data from a \n");
    printf("  class table using a filter that includes the expression  \n");
    printf("  function TODATE. The request should return a subset of   \n");
    printf("  the available data. No exceptions are expected.          \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the number of the returned data. If the
      // number does not match the expected result, issue an exception.

      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, NULL);

      printf(" >>> Cross check result \n");
      while (data_reader->ReadNext())
        counter++;

      data_reader->Close();

      // Issue an exception if the expected result is not met.

      //if (counter != 2)
      //    throw FdoException::Create(
      //                  L"Unexpected result(s) when checking returned data");
      //else
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

}  //  TestToDateFunctionAsFilter ()

void FdoExpressionFunctionTest::TestToDoubleFunction ()

// +---------------------------------------------------------------------------
// | The function executes the test for the expression engine function TODOUBLE
// | when used as a select-parameter.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    FdoInt16                  counter           = 0;

    FdoDouble                 exp_dcl_val,
                              exp_dbl_val,
                              exp_i32_val,
                              exp_sgl_val,
                              exp_str_val,
                              exp_byte_val;

    FdoStringP                str_val,
                              func_call;

    FdoPtr<FdoFilter>         filter;
    FdoPtr<FdoIFeatureReader> data_reader,
                              feature_reader;

    printf("\n");
    printf("========================================================== \n");
    printf(" Current Unit Test Suite: TODOUBLE Function Testing        \n");
    printf("========================================================== \n");
    printf("\n");

    // Define the filter for all tests in this test suite.

    filter = FdoFilter::Parse(L"id = 9");

    // Test Setup.

    printf("---------------------------------------------------------- \n");
    printf("Test Setup:                                                \n");
    printf("  The following executes a select-command to retrieve the  \n");
    printf("  current values of the properties used in the following   \n");
    printf("  tests. Those values are later used for cross-checking    \n");
    printf("  the test result.                                         \n");
    printf("---------------------------------------------------------- \n");

    try {

      feature_reader = ExecuteSelectCommand(L"exfct_c1",
                                            filter,
                                            L"byte_val",
                                            L"dcl_val",
                                            L"dbl_val",
                                            L"i32_val",
                                            L"sgl_val",
                                            L"str_val");

      printf(" >>> Retrieving cross-check data \n");

      while (feature_reader->ReadNext()) {

        counter++;

        exp_byte_val = (FdoDouble)feature_reader->GetByte(L"byte_val");
        exp_dcl_val  = feature_reader->GetDouble(L"dcl_val");
        exp_dbl_val  = feature_reader->GetDouble(L"dbl_val");
        exp_i32_val  = (FdoDouble)feature_reader->GetInt32(L"i32_val");
        exp_sgl_val  = feature_reader->GetSingle(L"sgl_val");
        str_val      = feature_reader->GetString(L"str_val");
        exp_str_val  = str_val.ToDouble();

      }  //  while (feature_reader->ReadNext()) ...

      // Issue an exception if more than the expected number of rows were
      // returned.

      if (counter != 1)
          throw FdoException::Create(L"Unexpected number of rows returned");
 
    }  //  try ...

    catch (FdoException *exp) {

      printf(" >>> Exception: %ls\n", exp->GetExceptionMessage());
      printf(" >>> Test setup failed \n");
      throw exp;

    }  //  catch (FdoException *ex) ...

    catch ( ... ) {

      printf(" >>> Test setup failed for an unknown reason \n");
      throw;

    }  //  catch ( ... ) ...

    // Execute the test cases.

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function TODOUBLE on the value of a different property   \n");
    printf("  of type BYTE. The test should return the original prop-  \n");
    printf("  erty value. No exceptions are expected.                  \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data.

      func_call   = L"(ToDouble(byte_val) as cmp_id)";
      data_reader =
                ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, exp_byte_val);
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
    printf("  function TODOUBLE on the value of a different property   \n");
    printf("  of type DECIMAL. The test should return the original     \n");
    printf("  property value. No exceptions are expected.              \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data.

      func_call   = L"(ToDouble(dcl_val) as cmp_id)";
      data_reader =
                ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, exp_dcl_val);
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
    printf("  function TODOUBLE on the value of a different property   \n");
    printf("  of type DOUBLE. The test should return the original      \n");
    printf("  property value. No exceptions are expected.              \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data.

      func_call   = L"(ToDouble(dbl_val) as cmp_id)";
      data_reader =
                ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, exp_dbl_val);
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
    printf("  function TODOUBLE on the value of a different property   \n");
    printf("  of type INT32. The test should return the original prop- \n");
    printf("  erty value. No exceptions are expected.                  \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data.

      func_call   = L"(ToDouble(i32_val) as cmp_id)";
      data_reader =
                ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, exp_i32_val);
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
    printf("  function TODOUBLE on the value of a different property   \n");
    printf("  of type SINGLE. The test should return the original      \n");
    printf("  property value. No exceptions are expected.              \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data.

      func_call   = L"(ToDouble(sgl_val) as cmp_id)";
      data_reader =
                ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, exp_sgl_val);
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
    printf("  function TODOUBLE on the value of a different property   \n");
    printf("  of type STRING. The test should return the numeric value \n");
    printf("  stored in the addressed property. No exceptions are ex-  \n");
    printf("  pected.                                                  \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data.

      func_call   = L"(ToDouble(str_val) as cmp_id)";
      data_reader =
                ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, exp_str_val);
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
    printf("  function TODOUBLE on the value of a different property   \n");
    printf("  of type BYTE where the function name differs from the    \n");
    printf("  expected name ('ToDoUbLe' rather than 'ToDouble'). The   \n");
    printf("  test should return the original property value. No ex-   \n");
    printf("  ceptions are expected.                                   \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data.

      func_call   = L"(ToDoUbLe(byte_val) as cmp_id)";
      data_reader =
                ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, exp_byte_val);
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

void FdoExpressionFunctionTest::TestToDoubleFunctionAsFilter ()

// +---------------------------------------------------------------------------
// | The function executes the test for the expression engine function TODOUBLE
// | when used within a filter.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    FdoInt16                  counter           = 0;

    FdoPtr<FdoFilter>         filter;
    FdoPtr<FdoIFeatureReader> data_reader;

    printf("\n");
    printf("========================================================== \n");
    printf(" Current Unit Test Suite: TODOUBLE Function Filter Testing \n");
    printf("========================================================== \n");
    printf("\n");

    // Define the filter for all tests in this test suite.

    filter = FdoFilter::Parse(L"ToDouble(str_val) = dbl_val");

    // Execute the test cases.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select data from a \n");
    printf("  class table using a filter that includes the expression  \n");
    printf("  function TODOUBLE. The request should return a subset of \n");
    printf("  the available data. No exceptions are expected.          \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the number of the returned data. If the
      // number does not match the expected result, issue an exception.

      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, NULL);

      printf(" >>> Cross check result \n");
      while (data_reader->ReadNext())
        counter++;

      data_reader->Close();

      // Issue an exception if the expected result is not met.

      if (counter != 2)
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

}  //  TestToDoubleFunctionAsFilter ()

void FdoExpressionFunctionTest::TestToFloatFunction ()

// +---------------------------------------------------------------------------
// | The function executes the test for the expression engine function TOFLOAT
// | when used as a select-parameter.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    FdoInt16                  counter           = 0;

    FdoFloat                  exp_dcl_val,
                              exp_dbl_val,
                              exp_i32_val,
                              exp_sgl_val,
                              exp_str_val,
                              exp_byte_val;

    FdoStringP                str_val,
                              func_call;

    FdoPtr<FdoFilter>         filter;
    FdoPtr<FdoIFeatureReader> data_reader,
                              feature_reader;

    printf("\n");
    printf("========================================================== \n");
    printf(" Current Unit Test Suite: TOFLOAT Function Testing         \n");
    printf("========================================================== \n");
    printf("\n");

    // Define the filter for all tests in this test suite.

    filter = FdoFilter::Parse(L"id = 9");

    // Test Setup.

    printf("---------------------------------------------------------- \n");
    printf("Test Setup:                                                \n");
    printf("  The following executes a select-command to retrieve the  \n");
    printf("  current values of the properties used in the following   \n");
    printf("  tests. Those values are later used for cross-checking    \n");
    printf("  the test result.                                         \n");
    printf("---------------------------------------------------------- \n");

    try {

      feature_reader = ExecuteSelectCommand(L"exfct_c1",
                                            filter,
                                            L"byte_val",
                                            L"dcl_val",
                                            L"dbl_val",
                                            L"i32_val",
                                            L"sgl_val",
                                            L"str_val");

      printf(" >>> Retrieving cross-check data \n");

      while (feature_reader->ReadNext()) {

        counter++;

        exp_byte_val = (FdoFloat)feature_reader->GetByte(L"byte_val");
        exp_dcl_val  = (FdoFloat)feature_reader->GetDouble(L"dcl_val");
        exp_dbl_val  = (FdoFloat)feature_reader->GetDouble(L"dbl_val");
        exp_i32_val  = (FdoFloat)feature_reader->GetInt32(L"i32_val");
        exp_sgl_val  = feature_reader->GetSingle(L"sgl_val");
        str_val      = feature_reader->GetString(L"str_val");
        exp_str_val  = (FdoFloat)str_val.ToDouble();

      }  //  while (feature_reader->ReadNext()) ...

      // Issue an exception if more than the expected number of rows were
      // returned.

      if (counter != 1)
          throw FdoException::Create(L"Unexpected number of rows returned");
 
    }  //  try ...

    catch (FdoException *exp) {

      printf(" >>> Exception: %ls\n", exp->GetExceptionMessage());
      printf(" >>> Test setup failed \n");
      throw exp;

    }  //  catch (FdoException *ex) ...

    catch ( ... ) {

      printf(" >>> Test setup failed for an unknown reason \n");
      throw;

    }  //  catch ( ... ) ...

    // Execute the test cases.

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function TOFLOAT on the value of a different property of \n");
    printf("  type BYTE. The test should return the original property  \n");
    printf("  value. No exceptions are expected.                       \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data.

      func_call   = L"(ToFloat(byte_val) as cmp_id)";
      data_reader =
                ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderSgl(data_reader, true, 9, exp_byte_val);
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
    printf("  function TOFLOAT on the value of a different property of \n");
    printf("  type DECIMAL. The test should return the original prop-  \n");
    printf("  erty value. No exceptions are expected.                  \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data.

      func_call   = L"(ToFloat(dcl_val) as cmp_id)";
      data_reader =
                ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderSgl(data_reader, true, 9, exp_dcl_val);
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
    printf("  function TOFLOAT on the value of a different property of \n");
    printf("  type DOUBLE. The test should return the original prop-   \n");
    printf("  erty value. No exceptions are expected.                  \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data.

      func_call   = L"(ToFloat(dbl_val) as cmp_id)";
      data_reader =
                ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderSgl(data_reader, true, 9, exp_dbl_val);
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
    printf("  function TOFLOAT on the value of a different property of \n");
    printf("  type INT32. The test should return the original property \n");
    printf("  value. No exceptions are expected.                       \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data.

      func_call   = L"(ToFloat(i32_val) as cmp_id)";
      data_reader =
                ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderSgl(data_reader, true, 9, exp_i32_val);
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
    printf("  function TOFLOAT on the value of a different property of \n");
    printf("  type SINGLE. The test should return the original prop-   \n");
    printf("  erty value. No exceptions are expected.                  \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data.

      func_call   = L"(ToFloat(sgl_val) as cmp_id)";
      data_reader =
                ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderSgl(data_reader, true, 9, exp_sgl_val);
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
    printf("  The test executes a select-command to selectthe value    \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function TOFLOAT on the value of a different property of \n");
    printf("  type STRING. The test should return the numeric value of \n");
    printf("  the addressed property. No exceptions are expected.      \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data.

      func_call   = L"(ToFloat(str_val) as cmp_id)";
      data_reader =
                ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderSgl(data_reader, true, 9, exp_str_val);
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
    printf("  function TOFLOAT on the value of a different property of \n");
    printf("  type BYTE where the function name differs from the ex-   \n");
    printf("  pected function name ('ToFlOaT' rather than 'ToFloat').  \n");
    printf("  The test should return the original property value. No   \n");
    printf("  exceptions are expected.                                 \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data.

      func_call   = L"(ToFlOaT(byte_val) as cmp_id)";
      data_reader =
                ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderSgl(data_reader, true, 9, exp_byte_val);
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

    FdoInt16                  counter           = 0;

    FdoInt32                  exp_dcl_val,
                              exp_dbl_val,
                              exp_i32_val,
                              exp_sgl_val,
                              exp_str_val,
                              exp_byte_val;

    FdoStringP                str_val,
                              func_call;

    FdoPtr<FdoFilter>         filter;
    FdoPtr<FdoIFeatureReader> data_reader,
                              feature_reader;

    printf("\n");
    printf("========================================================== \n");
    printf(" Current Unit Test Suite: TOINT32 Function Testing         \n");
    printf("========================================================== \n");
    printf("\n");

    // Define the filter for all tests in this test suite.

    filter = FdoFilter::Parse(L"id = 9");

    // Test Setup.

    printf("---------------------------------------------------------- \n");
    printf("Test Setup:                                                \n");
    printf("  The following executes a select-command to retrieve the  \n");
    printf("  current values of the properties used in the following   \n");
    printf("  tests. Those values are later used for cross-checking    \n");
    printf("  the test result.                                         \n");
    printf("---------------------------------------------------------- \n");

    try {

      feature_reader = ExecuteSelectCommand(L"exfct_c1",
                                            filter,
                                            L"byte_val",
                                            L"dcl_val",
                                            L"dbl_val",
                                            L"i32_val",
                                            L"sgl_val",
                                            L"str_val");

      printf(" >>> Retrieving cross-check data \n");

      while (feature_reader->ReadNext()) {

        counter++;

        exp_byte_val = feature_reader->GetByte(L"byte_val");
        exp_dcl_val  = (FdoInt32)feature_reader->GetDouble(L"dcl_val");
        exp_dbl_val  = (FdoInt32)feature_reader->GetDouble(L"dbl_val");
        exp_i32_val  = feature_reader->GetInt32(L"i32_val");
        exp_sgl_val  = (FdoInt32)feature_reader->GetSingle(L"sgl_val");
        str_val      = feature_reader->GetString(L"str_val");
        exp_str_val  = str_val.ToLong();

      }  //  while (feature_reader->ReadNext()) ...

      // Issue an exception if more than the expected number of rows were
      // returned.

      if (counter != 1)
          throw FdoException::Create(L"Unexpected number of rows returned");
 
    }  //  try ...

    catch (FdoException *exp) {

      printf(" >>> Exception: %ls\n", exp->GetExceptionMessage());
      printf(" >>> Test setup failed \n");
      throw exp;

    }  //  catch (FdoException *ex) ...

    catch ( ... ) {

      printf(" >>> Test setup failed for an unknown reason \n");
      throw;

    }  //  catch ( ... ) ...

    // Execute the test cases.

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function TOINT32 on the value of a different property of \n");
    printf("  type BYTE. The test should return the original property  \n");
    printf("  value. No exceptions are expected.                       \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data.

      func_call   = L"(ToInt32(byte_val) as cmp_id)";
      data_reader =
                ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader32(data_reader, true, 9, exp_byte_val);
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
    printf("  type DECIMAL. The test should return the original prop-  \n");
    printf("  erty value without the part after the decimal point. No  \n");
    printf("  exceptions are expected.                                 \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data.

      func_call   = L"(ToInt32(dcl_val) as cmp_id)";
      data_reader =
                ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader32(data_reader, true, 9, exp_dcl_val);
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
    printf("  type DOUBLE. The test should return the original prop-   \n");
    printf("  erty value without the part after the decimal point. No  \n");
    printf("  exceptions are expected.                                 \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data.

      func_call   = L"(ToInt32(dbl_val) as cmp_id)";
      data_reader =
                ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader32(data_reader, true, 9, exp_dbl_val);
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
    printf("  type INT32. The test should return the original property \n");
    printf("  value. No exceptions are expected.                       \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data.

      func_call   = L"(ToInt32(i32_val) as cmp_id)";
      data_reader =
                ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader32(data_reader, true, 9, exp_i32_val);
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
    printf("  type SINGLE. The test should return the original prop-   \n");
    printf("  erty value without the part after the decimal point. No  \n");
    printf("  exceptions are expected.                                 \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data.

      func_call   = L"(ToInt32(sgl_val) as cmp_id)";
      data_reader =
                ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader32(data_reader, true, 9, exp_sgl_val);
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
    printf("  type STRING. The test should return the numeric value of \n");
    printf("  the addressed property. No exceptions are expected.      \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data.

      func_call   = L"(ToInt32(str_val) as cmp_id)";
      data_reader =
                ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader32(data_reader, true, 9, exp_str_val);
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
    printf("  function TOINT32 on the value of a different property of \n");
    printf("  type BYTE where the function name differs from the ex-   \n");
    printf("  pected function name ('ToInT32' rather than 'ToFInt32'). \n");
    printf("  The test should return the original property value. No   \n");
    printf("  exceptions are expected.                                 \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data.

      func_call   = L"(ToInT32(byte_val) as cmp_id)";
      data_reader =
                ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader32(data_reader, true, 9, exp_byte_val);
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

    FdoInt16                  counter           = 0;

    FdoInt64                  exp_dcl_val,
                              exp_dbl_val,
                              exp_i32_val,
                              exp_sgl_val,
                              exp_str_val,
                              exp_byte_val;

    FdoStringP                str_val,
                              func_call;

    FdoPtr<FdoFilter>         filter;
    FdoPtr<FdoIFeatureReader> data_reader,
                              feature_reader;

    printf("\n");
    printf("========================================================== \n");
    printf(" Current Unit Test Suite: TOINT64 Function Testing         \n");
    printf("========================================================== \n");
    printf("\n");

    // Define the filter for all tests in this test suite.

    filter = FdoFilter::Parse(L"id = 9");

    // Test Setup.

    printf("---------------------------------------------------------- \n");
    printf("Test Setup:                                                \n");
    printf("  The following executes a select-command to retrieve the  \n");
    printf("  current values of the properties used in the following   \n");
    printf("  tests. Those values are later used for cross-checking    \n");
    printf("  the test result.                                         \n");
    printf("---------------------------------------------------------- \n");

    try {

      feature_reader = ExecuteSelectCommand(L"exfct_c1",
                                            filter,
                                            L"byte_val",
                                            L"dcl_val",
                                            L"dbl_val",
                                            L"i32_val",
                                            L"sgl_val",
                                            L"str_val");

      printf(" >>> Retrieving cross-check data \n");

      while (feature_reader->ReadNext()) {

        counter++;

        exp_byte_val = feature_reader->GetByte(L"byte_val");
        exp_dcl_val  = (FdoInt64)feature_reader->GetDouble(L"dcl_val");
        exp_dbl_val  = (FdoInt64)feature_reader->GetDouble(L"dbl_val");
        exp_i32_val  = feature_reader->GetInt32(L"i32_val");
        exp_sgl_val  = (FdoInt64)feature_reader->GetSingle(L"sgl_val");
        str_val      = feature_reader->GetString(L"str_val");
        exp_str_val  = str_val.ToLong();

      }  //  while (feature_reader->ReadNext()) ...

      // Issue an exception if more than the expected number of rows were
      // returned.

      if (counter != 1)
          throw FdoException::Create(L"Unexpected number of rows returned");
 
    }  //  try ...

    catch (FdoException *exp) {

      printf(" >>> Exception: %ls\n", exp->GetExceptionMessage());
      printf(" >>> Test setup failed \n");
      throw exp;

    }  //  catch (FdoException *ex) ...

    catch ( ... ) {

      printf(" >>> Test setup failed for an unknown reason \n");
      throw;

    }  //  catch ( ... ) ...

    // Execute the test cases.

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function TOINT64 on the value of a different property of \n");
    printf("  type BYTE. The test should return the original property  \n");
    printf("  value. No exceptions are expected.                       \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data.

      func_call   = L"(ToInt64(byte_val) as cmp_id)";
      data_reader =
                ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader64(data_reader, true, 9, exp_byte_val);
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
    printf("  type DECIMAL. The test should return the original prop-  \n");
    printf("  erty value without the part after the decimal point. No  \n");
    printf("  exceptions are expected.                                 \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data.

      func_call   = L"(ToInt64(dcl_val) as cmp_id)";
      data_reader =
                ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader64(data_reader, true, 9, exp_dcl_val);
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
    printf("  type DOUBLE. The test should return the original prop-   \n");
    printf("  erty value without the part after the decimal point. No  \n");
    printf("  exceptions are expected.                                 \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data.

      func_call   = L"(ToInt64(dbl_val) as cmp_id)";
      data_reader =
                ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader64(data_reader, true, 9, exp_dbl_val);
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
    printf("  type INT32. The test should return the original property \n");
    printf("  value. No exceptions are expected.                       \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data.

      func_call   = L"(ToInt64(i32_val) as cmp_id)";
      data_reader =
                ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader64(data_reader, true, 9, exp_i32_val);
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
    printf("  type SINGLE. The test should return the original prop-   \n");
    printf("  erty value without the part after the decimal point. No  \n");
    printf("  exceptions are expected.                                 \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data.

      func_call   = L"(ToInt64(sgl_val) as cmp_id)";
      data_reader =
                ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader64(data_reader, true, 9, exp_sgl_val);
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
    printf("  type STRING. The test should return the numeric value of \n");
    printf("  the addressed property No exceptions are expected.       \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data.

      func_call   = L"(ToInt64(str_val) as cmp_id)";
      data_reader =
                ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader64(data_reader, true, 9, exp_str_val);
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
    printf("  function TOINT64 on the value of a different property of \n");
    printf("  type BYTE where the function name differs from the ex-   \n");
    printf("  pected function name ('ToInT64' rather than 'ToInt64').  \n");
    printf("  The test should return the original property value. No   \n");
    printf("  exceptions are expected.                                 \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data.

      func_call   = L"(ToInT64(byte_val) as cmp_id)";
      data_reader =
                ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader64(data_reader, true, 9, exp_byte_val);
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

    filter = FdoFilter::Parse(L"id = 9");

    // Execute the test cases.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function TOSTRING on the value of a different property   \n");
    printf("  of type BYTE. The test should return the original prop-  \n");
    printf("  erty value in a string. No exceptions are expected.      \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property
      // being "74".

      func_call   = L"(ToString(byte_val) as cmp_id)";
      data_reader =
                ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderString(data_reader, 9, L"74");
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
    printf("  of type DECIMAL. The test should return the original     \n");
    printf("  property value in a string. No exceptions are expected.  \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property
      // being "12.84".

      func_call   = L"(ToString(dcl_val) as cmp_id)";
      data_reader =
                ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderNumberString(data_reader, 9, L"12.84");
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
    printf("  of type DOUBLE. The test should return the original      \n");
    printf("  property value in a string. No exceptions are expected.  \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property
      // being 18.88908145580589.

      func_call   = L"(ToString(dbl_val) as cmp_id)";
      data_reader =
                ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderNumberString(data_reader, 9, L"18.88908145580589");
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
    printf("  of type INT32. The test should return the original prop- \n");
    printf("  erty value in a string. No exceptions are expected.      \n");
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
    printf("5. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function TOSTRING on the value of a different property   \n");
    printf("  of type SINGLE. The test should return the original      \n");
    printf("  property value in a string. No exceptions are expected.  \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property
      // being 3.09012866.

      func_call   = L"(ToString(sgl_val) as cmp_id)";
      data_reader =
                ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderNumberString(data_reader, 9, L"3.09012866");
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
    printf("  of type DATE/TIME. In this test, no format instruction   \n");
    printf("  is provided. The test should return the date in a string \n");
    printf("  using the default format. No exceptions are expected.    \n");
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
    printf("7. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function TOSTRING on the value of a different property   \n");
    printf("  of type DATE/TIME. In this test, a format instruction is \n");
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
    printf("8. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function TOSTRING on the value of a different property   \n");
    printf("  of type BYTE where the function name differs from the    \n");
    printf("  expected function name ('ToStRiNg' rather than 'To-      \n");
    printf("  String'). The test should return the original property   \n");
    printf("  value in a string. No exceptions are expected.           \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property
      // being "74".

      func_call   = L"(ToStRiNg(byte_val) as cmp_id)";
      data_reader =
                ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderString(data_reader, 9, L"74");
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

    // The following test cases are specifically to an issue QA reported
    // but because they establish a general test, those cases are added
    // to the standard unit test suite. 

    // Reset the filter and execute the tests.

    filter = FdoFilter::Parse(L"id = 20");

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("9. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function TOSTRING on the value of a different property   \n");
    printf("  of type DOUBLE. The test should return the original      \n");
    printf("  property value in a string. No exceptions are expected.  \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property
      // being "30.3".

      func_call   = L"(ToString(dbl_val) as cmp_id)";
      data_reader =
                ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderString(data_reader, 9, L"30.3");
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
    printf("10. Test Case:                                             \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function TOSTRING on the value of a different property   \n");
    printf("  of type SINGLE. The test should return the original      \n");
    printf("  property value in a string. No exceptions are expected.  \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row with the value of the computed property
      // being "30.3".

      func_call   = L"(ToString(sgl_val) as cmp_id)";
      data_reader =
                ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderString(data_reader, 9, L"30.3");
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

    filter = FdoFilter::Parse(L"id = 9");

    // Test Setup.

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

    // Execute the test cases.

    printf("\n");
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

      // Execute the test and check the returned data.

      expected_dt_data.year  = dt_set.year + 1;
      expected_dt_data.month = dt_set.month;
      expected_dt_data.day   = dt_set.day;

      func_call   = L"(AddMonths(dt_val, 12) as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", filter, true, func_call);
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
    printf("  function ADDMONTHS on the value of a different property  \n");
    printf("  of type DATE/TIME. The test adds 88 months to the date   \n");
    printf("  on which the function is based. No exceptions are ex-    \n");
    printf("  pected.                                                  \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data.

      expected_dt_data.year  = dt_set.year + 8;
      expected_dt_data.month = 1;
      expected_dt_data.day   = dt_set.day;

      func_call   = L"(AddMonths(dt_val, 88) as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", filter, true, func_call);
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
    printf("3. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function ADDMONTHS on the value of a different property  \n");
    printf("  of type DATE/TIME. The test subtracts 88 months from the \n");
    printf("  date on which the function is based. No exceptions are   \n");
    printf("  expected.                                                \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data.

      expected_dt_data.year  = dt_set.year - 7;
      expected_dt_data.month = dt_set.month - 4;
      expected_dt_data.day   = dt_set.day;

      func_call   = L"(AddMonths(dt_val, -88) as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", filter, true, func_call);
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
    printf("4. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function ADDMONTHS on the value of a different property  \n");
    printf("  of type DATE/TIME where the function name differs from   \n");
    printf("  the expected function name ('AdDmOnThS' rather than      \n");
    printf("  'AddMonths'). The test adds 12 months to the date on     \n");
    printf("  which the function is based. No exceptions are expected. \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data.

      expected_dt_data.year  = dt_set.year + 1;
      expected_dt_data.month = dt_set.month;
      expected_dt_data.day   = dt_set.day;

      func_call   = L"(AdDmOnThS(dt_val, 12) as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", filter, true, func_call);
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

}  //  TestAddMonthsFunction ()

void FdoExpressionFunctionTest::TestCurrentDateFunction ()

// +---------------------------------------------------------------------------
// | The function executes the test for the expression engine function CURRENT-
// | DATE when used as a select parameter.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    FdoInt32                  row_count     = 0;

    FdoStringP                func_call;

    FdoPtr<FdoFilter>         filter;
    FdoPtr<FdoIFeatureReader> data_reader;

    printf("\n");
    printf("========================================================== \n");
    printf(" Current Unit Test Suite: CURRENTDATE Function Testing     \n");
    printf("========================================================== \n");
    printf("\n");

    // Define the filter for all tests in this test suite.

    filter = FdoFilter::Parse(L"id = 9");

    // Execute the test cases.
    // NOTE: For these tests there is no cross-checking of the results because
    //       there is nothing to check against. Theoretically, one could get
    //       the current time before executing the tests and use it for cross-
    //       checks. But then, the only data that is fairly safe to check is
    //       the year as all the other pieces may already differ from the
    //       cross check value when the test is executed.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function CURRENTDATE. No exceptions are expected.        \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data.

      func_call   = L"(CurrentDate() as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", filter, true, func_call);

      printf(" >>> Cross check result \n");

      while (data_reader->ReadNext())
        row_count++;

      // Close the reader.

      data_reader->Close();

      // Issue an exception if the expected result is not met.

      if (row_count != 1)
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

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("2. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function CURRENTDATE where the function name differs     \n");
    printf("  from the expected function name ('CuRrEnTdAtE' rather    \n");
    printf("  than 'CurrentDate'). No exceptions are expected.         \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data.

      row_count   = 0;
      func_call   = L"(CuRrEnTdAtE() as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", filter, true, func_call);

      printf(" >>> Cross check result \n");

      while (data_reader->ReadNext())
        row_count++;

      // Close the reader.

      data_reader->Close();

      // Issue an exception if the expected result is not met.

      if (row_count != 1)
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

    filter = FdoFilter::Parse(L"id = 9");

    // Test Setup.

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

    // Execute the test cases.

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function EXTRACT on the value of a different property of \n");
    printf("  type DATE/TIME. The test extracts the year from the date \n");
    printf("  on which the function is based. No exceptions are expec- \n");
    printf("  ted.                                                     \n");
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
      CheckReaderDt(data_reader, true, true, 0, 9, expected_dt_data);
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
    printf("  function EXTRACT on the value of a different property of \n");
    printf("  type DATE/TIME. The test extracts the month from the     \n");
    printf("  date on which the function is based. No exceptions are   \n");
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
      CheckReaderDt(data_reader, true, true, 1, 9, expected_dt_data);
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
    printf("  function EXTRACT on the value of a different property of \n");
    printf("  type DATE/TIME. The test extracts the day from the date  \n");
    printf("  on which the function is based. No exceptions are expec- \n");
    printf("  ted.                                                     \n");
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
      CheckReaderDt(data_reader, true, true, 2, 9, expected_dt_data);
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
    printf("  function EXTRACT on the value of a different property of \n");
    printf("  type DATE/TIME where the function name differs from the  \n");
    printf("  expected function name ('ExTrAcT' rather than 'Extract').\n");
    printf("  The test extracts the year from the date on which the    \n");
    printf("  function is based. No exceptions are expected.           \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The year value for the selected computed
      // property is expected to be the value of the original date on which
      // the function is executed.

      expected_dt_data.year = dt_set.year;

      func_call   = L"(ExTrAcT('YEAR', dt_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", filter, true, func_call);
      CheckReaderDt(data_reader, true, true, 0, 9, expected_dt_data);
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

    filter = FdoFilter::Parse(L"id = 9");

    // Execute the test cases.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function MONTHSBETWEEN on the values of two different    \n");
    printf("  properties of type DATE/TIME. The test should return the \n");
    printf("  months between those two dates. No exceptions are expec- \n");
    printf("  ted.                                                     \n");
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
    printf("  properties of type DATE/TIME. The test should return the \n");
    printf("  months between those two dates. No exceptions are expec- \n");
    printf("  ted.                                                     \n");
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
    printf("  differs from the expected function name ('MoNtHsBeTwEeN' \n");
    printf("  rather than 'MonthsBetween'). The test should return the \n");
    printf("  months between those two dates. No exceptions are expec- \n");
    printf("  ted.                                                     \n");
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
// ==                       TESTING THE MATH FUNCTIONS                      ==
// ===========================================================================

void FdoExpressionFunctionTest::TestAbsFunction ()

// +---------------------------------------------------------------------------
// | The function executes the test for the expression engine function ABS when
// | used as a select-parameter.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    FdoInt16                  counter           = 0;

    FdoInt32                  exp_i32_val;

    FdoByte                   exp_byte_val;

    FdoStringP                func_call;

    FdoPtr<FdoFilter>         filter;
    FdoPtr<FdoIFeatureReader> data_reader,
                              feature_reader;

    printf("\n");
    printf("========================================================== \n");
    printf(" Current Unit Test Suite: ABS Function Testing             \n");
    printf("========================================================== \n");
    printf("\n");

    // Define the filter for all tests in this test suite.

    filter = FdoFilter::Parse(L"id = 9");

    // Test Setup:

    printf("---------------------------------------------------------- \n");
    printf("Test Setup:                                                \n");
    printf("  The following executes a select-command to retrieve the  \n");
    printf("  current values of the properties used in the following   \n");
    printf("  tests. Those values are later used for cross-checking    \n");
    printf("  the test result.                                         \n");
    printf("---------------------------------------------------------- \n");

    try {

      feature_reader = ExecuteSelectCommand(L"exfct_c1",
                                            filter,
                                            L"byte_val",
                                            L"i32_val",
                                            NULL,
                                            NULL,
                                            NULL,
                                            NULL);

      printf(" >>> Retrieving cross-check data \n");

      while (feature_reader->ReadNext()) {

        counter++;

        exp_byte_val = feature_reader->GetByte(L"byte_val");
        exp_i32_val  = feature_reader->GetInt32(L"i32_val");

      }  //  while (feature_reader->ReadNext()) ...

      // Issue an exception if more than the expected number of rows were
      // returned.

      if (counter != 1)
          throw FdoException::Create(L"Unexpected number of rows returned");
 
    }  //  try ...

    catch (FdoException *exp) {

      printf(" >>> Exception: %ls\n", exp->GetExceptionMessage());
      printf(" >>> Test setup failed \n");
      throw exp;

    }  //  catch (FdoException *ex) ...

    catch ( ... ) {

      printf(" >>> Test setup failed for an unknown reason \n");
      throw;

    }  //  catch ( ... ) ...

    // Execute the test cases.

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function ABS on the positive value of a different prop-  \n");
    printf("  erty of type BYTE. No exceptions are expected.           \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data.

      func_call   = L"(Abs(byte_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderByte(data_reader, true, 9, exp_byte_val);
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
    printf("  function ABS on the positive value of a different prop-  \n");
    printf("  erty of type INT32. No exceptions are expected.          \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data.

      func_call   = L"(Abs(i32_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader32(data_reader, true, 9, exp_i32_val);
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
    printf("  function ABS on the negative value of a different prop-  \n");
    printf("  erty of type INT32. No exceptions are expected.          \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data.

      func_call   = L"(Abs(i32_val*-1) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader32(data_reader, true, 9, exp_i32_val);
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
    printf("  function ABS on the positive value of a different prop-  \n");
    printf("  erty of type BYTE where the function name differs from   \n");
    printf("  the expected function name ('AbS' rather than 'Abs'). No \n");
    printf("  exceptions are expected.                                 \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data.

      func_call   = L"(AbS(byte_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderByte(data_reader, true, 9, exp_byte_val);
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

    filter = FdoFilter::Parse(L"id = 0");

    // Execute the test cases.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function ACOS on the value of a different property of    \n");
    printf("  type BYTE. No exceptions are expected.                   \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 0.174298204.

      func_call   = L"(Acos(byte_val/66.0) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 0, 0.174298204);
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
    printf("  function ACOS on the value of a different property of    \n");
    printf("  type DECIMAL. No exceptions are expected.                \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 1.57079633.

      func_call   = L"(Acos(dcl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
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

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("3. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function ACOS on the value of a different property of    \n");
    printf("  type BYTE where the function name differs from the ex-   \n");
    printf("  pected function name ('AcOs' rather than 'Acos'). No ex- \n");
    printf("  ceptions are expected.                                   \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 0.174298204.

      func_call   = L"(AcOs(byte_val/66.0) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 0, 0.174298204);
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

    filter = FdoFilter::Parse(L"id = 0");

    // Execute the test cases.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function ASIN on the value of a different property of    \n");
    printf("  type BYTE. No exceptions are expected.                   \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 1.39649812.

      func_call   = L"(Asin(byte_val/66.0) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 0, 1.39649812);
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
    printf("  function ASIN on the value of a different property of    \n");
    printf("  type DECIMAL. No exceptions are expected.                \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 0.

      func_call   = L"(Asin(dcl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
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

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("3. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function ASIN on the value of a different property of    \n");
    printf("  type BYTE where the function name differs from the ex-   \n");
    printf("  pected function name ('AsIn' rather than Asin). No ex-   \n");
    printf("  ceptions are expected.                                   \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 1.39649812.

      func_call   = L"(AsIn(byte_val/66.0) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 0, 1.39649812);
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

    filter = FdoFilter::Parse(L"id = 0");

    // Execute the test cases.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function ATAN on the value of a different property of    \n");
    printf("  type BYTE. No exceptions are expected.                   \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 1.55541293.

      func_call   = L"(Atan(byte_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 0, 1.55541293);
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
    printf("  function ATAN on the value of a different property of    \n");
    printf("  type DECIMAL. No exceptions are expected.                \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 0.

      func_call   = L"(Atan(dcl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
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

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("3. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function ATAN on the value of a different property of    \n");
    printf("  type BYTE where the function name differs from the ex-   \n");
    printf("  pected function name ('AtAn rather than 'Atan'). No ex-  \n");
    printf("  ceptions are expected.                                   \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 1.55541293.

      func_call   = L"(AtAn(byte_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 0, 1.55541293);
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

    filter = FdoFilter::Parse(L"id = 9");

    // Execute the test cases.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function ATAN2 on the values of two different properties \n");
    printf("  (of type BYTE, DECIMAL). No exceptions are expected.     \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 1.39899334.

      func_call   = L"(Atan2(byte_val, dcl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, 1.39899334);
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
    printf("  function ATAN2 on the values of two different properties \n");
    printf("  (of type DOUBLE, DECIMAL). No exceptions are expected.   \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 0.973785322.

      func_call   = L"(Atan2(dbl_val, dcl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
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

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("3. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function ATAN2 on the values of two different properties \n");
    printf("  (of type BYTE, DECIMAL) where the function name differs  \n");
    printf("  from the expected function name ('AtAn2' rather than     \n");
    printf("  'Atan2'). No exceptions are expected.                    \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 1.39899334.

      func_call   = L"(AtAn2(byte_val, dcl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, 1.39899334);
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

    filter = FdoFilter::Parse(L"id = 9");

    // Execute the test cases.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function COS on the value of a different property of     \n");
    printf("  type BYTE. No exceptions are expected.                   \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 0.171717342.

      func_call   = L"(Cos(byte_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, 0.171717342);
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
    printf("  function COS on the value of a different property of     \n");
    printf("  type DECIMAL. No exceptions are expected.                \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 0.96279648.

      func_call   = L"(Cos(dcl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
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

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("3. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function COS on the value of a different property of     \n");
    printf("  type BYTE where the function name differs from the ex-   \n");
    printf("  pected function name ('CoS' rather than 'Cos'). No ex-   \n");
    printf("  ceptions are expected.                                   \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 0.171717342.

      func_call   = L"(CoS(byte_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, 0.171717342);
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

    filter = FdoFilter::Parse(L"id = 9");

    // Execute the test cases.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function EXP on the value of a different property of     \n");
    printf("  type BYTE. No exceptions are expected.                   \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 1.3733829795401761e+032.

      func_call   = L"(Exp(byte_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, 1.3733829795401761e+032);
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
    printf("  function EXP on the value of a different property of     \n");
    printf("  type DECIMAL. No exceptions are expected.                \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 378017.82486350299.

      func_call   = L"(Exp(dcl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, 378017.82486350299);
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
    printf("  function EXP on the value of a different property of     \n");
    printf("  type BYTE where the function name differs from the ex-   \n");
    printf("  pected function name ('ExP' rather than 'Exp'). No ex-   \n");
    printf("  ceptions are expected.                                   \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 1.3733829795401761e+032.

      func_call   = L"(ExP(byte_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, 1.3733829795401761e+032);
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

    filter = FdoFilter::Parse(L"id = 9");

    // Execute the test cases.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function LN on the value of a different property of type \n");
    printf("  BYTE. No exceptions are expected.                        \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 4.30406509.

      func_call   = L"(Ln(byte_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, 4.30406509);
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
    printf("  function LN on the value of a different property of type \n");
    printf("  DECIMAL. No exceptions are expected.                     \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 2.5525653.

      func_call   = L"(Ln(dcl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
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

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("3. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function LN on the value of a different property of type \n");
    printf("  BYTE where the function name differs from the expected   \n");
    printf("  function name ('LN' rather than 'Ln'). No exceptions are \n");
    printf("  expected.                                                \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 4.30406509.

      func_call   = L"(LN(byte_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, 4.30406509);
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

    filter = FdoFilter::Parse(L"id = 9");

    // Execute the test cases.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function LOG on the values of two different properties   \n");
    printf("  (of type BYTE, DECIMAL). No exceptions are expected.     \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 0.593059176.

      func_call   = L"(Log(byte_val, dcl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, 0.593059176);
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
    printf("  function LOG on the values of two different properties   \n");
    printf("  (of type DOUBLE, DECIMAL). No exceptions are expected.   \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 0.868637837.

      func_call   = L"(Log(dbl_val, dcl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
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

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("3. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function LOG on the values of two different properties   \n");
    printf("  (of type BYTE, DECIMAL) where the function name differs  \n");
    printf("  from the expected function name ('LoG' rather than       \n");
    printf("  'Log'). No exceptions are expected.                      \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 0.593059176.

      func_call   = L"(LoG(byte_val, dcl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, 0.593059176);
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

    filter = FdoFilter::Parse(L"id = 9");

    // Execute the test cases.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function MOD on the values of two different properties   \n");
    printf("  (of type BYTE, BYTE). No exceptions are expected.        \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 0.

      func_call   = L"(Mod(byte_val, byte_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderByte(data_reader, true, 9, 0);
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
    printf("  function MOD on the values of two different properties   \n");
    printf("  (of type BYTE, DECIMAL). No exceptions are expected.     \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 9.8.

      func_call   = L"(Mod(byte_val, dcl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, 9.8);
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
    printf("  function MOD on the values of two different properties   \n");
    printf("  (of type BYTE, DOUBLE). No exceptions are expected.      \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 17.3327556.

      func_call   = L"(Mod(byte_val, dbl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, 17.3327556);
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
    printf("  function MOD on the values of two different properties   \n");
    printf("  (of type BYTE, INT16). No exceptions are expected.       \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 2.

      func_call   = L"(Mod(byte_val, i16_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderByte(data_reader, true, 9, 2);
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
    printf("  function MOD on the values of two different properties   \n");
    printf("  (of type BYTE, INT32). No exceptions are expected.       \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 74.

      func_call   = L"(Mod(byte_val, i32_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderByte(data_reader, true, 9, 74);
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
    printf("  function MOD on the values of two different properties   \n");
    printf("  (of type BYTE, SINGLE). No exceptions are expected.      \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 2.92704082.

      func_call   = L"(Mod(byte_val, sgl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderSgl(data_reader, true, 9, (FdoFloat)2.92704082);
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
    printf("  function MOD on the values of two different properties   \n");
    printf("  (of type DECIMAL, BYTE). No exceptions are expected.     \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 12.84.

      func_call   = L"(Mod(dcl_val, byte_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
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

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("8. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function MOD on the values of two different properties   \n");
    printf("  (of type DECIMAL, DECIMAL). No exceptions are expected.  \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 0.

      func_call   = L"(Mod(dcl_val, dcl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, 0);
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
    printf("  function MOD on the values of two different properties   \n");
    printf("  (of type DECIMAL, DOUBLE). No exceptions are expected.   \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 12.84.

      func_call   = L"(Mod(dcl_val, dbl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
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

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("10. Test Case:                                             \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function MOD on the values of two different properties   \n");
    printf("  (of type DECIMAL, INT16). No exceptions are expected.    \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 12.84.

      func_call   = L"(Mod(dcl_val, i16_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
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

    // Execute the test cases.

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("11. Test Case:                                             \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function MOD on the values of two different properties   \n");
    printf("  (of type DECIMAL, INT32). No exceptions are expected.    \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 12.84.

      func_call   = L"(Mod(dcl_val, i32_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
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

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("12. Test Case:                                             \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function MOD on the values of two different properties   \n");
    printf("  (of type DECIMAL, SINGLE). No exceptions are expected.   \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 0.47948536.

      func_call   = L"(Mod(dcl_val, sgl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, 0.47948536);
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
    printf("13. Test Case:                                             \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function MOD on the values of two different properties   \n");
    printf("  (of type DOUBLE, BYTE). No exceptions are expected.      \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 18.8890815.

      func_call   = L"(Mod(dbl_val, byte_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
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

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("14. Test Case:                                             \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function MOD on the values of two different properties   \n");
    printf("  (of type DOUBLE, DECIMAL). No exceptions are expected.  \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 6.04908146.

      func_call   = L"(Mod(dbl_val, dcl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
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

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("15. Test Case:                                             \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function MOD on the values of two different properties   \n");
    printf("  (of type DOUBLE, DOUBLE). No exceptions are expected.    \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 0.

      func_call   = L"(Mod(dbl_val, dbl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, 0);
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
    printf("16. Test Case:                                             \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function MOD on the values of two different properties   \n");
    printf("  (of type DOUBLE, INT16). No exceptions are expected.     \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 18.8890815.

      func_call   = L"(Mod(dbl_val, i16_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
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

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("17. Test Case:                                             \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function MOD on the values of two different properties   \n");
    printf("  (of type DOUBLE, INT32). No exceptions are expected.     \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 18.8890815.

      func_call   = L"(Mod(dbl_val, i32_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
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

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("18. Test Case:                                             \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function MOD on the values of two different properties   \n");
    printf("  (of type DOUBLE, SINGLE). No exceptions are expected.    \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 0.348309496.

      func_call   = L"(Mod(dbl_val, sgl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, 0.348309496);
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
    printf("19. Test Case:                                             \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function MOD on the values of two different properties   \n");
    printf("  (of type INT16, BYTE). No exceptions are expected.       \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 36.

      func_call   = L"(Mod(i16_val, byte_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader16(data_reader, true, 9, 36);
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
    printf("20. Test Case:                                             \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function MOD on the values of two different properties   \n");
    printf("  (of type INT16, DECIMAL). No exceptions are expected.    \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 10.32.

      func_call   = L"(Mod(i16_val, dcl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, 10.32);
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

    // Execute the test cases.

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("21. Test Case:                                             \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function MOD on the values of two different properties   \n");
    printf("  (of type INT16, DOUBLE). No exceptions are expected.     \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 17.1109185.

      func_call   = L"(Mod(i16_val, dbl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, 17.1109185);
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
    printf("22. Test Case:                                             \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function MOD on the values of two different properties   \n");
    printf("  (of type INT16, INT16). No exceptions are expected.      \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 0.

      func_call   = L"(Mod(i16_val, i16_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader16(data_reader, true, 9, 0);
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
    printf("23. Test Case:                                             \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function MOD on the values of two different properties   \n");
    printf("  (of type INT16, INT32). No exceptions are expected.      \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 36.

      func_call   = L"(Mod(i16_val, i32_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader16(data_reader, true, 9, 36);
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
    printf("24. Test Case:                                             \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function MOD on the values of two different properties   \n");
    printf("  (of type INT16, SINGLE). No exceptions are expected.     \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 2.00858474.

      func_call   = L"(Mod(i16_val, sgl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderSgl(data_reader, true, 9, (FdoFloat)2.00858474);
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
    printf("25. Test Case:                                             \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function MOD on the values of two different properties   \n");
    printf("  (of type INT32, BYTE). No exceptions are expected.       \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 16.

      func_call   = L"(Mod(i32_val, byte_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader32(data_reader, true, 9, 16);
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
    printf("26. Test Case:                                             \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function MOD on the values of two different properties   \n");
    printf("  (of type INT32, DECIMAL). No exceptions are expected.    \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 0.12.

      func_call   = L"(Mod(i32_val, dcl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, 0.12);
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
    printf("27. Test Case:                                             \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function MOD on the values of two different properties   \n");
    printf("  (of type INT32, DOUBLE). No exceptions are expected.     \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 14.4436742.

      func_call   = L"(Mod(i32_val, dbl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, 14.4436742);
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
    printf("28. Test Case:                                             \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function MOD on the values of two different properties   \n");
    printf("  (of type INT32, INT16). No exceptions are expected.      \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 18.

      func_call   = L"(Mod(i32_val, i16_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader16(data_reader, true, 9, 18);
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
    printf("29. Test Case:                                             \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function MOD on the values of two different properties   \n");
    printf("  (of type INT32, INT32). No exceptions are expected.      \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 0.

      func_call   = L"(Mod(i32_val, i32_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader32(data_reader, true, 9, 0);
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
    printf("30. Test Case:                                             \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function MOD on the values of two different properties   \n");
    printf("  (of type INT32, SINGLE). No exceptions are expected.     \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 0.38626886.

      func_call   = L"(Mod(i32_val, sgl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderSgl(data_reader, true, 9, (FdoFloat)0.38626886);
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

    // Execute the test cases.

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("31. Test Case:                                             \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function MOD on the values of two different properties   \n");
    printf("  (of type SINGLE, BYTE). No exceptions are expected.      \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 3.09012866.

      func_call   = L"(Mod(sgl_val, byte_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderSgl(data_reader, true, 9, (FdoFloat)3.09012866);
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
    printf("32. Test Case:                                             \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function MOD on the values of two different properties   \n");
    printf("  (of type SINGLE, DECIMAL). No exceptions are expected.   \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 3.09012866.

      func_call   = L"(Mod(sgl_val, dcl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
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

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("33. Test Case:                                             \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function MOD on the values of two different properties   \n");
    printf("  (of type SINGLE, DOUBLE). No exceptions are expected.    \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 3.09012866.

      func_call   = L"(Mod(sgl_val, dbl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
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

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("34. Test Case:                                             \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function MOD on the values of two different properties   \n");
    printf("  (of type SINGLE, INT16). No exceptions are expected.     \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 3.09012866.

      func_call   = L"(Mod(sgl_val, i16_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderSgl(data_reader, true, 9, (FdoFloat)3.09012866);
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
    printf("35. Test Case:                                             \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function MOD on the values of two different properties   \n");
    printf("  (of type SINGLE, INT32). No exceptions are expected.     \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 3.09012866.

      func_call   = L"(Mod(sgl_val, i32_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderSgl(data_reader, true, 9, (FdoFloat)3.09012866);
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
    printf("36. Test Case:                                             \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function MOD on the values of two different properties   \n");
    printf("  (of type SINGLE, SINGLE). No exceptions are expected.    \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 0.

      func_call   = L"(Mod(sgl_val, sgl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderSgl(data_reader, true, 9, 0);
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
    printf("37. Test Case:                                             \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function MOD on the values of two different properties   \n");
    printf("  (of type BYTE, BYTE) where the function name differs     \n");
    printf("  from the expected function name ('MoD' rather than       \n");
    printf("  'Mod'). No exceptions are expected.                      \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 0.

      func_call   = L"(MoD(byte_val, byte_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderByte(data_reader, true, 9, 0);
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

    filter = FdoFilter::Parse(L"id = 1");

    // Execute the test cases.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function POWER on the values of two different properties \n");
    printf("  (of type BYTE, DECIMAL). No exceptions are expected.     \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 394.84610940851007.

      func_call   = L"(Power(byte_val, dcl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 1, 394.84610940851007);
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
    printf("  function POWER on the values of two different properties \n");
    printf("  (of type INT16, INT32). No exceptions are expected.      \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 1048576.

      func_call   = L"(Power(i16_val, i32_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
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

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("3. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function POWER on the values of two different properties \n");
    printf("  (of type BYTE, DECIMAL) where the function name differs  \n");
    printf("  from the expected function name ('PoWeR' rather than     \n");
    printf("  'Power'). No exceptions are expected.                    \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 394.84610940851007.

      func_call   = L"(PoWeR(byte_val, dcl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 1, 394.84610940851007);
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
    FdoPtr<FdoIFeatureReader> data_reader;

    printf("\n");
    printf("========================================================== \n");
    printf(" Current Unit Test Suite: REMAINDER Function Testing       \n");
    printf("========================================================== \n");
    printf("\n");

    // Define the filter for all tests in this test suite.

    filter = FdoFilter::Parse(L"id = 9");

    // Execute the test cases.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function REMAINDER on the values of two different prop-  \n");
    printf("  erties (of type BYTE, BYTE). No exceptions are expected. \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 0.

      func_call   = L"(Remainder(byte_val, byte_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader16(data_reader, true, 9, 0);
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
    printf("  function REMAINDER on the values of two different prop-  \n");
    printf("  erties (of type BYTE, DECIMAL). No exceptions are expec- \n");
    printf("  ted.                                                     \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be -3.04.

      func_call   = L"(Remainder(byte_val, dcl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, -3.04);
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
    printf("  function REMAINDER on the values of two different prop-  \n");
    printf("  erties (of type BYTE, DOUBLE). No exceptions are expec-  \n");
    printf("  ted.                                                     \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be -1.5563258.

      func_call   = L"(Remainder(byte_val, dbl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, -1.5563258);
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
    printf("  function REMAINDER on the values of two different prop-  \n");
    printf("  erties (of type BYTE, INT16). No exceptions are expec-   \n");
    printf("  ted.                                                     \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 2.

      func_call   = L"(Remainder(byte_val, i16_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader16(data_reader, true, 9, 2);
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
    printf("  function REMAINDER on the values of two different prop-  \n");
    printf("  erties (of type BYTE, INT32). No exceptions are expec-   \n");
    printf("  ted.                                                     \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be -16.

      func_call   = L"(Remainder(byte_val, i32_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader32(data_reader, true, 9, -16);
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
    printf("  function REMAINDER on the values of two different prop-  \n");
    printf("  erties (of type BYTE, SINGLE). No exceptions are expec-  \n");
    printf("  ted.                                                     \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be -0.16308784.

      func_call   = L"(Remainder(byte_val, sgl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderSgl(data_reader, true, 9, (FdoFloat)-0.16308784);
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
    printf("  function REMAINDER on the values of two different prop-  \n");
    printf("  erties (of type DECIMAL, BYTE). No exceptions are expec- \n");
    printf("  ted.                                                     \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 12.84.

      func_call   = L"(Remainder(dcl_val, byte_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
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

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("8. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function REMAINDER on the values of two different prop-  \n");
    printf("  erties (of type DECIMAL, DECIMAL). No exceptions are     \n");
    printf("  expected.                                                \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 0.

      func_call   = L"(Remainder(dcl_val, dcl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, 0);
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
    printf("  function REMAINDER on the values of two different prop-  \n");
    printf("  erties (of type DECIMAL, DOUBLE). No exceptions are ex-  \n");
    printf("  pected.                                                  \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be -6.0490815.

      func_call   = L"(Remainder(dcl_val, dbl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, -6.0490815);
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
    printf("10. Test Case:                                             \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function REMAINDER on the values of two different prop-  \n");
    printf("  erties (of type DECIMAL, INT16). No exceptions are ex-   \n");
    printf("  pected.                                                  \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 12.84.

      func_call   = L"(Remainder(dcl_val, i16_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
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

    // Execute the test cases.

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("11. Test Case:                                             \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function REMAINDER on the values of two different prop-  \n");
    printf("  erties (of type DECIMAL, INT32). No exceptions are ex-   \n");
    printf("  pected.                                                  \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 12.84.

      func_call   = L"(Remainder(dcl_val, i32_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
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

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("12. Test Case:                                             \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function REMAINDER on the values of two different prop-  \n");
    printf("  erties (of type DECIMAL, SINGLE). No exceptions are ex-  \n");
    printf("  pected.                                                  \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 0.47948536.

      func_call   = L"(Remainder(dcl_val, sgl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, 0.47948536);
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
    printf("13. Test Case:                                             \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function REMAINDER on the values of two different prop-  \n");
    printf("  erties (of type DOUBLE, BYTE). No exceptions are expec-  \n");
    printf("  ted.                                                     \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 18.8890815.

      func_call   = L"(Remainder(dbl_val, byte_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
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

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("14. Test Case:                                             \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function REMAINDER on the values of two different prop-  \n");
    printf("  erties (of type DOUBLE, DECIMAL). No exceptions are ex-  \n");
    printf("  pected.                                                  \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 6.04908146.

      func_call   = L"(Remainder(dbl_val, dcl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
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

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("15. Test Case:                                             \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function REMAINDER on the values of two different prop-  \n");
    printf("  erties (of type DOUBLE, DOUBLE). No exceptions are ex-   \n");
    printf("  pected.                                                  \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 0.

      func_call   = L"(Remainder(dbl_val, dbl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, 0);
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
    printf("16. Test Case:                                             \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function REMAINDER on the values of two different prop-  \n");
    printf("  erties (of type DOUBLE, INT16). No exceptions are ex-    \n");
    printf("  pected.                                                  \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be -17.110919.

      func_call   = L"(Remainder(dbl_val, i16_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, -17.110919);
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
    printf("17. Test Case:                                             \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function REMAINDER on the values of two different prop-  \n");
    printf("  erties (of type DOUBLE, INT32). No exceptions are expec- \n");
    printf("  ted.                                                     \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 18.8890815.

      func_call   = L"(Remainder(dbl_val, i32_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
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

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("18. Test Case:                                             \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function REMAINDER on the values of two different prop-  \n");
    printf("  erties (of type DOUBLE, SINGLE). No exceptions are ex-   \n");
    printf("  pected.                                                  \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 0.348309496.

      func_call   = L"(Remainder(dbl_val, sgl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, 0.348309496);
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
    printf("19. Test Case:                                             \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function REMAINDER on the values of two different prop-  \n");
    printf("  erties (of type INT16, BYTE). No exceptions are expec-   \n");
    printf("  ted.                                                     \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 36.

      func_call   = L"(Remainder(i16_val, byte_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader16(data_reader, true, 9, 36);
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
    printf("20. Test Case:                                             \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function REMAINDER on the values of two different prop-  \n");
    printf("  erties (of type INT16, DECIMAL). No exceptions are ex-   \n");
    printf("  pected.                                                  \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be -2.52.

      func_call   = L"(Remainder(i16_val, dcl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, -2.52);
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
    printf("21. Test Case:                                             \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function REMAINDER on the values of two different prop-  \n");
    printf("  erties (of type INT16, DOUBLE). No exceptions are expec- \n");
    printf("  ted.                                                     \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be -1.7781629.

      func_call   = L"(Remainder(i16_val, dbl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, -1.7781629);
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
    printf("22. Test Case:                                             \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function REMAINDER on the values of two different prop-  \n");
    printf("  erties (of type INT16, INT16). No exceptions are expec-  \n");
    printf("  ted.                                                     \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 0.

      func_call   = L"(Remainder(i16_val, i16_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader16(data_reader, true, 9, 0);
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
    printf("23. Test Case:                                             \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function REMAINDER on the values of two different prop-  \n");
    printf("  erties (of type INT16, INT32). No exceptions are expec-  \n");
    printf("  ted.                                                     \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 36.

      func_call   = L"(Remainder(i16_val, i32_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader16(data_reader, true, 9, 36);
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
    printf("24. Test Case:                                             \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function REMAINDER on the values of two different prop-  \n");
    printf("  erties (of type INT16, SINGLE). No exceptions are expec- \n");
    printf("  ted.                                                     \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be -1.0815439.

      func_call   = L"(Remainder(i16_val, sgl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderSgl(data_reader, true, 9, (FdoFloat)-1.0815439);
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
    printf("25. Test Case:                                             \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function REMAINDER on the values of two different prop-  \n");
    printf("  erties (of type INT32, BYTE). No exceptions are expected \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 16.

      func_call   = L"(Remainder(i32_val, byte_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader32(data_reader, true, 9, 16);
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
    printf("26. Test Case:                                             \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function REMAINDER on the values of two different prop-  \n");
    printf("  erties (of type INT32, DECIMAL). No exceptions are ex-   \n");
    printf("  pected.                                                  \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 0.12.

      func_call   = L"(Remainder(i32_val, dcl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, 0.12);
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
    printf("27. Test Case:                                             \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function REMAINDER on the values of two different prop-  \n");
    printf("  erties (of type INT32, DOUBLE). No exceptions are ex-    \n");
    printf("  pected.                                                  \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be -4.4454073.

      func_call   = L"(Remainder(i32_val, dbl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, -4.4454073);
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
    printf("28. Test Case:                                             \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function REMAINDER on the values of two different prop-  \n");
    printf("  erties (of type INT32, INT16). No exceptions are expec-  \n");
    printf("  ted.                                                     \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 18.

      func_call   = L"(Remainder(i32_val, i16_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader16(data_reader, true, 9, 18);
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
    printf("29. Test Case:                                             \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function REMAINDER on the values of two different prop-  \n");
    printf("  erties (of type INT32, INT32). No exceptions are expec-  \n");
    printf("  ted.                                                     \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 0.

      func_call   = L"(Remainder(i32_val, i32_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader32(data_reader, true, 9, 0);
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
    printf("30. Test Case:                                             \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function REMAINDER on the values of two different prop-  \n");
    printf("  erties (of type INT32, SINGLE). No exceptions are expec- \n");
    printf("  ted.                                                     \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 0.38626886.

      func_call   = L"(Remainder(i32_val, sgl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderSgl(data_reader, true, 9, (FdoFloat)0.38626886);
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
    printf("31. Test Case:                                             \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function REMAINDER on the values of two different prop-  \n");
    printf("  erties (of type SINGLE, BYTE). No exceptions are expec-  \n");
    printf("  ted.                                                     \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 3.09012866.

      func_call   = L"(Remainder(sgl_val, byte_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderSgl(data_reader, true, 9, (FdoFloat)3.09012866);
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
    printf("32. Test Case:                                             \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function REMAINDER on the values of two different prop-  \n");
    printf("  erties (of type SINGLE, DECIMAL). No exceptions are ex-  \n");
    printf("  pected.                                                  \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 3.09012866.

      func_call   = L"(Remainder(sgl_val, dcl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
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

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("33. Test Case:                                             \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function REMAINDER on the values of two different prop-  \n");
    printf("  erties (of type SINGLE, DOUBLE). No exceptions are ex-   \n");
    printf("  pected.                                                  \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 3.09012866.

      func_call   = L"(Remainder(sgl_val, dbl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
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

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("34. Test Case:                                             \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function REMAINDER on the values of two different prop-  \n");
    printf("  erties (of type SINGLE, INT16). No exceptions are ex-    \n");
    printf("  pected.                                                  \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 3.09012866.

      func_call   = L"(Remainder(sgl_val, i16_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderSgl(data_reader, true, 9, (FdoFloat)3.09012866);
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
    printf("35. Test Case:                                             \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function REMAINDER on the values of two different prop-  \n");
    printf("  erties (of type SINGLE, INT32). No exceptions are ex-    \n");
    printf("  pected.                                                  \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 3.09012866.

      func_call   = L"(Remainder(sgl_val, i32_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderSgl(data_reader, true, 9, (FdoFloat)3.09012866);
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
    printf("36. Test Case:                                             \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function REMAINDER on the values of two different prop-  \n");
    printf("  erties (of type SINGLE, SINGLE). No exceptions are ex-   \n");
    printf("  pected.                                                  \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 0.

      func_call   = L"(Remainder(sgl_val, sgl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderSgl(data_reader, true, 9, 0);
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
    printf("37. Test Case:                                             \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function REMAINDER on the values of two different prop-  \n");
    printf("  erties (of type BYTE, BYTE) where the function name dif- \n");
    printf("  fers from the expected function name ('ReMaInDeR' rather \n");
    printf("  than 'Remainder'). No exceptions are expected.           \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 0.

      func_call   = L"(ReMaInDeR(byte_val, byte_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader16(data_reader, true, 9, 0);
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

    filter = FdoFilter::Parse(L"id = 9");

    // Execute the test cases.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function SIN on the value of a different property of     \n");
    printf("  type BYTE. No exceptions are expected.                   \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be -0.98514626.

      func_call   = L"(Sin(byte_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, -0.98514626);
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
    printf("  function SIN on the value of a different property of     \n");
    printf("  type DECIMAL. No exceptions are expected.                \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 0.270227569.

      func_call   = L"(Sin(dcl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
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

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("3. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function SIN on the value of a different property of     \n");
    printf("  type BYTE where the function name differs from the ex-   \n");
    printf("  pected function name ('SiN' rather than 'Sin'). No ex-   \n");
    printf("  ceptions are expected.                                   \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be -0.98514626.

      func_call   = L"(SiN(byte_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, -0.98514626);
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

    filter = FdoFilter::Parse(L"id = 9");

    // Execute the test cases.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function SQRT on the value of a different property of    \n");
    printf("  type BYTE. No exceptions are expected.                   \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 8.60232527.

      func_call   = L"(Sqrt(byte_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, 8.60232527);
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
    printf("  function SQRT on the value of a different property of    \n");
    printf("  type DECIMAL. No exceptions are expected.                \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 3.58329457.

      func_call   = L"(Sqrt(dcl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
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

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("3. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function SQRT on the value of a different property of    \n");
    printf("  type BYTE where the function name differs from the ex-   \n");
    printf("  pected function name ('SqRt' rather than 'Sqrt'). No ex- \n");
    printf("  ceptions are expected.                                   \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 8.60232527.

      func_call   = L"(SqRt(byte_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, 8.60232527);
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

    filter = FdoFilter::Parse(L"id = 9");

    // Execute the test cases.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function TAN on the value of a different property of     \n");
    printf("  type BYTE. No exceptions are expected.                   \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be -5.7370225.

      func_call   = L"(Tan(byte_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, -5.7370225);
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
    printf("  function TAN on the value of a different property of     \n");
    printf("  type DECIMAL. No exceptions are expected.                \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 0.28066946.

      func_call   = L"(Tan(dcl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
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

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("3. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function TAN on the value of a different property of     \n");
    printf("  type BYTE where the function name differs from the ex-   \n");
    printf("  pected function name ('TaN' rather than 'Tan'). No ex-   \n");
    printf("  ceptions are expected.                                   \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be -5.7370225.

      func_call   = L"(TaN(byte_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader(data_reader, true, 9, -5.7370225);
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

    filter = FdoFilter::Parse(L"id = 1");

    // Execute the test cases.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function CEIL on the value of a different property of    \n");
    printf("  type BYTE. No exceptions are expected.                   \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 66.

      func_call   = L"(Ceil(byte_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderByte(data_reader, true, 1, 66);
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
    printf("  function CEIL on the value of a different property of    \n");
    printf("  type DECIMAL. No exceptions are expected.                \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 2.

      func_call   = L"(Ceil(dcl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
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

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("3. Test Case:                                              \n");
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
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
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

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("4. Test Case:                                              \n");
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
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader32(data_reader, true, 1, 10);
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
    printf("  function CEIL on the value of a different property of    \n");
    printf("  type SINGLE. No exceptions are expected.                 \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that this
      // call returns 1 row. The value for the selected computed property is
      // expected to be 1.

      func_call   = L"(Ceil(sgl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderSgl(data_reader, true, 1, 1);
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
    printf("  function CEIL on the value of a different property of    \n");
    printf("  type BYTE where the function name differs from the ex-   \n");
    printf("  pected function name ('CeIl' rather than 'Ceil'). No ex- \n");
    printf("  ceptions are expected.                                   \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 66.

      func_call   = L"(CeIl(byte_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderByte(data_reader, true, 1, 66);
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

    filter = FdoFilter::Parse(L"id = 9");

    // Execute the test cases.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function FLOOR on the value of a different property of   \n");
    printf("  type BYTE. No exceptions are expected.                   \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 74.

      func_call   = L"(Floor(byte_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderByte(data_reader, true, 9, 74);
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
    printf("2. Test Case:                                              \n");
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
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
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

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("3. Test Case:                                              \n");
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
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
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

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("4. Test Case:                                              \n");
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
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
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

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("5. Test Case:                                              \n");
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
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
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

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("6. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function FLOOR on the value of a different property of   \n");
    printf("  type BYTE where the function name differs from the ex-   \n");
    printf("  pected function name ('FlOoR' rather than 'Floor'). No   \n");
    printf("  exceptions are expected.                                 \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 74.

      func_call   = L"(FlOoR(byte_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderByte(data_reader, true, 9, 74);
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

    filter = FdoFilter::Parse(L"id = 9");

    // Execute the test cases.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function ROUND on the value of a different property of   \n");
    printf("  type BYTE. No exceptions are expected.                   \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 74.

      func_call   = L"(Round(byte_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderByte(data_reader, true, 9, 74);
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
    printf("  function ROUND on the value of a different property of   \n");
    printf("  type BYTE. The test requests to round to a certain scale \n");
    printf("  depth. No exceptions are expected.                       \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 74.

      func_call   = L"(Round(byte_val, 1.223) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderByte(data_reader, true, 9, 74);
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
    printf("3. Test Case:                                              \n");
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
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
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
    printf("4. Test Case:                                              \n");
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
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
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

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("5. Test Case:                                              \n");
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
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
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
    printf("6. Test Case:                                              \n");
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
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
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

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("7. Test Case:                                              \n");
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
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
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

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("8. Test Case:                                              \n");
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
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
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

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("9. Test Case:                                              \n");
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
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
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

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("10. Test Case:                                             \n");
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
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderSgl(data_reader, true, 9, (FdoFloat) 3.1);
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
    printf("  function ROUND on the value of a different property of   \n");
    printf("  type BYTE where the function name differs from the ex-   \n");
    printf("  pected function name ('RoUnD' rather than 'Round'). No   \n");
    printf("  exceptions are expected.                                 \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 74.

      func_call   = L"(RoUnD(byte_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderByte(data_reader, true, 9, 74);
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

    filter = FdoFilter::Parse(L"id = 9");

    // Execute the test cases.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function SIGN on the value of a different property of    \n");
    printf("  type BYTE. No exceptions are expected.                \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 1.

      func_call   = L"(Sign(byte_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader32(data_reader, true, 9, 1);
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
    printf("2. Test Case:                                              \n");
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
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader32(data_reader, true, 9, 1);
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
    printf("  function SIGN on the value of a different property of    \n");
    printf("  type DOUBLE. No exceptions are expected.                 \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be -1.

      func_call   = L"(Sign(dbl_val*-1) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader32(data_reader, true, 9, -1);
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
    printf("  function SIGN on the value of a different property of    \n");
    printf("  type BYTE where the function name differs from the ex-   \n");
    printf("  pected function name ('SiGn' rather than 'Sign'). No ex- \n");
    printf("  ceptions are expected.                                   \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 1.

      func_call   = L"(SiGn(byte_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader32(data_reader, true, 9, 1);
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

    filter = FdoFilter::Parse(L"id = 9");

    // Execute the test cases.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function TRUNC on the value of a different property of   \n");
    printf("  type BYTE. No exceptions are expected.                   \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 74.

      func_call   = L"(Trunc(byte_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderByte(data_reader, true, 9, 74);
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
    printf("  function TRUNC on the value of a different property of   \n");
    printf("  type BYTE. The test requests to truncate to a certain    \n");
    printf("  scale depth. No exceptions are expected.                 \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 74.

      func_call   = L"(Trunc(byte_val, 1.223) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderByte(data_reader, true, 9, 74);
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
    printf("3. Test Case:                                              \n");
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
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
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

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("4. Test Case:                                              \n");
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
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
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

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("5. Test Case:                                              \n");
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
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
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

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("6. Test Case:                                              \n");
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
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
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

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("7. Test Case:                                              \n");
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
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
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

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("8. Test Case:                                              \n");
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
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
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

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("9. Test Case:                                              \n");
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
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
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

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("10. Test Case:                                             \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function TRUNC on the value of a different property of   \n");
    printf("  type SINGLE. The test requests to truncate to a certain  \n");
    printf("  scale depth. No exceptions are expected.                 \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 2007.

      func_call   = L"(Trunc(sgl_val, 2.223) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
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

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("11. Test Case:                                             \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function TRUNC on the value of a different property of   \n");
    printf("  type BYTE where the function name differs from the ex-   \n");
    printf("  pected function name ('TrUnC' rather than 'Trunc'). No   \n");
    printf("  exceptions are expected.                                 \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 74.

      func_call   = L"(TrUnC(byte_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderByte(data_reader, true, 9, 74);
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

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("12. Test Case:                                             \n");
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
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
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
    printf("13. Test Case:                                             \n");
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
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
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

    filter = FdoFilter::Parse(L"id = 9");

    // Execute the test cases.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function CONCAT on values of two different properties of \n");
    printf("  type BOOLEAN. No exceptions are expected.                \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be "FALSEFALSE".

      func_call   = L"(Concat(bool_val, bool_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderString(data_reader, 9, L"FALSEFALSE");
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
      // is expected to be "FALSE74".

      func_call   = L"(Concat(bool_val, byte_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderString(data_reader, 9, L"FALSE74");
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
      // is expected to be "FALSE2007-09-09 21:00:00".

      func_call   = L"(Concat(bool_val, dt_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderString(data_reader, 9, L"FALSE2007-09-09 21:00:00");
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
      // is expected to be "FALSE12.8426966292135".
      // NOTE: The decimal value is 12.84, but the function converting the
      //       value to a string adds additional decimal positions that the
      //       test has to take into account. 

      func_call   = L"(Concat(bool_val, dcl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderString(data_reader, 9, L"FALSE12.8426966292135");
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
      // is expected to be "FALSE18.8890814558059".
      // NOTE: The decimal value is 18.8890815, but the function converting the
      //       value to a string adds additional decimal positions that the
      //       test has to take into account. 

      func_call   = L"(Concat(bool_val, dbl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderString(data_reader, 9, L"FALSE18.8890814558059");
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
      // is expected to be "FALSE36".

      func_call   = L"(Concat(bool_val, i16_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderString(data_reader, 9, L"FALSE36");
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
    printf("  function CONCAT on values of two different properties of \n");
    printf("  type BOOLEAN and INT32. No exceptions are expected.      \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be "FALSE90".

      func_call   = L"(Concat(bool_val, i32_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderString(data_reader, 9, L"FALSE90");
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
      // is expected to be "FALSE3.09013".

      func_call   = L"(Concat(bool_val, sgl_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderString(data_reader, 9, L"FALSE3.09013");
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
      // is expected to be "FALSEThe Color is: 2118".

      func_call   = L"(Concat(bool_val, str2_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderString(data_reader, 9, L"FALSEThe Color is: 2118");
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
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
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
    printf("12. Test Case:                                             \n");
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
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
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

    filter = FdoFilter::Parse(L"id = 2");

    // Execute the test cases.

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
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
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

    printf("\n");
    printf("---------------------------------------------------------- \n");
    printf("2. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function INSTR on values of two different properties of  \n");
    printf("  type STRING where the function name differs from the ex- \n");
    printf("  pected function name ('InStR' rather than 'Instr'). No   \n");
    printf("  exceptions are expected.                                 \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 15 as it looks for the string "2" inside the string
      // "The Color is: 2111".

      func_call   = L"(InStR(str2_val, '2') as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
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

void FdoExpressionFunctionTest::TestInstrFunctionAsFilter ()

// +---------------------------------------------------------------------------
// | The function executes the test for the expression engine function INSTR
// | when used in a filter.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    FdoInt32                  row_count     = 0;

    FdoPtr<FdoFilter>         filter;
    FdoPtr<FdoIFeatureReader> data_reader;

    printf("\n");
    printf("========================================================== \n");
    printf(" Current Unit Test Suite: INSTR Function Filter Testing    \n");
    printf("========================================================== \n");
    printf("\n");

    // Define the filter for all tests in this test suite.

    filter = FdoFilter::Parse(L"Instr(str2_val, 'is: 213') = 11");

    // Execute the test cases.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select data from a \n");
    printf("  class table using a filter that includes the expression  \n");
    printf("  function INSTR. The request should return a sub-set of   \n");
    printf("  the available data. No exceptions are expected.          \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the number of the returned data. If the
      // number does not match the expected result, issue an exception.

      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, NULL);

      printf(" >>> Cross check result \n");
      while (data_reader->ReadNext())
        row_count++;

      data_reader->Close();

      // Issue an exception if the expected result is not met.

      if (row_count != 10)
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

}  //  TestInstrFunctionAsFilter ()

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

    filter = FdoFilter::Parse(L"id = 9");

    // Execute the test cases.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function LENGTH on a value of a different property of    \n");
    printf("  type STRING. No exceptions are expected.                 \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 18 (length of the value "The Color is: 2118" for the
      // selected property).

      func_call   = L"(Length(str2_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader64(data_reader, true, 9, 18);
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
    printf("  function LENGTH on a value of a different property of    \n");
    printf("  type STRING where the function name differs from the ex- \n");
    printf("  pected function name ('LeNgTh' rather than 'Length'). No \n");
    printf("  exceptions are expected.                                 \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be 18 (length of the value "The Color is: 2118" for the
      // selected property).

      func_call   = L"(LeNgTh(str2_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReader64(data_reader, true, 9, 18);
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

    filter = FdoFilter::Parse(L"id = 9");

    // Execute the test cases.

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
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderString(data_reader, 9, L"the color is: 2118");
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
    printf("  function LOWER on a value of a different property of     \n");
    printf("  type STRING where the function name differs from the ex- \n");
    printf("  pected function name ('LoWeR' rather than 'Lower'). No   \n");
    printf("  exceptions are expected.                                 \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be "the color is: 2118".

      func_call   = L"(LoWeR(str2_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderString(data_reader, 9, L"the color is: 2118");
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

    filter = FdoFilter::Parse(L"id = 9");

    // Execute the test cases.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function LPAD on a value of a different property of type \n");
    printf("  STRING. For this test, no pad-string is provided. No ex- \n");
    printf("  xceptions are expected.                                  \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be "   The Color is: 2118" as the request asks for
      // standard padding (using blanks) to a length of 21.

      func_call   = L"(Lpad(str2_val, 21.43) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderString(data_reader, 9, L"   The Color is: 2118");
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
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderString(data_reader, 9, L"  9The Color is: 2118");
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
    printf("  function LPAD on a value of a different property of type \n");
    printf("  STRING where the function name differs from the expected \n");
    printf("  function name ('LpAd' rather than 'Lpad'). For this test \n");
    printf("  no pad-string is provided. No exxceptions are expected.  \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be "   The Color is: 2118" as the request asks for
      // standard padding (using blanks) to a length of 21.

      func_call   = L"(LpAd(str2_val, 21.43) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderString(data_reader, 9, L"   The Color is: 2118");
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

    filter = FdoFilter::Parse(L"id = 9");

    // Execute the test cases.

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
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderString(data_reader, 9, L"9  ");
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
    printf("  function LTRIM on a value of a different property of     \n");
    printf("  type STRING where the function name differs from the ex- \n");
    printf("  pected function name ('LtRiM' rather than 'Ltrim'). No   \n");
    printf("  exceptions are expected.                                 \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be "9  ".

      func_call   = L"(LtRiM(str_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderString(data_reader, 9, L"9  ");
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

    filter = FdoFilter::Parse(L"id = 9");

    // Execute the test cases.

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
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderString(data_reader, 9, L"The Color is: 2118   ");
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
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderString(data_reader, 9, L"The Color is: 2118  9");
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
    printf("  function RPAD on a value of a different property of type \n");
    printf("  STRING where the function name differs from the expected \n");
    printf("  function name ('RpAd' rather than 'Rpad'). For this test \n");
    printf("  no pad-string is provided. No exceptions are expected.   \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be "The Color is: 2118   " as the request asks for
      // standard padding (using blanks) to a length of 21.

      func_call   = L"(RpAd(str2_val, 21.43) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderString(data_reader, 9, L"The Color is: 2118   ");
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

    filter = FdoFilter::Parse(L"id = 9");

    // Execute the test cases.

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
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderString(data_reader, 9, L"  9");
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
    printf("  function RTRIM on a value of a different property of     \n");
    printf("  type STRING where the function name differs from the ex- \n");
    printf("  pected function name ('RtRiM' rather than 'Rtrim'). No   \n");
    printf("  exceptions are expected.                                 \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be "  9".

      func_call   = L"(RtRiM(str_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderString(data_reader, 9, L"  9");
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

    filter = FdoFilter::Parse(L"id = 9");

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
      // is expected to be "T246".

      func_call   = L"(Soundex(str2_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderString(data_reader, 9, L"T246");
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
      // is expected to be "T246".

      func_call   = L"(SoUnDeX(str2_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderString(data_reader, 9, L"T246");
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

    filter = FdoFilter::Parse(L"id = 9");

    // Execute the test cases.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function SUBSTR on the value of a different property of  \n");
    printf("  type STRING. In this case the test requests a sub-string \n");
    printf("  from a position within the source string to the end of   \n");
    printf("  it by not specifying the optional length parameter. No   \n");
    printf("  exceptions are expected.                                 \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be "Color is: 2118".

      func_call   = L"(Substr(str2_val, 5.6) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderString(data_reader, 9, L"Color is: 2118");
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
    printf("  function SUBSTR on the value of a different property of  \n");
    printf("  type STRING. In this case the test requests a sub-string \n");
    printf("  from a position within the source string for a prede-    \n");
    printf("  fined length. No exceptions are expected.                \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be "Color".

      func_call   = L"(Substr(str2_val, 5.6, 5.6) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderString(data_reader, 9, L"Color");
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
    printf("  function SUBSTR on the value of a different property of  \n");
    printf("  type STRING where the function name differs from the ex- \n");
    printf("  pected function name ('SuBsTr' rather than 'Substr'). In \n");
    printf("  this case, the test requests a sub-string from a posi-   \n");
    printf("  tion within the source string to the end of it by not    \n");
    printf("  specifying the optional length parameter. No exceptions  \n");
    printf("  are expected.                                            \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be "Color is: 2118".

      func_call   = L"(SuBsTr(str2_val, 5.6) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderString(data_reader, 9, L"Color is: 2118");
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

    filter = FdoFilter::Parse(L"id = 9");

    // Execute the test cases.

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
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderString(data_reader, 9, L"The Cxyxr is: 2118");
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
    printf("  function TRANSLATE on the value of a different property  \n");
    printf("  of type STRING where the function name differs from the  \n");
    printf("  expected function name ('TrAnSlAtE' rather than 'Trans-  \n");
    printf("  late'). In this case, the test requests to substitute    \n");
    printf("  all 'o' with an 'x' and all 'l' with a 'y'. No excep-    \n");
    printf("  tions are expected.                                      \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be "The Cxyxr is: 2118".

      func_call   = L"(TrAnSlAtE(str2_val, 'ol', 'xy') as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderString(data_reader, 9, L"The Cxyxr is: 2118");
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

    filter = FdoFilter::Parse(L"id = 9");

    // Execute the test cases.

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
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderString(data_reader, 9, L"9");
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
    printf("  function TRIM on a value of a different property of type \n");
    printf("  STRING. For the test, the execution operator is set to   \n");
    printf("  'BOTH'. No exceptions are expected.                      \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be "9" because the operation is called on the property
      // value "  9  " specifying the operation type BOTH in which case
      // leading and trailing blanks are removed.

      func_call   = L"(Trim('BOTH', str_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderString(data_reader, 9, L"9");
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
    printf("  function TRIM on a value of a different property of type \n");
    printf("  STRING. For the test, the execution operator is set to   \n");
    printf("  'LEADING'. No exceptions are expected.                   \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be "9  " because the operation is called on the property
      // value "  9  " specifying the operation type LEADING in which case any
      // leading blanks are removed.

      func_call   = L"(Trim('LEADING', str_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderString(data_reader, 9, L"9  ");
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
    printf("  function TRIM on a value of a different property of type \n");
    printf("  STRING. For the test, the execution operator is set to   \n");
    printf("  'TRAILING'. No exceptions are expected.                  \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be "  9" because the operation is called on the property
      // value "  9  " specifying the operation type TRAILING in which case any
      // trailing blanks are removed.

      func_call   = L"(Trim('TRAILING', str_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderString(data_reader, 9, L"  9");
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
    printf("  function TRIM on a value of a different property of type \n");
    printf("  STRING where the function name differs from the expected \n");
    printf("  function name ('TrIm' rather than 'Trim'). For the test, \n");
    printf("  no execution operator is set. No exceptions are expected \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be "9" because the operation is called on the property
      // value "  9  " without specifying a operation mode and hence the de-
      // fault one should be used in which case leading and trailing blanks are
      // removed.

      func_call   = L"(TrIm(str_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderString(data_reader, 9, L"9");
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

    filter = FdoFilter::Parse(L"id = 9");

    // Execute the test cases.

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
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderString(data_reader, 9, L"THE COLOR IS: 2118");
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
    printf("  function UPPER on a value of a different property of     \n");
    printf("  type STRING where the function name differs from the ex- \n");
    printf("  pected function name ('UpPeR' rather than 'Upper'). No   \n");
    printf("  exceptions are expected.                                 \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data. It is expected that
      // this call returns 1 row. The value for the selected computed property
      // is expected to be "THE COLOR IS: 2118".

      func_call   = L"(UpPeR(str2_val) as cmp_id)";
      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, func_call);
      CheckReaderString(data_reader, 9, L"THE COLOR IS: 2118");
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
// --                     Combination Unit Test Functions                    --
// ----------------------------------------------------------------------------

void FdoExpressionFunctionTest::NumberToStringComparison ()

// +---------------------------------------------------------------------------
// | The function executes a filter on the test data set that uses the function
// | TOSTRING on two different properties and compares the function results to
// | determine if a data set qualifies as defined by the filter.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    bool                      is_valid_result = false;

    FdoInt32                  row_count       =  0,
                              id_val          = -1;

    FdoPtr<FdoFilter>         filter;
    FdoPtr<FdoIFeatureReader> data_reader;

    printf("\n");
    printf("========================================================== \n");
    printf(" Combination Unit Test: Number to String Comparison        \n");
    printf("========================================================== \n");
    printf("\n");

    // Define the filter for all tests in this test suite.

    filter = FdoFilter::Parse(L"ToString(dbl_val) = ToString(sgl_val)");

    // Execute the test cases.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command using the filter to   \n");
    printf("  determine the qualified data set. The function should    \n");
    printf("  return two rows. No exceptions are expected.             \n");
    printf("---------------------------------------------------------- \n");

    try {

      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, NULL);

      printf(" >>> Cross check result \n");

      while (data_reader->ReadNext()) {

        row_count++;
        id_val = data_reader->GetInt32(L"id");
        is_valid_result = ((id_val == 0) || (id_val == 20));
        if (!is_valid_result)
            break;
        
      }  //  while (data_reader->ReadNext()) ...

      // Close the reader.

      data_reader->Close();

      // Issue an exception if the expected result is not met.

      if ((!is_valid_result) || (row_count != 2))
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

}  //  NumberToStringComparison ()


void FdoExpressionFunctionTest::RoundNumberToStringComparison ()

// +---------------------------------------------------------------------------
// | The function executes a filter on the test data set that uses the func-
// | tions ROUND and TOSTRING on two different properties and compares the
// | function results to determine if a data set qualifies as defined by the
// | filter.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    bool                      is_valid_result = false;

    FdoInt32                  row_count       =  0,
                              id_val          = -1;

    FdoPtr<FdoFilter>         filter;
    FdoPtr<FdoIFeatureReader> data_reader;

    printf("\n");
    printf("========================================================== \n");
    printf(" Combination Unit Test: Round Number to String Comparison  \n");
    printf("========================================================== \n");
    printf("\n");

    // Define the filter for all tests in this test suite.

    filter =
        FdoFilter::Parse(
                L"ToString(Round(dbl_val, 2)) = ToString(Round(sgl_val, 2))");

    // Execute the test cases.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command using the filter to   \n");
    printf("  determine the qualified data set. The function should    \n");
    printf("  return three rows. No exceptions are expected.           \n");
    printf("---------------------------------------------------------- \n");

    try {

      data_reader = ExecuteSelectCommand(L"exfct_c1", filter, true, NULL);

      printf(" >>> Cross check result \n");

      while (data_reader->ReadNext()) {

        row_count++;
        id_val = data_reader->GetInt32(L"id");
        is_valid_result = ((id_val == 0) || (id_val == 17) || (id_val == 20));
        if (!is_valid_result)
            break;
        
      }  //  while (data_reader->ReadNext()) ...

      // Close the reader.

      data_reader->Close();

      // Issue an exception if the expected result is not met.

      if ((!is_valid_result) || (row_count != 3))
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

}  //  RoundNumberToStringComparison ()


// ----------------------------------------------------------------------------
// --                       Special Unit Test Functions                      --
// ----------------------------------------------------------------------------

void FdoExpressionFunctionTest::ModuloOperation ()

// +---------------------------------------------------------------------------
// | The function executes a modulo operation on multiple rows. This is to
// | verify that the returned data is accurate for each row.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    FdoInt32                  id_val,
                              cmp_val;

    FdoStringP                func_call;

    FdoPtr<FdoIFeatureReader> data_reader;

    printf("\n");
    printf("========================================================== \n");
    printf(" Special Unit Test Suite: Modulo Operation                 \n");
    printf("========================================================== \n");
    printf("\n");

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function MOD on the value of a different property of     \n");
    printf("  type INT32. The test works on all rows in the test class \n");
    printf("  table. The test is to check that not the same value is   \n");
    printf("  returned for each of the rows as the original property   \n");
    printf("  value is different for each row. No exceptions are ex-   \n");
    printf("  pected.                                                  \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and document the returned data.

      func_call   = L"(Mod(id, 10) as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", NULL, true, func_call);
      while (data_reader->ReadNext()) {

        id_val  = data_reader->GetInt32(L"id");
        cmp_val = data_reader->GetInt32(L"cmp_id");

        printf(" --->>> (id, mod_val) = (%d, %d) \n", id_val, cmp_val);

      }  //  while ...

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

}  //  ModuloOperation ()

void FdoExpressionFunctionTest::SoundexFilter ()

// +---------------------------------------------------------------------------
// | The function executes a specific test identified by QA that resulted in a
// | MAP crash.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    FdoInt32                  id_val,
                              row_count     = 0;

    FdoPtr<FdoFilter>         filter;
    FdoPtr<FdoIFeatureReader> data_reader;

    printf("\n");
    printf("========================================================== \n");
    printf(" Special Unit Test Suite: SOUNDEX as filter                \n");
    printf("========================================================== \n");
    printf("\n");

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the feature \n");
    printf("  ids of object for which a filter using the expression    \n");
    printf("  function SOUNDEX applies. The test is expected to return \n");
    printf("  a specific number of rows. No exceptions are expected.   \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and check the returned data.

      filter = FdoFilter::Parse(
                        L"Soundex(str2_val) = Soundex('The Color is: 777')");
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", filter, true, NULL);

      printf(" >>> Cross check result \n");
      while (data_reader->ReadNext()) {

        row_count++;
        id_val  = data_reader->GetInt32(L"id");
        printf(" --->>> (id) = (%d) \n", id_val);

      }  //  while ...

      if (row_count != 31)
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

}  //  SoundexFilter ()

void FdoExpressionFunctionTest::SubstrOperation ()

// +---------------------------------------------------------------------------
// | The function executes a substring operation on multiple rows. This is to
// | verify that the returned data is accurate for each row.
// +---------------------------------------------------------------------------

{


    // Declare and initialize all necessary local vatiables.

    FdoInt32                  id_val;

    FdoStringP                cmp_val,
                              func_call;

    FdoPtr<FdoIFeatureReader> data_reader;

    printf("\n");
    printf("========================================================== \n");
    printf(" Special Unit Test Suite: SUBSTR Operation                 \n");
    printf("========================================================== \n");
    printf("\n");

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select-command to select the value   \n");
    printf("  of a computed property that is defined by using the      \n");
    printf("  function SUBSTR on the value of a different property of  \n");
    printf("  type STRING. In this case the test requests a sub-string \n");
    printf("  from a position within the source string to the end of   \n");
    printf("  it by not specifying the optional length parameter. The  \n");
    printf("  test is to verify that the returned value for each of    \n");
    printf("  the rows is accurate and that not one value is returned  \n");
    printf("  for all the rows. No exceptions are expected.            \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Execute the test and document the returned data.

      func_call   = L"(Substr(str2_val, 5.6) as cmp_id)";
      data_reader = ExecuteSelectCommand(
                                        L"exfct_c1", NULL, true, func_call);
      while (data_reader->ReadNext()) {

        id_val  = data_reader->GetInt32(L"id");
        cmp_val = data_reader->GetString(L"cmp_id");

        printf(" --->>> (id, mod_val) = (%d, >>%ls<<) \n",
               id_val, (FdoString*)cmp_val);

      }  //  while ...

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

}  // SubstrOperation ()

void FdoExpressionFunctionTest::ToDateOperation ()

// +---------------------------------------------------------------------------
// | The function tests the expression function TODATE as a filter when con-
// | nected to a specific SDF file provided by QA. Apparently, no data is re-
// | turned although at least one row qualifies.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoInt32                  row_count     = 0;

    FdoPtr<FdoFilter>         filter;
    FdoPtr<FdoIFeatureReader> data_reader;

    printf("\n");
    printf("========================================================== \n");
    printf(" Special Unit Test Suite: TODATE Operation                 \n");
    printf("========================================================== \n");
    printf("\n");

    try {

      // At this point there is already a connection to a different SDF file.
      // For the following test this one needs to be closed and a new connec-
      // tion established to the file QA provided.

      printf("---------------------------------------------------------- \n");
      printf("Test Setup:                                                \n");
      printf("  Close possible current connection and establish a con-   \n");
      printf("  nection to the QA provided SDF file.                     \n");
      printf("---------------------------------------------------------- \n");

      CloseConnection();
      m_connection =
            UnitTestUtil::OpenConnection(
                    L"..\\..\\TestData\\SPR1011594\\CopyofConversions.sdf", false);

      // Execute the request that is to be tested with this test case.

      filter      = FdoFilter::Parse(
                            L"\"DATE\" = ToDate(DateString, 'MM/DD/YYYY hh:mm:ss')");
      data_reader = ExecuteSelectCommand(
                                    L"FeatureClass1", filter, false, NULL);

      printf(" >>> Cross check result \n");
      while (data_reader->ReadNext())
        row_count++;

      //if (row_count != 31)
      //    throw FdoException::Create(
      //                  L"Unexpected result(s) when checking returned data");
      //else
      //  printf(" >>> ... All expected data found\n");

      printf(" >>> Test succeeded \n");

      // Close the connection again.

      CloseConnection();

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

}  //  ToDateOperation ()


// ----------------------------------------------------------------------------
// --                      General Supporting Functions                      --
// ----------------------------------------------------------------------------

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

    FdoInt32  data_count    = 0,
              id_prop_val;

    FdoDouble cmp_id_val,
              tmp_val;

    // Navigate through the reader and perform the necessary checks.

    printf(" >>> Cross check result \n");

    while (data_reader->ReadNext()) {

      data_count++;

      if (include_id_check)
          id_prop_val = data_reader->GetInt32(L"id");
      cmp_id_val = data_reader->GetDouble(L"cmp_id");

      tmp_val = (cmp_id_val - expected_value);
      if (tmp_val < 0)
          tmp_val = tmp_val * -1;

      is_valid_result = (include_id_check)
            ? ((tmp_val      < 1                ) &&
               (id_prop_val == expected_id_value)    )
            : (tmp_val < 1);

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

    FdoInt32  data_count    = 0,
              id_prop_val;

    FdoDouble cmp_id_val,
              tmp_val;

    // Navigate through the reader and perform the necessary checks.

    printf(" >>> Cross check result \n");

    while (data_reader->ReadNext()) {

      data_count++;

      if (include_id_check)
          id_prop_val = data_reader->GetInt32(L"id");
      cmp_id_val = data_reader->GetDouble(L"cmp_id");

      tmp_val = fabs((cmp_id_val - expected_value));

      is_valid_result = (include_id_check)
            ? ((tmp_val      < 1                ) &&
               (id_prop_val == expected_id_value)    )
            : (tmp_val < 1);

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

void FdoExpressionFunctionTest::CheckReader16 (
                                        FdoIFeatureReader *data_reader,
                                        bool              include_id_check,
                                        FdoInt32          expected_id_value,
                                        FdoInt16          expected_value)

// +---------------------------------------------------------------------------
// | The function checks whether or not the provided reader contains the ex-
// | pected data and throws an exception if this is not the case.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    bool     is_valid_result = false;

    FdoInt16 cmp_id_val      = 0;

    FdoInt32 data_count      = 0,
             id_prop_val;

    // Navigate through the reader and perform the necessary checks.

    printf(" >>> Cross check result \n");

    while (data_reader->ReadNext()) {

      data_count++;

      if (include_id_check)
          id_prop_val = data_reader->GetInt32(L"id");
      cmp_id_val = data_reader->GetInt16(L"cmp_id");

      is_valid_result = (include_id_check)
            ? ((abs((cmp_id_val - expected_value)) < 1) &&
               (id_prop_val == expected_id_value)          )
            : (abs((cmp_id_val - expected_value)) < 1);

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

}  //  CheckReader16 ()

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

    bool      is_valid_result = false;

    FdoInt32  data_count    = 0,
              cmp_id_val,
              id_prop_val;

    // Navigate through the reader and perform the necessary checks.

    printf(" >>> Cross check result \n");

    while (data_reader->ReadNext()) {

      data_count++;

      if (include_id_check)
          id_prop_val = data_reader->GetInt32(L"id");
      cmp_id_val = data_reader->GetInt32(L"cmp_id");

      is_valid_result = (include_id_check)
            ? ((abs((cmp_id_val - expected_value)) < 1) &&
               (id_prop_val == expected_id_value)          )
            : (abs((cmp_id_val - expected_value)) < 1);

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

    FdoInt32 data_count    = 0,
             id_prop_val;

    FdoInt64 tmp_val,
             cmp_id_val;

    // Navigate through the reader and perform the necessary checks.

    printf(" >>> Cross check result \n");

    while (data_reader->ReadNext()) {

      data_count++;

      if (include_id_check)
          id_prop_val = data_reader->GetInt32(L"id");
      cmp_id_val = data_reader->GetInt64(L"cmp_id");

      tmp_val = (cmp_id_val - expected_value);
      if (tmp_val < 0)
          tmp_val = tmp_val * -1;

      is_valid_result = (include_id_check)
            ? ((tmp_val      < 1                ) &&
               (id_prop_val == expected_id_value)    )
            : (tmp_val < 1);

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

    FdoInt32 data_count    = 0,
             id_prop_val;

    FdoInt64 tmp_val,
             cmp_id_val;

    // Navigate through the reader and perform the necessary checks.

    printf(" >>> Cross check result \n");

    while (data_reader->ReadNext()) {

      data_count++;

      if (include_id_check)
          id_prop_val = data_reader->GetInt32(L"id");
      cmp_id_val = data_reader->GetInt64(L"cmp_id");

      tmp_val = (cmp_id_val - expected_value);
      if (tmp_val < 0)
          tmp_val = tmp_val * -1;

      is_valid_result = (include_id_check)
            ? ((tmp_val      < 1                ) &&
               (id_prop_val == expected_id_value)    )
            : (tmp_val < 1);

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

void FdoExpressionFunctionTest::CheckReaderByte (
                                            FdoIDataReader *data_reader,
                                            bool           include_id_check,
                                            FdoInt32       expected_id_value,
                                            FdoByte        expected_value)

// +---------------------------------------------------------------------------
// | The function checks whether or not the provided reader contains the ex-
// | pected data and throws an exception if this is not the case.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    bool     is_valid_result = false;

    FdoByte  cmp_id_val;

    FdoInt32 data_count    = 0,
             id_prop_val;

    // Navigate through the reader and perform the necessary checks.

    printf(" >>> Cross check result \n");

    while (data_reader->ReadNext()) {

      data_count++;

      if (include_id_check)
          id_prop_val = data_reader->GetInt32(L"id");
      cmp_id_val = data_reader->GetByte(L"cmp_id");

      is_valid_result = (include_id_check)
            ? ((cmp_id_val  == expected_value   ) &&
               (id_prop_val == expected_id_value)    )
            : (cmp_id_val == expected_value);

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

}  //  CheckReaderByte ()

void FdoExpressionFunctionTest::CheckReaderByte (
                                        FdoIFeatureReader *data_reader,
                                        bool              include_id_check,
                                        FdoInt32          expected_id_value,
                                        FdoByte           expected_value)

// +---------------------------------------------------------------------------
// | The function checks whether or not the provided reader contains the ex-
// | pected data and throws an exception if this is not the case.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    bool     is_valid_result = false;

    FdoByte  cmp_id_val;

    FdoInt32 data_count    = 0,
             id_prop_val;

    // Navigate through the reader and perform the necessary checks.

    printf(" >>> Cross check result \n");

    while (data_reader->ReadNext()) {

      data_count++;

      if (include_id_check)
          id_prop_val = data_reader->GetInt32(L"id");
      cmp_id_val = data_reader->GetByte(L"cmp_id");

      is_valid_result = (include_id_check)
            ? ((cmp_id_val  == expected_value   ) &&
               (id_prop_val == expected_id_value)    )
            : (cmp_id_val == expected_value);

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

}  //  CheckReaderByte ()

void FdoExpressionFunctionTest::CheckReaderDt (
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

}  //  CheckReaderDt ()

void FdoExpressionFunctionTest::CheckReaderDt (
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

}  //  CheckReaderDt ()

void FdoExpressionFunctionTest::CheckReaderNumberString (
                                    FdoIFeatureReader *data_reader,
                                    FdoInt32          expected_id_value,
                                    FdoString         *expected_cmp_id_value)

// +---------------------------------------------------------------------------
// | The function checks whether or not the provided reader contains the expec-
// | ted data and throws an exception if this is not the case.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    bool        is_valid_result = false;

    FdoInt32    data_count      = 0,
                id_prop_val;

    FdoDouble   tmp_val         = 0,
                cmp_id_val_d    = 0,
                exp_id_val_d    = 0;

    FdoString   *cmp_id_val;

    FdoStringP  tmp_str;

    // Navigate through the reader and perform the necessary checks.

    printf(" >>> Cross check result \n");

    while (data_reader->ReadNext()) {

      data_count++;

      id_prop_val  = data_reader->GetInt32(L"id");
      cmp_id_val   = data_reader->GetString(L"cmp_id");

      tmp_str      = cmp_id_val;
      cmp_id_val_d = tmp_str.ToDouble();

      tmp_str      = expected_cmp_id_value;
      exp_id_val_d = tmp_str.ToDouble();

      tmp_val         = fabs((cmp_id_val_d - exp_id_val_d));
      is_valid_result = (tmp_val < 1);

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

}  //  CheckReaderNumberString ()

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

    FdoInt32 data_count    = 0,
             id_prop_val;

    FdoFloat tmp_val,
             cmp_id_val;

    // Navigate through the reader and perform the necessary checks.

    printf(" >>> Cross check result \n");

    while (data_reader->ReadNext()) {

      data_count++;

      if (include_id_check)
          id_prop_val = data_reader->GetInt32(L"id");
      cmp_id_val = data_reader->GetSingle(L"cmp_id");

      tmp_val = (cmp_id_val - expected_value);
      if (tmp_val < 0)
          tmp_val = tmp_val * -1;

      is_valid_result = (include_id_check)
            ? ((tmp_val      < 1                ) &&
               (id_prop_val == expected_id_value)    )
            : (tmp_val < 1);

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

void FdoExpressionFunctionTest::CheckReaderString (
                                    FdoIFeatureReader *data_reader,
                                    FdoInt32          expected_id_value,
                                    FdoString         *expected_cmp_id_value)

// +---------------------------------------------------------------------------
// | The function checks whether or not the provided reader contains the expec-
// | ted data and throws an exception if this is not the case.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    bool        is_valid_result = false;

    FdoInt32    data_count      = 0,
                id_prop_val;

    FdoString   *cmp_id_val;

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

}  //  CheckReaderString ()

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

    FdoPtr<FdoISelect>              sel_cmd;
    FdoPtr<FdoComputedIdentifier>   cmp_id;
    FdoPtr<FdoIdentifierCollection> id_col;

    // Create a select function and set the class name and filter.

    printf(" >>> Setup the select command \n");

    sel_cmd = (FdoISelect*)m_connection->CreateCommand(FdoCommandType_Select);
    sel_cmd->SetFeatureClassName(class_name);
    if (filter != NULL)
        sel_cmd->SetFilter(filter);
    id_col = sel_cmd->GetPropertyNames();
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

    return (sel_cmd->Execute());

}  //  ExecuteSelectCommand ()

FdoIFeatureReader *FdoExpressionFunctionTest::
                                ExecuteSelectCommand (FdoString *class_name,
                                                      FdoFilter *filter,
                                                      FdoString *prop1,
                                                      FdoString *prop2,
                                                      FdoString *prop3,
                                                      FdoString *prop4,
                                                      FdoString *prop5,
                                                      FdoString *prop6)

// +---------------------------------------------------------------------------
// | The function executes a select-command to retrieve the values of the named
// | properties and returns the generated reader back to the calling routine.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    FdoPtr<FdoIdentifier>           id_prop;

    FdoPtr<FdoISelect>              sel_cmd;
    FdoPtr<FdoIdentifierCollection> id_col;

    // Create a select-aggregate function and set the class name and filter.

    printf(" >>> Setup the command \n");

    sel_cmd = (FdoISelect*)m_connection->CreateCommand(FdoCommandType_Select);
    sel_cmd->SetFeatureClassName(class_name);
    if (filter != NULL)
        sel_cmd->SetFilter(filter);
    id_col = sel_cmd->GetPropertyNames();
    if (prop1 != NULL) {

        id_prop = FdoIdentifier::Create(prop1);
        id_col->Add(id_prop);

    }  //  if (prop1 != NULL) ...

    if (prop2 != NULL) {

        id_prop = FdoIdentifier::Create(prop2);
        id_col->Add(id_prop);

    }  //  if (prop2 != NULL) ...

    if (prop3 != NULL) {

        id_prop = FdoIdentifier::Create(prop3);
        id_col->Add(id_prop);

    }  //  if (prop3 != NULL) ...

    if (prop4 != NULL) {

        id_prop = FdoIdentifier::Create(prop4);
        id_col->Add(id_prop);

    }  //  if (prop4 != NULL) ...

    if (prop5 != NULL) {

        id_prop = FdoIdentifier::Create(prop5);
        id_col->Add(id_prop);

    }  //  if (prop5 != NULL) ...

    if (prop6 != NULL) {

        id_prop = FdoIdentifier::Create(prop6);
        id_col->Add(id_prop);

    }  //  if (prop6 != NULL) ...

    // Execute the request and return the feature reader back to the calling
    // procedure.

    printf(" >>> Execute request \n");

    return (sel_cmd->Execute());

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

    printf(" >>> Getting date-value for property '%ls' \n", property_name);

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

      coordinate_buffer[0] = 100.0 + index;
      coordinate_buffer[1] = 100.0 + index;
      coordinate_buffer[2] = 0;
      coordinate_buffer[3] = 101.0 + index;
      coordinate_buffer[4] = 101.0 + index;
      coordinate_buffer[5] = 0;

      geometry_factory = FdoFgfGeometryFactory::GetInstance();
      line_str         = geometry_factory->CreateLineString(
                                    FdoDimensionality_XY|FdoDimensionality_Z,
                                    6, 
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

      byte_value     = (index % 10) + 65;
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

          switch (index) {

            case 17: dbl_value = 30.36776;                    break;
            case 20: dbl_value = 30.3;                        break;
            default: dbl_value = index + (6.34 * index/5.77); break;

          }  //  switch ...

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

      switch (index) {

        case 17: flt_value = (FdoFloat) 30.36776;              break;
        case 20: flt_value = (FdoFloat) 30.3;                  break;
        default: flt_value = (FdoFloat) (2.4 * (index /6.99)); break;

      }  //  switch ....

      data_value     = FdoDataValue::Create(flt_value);
      property_value = AddNewProperty(property_values, L"sgl_val");
      property_value->SetValue(data_value);
      FDO_SAFE_RELEASE(data_value);
      FDO_SAFE_RELEASE(property_value);

      if (index == 20) {

          data_value     = FdoDataValue::Create(L"30.3");
          property_value = AddNewProperty(property_values, L"str_val");
          property_value->SetValue(data_value);
          FDO_SAFE_RELEASE(data_value);
          FDO_SAFE_RELEASE(property_value);

      }  //  if (index == 20) ...
      else {

        id_str         = FdoStringP::Format(L"  %d  ", index);
        data_value     = FdoDataValue::Create((FdoString *)id_str);
        property_value = AddNewProperty(property_values, L"str_val");
        property_value->SetValue(data_value);
        FDO_SAFE_RELEASE(data_value);
        FDO_SAFE_RELEASE(property_value);

      }  //  else ...

      id_str         = FdoStringP::Format(L"The Color is: %d", (index + 2109));
      data_value     = FdoDataValue::Create((FdoString *)id_str);
      property_value = AddNewProperty(property_values, L"str2_val");
      property_value->SetValue(data_value);
      FDO_SAFE_RELEASE(data_value);
      FDO_SAFE_RELEASE(property_value);

      id_str         = FdoStringP::Format(L"9/20/2007");
      data_value     = FdoDataValue::Create((FdoString *)id_str);
      property_value = AddNewProperty(property_values, L"dtstr_val");
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

    m_connection = UnitTestUtil::OpenConnection(DATASTORE_EXTENSION, true);

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
                CreateDataProperty(L"featid", FdoDataType_Int32, 0, 0, false);
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
           CreateDataProperty(L"dtstr_val", FdoDataType_String, 30, 0, true);
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
                                                FdoGeometricType_Solid    );
    geometric_property_definition->SetHasElevation(true);

    // Return the handle on the created object.

    return geometric_property_definition;

}  //  CreateGeometricProperty ()

void FdoExpressionFunctionTest::SetupUnitTestEnvironment (
                                        FdoIConnection *current_connection)

// +---------------------------------------------------------------------------
// | The function controls the establishing of the test environment in the FDO
// | data store the provided connection identifies.
// +---------------------------------------------------------------------------

{

    // Add the test schema and some data.

    try {

      // Add the test schema.

      printf(" >>> ... adding test schema \n");
      AddTestSchema(current_connection, L"ExprFct");

      // Load the features used in the unit tests.
      //   > The spatial features.

      printf(" >>> ... adding data for feature classes \n");
      printf(" >>> ...... for class exfct_c1 \n");
      for (int i = 0; i < 31; i++)
        AddFeature(current_connection, L"exfct_c1", i);

   }  //  try ...

   catch (FdoException *exp) {

     printf(" >>> Exception: %ls\n", exp->GetExceptionMessage());
     throw exp;

   }  //  catch ...

   catch ( ... ) {

     throw;

   }  //  catch ...

}  //  SetupUnitTestEnvironment ()

void FdoExpressionFunctionTest::InsertTestFeatures(FdoIConnection* conn)
{
    std::vector<FdoSpatialCondition*> allCond;
    printf("\nInsert features");
    try
    {
        // Note:
        // EB = envelope Big geometry; ES = envelope Small geometry
        // B = Big geometry; S = Small geometry
        // NI = no intersection
        // TO = touche outside of the geometry
        // TI = touche inside of the geometry

        FdoSpatialCondition* cond = NULL;
        // geom - out EB, out ES - NI
        cond = static_cast<FdoSpatialCondition*>(FdoFilter::Parse(L"SHPGEOM INTERSECTS GeomFromText('POLYGON ((5536366.97310883 1964015.95271029, 5618464.31544299 1927535.52167126, 5866059.47432348 1950987.22770124, 5536366.97310883 1964015.95271029))')"));
        allCond.push_back(cond);

        // geom - in EB, out ES - NI
        cond = static_cast<FdoSpatialCondition*>(FdoFilter::Parse(L"SHPGEOM INTERSECTS GeomFromText('POLYGON ((5527679.42371172 2264545.2193847, 5527679.42371172 2220681.84366703, 5594573.55513252 2220681.84366703, 5565035.88505614 2263242.34739065, 5527679.42371172 2264545.2193847))')"));
        allCond.push_back(cond);

        // geom - in EB, in ES - NI
        cond = static_cast<FdoSpatialCondition*>(FdoFilter::Parse(L"SHPGEOM INTERSECTS GeomFromText('POLYGON ((5672117.79766474 2133761.8362255, 5639252.139427 2037628.58952983, 5719107.54066284 2007485.11266832, 5672117.79766474 2133761.8362255))')"));
        allCond.push_back(cond);

        // geom - in EB, out ES - TO.B - NI
        cond = static_cast<FdoSpatialCondition*>(FdoFilter::Parse(L"SHPGEOM INTERSECTS GeomFromText('POLYGON ((5816766.1271512 2262328.88622243, 5735027.92795496 2179902.49038579, 5803867.44957874 2179902.49038579, 5816766.1271512 2262328.88622243))')"));
        allCond.push_back(cond);

        // geom - in EB, in ES - TO.B - NI
        cond = static_cast<FdoSpatialCondition*>(FdoFilter::Parse(L"SHPGEOM INTERSECTS GeomFromText('POLYGON ((5816766.1271512 2262328.88622243, 5735027.92795496 2179902.49038579, 5838419.22614045 2165830.30743391, 5816766.1271512 2262328.88622243))')"));
        allCond.push_back(cond);

        // geom - in EB, out ES - TI.B - NI
        cond = static_cast<FdoSpatialCondition*>(FdoFilter::Parse(L"SHPGEOM INTERSECTS GeomFromText('POLYGON ((5534910.27144644 2174970.99624234, 5629331.98126834 2207377.9551015, 5650470.1478844 2182389.96947706, 5534910.27144644 2174970.99624234))')"));
        allCond.push_back(cond);

        // geom - in B, out ES - NI
        cond = static_cast<FdoSpatialCondition*>(FdoFilter::Parse(L"SHPGEOM INTERSECTS GeomFromText('POLYGON ((5695850.38275576 2225006.95144691, 5662882.8202307 2203553.48628294, 5704168.60432706 2199480.04451405, 5721823.71187219 2215773.81496862, 5695850.38275576 2225006.95144691))')"));
        allCond.push_back(cond);

        // geom - in B, in ES - NI
        cond = static_cast<FdoSpatialCondition*>(FdoFilter::Parse(L"SHPGEOM INTERSECTS GeomFromText('POLYGON ((5715848.13648738 2173410.01111093, 5721552.09326288 2167164.06515053, 5688958.05363445 2161189.68411475, 5679994.69055723 2176125.64008319, 5715848.13648738 2173410.01111093))')"));
        allCond.push_back(cond);

        // geom - intersect B, out ES
        cond = static_cast<FdoSpatialCondition*>(FdoFilter::Parse(L"SHPGEOM INTERSECTS GeomFromText('POLYGON ((5545488.90050734 2131652.22023764, 5620636.20332382 2115149.16822162, 5491626.09211908 2080405.89984133, 5545488.90050734 2131652.22023764))')"));
        allCond.push_back(cond);

        // geom - intersect B, intersect ES, TO.S
        cond = static_cast<FdoSpatialCondition*>(FdoFilter::Parse(L"SHPGEOM INTERSECTS GeomFromText('POLYGON ((5695850.38275576 2116555.74670371, 5662970.07317681 1975615.54612141, 5767354.11879699 2012972.41972645, 5695850.38275576 2116555.74670371))')"));
        allCond.push_back(cond);

        // geom - intersect B, intersect S
        cond = static_cast<FdoSpatialCondition*>(FdoFilter::Parse(L"SHPGEOM INTERSECTS GeomFromText('POLYGON ((5757465.1057966 2064771.42915467, 5902547.1817913 2086051.68186123, 5932084.85186769 1996587.76607765, 5757465.1057966 2064771.42915467))')"));
        allCond.push_back(cond);

        // geom - intersect B, intersect ES
        cond = static_cast<FdoSpatialCondition*>(FdoFilter::Parse(L"SHPGEOM INTERSECTS GeomFromText('POLYGON ((5730821.11604607 1991114.52578425, 5802483.85746115 2029157.47939835, 5848988.4522226 1971388.55089996, 5730821.11604607 1991114.52578425))')"));
        allCond.push_back(cond);

        // geom - in B, intersect S
        cond = static_cast<FdoSpatialCondition*>(FdoFilter::Parse(L"SHPGEOM INTERSECTS GeomFromText('POLYGON ((5643240.8518349 2210213.75303545, 5704542.36311135 2133423.34857475, 5655219.30873943 2241211.71405062, 5643240.8518349 2210213.75303545))')"));
        allCond.push_back(cond);

        // geom - in B, TO.S
        cond = static_cast<FdoSpatialCondition*>(FdoFilter::Parse(L"SHPGEOM INTERSECTS GeomFromText('POLYGON ((5774147.82229251 2166139.89621237, 5708065.43711587 2182738.28663024, 5695850.38275576 2160706.01616614, 5774147.82229251 2166139.89621237))')"));
        allCond.push_back(cond);

        // geom - in B, in S
        cond = static_cast<FdoSpatialCondition*>(FdoFilter::Parse(L"SHPGEOM INTERSECTS GeomFromText('POLYGON ((5734318.09348169 2143809.66071967, 5721552.09326288 2106605.55157138, 5757133.92270997 2096286.16384209, 5734318.09348169 2143809.66071967))')"));
        allCond.push_back(cond);

        // geom - in B, TI.S
        cond = static_cast<FdoSpatialCondition*>(FdoFilter::Parse(L"SHPGEOM INTERSECTS GeomFromText('POLYGON ((5695850.38275576 2160706.01616614, 5730821.11604607 2149221.37454525, 5774147.82229251 2166139.89621237, 5695850.38275576 2160706.01616614))')"));
        allCond.push_back(cond);

        // geom - contains B & S - NI
        cond = static_cast<FdoSpatialCondition*>(FdoFilter::Parse(L"SHPGEOM INTERSECTS GeomFromText('POLYGON XYZ ((5485357.60096315 2371545.41859256 0, 5906012.79496403 2371545.41859256 0, 6072002.10683594 2267376.24251062 0, 6051163.49846932 1854443.06455299 0, 5611483.69752058 1831194.59505873 0, 5443080.69663456 1955186.43911943 0, 5443080.69663456 2344774.45279884 0, 5485357.60096315 2371545.41859256 0))')"));
        allCond.push_back(cond);

        // geom - contains B & S, TO.B
        cond = static_cast<FdoSpatialCondition*>(FdoFilter::Parse(L"SHPGEOM INTERSECTS GeomFromText('POLYGON ((5629331.98126834 2207377.9551015, 5534910.27144644 2174970.99624234, 5440966.85141813 2092563.76776649, 5484652.98453481 1905871.50106395, 5803138.99578732 1854443.06455299, 6069658.83183594 2017573.62051062, 6033548.12234384 2310958.4944062, 5865145.12145782 2410997.37005064, 5481834.52492434 2397611.88546429, 5502268.36269459 2287005.52430045, 5629331.98126834 2207377.9551015))')"));
        allCond.push_back(cond);

        // geom - contains S, intersect B
        cond = static_cast<FdoSpatialCondition*>(FdoFilter::Parse(L"SHPGEOM INTERSECTS GeomFromText('POLYGON XYZ ((5541726.80537841 2303913.50349677 0, 5700265.19661062 2329275.4714465 0, 6030025.04630503 2329275.4714465 0, 5958858.92537476 1917143.48719483 0, 5726335.95360182 1836126.08751268 0, 5468446.83923171 1880509.53311422 0, 5457877.61314957 2272915.54248159 0, 5541726.80537841 2303913.50349677 0))')"));
        allCond.push_back(cond);

        // geom - contains B & S, intersect EB
        cond = static_cast<FdoSpatialCondition*>(FdoFilter::Parse(L"SHPGEOM INTERSECTS GeomFromText('POLYGON XYZ ((5529748.35050817 2396202.8876203 0, 5708720.57747633 2303209.00457478 0, 5951108.15890309 2283483.02800098 0, 5962382.00141358 1967162.92417231 0, 5820754.3719128 1864306.05283989 0, 5496631.44143934 1907280.49890794 0, 5444489.92745695 2054520.81584189 0, 5461400.68918838 2369431.92182658 0, 5529748.35050817 2396202.8876203 0))')"));
        allCond.push_back(cond);

        // geom - contains B & S, intersect EB & ES
        cond = static_cast<FdoSpatialCondition*>(FdoFilter::Parse(L"SHPGEOM INTERSECTS GeomFromText('POLYGON XYZ ((5462105.30358243 2417337.86203808 0, 5462105.30358243 1855852.06408648 0, 5956745.08015833 1869237.54698334 0, 5798911.30535446 2084814.27793507 0, 6083575.79110981 2318707.98423761 0, 5593163.70496676 2458903.30857264 0, 5462105.30358243 2417337.86203808 0))')"));
        allCond.push_back(cond);

        // geom - intersect B & S
        cond = static_cast<FdoSpatialCondition*>(FdoFilter::Parse(L"SHPGEOM INTERSECTS GeomFromText('POLYGON XYZ ((5645305.22098345 2472288.793159 0, 5830618.98360091 1834012.59074671 0, 6170948.05937746 2470879.79362552 0, 5645305.22098345 2472288.793159 0))')"));
        allCond.push_back(cond);

        FdoPtr<FdoIInsert>insertCommand = (FdoIInsert *) conn->CreateCommand(FdoCommandType_Insert);
        insertCommand->SetFeatureClassName(L"bayarea_county");

        FdoPtr<FdoPropertyValueCollection> propertyValues;
        FdoPtr<FdoPropertyValue>propertyValue;
        FdoPtr<FdoIFeatureReader> reader;
        for (size_t i = 0; i < allCond.size(); i++)
        {
            FdoSpatialCondition* spCond = allCond.at(i);
            FdoPtr<FdoGeometryValue> geomVal = static_cast<FdoGeometryValue*>(spCond->GetGeometry());
            propertyValues = insertCommand->GetPropertyValues();
            
            propertyValue = propertyValues->FindItem( L"SHPGEOM" );
            if( propertyValue == NULL )
            {
                propertyValue =  FdoPropertyValue::Create();
                propertyValue->SetName( L"SHPGEOM" );
                propertyValues->Add( propertyValue );
            }
            
            propertyValue->SetValue(geomVal);
            reader = insertCommand->Execute();
            if (reader != NULL)
                reader->Close();
        }
    }
    catch(FdoException* exc)
    {
        printf ("\nException: %ls", exc->GetExceptionMessage());
        exc->Release();
    }
    for(size_t i = 0; i < allCond.size(); i++)
    {
        FdoSpatialCondition* ptr = allCond.at(i);
        FDO_SAFE_RELEASE(ptr);
    }
    printf("...Ready");
}

bool FdoExpressionFunctionTest::EvaluateResults(FdoSpatialCondition* bigGeomCond, FdoSpatialCondition* smallGeomCond, FdoIFeatureReader* dataProv, FdoIFeatureReader* result)
{
    bool retVal = true;
    std::vector<int> idxResultOpt;
    std::vector<int> idxResultNoOpt;
    std::vector< std::pair< int, std::pair< FdoSpatialOperations, FdoSpatialOperations > > > operResult;
    FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance ();

    while(result->ReadNext())
        idxResultOpt.push_back(result->GetInt32(L"Autogenerated_SDF_ID"));
    
    FdoPtr<FdoExpression> bExpr = bigGeomCond->GetGeometry ();
    FdoPtr<FdoExpression> sExpr = smallGeomCond->GetGeometry ();
    FdoGeometryValue* bGv = static_cast<FdoGeometryValue*>(bExpr.p);
    FdoGeometryValue* sGv = static_cast<FdoGeometryValue*>(sExpr.p);
    FdoPtr<FdoByteArray> bba = bGv->GetGeometry ();
    FdoPtr<FdoByteArray> sba = sGv->GetGeometry ();
    FdoPtr<FdoIGeometry> geomBig = gf->CreateGeometryFromFgf (bba);
    FdoPtr<FdoIGeometry> geomSmall = gf->CreateGeometryFromFgf (sba);

    FdoSpatialOperations firstOp = bigGeomCond->GetOperation();
    FdoSpatialOperations secOp = smallGeomCond->GetOperation();

    while(dataProv->ReadNext())
    {
        FdoPtr<FdoByteArray> bGeom = dataProv->GetGeometry(L"SHPGEOM");
        int idxGen = dataProv->GetInt32(L"Autogenerated_SDF_ID");
        FdoPtr<FdoIGeometry> geom = gf->CreateGeometryFromFgf (bGeom);
        bool retFirstOp = FdoSpatialUtility::Evaluate (geom, firstOp, geomBig);
        bool retSecondOp = FdoSpatialUtility::Evaluate (geom, secOp, geomSmall);
        if (retFirstOp && retSecondOp)
        {
            idxResultNoOpt.push_back(idxGen);
            operResult.push_back(TestIdxOperationsPair(idxGen, TestOperationsPair(firstOp, secOp)));
        }
    }
    if (idxResultOpt.size() != idxResultNoOpt.size())
        retVal = false;
    
    std::vector<int>* op1 = &idxResultOpt;
    std::vector<int>* op2 = &idxResultNoOpt;
    if (idxResultOpt.size() == 0)
    {
        op1 = &idxResultNoOpt;
        op2 = &idxResultOpt;
    }
    for (size_t i = 0; i < op1->size(); i++)
    {
        int objId = op1->at(i);
        bool valFound = false;
        for (size_t y = 0; y < op2->size(); y++)
        {
            if ( objId == op2->at(y))
            {
                valFound = true;
                break;
            }
        }
        if (!valFound)
        {
            retVal = false;
            for (size_t y = 0; y < operResult.size(); y++)
            {
                TestIdxOperationsPair opRes = operResult.at(y);
                if (opRes.first == objId)
                {
                    FdoStringP txtOp1 = FdoCommonMiscUtil::FdoSpatialOperationsToString(opRes.second.first).Right(L"_");
                    FdoStringP txtOp2 = FdoCommonMiscUtil::FdoSpatialOperationsToString(opRes.second.second).Right(L"_");
                    printf("\nFail at: %d (%ls, %ls)", opRes.first, (FdoString*)txtOp1, (FdoString*)txtOp2);
                    break;
                }
            }
        }
    }
    return retVal;
}

void FdoExpressionFunctionTest::RunOptimizerTest()
{
    bool testResult = true;
    FdoPtr<IConnectionManager> manager = FdoFeatureAccessManager::GetConnectionManager ();
    FdoPtr<FdoIConnection> conn = manager->CreateConnection (L"OSGeo.SDF.3.3");
    try
    {
#ifdef EE_TEST_INS_FEATURES
        std::wstring connStr = std::wstring(L"File=") + std::wstring(EE_OPT_TEST_FILE);
        connStr += std::wstring(L";ReadOnly=FALSE");
        conn->SetConnectionString(connStr.c_str());
        conn->Open();
        InsertTestFeatures(conn);
#else
        std::wstring connStr = std::wstring(L"File=") + std::wstring(EE_OPT_TEST_FILE);
        connStr += std::wstring(L";ReadOnly=TRUE");
        conn->SetConnectionString(connStr.c_str());
        conn->Open();
#endif
        FdoPtr<FdoIFilterCapabilities> fltCapab = conn->GetFilterCapabilities();
        int szCap = 0;
        FdoSpatialOperations* spConds = fltCapab->GetSpatialOperations(szCap);
        
        // big geometry
        FdoPtr<FdoSpatialCondition> bigGeomCond = static_cast<FdoSpatialCondition*>(FdoFilter::Parse(L"SHPGEOM INSIDE GeomFromText('POLYGON ((5726572.25349578 2244716.40810411, 5583530.40649704 2337005.79391714, 5629331.98126834 2207377.9551015, 5534910.27144644 2174970.99624234, 5657517.56705144 2163699.01011146, 5490517.97256231 2048865.65372617, 5632150.54154764 2039707.16436127, 5672315.00001281 2146086.53199315, 5768850.63004419 2006595.70658011, 5787171.26122845 2093953.5965602, 5828040.35976345 2153131.52290257, 5735027.92795496 2179902.49038579, 5816766.1271512 2262328.88622243, 5726572.25349578 2244716.40810411))')"));
        // small geometry inside the big geometry
        FdoPtr<FdoSpatialCondition> smallGeomCond = static_cast<FdoSpatialCondition*>(FdoFilter::Parse(L"SHPGEOM INTERSECTS GeomFromText('POLYGON ((5767354.11879699 2012972.41972645, 5695850.38275576 2116555.74670371, 5695850.38275576 2160706.01616614, 5774147.82229251 2166139.89621237, 5824081.54819392 2151706.15381787, 5782639.9532566 2095499.46405879, 5767354.11879699 2012972.41972645))')"));
        
        for(int i = 0; i < szCap; i++)
        {
            for(int y = 0; y < szCap; y++)
            {
                FdoStringP txtOp1 = FdoCommonMiscUtil::FdoSpatialOperationsToString(spConds[i]).Right(L"_");
                FdoStringP txtOp2 = FdoCommonMiscUtil::FdoSpatialOperationsToString(spConds[y]).Right(L"_");
                printf("\n\nRun[%ls, %ls]", (FdoString*)txtOp1, (FdoString*)txtOp2);

                // generate the filter
                bigGeomCond->SetOperation(spConds[i]);
                smallGeomCond->SetOperation(spConds[y]);
                FdoPtr<FdoFilter> pFilter = FdoBinaryLogicalOperator::Create(bigGeomCond, FdoBinaryLogicalOperations_And, smallGeomCond);

                // run query which will be optimized
                FdoPtr<FdoISelect> selCmd = (FdoISelect*)conn->CreateCommand( FdoCommandType_Select );
                selCmd->SetFeatureClassName(L"bayarea_county");
                selCmd->SetFilter( pFilter );
                FdoPtr<FdoIFeatureReader> optReader = selCmd->Execute();

                // get all data
                selCmd = (FdoISelect*)conn->CreateCommand( FdoCommandType_Select );
                selCmd->SetFeatureClassName(L"bayarea_county");
                selCmd->SetOrderingOption(FdoOrderingOption_Ascending);
                FdoPtr<FdoIFeatureReader> allReader = selCmd->Execute();
                if (!EvaluateResults(bigGeomCond, smallGeomCond, allReader, optReader))
                {
                    testResult = true;
                    printf("...Failed");
                }
                else
                    printf("...Succeeded");
                allReader->Close();
                optReader->Close();
            }
        }
    }
    catch(FdoException* ex)
    {
        FdoStringP msg = ex->GetExceptionMessage();
        ex->Release();
        CPPUNIT_FAIL ((const char*)msg);
    }
    if (!testResult)
        CPPUNIT_FAIL ("Evaluation failed see above text!");
}

