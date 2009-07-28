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

#ifndef TESTEXPRESSIONFUNCTION_H
#define TESTEXPRESSIONFUNCTION_H

#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>
#include <TestCommonExpressionFunctionW.h>
#include "ArcSDETests.h"


// =============================================================================
// | The file contains the definition of the class FdoExpressionFunctionTest. It
// | defines the expression function tests
// =============================================================================


// ---------------------------------------------------------------------------
//                                  Constants                                 
// ---------------------------------------------------------------------------

class TestExpressionFunction : public TestCommonExpressionFunctionW
{

  CPPUNIT_TEST_SUB_SUITE(TestExpressionFunction, TestCommonExpressionFunctionW);
  CPPUNIT_TEST_SUITE_END();

public:
            TestExpressionFunction ();
    virtual ~TestExpressionFunction ();

protected:

    //  Connect:
    //      The function establishes the connection to the data store used in
    //      the executed test suites.

	virtual void Connect ();


    // ========================= GEOMETRY FUNCTIONS ==========================

    // TestXYZMFunction:
    //      The function executes the test for the expression engine functions
    //      X, Y, Z and M.

    void RunAllExpFctTests();

    virtual void setUp();

    virtual void tearDown();

    virtual void DropTestSchema( FdoString* schema_name );

    virtual void CreateSC();

    // Create or update a spatial context:
    void CreateOrUpdateSpatialContext(FdoIConnection *conn, FdoStringP& scName, double dXMin, double dYMin, double dXMax, double dYMax, FdoString* scCoordSys);

    virtual FdoFeatureClass *CreateFdoFeatureClass (FdoString *class_name,
                                                    bool has_elevation = true,
                                                    bool has_measure = false);

    virtual void AddFeature (FdoIConnection *current_connection,
                 FdoString      *class_name,
                 int            index);

    virtual double GetNullOrdinate();

    virtual double GetNaNOrdinate();

    ArcSDETests mSDETests;

    FdoStringP mSC;

};  //  class TestExpressionFunction

#endif
