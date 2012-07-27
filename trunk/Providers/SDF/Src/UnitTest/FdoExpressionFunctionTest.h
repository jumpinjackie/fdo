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

#include <TestCommonExpressionFunctionW.h>

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

class FdoExpressionFunctionTest : public TestCommonExpressionFunctionW
{

  CPPUNIT_TEST_SUB_SUITE(FdoExpressionFunctionTest, TestCommonExpressionFunctionW);
   // =========================================
   // ====       SPECIAL UNIT TESTS        ====
   // =========================================
   // CPPUNIT_TEST(ModuloOperation);
   // CPPUNIT_TEST(SoundexFilter);
   // CPPUNIT_TEST(SubstrOperation);
   // CPPUNIT_TEST(ToDateOperation);
   // =========================================
   // =========================================
   // ====         OPTIMIZER TEST          ====
   // =========================================
   CPPUNIT_TEST(RunOptimizerTest);
   // test Optimizer from EE
   CPPUNIT_TEST(RunOptimizerTest2);
   // =========================================
   // =========================================
   // ====     Embeded INTERSECTS TESTS    ====
   // =========================================
   CPPUNIT_TEST(RunEmbeddedIntersectsTest);
   CPPUNIT_TEST(RunEmbeddedIntersectsTest2);
   CPPUNIT_TEST_SUITE_END();

public:
            FdoExpressionFunctionTest ();
    virtual ~FdoExpressionFunctionTest ();

protected:

    void RunOptimizerTest2();

    //  Connect:
    //      The function establishes the connection to the data store used in
    //      the executed test suites.

	void Connect ();

    //-------------------------------------------------------------------------
    //                     Combination Unit Test Functions
    //-------------------------------------------------------------------------

    void NumberToStringComparison ();

    void RoundNumberToStringComparison ();

    //-------------------------------------------------------------------------
    //                     Embeded INTERSECTS Test Functions
    //-------------------------------------------------------------------------

    void RunEmbeddedIntersectsTest ();
    void RunEmbeddedIntersectsTest2 ();

    //-------------------------------------------------------------------------
    //                       Special Unit Test Functions
    //-------------------------------------------------------------------------

    //  ModuloOperation:
    //      The function executes a modulo operation on multiple rows. This is
    //      to verify that the returned data is accurate for each row.

    void ModuloOperation ();

    //  RunOptimizerTest:
    //      The function tests the expression optimizer for filters.

    void RunOptimizerTest();

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

};  //  class FdoExpressionFunctionTest

#endif
