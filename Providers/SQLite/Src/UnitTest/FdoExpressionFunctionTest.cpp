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

#ifdef _WIN32
static const wchar_t* EXPFCT_TEST_FILE = L"..\\..\\TestData\\ExpFctTest.slt";
#else
#include <unistd.h>
static const wchar_t* EXPFCT_TEST_FILE = L"../../TestData/ExpFctTest.slt";
#endif

CPPUNIT_TEST_SUITE_REGISTRATION(FdoExpressionFunctionTest);
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(FdoExpressionFunctionTest, "FdoExpressionFunctionTest");

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
}  //  ~FdoExpressionFunctionTest ()

void FdoExpressionFunctionTest::Connect ()

// +---------------------------------------------------------------------------
// | The function establishes the connection to the data store used in the
// | executed test suites.
// +---------------------------------------------------------------------------

{

    // Establish the connection.

    m_connection = UnitTestUtil::CreateConnection();

    UnitTestUtil::OpenConnection( EXPFCT_TEST_FILE, true, true, m_connection );

}  //  Connect ()

void FdoExpressionFunctionTest::RunAllExpFctTests()
{
    bool error_found = false;

    // TODO: Add more function tests or use base RunAllExpFctTests
    // when SQLite provider supports all standard functions.

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

}

double FdoExpressionFunctionTest::GetNullOrdinate()
{
    return GetNaNOrdinate();
}

