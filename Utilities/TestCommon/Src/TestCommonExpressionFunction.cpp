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
#include "TestCommonExpressionFunction.h"
#include <math.h>
#include <Spatial/SpatialStd.h>
#include <Spatial/SpatialUtility.h>
#include <FdoCommonMiscUtil.h>
#include <limits>       // For quiet_NaN()
using namespace std;

typedef std::pair< FdoSpatialOperations, FdoSpatialOperations > TestOperationsPair;
typedef std::pair< int, TestOperationsPair > TestIdxOperationsPair;


// ----------------------------------------------------------------------------
// --                            Helper functions                            --
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// --                          Test Setup Functions                          --
// ----------------------------------------------------------------------------

TestCommonExpressionFunction::TestCommonExpressionFunction ()

// +---------------------------------------------------------------------------
// | The function represents the class constructor.
// +---------------------------------------------------------------------------

{

}  //  TestCommonExpressionFunction ()

TestCommonExpressionFunction::~TestCommonExpressionFunction ()

// +---------------------------------------------------------------------------
// | The function represents the class destructor.
// +---------------------------------------------------------------------------

{
    m_connection = NULL;

}  //  ~TestCommonExpressionFunction ()

void TestCommonExpressionFunction::setUp ()

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

void TestCommonExpressionFunction::tearDown ()

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

void TestCommonExpressionFunction::RunAllExpFctTests ()

// +---------------------------------------------------------------------------
// | The function controls the execution of all unit tests. This way, the test
// | setup is done only once.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local variables.

    bool error_found = false;

    // Testing the aggregate functions.

    printf(" >>> ... Testing Aggregate Functions \n");
    printf("\n");
    try {

        TestAvgFunction();

    }  //  try ...

    catch ( ... ) {

      error_found = true;

    }  //  catch ...

    try {

      TestCountFunction();

    }  //  try ...

    catch ( ... ) {

      error_found = true;

    }  //  catch ...

    try {

      TestMaxFunction();

    }  //  try ...

    catch ( ... ) {

      error_found = true;

    }  //  catch ...

    try {

      TestMedianFunction();

    }  //  try ...

    catch ( ... ) {

      error_found = true;

    }  //  catch ...

    try {

      TestMinFunction();

    }  //  try ...

    catch ( ... ) {

      error_found = true;

    }  //  catch ...

    try {

      TestStddevFunction();

    }  //  try ...

    catch ( ... ) {

      error_found = true;

    }  //  catch ...

    try {

      TestSumFunction();

    }  //  try ...

    catch ( ... ) {

      error_found = true;

    }  //  catch ...

    // Testing the conversion functions.

    printf("\n");
    printf("\n");
    printf(" >>> ... Testing Conversion Functions \n");
    printf("\n");

    try {

      TestNullValueFunction();

    }  //  try ...

    catch ( ... ) {

      error_found = true;

    }  //  catch ...

    try {

      TestToDateFunction();

    }  //  try ...

    catch ( ... ) {

      error_found = true;

    }  //  catch ...

    try {

      TestToDoubleFunction();

    }  //  try ...

    catch ( ... ) {

      error_found = true;

    }  //  catch ...

    try {

      TestToFloatFunction();

    }  //  try ...

    catch ( ... ) {

      error_found = true;

    }  //  catch ...

    try {

      TestToInt32Function();

    }  //  try ...

    catch ( ... ) {

      error_found = true;

    }  //  catch ...

    try {

      TestToInt64Function();

    }  //  try ...

    catch ( ... ) {

      error_found = true;

    }  //  catch ...

    try {

      TestToStringFunction();

    }  //  try ...

    catch ( ... ) {

      error_found = true;

    }  //  catch ...

    // Testing the date functions.

    printf("\n");
    printf("\n");
    printf(" >>> ... Testing Date Functions \n");
    printf("\n");

    try {

      TestAddMonthsFunction();

    }  //  try ...

    catch ( ... ) {

      error_found = true;

    }  //  catch ...

    try {

      TestCurrentDateFunction();

    }  //  try ...

    catch ( ... ) {

      error_found = true;

    }  //  catch ...

    try {

      TestExtractFunction();

    }  //  try ...

    catch ( ... ) {

      error_found = true;

    }  //  catch ...

    try {

      TestExtractToDoubleFunction();

    }  //  try ...

    catch ( ... ) {

      error_found = true;

    }  //  catch ...

    try {

      TestExtractToIntFunction();

    }  //  try ...

    catch ( ... ) {

      error_found = true;

    }  //  catch ...

    try {

      TestMonthsBetweenFunction();

    }  //  try ...

    catch ( ... ) {

      error_found = true;

    }  //  catch ...

    // Testing the mathematical functions.

    printf("\n");
    printf("\n");
    printf(" >>> ... Testing Mathematical Functions \n");
    printf("\n");

    try {

      TestAbsFunction();

    }  //  try ...

    catch ( ... ) {

      error_found = true;

    }  //  catch ...

    try {

      TestAcosFunction();

    }  //  try ...

    catch ( ... ) {

      error_found = true;

    }  //  catch ...

    try {

      TestAsinFunction();

    }  //  try ...

    catch ( ... ) {

      error_found = true;

    }  //  catch ...

    try {

      TestAtanFunction();

    }  //  try ...

    catch ( ... ) {

      error_found = true;

    }  //  catch ...

    try {

      TestAtan2Function();

    }  //  try ...

    catch ( ... ) {

      error_found = true;

    }  //  catch ...

    try {

      TestCosFunction();

    }  //  try ...

    catch ( ... ) {

      error_found = true;

    }  //  catch ...

    try {

      TestExpFunction();

    }  //  try ...

    catch ( ... ) {

      error_found = true;

    }  //  catch ...

    try {

      TestLnFunction();

    }  //  try ...

    catch ( ... ) {

      error_found = true;

    }  //  catch ...

    try {

      TestLogFunction();

    }  //  try ...

    catch ( ... ) {

      error_found = true;

    }  //  catch ...

    try {

      TestModFunction();

    }  //  try ...

    catch ( ... ) {

      error_found = true;

    }  //  catch ...

    try {

      TestPowerFunction();

    }  //  try ...

    catch ( ... ) {

      error_found = true;

    }  //  catch ...

    try {

      TestRemainderFunction();

    }  //  try ...

    catch ( ... ) {

      error_found = true;

    }  //  catch ...

    try {

      TestSinFunction();

    }  //  try ...

    catch ( ... ) {

      error_found = true;

    }  //  catch ...

    try {

      TestSqrtFunction();

    }  //  try ...

    catch ( ... ) {

      error_found = true;

    }  //  catch ...

    try {

      TestTanFunction();

    }  //  try ...

    catch ( ... ) {

      error_found = true;

    }  //  catch ...

    // Testing the numeric functions.

    printf("\n");
    printf("\n");
    printf(" >>> ... Testing Numeric Functions \n");
    printf("\n");

    try {

      TestCeilFunction();

    }  //  try ...

    catch ( ... ) {

      error_found = true;

    }  //  catch ...

    try {

      TestFloorFunction();

    }  //  try ...

    catch ( ... ) {

      error_found = true;

    }  //  catch ...

    try {

      TestRoundFunction();

    }  //  try ...

    catch ( ... ) {

      error_found = true;

    }  //  catch ...

    try {

      TestSignFunction();

    }  //  try ...

    catch ( ... ) {

      error_found = true;

    }  //  catch ...

    try {

      TestTruncFunction();

    }  //  try ...

    catch ( ... ) {

      error_found = true;

    }  //  catch ...

    // Testing the string functions.

    printf("\n");
    printf("\n");
    printf(" >>> ... Testing String Functions \n");
    printf("\n");

    try {

      TestConcatFunction();

    }  //  try ...

    catch ( ... ) {

      error_found = true;

    }  //  catch ...

    try {

      TestInstrFunction();

    }  //  try ...

    catch ( ... ) {

      error_found = true;

    }  //  catch ...

    try {

      TestLengthFunction();

    }  //  try ...

    catch ( ... ) {

      error_found = true;

    }  //  catch ...

    try {

      TestLowerFunction();

    }  //  try ...

    catch ( ... ) {

      error_found = true;

    }  //  catch ...

    try {

      TestLpadFunction();

    }  //  try ...

    catch ( ... ) {

      error_found = true;

    }  //  catch ...

    try {

      TestLtrimFunction();

    }  //  try ...

    catch ( ... ) {

      error_found = true;

    }  //  catch ...

    try {

      TestRpadFunction();

    }  //  try ...

    catch ( ... ) {

      error_found = true;

    }  //  catch ...

    try {

      TestRtrimFunction();

    }  //  try ...

    catch ( ... ) {

      error_found = true;

    }  //  catch ...

    try {

      TestSoundexFunction();

    }  //  try ...

    catch ( ... ) {

      error_found = true;

    }  //  catch ...

    try {

      TestSubstrFunction();

    }  //  try ...

    catch ( ... ) {

      error_found = true;

    }  //  catch ...

    try {

      TestTranslateFunction();

    }  //  try ...

    catch ( ... ) {

      error_found = true;

    }  //  catch ...

    try {

      TestTrimFunction();

    }  //  try ...

    catch ( ... ) {

      error_found = true;

    }  //  catch ...

    try {

      TestUpperFunction();

    }  //  try ...

    catch ( ... ) {

      error_found = true;

    }  //  catch ...

    // Executing the combination tests.

    try {

      NumberToStringComparison();

    }  //  try ...

    catch ( ... ) {

      error_found = true;

    }  //  catch ...

    try {

      RoundNumberToStringComparison();

    }  //  try ...

    catch ( ... ) {

      error_found = true;

    }  //  catch ...

    // Executing the geometry functions.

    printf("\n");
    printf("\n");
    printf(" >>> ... Testing Geometry Functions \n");
    printf("\n");

    try {

      TestXYZMFunction();

    }  //  try ...

    catch ( ... ) {

      error_found = true;

    }  //  catch ...

    if (error_found)
        throw FdoException::Create(L"Unit tests executed with errors");

}  //  RunAllExpFctTests ()

void TestCommonExpressionFunction::TestAvgFunction ()
{
}

void TestCommonExpressionFunction::TestCountFunction ()
{
}

void TestCommonExpressionFunction::TestMaxFunction ()
{
}

void TestCommonExpressionFunction::TestMedianFunction ()
{
}

void TestCommonExpressionFunction::TestMinFunction ()
{
}

void TestCommonExpressionFunction::TestStddevFunction ()
{
}

void TestCommonExpressionFunction::TestSumFunction ()
{
}

// ======================== CONVERSION FUNCTIONS =========================

void TestCommonExpressionFunction::TestNullValueFunction ()
{
}

void TestCommonExpressionFunction::TestToDateFunction ()
{
}

void TestCommonExpressionFunction::TestToDateFunctionAsFilter ()
{
}

void TestCommonExpressionFunction::TestToDoubleFunction ()
{
}

void TestCommonExpressionFunction::TestToDoubleFunctionAsFilter ()
{
}

void TestCommonExpressionFunction::TestToFloatFunction ()
{
}

void TestCommonExpressionFunction::TestToInt32Function ()
{
}

void TestCommonExpressionFunction::TestToInt64Function ()
{
}

void TestCommonExpressionFunction::TestToStringFunction ()
{
}


// =========================== DATE FUNCTIONS ============================

void TestCommonExpressionFunction::TestAddMonthsFunction ()
{
}

void TestCommonExpressionFunction::TestCurrentDateFunction ()
{
}

void TestCommonExpressionFunction::TestExtractFunction ()
{
}

void TestCommonExpressionFunction::TestExtractToDoubleFunction ()
{
}

void TestCommonExpressionFunction::TestExtractToIntFunction ()
{
}

void TestCommonExpressionFunction::TestMonthsBetweenFunction ()
{
}


// ===========================================================================
// ==                     TESTING THE GEOMETRY FUNCTIONS                    ==
// ===========================================================================

void TestCommonExpressionFunction::TestXYZMFunction ()
{
}

// =========================== MATH FUNCTIONS ============================

void TestCommonExpressionFunction::TestAbsFunction ()
{
}

void TestCommonExpressionFunction::TestAcosFunction ()
{
}

void TestCommonExpressionFunction::TestAsinFunction ()
{
}

void TestCommonExpressionFunction::TestAtanFunction ()
{
}

void TestCommonExpressionFunction::TestAtan2Function ()
{
}

void TestCommonExpressionFunction::TestCosFunction ()
{
}

void TestCommonExpressionFunction::TestExpFunction ()
{
}

void TestCommonExpressionFunction::TestLnFunction ()
{
}

void TestCommonExpressionFunction::TestLogFunction ()
{
}

void TestCommonExpressionFunction::TestModFunction ()
{
}

void TestCommonExpressionFunction::TestPowerFunction ()
{
}

void TestCommonExpressionFunction::TestRemainderFunction ()
{
}

void TestCommonExpressionFunction::TestSinFunction ()
{
}

void TestCommonExpressionFunction::TestSqrtFunction ()
{
}

void TestCommonExpressionFunction::TestTanFunction ()
{
}


// ========================== NUMERIC FUNCTIONS ==========================

void TestCommonExpressionFunction::TestCeilFunction ()
{
}

void TestCommonExpressionFunction::TestFloorFunction ()
{
}

void TestCommonExpressionFunction::TestRoundFunction ()
{
}

void TestCommonExpressionFunction::TestSignFunction ()
{
}

void TestCommonExpressionFunction::TestTruncFunction ()
{
}


// ========================== STRING FUNCTIONS ===========================

void TestCommonExpressionFunction::TestConcatFunction ()
{
}

void TestCommonExpressionFunction::TestInstrFunction ()
{
}

void TestCommonExpressionFunction::TestInstrFunctionAsFilter ()
{
}

void TestCommonExpressionFunction::TestLengthFunction ()
{
}

void TestCommonExpressionFunction::TestLowerFunction ()
{
}

void TestCommonExpressionFunction::TestLpadFunction ()
{
}

void TestCommonExpressionFunction::TestLtrimFunction ()
{
}

void TestCommonExpressionFunction::TestRpadFunction ()
{
}

void TestCommonExpressionFunction::TestRtrimFunction ()
{
}

void TestCommonExpressionFunction::TestSoundexFunction ()
{
}

void TestCommonExpressionFunction::TestSubstrFunction ()
{
}

void TestCommonExpressionFunction::TestTranslateFunction ()
{
}

void TestCommonExpressionFunction::TestTrimFunction ()
{
}

void TestCommonExpressionFunction::TestUpperFunction ()
{
}


//-------------------------------------------------------------------------
//                     Combination Unit Test Functions
//-------------------------------------------------------------------------

void TestCommonExpressionFunction::NumberToStringComparison ()
{
}

void TestCommonExpressionFunction::RoundNumberToStringComparison ()
{
}

//-------------------------------------------------------------------------
//                       Special Unit Test Functions
//-------------------------------------------------------------------------

void TestCommonExpressionFunction::ModuloOperation ()
{
}

void TestCommonExpressionFunction::RunOptimizerTest()
{
}

void TestCommonExpressionFunction::SoundexFilter ()
{
}

void TestCommonExpressionFunction::SubstrOperation ()
{
}

void TestCommonExpressionFunction::ToDateOperation ()
{
}

void TestCommonExpressionFunction::CheckXYZMCapabilities()
{
    bool                            func_check_error = false;

    FdoFunctionDefinition           *this_func;
    FdoIExpressionCapabilities      *exp_cap;
    FdoFunctionDefinitionCollection *func_col; 

    try {

      exp_cap = m_connection->GetExpressionCapabilities();
      func_col = exp_cap->GetFunctions();

      this_func = (FdoFunctionDefinition *) func_col->FindItem(L"X");
      if (this_func == NULL)
          func_check_error = true;
	  
      this_func = (FdoFunctionDefinition *) func_col->FindItem(L"Y");
      if (this_func == NULL)
          func_check_error = true;

      this_func = (FdoFunctionDefinition *) func_col->FindItem(L"Z");
      if (this_func == NULL)
          func_check_error = true;

      this_func = (FdoFunctionDefinition *) func_col->FindItem(L"M");
      if (this_func == NULL)
          func_check_error = true;

      if (func_check_error)
          throw FdoException::Create(
                    L"Unexpected result(s) when checking function existence");
      else
        printf(" >>> Check succeeded \n");

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
}



// ----------------------------------------------------------------------------
// --                      General Supporting Functions                      --
// ----------------------------------------------------------------------------

void TestCommonExpressionFunction::SelectXYZM (FdoFilter *filter,
                                             FdoString *cmp_id,
                                             FdoString *cmp_id_str,
                                             FdoDouble exp_values[4][5])
{
    FdoPtr<FdoIFeatureReader>            feature_reader;
    FdoPtr<FdoIGeometryCapabilities>     geom_caps;  
    FdoInt32                             dimensionalities;

    // Find out supported dimensionalities

    geom_caps = m_connection->GetGeometryCapabilities();
    dimensionalities = geom_caps->GetDimensionalities();

    feature_reader = ExecuteSelectCommand(
                                    XY_POINT_CLASS, filter, true, cmp_id_str);
    CheckXYZMValue(feature_reader, 0, cmp_id, exp_values);

    if ( dimensionalities & FdoDimensionality_Z ) {
        feature_reader = ExecuteSelectCommand(
                                        XYZ_POINT_CLASS, filter, true, cmp_id_str);
        CheckXYZMValue(feature_reader, 1, cmp_id, exp_values);
    }

    if ( (dimensionalities & FdoDimensionality_Z) && (dimensionalities & FdoDimensionality_M) ) {
        feature_reader = ExecuteSelectCommand(
                                        XYZM_POINT_CLASS, filter, true, cmp_id_str);
        CheckXYZMValue(feature_reader, 2, cmp_id, exp_values);
    }

    if ( dimensionalities & FdoDimensionality_M ) {
        feature_reader = ExecuteSelectCommand(
                                        XYM_POINT_CLASS, filter, true, cmp_id_str);
        CheckXYZMValue(feature_reader, 3, cmp_id, exp_values);
    }
}

void TestCommonExpressionFunction::SelectXYZMFilter (FdoFilter *filter,
                                      FdoString *cmp_id,
                                      FdoString* class_name,
                                      FdoInt32 dimension,
                                      FdoInt32 exp_count,
                                      FdoInt32 *exp_indexes)
{
    FdoPtr<FdoIFeatureReader> feature_reader = ExecuteSelectCommand(
                                    class_name, filter, true, cmp_id);

    FdoInt32 count = 0;

    while ( feature_reader->ReadNext() ) {
        FdoInt32 idx = GetXYZMIndex( dimension, (FdoInt32)(GetFeatId(feature_reader)) );

        bool found = false;
        for ( int i = 0; i < exp_count; i++ ) {
            if ( idx == exp_indexes[i] ) {
                found = true;
                break;
            }
        }

        if ( !found ) 
            throw FdoException::Create(
                        L"Unexpected result(s) when checking returned data");
        
        count++;
    }

    if ( count != exp_count ) 
        throw FdoException::Create(
            L"Unexpected result(s) when checking returned data");
}


void TestCommonExpressionFunction::CheckReader (FdoIDataReader *data_reader,
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

void TestCommonExpressionFunction::CheckReader (
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

void TestCommonExpressionFunction::CheckReader16 (
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

void TestCommonExpressionFunction::CheckReader32 (
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

void TestCommonExpressionFunction::CheckReader64 (
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

void TestCommonExpressionFunction::CheckReader64 (
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

void TestCommonExpressionFunction::CheckReaderByte (
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

void TestCommonExpressionFunction::CheckReaderByte (
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

void TestCommonExpressionFunction::CheckReaderDt (
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

void TestCommonExpressionFunction::CheckReaderDt (
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

void TestCommonExpressionFunction::CheckReaderNumberString (
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

void TestCommonExpressionFunction::CheckReaderSgl (
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

void TestCommonExpressionFunction::CheckReaderString (
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

      id_prop_val = data_reader->GetInt32(L"id");
      cmp_id_val  = (data_reader->IsNull(L"cmp_id"))
                  ? NULL
                  : data_reader->GetString(L"cmp_id");

      is_valid_result =
        ((id_prop_val == expected_id_value) &&
         (((cmp_id_val == NULL) && (expected_cmp_id_value == NULL)) ||
          ((cmp_id_val != NULL) && (expected_cmp_id_value != NULL) &&
                         (wcscmp(cmp_id_val, expected_cmp_id_value) == 0))));
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

void TestCommonExpressionFunction::CheckXYZMValue (
                                    FdoIFeatureReader *data_reader,
                                    int               dimension,
                                    FdoString         *property_name,
                                    FdoDouble         exp_values[4][5])

// +---------------------------------------------------------------------------
// | The function checks whether or not the provided reader contains the ex-
// | pected data and throws an exception if this is not the case.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    int       data_count            = 0;
    FdoInt32  idx                   = 0;

    bool      unexpected_result     = false,
              unexpected_data_count = false;

    FdoDouble curr_value;

    // Navigate through the reader and perform the necessary checks.

    printf(" >>> Cross check result \n");

    while (data_reader->ReadNext()) {

      idx = GetXYZMIndex( dimension, GetFeatId(data_reader) );

      data_count++;
      if (data_reader->IsNull(property_name))
          curr_value = -1;
          //printf("      >> Feature (%d) %ls = null \n",
          //       data_count,
          //       property_name);
      else {

        curr_value = data_reader->GetDouble(property_name);
        //printf("      >> Feature (%d) %ls = %f \n",
        //       data_count,
        //       property_name,
        //       curr_value);

      }  //  else ...

      if (idx > 11)
          unexpected_data_count = true;
      else {

        if (curr_value != exp_values[dimension][idx])
            unexpected_result = true;


      }  //  else ...

	}  //  while ...

    // Close the reader.

    data_reader->Close();

    if ((unexpected_data_count) || (unexpected_result)) {

        if (unexpected_data_count)
            throw FdoException::Create(
                        L"Unexpected result(s) when checking returned data");
        else
            throw FdoException::Create(
                        L"Unexpected result(s) when checking returned data");

    }  //  if ((unexpected_data_count) || ...
    else
      printf(" >>> ... All expected data found\n");

}  //  CheckXYZMValue ()

void TestCommonExpressionFunction::CheckXYZMDim   (FdoIFeatureReader *data_reader,
                                                   FdoString         *property_name,
                                                   FdoDouble          exp_value,
                                                   FdoBoolean         exp_null)
{
    if ( data_reader->IsNull(property_name) )
    {
        if ( !exp_null ) 
            throw FdoException::Create(
                FdoStringP::Format( L"%ls is null when value expected", property_name )
            );
    }
    else 
    {
        if ( exp_null ) 
            throw FdoException::Create(
                FdoStringP::Format( L"%ls is not null when expected to be null", property_name )
            );

        double value = data_reader->GetDouble( property_name );
        if ( value != exp_value ) 
            throw FdoException::Create(
                FdoStringP::Format( L"%ls is %lf, expected %lf", property_name, value, exp_value )
            );

    }
}

bool TestCommonExpressionFunction::EvaluateResults (
                                        FdoSpatialCondition *bigGeomCond,
                                        FdoSpatialCondition *smallGeomCond,
                                        FdoIFeatureReader   *dataProv,
                                        FdoIFeatureReader   *result)

// +---------------------------------------------------------------------------
// | The function evaluates the result from the filter optimizer test.
// +---------------------------------------------------------------------------

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

    while(dataProv->ReadNext()) {

      FdoPtr<FdoByteArray> bGeom = dataProv->GetGeometry(L"SHPGEOM");
      int idxGen = dataProv->GetInt32(L"Autogenerated_SDF_ID");
      FdoPtr<FdoIGeometry> geom = gf->CreateGeometryFromFgf (bGeom);
      bool retFirstOp = FdoSpatialUtility::Evaluate (geom, firstOp, geomBig);
      bool retSecondOp = FdoSpatialUtility::Evaluate (geom, secOp, geomSmall);
      if (retFirstOp && retSecondOp) {

          idxResultNoOpt.push_back(idxGen);
          operResult.push_back(TestIdxOperationsPair(idxGen, TestOperationsPair(firstOp, secOp)));

        }  //  if (retFirstOp && retSecondOp) ...

    }  //  while(dataProv->ReadNext()) ...

    if (idxResultOpt.size() != idxResultNoOpt.size())
        retVal = false;
    
    std::vector<int>* op1 = &idxResultOpt;
    std::vector<int>* op2 = &idxResultNoOpt;
    if (idxResultOpt.size() == 0) {

        op1 = &idxResultNoOpt;
        op2 = &idxResultOpt;

    }  //  if (idxResultOpt.size() == 0) ...

    for (size_t i = 0; i < op1->size(); i++) {

      int objId = op1->at(i);
      bool valFound = false;
      for (size_t y = 0; y < op2->size(); y++) {

      if (objId == op2->at(y)) {

          valFound = true;
          break;

      }  //  if (objId == op2->at(y)) ...

    }  //  for (size_t y = 0; ...

    if (!valFound) {

        retVal = false;
        for (size_t y = 0; y < operResult.size(); y++) {

          TestIdxOperationsPair opRes = operResult.at(y);
          if (opRes.first == objId) {

              FdoStringP txtOp1 = FdoCommonMiscUtil::FdoSpatialOperationsToString(opRes.second.first).Right(L"_");
              FdoStringP txtOp2 = FdoCommonMiscUtil::FdoSpatialOperationsToString(opRes.second.second).Right(L"_");
              printf("\nFail at: %d (%ls, %ls)", opRes.first, (FdoString*)txtOp1, (FdoString*)txtOp2);
              break;

          }  //  if (opRes.first == objId) ...

        }  //  for (size_t y = 0; ...

      }  //  if (!valFound) ...

    }  //  for (size_t i = 0; ...

    return retVal;

}  //  EvaluateResults ()



FdoIDataReader *TestCommonExpressionFunction::ExecuteSelAggrCommand (
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

FdoIFeatureReader *TestCommonExpressionFunction::ExecuteSelectCommand (
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
    id_prop = FdoIdentifier::Create(L"featid");
    id_col->Add(id_prop);
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

FdoIFeatureReader *TestCommonExpressionFunction::
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

FdoDateTime TestCommonExpressionFunction::GetDate (FdoString *class_name,
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

FdoStringP TestCommonExpressionFunction::GetStringValue (
                                                    FdoString *class_name,
                                                    FdoString *property_name,
                                                    FdoFilter *filter)

// +---------------------------------------------------------------------------
// | The function retrieves the value for a string property identified by the
// | provided filter and returns it back to the calling procedure.
// +---------------------------------------------------------------------------

{

    // Declare and initialize all necessary local vatiables.

    FdoStringP                      str_data;

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
      str_data = feature_reader->GetString(property_name);

    return str_data;

}  //  GetStringValue ()


// ----------------------------------------------------------------------------
// --                    Test Environment Setup Functions                    --
// ----------------------------------------------------------------------------

void TestCommonExpressionFunction::CloseConnection ()

// +---------------------------------------------------------------------------
// | The function closes all open connections to the data store used in the
// | executed unit tests.
// +---------------------------------------------------------------------------

{

    if (m_connection != NULL)
        m_connection->Close();

}  //  CloseConnection ()

void TestCommonExpressionFunction::Connect ()

// +---------------------------------------------------------------------------
// | The function establishes the connection to the data store used in the
// | executed test suites.
// +---------------------------------------------------------------------------

{
}  //  Connect ()

void TestCommonExpressionFunction::SetupUnitTestEnvironment (
                                        FdoIConnection* /* current_connection */)
{
}  //  SetupUnitTestEnvironment ()

FdoInt32 TestCommonExpressionFunction::GetFeatId( FdoIFeatureReader* rdr )
{
    return rdr->GetInt32(L"featid");
}

FdoDataType TestCommonExpressionFunction::GetFeatIdType()
{
    return FdoDataType_Int32;
}

void TestCommonExpressionFunction::SetXYZMIndex( int dimension, int index, FdoInt32 FeatId )
{
    m_XYZMIndex[dimension][index] = FeatId;
}

int TestCommonExpressionFunction::GetXYZMIndex( int dimension, FdoInt32 FeatId )
{
    for ( int idx = 0; idx < 11; idx++ ) {
        if ( FeatId == m_XYZMIndex[dimension][idx] ) 
            return idx;
    }

    return -1;
}

bool TestCommonExpressionFunction::PointMAlwaysNull()
{
    return false;
}


