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

#ifndef TESTCOMMONEXPRESSIONFUNCTIONW_H
#define TESTCOMMONEXPRESSIONFUNCTIONW_H

#include "TestCommonExpressionFunction.h"


// =============================================================================
// | The file contains the definition of the class FdoExpressionFunctionTest. It
// | defines the expression function tests
// =============================================================================


class TestCommonExpressionFunctionW : public TestCommonExpressionFunction
{

  CPPUNIT_TEST_SUB_SUITE(TestCommonExpressionFunctionW, TestCommonExpressionFunction);
  CPPUNIT_TEST_SUITE_END();

public:
            TestCommonExpressionFunctionW ();
    virtual ~TestCommonExpressionFunctionW ();


protected:

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

    // AddXYZMFeature:
    //      The function adds a new object for the specified class. The values
    //      being added are predefined based on the predefined schema.

    void AddXYZMFeature (FdoIConnection *current_connection,
                         FdoString      *class_name);

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

    virtual FdoFeatureClass *CreateFdoFeatureClass (FdoString *class_name,
                                                    bool has_elevation = true,
                                                    bool has_measure = false);

    //  CreateFdoClass:
    //      The function creates a predefined non-feature class of the given
    //      name.

    virtual FdoClass *CreateFdoClass (FdoString *class_name);

    //  CreateGeometricProperty:
    //      The function creates the named geometry property and returns it
    //      back to the calling routine.

    virtual FdoGeometricPropertyDefinition *CreateGeometricProperty (
                                                    FdoString *property_name,
                                                    bool has_elevation = true,
                                                    bool has_measure = false);

    // InsertTestFeatures:
    //      The function inserts the test features for the filter optinmizer
    //      test cases.

    virtual void InsertTestFeatures (FdoIConnection *connection);

    //  SetupUnitTestEnvironment:
    //      The function controls the establishing of the test environment in
    //      the FDO data store the provided connection identifies.

    virtual void SetupUnitTestEnvironment (FdoIConnection *current_connection);

    // ========================= GEOMETRY FUNCTIONS ==========================

    // TestXYZMFunction:
    //      The function executes the test for the expression engine functions
    //      X, Y, Z and M.

    virtual void TestXYZMFunction ();

};  //  class TestCommonExpressionFunctionW

#endif
