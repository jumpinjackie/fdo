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
#include "FdoFilterTest.h"
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

FdoFilterTest::FdoFilterTest ()

// +---------------------------------------------------------------------------
// | The function represents the class constructor.
// +---------------------------------------------------------------------------

{

}  //  FdoFilterTest ()

FdoFilterTest::~FdoFilterTest ()

// +---------------------------------------------------------------------------
// | The function represents the class destructor.
// +---------------------------------------------------------------------------

{
    m_connection = NULL;

}  //  ~FdoFilterTest ()

void FdoFilterTest::setUp ()

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
      printf("                      FILTER UNIT TESTS                    \n");
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
      printf("                      FILTER UNIT TESTS                    \n");
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
      printf("                      FILTER UNIT TESTS                    \n");
      printf(" Status: Failure in test setup                             \n");
      printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<< \n");
      printf("\n");

      throw;

    }  //  catch ...

}  //  setUp ()

void FdoFilterTest::tearDown ()

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
      printf("                      FILTER UNIT TESTS                    \n");
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
      printf("                      FILTER UNIT TESTS                    \n");
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
      printf("                      FILTER UNIT TESTS                    \n");
      printf(" Status: Failure in test tear down                         \n");
      printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<<<<<<<<<< \n");
      printf("\n");

      throw;

    }  //  catch ...

}  //  tearDown ()

void FdoFilterTest::SetProvider ()

// +---------------------------------------------------------------------------
// | The function sets the provider property.
// +---------------------------------------------------------------------------

{

}  //  SetProvider ()


// ----------------------------------------------------------------------------
// --                      Standard Unit Test Functions                      --
// ----------------------------------------------------------------------------

void FdoFilterTest::RunAllFltTests ()

// +---------------------------------------------------------------------------
// | The function controls the execution of all unit tests. This way, the test
// | setup is done only once.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    int error_count = 0;

    // Executing all test functions.

    printf(" >>> ... Testing Nesting \n");
    printf("\n");

    try {

      NestedFilterSQLTest();

    }  //  try ...
    catch ( ... ) {

      error_count++;

    }  //  catch ...

    try {

      MaxORFilterSelectTest();

    }  //  try ...
    catch ( ... ) {

      error_count++;

    }  //  catch ...

    try {

      NegationORFilterSelectTest();

    }  //  try ...
    catch ( ... ) {

      error_count++;

    }  //  catch ...

    try {

        SpatialObjectFilterTest();

    }  //  try ...
    catch ( ... ) {

      error_count++;

    }  //  catch ...

    if (error_count > 0)
        throw FdoException::Create(
                            L">>> Unit tests executed with errors");


}  //  RunAllFltTests ()


// ----------------------------------------------------------------------------
// --                      SPECIFIC UNIT TEST FUNCTIONS                      --
// ----------------------------------------------------------------------------

void FdoFilterTest::MaxORFilterSelectTest ()

// +---------------------------------------------------------------------------
// | The function executes a select-test with a filter that consists of a
// | provider-specific maximum number of filter elements. The aim of the test
// | is to check whether or not such a filter can be executed as this was not
// | always the case before the nested filter issue was addressed.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    FdoInt32                  max_filter_element_number;

    FdoPtr<FdoFilter>         filter;
    FdoPtr<FdoIFeatureReader> feature_reader;

    printf("\n");
    printf("========================================================== \n");
    printf(" Current Unit Test Suite: Maximum OR filter elements       \n");
    printf("========================================================== \n");
    printf("\n");

    // Execute the test cases.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select command with a filter that    \n");
    printf("  uses a single binary logical operator (OR) to combine    \n");
    printf("  a maximum number of filter elements. In this case, it    \n");
    printf("  is expected that the generated filter is not nested. No  \n");
    printf("  exceptions are expected.                                 \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Build the filter.

      max_filter_element_number = GetMaxFilterElementNumber();
      filter = BuildSingleOperationFilter(1,
                                          max_filter_element_number,
                                          FdoBinaryLogicalOperations_Or);

      // As part of the test, the function also checks whether or not the
      // generated SQL statement is nested. In this case, no nesting should
      // be present.

      TranslateFilter(filter, false, false, MAXORFILTERSELECTTEST);

      // Execute the request.

      printf(" >>> Execute the request \n");
      feature_reader = ExecuteSelectCommand(L"flt_c1", filter);
      CheckReader(feature_reader, max_filter_element_number);

      printf(" >>> Test succeeded \n");

    }  //  try ...

    catch (FdoException *exp) {

      printf(" >>> Exception: %ls\n", exp->GetExceptionMessage());
      printf(" >>> Test failed \n");
      throw exp;

    }  //  catch (FdoException *ex) ...

    catch ( ... ) {

      printf(" >>> Test failed for an unknown reason \n");
      throw;

    }  //  catch ( ... ) ...

}  //  MaxORFilterSelectTest ()

void FdoFilterTest::NegationORFilterSelectTest ()

// +---------------------------------------------------------------------------
// | The function executes a select-test with a filter that consists of a list
// | of binary logical operator combined elements where all or some of them are
// | negated. The test verifies that the generated SQL statement is valid.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    FdoPtr<FdoFilter> filter;

    printf("\n");
    printf("========================================================== \n");
    printf(" Current Unit Test Suite: Negation Filter Tests            \n");
    printf("========================================================== \n");
    printf("\n");

    // Execute the test cases.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test creates a filter using a single binary logical  \n");
    printf("  operator (OR) to combine the different filter elements   \n");
    printf("  and negates it. In this case, it is expected that the    \n");
    printf("  generated filter is not nested. No exceptions are ex-    \n");
    printf("  pected.                                                  \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Build the filter.

      filter = FdoFilter::Parse(L"NOT ((id = 1) or (id = 2) or (id = 3))");

      // Translate the filter into a SQL statement and check whether or not
      // nesting is used. In this case, no nesting should be present.

      TranslateFilter(filter, false, false, NEGATIONORFILTERSELECTTEST_1);
      printf(" >>> Test succeeded \n");

    }  //  try ...

    catch (FdoException *exp) {

      printf(" >>> Exception: %ls\n", exp->GetExceptionMessage());
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
    printf("  The test creates a filter using a single binary logical  \n");
    printf("  operator (OR) to combine the different filter elements   \n");
    printf("  where a subset is negated. In this case, it is expected  \n");
    printf("  that the generated filter is not nested. No exceptions   \n");
    printf("  are expected.                                            \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Build the filter.

      filter = FdoFilter::Parse(
                    L"id = 1 or id = 2 or not (id = 3 or id = 4) or id = 5");

      // Translate the filter into a SQL statement and check whether or not
      // nesting is used. In this case, no nesting should be present.

      TranslateFilter(filter, false, false, NEGATIONORFILTERSELECTTEST_2);
      printf(" >>> Test succeeded \n");

    }  //  try ...

    catch (FdoException *exp) {

      printf(" >>> Exception: %ls\n", exp->GetExceptionMessage());
      printf(" >>> Test failed \n");
      throw exp;

    }  //  catch (FdoException *ex) ...

    catch ( ... ) {

      printf(" >>> Test failed for an unknown reason \n");
      throw;

    }  //  catch ( ... ) ...

}  //  NegationORFilterSelectTest ()

void FdoFilterTest::NestedFilterSQLTest ()

// +---------------------------------------------------------------------------
// | The function executes the nested filter SQL tests and checks if nesting of
// | a filter statement is applied only if necessary.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    FdoPtr<FdoFilter> filter;

    printf("\n");
    printf("========================================================== \n");
    printf(" Current Unit Test Suite: Nesting Filters                  \n");
    printf("========================================================== \n");
    printf("\n");

    // Execute the test cases.

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test creates a filter using a single binary logical  \n");
    printf("  operator (OR) to combine the different filter elements.  \n");
    printf("  In this case, it is expected that the generated filter   \n");
    printf("  is not nested. No exceptions are expected.               \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Build the filter.

      filter =
            BuildSingleOperationFilter(1, 10, FdoBinaryLogicalOperations_Or);

      // Translate the filter into a SQL statement and check whether or not
      // nesting is used. In this case, no nesting should be present.

      TranslateFilter(filter, false, false, NESTEDFILTERSQLTEST);
      printf(" >>> Test succeeded \n");

    }  //  try ...

    catch (FdoException *exp) {

      printf(" >>> Exception: %ls\n", exp->GetExceptionMessage());
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
    printf("  The test creates a filter using a single binary logical  \n");
    printf("  operator (AND) to combine different filter elements. In  \n");
    printf("  this case, it is expected that the generated filter is   \n");
    printf("  not nested. No exceptions are expected.                  \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Build the filter.

      filter =
            BuildSingleOperationFilter(1, 10, FdoBinaryLogicalOperations_And);

      // Translate the filter into a SQL statement and check whether or not
      // nesting is used. In this case, no nesting should be present.

      TranslateFilter(filter, false, false, NESTEDFILTERSQLTEST);
      printf(" >>> Test succeeded \n");

    }  //  try ...

    catch (FdoException *exp) {

      printf(" >>> Exception: %ls\n", exp->GetExceptionMessage());
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
    printf("  The test creates a filter using multiple binary logical  \n");
    printf("  operators to combine the different filter elements. In   \n");
    printf("  this case, it is expected that the generated filter is   \n");
    printf("  nested. No exceptions are expected.                      \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Build the filter.

      filter = BuildMixedOperationFilter(1,
                                         10,
                                         FdoBinaryLogicalOperations_And,
                                         FdoBinaryLogicalOperations_Or);

      // Translate the filter into a SQL statement and check whether or not
      // nesting is used. In this case, nesting should be present.

      TranslateFilter(filter, true, false, NESTEDFILTERSQLTEST);
      printf(" >>> Test succeeded \n");

    }  //  try ...

    catch (FdoException *exp) {

      printf(" >>> Exception: %ls\n", exp->GetExceptionMessage());
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
    printf("  The test creates a filter using multiple binary logical  \n");
    printf("  operators to combine the different filter elements. In   \n");
    printf("  this case, it is expected that the generated filter is   \n");
    printf("  nested. No exceptions are expected.                      \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Build the filter.

      filter = BuildMixedOperationFilter(1,
                                         10,
                                         FdoBinaryLogicalOperations_Or,
                                         FdoBinaryLogicalOperations_And);

      // Translate the filter into a SQL statement and check whether or not
      // nesting is used. In this case, nesting should be present.

      TranslateFilter(filter, true, false, NESTEDFILTERSQLTEST);
      printf(" >>> Test succeeded \n");

    }  //  try ...

    catch (FdoException *exp) {

      printf(" >>> Exception: %ls\n", exp->GetExceptionMessage());
      printf(" >>> Test failed \n");
      throw exp;

    }  //  catch (FdoException *ex) ...

    catch ( ... ) {

      printf(" >>> Test failed for an unknown reason \n");
      throw;

    }  //  catch ( ... ) ...

}  //  NestedFilterSQLTest ()

void FdoFilterTest::SpatialObjectFilterTest ()

// +---------------------------------------------------------------------------
// | The function executes tests that check if the filter processor correctly
// | identifies a filter case where a spatial and an object filter are combined
// | with a binary logical operation as this kind of filter has a specific
// | optimization.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    FdoPtr<FdoFilter>         filter,
                              object_filter,
                              spatial_filter,
                              tc4_filter_left,
                              tc4_filter_right;

    FdoPtr<FdoIFeatureReader> feature_reader;

    printf("\n");
    printf("========================================================== \n");
    printf(" Current Unit Test Suite: Spatial/Object Filter Test       \n");
    printf("========================================================== \n");
    printf("\n");

    // Test Setup.

    printf("---------------------------------------------------------- \n");
    printf("Test Setup:                                                \n");
    printf("   Define filters that are commonly used in the tests.     \n"); 
    printf("---------------------------------------------------------- \n");

    try {

      spatial_filter =
            FdoFilter::Parse(
                    L"(id > -180 and id < 180) and (id > -90 and id < 90)");
      object_filter  =
            FdoFilter::Parse(
                        L"id = 1 or id = 2 or id = 3 or id = 4 or id = 5");

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

    printf("---------------------------------------------------------- \n");
    printf("1. Test Case:                                              \n");
    printf("  The test executes a select command with a filter that    \n");
    printf("  is constructed of a spatial and object filter combined   \n");
    printf("  by the binary logical operator AND where the spatial     \n");
    printf("  filter is the left and the object filter the right       \n");
    printf("  operand. The generated SQL statement should not be       \n");
    printf("  nested. However, the two filters should each be grouped  \n");
    printf("  (included in opening and closing brackets). No exception \n");
    printf("  is expected.                                             \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Build the filter.

      filter = FdoFilter::Combine(spatial_filter,
                                  FdoBinaryLogicalOperations_And,
                                  object_filter);

      // As part of the test, the function also checks whether or not the
      // generated SQL statement is nested. In this case, no nesting should
      // be present.

      TranslateFilter(filter, false, true, SPATIALOBJECTFILTERCASE_1);

      // Execute the request.

      printf(" >>> Execute the request \n");
      feature_reader = ExecuteSelectCommand(L"flt_c1", filter);
      CheckReader(feature_reader, 5);

      printf(" >>> Test succeeded \n");

    }  //  try ...

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
    printf("  The test executes a select command with a filter that    \n");
    printf("  is constructed of a spatial and object filter combined   \n");
    printf("  by the binary logical operator AND where the spatial     \n");
    printf("  filter is the right and the object filter the left       \n");
    printf("  operand. The generated SQL statement should not be       \n");
    printf("  nested. However, the two filters should each be grouped  \n");
    printf("  (included in opening and closing brackets). No exception \n");
    printf("  is expected.                                             \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Build the filter.

      filter = FdoFilter::Combine(object_filter,
                                  FdoBinaryLogicalOperations_And,
                                  spatial_filter);

      // As part of the test, the function also checks whether or not the
      // generated SQL statement is nested. In this case, no nesting should
      // be present.

      TranslateFilter(filter, false, true, SPATIALOBJECTFILTERCASE_1);

      // Execute the request.

      printf(" >>> Execute the request \n");
      feature_reader = ExecuteSelectCommand(L"flt_c1", filter);
      CheckReader(feature_reader, 5);

      printf(" >>> Test succeeded \n");

    }  //  try ...

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
    printf("  The test executes a select command with a filter that    \n");
    printf("  is constructed of a spatial and object filter combined   \n");
    printf("  by the binary logical operator OR where the spatial      \n");
    printf("  filter is the right and the object filter the left       \n");
    printf("  operand. The generated SQL statement should be nested    \n");
    printf("  and not grouped (included opening and closing brackets). \n");
    printf("  No exceptions are expected.                              \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Build the filter.

      filter = FdoFilter::Combine(object_filter,
                                  FdoBinaryLogicalOperations_Or,
                                  spatial_filter);

      // As part of the test, the function also checks whether or not the
      // generated SQL statement is nested. In this case, no nesting should
      // be present.

      TranslateFilter(filter, true, false, SPATIALOBJECTFILTERCASE_2);

      // Execute the request.

      printf(" >>> Execute the request \n");
      feature_reader = ExecuteSelectCommand(L"flt_c1", filter);
      CheckReader(feature_reader, 90);

      printf(" >>> Test succeeded \n");

    }  //  try ...

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
    printf("4. Test Case:                                              \n");
    printf("  The test executes a select command with a filter that    \n");
    printf("  consists of two of the spatial/object filters combined   \n");
    printf("  by a binary logical operator AND. The generated SQL      \n");
    printf("  statement should be nested and not grouped (included     \n");
    printf("  opening and closing brackets). No exceptions are         \n");
    printf("  expected.                                                \n");
    printf("---------------------------------------------------------- \n");

    try {

      // Build the filter.

      tc4_filter_left  = FdoFilter::Combine(object_filter,
                                            FdoBinaryLogicalOperations_And,
                                            spatial_filter);
      tc4_filter_right = FdoFilter::Combine(object_filter,
                                            FdoBinaryLogicalOperations_And,
                                            spatial_filter);

      filter = FdoFilter::Combine(tc4_filter_left,
                                  FdoBinaryLogicalOperations_And,
                                  tc4_filter_right);

      // As part of the test, the function also checks whether or not the
      // generated SQL statement is nested. In this case, no nesting should
      // be present.

      TranslateFilter(filter, true, false, SPATIALOBJECTFILTERCASE_2);

      // Execute the request.

      printf(" >>> Execute the request \n");
      feature_reader = ExecuteSelectCommand(L"flt_c1", filter);
      CheckReader(feature_reader, 5);

      printf(" >>> Test succeeded \n");

    }  //  try ...

    catch (FdoException *exp) {

      printf(" >>> Exception: %ls\n", exp->GetExceptionMessage());
      printf(" >>> Test failed \n");
      throw exp;

    }  //  catch (FdoException *ex) ...

    catch ( ... ) {

      printf(" >>> Test failed for an unknown reason \n");
      throw;

    }  //  catch ( ... ) ...

}  //  SpatialObjectFilterTest ()


// ----------------------------------------------------------------------------
// --                      General Supporting Functions                      --
// ----------------------------------------------------------------------------

FdoFilter *FdoFilterTest::BuildMixedOperationFilter (
                                    FdoInt32                   start_index,
                                    FdoInt32                   end_index,
                                    FdoBinaryLogicalOperations first_op,
                                    FdoBinaryLogicalOperations second_op)

// +---------------------------------------------------------------------------
// | The function generates a filter where each of the filter elements are
// | connected by different binary logical operations. The generated filter
// | is returned to the calling routine.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoInt32                       i;

    FdoPtr<FdoFilter>              filter;
    FdoPtr<FdoComparisonCondition> filter_element;

    // Construct the filter using the given start and end index value.

    printf(" >>> Building filter for the test \n");

    for (i = start_index; i < (end_index + 1); i++) {

      if (i == start_index)      
          filter_element =
            FdoComparisonCondition::Create(
                    FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"id_str")),
                    FdoComparisonOperations_EqualTo,
                    FdoPtr<FdoDataValue>(FdoDataValue::Create(L"testing")));
      else
        filter_element =
            FdoComparisonCondition::Create(
                        FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"id")),
                        FdoComparisonOperations_EqualTo,
                        FdoPtr<FdoDataValue>(FdoDataValue::Create(i)));

      if (i == start_index)
          filter = filter_element;
      else
        if (i == (start_index + 1))
            filter = FdoFilter::Combine(filter, first_op, filter_element);
        else
          filter = FdoFilter::Combine(filter, second_op, filter_element);

    }  //  for (...

    return FDO_SAFE_ADDREF(filter.p);

}  //  BuildMixedOperationFilter ()

FdoFilter *FdoFilterTest::BuildSingleOperationFilter (
                                        FdoInt32 start_index,
                                        FdoInt32 end_index,
                                        FdoBinaryLogicalOperations operation)

// +---------------------------------------------------------------------------
// | The function generates a filter where each of the filter elements are
// | connected by the given operation. The generated filter is returned to
// | the calling routine.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoInt32                       i;

    FdoPtr<FdoFilter>              filter;
    FdoPtr<FdoComparisonCondition> filter_element;

    // Construct the filter using the given start and end index value.

    printf(" >>> Building filter for the test \n");

    for (i = start_index; i < (end_index + 1); i++) {

      filter_element =
            FdoComparisonCondition::Create(
                        FdoPtr<FdoIdentifier>(FdoIdentifier::Create(L"id")),
                        FdoComparisonOperations_EqualTo,
                        FdoPtr<FdoDataValue>(FdoDataValue::Create(i)));

      if (i == start_index)
          filter = filter_element;
      else
        filter = FdoFilter::Combine(filter, operation, filter_element);


    }  //  for (...

    return FDO_SAFE_ADDREF(filter.p);

}  //  BuildSingleOperationFilter ()

bool FdoFilterTest::CheckForGrouping (FdoStringP  sql_statement,
                                      UnitTestIds unit_test_id)

// +---------------------------------------------------------------------------
// | The function checks whether or not two filter elements linked by a binary
// | logical operator are grouped (surrounded by brackets).
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    bool       op_processed      = false,
               invalid_operator  = true;

    FdoInt32   i,
               op_pos            = 0,
               bracket_count     = 0,
               sql_string_length = (FdoInt32)sql_statement.GetLength();

    FdoStringP curr_char,
               curr_word;

    // Navigate through the provided SQL string and count the brackets. The
    // string contains grouping if a structure like this is found:
    //
    // ( <filter values> ) OPERATOR ( <filter values> )
    //
    // where <filter values> consists of any number of terms combined by binary
    // logical operators of any kind. The check counts the bracket pairs and
    // checks for the presence of the expected structure.

    for (i = 1; i < (sql_string_length - 1); i++) {

      curr_char = sql_statement.Mid(i, 1);
      if ((curr_char.ICompare(L"(") != 0) &&
          (curr_char.ICompare(L")") != 0)    )
          continue;

      if (curr_char.ICompare(L"(") == 0)
          bracket_count++;

      if (curr_char.ICompare(L")") == 0)
          bracket_count--;

      // If the bracket count is 0 and the operator has not yet been processed,
      // then the left operand has been validated. Next, get the operand and
      // check it the one that is expected.

      if ((bracket_count == 0) && (!op_processed)) {

          op_processed = true;
          i++;
          while ((curr_char = sql_statement.Mid(i, 1)).ICompare(L" ") == 0)
            i++;
          op_pos = i;
          while ((curr_char = sql_statement.Mid(i+1, 1)).ICompare(L" ") != 0)
            i++;
          curr_word = sql_statement.Mid(op_pos, (i-op_pos+1));
          i++;

          switch (unit_test_id) {

            case SPATIALOBJECTFILTERCASE_1:
              invalid_operator = (curr_word.ICompare(L"AND") != 0);
              break;

            default:
              invalid_operator = true;
              break;

          }  //  switch ...

      }  //  if ((bracket_count == 0) && ( ...

    }  //  for (i = 1; i < (sql_string_length - 1); i++) ...

    if (invalid_operator)
        throw FdoException::Create(
                            L"Unexpected operator in generated SQL filter");

    return (bracket_count == 0);

}  //  CheckForGrouping ()

bool FdoFilterTest::CheckForNesting (FdoStringP sql_statement)

// +---------------------------------------------------------------------------
// | The function checks whether or not the given SQL string contains nested
// | elements and returns the findings of the check back to the calling pro-
// | cedure.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    FdoInt32   i,
               bracket_count     = 0,
               sql_string_length = (FdoInt32)sql_statement.GetLength();

    FdoStringP curr_char;

    // The following navigates through the provided SQL statement and checks
    // whether or not nesting is present. This is done by looking at the
    // beginning of the string as it would look different if it is nested/not
    // nested (example):
    //
    // NESTED
    //  (((id = 1) OR (id = 2)) OR (id = 3))
    // NOT NESTED
    //  (id = 1) OR (id = 2) OR (id = 3)
    //
    // Depending on the string, the function determines whether or not nesting
    // is present and returns this information back to the caller.

    for (i = 1; i < (sql_string_length - 1); i++) {

      curr_char = sql_statement.Mid(i, 1);
      if (curr_char.ICompare(L" ") == 0)
          continue;

      if ((bracket_count == 1) && (curr_char.ICompare(L"(") != 0))
          break;

      if (curr_char.ICompare(L"(") == 0)
          bracket_count++;

      if (bracket_count > 2)
          break;

    }  //  for (i = 1; i < (sql_string_length - 1); i++) ...

    return (bracket_count > 2);

}  //  CheckForNesting ()

void FdoFilterTest::CheckReader (FdoIFeatureReader *reader,
                                 FdoInt32          expected_count)

// +---------------------------------------------------------------------------
// | The function checks whether or not the provided reader contains the ex-
// | pected data and throws an exception if this is not the case.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    FdoInt32 data_count = 0;

    // Navigate through the reader and perform the necessary checks.

    printf(" >>> ... Cross check result \n");

    while (reader->ReadNext())
      data_count++;

    // Close the reader.

    reader->Close();

    // Issue an exception if the expected result is not met.

    if (data_count != expected_count)
        throw FdoException::Create(
                        L"Unexpected result(s) when checking returned data");
    else
      printf(" >>> ...... All expected data found\n");

}  //  CheckReader ()

FdoIFeatureReader *FdoFilterTest::ExecuteSelectCommand (FdoString *class_name,
                                                        FdoFilter *filter)

// +---------------------------------------------------------------------------
// | The function executes a select-command using the provided data and returns
// | the generated reader back to the calling routine.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    FdoPtr<FdoISelect>              select_cmd;
    FdoPtr<FdoIdentifier>           id_prop;
    FdoPtr<FdoIdentifierCollection> id_col;

    // Create a select function and set the class name and filter.

    printf(" >>> ... Command setup \n");

    select_cmd =
            (FdoISelect*)m_connection->CreateCommand(FdoCommandType_Select);
    select_cmd->SetFeatureClassName(class_name);
    if (filter != NULL)
        select_cmd->SetFilter(filter);
    id_col = select_cmd->GetPropertyNames();
    id_prop = FdoIdentifier::Create(L"id");
    id_col->Add(id_prop);

    // Execute the request and return the data reader back to the calling pro-
    // cedure.

    printf(" >>> ... Execute \n");

    return (select_cmd->Execute());

}  //  ExecuteSelectCommand ()

FdoInt32 FdoFilterTest::GetMaxFilterElementNumber ()

// +---------------------------------------------------------------------------
// | The function returns the maximum number of filter elements that should be
// | combined in a filter.
// +---------------------------------------------------------------------------

{

    return 800;

}  //  GetMaxFilterElementNumber ()

void FdoFilterTest::TranslateFilter (FdoFilter   *filter,
                                     bool        is_nesting_expected,
                                     bool        is_grouping_expected,
                                     UnitTestIds unit_test_id)

// +---------------------------------------------------------------------------
// | The function requests the SQL representation of the given filter and
// | checks the result whether or not nesting or grouping has been used. If
// | nesting or grouping has been used where none is expected or nesting or
// | grouping has not been used when expected an exception is issued.
// +---------------------------------------------------------------------------

{

    // THE FUNCTION HAS TO BE IMPLEMENTED BY THE PROVIDER SPECIFIC UNIT TEST
    // IMPLEMENTATION. TO ENSURE THIS HAPPENS THE DEFAULT IMPLEMENTATION WILL
    // THROW AN EXCEPTION IF CALLED.

    throw FdoException::Create(L"Missing implementation of TranslateFilter");

}  //  TranslateFilter ()


// ----------------------------------------------------------------------------
// --                    Test Environment Setup Functions                    --
// ----------------------------------------------------------------------------

void FdoFilterTest::AddFeature (FdoIConnection *current_connection,
                                FdoString      *class_name,
                                int            index)

// +---------------------------------------------------------------------------
// | The function adds a new object for the specified class. The values being
// | added are predefined based on the predefined schema.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    double                     coordinate_buffer[7];

    FdoInt32                   i                    = 0,
                               dimension_type,
                               dimensionality       = 3;

    FdoIInsert                 *insert_command      = NULL;

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

      coordinate_buffer[i++] = 100.0 + index;
      coordinate_buffer[i++] = 100.0 + index;
      if (dimensionality == 3)
          coordinate_buffer[i++] = 0;
      coordinate_buffer[i++] = 101.0 + index;
      coordinate_buffer[i++] = 101.0 + index;
      if (dimensionality == 3)
          coordinate_buffer[i++] = 0;

      dimension_type = (dimensionality == 3)
                     ? FdoDimensionality_XY|FdoDimensionality_Z
                     : FdoDimensionality_XY;

      geometry_factory = FdoFgfGeometryFactory::GetInstance();
      line_str         = geometry_factory->CreateLineString(dimension_type,
                                                            i, 
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

      data_value     = FdoDataValue::Create(L"testing");
      property_value = AddNewProperty(property_values, L"id_str");
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

void FdoFilterTest::AddTestSchema (FdoIConnection *current_connection,
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

	  schema  = FdoFeatureSchema::Create(schema_name, L"Filter Tests");
      classes = schema->GetClasses();

      // Create the test classes for the unit test and add them to the schema.

      printf(" >>> ...... adding class flt_c1 \n");
      schema_feature_class = CreateFdoFeatureClass(L"flt_c1");
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

void FdoFilterTest::CloseConnection ()

// +---------------------------------------------------------------------------
// | The function closes all open connections to the data store used in the
// | executed unit tests.
// +---------------------------------------------------------------------------

{

    if (m_connection != NULL)
        m_connection->Close();

}  //  CloseConnection ()

void FdoFilterTest::Connect ()

// +---------------------------------------------------------------------------
// | The function establishes the connection to the data store used in the
// | executed test suites.
// +---------------------------------------------------------------------------

{

      // Establish the connection.

      m_connection = UnitTestUtil::GetConnection(DATASTORE_EXTENSION);

}  //  Connect ()

FdoDataPropertyDefinition *FdoFilterTest::CreateDataProperty (
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

FdoFeatureClass *FdoFilterTest::CreateFdoFeatureClass (FdoString *class_name)

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
           CreateDataProperty(L"id", FdoDataType_Int32, 0, 0, true);
	  data_property_definitions->Add(data_property_definition);
	  FDO_SAFE_RELEASE(data_property_definition);

      data_property_definition =
           CreateDataProperty(L"id_str", FdoDataType_String, 30, 0, true);
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
            *FdoFilterTest::CreateGeometricProperty (FdoString *property_name)

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

void FdoFilterTest::CreateTestDataStore (FdoString *datastore_extension)

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

FdoInt32 FdoFilterTest::GetDimensionality ()

// +---------------------------------------------------------------------------
// | The function returns the dimensionality the provider supports.
// +---------------------------------------------------------------------------

{

    return (3);

}  //  GetDimensionality ()   

void FdoFilterTest::DropTestDataStore (FdoString *datastore_extension)

// +---------------------------------------------------------------------------
// | The function drops the test data store.
// +---------------------------------------------------------------------------

{

    printf(" >>> ... dropping the data store with extension '%ls' \n",
           datastore_extension);
    UnitTestUtil::DropDb(FdoStringP(datastore_extension));

}  //  DropTestDataStore ()

void FdoFilterTest::SetupUnitTestEnvironment (
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
      AddTestSchema(current_connection, L"FltTests");

      // Start a transaction to cover the following operations.

      if ((fdo_i_transaction = current_connection->BeginTransaction()) == NULL)
          throw FdoException::Create(
                  L"Unable to start transaction in SetupUnitTestEnvironment");

      transaction_started = true;

      // Load the features used in the unit tests.
      //   > The spatial features.

      printf(" >>> ... adding data for feature classes \n");
      printf(" >>> ...... for class flt_c1 \n");
      for (int i = 0; i < 8000; i++) {

        AddFeature(current_connection, L"flt_c1", i);
        if ((i > 0) && ((i % 1000) == 0))
            printf(" >>> ......... %d (of 8000) \n", i);

      }  //  for (int i = 0; i < 8000; i++) ...

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

