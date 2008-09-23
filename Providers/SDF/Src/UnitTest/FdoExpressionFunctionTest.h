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

#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>
#include <Fdo.h>


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



class FdoExpressionFunctionTest : public CppUnit::TestCase
{

  CPPUNIT_TEST_SUITE(FdoExpressionFunctionTest);
    // =========================================
    // ====  AGGREGATE FUNCTION UNIT TESTS  ====
    // =========================================
    // CPPUNIT_TEST(TestAvgFunction);
    // CPPUNIT_TEST(TestCountFunction);
    // CPPUNIT_TEST(TestMaxFunction);
    // CPPUNIT_TEST(TestMedianFunction);
    // CPPUNIT_TEST(TestMinFunction);
    // CPPUNIT_TEST(TestStddevFunction);
    // CPPUNIT_TEST(TestSumFunction);
    // =========================================
    // ==== CONVERSION FUNCTION UNIT TESTS  ====
    // =========================================
    // CPPUNIT_TEST(TestNullValueFunction);
    // CPPUNIT_TEST(TestToDateFunction);
    // CPPUNIT_TEST(TestToDateFunctionAsFilter);
    // CPPUNIT_TEST(TestToDoubleFunction);
    // CPPUNIT_TEST(TestToDoubleFunctionAsFilter);
    // CPPUNIT_TEST(TestToFloatFunction);
    // CPPUNIT_TEST(TestToInt32Function);
    // CPPUNIT_TEST(TestToInt64Function);
    // CPPUNIT_TEST(TestToStringFunction);
    // =========================================
    // ====    DATE FUNCTION UNIT TESTS     ====
    // =========================================
    // CPPUNIT_TEST(TestAddMonthsFunction);
    // CPPUNIT_TEST(TestCurrentDateFunction);
    // CPPUNIT_TEST(TestExtractFunction);
    // CPPUNIT_TEST(TestMonthsBetweenFunction);
    // =========================================
    // ====    MATH FUNCTION UNIT TESTS     ====
    // =========================================
    // CPPUNIT_TEST(TestAbsFunction);
    // CPPUNIT_TEST(TestAcosFunction);
    // CPPUNIT_TEST(TestAsinFunction);
    // CPPUNIT_TEST(TestAtanFunction);
    // CPPUNIT_TEST(TestAtan2Function);
    // CPPUNIT_TEST(TestCosFunction);
    // CPPUNIT_TEST(TestExpFunction);
    // CPPUNIT_TEST(TestLnFunction);
    // CPPUNIT_TEST(TestLogFunction);
    // CPPUNIT_TEST(TestModFunction);
    // CPPUNIT_TEST(TestPowerFunction);
    // CPPUNIT_TEST(TestRemainderFunction);
    // CPPUNIT_TEST(TestSinFunction);
    // CPPUNIT_TEST(TestSqrtFunction);
    // CPPUNIT_TEST(TestTanFunction);
    // =========================================
    // ====   NUMERIC FUNCTION UNIT TESTS   ====
    // =========================================
    // CPPUNIT_TEST(TestCeilFunction);
    // CPPUNIT_TEST(TestFloorFunction);
    // CPPUNIT_TEST(TestRoundFunction);
    // CPPUNIT_TEST(TestSignFunction);
    // CPPUNIT_TEST(TestTruncFunction);
    // =========================================
    // ====   STRING FUNCTION UNIT TESTS    ====
    // =========================================
    // CPPUNIT_TEST(TestConcatFunction);
    // CPPUNIT_TEST(TestInstrFunction);
    // CPPUNIT_TEST(TestInstrFunctionAsFilter);
    // CPPUNIT_TEST(TestLengthFunction);
    // CPPUNIT_TEST(TestLowerFunction);
    // CPPUNIT_TEST(TestLpadFunction);
    // CPPUNIT_TEST(TestLtrimFunction);
    // CPPUNIT_TEST(TestRpadFunction);
    // CPPUNIT_TEST(TestRtrimFunction);
    // CPPUNIT_TEST(TestSoundexFunction);
    // CPPUNIT_TEST(TestSubstrFunction);
    // CPPUNIT_TEST(TestTranslateFunction);
    // CPPUNIT_TEST(TestTrimFunction);
    // CPPUNIT_TEST(TestUpperFunction);
    // =========================================
    // ====     COMBINATION UNIT TESTS      ====
    // =========================================
    // CPPUNIT_TEST(NumberToStringComparison);
    // CPPUNIT_TEST(RoundNumberToStringComparison);
    // =========================================
    // ====       RUN ALL UNIT TESTS        ====
    // =========================================
    CPPUNIT_TEST(RunAllExpFctTests);
    // =========================================
    // ====       SPECIAL UNIT TESTS        ====
    // =========================================
    // CPPUNIT_TEST(ModuloOperation);
    // CPPUNIT_TEST(SoundexFilter);
    // CPPUNIT_TEST(SubstrOperation);
    // CPPUNIT_TEST(ToDateOperation);
    // =========================================
    // ====         OPTIMIZER TEST          ====
    // =========================================
    CPPUNIT_TEST(RunOptimizerTest);
  CPPUNIT_TEST_SUITE_END();

public:
            FdoExpressionFunctionTest ();
    virtual ~FdoExpressionFunctionTest ();
    void    setUp ();
    void    tearDown ();


protected:

    //-------------------------------------------------------------------------
    //                       Test Environment Setup Functions
    //-------------------------------------------------------------------------

    //  AddFeature:
    //      The function adds a new object for the specified class.The values
    //      being added are predefined based on the predefined schema.

    void AddFeature (FdoIConnection *current_connection,
                     FdoString      *class_name,
                     int            index);

    //  AddTestSchema:
    //      The function establishes the test schema in the FDO data store
    //      the provided connection identifies.

    void AddTestSchema (FdoIConnection *current_connection,
                        FdoString      *schema_name);

    //  CloseConnection:
    //      The function closes all open connections to the data store used in
    //      the executed unit tests.

    void CloseConnection ();

    //  Connect:
    //      The function establishes the connection to the data store used in
    //      the executed test suites.

	void Connect ();

    //  CreateDataProperty :
    //      The function creates a data property object based on the provided
    //      data and returns it back to the calling routine.

    FdoDataPropertyDefinition *CreateDataProperty (
                                                   FdoString   *property_name,
                                                   FdoDataType data_type,
                                                   FdoInt32    data_size,
                                                   FdoInt32    scale,
                                                   bool        is_nullable);

    //  CreateFdoFeatureClass:
    //      The function creates a predefined feature class with the caller
    //      identifying the class name.

    FdoFeatureClass *CreateFdoFeatureClass (FdoString *class_name);

    //  CreateGeometricProperty:
    //      The function creates the named geometry property and returns it
    //      back to the calling routine.

    FdoGeometricPropertyDefinition *CreateGeometricProperty (
                                                    FdoString *property_name);

    //  SetupUnitTestEnvironment:
    //      The function controls the establishing of the test environment in
    //      the FDO data store the provided connection identifies.

    void SetupUnitTestEnvironment (FdoIConnection *current_connection);


    //-------------------------------------------------------------------------
    //                           Unit Test Functions
    //-------------------------------------------------------------------------

    //  RunAllExpFctTests:
    //      The function controls the execution of all unit tests. This way,
    //      the test setup is done only once.

    void RunAllExpFctTests ();

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

    //  RunOptimizerTest:
    //      The function tests the expression optimizer for filters
    void RunOptimizerTest();

    //-------------------------------------------------------------------------
    //                       General Supporting Functions
    //-------------------------------------------------------------------------

    //  CheckReader:
    //      The function checks whether or not the provided reader contains
    //      the expected data and throws an exception if this is not the 
    //      case.

    void CheckReader (FdoIDataReader *data_reader,
                      bool           include_id_check,
                      FdoInt32       expected_id_value,
                      FdoDouble      expected_value);

    void CheckReader (FdoIFeatureReader *data_reader,
                      bool              include_id_check,
                      FdoInt32          expected_id_value,
                      FdoDouble         expected_value);

    //  CheckReader16:
    //      The function checks whether or not the provided reader contains
    //      the expected data and throws an exception if this is not the 
    //      case.

    void CheckReader16 (FdoIFeatureReader *data_reader,
                        bool              include_id_check,
                        FdoInt32          expected_id_value,
                        FdoInt16          expected_value);

    //  CheckReader32:
    //      The function checks whether or not the provided reader contains
    //      the expected data and throws an exception if this is not the 
    //      case.

    void CheckReader32 (FdoIFeatureReader *data_reader,
                        bool              include_id_check,
                        FdoInt32          expected_id_value,
                        FdoInt32          expected_value);

    //  CheckReader64:
    //      The function checks whether or not the provided reader contains
    //      the expected data and throws an exception if this is not the 
    //      case.

    void CheckReader64 (FdoIDataReader *data_reader,
                        bool           include_id_check,
                        FdoInt32       expected_id_value,
                        FdoInt64       expected_value);

    void CheckReader64 (FdoIFeatureReader *data_reader,
                        bool              include_id_check,
                        FdoInt32          expected_id_value,
                        FdoInt64          expected_value);

    //  CheckReaderByte:
    //      The function checks whether or not the provided reader contains
    //      the expected data and throws an exception if this is not the 
    //      case.

    void CheckReaderByte (FdoIDataReader *data_reader,
                          bool           include_id_check,
                          FdoInt32       expected_id_value,
                          FdoByte        expected_value);

    void CheckReaderByte (FdoIFeatureReader *data_reader,
                          bool              include_id_check,
                          FdoInt32          expected_id_value,
                          FdoByte           expected_value);

    //  CheckReaderDt:
    //      The function checks whether or not the provided reader contains
    //      the expected data and throws an exception if this is not the 
    //      case.

    void CheckReaderDt (FdoIDataReader *data_reader,
                        bool           include_id_check,
                        bool           is_extract_request,
                        FdoInt16       extract_type_id,
                        FdoInt32       expected_id_value,
                        FdoDateTime    expected_cmp_id_value);

    void CheckReaderDt (FdoIFeatureReader *data_reader,
                        bool              include_id_check,
                        bool              is_extract_request,
                        FdoInt16          extract_type_id,
                        FdoInt32          expected_id_value,
                        FdoDateTime       expected_cmp_id_value);

    //  CheckReaderNumberString:
    //      The function checks whether or not the provided reader contains
    //      the expected data and throws an exception if this is not the 
    //      case.

    void CheckReaderNumberString (FdoIFeatureReader *data_reader,
                                  FdoInt32          expected_id_value,
                                  FdoString         *expected_cmp_id_value);

    //  CheckReaderSgl:
    //      The function checks whether or not the provided reader contains
    //      the expected data and throws an exception if this is not the 
    //      case.

    void CheckReaderSgl (FdoIFeatureReader *data_reader,
                         bool              include_id_check,
                         FdoInt32          expected_id_value,
                         FdoFloat          expected_value);

    //  CheckReaderString:
    //      The function checks whether or not the provided reader contains
    //      the expected data and throws an exception if this is not the 
    //      case.

    void CheckReaderString (FdoIFeatureReader *data_reader,
                            FdoInt32          expected_id_value,
                            FdoString         *expected_cmp_id_value);

    //  ExecuteSelAggrCommand:
    //      The function executes a select-aggregate command using the pro-
    //      vided data and returns the generated reader back to the calling
    //      routine.

    FdoIDataReader *ExecuteSelAggrCommand (FdoString *class_name,
                                           FdoFilter *filter,
                                           bool      inc_id_prop,
                                           FdoString *cmp_id_str);

    //  ExecuteSelectCommand:
    //      The function executes a select-command using the provided data and
    //      returns the generated reader back to the calling routine.

    FdoIFeatureReader *ExecuteSelectCommand (FdoString *class_name,
                                             FdoFilter *filter,
                                             bool      inc_id_prop,
                                             FdoString *cmp_id_str);

    //  ExecuteSelectCommand:
    //      The function executes a select-command to retrieve the values of
    //      the named properties and returns the generated reader back to the
    //      calling routine.

    FdoIFeatureReader *ExecuteSelectCommand (FdoString *class_name,
                                             FdoFilter *filter,
                                             FdoString *prop1,
                                             FdoString *prop2,
                                             FdoString *prop3,
                                             FdoString *prop4,
                                             FdoString *prop5,
                                             FdoString *prop6);

    //  GetDate:
    //      The function retrieves the date set for a property identified by
    //      the provided filter and returns this one back to the calling pro-
    //      cedure.

    FdoDateTime GetDate (FdoString *class_name,
                         FdoString *property_name,
                         FdoFilter *filter);

    // optimizer util functions
    // inser test geometries
    void InsertTestFeatures(FdoIConnection* conn);
    // evaluate results from optimizer with direct evaluation
    bool EvaluateResults(FdoSpatialCondition* bigGeomCond, FdoSpatialCondition* smallGeomCond, FdoIFeatureReader* dataProv, FdoIFeatureReader* result);

    //-------------------------------------------------------------------------
    //                                Variables
    //-------------------------------------------------------------------------

    //  m_connection:
    //      The variable holds the connection to the test data store.

    FdoPtr<FdoIConnection> m_connection;

};  //  class FdoExpressionFunctionTest

#endif
