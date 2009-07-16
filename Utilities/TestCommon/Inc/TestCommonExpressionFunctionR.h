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

#ifndef TESTCOMMONEXPRESSIONFUNCTIONR_H
#define TESTCOMMONEXPRESSIONFUNCTIONR_H

#include "TestCommonExpressionFunction.h"

// =============================================================================
// | The file contains the definition of the class FdoExpressionFunctionTest. It
// | defines the expression function tests
// =============================================================================


// ---------------------------------------------------------------------------
//                                  Constants                                 
// ---------------------------------------------------------------------------

class TestCommonExpressionFunctionR : public TestCommonExpressionFunction
{

  CPPUNIT_TEST_SUB_SUITE(TestCommonExpressionFunctionR, TestCommonExpressionFunction);
  CPPUNIT_TEST_SUITE_END();

public:
            TestCommonExpressionFunctionR ();
    virtual ~TestCommonExpressionFunctionR ();


protected:

    // ----------------------------------------------------------------------------
    // --                      Alternative tests for providers that don't        --
    // --                      ApplySchema. Provider must supply pre-created     --
    // --                      test data.                                        --
    // ----------------------------------------------------------------------------

    void doTestXYZMRdOnly( FdoString* className, FdoString* geomName );

};  //  class TestCommonExpressionFunctionR

#endif
