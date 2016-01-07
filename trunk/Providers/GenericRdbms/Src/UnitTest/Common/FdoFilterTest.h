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

#ifndef FDO_FILTERTEST_H
#define FDO_FILTERTEST_H


// =============================================================================
// | The file contains the definition of the class FdoFilterTest. It defines the
// | common filter-related unit tests
// =============================================================================


// ---------------------------------------------------------------------------
//                                  Constants                                 
// ---------------------------------------------------------------------------

//  DATASTORE_EXTENSION:
//      The constants specifies the extension to the data store name to be
//      used when creating the data store

#define DATASTORE_EXTENSION      L"_FLT_X"


class FdoFilterTest : public CppUnit::TestCase
{

  CPPUNIT_TEST_SUITE(FdoFilterTest);
    // =========================================
    // ====     NESTED FILTER SQL TESTS     ====
    // =========================================
	// CPPUNIT_TEST(NestedFilterSQLTest);
	// CPPUNIT_TEST(MaxORFilterSelectTest);
	// CPPUNIT_TEST(NegationORFilterSelectTest);
	// CPPUNIT_TEST(SpatialObjectFilterTest);
    // =========================================
    // ====       RUN ALL UNIT TESTS        ====
    // =========================================
    CPPUNIT_TEST(RunAllFltTests);
  CPPUNIT_TEST_SUITE_END();

public:
            FdoFilterTest ();
    virtual ~FdoFilterTest ();
    void    setUp ();
    void    tearDown ();


protected:

    //-------------------------------------------------------------------------
    //                                 Enumerations
    //-------------------------------------------------------------------------

    //  UnitTestIds
    //      The different providers generate different strings from any of the
    //      given filters. To verify whether or not nesting is present, the
    //      verification procedure must identify the part that represents the
    //      filter. This depends on the executed test.
    //      This enumeration contains a test id for each of the tests executed
    //      in the test suite. It is used by the provider-specific implementa-
    //      tion to identify where the break-up point is.

    enum UnitTestIds {

        MAXORFILTERSELECTTEST,
        NEGATIONORFILTERSELECTTEST_1,
        NEGATIONORFILTERSELECTTEST_2,
        NESTEDFILTERSQLTEST,
        SPATIALOBJECTFILTERCASE_1,
        SPATIALOBJECTFILTERCASE_2

    };  //  enum UnitTestIds

    //-------------------------------------------------------------------------
    //                       Test Environment Setup Functions
    //-------------------------------------------------------------------------

    //  AddFeature:
    //      The function adds a new object for the specified class.The values
    //      being added are predefined based on the predefined schema.

    virtual void AddFeature (FdoIConnection *current_connection,
                             FdoString      *class_name,
                             int            index);

    //  AddTestSchema:
    //      The function establishes the test schema in the FDO data store
    //      the provided connection identifies.

    virtual void AddTestSchema (FdoIConnection *current_connection,
                                FdoString      *schema_name);

    //  CloseConnection:
    //      The function closes all open connections to the data store used in
    //      the executed unit tests.

    virtual void CloseConnection ();

    //  Connect:
    //      The function establishes the connection to the data store used in
    //      the executed test suites.

	virtual void Connect ();

    //  CreateDataProperty :
    //      The function creates a data property object based on the provided
    //      data and returns it back to the calling routine.

    virtual FdoDataPropertyDefinition *CreateDataProperty (
                                                  FdoString   *property_name,
                                                  FdoDataType data_type,
                                                  FdoInt32    data_size,
                                                  FdoInt32    scale,
                                                  bool        is_nullable);

    //  CreateFdoFeatureClass:
    //      The function creates a predefined feature class with the caller
    //      identifying the class name.

    virtual FdoFeatureClass *CreateFdoFeatureClass (FdoString *class_name);

    //  CreateGeometricProperty:
    //      The function creates the named geometry property and returns it
    //      back to the calling routine.

    virtual FdoGeometricPropertyDefinition *CreateGeometricProperty (
                                                    FdoString *property_name);

    //  CreateTestDataStore:
    //      The function creates the test data store.

    virtual void CreateTestDataStore (FdoString *datastore_extension);

    //  DropTestDataStores:
    //      The function drops the test data store.

    virtual void DropTestDataStore (FdoString *datastore_extension);

    //  GetDimensionality:
    //      The function returns the dimensionality the provider supports.

    virtual FdoInt32 GetDimensionality ();   

    //  SetProvider:
    //      Sets the provider for the current test. Any Provider that supports
    //      these tests need to implement it.

    virtual void SetProvider ();   

    //  SetupUnitTestEnvironment:
    //      The function controls the establishing of the test environment in
    //      the FDO data store the provided connection identifies.

    virtual void SetupUnitTestEnvironment (FdoIConnection *current_connection);


    //-------------------------------------------------------------------------
    //                           Unit Test Functions
    //-------------------------------------------------------------------------

    //  RunAllFltTests:
    //      The function controls the execution of all unit tests. This way,
    //      the test setup is done only once.

    virtual void RunAllFltTests ();


    // ===================== SPECIFIC UNIT TEST FUNCTIONS ====================

    //  MaxORFilterSelectTest:
    //      The function executes a select-test with a filter that consists
    //      of a provider-specific maximum number of filter elements. The
    //      aim of the test is to check whether or not such a filter can be
    //      executed as this was not always the case before the nested filter
    //      issue was addressed.

    virtual void MaxORFilterSelectTest ();

    //  NegationORFilterSelectTest:
    //      The function executes a select-test with a filter that consists
    //      of a list of binary logical operator combined elements where all
    //      or some of them are negated. The test verifies that the generated
    //      SQL statement is valid.

    virtual void NegationORFilterSelectTest ();

    //  NestedFilterSQLTest:
    //      The function executes the nested filter SQL tests and checks if
    //      nesting of a filter statement is applied only if necessary.

    virtual void NestedFilterSQLTest ();

    //  SpatialObjectFilterTest:
    //      The function executes tests that check if the filter processor
    //      correctly identifies a filter case where a spatial and an object
    //      filter are combined with a binary logical operation as this kind
    //      of filter has a specific optimization.

    virtual void SpatialObjectFilterTest ();


    //-------------------------------------------------------------------------
    //                       General Supporting Functions
    //-------------------------------------------------------------------------

    //  BuildMixedOperationFilter:
    //      The function generates a filter where each of the filter elements
    //      are connected by different binary logical operations. The generated
    //      filter is returned to the calling routine.

    FdoFilter *BuildMixedOperationFilter (
                                    FdoInt32                   start_index,
                                    FdoInt32                   end_index,
                                    FdoBinaryLogicalOperations first_op,
                                    FdoBinaryLogicalOperations second_op);

    //  BuildSingleOperationFilter:
    //      The function generates a filter where each of the filter elements
    //      are connected by the given operation. The generated filter is re-
    //      turned to the calling routine.

    FdoFilter *BuildSingleOperationFilter (
                                    FdoInt32                   start_index,
                                    FdoInt32                   end_index,
                                    FdoBinaryLogicalOperations operation);

    //  CheckForGrouping:
    //      The function checks whether or not two filter elements linked by
    //      a binary logical operator are grouped (surrounded by brackets).

    bool CheckForGrouping (FdoStringP  sql_statement,
                           UnitTestIds unit_test_id);

    //  CheckForNesting:
    //      The function checks whether or not the given SQL string contains
    //      nested elements and returns the findings of the check back to the
    //      calling routine.

    bool CheckForNesting (FdoStringP sql_statement);

    //  CheckReader:
    //      The function checks whether or not the provided reader contains
    //      the expected data and throws an exception if this is not the 
    //      case.

    void CheckReader (FdoIFeatureReader *reader,
                      FdoInt32          expected_count);

    //  ExecuteSelectCommand:
    //      The function executes a select-command using the provided data
    //      and returns the generated reader back to the calling routine.

    FdoIFeatureReader *ExecuteSelectCommand (FdoString *class_name,
                                             FdoFilter *filter);

    //  GetMaxFilterElementNumber:
    //      The function returns the maximum number of filter elements that
    //      should be combined in a filter. 

    virtual FdoInt32 GetMaxFilterElementNumber ();

    //  TranslateFilter:
    //      The function requests the SQL representation of the given filter
    //      and checks the result whether or not nesting or grouping has been
    //      used. If nesting or grouping has been used where none is expected
    //      or nesting or grouping has not been used when expected an exception
    //      is issued.

    virtual void TranslateFilter (FdoFilter   *filter,
                                  bool        is_nesting_expected,
                                  bool        is_grouping_expected,
                                  UnitTestIds unit_test_id);


    //-------------------------------------------------------------------------
    //                                Variables
    //-------------------------------------------------------------------------

    //  m_connection:
    //      The variable holds the connection to the test data store.

    FdoPtr<FdoIConnection> m_connection;

};  //  class FdoFilterTest

#endif
