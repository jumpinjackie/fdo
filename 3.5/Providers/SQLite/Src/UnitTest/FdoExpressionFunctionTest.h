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

class FdoExpressionFunctionTest : public TestCommonExpressionFunctionW
{

  CPPUNIT_TEST_SUB_SUITE(FdoExpressionFunctionTest, TestCommonExpressionFunctionW);
  CPPUNIT_TEST_SUITE_END();

public:
            FdoExpressionFunctionTest ();
    virtual ~FdoExpressionFunctionTest ();


protected:

    //  Connect:
    //      The function establishes the connection to the data store used in
    //      the executed test suites.

	void Connect ();

    void RunAllExpFctTests();

    virtual double GetNullOrdinate();

    void TestDateStringConv();

    void TestConcat();

};  //  class FdoExpressionFunctionTest

#endif
